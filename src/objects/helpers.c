/**
 *    Name:        objects/helpers.c
 *    Description: small routines the objects of these files share: their stats, moves, free objects, the bosses' targets
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* picks one of the turtles in play at random as the boss's target and aims at it; none, none picked */
void boss_pick_target(uint32_t object, uint32_t *p_direction_offset, uint32_t *p_picked, uint32_t *p_choices)
{
	uint16_t count = 0;
	uint32_t choice_count = 0, object_2 = 0;
	uint16_t t1 = 0;

	object_2 = OBJECT_ADDRESS(3);
	*p_choices = boss_target_choices_ADDRESS;
	boss_target_choices(0) = 0u;
	boss_target_choices(1) = 0u;
	boss_target_choices(2) = 0u;
	boss_target_choices(3) = 0u;
	count = 3u;
	choice_count = 0u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_knocked_out & shl32(1u, (int)(count & 7u))) == 0u && OBJECT(object_2)->routine != 4u)
		{
			boss_target_choices(choice_count) = (uint8_t)count;
			choice_count = (uint8_t)(choice_count + 1u);
		}
		t1 = count - 1u;
		count = t1;
		object_2 -= 0x50u;
	} while (t1 != 0xFFFFu);
	if (choice_count != 0u)
	{
		choice_count = random_below((uint16_t)choice_count, object);
		choice_count = ((choice_count & ~0xFFu) | boss_target_choices(choice_count)) * 0x50u;
		OBJECT(object)->boss.target = OBJECT_ADDRESS(0) + choice_count;
		boss_aim_at_target(object, p_direction_offset, &choice_count, p_choices);
		choice_count = IN_PAGE(choice_count, 1u);
	}
	*p_picked = choice_count;
}

/* sets the boss's speeds toward its target and turns it that way */
void boss_aim_at_target(uint32_t object, uint32_t *p_direction_offset, uint32_t *p_y_speed, uint32_t *p_target)
{
	uint16_t y = 0;
	uint32_t direction = 0, speeds = 0;

	boss_direction_to_target(object, &direction, p_target);
	speeds = rock_soldier_site_speeds_ADDRESS;
	if (OBJECT(object)->kind != 0x1Eu)
		speeds = boss_speeds_ADDRESS;
	direction = (uint16_t)(direction << 2);
	OBJECT(object)->x_speed = OFFSET_PAIR(speeds + ((int32_t)(direction / 4u)) * 0x4u)->x;
	y = (uint16_t)OFFSET_PAIR(speeds + ((int32_t)(direction / 4u)) * 0x4u)->y;
	OBJECT(object)->y_speed = y;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->flags &= 0xBFu;
	if (OBJECT(object)->x_speed >= 0)
		OBJECT(object)->flags |= 0x40u;
	*p_direction_offset = direction;
	*p_y_speed = (*p_y_speed & 0xFFFF0000u) | y;
}

/* an object's attack, health, depth and the sound a turtle it hits makes, from its record */
void set_object_stats(uint32_t object, uint32_t stats)
{
	OBJECT(object)->attack = OBJECT_STATS(stats)->attack;
	OBJECT(object)->hit = OBJECT_STATS(stats)->hit;
	OBJECT(object)->health_bars = OBJECT_STATS(stats)->health_bars;
	OBJECT(object)->health = OBJECT_STATS(stats)->health;
	OBJECT(object)->damage = OBJECT_STATS(stats)->damage;
	OBJECT(object)->depth = OBJECT_STATS(stats)->depth;
	OBJECT(object)->set_object_stats.hit_sound = OBJECT_STATS(stats)->hit_sound;
}

/* moves the object by its speeds across, up or down the screen, and in height */
uint32_t move_by_speeds(uint32_t object)
{
	uint32_t height_speed = 0, y_speed = 0;

	OBJECT(object)->x += OBJECT(object)->x_speed;
	y_speed = OBJECT(object)->y_speed;
	OBJECT(object)->y += y_speed;
	height_speed = (y_speed & ~0xFFFFu) | (uint16_t)OBJECT(object)->height_speed;
	OBJECT(object)->height += height_speed;
	return height_speed;
}

/* finds an object not in use among the next ones */
void find_free_object(uint32_t *p_objects_left, uint32_t *p_object)
{
	uint8_t objects_left = (uint8_t)*p_objects_left;
	uint8_t t1 = 0;

	do
	{
		if (OBJECT(*p_object)->flags == 0u)
			break;
		t1 = objects_left;
		objects_left = (uint8_t)(t1 - 1u);
		*p_object += 0x50u;
	} while (t1 != 1u);
	*p_objects_left = (*p_objects_left & 0xFFFFFF00u) | objects_left;
}

/* the object is gone once it is off the screen */
void remove_if_off_screen(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		remove_object(object);
		return;
	}
}

/* the object is gone: out of use */
void remove_object(uint32_t object)
{
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
	OBJECT(object)->kind = 0u;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->routine = 0u;
	OBJECT(object)->state = 0u;
}

/* the direction, one of sixteen, from the boss to its target */
void boss_direction_to_target(uint32_t object, uint32_t *p_direction, uint32_t *p_target)
{
	uint32_t overflow = 0;
	uint16_t quadrant = 0, slope_offset = 0, y_distance = 0;
	uint32_t direction = 0, target = 0, x_distance = 0;
	uint16_t t1 = 0, t2 = 0;

	target = OBJECT(object)->boss.target;
	quadrant = 0u;
	t1 = mem_read16(target + 0xCu) - OBJECT(object)->screen_x;
	x_distance = t1;
	if ((int16_t)t1 < 0)
	{
		quadrant = (uint8_t)(quadrant + 1u);
		x_distance = (uint16_t)~x_distance;
		x_distance = (x_distance & ~0xFFu) | (uint8_t)(x_distance + 1u);
	}
	t2 = mem_read16(target + 0xEu) - OBJECT(object)->screen_y;
	y_distance = t2;
	if ((int16_t)t2 < 0)
	{
		quadrant = (uint8_t)(quadrant + 2u);
		y_distance = (uint16_t)~y_distance;
		y_distance = (uint16_t)((y_distance & ~0xFFu) | (uint8_t)(y_distance + 1u));
	}
	if (x_distance == 0u)
		direction = boss_axis_directions[quadrant];
	else if (y_distance == 0u)
		direction = boss_axis_directions[3 + quadrant];
	else
	{
		slope_offset = 0u;
		x_distance = x_distance * 0x3E8u;
		overflow = m68k_divu_ov(x_distance, y_distance);
		x_distance = y_distance == 0u ? x_distance : m68k_divu(x_distance, y_distance);
		if (y_distance == 0u)
			exception_hang();
		if (overflow == 0u)
		{
			while ((uint16_t)x_distance < TABLE16(boss_axis_directions, 0x6u + (uint32_t)(int16_t)slope_offset))
			{
				slope_offset += 2u;
			}
		}
		direction = boss_slope_directions[quadrant * 5u + (slope_offset >> 1)];
	}
	*p_direction = direction;
	*p_target = target;
}

/* puts the other object where this one is */
void place_at_object(uint32_t object, uint32_t object_2)
{
	OBJECT(object_2)->x = OBJECT(object)->x;
	OBJECT(object_2)->y = OBJECT(object)->y;
	OBJECT(object_2)->height = OBJECT(object)->height;
}
