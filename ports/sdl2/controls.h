/**
 *    Name:        controls.h
 *    Description: the keys and game controller buttons of each player's controls (TMNT's OPTIONS > CONTROLS), and
 *                 the file that keeps them (controls.ini)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_PORT_CONTROLS_H
#define TMNT_PORT_CONTROLS_H

#include "console.h"

#include <SDL.h>
#include <stdbool.h>

#define CONTROL_KEYS     2
#define CONTROL_BUTTONS  3

/* The triggers as buttons of the bindings, past SDL's own (its game controller has them as axes) */
#define CONTROL_LEFT_TRIGGER   ((SDL_GameControllerButton)SDL_CONTROLLER_BUTTON_MAX)
#define CONTROL_RIGHT_TRIGGER  ((SDL_GameControllerButton)(SDL_CONTROLLER_BUTTON_MAX + 1))

/* A trigger holds its control from past its rest's wobble (of 32767) */
#define TRIGGER_PRESSED  8192

typedef struct
{
	const char *name;                                   /* as the settings file says it */
	SDL_Scancode keys[CONTROL_KEYS];                    /* SDL_SCANCODE_UNKNOWN: none */
	SDL_GameControllerButton buttons[CONTROL_BUTTONS];  /* SDL_CONTROLLER_BUTTON_INVALID: none; or a trigger */
} control_binding;

/* Each player's: their keys, and the buttons of their own game controller (the first one plugged in is player 1's,
   the second player 2's...) */
extern control_binding control_bindings[CONTROL_PLAYERS][CONTROLS];

/* controls.ini at 'path': read (the defaults where it says nothing), or written when there is none */
void controls_open(const char *path);
void controls_save(void);
void controls_defaults(int player);    /* (and saved) */

/* The key or button pressed for a player's control on the CONTROLS page: its first one now, and no longer any other
   control's of the player's (and saved) */
void controls_bind_key(int player, int control, SDL_Scancode key);
void controls_bind_button(int player, int control, SDL_GameControllerButton button);

/* True while one of the player's keys or their game controller's buttons (or triggers) for 'control' is held */
bool controls_holds(int player, SDL_GameController *pad, int control);

#endif
