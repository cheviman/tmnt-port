/**
 *    Name:        objects/rock_soldiers.c
 *    Description: the two rock soldier bosses (areas 7 and 8), their shots, bars and rubble
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the building site's rock soldier comes in, by the state of his way in */
void rock_soldier_site_enter(uint16_t routine_offset, uint32_t object, uint32_t *p_d5, uint32_t *p_state_offset, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t state_offset = 0;

	state_offset = OBJECT(object)->state;
	state_offset = (uint16_t)(state_offset << 2);
	switch (state_offset)
	{
	case 0:
		state_offset = rock_soldier_site_start(object);
		break;
	case 4:
		rock_soldier_site_enter_wait_for_scroll(object);
		break;
	case 8:
		rock_soldier_site_enter_wait(object);
		break;
	case 12:
		rock_soldier_site_enter_drop(routine_offset, object, &d5, &state_offset);
		break;
	case 16:
		rock_soldier_site_enter_land(object, &state_offset, p_a5);
		break;
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_state_offset = state_offset;
}

/* the rock soldier waits high up, his hits set by the players in play; the area is not cleared */
uint32_t rock_soldier_site_start(uint32_t object)
{
	uint16_t count = 0;
	uint32_t animation = 0, players = 0, stats = 0;
	uint16_t t1 = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = rock_soldier_site_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->rock_soldier.hits_left = OBJECT(object)->health_bars;
	players = 0u;
	count = 3u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
			players = (uint8_t)(players + 1u);
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->rock_soldier.players = players;
	players = (uint16_t)(players << 1);
	OBJECT(object)->rock_soldier.hits_left += players;
	animation = rock_soldier_site_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->rock_soldier.hurt = 0u;
	OBJECT(object)->rock_soldier.hit_delay = 0u;
	OBJECT(object)->rock_soldier.blows = 0u;
	OBJECT(object)->state++;
	enemies_on_screen++;
	mousers_break_down = 0u;
	boss_defeated = 0u;
	return players;
}

/* once the scroll reaches him the music fades */
void rock_soldier_site_enter_wait_for_scroll(uint32_t object)
{
	uint8_t sound = 0;

	if (LAYER(LAYER_B)->x >= 0x6E000u)
	{
		sound = 0xFEu;
		music = 0xFEu;
		queue_sound(sound);
		OBJECT(object)->state++;
		OBJECT(object)->rock_soldier.timer = 0x50u;
	}
}

/* after a while he drops */
void rock_soldier_site_enter_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		animation = rock_soldier_site_animations[1];
		set_animation(0u, object, animation);
		OBJECT(object)->y_speed = 0x200;
		OBJECT(object)->height_speed = -0x800;
		OBJECT(object)->state++;
	}
}

/* he drops to the ground, then the boss music and his balloon "Crash the turtles!" */
void rock_soldier_site_enter_drop(uint16_t routine_offset, uint32_t object, uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d5 = *p_d5;
	uint16_t on_right = 0, screen_x = 0, screen_y = 0;
	uint32_t d7 = 0;

	d7 = rock_soldier_site_move(object);
	if (OBJECT(object)->height == 0u)
	{
		d7 = (d7 & ~0xFFFFu) | 0x3Eu;
		queue_sound((uint8_t)d7);
		d7 = (d7 & ~0xFFFFu) | 0x64u;
		music = d7;
		queue_sound((uint8_t)d7);
		on_right = (uint16_t)((routine_offset & ~0xFFu) | 0xFFu);
		screen_x = (uint16_t)(OBJECT(object)->screen_x - 0x20u);
		screen_y = (uint16_t)(OBJECT(object)->screen_y + 0x20u);
		say_crash_the_turtles(on_right, screen_x, screen_y, &d5, &d7);
		OBJECT(object)->rock_soldier.timer = 0x28u;
		OBJECT(object)->state++;
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_d7 = d7;
}

/* the screen shakes from his landing, then he fights */
void rock_soldier_site_enter_land(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t last_move = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 0u)
	{
		OBJECT(object)->flags |= 6u;
		last_move = 0u;
		rock_soldier_site_next_move(last_move, object, p_d7, p_a5);
		return;
	}
	rock_soldier_site_shake_screen();
}

/* Unused: ends the way in, the next routine */
void rock_soldier_site_enter_end(uint32_t object)
{
	OBJECT(object)->rock_soldier.timer = 0u;
	OBJECT(object)->rock_soldier.move_time = 0u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->routine++;
}

/* the screen shakes */
void rock_soldier_site_shake_screen(void)
{
	layer_b_scroll_y = (layer_b_scroll_y & 0xFF00u) | (0u - ((((uint8_t)frame_count & 2u) << 1) + LAYER(LAYER_B)->y_pixel) & 0xFFu);
}

/* he strikes a turtle close by at a chance, else goes on with his move */
void rock_soldier_site_think(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint8_t sound = 0;
	uint16_t reach_offset = 0;
	uint32_t animation = 0, attack_chance = 0, close_turtle = 0, ignored = 0, move_offset = 0, range = 0, state_offset = 0;

	rock_soldier_site_weak_flash(object, &d5, &ignored);
	if (OBJECT(object)->state == 3u || OBJECT(object)->state == 4u)
	{
		state_offset = OBJECT(object)->state;
		state_offset = (uint16_t)(state_offset << 2);
		switch (state_offset)
		{
		case 0:
			rock_soldier_site_walk(object, &d5, &state_offset, p_a5);
			break;
		case 4:
			rock_soldier_site_walk_on(object, &d5, &state_offset, p_a5);
			break;
		case 8:
			rock_soldier_site_stand(object, &d5, &state_offset, p_a5);
			break;
		case 12:
			rock_soldier_site_jump(object);
			break;
		case 16:
			rock_soldier_site_jump_land(object, &d5, &state_offset, p_a5);
			break;
		}
	}
	else
	{
		rock_soldier_find_close_turtle(object, &close_turtle, &d5);
		if ((uint8_t)close_turtle == 0u)
		{
			state_offset = OBJECT(object)->state;
			state_offset = (uint16_t)(state_offset << 2);
			switch (state_offset)
			{
			case 0:
				rock_soldier_site_walk(object, &d5, &state_offset, p_a5);
				break;
			case 4:
				rock_soldier_site_walk_on(object, &d5, &state_offset, p_a5);
				break;
			case 8:
				rock_soldier_site_stand(object, &d5, &state_offset, p_a5);
				break;
			case 12:
				rock_soldier_site_jump(object);
				break;
			case 16:
				rock_soldier_site_jump_land(object, &d5, &state_offset, p_a5);
				break;
			}
		}
		else
		{
			reach_offset = (uint16_t)((uint32_t)OBJECT(object)->rock_soldier.blows << 1);
			if ((int8_t)close_turtle < 0)
			{
				if ((OBJECT(object)->flags & 0x40u) == 0u)
					reach_offset = (uint16_t)((reach_offset & ~0xFFu) | (uint8_t)(reach_offset + 1u));
			}
			else if ((OBJECT(object)->flags & 0x40u) != 0u)
				reach_offset = (uint16_t)((reach_offset & ~0xFFu) | (uint8_t)(reach_offset + 1u));
			attack_chance = rock_soldier_site_attack_reaches[(uint16_t)(reach_offset + ((uint32_t)turtles_in_play << 2))];
			range = 0x40u;
			range = random_below((uint16_t)range, object);
			if (attack_chance <= (uint8_t)range)
			{
				state_offset = OBJECT(object)->state;
				state_offset = (uint16_t)(state_offset << 2);
				switch (state_offset)
				{
				case 0:
					rock_soldier_site_walk(object, &d5, &state_offset, p_a5);
					break;
				case 4:
					rock_soldier_site_walk_on(object, &d5, &state_offset, p_a5);
					break;
				case 8:
					rock_soldier_site_stand(object, &d5, &state_offset, p_a5);
					break;
				case 12:
					rock_soldier_site_jump(object);
					break;
				case 16:
					rock_soldier_site_jump_land(object, &d5, &state_offset, p_a5);
					break;
				}
			}
			else
			{
				OBJECT(object)->rock_soldier.close_turtle = close_turtle;
				OBJECT(object)->rock_soldier.explosions = 0u;
				OBJECT(object)->routine = 3u;
				OBJECT(object)->state = 0u;
				OBJECT(object)->flags &= 0xBFu;
				OBJECT(object)->attack &= 0x7Fu;
				if ((int8_t)OBJECT(object)->rock_soldier.close_turtle >= 0)
				{
					OBJECT(object)->flags |= 0x40u;
					OBJECT(object)->attack |= 0x80u;
				}
				OBJECT(object)->rock_soldier.close_turtle = 0u;
				sound = 8u;
				queue_sound(sound);
				move_offset = turtles_in_play;
				move_offset = (uint16_t)(move_offset << 2);
				move_offset = (move_offset & ~0xFFu) | (uint8_t)(move_offset + OBJECT(object)->rock_soldier.blows);
				state_offset = (move_offset & ~0xFFu) | rock_soldier_site_moves[move_offset];
				animation = TABLE32(rock_soldier_site_animations, state_offset);
				set_animation(attack_chance, object, animation);
				OBJECT(object)->rock_soldier.blows++;
				if (OBJECT(object)->rock_soldier.blows >= 4u)
					OBJECT(object)->rock_soldier.blows = 3u;
			}
		}
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = state_offset;
}

/* he walks at his target, aiming at it now and then */
void rock_soldier_site_walk(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t d7 = *p_d7;
	uint32_t d7_2 = 0, ignored = 0, last_move = 0;
	uint8_t t1 = 0, t2 = 0, move_time = 0;

	d7_2 = (d7 & ~0xFFu) | (uint8_t)frame_count;
	t1 = d7_2 & 0xFu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 == 0u)
		boss_aim_at_target(object, &ignored, &d7_2, p_a5);
	t2 = d7_2 & 1u;
	d7_2 = (d7_2 & ~0xFFu) | t2;
	if (t2 == 0u)
		d7_2 = rock_soldier_site_move(object);
	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 1u;
		rock_soldier_site_move_done(last_move, object, &d5, &d7_2, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = d7_2;
}

/* he walks on at his target */
void rock_soldier_site_walk_on(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t d7 = *p_d7;
	uint32_t d7_2 = 0, ignored = 0, last_move = 0;
	uint8_t t1 = 0, move_time = 0;

	d7_2 = (d7 & ~0xFFu) | (uint8_t)frame_count;
	t1 = d7_2 & 0xFu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 == 0u)
		boss_aim_at_target(object, &ignored, &d7_2, p_a5);
	if ((frame_count & 2u) == 0u)
		d7_2 = rock_soldier_site_move(object);
	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 2u;
		rock_soldier_site_move_done(last_move, object, &d5, &d7_2, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = d7_2;
}

/* he stands a while */
void rock_soldier_site_stand(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t last_move = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time != 0u)
		return;
	last_move = 3u;
	rock_soldier_site_move_done(last_move, object, &d5, p_d7, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* he is in the air until he lands */
void rock_soldier_site_jump(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t t1 = 0;

	t1 = OBJECT(object)->height + OBJECT(object)->height_speed;
	OBJECT(object)->height = t1;
	if ((int16_t)t1 >= 0)
	{
		OBJECT(object)->rock_soldier.move_time--;
		OBJECT(object)->height_speed -= 0x80u;
		return;
	}
	sound = 0x3Fu;
	queue_sound(sound);
	OBJECT(object)->height = 0u;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
}

/* the screen shakes from his landing, then he can be hit again */
void rock_soldier_site_jump_land(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t last_move = 0;
	uint8_t move_time = 0;

	rock_soldier_site_shake_screen();
	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time != 0u)
		return;
	OBJECT(object)->flags |= 2u;
	last_move = 4u;
	rock_soldier_site_move_done(last_move, object, &d5, p_d7, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* a move over: a burst at a turtle in front within reach, else the next move */
void rock_soldier_site_move_done(uint32_t last_move, uint32_t object, uint32_t *p_d5, uint32_t *p_roll, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t animation = 0, range = 0, turtle_ahead = 0;

	rock_soldier_find_turtle_ahead(object, &d5, &turtle_ahead);
	if ((uint8_t)turtle_ahead != 0u)
	{
		OBJECT(object)->rock_soldier.explosions = 0u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.shot_hit = 0u;
		range = 4u;
		range = random_below((uint16_t)range, object);
		OBJECT(object)->rock_soldier.timer = rock_soldier_site_waits[range];
		OBJECT(object)->rock_soldier.move_time = 0x20u;
		animation = rock_soldier_site_animations[4];
		set_animation(0u, object, animation);
	}
	else
		rock_soldier_site_next_move(last_move, object, &range, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_roll = range;
}

/* he fires gun blasts until the burst is over */
void rock_soldier_site_fire(uint32_t object, uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d7 = *p_d7;
	uint32_t animation = 0, blast = 0, blast_time = 0, d5 = 0;
	uint8_t timer = 0, move_time = 0, t1 = 0;

	rock_soldier_site_weak_flash(object, &d5, &d7);
	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 0u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.shot_hit = 0u;
		animation = rock_soldier_site_animations[4];
		set_animation(0u, object, animation);
	}
	else
	{
		move_time = OBJECT(object)->rock_soldier.move_time;
		OBJECT(object)->rock_soldier.move_time = move_time - 1u;
		if (move_time != 0u)
		{
			blast_time = (d7 & ~0xFFu) | OBJECT(object)->rock_soldier.timer;
			t1 = blast_time & 3u;
			d7 = (blast_time & ~0xFFu) | t1;
			if (t1 == 0u)
			{
				blast = OBJECT_ADDRESS(24);
				d7 = (d7 & ~0xFFu) | 8u;
				d7 = (d7 & ~0xFFFFu) | (uint8_t)d7;
				find_free_object(&d7, &blast);
				if ((uint8_t)d7 != 0u)
				{
					OBJECT(blast)->flags = 1u;
					OBJECT(blast)->contacts = 0u;
					OBJECT(blast)->kind = 0x34u;
					OBJECT(blast)->parameter = 0u;
					OBJECT(blast)->colour = 0xFFu;
					OBJECT(blast)->animation_delay = 0u;
					place_at_object(object, blast);
					OBJECT(blast)->gun_blast.flies_left = 0u;
					d7 = (d7 & ~0xFFFFu) | 0x4800u;
					if ((OBJECT(object)->flags & 0x40u) == 0u)
					{
						d7 = (int16_t)~d7;
						OBJECT(blast)->gun_blast.flies_left |= 0x80u;
					}
					OBJECT(blast)->x += d7;
					OBJECT(blast)->height = 0x3000u;
					OBJECT(blast)->gun_blast.shooter = object;
					if ((OBJECT(object)->flags & 0x40u) == 0u)
						OBJECT(blast)->flags |= 0x40u;
					OBJECT(blast)->routine = 0u;
					OBJECT(blast)->state = 0u;
				}
			}
		}
		else
		{
			OBJECT(object)->rock_soldier.move_time = 0u;
			if ((int8_t)OBJECT(object)->rock_soldier.shot_hit < 0)
			{
				OBJECT(object)->routine++;
				OBJECT(object)->state = 0u;
				OBJECT(object)->rock_soldier.shot_hit = 0u;
				animation = rock_soldier_site_animations[4];
				set_animation(0u, object, animation);
			}
			else
			{
				blast_time = (d7 & ~0xFFu) | OBJECT(object)->rock_soldier.timer;
				t1 = blast_time & 3u;
				d7 = (blast_time & ~0xFFu) | t1;
				if (t1 == 0u)
				{
					blast = OBJECT_ADDRESS(24);
					d7 = (d7 & ~0xFFu) | 8u;
					d7 = (d7 & ~0xFFFFu) | (uint8_t)d7;
					find_free_object(&d7, &blast);
					if ((uint8_t)d7 != 0u)
					{
						OBJECT(blast)->flags = 1u;
						OBJECT(blast)->contacts = 0u;
						OBJECT(blast)->kind = 0x34u;
						OBJECT(blast)->parameter = 0u;
						OBJECT(blast)->colour = 0xFFu;
						OBJECT(blast)->animation_delay = 0u;
						place_at_object(object, blast);
						OBJECT(blast)->gun_blast.flies_left = 0u;
						d7 = (d7 & ~0xFFFFu) | 0x4800u;
						if ((OBJECT(object)->flags & 0x40u) == 0u)
						{
							d7 = (int16_t)~d7;
							OBJECT(blast)->gun_blast.flies_left |= 0x80u;
						}
						OBJECT(blast)->x += d7;
						OBJECT(blast)->height = 0x3000u;
						OBJECT(blast)->gun_blast.shooter = object;
						if ((OBJECT(object)->flags & 0x40u) == 0u)
							OBJECT(blast)->flags |= 0x40u;
						OBJECT(blast)->routine = 0u;
						OBJECT(blast)->state = 0u;
					}
				}
			}
		}
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_d7 = d7;
}

/* the blow, or the gun held up after the burst, plays out; then the next move */
void rock_soldier_site_finish_attack(uint32_t object, uint32_t *p_d5, uint32_t *p_flash_phase, uint32_t *p_a5)
{
	uint32_t d5 = 0, last_move = 0;

	rock_soldier_site_weak_flash(object, &d5, p_flash_phase);
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.hurt = 0u;
		last_move = 5u;
		rock_soldier_site_next_move(last_move, object, p_flash_phase, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
}

/* a hit makes the rock soldier flinch, or the last one defeats him */
uint32_t rock_soldier_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation_entry = 0, animations = 0;
	uint8_t hits_left = 0;

	OBJECT(object)->rock_soldier.hurt = 0x80u;
	OBJECT(object)->colour = 0xFFu;
	animations = rock_soldier_site_animations_ADDRESS;
	if (OBJECT(object)->kind != 0x1Eu)
		animations = rock_soldier_technodrome_animations_ADDRESS;
	hits_left = OBJECT(object)->rock_soldier.hits_left;
	OBJECT(object)->rock_soldier.hits_left = hits_left - 1u;
	if (hits_left == 1u)
	{
		animation_entry = ANIMATION_ENTRY(animations + 0x1Cu);
		set_animation(0u, object, animation_entry);
		if (enemies_on_screen == 1u)
		{
			sound = 0u;
			queue_sound(sound);
		}
		sound = 0x88u;
		queue_sound(sound);
		OBJECT(object)->routine += 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.explosions = 0u;
		OBJECT(object)->rock_soldier.timer = 0u;
		OBJECT(object)->rock_soldier.blows = 0u;
		OBJECT(object)->flags &= ~6u;
		return animations;
	}
	OBJECT(object)->health_bars = 0xAu;
	if (OBJECT(object)->rock_soldier.hits_left == 1u)
		OBJECT(object)->health_bars = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->health = 1u;
	OBJECT(object)->rock_soldier.blows = 0u;
	OBJECT(object)->rock_soldier.timer = 0x1Cu;
	OBJECT(object)->flags &= ~2u;
	sound = 0x82u;
	queue_sound(sound);
	animation_entry = ANIMATION_ENTRY(animations + 0x18u);
	set_animation(0u, object, animation_entry);
	return animations;
}

/* the rock soldier gets over the hit and moves on */
void rock_soldier_recover(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t last_move = 0;
	uint8_t timer = 0;

	if (OBJECT(object)->rock_soldier.hits_left < 0x14u)
		OBJECT(object)->colour = 8u;
	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.hurt = 0u;
		OBJECT(object)->rock_soldier.hit_delay = 8u;
		OBJECT(object)->colour = 0xFFu;
		last_move = 6u;
		if (OBJECT(object)->kind == 0x1Eu)
			rock_soldier_site_next_move(last_move, object, p_d7, p_a5);
		else
			rock_soldier_technodrome_next_move(last_move, object, p_d7, p_a5);
	}
}

/* the defeated rock soldier, by the state of his defeat */
void rock_soldier_defeated(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	*p_d7 = OBJECT(object)->state;
	*p_d7 = (uint16_t)(*p_d7 << 2);
	switch (*p_d7)
	{
	case 0:
		rock_soldier_defeated_start(object);
		return;
	case 4:
		rock_soldier_defeated_wait(object);
		return;
	case 8:
		rock_soldier_defeated_explode(object, p_d7, p_a5);
		return;
	case 12:
		rock_soldier_defeated_next_explosion(object);
		return;
	case 16:
		rock_soldier_defeated_crumble(object, p_d7, p_a5);
		return;
	case 20:
		rock_soldier_defeated_stay();
		return;
	}
}

/* the defeated rock soldier stands a while */
void rock_soldier_defeated_start(uint32_t object)
{
	OBJECT(object)->rock_soldier.timer = 0x64u;
	OBJECT(object)->state++;
}

/* he stands until the explosions */
void rock_soldier_defeated_wait(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 1u)
		OBJECT(object)->state++;
}

/* an explosion on him */
void rock_soldier_defeated_explode(uint32_t object, uint32_t *p_objects_left, uint32_t *p_a5)
{
	uint32_t objects_left = *p_objects_left;
	uint32_t explosion = 0;

	OBJECT(object)->rock_soldier.timer = 0xCu;
	OBJECT(object)->state = 3u;
	explosion = OBJECT_ADDRESS(24);
	objects_left = (objects_left & ~0xFFu) | 8u;
	objects_left = (uint8_t)objects_left;
	find_free_object(&objects_left, &explosion);
	if ((uint8_t)objects_left != 0u)
		boss_start_explosion(object, explosion, &objects_left, p_a5);
	else
		OBJECT(object)->state = 4u;
	*p_objects_left = objects_left;
}

/* the next explosion, until ten */
void rock_soldier_defeated_next_explosion(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state--;
		OBJECT(object)->rock_soldier.explosions++;
		if (OBJECT(object)->rock_soldier.explosions == 0xAu)
		{
			OBJECT(object)->state = 4u;
			OBJECT(object)->rock_soldier.timer = 0x96u;
		}
	}
}

/* rubble falls off him; then the boss is down and the area cleared */
void rock_soldier_defeated_crumble(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d7 = *p_d7;
	uint16_t parameter = 0;
	uint32_t rubble = 0;
	uint8_t timer = 0;
	uint16_t t1 = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 0u)
	{
		OBJECT(object)->state = 5u;
		enemies_on_screen--;
		mousers_break_down = 1u;
		boss_defeated = 1u;
	}
	else
	{
		d7 = IN_PAGE(d7, OBJECT(object)->rock_soldier.timer);
		parameter = 3u;
		do
		{
			if ((uint8_t)d7 == TABLE8(rock_soldier_crumble_times, (uint32_t)(int16_t)parameter))
			{
				rubble = OBJECT_ADDRESS(32);
				d7 = IN_PAGE(d7, 8u);
				d7 = (uint8_t)d7;
				find_free_object(&d7, &rubble);
				if ((uint8_t)d7 != 0u)
				{
					OBJECT(rubble)->flags = 1u;
					OBJECT(rubble)->contacts = 0u;
					OBJECT(rubble)->kind = 0x41u;
					OBJECT(rubble)->parameter = 0u;
					OBJECT(rubble)->parameter = (uint8_t)parameter;
					OBJECT(rubble)->colour = 8u;
					OBJECT(rubble)->animation_delay = 0u;
					place_at_object(object, rubble);
					*p_a5 = rubble_place_x_ADDRESS;
					d7 = OBJECT(rubble)->parameter;
					d7 = (uint16_t)(d7 << 1);
					OBJECT(rubble)->x += (uint32_t)(int16_t)rubble_place_x[d7] << 8;
					OBJECT(rubble)->y += (uint32_t)(int16_t)rubble_place_y[d7] << 8;
					OBJECT(rubble)->height = 0x8000u;
					OBJECT(rubble)->routine = 0u;
					OBJECT(rubble)->state = 0u;
					break;
				}
				break;
			}
			t1 = parameter - 1u;
			parameter = t1;
		} while (t1 != 0xFFFFu);
	}
	*p_d7 = d7;
}

/* he stays a heap of rocks */
void rock_soldier_defeated_stay(void)
{
}

/* a turtle close enough to strike, and on which side */
void rock_soldier_find_close_turtle(uint32_t object, uint32_t *p_close_turtle, uint32_t *p_d5)
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
			if (d5 < 0xCu)
			{
				t2 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
				d5 = t2;
				if ((int16_t)t2 < 0)
				{
					close_turtle = close_turtle | 0x80u;
					d5 = (uint16_t)~d5;
				}
				if (d5 < 0x3Cu)
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

/* a turtle in front of him at the range of his weapon */
void rock_soldier_find_turtle_ahead(uint32_t object, uint32_t *p_d5, uint32_t *p_turtle_ahead)
{
	uint16_t d5 = (uint16_t)*p_d5;
	uint16_t count = 0;
	uint32_t object_2 = 0, turtle_ahead = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0;

	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	turtle_ahead = 0u;
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
		if (OBJECT(object)->kind == 0x1Du)
		{
			if ((OBJECT(object)->flags & 0x40u) != 0u)
			{
				t3 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
				d5 = t3;
				if ((int16_t)t3 < 0 || d5 < 0x46u)
				{
					t1 = count - 1u;
					count = t1;
					object_2 -= 0x50u;
					if (t1 == 0xFFFFu)
						break;
					continue;
				}
				turtle_ahead |= shl32(1u, (int)(count & 0x1Fu));
				break;
			}
			t4 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
			d5 = t4;
			if ((int16_t)t4 < 0)
			{
				d5 = (uint16_t)~d5;
				if (d5 >= 0x46u)
				{
					turtle_ahead |= shl32(1u, (int)(count & 0x1Fu));
					break;
				}
			}
			t1 = count - 1u;
			count = t1;
			object_2 -= 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if ((OBJECT(object)->flags & 0x40u) != 0u)
		{
			t5 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
			d5 = t5;
			if ((int16_t)t5 < 0 || d5 >= 0x8Cu)
			{
				t1 = count - 1u;
				count = t1;
				object_2 -= 0x50u;
				if (t1 == 0xFFFFu)
					break;
				continue;
			}
			turtle_ahead |= shl32(1u, (int)(count & 0x1Fu));
			break;
		}
		t6 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
		d5 = t6;
		if ((int16_t)t6 < 0)
		{
			d5 = (uint16_t)~d5;
			if (d5 < 0x8Cu)
			{
				turtle_ahead |= shl32(1u, (int)(count & 0x1Fu));
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
	*p_turtle_ahead = turtle_ahead;
}

/* the next move by chance, by what he did before */
void rock_soldier_site_next_move(uint32_t last_move, uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t count = 0;
	uint32_t a5 = 0, chances = 0;
	uint8_t move_chance = 0, t1 = 0;
	uint16_t t2 = 0;

	*p_d7 = 0x10u;
	*p_d7 = random_below((uint16_t)*p_d7, object);
	count = 3u;
	chances = table_loaded_by_lea_011C42_ADDRESS + (uint16_t)(last_move << 2);
	for (;;)
	{
		move_chance = MOVE_CHANCE(chances + (int16_t)count);
		t1 = *p_d7;
		*p_d7 = (uint8_t)(t1 - move_chance);
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
		rock_soldier_site_walk_start(object, p_d7, p_a5);
		return;
	case 4:
		*p_d7 = rock_soldier_site_walk_on_start(object);
		return;
	case 8:
		*p_d7 = rock_soldier_site_stand_start(object);
		return;
	case 12:
		*p_d7 = rock_soldier_site_jump_start(object);
		return;
	}
}

/* he picks a turtle and walks at it, or stands if there is none */
void rock_soldier_site_walk_start(uint32_t object, uint32_t *p_range, uint32_t *p_a5)
{
	uint32_t animation = 0, ignored = 0, picked = 0;

	boss_pick_target(object, &ignored, &picked, p_a5);
	if ((uint8_t)picked == 0u)
	{
		*p_range = rock_soldier_site_stand_start(object);
		return;
	}
	*p_range = 8u;
	*p_range = rock_soldier_site_random_move_time(*p_range, object);
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 0u;
	animation = rock_soldier_site_animations[2];
	set_animation(0u, object, animation);
}

/* he walks on as he was */
uint32_t rock_soldier_site_walk_on_start(uint32_t object)
{
	uint32_t range = 0;

	range = 8u;
	range = rock_soldier_site_random_move_time(range, object);
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 1u;
	return range;
}

/* he stands a while */
uint32_t rock_soldier_site_stand_start(uint32_t object)
{
	uint32_t animation = 0, range = 0;

	range = 4u;
	range = rock_soldier_site_random_move_time(range, object);
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 2u;
	animation = rock_soldier_site_animations[3];
	set_animation(0u, object, animation);
	return range;
}

/* he jumps where he stands, now and then with "Crash the turtles!" */
uint32_t rock_soldier_site_jump_start(uint32_t object)
{
	uint32_t animation = 0, range = 0;

	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->rock_soldier.move_time = 0x28u;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 3u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->shadow = rock_soldier_shadow_ADDRESS;
	range = 4u;
	range = random_below((uint16_t)range, object);
	if ((uint8_t)range == 0u)
	{
		range = 0xB1u;
		queue_sound((uint8_t)range);
	}
	animation = rock_soldier_site_animations[1];
	set_animation(0u, object, animation);
	return range;
}

/* how long the move lasts, at random */
uint32_t rock_soldier_site_random_move_time(uint32_t range, uint32_t object)
{
	range = random_below((uint16_t)range, object);
	OBJECT(object)->rock_soldier.move_time = rock_soldier_site_move_times[range];
	return range;
}

/* his flashing as he weakens, in his colours */
void rock_soldier_technodrome_weak_flash(uint32_t object, uint32_t *p_d5, uint32_t *p_flash_phase)
{
	uint8_t colour = 0, colour_2 = 0;

	colour = 8u;
	colour_2 = 0xEu;
	*p_flash_phase = rock_soldier_weak_flash(colour, colour_2, *p_flash_phase, object);
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | colour;
}

/* his flashing as he weakens, in his colours */
void rock_soldier_site_weak_flash(uint32_t object, uint32_t *p_d5, uint32_t *p_flash_phase)
{
	uint8_t colour = 0, colour_2 = 0;

	colour = 8u;
	colour_2 = 0xDu;
	*p_flash_phase = rock_soldier_weak_flash(colour, colour_2, *p_flash_phase, object);
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | colour;
}

/* he can be hit again after a while, and flashes more as he weakens */
uint32_t rock_soldier_weak_flash(uint8_t colour, uint8_t colour_2, uint32_t flash_phase, uint32_t object)
{
	uint32_t flash_mask = 0;
	uint8_t hit_delay = 0, t1 = 0;

	if (OBJECT(object)->rock_soldier.hit_delay != 0u)
	{
		hit_delay = OBJECT(object)->rock_soldier.hit_delay;
		OBJECT(object)->rock_soldier.hit_delay = hit_delay - 1u;
		if (hit_delay == 1u)
			OBJECT(object)->flags |= 2u;
	}
	if (OBJECT(object)->rock_soldier.hits_left >= 0x1Cu)
		return flash_phase;
	flash_mask = 0x1Cu;
	if (OBJECT(object)->rock_soldier.hits_left < 0x14u)
	{
		flash_mask = 8u;
		if (OBJECT(object)->rock_soldier.hits_left < 0xAu)
			flash_mask = 1u;
	}
	OBJECT(object)->colour = colour_2;
	t1 = flash_mask & (uint8_t)frame_count;
	flash_phase = t1;
	if (t1 == 0u)
		OBJECT(object)->colour = colour;
	return flash_phase;
}

/* he moves by his speeds within the area's depth */
uint32_t rock_soldier_site_move(uint32_t object)
{
	uint32_t d7 = 0;

	d7 = move_by_speeds(object);
	if (OBJECT(object)->y >= 0x7800u)
		OBJECT(object)->y = 0x7800u;
	if (OBJECT(object)->y < 0x1000u)
		OBJECT(object)->y = 0x1000u;
	return d7;
}

/* a gun blast flies the way the rock soldier faces */
void gun_blast_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x84u;
	stats = gun_blast_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->x_speed = 0x400;
	if ((int8_t)OBJECT(object)->gun_blast.flies_left < 0)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->attack &= 0x7Fu;
		OBJECT(object)->x_speed = ~OBJECT(object)->x_speed;
	}
	animation = gun_blast_animations[0];
	set_animation(0u, object, animation);
	sound = 6u;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the blast swells as it flies, and tells its rock soldier when it hits a turtle */
uint32_t gun_blast_fly(uint32_t d7, uint32_t object)
{
	uint32_t shooter = 0, x_speed = 0;

	if (OBJECT(object)->frame_time == 0u || (OBJECT(object)->flags & 0x20u) != 0u)
	{
		remove_object(object);
		return d7;
	}
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		shooter = OBJECT(object)->gun_blast.shooter;
		OBJECT(shooter)->rock_soldier.shot_hit = 0x80u;
	}
	return x_speed;
}

/* a piece of rock falls off the defeated rock soldier */
uint32_t rubble_start(uint32_t object)
{
	uint32_t animation = 0, piece = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = -0x600;
	piece = OBJECT(object)->parameter;
	piece = (uint16_t)(piece << 2);
	animation = rubble_animations[piece / 4u];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	return piece;
}

/* the piece falls to the ground */
uint32_t rubble_fall(uint32_t object)
{
	uint32_t height_speed = 0;

	height_speed = move_by_speeds(object);
	if ((int16_t)OBJECT(object)->height < 0)
	{
		OBJECT(object)->height = 0u;
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->height_speed = 0;
		height_speed = (height_speed & ~0xFFFFu) | 0x31u;
		queue_sound((uint8_t)height_speed);
		OBJECT(object)->routine++;
	}
	return height_speed;
}

/* the piece stays on the ground */
void rubble_stay(void)
{
}

/* the Technodrome's rock soldier comes in, by the state of his way in */
void rock_soldier_technodrome_enter(uint32_t object, uint32_t *p_state_offset, uint32_t *p_a5)
{
	*p_state_offset = OBJECT(object)->state;
	*p_state_offset = (uint16_t)(*p_state_offset << 2);
	switch (*p_state_offset)
	{
	case 0:
		*p_state_offset = rock_soldier_technodrome_start(object);
		return;
	case 4:
		rock_soldier_technodrome_enter_wait_for_scroll(object);
		return;
	case 8:
		rock_soldier_technodrome_enter_wait(object);
		return;
	case 12:
		rock_soldier_technodrome_enter_appear(object, p_state_offset, p_a5);
		return;
	}
}

/* the rock soldier waits unseen, his hits set by the players in play; the area is not cleared */
uint32_t rock_soldier_technodrome_start(uint32_t object)
{
	uint16_t count = 0;
	uint32_t animation = 0, players = 0, stats = 0;
	uint16_t t1 = 0;

	stats = rock_soldier_technodrome_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->rock_soldier.hits_left = OBJECT(object)->health_bars;
	players = 0u;
	count = 3u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
			players = (uint8_t)(players + 1u);
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->rock_soldier.players = players;
	OBJECT(object)->rock_soldier.hits_left += players;
	animation = rock_soldier_technodrome_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->rock_soldier.hurt = 0u;
	OBJECT(object)->rock_soldier.hit_delay = 0u;
	OBJECT(object)->rock_soldier.blows = 0u;
	OBJECT(object)->state++;
	enemies_on_screen++;
	missile_out = 0u;
	mousers_break_down = 0u;
	boss_defeated = 0u;
	return players;
}

/* once the scroll reaches him the music fades */
void rock_soldier_technodrome_enter_wait_for_scroll(uint32_t object)
{
	uint8_t sound = 0;

	if (LAYER(LAYER_B)->x >= 0xD1000u)
	{
		sound = 0xFEu;
		music = 0xFEu;
		queue_sound(sound);
		OBJECT(object)->state++;
		OBJECT(object)->rock_soldier.timer = 0x50u;
	}
}

/* the boss music starts */
void rock_soldier_technodrome_enter_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		sound = 0x64u;
		music = 0x64u;
		queue_sound(sound);
		OBJECT(object)->rock_soldier.timer = 0x28u;
		OBJECT(object)->state++;
	}
}

/* he appears and fights, with the wall slab flying off and the shock bars */
void rock_soldier_technodrome_enter_appear(uint32_t object, uint32_t *p_objects_left, uint32_t *p_a5)
{
	uint32_t objects_left = *p_objects_left;
	uint32_t bars = 0, d7 = 0, hit_box = 0, last_move = 0, wall_slab = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->rock_soldier.timer;
	OBJECT(object)->rock_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags |= 0x86u;
		last_move = 0u;
		rock_soldier_technodrome_next_move(last_move, object, &d7, p_a5);
		wall_slab = OBJECT_ADDRESS(24);
		objects_left = (d7 & ~0xFFu) | 8u;
		objects_left = (uint8_t)objects_left;
		find_free_object(&objects_left, &wall_slab);
		if ((uint8_t)objects_left != 0u)
		{
			rock_soldier_technodrome_start_wall_slab(object, wall_slab);
			bars = OBJECT_ADDRESS(24);
			objects_left = (objects_left & ~0xFFu) | 8u;
			objects_left = (objects_left & ~0xFFFFu) | (uint8_t)objects_left;
			find_free_object(&objects_left, &bars);
			if ((uint8_t)objects_left != 0u)
			{
				OBJECT(bars)->flags = 1u;
				OBJECT(bars)->contacts = 0u;
				OBJECT(bars)->kind = 0x4Cu;
				OBJECT(bars)->parameter = 0u;
				OBJECT(bars)->colour = 0xFFu;
				OBJECT(bars)->animation_delay = 0u;
				place_at_object(object, bars);
				OBJECT(bars)->y += 0x8A00u;
				OBJECT(bars)->shock_bars.leader = object;
				OBJECT(bars)->routine = 0u;
				OBJECT(bars)->state = 0u;
				*p_a5 = bars;
				hit_box = OBJECT_ADDRESS(24);
				objects_left = (objects_left & ~0xFFu) | 8u;
				objects_left = (objects_left & ~0xFFFFu) | (uint8_t)objects_left;
				find_free_object(&objects_left, &hit_box);
				if ((uint8_t)objects_left != 0u)
				{
					OBJECT(hit_box)->flags = 1u;
					OBJECT(hit_box)->contacts = 0u;
					OBJECT(hit_box)->kind = 0x4Du;
					OBJECT(hit_box)->parameter = 0u;
					OBJECT(hit_box)->colour = 0xFFu;
					OBJECT(hit_box)->animation_delay = 0u;
					place_at_object(object, hit_box);
					OBJECT(hit_box)->shock_bars.leader = *p_a5;
					OBJECT(hit_box)->routine = 0u;
					OBJECT(hit_box)->state = 0u;
				}
			}
		}
	}
	*p_objects_left = objects_left;
}

/* he strikes a turtle close by at a chance, more often as he weakens, else goes on with his move */
void rock_soldier_technodrome_think(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint8_t sound = 0;
	uint32_t animation = 0, attack_chance = 0, close_turtle = 0, ignored = 0, move_offset = 0, range = 0, reaches = 0, state_offset = 0;

	rock_soldier_technodrome_weak_flash(object, &d5, &ignored);
	if (OBJECT(object)->state == 3u)
	{
		state_offset = OBJECT(object)->state;
		state_offset = (uint16_t)(state_offset << 2);
		switch (state_offset)
		{
		case 0:
			rock_soldier_technodrome_walk(object, &d5, &state_offset, p_a5);
			break;
		case 4:
			rock_soldier_technodrome_walk_on(object, &d5, &state_offset, p_a5);
			break;
		case 8:
			rock_soldier_technodrome_stand(object, &d5, &state_offset, p_a5);
			break;
		case 12:
			rock_soldier_technodrome_taunt(object, &d5, &state_offset, p_a5);
			break;
		}
	}
	else
	{
		rock_soldier_find_close_turtle(object, &close_turtle, &d5);
		if ((uint8_t)close_turtle == 0u)
		{
			state_offset = OBJECT(object)->state;
			state_offset = (uint16_t)(state_offset << 2);
			switch (state_offset)
			{
			case 0:
				rock_soldier_technodrome_walk(object, &d5, &state_offset, p_a5);
				break;
			case 4:
				rock_soldier_technodrome_walk_on(object, &d5, &state_offset, p_a5);
				break;
			case 8:
				rock_soldier_technodrome_stand(object, &d5, &state_offset, p_a5);
				break;
			case 12:
				rock_soldier_technodrome_taunt(object, &d5, &state_offset, p_a5);
				break;
			}
		}
		else
		{
			reaches = rock_soldier_technodrome_reaches_ADDRESS;
			if (OBJECT(object)->rock_soldier.hits_left < 0x14u)
				reaches = rock_soldier_technodrome_reaches_2_ADDRESS;
			attack_chance = REACH(reaches + (OBJECT(object)->rock_soldier.blows + ((uint32_t)turtles_in_play << 2)));
			range = 0x40u;
			range = random_below((uint16_t)range, object);
			if (attack_chance <= (uint8_t)range)
			{
				state_offset = OBJECT(object)->state;
				state_offset = (uint16_t)(state_offset << 2);
				switch (state_offset)
				{
				case 0:
					rock_soldier_technodrome_walk(object, &d5, &state_offset, p_a5);
					break;
				case 4:
					rock_soldier_technodrome_walk_on(object, &d5, &state_offset, p_a5);
					break;
				case 8:
					rock_soldier_technodrome_stand(object, &d5, &state_offset, p_a5);
					break;
				case 12:
					rock_soldier_technodrome_taunt(object, &d5, &state_offset, p_a5);
					break;
				}
			}
			else
			{
				OBJECT(object)->rock_soldier.close_turtle = close_turtle;
				OBJECT(object)->rock_soldier.explosions = 0u;
				OBJECT(object)->routine = 3u;
				OBJECT(object)->state = 0u;
				OBJECT(object)->flags &= 0xBFu;
				OBJECT(object)->attack &= 0x7Fu;
				if ((int8_t)OBJECT(object)->rock_soldier.close_turtle >= 0)
				{
					OBJECT(object)->flags |= 0x40u;
					OBJECT(object)->attack |= 0x80u;
				}
				OBJECT(object)->rock_soldier.close_turtle = 0u;
				sound = 8u;
				queue_sound(sound);
				move_offset = turtles_in_play;
				move_offset = (uint16_t)(move_offset << 2);
				move_offset = (move_offset & ~0xFFu) | (uint8_t)(move_offset + OBJECT(object)->rock_soldier.blows);
				state_offset = (move_offset & ~0xFFu) | rock_soldier_technodrome_moves[move_offset];
				animation = TABLE32(rock_soldier_technodrome_animations, state_offset);
				set_animation(attack_chance, object, animation);
				OBJECT(object)->rock_soldier.blows++;
				if (OBJECT(object)->rock_soldier.blows >= 4u)
					OBJECT(object)->rock_soldier.blows = 3u;
			}
		}
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = state_offset;
}

/* he walks at his target, aiming at it now and then */
void rock_soldier_technodrome_walk(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t d7 = *p_d7;
	uint32_t d7_2 = 0, ignored = 0, last_move = 0;
	uint8_t t1 = 0, t2 = 0, move_time = 0;

	d7_2 = (d7 & ~0xFFu) | (uint8_t)frame_count;
	t1 = d7_2 & 0xFu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 == 0u)
		boss_aim_at_target(object, &ignored, &d7_2, p_a5);
	t2 = d7_2 & 1u;
	d7_2 = (d7_2 & ~0xFFu) | t2;
	if (t2 == 0u)
		d7_2 = rock_soldier_technodrome_move(object);
	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 1u;
		rock_soldier_technodrome_move_done(last_move, object, &d5, &d7_2, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = d7_2;
}

/* he walks on at his target */
void rock_soldier_technodrome_walk_on(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t d7 = *p_d7;
	uint32_t d7_2 = 0, ignored = 0, last_move = 0;
	uint8_t t1 = 0, move_time = 0;

	d7_2 = (d7 & ~0xFFu) | (uint8_t)frame_count;
	t1 = d7_2 & 0xFu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 == 0u)
		boss_aim_at_target(object, &ignored, &d7_2, p_a5);
	if ((frame_count & 2u) == 0u)
		d7_2 = rock_soldier_technodrome_move(object);
	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time == 0u)
	{
		last_move = 2u;
		rock_soldier_technodrome_move_done(last_move, object, &d5, &d7_2, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_d7 = d7_2;
}

/* he stands a while */
void rock_soldier_technodrome_stand(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t last_move = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->rock_soldier.move_time;
	OBJECT(object)->rock_soldier.move_time = move_time - 1u;
	if (move_time != 0u)
		return;
	last_move = 3u;
	rock_soldier_technodrome_move_done(last_move, object, &d5, p_d7, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* he taunts until the animation ends */
void rock_soldier_technodrome_taunt(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t last_move = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	last_move = 4u;
	rock_soldier_technodrome_move_done(last_move, object, &d5, p_d7, p_a5);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* a move over: now and then a missile at a turtle far enough in front, else the next move */
void rock_soldier_technodrome_move_done(uint32_t last_move, uint32_t object, uint32_t *p_d5, uint32_t *p_objects_left, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t animation = 0, missile = 0, objects_left = 0, range = 0, turtle_ahead = 0;

	if (missile_out != 0u)
		rock_soldier_technodrome_next_move(last_move, object, &objects_left, p_a5);
	else
	{
		rock_soldier_find_turtle_ahead(object, &d5, &turtle_ahead);
		if ((uint8_t)turtle_ahead == 0u)
			rock_soldier_technodrome_next_move(last_move, object, &objects_left, p_a5);
		else
		{
			range = 0x10u;
			range = random_below((uint16_t)range, object);
			if ((uint8_t)range < 4u)
			{
				OBJECT(object)->rock_soldier.explosions = 0u;
				OBJECT(object)->routine = 2u;
				OBJECT(object)->state = 0u;
				OBJECT(object)->rock_soldier.shot_hit = 0u;
				missile = OBJECT_ADDRESS(24);
				objects_left = (range & ~0xFFu) | 8u;
				objects_left = (uint8_t)objects_left;
				find_free_object(&objects_left, &missile);
				if ((uint8_t)objects_left == 0u)
					rock_soldier_technodrome_walk_start(object, &objects_left, p_a5);
				else
				{
					rock_soldier_technodrome_start_missile(object, missile, &objects_left, p_a5);
					animation = rock_soldier_technodrome_animations[4];
					set_animation(0u, object, animation);
				}
			}
			else
				rock_soldier_technodrome_next_move(last_move, object, &objects_left, p_a5);
		}
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_objects_left = objects_left;
}

/* the missile's firing plays out, then the next move */
void rock_soldier_technodrome_fire(uint32_t object, uint32_t *p_d5, uint32_t *p_flash_phase, uint32_t *p_a5)
{
	uint32_t d5 = 0, last_move = 0;

	rock_soldier_technodrome_weak_flash(object, &d5, p_flash_phase);
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.hurt = 0u;
		last_move = 5u;
		rock_soldier_technodrome_next_move(last_move, object, p_flash_phase, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
}

/* the blow plays out, then the next move */
void rock_soldier_technodrome_finish_blow(uint32_t object, uint32_t *p_d5, uint32_t *p_flash_phase, uint32_t *p_a5)
{
	uint32_t d5 = 0, last_move = 0;

	rock_soldier_technodrome_weak_flash(object, &d5, p_flash_phase);
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->rock_soldier.hurt = 0u;
		last_move = 5u;
		rock_soldier_technodrome_next_move(last_move, object, p_flash_phase, p_a5);
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
}

/* the next move by chance, by what he did before */
void rock_soldier_technodrome_next_move(uint32_t last_move, uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t count = 0;
	uint32_t a5 = 0, chances = 0;
	uint8_t move_chance = 0, t1 = 0;
	uint16_t t2 = 0;

	*p_d7 = 0x10u;
	*p_d7 = random_below((uint16_t)*p_d7, object);
	count = 3u;
	chances = table_loaded_by_lea_012486_ADDRESS + (uint16_t)(last_move << 2);
	for (;;)
	{
		move_chance = MOVE_CHANCE(chances + (int16_t)count);
		t1 = *p_d7;
		*p_d7 = (uint8_t)(t1 - move_chance);
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
		rock_soldier_technodrome_walk_start(object, p_d7, p_a5);
		return;
	case 4:
		*p_d7 = rock_soldier_technodrome_walk_on_start(object);
		return;
	case 8:
		*p_d7 = rock_soldier_technodrome_stand_start(object);
		return;
	case 12:
		*p_d7 = rock_soldier_technodrome_taunt_start(object);
		return;
	}
}

/* he picks a turtle and walks at it, or taunts if there is none */
void rock_soldier_technodrome_walk_start(uint32_t object, uint32_t *p_range, uint32_t *p_a5)
{
	uint32_t animation = 0, ignored = 0, picked = 0;

	boss_pick_target(object, &ignored, &picked, p_a5);
	if ((uint8_t)picked == 0u)
	{
		*p_range = rock_soldier_technodrome_taunt_start(object);
		return;
	}
	*p_range = 8u;
	*p_range = rock_soldier_technodrome_random_move_time(*p_range, object);
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 0u;
	animation = rock_soldier_technodrome_animations[2];
	set_animation(0u, object, animation);
}

/* he walks on as he was */
uint32_t rock_soldier_technodrome_walk_on_start(uint32_t object)
{
	uint32_t range = 0;

	range = 8u;
	range = rock_soldier_technodrome_random_move_time(range, object);
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 1u;
	return range;
}

/* he stands a while, crouched on his way in */
uint32_t rock_soldier_technodrome_stand_start(uint32_t object)
{
	uint32_t animation = 0, range = 0;

	range = 4u;
	range = rock_soldier_technodrome_random_move_time(range, object);
	animation = rock_soldier_technodrome_animations[0];
	if (OBJECT(object)->routine == 0u)
		animation = rock_soldier_technodrome_animations[1];
	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 2u;
	set_animation(0u, object, animation);
	return range;
}

/* he taunts with a speech */
uint32_t rock_soldier_technodrome_taunt_start(uint32_t object)
{
	uint8_t kept_roll = 0;
	uint32_t animation = 0, range = 0, sound = 0;

	OBJECT(object)->rock_soldier.explosions = 0u;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->state = 3u;
	range = 4u;
	range = random_below((uint16_t)range, object);
	kept_roll = (uint8_t)range;
	sound = 0xA6u;
	if (kept_roll == 0u)
		sound = 0xB1u;
	queue_sound((uint8_t)sound);
	animation = rock_soldier_technodrome_animations[3];
	set_animation(0u, object, animation);
	return sound;
}

/* how long the move lasts, at random */
uint32_t rock_soldier_technodrome_random_move_time(uint32_t range, uint32_t object)
{
	range = random_below((uint16_t)range, object);
	OBJECT(object)->rock_soldier.move_time = rock_soldier_technodrome_move_times[range];
	return range;
}

/* he moves by his speeds within the area's depth */
uint32_t rock_soldier_technodrome_move(uint32_t object)
{
	uint32_t d7 = 0;

	d7 = move_by_speeds(object);
	if (OBJECT(object)->y >= 0xF68000u)
		OBJECT(object)->y = 0xF68000u;
	if (OBJECT(object)->y < 0xF61000u)
		OBJECT(object)->y = 0xF61000u;
	return d7;
}

/* the wall slab flies off where he appears */
void rock_soldier_technodrome_start_wall_slab(uint32_t object, uint32_t slab)
{
	OBJECT(slab)->flags = 1u;
	OBJECT(slab)->contacts = 0u;
	OBJECT(slab)->kind = 0x48u;
	OBJECT(slab)->parameter = 0u;
	OBJECT(slab)->colour = 0xFFu;
	OBJECT(slab)->animation_delay = 0u;
	place_at_object(object, slab);
	OBJECT(slab)->y -= 0x800u;
	OBJECT(slab)->y -= 0x800u;
	OBJECT(slab)->height += 0x400u;
	OBJECT(slab)->count = 0u;
	OBJECT(slab)->routine = 0u;
	OBJECT(slab)->state = 0u;
}

/* a slab of the wall flies off to the left, knocking turtles flying */
void wall_slab_start(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t place = 0;
	uint32_t animation = 0, block = 0, stats = 0;

	OBJECT(object)->flags = 0x85u;
	stats = wall_slab_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = -0x800;
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	animation = wall_slab_animations[0];
	set_animation(0u, object, animation);
	sound = 0x3Eu;
	queue_sound(sound);
	block = wall_slab_tile_block_ADDRESS;
	place = 0x25u;
	draw_tile_block(place, block);
	OBJECT(object)->routine++;
}

/* the slab flies until it leaves the screen */
uint32_t wall_slab_fly(uint32_t d7, uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		remove_object(object);
		return d7;
	}
	return move_by_speeds(object);
}

/* a missile from his launcher, the way he faces */
void rock_soldier_technodrome_start_missile(uint32_t object, uint32_t missile, uint32_t *p_missile_offset, uint32_t *p_shadow)
{
	uint32_t missile_offset = *p_missile_offset;

	OBJECT(missile)->flags = 1u;
	OBJECT(missile)->contacts = 0u;
	OBJECT(missile)->kind = 0x49u;
	OBJECT(missile)->parameter = 0u;
	OBJECT(missile)->colour = 0xFFu;
	OBJECT(missile)->animation_delay = 0u;
	place_at_object(object, missile);
	OBJECT(missile)->missile.flies_left = 0u;
	missile_offset = (missile_offset & ~0xFFFFu) | 0x4000u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		missile_offset = (int16_t)~missile_offset;
		OBJECT(missile)->missile.flies_left |= 0x80u;
	}
	OBJECT(missile)->x += missile_offset;
	OBJECT(missile)->height = 0x3800u;
	OBJECT(missile)->missile.shooter = object;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(missile)->routine = 0u;
	OBJECT(missile)->state = 0u;
	OBJECT(missile)->contacts |= 0x80u;
	*p_shadow = missile_shadow_ADDRESS;
	OBJECT(missile)->shadow = missile_shadow_ADDRESS;
	*p_missile_offset = missile_offset;
}

/* a missile flies the way the rock soldier faces, knocking turtles flying */
void missile_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x86u;
	stats = missile_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->x_speed = 0x300;
	OBJECT(object)->flags |= 0x40u;
	if ((int8_t)OBJECT(object)->missile.flies_left < 0)
	{
		OBJECT(object)->flags &= 0xBFu;
		OBJECT(object)->attack &= 0x7Fu;
		OBJECT(object)->x_speed = ~OBJECT(object)->x_speed;
	}
	animation = missile_animations[0];
	set_animation(0u, object, animation);
	sound = 6u;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the missile flies until it meets a wall or reaches the screen's sides, and explodes */
void missile_fly(uint32_t object, uint32_t *p_d5, uint32_t *p_x_speed, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
	uint32_t x_speed = *p_x_speed;
	uint32_t a6 = 0, animation = 0;

	if (OBJECT(object)->screen_x < 0x60u || OBJECT(object)->screen_x >= 0x180u)
	{
		OBJECT(object)->routine++;
		x_speed = (x_speed & ~0xFFu) | 0x13u;
		queue_sound((uint8_t)x_speed);
		animation = rocket_animations[0];
		set_animation(0u, object, animation);
	}
	else
	{
		LAYER(WORKING_LAYER)->x = LAYER(LAYER_B)->x;
		read_floor_cells_on_screen(object, &d5, p_a5, &a6);
		turtle_walls_right(object, a6);
		if ((OBJECT(object)->contacts & 8u) != 0u)
		{
			OBJECT(object)->routine++;
			x_speed = (x_speed & ~0xFFu) | 0x13u;
			queue_sound((uint8_t)x_speed);
			animation = rocket_animations[0];
			set_animation(0u, object, animation);
		}
		else
		{
			x_speed = OBJECT(object)->x_speed;
			OBJECT(object)->x += x_speed;
		}
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_x_speed = x_speed;
}

/* the explosion ends and the missile is gone */
void missile_explode(uint32_t object)
{
	if (OBJECT(object)->height != 0u)
		OBJECT(object)->height -= 0x400u;
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 1u;
		if (hold_spent_missiles == 0u)
		{
			missile_out = 0u;
			remove_object(object);
			return;
		}
	}
	missile_idle();
}

/* nothing */
void missile_idle(void)
{
}

/* a turtle's attack bursts the missile */
void missile_struck(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->routine++;
	animation = explosion_small_animations[0];
	set_animation(0u, object, animation);
}

/* the burst ends and the missile is gone */
void missile_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		remove_object(object);
		return;
	}
}

/* the shock bars appear, blinking */
void shock_bars_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->shock_bars.blink_period = 0x3Du;
	OBJECT(object)->shock_bars.blink_timer = OBJECT(object)->shock_bars.blink_period;
	OBJECT(object)->routine++;
	animation = shock_bars_animations[0];
	set_animation(0u, object, animation);
}

/* the bars blink faster until they flicker on */
void shock_bars_blink_in(uint32_t object)
{
	shock_bars_blink(object);
	if ((frame_count & 0xFu) == 0u)
	{
		OBJECT(object)->shock_bars.blink_period -= 4u;
		if (OBJECT(object)->shock_bars.blink_period == 1u)
			OBJECT(object)->routine++;
	}
}

/* the bars flicker until their rock soldier is defeated */
void shock_bars_on(uint32_t object)
{
	shock_bars_blink(object);
	if (mem_read8(OBJECT(object)->shock_bars.leader + 4u) == 6u)
		OBJECT(object)->routine++;
}

/* the bars blink slower until they are gone */
void shock_bars_blink_out(uint32_t object)
{
	shock_bars_blink(object);
	if ((frame_count & 7u) != 0u)
		return;
	OBJECT(object)->shock_bars.blink_period++;
	if (OBJECT(object)->shock_bars.blink_period < 0x28u)
		return;
	remove_object(object);
}

/* the bars show now and then */
void shock_bars_blink(uint32_t object)
{
	uint8_t blink_timer = 0;

	OBJECT(object)->flags &= 0x7Fu;
	blink_timer = OBJECT(object)->shock_bars.blink_timer;
	OBJECT(object)->shock_bars.blink_timer = blink_timer - 1u;
	if (blink_timer == 0u)
	{
		OBJECT(object)->shock_bars.blink_timer = OBJECT(object)->shock_bars.blink_period;
		OBJECT(object)->flags |= 0x80u;
	}
}

/* the unseen part of the bars that electrocutes turtles */
void shock_bars_hit_box_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	stats = shock_bars_hit_box_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->routine++;
	animation = shock_bars_animations[0];
	set_animation(0u, object, animation);
}

/* Unused: counts down as the bars blink in */
void shock_bars_hit_box_blink_in(uint32_t object)
{
	if ((frame_count & 0xFu) == 0u)
	{
		OBJECT(object)->shock_bars.blink_period -= 4u;
		if (OBJECT(object)->shock_bars.blink_period == 1u)
			OBJECT(object)->routine++;
	}
}

/* while the bars flicker on it electrocutes turtles now and then; it goes with them */
void shock_bars_hit_box_zap(uint32_t object)
{
	uint8_t leader_routine = 0;

	leader_routine = mem_read8(OBJECT(object)->shock_bars.leader + 4u);
	if (leader_routine == 3u)
	{
		remove_object(object);
		return;
	}
	OBJECT(object)->flags &= ~4u;
	if (leader_routine == 2u && (frame_count & 0xFu) == 0u)
		OBJECT(object)->flags |= 4u;
}
