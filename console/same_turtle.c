/**
 *    Name:        same_turtle.c
 *    Description: "everyone the same turtle" (guideline 8, the Konami code's): every player plays the one chosen, its
 *                 moves, weapon and colours, on his own slot
 *    Version:     0.1.0
 *    Created:     05-11-2026
 *    Modified:    05-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A turtle object's kind is both the turtle (its animations, turtle_animation_sets, whose pieces carry its palette;
 *    its combos, attacks, hit animations and waits, walk speeds, Raphael's own states) and its player (the slot: his
 *    lives and health, his box, his prompts, the bit of his score and knock-out, where he starts and comes in). The
 *    kind stays the player's; these are the game's functions (src/objects/turtles.c, the same C) that take the
 *    turtle's tables by it, taking them by console_same_turtle when it is one (TURTLE_KIND), with turtle_run_all
 *    (console/wide_turtles.c). Otherwise the game's own run.
 */
#include "console_internal.h"

#include "game.h"

/* the turtle against solid objects, the floor map and its walls, and falling when nothing holds it */
void turtle_collide(uint32_t object, uint32_t inputs)
{
	uint32_t carry = 0;
	uint8_t forced_input = 0, raised_y = 0, sound = 0;
	uint32_t floor_cell_place = 0;
	uint8_t height_whole = 0;
	uint16_t height = 0;
	uint32_t y = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_collide(object, inputs);
		return;
	}
	if (OBJECT(object)->routine == 4u)
	{
		forced_input = 2u;
		if ((OBJECT(object)->turtle.knockback_flags & 1u) != 0u)
			forced_input = 1u;
		PLAYER_INPUT(inputs)->bits = forced_input;
	}
	else if (OBJECT(object)->routine == 3u && (OBJECT(object)->state >= 9u || (TURTLE_KIND(object) == 3u && OBJECT(object)->state == 7u)))
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


/* the turtle's walking speeds, straight or on a diagonal */
void turtle_walk_speed(uint32_t object, uint32_t inputs, uint32_t *p_speed_offset, uint32_t *p_walk_speeds)
{
	uint8_t direction_copy = 0, direction_inputs = 0;
	uint16_t diagonal_offset = 0, speed_offset = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_walk_speed(object, inputs, p_speed_offset, p_walk_speeds);
		return;
	}
	diagonal_offset = 0u;
	direction_inputs = PLAYER_INPUT(inputs)->bits;
	direction_copy = (uint8_t)direction_inputs;
	if ((direction_inputs & 3u) != 0u && (direction_copy & 0xCu) != 0u)
		diagonal_offset = 2u;
	*p_walk_speeds = turtle_walk_speeds_ADDRESS;
	speed_offset = (uint16_t)(((TURTLE_KIND(object) & 3u) << 3) + diagonal_offset);
	*p_speed_offset = (*p_speed_offset & 0xFFFF0000u) | speed_offset;
}


/* the jump button starts a jump, or the special attack with the attack button too */
void turtle_start_jump(uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t move = 0;
	uint16_t routine_state = 0;
	uint32_t held = 0, ignored = 0, ignored_2 = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_start_jump(object, inputs, presses);
		return;
	}
	if ((PLAYER_INPUT(presses)->bits & 0x20u) != 0u)
	{
		OBJECT(object)->flags &= ~8u;
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->turtle.combo_step = 0u;
		OBJECT(object)->turtle.combo = 6u;
		move = 7u;
		if (TURTLE_KIND(object) == 3u)
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
void turtle_start_combo(uint32_t object, uint32_t inputs)
{
	uint8_t attack = 0, combo_step = 0, move = 0;
	uint16_t routine_state = 0;
	uint32_t combo_table = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, range = 0;
	uint8_t t1 = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_start_combo(object, inputs);
		return;
	}
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
	combo_table = turtle_combos[TURTLE_KIND(object) & 3u];
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


/* the attack's kind, sound and damage, and the turtle attacking */
void turtle_set_attack(uint16_t move, uint16_t attack, uint32_t object)
{
	uint8_t sound = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_set_attack(move, attack, object);
		return;
	}
	OBJECT(object)->flags &= ~8u;
	OBJECT(object)->flags |= 4u;
	attack = (uint8_t)(attack + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
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


/* the turtle's animation for its routine and state */
uint32_t turtle_set_animation(uint32_t held, uint32_t object)
{
	uint16_t table_offset = 0;
	uint32_t animation_entry = 0, animation_set = 0;

	if (console_same_turtle < 0)
		return original_turtle_set_animation(held, object);
	table_offset = (uint16_t)((uint32_t)TURTLE_KIND(object) << 2);
	animation_set = turtle_animation_sets[table_offset / 4u];
	table_offset = (uint16_t)(IN_PAGE(table_offset, OBJECT(object)->routine) << 2);
	animation_entry = ANIMATION_ENTRY(ANIMATION_TABLE_ENTRY(animation_set + (table_offset / 4u) * 0x4u) + (IN_PAGE(table_offset, OBJECT(object)->state) & 0x3FFFu) * 0x4u);
	return set_animation(held, object, animation_entry);
}


/* in the air: a jump attack by its height, or the jump's state */
void turtle_jump(uint8_t held, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t attack_height = 0, height_above_floor = 0, move = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_jump(held, object, inputs, presses);
		return;
	}
	turtles_busy--;
	OBJECT(object)->flags &= ~0xCu;
	if ((PLAYER_INPUT(presses)->bits & 0x20u) != 0u)
	{
		attack_height = 0x38u;
		if (TURTLE_KIND(object) == 3u)
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


/* attacking: the jump attacks' arcs, steps forward, the end of the attack */
void turtle_attack(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t input_bit = 0, side_wall_bit = 0, wall_bit = 0;
	uint16_t jump_speed = 0;
	uint32_t x_step = 0;
	uint8_t move_timer = 0, t1 = 0;
	uint16_t height = 0, jump_speed_2 = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_attack(held, pressed, object, inputs, presses);
		return;
	}
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
	if (TURTLE_KIND(object) != 3u || OBJECT(object)->state != 7u)
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
void turtle_attack_over(uint8_t held, uint8_t pressed, uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint8_t facing = 0, sound = 0, stick = 0;
	uint16_t attack_2 = 0, job = 0, kind_offset = 0, move = 0, routine_state = 0, throw_move = 0;
	uint32_t animation_entry = 0, attack = 0, combo_table = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, target = 0, throw_counter = 0, thrown_enemy = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_attack_over(held, pressed, object, inputs, presses);
		return;
	}
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
		job = 0x400u | shl32(1u, (int)(TURTLE_KIND(object) & 3u));    /* (the player's point, by his slot) */
		if (TURTLE_KIND(object) == 1u || TURTLE_KIND(object) == 2u)
			OBJECT(object)->flags ^= 0x40u;    /* (Michelangelo's and Donatello's turn: the turtle's) */
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
			combo_table = turtle_combos[TURTLE_KIND(object) & 3u];
			if (OBJECT(object)->turtle.enemy_behind != 0u)
			{
				OBJECT(object)->turtle.combo += 0xCu;
				PLAYER_INPUT(inputs)->bits = 0u;
				move = COMBO_STEP(combo_table + ((int16_t)OBJECT(object)->turtle.combo + 1u));
				attack_2 = (uint8_t)(move + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
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
				kind_offset = (uint16_t)((uint32_t)TURTLE_KIND(object) << 1);
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
					if (TURTLE_KIND(object) == 0u || TURTLE_KIND(object) == 3u)
						OBJECT(object)->flags ^= 0x40u;
					attack = (uint8_t)(attack + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
					if ((OBJECT(object)->flags & 0x40u) != 0u)
						attack = attack | 0x80u;
					OBJECT(object)->attack = attack;
					attack = attack & 0x7Fu;
					sound = turtle_attack_sounds_by_move[attack];
					queue_sound(sound);
					OBJECT(object)->damage = turtle_attack_damages[attack & 0x7FFFu];
					thrown_enemy = OBJECT(object)->turtle.hit_target;
					animation_entry = ANIMATION_ENTRY(turtle_hit_animation_tables[TURTLE_KIND(object)] + ((int32_t)(OBJECT(thrown_enemy)->throw & 7u - 4u)) * 0x4u);
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
				attack_2 = (uint8_t)(move + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
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
				attack_2 = (uint8_t)(move + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
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
			attack_2 = (uint8_t)(move + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
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
		move = COMBO_STEP(turtle_combos[TURTLE_KIND(object) & 3u] + ((int16_t)OBJECT(object)->turtle.combo + 2u));
		attack_2 = (uint8_t)(move + turtle_attack_firsts[TURTLE_KIND(object) & 3u]);
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
void turtle_hit(uint32_t object, uint32_t inputs, uint32_t presses)
{
	uint16_t floor_height = 0, hit_reaction = 0, knockback_flags = 0, reaction_offset = 0, reaction_variant = 0, routine_state = 0;
	uint32_t hit_by = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, target = 0, x = 0, x_2 = 0;
	uint8_t health_bars = 0;

	if (console_same_turtle < 0)
	{
		original_turtle_hit(object, inputs, presses);
		return;
	}
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
					OBJECT(target)->enemy.move = turtle_hit_waits[TURTLE_KIND(object)];
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

