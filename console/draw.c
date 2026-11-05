/**
 *    Name:        draw.c
 *    Description: the console's own picture over the frame the video drew: the game's own letters and texts from
 *                 the fixed layer's character set at any size, small letters of its own, the frame's sprites again
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The menus write with the game's own letters (the fixed layer's font: capitals, digits and a few signs), in its
 *    texts' white and light blue (Sebas), with an '@' of the console's own (the font's '@' is its space); "@cheviman"
 *    goes in small letters of the console's own. Kenney Mini (Kenney, CC0: font.bmp beside the executable) still
 *    loads, and nothing draws with it now. The game's own pictures (the letters, the Konami logo, the sprites) are
 *    drawn from the player's assets as the board has them (game/data.h's tile and sprite memories, the palette
 *    RAM), the way runtime/src/video.c draws them: nothing of the game's is in the console.
 */
#include "console_internal.h"

#include "game.h"
#include "game/data.h"
#include "tmnt/machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef sprite_control    /* (the work RAM's copy, ram.h: here the board's register, tmnt_board's) */

#define FONT_FILE      "font.bmp"
#define FONT_FIRST      0x20
#define FONT_LAST       0x7E
#define FONT_CELL       16          /* pixels: a character's cell, 16 a row in the picture */
#define FONT_ROW        16
#define FONT_SPACING    1           /* pixels between two characters, past the lit columns */
#define SPACE_WIDTH     4
#define SHADOW          0x000000u

#define WIDTH           TMNT_VIDEO_WIDTH
#define HEIGHT          TMNT_VIDEO_HEIGHT
#define TILES           (GAME_TILES_SIZE / 32)
#define SPRITES         (GAME_SPRITES_SIZE / 128)
#define FIXED_LAYER     0x100000u   /* the K052109's fixed layer as the program writes it: a cell 2 bytes, 64 a row */
#define FIXED_ROW       0x80u
#define TEXT_END        0x3Fu       /* '?' */
#define TEXT_NEXT       0x2Fu       /* '/' */
#define TEXT_FIRST      0x30u       /* the tile of a character is it less this */
#define OWN_AT          0x10000     /* (no tile: the console's own '@' and ':', own_glyphs) */
#define OWN_COLON       0x10001
#define OWN_LESS        0x10002
#define OWN_GREATER     0x10003

static struct
{
	bool loaded;
	uint8_t lit[6 * FONT_CELL][FONT_ROW * FONT_CELL];    /* the picture's pixels (6 rows of cells): lit or not */
	int widths[FONT_LAST - FONT_FIRST + 1];
	int top;                    /* the first lit row of a capital, in its cell */
} font;

/* ---- the letters ---- */

static uint32_t read_le(const uint8_t *bytes, int count)
{
	uint32_t value = 0;

	for (int index = count - 1; index >= 0; index--)
		value = value << 8 | bytes[index];
	return value;
}

bool draw_load_font(const char *folder)
{
	char path[1100];
	uint8_t header[54];
	uint8_t *pixels;
	FILE *input;
	int width, height, bits, stride;
	uint32_t offset;
	bool bottom_up;

	snprintf(path, sizeof(path), "%s" FONT_FILE, folder ? folder : "");
	if ((input = fopen(path, "rb")) == NULL)
		return false;
	if (fread(header, 1, sizeof(header), input) != sizeof(header) || header[0] != 'B' || header[1] != 'M')
	{
		fclose(input);
		return false;
	}
	offset = read_le(&header[10], 4);
	width = (int)read_le(&header[18], 4);
	height = (int)(int32_t)read_le(&header[22], 4);
	bits = (int)read_le(&header[28], 2);
	bottom_up = height > 0;
	height = abs(height);
	if ((bits != 24 && bits != 32) || width > FONT_ROW * FONT_CELL || height > (int)(sizeof(font.lit) / sizeof(font.lit[0])))
	{
		fclose(input);
		return false;
	}
	stride = (width * (bits / 8) + 3) & ~3;
	pixels = malloc((size_t)stride * (size_t)height);
	if (!pixels || fseek(input, (long)offset, SEEK_SET) != 0 || fread(pixels, 1, (size_t)stride * (size_t)height, input)
		!= (size_t)stride * (size_t)height)
	{
		free(pixels);
		fclose(input);
		return false;
	}
	fclose(input);
	memset(&font, 0, sizeof(font));
	for (int y = 0; y < height; y++)
	{
		const uint8_t *row = &pixels[(size_t)(bottom_up ? height - 1 - y : y) * (size_t)stride];

		for (int x = 0; x < width; x++)
		{
			const uint8_t *pixel = &row[x * (bits / 8)];

			font.lit[y][x] = (pixel[0] | pixel[1] | pixel[2]) != 0;
		}
	}
	free(pixels);
	/* each character as wide as its last lit column; the capitals' first lit row */
	font.top = FONT_CELL;
	for (int index = 0; index <= FONT_LAST - FONT_FIRST; index++)
	{
		const int left = (index % FONT_ROW) * FONT_CELL, top = (index / FONT_ROW) * FONT_CELL;

		for (int y = 0; y < FONT_CELL; y++)
			for (int x = 0; x < FONT_CELL; x++)
				if (font.lit[top + y][left + x])
				{
					if (x + 1 > font.widths[index])
						font.widths[index] = x + 1;
					if (index + FONT_FIRST >= 'A' && index + FONT_FIRST <= 'Z' && y < font.top)
						font.top = y;
				}
	}
	font.widths[0] = SPACE_WIDTH;
	if (font.top == FONT_CELL)
		font.top = 0;
	font.loaded = true;
	return true;
}

int draw_text_width(const char *text, int scale)
{
	int width = 0;

	if (!font.loaded)
		return 0;
	for (; *text; text++)
		if (*text >= FONT_FIRST && *text <= FONT_LAST)
			width += (font.widths[*text - FONT_FIRST] + FONT_SPACING) * scale;
	return width > 0 ? width - FONT_SPACING * scale : 0;
}

static void put(tmnt_frame *frame, int x, int y, uint32_t colour)
{
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
		frame->pixels[y * WIDTH + x] = colour;
}

static void put_block(tmnt_frame *frame, int x, int y, int scale, uint32_t colour)
{
	for (int dy = 0; dy < scale; dy++)
		for (int dx = 0; dx < scale; dx++)
			put(frame, x + dx, y + dy, colour);
}

static void letters(tmnt_frame *frame, int x, int y, int scale, uint32_t colour, const char *text)
{
	for (; *text; text++)
	{
		int index, left, top;

		if (*text < FONT_FIRST || *text > FONT_LAST)
			continue;
		index = *text - FONT_FIRST;
		left = (index % FONT_ROW) * FONT_CELL;
		top = (index / FONT_ROW) * FONT_CELL;
		for (int row = font.top; row < FONT_CELL; row++)
			for (int column = 0; column < font.widths[index]; column++)
				if (font.lit[top + row][left + column])
					put_block(frame, x + column * scale, y + (row - font.top) * scale, scale, colour);
		x += (font.widths[index] + FONT_SPACING) * scale;
	}
}

void draw_text(tmnt_frame *frame, int x, int y, int scale, uint32_t colour, const char *text)
{
	if (!font.loaded)
		return;
	letters(frame, x + scale, y + scale, scale, SHADOW, text);
	letters(frame, x, y, scale, colour, text);
}

void draw_text_centred(tmnt_frame *frame, int y, int scale, uint32_t colour, const char *text)
{
	draw_text(frame, LEFT_STRIPE + (WIDTH - LEFT_STRIPE - draw_text_width(text, scale)) / 2, y, scale, colour, text);
}

/* ---- rectangles ---- */

static uint32_t darkened(uint32_t colour, int keep)
{
	const uint32_t red = ((colour >> 16) & 0xFFu) * (uint32_t)keep >> 8;
	const uint32_t green = ((colour >> 8) & 0xFFu) * (uint32_t)keep >> 8;
	const uint32_t blue = (colour & 0xFFu) * (uint32_t)keep >> 8;

	return red << 16 | green << 8 | blue;
}

void draw_shade(tmnt_frame *frame, int x, int y, int width, int height, int keep)
{
	for (int row = y < 0 ? 0 : y; row < y + height && row < HEIGHT; row++)
		for (int column = x < 0 ? 0 : x; column < x + width && column < WIDTH; column++)
			frame->pixels[row * WIDTH + column] = darkened(frame->pixels[row * WIDTH + column], keep);
}

void draw_fill(tmnt_frame *frame, int x, int y, int width, int height, uint32_t colour)
{
	for (int row = y < 0 ? 0 : y; row < y + height && row < HEIGHT; row++)
		for (int column = x < 0 ? 0 : x; column < x + width && column < WIDTH; column++)
			frame->pixels[row * WIDTH + column] = colour;
}

/* ---- the board's colours and graphics ---- */

static uint8_t five_bits(int value)
{
	return (uint8_t)((value << 3) | (value >> 2));
}

/* A colour of the palette RAM (xBGR 555), as the video turns it into the frame's */
static uint32_t palette_colour(int entry)
{
	const int raw = tmnt_board.palette[(entry & 0x3FF) * 2] << 8 | tmnt_board.palette[(entry & 0x3FF) * 2 + 1];

	return (uint32_t)five_bits(raw & 0x1F) << 16 | (uint32_t)five_bits((raw >> 5) & 0x1F) << 8
		| five_bits((raw >> 10) & 0x1F);
}

/* A tile's pen (k052109: four planes, a byte each for a row of 8) */
static int tile_pen(int code, int x, int y)
{
	const uint8_t *data = &game_tiles[(size_t)(code % TILES) * 32u + (size_t)y * 4u];
	const int column = x & 7;

	return ((data[3] >> (7 - column)) & 1) << 3 | ((data[2] >> (7 - column)) & 1) << 2
		| ((data[1] >> (7 - column)) & 1) << 1 | ((data[0] >> (7 - column)) & 1);
}

static void draw_tile(tmnt_frame *frame, int code, int bank, int x, int y, int scale, int lit_to, int dark)
{
	for (int row = 0; row < 8; row++)
		for (int column = 0; column < 8; column++)
		{
			const int pen = tile_pen(code, column, row);
			uint32_t colour;

			if (!pen)
				continue;
			colour = palette_colour(bank * 16 + pen);
			if (x + column * scale >= lit_to)
				colour = darkened(colour, dark);
			put_block(frame, x + column * scale, y + row * scale, scale, colour);
		}
}

/* Each cell of a text of game_texts (its places in the fixed layer, its characters) to 'visit' (its column and
   row in the layer, its tile), as print_table_text writes them */
typedef void (*cell_visitor)(int column, int row, int code, void *context);

static void walk_game_text(uint32_t text_number, cell_visitor visit, void *context)
{
	uint32_t text = TEXT_POINTER(game_texts_ADDRESS + text_number * 4u);

	for (;;)
	{
		const uint32_t place = TEXT_32(text) & 0xFFFFFFu;
		int column = (int)((place - FIXED_LAYER) % FIXED_ROW) / 2;
		const int row = (int)((place - FIXED_LAYER) / FIXED_ROW);

		text += 4u;
		for (;;)
		{
			const uint8_t character = TEXT(text);

			text++;
			if (character == TEXT_END)
				return;
			if (character == TEXT_NEXT)
				break;
			visit(column++, row, character - TEXT_FIRST, context);
		}
	}
}

typedef struct
{
	int left, top, right, bottom;
} cell_box;

static void measure_cell(int column, int row, int code, void *context)
{
	cell_box *box = context;

	if (code == 0x10)
		return;    /* (a space: the font's blank tile) */
	box->left = column < box->left ? column : box->left;
	box->top = row < box->top ? row : box->top;
	box->right = column > box->right ? column : box->right;
	box->bottom = row > box->bottom ? row : box->bottom;
}

static cell_box text_box(uint32_t text_number)
{
	cell_box box = { 1 << 20, 1 << 20, -1, -1 };

	walk_game_text(text_number, measure_cell, &box);
	return box;
}

void game_text_size(uint32_t text_number, int *width, int *height)
{
	const cell_box box = text_box(text_number);

	*width = box.right < box.left ? 0 : (box.right - box.left + 1) * 8;
	*height = box.bottom < box.top ? 0 : (box.bottom - box.top + 1) * 8;
}

typedef struct
{
	tmnt_frame *frame;
	cell_box box;
	int x, y, scale, bank, lit_to, dark;
} text_drawing;

static void draw_cell(int column, int row, int code, void *context)
{
	const text_drawing *drawing = context;

	draw_tile(drawing->frame, code, drawing->bank, drawing->x + (column - drawing->box.left) * 8 * drawing->scale,
		drawing->y + (row - drawing->box.top) * 8 * drawing->scale, drawing->scale, drawing->lit_to, drawing->dark);
}

void draw_game_text(tmnt_frame *frame, uint32_t text_number, int x, int y, int scale, int bank, int lit_to,
	int dark)
{
	text_drawing drawing = { frame, text_box(text_number), x, y, scale, bank, lit_to, dark };

	walk_game_text(text_number, draw_cell, &drawing);
}

/* ---- the game's own letters for the menus ---- */

/* The fixed layer's tile of a character, as the game's texts write them (the character less 0x30: digits 00-09,
   A-Z 11-2A; ',' '-' '.' '?' '!' are the texts' ';' '<' '=' '>' '?', 0B-0F; the apostrophe 2B); -1 for a space or
   one the font has not */
static int game_glyph(char character)
{
	if (character >= 'a' && character <= 'z')
		character = (char)(character - 'a' + 'A');
	if ((character >= '0' && character <= '9') || (character >= 'A' && character <= 'Z'))
		return character - (int)TEXT_FIRST;
	switch (character)
	{
		case ',': return 0x0B;
		case '-': return 0x0C;
		case '.': return 0x0D;
		case '?': return 0x0E;
		case '!': return 0x0F;
		case '\'': return 0x2B;
		case '@': return OWN_AT;
		case ':': return OWN_COLON;
		case '<': return OWN_LESS;
		case '>': return OWN_GREATER;
		default: return -1;
	}
}

/* Letters of the console's own in the font's manner, which it has not: '@' (its '@' is its space), ':' (WIDE's
   16:9), '<' and '>' (a value LEFT and RIGHT change, CONTROLS' PLAYER); their pens as the font's letters have them,
   1 the letter, B its shadow */
static const char *const own_glyphs[4][8] = {
	{
		".11111..",
		"11BBB11.",
		"11.111B1",
		"11.1B11B",
		"11.111B1",
		"11B.BBB.",
		".11111..",
		"..BBBBB.",
	},
	{
		"........",
		"..11....",
		"..11B...",
		"...BB...",
		"..11....",
		"..11B...",
		"...BB...",
		"........",
	},
	{
		"....11..",
		"...11BB.",
		"..11BB..",
		".11BB...",
		"..11....",
		"...11...",
		"....11..",
		".....BB.",
	},
	{
		".11.....",
		"..11....",
		"...11...",
		"....11..",
		"...11BB.",
		"..11BB..",
		".11BB...",
		"..BB....",
	},
};

static int letter_pen(int code, int x, int y)
{
	if (code >= OWN_AT)
	{
		const char pen = own_glyphs[code - OWN_AT][y][x];

		return pen == '1' ? 1 : pen == 'B' ? GAME_LETTERS_SHADOW_PEN : 0;
	}
	return tile_pen(code, x, y);
}

/* ---- the signature's small letters ---- */

/* A small font of the console's own, 5 pixels high, for "@cheviman" under the logo (Sebas: the game's 8 pixel one
   was too large there): the letters it needs, '#' lit; drawn in the game's texts' colours, a shadow under them */
static const struct
{
	char character;
	const char *rows[5];
} small_letters[] = {
	{ '@', { ".###.", "#.#.#", "#.###", "#....", ".###." } },
	{ 'c', { "...", ".##", "#..", "#..", ".##" } },
	{ 'h', { "#..", "#..", "##.", "#.#", "#.#" } },
	{ 'e', { "...", ".#.", "###", "#..", ".##" } },
	{ 'v', { "...", "#.#", "#.#", "#.#", ".#." } },
	{ 'i', { "#", ".", "#", "#", "#" } },
	{ 'm', { ".....", "##.#.", "#.#.#", "#.#.#", "#.#.#" } },
	{ 'a', { "...", ".##", "#.#", "#.#", ".##" } },
	{ 'n', { "...", "##.", "#.#", "#.#", "#.#" } },
};

static int small_letter(char character)
{
	for (int index = 0; index < (int)(sizeof(small_letters) / sizeof(small_letters[0])); index++)
		if (small_letters[index].character == character)
			return index;
	return -1;
}

int draw_small_letters_width(const char *text)
{
	int width = 0;

	for (; *text; text++)
	{
		const int index = small_letter(*text);

		width += (index < 0 ? 2 : (int)strlen(small_letters[index].rows[0])) + 1;
	}
	return width > 0 ? width - 1 : 0;
}

void draw_small_letters_centred(tmnt_frame *frame, int y, const char *text)
{
	int x = LEFT_STRIPE + (WIDTH - LEFT_STRIPE - draw_small_letters_width(text)) / 2;
	const uint32_t letter = palette_colour(GAME_LETTERS_BANK * 16 + 1);
	const uint32_t shadow = palette_colour(GAME_LETTERS_BANK * 16 + GAME_LETTERS_SHADOW_PEN);

	for (; *text; text++)
	{
		const int index = small_letter(*text);
		int width;

		if (index < 0)
		{
			x += 3;
			continue;
		}
		width = (int)strlen(small_letters[index].rows[0]);
		for (int pass = 0; pass < 2; pass++)
			for (int row = 0; row < 5; row++)
				for (int column = 0; column < width; column++)
					if (small_letters[index].rows[row][column] == '#')
						put(frame, x + column + (pass ? 0 : 1), y + row + (pass ? 0 : 1), pass ? letter : shadow);
		x += width + 1;
	}
}

int draw_game_letters_width(const char *text, int scale)
{
	return (int)strlen(text) * GAME_LETTER * scale;
}

void draw_game_letters(tmnt_frame *frame, int x, int y, int scale, uint32_t colour, const char *text)
{
	for (; *text; text++, x += GAME_LETTER * scale)
	{
		const int code = game_glyph(*text);

		if (code < 0)
			continue;
		if (colour == GAME_LETTERS_OWN && code < OWN_AT)
		{
			draw_tile(frame, code, GAME_LETTERS_BANK, x, y, scale, WIDTH, 256);
			continue;
		}
		/* one colour: the letter's pens in it, its shadow's darker (the own '@' in the game's colours: its pens
		   through the texts' palette) */
		for (int row = 0; row < 8; row++)
			for (int column = 0; column < 8; column++)
			{
				const int pen = letter_pen(code, column, row);

				if (!pen)
					continue;
				if (colour == GAME_LETTERS_OWN)
					put_block(frame, x + column * scale, y + row * scale, scale,
						palette_colour(GAME_LETTERS_BANK * 16 + pen));
				else
					put_block(frame, x + column * scale, y + row * scale, scale,
						pen == GAME_LETTERS_SHADOW_PEN ? darkened(colour, 96) : colour);
			}
	}
}

/* (centred in what the window shows: the picture without its first LEFT_STRIPE columns) */
void draw_game_letters_centred(tmnt_frame *frame, int y, int scale, uint32_t colour, const char *text)
{
	draw_game_letters(frame, LEFT_STRIPE + (WIDTH - LEFT_STRIPE - draw_game_letters_width(text, scale)) / 2, y, scale,
		colour, text);
}

/* ---- the frame's sprites again ---- */

static uint8_t sprites_next[TMNT_SPRITE_RAM_SIZE];     /* the sprite RAM at this frame's end: the next frame's */
static uint8_t sprites_shown[TMNT_SPRITE_RAM_SIZE];    /* the list the frame drawn shows */

void console_keep_sprites(void)
{
	memcpy(sprites_shown, sprites_next, sizeof(sprites_shown));
	if (!(tmnt_board.sprite_control & 0x10))    /* (the K051960's DMA, as the video makes it) */
		memcpy(sprites_next, tmnt_board.sprite_ram, sizeof(sprites_next));
}

/* A 16 x 16 cell's pen (k051960's spritelayout_reverse: four 8 x 8 quarters) */
static int sprite_pen(int code, int x, int y)
{
	const uint8_t *data = &game_sprites[(size_t)(code % SPRITES) * 128u + (size_t)((y & 8) * 8 + (x & 8) * 4 + (y & 7) * 4)];
	const int column = x & 7;

	return ((data[3] >> (7 - column)) & 1) << 3 | ((data[2] >> (7 - column)) & 1) << 2
		| ((data[1] >> (7 - column)) & 1) << 1 | ((data[0] >> (7 - column)) & 1);
}

/* As runtime/src/video.c draws the K051960's list (tmnt.cpp's callback, no zoom); a shadow's pen darkens */
void draw_board_sprites(tmnt_frame *frame)
{
	static const int x_offset[8] = { 0, 1, 4, 5, 16, 17, 20, 21 };
	static const int y_offset[8] = { 0, 2, 8, 10, 32, 34, 40, 42 };
	static const int widths[8] = { 1, 2, 1, 2, 4, 2, 4, 8 };
	static const int heights[8] = { 1, 1, 2, 2, 2, 4, 4, 8 };
	int sorted[128];

	for (int index = 0; index < 128; index++)
		sorted[index] = -1;
	for (int offset = 0; offset < TMNT_SPRITE_RAM_SIZE; offset += 8)
		if (sprites_shown[offset] & 0x80)
			sorted[sprites_shown[offset] & 0x7F] = offset;
	for (int priority = 0; priority < 128; priority++)
	{
		const uint8_t *sprite;
		int code, colour, size, w, h, ox, oy;
		bool shadow, flip_x, flip_y;

		if (sorted[priority] < 0)
			continue;
		sprite = &sprites_shown[sorted[priority]];
		code = sprite[2] + ((sprite[1] & 0x1F) << 8);
		colour = sprite[3];
		shadow = !(tmnt_board.shadow_config & 4) && ((tmnt_board.shadow_config & 2) || (colour & 0x80));
		code |= (colour & 0x10) << 9;
		colour = 16 + (colour & 0x0F);
		size = (sprite[1] & 0xE0) >> 5;
		w = widths[size];
		h = heights[size];
		if (w >= 2) code &= ~0x01;
		if (h >= 2) code &= ~0x02;
		if (w >= 4) code &= ~0x04;
		if (h >= 4) code &= ~0x08;
		if (w >= 8) code &= ~0x10;
		if (h >= 8) code &= ~0x20;
		ox = (256 * sprite[6] + sprite[7]) & 0x1FF;
		oy = 256 - ((256 * sprite[4] + sprite[5]) & 0x1FF);
		flip_x = (sprite[6] & 0x02) != 0;
		flip_y = (sprite[4] & 0x02) != 0;
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
				const int left = ((ox + 16 * x) & 0x1FF) - 96, top = oy + 16 * y - 16;

				for (int row = 0; row < 16; row++)
					for (int column = 0; column < 16; column++)
					{
						const int pen = sprite_pen(cell, flip_x ? 15 - column : column, flip_y ? 15 - row : row);
						const int px = left + column, py = top + row;

						if (!pen || px < 0 || px >= WIDTH || py < 0 || py >= HEIGHT)
							continue;
						if (pen == 15 && shadow)
							frame->pixels[py * WIDTH + px] = darkened(frame->pixels[py * WIDTH + px], 192);
						else
							frame->pixels[py * WIDTH + px] = palette_colour(colour * 16 + pen);
					}
			}
	}
}
