/**
 *    Name:        engine/directions.c
 *    Description: the direction from one object to another
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the direction from the object to a place on the screen, from its quadrant and the angle table */
uint32_t direction_to(uint32_t direction, uint16_t screen_x, uint16_t target_y, uint32_t object)
{
	uint32_t carry = 0, extend = 0;
	uint8_t shift = 0;
	uint32_t x_distance = 0, y_distance = 0;
	uint16_t t1 = 0, screen_y = 0, t2 = 0;
	uint32_t t3 = 0;

	direction &= ~0xFFFFu;
	t1 = screen_x - OBJECT(object)->screen_x;
	x_distance = t1;
	if ((int16_t)t1 < 0)
	{
		direction = 0x40u;
		x_distance = (uint16_t)(0u - x_distance);
	}
	screen_y = OBJECT(object)->screen_y;
	t2 = target_y - screen_y;
	carry = target_y < screen_y;
	extend = carry;
	y_distance = t2;
	if ((int16_t)t2 >= 0)
		direction = direction ^ 0xC0u;
	else
	{
		carry = y_distance > 0u;
		extend = carry;
		y_distance = (uint16_t)(0u - y_distance);
	}
	shift = 0u;
	if (y_distance <= x_distance)
	{
		while (x_distance >= 0x20u)
		{
			x_distance = x_distance >> 1;
			carry = (uint8_t)(shift + 1u) < shift;
			extend = carry;
			shift++;
		}
		y_distance = m68k_shift(M68K_LSR, y_distance, shift & 0x3Fu, extend, 16);
		if ((direction & 0x40u) != 0u)
		{
			t3 = x_distance | (screen_x_high & ~0xFFFFu);
			x_distance = y_distance | (distance_high_word & ~0xFFFFu);
			screen_x_high = x_distance;
			y_distance = t3;
			distance_high_word = y_distance;
		}
		direction = direction | TABLE8(direction_table, (uint32_t)(int16_t)TABLE16(direction_rows, (uint32_t)(int16_t)(y_distance << 1)) + (int16_t)x_distance);
		return direction;
	}
	while (y_distance >= 0x20u)
	{
		y_distance = y_distance >> 1;
		carry = (uint8_t)(shift + 1u) < shift;
		extend = carry;
		shift++;
	}
	x_distance = m68k_shift(M68K_LSR, x_distance, shift & 0x3Fu, extend, 16);
	if ((direction & 0x40u) != 0u)
	{
		t3 = x_distance | (screen_x_high & ~0xFFFFu);
		x_distance = y_distance | (distance_high_word & ~0xFFFFu);
		screen_x_high = x_distance;
		y_distance = t3;
		distance_high_word = y_distance;
	}
	direction = direction | TABLE8(direction_table, (uint32_t)(int16_t)TABLE16(direction_rows, (uint32_t)(int16_t)(y_distance << 1)) + (int16_t)x_distance);
	return direction;
}
