/**
 *    Name:        stage/layers.c
 *    Description: the areas' layers: their maps of blocks drawn as the scroll goes on
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a whole layer drawn from its map, and its floor cells into the floor map */
void draw_whole_layer_and_floor(uint32_t tile_place)
{
	uint8_t block_byte = 0, block_rows_left = 0, blocks_left = 0, tile_rows_left = 0;
	uint32_t block = 0, entry = 0, floor_place = 0, map = 0, page_offset = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	page_offset = LAYER(WORKING_LAYER)->x_page;
	page_offset = (uint8_t)(page_offset << 4);
	map = LAYER(WORKING_LAYER)->map + (uint16_t)(page_offset + (((uint32_t)LAYER(WORKING_LAYER)->y_page << 3) * LAYER(WORKING_LAYER)->map_width - LAYER(WORKING_LAYER)->map_width));
	floor_place = floor_map_ADDRESS;
	block_rows_left = 8u;
	do
	{
		blocks_left = 0x10u;
		entry = map;
		do
		{
			block = LAYER(WORKING_LAYER)->blocks + MAP_ENTRY(entry);
			tile_rows_left = 4u;
			block_byte = BLOCK_BYTE(block);
			entry += 2u;
			do
			{
				io_write8(tile_place, block_byte);
				io_write8(tile_place + 2u, block_byte);
				io_write8(tile_place + 4u, block_byte);
				io_write8(tile_place + 6u, block_byte);
				mem_write8(floor_place, BLOCK_BYTE(block + 0x1u));
				mem_write8(floor_place + 1u, BLOCK_BYTE(block + 0x2u));
				mem_write8(floor_place + 2u, BLOCK_BYTE(block + 0x3u));
				mem_write8(floor_place + 3u, BLOCK_BYTE(block + 0x4u));
				io_write8(tile_place + 1u, BLOCK_BYTE(block + 0x11u));
				io_write8(tile_place + 3u, BLOCK_BYTE(block + 0x12u));
				io_write8(tile_place + 5u, BLOCK_BYTE(block + 0x13u));
				io_write8(tile_place + 7u, BLOCK_BYTE(block + 0x14u));
				block += 4u;
				t1 = tile_rows_left;
				tile_rows_left = (uint8_t)(t1 - 1u);
				tile_place += 0x80u;
				floor_place += 0x40u;
			} while (t1 != 1u);
			t2 = blocks_left;
			blocks_left = (uint8_t)(t2 - 1u);
			tile_place -= 0x1F8u;
			floor_place -= 0xFCu;
		} while (t2 != 1u);
		map -= LAYER(WORKING_LAYER)->map_width;
		t3 = block_rows_left;
		block_rows_left = (uint8_t)(t3 - 1u);
		tile_place += 0x180u;
		floor_place += 0xC0u;
	} while (t3 != 1u);
}

/* a whole layer drawn from its map */
uint32_t draw_whole_layer(uint32_t tile_place)
{
	uint8_t block_rows_left = 0, blocks_left = 0, tile_rows_left = 0;
	uint32_t block = 0, entry = 0, map = 0, page_offset = 0, row_offset = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	page_offset = LAYER(WORKING_LAYER)->x_page;
	page_offset = (uint8_t)(page_offset << 4);
	row_offset = (uint32_t)(uint16_t)((uint32_t)LAYER(WORKING_LAYER)->y_page << 3) * LAYER(WORKING_LAYER)->map_width;
	row_offset = (row_offset & ~0xFFFFu) | (uint16_t)(row_offset - LAYER(WORKING_LAYER)->map_width);
	map = LAYER(WORKING_LAYER)->map + (uint16_t)(page_offset + row_offset);
	block_rows_left = 8u;
	do
	{
		blocks_left = 0x10u;
		entry = map;
		do
		{
			block = LAYER(WORKING_LAYER)->blocks + MAP_ENTRY(entry);
			tile_rows_left = 4u;
			row_offset = (row_offset & ~0xFFu) | BLOCK_BYTE(block);
			entry += 2u;
			do
			{
				io_write8(tile_place, row_offset);
				io_write8(tile_place + 2u, row_offset);
				io_write8(tile_place + 4u, row_offset);
				io_write8(tile_place + 6u, row_offset);
				io_write8(tile_place + 1u, BLOCK_BYTE(block + 0x1u));
				io_write8(tile_place + 3u, BLOCK_BYTE(block + 0x2u));
				io_write8(tile_place + 5u, BLOCK_BYTE(block + 0x3u));
				io_write8(tile_place + 7u, BLOCK_BYTE(block + 0x4u));
				t1 = tile_rows_left;
				tile_rows_left = (uint8_t)(t1 - 1u);
				tile_place += 0x80u;
				block += 4u;
			} while (t1 != 1u);
			t2 = blocks_left;
			blocks_left = (uint8_t)(t2 - 1u);
			tile_place -= 0x1F8u;
		} while (t2 != 1u);
		map -= LAYER(WORKING_LAYER)->map_width;
		t3 = block_rows_left;
		block_rows_left = (uint8_t)(t3 - 1u);
		tile_place += 0x180u;
	} while (t3 != 1u);
	return row_offset;
}

/* the area's layers from its record: the floor map, the first scroll segment and the layers drawn */
void start_area_layers(void)
{
	uint16_t area_offset = 0;

	scroll_segment = 0u;
	LAYER(LAYER_A)->column_phase = 0u;
	LAYER(LAYER_B)->column_phase = 0u;
	LAYER(LAYER_A)->row_phase = 0u;
	LAYER(LAYER_B)->row_phase = 0u;
	apartment_fire_spot = 0u;
	apartment_fire_frame = 0u;
	building_fire_spot = 0u;
	building_fire_frame = 0u;
	AT16(layer_loops, -1) = 0u;
	layers_step = 0u;
	AT32(last_segment, -3) = 0u;
	fire_last_frame = 0u;
	hurry_time = 0u;
	balloon_time = 0u;
	time_since_balloon = 0u;
	balloons_said = 0u;
	floor_end_walled = 0u;
	drill_arrival = 0;
	drill_arrival_step = 0u;
	drill_rumbling = 0u;
	unused_area_frames_copy(0) = 0u;
	unused_area_frames(0) = 0u;
	unused_area_frames(1) = 0u;
	unused_area_frames_copy(1) = 0u;
	unused_area_start_byte = 0u;
	tv_time = 0u;
	bebop_exchange_lines = 0u;
	prompt_shown(0) = 0u;
	prompt_shown(1) = 0u;
	prompt_shown(2) = 0u;
	prompt_shown(3) = 0u;
	area_offset = (uint16_t)((uint32_t)stage << 2);
	area_offset += (uint32_t)area_offset << 3;
	LAYER(LAYER_A)->x = area_layer_a_x[area_offset / 4u];
	LAYER(LAYER_A)->y = area_layer_a_y[area_offset / 4u];
	LAYER(LAYER_B)->x = area_layer_b_x[area_offset / 4u];
	LAYER(LAYER_B)->y = area_layer_b_y[area_offset / 4u];
	LAYER(LAYER_A)->map = area_layer_a_maps[area_offset / 4u];
	LAYER(LAYER_A)->blocks = area_layer_a_blocks[area_offset / 4u];
	LAYER(LAYER_B)->map = area_layer_b_maps[area_offset / 4u];
	LAYER(LAYER_B)->blocks = area_layer_b_blocks[area_offset / 4u];
	LAYER(LAYER_A)->map_width = area_layer_a_map_widths[area_offset / 2u];
	LAYER(LAYER_B)->map_width = area_layer_b_map_widths[area_offset / 2u];
	switch (stage)
	{
	case 0:
		start_layers_building();
		break;
	case 1:
		start_layers_apartment();
		break;
	case 2:
		start_layers_street();
		break;
	case 3:
		start_layers_sewer();
		break;
	case 4:
		start_layers_car_street();
		break;
	case 5:
		start_layers_highway();
		break;
	case 6:
		start_layers_skateboard();
		break;
	case 7:
		start_layers_building_site();
		break;
	case 8:
		start_layers_technodrome();
		break;
	case 9:
		start_layers_final();
		break;
	}
	set_layers_scroll();
}

/* the burning building's floor map and layer */
void start_layers_building(void)
{
	uint32_t cells = 0, floor_place = 0, tile_place = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x47Fu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
}

/* April's apartment's floor map, its left wall, April at the window, and the layer */
void start_layers_apartment(void)
{
	uint8_t columns_left = 0, rows_left = 0, wall_cell = 0;
	uint32_t animation = 0, april = 0, cells = 0, floor_place = 0, tile_place = 0, wall_column = 0, wall_height = 0, wall_place = 0;
	uint8_t t1 = 0, t2 = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x4BFu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	wall_column = floor_map_ADDRESS + 0x4u;
	columns_left = 6u;
	wall_height = 0x19u;
	wall_cell = 0xFDu;
	do
	{
		wall_place = wall_column;
		rows_left = (uint8_t)wall_height;
		wall_height = (uint8_t)(wall_height - 1u);
		wall_column++;
		do
		{
			mem_write8(wall_place, wall_cell);
			t1 = rows_left;
			rows_left = (uint8_t)(t1 - 1u);
			wall_place += 0x40u;
		} while (t1 != 1u);
		t2 = columns_left;
		columns_left = (uint8_t)(t2 - 1u);
	} while (t2 != 1u);
	april = OBJECT_ADDRESS(47);
	OBJECT(OBJECT_ADDRESS(47))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(47))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(47))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(47))->x = 0x12000u;
	OBJECT(OBJECT_ADDRESS(47))->screen_x = 0x120u;
	OBJECT(OBJECT_ADDRESS(47))->y = 0x6000u;
	OBJECT(OBJECT_ADDRESS(47))->screen_y = 0x60u;
	OBJECT(OBJECT_ADDRESS(47))->x_speed &= 0xFFu;
	animation = april_standing_animation_ADDRESS;
	set_animation(wall_height, april, animation);
	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
}

/* the street's layer and its floor map */
void start_layers_street(void)
{
	uint32_t tile_place = 0;

	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer_and_floor(tile_place);
	save_layer_b();
}

/* the sewer's floor map and both layers */
void start_layers_sewer(void)
{
	uint32_t cells = 0, floor_place = 0, tile_place = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x2FFu)
			continue;
		cells = 0x1010101u;
		if (floor_place < floor_map_ADDRESS + 0x4FFu)
			continue;
		cells = 0xFCFCFCFCu;
		if (floor_place < floor_map_ADDRESS + 0x5BFu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	start_scroll_segment();
	load_layer_a();
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
	save_layer_a();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
}

/* the street of the parked cars' floor map and layer */
void start_layers_car_street(void)
{
	uint32_t cells = 0, floor_place = 0, tile_place = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x47Fu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
}

/* the highway's floor map and layer */
void start_layers_highway(void)
{
	uint32_t cells = 0, floor_place = 0, tile_place = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x1FFu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
}

/* the skateboard ride's floor map and both layers */
void start_layers_skateboard(void)
{
	uint32_t cells = 0, floor_place = 0, tile_place = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x3BFu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
	load_layer_a();
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
	save_layer_a();
}

/* the building site's layers and floor map */
void start_layers_building_site(void)
{
	uint32_t tile_place = 0;

	start_scroll_segment();
	load_layer_a();
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
	save_layer_a();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer_and_floor(tile_place);
	save_layer_b();
}

/* the last area starts with Krang counted as an enemy, then as the Technodrome */
void start_layers_final(void)
{
	enemies_on_screen = 1u;
	start_layers_technodrome();
}

/* the Technodrome's floor map and layer */
void start_layers_technodrome(void)
{
	uint32_t cells = 0, floor_place = 0, tile_place = 0;

	floor_place = floor_map_ADDRESS;
	cells = 0xFFFFFFFFu;
	for (;;)
	{
		mem_write32(floor_place, cells);
		floor_place += 4u;
		if (floor_place < floor_map_ADDRESS + 0x3FFu)
			continue;
		cells = 0x1010101u;
		if (floor_place >= OBJECT_ADDRESS(0))
			break;
	}
	start_scroll_segment();
	load_layer_b();
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	save_layer_b();
}

/* layer A's variables into the working layer */
void load_layer_a(void)
{
	uint8_t longs_left = 0;
	uint32_t destination = 0, source = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	source = LAYER_A;
	destination = WORKING_LAYER;
	longs_left = 0x10u;
	do
	{
		t1 = destination;
		destination += 4u;
		mem_write32(t1, mem_read32(source));
		t2 = longs_left;
		longs_left = (uint8_t)(t2 - 1u);
		source += 4u;
	} while (t2 != 1u);
}

/* layer B's variables into the working layer */
void load_layer_b(void)
{
	uint8_t longs_left = 0;
	uint32_t destination = 0, source = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	source = LAYER_B;
	destination = WORKING_LAYER;
	longs_left = 0x10u;
	do
	{
		t1 = destination;
		destination += 4u;
		mem_write32(t1, mem_read32(source));
		t2 = longs_left;
		longs_left = (uint8_t)(t2 - 1u);
		source += 4u;
	} while (t2 != 1u);
}

/* the working layer back into layer A's variables */
void save_layer_a(void)
{
	uint8_t longs_left = 0;
	uint32_t destination = 0, source = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	destination = LAYER_A;
	source = WORKING_LAYER;
	longs_left = 0x10u;
	do
	{
		t1 = destination;
		destination += 4u;
		mem_write32(t1, mem_read32(source));
		t2 = longs_left;
		longs_left = (uint8_t)(t2 - 1u);
		source += 4u;
	} while (t2 != 1u);
}

/* the working layer back into layer B's variables */
void save_layer_b(void)
{
	uint8_t longs_left = 0;
	uint32_t destination = 0, source = 0;
	uint8_t t2 = 0;
	uint32_t t1 = 0;

	destination = LAYER_B;
	source = WORKING_LAYER;
	longs_left = 0x10u;
	do
	{
		t1 = destination;
		destination += 4u;
		mem_write32(t1, mem_read32(source));
		t2 = longs_left;
		longs_left = (uint8_t)(t2 - 1u);
		source += 4u;
	} while (t2 != 1u);
}

/* the row and the column the scroll has brought into view, with their floor cells */
void draw_new_row_and_column_and_floor(uint32_t *p_d5, uint32_t *p_blocks_left, uint32_t *p_floor_place)
{
	uint32_t d5 = *p_d5;
	uint32_t blocks_left = *p_blocks_left;
	uint32_t floor_place = *p_floor_place;
	uint8_t rows_left = 0, y_in_block = 0;
	uint32_t blocks = 0, column_map_y = 0, column_x = 0, floor_row_start = 0, last_block = 0, last_tile_place = 0, layer_blocks = 0, map = 0, map_column_x = 0, map_entry = 0, map_row_y = 0, next_tile_place = 0, row_map_x = 0, row_y = 0, tile_place = 0, tile_row_start = 0, x_offset = 0, y_offset = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (LAYER(WORKING_LAYER)->new_row != 0u)
	{
		row_y = LAYER(WORKING_LAYER)->new_row_y;
		map_row_y = row_y;
		d5 = row_y;
		x_offset = ((LAYER(WORKING_LAYER)->x - 0x2000u) & 0x1E000u) << 6;
		row_y &= 0xF800u;
		row_y = (uint16_t)(0u - row_y);
		row_y = row_y >> 4;
		row_y = (uint16_t)(row_y + ((x_offset << 16) | (x_offset >> 16)));
		tile_place = row_y + working_layer_tiles;
		floor_place = (row_y >> 1) + floor_map_ADDRESS;
		row_map_x = ((LAYER(WORKING_LAYER)->x - 0x2000u) & 0xFFE000u) << 4;
		map_row_y = ((map_row_y - 0x800u) & 0xFFE000u) << 3;
		map = ((row_map_x << 16) | (row_map_x >> 16)) + ((map_row_y << 16) | (map_row_y >> 16)) * LAYER(WORKING_LAYER)->map_width + LAYER(WORKING_LAYER)->map;
		d5 = (d5 - 0x800u) << 7;
		d5 = (d5 << 16) | (d5 >> 16);
		d5 = LINKED(d5, d5 & 0xCu);
		d5 = LINKED(d5, (uint16_t)(d5 ^ 0xCu));
		blocks_left = 0xDu;
		do
		{
			layer_blocks = LAYER(WORKING_LAYER)->blocks;
			map_entry = MAP_ENTRY(map);
			mem_write8(tile_place, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 2u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 4u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 6u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			layer_blocks += map_entry;
			mem_write8(floor_place, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 1u)));
			mem_write8(floor_place + 1u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 2u)));
			mem_write8(floor_place + 2u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 3u)));
			mem_write8(floor_place + 3u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 4u)));
			mem_write8(tile_place + 1u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x11u)));
			mem_write8(tile_place + 3u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x12u)));
			mem_write8(tile_place + 5u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x13u)));
			mem_write8(tile_place + 7u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x14u)));
			floor_row_start = floor_place;
			last_tile_place = tile_place;
			floor_place += 4u;
			tile_row_start = last_tile_place & ~0x7Fu;
			floor_row_start = floor_row_start & ~0x3Fu;
			map += 2u;
			tile_place += 8u;
			if (((last_tile_place + 8u) & 0xFF80u) != (uint16_t)tile_row_start)
			{
				tile_place = tile_row_start;
				floor_place = floor_row_start;
			}
			t1 = blocks_left;
			blocks_left = (uint8_t)(t1 - 1u);
		} while (t1 != 1u);
	}
	if (LAYER(WORKING_LAYER)->new_column != 0u)
	{
		column_x = LAYER(WORKING_LAYER)->new_column_x;
		map_column_x = column_x;
		d5 = column_x;
		y_offset = (LAYER(WORKING_LAYER)->y + 0x1800u) & 0xE000u;
		column_x = (column_x & 0x1F800u) << 6;
		column_x = (column_x << 16) | (column_x >> 16);
		column_x = (uint16_t)(column_x + ((uint16_t)(0u - y_offset) >> 4));
		tile_place = column_x + working_layer_tiles;
		floor_place = (column_x >> 1) + floor_map_ADDRESS;
		column_map_y = ((LAYER(WORKING_LAYER)->y - 0x800u) & 0xFFE000u) << 3;
		map_column_x = (map_column_x & 0xFFE000u) << 4;
		map = ((column_map_y << 16) | (column_map_y >> 16)) * LAYER(WORKING_LAYER)->map_width + ((map_column_x << 16) | (map_column_x >> 16)) + LAYER(WORKING_LAYER)->map;
		d5 = LINKED(d5, (uint16_t)d5 >> 8);
		d5 = LINKED(d5, d5 & 0x18u);
		d5 = LINKED(d5, (uint16_t)d5 >> 3);
		blocks_left = 8u;
		do
		{
			layer_blocks = LAYER(WORKING_LAYER)->blocks;
			map_entry = MAP_ENTRY(map);
			mem_write8(tile_place, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 0x80u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 0x100u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 0x180u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			layer_blocks += map_entry;
			mem_write8(floor_place, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 1u)));
			mem_write8(floor_place + 0x40u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 5u)));
			mem_write8(floor_place + 0x80u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 9u)));
			mem_write8(floor_place + 0xC0u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0xDu)));
			mem_write8(tile_place + 1u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x11u)));
			mem_write8(tile_place + 0x81u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x15u)));
			mem_write8(tile_place + 0x101u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x19u)));
			mem_write8(tile_place + 0x181u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0x1Du)));
			map -= LAYER(WORKING_LAYER)->map_width;
			floor_place = ((floor_place + 0x100u) & 0xFF1FFFu) | 0x61800u;
			next_tile_place = LINKED(tile_place, (uint16_t)(tile_place + 0x200u));
			tile_place = next_tile_place & ~0x9000u;
			t2 = blocks_left;
			blocks_left = (uint8_t)(t2 - 1u);
		} while (t2 != 1u);
		blocks = LAYER(WORKING_LAYER)->blocks;
		map_entry = MAP_ENTRY(map);
		t3 = LAYER(WORKING_LAYER)->y_pixel & 0x18u;
		y_in_block = t3;
		if (t3 != 0u)
		{
			last_block = blocks + map_entry;
			mem_write8(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
			mem_write8(floor_place, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 1u)));
			mem_write8(tile_place + 1u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 0x11u)));
			t4 = 4u - (y_in_block >> 3);
			rows_left = (uint8_t)(t4 - 1u);
			if (t4 != 1u)
			{
				mem_write8(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
				mem_write8(floor_place + 0x40u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 5u)));
				mem_write8(tile_place + 0x81u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 0x15u)));
				if (rows_left != 1u)
				{
					mem_write8(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
					mem_write8(floor_place + 0x80u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 9u)));
					mem_write8(tile_place + 0x101u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 0x19u)));
				}
			}
		}
	}
	*p_d5 = d5;
	*p_blocks_left = blocks_left;
	*p_floor_place = floor_place;
}

/* the row and the column of a layer the scroll has brought into view */
void draw_new_row_and_column(uint32_t *p_d1, uint32_t *p_d5, uint32_t *p_blocks_left)
{
	uint32_t d1 = *p_d1;
	uint32_t d5 = *p_d5;
	uint32_t blocks_left = *p_blocks_left;
	uint8_t rows_left = 0, y_in_block = 0;
	uint32_t blocks = 0, column_map_y = 0, column_x = 0, last_block = 0, last_tile_place = 0, layer_blocks = 0, map = 0, map_column_x = 0, map_entry = 0, map_row_y = 0, next_tile_place = 0, row_map_x = 0, row_y = 0, tile_place = 0, tile_row_start = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (LAYER(WORKING_LAYER)->new_row != 0u)
	{
		row_y = LAYER(WORKING_LAYER)->new_row_y;
		map_row_y = row_y;
		d5 = row_y;
		d1 = ((LAYER(WORKING_LAYER)->x - 0x2000u) & 0x1E000u) << 6;
		d1 = (d1 << 16) | (d1 >> 16);
		row_y &= 0xF800u;
		row_y = (uint16_t)(0u - row_y);
		row_y = row_y >> 4;
		tile_place = (uint16_t)(row_y + d1) + working_layer_tiles;
		row_map_x = ((LAYER(WORKING_LAYER)->x - 0x2000u) & 0xFFE000u) << 4;
		map_row_y = ((map_row_y - 0x800u) & 0xFFE000u) << 3;
		map = ((row_map_x << 16) | (row_map_x >> 16)) + ((map_row_y << 16) | (map_row_y >> 16)) * LAYER(WORKING_LAYER)->map_width + LAYER(WORKING_LAYER)->map;
		d5 = (d5 - 0x800u) << 7;
		d5 = (d5 << 16) | (d5 >> 16);
		d5 = LINKED(d5, d5 & 0xCu);
		d5 = LINKED(d5, (uint16_t)(d5 ^ 0xCu));
		blocks_left = 0xDu;
		do
		{
			layer_blocks = LAYER(WORKING_LAYER)->blocks;
			map_entry = MAP_ENTRY(map);
			mem_write8(tile_place, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 2u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 4u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 6u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			layer_blocks += map_entry;
			mem_write8(tile_place + 1u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 1u)));
			mem_write8(tile_place + 3u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 2u)));
			mem_write8(tile_place + 5u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 3u)));
			mem_write8(tile_place + 7u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 4u)));
			last_tile_place = tile_place;
			tile_row_start = last_tile_place & ~0x7Fu;
			map += 2u;
			tile_place += 8u;
			if (((last_tile_place + 8u) & 0xFF80u) != (uint16_t)tile_row_start)
				tile_place = tile_row_start;
			t1 = blocks_left;
			blocks_left = (uint8_t)(t1 - 1u);
		} while (t1 != 1u);
	}
	if (LAYER(WORKING_LAYER)->new_column != 0u)
	{
		column_x = LAYER(WORKING_LAYER)->new_column_x;
		map_column_x = column_x;
		d5 = column_x;
		d1 = (LAYER(WORKING_LAYER)->y + 0x1800u) & 0xE000u;
		d1 = (uint16_t)(0u - d1) >> 4;
		column_x = (column_x & 0x1F800u) << 6;
		column_x = (column_x << 16) | (column_x >> 16);
		tile_place = (uint16_t)(column_x + d1) + working_layer_tiles;
		column_map_y = ((LAYER(WORKING_LAYER)->y - 0x800u) & 0xFFE000u) << 3;
		map_column_x = (map_column_x & 0xFFE000u) << 4;
		map = ((column_map_y << 16) | (column_map_y >> 16)) * LAYER(WORKING_LAYER)->map_width + ((map_column_x << 16) | (map_column_x >> 16)) + LAYER(WORKING_LAYER)->map;
		d5 = LINKED(d5, (uint16_t)d5 >> 8);
		d5 = LINKED(d5, d5 & 0x18u);
		d5 = LINKED(d5, (uint16_t)d5 >> 3);
		blocks_left = 8u;
		do
		{
			layer_blocks = LAYER(WORKING_LAYER)->blocks;
			map_entry = MAP_ENTRY(map);
			mem_write8(tile_place, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 0x80u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 0x100u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			mem_write8(tile_place + 0x180u, BLOCK_BYTE(layer_blocks + (int16_t)map_entry));
			layer_blocks += map_entry;
			mem_write8(tile_place + 1u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 1u)));
			mem_write8(tile_place + 0x81u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 5u)));
			mem_write8(tile_place + 0x101u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 9u)));
			mem_write8(tile_place + 0x181u, BLOCK_BYTE(layer_blocks + ((d5 & 0xFFFFu) + 0xDu)));
			map -= LAYER(WORKING_LAYER)->map_width;
			next_tile_place = LINKED(tile_place, (uint16_t)(tile_place + 0x200u));
			tile_place = next_tile_place & ~0x9000u;
			t2 = blocks_left;
			blocks_left = (uint8_t)(t2 - 1u);
		} while (t2 != 1u);
		blocks = LAYER(WORKING_LAYER)->blocks;
		map_entry = MAP_ENTRY(map);
		t3 = LAYER(WORKING_LAYER)->y_pixel & 0x18u;
		y_in_block = t3;
		if (t3 != 0u)
		{
			last_block = blocks + map_entry;
			mem_write8(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
			mem_write8(tile_place + 1u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 1u)));
			t4 = 4u - (y_in_block >> 3);
			rows_left = (uint8_t)(t4 - 1u);
			if (t4 != 1u)
			{
				mem_write8(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
				mem_write8(tile_place + 0x81u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 5u)));
				if (rows_left != 1u)
				{
					mem_write8(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
					mem_write8(tile_place + 0x101u, BLOCK_BYTE(last_block + ((d5 & 0xFFFFu) + 9u)));
				}
			}
		}
	}
	*p_d1 = d1;
	*p_d5 = d5;
	*p_blocks_left = blocks_left;
}

/* every frame of play: the area's layers, and the HURRY! sign and the missiles when the players dawdle */
void update_area_layers(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1)
{
	uint16_t stage_offset = 0;
	uint32_t animation = 0, object = 0;

	time_since_balloon++;
	unused_area_frames_copy(0)++;
	unused_area_frames(0)++;
	unused_area_frames(1)++;
	unused_area_frames_copy(1)++;
	stage_offset = (uint16_t)((uint32_t)stage << 2);
	switch (stage_offset)
	{
	case 0:
		update_layers_building(stage_offset, p_d5, p_d7, p_a1);
		break;
	case 4:
		update_layers_apartment(p_d5, p_d7, p_a1);
		break;
	case 8:
		update_layers_street(stage_offset, p_d5, p_d7, p_a1);
		break;
	case 12:
		update_layers_sewer(p_d5, p_d7, p_a1);
		break;
	case 16:
		update_layers_car_street(p_d5, p_d7);
		break;
	case 20:
		update_layers_highway(p_d5, p_d7, p_a1);
		break;
	case 24:
		update_layers_skateboard(p_d5, p_d7);
		break;
	case 28:
		update_layers_building_site(p_d5, p_d7, p_a1);
		break;
	case 32:
		update_layers_technodrome(p_d5, p_d7, p_a1);
		break;
	case 36:
		update_layers_final(p_d5, p_d7);
		break;
	}
	if ((uint8_t)(LAYER(LAYER_A)->x_direction | LAYER(LAYER_B)->x_direction | last_segment) != 0u)
	{
		hurry_time = 0u;
		OBJECT(OBJECT_ADDRESS(49))->flags = 0u;
		return;
	}
	hurry_time++;
	if (enemies_on_screen >= 3u)
	{
		hurry_time = 0u;
		OBJECT(OBJECT_ADDRESS(49))->flags = 0u;
		return;
	}
	if ((uint8_t)(hurry_time >> 8u) == 0u)
	{
		OBJECT(OBJECT_ADDRESS(49))->flags = 0u;
		return;
	}
	object = OBJECT_ADDRESS(49);
	if (OBJECT(OBJECT_ADDRESS(49))->flags == 0u)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->screen_x = 0x158u;
		OBJECT(object)->screen_y = 0u;
		animation = hurry_arrow_animation_ADDRESS;
		if ((uint8_t)diagonal_scroll != 0u)
			animation = hurry_arrow_down_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	if (enemies_on_screen != 0u)
	{
		hurry_time = 0x100u;
		return;
	}
	if (hurry_time < 0x600u)
	{
		return;
	}
	rain_missiles_on_turtles(p_d5, p_d7);
	hurry_time = 0u;
	OBJECT(OBJECT_ADDRESS(49))->flags = 0u;
}

/* the two layers' places into the scroll the frame shows */
void set_layers_scroll(void)
{
	layer_a_scroll_x = (LAYER(LAYER_A)->x >> 8) & 0x1FFu;
	layer_b_scroll_x = (LAYER(LAYER_B)->x >> 8) & 0x1FFu;
	layer_a_scroll_y = (layer_a_scroll_y & 0xFF00u) | (0u - LAYER(LAYER_A)->y_pixel & 0xFFu);
	layer_b_scroll_y = (layer_b_scroll_y & 0xFF00u) | (0u - LAYER(LAYER_B)->y_pixel & 0xFFu);
}

/* the burning building's layer, the first cry of attack, and the wall at its end */
void update_layers_building(uint16_t on_right, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_wall_place)
{
	uint32_t wall_place = *p_wall_place;
	uint8_t columns_left = 0, rows_left = 0, wall_cell = 0, wall_height = 0;
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t ignored = 0, object = 0, wall_column = 0;
	uint8_t t1 = 0, t2 = 0;

	if (balloons_said == 0u && time_since_balloon == 0x30u)
	{
		object = OBJECT_ADDRESS(0);
		do
		{
			if ((OBJECT(object)->flags & 1u) != 0u)
			{
				on_right &= ~0xFFu;
				if ((OBJECT(object)->flags & 0x40u) != 0u)
					on_right |= (uint8_t)(on_right - 1u);
				screen_x = OBJECT(object)->screen_x;
				screen_y = OBJECT(object)->screen_y;
				say_attack(on_right, screen_x, screen_y, p_d5, p_d7);
				break;
			}
			object += 0x50u;
		} while (object < OBJECT_ADDRESS(4));
	}
	LAYER(LAYER_A)->x_direction = 0;
	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	set_layers_scroll();
	if (floor_end_walled == 0u && LAYER(LAYER_B)->x >= 0x3C000u)
	{
		wall_column = floor_map_ADDRESS + 0x24u;
		columns_left = 0xCu;
		wall_height = 0x12u;
		wall_cell = 0xFEu;
		do
		{
			rows_left = (uint8_t)wall_height;
			wall_height++;
			wall_place = wall_column;
			wall_column++;
			do
			{
				mem_write8(wall_place, wall_cell);
				t1 = rows_left;
				rows_left = (uint8_t)(t1 - 1u);
				wall_place += 0x40u;
			} while (t1 != 1u);
			t2 = columns_left;
			columns_left = (uint8_t)(t2 - 1u);
		} while (t2 != 1u);
		floor_end_walled--;
	}
	*p_wall_place = wall_place;
}

/* the apartment's layer, the wall at its end, and April */
void update_layers_apartment(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_wall_place)
{
	uint32_t wall_place = *p_wall_place;
	uint8_t columns_left = 0, rows_left = 0, wall_cell = 0, wall_height = 0;
	uint32_t ignored = 0, wall_column = 0;
	uint8_t t1 = 0, t2 = 0;

	if (floor_end_walled == 0u && LAYER(LAYER_B)->x >= 0x2000u)
	{
		wall_column = floor_map_ADDRESS + 0x34u;
		columns_left = 8u;
		wall_height = 0x13u;
		wall_cell = 0xFEu;
		do
		{
			rows_left = (uint8_t)wall_height;
			wall_height++;
			wall_place = wall_column;
			wall_column++;
			do
			{
				mem_write8(wall_place, wall_cell);
				t1 = rows_left;
				rows_left = (uint8_t)(t1 - 1u);
				wall_place += 0x40u;
			} while (t1 != 1u);
			t2 = columns_left;
			columns_left = (uint8_t)(t2 - 1u);
		} while (t2 != 1u);
		floor_end_walled--;
	}
	LAYER(LAYER_A)->x_direction = 0;
	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	set_layers_scroll();
	run_drill_arrival(p_d5, p_d7, &wall_place);
	april_wait_for_rescue(&ignored, p_d5, p_d7);
	*p_wall_place = wall_place;
}

/* April at the window cries for help and paces, kept at her place */
void april_wait_for_rescue(uint32_t *p_d1, uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d5 = *p_d5;
	uint16_t screen_y = 0;
	uint32_t april = 0, on_right = 0;

	april = OBJECT_ADDRESS(47);
	if (balloons_said == 0u && time_since_balloon == 0x30u)
	{
		on_right = 0x7Fu;
		*p_d1 = (*p_d1 & ~0xFFFFu) | OBJECT(april)->screen_x;
		screen_y = 0x70u;
		say_help_me(screen_y, &on_right, p_d1, &d5, p_d7);
	}
	if (OBJECT(april)->frame_time == 0x30u)
	{
		if (OBJECT(april)->april.stepping_left != 0u)
			OBJECT(april)->x -= 0x400u;
		else
			OBJECT(april)->x += 0x400u;
		OBJECT(april)->april.stepping_left ^= 0xFFu;
	}
	OBJECT(april)->x -= (int16_t)LAYER(LAYER_B)->x_speed;
	OBJECT(april)->screen_x = OBJECT(april)->x >> 8;
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
}

/* the street's layers: the TV in the shop window shows April and Shredder, and a turtle's answer */
void update_layers_street(uint32_t stage_offset, uint32_t *p_d5, uint32_t *p_count, uint32_t *p_right_half_place)
{
	uint32_t right_half_place = *p_right_half_place;
	uint32_t carry = 0;
	uint8_t space_tile = 0;
	uint16_t count = 0, screen_x_2 = 0, screen_y = 0;
	uint32_t left_half_place = 0, object = 0, screen_x = 0, tiles = 0, tv_frame = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	LAYER(LAYER_A)->x_direction = 0;
	LAYER(LAYER_A)->x = LAYER(LAYER_B)->x;
	LAYER(LAYER_A)->y = LAYER(LAYER_B)->y;
	if (LAYER(LAYER_B)->x >= 0x12000u)
	{
		if (LAYER(LAYER_B)->x < 0x1E000u)
		{
			if (tv_time != 0u)
			{
				if (tv_time == 0x10u)
				{
					stage_offset = 0x7Fu;
					screen_y = 0x70u;
					screen_x = (0x26000u - LAYER(LAYER_B)->x) >> 8;
					say_help_me(screen_y, &stage_offset, &screen_x, p_d5, p_count);
				}
				else if (tv_time == 0xA0u)
				{
					tiles = tv_picture_tiles_ADDRESS;
					stage_offset = 8u;
					draw_tv_picture((uint8_t)stage_offset, tiles, p_count, &right_half_place);
				}
				else if (tv_time == 0xB0u)
				{
					stage_offset = 0x7Fu;
					screen_y = 0x70u;
					screen_x_2 = (uint16_t)((0x26000u - LAYER(LAYER_B)->x) >> 8);
					say_turtle_soup(screen_x_2, screen_y, &stage_offset, p_d5, p_count);
				}
				else if (tv_time == 0x170u)
				{
					left_half_place = TILE_RAM + 0x2478u;
					right_half_place = TILE_RAM + 0x2400u;
					stage_offset &= ~0xFFu;
					space_tile = 0x10u;
					*p_count = 7u;
					do
					{
						count = 3u;
						do
						{
							io_write8(right_half_place, stage_offset);
							io_write8(left_half_place, stage_offset);
							right_half_place++;
							io_write8(right_half_place, space_tile);
							left_half_place++;
							io_write8(left_half_place, space_tile);
							t1 = count - 1u;
							count = t1;
							right_half_place++;
							left_half_place++;
						} while (t1 != 0xFFFFu);
						t2 = *p_count - 1u;
						*p_count = t2;
						left_half_place += 0x78u;
						right_half_place += 0x78u;
					} while (t2 != 0xFFFFu);
				}
				else if (tv_time >= 0x10u && tv_time < 0xA0u)
				{
					stage_offset = tv_time;
					if ((stage_offset & 7u) == 0u)
					{
						stage_offset = (stage_offset & 0x18u) >> 1;
						tv_frame = TABLE32(street_tv_pictures, stage_offset);
						animate_tv_picture(tv_frame, p_count, &right_half_place);
					}
				}
			}
			else
			{
				tiles = record_01F0D8_ADDRESS;
				stage_offset = 0x28u;
				draw_tv_picture((uint8_t)stage_offset, tiles, p_count, &right_half_place);
			}
			t3 = tv_time;
			carry = (uint16_t)(t3 + 1u) < t3;
			tv_time = t3 + 1u;
			if (carry)
				tv_time--;
		}
		else if (tv_time < 0x170u)
		{
			tv_time = 0x170u;
			left_half_place = TILE_RAM + 0x2478u;
			right_half_place = TILE_RAM + 0x2400u;
			stage_offset &= ~0xFFu;
			space_tile = 0x10u;
			*p_count = 7u;
			do
			{
				count = 3u;
				do
				{
					io_write8(right_half_place, stage_offset);
					io_write8(left_half_place, stage_offset);
					right_half_place++;
					io_write8(right_half_place, space_tile);
					left_half_place++;
					io_write8(left_half_place, space_tile);
					t1 = count - 1u;
					count = t1;
					right_half_place++;
					left_half_place++;
				} while (t1 != 0xFFFFu);
				t2 = *p_count - 1u;
				*p_count = t2;
				left_half_place += 0x78u;
				right_half_place += 0x78u;
			} while (t2 != 0xFFFFu);
			t3 = tv_time;
			carry = (uint16_t)(t3 + 1u) < t3;
			tv_time = t3 + 1u;
			if (carry)
				tv_time--;
		}
	}
	if (bebop_exchange_lines != 1u || time_since_balloon < 0xA0u)
	{
		load_layer_b();
		working_layer_tiles = TILE_RAM + 0x4000u;
		draw_new_row_and_column_and_floor(p_d5, p_count, &right_half_place);
		save_layer_b();
		set_layers_scroll();
		*p_right_half_place = right_half_place;
		return;
	}
	object = OBJECT_ADDRESS(0);
	while ((OBJECT(object)->flags & 1u) == 0u)
	{
		object += 0x50u;
		if (object >= OBJECT_ADDRESS(4))
		{
			load_layer_b();
			working_layer_tiles = TILE_RAM + 0x4000u;
			draw_new_row_and_column_and_floor(p_d5, p_count, &right_half_place);
			save_layer_b();
			set_layers_scroll();
			*p_right_half_place = right_half_place;
			return;
		}
	}
	screen_x_2 = OBJECT(object)->screen_x;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		screen_x_2 -= 0x18u;
	screen_y = (uint16_t)(OBJECT(object)->screen_y + 8u);
	stage_offset &= ~0xFFu;
	say_they_aint_getting_prettier(stage_offset, screen_x_2, screen_y, p_d5, p_count);
	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column_and_floor(p_d5, p_count, &right_half_place);
	save_layer_b();
	set_layers_scroll();
	*p_right_half_place = right_half_place;
}

/* a picture on the TV in the shop window */
void draw_tv_picture(uint8_t tile_attribute, uint32_t tiles, uint32_t *p_count, uint32_t *p_right_half_place)
{
	uint16_t count_2 = 0;
	uint32_t count = 0, left_half_place = 0, right_half_place = 0;
	uint16_t t4 = 0, t5 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	left_half_place = TILE_RAM + 0x2478u;
	right_half_place = TILE_RAM + 0x2400u;
	count = 7u;
	do
	{
		count_2 = 3u;
		do
		{
			io_write8(right_half_place, tile_attribute);
			t1 = left_half_place;
			left_half_place++;
			io_write8(t1, tile_attribute);
			right_half_place++;
			io_write8(right_half_place, PICTURE_TILE(tiles + 0x4u));
			t2 = tiles;
			tiles++;
			t3 = left_half_place;
			left_half_place++;
			io_write8(t3, PICTURE_TILE(t2));
			t4 = count_2 - 1u;
			count_2 = t4;
			right_half_place++;
		} while (t4 != 0xFFFFu);
		right_half_place += 0x78u;
		tiles += 4u;
		t5 = count - 1u;
		count = t5;
		left_half_place += 0x78u;
	} while (t5 != 0xFFFFu);
	*p_count = count;
	*p_right_half_place = right_half_place;
}

/* April talking on the TV */
void animate_tv_picture(uint32_t tv_frame, uint32_t *p_count, uint32_t *p_right_half_place)
{
	uint32_t count = 0, left_half_place = 0, right_half_place = 0;
	uint16_t t1 = 0;

	left_half_place = TILE_RAM + 0x25FDu;
	right_half_place = TILE_RAM + 0x2581u;
	count = 2u;
	do
	{
		io_write8(left_half_place, PICTURE_TILE(tv_frame));
		io_write8(left_half_place + 2u, PICTURE_TILE(tv_frame + 0x1u));
		io_write8(right_half_place, PICTURE_TILE(tv_frame + 0x2u));
		right_half_place += 0x80u;
		tv_frame += 3u;
		t1 = count - 1u;
		count = t1;
		left_half_place += 0x80u;
	} while (t1 != 0xFFFFu);
	*p_count = count;
	*p_right_half_place = right_half_place;
}

/* the sewer's two layers and the wall at its end */
void update_layers_sewer(uint32_t *p_d5, uint32_t *p_columns_left, uint32_t *p_wall_column)
{
	uint32_t columns_left = *p_columns_left;
	uint32_t wall_column = *p_wall_column;
	uint8_t rows_left = 0, wall_cell = 0;
	uint32_t ignored = 0, tiles = 0, wall_place = 0;
	uint8_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	if (floor_end_walled == 0u && LAYER(LAYER_B)->x >= 0x30000u)
	{
		wall_column = floor_map_ADDRESS + 0x10u;
		tiles = sewer_water_tiles_ADDRESS;
		wall_cell = 0xFEu;
		columns_left = 0x10u;
		do
		{
			wall_place = wall_column;
			t1 = tiles;
			tiles++;
			rows_left = PICTURE_TILE(t1);
			wall_column++;
			do
			{
				mem_write8(wall_place, wall_cell);
				t2 = rows_left;
				rows_left = (uint8_t)(t2 - 1u);
				wall_place += 0x40u;
			} while (t2 != 1u);
			t3 = columns_left;
			columns_left = (uint8_t)(t3 - 1u);
		} while (t3 != 1u);
		floor_end_walled--;
	}
	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, &columns_left);
	save_layer_b();
	LAYER(LAYER_A)->x = LAYER(LAYER_B)->x;
	LAYER(LAYER_A)->new_column_x = LAYER(LAYER_B)->new_column_x;
	LAYER(LAYER_A)->y = LAYER(LAYER_B)->y;
	LAYER(LAYER_A)->new_row_y = LAYER(LAYER_B)->new_row_y;
	LAYER(LAYER_A)->new_row = LAYER(LAYER_B)->new_row;
	LAYER(LAYER_A)->new_column = LAYER(LAYER_B)->new_column;
	load_layer_a();
	working_layer_tiles = TILE_RAM + 0x2000u;
	draw_new_row_and_column(&ignored, p_d5, &columns_left);
	save_layer_a();
	set_layers_scroll();
	*p_columns_left = columns_left;
	*p_wall_column = wall_column;
}

/* the street's layer, and April tied up at its end */
void update_layers_car_street(uint32_t *p_d5, uint32_t *p_d7)
{
	uint8_t frame_time = 0;
	uint32_t animation = 0, data = 0, ignored = 0, object = 0;

	LAYER(LAYER_A)->x_direction = 0;
	if (LAYER(LAYER_B)->x < 0x33000u)
	{
		load_layer_b();
		working_layer_tiles = TILE_RAM + 0x4000u;
		draw_new_row_and_column(&ignored, p_d5, p_d7);
		save_layer_b();
		set_layers_scroll();
		return;
	}
	object = OBJECT_ADDRESS(44);
	if (OBJECT(OBJECT_ADDRESS(44))->flags == 0u)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->screen_y = 0xF0u;
		animation = car_street_bystanders_animation_ADDRESS;
		set_animation(0u, object, animation);
		data = 0xCA04u;
		*p_d7 = (*p_d7 & ~0xFFFFu) | 0x43Du;
		post_palette_request((uint16_t)*p_d7, data);
	}
	OBJECT(object)->screen_x = (0x4D000u - LAYER(LAYER_B)->x) >> 8;
	if ((uint8_t)frame_count != 0u)
	{
		load_layer_b();
		working_layer_tiles = TILE_RAM + 0x4000u;
		draw_new_row_and_column(&ignored, p_d5, p_d7);
		save_layer_b();
		set_layers_scroll();
		return;
	}
	frame_time = 0u;
	if (OBJECT(object)->frame_time == 0u)
		frame_time = 0x10u;
	OBJECT(object)->frame_time = frame_time;
	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	set_layers_scroll();
}

/* the last area's layer */
void update_layers_final(uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t ignored = 0;

	LAYER(LAYER_A)->x_direction = 0;
	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	set_layers_scroll();
}

/* the building site's layers, and Splinter hanging tied at its end */
void update_layers_building_site(uint32_t *p_d5, uint32_t *p_count, uint32_t *p_a1)
{
	uint32_t count = *p_count;
	uint8_t blank_attribute = 0;
	uint16_t x_speed = 0;
	uint32_t animation = 0, object = 0, space_tile = 0, tile_place = 0;
	uint8_t x_direction = 0;
	uint16_t t1 = 0;

	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column_and_floor(p_d5, &count, p_a1);
	save_layer_b();
	x_direction = LAYER(LAYER_B)->x_direction;
	LAYER(LAYER_A)->x_direction = x_direction;
	if (x_direction != 0u)
	{
		x_speed = LAYER(LAYER_B)->x_speed;
		LAYER(LAYER_A)->x += (int16_t)(x_speed + (x_speed >> 2));
	}
	set_layers_scroll();
	if (LAYER(LAYER_B)->x < 0x60000u)
		run_drill_arrival(p_d5, &count, p_a1);
	else
	{
		if (io_read16(TILES_102030) != 0x10u)
		{
			tile_place = TILES_102030;
			blank_attribute = 0u;
			space_tile = 0x10u;
			count = 0x1Fu;
			do
			{
				io_write8(tile_place, blank_attribute);
				tile_place++;
				io_write8(tile_place, space_tile);
				tile_place++;
				io_write8(tile_place, blank_attribute);
				tile_place++;
				io_write8(tile_place, space_tile);
				tile_place++;
				io_write8(tile_place, blank_attribute);
				tile_place++;
				io_write8(tile_place, space_tile);
				tile_place++;
				io_write8(tile_place, blank_attribute);
				tile_place++;
				io_write8(tile_place, space_tile);
				t1 = count - 1u;
				count = t1;
				tile_place += 0x79u;
			} while (t1 != 0xFFFFu);
			io_write8(TILES_102EAE, blank_attribute);
			io_write8(TILES_102EAF, space_tile);
			io_write8(TILES_102EB8, blank_attribute);
			io_write8(TILES_102EB9, space_tile);
		}
		if (LAYER(LAYER_B)->x >= 0x68800u)
		{
			object = OBJECT_ADDRESS(44);
			OBJECT(OBJECT_ADDRESS(44))->screen_x = (0x84000u - LAYER(LAYER_B)->x) >> 8;
			if (OBJECT(OBJECT_ADDRESS(44))->flags != 0u)
			{
				if ((uint8_t)frame_count == 0u)
				{
					animation = splinter_tied_up_animation_ADDRESS;
					if (OBJECT(object)->frame_time == 0u)
						animation = splinter_struggles_animation_ADDRESS;
					set_animation(space_tile, object, animation);
				}
			}
			else
			{
				OBJECT(object)->flags = 0x81u;
				OBJECT(object)->contacts = 0x80u;
				OBJECT(object)->colour = 0xFFu;
				OBJECT(object)->animation_delay = 0u;
				OBJECT(object)->screen_y = 0x60u;
				OBJECT(object)->height = 0x3800u;
				OBJECT(object)->shadow = common_shadow_ADDRESS;
				animation = splinter_struggles_animation_ADDRESS;
				set_animation(space_tile, object, animation);
			}
		}
		else
			run_drill_arrival(p_d5, &count, p_a1);
	}
	*p_count = count;
}

/* the Technodrome's layers by the step: to the lift, the ride, off the lift */
void update_layers_technodrome(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1)
{
	switch (layers_step)
	{
	case 0:
		technodrome_reach_lift(p_d5, p_d7);
		break;
	case 1:
		technodrome_ride_lift(p_d5, p_d7, p_a1);
		break;
	case 2:
		technodrome_leave_lift(p_d5, p_d7, p_a1);
		break;
	}
	set_layers_scroll();
}

/* the layers up to the lift, and the ride starts when every turtle is on it */
void technodrome_reach_lift(uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t animation = 0, ignored = 0, object = 0;

	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	if (LAYER(LAYER_B)->x < 0x42000u)
	{
		return;
	}
	LAYER(LAYER_A)->x = LAYER(LAYER_B)->x - 0x40000u;
	LAYER(LAYER_A)->new_column_x = LAYER(LAYER_B)->new_column_x - 0x40000u;
	LAYER(LAYER_A)->new_column = LAYER(LAYER_B)->new_column;
	load_layer_a();
	working_layer_tiles = TILE_RAM + 0x2000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_a();
	if (LAYER(LAYER_B)->x < 0x58000u)
	{
		return;
	}
	scroll_held_right = 0xFFu;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x <= OBJECT(object)->y - 0x2000u + 0x9000u)
		{
			return;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	scroll_mode = -1;
	LAYER(LAYER_B)->y_direction = -0x80;
	LAYER(LAYER_B)->x_direction = -0x80;
	LAYER(LAYER_B)->x_speed = 0u;
	LAYER(LAYER_B)->y_speed = 0u;
	object = OBJECT_ADDRESS(44);
	OBJECT(OBJECT_ADDRESS(44))->flags = 1u;
	OBJECT(OBJECT_ADDRESS(44))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(44))->colour = 0xFFu;
	animation = technodrome_lift_animation_ADDRESS;
	set_animation(ignored, object, animation);
	*p_d7 = (*p_d7 & ~0xFFu) | 0x21u;
	queue_sound((uint8_t)*p_d7);
	layers_step++;
}

/* the lift's ride: the scroll speeds up, loops the shaft, slows down and stops */
void technodrome_ride_lift(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_wall_place)
{
	uint32_t d7 = *p_d7;
	uint32_t wall_place = *p_wall_place;
	uint8_t columns_left = 0, rows_left = 0, wall_cell = 0, wall_height = 0;
	uint32_t ignored = 0, wall_column = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	if (layer_loops < 0)
	{
		if (LAYER(WORKING_LAYER)->x < 0xA0000u)
		{
			if (LAYER(WORKING_LAYER)->x + (int16_t)LAYER(WORKING_LAYER)->x_speed >= 0xA2800u && layer_loops >= 0)
			{
				LAYER(WORKING_LAYER)->x -= 0x40000u;
				LAYER(WORKING_LAYER)->y += 0x40000u;
				t1 = layer_loops + 1u;
				layer_loops = t1;
				if ((int8_t)t1 >= 0)
					layer_loops = -0x80;
			}
		}
		else
		{
			if (LAYER(WORKING_LAYER)->x_speed < 0x120u && LAYER(WORKING_LAYER)->x_speed >= 0x110u)
			{
				d7 = (d7 & ~0xFFu) | 0x29u;
				queue_sound((uint8_t)d7);
			}
			LAYER(WORKING_LAYER)->x_speed -= 0x10u;
			LAYER(WORKING_LAYER)->y_speed -= 0x10u;
			if (LAYER(WORKING_LAYER)->x_speed >= 0x100u)
			{
				if (LAYER(WORKING_LAYER)->x + (int16_t)LAYER(WORKING_LAYER)->x_speed >= 0xA2800u && layer_loops >= 0)
				{
					LAYER(WORKING_LAYER)->x -= 0x40000u;
					LAYER(WORKING_LAYER)->y += 0x40000u;
					t1 = layer_loops + 1u;
					layer_loops = t1;
					if ((int8_t)t1 >= 0)
						layer_loops = -0x80;
				}
			}
			else
			{
				LAYER(WORKING_LAYER)->x_speed = 0x100u;
				LAYER(WORKING_LAYER)->y_speed = 0x100u;
			}
		}
	}
	else
	{
		LAYER(WORKING_LAYER)->x_speed += 4u;
		LAYER(WORKING_LAYER)->y_speed += 4u;
		if (LAYER(WORKING_LAYER)->x_speed < 0x400u)
		{
			if (LAYER(WORKING_LAYER)->x + (int16_t)LAYER(WORKING_LAYER)->x_speed >= 0xA2800u && layer_loops >= 0)
			{
				LAYER(WORKING_LAYER)->x -= 0x40000u;
				LAYER(WORKING_LAYER)->y += 0x40000u;
				t1 = layer_loops + 1u;
				layer_loops = t1;
				if ((int8_t)t1 >= 0)
					layer_loops = -0x80;
			}
		}
		else
		{
			LAYER(WORKING_LAYER)->x_speed = 0x400u;
			LAYER(WORKING_LAYER)->y_speed = 0x400u;
			if (LAYER(WORKING_LAYER)->x + (int16_t)LAYER(WORKING_LAYER)->x_speed >= 0xA2800u && layer_loops >= 0)
			{
				LAYER(WORKING_LAYER)->x -= 0x40000u;
				LAYER(WORKING_LAYER)->y += 0x40000u;
				t1 = layer_loops + 1u;
				layer_loops = t1;
				if ((int8_t)t1 >= 0)
					layer_loops = -0x80;
			}
		}
	}
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, &d7);
	save_layer_b();
	place_lift_edge();
	if (scroll_mode >= 0)
	{
		d7 = (d7 & ~0xFFu) | 0x2Bu;
		queue_sound((uint8_t)d7);
		wall_column = floor_map_ADDRESS + 0x30u;
		columns_left = 0x10u;
		wall_height = 0x20u;
		wall_cell = 0xFDu;
		do
		{
			wall_place = wall_column;
			rows_left = (uint8_t)wall_height;
			wall_height--;
			wall_column++;
			do
			{
				mem_write8(wall_place, wall_cell);
				t2 = rows_left;
				rows_left = (uint8_t)(t2 - 1u);
				wall_place += 0x40u;
			} while (t2 != 1u);
			t3 = columns_left;
			columns_left = (uint8_t)(t3 - 1u);
		} while (t3 != 1u);
		layers_step++;
	}
	*p_d7 = d7;
	*p_wall_place = wall_place;
}

/* the layers after the ride, the floor map set again and the wall at the end */
void technodrome_leave_lift(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1)
{
	uint8_t columns_left = 0, rows = 0, rows_left = 0, wall_cell = 0, wall_height = 0;
	uint32_t cells = 0, floor_row = 0, ignored = 0, wall_column = 0, x = 0;
	uint8_t t1 = 0, t2 = 0;

	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	LAYER(LAYER_A)->x = LAYER(LAYER_B)->x - 0xA0000u;
	LAYER(LAYER_A)->new_column_x = LAYER(LAYER_B)->new_column_x - 0xA0000u;
	LAYER(LAYER_A)->new_column = LAYER(LAYER_B)->new_column;
	load_layer_a();
	working_layer_tiles = TILE_RAM + 0x2000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_a();
	place_lift_edge();
	x = LAYER(LAYER_B)->x;
	if (x >= 0xC0000u)
	{
		if (x < 0xC0200u)
		{
			floor_row = floor_map_ADDRESS + 0x30u;
			if (floor_map(48) != 0xFFu)
			{
				cells = 0xFFFFFFFFu;
				rows = 0x10u;
				fill_floor_map_rows(rows, cells, floor_row);
				floor_row = floor_map_ADDRESS + 0x430u;
				cells = 0x1010101u;
				rows = 0x10u;
				fill_floor_map_rows(rows, cells, floor_row);
			}
			return;
		}
		if (floor_end_walled == 0u && LAYER(LAYER_B)->x >= 0xC9000u)
		{
			wall_column = floor_map_ADDRESS + 0x38u;
			columns_left = 0x10u;
			wall_height = 0x10u;
			wall_cell = 0xFEu;
			do
			{
				rows_left = (uint8_t)wall_height;
				wall_height++;
				*p_a1 = wall_column;
				wall_column++;
				if (wall_column >= floor_map_ADDRESS + 0x40u)
					wall_column = floor_map_ADDRESS;
				do
				{
					mem_write8(*p_a1, wall_cell);
					t1 = rows_left;
					rows_left = (uint8_t)(t1 - 1u);
					*p_a1 += 0x40u;
				} while (t1 != 1u);
				t2 = columns_left;
				columns_left = (uint8_t)(t2 - 1u);
			} while (t2 != 1u);
			floor_end_walled--;
		}
	}
}

/* rows of the floor map filled with one kind of cell */
void fill_floor_map_rows(uint8_t rows, uint32_t cells, uint32_t floor_place)
{
	uint8_t longs_left = 0;
	uint8_t t1 = 0, t2 = 0;

	do
	{
		longs_left = 4u;
		do
		{
			mem_write32(floor_place, cells);
			t1 = longs_left;
			longs_left = (uint8_t)(t1 - 1u);
			floor_place += 4u;
		} while (t1 != 1u);
		t2 = rows;
		rows = (uint8_t)(t2 - 1u);
		floor_place += 0x30u;
	} while (t2 != 1u);
}

/* the lift's striped edge kept at its place while it is near */
void place_lift_edge(void)
{
	uint32_t distance = 0, object = 0;
	uint32_t x = 0;

	object = OBJECT_ADDRESS(44);
	OBJECT(OBJECT_ADDRESS(44))->flags &= 0x7Fu;
	x = LAYER(LAYER_B)->x;
	distance = 0xCE000u - x;
	if (x > 0xCE000u || distance >= LIFT_EDGE_RANGE)
		return;
	OBJECT(object)->screen_x = (distance + 0x6000u) >> 8;
	OBJECT(object)->screen_y = (0x18000u - LAYER(LAYER_B)->y) >> 8;
	OBJECT(object)->flags |= 0x80u;
}

/* the highway's layer and the cars full of Foot soldiers */
void update_layers_highway(uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1)
{
	uint32_t ignored = 0;

	load_layer_b();
	working_layer_tiles = TILE_RAM + 0x4000u;
	draw_new_row_and_column(&ignored, p_d5, p_d7);
	save_layer_b();
	LAYER(LAYER_A)->y = LAYER(LAYER_B)->y;
	switch (layers_step)
	{
	case 0:
		highway_first_car(p_d5, p_d7, p_a1);
		break;
	case 1:
		highway_first_car_drive(p_d5, p_d7, p_a1);
		break;
	case 2:
		highway_second_car(p_d5, p_d7, p_a1);
		break;
	case 3:
		highway_second_car_drive(p_d5, p_d7, p_a1);
		break;
	case 4:
		highway_third_car(p_d5, p_d7, p_a1);
		break;
	case 5:
		highway_third_car_drive(p_d5, p_d7, p_a1);
		break;
	case 6:
		highway_cars_over();
		break;
	}
	set_layers_scroll();
}

/* the first car full of Foot soldiers comes */
void highway_first_car(uint32_t *p_colour_offset, uint32_t *p_sound, uint32_t *p_colours)
{
	uint8_t colour_offset = (uint8_t)*p_colour_offset;

	if (LAYER(LAYER_B)->x < 0x10000u)
		return;
	colour_offset = 0u;
	draw_screen_picture(colour_offset, p_sound, p_colours);
	start_foot_car();
	layers_step++;
	*p_colour_offset = (*p_colour_offset & 0xFFFFFF00u) | colour_offset;
}

/* the first car drives past */
void highway_first_car_drive(uint32_t *p_colour_offset, uint32_t *p_count, uint32_t *p_right_wheel_place)
{
	uint8_t colour_offset = 0;

	colour_offset = 0u;
	drive_foot_car(colour_offset, p_count, p_right_wheel_place);
	*p_colour_offset = (*p_colour_offset & 0xFFFFFF00u) | colour_offset;
}

/* the second car comes, in other colours */
void highway_second_car(uint32_t *p_d5, uint32_t *p_sound, uint32_t *p_colours)
{
	if (LAYER(LAYER_B)->x < 0x30000u)
		return;
	*p_d5 = 0x40u;
	draw_screen_picture((uint8_t)*p_d5, p_sound, p_colours);
	start_foot_car();
	layers_step++;
}

/* the second car drives past */
void highway_second_car_drive(uint32_t *p_colour_offset, uint32_t *p_count, uint32_t *p_right_wheel_place)
{
	*p_colour_offset = 0x40u;
	drive_foot_car((uint8_t)*p_colour_offset, p_count, p_right_wheel_place);
}

/* the third car comes, in other colours */
void highway_third_car(uint32_t *p_d5, uint32_t *p_sound, uint32_t *p_colours)
{
	if (LAYER(LAYER_B)->x < 0x50000u)
		return;
	*p_d5 = 0xFFFFFF80u;
	draw_screen_picture((uint8_t)*p_d5, p_sound, p_colours);
	start_foot_car();
	layers_step++;
}

/* the third car drives past */
void highway_third_car_drive(uint32_t *p_colour_offset, uint32_t *p_count, uint32_t *p_right_wheel_place)
{
	*p_colour_offset = 0xFFFFFF80u;
	drive_foot_car((uint8_t)*p_colour_offset, p_count, p_right_wheel_place);
}

/* no more cars */
void highway_cars_over(void)
{
}

/* the car's wheels turn and it drives across the screen */
void drive_foot_car(uint8_t colour_offset, uint32_t *p_count, uint32_t *p_right_wheel_place)
{
	uint32_t count = *p_count;
	uint32_t right_wheel_place = *p_right_wheel_place;
	uint8_t tile_attribute = 0;
	uint16_t count_2 = 0;
	uint32_t left_wheel_place = 0, tiles = 0, tiles_b = 0, x = 0;
	uint16_t t3 = 0, t4 = 0;
	uint32_t t1 = 0, t2 = 0, x_2 = 0;

	if ((frame_count & 3u) == 0u)
	{
		tile_attribute = 0x34u;
		tiles = foot_car_tiles_ADDRESS;
		tiles_b = foot_car_tiles_b_ADDRESS;
		if ((frame_count & 4u) != 0u)
		{
			tile_attribute++;
			tiles = foot_car_tiles_2_ADDRESS;
			tiles_b = foot_car_tiles_2b_ADDRESS;
		}
		left_wheel_place = TILE_RAM + 0x2A56u;
		right_wheel_place = TILE_RAM + 0x2A6Cu;
		count = 3u;
		tile_attribute += colour_offset;
		do
		{
			count_2 = 3u;
			do
			{
				io_write8(left_wheel_place, tile_attribute);
				io_write8(right_wheel_place, tile_attribute);
				t1 = tiles;
				tiles++;
				left_wheel_place++;
				io_write8(left_wheel_place, PICTURE_TILE(t1));
				t2 = tiles_b;
				tiles_b++;
				right_wheel_place++;
				io_write8(right_wheel_place, PICTURE_TILE(t2));
				t3 = count_2 - 1u;
				count_2 = t3;
				left_wheel_place++;
				right_wheel_place++;
			} while (t3 != 0xFFFFu);
			t4 = count - 1u;
			count = t4;
			left_wheel_place += 0x78u;
			right_wheel_place += 0x78u;
		} while (t4 != 0xFFFFu);
	}
	x = (uint16_t)(0x480u - LAYER(LAYER_B)->x_speed);
	x_2 = LAYER(LAYER_A)->x;
	LAYER(LAYER_A)->x = x_2 - x;
	if (x_2 < x)
	{
		LAYER(LAYER_A)->x = 0x1FFFFu;
		layers_step++;
	}
	*p_count = count;
	*p_right_wheel_place = right_wheel_place;
}

/* the picture of the Foot's car drawn into layer A in the colours given, with a sound */
void draw_screen_picture(uint8_t colour_offset, uint32_t *p_sound, uint32_t *p_colours)
{
	uint16_t count_2 = 0;
	uint32_t colours = 0, count = 0, tile_place = 0, tiles = 0;
	uint16_t t4 = 0, t5 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	tile_place = TILE_RAM + 0x25D0u;
	colours = screen_picture_colours_ADDRESS;
	tiles = screen_picture_tiles_ADDRESS;
	count = 0xCu;
	do
	{
		count_2 = 0x17u;
		do
		{
			t1 = tile_place;
			tile_place++;
			io_write8(t1, PICTURE_TILE(colours) + colour_offset);
			t2 = tiles;
			tiles++;
			t3 = tile_place;
			tile_place++;
			io_write8(t3, PICTURE_TILE(t2));
			t4 = count_2 - 1u;
			count_2 = t4;
			colours++;
		} while (t4 != 0xFFFFu);
		t5 = count - 1u;
		count = t5;
		tile_place += 0x50u;
	} while (t5 != 0xFFFFu);
	*p_sound = (count & ~0xFFu) | 0x15u;
	queue_sound((uint8_t)*p_sound);
	*p_colours = colours;
}

/* the skateboard ride: the far background and the road scroll by themselves */
void update_layers_skateboard(uint32_t *p_d5, uint32_t *p_blocks_left)
{
	uint32_t ignored = 0;

	load_layer_b();
	LAYER(WORKING_LAYER)->x += LAYER(WORKING_LAYER)->x_speed;
	LAYER(WORKING_LAYER)->x &= 0x1FF00u;
	save_layer_b();
	load_layer_a();
	switch (layers_step)
	{
	case 0:
		skateboard_road_first_loop();
		break;
	case 1:
		skateboard_road_to_second_loop();
		break;
	case 2:
		skateboard_road_second_loop();
		break;
	case 3:
		skateboard_road_to_third_loop();
		break;
	case 4:
		skateboard_road_third_loop();
		break;
	case 5:
		skateboard_road_last_loop();
		break;
	}
	working_layer_tiles = TILE_RAM + 0x2000u;
	draw_new_row_and_column(&ignored, p_d5, p_blocks_left);
	save_layer_a();
	set_layers_scroll();
	OBJECT(OBJECT_ADDRESS(0))->contacts |= 0x80u;
	OBJECT(OBJECT_ADDRESS(1))->contacts |= 0x80u;
	OBJECT(OBJECT_ADDRESS(2))->contacts |= 0x80u;
	OBJECT(OBJECT_ADDRESS(3))->contacts |= 0x80u;
}

/* the road goes round its first stretch twenty times, then on */
void skateboard_road_first_loop(void)
{
	LAYER(WORKING_LAYER)->column_phase = 0u;
	if ((uint8_t)layer_loops == 0x14u)
	{
		if (LAYER(WORKING_LAYER)->x < 0xA200u)
			return;
		skateboard_road_go_on();
		return;
	}
	if ((LAYER(WORKING_LAYER)->x_page & 2u) == 0u)
		return;
	LAYER(WORKING_LAYER)->x &= 0x1FF00u;
	layer_loops++;
}

/* the road goes on to new stretches */
void skateboard_road_go_on(void)
{
	LAYER(WORKING_LAYER)->column_phase = (LAYER(WORKING_LAYER)->x & 0x700u) << 5;
	layer_loops = 0;
	layers_step++;
}

/* the road on to its second stretch */
void skateboard_road_to_second_loop(void)
{
	if (LAYER(WORKING_LAYER)->x >= 0x3A200u)
	{
		layers_step++;
		return;
	}
}

/* the road goes round its second stretch twenty times, then on */
void skateboard_road_second_loop(void)
{
	LAYER(WORKING_LAYER)->column_phase = 0u;
	if ((uint8_t)layer_loops == 0x14u)
	{
		if (LAYER(WORKING_LAYER)->x >= 0x3A200u)
		{
			skateboard_road_go_on();
			return;
		}
		return;
	}
	if (LAYER(WORKING_LAYER)->x < 0x50000u)
		return;
	LAYER(WORKING_LAYER)->x = 0x30000u;
	layer_loops++;
}

/* the road on to its third stretch */
void skateboard_road_to_third_loop(void)
{
	if (LAYER(WORKING_LAYER)->x < 0x6A200u)
		return;
	LAYER(WORKING_LAYER)->column_phase = 0u;
	layers_step++;
}

/* the road goes round its third stretch twenty times, then on */
void skateboard_road_third_loop(void)
{
	LAYER(WORKING_LAYER)->column_phase = 0u;
	if ((uint8_t)layer_loops == 0x14u)
	{
		if (LAYER(WORKING_LAYER)->x >= 0x6A200u)
		{
			skateboard_road_go_on();
			return;
		}
		skateboard_road_last_loop();
		return;
	}
	if (LAYER(WORKING_LAYER)->x < 0x80000u)
		return;
	LAYER(WORKING_LAYER)->x = 0x60000u;
	layer_loops++;
}

/* the road goes round its last stretch from then on */
void skateboard_road_last_loop(void)
{
	LAYER(WORKING_LAYER)->column_phase = 0u;
	if (LAYER(WORKING_LAYER)->x < 0xA0000u)
		return;
	LAYER(WORKING_LAYER)->x = 0x80000u;
}
