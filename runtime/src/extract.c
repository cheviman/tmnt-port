/**
 *    Name:        extract.c
 *    Description: the game's first run: its assets out of the player's ROM set, the files extract_assets.py
 *                 writes, byte for byte (game/extract.h)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A player of the port has no Python: the game reads the ROM set itself (rom_set.c) and writes the folder
 *    tools/extract_assets.py writes, every file the same, so the game loads it as it loads the Python's
 *    (runtime/src/assets.c) and its assets can be edited the same way. What the Python tools work out from the static
 *    analysis and the walks (which bytes are which piece, their names and formats, the sheets' codes and palettes)
 *    comes from extract_layout.c, which tools/extract_layout.py writes with no byte of the ROM; the bytes come from
 *    the player's set. The parts:
 *      extract_graphics.c   tiles/ and sprites/: the sheets (PNG) and their listings (tools/extract_assets.py)
 *      extract_samples.c    sounds/: the title music, the K007232's samples, the uPD7759's speech (sound_assets.py)
 *      this file            data/: the 68000 program's data as text, its layout and functions (data_assets.py);
 *                           sound_data.txt, sound_layout.txt, sound_functions.txt (sound_data_assets.py)
 *      extract_music.c      sound/music/: the songs as Standard MIDI Files and music.txt (sound_music.py)
 *    Text files are written as Python writes them, their line ends the system's. data/layout.txt (the file the game
 *    looks for in a folder of assets) and GAME_ASSETS_VERSION_FILE are written last: a folder without them is not
 *    whole. The ROM set's regions are cleared when the assets are out:
 *    the game runs on its assets.
 */
#include "game/extract.h"

#include "extract_internal.h"
#include "extract_layout.h"
#include "game/data.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#define WRITERS_VERSION 1u      /* the writers' own: one more when what they write changes */
#define BYTES_A_LINE    16
#define WORDS_A_LINE    8
#define REFS_A_LINE     4

char extract_problem[EXTRACT_PATH_SIZE + 128];

/* ---- buffers and files ---- */

static void grow(extract_buffer *into, size_t more)
{
	char *grown;

	if (into->failed || into->size + more <= into->capacity)
		return;
	into->capacity = (into->size + more) * 2 + 4096;
	if (!(grown = realloc(into->data, into->capacity)))
	{
		into->failed = 1;
		return;
	}
	into->data = grown;
}

void buffer_bytes(extract_buffer *into, const void *bytes, size_t size)
{
	grow(into, size);
	if (into->failed)
		return;
	memcpy(into->data + into->size, bytes, size);
	into->size += size;
}

void buffer_byte(extract_buffer *into, uint8_t byte)
{
	buffer_bytes(into, &byte, 1);
}

void buffer_text(extract_buffer *into, const char *format, ...)
{
	va_list arguments;
	int length;

	va_start(arguments, format);
	length = vsnprintf(NULL, 0, format, arguments);
	va_end(arguments);
	if (length < 0)
	{
		into->failed = 1;
		return;
	}
	grow(into, (size_t)length + 1);
	if (into->failed)
		return;
	va_start(arguments, format);
	vsnprintf(into->data + into->size, (size_t)length + 1, format, arguments);
	va_end(arguments);
	into->size += (size_t)length;
}

void buffer_free(extract_buffer *buffer)
{
	free(buffer->data);
	buffer->data = NULL;
	buffer->size = buffer->capacity = 0;
}

static int write_file(const char *folder, const char *relative, const void *bytes, size_t size, const char *mode)
{
	char path[EXTRACT_PATH_SIZE];
	FILE *file;
	int written;

	snprintf(path, sizeof(path), "%s/%s", folder, relative);
	if (!(file = fopen(path, mode)))
	{
		snprintf(extract_problem, sizeof(extract_problem), "%s cannot be written", path);
		return 0;
	}
	written = size == 0 || fwrite(bytes, 1, size, file) == size;
	if (fclose(file) != 0 || !written)
	{
		snprintf(extract_problem, sizeof(extract_problem), "%s cannot be written", path);
		return 0;
	}
	return 1;
}

int extract_write_text(const char *folder, const char *relative, const extract_buffer *text)
{
	if (text->failed)
	{
		snprintf(extract_problem, sizeof(extract_problem), "no memory for %s", relative);
		return 0;
	}
	return write_file(folder, relative, text->data, text->size, "w");
}

int extract_write_bytes(const char *folder, const char *relative, const void *bytes, size_t size)
{
	return write_file(folder, relative, bytes, size, "wb");
}

static void make_folder(const char *path)
{
#ifdef _WIN32
	_mkdir(path);
#else
	mkdir(path, 0755);
#endif
}

/* 'path' made, and every folder it is in that is not there */
static void make_folders(const char *path)
{
	char partial[EXTRACT_PATH_SIZE];
	const size_t length = strlen(path);

	if (length >= sizeof(partial))
		return;
	for (size_t at = 1; at <= length; at++)
		if (at == length || path[at] == '/' || path[at] == '\\')
		{
			memcpy(partial, path, at);
			partial[at] = 0;
			make_folder(partial);
		}
}

/* ---- the names of addresses (data_assets.Names, sound_data_assets.Names) ---- */

typedef struct
{
	uint32_t first, stop;
	const char *name;
	size_t order;           /* its place in the layout's list: of two at one place, the later one names it */
} named_span;

typedef struct
{
	named_span *pieces;     /* by their first byte */
	size_t count;
	const extract_function *functions;
	size_t function_count;
	uint8_t *used;          /* the functions a name was asked of (data/functions.txt lists them) */
} namer;

static int by_first(const void *one, const void *other)
{
	const named_span *a = one, *b = other;

	if (a->first != b->first)
		return a->first < b->first ? -1 : 1;
	return a->order < b->order ? -1 : a->order > b->order;
}

/* NAME or NAME+0xNN of an address a function or a piece holds, written into 'text'; 0 when none has it */
static int name_of(const namer *names, uint32_t value, char *text, size_t size)
{
	size_t low = 0, high = names->function_count;

	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (names->functions[middle].address < value)
			low = middle + 1;
		else
			high = middle;
	}
	if (low < names->function_count && names->functions[low].address == value)
	{
		if (names->used)
			names->used[low] = 1;
		snprintf(text, size, "%s", names->functions[low].name);
		return 1;
	}
	low = 0;
	high = names->count;
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (names->pieces[middle].first <= value)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0)
		return 0;
	{
		const named_span *found = &names->pieces[low - 1];

		if (value >= found->stop)
			return 0;
		if (value == found->first)
			snprintf(text, size, "%s", found->name);
		else
			snprintf(text, size, "%s+0x%X", found->name, (unsigned)(value - found->first));
	}
	return 1;
}

/* the names' pieces sorted by their first byte; of two at one place, the later one (a dict's) */
static int sort_names(namer *names)
{
	size_t kept = 0;

	qsort(names->pieces, names->count, sizeof(*names->pieces), by_first);
	for (size_t index = 0; index < names->count; index++)
	{
		if (kept > 0 && names->pieces[kept - 1].first == names->pieces[index].first)
			kept--;
		names->pieces[kept++] = names->pieces[index];
	}
	names->count = kept;
	return 1;
}

/* ---- the 68000 program's data (tools/data_assets.py) ---- */

/* the image's bytes as Python's slices give them: past its end, none */
static uint32_t program_byte(uint32_t at)
{
	return at < GAME_PROGRAM_SIZE ? game_program[at] : 0u;
}

static uint32_t big(uint32_t at, int bytes)
{
	uint32_t value = 0;

	for (int index = 0; index < bytes && at + (uint32_t)index < GAME_PROGRAM_SIZE; index++)
		value = value << 8 | game_program[at + (uint32_t)index];
	return value;
}

static int32_t signed16(uint32_t at)
{
	const uint32_t value = big(at, 2);

	return value & 0x8000u ? (int32_t)value - 0x10000 : (int32_t)value;
}

/* "ref NAME" for an address a piece or a function has (the top byte 0), else "u32 0x...": whether it was a ref */
static int ref_item(const namer *names, uint32_t value, char *text, size_t size)
{
	char name[256];

	if (!(value >> 24) && name_of(names, value & 0xFFFFFFu, name, sizeof(name)))
	{
		snprintf(text, size, "ref %s", name);
		return 1;
	}
	snprintf(text, size, "u32 0x%08X", (unsigned)value);
	return 0;
}

/* NAME, or the address in Python's hex() */
static void address_text(const namer *names, uint32_t value, char *text, size_t size)
{
	if (!name_of(names, value, text, size))
		snprintf(text, size, "0x%x", (unsigned)value);
}

/* 'count' numbers ('bytes' wide, from 'at' on, 'step' bytes apart) in lines of 'per_line' after 'kind' */
static void numbers(extract_buffer *out, const char *kind, uint32_t at, uint32_t count, int bytes, uint32_t per_line)
{
	for (uint32_t index = 0; index < count; index++)
	{
		const uint32_t value = big(at + (uint32_t)bytes * index, bytes);

		if (index % per_line == 0)
			buffer_text(out, "  %s", kind);
		buffer_text(out, bytes == 1 ? " 0x%02X" : " 0x%04X", (unsigned)value);
		if (index % per_line == per_line - 1 || index + 1 == count)
			buffer_text(out, "\n");
	}
}

static void text_literal(extract_buffer *out, uint32_t first, uint32_t stop)
{
	buffer_text(out, "\"");
	for (uint32_t at = first; at < stop; at++)
	{
		const uint32_t byte = program_byte(at);

		if (byte >= 0x20 && byte < 0x7F && byte != 0x22 && byte != 0x5C)
			buffer_byte(out, (uint8_t)byte);
		else
			buffer_text(out, "\\x%02X", (unsigned)byte);
	}
	buffer_text(out, "\"");
}

/* the lines of a span (data_assets.write_piece), each after two spaces */
static void write_span(extract_buffer *out, const extract_span *span, const namer *names)
{
	const uint32_t start = span->first, end = span->stop;
	char text[300], other[300];

	switch (span->format)
	{
	case EXTRACT_FRAME:
		address_text(names, big(start + 2, 4), text, sizeof(text));
		buffer_text(out, "  frame %u %u %s 0x%04X %d", (unsigned)program_byte(start), (unsigned)program_byte(start + 1),
			text, (unsigned)big(start + 6, 2), (int)signed16(start + 8));
		if (end - start == 18)
			buffer_text(out, " %d %d %d %d", (int)signed16(start + 10), (int)signed16(start + 12), (int)signed16(start + 14),
				(int)signed16(start + 16));
		buffer_text(out, "\n");
		break;
	case EXTRACT_END:
		buffer_text(out, "  end %u\n", (unsigned)program_byte(start + 1));
		break;
	case EXTRACT_JUMP:
		address_text(names, big(start + 2, 4), text, sizeof(text));
		buffer_text(out, "  jump %u %s\n", (unsigned)program_byte(start + 1), text);
		break;
	case EXTRACT_SPRITES:
		for (uint32_t at = start; at < end; at += 8)
			buffer_text(out, "  sprite 0x%04X 0x%02X 0x%02X %d %d\n", (unsigned)big(at, 2), (unsigned)program_byte(at + 2),
				(unsigned)program_byte(at + 3), (int)signed16(at + 4), (int)signed16(at + 6));
		break;
	case EXTRACT_PALETTE:
		for (uint32_t at = start; at + 1 < end; at += 32)
		{
			buffer_text(out, "  colours");
			for (uint32_t entry = at; entry < (at + 32 < end ? at + 32 : end); entry += 2)
			{
				const uint32_t value = big(entry, 2);

				buffer_text(out, " #%02X%02X%02X%s", (unsigned)((value & 0x1F) << 3), (unsigned)(((value >> 5) & 0x1F) << 3),
					(unsigned)(((value >> 10) & 0x1F) << 3), value & 0x8000u ? "+" : "");
			}
			buffer_text(out, "\n");
		}
		break;
	case EXTRACT_TEXT:
		buffer_text(out, "  text ");
		text_literal(out, start, end);
		buffer_text(out, "\n");
		break;
	case EXTRACT_POINTERS:
	{
		/* the entries 4 a line when they are all names, else a line each */
		uint32_t entries = end > start + 3 ? (end - 3 - start + 3) / 4 : 0;

		for (uint32_t group = 0; group < entries; group += REFS_A_LINE)
		{
			const uint32_t count = entries - group < REFS_A_LINE ? entries - group : REFS_A_LINE;
			int all = 1;

			for (uint32_t index = 0; index < count; index++)
				all &= ref_item(names, big(start + 4 * (group + index), 4), text, sizeof(text));
			if (all)
			{
				buffer_text(out, "  ref");
				for (uint32_t index = 0; index < count; index++)
				{
					ref_item(names, big(start + 4 * (group + index), 4), text, sizeof(text));
					buffer_text(out, " %s", text + 4);
				}
				buffer_text(out, "\n");
			}
			else
				for (uint32_t index = 0; index < count; index++)
				{
					ref_item(names, big(start + 4 * (group + index), 4), other, sizeof(other));
					buffer_text(out, "  %s\n", other);
				}
		}
		if ((end - start) % 4)
			numbers(out, "u8", start + (end - start) / 4 * 4, (end - start) % 4, 1, BYTES_A_LINE);
		break;
	}
	case EXTRACT_WORDS:
		numbers(out, "u16", start, end > start + 1 ? (end - 1 - start + 1) / 2 : 0, 2, WORDS_A_LINE);
		if ((end - start) % 2)
			buffer_text(out, "  u8 0x%02X\n", (unsigned)program_byte(end - 1));
		break;
	case EXTRACT_LONGS:
		for (uint32_t at = start; at + 3 < end; at += 4)
		{
			ref_item(names, big(at, 4), text, sizeof(text));
			buffer_text(out, "  %s\n", text);
		}
		break;
	default:
		numbers(out, "u8", start, end - start, 1, BYTES_A_LINE);
		break;
	}
}

/* data/: a file a subject, and functions.txt; its layout.txt into 'layout', which the first run writes last (the
   file the game looks for in a folder of assets: a folder the first run did not finish has none) */
static int write_program_data(const char *folder, extract_buffer *layout_out)
{
	namer names = { NULL, 0, extract_functions, extract_function_count, NULL };
	extract_buffer *files = calloc(extract_data_group_count, sizeof(*files));
	extract_buffer layout = { 0 }, functions = { 0 };
	int ok = files != NULL;

	names.pieces = malloc((extract_data_piece_count + 1) * sizeof(*names.pieces));
	names.used = calloc(extract_function_count + 1, 1);
	if (!names.pieces || !names.used)
		ok = 0;
	for (size_t index = 0; index < extract_data_piece_count && ok; index++)
	{
		const extract_data_piece *piece = &extract_data_pieces[index];

		if (piece->named)
			names.pieces[names.count++] = (named_span){ piece->spans[0].first, piece->spans[piece->count - 1].stop, piece->name, index };
	}
	ok = ok && sort_names(&names);
	for (size_t group = 0; group < extract_data_group_count && ok; group++)
		buffer_text(&files[group], "# %s: pieces of the program's data, by name\n", extract_data_groups[group]);
	buffer_text(&layout, "# each piece and where the program had it (for --check)\n");
	for (size_t index = 0; index < extract_data_piece_count && ok; index++)
	{
		const extract_data_piece *piece = &extract_data_pieces[index];
		extract_buffer *out = &files[piece->group];

		buffer_text(out, "[%s]\n", piece->name);
		for (uint32_t span = 0; span < piece->count; span++)
			write_span(out, &piece->spans[span], &names);
		buffer_text(&layout, "%s %06X %s\n", piece->name, (unsigned)piece->spans[0].first, extract_data_groups[piece->group]);
	}
	buffer_text(&functions, "# the program's places in its code the data names, and their addresses\n");
	for (size_t index = 0; index < extract_function_count && ok; index++)
		if (names.used[index])
			buffer_text(&functions, "%s %06X\n", extract_functions[index].name, (unsigned)extract_functions[index].address);
	for (size_t group = 0; group < extract_data_group_count && ok; group++)
	{
		char relative[256];

		snprintf(relative, sizeof(relative), "data/%s.txt", extract_data_groups[group]);
		ok = extract_write_text(folder, relative, &files[group]);
	}
	ok = ok && extract_write_text(folder, "data/functions.txt", &functions);
	*layout_out = layout;
	layout.data = NULL;
	if (!files || !names.pieces || !names.used)
		snprintf(extract_problem, sizeof(extract_problem), "no memory for the program's data");
	for (size_t group = 0; files && group < extract_data_group_count; group++)
		buffer_free(&files[group]);
	free(files);
	buffer_free(&layout);
	buffer_free(&functions);
	free(names.pieces);
	free(names.used);
	return ok;
}

/* ---- the sound program's data (tools/sound_data_assets.py) ---- */

static uint32_t sound_byte(uint32_t at)
{
	return at < GAME_SOUND_SIZE ? game_sound[at] : 0u;
}

static void sound_numbers(extract_buffer *out, const char *kind, uint32_t at, uint32_t count, int bytes, uint32_t per_line)
{
	for (uint32_t index = 0; index < count; index++)
	{
		const uint32_t place = at + (uint32_t)bytes * index;
		const uint32_t value = bytes == 1 ? sound_byte(place) : sound_byte(place) | sound_byte(place + 1) << 8;

		if (index % per_line == 0)
			buffer_text(out, "  %s", kind);
		buffer_text(out, bytes == 1 ? " 0x%02X" : " 0x%04X", (unsigned)value);
		if (index % per_line == per_line - 1 || index + 1 == count)
			buffer_text(out, "\n");
	}
}

static int write_sound_data(const char *folder)
{
	namer names = { NULL, 0, extract_sound_functions, extract_sound_function_count, NULL };
	extract_buffer data = { 0 }, layout = { 0 }, functions = { 0 };
	int ok;

	names.pieces = malloc((extract_sound_piece_count + 1) * sizeof(*names.pieces));
	if (!names.pieces)
	{
		snprintf(extract_problem, sizeof(extract_problem), "no memory for the sound program's data");
		return 0;
	}
	for (size_t index = 0; index < extract_sound_piece_count; index++)
		names.pieces[names.count++] = (named_span){ extract_sound_pieces[index].first, extract_sound_pieces[index].stop,
			extract_sound_pieces[index].name, index };
	sort_names(&names);
	buffer_text(&data, "# the sound program's data: its pieces by name (the music and the songs table: sound/music)\n");
	buffer_text(&layout, "# each piece and where the sound program had it: the address the game gives it\n");
	for (size_t index = 0; index < extract_sound_piece_count; index++)
	{
		const extract_sound_piece *piece = &extract_sound_pieces[index];
		const uint32_t first = piece->first, stop = piece->stop;

		buffer_text(&layout, "%s %04X\n", piece->name, (unsigned)first);
		if (piece->format == EXTRACT_SOUND_MUSIC)
			continue;
		buffer_text(&data, "[%s]  # %s\n", piece->name, piece->kind);
		if (piece->format == EXTRACT_SOUND_ADDRESSES)
			for (uint32_t at = first; at + 1 < stop; at += 2)
			{
				const uint32_t value = sound_byte(at) | sound_byte(at + 1) << 8;
				char name[256];

				if (name_of(&names, value, name, sizeof(name)))
					buffer_text(&data, "  ref %s\n", name);
				else
					buffer_text(&data, "  u16 0x%04X\n", (unsigned)value);
			}
		else if (piece->format == EXTRACT_SOUND_WORDS)
			sound_numbers(&data, "u16", first, stop > first + 1 ? (stop - first) / 2 : 0, 2, WORDS_A_LINE);
		else
			sound_numbers(&data, "u8", first, stop - first, 1, BYTES_A_LINE);
		if (piece->format != EXTRACT_SOUND_BYTES && (stop - first) % 2)
			buffer_text(&data, "  u8 0x%02X\n", (unsigned)sound_byte(stop - 1));
	}
	buffer_text(&functions, "# the sound program's places in its code the data names, and their addresses\n");
	for (size_t index = 0; index < extract_sound_function_count; index++)
		buffer_text(&functions, "%s %04X\n", extract_sound_functions[index].name, (unsigned)extract_sound_functions[index].address);
	ok = extract_write_text(folder, "sound_data.txt", &data) && extract_write_text(folder, "sound_layout.txt", &layout)
		&& extract_write_text(folder, "sound_functions.txt", &functions);
	buffer_free(&data);
	buffer_free(&layout);
	buffer_free(&functions);
	free(names.pieces);
	return ok;
}

/* ---- the first run ---- */

uint32_t game_assets_version(void)
{
	return extract_layout_version ^ WRITERS_VERSION * 0x9E3779B9u;
}

const char *game_assets_extract(const char *source, const char *folder)
{
	static const char *const folders[] = { "tiles", "sprites", "sounds/pcm", "sounds/speech", "data", "sound/music" };
	const char *wrong = game_rom_set_load(source);
	char path[EXTRACT_PATH_SIZE];
	extract_buffer layout = { 0 };
	int ok;

	if (wrong)
		return wrong;
	extract_problem[0] = 0;
	for (size_t index = 0; index < sizeof(folders) / sizeof(folders[0]); index++)
	{
		snprintf(path, sizeof(path), "%s/%s", folder, folders[index]);
		make_folders(path);
	}
	/* (until the folder is whole again, the game does not take it for its assets: data/layout.txt and the version
	   file are written last) */
	snprintf(path, sizeof(path), "%s/" GAME_ASSETS_VERSION_FILE, folder);
	remove(path);
	snprintf(path, sizeof(path), "%s/data/layout.txt", folder);
	remove(path);
	ok = extract_graphics(folder) && extract_samples(folder) && write_program_data(folder, &layout)
		&& write_sound_data(folder) && extract_music(folder) && extract_write_text(folder, "data/layout.txt", &layout);
	buffer_free(&layout);
	if (ok)
	{
		extract_buffer version = { 0 };

		buffer_text(&version, "%08X\n", (unsigned)game_assets_version());
		ok = extract_write_text(folder, GAME_ASSETS_VERSION_FILE, &version);
		buffer_free(&version);
	}
	/* (the game runs on its assets: nothing of the ROM set stays in its memory) */
	memset(game_program, 0, GAME_PROGRAM_SIZE);
	memset(game_sound, 0, GAME_SOUND_SIZE);
	memset(game_proms, 0, GAME_PROMS_SIZE);
	return ok ? NULL : extract_problem[0] ? extract_problem : "the assets cannot be written";
}
