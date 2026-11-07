/**
 *    Name:        console_internal.h
 *    Description: what the console version's files share among themselves
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_CONSOLE_INTERNAL_H
#define TMNT_CONSOLE_INTERNAL_H

#include "console.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The game's states (game_state) and the attract mode's steps (game_step) the console shows */
#define GAME_STATE_ATTRACT      1u
#define GAME_STATE_GAME         2u
#define ATTRACT_COIN_SCREEN     1u    /* the turtle choice: the coin's screen, the four turtles' pictures */
#define ATTRACT_TITLE          3u    /* the menu's screen: the title's logo */
#define ATTRACT_BEST_PLAYERS    6u    /* OPTIONS: BEST 10 PLAYERS' picture */
#define ATTRACT_CONSOLE_STEP    9u    /* none of the attract's: run_attract runs no step (the presentation) */

/* The console's pages */
enum
{
	PAGE_PRESENTATION,     /* the Konami presentation, at power-on */
	PAGE_MENU,             /* the title's logo and the menu: ARCADE, SPLINTER WAY, LAN, OPTIONS, EXIT */
	PAGE_SPLINTER_WAY,     /* Splinter Way's modes: NORMAL, BOSS RUSH, RONIN, BACK */
	PAGE_LEVEL_SELECT,     /* the Konami code's level select (guideline 8; the menu's KONAMI CODE while it is tried):
	                          the area to start in, infinite lives */
	PAGE_OPTIONS,          /* over BEST 10 PLAYERS' picture: the DIP switches, the region, the volume, its pages */
	PAGE_VIDEO,            /* OPTIONS > VIDEO: the window's size, full screen, the pixels, WIDE */
	PAGE_SOUND_TEST,       /* OPTIONS > SOUND TEST: the songs by name, the effects, the voices */
	PAGE_CONTROLS,         /* OPTIONS > CONTROLS: each player's keys and game controller buttons */
	PAGE_LAN,              /* the menu's LAN (guideline 20, lan.c): HOST, JOIN, NAME, BACK, over the same picture */
	PAGE_LAN_NAME,         /* LAN > NAME: the name written letter by letter */
	PAGE_LAN_ROOMS,        /* LAN > JOIN: the rooms found */
	PAGE_LAN_ROOM,         /* the room hosted or joined: its devices, the host's START */
	PAGE_CHOICE,           /* the coin's screen: the turtle chosen */
	PAGE_STARTING,         /* the chosen turtle's coin and attack going in */
	PAGE_GAME              /* the game's own, from its start to the attract mode */
};

/* OPTIONS and its pages, over the same picture */
#define OPTIONS_PAGE(page) ((page) >= PAGE_OPTIONS && (page) <= PAGE_LAN_ROOM)    /* (and LAN's, over it too) */

/* The games the menus start */
enum
{
	MODE_ARCADE,           /* the arcade game at home: the DIP switches' lives, continues while START is pressed */
	MODE_SPLINTER_WAY      /* Splinter Way's NORMAL: 5 lives and 3 continues */
};

#define SPLINTER_WAY_LIVES      0x05u    /* BCD, as lives_per_credit */
#define SPLINTER_WAY_CONTINUES  3

typedef struct
{
	uint32_t pressed;          /* the actions pressed since the game's last frame */
	int page;
	int choice;                /* the line chosen on the page */
	int mode;                  /* MODE_*: the game being started or played */
	int turtle;                /* the turtle chosen: 0 Leonardo, 1 Michelangelo, 2 Donatello, 3 Raphael */
	int chooser;               /* the player who chose it */
	bool powered_on;           /* the presentation is over */
	bool quit;
	bool restarting;           /* the pause's RESTART: the same game again, its turtle picked at once */
	int page_frames;           /* the game's frames on the page */
	int last_presser;          /* the last player who pressed a button or START (the one who confirms) */
	uint32_t player_pressed[CONSOLE_PLAYERS];    /* each player's actions since the game's last frame, out of a
	                                                game: his stick, buttons and START (the port's fixed keys go
	                                                with player 1's) */
	bool level_select;         /* the game being started or played is LEVEL SELECT's: */
	int level_area;            /* its area (0-9, the game's), */
	bool infinite_lives;       /* its lives kept up, */
	bool original_turtles;     /* every turtle in a red bandana (guideline 18), */
	bool same_turtle;          /* every player the turtle the first one picks (guideline 8), */
	bool ronin;                /* Splinter Way's RONIN (guideline 7): one player, the enemies of four (modes.c) */
	bool boss_rush;            /* Splinter Way's BOSS RUSH (guideline 6): the bosses one after another (modes.c), */
	bool rush_crossing;        /* and an area being crossed to its boss */
	bool level_pending;        /* and its area not yet set in the game started */
	int choice_turtle[CONSOLE_PLAYERS];    /* the turtle choice: each player's turtle there (-1: not in it), */
	bool choice_picked[CONSOLE_PLAYERS];   /* and whether he picked it (kept for the pause's RESTART) */
	int turtle_change[CONSOLE_PLAYERS];    /* a continue's turtle change: the turtle each player has on it, plus 1
	                                          (0: none; console.c's start_pressed, turtle_change_frame) */
} console_state;

extern console_state console;

/* "Everyone the same turtle" (guideline 8): the turtle every player plays in the game (0 Leonardo ... 3 Raphael), each
   on his own slot, or -1 (each his own); a turtle object's kind stays its player's, and the turtle's own tables are
   taken by TURTLE_KIND (same_turtle.c, wide_turtles.c) */
extern int console_same_turtle;
#define TURTLE_KIND(object) (console_same_turtle >= 0 ? (uint8_t)console_same_turtle : OBJECT(object)->kind)

/* Each player's slot (0-3, the cabinet's: its turtle), -1 for none; and the coins put in on each slot this game */
extern int console_slot_of[CONSOLE_PLAYERS];
extern int console_coins_put[CONSOLE_PLAYERS];

/* A player starts on 'slot' (with a coin when he has no lives): the coin's and attack's frames on the board */
void console_join(int slot);

/* The DIP switches of the settings onto the board and into the copies the boot made of them (reset: their
   reading in the boot's own place); and the lives a coin gives by the mode */
void console_apply_dip_switches(void);
void console_set_mode_lives(int mode);

/* The presentation (presentation.c): its frame in the attract mode, with the actions pressed: true while it
   lasts; its picture over the frame */
bool presentation_frame(uint32_t pressed);
void presentation_overlay(console_picture *frame);

/* The menus (menu.c): their frame in the attract mode (the page's own, then the attract's step), with the actions
   pressed (any player's) and each player's own; their picture */
void menu_frame(uint32_t pressed, const uint32_t player_pressed[CONSOLE_PLAYERS]);
void menu_overlay(console_picture *frame);
void turtle_change_overlay(console_picture *frame);    /* (a continue's turtle change, over the game: menu.c) */

/* Splinter Way's modes (modes.c), at each frame before the game's: RONIN's other boxes kept clear; BOSS RUSH's
   crossing (its turtles' controls), its spawn lists' Foot soldiers passed (spawn_enemies), its picture */
void ronin_frame(void);
void boss_rush_start(void);
void boss_rush_frame(void);
bool boss_rush_crossing(void);
void boss_rush_spawns(void);
void boss_rush_overlay(console_picture *frame);

/* WIDE's room for the turtles (wide.c): its sides, as far as the scroll's segment lets the picture go */
void console_wide_turtle_room(int *left, int *right);

/* WIDE on the menus' screens (wide.c), at each of their frames: OPTIONS' background past the board's 320 */
void console_wide_menus(void);

/* WIDE in an area's exit's picture (wide.c): its sides painted with its background's colour */
void wide_picture_sides(console_picture *frame);

/* The LAN game's rooms (lan.c), for the menu's LAN pages: HOST, JOIN (the rooms found, one joined), the room's list
   and the host's START (two devices or more), what went wrong last (until a line is moved to), and the datagrams at
   each frame of those pages */
bool console_lan_host(void);
void console_lan_search(void);
void console_lan_leave(void);
void console_lan_name_changed(void);
bool console_lan_searching(void);
bool console_lan_in_room(void);
bool console_lan_joining(void);
bool console_lan_hosting(void);
int console_lan_room_count(void);
void console_lan_room_text(int index, char *text, size_t size);
bool console_lan_join(int index);
int console_lan_member_count(void);
void console_lan_member_text(int index, char *text, size_t size);
bool console_lan_can_start(void);
void console_lan_start(void);
const char *console_lan_problem(void);
void console_lan_problem_seen(void);
void console_lan_update(void);
void console_lan_game_over(void);
const char *console_lan_waiting(void);    /* (a LAN game held: the device it waits for) */

/* The console as at power-on (console.c and each file's own state: menu.c, wide.c, wide_turtles.c, modes.c, draw.c),
   for a LAN game's start on every device alike; with 'lan_game' Splinter Way's turtle choice open (menu.c) */
void console_power_on(bool lan_game);
void menu_power_on(bool lan_game);
void wide_power_on(void);
void wide_turtles_power_on(void);
void modes_power_on(void);
void draw_power_on(void);

/* The menu's LAN page again: console_back_to_lan_page as a LAN game ends for this device (console.c: after the game's
   game over), console_open_lan_page the page itself (menu.c) */
void console_back_to_lan_page(void);
void console_open_lan_page(void);

/* ---- drawing over the frame (draw.c) ---- */

/* The picture's first column and the one past its last, in the board's x (WIDE's sides past 0 and 319); the
   window's middle, where the centred lines go: the picture's (WIDE), or the board's without its first LEFT_STRIPE
   columns, which the window leaves out */
#define PICTURE_FIRST(picture)  (-(picture)->left)
#define PICTURE_END(picture)    (TMNT_VIDEO_WIDTH + (picture)->right)
int draw_centred_x(const console_picture *frame, int width);

/* Kenney Mini (font.bmp): loaded from 'folder'; its line's height */
bool draw_load_font(const char *folder);
#define FONT_HEIGHT 10         /* a capital's: the lit rows of the font's cells */

/* A line of text at x, y (its top), 'scale' times, in 0x00RRGGBB with a shadow under it; its width */
void draw_text(console_picture *frame, int x, int y, int scale, uint32_t colour, const char *text);
void draw_text_centred(console_picture *frame, int y, int scale, uint32_t colour, const char *text);
int draw_text_width(const char *text, int scale);

/* The game's own letters (the fixed layer's 8 x 8 font, as the game's texts write them: capitals, digits, , - . ? !
   and the apostrophe), 'scale' times, at x, y (the top left): in the game's colours for its texts (white over light
   blue: GAME_LETTERS_OWN) or in one colour, their shadow darker; their width */
#define LEFT_STRIPE             6      /* the columns the K052109's layers A and B show 6 pixels further */
#define GAME_LETTER             8
#define GAME_LETTERS_OWN        0xFFFFFFFFu
#define GAME_LETTERS_BANK       0      /* the fixed layer's palette the texts show with (graphics_subjects.txt) */
#define GAME_LETTERS_SHADOW_PEN 11     /* the letters' shadow under them */
void draw_game_letters(console_picture *frame, int x, int y, int scale, uint32_t colour, const char *text);
void draw_game_letters_centred(console_picture *frame, int y, int scale, uint32_t colour, const char *text);
int draw_game_letters_width(const char *text, int scale);

/* The signature's small letters (5 pixels high, the console's own: "@cheviman"), centred, in the game's colours */
void draw_small_letters_centred(console_picture *frame, int y, const char *text);
int draw_small_letters_width(const char *text);

/* A rectangle darkened ('keep': 0-256 of its light kept), or filled */
void draw_shade(console_picture *frame, int x, int y, int width, int height, int keep);
void draw_fill(console_picture *frame, int x, int y, int width, int height, uint32_t colour);

/* A text of the game's (game_texts' 'number') from the fixed layer's character set, 'scale' times, its top
   left cell at x, y, in the palette's 'bank' (its colours, palette RAM); left of 'lit_to' as it is, right of
   it darkened to 'dark' (0-256). Its size in pixels, unscaled */
void draw_game_text(console_picture *frame, uint32_t text_number, int x, int y, int scale, int bank, int lit_to,
	int dark);
void game_text_size(uint32_t text_number, int *width, int *height);

/* The sprites the frame shows (the K051960's list as the video drew it: console_keep_sprites at each frame)
   drawn again over what the console drew */
void draw_board_sprites(console_picture *frame);
void console_keep_sprites(void);

#endif
