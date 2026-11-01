/**
 *    Name:        video/tile_blocks.c
 *    Description: blocks of tiles drawn into the layers, and texts typed a character at a time
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a text to type, its first character at once */
uint32_t start_typed_text_at_once(uint32_t text)
{
	uint32_t colour_number = 0;

	start_typed_text(&colour_number, &text);
	typed_text_wait = 0u;
	return colour_number;
}

/* the typed text's next character, a new line, or its end */
void type_next_character(void)
{
	uint8_t text = 0;
	uint32_t ignored = 0, text_record = 0, tile_place = 0;
	uint8_t t1 = 0;

	if (typed_text == 0u)
		return;
	t1 = typed_text_wait;
	typed_text_wait = t1 - 1u;
	if (t1 != 0u)
		return;
	text_record = typed_text;
	tile_place = typed_text_place;
	text = TEXT(text_record);
	text_record++;
	if (text == 0x21u)
	{
		typed_text_place = 0u;
		typed_text = 0u;
		typed_text_wait = 0u;
		typed_text_delay = 0u;
		unused_typed_text_flag = 0u;
		typed_text_colour = 0u;
		return;
	}
	if (text == 0x2Fu)
	{
		start_typed_text(&ignored, &text_record);
		text = TEXT(text_record);
		tile_place = typed_text_place;
		text_record++;
	}
	io_write8(tile_place, typed_text_colour);
	tile_place++;
	io_write8(tile_place, text - 0x30u);
	typed_text = text_record;
	typed_text_place = tile_place + 1u;
	typed_text_wait = typed_text_delay;
}

/* a text typed a character at a time: its place, the frames between characters and its colour */
void start_typed_text(uint32_t *p_colour_number, uint32_t *p_text)
{
	uint32_t text = *p_text;
	uint32_t colour_number = 0;

	typed_text_place = TEXT_32(text);
	text += 4u;
	typed_text_delay = TEXT(text);
	text++;
	colour_number = TEXT(text);
	typed_text = text + 1u;
	unused_typed_text_flag = 0u;
	if ((int8_t)colour_number < 0)
	{
		colour_number &= ~0x80u;
		unused_typed_text_flag = 1u;
	}
	typed_text_colour = text_colours[colour_number];
	text = typed_text;
	*p_colour_number = colour_number;
	*p_text = text;
}

/* a block of tiles into the tile RAM at the place picked, wrapping round the layer */
void draw_tile_block(uint16_t place, uint32_t block)
{
	uint16_t column = 0, column_offset = 0, count = 0, count_2 = 0, width = 0;
	uint32_t first_row_place = 0, row_place = 0, tile_row = 0;
	uint16_t t1 = 0, t2 = 0;

	first_row_place = TABLE32(tile_block_places, (uint32_t)(int16_t)((uint32_t)place << 2));
	column_offset = (uint16_t)first_row_place;
	first_row_place &= ~0x7Fu;
	column_offset -= first_row_place;
	row_place = first_row_place;
	width = TILE_BLOCK_BYTE(block);
	block++;
	count = TILE_BLOCK_BYTE(block);
	block++;
	do
	{
		tile_row = row_place;
		count_2 = (uint16_t)width;
		column = (uint16_t)column_offset;
		do
		{
			mem_write8(tile_row + (int16_t)column, TILE_BLOCK_BYTE(block));
			block++;
			mem_write8(tile_row + 1u + (int16_t)column, TILE_BLOCK_BYTE(block));
			column = (column + 2u) & 0x7Fu;
			t1 = count_2 - 1u;
			count_2 = t1;
			block++;
		} while (t1 != 0xFFFFu);
		row_place = ((row_place + 0x80u) & 0xFFFu) | (row_place & ~0xFFFu);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
}

/* Unused: the spawn scroll's place shown on the screen in hex digits */
void unreached_01845E(void)
{
	uint16_t count = 0;
	uint32_t number = 0, tile_place = 0;

	count = 3u;
	number = spawn_x >> 8;
	tile_place = TILE_RAM + 0xE24u;
	draw_hex_digits(count, number, tile_place);
	count = 3u;
	number = spawn_y >> 8;
	tile_place = TILE_RAM + 0xE30u;
	draw_hex_digits(count, number, tile_place);
}

/* a number drawn in hexadecimal digits */
void draw_hex_digits(uint16_t count, uint32_t number, uint32_t tile_place)
{
	uint8_t tile_code = 0;
	uint16_t digit = 0;
	uint16_t t1 = 0;

	do
	{
		digit = number & 0xFu;
		number = number >> 4;
		tile_code = (uint8_t)(uint16_t)(hex_digits[digit] - 0x30u);
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, tile_code);
		t1 = count - 1u;
		count = t1;
		tile_place -= 2u;
	} while (t1 != 0xFFFFu);
}

/* a text typed eight times over, each pass in another colour and a little later */
void start_text_typed_in_colours(uint8_t delay_shift, uint32_t text, uint32_t extend)
{
	uint32_t carry = 0;
	uint8_t kept_shift = 0, pass_number = 0;
	uint16_t count = 0;
	uint32_t kept_text = 0, typed_pass = 0;
	uint8_t t1 = 0, t2 = 0;
	uint16_t t3 = 0;

	typed_pass = TYPED_PASS_ADDRESS(7);
	kept_shift = (uint8_t)delay_shift;
	count = 7u;
	do
	{
		kept_text = text;
		start_typed_line(typed_pass, text);
		text = kept_text;
		pass_number = (uint8_t)count;
		t1 = kept_shift;
		t2 = m68k_shift(M68K_LSL, pass_number, t1, extend, 8);
		carry = m68k_shift_carry(M68K_LSL, pass_number, t1, extend, 8);
		extend = t1 != 0u ? carry : extend;
		TYPED_PASS(typed_pass)->wait = t2;
		t3 = count - 1u;
		count = t3;
		typed_pass -= 0xAu;
	} while (t3 != 0xFFFFu);
	typed_passes_left = 8u;
}

/* every pass of the coloured text typed on */
void type_text_in_colours(void)
{
	uint16_t count = 0;
	uint32_t typed_pass = 0;
	uint16_t t1 = 0;

	typed_pass = TYPED_PASS_ADDRESS(7);
	count = 7u;
	do
	{
		type_colour_pass(count, typed_pass);
		t1 = count - 1u;
		count = t1;
		typed_pass -= 0xAu;
	} while (t1 != 0xFFFFu);
}

/* one pass's next character in its colour */
void type_colour_pass(uint16_t count, uint32_t typed_pass)
{
	uint8_t text_2 = 0;
	uint32_t text = 0, tile_place = 0;
	uint8_t wait = 0;

	if (TYPED_PASS(typed_pass)->text == 0u)
		return;
	wait = TYPED_PASS(typed_pass)->wait;
	TYPED_PASS(typed_pass)->wait = wait - 1u;
	if (wait != 0u)
		return;
	text = TYPED_PASS(typed_pass)->text;
	tile_place = TYPED_PASS(typed_pass)->place;
	text_2 = TEXT(text);
	text++;
	if (text_2 == 0x21u)
	{
		TYPED_PASS(typed_pass)->place = 0u;
		TYPED_PASS(typed_pass)->text = 0u;
		TYPED_PASS(typed_pass)->wait = 0u;
		TYPED_PASS(typed_pass)->delay = 0u;
		typed_passes_left--;
		return;
	}
	if (text_2 == 0x2Fu)
	{
		text = start_typed_line(typed_pass, text);
		text_2 = TEXT(text);
		tile_place = TYPED_PASS(typed_pass)->place;
		text++;
	}
	io_write8(tile_place, TABLE8(text_colours, (uint32_t)(int16_t)count));
	tile_place++;
	io_write8(tile_place, text_2 - 0x30u);
	TYPED_PASS(typed_pass)->text = text;
	TYPED_PASS(typed_pass)->place = tile_place + 1u;
	TYPED_PASS(typed_pass)->wait = TYPED_PASS(typed_pass)->delay;
}

/* one of several lines typed at once: its place, the frames between characters and its text */
uint32_t start_typed_line(uint32_t typed_pass, uint32_t text)
{
	TYPED_PASS(typed_pass)->place = TEXT_32(text);
	text += 4u;
	TYPED_PASS(typed_pass)->delay = TEXT(text);
	text += 2u;
	TYPED_PASS(typed_pass)->text = text;
	return text;
}

/* the screen covered, and a box to open it from the middle */
void start_box_opening(void)
{
	uint16_t count = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0;

	box_frames_left = 0x14u;
	box_width = 1u;
	box_height = 1u;
	box_tile = 0x10u;
	box_place = box_opening_place;
	tile_place = TILE_RAM;
	count = 0xFFFu;
	do
	{
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, 0x3Au);
		t1 = count - 1u;
		count = t1;
		tile_place++;
	} while (t1 != 0xFFFFu);
}

/* the box drawn bigger every frame */
void grow_tile_box(void)
{
	uint8_t tile_code = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t corner_place = 0;

	if (box_frames_left != 0u)
	{
		corner_place = box_place;
		tile_code = box_tile;
		count = box_width;
		count_2 = box_height;
		draw_tile_box(count, count_2, tile_code, corner_place);
		box_frames_left--;
		box_width += 2u;
		box_place -= 2u;
		if (box_height < 0x1Du)
		{
			box_height += 2u;
			box_place -= 0x80u;
		}
	}
}

/* Unused: a box's opening set up: 16 frames, its size, its tile and its place */
void unreached_0186E2(void)
{
	box_frames_left = 0x10u;
	box_width = 0x1Fu;
	box_height = 0x1Fu;
	box_tile = 0u;
	box_place = unreached_box_opening_place;
}

/* the outline of a box of tiles */
void draw_tile_box(uint16_t count, uint16_t count_2, uint8_t tile_code, uint32_t corner_place)
{
	uint16_t count_3 = 0, right_offset = 0;
	uint32_t top_place = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	count_3 = (uint16_t)count;
	top_place = corner_place;
	do
	{
		io_write8(top_place, 0u);
		top_place++;
		io_write8(top_place, tile_code);
		t1 = count_3 - 1u;
		count_3 = t1;
		top_place++;
	} while (t1 != 0xFFFFu);
	right_offset = (uint16_t)((uint32_t)count << 1);
	count_2 = (uint8_t)(count_2 - 1u);
	corner_place += 0x80u;
	for (;;)
	{
		t2 = count_2 - 1u;
		count_2 = t2;
		if (t2 == 0xFFFFu)
			break;
		io_write8(corner_place, 0u);
		io_write8(corner_place + 1u, tile_code);
		io_write8(corner_place + right_offset, 0u);
		io_write8(corner_place + 1u + right_offset, tile_code);
		corner_place += 0x80u;
	}
	do
	{
		io_write8(corner_place, 0u);
		corner_place++;
		io_write8(corner_place, tile_code);
		t3 = count - 1u;
		count = t3;
		corner_place++;
	} while (t3 != 0xFFFFu);
}
