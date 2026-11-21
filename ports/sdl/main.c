/**
 *    Name:        main.c
 *    Description: the game in a window: SDL shows its screen, reads the controls and keeps the pace, around
 *                 the same game and board the headless runs check
 *    Version:     0.2.0
 *    Created:     29-10-2026
 *    Modified:    19-11-2026
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
 *      C                              attack and jump at once (the special attack)
 *      5, 6, 7, 8                     coins 1-4      9                 service 1
 *      Escape                         quit           F11 or Alt+Enter  full screen
 *      F3                             smooth or sharp pixels
 *    Game controllers, players 1 to 4 in the order they are plugged: the pad or the left stick moves, A
 *    attacks, B jumps, X does both at once, Back is the coin.
 *    TMNT: no coins; Enter (player 1) and a controller's Start are START, which brings a player in; in the
 *    menus the arrows move, attack (Z, A) picks, jump (X, B) or Escape goes back; F3 the next kind of pixels.
 */
#ifndef TMNT_HANDHELD    /* (a device makes nothing from the ROM set: its assets pack comes with the game) */
#include "first_run.h"
#endif

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
#ifdef TMNT_TOUCH
#include "touch.h"         /* (Android's touch screen: player 1's controls) */
#endif

#include "port_sdl.h"           /* SDL 3; or its names over a device's own SDL 2 (sdl2_layer.h) */
#ifndef TMNT_SDL2_LAYER
#include <SDL3/SDL_main.h> /* (the program's start as each platform has it: WinMain, Android's activity) */
#endif
#if defined(SDL_PLATFORM_MACOS) && defined(TMNT_CONSOLE)
#include <objc/message.h>  /* (mac_full_screen: the window's NSWindow) */
#include <objc/runtime.h>
#endif

#ifdef __SWITCH__
#include <switch.h>        /* romfsInit: the game's own files, its .nsp's or .nro's */
#include <sys/stat.h>
#endif
#if !defined(_WIN32) && !defined(TMNT_HANDHELD)
#include <unistd.h>        /* (isatty: a computer's log, open_log) */
#endif

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TMNT_CONSOLE
#define FILE_NAME           "tmnt"
#else
#define FILE_NAME           "tmnt-arcade"
#endif
#ifdef __SWITCH__
#define SWITCH_FOLDER       "sdmc:/switch/" FILE_NAME    /* the game's folder on the SD card: its settings, its log */
#endif
#define MAPPINGS_FILE       "gamecontrollerdb.txt"    /* more game controllers for SDL (SDL_GameControllerDB's) */
#define LOG_RUNS            3                         /* the runs whose logs are kept: .log, .1.log, .2.log (log_into) */
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
static SDL_AudioStream *audio;     /* the sound's stream, on the default device (open_audio) */
static SDL_Gamepad *controllers[PLAYERS];
#ifndef TMNT_CONSOLE
static bool smooth;                   /* F3: the picture as SDL softens it */
#endif
static uint64_t next_frame;          /* performance counter value the next frame waits for */
static bool vsync_on;                /* the renderer shows each frame at the screen's refresh (apply_vsync) */
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
	if (!crt_overlay || !SDL_LockTexture(crt_overlay, NULL, (void **)&pixels, &pitch))
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
	SDL_FRect cells[2][14 * 15];
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
					cells[1][count] = (SDL_FRect){ (float)(x + column * pixel), (float)(shown->y + (4 + row) * pixel),
						(float)pixel, (float)pixel };
					cells[0][count] = cells[1][count];
					cells[0][count].x += (float)pixel;
					cells[0][count].y += (float)pixel;
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
	int what;      /* an SDL_Scancode or an SDL_GamepadButton */
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
	{ SDL_GAMEPAD_BUTTON_SOUTH, 0, 4, 1 }, { SDL_GAMEPAD_BUTTON_EAST, 0, 5, 1 },
	{ SDL_GAMEPAD_BUTTON_WEST, 0, 6, 1 }, { SDL_GAMEPAD_BUTTON_NORTH, 0, 7, 1 },
	{ SDL_GAMEPAD_BUTTON_DPAD_UP, 1, 1, 1 }, { SDL_GAMEPAD_BUTTON_DPAD_RIGHT, 1, 2, 1 },
	{ SDL_GAMEPAD_BUTTON_DPAD_DOWN, 1, 3, 1 }, { SDL_GAMEPAD_BUTTON_DPAD_LEFT, 1, 4, 1 },
	{ SDL_GAMEPAD_BUTTON_BACK, 18, 4, 1 }, { SDL_GAMEPAD_BUTTON_START, 18, 5, 1 },
	{ SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, 18, 7, 1 }, { SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER, 18, 8, 1 },
	{ CONTROL_LEFT_TRIGGER, 18, 9, 1 }, { CONTROL_RIGHT_TRIGGER, 18, 10, 1 },
	{ SDL_GAMEPAD_BUTTON_LEFT_STICK, 12, 16, 1 }, { SDL_GAMEPAD_BUTTON_RIGHT_STICK, 14, 16, 1 },
	{ SDL_GAMEPAD_BUTTON_GUIDE, 18, 2, 1 }, { SDL_GAMEPAD_BUTTON_MISC1, 18, 6, 1 },
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
	prompt_sheet = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_ARGB8888);
	SDL_DestroySurface(loaded);
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
   the 416 columns its frames have less 9 at each side), 496 (20:9) or 358 (16:10, the Steam Deck's: 384 less 13) */
static int view_width(void)
{
#ifdef TMNT_CONSOLE
	static const int widths[WIDE_KINDS] = { SHOWN_WIDTH, 398, 496, 358 };

	return console_settings.wide > 0 && console_settings.wide < WIDE_KINDS ? widths[console_settings.wide] : SHOWN_WIDTH;
#else
	return SHOWN_WIDTH;
#endif
}

/* The part of the texture the frame shows: the board's picture, or WIDE's wider one, its middle as wide as the
   window's picture; a narrower one (the menus' with WIDE) between black bars */
static SDL_Rect content = { SHOWN_LEFT, 0, SHOWN_WIDTH, TMNT_VIDEO_HEIGHT };

/* The picture in the window, as large as it fits with its proportions */
/* A frame a whole frame's time behind the game's pace (a screen a little slower than the board's 59.19 a second, or
   the machine busy) is not shown, so that it does not wait for the screen's refresh: the game, and its sound, keep
   their pace, a frame left out now and then (as outrunners-port's) */
static bool frame_behind(void)
{
	return vsync_on && SDL_GetPerformanceCounter()
		> next_frame + SDL_GetPerformanceFrequency() * FRAME_RATE_DENOMINATOR / FRAME_RATE_NUMERATOR;
}

#ifdef SDL_PLATFORM_IOS
/* ---- iOS and iPadOS (ports/ios, as simpsons-port's) ---- */

static bool ios_background;    /* the app gone to the background (ios_app_events): nothing drawn there */

/* The app's Documents folder, which the Files app shows as the app's own (Info.plist's UIFileSharingEnabled and
   LSSupportsOpeningDocumentsInPlace): the device's folder, where the player puts the ROM set, and the folder of what
   the game keeps and of its log. HOME is the app's own container on iOS. The caller frees it (SDL_free) */
static char *ios_documents(void)
{
	const char *home = getenv("HOME");
	const size_t length = (home ? strlen(home) : 0) + sizeof("/Documents/");
	char *folder = SDL_malloc(length);

	if (folder)
		snprintf(folder, length, "%s/Documents/", home ? home : "");
	return folder;
}

/* The app going to the background and coming back, as SDL sends them (an event watch: at once, before the frame's
   events are read, as iOS may give the app no more time): from SDL_EVENT_WILL_ENTER_BACKGROUND to
   SDL_EVENT_DID_ENTER_FOREGROUND nothing is drawn (present), an app's GPU work being refused there */
static bool ios_app_events(void *data, SDL_Event *event)
{
	(void)data;
	if (event->type == SDL_EVENT_WILL_ENTER_BACKGROUND)
		ios_background = true;
	else if (event->type == SDL_EVENT_DID_ENTER_FOREGROUND)
		ios_background = false;
	return true;
}

/* The width of each of the screen's sides the notch or the Dynamic Island takes in landscape (the window's safe area,
   SDL's: the larger of its left and right), in the renderer's pixels for a screen 'out_width' of them wide: the
   picture stays between them (present), and TMNT's WIDE is chosen for the width between them (follow_screen). 0
   where none is known */
static int ios_side_inset(int out_width)
{
	SDL_Rect safe;
	int points = 0, points_high = 0;

	if (!window || !SDL_GetWindowSize(window, &points, &points_high) || points <= 0
		|| !SDL_GetWindowSafeArea(window, &safe))
		return 0;
	return (int)(SDL_max(safe.x, points - (safe.x + safe.w)) * (double)out_width / points + 0.5);
}
#endif

#if (defined(__ANDROID__) || defined(SDL_PLATFORM_IOS)) && defined(TMNT_CONSOLE)
/* WIDE as the phone's screen is (Android's VIDEO has no WIDE, nor iOS's), chosen again whenever the screen changes: a Fold
   opened or closed mid-game (Sebas: the picture stayed as it was, in half the screen), as outrunners-port's
   follow_screen. 20:9 where the screen is twice as wide as high or more (most phones), 16:9 from 16:9 on and on a
   tablet's screen (its short side TABLET_SIDE_INCHES or more: an open Fold's, the picture at the top and the touch
   controls under it), 16:10 from 3:2 on, else the board's 320 (a handheld's 4:3) */
#define TABLET_SIDE_INCHES 4.0f

static int shape_width, shape_height;    /* the screen WIDE was last chosen for */

/* (the screen's dots an inch: Android's density, which is SDL 3's content scale of the display, times its 160) */
static bool tablet_screen(int width, int height)
{
#ifdef SDL_PLATFORM_IOS
	(void)width;
	(void)height;
	return SDL_IsTablet();    /* (an iPad) */
#else
	const float dots = 160.0f
		* SDL_GetDisplayContentScale(window ? SDL_GetDisplayForWindow(window) : SDL_GetPrimaryDisplay());

	if (dots <= 0.0f)
		return false;
	return SDL_min(width, height) / dots >= TABLET_SIDE_INCHES;
#endif
}

static int wide_for_screen(int width, int height)
{
	const int long_side = SDL_max(width, height), short_side = SDL_min(width, height);

	if (long_side * 9 >= short_side * 20)
		return WIDE_20_9;
	if (long_side * 9 >= short_side * 16 || tablet_screen(width, height))
		return WIDE_16_9;
	return long_side * 2 >= short_side * 3 ? WIDE_16_10 : WIDE_OFF;
}

/* The screen at a frame, as the renderer has it: WIDE chosen again where its size changed (the log says so) */
static void follow_screen(int width, int height)
{
	if (width <= 0 || height <= 0 || (width == shape_width && height == shape_height))
		return;
	if (console_lan_playing())
		return;    /* (a LAN game's WIDE is the host's on every device: the screen's again once it is over) */
	shape_width = width;
	shape_height = height;
	console_settings.wide = wide_for_screen(width, height);
	fprintf(stderr, "Screen %dx%d: WIDE %s\n", width, height, console_wide_names[console_settings.wide]);
}
#endif

static void present(void)
{
	const int kind = pixels_kind();
	const SDL_Rect source = content;
	const int view = view_width();
	int out_width = 0, out_height = 0;
	double scale;
	SDL_Rect frame, shown;
	SDL_FRect from, to;    /* ('source' and 'shown' as SDL draws with them) */

#ifdef SDL_PLATFORM_IOS
	if (ios_background)
		return;    /* (the app in the background: iOS refuses its GPU work there) */
#endif
	SDL_GetRenderOutputSize(renderer, &out_width, &out_height);
#if defined(__ANDROID__) && defined(TMNT_CONSOLE)
	follow_screen(out_width, out_height);    /* (a Fold opened or closed: its WIDE, from the next frame) */
#elif defined(SDL_PLATFORM_IOS) && defined(TMNT_CONSOLE)
	/* (WIDE for the screen between the sides the notch or the Dynamic Island takes) */
	follow_screen(out_width - 2 * ios_side_inset(out_width), out_height);
#endif
	scale = SDL_min((double)out_width / view, (double)out_height / TMNT_VIDEO_HEIGHT);
#ifdef SDL_PLATFORM_IOS
	{
		/* (the picture between those sides: as wide as fits there) */
		const int room = out_width - 2 * ios_side_inset(out_width);

		if (room > 0)
			scale = SDL_min(scale, (double)room / view);
	}
#endif
#ifdef TMNT_CONSOLE
	if (console_settings.integer_scale && scale >= 1.0)
		scale = floor(scale);    /* (OPTIONS > VIDEO > INTEGER SCALE: every pixel of the game's the same size) */
#endif
	frame.w = (int)(view * scale);
	frame.h = (int)(TMNT_VIDEO_HEIGHT * scale);
	frame.x = (out_width - frame.w) / 2;
	frame.y = (out_height - frame.h) / 2;
#ifdef TMNT_TOUCH
	touch_layout(out_width, out_height, &frame);    /* (a tall screen: the picture at the top, the controls under it) */
#endif
	shown = frame;
	shown.w = (int)(source.w * scale);
	shown.x = (out_width - shown.w) / 2;
	SDL_RectToFRect(&source, &from);
	SDL_RectToFRect(&shown, &to);
	SDL_SetTextureScaleMode(texture, kind == 2 ? SDL_SCALEMODE_LINEAR : SDL_SCALEMODE_NEAREST);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	if (kind == 1 || kind == 3)
	{
		const int times = SDL_max(1, (int)scale);
		const SDL_FRect whole = { 0.0f, 0.0f, (float)(source.w * times), (float)(TMNT_VIDEO_HEIGHT * times) };

		if (!scaled || scaled_times != times || scaled_width != source.w)
		{
			if (scaled)
				SDL_DestroyTexture(scaled);
			scaled = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
				source.w * times, TMNT_VIDEO_HEIGHT * times);
			scaled_width = source.w;
			if (scaled)
			{
				SDL_SetTextureScaleMode(scaled, SDL_SCALEMODE_LINEAR);
				/* (the picture as it is: SDL 3 blends a texture with an alpha channel unless told not to) */
				SDL_SetTextureBlendMode(scaled, SDL_BLENDMODE_NONE);
			}
			scaled_times = times;
		}
		if (scaled)
		{
			SDL_SetRenderTarget(renderer, scaled);
			SDL_RenderTexture(renderer, texture, &from, &whole);
			SDL_SetRenderTarget(renderer, NULL);
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, scaled, &whole, &to);
		}
		else
			SDL_RenderTexture(renderer, texture, &from, &to);
		if (kind == 3)
		{
			make_crt_overlay(shown.w, shown.h);
			if (crt_overlay)
				SDL_RenderTexture(renderer, crt_overlay, NULL, &to);
		}
	}
	else
		SDL_RenderTexture(renderer, texture, &from, &to);
#ifdef TMNT_CONSOLE
	count_fps();
	draw_fps(&frame);
#endif
#ifdef TMNT_TOUCH
	touch_draw(renderer, out_width, out_height);
#endif
	if (!frame_behind())
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

#if defined(SDL_PLATFORM_MACOS) && defined(TMNT_CONSOLE)
/* macOS's own full screen (the window's green button, Control+Command+F, View > Enter Full Screen): a Space of the
   window's own, which the settings do not know (SDL 2 did not count it as full screen; SDL 3 does, from its
   windowDidEnterFullScreen on). The NSWindow's styleMask has NSWindowStyleMaskFullScreen there, whatever SDL says */
#define MAC_STYLE_FULL_SCREEN (1ul << 14)

static bool mac_full_screen(void)
{
	void *ns_window = window ? SDL_GetPointerProperty(SDL_GetWindowProperties(window),
		SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL) : NULL;
	unsigned long style;

	if (!ns_window)
		return false;
	style = ((unsigned long (*)(id, SEL))objc_msgSend)((id)ns_window, sel_registerName("styleMask"));
	return (style & MAC_STYLE_FULL_SCREEN) != 0;
}

/* macOS's own full screen taken as SDL's (SDL_SetWindowFullscreen finds the window already in its Space: no change
   seen), so that SDL can take it out of it */
static void adopt_mac_full_screen(void)
{
	if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) && mac_full_screen())
		SDL_SetWindowFullscreen(window, true);
}
#else
#define mac_full_screen() false
#define adopt_mac_full_screen()
#endif

static void toggle_full_screen(void)
{
	bool full;

#ifdef __SWITCH__
	return;    /* (a keyboard's F11 on the Switch: its screen is the window already, apply_settings) */
#endif
	adopt_mac_full_screen();    /* (F11 in macOS's own full screen: out of it) */
	full = !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
	SDL_SetWindowFullscreen(window, full);    /* (the desktop's own mode: SDL 3's full screen with no mode chosen) */
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

/* The game's samples (16 bits, one channel, at the board's rate) into a stream on the default device, which SDL
   converts to what the device plays; the device asked for 1024 samples at a time, as it was on SDL 2 */
static void open_audio(void)
{
	SDL_AudioSpec wanted;

	if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
		return;
	SDL_zero(wanted);
	wanted.freq = (int)game_sound_rate();
	wanted.format = SDL_AUDIO_S16;
	wanted.channels = 1;
	SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, "1024");
	audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &wanted, NULL, NULL);
	if (audio)
		SDL_ResumeAudioStreamDevice(audio);    /* (a stream's device starts paused) */
}

/* the frame's samples, unless the queue is already many frames ahead (a slow start, a pause); in TMNT at
   OPTIONS' volume (5: the game's own; 10 twice it, clipped) */
static void queue_sound(void)
{
	size_t count;
	const int16_t *samples = game_sound_samples(&count);

	if (!audio || !count || fast)
		return;
	if ((size_t)SDL_GetAudioStreamQueued(audio) > QUEUE_FRAMES_MAX * count * sizeof(int16_t))
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
		SDL_PutAudioStreamData(audio, loud, (int)(taken * sizeof(int16_t)));
		return;
	}
#endif
	SDL_PutAudioStreamData(audio, samples, (int)(count * sizeof(int16_t)));
}

/* ---- the controls ---- */

static bool stick(SDL_Gamepad *pad, SDL_GamepadAxis axis, int sign)
{
	return pad && SDL_GetGamepadAxis(pad, axis) * sign > STICK_DEAD_ZONE;
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
		case SDLK_ESCAPE: case SDLK_BACKSPACE: case SDLK_AC_BACK: actions_pressed |= ACTION_BIT(ACTION_BACK); break;
		case SDLK_RETURN: case SDLK_KP_ENTER: actions_pressed |= ACTION_BIT(ACTION_START); break;
		default: break;
	}
}

/* The button or trigger pressed on game controller 'which' for the control CONTROLS waits for, when it is the shown
   player's */
static void bind_button(SDL_JoystickID which, SDL_GamepadButton button)
{
	const int player = console_controls_player();

	if (player < PLAYERS && controllers[player]
		&& SDL_GetJoystickID(SDL_GetGamepadJoystick(controllers[player])) == which)
	{
		controls_bind_button(player, console_controls_waiting(), button);
		console_controls_bound();
	}
}
#endif

#ifdef TMNT_HANDHELD_KEYS
/* Anbernic's H700 handhelds: their buttons, a joystick SDL 2.0.12 (the stock firmware's) does not know, as a game
   controller's (outrunners-port's mapping); MENU, its button 8, leaves the game as the firmware's other games */
#define HANDHELD_MENU_BUTTON 8

static const char *const device_mappings[] = {
	"19000000010000000100000000010000,ANBERNIC-keys,a:b0,b:b1,x:b3,y:b2,leftshoulder:b4,rightshoulder:b10,"
	"lefttrigger:b5,righttrigger:b11,back:b6,start:b7,guide:b8,dpup:h0.1,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,"
	"leftx:a0,lefty:a1,rightx:a2,righty:a3,leftstick:b9,rightstick:b12,platform:Linux,",
};

#endif

#ifdef TMNT_CONTROLLER_DB
/* SDL_GameControllerDB's mappings for this platform (ports/sdl/controllerdb, built in by CMakeLists.txt) */
extern const unsigned char tmnt_controller_db[];
extern const size_t tmnt_controller_db_size;
#endif

/* Text added at the end of a growing one (malloc's: the Anbernic's SDL 2.0.12 is linked by name only) */
static void add_text(char **text, size_t *length, const char *more, size_t count)
{
	char *grown = (char *)realloc(*text, *length + count + 1);

	if (!grown)
		return;
	memcpy(grown + *length, more, count);
	*length += count;
	grown[*length] = '\0';
	*text = grown;
}

#ifndef TMNT_HANDHELD
static char *saves_folder(void);

/* gamecontrollerdb.txt, in the folder of what the game keeps or else beside the executable: the mappings of the
   game controllers SDL does not know (or knows wrong), a line each as SDL_GameControllerDB has them; its lines for
   this platform (or for none) at the end of 'text', without carriage returns */
static void add_mappings_file(char **text, size_t *length)
{
	char path[1024];
	char *saves = saves_folder();
	const char *folders[2];
	const char *platform = SDL_GetPlatform();

	folders[0] = saves;
	folders[1] = SDL_GetBasePath();    /* (SDL's own text: not freed) */
	for (int folder = 0; folder < 2; folder++)
	{
		FILE *file;
		char line[2048];
		int lines = 0;

		if (!folders[folder])
			continue;
		snprintf(path, sizeof(path), "%s" MAPPINGS_FILE, folders[folder]);
		if ((file = fopen(path, "r")) == NULL)
			continue;
		while (fgets(line, sizeof(line), file))
		{
			char *end = line + strlen(line), *field = strstr(line, "platform:");

			while (end > line && (end[-1] == '\n' || end[-1] == '\r'))
				*--end = '\0';
			if (line[0] == '\0' || line[0] == '#'
				|| (field && (strncmp(field + 9, platform, strlen(platform)) != 0 || field[9 + strlen(platform)] != ',')))
				continue;
			add_text(text, length, line, strlen(line));
			add_text(text, length, "\n", 1);
			lines++;
		}
		fclose(file);
		fprintf(stderr, "Game controllers: %s, %d mappings for %s\n", path, lines, platform);
		break;
	}
	SDL_free(saves);
}
#endif

/* The game controllers' mappings, a hint before SDL starts (SDL_HINT_GAMECONTROLLERCONFIG, which SDL reads before
   it looks for joysticks: a mapping added after SDL_Init leaves a joystick already plugged in a joystick, which the
   game does not take, as SDL 2.32's SDL_PrivateAddMappingForGUID did). SDL_GameControllerDB's for this platform
   built in (a player's report: an Nvidia Shield's PS4 and Xbox controllers, plugged in before the game started), the
   Anbernic's own buttons, the environment's SDL_GAMECONTROLLERCONFIG (Steam's, in the games it starts), and last the
   player's gamecontrollerdb.txt, so that its lines go over the others for the same controller. The hint overrides
   the environment's, which SDL_GetHint takes over a hint of normal priority: the built-in lines and the file left
   out */
static void add_mappings(void)
{
	char *text = NULL;
	size_t length = 0;
	const char *environment = getenv(SDL_HINT_GAMECONTROLLERCONFIG);    /* (C's: the Anbernic's SDL by name only) */

#ifdef TMNT_CONTROLLER_DB
	add_text(&text, &length, (const char *)tmnt_controller_db, tmnt_controller_db_size);
#endif
#ifdef TMNT_HANDHELD_KEYS
	for (size_t mapping = 0; mapping < SDL_arraysize(device_mappings); mapping++)
	{
		add_text(&text, &length, device_mappings[mapping], strlen(device_mappings[mapping]));
		add_text(&text, &length, "\n", 1);
	}
#endif
	if (environment && environment[0])
	{
		add_text(&text, &length, environment, strlen(environment));
		add_text(&text, &length, "\n", 1);
		fprintf(stderr, "Game controllers: the environment's " SDL_HINT_GAMECONTROLLERCONFIG " too\n");
	}
#ifndef TMNT_HANDHELD
	add_mappings_file(&text, &length);
#endif
	if (text)
		SDL_SetHintWithPriority(SDL_HINT_GAMECONTROLLERCONFIG, text, SDL_HINT_OVERRIDE);
	free(text);
}

/* What SDL finds of each joystick, in the log (a player's report: on an Nvidia Shield the controllers' buttons did
   what they should not): its name, its GUID, whether it is a game controller and its mapping; and the first presses
   of keys and buttons, to tell a controller's button that comes as a key */
#define INPUT_LOG_PRESSES   48

static void log_joystick(SDL_JoystickID which)
{
	char guid_text[33];
	const SDL_GUID guid = SDL_GetJoystickGUIDForID(which);
	const char *name = SDL_GetJoystickNameForID(which);
	char *mapping;

	SDL_GUIDToString(guid, guid_text, sizeof(guid_text));
	mapping = SDL_GetGamepadMappingForGUID(guid);
	fprintf(stderr, "Joystick %d: %s, GUID %s, %s%s\n", (int)which, name ? name : "(no name)", guid_text,
		SDL_IsGamepad(which) ? "a game controller: " : "no game controller (no mapping)",
		SDL_IsGamepad(which) && mapping ? mapping : "");
	SDL_free(mapping);
}

static void log_press(const SDL_Event *event)
{
	static int presses;

	if (presses >= INPUT_LOG_PRESSES)
		return;
	if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
		fprintf(stderr, "Key %s (scancode %d)\n", SDL_GetKeyName(event->key.key), (int)event->key.scancode);
	else if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
		fprintf(stderr, "Game controller %d: %s\n", (int)event->gbutton.which,
			SDL_GetGamepadStringForButton((SDL_GamepadButton)event->gbutton.button));
	else if (event->type == SDL_EVENT_JOYSTICK_BUTTON_DOWN)
		fprintf(stderr, "Joystick %d: button %d\n", (int)event->jbutton.which, (int)event->jbutton.button);
	else
		return;
	if (++presses == INPUT_LOG_PRESSES)
		fprintf(stderr, "(no more presses in the log)\n");
}

/* A TV's remote, which SDL takes for a game controller (the Shield's, Android TV's, Fire TV's: "... Remote"); on
   Android also one by its buttons, whatever its name: a pad, OK (A) and back (Back), but no B and no Start (the
   Chromecast with Google TV's) */
static bool is_remote(SDL_Gamepad *pad)
{
	const char *name = SDL_GetGamepadName(pad);

	for (const char *at = name ? name : ""; *at; at++)
		if (SDL_strncasecmp(at, "remote", 6) == 0)
			return true;
#ifdef __ANDROID__
	if (!SDL_GamepadHasButton(pad, SDL_GAMEPAD_BUTTON_EAST)
		&& !SDL_GamepadHasButton(pad, SDL_GAMEPAD_BUTTON_START))
		return true;
#endif
	return false;
}

#if defined(__ANDROID__) && defined(TMNT_CONSOLE)
/* The game controller 'which' is a TV's remote: its Back is its player's JUMP, not the pause (read_players) */
static bool remote_back(SDL_JoystickID which)
{
	SDL_Gamepad *pad = SDL_GetGamepadFromID(which);

	return pad && is_remote(pad);
}
#else
#define remote_back(which) false
#endif

/* A game controller plugged in: the first player without one, but a game controller before a TV's remote (a
   player's report: the Shield's remote, found first, took player 1 from his Xbox controller) */
static void controller_added(SDL_JoystickID which)
{
	for (int player = 0; player < PLAYERS; player++)
		if (!controllers[player])
		{
			controllers[player] = SDL_OpenGamepad(which);
			if (!controllers[player] || is_remote(controllers[player]))
				return;
			for (int before = 0; before < player; before++)
				if (is_remote(controllers[before]))
				{
					SDL_Gamepad *remote = controllers[before];

					controllers[before] = controllers[player];
					controllers[player] = remote;
					return;
				}
			return;
		}
}

#ifndef TMNT_HANDHELD
/* The mouse's pointer, hidden over the game once it has stood still for two seconds; moving it shows it again (a
   player's suggestion, on a Mac: issue 8) */
#define POINTER_SHOWN_MS  2000u
static Uint64 pointer_moved;

static void pointer_event(const SDL_Event *event)
{
	if (event->type != SDL_EVENT_MOUSE_MOTION && event->type != SDL_EVENT_MOUSE_BUTTON_DOWN && event->type != SDL_EVENT_MOUSE_WHEEL)
		return;
	pointer_moved = SDL_GetTicks();
	if (!SDL_CursorVisible())
		SDL_ShowCursor();
}

static void pointer_frame(void)
{
	if (SDL_GetTicks() - pointer_moved >= POINTER_SHOWN_MS && SDL_CursorVisible())
		SDL_HideCursor();
}
#else
#define pointer_event(event)
#define pointer_frame()
#endif

static void read_events(void)
{
	SDL_Event event;

	pointer_frame();
	while (SDL_PollEvent(&event))
	{
		log_press(&event);
		pointer_event(&event);
#ifdef TMNT_TOUCH
		touch_event(&event);
#endif
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				program_stop();    /* (back to main: the game runs on a task's stack) */
				return;
#ifdef TMNT_HANDHELD_KEYS
			case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
				if (event.jbutton.button == HANDHELD_MENU_BUTTON)
				{
					program_stop();    /* (MENU leaves the game, as the firmware's other games) */
					return;
				}
				break;
#endif
#if (defined(__ANDROID__) || defined(SDL_PLATFORM_IOS)) && defined(TMNT_CONSOLE)
			case SDL_EVENT_WILL_ENTER_BACKGROUND:
				if (console_can_pause())
					pause_requested = true;    /* (Android or iOS may end the game there: a game waits, paused) */
				break;
#endif
			case SDL_EVENT_KEY_DOWN:
				if (event.key.repeat)
					break;
#if defined(TMNT_HANDHELD_KEYS) && defined(TMNT_CONSOLE)
				if (event.key.key == SDLK_VOLUMEUP || event.key.key == SDLK_VOLUMEDOWN)
				{
					/* (the handheld's volume keys: OPTIONS > VOLUME's, kept) */
					const int volume = console_settings.volume + (event.key.key == SDLK_VOLUMEUP ? 1 : -1);

					console_settings.volume = SDL_clamp(volume, 0, 10);
					console_settings.changed = true;
					break;
				}
#endif
#ifdef TMNT_CONSOLE
				if (console_controls_waiting() >= 0)
				{
					/* CONTROLS waits for a key: this one (Escape leaves the control as it was) */
					if (event.key.key != SDLK_ESCAPE)
						controls_bind_key(console_controls_player(), console_controls_waiting(), event.key.scancode);
					console_controls_bound();
					break;
				}
				if (!(event.key.mod & SDL_KMOD_ALT))    /* (Alt+Enter: the full screen alone) */
					action_key(event.key.key);
				if ((event.key.key == SDLK_ESCAPE || event.key.key == SDLK_AC_BACK)
					&& (console_can_pause() || console_lan_playing()))
					pause_requested = true;    /* (a LAN game's: out of it, console_lan_quit; Android's back too) */
#else
				if (event.key.key == SDLK_ESCAPE)
				{
					program_stop();
					return;
				}
#endif
				if (event.key.key == SDLK_F11
					|| (event.key.key == SDLK_RETURN && (event.key.mod & SDL_KMOD_ALT)))
					toggle_full_screen();
				if (event.key.key == SDLK_F3)
					toggle_smooth();
				break;
#ifdef TMNT_CONSOLE
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				/* ... or a button of the game controller plugged in for that player */
				if (console_controls_waiting() >= 0)
					bind_button(event.gbutton.which, (SDL_GamepadButton)event.gbutton.button);
				else if (event.gbutton.button == SDL_GAMEPAD_BUTTON_BACK && console_can_pause()
					&& !remote_back(event.gbutton.which) && !console_coins_on())    /* (COINS: Back is the coin) */
					pause_requested = true;    /* (a TV remote's: its home pauses, SDL_EVENT_WILL_ENTER_BACKGROUND) */
				break;
			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				/* ... or one of its triggers, pressed halfway */
				if (console_controls_waiting() >= 0 && event.gaxis.value >= TRIGGER_BOUND
					&& (event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER || event.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER))
					bind_button(event.gaxis.which, event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER
						? CONTROL_LEFT_TRIGGER : CONTROL_RIGHT_TRIGGER);
				break;
#endif
			case SDL_EVENT_JOYSTICK_ADDED:
				log_joystick(event.jdevice.which);
				break;
			case SDL_EVENT_GAMEPAD_ADDED:
				controller_added(event.gdevice.which);
				break;
			case SDL_EVENT_GAMEPAD_REMOVED:
				for (int player = 0; player < PLAYERS; player++)
					if (controllers[player]
						&& SDL_GetJoystickID(SDL_GetGamepadJoystick(controllers[player])) == event.gdevice.which)
					{
						SDL_CloseGamepad(controllers[player]);
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
		SDL_Gamepad *pad = controllers[player];
		bool start = controls_holds(player, pad, CONTROL_START);
		uint8_t pressed = 0;

#ifdef TMNT_TOUCH
		if (player == 0)
		{
			pressed |= touch_controls();
			start = start || touch_start();
		}
#endif
		pressed |= controls_holds(player, pad, CONTROL_LEFT) || stick(pad, SDL_GAMEPAD_AXIS_LEFTX, -1) ? TMNT_LEFT : 0;
		pressed |= controls_holds(player, pad, CONTROL_RIGHT) || stick(pad, SDL_GAMEPAD_AXIS_LEFTX, 1) ? TMNT_RIGHT : 0;
		pressed |= controls_holds(player, pad, CONTROL_UP) || stick(pad, SDL_GAMEPAD_AXIS_LEFTY, -1) ? TMNT_UP : 0;
		pressed |= controls_holds(player, pad, CONTROL_DOWN) || stick(pad, SDL_GAMEPAD_AXIS_LEFTY, 1) ? TMNT_DOWN : 0;
		pressed |= controls_holds(player, pad, CONTROL_ATTACK) ? TMNT_ATTACK : 0;
		pressed |= controls_holds(player, pad, CONTROL_JUMP) ? TMNT_JUMP : 0;
		pressed |= controls_holds(player, pad, CONTROL_SPECIAL) ? TMNT_ATTACK | TMNT_JUMP : 0;    /* (both at once) */
#ifdef __ANDROID__
		/* a TV's remote (the Chromecast with Google TV's, which has a pad, OK and back alone): its back is JUMP (the
		   menus' back, the pause's CONTINUE), and its OK, ATTACK, the START of a player out of the game (in, or a
		   continue) */
		if (pad && is_remote(pad))
		{
			if (SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_BACK))
				pressed |= TMNT_JUMP;
			if (SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_SOUTH) && console_can_pause()
				&& !console_start_pauses(player))
				start = true;
		}
#endif
		if (start && !starts_held[player] && !waiting)
			*starts |= (uint8_t)(1u << player);
		starts_held[player] = start;
		players[player] = waiting ? 0 : pressed;
	}
}

/* COINS (console_coins_on): each player's coin newly pressed, the cabinet's keys 5-8 (as TMNT Arcade's) or his game
   controller's Back (not a TV remote's: its back is JUMP) */
static void read_coins(void)
{
	static bool held[PLAYERS];
	const bool *keys = SDL_GetKeyboardState(NULL);

	for (int player = 0; player < PLAYERS; player++)
	{
		SDL_Gamepad *pad = controllers[player];
		const bool coin = keys[SDL_SCANCODE_5 + player]
			|| (pad && !is_remote(pad) && SDL_GetGamepadButton(pad, SDL_GAMEPAD_BUTTON_BACK));

		if (coin && !held[player] && console_coins_on())
			console_coin(player);
		held[player] = coin;
	}
}

static void read_controls(void)
{
	uint8_t players[PLAYERS], starts;

	if (console_controls_defaults())
		controls_defaults(console_controls_player());
	read_coins();
	read_players(players, &starts);
	for (int player = 0; player < PLAYERS; player++)
		if ((starts & (1u << player)) && console_start_pauses(player))
		{
			starts &= (uint8_t)~(1u << player);
			pause_requested = true;    /* (START of a player in the game: the pause, as a console's) */
		}
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

/* The pause's actions from the events since the last look: the arrows, Enter, Escape or Backspace (Android's back) and
   a game controller's Back; WINDOW_CLOSED when the window was closed. The players' own controls are players_pressed's;
   the touch screen's fingers go to its controls */
#define PAUSE_BACK  ACTION_BIT(ACTION_BACK)
#define WINDOW_CLOSED (1u << 31)

static uint32_t pause_events(void)
{
	SDL_Event event;
	uint32_t pressed = 0;

	pointer_frame();
	while (SDL_PollEvent(&event))
	{
		pointer_event(&event);
#ifdef TMNT_TOUCH
		touch_event(&event);
#endif
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				program_stop();
				return WINDOW_CLOSED;
			case SDL_EVENT_KEY_DOWN:
				if (event.key.repeat)
					break;
				switch (event.key.key)
				{
					case SDLK_UP: pressed |= ACTION_BIT(ACTION_UP); break;
					case SDLK_DOWN: pressed |= ACTION_BIT(ACTION_DOWN); break;
					case SDLK_RETURN: case SDLK_KP_ENTER:
						if (event.key.mod & SDL_KMOD_ALT)
							toggle_full_screen();
						else
							pressed |= ACTION_BIT(ACTION_ACCEPT);
						break;
					case SDLK_ESCAPE: case SDLK_BACKSPACE: case SDLK_AC_BACK: pressed |= PAUSE_BACK; break;
					case SDLK_F11: toggle_full_screen(); break;
					default: break;
				}
				break;
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
				if (event.gbutton.button == SDL_GAMEPAD_BUTTON_BACK)
					pressed |= PAUSE_BACK;    /* (the button that pauses, as Escape) */
				break;
			default:
				break;
		}
	}
	return pressed;
}

/* The pause's actions from every player's controls newly pressed, as the menus take them (console_controls): up and
   down, ATTACK the line chosen, JUMP or START go on. Their keys and buttons as CONTROLS binds them, the left stick, the
   touch screen's (a player's report: with ATTACK on a controller's X, the menus took X and the pause A). 'held': what
   each player held at the last look */
static uint32_t players_pressed(uint8_t held[PLAYERS])
{
	uint8_t players[PLAYERS], starts;
	uint32_t pressed = 0;

	read_players(players, &starts);
	for (int player = 0; player < PLAYERS; player++)
	{
		uint8_t fresh = (uint8_t)(players[player] & ~held[player]);

		if ((fresh & (TMNT_ATTACK | TMNT_JUMP)) == (TMNT_ATTACK | TMNT_JUMP))
			fresh &= (uint8_t)~TMNT_JUMP;    /* (SPECIAL's both at once: the attack's, a pick) */
		pressed |= (fresh & TMNT_UP) ? ACTION_BIT(ACTION_UP) : 0u;
		pressed |= (fresh & TMNT_DOWN) ? ACTION_BIT(ACTION_DOWN) : 0u;
		pressed |= (fresh & TMNT_ATTACK) ? ACTION_BIT(ACTION_ACCEPT) : 0u;
		pressed |= (fresh & TMNT_JUMP) ? PAUSE_BACK : 0u;
		held[player] = players[player];
	}
	return pressed | (starts ? PAUSE_BACK : 0u);
}

/* The game stopped, its sound too, the picture in grey under PAUSE (as outrunners-port's): Escape, Backspace, a game
   controller's Back or a player's JUMP or START goes on, as CONTINUE does (Android's back too); RESTART starts the
   same game again (console_restart_game), QUIT GAME ends it, back to the menu (console_quit_game). It keeps the
   frames' pace (wait_for_frame): with VSYNC a picture a frame behind it is not shown (frame_behind), and a pause
   that slept on its own showed its first picture only (players' reports: the line chosen never moved). What was held
   over it is no new press after it */
static void run_pause(void)
{
	static uint32_t frozen_pixels[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
	static uint32_t shown_pixels[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
	console_picture shown = { shown_pixels, TMNT_VIDEO_MAX_WIDTH, 0, 0 };
	int choice = CONSOLE_PAUSE_CONTINUE;
	uint8_t players[PLAYERS], starts, held[PLAYERS];

	pause_requested = false;
	if (audio)
		SDL_PauseAudioStreamDevice(audio);
	read_players(held, &starts);    /* (what was held as the pause came, the START that paused too: no new press) */
	memcpy(frozen_pixels, picture.pixels, sizeof(frozen_pixels));    /* (WIDE's whole frame: Sebas) */
	shown.left = picture.left;
	shown.right = picture.right;
	for (;;)
	{
		uint32_t pressed = pause_events();

		pressed |= players_pressed(held);
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
		wait_for_frame();
	}
	read_players(players, &starts);
	console_controls_held(players);
	actions_pressed = 0;
	if (audio)
		SDL_ResumeAudioStreamDevice(audio);
	next_frame = 0;    /* (the pace from here) */
}
#else
static bool button(SDL_Gamepad *pad, SDL_GamepadButton which)
{
	return pad && SDL_GetGamepadButton(pad, which);
}

/* the controls into the board's inputs, which it reads in the next frame (active low: a cleared bit is a pressed
   button): player 1's keys and every player's game controller */
static void read_controls(void)
{
	const bool *keys = SDL_GetKeyboardState(NULL);
	uint8_t coins = 0xFF;

	for (int player = 0; player < PLAYERS; player++)
	{
		SDL_Gamepad *pad = controllers[player];
		const bool keyboard = player == 0;
		bool left = button(pad, SDL_GAMEPAD_BUTTON_DPAD_LEFT) || stick(pad, SDL_GAMEPAD_AXIS_LEFTX, -1);
		bool right = button(pad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT) || stick(pad, SDL_GAMEPAD_AXIS_LEFTX, 1);
		bool up = button(pad, SDL_GAMEPAD_BUTTON_DPAD_UP) || stick(pad, SDL_GAMEPAD_AXIS_LEFTY, -1);
		bool down = button(pad, SDL_GAMEPAD_BUTTON_DPAD_DOWN) || stick(pad, SDL_GAMEPAD_AXIS_LEFTY, 1);
		bool attack = button(pad, SDL_GAMEPAD_BUTTON_SOUTH);
		bool jump = button(pad, SDL_GAMEPAD_BUTTON_EAST);
		bool special = button(pad, SDL_GAMEPAD_BUTTON_WEST);    /* (attack and jump at once: the special attack) */
		uint8_t pressed = 0;

		if (keyboard)
		{
			left |= keys[SDL_SCANCODE_LEFT] != 0;
			right |= keys[SDL_SCANCODE_RIGHT] != 0;
			up |= keys[SDL_SCANCODE_UP] != 0;
			down |= keys[SDL_SCANCODE_DOWN] != 0;
			attack |= keys[SDL_SCANCODE_Z] || keys[SDL_SCANCODE_LCTRL];
			jump |= keys[SDL_SCANCODE_X] || keys[SDL_SCANCODE_LALT];
			special |= keys[SDL_SCANCODE_C] != 0;
		}
		attack |= special;
		jump |= special;
		pressed |= left ? TMNT_LEFT : 0;
		pressed |= right ? TMNT_RIGHT : 0;
		pressed |= up ? TMNT_UP : 0;
		pressed |= down ? TMNT_DOWN : 0;
		pressed |= attack ? TMNT_ATTACK : 0;
		pressed |= jump ? TMNT_JUMP : 0;
#ifdef TMNT_TOUCH
		if (player == 0)
			pressed |= touch_controls();
#endif
		tmnt_board.players[player] = (uint8_t)~pressed;
		if (keys[SDL_SCANCODE_5 + player] || button(pad, SDL_GAMEPAD_BUTTON_BACK))
			coins &= (uint8_t)~(TMNT_COIN1 << player);
#ifdef TMNT_TOUCH
		if (player == 0 && touch_start())
			coins &= (uint8_t)~TMNT_COIN1;    /* (TMNT Arcade's touch screen: COIN) */
#endif
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
	fprintf(output, "window_scale=%d\nfull_screen=%d\npixels=%s\ninteger_scale=%d\nwide=%s\nvsync=%d\nvolume=%d\n"
		"fps=%d\npress_start=%s\nregion=%d\ncoins=%d\ndsw1=%02X\ndsw2=%02X\ndsw3=%02X\nname=%s\n", settings->window_scale,
		settings->full_screen, console_pixel_names[settings->pixels], settings->integer_scale,
		console_wide_names[settings->wide], settings->vsync, settings->volume, settings->fps,
		console_press_start_names[settings->press_start], settings->region, settings->coins, settings->dsw[0],
		settings->dsw[1], settings->dsw[2], settings->name);
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
		else if (sscanf(line, "coins=%d", &number) == 1)
			settings->coins = number != 0;
		else if (sscanf(line, "vsync=%d", &number) == 1)
			settings->vsync = number != 0;
		else if (sscanf(line, "integer_scale=%d", &number) == 1)
			settings->integer_scale = number != 0;
		else if (sscanf(line, "press_start=%31s", text) == 1)
		{
			for (int kind = 0; kind < PRESS_START_KINDS; kind++)
				if (strcmp(text, console_press_start_names[kind]) == 0)
					settings->press_start = kind;
		}
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

/* VSYNC (OPTIONS > VIDEO): the renderer shows each frame at the screen's refresh, unless the frames run as fast as
   they can (--fast). The game keeps its own pace, the board's 59.19 frames a second (wait_for_frame): a faster screen
   (120 Hz) shows a frame twice. A device's screen keeps the system's own refresh (and the Anbernic's SDL 2.0.12 has
   no SDL_SetRenderVSync) */
static void apply_vsync(void)
{
#ifndef TMNT_HANDHELD
	const bool wanted = console_settings.vsync && !fast;

	if (renderer && wanted != vsync_on)
		vsync_on = SDL_SetRenderVSync(renderer, wanted ? 1 : 0) ? wanted : false;
#endif
}

/* The window as the settings say: full screen or not, and out of it the picture times window_scale; done again
   only when they change (OPTIONS > VIDEO, F11), so a window sized by hand keeps its size otherwise. In macOS's own
   full screen, which the settings do not know, a size or WIDE changed leaves it full screen: the window made that
   size there was the picture at the screen's top left (a player's report on a Mac) */
static void apply_settings(void)
{
	static int applied_scale = -1, applied_full = -1, applied_wide = -1;
	const bool full_changed = applied_full != (int)console_settings.full_screen;

	apply_vsync();

	if (applied_scale == console_settings.window_scale && !full_changed && applied_wide == console_settings.wide)
		return;
	applied_scale = console_settings.window_scale;
	applied_full = (int)console_settings.full_screen;
	applied_wide = console_settings.wide;
#ifdef __SWITCH__
	return;    /* (the window is the Switch's 1280 x 720 screen already. Full screen would make it SDL's desktop mode,
	              which devkitPro's SDL has 1920 x 1080 always (SWITCH_VideoInit): the picture drawn for 1080p and
	              shown in part on the 720p screen, the players' issue 19. Docked, the system scales the 720p up) */
#endif
	if (mac_full_screen() && !full_changed)
		return;    /* (the picture fills the screen at any size) */
	adopt_mac_full_screen();    /* (FULL SCREEN changed there: SDL's own from here, OFF out of it) */
	SDL_SetWindowFullscreen(window, console_settings.full_screen);
	if (!console_settings.full_screen)
		SDL_SetWindowSize(window, view_width() * console_settings.window_scale,
			TMNT_VIDEO_HEIGHT * console_settings.window_scale);
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
		/* (the pace from here: wait_for_frame is not reached, and with VSYNC frame_behind would hold back every
		   picture of the crossing, its card never shown, the players' issue 24) */
		next_frame = SDL_GetPerformanceCounter();
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
static char *base_folder(void)
{
	const char *base = SDL_GetBasePath();    /* (SDL's own text: a copy of it) */

	return base ? SDL_strdup(base) : NULL;
}

static char *saves_folder(void)
{
#ifdef __SWITCH__
	return SDL_strdup(SWITCH_FOLDER "/");
#elif defined(__ANDROID__)
	return SDL_GetPrefPath("", "TMNT");    /* (the app's own folder, its internal storage) */
#elif defined(SDL_PLATFORM_IOS)
	return ios_documents();    /* (the app's Documents, the Files app's: the player can see and keep them) */
#elif defined(_WIN32) || defined(TMNT_HANDHELD)
	return base_folder();    /* (a handheld's: the game's folder on its card) */
#else
	return SDL_GetPrefPath("", "TMNT");
#endif
}

/* The log into FILE_NAME.log in 'folder' (with its separator), the last runs' moved before it to FILE_NAME.1.log and
   FILE_NAME.2.log, the older ones dropped (simpsons-port's) */
static void log_into(const char *folder)
{
	char path[1100], older[1100];

	for (int run = LOG_RUNS - 1; run > 0; run--)
	{
		snprintf(older, sizeof(older), "%s" FILE_NAME ".%d.log", folder, run);
		if (run > 1)
			snprintf(path, sizeof(path), "%s" FILE_NAME ".%d.log", folder, run - 1);
		else
			snprintf(path, sizeof(path), "%s" FILE_NAME ".log", folder);
		remove(older);
		rename(path, older);
	}
	snprintf(path, sizeof(path), "%s" FILE_NAME ".log", folder);
	if (freopen(path, "w", stderr))
		setvbuf(stderr, NULL, _IONBF, 0);
}

/* The log: on Windows a game started with nowhere for its standard error to go (from Explorer, not a terminal), and on
   Linux and macOS one whose standard error is not a terminal (a launcher's, the Finder's), writes it to FILE_NAME.log
   in the folder of what the game keeps: what stopped a game reaches the player, who can send it (issue 21, TMNT gone
   after the Technodrome's lift with nothing said); on the Switch, which has no console (and SDL no message boxes
   there), always, in its folder on the SD card; on Android always, in the app's folder on the storage; on the
   Anbernic, where its launcher sends it. Each keeps the last runs' too (log_into) */
static void open_log(void)
{
#ifdef __SWITCH__
	mkdir("sdmc:/switch", 0777);
	mkdir(SWITCH_FOLDER, 0777);
	log_into(SWITCH_FOLDER "/");
#elif defined(__ANDROID__)
	/* (Android's standard error goes nowhere: the log in the app's folder on the storage, Android/data/<the
	   app>/files, where the player can read it) */
	if (SDL_GetAndroidExternalStoragePath())
	{
		char folder[1100];

		snprintf(folder, sizeof(folder), "%s/", SDL_GetAndroidExternalStoragePath());
		log_into(folder);
	}
#elif defined(SDL_PLATFORM_IOS)
	{
		/* (an app's standard error reaches no one: the log in its Documents, where the player puts the ROM set and the
		   Files app shows it) */
		char *documents = ios_documents();

		log_into(documents ? documents : "");
		SDL_free(documents);
	}
#elif defined(_WIN32)
	if (_fileno(stderr) < 0)
	{
		char *saves = saves_folder();

		log_into(saves ? saves : "");
		SDL_free(saves);
	}
#elif !defined(TMNT_HANDHELD)
	if (!isatty(fileno(stderr)))
	{
		char *saves = saves_folder();

		log_into(saves ? saves : "");
		SDL_free(saves);
	}
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

static void fail(const char *message);

#ifdef __ANDROID__
static const char no_pack_in_apk[] = "";    /* (load_android_pack: the APK carries no pack, a release's) */

/* Android's pack, in the APK's assets (stored there, android.cmake: TMNT APK's): no file, so read whole through SDL's
   files and loaded from its bytes. NULL, no_pack_in_apk, or what is wrong */
static const char *load_android_pack(void)
{
	SDL_IOStream *packed = SDL_IOFromFile(GAME_PACK_FILE, "rb");
	const Sint64 size = packed ? SDL_GetIOSize(packed) : -1;
	uint8_t *bytes = size > 0 ? malloc((size_t)size) : NULL;
	size_t got = 0;
	const char *problem;

	if (!packed)
		return no_pack_in_apk;
	while (bytes && got < (size_t)size)
	{
		/* (its bytes as the asset manager hands them: maybe fewer than asked for at a time) */
		const size_t now = SDL_ReadIO(packed, bytes + got, (size_t)size - got);

		if (now == 0)
			break;
		got += now;
	}
	SDL_CloseIO(packed);
	problem = !bytes || got != (size_t)size ? "The game's assets cannot be read" : assets_load_pack_bytes(bytes, got);
	free(bytes);
	return problem;
}
#endif

#ifdef TMNT_HANDHELD
#define ROM_SET_FILE    "tmnt.zip"       /* (first_run.c's: the ROM set, or its folder rom with its files) */
#define ROM_SET_FOLDER  "rom"
#define ROM_SET_PROBE   "963-x23.j17"

/* A device's own folder, where the player puts the ROM set (or a pack) and the game takes the assets out of it: the
   Switch's sdmc:/switch/tmnt/ (TMNT's and TMNT Arcade's both), the handheld's PORTS/tmnt/ (beside both
   executables), Android's app folder on its storage (Android/data/<the app>/files/, where its log is), iOS's app's
   Documents (the Files app's, each game its own). The caller frees it (SDL_free) */
static char *device_folder(void)
{
#ifdef __SWITCH__
	return SDL_strdup("sdmc:/switch/tmnt/");
#elif defined(__ANDROID__)
	const char *storage = SDL_GetAndroidExternalStoragePath();
	const size_t length = storage ? strlen(storage) + 2 : 1;
	char *folder = SDL_malloc(length);

	if (folder)
		snprintf(folder, length, "%s%s", storage ? storage : "", storage ? "/" : "");
	return folder;
#elif defined(SDL_PLATFORM_IOS)
	return ios_documents();
#else
	return base_folder();
#endif
}

/* The assets in the device's folder, for the packages of the releases, which carry none (Sebas: "todos por separado
   sin assets"): the pack tmnt.assets there, or the folder assets the game took out before (of this version), or else
   taken out now of the ROM set there (tmnt.zip, or the folder rom with the set's files or its .zip) into the folder
   assets, as a computer's first run does, with no window. false, and what is wrong in 'problem', when there is none */
static bool device_assets(char *found, size_t size, char *problem, size_t problem_size)
{
	char *folder = device_folder();
	const char *place = folder ? folder : "";
	char source[1100], probe[1200];
	const char *failed;
	bool ready = false;

	snprintf(found, size, "%s" GAME_PACK_FILE, place);
	if (exists(found))
		ready = true;
	if (!ready)
	{
		snprintf(found, size, "%sassets", place);
		snprintf(probe, sizeof(probe), "%s/data/layout.txt", found);
		ready = exists(probe) && !assets_of_another_version(found);
	}
	if (!ready)
	{
		snprintf(source, sizeof(source), "%s" ROM_SET_FILE, place);
		if (!exists(source))
		{
			snprintf(source, sizeof(source), "%s" ROM_SET_FOLDER, place);
			snprintf(probe, sizeof(probe), "%s/" ROM_SET_FILE, source);
			if (!exists(probe))
			{
				snprintf(probe, sizeof(probe), "%s/" ROM_SET_PROBE, source);
				if (!exists(probe))
					source[0] = '\0';
			}
		}
		if (!source[0])
#ifdef SDL_PLATFORM_IOS
			/* (the app's Documents as the player finds it: in the Files app, or from a computer) */
			snprintf(problem, problem_size, "No ROM set: put " ROM_SET_FILE " (MAME's tmnt set) in this game's folder, in "
				"the Files app: On My iPhone (On My iPad) > " TITLE ". From a computer: Finder (or the Apple Devices app "
				"on Windows), the device, Files, " TITLE ". Then start the game again: the first start takes the game's "
				"assets out of it (some seconds).");
#else
			snprintf(problem, problem_size, "No ROM set: put " ROM_SET_FILE " (MAME's tmnt set) in %s, and start the game "
				"again. The first start takes the game's assets out of it (some seconds).", place);
#endif
		else
		{
			fprintf(stderr, "The assets out of %s into %s\n", source, found);
			if ((failed = game_assets_extract(source, found)) == NULL)
				ready = true;
			else
				snprintf(problem, problem_size, "The assets cannot be taken out of %s: %s", source, failed);
		}
	}
	SDL_free(folder);
	return ready;
}

#ifndef __ANDROID__    /* (Android's pack: load_android_pack) */
/* A device's assets: the pack its package carries (in its romfs on the Switch, beside the executables on the
   handheld: the makers' packages), else device_assets'; none, and the game says what to do */
static bool find_assets(char *found, size_t size)
{
	char problem[1200];

#ifdef __SWITCH__
	snprintf(found, size, "romfs:/" GAME_PACK_FILE);
	if (exists(found))
		return true;
#endif
	if (!device_assets(found, size, problem, sizeof(problem)))
		fail(problem);
	return true;
}
#endif
#else
/* The game's assets into 'found': the pack or the folder 'assets' beside the executable (beside the .app on macOS),
   in the folder of what the game keeps or in the current folder; with none (or a folder of another version), the
   first run's, in the folder of what the game keeps. false when the first run's window was closed before a ROM set
   came */
static bool find_assets(char *found, size_t size)
{
	const char *base = SDL_GetBasePath();
	char *saves = saves_folder();
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
		ready = first_run(TITLE, base, in_app ? beside_app : NULL, saves, found) != 0;    /* (the ROM set beside it) */
	}
	SDL_free(saves);
	return ready;
}
#endif

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
#ifdef __SWITCH__
	{
		/* (the Switch's SDL has no message box: libnx's text console, before any window, till + is pressed. Not the
		   system's error dialog: from 10.0.0 it is a fatal error out of an application, as an .nro from the album) */
		PadState pad;

		consoleInit(NULL);
		padConfigureInput(1, HidNpadStyleSet_NpadStandard);
		padInitializeDefault(&pad);
		printf("%s\n\n%s\n\nPress + to leave.\n", TITLE, message);
		while (appletMainLoop())
		{
			padUpdate(&pad);
			if (padGetButtonsDown(&pad) & HidNpadButton_Plus)
				break;
			consoleUpdate(NULL);
		}
		consoleExit(NULL);
	}
#else
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITLE, message, NULL);
#endif
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
#ifdef __SWITCH__
	/* (the assets pack in the game's romfs; its settings and its log on the SD card: the Switch has no console, and
	   SDL no message boxes there) */
	romfsInit();
#endif
	open_log();    /* (what the game says, its failures among them: the log, where the platform has no console) */
#ifdef __ANDROID__
	SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");    /* (back is the menus' back, never the end) */
	SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");          /* (fingers are the touch controls, not the mouse) */
	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
#endif
#ifdef SDL_PLATFORM_IOS
	SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");          /* (fingers are the touch controls, not the mouse) */
	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
	SDL_SetHint(SDL_HINT_IOS_HIDE_HOME_INDICATOR, "2");     /* (dimmed, and a first swipe from the bottom only shows it) */
	SDL_SetHint(SDL_HINT_AUDIO_CATEGORY, "playback");       /* (AVAudioSession's playback: heard with the ringer off) */
#endif
	add_mappings();
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
	{
		snprintf(message, sizeof(message), "The window cannot be opened: %s", SDL_GetError());
		fail(message);
	}
	{
		int mappings = 0;

		SDL_free(SDL_GetGamepadMappings(&mappings));
		fprintf(stderr, "SDL %d.%d.%d\n", SDL_VERSIONNUM_MAJOR(SDL_GetVersion()),
			SDL_VERSIONNUM_MINOR(SDL_GetVersion()), SDL_VERSIONNUM_MICRO(SDL_GetVersion()));
		fprintf(stderr, "Game controllers: %d mappings\n", mappings);
	}
#ifdef SDL_PLATFORM_IOS
	SDL_AddEventWatch(ios_app_events, NULL);    /* (the background, as it comes: nothing drawn there) */
	SDL_DisableScreenSaver();                   /* (no dimming nor locking while a game controller plays) */
#endif
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
#ifdef TMNT_HANDHELD
		console_settings.full_screen = true;    /* (a device's screen, whole: apply_settings never makes it a window) */
#endif
#ifdef TMNT_SCREEN_WIDE
		console_settings.wide = WIDE_16_9;    /* (a device's screen: 16:9, its picture fixed) */
#endif
	}
#endif
#ifdef __ANDROID__
	{
		const char *problem = load_android_pack();

		if (problem == no_pack_in_apk)
		{
			/* (a release's APK: the assets in the app's folder, or out of the ROM set the player put there) */
			if (!device_assets(assets_path, sizeof(assets_path), message, sizeof(message)))
				fail(message);
			problem = game_load(assets_path);
		}
		if (problem)
			fail(problem);
	}
#else
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
#endif
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

#ifdef __SWITCH__
	(void)scale;
	/* (the Switch's screen, whole. OPENGL: devkitPro's SDL 3 loads EGL only for a window that asks for it, and its
	   Switch window is an EGL surface always; without it SDL_CreateWindow fails, "EGL not initialized", issue 27) */
	window = SDL_CreateWindow(TITLE, 1280, 720, SDL_WINDOW_OPENGL);
#elif defined(TMNT_HANDHELD_KEYS)
	(void)scale;
	window = SDL_CreateWindow(TITLE, 640, 480, SDL_WINDOW_FULLSCREEN);    /* (the handheld's, whole) */
#elif defined(__ANDROID__)
	(void)scale;
	/* (the phone's whole screen: full screen is SDL's immersive mode, the system's bars hidden. A window would leave
	   them over the picture, Android 15 drawing the app under them: a player's OnePlus 12, the score cut) */
	window = SDL_CreateWindow(TITLE, 1280, 720, SDL_WINDOW_FULLSCREEN);
#elif defined(SDL_PLATFORM_IOS)
	(void)scale;
	/* (the whole screen at its own pixels (HIGH_PIXEL_DENSITY: the touch controls' size by the screen's density,
	   touch.c, in those pixels), the status bar hidden (BORDERLESS)) */
	window = SDL_CreateWindow(TITLE, 1280, 720, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
		| SDL_WINDOW_HIGH_PIXEL_DENSITY);
#else
	window = SDL_CreateWindow(TITLE, view_width() * scale, TMNT_VIDEO_HEIGHT * scale, SDL_WINDOW_RESIZABLE);
#endif
	/* The platform's first renderer that starts, SDL's own in software the last of them. On Windows that is Direct3D
	   11 (Sebas, "pone por default que use dx11"): SDL 3's first there, where SDL 2's was Direct3D 9 and the game
	   asked for this one by a hint. Its swap chain is DXGI's flip model, the frames' pace steadier in a window with
	   VSYNC, and Alt+Tab or the full screen lose none of its textures. SDL_RENDER_DRIVER in the environment still
	   chooses; the log says which one came */
	renderer = window ? SDL_CreateRenderer(window, NULL) : NULL;
	if (renderer)
		fprintf(stderr, "Renderer: %s\n", SDL_GetRendererName(renderer));    /* (which one came: a player's question) */
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
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
#ifdef TMNT_TOUCH
	{
#ifdef __ANDROID__
		const char *base = "";    /* (the APK's assets, which SDL's files read by a relative path) */
#else
		const char *base = SDL_GetBasePath();
#endif

		touch_open(renderer, base ? base : "");
	}
#endif
#if (defined(__ANDROID__) || defined(SDL_PLATFORM_IOS)) && defined(TMNT_CONSOLE)
	{
		/* WIDE as the phone's screen is at the start (follow_screen: again as it changes) */
		int out_width = 0, out_height = 0;

		SDL_GetRenderOutputSize(renderer, &out_width, &out_height);
		if (out_height <= 0)
			console_settings.wide = WIDE_16_9;
		follow_screen(out_width, out_height);
	}
#endif
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
		const char *base = SDL_GetBasePath();

		if (!console_attach(base))
			fprintf(stderr, "%sfont.bmp cannot be read: the menus have no letters\n", base ? base : "");
		load_prompts(base);
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
		SDL_DestroyAudioStream(audio);    /* (its device with it) */
	if (crt_overlay)
		SDL_DestroyTexture(crt_overlay);
	if (scaled)
		SDL_DestroyTexture(scaled);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
#ifdef __SWITCH__
	romfsExit();
#endif
#ifdef __ANDROID__
	exit(0);    /* (the app's process ends with the game: Android would start the next one in it, as this one left it) */
#endif
	return 0;
}
