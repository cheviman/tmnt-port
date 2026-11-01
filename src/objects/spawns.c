/**
 *    Name:        objects/spawns.c
 *    Description: the enemies' spawn lists, by the scroll
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the area's spawn list, the speeds and directions of its spawn scroll and where that scroll starts */
void start_area_spawns(void)
{
	uint16_t area_offset = 0;
	uint32_t speeds = 0;

	area_offset = (uint16_t)((uint32_t)(int8_t)area << 2);
	spawn_next = TABLE32(area_spawn_lists, (uint32_t)(int16_t)area_offset);
	area_offset = (uint16_t)((uint32_t)area_offset << 1);
	speeds = TABLE32(area_spawn_scrolls, (uint32_t)(int16_t)area_offset);
	mem_write16(speeds + 2u, 0u);
	mem_write16(speeds, 0u);
	spawn_speeds = speeds;
	spawn_directions = TABLE32(area_spawn_scrolls, 0x4u + (uint32_t)(int16_t)area_offset);
	spawn_x = TABLE32(area_spawn_starts, (uint32_t)(int16_t)area_offset);
	spawn_y = TABLE32(area_spawn_starts, 0x4u + (uint32_t)(int16_t)area_offset);
}

/* every frame: the spawn scroll moved on and, when it reaches the next record, its enemy put into a free
   object */
uint32_t spawn_enemies(uint32_t offset)
{
	uint32_t zero = 0, carry = 0;
	uint16_t x_step = 0, y_step = 0;
	uint32_t directions = 0, enemy_x = 0, object = 0, spawn_record = 0, speeds = 0;
	uint32_t kind_halfword = 0, kind_halfword_2 = 0;

	speeds = spawn_speeds;
	directions = spawn_directions;
	x_step = SPAWN_SPEED(speeds)->x;
	if ((int8_t)SPAWN_DIRECTION(directions)->x >= 0)
		x_step = (uint16_t)(0u - x_step);
	spawn_x += (int16_t)x_step;
	y_step = SPAWN_SPEED(speeds)->y;
	if ((int8_t)SPAWN_DIRECTION(directions)->y < 0)
		y_step = (uint16_t)(0u - y_step);
	spawn_y += (int16_t)y_step;
	spawn_record = spawn_next;
	if (SPAWN_RECORD(spawn_record)->x == 0xFFFFu || (int16_t)(spawn_x >> 8) < (int16_t)SPAWN_RECORD(spawn_record)->x)
		return offset;
	if (area == 6u)
	{
		carry = hold_scroll_for_enemies();
		if (carry)
		{
			spawn_next = spawn_record + 8u;
			return offset;
		}
		if (difficulty < (SPAWN_RECORD(spawn_record)->flags & 0xFu))
			spawn_next = spawn_record + 8u;
		else
		{
			offset = spawn_offset(spawn_record);
			take_free_object(spawn_record, &object, &zero);
			if (zero)
			{
				OBJECT(object)->flags = 1u;
				OBJECT(object)->contacts = 0u;
				kind_halfword = SPAWN_RECORD(spawn_record)->kind_and_parameter;
				OBJECT(object)->kind = (uint8_t)(kind_halfword >> 8u);
				OBJECT(object)->parameter = (uint8_t)kind_halfword;
				OBJECT(object)->routine = 0u;
				OBJECT(object)->state = 0u;
				OBJECT(object)->hit_routine = 0u;
				OBJECT(object)->colour = 0xFFu;
				OBJECT(object)->animation_delay = 0u;
				OBJECT(object)->throw = 0u;
				OBJECT(object)->height_whole = SPAWN_RECORD(spawn_record)->height;
				OBJECT(object)->height &= 0xFF00u;
				enemy_x = SPAWN_RECORD(spawn_record)->x;
				if ((SPAWN_RECORD(spawn_record)->flags & 0x40u) != 0u)
				{
					enemy_x = (uint16_t)(enemy_x - 0x140u);
					enemy_x = (uint16_t)(enemy_x - SCREEN_MARGIN(offset)->x);
				}
				else
					enemy_x = (uint16_t)(enemy_x + SCREEN_MARGIN(offset)->x);
				OBJECT(object)->x = enemy_x << 8;
				OBJECT(object)->y = (uint32_t)SPAWN_RECORD(spawn_record)->y << 8;
				spawn_next = spawn_record + 8u;
				return offset;
			}
		}
		return offset;
	}
	if (difficulty < (SPAWN_RECORD(spawn_record)->flags & 0xFu))
		spawn_next = spawn_record + 8u;
	else
	{
		offset = spawn_offset(spawn_record);
		take_free_object(spawn_record, &object, &zero);
		if (zero)
		{
			OBJECT(object)->flags = 1u;
			OBJECT(object)->contacts = 0u;
			kind_halfword_2 = SPAWN_RECORD(spawn_record)->kind_and_parameter;
			OBJECT(object)->kind = (uint8_t)(kind_halfword_2 >> 8u);
			OBJECT(object)->parameter = (uint8_t)kind_halfword_2;
			OBJECT(object)->routine = 0u;
			OBJECT(object)->state = 0u;
			OBJECT(object)->hit_routine = 0u;
			OBJECT(object)->colour = 0xFFu;
			OBJECT(object)->animation_delay = 0u;
			OBJECT(object)->throw = 0u;
			OBJECT(object)->height_whole = SPAWN_RECORD(spawn_record)->height;
			OBJECT(object)->height &= 0xFF00u;
			enemy_x = SPAWN_RECORD(spawn_record)->x;
			if ((SPAWN_RECORD(spawn_record)->flags & 0x40u) != 0u)
			{
				enemy_x = (uint16_t)(enemy_x - 0x140u);
				enemy_x = (uint16_t)(enemy_x - SCREEN_MARGIN(offset)->x);
			}
			else
				enemy_x = (uint16_t)(enemy_x + SCREEN_MARGIN(offset)->x);
			OBJECT(object)->x = enemy_x << 8;
			OBJECT(object)->y = (uint32_t)SPAWN_RECORD(spawn_record)->y << 8;
			spawn_next = spawn_record + 8u;
			return offset;
		}
	}
	return offset;
}

/* Unused: the next record's enemy put into a free object when the spawn scroll comes level with its y */
void unreached_047B1C(uint32_t spawn)
{
	uint32_t zero = 0;
	uint16_t scroll_y = 0;
	uint32_t enemy_y = 0, margin = 0, object = 0;
	uint32_t kind_halfword = 0;

	scroll_y = (uint16_t)((spawn_y >> 8) + 8u);
	if ((int16_t)SPAWN_RECORD(spawn)->y <= (int16_t)scroll_y && (int16_t)(scroll_y - 0x10u) <= (int16_t)SPAWN_RECORD(spawn)->y)
	{
		margin = spawn_offset(spawn);
		take_free_object(spawn, &object, &zero);
		if (zero == 0u)
			return;
		OBJECT(object)->flags = 1u;
		OBJECT(object)->contacts = 0u;
		kind_halfword = SPAWN_RECORD(spawn)->kind_and_parameter;
		OBJECT(object)->kind = (uint8_t)(kind_halfword >> 8u);
		OBJECT(object)->parameter = (uint8_t)kind_halfword;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->hit_routine = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->throw = 0u;
		OBJECT(object)->height_whole = SPAWN_RECORD(spawn)->height;
		OBJECT(object)->height &= 0xFF00u;
		OBJECT(object)->x = (uint32_t)SPAWN_RECORD(spawn)->x << 8;
		enemy_y = SPAWN_RECORD(spawn)->y;
		if ((SPAWN_RECORD(spawn)->flags & 0x40u) != 0u)
		{
			enemy_y = (uint16_t)(enemy_y - 0x100u);
			enemy_y = (uint16_t)(enemy_y - SCREEN_MARGIN(margin)->y);
		}
		OBJECT(object)->y = enemy_y << 8;
		spawn_next = spawn + 8u;
	}
}

/* the screen margin a spawn record's kind of enemy comes in from */
uint32_t spawn_offset(uint32_t spawn_record)
{
	uint32_t margin = 0;

	margin = spawn_offsets_ADDRESS + (uint16_t)(((uint8_t)(SPAWN_RECORD(spawn_record)->kind_and_parameter >> 8u) & 0x7Fu) << 2);
	return margin;
}

/* the first free object of the group a spawn record picks, and whether there is one */
void take_free_object(uint32_t spawn_record, uint32_t *p_object, uint32_t *p_zero)
{
	uint32_t zero = *p_zero;
	uint16_t count = 0, group_offset = 0;
	uint32_t object = 0;
	uint8_t flags = 0;
	uint16_t t1 = 0;

	group_offset = (SPAWN_RECORD(spawn_record)->flags & 0x30u) >> 2;
	object = TABLE32(object_groups, group_offset);
	group_offset = group_offset >> 2;
	count = object_group_counts[group_offset];
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
