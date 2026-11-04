/**
 *    Name:        scenes/ranking.c
 *    Description: the ranking after a game: the players' places, their columns, the name entry, and the ranking's defaults
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the ranking after a game, by its steps */
void scene_ranking(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		ranking_setup();
		return;
	case 1:
		rank_players_scores();
		return;
	case 2:
		show_ranked_players();
		return;
	case 3:
		ranking_choose_entry();
		return;
	case 4:
		ranking_wait();
		return;
	case 5:
		enter_initials();
		return;
	case 6:
		ranking_entry_done();
		return;
	case 7:
		ranking_end_wait();
		return;
	}
}

/* the ranking's first step, by its steps */
void ranking_setup(void)
{
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		ranking_clear_layers();
		return;
	case 1:
		ranking_clear();
		return;
	case 2:
		ranking_reset();
		return;
	}
}

/* the layers cleared for the ranking */
void ranking_clear_layers(void)
{
	uint16_t job = 0;

	if (jobs_pending == 0u)
	{
		jobs_pending = 1u;
		job = 7u;
		queue_job(job);
		scene_substep++;
	}
}

/* the objects cleared and the ranking's palettes requested */
void ranking_clear(void)
{
	uint16_t kind = 0;
	uint32_t data = 0;

	if (jobs_pending == 0u)
	{
		clear_objects();
		clear_sprite_ram();
		data = ranking_palettes_ADDRESS;
		kind = 0x20Fu;
		post_palette_request(kind, data);
		clear_palette_effects();
		scene_substep++;
	}
}

/* the places, the tile banks and the scroll reset for the ranking */
void ranking_reset(void)
{
	if (jobs_pending != 0u || palettes_busy != 0u)
		return;
	ranking_places(0) = 0u;
	ranking_places(1) = 0u;
	ranking_places(2) = 0u;
	ranking_places(3) = 0u;
	tile_banks_0_1 = 0u;
	io_write8(TILES_106B00, 0u);
	tile_banks_2_3 = 0u;
	io_write8(TILES_106E00, 0u);
	layer_b_scroll_x = 0x60u;
	layer_b_scroll_y = 0u;
	name_entered = 0u;
	scene_step++;
	scene_substep = 0u;
}

/* each player's score put in the ranking, with his place */
void rank_players_scores(void)
{
	uint32_t carry = 0, extend = 0;
	uint8_t place = 0;
	uint16_t count = 0, count_2 = 0, initials_offset = 0, score = 0, score_offset = 0;
	uint32_t initials = 0, moved_initials = 0, places = 0, player = 0, scores = 0;
	uint8_t t4 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t5 = 0, t6 = 0;

	scores = ranking_scores_ADDRESS;
	initials = ranking_initials_ADDRESS;
	places = ranking_places_ADDRESS;
	player = PLAYER_ADDRESS(3);
	count = 3u;
	do
	{
		extend = 0u;
		ranking_new_initials = 0x41414100u;
		score_offset = 0xC4u;
		ranking_new_score = PLAYER(player)->score;
		do
		{
			score = RAM16(ranking_scores_ADDRESS + (uint32_t)(int16_t)score_offset);
			if (RAM16(ranking_scores_ADDRESS + 0x2u + (uint32_t)(int16_t)score_offset) < score)
				break;
			RAM16(ranking_scores_ADDRESS + (uint32_t)(int16_t)score_offset) = RAM16(ranking_scores_ADDRESS + 0x2u + (uint32_t)(int16_t)score_offset);
			RAM16(ranking_scores_ADDRESS + 0x2u + (uint32_t)(int16_t)score_offset) = score;
			place = m68k_sbcd(ranking_places((uint32_t)(int16_t)count), 1u, extend);
			ranking_places((uint32_t)(int16_t)count) = place;
			if (place < 0x11u)
			{
				initials_offset = (uint16_t)place;
				if (initials_offset == 0x10u)
					initials_offset = 0xAu;
				initials_offset = (uint16_t)((initials_offset - 1u) << 2);
				moved_initials = AT32(ranking_initials(4 + (uint32_t)(int16_t)initials_offset), -3);
				AT32(ranking_initials(4 + (uint32_t)(int16_t)initials_offset), -3) = RAM32(ranking_initials_ADDRESS + (uint32_t)(int16_t)initials_offset);
				RAM32(ranking_initials_ADDRESS + (uint32_t)(int16_t)initials_offset) = moved_initials;
			}
			t1 = score_offset;
			carry = t1 < 2u;
			extend = carry;
			score_offset = (uint16_t)(t1 - 2u);
		} while (t1 >= 2u);
		t2 = count - 1u;
		count = t2;
		player -= 8u;
	} while (t2 != 0xFFFFu);
	count = 3u;
	do
	{
		place = ranking_places((uint32_t)(int16_t)count);
		t3 = count;
		carry = t3 == 0u;
		extend = carry;
		count_2 = (uint16_t)(t3 - 1u);
		if (t3 != 0u)
		{
			do
			{
				if (place == 0u)
					break;
				if (ranking_places((uint32_t)(int16_t)count_2) != 0u && ranking_places((uint32_t)(int16_t)count_2) <= place)
				{
					t4 = m68k_abcd(place, 1u, extend);
					carry = m68k_abcd_cy(place, 1u, extend);
					extend = carry;
					place = t4;
				}
				t5 = count_2 - 1u;
				count_2 = t5;
			} while (t5 != 0xFFFFu);
		}
		ranking_places((uint32_t)(int16_t)count) = place;
		t6 = count - 1u;
		count = t6;
	} while (t6 != 0xFFFFu);
	scene_step++;
}

/* each ranked player's column: frame, label, score, place and his turtle */
void show_ranked_players(void)
{
	uint8_t score_players = 0;
	uint16_t count = 0, player_number = 0;
	uint32_t colour = 0, picture_players = 0, places = 0;
	uint16_t t1 = 0;

	count = 3u;
	places = ranking_places_ADDRESS;
	turtles_in_black = 0x4Fu;
	do
	{
		if (ranking_places((uint32_t)(int16_t)count) != 0u)
		{
			colour = draw_ranking_frame(colour, count);
			draw_ranking_label((uint8_t)colour, count);
			score_players = (uint8_t)shl32(1u, (int)(count & 0x1Fu));
			draw_scores(score_players);
			count = (uint16_t)count;
			draw_ranking_place(count, places);
			player_number = (uint16_t)count;
			picture_players = shl32(1u, (int)(player_number & 0x1Fu));
			turtles_in_black &= ~shl32(1u, (int)(player_number & 7u));
			draw_four_turtle_pictures(picture_players);
			count = (uint16_t)player_number;
		}
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	scene_step++;
}

/* back to the attract mode with no players, else the name entry for the top ten or a wait */
void ranking_choose_entry(void)
{
	uint8_t entering = 0, sound = 0, unranked = 0;
	uint16_t count = 0;
	uint32_t entered = 0, loop_end = 0, places = 0;
	uint16_t t1 = 0, t2 = 0;

	places = ranking_places_ADDRESS;
	unranked = 0u;
	count = 3u;
	for (;;)
	{
		if (ranking_places((uint32_t)(int16_t)count) == 0u)
		{
			unranked++;
			t1 = count - 1u;
			count = t1;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if (ranking_places((uint32_t)(int16_t)count) < 0x11u)
		{
			sound = 0x71u;
			queue_sound(sound);
			places = ranking_places_ADDRESS;
			entered = initials_entered_ADDRESS;
			entering = 0u;
			count = 3u;
			do
			{
				if (ranking_places((uint32_t)(int16_t)count) == 0u || ranking_places((uint32_t)(int16_t)count) >= 0x11u)
					initials_entered((uint32_t)(int16_t)count) = 3u;
				else
				{
					initials_entered((uint32_t)(int16_t)count) = 0u;
					entering |= shl32(1u, (int)(count & 0x1Fu));
				}
				t2 = count - 1u;
				count = t2;
			} while (t2 != 0xFFFFu);
			players_entering_names = entering;
			initials_buttons_held = 0u;
			loop_end = draw_ranking_background();
			loop_end = (loop_end & ~0xFFu) | turtles_in_black;
			draw_four_turtle_pictures(loop_end);
			ranking_time = 0x5DCu;
			scene_step += 2u;
			return;
		}
		t1 = count - 1u;
		count = t1;
		if (t1 == 0xFFFFu)
			break;
	}
	if (unranked == 4u)
	{
		back_to_attract();
		return;
	}
	sound = 0x71u;
	queue_sound(sound);
	loop_end = draw_ranking_background();
	loop_end = (loop_end & ~0xFFu) | turtles_in_black;
	draw_four_turtle_pictures(loop_end);
	ranking_time = 0x1E0u;
	scene_step++;
}

/* the ranking shown until the time runs out or a button */
void ranking_wait(void)
{
	uint8_t sound = 0;
	uint16_t t1 = 0;

	t1 = ranking_time;
	ranking_time = t1 - 1u;
	if (t1 != 1u && ((player_inputs(0) | player_inputs(1) | player_inputs(2) | player_inputs(3)) & 0x30u) == 0u)
		return;
	sound = 0xFEu;
	queue_sound(sound);
	ranking_time = 0x3Cu;
	scene_step = 7u;
}

/* the top ten players enter their initials with the joystick and a button */
void enter_initials(void)
{
	uint8_t buttons_held = 0, joystick = 0, letter = 0, letter_sound = 0, players_done = 0, sound = 0;
	uint16_t count = 0, initial_offset = 0;
	uint32_t entered = 0, initials = 0, inputs = 0, places = 0;
	uint32_t stack = 0;
	uint8_t t3 = 0;
	uint16_t t1 = 0, t2 = 0, t5 = 0;
	uint32_t turtle_inputs_0_long = 0, t4 = 0;

	t1 = ranking_time;
	ranking_time = t1 - 1u;
	if (t1 == 1u)
	{
		sound = 0xFEu;
		queue_sound(sound);
		name_entered = 1u;
		ranking_time = 0x3Cu;
		scene_step = 7u;
		return;
	}
	if ((frame_count & 7u) != 0u)
		return;
	count = 3u;
	do
	{
		if ((players_entering_names & shl32(1u, (int)(count & 7u))) != 0u)
			draw_initials(count);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	entered = initials_entered_ADDRESS;
	initials = ranking_initials_ADDRESS;
	places = ranking_places_ADDRESS;
	turtle_inputs_0_long = (((uint32_t)player_inputs(0) << 24u) | ((uint32_t)player_inputs(1) << 16u) | ((uint32_t)player_inputs(2) << 8u) | player_inputs(3)) | (((uint32_t)player_inputs_last_frame(0) << 24u) | ((uint32_t)player_inputs_last_frame(1) << 16u) | ((uint32_t)player_inputs_last_frame(2) << 8u) | player_inputs_last_frame(3)) | player_inputs_2_frames_ago | player_inputs_3_frames_ago;
	turtle_inputs(0) = (uint8_t)(turtle_inputs_0_long >> 24u);
	turtle_inputs(1) = (uint8_t)(turtle_inputs_0_long >> 16u);
	turtle_inputs(2) = (uint8_t)(turtle_inputs_0_long >> 8u);
	turtle_inputs(3) = (uint8_t)turtle_inputs_0_long;
	inputs = turtle_inputs_ADDRESS;
	players_done = 0u;
	buttons_held = 0u;
	count = 3u;
	do
	{
		if (initials_entered((uint32_t)(int16_t)count) < 3u)
		{
			if ((players_entering_names & shl32(1u, (int)(count & 7u))) != 0u)
			{
				t3 = turtle_inputs((uint32_t)(int16_t)count);
				joystick = t3;
				if (t3 != 0u)
				{
					if ((joystick & 0x30u) == 0u)
					{
						initial_offset = ranking_places((uint32_t)(int16_t)count);
						if (initial_offset == 0x10u)
							initial_offset = 0xAu;
						initial_offset = (uint16_t)((uint32_t)(uint8_t)(initial_offset - 1u) << 2);
						initial_offset = (uint16_t)((initial_offset & ~0xFFu) | (uint8_t)(initial_offset + initials_entered((uint32_t)(int16_t)count)));
						letter = (uint8_t)(ranking_initials(initial_offset) - 0x40u);
						if ((joystick & 1u) == 0u)
						{
							if ((joystick & 2u) != 0u)
							{
								ranking_initials(initial_offset) = ((letter + 1u) & 0x1Fu) + 0x40u;
								stack = players_done;
								letter_sound = 1u;
								queue_sound_d0(letter_sound);
								players_done = (uint8_t)stack;
							}
						}
						else
						{
							ranking_initials(initial_offset) = ((letter - 1u) & 0x1Fu) + 0x40u;
							stack = players_done;
							letter_sound = 1u;
							queue_sound_d0(letter_sound);
							players_done = (uint8_t)stack;
						}
					}
					else
					{
						buttons_held |= shl32(1u, (int)(count & 0x1Fu));
						if ((initials_buttons_held & shl32(1u, (int)(count & 7u))) != 0u)
						{
							initial_offset = ranking_places((uint32_t)(int16_t)count);
							if (initial_offset == 0x10u)
								initial_offset = 0xAu;
							initial_offset = (uint16_t)((uint32_t)(uint8_t)(initial_offset - 1u) << 2);
							initial_offset = (uint16_t)((initial_offset & ~0xFFu) | (uint8_t)(initial_offset + initials_entered((uint32_t)(int16_t)count)));
							letter = (uint8_t)(ranking_initials(initial_offset) - 0x40u);
							if ((joystick & 1u) == 0u)
							{
								if ((joystick & 2u) != 0u)
								{
									ranking_initials(initial_offset) = ((letter + 1u) & 0x1Fu) + 0x40u;
									stack = players_done;
									letter_sound = 1u;
									queue_sound_d0(letter_sound);
									players_done = (uint8_t)stack;
								}
							}
							else
							{
								ranking_initials(initial_offset) = ((letter - 1u) & 0x1Fu) + 0x40u;
								stack = players_done;
								letter_sound = 1u;
								queue_sound_d0(letter_sound);
								players_done = (uint8_t)stack;
							}
						}
						else
						{
							t4 = entered + (int16_t)count;
							mem_write8(t4, mem_read8(t4) + 1u);
							stack = players_done;
							letter_sound = 0x1Du;
							queue_sound_d0(letter_sound);
							players_done = (uint8_t)stack;
						}
					}
				}
			}
		}
		else
			players_done++;
		t5 = count - 1u;
		count = t5;
	} while (t5 != 0xFFFFu);
	initials_buttons_held = buttons_held;
	if (players_done == 4u)
	{
		sound = 0xFEu;
		queue_sound(sound);
		name_entered = 1u;
		ranking_time = 0x78u;
		scene_step++;
	}
}

/* a pause after the name entry */
void ranking_entry_done(void)
{
	uint16_t t1 = 0;

	t1 = ranking_time;
	ranking_time = t1 - 1u;
	if (t1 == 1u)
	{
		leave_ranking();
		return;
	}
}

/* the ranking's last pause */
void ranking_end_wait(void)
{
	uint16_t t1 = 0;

	t1 = ranking_time;
	ranking_time = t1 - 1u;
	if (t1 == 1u)
	{
		leave_ranking();
		return;
	}
}

/* BEST 10 PLAYERS after a name entered with no one left to play, else the attract mode */
void leave_ranking(void)
{
	if (players_with_lives != 0u || name_entered == 0u)
	{
		back_to_attract();
		return;
	}
	game_state = 1u;
	game_step = 6u;
	scene = 0u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
	game_on = 0u;
}

/* the game over, back to the attract mode's first step */
void back_to_attract(void)
{
	game_state = 1u;
	game_step = 0u;
	scene = 0u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
	game_on = 0u;
}

/* the ranking's background of coloured rows */
uint32_t draw_ranking_background(void)
{
	uint8_t tile_code = 0;
	uint16_t column_offset = 0, count_2 = 0;
	uint32_t count = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = TILE_RAM + 0x4F18u;
	count = 0x1Du;
	do
	{
		count_2 = 0x27u;
		tile_code = TABLE8(ranking_background_tiles, (uint32_t)(int16_t)count);
		do
		{
			column_offset = (uint16_t)((uint32_t)(uint8_t)count_2 << 1);
			io_write8(tile_place + column_offset, 0u);
			io_write8(tile_place + 1u + column_offset, tile_code);
			t1 = count_2 - 1u;
			count_2 = t1;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place -= 0x80u;
	} while (t2 != 0xFFFFu);
	return count;
}

/* a player's three initials, the one being entered highlighted */
void draw_initials(uint16_t count)
{
	uint8_t attribute = 0;
	uint16_t initial = 0, initials_offset = 0;
	uint32_t entered = 0, initials = 0, tile_place = 0;

	tile_place = TABLE32(ranking_initials_places, (uint32_t)(int16_t)((uint32_t)count << 2));
	entered = initials_entered_ADDRESS;
	initials = ranking_initials_ADDRESS;
	initials_offset = ranking_places((uint32_t)(int16_t)count);
	if (initials_offset == 0x10u)
		initials_offset = 0xAu;
	initials_offset = (uint16_t)((uint32_t)(uint8_t)(initials_offset - 1u) << 2);
	initial = ranking_initials(initials_offset);
	attribute = 0u;
	if (initials_entered((uint32_t)(int16_t)count) == 0u)
		attribute = 0xA0u;
	draw_initial(attribute, initial, tile_place);
	initial = ranking_initials(1 + initials_offset);
	attribute = 0u;
	tile_place += 4u;
	if (initials_entered((uint32_t)(int16_t)count) == 1u)
		attribute = 0xA0u;
	draw_initial(attribute, initial, tile_place);
	initial = ranking_initials(2 + initials_offset);
	attribute = 0u;
	tile_place += 4u;
	if (initials_entered((uint32_t)(int16_t)count) == 2u)
		attribute = 0xA0u;
	draw_initial(attribute, initial, tile_place);
}

/* a tall letter in a colour */
void draw_initial(uint8_t attribute, uint16_t initial, uint32_t tile_place)
{
	initial = (uint16_t)((uint32_t)(uint8_t)(initial - 0x30u) << 1);
	io_write8(tile_place, attribute);
	io_write8(tile_place + 1u, big_font_tiles_table[initial]);
	io_write8(tile_place + 0x80u, attribute);
	io_write8(tile_place + 0x81u, big_font_tiles_table[1 + initial]);
}

/* the frame of a player's column in his colour */
uint32_t draw_ranking_frame(uint32_t colour, uint16_t count)
{
	uint16_t count_2 = 0, tile_offset = 0;
	uint32_t tile_place = 0, tiles = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	colour = LINKED(colour, count);
	colour = LINKED(colour, (uint16_t)(colour << 2));
	tile_place = TABLE32(ranking_frame_places, (uint32_t)(int16_t)colour);
	colour = LINKED(colour, count);
	colour = ranking_frame_colour(colour);
	count_2 = 8u;
	tile_offset = 0u;
	tiles = ranking_frame_tiles_ADDRESS;
	do
	{
		tile_offset += 0x80u;
		io_write8(tile_place + (int16_t)tile_offset, colour);
		io_write8(tile_place + 1u + (int16_t)tile_offset, PICTURE_TILE(tiles + 0x9u));
		tile_offset -= 0x80u;
		io_write8(tile_place + (int16_t)tile_offset, colour);
		io_write8(tile_place + 1u + (int16_t)tile_offset, PICTURE_TILE(tiles));
		tile_offset = (uint16_t)((tile_offset & ~0xFFu) | (uint8_t)(tile_offset + 2u));
		t1 = count_2 - 1u;
		count_2 = t1;
		tiles++;
	} while (t1 != 0xFFFFu);
	count_2 = 0x17u;
	tile_place += 0x100u;
	do
	{
		io_write8(tile_place, colour);
		io_write8(tile_place + 1u, 0x1Au);
		io_write8(tile_place + 0x10u, colour);
		io_write8(tile_place + 0x11u, 0x1Du);
		t2 = count_2 - 1u;
		count_2 = t2;
		tile_place += 0x80u;
	} while (t2 != 0xFFFFu);
	count_2 = 8u;
	tile_offset = 0u;
	tiles = ranking_frame_tiles_2_ADDRESS;
	do
	{
		io_write8(tile_place + tile_offset, colour);
		io_write8(tile_place + 1u + tile_offset, PICTURE_TILE(tiles));
		tile_offset = (uint8_t)(tile_offset + 2u);
		t3 = count_2 - 1u;
		count_2 = t3;
		tiles++;
	} while (t3 != 0xFFFFu);
	return colour;
}

/* a player's label at the top of his column */
void draw_ranking_label(uint8_t colour, uint16_t count)
{
	uint32_t label = 0, tile_place = 0;

	tile_place = TABLE32(ranking_label_places, (uint32_t)(int16_t)((uint32_t)count << 2));
	label = TABLE32(box_labels, (uint32_t)(int16_t)((uint32_t)count << 2));
	io_write8(tile_place, colour);
	io_write8(tile_place + 2u, colour);
	io_write8(tile_place + 0x80u, colour);
	io_write8(tile_place + 0x82u, colour);
	io_write8(tile_place + 1u, TEXT(label));
	io_write8(tile_place + 3u, TEXT(label + 0x1u));
	io_write8(tile_place + 0x81u, TEXT(label + 0x2u));
	io_write8(tile_place + 0x83u, TEXT(label + 0x3u));
}

/* a player's place, its digits and ending */
void draw_ranking_place(uint16_t count, uint32_t places)
{
	uint8_t place = 0;
	uint16_t ending_offset = 0, tens_offset = 0, units_offset = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0;

	tile_place = TABLE32(ranking_place_places, (uint32_t)(int16_t)((uint32_t)count << 2));
	place = ranking_places((uint32_t)(int16_t)count);
	ending_offset = (uint16_t)place;
	ending_offset = (uint8_t)(ending_offset - 1u);
	if (ending_offset >= 3u)
		ending_offset = 3u;
	ending_offset = (uint16_t)((uint32_t)ending_offset << 1);
	io_write8(tile_place + 0x84u, 0u);
	io_write8(tile_place + 0x85u, ranking_place_tiles_1[ending_offset]);
	io_write8(tile_place + 0x86u, 0u);
	io_write8(tile_place + 0x87u, ranking_place_tiles_2[ending_offset]);
	units_offset = (uint16_t)((place & 0xFu) << 1);
	io_write8(tile_place + 2u, 0u);
	io_write8(tile_place + 3u, big_font_tiles_table[units_offset]);
	io_write8(tile_place + 0x82u, 0u);
	units_offset = (uint8_t)(units_offset + 1u);
	io_write8(tile_place + 0x83u, big_font_tiles_table[units_offset]);
	t1 = place & 0xF0u;
	tens_offset = t1;
	if (t1 != 0u)
	{
		tens_offset = tens_offset >> 3;
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, big_font_tiles_table[tens_offset]);
		io_write8(tile_place + 0x80u, 0u);
		tens_offset = (uint8_t)(tens_offset + 1u);
		io_write8(tile_place + 0x81u, big_font_tiles_table[tens_offset]);
	}
}

/* the colour of a player's column */
uint32_t ranking_frame_colour(uint32_t player_number)
{
	player_number = (player_number & ~0xFFu) | TABLE8(text_colours, 0x1u + (uint32_t)(int16_t)player_number);
	player_number = player_number | 1u;
	return player_number;
}

/* the ranking's default scores and initials, and the top score */
void reset_ranking(void)
{
	uint16_t count = 0;
	uint32_t defaults = 0, ranking_entry = 0;
	uint16_t t2 = 0, t4 = 0;
	uint32_t t1 = 0, t3 = 0;

	ranking_entry = ranking_scores_ADDRESS;
	defaults = ranking_defaults_ADDRESS;
	top_score = 0x1000u;
	count = 0x31u;
	do
	{
		t1 = ranking_entry;
		ranking_entry += 4u;
		mem_write32(t1, RANKING_DEFAULT(defaults));
		t2 = count - 1u;
		count = t2;
		defaults += 4u;
	} while (t2 != 0xFFFFu);
	count = 0xAu;
	do
	{
		t3 = ranking_entry;
		ranking_entry += 4u;
		mem_write32(t3, RANKING_DEFAULT(defaults));
		t4 = count - 1u;
		count = t4;
		defaults += 4u;
	} while (t4 != 0xFFFFu);
}
