/**
 *    Name:        stage/area_clear.c
 *    Description: the checks for the end of each area: the scroll locked, every player ready and no enemy left, then the area cleared
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the stage's check for the end of the area, from its last scroll segment on */
void check_area_cleared(void)
{
	switch (stage)
	{
	case 0:
		check_cleared_building();
		return;
	case 1:
		check_cleared_apartment();
		return;
	case 2:
		check_cleared_street();
		return;
	case 3:
		check_cleared_sewer();
		return;
	case 4:
		check_cleared_car_street();
		return;
	case 5:
		check_cleared_highway();
		return;
	case 6:
		check_cleared_skateboard();
		return;
	case 7:
		check_cleared_building_site();
		return;
	case 8:
		check_cleared_technodrome();
		return;
	case 9:
		check_cleared_final();
		return;
	}
}

/* area 0: the scroll held; every player ready and no enemy left, 96 frames with sounds, then the area
   beaten */
void check_cleared_building(void)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint8_t t1 = 0;

	if (last_segment == 0u)
	{
		scroll_held = 0xFFFFFFFFu;
		last_segment = 0xFFu;
		building_clear_wait = 0u;
		return;
	}
	if (building_clear_wait == 0u)
	{
		zero = players_ready();
		if (zero || enemies_on_screen != 0u)
			return;
		building_clear_wait = 0x60u;
		return;
	}
	t1 = building_clear_wait;
	building_clear_wait = t1 - 1u;
	if (t1 != 1u)
	{
		if (building_clear_wait != 0x40u)
			return;
		sound = 0xBAu;
		queue_sound(sound);
		sound = 0xFEu;
		queue_sound(sound);
		return;
	}
	area_beaten = 0xFFu;
}

/* area 1: the scroll held; beaten with every player ready, no enemy left and the drill's arrival over */
void check_cleared_apartment(void)
{
	uint32_t zero = 0;

	scroll_held = 0xFFFFFFFFu;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u && drill_arrival < 0)
		area_beaten = 0xFFu;
}

/* area 2: the scroll held; beaten with every player ready and no enemy left */
void check_cleared_street(void)
{
	uint32_t zero = 0;

	scroll_held = 0xFFFFFFFFu;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 3: the scroll held one way; every 256 frames the enemies beyond row 0x88 taken off; beaten when
   all are gone */
void check_cleared_sewer(void)
{
	uint32_t zero = 0;
	uint8_t limit_y = 0;
	uint32_t object = 0;

	if ((uint8_t)frame_count != 0u)
	{
		scroll_held = (scroll_held & 0xFFFFu) | 0xFFFF0000u;
		last_segment = 0xFFu;
		zero = players_ready();
		if (zero == 0u && enemies_on_screen == 0u)
			area_beaten = 0xFFu;
		return;
	}
	object = OBJECT_ADDRESS(12);
	limit_y = 0x88u;
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u && (OBJECT(object)->flags & 0x80u) != 0u && (int8_t)OBJECT(object)->throw < 0 && (int8_t)(uint8_t)(OBJECT(object)->y >> 24u) >= 0 && limit_y < (uint8_t)(OBJECT(object)->y >> 8u))
		{
			OBJECT(object)->flags = 0u;
			enemies_on_screen--;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(24));
	scroll_held = (scroll_held & 0xFFFFu) | 0xFFFF0000u;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 4: the scroll held; beaten with every player ready and no enemy left */
void check_cleared_car_street(void)
{
	uint32_t zero = 0;

	scroll_held = 0xFFFFFFFFu;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 5: the scroll held one way; beaten with every player ready and no enemy left */
void check_cleared_highway(void)
{
	uint32_t zero = 0;

	scroll_held = (scroll_held & 0xFFFFu) | 0xFFFF0000u;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 6: the scroll goes on by itself; beaten with every player ready and no enemy left */
void check_cleared_skateboard(void)
{
	uint32_t zero = 0;

	scroll_mode = -1;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 7: the scroll held; beaten with every player ready and no enemy left */
void check_cleared_building_site(void)
{
	uint32_t zero = 0;

	scroll_held = 0xFFFFFFFFu;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 8: the scroll held; beaten with every player ready and no enemy left */
void check_cleared_technodrome(void)
{
	uint32_t zero = 0;

	scroll_held = 0xFFFFFFFFu;
	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* area 9: beaten with every player ready and no enemy left */
void check_cleared_final(void)
{
	uint32_t zero = 0;

	last_segment = 0xFFu;
	zero = players_ready();
	if (zero == 0u && enemies_on_screen == 0u)
		area_beaten = 0xFFu;
}

/* whether every player in play has a turtle that is not being hit */
uint32_t players_ready(void)
{
	uint32_t zero = 0, carry = 0;
	uint8_t players = 0;
	uint32_t object = 0;
	uint8_t flags = 0, routine = 0;

	object = OBJECT_ADDRESS(0);
	players = players_in_play;
	do
	{
		carry = players & 1u;
		players = players >> 1;
		if (carry)
		{
			flags = OBJECT(object)->flags;
			zero = (flags & 1u) == 0u;
			if ((flags & 1u) == 0u)
				return zero;
			routine = OBJECT(object)->routine;
			zero = (uint8_t)(routine - 4u) == 0u;
			if (routine == 4u)
				return zero;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	zero = 0xFFu == 0u;
	return zero;
}
