/**
 *    Name:        main.c
 *    Description: the game in a window: SDL2 shows its screen, reads the controls and keeps the pace, around
 *                 the same game and board the headless runs check
 *    Version:     0.1.3
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: "TMNT Arcade" [--script FILE [--fast]] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE]
 *                         [--service] [--assets FOLDER or PACK] [--write-pack FILE]
 *           TMNT [--assets FOLDER or PACK]
 *
 *    TMNT Arcade, the arcade game as it is (runtime/src/game.c and the program): this file only wraps it. When
 *    each frame is done the port puts it in the window, queues its sound, reads the keyboard and the game
 *    controllers into the board's inputs for the next one, and waits for the frame's time (the board's 59.185 a
 *    second) to pass.
 *
 *    TMNT, the console version (TMNT_CONSOLE, console/): the same window around the console's game, which takes
 *    the players' controls and its menus' actions (console/console.h) and draws its own picture over each frame;
 *    its settings (OPTIONS: the DIP switches, the region, the volume, the pixels) in tmnt.ini, in the folder of what
 *    the game keeps.
 *
 *    The game's data is its assets (game/assets.h), the files tools/extract_assets.py writes from the
 *    player's ROM set: --assets (a folder, or a pack: tmnt.assets), or the pack or the folder 'assets' beside the
 *    executable, in the folder of what the game keeps (beside the executable on Windows, the user's own folder for
 *    it elsewhere: saves_folder), or in the current folder. A folder the first run made for another build of the
 *    game (its version.txt) is taken out again. Where there is none, the first run takes them out of the ROM set
 *    itself into the folder of what the game keeps (first_run.c, runtime/src/extract.c): tmnt.zip or rom/ beside
 *    the game or in that folder, or a set dropped on its window. --write-pack writes the pack of the assets
 *    loaded (for the devices, which never take them out of a ROM set) and stops.
 *
 *    --script plays a control script (runtime/include/tmnt/script.h) instead of the keyboard, to its end;
 *    --fast runs it without waiting for the frames' time. --hashes, --video-hashes and --sound-hashes write
 *    for each frame the hashes ports/headless and kref write, so a run in the window compares with the reference frame by
 *    frame. --service turns the test mode's DIP switch on.
 *
 *    Controls (the cabinet has four players, each with a coin slot and no start: a coin and the attack
 *    button bring a turtle in; player 1 is Leonardo, 2 Michelangelo, 3 Donatello, 4 Raphael):
 *      arrows                         move (player 1)
 *      Z or left Ctrl                 attack         X or left Alt     jump
 *      5, 6, 7, 8                     coins 1-4      9                 service 1
 *      Escape                         quit           F11 or Alt+Enter  full screen
 *      F3                             smooth or sharp pixels
 *    Game controllers, players 1 to 4 in the order they are plugged: the pad or the left stick moves, A
 *    attacks, B jumps, Back is the coin.
 *    TMNT: no coins; Enter (player 1) and a controller's Start are START, which brings a player in; in the
 *    menus the arrows move, attack (Z, A) picks, jump (X, B) or Escape goes back; F3 the next kind of pixels.
 */
#include "first_run.h"

#include "game/assets.h"
#include "game/extract.h"
#include "game/game.h"
#include "tmnt/machine.h"
#include "tmnt/program.h"
#include "tmnt/script.h"
#include "tmnt/sound_board.h"
#include "tmnt/video.h"
#ifdef TMNT_CONSOLE
#include "console.h"
#include "controls.h"
#endif

#include <SDL.h>

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TMNT_CONSOLE
#define TITLE               "TMNT"
#define SETTINGS_FILE       "tmnt.ini"
#define CONTROLS_FILE       "controls.ini"    /* (OPTIONS > CONTROLS: controls.c) */
#define SHOWN_LEFT          6          /* the picture's first columns the window leaves out (present) */
#else
#define TITLE               "TMNT Arcade"
#define SHOWN_LEFT          0
#endif
#define SHOWN_WIDTH         (TMNT_VIDEO_WIDTH - SHOWN_LEFT)
#define PLAYERS             4
#define WINDOW_SCALE        3
#define STICK_DEAD_ZONE     8000
#define QUEUE_FRAMES_MAX    6          /* sound queued beyond this many frames is dropped */
/* the board's refresh: 6 MHz pixels, 384 a line, 264 lines */
#define FRAME_RATE_NUMERATOR    6000000u
#define FRAME_RATE_DENOMINATOR  (384u * 264u)

#define USAGE "Usage: \"" TITLE "\" [--script FILE [--fast]] [--hashes FILE] [--video-hashes FILE]" \
	" [--sound-hashes FILE] [--service] [--assets FOLDER or PACK] [--write-pack FILE]"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_AudioDeviceID audio;
static SDL_GameController *controllers[PLAYERS];
#ifndef TMNT_CONSOLE
static bool smooth;                   /* F3: the picture as SDL softens it */
#endif
static uint64_t next_frame;          /* performance counter value the next frame waits for */
static tmnt_script *script;
static uint64_t script_frames;
static bool fast;
static FILE *hashes;
static FILE *video_hashes;
static FILE *sound_hashes;

/* ---- the pixels: sharp, smooth ("sharp bilinear": a whole number of times with sharp pixels into 'scaled',
   then that softened the rest of the way, so only the pixels' edges blend), plain bilinear, or CRT (a
   multiplying overlay on smooth pixels, the size the picture is shown at), as outrunners-port's ---- */

#define CRT_SCANLINE_DARK  0.45    /* how much the scanlines' edges darken (their middles keep it all) */
#define CRT_MASK           0.80    /* the other two channels in a phosphor column */
#define CRT_VIGNETTE       0.18    /* how much the corners darken */

static SDL_Texture *scaled;
static int scaled_times, scaled_width;
static SDL_Texture *crt_overlay;
static int crt_width, crt_height;

/* For each pixel of the shown picture: its game row's scanline (bright in the middle, dark at the edges),
   the column's phosphor (red, green, blue in turn), and the vignette toward the edges */
static void make_crt_overlay(int width, int height)
{
	uint32_t *pixels;
	int pitch = 0;

	if (crt_overlay && crt_width == width && crt_height == height)
		return;
	if (crt_overlay)
		SDL_DestroyTexture(crt_overlay);
	crt_overlay = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	crt_width = width;
	crt_height = height;
	if (!crt_overlay || SDL_LockTexture(crt_overlay, NULL, (void **)&pixels, &pitch) != 0)
		return;
	for (int y = 0; y < height; y++)
	{
		const double row = (y + 0.5) * TMNT_VIDEO_HEIGHT / height;
		const double scan = 1.0 - CRT_SCANLINE_DARK * (1.0 - sin(3.14159265358979 * (row - floor(row))));
		const double dy = (2.0 * y / height - 1.0);

		for (int x = 0; x < width; x++)
		{
			const double dx = (2.0 * x / width - 1.0);
			const double light = scan * (1.0 - CRT_VIGNETTE * (dx * dx * dy * dy));
			const int phosphor = x % 3;
			const int red = (int)(255.0 * light * (phosphor == 0 ? 1.0 : CRT_MASK));
			const int green = (int)(255.0 * light * (phosphor == 1 ? 1.0 : CRT_MASK));
			const int blue = (int)(255.0 * light * (phosphor == 2 ? 1.0 : CRT_MASK));

			pixels[y * (pitch / 4) + x] = 0xFF000000u | (uint32_t)red << 16 | (uint32_t)green << 8 | (uint32_t)blue;
		}
	}
	SDL_UnlockTexture(crt_overlay);
	SDL_SetTextureBlendMode(crt_overlay, SDL_BLENDMODE_MOD);
}

/* The kind of pixels shown (0 sharp, 1 smooth, 2 bilinear, 3 CRT): the console's OPTIONS > FILTER; the arcade's
   F3, sharp or plain bilinear (the picture as SDL scales it) */
static int pixels_kind(void)
{
#ifdef TMNT_CONSOLE
	return console_settings.pixels;
#else
	return smooth ? 2 : 0;
#endif
}

#ifdef TMNT_CONSOLE
/* ---- OPTIONS > FPS: the frames shown a second, at the picture's top right and in the window's title, as
   outrunners-port's ---- */

#define FPS_GLYPHS "0123456789 FPS"

/* FPS_GLYPHS' letters, 3 x 5 pixels of the game's: a row each, its high bit the left */
static const uint8_t fps_glyphs[][5] = {
	{ 7, 5, 5, 5, 7 }, { 2, 6, 2, 2, 7 }, { 7, 1, 7, 4, 7 }, { 7, 1, 7, 1, 7 }, { 5, 5, 7, 1, 1 },
	{ 7, 4, 7, 1, 7 }, { 7, 4, 7, 5, 7 }, { 7, 1, 2, 2, 2 }, { 7, 5, 7, 5, 7 }, { 7, 5, 7, 1, 7 },
	{ 0, 0, 0, 0, 0 }, { 7, 4, 6, 4, 4 }, { 6, 5, 6, 4, 4 }, { 3, 4, 2, 1, 6 }
};
static int fps_counted = -1;    /* the last half second's frames a second (-1: none counted yet) */

/* A frame shown: the frames a second, each half second; the window says them in its title */
static void count_fps(void)
{
	static uint64_t start;
	static int frames;
	static bool titled;
	const uint64_t now = SDL_GetPerformanceCounter(), frequency = SDL_GetPerformanceFrequency();

	if (!start)
		start = now;
	if (++frames < 2 || now - start < frequency / 2)
		return;
	fps_counted = (int)lround(frames * (double)frequency / (double)(now - start));
	start = now;
	frames = 0;
	if (console_settings.fps || titled)
	{
		char title[48];

		snprintf(title, sizeof(title), console_settings.fps ? TITLE " - %d FPS" : TITLE, fps_counted);
		SDL_SetWindowTitle(window, title);
		titled = console_settings.fps;
	}
}

/* The frames a second at the top right of the picture 'shown', its letters the game's pixels, with a shadow */
static void draw_fps(const SDL_Rect *shown)
{
	SDL_Rect cells[2][14 * 15];
	int count = 0;
	char text[16];
	const int pixel = SDL_max(1, shown->h / TMNT_VIDEO_HEIGHT);
	int x;

	if (!console_settings.fps || fps_counted < 0)
		return;
	snprintf(text, sizeof(text), "%d FPS", SDL_min(fps_counted, 999));
	x = shown->x + shown->w - (4 * (int)strlen(text) + 3) * pixel;
	for (const char *letter = text; *letter; letter++, x += 4 * pixel)
	{
		const uint8_t *rows = fps_glyphs[strchr(FPS_GLYPHS, *letter) - FPS_GLYPHS];

		for (int row = 0; row < 5; row++)
			for (int column = 0; column < 3; column++)
				if (rows[row] & (4 >> column))
				{
					cells[1][count] = (SDL_Rect){ x + column * pixel, shown->y + (4 + row) * pixel, pixel, pixel };
					cells[0][count] = cells[1][count];
					cells[0][count].x += pixel;
					cells[0][count].y += pixel;
					count++;
				}
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRects(renderer, cells[0], count);
	SDL_SetRenderDrawColor(renderer, 255, 224, 0, 255);
	SDL_RenderFillRects(renderer, cells[1], count);
}
/* ---- the CONTROLS page's keys and buttons: Kenney's Input Prompts Pixel (CC0), 16 x 16 tiles, as
   outrunners-port's ---- */

typedef struct
{
	int what;      /* an SDL_Scancode or an SDL_GameControllerButton */
	uint8_t row, column, tiles;    /* where in the sheet, and how many tiles wide */
} prompt;

static const prompt key_prompts[] = {
	{ SDL_SCANCODE_ESCAPE, 0, 17, 1 }, { SDL_SCANCODE_F1, 0, 18, 1 }, { SDL_SCANCODE_F2, 0, 19, 1 },
	{ SDL_SCANCODE_F3, 0, 20, 1 }, { SDL_SCANCODE_F4, 0, 21, 1 }, { SDL_SCANCODE_F5, 0, 22, 1 },
	{ SDL_SCANCODE_F6, 0, 23, 1 }, { SDL_SCANCODE_F7, 0, 24, 1 }, { SDL_SCANCODE_F8, 0, 25, 1 },
	{ SDL_SCANCODE_F9, 0, 26, 1 }, { SDL_SCANCODE_F10, 0, 27, 1 }, { SDL_SCANCODE_F11, 0, 28, 1 },
	{ SDL_SCANCODE_F12, 0, 29, 1 },
	{ SDL_SCANCODE_1, 1, 17, 1 }, { SDL_SCANCODE_2, 1, 18, 1 }, { SDL_SCANCODE_3, 1, 19, 1 },
	{ SDL_SCANCODE_4, 1, 20, 1 }, { SDL_SCANCODE_5, 1, 21, 1 }, { SDL_SCANCODE_6, 1, 22, 1 },
	{ SDL_SCANCODE_7, 1, 23, 1 }, { SDL_SCANCODE_8, 1, 24, 1 }, { SDL_SCANCODE_9, 1, 25, 1 },
	{ SDL_SCANCODE_0, 1, 26, 1 }, { SDL_SCANCODE_MINUS, 1, 27, 1 }, { SDL_SCANCODE_EQUALS, 1, 29, 1 },
	{ SDL_SCANCODE_BACKSPACE, 1, 32, 2 },
	{ SDL_SCANCODE_Q, 2, 17, 1 }, { SDL_SCANCODE_W, 2, 18, 1 }, { SDL_SCANCODE_E, 2, 19, 1 },
	{ SDL_SCANCODE_R, 2, 20, 1 }, { SDL_SCANCODE_T, 2, 21, 1 }, { SDL_SCANCODE_Y, 2, 22, 1 },
	{ SDL_SCANCODE_U, 2, 23, 1 }, { SDL_SCANCODE_I, 2, 24, 1 }, { SDL_SCANCODE_O, 2, 25, 1 },
	{ SDL_SCANCODE_P, 2, 26, 1 }, { SDL_SCANCODE_LEFTBRACKET, 2, 27, 1 }, { SDL_SCANCODE_RIGHTBRACKET, 2, 28, 1 },
	{ SDL_SCANCODE_BACKSLASH, 2, 31, 1 },
	{ SDL_SCANCODE_A, 3, 18, 1 }, { SDL_SCANCODE_S, 3, 19, 1 }, { SDL_SCANCODE_D, 3, 20, 1 },
	{ SDL_SCANCODE_F, 3, 21, 1 }, { SDL_SCANCODE_G, 3, 22, 1 }, { SDL_SCANCODE_H, 3, 23, 1 },
	{ SDL_SCANCODE_J, 3, 24, 1 }, { SDL_SCANCODE_K, 3, 25, 1 }, { SDL_SCANCODE_L, 3, 26, 1 },
	{ SDL_SCANCODE_APOSTROPHE, 3, 27, 1 }, { SDL_SCANCODE_SEMICOLON, 3, 30, 1 },
	{ SDL_SCANCODE_RETURN, 3, 32, 2 }, { SDL_SCANCODE_KP_ENTER, 3, 32, 2 },
	{ SDL_SCANCODE_Z, 4, 19, 1 }, { SDL_SCANCODE_X, 4, 20, 1 }, { SDL_SCANCODE_C, 4, 21, 1 },
	{ SDL_SCANCODE_V, 4, 22, 1 }, { SDL_SCANCODE_B, 4, 23, 1 }, { SDL_SCANCODE_N, 4, 24, 1 },
	{ SDL_SCANCODE_M, 4, 25, 1 }, { SDL_SCANCODE_COMMA, 4, 26, 1 }, { SDL_SCANCODE_PERIOD, 4, 27, 1 },
	{ SDL_SCANCODE_SLASH, 4, 29, 1 },
	{ SDL_SCANCODE_UP, 4, 30, 1 }, { SDL_SCANCODE_RIGHT, 4, 31, 1 }, { SDL_SCANCODE_DOWN, 4, 32, 1 },
	{ SDL_SCANCODE_LEFT, 4, 33, 1 },
	{ SDL_SCANCODE_LALT, 5, 17, 2 }, { SDL_SCANCODE_RALT, 5, 17, 2 }, { SDL_SCANCODE_TAB, 5, 19, 2 },
	{ SDL_SCANCODE_DELETE, 5, 21, 2 }, { SDL_SCANCODE_END, 5, 23, 2 },
	{ SDL_SCANCODE_LCTRL, 6, 17, 2 }, { SDL_SCANCODE_RCTRL, 6, 17, 2 }, { SDL_SCANCODE_CAPSLOCK, 6, 19, 2 },
	{ SDL_SCANCODE_HOME, 6, 21, 2 }, { SDL_SCANCODE_PAGEUP, 6, 23, 2 }, { SDL_SCANCODE_PAGEDOWN, 6, 25, 2 },
	{ SDL_SCANCODE_SPACE, 6, 32, 2 },
	{ SDL_SCANCODE_LSHIFT, 7, 17, 2 }, { SDL_SCANCODE_RSHIFT, 7, 17, 2 }, { SDL_SCANCODE_INSERT, 7, 19, 2 },
	{ SDL_SCANCODE_PAUSE, 7, 25, 2 },
};

static const prompt button_prompts[] = {
	{ SDL_CONTROLLER_BUTTON_A, 0, 4, 1 }, { SDL_CONTROLLER_BUTTON_B, 0, 5, 1 },
	{ SDL_CONTROLLER_BUTTON_X, 0, 6, 1 }, { SDL_CONTROLLER_BUTTON_Y, 0, 7, 1 },
	{ SDL_CONTROLLER_BUTTON_DPAD_UP, 1, 1, 1 }, { SDL_CONTROLLER_BUTTON_DPAD_RIGHT, 1, 2, 1 },
	{ SDL_CONTROLLER_BUTTON_DPAD_DOWN, 1, 3, 1 }, { SDL_CONTROLLER_BUTTON_DPAD_LEFT, 1, 4, 1 },
	{ SDL_CONTROLLER_BUTTON_BACK, 18, 4, 1 }, { SDL_CONTROLLER_BUTTON_START, 18, 5, 1 },
	{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER, 18, 7, 1 }, { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, 18, 8, 1 },
	{ CONTROL_LEFT_TRIGGER, 18, 9, 1 }, { CONTROL_RIGHT_TRIGGER, 18, 10, 1 },
	{ SDL_CONTROLLER_BUTTON_LEFTSTICK, 12, 16, 1 }, { SDL_CONTROLLER_BUTTON_RIGHTSTICK, 14, 16, 1 },
	{ SDL_CONTROLLER_BUTTON_GUIDE, 18, 2, 1 }, { SDL_CONTROLLER_BUTTON_MISC1, 18, 6, 1 },
};

#define PROMPT_SIZE  16
#define PROMPT_CLEAR 0xFFFF00FFu    /* magenta: the sheet's see-through */

static SDL_Surface *prompt_sheet;    /* input_prompts.bmp beside the executable, as ARGB */

static void load_prompts(const char *base)
{
	char path[1100];
	SDL_Surface *loaded;

	snprintf(path, sizeof(path), "%sinput_prompts.bmp", base ? base : "");
	loaded = SDL_LoadBMP(path);
	if (!loaded)
		return;    /* (CONTROLS then shows the names alone) */
	prompt_sheet = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(loaded);
}

static const prompt *find_prompt(const prompt *list, size_t count, int what)
{
	for (size_t index = 0; index < count; index++)
		if (list[index].what == what)
			return &list[index];
	return NULL;
}

/* One prompt into the picture, its top left at (x, y) (the board's x: WIDE's sides past it) */
static void draw_prompt(console_picture *picture, const prompt *icon, int x, int y)
{
	const uint32_t *sheet = (const uint32_t *)prompt_sheet->pixels;
	const int pitch = prompt_sheet->pitch / 4;

	for (int row = 0; row < PROMPT_SIZE; row++)
		for (int column = 0; column < PROMPT_SIZE * icon->tiles; column++)
		{
			const uint32_t colour = sheet[(icon->row * PROMPT_SIZE + row) * pitch + icon->column * PROMPT_SIZE + column];

			if ((colour | 0xFF000000u) == PROMPT_CLEAR || y + row < 0 || y + row >= TMNT_VIDEO_HEIGHT
				|| x + column < -picture->left || x + column >= TMNT_VIDEO_WIDTH + picture->right)
				continue;
			picture->pixels[(y + row) * picture->stride + picture->left + x + column] = colour & 0xFFFFFFu;
		}
}

/* On CONTROLS, each of the shown player's controls' keys and first button under its name */
static void draw_controls(console_picture *picture)
{
	const control_binding *bindings = control_bindings[console_controls_player()];

	if (!prompt_sheet)
		return;
	for (int control = 0; control < CONTROLS; control++)
	{
		const prompt *icon, *first = NULL;
		int shown = 0, x, y;

		if (!console_controls_place(control, &x, &y))
			return;
		for (int key = 0; key < CONTROL_KEYS && shown < 2; key++)
		{
			icon = find_prompt(key_prompts, SDL_arraysize(key_prompts), bindings[control].keys[key]);
			if (!icon || (first && first->row == icon->row && first->column == icon->column))
				continue;    /* (no picture, or the first key's: Enter and the keypad's) */
			draw_prompt(picture, icon, x + (shown++ ? CONSOLE_CONTROLS_SECOND_X : 0), y);
			first = first ? first : icon;
		}
		if ((icon = find_prompt(button_prompts, SDL_arraysize(button_prompts), bindings[control].buttons[0])) != NULL)
			draw_prompt(picture, icon, x + CONSOLE_CONTROLS_BUTTON_X, y);
	}
}
#endif

/* The window's picture, in the game's pixels: the board's 314 columns in the console version (its first 6 left out:
   console_internal.h's LEFT_STRIPE, there the K052109's layers A and B show the end of their maps, which the cabinet's
   bezel hid; Sebas: "no la quiero"), the arcade game's 320 whole, as MAME draws it; or the console's WIDE, 398 (16:9,
   the 416 columns its frames have less 9 at each side) or 496 (20:9) */
static int view_width(void)
{
#ifdef TMNT_CONSOLE
	static const int widths[WIDE_KINDS] = { SHOWN_WIDTH, 398, 496 };

	return console_settings.wide > 0 && console_settings.wide < WIDE_KINDS ? widths[console_settings.wide] : SHOWN_WIDTH;
#else
	return SHOWN_WIDTH;
#endif
}

/* The part of the texture the frame shows: the board's picture, or WIDE's wider one, its middle as wide as the
   window's picture; a narrower one (the menus' with WIDE) between black bars */
static SDL_Rect content = { SHOWN_LEFT, 0, SHOWN_WIDTH, TMNT_VIDEO_HEIGHT };

/* The picture in the window, as large as it fits with its proportions */
static void present(void)
{
	const int kind = pixels_kind();
	const SDL_Rect source = content;
	const int view = view_width();
	int out_width = 0, out_height = 0;
	double scale;
	SDL_Rect frame, shown;

	SDL_GetRendererOutputSize(renderer, &out_width, &out_height);
	scale = SDL_min((double)out_width / view, (double)out_height / TMNT_VIDEO_HEIGHT);
	frame.w = (int)(view * scale);
	frame.h = (int)(TMNT_VIDEO_HEIGHT * scale);
	frame.x = (out_width - frame.w) / 2;
	frame.y = (out_height - frame.h) / 2;
	shown = frame;
	shown.w = (int)(source.w * scale);
	shown.x = (out_width - shown.w) / 2;
	SDL_SetTextureScaleMode(texture, kind == 2 ? SDL_ScaleModeLinear : SDL_ScaleModeNearest);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	if (kind == 1 || kind == 3)
	{
		const int times = SDL_max(1, (int)scale);
		const SDL_Rect whole = { 0, 0, source.w * times, TMNT_VIDEO_HEIGHT * times };

		if (!scaled || scaled_times != times || scaled_width != source.w)
		{
			if (scaled)
				SDL_DestroyTexture(scaled);
			scaled = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
				source.w * times, TMNT_VIDEO_HEIGHT * times);
			scaled_width = source.w;
			if (scaled)
				SDL_SetTextureScaleMode(scaled, SDL_ScaleModeLinear);
			scaled_times = times;
		}
		if (scaled)
		{
			SDL_SetRenderTarget(renderer, scaled);
			SDL_RenderCopy(renderer, texture, &source, &whole);
			SDL_SetRenderTarget(renderer, NULL);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, scaled, &whole, &shown);
		}
		else
			SDL_RenderCopy(renderer, texture, &source, &shown);
		if (kind == 3)
		{
			make_crt_overlay(shown.w, shown.h);
			if (crt_overlay)
				SDL_RenderCopy(renderer, crt_overlay, NULL, &shown);
		}
	}
	else
		SDL_RenderCopy(renderer, texture, &source, &shown);
#ifdef TMNT_CONSOLE
	count_fps();
	draw_fps(&frame);
#endif
	SDL_RenderPresent(renderer);
}

/* ---- the picture ---- */

#ifdef TMNT_CONSOLE
/* the frame shown, with the console's picture over it (the pause stops on it): the board's 320 columns, or WIDE's
   wider frame */
static uint32_t picture_pixels[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
static console_picture picture = { picture_pixels, TMNT_VIDEO_MAX_WIDTH, 0, 0 };

/* The frame the video drew into 'into' (its rows TMNT_VIDEO_MAX_WIDTH apart): WIDE's, with its sides, or the board's */
static void take_frame(console_picture *into)
{
	int width;
	const uint32_t *wide = tmnt_video_wide_frame(&width);

	if (width > TMNT_VIDEO_WIDTH)
	{
		into->left = tmnt_video_wide_left();
		into->right = width - TMNT_VIDEO_WIDTH - into->left;
		for (int y = 0; y < TMNT_VIDEO_HEIGHT; y++)
			memcpy(&into->pixels[y * into->stride], &wide[y * TMNT_VIDEO_MAX_WIDTH], (size_t)width * sizeof(uint32_t));
	}
	else
	{
		into->left = into->right = 0;
		for (int y = 0; y < TMNT_VIDEO_HEIGHT; y++)
			memcpy(&into->pixels[y * into->stride], &game_frame()->pixels[y * TMNT_VIDEO_WIDTH],
				TMNT_VIDEO_WIDTH * sizeof(uint32_t));
	}
}

/* The picture into the texture, and the part of it the window shows: WIDE's middle as wide as the window's picture,
   or the board's without its first columns */
static void upload_picture(const console_picture *shown_picture)
{
	const int width = TMNT_VIDEO_WIDTH + shown_picture->left + shown_picture->right;
	const SDL_Rect place = { 0, 0, width, TMNT_VIDEO_HEIGHT };

	SDL_UpdateTexture(texture, &place, shown_picture->pixels, shown_picture->stride * (int)sizeof(uint32_t));
	if (width > TMNT_VIDEO_WIDTH)
	{
		const int view = view_width();

		content = place;
		if (width > view)
		{
			content.x = (width - view) / 2;
			content.w = view;
		}
	}
	else
		content = (SDL_Rect){ SHOWN_LEFT, 0, SHOWN_WIDTH, TMNT_VIDEO_HEIGHT };
}
#endif

static void show_frame(void)
{
#ifdef TMNT_CONSOLE
	take_frame(&picture);
	console_overlay(&picture);
	draw_controls(&picture);    /* (OPTIONS > CONTROLS: the keys' and buttons' pictures) */
	upload_picture(&picture);
#else
	SDL_UpdateTexture(texture, NULL, game_frame()->pixels, TMNT_VIDEO_WIDTH * (int)sizeof(uint32_t));
#endif
	present();
}

static void toggle_full_screen(void)
{
	const bool full = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) == 0;

	SDL_SetWindowFullscreen(window, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#ifdef TMNT_CONSOLE
	console_settings.full_screen = full;
	console_settings.changed = true;
#endif
}

static void toggle_smooth(void)
{
#ifdef TMNT_CONSOLE
	console_settings.pixels = (console_settings.pixels + 1) % PIXELS_KINDS;
	console_settings.changed = true;
#else
	smooth = !smooth;
#endif
}

/* ---- the sound ---- */

static void open_audio(void)
{
	SDL_AudioSpec wanted, got;

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
		return;
	SDL_zero(wanted);
	wanted.freq = (int)game_sound_rate();
	wanted.format = AUDIO_S16SYS;
	wanted.channels = 1;
	wanted.samples = 1024;
	audio = SDL_OpenAudioDevice(NULL, 0, &wanted, &got, 0);
	if (audio)
		SDL_PauseAudioDevice(audio, 0);
}

/* the frame's samples, unless the queue is already many frames ahead (a slow start, a pause); in TMNT at
   OPTIONS' volume (5: the game's own; 10 twice it, clipped) */
static void queue_sound(void)
{
	size_t count;
	const int16_t *samples = game_sound_samples(&count);

	if (!audio || !count || fast)
		return;
	if (SDL_GetQueuedAudioSize(audio) > QUEUE_FRAMES_MAX * count * sizeof(int16_t))
		return;
#ifdef TMNT_CONSOLE
	if (console_settings.volume != 5)
	{
		static int16_t loud[8192];
		const size_t taken = count < sizeof(loud) / sizeof(loud[0]) ? count : sizeof(loud) / sizeof(loud[0]);

		for (size_t index = 0; index < taken; index++)
		{
			const int value = samples[index] * console_settings.volume / 5;

			loud[index] = (int16_t)(value > 32767 ? 32767 : (value < -32768 ? -32768 : value));
		}
		SDL_QueueAudio(audio, loud, (Uint32)(taken * sizeof(int16_t)));
		return;
	}
#endif
	SDL_QueueAudio(audio, samples, (Uint32)(count * sizeof(int16_t)));
}

/* ---- the controls ---- */

static bool stick(SDL_GameController *pad, SDL_GameControllerAxis axis, int sign)
{
	return pad && SDL_GameControllerGetAxis(pad, axis) * sign > STICK_DEAD_ZONE;
}

#ifdef TMNT_CONSOLE
/* TMNT: each player's controls as OPTIONS > CONTROLS binds them (controls.c), the left stick moving besides; the
   menus' fixed keys besides (the arrows, Enter, Escape or Backspace), whatever the players' are */
#define TRIGGER_BOUND  16384    /* a trigger takes the control CONTROLS waits for from halfway down (of 32767) */

static uint32_t actions_pressed;      /* the menus' fixed keys newly pressed since the last frame */
static bool starts_held[PLAYERS];     /* each player's START at the last frame */
static bool pause_requested;          /* Escape or a game controller's Back in a game: run_pause, once the frame is done */

static void action_key(SDL_Keycode key)
{
	switch (key)
	{
		case SDLK_UP: actions_pressed |= ACTION_BIT(ACTION_UP); break;
		case SDLK_DOWN: actions_pressed |= ACTION_BIT(ACTION_DOWN); break;
		case SDLK_LEFT: actions_pressed |= ACTION_BIT(ACTION_LEFT); break;
		case SDLK_RIGHT: actions_pressed |= ACTION_BIT(ACTION_RIGHT); break;
		case SDLK_ESCAPE: case SDLK_BACKSPACE: actions_pressed |= ACTION_BIT(ACTION_BACK); break;
		case SDLK_RETURN: case SDLK_KP_ENTER: actions_pressed |= ACTION_BIT(ACTION_START); break;
		default: break;
	}
}

/* The button or trigger pressed on game controller 'which' for the control CONTROLS waits for, when it is the shown
   player's */
static void bind_button(SDL_JoystickID which, SDL_GameControllerButton button)
{
	const int player = console_controls_player();

	if (player < PLAYERS && controllers[player]
		&& SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[player])) == which)
	{
		controls_bind_button(player, console_controls_waiting(), button);
		console_controls_bound();
	}
}
#endif

static void read_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				program_stop();    /* (back to main: the game runs on a task's stack) */
				return;
			case SDL_KEYDOWN:
				if (event.key.repeat)
					break;
#ifdef TMNT_CONSOLE
				if (console_controls_waiting() >= 0)
				{
					/* CONTROLS waits for a key: this one (Escape leaves the control as it was) */
					if (event.key.keysym.sym != SDLK_ESCAPE)
						controls_bind_key(console_controls_player(), console_controls_waiting(), event.key.keysym.scancode);
					console_controls_bound();
					break;
				}
				if (!(event.key.keysym.mod & KMOD_ALT))    /* (Alt+Enter: the full screen alone) */
					action_key(event.key.keysym.sym);
				if (event.key.keysym.sym == SDLK_ESCAPE && (console_can_pause() || console_lan_playing()))
					pause_requested = true;    /* (a LAN game's: out of it, console_lan_quit) */
#else
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					program_stop();
					return;
				}
#endif
				if (event.key.keysym.sym == SDLK_F11
					|| (event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_ALT)))
					toggle_full_screen();
				if (event.key.keysym.sym == SDLK_F3)
					toggle_smooth();
				break;
#ifdef TMNT_CONSOLE
			case SDL_CONTROLLERBUTTONDOWN:
				/* ... or a button of the game controller plugged in for that player */
				if (console_controls_waiting() >= 0)
					bind_button(event.cbutton.which, (SDL_GameControllerButton)event.cbutton.button);
				else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK && console_can_pause())
					pause_requested = true;
				break;
			case SDL_CONTROLLERAXISMOTION:
				/* ... or one of its triggers, pressed halfway */
				if (console_controls_waiting() >= 0 && event.caxis.value >= TRIGGER_BOUND
					&& (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT || event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT))
					bind_button(event.caxis.which, event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT
						? CONTROL_LEFT_TRIGGER : CONTROL_RIGHT_TRIGGER);
				break;
#endif
			case SDL_CONTROLLERDEVICEADDED:
				for (int player = 0; player < PLAYERS; player++)
					if (!controllers[player])
					{
						controllers[player] = SDL_GameControllerOpen(event.cdevice.which);
						break;
					}
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				for (int player = 0; player < PLAYERS; player++)
					if (controllers[player]
						&& SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[player])) == event.cdevice.which)
					{
						SDL_GameControllerClose(controllers[player]);
						controllers[player] = NULL;
					}
				break;
			default:
				break;
		}
	}
}

#ifdef TMNT_CONSOLE
/* TMNT: each player's controls as CONTROLS binds them, and their START newly pressed, into the console, which puts
   each player's on his turtle's slot (while CONTROLS waits for a key or button, none) */
static void read_players(uint8_t players[PLAYERS], uint8_t *starts)
{
	const bool waiting = console_controls_waiting() >= 0;

	*starts = 0;
	for (int player = 0; player < PLAYERS; player++)
	{
		SDL_GameController *pad = controllers[player];
		const bool start = controls_holds(player, pad, CONTROL_START);
		uint8_t pressed = 0;

		pressed |= controls_holds(player, pad, CONTROL_LEFT) || stick(pad, SDL_CONTROLLER_AXIS_LEFTX, -1) ? TMNT_LEFT : 0;
		pressed |= controls_holds(player, pad, CONTROL_RIGHT) || stick(pad, SDL_CONTROLLER_AXIS_LEFTX, 1) ? TMNT_RIGHT : 0;
		pressed |= controls_holds(player, pad, CONTROL_UP) || stick(pad, SDL_CONTROLLER_AXIS_LEFTY, -1) ? TMNT_UP : 0;
		pressed |= controls_holds(player, pad, CONTROL_DOWN) || stick(pad, SDL_CONTROLLER_AXIS_LEFTY, 1) ? TMNT_DOWN : 0;
		pressed |= controls_holds(player, pad, CONTROL_ATTACK) ? TMNT_ATTACK : 0;
		pressed |= controls_holds(player, pad, CONTROL_JUMP) ? TMNT_JUMP : 0;
		if (start && !starts_held[player] && !waiting)
			*starts |= (uint8_t)(1u << player);
		starts_held[player] = start;
		players[player] = waiting ? 0 : pressed;
	}
}

static void read_controls(void)
{
	uint8_t players[PLAYERS], starts;

	if (console_controls_defaults())
		controls_defaults(console_controls_player());
	read_players(players, &starts);
	console_actions(console_controls_waiting() >= 0 ? 0 : actions_pressed);
	console_controls(players, starts);
	actions_pressed = 0;
}

/* TMNT's LAN game: this device's player's controls (player 1's keys and game controller) and START go to the network,
   and every player's come back onto the board (console_lan_frame); till they have come, the game holds: the last
   picture again (WAITING FOR its device's name, after a third of a second), the events read, the pace kept */
static void wait_for_frame(void);
static void show_frame(void);

static void lan_controls(void)
{
	uint8_t players[PLAYERS], starts;

	read_players(players, &starts);
	actions_pressed = 0;
	while (!console_lan_frame(players[0], (starts & 1u) != 0))
	{
		read_events();
		actions_pressed = 0;
		if (pause_requested)
		{
			pause_requested = false;
			console_lan_quit();
			return;
		}
		show_frame();
		wait_for_frame();
	}
}

/* ---- the pause ---- */

/* The pause's actions from the events since the last look: the arrows, Enter, Escape or Backspace, a game
   controller's pad, A, B, Start and Back; WINDOW_CLOSED when the window was closed */
#define PAUSE_BACK  ACTION_BIT(ACTION_BACK)
#define WINDOW_CLOSED (1u << 31)

static uint32_t pause_events(void)
{
	SDL_Event event;
	uint32_t pressed = 0;

	while (SDL_PollEvent(&event))
		switch (event.type)
		{
			case SDL_QUIT:
				program_stop();
				return WINDOW_CLOSED;
			case SDL_KEYDOWN:
				if (event.key.repeat)
					break;
				switch (event.key.keysym.sym)
				{
					case SDLK_UP: pressed |= ACTION_BIT(ACTION_UP); break;
					case SDLK_DOWN: pressed |= ACTION_BIT(ACTION_DOWN); break;
					case SDLK_RETURN: case SDLK_KP_ENTER:
						if (event.key.keysym.mod & KMOD_ALT)
							toggle_full_screen();
						else
							pressed |= ACTION_BIT(ACTION_ACCEPT);
						break;
					case SDLK_ESCAPE: case SDLK_BACKSPACE: pressed |= PAUSE_BACK; break;
					case SDLK_F11: toggle_full_screen(); break;
					default: break;
				}
				break;
			case SDL_CONTROLLERBUTTONDOWN:
				switch (event.cbutton.button)
				{
					case SDL_CONTROLLER_BUTTON_DPAD_UP: pressed |= ACTION_BIT(ACTION_UP); break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN: pressed |= ACTION_BIT(ACTION_DOWN); break;
					case SDL_CONTROLLER_BUTTON_A: pressed |= ACTION_BIT(ACTION_ACCEPT); break;
					case SDL_CONTROLLER_BUTTON_B: case SDL_CONTROLLER_BUTTON_START: case SDL_CONTROLLER_BUTTON_BACK:
						pressed |= PAUSE_BACK;
						break;
					default: break;
				}
				break;
			default:
				break;
		}
	return pressed;
}

/* The game stopped, its sound too, the picture in grey under PAUSE (as outrunners-port's): Escape, Backspace or a
   game controller's B, Start or Back goes on, as CONTINUE does; RESTART starts the same game again
   (console_restart_game), QUIT GAME ends it, back to the menu (console_quit_game). What was held over it is no new
   press after it */
static void run_pause(void)
{
	static uint32_t frozen_pixels[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
	static uint32_t shown_pixels[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
	console_picture shown = { shown_pixels, TMNT_VIDEO_MAX_WIDTH, 0, 0 };
	int choice = CONSOLE_PAUSE_CONTINUE;
	uint8_t players[PLAYERS], starts;

	pause_requested = false;
	if (audio)
		SDL_PauseAudioDevice(audio, 1);
	memcpy(frozen_pixels, picture.pixels, sizeof(frozen_pixels));    /* (WIDE's whole frame: Sebas) */
	shown.left = picture.left;
	shown.right = picture.right;
	for (;;)
	{
		const uint32_t pressed = pause_events();

		if (pressed & WINDOW_CLOSED)
			return;
		if (pressed & ACTION_BIT(ACTION_UP))
			choice = (choice + CONSOLE_PAUSE_ITEMS - 1) % CONSOLE_PAUSE_ITEMS;
		if (pressed & ACTION_BIT(ACTION_DOWN))
			choice = (choice + 1) % CONSOLE_PAUSE_ITEMS;
		if (pressed & ACTION_BIT(ACTION_ACCEPT))
		{
			if (choice == CONSOLE_PAUSE_RESTART)
				console_restart_game();
			else if (choice == CONSOLE_PAUSE_QUIT)
				console_quit_game();
			break;
		}
		if (pressed & PAUSE_BACK)
			break;
		memcpy(shown_pixels, frozen_pixels, sizeof(shown_pixels));
		console_pause_overlay(&shown, choice);
		upload_picture(&shown);
		present();    /* (the line chosen, a window resized or put full screen meanwhile) */
		SDL_Delay(16);
	}
	read_players(players, &starts);
	console_controls_held(players);
	actions_pressed = 0;
	if (audio)
		SDL_PauseAudioDevice(audio, 0);
	next_frame = 0;    /* (the pace from here) */
}
#else
static bool button(SDL_GameController *pad, SDL_GameControllerButton which)
{
	return pad && SDL_GameControllerGetButton(pad, which) != 0;
}

/* the controls into the board's inputs, which it reads in the next frame (active low: a cleared bit is a pressed
   button): player 1's keys and every player's game controller */
static void read_controls(void)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	uint8_t coins = 0xFF;

	for (int player = 0; player < PLAYERS; player++)
	{
		SDL_GameController *pad = controllers[player];
		const bool keyboard = player == 0;
		bool left = button(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT) || stick(pad, SDL_CONTROLLER_AXIS_LEFTX, -1);
		bool right = button(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || stick(pad, SDL_CONTROLLER_AXIS_LEFTX, 1);
		bool up = button(pad, SDL_CONTROLLER_BUTTON_DPAD_UP) || stick(pad, SDL_CONTROLLER_AXIS_LEFTY, -1);
		bool down = button(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN) || stick(pad, SDL_CONTROLLER_AXIS_LEFTY, 1);
		bool attack = button(pad, SDL_CONTROLLER_BUTTON_A);
		bool jump = button(pad, SDL_CONTROLLER_BUTTON_B);
		uint8_t pressed = 0;

		if (keyboard)
		{
			left |= keys[SDL_SCANCODE_LEFT] != 0;
			right |= keys[SDL_SCANCODE_RIGHT] != 0;
			up |= keys[SDL_SCANCODE_UP] != 0;
			down |= keys[SDL_SCANCODE_DOWN] != 0;
			attack |= keys[SDL_SCANCODE_Z] || keys[SDL_SCANCODE_LCTRL];
			jump |= keys[SDL_SCANCODE_X] || keys[SDL_SCANCODE_LALT];
		}
		pressed |= left ? TMNT_LEFT : 0;
		pressed |= right ? TMNT_RIGHT : 0;
		pressed |= up ? TMNT_UP : 0;
		pressed |= down ? TMNT_DOWN : 0;
		pressed |= attack ? TMNT_ATTACK : 0;
		pressed |= jump ? TMNT_JUMP : 0;
		tmnt_board.players[player] = (uint8_t)~pressed;
		if (keys[SDL_SCANCODE_5 + player] || button(pad, SDL_CONTROLLER_BUTTON_BACK))
			coins &= (uint8_t)~(TMNT_COIN1 << player);
	}
	if (keys[SDL_SCANCODE_9])
		coins &= (uint8_t)~TMNT_SERVICE1;
	tmnt_board.coins = coins;
}
#endif

/* ---- the console's settings (OPTIONS), kept in tmnt.ini ---- */

#ifdef TMNT_CONSOLE
static char settings_path[1100];

static void save_settings(void)
{
	const console_settings_t *settings = &console_settings;
	FILE *output;

	if (script || !settings_path[0] || console_lan_playing() || (output = fopen(settings_path, "w")) == NULL)
		return;    /* (a LAN game's: the host's settings, not this device's) */
	fprintf(output, "window_scale=%d\nfull_screen=%d\npixels=%s\nwide=%s\nvolume=%d\nfps=%d\nregion=%d\ndsw1=%02X\n"
		"dsw2=%02X\ndsw3=%02X\nname=%s\n", settings->window_scale, settings->full_screen,
		console_pixel_names[settings->pixels], console_wide_names[settings->wide], settings->volume, settings->fps,
		settings->region, settings->dsw[0], settings->dsw[1], settings->dsw[2], settings->name);
	fclose(output);
}

static void load_settings(void)
{
	console_settings_t *settings = &console_settings;
	char line[128], text[32];
	int number;
	unsigned bits;
	FILE *input = fopen(settings_path, "r");

	if (!input)
		return;
	while (fgets(line, sizeof(line), input))
	{
		if (sscanf(line, "window_scale=%d", &number) == 1 && number >= 1 && number <= 6)
			settings->window_scale = number;
		else if (sscanf(line, "full_screen=%d", &number) == 1)
			settings->full_screen = number != 0;
		else if (sscanf(line, "volume=%d", &number) == 1 && number >= 0 && number <= 10)
			settings->volume = number;
		else if (sscanf(line, "region=%d", &number) == 1 && number >= 0 && number < CONSOLE_REGIONS)
			settings->region = number;
		else if (sscanf(line, "dsw%d=%x", &number, &bits) == 2 && number >= 1 && number <= 3 && bits <= 0xFF)
			settings->dsw[number - 1] = (uint8_t)bits;
		else if (sscanf(line, "fps=%d", &number) == 1)
			settings->fps = number != 0;
		else if (strncmp(line, "name=", 5) == 0)
		{
			/* (LAN > NAME's letters only, as many as it holds) */
			int length = 0;

			for (const char *at = line + 5; *at && length < CONSOLE_NAME_LENGTH; at++)
				if ((*at >= 'A' && *at <= 'Z') || (*at >= '0' && *at <= '9') || *at == '.' || *at == '-' || *at == ' ')
					settings->name[length++] = *at;
			while (length > 0 && settings->name[length - 1] == ' ')
				length--;
			settings->name[length] = '\0';
		}
		else if (sscanf(line, "pixels=%31s", text) == 1)
		{
			for (int kind = 0; kind < PIXELS_KINDS; kind++)
				if (strcmp(text, console_pixel_names[kind]) == 0)
					settings->pixels = kind;
		}
		else if (sscanf(line, "wide=%31s", text) == 1)
		{
			for (int kind = 0; kind < WIDE_KINDS; kind++)
				if (strcmp(text, console_wide_names[kind]) == 0)
					settings->wide = kind;
		}
	}
	fclose(input);
}

/* The window as the settings say: full screen or not, and out of it the picture times window_scale; done again
   only when they change (OPTIONS > VIDEO, F11), so a window sized by hand keeps its size otherwise */
static void apply_settings(void)
{
	static int applied_scale = -1, applied_full = -1, applied_wide = -1;

	if (applied_scale == console_settings.window_scale && applied_full == (int)console_settings.full_screen
		&& applied_wide == console_settings.wide)
		return;
	SDL_SetWindowFullscreen(window, console_settings.full_screen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	if (!console_settings.full_screen)
		SDL_SetWindowSize(window, view_width() * console_settings.window_scale,
			TMNT_VIDEO_HEIGHT * console_settings.window_scale);
	applied_scale = console_settings.window_scale;
	applied_full = (int)console_settings.full_screen;
	applied_wide = console_settings.wide;
}
#endif

/* ---- the frames ---- */

static void wait_for_frame(void)
{
	const uint64_t frequency = SDL_GetPerformanceFrequency();
	const uint64_t period = frequency * FRAME_RATE_DENOMINATOR / FRAME_RATE_NUMERATOR;
	uint64_t now = SDL_GetPerformanceCounter();

	if (next_frame == 0 || now > next_frame + 4 * period)
		next_frame = now;    /* (behind by more than a few frames: the pace starts again from here) */
	next_frame += period;
	while ((now = SDL_GetPerformanceCounter()) < next_frame)
	{
		const uint64_t left_ms = (next_frame - now) * 1000 / frequency;

		if (left_ms > 1)
			SDL_Delay((Uint32)(left_ms - 1));
	}
}

#define FAST_FORWARD_SHOWN  8u

static void on_frame_done(void)
{
	read_events();
	if (script)
		tmnt_script_apply(script, tmnt_board.frame);
#ifdef TMNT_CONSOLE
	if (script)
	{
		/* (a script's controls are the players': the console puts them on their slots) */
		uint8_t players[PLAYERS];

		for (int player = 0; player < PLAYERS; player++)
			players[player] = (uint8_t)~tmnt_board.players[player];
		console_controls(players, 0);
	}
	else if (console_lan_playing())
		lan_controls();
	else
		read_controls();
	console_wide_frame();    /* (WIDE: from the next frame drawn) */
#else
	else
		read_controls();
#endif
	game_end_frame();
#ifdef TMNT_CONSOLE
	if (console_fast_forward())
	{
		/* (BOSS RUSH crossing an area: as fast as it goes, a frame in eight shown, no sound) */
		static unsigned skipped;

		if (++skipped % FAST_FORWARD_SHOWN != 0u)
		{
			read_events();
			return;
		}
		show_frame();
		return;
	}
#endif
	queue_sound();
	if (hashes)
		fprintf(hashes, "%" PRIu64 " %016" PRIx64 "\n", tmnt_board.frame, tmnt_state_hash());
	if (video_hashes)
		fprintf(video_hashes, "%" PRIu64 " %016" PRIx64 "\n", tmnt_board.frame, tmnt_video_hash(game_frame()));
	if (sound_hashes)
		fprintf(sound_hashes, "%" PRIu64 " %016" PRIx64 " %016" PRIx64 "\n", tmnt_board.frame,
			tmnt_sound_samples_hash(), tmnt_sound_state_hash());
	show_frame();
#ifdef TMNT_CONSOLE
	if (pause_requested && console_lan_playing())
		console_lan_quit();
	else if (pause_requested && console_can_pause())
		run_pause();
	pause_requested = false;
	if (console_lan_reset_wanted())
	{
		program_stop();    /* (a LAN game's start: the program again from its start, main's) */
		return;
	}
	if (console_settings.changed)
	{
		console_settings.changed = false;
		apply_settings();
		save_settings();
	}
	if (console_quit_requested())
	{
		program_stop();
		return;
	}
#endif
	if (script && script_frames && tmnt_board.frame >= script_frames)
		program_stop();
	else if (!fast)
		wait_for_frame();
}

/* ---- the start ---- */

/* Where the game keeps what it writes (the first run's assets): beside the executable on Windows; elsewhere the
   user's own folder for the game (SDL_GetPrefPath: ~/.local/share/TMNT, ~/Library/Application Support/TMNT), as
   the executable's may not be written. The caller frees it (SDL_free) */
static char *saves_folder(void)
{
#ifdef _WIN32
	return SDL_GetBasePath();
#else
	return SDL_GetPrefPath("", "TMNT");
#endif
}

static bool exists(const char *path)
{
	FILE *probe = fopen(path, "rb");

	if (!probe)
		return false;
	fclose(probe);
	return true;
}

/* A folder of assets the first run made for another build of the game (its version file; extract_assets.py's
   have none) */
static bool assets_of_another_version(const char *folder)
{
	char path[1200];
	unsigned version = 0;
	FILE *file;
	bool other;

	snprintf(path, sizeof(path), "%s/" GAME_ASSETS_VERSION_FILE, folder);
	if ((file = fopen(path, "rb")) == NULL)
		return false;
	other = fscanf(file, "%x", &version) != 1 || version != game_assets_version();
	fclose(file);
	return other;
}

/* On macOS the folder the .app is in, where 'base' (SDL_GetBasePath) is its Resources: where a player puts the ROM
   set or the assets beside the game, as beside an executable elsewhere (outrunners-port's). 0 elsewhere */
static int app_folder(const char *base, char *folder, size_t size)
{
#ifdef __APPLE__
	static const char inside[] = ".app/Contents/Resources/";
	const size_t length = base ? strlen(base) : 0, tail = sizeof(inside) - 1;
	size_t end;

	if (length < tail || length >= size || strcmp(base + length - tail, inside) != 0)
		return 0;
	for (end = length - tail; end > 0 && base[end - 1] != '/'; end--)
		;    /* (back over the .app's name) */
	memcpy(folder, base, end);
	folder[end] = ' ';
	return end > 0;
#else
	(void)base;
	(void)folder;
	(void)size;
	return 0;
#endif
}

/* The game's assets into 'found': the pack or the folder 'assets' beside the executable (beside the .app on macOS),
   in the folder of what the game keeps or in the current folder; with none (or a folder of another version), the
   first run's, in the folder of what the game keeps. false when the first run's window was closed before a ROM set
   came */
static bool find_assets(char *found, size_t size)
{
	char *base = SDL_GetBasePath(), *saves = saves_folder();
	char beside_app[1100];
	const bool in_app = app_folder(base, beside_app, sizeof(beside_app)) != 0;
	const char *places[4];
	size_t count = 0;
	bool stale = false, ready = false;
	char layout[1300];

	places[count++] = base ? base : "";
	if (in_app)
		places[count++] = beside_app;
	if (saves && (!base || strcmp(base, saves) != 0))
		places[count++] = saves;
	places[count++] = "";
	for (size_t place = 0; place < count && !ready && !stale; place++)
	{
		snprintf(found, size, "%s" GAME_PACK_FILE, places[place]);
		if (exists(found))
		{
			ready = true;
			break;
		}
		snprintf(found, size, "%sassets", places[place]);
		snprintf(layout, sizeof(layout), "%s/data/layout.txt", found);
		if (exists(layout))
		{
			if (assets_of_another_version(found))
				stale = true;    /* (the first run's, before an update: taken out again there) */
			else
				ready = true;
		}
	}
	if (!ready)
	{
		if (!stale)
			snprintf(found, size, "%sassets", saves ? saves : (base ? base : ""));
		ready = first_run(TITLE, in_app ? beside_app : base, saves, found) != 0;    /* (the ROM set beside it) */
	}
	SDL_free(base);
	SDL_free(saves);
	return ready;
}

static FILE *open_output(const char *path)
{
	FILE *file = fopen(path, "w");

	if (!file)
	{
		fprintf(stderr, "%s: cannot be written\n", path);
		exit(1);
	}
	return file;
}

static void fail(const char *message)
{
	fprintf(stderr, "%s\n", message);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITLE, message, NULL);
	exit(1);
}

int main(int argc, char **argv)
{
	char assets_path[1100];
	const char *assets = NULL;
	const char *pack_path = NULL;
	const char *script_path = NULL;
	bool service = false;
	char message[1200];
	int scale = WINDOW_SCALE;

	for (int index = 1; index < argc; index++)
	{
		if (strcmp(argv[index], "--script") == 0 && index + 1 < argc)
			script_path = argv[++index];
		else if (strcmp(argv[index], "--fast") == 0)
			fast = true;
		else if (strcmp(argv[index], "--hashes") == 0 && index + 1 < argc)
			hashes = open_output(argv[++index]);
		else if (strcmp(argv[index], "--video-hashes") == 0 && index + 1 < argc)
			video_hashes = open_output(argv[++index]);
		else if (strcmp(argv[index], "--sound-hashes") == 0 && index + 1 < argc)
			sound_hashes = open_output(argv[++index]);
		else if (strcmp(argv[index], "--service") == 0)
			service = true;
		else if (strcmp(argv[index], "--assets") == 0 && index + 1 < argc)
			assets = argv[++index];
		else if (strcmp(argv[index], "--write-pack") == 0 && index + 1 < argc)
			pack_path = argv[++index];
		else
			fail(USAGE);
	}
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0)
	{
		snprintf(message, sizeof(message), "The window cannot be opened: %s", SDL_GetError());
		fail(message);
	}
#ifdef TMNT_CONSOLE
	{
		char *saves = saves_folder();

		char controls_path[1100];

		snprintf(settings_path, sizeof(settings_path), "%s" SETTINGS_FILE, saves ? saves : "");
		snprintf(controls_path, sizeof(controls_path), "%s" CONTROLS_FILE, saves ? saves : "");
		SDL_free(saves);
		load_settings();
		controls_open(controls_path);
		scale = console_settings.window_scale;
	}
#endif
	if (!assets)
	{
		if (!find_assets(assets_path, sizeof(assets_path)))
		{
			/* (the first run's window closed before a ROM set came) */
			SDL_Quit();
			return 0;
		}
		assets = assets_path;
	}
	{
		const char *problem = game_load(assets);

		if (problem)
		{
			snprintf(message, sizeof(message), "The game's assets in %s cannot be read: %s", assets, problem);
			fail(message);
		}
	}
	if (pack_path)
	{
		const char *problem = assets_pack_write(pack_path);

		if (problem)
		{
			snprintf(message, sizeof(message), "%s: %s", pack_path, problem);
			fail(message);
		}
		SDL_Quit();
		return 0;
	}
	if (script_path)
	{
		char error[256];

		script = tmnt_script_load(script_path, error, sizeof(error));
		if (!script)
			fail(error);
		script_frames = tmnt_script_frames(script);
	}

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		view_width() * scale, TMNT_VIDEO_HEIGHT * scale, SDL_WINDOW_RESIZABLE);
	renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE) : NULL;
	if (!renderer)
		renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE) : NULL;
#ifdef TMNT_CONSOLE
	texture = renderer ? SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING,
		TMNT_VIDEO_MAX_WIDTH, TMNT_VIDEO_HEIGHT) : NULL;    /* (WIDE's frames at its left) */
#else
	texture = renderer ? SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING,
		TMNT_VIDEO_WIDTH, TMNT_VIDEO_HEIGHT) : NULL;
#endif
	if (!texture)
	{
		snprintf(message, sizeof(message), "The window cannot be opened: %s", SDL_GetError());
		fail(message);
	}
	SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);
	SDL_RaiseWindow(window);    /* (the keyboard's: after the first run's window closed, another may have had it) */

	{
		const char *problem = game_reset(service, true);

		if (problem)
		{
			snprintf(message, sizeof(message), "The game does not start: %s", problem);
			fail(message);
		}
	}
#ifdef TMNT_CONSOLE
	{
		/* (the menus' letters beside the executable) */
		char *base = SDL_GetBasePath();

		if (!console_attach(base))
			fprintf(stderr, "%sfont.bmp cannot be read: the menus have no letters\n", base ? base : "");
		load_prompts(base);
		SDL_free(base);
		apply_settings();
	}
#endif
	open_audio();
	if (script)
		tmnt_script_apply(script, 0);
	program_frame_done = on_frame_done;
	program_run();
#ifdef TMNT_CONSOLE
	while (console_lan_reset_wanted())
	{
		/* (a LAN game: the board reset, the console as at power-on with the host's settings, the program from its
		   start, on every device alike) */
		const char *problem = game_reset(false, true);

		if (problem)
			fail(problem);
		console_lan_begin();
		program_run();
	}
#endif

	if (hashes)
		fclose(hashes);
	if (video_hashes)
		fclose(video_hashes);
	if (sound_hashes)
		fclose(sound_hashes);
	tmnt_script_free(script);
	if (audio)
		SDL_CloseAudioDevice(audio);
	if (crt_overlay)
		SDL_DestroyTexture(crt_overlay);
	if (scaled)
		SDL_DestroyTexture(scaled);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
