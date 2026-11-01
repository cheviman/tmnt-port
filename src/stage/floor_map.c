/**
 *    Name:        stage/floor_map.c
 *    Description: the floor map's cells around an object, for the turtles, the enemies and the missiles
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the floor map's cells around the object, by its place on the screen */
void read_floor_cells_on_screen(uint32_t object, uint32_t *p_d5, uint32_t *p_a5, uint32_t *p_a6)
{
	uint8_t feet_y = 0;
	uint16_t floor_column = 0;
	uint32_t d5 = 0;

	floor_column = ((OBJECT(object)->screen_x - 0x60u + ((LAYER(WORKING_LAYER)->x >> 8) & 0x1FFu)) & 0x1FFu) >> 3;
	feet_y = (uint8_t)OBJECT(object)->screen_y;
	read_floor_cells_at(feet_y, floor_column, object, &d5, p_a5, p_a6);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* the floor map's cells around the object, by its place in the area */
void read_floor_cells(uint32_t object, uint32_t *p_d5, uint32_t *p_floor_column, uint32_t *p_a6)
{
	uint8_t feet_y = 0;
	uint32_t d5 = 0, floor_column = 0, ignored = 0;

	floor_column = OBJECT(object)->x >> 8;
	floor_column = (floor_column & ~0xFFFFu) | (uint16_t)(floor_column - 0x60u);
	floor_column = (floor_column & ~0xFFFFu) | (uint16_t)(floor_column + ((LAYER(WORKING_LAYER)->x >> 8) & 0x1FFu));
	floor_column = floor_column & ~0xFE00u;
	floor_column = (floor_column & ~0xFFFFu) | ((uint16_t)floor_column >> 3);
	feet_y = (uint8_t)(OBJECT(object)->y >> 8u);
	read_floor_cells_at(feet_y, (uint16_t)floor_column, object, &d5, &ignored, p_a6);
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
	*p_floor_column = floor_column;
}

/* the floor map's cells at the object's feet and at its height, with the ones around them */
void read_floor_cells_at(uint8_t feet_y, uint16_t floor_column, uint32_t object, uint32_t *p_height_cell_offset, uint32_t *p_height_cell_place, uint32_t *p_floor_cell_place)
{
	uint16_t height_cell_offset = 0;
	uint32_t floor_cell_place = 0, height_cell_place = 0, neighbour_place = 0;

	feet_y += OBJECT(object)->floor_height;
	height_cell_offset = (uint16_t)((((uint8_t)(0u - (feet_y + (OBJECT(object)->height_whole - OBJECT(object)->floor_height)) - LAYER(WORKING_LAYER)->y_pixel) & 0xF8u) << 3) + floor_column);
	floor_cell_place = floor_map_ADDRESS + (uint16_t)((((uint8_t)(0u - feet_y - LAYER(WORKING_LAYER)->y_pixel) & 0xF8u) << 3) + floor_column);
	height_cell_place = floor_map_ADDRESS + height_cell_offset;
	floor_cell = mem_read8(floor_cell_place);
	height_cell = mem_read8(height_cell_place);
	if (floor_cell_place < floor_map_ADDRESS + 0x40u)
		floor_cell_up = mem_read8(floor_cell_place + 0x7C0u);
	else
		floor_cell_up = mem_read8(floor_cell_place - 0x40u);
	if ((floor_cell_place & 0x3Fu) == 0x3Fu)
		floor_cell_right = mem_read8(floor_cell_place - 0x3Fu);
	else
		floor_cell_right = mem_read8(floor_cell_place + 1u);
	if ((floor_cell_place & 0x3Fu) == 0u)
		floor_cell_left = mem_read8(floor_cell_place + 0x3Fu);
	else
		floor_cell_left = mem_read8(floor_cell_place - 1u);
	neighbour_place = floor_cell_place;
	if (floor_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((floor_cell_place & 0x3Fu) == 0x3Fu)
		neighbour_place -= 0x40u;
	floor_cell_up_right = mem_read8(neighbour_place - 0x3Fu);
	neighbour_place = floor_cell_place;
	if (floor_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((floor_cell_place & 0x3Fu) == 0u)
		neighbour_place += 0x40u;
	floor_cell_up_left = mem_read8(neighbour_place - 0x41u);
	neighbour_place = floor_cell_place;
	if (floor_cell_place >= floor_map_ADDRESS + 0x7C0u)
		neighbour_place -= 0x800u;
	floor_cell_down = mem_read8(neighbour_place + 0x40u);
	if (height_cell_place < floor_map_ADDRESS + 0x40u)
		height_cell_up = mem_read8(height_cell_place + 0x7C0u);
	else
		height_cell_up = mem_read8(height_cell_place - 0x40u);
	if ((height_cell_place & 0x3Fu) == 0x3Fu)
		height_cell_right = mem_read8(height_cell_place - 0x3Fu);
	else
		height_cell_right = mem_read8(height_cell_place + 1u);
	if ((height_cell_place & 0x3Fu) == 0u)
		height_cell_left = mem_read8(height_cell_place + 0x3Fu);
	else
		height_cell_left = mem_read8(height_cell_place - 1u);
	neighbour_place = height_cell_place;
	if (height_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((height_cell_place & 0x3Fu) == 0x3Fu)
		neighbour_place -= 0x40u;
	height_cell_up_right = mem_read8(neighbour_place - 0x3Fu);
	neighbour_place = height_cell_place;
	if (height_cell_place < floor_map_ADDRESS + 0x40u)
		neighbour_place += 0x800u;
	if ((height_cell_place & 0x3Fu) == 0u)
		neighbour_place += 0x40u;
	height_cell_up_left = mem_read8(neighbour_place - 0x41u);
	if ((int8_t)(uint8_t)(OBJECT(object)->y >> 24u) < 0)
		floor_cell = 0u;
	*p_height_cell_offset = (*p_height_cell_offset & 0xFFFF0000u) | height_cell_offset;
	*p_height_cell_place = height_cell_place;
	*p_floor_cell_place = floor_cell_place;
}
