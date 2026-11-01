/**
 *    Name:        scenes/intro.c
 *    Description: the story before area 0: the rooftop, "Fire!!", the climb, the burning roof
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the story before the first area starts */
void start_intro(void)
{
	uint16_t kind = 0;
	uint32_t data = 0;

	time_without_knockout = 0u;
	difficulty_from_time = 0u;
	prompt_shown(0) = 0u;
	prompt_shown(1) = 0u;
	prompt_shown(2) = 0u;
	prompt_shown(3) = 0u;
	cutscene_step = 0u;
	intro_over = 0u;
	balloon_time = 0u;
	balloons_said = 0u;
	cutscene_counter = 0u;
	LAYER(WORKING_LAYER)->x = 0x800u;
	LAYER(LAYER_A)->x = 0x800u;
	LAYER(LAYER_B)->x = 0x800u;
	LAYER(WORKING_LAYER)->y = 0x10000u;
	LAYER(LAYER_A)->y = 0x10000u;
	LAYER(LAYER_B)->y = 0x10000u;
	set_layers_scroll();
	start_box_opening();
	data = 0xA0EEu;
	kind = 0x20Fu;
	post_palette_request(kind, data);
}

/* every frame of the story: its balloon, its step, the layers' scroll */
void run_intro(void)
{
	uint16_t step_offset = 0;
	uint32_t ignored = 0, ignored_2 = 0;
	uint8_t t1 = 0;

	if (balloon_time != 0u)
	{
		t1 = balloon_time;
		balloon_time = t1 - 1u;
		if (t1 == 1u)
			erase_balloon(&ignored, &ignored_2);
	}
	step_offset = (uint16_t)((uint32_t)cutscene_step << 2);
	switch (step_offset)
	{
	case 0:
		intro_rooftop_start();
		break;
	case 4:
		intro_rooftop(step_offset);
		break;
	case 8:
		intro_climb_start();
		break;
	case 12:
		intro_climb();
		break;
	case 16:
		intro_roof_start();
		break;
	case 20:
		intro_roof_land();
		break;
	}
	set_layers_scroll();
}

/* the rooftop picture with Splinter and the four turtles by the burning building */
void intro_rooftop_start(void)
{
	uint32_t carry = 0;
	uint8_t colour = 0, leap_wait = 0, tile_attribute = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t animation = 0, animation_entry = 0, animations = 0, object = 0, tile_code = 0, tile_place = 0, tiles = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (palettes_busy != 0u)
		return;
	cutscene_time = 0x100u;
	tile_place = TILE_RAM + 0x4008u;
	tile_attribute = 0x24u;
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
			tile_code = (uint8_t)(tile_code + 1u);
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
	tile_place = TILE_RAM + 0x4038u;
	colour = 4u;
	colour_tiles_8x8(colour, tile_place);
	tile_place = TILE_RAM + 0x4438u;
	colour = 5u;
	colour_tiles_8x8(colour, tile_place);
	tile_place = TILE_RAM + 0x4838u;
	colour = 6u;
	colour_tiles_8x8(colour, tile_place);
	tile_place = TILE_RAM + 0x4C40u;
	colour = 7u;
	colour_tiles_4x4(colour, tile_place);
	tile_place = TILE_RAM + 0x4630u;
	colour = 5u;
	colour_tiles_4x4(colour, tile_place);
	tile_place = TILE_RAM + 0x4828u;
	colour = 6u;
	colour_tiles_8x4(colour, tile_place);
	tile_place = TILE_RAM + 0x4A28u;
	colour = 0x46u;
	colour_tiles_4x4(colour, tile_place);
	tile_place = TILE_RAM + 0x4A30u;
	colour = 6u;
	colour_tiles_4x4(colour, tile_place);
	tile_place = TILE_RAM + 0x4C30u;
	colour = 0x47u;
	colour_tiles_8x4(colour, tile_place);
	io_write8(TILES_106B00, 0xF0u);
	tile_banks_0_1 = 0xF0u;
	io_write8(TILES_106E00, 0x21u);
	tile_banks_2_3 = 0x21u;
	object = OBJECT_ADDRESS(44);
	OBJECT(OBJECT_ADDRESS(44))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(44))->height = 0u;
	OBJECT(OBJECT_ADDRESS(44))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(44))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(44))->screen_x = 0xA0u;
	OBJECT(OBJECT_ADDRESS(44))->screen_y = 0x58u;
	animation = april_on_rooftop_animation_ADDRESS;
	set_animation(tile_code, object, animation);
	object = OBJECT_ADDRESS(45);
	animations = intro_rooftop_animations_ADDRESS;
	leap_wait = 0x70u;
	do
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->screen_x = 0xC0u;
		OBJECT(object)->screen_y = 0x70u;
		OBJECT(object)->x_speed = 0x500;
		OBJECT(object)->y_speed &= 0xFFu;
		OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (leap_wait & 0xFFu);
		animation_entry = ANIMATION_ENTRY(animations);
		set_animation(tile_code, object, animation_entry);
		leap_wait += 0x14u;
		animations += 4u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(49));
	tile_place = TILE_RAM + 0x2B28u;
	tiles = rooftop_picture_tiles_ADDRESS;
	tile_attribute = 0x3Bu;
	count = 9u;
	do
	{
		count_2 = 0xFu;
		do
		{
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, PICTURE_TILE(tiles));
			t3 = count_2 - 1u;
			count_2 = t3;
			tile_place++;
			tiles++;
		} while (t3 != 0xFFFFu);
		t4 = count - 1u;
		count = t4;
		tile_place += 0x60u;
	} while (t4 != 0xFFFFu);
	cutscene_step++;
}

/* a square of 8 by 8 tiles given one colour */
void colour_tiles_8x8(uint8_t colour, uint32_t tile_place)
{
	uint16_t count = 0, count_2 = 0, row_skip = 0, width = 0;
	uint16_t t1 = 0, t2 = 0;

	row_skip = 0x70u;
	count = 7u;
	width = 7u;
	do
	{
		count_2 = (uint16_t)width;
		do
		{
			io_write8(tile_place, colour);
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place += 2u;
		} while (t1 != 0xFFFFu);
		tile_place += row_skip;
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
}

/* a square of 4 by 4 tiles given one colour */
void colour_tiles_4x4(uint8_t colour, uint32_t tile_place)
{
	uint16_t count = 0, count_2 = 0, row_skip = 0, width = 0;
	uint16_t t1 = 0, t2 = 0;

	row_skip = 0x78u;
	count = 3u;
	width = 3u;
	do
	{
		count_2 = (uint16_t)width;
		do
		{
			io_write8(tile_place, colour);
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place += 2u;
		} while (t1 != 0xFFFFu);
		tile_place += row_skip;
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
}

/* four rows of eight tiles given one colour */
void colour_tiles_8x4(uint8_t colour, uint32_t tile_place)
{
	uint16_t count = 0, count_2 = 0, row_skip = 0, width = 0;
	uint16_t t1 = 0, t2 = 0;

	row_skip = 0x70u;
	count = 3u;
	width = 7u;
	do
	{
		count_2 = (uint16_t)width;
		do
		{
			io_write8(tile_place, colour);
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place += 2u;
		} while (t1 != 0xFFFFu);
		tile_place += row_skip;
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
}

/* "Fire!!": the turtles leap off the rooftop one after another, "Hang on, April" */
void intro_rooftop(uint32_t height)
{
	uint8_t sound = 0, tile_attribute = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0, kind = 0, line = 0, screen_y = 0;
	uint32_t animation_entry = 0, animations = 0, data = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, ignored_5 = 0, ignored_6 = 0, object = 0, screen_x = 0, tile_place = 0;
	uint8_t t1 = 0;
	uint16_t height_2 = 0, x_speed = 0, t2 = 0, t3 = 0, t4 = 0;

	grow_tile_box();
	height = intro_animate_fire(height);
	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(45);
	animations = intro_rooftop_turtle_animations_ADDRESS;
	do
	{
		if ((uint8_t)OBJECT(object)->y_speed == 0u)
		{
			if ((OBJECT(object)->flags & 0x80u) != 0u)
			{
				turtles_on_the_way--;
				if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
				{
					OBJECT(object)->height += 0x100u;
					height = (uint16_t)OBJECT(object)->x_speed;
					height_2 = OBJECT(object)->height;
					OBJECT(object)->height = height_2 - height;
					if (height_2 < height)
						OBJECT(object)->flags &= 0x7Fu;
					OBJECT(object)->screen_x++;
					OBJECT(object)->screen_y--;
					OBJECT(object)->x_speed += 0x20u;
					if ((uint16_t)OBJECT(object)->x_speed >= 0x500u)
						OBJECT(object)->x_speed = 0x500;
				}
				else
				{
					if (object == OBJECT_ADDRESS(48) && (uint16_t)OBJECT(object)->x_speed == 0x360u)
					{
						io_write8(PRIORITY_0C0001, 4u);
						layer_priority = 4u;
					}
					OBJECT(object)->height += 0x100u;
					height = (uint16_t)OBJECT(object)->x_speed;
					OBJECT(object)->height += height;
					OBJECT(object)->screen_x++;
					OBJECT(object)->screen_y--;
					x_speed = OBJECT(object)->x_speed;
					OBJECT(object)->x_speed = x_speed - 0x20u;
					if (x_speed < 0x20u)
					{
						OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
						OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
					}
				}
			}
		}
		else
		{
			turtles_on_the_way--;
			t1 = (uint8_t)OBJECT(object)->y_speed;
			OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
			if (t1 == 1u)
			{
				animation_entry = ANIMATION_ENTRY(animations);
				set_animation(ignored_6, object, animation_entry);
			}
		}
		object += 0x50u;
		animations += 4u;
	} while (object < OBJECT_ADDRESS(49));
	if (turtles_on_the_way == 0u)
	{
		start_box_opening();
		tile_place = TILE_RAM + 0x2B28u;
		tile_attribute = 0x3Bu;
		tile_code = 0x41u;
		count = 9u;
		do
		{
			count_2 = 0xFu;
			do
			{
				io_write8(tile_place, tile_attribute);
				tile_place++;
				io_write8(tile_place, tile_code);
				t2 = count_2 - 1u;
				count_2 = t2;
				tile_place++;
			} while (t2 != 0xFFFFu);
			t3 = count - 1u;
			count = t3;
			tile_place += 0x60u;
		} while (t3 != 0xFFFFu);
		data = 0xA0FEu;
		kind = 0x20Fu;
		post_palette_request(kind, data);
		cutscene_step++;
		return;
	}
	t4 = cutscene_time;
	cutscene_time = t4 - 1u;
	if (t4 == 1u)
		cutscene_time++;
	if (cutscene_time == 0xD0u)
	{
		height &= ~0xFFu;
		screen_x = 0x108u;
		screen_y = 0x28u;
		line = 0x41u;
		show_balloon_beside(screen_y, line, &height, &screen_x, &ignored, &ignored_2, &ignored_3, &ignored_4, &ignored_5);
		draw_balloon_pointer(ignored_5);
		return;
	}
	if (cutscene_time == 0x90u)
	{
		sound = 0x7Du;
		queue_sound(sound);
		return;
	}
	if (cutscene_time != 0x70u)
		return;
	height &= ~0xFFu;
	screen_x = 0xA8u;
	screen_y = 0x28u;
	line = 0u;
	show_balloon_beside(screen_y, line, &height, &screen_x, &ignored, &ignored_2, &ignored_3, &ignored_4, &ignored_5);
	draw_balloon_pointer(ignored_5);
}

/* the picture of a tall building seen from below with the turtles at its foot */
void intro_climb_start(void)
{
	uint32_t carry = 0;
	uint8_t tile_attribute = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t animation = 0, object = 0, object_2 = 0, tile_code = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	if (palettes_busy != 0u)
		return;
	tile_place = TILE_RAM + 0x4008u;
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
			tile_code = (uint8_t)(tile_code + 1u);
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
	object = OBJECT_ADDRESS(44);
	object_2 = intro_object_animations_ADDRESS;
	count = 3u;
	do
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->screen_x = 0xF8u;
		OBJECT(object)->screen_y = 0xB8u;
		animation = INTRO_OBJECT(object_2)->animation;
		object_2 += 4u;
		OBJECT(object)->y_speed = INTRO_OBJECT(object_2 - 0x4u)->y_speed;
		object_2 += 2u;
		set_animation(tile_code, object, animation);
		t3 = count - 1u;
		count = t3;
		object += 0x50u;
	} while (t3 != 0xFFFFu);
	cutscene_step++;
}

/* the turtles leap up the building out of sight */
void intro_climb(void)
{
	uint16_t kind = 0;
	uint32_t data = 0, object = 0;

	grow_tile_box();
	object = OBJECT_ADDRESS(44);
	do
	{
		if (OBJECT(object)->frame_time == 0u && (OBJECT(object)->flags & 0x80u) != 0u)
		{
			OBJECT(object)->screen_y -= 8u;
			OBJECT(object)->screen_x += OBJECT(object)->y_speed;
			if (OBJECT(object)->screen_y < 0x80u)
				OBJECT(object)->flags &= 0x7Fu;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(48));
	if ((OBJECT(object - 0x50u)->flags & 0x80u) != 0u)
		return;
	start_box_opening();
	LAYER(WORKING_LAYER)->x = 0u;
	LAYER(LAYER_A)->x = 0u;
	LAYER(LAYER_B)->x = 0u;
	data = 0xA10Eu;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	cutscene_step++;
}

/* the burning roof with the four turtles dropping onto it */
void intro_roof_start(void)
{
	uint16_t routine_state = 0;
	uint32_t drop_wait = 0, effect_lists = 0, ignored = 0, ignored_2 = 0, kind = 0, object = 0, screen_x = 0, tile_place = 0;

	if (palettes_busy != 0u)
		return;
	io_write8(TILES_106B00, 0x30u);
	tile_banks_0_1 = 0x30u;
	LAYER(WORKING_LAYER)->map = layers_04618C_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = intro_roof_blocks_ADDRESS;
	LAYER(WORKING_LAYER)->map_width = 0x20u;
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	effect_lists = 0x9C08u;
	start_palette_effect_lists(effect_lists);
	object = OBJECT_ADDRESS(44);
	kind = 0u;
	screen_x = 0xB0u;
	drop_wait = 0x20u;
	do
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = kind;
		OBJECT(object)->height = 0xB000u;
		OBJECT(object)->screen_x = screen_x;
		OBJECT(object)->screen_y = 0x50u;
		OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (drop_wait & 0xFFu);
		OBJECT(object)->shadow = common_shadow_ADDRESS;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		routine_state = 0x203u;
		turtle_set_state(routine_state, object, &ignored, &kind, &screen_x, &drop_wait, &ignored_2);
		kind = (uint8_t)(kind + 1u);
		screen_x = (uint16_t)(screen_x - 0xCu);
		drop_wait = (uint8_t)(drop_wait + 0x10u);
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(48));
	cutscene_step++;
}

/* the turtles land on the roof, Michelangelo trips, and they walk off into the fire */
void intro_roof_land(void)
{
	uint16_t routine_state = 0;
	uint32_t animation = 0, ignored = 0, ignored_3 = 0, ignored_4 = 0, ignored_5 = 0, ignored_6 = 0, object = 0;
	uint8_t t1 = 0;
	uint16_t height = 0;

	grow_tile_box();
	intro_animate_fire(0u);
	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(44);
	do
	{
		if ((OBJECT(object)->flags & 0x80u) != 0u)
		{
			turtles_on_the_way--;
			if ((uint8_t)OBJECT(object)->y_speed == 0u)
			{
				if (OBJECT(object)->height == 0u)
				{
					OBJECT(object)->contacts = 0u;
					OBJECT(object)->screen_x += 2u;
					if (OBJECT(object)->screen_x >= 0x158u)
						OBJECT(object)->flags &= 0x7Fu;
				}
				else
				{
					OBJECT(object)->contacts = 0u;
					OBJECT(object)->screen_x += 3u;
					if (OBJECT(object)->screen_x >= 0xE0u)
						OBJECT(object)->contacts = 0x80u;
					height = OBJECT(object)->height;
					OBJECT(object)->height = height - 0x600u;
					if (height < 0x600u)
					{
						OBJECT(object)->height = 0u;
						if (OBJECT(object)->kind == 1u)
						{
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0x40u;
							animation = intro_turtle_lands_animation_ADDRESS;
							set_animation(ignored, object, animation);
						}
						else
						{
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0xCu;
							routine_state = 0x200u;
							turtle_set_state(routine_state, object, &ignored, &ignored_3, &ignored_4, &ignored_5, &ignored_6);
						}
					}
				}
			}
			else
			{
				t1 = (uint8_t)OBJECT(object)->y_speed;
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
				if (t1 == 1u && OBJECT(object)->height == 0u)
				{
					routine_state = 0x100u;
					turtle_set_state(routine_state, object, &ignored, &ignored_3, &ignored_4, &ignored_5, &ignored_6);
				}
			}
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(48));
	if (turtles_on_the_way != 0u)
		return;
	intro_over--;
}

/* the fire and its smoke in the story's pictures, a new picture every eight frames */
uint32_t intro_animate_fire(uint32_t d0)
{
	uint8_t next_part_code = 0, picture_tile = 0, tile_code = 0;
	uint16_t count_2 = 0, count_3 = 0, count_4 = 0, count_5 = 0;
	uint32_t count = 0, fire_frame = 0, frame = 0, tile_attribute = 0, tile_place = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0;

	frame = (d0 & ~0xFFu) | (uint8_t)frame_count;
	t1 = frame & 7u;
	frame = (frame & ~0xFFu) | t1;
	if (t1 != 0u)
		return frame;
	cutscene_counter += 4u;
	if (cutscene_counter >= 0xCu)
		cutscene_counter = 0u;
	if (cutscene_step != 1u)
	{
		fire_frame = TABLE32(intro_fire_frames, cutscene_counter);
		fire_frame = draw_tile_square_8x8(fire_frame);
		fire_frame = draw_tile_square_8x8(fire_frame);
		fire_frame = draw_tile_square_4x4(fire_frame);
		fire_frame = draw_tile_square_4x4(fire_frame);
		tile_place = PICTURE_TILE_32(fire_frame);
		fire_frame += 4u;
		picture_tile = PICTURE_TILE(fire_frame);
		count = 3u;
		fire_frame++;
		do
		{
			count_2 = 3u;
			do
			{
				mem_write8(tile_place, picture_tile);
				tile_place++;
				mem_write8(tile_place, PICTURE_TILE(fire_frame));
				t2 = count_2 - 1u;
				count_2 = t2;
				tile_place++;
				fire_frame++;
			} while (t2 != 0xFFFFu);
			t3 = count - 1u;
			count = t3;
			tile_place += 0x78u;
		} while (t3 != 0xFFFFu);
		return count;
	}
	if (cutscene_counter == 0u)
	{
		tile_place = TILE_RAM + 0x4038u;
		tile_attribute = 4u;
		tile_code = 0x18u;
		count_3 = 2u;
		do
		{
			count_4 = 7u;
			for (;;)
			{
				count_5 = 7u;
				do
				{
					io_write8(tile_place, tile_attribute);
					tile_place++;
					io_write8(tile_place, tile_code);
					tile_code++;
					t4 = count_5 - 1u;
					count_5 = t4;
					tile_place++;
				} while (t4 != 0xFFFFu);
				tile_code += 0x18u;
				t5 = count_4 - 1u;
				count_4 = t5;
				tile_place += 0x70u;
				if (t5 != 0xFFFFu)
					continue;
				tile_attribute = (uint8_t)(tile_attribute + 1u);
				if ((uint8_t)count_3 != 1u)
					break;
				count_3 = (uint16_t)((count_3 & ~0xFFu) | (uint8_t)(count_3 - 1u));
				count_4 = 3u;
			}
			t6 = count_3 - 1u;
			count_3 = t6;
		} while (t6 != 0xFFFFu);
		return tile_attribute;
	}
	tile_code = 0u;
	next_part_code = 0u;
	if (cutscene_counter != 4u)
	{
		tile_code = 0x80u;
		next_part_code = 0x20u;
	}
	tile_attribute = 0x1Au;
	tile_place = TILE_RAM + 0x4038u;
	count_3 = 1u;
	count_4 = 0xFu;
	for (;;)
	{
		count_5 = 7u;
		do
		{
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, tile_code);
			tile_code++;
			t7 = count_5 - 1u;
			count_5 = t7;
			tile_place++;
		} while (t7 != 0xFFFFu);
		t8 = count_4 - 1u;
		count_4 = t8;
		tile_place += 0x70u;
		if (t8 == 0xFFFFu)
		{
			count_4 = 3u;
			tile_attribute = 0x1Bu;
			tile_code = (uint8_t)next_part_code;
			t9 = count_3 - 1u;
			count_3 = t9;
			if (t9 != 0xFFFFu)
				continue;
			break;
		}
	}
	return tile_attribute;
}

/* a square of 8 by 8 tiles drawn, each row on from its first tile */
uint32_t draw_tile_square_8x8(uint32_t square)
{
	uint8_t picture_tile = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = PICTURE_TILE_32(square);
	square += 4u;
	picture_tile = PICTURE_TILE(square);
	count = 7u;
	square++;
	do
	{
		count_2 = 7u;
		tile_code = PICTURE_TILE(square);
		square++;
		do
		{
			mem_write8(tile_place, picture_tile);
			tile_place++;
			mem_write8(tile_place, tile_code);
			tile_code++;
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x70u;
	} while (t2 != 0xFFFFu);
	square++;
	return square;
}

/* a square of 4 by 4 tiles drawn, each row on from its first tile */
uint32_t draw_tile_square_4x4(uint32_t square)
{
	uint8_t picture_tile = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = PICTURE_TILE_32(square);
	square += 4u;
	picture_tile = PICTURE_TILE(square);
	count = 3u;
	square++;
	do
	{
		count_2 = 3u;
		tile_code = PICTURE_TILE(square);
		square++;
		do
		{
			mem_write8(tile_place, picture_tile);
			tile_place++;
			mem_write8(tile_place, tile_code);
			tile_code++;
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x78u;
	} while (t2 != 0xFFFFu);
	square++;
	return square;
}
