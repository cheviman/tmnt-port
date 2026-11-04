/**
 *    Name:        stage/drill_arrival.c
 *    Description: the drill pod that comes down through the ceiling in areas 1 and 7 and lets out Rocksteady or the flying drones
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the drill pod's arrival in areas 1 and 7, a step a frame */
uint32_t run_drill_arrival(uint32_t a1)
{
	if (drill_arrival < 0)
		return a1;
	switch (drill_arrival_step)
	{
	case 0:
		drill_arrival_wait();
		return a1;
	case 1:
		drill_arrival_rumble();
		return a1;
	case 2:
		return drill_arrival_descend(a1);
	case 3:
		drill_arrival_open();
		return a1;
	case 4:
		drill_arrival_end();
		return a1;
	}
	unreachable();
}

/* at its place in the area the scroll stops and the rumbling starts */
void drill_arrival_wait(void)
{
	uint8_t sound = 0;

	if (stage != 1u)
	{
		if (LAYER(LAYER_B)->x < 0x4B000u)
			return;
		hurry_time = 0u;
		drill_arrival = 1;
		scroll_held_right = 0xFFu;
		drill_arrival_time = 0xC0u;
		shake_step = 0u;
		sound = 0xFEu;
		queue_sound(sound);
		sound = 0x34u;
		queue_sound(sound);
		drill_rumbling = 0xFFu;
		drill_arrival_step++;
		return;
	}
	if (LAYER(LAYER_B)->x < 0xA000u)
		return;
	hurry_time = 0u;
	drill_arrival = 1;
	scroll_held_right = 0xFFu;
	drill_arrival_time = 0xC0u;
	shake_step = 0u;
	sound = 0xFEu;
	queue_sound(sound);
	sound = 0x34u;
	queue_sound(sound);
	drill_rumbling = 0xFFu;
	drill_arrival_step++;
}

/* the screen shakes, then the hole, the drill pod and its door appear */
void drill_arrival_rumble(void)
{
	uint8_t sound = 0;
	uint32_t animation = 0, object = 0, pod_x = 0, pod_y = 0;
	uint8_t t1 = 0;

	hurry_time = 0u;
	t1 = drill_arrival_time;
	drill_arrival_time = t1 - 1u;
	if (t1 != 1u)
	{
		shake_layers_and_objects();
		return;
	}
	sound = 0xCu;
	queue_sound(sound);
	object = OBJECT_ADDRESS(44);
	OBJECT(OBJECT_ADDRESS(44))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(45))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(46))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(44))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(45))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(46))->colour = 0xFFu;
	pod_y = 0x6500u;
	pod_x = DRILL_POD_X_AREA_1;
	if (stage != 1u)
	{
		pod_y = 0x8500u;
		pod_x = DRILL_POD_X_AREA_7;
	}
	pod_x -= LAYER(LAYER_B)->x;
	OBJECT(object)->x = pod_x;
	OBJECT(object + 0x50u)->x = pod_x;
	OBJECT(object + 0xA0u)->x = pod_x;
	pod_x = pod_x >> 8;
	OBJECT(object)->screen_x = pod_x;
	OBJECT(object + 0x50u)->screen_x = pod_x;
	OBJECT(object + 0xA0u)->screen_x = pod_x;
	OBJECT(object)->y = pod_y;
	pod_y += 0xE00u;
	OBJECT(object + 0x50u)->y = pod_y;
	pod_y -= 0xC00u;
	OBJECT(object + 0xA0u)->y = pod_y;
	pod_y = pod_y >> 8;
	OBJECT(object + 0xA0u)->screen_y = pod_y;
	pod_y = (uint16_t)(pod_y + 0xCu);
	OBJECT(object + 0x50u)->screen_y = pod_y;
	OBJECT(object)->screen_y = pod_y - 0xEu;
	animation = drill_ceiling_area_1_animation_ADDRESS;
	if (stage != 1u)
		animation = drill_ceiling_animation_ADDRESS;
	set_animation(pod_y, object, animation);
	animation = animations_02D796_ADDRESS;
	object += 0x50u;
	set_animation(pod_y, object, animation);
	animation = drill_pod_animation_ADDRESS;
	object += 0x50u;
	set_animation(pod_y, object, animation);
	shake_layers_and_objects();
	drill_arrival_step++;
}

/* the layers shake and the turtles and the drill pod stay where they are */
void shake_layers_and_objects(void)
{
	uint16_t screen_y = 0, shake_offset = 0;
	uint32_t object = 0, x_shake = 0, y_shake = 0;

	if ((frame_count & 1u) != 0u)
		return;
	shake_offset = shake_step;
	shake_step++;
	shake_offset = (uint16_t)((shake_offset & 7u) << 1);
	y_shake = (uint8_t)shake_y[shake_offset];
	x_shake = (uint16_t)shake_x[shake_offset];
	layer_a_scroll_x += x_shake;
	layer_b_scroll_x += x_shake;
	layer_a_scroll_y = (layer_a_scroll_y & 0xFF00u) | ((uint8_t)layer_a_scroll_y + y_shake & 0xFFu);
	layer_b_scroll_y = (layer_b_scroll_y & 0xFF00u) | ((uint8_t)layer_b_scroll_y + y_shake & 0xFFu);
	x_shake = (uint32_t)(int16_t)(0u - x_shake) << 8;
	y_shake = (uint32_t)(int16_t)(int8_t)y_shake << 8;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			OBJECT(object)->x += x_shake;
			OBJECT(object)->y += y_shake;
			screen_y = (uint16_t)(OBJECT(object)->y >> 8);
			OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
			OBJECT(object)->screen_y = screen_y;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	OBJECT(OBJECT_ADDRESS(44))->x += x_shake;
	OBJECT(OBJECT_ADDRESS(44))->y += y_shake;
	screen_y = (uint16_t)(OBJECT(OBJECT_ADDRESS(44))->y >> 8);
	OBJECT(OBJECT_ADDRESS(44))->screen_x = OBJECT(OBJECT_ADDRESS(44))->x >> 8;
	OBJECT(OBJECT_ADDRESS(44))->screen_y = screen_y;
	OBJECT(OBJECT_ADDRESS(45))->x += x_shake;
	OBJECT(OBJECT_ADDRESS(45))->y += y_shake;
	screen_y = (uint16_t)(OBJECT(OBJECT_ADDRESS(45))->y >> 8);
	OBJECT(OBJECT_ADDRESS(45))->screen_x = OBJECT(OBJECT_ADDRESS(45))->x >> 8;
	OBJECT(OBJECT_ADDRESS(45))->screen_y = screen_y;
	OBJECT(OBJECT_ADDRESS(46))->x += x_shake;
	OBJECT(OBJECT_ADDRESS(46))->y += y_shake;
	screen_y = (uint16_t)(OBJECT(OBJECT_ADDRESS(46))->y >> 8);
	OBJECT(OBJECT_ADDRESS(46))->screen_x = OBJECT(OBJECT_ADDRESS(46))->x >> 8;
	OBJECT(OBJECT_ADDRESS(46))->screen_y = screen_y;
}

/* the drill pod drills down through the ceiling into the room */
uint32_t drill_arrival_descend(uint32_t object)
{
	uint16_t screen_x = 0, screen_y = 0, screen_y_2 = 0;
	uint32_t object_2 = 0;

	hurry_time = 0u;
	shake_layers_and_objects();
	object_2 = OBJECT_ADDRESS(45);
	OBJECT(OBJECT_ADDRESS(46))->y += 0xC0u;
	OBJECT(OBJECT_ADDRESS(46))->screen_y = OBJECT(OBJECT_ADDRESS(46))->y >> 8;
	OBJECT(OBJECT_ADDRESS(45))->y += 0xC0u;
	screen_y = (uint16_t)(OBJECT(OBJECT_ADDRESS(45))->y >> 8);
	OBJECT(OBJECT_ADDRESS(45))->screen_y = screen_y;
	if (stage == 7u && OBJECT(object_2)->screen_y >= 0x9Eu && balloons_said == 1u)
	{
		object = OBJECT_ADDRESS(0);
		do
		{
			if ((OBJECT(object)->flags & 1u) != 0u)
			{
				screen_y &= ~0xFFu;
				screen_x = (uint16_t)(OBJECT(object)->screen_x - 0x40u);
				screen_x_high = screen_x;
				screen_y_2 = OBJECT(object)->screen_y;
				say_overgrown_cork_screw(screen_y, (uint8_t)screen_x, screen_y_2);
				break;
			}
			object += 0x50u;
		} while (object < OBJECT_ADDRESS(4));
	}
	screen_y = 0x124u;
	if (stage != 1u)
		screen_y = 0x144u;
	if (OBJECT(object_2)->screen_y <= screen_y)
		return object;
	OBJECT(object_2)->frame_time = 0u;
	drill_arrival_time = 0x70u;
	drill_arrival_step++;
	return object;
}

/* the drill pod's door opens and Rocksteady or the flying drones come out */
void drill_arrival_open(void)
{
	uint8_t sound = 0, time_left = 0;
	uint16_t open_y = 0, screen_y = 0;
	uint32_t object = 0, pod_x = 0, pod_y = 0;
	uint8_t t1 = 0, t2 = 0;

	hurry_time = 0u;
	t1 = drill_arrival_time;
	time_left = t1;
	if (t1 == 0u)
	{
		object = OBJECT_ADDRESS(46);
		OBJECT(OBJECT_ADDRESS(46))->y -= 0x200u;
		if (stage == 1u && OBJECT(object)->y == 0x11300u)
			start_rocksteady();
		screen_y = (uint16_t)(OBJECT(object)->y >> 8);
		OBJECT(object)->screen_y = screen_y;
		open_y = 0xA0u;
		if (stage != 1u)
			open_y = 0xC0u;
		if (open_y <= (uint16_t)screen_y)
			return;
		if (stage == 7u)
			release_flying_drones();
		drill_arrival_step++;
		return;
	}
	if (time_left >= 0x30u)
	{
		if (time_left != 0x30u)
			shake_layers_and_objects();
		else
		{
			drill_rumbling = 0u;
			sound = 0x35u;
			queue_sound(sound);
			if (stage != 1u)
			{
				sound = 0x66u;
				music = 0x66u;
				queue_sound(sound);
			}
			pod_y = 0x6500u;
			pod_x = DRILL_POD_X_AREA_1;
			if (stage != 1u)
			{
				pod_y = 0x8500u;
				pod_x = DRILL_POD_OPEN_X_AREA_7;
			}
			pod_x -= LAYER(LAYER_B)->x;
			OBJECT(OBJECT_ADDRESS(44))->x = pod_x;
			OBJECT(OBJECT_ADDRESS(45))->x = pod_x;
			OBJECT(OBJECT_ADDRESS(46))->x = pod_x;
			pod_x = pod_x >> 8;
			OBJECT(OBJECT_ADDRESS(44))->screen_x = pod_x;
			OBJECT(OBJECT_ADDRESS(45))->screen_x = pod_x;
			OBJECT(OBJECT_ADDRESS(46))->screen_x = pod_x;
			OBJECT(OBJECT_ADDRESS(44))->y = pod_y;
			pod_y += 0xC000u;
			OBJECT(OBJECT_ADDRESS(45))->y = pod_y;
			pod_y -= 0xC00u;
			OBJECT(OBJECT_ADDRESS(46))->y = pod_y;
			pod_y = pod_y >> 8;
			OBJECT(OBJECT_ADDRESS(46))->screen_y = pod_y;
			pod_y = (uint16_t)(pod_y + 0xCu);
			OBJECT(OBJECT_ADDRESS(45))->screen_y = pod_y;
			OBJECT(OBJECT_ADDRESS(44))->screen_y = pod_y - 0xC0u;
		}
	}
	t2 = drill_arrival_time;
	drill_arrival_time = t2 - 1u;
	if (t2 != 1u)
		return;
	if (stage == 7u)
		start_flying_drones();
	sound = 0x36u;
	queue_sound(sound);
}

/* the drill pod stays as part of the room and the scroll goes on */
void drill_arrival_end(void)
{
	uint16_t place = 0;
	uint32_t block = 0;

	hurry_time = 0u;
	scroll_held_right = 0u;
	drill_arrival = -1;
	OBJECT(OBJECT_ADDRESS(44))->flags = 0u;
	OBJECT(OBJECT_ADDRESS(45))->flags = 0u;
	OBJECT(OBJECT_ADDRESS(46))->flags = 0u;
	place = 3u;
	block = drill_hole_area_1_tile_block_ADDRESS;
	if (stage != 1u)
	{
		place = 0x1Cu;
		block = drill_hole_tile_block_ADDRESS;
	}
	draw_tile_block(place, block);
	place = 4u;
	offset_high_word = 4u;
	block = drill_hole_area_1_tile_block_2_ADDRESS;
	if (stage != 1u)
	{
		place = 0x1Du;
		offset_high_word = 0x1Du;
		block = drill_hole_tile_block_2_ADDRESS;
	}
	draw_tile_block(place, block);
}
