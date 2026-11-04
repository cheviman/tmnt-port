/**
 *    Name:        stage/fire.c
 *    Description: the fire's tiles redrawn in the first two areas
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* areas 0 and 1, in play or the apartment's exit: the fire's tiles redrawn, a spot a frame, a new frame
   every four */
void animate_fire_tiles(uint32_t *p_third_row_offset, uint32_t *p_second_row_offset)
{
	uint32_t third_row_offset = *p_third_row_offset;
	uint32_t second_row_offset = *p_second_row_offset;
	uint8_t frame = 0, tile_attribute = 0;
	uint16_t block_offset = 0, count = 0, spot_offset = 0;
	uint32_t frame_blocks = 0, spot_record = 0, tile_place = 0;
	uint16_t t1 = 0;

	if (game_state != 2u)
		return;
	if (scene != 5u)
	{
		if (scene != 4u)
			return;
		if (scene_step != 0u)
		{
			if (scene_step != 1u || scene_substep != 0u)
				return;
		}
		else if (scene_substep != 4u)
			return;
		if (stage != 0u && stage != 1u)
			return;
	}
	else if (stage != 0u && (stage != 1u || cutscene_step >= 4u))
		return;
	LAYER(LAYER_A)->x = LAYER(LAYER_B)->x;
	LAYER(LAYER_A)->y = LAYER(LAYER_B)->y;
	frame = (uint8_t)frame_count;
	if (frame == fire_last_frame)
		return;
	fire_last_frame = frame;
	building_fire_spot++;
	if (building_fire_spot >= 4u)
	{
		building_fire_spot = 0u;
		building_fire_frame++;
		if (building_fire_frame >= 8u)
			building_fire_frame = 0u;
	}
	if (stage != 0u)
	{
		apartment_fire_spot++;
		if (apartment_fire_spot >= 4u)
		{
			apartment_fire_spot = 0u;
			apartment_fire_frame++;
			if (apartment_fire_frame >= 8u)
				apartment_fire_frame = 0u;
		}
		spot_offset = (uint16_t)((uint32_t)apartment_fire_spot << 2);
		spot_record = fire_tiles[spot_offset / 4u];
		tile_place = FIRE_SPOT_LONG(spot_record);
		frame_blocks = FIRE_SPOT_LONG(spot_record + ((IN_PAGE(spot_offset, apartment_fire_frame) & 0x3FFFu) + 1u) * 0x4u);
		count = 7u;
		second_row_offset = 0x10u;
		third_row_offset = 0x20u;
	}
	else
	{
		spot_offset = (uint16_t)((uint32_t)building_fire_spot << 2);
		spot_record = fire_tiles_area_0[spot_offset / 4u];
		tile_place = FIRE_SPOT_LONG(spot_record);
		frame_blocks = FIRE_SPOT_LONG(spot_record + ((IN_PAGE(spot_offset, building_fire_frame) & 0x3FFFu) + 1u) * 0x4u);
		count = 3u;
		second_row_offset = 8u;
		third_row_offset = 0x10u;
	}
	do
	{
		block_offset = FIRE_BLOCK_OFFSET(frame_blocks);
		tile_attribute = TABLE8(fire_blocks, (uint32_t)(int16_t)block_offset);
		mem_write8(tile_place, tile_attribute);
		mem_write8(tile_place + 2u, tile_attribute);
		mem_write8(tile_place + 4u, tile_attribute);
		mem_write8(tile_place + 6u, tile_attribute);
		mem_write8(tile_place + 0x80u, tile_attribute);
		mem_write8(tile_place + 0x82u, tile_attribute);
		mem_write8(tile_place + 0x84u, tile_attribute);
		mem_write8(tile_place + 0x86u, tile_attribute);
		mem_write8(tile_place + 0x100u, tile_attribute);
		mem_write8(tile_place + 0x102u, tile_attribute);
		mem_write8(tile_place + 0x104u, tile_attribute);
		mem_write8(tile_place + 0x106u, tile_attribute);
		mem_write8(tile_place + 0x180u, tile_attribute);
		mem_write8(tile_place + 0x182u, tile_attribute);
		mem_write8(tile_place + 0x184u, tile_attribute);
		mem_write8(tile_place + 0x186u, tile_attribute);
		mem_write8(tile_place + 1u, TABLE8(fire_blocks, 0x1u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 3u, TABLE8(fire_blocks, 0x2u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 5u, TABLE8(fire_blocks, 0x3u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 7u, TABLE8(fire_blocks, 0x4u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x81u, TABLE8(fire_blocks, 0x5u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x83u, TABLE8(fire_blocks, 0x6u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x85u, TABLE8(fire_blocks, 0x7u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x87u, TABLE8(fire_blocks, 0x8u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x101u, TABLE8(fire_blocks, 0x9u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x103u, TABLE8(fire_blocks, 0xAu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x105u, TABLE8(fire_blocks, 0xBu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x107u, TABLE8(fire_blocks, 0xCu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x181u, TABLE8(fire_blocks, 0xDu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x183u, TABLE8(fire_blocks, 0xEu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x185u, TABLE8(fire_blocks, 0xFu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x187u, TABLE8(fire_blocks, 0x10u + (uint32_t)(int16_t)block_offset));
		block_offset = FIRE_BLOCK_OFFSET(frame_blocks + second_row_offset);
		tile_attribute = TABLE8(fire_blocks, (uint32_t)(int16_t)block_offset);
		mem_write8(tile_place + 0x200u, tile_attribute);
		mem_write8(tile_place + 0x202u, tile_attribute);
		mem_write8(tile_place + 0x204u, tile_attribute);
		mem_write8(tile_place + 0x206u, tile_attribute);
		mem_write8(tile_place + 0x280u, tile_attribute);
		mem_write8(tile_place + 0x282u, tile_attribute);
		mem_write8(tile_place + 0x284u, tile_attribute);
		mem_write8(tile_place + 0x286u, tile_attribute);
		mem_write8(tile_place + 0x300u, tile_attribute);
		mem_write8(tile_place + 0x302u, tile_attribute);
		mem_write8(tile_place + 0x304u, tile_attribute);
		mem_write8(tile_place + 0x306u, tile_attribute);
		mem_write8(tile_place + 0x380u, tile_attribute);
		mem_write8(tile_place + 0x382u, tile_attribute);
		mem_write8(tile_place + 0x384u, tile_attribute);
		mem_write8(tile_place + 0x386u, tile_attribute);
		mem_write8(tile_place + 0x201u, TABLE8(fire_blocks, 0x1u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x203u, TABLE8(fire_blocks, 0x2u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x205u, TABLE8(fire_blocks, 0x3u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x207u, TABLE8(fire_blocks, 0x4u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x281u, TABLE8(fire_blocks, 0x5u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x283u, TABLE8(fire_blocks, 0x6u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x285u, TABLE8(fire_blocks, 0x7u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x287u, TABLE8(fire_blocks, 0x8u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x301u, TABLE8(fire_blocks, 0x9u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x303u, TABLE8(fire_blocks, 0xAu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x305u, TABLE8(fire_blocks, 0xBu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x307u, TABLE8(fire_blocks, 0xCu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x381u, TABLE8(fire_blocks, 0xDu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x383u, TABLE8(fire_blocks, 0xEu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x385u, TABLE8(fire_blocks, 0xFu + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x387u, TABLE8(fire_blocks, 0x10u + (uint32_t)(int16_t)block_offset));
		block_offset = FIRE_BLOCK_OFFSET(frame_blocks + third_row_offset);
		tile_attribute = TABLE8(fire_blocks, (uint32_t)(int16_t)block_offset);
		mem_write8(tile_place + 0x400u, tile_attribute);
		mem_write8(tile_place + 0x402u, tile_attribute);
		mem_write8(tile_place + 0x404u, tile_attribute);
		mem_write8(tile_place + 0x406u, tile_attribute);
		mem_write8(tile_place + 0x480u, tile_attribute);
		mem_write8(tile_place + 0x482u, tile_attribute);
		mem_write8(tile_place + 0x484u, tile_attribute);
		mem_write8(tile_place + 0x486u, tile_attribute);
		mem_write8(tile_place + 0x401u, TABLE8(fire_blocks, 0x1u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x403u, TABLE8(fire_blocks, 0x2u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x405u, TABLE8(fire_blocks, 0x3u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x407u, TABLE8(fire_blocks, 0x4u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x481u, TABLE8(fire_blocks, 0x5u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x483u, TABLE8(fire_blocks, 0x6u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x485u, TABLE8(fire_blocks, 0x7u + (uint32_t)(int16_t)block_offset));
		mem_write8(tile_place + 0x487u, TABLE8(fire_blocks, 0x8u + (uint32_t)(int16_t)block_offset));
		t1 = count - 1u;
		count = t1;
		tile_place += 8u;
		frame_blocks += 2u;
	} while (t1 != 0xFFFFu);
	fade_banks = 0u;
	*p_third_row_offset = third_row_offset;
	*p_second_row_offset = second_row_offset;
}
