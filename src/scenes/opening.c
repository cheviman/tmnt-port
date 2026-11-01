/**
 *    Name:        scenes/opening.c
 *    Description: the attract mode's opening: the city at night, the manhole, the turtles leap out, their four pictures
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the opening's city at night and two clouds by the moon */
void start_opening(void)
{
	uint32_t animation = 0, ignored = 0, object = 0, tile_place = 0;

	fade_state = 0u;
	saved_turtle = 0xE0000000u;
	cutscene_step = 0u;
	LAYER(LAYER_A)->map = layers_03F196_ADDRESS;
	LAYER(LAYER_B)->map = layers_03E98A_ADDRESS;
	LAYER(LAYER_A)->blocks = opening_layer_a_blocks_ADDRESS;
	LAYER(LAYER_B)->blocks = opening_layer_b_blocks_ADDRESS;
	LAYER(LAYER_A)->x = 0u;
	LAYER(LAYER_B)->x = 0u;
	LAYER(LAYER_A)->y = 0x60000u;
	LAYER(LAYER_B)->y = 0x20000u;
	LAYER(LAYER_A)->map_width = 0x20u;
	LAYER(LAYER_B)->map_width = 0x20u;
	LAYER(LAYER_A)->x_speed = 0u;
	LAYER(LAYER_A)->y_speed = 0u;
	LAYER(LAYER_B)->x_speed = 0u;
	LAYER(LAYER_B)->y_speed = 0u;
	LAYER(LAYER_A)->row_phase = 0u;
	LAYER(LAYER_B)->row_phase = 0u;
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
	load_layer_a();
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
	save_layer_a();
	object = OBJECT_ADDRESS(0);
	OBJECT(OBJECT_ADDRESS(0))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(1))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(0))->screen_x = 0x88u;
	OBJECT(OBJECT_ADDRESS(0))->screen_y = 0xC4u;
	OBJECT(OBJECT_ADDRESS(1))->screen_x = 0x130u;
	OBJECT(OBJECT_ADDRESS(1))->screen_y = 0xA0u;
	OBJECT(OBJECT_ADDRESS(0))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(1))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(0))->height = 0u;
	OBJECT(OBJECT_ADDRESS(1))->height = 0u;
	OBJECT(OBJECT_ADDRESS(0))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(1))->colour = 0xFFu;
	animation = opening_smoke_and_turtle_animation_ADDRESS;
	set_animation(ignored, object, animation);
	animation = opening_smoke_animation_ADDRESS;
	object = OBJECT_ADDRESS(1);
	set_animation(ignored, object, animation);
}

/* every frame of the opening: its step, then the layers' scroll */
void run_opening(uint16_t d6, uint32_t a5)
{
	uint16_t step_number = 0;

	step_number = cutscene_step;
	switch ((uint8_t)((uint32_t)step_number << 2))
	{
	case 0:
		opening_city_fade_in(d6, a5);
		break;
	case 4:
		opening_sweep();
		break;
	case 8:
		opening_sweep_fast();
		break;
	case 12:
		opening_sweep_to_street();
		break;
	case 16:
		opening_turtles_ready();
		break;
	case 20:
		opening_turtles_leap_out();
		break;
	case 24:
		opening_portraits_start();
		break;
	case 28:
		opening_portraits_slide_in();
		break;
	case 32:
		opening_hold();
		break;
	}
	set_layers_scroll();
}

/* the city fades in and the clouds drift apart across the moon */
void opening_city_fade_in(uint16_t fade, uint32_t target_palettes)
{
	uint8_t sound = 0;
	uint8_t t1 = 0;

	if (fading == 0u)
	{
		target_palettes = 0xA0AEu;
		fade = 0xC02u;
		fade_state = 0u;
	}
	copy_palette_sets(fade, target_palettes);
	if ((frame_count & 3u) == 0u)
	{
		OBJECT(OBJECT_ADDRESS(0))->screen_x++;
		OBJECT(OBJECT_ADDRESS(1))->screen_x--;
	}
	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 == 1u)
	{
		next_opening_step();
		return;
	}
	if (cutscene_wait != 0x30u)
		return;
	sound = 0x7Du;
	queue_sound(sound);
}

/* the clouds leave and the view starts sweeping down the city */
void opening_sweep(void)
{
	uint32_t object = 0;

	if ((frame_count & 3u) == 0u)
	{
		OBJECT(OBJECT_ADDRESS(0))->screen_x++;
		OBJECT(OBJECT_ADDRESS(1))->screen_x--;
	}
	object = OBJECT_ADDRESS(0);
	OBJECT(OBJECT_ADDRESS(0))->screen_y += 2u;
	OBJECT(OBJECT_ADDRESS(1))->screen_y += 2u;
	if (OBJECT(OBJECT_ADDRESS(1))->screen_y >= 0x100u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object + 0x50u)->flags = 0u;
	}
	load_layer_b();
	LAYER(WORKING_LAYER)->y_speed = 0x200u;
	working_layer_tiles = TILE_RAM + 0x4000u;
	opening_scroll_layer();
	save_layer_b();
	load_layer_a();
	LAYER(WORKING_LAYER)->y_speed += 0x40u;
	if (LAYER(WORKING_LAYER)->y_speed >= 0x1000u)
		LAYER(WORKING_LAYER)->y_speed = 0xFFFu;
	working_layer_tiles = TILE_RAM + 0x2000u;
	opening_scroll_layer();
	save_layer_a();
	if (LAYER(LAYER_A)->y >= 0x30000u)
		return;
	next_opening_step();
}

/* the opening's next step */
void next_opening_step(void)
{
	cutscene_step++;
}

/* the view rushes down past the buildings */
void opening_sweep_fast(void)
{
	uint32_t y_speed = 0;

	load_layer_a();
	LAYER(WORKING_LAYER)->y_speed += 0x20u;
	if (LAYER(WORKING_LAYER)->y_speed >= 0x1000u)
		LAYER(WORKING_LAYER)->y_speed = 0xFFFu;
	y_speed = LAYER(WORKING_LAYER)->y_speed;
	LAYER(WORKING_LAYER)->y -= y_speed;
	LAYER(WORKING_LAYER)->row_phase -= y_speed << 4;
	if (LAYER(WORKING_LAYER)->y < 0x20000u)
	{
		LAYER(WORKING_LAYER)->y = (LAYER(WORKING_LAYER)->y & 0xFFFFu) | 0x20000u;
		cutscene_count++;
	}
	save_layer_a();
	if (cutscene_count < 3u || LAYER(LAYER_A)->y < 0x2C000u)
		return;
	next_opening_step();
}

/* the view slows down onto the street with the manhole */
void opening_sweep_to_street(void)
{
	uint32_t animation = 0, ignored = 0, object = 0;

	load_layer_a();
	if (LAYER(WORKING_LAYER)->y < 0x1A000u)
	{
		LAYER(WORKING_LAYER)->y_speed -= 0x60u;
		if (LAYER(WORKING_LAYER)->y_speed < 0x800u)
			LAYER(WORKING_LAYER)->y_speed = 0x800u;
	}
	working_layer_tiles = TILE_RAM + 0x2000u;
	opening_scroll_layer();
	save_layer_a();
	if (LAYER(WORKING_LAYER)->y < 0x14000u)
	{
		object = OBJECT_ADDRESS(0);
		OBJECT(OBJECT_ADDRESS(0))->flags = 0x81u;
		OBJECT(OBJECT_ADDRESS(0))->contacts = 0u;
		OBJECT(OBJECT_ADDRESS(0))->colour = 0xFFu;
		OBJECT(OBJECT_ADDRESS(0))->screen_x = 0x100u;
		OBJECT(OBJECT_ADDRESS(0))->height = 0u;
		OBJECT(OBJECT_ADDRESS(0))->screen_y = (0x11000u - LAYER(WORKING_LAYER)->y) >> 8;
		animation = opening_street_post_animation_ADDRESS;
		set_animation(0u, object, animation);
		object = OBJECT_ADDRESS(1);
		OBJECT(OBJECT_ADDRESS(1))->flags = 0x81u;
		OBJECT(OBJECT_ADDRESS(1))->contacts = 0u;
		OBJECT(OBJECT_ADDRESS(1))->colour = 0xFFu;
		OBJECT(OBJECT_ADDRESS(1))->screen_x = 0x100u;
		OBJECT(OBJECT_ADDRESS(1))->height = 0u;
		OBJECT(OBJECT_ADDRESS(1))->screen_y = (0x10000u - LAYER(WORKING_LAYER)->y) >> 8;
		animation = opening_manhole_cover_animation_ADDRESS;
		set_animation(ignored, object, animation);
		if (LAYER(WORKING_LAYER)->y == 0x10000u)
		{
			cutscene_wait = 0x30u;
			next_opening_step();
			return;
		}
	}
}

/* the four turtles wait under the manhole */
void opening_turtles_ready(void)
{
	uint16_t kind = 0;
	uint32_t animation = 0, object = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 != 1u)
		return;
	cutscene_wait = 0x40u;
	object = OBJECT_ADDRESS(6);
	OBJECT(OBJECT_ADDRESS(6))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(6))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(6))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(6))->screen_x = 0x100u;
	OBJECT(OBJECT_ADDRESS(6))->screen_y = 8u;
	animation = opening_turtles_ready_animation_ADDRESS;
	set_animation(0u, object, animation);
	object = OBJECT_ADDRESS(2);
	kind = 0u;
	do
	{
		OBJECT(object)->kind = (uint8_t)kind;
		OBJECT(object)->y_speed = opening_turtle_y_speeds[kind & 0x7FFFu];
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->screen_x = 0x100u;
		OBJECT(object)->screen_y = 0x14u;
		OBJECT(object)->x = 0x10000u;
		OBJECT(object)->y = 0x1400u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 8u;
		kind = (uint8_t)(kind + 1u);
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(6));
	next_opening_step();
}

/* the manhole cover blows off in a flash of light and the turtles leap out */
void opening_turtles_leap_out(void)
{
	uint8_t tile_attribute = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0, routine_state = 0, screen_y = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, ignored_5 = 0, object = 0, tile_place = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0, t3 = 0;

	if (cutscene_wait == 0u)
	{
		object = OBJECT_ADDRESS(2);
		do
		{
			OBJECT(object)->height += 0x240u;
			OBJECT(object)->x_speed += OBJECT(object)->y_speed;
			OBJECT(object)->x += OBJECT(object)->x_speed;
			screen_y = (uint16_t)(OBJECT(object)->y >> 8);
			OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
			OBJECT(object)->screen_y = screen_y;
			object += 0x50u;
		} while (object < OBJECT_ADDRESS(6));
		if (OBJECT(OBJECT_ADDRESS(2))->screen_x < 0xE0u)
			next_opening_step();
	}
	else
	{
		t1 = cutscene_wait;
		cutscene_wait = t1 - 1u;
		if (t1 == 1u)
		{
			object = OBJECT_ADDRESS(2);
			do
			{
				OBJECT(object)->flags = 0x81u;
				OBJECT(object)->contacts = 0u;
				routine_state = 0x204u;
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3, &ignored_4, &ignored_5);
				object += 0x50u;
			} while (object < OBJECT_ADDRESS(6));
			object = OBJECT_ADDRESS(2);
			do
			{
				OBJECT(object)->height += 0x240u;
				OBJECT(object)->x_speed += OBJECT(object)->y_speed;
				OBJECT(object)->x += OBJECT(object)->x_speed;
				screen_y = (uint16_t)(OBJECT(object)->y >> 8);
				OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
				OBJECT(object)->screen_y = screen_y;
				object += 0x50u;
			} while (object < OBJECT_ADDRESS(6));
			if (OBJECT(OBJECT_ADDRESS(2))->screen_x < 0xE0u)
				next_opening_step();
		}
		else if (cutscene_wait >= 0x20u)
		{
			if (cutscene_wait != 0x20u)
				return;
			object = OBJECT_ADDRESS(7);
			OBJECT(OBJECT_ADDRESS(7))->flags = 0x81u;
			OBJECT(OBJECT_ADDRESS(7))->contacts = 0u;
			OBJECT(OBJECT_ADDRESS(7))->colour = 0xFFu;
			OBJECT(OBJECT_ADDRESS(7))->animation_delay = 0u;
			OBJECT(OBJECT_ADDRESS(7))->screen_x = 0x100u;
			OBJECT(OBJECT_ADDRESS(7))->screen_y = 0x50u;
			animation = opening_turtles_leap_out_animation_ADDRESS;
			set_animation(0u, object, animation);
			OBJECT(OBJECT_ADDRESS(1))->height = 0x1000u;
			OBJECT(OBJECT_ADDRESS(6))->frame_time = 0u;
			return;
		}
	}
	tile_code = 0x3Cu;
	if (cutscene_wait != 0x1Cu)
	{
		if (cutscene_wait == 0x1Au)
			tile_code = 0x10u;
		else
		{
			tile_code = 0x3Cu;
			if (cutscene_wait != 0x10u)
			{
				if (cutscene_wait != 0xEu)
				{
					object = OBJECT_ADDRESS(1);
					if (OBJECT(OBJECT_ADDRESS(1))->flags != 0u)
					{
						OBJECT(object)->height += 0x1000u;
						if (OBJECT(object)->height_whole == 0xB0u)
						{
							OBJECT(object)->flags = 0u;
							OBJECT(OBJECT_ADDRESS(6))->frame_time = 1u;
						}
					}
					return;
				}
				tile_code = 0x10u;
			}
		}
	}
	tile_place = TILE_RAM + 0x118u;
	tile_attribute = 0u;
	count = 0x1Bu;
	do
	{
		count_2 = 0x13u;
		do
		{
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, tile_code);
			tile_place++;
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, tile_code);
			t2 = count_2 - 1u;
			count_2 = t2;
			tile_place++;
		} while (t2 != 0xFFFFu);
		t3 = count - 1u;
		count = t3;
		tile_place += 0x30u;
	} while (t3 != 0xFFFFu);
	object = OBJECT_ADDRESS(1);
	if (OBJECT(OBJECT_ADDRESS(1))->flags != 0u)
	{
		OBJECT(object)->height += 0x1000u;
		if (OBJECT(object)->height_whole == 0xB0u)
		{
			OBJECT(object)->flags = 0u;
			OBJECT(OBJECT_ADDRESS(6))->frame_time = 1u;
		}
	}
}

/* the screen cleared and the four turtles' pictures at its corners */
void opening_portraits_start(void)
{
	uint8_t blank_attribute = 0, flags = 0, space_tile = 0;
	uint32_t animation = 0, layer_a_place = 0, layer_b_place = 0, object = 0, portrait = 0, x = 0, y = 0;

	layer_a_place = TILE_RAM + 0x2000u;
	layer_b_place = TILE_RAM + 0x4000u;
	blank_attribute = 0u;
	space_tile = 0x10u;
	do
	{
		io_write8(layer_b_place, blank_attribute);
		layer_b_place++;
		io_write8(layer_b_place, space_tile);
		layer_b_place++;
		io_write8(layer_b_place, blank_attribute);
		layer_b_place++;
		io_write8(layer_b_place, space_tile);
		layer_b_place++;
	} while (layer_b_place < TILE_RAM + 0x5000u);
	do
	{
		io_write8(layer_a_place, blank_attribute);
		layer_a_place++;
		io_write8(layer_a_place, space_tile);
		layer_a_place++;
		io_write8(layer_a_place, blank_attribute);
		layer_a_place++;
		io_write8(layer_a_place, space_tile);
		layer_a_place++;
	} while (layer_a_place < TILE_RAM + 0x3000u);
	object = OBJECT_ADDRESS(0);
	flags = 0u;
	do
	{
		OBJECT(object)->flags = flags;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(19));
	object = OBJECT_ADDRESS(0);
	portrait = opening_portraits_ADDRESS;
	do
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->height = 0u;
		x = PORTRAIT(portrait)->x;
		portrait += 4u;
		y = PORTRAIT(portrait - 0x4u)->y;
		OBJECT(object)->x = x;
		OBJECT(object)->y = y;
		OBJECT(object)->screen_x = x >> 8;
		OBJECT(object)->screen_y = y >> 8;
		portrait += 4u;
		animation = PORTRAIT(portrait - 0x8u)->animation;
		portrait += 4u;
		set_animation(y, object, animation);
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	next_opening_step();
}

/* the four pictures slide in from the corners */
void opening_portraits_slide_in(void)
{
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t object = 0;

	object = OBJECT_ADDRESS(0);
	OBJECT(OBJECT_ADDRESS(0))->x += 0x400u;
	OBJECT(OBJECT_ADDRESS(1))->x -= 0x400u;
	OBJECT(OBJECT_ADDRESS(2))->x += 0x400u;
	OBJECT(OBJECT_ADDRESS(3))->x -= 0x400u;
	OBJECT(OBJECT_ADDRESS(0))->y -= 0x400u;
	OBJECT(OBJECT_ADDRESS(1))->y -= 0x400u;
	OBJECT(OBJECT_ADDRESS(2))->y += 0x400u;
	OBJECT(OBJECT_ADDRESS(3))->y += 0x400u;
	do
	{
		screen_x = (uint16_t)(OBJECT(object)->x >> 8);
		screen_y = (uint16_t)(OBJECT(object)->y >> 8);
		OBJECT(object)->screen_x = screen_x;
		OBJECT(object)->screen_y = screen_y;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if ((uint16_t)screen_x == 0x100u)
	{
		next_opening_step();
		return;
	}
	opening_hold();
}

/* the four pictures stay on the screen */
void opening_hold(void)
{
}

/* the opening's layer sweeps on by its speed and its new rows are drawn */
void opening_scroll_layer(void)
{
	uint8_t blocks_left = 0;
	uint16_t row_phase = 0, y_speed = 0;
	uint32_t block_row_y = 0, blocks = 0, map_entry = 0, map_place = 0, map_row_y = 0, row_y = 0, tile_place = 0;
	uint8_t t1 = 0;
	uint16_t row_phase_2 = 0;

	y_speed = LAYER(WORKING_LAYER)->y_speed;
	row_phase = (uint16_t)((uint32_t)y_speed << 4);
	LAYER(WORKING_LAYER)->y -= y_speed;
	if (LAYER(WORKING_LAYER)->y < 0x10000u)
	{
		LAYER(WORKING_LAYER)->y = 0x10000u;
		return;
	}
	row_phase_2 = LAYER(WORKING_LAYER)->row_phase;
	LAYER(WORKING_LAYER)->row_phase = row_phase_2 - row_phase;
	if (row_phase <= row_phase_2)
		return;
	row_y = LAYER(WORKING_LAYER)->y - 0xF000u;
	map_row_y = row_y;
	block_row_y = row_y;
	row_y &= 0xF000u;
	row_y = (uint16_t)(row_y + 0x1000u);
	row_y = (uint16_t)(0u - row_y);
	tile_place = (row_y >> 4) + working_layer_tiles;
	map_row_y = (map_row_y & 0xFFE000u) << 3;
	map_place = ((map_row_y << 16) | (map_row_y >> 16)) * LAYER(WORKING_LAYER)->map_width + LAYER(WORKING_LAYER)->map;
	block_row_y = block_row_y << 7;
	block_row_y = (((block_row_y << 16) | (block_row_y >> 16)) & 8u) ^ 8u;
	blocks_left = 0xBu;
	do
	{
		blocks = LAYER(WORKING_LAYER)->blocks;
		map_entry = MAP_ENTRY(map_place);
		io_write8(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 2u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 4u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 6u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x82u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x84u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x86u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		blocks += map_entry;
		io_write8(tile_place + 1u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 1u))));
		io_write8(tile_place + 3u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 2u))));
		io_write8(tile_place + 5u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 3u))));
		io_write8(tile_place + 7u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 4u))));
		io_write8(tile_place + 0x81u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 5u))));
		io_write8(tile_place + 0x83u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 6u))));
		io_write8(tile_place + 0x85u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 7u))));
		io_write8(tile_place + 0x87u, BLOCK_BYTE(blocks + ((int32_t)(block_row_y + 8u))));
		t1 = blocks_left;
		blocks_left = (uint8_t)(t1 - 1u);
		map_place += 2u;
		tile_place += 8u;
	} while (t1 != 1u);
}
