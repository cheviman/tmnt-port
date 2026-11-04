/**
 *    Name:        scenes/continue.c
 *    Description: the continue's countdown and the GAME OVER after it
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the continue with its countdown, then GAME OVER, by its steps */
void scene_continue(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		continue_setup();
		break;
	case 1:
		continue_run();
		break;
	case 2:
		game_over();
		break;
	}
	do_nothing();
}

/* the continue's first step, by its steps */
void continue_setup(void)
{
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		continue_clear_text();
		break;
	case 1:
		continue_start();
		break;
	}
	check_joins();
}

/* the texts cleared for the continue */
void continue_clear_text(void)
{
	uint16_t job = 0;

	jobs_pending = 1u;
	job = 1u;
	queue_job(job);
	scene_substep++;
}

/* CONTINUE, the count from twenty and the continue's music */
void continue_start(void)
{
	uint8_t sound = 0;
	uint16_t job = 0;

	if (jobs_pending != 0u)
		return;
	job = 0xB7Fu;
	queue_job(job);
	job = 0xF02u;
	queue_job(job);
	continue_count = 0x14u;
	continue_count_time = 1u;
	sound = 0u;
	queue_sound(sound);
	sound = 0x77u;
	queue_sound(sound);
	scene_step++;
	scene_substep = 0u;
}

/* the countdown, the messages under the boxes and the boxes of the players with lives */
void continue_run(void)
{
	uint16_t job = 0, substep_offset = 0;

	substep_offset = (uint16_t)((uint32_t)(int8_t)scene_substep << 2);
	switch ((uint32_t)(int16_t)substep_offset)
	{
	case 0:
		continue_countdown(substep_offset);
		break;
	}
	update_player_messages();
	job = (uint16_t)(0xB00u | 0x10u | players_with_lives);
	queue_job(job);
}

/* the countdown: a count less every so often, the play back when a player joins, GAME OVER at zero */
void continue_countdown(uint32_t count_left)
{
	uint8_t player_inputs_now = 0;
	uint16_t count = 0, job = 0;
	uint32_t inputs = 0, player = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0, t3 = 0;

	if (jobs_pending != 0u)
	{
		check_joins();
		return;
	}
	t1 = continue_count_time;
	continue_count_time = t1 - 1u;
	if (t1 == 1u)
	{
		if (continue_count == 0u)
		{
			scene_step++;
			scene_substep = 0u;
			continue_count = 0u;
			continue_count_time = 0u;
			job = 0x200u;
			queue_job(job);
			return;
		}
		continue_count_time = 0x48u;
		continue_count--;
		count_left = continue_count;
		count_left = draw_big_character((uint8_t)count_left);
	}
	if ((players_joining | players_in_play) != 0u)
	{
		jobs_pending = 1u;
		job = 1u;
		queue_job(job);
		job = 0xB7Fu;
		queue_job(job);
		start_area_music();
		scene = 4u;
		scene_step = 1u;
		scene_substep = 0u;
		return;
	}
	if (continue_count > 0x10u)
	{
		check_joins();
		return;
	}
	count = (uint16_t)((count_left & ~0xFFu) | 3u);
	inputs = player_inputs_ADDRESS;
	player = PLAYER_ADDRESS(0);
	for (;;)
	{
		player_inputs_now = mem_read8(inputs);
		inputs++;
		if ((player_inputs_now & 0x30u) != 0u && PLAYER(player)->lives == 0u)
		{
			t2 = frame_count & 7u;
			count = t2;
			if (t2 == 0u)
				break;
		}
		t3 = count - 1u;
		count = t3;
		player += 8u;
		if (t3 == 0xFFFFu)
		{
			check_joins();
			return;
		}
	}
	continue_count_time = 1u;
	check_joins();
}

/* the GAME OVER after the continue, by its steps */
void game_over(void)
{
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		game_over_clear_text();
		return;
	case 1:
		game_over_start();
		return;
	case 2:
		game_over_wait();
		return;
	}
}

/* the texts cleared for GAME OVER */
void game_over_clear_text(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 1u;
	queue_job(job);
	scene_substep++;
}

/* the GAME OVER box with its colours and music */
void game_over_start(void)
{
	uint8_t sound = 0;
	uint16_t text_number = 0;
	uint32_t palette_effect_pointer = 0;

	if (jobs_pending != 0u)
		return;
	continue_count = 0xF0u;
	text_number = 1u;
	print_colour_text(text_number);
	clear_palette_effects();
	palette_effect_pointer = 0xD2C4u;
	start_palette_effects(palette_effect_pointer);
	palette_effect_pointer = 0xD2E6u;
	start_palette_effects(palette_effect_pointer);
	fade_state = 0u;
	scene_substep++;
	sound = 0x6Du;
	queue_sound(sound);
}

/* GAME OVER held over the darkened play until the time runs out or a button, then the ranking */
void game_over_wait(void)
{
	uint8_t t1 = 0;

	if (jobs_pending == 0u)
	{
		game_over_skip();
		game_over_dim();
		t1 = continue_count;
		continue_count = t1 - 1u;
		if (t1 == 1u)
		{
			scene++;
			scene_step = 0u;
			scene_substep = 0u;
		}
	}
}

/* the play darkened behind the GAME OVER box */
void game_over_dim(void)
{
	if (fading == 0u)
	{
		palette_place = 0xA08Eu;
		fade_banks = 0x1E02u;
		fade_state = 0u;
	}
	copy_palette_sets();
}

/* a button ends GAME OVER early */
void game_over_skip(void)
{
	uint8_t sound = 0;

	if (continue_count >= 0xB0u || ((player_presses(0) | player_presses(1) | player_presses(2) | player_presses(3)) & 0x30u) == 0u)
		return;
	sound = 0u;
	queue_sound(sound);
	scene++;
	scene_step = 0u;
	scene_substep = 0u;
}
