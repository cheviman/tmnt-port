/**
 *    Name:        main.c
 *    Description: the game in a window: SDL2 shows its screen, reads the controls and keeps the pace, around
 *                 the same game and board the headless runs check
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: tmnt [--script FILE [--fast]] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE] [--service]
 *                [--assets FOLDER]
 *
 *    The game (runtime/src/game.c and the program) runs as it is: this file only wraps it. When each frame is
 *    done the port puts it in the window, queues its sound, reads the keyboard and the game controllers into
 *    the board's inputs for the next one, and waits for the frame's time (the board's 59.185 a second) to
 *    pass.
 *
 *    The game's data is its assets (game/assets.h), the files tools/extract_assets.py writes from the
 *    player's ROM set: --assets, or the folder 'assets' beside the executable, or in the current folder.
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
 */
#include "game/game.h"
#include "tmnt/machine.h"
#include "tmnt/program.h"
#include "tmnt/script.h"
#include "tmnt/sound_board.h"
#include "tmnt/video.h"

#include <SDL.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TITLE               "Teenage Mutant Ninja Turtles"
#define PLAYERS             4
#define WINDOW_SCALE        3
#define STICK_DEAD_ZONE     8000
#define QUEUE_FRAMES_MAX    6          /* sound queued beyond this many frames is dropped */
/* the board's refresh: 6 MHz pixels, 384 a line, 264 lines */
#define FRAME_RATE_NUMERATOR    6000000u
#define FRAME_RATE_DENOMINATOR  (384u * 264u)

#define USAGE "Usage: tmnt [--script FILE [--fast]] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE]" \
	" [--service] [--assets FOLDER]"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static SDL_AudioDeviceID audio;
static SDL_GameController *controllers[PLAYERS];
static bool smooth;
static uint64_t next_frame;           /* performance counter value the next frame waits for */
static tmnt_script *script;
static uint64_t script_frames;
static bool fast;
static FILE *hashes;
static FILE *video_hashes;
static FILE *sound_hashes;

/* ---- the picture ---- */

static void show_frame(void)
{
	SDL_UpdateTexture(texture, NULL, game_frame()->pixels, TMNT_VIDEO_WIDTH * (int)sizeof(uint32_t));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

static void toggle_full_screen(void)
{
	const bool full = (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) == 0;

	SDL_SetWindowFullscreen(window, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

static void toggle_smooth(void)
{
	smooth = !smooth;
	SDL_SetTextureScaleMode(texture, smooth ? SDL_ScaleModeLinear : SDL_ScaleModeNearest);
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

/* the frame's samples, unless the queue is already many frames ahead (a slow start, a pause) */
static void queue_sound(void)
{
	size_t count;
	const int16_t *samples = game_sound_samples(&count);

	if (!audio || !count || fast)
		return;
	if (SDL_GetQueuedAudioSize(audio) > QUEUE_FRAMES_MAX * count * sizeof(int16_t))
		return;
	SDL_QueueAudio(audio, samples, (Uint32)(count * sizeof(int16_t)));
}

/* ---- the controls ---- */

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
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					program_stop();
					return;
				}
				if (event.key.keysym.sym == SDLK_F11
					|| (event.key.keysym.sym == SDLK_RETURN && (event.key.keysym.mod & KMOD_ALT)))
					toggle_full_screen();
				if (event.key.keysym.sym == SDLK_F3)
					toggle_smooth();
				break;
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

static bool button(SDL_GameController *pad, SDL_GameControllerButton which)
{
	return pad && SDL_GameControllerGetButton(pad, which) != 0;
}

static bool stick(SDL_GameController *pad, SDL_GameControllerAxis axis, int sign)
{
	return pad && SDL_GameControllerGetAxis(pad, axis) * sign > STICK_DEAD_ZONE;
}

/* the controls into the board's inputs, which it reads in the next frame (active low: a cleared bit is a
   pressed button) */
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

static void on_frame_done(void)
{
	read_events();
	if (script)
		tmnt_script_apply(script, tmnt_board.frame);
	else
		read_controls();
	game_end_frame();
	queue_sound();
	if (hashes)
		fprintf(hashes, "%" PRIu64 " %016" PRIx64 "\n", tmnt_board.frame, tmnt_state_hash());
	if (video_hashes)
		fprintf(video_hashes, "%" PRIu64 " %016" PRIx64 "\n", tmnt_board.frame, tmnt_video_hash(game_frame()));
	if (sound_hashes)
		fprintf(sound_hashes, "%" PRIu64 " %016" PRIx64 " %016" PRIx64 "\n", tmnt_board.frame,
			tmnt_sound_samples_hash(), tmnt_sound_state_hash());
	show_frame();
	if (script && script_frames && tmnt_board.frame >= script_frames)
		program_stop();
	else if (!fast)
		wait_for_frame();
}

/* ---- the start ---- */

/* 'assets' beside the executable, else in the current folder */
static const char *find_assets(char *path, size_t size)
{
	char *base = SDL_GetBasePath();

	if (base)
	{
		FILE *probe;

		snprintf(path, size, "%sassets/data/layout.txt", base);
		probe = fopen(path, "rb");
		snprintf(path, size, "%sassets", base);
		SDL_free(base);
		if (probe)
		{
			fclose(probe);
			return path;
		}
	}
	return "assets";
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
	char assets_path[1024];
	const char *assets = NULL;
	const char *script_path = NULL;
	bool service = false;
	char message[1200];

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
		else
			fail(USAGE);
	}
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0)
	{
		snprintf(message, sizeof(message), "The window cannot be opened: %s", SDL_GetError());
		fail(message);
	}
	if (!assets)
		assets = find_assets(assets_path, sizeof(assets_path));
	{
		const char *problem = game_load(assets);

		if (problem)
		{
			snprintf(message, sizeof(message), "The game's assets in %s cannot be read: %s", assets, problem);
			fail(message);
		}
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
		TMNT_VIDEO_WIDTH * WINDOW_SCALE, TMNT_VIDEO_HEIGHT * WINDOW_SCALE, SDL_WINDOW_RESIZABLE);
	renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) : NULL;
	if (!renderer)
		renderer = window ? SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE) : NULL;
	texture = renderer ? SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING,
		TMNT_VIDEO_WIDTH, TMNT_VIDEO_HEIGHT) : NULL;
	if (!texture)
	{
		snprintf(message, sizeof(message), "The window cannot be opened: %s", SDL_GetError());
		fail(message);
	}
	SDL_RenderSetLogicalSize(renderer, TMNT_VIDEO_WIDTH, TMNT_VIDEO_HEIGHT);
	SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);

	{
		const char *problem = game_reset(service, true);

		if (problem)
		{
			snprintf(message, sizeof(message), "The game does not start: %s", problem);
			fail(message);
		}
	}
	open_audio();
	if (script)
		tmnt_script_apply(script, 0);
	program_frame_done = on_frame_done;
	program_run();

	if (hashes)
		fclose(hashes);
	if (video_hashes)
		fclose(video_hashes);
	if (sound_hashes)
		fclose(sound_hashes);
	tmnt_script_free(script);
	if (audio)
		SDL_CloseAudioDevice(audio);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
