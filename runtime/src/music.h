/**
 *    Name:        music.h
 *    Description: the songs' Standard MIDI Files back into the sound program's music (runtime/src/music.c)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_MUSIC_H
#define TMNT_MUSIC_H

#include <stddef.h>
#include <stdint.h>

/* the sound program's addresses */
#define MUSIC_SPACE 0x10000u

/* What the way back needs of the other assets: the places' names, note_lengths and the songs table's address */
typedef struct
{
	int (*address_of)(void *host, const char *name, uint32_t *address);   /* NAME or NAME+0xNN; 0: no piece has it */
	void (*name_of)(void *host, uint32_t address, char *text, size_t size);
	void *host;
	const uint8_t *lengths;
	size_t length_count;
	uint32_t songs_table;
} music_places;

/* The songs' MIDI files that 'folder'/music.txt lists, back into the music's and the songs table's bytes: 'image'
   and 'given' (MUSIC_SPACE each, by address) take them; NULL, or what failed */
const char *music_read(const char *folder, const music_places *places, uint8_t *image, uint8_t *given);

#endif
