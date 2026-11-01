/**
 *    Name:        objects/explosions.c
 *    Description: the explosions other objects start
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a small explosion, harmless */
uint32_t start_small_explosion(uint16_t x_offset, uint32_t height_offset, uint32_t object, uint32_t object_2)
{
	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x31u;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	height_offset = place_explosion(x_offset, (uint16_t)height_offset, object, object_2);
	return height_offset;
}

/* one of the explosions of a defeated boss, with its sound */
void boss_start_explosion(uint32_t object, uint32_t explosion, uint32_t *p_sound, uint32_t *p_sounds)
{
	uint16_t explosion_offset = 0, height_offset = 0, x_offset = 0;
	uint32_t sound_choice = 0;

	explosion_offset = (uint16_t)((uint32_t)OBJECT(object)->boss.explosions << 2);
	x_offset = (uint16_t)boss_explosion_x[explosion_offset / 2u];
	height_offset = (uint16_t)boss_explosion_height[explosion_offset / 2u];
	start_big_explosion(x_offset, height_offset, object, explosion);
	*p_sounds = explosion_sounds_ADDRESS;
	sound_choice = OBJECT(object)->boss.explosions;
	sound_choice = sound_choice & 1u;
	*p_sound = explosion_sounds[sound_choice];
	queue_sound((uint8_t)*p_sound);
}

/* a big explosion, harmless */
void start_big_explosion(uint16_t x_offset, uint32_t height_offset, uint32_t object, uint32_t object_2)
{
	OBJECT(object_2)->flags = 1u;
	OBJECT(object_2)->contacts = 0u;
	OBJECT(object_2)->kind = 0x32u;
	OBJECT(object_2)->parameter = 0u;
	OBJECT(object_2)->colour = 0xFFu;
	OBJECT(object_2)->animation_delay = 0u;
	place_explosion(x_offset, (uint16_t)height_offset, object, object_2);
}

/* Unused: an attacking small explosion where the object is, with its sound */
void explode_small(uint32_t object, uint32_t explosion)
{
	uint8_t sound = 0;
	uint16_t height_offset = 0, x_offset = 0;

	x_offset = 0u;
	height_offset = 0u;
	start_small_blast(x_offset, height_offset, object, explosion);
	sound = 0x11u;
	queue_sound(sound);
}

/* Unused: a small explosion that knocks flying what it hits */
void start_small_blast(uint16_t x_offset, uint32_t height_offset, uint32_t object, uint32_t blast)
{
	OBJECT(blast)->flags = 5u;
	OBJECT(blast)->contacts = 0u;
	OBJECT(blast)->kind = 0x31u;
	OBJECT(blast)->parameter = 0u;
	OBJECT(blast)->colour = 0xFFu;
	OBJECT(blast)->animation_delay = 0u;
	place_explosion(x_offset, (uint16_t)height_offset, object, blast);
}

/* the explosion where the object is, moved across and up */
uint32_t place_explosion(uint16_t x_offset, uint16_t height_offset, uint32_t object, uint32_t object_2)
{
	uint32_t depth_offset = 0;

	OBJECT(object_2)->height = OBJECT(object)->height;
	OBJECT(object_2)->height += height_offset;
	OBJECT(object_2)->x = OBJECT(object)->x;
	OBJECT(object_2)->x += (int16_t)x_offset;
	OBJECT(object_2)->y = OBJECT(object)->y;
	depth_offset = OBJECT(object)->depth;
	depth_offset = (uint16_t)(depth_offset << 8);
	depth_offset = (uint16_t)(depth_offset + 0x200u);
	OBJECT(object_2)->y -= depth_offset;
	OBJECT(object_2)->count = 0u;
	OBJECT(object_2)->routine = 0u;
	OBJECT(object_2)->state = 0u;
	return depth_offset;
}

/* a small explosion */
void explosion_small_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	animation = explosion_small_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	stats = explosion_stats_ADDRESS;
	set_object_stats(object, stats);
}

/* a big explosion */
void explosion_big_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	animation = explosion_big_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	stats = explosion_stats_ADDRESS;
	set_object_stats(object, stats);
}

/* the oil drum's explosion */
void explosion_drum_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	animation = explosion_drum_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	stats = explosion_stats_ADDRESS;
	set_object_stats(object, stats);
}

/* Unused: an explosion like the rocket's */
void explosion_rocket_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	animation = rocket_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	stats = explosion_stats_ADDRESS;
	set_object_stats(object, stats);
}

/* a medium explosion */
void explosion_medium_start(uint32_t object)
{
	uint32_t animation = 0, stats = 0;

	animation = explosion_medium_animations[0];
	set_animation(0u, object, animation);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	stats = explosion_stats_ADDRESS;
	set_object_stats(object, stats);
}

/* the explosion ends and it is gone */
void explosion_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		remove_object(object);
		return;
	}
}
