/**
 *    Name:        video/texts.c
 *    Description: the texts: from the program's tables of texts into the fixed layer, big characters, numbers
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a job of the main loop: a text of the demo's table into the fixed layer */
void print_demo_text(uint16_t text_number)
{
	uint32_t texts = 0;

	texts = demo_texts_ADDRESS;
	print_table_text(text_number, texts);
}

/* a job of the main loop: a text of the test mode's table into the fixed layer */
void print_test_text(uint16_t text_number)
{
	uint32_t texts = 0;

	texts = test_mode_texts_ADDRESS;
	print_table_text(text_number, texts);
}

/* a job of the main loop: a text of the game's table into the fixed layer */
void print_game_text(uint16_t text_number)
{
	uint32_t texts = 0;

	texts = game_texts_ADDRESS;
	print_table_text(text_number, texts);
}

/* a text of a table of texts into the fixed layer, or erased */
void print_table_text(uint16_t text_number, uint32_t texts)
{
	uint8_t character = 0, colour = 0;
	uint32_t text = 0, tile_place = 0;

	text_number = (uint16_t)((uint32_t)text_number << 2);
	if ((text_number & 0x200u) != 0u)
	{
		text = TEXT_POINTER(texts + (text_number & 0x1FFu));
		colour = text_colour;
		for (;;)
		{
			tile_place = TEXT_32(text);
			text += 4u;
			for (;;)
			{
				character = TEXT(text);
				text++;
				if (character == 0x3Fu)
					return;
				if (character == 0x2Fu)
					break;
				io_write8(tile_place, colour);
				tile_place++;
				io_write8(tile_place, 0x10u);
				tile_place++;
			}
		}
	}
	text = TEXT_POINTER(texts + text_number);
	colour = text_colour;
	for (;;)
	{
		tile_place = TEXT_32(text);
		text += 4u;
		for (;;)
		{
			character = TEXT(text);
			text++;
			if (character == 0x3Fu)
				return;
			if (character == 0x2Fu)
				break;
			io_write8(tile_place, colour);
			tile_place++;
			io_write8(tile_place, character - 0x30u);
			tile_place++;
		}
	}
}

/* the colour of the texts printed next */
void set_text_colour(uint16_t colour_number)
{
	text_colour = text_colours[colour_number];
}

/* a second texts' colour no text uses */
void set_spare_text_colour(uint16_t colour_number)
{
	spare_text_colour = text_colours[colour_number];
}

/* an enemy defeated: a point to the players' scores, the top score kept */
uint32_t add_point(uint32_t kept_bits, uint16_t players)
{
	uint32_t score_end = 0, scored_players = 0;

	if (game_on == 0u)
		return kept_bits;
	scored_players = players | (offset_high_word & ~0xFFFFu);
	if ((scored_players & 1u) != 0u)
	{
		score_end = player_1_health_bars_ADDRESS;
		add_point_to_score(score_end);
	}
	if ((scored_players & 2u) != 0u)
	{
		score_end = player_2_health_bars_ADDRESS;
		add_point_to_score(score_end);
	}
	if ((scored_players & 4u) != 0u)
	{
		score_end = player_3_health_bars_ADDRESS;
		add_point_to_score(score_end);
	}
	if ((scored_players & 8u) != 0u)
	{
		score_end = player_4_health_bars_ADDRESS;
		add_point_to_score(score_end);
	}
	draw_scores((uint8_t)scored_players);
	return scored_players;
}

/* a point to a player's score and the top score kept */
void add_point_to_score(uint32_t score_end)
{
	uint32_t carry = 0, extend = 0;
	uint16_t count = 0, score = 0;
	uint32_t digits = 0, top_score_place = 0;
	uint8_t bcd_digits = 0, digits_2 = 0, t1 = 0;
	uint16_t t2 = 0;

	digits = draw_score_digits_ADDRESS;
	count = 1u;
	extend = 0u;
	do
	{
		digits--;
		bcd_digits = BCD_DIGITS(digits);
		score_end--;
		digits_2 = BCD_PAIR(score_end)->digits;
		t1 = m68k_abcd(digits_2, bcd_digits, extend);
		carry = m68k_abcd_cy(digits_2, bcd_digits, extend);
		extend = carry;
		BCD_PAIR(score_end)->digits = t1;
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	score = AT16(BCD_PAIR(score_end)->digits, -1);
	top_score_place = top_score_ADDRESS;
	if (top_score < score)
		top_score = score;
}

/* the chosen players' scores drawn in their boxes */
void draw_scores(uint8_t players)
{
	if ((players & 1u) != 0u)
		draw_score_player_1();
	if ((players & 2u) != 0u)
		draw_score_player_2();
	if ((players & 4u) != 0u)
		draw_score_player_3();
	if ((players & 8u) != 0u)
		draw_score_player_4();
}

/* the first player's score in his box */
void draw_score_player_1(void)
{
	uint32_t score = 0, tile_place = 0;

	score = player_1_score_ADDRESS;
	tile_place = TILE_RAM + 0x1A4u;
	draw_score_digits(tile_place, score);
}

/* the second player's score in his box */
void draw_score_player_2(void)
{
	uint32_t score = 0, tile_place = 0;

	score = player_2_score_ADDRESS;
	tile_place = TILE_RAM + 0x1B6u;
	draw_score_digits(tile_place, score);
}

/* the third player's score in his box */
void draw_score_player_3(void)
{
	uint32_t score = 0, tile_place = 0;

	score = player_3_score_ADDRESS;
	tile_place = TILE_RAM + 0x1C8u;
	draw_score_digits(tile_place, score);
}

/* the fourth player's score in his box */
void draw_score_player_4(void)
{
	uint32_t score = 0, tile_place = 0;

	score = player_4_score_ADDRESS;
	tile_place = TILE_RAM + 0x1DAu;
	draw_score_digits(tile_place, score);
}

/* the top score after HIGH */
void draw_top_score(void)
{
	uint32_t score = 0, tile_place = 0;

	score = top_score_ADDRESS;
	tile_place = TILE_RAM + 0x1C2u;
	draw_score_digits(tile_place, score);
}

/* a score's four digits, without leading zeros */
void draw_score_digits(uint32_t tile_place, uint32_t score)
{
	uint8_t digit = 0;

	if (BCD_PAIR(score)->digits >> 4 != 0u)
	{
		digit = BCD_PAIR(score)->digits >> 4;
		tile_place = draw_digit(digit, tile_place);
		digit = BCD_PAIR(score)->digits & 0xFu;
		tile_place = draw_digit(digit, tile_place);
		digit = BCD_PAIR(score + 0x1u)->digits >> 4;
		tile_place = draw_digit(digit, tile_place);
		digit = BCD_PAIR(score + 0x1u)->digits & 0xFu;
		draw_digit(digit, tile_place);
		return;
	}
	tile_place = draw_blank_digit(tile_place);
	if ((BCD_PAIR(score)->digits & 0xFu) != 0u)
	{
		digit = BCD_PAIR(score)->digits & 0xFu;
		tile_place = draw_digit(digit, tile_place);
		digit = BCD_PAIR(score + 0x1u)->digits >> 4;
		tile_place = draw_digit(digit, tile_place);
		digit = BCD_PAIR(score + 0x1u)->digits & 0xFu;
		draw_digit(digit, tile_place);
		return;
	}
	tile_place = draw_blank_digit(tile_place);
	if (BCD_PAIR(score + 0x1u)->digits >> 4 != 0u)
	{
		digit = BCD_PAIR(score + 0x1u)->digits >> 4;
		tile_place = draw_digit(digit, tile_place);
		digit = BCD_PAIR(score + 0x1u)->digits & 0xFu;
		draw_digit(digit, tile_place);
		return;
	}
	tile_place = draw_blank_digit(tile_place);
	digit = BCD_PAIR(score + 0x1u)->digits & 0xFu;
	draw_digit(digit, tile_place);
}

/* a blank in place of a leading zero */
uint32_t draw_blank_digit(uint32_t tile_place)
{
	uint8_t digit = 0;

	digit = 0x10u;
	return draw_digit(digit, tile_place);
}

/* a digit of a score */
uint32_t draw_digit(uint8_t digit, uint32_t tile_place)
{
	io_write8(tile_place, 0u);
	tile_place++;
	io_write8(tile_place, digit);
	tile_place++;
	return tile_place;
}

/* the chosen scores to 0 */
void clear_scores(uint8_t players)
{
	if ((players & 1u) != 0u)
		PLAYER(PLAYER_ADDRESS(0))->score = 0u;
	if ((players & 2u) != 0u)
		PLAYER(PLAYER_ADDRESS(1))->score = 0u;
	if ((players & 4u) != 0u)
		PLAYER(PLAYER_ADDRESS(2))->score = 0u;
	if ((players & 8u) != 0u)
		PLAYER(PLAYER_ADDRESS(3))->score = 0u;
	if ((players & 0x10u) != 0u)
		top_score = 0u;
}

/* a short text in the plain colour */
void print_plain_text(uint32_t tile_place, uint32_t text)
{
	uint8_t character = 0;

	for (;;)
	{
		character = TEXT(text);
		text++;
		if (character == 0x3Fu)
			break;
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, character - 0x30u);
		tile_place++;
	}
}

/* unused: the labels 1UP to 4UP and the top score */
void draw_score_labels(uint8_t which_labels)
{
	uint32_t text = 0, tile_place = 0;

	if ((which_labels & 1u) == 0u)
	{
		text = text_1up_ADDRESS;
		tile_place = TILE_RAM + 0x11Cu;
		print_plain_text(tile_place, text);
		text = text_2up_ADDRESS;
		tile_place = TILE_RAM + 0x12Eu;
		print_plain_text(tile_place, text);
		text = text_3up_ADDRESS;
		tile_place = TILE_RAM + 0x140u;
		print_plain_text(tile_place, text);
		text = text_4up_ADDRESS;
		tile_place = TILE_RAM + 0x152u;
		print_plain_text(tile_place, text);
		return;
	}
	if ((which_labels & 2u) != 0u)
	{
		text = text_high_ADDRESS;
		tile_place = TILE_RAM + 0x138u;
		print_plain_text(tile_place, text);
		draw_top_score();
	}
	if ((which_labels & 1u) == 0u)
	{
		text = text_1up_ADDRESS;
		tile_place = TILE_RAM + 0x11Cu;
		print_plain_text(tile_place, text);
		text = text_2up_ADDRESS;
		tile_place = TILE_RAM + 0x12Eu;
		print_plain_text(tile_place, text);
		return;
	}
	text = text_1up_ADDRESS;
	tile_place = TILE_RAM + 0x11Cu;
	print_plain_text(tile_place, text);
	text = text_2up_ADDRESS;
	tile_place = TILE_RAM + 0x12Eu;
	print_plain_text(tile_place, text);
	text = text_3up_ADDRESS;
	tile_place = TILE_RAM + 0x140u;
	print_plain_text(tile_place, text);
	text = text_4up_ADDRESS;
	tile_place = TILE_RAM + 0x152u;
	print_plain_text(tile_place, text);
}

/* Unused: a 16-bit number into six decimal digits, the BCD powers of two of its set bits added up in a
   stack frame the C has not (lost_stack) */
void word_to_decimal_digits(void)
{
	lost_stack(0x00218Au);
}

/* a text of the test mode's and the credits' table, in its own colour */
void print_colour_text(uint16_t text_number)
{
	uint8_t text = 0, text_2 = 0;
	uint32_t text_record = 0, tile_place = 0;

	text_number = (uint16_t)((uint32_t)text_number << 2);
	if ((text_number & 0x200u) != 0u)
	{
		text_record = TABLE32(colour_texts, text_number & 0x1FFu);
		for (;;)
		{
			tile_place = TEXT_32(text_record);
			text_record += 4u;
			text_2 = TEXT(text_record);
			text_record++;
			for (;;)
			{
				text = TEXT(text_record);
				text_record++;
				if (text == 0x3Fu)
					return;
				if (text == 0x2Fu)
					break;
				io_write8(tile_place, text_2);
				tile_place++;
				io_write8(tile_place, 0x10u);
				tile_place++;
			}
		}
	}
	text_record = TABLE32(colour_texts, (uint32_t)(int16_t)text_number);
	for (;;)
	{
		tile_place = TEXT_32(text_record);
		text_record += 4u;
		text_2 = TEXT(text_record);
		text_record++;
		for (;;)
		{
			text = TEXT(text_record);
			text_record++;
			if (text == 0x3Fu)
				return;
			if (text == 0x2Fu)
				break;
			io_write8(tile_place, text_2);
			tile_place++;
			io_write8(tile_place, text - 0x30u);
			tile_place++;
		}
	}
}

/* the main loop's job 8, which no code queues: does nothing */
void job_none_after_print_colour_text(void)
{
}

/* unused: a picture of rows of tiles */
uint32_t draw_tile_picture(uint16_t picture_number)
{
	uint8_t tile_picture = 0, tile_picture_2 = 0;
	uint16_t count = 0;
	uint32_t codes = 0, count_2 = 0, picture = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	picture = tile_pictures[picture_number & 0x3FFFu];
	count = TILE_PICTURE_16(picture);
	picture += 2u;
	codes = picture + (int16_t)(count * 6u);
	count = (uint16_t)((count & ~0xFFu) | (uint8_t)(count - 1u));
	do
	{
		tile_place = TILE_PICTURE_32(picture);
		picture += 4u;
		count_2 = TILE_PICTURE(picture);
		count_2 = (uint8_t)(count_2 - 1u);
		picture++;
		tile_picture = TILE_PICTURE(picture);
		picture++;
		do
		{
			tile_picture_2 = TILE_PICTURE(codes);
			io_write8(tile_place, tile_picture);
			tile_place++;
			io_write8(tile_place, tile_picture_2);
			t1 = count_2 - 1u;
			count_2 = t1;
			codes++;
			tile_place++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	return count_2;
}

/* the main loop's job 0xA, which no code queues: does nothing */
void job_none_after_draw_tile_picture(void)
{
}

/* the chosen players' boxes, and their scores */
void draw_player_boxes(uint16_t box_flags, uint32_t *p_players, uint32_t *p_box_offset, uint32_t *p_player)
{
	uint32_t players = *p_players;
	uint32_t box_offset = *p_box_offset;
	uint32_t player = 0;

	player = 0u;
	fade_banks = 0u;
	do
	{
		if (((box_flags | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(player & 0x1Fu))) != 0u)
		{
			player = (player & ~0xFF00u) | (fade_banks & 0xFF00u);
			draw_player_box((uint16_t)player, (uint8_t)box_flags, &players, &box_offset);
		}
		player = (player & ~0xFFu) | (uint8_t)(player + 1u);
	} while ((uint8_t)player < 4u);
	if ((box_flags & 0x80u) == 0u && (box_flags & 0x40u) != 0u)
	{
		players = (uint8_t)box_flags;
		players = players & 0x1Fu;
		draw_scores((uint8_t)players);
	}
	*p_players = players;
	*p_box_offset = (*p_box_offset & 0xFFFF0000u) | (box_offset & 0xFFFFu);
	*p_player = player;
}

/* a player's box: its frame and label, its lives, its health bar */
void draw_player_box(uint16_t player, uint8_t box_flags, uint32_t *p_drawn, uint32_t *p_box_offset)
{
	uint32_t drawn = *p_drawn;
	uint32_t box_offset = 0, box_places = 0, part_place = 0;

	box_places = player_box_places_ADDRESS;
	box_offset = (uint16_t)((uint32_t)player << 4);
	if ((box_flags & 0x40u) != 0u)
	{
		part_place = TABLE32(player_box_places, (uint32_t)(int16_t)box_offset);
		distance_low_byte = box_offset;
		draw_box_frame_and_label(player, box_flags, part_place, box_places, &drawn, &box_offset);
	}
	if ((box_flags & 0x10u) != 0u)
	{
		part_place = TABLE32(player_box_places, 0x8u + (uint32_t)(int16_t)box_offset);
		drawn = draw_lives(drawn, player, box_flags, part_place);
	}
	if ((box_flags & 0x20u) != 0u)
	{
		part_place = TABLE32(player_box_places, 0xCu + (uint32_t)(int16_t)box_offset);
		drawn = draw_health_bar(player, box_flags, part_place);
	}
	distance_low_byte = box_offset;
	*p_drawn = drawn;
	*p_box_offset = (*p_box_offset & 0xFFFF0000u) | (box_offset & 0xFFFFu);
}

/* a player's box frame and label in his colour */
void draw_box_frame_and_label(uint16_t player, uint8_t box_flags, uint32_t part_place, uint32_t box_places, uint32_t *p_label_offset, uint32_t *p_box_offset)
{
	uint32_t label_offset = *p_label_offset;
	uint16_t box_offset = (uint16_t)*p_box_offset;
	uint32_t colour = 0;

	colour = player_box_colour(0u, player);
	draw_box_frame((uint8_t)colour, box_flags, part_place);
	box_offset = (uint16_t)((box_offset & ~0xFFu) | (distance_low_byte & 0xFFu));
	label_offset = draw_box_label((uint8_t)colour, label_offset, box_offset, player, box_flags, box_places);
	*p_label_offset = (*p_label_offset & 0xFFFF0000u) | (label_offset & 0xFFFFu);
	*p_box_offset = (*p_box_offset & 0xFFFFFF00u) | (box_offset & 0xFFu);
}

/* a player's lives in his box */
uint32_t draw_lives(uint32_t drawn, uint16_t player, uint8_t box_flags, uint32_t part_place)
{
	uint16_t hundreds_offset = 0, tens_offset = 0, units_offset = 0;
	uint32_t colour = 0, tiles = 0;

	colour = player_box_colour(0u, player);
	clear_lives_digits((uint8_t)colour, part_place);
	if ((box_flags & 0x80u) != 0u)
		return drawn;
	drawn = LINKED(drawn, player);
	drawn = LINKED(drawn, (uint16_t)(drawn << 3));
	drawn = LINKED(drawn, PLAYER(PLAYER_ADDRESS(0) + (int16_t)drawn)->lives);
	units_offset = (uint8_t)drawn;
	units_offset = (uint16_t)((units_offset & 0xFu) << 1);
	io_write8(part_place + 4u, colour);
	io_write8(part_place + 0x84u, colour);
	io_write8(part_place + 5u, lives_digit_top[units_offset]);
	io_write8(part_place + 0x85u, lives_digit_bottom[units_offset]);
	tens_offset = (uint8_t)drawn;
	tens_offset = (tens_offset & ~0xFu) >> 3;
	io_write8(part_place + 2u, colour);
	io_write8(part_place + 0x82u, colour);
	tiles = lives_icon_top_ADDRESS;
	io_write8(part_place + 3u, lives_icon_top[tens_offset]);
	io_write8(part_place + 0x83u, lives_icon_bottom[tens_offset]);
	if (tens_offset == 0u && (drawn & 0xF00u) != 0u)
	{
		io_write8(part_place + 3u, 0x10u);
		io_write8(part_place + 0x83u, 0x20u);
	}
	hundreds_offset = (drawn & 0xF00u) >> 7;
	io_write8(part_place, colour);
	io_write8(part_place + 0x80u, colour);
	tiles += 0x14u;
	io_write8(part_place + 1u, PICTURE_TILE(tiles + hundreds_offset));
	io_write8(part_place + 0x81u, PICTURE_TILE(tiles + ((int32_t)(hundreds_offset + 1u))));
	return drawn;
}

/* a player's health bar in his box */
uint32_t draw_health_bar(uint16_t player, uint8_t box_flags, uint32_t part_place)
{
	uint32_t colour = 0, count = 0, tiles = 0;
	uint16_t t1 = 0;

	colour = player_box_colour(0u, player);
	count = clear_health_bar((uint8_t)colour, part_place);
	if ((box_flags & 0x80u) != 0u)
		return count;
	tiles = health_bar_tiles_ADDRESS + PLAYER(PLAYER_ADDRESS(0) + (int16_t)((uint32_t)player << 3))->health_bars * 8u;
	count = 3u;
	do
	{
		io_write8(part_place, colour);
		io_write8(part_place + 0x80u, colour);
		io_write8(part_place + 0x81u, PICTURE_TILE(tiles + 0x4u));
		io_write8(part_place + 1u, PICTURE_TILE(tiles));
		t1 = count - 1u;
		count = t1;
		tiles++;
		part_place += 2u;
	} while (t1 != 0xFFFFu);
	return count;
}

/* a player's colour in the boxes */
uint32_t player_box_colour(uint32_t colour, uint16_t player)
{
	colour = (colour & ~0xFFu) | TABLE8(text_colours, 0x1u + (uint32_t)(int16_t)player);
	colour = colour | 1u;
	return colour;
}

/* the lives' place in a box blank */
void clear_lives_digits(uint8_t colour, uint32_t part_place)
{
	io_write8(part_place, colour);
	io_write8(part_place + 2u, colour);
	io_write8(part_place + 4u, colour);
	io_write8(part_place + 0x80u, colour);
	io_write8(part_place + 0x82u, colour);
	io_write8(part_place + 0x84u, colour);
	io_write8(part_place + 1u, 0x5Eu);
	io_write8(part_place + 3u, 0x5Eu);
	io_write8(part_place + 5u, 0x5Eu);
	io_write8(part_place + 0x81u, 0x6Eu);
	io_write8(part_place + 0x83u, 0x6Eu);
	io_write8(part_place + 0x85u, 0x6Eu);
	io_write8(part_place - 2u, colour);
	io_write8(part_place + 0x7Eu, colour);
	io_write8(part_place - 1u, 0x1Au);
	io_write8(part_place + 0x7Fu, 0x2Au);
}

/* the health bar's place in a box blank */
uint32_t clear_health_bar(uint8_t colour, uint32_t part_place)
{
	uint32_t tile_offset = 0;

	tile_offset = 0u;
	do
	{
		io_write8(part_place + (int16_t)tile_offset, colour);
		io_write8(part_place + 1u + (int16_t)tile_offset, 0x5Eu);
		tile_offset = (uint16_t)(tile_offset + 0x80u);
		io_write8(part_place + (int16_t)tile_offset, colour);
		io_write8(part_place + 1u + (int16_t)tile_offset, 0x6Eu);
		tile_offset = (uint16_t)(tile_offset - 0x80u);
		tile_offset = (tile_offset & ~0xFFu) | (uint8_t)(tile_offset + 2u);
	} while ((uint8_t)tile_offset < 8u);
	io_write8(part_place + (int16_t)tile_offset, colour);
	io_write8(part_place + 1u + (int16_t)tile_offset, 0x1Du);
	tile_offset = (uint16_t)(tile_offset + 0x80u);
	io_write8(part_place + (int16_t)tile_offset, colour);
	io_write8(part_place + 1u + (int16_t)tile_offset, 0x2Du);
	return tile_offset;
}

/* a player's box frame */
void draw_box_frame(uint8_t colour, uint8_t box_flags, uint32_t part_place)
{
	uint8_t tile_code = 0;
	uint32_t tile_offset = 0, tiles = 0;

	tiles = box_frame_tiles_ADDRESS;
	tile_offset = 0u;
	for (;;)
	{
		tile_code = PICTURE_TILE(tiles);
		tiles++;
		if (tile_code == 0x3Fu)
			break;
		if (tile_code != 0x2Fu)
		{
			if ((box_flags & 0x80u) != 0u)
				tile_code = 0x5Eu;
			io_write8(part_place + (int16_t)tile_offset, colour);
			io_write8(part_place + 1u + (int16_t)tile_offset, tile_code);
			tile_offset = (uint16_t)(tile_offset + 2u);
			continue;
		}
		tile_offset = 0u;
		part_place += 0x80u;
	}
	screen_x_high = tile_offset;
}

/* a player's box label 1UP to 4UP */
uint32_t draw_box_label(uint8_t colour, uint32_t label_offset, uint16_t box_offset, uint16_t player, uint8_t box_flags, uint32_t box_places)
{
	uint32_t label = 0, label_place = 0;

	if ((box_flags & 0x80u) == 0u)
	{
		label_place = BOX_PART_PLACE(box_places + (int16_t)box_offset + 0x4u);
		label_offset = LINKED(label_offset, player);
		label_offset = LINKED(label_offset, (uint16_t)(label_offset << 2));
		label = TABLE32(box_labels, (uint32_t)(int16_t)label_offset);
		io_write8(label_place, colour);
		io_write8(label_place + 2u, colour);
		io_write8(label_place + 0x80u, colour);
		io_write8(label_place + 0x82u, colour);
		io_write8(label_place + 1u, TEXT(label));
		io_write8(label_place + 3u, TEXT(label + 0x1u));
		io_write8(label_place + 0x81u, TEXT(label + 0x2u));
		io_write8(label_place + 0x83u, TEXT(label + 0x3u));
	}
	return label_offset;
}

/* Unused: a box's frame drawn into the tile RAM from its tiles, a new row at each '/' */
void draw_box_frame_by_rows(void)
{
	uint8_t colour = 0, picture_tile = 0;
	uint16_t tile_offset = 0;
	uint32_t part_place = 0, tiles = 0;

	tiles = unreached_box_frame_tiles_ADDRESS;
	part_place = TILE_RAM + 0x138u;
	colour = 0x61u;
	tile_offset = 0u;
	for (;;)
	{
		picture_tile = PICTURE_TILE(tiles);
		tiles++;
		if (picture_tile == 0x3Fu)
			break;
		if (picture_tile != 0x2Fu)
		{
			io_write8(part_place + (int16_t)tile_offset, colour);
			io_write8(part_place + 1u + (int16_t)tile_offset, picture_tile);
			tile_offset += 2u;
			continue;
		}
		tile_offset = 0u;
		part_place += 0x80u;
	}
}

/* unused: the stage's number and its name of the development */
void print_stage_title(void)
{
	uint8_t colour = 0, text = 0;
	uint32_t text_record = 0, tile_place = 0;

	text_record = stage_titles[stage];
	colour = text_colour;
	for (;;)
	{
		tile_place = TEXT_32(text_record);
		text_record += 4u;
		for (;;)
		{
			text = TEXT(text_record);
			text_record++;
			if (text == 0x3Fu)
				return;
			if (text == 0x2Fu)
				break;
			io_write8(tile_place, colour);
			tile_place++;
			io_write8(tile_place, text - 0x30u);
			tile_place++;
		}
	}
}

/* the message under a player's box */
void draw_player_message(uint8_t player_message, uint32_t *p_colour, uint32_t *p_message_offset)
{
	uint32_t colour = *p_colour;
	uint16_t player_number = 0;
	uint32_t message_offset = 0, player = 0;

	player_number = player_message & 3u;
	text_colour = text_colours[1 + player_number];
	text_colour |= 1u;
	player = PLAYER_ADDRESS(0) + (uint16_t)((uint32_t)player_number << 3);
	fade_banks = 0u;
	message_offset = (uint8_t)player_message;
	message_offset = message_offset & 0x3Cu;
	switch (message_offset)
	{
	case 0:
		message_insert_coin(player_message, player, &colour, &message_offset);
		break;
	case 4:
		message_press_start(player_message, player, &colour, &message_offset);
		break;
	case 8:
		message_out_of_lives(player_message, player, &colour, &message_offset);
		break;
	case 12:
		message_clear(player_message, player, &colour, &message_offset);
		break;
	}
	*p_colour = (*p_colour & 0xFFFF0000u) | (colour & 0xFFFFu);
	*p_message_offset = message_offset;
}

/* INSERT COIN(S) in a player's box */
void message_insert_coin(uint8_t player_message, uint32_t player, uint32_t *p_colour, uint32_t *p_text_number)
{
	uint32_t colour = 0;

	if ((PLAYER(player)->message & 1u) == 0u)
		PLAYER(player)->message = 1u;
	*p_text_number = 0u;
	colour = print_text_at(colour, (uint16_t)*p_text_number, player_message);
	*p_colour = (*p_colour & 0xFFFF0000u) | (colour & 0xFFFFu);
}

/* PRESS START blinking under a player's box */
void message_press_start(uint8_t player_message, uint32_t player, uint32_t *p_colour, uint32_t *p_text_number)
{
	uint32_t colour = *p_colour;
	uint32_t blink_phase = 0;
	uint8_t t1 = 0;

	if ((PLAYER(player)->message & 2u) != 0u)
	{
		blink_phase = (uint8_t)frame_count;
		t1 = blink_phase & 0xFu;
		blink_phase = t1;
		if (t1 == 0u)
		{
			blink_phase = 2u;
			if (hide_press_start != 0u)
				blink_phase = 8u;
			if ((frame_count & 0x10u) != 0u)
				blink_phase = (uint8_t)(blink_phase + 1u);
			colour = print_text_at(colour, (uint16_t)blink_phase, player_message);
		}
	}
	else
	{
		PLAYER(player)->message = 2u;
		blink_phase = 2u;
		if (hide_press_start != 0u)
			blink_phase = 8u;
		if ((frame_count & 0x10u) != 0u)
			blink_phase = (uint8_t)(blink_phase + 1u);
		colour = print_text_at(colour, (uint16_t)blink_phase, player_message);
	}
	*p_colour = (*p_colour & 0xFFFF0000u) | (colour & 0xFFFFu);
	*p_text_number = blink_phase;
}

/* the knocked-out head of a player who lost his last life */
void message_out_of_lives(uint8_t player_message, uint32_t player, uint32_t *p_colour, uint32_t *p_text_number)
{
	uint32_t colour = *p_colour;
	uint32_t blink_phase = 0;
	uint8_t t1 = 0, t2 = 0;

	if ((PLAYER(player)->message & 4u) != 0u)
	{
		blink_phase = (uint8_t)frame_count;
		t1 = blink_phase & 7u;
		blink_phase = t1;
		if (t1 == 0u)
		{
			blink_phase = (uint8_t)(4u + (((uint8_t)frame_count & 0x18u) >> 3));
			colour = print_text_at(colour, (uint16_t)blink_phase, player_message);
			t2 = (uint8_t)frame_count & 0x18u;
			distance_low_byte = t2;
			if (t2 == 0u)
			{
				PLAYER(player)->message += 0x20u;
				if ((int8_t)PLAYER(player)->message < 0)
				{
					PLAYER(player)->message = 0u;
					players_out_of_lives &= ~shl32(1u, (int)(player_message & 3u));
				}
			}
		}
	}
	else
	{
		PLAYER(player)->message = 4u;
		blink_phase = (uint8_t)(4u + (((uint8_t)frame_count & 0x18u) >> 3));
		colour = print_text_at(colour, (uint16_t)blink_phase, player_message);
		t2 = (uint8_t)frame_count & 0x18u;
		distance_low_byte = t2;
		if (t2 == 0u)
		{
			PLAYER(player)->message += 0x20u;
			if ((int8_t)PLAYER(player)->message < 0)
			{
				PLAYER(player)->message = 0u;
				players_out_of_lives &= ~shl32(1u, (int)(player_message & 3u));
			}
		}
	}
	*p_colour = (*p_colour & 0xFFFF0000u) | (colour & 0xFFFFu);
	*p_text_number = blink_phase;
}

/* the line under a player's box cleared */
void message_clear(uint8_t player_message, uint32_t player, uint32_t *p_colour, uint32_t *p_text_number)
{
	uint32_t colour = *p_colour;

	if ((PLAYER(player)->message & 8u) != 0u)
		return;
	PLAYER(player)->message = 2u;
	*p_text_number = 1u;
	colour = print_text_at(colour, (uint16_t)*p_text_number, player_message);
	*p_colour = (*p_colour & 0xFFFF0000u) | (colour & 0xFFFFu);
}

/* a positioned text into the layer and place picked, in the text colour; '/' a new place, '?' the end */
uint32_t print_text_at(uint32_t colour_bits, uint16_t text_number, uint8_t player_message)
{
	uint8_t character = 0;
	uint16_t text_place = 0;
	uint32_t layer_place = 0, text = 0;

	text = positioned_texts[text_number & 0x3FFFu];
	colour_bits = IN_PAGE(colour_bits, (uint8_t)player_message);
	colour_bits = LINKED(colour_bits, colour_bits & 3u);
	colour_bits = LINKED(colour_bits, (uint16_t)(colour_bits << 2));
	layer_place = text_layer_places[(uint16_t)colour_bits / 4u];
	colour_bits = IN_PAGE(colour_bits, text_colour);
	for (;;)
	{
		text_place = TEXT_16(text);
		text += 2u;
		for (;;)
		{
			character = TEXT(text);
			text++;
			if (character == 0x3Fu)
			{
				distance_low_byte = character;
				fade_banks = text_place & 0xFF00u;
				return colour_bits;
			}
			if (character == 0x2Fu)
				break;
			io_write8(layer_place + (int16_t)text_place, colour_bits);
			io_write8(layer_place + 1u + (int16_t)text_place, character);
			text_place += 2u;
		}
	}
	unreachable();
}

/* a big character of the continue's countdown drawn in tiles, nine rows of eight, in its colour */
uint32_t draw_big_character(uint16_t pattern_bits, uint8_t character)
{
	uint16_t count = 0, count_2 = 0;
	uint32_t colour_number = 0, pattern = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = big_font_place;
	pattern = big_font_ADDRESS + (uint16_t)((character & 0x1Fu) * 0x24u);
	count = 8u;
	do
	{
		count_2 = 3u;
		colour_number = 0u;
		colour_number = big_character_colour(colour_number);
		do
		{
			pattern_bits = (IN_PAGE(pattern_bits, PICTURE_TILE(pattern)) >> 4) & 0xFu;
			io_write8(tile_place, colour_number);
			tile_place++;
			io_write8(tile_place, big_font_tiles[pattern_bits]);
			pattern_bits = PICTURE_TILE(pattern) & 0xFu;
			tile_place++;
			io_write8(tile_place, colour_number);
			tile_place++;
			io_write8(tile_place, big_font_tiles[pattern_bits]);
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place++;
			pattern++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x70u;
	} while (t2 != 0xFFFFu);
	return colour_number;
}

/* the colour of a big character */
uint32_t big_character_colour(uint32_t colour_number)
{
	uint32_t colour = 0;

	colour = text_colours[colour_number];
	return colour;
}

/* a text in tall characters: the rankings, CONTINUE, GAME OVER */
void print_tall_text(uint8_t text_number, uint32_t *p_colour, uint32_t *p_character)
{
	uint32_t colour = *p_colour;
	uint32_t character = *p_character;
	uint32_t colour_number = 0, font_tiles = 0, text = 0, tile_place = 0;

	font_tiles = big_font_tiles_table_ADDRESS;
	if ((text_number & 0x80u) != 0u)
	{
		text = tall_texts[text_number & 0x3Fu];
		for (;;)
		{
			tile_place = TEXT_32(text);
			text += 5u;
			for (;;)
			{
				character = (character & ~0xFFu) | TEXT(text);
				text++;
				if ((uint8_t)character == 0x3Fu)
				{
					*p_character = character;
					return;
				}
				if ((uint8_t)character == 0x2Fu)
					break;
				tile_place = erase_tall_character(tile_place);
			}
		}
	}
	text = tall_texts[text_number & 0x3Fu];
	for (;;)
	{
		tile_place = TEXT_32(text);
		text += 4u;
		colour_number = TEXT(text);
		text++;
		colour_number = tall_text_colour(colour_number);
		for (;;)
		{
			fade_banks = 0u;
			character = TEXT(text);
			text++;
			if (character == 0x3Fu)
			{
				*p_colour = colour_number;
				*p_character = character;
				return;
			}
			if (character == 0x2Fu)
				break;
			character = (uint8_t)(character - 0x30u);
			tile_place = draw_tall_character((uint8_t)colour_number, (uint16_t)character, tile_place, font_tiles);
		}
	}
}

/* a tall character */
uint32_t draw_tall_character(uint8_t colour, uint16_t character, uint32_t tile_place, uint32_t font_tiles)
{
	if (character == 0xFEu)
	{
		tile_place += 2u;
		return tile_place;
	}
	character = (uint16_t)((uint32_t)character << 1);
	io_write8(tile_place + 0x80u, colour);
	io_write8(tile_place + 0x81u, FONT_TILE(font_tiles + ((int32_t)(character + 1u))));
	io_write8(tile_place, colour);
	tile_place++;
	io_write8(tile_place, FONT_TILE(font_tiles + character));
	tile_place++;
	return tile_place;
}

/* a tall character erased */
uint32_t erase_tall_character(uint32_t tile_place)
{
	io_write8(tile_place + 0x80u, 0u);
	io_write8(tile_place + 0x81u, 0x10u);
	io_write8(tile_place, 0u);
	tile_place++;
	io_write8(tile_place, 0x10u);
	tile_place++;
	return tile_place;
}

/* the colour of a text in tall characters */
uint32_t tall_text_colour(uint32_t colour_number)
{
	uint32_t colour = 0;

	colour = text_colours[colour_number];
	return colour;
}
