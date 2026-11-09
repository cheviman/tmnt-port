/**
 *    Name:        console.h
 *    Description: what the console version (TMNT) tells the ports, and what they hand it
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The console version is the arcade game's program with the functions console/replaced.txt lists in the
 *    console's own versions (console.c): a front end of its own before the title (the Konami presentation, the
 *    menus over the title's logo, OPTIONS over BEST 10 PLAYERS' picture, the turtle choice on the coin's screen),
 *    no coins (a player joins with START: the console puts the slot's coin and attack in), and Splinter Way's
 *    modes. The ports hand it each frame's controls and actions, and draw its overlay on each frame.
 */
#ifndef TMNT_CONSOLE_H
#define TMNT_CONSOLE_H

#include "tmnt/video.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONSOLE_PLAYERS 4

/* Sets the console up, after game_reset and before the program runs: the DIP switches of the settings on the
   board, and the overlay's letters (Kenney Mini, font.bmp in 'folder', with its separator; NULL: the current
   folder). false when the letters are not there (the menus are then written with none) */
bool console_attach(const char *folder);

/* The menus' actions, from any player's keys and game controller: the port hands those newly pressed at each
   frame, and the menus take them on the game's next frame */
enum
{
	ACTION_UP,
	ACTION_DOWN,
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_ACCEPT,      /* the attack button, Enter */
	ACTION_BACK,        /* the jump button, Escape */
	ACTION_START,
	ACTIONS
};
#define ACTION_BIT(action) (1u << (action))

void console_actions(uint32_t pressed);

/* Each frame, after the port has read its controls: each player's stick and buttons (tmnt/machine.h's
   TMNT_LEFT ... TMNT_ATTACK bits, set when pressed) and START (bit n: player n's, newly pressed). The console
   puts them on the board: a player's on the turtle's slot he plays, a START in a game on a slot of its own, with
   the slot's coin and attack (the board's inputs for the next frame). Out of a game, what a player newly presses
   is also a menu action (the stick's directions, attack ACCEPT, jump BACK) */
void console_controls(const uint8_t pressed[CONSOLE_PLAYERS], uint8_t starts);

/* The picture the console draws over: the frame the video drew, with WIDE's columns more at its sides ('left' and
   'right': tmnt_video_wide_frame's, 0 without WIDE), its rows 'stride' pixels apart, the board's column 0 of each at
   'pixels' + 'left'. The console's x are the board's, from -left to 319 + right */
typedef struct
{
	uint32_t *pixels;
	int stride;
	int left, right;
} console_picture;

/* The console's own picture over the frame the video drew (the presentation, the menus' letters): the port
   calls it on each frame it shows or writes */
void console_overlay(console_picture *picture);

/* True while the game runs out of sight (BOSS RUSH crossing an area to its boss): the port runs its frames as fast as
   it can, showing one in eight, without their sound */
bool console_fast_forward(void);

/* The LAN game (console/lan.c): the host's START has begun it, and the board is to be reset for it (the port:
   program_stop, then game_reset and console_lan_begin, and program_run again from the program's start) */
bool console_lan_reset_wanted(void);
void console_lan_begin(void);

/* True through a LAN game, from its reset: each frame's controls come from the network, in place of console_controls
   (console_lan_frame), and there is no pause (Escape or Back leaves the game: console_lan_quit) */
bool console_lan_playing(void);

/* Each frame of a LAN game: this device's player's controls (TMNT_* bits) and his START newly pressed. True when
   the next frame's every player's controls have come and are on the board; false while the game holds (the port
   shows its last picture again, and calls it again at its next frame) */
bool console_lan_frame(uint8_t pressed, bool start);
void console_lan_quit(void);

/* The LAN frame whose controls are on the board (for checks: tmnt_console_headless --lan-hashes) */
uint32_t console_lan_frame_number(void);

/* True once the menu's EXIT was picked: the port closes the game */
bool console_quit_requested(void);

/* The pause (as outrunners-port's): true while a game the menus started is played, when the port may stop it
   (Escape, a game controller's Back, or START of a player whose turtle plays: console_start_pauses; another's START
   brings him in); its picture over the frame it stopped at, the line
   'choice' chosen; QUIT GAME ends the game, back to the menu; RESTART ends it and starts the same game again, the
   same turtle for the same player */
enum
{
	CONSOLE_PAUSE_CONTINUE,
	CONSOLE_PAUSE_RESTART,
	CONSOLE_PAUSE_QUIT,
	CONSOLE_PAUSE_ITEMS
};

bool console_can_pause(void);
bool console_start_pauses(int player);
void console_pause_overlay(console_picture *picture, int choice);
void console_quit_game(void);
void console_restart_game(void);

/* The players' controls as they are held now, taken as the last ones (after the pause: what was held over it is
   no new press) */
void console_controls_held(const uint8_t pressed[CONSOLE_PLAYERS]);

/* The settings OPTIONS shows and changes: the port loads them (tmnt.ini) before console_attach, and applies and
   saves them when 'changed' is set */
enum
{
	PIXELS_SHARP,       /* the board's pixels, square */
	PIXELS_SMOOTH,      /* sharp bilinear: square pixels with soft edges */
	PIXELS_BILINEAR,    /* plain bilinear, softer */
	PIXELS_CRT,         /* scanlines, a phosphor mask and a little vignette over smooth pixels */
	PIXELS_KINDS
};

/* OPTIONS > CONTROLS: each player's controls, which the port binds to keys and game controller buttons (as
   outrunners-port's) */
enum
{
	CONTROL_UP,
	CONTROL_DOWN,
	CONTROL_LEFT,
	CONTROL_RIGHT,
	CONTROL_ATTACK,
	CONTROL_JUMP,
	CONTROL_START,
	CONTROLS
};
#define CONTROL_PLAYERS 4

/* CONTROLS on screen: where, in the picture's pixels, the pictures of 'control''s keys and button go (their top
   left; false when the page is not shown): the first key's there, the second's and the game controller button's
   that many pixels to the right */
bool console_controls_place(int control, int *x, int *y);
#define CONSOLE_CONTROLS_SECOND_X   36     /* (room for a key two tiles wide, Enter or Space) */
#define CONSOLE_CONTROLS_BUTTON_X   76

/* The player whose controls CONTROLS shows (0-3) */
int console_controls_player(void);

/* The control CONTROLS waits for a new key or button for, or -1: the port takes the next one pressed, binds it
   (Escape: leaves it as it was) and says so */
int console_controls_waiting(void);
void console_controls_bound(void);

/* DEFAULTS was picked on CONTROLS (true once): the port puts the shown player's keys and buttons back */
bool console_controls_defaults(void);

/* OPTIONS > VIDEO > WIDE: the picture wider than the board's 320 pixels, as outrunners-port's */
enum
{
	WIDE_OFF,           /* the board's 320 pixels */
	WIDE_16_9,          /* 48 more at each side */
	WIDE_20_9,          /* 88 more at each side: a phone's screen */
	WIDE_16_10,         /* 32 more at each side: the Steam Deck's screen (a player's request), a tablet's (after the
	                       others: tmnt.ini keeps WIDE by name, a LAN game sends its number; OPTIONS shows it second) */
	WIDE_KINDS
};

/* OPTIONS > VIDEO > PRESS START: the line under the box of a player who can come in, in a game (a player's request: it
   blinks all through the play) */
enum
{
	PRESS_START_BLINK,  /* the game's own */
	PRESS_START_ON,     /* still */
	PRESS_START_OFF,    /* none (the continue's countdown keeps it: its call to come back) */
	PRESS_START_KINDS
};

#define CONSOLE_REGIONS 4    /* the program's version byte (06009F): 0-3 */
#define CONSOLE_NAME_LENGTH 10
#define CONSOLE_DEFAULT_NAME "PLAYER"

typedef struct
{
	int window_scale;      /* 1-6: the window, in times the game's picture */
	bool full_screen;
	int pixels;            /* PIXELS_* */
	int wide;              /* WIDE_* */
	bool integer_scale;    /* OPTIONS > VIDEO > INTEGER SCALE: the picture a whole number of times its pixels (a
	                          player's request: square pixels all the same size), black around it */
	int press_start;       /* PRESS_START_* */
	int volume;            /* 0-10: 5 is the game's own */
	bool fps;              /* OPTIONS > FPS: the frames a second at the picture's top right, and in the window's title */
	bool vsync;            /* OPTIONS > VIDEO > VSYNC: each frame shown at the screen's refresh (a computer's) */
	int region;            /* the version byte: 0 NINJA (the set's own), 1 HERO, 2 and 3 (console/PLAN.md) */
	uint8_t dsw[3];        /* the DIP switches as the board has them (active low), tmnt/machine.h */
	char name[CONSOLE_NAME_LENGTH + 1];    /* LAN > NAME: this device's player's, as the rooms show it */
	bool changed;
} console_settings_t;

extern console_settings_t console_settings;

/* WIDE's columns more at the left and the right of the board's 320 now (0: none; in a game only), for the port to
   set on the video before each frame (tmnt_video_set_wide) and show */
void console_wide_sides(int *left, int *right);

/* The picture's first columns WIDE leaves black (an area's start: the board's first 6, which the arcade never shows),
   and its last (the Technodrome's lift in 20:9: its shaft's map past its band) */
int console_wide_hidden_left(void);
int console_wide_hidden_right(void);

/* The port calls it before each frame: WIDE's sides and the layers' maps' ends onto the video */
void console_wide_frame(void);

/* The names the settings file gives the pixels' kinds and WIDE's */
extern const char *const console_pixel_names[PIXELS_KINDS];
extern const char *const console_wide_names[WIDE_KINDS];
extern const char *const console_press_start_names[PRESS_START_KINDS];

/* For checks (tmnt_console_headless --arcade-start): the arcade's own attract mode and controls (a script's coins
   and players' buttons straight onto the board, player N's on slot N), so the arcade's scenarios run as they are
   in the console's build, its WIDE in their games */
void console_arcade_test(void);

/* For checks (tmnt_console_headless --wide-check): WIDE's sides' columns whose tiles are not their map's, described
   in 'text'; their count, 0 when none */
int console_wide_check(char *text, size_t size);

/* For scripted checks: where the console stands, a line of text (the page, the mode, the slots) */
void console_describe(char *text, size_t size);

#ifdef __cplusplus
}
#endif

#endif
