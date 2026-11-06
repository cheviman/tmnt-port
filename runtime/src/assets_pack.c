/**
 *    Name:        assets_pack.c
 *    Description: the game's assets as one file, the pack (tmnt.assets): written from the assets loaded, and
 *                 loaded in their place (game/assets.h)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    For the devices, which never take the assets out of a ROM set (the Switch's romfs, Android's APK, the
 *    Anbernic's folder): the pack is made on the player's computer from the player's assets, as the assets are,
 *    and never shared. Format (little endian, its parts 4-byte aligned): "TMAP", u32 the pack's format, u32 the
 *    tables' version (the names and addresses of the tables both programs' C reads by name: a build of another
 *    layout reads other places), then what assets.c loads, as it is in memory (assets_pack_put): the video
 *    chips' and the sound chips' memories, and the programs' pieces (name, address, file, bytes) and names.
 */
#include "game/assets.h"

#include "game/support.h"
/* (the two programs' supports name their own table macros alike: none of them is used here) */
#undef TABLE8
#undef TABLE16
#undef IN_PAGE
#include "sound/support.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACK_MAGIC      "TMAP"
#define PACK_FORMAT     1u

static uint32_t fnv(uint32_t hash, const void *bytes, size_t size)
{
	const uint8_t *byte = bytes;

	for (size_t index = 0; index < size; index++)
		hash = (hash ^ byte[index]) * 16777619u;
	return hash;
}

/* the tables both programs' C reads by name, their names and addresses (FNV-1a): what a pack is made for */
static uint32_t tables_version(void)
{
	uint32_t hash = 2166136261u;

#ifndef TMNT_TRANSLATION_SCAFFOLD    /* (the translation scaffold reads the program's data by address alone) */
	for (size_t index = 0; index < game_table_count; index++)
	{
		const uint8_t address[4] = { (uint8_t)game_tables[index].address, (uint8_t)(game_tables[index].address >> 8),
			(uint8_t)(game_tables[index].address >> 16), (uint8_t)(game_tables[index].address >> 24) };

		hash = fnv(hash, game_tables[index].name, strlen(game_tables[index].name) + 1);
		hash = fnv(hash, address, 4);
	}
#endif
	for (size_t index = 0; index < sound_table_count; index++)
	{
		const uint8_t address[2] = { (uint8_t)sound_tables[index].address, (uint8_t)(sound_tables[index].address >> 8) };

		hash = fnv(hash, sound_tables[index].name, strlen(sound_tables[index].name) + 1);
		hash = fnv(hash, address, 2);
	}
	return hash;
}

static int put32(FILE *output, uint32_t value)
{
	const uint8_t bytes[4] = { (uint8_t)value, (uint8_t)(value >> 8), (uint8_t)(value >> 16), (uint8_t)(value >> 24) };

	return fwrite(bytes, 1, 4, output) == 4;
}

static uint32_t get32(const uint8_t *bytes)
{
	return bytes[0] | (uint32_t)bytes[1] << 8 | (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;
}

const char *assets_pack_write(const char *path)
{
	FILE *output = fopen(path, "wb");
	int written;

	if (!output)
		return "the assets pack cannot be written";
	written = fwrite(PACK_MAGIC, 1, 4, output) == 4 && put32(output, PACK_FORMAT) && put32(output, tables_version())
		&& assets_pack_put(output);
	if (fclose(output) != 0 || !written)
	{
		remove(path);
		return "the assets pack cannot be written";
	}
	return NULL;
}

const char *assets_load_pack(const char *path)
{
	FILE *input = fopen(path, "rb");
	uint8_t *bytes = NULL;
	const char *problem;
	long size = 0;

	if (!input)
		return "the assets pack cannot be read";
	if (fseek(input, 0, SEEK_END) != 0 || (size = ftell(input)) <= 0 || fseek(input, 0, SEEK_SET) != 0
		|| !(bytes = malloc((size_t)size)) || fread(bytes, 1, (size_t)size, input) != (size_t)size)
	{
		fclose(input);
		free(bytes);
		return "the assets pack cannot be read";
	}
	fclose(input);
	problem = assets_load_pack_bytes(bytes, (size_t)size);
	free(bytes);
	return problem;
}

const char *assets_load_pack_bytes(const uint8_t *bytes, size_t size)
{
	if (size < 12 || memcmp(bytes, PACK_MAGIC, 4) != 0)
		return "the assets pack is not one";
	if (get32(bytes + 4) != PACK_FORMAT || get32(bytes + 8) != tables_version())
		return "the assets pack is of another version of the game";
	return assets_pack_get(bytes + 12, size - 12u);
}
