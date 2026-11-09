/**
 *    Name:        modes.c
 *    Description: Splinter Way's modes of their own: RONIN (one turtle against the enemies of four) and BOSS RUSH
 *                 (the bosses one after another)
 *    Version:     0.1.0
 *    Created:     05-11-2026
 *    Modified:    09-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    RONIN (guideline 7): one player alone, against what four turtles would have. The game's enemies come by its
 *    difficulty: the DIP switch's, one more for each player in play, and the play's time without a knock-out
 *    (update_difficulty, 7 at most), which picks the spawn lists' records that come (spawn_enemies: a record's
 *    flags' low nibble, the difficulty it wants) and how many enemies hold the scroll (enemy_limits, by stage and
 *    difficulty). In RONIN the console's update_difficulty counts the four players; how the enemies share the
 *    turtles (enemies_per_turtle, the slots) stays the one turtle's; and the boxes are its player's alone.
 */
#include "console_internal.h"

#include "game.h"
#include "tmnt/machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCENE_SETUP_NUMBER      2u       /* the game's scene of an area's set-up (then its title, 3) */
#define SCENE_PLAY_NUMBER       4u       /* the game's scene of its play */
#define LAST_FOOT_SOLDIER_KIND  0x0Du    /* kinds 00-0D: the Foot soldiers (wide.c's too) */
#define TECHNODROME_STAGE       8u       /* and the Technodrome's lift's ride (update_layers_technodrome's step 1: */
#define LIFT_RIDE_STEP          1u       /* wide.c's too) */
#define LIFT_DIAGONAL           0xC8100u /* the shaft's x and y through the ride: their sum, the players' walk's (1409
                                            and 1792 at its start): at its end, x at 0xB8000 or past it by less than a
                                            pixel, y over 0x10000, the map's first row */

#define DIFFICULTY_MOST     7u
#define RONIN_PLAYERS       4u    /* the players RONIN's difficulty counts */

/* The game's update_difficulty (scenes/game.c), the four players counted in RONIN */
void update_difficulty(void)
{
	if (!console.ronin || console.page != PAGE_GAME)
	{
		original_update_difficulty();
		return;
	}
	difficulty = 2u;
	if (game_on == 0u)
		return;
	difficulty = (uint8_t)(difficulty_setting + RONIN_PLAYERS + difficulty_from_time);
	if (difficulty >= DIFFICULTY_MOST)
		difficulty = DIFFICULTY_MOST;
}

/* The game's draw_player_boxes (video/texts.c): in RONIN its player's box alone, the others not drawn, nor their
   scores (Sebas: "en ronin, saquemos los cuadros"): the boxes' flags keep his bit of the four (bits 0-3) */
void draw_player_boxes(uint16_t box_flags, uint32_t *p_players, uint32_t *p_box_offset, uint32_t *p_player)
{
	const int slot = console_slot_of[console.chooser];

	if (console.ronin && console.page == PAGE_GAME && slot >= 0)
		box_flags = (uint16_t)((box_flags & ~0xFu) | (box_flags & (1u << slot)));
	original_draw_player_boxes(box_flags, p_players, p_box_offset, p_player);
}

/* In RONIN the cells of the other players' boxes kept clear each frame (the credits' circled digit each box shows,
   which the coins' count draws there by its own), as the fixed layer's blank: the boxes' 9 columns from column 14,
   their rows 2 to 7 */
#define FIXED_LAYER_CELLS   0x100000u
#define FIXED_ROW_BYTES     0x80u
#define BOX_COLUMNS         9
#define FIRST_BOX_COLUMN    14
#define BOX_FIRST_ROW       2
#define BOX_ROWS            6
#define BLANK_CODE          0x10u    /* the font's space */

void ronin_frame(void)
{
	const int slot = console_slot_of[console.chooser];

	if (!console.ronin || console.page != PAGE_GAME || game_state != GAME_STATE_GAME || slot < 0)
		return;
	for (int box = 0; box < CONSOLE_PLAYERS; box++)
		for (int row = 0; row < BOX_ROWS && box != slot; row++)
			for (int column = 0; column < BOX_COLUMNS; column++)
			{
				const uint32_t place = FIXED_LAYER_CELLS + (uint32_t)(BOX_FIRST_ROW + row) * FIXED_ROW_BYTES
					+ (uint32_t)(FIRST_BOX_COLUMN + box * BOX_COLUMNS + column) * 2u;

				io_write8(place, 0u);
				io_write8(place + 1u, BLANK_CODE);
			}
}

/* ---- BOSS RUSH (guideline 6): the bosses one after another ----

   Each area is crossed to its boss by the game's own play, fast and out of sight: the Foot soldiers of the spawn
   lists do not come (their records passed as the game passes those its difficulty does not want), and those the
   doors, the manholes and the rest let out are off the screen for the game, which takes them off its own way (its
   turtle's count and the screen's one less: enemies_on_screen holds the scroll; one riding, a skateboard or a car,
   stays, as taking it off would leave its ride on nothing), the turtles walk
   right on their own, attacking now and then, their health kept full; the picture black under the area and its
   boss's name. His fight's beginning ends it (the game's music changing to a boss's: 0x64, 0x6A Shredder's; not the
   0xFE that stops the music as he comes, while he waits for the last enemies, Stockman's: Sebas, "me aparecieron
   ninjas antes"; nor the drill's 0x66, which area 4 plays as it starts), and the players play from there; when the
   area is over the next one is crossed again. The game starts at area 1 (area 0's scene's boss is the apartment's);
   the highway (5) and the skateboards' road (6), with no boss, are over as their play starts; Krang's room (9) has
   nothing to cross. A section the crossing does not get through (the scroll unmoved for 15 seconds of the play), or
   one crossed for 10000 frames, is the players' to play from there. What held it, as tried: the
   street's turn down (its scroll segment ends at a height: the turtles walk down there, crossing_way), a wall at
   the turtles' right (they walk along it, down and up, till they are past it: the Technodrome's halls), the building
   site's Mousers and drones (off the screen as the soldiers, destroyed as they fly), the Technodrome's turrets
   (destroyed as they come), Rodney (off the screen) and its freeze nozzles (broken as they come up, their stubs off
   the screen). The game starts at area 1 (Rocksteady's), as KONAMI CODE's sets its area */

#define BOSS_MUSIC          0x64u
#define SHREDDER_MUSIC      0x6Au
#define RUSH_FIRST_AREA     1
#define HIGHWAY_AREA        5u
#define SKATEBOARD_AREA     6u      /* the skateboards' road: no boss either */
#define FINAL_AREA          9u      /* Krang's room from its start (his music, 0x62): nothing to cross */
#define RUSH_ATTACK_EVERY   48      /* frames: the turtles' attack while they cross, pressed when none goes on */
#define TURTLE_ATTACKING    3u      /* (a turtle's routine 3, turtle_attack). Pressed every 16 frames, as each of
                                       Raphael's and Donatello's longer attacks ended another began, and they hardly
                                       walked: Raphael held at each area's start, Donatello at the street's hydrant,
                                       and the players got the area to play from there (issue #3) */
#define RUSH_HEALTH_BARS    0x0Au   /* a turtle's full health, as turtle_join gives it */
#define RUSH_HEALTH         3u
#define OFF_SCREEN          0x20u   /* an object's flag: off the screen (update_object_screen_place) */
#define MOUSER_KIND         0x4Eu   /* the Mousers and Roadkill Rodney (enemy_mouser_think, enemy_rodney_think: off */
#define RODNEY_KIND         0x53u   /* the screen, they leave as the soldiers) */
#define TURRET_KIND         0x2Bu   /* the Technodrome's turrets on their rails, solid till destroyed (turret_struck) */
#define TURRET_WRECK        3u      /* (its routine: the wreck's explosions) */
#define DRONE_KIND          0x54u   /* the building site's flying drones, out of the drill (flying_drone_struck) */
#define DRONE_DESTROYED     6u
#define NOZZLE_KIND         0x2Eu   /* the Technodrome's freeze nozzles, up out of the floor: their spray froze the */
#define NOZZLE_RISE         1u      /* turtles in ice there (its routines, freeze_nozzle_routines: up, spraying, and */
#define NOZZLE_SPRAY        2u      /* struck, freeze_nozzle_struck taking it on to broken) */
#define NOZZLE_STRUCK       4u
#define NOZZLE_BROKEN       5u      /* (its stub, solid as the objects 4-11 are, till it leaves the screen) */
#define RUSH_STUCK_FRAMES   900     /* the scroll unmoved this long while crossing: the players play from there */
#define RUSH_MOST_FRAMES    10000u  /* and an area crossed this long (no boss's music came) */

static uint8_t rush_area = 0xFFu;
static uint32_t rush_place, rush_still, rush_frames;    /* (the layers' place at the last frame, the frames it stayed) */
static uint8_t rush_music;    /* (the music at the last frame: a boss's coming is the music changing to his) */
static uint16_t rush_blocked[CONSOLE_PLAYERS];    /* (the frames each turtle has been against a wall at its right) */
static bool rush_lift_set;    /* (the Technodrome's ride set on its diagonal) */
static uint16_t rush_announced;    /* (the frames Krang's set-up has waited on his name: rush_announcing) */
static uint16_t rush_time;    /* (the play's time without a knock-out as the area's crossing started) */

#define WALL_AT_RIGHT       0x08u   /* a turtle's contacts: a wall or a solid thing at its right (turtle_collide) */
#define SLIDE_AFTER         8u      /* the frames against it before the turtle walks along it, down, then up, */
#define SLIDE_FRAMES        96u     /* each this long */

static bool boss_music(uint8_t value)
{
	return value == BOSS_MUSIC || value == SHREDDER_MUSIC;
}

bool console_fast_forward(void)
{
	return boss_rush_crossing();
}

bool boss_rush_crossing(void)
{
	return console.boss_rush && console.rush_crossing && console.page == PAGE_GAME && game_state == GAME_STATE_GAME;
}

void boss_rush_start(void)
{
	console.boss_rush = true;
	console.rush_crossing = true;
	console.level_area = RUSH_FIRST_AREA;
	rush_area = 0xFFu;
	rush_announced = 0u;
}

/* The way the crossing's turtles walk: right, and down or up where the area's scroll segment ends at a height of its
   layers (its end_axis negative: the street turns down at 1304, from 768 to 384, and the crossing held there, right
   only: Sebas, "empieza MUY antes del jefe", "yo solo quiero jefe") */
static uint8_t crossing_way(void)
{
	const uint32_t record = stage_scroll_locks[stage] != 0u ? LAYER_B : LAYER_A;    /* (start_scroll_segment's) */
	uint32_t setting;
	uint8_t way = TMNT_RIGHT;

	if (scene != SCENE_PLAY_NUMBER || scene_step < 1u || last_segment != 0u)
		return way;    /* (the area's segment only in its play: through its set-up scroll_segment is the last area's,
		                  past the end of this one's list, and the game stopped reading a record there) */
	setting = SCROLL_SETTING_LIST(area_scroll_segments[stage] + (int16_t)scroll_segment);
	if (SCROLL_SETTING(setting)->mode != 0u && (int8_t)SCROLL_SETTING(setting)->mode > 0
		&& (int8_t)SCROLL_SETTING(setting)->end_axis < 0)
		way |= SCROLL_SETTING(setting)->end < LAYER(record)->y ? TMNT_DOWN : TMNT_UP;
	return way;
}

/* Each frame before the game's: a new area crossed, a boss's music ending the crossing; while crossing, the turtles'
   controls (its way, the attack now and then) and their health kept full */
void boss_rush_frame(void)
{
	if (!console.boss_rush || console.page != PAGE_GAME || game_state != GAME_STATE_GAME)
		return;
	if (area != rush_area && scene < SCENE_PLAY_NUMBER)
	{
		rush_area = area;    /* (an area being set up: crossed when it plays; the last one's boss music still kept) */
		console.rush_crossing = true;
		rush_music = music;
		rush_still = 0u;
		rush_frames = 0u;
		rush_lift_set = false;
		rush_time = time_without_knockout;
		if (area == FINAL_AREA)
			console.rush_crossing = false;    /* (Krang's fight from the area's start: Sebas, "yo solo quiero jefe") */
	}
	if (console.rush_crossing && boss_music(music) && !boss_music(rush_music))
		console.rush_crossing = false;
	if (console.rush_crossing && (area == HIGHWAY_AREA || area == SKATEBOARD_AREA) && scene == SCENE_PLAY_NUMBER
		&& scene_step >= 1u)
		area_cleared = 1u;    /* (the highway and the skateboards' road have no boss: over at once, as the scenarios
		                         end area 9) */
	rush_music = music;
	if (boss_rush_crossing() && scene == SCENE_PLAY_NUMBER && scene_step >= 1u)    /* (its play, past its entry) */
	{
		const uint32_t place = (LAYER(LAYER_A)->x >> 8) + (LAYER(LAYER_B)->x >> 8) * 3u + (LAYER(LAYER_A)->y >> 8) * 5u
			+ (LAYER(LAYER_B)->y >> 8) * 7u;    /* (the layers' places: one of them moved, or none) */

		rush_still = place == rush_place ? rush_still + 1u : 0u;
		rush_place = place;
		if (rush_still >= RUSH_STUCK_FRAMES || ++rush_frames >= RUSH_MOST_FRAMES)
			console.rush_crossing = false;    /* (a section the crossing does not get through: theirs to play) */
	}
	if (!boss_rush_crossing())
		return;
	time_without_knockout = rush_time;    /* (the crossing is not the players' play: the time it takes adds nothing to
	                                         the difficulty, update_time_without_knockout's, which the bosses' health
	                                         and attacks go by. Counted, it took the difficulty of one player to 4 by
	                                         the Technodrome and on to 7, the most: issue #3) */
	if (stage == TECHNODROME_STAGE && layers_step == LIFT_RIDE_STEP && !rush_lift_set)
	{
		/* the ride's start: the shaft's y on its diagonal with its x, as the players' walk to the lift leaves it.
		   The crossing's turtles, along the halls' walls to the top of the screen, pushed its y two pixels off; at
		   the ride's end y went past the map's first row, and the game stopped reading the row under it */
		LAYER(LAYER_B)->y = LIFT_DIAGONAL - LAYER(LAYER_B)->x;
		rush_lift_set = true;
	}
	for (uint32_t object = OBJECT_ADDRESS(4); object < OBJECT_ADDRESS(40); object += 0x50u)
	{
		if ((OBJECT(object)->flags & 1u) == 0u)
			continue;
		if (OBJECT(object)->kind <= LAST_FOOT_SOLDIER_KIND || OBJECT(object)->kind == MOUSER_KIND
			|| OBJECT(object)->kind == RODNEY_KIND)
			OBJECT(object)->flags |= OFF_SCREEN;    /* (the soldier leaves by the game's own way, where it does; the
			                                           Mousers and Rodney too: the building site's drill lets the
			                                           Mousers out, Rodney comes in the Technodrome's halls, and they
			                                           held the scroll there) */
		else if (OBJECT(object)->kind == TURRET_KIND && OBJECT(object)->routine != 0u
			&& OBJECT(object)->routine != TURRET_WRECK)
		{
			OBJECT(object)->health_bars = 0u;    /* (destroyed as by a turtle's last hit: the Technodrome's held the */
			turret_struck(object);               /* turtles before them, sliding up and down out of their reach) */
		}
		else if (OBJECT(object)->kind == DRONE_KIND && OBJECT(object)->routine != 0u
			&& OBJECT(object)->routine != DRONE_DESTROYED)
		{
			OBJECT(object)->health_bars = 0u;    /* (the drones, as they fly: they held the building site's scroll) */
			flying_drone_struck(object);
		}
		else if (OBJECT(object)->kind == NOZZLE_KIND
			&& (OBJECT(object)->routine == NOZZLE_RISE || OBJECT(object)->routine == NOZZLE_SPRAY))
		{
			OBJECT(object)->routine = NOZZLE_STRUCK;    /* (broken as by a turtle's hit: its row lets out no more) */
			freeze_nozzle_struck(object);
		}
		else if (OBJECT(object)->kind == NOZZLE_KIND && OBJECT(object)->routine == NOZZLE_BROKEN)
			OBJECT(object)->flags |= OFF_SCREEN;    /* (its stub off the screen for the game, which takes it off: it
			                                           comes up where a turtle is, and broken there it stood at the
			                                           turtle's right at the floor's front edge; Leonardo, walking
			                                           more, held there) */
	}
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
	{
		uint8_t controls = crossing_way();

		if ((players_in_play & (1u << slot)) == 0u)
			continue;
		if ((OBJECT(OBJECT_ADDRESS(slot))->contacts & WALL_AT_RIGHT) != 0u)
			rush_blocked[slot]++;
		else if (rush_blocked[slot] >= SLIDE_AFTER)
			rush_blocked[slot] -= rush_blocked[slot] > SLIDE_AFTER + 16u ? 16u : 1u;    /* (along it a while yet) */
		else
			rush_blocked[slot] = 0u;
		if (rush_blocked[slot] >= SLIDE_AFTER && (controls & (TMNT_UP | TMNT_DOWN)) == 0u)
			controls |= (rush_blocked[slot] - SLIDE_AFTER) / SLIDE_FRAMES % 2u == 0u ? TMNT_DOWN : TMNT_UP;
			/* (a wall at its right: the way on is down or up, the Technodrome's halls' turns) */
		if (stage == TECHNODROME_STAGE && layers_step == LIFT_RIDE_STEP)
			controls = 0u;    /* (on the lift, still: walking along its walls took a turtle where the ride's end
			                     puts its wall in the floor map, and the game stopped there) */
		if ((tmnt_board.frame % RUSH_ATTACK_EVERY) < 2u && OBJECT(OBJECT_ADDRESS(slot))->routine != TURTLE_ATTACKING)
			controls |= TMNT_ATTACK;
		tmnt_board.players[slot] = (uint8_t)~controls;
		if (OBJECT(OBJECT_ADDRESS(slot))->flags != 0u)
		{
			OBJECT(OBJECT_ADDRESS(slot))->health_bars = RUSH_HEALTH_BARS;    /* (its health kept full: a normal
			                                                                  turtle's, nothing the game never had) */
			OBJECT(OBJECT_ADDRESS(slot))->health = RUSH_HEALTH;
		}
	}
}

/* The spawn lists' Foot soldiers passed while the area is crossed (spawn_enemies, before the game's) */
void boss_rush_spawns(void)
{
	while (boss_rush_crossing())
	{
		const uint32_t record = spawn_next;

		if (SPAWN_RECORD(record)->x == 0xFFFFu || (int16_t)(spawn_x >> 8) < (int16_t)SPAWN_RECORD(record)->x
			|| (uint8_t)(SPAWN_RECORD(record)->kind_and_parameter >> 8u) > LAST_FOOT_SOLDIER_KIND)
			return;
		spawn_next = record + 8u;
	}
}

/* While crossing, the picture: black, the area and the boss coming (Sebas: "el nombre del enemigo"; the rock soldiers
   by what they are, which is all the decompilation names them; the highway and the skateboards have none, and while
   they are crossed, over as they start, the card is the next area's: its number and its boss, the building site's.
   "AREA 6" with no name read as a boss's card missing his name: issue #3) */
static const char *const boss_names[10] = { "", "ROCKSTEADY", "BEBOP", "BAXTER STOCKMAN", "BEBOP AND ROCKSTEADY", "",
	"", "ROCK SOLDIER", "ROCK SOLDIER", "KRANG AND SHREDDER" };

/* Krang's room has nothing to cross: his name on the black through his area's set-up, which waits on its last step
   RUSH_ANNOUNCE_FRAMES first (the game's area_setup_done, scenes/game.c: the set-up and the title, which BOSS RUSH's
   game skips, took 12 frames: Sebas, "krang no lo anuncia") */
#define RUSH_ANNOUNCE_FRAMES 150u

static bool rush_announcing(void)
{
	return console.boss_rush && console.page == PAGE_GAME && game_state == GAME_STATE_GAME && area == FINAL_AREA
		&& scene >= SCENE_SETUP_NUMBER && scene < SCENE_PLAY_NUMBER;
}

void area_setup_done(void)
{
	if (rush_announcing() && rush_announced < RUSH_ANNOUNCE_FRAMES)
	{
		rush_announced++;
		return;
	}
	rush_announced = 0u;
	original_area_setup_done();
}

/* A name wider than the board's screen in the big letters (BEBOP AND ROCKSTEADY) in two lines of them, broken at
   its space nearest its middle, as big as the others' (Sebas: "el tamaño del texto del jefe 5 ... es distinto") */
#define NAME_Y              108
#define NAME_SCALE          2
#define NAME_MOST_WIDTH     300
#define NAME_LINE_STEP      20

static void draw_boss_name(console_picture *frame, const char *name)
{
	const int length = (int)strlen(name);
	int split = -1;
	char line[24];

	if (draw_game_letters_width(name, NAME_SCALE) <= NAME_MOST_WIDTH || length >= (int)sizeof(line))
	{
		draw_game_letters_centred(frame, NAME_Y, NAME_SCALE, GAME_LETTERS_OWN, name);
		return;
	}
	for (int place = 0; place < length; place++)
		if (name[place] == ' ' && (split < 0 || abs(2 * place - length) < abs(2 * split - length)))
			split = place;
	snprintf(line, sizeof(line), "%.*s", split, name);
	draw_game_letters_centred(frame, NAME_Y, NAME_SCALE, GAME_LETTERS_OWN, line);
	draw_game_letters_centred(frame, NAME_Y + NAME_LINE_STEP, NAME_SCALE, GAME_LETTERS_OWN, name + split + 1);
}

void boss_rush_overlay(console_picture *frame)
{
	char text[24];
	unsigned shown = area;

	if (!boss_rush_crossing() && !rush_announcing())
		return;
	while (shown < FINAL_AREA && !boss_names[shown][0])
		shown++;    /* (an area with no boss: the next one's card) */
	draw_fill(frame, PICTURE_FIRST(frame), 0, PICTURE_END(frame) - PICTURE_FIRST(frame), TMNT_VIDEO_HEIGHT, 0x000000u);
	snprintf(text, sizeof(text), "AREA %u", shown + 1u);
	draw_game_letters_centred(frame, 92, 1, GAME_LETTERS_OWN, text);
	if (shown < 10u && boss_names[shown][0])
		draw_boss_name(frame, boss_names[shown]);
}

/* For a LAN game's start (console_power_on): the modes' state as at power-on */
void modes_power_on(void)
{
	rush_area = 0xFFu;
	rush_place = rush_still = rush_frames = 0u;
	rush_music = 0u;
	memset(rush_blocked, 0, sizeof(rush_blocked));
	rush_lift_set = false;
	rush_announced = 0u;
	rush_time = 0u;
}
