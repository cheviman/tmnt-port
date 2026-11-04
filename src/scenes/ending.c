/**
 *    Name:        scenes/ending.c
 *    Description: the ending after the last boss: the Technodrome explodes, the epilogue, the staff roll, GAME OVER
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the ending after the last boss, by its steps */
void scene_ending(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		ending_setup();
		return;
	case 1:
		ending_epilogue();
		return;
	}
}

/* the ending's first step, by its steps */
void ending_setup(void)
{
	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		ending_clear();
		return;
	case 1:
		ending_show_technodrome();
		return;
	case 2:
		ending_open_box();
		return;
	}
}

/* the players taken out, the screen and the objects cleared, the ending's palettes requested */
void ending_clear(void)
{
	uint16_t job = 0, kind = 0;
	uint32_t data = 0;

	hide_press_start = 0u;
	drop_players_in_play();
	jobs_pending++;
	job = 7u;
	queue_job(job);
	data = 0xA03Eu;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	clear_palette_effects();
	clear_objects();
	clear_sprite_ram();
	scene_substep++;
}

/* the Technodrome's picture in a box that starts to open */
void ending_show_technodrome(void)
{
	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		start_box_opening();
		draw_technodrome_screen();
		hide_all_sprites();
		scene_substep++;
		layer_b_scroll_x = 0x20u;
	}
}

/* the box opened, then a pause */
void ending_open_box(void)
{
	grow_tile_box();
	if (box_frames_left == 0u)
	{
		game_scene_timer = 0x28u;
		scene_step++;
		scene_substep = 0u;
	}
}

/* the explosion, the epilogue, the staff roll and GAME OVER, by their steps */
void ending_epilogue(void)
{
	uint32_t ignored = 0;

	switch ((uint32_t)(int8_t)scene_substep)
	{
	case 0:
		ending_pause();
		break;
	case 1:
		ending_technodrome_glows();
		break;
	case 2:
		ending_technodrome_explodes();
		break;
	case 3:
		ending_show_wreck();
		break;
	case 4:
		ending_fade_to_wreck();
		break;
	case 5:
		ending_start_epilogue();
		break;
	case 6:
		ending_type_epilogue();
		break;
	case 7:
		ending_dim_epilogue();
		break;
	case 8:
		ending_staff_roll();
		break;
	case 9:
		ending_game_over();
		break;
	}
	build_draw_list();
	draw_sprites(ignored);
}

/* a pause, then a sound */
void ending_pause(void)
{
	uint8_t sound = 0;
	uint8_t t1 = 0;

	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
	{
		sound = 0x22u;
		queue_sound(sound);
		fade_state = 0u;
		game_scene_timer = 0x78u;
		scene_substep++;
	}
}

/* the Technodrome turns red */
void ending_technodrome_glows(void)
{
	uint8_t t1 = 0;

	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
	{
		fade_state = 0u;
		game_scene_timer = 0x96u;
		scene_substep++;
		return;
	}
	if (fading == 0u)
	{
		fade_banks = ending_palette_fades[0].banks;
		palette_place = ending_palette_fades[0].palettes;
		fade_state = 0u;
	}
	copy_palette_sets();
}

/* explosions over the Technodrome, the screen shaking, the colours going to white */
void ending_technodrome_explodes(void)
{
	uint8_t sound = 0;
	uint16_t sound_choice = 0;
	uint32_t object = 0, objects_left = 0, palette_fade = 0;
	uint8_t t1 = 0;

	remove_spent_explosions();
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
	{
		fade_state = 0u;
		scene_substep++;
		return;
	}
	if (game_scene_timer >= 0x32u)
	{
		if (game_scene_timer == 0x32u)
		{
			sound = 0x1Au;
			queue_sound(sound);
			object = OBJECT_ADDRESS(12);
			objects_left = 0xCu;
			find_free_object(&objects_left, &object);
			if ((uint8_t)objects_left != 0u)
				start_technodrome_explosion(object);
			palette_fade = ending_palette_fades_ADDRESS + 0x6u;
			if (game_scene_timer < 0x78u)
				palette_fade = ending_palette_fades_ADDRESS + 0xCu;
			if (fading == 0u)
			{
				fade_banks = PALETTE_FADE(palette_fade)->banks;
				palette_place = PALETTE_FADE(palette_fade)->palettes;
				fade_state = 0u;
			}
			copy_palette_sets();
			return;
		}
		shake_technodrome();
		if ((game_scene_timer & 7u) == 0u)
		{
			sound_choice = (uint8_t)frame_count;
			sound_choice = sound_choice & 0x18u;
			sound = technodrome_explosion_sounds[sound_choice >> 3];
			queue_sound(sound);
			object = OBJECT_ADDRESS(12);
			objects_left = 0xCu;
			find_free_object(&objects_left, &object);
			if ((uint8_t)objects_left != 0u)
				start_technodrome_explosion(object);
		}
	}
	palette_fade = ending_palette_fades_ADDRESS + 0x6u;
	if (game_scene_timer < 0x78u)
		palette_fade = ending_palette_fades_ADDRESS + 0xCu;
	if (fading == 0u)
	{
		fade_banks = PALETTE_FADE(palette_fade)->banks;
		palette_place = PALETTE_FADE(palette_fade)->palettes;
		fade_state = 0u;
	}
	copy_palette_sets();
}

/* the Technodrome's wreck drawn */
void ending_show_wreck(void)
{
	clear_technodrome_top();
	draw_technodrome_wreck();
	fade_state = 0u;
	game_scene_timer = 0xB4u;
	scene_substep++;
}

/* the colours back from white to the wreck, then the epilogue's music */
void ending_fade_to_wreck(void)
{
	uint8_t sound = 0;
	uint8_t t1 = 0;

	remove_spent_explosions();
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 != 0u)
	{
		if (fading == 0u)
		{
			fade_banks = ending_palette_fades[3].banks;
			palette_place = ending_palette_fades[3].palettes;
			fade_state = 0u;
		}
		copy_palette_sets();
		return;
	}
	fade_state = 0u;
	game_scene_timer = 0x1Eu;
	sound = 0x6Cu;
	queue_sound(sound);
	scene_substep++;
}

/* the epilogue's text started */
void ending_start_epilogue(void)
{
	uint32_t carry = 0, extend = 0;
	uint8_t delay_shift = 0;
	uint32_t text = 0;
	uint8_t t1 = 0;

	t1 = game_scene_timer;
	carry = t1 == 0u;
	extend = carry;
	game_scene_timer = t1 - 1u;
	if (t1 == 1u)
	{
		text = epilogue_typed_text_ADDRESS;
		delay_shift = 2u;
		start_text_typed_in_colours(delay_shift, text, extend);
		scene_substep++;
	}
}

/* the epilogue typed until it is done */
void ending_type_epilogue(void)
{
	type_text_in_colours();
	if (typed_passes_left == 0u)
	{
		fade_state = 0u;
		game_scene_timer = 0xB4u;
		scene_substep++;
	}
}

/* the picture darkened before the staff roll */
void ending_dim_epilogue(void)
{
	uint8_t t1 = 0;

	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
	{
		fade_state = 0u;
		game_scene_timer = 0xB4u;
		scene_substep++;
		scene_subsubstep = 0u;
		return;
	}
	if (fading == 0u)
	{
		fade_banks = ending_palette_fades[4].banks;
		palette_place = ending_palette_fades[4].palettes;
		fade_state = 0u;
	}
	copy_palette_sets();
}

/* the staff roll, by its steps */
void ending_staff_roll(void)
{
	switch ((uint32_t)(int8_t)scene_subsubstep)
	{
	case 0:
		staff_roll_clear();
		return;
	case 1:
		staff_roll_start();
		return;
	case 2:
		staff_roll_next_credit();
		return;
	case 3:
		staff_roll_type_name();
		return;
	case 4:
		staff_roll_hold_name();
		return;
	case 5:
		staff_roll_erase_name();
		return;
	case 6:
		staff_roll_end();
		return;
	}
}

/* the screen cleared and the staff roll's palettes requested */
void staff_roll_clear(void)
{
	uint16_t job = 0, kind = 0;
	uint32_t data = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	data = 0xA0DEu;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	scene_subsubstep++;
}

/* the city, the turtles' portraits and the music for the staff roll */
void staff_roll_start(void)
{
	uint8_t sound = 0;

	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		io_write8(TILES_106B00, 0xF0u);
		tile_banks_0_1 = 0xF0u;
		io_write8(TILES_106E00, 7u);
		tile_banks_2_3 = 7u;
		layer_b_scroll_x = 0x48u;
		draw_staff_roll_city();
		start_staff_roll_portraits();
		scene_subsubstep++;
		staff_roll_credit = 0u;
		game_scene_timer = 0xB4u;
		sound = 0x70u;
		queue_sound(sound);
	}
}

/* the next credit's title and name, or PRODUCED BY and the Konami logo after the last */
void staff_roll_next_credit(void)
{
	uint8_t sound = 0;
	uint16_t count = 0;
	uint32_t credit_number = 0, jobs = 0, text = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	slide_staff_roll_portraits();
	if (staff_roll_credit >= 0xEu)
	{
		tile_place = TILE_RAM + 0x324u;
		text = staff_roll_blank_text_ADDRESS;
		count = 0xFu;
		do
		{
			io_write8(tile_place, 0u);
			tile_place++;
			io_write8(tile_place, TEXT(text) - 0x30u);
			t1 = count - 1u;
			count = t1;
			tile_place++;
			text++;
		} while (t1 != 0xFFFFu);
		jobs = table_loaded_by_lea_007018_ADDRESS;
		queue_jobs(jobs);
		game_scene_timer = 0xB4u;
		scene_subsubstep = 6u;
		sound = 0xFEu;
		queue_sound(sound);
		return;
	}
	tile_place = TILE_RAM + 0x324u;
	credit_number = staff_roll_credit;
	text = staff_roll_credits_ADDRESS + (uint16_t)(credit_number << 4);
	count = 0xFu;
	do
	{
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, TEXT(text) - 0x30u);
		t2 = count - 1u;
		count = t2;
		tile_place++;
		text++;
	} while (t2 != 0xFFFFu);
	start_credit_name();
	scene_subsubstep++;
}

/* the credit's name typed in colours */
void staff_roll_type_name(void)
{
	slide_staff_roll_portraits();
	staff_roll_skip();
	type_credit_name();
	if (typed_passes_left == 0u)
	{
		game_scene_timer = 0x14u;
		scene_subsubstep++;
	}
}

/* the name held a moment, then its erasing started */
void staff_roll_hold_name(void)
{
	uint8_t t1 = 0;

	slide_staff_roll_portraits();
	staff_roll_skip();
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
	{
		scene_subsubstep++;
		start_credit_name_erase();
	}
}

/* the name erased, then the next credit */
void staff_roll_erase_name(void)
{
	slide_staff_roll_portraits();
	staff_roll_skip();
	erase_credit_name();
	if (typed_passes_left == 0u)
	{
		staff_roll_credit++;
		scene_subsubstep -= 3u;
	}
}

/* the last pause of the staff roll */
void staff_roll_end(void)
{
	uint8_t t1 = 0;

	slide_staff_roll_portraits();
	staff_roll_skip();
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 0u)
	{
		scene_substep++;
		scene_subsubstep = 0u;
		return;
	}
}

/* the GAME OVER after the ending, by its steps */
void ending_game_over(void)
{
	switch ((uint32_t)(int8_t)scene_subsubstep)
	{
	case 0:
		ending_game_over_clear();
		return;
	case 1:
		ending_game_over_start();
		return;
	case 2:
		ending_game_over_wait();
		return;
	}
}

/* the screen and the objects cleared for GAME OVER */
void ending_game_over_clear(void)
{
	uint16_t job = 0, kind = 0;
	uint32_t data = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	data = 0xA01Eu;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	clear_objects();
	clear_sprite_ram();
	scene_subsubstep++;
}

/* the GAME OVER box with its colours and music */
void ending_game_over_start(void)
{
	uint8_t sound = 0;
	uint16_t text_number = 0;
	uint32_t palette_effect_pointer = 0;

	if (jobs_pending != 0u || palettes_busy != 0u)
		return;
	text_number = 1u;
	print_colour_text(text_number);
	palette_effect_pointer = 0xD2C4u;
	start_palette_effects(palette_effect_pointer);
	palette_effect_pointer = 0xD2E6u;
	start_palette_effects(palette_effect_pointer);
	game_scene_timer = 0xFAu;
	scene_subsubstep++;
	sound = 0x6Du;
	queue_sound(sound);
}

/* GAME OVER held until the time runs out or a button, then the ranking */
void ending_game_over_wait(void)
{
	uint8_t t1 = 0;

	ending_game_over_skip();
	t1 = game_scene_timer;
	game_scene_timer = t1 - 1u;
	if (t1 == 1u)
	{
		scene = 8u;
		scene_step = 0u;
		scene_substep = 0u;
		scene_subsubstep = 0u;
		return;
	}
}

/* the players in play leave the game when it ends */
void drop_players_in_play(void)
{
	uint16_t count = 0;
	uint32_t player = 0;
	uint16_t t1 = 0;

	count = 3u;
	player = PLAYER_ADDRESS(3);
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_joining & shl32(1u, (int)(count & 7u))) == 0u)
		{
			players_joining &= ~shl32(1u, (int)(count & 7u));
			players_in_play &= ~shl32(1u, (int)(count & 7u));
			PLAYER(player)->lives = 0u;
			PLAYER(player)->health_bars = 0u;
			PLAYER(player)->message = 0u;
		}
		t1 = count - 1u;
		count = t1;
		player -= 8u;
	} while (t1 != 0xFFFFu);
	players_knocked_out = 0u;
	players_out_of_lives = 0u;
}

/* the screen shaken while the Technodrome explodes */
void shake_technodrome(void)
{
	layer_b_scroll_y = (layer_b_scroll_y & 0xFF00u) | (0u - ((((uint8_t)frame_count & 2u) << 1) + LAYER(LAYER_B)->y_pixel) & 0xFFu);
}

/* the top of the Technodrome taken off the picture */
void clear_technodrome_top(void)
{
	uint16_t count = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0;

	count = 0x1FFu;
	tile_place = TILE_RAM + 0x2200u;
	do
	{
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, 0x10u);
		t1 = count - 1u;
		count = t1;
		tile_place++;
	} while (t1 != 0xFFFFu);
}

/* the Technodrome's wreck drawn in tiles */
void draw_technodrome_wreck(void)
{
	uint8_t tile = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0, count_3 = 0;
	uint32_t part = 0, row_place = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	count = 1u;
	part = technodrome_wreck_parts_ADDRESS;
	do
	{
		row_place = WRECK_PART(part)->place;
		tile = WRECK_PART(part)->tile;
		count_2 = WRECK_PART(part)->colour;
		tile_code = 0u;
		do
		{
			tile_place = row_place;
			count_3 = WRECK_PART(part)->rows;
			tile_code &= ~0xFu;
			do
			{
				io_write8(tile_place, tile);
				tile_place++;
				io_write8(tile_place, tile_code);
				tile_code++;
				t1 = count_3 - 1u;
				count_3 = t1;
				tile_place++;
			} while (t1 != 0xFFFFu);
			tile_code = (uint8_t)(tile_code - 1u + 0x10u);
			t2 = count_2 - 1u;
			count_2 = t2;
			row_place += 0x80u;
		} while (t2 != 0xFFFFu);
		t3 = count - 1u;
		count = t3;
		part += 8u;
	} while (t3 != 0xFFFFu);
}

/* an explosion started at one of the Technodrome's places */
void start_technodrome_explosion(uint32_t object)
{
	uint16_t explosion_count = 0;
	uint32_t animation = 0, object_2 = 0;

	explosion_count = technodrome_explosions;
	explosion_count = (uint16_t)((explosion_count & 7u) << 2);
	OBJECT(object)->screen_x = technodrome_explosion_x[explosion_count / 2u];
	OBJECT(object)->screen_x += 0x100u;
	OBJECT(object)->screen_y = technodrome_explosion_y[explosion_count / 2u];
	OBJECT(object)->screen_y += 0x60u;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	OBJECT(object)->kind = 0u;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->colour = 0xFFu;
	OBJECT(object)->animation_delay = 0u;
	OBJECT(object)->count = 0u;
	OBJECT(object)->routine = 0u;
	OBJECT(object)->state = 0u;
	object_2 = object;
	animation = explosion_big_animations[0];
	set_animation(0u, object_2, animation);
	technodrome_explosions++;
	palette_place = technodrome_explosion_x_ADDRESS;
}

/* the explosions whose animation is over removed */
void remove_spent_explosions(void)
{
	uint16_t count = 0;
	uint32_t object = 0;
	uint16_t t1 = 0;

	object = OBJECT_ADDRESS(12);
	count = 0xBu;
	do
	{
		if (OBJECT(object)->flags != 0u && OBJECT(object)->frame_time == 0u)
			remove_object(object);
		t1 = count - 1u;
		count = t1;
		object += 0x50u;
	} while (t1 != 0xFFFFu);
}

/* the four turtles' portraits put in for the staff roll */
void start_staff_roll_portraits(void)
{
	uint16_t portrait_offset = 0;
	uint32_t animation = 0, count = 0, ignored = 0, object = 0, portrait = 0;
	uint16_t t1 = 0;

	count = 3u;
	object = OBJECT_ADDRESS(12);
	do
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->count = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		portrait_offset = (uint16_t)(count << 2);
		OBJECT(object)->screen_x = TABLE16(staff_roll_portrait_places, (uint32_t)(int16_t)portrait_offset);
		OBJECT(object)->screen_y = TABLE16(staff_roll_portrait_places, 0x2u + (uint32_t)(int16_t)portrait_offset);
		portrait = object;
		animation = staff_roll_portrait_animations[count & 0x3FFFu];
		set_animation(ignored, portrait, animation);
		t1 = count - 1u;
		count = t1;
		object = portrait + 0x50u;
	} while (t1 != 0xFFFFu);
}

/* the portraits slide to the left round the screen */
void slide_staff_roll_portraits(void)
{
	uint16_t count = 0, screen_x = 0;
	uint32_t object = 0;
	uint16_t screen_x_2 = 0, t1 = 0;

	count = 3u;
	object = OBJECT_ADDRESS(12);
	do
	{
		screen_x = frame_count & 1u;
		screen_x_2 = OBJECT(object)->screen_x;
		OBJECT(object)->screen_x = screen_x_2 - screen_x;
		if (screen_x_2 < screen_x)
			OBJECT(object)->screen_x = 0x300u;
		t1 = count - 1u;
		count = t1;
		object += 0x50u;
	} while (t1 != 0xFFFFu);
}

/* a button ends the staff roll after its first credit */
void staff_roll_skip(void)
{
	uint8_t sound = 0;

	if (staff_roll_credit == 0u || ((player_presses(0) | player_presses(1) | player_presses(2) | player_presses(3)) & 0x30u) == 0u)
		return;
	sound = 0xFEu;
	queue_sound(sound);
	scene_substep++;
	scene_subsubstep = 0u;
}

/* the city drawn behind the staff roll */
void draw_staff_roll_city(void)
{
	uint32_t carry = 0;
	uint8_t tile_attribute = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = TILE_RAM + 0x4018u;
	tile_attribute = 0x14u;
	tile_code = 0u;
	count = 0x1Fu;
	do
	{
		count_2 = 0x1Fu;
		do
		{
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, tile_code);
			carry = (uint8_t)(tile_code + 1u) < tile_code;
			tile_code++;
			tile_place++;
			if (carry)
				tile_attribute++;
			t1 = count_2 - 1u;
			count_2 = t1;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x40u;
	} while (t2 != 0xFFFFu);
}

/* the credit's name started in seven colour passes */
void start_credit_name(void)
{
	uint16_t count = 0;
	uint32_t typed_pass = 0;
	uint16_t t1 = 0;

	typed_pass = TYPED_PASS_ADDRESS(7);
	count = 6u;
	do
	{
		set_credit_name_pass(typed_pass);
		TYPED_PASS(typed_pass)->wait = (uint32_t)count << 3;
		t1 = count - 1u;
		count = t1;
		typed_pass -= 0xAu;
	} while (t1 != 0xFFFFu);
	typed_passes_left = 7u;
}

/* the credit's name recoloured and then blanked in eight passes */
void start_credit_name_erase(void)
{
	uint16_t count = 0;
	uint32_t typed_pass = 0;
	uint16_t t1 = 0;

	typed_pass = TYPED_PASS_ADDRESS(7);
	count = 7u;
	do
	{
		set_credit_name_pass(typed_pass);
		TYPED_PASS(typed_pass)->wait = (8u - count) << 3;
		t1 = count - 1u;
		count = t1;
		typed_pass -= 0xAu;
	} while (t1 != 0xFFFFu);
	typed_passes_left = 8u;
	TYPED_PASS(TYPED_PASS_ADDRESS(0))->text = 0x77D2u;
}

/* a pass of the credit's name set at its place, speed and text */
void set_credit_name_pass(uint32_t typed_pass)
{
	uint32_t credit_number = 0;

	TYPED_PASS(typed_pass)->place = TILE_RAM + 0x42Au;
	TYPED_PASS(typed_pass)->delay = 4u;
	credit_number = staff_roll_credit;
	TYPED_PASS(typed_pass)->text = staff_roll_names_ADDRESS + (uint16_t)(credit_number << 4);
}

/* the seven passes of the name typed on */
void type_credit_name(void)
{
	uint16_t count = 0;
	uint32_t typed_pass = 0;
	uint16_t t1 = 0;

	typed_pass = TYPED_PASS_ADDRESS(7);
	count = 6u;
	do
	{
		type_credit_name_pass(count, typed_pass);
		t1 = count - 1u;
		count = t1;
		typed_pass -= 0xAu;
	} while (t1 != 0xFFFFu);
}

/* the eight passes that take the name off typed on */
void erase_credit_name(void)
{
	uint16_t colour_number = 0, count = 0;
	uint32_t kept_pass = 0, typed_pass = 0;
	uint16_t t1 = 0;

	typed_pass = TYPED_PASS_ADDRESS(7);
	count = 7u;
	do
	{
		kept_pass = typed_pass;
		colour_number = (uint16_t)count;
		if (count != 0u)
			colour_number = (uint16_t)((colour_number & ~0xFFu) | (uint8_t)(colour_number - 1u));
		type_credit_name_pass(colour_number, typed_pass);
		t1 = count - 1u;
		count = t1;
		typed_pass = kept_pass - 0xAu;
	} while (t1 != 0xFFFFu);
}

/* the next big character of a pass in its colour */
void type_credit_name_pass(uint16_t colour_number, uint32_t typed_pass)
{
	uint8_t colour = 0;
	uint16_t character = 0;
	uint32_t place = 0;
	uint8_t wait = 0;

	if (TYPED_PASS(typed_pass)->text == 0u)
		return;
	wait = TYPED_PASS(typed_pass)->wait;
	TYPED_PASS(typed_pass)->wait = wait - 1u;
	if (wait != 0u)
		return;
	place = TYPED_PASS(typed_pass)->place;
	character = TEXT(TYPED_PASS(typed_pass)->text);
	if (character == 0x21u)
	{
		TYPED_PASS(typed_pass)->place = 0u;
		TYPED_PASS(typed_pass)->text = 0u;
		TYPED_PASS(typed_pass)->wait = 0u;
		TYPED_PASS(typed_pass)->delay = 0u;
		typed_passes_left--;
		return;
	}
	colour = TABLE8(text_colours, 0x1u + (uint32_t)(int16_t)colour_number);
	if (character != 0x40u)
		colour += 0x18u;
	io_write8(place, colour);
	io_write8(place + 2u, colour);
	io_write8(place + 0x80u, colour);
	io_write8(place + 0x82u, colour);
	character = (uint16_t)((uint32_t)(uint8_t)(character - 0x40u) << 2);
	io_write8(place + 1u, credit_name_tiles_1[character]);
	io_write8(place + 3u, credit_name_tiles_2[character]);
	io_write8(place + 0x81u, credit_name_tiles_3[character]);
	io_write8(place + 0x83u, credit_name_tiles_4[character]);
	TYPED_PASS(typed_pass)->text++;
	TYPED_PASS(typed_pass)->place += 4u;
	TYPED_PASS(typed_pass)->wait = TYPED_PASS(typed_pass)->delay;
}

/* a button ends GAME OVER early */
void ending_game_over_skip(void)
{
	uint8_t sound = 0;

	if (game_scene_timer >= 0xF0u || ((player_presses(0) | player_presses(1) | player_presses(2) | player_presses(3)) & 0x30u) == 0u)
		return;
	sound = 0u;
	queue_sound(sound);
	scene = 8u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
}
