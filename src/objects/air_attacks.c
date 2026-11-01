/**
 *    Name:        objects/air_attacks.c
 *    Description: the helicopters of the skateboard ride, the missiles that fall on idle turtles, the flying drones
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a helicopter comes in the air, its sound on */
uint32_t helicopter_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, d7 = 0, shadow = 0, stats = 0;

	OBJECT(object)->flags |= 0x86u;
	stats = helicopter_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = helicopter_animations[0];
	if ((OBJECT(object)->parameter & 1u) != 0u)
		animation = helicopter_animations[1];
	set_animation(0u, object, animation);
	sound = 0xAu;
	queue_sound(sound);
	OBJECT(object)->helicopter.stay_time = 0x384u;
	OBJECT(object)->contacts |= 0x80u;
	shadow = helicopter_shadow_ADDRESS;
	if ((int8_t)OBJECT(object)->parameter < 0)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->attack |= 0x80u;
		shadow = helicopter_flipped_shadow_ADDRESS;
	}
	OBJECT(object)->shadow = shadow;
	d7 = helicopter_random_wait(object);
	OBJECT(object)->helicopter.drift = 0u;
	OBJECT(object)->routine++;
	enemies_on_screen++;
	helicopter_count++;
	return d7;
}

/* the helicopter hovers in front of the turtles until it fires */
void helicopter_hover(uint32_t object, uint32_t *p_in_line, uint32_t *p_count)
{
	uint8_t timer = 0;

	helicopter_bob(object);
	helicopter_fly(object, p_in_line, p_count);
	timer = OBJECT(object)->helicopter.timer;
	OBJECT(object)->helicopter.timer = timer - 1u;
	if (timer != 0u)
	{
		return;
	}
	OBJECT(object)->helicopter.timer = 0u;
	if ((OBJECT(object)->parameter & 1u) != 0u)
	{
		OBJECT(object)->helicopter.shots_left = 3u;
		OBJECT(object)->routine += 2u;
		return;
	}
	OBJECT(object)->helicopter.shots_left = 4u;
	OBJECT(object)->routine++;
}

/* the helicopter fires a burst of bullets */
void helicopter_fire_bullets(uint32_t object, uint32_t *p_in_line, uint32_t *p_d7)
{
	uint32_t bullet = 0;
	uint8_t timer = 0, shots_left = 0;

	helicopter_bob(object);
	helicopter_fly(object, p_in_line, p_d7);
	timer = OBJECT(object)->helicopter.timer;
	OBJECT(object)->helicopter.timer = timer - 1u;
	if (timer != 0u)
	{
		return;
	}
	shots_left = OBJECT(object)->helicopter.shots_left;
	OBJECT(object)->helicopter.shots_left = shots_left - 1u;
	if (shots_left == 0u)
	{
		OBJECT(object)->routine--;
		*p_d7 = helicopter_random_wait(object);
		OBJECT(object)->helicopter.shots_left = 0u;
		return;
	}
	bullet = OBJECT_ADDRESS(32);
	*p_d7 = 8u;
	find_free_object(p_d7, &bullet);
	if ((uint8_t)*p_d7 == 0u)
	{
		OBJECT(object)->routine--;
		*p_d7 = helicopter_random_wait(object);
		OBJECT(object)->helicopter.shots_left = 0u;
		return;
	}
	*p_d7 = helicopter_start_bullet(object, bullet);
	OBJECT(object)->helicopter.timer = 5u;
}

/* the helicopter drops its missiles */
void helicopter_drop_missiles(uint32_t object, uint32_t *p_in_line, uint32_t *p_d7)
{
	uint32_t missile = 0;
	uint8_t timer = 0, shots_left = 0;

	helicopter_bob(object);
	helicopter_fly(object, p_in_line, p_d7);
	timer = OBJECT(object)->helicopter.timer;
	OBJECT(object)->helicopter.timer = timer - 1u;
	if (timer != 0u)
	{
		return;
	}
	shots_left = OBJECT(object)->helicopter.shots_left;
	OBJECT(object)->helicopter.shots_left = shots_left - 1u;
	if (shots_left == 0u)
	{
		OBJECT(object)->routine -= 2u;
		*p_d7 = helicopter_random_wait(object);
		OBJECT(object)->helicopter.shots_left = 0u;
		return;
	}
	missile = OBJECT_ADDRESS(24);
	*p_d7 = (*p_d7 & ~0xFFu) | 8u;
	*p_d7 = (*p_d7 & ~0xFFFFu) | (uint8_t)*p_d7;
	find_free_object(p_d7, &missile);
	if ((uint8_t)*p_d7 == 0u)
	{
		OBJECT(object)->routine -= 2u;
		*p_d7 = helicopter_random_wait(object);
		OBJECT(object)->helicopter.shots_left = 0u;
		return;
	}
	helicopter_start_missile(object, missile);
	OBJECT(object)->helicopter.timer = 7u;
}

/* a hit makes the helicopter shake, or the last one destroys it in an explosion */
uint32_t helicopter_struck(uint32_t object)
{
	uint16_t x_offset = 0;
	uint32_t height_offset = 0, object_2 = 0, objects_left = 0, sound = 0;

	sound = helicopter_bob(object);
	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->helicopter.drift = 0u;
		OBJECT(object)->flags &= ~6u;
		if (helicopter_count == 1u)
		{
			sound = 9u;
			queue_sound((uint8_t)sound);
		}
		object_2 = OBJECT_ADDRESS(24);
		objects_left = (sound & ~0xFFu) | 8u;
		objects_left = (uint8_t)objects_left;
		find_free_object(&objects_left, &object_2);
		if ((uint8_t)objects_left == 0u)
			return objects_left;
		x_offset = 0u;
		height_offset = 0u;
		OBJECT(object_2)->flags = 1u;
		OBJECT(object_2)->contacts = 0u;
		OBJECT(object_2)->kind = 0x32u;
		OBJECT(object_2)->parameter = 0u;
		OBJECT(object_2)->colour = 0xFFu;
		OBJECT(object_2)->animation_delay = 2u;
		height_offset = place_explosion(x_offset, (uint16_t)height_offset, object, object_2);
		sound = (height_offset & ~0xFFu) | 0x11u;
		queue_sound((uint8_t)sound);
		return sound;
	}
	OBJECT(object)->colour = 9u;
	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->helicopter.timer = 0x28u;
	OBJECT(object)->flags &= ~2u;
	sound = (sound & ~0xFFu) | 5u;
	queue_sound((uint8_t)sound);
	return sound;
}

/* the helicopter gets over the hit */
uint32_t helicopter_recover(uint32_t object)
{
	uint32_t d7 = 0;
	uint8_t timer = 0;

	d7 = helicopter_shake(object);
	if (OBJECT(object)->helicopter.timer == 0x20u)
		OBJECT(object)->colour = 0xFFu;
	timer = OBJECT(object)->helicopter.timer;
	OBJECT(object)->helicopter.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->health = 1u;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->flags |= 2u;
	}
	return d7;
}

/* the wreck stays behind until it leaves the screen */
void helicopter_destroyed(uint32_t object)
{
	remove_if_off_screen(object);
	if (OBJECT(object)->flags == 0u)
	{
		enemies_on_screen--;
		helicopter_count--;
	}
}

/* the helicopter drifts in depth and keeps in front of the turtles; after a while it falls behind and is
   gone */
void helicopter_fly(uint32_t object, uint32_t *p_in_line, uint32_t *p_count)
{
	uint32_t in_line = *p_in_line;
	uint16_t drift = 0, hover_offset = 0, screen_x = 0, y_distance = 0;
	uint32_t count = 0, object_2 = 0;
	uint16_t t1 = 0, t2 = 0, stay_time = 0;

	drift = OBJECT(object)->helicopter.drift;
	OBJECT(object)->y += helicopter_drift_y[drift & 1u];
	if (OBJECT(object)->y >= 0x7800u || OBJECT(object)->y < 0x1000u)
		OBJECT(object)->helicopter.drift ^= 1u;
	object_2 = OBJECT_ADDRESS(3);
	count = 3u;
	screen_x = 0u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u && (players_knocked_out & shl32(1u, (int)(count & 7u))) == 0u && OBJECT(object_2)->routine != 4u)
		{
			if ((OBJECT(object)->flags & 0x40u) != 0u)
			{
				if (screen_x == 0u)
					screen_x = 0x1FFu;
				if (OBJECT(object_2)->screen_x <= screen_x)
				{
					screen_x = OBJECT(object_2)->screen_x;
					in_line = 0u;
					t1 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
					y_distance = t1;
					if ((int16_t)t1 < 0)
						y_distance = (uint16_t)~y_distance;
					if (y_distance < 6u)
						in_line = 1u;
				}
			}
			else if (screen_x < OBJECT(object_2)->screen_x)
			{
				screen_x = OBJECT(object_2)->screen_x;
				in_line = 0u;
				t1 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
				y_distance = t1;
				if ((int16_t)t1 < 0)
					y_distance = (uint16_t)~y_distance;
				if (y_distance < 6u)
					in_line = 1u;
			}
		}
		t2 = count - 1u;
		count = t2;
		object_2 -= 0x50u;
	} while (t2 != 0xFFFFu);
	stay_time = OBJECT(object)->helicopter.stay_time;
	OBJECT(object)->helicopter.stay_time = stay_time - 1u;
	if (stay_time != 0u)
	{
		if (screen_x != 0u)
		{
			hover_offset = (uint16_t)((frame_count & 0x3Fu) + 0x10u);
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				hover_offset = (uint16_t)~hover_offset;
			screen_x += hover_offset;
			count = 0xFF80u;
			if ((uint8_t)in_line != 0u)
				count = (uint16_t)(count << 2);
			count = (int16_t)count;
			if (OBJECT(object)->screen_x <= screen_x)
				count = ~count;
			OBJECT(object)->x += count;
		}
	}
	else
	{
		OBJECT(object)->helicopter.stay_time = 0u;
		OBJECT(object)->x -= 0x100u;
		if (OBJECT(object)->screen_x < 0x10u)
		{
			enemies_on_screen--;
			helicopter_count--;
			if (helicopter_count == 0u)
			{
				count = 9u;
				queue_sound((uint8_t)count);
			}
			remove_object(object);
		}
	}
	*p_in_line = in_line;
	*p_count = count;
}

/* the hit helicopter shakes, carried with the ride */
uint32_t helicopter_shake(uint32_t object)
{
	uint32_t height = 0, shake_offset = 0;

	OBJECT(object)->x += 0x600u;
	shake_offset = (uint8_t)frame_count;
	shake_offset = shake_offset & 6u;
	height = helicopter_shake_heights[shake_offset / 2u];
	height = (uint16_t)(height + 0x4000u);
	OBJECT(object)->height = height;
	return height;
}

/* the helicopter bobs in the air, carried with the ride */
uint32_t helicopter_bob(uint32_t object)
{
	uint32_t bob_offset = 0, height = 0;

	bob_offset = (uint8_t)frame_count;
	bob_offset = bob_offset & 0x3Cu;
	bob_offset = bob_offset >> 1;
	height = TABLE16(helicopter_bob_heights, bob_offset);
	height = (uint16_t)(height + 0x4000u);
	OBJECT(object)->height = height;
	carry_with_ride(object);
	return height;
}

/* in the skateboard ride the object moves on with the scroll */
void carry_with_ride(uint32_t object)
{
	if (stage == 6u)
		OBJECT(object)->x += 0x600u;
}

/* how long until the helicopter fires, at random */
uint32_t helicopter_random_wait(uint32_t object)
{
	uint32_t difficulty_level = 0, range = 0, waits = 0;

	range = 4u;
	range = random_below((uint16_t)range, object);
	waits = helicopter_waits_ADDRESS;
	if ((OBJECT(object)->parameter & 1u) != 0u)
		waits = helicopter_waits_2_ADDRESS;
	OBJECT(object)->helicopter.timer = WAIT_TIME(waits + (int32_t)range);
	difficulty_level = IN_PAGE(range, difficulty_setting);
	difficulty_level = (difficulty_level & ~0xFFu) | (uint8_t)(difficulty_level << 3);
	OBJECT(object)->helicopter.timer -= difficulty_level;
	return difficulty_level;
}

/* a bullet from the front of the helicopter */
uint32_t helicopter_start_bullet(uint32_t object, uint32_t object_2)
{
	uint32_t bullet_offset = 0;

	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x44u;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	place_at_object(object, object_2);
	OBJECT(object_2)->y--;
	OBJECT(object_2)->height -= 0x2200u;
	bullet_offset = 0x3800u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(object_2)->flags |= 0x40u;
		bullet_offset = 0xFFFFD500u;
	}
	OBJECT(object_2)->x -= bullet_offset;
	OBJECT(object_2)->count = 0u;
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
	return bullet_offset;
}

/* a bullet flies down at a slant, knocking turtles down */
void copter_bullet_start(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t x_speed = 0;
	uint32_t animation = 0, stats = 0;

	carry_with_ride(object);
	OBJECT(object)->flags |= 0x84u;
	stats = copter_bullet_stats_ADDRESS;
	set_object_stats(object, stats);
	x_speed = 0xFB00u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(object)->attack |= 0x80u;
		x_speed = (uint16_t)~x_speed;
	}
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = -0x500;
	animation = copter_bullet_animations[0];
	set_animation(0u, object, animation);
	sound = 0x17u;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the bullet bursts on what it hits, or sparks on the road */
uint32_t copter_bullet_fly(uint32_t object)
{
	uint32_t animation = 0, d7 = 0;

	carry_with_ride(object);
	d7 = move_by_speeds(object);
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->routine += 2u;
		OBJECT(object)->animation_delay = 2u;
		animation = turret_shot_animations[1];
		set_animation(0u, object, animation);
		return d7;
	}
	if ((int16_t)OBJECT(object)->height < 0)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->routine++;
		animation = copter_bullet_animations[2];
		set_animation(0u, object, animation);
		return d7;
	}
	return d7;
}

/* the bullet's burst goes on with the ride */
void copter_bullet_burst(uint32_t object)
{
	carry_with_ride(object);
	copter_bullet_vanish(object);
}

/* the bullet is gone when its spark or burst ends */
void copter_bullet_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		remove_object(object);
		return;
	}
}

/* a missile from the helicopter */
void helicopter_start_missile(uint32_t object, uint32_t object_2)
{
	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x46u;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	place_at_object(object, object_2);
	OBJECT(object_2)->height += 0x1000u;
	OBJECT(object_2)->count = 0u;
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
}

/* a missile drops from above */
void falling_missile_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, stats = 0;

	carry_with_ride(object);
	OBJECT(object)->flags |= 0x84u;
	stats = falling_missile_stats_ADDRESS;
	if (OBJECT(object)->parameter != 0u)
		stats = falling_missile_at_turtle_stats_ADDRESS;
	set_object_stats(object, stats);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0x300;
	if (OBJECT(object)->parameter != 0u)
		OBJECT(object)->height_speed = 0x800;
	animation = copter_bullet_animations[3];
	if (OBJECT(object)->parameter != 0u)
		animation = falling_missile_animations[0];
	set_animation(0u, object, animation);
	sound = 3u;
	queue_sound(sound);
	OBJECT(object)->routine++;
	enemies_on_screen++;
}

/* the missile drops until it hits the ground and explodes */
void falling_missile_fall(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t height_speed = 0;
	uint32_t animation = 0;
	uint16_t height = 0;

	carry_with_ride(object);
	height_speed = (uint16_t)OBJECT(object)->height_speed;
	height = OBJECT(object)->height;
	OBJECT(object)->height = height - height_speed;
	if (height_speed <= height)
		return;
	OBJECT(object)->height = 0u;
	OBJECT(object)->routine++;
	sound = 0x13u;
	queue_sound(sound);
	animation = rocket_animations[0];
	set_animation(0u, object, animation);
}

/* the explosion ends and the missile is gone */
void falling_missile_explode(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemies_on_screen--;
	remove_object(object);
}

/* missiles drop on the turtles that dawdle */
void rain_missiles_on_turtles(uint32_t *p_count, uint32_t *p_objects_left)
{
	uint32_t objects_left = *p_objects_left;
	uint32_t count = 0, missile = 0, turtle = 0;
	uint16_t t1 = 0;

	turtle = OBJECT_ADDRESS(3);
	count = 3u;
	do
	{
		if ((players_in_play & shl32(1u, (int)(count & 7u))) != 0u)
		{
			missile = OBJECT_ADDRESS(12);
			objects_left = (objects_left & ~0xFFu) | 0xCu;
			objects_left = (objects_left & ~0xFFFFu) | (uint8_t)objects_left;
			find_free_object(&objects_left, &missile);
			if ((uint8_t)objects_left != 0u)
				objects_left = start_falling_missile_at_turtle(turtle, missile);
		}
		t1 = count - 1u;
		count = t1;
		turtle -= 0x50u;
	} while (t1 != 0xFFFFu);
	*p_count = count;
	*p_objects_left = objects_left;
}

/* a missile high above a turtle */
uint32_t start_falling_missile_at_turtle(uint32_t object, uint32_t object_2)
{
	uint32_t y = 0;

	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x46u;
	OBJECT(object_2)->parameter = 1u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	OBJECT(object_2)->x = spawn_x - 0x1A000u + ((uint32_t)OBJECT(object)->screen_x << 8);
	y = spawn_y - 0x10000u + ((uint32_t)OBJECT(object)->screen_y << 8);
	OBJECT(object_2)->y = y;
	OBJECT(object_2)->height = 0xF000u;
	OBJECT(object_2)->count = 0u;
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
	return y;
}

/* the drones wait far back */
void start_flying_drones(void)
{
	uint16_t count = 0;
	uint32_t drone = 0, objects_left = 0;
	uint16_t t1 = 0;

	count = 6u;
	do
	{
		drone = OBJECT_ADDRESS(12);
		objects_left = 0xCu;
		find_free_object(&objects_left, &drone);
		if ((uint8_t)objects_left != 0u)
			start_flying_drone(count, drone);
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	flying_drones_released = 0u;
	flying_drone_count = 0u;
}

/* a drone with its own way to fly */
void start_flying_drone(uint16_t aim, uint32_t drone)
{
	OBJECT(drone)->flags = 1u;
	OBJECT(drone)->contacts = 0u;
	OBJECT(drone)->kind = 0x54u;
	OBJECT(drone)->parameter = 0u;
	OBJECT(drone)->colour = 0xFFu;
	OBJECT(drone)->animation_delay = 0u;
	OBJECT(drone)->x = 0x54C00u;
	OBJECT(drone)->y = 0x13800u;
	OBJECT(drone)->height = 0u;
	OBJECT(drone)->flying_drone.direction = TABLE8(flying_drone_starts, (uint32_t)(int16_t)aim);
	OBJECT(drone)->flying_drone.heading &= 0xFF00u;
	OBJECT(drone)->flying_drone.turning = TABLE8(flying_drone_starts, 0x8u + (uint32_t)(int16_t)aim);
	OBJECT(drone)->flying_drone.turn_speed = TABLE8(flying_drone_starts, 0x10u + (uint32_t)(int16_t)aim);
	OBJECT(drone)->flying_drone.aim = (uint8_t)aim;
	OBJECT(drone)->routine = 0u;
	OBJECT(drone)->state = 0u;
}

/* the drones come out, their sound on */
void release_flying_drones(void)
{
	flying_drones_released = 1u;
	drone_sound_on = 1u;
	drone_sound_wait = 0u;
}

/* the drone comes in, by the state of its way in */
void flying_drone_enter(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t a5 = 0;

	switch (OBJECT(object)->state)
	{
	case 0:
		*p_d7 = flying_drone_start(object);
		return;
	case 1:
		*p_d7 = flying_drone_wait(object);
		return;
	case 2:
		*p_d7 = flying_drone_take_off(object);
		return;
	case 3:
		*p_a5 = a5;
		flying_drone_ready(object, p_d7, p_a5);
		return;
	}
}

/* the drone rests */
uint32_t flying_drone_start(uint32_t object)
{
	uint32_t animation = 0, d7 = 0, stats = 0;

	OBJECT(object)->flags |= 0x80u;
	enemies_on_screen++;
	flying_drone_count++;
	OBJECT(object)->state++;
	OBJECT(object)->flying_drone.bob = (uint32_t)OBJECT(object)->flying_drone.aim << 2;
	d7 = flying_drone_bob_low(object);
	OBJECT(object)->flying_drone.target = 0u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	stats = flying_drone_stats_ADDRESS;
	set_object_stats(object, stats);
	animation = flying_drone_animations[12];
	set_animation(0u, object, animation);
	return d7;
}

/* the drone bobs until it is released, then takes off */
uint32_t flying_drone_wait(uint32_t object)
{
	uint32_t d7 = 0;

	d7 = flying_drone_bob_low(object);
	if (flying_drones_released != 0u)
	{
		OBJECT(object)->state++;
		OBJECT(object)->contacts |= 0x80u;
		OBJECT(object)->shadow = flying_drone_shadow_ADDRESS;
		OBJECT(object)->x = 0x54C00u;
		OBJECT(object)->y = 0x8800u;
		OBJECT(object)->flying_drone.timer = 0x12u;
		d7 = flying_drone_animate(object);
	}
	return d7;
}

/* the drone flies out on its own way, rising */
uint32_t flying_drone_take_off(uint32_t object)
{
	uint32_t d7 = 0;
	uint8_t timer = 0;

	d7 = flying_drone_move(object);
	timer = OBJECT(object)->flying_drone.timer;
	OBJECT(object)->flying_drone.timer = timer - 1u;
	if (timer == 0u)
	{
		OBJECT(object)->state++;
		return d7;
	}
	OBJECT(object)->height += 0x100u;
	if (OBJECT(object)->height >= 0x2C00u)
		OBJECT(object)->height = 0x2C00u;
	return d7;
}

/* the drone picks a turtle and can be hit */
void flying_drone_ready(uint32_t object, uint32_t *p_d7, uint32_t *p_choices)
{
	uint32_t ignored = 0, picked = 0;

	OBJECT(object)->flags |= 2u;
	boss_pick_target(object, &ignored, &picked, p_choices);
	if ((uint8_t)picked == 0u)
		OBJECT(object)->flying_drone.target = 0u;
	*p_d7 = flying_drone_random_wait(object);
	OBJECT(object)->flying_drone.time_left = 0x708u;
	OBJECT(object)->routine++;
	OBJECT(object)->state = 0u;
}

/* the drone weaves at its turtle until it fires; after a while it rises out of sight */
void flying_drone_fly(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t ignored = 0;
	uint8_t timer = 0;

	if (OBJECT(object)->flying_drone.time_left != 0u)
	{
		OBJECT(object)->flying_drone.time_left--;
		flying_drone_bob(object);
		flying_drone_steer(object, &ignored, p_a5);
		*p_d7 = flying_drone_animate(object);
		timer = OBJECT(object)->flying_drone.timer;
		OBJECT(object)->flying_drone.timer = timer - 1u;
		if (timer == 0u)
		{
			OBJECT(object)->flying_drone.timer = 0u;
			OBJECT(object)->flying_drone.shots_left = 4u;
			OBJECT(object)->routine++;
			return;
		}
		return;
	}
	flying_drone_steer(object, &ignored, p_a5);
	*p_d7 = flying_drone_animate(object);
	OBJECT(object)->height_whole++;
	if (OBJECT(object)->height_whole < 0xE0u)
	{
		return;
	}
	enemies_on_screen--;
	flying_drone_count--;
	remove_object(object);
}

/* the drone fires a burst of shots */
void flying_drone_fire(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t shot = 0;
	uint8_t timer = 0, shots_left = 0;

	*p_d7 = flying_drone_bob(object);
	flying_drone_steer(object, p_d7, p_a5);
	timer = OBJECT(object)->flying_drone.timer;
	OBJECT(object)->flying_drone.timer = timer - 1u;
	if (timer != 0u)
	{
		return;
	}
	shots_left = OBJECT(object)->flying_drone.shots_left;
	OBJECT(object)->flying_drone.shots_left = shots_left - 1u;
	if (shots_left == 0u)
	{
		OBJECT(object)->routine--;
		*p_d7 = flying_drone_random_wait(object);
		OBJECT(object)->flying_drone.shots_left = 0u;
		return;
	}
	shot = OBJECT_ADDRESS(32);
	*p_d7 = 8u;
	find_free_object(p_d7, &shot);
	if ((uint8_t)*p_d7 == 0u)
	{
		OBJECT(object)->routine--;
		*p_d7 = flying_drone_random_wait(object);
		OBJECT(object)->flying_drone.shots_left = 0u;
		return;
	}
	flying_drone_start_shot(object, shot, p_d7, p_a5);
	OBJECT(object)->flying_drone.timer = 7u;
}

/* a hit makes the drone shake, or the last one wrecks it in an explosion */
void flying_drone_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, objects_left = 0;

	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->flags &= ~6u;
		animation = flying_drone_animations[13];
		set_animation(0u, object, animation);
		object_2 = OBJECT_ADDRESS(24);
		objects_left = 8u;
		find_free_object(&objects_left, &object_2);
		if ((uint8_t)objects_left == 0u)
			return;
		OBJECT(object_2)->flags = 1u;
		OBJECT(object_2)->contacts = 0u;
		OBJECT(object_2)->kind = 0x57u;
		OBJECT(object_2)->parameter = 0u;
		OBJECT(object_2)->colour = 0xFFu;
		OBJECT(object_2)->animation_delay = 0u;
		place_at_object(object, object_2);
		OBJECT(object_2)->y -= 0x1600u;
		OBJECT(object_2)->count = 0u;
		OBJECT(object_2)->routine = 0u;
		OBJECT(object_2)->state = 0u;
		sound = 0x11u;
		queue_sound(sound);
		return;
	}
	OBJECT(object)->colour = 8u;
	OBJECT(object)->health = 1u;
	OBJECT(object)->routine++;
	OBJECT(object)->flying_drone.timer = 0x1Eu;
	OBJECT(object)->flags &= ~2u;
	sound = 5u;
	queue_sound(sound);
}

/* the drone gets over the hit */
uint32_t flying_drone_recover(uint32_t object)
{
	uint32_t d7 = 0;
	uint8_t timer = 0;

	d7 = flying_drone_shake(object);
	if (OBJECT(object)->flying_drone.timer == 0x16u)
		OBJECT(object)->colour = 0xFFu;
	timer = OBJECT(object)->flying_drone.timer;
	OBJECT(object)->flying_drone.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->health = 1u;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->flags |= 2u;
	}
	return d7;
}

/* the wreck is gone when its animation ends */
void flying_drone_destroyed(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemies_on_screen--;
	flying_drone_count--;
	remove_object(object);
}

/* the hit drone shakes */
uint32_t flying_drone_shake(uint32_t object)
{
	uint32_t height = 0, shake_offset = 0;

	shake_offset = (uint8_t)frame_count;
	shake_offset = shake_offset & 6u;
	height = flying_drone_shake_heights[shake_offset / 2u];
	height = (uint16_t)(height + 0x2C00u);
	OBJECT(object)->height = height;
	return height;
}

/* the drone weaves toward its turtle */
void flying_drone_steer(uint32_t object, uint32_t *p_picked, uint32_t *p_choices)
{
	uint32_t frame = 0, ignored = 0, target = 0, y_step = 0;
	uint16_t t1 = 0;

	frame = (*p_picked & ~0xFFFFu) | frame_count;
	t1 = frame & 0x7Fu;
	*p_picked = (frame & ~0xFFFFu) | t1;
	if (t1 == 0u)
	{
		boss_pick_target(object, &ignored, p_picked, p_choices);
		if ((uint8_t)*p_picked == 0u)
			OBJECT(object)->flying_drone.target = 0u;
	}
	target = OBJECT(object)->flying_drone.target;
	if (target == 0u)
	{
		return;
	}
	if (OBJECT(object)->screen_x < mem_read16(target + 0xCu))
	{
		if (OBJECT(object)->flying_drone.turning != 0u)
		{
			OBJECT(object)->flying_drone.heading -= OBJECT(object)->flying_drone.turn_speed;
			OBJECT(object)->flying_drone.direction &= 0xFu;
			if (OBJECT(object)->flying_drone.direction == 0xCu)
				OBJECT(object)->flying_drone.turning ^= 1u;
		}
		else
		{
			OBJECT(object)->flying_drone.heading += OBJECT(object)->flying_drone.turn_speed;
			OBJECT(object)->flying_drone.direction &= 0xFu;
			if (OBJECT(object)->flying_drone.direction == 4u)
				OBJECT(object)->flying_drone.turning ^= 1u;
		}
	}
	else if (OBJECT(object)->flying_drone.turning != 0u)
	{
		OBJECT(object)->flying_drone.heading -= OBJECT(object)->flying_drone.turn_speed;
		OBJECT(object)->flying_drone.direction &= 0xFu;
		if (OBJECT(object)->flying_drone.direction == 4u)
			OBJECT(object)->flying_drone.turning ^= 1u;
	}
	else
	{
		OBJECT(object)->flying_drone.heading += OBJECT(object)->flying_drone.turn_speed;
		OBJECT(object)->flying_drone.direction &= 0xFu;
		if (OBJECT(object)->flying_drone.direction == 0xCu)
			OBJECT(object)->flying_drone.turning ^= 1u;
	}
	y_step = 0x100u;
	if (mem_read16(target + 0xEu) <= OBJECT(object)->screen_y)
		y_step = ~y_step;
	OBJECT(object)->y += y_step;
	*p_picked = flying_drone_move(object);
	flying_drone_keep_in_area(object);
}

/* the drone or its shot is kept on the screen and within the area's depth */
void flying_drone_keep_in_area(uint32_t object)
{
	if (OBJECT(object)->screen_x < 0x20u)
		OBJECT(object)->x += 0x2000u;
	if (OBJECT(object)->y >= 0x7C00u)
	{
		OBJECT(object)->y = 0x7C00u;
		return;
	}
	if (OBJECT(object)->y < 0x1000u)
	{
		OBJECT(object)->y = 0x1000u;
		return;
	}
}

/* the resting drone bobs */
uint32_t flying_drone_bob_low(uint32_t object)
{
	uint32_t bob_offset = 0, height = 0;

	bob_offset = OBJECT(object)->flying_drone.bob;
	bob_offset = (uint16_t)(bob_offset << 1);
	height = flying_drone_bob_heights[bob_offset / 2u];
	height = (uint16_t)(height + 0x2000u);
	OBJECT(object)->height = height;
	OBJECT(object)->flying_drone.bob++;
	OBJECT(object)->flying_drone.bob &= 0x1Fu;
	return height;
}

/* the flying drone bobs */
uint32_t flying_drone_bob(uint32_t object)
{
	uint32_t bob_offset = 0, height = 0;

	bob_offset = OBJECT(object)->flying_drone.bob;
	bob_offset = bob_offset & 0x3Eu;
	height = flying_drone_bob_heights[bob_offset / 2u];
	height = (uint16_t)(height + 0x2C00u);
	OBJECT(object)->height = height;
	OBJECT(object)->flying_drone.bob++;
	OBJECT(object)->flying_drone.bob &= 0x3Fu;
	return height;
}

/* the drone moves the way it flies */
uint32_t flying_drone_move(uint32_t object)
{
	uint32_t direction = 0;

	direction = OBJECT(object)->flying_drone.direction;
	direction = (uint16_t)(direction << 2);
	OBJECT(object)->x += flying_drone_x_speeds[direction / 2u];
	OBJECT(object)->y += flying_drone_y_speeds[direction / 2u];
	return direction;
}

/* the drone faces its turtle and points its gun, the rotor turning */
uint32_t flying_drone_animate(uint32_t object)
{
	uint32_t aim = 0, animation = 0, direction = 0;

	if (OBJECT(object)->flying_drone.target == 0u)
	{
		direction = OBJECT(object)->flying_drone.direction;
		aim = flying_drone_aims[direction];
		OBJECT(object)->flags &= 0xBFu;
		if ((int8_t)aim < 0)
		{
			aim = aim & 0x7Fu;
			OBJECT(object)->flags |= 0x40u;
		}
	}
	else
	{
		OBJECT(object)->flags &= 0xBFu;
		if (OBJECT(object)->screen_x < mem_read16(OBJECT(object)->flying_drone.target + 0xCu))
			OBJECT(object)->flags |= 0x40u;
		direction = OBJECT(object)->flying_drone.direction;
		aim = flying_drone_aims[direction];
		aim = aim & 3u;
	}
	OBJECT(object)->flying_drone.aim = aim;
	aim = (uint16_t)(aim << 4);
	aim = (aim & ~0xFFu) | (uint8_t)(aim + (((uint8_t)frame_count & 3u) << 2));
	animation = flying_drone_animations[aim / 4u];
	set_animation(0u, object, animation);
	return aim;
}

/* how long until the drone fires, at random */
uint32_t flying_drone_random_wait(uint32_t object)
{
	uint32_t difficulty_level = 0, range = 0;

	range = 4u;
	range = random_below((uint16_t)range, object);
	OBJECT(object)->flying_drone.timer = flying_drone_waits[range];
	difficulty_level = IN_PAGE(range, difficulty_setting);
	difficulty_level = (difficulty_level & ~0xFFu) | (uint8_t)(difficulty_level << 3);
	OBJECT(object)->flying_drone.timer -= difficulty_level;
	return difficulty_level;
}

/* while drones are about their sound plays now and then */
void sound_flying_drones(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t range = 0;
	uint8_t t1 = 0;

	if (stage != 7u || drone_sound_on == 0u)
		return;
	if (flying_drone_count == 0u)
	{
		drone_sound_on = 0u;
		drone_sound_wait = 0u;
		return;
	}
	t1 = drone_sound_wait;
	drone_sound_wait = t1 - 1u;
	if (t1 != 0u)
		return;
	range = 4u;
	range = random_below((uint16_t)range, object);
	drone_sound_wait = flying_drone_sound_waits[range];
	sound = 0x38u;
	queue_sound(sound);
}

/* a shot from the drone's gun */
void flying_drone_start_shot(uint32_t object, uint32_t shot, uint32_t *p_aim_offset, uint32_t *p_shot_places)
{
	uint32_t aim_side = 0;

	OBJECT(shot)->flags = 1u;
	OBJECT(shot)->contacts = 0u;
	OBJECT(shot)->kind = 0x55u;
	OBJECT(shot)->parameter = 0u;
	OBJECT(shot)->colour = 0xFFu;
	OBJECT(shot)->animation_delay = 0u;
	place_at_object(object, shot);
	*p_shot_places = drone_shot_place_x_ADDRESS;
	aim_side = 0u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(shot)->flags |= 0x40u;
		aim_side = 3u;
	}
	aim_side = (uint8_t)(aim_side + OBJECT(object)->flying_drone.aim);
	aim_side = (uint16_t)(aim_side << 2);
	OBJECT(shot)->x += drone_shot_place_x[aim_side / 2u];
	OBJECT(shot)->height += drone_shot_place_height[aim_side / 2u];
	OBJECT(shot)->drone_shot.aim = OBJECT(object)->flying_drone.aim;
	OBJECT(shot)->routine = 0u;
	OBJECT(shot)->state = 0u;
	*p_aim_offset = aim_side;
}

/* the shot flashes at the gun */
uint32_t drone_shot_start(uint32_t object)
{
	uint32_t aim_side = 0, animation = 0, sound = 0, stats = 0;

	OBJECT(object)->flags |= 0x84u;
	stats = drone_shot_stats_ADDRESS;
	set_object_stats(object, stats);
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->attack |= 0x80u;
	animation = drone_shot_animations[OBJECT(object)->drone_shot.aim * 2u];
	set_animation(0u, object, animation);
	aim_side = 0u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		aim_side = 3u;
	aim_side = (uint8_t)(aim_side + OBJECT(object)->drone_shot.aim);
	aim_side = (uint16_t)(aim_side << 2);
	OBJECT(object)->x_speed = drone_shot_x_speeds[aim_side / 2u];
	OBJECT(object)->y_speed = drone_shot_y_speeds[aim_side / 2u];
	OBJECT(object)->height_speed = -0x400;
	sound = IN_PAGE(aim_side, 0x14u);
	queue_sound((uint8_t)sound);
	OBJECT(object)->routine++;
	return sound;
}

/* after the flash the shot streaks down */
uint32_t drone_shot_flash(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		d7 = OBJECT(object)->drone_shot.aim;
		d7 = (uint16_t)(d7 << 3);
		animation = TABLE32(drone_shot_animations, 0x4u + d7);
		set_animation(0u, object, animation);
		OBJECT(object)->routine++;
		OBJECT(object)->flags &= ~4u;
	}
	return d7;
}

/* the shot drops until it hits the ground and flashes there */
uint32_t drone_shot_fly(uint32_t object)
{
	uint32_t animation = 0, d7 = 0;

	d7 = move_by_speeds(object);
	flying_drone_keep_in_area(object);
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->routine += 2u;
		OBJECT(object)->animation_delay = 2u;
		animation = turret_shot_animations[1];
		set_animation(0u, object, animation);
		return d7;
	}
	if ((int16_t)OBJECT(object)->height < 0)
	{
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->routine++;
		animation = drone_shot_animations[6];
		set_animation(0u, object, animation);
		return d7;
	}
	return d7;
}

/* the shot is gone when its flash ends */
void drone_shot_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		remove_object(object);
		return;
	}
}
