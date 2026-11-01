/**
 *    Name:        engine/sprites.c
 *    Description: the sprite engine: the objects' animations, their sprites into the K051960, their boxes
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* every sprite of the sprite RAM turned off */
void hide_all_sprites(void)
{
	uint8_t priority = 0;
	uint16_t count = 0;
	uint32_t sprite_place = 0;
	uint16_t t1 = 0;

	sprite_place = SPRITE_RAM;
	count = 0x7Fu;
	priority = 0x4Fu;
	do
	{
		io_write8(sprite_place, priority);
		priority = (priority - 1u) & 0x7Fu;
		t1 = count - 1u;
		count = t1;
		sprite_place += 8u;
	} while (t1 != 0xFFFFu);
}

/* every frame: the objects drawn sorted by their depth on the screen into the draw list */
void build_draw_list(uint32_t *p_count, uint32_t *p_object)
{
	uint8_t drawn_count = 0;
	uint16_t draw_depth = 0, first_slot = 0;
	uint32_t count = 0, draw_list_place = 0, frame = 0, next_object = 0, object = 0, object_2 = 0;
	uint16_t draw_next = 0, t1 = 0;

	count = 0x31u;
	drawn_count = 0u;
	first_slot = 0x300u;
	object = OBJECT_ADDRESS(0);
	draw_list_place = draw_list_ADDRESS;
	next_object = OBJECT_ADDRESS(0);
	do
	{
		if ((int8_t)OBJECT(object)->flags >= 0)
		{
			if (OBJECT(object)->flags != 0u)
			{
				frame = OBJECT(object)->frame;
				step_animation(object, frame);
			}
		}
		else
		{
			OBJECT(object)->draw_next = 0u;
			draw_depth = (uint16_t)(OBJECT(object)->depth + OBJECT(object)->screen_y + 0x38u);
			if (draw_depth >= 0x180u)
			{
				frame = OBJECT(object)->frame;
				step_animation(object, frame);
			}
			else
			{
				drawn_count++;
				draw_depth &= ~1u;
				if (draw_depth < first_slot)
					first_slot = (uint16_t)draw_depth;
				if (RAM16(draw_list_ADDRESS + (uint32_t)(int16_t)draw_depth) == 0u)
					RAM16(draw_list_ADDRESS + (uint32_t)(int16_t)draw_depth) = object;
				else
				{
					next_object = (next_object & ~0xFFFFu) | RAM16(draw_list_ADDRESS + (uint32_t)(int16_t)draw_depth);
					do
					{
						object_2 = next_object;
						draw_next = OBJECT(object_2)->draw_next;
						next_object = (next_object & ~0xFFFFu) | draw_next;
					} while (draw_next != 0u);
					OBJECT(object_2)->draw_next = object;
				}
			}
		}
		t1 = count - 1u;
		count = t1;
		object += 0x50u;
	} while (t1 != 0xFFFFu);
	draw_first = first_slot;
	draw_count = drawn_count;
	*p_count = count;
	*p_object = object;
}

/* every frame: the draw list's objects into the sprite chip with their shadows, their animations stepped
   on, the rest turned off */
void draw_sprites(uint32_t *p_d1, uint32_t *p_object)
{
	uint16_t d1 = (uint16_t)*p_d1;
	uint32_t object = *p_object;
	bool L_047164 = false;
	uint8_t flip = 0, flips = 0, frame_time = 0, object_colour = 0, objects_left = 0;
	uint16_t bottom = 0, count = 0, shadow_y = 0;
	uint32_t draw_list_place = 0, frame = 0, next_object = 0, size = 0, sprite = 0, sprite_place = 0;
	uint8_t t2 = 0, colour = 0, frame_time_2 = 0, time = 0, last_sprite = 0, t5 = 0, t6 = 0;
	uint16_t t1 = 0, t3 = 0, t4 = 0, draw_next = 0;

	draw_list_place = draw_list_ADDRESS + (int16_t)draw_first;
	sprite_place = SPRITE_RAM;
	next_object = OBJECT_ADDRESS(0);
	objects_left = draw_count;
	while (draw_list_place < 0x61580u)
	{
		t1 = mem_read16(draw_list_place);
		next_object = (next_object & ~0xFFFFu) | t1;
		draw_list_place += 2u;
		if (t1 == 0u)
			continue;
		mem_write16(draw_list_place - 2u, 0u);
		do
		{
			object = next_object;
			frame = OBJECT(object)->frame;
			count = ANIMATION_FRAME(frame)->last_sprite;
			sprite = ANIMATION_FRAME(frame)->sprites;
			do
			{
				size = sprite_sizes_ADDRESS + ((SPRITE(sprite)->size_code & 0xE0u) >> 2);
				d1 = (uint16_t)SPRITE(sprite)->x;
				t2 = (uint32_t)OBJECT(object)->flags << 1;
				flip = t2;
				if ((int8_t)t2 < 0)
					d1 = (uint16_t)(0u - d1 - SPRITE_SIZE(size)->width);
				d1 += OBJECT(object)->screen_x;
				if ((int16_t)d1 < 0x1A0 && SPRITE_SIZE(size)->left < (int16_t)d1)
				{
					t3 = OBJECT(object)->height_whole + SPRITE(sprite)->y + OBJECT(object)->screen_y;
					bottom = t3;
					if ((int16_t)t3 >= 0 && (int16_t)bottom < SPRITE_SIZE(size)->bottom)
					{
						d1 &= 0x1FFu;
						flips = SPRITE(sprite)->flips;
						if ((int8_t)(flip ^ flips) < 0)
							d1 |= 0x200u;
						bottom &= 0x1FFu;
						if ((flips & 0x40u) != 0u)
							bottom |= 0x200u;
						io_write8(sprite_place, io_read8(sprite_place) | 0x80u);
						io_write8(sprite_place + 1u, SPRITE(sprite)->size_code);
						io_write8(sprite_place + 2u, SPRITE(sprite)->code);
						colour = OBJECT(object)->colour;
						object_colour = colour;
						if ((int8_t)colour < 0 || SPRITE(sprite)->colour == 0xFu)
							io_write8(sprite_place + 3u, SPRITE(sprite)->colour);
						else
							io_write8(sprite_place + 3u, object_colour | (SPRITE(sprite)->colour & 0x90u));
						io_write16(sprite_place + 6u, d1);
						io_write8(sprite_place + 7u, (uint8_t)d1);
						io_write16(sprite_place + 4u, bottom);
						io_write8(sprite_place + 5u, (uint8_t)bottom);
						sprite_place += 8u;
					}
				}
				t4 = count - 1u;
				count = t4;
				sprite += 8u;
			} while (t4 != 0xFFFFu);
			if ((int8_t)OBJECT(object)->contacts < 0)
			{
				sprite = OBJECT(object)->shadow;
				shadow_y = (uint16_t)(OBJECT(object)->floor_height + SPRITE(sprite)->y + OBJECT(object)->screen_y);
				d1 = (SPRITE(sprite)->x + OBJECT(object)->screen_x) & 0x1FFu;
				if ((int8_t)SPRITE(sprite)->flips < 0)
					d1 |= 0x200u;
				shadow_y &= 0x1FFu;
				if ((SPRITE(sprite)->flips & 0x40u) != 0u)
					shadow_y |= 0x200u;
				io_write8(sprite_place, io_read8(sprite_place) | 0x80u);
				io_write8(sprite_place + 1u, SPRITE(sprite)->size_code);
				io_write8(sprite_place + 2u, SPRITE(sprite)->code);
				io_write8(sprite_place + 3u, SPRITE(sprite)->colour);
				io_write16(sprite_place + 6u, d1);
				io_write8(sprite_place + 7u, (uint8_t)d1);
				io_write16(sprite_place + 4u, shadow_y);
				io_write8(sprite_place + 5u, (uint8_t)shadow_y);
				sprite_place += 8u;
			}
			if (OBJECT(object)->frame_time != 0u)
			{
				frame_time_2 = OBJECT(object)->frame_time;
				OBJECT(object)->frame_time = frame_time_2 - 1u;
				if (frame_time_2 == 1u)
				{
					frame += 0xAu;
					if (ANIMATION_FRAME(frame - 0xAu)->attack_depth >= 0)
						frame += 8u;
					time = ANIMATION_FRAME(frame)->time;
					frame_time = time;
					if (time != 0u)
					{
						OBJECT(object)->frame = frame;
						frame_time += OBJECT(object)->animation_delay;
					}
					else
					{
						last_sprite = ANIMATION_FRAME(frame)->last_sprite;
						d1 = (uint16_t)((d1 & ~0xFFu) | last_sprite);
						if (last_sprite != 0u)
						{
							frame = ANIMATION_FRAME(frame)->sprites;
							frame_time = ANIMATION_FRAME(frame)->time;
							OBJECT(object)->frame = frame;
							frame_time += OBJECT(object)->animation_delay;
						}
					}
					OBJECT(object)->frame_time = frame_time;
				}
			}
			t5 = objects_left;
			objects_left = (uint8_t)(t5 - 1u);
			L_047164 = t5 == 1u;
			if (L_047164)
				break;
			draw_next = OBJECT(object)->draw_next;
			next_object = (next_object & ~0xFFFFu) | draw_next;
		} while (draw_next != 0u);
		if (L_047164)
			break;
	}
	for (;;)
	{
		t6 = io_read8(sprite_place);
		io_write8(sprite_place, t6 & 0x7Fu);
		if ((t6 & 0x80u) == 0u)
			break;
		sprite_place += 8u;
	}
	*p_d1 = (*p_d1 & 0xFFFF0000u) | d1;
	*p_object = object;
}

/* the object's animation stepped on to its next frame when the frame's time runs out, without drawing it */
void step_animation(uint32_t object, uint32_t frame)
{
	uint8_t frame_time = 0;
	uint8_t frame_time_2 = 0, time = 0;

	if (OBJECT(object)->frame_time != 0u)
	{
		frame_time_2 = OBJECT(object)->frame_time;
		OBJECT(object)->frame_time = frame_time_2 - 1u;
		if (frame_time_2 != 1u)
			return;
		frame += 0xAu;
		if (ANIMATION_FRAME(frame - 0xAu)->attack_depth >= 0)
			frame += 8u;
		time = ANIMATION_FRAME(frame)->time;
		frame_time = time;
		if (time != 0u)
		{
			OBJECT(object)->frame = frame;
			frame_time += OBJECT(object)->animation_delay;
			OBJECT(object)->frame_time = frame_time;
			return;
		}
		if (ANIMATION_FRAME(frame)->last_sprite == 0u)
		{
			OBJECT(object)->frame_time = frame_time;
			return;
		}
		frame = ANIMATION_FRAME(frame)->sprites;
		frame_time = ANIMATION_FRAME(frame)->time;
		OBJECT(object)->frame = frame;
		frame_time += OBJECT(object)->animation_delay;
		OBJECT(object)->frame_time = frame_time;
		return;
	}
}

/* every frame: the turtles' and the enemies' attacks against each other, the hits and the damage */
void check_attacks(void)
{
	uint16_t attack_x = 0, count = 0, count_2 = 0, health_bars = 0, height_distance = 0, job = 0, x_distance = 0, y_distance = 0;
	uint32_t attack = 0, box = 0, first_enemy = 0, object = 0, object_2 = 0, turtle = 0, turtle_2 = 0;
	uint8_t t4 = 0, t5 = 0, t12 = 0, t13 = 0, t20 = 0, t21 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, damage = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, damage_2 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, damage_3 = 0, t22 = 0, t23 = 0, t24 = 0;

	first_enemy = OBJECT_ADDRESS(4);
	if ((frame_count & 1u) != 0u)
		first_enemy = OBJECT_ADDRESS(5);
	turtle = OBJECT_ADDRESS(0);
	count = 3u;
	do
	{
		if ((OBJECT(turtle)->flags & 4u) != 0u)
		{
			attack = OBJECT(turtle)->frame;
			if (ATTACK_FRAME(attack)->attack_depth >= 0)
			{
				object_2 = first_enemy;
				count_2 = 0x11u;
				do
				{
					if ((OBJECT(object_2)->flags & 2u) != 0u)
					{
						t1 = OBJECT(turtle)->screen_y - OBJECT(object_2)->screen_y;
						y_distance = t1;
						if ((int16_t)t1 >= 0)
							y_distance = (uint16_t)(0u - y_distance);
						if ((int16_t)(y_distance + ATTACK_FRAME(attack)->attack_depth + OBJECT(object_2)->depth) >= 0)
						{
							box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(object_2)->frame)->body_box;
							attack_x = (uint16_t)ATTACK_FRAME(attack)->attack_x;
							if ((OBJECT(turtle)->flags & 0x40u) != 0u)
								attack_x = (uint16_t)(0u - attack_x);
							t2 = attack_x + OBJECT(turtle)->screen_x - OBJECT(object_2)->screen_x;
							x_distance = t2;
							if ((int16_t)t2 >= 0)
								x_distance = (uint16_t)(0u - x_distance);
							if ((int16_t)(x_distance + ATTACK_FRAME(attack)->attack_width + BODY_BOX(box)->half_width) >= 0)
							{
								t3 = OBJECT(turtle)->height_whole + ATTACK_FRAME(attack)->attack_y - OBJECT(object_2)->height_whole;
								height_distance = t3;
								if ((int16_t)t3 >= 0)
								{
									if ((int16_t)(0u - height_distance + ATTACK_FRAME(attack)->attack_height + BODY_BOX(box)->height) >= 0)
									{
										OBJECT(object_2)->attacker = turtle;
										OBJECT(turtle)->turtle.hit_target = object_2;
										OBJECT(object_2)->hit_routine = (((uint32_t)OBJECT(object_2)->routine << 8u) | OBJECT(object_2)->state);
										OBJECT(object_2)->routine = 4u;
										OBJECT(object_2)->state = 0u;
										OBJECT(turtle)->flags |= 8u;
										OBJECT(object_2)->flags &= ~2u;
										OBJECT(object_2)->hit = OBJECT(turtle)->attack;
										health_bars = (((uint32_t)OBJECT(object_2)->health_bars << 8u) | OBJECT(object_2)->health);
										t4 = (uint8_t)OBJECT(turtle)->damage;
										t5 = (uint8_t)health_bars;
										health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t5 - t4));
										if (t5 <= t4)
										{
											damage = OBJECT(turtle)->damage;
											t6 = (health_bars & ~0xFFu) | (uint8_t)OBJECT(turtle)->damage;
											health_bars = (uint16_t)(t6 - damage);
											if (t6 <= damage)
											{
												job = (uint16_t)(0x400u | TABLE8(attack_hit_jobs, (uint32_t)(int16_t)count));
												queue_job(job);
												health_bars = 0u;
											}
										}
										OBJECT(object_2)->health_bars = (uint8_t)(health_bars >> 8u);
										OBJECT(object_2)->health = (uint8_t)health_bars;
									}
								}
								else if ((int16_t)(height_distance + ATTACK_FRAME(attack)->attack_height) >= 0)
								{
									OBJECT(object_2)->attacker = turtle;
									OBJECT(turtle)->turtle.hit_target = object_2;
									OBJECT(object_2)->hit_routine = (((uint32_t)OBJECT(object_2)->routine << 8u) | OBJECT(object_2)->state);
									OBJECT(object_2)->routine = 4u;
									OBJECT(object_2)->state = 0u;
									OBJECT(turtle)->flags |= 8u;
									OBJECT(object_2)->flags &= ~2u;
									OBJECT(object_2)->hit = OBJECT(turtle)->attack;
									health_bars = (((uint32_t)OBJECT(object_2)->health_bars << 8u) | OBJECT(object_2)->health);
									t4 = (uint8_t)OBJECT(turtle)->damage;
									t5 = (uint8_t)health_bars;
									health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t5 - t4));
									if (t5 <= t4)
									{
										damage = OBJECT(turtle)->damage;
										t6 = (health_bars & ~0xFFu) | (uint8_t)OBJECT(turtle)->damage;
										health_bars = (uint16_t)(t6 - damage);
										if (t6 <= damage)
										{
											job = (uint16_t)(0x400u | TABLE8(attack_hit_jobs, (uint32_t)(int16_t)count));
											queue_job(job);
											health_bars = 0u;
										}
									}
									OBJECT(object_2)->health_bars = (uint8_t)(health_bars >> 8u);
									OBJECT(object_2)->health = (uint8_t)health_bars;
								}
							}
						}
					}
					t7 = count_2 - 1u;
					count_2 = t7;
					object_2 += 0xA0u;
				} while (t7 != 0xFFFFu);
			}
		}
		t8 = count - 1u;
		count = t8;
		turtle += 0x50u;
	} while (t8 != 0xFFFFu);
	object = first_enemy;
	count = 0x11u;
	do
	{
		if ((OBJECT(object)->flags & 4u) != 0u)
		{
			attack = OBJECT(object)->frame;
			if (ATTACK_FRAME(attack)->attack_depth >= 0)
			{
				turtle_2 = OBJECT_ADDRESS(0);
				count_2 = 3u;
				do
				{
					if ((OBJECT(turtle_2)->flags & 2u) != 0u)
					{
						t9 = OBJECT(object)->screen_y - OBJECT(turtle_2)->screen_y;
						y_distance = t9;
						if ((int16_t)t9 >= 0)
							y_distance = (uint16_t)(0u - y_distance);
						if ((int16_t)(y_distance + ATTACK_FRAME(attack)->attack_depth + OBJECT(turtle_2)->depth) >= 0)
						{
							box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(turtle_2)->frame)->body_box;
							attack_x = (uint16_t)ATTACK_FRAME(attack)->attack_x;
							if ((OBJECT(object)->flags & 0x40u) != 0u)
								attack_x = (uint16_t)(0u - attack_x);
							t10 = attack_x + OBJECT(object)->screen_x - OBJECT(turtle_2)->screen_x;
							x_distance = t10;
							if ((int16_t)t10 >= 0)
								x_distance = (uint16_t)(0u - x_distance);
							if ((int16_t)(x_distance + ATTACK_FRAME(attack)->attack_width + BODY_BOX(box)->half_width) >= 0)
							{
								t11 = OBJECT(object)->height_whole + ATTACK_FRAME(attack)->attack_y - OBJECT(turtle_2)->height_whole;
								height_distance = t11;
								if ((int16_t)t11 >= 0)
								{
									if ((int16_t)(0u - height_distance + ATTACK_FRAME(attack)->attack_height + BODY_BOX(box)->height) >= 0)
									{
										OBJECT(turtle_2)->flags &= ~2u;
										OBJECT(turtle_2)->hit_routine = (((uint32_t)OBJECT(turtle_2)->routine << 8u) | OBJECT(turtle_2)->state);
										OBJECT(turtle_2)->routine = 4u;
										OBJECT(turtle_2)->state = 0u;
										OBJECT(turtle_2)->turtle.hit_by = object;
										OBJECT(object)->flags |= 8u;
										OBJECT(object)->hit_object = turtle_2;
										OBJECT(turtle_2)->hit = OBJECT(object)->attack;
										health_bars = (((uint32_t)OBJECT(turtle_2)->health_bars << 8u) | OBJECT(turtle_2)->health);
										t12 = (uint8_t)OBJECT(object)->damage;
										t13 = (uint8_t)health_bars;
										health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t13 - t12));
										if (t13 <= t12)
										{
											damage_2 = OBJECT(object)->damage;
											t14 = (health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage;
											health_bars = (uint16_t)(t14 - damage_2);
											if (t14 < damage_2)
												health_bars = 0u;
										}
										OBJECT(turtle_2)->health_bars = (uint8_t)(health_bars >> 8u);
										OBJECT(turtle_2)->health = (uint8_t)health_bars;
										break;
									}
								}
								else if ((int16_t)(height_distance + ATTACK_FRAME(attack)->attack_height) >= 0)
								{
									OBJECT(turtle_2)->flags &= ~2u;
									OBJECT(turtle_2)->hit_routine = (((uint32_t)OBJECT(turtle_2)->routine << 8u) | OBJECT(turtle_2)->state);
									OBJECT(turtle_2)->routine = 4u;
									OBJECT(turtle_2)->state = 0u;
									OBJECT(turtle_2)->turtle.hit_by = object;
									OBJECT(object)->flags |= 8u;
									OBJECT(object)->hit_object = turtle_2;
									OBJECT(turtle_2)->hit = OBJECT(object)->attack;
									health_bars = (((uint32_t)OBJECT(turtle_2)->health_bars << 8u) | OBJECT(turtle_2)->health);
									t12 = (uint8_t)OBJECT(object)->damage;
									t13 = (uint8_t)health_bars;
									health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t13 - t12));
									if (t13 <= t12)
									{
										damage_2 = OBJECT(object)->damage;
										t14 = (health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage;
										health_bars = (uint16_t)(t14 - damage_2);
										if (t14 < damage_2)
											health_bars = 0u;
									}
									OBJECT(turtle_2)->health_bars = (uint8_t)(health_bars >> 8u);
									OBJECT(turtle_2)->health = (uint8_t)health_bars;
									break;
								}
							}
						}
					}
					t15 = count_2 - 1u;
					count_2 = t15;
					turtle_2 += 0x50u;
				} while (t15 != 0xFFFFu);
			}
		}
		t16 = count - 1u;
		count = t16;
		object += 0xA0u;
	} while (t16 != 0xFFFFu);
	object = attack_check_objects[(uint8_t)frame_count & 3u];
	count = 1u;
	do
	{
		if ((OBJECT(object)->flags & 4u) != 0u)
		{
			attack = OBJECT(object)->frame;
			if (ATTACK_FRAME(attack)->attack_depth >= 0)
			{
				object_2 = OBJECT_ADDRESS(12);
				count_2 = 0xBu;
				do
				{
					if ((OBJECT(object_2)->flags & 2u) != 0u)
					{
						t17 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
						y_distance = t17;
						if ((int16_t)t17 >= 0)
							y_distance = (uint16_t)(0u - y_distance);
						if ((int16_t)(y_distance + ATTACK_FRAME(attack)->attack_depth + OBJECT(object_2)->depth) >= 0)
						{
							box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(object_2)->frame)->body_box;
							attack_x = (uint16_t)ATTACK_FRAME(attack)->attack_x;
							if ((OBJECT(object)->flags & 0x40u) != 0u)
								attack_x = (uint16_t)(0u - attack_x);
							t18 = attack_x + OBJECT(object)->screen_x - OBJECT(object_2)->screen_x;
							x_distance = t18;
							if ((int16_t)t18 >= 0)
								x_distance = (uint16_t)(0u - x_distance);
							if ((int16_t)(x_distance + ATTACK_FRAME(attack)->attack_width + BODY_BOX(box)->half_width) >= 0)
							{
								t19 = OBJECT(object)->height_whole + ATTACK_FRAME(attack)->attack_y - OBJECT(object_2)->height_whole;
								height_distance = t19;
								if ((int16_t)t19 >= 0)
								{
									if ((int16_t)(0u - height_distance + ATTACK_FRAME(attack)->attack_height + BODY_BOX(box)->height) >= 0)
									{
										OBJECT(object_2)->attacker = object;
										OBJECT(object)->hit_object = object_2;
										OBJECT(object_2)->hit_routine = (((uint32_t)OBJECT(object_2)->routine << 8u) | OBJECT(object_2)->state);
										OBJECT(object_2)->routine = 4u;
										OBJECT(object_2)->state = 0u;
										OBJECT(object)->flags |= 8u;
										OBJECT(object_2)->flags &= ~2u;
										OBJECT(object_2)->hit = OBJECT(object)->attack;
										OBJECT(object_2)->hit ^= 0x80u;
										health_bars = (((uint32_t)OBJECT(object_2)->health_bars << 8u) | OBJECT(object_2)->health);
										t20 = (uint8_t)OBJECT(object)->damage;
										t21 = (uint8_t)health_bars;
										health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t21 - t20));
										if (t21 <= t20)
										{
											damage_3 = OBJECT(object)->damage;
											t22 = (health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage;
											health_bars = (uint16_t)(t22 - damage_3);
											if (t22 < damage_3)
												health_bars = 0u;
										}
										OBJECT(object_2)->health_bars = (uint8_t)(health_bars >> 8u);
										OBJECT(object_2)->health = (uint8_t)health_bars;
									}
								}
								else if ((int16_t)(height_distance + ATTACK_FRAME(attack)->attack_height) >= 0)
								{
									OBJECT(object_2)->attacker = object;
									OBJECT(object)->hit_object = object_2;
									OBJECT(object_2)->hit_routine = (((uint32_t)OBJECT(object_2)->routine << 8u) | OBJECT(object_2)->state);
									OBJECT(object_2)->routine = 4u;
									OBJECT(object_2)->state = 0u;
									OBJECT(object)->flags |= 8u;
									OBJECT(object_2)->flags &= ~2u;
									OBJECT(object_2)->hit = OBJECT(object)->attack;
									OBJECT(object_2)->hit ^= 0x80u;
									health_bars = (((uint32_t)OBJECT(object_2)->health_bars << 8u) | OBJECT(object_2)->health);
									t20 = (uint8_t)OBJECT(object)->damage;
									t21 = (uint8_t)health_bars;
									health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t21 - t20));
									if (t21 <= t20)
									{
										damage_3 = OBJECT(object)->damage;
										t22 = (health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage;
										health_bars = (uint16_t)(t22 - damage_3);
										if (t22 < damage_3)
											health_bars = 0u;
									}
									OBJECT(object_2)->health_bars = (uint8_t)(health_bars >> 8u);
									OBJECT(object_2)->health = (uint8_t)health_bars;
								}
							}
						}
					}
					t23 = count_2 - 1u;
					count_2 = t23;
					object_2 += 0x50u;
				} while (t23 != 0xFFFFu);
			}
		}
		t24 = count - 1u;
		count = t24;
		object += 0x140u;
	} while (t24 != 0xFFFFu);
}

/* the turtle against the solid objects: blocked, standing on one, or carried by a platform */
void turtle_collide_with_objects(uint32_t object)
{
	uint8_t contact_bits = 0;
	uint16_t count = 0, depth_overlap = 0, height_difference = 0, height_overlap = 0, other_height = 0, width_overlap = 0, x_distance = 0, y_distance = 0;
	uint32_t box = 0, object_2 = 0, other_box = 0;
	uint8_t flags = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	object_2 = OBJECT_ADDRESS(4);
	count = 7u;
	OBJECT(object)->contacts = 0u;
	flags = OBJECT(object)->flags;
	OBJECT(object)->flags = flags & 0xEFu;
	if ((flags & 0x10u) != 0u)
		OBJECT(object)->floor_height = 0u;
	box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(object)->frame)->body_box;
	for (;;)
	{
		if (OBJECT(object_2)->flags == 0u)
		{
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		contact_bits = 0xAu;
		t2 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
		y_distance = t2;
		if ((int16_t)t2 >= 0)
		{
			y_distance = (uint16_t)(0u - y_distance);
			contact_bits += 2u;
		}
		t3 = y_distance + OBJECT(object)->depth + OBJECT(object_2)->depth;
		depth_overlap = t3;
		if ((int16_t)t3 < 0)
		{
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		other_box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(object_2)->frame)->body_box;
		t4 = OBJECT(object)->screen_x - OBJECT(object_2)->screen_x;
		x_distance = t4;
		if ((int16_t)t4 >= 0)
		{
			x_distance = (uint16_t)(0u - x_distance);
			contact_bits += 8u;
		}
		t5 = x_distance + BODY_BOX(box)->half_width + BODY_BOX(other_box)->half_width;
		width_overlap = t5;
		if ((int16_t)t5 < 0)
		{
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if (depth_overlap < width_overlap)
			contact_bits &= 0xE7u;
		else
			contact_bits &= ~6u;
		other_height = OBJECT(object_2)->height_whole;
		t6 = OBJECT(object)->height_whole - other_height;
		height_difference = t6;
		if ((int16_t)t6 < 0)
		{
			if ((int16_t)(height_difference + BODY_BOX(box)->height) >= 0)
			{
				OBJECT(object)->contacts |= contact_bits;
				break;
			}
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		other_height += BODY_BOX(other_box)->height;
		t7 = 0u - height_difference + BODY_BOX(other_box)->height;
		height_overlap = t7;
		if ((int16_t)t7 < 0)
		{
			if (depth_overlap >= 3u && width_overlap >= 3u)
			{
				OBJECT(object)->floor_height = (uint8_t)other_height;
				OBJECT(object)->flags |= 0x10u;
				break;
			}
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if (depth_overlap < 3u || width_overlap < 3u)
		{
			OBJECT(object)->contacts |= contact_bits;
			return;
		}
		OBJECT(object)->flags |= 0x10u;
		if (height_overlap >= 5u)
		{
			OBJECT(object)->contacts |= contact_bits;
			return;
		}
		OBJECT(object)->floor_height = (uint8_t)other_height;
		OBJECT(object)->height_whole = (uint8_t)other_height;
		OBJECT(object)->height &= 0xFF00u;
		contact_bits = 1u;
		if ((int8_t)OBJECT(object_2)->kind >= 0)
		{
			OBJECT(object)->contacts |= contact_bits;
			return;
		}
		OBJECT(object)->x += OBJECT(object_2)->x_speed;
		OBJECT(object)->y += OBJECT(object_2)->y_speed;
		OBJECT(object)->height += OBJECT(object_2)->height_speed;
		OBJECT(object)->contacts |= contact_bits;
		return;
	}
}

/* the enemy against the solid objects: blocked or standing on one */
void enemy_collide_with_objects(uint32_t object, uint32_t *p_width_overlap, uint32_t *p_count, uint32_t *p_other_box)
{
	uint16_t width_overlap = (uint16_t)*p_width_overlap;
	uint32_t other_box = *p_other_box;
	uint8_t contact_bits = 0;
	uint16_t depth_overlap = 0, height_difference = 0, height_overlap = 0, other_height = 0, x_distance = 0, y_distance = 0;
	uint32_t box = 0, count = 0, object_2 = 0;
	uint8_t flags = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	object_2 = OBJECT_ADDRESS(4);
	count = 7u;
	OBJECT(object)->contacts &= 0xE0u;
	flags = OBJECT(object)->flags;
	OBJECT(object)->flags = flags & 0xEFu;
	if ((flags & 0x10u) != 0u)
		OBJECT(object)->floor_height = 0u;
	box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(object)->frame)->body_box;
	for (;;)
	{
		if (OBJECT(object_2)->flags == 0u)
		{
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		contact_bits = 0xAu;
		t2 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
		y_distance = t2;
		if ((int16_t)t2 >= 0)
		{
			y_distance = (uint16_t)(0u - y_distance);
			contact_bits += 2u;
		}
		t3 = y_distance + OBJECT(object)->depth + OBJECT(object_2)->depth;
		depth_overlap = t3;
		if ((int16_t)t3 < 0)
		{
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		other_box = body_boxes_ADDRESS + (int16_t)ANIMATION_FRAME(OBJECT(object_2)->frame)->body_box;
		t4 = OBJECT(object)->screen_x - OBJECT(object_2)->screen_x;
		x_distance = t4;
		if ((int16_t)t4 >= 0)
		{
			x_distance = (uint16_t)(0u - x_distance);
			contact_bits += 8u;
		}
		t5 = x_distance + BODY_BOX(box)->half_width + BODY_BOX(other_box)->half_width;
		width_overlap = t5;
		if ((int16_t)t5 < 0)
		{
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if (depth_overlap < width_overlap)
			contact_bits &= 0xE7u;
		else
			contact_bits &= ~6u;
		other_height = OBJECT(object_2)->height_whole;
		t6 = OBJECT(object)->height_whole - other_height;
		height_difference = t6;
		if ((int16_t)t6 < 0)
		{
			if ((int16_t)(height_difference + BODY_BOX(box)->height) < 0)
			{
				t1 = count - 1u;
				count = t1;
				object_2 += 0x50u;
				if (t1 == 0xFFFFu)
					break;
				continue;
			}
			OBJECT(object)->contacts |= contact_bits;
			break;
		}
		other_height += BODY_BOX(other_box)->height;
		t7 = 0u - height_difference + BODY_BOX(other_box)->height;
		height_overlap = t7;
		if ((int16_t)t7 < 0)
		{
			if (depth_overlap >= 3u && width_overlap >= 3u)
			{
				OBJECT(object)->floor_height = (uint8_t)other_height;
				OBJECT(object)->flags |= 0x10u;
				break;
			}
			t1 = count - 1u;
			count = t1;
			object_2 += 0x50u;
			if (t1 == 0xFFFFu)
				break;
			continue;
		}
		if (depth_overlap < 3u || width_overlap < 3u)
		{
			OBJECT(object)->contacts |= contact_bits;
			break;
		}
		OBJECT(object)->flags |= 0x10u;
		if (height_overlap >= 5u)
		{
			OBJECT(object)->contacts |= contact_bits;
			break;
		}
		OBJECT(object)->floor_height = (uint8_t)other_height;
		OBJECT(object)->height_whole = (uint8_t)other_height;
		OBJECT(object)->height &= 0xFF00u;
		contact_bits = 1u;
		OBJECT(object)->contacts |= contact_bits;
		break;
	}
	*p_width_overlap = (*p_width_overlap & 0xFFFF0000u) | width_overlap;
	*p_count = count;
	*p_other_box = other_box;
}

/* the players' slots the enemies go after, set up for the area */
void start_player_slots(void)
{
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->turtle = OBJECT_ADDRESS(0);
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->turtle = OBJECT_ADDRESS(1);
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->turtle = OBJECT_ADDRESS(2);
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->turtle = OBJECT_ADDRESS(3);
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->enemies = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->rodney_sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->mouser_sides = 0u;
	AT32(PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->grab_time, -3) = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(0))->unused = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->enemies = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->rodney_sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->mouser_sides = 0u;
	AT32(PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->grab_time, -3) = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(1))->unused = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->enemies = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->rodney_sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->mouser_sides = 0u;
	AT32(PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->grab_time, -3) = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(2))->unused = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->enemies = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->rodney_sides = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->mouser_sides = 0u;
	AT32(PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->grab_time, -3) = 0u;
	PLAYER_SLOT(PLAYER_SLOT_ADDRESS(3))->unused = 0u;
	nearest_slot = PLAYER_SLOT_ADDRESS(0);
	nearest_turtle = OBJECT_ADDRESS(0);
	stairs_top_height = area_stairs_top_heights[area & 0xFu];
}

/* the object loop: the enemies' spawns, the players' slots, and every object's routine and screen place */
void run_objects(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1)
{
	uint32_t d7 = *p_d7;
	uint32_t a1 = *p_a1;
	uint16_t routine_offset = 0;
	uint32_t a5 = 0, object = 0, routine_entry = 0;

	a1 = spawn_enemies(a1);
	load_layer_b();
	a5 = update_player_slots();
	object = OBJECT_ADDRESS(4);
	do
	{
		if (OBJECT(object)->flags != 0u)
		{
			routine_offset = (uint16_t)((uint32_t)OBJECT(object)->routine << 2);
			routine_entry = ROUTINE_ENTRY(object_routine_tables[OBJECT(object)->kind & 0x7Fu] + (routine_offset / 4u) * 0x4u);
			switch (routine_entry)
			{
			case 0x00DF7Eu:
				krang_appear(object, &d7, &a5);
				break;
			case 0x00E03Cu:
				krang_walk(object, p_d5, &d7, &a5);
				break;
			case 0x00E124u:
				krang_melee(object, p_d5, &d7, &a5);
				break;
			case 0x00E1A8u:
				krang_fire_lasers(object, p_d5, &d7, &a5);
				break;
			case 0x00E21Au:
				krang_struck(object);
				break;
			case 0x00E2A8u:
				krang_recoil(object, &d7, &a5);
				break;
			case 0x00E2DCu:
				krang_explode(object, &d7, &a5);
				break;
			case 0x00E39Cu:
				krang_escape(routine_offset, object, p_d5, &d7);
				break;
			case 0x00E962u:
				krang_curtain_start(object);
				break;
			case 0x00E992u:
				krang_curtain_open(object);
				break;
			case 0x00E9B6u:
				krang_curtain_end(object);
				break;
			case 0x00EAB6u:
				d7 = krang_laser_start(object);
				break;
			case 0x00EB2Au:
				krang_laser_wait(object);
				break;
			case 0x00EB3Eu:
				d7 = krang_laser_fly(object);
				break;
			case 0x00EB70u:
				krang_laser_spark(object);
				break;
			case 0x00EBFEu:
				krang_fist_start(object);
				break;
			case 0x00EC4Cu:
				krang_fist_launch(object);
				break;
			case 0x00EC76u:
				d7 = krang_fist_fly(d7, object);
				break;
			case 0x00F8DAu:
				ball_dropper_start(object);
				break;
			case 0x00F8F2u:
				d7 = ball_dropper_wait(object);
				break;
			case 0x00F96Au:
				ball_start(object);
				break;
			case 0x00F98Eu:
				ball_drop(object);
				break;
			case 0x00F9DCu:
				d7 = ball_bounce(object);
				break;
			case 0x00FA1Cu:
				road_barrier_start(object);
				break;
			case 0x00FA5Au:
				road_barrier_stay(object);
				break;
			case 0x00FA68u:
				tire_stack_start(object);
				break;
			case 0x00FA9Au:
				tire_stack_stay(object);
				break;
			case 0x00FAACu:
				technodrome_ball_start(object);
				break;
			case 0x00FADCu:
				technodrome_ball_drop(object);
				break;
			case 0x00FB34u:
				d7 = technodrome_ball_bounce(object);
				break;
			case 0x00FBB4u:
				hydrant_start(object);
				break;
			case 0x00FBD8u:
				hydrant_stay(object);
				break;
			case 0x00FBDEu:
				d7 = hydrant_spray(d7, object);
				break;
			case 0x00FC1Cu:
				hydrant_struck(object);
				break;
			case 0x00FCA6u:
				hydrant_recover(object);
				break;
			case 0x00FD3Au:
				d7 = hydrant_water_start(object);
				break;
			case 0x00FDA4u:
				d7 = hydrant_water_fly(object);
				break;
			case 0x00FDF2u:
				roadside_cone_start(object);
				break;
			case 0x00FDFCu:
				roadside_pole_start(object);
				break;
			case 0x00FE44u:
				roadside_stay(object);
				break;
			case 0x00FE4Cu:
				d7 = roadside_fly(object);
				break;
			case 0x00FE5Eu:
				roadside_speed_sign_struck(object);
				break;
			case 0x00FE6Eu:
				roadside_cone_struck(object);
				break;
			case 0x00FE7Eu:
				roadside_parking_meter_struck(object);
				break;
			case 0x00FEE2u:
				d7 = rat_start(object);
				break;
			case 0x00FF70u:
				rat_drop(object);
				break;
			case 0x00FFA0u:
				d7 = rat_sit(d7, object);
				break;
			case 0x00FFBEu:
				d7 = rat_run(d7, object);
				break;
			case 0x010026u:
				alligator_start(object);
				break;
			case 0x010058u:
				d7 = alligator_swim(object);
				break;
			case 0x010080u:
				d7 = alligator_snap(object);
				break;
			case 0x0100ACu:
				skateboarder_start(object);
				break;
			case 0x0100D4u:
				skateboarder_idle();
				break;
			case 0x0100D6u:
				d7 = skateboarder_glide(object);
				break;
			case 0x0100DAu:
				skateboarder_struck(object);
				break;
			case 0x010108u:
				d7 = pizza_start(object);
				break;
			case 0x010158u:
				pizza_wait(routine_offset, object, p_d5, &d7);
				break;
			case 0x0101DCu:
				pizza_vanish(object);
				break;
			case 0x0101F8u:
				billboard_start(object);
				break;
			case 0x01023Au:
				d7 = billboard_wait(d7, object);
				break;
			case 0x0102BCu:
				billboard_fall(object);
				break;
			case 0x0102E0u:
				billboard_land(object);
				break;
			case 0x010354u:
				billboard_soldier_start(object);
				break;
			case 0x010398u:
				billboard_soldier_wait(object);
				break;
			case 0x0103C0u:
				d7 = billboard_soldier_jump_down(object);
				break;
			case 0x0103F2u:
				billboard_soldier_become_boomerang_soldier(object);
				break;
			case 0x01042Cu:
				oil_drum_start(object);
				break;
			case 0x010450u:
				oil_drum_stay(object);
				break;
			case 0x010458u:
				oil_drum_blow_up(object);
				break;
			case 0x010490u:
				oil_drum_become_wreck(object);
				break;
			case 0x0104C2u:
				oil_drum_struck(object);
				break;
			case 0x010520u:
				oil_drum_recover(object);
				break;
			case 0x010554u:
				d7 = turret_start(object);
				break;
			case 0x01059Eu:
				d7 = turret_move(object);
				break;
			case 0x0105BCu:
				d7 = turret_fire(object);
				break;
			case 0x010602u:
				turret_wreck(object, &d7, &a5);
				break;
			case 0x010674u:
				turret_struck(object);
				break;
			case 0x0106D6u:
				d7 = turret_recover(object);
				break;
			case 0x01080Au:
				turret_shot_start(object);
				break;
			case 0x010880u:
				d7 = turret_shot_fly(d7, object);
				break;
			case 0x0108B6u:
				turret_shot_vanish(object);
				break;
			case 0x0108D4u:
				waiting_soldier_start(object);
				break;
			case 0x0108FCu:
				waiting_soldier_wait(object);
				break;
			case 0x01090Cu:
				waiting_soldier_become_knife_soldier(object);
				break;
			case 0x01092Eu:
				one_way_sign_start(object);
				break;
			case 0x010952u:
				one_way_sign_stay(object);
				break;
			case 0x010958u:
				one_way_sign_spin(object);
				break;
			case 0x010970u:
				one_way_sign_idle();
				break;
			case 0x010972u:
				one_way_sign_struck(object);
				break;
			case 0x0109D2u:
				nozzle_row_start(object);
				break;
			case 0x0109F8u:
				nozzle_row_run(object, p_d5, &d7, &a5);
				break;
			case 0x010AD4u:
				nozzle_row_done(object);
				break;
			case 0x010B22u:
				freeze_nozzle_start(object);
				break;
			case 0x010B4Au:
				freeze_nozzle_rise(object);
				break;
			case 0x010B6Au:
				d7 = freeze_nozzle_spray(object);
				break;
			case 0x010BE0u:
				freeze_nozzle_sink(object);
				break;
			case 0x010BF8u:
				d7 = freeze_nozzle_struck(d7, object);
				break;
			case 0x010C46u:
				freeze_nozzle_broken(object);
				break;
			case 0x010C56u:
				electric_beam_start(object, &d7, &a5);
				break;
			case 0x010C76u:
				electric_beam_wait(object);
				break;
			case 0x010CB2u:
				electric_beam_shine(object, &d7, &a5);
				break;
			case 0x010CDAu:
				electric_vent_start(object, &d7, &a5);
				break;
			case 0x010CFAu:
				electric_vent_wait(object, &d7, &a5);
				break;
			case 0x010D36u:
				electric_vent_wait_column(object);
				break;
			case 0x010DA8u:
				electric_column_start(object);
				break;
			case 0x010DD4u:
				electric_column_rise(object);
				break;
			case 0x010DE8u:
				electric_column_flicker(object);
				break;
			case 0x010E0Cu:
				electric_column_leave(object);
				break;
			case 0x010E50u:
				grille_start(object);
				break;
			case 0x010E76u:
				grille_idle();
				break;
			case 0x0110A8u:
				explosion_small_start(object);
				break;
			case 0x0110B2u:
				explosion_big_start(object);
				break;
			case 0x0110BCu:
				explosion_drum_start(object);
				break;
			case 0x0110C6u:
				explosion_rocket_start(object);
				break;
			case 0x0110D0u:
				explosion_medium_start(object);
				break;
			case 0x0110F4u:
				explosion_end(object);
				break;
			case 0x011116u:
				spiked_pole_start(object);
				break;
			case 0x01115Au:
				spiked_pole_wait(object);
				break;
			case 0x011190u:
				spiked_pole_hold_up(object);
				break;
			case 0x0111C6u:
				spiked_pole_drop(object);
				break;
			case 0x011208u:
				spiked_pole_hold_down(object);
				break;
			case 0x01123Eu:
				spiked_pole_rise(object);
				break;
			case 0x01129Cu:
				sewer_launcher_start(object);
				break;
			case 0x0112DAu:
				sewer_launcher_wait(object, p_d5, &d7);
				break;
			case 0x01133Au:
				sewer_launcher_fired(object);
				break;
			case 0x011374u:
				sewer_launcher_remove(object);
				break;
			case 0x01147Eu:
				sewer_missile_start(object);
				break;
			case 0x0114C6u:
				d7 = sewer_missile_rise(object);
				break;
			case 0x0114F6u:
				d7 = sewer_missile_fly(object);
				break;
			case 0x0114FEu:
				sewer_missile_remove(object);
				break;
			case 0x011502u:
				sewer_missile_struck(object);
				break;
			case 0x011528u:
				sewer_missile_vanish(object);
				break;
			case 0x01154Eu:
				rock_soldier_site_enter(routine_offset, object, p_d5, &d7, &a5);
				break;
			case 0x0116CCu:
				rock_soldier_site_think(object, p_d5, &d7, &a5);
				break;
			case 0x01186Cu:
				rock_soldier_site_fire(object, p_d5, &d7);
				break;
			case 0x0118CAu:
				rock_soldier_site_finish_attack(object, p_d5, &d7, &a5);
				break;
			case 0x0118ECu:
				a1 = rock_soldier_struck(object);
				break;
			case 0x0119A0u:
				rock_soldier_recover(object, &d7, &a5);
				break;
			case 0x0119DEu:
				rock_soldier_defeated(object, &d7, &a5);
				break;
			case 0x012024u:
				gun_blast_start(object);
				break;
			case 0x012076u:
				d7 = gun_blast_fly(d7, object);
				break;
			case 0x012110u:
				d7 = rubble_start(object);
				break;
			case 0x01214Eu:
				d7 = rubble_fall(object);
				break;
			case 0x01217Au:
				rubble_stay();
				break;
			case 0x012198u:
				rock_soldier_technodrome_enter(object, &d7, &a5);
				break;
			case 0x0122C8u:
				rock_soldier_technodrome_think(object, p_d5, &d7, &a5);
				break;
			case 0x012422u:
				rock_soldier_technodrome_fire(object, p_d5, &d7, &a5);
				break;
			case 0x012430u:
				rock_soldier_technodrome_finish_blow(object, p_d5, &d7, &a5);
				break;
			case 0x0126A0u:
				wall_slab_start(object);
				break;
			case 0x0126F0u:
				d7 = wall_slab_fly(d7, object);
				break;
			case 0x01277Eu:
				missile_start(object);
				break;
			case 0x0127D6u:
				missile_fly(object, p_d5, &d7, &a5);
				break;
			case 0x012830u:
				missile_explode(object);
				break;
			case 0x012862u:
				missile_idle();
				break;
			case 0x012864u:
				missile_struck(object);
				break;
			case 0x01287Au:
				missile_vanish(object);
				break;
			case 0x0128D8u:
				shock_bars_start(object);
				break;
			case 0x0128FEu:
				shock_bars_blink_in(object);
				break;
			case 0x012924u:
				shock_bars_on(object);
				break;
			case 0x01293Cu:
				shock_bars_blink_out(object);
				break;
			case 0x0129A6u:
				shock_bars_hit_box_start(object);
				break;
			case 0x0129E2u:
				shock_bars_hit_box_zap(object);
				break;
			case 0x012A2Eu:
				parked_car_start(object);
				break;
			case 0x012A68u:
				parked_car_draw(object);
				break;
			case 0x012A88u:
				parked_car_wait(object);
				break;
			case 0x012AA6u:
				parked_car_start_drive(object, &d7, &a5);
				break;
			case 0x012AE2u:
				parked_car_drive_off(object);
				break;
			case 0x012B3Cu:
				parked_car_release_soldiers(object);
				break;
			case 0x012C1Cu:
				car_hit_box_start(object);
				break;
			case 0x012C58u:
				d7 = car_hit_box_move(d7, object);
				break;
			case 0x012CB2u:
				d7 = car_soldier_start(object);
				break;
			case 0x012D06u:
				d7 = car_soldier_leap(d7, object);
				break;
			case 0x012D4Eu:
				d7 = helicopter_start(object);
				break;
			case 0x012DCEu:
				helicopter_hover(object, p_d5, &d7);
				break;
			case 0x012E06u:
				helicopter_fire_bullets(object, p_d5, &d7);
				break;
			case 0x012E50u:
				helicopter_drop_missiles(object, p_d5, &d7);
				break;
			case 0x012E9Au:
				d7 = helicopter_struck(object);
				break;
			case 0x012F10u:
				d7 = helicopter_recover(object);
				break;
			case 0x012F40u:
				helicopter_destroyed(object);
				break;
			case 0x0131B8u:
				copter_bullet_start(object);
				break;
			case 0x01320Cu:
				d7 = copter_bullet_fly(object);
				break;
			case 0x013260u:
				copter_bullet_burst(object);
				break;
			case 0x013264u:
				copter_bullet_vanish(object);
				break;
			case 0x0132A2u:
				falling_missile_start(object);
				break;
			case 0x013310u:
				falling_missile_fall(object);
				break;
			case 0x013340u:
				falling_missile_explode(object);
				break;
			case 0x0134B6u:
				flying_drone_enter(object, &d7, &a5);
				break;
			case 0x0135B4u:
				flying_drone_fly(object, &d7, &a5);
				break;
			case 0x01360Cu:
				flying_drone_fire(object, &d7, &a5);
				break;
			case 0x013656u:
				flying_drone_struck(object);
				break;
			case 0x0136BAu:
				d7 = flying_drone_recover(object);
				break;
			case 0x0136EAu:
				flying_drone_destroyed(object);
				break;
			case 0x013AFCu:
				d7 = drone_shot_start(object);
				break;
			case 0x013B8Au:
				d7 = drone_shot_flash(d7, object);
				break;
			case 0x013BB6u:
				d7 = drone_shot_fly(object);
				break;
			case 0x013C0Cu:
				drone_shot_vanish(object);
				break;
			case 0x013C32u:
				stockman_enter(routine_offset, object, p_d5, &d7, &a5);
				break;
			case 0x013DA8u:
				stockman_fly(object, p_d5, &d7, &a5);
				break;
			case 0x013E36u:
				stockman_drop_mouser(object, p_d5, &d7, &a5);
				break;
			case 0x013E64u:
				stockman_idle();
				break;
			case 0x013E66u:
				stockman_struck(object);
				break;
			case 0x013EC0u:
				stockman_recover(object, p_d5, &d7, &a5);
				break;
			case 0x013EF4u:
				stockman_defeated(object, &d7, &a5);
				break;
			case 0x014336u:
				falling_mouser_start(object);
				break;
			case 0x014366u:
				falling_mouser_fall(object);
				break;
			case 0x0143BAu:
				falling_mouser_idle();
				break;
			case 0x0143BCu:
				falling_mouser_struck(object);
				break;
			case 0x01832Au:
				remove_object(object);
				break;
			case 0x0489E2u:
				enemy_foot_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x048A60u:
				enemy_enter(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x048A94u:
				enemy_think(object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x048D8Au:
				enemy_attack(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x048DBAu:
				enemy_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x048EDAu:
				enemy_react(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x048F02u:
				enemy_step_over_edge(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04A744u:
				a5 = boss_shutter_start(object);
				break;
			case 0x04A76Au:
				boss_shutter_wait(object);
				break;
			case 0x04A7AAu:
				boss_shutter_open(object);
				break;
			case 0x04A7F8u:
				scenery_lamp_start(object);
				break;
			case 0x04A814u:
				scenery_stay();
				break;
			case 0x04A830u:
				scenery_open_manhole_start(object);
				break;
			case 0x04A856u:
				scenery_gone_off_screen(object);
				break;
			case 0x04A880u:
				scenery_chair_start(object);
				break;
			case 0x04A8E6u:
				scenery_fire_start(object);
				break;
			case 0x04A97Eu:
				foot_car_start(object);
				break;
			case 0x04A9B6u:
				foot_car_drive(object);
				break;
			case 0x04AA82u:
				falling_debris_start(object);
				break;
			case 0x04AAAAu:
				falling_debris_wait(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04AAE4u:
				falling_debris_fall(object);
				break;
			case 0x04AB2Au:
				falling_debris_burn(object);
				break;
			case 0x04AB5Eu:
				motorbike_start(object);
				break;
			case 0x04ABCAu:
				motorbike_ride(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04AC0Eu:
				motorbike_ride_off(object);
				break;
			case 0x04AC22u:
				a5 = motorbike_struck(object);
				break;
			case 0x04ACB0u:
				enemy_sword_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04AD3Cu:
				enemy_hammer_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04B7F8u:
				whip_arm_start(routine_offset, object, &d7, &a1);
				break;
			case 0x04B828u:
				whip_arm_follow(routine_offset, object, &d7, &a1);
				break;
			case 0x04B88Eu:
				whip_arm_let_go(object, &d7, &a1);
				break;
			case 0x04B89Eu:
				whip_arm_drag(object, &d7, &a1);
				break;
			case 0x04B900u:
				whip_arm_struck(routine_offset, object, &d7, &a1);
				break;
			case 0x04B9C2u:
				laser_bolt_start(object);
				break;
			case 0x04BA04u:
				laser_bolt_fly(object);
				break;
			case 0x04BA34u:
				laser_bolt_vanish(object);
				break;
			case 0x04BA56u:
				rolling_manhole_cover_start(object);
				break;
			case 0x04BAB2u:
				rolling_wait_hittable(object);
				break;
			case 0x04BAC4u:
				rolling_roll(object);
				break;
			case 0x04BAE8u:
				rolling_struck(object);
				break;
			case 0x04BB32u:
				knife_start(object);
				break;
			case 0x04BB88u:
				knife_fly(object);
				break;
			case 0x04BBB6u:
				knife_vanish(object);
				break;
			case 0x04BBD8u:
				knife_struck(object);
				break;
			case 0x04BC0Eu:
				spear_start(object);
				break;
			case 0x04BC50u:
				spear_fly(object);
				break;
			case 0x04BC60u:
				spear_vanish(object);
				break;
			case 0x04BC6Eu:
				spear_struck(object);
				break;
			case 0x04BC98u:
				axe_start(object);
				break;
			case 0x04BCEEu:
				axe_fly(object);
				break;
			case 0x04BD2Cu:
				gun_shot_start(object);
				break;
			case 0x04BD78u:
				gun_shot_fly(object);
				break;
			case 0x04BD9Eu:
				gun_shot_vanish(object);
				break;
			case 0x04BDC0u:
				gun_shot_struck(object);
				break;
			case 0x04BE12u:
				boomerang_start(object);
				break;
			case 0x04BE5Au:
				boomerang_fly_out(object);
				break;
			case 0x04BE92u:
				boomerang_fly_back(object);
				break;
			case 0x04BEBAu:
				boomerang_fly_on(object);
				break;
			case 0x04BECEu:
				boomerang_struck(object);
				break;
			case 0x04BEE8u:
				boomerang_vanish(object);
				break;
			case 0x04BF0Au:
				shuriken_start(object);
				break;
			case 0x04BF4Cu:
				shuriken_fly(object);
				break;
			case 0x04BF96u:
				shuriken_vanish(object);
				break;
			case 0x04BFA4u:
				shuriken_struck(object);
				break;
			case 0x04BFE6u:
				rocksteady_bullet_start(object, p_d5, &d7, &a5);
				break;
			case 0x04C054u:
				rocksteady_bullet_fly(object);
				break;
			case 0x04C078u:
				rocksteady_bullet_vanish(object);
				break;
			case 0x04C0AEu:
				bebop_shot_start(object);
				break;
			case 0x04C0F0u:
				bebop_shot_fly(object);
				break;
			case 0x04C11Au:
				rolling_tire_start(object);
				break;
			case 0x04C17Cu:
				shredder_piece_start(object);
				break;
			case 0x04C1B6u:
				d7 = shredder_piece_bounce(d7, object);
				break;
			case 0x04C204u:
				shredder_ray_start(object);
				break;
			case 0x04C23Cu:
				shredder_ray_shine(object);
				break;
			case 0x04C264u:
				rocket_start(object);
				break;
			case 0x04C2CEu:
				d7 = rocket_fly(d7, object);
				break;
			case 0x04C30Cu:
				rocket_explode(object);
				break;
			case 0x04C32Au:
				dynamite_start(object);
				break;
			case 0x04C35Eu:
				dynamite_throw(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04C386u:
				d7 = dynamite_bounce(d7, object);
				break;
			case 0x04C3F2u:
				dynamite_explode(object);
				break;
			case 0x04C4E6u:
				kicked_door_start(object);
				break;
			case 0x04C4FCu:
				kicked_door_wait(object);
				break;
			case 0x04C522u:
				a5 = kicked_door_break(object, a5);
				break;
			case 0x04C552u:
				kicked_door_become_foot_soldier(object);
				break;
			case 0x04C5F4u:
				door_piece_start(object, &d7, &a5);
				break;
			case 0x04C63Au:
				door_piece_fly(object);
				break;
			case 0x04C6E8u:
				elevator_start(object);
				break;
			case 0x04C716u:
				a1 = elevator_wait_on_screen(object);
				break;
			case 0x04C730u:
				a5 = elevator_open(object, a5);
				break;
			case 0x04C784u:
				elevator_scroll_off(object);
				break;
			case 0x04C89Cu:
				tire_soldier_start(object);
				break;
			case 0x04C8E0u:
				tire_soldier_wait(object);
				break;
			case 0x04C906u:
				tire_soldier_jump(object);
				break;
			case 0x04C936u:
				d7 = tire_soldier_fly(d7, object);
				break;
			case 0x04C964u:
				a5 = tire_soldier_throw(object, a5);
				break;
			case 0x04C9C6u:
				manhole_soldier_start(object);
				break;
			case 0x04CA06u:
				manhole_soldier_wait(object);
				break;
			case 0x04CA2Cu:
				a5 = manhole_soldier_jump_out(object, a5);
				break;
			case 0x04CA68u:
				manhole_soldier_fly(object);
				break;
			case 0x04CA98u:
				a5 = manhole_soldier_throw(object, a5);
				break;
			case 0x04CAD2u:
				thrower_become_foot_soldier(object);
				break;
			case 0x04CBFCu:
				a5 = window_soldier_drop_start(object);
				break;
			case 0x04CC2Eu:
				a5 = window_soldier_drop_jump_out(object, a5);
				break;
			case 0x04CC76u:
				a5 = window_soldier_drop_break_window(object, a5);
				break;
			case 0x04CCA0u:
				window_soldier_drop_fall(object);
				break;
			case 0x04CCECu:
				window_soldier_become_foot_soldier(object);
				break;
			case 0x04CD80u:
				window_soldier_dive_start(object);
				break;
			case 0x04CDAAu:
				a5 = window_soldier_dive_wait(object, a5);
				break;
			case 0x04CE06u:
				window_soldier_dive_fall(object);
				break;
			case 0x04CECAu:
				glass_shards_start(object);
				break;
			case 0x04CF10u:
				glass_shards_fall(object);
				break;
			case 0x04D1AEu:
				sewer_ripple_start(object);
				break;
			case 0x04D1DEu:
				sewer_ripple_swim(object);
				break;
			case 0x04D24Cu:
				a5 = sewer_ripple_leap_out(object, a5);
				break;
			case 0x04D288u:
				sewer_ripple_vanish(object);
				break;
			case 0x04D2A6u:
				sewer_ripple_swim_again(object);
				break;
			case 0x04D46Cu:
				bomber_start(object, &d7, &a5);
				break;
			case 0x04D488u:
				bomber_wait(object);
				break;
			case 0x04D4B0u:
				bomber_lean_out(object);
				break;
			case 0x04D4D0u:
				a5 = bomber_throw(object, a5);
				break;
			case 0x04D4FEu:
				bomber_step_out(object);
				break;
			case 0x04D52Cu:
				bomber_walk_in(object);
				break;
			case 0x04D564u:
				enemy_rocket_soldier_start(object);
				break;
			case 0x04D58Eu:
				enemy_rocket_soldier_line_up(object, p_d5, &d7, &a5);
				break;
			case 0x04D5EEu:
				a5 = enemy_rocket_soldier_fire(object, a5);
				break;
			case 0x04D624u:
				enemy_rocket_soldier_unarmed(object);
				break;
			case 0x04D64Cu:
				enemy_rocket_soldier_struck(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04D6C8u:
				double_door_start(object);
				break;
			case 0x04D6CEu:
				a5 = double_door_wait(object, a5);
				break;
			case 0x04D722u:
				double_door_open(object);
				break;
			case 0x04D76Cu:
				technodrome_door_start(object);
				break;
			case 0x04D782u:
				a5 = technodrome_door_wait(object, a5);
				break;
			case 0x04D7D0u:
				technodrome_door_open(object);
				break;
			case 0x04D7E8u:
				a5 = technodrome_door_let_out(object, a5);
				break;
			case 0x04DC04u:
				enemy_knife_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04DC9Eu:
				enemy_spear_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04E6A2u:
				enemy_axe_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04E736u:
				enemy_gun_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04EE2Eu:
				enemy_boomerang_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04EEC8u:
				enemy_shuriken_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04EF58u:
				enemy_skateboard_soldier_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04EFECu:
				enemy_skateboard_think(object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F002u:
				enemy_skateboard_attack(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F014u:
				enemy_skateboard_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F026u:
				enemy_skateboard_react(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F1D2u:
				enemy_rocksteady_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x04F28Au:
				enemy_rocksteady_enter(object, a1, p_d5, &d7, &a5);
				break;
			case 0x04F3BAu:
				enemy_rocksteady_think(object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F5E4u:
				enemy_rocksteady_attack(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F622u:
				enemy_rocksteady_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x04F680u:
				enemy_rocksteady_react(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x0501C6u:
				enemy_bebop_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x05027Au:
				enemy_bebop_enter(object, a1, p_d5, &d7, &a5);
				break;
			case 0x0503ECu:
				enemy_bebop_think(routine_offset, object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x050612u:
				enemy_bebop_attack(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x050622u:
				enemy_bebop_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x05066Eu:
				enemy_bebop_react(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x050F98u:
				enemy_mouser_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x051002u:
				enemy_mouser_enter(object, a1, p_d5, &d7, &a5);
				break;
			case 0x0510F4u:
				enemy_mouser_think(object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x051314u:
				enemy_mouser_bite(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x05144Eu:
				enemy_mouser_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x051492u:
				enemy_mouser_step_over_edge(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x051B56u:
				mouser_hole_start(object);
				break;
			case 0x051BA2u:
				mouser_hole_wait(object);
				break;
			case 0x051BDAu:
				mouser_hole_open(object);
				break;
			case 0x051BF4u:
				a5 = mouser_hole_let_out(object, a5);
				break;
			case 0x051C44u:
				enemy_rodney_start(object, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x051CACu:
				enemy_rodney_enter(object, a1, p_d5, &d7, &a5);
				break;
			case 0x051D80u:
				enemy_rodney_think(object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x051FA0u:
				enemy_rodney_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x052790u:
				enemy_shredder_start(object, a1, routine_entry, p_d5, &d7, &a5);
				break;
			case 0x052830u:
				enemy_shredder_enter(object, a1, p_d5, &d7, &a5);
				break;
			case 0x052840u:
				enemy_shredder_think(object, routine_entry, p_d5, &d7, &a1, &a5);
				break;
			case 0x052B84u:
				enemy_shredder_attack(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x052BC8u:
				enemy_shredder_hit(object, p_d5, &d7, &a1, &a5);
				break;
			case 0x052C68u:
				enemy_shredder_react(object, p_d5, &d7, &a1, &a5);
				break;
			}
			update_object_screen_place(object);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(40));
	*p_d7 = d7;
	*p_a1 = a1;
}

/* every frame: the players' slots counted and the least crowded one found */
uint32_t update_player_slots(void)
{
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint16_t count = 0, turtle_count = 0;
	uint32_t per_turtle = 0, player_slot = 0, turtle = 0;
	uint16_t t1 = 0;

	player_slot = PLAYER_SLOT_ADDRESS(0);
	count = 3u;
	turtle_count = 0u;
	enemies = 0xFFu;
	do
	{
		turtle = PLAYER_SLOT(player_slot)->turtle;
		if (OBJECT(turtle)->flags != 0u)
		{
			OBJECT(turtle)->turtle.enemy_behind = 0u;
			turtle_count++;
			if (PLAYER_SLOT(player_slot)->enemies < (uint8_t)enemies)
			{
				enemies = PLAYER_SLOT(player_slot)->enemies;
				least_crowded_slot = player_slot;
				if (PLAYER_SLOT(player_slot)->grab_time != 0u)
					PLAYER_SLOT(player_slot)->grab_time--;
			}
		}
		t1 = count - 1u;
		count = t1;
		player_slot += 0x10u;
	} while (t1 != 0xFFFFu);
	per_turtle = 0u;
	turtles_in_play = (uint8_t)turtle_count;
	if ((uint8_t)turtle_count == 0u)
	{
		enemies_per_turtle = per_turtle;
		return turtle;
	}
	per_turtle = enemies_on_screen;
	zero = m68k_divu_z(per_turtle, turtle_count);
	per_turtle = turtle_count == 0u ? per_turtle : m68k_divu(per_turtle, turtle_count);
	if (turtle_count == 0u)
		exception_hang();
	if (zero)
		per_turtle = (uint16_t)(per_turtle + 1u);
	enemies_per_turtle = per_turtle;
	return turtle;
}

/* the object's place on the screen from its place in the area, and whether it is off the screen */
void update_object_screen_place(uint32_t object)
{
	uint16_t edge_x = 0, screen_x = 0, screen_y = 0;
	uint32_t margin = 0;

	if (OBJECT(object)->flags != 0u)
	{
		screen_x = (uint16_t)(((int32_t)(OBJECT(object)->x - (spawn_x & ~0xFFu)) >> 8) + 0x1A0u);
		OBJECT(object)->screen_x = screen_x;
		screen_y = (uint16_t)(((int32_t)(OBJECT(object)->y - (spawn_y & ~0xFFu)) >> 8) + 0x100u);
		OBJECT(object)->screen_y = screen_y;
		OBJECT(object)->flags &= 0xDFu;
		margin = spawn_offsets_ADDRESS + (int16_t)((uint32_t)(int8_t)OBJECT(object)->kind << 2);
		edge_x = (uint16_t)screen_x;
		if ((int16_t)(screen_x - SCREEN_MARGIN(margin)->x) >= 0x1C0 || (int16_t)(edge_x + SCREEN_MARGIN(margin)->x) < 0x40 || (int16_t)screen_y >= 0x120 || (int16_t)(screen_y + SCREEN_MARGIN(margin)->y) < -0x20)
		{
			OBJECT(object)->flags |= 0x20u;
			return;
		}
	}
}

/* starts an animation on the object: its first frame, timed by the frame's count plus the object's
   animation delay */
uint32_t set_animation(uint32_t d1, uint32_t object, uint32_t animation)
{
	uint32_t frame_time = 0;

	frame_time = (d1 & ~0xFFu) | ANIMATION_FRAME(animation)->time;
	frame_time = (frame_time & ~0xFFu) | (uint8_t)(frame_time + OBJECT(object)->animation_delay);
	OBJECT(object)->frame_time = frame_time;
	OBJECT(object)->frame = animation;
	return frame_time;
}

/* the first free object of a group */
void take_free_object_in_group(uint16_t group, uint32_t *p_object, uint32_t *p_zero)
{
	uint32_t zero = *p_zero;
	uint16_t count = 0;
	uint32_t object = 0;
	uint8_t flags = 0;
	uint16_t t1 = 0;

	group &= 3u;
	count = object_group_counts[group];
	object = object_groups[group & 0x3FFFu];
	do
	{
		flags = OBJECT(object)->flags;
		zero = flags == 0u;
		if (flags == 0u)
			break;
		t1 = count - 1u;
		count = t1;
		object += 0x50u;
	} while (t1 != 0xFFFFu);
	*p_object = object;
	*p_zero = zero;
}

/* an object started from a record, placed by the object that starts it */
void start_object_from_record(uint32_t object, uint32_t record, uint32_t *p_started, uint32_t *p_zero)
{
	uint32_t zero = *p_zero;
	uint8_t group = 0;
	uint32_t started = 0;
	uint32_t kind_halfword = 0;

	group = OBJECT_RECORD(record)->group;
	take_free_object_in_group(group, &started, &zero);
	if (zero)
	{
		OBJECT(started)->flags = 1u;
		OBJECT(started)->contacts = 0u;
		kind_halfword = OBJECT_RECORD(record)->kind_and_parameter;
		OBJECT(started)->kind = (uint8_t)(kind_halfword >> 8u);
		OBJECT(started)->parameter = (uint8_t)kind_halfword;
		OBJECT(started)->routine = 0u;
		OBJECT(started)->state = 0u;
		OBJECT(started)->colour = 0xFFu;
		OBJECT(started)->animation_delay = 0u;
		OBJECT(started)->throw = 0u;
		zero = place_from_record(object, record, started);
	}
	*p_started = started;
	*p_zero = zero;
}

/* a shot started from a record, placed by its shooter */
void start_shot(uint32_t object, uint32_t record, uint32_t *p_shot, uint32_t *p_zero)
{
	uint32_t zero = *p_zero;
	uint8_t group = 0;
	uint32_t shot = 0;

	group = 3u;
	take_free_object_in_group(group, &shot, &zero);
	if (zero)
	{
		OBJECT(shot)->flags = 1u;
		OBJECT(shot)->contacts = 0u;
		OBJECT(shot)->kind = SHOT_RECORD(record)->kind;
		OBJECT(shot)->parameter = 0u;
		OBJECT(shot)->routine = 0u;
		OBJECT(shot)->state = 0u;
		OBJECT(shot)->colour = 0xFFu;
		OBJECT(shot)->animation_delay = 0u;
		OBJECT(shot)->throw = 0u;
		OBJECT(shot)->start_shot.hit_sound = OBJECT(object)->start_shot.hit_sound;
		zero = place_from_record(object, record, shot);
	}
	*p_shot = shot;
	*p_zero = zero;
}

/* an object placed by a record's offsets from another, mirrored with it */
uint32_t place_from_record(uint32_t object, uint32_t placement, uint32_t object_2)
{
	uint32_t zero = 0;
	uint16_t x_offset = 0;

	x_offset = (uint16_t)PLACEMENT(placement)->x;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(object_2)->flags |= 0x40u;
		x_offset = (uint16_t)(0u - x_offset);
	}
	OBJECT(object_2)->x = ((uint32_t)(int16_t)x_offset << 8) + OBJECT(object)->x;
	OBJECT(object_2)->y = ((uint32_t)PLACEMENT(placement)->y << 8) + OBJECT(object)->y;
	OBJECT(object_2)->height_whole = PLACEMENT(placement)->height + OBJECT(object)->height_whole;
	zero = 1u;
	OBJECT(object_2)->height &= 0xFF00u;
	return zero;
}

/* the speeds across and up or down of a speed in a direction */
void speeds_from_direction(uint8_t direction, uint16_t speed, uint32_t *p_x_speed, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint16_t angle = 0;
	uint32_t cosines = 0;
	uint32_t t1 = 0;

	*p_sines = quarter_sine_ADDRESS;
	cosines = quarter_sine_last_ADDRESS;
	angle = (uint8_t)direction & 0x3Fu;
	if ((direction & 0x40u) == 0u)
	{
		t1 = *p_sines;
		*p_sines = cosines;
		cosines = t1;
		angle = (uint16_t)(0u - angle);
	}
	*p_x_speed = ((uint32_t)SINE(*p_sines + (int16_t)angle) * (uint16_t)speed) >> 8;
	if ((int8_t)(direction + 0x40u) < 0)
		*p_x_speed = (uint16_t)(0u - *p_x_speed);
	*p_y_speed = ((uint32_t)SINE(cosines + ((int16_t)(0u - angle))) * (uint16_t)speed) >> 8;
	if ((int8_t)direction < 0)
	{
		return;
	}
	*p_y_speed = (uint16_t)(0u - *p_y_speed);
}

/* Unused: an empty routine that returns at once */
void unreached_047D0E(uint8_t direction)
{
}

/* the direction from the object to another one */
uint32_t direction_to_object(uint32_t direction, uint32_t object, uint32_t object_2)
{
	uint32_t target_x = 0, target_y = 0;

	target_x = OBJECT(object_2)->screen_x;
	target_y = OBJECT(object_2)->screen_y;
	direction_to(object, &direction, &target_x, &target_y);
	return direction;
}
