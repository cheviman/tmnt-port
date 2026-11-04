/**
 *    Name:        scenes/demo.c
 *    Description: the players' inputs, the demos' recorded ones, and the demo game
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the players' inputs for the turtles, or a demo's recorded ones */
void update_game_inputs(void)
{
	uint8_t record_byte = 0;
	uint16_t channel_offset = 0, count = 0;
	uint32_t channels = 0, held_inputs = 0, hold_time = 0, inputs_place = 0, join_routine = 0, record = 0, record_place = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	if (game_on == 0u)
	{
		turtle_inputs_3_frames_ago = turtle_inputs_2_frames_ago;
		turtle_inputs_2_frames_ago = turtle_inputs_last_frame;
		turtle_inputs_last_frame = (((uint32_t)demo_held_inputs(0) << 24u) | ((uint32_t)demo_held_inputs(1) << 16u) | ((uint32_t)demo_held_inputs(2) << 8u) | demo_held_inputs(3));
		count = 3u;
		offset_high_word = 3u;
		channels = channel_records_ADDRESS;
		held_inputs = demo_held_inputs_ADDRESS;
		do
		{
			channel_offset = (uint16_t)((uint32_t)count << 4);
			record_place = mem_read32(channels + (int16_t)channel_offset);
			hold_time = mem_read32(channels + 4u + (int16_t)channel_offset);
			inputs_place = mem_read32(channels + 8u + (int16_t)channel_offset);
			join_routine = mem_read32(channels + 0xCu + (int16_t)channel_offset);
			record = mem_read32(record_place);
			t1 = mem_read8(hold_time);
			mem_write8(hold_time, t1 - 1u);
			if (t1 != 1u)
				mem_write8(inputs_place, demo_held_inputs((uint32_t)(int16_t)count));
			else
			{
				record_byte = DEMO_INPUT(record);
				record++;
				mem_write8(hold_time, DEMO_INPUT(record));
				scratch_byte = record_byte;
				record_byte &= 0x7Fu;
				mem_write8(inputs_place, record_byte);
				demo_held_inputs((uint32_t)(int16_t)count) = record_byte;
				mem_write32(record_place, record + 1u);
				switch (join_routine)
				{
				case 0x0048B2u:
					channels = demo_join_player_1(channels);
					break;
				case 0x0048E6u:
					channels = demo_join_player_2(channels);
					break;
				case 0x00491Au:
					channels = demo_join_player_3(channels);
					break;
				case 0x00494Eu:
					channels = demo_join_player_4(channels);
					break;
				}
			}
			t2 = count - 1u;
			count = t2;
		} while (t2 != 0xFFFFu);
		turtle_presses(0) = turtle_inputs(0) & ~(uint8_t)(turtle_inputs_last_frame >> 24u);
		turtle_presses(1) = turtle_inputs(1) & ~(uint8_t)(turtle_inputs_last_frame >> 16u);
		turtle_presses(2) = turtle_inputs(2) & ~(uint8_t)(turtle_inputs_last_frame >> 8u);
		turtle_presses(3) = turtle_inputs(3) & ~(uint8_t)turtle_inputs_last_frame;
		return;
	}
	turtle_inputs_3_frames_ago = turtle_inputs_2_frames_ago;
	turtle_inputs_2_frames_ago = turtle_inputs_last_frame;
	turtle_inputs_last_frame = (((uint32_t)turtle_inputs(0) << 24u) | ((uint32_t)turtle_inputs(1) << 16u) | ((uint32_t)turtle_inputs(2) << 8u) | turtle_inputs(3));
	turtle_inputs(0) = player_inputs(0);
	turtle_inputs(1) = player_inputs(1);
	turtle_inputs(2) = player_inputs(2);
	turtle_inputs(3) = player_inputs(3);
	turtle_presses(0) = player_presses(0);
	turtle_presses(1) = player_presses(1);
	turtle_presses(2) = player_presses(2);
	turtle_presses(3) = player_presses(3);
}

/* in a demo, the first player joins */
uint32_t demo_join_player_1(uint32_t channels)
{
	if ((int8_t)scratch_byte < 0 && (players_in_play & 1u) == 0u)
	{
		players_in_play |= 1u;
		players_joining |= 1u;
		channels = reset_player_1();
		PLAYER(PLAYER_ADDRESS(0))->lives = (PLAYER(PLAYER_ADDRESS(0))->lives & 0xFFu) | 0x100u;
	}
	return channels;
}

/* in a demo, the second player joins */
uint32_t demo_join_player_2(uint32_t channels)
{
	if ((int8_t)scratch_byte < 0 && (players_in_play & 2u) == 0u)
	{
		players_in_play |= 2u;
		players_joining |= 2u;
		channels = reset_player_2();
		PLAYER(PLAYER_ADDRESS(1))->lives = (PLAYER(PLAYER_ADDRESS(1))->lives & 0xFFu) | 0x100u;
	}
	return channels;
}

/* in a demo, the third player joins */
uint32_t demo_join_player_3(uint32_t channels)
{
	if ((int8_t)scratch_byte < 0 && (players_in_play & 4u) == 0u)
	{
		players_in_play |= 4u;
		players_joining |= 4u;
		channels = reset_player_3();
		PLAYER(PLAYER_ADDRESS(2))->lives = (PLAYER(PLAYER_ADDRESS(2))->lives & 0xFFu) | 0x100u;
	}
	return channels;
}

/* in a demo, the fourth player joins */
uint32_t demo_join_player_4(uint32_t channels)
{
	if ((int8_t)scratch_byte < 0 && (players_in_play & 8u) == 0u)
	{
		players_in_play |= 8u;
		players_joining |= 8u;
		channels = reset_player_4();
		PLAYER(PLAYER_ADDRESS(3))->lives = (PLAYER(PLAYER_ADDRESS(3))->lives & 0xFFu) | 0x100u;
	}
	return channels;
}

/* the demo game */
void attract_demo(void)
{
	if (new_credit != 0u)
	{
		frame_count = saved_frame_count;
		go_to_coin_screen();
		return;
	}
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		begin_demo();
		return;
	case 4:
		demo_play();
		return;
	case 8:
		end_demo();
		return;
	case 1:
		scene_choose_turtles();
		return;
	case 2:
		scene_area_setup();
		return;
	case 3:
		scene_area_title();
		return;
	case 5:
		scene_area_exit();
		return;
	case 6:
		scene_ending();
		return;
	case 7:
		scene_continue();
		return;
	}
}

/* the demo's start: the play cleared and the demo's settings */
void begin_demo(void)
{
	clear_play();
	PLAYER(PLAYER_ADDRESS(0))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(1))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(2))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(3))->health_bars = 0u;
	difficulty_setting = 0u;
	time_without_knockout = 0u;
	saved_frame_count = frame_count;
	frame_count = 0u;
	start_demo();
}

/* a demo starts: its area, its players and their recorded inputs by the demo's number, then the next
   scene */
void start_demo(void)
{
	uint16_t demo_offset = 0;

	demo_offset = (uint16_t)((uint32_t)demo << 2);
	area = demo_settings_table[demo_offset];
	players_in_play = demo_settings_table[1 + demo_offset];
	players_joining = demo_settings_table[2 + demo_offset];
	players_knocked_out = 0u;
	players_out_of_lives = 0u;
	demo_held_inputs(0) = 0u;
	demo_held_inputs(1) = 0u;
	demo_held_inputs(2) = 0u;
	demo_held_inputs(3) = 0u;
	demo_offset = (uint16_t)((uint32_t)demo_offset << 2);
	demo_inputs_1 = demo_input_streams[demo_offset / 4u];
	demo_inputs_2 = TABLE32(demo_input_streams, 0x4u + demo_offset);
	demo_inputs_3 = TABLE32(demo_input_streams, 0x8u + demo_offset);
	demo_inputs_4 = TABLE32(demo_input_streams, 0xCu + demo_offset);
	attract_timer = 1u;
	demo_hold_2 = 1u;
	demo_hold_3 = 1u;
	demo_hold_4 = 1u;
	demo_frames = 0u;
	scene++;
	scene_step = 0u;
}

/* the demo plays for a while */
void demo_play(void)
{
	if (players_in_play != 0u)
	{
		demo_frames++;
		if (demo_frames < 0x384u)
		{
			scene_play();
			return;
		}
	}
	scene = 8u;
	scene_step = 0u;
}

/* the demo's end: the next demo, the players out */
void end_demo(void)
{
	uint8_t sound = 0;

	sound = 0u;
	queue_sound(sound);
	layer_priority = 0u;
	io_write8(PRIORITY_0C0001, layer_priority);
	area_continues_scene = 0u;
	frame_count = saved_frame_count;
	demo++;
	if (demo >= 3u)
		demo = 0u;
	players_in_play = 0u;
	players_joining = 0u;
	players_knocked_out = 0u;
	players_out_of_lives = 0u;
	PLAYER(PLAYER_ADDRESS(0))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(1))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(2))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(3))->health_bars = 0u;
	game_step++;
	scene = 0u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
}
