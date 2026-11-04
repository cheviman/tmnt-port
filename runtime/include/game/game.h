/**
 *    Name:        game.h
 *    Description: the game as the ports run it: the board with the assets in place of the ROM, the program on
 *                 it, its frames drawn
 *    Version:     0.1.1
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The ports (ports/headless, ports/sdl2) load the assets, reset the game and run it; the program calls
 *    their program_frame_done (tmnt/program.h) after each frame's VBLANK events, where they apply the
 *    controls, show the frame, queue its sound and take its hashes. The program's data is the assets'
 *    pieces (game/assets.h): the game opens no ROM.
 */
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "tmnt/video.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* the assets pack's name and its ending (game/assets.h) */
#define GAME_PACK_FILE      "tmnt.assets"
#define GAME_PACK_EXTENSION ".assets"

/* Loads the assets in the folder 'place', or the pack 'place' (a name that ends in GAME_PACK_EXTENSION): NULL, or
   what failed */
const char *game_load(const char *place);

/* The board after its reset with the program's data from the assets, its sound board at power on, the
   program at its reset; the test mode's DIP switch on if 'service'. With 'draw', every frame is drawn
   (game_frame). NULL, or what failed */
const char *game_reset(bool service, bool draw);

/* Closes the frame's sound, which the ports call when the frame is done (after their controls): its
   samples mixed (game_sound_samples) and its hashes (tmnt/sound_board.h) */
void game_end_frame(void);

/* The frame drawn when the last VBLANK started */
const tmnt_frame *game_frame(void);

/* The last frame's sound, mixed to one speaker at game_sound_rate() samples a second */
const int16_t *game_sound_samples(size_t *count);
uint32_t game_sound_rate(void);

#ifdef __cplusplus
}
#endif

#endif
