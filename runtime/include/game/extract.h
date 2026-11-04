/**
 *    Name:        extract.h
 *    Description: the player's ROM set into the board's memory (game/data.h), and the game's first run: its assets
 *                 out of the ROM set
 *    Version:     0.1.1
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The ROM set is MAME's 'tmnt' (World 4 Players, version X), each of its 16 files known by its CRC32: in
 *    a .zip (stored or deflated) or a folder of its files (by MAME's names).
 */
#ifndef GAME_EXTRACT_H
#define GAME_EXTRACT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* the file the first run writes last in the assets' folder: the version of what made them */
#define GAME_ASSETS_VERSION_FILE "version.txt"

/* The ROM set at 'source' into game/data.h's regions, laid out as the board has them, the code too (for
   the reference, which runs it, and the first run, which takes the assets out of it): NULL, or what is wrong
   with it */
const char *game_rom_set_load(const char *source);

/* The game's first run: the assets out of the ROM set at 'source' (a .zip or a folder) into 'folder', the files
   tools/extract_assets.py writes, byte for byte, and GAME_ASSETS_VERSION_FILE last. NULL, or what failed */
const char *game_assets_extract(const char *source, const char *folder);

/* The version of what the first run writes (its tables and its writers): a folder whose version file says another
   one is taken out again */
uint32_t game_assets_version(void);

#ifdef __cplusplus
}
#endif

#endif
