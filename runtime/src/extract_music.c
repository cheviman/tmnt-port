/**
 *    Name:        extract_music.c
 *    Description: the first run's sound/music/: the songs as Standard MIDI Files and music.txt, as
 *                 tools/sound_music.py writes them
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    tools/sound_music.py's write, the same steps in C (its docstring says the format whole; runtime/src/music.c is
 *    the way back): a file for each different set of the songs table's 9 tracks, named by the first of its songs the
 *    game names (extract_layout.c's uses) or song_XX; each song played pass by pass as the driver plays it (the
 *    Player: its channels' reads up to the tick where the last of them first comes back to a state it had, its
 *    tempo by tick and the time its passes take), the notes' keys and velocities as the way back reads them
 *    (replay_keys), the FM instruments' programs by their keys, the bytes no file shows, and the events in each
 *    track in the order the Python sorts them. The velocities of the YM2151's attenuations are Python's
 *    (extract_layout.c); the tempos are worked out in IEEE doubles as Python's floats are.
 */
#include "extract_internal.h"
#include "extract_layout.h"

#include "game/data.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TICKS_A_QUARTER     24
#define SOUND_CLOCK         3579545
#define TIMER_B_SAMPLES     (16 * (256 - 0xF2))
#define POLL_CYCLES         205
#define RESTART_CYCLES      380
#define WORK_BASE           3900
#define WORK_FM_NOTE        4900
#define WORK_FM_REST        1000
#define WORK_PCM_NOTE       3000
#define WORK_PCM_REST       1400
#define WORK_INSTRUMENT     3600
#define WORK_COMMAND        160
#define CAP_PASSES          45000
#define SONG_TRACKS         9
#define FIRST_SONG          0x60
#define TAG                 0x7D
#define FM_BASS             38
#define FM_LEAD             62
#define PCM_BASS            33
#define DRUM_CHANNEL        9
#define NONE                0xFFFFFFFFu     /* None in a channel's state */
#define NO_KEY              (-1)
#define STATE_WORDS         26
#define NAME_SIZE           300

static const int drum_keys[16] = { NO_KEY, 36, 38, 40, 37, 42, 41, 43, 45, 47, 48, 50, 64, 63, 61, 60 };
static const char *const channel_names[SONG_TRACKS] = { "YM2151 channel 1", "YM2151 channel 2", "YM2151 channel 3",
	"YM2151 channel 4", "YM2151 channel 5", "YM2151 channel 6", "YM2151 channel 7", "K007232 channel A", "K007232 channel B" };

static int failed;      /* no memory, or what the Python would stop at */

/* ---- the driver ---- */

static uint32_t image_byte(uint32_t at)
{
	return at < GAME_SOUND_SIZE ? game_sound[at] : 0u;
}

/* a command's bytes (sound_music.py COMMAND_LENGTHS: sound_data_walk.py's FIXED and the jumps), 0 for one it has not */
static uint32_t command_length(uint32_t command)
{
	if (command < 0xE0)
		return 1;
	switch (command)
	{
	case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE6: case 0xE7: case 0xE8: case 0xE9: case 0xEA:
	case 0xEB: case 0xEE: case 0xEF: case 0xF2: case 0xF4: case 0xFC: case 0xFD: case 0xFE:
		return 2;
	case 0xED: case 0xF0: case 0xF8: case 0xFA:
		return 3;
	case 0xF1: case 0xF6:
		return 4;
	case 0xEC:
		return 6;
	case 0xF3: case 0xF5: case 0xF7: case 0xF9: case 0xFB: case 0xFF:
		return 1;
	default:
		return 0;
	}
}

/* where a command's address is, 0 when it has none (ADDRESS_AT) */
static uint32_t address_at(uint32_t command)
{
	switch (command)
	{
	case 0xEC: return 4;
	case 0xF1: case 0xF6: return 2;
	case 0xF0: case 0xF8: case 0xFA: return 1;
	default: return 0;
	}
}

static uint32_t transposition(uint32_t byte)
{
	const uint32_t value = (24u * ((byte & 0x70u) >> 4) + 2u * (byte & 0x0Fu)) & 0xFFu;

	return byte & 0x80u ? (0u - value) & 0xFFu : value;
}

static uint32_t attenuation_offset(uint32_t byte)
{
	return byte & 0x80u ? (0u - (byte & 0x7Fu)) & 0xFFu : byte;
}

/* the bytes of a held note after E5 at 'at' (up to the byte with a 0 nibble, b1 always), as Python's slice of 39 */
static uint32_t held_length(uint32_t at)
{
	const uint32_t size = at < GAME_SOUND_SIZE ? (GAME_SOUND_SIZE - at < 39u ? GAME_SOUND_SIZE - at : 39u) : 0u;
	uint32_t index = 1;

	while (index < size && (image_byte(at + index) & 0xF0u) && (image_byte(at + index) & 0x0Fu))
		index++;
	return index + 1 < size ? index + 1 : size;
}

typedef struct
{
	const uint8_t *lengths;
	uint32_t count;
} length_table;

/* the ticks of a held note's bytes, as the driver adds them up (0 is 256); -1 when a code is none of note_lengths' */
static int held_ticks(uint32_t at, uint32_t size, const length_table *lengths)
{
	uint32_t codes[80], count = 0, sum = 0;

	codes[count++] = image_byte(at) >> 4;
	for (uint32_t index = 1; index < size; index++)
	{
		const uint32_t byte = image_byte(at + index);

		if (!(byte & 0xF0u))
			break;
		codes[count++] = byte >> 4;
		if (!(byte & 0x0Fu))
			break;
		codes[count++] = byte & 0x0Fu;
	}
	for (uint32_t index = 0; index < count; index++)
	{
		if (codes[index] < 1 || codes[index] > lengths->count)
			return -1;
		sum += lengths->lengths[codes[index] - 1];
	}
	return (sum & 0xFFu) ? (int)(sum & 0xFFu) : 256;
}

typedef struct
{
	uint32_t tempo, tempo_increase, accelerando, transpose, volume;
} music_state;

typedef struct
{
	int number, pcm;
	uint32_t place;
	uint32_t octave, transpose, loop_transpose, loop_volume_offset, volume_offset, mode, instrument, volume, gate, detune,
		portamento, lfo_ramp, attenuation, loop_count, loop_start, loop_2_count, loop_2_start, repeat_count, repeat_return,
		repeat_part, call_return, call_2_return, loop_exit_count, transposed_loop_count, in_call, in_call_2;
} channel_record;

/* the command at 'at' ('length' bytes, 'target' the address it carries): what it does to the record and to the
   music's variables; where the track goes on, -1 when it ends (Channel.execute) */
static int64_t execute(channel_record *channel, music_state *music, uint32_t at, uint32_t length, uint32_t target)
{
	const uint32_t command = image_byte(at), argument = length > 1 ? image_byte(at + 1) : 0;
	uint32_t count;

	if (command < 0x10)
	{
		channel->octave = channel->pcm ? command & 0x0Fu : (24u * (command & 0x0Fu)) & 0xFFu;
		return at + 1;
	}
	if ((command & 0xF0u) == 0x70)
	{
		channel->attenuation = command & 0x0Fu;
		return at + 1;
	}
	switch (command)
	{
	case 0xE0: channel->mode = argument; break;
	case 0xE1: music->tempo = argument; break;
	case 0xE2: channel->instrument = argument; break;
	case 0xE3:
		channel->volume = (argument + (channel->pcm ? 0u : 10u)) & 0xFFu;
		channel->attenuation = 0;
		break;
	case 0xE4: channel->gate = argument; break;
	case 0xE6: channel->detune = argument; break;
	case 0xE7: channel->portamento = argument; break;
	case 0xE8: music->transpose = transposition(argument); break;
	case 0xE9: channel->transpose = transposition(argument); break;
	case 0xEA: music->volume = attenuation_offset(argument); break;
	case 0xEB: channel->volume_offset = attenuation_offset(argument); break;
	case 0xEC:
		count = channel->transposed_loop_count;
		if (count == 1)
		{
			channel->transposed_loop_count = 0;
			channel->loop_transpose = channel->loop_volume_offset = 0;
			return at + 6;
		}
		if (count == 0)
		{
			count = argument;
			channel->loop_transpose = channel->loop_volume_offset = 0;
		}
		channel->transposed_loop_count = (count - 1u) & 0xFFu;
		channel->loop_transpose = (channel->loop_transpose + transposition(image_byte(at + 2))) & 0xFFu;
		channel->loop_volume_offset = (channel->loop_volume_offset + attenuation_offset(image_byte(at + 3))) & 0xFFu;
		return target;
	case 0xEE: channel->lfo_ramp = argument; break;
	case 0xF0: return target;
	case 0xF1:
		count = channel->loop_exit_count;
		if (count == 1)
		{
			channel->loop_exit_count = 0;
			return target;
		}
		channel->loop_exit_count = ((count == 0 ? argument : count) - 1u) & 0xFFu;
		return at + 4;
	case 0xF2:
		channel->loop_count = argument;
		channel->loop_start = at + 2;
		return at + 2;
	case 0xF4:
		channel->loop_2_count = argument;
		channel->loop_2_start = at + 2;
		return at + 2;
	case 0xF3:
		count = channel->loop_count;
		channel->loop_count = (count - 1u) & 0xFFu;
		return count == 1 ? at + 1 : channel->loop_start;
	case 0xF5:
		count = channel->loop_2_count;
		channel->loop_2_count = (count - 1u) & 0xFFu;
		return count == 1 ? at + 1 : channel->loop_2_start;
	case 0xF6:
		channel->repeat_count = argument;
		channel->repeat_return = at + 4;
		channel->repeat_part = target;
		return target;
	case 0xF7:
		count = channel->repeat_count;
		channel->repeat_count = (count - 1u) & 0xFFu;
		return count == 1 ? channel->repeat_return : channel->repeat_part;
	case 0xF8:
		channel->call_return = at + 3;
		channel->in_call = 1;
		return target;
	case 0xF9:
		channel->in_call = 0;
		return channel->call_return;
	case 0xFA:
		channel->call_2_return = at + 3;
		channel->in_call_2 = 1;
		return target;
	case 0xFB:
		channel->in_call_2 = 0;
		return channel->call_2_return;
	case 0xFC: case 0xFD: case 0xFE:
		music->accelerando = argument | ((argument & 0x0Fu) << 4) << 8;
		music->tempo_increase = 0;
		break;
	case 0xFF:
		return -1;
	default:
		break;
	}
	return at + length;
}

static int drums(const channel_record *channel)
{
	return channel->pcm && (channel->mode & 1u);
}

/* the key of the note whose pitch nibble is 'nibble', NO_KEY when none has it */
static int key_of(const channel_record *channel, const music_state *music, uint32_t nibble)
{
	int key;

	if (drums(channel))
		return nibble > 0 && nibble < 16 ? drum_keys[nibble] : NO_KEY;
	if (channel->pcm)
	{
		const uint32_t octave = (channel->octave + 1u) & 0xFFu;

		key = (int)((nibble + 12u * (octave ? octave : 256u)) & 0xFFu) - 1;
	}
	else
		key = (int)(((music->transpose + 2u * nibble + channel->octave + channel->transpose + channel->loop_transpose) & 0xFFu) / 2u) + 11;
	return key >= 0 && key <= 127 ? key : NO_KEY;
}

/* the pitch nibble that gives 'key', -1 when none does */
static int nibble_of(const channel_record *channel, const music_state *music, int key)
{
	uint32_t nibble;

	if (drums(channel))
	{
		for (int index = 1; index < 16; index++)
			if (drum_keys[index] == key)
				return index;
		return -1;
	}
	if (channel->pcm)
	{
		const uint32_t octave = (channel->octave + 1u) & 0xFFu;

		nibble = ((uint32_t)key + 1u - 12u * (octave ? octave : 256u)) & 0xFFu;
	}
	else
	{
		if (key < 11)
			return -1;
		nibble = ((2u * (uint32_t)(key - 11) - music->transpose - channel->octave - channel->transpose - channel->loop_transpose)
			& 0xFFu) / 2u;
	}
	return nibble <= 15 && key_of(channel, music, nibble) == key ? (int)nibble : -1;
}

static int midi_channel(const channel_record *channel)
{
	if (drums(channel))
		return DRUM_CHANNEL;
	return channel->pcm ? (channel->number == 7 ? 8 : 10) : channel->number + 1;
}

static int released(const channel_record *channel, int length)
{
	int when;

	if (channel->pcm || !channel->gate)
		return length;
	when = length - ((int)(((uint32_t)length & 0xFFu) * channel->gate >> 8) + 1);
	return when > 1 ? when : 1;
}

/* ---- the songs, played pass by pass (sound_music.py Player) ---- */

enum { STEP_NONE, STEP_NOTE, STEP_REST, STEP_HELD, STEP_END, STEP_CUT };

typedef struct
{
	uint32_t at, length, target;
} command_item;

typedef struct
{
	int64_t tick;
	size_t first_item, item_count;
	int step;
	uint32_t step_at, step_length;  /* the step's bytes (a note's or a rest's one, a held note's E5 and the rest) */
	int key;
	int ticks;
	/* the shown read's note (replay_keys) */
	int note_key, velocity, note_channel, note_released;
} track_read;

typedef struct
{
	uint32_t *words;        /* STATE_WORDS a state */
	uint8_t *taken;
	size_t capacity, count;
} state_set;

typedef struct
{
	channel_record record;
	track_read *reads;
	size_t read_count, read_capacity;
	command_item *items;
	size_t item_count, item_capacity;
	state_set seen;
	int handler;            /* 0 none, 1 start, 2 first, 3 tick */
	int64_t tick, done;     /* done: -1 None */
	uint32_t tempo_count, ticks_left;
} player_channel;

typedef struct
{
	int64_t tick;
	uint32_t tempo;
} tempo_change;

typedef struct
{
	music_state music;
	player_channel channels[SONG_TRACKS];
	tempo_change *tempos;
	size_t tempo_count, tempo_capacity;
	int64_t last_tempo;     /* -1 None */
	uint32_t passes;
	int64_t clock;
	int64_t *clocks;        /* by tick: the cycles at the pass that first reached it, -1 none */
	size_t clock_capacity;
	int64_t clock_top;      /* the highest tick in clocks */
	int64_t work;
	const length_table *lengths;
} player;

static void *grow_list(void *list, size_t *capacity, size_t needed, size_t unit)
{
	void *grown;

	if (needed <= *capacity)
		return list;
	if (!(grown = realloc(list, (needed * 2 + 64) * unit)))
	{
		failed = 1;
		return list;
	}
	*capacity = needed * 2 + 64;
	return grown;
}

static void channel_state(const channel_record *c, uint32_t *state)
{
	const uint32_t words[STATE_WORDS] = { c->place, c->octave, c->transpose, c->loop_transpose, c->loop_volume_offset,
		c->volume_offset, c->mode, c->instrument, c->volume, c->gate, c->detune, c->portamento, c->lfo_ramp, c->attenuation,
		c->loop_count, c->loop_2_count, c->loop_exit_count, c->transposed_loop_count, c->repeat_count,
		c->loop_count ? c->loop_start : NONE, c->loop_2_count ? c->loop_2_start : NONE,
		c->repeat_count ? c->repeat_return : NONE, c->repeat_count ? c->repeat_part : NONE,
		c->in_call ? c->call_return : NONE, c->in_call_2 ? c->call_2_return : NONE, 0 };

	memcpy(state, words, sizeof(words));
}

static size_t state_hash(const uint32_t *state)
{
	uint64_t hash = 1469598103934665603ull;

	for (int index = 0; index < STATE_WORDS; index++)
		hash = (hash ^ state[index]) * 1099511628211ull;
	return (size_t)(hash ^ hash >> 29);
}

/* whether the state was in the set, and it is there now */
static int state_seen(state_set *set, const uint32_t *state)
{
	size_t slot;

	if (set->count * 2 + 2 > set->capacity)
	{
		state_set bigger = { NULL, NULL, set->capacity ? set->capacity * 2 : 1024, 0 };

		bigger.words = malloc(bigger.capacity * STATE_WORDS * sizeof(uint32_t));
		bigger.taken = calloc(bigger.capacity, 1);
		if (!bigger.words || !bigger.taken)
		{
			free(bigger.words);
			free(bigger.taken);
			failed = 1;
			return 0;
		}
		for (size_t index = 0; index < set->capacity; index++)
			if (set->taken[index])
			{
				const uint32_t *old = &set->words[index * STATE_WORDS];

				for (slot = state_hash(old) % bigger.capacity; bigger.taken[slot]; slot = (slot + 1) % bigger.capacity)
					;
				memcpy(&bigger.words[slot * STATE_WORDS], old, STATE_WORDS * sizeof(uint32_t));
				bigger.taken[slot] = 1;
				bigger.count++;
			}
		free(set->words);
		free(set->taken);
		*set = bigger;
	}
	for (slot = state_hash(state) % set->capacity; set->taken[slot]; slot = (slot + 1) % set->capacity)
		if (!memcmp(&set->words[slot * STATE_WORDS], state, STATE_WORDS * sizeof(uint32_t)))
			return 1;
	memcpy(&set->words[slot * STATE_WORDS], state, STATE_WORDS * sizeof(uint32_t));
	set->taken[slot] = 1;
	set->count++;
	return 0;
}

/* read_track: the channel's commands up to its next step, as a read */
static void read_track(player *p, player_channel *channel)
{
	uint32_t state[STATE_WORDS];
	track_read *read;

	channel_state(&channel->record, state);
	if (state_seen(&channel->seen, state) && channel->done < 0)
		channel->done = channel->tick;
	channel->reads = grow_list(channel->reads, &channel->read_capacity, channel->read_count + 1, sizeof(*channel->reads));
	if (failed)
		return;
	read = &channel->reads[channel->read_count++];
	memset(read, 0, sizeof(*read));
	read->tick = channel->tick;
	read->first_item = channel->item_count;
	read->key = NO_KEY;
	for (;;)
	{
		const uint32_t at = channel->record.place, byte = image_byte(at);

		if ((byte & 0xF0u) == 0x00 || (byte & 0xF0u) == 0x70 || (byte >= 0xE0 && byte != 0xE5))
		{
			const uint32_t length = command_length(byte);
			const uint32_t where = address_at(byte);
			const uint32_t target = where ? image_byte(at + where) | image_byte(at + where + 1) << 8 : NONE;
			int64_t place;

			if (!length)
			{
				failed = 1;
				return;
			}
			channel->items = grow_list(channel->items, &channel->item_capacity, channel->item_count + 1, sizeof(*channel->items));
			if (failed)
				return;
			channel->items[channel->item_count++] = (command_item){ at, length, target };
			read->item_count++;
			place = execute(&channel->record, &p->music, at, length, target);
			if (place < 0)
			{
				read->step = STEP_END;
				read->step_at = at;
				channel->handler = 0;
				if (channel->done < 0)
					channel->done = channel->tick;
				return;
			}
			channel->record.place = (uint32_t)place;
			continue;
		}
		if (byte == 0xE5)
		{
			const uint32_t length = held_length(at + 1);
			const uint32_t nibble = channel->record.pcm && !drums(&channel->record) ? image_byte(at + length) & 0x0Fu
				: image_byte(at + 1) & 0x0Fu;
			const int ticks = held_ticks(at + 1, length, p->lengths);

			if (ticks < 0)
			{
				failed = 1;
				return;
			}
			read->step = STEP_HELD;
			read->step_at = at;
			read->step_length = 1 + length;
			read->key = key_of(&channel->record, &p->music, nibble);
			channel->ticks_left = (uint32_t)ticks & 0xFFu;
			read->ticks = ticks;
			channel->record.place = at + 1 + length;
		}
		else
		{
			const uint32_t code = byte >> 4;

			if (code < 1 || code > p->lengths->count)
			{
				failed = 1;
				return;
			}
			read->step = byte & 0x0Fu ? STEP_NOTE : STEP_REST;
			read->step_at = at;
			read->step_length = 1;
			read->key = key_of(&channel->record, &p->music, byte & 0x0Fu);
			channel->ticks_left = p->lengths->lengths[code - 1];
			read->ticks = (int)channel->ticks_left;
			channel->record.place = at + 1;
		}
		return;
	}
}

/* read_track, and its work in the pass */
static void read_channel(player *p, player_channel *channel)
{
	const track_read *read;
	int note;

	read_track(p, channel);
	if (failed)
		return;
	read = &channel->reads[channel->read_count - 1];
	note = read->step == STEP_NOTE || read->step == STEP_HELD;
	p->work += (channel->record.pcm ? (note ? WORK_PCM_NOTE : WORK_PCM_REST) : (note ? WORK_FM_NOTE : WORK_FM_REST))
		+ WORK_COMMAND * (int64_t)read->item_count;
	if (!channel->record.pcm)
		for (size_t index = 0; index < read->item_count; index++)
			if (image_byte(channel->items[read->first_item + index].at) == 0xE2)
				p->work += WORK_INSTRUMENT;
}

static int64_t tick_now(const player *p)
{
	int64_t most = -1, any = -1;

	for (int index = 0; index < SONG_TRACKS; index++)
	{
		const player_channel *channel = &p->channels[index];

		if (channel->handler && channel->tick > most)
			most = channel->tick;
		if (channel->tick > any)
			any = channel->tick;
	}
	return most >= 0 ? most : any;
}

static void note_tempo(player *p)
{
	const uint32_t tempo = (p->music.tempo + p->music.tempo_increase) & 0xFFu;

	if ((int64_t)tempo != p->last_tempo)
	{
		const int64_t tick = tick_now(p);

		if (p->tempo_count && p->tempos[p->tempo_count - 1].tick == tick)
			p->tempo_count--;
		p->tempos = grow_list(p->tempos, &p->tempo_capacity, p->tempo_count + 1, sizeof(*p->tempos));
		if (failed)
			return;
		p->tempos[p->tempo_count++] = (tempo_change){ tick, tempo };
		p->last_tempo = tempo;
	}
}

/* a positive value over 'by', rounded up (Python's -(-value // by)) */
static int64_t ceiling_divide(int64_t value, int64_t by)
{
	return (value + by - 1) / by;
}

/* the clock at a tick, first come */
static void set_clock(player *p, int64_t tick, int64_t clock)
{
	if (tick < 0)
		return;
	if ((size_t)tick >= p->clock_capacity)
	{
		const size_t old = p->clock_capacity;

		p->clocks = grow_list(p->clocks, &p->clock_capacity, (size_t)tick + 1, sizeof(*p->clocks));
		if (failed)
			return;
		for (size_t index = old; index < p->clock_capacity; index++)
			p->clocks[index] = -1;
	}
	if (p->clocks[tick] < 0)
		p->clocks[tick] = clock;
	if (tick > p->clock_top)
		p->clock_top = tick;
}

static void pass_time(player *p)
{
	const int64_t samples = ceiling_divide(p->clock, 64);
	const int64_t expiry = p->clock + (TIMER_B_SAMPLES - (samples & 15)) * 64;
	const int64_t end = p->clock + WORK_BASE + p->work;
	const int64_t found = end >= expiry ? end : end + POLL_CYCLES * ceiling_divide(expiry - end, POLL_CYCLES);

	p->clock = found + RESTART_CYCLES;
	p->work = 0;
	set_clock(p, tick_now(p), p->clock);
}

static void step(player *p)
{
	music_state *music = &p->music;

	p->passes++;
	for (int index = 0; index < SONG_TRACKS && !failed; index++)
	{
		player_channel *channel = &p->channels[index];

		if (channel->handler == 1)
			channel->handler = 2;
		else if (channel->handler == 2)
		{
			channel->handler = 3;
			read_channel(p, channel);
		}
		else if (channel->handler == 3)
		{
			uint32_t tempo;

			if (index == 0 && (music->accelerando & 0x0Fu))
			{
				uint32_t count = ((music->accelerando >> 8) - 1u) & 0xFFu;

				if (count == 0)
				{
					count = (music->accelerando & 0x0Fu) << 4;
					music->tempo_increase = (music->tempo_increase + 1u) & 0xFFu;
				}
				music->accelerando = (music->accelerando & 0xFFu) | count << 8;
			}
			tempo = (music->tempo_increase + music->tempo) & 0xFFu;
			if (tempo == 0)
			{
				channel->handler = 0;
				if (channel->done < 0)
					channel->done = channel->tick;
				continue;    /* (no tempo noted for it: the Python's continue) */
			}
			channel->tempo_count += tempo;
			if (channel->tempo_count > 0xFF)
			{
				const uint32_t left = channel->ticks_left;

				channel->tempo_count &= 0xFFu;
				channel->tick++;
				channel->ticks_left = (left - 1u) & 0xFFu;
				if (left == 1)
					read_channel(p, channel);
			}
		}
		note_tempo(p);
	}
	pass_time(p);
}

static int64_t run(player *p)
{
	int64_t end = -1;

	for (;;)
	{
		int all_done = 1, all_past = 1;

		step(p);
		if (failed)
			return -1;
		for (int index = 0; index < SONG_TRACKS; index++)
			all_done &= p->channels[index].done >= 0;
		if (end < 0 && (all_done || p->passes >= CAP_PASSES))
			for (int index = 0; index < SONG_TRACKS; index++)
			{
				const player_channel *channel = &p->channels[index];
				const int64_t at = channel->done >= 0 ? channel->done : channel->tick;

				if (at > end)
					end = at;
			}
		if (end < 0)
			continue;
		for (int index = 0; index < SONG_TRACKS; index++)
		{
			const player_channel *channel = &p->channels[index];

			if (channel->handler && !(channel->read_count && channel->reads[channel->read_count - 1].tick >= end))
				all_past = 0;
		}
		if (all_past)
			return end;
	}
}

/* each channel's reads the file shows (Player.shown): those before 'end', then the first at or after it, its
   commands only (its step a cut) unless it ends the track; after the last read of a channel the tempo stopped, a
   cut. The reads are cut down in place, and a cut read added where it takes one */
static void shown(player *p, int64_t end)
{
	for (int index = 0; index < SONG_TRACKS && !failed; index++)
	{
		player_channel *channel = &p->channels[index];
		size_t kept = 0;
		int broke = 0;

		for (; kept < channel->read_count; kept++)
		{
			track_read *read = &channel->reads[kept];

			if (read->tick < end)
				continue;
			if (read->step != STEP_END)
			{
				read->step = STEP_CUT;
				read->ticks = 0;
				read->key = NO_KEY;
			}
			kept++;
			broke = 1;
			break;
		}
		channel->read_count = kept;
		if (!broke && kept && channel->reads[kept - 1].step != STEP_END)
		{
			const track_read last = channel->reads[kept - 1];
			track_read *cut;

			channel->reads = grow_list(channel->reads, &channel->read_capacity, kept + 1, sizeof(*channel->reads));
			if (failed)
				return;
			cut = &channel->reads[channel->read_count++];
			memset(cut, 0, sizeof(*cut));
			cut->tick = last.tick + last.ticks;
			cut->first_item = channel->item_count;
			cut->step = STEP_CUT;
			cut->key = NO_KEY;
		}
	}
}

/* [(tick, microseconds a quarter note)] where the tempo changes, into 'out' (Player.tempo_events) */
static size_t tempo_events(const player *p, int64_t end, int64_t (*out)[2])
{
	size_t count = 0, changes = 0;
	const int64_t top = p->clock_top;

	for (size_t index = 0; index < p->tempo_count; index++)
		if (p->tempos[index].tick <= end)
			changes++;
	for (size_t index = 0, change = 0; index < p->tempo_count; index++)
	{
		const tempo_change *now = &p->tempos[index];
		int64_t stop;
		double cycles;

		if (now->tick > end)
			continue;
		stop = change + 1 < changes ? -1 : (end < top ? end : top);
		if (stop < 0)
			for (size_t next = index + 1; next < p->tempo_count; next++)
				if (p->tempos[next].tick <= end)
				{
					stop = p->tempos[next].tick;
					break;
				}
		change++;
		if (!now->tempo)
			continue;
		if (stop > now->tick && stop >= 0 && stop <= top)
		{
			int64_t at_stop = -1, at_tick = -1;

			/* (a tick no pass reached first has the clock of the one before it) */
			for (int64_t tick = stop; tick >= 0 && at_stop < 0; tick--)
				at_stop = p->clocks[tick];
			for (int64_t tick = now->tick; tick >= 0 && at_tick < 0; tick--)
				at_tick = p->clocks[tick];
			cycles = (double)(at_stop - at_tick) / (double)(stop - now->tick);
		}
		else
			cycles = 256.0 / (double)now->tempo * (double)TIMER_B_SAMPLES * 64.0;
		out[count][0] = now->tick;
		out[count][1] = (int64_t)nearbyint(cycles * (double)TICKS_A_QUARTER * 1000000.0 / (double)SOUND_CLOCK);
		count++;
	}
	return count;
}

/* ---- the places' names (sound_music.py Names) ---- */

static int place_name(uint32_t address, char *text, size_t size)
{
	const extract_sound_piece *found = NULL;

	for (size_t index = 0; index < extract_sound_piece_count; index++)
		if (extract_sound_pieces[index].first <= address)
			found = &extract_sound_pieces[index];
	if (!found || address >= found->stop)
	{
		snprintf(extract_problem, sizeof(extract_problem), "the music: %04X is in no piece of the sound program", (unsigned)address);
		failed = 1;
		return 0;
	}
	if (address == found->first)
		snprintf(text, size, "%s", found->name);
	else
		snprintf(text, size, "%s+0x%X", found->name, (unsigned)(address - found->first));
	return 1;
}

static const extract_sound_piece *piece_named(const char *name)
{
	for (size_t index = 0; index < extract_sound_piece_count; index++)
		if (!strcmp(extract_sound_pieces[index].name, name))
			return &extract_sound_pieces[index];
	return NULL;
}

/* ---- the notes' keys (sound_music.py replay_keys) ---- */

typedef struct
{
	int64_t tick;
	int number;
	size_t index;
} read_order;

static int by_order(const void *one, const void *other)
{
	const read_order *a = one, *b = other;

	if (a->tick != b->tick)
		return a->tick < b->tick ? -1 : 1;
	if (a->number != b->number)
		return a->number < b->number ? -1 : 1;
	return a->index < b->index ? -1 : a->index > b->index;
}

typedef struct
{
	uint32_t keys[256][128];    /* each FM instrument's keys, counted */
} played_keys;

static int replay_keys(player *p, const uint8_t *drum_levels, const uint8_t *instrument_levels, played_keys *played)
{
	music_state music = { 0 };
	channel_record channels[SONG_TRACKS];
	size_t total = 0, at = 0;
	read_order *order;

	for (int number = 0; number < SONG_TRACKS; number++)
	{
		memset(&channels[number], 0, sizeof(channels[number]));
		channels[number].number = number;
		channels[number].pcm = number >= 7;
		total += p->channels[number].read_count;
	}
	if (!(order = malloc((total + 1) * sizeof(*order))))
	{
		failed = 1;
		return 0;
	}
	for (int number = 0; number < SONG_TRACKS; number++)
		for (size_t index = 0; index < p->channels[number].read_count; index++)
			order[at++] = (read_order){ p->channels[number].reads[index].tick, number, index };
	qsort(order, total, sizeof(*order), by_order);
	for (size_t entry = 0; entry < total; entry++)
	{
		player_channel *source = &p->channels[order[entry].number];
		channel_record *channel = &channels[order[entry].number];
		track_read *read = &source->reads[order[entry].index];
		uint32_t nibble;
		int key, velocity;

		for (size_t item = 0; item < read->item_count; item++)
		{
			const command_item *command = &source->items[read->first_item + item];

			execute(channel, &music, command->at, command->length, command->target);
		}
		if (read->step != STEP_NOTE && read->step != STEP_HELD)
			continue;
		if (read->step == STEP_NOTE)
			nibble = image_byte(read->step_at) & 0x0Fu;
		else if (channel->pcm && !drums(channel))
			nibble = image_byte(read->step_at + read->step_length - 1) & 0x0Fu;
		else
			nibble = image_byte(read->step_at + 1) & 0x0Fu;
		key = key_of(channel, &music, nibble);
		if (key == NO_KEY || nibble_of(channel, &music, key) != (int)nibble || key != read->key)
		{
			snprintf(extract_problem, sizeof(extract_problem), "the music: %s, tick %lld: the pitch %u has no key",
				channel_names[order[entry].number], (long long)read->tick, (unsigned)nibble);
			free(order);
			failed = 1;
			return 0;
		}
		if (channel->pcm)
		{
			const int level = drums(channel) ? drum_levels[nibble - 1] : instrument_levels[channel->instrument & 3u];
			const int sum = (int)((channel->volume + channel->attenuation + channel->loop_volume_offset) & 0xFFu);
			const int difference = level - sum > 0 ? level - sum : 0;

			velocity = (int)nearbyint(127.0 * sqrt((double)difference / 15.0));
		}
		else
		{
			const uint32_t sum = (music.volume + channel->volume_offset + channel->loop_volume_offset + channel->attenuation
				+ channel->volume) & 0xFFu;

			velocity = extract_fm_velocities[sum & 0x80u ? 0x7Fu : sum];
			played->keys[channel->instrument & 0xFFu][key]++;
		}
		read->note_key = key;
		read->velocity = velocity < 1 ? 1 : velocity > 127 ? 127 : velocity;
		read->note_channel = midi_channel(channel);
		read->note_released = released(channel, read->ticks);
	}
	free(order);
	return 1;
}

/* ---- the files ---- */

typedef struct
{
	int64_t tick;
	int note_off;
	size_t sequence, at, size;
} midi_event;

typedef struct
{
	midi_event *events;
	size_t count, capacity;
	extract_buffer data;
} midi_track;

static void vlq(extract_buffer *out, uint64_t value)
{
	uint8_t bytes[10];
	int count = 0;

	bytes[count++] = (uint8_t)(value & 0x7Fu);
	for (value >>= 7; value; value >>= 7)
		bytes[count++] = (uint8_t)(0x80u | (value & 0x7Fu));
	while (count > 0)
		buffer_byte(out, bytes[--count]);
}

/* an event's bytes from 'from' on in the track's data */
static void add_event(midi_track *track, int64_t tick, size_t from, int note_off)
{
	track->events = grow_list(track->events, &track->capacity, track->count + 1, sizeof(*track->events));
	if (failed)
		return;
	track->events[track->count] = (midi_event){ tick, note_off, track->count, from, track->data.size - from };
	track->count++;
}

static void add_meta(midi_track *track, int64_t tick, uint8_t kind, const void *bytes, size_t size)
{
	const size_t from = track->data.size;

	buffer_byte(&track->data, 0xFF);
	buffer_byte(&track->data, kind);
	vlq(&track->data, size);
	buffer_bytes(&track->data, bytes, size);
	add_event(track, tick, from, 0);
}

/* a driver's event: FF 7F LEN 7D TAG DATA, DATA the bytes and then 'text' */
static void add_driver(midi_track *track, int64_t tick, char tag, const uint8_t *bytes, size_t size, const char *text)
{
	uint8_t body[NAME_SIZE + 64];
	const size_t text_size = text ? strlen(text) : 0;

	if (2 + size + text_size > sizeof(body))
	{
		failed = 1;
		return;
	}
	body[0] = TAG;
	body[1] = (uint8_t)tag;
	if (size)
		memcpy(body + 2, bytes, size);
	if (text_size)
		memcpy(body + 2 + size, text, text_size);
	add_meta(track, tick, 0x7F, body, 2 + size + text_size);
}

static void add_short(midi_track *track, int64_t tick, const uint8_t *bytes, size_t size, int note_off)
{
	const size_t from = track->data.size;

	buffer_bytes(&track->data, bytes, size);
	add_event(track, tick, from, note_off);
}

static int by_event(const void *one, const void *other)
{
	const midi_event *a = one, *b = other;

	if (a->tick != b->tick)
		return a->tick < b->tick ? -1 : 1;
	if (a->note_off != b->note_off)
		return a->note_off ? -1 : 1;
	return a->sequence < b->sequence ? -1 : a->sequence > b->sequence;
}

/* the track's chunk: its events by tick, a tick's Note Offs first, in the order they were added */
static void track_chunk(midi_track *track, extract_buffer *file)
{
	extract_buffer body = { 0 };
	int64_t now = 0;
	uint8_t head[8];

	qsort(track->events, track->count, sizeof(*track->events), by_event);
	for (size_t index = 0; index < track->count; index++)
	{
		const midi_event *event = &track->events[index];

		vlq(&body, (uint64_t)(event->tick - now));
		buffer_bytes(&body, track->data.data + event->at, event->size);
		now = event->tick;
	}
	buffer_bytes(&body, "\x00\xFF\x2F\x00", 4);
	memcpy(head, "MTrk", 4);
	head[4] = (uint8_t)(body.size >> 24);
	head[5] = (uint8_t)(body.size >> 16);
	head[6] = (uint8_t)(body.size >> 8);
	head[7] = (uint8_t)body.size;
	buffer_bytes(file, head, 8);
	buffer_bytes(file, body.data, body.size);
	if (body.failed)
		failed = 1;
	buffer_free(&body);
	free(track->events);
	buffer_free(&track->data);
	memset(track, 0, sizeof(*track));
}

/* ---- the songs ---- */

typedef struct
{
	char name[64];
	uint8_t numbers[64];
	size_t number_count;
	uint32_t starts[SONG_TRACKS];
	player *play;
	int64_t end;
	extract_buffer unshown;     /* the 'U' events' places and bytes: a place's name, 0, its byte count (u32), the bytes */
} song;

static const extract_song_use *song_use(uint32_t command)
{
	for (size_t index = 0; index < extract_song_use_count; index++)
		if (extract_song_uses[index].command == command)
			return &extract_song_uses[index];
	return NULL;
}

/* the songs table's entry a piece's bytes belong to, by its name (songs_N_..., song_xx_...); -1: none */
static int song_owner(const char *name)
{
	if (!strncmp(name, "songs_", 6) && name[6] >= '0' && name[6] <= '9')
	{
		const char *at = name + 6;
		int number = 0;

		while (*at >= '0' && *at <= '9')
			number = number * 10 + (*at++ - '0');
		if (*at == '_')
			return number;
	}
	if (!strncmp(name, "song_", 5))
	{
		const char *at = name + 5;
		int digits = 0, value = 0;

		for (; digits < 2; digits++, at++)
		{
			if (*at >= '0' && *at <= '9')
				value = value * 16 + (*at - '0');
			else if (*at >= 'a' && *at <= 'f')
				value = value * 16 + (*at - 'a' + 10);
			else
				break;
		}
		if (digits == 2 && *at == '_')
			return value - FIRST_SONG;
	}
	return -1;
}

static void free_player(player *p)
{
	if (!p)
		return;
	for (int index = 0; index < SONG_TRACKS; index++)
	{
		free(p->channels[index].reads);
		free(p->channels[index].items);
		free(p->channels[index].seen.words);
		free(p->channels[index].seen.taken);
	}
	free(p->tempos);
	free(p->clocks);
	free(p);
}

static int write_song(const char *folder, song *each, const int *programs)
{
	player *p = each->play;
	extract_buffer file = { 0 };
	midi_track track = { 0 };
	int64_t (*tempos)[2] = malloc((p->tempo_count + 1) * sizeof(*tempos));
	char text[NAME_SIZE + 400], relative[128];
	size_t tempo_count, at;
	int ok;

	if (!tempos)
	{
		failed = 1;
		return 0;
	}
	/* the song's track: its name, 'N', the tempos, the bytes no file shows */
	{
		size_t length = (size_t)snprintf(text, sizeof(text), "TMNT %s: the songs table's ", each->name);

		for (size_t index = 0; index < each->number_count; index++)
			length += (size_t)snprintf(text + length, sizeof(text) - length, "%s%u (command %02X)", index ? ", " : "",
				(unsigned)each->numbers[index], (unsigned)(FIRST_SONG + each->numbers[index]));
		add_meta(&track, 0, 0x03, text, strlen(text));
	}
	add_driver(&track, 0, 'N', each->numbers, each->number_count, NULL);
	tempo_count = tempo_events(p, each->end, tempos);
	for (size_t index = 0; index < tempo_count; index++)
	{
		const int64_t quarter = tempos[index][1] < 0xFFFFFF ? tempos[index][1] : 0xFFFFFF;
		const uint8_t bytes[3] = { (uint8_t)(quarter >> 16), (uint8_t)(quarter >> 8), (uint8_t)quarter };

		add_meta(&track, tempos[index][0], 0x51, bytes, 3);
	}
	free(tempos);
	for (at = 0; at < each->unshown.size;)
	{
		const char *place = each->unshown.data + at;
		const size_t name_size = strlen(place) + 1;
		const uint8_t *size_bytes = (const uint8_t *)each->unshown.data + at + name_size;
		const uint32_t count = size_bytes[0] | (uint32_t)size_bytes[1] << 8 | (uint32_t)size_bytes[2] << 16 | (uint32_t)size_bytes[3] << 24;
		const size_t from = track.data.size;
		extract_buffer body = { 0 };

		buffer_byte(&body, TAG);
		buffer_byte(&body, 'U');
		buffer_bytes(&body, place, name_size);    /* (the name and its 0) */
		buffer_bytes(&body, size_bytes + 4, count);
		buffer_byte(&track.data, 0xFF);
		buffer_byte(&track.data, 0x7F);
		vlq(&track.data, body.size);
		buffer_bytes(&track.data, body.data, body.size);
		add_event(&track, 0, from, 0);
		buffer_free(&body);
		at += name_size + 4 + count;
	}
	buffer_bytes(&file, "MThd\x00\x00\x00\x06\x00\x01", 10);
	buffer_byte(&file, 0);
	buffer_byte(&file, 1 + SONG_TRACKS);
	buffer_byte(&file, 0);
	buffer_byte(&file, TICKS_A_QUARTER);
	track_chunk(&track, &file);
	/* the driver's channels */
	for (int number = 0; number < SONG_TRACKS && !failed; number++)
	{
		const player_channel *channel = &p->channels[number];

		add_meta(&track, 0, 0x03, channel_names[number], strlen(channel_names[number]));
		if (!place_name(each->starts[number], text, sizeof(text)))
			break;
		add_driver(&track, 0, 'S', NULL, 0, text);
		if (number < 7)
		{
			const uint8_t change[2] = { (uint8_t)(0xC0 | (number + 1)), (uint8_t)programs[0] };

			add_short(&track, 0, change, 2, 0);
		}
		else
		{
			const uint8_t change[2] = { (uint8_t)(number == 7 ? 0xC8 : 0xCA), PCM_BASS };

			add_short(&track, 0, change, 2, 0);
		}
		for (size_t index = 0; index < channel->read_count && !failed; index++)
		{
			const track_read *read = &channel->reads[index];

			for (size_t item = 0; item < read->item_count && !failed; item++)
			{
				const command_item *command = &channel->items[read->first_item + item];
				const uint32_t first = image_byte(command->at), where = address_at(first);
				uint8_t bytes[8];

				for (uint32_t byte = 0; byte < command->length && byte < sizeof(bytes); byte++)
					bytes[byte] = (uint8_t)image_byte(command->at + byte);
				if (where)
				{
					if (!place_name(command->target, text, sizeof(text)))
						break;
					add_driver(&track, read->tick, 'C', bytes, where, text);
				}
				else
					add_driver(&track, read->tick, 'C', bytes, command->length, NULL);
				if (first == 0xE2 && number < 7)
				{
					const uint8_t change[2] = { (uint8_t)(0xC0 | (number + 1)), (uint8_t)programs[bytes[1]] };

					add_short(&track, read->tick, change, 2, 0);
				}
			}
			if (read->step == STEP_REST)
				add_driver(&track, read->tick, 'R', NULL, 0, NULL);
			else if (read->step == STEP_CUT)
				add_driver(&track, read->tick, 'Z', NULL, 0, NULL);
			else if (read->step == STEP_NOTE || read->step == STEP_HELD)
			{
				const uint8_t on[3] = { (uint8_t)(0x90 | read->note_channel), (uint8_t)read->note_key, (uint8_t)read->velocity };
				const uint8_t off[3] = { (uint8_t)(0x80 | read->note_channel), (uint8_t)read->note_key, 0 };

				if (read->step == STEP_HELD)
				{
					uint8_t held[64];

					for (uint32_t byte = 1; byte < read->step_length && byte < sizeof(held); byte++)
						held[byte - 1] = (uint8_t)image_byte(read->step_at + byte);
					add_driver(&track, read->tick, 'H', held, read->step_length - 1, NULL);
				}
				add_short(&track, read->tick, on, 3, 0);
				add_short(&track, read->tick + read->note_released, off, 3, 1);
			}
		}
		track_chunk(&track, &file);
	}
	snprintf(relative, sizeof(relative), "sound/music/%s.mid", each->name);
	ok = !failed && !file.failed && extract_write_bytes(folder, relative, file.data, file.size);
	buffer_free(&file);
	free(track.events);
	buffer_free(&track.data);
	return ok;
}

int extract_music(const char *folder)
{
	const extract_sound_piece *lengths_piece = piece_named("note_lengths"), *drums_piece = piece_named("pcm_drums");
	const extract_sound_piece *instruments_piece = piece_named("pcm_instruments"), *table_piece = piece_named("songs");
	uint8_t lengths_bytes[256], drum_levels[15], instrument_levels[4];
	length_table lengths;
	song *songs = NULL;
	size_t song_count = 0, entry_count;
	int *file_of = NULL, programs[256];
	played_keys *played = NULL;
	uint8_t *shown_bytes = NULL;
	extract_buffer listing = { 0 };
	int ok = 0;

	failed = 0;
	if (!lengths_piece || !drums_piece || !instruments_piece || !table_piece || lengths_piece->stop - lengths_piece->first > 256)
	{
		snprintf(extract_problem, sizeof(extract_problem), "the music: the sound program's layout has no note_lengths, "
			"pcm_drums, pcm_instruments or songs");
		return 0;
	}
	lengths.count = lengths_piece->stop - lengths_piece->first;
	for (uint32_t index = 0; index < lengths.count; index++)
		lengths_bytes[index] = (uint8_t)image_byte(lengths_piece->first + index);
	lengths.lengths = lengths_bytes;
	for (uint32_t index = 0; index < 15; index++)
		drum_levels[index] = (uint8_t)image_byte(drums_piece->first + 6 * index + 3);
	for (uint32_t index = 0; index < 4; index++)
		instrument_levels[index] = (uint8_t)image_byte(instruments_piece->first + 4 * index + 3);
	entry_count = (size_t)(table_piece->stop - table_piece->first) / (2 * SONG_TRACKS);
	songs = calloc(entry_count + 1, sizeof(*songs));
	file_of = calloc(entry_count + 1, sizeof(*file_of));
	played = calloc(1, sizeof(*played));
	shown_bytes = calloc(0x10000, 1);
	if (!songs || !file_of || !played || !shown_bytes)
	{
		failed = 1;
		goto done;
	}

	/* a file for each different set of tracks, named by the first of its songs the game names */
	for (size_t number = 0; number < entry_count; number++)
	{
		uint32_t starts[SONG_TRACKS];
		size_t found;

		for (int track = 0; track < SONG_TRACKS; track++)
		{
			const uint32_t at = table_piece->first + 2u * (uint32_t)(SONG_TRACKS * number + (size_t)track);

			starts[track] = image_byte(at) | image_byte(at + 1) << 8;
		}
		for (found = 0; found < song_count; found++)
			if (!memcmp(songs[found].starts, starts, sizeof(starts)))
				break;
		if (found == song_count)
			memcpy(songs[song_count++].starts, starts, sizeof(starts));
		if (songs[found].number_count < sizeof(songs[found].numbers))
			songs[found].numbers[songs[found].number_count++] = (uint8_t)number;
		file_of[number] = (int)found;
	}
	for (size_t index = 0; index < song_count && !failed; index++)
	{
		song *each = &songs[index];
		const extract_song_use *named = NULL;
		player *p;

		for (size_t number = 0; number < each->number_count && !named; number++)
			named = song_use(FIRST_SONG + each->numbers[number]);
		if (named)
			snprintf(each->name, sizeof(each->name), "%s", named->file);
		else
			snprintf(each->name, sizeof(each->name), "song_%02x", (unsigned)(FIRST_SONG + each->numbers[0]));
		if (!(p = each->play = calloc(1, sizeof(*p))))
		{
			failed = 1;
			break;
		}
		p->lengths = &lengths;
		p->last_tempo = -1;
		p->clock = 3 * 1024 + 300;
		p->clock_top = -1;
		set_clock(p, 0, p->clock);
		for (int track = 0; track < SONG_TRACKS; track++)
		{
			player_channel *channel = &p->channels[track];

			channel->record.number = track;
			channel->record.pcm = track >= 7;
			channel->record.place = each->starts[track];
			channel->handler = 1;
			channel->done = -1;
		}
		each->end = run(p);
		if (failed)
			break;
		shown(p, each->end);
		if (!failed)
			replay_keys(p, drum_levels, instrument_levels, played);
	}
	if (failed)
		goto done;

	/* the FM instruments' programs: a bass below key 48 (the median of the keys it plays) */
	for (int instrument = 0; instrument < 256; instrument++)
	{
		uint64_t count = 0, seen = 0;

		programs[instrument] = FM_LEAD;
		for (int key = 0; key < 128; key++)
			count += played->keys[instrument][key];
		if (!count)
			continue;
		for (int key = 0; key < 128; key++)
		{
			seen += played->keys[instrument][key];
			if (seen > count / 2)
			{
				programs[instrument] = key < 48 ? FM_BASS : FM_LEAD;
				break;
			}
		}
	}

	/* the bytes no file shows, in the file of the song their piece belongs to */
	for (size_t index = 0; index < song_count; index++)
		for (int number = 0; number < SONG_TRACKS; number++)
		{
			const player_channel *channel = &songs[index].play->channels[number];

			for (size_t read = 0; read < channel->read_count; read++)
			{
				const track_read *each = &channel->reads[read];

				for (size_t item = 0; item < each->item_count; item++)
				{
					const command_item *command = &channel->items[each->first_item + item];

					for (uint32_t byte = 0; byte < command->length; byte++)
						shown_bytes[(command->at + byte) & 0xFFFFu] = 1;
				}
				if (each->step == STEP_NOTE || each->step == STEP_REST || each->step == STEP_HELD)
					for (uint32_t byte = 0; byte < each->step_length; byte++)
						shown_bytes[(each->step_at + byte) & 0xFFFFu] = 1;
			}
		}
	{
		int owner = -1;

		for (size_t index = 0; index < extract_sound_piece_count && !failed; index++)
		{
			const extract_sound_piece *piece = &extract_sound_pieces[index];
			const int named = song_owner(piece->name);
			uint32_t run_first = 0, run_stop = 0;
			int in_run = 0;

			if (strncmp(piece->kind, "music", 5) != 0)
				continue;
			if (named >= 0)
				owner = named;
			for (uint32_t address = piece->first; address <= piece->stop; address++)
			{
				if (address < piece->stop && !shown_bytes[address])
				{
					if (!in_run)
						run_first = address;
					in_run = 1;
					run_stop = address + 1;
					continue;
				}
				if (in_run)
				{
					char name[NAME_SIZE];
					song *into;
					const uint32_t count = run_stop - run_first;
					const uint8_t size_bytes[4] = { (uint8_t)count, (uint8_t)(count >> 8), (uint8_t)(count >> 16), (uint8_t)(count >> 24) };

					if (owner < 0 || (size_t)owner >= entry_count)
					{
						snprintf(extract_problem, sizeof(extract_problem), "the music: %s belongs to no song", piece->name);
						failed = 1;
						break;
					}
					into = &songs[file_of[owner]];
					if (!place_name(run_first, name, sizeof(name)))
						break;
					buffer_bytes(&into->unshown, name, strlen(name) + 1);
					buffer_bytes(&into->unshown, size_bytes, 4);
					for (uint32_t at = run_first; at < run_stop; at++)
						buffer_byte(&into->unshown, (uint8_t)image_byte(at));
					in_run = 0;
				}
			}
		}
	}
	if (failed)
		goto done;

	/* the files and music.txt */
	buffer_text(&listing, "# the songs (tools/sound_music.py): each MIDI file, the songs table's entries it plays (their commands\n");
	buffer_text(&listing, "# 60 on), and what the game plays it for\n");
	for (size_t index = 0; index < song_count && !failed; index++)
	{
		song *each = &songs[index];
		int uses = 0;

		if (!write_song(folder, each, programs))
		{
			failed = 1;
			break;
		}
		buffer_text(&listing, "%s  ", each->name);
		for (size_t number = 0; number < each->number_count; number++)
			buffer_text(&listing, number ? " %u" : "%u", (unsigned)each->numbers[number]);
		buffer_text(&listing, "  # ");
		for (size_t number = 0; number < each->number_count; number++)
			buffer_text(&listing, number ? ", %02X" : "%02X", (unsigned)(FIRST_SONG + each->numbers[number]));
		for (size_t number = 0; number < each->number_count; number++)
		{
			const extract_song_use *use = song_use(FIRST_SONG + each->numbers[number]);

			if (use)
				buffer_text(&listing, uses++ ? "; %s" : ": %s", use->use);
		}
		buffer_text(&listing, "\n");
	}
	ok = !failed && extract_write_text(folder, "sound/music/music.txt", &listing);
done:
	if (failed && !extract_problem[0])
		snprintf(extract_problem, sizeof(extract_problem), "the music cannot be written");
	for (size_t index = 0; songs && index < song_count; index++)
	{
		free_player(songs[index].play);
		buffer_free(&songs[index].unshown);
	}
	free(songs);
	free(file_of);
	free(played);
	free(shown_bytes);
	buffer_free(&listing);
	return ok && !failed;
}
