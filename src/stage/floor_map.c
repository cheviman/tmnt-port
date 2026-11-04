/**
 *    Name:        stage/floor_map.c
 *    Description: the floor map's cells around an object, for the turtles, the enemies and the missiles
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the floor map's cells around the object, by its place on the screen */
uint32_t read_floor_cells_on_screen(uint32_t object)
{
	uint8_t feet_y = 0;
	uint16_t floor_column = 0;

	floor_column = ((OBJECT(object)->screen_x - 0x60u + ((LAYER(WORKING_LAYER)->x >> 8) & 0x1FFu)) & 0x1FFu) >> 3;
	feet_y = (uint8_t)OBJECT(object)->screen_y;
	return read_floor_cells_at(feet_y, floor_column, object);
}

/* the floor map's cells around the object, by its place in the area */
uint32_t read_floor_cells(uint32_t object)
{
	uint8_t feet_y = 0;
	uint32_t floor_column = 0;

	floor_column = OBJECT(object)->x >> 8;
	offset_high_word = floor_column;
	floor_column = ((floor_column - 0x60u + ((LAYER(WORKING_LAYER)->x >> 8) & 0x1FFu)) & 0x1FFu) >> 3;
	feet_y = (uint8_t)(OBJECT(object)->y >> 8u);
	return read_floor_cells_at(feet_y, (uint16_t)floor_column, object);
}

/* the floor map's cells at the object's feet and at its height, with the ones around them */
uint32_t read_floor_cells_at(uint8_t feet_y, uint16_t floor_column, uint32_t object)
{
	uint16_t height_cell_offset = 0;
	uint32_t floor_cell_place = 0, height_cell_place = 0, neighbour_place = 0;

	feet_y += OBJECT(object)->floor_height;
	height_cell_offset = (uint16_t)((((uint8_t)(0u - (feet_y + (OBJECT(object)->height_whole - OBJECT(object)->floor_height)) - LAYER(WORKING_LAYER)->y_pixel) & 0xF8u) << 3) + floor_column);
	floor_cell_place = floor_map_ADDRESS + (uint16_t)((((uint8_t)(0u - feet_y - LAYER(WORKING_LAYER)->y_pixel) & 0xF8u) << 3) + floor_column);
	height_cell_place = floor_map_ADDRESS + height_cell_offset;
	floor_cell = FLOOR_MAP_CELL(floor_cell_place)->cell;
	height_cell = FLOOR_MAP_CELL(height_cell_place)->cell;
	if (floor_cell_place < floor_map_ADDRESS + 0x40u)
		floor_cell_up = FLOOR_MAP_CELL(floor_cell_place + 0x7C0u)->cell;
	else
		floor_cell_up = FLOOR_MAP_CELL(floor_cell_place - 0x40u)->cell;
	if ((floor_cell_place & 0x3Fu) == 0x3Fu)
		floor_cell_right = FLOOR_MAP_CELL(floor_cell_place - 0x3Fu)->cell;
	else
		floor_cell_right = FLOOR_MAP_CELL(floor_cell_place + 0x1u)->cell;
	if ((floor_cell_place & 0x3Fu) == 0u)
		floor_cell_left = FLOOR_MAP_CELL(floor_cell_place + 0x3Fu)->cell;
	else
		floor_cell_left = FLOOR_MAP_CELL(floor_cell_place - 0x1u)->cell;
	neighbour_place = floor_cell_place;
	if (floor_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((floor_cell_place & 0x3Fu) == 0x3Fu)
		neighbour_place -= 0x40u;
	floor_cell_up_right = FLOOR_MAP_CELL(neighbour_place - 0x3Fu)->cell;
	neighbour_place = floor_cell_place;
	if (floor_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((floor_cell_place & 0x3Fu) == 0u)
		neighbour_place += 0x40u;
	floor_cell_up_left = FLOOR_MAP_CELL(neighbour_place - 0x41u)->cell;
	neighbour_place = floor_cell_place;
	if (floor_cell_place >= floor_map_ADDRESS + 0x7C0u)
		neighbour_place -= 0x800u;
	floor_cell_down = FLOOR_MAP_CELL(neighbour_place + 0x40u)->cell;
	if (height_cell_place < floor_map_ADDRESS + 0x40u)
		height_cell_up = FLOOR_MAP_CELL(height_cell_place + 0x7C0u)->cell;
	else
		height_cell_up = FLOOR_MAP_CELL(height_cell_place - 0x40u)->cell;
	if ((height_cell_place & 0x3Fu) == 0x3Fu)
		height_cell_right = FLOOR_MAP_CELL(height_cell_place - 0x3Fu)->cell;
	else
		height_cell_right = FLOOR_MAP_CELL(height_cell_place + 0x1u)->cell;
	if ((height_cell_place & 0x3Fu) == 0u)
		height_cell_left = FLOOR_MAP_CELL(height_cell_place + 0x3Fu)->cell;
	else
		height_cell_left = FLOOR_MAP_CELL(height_cell_place - 0x1u)->cell;
	neighbour_place = height_cell_place;
	if (height_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((height_cell_place & 0x3Fu) == 0x3Fu)
		neighbour_place -= 0x40u;
	height_cell_up_right = FLOOR_MAP_CELL(neighbour_place - 0x3Fu)->cell;
	neighbour_place = height_cell_place;
	if (height_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((height_cell_place & 0x3Fu) == 0u)
		neighbour_place += 0x40u;
	height_cell_up_left = FLOOR_MAP_CELL(neighbour_place - 0x41u)->cell;
	if ((int8_t)(uint8_t)(OBJECT(object)->y >> 24u) < 0)
		floor_cell = 0u;
	distance_low_byte = height_cell_offset;
	object_loop_leftover = height_cell_place;
	return floor_cell_place;
}
