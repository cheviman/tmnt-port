/**
 *    Name:        extract_layout.h
 *    Description: the tables of extract_layout.c (tools/extract_layout.py writes it): what the first run takes from
 *                 the analysis of the ROM set, without its bytes
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_EXTRACT_LAYOUT_H
#define TMNT_EXTRACT_LAYOUT_H

#include <stddef.h>
#include <stdint.h>

#define EXTRACT_NO_PLACE 0xFFFFFFFFu    /* a sheet's palette no colour comes to: black */

/* a cell of a sheet: its code, the sheet's palette (0-15) it shows with, whether it is there as a neighbour of
   what the data or the scenarios took */
typedef struct
{
	uint16_t code;
	uint8_t slot;
	uint8_t unseen;
} extract_cell;

/* a sheet of tiles or sprites: its name, its cells in order (16 a row) and for each of its 16 palettes the place in
   the 68000 program of the 16 colours it shows with */
typedef struct
{
	const char *name;
	const extract_cell *cells;
	uint32_t count;
	uint32_t palettes[16];
} extract_sheet;

/* how a span of the 68000 program's data is written (tools/data_assets.py write_piece) */
enum
{
	EXTRACT_FRAME,          /* frame D N SPRITES BOX ATTACK [A B C D] */
	EXTRACT_END,            /* end N */
	EXTRACT_JUMP,           /* jump N TARGET */
	EXTRACT_SPRITES,        /* sprite CODE COLOUR FLIPS X Y, 8 bytes each */
	EXTRACT_PALETTE,        /* colours, 16 a line */
	EXTRACT_TEXT,           /* text "..." */
	EXTRACT_POINTERS,       /* ref NAME (4 a line when they all are) or u32 */
	EXTRACT_WORDS,          /* u16, 8 a line */
	EXTRACT_LONGS,          /* ref NAME or u32, one a line */
	EXTRACT_BYTES           /* u8, 16 a line */
};

typedef struct
{
	uint32_t first, stop;
	uint8_t format;
} extract_span;

/* a piece of the 68000 program's data: its name, its file (extract_data_groups), whether the names of addresses
   take it (the code the program reads as data is a piece no address is named after), its spans */
typedef struct
{
	const char *name;
	uint8_t group;
	uint8_t named;
	const extract_span *spans;
	uint32_t count;
} extract_data_piece;

typedef struct
{
	uint32_t address;
	const char *name;
} extract_function;

/* how a piece of the sound program's data is written (tools/sound_data_assets.py write_piece): the music's go
   into the songs' MIDI files */
enum
{
	EXTRACT_SOUND_ADDRESSES,    /* ref NAME or u16, one a line */
	EXTRACT_SOUND_WORDS,        /* u16, 8 a line */
	EXTRACT_SOUND_BYTES,        /* u8, 16 a line */
	EXTRACT_SOUND_MUSIC         /* a track, a part or the songs table */
};

typedef struct
{
	const char *name;
	const char *kind;
	uint16_t first, stop;
	uint8_t format;
} extract_sound_piece;

/* a command of the songs table the game names: its song's file, and what the game plays it for */
typedef struct
{
	uint8_t command;
	const char *file;
	const char *use;
} extract_song_use;

extern const extract_sheet extract_tiles_sheets[];
extern const size_t extract_tiles_sheet_count;
extern const extract_sheet extract_sprites_sheets[];
extern const size_t extract_sprites_sheet_count;
extern const char *const extract_data_groups[];
extern const size_t extract_data_group_count;
extern const extract_data_piece extract_data_pieces[];
extern const size_t extract_data_piece_count;
extern const extract_function extract_functions[];          /* by address */
extern const size_t extract_function_count;
extern const extract_sound_piece extract_sound_pieces[];    /* by address */
extern const size_t extract_sound_piece_count;
extern const extract_function extract_sound_functions[];    /* by address */
extern const size_t extract_sound_function_count;
extern const extract_song_use extract_song_uses[];
extern const size_t extract_song_use_count;
extern const uint8_t extract_fm_velocities[128];
extern const uint32_t extract_layout_version;

#endif
