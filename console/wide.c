/**
 *    Name:        wide.c
 *    Description: the console's WIDE (OPTIONS > VIDEO): the picture wider than the board's 320 pixels in a game's
 *                 play and in the menus, the game's sprites and layers' columns written for the wider screen, and its
 *                 enemies coming in and going out past its sides
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    10-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As outrunners-port's WIDE (16:9, 48 columns more at each side; 20:9, 88: the K051960's 9-bit x and the
 *    K052109's 512-pixel maps leave no room for more). The video draws the layers' maps and the sprites there
 *    (tmnt_video_set_wide, which the port sets each frame from console_wide_sides), and the console draws its own
 *    picture over the whole of it (console_picture: the menus, the pause); in a game's play and on the menus' screens
 *    (Sebas: "el menu tmb tiene que tener wide"), the presentation, the story screens, the continue, the ending and
 *    the ranking keeping the board's 320 between black bars (their pictures were never drawn wider).
 *
 *    The game writes into the K051960 only the pieces of its objects that show on its 320 columns (draw_sprites:
 *    the piece's left before 0x1A0, the screen's right edge, and past its size's own left, so a pixel shows): with
 *    WIDE the console's version writes those that show on the wider screen too, the same pieces in the same order
 *    otherwise, at most the 128 the sprite RAM holds; but in the sides not those of a sprite bank the game has lent
 *    to another object's palette, their object left behind past the board's left edge (lent_banks_frame: the parked
 *    cars' barrel in Bebop's colours). Its work on the objects (their animations stepped on) and what it leaves in the
 *    registers the game carries (screen_x_high) are the game's own.
 */
#include "console_internal.h"

#include "game.h"
#include "tmnt/video.h"

#include <stdio.h>
#include <string.h>

#define SPRITE_ENTRIES_END  (SPRITE_RAM + 0x400u)    /* the sprite RAM's 128 entries of 8 bytes */
#define SCREEN_LEFT         0x60                     /* the K051960's x of the screen's left edge */
#define SCREEN_RIGHT        0x1A0                    /* (and of its right edge) */
#define SCENE_PLAY          4u                       /* the game's scenes WIDE shows: the play and the area's exit */
#define SCENE_EXIT          5u
#define SCENE_CONTINUE      7u                       /* (and the continue's countdown, over the play) */
#define LAYERS_DX           6                        /* a map's pixel on the screen: its x less the layer's x, plus 6
                                                        (the K052109's -90 and the program's scroll, x - 0x60) */
#define BLOCK_PIXELS        16                       /* a map's row: map_width bytes, 2 a block of 32 pixels */
#define RING_PIXELS         512                      /* a map no wider than the K052109's ring repeats itself */
#define BOXES_ROWS          32                       /* the players' boxes: the fixed layer's first 32 rows on the
                                                        screen, kept in the middle of the wider picture whatever its
                                                        sides (a player's report, on Reddit: the boxes did not stay
                                                        in the middle at an area's start, its sides all at the right) */
#define MESSAGE_ROWS        16                       /* (and in a play the line under them: PRESS START, GAME OVER) */
#define LEAST(a, b)         ((a) < (b) ? (a) : (b))
#define MOST(a, b)          ((a) > (b) ? (a) : (b))

/* The layers the game draws as maps of blocks in the area being played, A 1 and B 2: those it draws whole
   (start_area_layers' start_layers_..., an exit's picture) or column by column as the scroll goes. The other layer's
   record keeps what an earlier area or screen left in it, no map of this one (an area of one map, as the street's):
   WIDE takes no room, ends nor columns from it */
static unsigned layer_maps;
static int drawn_first[2], drawn_last[2], drawn_y[2];
static bool drawn_known[2];    /* (the ring's drawn columns: draw_shown_columns; at the height drawn_y) */
static bool area_started;    /* (start_area_layers done for the area being played: its layers drawn) */

/* A map the game loops (the skateboards' road: every 90 pixels its x goes back to show the same again) has no ends
   for WIDE either: once its x has gone back by more than a scroll's step it takes no room (Sebas: "se mueve raro",
   the picture moved at each loop). The Technodrome's lift goes round its shaft once through the ride, a jump of its
   x back and its y on (technodrome_ride_lift), no map that loops: its columns past the board's drawn from the map at
   each frame (draw_shown_columns: the right side's last ones came late as the picture panned), and off the lift its
   map has its ends (the boss's room, its wall at the right) */
#define LOOP_STEP           32
static unsigned looping_maps;
static int looping_last_x[2];

/* And each one's first column the area shows: the board's column 0 where the scroll starts it (the map's columns
   before it, which the arcade never shows, may be blank: area 0's 32 of nothing before the building). The picture's
   left side shows no column before it: at an area's start the wider screen's columns are all at the right (Sebas:
   "correr todo para la derecha para que no aparezca"), back to the middle as the scroll brings the map */
static int layer_first[2];

/* At an area's start the picture's first columns may be the board's first 6, which the arcade never shows (the
   K052109 shows layers A and B 6 pixels further than the fixed layer, and there the end of their maps; the cabinet's
   bezel hid them, the window in 4:3 leaves them out): with WIDE all at the right they are at the picture's left
   edge, black (in 16:9 the window's own crop leaves them out; 20:9 shows its whole picture) */
static int wide_hidden;

static void layer_is_map(int layer)
{
	layer_maps |= 1u << layer;
}

static bool layer_has_map(int layer)
{
	return (layer_maps & (1u << layer)) != 0;
}

/* The game's start_area_layers (stage/layers.c): the area's layers, none of them a map before it draws them */
void start_area_layers(void)
{
	layer_maps = 0;
	looping_maps = 0;
	drawn_known[0] = drawn_known[1] = false;
	original_start_area_layers();
	area_started = true;
	looping_last_x[0] = (int)((LAYER(LAYER_A)->x >> 8) & 0xFFFFu);
	looping_last_x[1] = (int)((LAYER(LAYER_B)->x >> 8) & 0xFFFFu);
	layer_first[0] = (int)((LAYER(LAYER_A)->x >> 8) & 0xFFFFu) - LAYERS_DX;
	layer_first[1] = (int)((LAYER(LAYER_B)->x >> 8) & 0xFFFFu) - LAYERS_DX;
}

/* An area's exit's picture (exit_cutscene set: April carried off, the villains with April, April's hug, the
   skateboards' crash, Splinter's thanks and the turtles' drop before the Technodrome): the game clears the layers and
   draws its picture into them from maps of its own, the layers' records left with the area's map and a place of 0.
   WIDE takes nothing from them (no room, no ends, no columns: the area's map's columns drawn over the picture's,
   Sebas: "se rompio la animacion"), the picture in the middle, its sides painted with its background's colour
   (wide_picture_sides) */
static bool exit_picture(void)
{
	return console.page == PAGE_GAME && game_state == GAME_STATE_GAME && scene == SCENE_EXIT && exit_cutscene != 0u;
}

enum { PICTURE_MIDDLE, PICTURE_AT_LEFT, PICTURE_AT_RIGHT };
static int picture_place = PICTURE_MIDDLE;    /* (where it goes: read_picture_place) */

/* The menus' pages (Sebas: "el menu tmb tiene que tener wide"): their screens with the columns more at both sides */
static bool menu_page_wide(void)
{
	return game_state == GAME_STATE_ATTRACT && console.page != PAGE_PRESENTATION && console.page != PAGE_GAME;
}

/* In a game's play, and in the menus: the story screens, the ending and the ranking keep the board's 320 (their
   pictures were never drawn wider), and the presentation, on black. The continue's countdown and its GAME OVER keep the
   play's (they are drawn over it, the play going on under them: a player's report, the picture back to 4:3 as his
   last life went). In a game the picture has its columns
   more at both sides, but where a side has no map past the board's screen (an area's start at its left, its end at
   its right), the picture shows as much of it as there is and the rest at the other side (Sebas: "recorta de la
   izquierda y corre"): the game is the same, only what the picture shows of it moves, back to the middle as the
   scroll brings the map */
/* The picture's sides in a game's play, as they come to be (wide_sides_now: console_wide_frame moves them there) */
/* The Technodrome's lift (update_layers_technodrome's ride): layer B, the shaft, scrolls on the diagonal (its x on as
   its y back) and goes round it once, and its map past the shaft's band, which the arcade's screen never shows, is
   black blocks: below the band they went by in the left side's bottom corner over and over, up to 75 pixels in;
   above it, at the shaft's end as the lift slows, in the right side's top (Sebas: "corrélo", "arregla lo del pozo").
   Through the ride the picture keeps off them: its sides no wider than where the black is, at the scroll's place
   now and where it will be in the next LIFT_AHEAD_FRAMES (the shaft's map read along its way, the loop to come
   counted), the rest at the other side, and what no side can take black at both (lift_bar: in 20:9 the band is
   narrower than its picture). The sides and their bars move there by WIDE_PAN a frame, as the picture pans */
#define TECHNODROME_STAGE   8u
#define LIFT_RIDE_STEP      1u
#define LIFT_AHEAD_FRAMES   80
#define LIFT_LOOP_X         0xA28    /* (technodrome_ride_lift: there, with its loop to come, x goes back 0x400, y on) */
#define LIFT_LOOP           0x400
#define SCREEN_TOP          16       /* the K052109's first row shown (the board's 224 from it): layer B's y less it */
#define TILE_PIXELS         8
static int lift_bar_left, lift_bar_right;            /* (where they come to be) */
static int lift_bar_left_now, lift_bar_right_now;    /* (and now) */

/* The black tiles of a block of the shaft's map (bit 4 * row + column): its blocks past the band, all of one tile, and
   those of its edges, part of it */
#define LIFT_CACHE          256
static struct { uint16_t entry, black; bool known; } lift_cache[LIFT_CACHE];
static uint32_t lift_cache_blocks;

static unsigned lift_black_tiles(uint32_t blocks, uint32_t entry)
{
	const unsigned slot = (entry ^ (entry >> 8)) & (LIFT_CACHE - 1u);
	uint8_t attribute;
	unsigned black = 0;

	if (blocks != lift_cache_blocks)
	{
		memset(lift_cache, 0, sizeof(lift_cache));
		lift_cache_blocks = blocks;
	}
	if (lift_cache[slot].known && lift_cache[slot].entry == entry)
		return lift_cache[slot].black;
	attribute = BLOCK_BYTE(blocks + (int16_t)entry);
	for (unsigned tile = 0; tile < 16u; tile++)
	{
		const uint8_t code = BLOCK_BYTE(blocks + entry + 1u + tile);

		if ((attribute == 0x14u && code == 0x2Du) || (attribute == 0x04u && code == 0xF8u))
			black |= 1u << tile;
	}
	lift_cache[slot].entry = (uint16_t)entry;
	lift_cache[slot].black = (uint16_t)black;
	lift_cache[slot].known = true;
	return black;
}

/* The room the sides have through the ride: no further than the shaft's first black tile past the board's columns,
   in the screen of the scroll's place now and of those to come (8 pixels apart on its way). A map's row of blocks is
   32 pixels of the shaft's height, its first row's tiles the highest: row r holds the heights 32 r + 1 to 32 r + 32,
   and the screen's first row shows the height y - SCREEN_TOP */
static void lift_room(int *room_left, int *room_right)
{
	const uint32_t record = LAYER_B, blocks = LAYER(record)->blocks;
	const int map_pixels = LAYER(record)->map_width * BLOCK_PIXELS;
	const int ahead = (int)LAYER(record)->x_speed * LIFT_AHEAD_FRAMES >> 8;
	const int x = (int)((LAYER(record)->x >> 8) & 0xFFFFu), y = (int)((LAYER(record)->y >> 8) & 0xFFFFu);

	for (int on = 0; on <= ahead; on += TILE_PIXELS)
	{
		int place_x = x + on, place_y = y - on;
		int first, top, bottom;

		if (layer_loops >= 0 && place_x >= LIFT_LOOP_X)
		{
			place_x -= LIFT_LOOP;
			place_y += LIFT_LOOP;
		}
		first = place_x - LAYERS_DX;           /* (the map's pixel at the board's column 0) */
		top = place_y - SCREEN_TOP;            /* (the heights the screen shows, its first row's and its last's) */
		bottom = top - (TMNT_VIDEO_HEIGHT - 1);
		for (int row = (top - 1) / 32; row >= 0 && 32 * row + 32 >= bottom; row--)
		{
			const int column_from = (first - *room_left) / 32;
			const int column_to = (first + TMNT_VIDEO_WIDTH - 1 + *room_right) / 32;

			for (int column = column_from < 0 ? 0 : column_from; column <= column_to && column * 32 < map_pixels; column++)
			{
				const uint32_t map = (uint32_t)row * LAYER(record)->map_width + (uint32_t)column * 2u + LAYER(record)->map;
				const unsigned black = lift_black_tiles(blocks, MAP_ENTRY(map));

				for (unsigned tile = 0; black && tile < 16u; tile++)
				{
					const int tile_top = 32 * row + 32 - TILE_PIXELS * (int)(tile >> 2);    /* (its highest row) */
					const int board_x = 32 * column + TILE_PIXELS * (int)(tile & 3u) - first;

					if (!(black & (1u << tile)) || tile_top - (TILE_PIXELS - 1) > top || tile_top < bottom)
						continue;    /* (not black, or not in the screen's rows) */
					if (board_x + TILE_PIXELS - 1 >= TMNT_VIDEO_WIDTH)
						*room_right = LEAST(*room_right, board_x - TMNT_VIDEO_WIDTH < 0 ? 0 : board_x - TMNT_VIDEO_WIDTH);
					if (board_x < 0)
						*room_left = LEAST(*room_left, -(board_x + TILE_PIXELS) < 0 ? 0 : -(board_x + TILE_PIXELS));
				}
			}
		}
	}
}

static void wide_sides_target(int *left, int *right)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	static const int sides[WIDE_KINDS] = { 0, 48, 88, 32 };
	int side, room_left = 0x10000, room_right = 0x10000, more;

	*left = *right = 0;
	if (console_settings.wide < 0 || console_settings.wide >= WIDE_KINDS)
		return;
	side = sides[console_settings.wide];
	if (menu_page_wide())
	{
		*left = *right = side;
		return;
	}
	wide_hidden = 0;
	if (game_state != GAME_STATE_GAME || scene < SCENE_PLAY)
		area_started = false;    /* (an area's set-up, its title, a story: its layers to come, at its play's entry) */
	if (console.page != PAGE_GAME || game_state != GAME_STATE_GAME
		|| (scene != SCENE_PLAY && scene != SCENE_EXIT && scene != SCENE_CONTINUE) || !area_started)
		return;    /* (the board's 320 until the area has drawn its layers: Sebas, "no arranca bien del borde") */
	lift_bar_left = lift_bar_right = 0;
	if (exit_picture())
	{
		*left = picture_place == PICTURE_AT_LEFT ? 0 : picture_place == PICTURE_AT_RIGHT ? 2 * side : side;
		*right = 2 * side - *left;
		return;
	}
	for (int layer = 0; layer < 2; layer++)
	{
		const int width = LAYER(records[layer])->map_width * BLOCK_PIXELS;
		const int first = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu) - LAYERS_DX;    /* (the board's column 0) */
		const int from_start = first - (layer_first[layer] + LEFT_STRIPE);    /* (the arcade's first column shown) */

		if (width <= RING_PIXELS || !layer_has_map(layer) || (looping_maps & (1u << layer)))
			continue;    /* (a map no wider than the ring repeats itself: no ends, nor one the game loops; a record of no
			                map, none. Area 7's girders, 512 pixels, go round and round as its scroll goes: taken for a
			                room, the picture went all to the left, Sebas: "en el 8 se movio mucho la barra") */
		room_left = LEAST(room_left, from_start < 0 ? 0 : from_start);
		if (-from_start > wide_hidden)
			wide_hidden = -from_start;
		room_right = LEAST(room_right, width - (first + TMNT_VIDEO_WIDTH) < 0 ? 0 : width - (first + TMNT_VIDEO_WIDTH));
		if (last_segment != 0u)
		{
			/* the area's last section (its boss's): no further than the scroll goes there (x_min, x_max), which the
			   arcade's screen never passes; the map past it may be anything (the first boss's apartment and
			   Krang's room go on in another copy of the room: Sebas, "hay una fina linea", "se sigue yendo de
			   margen") */
			const int x = first + LAYERS_DX;
			const int x_min = (int)((LAYER(records[layer])->x_min >> 8) & 0xFFFFu);
			const int x_max = (int)((LAYER(records[layer])->x_max >> 8) & 0xFFFFu);

			if (x >= x_min && x <= x_max)    /* (the scroll's: its layer inside them. Layer A off the Technodrome's
			                                    lift, 2560 pixels before layer B, carries B's) */
			{
				room_left = LEAST(room_left, x - x_min);
				room_right = LEAST(room_right, x_max - x);
			}
		}
	}
	*left = LEAST(side, room_left);
	*right = LEAST(side, room_right);
	more = 2 * side - *left - *right;
	*right += LEAST(more, room_right - *right);
	more = 2 * side - *left - *right;
	*left += LEAST(more, LEAST(room_left, TMNT_VIDEO_MAX_LEFT) - *left);
	if (*left)
		wide_hidden = 0;
	if (stage == TECHNODROME_STAGE && layers_step == LIFT_RIDE_STEP && scene == SCENE_PLAY && (*left || *right))
	{
		const int shown = *left + *right;
		int lift_left = LEAST(room_left, TMNT_VIDEO_MAX_LEFT), lift_right = LEAST(room_right, shown), bars;

		lift_room(&lift_left, &lift_right);
		*left = LEAST(*left, lift_left);
		*right = LEAST(*right, lift_right);
		*right += LEAST(shown - *left - *right, lift_right - *right);    /* (what a side cannot take, the other) */
		*left += LEAST(shown - *left - *right, lift_left - *left);
		bars = shown - *left - *right;
		lift_bar_left = bars / 2;
		lift_bar_right = bars - bars / 2;
		*left += lift_bar_left;
		*right += lift_bar_right;
	}
}

/* The sides now: the menus' at once, a game's play's moving to where they come to be by WIDE_PAN a frame (the area's
   last section takes them to the other side: the picture pans there, as a camera, rather than jumping), at once when
   WIDE's sides come */
#define WIDE_PAN            4
static int wide_left_now, wide_right_now;

void console_wide_sides(int *left, int *right)
{
	if (menu_page_wide() || console.page != PAGE_GAME)
	{
		wide_sides_target(left, right);
		return;
	}
	*left = wide_left_now;
	*right = wide_right_now;
}

static int pan_to(int now, int target)
{
	return now + (target > now ? LEAST(WIDE_PAN, target - now) : -LEAST(WIDE_PAN, now - target));
}

static void wide_sides_frame(void)
{
	int left, right;

	wide_sides_target(&left, &right);
	if (!left && !right)
		wide_left_now = wide_right_now = lift_bar_left_now = lift_bar_right_now = 0;
	else if ((!wide_left_now && !wide_right_now) || exit_picture())
	{
		wide_left_now = left;    /* (the sides come: at once; an exit's picture in the middle at once, as the game
		                            clears the screen for it) */
		wide_right_now = right;
		lift_bar_left_now = lift_bar_left;
		lift_bar_right_now = lift_bar_right;
	}
	else
	{
		wide_left_now = pan_to(wide_left_now, left);
		wide_right_now = pan_to(wide_right_now, right);
		lift_bar_left_now = pan_to(lift_bar_left_now, lift_bar_left);
		lift_bar_right_now = pan_to(lift_bar_right_now, lift_bar_right);
	}
}

int console_wide_hidden_left(void)
{
	return console.page == PAGE_GAME && lift_bar_left_now > wide_hidden ? lift_bar_left_now : wide_hidden;
}

int console_wide_hidden_right(void)
{
	return console.page == PAGE_GAME ? lift_bar_right_now : 0;
}

/* An exit's picture's edges (the first column the K052109 shows its layers at, LEFT_STRIPE, and the last), in a
   frame the video drew ('pixels' its board's column 0): the colour most of their pixels have, and whether each edge
   is all of it (its background) or half of it (what the picture shows there goes on past the screen: the turtle's
   arm in April's hug, at the left) */
#define EDGE_COLOURS        8
#define EDGE_ALL(count)     ((count) * 20 >= TMNT_VIDEO_HEIGHT * 19)
#define EDGE_HALF(count)    ((count) * 2 >= TMNT_VIDEO_HEIGHT)

typedef struct
{
	uint32_t colour;
	bool left_all, right_all, left_half, right_half;
} picture_edges;

static picture_edges read_edges(const uint32_t *pixels, int stride)
{
	uint32_t colours[EDGE_COLOURS];
	int counts[EDGE_COLOURS][2], kinds = 0, best = 0;
	picture_edges edges = { 0 };

	for (int y = 0; y < TMNT_VIDEO_HEIGHT; y++)
		for (int edge = 0; edge < 2; edge++)
		{
			const uint32_t colour = pixels[y * stride + (edge ? TMNT_VIDEO_WIDTH - 1 : LEFT_STRIPE)];
			int kind = 0;

			while (kind < kinds && colours[kind] != colour)
				kind++;
			if (kind == kinds)
			{
				if (kinds == EDGE_COLOURS)
					continue;
				colours[kinds] = colour;
				counts[kinds][0] = counts[kinds][1] = 0;
				kinds++;
			}
			counts[kind][edge]++;
		}
	for (int kind = 1; kind < kinds; kind++)
		if (counts[kind][0] + counts[kind][1] > counts[best][0] + counts[best][1])
			best = kind;
	if (!kinds)
		return edges;
	edges.colour = colours[best];
	edges.left_all = EDGE_ALL(counts[best][0]);
	edges.right_all = EDGE_ALL(counts[best][1]);
	edges.left_half = EDGE_HALF(counts[best][0]);
	edges.right_half = EDGE_HALF(counts[best][1]);
	return edges;
}

/* Where the picture goes: in the middle, its sides its background (Splinter's yellow, the black about April carried
   off), or black where its edges are not (the villains' picture, the Technodrome the turtles drop before); at the
   window's left edge when what it shows goes on past its left (April's hug: Sebas, its turtle's arm cut there and
   the pink beyond), its background all at the right; at its right edge the other way. Read from the last frame drawn,
   at each frame (console_wide_frame) */
static int place_of(picture_edges edges)
{
	if (edges.right_all && !edges.left_all && edges.left_half)
		return PICTURE_AT_LEFT;
	if (edges.left_all && !edges.right_all && edges.right_half)
		return PICTURE_AT_RIGHT;
	return PICTURE_MIDDLE;
}

static void read_picture_place(void)
{
	int width;
	const uint32_t *pixels = tmnt_video_wide_frame(&width);

	picture_place = exit_picture() ? place_of(read_edges(pixels + tmnt_video_wide_left(), TMNT_VIDEO_MAX_WIDTH))
		: PICTURE_MIDDLE;
}

/* Its sides, over the frame the video drew: its background's colour at an edge all of it, else black */
void wide_picture_sides(console_picture *frame)
{
	picture_edges edges;

	if (!exit_picture() || (!frame->left && !frame->right))
		return;
	edges = read_edges(frame->pixels + frame->left, frame->stride);
	draw_fill(frame, PICTURE_FIRST(frame), 0, LEFT_STRIPE - PICTURE_FIRST(frame), TMNT_VIDEO_HEIGHT,
		edges.left_all ? edges.colour : 0x000000u);
	draw_fill(frame, TMNT_VIDEO_WIDTH, 0, frame->right, TMNT_VIDEO_HEIGHT, edges.right_all ? edges.colour : 0x000000u);
}

/* WIDE's room for the turtles (wide_turtles.c): the sides, no further than where the scroll's segment lets the
   picture go (the layers' x_min and x_max, which the game's scroll keeps to): in a room the scroll holds (Krang's)
   the turtles keep the arcade's limits (Sebas: "necesita un tope, porque se pasa"), where it goes on they walk the
   sides */
void console_wide_turtle_room(int *left, int *right)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };

	console_wide_sides(left, right);
	if (boss_rush_crossing())
	{
		*left = *right = 0;    /* (BOSS RUSH's crossing, out of sight: the arcade's limits, the scroll pushed where the
		                          arcade's turtles push it; in the sides the street's turn's wall held them short of
		                          it: Sebas, "lo de bebop no lo arreglaste, empieza muy atras") */
		return;
	}
	if (console.page == PAGE_GAME)
	{
		*left -= LEAST(*left, lift_bar_left_now);    /* (the lift's bars: not to be walked) */
		*right -= LEAST(*right, lift_bar_right_now);
	}
	for (int layer = 0; layer < 2 && (*left || *right); layer++)
	{
		const int x = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu);
		const int x_min = (int)((LAYER(records[layer])->x_min >> 8) & 0xFFFFu);
		const int x_max = (int)((LAYER(records[layer])->x_max >> 8) & 0xFFFFu);

		if (!layer_has_map(layer) || (looping_maps & (1u << layer)) || x < x_min || x > x_max)
			continue;    /* (limits not the layer's: wide_sides_target) */
		*left = LEAST(*left, x - x_min);
		*right = LEAST(*right, x_max - x);
	}
}

/* The video's WIDE for the next frame: its sides, and where layers A and B have a map (an area's start shows past the
   map's left end, its end past its right: their ring holds other columns there) */
static void rebuild_layers(int left, int right);
static void read_picture_place(void);

/* The sprite banks the game lends: in a play it loads a palette of its own into one now and then
   (palette_request_palette, a request of one palette for one bank: a boss's as he comes, Bebop's into bank 14 at the
   parked cars, Rocksteady's into 10, Stockman's into 12), a bank the area's scenery may have drawn with until then:
   the game lends it when the arcade's screen no longer shows them. At the parked cars' arena the second DANGER barrel
   (its pieces' own bank 14) is past the board's left edge but for a strip, the scroll held through Bebop and
   Rocksteady's fight; WIDE's left side showed it whole, in Bebop's pink and red (issue #3). An object whose place is
   past the board's left edge (the scroll has left it behind: the arcade's screen does not show it again) when a bank
   it drew with is lent has its pieces with that bank drawn only where they lie mostly on the board's 320, as the
   arcade's; not in WIDE's sides. The one the bank is lent to keeps it: Bebop had not drawn with it (he flashes with it
   when weak), and Stockman, who had, waits for his fight past the board's right edge */
#define OBJECTS             50
#define PALETTE_ONE_BANK    0x0400u    /* (a palette request's kind: its type bits 0x3F00; its low byte the bank's group */
#define PALETTE_TYPE_BITS   0x3F00u    /* and number, 0x30 to 0x3F the sprites' 16) */
#define SPRITE_BANKS        0x30u
#define BANK_GROUP_BITS     0xF0u
#define BANK_BITS           0x0Fu
#define REQUEST_BYTES       6u
#define PALETTE_REQUESTS    40         /* (the game's queue of them, a ring) */
static uint16_t banks_drawn[OBJECTS];        /* (each object's banks, in the frames before this one) */
static uint16_t banks_drawn_now[OBJECTS];    /* (in this one) */
static uint16_t banks_lent[OBJECTS];         /* (lent since it drew with them, the scroll past it) */
static uint32_t palette_queue_seen;          /* (the game's palette_queue_write at the last frame's end) */

/* At a frame's end: the palette requests the game queued in it (each is there until the next frames' VBLANKs run
   them, a bank a frame), and the objects gone (their slots taken again by others) */
static void lent_banks_frame(void)
{
	uint32_t request = palette_queue_seen;

	for (int requests = 0; requests < PALETTE_REQUESTS && request != palette_queue_write
		&& request >= PALETTE_REQUEST_ADDRESS(0) && request < PALETTE_REQUEST_ADDRESS(PALETTE_REQUESTS); requests++)
	{
		const uint16_t kind = PALETTE_REQUEST(request)->kind;

		if ((kind & PALETTE_TYPE_BITS) == PALETTE_ONE_BANK && (kind & BANK_GROUP_BITS) == SPRITE_BANKS)
			for (int slot = 0; slot < OBJECTS; slot++)
				if ((int16_t)OBJECT(OBJECT_ADDRESS(slot))->screen_x < SCREEN_LEFT)
					banks_lent[slot] |= banks_drawn[slot] & (uint16_t)(1u << (kind & BANK_BITS));
		request += REQUEST_BYTES;
		if (request >= PALETTE_REQUEST_ADDRESS(PALETTE_REQUESTS))
			request = PALETTE_REQUEST_ADDRESS(0);
	}
	palette_queue_seen = palette_queue_write;
	for (int slot = 0; slot < OBJECTS; slot++)
	{
		if (OBJECT(OBJECT_ADDRESS(slot))->flags == 0u)
			banks_drawn[slot] = banks_drawn_now[slot] = banks_lent[slot] = 0;
		banks_drawn[slot] |= banks_drawn_now[slot];
		banks_drawn_now[slot] = 0;
	}
}

/* Whether a piece of 'object' with 'colour' (the K051960's: its bank the low bits) has its bank lent (it is counted as
   drawn), and whether a piece at 'left', 'width' pixels wide, lies mostly on the board's 320: one with a bank lent is
   drawn only there (the barrel's one piece of 32 showed 10 of its pixels on the board, 6 of them in the columns the
   bezel hid, the rest in WIDE's left side) */
static bool piece_bank_lent(uint32_t object, uint8_t colour)
{
	const unsigned slot = (unsigned)((object - OBJECT_ADDRESS(0)) / 0x50u);
	const uint16_t bank = (uint16_t)(1u << (colour & BANK_BITS));

	if (object < OBJECT_ADDRESS(0) || slot >= OBJECTS)
		return false;
	banks_drawn_now[slot] |= bank;
	return (banks_lent[slot] & bank) != 0;
}

static bool mostly_on_board(uint16_t left, int16_t width)
{
	const int middle = (int16_t)left + width / 2;

	return middle >= SCREEN_LEFT && middle < SCREEN_RIGHT;
}

/* The highway's cars full of Foot soldiers (update_layers_highway): each one the picture draw_screen_picture draws
   into layer A, its map's pixels 320 to 511, the 192 of the K052109's ring of 512 that the arcade's screen never
   shows, then drives across by layer A's x going down from 511 (drive_foot_car), and when it is past the screen's
   right edge the game leaves its x at 511 again: the car stays in the ring, at the screen's -185 to 6 (the board's
   first 6 columns the bezel hid), until the next car is drawn over it. WIDE's sides showed it there, its front at the
   left and its back at the right (the ring's other turn, 512 on), and its back at the right as it came (issue #14:
   "it's like it's stuck on both sides"). On the highway layer A is drawn only where the car is as it drives and on
   the board's 320, and once it is past, on the board after its place */
#define HIGHWAY_STAGE       5u
#define CAR_FIRST_PIXEL     320    /* (the car's picture: its map's first pixel, and its width) */
#define CAR_PIXELS          192

static bool highway_car_window(int x, int *first, int *last)
{
	const int car = CAR_FIRST_PIXEL + LAYERS_DX - x;    /* (the screen's x of the car's left, from -185 to 326) */

	if (stage != HIGHWAY_STAGE || console.page != PAGE_GAME || menu_page_wide() || exit_picture()
		|| (scene != SCENE_PLAY && scene != SCENE_CONTINUE) || layers_step == 0u)    /* (no car yet) */
		return false;
	if (layers_step == 1u || layers_step == 3u || layers_step == 5u)    /* (driving: drive_foot_car's steps) */
	{
		*first = LEAST(car, 0);
		*last = MOST(car + CAR_PIXELS - 1, TMNT_VIDEO_WIDTH - 1);
	}
	else
	{
		*first = MOST(car + CAR_PIXELS, 0);
		*last = TMNT_VIDEO_WIDTH - 1;
	}
	return true;
}

void console_wide_frame(void)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	int left, right;

	lent_banks_frame();
	for (int layer = 0; layer < 2 && area_started; layer++)
	{
		const int x = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu);

		if (x < looping_last_x[layer] - LOOP_STEP)
			looping_maps |= 1u << layer;
		looping_last_x[layer] = x;
	}
	if (stage == TECHNODROME_STAGE && layers_step >= LIFT_RIDE_STEP)
		looping_maps = 0;
	read_picture_place();
	wide_sides_frame();
	console_wide_sides(&left, &right);
	rebuild_layers(left, right);
	tmnt_video_set_wide(left, right);
	/* the boxes in the picture's middle, and in a play the line under them; the continue's whole fixed layer, its
	   CONTINUE and its count over the play too (an exit's picture: the boxes alone, its texts its own) */
	tmnt_video_set_fixed_band((right - left) / 2, exit_picture() ? BOXES_ROWS
		: console.page == PAGE_GAME && game_state == GAME_STATE_GAME && scene == SCENE_CONTINUE ? TMNT_VIDEO_HEIGHT
		: BOXES_ROWS + MESSAGE_ROWS);
	for (int layer = 0; layer < 2; layer++)
	{
		const int width = LAYER(records[layer])->map_width * BLOCK_PIXELS;
		const int x = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu);

		int first, last;

		if (width > RING_PIXELS && layer_has_map(layer) && !(looping_maps & (1u << layer))
			&& !menu_page_wide() && !exit_picture())    /* (the menus' screens: the attract's, no area's map) */
			tmnt_video_set_layer_window(1 + layer, LAYERS_DX - x, width + LAYERS_DX - x - 1);
		else if (layer == 0 && highway_car_window(x, &first, &last))
			tmnt_video_set_layer_window(1, first, last);
		else
			tmnt_video_set_layer_window(1 + layer, -0x10000, 0x10000);
	}
}

/* The K051960's x of a piece for the wider screen (tmnt/video.h): its 9 bits, the flip, and the tenth bit */
static uint16_t wide_x(uint16_t left, bool flip_x)
{
	const uint16_t x = (uint16_t)(left & 0x3FFu);

	return (uint16_t)((x & 0x1FFu) | (flip_x ? 0x200u : 0u) | ((x & 0x200u) ? 0x400u : 0u));
}

/* An entry of the sprite RAM, unless the 128 are written */
static bool write_sprite(uint32_t *sprite_place, uint8_t size_code, uint8_t code, uint8_t colour, uint16_t x,
	uint16_t y)
{
	const uint32_t place = *sprite_place;

	if (place >= SPRITE_ENTRIES_END)
		return false;
	io_write8(place, io_read8(place) | 0x80u);
	io_write8(place + 1u, size_code);
	io_write8(place + 2u, code);
	io_write8(place + 3u, colour);
	io_write16(place + 6u, x);
	io_write8(place + 7u, (uint8_t)x);
	io_write16(place + 4u, y);
	io_write8(place + 5u, (uint8_t)y);
	*sprite_place = place + 8u;
	return true;
}

/* The HURRY arrow (update_area_layers: object 49, its x the screen's 0x158 when it comes, past the board's last
   enemies) at the wider screen's right as at the board's */
#define HURRY_OBJECT        49
#define HURRY_X             0x158u

/* The title's TURTLES (object 12) waits past the board's right edge while the banner comes down (start_title_logo's
   place), then slides in (title_turtles_slide_in): with WIDE it waits past the wider screen's (Sebas: its T showed
   there), its slide the longer */
static bool title_turtles_moved;

static void title_turtles_out_of_sight(int side_right)
{
	bool *const moved = &title_turtles_moved;

	if (game_step != ATTRACT_TITLE || scene != 1u || scene_step != 1u)
	{
		*moved = false;
		return;
	}
	if (!*moved)
		OBJECT(OBJECT_ADDRESS(12))->screen_x += (uint16_t)side_right;
	*moved = true;
}

/* The game's draw_sprites (engine/sprites.c), each piece written when it shows on the wider screen */
uint32_t draw_sprites(uint32_t object)
{
	int side_left, side_right;
	bool last_object = false;
	uint8_t flip = 0, flips = 0, frame_time = 0, objects_left = 0;
	uint16_t bottom = 0, count = 0, left = 0, shadow_y = 0, sprite_x = 0;
	uint32_t draw_list_place = 0, frame = 0, next_object = 0, size = 0, sprite = 0, sprite_place = 0;
	uint8_t t1 = 0, colour = 0, frame_time_2 = 0, time = 0, t4 = 0, t5 = 0;
	uint16_t object_2 = 0, t3 = 0, draw_next = 0;

	console_wide_sides(&side_left, &side_right);
	if (!side_left && !side_right)
		return original_draw_sprites(object);
	if (menu_page_wide())
		title_turtles_out_of_sight(side_right);
	if (console.page == PAGE_GAME && OBJECT(OBJECT_ADDRESS(HURRY_OBJECT))->flags != 0u)
		OBJECT(OBJECT_ADDRESS(HURRY_OBJECT))->screen_x = (uint16_t)(HURRY_X + side_right);    /* (Sebas) */
	draw_list_place = draw_list_ADDRESS + (int16_t)draw_first;
	sprite_place = SPRITE_RAM;
	next_object = OBJECT_ADDRESS(0);
	objects_left = draw_count;
	while (draw_list_place < 0x61580u)
	{
		object_2 = DRAW_ENTRY(draw_list_place)->object;
		next_object = (next_object & ~0xFFFFu) | object_2;
		draw_list_place += 2u;
		if (object_2 == 0u)
			continue;
		DRAW_ENTRY(draw_list_place - 0x2u)->object = 0u;
		do
		{
			object = next_object;
			frame = OBJECT(object)->frame;
			count = ANIMATION_FRAME(frame)->last_sprite;
			sprite = ANIMATION_FRAME(frame)->sprites;
			do
			{
				size = sprite_sizes_ADDRESS + ((SPRITE(sprite)->size_code & 0xE0u) >> 2);
				left = (uint16_t)SPRITE(sprite)->x;
				t1 = (uint8_t)((uint32_t)OBJECT(object)->flags << 1);
				flip = t1;
				if ((int8_t)t1 < 0)
					left = (uint16_t)(0u - left - SPRITE_SIZE(size)->width);
				left += OBJECT(object)->screen_x;
				screen_x_high = left;
				if ((int16_t)left < SCREEN_RIGHT + side_right && SPRITE_SIZE(size)->left - side_left < (int16_t)left)
				{
					const bool on_board = (int16_t)left < SCREEN_RIGHT && SPRITE_SIZE(size)->left < (int16_t)left;

					bottom = (uint16_t)(OBJECT(object)->height_whole + SPRITE(sprite)->y + OBJECT(object)->screen_y);
					if ((int16_t)bottom >= 0 && (int16_t)bottom < SPRITE_SIZE(size)->bottom)
					{
						uint16_t x;

						flips = SPRITE(sprite)->flips;
						x = wide_x(left, (int8_t)(flip ^ flips) < 0);
						if (on_board)
							screen_x_high = (uint16_t)((left & 0x1FFu) | (x & 0x200u));    /* (as the game leaves it: its
							                                                                   own pieces only) */
						bottom &= 0x1FFu;
						if ((flips & 0x40u) != 0u)
							bottom |= 0x200u;
						colour = OBJECT(object)->colour;
						colour = (int8_t)colour < 0 || SPRITE(sprite)->colour == 0xFu ? SPRITE(sprite)->colour
							: (uint8_t)(colour | (SPRITE(sprite)->colour & 0x90u));
						if (!piece_bank_lent(object, colour) || mostly_on_board(left, SPRITE_SIZE(size)->width))
							write_sprite(&sprite_place, SPRITE(sprite)->size_code, SPRITE(sprite)->code, colour, x,
								bottom);
					}
				}
				t3 = count - 1u;
				count = t3;
				sprite += 8u;
			} while (t3 != 0xFFFFu);
			if ((int8_t)OBJECT(object)->contacts < 0)
			{
				sprite = OBJECT(object)->shadow;
				shadow_y = (uint16_t)(OBJECT(object)->floor_height + SPRITE(sprite)->y + OBJECT(object)->screen_y);
				sprite_x = (uint16_t)(SPRITE(sprite)->x + OBJECT(object)->screen_x);
				screen_x_high = (uint16_t)(sprite_x & 0x1FFu);
				if ((int8_t)SPRITE(sprite)->flips < 0)
					screen_x_high = (uint16_t)((sprite_x & 0x1FFu) | 0x200u);
				sprite_x = wide_x(sprite_x, (int8_t)SPRITE(sprite)->flips < 0);
				shadow_y &= 0x1FFu;
				if ((SPRITE(sprite)->flips & 0x40u) != 0u)
					shadow_y |= 0x200u;
				write_sprite(&sprite_place, SPRITE(sprite)->size_code, SPRITE(sprite)->code, SPRITE(sprite)->colour,
					sprite_x, shadow_y);
			}
			if (OBJECT(object)->frame_time != 0u)
			{
				frame_time_2 = OBJECT(object)->frame_time;
				OBJECT(object)->frame_time = frame_time_2 - 1u;
				if (frame_time_2 == 1u)
				{
					frame += 0xAu;
					if (ANIMATION_FRAME(frame - 0xAu)->attack_depth >= 0)
						frame += 8u;
					time = ANIMATION_FRAME(frame)->time;
					frame_time = time;
					if (time != 0u)
					{
						OBJECT(object)->frame = frame;
						frame_time += OBJECT(object)->animation_delay;
					}
					else if (ANIMATION_FRAME(frame)->last_sprite != 0u)
					{
						frame = ANIMATION_FRAME(frame)->sprites;
						frame_time = ANIMATION_FRAME(frame)->time;
						OBJECT(object)->frame = frame;
						frame_time += OBJECT(object)->animation_delay;
					}
					OBJECT(object)->frame_time = frame_time;
				}
			}
			t4 = objects_left;
			objects_left = (uint8_t)(t4 - 1u);
			last_object = t4 == 1u;
			if (last_object)
				break;
			draw_next = OBJECT(object)->draw_next;
			next_object = (next_object & ~0xFFFFu) | draw_next;
		} while (draw_next != 0u);
		if (last_object)
			break;
	}
	/* the entries after the last written turned off, up to the first that was off */
	while (sprite_place < SPRITE_ENTRIES_END)
	{
		t5 = io_read8(sprite_place);
		io_write8(sprite_place, t5 & 0x7Fu);
		if ((t5 & 0x80u) == 0u)
			break;
		sprite_place += 8u;
	}
	return object;
}

/* ---- the layers' columns and rows past the game's ----

   When the scroll brings a column into view the game draws the map's column 8 pixels past its screen's right edge
   (or left), and when it brings a row, the row's 13 blocks of 32 pixels around its screen: with WIDE, the console
   draws after it the columns up to the wider screen's edge, and the row's blocks to its sides, from the same map and
   blocks: their tiles, and the floor map's cells where the game writes them (its floor variant: the play's walls and
   holes, so the turtles walk the sides as the board), not the registers it carries. Never a
   column of the K052109's ring of 512 pixels that the wider screen shows of another place (what it shows spans 496),
   nor past the map's ends; a map no wider than the ring is in it whole already */

#define TILE_ROW_BYTES      0x80u    /* a tile row of a layer's tile RAM: 64 tiles, their attribute and code */
#define RING_BLOCKS         16

/* The column at map pixel 'column' (a multiple of 8) of the layer whose record is 'record' (the working layer's, or
   layer A's or B's) and whose tiles are at 'tiles', as draw_new_row_and_column draws its new column; its tiles' codes
   'codes' bytes into each block's (CODES_PLAIN, or CODES_FLOOR: the blocks of the maps with a floor,
   draw_new_row_and_column_and_floor's, keep their floor's cells first) */
#define CODES_PLAIN         0x01u
#define CODES_FLOOR         0x11u

static int checking = -1;    /* (console_wide_check: the cells that differ from the map's, counted, not written) */
static int checked;

static void put_tile(uint32_t place, uint8_t value)
{
	if (checking < 0)
		io_write8(place, value);
	else
	{
		checked++;
		checking += io_read8(place) != value;
	}
}

/* A cell of the floor map (the play's walls and holes, the game's ring of 512 pixels as the tiles'), as the floor
   variant writes the column's: so a turtle or an enemy in WIDE's sides reads its floor as on the board */
static void put_floor(uint32_t place, uint8_t value)
{
	if (checking < 0)
		FLOOR_MAP_CELL(place)->cell = value;
}

#define FLOOR_MAP_END_BITS  0xFF1FFFu    /* the floor map's 0x800 bytes from floor_map_ADDRESS: its place wraps */

static void draw_column(uint32_t record, uint32_t tiles, int column, uint32_t codes)
{
	const uint32_t column_x = (uint32_t)column << 8;
	const uint32_t y_offset = (uint16_t)(0u - ((LAYER(record)->y + 0x1800u) & 0xE000u)) >> 4;
	const uint32_t ring_column = (column_x & 0x1F800u) >> 10;    /* (the tile's place in its row, 2 bytes a tile) */
	const uint32_t column_map_y = (((LAYER(record)->y - 0x800u) & 0xFFE000u) << 3) >> 16;
	const uint32_t block_column = ((column_x >> 8) & 0x18u) >> 3;
	uint32_t tile_place = (uint16_t)(ring_column + y_offset) + tiles;
	uint32_t map = column_map_y * LAYER(record)->map_width + (((column_x & 0xFFE000u) << 4) >> 16)
		+ LAYER(record)->map;
	const uint32_t blocks = LAYER(record)->blocks;
	const bool floor = codes == CODES_FLOOR;    /* (the floor's cells first in the blocks, as the floor variant) */
	uint32_t floor_place = ((uint16_t)(ring_column + y_offset) >> 1) + floor_map_ADDRESS;
	uint32_t map_entry, last_block;
	uint8_t y_in_block;

	for (int block = 0; block < 8; block++)
	{
		map_entry = MAP_ENTRY(map);
		put_tile(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
		put_tile(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		put_tile(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		put_tile(tile_place + 0x180u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		last_block = blocks + map_entry;
		put_tile(tile_place + 1u, BLOCK_BYTE(last_block + block_column + codes));
		put_tile(tile_place + 0x81u, BLOCK_BYTE(last_block + block_column + codes + 0x4u));
		put_tile(tile_place + 0x101u, BLOCK_BYTE(last_block + block_column + codes + 0x8u));
		put_tile(tile_place + 0x181u, BLOCK_BYTE(last_block + block_column + codes + 0xCu));
		if (floor)
		{
			put_floor(floor_place, BLOCK_BYTE(last_block + block_column + 1u));
			put_floor(floor_place + 0x40u, BLOCK_BYTE(last_block + block_column + 5u));
			put_floor(floor_place + 0x80u, BLOCK_BYTE(last_block + block_column + 9u));
			put_floor(floor_place + 0xC0u, BLOCK_BYTE(last_block + block_column + 0xDu));
			floor_place = ((floor_place + 0x100u) & FLOOR_MAP_END_BITS) | floor_map_ADDRESS;
		}
		map -= LAYER(record)->map_width;
		tile_place = LINKED(tile_place, (uint16_t)(tile_place + 0x200u)) & ~0x9000u;
	}
	map_entry = MAP_ENTRY(map);
	y_in_block = LAYER(record)->y_pixel & 0x18u;
	if (y_in_block == 0u)
		return;
	last_block = blocks + map_entry;
	put_tile(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
	put_tile(tile_place + 1u, BLOCK_BYTE(last_block + block_column + codes));
	if (floor)
		put_floor(floor_place, BLOCK_BYTE(last_block + block_column + 1u));
	if (4u - (y_in_block >> 3) == 1u)
		return;
	put_tile(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	put_tile(tile_place + 0x81u, BLOCK_BYTE(last_block + block_column + codes + 0x4u));
	if (floor)
		put_floor(floor_place + 0x40u, BLOCK_BYTE(last_block + block_column + 5u));
	if (4u - (y_in_block >> 3) == 2u)
		return;
	put_tile(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	put_tile(tile_place + 0x101u, BLOCK_BYTE(last_block + block_column + codes + 0x8u));
	if (floor)
		put_floor(floor_place + 0x80u, BLOCK_BYTE(last_block + block_column + 9u));
}

/* Block 'block' (of 32 pixels, from the map's left) of the working layer's new row, as draw_new_row_and_column draws
   the row's blocks ('codes' as draw_column's) */
static void draw_row_block(int block, uint32_t codes)
{
	const uint32_t row_y = LAYER(WORKING_LAYER)->new_row_y;
	const uint32_t tile_row = (uint16_t)(0u - (row_y & 0xF800u)) >> 4;
	const uint32_t map_row_y = (((row_y - 0x800u) & 0xFFE000u) << 3) >> 16;
	const uint32_t block_row = ((((row_y - 0x800u) << 7) >> 16) & 0xCu) ^ 0xCu;
	const uint32_t tile_place = (uint16_t)(tile_row + (uint32_t)(block & (RING_BLOCKS - 1)) * 8u) + working_layer_tiles;
	const uint32_t map = (uint32_t)block * 2u + map_row_y * LAYER(WORKING_LAYER)->map_width + LAYER(WORKING_LAYER)->map;
	const uint32_t blocks = LAYER(WORKING_LAYER)->blocks;
	const uint32_t map_entry = MAP_ENTRY(map);
	const uint32_t last_block = blocks + map_entry;

	io_write8(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 2u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 4u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 6u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 1u, BLOCK_BYTE(last_block + block_row + codes));
	io_write8(tile_place + 3u, BLOCK_BYTE(last_block + block_row + codes + 1u));
	io_write8(tile_place + 5u, BLOCK_BYTE(last_block + block_row + codes + 2u));
	io_write8(tile_place + 7u, BLOCK_BYTE(last_block + block_row + codes + 3u));
	if (codes == CODES_FLOOR)
	{
		const uint32_t floor_place = ((uint16_t)(tile_row + (uint32_t)(block & (RING_BLOCKS - 1)) * 8u) >> 1)
			+ floor_map_ADDRESS;

		put_floor(floor_place, BLOCK_BYTE(last_block + block_row + 1u));
		put_floor(floor_place + 1u, BLOCK_BYTE(last_block + block_row + 2u));
		put_floor(floor_place + 2u, BLOCK_BYTE(last_block + block_row + 3u));
		put_floor(floor_place + 3u, BLOCK_BYTE(last_block + block_row + 4u));
	}
}

/* Layers A and B (0, 1) by their tiles in the tile RAM (the program's addresses) */
#define LAYER_A_TILES       (TILE_RAM + 0x2000u)
#define LAYER_B_TILES       (TILE_RAM + 0x4000u)
#define LAYER_OF(tile_place) ((((tile_place) - TILE_RAM) & 0x6000u) == 0x4000u ? 1 : 0)

/* Each layer's blocks' codes as the game last drew it (CODES_PLAIN or CODES_FLOOR), and whether its columns past the
   board's are to be drawn again: after the game drew the whole layer (an area's start, a section of its own: it draws
   the ring from a page of 256 pixels, which the wider screen's left side may lie before, showing there what the ring
   holds of the map 512 pixels on: Sebas, "en la parte izquierda, hay que reconstruir en algunas secciones"), or when
   WIDE's sides come (the play after the menus, the story screens) */
static uint32_t layer_codes[2] = { CODES_PLAIN, CODES_PLAIN };
static bool layer_rebuild[2];

/* And the map's columns each layer's ring holds now (map pixels, a multiple of 8 to the last of a column): the 512 a
   whole layer drawn holds from its page, what the game's scroll draws, what the console has drawn. A room the game
   never scrolls past keeps only its whole draw's 512 though its map is wider (the first boss's apartment, Krang's:
   768), and the wider screen showed the ring's other end past them (Sebas: "hay una fina linea", "se sigue yendo de
   margen"): the columns it shows outside them are drawn from the map at each frame's end */
/* (drawn_first, drawn_last, drawn_known: declared with the layers' maps, above) */

/* After the game's new column or row of the working layer: the wider screen's */
static void draw_wide_layer(uint32_t codes)
{
	int left, right, map_pixels, x, shown_first, shown_last, ring_first, ring_last;

	layer_codes[LAYER_OF(working_layer_tiles)] = codes;
	layer_is_map(LAYER_OF(working_layer_tiles));
	console_wide_sides(&left, &right);
	map_pixels = LAYER(WORKING_LAYER)->map_width * BLOCK_PIXELS;
	if ((!left && !right) || map_pixels <= RING_PIXELS)
		return;
	x = (int)((LAYER(WORKING_LAYER)->x >> 8) & 0xFFFFu);
	shown_first = x - LAYERS_DX - left;                         /* the map's pixels the wider screen shows */
	shown_last = x - LAYERS_DX + TMNT_VIDEO_WIDTH - 1 + right;
	ring_first = shown_last - (RING_PIXELS - 1);                /* and those the ring may hold with them */
	ring_last = shown_first + (RING_PIXELS - 1);
	if (LAYER(WORKING_LAYER)->new_column != 0u)
	{
		const int game_column = (int)((LAYER(WORKING_LAYER)->new_column_x >> 8) & 0xFFF8u);

		if (game_column > x)
			for (int column = game_column + 8; column <= shown_last && column + 7 <= ring_last
				&& column + 8 <= map_pixels; column += 8)
				draw_column(WORKING_LAYER, working_layer_tiles, column, codes);
		else
			for (int column = game_column - 8; column + 7 >= shown_first && column >= ring_first && column >= 0;
				column -= 8)
				draw_column(WORKING_LAYER, working_layer_tiles, column, codes);
	}
	if (LAYER(WORKING_LAYER)->new_row != 0u)
	{
		const int game_first = (int)(((int32_t)LAYER(WORKING_LAYER)->x - 0x2000) >> 13);    /* (its 13 blocks) */

		for (int block = game_first - 1; block >= 0 && block * 32 + 31 >= shown_first && block * 32 >= ring_first;
			block--)
			draw_row_block(block, codes);
		for (int block = game_first + 13; block * 32 <= shown_last && block * 32 + 31 <= ring_last
			&& (block + 1) * 32 <= map_pixels; block++)
			draw_row_block(block, codes);
	}
}

void draw_new_row_and_column(void)
{
	original_draw_new_row_and_column();
	draw_wide_layer(CODES_PLAIN);
}

void draw_new_row_and_column_and_floor(void)
{
	original_draw_new_row_and_column_and_floor();
	draw_wide_layer(CODES_FLOOR);
}

/* The game's whole layer (stage/layers.c): its columns past the board's drawn again at the next frame's end */
void draw_whole_layer(uint32_t tile_place)
{
	original_draw_whole_layer(tile_place);
	layer_codes[LAYER_OF(tile_place)] = CODES_PLAIN;
	layer_rebuild[LAYER_OF(tile_place)] = true;
	layer_is_map(LAYER_OF(tile_place));
	drawn_first[LAYER_OF(tile_place)] = (int)LAYER(WORKING_LAYER)->x_page * 256;    /* (its 16 blocks from its page) */
	drawn_last[LAYER_OF(tile_place)] = drawn_first[LAYER_OF(tile_place)] + RING_PIXELS - 1;
	drawn_known[LAYER_OF(tile_place)] = true;
	layer_first[LAYER_OF(tile_place)] = (int)((LAYER(WORKING_LAYER)->x >> 8) & 0xFFFFu) - LAYERS_DX;
}

void draw_whole_layer_and_floor(uint32_t tile_place)
{
	original_draw_whole_layer_and_floor(tile_place);
	layer_codes[LAYER_OF(tile_place)] = CODES_FLOOR;
	layer_rebuild[LAYER_OF(tile_place)] = true;
	layer_is_map(LAYER_OF(tile_place));
	drawn_first[LAYER_OF(tile_place)] = (int)LAYER(WORKING_LAYER)->x_page * 256;
	drawn_last[LAYER_OF(tile_place)] = drawn_first[LAYER_OF(tile_place)] + RING_PIXELS - 1;
	drawn_known[LAYER_OF(tile_place)] = true;
	layer_first[LAYER_OF(tile_place)] = (int)((LAYER(WORKING_LAYER)->x >> 8) & 0xFFFFu) - LAYERS_DX;
}

/* Layer 'layer''s columns the wider screen shows past the board's, from its record (the game's frame done: its
   place as the next frame shows it), the tiles only: those the ring holds with the ones shown, never past the map's
   ends */
static void rebuild_layer(int layer, int left, int right)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	static const uint32_t tiles[2] = { LAYER_A_TILES, LAYER_B_TILES };
	const uint32_t record = records[layer];
	const int map_pixels = LAYER(record)->map_width * BLOCK_PIXELS;
	const int x = (int)((LAYER(record)->x >> 8) & 0xFFFFu);
	const int board_first = x - LAYERS_DX, shown_first = board_first - left;
	const int shown_last = board_first + TMNT_VIDEO_WIDTH - 1 + right;
	const int ring_first = shown_last - (RING_PIXELS - 1), ring_last = shown_first + (RING_PIXELS - 1);

	if (map_pixels <= RING_PIXELS || !layer_has_map(layer))
		return;
	for (int column = (board_first & ~7) - 8; column + 7 >= shown_first && column >= ring_first && column >= 0;
		column -= 8)
		draw_column(record, tiles[layer], column, layer_codes[layer]);
	for (int column = (board_first + TMNT_VIDEO_WIDTH) & ~7; column <= shown_last && column + 7 <= ring_last
		&& column + 8 <= map_pixels; column += 8)
		draw_column(record, tiles[layer], column, layer_codes[layer]);
}

/* Layer 'layer''s columns the wider screen shows that its ring does not hold (drawn_first, drawn_last), drawn from
   its map; the span the game itself keeps (its screen's columns and its next ones, as its scroll goes) counted as
   drawn, the span the ring holds at most 512 */
static void draw_shown_columns(int layer, int left, int right)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	static const uint32_t tiles[2] = { LAYER_A_TILES, LAYER_B_TILES };
	const uint32_t record = records[layer];
	const int map_pixels = LAYER(record)->map_width * BLOCK_PIXELS;
	const int x = (int)((LAYER(record)->x >> 8) & 0xFFFFu);
	const int board_first = x - LAYERS_DX;
	const int game_first = (board_first & ~7) - 8, game_last = board_first + TMNT_VIDEO_WIDTH + 8;
	const int shown_first = (board_first - left) & ~7;
	const int shown_last = board_first + TMNT_VIDEO_WIDTH - 1 + right;
	const int y = (int)((LAYER(record)->y >> 8) & 0xFFFFu);

	if (map_pixels <= RING_PIXELS || !layer_has_map(layer) || (looping_maps & (1u << layer)))
		return;
	if (drawn_known[layer] && y != drawn_y[layer])
	{
		/* the layer scrolled up or down: the rows the game drew since, and the console's beside them
		   (draw_wide_layer), span the screen shown then, at most WIDE_PAN pixels less at a side; past it the ring holds
		   its columns' older rows (the Technodrome's lift's left side, open again at the ride's end) */
		drawn_first[layer] = MOST(drawn_first[layer], LEAST(game_first, shown_first + 8));
		drawn_last[layer] = LEAST(drawn_last[layer], MOST(game_last, shown_last - 8));
	}
	drawn_y[layer] = y;
	if (!drawn_known[layer] || game_first > drawn_last[layer] + 8 || game_last < drawn_first[layer] - 8)
	{
		drawn_first[layer] = game_first;    /* (a jump: what the game keeps) */
		drawn_last[layer] = game_last;
		drawn_known[layer] = true;
	}
	if (game_first < drawn_first[layer])
		drawn_first[layer] = game_first;
	if (game_last > drawn_last[layer])
		drawn_last[layer] = game_last;
	if (drawn_last[layer] - drawn_first[layer] >= RING_PIXELS)
	{
		if (game_last >= drawn_last[layer])
			drawn_first[layer] = drawn_last[layer] - (RING_PIXELS - 1);
		else
			drawn_last[layer] = drawn_first[layer] + RING_PIXELS - 1;
	}
	for (int column = (drawn_last[layer] + 1 + 7) & ~7; column <= shown_last && column + 8 <= map_pixels; column += 8)
	{
		draw_column(record, tiles[layer], column, layer_codes[layer]);
		drawn_last[layer] = column + 7;
		if (drawn_last[layer] - drawn_first[layer] >= RING_PIXELS)
			drawn_first[layer] = drawn_last[layer] - (RING_PIXELS - 1);
	}
	for (int column = ((drawn_first[layer]) & ~7) - 8; column + 7 >= shown_first && column >= 0; column -= 8)
	{
		draw_column(record, tiles[layer], column, layer_codes[layer]);
		drawn_first[layer] = column;
		if (drawn_last[layer] - drawn_first[layer] >= RING_PIXELS)
			drawn_last[layer] = drawn_first[layer] + RING_PIXELS - 1;
	}
}

/* At each frame's end: the layers drawn whole, or all of them when WIDE's sides come, rebuilt past the board's */
static bool rebuild_had_sides;    /* (the last frame's: WIDE's sides there) */

static void rebuild_layers(int left, int right)
{
	bool *const had = &rebuild_had_sides;
	const bool sides = left || right;
	const bool in_play = sides && console.page == PAGE_GAME && !exit_picture();    /* (the menus' screens: the
	                                                                                   attract's, no area's map) */

	if (sides && !*had)
		layer_rebuild[0] = layer_rebuild[1] = true;
	*had = sides;
	for (int layer = 0; layer < 2 && in_play; layer++)
		if (layer_rebuild[layer])
		{
			rebuild_layer(layer, left, right);
			layer_rebuild[layer] = false;
		}
	for (int layer = 0; layer < 2 && in_play; layer++)
		draw_shown_columns(layer, left, right);
}

/* ---- the menus' screens ----

   OPTIONS' picture is BEST 10 PLAYERS' (best_players_draw): layer B's background, a row of one tile each
   (draw_ranking_background: rows 1 to 30, the ring's columns 12 to 51, which its scroll of 0x60 shows on the board's
   320), the turtles' pictures sliding over it. With WIDE the console puts each row's tile in the ring's other columns
   too, which the wider screen shows at its sides; the attract's next screens clear the layers as they set up */

#define BEST_PLAYERS_TILES      (TILE_RAM + 0x4000u)    /* layer B, as the program writes it: 2 bytes a cell */
#define BACKGROUND_FIRST_ROW    1
#define BACKGROUND_LAST_ROW     30
#define BACKGROUND_FIRST_COLUMN 12
#define BACKGROUND_COLUMNS      40
#define RING_COLUMNS            64

void console_wide_menus(void)
{
	int left, right;

	console_wide_sides(&left, &right);
	if ((!left && !right) || !OPTIONS_PAGE(console.page) || game_step != ATTRACT_BEST_PLAYERS || scene != 1u)
		return;
	for (uint32_t row = BACKGROUND_FIRST_ROW; row <= BACKGROUND_LAST_ROW; row++)
	{
		const uint32_t row_place = BEST_PLAYERS_TILES + row * TILE_ROW_BYTES;
		const uint8_t attribute = io_read8(row_place + BACKGROUND_FIRST_COLUMN * 2u);
		const uint8_t code = io_read8(row_place + BACKGROUND_FIRST_COLUMN * 2u + 1u);

		for (uint32_t column = BACKGROUND_FIRST_COLUMN + BACKGROUND_COLUMNS;
			column < BACKGROUND_FIRST_COLUMN + RING_COLUMNS; column++)
		{
			const uint32_t place = row_place + (column % RING_COLUMNS) * 2u;

			io_write8(place, attribute);
			io_write8(place + 1u, code);
		}
	}
}

/* ---- the enemies past the wider screen's sides ----

   The game puts an enemy of the spawn lists its kind's margin past the screen's right edge (or left: spawn_offsets'
   x), and takes an object off the screen 32 pixels past that margin (update_object_screen_place): on the board's 320
   both happen out of sight, but WIDE's sides show them. With WIDE the console puts them past the wider screen's
   sides instead (Sebas takes the changes to the play WIDE needs: the enemies walk the sides' columns more to come
   in), the same otherwise */

#define SPAWN_ON_LEFT           0x40u    /* a spawn record's flag: its enemy comes in at the left */
#define LAST_FOOT_SOLDIER       0x0Du    /* kinds 00-0D: the Foot soldiers (object_routine_tables), which with a
                                            parameter of 0 walk in from where they are put; with another, one of
                                            enemy_ways_in (out of a door, a window, from above) */

/* The game's spawn_enemies (objects/spawns.c): the enemy it put in, moved past the wider screen's side */
void spawn_enemies(void)
{
	uint32_t record = 0, object = 0, is_free = 0;
	int side_left, side_right;

	boss_rush_spawns();    /* (BOSS RUSH's crossing: the Foot soldiers passed, modes.c) */
	record = spawn_next;
	console_wide_sides(&side_left, &side_right);
	if ((!side_left && !side_right) || SPAWN_RECORD(record)->x == 0xFFFFu)
	{
		original_spawn_enemies();
		return;
	}
	take_free_object(record, &object, &is_free);    /* (the object the game takes, if it puts one in) */
	original_spawn_enemies();
	if (spawn_next != record + 8u || !is_free || OBJECT(object)->flags != 1u
		|| OBJECT(object)->kind != (uint8_t)(SPAWN_RECORD(record)->kind_and_parameter >> 8u))
		return;
	if (OBJECT(object)->kind > LAST_FOOT_SOLDIER || OBJECT(object)->parameter != 0u)
		return;    /* (a door, a window, the scenery, a soldier coming out of one: their places are the map's) */
	if ((SPAWN_RECORD(record)->flags & SPAWN_ON_LEFT) != 0u)
		OBJECT(object)->x -= (uint32_t)side_left << 8;
	else
		OBJECT(object)->x += (uint32_t)side_right << 8;
}

/* The game's update_object_screen_place (engine/sprites.c), off the screen past the wider screen's sides */
void update_object_screen_place(uint32_t object)
{
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t margin = 0;
	int side_left, side_right;

	console_wide_sides(&side_left, &side_right);
	if (!side_left && !side_right)
	{
		original_update_object_screen_place(object);
		return;
	}
	if (OBJECT(object)->flags == 0u)
		return;
	screen_x = (uint16_t)(((int32_t)(OBJECT(object)->x - (spawn_x & ~0xFFu)) >> 8) + 0x1A0u);
	OBJECT(object)->screen_x = screen_x;
	screen_y = (uint16_t)(((int32_t)(OBJECT(object)->y - (spawn_y & ~0xFFu)) >> 8) + 0x100u);
	OBJECT(object)->screen_y = screen_y;
	OBJECT(object)->flags &= 0xDFu;
	margin = spawn_offsets_ADDRESS + (int16_t)((uint32_t)(int8_t)OBJECT(object)->kind << 2);
	if ((int16_t)(screen_x - SCREEN_MARGIN(margin)->x) >= 0x1C0 + side_right
		|| (int16_t)(screen_x + SCREEN_MARGIN(margin)->x) < 0x40 - side_left
		|| (int16_t)screen_y >= 0x120 || (int16_t)(screen_y + SCREEN_MARGIN(margin)->y) < -0x20)
		OBJECT(object)->flags |= 0x20u;
}

/* For checks (tmnt_console_headless --wide-check): the columns of WIDE's sides (whole in the wider screen, past the
   board's) whose tiles are not their map's, of each layer with a map wider than the ring; 0 when all are */
int console_wide_check(char *text, size_t size)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	static const uint32_t tiles[2] = { LAYER_A_TILES, LAYER_B_TILES };
	int left, right, bad = 0, used = 0;

	text[0] = 0;
	console_wide_sides(&left, &right);
	if ((!left && !right) || console.page != PAGE_GAME || exit_picture())
		return 0;    /* (an exit's picture: no map's, its sides painted) */
	for (int layer = 0; layer < 2; layer++)
	{
		const uint32_t record = records[layer];
		const int map_pixels = LAYER(record)->map_width * BLOCK_PIXELS;
		const int x = (int)((LAYER(record)->x >> 8) & 0xFFFFu);
		const int board_first = x - LAYERS_DX, shown_first = board_first - left;
		const int shown_last = board_first + TMNT_VIDEO_WIDTH - 1 + right;
		int bad_left = 0, bad_right = 0, first_bad = -1;

		if (map_pixels <= RING_PIXELS || !layer_has_map(layer))
			continue;
		for (int column = (shown_first + 7) & ~7; column + 7 <= shown_last && column + 8 <= map_pixels; column += 8)
		{
			if (column >= (board_first & ~7) - 8 && column + 7 <= board_first + TMNT_VIDEO_WIDTH + 8)
				continue;    /* (the board's, and the game's own next columns) */
			if (column < 0)
				continue;
			checking = 0;
			checked = 0;
			draw_column(record, tiles[layer], column, layer_codes[layer]);
			if (checking * 4 > checked * 3)    /* (most of the column: not the game's own tiles changed in place) */
			{
				if (column < board_first)
					bad_left++;
				else
					bad_right++;
				if (first_bad < 0)
					first_bad = column;
			}
			checking = -1;
		}
		if (bad_left || bad_right)
		{
			used += snprintf(text + used, size - (size_t)used, "layer %c: %d columns at the left, %d at the right (map %d, "
				"board's first %d, sides %d %d, scene %u) ", 'A' + layer, bad_left, bad_right, first_bad, board_first, left, right,
				(unsigned)scene);
			bad += bad_left + bad_right;
		}
	}
	return bad;
}

/* For a LAN game's start (console_power_on): WIDE's state as at power-on */
void wide_power_on(void)
{
	layer_maps = 0;
	looping_maps = 0;
	memset(banks_drawn, 0, sizeof(banks_drawn));
	memset(banks_drawn_now, 0, sizeof(banks_drawn_now));
	memset(banks_lent, 0, sizeof(banks_lent));
	palette_queue_seen = 0;
	area_started = false;
	wide_hidden = 0;
	picture_place = PICTURE_MIDDLE;
	lift_bar_left = lift_bar_right = lift_bar_left_now = lift_bar_right_now = 0;
	memset(lift_cache, 0, sizeof(lift_cache));
	lift_cache_blocks = 0;
	wide_left_now = wide_right_now = 0;
	title_turtles_moved = false;
	rebuild_had_sides = false;
	checking = -1;
	checked = 0;
	for (int layer = 0; layer < 2; layer++)
	{
		drawn_first[layer] = drawn_last[layer] = drawn_y[layer] = 0;
		drawn_known[layer] = false;
		looping_last_x[layer] = 0;
		layer_first[layer] = 0;
		layer_codes[layer] = CODES_PLAIN;
		layer_rebuild[layer] = false;
	}
}
