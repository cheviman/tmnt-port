/**
 *    Name:        main.c
 *    Description: runs the game with no screen or sound, with the controls a script drives, and writes hashes
 *                 of every frame in the formats tools/core/konami/reference/kref writes
 *    Version:     0.1.2
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: tmnt_headless [--frames N] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE] [--script FILE]
 *                         [--service] [--assets FOLDER or PACK] [--dump FRAME --dump-dir DIR] [--write-pack FILE]
 *                         [--video-dump FRAME ... --video-dir DIR]
 *                         (tmnt_console_headless: [--actions FILE])
 *    The game's data is its assets (game/assets.h), the files tools/extract_assets.py writes from the
 *    player's ROM set (or the game's first run, runtime/src/extract.c): --assets (a folder, or a pack: a file
 *    whose name ends in .assets), or the folder 'assets', or where there is none the pack tmnt.assets, in the
 *    current folder. The game opens no ROM. --write-pack writes the pack of the assets loaded and stops.
 *
 *    --dump writes the board's memories at the end of frame FRAME into DIR as kref --dump does (FRAME_work_ram.bin,
 *    in the order of the addresses, and the palette, the tile RAM and the sprite RAM), to compare them.
 *    --video-dump writes the frames FRAME ... as pictures into DIR (frame_FRAME.bmp, as kref --video-dump), the
 *    console's own picture over them in its build.
 *
 *    The board runs with frame timing, as kref does by default (tmnt/frame.h), the script is applied when each
 *    VBLANK starts, and the hashes are kref's ("FRAME HASH": tmnt_state_hash, the frame's pixels with
 *    --video-hashes, the sound board's samples and RAM with --sound-hashes), so the two compare line by line. --service turns the test mode's DIP switch on.
 *
 *    The console version's build (TMNT_CONSOLE: tmnt_console_headless) runs the console's front end, its
 *    settings the defaults: the script's controls are the players' (player N's on the slot of the turtle he plays,
 *    console/console.c), and --actions FILE gives its menus' actions, a line each: the frame, then up, down, left,
 *    right, accept, back or start, and for start the player (1-4, 1 when none: his START, in the menus and in a
 *    game). Each picture --video-dump writes says where the console stands, on the standard output.
 */
#include "game/assets.h"
#include "game/game.h"
#include "tmnt/machine.h"
#include "tmnt/program.h"
#include "tmnt/script.h"
#include "tmnt/sound_board.h"
#include "tmnt/video.h"
#ifdef TMNT_CONSOLE
#include "console.h"
#endif

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: tmnt_headless [--frames N] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE]" \
	" [--script FILE] [--service] [--assets FOLDER or PACK] [--dump FRAME --dump-dir DIR] [--write-pack FILE]" \
	" [--video-dump FRAME ... --video-dir DIR] (tmnt_console_headless: [--actions FILE] [--wide 1|2])\n"
#define MAX_VIDEO_DUMPS 64

static uint64_t frames_wanted = 600;
static FILE *hashes;
static FILE *video_hashes;
static FILE *sound_hashes;
static tmnt_script *script;
static uint64_t dump_frame = UINT64_MAX;
static const char *dump_dir = ".";
static uint64_t video_dumps[MAX_VIDEO_DUMPS];
static int video_dump_count;
static const char *video_dir = ".";

#ifdef TMNT_CONSOLE
/* --actions: the console's menus' actions, and the players' START */
#define MAX_ACTIONS 4096

static struct
{
	uint64_t frame;
	uint32_t action;
	int player;
} actions[MAX_ACTIONS];
static size_t action_count;

/* The pause's, besides the menus': "pause" shows the pause's picture in the dumps from then on (CONTINUE chosen),
   "quit" and "restart" do QUIT GAME and RESTART there (the game goes on meanwhile: the window's pause holds it) */
#define ACTION_PAUSE    (1u << 29)
#define ACTION_QUIT     (1u << 30)
#define ACTION_RESTART  (1u << 31)
static bool pause_shown;

static const char *load_actions(const char *path)
{
	static const char *const names[ACTIONS] = { "up", "down", "left", "right", "accept", "back", "start" };
	char line[256], name[32];
	unsigned long long frame;
	int player;
	FILE *input = fopen(path, "r");

	if (!input)
		return "The actions file cannot be read.";
	while (fgets(line, sizeof(line), input) && action_count < MAX_ACTIONS)
	{
		int action, read;

		if (line[0] == '#' || (read = sscanf(line, "%llu %31s %d", &frame, name, &player)) < 2)
			continue;
		for (action = 0; action < ACTIONS && strcmp(name, names[action]) != 0; action++)
			;
		actions[action_count].frame = frame;
		if (strcmp(name, "pause") == 0 || strcmp(name, "quit") == 0 || strcmp(name, "restart") == 0)
			actions[action_count].action = name[0] == 'p' ? ACTION_PAUSE : (name[0] == 'q' ? ACTION_QUIT : ACTION_RESTART);
		else if (action == ACTIONS)
		{
			fclose(input);
			return "An action is not one of up, down, left, right, accept, back, start, pause, quit, restart.";
		}
		else
			actions[action_count].action = ACTION_BIT(action);
		actions[action_count].player = read == 3 && player >= 1 && player <= CONSOLE_PLAYERS ? player - 1 : 0;
		action_count++;
	}
	fclose(input);
	return NULL;
}

/* The script's controls as the players' (the board's inputs it set, player N's on slot N), and the actions of
   the frame, into the console: it puts them on the board for the next frame */
static void console_inputs(void)
{
	uint8_t pressed[CONSOLE_PLAYERS], starts = 0;
	uint32_t menu_actions = 0;

	for (int player = 0; player < CONSOLE_PLAYERS; player++)
		pressed[player] = (uint8_t)~tmnt_board.players[player];
	for (size_t index = 0; index < action_count; index++)
		if (actions[index].frame == tmnt_board.frame)
		{
			if (actions[index].action == ACTION_PAUSE)
			{
				pause_shown = console_can_pause();
				continue;
			}
			if (actions[index].action == ACTION_QUIT || actions[index].action == ACTION_RESTART)
			{
				if (console_can_pause())
				{
					if (actions[index].action == ACTION_QUIT)
						console_quit_game();
					else
						console_restart_game();
				}
				pause_shown = false;
				continue;
			}
			menu_actions |= actions[index].action;
			if (actions[index].action == ACTION_BIT(ACTION_START))
				starts |= (uint8_t)(1u << actions[index].player);
		}
	console_actions(menu_actions);
	console_controls(pressed, starts);
}
#endif

/* --video-dump: a frame as a 24-bit BMP, as kref writes them ('width' pixels a row, 'stride' apart: WIDE's are wider) */
static void write_bmp(const char *path, const uint32_t *pixels, int width, int stride)
{
	const int height = TMNT_VIDEO_HEIGHT;
	const uint32_t row_bytes = (uint32_t)width * 3u, size = 54u + row_bytes * (uint32_t)height;
	uint8_t header[54] = { 'B', 'M' };
	FILE *output = fopen(path, "wb");

	if (!output)
	{
		fprintf(stderr, "%s: cannot be written\n", path);
		exit(1);
	}
	for (int byte = 0; byte < 4; byte++)
	{
		header[2 + byte] = (uint8_t)(size >> (8 * byte));
		header[18 + byte] = (uint8_t)((uint32_t)width >> (8 * byte));
		header[22 + byte] = (uint8_t)((uint32_t)height >> (8 * byte));
		header[34 + byte] = (uint8_t)((row_bytes * (uint32_t)height) >> (8 * byte));
	}
	header[10] = 54;
	header[14] = 40;
	header[26] = 1;
	header[28] = 24;
	fwrite(header, 1, sizeof(header), output);
	for (int y = height - 1; y >= 0; y--)
		for (int x = 0; x < width; x++)
		{
			const uint32_t pixel = pixels[y * stride + x];
			const uint8_t bgr[3] = { (uint8_t)pixel, (uint8_t)(pixel >> 8), (uint8_t)(pixel >> 16) };

			fwrite(bgr, 1, 3, output);
		}
	fclose(output);
}

static void dump_video(void)
{
	static tmnt_frame picture;
	char path[1100];

	for (int index = 0; index < video_dump_count; index++)
		if (video_dumps[index] == tmnt_board.frame)
		{
			picture = *game_frame();
#ifdef TMNT_CONSOLE
			console_overlay(&picture);
			if (pause_shown)
				console_pause_overlay(&picture, CONSOLE_PAUSE_CONTINUE);
			{
				char text[256];

				console_describe(text, sizeof(text));
				printf("frame %" PRIu64 ": %s\n", tmnt_board.frame, text);
			}
#endif
			snprintf(path, sizeof(path), "%s/frame_%" PRIu64 ".bmp", video_dir, tmnt_board.frame);
			{
				int width;
				const uint32_t *wide = tmnt_video_wide_frame(&width);

				if (width > TMNT_VIDEO_WIDTH)
					write_bmp(path, wide, width, TMNT_VIDEO_MAX_WIDTH);    /* (--wide in a game: the wider picture) */
				else
					write_bmp(path, picture.pixels, TMNT_VIDEO_WIDTH, TMNT_VIDEO_WIDTH);
			}
		}
}

/* The board's memories into dump_dir, a file each, as kref --dump writes them */
static void dump_memories(void)
{
	uint8_t work_ram[TMNT_WORK_RAM_SIZE];
	const struct { const char *name; const uint8_t *data; size_t size; } memories[] = {
		{ "work_ram", work_ram, sizeof(work_ram) },
		{ "palette", tmnt_board.palette, sizeof(tmnt_board.palette) },
		{ "tile_ram", tmnt_board.tile_ram, sizeof(tmnt_board.tile_ram) },
		{ "sprite_ram", tmnt_board.sprite_ram, sizeof(tmnt_board.sprite_ram) },
	};

	for (uint32_t offset = 0; offset < TMNT_WORK_RAM_SIZE; offset++)
		work_ram[offset] = *tmnt_work_ram_byte(TMNT_WORK_RAM + offset);
	for (size_t index = 0; index < sizeof(memories) / sizeof(memories[0]); index++)
	{
		char path[1024];
		FILE *file;

		snprintf(path, sizeof(path), "%s/%" PRIu64 "_%s.bin", dump_dir, tmnt_board.frame, memories[index].name);
		file = fopen(path, "wb");
		if (!file)
		{
			fprintf(stderr, "%s: cannot be written\n", path);
			exit(1);
		}
		fwrite(memories[index].data, 1, memories[index].size, file);
		fclose(file);
	}
}

static void on_frame_done(void)
{
	/* the controls change when the VBLANK starts, then the frame's state is taken */
	tmnt_script_apply(script, tmnt_board.frame);
#ifdef TMNT_CONSOLE
	console_inputs();
	console_wide_frame();    /* (--wide: from the next frame drawn) */
#endif
	game_end_frame();
	if (hashes)
		fprintf(hashes, "%" PRIu64 " %016" PRIx64 "\n", tmnt_board.frame, tmnt_state_hash());
	if (video_hashes)
		fprintf(video_hashes, "%" PRIu64 " %016" PRIx64 "\n", tmnt_board.frame, tmnt_video_hash(game_frame()));
	if (sound_hashes)
		fprintf(sound_hashes, "%" PRIu64 " %016" PRIx64 " %016" PRIx64 "\n", tmnt_board.frame,
			tmnt_sound_samples_hash(), tmnt_sound_state_hash());
	if (tmnt_board.frame == dump_frame)
		dump_memories();
	if (video_dump_count)
		dump_video();
	if (tmnt_board.frame >= frames_wanted)
		program_stop();
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

int main(int argc, char **argv)
{
	const char *hashes_path = NULL, *video_hashes_path = NULL, *sound_hashes_path = NULL;
	const char *script_path = NULL;
	const char *assets = NULL;
	const char *pack_path = NULL;
	bool frames_given = false;
	bool service = false;

	for (int index = 1; index < argc; index++)
	{
		if (strcmp(argv[index], "--frames") == 0 && index + 1 < argc)
		{
			frames_wanted = strtoull(argv[++index], NULL, 10);
			frames_given = true;
		}
		else if (strcmp(argv[index], "--hashes") == 0 && index + 1 < argc)
			hashes_path = argv[++index];
		else if (strcmp(argv[index], "--video-hashes") == 0 && index + 1 < argc)
			video_hashes_path = argv[++index];
		else if (strcmp(argv[index], "--sound-hashes") == 0 && index + 1 < argc)
			sound_hashes_path = argv[++index];
		else if (strcmp(argv[index], "--script") == 0 && index + 1 < argc)
			script_path = argv[++index];
		else if (strcmp(argv[index], "--assets") == 0 && index + 1 < argc)
			assets = argv[++index];
		else if (strcmp(argv[index], "--service") == 0)
			service = true;
		else if (strcmp(argv[index], "--dump") == 0 && index + 1 < argc)
			dump_frame = strtoull(argv[++index], NULL, 10);
		else if (strcmp(argv[index], "--dump-dir") == 0 && index + 1 < argc)
			dump_dir = argv[++index];
		else if (strcmp(argv[index], "--write-pack") == 0 && index + 1 < argc)
			pack_path = argv[++index];
		else if (strcmp(argv[index], "--video-dump") == 0 && index + 1 < argc)
		{
			while (index + 1 < argc && argv[index + 1][0] != '-' && video_dump_count < MAX_VIDEO_DUMPS)
				video_dumps[video_dump_count++] = strtoull(argv[++index], NULL, 10);
		}
		else if (strcmp(argv[index], "--wide-side") == 0 && index + 1 < argc)
		{
			const int side = atoi(argv[++index]);    /* (a check of the video's WIDE: the console sets its own) */

			tmnt_video_set_wide(side, side);
		}
		else if (strcmp(argv[index], "--video-dir") == 0 && index + 1 < argc)
			video_dir = argv[++index];
#ifdef TMNT_CONSOLE
		else if (strcmp(argv[index], "--arcade-start") == 0)
			console_arcade_test();
		else if (strcmp(argv[index], "--wide") == 0 && index + 1 < argc)
		{
			console_settings.wide = atoi(argv[++index]);    /* (OPTIONS > VIDEO > WIDE: 1 16:9, 2 20:9) */
			if (console_settings.wide < 0 || console_settings.wide >= WIDE_KINDS)
				console_settings.wide = 0;
		}
		else if (strcmp(argv[index], "--actions") == 0 && index + 1 < argc)
		{
			const char *problem = load_actions(argv[++index]);

			if (problem)
			{
				fprintf(stderr, "%s\n", problem);
				return 1;
			}
		}
#endif
		else
		{
			fprintf(stderr, USAGE);
			return 1;
		}
	}
	if (!assets)
	{
		/* (the folder 'assets', or where there is none the pack) */
		FILE *probe = fopen("assets/data/layout.txt", "rb");

		assets = "assets";
		if (probe)
			fclose(probe);
		else if ((probe = fopen(GAME_PACK_FILE, "rb")) != NULL)
		{
			fclose(probe);
			assets = GAME_PACK_FILE;
		}
	}
	{
		const char *problem = game_load(assets);

		if (problem)
		{
			fprintf(stderr, "The game's assets in %s cannot be read: %s\n", assets, problem);
			return 1;
		}
	}
	if (pack_path)
	{
		const char *problem = assets_pack_write(pack_path);

		if (problem)
		{
			fprintf(stderr, "%s: %s\n", pack_path, problem);
			return 1;
		}
		printf("the assets in %s packed into %s\n", assets, pack_path);
		return 0;
	}
	{
		const char *problem = game_reset(service, video_hashes_path != NULL || video_dump_count > 0);

		if (problem)
		{
			fprintf(stderr, "The game does not start: %s\n", problem);
			return 1;
		}
	}
#ifdef TMNT_CONSOLE
	{
		/* (its letters beside the executable) */
		char folder[1024];
		const char *slash = strrchr(argv[0], '/'), *backslash = strrchr(argv[0], '\\');
		const char *last = slash > backslash ? slash : backslash;

		snprintf(folder, sizeof(folder), "%.*s", last ? (int)(last - argv[0] + 1) : 0, argv[0]);
		if (!console_attach(folder))
			fprintf(stderr, "%sfont.bmp cannot be read: the menus have no letters\n", folder);
	}
#endif
	if (script_path)
	{
		char error[256];

		script = tmnt_script_load(script_path, error, sizeof(error));
		if (!script)
		{
			fprintf(stderr, "%s\n", error);
			return 1;
		}
		if (!frames_given && tmnt_script_frames(script))
			frames_wanted = tmnt_script_frames(script);
		tmnt_script_apply(script, 0);
	}
	if (hashes_path)
		hashes = open_output(hashes_path);
	if (video_hashes_path)
		video_hashes = open_output(video_hashes_path);
	if (sound_hashes_path)
		sound_hashes = open_output(sound_hashes_path);

	program_frame_done = on_frame_done;
	program_run();

	if (hashes)
		fclose(hashes);
	if (video_hashes)
		fclose(video_hashes);
	if (sound_hashes)
		fclose(sound_hashes);
	tmnt_script_free(script);
	printf("frames:            %" PRIu64 "\n", program_frames());
	printf("interrupts taken:  %u\n", program_interrupts());
	printf("sound commands:    %u\n", tmnt_board.sound_irqs);
	printf("watchdog resets:   %u\n", tmnt_board.watchdog_resets);
	return 0;
}
