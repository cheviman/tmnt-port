/**
 *    Name:        wide.c
 *    Description: the console's WIDE (OPTIONS > VIDEO): the picture wider than the board's 320 pixels in a game,
 *                 and the game's sprites and layers' columns written for the wider screen
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As outrunners-port's WIDE (16:9, 48 columns more at each side; 20:9, 88: the K051960's 9-bit x and the
 *    K052109's 512-pixel maps leave no room for more). The video draws the layers' maps and the sprites there
 *    (tmnt_video_set_wide, which the port sets each frame from console_wide_sides); in a game only, the menus and
 *    the attract mode's screens keeping the board's 320 between black bars (their pictures were never drawn wider).
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

#define SPRITE_ENTRIES_END  (SPRITE_RAM + 0x400u)    /* the sprite RAM's 128 entries of 8 bytes */
#define SCREEN_RIGHT        0x1A0                    /* the K051960's x of the screen's right edge */
#define SCENE_PLAY          4u                       /* the game's scenes WIDE shows: the play and the area's exit */
#define SCENE_EXIT          5u
#define LAYERS_DX           6                        /* a map's pixel on the screen: its x less the layer's x, plus 6
                                                        (the K052109's -90 and the program's scroll, x - 0x60) */
#define BLOCK_PIXELS        16                       /* a map's row: map_width bytes, 2 a block of 32 pixels */
#define RING_PIXELS         512                      /* a map no wider than the K052109's ring repeats itself */
#define LEAST(a, b)         ((a) < (b) ? (a) : (b))

/* In a game's play only: the story screens, the continue, the ending and the ranking keep the board's 320 (their
   pictures were never drawn wider). The picture has its columns more at both sides, but where a side has no map past
   the board's screen (an area's start at its left, its end at its right), the picture shows as much of it as there
   is and the rest at the other side (Sebas: "recorta de la izquierda y corre"): the game is the same, only what the
   picture shows of it moves, back to the middle as the scroll brings the map */
void console_wide_sides(int *left, int *right)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	static const int sides[WIDE_KINDS] = { 0, 48, 88 };
	int side, room_left = 0x10000, room_right = 0x10000, more;

	*left = *right = 0;
	if (console.page != PAGE_GAME || game_state != GAME_STATE_GAME || (scene != SCENE_PLAY && scene != SCENE_EXIT)
		|| console_settings.wide < 0 || console_settings.wide >= WIDE_KINDS)
		return;
	side = sides[console_settings.wide];
	for (int layer = 0; layer < 2; layer++)
	{
		const int width = LAYER(records[layer])->map_width * BLOCK_PIXELS;
		const int first = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu) - LAYERS_DX;    /* (the board's column 0) */

		if (width <= RING_PIXELS)
			continue;    /* (a map that repeats itself has no ends) */
		room_left = LEAST(room_left, first < 0 ? 0 : first);
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
void console_wide_frame(void)
{
	static const uint32_t records[2] = { LAYER_A, LAYER_B };
	int left, right;

	console_wide_sides(&left, &right);
	tmnt_video_set_wide(left, right);
	for (int layer = 0; layer < 2; layer++)
	{
		const int width = LAYER(records[layer])->map_width * BLOCK_PIXELS;
		const int x = (int)((LAYER(records[layer])->x >> 8) & 0xFFFFu);

		if (width > RING_PIXELS)
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

/* The column of the working layer at map pixel 'column' (a multiple of 8), as draw_new_row_and_column draws its new
   column; its tiles' codes 'codes' bytes into each block's (CODES_PLAIN, or CODES_FLOOR: the blocks of the maps with a
   floor, draw_new_row_and_column_and_floor's, keep their floor's cells first) */
#define CODES_PLAIN         0x01u
#define CODES_FLOOR         0x11u

static void draw_column(int column, uint32_t codes)
{
	const uint32_t column_x = (uint32_t)column << 8;
	const uint32_t y_offset = (uint16_t)(0u - ((LAYER(WORKING_LAYER)->y + 0x1800u) & 0xE000u)) >> 4;
	const uint32_t ring_column = (column_x & 0x1F800u) >> 10;    /* (the tile's place in its row, 2 bytes a tile) */
	const uint32_t column_map_y = (((LAYER(WORKING_LAYER)->y - 0x800u) & 0xFFE000u) << 3) >> 16;
	const uint32_t block_column = ((column_x >> 8) & 0x18u) >> 3;
	uint32_t tile_place = (uint16_t)(ring_column + y_offset) + working_layer_tiles;
	uint32_t map = column_map_y * LAYER(WORKING_LAYER)->map_width + (((column_x & 0xFFE000u) << 4) >> 16)
		+ LAYER(WORKING_LAYER)->map;
	const uint32_t blocks = LAYER(WORKING_LAYER)->blocks;
	uint32_t map_entry, last_block;
	uint8_t y_in_block;

	for (int block = 0; block < 8; block++)
	{
		map_entry = MAP_ENTRY(map);
		io_write8(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		io_write8(tile_place + 0x180u, BLOCK_BYTE(blocks + (int16_t)map_entry));
		last_block = blocks + map_entry;
		io_write8(tile_place + 1u, BLOCK_BYTE(last_block + block_column + codes));
		io_write8(tile_place + 0x81u, BLOCK_BYTE(last_block + block_column + codes + 0x4u));
		io_write8(tile_place + 0x101u, BLOCK_BYTE(last_block + block_column + codes + 0x8u));
		io_write8(tile_place + 0x181u, BLOCK_BYTE(last_block + block_column + codes + 0xCu));
		map -= LAYER(WORKING_LAYER)->map_width;
		tile_place = LINKED(tile_place, (uint16_t)(tile_place + 0x200u)) & ~0x9000u;
	}
	map_entry = MAP_ENTRY(map);
	y_in_block = LAYER(WORKING_LAYER)->y_pixel & 0x18u;
	if (y_in_block == 0u)
		return;
	last_block = blocks + map_entry;
	io_write8(tile_place, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 1u, BLOCK_BYTE(last_block + block_column + codes));
	if (4u - (y_in_block >> 3) == 1u)
		return;
	io_write8(tile_place + 0x80u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 0x81u, BLOCK_BYTE(last_block + block_column + codes + 0x4u));
	if (4u - (y_in_block >> 3) == 2u)
		return;
	io_write8(tile_place + 0x100u, BLOCK_BYTE(blocks + (int16_t)map_entry));
	io_write8(tile_place + 0x101u, BLOCK_BYTE(last_block + block_column + codes + 0x8u));
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

/* After the game's new column or row of the working layer: the wider screen's */
static void draw_wide_layer(uint32_t codes)
{
	int left, right, map_pixels, x, shown_first, shown_last, ring_first, ring_last;

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
				draw_column(column, codes);
		else
			for (int column = game_column - 8; column + 7 >= shown_first && column >= ring_first && column >= 0;
				column -= 8)
				draw_column(column, codes);
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
