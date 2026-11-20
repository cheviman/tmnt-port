/**
 *    Name:        corrections.c
 *    Description: the slips of the arcade game's own data that the console version puts right
 *    Version:     0.1.0
 *    Created:     19-11-2026
 *    Modified:    19-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The arcade build (TMNT Arcade) keeps the program's data as the board has it, slips and all. The console version
 *    corrects these in the data it has loaded, once, before the program runs (console_attach), never in the assets'
 *    files, which both builds read:
 *    - Donatello lands with Raphael's sai at his feet (tmnt-port's issue 23, and the board's own: MAME shows it too).
 *      The pose a turtle lands in when the turtles fall into an area (areas 2, 3 and 7's entries, turtle_fall_in: his
 *      animation set's routine 2, state 0) has three pieces, and its first one's code is 3B56, a frame of the sai
 *      Raphael twirls, where the bo's lower end goes: 3B76, the piece the pose he comes down in has in the same
 *      place under the same hand (routine 2, state 3). One bit of the program's ROM, at 026FAF.
 *    A byte that is not the board's is left as it is: assets the player changed.
 */
#include "console_internal.h"

#include "game.h"
#include "game/assets.h"

/* A byte of the program's data, 'from' on the board: in its piece and in the piece's copy the other way round, where
   the records are read (game/assets.h) */
static void correct_byte(uint32_t address, uint8_t from, uint8_t to)
{
	uint8_t *const byte = (uint8_t *)assets_data_at(address);
	uint8_t *const in_record = (uint8_t *)assets_record_at(address, 1u);

	if (!byte || !in_record || *byte != from)
		return;
	*byte = to;
	*in_record = to;
}

#define DONATELLO_LANDING_PIECE (donatello_animation_set_sprites_ADDRESS + 0x2F0u)    /* (its size and code: 3B56) */

void console_correct_data(void)
{
	correct_byte(DONATELLO_LANDING_PIECE + 1u, 0x56u, 0x76u);
}
