/**
 *    Name:        wide.c
 *    Description: the console's WIDE (OPTIONS > VIDEO): the picture wider than the board's 320 pixels in a game's
 *                 play and in the menus, the game's sprites and layers' columns written for the wider screen, and its
 *                 enemies coming in and going out past its sides
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    05-11-2026
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
 *    otherwise, at most the 128 the sprite RAM holds. Its work on the objects (their animations stepped on) and what
 *    it leaves in the registers the game carries (screen_x_high) are the game's own.
 */
#include "console_internal.h"

#include "game.h"
#include "tmnt/video.h"

#include <stdio.h>

#define SPRITE_ENTRIES_END  (SPRITE_RAM + 0x400u)    /* the sprite RAM's 128 entries of 8 bytes */
#define SCREEN_RIGHT        0x1A0                    /* the K051960's x of the screen's right edge */
#define SCENE_PLAY          4u                       /* the game's scenes WIDE shows: the play and the area's exit */
#define SCENE_EXIT          5u
#define LAYERS_DX           6                        /* a map's pixel on the screen: its x less the layer's x, plus 6
                                                        (the K052109's -90 and the program's scroll, x - 0x60) */
#define BLOCK_PIXELS        16                       /* a map's row: map_width bytes, 2 a block of 32 pixels */
#define RING_PIXELS         512                      /* a map no wider than the K052109's ring repeats itself */
#define LEAST(a, b)         ((a) < (b) ? (a) : (b))

/* The layers the game draws as maps of blocks in the area being played, A 1 and B 2: those it draws whole
   (start_area_layers' start_layers_..., an exit's picture) or column by column as the scroll goes. The other layer's
   record keeps what an earlier area or screen left in it, no map of this one (an area of one map, as the street's):
   WIDE takes no room, ends nor columns from it */
static unsigned layer_maps;

/* And each one's first column the area shows: the board's column 0 where the scroll starts it (the map's columns
   before it, which the arcade never shows, may be blank: area 0's 32 of nothing before the building). The picture's
   left side shows no column before it: at an area's start the wider screen's columns are all at the right (Sebas:
   "correr todo para la derecha para que no aparezca"), back to the middle as the scroll brings the map */
static int layer_first[2];

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
	original_start_area_layers();
	layer_first[0] = (int)((LAYER(LAYER_A)->x >> 8) & 0xFFFFu) - LAYERS_DX;
	layer_first[1] = (int)((LAYER(LAYER_B)->x >> 8) & 0xFFFFu) - LAYERS_DX;
}

/* The menus' pages (Sebas: "el menu tmb tiene que tener wide"): their screens with the columns more at both sides */
static bool menu_page_wide(void)
{
	return game_state == GAME_STATE_ATTRACT && console.page != PAGE_PRESENTATION && console.page != PAGE_GAME;
}

/* In a game's play, and in the menus: the story screens, the continue, the ending and the ranking keep the board's
   320 (their pictures were never drawn wider), and the presentation, on black. In a game the picture has its columns
   more at both sides, but where a side has no map past the board's screen (an area's start at its left, its end at
   its right), the picture shows as much of it as there is and the rest at the other side (Sebas: "recorta de la
   izquierda y corre"): the game is the same, only what the picture shows of it moves, back to the middle as the
   scroll brings the map */
void console_wide_sides(int *left, int *right)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	static const int sides[WIDE_KINDS] = { 0, 48, 88 };
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
	if (console.page != PAGE_GAME || game_state != GAME_STATE_GAME || (scene != SCENE_PLAY && scene != SCENE_EXIT))
		return;
	for (int layer = 0; layer < 2; layer++)
	{
		const int width = LAYER(records[layer])->map_width * BLOCK_PIXELS;
		const int first = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu) - LAYERS_DX;    /* (the board's column 0) */

		if (width <= RING_PIXELS || !layer_has_map(layer))
			continue;    /* (a map that repeats itself has no ends; a record of no map, none) */
		room_left = LEAST(room_left, first - layer_first[layer] < 0 ? 0 : first - layer_first[layer]);
		room_right = LEAST(room_right, width - (first + TMNT_VIDEO_WIDTH) < 0 ? 0 : width - (first + TMNT_VIDEO_WIDTH));
	}
	*left = LEAST(side, room_left);
	*right = LEAST(side, room_right);
	more = 2 * side - *left - *right;
	*right += LEAST(more, room_right - *right);
	more = 2 * side - *left - *right;
	*left += LEAST(more, LEAST(room_left, TMNT_VIDEO_MAX_LEFT) - *left);
}

/* The video's WIDE for the next frame: its sides, and where layers A and B have a map (an area's start shows past the
   map's left end, its end past its right: their ring holds other columns there) */
static void rebuild_layers(int left, int right);

void console_wide_frame(void)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	int left, right;

	console_wide_sides(&left, &right);
	rebuild_layers(left, right);
	tmnt_video_set_wide(left, right);
	for (int layer = 0; layer < 2; layer++)
	{
		const int width = LAYER(records[layer])->map_width * BLOCK_PIXELS;
		const int x = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu);

		if (width > RING_PIXELS && layer_has_map(layer) && !menu_page_wide())    /* (the menus' screens: the
		                                                                              attract's, no area's map) */
			tmnt_video_set_layer_window(1 + layer, LAYERS_DX - x, width + LAYERS_DX - x - 1);
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

/* The title's TURTLES (object 12) waits past the board's right edge while the banner comes down (start_title_logo's
   place), then slides in (title_turtles_slide_in): with WIDE it waits past the wider screen's (Sebas: its T showed
   there), its slide the longer */
static void title_turtles_out_of_sight(int side_right)
{
	static bool moved;

	if (game_step != ATTRACT_TITLE || scene != 1u || scene_step != 1u)
	{
		moved = false;
		return;
	}
	if (!moved)
		OBJECT(OBJECT_ADDRESS(12))->screen_x += (uint16_t)side_right;
	moved = true;
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
						write_sprite(&sprite_place, SPRITE(sprite)->size_code, SPRITE(sprite)->code,
							(int8_t)colour < 0 || SPRITE(sprite)->colour == 0xFu ? SPRITE(sprite)->colour
								: (uint8_t)(colour | (SPRITE(sprite)->colour & 0x90u)), x, bottom);
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
   blocks, the tiles only (not the floor map, which the game's play reads; nor the registers it carries). Never a
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
	if (4u - (y_in_block >> 3) == 1u)
		return;
	put_tile(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	put_tile(tile_place + 0x81u, BLOCK_BYTE(last_block + block_column + codes + 0x4u));
	if (4u - (y_in_block >> 3) == 2u)
		return;
	put_tile(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	put_tile(tile_place + 0x101u, BLOCK_BYTE(last_block + block_column + codes + 0x8u));
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
	layer_first[LAYER_OF(tile_place)] = (int)((LAYER(WORKING_LAYER)->x >> 8) & 0xFFFFu) - LAYERS_DX;
}

void draw_whole_layer_and_floor(uint32_t tile_place)
{
	original_draw_whole_layer_and_floor(tile_place);
	layer_codes[LAYER_OF(tile_place)] = CODES_FLOOR;
	layer_rebuild[LAYER_OF(tile_place)] = true;
	layer_is_map(LAYER_OF(tile_place));
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

/* At each frame's end: the layers drawn whole, or all of them when WIDE's sides come, rebuilt past the board's */
static void rebuild_layers(int left, int right)
{
	static bool had_sides;
	const bool sides = left || right;
	const bool in_play = sides && console.page == PAGE_GAME;    /* (the menus' screens: the attract's, no area's map) */

	if (sides && !had_sides)
		layer_rebuild[0] = layer_rebuild[1] = true;
	had_sides = sides;
	for (int layer = 0; layer < 2 && in_play; layer++)
		if (layer_rebuild[layer])
		{
			rebuild_layer(layer, left, right);
			layer_rebuild[layer] = false;
		}
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
	const uint32_t record = spawn_next;
	uint32_t object = 0, is_free = 0;
	int side_left, side_right;

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
	if ((!left && !right) || console.page != PAGE_GAME)
		return 0;
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
