/**
 *    Name:        main.c
 *    Description: runs the game with no screen or sound, with the controls a script drives, and writes hashes
 *                 of every frame in the formats tools/core/konami/reference/kref writes
 *    Version:     0.1.1
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: tmnt_headless [--frames N] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE] [--script FILE]
 *                         [--service] [--assets FOLDER or PACK] [--dump FRAME --dump-dir DIR] [--write-pack FILE]
 *    The game's data is its assets (game/assets.h), the files tools/extract_assets.py writes from the
 *    player's ROM set (or the game's first run, runtime/src/extract.c): --assets (a folder, or a pack: a file
 *    whose name ends in .assets), or the folder 'assets', or where there is none the pack tmnt.assets, in the
 *    current folder. The game opens no ROM. --write-pack writes the pack of the assets loaded and stops.
 *
 *    --dump writes the board's memories at the end of frame FRAME into DIR as kref --dump does (FRAME_work_ram.bin,
 *    in the order of the addresses, and the palette, the tile RAM and the sprite RAM), to compare them.
 *
 *    The board runs with frame timing, as kref does by default (tmnt/frame.h), the script is applied when each
 *    VBLANK starts, and the hashes are kref's ("FRAME HASH": tmnt_state_hash, the frame's pixels with
 *    --video-hashes, the sound board's samples and RAM with --sound-hashes), so the two compare line by line. --service turns the test mode's DIP switch on.
 */
#include "game/assets.h"
#include "game/game.h"
#include "tmnt/machine.h"
#include "tmnt/program.h"
#include "tmnt/script.h"
#include "tmnt/sound_board.h"
#include "tmnt/video.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "Usage: tmnt_headless [--frames N] [--hashes FILE] [--video-hashes FILE] [--sound-hashes FILE]" \
	" [--script FILE] [--service] [--assets FOLDER or PACK] [--dump FRAME --dump-dir DIR] [--write-pack FILE]\n"

static uint64_t frames_wanted = 600;
static FILE *hashes;
static FILE *video_hashes;
static FILE *sound_hashes;
static tmnt_script *script;
static uint64_t dump_frame = UINT64_MAX;
static const char *dump_dir = ".";

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
		const char *problem = game_reset(service, video_hashes_path != NULL);

		if (problem)
		{
			fprintf(stderr, "The game does not start: %s\n", problem);
			return 1;
		}
	}
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
