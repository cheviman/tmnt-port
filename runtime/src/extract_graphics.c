/**
 *    Name:        extract_graphics.c
 *    Description: the first run's tiles/ and sprites/: the sheets as extract_assets.py writes them
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A sheet (tools/extract_assets.py write_sheet) is an 8-bit indexed PNG, 16 tiles or sprites a row: a pixel is the
 *    pen plus 16 times the sheet's palette the cell shows with, and its 256 colours are the 16 palettes' (each the
 *    16 colours of a place of the 68000 program's data, rgb555 as asset_formats.py has it, or black). Which codes,
 *    palettes and places each sheet has is extract_layout.c's; the pens are the video chips' memories as rom_set.c
 *    lays them out (tools/tmnt_graphics.py tile_pens, sprite_pens), and the pixels go into the PNG as Python's
 *    zlib.compress(raw, 9) packs them (deflate.c). tilesets.txt and sprites.txt list each sheet's codes.
 */
#include "extract_internal.h"
#include "extract_layout.h"

#include "deflate.h"
#include "game/data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLUMNS         16
#define TILE_BYTES      32
#define SPRITE_BYTES    128

static void big32(uint8_t *out, uint32_t value)
{
	out[0] = (uint8_t)(value >> 24);
	out[1] = (uint8_t)(value >> 16);
	out[2] = (uint8_t)(value >> 8);
	out[3] = (uint8_t)value;
}

static uint32_t crc32_update(uint32_t crc, const uint8_t *data, size_t size)
{
	static uint32_t table[256];

	if (table[1] == 0u)
		for (uint32_t index = 0; index < 256u; index++)
		{
			table[index] = index;
			for (int bit = 0; bit < 8; bit++)
				table[index] = (table[index] >> 1) ^ (0xEDB88320u & (0u - (table[index] & 1u)));
		}
	crc = ~crc;
	for (size_t index = 0; index < size; index++)
		crc = (crc >> 8) ^ table[(crc ^ data[index]) & 0xFFu];
	return ~crc;
}

/* a chunk: its length, its type, its body and the CRC of the two (asset_formats.py _png_chunk) */
static void put_chunk(extract_buffer *out, const char *type, const uint8_t *body, size_t size)
{
	uint8_t head[8], tail[4];

	big32(head, (uint32_t)size);
	memcpy(head + 4, type, 4);
	big32(tail, crc32_update(crc32_update(0u, head + 4, 4), body, size));
	buffer_bytes(out, head, 8);
	if (size)
		buffer_bytes(out, body, size);
	buffer_bytes(out, tail, 4);
}

/* the program's byte as the analysis' memory reads it: 0xFF past the program */
static uint32_t program_byte(uint32_t at)
{
	return at < GAME_PROGRAM_SIZE ? game_program[at] : 0xFFu;
}

/* the sheet's 256 colours: each palette's 16 from its place (a word of the program, rgb555), or black */
static void sheet_palette(const extract_sheet *sheet, uint8_t *palette)
{
	for (int slot = 0; slot < 16; slot++)
		for (int colour = 0; colour < 16; colour++)
		{
			uint8_t *entry = &palette[(slot * 16 + colour) * 3];
			const uint32_t place = sheet->palettes[slot];

			if (place == EXTRACT_NO_PLACE)
			{
				entry[0] = entry[1] = entry[2] = 0;
				continue;
			}
			{
				const uint32_t at = place + 2u * (uint32_t)colour;
				const uint32_t value = program_byte(at) << 8 | program_byte(at + 1);

				for (int component = 0; component < 3; component++)
					entry[component] = (uint8_t)(((value >> (5 * component)) & 0x1Fu) * 255u / 31u);
			}
		}
}

/* an 8 x 8 block of pens from the planar memory at 'base' (its rows 4 bytes apart, the most significant pen bit in
   the last byte) into 'pixels' at (left, top), each plus 'colour' */
static void block_pens(const uint8_t *base, uint8_t *pixels, size_t stride, uint32_t left, uint32_t top, uint8_t colour)
{
	for (uint32_t row = 0; row < 8; row++)
	{
		const uint8_t *planes = base + 4 * row;

		for (uint32_t x = 0; x < 8; x++)
		{
			const uint32_t shift = 7 - x;
			const uint32_t pen = ((planes[3] >> shift) & 1u) << 3 | ((planes[2] >> shift) & 1u) << 2
				| ((planes[1] >> shift) & 1u) << 1 | ((planes[0] >> shift) & 1u);

			pixels[(top + row) * stride + left + x] = (uint8_t)(pen + 16u * colour);
		}
	}
}

/* "0010-001F 0123": the codes of 'count' cells (those 'unseen' alone when it is 1, all of them when it is -1) */
static void ranges(extract_buffer *out, const extract_cell *cells, uint32_t count, int unseen)
{
	int first_part = 1;
	uint32_t index = 0;

	while (index < count)
	{
		uint32_t first, last;

		if (unseen >= 0 && cells[index].unseen != unseen)
		{
			index++;
			continue;
		}
		first = last = cells[index].code;
		for (index++; index < count; index++)
		{
			if (unseen >= 0 && cells[index].unseen != unseen)
				continue;
			if (cells[index].code != last + 1u)
				break;
			last = cells[index].code;
		}
		buffer_text(out, first_part ? "%04X" : " %04X", (unsigned)first);
		if (last != first)
			buffer_text(out, "-%04X", (unsigned)last);
		first_part = 0;
	}
}

static int write_sheet(const char *folder, const char *sub, const extract_sheet *sheet, int size)
{
	const uint32_t rows = (sheet->count + COLUMNS - 1u) / COLUMNS * (uint32_t)size;
	const uint32_t width = COLUMNS * (uint32_t)size;
	const size_t stride = width + 1u;
	uint8_t *raw = calloc((size_t)rows * stride + 1u, 1), *packed = NULL;
	uint8_t header[13], palette[256 * 3];
	extract_buffer png = { 0 };
	char relative[300];
	size_t packed_size = 0;
	int ok;

	if (!raw)
	{
		snprintf(extract_problem, sizeof(extract_problem), "no memory for %s/%s.png", sub, sheet->name);
		return 0;
	}
	/* the pixels after each row's filter byte (0: none), as raw has them */
	for (uint32_t cell = 0; cell < sheet->count; cell++)
	{
		const uint32_t left = (cell % COLUMNS) * (uint32_t)size + 1u, top = (cell / COLUMNS) * (uint32_t)size;
		const uint32_t code = sheet->cells[cell].code;
		const uint8_t colour = sheet->cells[cell].slot;

		if (size == 8)
			block_pens(&game_tiles[(size_t)code * TILE_BYTES], raw, stride, left, top, colour);
		else
		{
			const uint8_t *base = &game_sprites[(size_t)code * SPRITE_BYTES];

			block_pens(base, raw, stride, left, top, colour);
			block_pens(base + 32, raw, stride, left + 8u, top, colour);
			block_pens(base + 64, raw, stride, left, top + 8u, colour);
			block_pens(base + 96, raw, stride, left + 8u, top + 8u, colour);
		}
	}
	packed = deflate_zlib(raw, (size_t)rows * stride, &packed_size);
	free(raw);
	if (!packed)
	{
		snprintf(extract_problem, sizeof(extract_problem), "no memory for %s/%s.png", sub, sheet->name);
		return 0;
	}
	big32(header, width);
	big32(header + 4, rows);
	header[8] = 8;
	header[9] = 3;    /* (palette indexes) */
	header[10] = header[11] = header[12] = 0;
	sheet_palette(sheet, palette);
	buffer_bytes(&png, "\x89PNG\r\n\x1a\n", 8);
	put_chunk(&png, "IHDR", header, sizeof(header));
	put_chunk(&png, "PLTE", palette, sizeof(palette));
	put_chunk(&png, "IDAT", packed, packed_size);
	put_chunk(&png, "IEND", NULL, 0);
	free(packed);
	snprintf(relative, sizeof(relative), "%s/%s.png", sub, sheet->name);
	ok = !png.failed && extract_write_bytes(folder, relative, png.data, png.size);
	if (png.failed)
		snprintf(extract_problem, sizeof(extract_problem), "no memory for %s", relative);
	buffer_free(&png);
	return ok;
}

static int write_sheets(const char *folder, const char *sub, const char *listing, const extract_sheet *sheets, size_t count,
	int size)
{
	extract_buffer lines = { 0 };
	char relative[300];
	int ok = 1;

	buffer_text(&lines, "# %s: each sheet's codes in the order of its cells, %d a row; before a sheet, the codes\n", sub, COLUMNS);
	buffer_text(&lines, "# it has that no data uses and no scenario shows, taken with their neighbours (tools/extract_assets.py)\n");
	for (size_t index = 0; index < count && ok; index++)
	{
		const extract_sheet *sheet = &sheets[index];
		uint32_t unseen = 0;

		ok = write_sheet(folder, sub, sheet, size);
		for (uint32_t cell = 0; cell < sheet->count; cell++)
			unseen += sheet->cells[cell].unseen;
		if (unseen)
		{
			buffer_text(&lines, "# %s: %u unseen, ", sheet->name, (unsigned)unseen);
			ranges(&lines, sheet->cells, sheet->count, 1);
			buffer_text(&lines, "\n");
		}
		buffer_text(&lines, "%s  ", sheet->name);
		ranges(&lines, sheet->cells, sheet->count, -1);
		buffer_text(&lines, "\n");
	}
	snprintf(relative, sizeof(relative), "%s/%s", sub, listing);
	ok = ok && extract_write_text(folder, relative, &lines);
	buffer_free(&lines);
	return ok;
}

int extract_graphics(const char *folder)
{
	return write_sheets(folder, "tiles", "tilesets.txt", extract_tiles_sheets, extract_tiles_sheet_count, 8)
		&& write_sheets(folder, "sprites", "sprites.txt", extract_sprites_sheets, extract_sprites_sheet_count, 16);
}
