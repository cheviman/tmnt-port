/**
 *    Name:        video.h
 *    Description: TMNT's video: the K052109's three tile layers, the K051960's sprites and the palette, drawn
 *                 from the board's memories as the board shows them
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The game's own video (no MAME code): the frames it draws have to be the ones MAME's tmnt.cpp,
 *    k052109.cpp and k051960.cpp draw, pixel for pixel (kref --video-check compares them, frame by frame).
 *    As on the board: the K052109 takes its scroll registers when VBLANK ends, a frame is drawn when VBLANK
 *    starts, from the tile RAM and palette of that moment and the sprites of the K051960's last DMA, and the
 *    DMA copies the sprite RAM right after (unless the 051937 has it off).
 */
#ifndef TMNT_VIDEO_H
#define TMNT_VIDEO_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TMNT_VIDEO_WIDTH   320
#define TMNT_VIDEO_HEIGHT  224

/* A frame of the screen, 0x00RRGGBB a pixel */
typedef struct
{
	uint32_t pixels[TMNT_VIDEO_WIDTH * TMNT_VIDEO_HEIGHT];
} tmnt_frame;

/* Back to the state after a reset: no scroll, no sprites */
void tmnt_video_reset(void);

/* VBLANK ends (line 16): the K052109 takes its scroll */
void tmnt_video_vblank_end(void);

/* VBLANK starts (line 240): the frame is drawn into 'frame' if it is not NULL, then the sprite DMA */
void tmnt_video_vblank_start(tmnt_frame *frame);

/* FNV-1a hash of a frame's pixels (red, green, blue of each), as kref's --video-hashes */
uint64_t tmnt_video_hash(const tmnt_frame *frame);

/* A wider picture, for a port's WIDE (the console's; the arcade's is always none): 'left' and 'right' columns more
   at each side of the board's 320 (their sum up to TMNT_VIDEO_MAX_MORE, the left up to TMNT_VIDEO_MAX_LEFT), the
   layers' maps and the sprites drawn there too, from the next frame drawn on. The frame above stays the board's
   320 columns, which tmnt_video_wide_frame gives with its sides, and its width (320 + left + right).
   With a wider picture the sprites' x takes a tenth bit, byte 6's bit 2 (a zoom bit, which tmnt never sets: the
   console's draw_sprites writes it), x from -256 to 767 then, so a sprite past one side never shows at the other */
#define TMNT_VIDEO_MAX_MORE   176
#define TMNT_VIDEO_MAX_LEFT   176
#define TMNT_VIDEO_MAX_WIDTH  (TMNT_VIDEO_WIDTH + TMNT_VIDEO_MAX_MORE)
void tmnt_video_set_wide(int left, int right);

/* WIDE's maps' ends: layer 'map' (1 A, 2 B) shows its map from screen column 'first' to 'last' only (the screen's
   0-319 the board's), black or nothing past them (B, A), where the map has no more (the area's ends: its ring of 512
   pixels holds other columns there); from the next frame drawn on, and only with a wider picture */
void tmnt_video_set_layer_window(int map, int first, int last);

/* WIDE's band of the fixed layer: its first 'rows' rows on the screen drawn 'dx' columns further right than the
   board's, its 320 columns alone (the console's players' boxes kept in the middle of a picture whose sides differ:
   a player's report, the boxes moved as WIDE's sides did); from the next frame drawn on, and only with a wider
   picture */
void tmnt_video_set_fixed_band(int dx, int rows);
const uint32_t *tmnt_video_wide_frame(int *width);    /* (its rows TMNT_VIDEO_MAX_WIDTH apart) */
int tmnt_video_wide_left(void);    /* (the columns of that frame left of the board's column 0) */

/* What the frames drew, for the reference's --graphics-uses (NULL in the game, which draws the same either way):
   when set, a tile code's byte gets bit 0, 1 or 2 when one of its pixels other than pen 0 is drawn on the fixed
   layer, layer A or layer B (32768 codes), and a sprite cell's byte 1 when one of its pixels is (16384 codes) */
extern uint8_t *tmnt_video_tiles_shown;
extern uint8_t *tmnt_video_sprites_shown;
/* and, when set too, the palette (0-63: the palette RAM's colour / 16) the last such pixel of the code showed with */
extern uint8_t *tmnt_video_tile_palettes;
extern uint8_t *tmnt_video_sprite_palettes;

#ifdef __cplusplus
}
#endif

#endif
