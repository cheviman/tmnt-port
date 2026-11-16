/**
 *    Name:        wide_bosses.c
 *    Description: WIDE's room for the bosses: the game's functions that keep them on the board's screen, with the
 *                 room the turtles have in the wider screen's sides added to their limits
 *    Version:     0.1.0
 *    Created:     13-11-2026
 *    Modified:    13-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    With WIDE the turtles walk into the wider screen's sides (wide_turtles.c), but the bosses kept to the board's
 *    320 columns (a video review: "on the boss pattern here, clearly still kind of adhering to a 4x3 area"; Sebas:
 *    "investigar a ver si le mejoramos la logica para usar toda la pantalla"). These are the game's own, the same C,
 *    their screen limits moved by the turtles' room (console_wide_turtle_room: the sides, as far as the scroll's
 *    segment lets the picture go; none in a room the scroll holds, Krang's, where the turtles keep the arcade's
 *    limits too; in a boss's room, the scroll at its section's end, nearly all of it at the left):
 *    - Rocksteady, Bebop and Shredder: enemy_keep_on_screen, the walls they stop or turn at (their charges, their runs
 *      to an edge, their circles, Shredder's ray lined up), 0x80 and 0x170 on the screen; enemy_pincer_charge_take_edge,
 *      the edge each of the two runs to by the screen's middle, 0x100, now the picture's.
 *    - enemy_on_screen, 0x70 to 0x190, where an enemy attacks: for those three (Bebop attacks only there), so that none
 *      stands by a turtle in a side without attacking it; the other enemies keep the board's.
 *    - Baxter Stockman: stockman_keep_in_area, his flight between the area's x 0x3C0 and 0x470 (the board's left part,
 *      the sewer's scroll held at its end), its left end moved by the room at the left.
 *    - The Technodrome's rock soldier's missile: missile_fly, which blows up past the screen's 0x60 or 0x180.
 *    The rock soldiers walk after a turtle with no limit across, and Krang's room has none. At the left no further than
 *    TURTLE_LEFT_MOST's room (wide_turtles.c), where the game's unsigned screen x would go under 0; the comparisons
 *    signed. Without WIDE (or out of a game's play) the game's own.
 */
#include "console_internal.h"

#include "game.h"

#define BOSS_LEFT_MOST      0x60    /* the room at the left at most (wide_turtles.c's TURTLE_LEFT_MOST) */
#define SHREDDER_KIND       0x0Fu
#define BEBOP_KIND          0x10u
#define ROCKSTEADY_KIND     0x11u

/* WIDE's room now, the turtles', in pixels; false when there is none */
static bool boss_room(int *left, int *right)
{
	console_wide_turtle_room(left, right);
	if (*left > BOSS_LEFT_MOST)
		*left = BOSS_LEFT_MOST;
	return *left > 0 || *right > 0;
}

static int screen_x_of(uint32_t object)
{
	return (int)(int16_t)OBJECT(object)->screen_x;
}

/* The game's enemy_keep_on_screen (objects/states.c): the walls past the room at each side */
void enemy_keep_on_screen(uint32_t object)
{
	int left, right;

	if (!boss_room(&left, &right))
	{
		original_enemy_keep_on_screen(object);
		return;
	}
	if (screen_x_of(object) >= 0x170 + right)
		OBJECT(object)->contacts |= 8u;
	if (screen_x_of(object) < 0x80 - left)
		OBJECT(object)->contacts |= 0x10u;
}

/* The bosses that keep to the walls of enemy_keep_on_screen */
static bool wall_boss(uint32_t object)
{
	const uint8_t kind = OBJECT(object)->kind;

	return kind == SHREDDER_KIND || kind == BEBOP_KIND || kind == ROCKSTEADY_KIND;
}

/* The game's enemy_on_screen (objects/states.c): where an enemy attacks, past the room at each side for the bosses
   (the Foot soldiers, the Mousers and Roadkill Rodney keep the board's: let attack from the sides, they came at a
   turtle all at once, many more than on the board, and the play got harder than the arcade's) */
uint32_t enemy_on_screen(uint32_t object)
{
	int left, right;

	if (!wall_boss(object) || !boss_room(&left, &right))
		return original_enemy_on_screen(object);
	if (screen_x_of(object) >= 0x190 + right || screen_x_of(object) < 0x70 - left)
	{
		offset_high_word = 0xFFFFFFFFu;
		return 0u;
	}
	return 1u;
}

/* The game's enemy_pincer_charge_take_edge (objects/states.c): the picture's middle, not the board's */
void enemy_pincer_charge_take_edge(uint32_t object)
{
	uint8_t t1 = 0, t2 = 0;
	int left, right;

	if (!boss_room(&left, &right))
	{
		original_enemy_pincer_charge_take_edge(object);
		return;
	}
	OBJECT(object)->enemy.side = 0u;
	if (screen_x_of(object) < 0x100 + (right - left) / 2)
	{
		t1 = pincer_charge_edges;
		pincer_charge_edges = t1 | 0x40u;
		if ((t1 & 0x40u) == 0u)
		{
			OBJECT(object)->enemy.side |= 0x40u;
			OBJECT(object)->flags &= 0xBFu;
			OBJECT(object)->x_speed = -0x200;
			OBJECT(object)->y_speed = 0;
			OBJECT(object)->state++;
			return;
		}
	}
	do
	{
		t2 = pincer_charge_edges;
		pincer_charge_edges = t2 | 0x80u;
		if ((t2 & 0x80u) == 0u)
		{
			OBJECT(object)->enemy.side |= 0x80u;
			OBJECT(object)->flags |= 0x40u;
			OBJECT(object)->x_speed = 0x200;
			OBJECT(object)->y_speed = 0;
			OBJECT(object)->state++;
			return;
		}
		t1 = pincer_charge_edges;
		pincer_charge_edges = t1 | 0x40u;
	} while ((t1 & 0x40u) != 0u);
	OBJECT(object)->enemy.side |= 0x40u;
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->x_speed = -0x200;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->state++;
}

/* The game's stockman_keep_in_area (objects/stockman.c): the left end of his flight moved by the room at the left */
void stockman_keep_in_area(uint32_t object)
{
	int left, right;
	uint32_t x_min = 0x3C000u;

	if (!boss_room(&left, &right))
	{
		original_stockman_keep_in_area(object);
		return;
	}
	x_min -= (uint32_t)left << 8;
	if (OBJECT(object)->x >= 0x47000u)
		OBJECT(object)->x = 0x47000u;
	else if (OBJECT(object)->x < x_min)
		OBJECT(object)->x = x_min;
	if (OBJECT(object)->y >= 0x7800u)
	{
		OBJECT(object)->y = 0x7800u;
		return;
	}
	if (OBJECT(object)->y < 0x1800u)
	{
		OBJECT(object)->y = 0x1800u;
		return;
	}
}

/* The game's missile_fly (objects/rock_soldiers.c): it flies on through the room at each side (the floor's walls
   there as on the board: the ring's columns at the left, the console's at the right) */
void missile_fly(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, floor_cell_place = 0, x_speed = 0;
	int left, right;

	if (!boss_room(&left, &right))
	{
		original_missile_fly(object);
		return;
	}
	if (screen_x_of(object) >= 0x60 - left && screen_x_of(object) < 0x180 + right)
	{
		LAYER(WORKING_LAYER)->x = LAYER(LAYER_B)->x;
		floor_cell_place = read_floor_cells_on_screen(object);
		turtle_walls_right(object, floor_cell_place);
		if ((OBJECT(object)->contacts & 8u) == 0u)
		{
			x_speed = OBJECT(object)->x_speed;
			OBJECT(object)->x += x_speed;
			offset_high_word = x_speed;
			return;
		}
	}
	OBJECT(object)->routine++;
	sound = 0x13u;
	queue_sound(sound);
	animation = rocket_animations[0];
	set_animation(0u, object, animation);
}
