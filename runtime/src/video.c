/**
 *    Name:        video.c
 *    Description: TMNT's video: the K052109's three tile layers, the K051960's sprites and the palette, drawn
 *                 from the board's memories as the board shows them
 *    Version:     0.2.0
 *    Created:     29-10-2026
 *    Modified:    20-11-2026
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
 *
 *    It is drawn a tile's row at a time, and a sprite cell's: a row's 8 pixels are four bytes of the ROM, a bit
 *    plane each, which k_spread turns into 8 pens in one number (row_pens). A layer with one scroll, as nearly all
 *    the game's are, is drawn a tile at a time (draw_tiles: a tile's rows are together in the ROM, and it is looked
 *    up once), an empty one left out at once; the others a line at a time, the rows of the tiles it crosses
 *    (draw_layer). The palette's colours are worked out again only where the palette RAM changed (update_colours).
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

/* A bit plane's byte of a row (its leftmost pixel in bit 7) with its bits a nibble apart, the leftmost pixel's the
   lowest: the four planes' put together are the row's 8 pens, pixel n's in bits 4n to 4n + 3 (row_pens) */
#define SPREAD(byte)    ((uint32_t)(((byte) >> 7 & 1) | ((byte) >> 6 & 1) << 4 | ((byte) >> 5 & 1) << 8 \
	| ((byte) >> 4 & 1) << 12 | ((byte) >> 3 & 1) << 16 | ((byte) >> 2 & 1) << 20 | ((byte) >> 1 & 1) << 24 \
	| ((byte) & 1) << 28))
#define SPREAD_4(byte)  SPREAD(byte), SPREAD((byte) + 1), SPREAD((byte) + 2), SPREAD((byte) + 3)
#define SPREAD_16(byte) SPREAD_4(byte), SPREAD_4((byte) + 4), SPREAD_4((byte) + 8), SPREAD_4((byte) + 12)
#define SPREAD_64(byte) SPREAD_16(byte), SPREAD_16((byte) + 16), SPREAD_16((byte) + 32), SPREAD_16((byte) + 48)

static const uint32_t k_spread[256] = { SPREAD_64(0), SPREAD_64(64), SPREAD_64(128), SPREAD_64(192) };

static layer g_layers[3];
static uint8_t g_sprite_buffer[TMNT_SPRITE_RAM_SIZE];  /* the K051960's DMA copy */
static uint16_t g_bitmap[BITMAP_HEIGHT * BITMAP_WIDTH];
static uint32_t g_colours[2 * PENS + 1];               /* the pens' colours, the shaded ones' after them, and black */
static uint8_t g_palette_seen[TMNT_PALETTE_SIZE];      /* the palette RAM g_colours was made from */
static int g_left, g_right, g_left_next, g_right_next;    /* WIDE: the columns drawn more at each side */
static int g_first[3] = { -0x10000, -0x10000, -0x10000 }, g_last[3] = { 0x10000, 0x10000, 0x10000 };
static int g_first_next[3] = { -0x10000, -0x10000, -0x10000 }, g_last_next[3] = { 0x10000, 0x10000, 0x10000 };
                                        /* WIDE: the maps' ends on the screen (the fixed layer's never set) */
#define BLACK_PEN       (2 * PENS)      /* (WIDE: past a map's end on layer B; black, after the shaded pens) */
static uint32_t g_wide[TMNT_VIDEO_MAX_WIDTH * TMNT_VIDEO_HEIGHT];
static int g_wide_width = TMNT_VIDEO_WIDTH;
static int g_wide_left;    /* (g_left when g_wide was drawn) */
static int g_band_dx, g_band_rows, g_band_dx_next, g_band_rows_next;    /* WIDE: the fixed layer's top rows moved */

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

/* The 8 pens of a row of a tile, or of half a sprite cell's row (four bytes of the ROM, a bit plane each), its
   leftmost pixel's in the lowest nibble */
static uint32_t row_pens(const uint8_t *data)
{
	return k_spread[data[0]] | k_spread[data[1]] << 1 | k_spread[data[2]] << 2 | k_spread[data[3]] << 3;
}

/* A row's pens the other way round (a flip in x) */
static uint32_t pens_reversed(uint32_t pens)
{
	pens = (pens & 0x0F0F0F0Fu) << 4 | (pens >> 4 & 0x0F0F0F0Fu);
	return pens << 24 | (pens & 0xFF00u) << 8 | (pens >> 8 & 0xFF00u) | pens >> 24;
}

/* A whole row's 8 pens onto a line, in the colour whose first pen is 'base' */
static void put_row(uint16_t *line, uint32_t pens, int base)
{
	line[0] = (uint16_t)(base + (pens & 15));
	line[1] = (uint16_t)(base + (pens >> 4 & 15));
	line[2] = (uint16_t)(base + (pens >> 8 & 15));
	line[3] = (uint16_t)(base + (pens >> 12 & 15));
	line[4] = (uint16_t)(base + (pens >> 16 & 15));
	line[5] = (uint16_t)(base + (pens >> 20 & 15));
	line[6] = (uint16_t)(base + (pens >> 24 & 15));
	line[7] = (uint16_t)(base + (pens >> 28));
}

/* A cell of a layer's map: its tile in the ROM and how it shows (k052109.cpp's get_tile_info, tmnt.cpp's callback) */
typedef struct
{
	int index;              /* the cell: its row * 64 + its column; -1 none yet */
	const uint8_t *data;    /* the tile's 8 rows in the ROM */
	int code, colour;
	bool flip_y;            /* its rows upside down */
	bool empty;             /* all its pixels pen 0 */
} tile;

static void take_tile(int map, int index, tile *into)
{
	const uint8_t *ram = tmnt_board.tile_ram;
	int attribute = ram[COLOUR_RAM + map * 0x800 + index];
	int code = ram[CODE_RAM + map * 0x800 + index] + 256 * ram[CODE_RAM_2 + map * 0x800 + index];
	int bank = tmnt_board.char_rom_bank[(attribute & 0x0C) >> 2];
	uint64_t rows[4];

	if (!(ram[REG_ADDRESS_MAP] & 0x40))
		attribute = (attribute & 0xF3) | ((bank & 3) << 2);
	bank >>= 2;
	code |= ((attribute & 0x03) << 8) | ((attribute & 0x10) << 6) | ((attribute & 0x0C) << 9) | (bank << 13);
	into->index = index;
	into->code = code % TILES;
	into->colour = k_colour_base[map] + ((attribute & 0xE0) >> 5);
	into->flip_y = (attribute & 0x02) && (ram[REG_FLIP] & 0x04);
	into->data = &game_tiles[(size_t)into->code * 32u];
	memcpy(rows, into->data, sizeof(rows));
	into->empty = !(rows[0] | rows[1] | rows[2] | rows[3]);
}

/* A row of a tile's as a line shows it: the pens of 'count' of its pixels, the first in the lowest nibble, after
   the 'before' that are left of them on the screen. 'py' is the map's row; 'flipped', the screen turned round */
static uint32_t shown_pens(const tile *cell, int py, bool flipped, int before, int count)
{
	uint32_t pens = row_pens(cell->data + (cell->flip_y ? 7 - (py & 7) : py & 7) * 4);

	if (flipped)
		pens = pens_reversed(pens);    /* (the map's columns from right to left) */
	pens >>= 4 * before;
	return count < 8 - before ? pens & ((1u << (4 * count)) - 1u) : pens;
}

/* Those pens onto a line in the colour whose first pen is 'base': all of them, or those that are not pen 0 */
static void put_pens(uint16_t *line, uint32_t pens, int count, int base, bool opaque)
{
	if (count == 8 && (opaque || ((pens | pens >> 1 | pens >> 2 | pens >> 3) & 0x11111111u) == 0x11111111u))
		put_row(line, pens, base);    /* (a whole row, none of its pixels left out) */
	else if (opaque)
		for (int pixel = 0; pixel < count; pixel++, pens >>= 4)
			line[pixel] = (uint16_t)(base + (pens & 15));
	else
		for (; pens; pens >>= 4, line++)
			if (pens & 15)
				*line = (uint16_t)(base + (pens & 15));
}

/* The screen's columns the layer 'map' shows on a line, 'from' to 'to' (none: from past to), and how far right of
   its place the map is drawn there: all the columns drawn, WIDE's sides with them, but only the board's 320, moved,
   on the fixed layer's band (WIDE: tmnt_video_set_fixed_band), and up to the map's ends
   (tmnt_video_set_layer_window) */
static int layer_columns(int map, bool banded, int *from, int *to)
{
	const int moved = banded ? g_band_dx : 0;

	*from = CLIP_LEFT - g_left;
	*to = CLIP_RIGHT + g_right;
	if (banded)
	{
		*from = *from > CLIP_LEFT + moved ? *from : CLIP_LEFT + moved;
		*to = *to < CLIP_RIGHT + moved ? *to : CLIP_RIGHT + moved;
	}
	*from = *from > g_first[map] ? *from : g_first[map];
	*to = *to < g_last[map] ? *to : g_last[map];
	return moved;
}

/* WIDE: black on a line of the layer drawn whole, past the map's ends */
static void black_past(uint16_t *line, int from, int to)
{
	const int first = CLIP_LEFT - g_left, last = CLIP_RIGHT + g_right;

	for (int x = first; x < from && x <= last; x++)
		line[x] = BLACK_PEN;
	for (int x = to + 1 > first ? to + 1 : first; x <= last; x++)
		line[x] = BLACK_PEN;
}

/* A layer with one scroll, a tile at a time: the screen's lines 'top' to 'bottom', its columns 'from' to 'to', the
   screen's (x, y) showing the map's (x - scroll_x, y - scroll_y), turned round when 'flipped' */
static void draw_tiles(int map, bool opaque, bool flipped, int top, int bottom, int from, int to, int scroll_x,
	int scroll_y)
{
	const int first_px = wrap(from - scroll_x, MAP_WIDTH);

	for (int y = top, lines; y <= bottom; y += lines)
	{
		int py = wrap(y - scroll_y, MAP_HEIGHT), px = first_px;

		/* the lines that cross this row of tiles: down the tiles' rows, or up them when flipped */
		if (flipped)
			py = MAP_HEIGHT - 1 - py;
		lines = flipped ? (py & 7) + 1 : 8 - (py & 7);
		lines = lines < bottom - y + 1 ? lines : bottom - y + 1;
		for (int x = from, count; x <= to; x += count, px = (px + count) & (MAP_WIDTH - 1))
		{
			const int before = px & 7;    /* (the tile's pixels left of these) */
			uint16_t *line = &g_bitmap[y * BITMAP_WIDTH + BITMAP_LEFT + x];
			tile cell;

			count = 8 - before < to - x + 1 ? 8 - before : to - x + 1;
			take_tile(map, (py >> 3) * 64 + ((flipped ? MAP_WIDTH - 1 - px : px) >> 3), &cell);
			if (cell.empty && !opaque)
				continue;
			for (int row = 0; row < lines; row++, line += BITMAP_WIDTH)
				put_pens(line, shown_pens(&cell, flipped ? py - row : py + row, flipped, before, count), count,
					cell.colour * 16, opaque);
		}
	}
}

/* k052109's tilemap_draw: the map 'map' over the bitmap, all its pixels or only those that are not pen 0 */
static void draw_layer(int map, bool opaque)
{
	const layer *scroll = &g_layers[map];
	const bool flipped = (tmnt_board.tile_ram[REG_FLIP] & 0x01) != 0;
	const int band_rows = map == 0 ? g_band_rows : 0;    /* (the first lines, the fixed layer's band's) */
	int rowscroll[MAP_HEIGHT], colscroll[MAP_WIDTH / 8];
	tile tiles[MAP_WIDTH / 8];    /* the cell drawn last at each column of the map: the next line's too, most times */
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

	if (!mixed && rows == 1 && cols == 1 && !tmnt_video_tiles_shown)
	{
		/* one scroll: a tile at a time, the band's lines and then the rest (what was shown is marked a line at a
		   time, below: tmnt_video_tile_palettes keeps each code's last pixel's, in the order of the lines) */
		for (int part = 0; part < 2; part++)
		{
			const int top = part == 0 ? CLIP_TOP : CLIP_TOP + band_rows;
			const int bottom = part == 0 ? CLIP_TOP + band_rows - 1 : CLIP_BOTTOM;
			int from, to;
			const int moved = layer_columns(map, part == 0, &from, &to);

			for (int y = top; opaque && y <= bottom; y++)
				black_past(&g_bitmap[y * BITMAP_WIDTH + BITMAP_LEFT], from, to);
			if (from <= to)
				draw_tiles(map, opaque, flipped, top, bottom, from, to, moved + rowscroll[0], colscroll[0]);
		}
		return;
	}
	for (int column = 0; column < MAP_WIDTH / 8; column++)
		tiles[column].index = -1;
	for (int y = CLIP_TOP; y <= CLIP_BOTTOM; y++)
	{
		uint16_t *line = &g_bitmap[y * BITMAP_WIDTH + BITMAP_LEFT];
		int from, to;    /* the screen's columns the map shows on this line */
		const int moved = layer_columns(map, y < CLIP_TOP + band_rows, &from, &to);
		int row_scroll, start_x = 0, line_py = 0, px;
		bool by_column = false;    /* the map's row is each column's own (its scroll) */

		if (opaque)
			black_past(line, from, to);
		if (from > to)
			continue;
		if (mixed)
		{
			const int band = y / (MAP_HEIGHT / scroll->rows);

			start_x = scroll->rowscroll[flipped ? scroll->rows - 1 - band : band] & ~7;
			if (!flipped)
				start_x = -start_x;
			row_scroll = rowscroll[band];
			by_column = true;
		}
		else if (rows == 1 && cols == 1)
		{
			row_scroll = rowscroll[0];
			line_py = wrap(y - colscroll[0], MAP_HEIGHT);
		}
		else if (cols == 1)
		{
			line_py = wrap(y - colscroll[0], MAP_HEIGHT);
			row_scroll = rowscroll[line_py / (MAP_HEIGHT / rows)];
		}
		else
		{
			row_scroll = rowscroll[0];
			by_column = true;
		}
		/* the line, the part of a tile's row at a time: 'count' pixels from the map's column px on */
		px = wrap(from - moved - row_scroll, MAP_WIDTH);
		for (int x = from, count; x <= to; x += count, px = (px + count) & (MAP_WIDTH - 1))
		{
			const int before = px & 7;    /* (the row's pixels left of these) */
			int py = line_py, column = px >> 3;
			uint32_t pens;
			tile *cell;

			count = 8 - before < to - x + 1 ? 8 - before : to - x + 1;
			if (by_column)
				py = wrap(y - colscroll[mixed ? ((start_x + (px & ~7)) >> 3) & 0x3F : px / (MAP_WIDTH / cols)], MAP_HEIGHT);
			if (flipped)
			{
				column = (MAP_WIDTH - 1 - px) >> 3;
				py = MAP_HEIGHT - 1 - py;
			}
			cell = &tiles[column];
			if (cell->index != (py >> 3) * 64 + column)
				take_tile(map, (py >> 3) * 64 + column, cell);
			if (cell->empty && !opaque)
				continue;
			pens = shown_pens(cell, py, flipped, before, count);
			put_pens(&line[x], pens, count, cell->colour * 16, opaque);
			if (pens && tmnt_video_tiles_shown)
			{
				tmnt_video_tiles_shown[cell->code] |= (uint8_t)(1 << map);
				if (tmnt_video_tile_palettes)
					tmnt_video_tile_palettes[cell->code] = (uint8_t)cell->colour;
			}
		}
	}
}

/* drawgfx's transtable for one cell (k051960's spritelayout_reverse: 16 x 16 pixels in four 8 x 8 quarters): pen 0
   left out, pen 15 a shadow when the sprite casts one; a row at a time, its 16 pens in one number */
static void draw_cell(int code, int colour, bool flip_x, bool flip_y, int left, int top, bool shadow)
{
	const uint8_t *data = &game_sprites[(size_t)(code % SPRITES) * 128u];
	/* (the cell's columns on the screen) */
	const int first = CLIP_LEFT - g_left - left > 0 ? CLIP_LEFT - g_left - left : 0;
	const int last = CLIP_RIGHT + g_right - left < 15 ? CLIP_RIGHT + g_right - left : 15;
	const int base = colour * 16;
	bool shown = false;

	if (first > last)
		return;
	for (int y = 0; y < 16; y++)
	{
		const int screen_y = top + y, row = flip_y ? 15 - y : y;
		const uint8_t *halves = data + (row & 8) * 8 + (row & 7) * 4;
		uint64_t pens;
		uint16_t *pixel;

		if (screen_y < CLIP_TOP || screen_y > CLIP_BOTTOM)
			continue;
		pens = row_pens(halves) | (uint64_t)row_pens(halves + 32) << 32;
		if (!pens)
			continue;
		if (flip_x)
			pens = (uint64_t)pens_reversed((uint32_t)pens) << 32 | pens_reversed((uint32_t)(pens >> 32));
		pens >>= 4 * first;
		if (last < 15)
			pens &= ((uint64_t)1 << (4 * (last - first + 1))) - 1u;
		if (!pens)
			continue;
		shown = true;
		pixel = &g_bitmap[screen_y * BITMAP_WIDTH + BITMAP_LEFT + left + first];
		for (; pens; pens >>= 4, pixel++)
		{
			const int pen = (int)(pens & 15);

			if (!pen)
				continue;
			if (pen == 15 && shadow)
			{
				if (*pixel < PENS)
					*pixel = (uint16_t)(*pixel + PENS);
			}
			else
				*pixel = (uint16_t)(base + pen);
		}
	}
	if (shown && tmnt_video_sprites_shown)
	{
		tmnt_video_sprites_shown[code % SPRITES] = 1;
		if (tmnt_video_sprite_palettes)
			tmnt_video_sprite_palettes[code % SPRITES] = (uint8_t)colour;
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

/* The pens' colours from the palette RAM, the entries that changed since the last frame drawn: all of them the
   first time (the RAM seen and the colours start as zeroes, which are each other's) */
static void update_colours(void)
{
	const uint8_t *palette = tmnt_board.palette;

	if (memcmp(g_palette_seen, palette, sizeof(g_palette_seen)) == 0)
		return;
	for (int entry = 0; entry < PENS; entry++)
	{
		const int raw = palette[entry * 2] << 8 | palette[entry * 2 + 1];
		const uint8_t red = pal5bit(raw & 0x1F), green = pal5bit((raw >> 5) & 0x1F), blue = pal5bit((raw >> 10) & 0x1F);

		if (g_palette_seen[entry * 2] == palette[entry * 2] && g_palette_seen[entry * 2 + 1] == palette[entry * 2 + 1])
			continue;
		g_colours[entry] = (uint32_t)red << 16 | (uint32_t)green << 8 | blue;
		g_colours[PENS + entry] = (uint32_t)shade(red) << 16 | (uint32_t)shade(green) << 8 | shade(blue);
	}
	memcpy(g_palette_seen, palette, sizeof(g_palette_seen));
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

void tmnt_video_set_fixed_band(int dx, int rows)
{
	g_band_dx_next = dx;
	g_band_rows_next = rows < 0 ? 0 : (rows > TMNT_VIDEO_HEIGHT ? TMNT_VIDEO_HEIGHT : rows);
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
	g_band_dx = g_left || g_right ? g_band_dx_next : 0;    /* (only with a wider picture) */
	g_band_rows = g_left || g_right ? g_band_rows_next : 0;
	for (int map = 0; map < 3; map++)
	{
		/* (the maps' ends only with a wider picture: the board's own shows them all) */
		g_first[map] = g_left || g_right ? g_first_next[map] : -0x10000;
		g_last[map] = g_left || g_right ? g_last_next[map] : 0x10000;
	}
	if (frame)
	{
		const int left = g_left, width = TMNT_VIDEO_WIDTH + g_left + g_right;
		const bool wide = width > TMNT_VIDEO_WIDTH;

		update_colours();
		/* tmnt.cpp's screen_update */
		draw_layer(2, true);
		if ((tmnt_board.priority & 1) == 1)
			draw_sprites();
		draw_layer(1, false);
		if ((tmnt_board.priority & 1) == 0)
			draw_sprites();
		draw_layer(0, false);
		/* the pens into colours, a line at a time: the board's picture, or WIDE's with its sides and the board's
		   320 columns out of it */
		g_wide_width = width;
		g_wide_left = left;
		for (int y = 0; y < TMNT_VIDEO_HEIGHT; y++)
		{
			const uint16_t *line = &g_bitmap[(CLIP_TOP + y) * BITMAP_WIDTH + BITMAP_LEFT - left];
			uint32_t *board = &frame->pixels[y * TMNT_VIDEO_WIDTH];
			uint32_t *colours = wide ? &g_wide[y * TMNT_VIDEO_MAX_WIDTH] : board;

			for (int x = 0; x < width; x++)
				colours[x] = g_colours[line[x]];
			if (wide)
				memcpy(board, colours + left, TMNT_VIDEO_WIDTH * sizeof(uint32_t));
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
