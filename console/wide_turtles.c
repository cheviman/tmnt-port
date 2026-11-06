/**
 *    Name:        wide_turtles.c
 *    Description: WIDE's room for the turtles: the game's functions that keep them on the board's screen, with the
 *                 wider screen's sides added to their limits
 *    Version:     0.1.0
 *    Created:     05-11-2026
 *    Modified:    05-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A turtle's x is the screen's (the K051960's: the board's 320 columns from 0x60 to 0x1A0), and the game keeps it
 *    between 0x90 and 0x160 (turtle_run_all), scrolls the layers when one passes 0x160 or, where the area lets it,
 *    0x91 (turtle_move, its pushes), holds the scroll while another turtle is at the other edge
 *    (turtle_other_at_edge) and keeps a knocked one in (the slides, turtle_knockback_arc, turtle_walk_off_screen):
 *    with WIDE the turtles walk into the wider screen's sides too (Sebas: "no puedo ir mas de esta parte para la
 *    izquierda"). These are the game's own (src/objects/turtles.c, the same C), those limits moved by the left side's
 *    columns and by the right side's, at most 48: the game draws the floor map only 8 pixels past the board's right
 *    edge (its new column), while at the left the floor map's ring keeps the columns the scroll has passed, so a turtle
 *    there reads its walls and holes as on the board. Without WIDE (or out of a game's play) the game's own run.
 */
#include "console_internal.h"

#include "game.h"

#define RIGHT_MOST  48    /* the right side's columns the turtles may walk into: the board's 320 less 16 */

/* WIDE's room for the turtles now, in their x's units (a pixel 0x100): false when there is none */
static uint32_t turtle_left, turtle_right;

static bool turtle_limits(void)
{
	int left, right;

	console_wide_sides(&left, &right);
	if (console.page != PAGE_GAME)
		left = right = 0;
	turtle_left = (uint32_t)left << 8;
	turtle_right = (uint32_t)(right < RIGHT_MOST ? right : RIGHT_MOST) << 8;
	return turtle_left || turtle_right;
}

/* every turtle in play a frame: controls, move, the screen's limits, the scroll, floor and walls */
void turtle_run_all(void)
{
	uint8_t routine_past_walk = 0, segment_end_axis = 0;
	uint16_t scroll_stage = 0;
	uint32_t end = 0, inputs = 0, no_scroll = 0, object = 0, presses = 0, scroll_setting = 0, x = 0, x_2 = 0, x_speed = 0, y_speed = 0;
	uint8_t routine = 0, end_axis = 0;
	uint32_t y = 0;

	if (!turtle_limits())
	{
		original_turtle_run_all();
		return;
	}
	diagonal_scroll = 0;
	if (stage == 2u && scroll_segment == 4u)
	{
		diagonal_scroll--;
		if (LAYER(LAYER_B)->y < 0x1C000u)
			scroll_held_right = 0xFFu;
	}
	no_scroll = 0u;
	LAYER(WORKING_LAYER)->new_row = 0u;
	LAYER(WORKING_LAYER)->new_column = 0u;
	LAYER(LAYER_A)->new_row = 0u;
	LAYER(LAYER_A)->new_column = 0u;
	LAYER(LAYER_B)->new_row = 0u;
	LAYER(LAYER_B)->new_column = 0u;
	turtles_busy = 0u;
	if (scroll_mode >= 0)
	{
		LAYER(WORKING_LAYER)->y_direction = (uint8_t)(no_scroll >> 8u);
		LAYER(WORKING_LAYER)->x_direction = (uint8_t)no_scroll;
		LAYER(WORKING_LAYER)->x_speed = (uint16_t)(no_scroll >> 16u);
		LAYER(WORKING_LAYER)->y_speed = (uint16_t)no_scroll;
	}
	object = OBJECT_ADDRESS(0);
	inputs = turtle_inputs_ADDRESS;
	presses = turtle_presses_ADDRESS;
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			turtle_control(object, inputs, presses);
			turtle_move(object, inputs);
			if (OBJECT(object)->kind == 3u && (((uint32_t)OBJECT(object)->routine << 8u) | OBJECT(object)->state) == 0x307u)
				OBJECT(object)->contacts |= 0x80u;
			if (stage != 8u || scroll_mode >= 0)
			{
				if (OBJECT(object)->x >= (0x9000u - turtle_left))
				{
					if (OBJECT(object)->x >= (0x16000u + turtle_right))
						OBJECT(object)->x = (0x16000u + turtle_right);
				}
				else
					OBJECT(object)->x = (0x9000u - turtle_left);
				if (OBJECT(object)->y >= 0x1000u)
				{
					if (OBJECT(object)->y >= 0xA000u)
						OBJECT(object)->y = 0xA000u;
				}
				else
					OBJECT(object)->y = 0x1000u;
				if ((int8_t)OBJECT(object)->height_whole < 0)
				{
					routine = OBJECT(object)->routine;
					routine_past_walk = (uint8_t)(routine - 2u);
					if (routine < 2u || routine_past_walk >= 3u)
					{
						OBJECT(object)->height = 0u;
						OBJECT(object)->floor_height = 0u;
					}
				}
			}
			else
			{
				if (OBJECT(object)->x >= 0x18800u)
					OBJECT(object)->x = 0x18800u;
				y = OBJECT(object)->y;
				x = y - 0x1000u;
				if (y < 0x1000u)
					x = 0u;
				x_2 = x + 0x13000u;
				x += 0x8000u;
				if (OBJECT(object)->x <= x)
					OBJECT(object)->x = x;
				if (x_2 < OBJECT(object)->x)
					OBJECT(object)->x = x_2;
			}
		}
		presses++;
		object += 0x50u;
		inputs++;
	} while (object < OBJECT_ADDRESS(4));
	if (stage == 6u)
		object = turtle_skateboard_shadow();
	auto_scroll(object);
	object = OBJECT_ADDRESS(0);
	inputs = turtle_inputs_ADDRESS;
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			if (scroll_mode >= 0)
			{
				x_speed = LAYER(WORKING_LAYER)->x_speed;
				y_speed = LAYER(WORKING_LAYER)->y_speed;
				if ((OBJECT(object)->parameter & 0x7Fu) == 0u && (uint8_t)LAYER(WORKING_LAYER)->x_direction != 0u)
				{
					if (LAYER(WORKING_LAYER)->x_direction < 0)
						OBJECT(object)->x -= x_speed;
					else
						OBJECT(object)->x += x_speed;
				}
				if ((int8_t)OBJECT(object)->parameter >= 0 && (uint8_t)LAYER(WORKING_LAYER)->y_direction != 0u)
				{
					if (LAYER(WORKING_LAYER)->y_direction >= 0)
						OBJECT(object)->y -= y_speed;
					else
						OBJECT(object)->y += y_speed;
				}
			}
			OBJECT(object)->screen_x = (OBJECT(object)->screen_x & 0xFFu) | (((uint8_t)(OBJECT(object)->x >> 16u) & 0xFFu) << 8u);
			OBJECT(object)->screen_x = (OBJECT(object)->screen_x & 0xFF00u) | ((uint8_t)(OBJECT(object)->x >> 8u) & 0xFFu);
			OBJECT(object)->screen_y = (OBJECT(object)->screen_y & 0xFFu) | (((uint8_t)(OBJECT(object)->y >> 16u) & 0xFFu) << 8u);
			OBJECT(object)->screen_y = (OBJECT(object)->screen_y & 0xFF00u) | ((uint8_t)(OBJECT(object)->y >> 8u) & 0xFFu);
			if (stage == 8u)
			{
				save_layer_b();
				load_layer_a();
				turtle_collide(object, inputs);
				save_layer_a();
				load_layer_b();
			}
			else
				turtle_collide(object, inputs);
		}
		if (OBJECT(object)->kind == 3u && OBJECT(object)->state == 7u)
			OBJECT(object)->contacts |= 0x80u;
		OBJECT(object)->screen_x = (OBJECT(object)->screen_x & 0xFFu) | (((uint8_t)(OBJECT(object)->x >> 16u) & 0xFFu) << 8u);
		OBJECT(object)->screen_x = (OBJECT(object)->screen_x & 0xFF00u) | ((uint8_t)(OBJECT(object)->x >> 8u) & 0xFFu);
		OBJECT(object)->screen_y = (OBJECT(object)->screen_y & 0xFFu) | (((uint8_t)(OBJECT(object)->y >> 16u) & 0xFFu) << 8u);
		OBJECT(object)->screen_y = (OBJECT(object)->screen_y & 0xFF00u) | ((uint8_t)(OBJECT(object)->y >> 8u) & 0xFFu);
		object += 0x50u;
		inputs++;
	} while (object < OBJECT_ADDRESS(4));
	if (last_segment != 0u)
	{
		check_area_cleared();
		if (area_beaten != 0u && turtles_busy == 0u)
			area_cleared--;
		object_loop_slot = inputs;
		return;
	}
	scroll_stage = stage;
	scroll_stage = (uint8_t)((uint32_t)scroll_stage << 2);
	scroll_setting = SCROLL_SETTING_LIST(area_scroll_segments[scroll_stage / 4u] + (int16_t)scroll_segment);
	if (SCROLL_SETTING(scroll_setting)->mode == 0u)
	{
		object_loop_slot = inputs;
		return;
	}
	end = LAYER(WORKING_LAYER)->x;
	end_axis = SCROLL_SETTING(scroll_setting)->end_axis;
	segment_end_axis = end_axis;
	if ((int8_t)end_axis < 0)
		end = LAYER(WORKING_LAYER)->y;
	if ((segment_end_axis & 0x7Fu) != 0u)
	{
		if (SCROLL_SETTING(scroll_setting)->end <= end)
		{
			object_loop_slot = inputs;
			return;
		}
		scroll_segment += 4u;
		object_loop_slot = inputs;
		start_scroll_segment();
		return;
	}
	if (SCROLL_SETTING(scroll_setting)->end <= end)
	{
		scroll_segment += 4u;
		object_loop_slot = inputs;
		start_scroll_segment();
		return;
	}
	object_loop_slot = inputs;
}


/* the stick moves the turtle across and up or down, or pushes the scroll at the screen's edges */
void turtle_move(uint32_t object, uint32_t inputs)
{
	bool L_01AE2C = false, L_01AEA8 = false, L_01AFD4 = false, L_01B012 = false, L_01B086 = false;
	uint32_t carry = 0;
	uint8_t direction_inputs = 0, edge = 0;
	uint16_t x_speed = 0;
	uint32_t new_column_x = 0, phase_step = 0, scroll_x_moved = 0, speed_offset = 0, walk_speeds = 0, x_max = 0, y_max = 0, y_speed = 0;
	uint8_t t1 = 0, t2 = 0;
	uint16_t column_phase = 0, x_speed_2 = 0, column_phase_2 = 0, column_phase_3 = 0, x_speed_3 = 0, column_phase_4 = 0, row_phase = 0, y_speed_2 = 0, row_phase_2 = 0, row_phase_3 = 0, row_phase_4 = 0, y_speed_3 = 0;

	if (!turtle_limits())
	{
		original_turtle_move(object, inputs);
		return;
	}
	OBJECT(object)->turtle.scroll_push = 0u;
	t1 = PLAYER_INPUT(inputs)->bits & 3u;
	direction_inputs = t1;
	if (t1 != 0u)
	{
		if ((direction_inputs & 1u) == 0u)
		{
			OBJECT(object)->flags &= 0xBFu;
			if (scroll_mode >= 0)
			{
				if ((uint8_t)LAYER(WORKING_LAYER)->x_direction != 0u)
				{
					turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
					x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
					if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
					{
						if ((OBJECT(object)->contacts & 8u) == 0u)
							OBJECT(object)->x += x_speed & 0xFFFFu;
					}
					else if ((OBJECT(object)->contacts & 0x28u) == 0u)
						OBJECT(object)->x += x_speed & 0xFFFFu;
				}
				else
				{
					do
					{
						if (L_01AFD4 || (uint8_t)diagonal_scroll == 0u)
						{
							if (!L_01AFD4 && OBJECT(object)->x < (0x12000u + turtle_right))
							{
								turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
								x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
								if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
								{
									if ((OBJECT(object)->contacts & 8u) == 0u)
										OBJECT(object)->x += x_speed & 0xFFFFu;
								}
								else if ((OBJECT(object)->contacts & 0x28u) == 0u)
									OBJECT(object)->x += x_speed & 0xFFFFu;
							}
							else
							{
								L_01AFD4 = false;
								if (OBJECT(object)->x >= (0x16000u + turtle_right))
								{
									do
									{
										if (L_01B086 || (PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
										{
											if (L_01B086 || (OBJECT(object)->contacts & 8u) == 0u)
											{
												L_01B086 = false;
												if (scroll_held_right == 0u)
												{
													edge = 0x7Fu;
													carry = turtle_other_at_edge(edge, object);
													if (carry == 0u)
													{
														turtle_push_scroll_right(object, inputs, &new_column_x, &phase_step, &x_max, &carry);
														if (carry)
														{
															LAYER(WORKING_LAYER)->x = x_max;
															LAYER(WORKING_LAYER)->x_speed = 0u;
															LAYER(WORKING_LAYER)->column_phase = 0u;
															LAYER(WORKING_LAYER)->x_direction = 0;
														}
														else
														{
															column_phase = LAYER(WORKING_LAYER)->column_phase;
															carry = (uint16_t)(column_phase + phase_step) < column_phase;
															LAYER(WORKING_LAYER)->column_phase = column_phase + phase_step;
															if (carry)
															{
																LAYER(WORKING_LAYER)->new_column_x = new_column_x;
																LAYER(WORKING_LAYER)->new_column--;
															}
														}
													}
												}
											}
										}
										else
											L_01B086 = (OBJECT(object)->contacts & 0x28u) == 0u;
									} while (L_01B086);
								}
								else if (scroll_held_right != 0u)
								{
									turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
									x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
									if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
									{
										if ((OBJECT(object)->contacts & 8u) == 0u)
											OBJECT(object)->x += x_speed & 0xFFFFu;
									}
									else if ((OBJECT(object)->contacts & 0x28u) == 0u)
										OBJECT(object)->x += x_speed & 0xFFFFu;
								}
								else
								{
									do
									{
										if (L_01B012 || (PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
										{
											if (L_01B012 || (OBJECT(object)->contacts & 8u) == 0u)
											{
												L_01B012 = false;
												edge = 0x7Fu;
												carry = turtle_other_at_edge(edge, object);
												if (carry)
												{
													turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
													x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
													if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
													{
														if ((OBJECT(object)->contacts & 8u) == 0u)
															OBJECT(object)->x += x_speed & 0xFFFFu;
													}
													else if ((OBJECT(object)->contacts & 0x28u) == 0u)
														OBJECT(object)->x += x_speed & 0xFFFFu;
												}
												else
												{
													turtle_push_scroll_right(object, inputs, &new_column_x, &phase_step, &x_max, &carry);
													if (carry)
													{
														scroll_x_moved = LAYER(WORKING_LAYER)->x - x_max;
														x_speed = (uint16_t)scroll_x_moved;
														scroll_x_moved = (scroll_x_moved << 16) | (scroll_x_moved >> 16);
														x_speed_2 = LAYER(WORKING_LAYER)->x_speed;
														LAYER(WORKING_LAYER)->x_speed = x_speed_2 - x_speed;
														if (x_speed_2 == (uint16_t)x_speed)
															LAYER(WORKING_LAYER)->x_direction = scroll_x_moved;
														LAYER(WORKING_LAYER)->x = x_max;
														LAYER(WORKING_LAYER)->column_phase = scroll_x_moved;
														if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
														{
															if ((OBJECT(object)->contacts & 8u) == 0u)
																OBJECT(object)->x += x_speed & 0xFFFFu;
														}
														else if ((OBJECT(object)->contacts & 0x28u) == 0u)
															OBJECT(object)->x += x_speed & 0xFFFFu;
													}
													else
													{
														column_phase_2 = LAYER(WORKING_LAYER)->column_phase;
														carry = (uint16_t)(column_phase_2 + phase_step) < column_phase_2;
														LAYER(WORKING_LAYER)->column_phase = column_phase_2 + phase_step;
														if (carry)
														{
															LAYER(WORKING_LAYER)->new_column_x = new_column_x;
															LAYER(WORKING_LAYER)->new_column--;
														}
													}
												}
											}
										}
										else
											L_01B012 = (OBJECT(object)->contacts & 0x28u) == 0u;
									} while (L_01B012);
								}
							}
						}
						else
						{
							L_01AFD4 = OBJECT(object)->x >= (0x15800u + turtle_right);
							if (!L_01AFD4)
							{
								turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
								x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
								if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
								{
									if ((OBJECT(object)->contacts & 8u) == 0u)
										OBJECT(object)->x += x_speed & 0xFFFFu;
								}
								else if ((OBJECT(object)->contacts & 0x28u) == 0u)
									OBJECT(object)->x += x_speed & 0xFFFFu;
							}
						}
					} while (L_01AFD4);
				}
			}
			else if (stage == 8u || OBJECT(object)->x < (0x16000u + turtle_right))
			{
				turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
				x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
				if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 0xAu)
				{
					if ((OBJECT(object)->contacts & 8u) == 0u)
						OBJECT(object)->x += x_speed & 0xFFFFu;
				}
				else if ((OBJECT(object)->contacts & 0x28u) == 0u)
					OBJECT(object)->x += x_speed & 0xFFFFu;
			}
		}
		else
		{
			OBJECT(object)->flags |= 0x40u;
			if (scroll_mode >= 0)
			{
				if ((uint8_t)LAYER(WORKING_LAYER)->x_direction != 0u || OBJECT(object)->x >= (0xE000u - turtle_left))
				{
					turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
					x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
					if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
					{
						if ((OBJECT(object)->contacts & 0x10u) == 0u)
							OBJECT(object)->x -= x_speed;
					}
					else if ((OBJECT(object)->contacts & 0x50u) == 0u)
						OBJECT(object)->x -= x_speed;
				}
				else if (OBJECT(object)->x < (0x9100u - turtle_left))
				{
					do
					{
						if (L_01AEA8 || (PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
						{
							if (L_01AEA8 || (OBJECT(object)->contacts & 0x10u) == 0u)
							{
								L_01AEA8 = false;
								if (scroll_held_left == 0u)
								{
									edge = 0u;
									carry = turtle_other_at_edge(edge, object);
									if (carry == 0u)
									{
										turtle_push_scroll_left(object, inputs, &new_column_x, &phase_step, &carry);
										if (carry)
										{
											LAYER(WORKING_LAYER)->x = LAYER(WORKING_LAYER)->x_min;
											LAYER(WORKING_LAYER)->x_speed = 0u;
											LAYER(WORKING_LAYER)->column_phase = 0u;
											LAYER(WORKING_LAYER)->x_direction = 0;
										}
										else
										{
											column_phase_3 = LAYER(WORKING_LAYER)->column_phase;
											LAYER(WORKING_LAYER)->column_phase = column_phase_3 - phase_step;
											if (column_phase_3 < (uint16_t)phase_step)
											{
												LAYER(WORKING_LAYER)->new_column_x = new_column_x;
												LAYER(WORKING_LAYER)->new_column--;
											}
										}
									}
								}
							}
						}
						else
							L_01AEA8 = (OBJECT(object)->contacts & 0x50u) == 0u;
					} while (L_01AEA8);
				}
				else if (scroll_held_left != 0u)
				{
					turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
					x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
					if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
					{
						if ((OBJECT(object)->contacts & 0x10u) == 0u)
							OBJECT(object)->x -= x_speed;
					}
					else if ((OBJECT(object)->contacts & 0x50u) == 0u)
						OBJECT(object)->x -= x_speed;
				}
				else
				{
					do
					{
						if (L_01AE2C || (PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
						{
							if (L_01AE2C || (OBJECT(object)->contacts & 0x10u) == 0u)
							{
								L_01AE2C = false;
								edge = 0u;
								carry = turtle_other_at_edge(edge, object);
								if (carry)
								{
									turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
									x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
									if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
									{
										if ((OBJECT(object)->contacts & 0x10u) == 0u)
											OBJECT(object)->x -= x_speed;
									}
									else if ((OBJECT(object)->contacts & 0x50u) == 0u)
										OBJECT(object)->x -= x_speed;
								}
								else
								{
									turtle_push_scroll_left(object, inputs, &new_column_x, &phase_step, &carry);
									if (carry)
									{
										x_speed = (uint16_t)(LAYER(WORKING_LAYER)->x_min - LAYER(WORKING_LAYER)->x);
										x_speed_3 = LAYER(WORKING_LAYER)->x_speed;
										LAYER(WORKING_LAYER)->x_speed = x_speed_3 - x_speed;
										if (x_speed_3 == x_speed)
											LAYER(WORKING_LAYER)->x_direction = 0;
										LAYER(WORKING_LAYER)->x = LAYER(WORKING_LAYER)->x_min;
										LAYER(WORKING_LAYER)->column_phase = 0u;
										if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
										{
											if ((OBJECT(object)->contacts & 0x10u) == 0u)
												OBJECT(object)->x -= x_speed;
										}
										else if ((OBJECT(object)->contacts & 0x50u) == 0u)
											OBJECT(object)->x -= x_speed;
									}
									else
									{
										column_phase_4 = LAYER(WORKING_LAYER)->column_phase;
										LAYER(WORKING_LAYER)->column_phase = column_phase_4 - phase_step;
										if (column_phase_4 < (uint16_t)phase_step)
										{
											LAYER(WORKING_LAYER)->new_column_x = new_column_x;
											LAYER(WORKING_LAYER)->new_column--;
										}
									}
								}
							}
						}
						else
							L_01AE2C = (OBJECT(object)->contacts & 0x50u) == 0u;
					} while (L_01AE2C);
				}
			}
			else if (OBJECT(object)->x >= (0x9000u - turtle_left))
			{
				turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
				x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
				if ((PLAYER_INPUT(inputs)->bits & 0xFu) != 9u)
				{
					if ((OBJECT(object)->contacts & 0x10u) == 0u)
						OBJECT(object)->x -= x_speed;
				}
				else if ((OBJECT(object)->contacts & 0x50u) == 0u)
					OBJECT(object)->x -= x_speed;
			}
		}
	}
	t2 = PLAYER_INPUT(inputs)->bits & 0xCu;
	direction_inputs = t2;
	if (t2 == 0u)
		return;
	if ((direction_inputs & 8u) != 0u)
	{
		if (scroll_mode >= 0)
		{
			if ((uint8_t)LAYER(WORKING_LAYER)->y_direction != 0u || OBJECT(object)->y >= 0x6000u)
			{
				turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
				y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
				if ((OBJECT(object)->contacts & 4u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y -= y_speed;
				return;
			}
			if (OBJECT(object)->y < 0x1100u)
			{
				if ((OBJECT(object)->contacts & 4u) != 0u || scroll_held_down != 0u)
					return;
				edge = 0x80u;
				carry = turtle_other_at_edge(edge, object);
				if (carry)
					return;
				if ((uint8_t)diagonal_scroll != 0u)
				{
					edge = 0x7Fu;
					carry = turtle_other_at_edge(edge, object);
					if (carry)
						return;
				}
				turtle_push_scroll_down(object, inputs, &scroll_x_moved, &phase_step, &carry);
				if (carry)
				{
					LAYER(WORKING_LAYER)->y = LAYER(WORKING_LAYER)->y_min;
					LAYER(WORKING_LAYER)->y_speed = 0u;
					LAYER(WORKING_LAYER)->row_phase = 0u;
					LAYER(WORKING_LAYER)->y_direction = 0;
					return;
				}
				row_phase = LAYER(WORKING_LAYER)->row_phase;
				LAYER(WORKING_LAYER)->row_phase = row_phase - phase_step;
				if (row_phase < (uint16_t)phase_step)
				{
					queue_scroll_row(scroll_x_moved, object);
					return;
				}
				if ((uint8_t)diagonal_scroll != 0u)
				{
					push_scroll_diagonally(object);
					return;
				}
				return;
			}
			if (scroll_held_down != 0u)
			{
				turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
				y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
				if ((OBJECT(object)->contacts & 4u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y -= y_speed;
				return;
			}
			if ((OBJECT(object)->contacts & 4u) != 0u)
				return;
			edge = 0x80u;
			carry = turtle_other_at_edge(edge, object);
			if (carry)
			{
				turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
				y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
				if ((OBJECT(object)->contacts & 4u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y -= y_speed;
				return;
			}
			if ((uint8_t)diagonal_scroll != 0u)
			{
				edge = 0x7Fu;
				carry = turtle_other_at_edge(edge, object);
				if (carry)
				{
					turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
					y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
					if ((OBJECT(object)->contacts & 4u) != 0u)
						return;
					if (OBJECT(object)->routine == 3u)
						y_speed = y_speed >> 3;
					OBJECT(object)->y -= y_speed;
					return;
				}
			}
			turtle_push_scroll_down(object, inputs, &scroll_x_moved, &phase_step, &carry);
			if (carry)
			{
				y_speed = (uint16_t)(LAYER(WORKING_LAYER)->y_min - LAYER(WORKING_LAYER)->y);
				y_speed_2 = LAYER(WORKING_LAYER)->y_speed;
				LAYER(WORKING_LAYER)->y_speed = y_speed_2 - y_speed;
				if (y_speed_2 == y_speed)
					LAYER(WORKING_LAYER)->y_direction = 0;
				LAYER(WORKING_LAYER)->y = LAYER(WORKING_LAYER)->y_min;
				LAYER(WORKING_LAYER)->row_phase = 0u;
				if ((OBJECT(object)->contacts & 4u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y -= y_speed;
				return;
			}
			row_phase_2 = LAYER(WORKING_LAYER)->row_phase;
			LAYER(WORKING_LAYER)->row_phase = row_phase_2 - phase_step;
			if (row_phase_2 < (uint16_t)phase_step)
			{
				queue_scroll_row(scroll_x_moved, object);
				return;
			}
			if ((uint8_t)diagonal_scroll != 0u)
			{
				push_scroll_diagonally(object);
				return;
			}
			return;
		}
		if (OBJECT(object)->y < 0x1000u)
			return;
		turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
		y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
		if ((OBJECT(object)->contacts & 4u) != 0u)
			return;
		if (OBJECT(object)->routine == 3u)
			y_speed = y_speed >> 3;
		OBJECT(object)->y -= y_speed;
		return;
	}
	if (scroll_mode >= 0)
	{
		if ((uint8_t)LAYER(WORKING_LAYER)->y_direction != 0u || OBJECT(object)->y < 0x6000u)
		{
			turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
			y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
			direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
			if (direction_inputs == 5u)
			{
				if ((OBJECT(object)->contacts & 0x52u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y += y_speed;
				return;
			}
			if (direction_inputs != 6u)
			{
				if ((OBJECT(object)->contacts & 0x62u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y += y_speed;
				return;
			}
			if ((OBJECT(object)->contacts & 0x2Au) != 0u)
				return;
			if (OBJECT(object)->routine == 3u)
				y_speed = y_speed >> 3;
			OBJECT(object)->y += y_speed;
			return;
		}
		if (OBJECT(object)->y >= 0xA000u)
		{
			direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
			if (direction_inputs == 5u)
			{
				if ((OBJECT(object)->contacts & 0x50u) != 0u)
					return;
				if (scroll_held_up != 0u)
					return;
				edge = 0xFFu;
				carry = turtle_other_at_edge(edge, object);
				if (carry)
					return;
				turtle_push_scroll_up(object, inputs, &scroll_x_moved, &phase_step, &y_max, &carry);
				if (carry)
				{
					LAYER(WORKING_LAYER)->y = y_max;
					LAYER(WORKING_LAYER)->y_speed = 0u;
					LAYER(WORKING_LAYER)->row_phase = 0u;
					LAYER(WORKING_LAYER)->y_direction = 0;
					return;
				}
				row_phase_3 = LAYER(WORKING_LAYER)->row_phase;
				carry = (uint16_t)(row_phase_3 + phase_step) < row_phase_3;
				LAYER(WORKING_LAYER)->row_phase = row_phase_3 + phase_step;
				if (carry)
				{
					queue_scroll_row(scroll_x_moved, object);
					return;
				}
				return;
			}
			if (direction_inputs != 6u)
			{
				if ((OBJECT(object)->contacts & 0x62u) != 0u)
					return;
				if (scroll_held_up != 0u)
					return;
				edge = 0xFFu;
				carry = turtle_other_at_edge(edge, object);
				if (carry)
					return;
				turtle_push_scroll_up(object, inputs, &scroll_x_moved, &phase_step, &y_max, &carry);
				if (carry)
				{
					LAYER(WORKING_LAYER)->y = y_max;
					LAYER(WORKING_LAYER)->y_speed = 0u;
					LAYER(WORKING_LAYER)->row_phase = 0u;
					LAYER(WORKING_LAYER)->y_direction = 0;
					return;
				}
				row_phase_3 = LAYER(WORKING_LAYER)->row_phase;
				carry = (uint16_t)(row_phase_3 + phase_step) < row_phase_3;
				LAYER(WORKING_LAYER)->row_phase = row_phase_3 + phase_step;
				if (carry)
				{
					queue_scroll_row(scroll_x_moved, object);
					return;
				}
				return;
			}
			if ((OBJECT(object)->contacts & 0x28u) != 0u)
				return;
			if (scroll_held_up != 0u)
				return;
			edge = 0xFFu;
			carry = turtle_other_at_edge(edge, object);
			if (carry)
				return;
			turtle_push_scroll_up(object, inputs, &scroll_x_moved, &phase_step, &y_max, &carry);
			if (carry)
			{
				LAYER(WORKING_LAYER)->y = y_max;
				LAYER(WORKING_LAYER)->y_speed = 0u;
				LAYER(WORKING_LAYER)->row_phase = 0u;
				LAYER(WORKING_LAYER)->y_direction = 0;
				return;
			}
			row_phase_3 = LAYER(WORKING_LAYER)->row_phase;
			carry = (uint16_t)(row_phase_3 + phase_step) < row_phase_3;
			LAYER(WORKING_LAYER)->row_phase = row_phase_3 + phase_step;
			if (carry)
			{
				queue_scroll_row(scroll_x_moved, object);
				return;
			}
			return;
		}
		if (scroll_held_up != 0u)
		{
			turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
			y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
			direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
			if (direction_inputs == 5u)
			{
				if ((OBJECT(object)->contacts & 0x52u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y += y_speed;
				return;
			}
			if (direction_inputs != 6u)
			{
				if ((OBJECT(object)->contacts & 0x62u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y += y_speed;
				return;
			}
			if ((OBJECT(object)->contacts & 0x2Au) != 0u)
				return;
			if (OBJECT(object)->routine == 3u)
				y_speed = y_speed >> 3;
			OBJECT(object)->y += y_speed;
			return;
		}
		direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
		if (direction_inputs == 5u)
		{
			if ((OBJECT(object)->contacts & 0x50u) != 0u)
				return;
		}
		else if (direction_inputs != 6u)
		{
			if ((OBJECT(object)->contacts & 0x62u) != 0u)
				return;
		}
		else if ((OBJECT(object)->contacts & 0x28u) != 0u)
			return;
		edge = 0xFFu;
		carry = turtle_other_at_edge(edge, object);
		if (carry)
		{
			turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
			y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
			direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
			if (direction_inputs == 5u)
			{
				if ((OBJECT(object)->contacts & 0x52u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y += y_speed;
				return;
			}
			if (direction_inputs != 6u)
			{
				if ((OBJECT(object)->contacts & 0x62u) != 0u)
					return;
				if (OBJECT(object)->routine == 3u)
					y_speed = y_speed >> 3;
				OBJECT(object)->y += y_speed;
				return;
			}
			if ((OBJECT(object)->contacts & 0x2Au) != 0u)
				return;
			if (OBJECT(object)->routine == 3u)
				y_speed = y_speed >> 3;
			OBJECT(object)->y += y_speed;
			return;
		}
		turtle_push_scroll_up(object, inputs, &scroll_x_moved, &phase_step, &y_max, &carry);
		if (carry == 0u)
		{
			row_phase_4 = LAYER(WORKING_LAYER)->row_phase;
			carry = (uint16_t)(row_phase_4 + phase_step) < row_phase_4;
			LAYER(WORKING_LAYER)->row_phase = row_phase_4 + phase_step;
			if (carry)
			{
				queue_scroll_row(scroll_x_moved, object);
				return;
			}
			return;
		}
		scroll_x_moved -= y_max;
		y_speed = (uint16_t)scroll_x_moved;
		scroll_x_moved = (scroll_x_moved << 16) | (scroll_x_moved >> 16);
		y_speed_3 = LAYER(WORKING_LAYER)->y_speed;
		LAYER(WORKING_LAYER)->y_speed = y_speed_3 - y_speed;
		if (y_speed_3 == y_speed)
			LAYER(WORKING_LAYER)->y_direction = scroll_x_moved;
		LAYER(WORKING_LAYER)->y = y_max;
		LAYER(WORKING_LAYER)->row_phase = scroll_x_moved;
		direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
		if (direction_inputs == 5u)
		{
			if ((OBJECT(object)->contacts & 0x52u) != 0u)
				return;
			if (OBJECT(object)->routine == 3u)
				y_speed = y_speed >> 3;
			OBJECT(object)->y += y_speed;
			return;
		}
		if (direction_inputs != 6u)
		{
			if ((OBJECT(object)->contacts & 0x62u) != 0u)
				return;
			if (OBJECT(object)->routine == 3u)
				y_speed = y_speed >> 3;
			OBJECT(object)->y += y_speed;
			return;
		}
		if ((OBJECT(object)->contacts & 0x2Au) != 0u)
			return;
		if (OBJECT(object)->routine == 3u)
			y_speed = y_speed >> 3;
		OBJECT(object)->y += y_speed;
		return;
	}
	if (OBJECT(object)->y >= 0xA000u)
		return;
	turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
	y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
	direction_inputs = PLAYER_INPUT(inputs)->bits & 0xFu;
	if (direction_inputs == 5u)
	{
		if ((OBJECT(object)->contacts & 0x52u) != 0u)
			return;
		if (OBJECT(object)->routine == 3u)
			y_speed = y_speed >> 3;
		OBJECT(object)->y += y_speed;
		return;
	}
	if (direction_inputs != 6u)
	{
		if ((OBJECT(object)->contacts & 0x62u) != 0u)
			return;
		if (OBJECT(object)->routine == 3u)
			y_speed = y_speed >> 3;
		OBJECT(object)->y += y_speed;
		return;
	}
	if ((OBJECT(object)->contacts & 0x2Au) != 0u)
		return;
	if (OBJECT(object)->routine == 3u)
		y_speed = y_speed >> 3;
	OBJECT(object)->y += y_speed;
}


/* whether another turtle stands at the opposite edge, so the scroll cannot move away from it */
uint32_t turtle_other_at_edge(uint8_t edge, uint32_t turtle)
{
	uint32_t carry = 0;
	uint8_t turtles_left = 0;
	uint32_t object = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (!turtle_limits())
		return original_turtle_other_at_edge(edge, turtle);
	object = OBJECT_ADDRESS(0);
	turtles_left = 4u;
	screen_x_high = 4u;
	if ((uint8_t)edge != 0u)
	{
		if ((int8_t)edge < 0)
		{
			if ((edge & 0x7Fu) != 0u)
			{
				while (object == turtle || (OBJECT(object)->flags & 1u) == 0u || OBJECT(object)->height + OBJECT(object)->y >= 0x1100u)
				{
					t1 = turtles_left;
					carry = t1 == 0u;
					turtles_left = (uint8_t)(t1 - 1u);
					object += 0x50u;
					if (t1 == 1u)
						return carry;
				}
				carry = 1u;
				return carry;
			}
			do
			{
				if (object != turtle && (OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->height + OBJECT(object)->y >= 0xA000u)
				{
					carry = 1u;
					return carry;
				}
				t2 = turtles_left;
				carry = t2 == 0u;
				turtles_left = (uint8_t)(t2 - 1u);
				object += 0x50u;
			} while (t2 != 1u);
			return carry;
		}
		do
		{
			if (object != turtle && (OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x < (0x9100u - turtle_left))
			{
				carry = 1u;
				return carry;
			}
			t3 = turtles_left;
			carry = t3 == 0u;
			turtles_left = (uint8_t)(t3 - 1u);
			object += 0x50u;
		} while (t3 != 1u);
		return carry;
	}
	do
	{
		if (object != turtle && (OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x >= (0x16000u + turtle_right))
		{
			carry = 1u;
			return carry;
		}
		t4 = turtles_left;
		carry = t4 == 0u;
		turtles_left = (uint8_t)(t4 - 1u);
		object += 0x50u;
	} while (t4 != 1u);
	return carry;
}


/* the turtle walks off the screen */
void turtle_walk_off_screen(uint32_t object)
{
	uint32_t x_step = 0;

	if (!turtle_limits())
	{
		original_turtle_walk_off_screen(object);
		return;
	}
	x_step = 0x200u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
	if (OBJECT(object)->x >= (0x16000u + turtle_right) || OBJECT(object)->x < (0x9100u - turtle_left))
		OBJECT(object)->turtle.reaction_step = 0xFFu;
}


/* slides along the ground within the walls and the screen, farther for the hard knocks, until the pose
   ends */
void turtle_knocked_down_slide(uint32_t object)
{
	uint8_t wall_bit = 0;
	uint16_t x_step = 0;

	if (!turtle_limits())
	{
		original_turtle_knocked_down_slide(object);
		return;
	}
	turtle_drop_to_floor(object);
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_down_next(object);
		return;
	}
	x_step = 0u;
	if (OBJECT(object)->turtle.wait_timer != 0u)
	{
		OBJECT(object)->turtle.wait_timer--;
		x_step = 0x100u;
	}
	if (OBJECT(object)->turtle.reaction_variant >= 2u)
		x_step = 0x280u;
	wall_bit = 3u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
	{
		x_step = (uint16_t)(0u - x_step);
		wall_bit++;
	}
	if ((OBJECT(object)->contacts & shl32(1u, (int)(wall_bit))) != 0u)
		return;
	OBJECT(object)->x += (int16_t)x_step;
	if (OBJECT(object)->x < (0x16000u + turtle_right))
	{
		if (OBJECT(object)->x >= (0x9000u - turtle_left))
			return;
		OBJECT(object)->x = (0x9000u - turtle_left);
		return;
	}
	OBJECT(object)->x = (0x16000u + turtle_right);
}


/* slides along the ground for the hard knocks, within the walls and the screen, until the pose ends */
void turtle_knocked_back_slide(uint32_t object)
{
	uint8_t wall_bit = 0;
	uint16_t x_step = 0;

	if (!turtle_limits())
	{
		original_turtle_knocked_back_slide(object);
		return;
	}
	turtle_drop_to_floor(object);
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_back_next(object);
		return;
	}
	x_step = 0u;
	if (OBJECT(object)->turtle.reaction_variant >= 2u)
		x_step = 0x280u;
	wall_bit = 3u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
	{
		x_step = (uint16_t)(0u - x_step);
		wall_bit++;
	}
	if ((OBJECT(object)->contacts & shl32(1u, (int)(wall_bit))) != 0u)
		return;
	OBJECT(object)->x += (int16_t)x_step;
	if (OBJECT(object)->x < (0x16000u + turtle_right))
	{
		if (OBJECT(object)->x >= (0x9000u - turtle_left))
			return;
		OBJECT(object)->x = (0x9000u - turtle_left);
		return;
	}
	OBJECT(object)->x = (0x16000u + turtle_right);
}


/* after hitting a wall it drops down to the floor, else it slides a little; then a wait */
void turtle_knocked_flying_slide(uint32_t object)
{
	uint32_t x_step = 0;
	uint16_t height = 0;

	if (!turtle_limits())
	{
		original_turtle_knocked_flying_slide(object);
		return;
	}
	if (OBJECT(object)->turtle.reaction_variant == 2u || OBJECT(object)->turtle.reaction_variant == 4u)
	{
		if (OBJECT(object)->frame_time != 0u)
			return;
		if ((OBJECT(object)->contacts & 1u) != 0u)
		{
			turtle_knocked_flying_wait(object);
			return;
		}
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - 0x100u;
		if (height < 0x100u)
		{
			OBJECT(object)->height = 0u;
			OBJECT(object)->floor_height = 0u;
			turtle_knocked_flying_wait(object);
			return;
		}
		if (OBJECT(object)->floor_height <= OBJECT(object)->height_whole)
			return;
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		turtle_knocked_flying_wait(object);
		return;
	}
	if (OBJECT(object)->turtle.wait_timer == 0u)
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_wait(object);
			return;
		}
		return;
	}
	OBJECT(object)->turtle.wait_timer--;
	x_step = 0x100u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
	if (OBJECT(object)->x >= (0x16000u + turtle_right))
	{
		OBJECT(object)->x = (0x16000u + turtle_right);
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_wait(object);
			return;
		}
		return;
	}
	if (OBJECT(object)->x >= (0x9000u - turtle_left))
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_wait(object);
			return;
		}
		return;
	}
	OBJECT(object)->x = (0x9000u - turtle_left);
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_flying_wait(object);
		return;
	}
}


/* the turtle slides along the ground, or drops down a wall */
void turtle_knocked_flying_from_behind_slide(uint32_t object)
{
	uint32_t x_step = 0;
	uint16_t height = 0;

	if (!turtle_limits())
	{
		original_turtle_knocked_flying_from_behind_slide(object);
		return;
	}
	if (OBJECT(object)->turtle.reaction_variant == 1u || OBJECT(object)->turtle.reaction_variant == 4u)
	{
		if (OBJECT(object)->frame_time != 0u)
			return;
		if ((OBJECT(object)->contacts & 1u) != 0u)
		{
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - 0x100u;
		if (height < 0x100u)
		{
			OBJECT(object)->height = 0u;
			OBJECT(object)->floor_height = 0u;
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		if (OBJECT(object)->floor_height <= OBJECT(object)->height_whole)
			return;
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		turtle_knocked_flying_from_behind_wait(object);
		return;
	}
	if (OBJECT(object)->turtle.wait_timer == 0u)
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		return;
	}
	OBJECT(object)->turtle.wait_timer--;
	x_step = 0x100u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
	if (OBJECT(object)->x >= (0x16000u + turtle_right))
	{
		OBJECT(object)->x = (0x16000u + turtle_right);
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		return;
	}
	if (OBJECT(object)->x >= (0x9000u - turtle_left))
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		return;
	}
	OBJECT(object)->x = (0x9000u - turtle_left);
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_flying_from_behind_wait(object);
		return;
	}
}


/* the turtle on a knock's arc across and through the air, until it lands */
void turtle_knockback_arc(uint16_t jump_speed, uint32_t object)
{
	uint16_t jump_speed_2 = 0;
	uint16_t height = 0, jump_speed_3 = 0;

	if (!turtle_limits())
	{
		original_turtle_knockback_arc(jump_speed, object);
		return;
	}
	if (OBJECT(object)->turtle.falling != 0u && (OBJECT(object)->contacts & 1u) != 0u)
	{
		OBJECT(object)->contacts |= 1u;
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		return;
	}
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
	{
		if ((OBJECT(object)->contacts & 0x10u) == 0u)
		{
			OBJECT(object)->x -= OBJECT(object)->turtle.drift_speed;
			if (OBJECT(object)->x < (0x9000u - turtle_left))
				OBJECT(object)->x = (0x9000u - turtle_left);
			OBJECT(object)->turtle.drift_speed -= jump_speed;
			if (OBJECT(object)->turtle.drift_speed < 0x200u)
				OBJECT(object)->turtle.drift_speed = 0x200u;
		}
	}
	else if ((OBJECT(object)->contacts & 8u) == 0u)
	{
		OBJECT(object)->x += OBJECT(object)->turtle.drift_speed;
		if (OBJECT(object)->x < (0x16000u + turtle_right))
		{
			OBJECT(object)->turtle.drift_speed -= jump_speed;
			if (OBJECT(object)->turtle.drift_speed < 0x200u)
				OBJECT(object)->turtle.drift_speed = 0x200u;
		}
		else
		{
			OBJECT(object)->x = (0x16000u + turtle_right);
			OBJECT(object)->turtle.drift_speed -= jump_speed;
			if (OBJECT(object)->turtle.drift_speed < 0x200u)
				OBJECT(object)->turtle.drift_speed = 0x200u;
		}
	}
	if (OBJECT(object)->turtle.falling != 0u)
	{
		OBJECT(object)->turtle.jump_speed += jump_speed;
		jump_speed_2 = OBJECT(object)->turtle.jump_speed;
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - jump_speed_2;
		if (height < jump_speed_2 || OBJECT(object)->height_whole < OBJECT(object)->floor_height)
		{
			OBJECT(object)->contacts |= 1u;
			OBJECT(object)->height_whole = OBJECT(object)->floor_height;
			OBJECT(object)->height &= 0xFF00u;
			return;
		}
		return;
	}
	OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
	jump_speed_3 = OBJECT(object)->turtle.jump_speed;
	OBJECT(object)->turtle.jump_speed = jump_speed_3 - jump_speed;
	if (jump_speed <= jump_speed_3)
		return;
	OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
	OBJECT(object)->turtle.falling--;
}

