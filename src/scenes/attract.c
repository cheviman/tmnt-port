/**
 *    Name:        scenes/attract.c
 *    Description: the attract mode's steps: the title, the coin's screen, the opening's steps, a character's profile, BEST 10 PLAYERS, its end
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* layer B all one tile */
void fill_layer_b(uint8_t tile_number, uint32_t colour_number)
{
	uint16_t count = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0;

	colour_number = fill_colour(colour_number);
	tile_number += 0x30u;
	count = 0xFFEu;
	tile_place = TILE_RAM + 0x4000u;
	do
	{
		io_write8(tile_place, colour_number);
		tile_place++;
		io_write8(tile_place, tile_number);
		t1 = count - 1u;
		count = t1;
		tile_place++;
	} while (t1 != 0xFFFFu);
}

/* the colour of the tile layer B is filled with */
uint32_t fill_colour(uint32_t colour_number)
{
	uint32_t colour = 0;

	colour = text_colours[colour_number];
	return colour;
}

/* nothing */
void do_nothing(void)
{
}

/* the title screen */
void attract_title(void)
{
	uint8_t sound = 0;

	if (new_credit != 0u)
	{
		sound = 0x7Eu;
		queue_sound(sound);
		go_to_coin_screen();
		return;
	}
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		title_setup();
		return;
	case 1:
		title_logo();
		return;
	}
}

/* the title's screen set up */
void title_setup(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		title_clear();
		return;
	case 1:
		title_palettes();
		return;
	case 2:
		title_background();
		return;
	}
}

/* the title's screen cleared */
void title_clear(void)
{
	uint16_t job = 0;

	if (jobs_pending == 0u)
	{
		clear_play();
		jobs_pending++;
		job = 7u;
		queue_job(job);
		scene_step++;
	}
}

/* the title's palettes */
void title_palettes(void)
{
	if (jobs_pending == 0u)
	{
		request_game_step_palettes();
		clear_palette_effects();
		start_game_step_palette_effects();
		scene_step++;
	}
}

/* the title's blue background */
void title_background(void)
{
	uint8_t tile_number = 0;
	uint16_t colour_number = 0;

	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		do_nothing();
		layer_b_scroll_x = 0x60u;
		layer_b_scroll_y = 0u;
		colour_number = 1u;
		tile_number = 3u;
		fill_layer_b(tile_number, colour_number);
		hide_all_sprites();
		scene++;
		scene_step = 0u;
	}
}

/* the title's logo */
void title_logo(void)
{
	uint32_t ignored = 0;

	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		start_title_logo();
		break;
	case 1:
		title_banner_move();
		break;
	case 2:
		title_turtles_slide_in();
		break;
	case 3:
		title_wait();
		break;
	}
	build_draw_list();
	draw_sprites(ignored);
}

/* the logo's pieces placed */
void start_title_logo(void)
{
	uint16_t animation_number = 0, count = 0, piece_offset = 0;
	uint32_t animation_entry = 0, animations = 0, ignored = 0, object = 0, piece_places = 0, title_version = 0;
	uint16_t t1 = 0;

	object = OBJECT_ADDRESS(12);
	count = 2u;
	do
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->colour = 0xFFu;
		title_version = version;
		title_version = (uint16_t)(title_version << 3);
		animations = title_logo_animations_ADDRESS + title_version;
		animation_number = 0u;
		if ((uint8_t)count != 0u)
		{
			animation_number = 1u;
			if ((uint8_t)count != 2u)
			{
				animation_number = 8u;
				animations -= title_version;
			}
		}
		animation_entry = ANIMATION_ENTRY(animations + (animation_number & 0x3FFFu) * 0x4u);
		set_animation(ignored, object, animation_entry);
		piece_places = title_logo_places_ADDRESS;
		if ((version & 2u) != 0u)
			piece_places = hero_title_logo_places_ADDRESS;
		piece_offset = (uint16_t)((uint32_t)count << 2);
		OBJECT(object)->screen_x = LOGO_PLACE(piece_places + (int16_t)piece_offset);
		OBJECT(object)->screen_y = LOGO_PLACE(piece_places + (int16_t)piece_offset + 0x2u);
		t1 = count - 1u;
		count = t1;
		object += 0x50u;
	} while (t1 != 0xFFFFu);
	initials_entered(0) = 0u;
	initials_entered(1) = 0u;
	scene_step++;
}

/* the banner's moves */
void title_banner_move(void)
{
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		title_banner_drop();
		break;
	case 1:
		title_banner_bounce();
		break;
	case 2:
		title_banner_wait();
		break;
	}
}

/* the banner drops to the middle */
void title_banner_drop(void)
{
	OBJECT(OBJECT_ADDRESS(14))->screen_y -= 7u;
	if (OBJECT(OBJECT_ADDRESS(14))->screen_y < 0x58u)
	{
		scene_substep++;
		initials_entered(0) = 0u;
		initials_entered(1) = 7u;
	}
}

/* the banner bounces back to the top */
void title_banner_bounce(void)
{
	uint16_t screen_y = 0;
	uint32_t object = 0;
	uint32_t initials_entered_0_halfword = 0;

	object = OBJECT_ADDRESS(14);
	if ((((uint32_t)initials_entered(0) << 8u) | initials_entered(1)) != 0xFFF9u)
	{
		initials_entered_0_halfword = (((uint32_t)initials_entered(0) << 8u) | initials_entered(1)) - 1u;
		initials_entered(0) = (uint8_t)(initials_entered_0_halfword >> 8u);
		initials_entered(1) = (uint8_t)initials_entered_0_halfword;
	}
	OBJECT(object)->screen_y -= (((uint32_t)initials_entered(0) << 8u) | initials_entered(1));
	screen_y = 0xC0u;
	if ((version & 2u) != 0u)
		screen_y = 0xD0u;
	if (screen_y < OBJECT(object)->screen_y)
	{
		OBJECT(object)->screen_y = screen_y;
		scene_substep++;
		attract_timer = 0x14u;
		initials_entered(0) = 0u;
		initials_entered(1) = 0u;
	}
}

/* the banner stays a moment */
void title_banner_wait(void)
{
	uint8_t t1 = 0;

	t1 = attract_timer;
	attract_timer = t1 - 1u;
	if (t1 == 1u)
	{
		scene_step++;
		scene_substep = 0u;
		return;
	}
}

/* TURTLES slides in, then the copyright */
void title_turtles_slide_in(void)
{
	uint32_t jobs = 0, object = 0;

	object = OBJECT_ADDRESS(12);
	OBJECT(OBJECT_ADDRESS(12))->screen_x -= 0xAu;
	if (OBJECT(OBJECT_ADDRESS(12))->screen_x >= 0x100u)
		return;
	OBJECT(object)->screen_x = 0x100u;
	attract_timer = 0xF0u;
	scene_step++;
	jobs = ninja_title_jobs_ADDRESS;
	if ((version & 1u) != 0u)
		jobs = hero_title_jobs_ADDRESS;
	queue_jobs(jobs);
}

/* the title stays, then the attract mode goes on */
void title_wait(void)
{
	uint8_t t1 = 0;

	t1 = attract_timer;
	attract_timer = t1 - 1u;
	if (t1 == 1u)
	{
		game_step++;
		scene = 0u;
		scene_step = 0u;
		return;
	}
}

/* a character's profile */
void attract_profile(void)
{
	if (new_credit != 0u)
	{
		go_to_coin_screen();
		return;
	}
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		profile_setup();
		return;
	case 1:
		profile_show();
		return;
	}
}

/* the profile's screen set up */
void profile_setup(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		profile_pause();
		return;
	case 1:
		profile_clear();
		return;
	case 2:
		profile_clear_layers();
		return;
	case 3:
		profile_palettes();
		return;
	case 4:
		profile_start_text();
		return;
	}
}

/* a short pause */
void profile_pause(void)
{
	step_timer = 2u;
	scene_step++;
}

/* the play cleared */
void profile_clear(void)
{
	uint8_t t1 = 0;

	if (jobs_pending == 0u)
	{
		t1 = step_timer;
		step_timer = t1 - 1u;
		if (t1 == 0u)
		{
			clear_play();
			scene_step++;
		}
	}
}

/* the layers cleared */
void profile_clear_layers(void)
{
	uint16_t job = 0;

	if (jobs_pending == 0u)
	{
		jobs_pending++;
		job = 7u;
		queue_job(job);
		scene_step++;
	}
}

/* the profile's palettes */
void profile_palettes(void)
{
	if (jobs_pending == 0u)
	{
		request_game_step_palettes();
		clear_palette_effects();
		start_game_step_palette_effects();
		scene_step++;
	}
}

/* the profile's text starts typing */
void profile_start_text(void)
{
	uint32_t carry = 0, extend = 0;
	uint8_t delay_shift = 0, sound = 0;
	uint16_t profile_number = 0;
	uint32_t text = 0;

	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		profile_number = profile_character;
		carry = profile_number >> 14;
		extend = carry;
		text = profile_texts[profile_number & 0x3FFFu];
		delay_shift = 1u;
		start_text_typed_in_colours(delay_shift, text, extend);
		hide_all_sprites();
		sound = 0x78u;
		queue_sound(sound);
		scene++;
		scene_step = 0u;
	}
}

/* the profile's picture and text */
void profile_show(void)
{
	uint32_t ignored = 0;

	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		profile_start_picture();
		break;
	case 1:
		profile_move_picture();
		break;
	case 2:
		profile_type_text();
		break;
	case 3:
		profile_wait();
		break;
	}
	build_draw_list();
	draw_sprites(ignored);
}

/* the character's picture placed */
void profile_start_picture(void)
{
	uint32_t animation = 0, motion = 0, picture = 0, profile_number = 0;

	if (jobs_pending == 0u)
	{
		picture = OBJECT_ADDRESS(12);
		OBJECT(OBJECT_ADDRESS(12))->flags = 0x81u;
		OBJECT(OBJECT_ADDRESS(12))->contacts = 0u;
		OBJECT(OBJECT_ADDRESS(12))->colour = 0xFFu;
		animation = profile_animations[profile_character];
		set_animation(0u, picture, animation);
		profile_number = profile_character;
		profile_number = (uint16_t)(profile_number << 4);
		motion = profile_motions_ADDRESS + profile_number;
		OBJECT(OBJECT_ADDRESS(12))->screen_x = PROFILE_MOTION(motion)->x;
		motion += 2u;
		OBJECT(OBJECT_ADDRESS(12))->screen_y = PROFILE_MOTION(motion - 0x2u)->y;
		motion += 2u;
		OBJECT(OBJECT_ADDRESS(12))->profile.end_x = PROFILE_MOTION(motion - 0x4u)->end_x;
		motion += 2u;
		OBJECT(OBJECT_ADDRESS(12))->profile.end_y = PROFILE_MOTION(motion - 0x6u)->end_y;
		motion += 2u;
		OBJECT(OBJECT_ADDRESS(12))->x_speed = PROFILE_MOTION(motion - 0x8u)->x_speed;
		OBJECT(OBJECT_ADDRESS(12))->y_speed = PROFILE_MOTION(motion - 0x8u)->y_speed;
		scene_step++;
	}
}

/* the picture moves into its place */
void profile_move_picture(void)
{
	uint8_t sound = 0;
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t picture = 0;

	if (sound_stopped_by_coin != 0u)
	{
		sound = 0x78u;
		queue_sound(sound);
	}
	picture = OBJECT_ADDRESS(12);
	screen_x = (uint16_t)(OBJECT(OBJECT_ADDRESS(12))->x_speed + OBJECT(OBJECT_ADDRESS(12))->screen_x);
	if (screen_x == OBJECT(OBJECT_ADDRESS(12))->profile.end_x)
		OBJECT(picture)->x_speed = 0;
	OBJECT(picture)->screen_x = screen_x;
	screen_y = (uint16_t)(OBJECT(picture)->y_speed + OBJECT(picture)->screen_y);
	if (screen_y == OBJECT(picture)->profile.end_y)
		OBJECT(picture)->y_speed = 0;
	OBJECT(picture)->screen_y = screen_y;
	if ((OBJECT(picture)->x_speed | OBJECT(picture)->y_speed) == 0u)
		scene_step++;
}

/* the text typed on */
void profile_type_text(void)
{
	uint8_t sound = 0;

	type_text_in_colours();
	if (typed_passes_left == 0u)
	{
		step_timer = 0x64u;
		scene_step++;
		sound = 0xFEu;
		queue_sound(sound);
		return;
	}
	if (sound_stopped_by_coin != 0u)
	{
		sound = 0x78u;
		queue_sound(sound);
	}
}

/* the profile stays, then the next character's comes next time */
void profile_wait(void)
{
	uint8_t t1 = 0;

	t1 = step_timer;
	step_timer = t1 - 1u;
	if (t1 != 1u)
		return;
	profile_character++;
	if (profile_character >= 6u)
		profile_character = 0u;
	game_step++;
	scene = 0u;
	scene_step = 0u;
}

/* the attract mode's end */
void attract_end(void)
{
	if (new_credit != 0u)
	{
		go_to_coin_screen();
		return;
	}
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		attract_end_clear();
		return;
	case 1:
		attract_end_reset();
		return;
	case 2:
		attract_end_restart();
		return;
	}
}

/* the screen cleared */
void attract_end_clear(void)
{
	uint16_t job = 0;

	clear_play();
	request_game_step_palettes();
	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene++;
}

/* the video set back */
void attract_end_reset(void)
{
	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		layer_priority &= ~4u;
		set_screen_flip();
		reset_layers_scroll();
		scene_timer = 1u;
		scene++;
	}
}

/* the attract mode from its beginning */
void attract_end_restart(void)
{
	uint8_t t1 = 0;

	t1 = scene_timer;
	scene_timer = t1 - 1u;
	if (t1 == 1u)
	{
		game_step = 0u;
		scene = 0u;
		scene_step = 0u;
		scene_substep = 0u;
		scene_subsubstep = 0u;
	}
}

/* Unused: eight sprites copied into the sprite chip, and layer B scrolled and filled */
void copy_sprites_and_fill_layer_b(void)
{
	uint8_t tile_number = 0;
	uint16_t colour_number = 0, count = 0;
	uint32_t bytes = 0, sprite_place = 0;
	uint16_t t2 = 0;
	uint32_t t1 = 0;

	bytes = unreached_sprite_bytes_ADDRESS;
	sprite_place = SPRITE_RAM;
	count = 0x3Fu;
	do
	{
		t1 = sprite_place;
		sprite_place++;
		io_write8(t1, PICTURE_TILE(bytes));
		t2 = count - 1u;
		count = t2;
		bytes++;
	} while (t2 != 0xFFFFu);
	layer_b_scroll_x = 0x60u;
	layer_b_scroll_y = 0u;
	colour_number = 1u;
	tile_number = 1u;
	fill_layer_b(tile_number, colour_number);
}

/* the attract mode's start, straight on to the coin's screen */
void attract_begin(void)
{
	game_step++;
	scene = 0u;
	scene_step = 0u;
	scene_substep = 0u;
}

/* the coin's screen, by its steps: the boxes, the players' turtles, PRESS START */
void attract_coin_screen(void)
{
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		coin_screen_setup();
		return;
	case 1:
		coin_screen_show();
		return;
	}
}

/* the coin's screen set up, by its steps */
void coin_screen_setup(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		coin_screen_pause();
		return;
	case 1:
		coin_screen_clear();
		return;
	case 2:
		coin_screen_palettes();
		return;
	case 3:
		coin_screen_check_credits();
		return;
	}
}

/* a two-frame pause */
void coin_screen_pause(void)
{
	scene_timer = 2u;
	scene_step++;
}

/* the play's work and the layers cleared */
void coin_screen_clear(void)
{
	uint16_t job = 0;
	uint8_t t1 = 0;

	if (jobs_pending == 0u)
	{
		t1 = scene_timer;
		scene_timer = t1 - 1u;
		if (t1 == 0u)
		{
			clear_play();
			jobs_pending++;
			job = 7u;
			queue_job(job);
			scene_step++;
		}
	}
}

/* the attract mode's palettes and palette effects requested */
void coin_screen_palettes(void)
{
	if (jobs_pending == 0u)
	{
		request_game_step_palettes();
		clear_palette_effects();
		start_game_step_palette_effects();
		scene_step++;
	}
}

/* on to the coin's screen when a player has lives, else to the opening */
void coin_screen_check_credits(void)
{
	uint8_t sound = 0;
	uint16_t any_lives = 0;

	if (jobs_pending != 0u || palettes_busy != 0u)
		return;
	set_screen_flip();
	reset_layers_scroll();
	any_lives = PLAYER(PLAYER_ADDRESS(0))->lives | PLAYER(PLAYER_ADDRESS(1))->lives | PLAYER(PLAYER_ADDRESS(2))->lives | PLAYER(PLAYER_ADDRESS(3))->lives;
	any_lives = (uint16_t)(any_lives | free_play);
	any_lives = (uint16_t)(any_lives | extra_credits);
	if (any_lives == 0u)
	{
		game_step++;
		scene = 0u;
		scene_step = 0u;
		return;
	}
	sound = 0x7Eu;
	queue_sound(sound);
	scene++;
	scene_step = 0u;
}

/* the coin's screen shown, by its steps */
void coin_screen_show(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		coin_screen_boxes();
		return;
	case 1:
		coin_screen_first_wait();
		return;
	case 2:
		coin_screen_second_wait();
		return;
	case 3:
		coin_screen_draw();
		return;
	case 4:
		coin_screen_wait();
		return;
	}
}

/* the four players' boxes drawn */
void coin_screen_boxes(void)
{
	uint16_t job = 0;

	job = 0xB5Fu;
	queue_job(job);
	scene_step++;
}

/* a frame's wait */
void coin_screen_first_wait(void)
{
	scene_step++;
}

/* another frame's wait */
void coin_screen_second_wait(void)
{
	scene_step++;
}

/* the Konami texts, the pictures of the players with lives and the coin screen's music */
void coin_screen_draw(void)
{
	uint8_t sound = 0;
	uint16_t count = 0;
	uint32_t jobs = 0, player = 0, players = 0;
	uint16_t t1 = 0;

	jobs = coin_screen_jobs_ADDRESS;
	queue_jobs(jobs);
	players = 0u;
	count = 3u;
	player = PLAYER_ADDRESS(3);
	do
	{
		if (PLAYER(player)->lives != 0u)
			players |= shl32(1u, (int)(count & 0x1Fu));
		t1 = count - 1u;
		count = t1;
		player -= 8u;
	} while (t1 != 0xFFFFu);
	player_turtles(0) = 0u;
	player_turtles(1) = 1u;
	player_turtles(2) = 2u;
	player_turtles(3) = 3u;
	draw_four_turtle_pictures(players);
	scene_timer = 0xAu;
	continue_count = 0u;
	scene_step++;
	sound = 0x6Fu;
	queue_sound(sound);
}

/* the coin's screen held until a player starts, redrawn when a coin comes in */
void coin_screen_wait(void)
{
	uint8_t sound = 0;
	uint16_t job = 0;
	uint8_t t1 = 0;

	update_player_messages();
	if (new_credit != 0u)
	{
		job = (uint16_t)(0xB00u | 0x10u | players_credited);
		queue_job(job);
		scene_step--;
		return;
	}
	if (sound_stopped_by_coin != 0u)
	{
		sound = 0x6Fu;
		queue_sound(sound);
	}
	if ((frame_count & 0x3Fu) == 0u)
	{
		t1 = scene_timer;
		scene_timer = t1 - 1u;
		if (t1 == 1u)
			return;
	}
}

/* a coin in the attract mode: on to the coin's screen with no demo player left */
void go_to_coin_screen(void)
{
	scene = 0u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
	game_step = 1u;
	players_in_play = 0u;
	players_joining = 0u;
	players_knocked_out = 0u;
	players_out_of_lives = 0u;
}

/* the pictures of the turtles of the players chosen */
void draw_four_turtle_pictures(uint32_t players)
{
	uint16_t count = 0;

	count = 3u;
	draw_turtle_pictures(count, players);
}

/* the turtles' big pictures as sprites, at their players' places, in colour or in black */
void draw_turtle_pictures(uint16_t count, uint32_t players)
{
	uint16_t count_2 = 0, picture_offset = 0;
	uint32_t player_turtles_place = 0, sprite_place = 0, sprites = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	sprite_place = SPRITE_RAM;
	player_turtles_place = player_turtles_ADDRESS;
	do
	{
		if ((players & shl32(1u, (int)(count & 0x1Fu))) != 0u)
		{
			picture_offset = (uint16_t)((uint32_t)player_turtles((uint32_t)(int16_t)count) << 4);
			sprites = turtle_picture_sprites_ADDRESS;
			if ((players & 0x80u) != 0u)
				sprites = ranking_turtle_picture_sprites_ADDRESS;
			sprites += picture_offset;
			count_2 = 3u;
			do
			{
				io_write8(sprite_place + (int16_t)count_2, PICTURE_SPRITE_BYTE(sprites + (int16_t)count_2));
				io_write8(sprite_place + 8u + (int16_t)count_2, PICTURE_SPRITE_BYTE(sprites + ((int16_t)count_2 + 8u)));
				t1 = count_2 - 1u;
				count_2 = t1;
			} while (t1 != 0xFFFFu);
			picture_offset = (uint16_t)((uint32_t)(uint8_t)count << 4);
			sprites = turtle_picture_sprites_ADDRESS;
			if ((players & 0x80u) != 0u)
				sprites = ranking_turtle_picture_sprites_ADDRESS;
			sprites += picture_offset;
			count_2 = 3u;
			do
			{
				io_write8(sprite_place + 4u + (int16_t)count_2, PICTURE_SPRITE_BYTE(sprites + ((int16_t)count_2 + 4u)));
				io_write8(sprite_place + 0xCu + (int16_t)count_2, PICTURE_SPRITE_BYTE(sprites + ((int16_t)count_2 + 0xCu)));
				t2 = count_2 - 1u;
				count_2 = t2;
			} while (t2 != 0xFFFFu);
			if ((players & 0x40u) != 0u)
			{
				io_write8(sprite_place + 3u, 8u);
				io_write8(sprite_place + 0xBu, 8u);
			}
		}
		t3 = count - 1u;
		count = t3;
		sprite_place += 0x10u;
	} while (t3 != 0xFFFFu);
}

/* the layers' scroll back to zero */
void reset_layers_scroll(void)
{
	layer_a_scroll_x = 0u;
	layer_a_scroll_y = 0u;
	layer_b_scroll_x = 0u;
	layer_b_scroll_y = 0u;
	unused_layer_a_scroll_x_copy = 0u;
	unused_layer_a_scroll_y_copy = 0u;
	unused_layer_b_scroll_x_copy = 0u;
	unused_layer_b_scroll_y_copy = 0u;
	unused_scroll_reset_bytes(0) = 0u;
	unused_scroll_reset_bytes(1) = 0u;
	unused_scroll_reset_bytes(2) = 0u;
	unused_scroll_reset_bytes(3) = 0u;
	io_write8(TILES_106400, 0u);
	io_write8(TILES_106402, 0u);
	io_write8(TILES_106018, 0u);
	io_write8(TILES_106401, 0u);
	io_write8(TILES_106403, 0u);
	io_write8(TILES_106019, 0u);
}

/* the opening, by its steps, until a coin */
void attract_opening(void)
{
	uint8_t sound = 0;

	if (new_credit != 0u)
	{
		sound = 0x7Eu;
		queue_sound(sound);
		go_to_coin_screen();
		return;
	}
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		opening_setup();
		return;
	case 1:
		opening_show();
		return;
	}
}

/* the opening set up, by its steps */
void opening_setup(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		opening_setup_clear();
		return;
	case 1:
		opening_setup_palettes();
		return;
	case 2:
		opening_setup_priority();
		return;
	case 3:
		opening_setup_start();
		return;
	}
}

/* the play's work and the layers cleared */
void opening_setup_clear(void)
{
	uint16_t job = 0;

	if (jobs_pending == 0u)
	{
		clear_play();
		jobs_pending++;
		job = 7u;
		queue_job(job);
		scene_step++;
	}
}

/* the attract mode's palettes and palette effects requested */
void opening_setup_palettes(void)
{
	if (jobs_pending == 0u)
	{
		request_game_step_palettes();
		clear_palette_effects();
		start_game_step_palette_effects();
		scene_step++;
	}
}

/* the layers' priority set */
void opening_setup_priority(void)
{
	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		layer_priority = 0u;
		io_write8(PRIORITY_0C0001, layer_priority);
		scene_step++;
	}
}

/* the opening started */
void opening_setup_start(void)
{
	if (jobs_pending == 0u)
	{
		scene++;
		scene_step = 0u;
		attract_timer = 0xB2u;
		hide_all_sprites();
		start_opening();
		io_write8(TILES_106B00, 0u);
		tile_banks_0_1 = 0u;
		io_write8(TILES_106E00, 0u);
		tile_banks_2_3 = 0u;
	}
}

/* the opening played for its time, then the title */
void opening_show(void)
{
	uint32_t ignored = 0;
	uint8_t t1 = 0;

	if ((frame_count & 3u) == 0u)
	{
		t1 = attract_timer;
		attract_timer = t1 - 1u;
		if (t1 == 1u)
		{
			game_step++;
			scene = 0u;
			scene_step = 0u;
			return;
		}
	}
	run_opening();
	build_draw_list();
	draw_sprites(ignored);
}

/* BEST 10 PLAYERS, by its steps, until a coin */
void attract_best_players(void)
{
	if (new_credit != 0u)
	{
		go_to_coin_screen();
		return;
	}
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		best_players_setup();
		return;
	case 1:
		best_players_show();
		return;
	}
}

/* BEST 10 PLAYERS set up, by its steps */
void best_players_setup(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		best_players_clear();
		return;
	case 1:
		best_players_palettes();
		return;
	case 2:
		best_players_draw();
		return;
	}
}

/* the play's work and the layers cleared */
void best_players_clear(void)
{
	uint16_t job = 0;

	if (jobs_pending == 0u)
	{
		clear_play();
		jobs_pending++;
		job = 7u;
		queue_job(job);
		scene_step++;
	}
}

/* the attract mode's palettes and palette effects requested */
void best_players_palettes(void)
{
	if (jobs_pending == 0u)
	{
		request_game_step_palettes();
		clear_palette_effects();
		start_game_step_palette_effects();
		scene_step++;
	}
}

/* the background, the turtles' pictures and the table of the best ten drawn */
void best_players_draw(void)
{
	uint16_t count = 0, kind = 0;
	uint32_t data = 0, loop_end = 0;

	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		data = best_players_palette_ADDRESS;
		kind = 0x500u;
		post_palette_request(kind, data);
		tile_banks_0_1 = 0u;
		io_write8(TILES_106B00, 0u);
		tile_banks_2_3 = 0u;
		io_write8(TILES_106E00, 0u);
		layer_b_scroll_x = 0x60u;
		layer_b_scroll_y = 0u;
		loop_end = draw_ranking_background();
		count = 4u;
		loop_end = (loop_end & ~0xFFu) | 0x8Fu;
		player_turtles(0) = 0u;
		player_turtles(1) = 1u;
		player_turtles(2) = 2u;
		player_turtles(3) = 3u;
		draw_turtle_pictures(count, loop_end);
		draw_best_players();
		scene++;
		scene_step = 0u;
	}
}

/* BEST 10 PLAYERS shown, by its steps */
void best_players_show(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		best_players_pause();
		return;
	case 1:
		best_players_scroll();
		return;
	}
}

/* a pause before the turtles move */
void best_players_pause(void)
{
	if (jobs_pending == 0u)
	{
		scene_step++;
		step_timer = 0xFFu;
	}
}

/* the turtles' pictures slide across behind the table, then the next attract step */
void best_players_scroll(void)
{
	uint16_t count = 0;
	uint32_t sprite_place = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;
	uint16_t t4 = 0;

	t1 = step_timer;
	step_timer = t1 - 1u;
	if (t1 == 1u)
	{
		game_step++;
		scene = 0u;
		scene_step = 0u;
		return;
	}
	sprite_place = SPRITE_RAM;
	count = 7u;
	for (;;)
	{
		if ((io_read8(sprite_place) & 0x80u) == 0u)
		{
			sprite_place += 8u;
			continue;
		}
		t2 = io_read8(sprite_place + 6u);
		turtles_in_black = t2;
		(void)t2;
		t3 = io_read8(sprite_place + 7u);
		ranking_time = (ranking_time & 0xFF00u) | (t3 & 0xFFu);
		(void)t3;
		ranking_time &= 0x1FFu;
		ranking_time += 4u;
		ranking_time &= 0x1FFu;
		io_write8(sprite_place + 6u, turtles_in_black);
		io_write8(sprite_place + 7u, (uint8_t)ranking_time);
		t4 = count - 1u;
		count = t4;
		sprite_place += 8u;
		if (t4 != 0xFFFFu)
			continue;
		break;
	}
}

/* the title, the places, the initials and the scores of the best ten */
void draw_best_players(void)
{
	uint32_t jobs = 0;

	jobs = best_players_jobs_ADDRESS;
	queue_jobs(jobs);
	draw_best_players_initials();
	draw_best_players_scores();
}

/* the ten players' initials, the first three in their colours */
void draw_best_players_initials(void)
{
	uint8_t colour = 0;
	uint16_t character = 0, place = 0;
	uint32_t font_tiles = 0, initials = 0, tile_place = 0;

	tile_place = best_players_places[0];
	initials = ranking_initials_ADDRESS;
	font_tiles = big_font_tiles_table_ADDRESS;
	place = 0u;
	do
	{
		colour = best_players_colours[place];
		io_write8(tile_place, colour);
		io_write8(tile_place + 4u, colour);
		io_write8(tile_place + 8u, colour);
		io_write8(tile_place + 0x80u, colour);
		io_write8(tile_place + 0x84u, colour);
		io_write8(tile_place + 0x88u, colour);
		character = RANKING_NAME(initials)->first;
		draw_best_players_character(character, font_tiles, tile_place);
		character = RANKING_NAME(initials)->second;
		tile_place += 4u;
		draw_best_players_character(character, font_tiles, tile_place);
		character = RANKING_NAME(initials)->third;
		tile_place += 4u;
		draw_best_players_character(character, font_tiles, tile_place);
		place = (uint8_t)(place + 1u);
		tile_place += 0xF8u;
		initials += 4u;
	} while (place < 0xAu);
}

/* a tall character's tiles */
void draw_best_players_character(uint16_t character, uint32_t font_tiles, uint32_t tile_place)
{
	character = (uint16_t)((uint32_t)(uint8_t)(character - 0x30u) << 1);
	io_write8(tile_place + 1u, FONT_TILE(font_tiles + character));
	io_write8(tile_place + 0x81u, FONT_TILE(font_tiles + ((int32_t)(character + 1u))));
}

/* the ten scores with PTS */
void draw_best_players_scores(void)
{
	uint8_t colour = 0;
	uint16_t digit_offset = 0, place = 0, score_digits = 0;
	uint32_t score = 0, score_place = 0, tile_place = 0;

	score = ranking_scores_ADDRESS;
	place = 0u;
	do
	{
		tile_place = best_players_places[1] + place * 0x100u;
		score_place = tile_place;
		score_digits = RANKING_SCORE(score)->score;
		while (score_digits != 0u)
		{
			colour = best_players_colours[place];
			io_write8(tile_place, colour);
			io_write8(tile_place + 0x80u, colour);
			digit_offset = (uint8_t)score_digits;
			digit_offset = (uint16_t)((digit_offset & 0xFu) << 1);
			io_write8(tile_place + 1u, big_font_tiles_table[digit_offset]);
			io_write8(tile_place + 0x81u, big_font_tiles_table[1 + digit_offset]);
			score_digits = score_digits >> 4;
			tile_place -= 4u;
		}
		colour = best_players_colours[place];
		io_write8(score_place + 4u, colour);
		io_write8(score_place + 6u, colour);
		io_write8(score_place + 8u, colour);
		io_write8(score_place + 0x84u, colour);
		io_write8(score_place + 0x86u, colour);
		io_write8(score_place + 0x88u, colour);
		io_write8(score_place + 5u, big_font_tiles_table[64]);
		io_write8(score_place + 0x85u, big_font_tiles_table[65]);
		io_write8(score_place + 7u, big_font_tiles_table[72]);
		io_write8(score_place + 0x87u, big_font_tiles_table[73]);
		io_write8(score_place + 9u, big_font_tiles_table[70]);
		io_write8(score_place + 0x89u, big_font_tiles_table[71]);
		place = (uint8_t)(place + 1u);
		score += 2u;
	} while (place < 0xAu);
}

/* an empty attract step */
void attract_spare_step(void)
{
	game_step++;
}
