/**
 *    Name:        assets.h
 *    Description: the game's assets (tools/extract_assets.py writes them from the player's ROM set) loaded
 *                 into the board's memories and the program's data, by name
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    What the game reads at start instead of the ROM: the tiles and the sprites (PNG sheets) into the video
 *    chips' memories (game/data.h's game_tiles, game_sprites, as the chips read them), the samples (WAV)
 *    into the sound chips' (game_pcm, game_speech, game_title), and the data of both programs (text pieces,
 *    each by name; the sound program's music and songs table from the songs' MIDI files) into pieces the code
 *    finds by name or by the address the pieces' layout gives them (the places the data's own pointers hold).
 */
#ifndef GAME_ASSETS_H
#define GAME_ASSETS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Loads every asset under 'folder' (assets/): NULL, or what is wrong */
const char *assets_load(const char *folder);

/* The assets pack (tmnt.assets): one file with what the assets load into, for the devices, which never take
   them out of a ROM set. assets_pack_write writes it from the assets loaded, assets_load_pack loads one in their
   place, if it was made for this build's tables (runtime/src/assets_pack.c). NULL, or what failed */
const char *assets_pack_write(const char *path);
const char *assets_load_pack(const char *path);

/* A pack already read whole (an Android APK's assets, which are no files): what assets_load_pack loads */
const char *assets_load_pack_bytes(const uint8_t *bytes, size_t size);

/* the pack's body, after its header: the memories and the programs' data as they are loaded, and back */
int assets_pack_put(FILE *output);
const char *assets_pack_get(const uint8_t *bytes, size_t size);

/* A piece of the 68000 program's data by its name (NULL if there is none), and its size */
const uint8_t *assets_piece(const char *name, size_t *size);

/* The program's data at an address of its layout (NULL outside every piece); the same for the sound
   program's (its Z80 addresses) */
const uint8_t *assets_data_at(uint32_t address);
const uint8_t *assets_sound_data_at(uint32_t address);

/* The 'size' bytes of the 68000 program's data from 'address' the other way round, as the host's place of a
   record of the big-endian program (its fields declared from its end, as the work RAM's structures): NULL
   unless they are all in one piece */
const void *assets_record_at(uint32_t address, uint32_t size);

/* The 68000 program's data laid out where its layout puts it, and the sound program's: whole images
   with every piece in place (the code's bytes are 0), and in 'placed' (if not NULL) a 1 at every byte a
   piece gives: for the tools that compare them with the ROM set */
void assets_program_image(uint8_t *image, uint8_t *placed, size_t size);
void assets_sound_image(uint8_t *image, uint8_t *placed, size_t size);

/* A piece of the sound program's data by its name (NULL if there is none), and its size: the tables its C
   reads by name (src/sound/tables.c) */
const uint8_t *assets_sound_piece(const char *name, size_t *size);

#ifdef __cplusplus
}
#endif

#endif
