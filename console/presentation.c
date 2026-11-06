/**
 *    Name:        presentation.c
 *    Description: the console's Konami presentation, before the title at power-on: the ribbon and KONAMI large on
 *                 black, unseen until the four turtles run across and light them up, then a sound of the game's
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As outrunners-port's SEGA presentation (its console/presentation.c). The logo is the fixed layer's larger
 *    Konami logo, the ribbon and KONAMI 16 pixels high (game text 16, which no screen of the game shows; not the
 *    "(C) KONAMI 1989" under the smaller one, which is the font's text), drawn twice its size from the player's
 *    assets in the colours of the palette's first bank (the one the coin's screen prints its logo with). The
 *    turtles are the game's objects (the turtles' slots, 0-3), each with its own walk from turtle_animation_sets
 *    (routine 1, state 0, as turtle_walk sets it), drawn by the game's sprite engine and stepped on by it, with
 *    area 0's palettes (the turtles' own in the sprites' first four banks, as an area's play has them); the
 *    console draws them again over the logo, which is black until they pass (Sebas: it showed grey before). The
 *    sound is the coin's (PCM effect 86), which Sebas chose for it over speech A2 (that one went to the menus).
 */
#include "console_internal.h"

#include "game.h"

#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       224
#define LOGO_TEXT           16u        /* game_texts' larger Konami logo */
#define LOGO_SCALE          2
#define LOGO_BANK           0          /* the palette's first bank: the coin's screen's logo colours */
#define LOGO_DARK           0          /* of 256: the letters not lit yet, black */
#define LOGO_TOP            72
#define TURTLES             4
#define TURTLE_SPACING      34         /* pixels between the turtles as they run */
#define TURTLE_SPEED        3          /* pixels a frame */
#define TURTLE_GROUND       0x58       /* their screen_y: their feet under the logo's middle */
#define TURTLE_LIGHT        16         /* the logo is lit up to this far past the leading turtle's place */
#define SCREEN_LEFT         96         /* the sprites' x of the screen's first column */
#define WALK_ROUTINE        1u
#define WALK_STATE          0u
#define VOICE               0x86u      /* the coin's sound (PCM effect 86) */
#define AREA_PALETTES       0x10Fu     /* a palette request: the area's set (area 0's at power-on), its four banks */
#define FADE_FRAMES         16

/* The scene's frames (the game's 60 a second) */
#define AT_TURTLES          30
#define AT_VOICE_AFTER      12         /* frames after the last turtle is off the screen */
#define HOLD_AFTER_VOICE    90
#define AT_END_MOST         600        /* (at the latest) */

static struct
{
	int frame;
	int part;                  /* 0 setting up, 1 running, 2 lit, 3 fading out */
	int part_frame;
	int lit_to;                /* the logo is lit left of here */
	int brightness;            /* 0-256 */
} show;

static int logo_left(void)
{
	int width, height;

	game_text_size(LOGO_TEXT, &width, &height);
	return (SCREEN_WIDTH - width * LOGO_SCALE) / 2;
}

/* The four turtles at the left of the screen, a turtle's spacing apart, Leonardo leading */
static void start_turtles(void)
{
	for (int turtle = 0; turtle < TURTLES; turtle++)
	{
		const uint32_t object = OBJECT_ADDRESS(turtle);

		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->kind = (uint8_t)turtle;
		OBJECT(object)->routine = WALK_ROUTINE;
		OBJECT(object)->state = WALK_STATE;
		OBJECT(object)->screen_x = (uint16_t)(SCREEN_LEFT - 48 - turtle * TURTLE_SPACING);
		OBJECT(object)->screen_y = (uint16_t)(TURTLE_GROUND + (turtle & 1) * 6);
		turtle_set_animation(0u, object);
	}
}

/* The turtles a step on; false once they are all past the screen's right */
static bool run_turtles(void)
{
	bool running = false;

	for (int turtle = 0; turtle < TURTLES; turtle++)
	{
		const uint32_t object = OBJECT_ADDRESS(turtle);
		const int x = (int16_t)OBJECT(object)->screen_x + TURTLE_SPEED;

		OBJECT(object)->screen_x = (uint16_t)x;
		if (x - SCREEN_LEFT + TURTLE_LIGHT > show.lit_to)
			show.lit_to = x - SCREEN_LEFT + TURTLE_LIGHT;
		if (x - SCREEN_LEFT < SCREEN_WIDTH + 48)
			running = true;
		else
			OBJECT(object)->flags = 0u;
	}
	return running;
}

bool presentation_frame(uint32_t pressed)
{
	uint32_t ignored = 0;

	if (show.part == 4)
		return false;
	show.frame++;
	show.part_frame++;
	if (show.frame == 1)
	{
		/* as an attract step clears its screen (the play's work, the layers), with area 0's palettes (the
		   turtles'), from black */
		clear_play();
		jobs_pending++;
		queue_job(7u);
		clear_palette_effects();
		post_palette_request(AREA_PALETTES, 0u);
		hide_all_sprites();
		show.lit_to = logo_left();
	}
	if ((pressed & (ACTION_BIT(ACTION_START) | ACTION_BIT(ACTION_ACCEPT))) && show.part < 3)
	{
		show.part = 3;    /* (skipped: the fade out at once) */
		show.part_frame = 0;
	}
	switch (show.part)
	{
		case 0:
			show.brightness = 0;
			if (show.frame >= AT_TURTLES && jobs_pending == 0u && palettes_busy == 0u)
			{
				start_turtles();
				show.part = 1;
				show.part_frame = 0;
			}
			break;
		case 1:
			show.brightness = show.part_frame >= FADE_FRAMES ? 256 : show.part_frame * 256 / FADE_FRAMES;
			if (!run_turtles())
			{
				show.lit_to = SCREEN_WIDTH;
				show.part = 2;
				show.part_frame = 0;
			}
			break;
		case 2:
			show.brightness = 256;
			if (show.part_frame == AT_VOICE_AFTER)
				queue_sound_command(VOICE);
			if (show.part_frame >= AT_VOICE_AFTER + HOLD_AFTER_VOICE)
			{
				show.part = 3;
				show.part_frame = 0;
			}
			break;
		case 3:
			show.brightness = show.part_frame >= FADE_FRAMES ? 0 : 256 - show.part_frame * 256 / FADE_FRAMES;
			if (show.part_frame >= FADE_FRAMES)
			{
				show.part = 4;
				clear_objects();
				hide_all_sprites();
				return false;
			}
			break;
	}
	if (show.frame >= AT_END_MOST && show.part < 3)
	{
		show.part = 3;
		show.part_frame = 0;
	}
	if (show.part == 1 || show.part == 2 || show.part == 3)
	{
		/* (the game's sprite engine: the turtles' animations stepped on, their sprites into the list) */
		build_draw_list();
		draw_sprites(ignored);
	}
	return true;
}

void presentation_overlay(console_picture *frame)
{
	draw_fill(frame, PICTURE_FIRST(frame), 0, PICTURE_END(frame) - PICTURE_FIRST(frame), SCREEN_HEIGHT, 0x000000u);
	if (show.part == 0)
		return;
	draw_game_text(frame, LOGO_TEXT, logo_left(), LOGO_TOP, LOGO_SCALE, LOGO_BANK, show.lit_to, LOGO_DARK);
	draw_board_sprites(frame);
	if (show.brightness < 256)
		draw_shade(frame, PICTURE_FIRST(frame), 0, PICTURE_END(frame) - PICTURE_FIRST(frame), SCREEN_HEIGHT,
			show.brightness);
}
