/**
 *    Name:        stage/scroll.c
 *    Description: the areas' scroll segments: the layers' limits and speeds
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the area's next scroll segment: the layers' limits, or their speeds when it scrolls by itself; at the
   end, the area's check */
void start_scroll_segment(void)
{
	uint32_t negative = 0;
	uint16_t stage_number = 0;
	uint32_t setting = 0;
	uint8_t mode = 0;

	stage_number = stage;
	stage_number = (uint8_t)((uint32_t)stage_number << 2);
	setting = SCROLL_SETTING_LIST(area_scroll_segments[stage_number / 4u] + (int16_t)scroll_segment);
	mode = SCROLL_SETTING(setting)->mode;
	negative = mode >> 7;
	scroll_mode = mode;
	if (mode == 0u)
	{
		check_area_cleared();
		return;
	}
	if (negative == 0u)
	{
		scroll_held = SCROLL_SETTING(setting)->held;
		LAYER(LAYER_A)->x_max = SCROLL_SETTING(setting)->a_x_max;
		LAYER(LAYER_A)->y_max = SCROLL_SETTING(setting)->a_y_max;
		LAYER(LAYER_B)->x_max = SCROLL_SETTING(setting)->b_x_max;
		LAYER(LAYER_B)->y_max = SCROLL_SETTING(setting)->b_y_max;
		LAYER(LAYER_A)->x_min = SCROLL_SETTING(setting)->a_x_min;
		LAYER(LAYER_A)->y_min = SCROLL_SETTING(setting)->a_y_min;
		LAYER(LAYER_B)->x_min = SCROLL_SETTING(setting)->b_x_min;
		LAYER(LAYER_B)->y_min = SCROLL_SETTING(setting)->b_y_min;
		if (stage_scroll_locks[stage] != 0u)
		{
			LAYER(WORKING_LAYER)->x_max = LAYER(LAYER_B)->x_max;
			LAYER(WORKING_LAYER)->y_max = LAYER(LAYER_B)->y_max;
			LAYER(WORKING_LAYER)->x_min = LAYER(LAYER_B)->x_min;
			LAYER(WORKING_LAYER)->y_min = LAYER(LAYER_B)->y_min;
			return;
		}
		LAYER(WORKING_LAYER)->x_max = LAYER(LAYER_A)->x_max;
		LAYER(WORKING_LAYER)->y_max = LAYER(LAYER_A)->y_max;
		LAYER(WORKING_LAYER)->x_min = LAYER(LAYER_A)->x_min;
		LAYER(WORKING_LAYER)->y_min = LAYER(LAYER_A)->y_min;
		return;
	}
	LAYER(LAYER_A)->x_speed = SCROLL_SETTING(setting)->a_x_speed;
	LAYER(LAYER_A)->y_speed = SCROLL_SETTING(setting)->a_y_speed;
	LAYER(LAYER_B)->x_speed = SCROLL_SETTING(setting)->b_x_speed;
	LAYER(LAYER_B)->y_speed = SCROLL_SETTING(setting)->b_y_speed;
	LAYER(WORKING_LAYER)->y_direction = SCROLL_SETTING(setting)->y_direction;
	LAYER(LAYER_A)->y_direction = SCROLL_SETTING(setting)->y_direction;
	LAYER(LAYER_B)->y_direction = SCROLL_SETTING(setting)->y_direction;
	LAYER(WORKING_LAYER)->x_direction = SCROLL_SETTING(setting)->x_direction;
	LAYER(LAYER_A)->x_direction = SCROLL_SETTING(setting)->x_direction;
	LAYER(LAYER_B)->x_direction = SCROLL_SETTING(setting)->x_direction;
	if (stage_scroll_locks[stage] != 0u)
	{
		LAYER(WORKING_LAYER)->x_speed = LAYER(LAYER_B)->x_speed;
		LAYER(WORKING_LAYER)->y_speed = LAYER(LAYER_B)->y_speed;
		return;
	}
	LAYER(WORKING_LAYER)->x_speed = LAYER(LAYER_A)->x_speed;
	LAYER(WORKING_LAYER)->y_speed = LAYER(LAYER_A)->y_speed;
}

/* a segment that scrolls by itself: the layer moved by its speeds, a new column or row to draw past each
   tile */
void auto_scroll(uint32_t turtle)
{
	uint32_t carry = 0;
	uint16_t column_phase = 0, row_phase = 0;
	uint32_t new_column_x = 0, new_row_y = 0, x_step = 0, y_step = 0;
	uint16_t column_phase_2 = 0, column_phase_3 = 0, row_phase_2 = 0, row_phase_3 = 0;

	if (scroll_mode >= 0)
		return;
	x_step = LAYER(WORKING_LAYER)->x_speed;
	column_phase = (uint16_t)(x_step << 5);
	if ((uint8_t)LAYER(WORKING_LAYER)->x_direction != 0u)
	{
		if (LAYER(WORKING_LAYER)->x_direction < 0)
		{
			LAYER(WORKING_LAYER)->x += x_step;
			new_column_x = LAYER(WORKING_LAYER)->x + 0x15800u;
			column_phase_2 = LAYER(WORKING_LAYER)->column_phase;
			carry = (uint16_t)(column_phase_2 + column_phase) < column_phase_2;
			LAYER(WORKING_LAYER)->column_phase = column_phase_2 + column_phase;
			if (carry)
			{
				LAYER(WORKING_LAYER)->new_column_x = new_column_x;
				LAYER(WORKING_LAYER)->new_column--;
			}
		}
		else
		{
			LAYER(WORKING_LAYER)->x -= x_step;
			new_column_x = LAYER(WORKING_LAYER)->x - 0x1800u;
			column_phase_3 = LAYER(WORKING_LAYER)->column_phase;
			LAYER(WORKING_LAYER)->column_phase = column_phase_3 - column_phase;
			if (column_phase_3 < column_phase)
			{
				LAYER(WORKING_LAYER)->new_column_x = new_column_x;
				LAYER(WORKING_LAYER)->new_column--;
			}
		}
	}
	y_step = LAYER(WORKING_LAYER)->y_speed;
	row_phase = (uint16_t)(y_step << 5);
	if ((uint8_t)LAYER(WORKING_LAYER)->y_direction == 0u)
	{
		screen_x_high = (screen_x_high & ~0xFF00u) | (row_phase & 0xFF00u);
		return;
	}
	if (LAYER(WORKING_LAYER)->y_direction < 0)
	{
		LAYER(WORKING_LAYER)->y -= y_step;
		new_row_y = LAYER(WORKING_LAYER)->y - 0xF800u;
		row_phase_2 = LAYER(WORKING_LAYER)->row_phase;
		LAYER(WORKING_LAYER)->row_phase = row_phase_2 - row_phase;
		if (row_phase_2 < row_phase)
		{
			screen_x_high = (screen_x_high & ~0xFF00u) | (row_phase & 0xFF00u);
			queue_scroll_row(new_row_y, turtle);
			return;
		}
		screen_x_high = (screen_x_high & ~0xFF00u) | (row_phase & 0xFF00u);
		return;
	}
	LAYER(WORKING_LAYER)->y += y_step;
	new_row_y = LAYER(WORKING_LAYER)->y;
	row_phase_3 = LAYER(WORKING_LAYER)->row_phase;
	carry = (uint16_t)(row_phase_3 + row_phase) < row_phase_3;
	LAYER(WORKING_LAYER)->row_phase = row_phase_3 + row_phase;
	if (carry)
	{
		screen_x_high = (screen_x_high & ~0xFF00u) | (row_phase & 0xFF00u);
		queue_scroll_row(new_row_y, turtle);
		return;
	}
	screen_x_high = (screen_x_high & ~0xFF00u) | (row_phase & 0xFF00u);
}

/* every frame of play: the time without a knock-out, the difficulty it adds, the balloon's time */
void update_time_without_knockout(void)
{
	uint32_t carry = 0;
	uint8_t t1 = 0;

	time_without_knockout++;
	difficulty_from_time = (uint8_t)(time_without_knockout >> 8u) >> 5;
	if (balloon_time != 0u)
	{
		t1 = balloon_time;
		balloon_time = t1 - 1u;
		if (t1 == 1u)
		{
			erase_balloon();
			carry = hold_scroll_for_enemies();
			return;
		}
	}
	carry = hold_scroll_for_enemies();
}

/* the scroll is held while more enemies are on the screen than the stage allows at this difficulty */
uint32_t hold_scroll_for_enemies(void)
{
	uint8_t enemy_limit = 0, held = 0;
	uint32_t stage_number = 0;

	stage_number = stage;
	enemy_limit = enemy_limits[(uint32_t)(uint16_t)(stage_number << 3) + difficulty];
	held = 0u;
	switch (stage)
	{
	case 0:
		return hold_scroll_building(held, enemy_limit);
	case 1:
		return hold_scroll_apartment(held, enemy_limit);
	case 2:
		return hold_scroll_street(held, enemy_limit);
	case 3:
		return hold_scroll_sewer(held, enemy_limit);
	case 4:
		return hold_scroll_car_street(held, enemy_limit);
	case 5:
		return hold_scroll_highway(held, enemy_limit);
	case 6:
		return hold_scroll_skateboard(enemy_limit);
	case 7:
		return hold_scroll_building_site(held, enemy_limit);
	case 8:
		return hold_scroll_technodrome(held, enemy_limit);
	case 9:
		return hold_scroll_final(held, enemy_limit);
	}
	unreachable();
}

/* the burning building's scroll held while the screen has too many enemies */
uint32_t hold_scroll_building(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen || building_scroll_hold != 0u)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the apartment's scroll held while the screen has too many enemies */
uint32_t hold_scroll_apartment(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the street's scroll held, right and down, while the screen has too many enemies */
uint32_t hold_scroll_street(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen)
		held--;
	scroll_held_down = held;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the sewer's scroll held while the screen has too many enemies */
uint32_t hold_scroll_sewer(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the parked cars' street's scroll held while the screen has too many enemies */
uint32_t hold_scroll_car_street(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the highway's scroll held while the screen has too many enemies */
uint32_t hold_scroll_highway(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the skateboard ride is never held */
uint32_t hold_scroll_skateboard(uint8_t enemy_limit)
{
	uint32_t carry = 0;

	carry = enemy_limit < enemies_on_screen;
	return carry;
}

/* the building site's scroll held while the screen has too many enemies, unless the drill is arriving */
uint32_t hold_scroll_building_site(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	carry = 0u;
	if ((uint8_t)drill_arrival != 0u && drill_arrival >= 0)
		return carry;
	if (enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the Technodrome's scroll held on its middle stretch, and while the screen has too many enemies */
uint32_t hold_scroll_technodrome(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if ((LAYER(LAYER_B)->x < 0xB8000u && LAYER(LAYER_B)->x >= 0x58000u) || enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}

/* the last area's scroll held while the screen has too many enemies */
uint32_t hold_scroll_final(uint8_t held, uint8_t enemy_limit)
{
	uint32_t carry = 0;

	if (enemy_limit < enemies_on_screen)
		held--;
	carry = 0u;
	scroll_held_right = held;
	return carry;
}
