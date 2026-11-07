/**
 *    Name:        first_run.h
 *    Description: the first run: the game's assets out of the player's ROM set (first_run.c)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_SDL2_FIRST_RUN_H
#define TMNT_SDL2_FIRST_RUN_H

#include <stddef.h>

/* On macOS the folder the .app is in (with its separator), where 'base' (SDL_GetBasePath) is its Resources: where a
   player puts the ROM set or the assets beside the game, as beside an executable elsewhere (outrunners-port's); the
   folder the player put it in when macOS runs it from a copy of its own (App Translocation). 0 elsewhere */
int app_folder(const char *base, char *folder, size_t size);

/* The game's assets into 'folder' from the player's ROM set: tmnt.zip or a folder rom/ beside the executable
   ('base', with its separator: the window's letters are there too), in 'beside' (the folder the .app is in on
   macOS; NULL elsewhere) or in the folder of what the game keeps ('saves'); else one dropped on a window that asks
   for it, with 'title' on it. 1 when the assets are written, 0 when the window is closed first */
int first_run(const char *title, const char *base, const char *beside, const char *saves, const char *folder);

#endif
