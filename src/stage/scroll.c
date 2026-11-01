/**
 *    Name:        stage/scroll.c
 *    Description: the areas' scroll segments: the layers' limits and speeds
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
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
	setting = SCROLL_SETTING_LIST(area_scroll_segments[(uint8_t)((uint32_t)stage_number << 2) / 4u] + (int16_t)scroll_segment);
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
uint32_t auto_scroll(uint32_t d1, uint32_t turtle)
{
	uint32_t carry = 0;
	uint32_t column_phase = 0, new_column_x = 0, new_row_y = 0, row_phase = 0, x_step = 0, y_step = 0;
	uint16_t column_phase_2 = 0, column_phase_3 = 0, row_phase_2 = 0, row_phase_3 = 0;

	if (scroll_mode >= 0)
		return d1;
	x_step = LAYER(WORKING_LAYER)->x_speed;
	column_phase = (d1 & ~0xFFFFu) | x_step;
	column_phase = (column_phase & ~0xFFFFu) | (uint16_t)(column_phase << 5);
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
			if (column_phase_3 < (uint16_t)column_phase)
			{
				LAYER(WORKING_LAYER)->new_column_x = new_column_x;
				LAYER(WORKING_LAYER)->new_column--;
			}
		}
	}
	y_step = LAYER(WORKING_LAYER)->y_speed;
	row_phase = (column_phase & ~0xFFFFu) | y_step;
	row_phase = (row_phase & ~0xFFFFu) | (uint16_t)(row_phase << 5);
	if ((uint8_t)LAYER(WORKING_LAYER)->y_direction == 0u)
		return row_phase;
	if (LAYER(WORKING_LAYER)->y_direction < 0)
	{
		LAYER(WORKING_LAYER)->y -= y_step;
		new_row_y = LAYER(WORKING_LAYER)->y - 0xF800u;
		row_phase_2 = LAYER(WORKING_LAYER)->row_phase;
		LAYER(WORKING_LAYER)->row_phase = row_phase_2 - row_phase;
		if (row_phase_2 < (uint16_t)row_phase)
			return queue_scroll_row(new_row_y, row_phase, turtle);
		return row_phase;
	}
	LAYER(WORKING_LAYER)->y += y_step;
	new_row_y = LAYER(WORKING_LAYER)->y;
	row_phase_3 = LAYER(WORKING_LAYER)->row_phase;
	carry = (uint16_t)(row_phase_3 + row_phase) < row_phase_3;
	LAYER(WORKING_LAYER)->row_phase = row_phase_3 + row_phase;
	if (carry)
		return queue_scroll_row(new_row_y, row_phase, turtle);
	return row_phase;
}

/* every frame of play: the time without a knock-out, the difficulty it adds, the balloon's time */
void update_time_without_knockout(uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d5 = *p_d5;
	uint32_t carry = 0;
	uint8_t t1 = 0;

	time_without_knockout++;
	difficulty_from_time = (uint8_t)(time_without_knockout >> 8u) >> 5;
	if (balloon_time == 0u)
		carry = hold_scroll_for_enemies();
	else
	{
		t1 = balloon_time;
		balloon_time = t1 - 1u;
		if (t1 != 1u)
			carry = hold_scroll_for_enemies();
		else
		{
			erase_balloon(&d5, p_d7);
			carry = hold_scroll_for_enemies();
		}
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
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
