/**
 *    Name:        objects/stockman.c
 *    Description: Baxter Stockman (area 3) and the Mousers he drops
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Stockman comes in, by the state of his way in */
void stockman_enter(uint16_t on_right, uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		stockman_start(object);
		return;
	case 1:
		stockman_wait_for_last_enemy(object);
		return;
	case 2:
		stockman_wait(object);
		return;
	case 3:
		stockman_fly_in(on_right, object);
		return;
	}
}

/* Stockman waits in his flying machine */
void stockman_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->shadow = stockman_shadow_ADDRESS;
	stats = stockman_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->stockman.time_left = (OBJECT(object)->stockman.time_left & 0xFFu) | ((OBJECT(object)->health_bars & 0xFFu) << 8u);
	animation = stockman_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	mousers_break_down = 0u;
	OBJECT(object)->stockman.explosions = 0u;
	OBJECT(object)->state++;
	enemies_on_screen++;
}

/* once he is the last enemy the music fades */
void stockman_wait_for_last_enemy(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t kind = 0;
	uint32_t data = 0;

	if (enemies_on_screen == 1u)
	{
		sound = 0xFEu;
		music = 0xFEu;
		queue_sound(sound);
		data = stockman_palette_ADDRESS;
		kind = 0x43Cu;
		post_palette_request(kind, data);
		OBJECT(object)->stockman.timer = 0x64u;
		OBJECT(object)->state++;
	}
}

/* then the boss music, and he flies in */
void stockman_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint8_t timer = 0;

	if (palettes_busy == 0u)
	{
		timer = OBJECT(object)->stockman.timer;
		OBJECT(object)->stockman.timer = timer - 1u;
		if (timer == 1u)
		{
			sound = 0x33u;
			queue_sound(sound);
			sound = 0x64u;
			music = 0x64u;
			queue_sound(sound);
			sound = 0xBFu;
			queue_sound(sound);
			OBJECT(object)->x_speed = -0x300;
			OBJECT(object)->y_speed = 0;
			OBJECT(object)->height_speed = 0x100;
			OBJECT(object)->state++;
		}
	}
}

/* he flies in, picks a turtle and says "Yippee-I-Ay!" */
void stockman_fly_in(uint32_t on_right, uint32_t object)
{
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t picked = 0;

	stockman_set_floor(object);
	move_by_speeds(object);
	if (OBJECT(object)->x >= 0x47000u)
		return;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->stockman.heading = 0x800u;
	OBJECT(object)->stockman.turning = 1u;
	OBJECT(object)->stockman.turn_speed = 0x20u;
	OBJECT(object)->stockman.bob = 0u;
	OBJECT(object)->stockman.target = 0u;
	OBJECT(object)->stockman.time_left = 0xE10u;
	boss_pick_target(object, &on_right, &picked);
	if ((uint8_t)picked == 0u)
		OBJECT(object)->stockman.target = 0u;
	stockman_random_drop_time(object);
	on_right = on_right | 0xFFu;
	screen_x = OBJECT(object)->screen_x;
	screen_y = (uint16_t)(OBJECT(object)->screen_y + 0x20u);
	say_yippee_i_ay(on_right, screen_x, screen_y);
	OBJECT(object)->height = 0x2C00u;
	OBJECT(object)->routine++;
	OBJECT(object)->state = 0u;
}

/* he weaves at his turtle and now and then drops a Mouser; after a while he rises out of sight */
void stockman_fly(uint32_t object)
{
	uint32_t animation = 0, highest_player = 0, mouser = 0, objects_left = 0;
	uint8_t timer = 0;

	stockman_set_floor(object);
	if (OBJECT(object)->stockman.time_left == 0u)
	{
		stockman_steer(object);
		OBJECT(object)->height_whole++;
		if (OBJECT(object)->height_whole < 0xE0u)
			return;
		enemies_on_screen--;
		remove_object(object);
		return;
	}
	OBJECT(object)->stockman.time_left--;
	stockman_bob(object);
	stockman_steer(object);
	timer = OBJECT(object)->stockman.timer;
	OBJECT(object)->stockman.timer = timer - 1u;
	if (timer != 0u)
		return;
	highest_player = highest_player_in_play();
	if (enemies_on_screen <= stockman_fly_moves[highest_player])
	{
		mouser = OBJECT_ADDRESS(12);
		objects_left = 0xCu;
		find_free_object(&objects_left, &mouser);
		if ((uint8_t)objects_left != 0u)
		{
			stockman_start_mouser(object, mouser);
			animation = stockman_animations[1];
			set_animation(0u, object, animation);
			OBJECT(object)->routine++;
			return;
		}
	}
	stockman_random_drop_time(object);
}

/* his claws open as the Mouser drops */
void stockman_drop_mouser(uint32_t object)
{
	uint32_t animation = 0;

	stockman_set_floor(object);
	stockman_bob(object);
	stockman_steer(object);
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine--;
		animation = stockman_animations[0];
		set_animation(0u, object, animation);
		stockman_random_drop_time(object);
		OBJECT(object)->stockman.unused_cleared = 0u;
	}
}

/* Unused: nothing */
void stockman_idle(void)
{
}

/* a hit makes him shake, or the last one defeats him */
void stockman_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	stockman_set_floor(object);
	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->stockman.explosions = 0u;
		OBJECT(object)->flags &= ~6u;
		animation = stockman_animations[3];
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->stockman.timer = 0x1Eu;
	OBJECT(object)->flags &= ~2u;
	animation = stockman_animations[2];
	set_animation(0u, object, animation);
	sound = 5u;
	queue_sound(sound);
}

/* he gets over the hit */
void stockman_recover(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	stockman_set_floor(object);
	stockman_shake(object);
	stockman_steer(object);
	timer = OBJECT(object)->stockman.timer;
	OBJECT(object)->stockman.timer = timer - 1u;
	if (timer == 1u)
	{
		animation = stockman_animations[0];
		set_animation(0u, object, animation);
		OBJECT(object)->health = 1u;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->flags |= 2u;
	}
}

/* the defeated Stockman, by the state of his defeat */
void stockman_defeated(uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		stockman_defeated_explode(object);
		return;
	case 1:
		stockman_defeated_next_explosion(object);
		return;
	case 2:
		stockman_defeated_vanish(object);
		return;
	}
}

/* an explosion on his machine */
void stockman_defeated_explode(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t explosion_offset = 0, height_offset = 0, sound_choice = 0, x_offset = 0;
	uint32_t explosion = 0, objects_left = 0;

	stockman_set_floor(object);
	OBJECT(object)->stockman.timer = 0xCu;
	OBJECT(object)->state = 1u;
	explosion = OBJECT_ADDRESS(24);
	objects_left = 8u;
	find_free_object(&objects_left, &explosion);
	if ((uint8_t)objects_left == 0u)
	{
		OBJECT(object)->state = 2u;
		return;
	}
	explosion_offset = (uint16_t)((uint32_t)OBJECT(object)->stockman.explosions << 2);
	x_offset = (uint16_t)stockman_explosion_x[explosion_offset / 2u];
	height_offset = (uint16_t)stockman_explosion_height[explosion_offset / 2u];
	start_big_explosion(x_offset, height_offset, object, explosion);
	offset_high_word = 0u;
	sound_choice = OBJECT(object)->stockman.explosions;
	sound = explosion_sounds[sound_choice & 1u];
	object_loop_leftover = explosion_sounds_ADDRESS;
	queue_sound(sound);
}

/* the next explosion, until four; the boss is down */
void stockman_defeated_next_explosion(uint32_t object)
{
	uint8_t timer = 0;

	stockman_set_floor(object);
	timer = OBJECT(object)->stockman.timer;
	OBJECT(object)->stockman.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state--;
		OBJECT(object)->stockman.explosions++;
		mousers_break_down = 1u;
		if (OBJECT(object)->stockman.explosions == 4u)
			OBJECT(object)->state = 2u;
	}
}

/* he is gone when his defeat's animation ends */
void stockman_defeated_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemies_on_screen--;
	remove_object(object);
}

/* the hit machine shakes at its height */
void stockman_shake(uint32_t object)
{
	uint16_t shake_height = 0, shake_offset = 0, step_offset = 0, y = 0;

	offset_high_word = 0u;
	shake_offset = (uint8_t)frame_count;
	shake_height = stockman_shake_heights[(shake_offset & 6u) / 2u];
	step_offset = 0u;
	y = (uint16_t)OBJECT(object)->y;
	do
	{
		if ((uint16_t)y < stockman_height_steps[0])
			break;
		step_offset += 2u;
	} while ((uint8_t)step_offset < 0x1Eu);
	OBJECT(object)->height = shake_height + TABLE16(stockman_height_steps, 0x1Eu + (uint32_t)(int16_t)step_offset);
	distance_low_byte = step_offset;
}

/* he weaves toward his turtle */
void stockman_steer(uint32_t object)
{
	uint32_t ignored = 0, picked = 0, target = 0, y_step = 0;

	if ((frame_count & 0x3Fu) == 0u)
	{
		boss_pick_target(object, &ignored, &picked);
		if ((uint8_t)picked == 0u)
			OBJECT(object)->stockman.target = 0u;
	}
	target = OBJECT(object)->stockman.target;
	if (target == 0u)
		return;
	if (OBJECT(object)->screen_x < OBJECT(target)->screen_x)
	{
		if (OBJECT(object)->stockman.turning != 0u)
		{
			OBJECT(object)->stockman.heading -= OBJECT(object)->stockman.turn_speed;
			OBJECT(object)->stockman.direction &= 0xFu;
			if (OBJECT(object)->stockman.direction == 0xCu)
				OBJECT(object)->stockman.turning ^= 1u;
		}
		else
		{
			OBJECT(object)->stockman.heading += OBJECT(object)->stockman.turn_speed;
			OBJECT(object)->stockman.direction &= 0xFu;
			if (OBJECT(object)->stockman.direction == 4u)
				OBJECT(object)->stockman.turning ^= 1u;
		}
	}
	else if (OBJECT(object)->stockman.turning != 0u)
	{
		OBJECT(object)->stockman.heading -= OBJECT(object)->stockman.turn_speed;
		OBJECT(object)->stockman.direction &= 0xFu;
		if (OBJECT(object)->stockman.direction == 4u)
			OBJECT(object)->stockman.turning ^= 1u;
	}
	else
	{
		OBJECT(object)->stockman.heading += OBJECT(object)->stockman.turn_speed;
		OBJECT(object)->stockman.direction &= 0xFu;
		if (OBJECT(object)->stockman.direction == 0xCu)
			OBJECT(object)->stockman.turning ^= 1u;
	}
	y_step = 0x100u;
	if (OBJECT(target)->screen_y <= OBJECT(object)->screen_y)
		y_step = ~y_step;
	OBJECT(object)->y += y_step;
	stockman_move(object);
	stockman_keep_in_area(object);
}

/* he is kept within his part of the sewer */
void stockman_keep_in_area(uint32_t object)
{
	if (OBJECT(object)->x >= 0x47000u)
		OBJECT(object)->x = 0x47000u;
	else if (OBJECT(object)->x < 0x3C000u)
		OBJECT(object)->x = 0x3C000u;
	if (OBJECT(object)->y >= 0x7800u)
	{
		OBJECT(object)->y = 0x7800u;
		return;
	}
	if (OBJECT(object)->y < 0x1800u)
	{
		OBJECT(object)->y = 0x1800u;
		return;
	}
}

/* he bobs in the air at a height by how far back he is */
void stockman_bob(uint32_t object)
{
	uint16_t bob_height = 0, step_offset = 0, y = 0;

	offset_high_word = 0u;
	bob_height = stockman_bob_heights[(OBJECT(object)->stockman.bob & 0x3Eu) / 2u];
	step_offset = 0u;
	y = (uint16_t)OBJECT(object)->y;
	do
	{
		if ((uint16_t)y < stockman_height_steps[0])
			break;
		step_offset += 2u;
	} while ((uint8_t)step_offset < 0x1Eu);
	OBJECT(object)->height = bob_height + TABLE16(stockman_height_steps, 0x1Eu + (uint32_t)(int16_t)step_offset);
	OBJECT(object)->stockman.bob++;
	OBJECT(object)->stockman.bob &= 0x3Fu;
	distance_low_byte = step_offset;
}

/* he moves the way he flies */
void stockman_move(uint32_t object)
{
	uint16_t direction_offset = 0;

	offset_high_word = 0u;
	direction_offset = (uint16_t)((uint32_t)OBJECT(object)->stockman.direction << 2);
	OBJECT(object)->x += stockman_x_speeds[direction_offset / 2u];
	OBJECT(object)->y += stockman_y_speeds[direction_offset / 2u];
}

/* how long until his next drop, at random */
void stockman_random_drop_time(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t timer = 0;
	uint32_t range = 0;
	uint8_t timer_2 = 0, t1 = 0;

	range = 8u;
	range = random_below((uint16_t)range, object);
	OBJECT(object)->stockman.timer = stockman_drop_times[range];
	timer = (uint8_t)((uint32_t)difficulty_setting << 3);
	timer_2 = OBJECT(object)->stockman.timer;
	t1 = timer_2 - timer;
	zero = t1 == 0u;
	OBJECT(object)->stockman.timer = t1;
	if (timer_2 < timer || zero)
		OBJECT(object)->stockman.timer = 1u;
}

/* the floor under him, higher at the back */
void stockman_set_floor(uint32_t object)
{
	OBJECT(object)->floor_height = 0u;
	if (OBJECT(object)->y >= 0x6000u)
		OBJECT(object)->floor_height = 0x18u;
}

/* the number of the highest player in play */
uint32_t highest_player_in_play(void)
{
	uint8_t players = 0;
	uint16_t count = 0;
	uint32_t highest_player = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	highest_player = 0u;
	offset_high_word = 0u;
	count = 3u;
	players = players_in_play;
	do
	{
		t1 = players >> 1;
		players = t1;
		if (t1 != 0u)
			highest_player = (uint8_t)(highest_player + 1u);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	distance_low_byte = players;
	return highest_player;
}

/* a Mouser drops from his claws, unless he is over the middle of the sewer */
void stockman_start_mouser(uint32_t object, uint32_t object_2)
{
	if (OBJECT(object)->y >= 0x3800u && OBJECT(object)->y < 0x6800u)
		return;
	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x5Eu;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	OBJECT(object_2)->contacts |= 0x80u;
	OBJECT(object_2)->shadow = mouser_shadow_ADDRESS;
	place_at_object(object, object_2);
	OBJECT(object_2)->y += 0x280u;
	OBJECT(object_2)->height -= 0x800u;
	OBJECT(object_2)->count = 0u;
	enemies_on_screen++;
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
	object_loop_leftover = mouser_shadow_ADDRESS;
}

/* a Mouser falls from Stockman's claws */
void falling_mouser_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags = 0x81u;
	stats = falling_mouser_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0x400;
	animation = falling_mouser_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the Mouser falls to the floor and is a Mouser that goes after the turtles */
void falling_mouser_fall(uint32_t object)
{
	uint16_t height_speed = 0;
	uint16_t height = 0;

	height_speed = (uint16_t)OBJECT(object)->height_speed;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - height_speed;
	if (height < height_speed)
		OBJECT(object)->height = 0u;
	if (OBJECT(object)->y >= 0x4800u)
	{
		if (OBJECT(object)->height >= 0x1800u)
		{
			falling_mouser_idle();
			return;
		}
		OBJECT(object)->height = 0x1800u;
		OBJECT(object)->floor_height = 0x18u;
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0x4Eu;
		OBJECT(object)->parameter = 0x80u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		enemies_on_screen--;
		falling_mouser_idle();
		return;
	}
	if (OBJECT(object)->height != 0u)
	{
		falling_mouser_idle();
		return;
	}
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	OBJECT(object)->kind = 0x4Eu;
	OBJECT(object)->parameter = 0x80u;
	OBJECT(object)->routine = 0u;
	OBJECT(object)->state = 0u;
	enemies_on_screen--;
	falling_mouser_idle();
}

/* nothing */
void falling_mouser_idle(void)
{
}

/* Unused: the falling Mouser cannot be hit; it would be a Mouser on the floor */
void falling_mouser_struck(uint32_t object)
{
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	OBJECT(object)->kind = 0x4Eu;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->routine = 0u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->floor_height = 0u;
	enemies_on_screen--;
}
