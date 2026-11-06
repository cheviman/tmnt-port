/**
 *    Name:        scenes/game.c
 *    Description: the game's scenes: a new game, the players' turtles, an area's set-up, its story and title screens, the play, the area's exit
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a new game, by its steps */
void scene_new_game(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		new_game_reset();
		break;
	case 1:
		new_game_first_area();
		break;
	}
	check_joins();
}

/* the players' records and scores, the scroll, the difficulty and the play time reset for a new game */
void new_game_reset(void)
{
	uint8_t players = 0;

	if (jobs_pending == 0u)
	{
		clear_play();
		reset_player_1();
		reset_player_2();
		reset_player_3();
		reset_player_4();
		players = 0xFu;
		clear_scores(players);
		layer_a_scroll_x = 0u;
		layer_a_scroll_y = 0u;
		layer_b_scroll_x = 0u;
		layer_b_scroll_y = 0u;
		difficulty_setting = (dip_switches_2 & 0x60u) >> 5;
		difficulty_from_time = 0u;
		hide_press_start = 0u;
		scene_step++;
	}
}

/* the game starts at the first area */
void new_game_first_area(void)
{
	area = 0u;
	scene++;
	scene_step = 0u;
}

/* the turtle each player plays, by its steps: in this set each player his own */
void scene_choose_turtles(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		choose_turtles_clear();
		break;
	case 1:
		choose_turtles_fixed();
		break;
	case 2:
		choose_turtles_done();
		break;
	}
	check_joins();
}

/* every player on the first turtle */
void choose_turtles_clear(void)
{
	player_turtles(0) = 0u;
	player_turtles(1) = 0u;
	player_turtles(2) = 0u;
	player_turtles(3) = 0u;
	scene_step++;
}

/* each player on his own turtle, Leonardo first */
void choose_turtles_fixed(void)
{
	player_turtles(0) = 0u;
	player_turtles(1) = 1u;
	player_turtles(2) = 2u;
	player_turtles(3) = 3u;
	scene_step++;
}

/* on to the area's set-up */
void choose_turtles_done(void)
{
	scene++;
	scene_step = 0u;
}

/* an area's set-up, by its steps */
void scene_area_setup(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		area_setup_variables();
		break;
	case 1:
		area_setup_clear_objects();
		break;
	case 2:
		area_setup_done();
		break;
	}
	check_joins();
}

/* the area's stage and variables set, the play's work and the layers cleared */
void area_setup_variables(void)
{
	uint16_t area_offset = 0, job = 0;

	area_offset = (uint16_t)((uint32_t)area << 1);
	area_continues_scene = area_continue_scenes[area_offset];
	stage = area_stages[area_offset];
	unused_area_setup_bytes(0) = 0u;
	unused_area_setup_bytes(1) = 0u;
	exit_cutscene = 0u;
	clear_play();
	area_cleared = 0u;
	area_beaten = 0u;
	hide_press_start = 0u;
	scene_step++;
	jobs_pending++;
	job = 7u;
	queue_job(job);
}

/* the objects cleared once the layers are */
void area_setup_clear_objects(void)
{
	if (jobs_pending == 0u)
	{
		clear_objects();
		scene_step++;
	}
}

/* on to the area's story and title */
void CONSOLE_ORIGINAL(area_setup_done)(void)
{
	scene++;
	scene_step = 0u;
}

/* the area's story and title screens, by their steps */
void scene_area_title(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		area_title_intro();
		break;
	case 1:
		area_title_screen();
		break;
	case 2:
		area_title_done();
		break;
	}
	check_joins();
}

/* in a game's first area the intro, by its steps; else straight to its end */
void area_title_intro(void)
{
	if (area != 0u || game_on == 0u)
	{
		area_title_intro_end();
		return;
	}
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		area_title_intro_start();
		return;
	case 1:
		area_title_intro_run();
		return;
	}
}

/* the intro started on a clean screen */
void area_title_intro_start(void)
{
	scene_substep++;
	game_scene_timer = 0xB4u;
	io_write8(PRIORITY_0C0001, 0u);
	layer_priority = 0u;
	clear_objects();
	clear_sprite_ram();
	start_intro();
	hide_all_sprites();
}

/* the intro played until it ends or a button skips it */
void area_title_intro_run(void)
{
	uint8_t sound = 0;
	uint32_t ignored = 0;
	uint8_t t1 = 0;

	if (intro_over != 0u)
	{
		area_title_intro_end();
		return;
	}
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
		game_scene_timer = 0u;
	if (palettes_busy != 0u || game_scene_timer != 0u || ((player_presses(0) | player_presses(1) | player_presses(2) | player_presses(3)) & 0x30u) == 0u)
	{
		run_intro();
		build_draw_list();
		draw_sprites(ignored);
		return;
	}
	sound = 0x7Eu;
	queue_sound(sound);
	area_title_intro_end();
}

/* the screen cleared after the intro */
void area_title_intro_end(void)
{
	clear_palette_effects();
	clear_objects();
	clear_sprite_ram();
	clear_layers();
	scene_step++;
	scene_substep = 0u;
	scene_subsubstep = 0u;
}

/* the area's title screen, by its steps */
void area_title_screen(void)
{
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		area_title_clear();
		return;
	case 1:
		area_title_reset();
		return;
	case 2:
		area_title_start_text();
		return;
	case 3:
		area_title_wait();
		return;
	}
}

/* the layers cleared and the area's palettes requested */
void area_title_clear(void)
{
	uint16_t job = 0, kind = 0;
	uint32_t data = 0;

	scene_substep++;
	jobs_pending++;
	job = 7u;
	queue_job(job);
	data = area_palettes_ADDRESS;
	kind = 0x10Fu;
	post_palette_request(kind, data);
}

/* the objects cleared and the scroll reset */
void area_title_reset(void)
{
	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		clear_objects();
		reset_layers_scroll();
		scene_substep++;
	}
}

/* the area's SCENE text started and the boxes drawn, in a game and for the areas that have one */
void area_title_start_text(void)
{
	uint16_t job = 0;
	uint32_t box_flags = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, text = 0;

	scene_substep++;
	job = 0x200u;
	queue_job(job);
	if (game_on == 0u || (int8_t)area_continues_scene < 0)
	{
		scene_step++;
		scene_substep = 0u;
		return;
	}
	text = area_title_texts[stage];
	ignored = start_typed_text_at_once(text);
	game_scene_timer = 0xB4u;
	box_flags = 0xB7Fu;
	offset_high_word = box_flags;
	draw_player_boxes((uint16_t)box_flags, &ignored_2, &ignored_3, &ignored_4);
}

/* the text typed and the health bars filled until the time runs out or a button */
void area_title_wait(void)
{
	uint16_t box_flags = 0, count = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, player = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	refill_health_bars();
	offset_high_word = 0u;
	box_flags = (uint16_t)(players_in_play + 0xB20u);
	draw_player_boxes(box_flags, &ignored, &ignored_2, &ignored_3);
	update_players_in_game();
	type_next_character();
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 != 1u && (game_scene_timer >= 0x8Cu || ((player_presses(0) | player_presses(1) | player_presses(2) | player_presses(3)) & 0x30u) == 0u))
		return;
	count = 3u;
	player = PLAYER_ADDRESS(3);
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
			PLAYER(player)->health_bars = 0xAu;
		t2 = count - 1u;
		count = t2;
		player -= 8u;
	} while (t2 != 0xFFFFu);
	scene_step++;
	scene_substep = 0u;
}

/* a bar more of health every 8 frames for the players in play, with a tick */
void refill_health_bars(void)
{
	uint8_t refilled = 0, sound = 0;
	uint16_t count = 0;
	uint32_t player = 0;
	uint16_t t1 = 0;

	if (game_scene_timer >= 0xAAu || (frame_count & 7u) != 0u)
		return;
	refilled = 0u;
	count = 3u;
	player = PLAYER_ADDRESS(3);
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && PLAYER(player)->health_bars < 0xAu)
		{
			PLAYER(player)->health_bars++;
			refilled = 1u;
		}
		t1 = count - 1u;
		count = t1;
		player -= 8u;
	} while (t1 != 0xFFFFu);
	if (refilled != 0u)
	{
		sound = 1u;
		queue_sound(sound);
	}
}

/* on to the play */
void area_title_done(void)
{
	scene++;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
}

/* the play, by its steps: the area's entry, then the play's frames */
void scene_play(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		play_entry();
		return;
	case 1:
		play_main();
		return;
	}
}

/* the area's entry, by its steps */
void play_entry(void)
{
	uint16_t substep_offset = 0;

	substep_offset = (uint16_t)((uint32_t)(int8_t)scene_substep << 2);
	switch ((uint32_t)(int16_t)substep_offset)
	{
	case 0:
		play_entry_palettes();
		return;
	case 4:
		play_entry_layers();
		return;
	case 8:
		play_entry_show_game_over();
		return;
	case 12:
		play_entry_start(substep_offset);
		return;
	case 16:
		play_entry_run();
		return;
	}
}

/* the layers cleared, the area's palettes and palette effects requested */
void play_entry_palettes(void)
{
	uint16_t job = 0, kind = 0;
	uint32_t data = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	data = area_palettes_ADDRESS;
	kind = 0x10Fu;
	post_palette_request(kind, data);
	start_area_palette_effects();
	scene_substep++;
}

/* the scroll, the objects, the boxes, and the stage's layer priority and tile banks */
void play_entry_layers(void)
{
	uint16_t job = 0, stage_number = 0;

	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		reset_layers_scroll();
		clear_objects();
		job = 0xB7Fu;
		queue_job(job);
		stage_number = stage;
		layer_priority = area_layer_priorities[stage_number];
		io_write8(PRIORITY_0C0001, layer_priority);
		entry_without_blink = area_entry_without_blink[stage_number];
		stage_number = (uint16_t)((uint32_t)stage_number << 1);
		io_write8(TILES_106B00, area_tile_banks_table[stage_number]);
		tile_banks_0_1 = area_tile_banks_table[stage_number];
		io_write8(TILES_106E00, area_tile_banks_table[1 + stage_number]);
		tile_banks_2_3 = area_tile_banks_table[1 + stage_number];
		scene_substep++;
	}
}

/* the GAME OVER box when a demo plays */
void play_entry_show_game_over(void)
{
	uint16_t text_number = 0;

	if (jobs_pending != 0u)
		return;
	if (game_on == 0u)
	{
		text_number = 1u;
		print_colour_text(text_number);
	}
	scene_substep++;
}

/* the turtles, the spawns, the players' slots and the music started for the area */
void play_entry_start(uint16_t count)
{
	uint8_t score_players = 0, starting_players = 0;
	uint32_t player = 0;
	uint16_t t1 = 0;

	if (jobs_pending != 0u)
		return;
	if (game_on == 0u)
	{
		player = PLAYER_ADDRESS(3);
		count = (uint16_t)((count & ~0xFFu) | 3u);
		do
		{
			if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
				PLAYER(player)->lives = 1u;
			t1 = count - 1u;
			count = t1;
			player -= 8u;
		} while (t1 != 0xFFFFu);
	}
	if (game_on == 0u)
	{
		frame_count = 0u;
		difficulty = 0u;
	}
	area_entry_done = 0u;
	offset_high_word = 0u;
	starting_players = players_in_play & ~players_joining;
	turtle_start_area(starting_players);
	update_players_in_game();
	entry_without_blink = 0u;
	score_players = 0xFu;
	draw_scores(score_players);
	start_area_spawns();
	start_player_slots();
	hide_all_sprites();
	run_objects();
	run_objects();
	music = 0u;
	start_area_music();
	scene_substep++;
	hide_press_start = 1u;
}

/* the turtles walk in until the entry is done */
void play_entry_run(void)
{
	uint16_t job = 0;
	uint32_t ignored = 0;

	update_players_in_game();
	do_nothing();
	if (area_entry_done == 0u)
	{
		run_area_entry();
		check_attacks();
		build_draw_list();
		draw_sprites(ignored);
		return;
	}
	job = 0xB7Fu;
	queue_job(job);
	if (game_on == 0u)
	{
		frame_count = 0u;
		random_seed = 0u;
	}
	scene_step++;
	scene_substep = 0u;
	hide_press_start = 0u;
}

/* the play's frame with the inputs and the players who join */
void play_main(void)
{
	update_game_inputs();
	play_frame();
	check_joins();
}

/* the play's frame work, then the continue when no one plays or the exit when the area is cleared */
void play_frame(void)
{
	uint8_t frame = 0, frame_phase = 0;
	uint16_t text_number = 0;
	uint32_t object = 0;

	if (game_on == 0u)
	{
		frame = (uint8_t)frame_count;
		frame_phase = (uint8_t)frame;
		if ((frame & 0xFu) == 0u)
		{
			text_number = 1u;
			if ((frame_phase & 0x30u) == 0u)
				text_number = text_number | 0x80u;
			print_colour_text(text_number);
		}
	}
	update_players_in_game();
	do_nothing();
	update_difficulty();
	update_time_without_knockout();
	update_players();
	update_area_layers();
	run_objects();
	check_attacks();
	object = build_draw_list();
	object = draw_sprites(object);
	sound_flying_drones(object);
	if (stage < 2u)
		flicker_fire_priority();
	if ((players_out_of_lives | players_joining | players_in_play) == 0u)
	{
		scene = 7u;
		scene_step = 0u;
		scene_substep = 0u;
		scene_subsubstep = 0u;
		return;
	}
	if (area_cleared == 0u || ((players_knocked_out | players_out_of_lives) & 0xFu) != 0u)
		return;
	scene = 5u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
}

/* the fire flickering over the sprites in the first two stages */
void flicker_fire_priority(void)
{
	uint8_t priority = 0;

	if ((uint8_t)drill_arrival == 0u || (uint8_t)drill_arrival == 0xFFu)
	{
		priority = fire_priority_flicker[(uint8_t)frame_count & 7u];
		layer_priority = priority;
		io_write8(PRIORITY_0C0001, priority);
		return;
	}
	io_write8(PRIORITY_0C0001, 4u);
	layer_priority = 4u;
}

/* the enemies' level from the settings, the players in play and the play time */
void CONSOLE_ORIGINAL(update_difficulty)(void)
{
	uint16_t count = 0;
	uint16_t t1 = 0;

	difficulty = 2u;
	if (game_on == 0u)
		return;
	difficulty = difficulty_setting;
	count = 3u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
			difficulty++;
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	difficulty += difficulty_from_time;
	if (difficulty >= 7u)
		difficulty = 7u;
}

/* the area's music or a boss's in a game */
void start_area_music(void)
{
	uint8_t sound = 0;
	uint8_t t1 = 0;

	if (game_on == 0u)
		return;
	if (drill_rumbling != 0u)
	{
		sound = 0x34u;
		queue_sound(sound);
		if (stage == 6u && helicopter_count != 0u)
		{
			sound = 0xAu;
			queue_sound(sound);
		}
		return;
	}
	t1 = music;
	sound = t1;
	if (t1 == 0u)
	{
		sound = TABLE8(area_music, (uint32_t)(int16_t)(int8_t)stage);
		music = sound;
	}
	queue_sound(sound);
	if (stage == 6u && helicopter_count != 0u)
	{
		sound = 0xAu;
		queue_sound(sound);
	}
}

/* the area's exit, by its steps */
void scene_area_exit(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		area_exit_begin();
		break;
	case 1:
		area_exit_play();
		break;
	case 2:
		area_exit_next_area();
		break;
	}
	do_nothing();
	if (exit_cutscene != 0u)
		return;
	update_players_in_game();
}

/* the turtles start walking out of the cleared area */
void area_exit_begin(void)
{
	uint8_t sound = 0;

	scene_step++;
	if (stage != 0u)
	{
		sound = 0xFEu;
		queue_sound(sound);
	}
	turtle_start_area_exit();
	hide_press_start = 1u;
	unused_exit_wait = 0x78u;
}

/* the exit's frame */
void area_exit_play(void)
{
	uint32_t ignored = 0;

	run_area_exit();
	build_draw_list();
	draw_sprites(ignored);
	if (stage < 2u)
		flicker_fire_priority();
}

/* on to the next area, or to the ending after the last one */
void area_exit_next_area(void)
{
	uint8_t sound = 0;

	sound = 0u;
	queue_sound(sound);
	if (stage == 9u)
	{
		scene++;
		scene_step = 0u;
		return;
	}
	area++;
	area_cleared = 0u;
	scene = 2u;
	scene_step = 0u;
}
