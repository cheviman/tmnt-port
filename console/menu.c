/**
 *    Name:        menu.c
 *    Description: the console's menus over the attract mode's own screens: the menu under the title's logo
 *                 (ARCADE, SPLINTER WAY, OPTIONS, EXIT), Splinter Way's modes, OPTIONS over BEST 10 PLAYERS'
 *                 picture, and the turtle choice on the coin's screen
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The menus hold an attract step on the screen while they are open (the title, BEST 10 PLAYERS, the coin's
 *    screen: their own set-up, as the attract mode runs them), and write over it with the game's own letters
 *    (draw.c). The menu's screen is the title's logo on its light blue (title_turtles_slide_in in the console's
 *    version: of the copyright lines "(C) KONAMI 1989" alone), with "@cheviman" under it (Sebas). The turtle choice
 *    is the coin's screen: the four turtles' pictures (draw_turtle_pictures, as the coin's screen draws them), the
 *    ones not chosen darkened over them; the game starts as the cabinet's does, the coin and the attack on that
 *    turtle's slot (console.c). The menus' music is the staff roll's song (Sebas); the turtle choice has the coin's
 *    screen's, which that screen starts itself.
 */
#include "console_internal.h"

#include "tmnt/sound_board.h"    /* (before game.h: ram.h's names are macros) */
#include "game.h"
#include "tmnt/machine.h"

#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       224
#define ITEM_COLOUR         0xFFFFFFu
#define CHOSEN_COLOUR       0xFFD840u    /* yellow */
#define HEADING_COLOUR      0x60E060u    /* the turtles' green */
#define SIGNATURE_COLOUR    0xFFFFFFu
#define MENU_Y              122          /* the menu, under the logo (where the title's copyright lines were) */
#define ITEM_STEP           12
#define KONAMI_LINES_TOP    192          /* the coin's screen's Konami logo and (C) KONAMI 1989, to its bottom */
#define SIGNATURE           "@cheviman"  /* the port's author, under the title's (C) KONAMI 1989 (Sebas) */
#define SIGNATURE_Y         210          /* under "(C) KONAMI 1989", which console.c puts at 200-207 */
#define VERSION_TEXT        "V" TMNT_VERSION    /* OPTIONS: the port's release, at the bottom right (Sebas: as
                                            outrunners-port's) */
#define VERSION_Y           212
#define VERSION_MARGIN      3            /* its right edge, to the screen's */
#define PAGE_Y              28           /* OPTIONS' heading, and its pages' */
#define LINE_Y              52           /* and its first line */
#define LINE_STEP           13
#define LINE_LEFT           40
#define LINE_RIGHT          (SCREEN_WIDTH - 40)
/* (the menus' sounds go straight into the sound board's queue, queue_sound_command: the menus are no demo, so
   they sound whatever DEMO SOUND says) */
#define MOVE_SOUND          0xA2u        /* speech A2, the uPD7759's sample 2 (speech_02): a line moved to, a
                                            setting or the turtle changed (Sebas: not the name entry's letter, 1D,
                                            then this one and the coin's for the presentation); a line picked or a
                                            page left sounds nothing */
#define MENU_SONG           0x70u        /* the staff roll's song (staff_roll): the menus' music (Sebas), which the
                                            game plays once (the staff roll): asked for again when it ends */
#define MUSIC_RECORDS       0x0040u      /* the music's channel records in the sound board's RAM (8040-827F: its FM
                                            channels 1-7, its PCM channels; tools' sound_symbols.txt) */
#define MUSIC_RECORD_COUNT  9
#define RECORD_SIZE         0x40u
#define RECORD_SOUND        1u           /* a record's sound, 0 when free */
#define SONG_START_FRAMES   30           /* the song's command reaching the sound board, its channels taken */
#define COIN_SCREEN_SONG    0x6Fu        /* the coin's screen's, which coin_screen_draw starts: the turtle choice's */
#define TITLE_TIME          0xF0u        /* the title's wait, kept from running out (title_wait) */
#define BEST_PLAYERS_TIME   0xFFu        /* BEST 10 PLAYERS' (best_players_scroll) */
#define PICTURES_SLOTS      3u           /* draw_turtle_pictures: slots 3 to 0 */
#define PICTURE_DIMMED      72           /* of 256: the pictures of the turtles not chosen */
#define PICTURE_LEFT       20           /* the first slot's picture on the coin's screen, and the next one's */
#define PICTURE_STEP        72
#define PICTURE_TOP         56
#define PICTURE_WIDTH       64
#define PICTURE_HEIGHT      128
#define CHOICE_HEADING_Y    44
#define CHOICE_NAME_Y       188
#define PAUSE_Y             76           /* the pause's PAUSE, and its lines under it */
#define PAUSE_LINES_Y       108
#define PAUSE_LINE_STEP     14

enum { MENU_ARCADE, MENU_SPLINTER_WAY, MENU_OPTIONS, MENU_EXIT, MENU_ITEMS };
enum { WAY_NORMAL, WAY_BOSS_RUSH, WAY_RONIN, WAY_BACK, WAY_ITEMS };
enum { OPTION_CONTROLS, OPTION_DIFFICULTY, OPTION_LIVES, OPTION_DEMO_SOUND, OPTION_SCREEN, OPTION_REGION,
	OPTION_VIDEO, OPTION_VOLUME, OPTION_SOUND_TEST, OPTION_FPS, OPTION_BACK, OPTION_ITEMS };
enum { VIDEO_WINDOW, VIDEO_FULL_SCREEN, VIDEO_PIXELS, VIDEO_WIDE, VIDEO_BACK, VIDEO_ITEMS };
enum { SOUND_MUSIC, SOUND_EFFECT, SOUND_VOICE, SOUND_STOP, SOUND_BACK, SOUND_ITEMS };

static const char *const menu_items[MENU_ITEMS] = { "ARCADE", "SPLINTER WAY", "OPTIONS", "EXIT" };
static const char *const way_items[WAY_ITEMS] = { "NORMAL", "BOSS RUSH", "RONIN", "BACK" };
/* OPTIONS as outrunners-port's (CONTROLS, VIDEO, VOLUME, SOUND TEST, FPS: Sebas), with the DIP switches by the test
   mode's DIP SW SELECT names (test_mode_texts), but its PLAYERS, the lives a credit gives (1, 2, 3 or 5), as LIVES;
   no coins at home, so no COIN1 (Sebas) */
static const char *const option_items[OPTION_ITEMS] = { "CONTROLS", "DIFFICULTY", "LIVES", "DEMO SOUND", "SCREEN",
	"REGION", "VIDEO", "VOLUME", "SOUND TEST", "FPS", "BACK" };
static const char *const video_items[VIDEO_ITEMS] = { "WINDOW", "FULL SCREEN", "PIXELS", "WIDE", "BACK" };
static const char *const wide_kinds[WIDE_KINDS] = { "OFF", "16:9", "20:9" };
static const char *const sound_items[SOUND_ITEMS] = { "MUSIC", "EFFECT", "VOICE", "STOP", "BACK" };
/* SOUND TEST's songs in the game's order, by the names tools/sound_music.py gives them (music.txt): the songs'
   commands 60-78 (74 plays 60's song again: left out) and the title music (7D) */
static const struct
{
	uint8_t command;
	const char *name;
} songs[] = {
	{ 0x7Du, "TITLE" }, { 0x63u, "AREA 0" }, { 0x68u, "AREA 1" }, { 0x67u, "AREA 2" }, { 0x65u, "AREA 3" },
	{ 0x66u, "AREA 4 AND 7" }, { 0x61u, "AREA 5" }, { 0x60u, "AREA 6" }, { 0x69u, "AREA 8" }, { 0x62u, "AREA 9" },
	{ 0x64u, "BOSS" }, { 0x6Au, "SHREDDER APPEARS" }, { 0x6Eu, "AREA EXIT" }, { 0x72u, "SHREDDER TAKES APRIL" },
	{ 0x73u, "AREA 2 AND 8 EXIT" }, { 0x75u, "AREA 7 EXIT DROP" }, { 0x6Fu, "COIN SCREEN" }, { 0x78u, "PROFILE" },
	{ 0x77u, "CONTINUE" }, { 0x6Du, "GAME OVER" }, { 0x71u, "RANKING" }, { 0x6Cu, "ENDING" }, { 0x70u, "STAFF ROLL" },
	{ 0x6Bu, "SONG 6B" }, { 0x76u, "SONG 76" },
};
#define SONGS               ((int)(sizeof(songs) / sizeof(songs[0])))
/* its effects: the FM ones (01-41) and the PCM ones (80-93), by their commands; its voices, the uPD7759's samples
   (A0-C3: speech_00 to speech_35 in the assets) */
#define FM_EFFECTS          0x41
#define PCM_EFFECT_FIRST    0x80
#define EFFECTS             (FM_EFFECTS + 0x14)
#define VOICE_FIRST         0xA0u
#define VOICES              36
#define SOUND_SILENCE       0xFFu        /* the sound program's silence */
#define TITLE_MUSIC_STOP    0x7Eu
/* the DIP switch's four (the test mode says DIFFICULT and VERY DIFFICULT: Sebas, "es hard y very hard") */
static const char *const difficulties[4] = { "EASY", "NORMAL", "HARD", "VERY HARD" };
static const char *const filters[PIXELS_KINDS] = { "SHARP", "SMOOTH", "BILINEAR", "CRT" };
/* the version byte's values, by what the title shows with them (console/PLAN.md): TEENAGE MUTANT NINJA TURTLES
   (the World and US sets'), HERO (the UK's), NINJA with its Japanese subtitle under TURTLES (the Japanese set's,
   the banner a little higher), and HERO with the subtitle (no set's) */
static const char *const regions[CONSOLE_REGIONS] = { "NINJA", "HERO", "JAPAN", "HERO JAPAN" };
static const char *const turtle_names[4] = { "LEONARDO", "MICHELANGELO", "DONATELLO", "RAPHAEL" };

static uint8_t song;           /* the song the menus asked for, or the coin's screen started (0: none) */
static int sound_song, sound_effect, sound_voice;    /* SOUND TEST's: a song (songs[]), an effect, a voice */

/* CONTROLS: PLAYER on top, the controls in two columns (each its name, and under it the pictures of its keys and
   button, the port's), DEFAULTS and BACK side by side at the bottom; as outrunners-port's */
enum { CONTROLS_PLAYER, CONTROLS_FIRST, CONTROLS_DEFAULTS = CONTROLS_FIRST + CONTROLS, CONTROLS_BACK };
#define CONTROLS_COLUMN_ROWS  4      /* UP, DOWN, LEFT, RIGHT; then ATTACK, JUMP, START */
#define CONTROLS_HEADING_Y    18
#define CONTROLS_PLAYER_Y     44
#define CONTROLS_CELL_Y       64     /* the first row of controls: their names */
#define CONTROLS_CELL_STEP    30     /* a name, its pictures under it, a gap */
#define CONTROLS_PICTURES_Y   10     /* under the name */
#define CONTROLS_BOTTOM_Y     194    /* DEFAULTS and BACK, or PRESS A KEY OR BUTTON */
#define CONTROLS_SETTLE       2      /* frames the menus take no action after a key or button is bound (it is held) */
static const int controls_column_x[2] = { 40, 176 };
static const char *const control_names[CONTROLS] = { "UP", "DOWN", "LEFT", "RIGHT", "ATTACK", "JUMP", "START" };
static int controls_player;        /* whose controls are shown */
static int controls_waiting = -1;  /* the control waiting for its key or button, or -1 */
static bool controls_defaults;     /* DEFAULTS picked, for the port */
static int controls_settle;        /* frames left without actions after a binding */
static int song_frames;        /* the frames since the menus asked for theirs */
static bool settings_touched;  /* OPTIONS changed a switch: onto the board when it closes */

/* ---- the attract steps the pages hold ---- */

/* 'step' of the attract mode on the screen, from its start (the region's version for the title's logo) */
static void show_step(uint8_t step)
{
	if (game_step == step)
		return;
	if (step == ATTRACT_TITLE)
		version = (uint8_t)console_settings.region;
	game_step = step;
	scene = 0u;
	scene_step = 0u;
	scene_substep = 0u;
	scene_subsubstep = 0u;
}

/* The title's logo in its place (TURTLES in, the copyright line printed) */
static bool logo_in(void)
{
	return game_step == ATTRACT_TITLE && scene == 1u && scene_step >= 3u;
}

static void hold_steps(void)
{
	if (logo_in())
		attract_timer = TITLE_TIME;
	if (game_step == ATTRACT_BEST_PLAYERS && scene == 1u)
		step_timer = BEST_PLAYERS_TIME;
}

/* ---- the pages ---- */

static void open_page(int page, int choice)
{
	console.page = page;
	console.choice = choice;
	console.page_frames = 0;
}

static int step(int value, int by, int low, int high)
{
	value += by;
	return value < low ? high : (value > high ? low : value);
}

/* LEFT or RIGHT: -1, +1 or 0 */
static int change(uint32_t pressed)
{
	return (pressed & ACTION_BIT(ACTION_LEFT)) ? -1 : ((pressed & ACTION_BIT(ACTION_RIGHT)) ? 1 : 0);
}

static bool picked(uint32_t pressed)
{
	return (pressed & (ACTION_BIT(ACTION_ACCEPT) | ACTION_BIT(ACTION_START))) != 0;
}

static void choose_turtle(int mode)
{
	console.mode = mode;
	console.turtle = 0;
	open_page(PAGE_CHOICE, 0);
}

static void menu_page(uint32_t pressed)
{
	if (!picked(pressed))
		return;
	switch (console.choice)
	{
		case MENU_ARCADE:
			choose_turtle(MODE_ARCADE);
			break;
		case MENU_SPLINTER_WAY:
			open_page(PAGE_SPLINTER_WAY, WAY_NORMAL);
			break;
		case MENU_OPTIONS:
			open_page(PAGE_OPTIONS, 0);
			break;
		case MENU_EXIT:
			console.quit = true;
			break;
	}
}

static void way_page(uint32_t pressed)
{
	if (pressed & ACTION_BIT(ACTION_BACK))
	{
		open_page(PAGE_MENU, MENU_SPLINTER_WAY);
		return;
	}
	if (!picked(pressed))
		return;
	if (console.choice == WAY_NORMAL)
		choose_turtle(MODE_SPLINTER_WAY);
	else if (console.choice == WAY_BACK)
		open_page(PAGE_MENU, MENU_SPLINTER_WAY);
}

/* The DIP switches as OPTIONS shows them (active low on the board) */
static int lives_setting(void)
{
	return ~console_settings.dsw[1] & 3;
}

static int difficulty_switch(void)
{
	return (~console_settings.dsw[1] & 0x60) >> 5;
}

static bool demo_sound_on(void)
{
	return (console_settings.dsw[1] & 0x80) == 0;
}

static bool upside_down(void)
{
	return (console_settings.dsw[2] & 0x01) == 0;
}

static void set_bits(uint8_t *bank, uint8_t mask, int shift, int value)
{
	*bank = (uint8_t)((*bank & ~mask) | ((~value << shift) & mask));
}

static void options_page(uint32_t pressed)
{
	const int by = change(pressed) ? change(pressed) : (picked(pressed) ? 1 : 0);    /* (ACCEPT: the next value) */
	console_settings_t *settings = &console_settings;

	if ((pressed & ACTION_BIT(ACTION_BACK)) || (picked(pressed) && console.choice == OPTION_BACK))
	{
		if (settings_touched)
			console_apply_dip_switches();
		settings_touched = false;
		settings->changed = true;    /* (the port keeps them) */
		open_page(PAGE_MENU, MENU_OPTIONS);
		return;
	}
	if (picked(pressed))
		switch (console.choice)
		{
			case OPTION_CONTROLS:
				open_page(PAGE_CONTROLS, 0);
				return;
			case OPTION_VIDEO:
				open_page(PAGE_VIDEO, 0);
				return;
			case OPTION_SOUND_TEST:
				open_page(PAGE_SOUND_TEST, 0);
				return;
			default:
				break;
		}
	if (!by)
		return;
	switch (console.choice)
	{
		case OPTION_DIFFICULTY:
			set_bits(&settings->dsw[1], 0x60, 5, step(difficulty_switch(), by, 0, 3));
			settings_touched = true;
			break;
		case OPTION_LIVES:
			set_bits(&settings->dsw[1], 0x03, 0, step(lives_setting(), by, 0, 3));
			settings_touched = true;
			break;
		case OPTION_DEMO_SOUND:
			settings->dsw[1] ^= 0x80;
			settings_touched = true;
			break;
		case OPTION_SCREEN:
			settings->dsw[2] ^= 0x01;
			settings_touched = true;
			break;
		case OPTION_REGION:
			settings->region = step(settings->region, by, 0, CONSOLE_REGIONS - 1);
			break;
		case OPTION_VOLUME:
			settings->volume = step(settings->volume, by, 0, 10);
			settings->changed = true;
			break;
		case OPTION_FPS:
			settings->fps = !settings->fps;
			settings->changed = true;
			break;
		default:
			break;
	}
}

/* VIDEO: the window's size (1 to 6 times the picture), full screen, the pixels, WIDE; the port applies them */
static void video_page(uint32_t pressed)
{
	const int by = change(pressed) ? change(pressed) : (picked(pressed) ? 1 : 0);
	console_settings_t *settings = &console_settings;

	if ((pressed & ACTION_BIT(ACTION_BACK)) || (picked(pressed) && console.choice == VIDEO_BACK))
	{
		open_page(PAGE_OPTIONS, OPTION_VIDEO);
		return;
	}
	if (!by)
		return;
	switch (console.choice)
	{
		case VIDEO_WINDOW:
			settings->window_scale = step(settings->window_scale, by, 1, 6);
			break;
		case VIDEO_FULL_SCREEN:
			settings->full_screen = !settings->full_screen;
			break;
		case VIDEO_PIXELS:
			settings->pixels = step(settings->pixels, by, 0, PIXELS_KINDS - 1);
			break;
		case VIDEO_WIDE:
			settings->wide = step(settings->wide, by, 0, WIDE_KINDS - 1);
			break;
		default:
			return;
	}
	settings->changed = true;
}

/* SOUND TEST: LEFT and RIGHT choose a song, an effect or a voice, ACCEPT plays it (a song after silence); STOP
   silences it all; leaving it, the menus' song again if another sounded */
static void sound_test_options_page(uint32_t pressed)
{
	const int by = change(pressed);

	if ((pressed & ACTION_BIT(ACTION_BACK)) || (picked(pressed) && console.choice == SOUND_BACK))
	{
		if (song == 0u)
		{
			queue_sound_command(TITLE_MUSIC_STOP);
			queue_sound_command(SOUND_SILENCE);
		}
		open_page(PAGE_OPTIONS, OPTION_SOUND_TEST);
		return;
	}
	switch (console.choice)
	{
		case SOUND_MUSIC:
			sound_song = step(sound_song, by, 0, SONGS - 1);
			if (picked(pressed))
			{
				queue_sound_command(TITLE_MUSIC_STOP);
				queue_sound_command(SOUND_SILENCE);
				queue_sound_command(songs[sound_song].command);
				song = 0u;
			}
			break;
		case SOUND_EFFECT:
			sound_effect = step(sound_effect, by, 0, EFFECTS - 1);
			if (picked(pressed))
				queue_sound_command((uint8_t)(sound_effect < FM_EFFECTS ? 1 + sound_effect
					: PCM_EFFECT_FIRST + sound_effect - FM_EFFECTS));
			break;
		case SOUND_VOICE:
			sound_voice = step(sound_voice, by, 0, VOICES - 1);
			if (picked(pressed))
				queue_sound_command((uint8_t)(VOICE_FIRST + sound_voice));
			break;
		case SOUND_STOP:
			if (picked(pressed))
			{
				queue_sound_command(TITLE_MUSIC_STOP);
				queue_sound_command(SOUND_SILENCE);
				song = 0u;
			}
			break;
		default:
			break;
	}
}

/* ---- CONTROLS ---- */

static void control_place(int control, int *x, int *y)
{
	*x = controls_column_x[control / CONTROLS_COLUMN_ROWS];
	*y = CONTROLS_CELL_Y + (control % CONTROLS_COLUMN_ROWS) * CONTROLS_CELL_STEP;
}

/* The arrows on the grid: up and down within a column (to PLAYER above it, to DEFAULTS or BACK under it), left and
   right across the columns (on PLAYER: the player) */
static void controls_move(uint32_t pressed)
{
	const int choice = console.choice;
	const int control = choice - CONTROLS_FIRST;
	const bool on_control = choice >= CONTROLS_FIRST && choice < CONTROLS_DEFAULTS;
	const int column = on_control ? control / CONTROLS_COLUMN_ROWS : (choice == CONTROLS_BACK ? 1 : 0);
	const int column_last = column ? CONTROLS - 1 : CONTROLS_COLUMN_ROWS - 1;

	if (pressed & ACTION_BIT(ACTION_UP))
	{
		if (choice == CONTROLS_DEFAULTS || choice == CONTROLS_BACK)
			console.choice = CONTROLS_FIRST + column_last;
		else if (on_control && control % CONTROLS_COLUMN_ROWS != 0)
			console.choice = choice - 1;
		else if (on_control)
			console.choice = CONTROLS_PLAYER;
		else
			console.choice = CONTROLS_DEFAULTS;    /* (from PLAYER round to the bottom) */
	}
	else if (pressed & ACTION_BIT(ACTION_DOWN))
	{
		if (choice == CONTROLS_PLAYER)
			console.choice = CONTROLS_FIRST;
		else if (on_control && control != column_last)
			console.choice = choice + 1;
		else if (on_control)
			console.choice = column ? CONTROLS_BACK : CONTROLS_DEFAULTS;
		else
			console.choice = CONTROLS_PLAYER;
	}
	else if (change(pressed))
	{
		if (choice == CONTROLS_PLAYER)
			controls_player = step(controls_player, change(pressed), 0, CONTROL_PLAYERS - 1);
		else if (on_control)
		{
			const int row = control % CONTROLS_COLUMN_ROWS;
			const int other = column ? row : CONTROLS_COLUMN_ROWS + row;

			console.choice = CONTROLS_FIRST + (other > CONTROLS - 1 ? CONTROLS - 1 : other);
		}
		else
			console.choice = choice == CONTROLS_DEFAULTS ? CONTROLS_BACK : CONTROLS_DEFAULTS;
	}
}

static void controls_page(uint32_t pressed)
{
	if (controls_waiting >= 0)
		return;    /* (the port takes the next key or button: console_controls_bound) */
	if (controls_settle > 0)
	{
		controls_settle--;
		return;
	}
	if ((pressed & ACTION_BIT(ACTION_BACK)) || (picked(pressed) && console.choice == CONTROLS_BACK))
	{
		open_page(PAGE_OPTIONS, OPTION_CONTROLS);
		return;
	}
	controls_move(pressed);
	if (!picked(pressed))
		return;
	if (console.choice >= CONTROLS_FIRST && console.choice < CONTROLS_DEFAULTS)
		controls_waiting = console.choice - CONTROLS_FIRST;
	else if (console.choice == CONTROLS_DEFAULTS)
		controls_defaults = true;
}

bool console_controls_place(int control, int *x, int *y)
{
	if (console.page != PAGE_CONTROLS || game_state != GAME_STATE_ATTRACT || controls_waiting >= 0 || control < 0
		|| control >= CONTROLS)
		return false;
	control_place(control, x, y);
	*y += CONTROLS_PICTURES_Y;
	return true;
}

int console_controls_player(void)
{
	return controls_player;
}

int console_controls_waiting(void)
{
	return console.page == PAGE_CONTROLS ? controls_waiting : -1;
}

void console_controls_bound(void)
{
	controls_waiting = -1;
	controls_settle = CONTROLS_SETTLE;
}

bool console_controls_defaults(void)
{
	const bool chosen = controls_defaults;

	controls_defaults = false;
	return chosen;
}

/* The coin's screen's pictures: the four turtles' while one is chosen (the others darkened over them,
   choice_overlay), then the chosen one's */
static void draw_pictures(void)
{
	if (game_step != ATTRACT_COIN_SCREEN || scene != 1u || scene_step < 4u)
		return;
	draw_turtle_pictures(PICTURES_SLOTS, console.page == PAGE_CHOICE ? 0xFu : 1u << console.turtle);
}

static void choice_page(uint32_t pressed)
{
	if (pressed & ACTION_BIT(ACTION_BACK))
	{
		free_play = 0u;
		if (console.mode == MODE_SPLINTER_WAY)
			open_page(PAGE_SPLINTER_WAY, WAY_NORMAL);
		else
			open_page(PAGE_MENU, MENU_ARCADE);
		return;
	}
	console.turtle = step(console.turtle, change(pressed), 0, 3);
	if ((picked(pressed) || console.restarting) && scene == 1u && scene_step >= 4u)
	{
		/* the chosen turtle's slot for its player, and its coin and attack: the game starts */
		if (!console.restarting)
			console.chooser = console.last_presser;    /* (RESTART: the same player) */
		console.restarting = false;
		console_set_mode_lives(console.mode);
		for (int player = 0; player < CONSOLE_PLAYERS; player++)
		{
			console_slot_of[player] = -1;
			console_coins_put[player] = 0;
		}
		console_slot_of[console.chooser] = console.turtle;
		console_join(console.turtle);
		open_page(PAGE_STARTING, 0);
	}
}

/* Whether a song plays: one of the music's channel records has a sound (a song that ends frees them all) */
static bool music_playing(void)
{
	for (unsigned record = 0; record < MUSIC_RECORD_COUNT; record++)
		if (tmnt_sound.ram[MUSIC_RECORDS + record * RECORD_SIZE + RECORD_SOUND] != 0u)
			return true;
	return false;
}

void menu_frame(uint32_t pressed)
{
	const int page = console.page;
	int lines = 0;

	console.page_frames++;
	switch (page)
	{
		case PAGE_MENU: lines = MENU_ITEMS; break;
		case PAGE_SPLINTER_WAY: lines = WAY_ITEMS; break;
		case PAGE_OPTIONS: lines = OPTION_ITEMS; break;
		case PAGE_VIDEO: lines = VIDEO_ITEMS; break;
		case PAGE_SOUND_TEST: lines = SOUND_ITEMS; break;
		default: break;
	}
	if (lines)
	{
		if (pressed & ACTION_BIT(ACTION_UP))
			console.choice = step(console.choice, -1, 0, lines - 1);
		if (pressed & ACTION_BIT(ACTION_DOWN))
			console.choice = step(console.choice, 1, 0, lines - 1);
	}
	if (page != PAGE_SOUND_TEST && ((lines && (pressed & (ACTION_BIT(ACTION_UP) | ACTION_BIT(ACTION_DOWN))))
		|| ((page == PAGE_OPTIONS || page == PAGE_VIDEO || page == PAGE_CHOICE) && change(pressed))
		|| (page == PAGE_CONTROLS && controls_waiting < 0 && controls_settle == 0
			&& (pressed & (ACTION_BIT(ACTION_UP) | ACTION_BIT(ACTION_DOWN) | ACTION_BIT(ACTION_LEFT)
				| ACTION_BIT(ACTION_RIGHT))))))
		queue_sound_command(MOVE_SOUND);    /* (none on SOUND TEST: it would cut what is tested) */
	switch (page)
	{
		case PAGE_MENU:
			menu_page(pressed);
			break;
		case PAGE_SPLINTER_WAY:
			way_page(pressed);
			break;
		case PAGE_OPTIONS:
			options_page(pressed);
			break;
		case PAGE_VIDEO:
			video_page(pressed);
			break;
		case PAGE_SOUND_TEST:
			sound_test_options_page(pressed);
			break;
		case PAGE_CONTROLS:
			controls_page(pressed);
			break;
		case PAGE_CHOICE:
			choice_page(pressed);
			break;
		default:
			break;
	}
	/* the attract step the page shows, held there */
	switch (console.page)
	{
		case PAGE_MENU:
		case PAGE_SPLINTER_WAY:
			show_step(ATTRACT_TITLE);
			break;
		case PAGE_OPTIONS:
		case PAGE_VIDEO:
		case PAGE_SOUND_TEST:
		case PAGE_CONTROLS:
			show_step(ATTRACT_BEST_PLAYERS);
			break;
		case PAGE_CHOICE:
		case PAGE_STARTING:
			free_play = 1u;    /* (the coin's screen without credits, as with the switch's free play) */
			show_step(ATTRACT_COIN_SCREEN);
			break;
		default:
			break;
	}
	hold_steps();
	if (console.page == PAGE_CHOICE || console.page == PAGE_STARTING)
		draw_pictures();
	/* the menus' song, but on the coin's screen (its own) and from the game's start (the game's own: the menus'
	   song again after it) */
	if (console.page == PAGE_SOUND_TEST)
		;    /* (its own sounds: the menus' song again after it, if another sounded) */
	else if (console.page == PAGE_CHOICE)
		song = COIN_SCREEN_SONG;
	else if (console.page == PAGE_STARTING)
		song = 0u;
	else if (song != MENU_SONG || (++song_frames > SONG_START_FRAMES && !music_playing()))
	{
		song = MENU_SONG;
		song_frames = 0;
		queue_sound_command(MENU_SONG);
	}
}

/* ---- the picture ---- */

static tmnt_frame *target;    /* the frame being drawn over */

/* The menus write with the game's own letters (Sebas: the white and light blue ones; draw.c): an item in the game's
   colours, the one chosen in yellow */
static void list(const char *const *items, int count, int y)
{
	for (int index = 0; index < count; index++)
		draw_game_letters_centred(target, y + index * ITEM_STEP, 1,
			index == console.choice ? CHOSEN_COLOUR : GAME_LETTERS_OWN, items[index]);
}

/* A page of OPTIONS': its heading twice the letters' size over its 'lines', the picture darkened behind them */
static void page_box(const char *heading, int lines)
{
	draw_shade(target, LINE_LEFT - 12, PAGE_Y - 8, LINE_RIGHT - LINE_LEFT + 24, LINE_Y - PAGE_Y + lines * LINE_STEP + 10, 80);
	draw_game_letters_centred(target, PAGE_Y, 2, GAME_LETTERS_OWN, heading);
}

/* A line of a page: its name at the left, its value (or none: a page of its own, or BACK) at the right */
static void option_line(const char *const *items, int index, const char *value)
{
	const uint32_t colour = index == console.choice ? CHOSEN_COLOUR : GAME_LETTERS_OWN;
	const int y = LINE_Y + index * LINE_STEP;

	draw_game_letters(target, LINE_LEFT, y, 1, colour, items[index]);
	if (value)
		draw_game_letters(target, LINE_RIGHT - draw_game_letters_width(value, 1), y, 1, colour, value);
}

static void options_overlay(void)
{
	char value[40];

	page_box("OPTIONS", OPTION_ITEMS);
	option_line(option_items, OPTION_CONTROLS, NULL);
	option_line(option_items, OPTION_DIFFICULTY, difficulties[difficulty_switch()]);
	snprintf(value, sizeof(value), "%d", lives_settings[lives_setting()]);
	option_line(option_items, OPTION_LIVES, value);
	option_line(option_items, OPTION_DEMO_SOUND, demo_sound_on() ? "ON" : "OFF");
	option_line(option_items, OPTION_SCREEN, upside_down() ? "UPSIDE DOWN" : "NORMAL");
	option_line(option_items, OPTION_REGION, regions[console_settings.region]);
	option_line(option_items, OPTION_VIDEO, NULL);
	snprintf(value, sizeof(value), "%d", console_settings.volume);
	option_line(option_items, OPTION_VOLUME, value);
	option_line(option_items, OPTION_SOUND_TEST, NULL);
	option_line(option_items, OPTION_FPS, console_settings.fps ? "ON" : "OFF");
	option_line(option_items, OPTION_BACK, NULL);
	draw_game_letters(target, SCREEN_WIDTH - VERSION_MARGIN - draw_game_letters_width(VERSION_TEXT, 1), VERSION_Y, 1,
		GAME_LETTERS_OWN, VERSION_TEXT);
}

static void video_overlay(void)
{
	char value[16];

	page_box("VIDEO", VIDEO_ITEMS);
	snprintf(value, sizeof(value), "%dX", console_settings.window_scale);
	option_line(video_items, VIDEO_WINDOW, value);
	option_line(video_items, VIDEO_FULL_SCREEN, console_settings.full_screen ? "ON" : "OFF");
	option_line(video_items, VIDEO_PIXELS, filters[console_settings.pixels]);
	option_line(video_items, VIDEO_WIDE, wide_kinds[console_settings.wide]);
	option_line(video_items, VIDEO_BACK, NULL);
}

static void sound_test_options_overlay(void)
{
	char value[16];
	const int effect = sound_effect < FM_EFFECTS ? 1 + sound_effect : PCM_EFFECT_FIRST + sound_effect - FM_EFFECTS;

	page_box("SOUND TEST", SOUND_ITEMS);
	option_line(sound_items, SOUND_MUSIC, songs[sound_song].name);
	snprintf(value, sizeof(value), "%s %02X", sound_effect < FM_EFFECTS ? "FM" : "PCM", effect);
	option_line(sound_items, SOUND_EFFECT, value);
	snprintf(value, sizeof(value), "%02d", sound_voice);
	option_line(sound_items, SOUND_VOICE, value);
	option_line(sound_items, SOUND_STOP, NULL);
	option_line(sound_items, SOUND_BACK, NULL);
}

static void controls_overlay(void)
{
	char value[16];
	int x, y;

	draw_shade(target, 16, CONTROLS_HEADING_Y - 8, SCREEN_WIDTH - 32, CONTROLS_BOTTOM_Y - CONTROLS_HEADING_Y + 18, 80);
	draw_game_letters_centred(target, CONTROLS_HEADING_Y, 2, GAME_LETTERS_OWN, "CONTROLS");
	if (controls_waiting >= 0)
	{
		control_place(controls_waiting, &x, &y);
		draw_game_letters(target, x, y, 1, CHOSEN_COLOUR, control_names[controls_waiting]);
		draw_game_letters_centred(target, CONTROLS_BOTTOM_Y, 1, GAME_LETTERS_OWN, "PRESS A KEY OR BUTTON");
		return;
	}
	snprintf(value, sizeof(value), "< PLAYER %d >", controls_player + 1);    /* (LEFT and RIGHT: players 1-4) */
	draw_game_letters_centred(target, CONTROLS_PLAYER_Y, 1,
		console.choice == CONTROLS_PLAYER ? CHOSEN_COLOUR : GAME_LETTERS_OWN, value);
	for (int control = 0; control < CONTROLS; control++)
	{
		control_place(control, &x, &y);
		draw_game_letters(target, x, y, 1, console.choice == CONTROLS_FIRST + control ? CHOSEN_COLOUR : GAME_LETTERS_OWN,
			control_names[control]);
	}
	draw_game_letters(target, controls_column_x[0], CONTROLS_BOTTOM_Y, 1,
		console.choice == CONTROLS_DEFAULTS ? CHOSEN_COLOUR : GAME_LETTERS_OWN, "DEFAULTS");
	draw_game_letters(target, controls_column_x[1], CONTROLS_BOTTOM_Y, 1,
		console.choice == CONTROLS_BACK ? CHOSEN_COLOUR : GAME_LETTERS_OWN, "BACK");
}

static void choice_overlay(void)
{
	const int x = PICTURE_LEFT + console.turtle * PICTURE_STEP;
	const char *heading = console.mode == MODE_SPLINTER_WAY ? "SPLINTER WAY - CHOOSE YOUR TURTLE" : "CHOOSE YOUR TURTLE";

	if (game_step != ATTRACT_COIN_SCREEN || scene != 1u || scene_step < 4u)
		return;
	/* the coin's screen's Konami logo and (C) KONAMI 1989 off (Sebas: "es ingame ya") */
	draw_fill(target, 0, KONAMI_LINES_TOP, TMNT_VIDEO_WIDTH, TMNT_VIDEO_HEIGHT - KONAMI_LINES_TOP, 0x000000u);
	for (int turtle = 0; turtle < 4 && console.page == PAGE_CHOICE; turtle++)
		if (turtle != console.turtle)
			draw_shade(target, PICTURE_LEFT + turtle * PICTURE_STEP, PICTURE_TOP, PICTURE_WIDTH, PICTURE_HEIGHT,
				PICTURE_DIMMED);
	draw_game_letters_centred(target, CHOICE_HEADING_Y, 1, GAME_LETTERS_OWN, heading);
	draw_fill(target, x - 2, PICTURE_TOP - 2, PICTURE_WIDTH + 4, 1, CHOSEN_COLOUR);
	draw_fill(target, x - 2, PICTURE_TOP + PICTURE_HEIGHT + 1, PICTURE_WIDTH + 4, 1, CHOSEN_COLOUR);
	draw_fill(target, x - 2, PICTURE_TOP - 2, 1, PICTURE_HEIGHT + 4, CHOSEN_COLOUR);
	draw_fill(target, x + PICTURE_WIDTH + 1, PICTURE_TOP - 2, 1, PICTURE_HEIGHT + 4, CHOSEN_COLOUR);
	draw_game_letters(target, x + (PICTURE_WIDTH - draw_game_letters_width(turtle_names[console.turtle], 1)) / 2,
		CHOICE_NAME_Y, 1, CHOSEN_COLOUR, turtle_names[console.turtle]);
}

/* The pause's picture: the frame it stopped at in grey, PAUSE and its lines over it in the game's letters (as
   outrunners-port's) */
void console_pause_overlay(tmnt_frame *frame, int choice)
{
	static const char *const items[CONSOLE_PAUSE_ITEMS] = { "CONTINUE", "RESTART", "QUIT GAME" };

	for (size_t index = 0; index < (size_t)TMNT_VIDEO_WIDTH * TMNT_VIDEO_HEIGHT; index++)
	{
		const uint32_t colour = frame->pixels[index];
		const uint32_t level = ((((colour >> 16) & 0xFFu) * 77u + ((colour >> 8) & 0xFFu) * 150u + (colour & 0xFFu) * 29u)
			>> 8) * 3u / 5u;

		frame->pixels[index] = level << 16 | level << 8 | level;
	}
	draw_game_letters_centred(frame, PAUSE_Y, 2, GAME_LETTERS_OWN, "PAUSE");
	for (int item = 0; item < CONSOLE_PAUSE_ITEMS; item++)
		draw_game_letters_centred(frame, PAUSE_LINES_Y + item * PAUSE_LINE_STEP, 1,
			item == choice ? CHOSEN_COLOUR : GAME_LETTERS_OWN, items[item]);
}

void menu_overlay(tmnt_frame *frame)
{
	target = frame;
	if (game_state != GAME_STATE_ATTRACT)
		return;
	switch (console.page)
	{
		case PAGE_MENU:
		case PAGE_SPLINTER_WAY:
			if (!logo_in())
				return;
			if (console.page == PAGE_MENU)
				list(menu_items, MENU_ITEMS, MENU_Y);
			else
				list(way_items, WAY_ITEMS, MENU_Y);
			draw_small_letters_centred(frame, SIGNATURE_Y, SIGNATURE);
			break;
		case PAGE_OPTIONS:
			options_overlay();
			break;
		case PAGE_VIDEO:
			video_overlay();
			break;
		case PAGE_SOUND_TEST:
			sound_test_options_overlay();
			break;
		case PAGE_CONTROLS:
			controls_overlay();
			break;
		case PAGE_CHOICE:
		case PAGE_STARTING:
			choice_overlay();
			break;
		default:
			break;
	}
}
