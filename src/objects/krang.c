/**
 *    Name:        objects/krang.c
 *    Description: Krang's android body (area 9), his fist, his lasers and the curtain he appears behind
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Krang's appearance, step by step */
void krang_appear(uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		krang_appear_setup(object);
		return;
	case 1:
		krang_appear_wait(object);
		return;
	case 2:
		krang_appear_end(object);
		return;
	}
}

/* Krang stands ready, with his hits by the number of players */
void krang_appear_setup(uint32_t object)
{
	uint8_t player_count = 0;
	uint16_t count = 0;
	uint32_t animation = 0, stats = 0;
	uint16_t t1 = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = krang_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->krang.hits_left = OBJECT(object)->health_bars;
	player_count = 0u;
	offset_high_word = 0u;
	count = 3u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
			player_count++;
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->krang.hits_left += (uint32_t)player_count << 1;
	animation = krang_animations[1];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->krang.hit_delay = 0u;
	OBJECT(object)->state++;
}

/* Krang waits for the curtain to go, then raises his arms */
void krang_appear_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if ((OBJECT(krang_curtain)->flags | OBJECT(krang_curtain)->contacts) == 0u)
	{
		OBJECT(object)->state++;
		sound = 0xB0u;
		queue_sound(sound);
		animation = krang_animations[2];
		set_animation(0u, object, animation);
	}
}

/* Krang can be hit and starts his first move */
void krang_appear_end(uint32_t object)
{
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		krang_posed_last = 0u;
		OBJECT(object)->flags |= 6u;
		last_move = 0u;
		krang_pick_move(last_move, object);
	}
}

/* Krang walks; with a turtle close by, he may kick it, fire steep lasers or pose instead */
void krang_walk(uint32_t object)
{
	uint32_t close_turtle = 0, range = 0;

	krang_count_down(object);
	if (OBJECT(object)->state == 3u)
	{
		offset_high_word = 0u;
		switch (OBJECT(object)->state)
		{
		case 0:
			krang_walk_at_target(object);
			return;
		case 1:
			krang_walk_on(object);
			return;
		case 2:
			krang_charge(object);
			return;
		case 3:
			krang_pose_end(object);
			return;
		}
	}
	else
	{
		close_turtle = krang_find_close_turtle(object);
		if ((uint8_t)close_turtle == 0u)
		{
			offset_high_word = 0u;
			switch (OBJECT(object)->state)
			{
			case 0:
				krang_walk_at_target(object);
				return;
			case 1:
				krang_walk_on(object);
				return;
			case 2:
				krang_charge(object);
				return;
			case 3:
				krang_pose_end(object);
				return;
			}
		}
		else
		{
			range = 0x10u;
			range = random_below((uint16_t)range, object);
			if ((uint8_t)range < 7u)
			{
				offset_high_word = 0u;
				switch (OBJECT(object)->state)
				{
				case 0:
					krang_walk_at_target(object);
					return;
				case 1:
					krang_walk_on(object);
					return;
				case 2:
					krang_charge(object);
					return;
				case 3:
					krang_pose_end(object);
					return;
				}
			}
			else
			{
				OBJECT(object)->krang.close_turtle = close_turtle;
				range = 2u;
				range = random_below((uint16_t)range, object);
				switch ((uint16_t)(range << 2))
				{
				case 8:
					krang_start_pose(object);
					return;
				case 0:
					krang_start_kick(object);
					return;
				case 4:
					krang_start_lasers_steep(object);
					return;
				}
			}
		}
	}
}

/* Krang walks at his target a while */
void krang_walk_at_target(uint32_t object)
{
	uint32_t aim_routine = 0, last_move = 0;
	uint8_t move_time = 0;

	aim_routine = krang_aim_walk_ADDRESS;
	krang_walk_step(object, aim_routine);
	move_time = OBJECT(object)->krang.move_time;
	OBJECT(object)->krang.move_time = move_time - 1u;
	if (move_time != 0u)
		return;
	last_move = 1u;
	krang_next_move(last_move, object);
}

/* Krang walks on at his target a while */
void krang_walk_on(uint32_t object)
{
	uint32_t aim_routine = 0, last_move = 0;
	uint8_t move_time = 0;

	aim_routine = krang_aim_walk_ADDRESS;
	krang_walk_step(object, aim_routine);
	move_time = OBJECT(object)->krang.move_time;
	OBJECT(object)->krang.move_time = move_time - 1u;
	if (move_time != 0u)
		return;
	last_move = 2u;
	krang_next_move(last_move, object);
}

/* Krang runs at his target a while */
void krang_charge(uint32_t object)
{
	uint32_t aim_routine = 0, last_move = 0;
	uint8_t move_time = 0;

	aim_routine = krang_aim_charge_ADDRESS;
	krang_walk_step(object, aim_routine);
	move_time = OBJECT(object)->krang.move_time;
	OBJECT(object)->krang.move_time = move_time - 1u;
	if (move_time != 0u)
		return;
	last_move = 3u;
	krang_next_move(last_move, object);
}

/* Krang's pose ends and he picks his next move */
void krang_pose_end(uint32_t object)
{
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	last_move = 4u;
	krang_next_move(last_move, object);
}

/* every 16 frames Krang aims again, and he moves every other frame */
void krang_walk_step(uint32_t object, uint32_t aim_routine)
{
	uint32_t phase = 0;
	uint8_t t1 = 0;

	t1 = (uint8_t)frame_count & 0xFu;
	phase = t1;
	if (t1 == 0u)
	{
		switch (aim_routine)
		{
		case krang_aim_charge_ADDRESS:
			phase = krang_aim_charge(phase, object);
			break;
		case krang_aim_walk_ADDRESS:
			phase = krang_aim_walk(phase, object);
			break;
		}
	}
	if ((phase & 1u) == 0u)
		krang_move(object);
}

/* with a turtle in front of him Krang fires lasers, punches or poses; else he picks another walk */
void krang_next_move(uint32_t last_move, uint32_t object)
{
	uint32_t range = 0, turtle_in_front = 0;

	turtle_in_front = krang_find_turtle_in_front(object);
	if ((uint8_t)turtle_in_front == 0u)
	{
		krang_pick_move(last_move, object);
		return;
	}
	range = 8u;
	range = random_below((uint16_t)range, object);
	switch ((uint16_t)(range << 2))
	{
	case 32:
		krang_start_pose(object);
		return;
	case 0:
	case 12:
	case 24:
		krang_start_lasers(object);
		return;
	case 4:
	case 16:
	case 28:
		krang_start_lasers_far(object);
		return;
	case 8:
	case 20:
		krang_start_punch(object);
		return;
	}
}

/* Krang's kick or punch, step by step */
void krang_melee(uint32_t object)
{
	krang_count_down(object);
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		krang_kick_end(object);
		return;
	case 1:
		krang_punch_throw(object);
		return;
	case 2:
		krang_punch_wait(object);
		return;
	}
}

/* Krang's kick ends and he walks again */
void krang_kick_end(uint32_t object)
{
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 0u;
		last_move = 6u;
		krang_pick_move(last_move, object);
		return;
	}
}

/* Krang throws his arm out and his fist flies off it */
void krang_punch_throw(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->frame_time++;
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->state++;
		animation = krang_animations[14];
		set_animation(0u, object, animation);
		krang_fist_flying = 1u;
		krang_start_fist(object);
	}
}

/* Krang waits for his fist to fly off the screen */
void krang_punch_wait(uint32_t object)
{
	uint32_t last_move = 0;

	if (krang_fist_flying != 0u)
		return;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 0u;
	last_move = 6u;
	krang_pick_move(last_move, object);
}

/* Krang's lasers, step by step */
void krang_fire_lasers(uint32_t object)
{
	krang_count_down(object);
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		krang_lasers_fire(object);
		return;
	case 1:
		krang_lasers_wait(object);
		return;
	case 2:
		krang_lasers_end(object);
		return;
	}
}

/* Krang fires a stream of five lasers */
void krang_lasers_fire(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t count = 0;
	uint32_t delay = 0, laser = 0, objects_left = 0;
	uint16_t t1 = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	count = 4u;
	do
	{
		laser = OBJECT_ADDRESS(32);
		objects_left = 8u;
		find_free_object(&objects_left, &laser);
		if ((uint8_t)objects_left == 0u)
		{
			if ((uint8_t)count != 4u)
			{
				sound = 0x1Bu;
				queue_sound(sound);
			}
			return;
		}
		krang_start_laser(object, laser);
		delay = count * 2u;
		offset_high_word = delay;
		OBJECT(laser)->krang_laser.delay = delay;
		OBJECT(laser)->krang_laser.last = 0u;
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	OBJECT(laser)->krang_laser.last |= 0x80u;
	if ((uint8_t)count != 4u)
	{
		sound = 0x1Bu;
		queue_sound(sound);
	}
}

/* Krang waits for all his lasers to be on their way */
void krang_lasers_wait(uint32_t object)
{
	if (krang_lasers_waiting == 0u)
	{
		OBJECT(object)->state++;
		OBJECT(object)->krang.timer = 0x14u;
		OBJECT(object)->flags |= 2u;
	}
}

/* after his lasers Krang walks again */
void krang_lasers_end(uint32_t object)
{
	uint32_t last_move = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->krang.timer;
	OBJECT(object)->krang.timer = timer - 1u;
	if (timer == 0u)
	{
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 0u;
		last_move = 7u;
		krang_pick_move(last_move, object);
		return;
	}
}

/* a turtle's attack takes one of Krang's hits; with none left he starts to burst */
void krang_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint8_t hits_left = 0;

	krang_posed_last = 0u;
	OBJECT(object)->krang.hurt = 0x80u;
	OBJECT(object)->colour = 0xFFu;
	hits_left = OBJECT(object)->krang.hits_left;
	OBJECT(object)->krang.hits_left = hits_left - 1u;
	if (hits_left != 1u)
	{
		OBJECT(object)->health_bars = 0xAu;
		if (OBJECT(object)->krang.hits_left == 1u)
			OBJECT(object)->health_bars = 1u;
		OBJECT(object)->routine++;
		OBJECT(object)->health = 1u;
		OBJECT(object)->flags &= ~2u;
		sound = 0x82u;
		queue_sound(sound);
		animation = krang_animations[8];
		set_animation(0u, object, animation);
		return;
	}
	animation = krang_animations[13];
	set_animation(0u, object, animation);
	sound = 0u;
	queue_sound(sound);
	sound = 0x8Eu;
	queue_sound(sound);
	OBJECT(object)->routine += 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->krang.timer = 0u;
	OBJECT(object)->flags &= ~6u;
}

/* Krang recovers from a hit and goes on */
void krang_recoil(uint32_t object)
{
	uint32_t last_move = 0;

	if (OBJECT(object)->krang.hits_left < 0x14u)
		OBJECT(object)->colour = 6u;
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->state = 0u;
	OBJECT(object)->krang.hurt = 0u;
	OBJECT(object)->krang.hit_delay = 0xCu;
	OBJECT(object)->colour = 0xFFu;
	last_move = 6u;
	krang_pick_move(last_move, object);
}

/* Krang's end, step by step */
void krang_explode(uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		krang_explode_start(object);
		return;
	case 1:
		krang_explode_wait(object);
		return;
	case 2:
		krang_explode_burst(object);
		return;
	case 3:
		krang_explode_next(object);
		return;
	}
}

/* Krang stops, with his shadow, before he bursts */
void krang_explode_start(uint32_t object)
{
	OBJECT(object)->krang_explode.timer = 0x8Cu;
	OBJECT(object)->shadow = krang_explode_shadow_ADDRESS;
	OBJECT(object)->state++;
}

/* Krang's body falls apart */
void krang_explode_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->krang_explode.timer;
	OBJECT(object)->krang_explode.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state++;
		animation = krang_animations[9];
		set_animation(0u, object, animation);
	}
}

/* an explosion bursts on Krang's body as it floats */
void krang_explode_burst(uint32_t object)
{
	uint32_t explosion = 0, objects_left = 0;

	if (OBJECT(object)->frame_time == 0u)
		krang_hover(object);
	OBJECT(object)->krang_explode.timer = 0xEu;
	OBJECT(object)->state = 3u;
	explosion = OBJECT_ADDRESS(24);
	objects_left = 8u;
	find_free_object(&objects_left, &explosion);
	if ((uint8_t)objects_left != 0u)
	{
		boss_start_explosion(object, explosion);
		return;
	}
	OBJECT(object)->routine++;
	OBJECT(object)->state = 0u;
}

/* Unused: an empty routine that returns at once */
void lone_return_before_krang_explode_next(void)
{
}

/* the next of Krang's ten explosions */
void krang_explode_next(uint32_t object)
{
	uint8_t timer = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->contacts |= 0x80u;
		krang_hover(object);
	}
	timer = OBJECT(object)->krang_explode.timer;
	OBJECT(object)->krang_explode.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state--;
		OBJECT(object)->krang_explode.explosions++;
		if (OBJECT(object)->krang_explode.explosions == 0xAu)
		{
			OBJECT(object)->routine++;
			OBJECT(object)->state = 0u;
		}
	}
}

/* Krang's escape, step by step */
void krang_escape(uint16_t on_right, uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		krang_escape_start(on_right, object);
		return;
	case 1:
		krang_escape_wait(object);
		return;
	case 2:
		krang_escape_dip(object);
		return;
	case 3:
		krang_escape_fly(object);
		return;
	case 4:
		krang_gone(object);
		return;
	}
}

/* Krang's head comes out in his pod, saying he is invincible */
void krang_escape_start(uint16_t on_right, uint32_t object)
{
	uint8_t sound = 0;
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t animation = 0;

	animation = krang_animations[10];
	set_animation(0u, object, animation);
	sound = 0xB0u;
	queue_sound(sound);
	on_right = (uint16_t)(on_right | 0xFFu);
	screen_y = OBJECT(object)->screen_y;
	screen_x = (uint16_t)(OBJECT(object)->screen_x + 0x10u);
	say_im_invincible(on_right, screen_x, screen_y);
	OBJECT(object)->state++;
}

/* the pod floats a while, then takes off */
void krang_escape_wait(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
	{
		krang_hover(object);
		return;
	}
	animation = krang_animations[11];
	set_animation(0u, object, animation);
	OBJECT(object)->height_speed = 0x280;
	OBJECT(object)->state++;
}

/* the pod dips, slower and slower */
void krang_escape_dip(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint16_t height_speed = 0;

	OBJECT(object)->height -= OBJECT(object)->height_speed;
	height_speed = OBJECT(object)->height_speed;
	OBJECT(object)->height_speed = height_speed - 0x20u;
	if (height_speed < 0x20u)
	{
		sound = 0x11u;
		queue_sound(sound);
		animation = krang_animations[12];
		set_animation(0u, object, animation);
		OBJECT(object)->height_speed = 0x500;
		OBJECT(object)->state++;
	}
}

/* the pod flies up off the screen */
void krang_escape_fly(uint32_t object)
{
	OBJECT(object)->height += OBJECT(object)->height_speed;
	if (OBJECT(object)->height >= 0xF000u)
	{
		krang_gone(object);
		return;
	}
}

/* Krang is gone: one enemy less */
void krang_gone(uint32_t object)
{
	enemies_on_screen--;
	remove_object(object);
}

/* Krang floats up and down */
void krang_hover(uint32_t object)
{
	offset_high_word = 0u;
	OBJECT(object)->height = krang_hover_heights[(frame_count & 0x3Eu) / 2u] + 0x4000u;
}

/* picks Krang's next walk, charge or pose by its chances */
void krang_pick_move(uint32_t last_move, uint32_t object)
{
	uint16_t count = 0;
	uint32_t chances = 0, range = 0;
	uint8_t move_chance = 0, t1 = 0;
	uint16_t t2 = 0;

	range = 0x10u;
	range = random_below((uint16_t)range, object);
	count = 3u;
	last_move = (uint16_t)(last_move << 2);
	chances = krang_move_chances_ADDRESS + last_move;
	for (;;)
	{
		move_chance = MOVE_CHANCE(chances + (int16_t)count);
		t1 = range;
		range = (uint8_t)(t1 - move_chance);
		if (t1 < move_chance)
			break;
		t2 = count - 1u;
		count = t2;
		if (t2 == 0xFFFFu)
		{
			count = 2u;
			break;
		}
	}
	switch ((uint32_t)(int16_t)((uint32_t)count << 2))
	{
	case 0:
		krang_start_walk(object);
		return;
	case 4:
		krang_start_walk_on(object);
		return;
	case 8:
		krang_start_charge(object);
		return;
	case 12:
		krang_start_pose(object);
		return;
	}
}

/* Krang picks a turtle and walks at it; with none, he poses */
void krang_start_walk(uint32_t object)
{
	uint16_t range = 0;
	uint32_t animation = 0, ignored = 0, picked = 0;

	boss_pick_target(object, &ignored, &picked);
	if ((uint8_t)picked == 0u)
	{
		krang_start_pose(object);
		return;
	}
	krang_posed_last = 0u;
	range = 8u;
	krang_set_move_time(range, object);
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 0u;
	animation = krang_animations[3];
	set_animation(0u, object, animation);
}

/* Krang walks on at his target */
void krang_start_walk_on(uint32_t object)
{
	uint16_t range = 0;

	range = 8u;
	krang_set_move_time(range, object);
	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 1u;
}

/* Krang runs at his target */
void krang_start_charge(uint32_t object)
{
	uint16_t range = 0;

	range = 8u;
	krang_set_move_time(range, object);
	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 2u;
}

/* Krang poses, with his cry the first time in a row */
void krang_start_pose(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 3u;
	if (krang_posed_last == 0u)
	{
		krang_posed_last = 1u;
		sound = 0x8Fu;
		queue_sound(sound);
	}
	animation = krang_animations[7];
	set_animation(0u, object, animation);
}

/* Krang kicks the turtle close to him */
void krang_start_kick(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->attack &= 0x7Fu;
	if ((int8_t)OBJECT(object)->krang.close_turtle >= 0)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->attack |= 0x80u;
	}
	OBJECT(object)->krang.close_turtle = 0u;
	sound = 8u;
	queue_sound(sound);
	animation = krang_animations[4];
	set_animation(0u, object, animation);
}

/* Krang aims his lasers steeply, at a turtle close by */
void krang_start_lasers_steep(uint32_t object)
{
	uint32_t animation = 0;

	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 2u;
	OBJECT(object)->routine = 3u;
	OBJECT(object)->state = 0u;
	animation = krang_animations[5];
	if (OBJECT(object)->krang.hits_left < 0x1Eu)
		animation = krang_animations[15];
	set_animation(0u, object, animation);
}

/* Krang aims his lasers at the middle angle */
void krang_start_lasers(uint32_t object)
{
	uint32_t animation = 0;

	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 1u;
	OBJECT(object)->routine = 3u;
	OBJECT(object)->state = 0u;
	animation = krang_animations[5];
	if (OBJECT(object)->krang.hits_left < 0x1Eu)
		animation = krang_animations[15];
	set_animation(0u, object, animation);
}

/* Krang aims his lasers at the shallowest angle, far in front of him */
void krang_start_lasers_far(uint32_t object)
{
	uint32_t animation = 0;

	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 3u;
	OBJECT(object)->state = 0u;
	animation = krang_animations[5];
	if (OBJECT(object)->krang.hits_left < 0x1Eu)
		animation = krang_animations[15];
	set_animation(0u, object, animation);
}

/* Krang draws back his arm to punch */
void krang_start_punch(uint32_t object)
{
	uint32_t animation = 0;

	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 1u;
	animation = krang_animations[6];
	set_animation(0u, object, animation);
}

/* how long Krang's walk lasts, at random */
void krang_set_move_time(uint32_t range, uint32_t object)
{
	range = random_below((uint16_t)range, object);
	OBJECT(object)->krang.move_time = krang_move_times[range];
}

/* aims Krang's run at his target */
uint32_t krang_aim_charge(uint32_t phase, uint32_t object)
{
	uint32_t direction = 0, speeds = 0;

	direction = boss_direction_to_target(object);
	speeds = krang_charge_speeds_ADDRESS;
	return krang_set_speeds((uint16_t)direction, phase, object, speeds);
}

/* aims Krang's walk at his target */
uint32_t krang_aim_walk(uint32_t phase, uint32_t object)
{
	uint32_t direction = 0, speeds = 0;

	direction = boss_direction_to_target(object);
	speeds = krang_walk_speeds_ADDRESS;
	return krang_set_speeds((uint16_t)direction, phase, object, speeds);
}

/* Krang's speeds in the direction picked, and he turns that way */
uint32_t krang_set_speeds(uint16_t direction, uint32_t phase, uint32_t object, uint32_t speeds)
{
	direction = (uint16_t)((uint32_t)direction << 2);
	phase = (phase & ~0xFFFFu) | KRANG_SPEED(speeds + (direction / 2u) * 0x2u);
	OBJECT(object)->x_speed = phase;
	phase = (phase & ~0xFFFFu) | KRANG_SPEED(speeds + ((int32_t)(direction / 2u + 1u)) * 0x2u);
	OBJECT(object)->y_speed = phase;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->flags &= 0xBFu;
	if (OBJECT(object)->x_speed >= 0)
		OBJECT(object)->flags |= 0x40u;
	return phase;
}

/* Krang can be hit again after a while; weak, he flashes faster and faster */
void krang_count_down(uint32_t object)
{
	uint8_t colour = 0, colour_2 = 0, flash_mask = 0;
	uint8_t hit_delay = 0;

	colour = 6u;
	colour_2 = 0xFFu;
	if (OBJECT(object)->krang.hit_delay != 0u)
	{
		hit_delay = OBJECT(object)->krang.hit_delay;
		OBJECT(object)->krang.hit_delay = hit_delay - 1u;
		if (hit_delay == 1u)
			OBJECT(object)->flags |= 2u;
	}
	if (OBJECT(object)->krang.hits_left >= 0x1Eu)
	{
		distance_low_byte = colour;
		return;
	}
	flash_mask = 0x3Cu;
	if (OBJECT(object)->krang.hits_left < 0x14u)
	{
		flash_mask = 8u;
		if (OBJECT(object)->krang.hits_left < 0xAu)
			flash_mask = 1u;
	}
	OBJECT(object)->colour = colour_2;
	if ((flash_mask & (uint8_t)frame_count) == 0u)
		OBJECT(object)->colour = colour;
	distance_low_byte = colour;
}

/* Krang moves, kept on the floor and on the screen */
void krang_move(uint32_t object)
{
	move_by_speeds(object);
	if (OBJECT(object)->y >= 0x8000u)
		OBJECT(object)->y = 0x8000u;
	if (OBJECT(object)->y < 0x1000u)
		OBJECT(object)->y = 0x1000u;
	if ((int16_t)OBJECT(object)->screen_x < 0)
		OBJECT(object)->x += 0x800u;
}

/* which turtle is close to Krang, and on which side */
uint32_t krang_find_close_turtle(uint32_t object)
{
	uint16_t count = 0, x_distance = 0, y_distance = 0;
	uint32_t close_turtle = 0, object_2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	close_turtle = 0u;
	for (;;)
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_knocked_out & shl32(1u, (int)(count & 7u))) == 0u && OBJECT(object_2)->routine != 4u)
		{
			t1 = OBJECT(object_2)->screen_y - OBJECT(object)->screen_y;
			y_distance = t1;
			distance_low_byte = y_distance;
			if ((int16_t)t1 < 0)
			{
				y_distance = (uint16_t)~y_distance;
				distance_low_byte = y_distance;
			}
			if (y_distance < 0xBu)
			{
				t2 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
				x_distance = t2;
				distance_low_byte = x_distance;
				if ((int16_t)t2 < 0)
				{
					close_turtle = close_turtle | 0x80u;
					x_distance = (uint16_t)~x_distance;
					distance_low_byte = x_distance;
				}
				if (x_distance < 0x3Au)
					break;
			}
		}
		close_turtle = 0u;
		t3 = count - 1u;
		count = t3;
		object_2 -= 0x50u;
		if (t3 == 0xFFFFu)
			return close_turtle;
	}
	close_turtle |= shl32(1u, (int)(count & 0x1Fu));
	return close_turtle;
}

/* which turtle is level with Krang and in front of him, far enough */
uint32_t krang_find_turtle_in_front(uint32_t object)
{
	uint16_t count = 0, x_distance = 0, y_distance = 0;
	uint32_t object_2 = 0, turtle_in_front = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	turtle_in_front = 0u;
	for (;;)
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_knocked_out & shl32(1u, (int)(count & 7u))) == 0u && OBJECT(object_2)->routine != 4u)
		{
			t1 = OBJECT(object_2)->screen_y - OBJECT(object)->screen_y;
			y_distance = t1;
			distance_low_byte = y_distance;
			if ((int16_t)t1 < 0)
			{
				y_distance = (uint16_t)~y_distance;
				distance_low_byte = y_distance;
			}
			if (y_distance < 0xBu)
			{
				if ((OBJECT(object)->flags & 0x40u) != 0u)
				{
					t2 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
					x_distance = t2;
					distance_low_byte = x_distance;
					if ((int16_t)t2 >= 0 && x_distance >= 0x3Eu)
						break;
				}
				else
				{
					t3 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
					x_distance = t3;
					distance_low_byte = x_distance;
					if ((int16_t)t3 < 0)
					{
						x_distance = (uint16_t)~x_distance;
						distance_low_byte = x_distance;
						if (x_distance >= 0x3Eu)
							break;
					}
				}
			}
		}
		t4 = count - 1u;
		count = t4;
		object_2 -= 0x50u;
		if (t4 == 0xFFFFu)
			return turtle_in_front;
	}
	turtle_in_front |= shl32(1u, (int)(count & 0x1Fu));
	return turtle_in_front;
}

/* the curtain where Krang appears shimmers */
void krang_curtain_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->colour = 8u;
	OBJECT(object)->animation_delay = 0u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = krang_curtain_animations[0];
	set_animation(0u, object, animation);
	krang_curtain = object;
	OBJECT(object)->routine++;
}

/* the curtain breaks up */
void krang_curtain_open(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	sound = 0x16u;
	queue_sound(sound);
	animation = krang_curtain_animations[1];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the curtain is gone, and Krang starts */
void krang_curtain_end(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	remove_object(object);
}

/* one laser of Krang's stream, from his body at the angle he aims */
void krang_start_laser(uint32_t object, uint32_t laser)
{
	uint16_t laser_angle = 0;
	uint32_t laser_places = 0;

	OBJECT(laser)->flags = 1u;
	OBJECT(laser)->contacts = 0u;
	OBJECT(laser)->kind = 0x67u;
	OBJECT(laser)->parameter = 0u;
	OBJECT(laser)->colour = 0xFFu;
	OBJECT(laser)->animation_delay = 0u;
	place_at_object(object, laser);
	OBJECT(laser)->krang_laser.flipped = 0u;
	laser_places = krang_laser_place_x_ADDRESS;
	laser_angle = OBJECT(object)->krang.laser_angle;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		laser_angle = (uint8_t)(laser_angle + 3u);
		OBJECT(laser)->krang_laser.flipped |= 0x80u;
	}
	laser_angle = (uint16_t)((uint32_t)laser_angle << 2);
	OBJECT(laser)->x += krang_laser_place_x[laser_angle / 2u];
	OBJECT(laser)->height += krang_laser_place_height[laser_angle / 2u];
	OBJECT(laser)->y -= 0x200u;
	OBJECT(laser)->krang_laser.shooter = object;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(laser)->flags |= 0x40u;
	OBJECT(laser)->parameter = OBJECT(object)->krang.laser_angle;
	OBJECT(laser)->routine = 0u;
	OBJECT(laser)->state = 0u;
	krang_lasers_waiting++;
	object_loop_leftover = laser_places;
}

/* a laser of Krang's stream, at its angle */
void krang_laser_start(uint32_t object)
{
	uint16_t angle_offset = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x84u;
	stats = krang_laser_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->flags &= 0xBFu;
	angle_offset = (uint16_t)((uint32_t)OBJECT(object)->parameter << 2);
	OBJECT(object)->x_speed = krang_laser_x_speeds[angle_offset / 2u];
	OBJECT(object)->height_speed = krang_laser_height_speeds[angle_offset / 2u];
	if ((int8_t)OBJECT(object)->krang_laser.flipped < 0)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->attack |= 0x80u;
		OBJECT(object)->x_speed = ~OBJECT(object)->x_speed;
	}
	offset_high_word = 0u;
	animation = krang_laser_animations[OBJECT(object)->parameter];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the laser waits its turn in the stream */
void krang_laser_wait(uint32_t object)
{
	uint8_t delay = 0;

	delay = OBJECT(object)->krang_laser.delay;
	OBJECT(object)->krang_laser.delay = delay - 1u;
	if (delay == 0u)
	{
		OBJECT(object)->routine++;
		krang_lasers_waiting--;
	}
}

/* the laser goes down to the floor; the stream's last one sparks there */
void krang_laser_fly(uint32_t object)
{
	uint32_t animation = 0;

	move_by_speeds(object);
	if ((int16_t)OBJECT(object)->height >= 0)
		return;
	if (OBJECT(object)->krang_laser.last != 0u)
	{
		OBJECT(object)->height = 0u;
		OBJECT(object)->routine++;
		animation = krang_laser_animations[3];
		set_animation(0u, object, animation);
		return;
	}
	remove_object(object);
}

/* the laser's spark ends and it is gone */
void krang_laser_spark(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	remove_object(object);
}

/* Krang's fist flies off his arm */
void krang_start_fist(uint32_t object)
{
	uint32_t fist = 0, fist_offset = 0, objects_left = 0;

	fist = OBJECT_ADDRESS(24);
	objects_left = 8u;
	find_free_object(&objects_left, &fist);
	OBJECT(fist)->flags = 1u;
	OBJECT(fist)->contacts = 0u;
	OBJECT(fist)->kind = 0x66u;
	OBJECT(fist)->parameter = 0u;
	OBJECT(fist)->colour = 0xFFu;
	OBJECT(fist)->animation_delay = 0u;
	place_at_object(object, fist);
	OBJECT(fist)->krang_fist.flipped = 0u;
	fist_offset = 0x4500u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		fist_offset = (int16_t)~fist_offset;
		offset_high_word = fist_offset;
		OBJECT(fist)->krang_fist.flipped |= 0x80u;
	}
	OBJECT(fist)->x += (fist_offset & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
	OBJECT(fist)->y -= 0x200u;
	OBJECT(fist)->height = 0x3200u;
	OBJECT(fist)->krang_fist.shooter = object;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(fist)->routine = 0u;
	OBJECT(fist)->state = 0u;
}

/* Krang's fist sets off across the screen */
void krang_fist_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = krang_fist_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->x_speed = 0x680;
	OBJECT(object)->flags |= 0x40u;
	if ((int8_t)OBJECT(object)->krang_fist.flipped < 0)
	{
		OBJECT(object)->flags &= 0xBFu;
		OBJECT(object)->attack &= 0x7Fu;
		OBJECT(object)->x_speed = ~OBJECT(object)->x_speed;
	}
	animation = krang_fist_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the fist comes off and flies, hurting */
void krang_fist_launch(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		animation = krang_fist_animations[1];
		set_animation(0u, object, animation);
		sound = 6u;
		queue_sound(sound);
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->routine++;
	}
}

/* the fist flies across; off the screen, Krang's punch is over */
void krang_fist_fly(uint32_t object)
{
	uint32_t x_speed = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		krang_fist_flying = 0u;
		remove_object(object);
		return;
	}
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	offset_high_word = x_speed;
}
