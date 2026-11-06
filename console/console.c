/**
 *    Name:        console.c
 *    Description: the console version's own versions of the game functions console/replaced.txt lists, the
 *                 players' controls on the turtles' slots, START's coins and the DIP switches
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The console version (TMNT) runs the arcade game's program with these in place of the game's own (whose
 *    definitions tools/decompile.py writes as CONSOLE_ORIGINAL(name): original_name here, for these to call), so
 *    the arcade build (TMNT Arcade) stays the program it is. In the attract mode the console's front end runs
 *    (run_attract): the Konami presentation at power-on (presentation.c), then the menus over the attract mode's
 *    own screens (menu.c), which it holds there: the title's logo for the menu, BEST 10 PLAYERS' picture for
 *    OPTIONS, the coin's screen for the turtle choice; the game starts as the cabinet's does, a coin and the attack
 *    button on the chosen turtle's slot.
 *
 *    No coins at home: a player's START puts the coin and the attack in (console_controls), on the slot of the
 *    turtle he plays, and every box that would show INSERT COIN(S) shows the game's PRESS START. The lives a coin
 *    gives are the PLAYERS switch's (ARCADE), or Splinter Way's 5, its continues 3 (the coins after the first).
 */
#include "console_internal.h"

#include "game.h"
#include "tmnt/machine.h"

#include <stdio.h>
#include <string.h>

#define JOIN_COIN_FRAMES    2      /* the coin held this long */
#define JOIN_ATTACK_FROM    5      /* then, its credit counted (2 frames after it is let go), the attack */
#define JOIN_ATTACK_FRAMES  2
#define JOIN_FRAMES         10
#define TITLE_COPYRIGHT_NINJA   18u    /* game_texts: the title's copyright lines, NINJA and HERO */
#define TITLE_COPYRIGHT_HERO    19u
#define TEXT_END            0x3Fu
#define TEXT_NEXT           0x2Fu
#define BLANK_LINE          1u     /* positioned_texts' line under a box, cleared */
#define FIXED_ROW           0x80u  /* the fixed layer's row, in the program's addresses */
#define COPYRIGHT_ROWS_UP   1u     /* "(C) KONAMI 1989" on the menu's screen: a row over the title's (y 200) */
#define TITLE_MUSIC_STOP    0x7Eu  /* the sound program's commands: the title music stopped, silence */
#define SOUND_SILENCE       0xFFu

console_state console = { .last_presser = 0 };
int console_slot_of[CONSOLE_PLAYERS] = { -1, -1, -1, -1 };
int console_same_turtle = -1;
int console_coins_put[CONSOLE_PLAYERS];

console_settings_t console_settings = { .window_scale = 3, .pixels = PIXELS_SHARP, .volume = 5,
	.dsw = { TMNT_DSW1_DEFAULT, TMNT_DSW2_DEFAULT, TMNT_DSW3_DEFAULT } };

const char *const console_pixel_names[PIXELS_KINDS] = { "sharp", "smooth", "bilinear", "crt" };
const char *const console_wide_names[WIDE_KINDS] = { "off", "16:9", "20:9" };

/* Each slot's join: its frames since START (-1: none), and whether a coin goes in first */
static struct
{
	int frames;
	bool coin;
} joins[CONSOLE_PLAYERS] = { { -1, false }, { -1, false }, { -1, false }, { -1, false } };

static uint8_t controls_last[CONSOLE_PLAYERS];    /* each player's controls at the last frame */
static void level_select_frame(void);
static bool arcade_test;                          /* console_arcade_test */

void console_arcade_test(void)
{
	arcade_test = true;
	console.powered_on = true;
}

void console_power_on(bool lan_game)
{
	const console_state fresh = { .last_presser = 0 };

	console = fresh;
	console.powered_on = lan_game;    /* (a LAN game: no presentation, the turtle choice from the boot on) */
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
	{
		console_slot_of[slot] = -1;
		console_coins_put[slot] = 0;
		joins[slot].frames = -1;
		joins[slot].coin = false;
		controls_last[slot] = 0;
	}
	console_same_turtle = -1;
	arcade_test = false;
	free_play = 0u;
	tmnt_board.dsw[0] = console_settings.dsw[0];
	tmnt_board.dsw[1] = console_settings.dsw[1];
	tmnt_board.dsw[2] = (uint8_t)(console_settings.dsw[2] | TMNT_DSW3_SERVICE);    /* (the test mode's switch off) */
	menu_power_on(lan_game);
	wide_power_on();
	wide_turtles_power_on();
	modes_power_on();
	draw_power_on();
}

/* ---- the DIP switches and the lives ---- */

/* As the boot reads them (reset): the switches' copies, the coins and credits of each slot, the lives a credit
   gives, the difficulty, the demo's sounds; then the screen's flip as the attract mode sets it */
void console_apply_dip_switches(void)
{
	uint16_t coinage, coinage_2;

	tmnt_board.dsw[0] = console_settings.dsw[0];
	tmnt_board.dsw[1] = console_settings.dsw[1];
	tmnt_board.dsw[2] = console_settings.dsw[2];
	dip_switches_1 = (uint8_t)~tmnt_board.dsw[0];
	dip_switches_2 = (uint8_t)~tmnt_board.dsw[1];
	dip_switches_3 = (uint8_t)~tmnt_board.dsw[2];
	coinage = (uint16_t)((dip_switches_1 & 0xFu) << 1);
	COIN_SLOT(COIN_SLOT_ADDRESS(0))->coins_per_credit = coinage_coins[coinage];
	COIN_SLOT(COIN_SLOT_ADDRESS(0))->credits_per_coins = coinage_credits[coinage];
	coinage_2 = (uint16_t)((dip_switches_1 & 0xF0u) >> 3);
	COIN_SLOT(COIN_SLOT_ADDRESS(1))->coins_per_credit = coinage_2_coins[coinage_2];
	COIN_SLOT(COIN_SLOT_ADDRESS(1))->credits_per_coins = coinage_2_credits[coinage_2];
	lives_per_credit = lives_settings[dip_switches_2 & 3u];
	difficulty_setting = (dip_switches_2 & 0x60u) >> 5;
	demo_sounds = dip_switches_2 & 0x80u;
	set_screen_flip();
}

/* The lives a credit gives in the game being started: the PLAYERS switch's, or Splinter Way's */
void console_set_mode_lives(int mode)
{
	lives_per_credit = mode == MODE_SPLINTER_WAY ? SPLINTER_WAY_LIVES : lives_settings[dip_switches_2 & 3u];
}

/* ---- the players and the slots ---- */

/* Whether a START on 'slot' would bring its turtle in: always, but in Splinter Way once its continues are
   used (the first coin and 3 more) */
static bool start_open(int slot)
{
	if (console.ronin && console.page == PAGE_GAME && console_slot_of[console.chooser] != slot)
		return false;    /* (RONIN: its one player alone, the other boxes' lines cleared) */
	if (PLAYER(PLAYER_ADDRESS(slot))->lives != 0u)
		return true;
	return console.mode != MODE_SPLINTER_WAY || console_coins_put[slot] <= SPLINTER_WAY_CONTINUES;
}

void console_join(int slot)
{
	const bool coin = PLAYER(PLAYER_ADDRESS(slot))->lives == 0u;

	if (slot < 0 || slot >= CONSOLE_PLAYERS || joins[slot].frames >= 0 || !start_open(slot))
		return;
	if (coin)
		console_coins_put[slot]++;
	joins[slot].frames = 0;
	joins[slot].coin = coin;
}

/* A START in a game: the player on his slot (the first free one when he has none), unless his turtle plays */
static void start_pressed(int player)
{
	int slot = console_slot_of[player];

	if (game_state != GAME_STATE_GAME || (console.ronin && slot < 0))
		return;    /* (RONIN: its one player alone) */
	if (slot < 0)
	{
		for (int free_slot = 0; free_slot < CONSOLE_PLAYERS && slot < 0; free_slot++)
		{
			bool taken = (players_in_play & (1u << free_slot)) != 0u || joins[free_slot].frames >= 0;

			for (int other = 0; other < CONSOLE_PLAYERS; other++)
				taken = taken || console_slot_of[other] == free_slot;
			if (!taken)
				slot = free_slot;
		}
		if (slot < 0)
			return;
		console_slot_of[player] = slot;
	}
	if ((players_in_play & (1u << slot)) != 0u || (players_joining & (1u << slot)) != 0u)
		return;
	console_join(slot);
}

void console_controls(const uint8_t pressed[CONSOLE_PLAYERS], uint8_t starts)
{
	uint8_t slots[CONSOLE_PLAYERS] = { 0, 0, 0, 0 };
	uint8_t coins = 0;

	console_keep_sprites();
	level_select_frame();
	ronin_frame();
	if (arcade_test)
	{
		/* (the board's inputs as the script left them: its coins, and each player's buttons on his own slot) */
		console.page = game_state == GAME_STATE_GAME ? PAGE_GAME : PAGE_MENU;
		for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
			tmnt_board.players[slot] = (uint8_t)~pressed[slot];
		return;
	}
	for (int player = 0; player < CONSOLE_PLAYERS; player++)
	{
		const uint8_t buttons = TMNT_ATTACK | TMNT_JUMP;
		const uint8_t fresh = (uint8_t)(pressed[player] & ~controls_last[player]);

		if ((starts & (1u << player)) || (fresh & buttons))
			console.last_presser = player;
		controls_last[player] = pressed[player];
		/* out of a game, a player's controls newly pressed are the menus' actions too (whatever the port reads into
		   them: its keys and pads as each player's), besides the ones the port sends (console_actions) */
		if (console.page != PAGE_GAME)
		{
			const uint32_t actions = ((fresh & TMNT_UP) ? ACTION_BIT(ACTION_UP) : 0u)
				| ((fresh & TMNT_DOWN) ? ACTION_BIT(ACTION_DOWN) : 0u)
				| ((fresh & TMNT_LEFT) ? ACTION_BIT(ACTION_LEFT) : 0u)
				| ((fresh & TMNT_RIGHT) ? ACTION_BIT(ACTION_RIGHT) : 0u)
				| ((fresh & TMNT_ATTACK) ? ACTION_BIT(ACTION_ACCEPT) : 0u)
				| ((fresh & TMNT_JUMP) ? ACTION_BIT(ACTION_BACK) : 0u);

			console.pressed |= actions;
			console.player_pressed[player] |= actions | ((starts & (1u << player)) ? ACTION_BIT(ACTION_START) : 0u);
		}
	}
	if (console.page != PAGE_GAME && starts)
		console.pressed |= ACTION_BIT(ACTION_START);    /* (any player's START, in the menus) */
	if (console.page == PAGE_GAME)
	{
		for (int player = 0; player < CONSOLE_PLAYERS; player++)
		{
			if (starts & (1u << player))
				start_pressed(player);
			if (console_slot_of[player] >= 0)
				slots[console_slot_of[player]] |= pressed[player];
		}
	}
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
	{
		const int frame = joins[slot].frames;

		if (frame < 0)
			continue;
		if (joins[slot].coin && frame < JOIN_COIN_FRAMES)
			coins |= (uint8_t)(TMNT_COIN1 << slot);
		if (frame < JOIN_ATTACK_FROM)
			slots[slot] &= (uint8_t)~(TMNT_ATTACK | TMNT_JUMP);    /* (let go, for the attack's press to be new) */
		else if (frame < JOIN_ATTACK_FROM + JOIN_ATTACK_FRAMES)
			slots[slot] |= TMNT_ATTACK;
		if (++joins[slot].frames >= JOIN_FRAMES)
			joins[slot].frames = -1;
	}
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
		tmnt_board.players[slot] = (uint8_t)~slots[slot];
	tmnt_board.coins = (uint8_t)~coins;
	boss_rush_frame();    /* (BOSS RUSH's crossing: its turtles' controls over the players') */
}

void console_actions(uint32_t pressed)
{
	console.pressed |= pressed;
	console.player_pressed[0] |= pressed;    /* (the port's fixed keys: player 1's, as his keys are by default) */
}

/* A game over and the attract mode again: the players off their slots, the coins and the lives back to the
   switches' */
static void slots_game_over(void);

/* A LAN game over for this device (lan.c: ended, left, its host gone): the LAN page, after the game over the pause's
   QUIT GAME has (at the attract mode's next frame, when it was in the game itself); the board's controls and the
   presses the network put let go */
static bool lan_page_next;

void console_back_to_lan_page(void)
{
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
	{
		tmnt_board.players[slot] = 0xFFu;
		controls_last[slot] = 0;
		console.player_pressed[slot] = 0;
	}
	tmnt_board.coins = 0xFFu;
	console.pressed = 0;
	if (console.page == PAGE_GAME)
	{
		lan_page_next = true;    /* (run_attract's game over first) */
		return;
	}
	slots_game_over();
	console_open_lan_page();
}

static void slots_game_over(void)
{
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
	{
		console_slot_of[slot] = -1;
		console_coins_put[slot] = 0;
		joins[slot].frames = -1;
	}
	free_play = 0u;
	console_set_mode_lives(MODE_ARCADE);
	if (!console.restarting)
	{
		console.level_select = false;    /* (RESTART: LEVEL SELECT's game again, in its area, its turtle) */
		console_same_turtle = -1;
		console.ronin = false;
		console.boss_rush = false;
	}
	console.level_pending = false;
}

/* LEVEL SELECT's game: its area set when the game has put its own (new_game_first_area: area 0) and not yet read it
   (area_setup_variables, the area's set-up's first step: its stage and the rest by it), as the scenarios poke it;
   its lives kept up, each box drawn again when they are (players_credited, redraw_new_lives) */
/* "Original turtles" (guideline 18: as the comics first drew them): each turtle keeps its moves and weapon, and its
   bandana is Raphael's red, wherever the game shows it (Sebas: the turtle choice's pictures and the boxes' faces too).
   The colours the palette RAM has (faded with the rest) of Raphael's go into the other three's each frame:
   - in a game, the turtles' sprites: the sprite banks 0-3 (colours 16-19 x 16, by their animations' pieces),
     Leonardo's to Raphael's, whose pens 10 and 11 are the bandana's;
   - on the turtle choice (the coin's screen), the four pictures: their own sprite palettes in the same banks, the
     bandana pens 1 to 3 (14 and 15 the picture's background);
   - in both, the boxes' faces: the fixed layer's cells of each box's face (rows 4 and 5, its columns 5 to 8; the
     face's tiles 92-9F and A2-AF) in Raphael's box's colours (the fixed layer's bank 4, the players' 1-4 alike but
     the bandana's and the label's pens, which the label shares: the cells' bank, not the pens) */
#define TURTLE_BANKS        16      /* the sprites' first palette, Leonardo's */
#define RAPHAEL_BANK        19
#define FIXED_LAYER_CELLS   0x100000u    /* the fixed layer as the program writes it: an attribute and a code a cell */
#define FIXED_ROW_BYTES     0x80u
#define BOX_COLUMNS         9       /* the boxes' cells: 9 columns from column 14, each */
#define FIRST_BOX_COLUMN    14
#define FACE_COLUMN         5       /* the face's 4 columns in its box, from the 5th */
#define FACE_ROW            4       /* its 2 rows */
#define RAPHAEL_BOX_BANK    4       /* the attribute's colour bits (5-7): the players' boxes 1-4 */

static void copy_pens(int from_bank, int to_bank, int pen, int count)
{
	for (int index = pen; index < pen + count; index++)
	{
		tmnt_board.palette[(to_bank * 16 + index) * 2] = tmnt_board.palette[(from_bank * 16 + index) * 2];
		tmnt_board.palette[(to_bank * 16 + index) * 2 + 1] = tmnt_board.palette[(from_bank * 16 + index) * 2 + 1];
	}
}

static void red_bandanas(bool choice)
{
	for (int turtle = 0; turtle < 3; turtle++)
	{
		if (choice)
			copy_pens(RAPHAEL_BANK, TURTLE_BANKS + turtle, 1, 3);
		else
			copy_pens(RAPHAEL_BANK, TURTLE_BANKS + turtle, 10, 2);
		for (int row = 0; row < 2; row++)
			for (int column = 0; column < 4; column++)
			{
				const uint32_t place = FIXED_LAYER_CELLS + (uint32_t)(FACE_ROW + row) * FIXED_ROW_BYTES
					+ (uint32_t)(FIRST_BOX_COLUMN + turtle * BOX_COLUMNS + FACE_COLUMN + column) * 2u;
				const uint8_t attribute = io_read8(place), code = io_read8(place + 1u);

				if ((code & 0xF0u) == (row ? 0xA0u : 0x90u) && (code & 0x0Fu) >= 2u && (attribute >> 5) == turtle + 1)
					io_write8(place, (uint8_t)((attribute & 0x1Fu) | (RAPHAEL_BOX_BANK << 5)));
			}
	}
}

#define SCENE_CHOOSE_TURTLES    1u
#define SCENE_AREA_SETUP        2u

static void level_select_frame(void)
{
	if (console.level_select && console.original_turtles
		&& (console.page == PAGE_CHOICE || console.page == PAGE_STARTING || game_state == GAME_STATE_GAME))
		red_bandanas(game_state != GAME_STATE_GAME);
	if ((!console.level_select && !console.boss_rush) || game_state != GAME_STATE_GAME)
		return;
	if (console.level_pending && (scene == SCENE_CHOOSE_TURTLES || (scene == SCENE_AREA_SETUP && scene_step == 0u)))
	{
		area = (uint8_t)console.level_area;
		console.level_pending = false;
	}
	for (int slot = 0; slot < CONSOLE_PLAYERS && console.infinite_lives && console.level_select; slot++)
		if ((players_in_play & (1u << slot)) != 0u && PLAYER(PLAYER_ADDRESS(slot))->lives < SPLINTER_WAY_LIVES)
		{
			PLAYER(PLAYER_ADDRESS(slot))->lives = SPLINTER_WAY_LIVES;
			players_credited |= (uint8_t)(1u << slot);
		}
}

/* ---- the game's functions in the console's versions ---- */

/* The attract mode's frame: the presentation at power-on, then the menus over the attract's screens they hold;
   the attract's own step and its start of a game after them (with no step while the presentation runs) */
void run_attract(void)
{
	uint32_t pressed = console.pressed;
	uint32_t player_pressed[CONSOLE_PLAYERS];

	memcpy(player_pressed, console.player_pressed, sizeof(player_pressed));
	memset(console.player_pressed, 0, sizeof(console.player_pressed));
	if (arcade_test)
	{
		original_run_attract();
		return;
	}
	console.pressed = 0;
	if (console.page == PAGE_GAME)
	{
		/* (back from a game: the menu, the actions pressed during the game let go; after the pause's RESTART, the
		   turtle choice, which picks the same turtle at once) */
		slots_game_over();
		console.page = console.restarting ? PAGE_CHOICE : PAGE_MENU;
		console.choice = 0;
		console.page_frames = 0;
		if (console_lan_playing())
			console_lan_game_over();    /* (a LAN game's: the LAN page again) */
		else if (lan_page_next)
		{
			lan_page_next = false;    /* (a LAN game left in the game itself) */
			console_open_lan_page();
		}
		pressed = 0;
		memset(player_pressed, 0, sizeof(player_pressed));
	}
	if (!console.powered_on)
	{
		if (presentation_frame(pressed))
		{
			game_step = ATTRACT_CONSOLE_STEP;
			original_run_attract();
			return;
		}
		console.powered_on = true;
		console.page = PAGE_MENU;
		console.choice = 0;
		pressed = 0;
		memset(player_pressed, 0, sizeof(player_pressed));
	}
	menu_frame(pressed, player_pressed);
	original_run_attract();
	if (game_state != GAME_STATE_ATTRACT)
	{
		/* (the game started: the players' controls on their slots from now on) */
		free_play = 0u;
		console.page = PAGE_GAME;
	}
}

/* The last line of a text of game_texts into the fixed layer, in 'colour', as print_table_text writes its
   lines, 'rows_up' rows higher than its place */
static void print_last_line(uint32_t text_number, uint8_t colour, uint32_t rows_up)
{
	uint32_t text = TEXT_POINTER(game_texts_ADDRESS + text_number * 4u), line = text;

	for (;;)
	{
		uint8_t character;

		text += 4u;
		while ((character = TEXT(text)) != TEXT_END && character != TEXT_NEXT)
			text++;
		if (character == TEXT_END)
			break;
		line = ++text;
	}
	text = line;
	{
		uint32_t tile_place = TEXT_32(text) - rows_up * FIXED_ROW;

		for (text += 4u; TEXT(text) != TEXT_END; text++)
		{
			io_write8(tile_place, colour);
			io_write8(tile_place + 1u, TEXT(text) - 0x30u);
			tile_place += 2u;
		}
	}
}

/* TURTLES slides in as the title's does; then of the copyright lines the title prints with it (in the colour
   its job list sets, text_colours' first), "(C) KONAMI 1989" alone, a row higher than the title's (room under it
   for "@cheviman", menu.c): the menu goes where the others were */
void title_turtles_slide_in(void)
{
	const uint32_t object = OBJECT_ADDRESS(12);

	OBJECT(object)->screen_x -= 0xAu;
	if (OBJECT(object)->screen_x >= 0x100u)
		return;
	OBJECT(object)->screen_x = 0x100u;
	attract_timer = 0xF0u;
	scene_step++;
	print_last_line((version & 1u) != 0u ? TITLE_COPYRIGHT_HERO : TITLE_COPYRIGHT_NINJA, text_colours[0],
		COPYRIGHT_ROWS_UP);
}

/* BEST 10 PLAYERS' table: none under OPTIONS, which shows its picture alone */
void draw_best_players(void)
{
	if (!OPTIONS_PAGE(console.page))
		original_draw_best_players();
}

/* The line under a box with no lives: the game's PRESS START (message_press_start, blinking) where the
   cabinet's says INSERT COIN(S); cleared when no START brings the turtle in (Splinter Way's continues used) */
void message_insert_coin(uint8_t player_message, uint32_t player, uint32_t *p_colour, uint32_t *p_text_number)
{
	const int slot = (int)((player - PLAYER_ADDRESS(0)) / 8u) & 3;

	if (console.page != PAGE_GAME || start_open(slot))
	{
		message_press_start(player_message, player, p_colour, p_text_number);
		return;
	}
	*p_text_number = BLANK_LINE;
	*p_colour = (*p_colour & 0xFFFF0000u) | (print_text_at(*p_colour, BLANK_LINE, player_message) & 0xFFFFu);
}

/* ---- the port ---- */

bool console_attach(const char *folder)
{
	console.infinite_lives = true;    /* (KONAMI CODE's at first; set here, not in the data: their bytes matched a
	                                     stretch of the sprites' planes, tools/rom_in_executables.py) */
	console_settings.dsw[0] = TMNT_DSW1_DEFAULT;    /* (the coins: 1 coin 1 credit, a START's coin a credit) */
	tmnt_board.dsw[0] = console_settings.dsw[0];
	tmnt_board.dsw[1] = console_settings.dsw[1];
	console_settings.dsw[2] |= 0x01u;    /* (the screen the right way up: OPTIONS has no SCREEN, Sebas) */
	tmnt_board.dsw[2] = console_settings.dsw[2] | TMNT_DSW3_SERVICE;    /* (the test mode's switch off) */
	console_settings.dsw[2] = tmnt_board.dsw[2];
	return draw_load_font(folder);
}

void console_overlay(console_picture *frame)
{
	if (!console.powered_on && game_state <= GAME_STATE_ATTRACT)
		presentation_overlay(frame);
	else
		menu_overlay(frame);
	boss_rush_overlay(frame);
	if (console_lan_playing() && game_state < GAME_STATE_ATTRACT)
	{
		/* (a LAN game's start: the board's boot, its RAM and ROM checks, under black) */
		draw_fill(frame, PICTURE_FIRST(frame), 0, PICTURE_END(frame) - PICTURE_FIRST(frame), TMNT_VIDEO_HEIGHT,
			0x000000u);
		draw_game_letters_centred(frame, 108, 1, GAME_LETTERS_OWN, "LAN GAME");
	}
	if (console_lan_waiting())
	{
		char text[40];

		snprintf(text, sizeof(text), "WAITING FOR %s", console_lan_waiting());
		draw_shade(frame, PICTURE_FIRST(frame), 100, PICTURE_END(frame) - PICTURE_FIRST(frame), 24, 80);
		draw_game_letters_centred(frame, 108, 1, GAME_LETTERS_OWN, text);
	}
	/* the picture's first 6 columns black: the K052109 shows layers A and B 6 pixels further than the fixed layer
	   (tmnt.cpp: -90 and -96), and there they show the end of their maps, which the game never draws for that
	   place; the cabinet's monitor hid them under its bezel (the arcade build keeps them, as MAME draws them). The
	   window leaves them out; WIDE's picture shows them, with what lies past them (wide.c) */
	if (!frame->left && !frame->right)
		draw_fill(frame, 0, 0, LEFT_STRIPE, TMNT_VIDEO_HEIGHT, 0x000000u);
	else
	{
		wide_picture_sides(frame);
		if (console_wide_hidden_left())
			draw_fill(frame, PICTURE_FIRST(frame), 0, console_wide_hidden_left(), TMNT_VIDEO_HEIGHT, 0x000000u);
		if (console_wide_hidden_right())    /* (the Technodrome's lift in 20:9, wide.c) */
			draw_fill(frame, PICTURE_END(frame) - console_wide_hidden_right(), 0, console_wide_hidden_right(),
				TMNT_VIDEO_HEIGHT, 0x000000u);
	}
}

bool console_quit_requested(void)
{
	return console.quit;
}

bool console_can_pause(void)
{
	return console.page == PAGE_GAME && game_state == GAME_STATE_GAME && !console_lan_playing();    /* (a LAN game's
		pause would hold every device: Escape leaves it) */
}

/* The game ended where it is, as its own game over ends it (back_to_attract): the play, the layers and the players
   cleared, the sound silenced; the console's menu (or RESTART's turtle choice) at the attract mode's next frame */
void console_quit_game(void)
{
	clear_play();
	clear_layers();
	clear_player_1();
	clear_player_2();
	clear_player_3();
	clear_player_4();
	players_in_play = 0u;
	players_joining = 0u;
	queue_sound_command(TITLE_MUSIC_STOP);
	queue_sound_command(SOUND_SILENCE);
	back_to_attract();
}

/* RESTART: the same game again, every player on the turtle he plays (those who came in during the game too) */
void console_restart_game(void)
{
	for (int player = 0; player < CONSOLE_PLAYERS; player++)
	{
		console.choice_turtle[player] = console_slot_of[player];
		console.choice_picked[player] = console_slot_of[player] >= 0;
	}
	console_quit_game();
	console.restarting = true;
}

void console_controls_held(const uint8_t pressed[CONSOLE_PLAYERS])
{
	for (int player = 0; player < CONSOLE_PLAYERS; player++)
		controls_last[player] = pressed[player];
}

void console_describe(char *text, size_t size)
{
	static const char *const pages[] = { "presentation", "menu", "splinter way", "level select", "options", "video",
		"sound test", "controls", "lan", "lan name", "lan rooms", "lan room", "choice", "starting", "game" };

	snprintf(text, size, "page %s, mode %s, turtle %d, slots %d %d %d %d, coins %d %d %d %d, lives %X, area %u, difficulty %u%s%s%s%s",
		pages[console.page],
		console.mode == MODE_SPLINTER_WAY ? "splinter way" : "arcade", console.turtle, console_slot_of[0],
		console_slot_of[1], console_slot_of[2], console_slot_of[3], console_coins_put[0], console_coins_put[1],
		console_coins_put[2], console_coins_put[3], (unsigned)lives_per_credit, (unsigned)area, (unsigned)difficulty,
		console.level_select ? (console.infinite_lives ? ", level select, infinite lives" : ", level select") : "",
		console.level_select && console.original_turtles ? ", original turtles" : "", console_same_turtle >= 0 ? ", same turtle" : "", console.ronin ? ", ronin" : console.boss_rush ? (console.rush_crossing ? ", boss rush, crossing" : ", boss rush") : "");
}
