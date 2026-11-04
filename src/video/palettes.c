/**
 *    Name:        video/palettes.c
 *    Description: the palettes: requests queued and done a frame later, the palette effects
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* a palette request, its kind, banks and data, queued to be done a frame later */
uint32_t post_palette_request(uint16_t kind, uint32_t data)
{
	uint32_t palette_request = 0;

	palette_request = palette_queue_write;
	PALETTE_REQUEST(palette_request)->kind = kind;
	palette_request += 2u;
	PALETTE_REQUEST(palette_request - 0x2u)->data = data;
	palette_request += 4u;
	if (palette_request > 0x60BEFu)
		palette_request = PALETTE_REQUEST_ADDRESS(0);
	palette_queue_write = palette_request;
	return palette_request;
}

/* Unused: every palette request of a list queued, up to its end mark */
void unreached_009264(uint32_t requests)
{
	uint16_t kind = 0;
	uint32_t data = 0;

	for (;;)
	{
		kind = mem_read16(requests);
		requests += 2u;
		if (kind == 0xFFFFu)
			break;
		data = mem_read32(requests);
		requests += 4u;
		post_palette_request(kind, data);
	}
}

/* a bank of the palette requests a frame: the one under way, or the next one queued */
void run_palette_requests(uint16_t d7)
{
	uint16_t banks = 0, request_kind = 0, second_half_offset = 0;
	uint32_t data = 0, palette_request = 0, palettes = 0;
	uint16_t kind = 0;
	uint32_t t1 = 0;

	if (palette_area_banks != 0u)
	{
		banks = (uint16_t)((d7 & ~0xFFu) | palette_area_banks);
		palette_request_area(banks);
		return;
	}
	if (palette_set_banks != 0u)
	{
		banks = (uint16_t)((d7 & ~0xFFu) | palette_set_banks);
		palettes = palette_set;
		palette_request_set_next_bank(banks, palettes);
		return;
	}
	if (palette_black_banks != 0u)
	{
		banks = (uint16_t)((d7 & ~0xFFu) | palette_black_banks);
		palette_request_black(banks);
		return;
	}
	if (palette_second_half != 0u)
	{
		offset_high_word = 0u;
		second_half_offset = (uint16_t)((uint32_t)palette_second_half << 2);
		switch (second_half_offset)
		{
		case 0:
		case 16:
		case 20:
		case 28:
		case 32:
			palette_request_none();
			break;
		case 4:
			palette_request_area_second_half(second_half_offset);
			break;
		case 8:
			palette_request_set_second_half(second_half_offset);
			break;
		case 12:
			palette_request_list_second_half(second_half_offset);
			break;
		case 24:
			palette_request_black_second_half(second_half_offset);
			break;
		}
		palette_second_half = 0u;
		return;
	}
	palette_request = palette_queue_read;
	kind = PALETTE_REQUEST(palette_request)->kind;
	request_kind = kind;
	if (kind == 0u)
	{
		palettes_busy = 0u;
		t1 = (((uint32_t)palette_area_banks << 24u) | ((uint32_t)palette_set_banks << 16u) | ((uint32_t)palette_black_banks << 8u) | palette_second_half);
		offset_high_word = t1;
		if (t1 != 0u)
			palettes_busy = 1u;
		return;
	}
	offset_high_word = 0u;
	banks = (uint8_t)request_kind;
	data = PALETTE_REQUEST(palette_request)->data;
	request_kind = (request_kind & 0x3F00u) >> 6;
	PALETTE_REQUEST(palette_request)->kind = 0u;
	palette_request += 2u;
	PALETTE_REQUEST(palette_request - 0x2u)->data = 0u;
	palette_request += 4u;
	if (palette_request > 0x60BEFu)
		palette_request = PALETTE_REQUEST_ADDRESS(0);
	palette_queue_read = palette_request;
	switch (request_kind)
	{
	case 0:
	case 36:
		palette_request_none();
		break;
	case 4:
		palette_request_area(banks);
		break;
	case 8:
		palette_request_set(banks, data);
		break;
	case 12:
		palette_request_list(banks, data);
		break;
	case 16:
		palette_request_palette(banks, data);
		break;
	case 20:
		palette_request_colours((uint8_t)banks, data);
		break;
	case 24:
		palette_request_black(banks);
		break;
	case 28:
	case 32:
		palette_request_spare();
		break;
	}
	palettes_busy = 0u;
	t1 = (((uint32_t)palette_area_banks << 24u) | ((uint32_t)palette_set_banks << 16u) | ((uint32_t)palette_black_banks << 8u) | palette_second_half);
	offset_high_word = t1;
	if (t1 != 0u)
		palettes_busy = 1u;
}

/* a palette request that does nothing */
void palette_request_none(void)
{
}

/* the area's palettes into the banks asked for, a bank a frame */
void palette_request_area(uint16_t banks)
{
	uint8_t done_bank = 0;
	uint16_t area_offset = 0, bank = 0, palette_count = 0;
	uint32_t a5 = 0, palettes = 0;

	bank = 0u;
	while ((((banks & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(bank & 0x1Fu))) == 0u)
	{
		bank = (uint8_t)(bank + 1u);
		if (bank >= 4u)
		{
			palette_area_banks = 0u;
			fade_banks = bank;
			return;
		}
	}
	done_bank = (uint8_t)bank;
	palette_area_banks = ((banks & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & ~shl32(1u, (int)(bank & 0x1Fu));
	offset_high_word = 0u;
	palette_count = palette_bank_counts[bank];
	area_offset = (uint16_t)((uint32_t)stage << 2);
	bank = (uint16_t)((uint32_t)bank << 2);
	palettes = mem_read32(area_palettes[area_offset / 4u] + bank);
	a5 = palette_bank_addresses[bank / 4u];
	if (palette_second_half != 0u)
	{
		a5 += 0x200u;
		palettes += 0x20u;
	}
	if (done_bank == 3u)
		palette_second_half = 1u;
	palette_place = a5;
	copy_palette_list(palette_count, palettes);
	distance_low_byte = area_offset;
}

/* the second half of the sprites' palettes of the area's set */
void palette_request_area_second_half(uint16_t second_half_offset)
{
	uint16_t banks = 0;

	banks = (uint16_t)((second_half_offset & ~0xFFu) | 8u);
	palette_request_area(banks);
}

/* a set of palettes into the banks asked for, a bank a frame */
void palette_request_set(uint16_t banks, uint32_t bank_lists)
{
	palette_set = bank_lists;
	palette_request_set_next_bank(banks, bank_lists);
}

/* the set's next bank */
void palette_request_set_next_bank(uint16_t banks, uint32_t bank_lists)
{
	uint8_t done_bank = 0;
	uint16_t bank = 0, palette_count = 0;
	uint32_t a5 = 0, palettes = 0;

	bank = 0u;
	while ((((banks & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(bank & 0x1Fu))) == 0u)
	{
		bank = (uint8_t)(bank + 1u);
		if (bank >= 4u)
		{
			palette_set_banks = 0u;
			palette_set = 0u;
			fade_banks = bank;
			return;
		}
	}
	done_bank = (uint8_t)bank;
	palette_set_banks = ((banks & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & ~shl32(1u, (int)(bank & 0x1Fu));
	offset_high_word = 0u;
	palette_count = palette_bank_counts[bank];
	bank = (uint16_t)((uint32_t)bank << 2);
	palettes = PALETTE_LIST_POINTER(bank_lists + (bank / 4u) * 0x4u);
	a5 = palette_bank_addresses[bank / 4u];
	if (palette_second_half != 0u)
	{
		a5 += 0x200u;
		palettes += 0x20u;
	}
	if (done_bank == 3u)
		palette_second_half = 2u;
	palette_place = a5;
	copy_palette_list(palette_count, palettes);
}

/* the second half of the sprites' palettes of the set */
void palette_request_set_second_half(uint16_t second_half_offset)
{
	uint16_t banks = 0;
	uint32_t bank_lists = 0;

	bank_lists = palette_set;
	banks = (uint16_t)((second_half_offset & ~0xFFu) | 8u);
	palette_request_set(banks, bank_lists);
}

/* a list of palettes into one bank */
void palette_request_list(uint16_t banks, uint32_t palettes)
{
	uint8_t done_bank = 0;
	uint16_t bank = 0, palette_count = 0;
	uint32_t a5 = 0;

	bank = 0u;
	while (((banks | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(bank & 0x1Fu))) == 0u)
	{
		bank = (uint8_t)(bank + 1u);
		if (bank >= 4u)
		{
			fade_banks = bank;
			return;
		}
	}
	done_bank = (uint8_t)bank;
	offset_high_word = 0u;
	palette_count = palette_bank_counts[bank];
	a5 = palette_bank_addresses[bank & 0x3FFFu];
	if (palette_second_half != 0u)
	{
		a5 += 0x200u;
		palettes += 0x20u;
	}
	if (done_bank == 3u)
	{
		palette_set = palettes;
		palette_second_half = 3u;
	}
	palette_place = a5;
	copy_palette_list(palette_count, palettes);
}

/* the second half of the sprites' palettes of the list */
void palette_request_list_second_half(uint16_t second_half_offset)
{
	uint16_t banks = 0;
	uint32_t palettes = 0;

	palettes = palette_set;
	banks = (uint16_t)((second_half_offset & ~0xFFu) | 8u);
	palette_request_list(banks, palettes);
}

/* sixteen colours into one palette */
void palette_request_palette(uint32_t palette, uint32_t data)
{
	uint16_t bank_offset = 0, count = 0;
	uint32_t colour_place = 0;
	uint16_t t1 = 0;

	bank_offset = (uint16_t)palette;
	palette &= 0xFu;
	bank_offset = bank_offset & ~0xFu;
	colour_place = TABLE32(palette_bank_addresses, bank_offset >> 2) + (uint16_t)(palette << 6);
	count = 0xFu;
	do
	{
		mem_write8(colour_place + 1u, COLOUR_BYTE(data));
		data++;
		mem_write8(colour_place + 3u, COLOUR_BYTE(data));
		t1 = count - 1u;
		count = t1;
		data++;
		colour_place += 4u;
	} while (t1 != 0xFFFFu);
	fade_banks = count;
	palette_place = colour_place;
}

/* single colours into a bank's palettes */
void palette_request_colours(uint8_t bank, uint32_t data)
{
	uint16_t bank_offset = 0;
	uint32_t a5 = 0, colour_number = 0, colour_place = 0, d6 = 0;
	uint8_t colour_byte = 0;

	bank_offset = (uint16_t)bank;
	bank_offset = bank_offset & ~0xFu;
	a5 = TABLE32(palette_bank_addresses, bank_offset >> 2);
	for (;;)
	{
		colour_byte = COLOUR_BYTE(data);
		d6 = colour_byte;
		data++;
		if ((int8_t)colour_byte < 0)
			break;
		colour_number = COLOUR_BYTE(data);
		colour_place = a5 + (uint16_t)(d6 << 6) + (uint16_t)(colour_number << 2);
		data++;
		mem_write8(colour_place + 1u, COLOUR_BYTE(data));
		data++;
		mem_write8(colour_place + 3u, COLOUR_BYTE(data));
		data++;
	}
	fade_banks = d6;
	palette_place = a5;
}

/* the banks asked for turned black, a bank a frame */
void palette_request_black(uint16_t banks)
{
	uint8_t done_bank = 0;
	uint16_t bank = 0, palette_count = 0;
	uint32_t a5 = 0;

	bank = 0u;
	while ((((banks & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(bank & 0x1Fu))) == 0u)
	{
		bank = (uint8_t)(bank + 1u);
		if (bank >= 4u)
		{
			palette_black_banks = 0u;
			fade_banks = bank;
			return;
		}
	}
	done_bank = (uint8_t)bank;
	palette_black_banks = ((banks & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & ~shl32(1u, (int)(bank & 0x1Fu));
	offset_high_word = 0u;
	palette_count = palette_bank_counts[bank];
	a5 = palette_bank_addresses[bank & 0x3FFFu];
	if (palette_second_half != 0u)
		a5 += 0x200u;
	if (done_bank == 3u)
		palette_second_half = 6u;
	palette_place = a5;
	blacken_palettes(palette_count);
}

/* the second half of the sprites' palettes turned black */
void palette_request_black_second_half(uint16_t second_half_offset)
{
	uint16_t banks = 0;

	banks = (uint16_t)((second_half_offset & ~0xFFu) | 8u);
	palette_request_black(banks);
}

/* a spare palette request kind that does nothing */
void palette_request_spare(void)
{
}

/* 16 colours from each palette of a list into the palette RAM */
void copy_palette_list(uint16_t count, uint32_t palettes)
{
	uint16_t count_2 = 0;
	uint32_t colours = 0;
	uint16_t t1 = 0, t2 = 0;

	do
	{
		count_2 = 0xFu;
		colours = PALETTE_POINTER(palettes);
		palettes += 4u;
		do
		{
			mem_write8(palette_place + 1u, COLOUR_BYTE(colours));
			colours++;
			mem_write8(palette_place + 3u, COLOUR_BYTE(colours));
			palette_place += 4u;
			t1 = count_2 - 1u;
			count_2 = t1;
			colours++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	fade_banks = count_2;
}

/* palettes turned black */
void blacken_palettes(uint16_t count)
{
	uint16_t count_2 = 0;
	uint16_t t1 = 0, t2 = 0;

	do
	{
		count_2 = 0xFu;
		do
		{
			mem_write8(palette_place + 1u, 0u);
			mem_write8(palette_place + 3u, 0u);
			t1 = count_2 - 1u;
			count_2 = t1;
			palette_place += 4u;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	fade_banks = count_2;
}

/* the palettes faded toward a list of palette sets, over the next frames */
void copy_palette_sets(void)
{
	switch ((uint32_t)(int8_t)fade_pass_step)
	{
	case 0:
		fade_palettes_start();
		return;
	case 1:
		fade_palettes_read();
		return;
	case 3:
		fade_palettes_write();
		return;
	case 2:
		fade_palettes_step();
		return;
	}
}

/* a fade toward a set of palettes starts */
void fade_palettes_start(void)
{
	fading = 1u;
	fade_pass_step++;
	fade_part = 0u;
	fade_lists_left = 0u;
	fade_part_mask = (uint8_t)(fade_banks >> 8u);
	fade_rate = (uint8_t)fade_banks;
	fade_target = palette_place;
	fade_buffer_place = 0x63800u;
	fade_palettes_read();
}

/* a part of the palettes read into the fade's buffer */
void fade_palettes_read(void)
{
	uint32_t buffer_place = 0, colour_place = 0, count = 0;
	uint16_t t1 = 0;

	for (;;)
	{
		count = fade_part;
		if ((fade_part_mask & shl32(1u, (int)(count & 7u))) != 0u)
		{
			fade_palettes_part(&count, &buffer_place, &colour_place);
			do
			{
				mem_write8(buffer_place, io_read8(colour_place + 1u));
				buffer_place++;
				mem_write8(buffer_place, io_read8(colour_place + 3u));
				t1 = count - 1u;
				count = t1;
				buffer_place++;
				colour_place += 4u;
			} while (t1 != 0xFFFFu);
			fade_buffer_place = buffer_place;
			fade_part++;
			if (fade_part < 5u)
				break;
			fade_pass_step++;
			fade_part = 0u;
			fade_lists_left = 0u;
			fade_buffer_place = 0x63800u;
			break;
		}
		fade_part++;
		if (fade_part < 5u)
			continue;
		fade_pass_step++;
		fade_part = 0u;
		fade_lists_left = 0u;
		fade_buffer_place = 0x63800u;
		return;
	}
}

/* the fade's colours written back into a part of the palettes */
void fade_palettes_write(void)
{
	uint32_t buffer_place = 0, colour_place = 0, count = 0;
	uint16_t t1 = 0;

	for (;;)
	{
		count = fade_part;
		if ((fade_part_mask & shl32(1u, (int)(count & 7u))) != 0u)
		{
			fade_palettes_part(&count, &buffer_place, &colour_place);
			do
			{
				io_write8(colour_place + 1u, mem_read8(buffer_place));
				buffer_place++;
				io_write8(colour_place + 3u, mem_read8(buffer_place));
				t1 = count - 1u;
				count = t1;
				buffer_place++;
				colour_place += 4u;
			} while (t1 != 0xFFFFu);
			fade_buffer_place = buffer_place;
			fade_part++;
			if (fade_part < 5u)
				break;
			fade_state = 0u;
			break;
		}
		fade_part++;
		if (fade_part < 5u)
			continue;
		fade_state = 0u;
		return;
	}
}

/* where a part of the palettes is and how many colours it has */
void fade_palettes_part(uint32_t *p_part, uint32_t *p_buffer_place, uint32_t *p_colour_place)
{
	uint16_t part = (uint16_t)*p_part;
	uint16_t colour_count = 0;

	*p_colour_place = palette_banks[part & 0x3FFFu];
	colour_count = 0x7Fu;
	*p_buffer_place = fade_buffer_place;
	*p_part = (*p_part & 0xFFFF0000u) | colour_count;
}

/* a part of the fade's colours a step toward the target palettes */
void fade_palettes_step(void)
{
	uint16_t colour = 0, count = 0, part = 0, rate = 0;
	uint32_t buffer_place = 0, channels = 0, new_colour = 0, target = 0, target_colour = 0, target_lists = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;

	buffer_place = fade_buffer_place;
	rate = fade_rate & 0xFu;
	target = fade_target;
	while ((fade_part_mask & shl32(1u, (int)(fade_part & 7u))) == 0u)
	{
		fade_part++;
		if (fade_part >= 5u)
		{
			fade_buffer_place = 0x63800u;
			fade_pass_step++;
			fade_part = 0u;
			fade_lists_left = 0u;
			return;
		}
	}
	part = fade_part;
	if (part == 4u)
		part = 3u;
	target_lists = PALETTE_POINTER(target + (part & 0x3FFFu) * 0x4u);
	fade_lists_left = 8u;
	do
	{
		target_colour = COLOUR_BYTE_32(target_lists);
		count = 0xFu;
		target_lists += 4u;
		do
		{
			colour = mem_read16(buffer_place);
			channels = table_loaded_by_lea_009872_ADDRESS;
			new_colour = 0u;
			fade_colour_part(rate, colour, target_colour, &new_colour, &channels);
			fade_colour_part(rate, colour, target_colour, &new_colour, &channels);
			fade_colour_part(rate, colour, target_colour, &new_colour, &channels);
			mem_write16(buffer_place, new_colour);
			t1 = count - 1u;
			count = t1;
			buffer_place += 2u;
			target_colour += 2u;
		} while (t1 != 0xFFFFu);
		t2 = fade_lists_left;
		fade_lists_left = t2 - 1u;
	} while (t2 != 1u);
	fade_part++;
	if (fade_part >= 5u)
	{
		fade_buffer_place = 0x63800u;
		fade_pass_step++;
		fade_part = 0u;
		fade_lists_left = 0u;
		return;
	}
	fade_buffer_place = buffer_place;
}

/* a colour's red, green or blue a step toward the target's */
void fade_colour_part(uint16_t rate, uint16_t colour, uint32_t target_colour, uint32_t *p_new_colour, uint32_t *p_channels)
{
	uint16_t new_colour = (uint16_t)*p_new_colour;
	uint32_t channels = *p_channels;
	uint16_t channel_mask = 0, channel_value = 0, step = 0, target_value = 0;
	uint16_t t1 = 0;

	channel_mask = mem_read16(channels);
	channel_value = colour & channel_mask;
	channels += 2u;
	step = (uint16_t)((uint32_t)rate * mem_read16(channels));
	target_value = mem_read16(target_colour) & channel_mask;
	channels += 2u;
	if (channel_value != target_value)
	{
		if (target_value <= channel_value)
		{
			t1 = channel_value;
			channel_value = (uint16_t)(t1 - step);
			if (t1 < (uint16_t)step)
				channel_value = 0u;
			if (channel_value < target_value)
				channel_value = (uint16_t)target_value;
		}
		else
		{
			channel_value += step;
			if (target_value <= channel_value)
				channel_value = (uint16_t)target_value;
		}
	}
	new_colour |= channel_value;
	*p_new_colour = (*p_new_colour & 0xFFFF0000u) | new_colour;
	*p_channels = channels;
}

/* the palette effects of a list started in free slots */
void start_palette_effects(uint32_t palette_effect_pointer)
{
	uint16_t bank_offset = 0, colour_code = 0, count = 0;
	uint32_t colour = 0, effect_steps = 0, palette_effect = 0;
	uint16_t t1 = 0;

	count = (uint16_t)(mem_read16(palette_effect_pointer) - 1u);
	palette_effect = PALETTE_EFFECT_ADDRESS(0);
	palette_effect_pointer += 2u;
	for (;;)
	{
		if ((PALETTE_EFFECT(palette_effect)->flags & 0x80u) == 0u)
		{
			effect_steps = mem_read32(palette_effect_pointer);
			colour_code = mem_read16(effect_steps);
			bank_offset = (colour_code & 0xF00u) >> 6;
			colour = TABLE32(palette_effect_banks, bank_offset) + (uint16_t)((colour_code & 0xF0u) << 2) + (uint16_t)((colour_code & 0xFu) << 2);
			PALETTE_EFFECT(palette_effect)->time = 0u;
			PALETTE_EFFECT(palette_effect)->flags = (bank_offset >> 2) | 0x80u;
			PALETTE_EFFECT(palette_effect)->step = effect_steps + 2u;
			PALETTE_EFFECT(palette_effect)->colour = colour;
			t1 = count - 1u;
			count = t1;
			palette_effect_pointer += 4u;
			if (t1 != 0xFFFFu)
				continue;
			break;
		}
		palette_effect += 0xAu;
		if (palette_effect >= 0x60700u)
			break;
	}
}

/* every frame: each palette effect's next colour when its time runs out; it loops, turns back or ends */
void step_palette_effects(void)
{
	uint32_t carry = 0;
	uint16_t count = 0, time = 0;
	uint32_t colour = 0, palette_effect = 0, step = 0;
	uint8_t time_2 = 0;
	uint16_t time_3 = 0, t1 = 0;

	palette_effect = PALETTE_EFFECT_ADDRESS(0);
	count = 0x3Fu;
	do
	{
		if ((PALETTE_EFFECT(palette_effect)->flags & 0x80u) != 0u)
		{
			time_2 = PALETTE_EFFECT(palette_effect)->time;
			carry = time_2 == 0u;
			PALETTE_EFFECT(palette_effect)->time = time_2 - 1u;
			if (time_2 == 1u || carry)
			{
				for (;;)
				{
					step = PALETTE_EFFECT(palette_effect)->step;
					time_3 = PALETTE_EFFECT_STEP(step)->time;
					time = time_3;
					if ((int16_t)time_3 < 0)
					{
						PALETTE_EFFECT(palette_effect)->flags = 0u;
						PALETTE_EFFECT(palette_effect)->time = 0u;
						PALETTE_EFFECT(palette_effect)->step = 0u;
						PALETTE_EFFECT(palette_effect)->colour = 0u;
						break;
					}
					if ((time & 0x4000u) != 0u)
					{
						PALETTE_EFFECT(palette_effect)->step = AT32(PALETTE_EFFECT_STEP(step)->colour, -2);
						continue;
					}
					if ((time & 0x2000u) != 0u)
						PALETTE_EFFECT(palette_effect)->flags |= 0x40u;
					else if ((time & 0x1000u) != 0u)
						PALETTE_EFFECT(palette_effect)->flags &= 0xBFu;
					PALETTE_EFFECT(palette_effect)->time = (uint8_t)time;
					colour = PALETTE_EFFECT(palette_effect)->colour;
					io_write8(colour + 1u, (uint8_t)(PALETTE_EFFECT_STEP(step)->colour >> 8u));
					io_write8(colour + 3u, (uint8_t)PALETTE_EFFECT_STEP(step)->colour);
					if ((PALETTE_EFFECT(palette_effect)->flags & 0x40u) != 0u)
						step -= 4u;
					else
						step += 4u;
					PALETTE_EFFECT(palette_effect)->step = step;
					break;
				}
			}
		}
		t1 = count - 1u;
		count = t1;
		palette_effect += 0xAu;
	} while (t1 != 0xFFFFu);
}

/* Unused: every running palette effect stopped, each by its layer's own stop */
void unreached_0099B4(uint8_t layers)
{
	uint16_t count = 0, effect_flags = 0;
	uint32_t palette_effect = 0;
	uint8_t flags = 0;
	uint16_t t1 = 0;

	palette_effect = PALETTE_EFFECT_ADDRESS(0);
	count = 0x3Fu;
	do
	{
		flags = PALETTE_EFFECT(palette_effect)->flags;
		effect_flags = flags;
		if ((int8_t)flags < 0)
		{
			switch (effect_flags & 3u)
			{
			case 0:
				stop_fixed_layer_effect(layers, palette_effect);
				break;
			case 1:
				stop_layer_a_effect(layers, palette_effect);
				break;
			case 2:
				stop_layer_b_effect(layers, palette_effect);
				break;
			case 3:
				stop_sprite_effect(layers, palette_effect);
				break;
			}
		}
		t1 = count - 1u;
		count = t1;
		palette_effect += 0xAu;
	} while (t1 != 0xFFFFu);
}

/* the effect stopped if the fixed layer's palettes are asked for */
void stop_fixed_layer_effect(uint8_t layers, uint32_t palette_effect)
{
	if ((layers & 1u) == 0u)
		return;
	clear_palette_effect(palette_effect);
}

/* the effect stopped if layer A's palettes are asked for */
void stop_layer_a_effect(uint8_t layers, uint32_t palette_effect)
{
	if ((layers & 2u) == 0u)
		return;
	clear_palette_effect(palette_effect);
}

/* the effect stopped if layer B's palettes are asked for */
void stop_layer_b_effect(uint8_t layers, uint32_t palette_effect)
{
	if ((layers & 4u) == 0u)
		return;
	clear_palette_effect(palette_effect);
}

/* the effect stopped if the sprites' palettes are asked for */
void stop_sprite_effect(uint8_t layers, uint32_t palette_effect)
{
	if ((layers & 8u) == 0u)
		return;
	clear_palette_effect(palette_effect);
}

/* a palette effect stopped */
void clear_palette_effect(uint32_t palette_effect)
{
	PALETTE_EFFECT(palette_effect)->flags = 0u;
	PALETTE_EFFECT(palette_effect)->time = 0u;
	PALETTE_EFFECT(palette_effect)->step = 0u;
	PALETTE_EFFECT(palette_effect)->colour = 0u;
}

/* every palette effect stopped */
void clear_palette_effects(void)
{
	uint16_t count = 0;
	uint32_t palette_effect = 0;
	uint16_t t1 = 0;

	palette_effect = PALETTE_EFFECT_ADDRESS(0);
	count = 0x40u;
	do
	{
		clear_palette_effect(palette_effect);
		t1 = count - 1u;
		count = t1;
		palette_effect += 0xAu;
	} while (t1 != 0xFFFFu);
}

/* the palettes of the attract mode's screen asked for */
void request_game_step_palettes(void)
{
	uint16_t kind = 0;
	uint32_t data = 0;

	data = game_step_palettes[game_step];
	kind = 0x20Fu;
	post_palette_request(kind, data);
}

/* Unused: the game step's palette effects started */
void unreached_009AA0(void)
{
	start_game_step_palette_effects();
}

/* the palette effects of the attract mode's screen started */
void start_game_step_palette_effects(void)
{
	uint32_t effect_lists = 0, palette_effect_pointer = 0;

	effect_lists = game_step_palette_effects[game_step];
	for (;;)
	{
		palette_effect_pointer = PALETTE_EFFECT_POINTER(effect_lists);
		effect_lists += 4u;
		if (palette_effect_pointer == 0u)
			break;
		start_palette_effects(palette_effect_pointer);
	}
}

/* the area's palette effects started */
void start_area_palette_effects(void)
{
	uint32_t effect_lists = 0;

	effect_lists = area_palette_effects[stage];
	start_palette_effect_lists(effect_lists);
}

/* every effect of a list of effect lists started */
void start_palette_effect_lists(uint32_t effect_lists)
{
	uint32_t palette_effect_pointer = 0;

	for (;;)
	{
		palette_effect_pointer = PALETTE_EFFECT_POINTER(effect_lists);
		effect_lists += 4u;
		if (palette_effect_pointer == 0u)
			break;
		start_palette_effects(palette_effect_pointer);
	}
}
