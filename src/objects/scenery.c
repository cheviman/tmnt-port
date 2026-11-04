/**
 *    Name:        objects/scenery.c
 *    Description: the areas' scenery and traps: balls, hydrants, signs, rats, oil drums, turrets, nozzles, electric beams, spiked poles, the parked car
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* an unseen spot high up that will drop four balls */
void ball_dropper_start(uint32_t object)
{
	uint32_t stats = 0;

	stats = ball_dropper_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->routine++;
	OBJECT(object)->ball_dropper.balls_left = 3u;
}

/* a ball drops each time the screen reaches the next place; the dropper is gone after the last */
void ball_dropper_wait(uint32_t object)
{
	uint32_t ball = 0, objects_left = 0;
	uint8_t t1 = 0;

	offset_high_word = 0u;
	if (OBJECT(object)->screen_x <= ball_drop_waits[OBJECT(object)->ball_dropper.balls_left])
	{
		ball = OBJECT_ADDRESS(4);
		objects_left = 8u;
		find_free_object(&objects_left, &ball);
		if ((uint8_t)objects_left != 0u)
		{
			ball_dropper_drop_ball(object, ball);
			t1 = OBJECT(object)->ball_dropper.balls_left - 1u;
			OBJECT(object)->ball_dropper.balls_left = t1;
			if ((int8_t)t1 < 0)
			{
				remove_object(object);
				return;
			}
		}
	}
}

/* a ball at the dropper's place */
void ball_dropper_drop_ball(uint32_t object, uint32_t object_2)
{
	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x60u;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	place_at_object(object, object_2);
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
}

/* a dark ball, high up */
void ball_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = ball_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = ball_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the ball drops and starts bouncing away, knocking turtles flying */
void ball_drop(uint32_t object)
{
	uint8_t sound = 0;

	OBJECT(object)->height -= 0x480u;
	if (OBJECT(object)->height < 0x3000u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->height = 0x3000u;
		OBJECT(object)->x_speed = 0x1E0;
		OBJECT(object)->y_speed = -0x1E0;
		OBJECT(object)->height_speed = 0x400;
		OBJECT(object)->contacts |= 0x80u;
		OBJECT(object)->shadow = ball_shadow_ADDRESS;
		sound = 0x24u;
		queue_sound(sound);
		OBJECT(object)->flags |= 4u;
	}
}

/* the ball bounces on until it is gone at the area's back */
void ball_bounce(uint32_t object)
{
	uint8_t sound = 0;

	move_by_speeds(object);
	OBJECT(object)->height_speed -= 0x60u;
	if ((int16_t)(uint16_t)(OBJECT(object)->y >> 16u) < 0)
	{
		remove_object(object);
		return;
	}
	if ((int8_t)OBJECT(object)->height_whole < 0)
	{
		OBJECT(object)->height_speed = 0x400;
		OBJECT(object)->height = 0u;
		sound = 0x24u;
		queue_sound(sound);
	}
}

/* a barrier across the highway, or one of its unseen parts that knock turtles down */
void road_barrier_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	stats = road_barrier_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->flags |= 4u;
	animation = road_barrier_animations[1];
	if (OBJECT(object)->parameter == 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->flags |= 0x80u;
		animation = road_barrier_animations[0];
	}
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the barrier stays until it leaves the screen */
void road_barrier_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* tires lying on the highway, two or one by the parameter */
void tire_stack_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = tire_stack_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = tire_stack_animations[0];
	if (OBJECT(object)->parameter != 0u)
		animation = tire_stack_animations[1];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the tires stay until they leave the screen */
void tire_stack_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* a ball high up in the Technodrome */
void technodrome_ball_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	technodrome_ball_drift(object);
	OBJECT(object)->colour = 9u;
	OBJECT(object)->animation_delay = 0u;
	OBJECT(object)->flags |= 0x80u;
	stats = technodrome_ball_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = ball_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the ball drops and starts bouncing off to the right, knocking turtles flying */
void technodrome_ball_drop(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t height = 0;

	technodrome_ball_drift(object);
	OBJECT(object)->x += 0x480u;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - 0x480u;
	if (height < 0x480u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->height = 0u;
		OBJECT(object)->x_speed = 0x3C0;
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->height_speed = 0x4E0;
		OBJECT(object)->contacts |= 0x80u;
		OBJECT(object)->shadow = ball_shadow_ADDRESS;
		OBJECT(object)->technodrome_ball.shadow_time = 0x32u;
		sound = 0x24u;
		queue_sound(sound);
		OBJECT(object)->flags |= 4u;
	}
}

/* the ball bounces on until it is past the screen's right edge */
void technodrome_ball_bounce(uint32_t object)
{
	uint8_t sound = 0;
	uint8_t shadow_time = 0;

	technodrome_ball_drift(object);
	move_by_speeds(object);
	if (OBJECT(object)->technodrome_ball.shadow_time != 0u)
	{
		shadow_time = OBJECT(object)->technodrome_ball.shadow_time;
		OBJECT(object)->technodrome_ball.shadow_time = shadow_time - 1u;
		if (shadow_time == 1u)
			OBJECT(object)->contacts &= 0x7Fu;
	}
	OBJECT(object)->height_speed -= 0x40u;
	if (OBJECT(object)->screen_x >= 0x1A0u)
	{
		remove_object(object);
		return;
	}
	if ((int8_t)OBJECT(object)->height_whole < 0)
	{
		OBJECT(object)->height_speed = 0x4E0;
		OBJECT(object)->height = 0u;
		sound = 0x24u;
		queue_sound(sound);
	}
}

/* the ball moves along the area's diagonal */
void technodrome_ball_drift(uint32_t object)
{
	OBJECT(object)->x += 0x400u;
	OBJECT(object)->y -= 0x400u;
}

/* a fire hydrant with four bars */
void hydrant_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x82u;
	stats = hydrant_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = hydrant_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the hydrant stays until it leaves the screen */
void hydrant_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* the broken hydrant sprays a jet of water every few frames until jets_left runs out */
void hydrant_spray(uint32_t object)
{
	uint32_t objects_left = 0, water = 0;
	uint8_t timer = 0, jets_left = 0;

	timer = OBJECT(object)->hydrant.timer;
	OBJECT(object)->hydrant.timer = timer - 1u;
	if (timer != 0u)
		return;
	jets_left = OBJECT(object)->hydrant.jets_left;
	OBJECT(object)->hydrant.jets_left = jets_left - 1u;
	if (jets_left == 0u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->hydrant.timer = 0u;
		OBJECT(object)->hydrant.jets_left = 0u;
		return;
	}
	water = OBJECT_ADDRESS(4);
	objects_left = 8u;
	find_free_object(&objects_left, &water);
	hydrant_spray_jet((uint8_t)objects_left, object, water);
}

/* a jet of water if there is room for it, else the spray ends */
void hydrant_spray_jet(uint8_t objects_left, uint32_t object, uint32_t water)
{
	if ((uint8_t)objects_left == 0u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->hydrant.timer = 0u;
		OBJECT(object)->hydrant.jets_left = 0u;
		return;
	}
	hydrant_start_water(object, water);
	OBJECT(object)->hydrant.timer = 4u;
}

/* a turtle's hit sparks on the hydrant, or breaks it and starts the spray the way the hit sent it */
void hydrant_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	if (OBJECT(object)->health_bars == 0u)
	{
		hydrant_sprays_left = 0u;
		animation = hydrant_animations[2];
		if ((int8_t)OBJECT(object)->hit < 0)
		{
			animation = hydrant_animations[3];
			hydrant_sprays_left = 1u;
		}
		set_animation(0u, object, animation);
		OBJECT(object)->routine = 2u;
		OBJECT(object)->hydrant.timer = 0u;
		OBJECT(object)->hydrant.jets_left = 4u;
		stats = hydrant_struck_stats_ADDRESS;
		set_object_stats(object, stats);
		sound = 5u;
		queue_sound(sound);
		OBJECT(object)->flags &= ~6u;
		return;
	}
	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->hydrant.timer = 0x28u;
	OBJECT(object)->flags &= ~2u;
	sound = 5u;
	queue_sound(sound);
	animation = hydrant_animations[1];
	set_animation(0u, object, animation);
}

/* the hydrant can be hit again */
void hydrant_recover(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->hydrant.timer;
	OBJECT(object)->hydrant.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->health = 1u;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->hydrant.timer = 8u;
		OBJECT(object)->flags |= 2u;
	}
}

/* a jet of water from the hydrant, weaker as the jets go */
void hydrant_start_water(uint32_t object, uint32_t water)
{
	uint32_t water_offset = 0;

	OBJECT(water)->flags = 1u;
	OBJECT(water)->contacts = 0u;
	OBJECT(water)->kind = 0x21u;
	OBJECT(water)->parameter = 0u;
	OBJECT(water)->colour = 0xFFu;
	OBJECT(water)->animation_delay = 0u;
	place_at_object(object, water);
	water_offset = 0x2000u;
	if (hydrant_sprays_left != 0u)
	{
		water_offset = (int16_t)~water_offset;
		offset_high_word = water_offset;
	}
	OBJECT(water)->x += (water_offset & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
	OBJECT(water)->height = 0xA00u;
	OBJECT(water)->hydrant_water.jet = 0u;
	if (OBJECT(object)->hydrant.jets_left != 3u)
		OBJECT(water)->hydrant_water.jet = 1u;
	if (OBJECT(object)->hydrant.jets_left == 0u)
		OBJECT(water)->hydrant_water.jet++;
	if (hydrant_sprays_left != 0u)
		OBJECT(water)->hydrant_water.jet |= 0x80u;
	OBJECT(water)->routine = 0u;
	OBJECT(water)->state = 0u;
}

/* a jet of water flies across, hitting what it meets */
void hydrant_water_start(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t jet = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags = 0x85u;
	stats = hydrant_water_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0x580;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->hydrant_water.jet < 0)
	{
		OBJECT(object)->x_speed = ~OBJECT(object)->x_speed;
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->attack &= 0x7Fu;
	}
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	offset_high_word = 0u;
	jet = OBJECT(object)->hydrant_water.jet;
	animation = hydrant_water_animations[jet & 3u];
	set_animation(0u, object, animation);
	sound = 0xEu;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the water flies on until it leaves the screen */
void hydrant_water_fly(uint32_t object)
{
	uint32_t x_speed = 0;

	remove_if_off_screen(object);
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	offset_high_word = x_speed;
}

/* a traffic cone */
void roadside_cone_start(uint32_t object)
{
	uint32_t stats = 0;

	stats = roadside_cone_stats_ADDRESS;
	roadside_stand(object, stats);
}

/* a parking meter or a speed limit sign */
void roadside_pole_start(uint32_t object)
{
	uint32_t stats = 0;

	stats = roadside_pole_stats_ADDRESS;
	roadside_stand(object, stats);
}

/* the thing stands by the road, and a turtle can hit it */
void roadside_stand(uint32_t object, uint32_t stats)
{
	uint32_t animation_entry = 0, animations = 0;

	set_object_stats(object, stats);
	OBJECT(object)->flags |= 0x82u;
	animations = parking_meter_animations_ADDRESS;
	if (OBJECT(object)->kind != 0x22u)
	{
		animations = speed_sign_animations_ADDRESS;
		if (OBJECT(object)->kind != 0x59u)
			animations = cone_animations_ADDRESS;
	}
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->routine++;
}

/* it stays until it leaves the screen */
void roadside_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* it flies on until it leaves the screen */
void roadside_fly(uint32_t object)
{
	uint32_t x_speed = 0;

	remove_if_off_screen(object);
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	offset_high_word = x_speed;
}

/* a turtle's hit sends the speed limit sign flying */
void roadside_speed_sign_struck(uint32_t object)
{
	uint32_t animations = 0;

	animations = speed_sign_animations_ADDRESS;
	OBJECT(object)->x_speed = 0x500;
	roadside_knock_away(object, animations);
}

/* a turtle's hit sends the traffic cone flying */
void roadside_cone_struck(uint32_t object)
{
	uint32_t animations = 0;

	animations = cone_animations_ADDRESS;
	OBJECT(object)->x_speed = 0x600;
	roadside_knock_away(object, animations);
}

/* a turtle's hit sends the parking meter flying */
void roadside_parking_meter_struck(uint32_t object)
{
	uint32_t animations = 0;

	animations = parking_meter_animations_ADDRESS;
	OBJECT(object)->x_speed = 0x400;
	roadside_knock_away(object, animations);
}

/* it flies off the way the hit sent it, knocking down what it hits */
void roadside_knock_away(uint32_t object, uint32_t animations)
{
	uint8_t sound = 0;
	uint32_t animation_entry = 0;

	if ((int8_t)OBJECT(object)->hit < 0)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->x_speed = ~OBJECT(object)->x_speed;
		OBJECT(object)->attack &= 0x7Fu;
	}
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	animation_entry = ANIMATION_ENTRY(animations + 0x4u);
	set_animation(0u, object, animation_entry);
	sound = 5u;
	queue_sound(sound);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->flags |= 4u;
}

/* a sewer rat, which drops in or runs */
void rat_start(uint32_t object)
{
	uint16_t x_speed = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	x_speed = 0x1C0u;
	OBJECT(object)->flags |= 0x40u;
	if ((int8_t)OBJECT(object)->parameter >= 0)
	{
		x_speed = (uint16_t)~x_speed;
		OBJECT(object)->flags &= 0xBFu;
	}
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->parameter &= 0x7Fu;
	OBJECT(object)->x -= rat_start_offsets[OBJECT(object)->parameter];
	animation = rat_animations[2];
	OBJECT(object)->routine = 3u;
	if (OBJECT(object)->height != 0u)
	{
		animation = rat_animations[0];
		OBJECT(object)->routine = 1u;
		OBJECT(object)->height_speed = 0x500;
	}
	set_animation(0u, object, animation);
	rat_random_run_time(object);
}

/* the rat drops in and sits */
void rat_drop(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height -= OBJECT(object)->height_speed;
	if (OBJECT(object)->height < 0x1800u)
	{
		animation = rat_animations[1];
		set_animation(0u, object, animation);
		OBJECT(object)->rat.timer = 0x40u;
		OBJECT(object)->routine++;
	}
}

/* the rat sits a while, then runs */
void rat_sit(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->rat.timer;
	OBJECT(object)->rat.timer = timer - 1u;
	if (timer == 0u)
	{
		animation = rat_animations[2];
		set_animation(0u, object, animation);
		rat_random_run_time(object);
		OBJECT(object)->routine++;
	}
}

/* the rat runs, and is gone once it leaves the screen */
void rat_run(uint32_t object)
{
	remove_if_off_screen(object);
	rat_run_step(object);
}

/* the rat runs a while, then stops with a squeak */
void rat_run_step(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, x_speed = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->rat.timer;
	OBJECT(object)->rat.timer = timer - 1u;
	if (timer != 0u)
	{
		x_speed = OBJECT(object)->x_speed;
		OBJECT(object)->x += x_speed;
		offset_high_word = x_speed;
		return;
	}
	animation = rat_animations[1];
	set_animation(0u, object, animation);
	sound = 0x32u;
	queue_sound(sound);
	OBJECT(object)->rat.timer = 0x40u;
	OBJECT(object)->routine--;
}

/* how long the rat runs, at random */
void rat_random_run_time(uint32_t object)
{
	uint32_t range = 0;

	range = 8u;
	range = random_below((uint16_t)range, object);
	OBJECT(object)->rat.timer = rat_run_times[range];
}

/* an alligator swims slowly in the sewer's water */
void alligator_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = alligator_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = alligator_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = -0x40;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->routine++;
}

/* the alligator swims, and snaps its jaws as it comes on */
void alligator_swim(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	move_across_screen(object);
	if (OBJECT(object)->screen_x >= 0xC0u)
		return;
	OBJECT(object)->routine++;
	sound = 0x12u;
	queue_sound(sound);
	animation = alligator_animations[1];
	set_animation(0u, object, animation);
}

/* the alligator swims on */
void alligator_snap(uint32_t object)
{
	move_across_screen(object);
}

/* it moves across until it leaves the screen */
void move_across_screen(uint32_t object)
{
	uint32_t x_speed = 0;

	remove_if_off_screen(object);
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	offset_high_word = x_speed;
}

/* a woman on a skateboard glides across the street */
void skateboarder_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x82u;
	stats = skateboarder_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = skateboarder_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = -0x200;
	OBJECT(object)->routine++;
	skateboarder_idle();
}

/* nothing */
void skateboarder_idle(void)
{
}

/* she glides on until she leaves the screen */
void skateboarder_glide(uint32_t object)
{
	move_across_screen(object);
}

/* a turtle's hit makes her duck with a cry */
void skateboarder_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->routine = 1u;
	sound = 0xBAu;
	queue_sound(sound);
	animation = skateboarder_animations[1];
	set_animation(0u, object, animation);
}

/* a pizza box, whose taking refills a turtle's health */
void pizza_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x84u;
	stats = pizza_stats_by_kind[OBJECT(object)->parameter];
	set_object_stats(object, stats);
	offset_high_word = 0u;
	animation = pizza_animations[OBJECT(object)->parameter];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	OBJECT(object)->pizza.announced = 0u;
}

/* as the pizza comes on the screen it is announced; a turtle that takes it gets its health back */
void pizza_wait(uint16_t on_right, uint32_t object)
{
	uint8_t sound = 0;
	uint16_t screen_y = 0;
	uint32_t animation = 0, screen_x = 0;

	if (OBJECT(object)->screen_x >= 0x170u || OBJECT(object)->pizza.announced != 0u)
	{
		remove_if_off_screen(object);
		if ((OBJECT(object)->flags & 8u) != 0u)
		{
			offset_high_word = 4u;
			animation = pizza_animations[(uint8_t)(4u + OBJECT(object)->parameter)];
			set_animation(screen_x, object, animation);
			OBJECT(object)->flags &= ~4u;
			sound = 0x19u;
			queue_sound(sound);
			OBJECT(object)->routine++;
		}
		return;
	}
	OBJECT(object)->pizza.announced = 1u;
	sound = 0xB7u;
	queue_sound(sound);
	on_right = (uint16_t)(on_right | 0xFFu);
	screen_x = (uint16_t)(OBJECT(object)->screen_x - 0xCu);
	screen_y = (uint16_t)(OBJECT(object)->screen_y - 0x20u);
	if (screen_y >= 0x300u)
		screen_y = 0u;
	say_pizza_time(on_right, (uint16_t)screen_x, screen_y, object);
	remove_if_off_screen(object);
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		offset_high_word = 4u;
		animation = pizza_animations[(uint8_t)(4u + OBJECT(object)->parameter)];
		set_animation(screen_x, object, animation);
		OBJECT(object)->flags &= ~4u;
		sound = 0x19u;
		queue_sound(sound);
		OBJECT(object)->routine++;
	}
}

/* the opened box is gone */
void pizza_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	remove_object(object);
}

/* a billboard high up, with Foot soldiers behind it */
void billboard_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = billboard_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->billboard.trigger_x = billboard_trigger_x[(frame_count & 6u) / 2u];
	OBJECT(object)->routine++;
}

/* two Foot soldiers jump down and the billboard topples onto the turtles */
void billboard_wait(uint32_t object)
{
	uint8_t soldier_number = 0;
	uint32_t animation = 0, objects_left = 0, soldier = 0, stats = 0;

	if (OBJECT(object)->billboard.trigger_x < OBJECT(object)->screen_x)
		return;
	soldier_number = 0u;
	soldier = OBJECT_ADDRESS(12);
	objects_left = 0xCu;
	find_free_object(&objects_left, &soldier);
	if ((uint8_t)objects_left != 0u)
	{
		billboard_start_soldier(soldier_number, object, soldier);
		soldier_number = 1u;
		soldier = OBJECT_ADDRESS(12);
		objects_left = 0xCu;
		find_free_object(&objects_left, &soldier);
		if ((uint8_t)objects_left != 0u)
			billboard_start_soldier(soldier_number, object, soldier);
	}
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->routine++;
	OBJECT(object)->height_speed = 0x480;
	OBJECT(object)->y -= 0x1000u;
	OBJECT(object)->x += 0x1000u;
	stats = billboard_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = billboard_animations[1];
	set_animation(0u, object, animation);
}

/* the billboard falls to the ground with a crash */
void billboard_fall(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t height_speed = 0;
	uint16_t height = 0;

	height_speed = (uint16_t)OBJECT(object)->height_speed;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - height_speed;
	if (height_speed <= height)
		return;
	OBJECT(object)->height = 0u;
	sound = 0x3Eu;
	queue_sound(sound);
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->routine++;
}

/* the fallen billboard stays in the background */
void billboard_land(uint32_t object)
{
	uint16_t place = 0;
	uint32_t block = 0;

	block = billboard_fallen_tile_block_ADDRESS;
	place = 0u;
	draw_tile_block(place, block);
	remove_object(object);
}

/* a Foot soldier on the billboard, counted as an enemy */
void billboard_start_soldier(uint8_t soldier_number, uint32_t object, uint32_t soldier)
{
	uint32_t x_offset = 0;

	OBJECT(soldier)->flags = 1u;
	OBJECT(soldier)->contacts = 0u;
	OBJECT(soldier)->kind = 0x2Au;
	OBJECT(soldier)->parameter = 0u;
	OBJECT(soldier)->colour = 6u;
	OBJECT(soldier)->animation_delay = 0u;
	place_at_object(object, soldier);
	x_offset = 0x2000u;
	if (soldier_number != 0u)
	{
		x_offset = (int16_t)~x_offset;
		offset_high_word = x_offset;
	}
	OBJECT(soldier)->x += (x_offset & 0xFFFFu) | (offset_high_word & ~0xFFFFu);
	OBJECT(soldier)->y += 0x100u;
	OBJECT(soldier)->billboard_soldier.flipped = 0u;
	if (soldier_number != 0u)
		OBJECT(soldier)->billboard_soldier.flipped |= 0x80u;
	enemies_on_screen++;
	OBJECT(soldier)->routine = 0u;
	OBJECT(soldier)->state = 0u;
}

/* the soldier crouches on the billboard */
void billboard_soldier_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags = 0x81u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	if ((int8_t)OBJECT(object)->billboard_soldier.flipped < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->billboard_soldier.timer = 0x28u;
	animation = billboard_soldier_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the soldier jumps */
void billboard_soldier_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->billboard_soldier.timer;
	OBJECT(object)->billboard_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		animation = billboard_soldier_animations[1];
		set_animation(0u, object, animation);
		OBJECT(object)->y_speed = 0x100;
		OBJECT(object)->height_speed = 0x400;
		OBJECT(object)->routine++;
		return;
	}
}

/* the soldier comes down to the ground and lands */
void billboard_soldier_jump_down(uint32_t object)
{
	uint32_t animation = 0, height = 0;
	uint16_t height_2 = 0;

	height = OBJECT(object)->y_speed;
	OBJECT(object)->y -= height;
	height = (height & ~0xFFFFu) | (uint16_t)OBJECT(object)->height_speed;
	height_2 = OBJECT(object)->height;
	OBJECT(object)->height = height_2 - height;
	if (height_2 < (uint16_t)height)
	{
		animation = billboard_soldier_animations[2];
		set_animation(0u, object, animation);
		OBJECT(object)->height = 0u;
		OBJECT(object)->billboard_soldier.timer = 0x14u;
		OBJECT(object)->routine++;
	}
	offset_high_word = height;
}

/* the soldier goes on as a Foot soldier with a boomerang */
void billboard_soldier_become_boomerang_soldier(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->billboard_soldier.timer;
	OBJECT(object)->billboard_soldier.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0xBu;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		enemies_on_screen--;
		return;
	}
}

/* an oil drum with three bars */
void oil_drum_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x82u;
	stats = oil_drum_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = oil_drum_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the drum, or its wreck, stays until it leaves the screen */
void oil_drum_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* the drum blows up, knocking flying what is near */
void oil_drum_blow_up(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, objects_left = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->oil_drum.timer;
	OBJECT(object)->oil_drum.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->oil_drum.timer = 0x1Eu;
		animation = oil_drum_animations[3];
		set_animation(0u, object, animation);
		object_2 = OBJECT_ADDRESS(4);
		objects_left = 8u;
		find_free_object(&objects_left, &object_2);
		if ((uint8_t)objects_left == 0u)
			return;
		OBJECT(object_2)->flags = 5u;
		OBJECT(object_2)->contacts = 0u;
		OBJECT(object_2)->kind = 0x33u;
		OBJECT(object_2)->parameter = 0u;
		OBJECT(object_2)->colour = 0xFFu;
		OBJECT(object_2)->animation_delay = 0u;
		place_at_object(object, object_2);
		OBJECT(object_2)->y -= 0x200u;
		OBJECT(object_2)->count = 0u;
		OBJECT(object_2)->routine = 0u;
		OBJECT(object_2)->state = 0u;
		sound = 0x13u;
		queue_sound(sound);
		return;
	}
}

/* the burnt wreck of the drum is left */
void oil_drum_become_wreck(uint32_t object)
{
	uint32_t animation = 0, stats = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->oil_drum.timer;
	OBJECT(object)->oil_drum.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags &= ~6u;
		stats = oil_drum_stats_ADDRESS;
		set_object_stats(object, stats);
		animation = oil_drum_animations[4];
		set_animation(0u, object, animation);
		OBJECT(object)->kind = 0x2Du;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
	}
}

/* a turtle's hit makes the drum flash, or lights its fuse */
void oil_drum_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (OBJECT(object)->health_bars == 0u)
	{
		sound = 5u;
		queue_sound(sound);
		animation = oil_drum_animations[2];
		set_animation(0u, object, animation);
		OBJECT(object)->routine = 2u;
		OBJECT(object)->oil_drum.timer = 0x3Cu;
		OBJECT(object)->flags &= ~6u;
		return;
	}
	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->oil_drum.timer = 0x28u;
	OBJECT(object)->flags &= ~2u;
	sound = 5u;
	queue_sound(sound);
	animation = oil_drum_animations[1];
	set_animation(0u, object, animation);
}

/* the drum can be hit again */
void oil_drum_recover(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->oil_drum.timer;
	OBJECT(object)->oil_drum.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->health = 1u;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->flags |= 2u;
	}
}

/* a gun on a tall column, with four bars */
void turret_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->shadow = ball_shadow_ADDRESS;
	stats = turret_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = turret_animations[0];
	set_animation(0u, object, animation);
	if ((int8_t)OBJECT(object)->parameter < 0)
		OBJECT(object)->flags |= 0x40u;
	turret_random_wait(object);
	OBJECT(object)->turret.phase = 0u;
	OBJECT(object)->routine++;
}

/* the turret slides, waiting to fire */
void turret_move(uint32_t object)
{
	uint8_t timer = 0;

	turret_slide(object);
	timer = OBJECT(object)->turret.timer;
	OBJECT(object)->turret.timer = timer - 1u;
	if (timer == 0u)
	{
		OBJECT(object)->turret.timer = 0u;
		OBJECT(object)->turret.shots_left = 4u;
		OBJECT(object)->routine++;
		return;
	}
}

/* the turret slides and fires a burst of four shots */
void turret_fire(uint32_t object)
{
	uint32_t objects_left = 0, shot = 0;
	uint8_t timer = 0, shots_left = 0;

	turret_slide(object);
	timer = OBJECT(object)->turret.timer;
	OBJECT(object)->turret.timer = timer - 1u;
	if (timer != 0u)
		return;
	shots_left = OBJECT(object)->turret.shots_left;
	OBJECT(object)->turret.shots_left = shots_left - 1u;
	if (shots_left != 0u)
	{
		shot = OBJECT_ADDRESS(32);
		objects_left = 8u;
		find_free_object(&objects_left, &shot);
		if ((uint8_t)objects_left != 0u)
		{
			turret_start_shot(object, shot);
			OBJECT(object)->turret.timer = 3u;
			return;
		}
	}
	OBJECT(object)->routine--;
	turret_random_wait(object);
	OBJECT(object)->turret.shots_left = 0u;
}

/* the destroyed turret's explosions */
void turret_wreck(uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		turret_wreck_explode(object);
		return;
	case 1:
		turret_wreck_next(object);
		return;
	case 2:
		turret_wreck_stay(object);
		return;
	}
}

/* an explosion on the destroyed turret's column */
void turret_wreck_explode(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t height_offset = 0, sound_choice = 0, x_offset = 0;
	uint32_t explosion = 0, objects_left = 0;

	OBJECT(object)->turret.timer = 4u;
	OBJECT(object)->state++;
	explosion = OBJECT_ADDRESS(24);
	objects_left = 8u;
	find_free_object(&objects_left, &explosion);
	if ((uint8_t)objects_left == 0u)
	{
		OBJECT(object)->state = 2u;
		return;
	}
	x_offset = 0u;
	height_offset = turret_wreck_waits[OBJECT(object)->turret.phase];
	start_small_explosion(x_offset, height_offset, object, explosion);
	offset_high_word = 0u;
	sound_choice = OBJECT(object)->turret.phase;
	sound = turret_wreck_sounds[sound_choice & 3u];
	object_loop_leftover = turret_wreck_waits_ADDRESS;
	queue_sound(sound);
}

/* the next explosion, higher up, until seven */
void turret_wreck_next(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->turret.timer;
	OBJECT(object)->turret.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state--;
		OBJECT(object)->turret.phase++;
		if (OBJECT(object)->turret.phase == 7u)
			OBJECT(object)->state = 2u;
	}
}

/* the wreck stays until it leaves the screen */
void turret_wreck_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* a turtle's hit makes the turret flash, or destroys it */
void turret_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	if (OBJECT(object)->health_bars == 0u)
	{
		stats = turret_struck_stats_ADDRESS;
		set_object_stats(object, stats);
		animation = turret_animations[2];
		set_animation(0u, object, animation);
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->turret.phase = 0u;
		OBJECT(object)->flags &= ~6u;
		return;
	}
	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->turret.timer = 0x28u;
	OBJECT(object)->flags &= ~2u;
	sound = 5u;
	queue_sound(sound);
	animation = turret_animations[1];
	set_animation(0u, object, animation);
}

/* the turret can be hit again */
void turret_recover(uint32_t object)
{
	uint8_t timer = 0;

	turret_slide(object);
	timer = OBJECT(object)->turret.timer;
	OBJECT(object)->turret.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->health = 1u;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->flags |= 2u;
	}
}

/* the turret slides back and forth in depth */
void turret_slide(uint32_t object)
{
	uint16_t phase_offset = 0;
	uint32_t slide = 0, slide_kind = 0, x = 0, y = 0;

	remove_if_off_screen(object);
	slide_kind = OBJECT(object)->parameter & 3u;
	slide_kind = (uint16_t)(slide_kind << 3);
	slide = turret_slides_ADDRESS + slide_kind;
	phase_offset = OBJECT(object)->turret.phase;
	phase_offset = (uint16_t)((phase_offset & 1u) << 2);
	x = OFFSET_PAIR(slide + (phase_offset / 4u) * 0x4u)->x;
	y = OFFSET_PAIR(slide + (phase_offset / 4u) * 0x4u)->y;
	OBJECT(object)->x += x;
	OBJECT(object)->y += y;
	if ((OBJECT(object)->parameter & 0x40u) != 0u)
	{
		if (OBJECT(object)->y >= 0x7000u || OBJECT(object)->y < 0x1000u)
			OBJECT(object)->turret.phase ^= 1u;
	}
	else if (OBJECT(object)->y >= 0x7000u || OBJECT(object)->y < 0x1000u)
	{
		OBJECT(object)->turret.phase ^= 1u;
		offset_high_word = y;
		return;
	}
	offset_high_word = y;
}

/* how long the turret waits before its next burst, at random */
void turret_random_wait(uint32_t object)
{
	uint32_t range = 0;

	range = 4u;
	range = random_below((uint16_t)range, object);
	OBJECT(object)->turret.timer = turret_waits[range];
}

/* a shot from the turret's gun */
void turret_start_shot(uint32_t object, uint32_t shot)
{
	OBJECT(shot)->flags = 1u;
	OBJECT(shot)->contacts = 0u;
	OBJECT(shot)->kind = 0x2Cu;
	OBJECT(shot)->parameter = 0u;
	OBJECT(shot)->colour = 0xFFu;
	OBJECT(shot)->animation_delay = 0u;
	place_at_object(object, shot);
	OBJECT(shot)->routine = 0u;
	OBJECT(shot)->state = 0u;
	OBJECT(shot)->height += 0x800u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(shot)->turret_shot.flipped = 1u;
		OBJECT(shot)->x += 0x2000u;
		return;
	}
	OBJECT(shot)->turret_shot.flipped = 0u;
	OBJECT(shot)->x -= 0x2000u;
}

/* the turret's shot flies the way the turret faces, knocking turtles down */
void turret_shot_start(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t x_speed = 0;
	uint32_t animation = 0, shadow = 0, stats = 0;

	OBJECT(object)->flags = 0x85u;
	OBJECT(object)->contacts |= 0x80u;
	shadow = turret_shot_shadow_ADDRESS;
	if (stage == 8u)
		shadow = turret_shot_technodrome_shadow_ADDRESS;
	OBJECT(object)->shadow = shadow;
	stats = turret_shot_stats_ADDRESS;
	set_object_stats(object, stats);
	x_speed = 0xF800u;
	OBJECT(object)->flags &= 0xBFu;
	if (OBJECT(object)->turret_shot.flipped != 0u)
	{
		x_speed = (uint16_t)~x_speed;
		OBJECT(object)->flags |= 0x40u;
	}
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	animation = turret_shot_animations[0];
	set_animation(0u, object, animation);
	sound = 0xBu;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the shot flies until it hits or leaves the screen */
void turret_shot_fly(uint32_t object)
{
	uint32_t animation = 0, x_speed = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		remove_object(object);
		return;
	}
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->contacts &= 0x7Fu;
		animation = turret_shot_animations[1];
		set_animation(0u, object, animation);
		return;
	}
	x_speed = OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	offset_high_word = x_speed;
}

/* the shot's burst ends and it is gone */
void turret_shot_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		remove_object(object);
		return;
	}
}

/* a Foot soldier standing with his back turned */
void waiting_soldier_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->colour = 7u;
	OBJECT(object)->animation_delay = 0u;
	stats = waiting_soldier_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->routine++;
	animation = waiting_soldier_animations[0];
	set_animation(0u, object, animation);
}

/* the soldier waits until he is well on the screen */
void waiting_soldier_wait(uint32_t object)
{
	if (OBJECT(object)->screen_x < 0x140u)
	{
		waiting_soldier_become_knife_soldier(object);
		return;
	}
	remove_if_off_screen(object);
}

/* the soldier goes on as a Foot soldier with knives */
void waiting_soldier_become_knife_soldier(uint32_t object)
{
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	OBJECT(object)->kind = 3u;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->routine = 0u;
	OBJECT(object)->state = 0u;
}

/* a ONE WAY sign on a pole */
void one_way_sign_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x82u;
	stats = one_way_sign_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = one_way_sign_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the sign stays until it leaves the screen */
void one_way_sign_stay(uint32_t object)
{
	remove_if_off_screen(object);
}

/* the sign spins, knocking flying what it hits, until it stops */
void one_way_sign_spin(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
	{
		one_way_sign_idle();
		return;
	}
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->routine--;
	one_way_sign_idle();
}

/* nothing */
void one_way_sign_idle(void)
{
}

/* a turtle's hit sets the sign spinning */
void one_way_sign_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation_entry = 0, animations = 0;

	animations = one_way_sign_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->hit < 0)
	{
		OBJECT(object)->attack &= 0x7Fu;
		animations += 4u;
	}
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	animation_entry = ANIMATION_ENTRY(animations + 0x4u);
	set_animation(0u, object, animation_entry);
	sound = 5u;
	queue_sound(sound);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->health_bars = 0x10u;
	OBJECT(object)->health = 1u;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->flags &= ~2u;
}

/* the marks of a row of nozzles in the Technodrome's floor */
void nozzle_row_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = nozzle_row_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->nozzle_row.nozzle_status = 0u;
	OBJECT(object)->routine++;
}

/* the row watches for turtles, waits for its nozzle, or is done */
void nozzle_row_run(uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->nozzle_row.nozzle_status)
	{
	case 0:
		nozzle_row_watch(object);
		return;
	case 1:
		nozzle_row_wait(object);
		return;
	case 2:
		nozzle_row_done(object);
		return;
	}
}

/* a turtle near one of the row's places makes a nozzle come up there */
void nozzle_row_watch(uint32_t object)
{
	uint16_t count = 0, count_2 = 0, x_distance = 0, y_distance = 0;
	uint32_t nozzle = 0, nozzle_place = 0, object_2 = 0, objects_left = 0, row = 0;
	uint16_t screen_x = 0, screen_x_2 = 0, screen_y = 0, t1 = 0, t2 = 0, t3 = 0;

	remove_if_off_screen(object);
	row = nozzle_rows[OBJECT(object)->parameter];
	offset_high_word = 0u;
	count = NOZZLE_PLACE(row);
	row++;
	for (;;)
	{
		nozzle_place = NOZZLE_PLACE(row);
		count_2 = 3u;
		object_2 = OBJECT_ADDRESS(3);
		object_loop_leftover = OBJECT_ADDRESS(3);
		row++;
		do
		{
			if ((players_in_play & shl32(1u, (int)(count_2 & 7u))) != 0u && OBJECT(object_2)->routine != 4u)
			{
				screen_x = OBJECT(object_2)->screen_x;
				screen_x_2 = OBJECT(object)->screen_x;
				x_distance = (uint16_t)(screen_x_2 - screen_x);
				distance_low_byte = x_distance;
				if (screen_x <= screen_x_2 && x_distance < 0x40u)
				{
					screen_y = OBJECT(object_2)->screen_y;
					t1 = OBJECT(object)->screen_y - nozzle_place;
					y_distance = (uint16_t)(t1 - screen_y);
					distance_low_byte = y_distance;
					if (t1 < screen_y)
					{
						y_distance = (uint16_t)~y_distance;
						distance_low_byte = y_distance;
					}
					if (y_distance < 8u)
					{
						nozzle = OBJECT_ADDRESS(4);
						objects_left = 8u;
						find_free_object(&objects_left, &nozzle);
						if ((uint8_t)objects_left != 0u)
						{
							nozzle_row_start_nozzle(nozzle_place, object, nozzle);
							OBJECT(object)->nozzle_row.nozzle_status = 1u;
						}
						return;
					}
				}
			}
			object_2 -= 0x50u;
			object_loop_leftover = object_2;
			t2 = count_2 - 1u;
			count_2 = t2;
		} while (t2 != 0xFFFFu);
		t3 = count - 1u;
		count = t3;
		if (t3 == 0xFFFFu)
			return;
	}
}

/* a nozzle is up */
void nozzle_row_wait(uint32_t object)
{
	remove_if_off_screen(object);
}

/* the row's nozzle was broken: no more come up */
void nozzle_row_done(uint32_t object)
{
	remove_if_off_screen(object);
}

/* a nozzle at one of the row's places */
void nozzle_row_start_nozzle(uint32_t nozzle_place, uint32_t object, uint32_t nozzle)
{
	uint32_t row_status = 0;

	OBJECT(nozzle)->flags = 1u;
	OBJECT(nozzle)->contacts = 0u;
	OBJECT(nozzle)->kind = 0x2Eu;
	OBJECT(nozzle)->parameter = 0u;
	OBJECT(nozzle)->colour = 0xFFu;
	OBJECT(nozzle)->animation_delay = 0u;
	place_at_object(object, nozzle);
	nozzle_place = (uint16_t)(nozzle_place << 8);
	OBJECT(nozzle)->y -= nozzle_place;
	row_status = object + 0x2Fu;
	OBJECT(nozzle)->freeze_nozzle.row_status = row_status;
	OBJECT(nozzle)->count = 0u;
	OBJECT(nozzle)->routine = 0u;
	OBJECT(nozzle)->state = 0u;
	object_loop_leftover = row_status;
}

/* a nozzle comes up out of the floor */
void freeze_nozzle_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = freeze_nozzle_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = freeze_nozzle_animations[1];
	set_animation(0u, object, animation);
	OBJECT(object)->freeze_nozzle.timer = 0x15u;
	OBJECT(object)->routine++;
}

/* the nozzle is up and can be hit */
void freeze_nozzle_rise(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->freeze_nozzle.timer;
	OBJECT(object)->freeze_nozzle.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->freeze_nozzle.timer = 0xAu;
		OBJECT(object)->routine++;
		OBJECT(object)->state = 0u;
		return;
	}
}

/* the nozzle gets ready, then sprays */
void freeze_nozzle_spray(uint32_t object)
{
	offset_high_word = 0u;
	switch (OBJECT(object)->state)
	{
	case 0:
		freeze_nozzle_spray_start(object);
		return;
	case 1:
		freeze_nozzle_spray_stop(object);
		return;
	}
}

/* the nozzle sprays a cloud that freezes turtles in ice */
void freeze_nozzle_spray_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->freeze_nozzle.timer;
	OBJECT(object)->freeze_nozzle.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->freeze_nozzle.timer = 0x3Cu;
	OBJECT(object)->state++;
	sound = 0x1Fu;
	queue_sound(sound);
	animation = freeze_nozzle_animations[2];
	set_animation(0u, object, animation);
}

/* the spray stops and the nozzle sinks */
void freeze_nozzle_spray_stop(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->freeze_nozzle.timer;
	OBJECT(object)->freeze_nozzle.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->flags &= ~6u;
	OBJECT(object)->freeze_nozzle.timer = 0x15u;
	OBJECT(object)->routine++;
	OBJECT(object)->state = 0u;
	animation = freeze_nozzle_animations[3];
	set_animation(0u, object, animation);
}

/* the nozzle is gone and its row watches again */
void freeze_nozzle_sink(uint32_t object)
{
	uint32_t row_status = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->freeze_nozzle.timer;
	OBJECT(object)->freeze_nozzle.timer = timer - 1u;
	if (timer == 1u)
	{
		remove_object(object);
		row_status = OBJECT(object)->freeze_nozzle.row_status;
		OBJECT(row_status - 0x2Fu)->nozzle_row.nozzle_status = 0u;
		return;
	}
}

/* a turtle's hit breaks the nozzle in a small explosion */
void freeze_nozzle_struck(uint32_t object)
{
	uint8_t sound = 0, struck_sound = 0;
	uint16_t height_offset = 0, x_offset = 0;
	uint32_t animation = 0, explosion = 0, objects_left = 0, row_status = 0;

	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->flags &= ~6u;
	OBJECT(object)->freeze_nozzle.timer = 0u;
	row_status = OBJECT(object)->freeze_nozzle.row_status;
	OBJECT(row_status - 0x2Fu)->nozzle_row.nozzle_status = 2u;
	struck_sound = 5u;
	queue_sound(struck_sound);
	animation = freeze_nozzle_animations[4];
	set_animation(0u, object, animation);
	explosion = OBJECT_ADDRESS(24);
	objects_left = 8u;
	find_free_object(&objects_left, &explosion);
	if ((uint8_t)objects_left != 0u)
	{
		x_offset = 0u;
		height_offset = 0x1000u;
		start_small_explosion(x_offset, height_offset, object, explosion);
		sound = 0x11u;
		queue_sound(sound);
		return;
	}
}

/* the broken nozzle stays until it leaves the screen */
void freeze_nozzle_broken(uint32_t object)
{
	remove_if_off_screen(object);
}

/* an electric beam, off for a while */
void electric_beam_start(uint32_t object)
{
	uint32_t stats = 0, waits = 0;

	OBJECT(object)->flags = 1u;
	stats = electric_beam_stats_ADDRESS;
	set_object_stats(object, stats);
	object_loop_leftover = electric_beam_waits_ADDRESS;
	waits = electric_beam_waits_ADDRESS;
	electric_random_wait(object, waits);
	OBJECT(object)->routine++;
}

/* the beam comes on and electrocutes turtles */
void electric_beam_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint8_t timer = 0;

	remove_if_off_screen(object);
	if (OBJECT(object)->flags == 0u)
		return;
	timer = OBJECT(object)->electric.timer;
	OBJECT(object)->electric.timer = timer - 1u;
	if (timer != 0u)
		return;
	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->electric.timer = 0x3Cu;
	OBJECT(object)->routine++;
	sound = 0x20u;
	queue_sound(sound);
	animation = electric_beam_animations[0];
	set_animation(0u, object, animation);
}

/* the beam stays on a second, then goes off again */
void electric_beam_shine(uint32_t object)
{
	uint8_t timer = 0;

	remove_if_off_screen(object);
	if (OBJECT(object)->flags != 0u)
	{
		timer = OBJECT(object)->electric.timer;
		OBJECT(object)->electric.timer = timer - 1u;
		if (timer == 0u)
		{
			OBJECT(object)->routine = 0u;
			electric_beam_start(object);
			return;
		}
	}
}

/* a spot in the floor that shoots electric columns */
void electric_vent_start(uint32_t object)
{
	uint32_t stats = 0, waits = 0;

	OBJECT(object)->flags = 1u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	object_loop_leftover = electric_waits_ADDRESS;
	waits = electric_waits_ADDRESS;
	electric_random_wait(object, waits);
	OBJECT(object)->routine++;
}

/* now and then the vent shoots an electric column */
void electric_vent_wait(uint32_t object)
{
	uint32_t column = 0, objects_left = 0, waits = 0;
	uint8_t timer = 0;

	remove_if_off_screen(object);
	timer = OBJECT(object)->electric.timer;
	OBJECT(object)->electric.timer = timer - 1u;
	if (timer == 0u)
	{
		object_loop_leftover = electric_waits_ADDRESS;
		waits = electric_waits_ADDRESS;
		electric_random_wait(object, waits);
		column = OBJECT_ADDRESS(32);
		objects_left = 8u;
		find_free_object(&objects_left, &column);
		if ((uint8_t)objects_left != 0u)
		{
			electric_vent_start_column(object, column);
			OBJECT(object)->routine++;
			OBJECT(object)->electric.column_done = 0u;
		}
	}
}

/* the vent waits for its column to be gone */
void electric_vent_wait_column(uint32_t object)
{
	if (OBJECT(object)->electric.column_done != 0u)
	{
		OBJECT(object)->routine--;
		OBJECT(object)->electric.column_done = 0u;
	}
}

/* how long the electric hazard waits, at random */
void electric_random_wait(uint32_t object, uint32_t waits)
{
	uint32_t range = 0;

	range = 8u;
	range = random_below((uint16_t)range, object);
	OBJECT(object)->electric.timer = WAIT_TIME(waits + (int32_t)range);
}

/* an electric column at the vent */
void electric_vent_start_column(uint32_t vent, uint32_t column)
{
	OBJECT(column)->flags = 1u;
	OBJECT(column)->contacts = 0u;
	OBJECT(column)->kind = 0x3Au;
	OBJECT(column)->parameter = 0u;
	OBJECT(column)->colour = 0xFFu;
	OBJECT(column)->animation_delay = 0u;
	place_at_object(vent, column);
	OBJECT(column)->y -= 0x1000u;
	OBJECT(column)->electric.vent = vent;
	OBJECT(column)->electric.column_done = 0u;
	OBJECT(column)->routine = 0u;
	OBJECT(column)->state = 0u;
}

/* an electric column shoots up out of the floor, electrocuting turtles */
void electric_column_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x84u;
	stats = electric_column_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = electric_column_animations[0];
	set_animation(0u, object, animation);
	sound = 0x1Bu;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the column is up */
void electric_column_rise(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->electric.timer = 0x30u;
		OBJECT(object)->routine++;
	}
}

/* the column blinks a while, then goes */
void electric_column_flicker(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	electric_column_blink(object);
	timer = OBJECT(object)->electric.timer;
	OBJECT(object)->electric.timer = timer - 1u;
	if (timer == 0u)
	{
		animation = electric_column_animations[1];
		set_animation(0u, object, animation);
		OBJECT(object)->routine++;
		OBJECT(object)->flags |= 0x80u;
	}
}

/* the column goes on up out of sight and tells its vent */
void electric_column_leave(uint32_t object)
{
	uint32_t vent = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		vent = OBJECT(object)->electric.vent;
		OBJECT(vent)->electric.column_done = 1u;
		remove_object(object);
		return;
	}
	OBJECT(object)->y -= 0x1000u;
}

/* the column shows on every other frame */
void electric_column_blink(uint32_t object)
{
	OBJECT(object)->flags |= 0x80u;
	if ((frame_count & 1u) == 0u)
		OBJECT(object)->flags &= 0x7Fu;
}

/* Unused: a slanted grille */
void grille_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->routine++;
	unused_grille = object;
	animation = grille_animations[0];
	set_animation(0u, object, animation);
}

/* Unused: nothing */
void grille_idle(void)
{
}

/* a spiked pole hangs over the sewer */
void spiked_pole_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	if ((OBJECT(object)->parameter & 1u) != 0u)
		OBJECT(object)->flags |= 4u;
	stats = spiked_pole_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	spiked_poles_phase = 0u;
	animation = spiked_pole_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
}

/* the first pole on the screen starts the row dropping */
void spiked_pole_wait(uint32_t object)
{
	spiked_pole_swap_hurting(object);
	if ((int8_t)OBJECT(object)->parameter < 0 && OBJECT(object)->screen_x < 0x160u)
		spiked_poles_phase = 1u;
	if (spiked_poles_phase == 1u)
	{
		OBJECT(object)->routine++;
		OBJECT(object)->spiked_pole.timer = 0xAu;
	}
}

/* the poles wait up a while, then drop */
void spiked_pole_hold_up(uint32_t object)
{
	uint8_t timer = 0;

	spiked_pole_swap_hurting(object);
	if (OBJECT(object)->parameter != 0u)
	{
		timer = OBJECT(object)->spiked_pole.timer;
		OBJECT(object)->spiked_pole.timer = timer - 1u;
		if (timer == 0u)
			spiked_poles_phase = 2u;
	}
	if (spiked_poles_phase == 2u)
	{
		OBJECT(object)->height_speed = 0x600;
		OBJECT(object)->routine++;
	}
	remove_if_off_screen(object);
}

/* the pole drops to just over the floor */
void spiked_pole_drop(uint32_t object)
{
	uint8_t sound = 0;

	spiked_pole_swap_hurting(object);
	OBJECT(object)->height -= OBJECT(object)->height_speed;
	if (OBJECT(object)->height < 0x1800u)
	{
		OBJECT(object)->height = 0x1800u;
		OBJECT(object)->routine++;
		if (OBJECT(object)->parameter != 0u)
		{
			OBJECT(object)->spiked_pole.timer = 0x28u;
			spiked_poles_phase = 3u;
			sound = 0x24u;
			queue_sound(sound);
		}
	}
}

/* the poles wait down a while, then rise */
void spiked_pole_hold_down(uint32_t object)
{
	uint8_t timer = 0;

	spiked_pole_swap_hurting(object);
	if (OBJECT(object)->parameter != 0u)
	{
		timer = OBJECT(object)->spiked_pole.timer;
		OBJECT(object)->spiked_pole.timer = timer - 1u;
		if (timer == 0u)
			spiked_poles_phase = 4u;
	}
	if (spiked_poles_phase == 4u)
	{
		OBJECT(object)->height_speed = 0x300;
		OBJECT(object)->routine++;
	}
	remove_if_off_screen(object);
}

/* the pole rises back up */
void spiked_pole_rise(uint32_t object)
{
	spiked_pole_swap_hurting(object);
	OBJECT(object)->height += OBJECT(object)->height_speed;
	if (OBJECT(object)->height >= 0x8000u)
	{
		OBJECT(object)->height = 0x8000u;
		OBJECT(object)->routine = 2u;
		if (OBJECT(object)->parameter != 0u)
		{
			OBJECT(object)->spiked_pole.timer = 0x1Eu;
			spiked_poles_phase = 0u;
		}
	}
}

/* the poles that hurt and the ones that do not swap now and then */
void spiked_pole_swap_hurting(uint32_t object)
{
	if ((frame_count & 0xFu) == 0u)
		OBJECT(object)->flags ^= 4u;
}

/* something lies hidden under the sewer's water */
void sewer_launcher_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags &= 0x7Fu;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->sewer_launcher.delay = 0x28u;
	OBJECT(object)->sewer_launcher.missiles_left = 0xCu;
	animation = sewer_launcher_animations[1];
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	OBJECT(object)->sewer_launcher.drift = 0u;
}

/* it drifts under the water; with a turtle in line, a missile leaps out at it */
void sewer_launcher_wait(uint32_t object)
{
	uint8_t kept_turtle = 0;
	uint32_t animation = 0, missile = 0, objects_left = 0, turtle_in_line = 0;

	remove_if_off_screen(object);
	if (OBJECT(object)->flags == 0u)
	{
		sewer_launcher_remove(object);
		return;
	}
	sewer_launcher_drift(object);
	if (OBJECT(object)->sewer_launcher.delay != 0u)
	{
		OBJECT(object)->sewer_launcher.delay--;
		remove_if_off_screen(object);
	}
	else
	{
		turtle_in_line = sewer_launcher_find_turtle(object);
		if ((uint8_t)turtle_in_line != 0u)
		{
			kept_turtle = (uint8_t)turtle_in_line;
			missile = OBJECT_ADDRESS(32);
			objects_left = 8u;
			find_free_object(&objects_left, &missile);
			if ((uint8_t)objects_left != 0u)
			{
				sewer_launcher_start_missile(kept_turtle, object, missile);
				OBJECT(object)->routine++;
				OBJECT(object)->flags |= 0x80u;
				animation = sewer_launcher_animations[2];
				set_animation(0u, object, animation);
				return;
			}
		}
	}
}

/* its ripples end and it hides a while; out of missiles, it is gone */
void sewer_launcher_fired(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t missiles_left = 0;

	remove_if_off_screen(object);
	if (OBJECT(object)->flags == 0u)
	{
		sewer_launcher_remove(object);
		return;
	}
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->sewer_launcher.delay = 0x64u;
	OBJECT(object)->routine--;
	missiles_left = OBJECT(object)->sewer_launcher.missiles_left;
	OBJECT(object)->sewer_launcher.missiles_left = missiles_left - 1u;
	if (missiles_left == 0u)
	{
		sewer_launcher_remove(object);
		return;
	}
	animation = sewer_launcher_animations[1];
	set_animation(0u, object, animation);
}

/* the launcher is gone */
void sewer_launcher_remove(uint32_t object)
{
	remove_object(object);
}

/* it drifts up and down the screen under the water */
void sewer_launcher_drift(uint32_t object)
{
	uint16_t drift_offset = 0;
	uint32_t x_step = 0, y_step = 0;

	drift_offset = OBJECT(object)->sewer_launcher.drift;
	drift_offset = (uint16_t)((drift_offset & 1u) << 2);
	x_step = sewer_launcher_drift_x[drift_offset / 2u];
	y_step = sewer_launcher_drift_y[drift_offset / 2u];
	OBJECT(object)->x += x_step;
	OBJECT(object)->y += y_step;
	if (OBJECT(object)->y >= 0x4000u || OBJECT(object)->y < 0x800u)
		OBJECT(object)->sewer_launcher.drift ^= 1u;
	offset_high_word = y_step;
}

/* which turtle is in line with it across the water, far enough */
uint32_t sewer_launcher_find_turtle(uint32_t object)
{
	uint16_t count = 0, x_distance = 0, y_distance = 0;
	uint32_t object_2 = 0, turtle_in_line = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	for (;;)
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_knocked_out & shl32(1u, (int)(count & 7u))) == 0u && OBJECT(object_2)->routine != 4u)
		{
			t1 = OBJECT(object_2)->screen_y - OBJECT(object)->screen_y;
			y_distance = t1;
			distance_low_byte = y_distance;
			if ((int16_t)t1 < 0)
			{
				y_distance = (uint16_t)~y_distance;
				distance_low_byte = y_distance;
			}
			if (y_distance < 4u)
			{
				turtle_in_line = 0u;
				t2 = OBJECT(object_2)->screen_x - OBJECT(object)->screen_x;
				x_distance = t2;
				distance_low_byte = x_distance;
				if ((int16_t)t2 < 0)
				{
					x_distance = (uint16_t)~x_distance;
					distance_low_byte = x_distance;
					turtle_in_line = 0x80u;
				}
				if (x_distance >= 0x30u)
					break;
			}
		}
		turtle_in_line = 0u;
		offset_high_word = 0u;
		t3 = count - 1u;
		count = t3;
		object_2 -= 0x50u;
		if (t3 == 0xFFFFu)
			return turtle_in_line;
	}
	turtle_in_line |= shl32(1u, (int)(count & 0x1Fu));
	offset_high_word = turtle_in_line;
	return turtle_in_line;
}

/* a missile leaps out at the turtle */
void sewer_launcher_start_missile(uint8_t turtle_in_line, uint32_t object, uint32_t missile)
{
	OBJECT(missile)->flags = 1u;
	OBJECT(missile)->contacts = 0u;
	OBJECT(missile)->kind = 0x50u;
	OBJECT(missile)->parameter = 0u;
	OBJECT(missile)->colour = 0xFFu;
	OBJECT(missile)->animation_delay = 0u;
	place_at_object(object, missile);
	OBJECT(missile)->routine = 0u;
	OBJECT(missile)->state = 0u;
	OBJECT(missile)->sewer_missile.goes_right = 0u;
	if ((int8_t)turtle_in_line >= 0)
	{
		OBJECT(missile)->sewer_missile.goes_right = 1u;
		OBJECT(missile)->flags |= 0x40u;
	}
}

/* the missile rises out of the water */
void sewer_missile_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x86u;
	stats = sewer_missile_stats_ADDRESS;
	set_object_stats(object, stats);
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->attack |= 0x80u;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0x200;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->routine++;
	animation = sewer_launcher_animations[0];
	set_animation(0u, object, animation);
	sound = 0xEu;
	queue_sound(sound);
}

/* the missile rises, then flies at the turtle */
void sewer_missile_rise(uint32_t object)
{
	uint16_t x_speed = 0;

	move_by_speeds(object);
	if (OBJECT(object)->height < 0x2000u)
		return;
	OBJECT(object)->routine++;
	x_speed = 0xFB80u;
	if (OBJECT(object)->sewer_missile.goes_right != 0u)
		x_speed = (uint16_t)~x_speed;
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
}

/* the missile flies across until it leaves the screen */
void sewer_missile_fly(uint32_t object)
{
	move_by_speeds(object);
	remove_if_off_screen(object);
}

/* the missile is gone */
void sewer_missile_remove(uint32_t object)
{
	remove_object(object);
}

/* a turtle's attack bursts the missile */
void sewer_missile_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->routine++;
	sound = 0xFu;
	queue_sound(sound);
	animation = explosion_small_animations[0];
	set_animation(0u, object, animation);
}

/* the missile's burst ends and it is gone */
void sewer_missile_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		remove_object(object);
		return;
	}
}

/* the car's spot shows a piece of wall; the car's picture will go into the layer */
void parked_car_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = parked_car_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->parked_car.scroll_x = 0u;
	OBJECT(object)->parked_car.scroll_y = 0u;
	OBJECT(object)->parked_car.blank_row = TILE_RAM + 0x2F80u;
	parked_car_set_scroll(object);
	OBJECT(object)->count = 0u;
	OBJECT(object)->routine++;
}

/* the parked car is drawn into the layer, where the object is */
void parked_car_draw(uint32_t object)
{
	uint16_t place = 0;
	uint32_t block = 0;

	OBJECT(object)->parked_car.last_screen_x = OBJECT(object)->screen_x;
	parked_car_follow_object(object);
	block = parked_car_tile_block_ADDRESS;
	place = 5u;
	draw_tile_block(place, block);
	OBJECT(object)->routine++;
}

/* the car waits until it is well on the screen */
void parked_car_wait(uint32_t object)
{
	parked_car_follow_object(object);
	if (OBJECT(object)->screen_x < 0xE0u)
	{
		OBJECT(object)->parked_car.counter = 0xAu;
		OBJECT(object)->routine++;
		OBJECT(object)->state = 0u;
	}
}

/* after a moment the car's three unseen hit boxes start */
void parked_car_start_drive(uint32_t object)
{
	uint16_t count = 0;
	uint32_t hit_box = 0, objects_left = 0;
	uint8_t counter = 0;
	uint16_t t1 = 0;

	parked_car_follow_object(object);
	counter = OBJECT(object)->parked_car.counter;
	OBJECT(object)->parked_car.counter = counter - 1u;
	if (counter != 1u)
		return;
	count = 2u;
	do
	{
		hit_box = OBJECT_ADDRESS(4);
		objects_left = 8u;
		find_free_object(&objects_left, &hit_box);
		if ((uint8_t)objects_left == 0u)
			break;
		parked_car_start_hit_box(count, object, hit_box);
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->parked_car.counter = 0u;
	OBJECT(object)->routine++;
	OBJECT(object)->state = 0u;
}

/* the car drives off to the front right, its picture blanked row by row as it goes */
void parked_car_drive_off(uint32_t object)
{
	OBJECT(object)->parked_car.counter++;
	if (OBJECT(object)->parked_car.counter >= 0x38u)
	{
		OBJECT(object)->parked_car.counter = 3u;
		if (turtles_in_play >= 2u)
			OBJECT(object)->parked_car.counter = 4u;
		OBJECT(object)->parked_car.soldier_time = 0u;
		OBJECT(object)->routine++;
		OBJECT(object)->state = 0u;
		return;
	}
	if ((OBJECT(object)->parked_car.counter & 1u) == 0u)
		parked_car_blank_row(object);
	OBJECT(object)->parked_car.scroll_x -= 4u;
	OBJECT(object)->parked_car.scroll_x &= 0x1FFu;
	OBJECT(object)->parked_car.scroll_y = (OBJECT(object)->parked_car.scroll_y & 0xFF00u) | ((uint8_t)OBJECT(object)->parked_car.scroll_y - 4u & 0xFFu);
	parked_car_follow_object(object);
}

/* Foot soldiers leap out one after another, then the car's spot is gone */
void parked_car_release_soldiers(uint32_t object)
{
	uint32_t objects_left = 0, soldier = 0;
	uint8_t soldier_time = 0, counter = 0;

	soldier_time = OBJECT(object)->parked_car.soldier_time;
	OBJECT(object)->parked_car.soldier_time = soldier_time - 1u;
	if (soldier_time != 0u)
		return;
	counter = OBJECT(object)->parked_car.counter;
	OBJECT(object)->parked_car.counter = counter - 1u;
	if (counter != 0u)
	{
		soldier = OBJECT_ADDRESS(12);
		objects_left = 0xCu;
		find_free_object(&objects_left, &soldier);
		if ((uint8_t)objects_left != 0u)
		{
			parked_car_start_soldier(object, soldier);
			OBJECT(object)->parked_car.soldier_time = 8u;
			return;
		}
	}
	remove_object(object);
}

/* the car's picture keeps with the object as the screen scrolls */
void parked_car_follow_object(uint32_t object)
{
	uint16_t screen_step = 0;

	screen_step = (uint16_t)(OBJECT(object)->parked_car.last_screen_x - OBJECT(object)->screen_x);
	OBJECT(object)->parked_car.last_screen_x = OBJECT(object)->screen_x;
	OBJECT(object)->parked_car.scroll_x += screen_step;
	OBJECT(object)->parked_car.scroll_x &= 0x1FFu;
	parked_car_set_scroll(object);
}

/* the layer's scroll for the car's picture */
void parked_car_set_scroll(uint32_t object)
{
	layer_a_scroll_x = OBJECT(object)->parked_car.scroll_x;
	layer_a_scroll_y = OBJECT(object)->parked_car.scroll_y;
}

/* a row of the layer blanked, then the one above next time */
void parked_car_blank_row(uint32_t object)
{
	uint16_t count = 0, tile_offset = 0;
	uint32_t blank_row = 0;
	uint16_t t1 = 0;

	blank_row = OBJECT(object)->parked_car.blank_row;
	count = 0x3Fu;
	tile_offset = 0u;
	do
	{
		io_write8(blank_row + (int16_t)tile_offset, 0u);
		io_write8(blank_row + 1u + (int16_t)tile_offset, 0x10u);
		tile_offset += 2u;
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->parked_car.blank_row = (OBJECT(object)->parked_car.blank_row & 0xFFFF0000u) | ((uint16_t)OBJECT(object)->parked_car.blank_row - 0x80u & 0xFFFFu);
}

/* an unseen hit box along the car's body */
void parked_car_start_hit_box(uint16_t count, uint32_t object, uint32_t object_2)
{
	uint32_t box_number = 0, place = 0, y = 0;

	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x36u;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	place_at_object(object, object_2);
	box_number = count;
	box_number = (uint16_t)(box_number << 2);
	place = car_hit_box_places_ADDRESS + box_number;
	OBJECT(object_2)->x += OFFSET_PAIR(place)->x;
	y = OFFSET_PAIR(place)->y;
	OBJECT(object_2)->y += y;
	OBJECT(object_2)->count = 0u;
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
	offset_high_word = y;
	object_loop_leftover = place;
}

/* an unseen box that knocks flying the turtles in the car's way */
void car_hit_box_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags = 5u;
	stats = car_hit_box_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0x400;
	OBJECT(object)->height_speed = 0;
	animation = parked_car_animations[1];
	set_animation(0u, object, animation);
	sound = 0x3Au;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the box goes with the car until it is off the screen */
void car_hit_box_move(uint32_t object)
{
	uint32_t x_speed = 0, y_speed = 0;

	if (OBJECT(object)->screen_x < 0x10u)
	{
		remove_object(object);
		return;
	}
	x_speed = (uint16_t)OBJECT(object)->x_speed;
	OBJECT(object)->x += x_speed;
	y_speed = (uint16_t)OBJECT(object)->y_speed;
	OBJECT(object)->y -= y_speed;
	offset_high_word = y_speed;
}

/* a Foot soldier where the car stood */
void parked_car_start_soldier(uint32_t object, uint32_t soldier)
{
	OBJECT(soldier)->flags = 1u;
	OBJECT(soldier)->contacts = 0u;
	OBJECT(soldier)->kind = 0x38u;
	OBJECT(soldier)->parameter = 0u;
	OBJECT(soldier)->colour = 5u;
	OBJECT(soldier)->animation_delay = 0u;
	place_at_object(object, soldier);
	OBJECT(soldier)->x += 0x1000u;
	OBJECT(soldier)->y += 0x2000u;
	OBJECT(soldier)->car_soldier.number = OBJECT(object)->parked_car.counter;
	OBJECT(soldier)->routine = 0u;
	OBJECT(soldier)->state = 0u;
}

/* a Foot soldier leaps out */
void car_soldier_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	OBJECT(object)->flags = 0x81u;
	stats = inert_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0x300;
	OBJECT(object)->y_speed = -0x300;
	OBJECT(object)->height_speed = 0;
	offset_high_word = 0u;
	OBJECT(object)->car_soldier.stop_y = car_soldier_stop_y[OBJECT(object)->car_soldier.number];
	animation = parked_car_animations[3];
	set_animation(0u, object, animation);
	enemies_on_screen++;
	OBJECT(object)->routine++;
}

/* the soldier leaps to his place, then is a gun soldier */
void car_soldier_leap(uint32_t object)
{
	if (OBJECT(object)->screen_y < OBJECT(object)->car_soldier.stop_y)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 9u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		enemies_on_screen--;
		return;
	}
	move_by_speeds(object);
}
