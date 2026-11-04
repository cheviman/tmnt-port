/**
 *    Name:        engine/sprites.c
 *    Description: the sprite engine: the objects' animations, their sprites into the K051960, their boxes
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
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
uint32_t build_draw_list(void)
{
	uint8_t drawn_count = 0;
	uint16_t count = 0, draw_depth = 0, first_slot = 0;
	uint32_t draw_list_place = 0, frame = 0, next_object = 0, object = 0, object_2 = 0;
	uint16_t draw_next = 0, t1 = 0;

	count = 0x31u;
	offset_high_word = 0x31u;
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
	return object;
}

/* every frame: the draw list's objects into the sprite chip with their shadows, their animations stepped
   on, the rest turned off */
uint32_t draw_sprites(uint32_t object)
{
	bool L_047164 = false;
	uint8_t flip = 0, flips = 0, frame_time = 0, object_colour = 0, objects_left = 0;
	uint16_t bottom = 0, count = 0, left = 0, shadow_y = 0, sprite_x = 0;
	uint32_t draw_list_place = 0, frame = 0, next_object = 0, size = 0, sprite = 0, sprite_place = 0;
	uint8_t t1 = 0, colour = 0, frame_time_2 = 0, time = 0, t4 = 0, t5 = 0;
	uint16_t object_2 = 0, t2 = 0, t3 = 0, draw_next = 0;

	draw_list_place = draw_list_ADDRESS + (int16_t)draw_first;
	sprite_place = SPRITE_RAM;
	next_object = OBJECT_ADDRESS(0);
	objects_left = draw_count;
	while (draw_list_place < 0x61580u)
	{
		object_2 = DRAW_ENTRY(draw_list_place)->object;
		next_object = (next_object & ~0xFFFFu) | object_2;
		draw_list_place += 2u;
		if (object_2 == 0u)
			continue;
		DRAW_ENTRY(draw_list_place - 0x2u)->object = 0u;
		do
		{
			object = next_object;
			frame = OBJECT(object)->frame;
			count = ANIMATION_FRAME(frame)->last_sprite;
			sprite = ANIMATION_FRAME(frame)->sprites;
			do
			{
				size = sprite_sizes_ADDRESS + ((SPRITE(sprite)->size_code & 0xE0u) >> 2);
				left = (uint16_t)SPRITE(sprite)->x;
				t1 = (uint32_t)OBJECT(object)->flags << 1;
				flip = t1;
				if ((int8_t)t1 < 0)
					left = (uint16_t)(0u - left - SPRITE_SIZE(size)->width);
				left += OBJECT(object)->screen_x;
				screen_x_high = left;
				if ((int16_t)left < 0x1A0 && SPRITE_SIZE(size)->left < (int16_t)left)
				{
					t2 = OBJECT(object)->height_whole + SPRITE(sprite)->y + OBJECT(object)->screen_y;
					bottom = t2;
					if ((int16_t)t2 >= 0 && (int16_t)bottom < SPRITE_SIZE(size)->bottom)
					{
						left &= 0x1FFu;
						screen_x_high = left;
						flips = SPRITE(sprite)->flips;
						if ((int8_t)(flip ^ flips) < 0)
						{
							left |= 0x200u;
							screen_x_high = left;
						}
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
						io_write16(sprite_place + 6u, left);
						io_write8(sprite_place + 7u, (uint8_t)left);
						io_write16(sprite_place + 4u, bottom);
						io_write8(sprite_place + 5u, (uint8_t)bottom);
						sprite_place += 8u;
					}
				}
				t3 = count - 1u;
				count = t3;
				sprite += 8u;
			} while (t3 != 0xFFFFu);
			if ((int8_t)OBJECT(object)->contacts < 0)
			{
				sprite = OBJECT(object)->shadow;
				shadow_y = (uint16_t)(OBJECT(object)->floor_height + SPRITE(sprite)->y + OBJECT(object)->screen_y);
				sprite_x = (SPRITE(sprite)->x + OBJECT(object)->screen_x) & 0x1FFu;
				screen_x_high = sprite_x;
				if ((int8_t)SPRITE(sprite)->flips < 0)
				{
					sprite_x |= 0x200u;
					screen_x_high = sprite_x;
				}
				shadow_y &= 0x1FFu;
				if ((SPRITE(sprite)->flips & 0x40u) != 0u)
					shadow_y |= 0x200u;
				io_write8(sprite_place, io_read8(sprite_place) | 0x80u);
				io_write8(sprite_place + 1u, SPRITE(sprite)->size_code);
				io_write8(sprite_place + 2u, SPRITE(sprite)->code);
				io_write8(sprite_place + 3u, SPRITE(sprite)->colour);
				io_write16(sprite_place + 6u, sprite_x);
				io_write8(sprite_place + 7u, (uint8_t)sprite_x);
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
					else if (ANIMATION_FRAME(frame)->last_sprite != 0u)
					{
						frame = ANIMATION_FRAME(frame)->sprites;
						frame_time = ANIMATION_FRAME(frame)->time;
						OBJECT(object)->frame = frame;
						frame_time += OBJECT(object)->animation_delay;
					}
					OBJECT(object)->frame_time = frame_time;
				}
			}
			t4 = objects_left;
			objects_left = (uint8_t)(t4 - 1u);
			L_047164 = t4 == 1u;
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
		t5 = io_read8(sprite_place);
		io_write8(sprite_place, t5 & 0x7Fu);
		if ((t5 & 0x80u) == 0u)
			break;
		sprite_place += 8u;
	}
	return object;
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
											health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)OBJECT(turtle)->damage);
											damage = OBJECT(turtle)->damage;
											t6 = health_bars;
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
										health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)OBJECT(turtle)->damage);
										damage = OBJECT(turtle)->damage;
										t6 = health_bars;
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
											health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage);
											damage_2 = OBJECT(object)->damage;
											t14 = health_bars;
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
										health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage);
										damage_2 = OBJECT(object)->damage;
										t14 = health_bars;
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
											health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage);
											damage_3 = OBJECT(object)->damage;
											t22 = health_bars;
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
										health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)OBJECT(object)->damage);
										damage_3 = OBJECT(object)->damage;
										t22 = health_bars;
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
void enemy_collide_with_objects(uint32_t object)
{
	uint8_t contact_bits = 0;
	uint16_t count = 0, depth_overlap = 0, height_difference = 0, height_overlap = 0, other_height = 0, width_overlap = 0, x_distance = 0, y_distance = 0;
	uint32_t box = 0, object_2 = 0, other_box = 0;
	uint8_t flags = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

	object_2 = OBJECT_ADDRESS(4);
	count = 7u;
	offset_high_word = 7u;
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
		object_loop_leftover = other_box;
		t4 = OBJECT(object)->screen_x - OBJECT(object_2)->screen_x;
		x_distance = t4;
		if ((int16_t)t4 >= 0)
		{
			x_distance = (uint16_t)(0u - x_distance);
			contact_bits += 8u;
		}
		t5 = x_distance + BODY_BOX(box)->half_width + BODY_BOX(other_box)->half_width;
		width_overlap = t5;
		distance_low_byte = width_overlap;
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
		OBJECT(object)->contacts |= contact_bits;
		return;
	}
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
void run_objects(void)
{
	uint16_t routine_offset = 0;
	uint32_t object = 0, routine_entry = 0;

	spawn_enemies();
	load_layer_b();
	update_player_slots();
	object = OBJECT_ADDRESS(4);
	do
	{
		if (OBJECT(object)->flags != 0u)
		{
			routine_offset = (uint16_t)((uint32_t)OBJECT(object)->routine << 2);
			routine_entry = ROUTINE_ENTRY(object_routine_tables[OBJECT(object)->kind & 0x7Fu] + (routine_offset / 4u) * 0x4u);
			switch (routine_entry)
			{
			case krang_appear_ADDRESS:
				krang_appear(object);
				break;
			case krang_walk_ADDRESS:
				krang_walk(object);
				break;
			case krang_melee_ADDRESS:
				krang_melee(object);
				break;
			case krang_fire_lasers_ADDRESS:
				krang_fire_lasers(object);
				break;
			case krang_struck_ADDRESS:
				krang_struck(object);
				break;
			case krang_recoil_ADDRESS:
				krang_recoil(object);
				break;
			case krang_explode_ADDRESS:
				krang_explode(object);
				break;
			case krang_escape_ADDRESS:
				krang_escape(routine_offset, object);
				break;
			case krang_curtain_start_ADDRESS:
				krang_curtain_start(object);
				break;
			case krang_curtain_open_ADDRESS:
				krang_curtain_open(object);
				break;
			case krang_curtain_end_ADDRESS:
				krang_curtain_end(object);
				break;
			case krang_laser_start_ADDRESS:
				krang_laser_start(object);
				break;
			case krang_laser_wait_ADDRESS:
				krang_laser_wait(object);
				break;
			case krang_laser_fly_ADDRESS:
				krang_laser_fly(object);
				break;
			case krang_laser_spark_ADDRESS:
				krang_laser_spark(object);
				break;
			case krang_fist_start_ADDRESS:
				krang_fist_start(object);
				break;
			case krang_fist_launch_ADDRESS:
				krang_fist_launch(object);
				break;
			case krang_fist_fly_ADDRESS:
				krang_fist_fly(object);
				break;
			case ball_dropper_start_ADDRESS:
				ball_dropper_start(object);
				break;
			case ball_dropper_wait_ADDRESS:
				ball_dropper_wait(object);
				break;
			case ball_start_ADDRESS:
				ball_start(object);
				break;
			case ball_drop_ADDRESS:
				ball_drop(object);
				break;
			case ball_bounce_ADDRESS:
				ball_bounce(object);
				break;
			case road_barrier_start_ADDRESS:
				road_barrier_start(object);
				break;
			case road_barrier_stay_ADDRESS:
				road_barrier_stay(object);
				break;
			case tire_stack_start_ADDRESS:
				tire_stack_start(object);
				break;
			case tire_stack_stay_ADDRESS:
				tire_stack_stay(object);
				break;
			case technodrome_ball_start_ADDRESS:
				technodrome_ball_start(object);
				break;
			case technodrome_ball_drop_ADDRESS:
				technodrome_ball_drop(object);
				break;
			case technodrome_ball_bounce_ADDRESS:
				technodrome_ball_bounce(object);
				break;
			case hydrant_start_ADDRESS:
				hydrant_start(object);
				break;
			case hydrant_stay_ADDRESS:
				hydrant_stay(object);
				break;
			case hydrant_spray_ADDRESS:
				hydrant_spray(object);
				break;
			case hydrant_struck_ADDRESS:
				hydrant_struck(object);
				break;
			case hydrant_recover_ADDRESS:
				hydrant_recover(object);
				break;
			case hydrant_water_start_ADDRESS:
				hydrant_water_start(object);
				break;
			case hydrant_water_fly_ADDRESS:
				hydrant_water_fly(object);
				break;
			case roadside_cone_start_ADDRESS:
				roadside_cone_start(object);
				break;
			case roadside_pole_start_ADDRESS:
				roadside_pole_start(object);
				break;
			case roadside_stay_ADDRESS:
				roadside_stay(object);
				break;
			case roadside_fly_ADDRESS:
				roadside_fly(object);
				break;
			case roadside_speed_sign_struck_ADDRESS:
				roadside_speed_sign_struck(object);
				break;
			case roadside_cone_struck_ADDRESS:
				roadside_cone_struck(object);
				break;
			case roadside_parking_meter_struck_ADDRESS:
				roadside_parking_meter_struck(object);
				break;
			case rat_start_ADDRESS:
				rat_start(object);
				break;
			case rat_drop_ADDRESS:
				rat_drop(object);
				break;
			case rat_sit_ADDRESS:
				rat_sit(object);
				break;
			case rat_run_ADDRESS:
				rat_run(object);
				break;
			case alligator_start_ADDRESS:
				alligator_start(object);
				break;
			case alligator_swim_ADDRESS:
				alligator_swim(object);
				break;
			case alligator_snap_ADDRESS:
				alligator_snap(object);
				break;
			case skateboarder_start_ADDRESS:
				skateboarder_start(object);
				break;
			case skateboarder_idle_ADDRESS:
				skateboarder_idle();
				break;
			case skateboarder_glide_ADDRESS:
				skateboarder_glide(object);
				break;
			case skateboarder_struck_ADDRESS:
				skateboarder_struck(object);
				break;
			case pizza_start_ADDRESS:
				pizza_start(object);
				break;
			case pizza_wait_ADDRESS:
				pizza_wait(routine_offset, object);
				break;
			case pizza_vanish_ADDRESS:
				pizza_vanish(object);
				break;
			case billboard_start_ADDRESS:
				billboard_start(object);
				break;
			case billboard_wait_ADDRESS:
				billboard_wait(object);
				break;
			case billboard_fall_ADDRESS:
				billboard_fall(object);
				break;
			case billboard_land_ADDRESS:
				billboard_land(object);
				break;
			case billboard_soldier_start_ADDRESS:
				billboard_soldier_start(object);
				break;
			case billboard_soldier_wait_ADDRESS:
				billboard_soldier_wait(object);
				break;
			case billboard_soldier_jump_down_ADDRESS:
				billboard_soldier_jump_down(object);
				break;
			case billboard_soldier_become_boomerang_soldier_ADDRESS:
				billboard_soldier_become_boomerang_soldier(object);
				break;
			case oil_drum_start_ADDRESS:
				oil_drum_start(object);
				break;
			case oil_drum_stay_ADDRESS:
				oil_drum_stay(object);
				break;
			case oil_drum_blow_up_ADDRESS:
				oil_drum_blow_up(object);
				break;
			case oil_drum_become_wreck_ADDRESS:
				oil_drum_become_wreck(object);
				break;
			case oil_drum_struck_ADDRESS:
				oil_drum_struck(object);
				break;
			case oil_drum_recover_ADDRESS:
				oil_drum_recover(object);
				break;
			case turret_start_ADDRESS:
				turret_start(object);
				break;
			case turret_move_ADDRESS:
				turret_move(object);
				break;
			case turret_fire_ADDRESS:
				turret_fire(object);
				break;
			case turret_wreck_ADDRESS:
				turret_wreck(object);
				break;
			case turret_struck_ADDRESS:
				turret_struck(object);
				break;
			case turret_recover_ADDRESS:
				turret_recover(object);
				break;
			case turret_shot_start_ADDRESS:
				turret_shot_start(object);
				break;
			case turret_shot_fly_ADDRESS:
				turret_shot_fly(object);
				break;
			case turret_shot_vanish_ADDRESS:
				turret_shot_vanish(object);
				break;
			case waiting_soldier_start_ADDRESS:
				waiting_soldier_start(object);
				break;
			case waiting_soldier_wait_ADDRESS:
				waiting_soldier_wait(object);
				break;
			case waiting_soldier_become_knife_soldier_ADDRESS:
				waiting_soldier_become_knife_soldier(object);
				break;
			case one_way_sign_start_ADDRESS:
				one_way_sign_start(object);
				break;
			case one_way_sign_stay_ADDRESS:
				one_way_sign_stay(object);
				break;
			case one_way_sign_spin_ADDRESS:
				one_way_sign_spin(object);
				break;
			case one_way_sign_idle_ADDRESS:
				one_way_sign_idle();
				break;
			case one_way_sign_struck_ADDRESS:
				one_way_sign_struck(object);
				break;
			case nozzle_row_start_ADDRESS:
				nozzle_row_start(object);
				break;
			case nozzle_row_run_ADDRESS:
				nozzle_row_run(object);
				break;
			case nozzle_row_done_ADDRESS:
				nozzle_row_done(object);
				break;
			case freeze_nozzle_start_ADDRESS:
				freeze_nozzle_start(object);
				break;
			case freeze_nozzle_rise_ADDRESS:
				freeze_nozzle_rise(object);
				break;
			case freeze_nozzle_spray_ADDRESS:
				freeze_nozzle_spray(object);
				break;
			case freeze_nozzle_sink_ADDRESS:
				freeze_nozzle_sink(object);
				break;
			case freeze_nozzle_struck_ADDRESS:
				freeze_nozzle_struck(object);
				break;
			case freeze_nozzle_broken_ADDRESS:
				freeze_nozzle_broken(object);
				break;
			case electric_beam_start_ADDRESS:
				electric_beam_start(object);
				break;
			case electric_beam_wait_ADDRESS:
				electric_beam_wait(object);
				break;
			case electric_beam_shine_ADDRESS:
				electric_beam_shine(object);
				break;
			case electric_vent_start_ADDRESS:
				electric_vent_start(object);
				break;
			case electric_vent_wait_ADDRESS:
				electric_vent_wait(object);
				break;
			case electric_vent_wait_column_ADDRESS:
				electric_vent_wait_column(object);
				break;
			case electric_column_start_ADDRESS:
				electric_column_start(object);
				break;
			case electric_column_rise_ADDRESS:
				electric_column_rise(object);
				break;
			case electric_column_flicker_ADDRESS:
				electric_column_flicker(object);
				break;
			case electric_column_leave_ADDRESS:
				electric_column_leave(object);
				break;
			case grille_start_ADDRESS:
				grille_start(object);
				break;
			case grille_idle_ADDRESS:
				grille_idle();
				break;
			case explosion_small_start_ADDRESS:
				explosion_small_start(object);
				break;
			case explosion_big_start_ADDRESS:
				explosion_big_start(object);
				break;
			case explosion_drum_start_ADDRESS:
				explosion_drum_start(object);
				break;
			case explosion_rocket_start_ADDRESS:
				explosion_rocket_start(object);
				break;
			case explosion_medium_start_ADDRESS:
				explosion_medium_start(object);
				break;
			case explosion_end_ADDRESS:
				explosion_end(object);
				break;
			case spiked_pole_start_ADDRESS:
				spiked_pole_start(object);
				break;
			case spiked_pole_wait_ADDRESS:
				spiked_pole_wait(object);
				break;
			case spiked_pole_hold_up_ADDRESS:
				spiked_pole_hold_up(object);
				break;
			case spiked_pole_drop_ADDRESS:
				spiked_pole_drop(object);
				break;
			case spiked_pole_hold_down_ADDRESS:
				spiked_pole_hold_down(object);
				break;
			case spiked_pole_rise_ADDRESS:
				spiked_pole_rise(object);
				break;
			case sewer_launcher_start_ADDRESS:
				sewer_launcher_start(object);
				break;
			case sewer_launcher_wait_ADDRESS:
				sewer_launcher_wait(object);
				break;
			case sewer_launcher_fired_ADDRESS:
				sewer_launcher_fired(object);
				break;
			case sewer_launcher_remove_ADDRESS:
				sewer_launcher_remove(object);
				break;
			case sewer_missile_start_ADDRESS:
				sewer_missile_start(object);
				break;
			case sewer_missile_rise_ADDRESS:
				sewer_missile_rise(object);
				break;
			case sewer_missile_fly_ADDRESS:
				sewer_missile_fly(object);
				break;
			case sewer_missile_remove_ADDRESS:
				sewer_missile_remove(object);
				break;
			case sewer_missile_struck_ADDRESS:
				sewer_missile_struck(object);
				break;
			case sewer_missile_vanish_ADDRESS:
				sewer_missile_vanish(object);
				break;
			case rock_soldier_site_enter_ADDRESS:
				rock_soldier_site_enter(routine_offset, object);
				break;
			case rock_soldier_site_think_ADDRESS:
				rock_soldier_site_think(object);
				break;
			case rock_soldier_site_fire_ADDRESS:
				rock_soldier_site_fire(object);
				break;
			case rock_soldier_site_finish_attack_ADDRESS:
				rock_soldier_site_finish_attack(object);
				break;
			case rock_soldier_struck_ADDRESS:
				rock_soldier_struck(object);
				break;
			case rock_soldier_recover_ADDRESS:
				rock_soldier_recover(object);
				break;
			case rock_soldier_defeated_ADDRESS:
				rock_soldier_defeated(object);
				break;
			case gun_blast_start_ADDRESS:
				gun_blast_start(object);
				break;
			case gun_blast_fly_ADDRESS:
				gun_blast_fly(object);
				break;
			case rubble_start_ADDRESS:
				rubble_start(object);
				break;
			case rubble_fall_ADDRESS:
				rubble_fall(object);
				break;
			case rubble_stay_ADDRESS:
				rubble_stay();
				break;
			case rock_soldier_technodrome_enter_ADDRESS:
				rock_soldier_technodrome_enter(object);
				break;
			case rock_soldier_technodrome_think_ADDRESS:
				rock_soldier_technodrome_think(object);
				break;
			case rock_soldier_technodrome_fire_ADDRESS:
				rock_soldier_technodrome_fire(object);
				break;
			case rock_soldier_technodrome_finish_blow_ADDRESS:
				rock_soldier_technodrome_finish_blow(object);
				break;
			case wall_slab_start_ADDRESS:
				wall_slab_start(object);
				break;
			case wall_slab_fly_ADDRESS:
				wall_slab_fly(object);
				break;
			case missile_start_ADDRESS:
				missile_start(object);
				break;
			case missile_fly_ADDRESS:
				missile_fly(object);
				break;
			case missile_explode_ADDRESS:
				missile_explode(object);
				break;
			case missile_idle_ADDRESS:
				missile_idle();
				break;
			case missile_struck_ADDRESS:
				missile_struck(object);
				break;
			case missile_vanish_ADDRESS:
				missile_vanish(object);
				break;
			case shock_bars_start_ADDRESS:
				shock_bars_start(object);
				break;
			case shock_bars_blink_in_ADDRESS:
				shock_bars_blink_in(object);
				break;
			case shock_bars_on_ADDRESS:
				shock_bars_on(object);
				break;
			case shock_bars_blink_out_ADDRESS:
				shock_bars_blink_out(object);
				break;
			case shock_bars_hit_box_start_ADDRESS:
				shock_bars_hit_box_start(object);
				break;
			case shock_bars_hit_box_zap_ADDRESS:
				shock_bars_hit_box_zap(object);
				break;
			case parked_car_start_ADDRESS:
				parked_car_start(object);
				break;
			case parked_car_draw_ADDRESS:
				parked_car_draw(object);
				break;
			case parked_car_wait_ADDRESS:
				parked_car_wait(object);
				break;
			case parked_car_start_drive_ADDRESS:
				parked_car_start_drive(object);
				break;
			case parked_car_drive_off_ADDRESS:
				parked_car_drive_off(object);
				break;
			case parked_car_release_soldiers_ADDRESS:
				parked_car_release_soldiers(object);
				break;
			case car_hit_box_start_ADDRESS:
				car_hit_box_start(object);
				break;
			case car_hit_box_move_ADDRESS:
				car_hit_box_move(object);
				break;
			case car_soldier_start_ADDRESS:
				car_soldier_start(object);
				break;
			case car_soldier_leap_ADDRESS:
				car_soldier_leap(object);
				break;
			case helicopter_start_ADDRESS:
				helicopter_start(object);
				break;
			case helicopter_hover_ADDRESS:
				helicopter_hover(object);
				break;
			case helicopter_fire_bullets_ADDRESS:
				helicopter_fire_bullets(object);
				break;
			case helicopter_drop_missiles_ADDRESS:
				helicopter_drop_missiles(object);
				break;
			case helicopter_struck_ADDRESS:
				helicopter_struck(object);
				break;
			case helicopter_recover_ADDRESS:
				helicopter_recover(object);
				break;
			case helicopter_destroyed_ADDRESS:
				helicopter_destroyed(object);
				break;
			case copter_bullet_start_ADDRESS:
				copter_bullet_start(object);
				break;
			case copter_bullet_fly_ADDRESS:
				copter_bullet_fly(object);
				break;
			case copter_bullet_burst_ADDRESS:
				copter_bullet_burst(object);
				break;
			case copter_bullet_vanish_ADDRESS:
				copter_bullet_vanish(object);
				break;
			case falling_missile_start_ADDRESS:
				falling_missile_start(object);
				break;
			case falling_missile_fall_ADDRESS:
				falling_missile_fall(object);
				break;
			case falling_missile_explode_ADDRESS:
				falling_missile_explode(object);
				break;
			case flying_drone_enter_ADDRESS:
				flying_drone_enter(object);
				break;
			case flying_drone_fly_ADDRESS:
				flying_drone_fly(object);
				break;
			case flying_drone_fire_ADDRESS:
				flying_drone_fire(object);
				break;
			case flying_drone_struck_ADDRESS:
				flying_drone_struck(object);
				break;
			case flying_drone_recover_ADDRESS:
				flying_drone_recover(object);
				break;
			case flying_drone_destroyed_ADDRESS:
				flying_drone_destroyed(object);
				break;
			case drone_shot_start_ADDRESS:
				drone_shot_start(object);
				break;
			case drone_shot_flash_ADDRESS:
				drone_shot_flash(object);
				break;
			case drone_shot_fly_ADDRESS:
				drone_shot_fly(object);
				break;
			case drone_shot_vanish_ADDRESS:
				drone_shot_vanish(object);
				break;
			case stockman_enter_ADDRESS:
				stockman_enter(routine_offset, object);
				break;
			case stockman_fly_ADDRESS:
				stockman_fly(object);
				break;
			case stockman_drop_mouser_ADDRESS:
				stockman_drop_mouser(object);
				break;
			case stockman_idle_ADDRESS:
				stockman_idle();
				break;
			case stockman_struck_ADDRESS:
				stockman_struck(object);
				break;
			case stockman_recover_ADDRESS:
				stockman_recover(object);
				break;
			case stockman_defeated_ADDRESS:
				stockman_defeated(object);
				break;
			case falling_mouser_start_ADDRESS:
				falling_mouser_start(object);
				break;
			case falling_mouser_fall_ADDRESS:
				falling_mouser_fall(object);
				break;
			case falling_mouser_idle_ADDRESS:
				falling_mouser_idle();
				break;
			case falling_mouser_struck_ADDRESS:
				falling_mouser_struck(object);
				break;
			case remove_object_ADDRESS:
				remove_object(object);
				break;
			case enemy_foot_soldier_start_ADDRESS:
				enemy_foot_soldier_start(object, routine_entry);
				break;
			case enemy_enter_ADDRESS:
				enemy_enter(object);
				break;
			case enemy_think_ADDRESS:
				enemy_think(object, routine_entry);
				break;
			case enemy_attack_ADDRESS:
				enemy_attack(object);
				break;
			case enemy_hit_ADDRESS:
				enemy_hit(object);
				break;
			case enemy_react_ADDRESS:
				enemy_react(object);
				break;
			case enemy_step_over_edge_ADDRESS:
				enemy_step_over_edge(object);
				break;
			case boss_shutter_start_ADDRESS:
				boss_shutter_start(object);
				break;
			case boss_shutter_wait_ADDRESS:
				boss_shutter_wait(object);
				break;
			case boss_shutter_open_ADDRESS:
				boss_shutter_open(object);
				break;
			case scenery_lamp_start_ADDRESS:
				scenery_lamp_start(object);
				break;
			case scenery_stay_ADDRESS:
				scenery_stay();
				break;
			case scenery_open_manhole_start_ADDRESS:
				scenery_open_manhole_start(object);
				break;
			case scenery_gone_off_screen_ADDRESS:
				scenery_gone_off_screen(object);
				break;
			case scenery_chair_start_ADDRESS:
				scenery_chair_start(object);
				break;
			case scenery_fire_start_ADDRESS:
				scenery_fire_start(object);
				break;
			case foot_car_start_ADDRESS:
				foot_car_start(object);
				break;
			case foot_car_drive_ADDRESS:
				foot_car_drive(object);
				break;
			case falling_debris_start_ADDRESS:
				falling_debris_start(object);
				break;
			case falling_debris_wait_ADDRESS:
				falling_debris_wait(object, routine_entry);
				break;
			case falling_debris_fall_ADDRESS:
				falling_debris_fall(object);
				break;
			case falling_debris_burn_ADDRESS:
				falling_debris_burn(object);
				break;
			case motorbike_start_ADDRESS:
				motorbike_start(object);
				break;
			case motorbike_ride_ADDRESS:
				motorbike_ride(object, routine_entry);
				break;
			case motorbike_ride_off_ADDRESS:
				motorbike_ride_off(object);
				break;
			case motorbike_struck_ADDRESS:
				motorbike_struck(object);
				break;
			case enemy_sword_soldier_start_ADDRESS:
				enemy_sword_soldier_start(object, routine_entry);
				break;
			case enemy_hammer_soldier_start_ADDRESS:
				enemy_hammer_soldier_start(object, routine_entry);
				break;
			case whip_arm_start_ADDRESS:
				whip_arm_start(routine_offset, object);
				break;
			case whip_arm_follow_ADDRESS:
				whip_arm_follow(routine_offset, object);
				break;
			case whip_arm_let_go_ADDRESS:
				whip_arm_let_go(object);
				break;
			case whip_arm_drag_ADDRESS:
				whip_arm_drag(object);
				break;
			case whip_arm_struck_ADDRESS:
				whip_arm_struck(routine_offset, object);
				break;
			case laser_bolt_start_ADDRESS:
				laser_bolt_start(object);
				break;
			case laser_bolt_fly_ADDRESS:
				laser_bolt_fly(object);
				break;
			case laser_bolt_vanish_ADDRESS:
				laser_bolt_vanish(object);
				break;
			case rolling_manhole_cover_start_ADDRESS:
				rolling_manhole_cover_start(object);
				break;
			case rolling_wait_hittable_ADDRESS:
				rolling_wait_hittable(object);
				break;
			case rolling_roll_ADDRESS:
				rolling_roll(object);
				break;
			case rolling_struck_ADDRESS:
				rolling_struck(object);
				break;
			case knife_start_ADDRESS:
				knife_start(object);
				break;
			case knife_fly_ADDRESS:
				knife_fly(object);
				break;
			case knife_vanish_ADDRESS:
				knife_vanish(object);
				break;
			case knife_struck_ADDRESS:
				knife_struck(object);
				break;
			case spear_start_ADDRESS:
				spear_start(object);
				break;
			case spear_fly_ADDRESS:
				spear_fly(object);
				break;
			case spear_vanish_ADDRESS:
				spear_vanish(object);
				break;
			case spear_struck_ADDRESS:
				spear_struck(object);
				break;
			case axe_start_ADDRESS:
				axe_start(object);
				break;
			case axe_fly_ADDRESS:
				axe_fly(object);
				break;
			case gun_shot_start_ADDRESS:
				gun_shot_start(object);
				break;
			case gun_shot_fly_ADDRESS:
				gun_shot_fly(object);
				break;
			case gun_shot_vanish_ADDRESS:
				gun_shot_vanish(object);
				break;
			case gun_shot_struck_ADDRESS:
				gun_shot_struck(object);
				break;
			case boomerang_start_ADDRESS:
				boomerang_start(object);
				break;
			case boomerang_fly_out_ADDRESS:
				boomerang_fly_out(object);
				break;
			case boomerang_fly_back_ADDRESS:
				boomerang_fly_back(object);
				break;
			case boomerang_fly_on_ADDRESS:
				boomerang_fly_on(object);
				break;
			case boomerang_struck_ADDRESS:
				boomerang_struck(object);
				break;
			case boomerang_vanish_ADDRESS:
				boomerang_vanish(object);
				break;
			case shuriken_start_ADDRESS:
				shuriken_start(object);
				break;
			case shuriken_fly_ADDRESS:
				shuriken_fly(object);
				break;
			case shuriken_vanish_ADDRESS:
				shuriken_vanish(object);
				break;
			case shuriken_struck_ADDRESS:
				shuriken_struck(object);
				break;
			case rocksteady_bullet_start_ADDRESS:
				rocksteady_bullet_start(object);
				break;
			case rocksteady_bullet_fly_ADDRESS:
				rocksteady_bullet_fly(object);
				break;
			case rocksteady_bullet_vanish_ADDRESS:
				rocksteady_bullet_vanish(object);
				break;
			case rocksteady_bullet_burst_ADDRESS:
				rocksteady_bullet_burst(object);
				break;
			case bebop_shot_start_ADDRESS:
				bebop_shot_start(object);
				break;
			case bebop_shot_fly_ADDRESS:
				bebop_shot_fly(object);
				break;
			case rolling_tire_start_ADDRESS:
				rolling_tire_start(object);
				break;
			case shredder_piece_start_ADDRESS:
				shredder_piece_start(object);
				break;
			case shredder_piece_bounce_ADDRESS:
				shredder_piece_bounce(object);
				break;
			case shredder_ray_start_ADDRESS:
				shredder_ray_start(object);
				break;
			case shredder_ray_shine_ADDRESS:
				shredder_ray_shine(object);
				break;
			case rocket_start_ADDRESS:
				rocket_start(object);
				break;
			case rocket_fly_ADDRESS:
				rocket_fly(object);
				break;
			case rocket_explode_ADDRESS:
				rocket_explode(object);
				break;
			case dynamite_start_ADDRESS:
				dynamite_start(object);
				break;
			case dynamite_throw_ADDRESS:
				dynamite_throw(object, routine_entry);
				break;
			case dynamite_bounce_ADDRESS:
				dynamite_bounce(object);
				break;
			case dynamite_explode_ADDRESS:
				dynamite_explode(object);
				break;
			case kicked_door_start_ADDRESS:
				kicked_door_start(object);
				break;
			case kicked_door_wait_ADDRESS:
				kicked_door_wait(object);
				break;
			case kicked_door_break_ADDRESS:
				kicked_door_break(object);
				break;
			case kicked_door_become_foot_soldier_ADDRESS:
				kicked_door_become_foot_soldier(object);
				break;
			case door_piece_start_ADDRESS:
				door_piece_start(object);
				break;
			case door_piece_fly_ADDRESS:
				door_piece_fly(object);
				break;
			case elevator_start_ADDRESS:
				elevator_start(object);
				break;
			case elevator_wait_on_screen_ADDRESS:
				elevator_wait_on_screen(object);
				break;
			case elevator_open_ADDRESS:
				elevator_open(object);
				break;
			case elevator_scroll_off_ADDRESS:
				elevator_scroll_off(object);
				break;
			case tire_soldier_start_ADDRESS:
				tire_soldier_start(object);
				break;
			case tire_soldier_wait_ADDRESS:
				tire_soldier_wait(object);
				break;
			case tire_soldier_jump_ADDRESS:
				tire_soldier_jump(object);
				break;
			case tire_soldier_fly_ADDRESS:
				tire_soldier_fly(object);
				break;
			case tire_soldier_throw_ADDRESS:
				tire_soldier_throw(object);
				break;
			case manhole_soldier_start_ADDRESS:
				manhole_soldier_start(object);
				break;
			case manhole_soldier_wait_ADDRESS:
				manhole_soldier_wait(object);
				break;
			case manhole_soldier_jump_out_ADDRESS:
				manhole_soldier_jump_out(object);
				break;
			case manhole_soldier_fly_ADDRESS:
				manhole_soldier_fly(object);
				break;
			case manhole_soldier_throw_ADDRESS:
				manhole_soldier_throw(object);
				break;
			case thrower_become_foot_soldier_ADDRESS:
				thrower_become_foot_soldier(object);
				break;
			case window_soldier_drop_start_ADDRESS:
				window_soldier_drop_start(object);
				break;
			case window_soldier_drop_jump_out_ADDRESS:
				window_soldier_drop_jump_out(object);
				break;
			case window_soldier_drop_break_window_ADDRESS:
				window_soldier_drop_break_window(object);
				break;
			case window_soldier_drop_fall_ADDRESS:
				window_soldier_drop_fall(object);
				break;
			case window_soldier_become_foot_soldier_ADDRESS:
				window_soldier_become_foot_soldier(object);
				break;
			case window_soldier_dive_start_ADDRESS:
				window_soldier_dive_start(object);
				break;
			case window_soldier_dive_wait_ADDRESS:
				window_soldier_dive_wait(object);
				break;
			case window_soldier_dive_fall_ADDRESS:
				window_soldier_dive_fall(object);
				break;
			case glass_shards_start_ADDRESS:
				glass_shards_start(object);
				break;
			case glass_shards_fall_ADDRESS:
				glass_shards_fall(object);
				break;
			case sewer_ripple_start_ADDRESS:
				sewer_ripple_start(object);
				break;
			case sewer_ripple_swim_ADDRESS:
				sewer_ripple_swim(object);
				break;
			case sewer_ripple_leap_out_ADDRESS:
				sewer_ripple_leap_out(object);
				break;
			case sewer_ripple_vanish_ADDRESS:
				sewer_ripple_vanish(object);
				break;
			case sewer_ripple_swim_again_ADDRESS:
				sewer_ripple_swim_again(object);
				break;
			case bomber_start_ADDRESS:
				bomber_start(object);
				break;
			case bomber_wait_ADDRESS:
				bomber_wait(object);
				break;
			case bomber_lean_out_ADDRESS:
				bomber_lean_out(object);
				break;
			case bomber_throw_ADDRESS:
				bomber_throw(object);
				break;
			case bomber_step_out_ADDRESS:
				bomber_step_out(object);
				break;
			case bomber_walk_in_ADDRESS:
				bomber_walk_in(object);
				break;
			case enemy_rocket_soldier_start_ADDRESS:
				enemy_rocket_soldier_start(object);
				break;
			case enemy_rocket_soldier_line_up_ADDRESS:
				enemy_rocket_soldier_line_up(object);
				break;
			case enemy_rocket_soldier_fire_ADDRESS:
				enemy_rocket_soldier_fire(object);
				break;
			case enemy_rocket_soldier_unarmed_ADDRESS:
				enemy_rocket_soldier_unarmed(object);
				break;
			case enemy_rocket_soldier_struck_ADDRESS:
				enemy_rocket_soldier_struck(object);
				break;
			case double_door_start_ADDRESS:
				double_door_start(object);
				break;
			case double_door_wait_ADDRESS:
				double_door_wait(object);
				break;
			case double_door_open_ADDRESS:
				double_door_open(object);
				break;
			case technodrome_door_start_ADDRESS:
				technodrome_door_start(object);
				break;
			case technodrome_door_wait_ADDRESS:
				technodrome_door_wait(object);
				break;
			case technodrome_door_open_ADDRESS:
				technodrome_door_open(object);
				break;
			case technodrome_door_let_out_ADDRESS:
				technodrome_door_let_out(object);
				break;
			case enemy_knife_soldier_start_ADDRESS:
				enemy_knife_soldier_start(object, routine_entry);
				break;
			case enemy_spear_soldier_start_ADDRESS:
				enemy_spear_soldier_start(object, routine_entry);
				break;
			case enemy_axe_soldier_start_ADDRESS:
				enemy_axe_soldier_start(object, routine_entry);
				break;
			case enemy_gun_soldier_start_ADDRESS:
				enemy_gun_soldier_start(object, routine_entry);
				break;
			case enemy_boomerang_soldier_start_ADDRESS:
				enemy_boomerang_soldier_start(object, routine_entry);
				break;
			case enemy_shuriken_soldier_start_ADDRESS:
				enemy_shuriken_soldier_start(object, routine_entry);
				break;
			case enemy_skateboard_soldier_start_ADDRESS:
				enemy_skateboard_soldier_start(object, routine_entry);
				break;
			case enemy_skateboard_think_ADDRESS:
				enemy_skateboard_think(object, routine_entry);
				break;
			case enemy_skateboard_attack_ADDRESS:
				enemy_skateboard_attack(object);
				break;
			case enemy_skateboard_hit_ADDRESS:
				enemy_skateboard_hit(object);
				break;
			case enemy_skateboard_react_ADDRESS:
				enemy_skateboard_react(object);
				break;
			case enemy_rocksteady_start_ADDRESS:
				enemy_rocksteady_start(object, routine_entry);
				break;
			case enemy_rocksteady_enter_ADDRESS:
				enemy_rocksteady_enter(object);
				break;
			case enemy_rocksteady_think_ADDRESS:
				enemy_rocksteady_think(object, routine_entry);
				break;
			case enemy_rocksteady_attack_ADDRESS:
				enemy_rocksteady_attack(object);
				break;
			case enemy_rocksteady_hit_ADDRESS:
				enemy_rocksteady_hit(object);
				break;
			case enemy_rocksteady_react_ADDRESS:
				enemy_rocksteady_react(object);
				break;
			case enemy_bebop_start_ADDRESS:
				enemy_bebop_start(object, routine_entry);
				break;
			case enemy_bebop_enter_ADDRESS:
				enemy_bebop_enter(object);
				break;
			case enemy_bebop_think_ADDRESS:
				enemy_bebop_think(routine_offset, object, routine_entry);
				break;
			case enemy_bebop_attack_ADDRESS:
				enemy_bebop_attack(object);
				break;
			case enemy_bebop_hit_ADDRESS:
				enemy_bebop_hit(object);
				break;
			case enemy_bebop_react_ADDRESS:
				enemy_bebop_react(object);
				break;
			case enemy_mouser_start_ADDRESS:
				enemy_mouser_start(object, routine_entry);
				break;
			case enemy_mouser_enter_ADDRESS:
				enemy_mouser_enter(object);
				break;
			case enemy_mouser_think_ADDRESS:
				enemy_mouser_think(object, routine_entry);
				break;
			case enemy_mouser_bite_ADDRESS:
				enemy_mouser_bite(object);
				break;
			case enemy_mouser_hit_ADDRESS:
				enemy_mouser_hit(object);
				break;
			case enemy_mouser_step_over_edge_ADDRESS:
				enemy_mouser_step_over_edge(object);
				break;
			case mouser_hole_start_ADDRESS:
				mouser_hole_start(object);
				break;
			case mouser_hole_wait_ADDRESS:
				mouser_hole_wait(object);
				break;
			case mouser_hole_open_ADDRESS:
				mouser_hole_open(object);
				break;
			case mouser_hole_let_out_ADDRESS:
				mouser_hole_let_out(object);
				break;
			case enemy_rodney_start_ADDRESS:
				enemy_rodney_start(object, routine_entry);
				break;
			case enemy_rodney_enter_ADDRESS:
				enemy_rodney_enter(object);
				break;
			case enemy_rodney_think_ADDRESS:
				enemy_rodney_think(object, routine_entry);
				break;
			case enemy_rodney_hit_ADDRESS:
				enemy_rodney_hit(object);
				break;
			case enemy_shredder_start_ADDRESS:
				enemy_shredder_start(object, routine_entry);
				break;
			case enemy_shredder_enter_ADDRESS:
				enemy_shredder_enter(object);
				break;
			case enemy_shredder_think_ADDRESS:
				enemy_shredder_think(object, routine_entry);
				break;
			case enemy_shredder_attack_ADDRESS:
				enemy_shredder_attack(object);
				break;
			case enemy_shredder_hit_ADDRESS:
				enemy_shredder_hit(object);
				break;
			case enemy_shredder_react_ADDRESS:
				enemy_shredder_react(object);
				break;
			}
			update_object_screen_place(object);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(40));
}

/* every frame: the players' slots counted and the least crowded one found */
void update_player_slots(void)
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
		object_loop_leftover = turtle;
		return;
	}
	per_turtle = enemies_on_screen;
	zero = m68k_divu_z(per_turtle, turtle_count);
	per_turtle = turtle_count == 0u ? per_turtle : m68k_divu(per_turtle, turtle_count);
	if (turtle_count == 0u)
		exception_hang();
	if (zero)
		per_turtle = (uint16_t)(per_turtle + 1u);
	enemies_per_turtle = per_turtle;
	object_loop_leftover = turtle;
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
uint32_t set_animation(uint32_t kept_bits, uint32_t object, uint32_t animation)
{
	kept_bits = (kept_bits & ~0xFFu) | ANIMATION_FRAME(animation)->time;
	kept_bits = (kept_bits & ~0xFFu) | (uint8_t)(kept_bits + OBJECT(object)->animation_delay);
	OBJECT(object)->frame_time = kept_bits;
	OBJECT(object)->frame = animation;
	return kept_bits;
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
		OBJECT(shot)->hit_sound = OBJECT(object)->hit_sound;
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
void speeds_from_direction(uint8_t direction, uint16_t speed, uint32_t *p_x_speed, uint32_t *p_y_speed)
{
	uint16_t angle = 0;
	uint32_t cosines = 0, sines = 0, x_speed = 0, y_speed = 0;
	uint32_t t1 = 0;

	sines = quarter_sine_ADDRESS;
	cosines = quarter_sine_last_ADDRESS;
	angle = (uint8_t)direction & 0x3Fu;
	if ((direction & 0x40u) == 0u)
	{
		t1 = sines;
		sines = cosines;
		cosines = t1;
		angle = (uint16_t)(0u - angle);
	}
	x_speed = ((uint32_t)SINE(sines + (int16_t)angle) * speed) >> 8;
	if ((int8_t)(direction + 0x40u) < 0)
		x_speed = (uint16_t)(0u - x_speed);
	y_speed = ((uint32_t)SINE(cosines + ((int16_t)(0u - angle))) * speed) >> 8;
	offset_high_word = y_speed;
	if ((int8_t)direction >= 0)
		y_speed = (uint16_t)(0u - y_speed);
	object_loop_leftover = sines;
	*p_x_speed = x_speed;
	*p_y_speed = y_speed;
}

/* Unused: speeds_from_direction's work without its last shift: the speeds across and up or down of a
   byte's speed in a direction, 256 times larger */
void speeds_from_direction_unscaled(uint8_t direction)
{
}

/* the direction from the object to another one */
uint32_t direction_to_object(uint32_t direction, uint32_t object, uint32_t object_2)
{
	uint16_t target_x = 0, target_y = 0;

	target_x = OBJECT(object_2)->screen_x;
	target_y = OBJECT(object_2)->screen_y;
	return direction_to(direction, target_x, target_y, object);
}
