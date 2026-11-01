/**
 *    Name:        system/palette_editor.c
 *    Description: an unused colour editor for the palette RAM, worked with the first player's joystick
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Unused: a colour editor for the palette, run a step at a time */
void palette_editor(void)
{
	switch (palette_editor_step)
	{
	case 0:
		palette_editor_start();
		break;
	case 1:
		palette_editor_frame();
		break;
	}
}

/* Unused: the colour editor's page, its numbers cleared */
void palette_editor_start(void)
{
	uint32_t jobs = 0;

	jobs = table_loaded_by_lea_009C5C_ADDRESS;
	queue_jobs(jobs);
	palette_editor_numbers(0) = 0u;
	palette_editor_numbers(1) = 0u;
	palette_editor_numbers(2) = 0u;
	palette_editor_numbers(3) = 0u;
	AT32(palette_editor_pick, -3) = 0u;
	palette_editor_last_pick = 1u;
	palette_editor_step++;
}

/* Unused: the joystick picks one of the colour editor's numbers and changes it */
void palette_editor_frame(void)
{
	uint8_t joystick = 0, limit = 0;
	uint16_t colour = 0, limit_choice = 0, number_offset = 0, part_offset = 0, pick = 0, stepped_colour = 0;
	uint32_t colour_place = 0, numbers = 0;
	uint8_t t1 = 0, t4 = 0;
	uint32_t t2 = 0, t3 = 0;

	if ((frame_count & 3u) != 0u)
		return;
	numbers = palette_editor_numbers_ADDRESS;
	palette_editor_draw(numbers);
	palette_editor_show_colour(numbers);
	palette_editor_last_pick = palette_editor_pick;
	mem_write8(6u, io_read8(IO_0A0003));
	mem_write8(6u, ~mem_read8(6u));
	joystick = mem_read8(6u) & 0xFu;
	if ((joystick & 2u) == 0u)
	{
		if ((joystick & 1u) != 0u)
		{
			t1 = palette_editor_pick;
			palette_editor_pick = t1 - 1u;
			if (t1 == 0u)
				palette_editor_pick = 5u;
		}
	}
	else
	{
		palette_editor_pick++;
		if (palette_editor_pick >= 6u)
			palette_editor_pick = 0u;
	}
	if (palette_editor_pick >= 3u)
	{
		pick = palette_editor_pick;
		palette_editor_part = palette_editor_parts[(uint8_t)(pick - 3u)];
		colour_place = palette_editor_entry(numbers);
		colour = (uint16_t)((uint32_t)io_read8(colour_place + 1u) << 8);
		colour |= (uint8_t)(colour + io_read8(colour_place + 3u));
		part_offset = palette_editor_part;
		part_offset = (uint8_t)((uint32_t)part_offset << 2);
		stepped_colour = (uint16_t)colour;
		if ((joystick & 4u) != 0u)
			stepped_colour += TABLE16(palette_editor_steps, part_offset);
		if ((joystick & 8u) != 0u)
			stepped_colour -= TABLE16(palette_editor_steps, part_offset);
		colour = (uint16_t)((colour & ~TABLE16(palette_editor_masks, part_offset)) + (stepped_colour & TABLE16(palette_editor_masks, part_offset)));
		io_write8(colour_place + 3u, (uint8_t)colour);
		io_write8(PALETTE_08000F, (uint8_t)colour);
		colour = colour >> 8;
		io_write8(colour_place + 1u, (uint8_t)colour);
		io_write8(PALETTE_08000D, (uint8_t)colour);
		return;
	}
	limit_choice = palette_editor_pick;
	if (palette_editor_numbers(1) != 3u)
	{
		limit_choice = (uint8_t)(limit_choice + 3u);
		if (palette_editor_numbers(2) >= 8u)
			palette_editor_numbers(2) = 7u;
	}
	limit = palette_editor_limits[limit_choice];
	number_offset = palette_editor_pick;
	number_offset = (uint8_t)(number_offset + 1u);
	if ((joystick & 8u) == 0u)
	{
		if ((joystick & 4u) != 0u)
		{
			t2 = numbers + number_offset;
			mem_write8(t2, mem_read8(t2) + 1u);
			if (limit < palette_editor_numbers(number_offset))
				palette_editor_numbers(number_offset) = 0u;
		}
	}
	else
	{
		t3 = numbers + number_offset;
		t4 = mem_read8(t3);
		mem_write8(t3, t4 - 1u);
		if (t4 == 0u)
			palette_editor_numbers(number_offset) = limit;
	}
}

/* Unused: the colour picked is shown */
void palette_editor_show_colour(uint32_t numbers)
{
	uint32_t colour_place = 0;

	colour_place = palette_editor_entry(numbers);
	io_write8(PALETTE_08000D, io_read8(colour_place + 1u));
	io_write8(PALETTE_08000F, io_read8(colour_place + 3u));
}

/* Unused: the colour editor's numbers drawn, the one picked in another colour */
void palette_editor_draw(uint32_t numbers)
{
	uint8_t part = 0;
	uint16_t bank_number = 0, colour = 0, colour_number = 0, palette_number = 0, pick = 0;
	uint32_t tile_place = 0;

	tile_place = palette_editor_place;
	bank_number = palette_editor_numbers(1);
	io_write8(tile_place, 0u);
	io_write8(tile_place + 1u, palette_editor_title[bank_number]);
	tile_place = palette_editor_number_places[0];
	palette_number = palette_editor_numbers(2);
	io_write8(tile_place, 0u);
	io_write8(tile_place + 1u, hex_digit_tiles[palette_number]);
	tile_place = palette_editor_number_places[1];
	colour_number = palette_editor_numbers(3);
	mem_write8(tile_place, 0u);
	mem_write8(tile_place + 1u, hex_digit_tiles[colour_number]);
	tile_place = palette_editor_number_places[2];
	colour = (uint16_t)((uint32_t)io_read8(PALETTE_08000D) << 8);
	colour |= (uint8_t)(colour + io_read8(PALETTE_08000F));
	part = (uint8_t)colour;
	palette_editor_draw_part(part, tile_place);
	colour = colour >> 5;
	part = (uint8_t)colour;
	tile_place += 6u;
	palette_editor_draw_part(part, tile_place);
	part = (uint8_t)(colour >> 5);
	tile_place += 6u;
	palette_editor_draw_part(part, tile_place);
	pick = palette_editor_pick;
	if (pick != palette_editor_last_pick)
	{
		tile_place = palette_editor_colour_places[pick & 0x3FFFu];
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, 0x1Fu);
		tile_place = palette_editor_colour_places[palette_editor_last_pick];
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, 0x10u);
	}
}

/* Unused: a part of a colour drawn as two digits */
void palette_editor_draw_part(uint8_t part, uint32_t tile_place)
{
	uint8_t high_digit = 0, low_digit_tile = 0;

	high_digit = 0u;
	part &= 0x1Fu;
	if ((part & 0x10u) != 0u)
		high_digit++;
	mem_write8(tile_place, 0u);
	mem_write8(tile_place + 1u, high_digit);
	low_digit_tile = hex_digit_tiles[part & 0xFu];
	mem_write8(tile_place + 2u, 0u);
	mem_write8(tile_place + 3u, low_digit_tile);
}

/* Unused: the palette entry the colour editor's numbers pick */
uint32_t palette_editor_entry(uint32_t numbers)
{
	uint32_t colour_place = 0;

	colour_place = palette_editor_banks[palette_editor_numbers(1)] + (uint16_t)((uint32_t)palette_editor_numbers(2) << 6) + (uint16_t)((uint32_t)palette_editor_numbers(3) << 2);
	return colour_place;
}
