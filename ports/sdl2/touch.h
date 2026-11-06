/**
 *    Name:        touch.h
 *    Description: the touch screen's controls (Android): a stick where the left thumb lands, ATTACK and JUMP at the
 *                 right, START (TMNT) or COIN (TMNT Arcade) over them, drawn over the picture, and the controls the
 *                 fingers on them hold: player 1's
 *    Version:     0.1.0
 *    Created:     06-11-2026
 *    Modified:    06-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_PORT_TOUCH_H
#define TMNT_PORT_TOUCH_H

#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

/* The controls' letters, from font.bmp in the folder 'base' (with its separator; Android's: "", the APK's assets);
   shown from the start where there is a touch screen */
void touch_open(SDL_Renderer *renderer, const char *base);

/* An event: a finger on the controls; a game controller's button or stick, or a key, hides them until a finger
   touches the screen again */
void touch_event(const SDL_Event *event);

/* The board's controls the fingers hold (TMNT_LEFT ... TMNT_JUMP), and whether one holds START (COIN in TMNT
   Arcade) */
uint8_t touch_controls(void);
bool touch_start(void);

/* The controls over a screen of 'width' x 'height', before it is presented (nothing while they are hidden) */
void touch_draw(SDL_Renderer *renderer, int width, int height);

#endif
