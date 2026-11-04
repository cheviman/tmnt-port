/**
 *    Name:        scenes/area_exits.c
 *    Description: the areas' exits: the turtles walk out, and the cutscenes between the areas (Shredder takes April, the villains' picture, the van, April's thanks, the van's crash, Splinter freed)
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the area is cleared: the turtles start walking out */
void turtle_start_area_exit(void)
{
	uint16_t cleared = 0;
	uint32_t marker_time = 0, object = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	object = OBJECT_ADDRESS(40);
	marker_time = marker_times_ADDRESS;
	cleared = 0u;
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			OBJECT(turtle)->flags |= 0x80u;
			OBJECT(object)->flags = (uint8_t)cleared;
			MARKER_TIMER(marker_time)->frames = (uint8_t)cleared;
			OBJECT(turtle)->turtle.invulnerable_time = (uint8_t)cleared;
		}
		turtle += 0x50u;
		marker_time++;
		object += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	time_since_balloon = cleared;
	balloons_said = (uint8_t)cleared;
	balloon_time = (uint8_t)cleared;
	cutscene_step = 0u;
	switch (stage)
	{
	case 0:
		turtle_exit_building_start();
		return;
	case 1:
		turtle_exit_apartment_start();
		return;
	case 2:
		turtle_exit_street_start();
		return;
	case 3:
		turtle_exit_sewer_start();
		return;
	case 4:
		turtle_exit_car_street_start();
		return;
	case 5:
		turtle_exit_highway_start();
		return;
	case 6:
		exit_skateboard_start();
		return;
	case 7:
		turtle_exit_building_site_start();
		return;
	case 8:
		turtle_exit_technodrome_start();
		return;
	case 9:
		turtle_exit_final_start();
		return;
	}
}

/* area 0's exit starts: each turtle walks, facing right */
void turtle_exit_building_start(void)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			OBJECT(turtle)->flags &= 0xBFu;
			OBJECT(turtle)->turtle.exit_step = 0u;
			routine_state = 0x100u;
			if (OBJECT(turtle)->y < 0x5000u)
				routine_state++;
			turtle_set_state(routine_state, turtle, &ignored, &ignored_2, &ignored_3);
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	cutscene_counter = 0u;
}

/* area 1's exit starts: each turtle stands, and the exit waits 32 frames */
void turtle_exit_apartment_start(void)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;

	cutscene_wait = 0x20u;
	balloons_said = 2u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			routine_state = 0u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
}

/* area 2's exit starts: each turtle walks, facing the spot where they jump, with a sound */
void turtle_exit_street_start(void)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			OBJECT(turtle)->flags &= 0xBFu;
			OBJECT(turtle)->turtle.exit_step = 0u;
			if (OBJECT(turtle)->x >= 0x13C00u)
				OBJECT(turtle)->flags |= 0x40u;
			routine_state = 0x100u;
			turtle_set_state(routine_state, turtle, &ignored, &ignored_2, &ignored_3);
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	cutscene_time = 0xA0u;
	sound = 0x73u;
	queue_sound(sound);
}

/* area 3's exit starts: each turtle walks left, without its shadow, with a sound */
void turtle_exit_sewer_start(void)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, turtle = 0;

	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			OBJECT(turtle)->flags |= 0x40u;
			OBJECT(turtle)->contacts &= 0x7Fu;
			OBJECT(turtle)->turtle.exit_step = 0u;
			routine_state = 0x100u;
			if (OBJECT(turtle)->screen_y < 0x50u)
				routine_state++;
			turtle_set_state(routine_state, turtle, &ignored, &ignored_2, &ignored_3);
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	sound = 0x6Eu;
	queue_sound(sound);
}

/* area 4's exit starts: the area's objects gone, each turtle walks toward its place, a cheer, the
   layers' priority */
void turtle_exit_car_street_start(void)
{
	uint16_t routine_state = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0, turtle = 0, turtle_x = 0;

	object = OBJECT_ADDRESS(24);
	do
	{
		OBJECT(object)->flags = 0u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(32));
	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			OBJECT(turtle)->flags &= 0xBFu;
			turtle_x = OBJECT(turtle)->x >> 8;
			screen_x_high = turtle_x;
			if (car_street_exit_x[OBJECT(turtle)->kind * 2u] < (uint16_t)turtle_x)
				OBJECT(turtle)->flags |= 0x40u;
			routine_state = 0x100u;
			OBJECT(turtle)->turtle.exit_step = 0u;
			if (OBJECT(turtle)->y < 0x4000u)
				routine_state++;
			turtle_set_state(routine_state, turtle, &ignored, &ignored_2, &ignored_3);
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	object = OBJECT_ADDRESS(44);
	animation = turtle_cheer_animation_ADDRESS;
	set_animation(ignored, object, animation);
	OBJECT(OBJECT_ADDRESS(40))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(41))->colour = 0xFFu;
	layer_priority = 4u;
	io_write8(PRIORITY_0C0001, 4u);
}

/* area 5's exit starts: each turtle crouches for a jump, and the music fades out */
void turtle_exit_highway_start(void)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;

	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			OBJECT(object)->flags &= 0xBFu;
			OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0x10u;
			routine_state = 0x200u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	sound = 0xFEu;
	queue_sound(sound);
}

/* the ride's exit starts: the van's colours and the road going round */
void exit_skateboard_start(void)
{
	uint16_t kind = 0;
	uint32_t data = 0;

	kind = 0x43Bu;
	data = skateboard_exit_van_palette_ADDRESS;
	post_palette_request(kind, data);
	kind = 0x43Cu;
	data = skateboard_exit_van_palette_2_ADDRESS;
	post_palette_request(kind, data);
	exit_skateboard_road_round();
}

/* area 7's exit starts: the area's objects gone, Splinter shown tied up, each turtle in its walk held, a
   sound */
void turtle_exit_building_site_start(void)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;

	object = OBJECT_ADDRESS(24);
	do
	{
		OBJECT(object)->flags = 0u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(32));
	object = OBJECT_ADDRESS(44);
	animation = splinter_tied_up_animation_ADDRESS;
	set_animation(0u, object, animation);
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			OBJECT(object)->flags &= 0xBFu;
			if (OBJECT(object)->x >= 0x10000u)
				OBJECT(object)->flags |= 0x40u;
			OBJECT(object)->y_speed = 0;
			routine_state = 0x101u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			OBJECT(object)->frame_time = 0u;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	sound = 0x6Eu;
	queue_sound(sound);
}

/* area 8's exit starts: each turtle walks, the layers' priority, a wait of 192 frames, a sound */
void turtle_exit_technodrome_start(void)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;

	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			OBJECT(object)->flags &= 0xBFu;
			routine_state = 0x100u;
			if (OBJECT(object)->y < 0x5000u)
				routine_state++;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	layer_priority = 4u;
	io_write8(PRIORITY_0C0001, 4u);
	cutscene_wait = 0xC0u;
	sound = 0x73u;
	queue_sound(sound);
}

/* area 9's exit starts: a long wait, each turtle's animation held */
void turtle_exit_final_start(void)
{
	uint32_t object = 0;

	cutscene_time = 0xC000u;
	object = OBJECT_ADDRESS(0);
	do
	{
		OBJECT(object)->frame_time = 0u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
}

/* every frame of an area's exit: the balloon timed out, then the stage's exit */
void run_area_exit(void)
{
	uint8_t t1 = 0;

	time_since_balloon++;
	if (balloon_time != 0u)
	{
		t1 = balloon_time;
		balloon_time = t1 - 1u;
		if (t1 == 1u)
			erase_balloon();
	}
	switch (stage)
	{
	case 0:
		turtle_exit_building();
		return;
	case 1:
		run_exit_apartment();
		return;
	case 2:
		turtle_exit_street();
		return;
	case 3:
		run_exit_sewer();
		return;
	case 4:
		run_exit_car_street();
		return;
	case 5:
		turtle_exit_highway();
		return;
	case 6:
		run_exit_skateboard();
		return;
	case 7:
		run_exit_building_site();
		return;
	case 8:
		turtle_exit_technodrome();
		return;
	case 9:
		run_exit_final();
		return;
	}
}

/* area 0's exit: the turtles walk to one spot and vanish there; 32 frames after the last, the exit is
   over */
void turtle_exit_building(void)
{
	uint8_t speed = 0;
	uint16_t screen_x = 0, screen_y = 0, target_x = 0, target_y = 0;
	uint32_t direction = 0, object = 0;
	uint8_t t1 = 0;

	if (cutscene_counter != 0u)
	{
		t1 = cutscene_counter;
		cutscene_counter = t1 - 1u;
		if (t1 == 1u)
		{
			area_beaten = 0u;
			area_cleared = 0u;
			scene_step++;
		}
		return;
	}
	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			screen_x = OBJECT(object)->screen_x;
			screen_x_high = (screen_x_high & ~0xFFFFu) | screen_x;
			if (screen_x < 0x170u)
			{
				turtles_on_the_way--;
				target_x = 0x170u;
				target_y = 0x50u;
				direction = direction_to(direction, target_x, target_y, object);
				speed = 0u;
				screen_x_high = (screen_x_high & ~0xFFFFu) | 0x300u;
				move_in_direction((uint8_t)direction, speed, object);
			}
			else
			{
				screen_y = OBJECT(object)->screen_y;
				if (screen_y >= 0x54u || screen_y < 0x4Cu)
				{
					turtles_on_the_way--;
					target_x = 0x170u;
					target_y = 0x50u;
					direction = direction_to(direction, target_x, target_y, object);
					speed = 0u;
					screen_x_high = (screen_x_high & ~0xFFFFu) | 0x300u;
					move_in_direction((uint8_t)direction, speed, object);
				}
				else
					OBJECT(object)->flags &= 0x7Fu;
			}
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (turtles_on_the_way == 0u)
		cutscene_counter = 0x20u;
}

/* area 1's exit by its steps, April waiting to be rescued in the first ones */
void run_exit_apartment(void)
{
	if (cutscene_step < 3u)
		april_wait_for_rescue();
	switch (cutscene_step)
	{
	case 0:
		exit_apartment_shredder_appears();
		break;
	case 1:
		exit_apartment_shredder_takes_april();
		break;
	case 2:
		exit_apartment_shredder_leaps();
		break;
	case 3:
		exit_apartment_shredder_escapes();
		break;
	case 4:
		exit_apartment_kidnap_picture();
		break;
	case 5:
		exit_apartment_picture_wait();
		break;
	}
	set_layers_scroll();
}

/* the turtles turn to the door and Shredder appears in it */
void exit_apartment_shredder_appears(void)
{
	uint8_t sound = 0;
	uint16_t routine_state = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 != 1u)
		return;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			OBJECT(object)->flags &= 0xBFu;
			if (OBJECT(object)->x >= 0x12C00u)
				OBJECT(object)->flags |= 0x40u;
			routine_state = 0x101u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			OBJECT(object)->frame_time = 0u;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	object = OBJECT_ADDRESS(48);
	OBJECT(OBJECT_ADDRESS(48))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(48))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(48))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(48))->x = 0x12C00u;
	OBJECT(OBJECT_ADDRESS(48))->y = 0x6000u;
	OBJECT(OBJECT_ADDRESS(48))->screen_x = 0x120u;
	OBJECT(OBJECT_ADDRESS(48))->screen_y = 0x60u;
	OBJECT(OBJECT_ADDRESS(48))->height = 0u;
	OBJECT(OBJECT_ADDRESS(48))->floor_height = 0u;
	animation = shredder_appears_animation_ADDRESS;
	set_animation(ignored, object, animation);
	sound = 0xFEu;
	queue_sound(sound);
	cutscene_wait = 0x20u;
	next_area_step();
}

/* the next step of an area's entry or exit */
void next_area_step(void)
{
	cutscene_step++;
}

/* Shredder draws his sword, walks to April and grabs her */
void exit_apartment_shredder_takes_april(void)
{
	uint8_t sound = 0;
	uint16_t screen_x = 0;
	uint32_t animation = 0, object = 0;
	uint8_t t1 = 0;

	object = OBJECT_ADDRESS(48);
	if (cutscene_wait == 0u)
	{
		OBJECT(object)->x -= 0x300u;
		screen_x = (uint16_t)(OBJECT(object)->x >> 8);
		OBJECT(object)->screen_x = screen_x;
		if ((uint16_t)screen_x >= 0xB0u)
			return;
		animation = shredder_carries_april_animation_ADDRESS;
		set_animation(0u, object, animation);
		cutscene_wait = 0x18u;
		next_area_step();
		return;
	}
	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 != 1u)
		return;
	animation = shredder_grabs_april_animation_ADDRESS;
	set_animation(0u, object, animation);
	sound = 0x72u;
	queue_sound(sound);
}

/* April is in Shredder's arms and he leaps away */
void exit_apartment_shredder_leaps(void)
{
	uint32_t object = 0;
	uint8_t t1 = 0;

	object = OBJECT_ADDRESS(48);
	if (OBJECT(OBJECT_ADDRESS(48))->frame_time == 0u)
	{
		OBJECT(object)->x_speed = 0x380;
		OBJECT(object)->y_speed &= 0xFFu;
		next_area_step();
		return;
	}
	if (cutscene_wait != 0u)
	{
		t1 = cutscene_wait;
		cutscene_wait = t1 - 1u;
		if (t1 == 1u)
		{
			OBJECT(OBJECT_ADDRESS(47))->flags = 0u;
			return;
		}
	}
}

/* Shredder bounds off with April, the turtles after him, and the screen is cleared */
void exit_apartment_shredder_escapes(void)
{
	uint8_t speed = 0;
	uint16_t kind = 0, target_x = 0, target_y = 0;
	uint32_t data = 0, direction = 0, object = 0;
	uint16_t x_speed = 0, x_speed_2 = 0;

	object = OBJECT_ADDRESS(48);
	if (OBJECT(OBJECT_ADDRESS(48))->screen_x < 0x178u)
	{
		target_x = 0x178u;
		target_y = 0x70u;
		direction = direction_to(0u, target_x, target_y, object);
		speed = 0u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x300u;
		move_in_direction((uint8_t)direction, speed, object);
		if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
		{
			direction = (uint16_t)OBJECT(object)->x_speed;
			x_speed = OBJECT(object)->x_speed;
			OBJECT(object)->x_speed = x_speed - direction;
			if (x_speed < direction)
				OBJECT(object)->height = 0u;
			OBJECT(object)->x_speed += 0x18u;
		}
		else
		{
			OBJECT(object)->y = (OBJECT(object)->y & 0xFFFF0000u) | ((uint16_t)OBJECT(object)->y + OBJECT(object)->x_speed & 0xFFFFu);
			x_speed_2 = OBJECT(object)->x_speed;
			OBJECT(object)->x_speed = x_speed_2 - 0x10u;
			if (x_speed_2 < 0x10u)
			{
				OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
			}
		}
		object = OBJECT_ADDRESS(0);
		do
		{
			if ((OBJECT(object)->flags & 1u) != 0u)
			{
				if (OBJECT(object)->frame_time == 0u)
					OBJECT(object)->frame_time = 4u;
				target_x = 0x150u;
				target_y = 0x58u;
				direction = direction_to(direction, target_x, target_y, object);
				speed = 0u;
				screen_x_high = (screen_x_high & ~0xFFFFu) | 0x100u;
				move_in_direction((uint8_t)direction, speed, object);
			}
			object += 0x50u;
		} while (object < OBJECT_ADDRESS(4));
		return;
	}
	OBJECT(object)->flags = 0u;
	OBJECT(OBJECT_ADDRESS(0))->flags &= 0x7Fu;
	OBJECT(OBJECT_ADDRESS(1))->flags &= 0x7Fu;
	OBJECT(OBJECT_ADDRESS(2))->flags &= 0x7Fu;
	OBJECT(OBJECT_ADDRESS(3))->flags &= 0x7Fu;
	next_area_step();
	exit_cutscene = 0xFFu;
	LAYER(LAYER_A)->x = 0x800u;
	LAYER(LAYER_B)->x = 0x800u;
	LAYER(WORKING_LAYER)->x = 0x800u;
	LAYER(LAYER_A)->y = 0x10000u;
	LAYER(LAYER_B)->y = 0x10000u;
	LAYER(WORKING_LAYER)->y = 0x10000u;
	clear_layers();
	data = kidnap_picture_palettes_ADDRESS;
	kind = 0x20Fu;
	post_palette_request(kind, data);
}

/* the picture of Shredder carrying April off with the turtles leaping after him */
void exit_apartment_kidnap_picture(void)
{
	uint32_t carry = 0;
	uint8_t sound = 0, tile_attribute = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t animation = 0, animation_entry = 0, animations = 0, object = 0, object_2 = 0, tile_code_2 = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (palettes_busy != 0u)
		return;
	clear_layers();
	tile_place = TILE_RAM + 0x4008u;
	tile_attribute = 0x34u;
	tile_code = 0u;
	count = 0x1Fu;
	do
	{
		count_2 = 0x1Fu;
		do
		{
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, tile_code);
			carry = (uint8_t)(tile_code + 1u) < tile_code;
			tile_code++;
			tile_place++;
			if (carry)
				tile_attribute++;
			t1 = count_2 - 1u;
			count_2 = t1;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x40u;
	} while (t2 != 0xFFFFu);
	io_write8(TILES_106B00, 0xE0u);
	tile_banks_0_1 = 0xE0u;
	tile_place = TILE_RAM + 0x2418u;
	tile_attribute = 0xBu;
	tile_code_2 = 0u;
	count = 0x17u;
	do
	{
		count_2 = 0x17u;
		do
		{
			io_write8(tile_place, tile_attribute);
			tile_place++;
			io_write8(tile_place, tile_code_2);
			carry = (uint8_t)(tile_code_2 + 1u) < tile_code_2;
			tile_code_2 = (uint8_t)(tile_code_2 + 1u);
			tile_place++;
			if (carry)
			{
				tile_attribute++;
				if (tile_attribute == 0xCu)
					tile_attribute = 0x18u;
			}
			t3 = count_2 - 1u;
			count_2 = t3;
		} while (t3 != 0xFFFFu);
		t4 = count - 1u;
		count = t4;
		tile_place += 0x50u;
	} while (t4 != 0xFFFFu);
	io_write8(TILES_106E00, 0x2Du);
	tile_banks_2_3 = 0x2Du;
	animations = kidnap_picture_animations_ADDRESS;
	object = OBJECT_ADDRESS(44);
	object_2 = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object_2)->flags & 1u) != 0u)
		{
			OBJECT(object)->flags = 0x81u;
			OBJECT(object)->height = 0u;
			OBJECT(object)->colour = 0xFFu;
			OBJECT(object)->animation_delay = 0u;
			OBJECT(object)->screen_x = 0x78u;
			OBJECT(object)->screen_y = 0x80u;
			animation_entry = ANIMATION_ENTRY(animations);
			set_animation(tile_code_2, object, animation_entry);
		}
		object_2 += 0x50u;
		animations += 4u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(48));
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->height = 0u;
	OBJECT(object)->colour = 0xFFu;
	OBJECT(object)->animation_delay = 0u;
	OBJECT(object)->screen_x = 0xA7u;
	OBJECT(object)->screen_y = 0x40u;
	animation = april_kidnapped_picture_animation_ADDRESS;
	set_animation(tile_code_2, object, animation);
	sound = 0x40u;
	queue_sound(sound);
	cutscene_time = 0x108u;
	next_area_step();
}

/* the picture stays a while with Shredder's speech, then the exit is over */
void exit_apartment_picture_wait(void)
{
	uint8_t sound = 0;
	uint16_t t1 = 0;

	t1 = cutscene_time;
	cutscene_time = t1 - 1u;
	if (t1 == 1u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
		return;
	}
	if (cutscene_time != 0xF0u)
		return;
	sound = 0xA7u;
	queue_sound(sound);
}

/* area 2's exit: the turtles walk to one place and jump in there */
void turtle_exit_street(void)
{
	uint32_t turtle = 0;
	uint16_t t1 = 0;

	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			switch (OBJECT(turtle)->turtle.exit_step)
			{
			case 0:
				turtle_exit_street_walk(turtle);
				break;
			case 1:
				turtle_exit_street_jump(turtle);
				break;
			case 2:
				turtle_exit_street_vanish(turtle);
				break;
			}
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	t1 = cutscene_time;
	cutscene_time = t1 - 1u;
	if (t1 == 1u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
	}
}

/* the turtle walks to the spot where they jump; near it, it jumps */
void turtle_exit_street_walk(uint32_t object)
{
	uint8_t speed = 0;
	uint16_t routine_state = 0, screen_x = 0, target_x = 0, target_y = 0;
	uint32_t direction = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0;

	turtles_on_the_way--;
	screen_x = OBJECT(object)->screen_x;
	screen_x_high = (screen_x_high & ~0xFFFFu) | screen_x;
	if (screen_x < 0x100u || OBJECT(object)->screen_y >= 0x60u)
	{
		target_x = 0x13Cu;
		target_y = 0x34u;
		direction = direction_to(0u, target_x, target_y, object);
		speed = 0x80u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x280u;
		move_in_direction((uint8_t)direction, speed, object);
		return;
	}
	OBJECT(object)->turtle.jump_speed = 0x700u;
	OBJECT(object)->height = 0u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->turtle.falling = 0u;
	OBJECT(object)->contacts |= 0x80u;
	routine_state = 0x201u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
	OBJECT(object)->turtle.exit_step++;
}

/* a frame of the turtle's jump to the spot; landed on it, its own pose */
void turtle_exit_street_jump(uint32_t object)
{
	uint8_t speed = 0;
	uint16_t target_x = 0, target_y = 0, x_from_spot = 0, y_from_spot = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, routine_state = 0, spot_y = 0;
	uint16_t height = 0, jump_speed = 0, screen_x = 0, screen_y = 0;

	turtles_on_the_way--;
	if (OBJECT(object)->turtle.falling != 0u)
	{
		routine_state = OBJECT(object)->turtle.jump_speed;
		height = OBJECT(object)->height;
		OBJECT(object)->height = height - routine_state;
		if (height < routine_state)
		{
			OBJECT(object)->height = 0u;
			OBJECT(object)->turtle.jump_speed = 0u;
		}
		OBJECT(object)->turtle.jump_speed += 0x50u;
	}
	else
	{
		OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
		jump_speed = OBJECT(object)->turtle.jump_speed;
		OBJECT(object)->turtle.jump_speed = jump_speed - 0x50u;
		if (jump_speed < 0x50u)
		{
			OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
			OBJECT(object)->turtle.falling--;
			routine_state = 0x203u;
			turtle_set_state((uint16_t)routine_state, object, &ignored, &ignored_2, &ignored_3);
		}
	}
	screen_x = OBJECT(object)->screen_x;
	x_from_spot = (uint16_t)(screen_x - 0x139u);
	if (screen_x >= 0x139u && x_from_spot < 7u)
	{
		screen_y = OBJECT(object)->screen_y;
		y_from_spot = (uint16_t)(screen_y - 0x31u);
		if (screen_y >= 0x31u && y_from_spot < 7u)
		{
			if (OBJECT(object)->height != 0u)
			{
				screen_x_high = (screen_x_high & ~0xFF00u) | (x_from_spot & 0xFF00u);
				return;
			}
			OBJECT(object)->x = 0x13C00u;
			OBJECT(object)->y = 0x3400u;
			spot_y = 0x34u;
			OBJECT(object)->screen_x = 0x13Cu;
			OBJECT(object)->screen_y = 0x34u;
			animation = exit_street_jump_animations[OBJECT(object)->kind];
			spot_y = set_animation(spot_y, object, animation);
			OBJECT(object)->turtle.exit_step++;
			screen_x_high = spot_y;
			return;
		}
	}
	target_x = 0x13Cu;
	target_y = 0x34u;
	routine_state = direction_to(routine_state, target_x, target_y, object);
	speed = 0x80u;
	screen_x_high = (screen_x_high & ~0xFFFFu) | 0x280u;
	move_in_direction((uint8_t)routine_state, speed, object);
}

/* the turtle hidden when its pose ends */
void turtle_exit_street_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags &= 0x7Fu;
		return;
	}
	turtles_on_the_way--;
}

/* area 3's exit by its steps */
void run_exit_sewer(void)
{
	uint16_t step_offset = 0;

	step_offset = (uint16_t)((uint32_t)cutscene_step << 2);
	switch (step_offset)
	{
	case 0:
		turtle_exit_sewer();
		break;
	case 4:
		exit_sewer_villains_picture();
		break;
	case 8:
		exit_sewer_villains_taunt(step_offset);
		break;
	}
	set_layers_scroll();
}

/* area 3's exit, first step: each turtle walks and jumps out; when all are gone, the next picture's
   layers and palette */
void turtle_exit_sewer(void)
{
	uint16_t kind = 0;
	uint32_t data = 0, turtle = 0;

	turtles_on_the_way = 0u;
	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			switch (OBJECT(turtle)->turtle.exit_step)
			{
			case 0:
				turtle_exit_sewer_walk(turtle);
				break;
			case 1:
				turtle_exit_sewer_jump(turtle);
				break;
			}
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	if (turtles_on_the_way != 0u)
		return;
	exit_cutscene = 0xFFu;
	LAYER(WORKING_LAYER)->x = 0u;
	LAYER(LAYER_A)->x = 0u;
	LAYER(LAYER_B)->x = 0u;
	LAYER(WORKING_LAYER)->y = 0x10000u;
	LAYER(LAYER_A)->y = 0x10000u;
	LAYER(LAYER_B)->y = 0x10000u;
	clear_layers();
	layer_priority = 0u;
	io_write8(PRIORITY_0C0001, 0u);
	data = villains_picture_palettes_ADDRESS;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	next_area_step();
}

/* the turtle walks left to the edge, then crouches 32 frames */
void turtle_exit_sewer_walk(uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	turtles_on_the_way--;
	if (OBJECT(object)->x >= 0x9000u)
	{
		OBJECT(object)->x -= 0x300u;
		OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
		return;
	}
	OBJECT(object)->turtle.wait_timer = 0x20u;
	routine_state = 0x200u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
	OBJECT(object)->turtle.exit_step++;
}

/* after the crouch, the turtle jumps, going up fast until high above its floor */
void turtle_exit_sewer_jump(uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t wait_timer = 0;

	if (OBJECT(object)->turtle.wait_timer == 0u)
	{
		if (OBJECT(object)->height + OBJECT(object)->y < 0x10000u)
		{
			turtles_on_the_way--;
			OBJECT(object)->height += 0x800u;
			if (OBJECT(object)->height >= 0x4000u)
			{
				routine_state = 0x202u;
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
		}
		return;
	}
	turtles_on_the_way--;
	wait_timer = OBJECT(object)->turtle.wait_timer;
	OBJECT(object)->turtle.wait_timer = wait_timer - 1u;
	if (wait_timer != 1u)
		return;
	routine_state = 0x201u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
}

/* the picture of Shredder, Bebop and Rocksteady with April tied up */
void exit_sewer_villains_picture(void)
{
	uint32_t animation = 0, object = 0, tile_place = 0;

	if (palettes_busy != 0u)
		return;
	io_write8(TILES_106B00, 0x40u);
	tile_banks_0_1 = 0x40u;
	LAYER(WORKING_LAYER)->map = villains_picture_map_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = villains_picture_blocks_ADDRESS;
	LAYER(WORKING_LAYER)->map_width = 0x20u;
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	LAYER(WORKING_LAYER)->map = villains_picture_map_2_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = villains_picture_blocks_2_ADDRESS;
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
	object = OBJECT_ADDRESS(44);
	OBJECT(OBJECT_ADDRESS(44))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(44))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(44))->screen_x = 0xD0u;
	OBJECT(OBJECT_ADDRESS(44))->screen_y = 0xA0u;
	OBJECT(OBJECT_ADDRESS(44))->height = 0u;
	OBJECT(OBJECT_ADDRESS(44))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(44))->animation_delay = 0u;
	animation = april_tied_up_picture_animation_ADDRESS;
	set_animation(0u, object, animation);
	cutscene_time = 0x130u;
	next_area_step();
}

/* Rocksteady taunts the turtles, Shredder speaks, then the exit is over */
void exit_sewer_villains_taunt(uint32_t on_right)
{
	uint8_t screen_x = 0, sound = 0;
	uint16_t line = 0, screen_y = 0;
	uint32_t animation = 0, bank = 0, ignored = 0, ignored_3 = 0, object = 0;
	uint16_t t1 = 0;

	t1 = cutscene_time;
	cutscene_time = t1 - 1u;
	if (t1 == 1u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
		return;
	}
	if (cutscene_time == 0x110u)
	{
		object = OBJECT_ADDRESS(45);
		OBJECT(OBJECT_ADDRESS(45))->flags = 0x81u;
		OBJECT(OBJECT_ADDRESS(45))->contacts = 0u;
		OBJECT(OBJECT_ADDRESS(45))->screen_x = 0x140u;
		OBJECT(OBJECT_ADDRESS(45))->screen_y = 0x58u;
		OBJECT(OBJECT_ADDRESS(45))->height = 0u;
		OBJECT(OBJECT_ADDRESS(45))->colour = 0xFFu;
		OBJECT(OBJECT_ADDRESS(45))->animation_delay = 0u;
		animation = exit_sewer_villains_taunt_animation_ADDRESS;
		set_animation(0u, object, animation);
		on_right &= ~0xFFu;
		screen_x = 0x10u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x110u;
		screen_y = 0x40u;
		bank = 0xFFFFFFD0u;
		line = 0x78u;
		show_balloon(screen_x, screen_y, line, &on_right, &bank, &ignored, &ignored_3);
		draw_wide_balloon_pointer(ignored_3);
		return;
	}
	if (cutscene_time == 0xA0u)
	{
		OBJECT(OBJECT_ADDRESS(45))->flags = 0u;
		return;
	}
	if (cutscene_time != 0x80u)
		return;
	sound = 0xA7u;
	queue_sound(sound);
}

/* area 4's exit by its steps */
void run_exit_car_street(void)
{
	uint16_t step_offset = 0;

	step_offset = (uint16_t)((uint32_t)cutscene_step << 2);
	switch (step_offset)
	{
	case 0:
		turtle_exit_car_street();
		break;
	case 4:
		exit_car_street_van_start_up(step_offset);
		break;
	case 8:
		exit_car_street_van_drives_off();
		break;
	case 12:
		exit_car_street_april_hug_start();
		break;
	case 16:
		exit_car_street_april_hug();
		break;
	}
	set_layers_scroll();
}

/* area 4's exit, first step: the turtles walk to their places and board; when all are waiting, the van
   drives in */
void turtle_exit_car_street(void)
{
	uint32_t animation = 0, object = 0, rear_y = 0, turtle = 0, y = 0;

	turtles_on_the_way = 0u;
	turtles_not_waiting = 0u;
	OBJECT(OBJECT_ADDRESS(40))->car_street_van.once = 0u;
	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u && (OBJECT(turtle)->flags & 0x80u) != 0u)
		{
			switch (OBJECT(turtle)->turtle.exit_step)
			{
			case 0:
				turtle_exit_car_street_wait(turtle);
				break;
			case 1:
				turtle_exit_car_street_board(turtle);
				break;
			case 2:
				turtle_exit_car_street_jump(turtle);
				break;
			}
			update_screen_place(turtle);
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	if (turtles_on_the_way == 0u)
	{
		cutscene_time = 0x97u;
		next_area_step();
		return;
	}
	if (turtles_not_waiting != 0u)
		return;
	OBJECT(OBJECT_ADDRESS(0))->turtle.exit_step = 1u;
	OBJECT(OBJECT_ADDRESS(1))->turtle.exit_step = 1u;
	OBJECT(OBJECT_ADDRESS(2))->turtle.exit_step = 1u;
	OBJECT(OBJECT_ADDRESS(3))->turtle.exit_step = 1u;
	object = OBJECT_ADDRESS(40);
	OBJECT(OBJECT_ADDRESS(40))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(40))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(41))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(41))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(40))->screen_x = 0x50u;
	OBJECT(OBJECT_ADDRESS(41))->screen_x = 0x50u;
	OBJECT(OBJECT_ADDRESS(40))->height = 0u;
	OBJECT(OBJECT_ADDRESS(41))->height = 0u;
	OBJECT(OBJECT_ADDRESS(40))->x = 0x5000u;
	OBJECT(OBJECT_ADDRESS(41))->x = 0x5000u;
	y = 0xE000u;
	if (stage != 5u)
		y = 0xA000u;
	y -= LAYER(LAYER_B)->y & 0xFF00u;
	OBJECT(object)->y = y;
	rear_y = y - 0x400u;
	OBJECT(object + 0x50u)->y = rear_y;
	rear_y = rear_y >> 8;
	OBJECT(object)->screen_y = y >> 8;
	OBJECT(object + 0x50u)->screen_y = rear_y;
	OBJECT(object)->x_speed = 0x6A0;
	animation = turtle_van_animation_ADDRESS;
	rear_y = set_animation(rear_y, object, animation);
	animation = turtle_van_rear_animation_ADDRESS;
	screen_x_high = rear_y;
	object += 0x50u;
	set_animation(rear_y, object, animation);
}

/* the turtle walks to its place, then stands facing its side and waits */
void turtle_exit_car_street_wait(uint32_t object)
{
	uint8_t speed = 0;
	uint16_t routine_state = 0, screen_x = 0, target_x = 0, target_y = 0, x_from_place = 0, y_from_place = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, kind_offset = 0;
	uint16_t screen_y = 0, screen_x_2 = 0;

	turtles_on_the_way--;
	screen_x = car_street_exit_x[OBJECT(object)->kind * 2u];
	screen_x_high = (screen_x_high & ~0xFFFFu) | screen_x;
	screen_y = OBJECT(object)->screen_y;
	y_from_place = (uint16_t)(screen_y - 0x3Cu);
	if (screen_y >= 0x3Cu && y_from_place < 8u)
	{
		screen_x_2 = OBJECT(object)->screen_x;
		x_from_place = (uint16_t)(screen_x_2 - screen_x);
		if (screen_x <= screen_x_2 && x_from_place < 8u)
		{
			OBJECT(object)->turtle.move_timer = 0x10u;
			OBJECT(object)->flags &= 0xBFu;
			if (OBJECT(object)->kind >= 2u)
				OBJECT(object)->flags |= 0x40u;
			routine_state = 0x101u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
			OBJECT(object)->frame_time = 0u;
			return;
		}
	}
	turtles_not_waiting = 0xFFu;
	kind_offset = (uint16_t)((uint32_t)OBJECT(object)->kind << 2);
	target_x = car_street_exit_x[kind_offset / 2u];
	target_y = car_street_exit_y[kind_offset / 2u];
	kind_offset = direction_to(kind_offset, target_x, target_y, object);
	speed = 0u;
	screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
	move_in_direction((uint8_t)kind_offset, speed, object);
}

/* the van slows to a stop with its sounds; then the turtle walks to its door and crouches to jump in */
void turtle_exit_car_street_board(uint32_t object)
{
	uint8_t sound = 0, speed = 0;
	uint16_t routine_state = 0, screen_x = 0, screen_y = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, layer_y = 0, van = 0, van_x = 0, y_from_door = 0;
	uint8_t move_timer = 0;
	uint16_t screen_y_2 = 0, screen_x_2 = 0, x_speed = 0;

	turtles_on_the_way--;
	turtles_not_waiting = 0xFFu;
	van = OBJECT_ADDRESS(40);
	if ((uint16_t)OBJECT(OBJECT_ADDRESS(40))->x_speed == 0u)
	{
		if (OBJECT(van)->x - OBJECT(van + 0x50u)->x < 0x4000u)
		{
			if (OBJECT(van)->car_street_van.once != 0u)
				return;
			OBJECT(van)->car_street_van.once--;
			OBJECT(van + 0x50u)->x -= 0x300u;
			OBJECT(van + 0x50u)->screen_x = OBJECT(van + 0x50u)->x >> 8;
			return;
		}
		if (OBJECT(object)->turtle.move_timer != 0u)
		{
			move_timer = OBJECT(object)->turtle.move_timer;
			OBJECT(object)->turtle.move_timer = move_timer - 1u;
			if (move_timer != 1u)
				return;
			OBJECT(object)->frame_time = 6u;
			return;
		}
		if (stage == 5u)
		{
			layer_y = LAYER(LAYER_B)->y >> 8;
			screen_x_high = layer_y;
			layer_y = layer_y & 0xFFu;
			screen_x_high = (screen_x_high & ~0xFFFFu) | layer_y;
			distance_high_word = 0x40u;
			screen_y = (uint16_t)(0x40u - layer_y + 0x4Du);
		}
		else
			screen_y = 0x4Du;
		screen_y_2 = OBJECT(object)->screen_y;
		y_from_door = (uint16_t)(screen_y_2 - screen_y);
		if (screen_y <= screen_y_2 && y_from_door < 7u)
		{
			screen_x_2 = OBJECT(object)->screen_x;
			y_from_door = (uint16_t)(screen_x_2 - 0xCDu);
			if (screen_x_2 >= 0xCDu && y_from_door < 7u)
			{
				OBJECT(object)->turtle.exit_step++;
				OBJECT(object)->turtle.jump_speed = 0x400u;
				OBJECT(object)->turtle.falling = 0u;
				OBJECT(object)->turtle.move_timer = 8u;
				routine_state = 0x200u;
				turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				return;
			}
		}
		screen_y += 3u;
		screen_x = 0xD0u;
		y_from_door = direction_to(y_from_door, screen_x, screen_y, object);
		speed = 0u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
		move_in_direction((uint8_t)y_from_door, speed, object);
		return;
	}
	if (OBJECT(van)->car_street_van.once != 0u)
		return;
	OBJECT(van)->car_street_van.once--;
	if ((uint16_t)OBJECT(van)->x_speed < 0x414u && (uint16_t)OBJECT(van)->x_speed >= 0x400u)
	{
		sound = 0x3Cu;
		queue_sound(sound);
		van = OBJECT_ADDRESS(40);
	}
	x_speed = OBJECT(van)->x_speed;
	OBJECT(van)->x_speed = x_speed - 0x14u;
	if (x_speed < 0x14u)
	{
		OBJECT(van)->x_speed = 0;
		OBJECT(OBJECT_ADDRESS(44))->flags = 0u;
		sound = 0x12u;
		queue_sound(sound);
		van = OBJECT_ADDRESS(40);
	}
	OBJECT(van)->x += OBJECT(van)->x_speed;
	van_x = OBJECT(van)->x;
	OBJECT(van + 0x50u)->x = van_x;
	van_x = van_x >> 8;
	OBJECT(van)->screen_x = van_x;
	OBJECT(van + 0x50u)->screen_x = van_x;
}

/* after a moment the turtle jumps into the van, hidden as it comes down */
void turtle_exit_car_street_jump(uint32_t object)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;
	uint8_t move_timer = 0;
	uint16_t jump_speed = 0;

	turtles_on_the_way--;
	turtles_not_waiting = 0xFFu;
	if (OBJECT(object)->turtle.move_timer != 0u)
	{
		move_timer = OBJECT(object)->turtle.move_timer;
		OBJECT(object)->turtle.move_timer = move_timer - 1u;
		if (move_timer != 1u)
			return;
		OBJECT(object)->contacts |= 0x80u;
		routine_state = 0x201u;
		turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		return;
	}
	if (OBJECT(object)->turtle.falling != 0u)
	{
		OBJECT(object)->height -= OBJECT(object)->turtle.jump_speed;
		OBJECT(object)->turtle.jump_speed += 0x40u;
		if (OBJECT(object)->height >= 0x1800u)
			return;
		OBJECT(object)->flags &= 0x7Fu;
		return;
	}
	OBJECT(object)->height += OBJECT(object)->turtle.jump_speed;
	jump_speed = OBJECT(object)->turtle.jump_speed;
	OBJECT(object)->turtle.jump_speed = jump_speed - 0x40u;
	if (jump_speed >= 0x40u)
		return;
	OBJECT(object)->turtle.jump_speed = 0u - OBJECT(object)->turtle.jump_speed;
	OBJECT(object)->turtle.falling--;
	OBJECT(object)->height -= OBJECT(object)->turtle.jump_speed;
	OBJECT(object)->turtle.jump_speed += 0x40u;
	if (OBJECT(object)->height < 0x1800u)
		OBJECT(object)->flags &= 0x7Fu;
}

/* the turtles are in the van: its door shuts, "Let's move it", the engine runs */
void exit_car_street_van_start_up(uint16_t on_right)
{
	uint8_t sound = 0;
	uint16_t screen_x = 0, shake_offset = 0, speaker_y = 0;
	uint32_t object = 0, screen_y = 0, van_x = 0, y = 0, y_shake = 0;
	uint8_t t1 = 0;

	t1 = cutscene_count;
	cutscene_count = t1 - 1u;
	if (t1 != 1u)
	{
		if (cutscene_count == 0x60u)
		{
			on_right &= ~0xFFu;
			screen_x = 0xE0u;
			speaker_y = 0x70u;
			say_lets_move_it(on_right, screen_x, speaker_y);
		}
		object = OBJECT_ADDRESS(40);
		if (OBJECT(OBJECT_ADDRESS(41))->x < OBJECT(OBJECT_ADDRESS(40))->x)
		{
			OBJECT(object + 0x50u)->x += 0x300u;
			OBJECT(object + 0x50u)->screen_x = OBJECT(object + 0x50u)->x >> 8;
			return;
		}
		if ((cutscene_count & 0x1Fu) == 0u)
		{
			sound = 0x2Du;
			queue_sound(sound);
		}
		if ((frame_count & 1u) != 0u)
			return;
		shake_offset = cutscene_wait;
		cutscene_wait++;
		shake_offset = (uint16_t)((shake_offset & 7u) << 2);
		y_shake = van_start_y_shakes[shake_offset / 2u];
		OBJECT(OBJECT_ADDRESS(40))->x += van_start_x_shakes[shake_offset / 2u];
		OBJECT(OBJECT_ADDRESS(40))->y += y_shake;
		van_x = OBJECT(OBJECT_ADDRESS(40))->x;
		screen_y = OBJECT(OBJECT_ADDRESS(40))->y;
		OBJECT(OBJECT_ADDRESS(41))->x = van_x;
		y = screen_y - 0x400u;
		OBJECT(OBJECT_ADDRESS(41))->y = y;
		van_x = van_x >> 8;
		screen_y = screen_y >> 8;
		OBJECT(OBJECT_ADDRESS(40))->screen_x = van_x;
		OBJECT(OBJECT_ADDRESS(40))->screen_y = screen_y;
		OBJECT(OBJECT_ADDRESS(41))->screen_x = van_x;
		OBJECT(OBJECT_ADDRESS(41))->screen_y = y >> 8;
		screen_x_high = screen_y;
		return;
	}
	sound = 0x3Au;
	queue_sound(sound);
	sound = 0x6Eu;
	queue_sound(sound);
	LAYER(LAYER_B)->x_speed = 0u;
	next_area_step();
}

/* the van drives off and the screen is cleared */
void exit_car_street_van_drives_off(void)
{
	uint16_t kind = 0;
	uint32_t data = 0, van_x = 0;

	van_x = (OBJECT(OBJECT_ADDRESS(40))->x + 0x600u) & 0x1FFFFu;
	OBJECT(OBJECT_ADDRESS(40))->x = van_x;
	OBJECT(OBJECT_ADDRESS(41))->x = van_x;
	van_x = van_x >> 8;
	OBJECT(OBJECT_ADDRESS(40))->screen_x = van_x;
	OBJECT(OBJECT_ADDRESS(41))->screen_x = van_x;
	if (van_x < 0x59u || van_x >= 0x60u)
		return;
	cutscene_wait = 0xC0u;
	exit_cutscene = 0xFFu;
	LAYER(LAYER_A)->x = 0u;
	LAYER(LAYER_B)->x = 0u;
	LAYER(WORKING_LAYER)->x = 0u;
	clear_layers();
	data = april_hug_palettes_ADDRESS;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	next_area_step();
}

/* the picture of April hugging the turtle of the player with the best score */
void exit_car_street_april_hug_start(void)
{
	uint8_t colour = 0, tile_attribute = 0;
	uint32_t animation_entry = 0, animations = 0, best_colour = 0, ignored = 0, more_animations = 0, object = 0, object_2 = 0, other_colour = 0, player = 0, player_2 = 0, tile_place = 0, turtle_colour = 0;
	uint8_t colour_2 = 0;

	if (palettes_busy != 0u)
		return;
	LAYER(WORKING_LAYER)->map = april_hug_first_map_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = april_hug_first_blocks_ADDRESS;
	LAYER(WORKING_LAYER)->map_width = 0x20u;
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	io_write8(TILES_106B00, 0xD0u);
	tile_banks_0_1 = 0xD0u;
	LAYER(WORKING_LAYER)->map = april_hug_map_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = april_hug_blocks_ADDRESS;
	LAYER(WORKING_LAYER)->map_width = 0x20u;
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
	tile_place = TILE_RAM + 0x2628u;
	tile_attribute = 0xC4u;
	do
	{
		io_write8(tile_place, tile_attribute);
		io_write8(tile_place + 0x80u, tile_attribute);
		io_write8(tile_place + 0x100u, tile_attribute);
		io_write8(tile_place + 0x180u, tile_attribute);
		tile_place += 2u;
	} while (tile_place < TILE_RAM + 0x2630u);
	object = OBJECT_ADDRESS(0);
	cutscene_counter = 0u;
	turtle_colour = hug_turtle_colours_ADDRESS;
	hug_turtle_colours(0) = 8u;
	hug_turtle_colours(1) = 9u;
	hug_turtle_colours(2) = 0xAu;
	hug_turtle_colours(3) = 0xBu;
	do
	{
		if ((OBJECT(object)->flags & 1u) == 0u)
			HUG_COLOUR(turtle_colour)->colour = 0xFFu;
		else
			cutscene_counter++;
		object += 0x50u;
		turtle_colour++;
	} while (object < OBJECT_ADDRESS(4));
	cutscene_counter--;
	object = OBJECT_ADDRESS(0);
	object_2 = OBJECT_ADDRESS(1);
	best_colour = hug_turtle_colours_ADDRESS;
	other_colour = hug_turtle_colours_ADDRESS + 0x1u;
	player = player_1_score_ADDRESS;
	player_2 = player_2_score_ADDRESS;
	do
	{
		if ((OBJECT(object)->flags & 1u) == 0u)
		{
			player += 8u;
			object += 0x50u;
			best_colour++;
		}
		else if ((OBJECT(object_2)->flags & 1u) != 0u && PLAYER(player - 0x4u)->score <= PLAYER(player_2 - 0x4u)->score)
		{
			object = object_2;
			best_colour = other_colour;
			player = player_2;
		}
		player_2 += 8u;
		object_2 += 0x50u;
		other_colour++;
	} while (object_2 < OBJECT_ADDRESS(4));
	colour = HUG_COLOUR(best_colour)->colour;
	HUG_COLOUR(best_colour)->colour = 0xFFu;
	animations = april_hug_animations_ADDRESS;
	object = OBJECT_ADDRESS(12);
	do
	{
		OBJECT(object)->flags = 1u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->screen_x = 0xB8u;
		OBJECT(object)->screen_y = 0x80u;
		animation_entry = ANIMATION_ENTRY(animations);
		animations += 4u;
		set_animation(ignored, object, animation_entry);
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(16));
	OBJECT(OBJECT_ADDRESS(13))->screen_y = (OBJECT(OBJECT_ADDRESS(13))->screen_y & 0xFF00u) | 0x78u;
	OBJECT(OBJECT_ADDRESS(12))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(12))->colour = colour;
	OBJECT(OBJECT_ADDRESS(13))->colour = colour;
	object = OBJECT_ADDRESS(44);
	turtle_colour = hug_turtle_colours_ADDRESS;
	more_animations = april_hug_animations_2_ADDRESS;
	while (cutscene_counter != 0u)
	{
		cutscene_counter--;
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->screen_x = 0x90u;
		OBJECT(object)->screen_y = 0xB8u;
		OBJECT(object)->animation_delay = 0u;
		do
		{
			colour_2 = HUG_COLOUR(turtle_colour)->colour;
			colour = colour_2;
			turtle_colour++;
		} while ((int8_t)colour_2 < 0);
		OBJECT(object)->colour = colour;
		HUG_COLOUR(turtle_colour - 0x1u)->colour = 0xFFu;
		animation_entry = ANIMATION_ENTRY(more_animations);
		more_animations += 4u;
		set_animation(ignored, object, animation_entry);
		object += 0x50u;
	}
	cutscene_wait = 0u;
	next_area_step();
}

/* April thanks the turtle and a heart floats up, then the exit is over */
void exit_car_street_april_hug(void)
{
	uint8_t first_row_tile = 0, fourth_row_tile = 0, second_row_tile = 0, third_row_tile = 0, tile_attribute = 0;
	uint16_t line = 0, screen_x = 0, screen_y = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, on_right = 0, tile_place = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 == 1u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
		return;
	}
	if (stage == 5u)
		return;
	if (cutscene_wait == 0xD0u)
	{
		on_right = 0x7Fu;
		screen_x = 0x110u;
		screen_y = 0x70u;
		line = 0x37u;
		show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
		draw_balloon_pointer(ignored_3);
		return;
	}
	if (cutscene_wait == 0xB0u)
	{
		tile_place = TILE_RAM + 0x2628u;
		tile_attribute = 0xA5u;
		first_row_tile = 0xDDu;
		second_row_tile = 0xE1u;
		third_row_tile = 0xE5u;
		fourth_row_tile = 0xE8u;
		do
		{
			io_write8(tile_place, tile_attribute);
			io_write8(tile_place + 0x80u, tile_attribute);
			io_write8(tile_place + 0x100u, tile_attribute);
			io_write8(tile_place + 0x180u, tile_attribute);
			io_write8(tile_place + 1u, first_row_tile);
			io_write8(tile_place + 0x81u, second_row_tile);
			io_write8(tile_place + 0x101u, third_row_tile);
			io_write8(tile_place + 0x181u, fourth_row_tile);
			first_row_tile++;
			second_row_tile++;
			third_row_tile++;
			fourth_row_tile++;
			tile_place += 2u;
		} while (tile_place < TILE_RAM + 0x2630u);
		io_write8(TILES_1027A9, 0x21u);
		return;
	}
	if (cutscene_wait == 0x90u)
	{
		LAYER(WORKING_LAYER)->map = april_hug_map_2_ADDRESS;
		LAYER(WORKING_LAYER)->blocks = april_hug_blocks_ADDRESS;
		LAYER(WORKING_LAYER)->map_width = 0x20u;
		tile_place = TILE_RAM + 0x2000u;
		draw_whole_layer(tile_place);
		OBJECT(OBJECT_ADDRESS(13))->flags = 0x81u;
		return;
	}
	if (cutscene_wait == 0x70u)
	{
		OBJECT(OBJECT_ADDRESS(15))->flags = 0x81u;
		return;
	}
	if (cutscene_wait == 0x50u)
	{
		LAYER(WORKING_LAYER)->map = april_hug_map_ADDRESS;
		LAYER(WORKING_LAYER)->blocks = april_hug_blocks_ADDRESS;
		LAYER(WORKING_LAYER)->map_width = 0x20u;
		tile_place = TILE_RAM + 0x2000u;
		draw_whole_layer(tile_place);
		tile_place = TILE_RAM + 0x2628u;
		tile_attribute = 0xC4u;
		do
		{
			io_write8(tile_place, tile_attribute);
			io_write8(tile_place + 0x80u, tile_attribute);
			io_write8(tile_place + 0x100u, tile_attribute);
			io_write8(tile_place + 0x180u, tile_attribute);
			tile_place += 2u;
		} while (tile_place < TILE_RAM + 0x2630u);
		OBJECT(OBJECT_ADDRESS(14))->flags = 0x81u;
		OBJECT(OBJECT_ADDRESS(15))->screen_y++;
		return;
	}
	if (cutscene_wait >= 0x70u)
		return;
	if (cutscene_wait >= 0x30u)
	{
		OBJECT(OBJECT_ADDRESS(15))->screen_y++;
		return;
	}
	OBJECT(OBJECT_ADDRESS(15))->flags = 0u;
}

/* the object a step at a speed the way a direction points, and its screen place */
void move_in_direction(uint8_t direction, uint16_t speed, uint32_t object)
{
	uint32_t x_speed = 0, y_speed = 0;

	speed = (uint16_t)((speed & ~0xFF00u) | (screen_x_high & 0xFF00u));
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x += (int16_t)x_speed;
	OBJECT(object)->y += (int16_t)y_speed;
	update_screen_place(object);
}

/* the object's screen place from its place in the area */
void update_screen_place(uint32_t object)
{
	uint16_t screen_y = 0;

	screen_y = (uint16_t)(OBJECT(object)->y >> 8);
	OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
	OBJECT(object)->screen_y = screen_y;
}

/* area 5's exit: each turtle jumps and flies off to the right, hidden past the edge; then the exit is
   over */
void turtle_exit_highway(void)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;
	uint8_t t1 = 0;

	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u && (OBJECT(object)->flags & 0x80u) != 0u)
		{
			if ((uint8_t)OBJECT(object)->y_speed == 0u)
			{
				turtles_on_the_way--;
				if (OBJECT(object)->screen_x < 0x1A0u)
				{
					OBJECT(object)->screen_x += 4u;
					if ((uint16_t)((OBJECT(object)->height >> 8) + OBJECT(object)->screen_y) < 0x100u)
						OBJECT(object)->height += 0x600u;
				}
				else
					OBJECT(object)->flags &= 0x7Fu;
			}
			else
			{
				turtles_on_the_way--;
				t1 = (uint8_t)OBJECT(object)->y_speed;
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
				if (t1 == 1u)
				{
					OBJECT(object)->contacts |= 0x80u;
					routine_state = 0x201u;
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				}
			}
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (turtles_on_the_way == 0u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
		return;
	}
}

/* area 6's exit: the shadows, the road scrolling on, and its steps */
void run_exit_skateboard(void)
{
	uint16_t step_offset = 0;
	uint32_t step_object = 0;

	step_object = turtle_skateboard_shadow();
	if (cutscene_step < 4u)
	{
		load_layer_b();
		LAYER(WORKING_LAYER)->x += LAYER(WORKING_LAYER)->x_speed;
		LAYER(WORKING_LAYER)->x &= 0x1FFFFu;
		save_layer_b();
	}
	load_layer_a();
	step_offset = (uint16_t)((uint32_t)cutscene_step << 2);
	switch (step_offset)
	{
	case 0:
		step_object = turtle_exit_skateboard_gather();
		break;
	case 4:
		step_object = turtle_exit_skateboard_board();
		break;
	case 8:
		step_object = exit_skateboard_van_door();
		break;
	case 12:
		step_object = exit_skateboard_van_drives(step_offset, step_object);
		break;
	case 16:
		step_object = exit_skateboard_crash_picture(step_object);
		break;
	case 20:
		finish_area_exit();
		break;
	}
	if (cutscene_step < 4u)
	{
		auto_scroll(step_object);
		working_layer_tiles = TILE_RAM + 0x2000u;
		draw_new_row_and_column();
		OBJECT(OBJECT_ADDRESS(0))->contacts |= 0x80u;
		OBJECT(OBJECT_ADDRESS(1))->contacts |= 0x80u;
		OBJECT(OBJECT_ADDRESS(2))->contacts |= 0x80u;
		OBJECT(OBJECT_ADDRESS(3))->contacts |= 0x80u;
	}
	save_layer_a();
	set_layers_scroll();
}

/* area 6's exit, first step: each turtle to its place; then the van comes in with a sound */
uint32_t turtle_exit_skateboard_gather(void)
{
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, object = 0, turtle = 0, van_rear = 0, x = 0;

	turtles_on_the_way = 0u;
	exit_skateboard_road_round();
	turtle = OBJECT_ADDRESS(0);
	do
	{
		turtle_exit_go_to_place(turtle);
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	if (turtles_on_the_way != 0u)
		return turtle;
	object = OBJECT_ADDRESS(40);
	OBJECT(OBJECT_ADDRESS(40))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(40))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(41))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(41))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(40))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(40))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(41))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(41))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(40))->screen_x = 0x50u;
	OBJECT(OBJECT_ADDRESS(41))->screen_x = 0x50u;
	screen_x_high = 0u;
	OBJECT(OBJECT_ADDRESS(40))->height = 0x50u;
	OBJECT(OBJECT_ADDRESS(41))->height = 0x50u;
	x = VAN_START_X;
	OBJECT(OBJECT_ADDRESS(40))->x = x;
	OBJECT(OBJECT_ADDRESS(41))->x = x;
	OBJECT(OBJECT_ADDRESS(40))->y = 0xA000u;
	OBJECT(OBJECT_ADDRESS(40))->screen_y = 0xA0u;
	OBJECT(OBJECT_ADDRESS(41))->y = 0x9C00u;
	OBJECT(OBJECT_ADDRESS(41))->screen_y = 0x9Cu;
	OBJECT(OBJECT_ADDRESS(40))->x_speed = 0x600;
	animation = turtle_van_animation_ADDRESS;
	set_animation(0u, object, animation);
	animation = turtle_van_rear_animation_ADDRESS;
	van_rear = OBJECT_ADDRESS(41);
	set_animation(ignored, van_rear, animation);
	sound = 0x15u;
	queue_sound(sound);
	next_area_step();
	return van_rear;
}

/* the van slows to a stop; each turtle walks to its door and jumps in, hidden as it comes down */
uint32_t turtle_exit_skateboard_board(void)
{
	uint8_t speed = 0;
	uint16_t routine_state = 0, target_x = 0, target_y = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0, van_x = 0, y_from_door = 0;
	uint8_t t1 = 0;
	uint16_t x_speed = 0, screen_y = 0, screen_x = 0, x_speed_2 = 0;

	object = OBJECT_ADDRESS(40);
	if ((uint16_t)OBJECT(OBJECT_ADDRESS(40))->x_speed != 0u)
	{
		x_speed = OBJECT(object)->x_speed;
		OBJECT(object)->x_speed = x_speed - 0x14u;
		if (x_speed < 0x14u)
			OBJECT(object)->x_speed = 0;
		OBJECT(object)->x += (uint16_t)OBJECT(object)->x_speed;
		van_x = OBJECT(object)->x;
		OBJECT(object + 0x50u)->x = van_x;
		van_x = van_x >> 8;
		OBJECT(object)->screen_x = van_x;
		OBJECT(object + 0x50u)->screen_x = van_x;
	}
	else if (OBJECT(object)->x - OBJECT(object + 0x50u)->x >= 0x4000u)
	{
		object = OBJECT_ADDRESS(0);
		do
		{
			if ((OBJECT(object)->flags & 1u) != 0u && (OBJECT(object)->flags & 0x80u) != 0u)
			{
				screen_y = OBJECT(object)->screen_y;
				y_from_door = (uint16_t)(screen_y - 0x4Du);
				if (screen_y < 0x4Du || y_from_door >= 7u)
				{
					OBJECT(object)->y_speed = 0;
					OBJECT(object)->height = 0u;
					target_x = 0xA0u;
					target_y = 0x50u;
					y_from_door = direction_to(y_from_door, target_x, target_y, object);
					speed = 0u;
					screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
					move_in_direction((uint8_t)y_from_door, speed, object);
				}
				else
				{
					screen_x = OBJECT(object)->screen_x;
					y_from_door = (uint16_t)(screen_x - 0x9Du);
					if (screen_x < 0x9Du || y_from_door >= 7u)
					{
						OBJECT(object)->y_speed = 0;
						OBJECT(object)->height = 0u;
						target_x = 0xA0u;
						target_y = 0x50u;
						y_from_door = direction_to(y_from_door, target_x, target_y, object);
						speed = 0u;
						screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
						move_in_direction((uint8_t)y_from_door, speed, object);
					}
					else
					{
						if ((uint8_t)OBJECT(object)->y_speed == 0u && OBJECT(object)->height == 0u)
						{
							OBJECT(object)->x_speed = 0x400;
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0x8u;
							routine_state = 0x200u;
							turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
						}
						if ((uint8_t)OBJECT(object)->y_speed == 0u)
						{
							if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
							{
								OBJECT(object)->height -= OBJECT(object)->x_speed;
								OBJECT(object)->x_speed += 0x40u;
								if (OBJECT(object)->height < 0x1800u)
									OBJECT(object)->flags &= 0x7Fu;
							}
							else
							{
								OBJECT(object)->height += OBJECT(object)->x_speed;
								x_speed_2 = OBJECT(object)->x_speed;
								OBJECT(object)->x_speed = x_speed_2 - 0x40u;
								if (x_speed_2 < 0x40u)
								{
									OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
									OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
									OBJECT(object)->height -= OBJECT(object)->x_speed;
									OBJECT(object)->x_speed += 0x40u;
									if (OBJECT(object)->height < 0x1800u)
										OBJECT(object)->flags &= 0x7Fu;
								}
							}
						}
						else
						{
							t1 = (uint8_t)OBJECT(object)->y_speed;
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
							if (t1 == 1u)
							{
								routine_state = 0x201u;
								turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
								if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
								{
									OBJECT(object)->height -= OBJECT(object)->x_speed;
									OBJECT(object)->x_speed += 0x40u;
									if (OBJECT(object)->height < 0x1800u)
										OBJECT(object)->flags &= 0x7Fu;
								}
								else
								{
									OBJECT(object)->height += OBJECT(object)->x_speed;
									x_speed_2 = OBJECT(object)->x_speed;
									OBJECT(object)->x_speed = x_speed_2 - 0x40u;
									if (x_speed_2 < 0x40u)
									{
										OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
										OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
										OBJECT(object)->height -= OBJECT(object)->x_speed;
										OBJECT(object)->x_speed += 0x40u;
										if (OBJECT(object)->height < 0x1800u)
											OBJECT(object)->flags &= 0x7Fu;
									}
								}
							}
						}
					}
				}
			}
			object += 0x50u;
		} while (object < OBJECT_ADDRESS(4));
	}
	else
	{
		OBJECT(object + 0x50u)->x -= 0x300u;
		OBJECT(object + 0x50u)->screen_x = OBJECT(object + 0x50u)->x >> 8;
	}
	exit_skateboard_road_round();
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 0x80u) != 0u)
			return object;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	next_area_step();
	return object;
}

/* the turtles are in the van and its door shuts */
uint32_t exit_skateboard_van_door(void)
{
	uint8_t sound = 0;
	uint32_t object = 0, screen_x = 0, x = 0;

	exit_skateboard_road_round();
	object = OBJECT_ADDRESS(40);
	if (OBJECT(OBJECT_ADDRESS(40))->x == OBJECT(OBJECT_ADDRESS(41))->x)
	{
		if (LAYER(WORKING_LAYER)->x < 0x8A200u || LAYER(WORKING_LAYER)->x >= 0x8A800u)
			return object;
		LAYER(WORKING_LAYER)->column_phase = (LAYER(WORKING_LAYER)->x & 0x700u) << 5;
		next_area_step();
		return object;
	}
	OBJECT(object + 0x50u)->x += 0x300u;
	x = OBJECT(object + 0x50u)->x;
	screen_x = x >> 8;
	screen_x_high = screen_x;
	OBJECT(object + 0x50u)->screen_x = screen_x;
	if (x < OBJECT(object)->x)
		return object;
	OBJECT(object + 0x50u)->x = OBJECT(object)->x;
	OBJECT(object + 0x50u)->screen_x = OBJECT(object)->x >> 8;
	sound = 0x6Eu;
	queue_sound(sound);
	if (LAYER(WORKING_LAYER)->x < 0x8A200u || LAYER(WORKING_LAYER)->x >= 0x8A800u)
		return object;
	LAYER(WORKING_LAYER)->column_phase = (LAYER(WORKING_LAYER)->x & 0x700u) << 5;
	next_area_step();
	return object;
}

/* the van speeds round the curve, "Hang on, guys", and the screen is cleared */
uint32_t exit_skateboard_van_drives(uint32_t on_right, uint32_t step_object)
{
	uint8_t sound = 0;
	uint16_t kind = 0, line = 0, screen_x = 0, screen_y = 0;
	uint32_t cleared_end = 0, data = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0;

	if (LAYER(WORKING_LAYER)->x < 0xA0000u)
	{
		if (balloons_said != 0u)
			return step_object;
		on_right &= ~0xFFu;
		screen_x = 0xC0u;
		screen_y = 0x70u;
		line = 0x3Cu;
		show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
		draw_balloon_pointer(ignored_3);
		return step_object;
	}
	if (LAYER(WORKING_LAYER)->x < 0xA4000u)
		return step_object;
	if (LAYER(WORKING_LAYER)->x < 0xA4600u)
	{
		sound = 0x3Cu;
		queue_sound(sound);
		return step_object;
	}
	if (LAYER(WORKING_LAYER)->x < 0xA8000u)
		return step_object;
	OBJECT(OBJECT_ADDRESS(40))->flags = 0u;
	OBJECT(OBJECT_ADDRESS(41))->flags = 0u;
	exit_cutscene = 0xFFu;
	LAYER(WORKING_LAYER)->x = 0x800u;
	LAYER(LAYER_A)->x = 0x800u;
	LAYER(LAYER_B)->x = 0x800u;
	cleared_end = clear_layers();
	data = skateboard_crash_picture_palettes_ADDRESS;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	next_area_step();
	return cleared_end;
}

/* the picture of the van crashing through the road's wall, the turtles leaping out */
uint32_t exit_skateboard_crash_picture(uint32_t step_object)
{
	uint32_t carry = 0;
	uint8_t columns_left = 0, rows_left = 0, sound = 0, tile_attribute = 0, tile_code = 0;
	uint32_t animation_entry = 0, animations = 0, object = 0, object_2 = 0, tile_code_2 = 0, tile_place = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	if (palettes_busy != 0u)
		return step_object;
	tile_place = TILE_RAM + 0x4008u;
	tile_attribute = 0x24u;
	tile_code = 0u;
	rows_left = 0x20u;
	do
	{
		columns_left = 0x20u;
		do
		{
			io_write8(tile_place, tile_attribute);
			io_write8(tile_place + 1u, tile_code);
			carry = (uint8_t)(tile_code + 1u) < tile_code;
			tile_code++;
			tile_place += 2u;
			if (carry)
				tile_attribute++;
			t1 = columns_left;
			columns_left = (uint8_t)(t1 - 1u);
		} while (t1 != 1u);
		t2 = rows_left;
		rows_left = (uint8_t)(t2 - 1u);
		tile_place += 0x40u;
	} while (t2 != 1u);
	io_write8(TILES_106B00, 0xE0u);
	tile_banks_0_1 = 0xE0u;
	tile_place = TILE_RAM + 0x2220u;
	tile_attribute = 0x13u;
	tile_code_2 = 0u;
	rows_left = 0x10u;
	do
	{
		columns_left = 0x10u;
		do
		{
			io_write8(tile_place, tile_attribute);
			io_write8(tile_place + 1u, tile_code_2);
			tile_code_2 = (uint8_t)(tile_code_2 + 1u);
			t3 = columns_left;
			columns_left = (uint8_t)(t3 - 1u);
			tile_place += 2u;
		} while (t3 != 1u);
		t4 = rows_left;
		rows_left = (uint8_t)(t4 - 1u);
		tile_place += 0x60u;
	} while (t4 != 1u);
	animations = skateboard_crash_picture_animations_ADDRESS;
	object = OBJECT_ADDRESS(44);
	object_2 = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object_2)->flags & 1u) != 0u)
		{
			OBJECT(object)->flags = 0x81u;
			OBJECT(object)->height = 0u;
			OBJECT(object)->colour = 0xFFu;
			OBJECT(object)->animation_delay = 0u;
			OBJECT(object)->screen_x = 0xF8u;
			OBJECT(object)->screen_y = 0x80u;
			animation_entry = ANIMATION_ENTRY(animations);
			set_animation(tile_code_2, object, animation_entry);
		}
		object_2 += 0x50u;
		animations += 4u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(48));
	sound = 0x1Au;
	queue_sound(sound);
	cutscene_wait = 0xA0u;
	next_area_step();
	return object;
}

/* the exit over when its wait runs out */
void finish_area_exit(void)
{
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 == 1u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
	}
}

/* the road goes round instead of running out of tiles */
void exit_skateboard_road_round(void)
{
	LAYER(WORKING_LAYER)->column_phase = 0u;
	if (LAYER(WORKING_LAYER)->x < 0xA0000u)
		return;
	LAYER(WORKING_LAYER)->x &= 0x80FFFu;
}

/* the turtle walks to its place for the van and stands there */
void turtle_exit_go_to_place(uint32_t object)
{
	uint8_t speed = 0;
	uint16_t kind_offset = 0, place_y_2 = 0, routine_state = 0, target_x = 0, x_from_place = 0, y_from_place = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, kind_offset_2 = 0, place_y = 0, screen_x = 0;
	uint16_t screen_y = 0, screen_x_2 = 0;

	kind_offset = (uint16_t)((uint32_t)OBJECT(object)->kind << 2);
	screen_x = exit_places_x[kind_offset / 2u];
	place_y = exit_places_y[kind_offset / 2u];
	screen_y = OBJECT(object)->screen_y;
	y_from_place = (uint16_t)(screen_y - 0x2Du);
	if (screen_y < 0x2Du || y_from_place >= 7u)
	{
		turtles_on_the_way--;
		kind_offset_2 = (uint16_t)((uint32_t)OBJECT(object)->kind << 2);
		target_x = (uint16_t)(exit_places_x[kind_offset_2 / 2u] + 3u);
		place_y_2 = exit_places_y[kind_offset_2 / 2u];
		kind_offset_2 = direction_to(kind_offset_2, target_x, place_y_2, object);
		speed = 0u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
		move_in_direction((uint8_t)kind_offset_2, speed, object);
		return;
	}
	screen_x_2 = OBJECT(object)->screen_x;
	x_from_place = (uint16_t)(screen_x_2 - screen_x);
	if (screen_x_2 < screen_x || x_from_place >= 7u)
	{
		turtles_on_the_way--;
		kind_offset_2 = (uint16_t)((uint32_t)OBJECT(object)->kind << 2);
		target_x = (uint16_t)(exit_places_x[kind_offset_2 / 2u] + 3u);
		place_y_2 = exit_places_y[kind_offset_2 / 2u];
		kind_offset_2 = direction_to(kind_offset_2, target_x, place_y_2, object);
		speed = 0u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
		move_in_direction((uint8_t)kind_offset_2, speed, object);
		return;
	}
	screen_x = (uint16_t)(screen_x + 2u);
	place_y = (uint16_t)(place_y + 2u);
	OBJECT(object)->screen_x = screen_x;
	OBJECT(object)->screen_y = place_y;
	screen_x = (uint32_t)(int16_t)screen_x << 8;
	screen_x_high = screen_x;
	place_y = (uint32_t)(int16_t)place_y << 8;
	OBJECT(object)->x = screen_x;
	OBJECT(object)->y = place_y;
	OBJECT(object)->turtle.move_timer = 0x10u;
	OBJECT(object)->flags &= 0xBFu;
	routine_state = 0x100u;
	turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
	OBJECT(object)->frame_time = 0u;
	distance_high_word = place_y;
}

/* area 7's exit by its steps */
void run_exit_building_site(void)
{
	uint16_t step_offset = 0;

	step_offset = (uint16_t)((uint32_t)cutscene_step << 2);
	switch (step_offset)
	{
	case 0:
		exit_building_site_free_splinter();
		break;
	case 4:
		turtle_exit_building_site_walk();
		break;
	case 8:
		turtle_exit_building_site_jump();
		break;
	case 12:
		exit_building_site_splinter_picture();
		break;
	case 16:
		exit_building_site_splinter_thanks();
		break;
	case 20:
		turtle_exit_building_site_drop();
		break;
	case 24:
		turtle_exit_building_site_land();
		break;
	case 28:
		turtle_exit_building_site_wait(step_offset);
		break;
	case 32:
		turtle_exit_building_site_rise();
		break;
	case 36:
		turtle_exit_building_site_leave();
		break;
	}
	set_layers_scroll();
}

/* Splinter, tied up, is let down to the turtles */
void exit_building_site_free_splinter(void)
{
	uint32_t animation = 0, object = 0;
	uint16_t height = 0;

	object = OBJECT_ADDRESS(44);
	height = OBJECT(OBJECT_ADDRESS(44))->height;
	OBJECT(OBJECT_ADDRESS(44))->height = height - 0x100u;
	if (height == 0x100u)
	{
		cutscene_wait = 0x80u;
		OBJECT(object)->contacts = 0u;
		animation = splinter_stretches_animation_ADDRESS;
		set_animation(0u, object, animation);
		next_area_step();
		return;
	}
	if (OBJECT(object)->height == 0x2000u)
	{
		animation = splinter_freed_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
}

/* after the wait, each turtle walks */
void turtle_exit_building_site_walk(void)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 != 1u)
		return;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			routine_state = 0x101u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	next_area_step();
}

/* each turtle walks to one spot and jumps off, hidden as it comes down; then the next picture's layers
   and palette */
void turtle_exit_building_site_jump(void)
{
	uint8_t speed = 0;
	uint16_t kind = 0, target_x = 0, target_y = 0;
	uint32_t data = 0, direction = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;
	uint8_t t1 = 0;
	uint16_t height = 0, x_speed = 0;

	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u && (OBJECT(object)->flags & 0x80u) != 0u)
		{
			turtles_on_the_way--;
			if (OBJECT(object)->y < 0x7000u || OBJECT(object)->x < 0xF000u || OBJECT(object)->x > 0x10FFFu)
			{
				target_x = 0x100u;
				target_y = 0x90u;
				direction = direction_to(direction, target_x, target_y, object);
				speed = 0u;
				screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
				move_in_direction((uint8_t)direction, speed, object);
			}
			else if (OBJECT(object)->height != 0u)
			{
				if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
				{
					direction = (uint16_t)OBJECT(object)->x_speed;
					height = OBJECT(object)->height;
					OBJECT(object)->height = height - direction;
					if (height < direction)
						OBJECT(object)->flags &= 0x7Fu;
					else
						OBJECT(object)->x_speed += 0x20u;
				}
				else
				{
					OBJECT(object)->height += OBJECT(object)->x_speed;
					x_speed = OBJECT(object)->x_speed;
					OBJECT(object)->x_speed = x_speed - 0x30u;
					if (x_speed < 0x30u)
					{
						OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
						OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
						direction = 0x203u;
						turtle_set_state((uint16_t)direction, object, &ignored, &ignored_2, &ignored_3);
					}
				}
				if (OBJECT(object)->y < 0x8400u)
				{
					target_x = 0x100u;
					target_y = 0x90u;
					direction = direction_to(direction, target_x, target_y, object);
					speed = 0u;
					screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
					move_in_direction((uint8_t)direction, speed, object);
				}
			}
			else
			{
				if ((uint8_t)OBJECT(object)->y_speed == 0u)
				{
					OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | 0x8u;
					direction = 0x200u;
					turtle_set_state((uint16_t)direction, object, &ignored, &ignored_2, &ignored_3);
					OBJECT(object)->contacts |= 0x80u;
				}
				t1 = (uint8_t)OBJECT(object)->y_speed;
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
				if (t1 == 1u)
				{
					direction = 0x201u;
					turtle_set_state((uint16_t)direction, object, &ignored, &ignored_2, &ignored_3);
					OBJECT(object)->x_speed = 0x500;
					if ((uint8_t)(OBJECT(object)->y_speed >> 8u) != 0u)
					{
						direction = (uint16_t)OBJECT(object)->x_speed;
						height = OBJECT(object)->height;
						OBJECT(object)->height = height - direction;
						if (height < direction)
							OBJECT(object)->flags &= 0x7Fu;
						else
							OBJECT(object)->x_speed += 0x20u;
					}
					else
					{
						OBJECT(object)->height += OBJECT(object)->x_speed;
						x_speed = OBJECT(object)->x_speed;
						OBJECT(object)->x_speed = x_speed - 0x30u;
						if (x_speed < 0x30u)
						{
							OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
							OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFFu) | (((uint8_t)(OBJECT(object)->y_speed >> 8u) - 1u & 0xFFu) << 8u);
							direction = 0x203u;
							turtle_set_state((uint16_t)direction, object, &ignored, &ignored_2, &ignored_3);
						}
					}
					if (OBJECT(object)->y < 0x8400u)
					{
						target_x = 0x100u;
						target_y = 0x90u;
						direction = direction_to(direction, target_x, target_y, object);
						speed = 0u;
						screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
						move_in_direction((uint8_t)direction, speed, object);
					}
				}
			}
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (turtles_on_the_way != 0u)
		return;
	OBJECT(OBJECT_ADDRESS(44))->flags = 0u;
	object = OBJECT_ADDRESS(32);
	do
	{
		OBJECT(object)->flags = 0u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(40));
	object = OBJECT_ADDRESS(12);
	do
	{
		OBJECT(object)->flags = 0u;
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(24));
	exit_cutscene = 0xFFu;
	clear_layers();
	LAYER(WORKING_LAYER)->x = 0u;
	LAYER(LAYER_A)->x = 0u;
	LAYER(LAYER_B)->x = 0u;
	data = splinter_picture_palettes_ADDRESS;
	kind = 0x20Fu;
	post_palette_request(kind, data);
	next_area_step();
}

/* the picture of Splinter thanking his turtles */
void exit_building_site_splinter_picture(void)
{
	uint8_t columns_left = 0, tile_attribute = 0;
	uint32_t tile_place = 0, tiles = 0;
	uint8_t t1 = 0;

	if (palettes_busy != 0u)
		return;
	io_write8(TILES_106B00, 0x70u);
	tile_banks_0_1 = 0x70u;
	LAYER(WORKING_LAYER)->map = splinter_picture_map_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = splinter_picture_blocks_ADDRESS;
	LAYER(WORKING_LAYER)->map_width = 0x20u;
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	tile_place = TILE_RAM + 0x4208u;
	tiles = splinter_picture_tiles_ADDRESS;
	columns_left = 0x10u;
	tile_attribute = 0x37u;
	screen_x_high = 0x37u;
	do
	{
		io_write8(tile_place, tile_attribute);
		io_write8(tile_place + 0x80u, tile_attribute);
		io_write8(tile_place + 0x100u, tile_attribute);
		io_write8(tile_place + 0x180u, tile_attribute);
		io_write8(tile_place + 0x200u, tile_attribute);
		io_write8(tile_place + 0x280u, tile_attribute);
		io_write8(tile_place + 0x300u, tile_attribute);
		io_write8(tile_place + 0x380u, tile_attribute);
		io_write8(tile_place + 1u, PICTURE_TILE(tiles));
		io_write8(tile_place + 0x81u, PICTURE_TILE(tiles + 0x10u));
		io_write8(tile_place + 0x101u, PICTURE_TILE(tiles + 0x20u));
		io_write8(tile_place + 0x181u, PICTURE_TILE(tiles + 0x30u));
		io_write8(tile_place + 0x201u, PICTURE_TILE(tiles + 0x40u));
		io_write8(tile_place + 0x281u, PICTURE_TILE(tiles + 0x50u));
		io_write8(tile_place + 0x301u, PICTURE_TILE(tiles + 0x60u));
		io_write8(tile_place + 0x381u, PICTURE_TILE(tiles + 0x70u));
		t1 = columns_left;
		columns_left = (uint8_t)(t1 - 1u);
		tiles++;
		tile_place += 2u;
	} while (t1 != 1u);
	cutscene_wait = 0xB0u;
	next_area_step();
}

/* Splinter speaks his thanks, then the screen is cleared */
void exit_building_site_splinter_thanks(void)
{
	uint8_t sound = 0;
	uint16_t kind = 0;
	uint32_t animation = 0, data = 0, object = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 == 1u)
	{
		clear_layers();
		data = technodrome_drop_palettes_ADDRESS;
		kind = 0x20Fu;
		post_palette_request(kind, data);
		next_area_step();
		return;
	}
	if (cutscene_wait != 0x80u)
	{
		if (cutscene_wait != 0x30u)
			return;
		OBJECT(OBJECT_ADDRESS(44))->flags = 0u;
		return;
	}
	sound = 0xB4u;
	queue_sound(sound);
	object = OBJECT_ADDRESS(44);
	OBJECT(OBJECT_ADDRESS(44))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(44))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(44))->height = 0u;
	OBJECT(OBJECT_ADDRESS(44))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(44))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(44))->screen_x = 0x100u;
	OBJECT(OBJECT_ADDRESS(44))->screen_y = 0x80u;
	animation = splinter_thanks_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the palettes are ready, each turtle placed high up to fall in, the Technodrome drawn, a sound */
void turtle_exit_building_site_drop(void)
{
	uint8_t sound = 0;
	uint16_t kind_offset = 0, routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, screen_x = 0, screen_y = 0, turtle = 0;

	if (palettes_busy != 0u)
		return;
	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			OBJECT(turtle)->flags = 0x81u;
			OBJECT(turtle)->contacts = 0u;
			kind_offset = (uint16_t)((uint32_t)OBJECT(turtle)->kind << 3);
			screen_x = building_site_exit_x[kind_offset / 2u];
			screen_y = building_site_exit_y[kind_offset / 2u];
			OBJECT(turtle)->height = building_site_exit_heights[kind_offset / 2u];
			OBJECT(turtle)->turtle.drift_speed = building_site_exit_drifts[kind_offset / 2u];
			OBJECT(turtle)->screen_x = screen_x;
			OBJECT(turtle)->screen_y = screen_y;
			OBJECT(turtle)->x = screen_x << 8;
			OBJECT(turtle)->y = screen_y << 8;
			routine_state = 0x203u;
			turtle_set_state(routine_state, turtle, &ignored, &ignored_2, &ignored_3);
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	LAYER(WORKING_LAYER)->x = 0x2000u;
	LAYER(LAYER_A)->x = 0x2000u;
	LAYER(LAYER_B)->x = 0x2000u;
	draw_technodrome_screen();
	layer_priority = 0u;
	io_write8(PRIORITY_0C0001, 0u);
	sound = 0x75u;
	queue_sound(sound);
	next_area_step();
}

/* each turtle falls in across and lands, then walks; when all have landed, a wait of 160 frames */
void turtle_exit_building_site_land(void)
{
	uint16_t routine_state = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0, screen_x = 0;
	uint8_t t1 = 0;
	uint16_t height = 0;

	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			if ((uint8_t)OBJECT(object)->y_speed != 0u)
			{
				turtles_on_the_way--;
				t1 = (uint8_t)OBJECT(object)->y_speed;
				OBJECT(object)->y_speed = (OBJECT(object)->y_speed & 0xFF00u) | (t1 - 1u & 0xFFu);
				if (t1 == 1u)
				{
					if (OBJECT(object)->screen_x >= 0x100u)
						OBJECT(object)->flags |= 0x40u;
					routine_state = 0x101u;
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
					OBJECT(object)->frame_time = 0u;
				}
			}
			else if (OBJECT(object)->routine != 1u)
			{
				turtles_on_the_way--;
				OBJECT(object)->x += 0x300u;
				screen_x = OBJECT(object)->x >> 8;
				screen_x_high = screen_x;
				OBJECT(object)->screen_x = screen_x;
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
	if (turtles_on_the_way != 0u)
		return;
	cutscene_wait = 0xA0u;
	next_area_step();
}

/* the wait: a speech balloon by the first turtle, then crouching and jumping at its end */
void turtle_exit_building_site_wait(uint32_t on_right)
{
	uint16_t line = 0, routine_state = 0, screen_x = 0, screen_y = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, object = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 != 1u)
	{
		if (cutscene_wait == 0x30u)
		{
			erase_balloon();
			return;
		}
		if (cutscene_wait != 0x70u)
		{
			if (cutscene_wait != 8u)
				return;
			object = OBJECT_ADDRESS(0);
			do
			{
				if ((OBJECT(object)->flags & 1u) != 0u)
				{
					routine_state = 0x200u;
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				}
				object += 0x50u;
			} while (object < OBJECT_ADDRESS(4));
			return;
		}
		object = OBJECT_ADDRESS(0);
		while ((OBJECT(object)->flags & 1u) == 0u)
		{
			object += 0x50u;
			if (object >= OBJECT_ADDRESS(4))
				return;
		}
		on_right &= ~0xFFu;
		if (OBJECT(object)->screen_x >= 0x100u)
			on_right |= (uint8_t)(on_right - 1u);
		screen_x = OBJECT(object)->screen_x;
		screen_y = 8u;
		line = 0xFu;
		show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored_2, &ignored_3, &ignored_4);
		draw_balloon_pointer(ignored_4);
		return;
	}
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			routine_state = 0x201u;
			turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	next_area_step();
}

/* each turtle goes up the screen, then its own pose */
void turtle_exit_building_site_rise(void)
{
	uint32_t animation = 0, ignored = 0, object = 0;

	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u)
		{
			if (OBJECT(object)->screen_y >= 0x100u)
			{
				animation = exit_building_site_rise_animations[OBJECT(object)->kind];
				set_animation(ignored, object, animation);
			}
			else
			{
				turtles_on_the_way--;
				OBJECT(object)->y += 0x600u;
				OBJECT(object)->screen_y = OBJECT(object)->y >> 8;
			}
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (turtles_on_the_way == 0u)
	{
		next_area_step();
		return;
	}
}

/* each turtle drifts across and down while its pose lasts, hidden when it ends; then the exit is over */
void turtle_exit_building_site_leave(void)
{
	uint32_t turtle = 0;

	turtles_on_the_way = 0u;
	turtle = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(turtle)->flags & 1u) != 0u)
		{
			if (OBJECT(turtle)->frame_time != 0u)
			{
				OBJECT(turtle)->x += (int16_t)OBJECT(turtle)->turtle.drift_speed;
				OBJECT(turtle)->screen_x = OBJECT(turtle)->x >> 8;
				turtles_on_the_way--;
				OBJECT(turtle)->y -= 0x100u;
				OBJECT(turtle)->screen_y = OBJECT(turtle)->y >> 8;
			}
			else
				OBJECT(turtle)->flags &= 0x7Fu;
		}
		turtle += 0x50u;
	} while (turtle < OBJECT_ADDRESS(4));
	if (turtles_on_the_way != 0u)
		return;
	area_beaten = 0u;
	area_cleared = 0u;
	scene_step++;
}

/* area 8's exit: the turtles walk off to the right, hidden past the edge; over after the wait */
void turtle_exit_technodrome(void)
{
	uint8_t speed = 0;
	uint16_t target_x = 0, target_y = 0;
	uint32_t direction = 0, object = 0;
	uint8_t t1 = 0;

	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 == 0u)
		cutscene_wait = 0u;
	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x < 0x19000u)
		{
			turtles_on_the_way--;
			target_x = 0x198u;
			target_y = 0x58u;
			direction = direction_to(direction, target_x, target_y, object);
			speed = 0u;
			screen_x_high = (screen_x_high & ~0xFFFFu) | 0x200u;
			move_in_direction((uint8_t)direction, speed, object);
			if (OBJECT(object)->x >= 0x18800u)
				OBJECT(object)->flags &= 0x7Fu;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (cutscene_wait == 0u && turtles_on_the_way == 0u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
	}
}

/* area 9's exit by its steps */
void run_exit_final(void)
{
	switch (cutscene_step)
	{
	case 0:
		turtle_exit_final_walk();
		return;
	case 1:
		exit_final_shake();
		return;
	}
}

/* area 9's exit: the turtles turn left and walk off with a sound while the screen shakes */
void turtle_exit_final_walk(void)
{
	uint8_t sound = 0, y_shake = 0;
	uint16_t routine_state = 0, shake_offset = 0, x_shake = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, object = 0;

	if (cutscene_wait != 0u)
	{
		cutscene_wait--;
		if (cutscene_wait >= 0x80u)
		{
			if (cutscene_wait != 0x80u)
				return;
			object = OBJECT_ADDRESS(0);
			do
			{
				if ((OBJECT(object)->flags & 1u) != 0u)
				{
					OBJECT(object)->flags |= 0x40u;
					routine_state = 0x100u;
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				}
				object += 0x50u;
			} while (object < OBJECT_ADDRESS(4));
			sound = 0x34u;
			queue_sound(sound);
		}
		if ((frame_count & 1u) == 0u)
		{
			shake_offset = cutscene_count;
			cutscene_count++;
			shake_offset = (uint16_t)((shake_offset & 7u) << 1);
			y_shake = (uint8_t)shake_y[shake_offset];
			x_shake = (uint16_t)shake_x[shake_offset];
			screen_x_high = (screen_x_high & ~0xFFFFu) | x_shake;
			layer_b_scroll_x += x_shake;
			layer_b_scroll_y = (layer_b_scroll_y & 0xFF00u) | ((uint8_t)layer_b_scroll_y + y_shake & 0xFFu);
		}
		if (cutscene_wait >= 0x70u)
		{
			if (cutscene_wait != 0x70u)
				return;
			object = OBJECT_ADDRESS(0);
			do
			{
				if ((OBJECT(object)->flags & 1u) != 0u)
				{
					routine_state = 0x100u;
					turtle_set_state(routine_state, object, &ignored, &ignored_2, &ignored_3);
				}
				object += 0x50u;
			} while (object < OBJECT_ADDRESS(4));
		}
	}
	turtles_on_the_way = 0u;
	object = OBJECT_ADDRESS(0);
	do
	{
		if ((OBJECT(object)->flags & 1u) != 0u && OBJECT(object)->x >= 0x4000u)
		{
			turtles_on_the_way--;
			OBJECT(object)->x -= 0x300u;
			OBJECT(object)->screen_x = OBJECT(object)->x >> 8;
		}
		object += 0x50u;
	} while (object < OBJECT_ADDRESS(4));
	if (turtles_on_the_way != 0u)
		return;
	cutscene_wait = 0x80u;
	next_area_step();
}

/* the screen shakes on a while, then the exit is over and the ending follows */
void exit_final_shake(void)
{
	uint8_t y_shake = 0;
	uint16_t shake_offset = 0, x_shake = 0;
	uint8_t t1 = 0;

	if ((frame_count & 1u) == 0u)
	{
		shake_offset = cutscene_count;
		cutscene_count++;
		shake_offset = (uint16_t)((shake_offset & 7u) << 1);
		y_shake = (uint8_t)shake_y[shake_offset];
		x_shake = (uint16_t)shake_x[shake_offset];
		screen_x_high = x_shake;
		layer_b_scroll_x += x_shake;
		layer_b_scroll_y = (layer_b_scroll_y & 0xFF00u) | ((uint8_t)layer_b_scroll_y + y_shake & 0xFFu);
	}
	t1 = cutscene_wait;
	cutscene_wait = t1 - 1u;
	if (t1 == 1u)
	{
		area_beaten = 0u;
		area_cleared = 0u;
		scene_step++;
	}
}

/* the screen of the Technodrome in its cave */
void draw_technodrome_screen(void)
{
	uint32_t tile_place = 0;

	io_write8(TILES_106B00, 0xC0u);
	tile_banks_0_1 = 0xC0u;
	LAYER(WORKING_LAYER)->map = technodrome_screen_map_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = technodrome_screen_blocks_ADDRESS;
	LAYER(WORKING_LAYER)->x = 0x2000u;
	LAYER(WORKING_LAYER)->y = 0x10000u;
	LAYER(WORKING_LAYER)->map_width = 0x20u;
	tile_place = TILE_RAM + 0x4000u;
	draw_whole_layer(tile_place);
	io_write8(TILES_106E00, 0x2Cu);
	tile_banks_2_3 = 0x2Cu;
	LAYER(WORKING_LAYER)->map = technodrome_screen_map_2_ADDRESS;
	LAYER(WORKING_LAYER)->blocks = technodrome_screen_blocks_2_ADDRESS;
	LAYER(WORKING_LAYER)->x = 0x2000u;
	LAYER(WORKING_LAYER)->y = 0x10000u;
	tile_place = TILE_RAM + 0x2000u;
	draw_whole_layer(tile_place);
}

/* the three tile layers cleared to blank between two screens */
uint32_t clear_layers(void)
{
	uint8_t blank_attribute = 0, space_tile = 0;
	uint16_t groups_left = 0;
	uint32_t fixed_place = 0, layer_a_place = 0, layer_b_place = 0;
	uint16_t t9 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	fixed_place = TILE_RAM;
	layer_a_place = TILE_RAM + 0x2000u;
	layer_b_place = TILE_RAM + 0x4000u;
	blank_attribute = 0u;
	space_tile = 0x10u;
	screen_x_high = 0x10u;
	groups_left = 0x200u;
	do
	{
		io_write8(fixed_place, blank_attribute);
		io_write8(layer_a_place, blank_attribute);
		t1 = layer_b_place;
		layer_b_place++;
		io_write8(t1, blank_attribute);
		fixed_place++;
		io_write8(fixed_place, space_tile);
		layer_a_place++;
		io_write8(layer_a_place, space_tile);
		t2 = layer_b_place;
		layer_b_place++;
		io_write8(t2, space_tile);
		fixed_place++;
		io_write8(fixed_place, blank_attribute);
		layer_a_place++;
		io_write8(layer_a_place, blank_attribute);
		t3 = layer_b_place;
		layer_b_place++;
		io_write8(t3, blank_attribute);
		fixed_place++;
		io_write8(fixed_place, space_tile);
		layer_a_place++;
		io_write8(layer_a_place, space_tile);
		t4 = layer_b_place;
		layer_b_place++;
		io_write8(t4, space_tile);
		fixed_place++;
		io_write8(fixed_place, blank_attribute);
		layer_a_place++;
		io_write8(layer_a_place, blank_attribute);
		t5 = layer_b_place;
		layer_b_place++;
		io_write8(t5, blank_attribute);
		fixed_place++;
		io_write8(fixed_place, space_tile);
		layer_a_place++;
		io_write8(layer_a_place, space_tile);
		t6 = layer_b_place;
		layer_b_place++;
		io_write8(t6, space_tile);
		fixed_place++;
		io_write8(fixed_place, blank_attribute);
		layer_a_place++;
		io_write8(layer_a_place, blank_attribute);
		t7 = layer_b_place;
		layer_b_place++;
		io_write8(t7, blank_attribute);
		fixed_place++;
		io_write8(fixed_place, space_tile);
		layer_a_place++;
		io_write8(layer_a_place, space_tile);
		t8 = layer_b_place;
		layer_b_place++;
		io_write8(t8, space_tile);
		t9 = groups_left;
		groups_left = (uint16_t)(t9 - 1u);
		fixed_place++;
		layer_a_place++;
	} while (t9 != 1u);
	return fixed_place;
}
