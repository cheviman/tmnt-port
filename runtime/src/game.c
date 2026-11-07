/**
 *    Name:        game.c
 *    Description: the game as the ports run it: the board with the assets in place of the ROM, the program on
 *                 it, its frames drawn
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#include "game/game.h"

#include "game/assets.h"
#include "tmnt/machine.h"
#include "tmnt/program.h"
#include "tmnt/sound_board.h"
#include "tmnt/video.h"

#include <stdio.h>
#include <stdlib.h>

static tmnt_frame g_frame;
static bool g_draw;

/* The program read its own code as data, which the game does not have */
static void code_read(uint32_t address)
{
	fprintf(stderr, "68000 stopped: the program read its code at %06X as data (frame %llu)\n",
		(unsigned)address & 0xFFFFFFu, (unsigned long long)tmnt_board.frame);
	exit(3);
}

static void vblank_end(void)
{
	if (g_draw)
		tmnt_video_vblank_end();
}

static void vblank_start(void)
{
	if (g_draw)
		tmnt_video_vblank_start(&g_frame);
}

const char *game_load(const char *folder)
{
	return assets_load(folder);
}

const char *game_reset(bool service, bool draw)
{
	const char *problem;

	tmnt_reset();
	tmnt_board.program_at = assets_data_at;
	tmnt_board.program_outside = code_read;
	if (service)
		tmnt_board.dsw[2] &= (uint8_t)~TMNT_DSW3_SERVICE;
	if ((problem = tmnt_sound_init()) != NULL)
		return problem;
	tmnt_sound_attach();
	g_draw = draw;
	tmnt_video_reset();
	program_vblank_end = vblank_end;
	program_vblank_start = vblank_start;
	program_reset();
	return NULL;
}

void game_end_frame(void)
{
	tmnt_sound_end_frame(tmnt_board.cycles);
}

const tmnt_frame *game_frame(void)
{
	return &g_frame;
}

const int16_t *game_sound_samples(size_t *count)
{
	return tmnt_sound_mix(count);
}

uint32_t game_sound_rate(void)
{
	return tmnt_sound_mix_rate();
}
