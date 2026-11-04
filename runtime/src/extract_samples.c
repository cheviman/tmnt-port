/**
 *    Name:        extract_samples.c
 *    Description: the first run's sounds/: the title music, the K007232's samples and the uPD7759's speech as WAV
 *                 files, as tools/sound_assets.py writes them
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    title.wav: the title music's words (little endian, 3.10 floating point in their top 13 bits) as 16-bit samples
 *    at 20 kHz (ymfm's decode_fp). pcm/sample_NN.wav: the K007232's bytes up to each end mark (bit 7), 8 bits at
 *    8 kHz, and pcm.txt where each starts. speech/speech_NN.wav: each uPD7759 sample decoded to 16 bits at 20 kHz
 *    (MAME's upd7759.cpp steps) with its bytes in a 'rawd' chunk, and speech.txt the header and the bytes before
 *    the first sample. Nothing here depends on the analysis: the set's own bytes say it all.
 */
#include "extract_internal.h"

#include "game/data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE_RATE      20000u
#define SPEECH_RATE     20000u
#define PCM_RATE        8000u
#define SPEECH_CLOCK    640000u
#define SPEECH_SAMPLES  256         /* the most a header's last sample number gives */

static const int upd_step[16][16] = {
	{ 0, 0, 1, 2, 3, 5, 7, 10, 0, 0, -1, -2, -3, -5, -7, -10 },
	{ 0, 1, 2, 3, 4, 6, 8, 13, 0, -1, -2, -3, -4, -6, -8, -13 },
	{ 0, 1, 2, 4, 5, 7, 10, 15, 0, -1, -2, -4, -5, -7, -10, -15 },
	{ 0, 1, 3, 4, 6, 9, 13, 19, 0, -1, -3, -4, -6, -9, -13, -19 },
	{ 0, 2, 3, 5, 8, 11, 15, 23, 0, -2, -3, -5, -8, -11, -15, -23 },
	{ 0, 2, 4, 7, 10, 14, 19, 29, 0, -2, -4, -7, -10, -14, -19, -29 },
	{ 0, 3, 5, 8, 12, 16, 22, 33, 0, -3, -5, -8, -12, -16, -22, -33 },
	{ 1, 4, 7, 10, 15, 20, 29, 43, -1, -4, -7, -10, -15, -20, -29, -43 },
	{ 1, 4, 8, 13, 18, 25, 35, 53, -1, -4, -8, -13, -18, -25, -35, -53 },
	{ 1, 6, 10, 16, 22, 31, 43, 64, -1, -6, -10, -16, -22, -31, -43, -64 },
	{ 2, 7, 12, 19, 27, 37, 51, 76, -2, -7, -12, -19, -27, -37, -51, -76 },
	{ 2, 9, 16, 24, 34, 46, 64, 96, -2, -9, -16, -24, -34, -46, -64, -96 },
	{ 3, 11, 19, 29, 41, 57, 79, 117, -3, -11, -19, -29, -41, -57, -79, -117 },
	{ 4, 13, 24, 36, 50, 69, 96, 143, -4, -13, -24, -36, -50, -69, -96, -143 },
	{ 4, 16, 29, 44, 62, 85, 118, 175, -4, -16, -29, -44, -62, -85, -118, -175 },
	{ 6, 20, 36, 54, 76, 104, 144, 214, -6, -20, -36, -54, -76, -104, -144, -214 },
};
static const int upd_state[16] = { -1, -1, 0, 0, 1, 2, 2, 3, -1, -1, 0, 0, 1, 2, 2, 3 };

static void little(extract_buffer *out, uint32_t value, int bytes)
{
	for (int index = 0; index < bytes; index++)
		buffer_byte(out, (uint8_t)(value >> (8 * index)));
}

/* A mono WAV of 8-bit samples ('data' as it is) or 16-bit ones ('samples'), and a 'rawd' chunk after its data if
   'raw' has bytes (sound_assets.py write_riff) */
static int write_riff(const char *folder, const char *relative, const uint8_t *data, const int16_t *samples, size_t count,
	uint32_t rate, uint32_t bits, const uint8_t *raw, size_t raw_size)
{
	const size_t data_size = bits == 8 ? count : count * 2u;
	const size_t chunks = 8 + 16 + 8 + data_size + data_size % 2 + (raw_size ? 8 + raw_size + raw_size % 2 : 0);
	extract_buffer out = { 0 };
	int ok;

	buffer_bytes(&out, "RIFF", 4);
	little(&out, (uint32_t)(4 + chunks), 4);
	buffer_bytes(&out, "WAVEfmt ", 8);
	little(&out, 16, 4);
	little(&out, 1, 2);    /* (PCM) */
	little(&out, 1, 2);    /* (one channel) */
	little(&out, rate, 4);
	little(&out, rate * bits / 8u, 4);
	little(&out, bits / 8u, 2);
	little(&out, bits, 2);
	buffer_bytes(&out, "data", 4);
	little(&out, (uint32_t)data_size, 4);
	if (bits == 8)
		buffer_bytes(&out, data, count);
	else
		for (size_t index = 0; index < count; index++)
			little(&out, (uint16_t)samples[index], 2);
	if (data_size % 2)
		buffer_byte(&out, 0);
	if (raw_size)
	{
		buffer_bytes(&out, "rawd", 4);
		little(&out, (uint32_t)raw_size, 4);
		buffer_bytes(&out, raw, raw_size);
		if (raw_size % 2)
			buffer_byte(&out, 0);
	}
	ok = !out.failed && extract_write_bytes(folder, relative, out.data, out.size);
	if (out.failed)
		snprintf(extract_problem, sizeof(extract_problem), "no memory for %s", relative);
	buffer_free(&out);
	return ok;
}

/* ymfm's decode_fp: a 3.10 floating point sample to 16 bits */
static int16_t decode_fp(uint32_t code)
{
	int32_t value;

	code ^= 0x1E00u;
	value = (int32_t)((code << 6) & 0xFFFFu);
	if (value & 0x8000)
		value -= 0x10000;
	return (int16_t)(value >> ((code >> 10) & 7u));
}

/* a growing list of 16-bit samples */
typedef struct
{
	int16_t *samples;
	size_t count, capacity;
	int failed;
} sample_list;

static void add_sample(sample_list *list, int16_t sample)
{
	if (list->failed)
		return;
	if (list->count == list->capacity)
	{
		int16_t *grown;

		list->capacity = list->capacity * 2 + 4096;
		if (!(grown = realloc(list->samples, list->capacity * sizeof(*grown))))
		{
			list->failed = 1;
			return;
		}
		list->samples = grown;
	}
	list->samples[list->count++] = sample;
}

/* A uPD7759 sample (the byte at 'start' a dummy, its blocks after it) as 16-bit samples at SPEECH_RATE
   (sound_assets.py decode_speech) */
static void decode_speech(const uint8_t *data, size_t size, size_t start, sample_list *out)
{
	int64_t sample = 0;
	int state = 0, valid = 0;
	size_t at = start + 1;
	double carry = 0.0;

	while (at < size)
	{
		const uint32_t header = data[at++], kind = header & 0xC0u;

		if (kind == 0x00)
		{
			if (header == 0 && valid)
				break;
			for (uint32_t index = 0; index < ((header & 0x3Fu) + 1u) * 1024u * SPEECH_RATE / SPEECH_CLOCK; index++)
				add_sample(out, 0);
			sample = 0;
			state = 0;
		}
		else if (kind == 0x40 || kind == 0x80)
		{
			const uint32_t rate = (header & 0x3Fu) + 1u;
			uint32_t nibbles = 256;

			if (kind == 0x80)
				nibbles = (uint32_t)data[at++] + 1u;
			for (uint32_t index = 0; index < nibbles; index++)
			{
				int nibble;

				if (at + index / 2 >= size)
					break;
				nibble = (data[at + index / 2] >> (index % 2 == 0 ? 4 : 0)) & 15;
				sample += upd_step[state][nibble];
				state += upd_state[nibble];
				state = state < 0 ? 0 : state > 15 ? 15 : state;
				carry += (double)(rate * 4u * SPEECH_RATE) / (double)SPEECH_CLOCK;
				while (carry >= 1)
				{
					const int64_t level = sample * 128;

					add_sample(out, (int16_t)(level < -32768 ? -32768 : level > 32767 ? 32767 : level));
					carry -= 1;
				}
			}
			at += (nibbles + 1u) / 2u;
		}
		if (header)
			valid = 1;
	}
}

static int write_title(const char *folder)
{
	const size_t count = GAME_TITLE_SIZE / 2;
	int16_t *samples = malloc(count * sizeof(*samples));
	int ok;

	if (!samples)
	{
		snprintf(extract_problem, sizeof(extract_problem), "no memory for the title music");
		return 0;
	}
	for (size_t index = 0; index < count; index++)
		samples[index] = decode_fp((uint32_t)(game_title[2 * index] | game_title[2 * index + 1] << 8) >> 3);
	ok = write_riff(folder, "sounds/title.wav", NULL, samples, count, TITLE_RATE, 16, NULL, 0);
	free(samples);
	return ok;
}

static int write_pcm(const char *folder)
{
	extract_buffer list = { 0 };
	uint32_t start = 0, number = 0;
	int ok = 1;

	buffer_text(&list, "# the K007232's samples: number, first byte, end (after its end mark)\n");
	for (uint32_t at = 0; at <= GAME_PCM_SIZE && ok; at++)
	{
		char relative[64];
		uint32_t stop;

		if (at < GAME_PCM_SIZE && !(game_pcm[at] & 0x80))
			continue;
		if (at == GAME_PCM_SIZE && start >= GAME_PCM_SIZE)
			break;
		stop = at < GAME_PCM_SIZE ? at + 1 : GAME_PCM_SIZE;
		snprintf(relative, sizeof(relative), "sounds/pcm/sample_%02u.wav", (unsigned)number);
		ok = write_riff(folder, relative, game_pcm + start, NULL, stop - start, PCM_RATE, 8, NULL, 0);
		buffer_text(&list, "%02u %05X %05X\n", (unsigned)number, (unsigned)start, (unsigned)stop);
		number++;
		start = stop;
	}
	ok = ok && extract_write_text(folder, "sounds/pcm.txt", &list);
	buffer_free(&list);
	return ok;
}

static int by_value(const void *one, const void *other)
{
	const uint32_t a = *(const uint32_t *)one, b = *(const uint32_t *)other;

	return a < b ? -1 : a > b;
}

static int write_speech(const char *folder)
{
	const uint8_t *speech = game_speech;
	const uint32_t last = speech[0], table_end = 5u + 2u * (last + 1u);
	uint32_t offsets[SPEECH_SAMPLES], starts[SPEECH_SAMPLES + 1];
	size_t start_count = 0;
	extract_buffer lines = { 0 };
	int ok = 1;

	for (uint32_t index = 0; index <= last; index++)
		offsets[index] = ((uint32_t)speech[index * 2 + 5] << 8 | speech[index * 2 + 6]) << 1;
	memcpy(starts, offsets, (last + 1u) * sizeof(*starts));
	qsort(starts, last + 1u, sizeof(*starts), by_value);
	for (size_t index = 0; index <= last; index++)
		if (start_count == 0 || starts[start_count - 1] != starts[index])
			starts[start_count++] = starts[index];
	buffer_text(&lines, "# the uPD7759's header: the last sample's number, its signature, each sample's offset (by number);\n");
	buffer_text(&lines, "# then the bytes between the header and the first sample\n");
	buffer_text(&lines, "last %u\nsignature %02x%02x%02x%02x\noffsets", (unsigned)last, speech[1], speech[2], speech[3], speech[4]);
	for (uint32_t index = 0; index <= last; index++)
		buffer_text(&lines, " %05X", (unsigned)offsets[index]);
	buffer_text(&lines, "\nbefore ");
	for (uint32_t at = table_end; at < starts[0] && at < GAME_SPEECH_SIZE; at++)
		buffer_text(&lines, "%02x", speech[at]);
	buffer_text(&lines, "\n");
	for (size_t number = 0; number < start_count && ok; number++)
	{
		const uint32_t offset = starts[number];
		const uint32_t bound = number + 1 < start_count ? starts[number + 1] : GAME_SPEECH_SIZE;
		sample_list samples = { NULL, 0, 0, 0 };
		char relative[64];

		decode_speech(speech, GAME_SPEECH_SIZE, offset, &samples);
		snprintf(relative, sizeof(relative), "sounds/speech/speech_%02u.wav", (unsigned)number);
		if (samples.failed)
		{
			snprintf(extract_problem, sizeof(extract_problem), "no memory for %s", relative);
			ok = 0;
		}
		else
			ok = write_riff(folder, relative, NULL, samples.samples, samples.count, SPEECH_RATE, 16, speech + offset,
				bound > offset ? bound - offset : 0);
		free(samples.samples);
	}
	ok = ok && extract_write_text(folder, "sounds/speech.txt", &lines);
	buffer_free(&lines);
	return ok;
}

int extract_samples(const char *folder)
{
	return write_title(folder) && write_pcm(folder) && write_speech(folder);
}
