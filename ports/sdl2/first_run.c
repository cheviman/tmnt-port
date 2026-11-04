/**
 *    Name:        first_run.c
 *    Description: the first run: the game's assets out of the player's ROM set, found beside the game or where it
 *                 keeps its files, or dropped on a window that asks for it
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As OutRunners' and SegaSonic's first run. The window has none of the game's letters (they are in the ROM): it
 *    writes with Kenney Mini (letters.c; font.bmp beside the executable). It says what it asks for, what it is
 *    doing and what failed. The ROM set is read once (runtime/src/extract.c): the assets come out of it into their
 *    folder, the same files tools/extract_assets.py writes, and the game never opens it again.
 */
#include "first_run.h"
#include "letters.h"

#include "game/extract.h"

#include <SDL.h>

#include <stdio.h>
#include <string.h>

#define ROM_SET_FILE    "tmnt.zip"
#define ROM_SET_FOLDER  "rom"
#define ROM_SET_PROBE   "963-x23.j17"    /* a file of the set by MAME's name: a folder of its files */
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   448
#define LINE_GAP        8
#define PROBLEM_LINES   4
#define TEXT_WIDTH      (WINDOW_WIDTH - 48)

/* 'text' in 'colour', in as many lines as it takes to fit the window (up to PROBLEM_LINES), broken after a space or
   a folder's separator; the y after it */
static int draw_lines(SDL_Renderer *renderer, const letters *font, const char *text, int y, SDL_Color colour)
{
	char line[256];

	for (int count = 0; count < PROBLEM_LINES && *text; count++)
	{
		size_t length = strlen(text) < sizeof(line) - 1 ? strlen(text) : sizeof(line) - 1, fits = 0;

		/* (the longest start of what is left that fits, ending at a break; a word too long, cut where it fits) */
		for (size_t end = 1; end <= length; end++)
		{
			memcpy(line, text, end);
			line[end] = 0;
			if (letters_width(font, line, 1) > TEXT_WIDTH)
				break;
			if (end == length || text[end - 1] == ' ' || text[end - 1] == '/' || text[end - 1] == '\\')
				fits = end;
		}
		if (fits == 0)
			for (fits = 1; fits < length; fits++)
			{
				memcpy(line, text, fits + 1);
				line[fits + 1] = 0;
				if (letters_width(font, line, 1) > TEXT_WIDTH)
					break;
			}
		memcpy(line, text, fits);
		line[fits] = 0;
		letters_draw(renderer, font, line, y, 1, colour, WINDOW_WIDTH);
		y += LETTERS_CELL + LINE_GAP / 2;
		text += fits;
	}
	return y;
}

static void draw_window(SDL_Renderer *renderer, const letters *font, const char *title, const char *problem, int working)
{
	const SDL_Color white = { 255, 255, 255, 255 }, green = { 120, 220, 90, 255 }, red = { 255, 110, 90, 255 };
	const int scale = letters_width(font, title, 3) <= TEXT_WIDTH ? 3 : 2;
	int y = 110;

	SDL_SetRenderDrawColor(renderer, 16, 40, 24, 255);
	SDL_RenderClear(renderer);
	letters_draw(renderer, font, title, y, scale, green, WINDOW_WIDTH);
	y += LETTERS_CELL * scale + LINE_GAP * 4;
	if (working)
		letters_draw(renderer, font, "Taking the graphics and sounds out of the ROM set...", y, 1, white, WINDOW_WIDTH);
	else
	{
		letters_draw(renderer, font, "Drop the arcade ROM set here", y, 2, white, WINDOW_WIDTH);
		y += LETTERS_CELL * 2 + LINE_GAP;
		letters_draw(renderer, font, "tmnt.zip: MAME's set, World 4 Players", y, 1, white, WINDOW_WIDTH);
		y += LETTERS_CELL + LINE_GAP;
		letters_draw(renderer, font, "The game takes its graphics and sounds from it, once.", y, 1, white, WINDOW_WIDTH);
		if (problem && problem[0])
			draw_lines(renderer, font, problem, y + LETTERS_CELL + LINE_GAP * 3, red);
	}
	SDL_RenderPresent(renderer);
}

static int exists(const char *path)
{
	SDL_RWops *probe = SDL_RWFromFile(path, "rb");

	if (!probe)
		return 0;
	SDL_RWclose(probe);
	return 1;
}

/* What went wrong with a ROM set, for the window: a file of the set missing says it is not the set */
static void say_problem(const char *failed, char *problem, size_t size)
{
	snprintf(problem, size, "%s%s", strncmp(failed, "no ", 3) == 0 ? "Not the set: " : "", failed);
	if (problem[0] >= 'a' && problem[0] <= 'z')
		problem[0] = (char)(problem[0] - 'a' + 'A');
}

/* The assets out of the ROM set in 'place' (a folder, with its separator): tmnt.zip there, or its folder rom/ with
   the set's files or its .zip. 1 when they came out; a set there they could not come out of leaves what went wrong
   in 'problem' */
static int extracted_from(const char *place, const char *folder, char *problem, size_t size)
{
	char source[1024], probe[1100];
	const char *failed;

	if (!place)
		return 0;
	snprintf(source, sizeof(source), "%s" ROM_SET_FILE, place);
	if (!exists(source))
	{
		snprintf(source, sizeof(source), "%s" ROM_SET_FOLDER, place);
		snprintf(probe, sizeof(probe), "%s/" ROM_SET_FILE, source);
		if (!exists(probe))
		{
			snprintf(probe, sizeof(probe), "%s/" ROM_SET_PROBE, source);
			if (!exists(probe))
				return 0;
		}
	}
	if ((failed = game_assets_extract(source, folder)) == NULL)
		return 1;
	say_problem(failed, problem, size);
	return 0;
}

int first_run(const char *title, const char *base, const char *saves, const char *folder)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	letters font;
	char problem[400] = "";
	int done = 0, running = 1;

	if (extracted_from(base, folder, problem, sizeof(problem))
		|| ((!base || !saves || strcmp(base, saves) != 0) && extracted_from(saves, folder, problem, sizeof(problem))))
		return 1;
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	renderer = window ? SDL_CreateRenderer(window, -1, 0) : NULL;
	if (!renderer || !letters_load(renderer, base, &font))
	{
		char message[700];

		/* (no window or no letters: the message alone) */
		if (renderer)
			SDL_DestroyRenderer(renderer);
		if (window)
			SDL_DestroyWindow(window);
		snprintf(message, sizeof(message), "The game takes its graphics and sounds from the arcade ROM set (tmnt.zip: MAME's "
			"set, World 4 Players), and there is none beside it; the window that asks for one does not open (no font.bmp "
			"beside the game, or no window).%s%s", problem[0] ? "\n\n" : "", problem);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, NULL);
		return 0;
	}
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	draw_window(renderer, &font, title, problem, 0);
	while (running && !done)
	{
		SDL_Event event;

		if (!SDL_WaitEvent(&event))
			break;
		if (event.type == SDL_QUIT)
			running = 0;
		else if (event.type == SDL_DROPFILE)
		{
			const char *failed;

			draw_window(renderer, &font, title, NULL, 1);
			failed = game_assets_extract(event.drop.file, folder);
			SDL_free(event.drop.file);
			if (!failed)
				done = 1;
			else
			{
				say_problem(failed, problem, sizeof(problem));
				draw_window(renderer, &font, title, problem, 0);
			}
		}
		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_EXPOSED)
			draw_window(renderer, &font, title, problem, 0);
	}
	letters_free(&font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return done;
}
