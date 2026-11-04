/**
 *    Name:        assets.c
 *    Description: the game's assets loaded into the board's memories and the programs' data, by name
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The formats are the extractor's (tools/extract_assets.py and the modules it names):
 *      tiles/tilesets.txt, sprites/sprites.txt   each sheet's codes ("0010-001F 0123"), its cells 16 a row;
 *                                                 the sheets are 8-bit indexed PNGs whose low 4 bits are
 *                                                 the pens (tools/tmnt_graphics.py)
 *      sounds/                                    title.wav (16-bit samples of the title music's 3.10
 *                                                 floating point words), pcm.txt and pcm/ (the K007232's
 *                                                 bytes), speech.txt and speech/ (the uPD7759's, in the
 *                                                 'rawd' chunks) (tools/sound_assets.py)
 *      data/                                      the 68000 program's pieces: layout.txt (each one's name,
 *                                                 address and file), the files' [NAME] sections of typed
 *                                                 items, functions.txt (the code places they name)
 *                                                 (tools/data_assets.py)
 *      sound_data.txt, sound_layout.txt, sound_functions.txt   the sound program's (tools/sound_data_assets.py),
 *                                                 but its music and its songs table:
 *      sound/music/                               music.txt and the songs' Standard MIDI Files, which give the
 *                                                 sound program's pieces sound_data.txt has not
 *                                                 (tools/sound_music.py; runtime/src/music.c reads them)
 *    Every name a piece gives for an address is a piece's or a function's, maybe with +0xNN.
 *    The assets pack (assets_pack.c) is what this loads, as it is in memory: assets_pack_put writes it and
 *    assets_pack_get loads it back in place of the files.
 */
#include "game/assets.h"

#include "game/data.h"
#include "music.h"
#include "png.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROBLEM_SIZE    256
#define COLUMNS         16
#define TILE_BYTES      32
#define SPRITE_BYTES    128

typedef struct
{
	char *name;
	uint32_t address;
	char *group;            /* the file it is in (NULL: the sound program's) */
	uint8_t *data;
	size_t size;
	uint8_t *reversed;      /* the 68000 program's: its bytes the other way round (assets_record_at) */
	int texted;             /* a text file has its section (the sound program's others are the music's) */
} piece;

typedef struct
{
	char *name;
	uint32_t address;
} named_address;

typedef struct
{
	piece *pieces;          /* sorted by address */
	size_t count;
	named_address *names;   /* the pieces' and the functions', sorted by name */
	size_t name_count;
	int little_endian;      /* the Z80's */
} data_region;

static char g_problem[PROBLEM_SIZE];
static data_region g_program, g_sound;

/* ---- files ---- */

static char *read_file(const char *path, size_t *size)
{
	FILE *file = fopen(path, "rb");
	char *data;
	long length;

	if (!file)
		return NULL;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	data = malloc((size_t)length + 1);
	if (data && fread(data, 1, (size_t)length, file) != (size_t)length)
	{
		free(data);
		data = NULL;
	}
	fclose(file);
	if (data)
	{
		data[length] = 0;
		if (size)
			*size = (size_t)length;
	}
	return data;
}

static char *read_asset(const char *folder, const char *path)
{
	char full[1024];
	char *text;

	snprintf(full, sizeof(full), "%s/%s", folder, path);
	text = read_file(full, NULL);
	if (!text)
		snprintf(g_problem, sizeof(g_problem), "no %s in the assets", path);
	return text;
}

/* The next line of 'text' from '*at' into 'line' (without its end); 0 at the end of the text */
static int next_line(const char **at, char *line, size_t size)
{
	size_t length = 0;

	if (!**at)
		return 0;
	while (**at && **at != '\n')
	{
		if (length + 1 < size && **at != '\r')
			line[length++] = **at;
		(*at)++;
	}
	if (**at == '\n')
		(*at)++;
	line[length] = 0;
	return 1;
}

/* ---- names ---- */

static int by_name(const void *first, const void *second)
{
	return strcmp(((const named_address *)first)->name, ((const named_address *)second)->name);
}

static int by_address(const void *first, const void *second)
{
	const uint32_t a = ((const piece *)first)->address, b = ((const piece *)second)->address;

	return a < b ? -1 : a > b;
}

static void add_name(data_region *region, const char *name, uint32_t address)
{
	region->names = realloc(region->names, (region->name_count + 1) * sizeof(*region->names));
	region->names[region->name_count].name = malloc(strlen(name) + 1);
	strcpy(region->names[region->name_count].name, name);
	region->names[region->name_count].address = address;
	region->name_count++;
}

/* NAME or NAME+0xNN as an address; -1 when there is no such name */
static int64_t address_of(const data_region *region, const char *text)
{
	char base[128];
	const char *plus = strchr(text, '+');
	const size_t length = plus ? (size_t)(plus - text) : strlen(text);
	named_address key, *found;

	if (length >= sizeof(base))
		return -1;
	memcpy(base, text, length);
	base[length] = 0;
	key.name = base;
	found = bsearch(&key, region->names, region->name_count, sizeof(*found), by_name);
	if (!found)
		return -1;
	return (int64_t)found->address + (plus ? (int64_t)strtoul(plus + 1, NULL, 16) : 0);
}

/* ---- a piece's items ---- */

typedef struct
{
	uint8_t *data;
	size_t size, capacity;
} writer;

static void put(writer *into, uint32_t value, int bytes, int little_endian)
{
	if (into->size + (size_t)bytes > into->capacity)
	{
		into->capacity = (into->capacity + (size_t)bytes) * 2;
		into->data = realloc(into->data, into->capacity);
	}
	for (int index = 0; index < bytes; index++)
	{
		const int shift = little_endian ? 8 * index : 8 * (bytes - 1 - index);

		into->data[into->size++] = (uint8_t)(value >> shift);
	}
}

/* An address item: a name, or a number (0x...) */
static int address_item(const data_region *region, const char *token, uint32_t *value)
{
	int64_t found;

	if (token[0] == '0' && (token[1] == 'x' || token[1] == 'X'))
	{
		*value = (uint32_t)strtoul(token, NULL, 16);
		return 1;
	}
	found = address_of(region, token);
	if (found < 0)
		return 0;
	*value = (uint32_t)found;
	return 1;
}

/* One line of items into 'into': 0 and the problem if it is not one */
static int read_item(const data_region *region, writer *into, char *line, const char *where)
{
	const int little = region->little_endian;
	char *token = strtok(line, " \t");
	char *rest;

	if (!token)
		return 1;
	if (!strcmp(token, "text"))
	{
		char *close;

		/* between the first quote and the last (the text may hold quotes) */
		rest = strtok(NULL, "");
		if (!rest || !(rest = strchr(rest, '"')) || (close = strrchr(rest, '"')) == rest)
			goto bad;
		for (rest++; rest < close; rest++)
		{
			if (*rest == '\\' && rest + 3 < close)
			{
				char digits[3] = { rest[2], rest[3], 0 };

				put(into, (uint32_t)strtoul(digits, NULL, 16), 1, little);
				rest += 3;
			}
			else
				put(into, (uint8_t)*rest, 1, little);
		}
		return 1;
	}
	if (!strcmp(token, "u8") || !strcmp(token, "u16") || !strcmp(token, "s16") || !strcmp(token, "u32"))
	{
		const int bytes = token[1] == '8' ? 1 : token[1] == '3' ? 4 : 2;
		const int is_signed = token[0] == 's';

		while ((rest = strtok(NULL, " \t")) != NULL)
			put(into, is_signed ? (uint32_t)strtol(rest, NULL, 10) : (uint32_t)strtoul(rest, NULL, 16), bytes, little);
		return 1;
	}
	if (!strcmp(token, "ref"))
	{
		while ((rest = strtok(NULL, " \t")) != NULL)
		{
			uint32_t value;

			if (!address_item(region, rest, &value))
				goto unknown;
			put(into, value, little ? 2 : 4, little);
		}
		return 1;
	}
	if (!strcmp(token, "frame"))
	{
		/* duration, count, sprite list, box, attack, then the attack box's offsets if it has one */
		char *parts[16];
		int count = 0;
		uint32_t sprites;

		while (count < 16 && (parts[count] = strtok(NULL, " \t")) != NULL)
			count++;
		if (count < 5)
			goto bad;
		if (!address_item(region, parts[2], &sprites))
			goto unknown;
		put(into, (uint32_t)atoi(parts[0]), 1, little);
		put(into, (uint32_t)atoi(parts[1]), 1, little);
		put(into, sprites, 4, little);
		put(into, (uint32_t)strtoul(parts[3], NULL, 16), 2, little);
		for (int index = 4; index < count; index++)
			put(into, (uint32_t)atoi(parts[index]), 2, little);
		return 1;
	}
	if (!strcmp(token, "end") || !strcmp(token, "jump"))
	{
		const int jump = token[0] == 'j';
		char *flag = strtok(NULL, " \t"), *target = jump ? strtok(NULL, " \t") : NULL;
		uint32_t value;

		if (!flag || (jump && !target))
			goto bad;
		put(into, 0, 1, little);
		put(into, (uint32_t)atoi(flag), 1, little);
		if (jump)
		{
			if (!address_item(region, target, &value))
				goto unknown;
			put(into, value, 4, little);
		}
		return 1;
	}
	if (!strcmp(token, "sprite"))
	{
		char *parts[5];

		for (int index = 0; index < 5; index++)
			if ((parts[index] = strtok(NULL, " \t")) == NULL)
				goto bad;
		put(into, (uint32_t)strtoul(parts[0], NULL, 16), 2, little);
		put(into, (uint32_t)strtoul(parts[1], NULL, 16), 1, little);
		put(into, (uint32_t)strtoul(parts[2], NULL, 16), 1, little);
		put(into, (uint32_t)atoi(parts[3]), 2, little);
		put(into, (uint32_t)atoi(parts[4]), 2, little);
		return 1;
	}
	if (!strcmp(token, "colours"))
	{
		while ((rest = strtok(NULL, " \t")) != NULL)
		{
			char digits[3] = { 0, 0, 0 };
			uint32_t component[3];

			if (rest[0] != '#' || strlen(rest) < 7)
				goto bad;
			for (int index = 0; index < 3; index++)
			{
				digits[0] = rest[1 + 2 * index];
				digits[1] = rest[2 + 2 * index];
				component[index] = (uint32_t)strtoul(digits, NULL, 16) >> 3;
			}
			put(into, (rest[7] == '+' ? 0x8000u : 0) | component[2] << 10 | component[1] << 5 | component[0], 2, little);
		}
		return 1;
	}
bad:
	snprintf(g_problem, sizeof(g_problem), "%s: an item the assets do not know", where);
	return 0;
unknown:
	snprintf(g_problem, sizeof(g_problem), "%s: a name no piece or function has", where);
	return 0;
}

/* ---- the programs' data ---- */

/* 'layout' ("NAME ADDRESS [FILE]") and 'functions' ("NAME ADDRESS") into the region's pieces and names */
static int read_layout(data_region *region, const char *layout, const char *functions)
{
	char line[512];
	const char *at = layout;

	while (next_line(&at, line, sizeof(line)))
	{
		char name[256], address[32], group[128] = "";

		if (!line[0] || line[0] == '#' || sscanf(line, "%255s %31s %127s", name, address, group) < 2)
			continue;
		region->pieces = realloc(region->pieces, (region->count + 1) * sizeof(*region->pieces));
		piece *entry = &region->pieces[region->count++];
		memset(entry, 0, sizeof(*entry));
		entry->name = malloc(strlen(name) + 1);
		strcpy(entry->name, name);
		entry->address = (uint32_t)strtoul(address, NULL, 16);
		if (group[0])
		{
			entry->group = malloc(strlen(group) + 1);
			strcpy(entry->group, group);
		}
		add_name(region, name, entry->address);
	}
	at = functions;
	while (next_line(&at, line, sizeof(line)))
	{
		char name[256], address[32];

		if (line[0] && line[0] != '#' && sscanf(line, "%255s %31s", name, address) == 2)
			add_name(region, name, (uint32_t)strtoul(address, NULL, 16));
	}
	qsort(region->names, region->name_count, sizeof(*region->names), by_name);
	return 1;
}

static piece *piece_named(data_region *region, const char *name)
{
	for (size_t index = 0; index < region->count; index++)
		if (!strcmp(region->pieces[index].name, name))
			return &region->pieces[index];
	return NULL;
}

/* A file of [NAME] sections: each section's items into the piece of that name */
static int read_sections(data_region *region, const char *text, const char *file)
{
	char line[4096], where[512];
	const char *at = text;
	piece *current = NULL;
	writer into = { NULL, 0, 0 };

	while (1)
	{
		const int more = next_line(&at, line, sizeof(line));

		if (!more || line[0] == '[')
		{
			if (current)
			{
				current->data = into.data;
				current->size = into.size;
				into.data = NULL;
				into.size = into.capacity = 0;
			}
			if (!more)
				break;
			{
				char name[256];
				const char *close = strchr(line, ']');
				const size_t length = close ? (size_t)(close - line - 1) : 0;

				if (!close || length >= sizeof(name))
				{
					snprintf(g_problem, sizeof(g_problem), "%s: a section's name is not closed", file);
					return 0;
				}
				memcpy(name, line + 1, length);
				name[length] = 0;
				if (!(current = piece_named(region, name)))
				{
					snprintf(g_problem, sizeof(g_problem), "%s: [%s] is in no layout", file, name);
					return 0;
				}
				current->texted = 1;
			}
			continue;
		}
		if (current && (line[0] == ' ' || line[0] == '\t'))
		{
			snprintf(where, sizeof(where), "%s [%s]", file, current->name);
			if (!read_item(region, &into, line, where))
				return 0;
		}
	}
	free(into.data);
	return 1;
}

static int read_program_data(const char *folder)
{
	char *layout = read_asset(folder, "data/layout.txt"), *functions = read_asset(folder, "data/functions.txt");
	int ok = layout && functions && read_layout(&g_program, layout, functions);

	free(layout);
	free(functions);
	if (!ok)
		return 0;
	/* each file once */
	for (size_t index = 0; index < g_program.count && ok; index++)
	{
		const char *group = g_program.pieces[index].group;
		int seen = 0;
		char path[256];
		char *text;

		for (size_t before = 0; before < index && !seen; before++)
			seen = !strcmp(g_program.pieces[before].group, group);
		if (seen)
			continue;
		snprintf(path, sizeof(path), "data/%s.txt", group);
		if (!(text = read_asset(folder, path)))
			return 0;
		ok = read_sections(&g_program, text, path);
		free(text);
	}
	qsort(g_program.pieces, g_program.count, sizeof(*g_program.pieces), by_address);
	/* the pieces the other way round: a big-endian value of the program's is a value of the host's there */
	for (size_t index = 0; index < g_program.count && ok; index++)
	{
		piece *each = &g_program.pieces[index];

		if (!(each->reversed = malloc(each->size ? each->size : 1u)))
		{
			snprintf(g_problem, sizeof(g_problem), "no memory for the program's data [%s]", each->name);
			return 0;
		}
		for (size_t at = 0; at < each->size; at++)
			each->reversed[each->size - 1u - at] = each->data[at];
	}
	return ok;
}

/* ---- the sound program's music ---- */

static int sound_address_of(void *host, const char *name, uint32_t *address)
{
	const int64_t found = address_of(&g_sound, name);

	(void)host;
	if (found < 0)
		return 0;
	*address = (uint32_t)found;
	return 1;
}

/* the name of the piece an address is in (the last that starts at or before it), +0xNN */
static void sound_name_of(void *host, uint32_t address, char *text, size_t size)
{
	const piece *found = NULL;

	(void)host;
	for (size_t index = 0; index < g_sound.count && g_sound.pieces[index].address <= address; index++)
		found = &g_sound.pieces[index];
	if (!found)
		snprintf(text, size, "%04X", (unsigned)address);
	else if (found->address == address)
		snprintf(text, size, "%s", found->name);
	else
		snprintf(text, size, "%s+0x%X", found->name, (unsigned)(address - found->address));
}

/* The songs' MIDI files (runtime/src/music.c) give the sound program's pieces no text has: each one the music's
   bytes from its address up to the next piece's */
static int read_music(const char *folder)
{
	const piece *lengths = piece_named(&g_sound, "note_lengths"), *songs = piece_named(&g_sound, "songs");
	uint8_t *image = calloc(MUSIC_SPACE, 1), *given = calloc(MUSIC_SPACE, 1);
	music_places places;
	char path[1024];
	const char *problem;
	int ok = 1;

	if (!image || !given)
	{
		snprintf(g_problem, sizeof(g_problem), "no memory for the music");
		ok = 0;
	}
	else if (!lengths || !lengths->data || !songs)
	{
		snprintf(g_problem, sizeof(g_problem), "the sound program's note_lengths or songs is in no layout");
		ok = 0;
	}
	if (ok)
	{
		places.address_of = sound_address_of;
		places.name_of = sound_name_of;
		places.host = NULL;
		places.lengths = lengths->data;
		places.length_count = lengths->size;
		places.songs_table = songs->address;
		snprintf(path, sizeof(path), "%s/sound/music", folder);
		if ((problem = music_read(path, &places, image, given)) != NULL)
		{
			snprintf(g_problem, sizeof(g_problem), "sound/music: %s", problem);
			ok = 0;
		}
	}
	for (size_t index = 0; index < g_sound.count && ok; index++)
	{
		piece *each = &g_sound.pieces[index];
		const uint32_t stop = index + 1 < g_sound.count ? g_sound.pieces[index + 1].address : MUSIC_SPACE;
		uint32_t length = 0;

		if (each->texted)
			continue;
		while (each->address + length < stop && each->address + length < MUSIC_SPACE && given[each->address + length])
			length++;
		if (!length || !(each->data = malloc(length)))
		{
			snprintf(g_problem, sizeof(g_problem), "the sound program's %s is in no asset", each->name);
			ok = 0;
			break;
		}
		memcpy(each->data, image + each->address, length);
		each->size = length;
	}
	free(image);
	free(given);
	return ok;
}

static int read_sound_data(const char *folder)
{
	char *layout = read_asset(folder, "sound_layout.txt"), *functions = read_asset(folder, "sound_functions.txt");
	char *text = read_asset(folder, "sound_data.txt");
	int ok;

	g_sound.little_endian = 1;
	ok = layout && functions && text && read_layout(&g_sound, layout, functions)
		&& read_sections(&g_sound, text, "sound_data.txt");
	free(layout);
	free(functions);
	free(text);
	qsort(g_sound.pieces, g_sound.count, sizeof(*g_sound.pieces), by_address);
	return ok && read_music(folder);
}

static const uint8_t *data_at(const data_region *region, uint32_t address)
{
	size_t low = 0, high = region->count;

	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (region->pieces[middle].address <= address)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0)
		return NULL;
	const piece *found = &region->pieces[low - 1];
	return address < found->address + found->size ? found->data + (address - found->address) : NULL;
}

const uint8_t *assets_data_at(uint32_t address)
{
	return data_at(&g_program, address & 0xFFFFFF);
}

const void *assets_record_at(uint32_t address, uint32_t size)
{
	const data_region *region = &g_program;
	size_t low = 0, high = region->count;
	const piece *found;

	address &= 0xFFFFFF;
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (region->pieces[middle].address <= address)
			low = middle + 1;
		else
			high = middle;
	}
	if (low == 0)
		return NULL;
	found = &region->pieces[low - 1];
	if (!found->reversed || address + size > found->address + found->size)
		return NULL;
	return found->reversed + (found->size - size - (address - found->address));
}

const uint8_t *assets_sound_data_at(uint32_t address)
{
	return data_at(&g_sound, address & 0xFFFF);
}

const uint8_t *assets_piece(const char *name, size_t *size)
{
	const piece *found = piece_named(&g_program, name);

	if (size)
		*size = found ? found->size : 0;
	return found ? found->data : NULL;
}

static void lay_out(const data_region *region, uint8_t *image, uint8_t *placed, size_t size)
{
	memset(image, 0, size);
	if (placed)
		memset(placed, 0, size);
	for (size_t index = 0; index < region->count; index++)
	{
		const piece *each = &region->pieces[index];
		size_t length;

		if (each->address >= size)
			continue;
		length = each->size <= size - each->address ? each->size : size - each->address;
		memcpy(image + each->address, each->data, length);
		if (placed)
			memset(placed + each->address, 1, length);
	}
}

void assets_program_image(uint8_t *image, uint8_t *placed, size_t size)
{
	lay_out(&g_program, image, placed, size);
}

void assets_sound_image(uint8_t *image, uint8_t *placed, size_t size)
{
	lay_out(&g_sound, image, placed, size);
}

const uint8_t *assets_sound_piece(const char *name, size_t *size)
{
	const piece *found = piece_named(&g_sound, name);

	if (size)
		*size = found ? found->size : 0;
	return found ? found->data : NULL;
}

/* ---- the tiles and the sprites ---- */

/* An 8 x 8 block of pens from the image at (left, top) into its 32 planar bytes (4 a row, the most
   significant pen bit in the last) */
static void put_block(const png_image *image, uint32_t left, uint32_t top, uint8_t *out)
{
	for (uint32_t row = 0; row < 8; row++)
		for (int lane = 0; lane < 4; lane++)
		{
			uint8_t value = 0;

			for (uint32_t x = 0; x < 8; x++)
				value |= (uint8_t)(((image->pixels[(top + row) * image->width + left + x] >> lane) & 1) << (7 - x));
			out[row * 4 + (uint32_t)lane] = value;
		}
}

/* A listing of sheets ("NAME RANGES") under 'sub': each sheet's cells into 'memory', 'unit' bytes a code */
static int read_sheets(const char *folder, const char *sub, const char *listing, int size, uint8_t *memory, uint32_t codes)
{
	char path[1024], line[65536];
	char *text;
	const char *at;

	snprintf(path, sizeof(path), "%s/%s", sub, listing);
	if (!(text = read_asset(folder, path)))
		return 0;
	at = text;
	while (next_line(&at, line, sizeof(line)))
	{
		char *sheet, *range;
		png_image image;
		const char *problem;
		uint32_t cell = 0;

		if (!line[0] || line[0] == '#' || !(sheet = strtok(line, " \t")))
			continue;
		snprintf(path, sizeof(path), "%s/%s/%s.png", folder, sub, sheet);
		if ((problem = png_read(path, &image)) != NULL)
		{
			snprintf(g_problem, sizeof(g_problem), "%s/%s.png: %s", sub, sheet, problem);
			free(text);
			return 0;
		}
		while ((range = strtok(NULL, " \t")) != NULL)
		{
			char *dash = strchr(range, '-');
			const uint32_t first = (uint32_t)strtoul(range, NULL, 16);
			const uint32_t last = dash ? (uint32_t)strtoul(dash + 1, NULL, 16) : first;

			for (uint32_t code = first; code <= last && code < codes; code++, cell++)
			{
				const uint32_t left = (cell % COLUMNS) * (uint32_t)size, top = (cell / COLUMNS) * (uint32_t)size;

				if (left + (uint32_t)size > image.width || top + (uint32_t)size > image.height)
				{
					snprintf(g_problem, sizeof(g_problem), "%s/%s.png is smaller than its codes", sub, sheet);
					png_free(&image);
					free(text);
					return 0;
				}
				if (size == 8)
					put_block(&image, left, top, memory + (size_t)code * TILE_BYTES);
				else
				{
					uint8_t *out = memory + (size_t)code * SPRITE_BYTES;

					put_block(&image, left, top, out);
					put_block(&image, left + 8, top, out + 32);
					put_block(&image, left, top + 8, out + 64);
					put_block(&image, left + 8, top + 8, out + 96);
				}
			}
		}
		png_free(&image);
	}
	free(text);
	return 1;
}

/* ---- the samples ---- */

/* A WAV's chunk 'id' (its bytes and size), from a file read whole */
static const uint8_t *riff_chunk(const uint8_t *file, size_t size, const char *id, size_t *length)
{
	size_t at = 12;

	while (at + 8 <= size)
	{
		const size_t chunk = (size_t)file[at + 4] | (size_t)file[at + 5] << 8 | (size_t)file[at + 6] << 16
			| (size_t)file[at + 7] << 24;

		if (!memcmp(file + at, id, 4))
		{
			*length = chunk <= size - at - 8 ? chunk : size - at - 8;
			return file + at + 8;
		}
		at += 8 + chunk + (chunk & 1);
	}
	return NULL;
}

static uint8_t *read_wav(const char *folder, const char *path, size_t *size)
{
	char full[1024];
	uint8_t *data;

	snprintf(full, sizeof(full), "%s/%s", folder, path);
	data = (uint8_t *)read_file(full, size);
	if (!data)
		snprintf(g_problem, sizeof(g_problem), "no %s in the assets", path);
	return data;
}

/* ymfm's encode_fp: a 16-bit sample as the 3.10 floating point the title music's ROM holds */
static uint16_t encode_fp(int32_t value)
{
	uint32_t scan = (uint32_t)(value ^ (value >> 31)) << 17;
	int exponent = 7;

	while (exponent > 0 && !(scan & 0x80000000u))
	{
		scan <<= 1;
		exponent--;
	}
	if (exponent < 1)
		exponent = 1;
	return (uint16_t)((((uint32_t)exponent << 10) | ((uint32_t)(value >> (exponent - 1)) & 0x3FF)) ^ 0x200);
}

static int read_samples(const char *folder)
{
	size_t size, length;
	uint8_t *file;
	const uint8_t *chunk;
	char *text, line[1024];
	const char *at;

	/* the title music */
	if (!(file = read_wav(folder, "sounds/title.wav", &size)))
		return 0;
	chunk = riff_chunk(file, size, "data", &length);
	for (size_t index = 0; chunk && index < length / 2 && index < GAME_TITLE_SIZE / 2; index++)
	{
		const int16_t sample = (int16_t)(chunk[2 * index] | chunk[2 * index + 1] << 8);
		const uint16_t word = (uint16_t)(encode_fp(sample) << 3);

		game_title[2 * index] = (uint8_t)word;
		game_title[2 * index + 1] = (uint8_t)(word >> 8);
	}
	free(file);

	/* the K007232's */
	if (!(text = read_asset(folder, "sounds/pcm.txt")))
		return 0;
	at = text;
	while (next_line(&at, line, sizeof(line)))
	{
		char number[16], path[64];
		unsigned first, stop;

		if (!line[0] || line[0] == '#' || sscanf(line, "%15s %x %x", number, &first, &stop) != 3)
			continue;
		snprintf(path, sizeof(path), "sounds/pcm/sample_%s.wav", number);
		if (!(file = read_wav(folder, path, &size)))
		{
			free(text);
			return 0;
		}
		chunk = riff_chunk(file, size, "data", &length);
		for (size_t index = 0; chunk && index < length && first + index < stop && first + index < GAME_PCM_SIZE; index++)
			game_pcm[first + index] = chunk[index];
		free(file);
	}
	free(text);

	/* the uPD7759's: its header, then its samples one after the other */
	if (!(text = read_asset(folder, "sounds/speech.txt")))
		return 0;
	{
		size_t out = 0;
		int samples = 0;

		at = text;
		while (next_line(&at, line, sizeof(line)))
		{
			char *key = strtok(line, " \t"), *value;

			if (!key || key[0] == '#')
				continue;
			if (!strcmp(key, "last") && (value = strtok(NULL, " \t")) != NULL)
				game_speech[out++] = (uint8_t)atoi(value);
			else if (!strcmp(key, "signature") || !strcmp(key, "before"))
			{
				value = strtok(NULL, " \t");
				for (size_t index = 0; value && value[index] && value[index + 1] && out < GAME_SPEECH_SIZE; index += 2)
				{
					char digits[3] = { value[index], value[index + 1], 0 };

					game_speech[out++] = (uint8_t)strtoul(digits, NULL, 16);
				}
			}
			else if (!strcmp(key, "offsets"))
				while ((value = strtok(NULL, " \t")) != NULL && out + 2 <= GAME_SPEECH_SIZE)
				{
					const uint32_t offset = (uint32_t)strtoul(value, NULL, 16);

					game_speech[out++] = (uint8_t)(offset >> 9);
					game_speech[out++] = (uint8_t)(offset >> 1);
					samples++;
				}
		}
		free(text);
		for (int number = 0;; number++)
		{
			char path[64], full[1024];
			FILE *exists;

			snprintf(path, sizeof(path), "sounds/speech/speech_%02d.wav", number);
			snprintf(full, sizeof(full), "%s/%s", folder, path);
			if (!(exists = fopen(full, "rb")))
				break;
			fclose(exists);
			if (!(file = read_wav(folder, path, &size)))
				return 0;
			chunk = riff_chunk(file, size, "rawd", &length);
			for (size_t index = 0; chunk && index < length && out < GAME_SPEECH_SIZE; index++)
				game_speech[out++] = chunk[index];
			free(file);
		}
		(void)samples;
	}
	return 1;
}

/* ---- the pack's part (assets_pack.c) ---- */

static int put32(FILE *output, uint32_t value)
{
	const uint8_t bytes[4] = { (uint8_t)value, (uint8_t)(value >> 8), (uint8_t)(value >> 16), (uint8_t)(value >> 24) };

	return fwrite(bytes, 1, 4, output) == 4;
}

/* 'size' bytes and zeros up to the next 4 */
static int put_bytes(FILE *output, const void *bytes, size_t size)
{
	static const uint8_t zeros[4] = { 0 };
	const size_t padding = (4u - size % 4u) % 4u;

	return (size == 0 || fwrite(bytes, 1, size, output) == size) && fwrite(zeros, 1, padding, output) == padding;
}

static int put_string(FILE *output, const char *text)
{
	const size_t length = text ? strlen(text) : 0;

	return put32(output, (uint32_t)length) && put_bytes(output, text ? text : "", length);
}

static int put_region(FILE *output, const data_region *region)
{
	int ok = put32(output, (uint32_t)region->count) && put32(output, (uint32_t)region->name_count);

	for (size_t index = 0; index < region->count && ok; index++)
	{
		const piece *each = &region->pieces[index];

		ok = put_string(output, each->name) && put32(output, each->address) && put_string(output, each->group)
			&& put32(output, (uint32_t)each->texted) && put32(output, (uint32_t)each->size) && put_bytes(output, each->data, each->size);
	}
	for (size_t index = 0; index < region->name_count && ok; index++)
		ok = put_string(output, region->names[index].name) && put32(output, region->names[index].address);
	return ok;
}

int assets_pack_put(FILE *output)
{
	const struct
	{
		const uint8_t *memory;
		uint32_t size;
	} memories[] = { { game_tiles, GAME_TILES_SIZE }, { game_sprites, GAME_SPRITES_SIZE }, { game_title, GAME_TITLE_SIZE },
		{ game_pcm, GAME_PCM_SIZE }, { game_speech, GAME_SPEECH_SIZE } };
	int ok = 1;

	for (size_t index = 0; index < sizeof(memories) / sizeof(memories[0]) && ok; index++)
		ok = put32(output, memories[index].size) && put_bytes(output, memories[index].memory, memories[index].size);
	return ok && put_region(output, &g_program) && put_region(output, &g_sound);
}

typedef struct
{
	const uint8_t *at, *end;
} pack_reader;

static int get32(pack_reader *reader, uint32_t *value)
{
	if (reader->end - reader->at < 4)
		return 0;
	*value = reader->at[0] | (uint32_t)reader->at[1] << 8 | (uint32_t)reader->at[2] << 16 | (uint32_t)reader->at[3] << 24;
	reader->at += 4;
	return 1;
}

static const uint8_t *get_bytes(pack_reader *reader, size_t size)
{
	const uint8_t *bytes = reader->at;
	const size_t aligned = size + (4u - size % 4u) % 4u;

	if ((size_t)(reader->end - reader->at) < aligned)
		return NULL;
	reader->at += aligned;
	return bytes;
}

/* a string of the pack, copied (NULL when it is cut short; 'empty_is_null' gives NULL for one of no letters) */
static char *get_string(pack_reader *reader, int *ok, int empty_is_null)
{
	uint32_t length;
	const uint8_t *bytes;
	char *copy;

	if (!*ok || !get32(reader, &length) || !(bytes = get_bytes(reader, length)) || !(copy = malloc(length + 1u)))
	{
		*ok = 0;
		return NULL;
	}
	memcpy(copy, bytes, length);
	copy[length] = 0;
	if (empty_is_null && length == 0)
	{
		free(copy);
		return NULL;
	}
	return copy;
}

static int get_region(pack_reader *reader, data_region *region, int little_endian)
{
	uint32_t count, names;
	int ok = get32(reader, &count) && get32(reader, &names);

	region->little_endian = little_endian;
	if (!ok || !(region->pieces = calloc(count + 1u, sizeof(*region->pieces)))
		|| !(region->names = calloc(names + 1u, sizeof(*region->names))))
		return 0;
	for (uint32_t index = 0; index < count && ok; index++)
	{
		piece *each = &region->pieces[index];
		uint32_t value, size;
		const uint8_t *bytes;

		each->name = get_string(reader, &ok, 0);
		ok = ok && get32(reader, &each->address);
		each->group = get_string(reader, &ok, 1);
		ok = ok && get32(reader, &value) && get32(reader, &size) && (bytes = get_bytes(reader, size)) != NULL
			&& (each->data = malloc(size + 1u)) != NULL;
		if (!ok)
			break;
		each->texted = (int)value;
		each->size = size;
		memcpy(each->data, bytes, size);
		if (!little_endian)
		{
			/* (the program's: its bytes the other way round, as read_program_data has them) */
			if (!(each->reversed = malloc(size + 1u)))
				return 0;
			for (size_t at = 0; at < size; at++)
				each->reversed[size - 1u - at] = each->data[at];
		}
		region->count++;
	}
	for (uint32_t index = 0; index < names && ok; index++)
	{
		region->names[index].name = get_string(reader, &ok, 0);
		ok = ok && get32(reader, &region->names[index].address);
		if (ok)
			region->name_count++;
	}
	return ok;
}

const char *assets_pack_get(const uint8_t *bytes, size_t size)
{
	uint8_t *const memories[] = { game_tiles, game_sprites, game_title, game_pcm, game_speech };
	const uint32_t sizes[] = { GAME_TILES_SIZE, GAME_SPRITES_SIZE, GAME_TITLE_SIZE, GAME_PCM_SIZE, GAME_SPEECH_SIZE };
	pack_reader reader = { bytes, bytes + size };

	for (size_t index = 0; index < sizeof(memories) / sizeof(memories[0]); index++)
	{
		uint32_t length;
		const uint8_t *memory;

		if (!get32(&reader, &length) || length != sizes[index] || !(memory = get_bytes(&reader, length)))
			return "the assets pack is cut short";
		memcpy(memories[index], memory, length);
	}
	if (!get_region(&reader, &g_program, 0) || !get_region(&reader, &g_sound, 1))
		return "the assets pack is cut short";
	return NULL;
}

/* ---- all of them ---- */

const char *assets_load(const char *folder)
{
	g_problem[0] = 0;
	if (!read_sheets(folder, "tiles", "tilesets.txt", 8, game_tiles, GAME_TILES_SIZE / TILE_BYTES)
		|| !read_sheets(folder, "sprites", "sprites.txt", 16, game_sprites, GAME_SPRITES_SIZE / SPRITE_BYTES)
		|| !read_samples(folder) || !read_program_data(folder) || !read_sound_data(folder))
		return g_problem[0] ? g_problem : "the assets do not load";
	return NULL;
}
