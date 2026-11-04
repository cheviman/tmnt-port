/**
 *    Name:        music.c
 *    Description: the songs' Standard MIDI Files back into the sound program's music and songs table
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The format is tools/sound_music.py's, whose docstring says it whole; this is its way back (read, rebuild),
 *    the same steps in C:
 *      - music.txt lists the files, a song each: its first word, the file's name without .mid;
 *      - a file is a Standard MIDI File; the tracks with an 'S' event (FF 7F LEN 7D 'S' NAME) are the song's 9
 *        channels, FM 1-7 then PCM A and B; 'N' gives the songs table's entries it plays, 'U' bytes no track
 *        shows; the other events of the file (tempo, names, programs, controllers) are the writer's, for players;
 *      - a channel's events at a tick are its commands ('C': the driver's bytes, an address last as a place's
 *        name) and then a step: a note (Note On; its Note Off where the driver keys it off), a rest ('R'), a held
 *        note ('H': the bytes after E5, and its Note On), or 'Z', where the file stops showing it; a step lasts
 *        to the channel's next one;
 *      - the channels are read together, by tick and then by channel, each keeping the driver's record (octave,
 *        transpositions, drum mode, gate, loops and returns) and its place, from 'S': a command's bytes go there
 *        and the place moves on as the driver's does; a note's byte is its length's code (note_lengths) and the
 *        pitch nibble its key gives; a byte that comes back twice has to be the same.
 */
#include "music.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TICKS_A_QUARTER 24      /* a driver's tick a file's tick at this division */
#define SONG_TRACKS     9
#define TAG             0x7D    /* the non-commercial manufacturer id of the driver's meta events */
#define NAME_SIZE       128
#define WHERE_SIZE      200
#define NO_TICK         (-1)

static char g_problem[512];

static int fail(const char *format, ...)
{
	va_list arguments;

	va_start(arguments, format);
	vsnprintf(g_problem, sizeof(g_problem), format, arguments);
	va_end(arguments);
	return 0;
}

static void *grow(void *list, size_t *capacity, size_t needed, size_t unit)
{
	if (needed <= *capacity)
		return list;
	*capacity = needed * 2 + 16;
	return realloc(list, *capacity * unit);
}

/* ---- the driver ---- */

/* a command's bytes (the octave and attenuation steps, E0-FF but E5, the held note) */
static uint32_t command_length(uint8_t command)
{
	if (command < 0xE0)
		return 1;
	switch (command)
	{
	case 0xED: case 0xF0: case 0xF8: case 0xFA:
		return 3;
	case 0xF1: case 0xF6:
		return 4;
	case 0xEC:
		return 6;
	case 0xF3: case 0xF5: case 0xF7: case 0xF9: case 0xFB: case 0xFF:
		return 1;
	default:
		return 2;
	}
}

/* where a command's address is (its last two bytes), or 0 when it has none */
static uint32_t address_at(uint8_t command)
{
	switch (command)
	{
	case 0xEC: return 4;
	case 0xF1: case 0xF6: return 2;
	case 0xF0: case 0xF8: case 0xFA: return 1;
	default: return 0;
	}
}

/* read_transpose: 12 x bits 4-6 plus bits 0-3 semitones, two bytes of fm_note_pitches a semitone, down with bit 7 */
static uint8_t transposition(uint8_t byte)
{
	const uint8_t value = (uint8_t)(24 * ((byte & 0x70) >> 4) + 2 * (byte & 0x0F));

	return byte & 0x80 ? (uint8_t)(0u - value) : value;
}

/* a music channel's record, what the way back needs of it */
typedef struct
{
	int number, pcm, done;
	uint32_t place;
	uint8_t octave, transpose, loop_transpose, mode, gate;
	uint8_t loop_count, loop_2_count, repeat_count, loop_exit_count, transposed_loop_count;
	uint32_t loop_start, loop_2_start, repeat_return, repeat_part, call_return, call_2_return;
} channel;

/* The command 'data' at the channel's place ('target' the address it carries): what it does to the record and to
   the music's transposition; where the track goes on, -1 when it ends */
static int64_t execute(channel *ch, uint8_t *music_transpose, const uint8_t *data, uint32_t target)
{
	const uint32_t at = ch->place;
	const uint8_t command = data[0];
	const uint8_t argument = command >= 0xE0 && command_length(command) > 1 ? data[1] : 0;
	uint8_t count;

	if (command < 0x10)
	{
		ch->octave = ch->pcm ? (uint8_t)(command & 0x0F) : (uint8_t)(24 * (command & 0x0F));
		return at + 1;
	}
	switch (command)
	{
	case 0xE0:
		ch->mode = argument;
		break;
	case 0xE4:
		ch->gate = argument;
		break;
	case 0xE8:
		*music_transpose = transposition(argument);
		break;
	case 0xE9:
		ch->transpose = transposition(argument);
		break;
	case 0xEC:
		count = ch->transposed_loop_count;
		if (count == 1)
		{
			ch->transposed_loop_count = 0;
			ch->loop_transpose = 0;
			return at + 6;
		}
		if (count == 0)
		{
			count = argument;
			ch->loop_transpose = 0;
		}
		ch->transposed_loop_count = (uint8_t)(count - 1);
		ch->loop_transpose = (uint8_t)(ch->loop_transpose + transposition(data[2]));
		return target;
	case 0xF0:
		return target;
	case 0xF1:
		count = ch->loop_exit_count;
		if (count == 1)
		{
			ch->loop_exit_count = 0;
			return target;
		}
		ch->loop_exit_count = (uint8_t)((count ? count : argument) - 1);
		return at + 4;
	case 0xF2:
		ch->loop_count = argument;
		ch->loop_start = at + 2;
		return at + 2;
	case 0xF3:
		count = ch->loop_count;
		ch->loop_count = (uint8_t)(count - 1);
		return count == 1 ? at + 1 : ch->loop_start;
	case 0xF4:
		ch->loop_2_count = argument;
		ch->loop_2_start = at + 2;
		return at + 2;
	case 0xF5:
		count = ch->loop_2_count;
		ch->loop_2_count = (uint8_t)(count - 1);
		return count == 1 ? at + 1 : ch->loop_2_start;
	case 0xF6:
		ch->repeat_count = argument;
		ch->repeat_return = at + 4;
		ch->repeat_part = target;
		return target;
	case 0xF7:
		count = ch->repeat_count;
		ch->repeat_count = (uint8_t)(count - 1);
		return count == 1 ? ch->repeat_return : ch->repeat_part;
	case 0xF8:
		ch->call_return = at + 3;
		return target;
	case 0xF9:
		return ch->call_return;
	case 0xFA:
		ch->call_2_return = at + 3;
		return target;
	case 0xFB:
		return ch->call_2_return;
	case 0xFF:
		return -1;
	default:
		break;
	}
	return at + command_length(command);
}

/* pcm_drums 1-15 as General MIDI percussion keys (tools/sound_music.py's DRUM_KEYS) */
static const int DRUM_KEYS[16] = { -1, 36, 38, 40, 37, 42, 41, 43, 45, 47, 48, 50, 64, 63, 61, 60 };

static int drums(const channel *ch)
{
	return ch->pcm && (ch->mode & 1);
}

/* the key of a note whose pitch nibble is 'nibble'; -1 when none has it */
static int key_of(const channel *ch, uint8_t music_transpose, int nibble)
{
	int key;

	if (drums(ch))
		return nibble > 0 && nibble < 16 ? DRUM_KEYS[nibble] : -1;
	if (ch->pcm)
	{
		const int count = (uint8_t)(ch->octave + 1) ? (uint8_t)(ch->octave + 1) : 256;

		key = ((nibble + 12 * count) & 0xFF) - 1;
	}
	else
		key = ((music_transpose + 2 * nibble + ch->octave + ch->transpose + ch->loop_transpose) & 0xFF) / 2 + 11;
	return key >= 0 && key <= 127 ? key : -1;
}

/* the pitch nibble that gives 'key'; -1 when none does */
static int nibble_of(const channel *ch, uint8_t music_transpose, int key)
{
	int nibble;

	if (drums(ch))
	{
		for (nibble = 1; nibble < 16; nibble++)
			if (DRUM_KEYS[nibble] == key)
				return nibble;
		return -1;
	}
	if (ch->pcm)
	{
		const int count = (uint8_t)(ch->octave + 1) ? (uint8_t)(ch->octave + 1) : 256;

		nibble = (key + 1 - 12 * count) & 0xFF;
	}
	else
	{
		if (key < 11)
			return -1;
		nibble = ((2 * (key - 11) - music_transpose - ch->octave - ch->transpose - ch->loop_transpose) & 0xFF) / 2;
	}
	return nibble <= 15 && key_of(ch, music_transpose, nibble) == key ? nibble : -1;
}

/* when the driver keys a note of 'length' ticks off: an FM one by its gate, a tick at least */
static int64_t released(const channel *ch, int64_t length)
{
	int64_t off;

	if (ch->pcm || !ch->gate)
		return length;
	off = (((length & 0xFF) * ch->gate) >> 8) + 1;
	return length > off ? length - off : 1;
}

/* a note's or a rest's length's code: 1-D but 7; 0 when none has the length */
static int code_of(int64_t length, const music_places *places)
{
	for (size_t code = 1; code <= 13 && code <= places->length_count; code++)
		if (code != 7 && places->lengths[code - 1] == length)
			return (int)code;
	return 0;
}

/* a held note's bytes after E5: up to the first byte with a 0 nibble (b1 always) */
static uint32_t held_length(const uint8_t *data, uint32_t size)
{
	uint32_t at = 1;

	while (at < size && (data[at] & 0xF0) && (data[at] & 0x0F))
		at++;
	return at + 1 < size ? at + 1 : size;
}

static int length_of(int code, const music_places *places)
{
	return code >= 1 && (size_t)code <= places->length_count ? places->lengths[code - 1] : -1;
}

/* a held note's ticks as the driver adds them up (in a byte, 0 being 256); -1 for a code note_lengths has not */
static int64_t held_ticks(const uint8_t *data, uint32_t size, const music_places *places)
{
	int64_t total = length_of(data[0] >> 4, places);

	if (total < 0)
		return -1;
	for (uint32_t at = 1; at < size; at++)
	{
		int part;

		if (!(data[at] & 0xF0))
			break;
		if ((part = length_of(data[at] >> 4, places)) < 0)
			return -1;
		total += part;
		if (!(data[at] & 0x0F))
			break;
		if ((part = length_of(data[at] & 0x0F, places)) < 0)
			return -1;
		total += part;
	}
	return (total & 0xFF) ? (total & 0xFF) : 256;
}

/* ---- the files ---- */

enum { EVENT_ON, EVENT_OFF, EVENT_META };

typedef struct
{
	int64_t tick;
	size_t order;               /* its place in the track */
	uint8_t kind, type, key;    /* type: a meta event's */
	const uint8_t *data;
	uint32_t size;
} event;

typedef struct
{
	event *list;
	size_t count, capacity;
} events;

static int add_event(events *track, int64_t tick, uint8_t kind, uint8_t type, uint8_t key, const uint8_t *data,
	uint32_t size)
{
	event *added;

	track->list = grow(track->list, &track->capacity, track->count + 1, sizeof(event));
	if (!track->list)
		return fail("no memory for a MIDI file's events");
	added = &track->list[track->count];
	added->order = track->count++;
	added->tick = tick;
	added->kind = kind;
	added->type = type;
	added->key = key;
	added->data = data;
	added->size = size;
	return 1;
}

/* a variable-length quantity (4 bytes at most) */
static int read_number(const uint8_t *body, size_t size, size_t *place, uint32_t *value, const char *where,
	size_t track)
{
	*value = 0;
	for (int bytes = 0; bytes < 4; bytes++)
	{
		if (*place >= size)
			return fail("%s: track %zu ends inside an event", where, track);
		*value = *value << 7 | (body[*place] & 0x7Fu);
		if (!(body[(*place)++] & 0x80))
			return 1;
	}
	return fail("%s: track %zu: a variable-length quantity of more than 4 bytes", where, track);
}

/* A Standard MIDI File's tracks: Note Ons, Note Offs and meta events (the rest left out) */
static int parse(const uint8_t *data, size_t size, const char *where, uint32_t *division, events **tracks,
	size_t *track_count)
{
	size_t at, capacity = 0;

	*tracks = NULL;
	*track_count = 0;
	if (size < 14 || memcmp(data, "MThd", 4))
		return fail("%s is no Standard MIDI File", where);
	*division = (uint32_t)data[12] << 8 | data[13];
	if ((*division & 0x8000) || !*division)
		return fail("%s: its time is no ticks a quarter note", where);
	at = 8 + ((size_t)data[4] << 24 | (size_t)data[5] << 16 | (size_t)data[6] << 8 | data[7]);
	while (at + 8 <= size)
	{
		const uint8_t *chunk = data + at;
		size_t length = (size_t)chunk[4] << 24 | (size_t)chunk[5] << 16 | (size_t)chunk[6] << 8 | chunk[7];
		const uint8_t *body = chunk + 8;
		size_t place = 0;
		int64_t tick = 0;
		int status = -1;
		events *track;

		if (length > size - at - 8)
			length = size - at - 8;
		at += 8 + length;
		if (memcmp(chunk, "MTrk", 4))
			continue;
		*tracks = grow(*tracks, &capacity, *track_count + 1, sizeof(events));
		if (!*tracks)
			return fail("no memory for a MIDI file's tracks");
		track = &(*tracks)[(*track_count)++];
		memset(track, 0, sizeof(*track));
		while (place < length)
		{
			uint32_t delta, count;
			uint8_t byte;

			if (!read_number(body, length, &place, &delta, where, *track_count))
				return 0;
			tick += delta;
			if (place >= length)
				return fail("%s: track %zu ends inside an event", where, *track_count);
			byte = body[place];
			if (byte == 0xFF)
			{
				uint8_t type;

				if (place + 1 >= length)
					return fail("%s: track %zu ends inside an event", where, *track_count);
				type = body[place + 1];
				place += 2;
				if (!read_number(body, length, &place, &count, where, *track_count))
					return 0;
				if (type == 0x2F)
					break;
				if (count > length - place)
					count = (uint32_t)(length - place);
				if (!add_event(track, tick, EVENT_META, type, 0, body + place, count))
					return 0;
				place += count;
				continue;
			}
			if (byte == 0xF0 || byte == 0xF7)
			{
				place++;
				if (!read_number(body, length, &place, &count, where, *track_count))
					return 0;
				place += count;
				continue;
			}
			if (byte & 0x80)
			{
				if (byte > 0xF0)
					return fail("%s: track %zu, tick %lld: a status byte %02X", where, *track_count, (long long)tick,
						byte);
				status = byte;
				place++;
			}
			else if (status < 0)
				return fail("%s: track %zu, tick %lld: data with no status before it", where, *track_count,
					(long long)tick);
			count = (status & 0xE0) == 0xC0 ? 1 : 2;
			if (place + count > length)
				return fail("%s: track %zu ends inside an event", where, *track_count);
			if ((status & 0xF0) == 0x90 && body[place + 1])
			{
				if (!add_event(track, tick, EVENT_ON, 0, body[place], NULL, 0))
					return 0;
			}
			else if ((status & 0xF0) == 0x80 || (status & 0xF0) == 0x90)
			{
				if (!add_event(track, tick, EVENT_OFF, 0, body[place], NULL, 0))
					return 0;
			}
			place += count;
		}
	}
	return 1;
}

/* ---- a song's channels ---- */

enum { STEP_NONE, STEP_NOTE, STEP_REST, STEP_HELD, STEP_CUT };

typedef struct
{
	int64_t tick;
	size_t first, count;        /* its commands in the track's */
	int step, key;              /* key -1: none yet */
	const uint8_t *held;
	uint32_t held_size;
	int64_t released;           /* its Note Off's tick, NO_TICK */
} step_read;

typedef struct
{
	const uint8_t *data;
	uint32_t size;
} command;

typedef struct
{
	char start[NAME_SIZE];
	char where[WHERE_SIZE];
	command *commands;
	size_t command_count, command_capacity;
	step_read *reads;
	size_t read_count, read_capacity;
	channel record;
} driver_track;

typedef struct
{
	const uint8_t *place;       /* a 'U' event's: its place's name, a 0 byte, the bytes */
	uint32_t size;
	char where[WHERE_SIZE];
} unshown_bytes;

typedef struct
{
	const uint8_t *numbers;     /* 'N' */
	uint32_t number_count;
	int has_numbers;
	unshown_bytes *unshown;
	size_t unshown_count, unshown_capacity;
	driver_track *tracks;
	size_t track_count, track_capacity;
} song;

static void free_song(song *each)
{
	for (size_t index = 0; index < each->track_count; index++)
	{
		free(each->tracks[index].commands);
		free(each->tracks[index].reads);
	}
	free(each->tracks);
	free(each->unshown);
	memset(each, 0, sizeof(*each));
}

/* the events of a tick in a track, the Note Offs first */
static int by_tick(const void *first, const void *second)
{
	const event *a = first, *b = second;
	const int a_off = a->kind == EVENT_OFF, b_off = b->kind == EVENT_OFF;

	if (a->tick != b->tick)
		return a->tick < b->tick ? -1 : 1;
	if (a_off != b_off)
		return a_off ? -1 : 1;
	return a->order < b->order ? -1 : a->order > b->order;
}

static void text_of(const uint8_t *data, uint32_t size, char *text, size_t capacity)
{
	const size_t length = size < capacity - 1 ? size : capacity - 1;

	memcpy(text, data, length);
	text[length] = 0;
}

/* A track's driver's events into a song's channel (when it has an 'S'), and its 'N' and 'U' events */
static int read_track(song *into, events *track, const char *file, size_t index)
{
	driver_track found;
	size_t open = 0;            /* the read whose note is on, 1 on (0: none) */
	int open_key = -1, ended = 0, has_start = 0;
	size_t ordered;

	memset(&found, 0, sizeof(found));
	snprintf(found.where, sizeof(found.where), "%s, track %zu", file, index + 1);
	qsort(track->list, track->count, sizeof(event), by_tick);
	for (ordered = 0; ordered < track->count; ordered++)
	{
		const event *each = &track->list[ordered];
		const uint8_t *data = NULL;
		uint32_t size = 0;
		char tag = 0;
		step_read *read;

		if (each->kind == EVENT_OFF)
		{
			if (open && open_key == each->key)
			{
				found.reads[open - 1].released = each->tick;
				open = 0;
			}
			continue;
		}
		if (each->kind == EVENT_META)
		{
			if (each->type != 0x7F || each->size < 2 || each->data[0] != TAG)
				continue;
			tag = (char)each->data[1];
			data = each->data + 2;
			size = each->size - 2;
			if (tag == 'N')
			{
				into->numbers = data;
				into->number_count = size;
				into->has_numbers = 1;
				continue;
			}
			if (tag == 'U')
			{
				unshown_bytes *added;

				if (!memchr(data, 0, size))
					return fail("%s, tick %lld: a 'U' event with no place", found.where, (long long)each->tick);
				into->unshown = grow(into->unshown, &into->unshown_capacity, into->unshown_count + 1,
					sizeof(unshown_bytes));
				if (!into->unshown)
					return fail("no memory for the music");
				added = &into->unshown[into->unshown_count++];
				added->place = data;
				added->size = size;
				snprintf(added->where, sizeof(added->where), "%s, tick %lld", found.where, (long long)each->tick);
				continue;
			}
			if (tag == 'S')
			{
				text_of(data, size, found.start, sizeof(found.start));
				has_start = 1;
				continue;
			}
			if (tag != 'C' && tag != 'R' && tag != 'H' && tag != 'Z')
				continue;
		}
		if (!has_start)
			return fail("%s, tick %lld: the driver's events come before its start ('S')", found.where,
				(long long)each->tick);
		if (ended)
			return fail("%s, tick %lld: an event after the channel's end", found.where, (long long)each->tick);
		if (!found.read_count || found.reads[found.read_count - 1].tick != each->tick)
		{
			if (found.read_count && found.reads[found.read_count - 1].step == STEP_NONE)
				return fail("%s, tick %lld: commands with no step after them", found.where,
					(long long)found.reads[found.read_count - 1].tick);
			found.reads = grow(found.reads, &found.read_capacity, found.read_count + 1, sizeof(step_read));
			if (!found.reads)
				return fail("no memory for the music");
			read = &found.reads[found.read_count++];
			memset(read, 0, sizeof(*read));
			read->tick = each->tick;
			read->first = found.command_count;
			read->key = -1;
			read->released = NO_TICK;
		}
		read = &found.reads[found.read_count - 1];
		if (each->kind == EVENT_ON)
		{
			if (read->step != STEP_NONE && !(read->step == STEP_HELD && read->key < 0))
				return fail("%s, tick %lld: a second step at a tick", found.where, (long long)each->tick);
			if (open)
				return fail("%s, tick %lld: a note starts before the one at tick %lld ends", found.where,
					(long long)each->tick, (long long)found.reads[open - 1].tick);
			if (read->step == STEP_NONE)
				read->step = STEP_NOTE;
			read->key = each->key;
			open = found.read_count;
			open_key = each->key;
			continue;
		}
		if (read->step != STEP_NONE)
			return fail("%s, tick %lld: an event after the tick's step", found.where, (long long)each->tick);
		if (tag == 'C')
		{
			if (!size)
				return fail("%s, tick %lld: a command with no bytes", found.where, (long long)each->tick);
			found.commands = grow(found.commands, &found.command_capacity, found.command_count + 1, sizeof(command));
			if (!found.commands)
				return fail("no memory for the music");
			found.commands[found.command_count].data = data;
			found.commands[found.command_count++].size = size;
			read->count++;
			ended = data[0] == 0xFF;
		}
		else if (tag == 'R')
			read->step = STEP_REST;
		else if (tag == 'H')
		{
			read->step = STEP_HELD;
			read->held = data;
			read->held_size = size;
		}
		else
		{
			read->step = STEP_CUT;
			ended = 1;
		}
	}
	if (!has_start)
	{
		free(found.commands);
		free(found.reads);
		return 1;
	}
	if (found.read_count && found.reads[found.read_count - 1].step == STEP_NONE && !ended)
		return fail("%s, tick %lld: commands with no step after them", found.where,
			(long long)found.reads[found.read_count - 1].tick);
	into->tracks = grow(into->tracks, &into->track_capacity, into->track_count + 1, sizeof(driver_track));
	if (!into->tracks)
		return fail("no memory for the music");
	into->tracks[into->track_count++] = found;
	return 1;
}

/* ---- the bytes ---- */

typedef struct
{
	const music_places *places;
	uint8_t *image, *given;
	uint8_t *source;            /* the file that gave a byte first (its number in music.txt, 1 on) */
	char (*files)[NAME_SIZE];
	uint8_t file;
} music_bytes;

static int put(music_bytes *into, uint32_t address, const uint8_t *data, uint32_t size, const char *where)
{
	for (uint32_t offset = 0; offset < size; offset++)
	{
		const uint32_t place = address + offset;

		if (place >= MUSIC_SPACE)
			return fail("%s: a byte past the sound program's addresses", where);
		if (into->given[place] && into->image[place] != data[offset])
		{
			char name[NAME_SIZE];

			into->places->name_of(into->places->host, place, name, sizeof(name));
			return fail("%s: the byte at %s comes back as %02X, and as %02X from %s", where, name, data[offset],
				into->image[place], into->files[into->source[place] - 1]);
		}
		into->image[place] = data[offset];
		into->given[place] = 1;
		into->source[place] = into->file;
	}
	return 1;
}

static int place_of(const music_places *places, const char *name, uint32_t *address, const char *where)
{
	if (!places->address_of(places->host, name, address))
		return fail("%s: no place is named %s", where, name);
	return 1;
}

/* A step's bytes at the channel's place */
static int put_step(music_bytes *into, channel *ch, uint8_t music_transpose, const step_read *read, int64_t length,
	const char *here)
{
	const music_places *places = into->places;
	uint8_t data[64];
	uint32_t size;

	if (read->step == STEP_HELD)
	{
		int64_t ticks;

		if (!read->held_size || held_length(read->held, read->held_size) != read->held_size
			|| read->held_size + 1 > sizeof(data))
			return fail("%s: the held note's bytes ('H') do not end at their first 0 nibble", here);
		data[0] = 0xE5;
		memcpy(data + 1, read->held, read->held_size);
		size = read->held_size + 1;
		ticks = held_ticks(read->held, read->held_size, places);
		if (ticks < 0)
			return fail("%s: the held note's bytes ('H') have a length's code note_lengths has not", here);
		if (ticks != length)
			return fail("%s: the held note lasts %lld ticks; its lengths ('H') add up to %lld", here,
				(long long)length, (long long)ticks);
		if (read->key < 0)
			return fail("%s: the held note ('H') has no Note On", here);
		if (ch->pcm && !drums(ch))
		{
			const int key = key_of(ch, music_transpose, read->held[read->held_size - 1] & 0x0F);

			if (key != read->key)
				return fail("%s: the held note plays key %d; its bytes ('H') give %d", here, read->key, key);
		}
		else
		{
			const int nibble = nibble_of(ch, music_transpose, read->key);

			if (nibble < 0)
				return fail("%s: no pitch gives the key %d from the channel's octave and transpositions", here,
					read->key);
			data[1] = (uint8_t)((data[1] & 0xF0) | nibble);
		}
	}
	else
	{
		const int code = code_of(length, places);
		int nibble = 0;

		if (!code)
			return fail("%s: no note or rest lasts %lld ticks (note_lengths)", here, (long long)length);
		if (read->step == STEP_NOTE)
		{
			nibble = nibble_of(ch, music_transpose, read->key);
			if (nibble <= 0)
				return fail("%s: no pitch gives the key %d from the channel's octave and transpositions", here,
					read->key);
		}
		data[0] = (uint8_t)(code << 4 | nibble);
		size = 1;
	}
	if (read->step != STEP_REST)
	{
		const int64_t expected = read->tick + released(ch, length);

		if (read->released == NO_TICK)
			return fail("%s: the note has no Note Off", here);
		if (read->released != expected)
			return fail("%s: the note ends at tick %lld; the driver keys it off at %lld", here,
				(long long)read->released, (long long)expected);
	}
	if (!put(into, ch->place, data, size, here))
		return 0;
	ch->place += size;
	return 1;
}

/* A file's song: its channels' commands and steps, read by tick and then by channel, into the bytes */
static int rebuild(music_bytes *into, const uint8_t *data, size_t size, const char *file)
{
	const music_places *places = into->places;
	uint32_t division;
	events *tracks = NULL;
	size_t track_count = 0, *next = NULL;
	song found;
	uint8_t music_transpose = 0;
	int ok = 0;

	memset(&found, 0, sizeof(found));
	if (!parse(data, size, file, &division, &tracks, &track_count))
		goto done;
	for (size_t index = 0; index < track_count; index++)
		if (!read_track(&found, &tracks[index], file, index))
			goto done;
	if (!found.has_numbers)
	{
		fail("%s: no 'N' event says its songs", file);
		goto done;
	}
	if (found.track_count != SONG_TRACKS)
	{
		fail("%s: %zu tracks with a start ('S'); a song has %d", file, found.track_count, SONG_TRACKS);
		goto done;
	}
	/* the driver's ticks */
	for (size_t index = 0; index < found.track_count; index++)
	{
		driver_track *track = &found.tracks[index];

		for (size_t read = 0; read < track->read_count; read++)
		{
			int64_t *ticks[2] = { &track->reads[read].tick, &track->reads[read].released };

			for (int slot = 0; slot < 2; slot++)
			{
				if (*ticks[slot] == NO_TICK)
					continue;
				if (*ticks[slot] * TICKS_A_QUARTER % division)
				{
					fail("%s: the tick %lld is no whole driver's tick", track->where, (long long)*ticks[slot]);
					goto done;
				}
				*ticks[slot] = *ticks[slot] * TICKS_A_QUARTER / division;
			}
		}
		memset(&track->record, 0, sizeof(track->record));
		track->record.number = (int)index;
		track->record.pcm = index >= 7;
		{
			char where[WHERE_SIZE + 16];

			snprintf(where, sizeof(where), "%s, its start", track->where);
			if (!place_of(places, track->start, &track->record.place, where))
				goto done;
		}
	}
	/* the songs table's entries it plays */
	for (uint32_t number = 0; number < found.number_count; number++)
		for (size_t index = 0; index < SONG_TRACKS; index++)
		{
			const uint32_t start = found.tracks[index].record.place;
			const uint8_t entry[2] = { (uint8_t)start, (uint8_t)(start >> 8) };
			char where[WHERE_SIZE];

			snprintf(where, sizeof(where), "%s's 'N'", file);
			if (!put(into, places->songs_table + 2 * (SONG_TRACKS * found.numbers[number] + (uint32_t)index), entry,
				2, where))
				goto done;
		}
	/* the reads, by tick and then by channel */
	next = calloc(SONG_TRACKS, sizeof(size_t));
	if (!next)
	{
		fail("no memory for the music");
		goto done;
	}
	for (;;)
	{
		driver_track *track = NULL;
		channel *ch;
		const step_read *read;
		char here[WHERE_SIZE + 32];
		size_t chosen = 0;

		for (size_t index = 0; index < SONG_TRACKS; index++)
		{
			const driver_track *each = &found.tracks[index];

			if (next[index] < each->read_count
				&& (!track || each->reads[next[index]].tick < track->reads[next[chosen]].tick))
			{
				track = &found.tracks[index];
				chosen = index;
			}
		}
		if (!track)
			break;
		ch = &track->record;
		read = &track->reads[next[chosen]];
		snprintf(here, sizeof(here), "%s, tick %lld", track->where, (long long)read->tick);
		if (ch->done)
		{
			fail("%s: the channel goes on after its end", here);
			goto done;
		}
		for (size_t index = 0; index < read->count; index++)
		{
			const command *each = &track->commands[read->first + index];
			const uint8_t code = each->data[0];
			uint8_t bytes[8];
			uint32_t length = each->size, target = 0;
			int64_t place;

			if ((code >= 0x10 && code < 0x70) || (code >= 0x80 && code < 0xE0) || code == 0xE5)
			{
				fail("%s: %02X is no command ('C')", here, code);
				goto done;
			}
			if (address_at(code))
			{
				char name[NAME_SIZE];
				const uint32_t at = address_at(code);

				if (each->size < at)
				{
					fail("%s: the command %02X has no place", here, code);
					goto done;
				}
				text_of(each->data + at, each->size - at, name, sizeof(name));
				if (!place_of(places, name, &target, here))
					goto done;
				memcpy(bytes, each->data, at);
				bytes[at] = (uint8_t)target;
				bytes[at + 1] = (uint8_t)(target >> 8);
				length = at + 2;
			}
			else if (each->size <= sizeof(bytes))
				memcpy(bytes, each->data, each->size);
			if (length != command_length(code))
			{
				fail("%s: the command %02X has %u bytes; the driver reads %u", here, code, (unsigned)length,
					(unsigned)command_length(code));
				goto done;
			}
			if (!put(into, ch->place, bytes, length, here))
				goto done;
			place = execute(ch, &music_transpose, bytes, target);
			if (place < 0)
			{
				ch->done = 1;
				break;
			}
			ch->place = (uint32_t)place;
		}
		next[chosen]++;
		if (ch->done || read->step == STEP_NONE)
			continue;
		if (read->step == STEP_CUT)
		{
			ch->done = 1;
			continue;
		}
		if (next[chosen] >= track->read_count)
		{
			fail("%s: no step after it says how long it is", here);
			goto done;
		}
		if (!put_step(into, ch, music_transpose, read, track->reads[next[chosen]].tick - read->tick, here))
			goto done;
	}
	/* the bytes no track shows */
	for (size_t index = 0; index < found.unshown_count; index++)
	{
		const unshown_bytes *each = &found.unshown[index];
		const uint8_t *zero = memchr(each->place, 0, each->size);
		const uint32_t name_size = (uint32_t)(zero - each->place);
		char name[NAME_SIZE];
		uint32_t address;

		text_of(each->place, name_size, name, sizeof(name));
		if (!place_of(places, name, &address, each->where)
			|| !put(into, address, zero + 1, each->size - name_size - 1, each->where))
			goto done;
	}
	ok = 1;
done:
	free(next);
	free_song(&found);
	for (size_t index = 0; index < track_count; index++)
		free(tracks[index].list);
	free(tracks);
	return ok;
}

/* ---- the songs ---- */

static uint8_t *read_whole(const char *path, size_t *size)
{
	FILE *file = fopen(path, "rb");
	uint8_t *data;
	long length;

	if (!file)
		return NULL;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	data = malloc(length > 0 ? (size_t)length : 1u);
	if (data && length > 0 && fread(data, 1, (size_t)length, file) != (size_t)length)
	{
		free(data);
		data = NULL;
	}
	fclose(file);
	*size = length > 0 ? (size_t)length : 0;
	return data;
}

const char *music_read(const char *folder, const music_places *places, uint8_t *image, uint8_t *given)
{
	char path[1024], (*files)[NAME_SIZE] = NULL;
	size_t size, file_count = 0, file_capacity = 0;
	uint8_t *listing;
	music_bytes into;
	int ok = 1;

	g_problem[0] = 0;
	snprintf(path, sizeof(path), "%s/music.txt", folder);
	if (!(listing = read_whole(path, &size)))
		return "there is no music.txt";
	/* the files music.txt names: each line's first word */
	{
		size_t at = 0;

		while (at < size)
		{
			size_t end = at, word = 0;
			char name[NAME_SIZE];

			while (end < size && listing[end] != '\n')
				end++;
			while (at < end && (listing[at] == ' ' || listing[at] == '\t'))
				at++;
			if (at < end && listing[at] != '#')
				while (at + word < end && listing[at + word] != ' ' && listing[at + word] != '\t'
					&& listing[at + word] != '\r' && word < NAME_SIZE - 1)
				{
					name[word] = (char)listing[at + word];
					word++;
				}
			if (word)
			{
				name[word] = 0;
				files = grow(files, &file_capacity, file_count + 1, sizeof(*files));
				if (!files || file_count >= 255)
				{
					free(listing);
					free(files);
					return "music.txt names more files than the music can take";
				}
				snprintf(files[file_count++], NAME_SIZE, "%s", name);
			}
			at = end + 1;
		}
	}
	free(listing);
	memset(&into, 0, sizeof(into));
	into.places = places;
	into.image = image;
	into.given = given;
	into.files = files;
	into.source = calloc(MUSIC_SPACE, 1);
	if (!into.source)
	{
		free(files);
		return "no memory for the music";
	}
	for (size_t index = 0; index < file_count && ok; index++)
	{
		uint8_t *data;
		char file[NAME_SIZE + 8];

		snprintf(file, sizeof(file), "%s.mid", files[index]);
		snprintf(path, sizeof(path), "%s/%s", folder, file);
		if (!(data = read_whole(path, &size)))
		{
			fail("music.txt names %s, and there is no %s", files[index], file);
			ok = 0;
			break;
		}
		snprintf(files[index], NAME_SIZE, "%s", file);
		into.file = (uint8_t)(index + 1);
		ok = rebuild(&into, data, size, file);
		free(data);
	}
	free(into.source);
	free(files);
	return ok ? NULL : g_problem;
}
