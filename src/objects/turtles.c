/**
 *    Name:        objects/turtles.c
 *    Description: the turtles: joining the game, the area's start and entry, and every turtle's frame: the controls, the moves and the scroll they push, the jumps, the attacks, the hits and the floor map's walls
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a player who has just put a coin in comes into the game with full health, blinking, with its marker */
void turtle_join(uint8_t players)
{
	uint32_t carry = 0;
	uint8_t by_entry = 0;
	uint16_t player = 0;
	uint32_t marker = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	object_loop_slot = OBJECT_ADDRESS(40);
	player = 0u;
	do
	{
		carry = players & 1u;
		players = players >> 1;
		if (carry)
		{
			by_entry = 0xFFu;
			turtle = turtle_init((uint8_t)player, by_entry, turtle);
			OBJECT(turtle)->health_bars = 0xAu;
			OBJECT(turtle)->health = 3u;
			marker_times(player) = 0x80u;
			marker = object_loop_slot;
			turtle_start_marker(player, &turtle, &marker);
			object_loop_slot = marker;
			if (entry_without_blink == 0u)
				OBJECT(turtle)->turtle.invulnerable_time |= 0x80u;
		}
		player = (uint8_t)(player + 1u);
		object_loop_slot += 0x50u;
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
}

/* a player whose turtle was knocked out comes back at the start point with full health */
void turtle_respawn(uint8_t players)
{
	uint32_t carry = 0;
	uint8_t by_entry = 0;
	uint16_t player = 0;
	uint32_t marker = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	object_loop_slot = OBJECT_ADDRESS(40);
	player = 0u;
	do
	{
		carry = players & 1u;
		players = players >> 1;
		if (carry)
		{
			by_entry = 0u;
			turtle = turtle_init((uint8_t)player, by_entry, turtle);
			OBJECT(turtle)->health_bars = 0xAu;
			OBJECT(turtle)->health = 3u;
			marker_times(player) = 0x80u;
			marker = object_loop_slot;
			turtle_start_marker(player, &turtle, &marker);
			object_loop_slot = marker;
			if (entry_without_blink == 0u)
				OBJECT(turtle)->turtle.invulnerable_time |= 0x80u;
		}
		player = (uint8_t)(player + 1u);
		object_loop_slot += 0x50u;
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
}

/* every player's turtle starts the area with the health bars it had */
void turtle_start_area(uint8_t starting_players)
{
	uint32_t carry = 0;
	uint8_t by_entry = 0;
	uint16_t player = 0;
	uint32_t marker = 0, player_health = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	object_loop_slot = OBJECT_ADDRESS(40);
	player_health = player_1_health_bars_ADDRESS;
	player = 0u;
	do
	{
		scratch = player_health;
		carry = starting_players & 1u;
		starting_players = starting_players >> 1;
		if (carry)
		{
			by_entry = 0xFFu;
			turtle = turtle_init((uint8_t)player, by_entry, turtle);
			player_health = scratch;
			if ((area_continues_scene & 0x80u) == 0u)
			{
				OBJECT(turtle)->health_bars = 0xAu;
				OBJECT(turtle)->health = 3u;
				PLAYER(player_health - 0x6u)->health_bars = 0xAu;
				PLAYER(player_health - 0x6u)->message = 3u;
			}
			marker_times(player) = 0x80u;
			marker = object_loop_slot;
			turtle_start_marker(player, &turtle, &marker);
			object_loop_slot = marker;
			if (entry_without_blink == 0u)
				OBJECT(turtle)->turtle.invulnerable_time |= 0x80u;
		}
		player = (uint8_t)(player + 1u);
		turtle += 0x50u;
		player_health = scratch + 8u;
		object_loop_slot += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	OBJECT(OBJECT_ADDRESS(0))->health_bars = PLAYER(PLAYER_ADDRESS(0))->health_bars;
	OBJECT(OBJECT_ADDRESS(1))->health_bars = PLAYER(PLAYER_ADDRESS(1))->health_bars;
	OBJECT(OBJECT_ADDRESS(2))->health_bars = PLAYER(PLAYER_ADDRESS(2))->health_bars;
	OBJECT(OBJECT_ADDRESS(3))->health_bars = PLAYER(PLAYER_ADDRESS(3))->health_bars;
}

/* a turtle's object set up for its player, invulnerable for a while, and placed by the area's entry */
uint32_t turtle_init(uint8_t kind, uint8_t by_entry, uint32_t turtle)
{
	uint16_t count = 0;
	uint32_t object = 0, stage_offset = 0, turtle_2 = 0;
	uint16_t t1 = 0;

	turtle_2 = turtle;
	stage_offset = 0u;
	count = 0x13u;
	do
	{
		mem_write32(turtle, stage_offset);
		t1 = count - 1u;
		count = t1;
		turtle += 4u;
	} while (t1 != 0xFFFFu);
	object = turtle_2;
	OBJECT(object)->turtle.prompt_time = 0x40u;
	area_entry_done = stage_offset;
	OBJECT(object)->kind = kind;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 1u;
	OBJECT(object)->turtle.invulnerable_time = 0x7Fu;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->colour = 0xFFu;
	OBJECT(object)->animation_delay = 0u;
	cutscene_step = 0u;
	if ((uint8_t)by_entry != 0u && scene == 4u && scene_step == 0u && scene_substep == 3u)
	{
		stage_offset = (uint16_t)((uint32_t)stage << 2);
		switch (stage_offset)
		{
		case 0:
		case 4:
		case 20:
		case 24:
		case 32:
		case 36:
			break;
		case 8:
		case 12:
		case 28:
			turtle_start_fall_in(stage_offset, object);
			return object;
		case 16:
			return turtle_jump_in_start(stage_offset, object);
		}
	}
	turtle_place_at_start(stage_offset, object);
	return object;
}

/* the turtle at its start point, standing */
void turtle_place_at_start(uint32_t kind_offset, uint32_t object)
{
	uint32_t places = 0, x = 0, y = 0;

	places = turtle_start_places_ADDRESS;
	if (stage == 3u)
		places = turtle_start_places_2_ADDRESS;
	kind_offset = (uint16_t)(IN_PAGE(kind_offset, OBJECT(object)->kind) << 2);
	x = (uint16_t)OFFSET_PAIR(places + ((int32_t)(kind_offset / 4u)) * 0x4u)->x;
	y = (uint16_t)OFFSET_PAIR(places + ((int32_t)(kind_offset / 4u)) * 0x4u)->y;
	OBJECT(object)->screen_x = x;
	OBJECT(object)->screen_y = y;
	OBJECT(object)->x = x << 8;
	OBJECT(object)->y = y << 8;
	turtle_set_animation(kind_offset, object);
}

/* the turtle starts the area falling in from above */
void turtle_start_fall_in(uint32_t kind_offset, uint32_t object)
{
	uint32_t places = 0, x = 0, y = 0;

	places = turtle_fall_in_places_ADDRESS;
	if (stage == 3u)
		places = turtle_fall_in_places_stage_3_ADDRESS;
	kind_offset = (uint16_t)(IN_PAGE(kind_offset, OBJECT(object)->kind) << 1);
	kind_offset = (uint16_t)((kind_offset << 1) + kind_offset);
	x = (uint16_t)FALL_IN_PLACE(places + kind_offset)->x;
	y = (uint16_t)FALL_IN_PLACE(places + kind_offset)->y;
	OBJECT(object)->height = FALL_IN_PLACE(places + kind_offset)->height;
	OBJECT(object)->screen_x = x;
	OBJECT(object)->screen_y = y;
	OBJECT(object)->x = x << 8;
	OBJECT(object)->y = y << 8;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 3u;
	turtle_set_animation(kind_offset, object);
}

/* the turtle starts area 4 jumping in, and the object that comes in with them */
uint32_t turtle_jump_in_start(uint32_t kind_offset, uint32_t object)
{
	uint16_t y = 0;
	uint32_t animation = 0, screen_x = 0, turtle = 0;

	kind_offset = (uint16_t)(IN_PAGE(kind_offset, OBJECT(object)->kind) << 1);
	kind_offset = (uint16_t)((kind_offset << 1) + kind_offset);
	screen_x = (uint16_t)turtle_jump_in_x[kind_offset / 2u];
	y = (uint16_t)turtle_jump_in_y[kind_offset / 2u];
	OBJECT(object)->turtle_jump_in.jump_in_speed = turtle_jump_in_speeds[kind_offset / 2u];
	OBJECT(object)->screen_x = screen_x;
	OBJECT(object)->screen_y = y;
	OBJECT(object)->x = screen_x << 8;
	OBJECT(object)->y = (uint32_t)(int16_t)y << 8;
	OBJECT(object)->turtle_jump_in.jump_speed = 0x800u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 1u;
	turtle_set_animation(kind_offset, object);
	turtle = object;
	object = OBJECT_ADDRESS(44);
	if ((OBJECT(OBJECT_ADDRESS(44))->flags & 1u) == 0u)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->screen_x = 0x60u;
		OBJECT(object)->screen_y = 0xFFC8u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->x_speed = 0x800;
		OBJECT(object)->y_speed &= 0xFFu;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		animation = turtle_jump_in_animation_ADDRESS;
		set_animation(kind_offset, object, animation);
	}
	object = turtle;
	return object;
}

/* the player's marker object starts at the turtle's place */
void turtle_start_marker(uint16_t player, uint32_t *p_object, uint32_t *p_marker)
{
	uint32_t object = *p_object;
	uint32_t marker = *p_marker;
	uint8_t longs_left = 0;
	uint32_t animation = 0, marker_2 = 0, marker_object = 0, object_2 = 0, player_offset = 0, zero_long = 0;
	uint8_t t1 = 0;

	marker_2 = marker;
	zero_long = 0u;
	longs_left = 0x14u;
	do
	{
		mem_write32(marker, zero_long);
		t1 = longs_left;
		longs_left = (uint8_t)(t1 - 1u);
		marker += 4u;
	} while (t1 != 1u);
	object_2 = marker_2;
	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->screen_x = OBJECT(object)->screen_x;
	OBJECT(object_2)->screen_y = OBJECT(object)->screen_y;
	OBJECT(object_2)->height = OBJECT(object)->height;
	OBJECT(object_2)->x = OBJECT(object)->x;
	OBJECT(object_2)->y = OBJECT(object)->y;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	player_offset = player;
	player_offset = (uint16_t)(player_offset << 2);
	animation = turtle_marker_animations[player_offset / 4u];
	saved_turtle = object;
	marker_object = object_2;
	player_offset = set_animation(player_offset, marker_object, animation);
	marker = marker_object;
	object = saved_turtle;
	screen_x_high = player_offset;
	*p_object = object;
	*p_marker = marker;
}

/* every frame of an area's start: the balloon timed out, the stage's entry, and the markers shown when
   it is over */
void run_area_entry(void)
{
	uint16_t count = 0;
	uint32_t object = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	if (balloon_time != 0u)
	{
		t1 = balloon_time;
		balloon_time = t1 - 1u;
		if (t1 == 1u)
			erase_balloon();
	}
	turtles_on_the_way = 0u;
	switch (stage)
	{
	case 0:
	case 1:
	case 5:
	case 6:
	case 8:
	case 9:
		area_entry_standing();
		break;
	case 2:
	case 3:
	case 7:
		area_entry_fall_in();
		break;
	case 4:
		area_entry_jump_in();
		break;
	}
	if (area_entry_done == 0u)
		return;
	object = OBJECT_ADDRESS(40);
	count = 3u;
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
			OBJECT(object)->flags = 0x81u;
		t2 = count - 1u;
		count = t2;
		object += 0x50u;
	} while (t2 != 0xFFFFu);
}

/* the areas the turtles start standing in: the layers drawn and the entry over at once */
void area_entry_standing(void)
{
	area_entry_done = 0xFFu;
	start_area_layers();
}

/* areas 2, 3 and 7: the turtles fall in, by the entry's step */
void area_entry_fall_in(void)
{
	uint16_t step_offset = 0;

	step_offset = (uint16_t)((uint32_t)cutscene_step << 2);
	switch (step_offset)
	{
	case 0:
		start_fall_in_entry();
		return;
	case 4:
		turtle_fall_in(step_offset);
		return;
	}
}

/* the layers drawn and, in area 7, the picture of the turtles falling in; then the fall */
void start_fall_in_entry(void)
{
	uint32_t animation = 0, object = 0;

	start_area_layers();
	if (stage == 7u)
	{
		object = OBJECT_ADDRESS(44);
		OBJECT(OBJECT_ADDRESS(44))->flags = 0x81u;
		OBJECT(OBJECT_ADDRESS(44))->contacts = 0u;
		OBJECT(OBJECT_ADDRESS(44))->height = 0u;
		OBJECT(OBJECT_ADDRESS(44))->colour = 0xFFu;
		OBJECT(OBJECT_ADDRESS(44))->animation_delay = 0u;
		OBJECT(OBJECT_ADDRESS(44))->screen_x = 0x40u;
		OBJECT(OBJECT_ADDRESS(44))->screen_y = 0x38u;
		animation = turtles_fall_in_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	cutscene_step++;
}

/* every frame of the entry: the turtles fall in across, land, wait and stand */
void turtle_fall_in(uint32_t on_right)
{
	uint8_t sound = 0;
	uint16_t screen_y = 0;
	uint32_t object = 0, screen_x = 0;
	uint8_t t1 = 0;
	uint16_t height = 0;

	if (stage != 7u || OBJECT(OBJECT_ADDRESS(44))->screen_x >= 0x120u)
	{
		object = OBJECT_ADDRESS(0);
		do
		{
			if ((OBJECT(object)->flags & 1u) != 0u)
			{
				if (OBJECT(object)->height == 0u)
				{
					if (OBJECT(object)->routine != 0u)
					{
						turtles_on_the_way--;
						t1 = (uint8_t)OBJECT(object)->y_speed;
						OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
						if (t1 == 1u)
						{
							OBJECT(object)->routine = 0u;
							turtle_set_animation(screen_x, object);
						}
					}
					else if (stage == 7u && balloons_said == 0u)
					{
						on_right &= ~0xFFu;
						screen_x = OBJECT(object)->screen_x;
						screen_y = OBJECT(object)->screen_y;
						on_right = say_we_gotta_save_splinter(on_right, (uint16_t)screen_x, screen_y);
					}
				}
				else
				{
					turtles_on_the_way--;
					OBJECT(object)->x += 0x300u;
					OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
					height = OBJECT(object)->height;
					OBJECT(object)->height = height - 0x600u;
					if (height < 0x600u)
					{
						OBJECT(object)->height = 0u;
						OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0x8u;
						OBJECT(object)->state = 0u;
						turtle_set_animation(screen_x, object);
					}
				}
			}
			object += 0x50u;
		} while (object < OBJECT_ADDRESS(4));
	}
	object = OBJECT_ADDRESS(44);
	if (OBJECT(OBJECT_ADDRESS(44))->flags == 0u)
	{
		if (turtles_on_the_way != 0u)
			return;
		area_entry_done = 0xFFu;
		return;
	}
	if ((frame_count & 0x1Fu) == 0u)
	{
		sound = 0x39u;
		queue_sound(sound);
	}
	OBJECT(object)->screen_x += 2u;
	if (OBJECT(object)->screen_x < 0x1C0u)
		return;
	OBJECT(object)->flags = 0u;
}

/* area 4: the turtles jump in, by the entry's step */
void area_entry_jump_in(void)
{
	switch (cutscene_step)
	{
	case 0:
		start_jump_in_entry();
		return;
	case 1:
		turtle_jump_in_step();
		return;
	}
}

/* the layers drawn, then the jump in */
void start_jump_in_entry(void)
{
	start_area_layers();
	cutscene_step++;
}

/* every frame of area 4's entry: the object that came with them bounces away, the turtles jump in, land
   and stand */
void turtle_jump_in_step(void)
{
	uint16_t screen_y = 0;
	uint32_t height = 0, kind = 0, object = 0;
	uint8_t t1 = 0;
	uint16_t x_speed = 0, height_2 = 0, x_speed_2 = 0;

	object = OBJECT_ADDRESS(44);
	if ((OBJECT(OBJECT_ADDRESS(44))->flags & 1u) != 0u)
	{
		OBJECT(object)->screen_x--;
		if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
		{
			OBJECT(object)->y -= OBJECT(object)->x_speed;
			OBJECT(object)->x_speed += 0x20u;
		}
		else
		{
			OBJECT(object)->y += OBJECT(object)->x_speed;
			x_speed = OBJECT(object)->x_speed;
			OBJECT(object)->x_speed = x_speed - 0x40u;
			if (x_speed < 0x40u)
			{
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
				OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
			}
		}
	}
	OBJECT(object)->screen_y = OBJECT(object)->y >> 8;
	if (OBJECT(object)->screen_x < 0x28u)
		OBJECT(object)->flags = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			if ((uint8_t)OBJECT(object)->y_speed == 0u)
			{
				if ((OBJECT(object)->routine | OBJECT(object)->state) != 0u)
				{
					turtles_on_the_way--;
					if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
					{
						OBJECT(object)->x_speed += 0x50u;
						height = (uint16_t)OBJECT(object)->x_speed;
						height_2 = OBJECT(object)->height;
						OBJECT(object)->height = height_2 - height;
						if (height_2 < height)
						{
							OBJECT(object)->height = 0u;
							OBJECT(object)->state = 0u;
							turtle_set_animation(height, object);
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0x8u;
						}
					}
					else
					{
						OBJECT(object)->height += OBJECT(object)->x_speed;
						if (OBJECT(object)->height >= 0x3800u && (int8_t)(uint8_t)(OBJECT(object)->y >> 24u) < 0)
						{
							OBJECT(object)->height -= 0x3800u;
							OBJECT(object)->y = 0u;
							OBJECT(object)->contacts |= 0x80u;
						}
						x_speed_2 = OBJECT(object)->x_speed;
						OBJECT(object)->x_speed = x_speed_2 - 0x20u;
						if (x_speed_2 < 0x20u)
						{
							kind = OBJECT(object)->kind;
							height = turtle_jump_in_heights[kind];
							OBJECT(object)->screen_y = height;
							height = (uint16_t)(height << 8);
							OBJECT(object)->height -= height;
							OBJECT(object)->y = height;
							OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
						}
					}
					OBJECT(object)->x += OBJECT(object)->height_speed;
					screen_y = (uint16_t)(OBJECT(object)->y >> 8);
					OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
					OBJECT(object)->screen_y = screen_y;
				}
			}
			else
			{
				turtles_on_the_way--;
				t1 = (uint8_t)OBJECT(object)->y_speed;
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
				if (t1 == 1u)
				{
					OBJECT(object)->routine = 0u;
					OBJECT(object)->state = 0u;
					turtle_set_animation(height, object);
				}
			}
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (turtles_on_the_way != 0u)
		return;
	area_entry_done = 0xFFu;
	OBJECT(OBJECT_ADDRESS(44))->flags = 0u;
}

/* on the skateboard the turtles' shadows change every four frames */
uint32_t turtle_skateboard_shadow(void)
{
	uint16_t shadow_offset = 0;
	uint32_t object = 0, shadows = 0;

	object = OBJECT_ADDRESS(0);
	shadows = skateboard_shadows_ADDRESS;
	do
	{
		shadow_offset = 0u;
		if ((frame_count & 4u) != 0u)
			shadow_offset = 4u;
		OBJECT(object)->shadow = SHADOW_ENTRY(shadows + shadow_offset);
		object += 0x50u;
		shadows += 8u;
	} while (object < OBJECT_ADDRESS(4));
	return object;
}

/* every frame of play: the markers kept on their turtles while they last, the bars kept, every turtle
   run */
void update_players(void)
{
	uint32_t marker_time = 0, object = 0, object_2 = 0;
	uint8_t frames = 0;
	uint32_t height_long = 0;

	object = OBJECT_ADDRESS(0);
	object_2 = OBJECT_ADDRESS(40);
	marker_time = marker_times_ADDRESS;
	do
	{
		if (MARKER_TIMER(marker_time)->frames != 0u)
		{
			OBJECT(object_2)->screen_x = OBJECT(object)->screen_x;
			OBJECT(object_2)->screen_y = OBJECT(object)->screen_y;
			height_long = (((uint32_t)OBJECT(object)->height << 16u) | (OBJECT(object)->x >> 16u));
			OBJECT(object_2)->height = (uint16_t)(height_long >> 16u);
			OBJECT(object_2)->x = (OBJECT(object_2)->x & 0xFFFFu) | ((height_long & 0xFFFFu) << 16u);
			frames = MARKER_TIMER(marker_time)->frames;
			MARKER_TIMER(marker_time)->frames = frames - 1u;
			if (frames == 1u)
				OBJECT(object_2)->flags = 0u;
		}
		marker_time++;
		object += 0x50u;
		object_2 += 0x50u;
	} while (marker_time < time_since_balloon_ADDRESS);
	PLAYER(PLAYER_ADDRESS(0))->health_bars = OBJECT(OBJECT_ADDRESS(0))->health_bars;
	PLAYER(PLAYER_ADDRESS(1))->health_bars = OBJECT(OBJECT_ADDRESS(1))->health_bars;
	PLAYER(PLAYER_ADDRESS(2))->health_bars = OBJECT(OBJECT_ADDRESS(2))->health_bars;
	PLAYER(PLAYER_ADDRESS(3))->health_bars = OBJECT(OBJECT_ADDRESS(3))->health_bars;
	if (stage_scroll_locks[stage] != 0u)
	{
		load_layer_b();
		turtle_run_all();
		save_layer_b();
		return;
	}
	load_layer_a();
	turtle_run_all();
	save_layer_a();
}

/* every turtle in play a frame: controls, move, the screen's limits, the scroll, floor and walls */
void CONSOLE_ORIGINAL(turtle_run_all)(void)
{
	uint8_t routine_past_walk = 0, segment_end_axis = 0;
	uint16_t scroll_stage = 0;
	uint32_t end = 0, inputs = 0, no_scroll = 0, object = 0, presses = 0, scroll_setting = 0, x = 0, x_2 = 0, x_speed = 0, y_speed = 0;
	uint8_t routine = 0, end_axis = 0;
	uint32_t y = 0;

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
				if (OBJECT(object)->x >= 0x9000u)
				{
					if (OBJECT(object)->x >= 0x16000u)
						OBJECT(object)->x = 0x16000u;
				}
				else
					OBJECT(object)->x = 0x9000u;
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

/* the turtle against solid objects, the floor map and its walls, and falling when nothing holds it */
void CONSOLE_ORIGINAL(turtle_collide)(uint32_t object, uint32_t inputs)
{
	uint32_t carry = 0;
	uint8_t forced_input = 0, raised_y = 0, sound = 0;
	uint32_t floor_cell_place = 0;
	uint8_t height_whole = 0;
	uint16_t height = 0;
	uint32_t y = 0;

	if (OBJECT(object)->routine == 4u)
	{
		forced_input = 2u;
		if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
			forced_input = 1u;
		PLAYER_INPUT(inputs)->bits = forced_input;
	}
	else if (OBJECT(object)->routine == 3u && (OBJECT(object)->state >= 9u || (OBJECT(object)->kind == 3u && OBJECT(object)->state == 7u)))
	{
		forced_input = 2u;
		if ((OBJECT(object)->flags & 0x40u) == 0u)
			PLAYER_INPUT(inputs)->bits = forced_input;
		else
		{
			forced_input = 1u;
			PLAYER_INPUT(inputs)->bits = forced_input;
		}
	}
	turtle_collide_with_objects(object);
	floor_cell_place = read_floor_cells(object);
	raised_y = (uint8_t)(OBJECT(object)->y >> 8u);
	height_whole = OBJECT(object)->height_whole;
	carry = (uint8_t)(raised_y + height_whole) < raised_y;
	raised_y += height_whole;
	if (carry || raised_y >= 0xF8u)
	{
		OBJECT(object)->contacts = 0x86u;
		if (stage != 5u && (OBJECT(object)->flags & 0x10u) == 0u && ((int8_t)floor_cell_right < 0 || (int8_t)floor_cell_left < 0))
			OBJECT(object)->contacts = 0xFEu;
	}
	else
		turtle_check_walls(object, inputs, floor_cell_place);
	if ((OBJECT(object)->contacts & 1u) != 0u || OBJECT(object)->routine == 2u || OBJECT(object)->routine == 4u)
		return;
	if (OBJECT(object)->height != 0u)
	{
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - 0x400u;
		if (height < 0x400u)
		{
			OBJECT(object)->height = 0u;
			sound = 0x12u;
			if (stage == 3u)
				sound = 0x30u;
			queue_sound(sound);
			return;
		}
		if (OBJECT(object)->floor_height <= OBJECT(object)->height_whole)
			return;
		if (OBJECT(object)->floor_height >= 0x60u)
			OBJECT(object)->floor_height = 0u;
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		if ((OBJECT(object)->contacts & 0x80u) == 0u)
			return;
		sound = 0x12u;
		if (stage != 3u || OBJECT(object)->height != 0u)
		{
			queue_sound(sound);
			return;
		}
		sound = 0x30u;
		queue_sound(sound);
		return;
	}
	y = OBJECT(object)->y;
	OBJECT(object)->y = y - 0x400u;
	if (y >= 0x400u)
		return;
	OBJECT(object)->height = 0u;
	sound = 0x12u;
	if (stage == 3u)
		sound = 0x30u;
	queue_sound(sound);
}

/* Unused: the floor map's cells around the object */
void jump_to_read_floor_cells(uint32_t object)
{
	read_floor_cells(object);
}

/* the walls around the turtle checked by the way its stick pushes */
void turtle_check_walls(uint32_t object, uint32_t inputs, uint32_t floor_cell_place)
{
	switch ((uint16_t)((PLAYER_INPUT(inputs)->bits & 0xFu) << 2))
	{
	case 0:
		turtle_walls_still(object, floor_cell_place);
		return;
	case 16:
		turtle_walls_up(object, floor_cell_place);
		return;
	case 12:
	case 32:
		turtle_walls_down(object, floor_cell_place);
		return;
	case 8:
		turtle_walls_right(object, floor_cell_place);
		return;
	case 4:
		turtle_walls_left(object, floor_cell_place);
		return;
	case 20:
		turtle_walls_up_left(object, floor_cell_place);
		return;
	case 24:
	case 28:
		turtle_walls_up_right(object, floor_cell_place);
		return;
	case 40:
	case 44:
	case 48:
	case 52:
	case 56:
	case 60:
		turtle_walls_down_right(object, floor_cell_place);
		return;
	case 36:
		turtle_walls_down_left(object, floor_cell_place);
		return;
	}
}

/* the stick moves the turtle across and up or down, or pushes the scroll at the screen's edges */
void CONSOLE_ORIGINAL(turtle_move)(uint32_t object, uint32_t inputs)
{
	bool L_01AE2C = false, L_01AEA8 = false, L_01AFD4 = false, L_01B012 = false, L_01B086 = false;
	uint32_t carry = 0;
	uint8_t direction_inputs = 0, edge = 0;
	uint16_t x_speed = 0;
	uint32_t new_column_x = 0, phase_step = 0, scroll_x_moved = 0, speed_offset = 0, walk_speeds = 0, x_max = 0, y_max = 0, y_speed = 0;
	uint8_t t1 = 0, t2 = 0;
	uint16_t column_phase = 0, x_speed_2 = 0, column_phase_2 = 0, column_phase_3 = 0, x_speed_3 = 0, column_phase_4 = 0, row_phase = 0, y_speed_2 = 0, row_phase_2 = 0, row_phase_3 = 0, row_phase_4 = 0, y_speed_3 = 0;

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
							if (!L_01AFD4 && OBJECT(object)->x < 0x12000u)
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
								if (OBJECT(object)->x >= 0x16000u)
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
							L_01AFD4 = OBJECT(object)->x >= 0x15800u;
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
			else if (stage == 8u || OBJECT(object)->x < 0x16000u)
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
				if ((uint8_t)LAYER(WORKING_LAYER)->x_direction != 0u || OBJECT(object)->x >= 0xE000u)
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
				else if (OBJECT(object)->x < 0x9100u)
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
			else if (OBJECT(object)->x >= 0x9000u)
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

/* the turtle at the left edge moves the scroll left at its walking speed */
void turtle_push_scroll_left(uint32_t object, uint32_t inputs, uint32_t *p_new_column_x, uint32_t *p_phase_step, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t phase_step = 0, x_speed = 0;
	uint32_t new_column_x = 0, speed_offset = 0, walk_speeds = 0, x_min = 0;

	LAYER(WORKING_LAYER)->x_direction = 0x7F;
	turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
	LAYER(WORKING_LAYER)->x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
	OBJECT(object)->turtle.scroll_push = 0x7Fu;
	x_speed = LAYER(WORKING_LAYER)->x_speed;
	phase_step = (uint16_t)((uint32_t)x_speed << 5);
	LAYER(WORKING_LAYER)->x -= x_speed;
	new_column_x = LAYER(WORKING_LAYER)->x;
	x_min = new_column_x;
	new_column_x -= 0x1800u;
	carry = x_min < LAYER(WORKING_LAYER)->x_min;
	*p_new_column_x = new_column_x;
	*p_phase_step = (*p_phase_step & 0xFFFF0000u) | phase_step;
	*p_carry = carry;
}

/* the turtle at the right edge moves the scroll right at its walking speed */
void turtle_push_scroll_right(uint32_t object, uint32_t inputs, uint32_t *p_new_column_x, uint32_t *p_phase_step, uint32_t *p_x_max, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t phase_step = 0, x_speed = 0;
	uint32_t speed_offset = 0, walk_speeds = 0;

	LAYER(WORKING_LAYER)->x_direction = -0x80;
	turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
	LAYER(WORKING_LAYER)->x_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset);
	OBJECT(object)->turtle.scroll_push = 0x7Fu;
	x_speed = LAYER(WORKING_LAYER)->x_speed;
	phase_step = (uint16_t)((uint32_t)x_speed << 5);
	LAYER(WORKING_LAYER)->x += x_speed;
	*p_new_column_x = LAYER(WORKING_LAYER)->x + 0x15800u;
	*p_x_max = LAYER(WORKING_LAYER)->x_max;
	carry = *p_x_max < LAYER(WORKING_LAYER)->x;
	*p_phase_step = (*p_phase_step & 0xFFFF0000u) | phase_step;
	*p_carry = carry;
}

/* the turtle at the top moves the scroll up at its walking speed */
void turtle_push_scroll_up(uint32_t object, uint32_t inputs, uint32_t *p_scroll_y, uint32_t *p_phase_step, uint32_t *p_y_max, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t phase_step = 0, y_speed = 0;
	uint32_t speed_offset = 0, walk_speeds = 0;

	LAYER(WORKING_LAYER)->y_direction = 0x7F;
	OBJECT(object)->turtle.scroll_push |= 0x80u;
	turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
	y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
	if (OBJECT(object)->routine == 3u)
		y_speed = y_speed >> 3;
	LAYER(WORKING_LAYER)->y_speed = y_speed;
	phase_step = (uint16_t)((uint32_t)y_speed << 5);
	LAYER(WORKING_LAYER)->y += y_speed;
	*p_scroll_y = LAYER(WORKING_LAYER)->y;
	*p_y_max = LAYER(WORKING_LAYER)->y_max;
	carry = *p_y_max < *p_scroll_y;
	*p_phase_step = (*p_phase_step & 0xFFFF0000u) | phase_step;
	*p_carry = carry;
}

/* on area 2's diagonal stretch, the scroll pushed right too, unless a turtle is at the left edge or it
   moves already */
void push_scroll_diagonally(uint32_t turtle)
{
	uint32_t carry = 0;
	uint8_t edge = 0;
	uint32_t new_column_x = 0;
	uint16_t column_phase = 0;

	edge = 0x7Fu;
	carry = turtle_other_at_edge(edge, turtle);
	if (carry == 0u && (uint8_t)LAYER(WORKING_LAYER)->x_direction == 0u)
	{
		LAYER(WORKING_LAYER)->x_direction = -0x80;
		LAYER(WORKING_LAYER)->x_speed = 0x120u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x2400u;
		LAYER(WORKING_LAYER)->x += 0x120u;
		new_column_x = LAYER(WORKING_LAYER)->x;
		column_phase = LAYER(WORKING_LAYER)->column_phase;
		carry = (uint16_t)(column_phase + 0x2400u) < column_phase;
		LAYER(WORKING_LAYER)->column_phase = column_phase + 0x2400u;
		new_column_x += 0x15800u;
		if (carry)
		{
			LAYER(WORKING_LAYER)->new_column_x = new_column_x;
			LAYER(WORKING_LAYER)->new_column--;
		}
	}
}

/* a new row of the layers to draw; on the diagonal stretch the scroll pushed right too */
void queue_scroll_row(uint32_t new_row_y, uint32_t turtle)
{
	LAYER(WORKING_LAYER)->new_row_y = new_row_y;
	LAYER(WORKING_LAYER)->new_row--;
	if ((uint8_t)diagonal_scroll != 0u)
	{
		push_scroll_diagonally(turtle);
		return;
	}
}

/* the turtle at the bottom moves the scroll down at its walking speed */
void turtle_push_scroll_down(uint32_t object, uint32_t inputs, uint32_t *p_new_row_y, uint32_t *p_phase_step, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t phase_step = 0, y_speed = 0;
	uint32_t new_row_y = 0, speed_offset = 0, walk_speeds = 0, y_min = 0;

	LAYER(WORKING_LAYER)->y_direction = -0x80;
	OBJECT(object)->turtle.scroll_push |= 0x80u;
	turtle_walk_speed(object, inputs, &speed_offset, &walk_speeds);
	y_speed = WALK_SPEED(walk_speeds + (uint16_t)speed_offset + 0x4u);
	if (OBJECT(object)->routine == 3u)
		y_speed = y_speed >> 3;
	LAYER(WORKING_LAYER)->y_speed = y_speed;
	phase_step = (uint16_t)((uint32_t)y_speed << 5);
	LAYER(WORKING_LAYER)->y -= y_speed;
	new_row_y = LAYER(WORKING_LAYER)->y;
	y_min = new_row_y;
	new_row_y -= 0xF800u;
	carry = y_min < LAYER(WORKING_LAYER)->y_min;
	*p_new_row_y = new_row_y;
	*p_phase_step = (*p_phase_step & 0xFFFF0000u) | phase_step;
	*p_carry = carry;
}

/* the turtle's walking speeds, straight or on a diagonal */
void CONSOLE_ORIGINAL(turtle_walk_speed)(uint32_t object, uint32_t inputs, uint32_t *p_speed_offset, uint32_t *p_walk_speeds)
{
	uint8_t direction_copy = 0, direction_inputs = 0;
	uint16_t diagonal_offset = 0, speed_offset = 0;

	diagonal_offset = 0u;
	direction_inputs = PLAYER_INPUT(inputs)->bits;
	direction_copy = (uint8_t)direction_inputs;
	if ((direction_inputs & 3u) != 0u && (direction_copy & 0xCu) != 0u)
		diagonal_offset = 2u;
	*p_walk_speeds = turtle_walk_speeds_ADDRESS;
	speed_offset = (uint16_t)(((OBJECT(object)->kind & 3u) << 3) + diagonal_offset);
	*p_speed_offset = (*p_speed_offset & 0xFFFF0000u) | speed_offset;
}

/* whether another turtle stands at the opposite edge, so the scroll cannot move away from it */
uint32_t CONSOLE_ORIGINAL(turtle_other_at_edge)(uint8_t edge, uint32_t turtle)
{
	uint32_t carry = 0;
	uint8_t turtles_left = 0;
	uint32_t object = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

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
			if (object != turtle && (OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x < 0x9100u)
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
		if (object != turtle && (OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x >= 0x16000u)
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

/* every frame: the time it cannot be hit, the blinking, and its routine (stand, walk, jump, attack, hit) */
void turtle_control(uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint32_t carry = 0;
	uint8_t held = 0, pressed = 0;
	uint8_t t1 = 0, attack_press_time = 0;

	OBJECT(object)->flags &= ~2u;
	if (OBJECT(object)->turtle.invulnerable_time == 0u)
	{
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->flags |= 0x80u;
	}
	else
	{
		t1 = OBJECT(object)->turtle.invulnerable_time - 1u;
		OBJECT(object)->turtle.invulnerable_time = t1;
		if ((int8_t)t1 >= 0)
		{
			OBJECT(object)->flags |= 0x80u;
			held = PLAYER_INPUT(inputs)->bits;
			pressed = PLAYER_INPUT(presses)->bits;
			if ((pressed & 0x20u) != 0u)
				OBJECT(object)->turtle.attack_press_time = 0u;
			attack_press_time = OBJECT(object)->turtle.attack_press_time;
			carry = (uint8_t)(attack_press_time + 1u) < attack_press_time;
			OBJECT(object)->turtle.attack_press_time = attack_press_time + 1u;
			if (carry)
				OBJECT(object)->turtle.attack_press_time--;
			switch (OBJECT(object)->routine)
			{
			case 0:
				turtle_stand(held, pressed, object, inputs, presses);
				return;
			case 1:
				turtle_walk(held, pressed, object, inputs, presses);
				return;
			case 2:
				turtle_jump(held, object, inputs, presses);
				return;
			case 3:
				turtle_attack(held, pressed, object, inputs, presses);
				return;
			case 4:
				turtle_hit(object, inputs, presses);
				return;
			case 5:
				turtle_fire_gun(held, pressed, object, inputs, presses);
				return;
			}
		}
		else if (OBJECT(object)->turtle.invulnerable_time != 0x80u)
		{
			if ((frame_count & 1u) == 0u)
				OBJECT(object)->flags ^= 0x80u;
		}
		else
		{
			OBJECT(object)->turtle.invulnerable_time = 0u;
			OBJECT(object)->flags |= 2u;
			OBJECT(object)->flags |= 0x80u;
		}
	}
	held = PLAYER_INPUT(inputs)->bits;
	pressed = PLAYER_INPUT(presses)->bits;
	if ((pressed & 0x20u) != 0u)
		OBJECT(object)->turtle.attack_press_time = 0u;
	attack_press_time = OBJECT(object)->turtle.attack_press_time;
	carry = (uint8_t)(attack_press_time + 1u) < attack_press_time;
	OBJECT(object)->turtle.attack_press_time = attack_press_time + 1u;
	if (carry)
		OBJECT(object)->turtle.attack_press_time--;
	switch (OBJECT(object)->routine)
	{
	case 0:
		turtle_stand(held, pressed, object, inputs, presses);
		return;
	case 1:
		turtle_walk(held, pressed, object, inputs, presses);
		return;
	case 2:
		turtle_jump(held, object, inputs, presses);
		return;
	case 3:
		turtle_attack(held, pressed, object, inputs, presses);
		return;
	case 4:
		turtle_hit(object, inputs, presses);
		return;
	case 5:
		turtle_fire_gun(held, pressed, object, inputs, presses);
		return;
	}
}

/* the turtle starts walking, across or up, or stands on the skateboard */
void turtle_start_walk(uint8_t stick, uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	if (stage == 6u)
	{
		OBJECT(object)->flags &= ~0xCu;
		routine_state = 0u;
		if (OBJECT(object)->turtle.has_gun == 0u)
		{
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		routine_state = 4u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->flags &= ~0xCu;
	routine_state = 0x100u;
	if (OBJECT(object)->turtle.has_gun != 0u)
		routine_state += 2u;
	if ((stick & 4u) == 0u)
	{
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	routine_state++;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* the jump button starts a jump, or the special attack with the attack button too */
void CONSOLE_ORIGINAL(turtle_start_jump)(uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t move = 0;
	uint16_t routine_state = 0;
	uint32_t held = 0, ignored = 0, ignored_2 = 0;

	if ((PLAYER_INPUT(presses)->bits & 0x20u) != 0u)
	{
		OBJECT(object)->flags &= ~8u;
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->turtle.combo_step = 0u;
		OBJECT(object)->turtle.combo = 6u;
		move = 7u;
		if (OBJECT(object)->kind == 3u)
		{
			turtle_start_attack(move, object);
			return;
		}
		move = 8u;
		OBJECT(object)->turtle.jump_speed = 0x900u;
		OBJECT(object)->turtle.falling = 0u;
		OBJECT(object)->turtle.move_timer = 0u;
		turtle_start_attack(move, object);
		return;
	}
	turtles_busy--;
	OBJECT(object)->flags &= ~0xCu;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->turtle.jump_speed = 0x500u;
	OBJECT(object)->turtle.falling = 0u;
	OBJECT(object)->turtle.move_timer = 0x10u;
	OBJECT(object)->turtle.jump_flip = 0u;
	OBJECT(object)->height += 0x500u;
	routine_state = 0x201u;
	turtle_set_state(routine_state, object, &held, &ignored, &ignored_2);
	turtle_jump((uint8_t)held, object, inputs, presses);
}

/* the attack button starts a combo at random, or the attack behind when an enemy is there */
void CONSOLE_ORIGINAL(turtle_start_combo)(uint32_t object, uint32_t inputs)
{
	uint8_t attack = 0, combo_step = 0, move = 0;
	uint16_t routine_state = 0;
	uint32_t combo_table = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, range = 0;
	uint8_t t1 = 0;

	t1 = PLAYER_INPUT(inputs)->bits & 3u;
	PLAYER_INPUT(inputs)->bits = t1;
	if (t1 != 0u)
	{
		if ((PLAYER_INPUT(inputs)->bits & 1u) != 0u)
			OBJECT(object)->flags |= 0x40u;
		else
			OBJECT(object)->flags &= 0xBFu;
	}
	PLAYER_INPUT(inputs)->bits = 0u;
	if (OBJECT(object)->turtle.has_gun != 0u)
	{
		OBJECT(object)->flags &= ~8u;
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->turtle.combo_step = 0u;
		routine_state = 0x500u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	range = 4u;
	range = random_below((uint16_t)range, object);
	range = range & 3u;
	range = (uint16_t)(range + (range << 1));
	OBJECT(object)->turtle.combo = range;
	combo_table = turtle_combos[OBJECT(object)->kind & 3u];
	if (OBJECT(object)->turtle.enemy_behind == 0u)
	{
		combo_step = COMBO_STEP(combo_table + (int32_t)range);
		turtle_start_attack(combo_step, object);
		return;
	}
	OBJECT(object)->turtle.combo_step = 1u;
	OBJECT(object)->turtle.combo += 0xCu;
	move = 3u;
	attack = 3u;
	turtle_set_attack(move, attack, object);
}

/* an attack from the first step of the combo */
void turtle_start_attack(uint8_t move, uint32_t object)
{
	uint8_t attack = 0;

	attack = (uint8_t)move;
	OBJECT(object)->turtle.combo_step = 0u;
	turtle_set_attack(move, attack, object);
}

/* the attack's kind, sound and damage, and the turtle attacking */
void CONSOLE_ORIGINAL(turtle_set_attack)(uint16_t move, uint16_t attack, uint32_t object)
{
	uint8_t sound = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	OBJECT(object)->flags &= ~8u;
	OBJECT(object)->flags |= 4u;
	attack = (uint8_t)(attack + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		attack = attack | 0x80u;
	if (move == 3u)
		attack = attack ^ 0x80u;
	OBJECT(object)->attack = (uint8_t)attack;
	attack &= 0x7Fu;
	sound = turtle_attack_sounds_by_move[attack];
	queue_sound(sound);
	OBJECT(object)->damage = turtle_attack_damages[attack & 0x7FFFu];
	move = (uint16_t)((move & 0x7Fu) + 0x300u);
	turtle_set_state(move, object, &ignored, &ignored_2, &ignored_3);
}

/* a new routine and state for the turtle with its animation */
void turtle_set_state(uint16_t routine_state, uint32_t object, uint32_t *p_player_offset, uint32_t *p_bank, uint32_t *p_fourth_row_tile)
{
	uint32_t player_offset = *p_player_offset;
	uint32_t fourth_row_tile = *p_fourth_row_tile;
	uint8_t sound = 0;
	uint16_t player = 0, screen_y = 0;

	if (routine_state == 0x411u)
	{
		sound = 0x26u;
		queue_sound(sound);
		player_offset = (player_offset & ~0xFFFFu) | OBJECT(object)->kind;
		player = (uint16_t)player_offset;
		player_offset = (player_offset & ~0xFFFFu) | (uint16_t)(player_offset << 3);
		screen_x_high = (screen_x_high & ~0xFFFFu) | (player_offset & 0xFFFFu);
		if (RAM16(player_1_lives_ADDRESS + (uint16_t)player_offset) == 0u)
		{
			routine_state &= ~0xFFu;
			prompt_shown(player) = (uint8_t)routine_state;
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				routine_state |= (uint8_t)(routine_state - 1u);
			player_offset = (player_offset & ~0xFFFFu) | OBJECT(object)->screen_x;
			screen_y = (uint16_t)(OBJECT(object)->screen_y - 0x10u);
			say_shell_shock(routine_state, (uint16_t)player_offset, screen_y, object, p_bank, &fourth_row_tile);
			routine_state = 0x411u;
		}
	}
	OBJECT(object)->turtle.idle_time = 0u;
	OBJECT(object)->routine = (uint8_t)(routine_state >> 8u);
	OBJECT(object)->state = (uint8_t)routine_state;
	player_offset = turtle_set_animation(player_offset, object);
	*p_player_offset = player_offset;
	*p_fourth_row_tile = (*p_fourth_row_tile & 0xFFFF0000u) | (fourth_row_tile & 0xFFFFu);
}

/* the turtle's animation for its routine and state */
uint32_t CONSOLE_ORIGINAL(turtle_set_animation)(uint32_t held, uint32_t object)
{
	uint16_t table_offset = 0;
	uint32_t animation_entry = 0, animation_set = 0;

	table_offset = (uint16_t)((uint32_t)OBJECT(object)->kind << 2);
	animation_set = turtle_animation_sets[table_offset / 4u];
	table_offset = (uint16_t)(IN_PAGE(table_offset, OBJECT(object)->routine) << 2);
	animation_entry = ANIMATION_ENTRY(ANIMATION_TABLE_ENTRY(animation_set + (table_offset / 4u) * 0x4u) + (IN_PAGE(table_offset, OBJECT(object)->state) & 0x3FFFu) * 0x4u);
	return set_animation(held, object, animation_entry);
}

/* standing: jump, attack, walk, or the idle animation */
void turtle_stand(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t buttons_pressed = 0;
	uint32_t stick = 0;
	uint8_t t1 = 0, t2 = 0;

	OBJECT(object)->flags &= ~0xCu;
	buttons_pressed = (uint8_t)pressed;
	if (area_beaten == 0u)
	{
		if ((OBJECT(object)->contacts & 1u) != 0u && (pressed & 0x10u) != 0u)
		{
			turtle_start_jump(object, inputs, presses);
			return;
		}
		if ((buttons_pressed & 0x20u) != 0u)
		{
			turtle_start_combo(object, inputs);
			return;
		}
	}
	t1 = held & 0xFu;
	stick = t1;
	if (t1 != 0u)
	{
		turtle_start_walk((uint8_t)stick, object);
		return;
	}
	if (OBJECT(object)->turtle.has_gun != 0u || OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->turtle.idle_time++;
	if (OBJECT(object)->turtle.idle_time < 0x18u)
		return;
	OBJECT(object)->turtle.idle_time = 0u;
	OBJECT(object)->state++;
	t2 = OBJECT(object)->state & 3u;
	OBJECT(object)->state = t2;
	if (t2 != 0u)
	{
		turtle_set_animation(stick, object);
		return;
	}
	OBJECT(object)->state = 1u;
	turtle_set_animation(stick, object);
}

/* walking: jump, attack, stand, or across and up by the stick */
void turtle_walk(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t buttons_pressed = 0, stick = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t t1 = 0;

	OBJECT(object)->flags &= ~0xCu;
	buttons_pressed = (uint8_t)pressed;
	if (area_beaten == 0u)
	{
		if ((OBJECT(object)->contacts & 1u) != 0u && (pressed & 0x10u) != 0u)
		{
			turtle_start_jump(object, inputs, presses);
			return;
		}
		if ((buttons_pressed & 0x20u) != 0u)
		{
			turtle_start_combo(object, inputs);
			return;
		}
	}
	routine_state = 0x100u;
	if (OBJECT(object)->turtle.has_gun != 0u)
		routine_state += 2u;
	t1 = held & 0xFu;
	stick = t1;
	if (t1 == 0u)
	{
		OBJECT(object)->flags &= ~0xCu;
		routine_state = 0u;
		if (OBJECT(object)->turtle.has_gun == 0u)
		{
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		routine_state = 4u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if ((stick & 4u) != 0u)
	{
		routine_state++;
		if ((OBJECT(object)->state & 1u) == 0u)
		{
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		return;
	}
	if ((OBJECT(object)->state & 1u) != 0u)
	{
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
}

/* in the air: a jump attack by its height, or the jump's state */
void CONSOLE_ORIGINAL(turtle_jump)(uint8_t held, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t attack_height = 0, height_above_floor = 0, move = 0;

	turtles_busy--;
	OBJECT(object)->flags &= ~0xCu;
	if ((PLAYER_INPUT(presses)->bits & 0x20u) != 0u)
	{
		attack_height = 0x38u;
		if (OBJECT(object)->kind == 3u)
			attack_height = 0x20u;
		OBJECT(object)->flags |= 4u;
		height_above_floor = (uint8_t)(OBJECT(object)->height_whole - OBJECT(object)->floor_height);
		OBJECT(object)->turtle.falling = 0u;
		OBJECT(object)->turtle.jump_speed = 0x500u;
		move = 0xAu;
		if (height_above_floor < 0x60u)
		{
			move--;
			if (height_above_floor < attack_height)
			{
				OBJECT(object)->turtle.jump_speed = 0x800u;
				move += 2u;
				OBJECT(object)->turtle.move_timer = 0u;
				turtle_start_attack(move, object);
				return;
			}
		}
		turtle_start_attack(move, object);
		return;
	}
	switch (OBJECT(object)->state)
	{
	case 0:
	case 1:
		turtle_jump_rise(object, inputs);
		return;
	case 2:
		turtle_jump_top(held, object, inputs);
		return;
	case 3:
		turtle_jump_fall(object);
		return;
	case 4:
		turtle_wall_jump(object, inputs);
		return;
	}
}

/* the jump going up, higher while the button is held */
void turtle_jump_rise(uint32_t object, uint32_t inputs)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t move_timer = 0;

	if (OBJECT(object)->turtle.move_timer == 0u)
		OBJECT(object)->turtle.jump_speed -= 0x50u;
	else
	{
		move_timer = OBJECT(object)->turtle.move_timer;
		OBJECT(object)->turtle.move_timer = move_timer - 1u;
		if (move_timer == 1u)
		{
			OBJECT(object)->turtle.jump_speed -= 0x50u;
			OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
			if (OBJECT(object)->turtle.jump_speed >= 0x400u)
				return;
			routine_state = 0x202u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		if ((PLAYER_INPUT(inputs)->bits & 0x10u) == 0u)
			OBJECT(object)->turtle.move_timer = 0u;
	}
	OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
	if (OBJECT(object)->turtle.jump_speed >= 0x400u)
		return;
	routine_state = 0x202u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* the top of the jump: the somersault, or kicking off a wall it is pushed against */
void turtle_jump_top(uint32_t held, uint32_t object, uint32_t inputs)
{
	uint16_t jump_speed = 0, routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t move_timer = 0;
	uint16_t height = 0, jump_speed_2 = 0;

	if (OBJECT(object)->turtle.jump_flip != 0u && OBJECT(object)->frame_time == 0u)
	{
		routine_state = 0x203u;
		turtle_set_state(routine_state, object, &held, &ignored, &ignored_2);
	}
	if (OBJECT(object)->turtle.falling != 0u)
	{
		if (OBJECT(object)->height + OBJECT(object)->y < 0xF400u && (uint8_t)(OBJECT(object)->height_whole - OBJECT(object)->floor_height) >= 0x40u)
		{
			if ((PLAYER_INPUT(inputs)->bits & 1u) != 0u)
			{
				if ((OBJECT(object)->contacts & 0x10u) != 0u)
				{
					OBJECT(object)->turtle.jump_flip = 0u;
					PLAYER_INPUT(inputs)->bits = 0u;
					OBJECT(object)->turtle.jump_speed = 0u;
					routine_state = 0x205u;
					turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
					OBJECT(object)->turtle.move_timer = 8u;
					OBJECT(object)->state = 4u;
					return;
				}
			}
			else if ((PLAYER_INPUT(inputs)->bits & 2u) != 0u && (OBJECT(object)->contacts & 8u) != 0u)
			{
				OBJECT(object)->turtle.jump_flip = 0xFFu;
				PLAYER_INPUT(inputs)->bits = 0u;
				OBJECT(object)->turtle.jump_speed = 0u;
				routine_state = 0x205u;
				turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
				OBJECT(object)->turtle.move_timer = 8u;
				OBJECT(object)->state = 4u;
				return;
			}
		}
		if ((OBJECT(object)->contacts & 1u) != 0u)
		{
			OBJECT(object)->height_whole = OBJECT(object)->floor_height;
			OBJECT(object)->height &= 0xFF00u;
			turtle_land(object);
			return;
		}
		OBJECT(object)->turtle.jump_speed += 0x50u;
		if (OBJECT(object)->turtle.jump_speed >= 0x500u)
			OBJECT(object)->turtle.jump_speed = 0x500u;
		jump_speed = OBJECT(object)->turtle.jump_speed;
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - jump_speed;
		if (height < jump_speed)
		{
			turtle_land_on_ground(object);
			return;
		}
		return;
	}
	if (OBJECT(object)->height + OBJECT(object)->y < 0xF400u && (uint8_t)(OBJECT(object)->height_whole - OBJECT(object)->floor_height) >= 0x40u)
	{
		if ((PLAYER_INPUT(inputs)->bits & 1u) != 0u)
		{
			if ((OBJECT(object)->contacts & 0x10u) != 0u)
			{
				OBJECT(object)->turtle.jump_flip = 0u;
				PLAYER_INPUT(inputs)->bits = 0u;
				OBJECT(object)->turtle.jump_speed = 0u;
				routine_state = 0x205u;
				turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
				OBJECT(object)->turtle.move_timer = 8u;
				OBJECT(object)->state = 4u;
				return;
			}
		}
		else if ((PLAYER_INPUT(inputs)->bits & 2u) != 0u && (OBJECT(object)->contacts & 8u) != 0u)
		{
			OBJECT(object)->turtle.jump_flip = 0xFFu;
			PLAYER_INPUT(inputs)->bits = 0u;
			OBJECT(object)->turtle.jump_speed = 0u;
			routine_state = 0x205u;
			turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
			OBJECT(object)->turtle.move_timer = 8u;
			OBJECT(object)->state = 4u;
			return;
		}
	}
	if (OBJECT(object)->turtle.move_timer == 0u)
	{
		jump_speed_2 = OBJECT(object)->turtle.jump_speed;
		OBJECT(object)->turtle.jump_speed = jump_speed_2 - 0x50u;
		if (jump_speed_2 >= 0x50u)
		{
			OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
			if (OBJECT(object)->turtle.jump_speed >= 0x380u || OBJECT(object)->turtle.jump_flip != 0u)
				return;
			OBJECT(object)->turtle.jump_flip--;
			routine_state = 0x204u;
			turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
			OBJECT(object)->state = 2u;
			return;
		}
		OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
		OBJECT(object)->turtle.falling = ~OBJECT(object)->turtle.falling;
		return;
	}
	move_timer = OBJECT(object)->turtle.move_timer;
	OBJECT(object)->turtle.move_timer = move_timer - 1u;
	if (move_timer == 1u)
	{
		jump_speed_2 = OBJECT(object)->turtle.jump_speed;
		OBJECT(object)->turtle.jump_speed = jump_speed_2 - 0x50u;
		if (jump_speed_2 >= 0x50u)
		{
			OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
			if (OBJECT(object)->turtle.jump_speed >= 0x380u || OBJECT(object)->turtle.jump_flip != 0u)
				return;
			OBJECT(object)->turtle.jump_flip--;
			routine_state = 0x204u;
			turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
			OBJECT(object)->state = 2u;
			return;
		}
		OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
		OBJECT(object)->turtle.falling = ~OBJECT(object)->turtle.falling;
		return;
	}
	if ((held & 0x10u) != 0u)
	{
		OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
		if (OBJECT(object)->turtle.jump_speed >= 0x380u || OBJECT(object)->turtle.jump_flip != 0u)
			return;
		OBJECT(object)->turtle.jump_flip--;
		routine_state = 0x204u;
		turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
		OBJECT(object)->state = 2u;
		return;
	}
	OBJECT(object)->turtle.move_timer = 0u;
	OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
	if (OBJECT(object)->turtle.jump_speed >= 0x380u || OBJECT(object)->turtle.jump_flip != 0u)
		return;
	OBJECT(object)->turtle.jump_flip--;
	routine_state = 0x204u;
	turtle_set_state(routine_state, object, &ignored_3, &ignored, &ignored_2);
	OBJECT(object)->state = 2u;
}

/* the jump coming down until it lands */
void turtle_jump_fall(uint32_t object)
{
	uint16_t jump_speed = 0;
	uint16_t height = 0;

	if ((OBJECT(object)->contacts & 1u) != 0u)
	{
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		turtle_land(object);
		return;
	}
	OBJECT(object)->turtle.jump_speed += 0x50u;
	if (OBJECT(object)->turtle.jump_speed >= 0x500u)
		OBJECT(object)->turtle.jump_speed = 0x500u;
	jump_speed = OBJECT(object)->turtle.jump_speed;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - jump_speed;
	if (jump_speed <= height)
		return;
	turtle_land_on_ground(object);
}

/* the turtle lands on the ground */
void turtle_land_on_ground(uint32_t object)
{
	OBJECT(object)->height = 0u;
	turtle_land(object);
}

/* the turtle lands and stands, with the landing's sound */
void turtle_land(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	OBJECT(object)->contacts &= 0x7Fu;
	routine_state = 0u;
	if (OBJECT(object)->turtle.has_gun != 0u)
		routine_state = 4u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
	sound = 0x12u;
	if (stage == 3u && OBJECT(object)->height == 0u)
		sound = 0x30u;
	queue_sound(sound);
}

/* the turtle on a wall for a moment, then away from it falling */
void turtle_wall_jump(uint32_t object, uint32_t inputs)
{
	uint16_t jump_speed = 0;
	uint16_t height = 0;

	PLAYER_INPUT(inputs)->bits = 0u;
	if (OBJECT(object)->turtle.move_timer != 0u)
	{
		OBJECT(object)->turtle.move_timer--;
		return;
	}
	if ((OBJECT(object)->contacts & 1u) != 0u)
	{
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		turtle_land(object);
		return;
	}
	if (OBJECT(object)->turtle.jump_flip != 0u)
	{
		if ((OBJECT(object)->contacts & 0x10u) == 0u)
			OBJECT(object)->x -= 0x300u;
	}
	else if ((OBJECT(object)->contacts & 8u) == 0u)
		OBJECT(object)->x += 0x300u;
	OBJECT(object)->turtle.jump_speed += 0x28u;
	jump_speed = OBJECT(object)->turtle.jump_speed;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - jump_speed;
	if (height < jump_speed)
	{
		turtle_land_on_ground(object);
		return;
	}
}

/* attacking: the jump attacks' arcs, steps forward, the end of the attack */
void CONSOLE_ORIGINAL(turtle_attack)(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t input_bit = 0, side_wall_bit = 0, wall_bit = 0;
	uint16_t jump_speed = 0;
	uint32_t x_step = 0;
	uint8_t move_timer = 0, t1 = 0;
	uint16_t height = 0, jump_speed_2 = 0;

	turtles_busy--;
	if (OBJECT(object)->state >= 8u)
	{
		PLAYER_INPUT(inputs)->bits &= 0x10u;
		x_step = 0x400u;
		if (OBJECT(object)->state != 9u)
		{
			x_step = (uint16_t)(x_step - 0x180u);
			if (OBJECT(object)->state != 0xBu)
			{
				x_step = (uint16_t)(x_step - 0x80u);
				if (OBJECT(object)->state != 0xAu)
					x_step = 0u;
			}
		}
		wall_bit = 3u;
		if ((OBJECT(object)->flags & 0x40u) != 0u)
		{
			wall_bit++;
			x_step = 0u - x_step;
		}
		if ((OBJECT(object)->contacts & shl32(1u, (int)(wall_bit))) == 0u)
			OBJECT(object)->x += x_step;
		if (OBJECT(object)->turtle.falling != 0u)
		{
			if ((OBJECT(object)->contacts & 1u) != 0u)
			{
				OBJECT(object)->height_whole = OBJECT(object)->floor_height;
				OBJECT(object)->height &= 0xFF00u;
				turtle_land(object);
				return;
			}
			OBJECT(object)->turtle.jump_speed += 0x40u;
			if (OBJECT(object)->turtle.jump_speed >= 0x500u)
				OBJECT(object)->turtle.jump_speed = 0x500u;
			jump_speed = OBJECT(object)->turtle.jump_speed;
			height = OBJECT(object)->height;
			OBJECT(object)->height = height - jump_speed;
			if (height < jump_speed)
			{
				turtle_land_on_ground(object);
				return;
			}
			return;
		}
		if (OBJECT(object)->turtle.move_timer == 0u)
		{
			jump_speed_2 = OBJECT(object)->turtle.jump_speed;
			OBJECT(object)->turtle.jump_speed = jump_speed_2 - 0x40u;
			if (jump_speed_2 >= 0x40u)
			{
				OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
				return;
			}
			OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
			OBJECT(object)->turtle.falling = ~OBJECT(object)->turtle.falling;
			return;
		}
		move_timer = OBJECT(object)->turtle.move_timer;
		OBJECT(object)->turtle.move_timer = move_timer - 1u;
		if (move_timer == 1u)
		{
			jump_speed_2 = OBJECT(object)->turtle.jump_speed;
			OBJECT(object)->turtle.jump_speed = jump_speed_2 - 0x40u;
			if (jump_speed_2 >= 0x40u)
			{
				OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
				return;
			}
			OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
			OBJECT(object)->turtle.falling = ~OBJECT(object)->turtle.falling;
			return;
		}
		t1 = PLAYER_INPUT(inputs)->bits & 0x10u;
		PLAYER_INPUT(inputs)->bits = t1;
		if (t1 != 0u)
		{
			OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
			return;
		}
		OBJECT(object)->turtle.move_timer = 0u;
		OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
		return;
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_attack_over(held, pressed, object, inputs, presses);
		return;
	}
	if (OBJECT(object)->state == 3u)
	{
		PLAYER_INPUT(inputs)->bits &= 0xCu;
		return;
	}
	if (OBJECT(object)->kind != 3u || OBJECT(object)->state != 7u)
	{
		x_step = 0x40u;
		input_bit = 1u;
		side_wall_bit = 3u;
		if ((OBJECT(object)->flags & 0x40u) != 0u)
		{
			x_step = 0u - x_step;
			input_bit = 0u;
			side_wall_bit++;
		}
		if ((OBJECT(object)->contacts & shl32(1u, (int)(side_wall_bit))) == 0u && (PLAYER_INPUT(inputs)->bits & shl32(1u, (int)(input_bit))) != 0u)
			OBJECT(object)->x += x_step;
		PLAYER_INPUT(inputs)->bits &= 0xCu;
		return;
	}
	PLAYER_INPUT(inputs)->bits = 0u;
	x_step = 0x300u;
	wall_bit = 3u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		x_step = 0u - x_step;
		wall_bit++;
	}
	if ((OBJECT(object)->contacts & shl32(1u, (int)(wall_bit))) != 0u)
		return;
	OBJECT(object)->x += x_step;
	PLAYER_INPUT(inputs)->bits &= 0xCu;
}

/* the attack ended: the combo's next attack, the throw, or back to standing */
void CONSOLE_ORIGINAL(turtle_attack_over)(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t facing = 0, sound = 0, stick = 0;
	uint16_t attack_2 = 0, job = 0, kind_offset = 0, move = 0, routine_state = 0, throw_move = 0;
	uint32_t animation_entry = 0, attack = 0, combo_table = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, target = 0, throw_counter = 0, thrown_enemy = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	OBJECT(object)->contacts &= 0x7Fu;
	if (area_beaten != 0u)
	{
		OBJECT(object)->flags &= ~0xCu;
		routine_state = 0u;
		if (OBJECT(object)->turtle.has_gun == 0u)
		{
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		routine_state = 4u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if (OBJECT(object)->state == 5u)
	{
		OBJECT(object)->flags |= 8u;
		job = 0x401u;
		if (OBJECT(object)->kind != 0u)
		{
			job = 0x408u;
			if (OBJECT(object)->kind != 3u)
			{
				OBJECT(object)->flags ^= 0x40u;
				job = 0x402u;
				if ((OBJECT(object)->kind & 1u) == 0u)
					job += 2u;
			}
		}
		queue_job(job);
	}
	if (OBJECT(object)->turtle.attack_press_time < 0xDu)
	{
		if (area_beaten != 0u)
		{
			OBJECT(object)->flags &= ~0xCu;
			routine_state = 0u;
			if (OBJECT(object)->turtle.has_gun == 0u)
			{
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
			routine_state = 4u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		if (OBJECT(object)->turtle.has_gun != 0u)
		{
			turtle_start_combo(object, inputs);
			return;
		}
		if ((OBJECT(object)->flags & 8u) == 0u)
		{
			OBJECT(object)->flags &= ~0xCu;
			if ((pressed & 0x10u) != 0u)
			{
				turtle_start_jump(object, inputs, presses);
				return;
			}
			if ((pressed & 0x20u) != 0u)
			{
				turtle_start_combo(object, inputs);
				return;
			}
			t1 = held & 0xFu;
			stick = t1;
			if (t1 != 0u)
			{
				turtle_start_walk(stick, object);
				return;
			}
			OBJECT(object)->flags &= ~0xCu;
			routine_state = 0u;
			if (OBJECT(object)->turtle.has_gun == 0u)
			{
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
			routine_state = 4u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		OBJECT(object)->flags &= ~8u;
		OBJECT(object)->flags |= 4u;
		if (OBJECT(object)->turtle.combo_step == 0u)
		{
			OBJECT(object)->turtle.combo_step++;
			combo_table = turtle_combos[OBJECT(object)->kind & 3u];
			if (OBJECT(object)->turtle.enemy_behind != 0u)
			{
				OBJECT(object)->turtle.combo += 0xCu;
				PLAYER_INPUT(inputs)->bits = 0u;
				move = COMBO_STEP(combo_table + ((int16_t)OBJECT(object)->turtle.combo + 1u));
				attack_2 = (uint8_t)(move + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
				if ((OBJECT(object)->flags & 0x40u) != 0u)
					attack_2 = attack_2 | 0x80u;
				if (move == 3u)
					attack_2 = attack_2 ^ 0x80u;
				OBJECT(object)->attack = (uint8_t)attack_2;
				attack_2 &= 0x7Fu;
				sound = turtle_attack_sounds_by_move[attack_2];
				queue_sound(sound);
				OBJECT(object)->damage = turtle_attack_damages[attack_2 & 0x7FFFu];
				move = (uint16_t)((move & 7u) + 0x300u);
				turtle_set_state(move, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
			target = OBJECT(object)->turtle.hit_target;
			if ((int8_t)OBJECT(target)->throw < 0 && OBJECT(target)->health != 0u && OBJECT(target)->height_whole == OBJECT(target)->floor_height)
			{
				kind_offset = (uint16_t)((uint32_t)OBJECT(object)->kind << 1);
				kind_offset += (uint32_t)kind_offset << 1;
				throw_move = turtle_throw_moves[kind_offset / 2u];
				throw_counter = TABLE32(turtle_throw_counters, kind_offset);
				if (THROW_TIMER(throw_counter)->frames >= 0x100u)
				{
					THROW_TIMER(throw_counter)->frames = 0u;
					OBJECT(object)->turtle.idle_time = 0u;
					PLAYER_INPUT(inputs)->bits = 0u;
					OBJECT(target)->routine = 5u;
					OBJECT(target)->state = 0u;
					OBJECT(target)->enemy.move = throw_move;
					facing = 0u;
					if ((OBJECT(object)->flags & 0x40u) == 0u)
						facing = 0x80u;
					OBJECT(target)->enemy.facing = facing;
					OBJECT(object)->turtle.combo += 0x18u;
					attack = 5u;
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 5u;
					if (OBJECT(object)->kind == 0u || OBJECT(object)->kind == 3u)
						OBJECT(object)->flags ^= 0x40u;
					attack = (uint8_t)(attack + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
					if ((OBJECT(object)->flags & 0x40u) != 0u)
						attack = attack | 0x80u;
					OBJECT(object)->attack = attack;
					attack = attack & 0x7Fu;
					sound = turtle_attack_sounds_by_move[attack];
					queue_sound(sound);
					OBJECT(object)->damage = turtle_attack_damages[attack & 0x7FFFu];
					thrown_enemy = OBJECT(object)->turtle.hit_target;
					animation_entry = ANIMATION_ENTRY(turtle_hit_animation_tables[OBJECT(object)->kind] + ((int32_t)(OBJECT(thrown_enemy)->throw & 7u - 4u)) * 0x4u);
					set_animation(attack, object, animation_entry);
					return;
				}
			}
			t2 = PLAYER_INPUT(inputs)->bits & 3u;
			PLAYER_INPUT(inputs)->bits = t2;
			if (t2 == 0u)
			{
				PLAYER_INPUT(inputs)->bits = 0u;
				move = COMBO_STEP(combo_table + ((int16_t)OBJECT(object)->turtle.combo + 1u));
				attack_2 = (uint8_t)(move + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
				if ((OBJECT(object)->flags & 0x40u) != 0u)
					attack_2 = attack_2 | 0x80u;
				if (move == 3u)
					attack_2 = attack_2 ^ 0x80u;
				OBJECT(object)->attack = (uint8_t)attack_2;
				attack_2 &= 0x7Fu;
				sound = turtle_attack_sounds_by_move[attack_2];
				queue_sound(sound);
				OBJECT(object)->damage = turtle_attack_damages[attack_2 & 0x7FFFu];
				move = (uint16_t)((move & 7u) + 0x300u);
				turtle_set_state(move, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
			if ((PLAYER_INPUT(inputs)->bits & 1u) != 0u)
			{
				OBJECT(object)->flags |= 0x40u;
				PLAYER_INPUT(inputs)->bits = 0u;
				move = COMBO_STEP(combo_table + ((int16_t)OBJECT(object)->turtle.combo + 1u));
				attack_2 = (uint8_t)(move + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
				if ((OBJECT(object)->flags & 0x40u) != 0u)
					attack_2 = attack_2 | 0x80u;
				if (move == 3u)
					attack_2 = attack_2 ^ 0x80u;
				OBJECT(object)->attack = (uint8_t)attack_2;
				attack_2 &= 0x7Fu;
				sound = turtle_attack_sounds_by_move[attack_2];
				queue_sound(sound);
				OBJECT(object)->damage = turtle_attack_damages[attack_2 & 0x7FFFu];
				move = (uint16_t)((move & 7u) + 0x300u);
				turtle_set_state(move, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
			OBJECT(object)->flags &= 0xBFu;
			PLAYER_INPUT(inputs)->bits = 0u;
			move = COMBO_STEP(combo_table + ((int16_t)OBJECT(object)->turtle.combo + 1u));
			attack_2 = (uint8_t)(move + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				attack_2 = attack_2 | 0x80u;
			if (move == 3u)
				attack_2 = attack_2 ^ 0x80u;
			OBJECT(object)->attack = (uint8_t)attack_2;
			attack_2 &= 0x7Fu;
			sound = turtle_attack_sounds_by_move[attack_2];
			queue_sound(sound);
			OBJECT(object)->damage = turtle_attack_damages[attack_2 & 0x7FFFu];
			move = (uint16_t)((move & 7u) + 0x300u);
			turtle_set_state(move, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		if (OBJECT(object)->turtle.combo_step != 1u)
		{
			turtle_start_combo(object, inputs);
			return;
		}
		if (OBJECT(object)->state != 5u)
		{
			t3 = PLAYER_INPUT(inputs)->bits & 3u;
			PLAYER_INPUT(inputs)->bits = t3;
			if (t3 != 0u)
			{
				if ((PLAYER_INPUT(inputs)->bits & 1u) != 0u)
					OBJECT(object)->flags |= 0x40u;
				else
					OBJECT(object)->flags &= 0xBFu;
			}
		}
		PLAYER_INPUT(inputs)->bits = 0u;
		OBJECT(object)->turtle.combo_step++;
		move = COMBO_STEP(turtle_combos[OBJECT(object)->kind & 3u] + ((int16_t)OBJECT(object)->turtle.combo + 2u));
		attack_2 = (uint8_t)(move + turtle_attack_firsts[OBJECT(object)->kind & 3u]);
		if ((OBJECT(object)->flags & 0x40u) != 0u)
			attack_2 = attack_2 | 0x80u;
		if (move == 3u)
			attack_2 = attack_2 ^ 0x80u;
		OBJECT(object)->attack = (uint8_t)attack_2;
		attack_2 &= 0x7Fu;
		sound = turtle_attack_sounds_by_move[attack_2];
		queue_sound(sound);
		OBJECT(object)->damage = turtle_attack_damages[attack_2 & 0x7FFFu];
		move = (uint16_t)((move & 7u) + 0x300u);
		turtle_set_state(move, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->flags &= ~0xCu;
	if ((pressed & 0x10u) != 0u)
	{
		turtle_start_jump(object, inputs, presses);
		return;
	}
	if ((pressed & 0x20u) != 0u)
	{
		turtle_start_combo(object, inputs);
		return;
	}
	t1 = held & 0xFu;
	stick = t1;
	if (t1 != 0u)
	{
		turtle_start_walk(stick, object);
		return;
	}
	OBJECT(object)->flags &= ~0xCu;
	routine_state = 0u;
	if (OBJECT(object)->turtle.has_gun == 0u)
	{
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	routine_state = 4u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* hit: the reaction by the attack that hit it, and out of play when its bars run out */
void CONSOLE_ORIGINAL(turtle_hit)(uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint16_t floor_height = 0, hit_reaction = 0, knockback_flags = 0, reaction_offset = 0, reaction_variant = 0, routine_state = 0;
	uint32_t hit_by = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, target = 0, x = 0, x_2 = 0;
	uint8_t health_bars = 0;

	PLAYER_INPUT(inputs)->bits = 0u;
	OBJECT(object)->flags &= ~0xEu;
	if (OBJECT(object)->state == 0u)
	{
		hit_by = OBJECT(object)->turtle.hit_by;
		x = turtle_hit_sounds[OBJECT(hit_by)->hit_sound >> 2];
		if (x != 0xFFu)
			queue_sound((uint8_t)x);
		OBJECT(object)->turtle.falling = 0u;
		OBJECT(object)->turtle.hit_reaction = 0u;
		OBJECT(object)->turtle.reaction_step = 0u;
		OBJECT(object)->turtle.reaction_variant = 0u;
		OBJECT(object)->turtle.wait_timer = 0u;
		knockback_flags = 0u;
		if ((OBJECT(object)->flags & 0x40u) != 0u)
		{
			if ((int8_t)OBJECT(object)->hit >= 0)
			{
				knockback_flags = 0x80u;
				knockback_flags = knockback_flags | 1u;
			}
		}
		else if ((int8_t)OBJECT(object)->hit >= 0)
			knockback_flags = knockback_flags | 1u;
		else
			knockback_flags = 0x80u;
		OBJECT(object)->turtle.knockback_flags = (uint8_t)knockback_flags;
		if ((uint8_t)(OBJECT(object)->hit_routine >> 8u) == 4u)
		{
			hit_reaction = OBJECT(object)->hit & 0x7Fu;
			if (hit_reaction != 0xFu && hit_reaction != 0x10u)
			{
				hit_reaction = 0x15u;
				if (OBJECT(object)->turtle.escape_presses != 0x10u && OBJECT(object)->turtle.escape_presses >= 5u)
					OBJECT(object)->turtle.escape_presses = 4u;
				OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
				routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
				x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
				turtle_save_health(x, object);
				return;
			}
		}
		else
		{
			if ((uint8_t)(OBJECT(object)->hit_routine >> 8u) == 3u && OBJECT(object)->turtle.combo >= 0x18u)
			{
				target = OBJECT(object)->turtle.hit_target;
				if ((OBJECT(target)->flags & 0x80u) == 0u)
				{
					OBJECT(target)->routine = 5u;
					OBJECT(target)->state = 0u;
					OBJECT(target)->enemy.move = turtle_hit_waits[OBJECT(object)->kind];
				}
			}
			hit_reaction = OBJECT(object)->hit & 0x7Fu;
			if (hit_reaction == 0xEu)
			{
				OBJECT(object)->height_whole = OBJECT(object)->floor_height;
				OBJECT(object)->height &= 0xFF00u;
			}
			if (hit_reaction == 0x18u)
			{
				if (OBJECT(object)->health_bars == 0u)
				{
					x = 0x26u;
					queue_sound((uint8_t)x);
					if ((int8_t)OBJECT(object)->turtle.knockback_flags >= 0 || hit_reaction >= 9u)
					{
						OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
						routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
						turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
						x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
						turtle_save_health(x, object);
						return;
					}
					hit_reaction = 6u;
					OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
					routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
					x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
					turtle_save_health(x, object);
					return;
				}
				hit_reaction = 4u;
			}
		}
		if (OBJECT(object)->turtle.escape_presses != 0x10u)
			OBJECT(object)->turtle.escape_presses = 0u;
		if (OBJECT(object)->health_bars == 0u && hit_reaction < 9u)
		{
			hit_reaction = 7u;
			OBJECT(object)->hit = 7u;
		}
		for (;;)
		{
			floor_height = OBJECT(object)->floor_height;
			if (floor_height != OBJECT(object)->height_whole)
			{
				if (hit_reaction >= 9u)
				{
					OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
					routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
					x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
					turtle_save_health(x, object);
					return;
				}
				if (hit_reaction < 4u)
				{
					hit_reaction = 4u;
					if (OBJECT(object)->health != 0u)
					{
						health_bars = OBJECT(object)->health_bars;
						OBJECT(object)->health_bars = health_bars - 1u;
						if (health_bars == 1u)
						{
							hit_reaction = 7u;
							OBJECT(object)->hit = 7u;
							continue;
						}
						hit_reaction = 0x16u;
					}
				}
				if ((int8_t)OBJECT(object)->turtle.knockback_flags >= 0)
					break;
				if (hit_reaction == 4u)
				{
					OBJECT(object)->flags ^= 0x40u;
					OBJECT(object)->turtle.knockback_flags &= 0x7Fu;
					break;
				}
				hit_reaction = (uint8_t)(hit_reaction + 1u);
				break;
			}
			if (OBJECT(object)->health != 0u)
			{
				if ((int8_t)OBJECT(object)->turtle.knockback_flags < 0 && hit_reaction < 9u)
					hit_reaction = 6u;
				OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
				routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
				x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
				turtle_save_health(x, object);
				return;
			}
			if (hit_reaction >= 9u)
			{
				OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
				routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
				x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
				turtle_save_health(x, object);
				return;
			}
			if (hit_reaction < 4u)
				hit_reaction = 4u;
			if ((int8_t)OBJECT(object)->turtle.knockback_flags >= 0)
				break;
			hit_reaction = (uint8_t)(hit_reaction + 1u);
			break;
		}
		reaction_offset = (uint16_t)((uint32_t)hit_reaction << 2);
		OBJECT(object)->turtle.drift_speed = TABLE16(turtle_hit_speeds, reaction_offset - 0x10u);
		OBJECT(object)->turtle.jump_speed = TABLE16(turtle_hit_speeds, reaction_offset - 0xEu);
		reaction_variant = (uint8_t)frame_count;
		reaction_variant = reaction_variant & 1u;
		OBJECT(object)->turtle.reaction_variant = (uint8_t)reaction_variant;
		x_2 = HIT_NEAR_LEFT_EDGE_X;
		x = HIT_NEAR_RIGHT_EDGE_X;
		offset_high_word = HIT_NEAR_RIGHT_EDGE_X;
		if (hit_reaction != 4u)
		{
			if (hit_reaction != 5u)
			{
				OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
				routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
				x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
				turtle_save_health(x, object);
				return;
			}
			x_2 = 0x10000u;
			x = 0x10000u;
			offset_high_word = 0x10000u;
		}
		if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		{
			if (OBJECT(object)->x <= x)
			{
				reaction_variant = 0u;
				if (OBJECT(object)->health_bars < 5u)
					reaction_variant = (uint8_t)(reaction_variant + 1u);
				OBJECT(object)->turtle.reaction_variant = (uint8_t)reaction_variant;
				OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
				routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
				x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
				turtle_save_health(x, object);
				return;
			}
			OBJECT(object)->turtle.reaction_variant += 2u;
			OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
			routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
			x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
			turtle_save_health(x, object);
			return;
		}
		if (x_2 < OBJECT(object)->x)
		{
			reaction_variant = 0u;
			if (OBJECT(object)->health_bars < 5u)
				reaction_variant = (uint8_t)(reaction_variant + 1u);
			OBJECT(object)->turtle.reaction_variant = (uint8_t)reaction_variant;
			OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
			routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
			x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
			turtle_save_health(x, object);
			return;
		}
		OBJECT(object)->turtle.reaction_variant += 2u;
		OBJECT(object)->turtle.hit_reaction = (uint8_t)hit_reaction;
		routine_state = (uint16_t)(turtle_hit_reactions[hit_reaction] + 0x400u);
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &x_2);
		x = (x & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
		turtle_save_health(x, object);
		return;
	}
	switch (OBJECT(object)->turtle.hit_reaction)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 6:
	case 13:
		turtle_hit_wait(object);
		break;
	case 4:
		turtle_knocked_down(object);
		break;
	case 5:
		turtle_knocked_back(object);
		break;
	case 7:
		turtle_knocked_flying(object);
		break;
	case 8:
		turtle_knocked_flying_from_behind(object);
		break;
	case 9:
		turtle_frozen(object);
		break;
	case 10:
	case 11:
		turtle_shocked(object);
		break;
	case 12:
		turtle_fall_in_manhole(object);
		break;
	case 14:
		turtle_held(object, presses);
		break;
	case 15:
		turtle_held_knock_off(object, presses);
		break;
	case 16:
		turtle_held_throw_off(object, presses);
		break;
	case 17:
	case 18:
	case 19:
	case 20:
		turtle_refill_health(object);
		break;
	case 21:
		turtle_held_briefly(object, presses);
		break;
	case 22:
	case 23:
		turtle_hit_fly(object);
		break;
	case 24:
		turtle_hit_by_shredder_ray(object);
		break;
	case 25:
		turtle_walk_off_screen(object);
		break;
	}
	if (OBJECT(object)->turtle.reaction_step != 0xFFu)
		return;
	if (OBJECT(object)->health == 0u)
		OBJECT(object)->health = 3u;
	if (OBJECT(object)->health_bars != 0u)
	{
		routine_state = 0u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	time_without_knockout = 0u;
	OBJECT(object)->flags = 0u;
	players_knocked_out |= shl32(1u, (int)(OBJECT(object)->kind & 7u));
}

/* the reaction lasts its animation */
void turtle_hit_wait(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
		OBJECT(object)->turtle.reaction_step = 0xFFu;
}

/* the turtle's health full again */
void turtle_refill_health(uint32_t object)
{
	uint32_t job = 0;

	OBJECT(object)->turtle.reaction_step = 0xFFu;
	OBJECT(object)->health_bars = 0xAu;
	OBJECT(object)->health = 3u;
	job = offset_high_word & ~0xFFFFu;
	turtle_save_health(job, object);
}

/* Shredder's ray turns the turtle into an ordinary turtle; with no lives left it walks off the screen */
void turtle_hit_by_shredder_ray(uint32_t object)
{
	uint16_t player_offset = 0, routine_state = 0, screen_x = 0, screen_y = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint16_t screen_y_2 = 0;

	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	if (OBJECT(object)->frame_time == 0u)
	{
		if (PLAYER(PLAYER_ADDRESS(0) + (OBJECT(object)->kind) * 0x8u)->lives == 0u)
		{
			OBJECT(object)->turtle.hit_reaction++;
			routine_state = 0x42Bu;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		OBJECT(object)->turtle.reaction_step = 0xFFu;
		return;
	}
	if (OBJECT(object)->frame_time != 0x30u)
		return;
	player_offset = (uint16_t)((uint32_t)OBJECT(object)->kind << 3);
	if (PLAYER(PLAYER_ADDRESS(0) + (player_offset / 8u) * 0x8u)->lives != 0u)
		return;
	screen_x = OBJECT(object)->screen_x;
	screen_y_2 = OBJECT(object)->screen_y;
	screen_y = (uint16_t)(screen_y_2 - 0x20u);
	if (screen_y_2 < 0x20u)
		screen_y = 0u;
	say_shell_shock(player_offset, screen_x, screen_y, object, &ignored_2, &ignored_3);
}

/* the turtle walks off the screen */
void CONSOLE_ORIGINAL(turtle_walk_off_screen)(uint32_t object)
{
	uint32_t x_step = 0;

	x_step = 0x200u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
	if (OBJECT(object)->x >= 0x16000u || OBJECT(object)->x < 0x9100u)
		OBJECT(object)->turtle.reaction_step = 0xFFu;
}

/* knocked down: flies, slides, lies, gets up */
void turtle_knocked_down(uint32_t object)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 0:
		turtle_knocked_down_fly(object);
		return;
	case 1:
		turtle_knocked_down_slide(object);
		return;
	case 2:
	case 3:
		turtle_knocked_down_lie(object);
		return;
	}
}

/* a frame of the knock down's flight, until it lands on a floor */
void turtle_knocked_down_fly(uint32_t object)
{
	uint16_t jump_speed = 0;

	OBJECT(object)->turtle.wait_timer = 8u;
	jump_speed = 0x60u;
	offset_high_word = 0x60u;
	turtle_knockback_arc(jump_speed, object);
	if (OBJECT(object)->turtle.falling == 0u)
		return;
	if ((OBJECT(object)->contacts & 1u) != 0u || OBJECT(object)->height == 0u)
	{
		turtle_knocked_down_next(object);
		return;
	}
}

/* slides along the ground within the walls and the screen, farther for the hard knocks, until the pose
   ends */
void CONSOLE_ORIGINAL(turtle_knocked_down_slide)(uint32_t object)
{
	uint8_t wall_bit = 0;
	uint16_t x_step = 0;

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
	if (OBJECT(object)->x < 0x16000u)
	{
		if (OBJECT(object)->x >= 0x9000u)
			return;
		OBJECT(object)->x = 0x9000u;
		return;
	}
	OBJECT(object)->x = 0x16000u;
}

/* lies on its floor until the pose ends */
void turtle_knocked_down_lie(uint32_t object)
{
	turtle_drop_to_floor(object);
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_down_next(object);
		return;
	}
}

/* the knock down's next step by its kind, or the end of the steps */
void turtle_knocked_down_next(uint32_t object)
{
	uint16_t knock_offset = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, knock_steps = 0;
	uint8_t knock_step = 0;

	OBJECT(object)->turtle.reaction_step++;
	knock_offset = (uint16_t)((uint32_t)OBJECT(object)->turtle.reaction_variant << 2);
	knock_steps = turtle_knocked_down_steps[knock_offset / 4u];
	knock_offset = (uint16_t)IN_PAGE(knock_offset, OBJECT(object)->turtle.reaction_step);
	knock_step = KNOCK_STEP(knock_steps + knock_offset);
	knock_offset = (uint16_t)IN_PAGE(knock_offset, knock_step);
	if ((int8_t)knock_step < 0)
	{
		OBJECT(object)->turtle.reaction_step = (uint8_t)knock_offset;
		return;
	}
	knock_offset += 0x400u;
	turtle_set_state(knock_offset, object, &ignored, &ignored_2, &ignored_3);
}

/* the turtle comes down to its floor */
void turtle_drop_to_floor(uint32_t object)
{
	uint8_t height_whole = 0;
	uint16_t height = 0;

	if ((OBJECT(object)->contacts & 1u) != 0u)
		return;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - 0x300u;
	if (height >= 0x300u)
	{
		height_whole = OBJECT(object)->height_whole;
		if (height_whole == OBJECT(object)->floor_height || height_whole < OBJECT(object)->floor_height)
		{
			OBJECT(object)->contacts |= 1u;
			OBJECT(object)->height_whole = OBJECT(object)->floor_height;
			OBJECT(object)->height &= 0xFF00u;
		}
		return;
	}
	OBJECT(object)->contacts |= 1u;
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
}

/* knocked back: pushed, slides, gets up */
void turtle_knocked_back(uint32_t object)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 0:
		turtle_knocked_back_start(object);
		return;
	case 1:
		turtle_knocked_back_slide(object);
		return;
	case 2:
	case 3:
		turtle_knocked_back_lie(object);
		return;
	}
}

/* when the pose ends, pushed back for the hard knocks, then the next step */
void turtle_knocked_back_start(uint32_t object)
{
	uint32_t x_step = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	if (OBJECT(object)->turtle.reaction_variant < 2u)
	{
		turtle_knocked_back_next(object);
		return;
	}
	x_step = 0x1800u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
	turtle_knocked_back_next(object);
}

/* slides along the ground for the hard knocks, within the walls and the screen, until the pose ends */
void CONSOLE_ORIGINAL(turtle_knocked_back_slide)(uint32_t object)
{
	uint8_t wall_bit = 0;
	uint16_t x_step = 0;

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
	if (OBJECT(object)->x < 0x16000u)
	{
		if (OBJECT(object)->x >= 0x9000u)
			return;
		OBJECT(object)->x = 0x9000u;
		return;
	}
	OBJECT(object)->x = 0x16000u;
}

/* lies on its floor until the pose ends */
void turtle_knocked_back_lie(uint32_t object)
{
	turtle_drop_to_floor(object);
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_back_next(object);
		return;
	}
}

/* the knock back's next step by its kind; at the end the light knocks push it back a little */
void turtle_knocked_back_next(uint32_t object)
{
	uint16_t knock_offset = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, knock_steps = 0, x_step = 0;
	uint8_t knock_step = 0;

	OBJECT(object)->turtle.reaction_step++;
	knock_offset = (uint16_t)((uint32_t)OBJECT(object)->turtle.reaction_variant << 2);
	knock_steps = turtle_knocked_back_steps[knock_offset / 4u];
	knock_offset = (uint16_t)IN_PAGE(knock_offset, OBJECT(object)->turtle.reaction_step);
	knock_step = KNOCK_STEP(knock_steps + knock_offset);
	knock_offset = (uint16_t)IN_PAGE(knock_offset, knock_step);
	if ((int8_t)knock_step >= 0)
	{
		knock_offset += 0x400u;
		turtle_set_state(knock_offset, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->turtle.reaction_step = (uint8_t)knock_offset;
	if (OBJECT(object)->turtle.reaction_variant >= 2u)
		return;
	x_step = reset_ADDRESS;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
}

/* knocked flying, or knocked out by the hit that takes its last bar */
void turtle_knocked_flying(uint32_t object)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 0:
		turtle_knocked_flying_fly(object);
		return;
	case 1:
		turtle_knocked_flying_slide(object);
		return;
	case 2:
	case 3:
		turtle_knocked_flying_lie(object);
		return;
	}
}

/* a frame of the flight: against a wall a crash with its sound; landing, the rest by the bars it has
   left */
void turtle_knocked_flying_fly(uint32_t object)
{
	uint8_t reaction_variant = 0, sound = 0, wall_bit = 0;
	uint16_t jump_speed = 0;

	OBJECT(object)->turtle.wait_timer = 8u;
	wall_bit = 3u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		wall_bit++;
	if ((OBJECT(object)->contacts & shl32(1u, (int)(wall_bit))) != 0u)
	{
		sound = 0x89u;
		queue_sound(sound);
		reaction_variant = 2u;
		if (OBJECT(object)->health_bars == 0u)
			reaction_variant = 4u;
		OBJECT(object)->turtle.reaction_variant = reaction_variant;
		turtle_knocked_flying_next(object);
		return;
	}
	jump_speed = 0x50u;
	offset_high_word = 0x50u;
	turtle_knockback_arc(jump_speed, object);
	if (OBJECT(object)->turtle.falling == 0u || (OBJECT(object)->height != 0u && (OBJECT(object)->contacts & 1u) == 0u))
		return;
	reaction_variant = 0u;
	if (OBJECT(object)->health_bars < 5u)
		reaction_variant++;
	if (OBJECT(object)->health_bars == 0u)
		reaction_variant = 3u;
	OBJECT(object)->turtle.reaction_variant = reaction_variant;
	turtle_knocked_flying_next(object);
}

/* after hitting a wall it drops down to the floor, else it slides a little; then a wait */
void CONSOLE_ORIGINAL(turtle_knocked_flying_slide)(uint32_t object)
{
	uint32_t x_step = 0;
	uint16_t height = 0;

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
	if (OBJECT(object)->x >= 0x16000u)
	{
		OBJECT(object)->x = 0x16000u;
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_wait(object);
			return;
		}
		return;
	}
	if (OBJECT(object)->x >= 0x9000u)
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_wait(object);
			return;
		}
		return;
	}
	OBJECT(object)->x = 0x9000u;
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_flying_wait(object);
		return;
	}
}

/* lies on its floor: waits its time, or until the pose ends */
void turtle_knocked_flying_lie(uint32_t object)
{
	uint8_t wait_timer = 0;

	turtle_drop_to_floor(object);
	if (OBJECT(object)->state == 0x11u)
	{
		wait_timer = OBJECT(object)->turtle.wait_timer;
		OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
		if (wait_timer != 1u)
			return;
		turtle_knocked_flying_wait(object);
		return;
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_flying_next(object);
		return;
	}
}

/* a wait of 96 frames, then the next step */
void turtle_knocked_flying_wait(uint32_t object)
{
	OBJECT(object)->turtle.wait_timer = 0x60u;
	turtle_knocked_flying_next(object);
}

/* the knock flying's next step by its kind, or the end of the steps */
void turtle_knocked_flying_next(uint32_t object)
{
	uint16_t knock_offset = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, knock_steps = 0;
	uint8_t knock_step = 0;

	OBJECT(object)->turtle.reaction_step++;
	knock_offset = (uint16_t)((uint32_t)OBJECT(object)->turtle.reaction_variant << 2);
	knock_steps = turtle_knocked_flying_steps[knock_offset / 4u];
	knock_offset = (uint16_t)IN_PAGE(knock_offset, OBJECT(object)->turtle.reaction_step);
	knock_step = KNOCK_STEP(knock_steps + knock_offset);
	knock_offset = (uint16_t)IN_PAGE(knock_offset, knock_step);
	if ((int8_t)knock_step < 0)
	{
		OBJECT(object)->turtle.reaction_step = (uint8_t)knock_offset;
		return;
	}
	knock_offset += 0x400u;
	turtle_set_state(knock_offset, object, &ignored, &ignored_2, &ignored_3);
}

/* knocked flying from behind: flies, slides, lies, gets up */
void turtle_knocked_flying_from_behind(uint32_t object)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 0:
		turtle_knocked_flying_from_behind_fly(object);
		return;
	case 1:
		turtle_knocked_flying_from_behind_slide(object);
		return;
	case 2:
	case 3:
		turtle_knocked_flying_from_behind_lie(object);
		return;
	}
}

/* the knock from behind's flight until it lands or meets a wall */
void turtle_knocked_flying_from_behind_fly(uint32_t object)
{
	uint8_t reaction_variant = 0, wall_bit = 0;
	uint16_t jump_speed = 0;

	OBJECT(object)->turtle.wait_timer = 8u;
	wall_bit = 3u;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		wall_bit++;
	if ((OBJECT(object)->contacts & shl32(1u, (int)(wall_bit))) != 0u)
	{
		if (OBJECT(object)->health_bars != 0u)
		{
			OBJECT(object)->turtle.reaction_variant = 1u;
			turtle_knocked_flying_from_behind_next(object);
			return;
		}
		OBJECT(object)->turtle.reaction_variant = 4u;
		turtle_knocked_flying_from_behind_next(object);
		return;
	}
	jump_speed = 0x50u;
	offset_high_word = 0x50u;
	turtle_knockback_arc(jump_speed, object);
	if (OBJECT(object)->turtle.falling == 0u || (OBJECT(object)->height != 0u && (OBJECT(object)->contacts & 1u) == 0u))
		return;
	reaction_variant = 0u;
	if (OBJECT(object)->health_bars == 0u)
		reaction_variant = 3u;
	OBJECT(object)->turtle.reaction_variant = reaction_variant;
	turtle_knocked_flying_from_behind_next(object);
}

/* the turtle slides along the ground, or drops down a wall */
void CONSOLE_ORIGINAL(turtle_knocked_flying_from_behind_slide)(uint32_t object)
{
	uint32_t x_step = 0;
	uint16_t height = 0;

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
	if (OBJECT(object)->x >= 0x16000u)
	{
		OBJECT(object)->x = 0x16000u;
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		return;
	}
	if (OBJECT(object)->x >= 0x9000u)
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			turtle_knocked_flying_from_behind_wait(object);
			return;
		}
		return;
	}
	OBJECT(object)->x = 0x9000u;
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_flying_from_behind_wait(object);
		return;
	}
}

/* the turtle lies on its floor */
void turtle_knocked_flying_from_behind_lie(uint32_t object)
{
	uint8_t wait_timer = 0;

	turtle_drop_to_floor(object);
	if (OBJECT(object)->state == 0x11u)
	{
		wait_timer = OBJECT(object)->turtle.wait_timer;
		OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
		if (wait_timer != 1u)
			return;
		turtle_knocked_flying_from_behind_wait(object);
		return;
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_knocked_flying_from_behind_next(object);
		return;
	}
}

/* the turtle lies a while */
void turtle_knocked_flying_from_behind_wait(uint32_t object)
{
	OBJECT(object)->turtle.wait_timer = 0x60u;
	turtle_knocked_flying_from_behind_next(object);
}

/* the knock's next step, or its end */
void turtle_knocked_flying_from_behind_next(uint32_t object)
{
	uint16_t knock_offset = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, knock_steps = 0, x_step = 0;
	uint8_t knock_step = 0;

	OBJECT(object)->turtle.reaction_step++;
	knock_offset = (uint16_t)((uint32_t)OBJECT(object)->turtle.reaction_variant << 2);
	knock_steps = turtle_knocked_flying_from_behind_steps[knock_offset / 4u];
	knock_offset = (uint16_t)IN_PAGE(knock_offset, OBJECT(object)->turtle.reaction_step);
	knock_step = KNOCK_STEP(knock_steps + knock_offset);
	knock_offset = (uint16_t)IN_PAGE(knock_offset, knock_step);
	if ((int8_t)knock_step >= 0)
	{
		knock_offset += 0x400u;
		turtle_set_state(knock_offset, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->turtle.reaction_step = (uint8_t)knock_offset;
	if (OBJECT(object)->turtle.reaction_variant == 1u || OBJECT(object)->turtle.reaction_variant == 4u)
		return;
	x_step = reset_ADDRESS;
	if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
		x_step = 0u - x_step;
	OBJECT(object)->x += x_step;
}

/* frozen in a block of ice until it bursts */
void turtle_frozen(uint32_t object)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 0:
		turtle_frozen_start(object);
		return;
	case 1:
		turtle_frozen_in_ice(object);
		return;
	case 2:
		turtle_frozen_thaw(object);
		return;
	case 3:
		turtle_frozen_countdown(object);
		return;
	}
}

/* the turtle frozen for a while */
void turtle_frozen_start(uint32_t object)
{
	OBJECT(object)->turtle.reaction_step++;
	OBJECT(object)->turtle.wait_timer = 0x60u;
}

/* the frozen turtle drops to the ground, then the ice bursts */
void turtle_frozen_in_ice(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t wait_timer = 0;
	uint16_t height = 0;

	if (OBJECT(object)->height != 0u)
	{
		OBJECT(object)->floor_height = 0u;
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - 0x300u;
		if (height < 0x300u)
			OBJECT(object)->height = 0u;
	}
	wait_timer = OBJECT(object)->turtle.wait_timer;
	OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
	if (wait_timer != 1u)
		return;
	OBJECT(object)->turtle.reaction_step++;
	routine_state = 0x41Du;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
	sound = 0x2Eu;
	queue_sound(sound);
}

/* free of the ice, or dead with no health left */
void turtle_frozen_thaw(uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->turtle.reaction_step = 0xFFu;
}

/* the turtle dies after a while */
void turtle_frozen_countdown(uint32_t object)
{
	uint8_t wait_timer = 0;

	wait_timer = OBJECT(object)->turtle.wait_timer;
	OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
	if (wait_timer == 1u)
	{
		OBJECT(object)->turtle.reaction_step = 0xFFu;
		return;
	}
}

/* electrocuted or blasted until it drops, or dies with no health left */
void turtle_shocked(uint32_t object)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 0:
		turtle_shocked_start(object);
		return;
	case 1:
		turtle_shocked_drop(object);
		return;
	case 2:
		turtle_hit_countdown(object);
		return;
	}
}

/* the turtle shocked for a while */
void turtle_shocked_start(uint32_t object)
{
	OBJECT(object)->turtle.reaction_step++;
	OBJECT(object)->turtle.wait_timer = 0x50u;
}

/* the shocked turtle drops to the ground, then is free or dies */
void turtle_shocked_drop(uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t wait_timer = 0;
	uint16_t height = 0;

	if (OBJECT(object)->height != 0u)
	{
		OBJECT(object)->floor_height = 0u;
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - 0x200u;
		if (height < 0x200u)
			OBJECT(object)->height = 0u;
	}
	wait_timer = OBJECT(object)->turtle.wait_timer;
	OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
	if (wait_timer != 1u)
		return;
	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->turtle.reaction_step = 0xFFu;
}

/* the reaction's last wait */
void turtle_hit_countdown(uint32_t object)
{
	uint8_t wait_timer = 0;

	wait_timer = OBJECT(object)->turtle.wait_timer;
	OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
	if (wait_timer == 1u)
	{
		OBJECT(object)->turtle.reaction_step = 0xFFu;
		return;
	}
}

/* the turtle falls into an open manhole and climbs out */
void turtle_fall_in_manhole(uint32_t object)
{
	uint16_t step_offset = 0;

	step_offset = (uint16_t)((uint32_t)OBJECT(object)->turtle.reaction_step << 2);
	switch (step_offset)
	{
	case 0:
		turtle_fall_in_manhole_start(object);
		return;
	case 4:
		turtle_fall_in_manhole_climb_out(step_offset, object);
		return;
	}
}

/* the turtle loses a bar and drops into the hole */
void turtle_fall_in_manhole_start(uint32_t object)
{
	uint32_t job = 0, manhole = 0, y = 0;

	if (OBJECT(object)->health_bars != 1u)
	{
		OBJECT(object)->health_bars--;
		job = offset_high_word & ~0xFFFFu;
		turtle_save_health(job, object);
	}
	OBJECT(object)->turtle.reaction_step++;
	OBJECT(object)->flags &= 0xBFu;
	manhole = OBJECT(object)->turtle.hit_by;
	y = (uint32_t)OBJECT(manhole)->screen_y << 8;
	OBJECT(object)->x = (uint32_t)OBJECT(manhole)->screen_x << 8;
	OBJECT(object)->y = y;
	OBJECT(object)->turtle.move_timer = 0x1Au;
}

/* a speech in the hole, then the turtle climbs out */
void turtle_fall_in_manhole_climb_out(uint32_t on_right, uint32_t object)
{
	uint16_t line = 0, screen_x = 0, screen_y = 0;
	uint32_t bank = 0, ignored = 0, ignored_2 = 0;
	uint8_t move_timer = 0;
	uint16_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->turtle.reaction_step = 0xFFu;
		OBJECT(object)->y -= 0x1000u;
		return;
	}
	if (OBJECT(object)->turtle.move_timer == 0u)
		return;
	move_timer = OBJECT(object)->turtle.move_timer;
	OBJECT(object)->turtle.move_timer = move_timer - 1u;
	if (move_timer != 1u)
		return;
	on_right &= ~0xFFu;
	screen_x = OBJECT(object)->screen_x;
	screen_y = OBJECT(object)->screen_y;
	if (bebop_exchange_lines != 0u && (bebop_exchange_lines < 3u || bebop_exchange_lines != 3u || time_since_balloon < 0x80u))
		return;
	screen_x -= 0x30u;
	t1 = screen_y;
	screen_y = (uint16_t)(t1 - 0x38u);
	if (t1 < 0x38u)
		screen_y = 0u;
	line = 0x64u;
	bank = 0x20u;
	screen_x_high = screen_x & 0xFF00u;
	show_balloon(screen_x, screen_y, line, &on_right, &bank, &ignored, &ignored_2);
	draw_wide_balloon_pointer(ignored_2);
}

/* held by an enemy until it breaks free or its health runs out */
void turtle_held(uint32_t object, uint32_t presses)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 1:
		turtle_hit_countdown(object);
		return;
	case 0:
		turtle_held_struggle(object, presses);
		return;
	}
}

/* held: the prompt, the health draining, the presses to break free */
void turtle_held_struggle(uint32_t object, uint32_t presses)
{
	uint8_t move = 0;
	uint16_t prompt_y = 0, routine_state = 0, screen_x = 0;
	uint32_t holder = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, job = 0;

	screen_x = OBJECT(object)->screen_x;
	prompt_y = (uint16_t)(OBJECT(object)->screen_y + OBJECT(object)->height_whole);
	turtle_show_prompt(screen_x, prompt_y, object);
	if (stage == 8u && scroll_mode < 0)
	{
		if (OBJECT(object)->health_bars == 0u)
		{
			turtle_held_dies(object);
			return;
		}
		turtle_erase_prompt(object);
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->state = 3u;
		OBJECT(object)->health = 3u;
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->turtle.escape_presses = 0u;
		OBJECT(object)->turtle.combo_step = 2u;
		move = 6u;
		turtle_start_attack(move, object);
		return;
	}
	if (OBJECT(object)->turtle.escape_presses >= 8u)
	{
		turtle_erase_prompt(object);
		OBJECT(object)->turtle.escape_presses = 0u;
		routine_state = 0u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if (OBJECT(object)->health_bars == 0u)
	{
		turtle_held_dies(object);
		return;
	}
	if ((frame_count & 0x7Fu) == 0u)
	{
		OBJECT(object)->health_bars--;
		job = offset_high_word & ~0xFFFFu;
		turtle_save_health(job, object);
		if (OBJECT(object)->health_bars == 0u)
		{
			turtle_held_dies(object);
			return;
		}
	}
	OBJECT(object)->flags |= 2u;
	if (PLAYER_INPUT(presses)->bits == 0u)
		return;
	OBJECT(object)->turtle.escape_presses++;
	if (OBJECT(object)->turtle.escape_presses < 8u)
		return;
	if (OBJECT(object)->health_bars == 0u)
	{
		turtle_held_dies(object);
		return;
	}
	turtle_erase_prompt(object);
	holder = OBJECT(object)->turtle.hit_by;
	OBJECT(holder)->state = 3u;
	OBJECT(object)->health = 3u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->turtle.escape_presses = 0u;
	OBJECT(object)->turtle.combo_step = 2u;
	move = 6u;
	turtle_start_attack(move, object);
}

/* the turtle's health ran out while held: the holder lets go and it dies */
void turtle_held_dies(uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t holder = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0;

	turtle_erase_prompt(object);
	holder = OBJECT(object)->turtle.hit_by;
	if (OBJECT(holder)->routine != 4u)
		OBJECT(holder)->state = 3u;
	OBJECT(object)->turtle.reaction_step++;
	OBJECT(object)->turtle.wait_timer = 0x60u;
	routine_state = 0x411u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* held by an enemy for a moment, free after fewer presses */
void turtle_held_briefly(uint32_t object, uint32_t presses)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 1:
		turtle_hit_countdown(object);
		return;
	case 0:
		turtle_held_briefly_struggle(object, presses);
		return;
	}
}

/* held a moment: the prompt; six presses break free with an attack, else held fast when the pose ends;
   no bars, it dies */
void turtle_held_briefly_struggle(uint32_t object, uint32_t presses)
{
	uint8_t move = 0;
	uint16_t prompt_y = 0, routine_state = 0, screen_x = 0;
	uint32_t holder = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0;

	screen_x = OBJECT(object)->screen_x;
	prompt_y = (uint16_t)(OBJECT(object)->screen_y + OBJECT(object)->height_whole);
	turtle_show_prompt(screen_x, prompt_y, object);
	if (stage == 8u && scroll_mode < 0)
	{
		if (OBJECT(object)->health_bars == 0u)
		{
			turtle_held_dies(object);
			return;
		}
		turtle_erase_prompt(object);
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->state = 3u;
		OBJECT(object)->turtle.escape_presses = 0u;
		OBJECT(object)->health = 3u;
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->turtle.combo_step = 2u;
		move = 6u;
		turtle_start_attack(move, object);
		return;
	}
	if (OBJECT(object)->frame_time != 0u)
	{
		if (OBJECT(object)->turtle.escape_presses >= 6u)
		{
			turtle_erase_prompt(object);
			if (OBJECT(object)->health_bars == 0u)
			{
				OBJECT(object)->turtle.reaction_step++;
				OBJECT(object)->turtle.wait_timer = 0x60u;
				routine_state = 0x411u;
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
			OBJECT(object)->turtle.escape_presses = 0u;
			routine_state = 0u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		if (PLAYER_INPUT(presses)->bits == 0u)
			return;
		OBJECT(object)->turtle.escape_presses++;
		if (OBJECT(object)->turtle.escape_presses < 6u)
			return;
		if (OBJECT(object)->health_bars == 0u)
		{
			turtle_held_dies(object);
			return;
		}
		turtle_erase_prompt(object);
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->state = 3u;
		OBJECT(object)->turtle.escape_presses = 0u;
		OBJECT(object)->health = 3u;
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->turtle.combo_step = 2u;
		move = 6u;
		turtle_start_attack(move, object);
		return;
	}
	if (OBJECT(object)->health_bars == 0u)
	{
		turtle_held_dies(object);
		return;
	}
	if (OBJECT(object)->health == 0u)
		OBJECT(object)->health = 3u;
	OBJECT(object)->turtle.hit_reaction = 0xEu;
	OBJECT(object)->turtle.reaction_step = 0u;
	routine_state = 0x422u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* held by an enemy that is knocked away when the turtle breaks free */
void turtle_held_knock_off(uint32_t object, uint32_t presses)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 1:
		turtle_hit_countdown(object);
		return;
	case 0:
		turtle_held_knock_off_struggle(object, presses);
		return;
	}
}

/* held: the prompt and a bar lost every 64 frames; eight presses break free and the holder lets go; no
   bars, it drops */
void turtle_held_knock_off_struggle(uint32_t object, uint32_t presses)
{
	uint16_t prompt_y = 0, routine_state = 0, screen_x = 0;
	uint32_t holder = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, job = 0;

	screen_x = OBJECT(object)->screen_x;
	prompt_y = (uint16_t)(OBJECT(object)->screen_y + OBJECT(object)->height_whole);
	turtle_show_prompt(screen_x, prompt_y, object);
	if (stage == 8u && scroll_mode < 0)
	{
		if (OBJECT(object)->health_bars != 0u)
		{
			holder = OBJECT(object)->turtle.hit_by;
			OBJECT(holder)->routine = 2u;
			turtle_erase_prompt(object);
			routine_state = 0u;
			OBJECT(object)->turtle.escape_presses = 0u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->routine = 2u;
		turtle_erase_prompt(object);
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	if (OBJECT(object)->turtle.escape_presses >= 8u)
	{
		turtle_erase_prompt(object);
		routine_state = 0u;
		OBJECT(object)->turtle.escape_presses = 0u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if (OBJECT(object)->health_bars == 0u)
	{
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->routine = 2u;
		turtle_erase_prompt(object);
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if ((frame_count & 0x3Fu) != 0u)
	{
		if (PLAYER_INPUT(presses)->bits == 0u)
			return;
		OBJECT(object)->turtle.escape_presses++;
		if (OBJECT(object)->turtle.escape_presses < 8u)
			return;
		if (OBJECT(object)->health_bars != 0u)
		{
			holder = OBJECT(object)->turtle.hit_by;
			OBJECT(holder)->routine = 2u;
			turtle_erase_prompt(object);
			routine_state = 0u;
			OBJECT(object)->turtle.escape_presses = 0u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->routine = 2u;
		turtle_erase_prompt(object);
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(object)->health_bars--;
	job = offset_high_word & ~0xFFFFu;
	turtle_save_health(job, object);
	if (OBJECT(object)->health_bars == 0u)
	{
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->routine = 2u;
		turtle_erase_prompt(object);
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if (PLAYER_INPUT(presses)->bits == 0u)
		return;
	OBJECT(object)->turtle.escape_presses++;
	if (OBJECT(object)->turtle.escape_presses < 8u)
		return;
	if (OBJECT(object)->health_bars != 0u)
	{
		holder = OBJECT(object)->turtle.hit_by;
		OBJECT(holder)->routine = 2u;
		turtle_erase_prompt(object);
		routine_state = 0u;
		OBJECT(object)->turtle.escape_presses = 0u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	holder = OBJECT(object)->turtle.hit_by;
	OBJECT(holder)->routine = 2u;
	turtle_erase_prompt(object);
	OBJECT(object)->turtle.reaction_step++;
	OBJECT(object)->turtle.wait_timer = 0x60u;
	routine_state = 0x411u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* held by an enemy the turtle throws when it breaks free */
void turtle_held_throw_off(uint32_t object, uint32_t presses)
{
	switch (OBJECT(object)->turtle.reaction_step)
	{
	case 1:
		turtle_hit_countdown(object);
		return;
	case 0:
		turtle_held_throw_off_struggle(object, presses);
		return;
	}
}

/* held: the prompt, a bar lost every 64 frames, a struggle each press; eight throw the holder off,
   scored; no bars, it dies */
void turtle_held_throw_off_struggle(uint32_t object, uint32_t presses)
{
	uint8_t facing = 0;
	uint16_t job_2 = 0, kind = 0, prompt_y = 0, routine_state = 0, screen_x = 0;
	uint32_t holder = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, job = 0;
	uint8_t wait_timer = 0;

	screen_x = OBJECT(object)->screen_x;
	prompt_y = (uint16_t)(OBJECT(object)->screen_y + OBJECT(object)->height_whole);
	turtle_show_prompt(screen_x, prompt_y, object);
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	if (OBJECT(object)->turtle.escape_presses < 0x10u)
	{
		if (OBJECT(object)->health_bars == 0u)
		{
			turtle_erase_prompt(object);
			holder = OBJECT(object)->turtle.hit_by;
			OBJECT(holder)->flags |= 2u;
			OBJECT(holder)->routine = 2u;
			OBJECT(holder)->state = 0u;
			OBJECT(holder)->height_whole = OBJECT(holder)->floor_height;
			OBJECT(holder)->enemy.move_time = 0u;
			OBJECT(holder)->flags |= 0x80u;
			OBJECT(object)->turtle.escape_presses = 0u;
			OBJECT(object)->turtle.reaction_step++;
			OBJECT(object)->turtle.wait_timer = 0x60u;
			routine_state = 0x411u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		if ((frame_count & 0x3Fu) == 0u)
		{
			OBJECT(object)->health_bars--;
			job = offset_high_word & ~0xFFFFu;
			turtle_save_health(job, object);
			if (OBJECT(object)->health_bars == 0u)
			{
				turtle_erase_prompt(object);
				holder = OBJECT(object)->turtle.hit_by;
				OBJECT(holder)->flags |= 2u;
				OBJECT(holder)->routine = 2u;
				OBJECT(holder)->state = 0u;
				OBJECT(holder)->height_whole = OBJECT(holder)->floor_height;
				OBJECT(holder)->enemy.move_time = 0u;
				OBJECT(holder)->flags |= 0x80u;
				OBJECT(object)->turtle.escape_presses = 0u;
				OBJECT(object)->turtle.reaction_step++;
				OBJECT(object)->turtle.wait_timer = 0x60u;
				routine_state = 0x411u;
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
		}
		if (PLAYER_INPUT(presses)->bits == 0u)
		{
			wait_timer = OBJECT(object)->turtle.wait_timer;
			OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
			if (wait_timer != 1u || OBJECT(object)->state == 0x24u)
				return;
			routine_state = 0x424u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
		OBJECT(object)->turtle.wait_timer = 0x10u;
		OBJECT(object)->turtle.escape_presses++;
		if (OBJECT(object)->turtle.escape_presses < 8u)
		{
			if (OBJECT(object)->state == 0x27u)
				return;
			routine_state = 0x427u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			return;
		}
	}
	turtle_erase_prompt(object);
	kind = OBJECT(object)->kind;
	job_2 = (uint16_t)(turtle_struggle_bits[kind] + 0x400u);
	queue_job(job_2);
	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->turtle.escape_presses = 0u;
		OBJECT(object)->turtle.reaction_step++;
		OBJECT(object)->turtle.wait_timer = 0x60u;
		routine_state = 0x411u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	holder = OBJECT(object)->turtle.hit_by;
	if (OBJECT(holder)->routine == 5u)
	{
		routine_state = 0u;
		OBJECT(object)->turtle.escape_presses = 0u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	OBJECT(holder)->routine = 5u;
	OBJECT(holder)->state = 0u;
	OBJECT(holder)->enemy.move_time = 0u;
	OBJECT(holder)->enemy.move = 0x94u;
	facing = 0u;
	if ((int8_t)OBJECT(object)->turtle.knockback_flags >= 0)
		facing = 0x80u;
	OBJECT(holder)->enemy.facing = facing;
	routine_state = 0u;
	OBJECT(object)->turtle.escape_presses = 0u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* knocked into the air until it lands */
void turtle_hit_fly(uint32_t object)
{
	uint16_t slowdown = 0;

	slowdown = 0x50u;
	offset_high_word = 0x50u;
	turtle_knockback_arc(slowdown, object);
	if (OBJECT(object)->turtle.falling == 0u || (OBJECT(object)->contacts & 1u) == 0u)
		return;
	OBJECT(object)->turtle.reaction_step = 0xFFu;
}

/* the prompt to press the buttons drawn blinking near the held turtle */
void turtle_show_prompt(uint32_t prompt_place, uint16_t prompt_y, uint32_t object)
{
	uint16_t count = 0, count_2 = 0;
	uint32_t tile_place = 0, tiles = 0;
	uint16_t t1 = 0, t2 = 0;

	if (game_on == 0u || prompt_shown(OBJECT(object)->kind) != 0u)
		return;
	tiles = prompt_tiles_ADDRESS;
	if (OBJECT(object)->turtle.prompt_frame != 0u)
		tiles = prompt_tiles_2_ADDRESS;
	OBJECT(object)->turtle.prompt_time--;
	if ((OBJECT(object)->turtle.prompt_time & 0x7Fu) == 0u)
	{
		OBJECT(object)->turtle.prompt_frame ^= 0xFFu;
		OBJECT(object)->turtle.prompt_time = 0xC0u;
		return;
	}
	if (OBJECT(object)->turtle.prompt_time == 0x3Fu)
	{
		prompt_place = ((prompt_place - 0x20u) & 0x1F8u) >> 2;
		if (prompt_y < 0x70u)
			prompt_y = (uint16_t)(((0u - prompt_y - 0x50u) & 0xF8u) << 4);
		else
			prompt_y = 0x400u;
		prompt_place = (uint16_t)(prompt_place + prompt_y);
		OBJECT(object)->turtle.prompt_place = prompt_place;
		tile_place = prompt_place + 0x100000u;
	}
	else
	{
		if (OBJECT(object)->turtle.prompt_time != 0xBFu)
			return;
		tile_place = 0x100000u + (int16_t)OBJECT(object)->turtle.prompt_place;
	}
	count = 3u;
	offset_high_word = 3u;
	do
	{
		count_2 = 9u;
		do
		{
			io_write8(tile_place, PROMPT_BYTE(tiles));
			tiles++;
			tile_place++;
			io_write8(tile_place, PROMPT_BYTE(tiles));
			t1 = count_2 - 1u;
			count_2 = t1;
			tiles++;
			tile_place++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x6Cu;
	} while (t2 != 0xFFFFu);
}

/* the prompt erased */
void turtle_erase_prompt(uint32_t object)
{
	uint8_t blank_attribute = 0, space_tile = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	if (game_on == 0u)
		return;
	tile_place = 0x100000u + (int16_t)OBJECT(object)->turtle.prompt_place;
	blank_attribute = 0u;
	space_tile = 0x10u;
	count = 3u;
	offset_high_word = 3u;
	do
	{
		count_2 = 9u;
		do
		{
			io_write8(tile_place, blank_attribute);
			tile_place++;
			io_write8(tile_place, space_tile);
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x6Cu;
	} while (t2 != 0xFFFFu);
	prompt_shown(OBJECT(object)->kind) = 0xFFu;
}

/* the turtle's health bars kept for its player and shown again */
void turtle_save_health(uint32_t job, uint32_t object)
{
	PLAYER(PLAYER_ADDRESS(0) + (OBJECT(object)->kind) * 0x8u)->health_bars = OBJECT(object)->health_bars;
	job = (job & ~0xFFFFu) | 0xB20u | shl32(1u, (int)(OBJECT(object)->kind & 0x1Fu));
	offset_high_word = job;
	queue_job((uint16_t)job);
}

/* the turtle on a knock's arc across and through the air, until it lands */
void CONSOLE_ORIGINAL(turtle_knockback_arc)(uint16_t jump_speed, uint32_t object)
{
	uint16_t jump_speed_2 = 0;
	uint16_t height = 0, jump_speed_3 = 0;

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
			if (OBJECT(object)->x < 0x9000u)
				OBJECT(object)->x = 0x9000u;
			OBJECT(object)->turtle.drift_speed -= jump_speed;
			if (OBJECT(object)->turtle.drift_speed < 0x200u)
				OBJECT(object)->turtle.drift_speed = 0x200u;
		}
	}
	else if ((OBJECT(object)->contacts & 8u) == 0u)
	{
		OBJECT(object)->x += OBJECT(object)->turtle.drift_speed;
		if (OBJECT(object)->x < 0x16000u)
		{
			OBJECT(object)->turtle.drift_speed -= jump_speed;
			if (OBJECT(object)->turtle.drift_speed < 0x200u)
				OBJECT(object)->turtle.drift_speed = 0x200u;
		}
		else
		{
			OBJECT(object)->x = 0x16000u;
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

/* Unused: the turtle fires a gun */
void turtle_fire_gun(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	turtles_busy--;
	if (OBJECT(object)->frame_time == 0u)
	{
		turtle_attack_over(held, pressed, object, inputs, presses);
		return;
	}
	PLAYER_INPUT(inputs)->bits &= 0xCu;
}

/* on another object: whether the turtle stands on it or is in the air */
void turtle_floor_contact(uint8_t new_contacts, uint32_t object)
{
	if (OBJECT(object)->height_whole != OBJECT(object)->floor_height)
		new_contacts |= 0x80u;
	else if (floor_cell == 1u)
		new_contacts |= 1u;
	OBJECT(object)->contacts |= new_contacts;
}

/* no stick: the floor and walls around the turtle, or off its floor cell a step by the side it was
   knocked to */
void turtle_walls_still(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t height_whole = 0, new_contacts = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	if (floor_cell != 1u)
	{
		if ((OBJECT(object)->turtle.knockback_flags & 1u) == 0u)
		{
			turtle_leave_floor_right(new_contacts, object, floor_cell_place);
			return;
		}
		turtle_leave_floor_left(new_contacts, object, floor_cell_place);
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
		turtle_wall_contacts(new_contacts, object);
		return;
	}
	if (OBJECT(object)->floor_height <= height_whole)
	{
		new_contacts |= 0x80u;
		turtle_wall_contacts(new_contacts, object);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	new_contacts |= 1u;
	turtle_wall_contacts(new_contacts, object);
}

/* stick up: the floor and walls around the turtle; off its floor cell, its floor raised */
void turtle_walls_up(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t height_whole = 0, new_contacts = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	if (floor_cell != 1u)
	{
		turtle_raise_floor(new_contacts, object, floor_cell_place);
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
		turtle_wall_contacts(new_contacts, object);
		return;
	}
	if (OBJECT(object)->floor_height <= height_whole)
	{
		new_contacts |= 0x80u;
		turtle_wall_contacts(new_contacts, object);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	new_contacts |= 1u;
	turtle_wall_contacts(new_contacts, object);
}

/* the walls around the turtle from the floor map's cells around it */
void turtle_wall_contacts(uint8_t new_contacts, uint32_t object)
{
	uint8_t floor_beside = 0, height_beside = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	height_beside = height_cell_up;
	t1 = floor_cell_up;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 2u;
	t2 = floor_cell_right;
	floor_beside = t2;
	if ((int8_t)t2 < 0 && (floor_beside == 0xFEu || (floor_beside == 0xFBu && new_contacts == 0xFBu)))
		new_contacts |= 8u;
	height_beside = height_cell_left;
	t3 = floor_cell_left;
	floor_beside = t3;
	if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_beside != 0xFAu)))
	{
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	new_contacts |= 0x10u;
	turtle_corner_contacts(new_contacts, object);
}

/* the turtle steps up a level of the floor map */
void turtle_raise_floor(uint8_t new_contacts, uint32_t object, uint32_t floor_cell_place)
{
	uint8_t max_rows = 0, reset_contacts = 0, rows = 0;

	if (OBJECT(object)->y < 0x6800u)
	{
		OBJECT(object)->y += 0x400u;
		OBJECT(object)->screen_y = OBJECT(object)->y >> 8;
	}
	rows = 0u;
	max_rows = 0x14u;
	do
	{
		OBJECT(object)->floor_height += 8u;
		floor_cell_place -= 0x40u;
		if (floor_cell_place < floor_map_ADDRESS)
			floor_cell_place += 0x800u;
		rows++;
		if (max_rows <= rows)
		{
			OBJECT(object)->x = 0xC000u;
			OBJECT(object)->y = 0x2000u;
			reset_contacts = 0x7Bu;
			OBJECT(object)->floor_height = 0u;
			OBJECT(object)->contacts |= reset_contacts;
			return;
		}
	} while ((int8_t)FLOOR_MAP_CELL(floor_cell_place)->cell < 0);
	OBJECT(object)->y = (OBJECT(object)->y & 0xFFFFFF00u) | 0xFFu;
	new_contacts |= 0x82u;
	if (OBJECT(object)->floor_height <= OBJECT(object)->height_whole)
	{
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	new_contacts |= 1u;
	turtle_corner_contacts(new_contacts, object);
}

/* stick down: the floor and walls around the turtle; off its floor cell, its floor lowered and the
   turtle moved down */
void turtle_walls_down(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t height_whole = 0, new_contacts = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	if ((int8_t)floor_cell_up < 0)
		new_contacts |= 2u;
	if (floor_cell != 1u)
	{
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->y -= 0x400u;
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
		turtle_wall_contacts(new_contacts, object);
		return;
	}
	if (OBJECT(object)->floor_height <= height_whole)
	{
		new_contacts |= 0x80u;
		turtle_wall_contacts(new_contacts, object);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	new_contacts |= 1u;
	turtle_wall_contacts(new_contacts, object);
}

/* the turtle steps down a level of the floor map */
void turtle_lower_floor(uint8_t new_contacts, uint32_t object, uint32_t floor_cell_place)
{
	uint8_t max_rows = 0, reset_contacts = 0, rows = 0;
	uint8_t floor_height = 0;

	rows = 0u;
	max_rows = 0x14u;
	for (;;)
	{
		floor_height = OBJECT(object)->floor_height;
		OBJECT(object)->floor_height = floor_height - 8u;
		if (floor_height < 8u)
			OBJECT(object)->floor_height = 0u;
		floor_cell_place += 0x40u;
		if (floor_cell_place >= OBJECT_ADDRESS(0))
			floor_cell_place -= 0x800u;
		rows++;
		if (max_rows <= rows)
			break;
		if ((int8_t)FLOOR_MAP_CELL(floor_cell_place)->cell >= 0)
		{
			OBJECT(object)->y &= 0xFFFFFF00u;
			new_contacts |= 0x82u;
			turtle_corner_contacts(new_contacts, object);
			return;
		}
	}
	OBJECT(object)->x = 0xC000u;
	OBJECT(object)->y = 0x2000u;
	reset_contacts = 0x7Bu;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->contacts |= reset_contacts;
}

/* stick right, or a missile's flight: the walls above and to the sides; off its floor cell, as leaving
   it to the right */
void turtle_walls_right(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_beside = 0, height_beside = 0, height_whole = 0, new_contacts = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	height_beside = height_cell_up;
	t1 = floor_cell_up;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 2u;
	if (floor_cell != 1u)
	{
		turtle_leave_floor_right(new_contacts, object, floor_cell_place);
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
	}
	else
	{
		if (height_whole < OBJECT(object)->floor_height)
		{
			OBJECT(object)->height_whole = OBJECT(object)->floor_height;
			OBJECT(object)->height &= 0xFF00u;
			new_contacts |= 1u;
			t2 = floor_cell_right;
			floor_beside = t2;
			if ((int8_t)t2 >= 0)
			{
				t3 = floor_cell_left;
				floor_beside = t3;
				if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
				{
					turtle_corner_contacts(new_contacts, object);
					return;
				}
				new_contacts |= 0x10u;
				turtle_corner_contacts(new_contacts, object);
				return;
			}
			if (floor_beside == 0xFEu)
			{
				new_contacts |= 8u;
				t3 = floor_cell_left;
				floor_beside = t3;
				if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
				{
					turtle_corner_contacts(new_contacts, object);
					return;
				}
				new_contacts |= 0x10u;
				turtle_corner_contacts(new_contacts, object);
				return;
			}
			if (floor_beside != 0xFBu)
			{
				t3 = floor_cell_left;
				floor_beside = t3;
				if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
				{
					turtle_corner_contacts(new_contacts, object);
					return;
				}
				new_contacts |= 0x10u;
				turtle_corner_contacts(new_contacts, object);
				return;
			}
			t4 = height_cell_right;
			floor_beside = t4;
			if ((int8_t)t4 >= 0 || floor_beside != 0xFBu)
			{
				t3 = floor_cell_left;
				floor_beside = t3;
				if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
				{
					turtle_corner_contacts(new_contacts, object);
					return;
				}
				new_contacts |= 0x10u;
				turtle_corner_contacts(new_contacts, object);
				return;
			}
			new_contacts |= 8u;
			t3 = floor_cell_left;
			floor_beside = t3;
			if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
			{
				turtle_corner_contacts(new_contacts, object);
				return;
			}
			new_contacts |= 0x10u;
			turtle_corner_contacts(new_contacts, object);
			return;
		}
		new_contacts |= 0x80u;
	}
	t2 = floor_cell_right;
	floor_beside = t2;
	if ((int8_t)t2 >= 0)
	{
		t3 = floor_cell_left;
		floor_beside = t3;
		if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
		{
			turtle_corner_contacts(new_contacts, object);
			return;
		}
		new_contacts |= 0x10u;
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	if (floor_beside == 0xFEu)
	{
		new_contacts |= 8u;
		t3 = floor_cell_left;
		floor_beside = t3;
		if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
		{
			turtle_corner_contacts(new_contacts, object);
			return;
		}
		new_contacts |= 0x10u;
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	if (floor_beside != 0xFBu)
	{
		t3 = floor_cell_left;
		floor_beside = t3;
		if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
		{
			turtle_corner_contacts(new_contacts, object);
			return;
		}
		new_contacts |= 0x10u;
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	t4 = height_cell_right;
	floor_beside = t4;
	if ((int8_t)t4 >= 0 || floor_beside != 0xFBu)
	{
		t3 = floor_cell_left;
		floor_beside = t3;
		if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
		{
			turtle_corner_contacts(new_contacts, object);
			return;
		}
		new_contacts |= 0x10u;
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	new_contacts |= 8u;
	t3 = floor_cell_left;
	floor_beside = t3;
	if ((int8_t)t3 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
	{
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	new_contacts |= 0x10u;
	turtle_corner_contacts(new_contacts, object);
}

/* off its floor cell going right: a step up or down, or pushed back left against every wall */
void turtle_leave_floor_right(uint8_t new_contacts, uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_cell_here = 0;
	uint32_t x_step = 0;

	if (OBJECT(object)->height_whole != OBJECT(object)->floor_height)
	{
		floor_cell_here = floor_cell;
		if (floor_cell_here == 0xFBu || floor_cell_here == 0xFCu)
		{
			turtle_raise_floor(new_contacts, object, floor_cell_place);
			return;
		}
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->x += 0x400u;
		return;
	}
	if (floor_cell == 0xFAu || floor_cell == 0xFCu || floor_cell == 0xFFu)
	{
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->x += 0x400u;
		return;
	}
	x_step = reset_ADDRESS;
	if (OBJECT(object)->routine == 4u && OBJECT(object)->turtle.reaction_variant >= 2u)
		x_step = 0x1800u;
	OBJECT(object)->x -= x_step;
	new_contacts |= 0x7Bu;
	OBJECT(object)->contacts |= new_contacts;
}

/* stick left: the walls above and to the sides; off its floor cell, as leaving it to the left */
void turtle_walls_left(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_beside = 0, height_beside = 0, height_whole = 0, new_contacts = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	height_beside = height_cell_up;
	t1 = floor_cell_up;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 2u;
	if (floor_cell != 1u)
	{
		turtle_leave_floor_left(new_contacts, object, floor_cell_place);
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
	}
	else
	{
		if (height_whole < OBJECT(object)->floor_height)
		{
			OBJECT(object)->height_whole = OBJECT(object)->floor_height;
			OBJECT(object)->height &= 0xFF00u;
			new_contacts |= 1u;
			t2 = floor_cell_left;
			floor_beside = t2;
			if ((int8_t)t2 < 0 && (floor_beside == 0xFDu || (floor_beside == 0xFAu && height_cell_left == 0xFAu)))
				new_contacts |= 0x10u;
			t3 = floor_cell_right;
			floor_beside = t3;
			if ((int8_t)t3 >= 0 || (floor_beside != 0xFEu && (floor_beside != 0xFBu || height_cell_right != 0xFBu)))
			{
				turtle_corner_contacts(new_contacts, object);
				return;
			}
			new_contacts |= 8u;
			turtle_corner_contacts(new_contacts, object);
			return;
		}
		new_contacts |= 0x80u;
	}
	t2 = floor_cell_left;
	floor_beside = t2;
	if ((int8_t)t2 < 0 && (floor_beside == 0xFDu || (floor_beside == 0xFAu && height_cell_left == 0xFAu)))
		new_contacts |= 0x10u;
	t3 = floor_cell_right;
	floor_beside = t3;
	if ((int8_t)t3 >= 0 || (floor_beside != 0xFEu && (floor_beside != 0xFBu || height_cell_right != 0xFBu)))
	{
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	new_contacts |= 8u;
	turtle_corner_contacts(new_contacts, object);
}

/* off its floor cell going left: a step up or down, or pushed back right against every wall */
void turtle_leave_floor_left(uint8_t new_contacts, uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_cell_here = 0;
	uint32_t x_step = 0;

	if (OBJECT(object)->height_whole != OBJECT(object)->floor_height)
	{
		floor_cell_here = floor_cell;
		if (floor_cell_here == 0xFAu || floor_cell_here == 0xFCu)
		{
			turtle_raise_floor(new_contacts, object, floor_cell_place);
			return;
		}
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->x -= 0x400u;
		return;
	}
	if (floor_cell == 0xFBu || floor_cell == 0xFCu || floor_cell == 0xFFu)
	{
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->x -= 0x400u;
		return;
	}
	x_step = reset_ADDRESS;
	if (OBJECT(object)->routine == 4u && OBJECT(object)->turtle.reaction_variant >= 2u)
		x_step = 0x1800u;
	OBJECT(object)->x += x_step;
	new_contacts |= 0x7Bu;
	OBJECT(object)->contacts |= new_contacts;
}

/* stick up and left: the diagonal's walls, or off its floor cell as going left */
void turtle_walls_up_left(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t new_contacts = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	if (floor_cell != 1u)
	{
		turtle_leave_floor_left(new_contacts, object, floor_cell_place);
		return;
	}
	turtle_walls_up_diagonal(new_contacts, object);
}

/* stick up and right: the diagonal's walls, or off its floor cell as going right */
void turtle_walls_up_right(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t new_contacts = 0;

	new_contacts = 0u;
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	if (floor_cell != 1u)
	{
		turtle_leave_floor_right(new_contacts, object, floor_cell_place);
		return;
	}
	turtle_walls_up_diagonal(new_contacts, object);
}

/* the walls going up on a diagonal: on its floor or in the air, the wall above, the sides and the
   corners */
void turtle_walls_up_diagonal(uint8_t new_contacts, uint32_t object)
{
	uint8_t floor_beside = 0, height_beside = 0, height_whole = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
	}
	else if (OBJECT(object)->floor_height <= height_whole)
		new_contacts |= 0x80u;
	else
	{
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
	}
	t1 = floor_cell_up;
	floor_beside = t1;
	if ((int8_t)t1 < 0)
	{
		if (floor_beside >= 0xFDu)
			new_contacts |= 2u;
		else
		{
			t2 = height_cell_up;
			height_beside = t2;
			if ((int8_t)t2 < 0 && height_beside < 0xFDu)
			{
				if (floor_beside != 0xFCu)
				{
					if (floor_beside != 0xFBu)
					{
						if (floor_beside == 0xFAu && height_beside == 0xFAu)
						{
							new_contacts |= 2u;
							t3 = floor_cell_right;
							floor_beside = t3;
							if ((int8_t)t3 < 0 && (floor_beside == 0xFEu || (floor_beside == 0xFBu && height_cell_right == 0xFBu)))
								new_contacts |= 8u;
							t4 = floor_cell_left;
							floor_beside = t4;
							if ((int8_t)t4 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
							{
								turtle_corner_contacts(new_contacts, object);
								return;
							}
							new_contacts |= 0x10u;
							turtle_corner_contacts(new_contacts, object);
							return;
						}
					}
					else if (height_beside == 0xFBu)
					{
						new_contacts |= 2u;
						t3 = floor_cell_right;
						floor_beside = t3;
						if ((int8_t)t3 < 0 && (floor_beside == 0xFEu || (floor_beside == 0xFBu && height_cell_right == 0xFBu)))
							new_contacts |= 8u;
						t4 = floor_cell_left;
						floor_beside = t4;
						if ((int8_t)t4 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
						{
							turtle_corner_contacts(new_contacts, object);
							return;
						}
						new_contacts |= 0x10u;
						turtle_corner_contacts(new_contacts, object);
						return;
					}
				}
				else if (height_beside == 0xFCu)
				{
					new_contacts |= 2u;
					t3 = floor_cell_right;
					floor_beside = t3;
					if ((int8_t)t3 < 0 && (floor_beside == 0xFEu || (floor_beside == 0xFBu && height_cell_right == 0xFBu)))
						new_contacts |= 8u;
					t4 = floor_cell_left;
					floor_beside = t4;
					if ((int8_t)t4 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
					{
						turtle_corner_contacts(new_contacts, object);
						return;
					}
					new_contacts |= 0x10u;
					turtle_corner_contacts(new_contacts, object);
					return;
				}
			}
		}
	}
	t3 = floor_cell_right;
	floor_beside = t3;
	if ((int8_t)t3 < 0 && (floor_beside == 0xFEu || (floor_beside == 0xFBu && height_cell_right == 0xFBu)))
		new_contacts |= 8u;
	t4 = floor_cell_left;
	floor_beside = t4;
	if ((int8_t)t4 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
	{
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	new_contacts |= 0x10u;
	turtle_corner_contacts(new_contacts, object);
}

/* the walls at the upper corners, then all the walls into the turtle's contacts */
void turtle_corner_contacts(uint8_t new_contacts, uint32_t object)
{
	uint8_t floor_beside = 0, height_beside = 0;
	uint8_t t1 = 0, t2 = 0;

	height_beside = height_cell_up_right;
	t1 = floor_cell_up_right;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 0x20u;
	height_beside = height_cell_up_left;
	t2 = floor_cell_up_left;
	floor_beside = t2;
	if ((int8_t)t2 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 0x40u;
	OBJECT(object)->contacts |= new_contacts;
}

/* stick down and right: the side walls; off its floor cell, a step up, or down a level moving down and
   right */
void turtle_walls_down_right(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_beside = 0, height_beside = 0, height_whole = 0, new_contacts = 0;
	uint8_t t1 = 0, floor_height = 0;

	new_contacts = 0u;
	if (floor_cell == 0u)
	{
		OBJECT(object)->contacts |= new_contacts;
		return;
	}
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	height_beside = height_cell_up;
	t1 = floor_cell_up;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 2u;
	if (floor_cell != 1u)
	{
		if (floor_cell == 0xFBu)
		{
			turtle_raise_floor(new_contacts, object, floor_cell_place);
			return;
		}
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->x += 0x400u;
		OBJECT(object)->y -= 0x400u;
		OBJECT(object)->height += 0x400u;
		floor_height = OBJECT(object)->floor_height;
		OBJECT(object)->floor_height = floor_height - 4u;
		if (floor_height < 4u)
			OBJECT(object)->floor_height = 0u;
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
		turtle_side_contacts(new_contacts, object);
		return;
	}
	if (OBJECT(object)->floor_height <= height_whole)
	{
		new_contacts |= 0x80u;
		turtle_side_contacts(new_contacts, object);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	new_contacts |= 1u;
	turtle_side_contacts(new_contacts, object);
}

/* the walls to the right and left, and the corners */
void turtle_side_contacts(uint8_t new_contacts, uint32_t object)
{
	uint8_t floor_beside = 0;
	uint8_t t1 = 0, t2 = 0;

	t1 = floor_cell_right;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside == 0xFEu || (floor_beside == 0xFBu && height_cell_right == 0xFBu)))
		new_contacts |= 8u;
	t2 = floor_cell_left;
	floor_beside = t2;
	if ((int8_t)t2 >= 0 || (floor_beside != 0xFDu && (floor_beside != 0xFAu || height_cell_left != 0xFAu)))
	{
		turtle_corner_contacts(new_contacts, object);
		return;
	}
	new_contacts |= 0x10u;
	turtle_corner_contacts(new_contacts, object);
}

/* stick down and left: the side walls; off its floor cell, a step up, or down a level moving down and
   left */
void turtle_walls_down_left(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_beside = 0, height_beside = 0, height_whole = 0, new_contacts = 0;
	uint8_t t1 = 0, floor_height = 0;

	new_contacts = 0u;
	if (floor_cell == 0u)
	{
		OBJECT(object)->contacts |= new_contacts;
		return;
	}
	if ((OBJECT(object)->flags & 0x10u) != 0u)
	{
		turtle_floor_contact(new_contacts, object);
		return;
	}
	height_beside = height_cell_up;
	t1 = floor_cell_up;
	floor_beside = t1;
	if ((int8_t)t1 < 0 && (floor_beside >= 0xFDu || height_beside == floor_beside))
		new_contacts |= 2u;
	if (floor_cell != 1u)
	{
		if (floor_cell == 0xFAu)
		{
			turtle_raise_floor(new_contacts, object, floor_cell_place);
			return;
		}
		turtle_lower_floor(new_contacts, object, floor_cell_place);
		OBJECT(object)->x -= 0x400u;
		OBJECT(object)->y -= 0x400u;
		OBJECT(object)->height += 0x400u;
		floor_height = OBJECT(object)->floor_height;
		OBJECT(object)->floor_height = floor_height - 4u;
		if (floor_height < 4u)
			OBJECT(object)->floor_height = 0u;
		return;
	}
	height_whole = OBJECT(object)->height_whole;
	if (height_whole == OBJECT(object)->floor_height)
	{
		OBJECT(object)->height &= 0xFF00u;
		new_contacts |= 1u;
		turtle_side_contacts(new_contacts, object);
		return;
	}
	if (OBJECT(object)->floor_height <= height_whole)
	{
		new_contacts |= 0x80u;
		turtle_side_contacts(new_contacts, object);
		return;
	}
	OBJECT(object)->height_whole = OBJECT(object)->floor_height;
	OBJECT(object)->height &= 0xFF00u;
	new_contacts |= 1u;
	turtle_side_contacts(new_contacts, object);
}
