/**
 *    Name:        objects/krang.c
 *    Description: Krang's android body (area 9), his fist, his lasers and the curtain he appears behind
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Krang's appearance, step by step */
void krang_appear(uint32_t object, uint32_t *p_state_offset, uint32_t *p_a5)
{
	*p_state_offset = OBJECT(object)->state;
	*p_state_offset = (uint16_t)(*p_state_offset << 2);
	switch (*p_state_offset)
	{
	case 0:
		*p_state_offset = krang_appear_setup(object);
		return;
	case 4:
		krang_appear_wait(object);
		return;
	case 8:
		krang_appear_end(object, p_state_offset, p_a5);
		return;
	}
}

/* Krang stands ready, with his hits by the number of players */
uint32_t krang_appear_setup(uint32_t object)
{
	uint16_t count = 0;
	uint32_t animation = 0, player_count = 0, stats = 0;
	uint16_t t1 = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = krang_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->krang.hits_left = OBJECT(object)->health_bars;
	player_count = 0u;
	count = 3u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
			player_count = (uint8_t)(player_count + 1u);
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	player_count = (uint16_t)(player_count << 1);
	OBJECT(object)->krang.hits_left += player_count;
	animation = krang_animations[1];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->krang.hit_delay = 0u;
	OBJECT(object)->state++;
	return player_count;
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
void krang_appear_end(uint32_t object, uint32_t *p_roll, uint32_t *p_a5)
{
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	krang_posed_last = 0u;
	OBJECT(object)->flags |= 6u;
	last_move = 0u;
	krang_pick_move(last_move, object, p_roll, p_a5);
}

/* Krang walks; with a turtle close by, he may kick it, fire steep lasers or pose instead */
void krang_walk(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t close_turtle = 0, range = 0, state_offset = 0;

	d5 = krang_count_down(d5, object);
	if (OBJECT(object)->state == 3u)
	{
		state_offset = OBJECT(object)->state;
		state_offset = (uint16_t)(state_offset << 2);
		switch (state_offset)
		{
		case 0:
			krang_walk_at_target(object, &d5, &state_offset, p_a5);
			break;
		case 4:
			krang_walk_on(object, &d5, &state_offset, p_a5);
			break;
		case 8:
			krang_charge(object, &d5, &state_offset, p_a5);
			break;
		case 12:
			krang_pose_end(object, &d5, &state_offset, p_a5);
			break;
		}
	}
	else
	{
		krang_find_close_turtle(object, &close_turtle, &d5);
		if ((uint8_t)close_turtle == 0u)
		{
			state_offset = OBJECT(object)->state;
			state_offset = (uint16_t)(state_offset << 2);
			switch (state_offset)
			{
			case 0:
				krang_walk_at_target(object, &d5, &state_offset, p_a5);
				break;
			case 4:
				krang_walk_on(object, &d5, &state_offset, p_a5);
				break;
			case 8:
				krang_charge(object, &d5, &state_offset, p_a5);
				break;
			case 12:
				krang_pose_end(object, &d5, &state_offset, p_a5);
				break;
			}
		}
		else
		{
			range = 0x10u;
			range = random_below((uint16_t)range, object);
			if ((uint8_t)range < 7u)
			{
				state_offset = OBJECT(object)->state;
				state_offset = (uint16_t)(state_offset << 2);
				switch (state_offset)
				{
				case 0:
					krang_walk_at_target(object, &d5, &state_offset, p_a5);
					break;
				case 4:
					krang_walk_on(object, &d5, &state_offset, p_a5);
					break;
				case 8:
					krang_charge(object, &d5, &state_offset, p_a5);
					break;
				case 12:
					krang_pose_end(object, &d5, &state_offset, p_a5);
					break;
				}
			}
			else
			{
				OBJECT(object)->krang.close_turtle = close_turtle;
				state_offset = 2u;
				state_offset = random_below((uint16_t)state_offset, object);
				state_offset = (uint16_t)(state_offset << 2);
				switch (state_offset)
				{
				case 8:
					krang_start_pose(object);
					break;
				case 0:
					krang_start_kick(object);
					break;
				case 4:
					krang_start_lasers_steep(object);
					break;
				}
			}
		}
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = state_offset;
}

/* Krang walks at his target a while */
void krang_walk_at_target(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t aim_routine = 0, last_move = 0;
	uint8_t move_time = 0;

	aim_routine = krang_aim_walk_ADDRESS;
	krang_walk_step(object, aim_routine, p_d7, p_a5);
	move_time = OBJECT(object)->krang.move_time;
	OBJECT(object)->krang.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 1u;
		krang_next_move(last_move, object, &d5, p_d7, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* Krang walks on at his target a while */
void krang_walk_on(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t aim_routine = 0, last_move = 0;
	uint8_t move_time = 0;

	aim_routine = krang_aim_walk_ADDRESS;
	krang_walk_step(object, aim_routine, p_d7, p_a5);
	move_time = OBJECT(object)->krang.move_time;
	OBJECT(object)->krang.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 2u;
		krang_next_move(last_move, object, &d5, p_d7, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* Krang runs at his target a while */
void krang_charge(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t aim_routine = 0, last_move = 0;
	uint8_t move_time = 0;

	aim_routine = krang_aim_charge_ADDRESS;
	krang_walk_step(object, aim_routine, p_d7, p_a5);
	move_time = OBJECT(object)->krang.move_time;
	OBJECT(object)->krang.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 3u;
		krang_next_move(last_move, object, &d5, p_d7, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* Krang's pose ends and he picks his next move */
void krang_pose_end(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	last_move = 4u;
	krang_next_move(last_move, object, &d5, p_d7, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* every 16 frames Krang aims again, and he moves every other frame */
void krang_walk_step(uint32_t object, uint32_t aim_routine, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d7 = *p_d7;
	uint32_t d7_2 = 0;
	uint8_t t1 = 0, t2 = 0;

	d7_2 = (d7 & ~0xFFu) | (uint8_t)frame_count;
	t1 = d7_2 & 0xFu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 == 0u)
	{
		switch (aim_routine)
		{
		case 0x00E708u:
			krang_aim_charge(object, &d7_2, p_a5);
			break;
		case 0x00E718u:
			krang_aim_walk(object, &d7_2, p_a5);
			break;
		}
	}
	t2 = d7_2 & 1u;
	d7_2 = (d7_2 & ~0xFFu) | t2;
	if (t2 == 0u)
		d7_2 = krang_move(object);
	*p_d7 = d7_2;
}

/* with a turtle in front of him Krang fires lasers, punches or poses; else he picks another walk */
void krang_next_move(uint32_t last_move, uint32_t object, uint32_t *p_d5, uint32_t *p_move_offset, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t range = 0, turtle_in_front = 0;

	krang_find_turtle_in_front(object, &d5, &turtle_in_front);
	if ((uint8_t)turtle_in_front != 0u)
	{
		range = 8u;
		range = random_below((uint16_t)range, object);
		range = (uint16_t)(range << 2);
		switch (range)
		{
		case 32:
			krang_start_pose(object);
			break;
		case 0:
		case 12:
		case 24:
			krang_start_lasers(object);
			break;
		case 4:
		case 16:
		case 28:
			krang_start_lasers_far(object);
			break;
		case 8:
		case 20:
			krang_start_punch(object);
			break;
		}
	}
	else
		krang_pick_move(last_move, object, &range, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_move_offset = range;
}

/* Krang's kick or punch, step by step */
void krang_melee(uint32_t object, uint32_t *p_d5, uint32_t *p_state_offset, uint32_t *p_a5)
{
	uint32_t d5 = 0, state_offset = 0;

	d5 = krang_count_down(d5, object);
	state_offset = OBJECT(object)->state;
	state_offset = (uint16_t)(state_offset << 2);
	switch (state_offset)
	{
	case 0:
		krang_kick_end(object, &state_offset, p_a5);
		break;
	case 4:
		state_offset = krang_punch_throw(state_offset, object);
		break;
	case 8:
		krang_punch_wait(object, &state_offset, p_a5);
		break;
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_state_offset = state_offset;
}

/* Krang's kick ends and he walks again */
void krang_kick_end(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 0u;
		last_move = 6u;
		krang_pick_move(last_move, object, p_d7, p_a5);
	}
}

/* Krang throws his arm out and his fist flies off it */
uint32_t krang_punch_throw(uint32_t state_offset, uint32_t object)
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
		state_offset = krang_start_fist(state_offset, object);
	}
	return state_offset;
}

/* Krang waits for his fist to fly off the screen */
void krang_punch_wait(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t last_move = 0;

	if (krang_fist_flying != 0u)
		return;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 0u;
	last_move = 6u;
	krang_pick_move(last_move, object, p_d7, p_a5);
}

/* Krang's lasers, step by step */
void krang_fire_lasers(uint32_t object, uint32_t *p_d5, uint32_t *p_state_offset, uint32_t *p_a5)
{
	uint32_t d5 = 0, state_offset = 0;

	d5 = krang_count_down(d5, object);
	state_offset = OBJECT(object)->state;
	state_offset = (uint16_t)(state_offset << 2);
	switch (state_offset)
	{
	case 0:
		krang_lasers_fire(object, &state_offset, p_a5);
		break;
	case 4:
		krang_lasers_wait(object);
		break;
	case 8:
		krang_lasers_end(object, &state_offset, p_a5);
		break;
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_state_offset = state_offset;
}

/* Krang fires a stream of five lasers */
void krang_lasers_fire(uint32_t object, uint32_t *p_objects_left, uint32_t *p_a5)
{
	uint32_t objects_left = *p_objects_left;
	uint16_t count = 0;
	uint32_t laser = 0;
	uint16_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->state++;
		count = 4u;
		for (;;)
		{
			laser = OBJECT_ADDRESS(32);
			objects_left = (objects_left & ~0xFFu) | 8u;
			objects_left = (objects_left & ~0xFFFFu) | (uint8_t)objects_left;
			find_free_object(&objects_left, &laser);
			if ((uint8_t)objects_left == 0u)
				break;
			*p_a5 = krang_start_laser(object, laser);
			objects_left = count * 2u;
			OBJECT(laser)->krang_laser.delay = objects_left;
			OBJECT(laser)->krang_laser.last = 0u;
			t1 = count - 1u;
			count = t1;
			if (t1 == 0xFFFFu)
			{
				OBJECT(laser)->krang_laser.last |= 0x80u;
				break;
			}
		}
		if ((uint8_t)count != 4u)
		{
			objects_left = (objects_left & ~0xFFFFu) | 0x1Bu;
			queue_sound((uint8_t)objects_left);
		}
	}
	*p_objects_left = objects_left;
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
void krang_lasers_end(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
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
		krang_pick_move(last_move, object, p_d7, p_a5);
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
void krang_recoil(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
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
	krang_pick_move(last_move, object, p_d7, p_a5);
}

/* Krang's end, step by step */
void krang_explode(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	*p_d7 = OBJECT(object)->state;
	*p_d7 = (uint16_t)(*p_d7 << 2);
	switch (*p_d7)
	{
	case 0:
		krang_explode_start(object);
		return;
	case 4:
		krang_explode_wait(object);
		return;
	case 8:
		krang_explode_burst(object, p_d7, p_a5);
		return;
	case 12:
		*p_d7 = krang_explode_next(*p_d7, object);
		return;
	}
}

/* Krang stops, with his shadow, before he bursts */
void krang_explode_start(uint32_t object)
{
	OBJECT(object)->krang_explode.timer = 0x8Cu;
	OBJECT(object)->shadow = 0xF66Cu;
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
void krang_explode_burst(uint32_t object, uint32_t *p_objects_left, uint32_t *p_a5)
{
	uint32_t objects_left = *p_objects_left;
	uint32_t explosion = 0;

	if (OBJECT(object)->frame_time == 0u)
		objects_left = krang_hover(object);
	OBJECT(object)->krang_explode.timer = 0xEu;
	OBJECT(object)->state = 3u;
	explosion = OBJECT_ADDRESS(24);
	objects_left = (objects_left & ~0xFFu) | 8u;
	objects_left = (uint8_t)objects_left;
	find_free_object(&objects_left, &explosion);
	if ((uint8_t)objects_left != 0u)
		boss_start_explosion(object, explosion, &objects_left, p_a5);
	else
	{
		OBJECT(object)->routine++;
		OBJECT(object)->state = 0u;
	}
	*p_objects_left = objects_left;
}

/* Unused: an empty routine that returns at once */
void unreached_00E364(void)
{
}

/* the next of Krang's ten explosions */
uint32_t krang_explode_next(uint32_t d7, uint32_t object)
{
	uint8_t timer = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->contacts |= 0x80u;
		d7 = krang_hover(object);
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
	return d7;
}

/* Krang's escape, step by step */
void krang_escape(uint16_t routine_offset, uint32_t object, uint32_t *p_d5, uint32_t *p_state_offset)
{
	uint32_t d5 = *p_d5;
	uint32_t state_offset = 0;

	state_offset = OBJECT(object)->state;
	state_offset = (uint16_t)(state_offset << 2);
	switch (state_offset)
	{
	case 0:
		krang_escape_start(routine_offset, object, &d5, &state_offset);
		break;
	case 4:
		state_offset = krang_escape_wait(state_offset, object);
		break;
	case 8:
		krang_escape_dip(object);
		break;
	case 12:
		krang_escape_fly(object);
		break;
	case 16:
		krang_gone(object);
		break;
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_state_offset = state_offset;
}

/* Krang's head comes out in his pod, saying he is invincible */
void krang_escape_start(uint16_t routine_offset, uint32_t object, uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d5 = *p_d5;
	uint16_t on_right = 0, screen_x = 0, screen_y = 0;
	uint32_t animation = 0;

	animation = krang_animations[10];
	set_animation(0u, object, animation);
	*p_d7 = 0xB0u;
	queue_sound((uint8_t)*p_d7);
	on_right = (uint16_t)((routine_offset & ~0xFFu) | 0xFFu);
	screen_y = OBJECT(object)->screen_y;
	screen_x = (uint16_t)(OBJECT(object)->screen_x + 0x10u);
	say_im_invincible(on_right, screen_x, screen_y, &d5, p_d7);
	OBJECT(object)->state++;
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
}

/* the pod floats a while, then takes off */
uint32_t krang_escape_wait(uint32_t state_offset, uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return krang_hover(object);
	animation = krang_animations[11];
	set_animation(0u, object, animation);
	OBJECT(object)->height_speed = 0x280;
	OBJECT(object)->state++;
	return state_offset;
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
uint32_t krang_hover(uint32_t object)
{
	uint32_t height = 0, hover_offset = 0;

	hover_offset = frame_count;
	hover_offset = hover_offset & 0x3Eu;
	height = krang_hover_heights[hover_offset / 2u];
	height = (uint16_t)(height + 0x4000u);
	OBJECT(object)->height = height;
	return height;
}

/* picks Krang's next walk, charge or pose by its chances */
void krang_pick_move(uint32_t last_move, uint32_t object, uint32_t *p_roll, uint32_t *p_a5)
{
	uint16_t count = 0;
	uint32_t a5 = 0, chances = 0;
	uint8_t move_chance = 0, t1 = 0;
	uint16_t t2 = 0;

	*p_roll = 0x10u;
	*p_roll = random_below((uint16_t)*p_roll, object);
	count = 3u;
	chances = krang_move_chances_ADDRESS + (uint16_t)(last_move << 2);
	for (;;)
	{
		move_chance = MOVE_CHANCE(chances + (int16_t)count);
		t1 = *p_roll;
		*p_roll = (*p_roll & ~0xFFu) | (uint8_t)(t1 - move_chance);
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
		*p_a5 = a5;
		krang_start_walk(object, p_roll, p_a5);
		return;
	case 4:
		*p_roll = krang_start_walk_on(object);
		return;
	case 8:
		*p_roll = krang_start_charge(object);
		return;
	case 12:
		krang_start_pose(object);
		return;
	}
}

/* Krang picks a turtle and walks at it; with none, he poses */
void krang_start_walk(uint32_t object, uint32_t *p_picked, uint32_t *p_a5)
{
	uint32_t animation = 0, ignored = 0;

	boss_pick_target(object, &ignored, p_picked, p_a5);
	if ((uint8_t)*p_picked == 0u)
	{
		krang_start_pose(object);
		return;
	}
	krang_posed_last = 0u;
	*p_picked = 8u;
	*p_picked = krang_set_move_time(*p_picked, object);
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 0u;
	animation = krang_animations[3];
	set_animation(0u, object, animation);
}

/* Krang walks on at his target */
uint32_t krang_start_walk_on(uint32_t object)
{
	uint32_t range = 0;

	range = 8u;
	range = krang_set_move_time(range, object);
	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 1u;
	return range;
}

/* Krang runs at his target */
uint32_t krang_start_charge(uint32_t object)
{
	uint32_t range = 0;

	range = 8u;
	range = krang_set_move_time(range, object);
	krang_posed_last = 0u;
	OBJECT(object)->krang.laser_angle = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 2u;
	return range;
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
uint32_t krang_set_move_time(uint32_t range, uint32_t object)
{
	range = random_below((uint16_t)range, object);
	OBJECT(object)->krang.move_time = krang_move_times[range];
	return range;
}

/* aims Krang's run at his target */
void krang_aim_charge(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d7 = 0, direction = 0, speeds = 0;

	boss_direction_to_target(object, &direction, p_a5);
	speeds = 0xE790u;
	d7 = krang_set_speeds((uint16_t)direction, d7, object, speeds);
	*p_d7 = (*p_d7 & 0xFFFF0000u) | (d7 & 0xFFFFu);
}

/* aims Krang's walk at his target */
void krang_aim_walk(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d7 = 0, direction = 0, speeds = 0;

	boss_direction_to_target(object, &direction, p_a5);
	speeds = indexed_table_of_16_bit_values_00E750_ADDRESS;
	d7 = krang_set_speeds((uint16_t)direction, d7, object, speeds);
	*p_d7 = (*p_d7 & 0xFFFF0000u) | (d7 & 0xFFFFu);
}

/* Krang's speeds in the direction picked, and he turns that way */
uint32_t krang_set_speeds(uint16_t direction, uint32_t d7, uint32_t object, uint32_t speeds)
{
	uint32_t x_speed = 0, y_speed = 0;

	direction = (uint16_t)((uint32_t)direction << 2);
	x_speed = (d7 & ~0xFFFFu) | KRANG_SPEED(speeds + (direction / 2u) * 0x2u);
	OBJECT(object)->x_speed = x_speed;
	y_speed = (x_speed & ~0xFFFFu) | KRANG_SPEED(speeds + ((int32_t)(direction / 2u + 1u)) * 0x2u);
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->flags &= 0xBFu;
	if (OBJECT(object)->x_speed >= 0)
		OBJECT(object)->flags |= 0x40u;
	return y_speed;
}

/* Krang can be hit again after a while; weak, he flashes faster and faster */
uint32_t krang_count_down(uint32_t d5, uint32_t object)
{
	uint8_t colour_2 = 0, flash_mask = 0;
	uint32_t colour = 0;
	uint8_t hit_delay = 0;

	colour = (d5 & ~0xFFu) | 6u;
	colour_2 = 0xFFu;
	if (OBJECT(object)->krang.hit_delay != 0u)
	{
		hit_delay = OBJECT(object)->krang.hit_delay;
		OBJECT(object)->krang.hit_delay = hit_delay - 1u;
		if (hit_delay == 1u)
			OBJECT(object)->flags |= 2u;
	}
	if (OBJECT(object)->krang.hits_left >= 0x1Eu)
		return colour;
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
	return colour;
}

/* Krang moves, kept on the floor and on the screen */
uint32_t krang_move(uint32_t object)
{
	uint32_t d7 = 0;

	d7 = move_by_speeds(object);
	if (OBJECT(object)->y >= 0x8000u)
		OBJECT(object)->y = 0x8000u;
	if (OBJECT(object)->y < 0x1000u)
		OBJECT(object)->y = 0x1000u;
	if ((int16_t)OBJECT(object)->screen_x < 0)
		OBJECT(object)->x += 0x800u;
	return d7;
}

/* which turtle is close to Krang, and on which side */
void krang_find_close_turtle(uint32_t object, uint32_t *p_close_turtle, uint32_t *p_d5)
{
	uint16_t d5 = (uint16_t)*p_d5;
	uint16_t count = 0;
	uint32_t close_turtle = 0, object_2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	close_turtle = 0u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_knocked_out & shl32(1u, (int)(count & 7u))) == 0u && OBJECT(object_2)->routine != 4u)
		{
			t1 = OBJECT(object_2)->screen_y - OBJECT(object)->screen_y;
			d5 = t1;
			if ((int16_t)t1 < 0)
				d5 = (uint16_t)~d5;
			if (d5 < 0xBu)
			{
				t2 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
				d5 = t2;
				if ((int16_t)t2 < 0)
				{
					close_turtle = close_turtle | 0x80u;
					d5 = (uint16_t)~d5;
				}
				if (d5 < 0x3Au)
				{
					close_turtle |= shl32(1u, (int)(count & 0x1Fu));
					break;
				}
			}
		}
		close_turtle = 0u;
		t3 = count - 1u;
		count = t3;
		object_2 -= 0x50u;
	} while (t3 != 0xFFFFu);
	*p_close_turtle = close_turtle;
	*p_d5 = (*p_d5 & 0xFFFF0000u) | d5;
}

/* which turtle is level with Krang and in front of him, far enough */
void krang_find_turtle_in_front(uint32_t object, uint32_t *p_d5, uint32_t *p_turtle_in_front)
{
	uint16_t d5 = (uint16_t)*p_d5;
	uint16_t count = 0;
	uint32_t object_2 = 0, turtle_in_front = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	turtle_in_front = 0u;
	for (;;)
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) == 0u || (players_knocked_out & shl32(1u, (int)(count & 7u))) != 0u || OBJECT(object_2)->routine == 4u)
		{
			t1 = count - 1u;
			count = t1;
			object_2 -= 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		t2 = OBJECT(object_2)->screen_y - OBJECT(object)->screen_y;
		d5 = t2;
		if ((int16_t)t2 < 0)
			d5 = (uint16_t)~d5;
		if (d5 >= 0xBu)
		{
			t1 = count - 1u;
			count = t1;
			object_2 -= 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if ((OBJECT(object)->flags & 0x40u) != 0u)
		{
			t3 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
			d5 = t3;
			if ((int16_t)t3 < 0 || d5 < 0x3Eu)
			{
				t1 = count - 1u;
				count = t1;
				object_2 -= 0x50u;
				if (t1 == 0xFFFFu)
					break;
				continue;
			}
			turtle_in_front |= shl32(1u, (int)(count & 0x1Fu));
			break;
		}
		t4 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
		d5 = t4;
		if ((int16_t)t4 < 0)
		{
			d5 = (uint16_t)~d5;
			if (d5 >= 0x3Eu)
			{
				turtle_in_front |= shl32(1u, (int)(count & 0x1Fu));
				break;
			}
		}
		t1 = count - 1u;
		count = t1;
		object_2 -= 0x50u;
		if (t1 == 0xFFFFu)
			break;
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | d5;
	*p_turtle_in_front = turtle_in_front;
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
uint32_t krang_start_laser(uint32_t object, uint32_t laser)
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
	return laser_places;
}

/* a laser of Krang's stream, at its angle */
uint32_t krang_laser_start(uint32_t object)
{
	uint16_t angle_offset = 0;
	uint32_t angle = 0, animation = 0, stats = 0;

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
	angle = OBJECT(object)->parameter;
	angle = (uint16_t)(angle << 2);
	animation = krang_laser_animations[angle / 4u];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	return angle;
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
uint32_t krang_laser_fly(uint32_t object)
{
	uint32_t animation = 0, d7 = 0;

	d7 = move_by_speeds(object);
	if ((int16_t)OBJECT(object)->height >= 0)
		return d7;
	if (OBJECT(object)->krang_laser.last != 0u)
	{
		OBJECT(object)->height = 0u;
		OBJECT(object)->routine++;
		animation = krang_laser_animations[3];
		set_animation(0u, object, animation);
		return d7;
	}
	remove_object(object);
	return d7;
}

/* the laser's spark ends and it is gone */
void krang_laser_spark(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	remove_object(object);
}

/* Krang's fist flies off his arm */
uint32_t krang_start_fist(uint32_t state_offset, uint32_t object)
{
	uint32_t fist = 0, fist_offset = 0, objects_left = 0;

	fist = OBJECT_ADDRESS(24);
	objects_left = (state_offset & ~0xFFu) | 8u;
	objects_left = (uint8_t)objects_left;
	find_free_object(&objects_left, &fist);
	OBJECT(fist)->flags = 1u;
	OBJECT(fist)->contacts = 0u;
	OBJECT(fist)->kind = 0x66u;
	OBJECT(fist)->parameter = 0u;
	OBJECT(fist)->colour = 0xFFu;
	OBJECT(fist)->animation_delay = 0u;
	place_at_object(object, fist);
	OBJECT(fist)->krang_fist.flipped = 0u;
	fist_offset = (objects_left & ~0xFFFFu) | 0x4500u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		fist_offset = (int16_t)~fist_offset;
		OBJECT(fist)->krang_fist.flipped |= 0x80u;
	}
	OBJECT(fist)->x += fist_offset;
	OBJECT(fist)->y -= 0x200u;
	OBJECT(fist)->height = 0x3200u;
	OBJECT(fist)->krang_fist.shooter = object;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(fist)->routine = 0u;
	OBJECT(fist)->state = 0u;
	return fist_offset;
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
uint32_t krang_fist_fly(uint32_t d7, uint32_t object)
{
	uint32_t x_speed = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		krang_fist_flying = 0u;
		remove_object(object);
		return d7;
	}
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	return x_speed;
}
