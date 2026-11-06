/**
 *    Name:        video.c
 *    Description: TMNT's video: the K052109's three tile layers, the K051960's sprites and the palette, drawn
 *                 from the board's memories as the board shows them
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Behavior follows MAME 0.289 pixel for pixel: konami/tmnt.cpp's screen update (Nicola Salmoria), the
 *    K052109's and the K051960's (konami/k052109.cpp, konami/k051960.cpp), and what they use of emu/tilemap.cpp
 *    and emu/drawgfx.cpp (Aaron Giles) and of the palette (util/palette.cpp). tools/core/konami/reference
 *    builds those files into kref, and kref --video-check compares its frames with these. MAME's terms are in
 *    tools/core/konami/reference/mame/LICENSE.
 *
 *    How a frame is made, in a 384 x 264 bitmap of palette pens (lines 16-239 and columns 0-319 shown): layer B
 *    with all its pixels, then the sprites when the priority's bit 0 is set, layer A without its pen 0, the
 *    sprites if they were not drawn yet, and the fixed layer without its pen 0. A layer is a 512 x 256 map of
 *    8 x 8 tiles; the screen's pixel shows the map's pixel its scroll gives (one for the layer, or one a row,
 *    a column, or both, as the K052109's scroll control register says), all of it turned round when the
 *    screen is flipped. A sprite is 1 to 8 x 8 cells of 16 x 16 pixels; its pen 15 shades what is under it
 *    when it casts a shadow. The pens become colours through the palette RAM (xBGR 555), and a shaded pen
 *    takes 0.75 of its colour.
 */
#include "tmnt/video.h"

#include "game/data.h"
#include "tmnt/machine.h"

#include <string.h>

#define BITMAP_LEFT     TMNT_VIDEO_MAX_LEFT    /* the screen's first column in the bitmap: room for WIDE's left side */
#define BITMAP_WIDTH    (BITMAP_LEFT + TMNT_VIDEO_WIDTH + TMNT_VIDEO_MAX_MORE)
#define BITMAP_HEIGHT   264
#define CLIP_LEFT       0
#define CLIP_RIGHT      319
#define CLIP_TOP        16
#define CLIP_BOTTOM     239
#define MAP_WIDTH       512
#define MAP_HEIGHT      256
#define PENS            1024            /* the palette's; the shaded ones follow them */
#define TILES           (GAME_TILES_SIZE / 32)
#define SPRITES         (GAME_SPRITES_SIZE / 128)
#define SHADOW_FACTOR   0.75f           /* tmnt.cpp's video_start */
/* tilemap.cpp's flip sums: the visible area's x0 + x1 + 1 and y0 + y1 + 1 */
#define X_EXTENT        (CLIP_RIGHT + CLIP_LEFT + 1)
#define Y_EXTENT        (CLIP_BOTTOM + CLIP_TOP + 1)

/* the K052109's RAM: colour (attribute), then code for the fixed layer, A and B, its registers among them */
#define COLOUR_RAM      0x0000
#define CODE_RAM        0x2000
#define CODE_RAM_2      0x4000
#define SCROLL_Y        0x1800
#define SCROLL_X        0x1A00
#define REG_ADDRESS_MAP 0x1C00
#define REG_SCROLL      0x1C80
#define REG_FLIP        0x1E80

typedef struct
{
	int dx;                         /* set_scrolldx (the same flipped) */
	int rows, cols;                 /* its scroll: one, a row, a column... (set_scroll_rows, set_scroll_cols) */
	int rowscroll[MAP_HEIGHT];
	int colscroll[MAP_WIDTH / 8];
} layer;

static const int k_colour_base[3] = { 0, 32, 40 };     /* tmnt.cpp's layer_colorbase */
static const int k_sprite_colour_base = 16;

static layer g_layers[3];
static uint8_t g_sprite_buffer[TMNT_SPRITE_RAM_SIZE];  /* the K051960's DMA copy */
static uint16_t g_bitmap[BITMAP_HEIGHT * BITMAP_WIDTH];
static int g_left, g_right, g_left_next, g_right_next;    /* WIDE: the columns drawn more at each side */
static int g_first[3] = { -0x10000, -0x10000, -0x10000 }, g_last[3] = { 0x10000, 0x10000, 0x10000 };
static int g_first_next[3] = { -0x10000, -0x10000, -0x10000 }, g_last_next[3] = { 0x10000, 0x10000, 0x10000 };
                                        /* WIDE: the maps' ends on the screen (the fixed layer's never set) */
#define BLACK_PEN       0xFFFFu         /* (WIDE: past a map's end on layer B) */
static uint32_t g_wide[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
static int g_wide_width = TMNT_VIDEO_WIDTH;
static int g_wide_left;    /* (g_left when g_wide was drawn) */

uint8_t *tmnt_video_tiles_shown = NULL;
uint8_t *tmnt_video_sprites_shown = NULL;
uint8_t *tmnt_video_tile_palettes = NULL;
uint8_t *tmnt_video_sprite_palettes = NULL;

void tmnt_video_reset(void)
{
	memset(g_layers, 0, sizeof(g_layers));
	g_layers[0].dx = -96;
	g_layers[1].dx = -90;
	g_layers[2].dx = -90;
	for (int index = 0; index < 3; index++)
		g_layers[index].rows = g_layers[index].cols = 1;
	memset(g_sprite_buffer, 0, sizeof(g_sprite_buffer));
}

/* k052109.cpp's update_scroll, for layers A and B */
void tmnt_video_vblank_end(void)
{
	static const int rows_table[4] = { 1, 1, 32, 256 };
	const uint8_t *ram = tmnt_board.tile_ram;

	for (int map = 0; map < 2; map++)
	{
		const int control = (ram[REG_SCROLL] >> (map * 3)) & 7;
		const int rows = rows_table[control & 3];
		const int cols = (control & 4) ? 64 : 1;
		const uint8_t *scroll_y = &ram[SCROLL_Y | (map ? 0x2000 : 0)];
		const uint8_t *scroll_x = &ram[SCROLL_X | (map ? 0x2000 : 0)];
		layer *target = &g_layers[map + 1];

		if (rows == 1 && cols == 1)
		{
			target->rows = target->cols = 1;
			target->rowscroll[0] = scroll_x[0] + 256 * scroll_x[1];
			target->colscroll[0] = scroll_y[12];
		}
		else if (cols == 1)
		{
			const int y = scroll_y[12];
			const int mask = rows == 256 ? 0xFF : 0xF8;

			target->rows = 256;
			target->cols = 1;
			target->colscroll[0] = y;
			for (int offset = 0; offset < 256; offset++)
				target->rowscroll[(offset + y) & 0xFF] = scroll_x[2 * (offset & mask)] + 256 * scroll_x[2 * (offset & mask) + 1];
		}
		else if (rows == 1)
		{
			const int x = scroll_x[0] + 256 * scroll_x[1];

			target->rows = 1;
			target->cols = 64;
			target->rowscroll[0] = x;
			for (int offset = 0; offset < 64; offset++)
				target->colscroll[(offset + x / 8) & 0x3F] = scroll_y[offset];
		}
		else
		{
			const int step = rows == 256 ? 2 : 16;

			target->rows = rows;
			target->cols = 64;
			for (int offset = 0; offset < rows; offset++)
				target->rowscroll[offset] = scroll_x[step * offset] + 256 * scroll_x[step * offset + 1];
			for (int offset = 0; offset < 64; offset++)
				target->colscroll[offset] = scroll_y[offset];
		}
	}
}

/* tilemap.cpp's effective_rowscroll and effective_colscroll: where the map's pixmap starts on the screen */
static int effective_rowscroll(const layer *map, int index, bool flipped)
{
	int value;

	if (flipped)
		index = map->rows - 1 - index;
	value = flipped ? X_EXTENT - MAP_WIDTH - (map->dx - map->rowscroll[index]) : map->dx - map->rowscroll[index];
	return value < 0 ? MAP_WIDTH - (-value) % MAP_WIDTH : value % MAP_WIDTH;
}

static int effective_colscroll(const layer *map, int index, bool flipped)
{
	int value;

	if (flipped)
		index = map->cols - 1 - index;
	value = flipped ? Y_EXTENT - MAP_HEIGHT - (0 - map->colscroll[index]) : 0 - map->colscroll[index];
	return value < 0 ? MAP_HEIGHT - (-value) % MAP_HEIGHT : value % MAP_HEIGHT;
}

static int wrap(int value, int size)
{
	value %= size;
	return value < 0 ? value + size : value;
}

/* The pen of a map pixel (0-511, 0-255), its colour and its tile's code (k052109.cpp's get_tile_info, tmnt.cpp's
   callback) */
static int tile_pixel(int map, int x, int y, int *colour, int *tile)
{
	const uint8_t *ram = tmnt_board.tile_ram;
	const int index = (y >> 3) * 64 + (x >> 3);
	int attribute = ram[COLOUR_RAM + map * 0x800 + index];
	int code = ram[CODE_RAM + map * 0x800 + index] + 256 * ram[CODE_RAM_2 + map * 0x800 + index];
	int bank = tmnt_board.char_rom_bank[(attribute & 0x0C) >> 2];
	int row = y & 7;
	const int column = x & 7;
	const uint8_t *data;

	if (!(ram[REG_ADDRESS_MAP] & 0x40))
		attribute = (attribute & 0xF3) | ((bank & 3) << 2);
	bank >>= 2;
	if ((attribute & 0x02) && (ram[REG_FLIP] & 0x04))
		row = 7 - row;
	code |= ((attribute & 0x03) << 8) | ((attribute & 0x10) << 6) | ((attribute & 0x0C) << 9) | (bank << 13);
	*colour = k_colour_base[map] + ((attribute & 0xE0) >> 5);
	*tile = code % TILES;
	data = &game_tiles[(size_t)(code % TILES) * 32u + (size_t)row * 4u];
	return ((data[3] >> (7 - column)) & 1) << 3 | ((data[2] >> (7 - column)) & 1) << 2
		| ((data[1] >> (7 - column)) & 1) << 1 | ((data[0] >> (7 - column)) & 1);
}

/* k052109's tilemap_draw: the map 'map' over the bitmap, all its pixels or only those that are not pen 0 */
static void draw_layer(int map, bool opaque)
{
	const layer *scroll = &g_layers[map];
	const bool flipped = (tmnt_board.tile_ram[REG_FLIP] & 0x01) != 0;
	int rowscroll[MAP_HEIGHT], colscroll[MAP_WIDTH / 8];
	bool same_rows = true, same_cols = true;
	int rows, cols;

	for (int index = 0; index < scroll->rows; index++)
	{
		rowscroll[index] = effective_rowscroll(scroll, index, flipped);
		same_rows = same_rows && rowscroll[index] == rowscroll[0];
	}
	for (int index = 0; index < scroll->cols; index++)
	{
		colscroll[index] = effective_colscroll(scroll, index, flipped);
		same_cols = same_cols && colscroll[index] == colscroll[0];
	}
	/* (k052109's tile_blitter takes the scroll of both rows and columns; else tilemap.cpp, which treats a
	   scroll whose rows, or columns, are all the same as one) */
	const bool mixed = scroll->rows > 1 && scroll->cols > 1;
	rows = same_rows ? 1 : scroll->rows;
	cols = same_cols ? 1 : scroll->cols;

	for (int y = CLIP_TOP; y <= CLIP_BOTTOM; y++)
	{
		uint16_t *line = &g_bitmap[y * BITMAP_WIDTH + BITMAP_LEFT];

		for (int x = CLIP_LEFT - g_left; x <= CLIP_RIGHT + g_right; x++)
		{
			int px, py, colour, pen, tile;

			if (mixed)
			{
				const int band = y / (MAP_HEIGHT / scroll->rows);
				int start_x = scroll->rowscroll[flipped ? scroll->rows - 1 - band : band] & ~7;

				px = wrap(x - rowscroll[band], MAP_WIDTH);
				if (!flipped)
					start_x = -start_x;
				py = wrap(y - colscroll[((start_x + (px & ~7)) >> 3) & 0x3F], MAP_HEIGHT);
			}
			else if (rows == 1 && cols == 1)
			{
				px = wrap(x - rowscroll[0], MAP_WIDTH);
				py = wrap(y - colscroll[0], MAP_HEIGHT);
			}
			else if (cols == 1)
			{
				py = wrap(y - colscroll[0], MAP_HEIGHT);
				px = wrap(x - rowscroll[py / (MAP_HEIGHT / rows)], MAP_WIDTH);
			}
			else
			{
				px = wrap(x - rowscroll[0], MAP_WIDTH);
				py = wrap(y - colscroll[px / (MAP_WIDTH / cols)], MAP_HEIGHT);
			}
			if (flipped)
			{
				px = MAP_WIDTH - 1 - px;
				py = MAP_HEIGHT - 1 - py;
			}
			if (x < g_first[map] || x > g_last[map])
			{
				if (opaque)
					line[x] = BLACK_PEN;
				continue;
			}
			pen = tile_pixel(map, px, py, &colour, &tile);
			if (opaque || pen)
				line[x] = (uint16_t)(colour * 16 + pen);
			if (pen && tmnt_video_tiles_shown)
			{
				tmnt_video_tiles_shown[tile] |= (uint8_t)(1 << map);
				if (tmnt_video_tile_palettes)
					tmnt_video_tile_palettes[tile] = (uint8_t)colour;
			}
		}
	}
}

/* A 16 x 16 sprite cell's pen (k051960's spritelayout_reverse: four 8 x 8 quarters) */
static int sprite_pixel(int code, int x, int y)
{
	const uint8_t *data = &game_sprites[(size_t)(code % SPRITES) * 128u + (size_t)((y & 8) * 8 + (x & 8) * 4 + (y & 7) * 4)];
	const int column = x & 7;

	return ((data[3] >> (7 - column)) & 1) << 3 | ((data[2] >> (7 - column)) & 1) << 2
		| ((data[1] >> (7 - column)) & 1) << 1 | ((data[0] >> (7 - column)) & 1);
}

/* drawgfx's transtable for one cell: pen 0 left out, pen 15 a shadow when the sprite casts one */
static void draw_cell(int code, int colour, bool flip_x, bool flip_y, int left, int top, bool shadow)
{
	for (int y = 0; y < 16; y++)
	{
		const int screen_y = top + y;

		if (screen_y < CLIP_TOP || screen_y > CLIP_BOTTOM)
			continue;
		for (int x = 0; x < 16; x++)
		{
			const int screen_x = left + x;
			int pen;
			uint16_t *pixel;

			if (screen_x < CLIP_LEFT - g_left || screen_x > CLIP_RIGHT + g_right)
				continue;
			pen = sprite_pixel(code, flip_x ? 15 - x : x, flip_y ? 15 - y : y);
			if (!pen)
				continue;
			if (tmnt_video_sprites_shown)
			{
				tmnt_video_sprites_shown[code % SPRITES] = 1;
				if (tmnt_video_sprite_palettes)
					tmnt_video_sprite_palettes[code % SPRITES] = (uint8_t)colour;
			}
			pixel = &g_bitmap[screen_y * BITMAP_WIDTH + BITMAP_LEFT + screen_x];
			if (pen == 15 && shadow)
			{
				if (*pixel < PENS)
					*pixel = (uint16_t)(*pixel + PENS);
			}
			else
				*pixel = (uint16_t)(colour * 16 + pen);
		}
	}
}

/* k051960_sprites_draw, as tmnt.cpp calls it (no priority buffer, priorities 0 to 0) */
static void draw_sprites(void)
{
	static const int x_offset[8] = { 0, 1, 4, 5, 16, 17, 20, 21 };
	static const int y_offset[8] = { 0, 2, 8, 10, 32, 34, 40, 42 };
	static const int width[8] = { 1, 2, 1, 2, 4, 2, 4, 8 };
	static const int height[8] = { 1, 1, 2, 2, 2, 4, 4, 8 };
	int sorted[128];

	for (int index = 0; index < 128; index++)
		sorted[index] = -1;
	for (int offset = 0; offset < TMNT_SPRITE_RAM_SIZE; offset += 8)
		if (g_sprite_buffer[offset] & 0x80)
			sorted[g_sprite_buffer[offset] & 0x7F] = offset;

	for (int priority = 0; priority < 128; priority++)
	{
		const uint8_t *sprite;
		int code, colour, size, w, h, ox, oy;
		bool shadow, flip_x, flip_y;

		if (sorted[priority] < 0)
			continue;
		sprite = &g_sprite_buffer[sorted[priority]];
		code = sprite[2] + ((sprite[1] & 0x1F) << 8);
		colour = sprite[3];
		shadow = !(tmnt_board.shadow_config & 4) && ((tmnt_board.shadow_config & 2) || (colour & 0x80));
		/* tmnt.cpp's sprite callback */
		code |= (colour & 0x10) << 9;
		colour = k_sprite_colour_base + (colour & 0x0F);
		size = (sprite[1] & 0xE0) >> 5;
		w = width[size];
		h = height[size];
		if (w >= 2) code &= ~0x01;
		if (h >= 2) code &= ~0x02;
		if (w >= 4) code &= ~0x04;
		if (h >= 4) code &= ~0x08;
		if (w >= 8) code &= ~0x10;
		if (h >= 8) code &= ~0x20;
		ox = (256 * sprite[6] + sprite[7]) & 0x1FF;
		if (g_left || g_right)
		{
			/* (WIDE: the tenth bit, x from -256 to 767, no wrap at 512) */
			ox |= (sprite[6] & 0x04) << 7;
			if (ox >= 768)
				ox -= 1024;
		}
		oy = 256 - ((256 * sprite[4] + sprite[5]) & 0x1FF);
		flip_x = (sprite[6] & 0x02) != 0;
		flip_y = (sprite[4] & 0x02) != 0;
		/* (the zoom bits, 2-7 of bytes 4 and 6: tmnt never sets them, so the cells keep their 16 pixels) */
		if (tmnt_board.sprite_control & 0x08)
		{
			ox = 512 - 16 * w - ox;
			oy = 256 - 16 * h - oy;
			flip_x = !flip_x;
			flip_y = !flip_y;
		}
		for (int y = 0; y < h; y++)
			for (int x = 0; x < w; x++)
			{
				const int cell = code + x_offset[flip_x ? w - 1 - x : x] + y_offset[flip_y ? h - 1 - y : y];

				draw_cell(cell, colour, flip_x, flip_y, (g_left || g_right ? ox + 16 * x : (ox + 16 * x) & 0x1FF) - 96,
					oy + 16 * y, shadow);
			}
	}
}

static uint8_t pal5bit(int value)
{
	return (uint8_t)((value << 3) | (value >> 2));
}

static uint8_t shade(uint8_t component)
{
	const int value = (int)((float)component * SHADOW_FACTOR);

	return (uint8_t)(value < 0 ? 0 : value > 255 ? 255 : value);
}

void tmnt_video_set_layer_window(int map, int first, int last)
{
	if (map >= 1 && map <= 2)
	{
		g_first_next[map] = first;
		g_last_next[map] = last;
	}
}

void tmnt_video_set_wide(int left, int right)
{
	g_left_next = left < 0 ? 0 : (left > TMNT_VIDEO_MAX_LEFT ? TMNT_VIDEO_MAX_LEFT : left);
	g_right_next = right < 0 ? 0 : (right > TMNT_VIDEO_MAX_MORE - g_left_next ? TMNT_VIDEO_MAX_MORE - g_left_next : right);
}

const uint32_t *tmnt_video_wide_frame(int *width)
{
	*width = g_wide_width;
	return g_wide;
}

int tmnt_video_wide_left(void)
{
	return g_wide_width > TMNT_VIDEO_WIDTH ? g_wide_left : 0;
}

void tmnt_video_vblank_start(tmnt_frame *frame)
{
	g_left = g_left_next;
	g_right = g_right_next;
	for (int map = 0; map < 3; map++)
	{
		/* (the maps' ends only with a wider picture: the board's own shows them all) */
		g_first[map] = g_left || g_right ? g_first_next[map] : -0x10000;
		g_last[map] = g_left || g_right ? g_last_next[map] : 0x10000;
	}
	if (frame)
	{
		uint32_t pens[2 * PENS];

		for (int entry = 0; entry < PENS; entry++)
		{
			const int raw = tmnt_board.palette[entry * 2] << 8 | tmnt_board.palette[entry * 2 + 1];
			const uint8_t red = pal5bit(raw & 0x1F), green = pal5bit((raw >> 5) & 0x1F), blue = pal5bit((raw >> 10) & 0x1F);

			pens[entry] = (uint32_t)red << 16 | (uint32_t)green << 8 | blue;
			pens[PENS + entry] = (uint32_t)shade(red) << 16 | (uint32_t)shade(green) << 8 | shade(blue);
		}
		/* tmnt.cpp's screen_update */
		draw_layer(2, true);
		if ((tmnt_board.priority & 1) == 1)
			draw_sprites();
		draw_layer(1, false);
		if ((tmnt_board.priority & 1) == 0)
			draw_sprites();
		draw_layer(0, false);
		for (int y = 0; y < TMNT_VIDEO_HEIGHT; y++)
			for (int x = 0; x < TMNT_VIDEO_WIDTH; x++)
			{
				const uint16_t pen = g_bitmap[(CLIP_TOP + y) * BITMAP_WIDTH + BITMAP_LEFT + x];

				frame->pixels[y * TMNT_VIDEO_WIDTH + x] = pen == BLACK_PEN ? 0u : pens[pen % (2 * PENS)];
			}
		/* WIDE's picture: the same with its sides */
		g_wide_width = TMNT_VIDEO_WIDTH + g_left + g_right;
		g_wide_left = g_left;
		for (int y = 0; (g_left || g_right) && y < TMNT_VIDEO_HEIGHT; y++)
			for (int x = 0; x < g_wide_width; x++)
			{
				const uint16_t pen = g_bitmap[(CLIP_TOP + y) * BITMAP_WIDTH + BITMAP_LEFT - g_left + x];

				g_wide[y * TMNT_VIDEO_MAX_WIDTH + x] = pen == BLACK_PEN ? 0u : pens[pen % (2 * PENS)];
			}
	}
	/* the K051960's DMA, unless the 051937 has the sprites off */
	if (!(tmnt_board.sprite_control & 0x10))
		memcpy(g_sprite_buffer, tmnt_board.sprite_ram, sizeof(g_sprite_buffer));
}

uint64_t tmnt_video_hash(const tmnt_frame *frame)
{
	uint64_t hash = 0xcbf29ce484222325ull;

	for (int index = 0; index < TMNT_VIDEO_WIDTH * TMNT_VIDEO_HEIGHT; index++)
		for (int shift = 16; shift >= 0; shift -= 8)
			hash = (hash ^ ((frame->pixels[index] >> shift) & 0xFFu)) * 0x100000001b3ull;
	return hash;
}
