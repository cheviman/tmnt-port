/**
 *    Name:        system/animation_viewer.c
 *    Description: an unused viewer of the objects' animations with their palettes, worked with the players' buttons
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* Unused: the animation viewer's start, its object in the middle of the screen with the first animation */
void animation_viewer_start(void)
{
	OBJECT(OBJECT_ADDRESS(12))->flags = 0x81u;
	OBJECT(OBJECT_ADDRESS(12))->contacts = 0u;
	OBJECT(OBJECT_ADDRESS(12))->kind = 0u;
	OBJECT(OBJECT_ADDRESS(12))->parameter = 0u;
	OBJECT(OBJECT_ADDRESS(12))->colour = 0xFFu;
	OBJECT(OBJECT_ADDRESS(12))->animation_delay = 0u;
	OBJECT(OBJECT_ADDRESS(12))->count = 0u;
	OBJECT(OBJECT_ADDRESS(12))->routine = 0u;
	OBJECT(OBJECT_ADDRESS(12))->state = 0u;
	OBJECT(OBJECT_ADDRESS(12))->screen_x = 0x100u;
	OBJECT(OBJECT_ADDRESS(12))->screen_y = 0x60u;
	viewer_entry = 0u;
	viewer_animation = 0u;
	hide_all_sprites();
	animation_viewer_show();
}

/* Unused: the viewer's frame, the players' buttons paging through the entries and their animations */
void animation_viewer_frame(void)
{
	uint8_t presses = 0;
	uint16_t count = 0;
	uint32_t ignored = 0, number = 0, tile_place = 0;
	uint8_t t1 = 0;

	if (palettes_busy == 0u)
	{
		presses = player_presses(0) | player_presses(1) | player_presses(2) | player_presses(3);
		if ((presses & 0x20u) != 0u)
		{
			animation_viewer_show();
			tile_place = TILE_RAM + 0x354u;
			count = 1u;
			number = viewer_entry;
			draw_hex_digits(count, number, tile_place);
			tile_place = TILE_RAM + 0x454u;
			count = 1u;
			number = viewer_animation;
			draw_hex_digits(count, number, tile_place);
			build_draw_list();
			draw_sprites(ignored);
			return;
		}
		if ((presses & 0x10u) != 0u)
		{
			viewer_animation++;
			if ((uint8_t)animation_viewer_counts[viewer_entry] <= viewer_animation)
				viewer_animation = 0u;
			animation_viewer_show();
			tile_place = TILE_RAM + 0x354u;
			count = 1u;
			number = viewer_entry;
			draw_hex_digits(count, number, tile_place);
			tile_place = TILE_RAM + 0x454u;
			count = 1u;
			number = viewer_animation;
			draw_hex_digits(count, number, tile_place);
			build_draw_list();
			draw_sprites(ignored);
			return;
		}
		if ((presses & 4u) != 0u)
		{
			viewer_entry++;
			viewer_animation = 0u;
			if (animation_viewer_counts[viewer_entry] < 0)
				viewer_entry = 0u;
			animation_viewer_show();
			tile_place = TILE_RAM + 0x354u;
			count = 1u;
			number = viewer_entry;
			draw_hex_digits(count, number, tile_place);
			tile_place = TILE_RAM + 0x454u;
			count = 1u;
			number = viewer_animation;
			draw_hex_digits(count, number, tile_place);
			build_draw_list();
			draw_sprites(ignored);
			return;
		}
		if ((presses & 8u) != 0u)
		{
			viewer_animation = 0u;
			t1 = viewer_entry;
			viewer_entry = t1 - 1u;
			if (t1 == 0u)
				viewer_entry = 0u;
			animation_viewer_show();
		}
	}
	tile_place = TILE_RAM + 0x354u;
	count = 1u;
	number = viewer_entry;
	draw_hex_digits(count, number, tile_place);
	tile_place = TILE_RAM + 0x454u;
	count = 1u;
	number = viewer_animation;
	draw_hex_digits(count, number, tile_place);
	build_draw_list();
	draw_sprites(ignored);
}

/* Unused: the entry's animation on the viewer's object, with its palettes */
void animation_viewer_show(void)
{
	uint16_t entry_offset = 0, kind = 0;
	uint32_t animation_entry = 0, data = 0, object = 0;

	object = OBJECT_ADDRESS(12);
	entry_offset = (uint16_t)((uint32_t)viewer_entry << 3);
	animation_entry = ANIMATION_ENTRY(animation_viewer_animations[entry_offset / 4u] + viewer_animation * 0x4u);
	set_animation(0u, object, animation_entry);
	data = animation_viewer_palettes[entry_offset / 4u];
	kind = 0x208u;
	post_palette_request(kind, data);
}
