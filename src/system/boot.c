/**
 *    Name:        system/boot.c
 *    Description: the reset, the RAM and ROM tests, and the main loop that runs the jobs the frames queue
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the board's start: the RAM and ROM check, the work RAM, the DIP switches, then the main loop that runs
   the queued jobs */
void reset(void)
{
	uint32_t negative = 0;
	uint8_t bottom_left_tile = 0, bottom_right_tile = 0, control = 0, dip_number = 0, flip = 0, free_play_setting = 0, priority = 0, tile_attribute = 0, top_left_tile = 0, top_right_tile = 0;
	uint16_t banks = 0, blocks = 0, coinage_2_offset = 0, coinage_offset = 0, count = 0, count_2 = 0, count_3 = 0, count_4 = 0, delay = 0, job = 0, job_parameter = 0, printed_sum = 0, sum = 0, switches = 0;
	uint32_t bad_chips = 0, chip_bits = 0, empty_jobs = 0, expected = 0, fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, job_slot = 0, palettes = 0, place = 0, place_2 = 0, quarter_offset = 0, ram_end = 0, ram_start = 0, screen_x_high_2 = 0, setting = 0, setting_offset = 0, sum_place = 0, sums = 0, tile_place = 0, unused_flag = 0;
	uint16_t t1 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t21 = 0, t22 = 0, job_2 = 0;
	uint32_t t2 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0;

	write_status(0x2600u);
	take_interrupts();
	io_write8(IO_0A0011, 0u);
	io_write8(UNMAPPED_10E801, 0x13u);
	io_write8(IO_0A0001, 0u);
	io_write8(0x140000u, 0x10u);
	io_write8(TILES_106402, 0u);
	io_write8(TILES_106400, 0u);
	io_write8(TILES_106403, 0u);
	io_write8(TILES_106401, 0u);
	io_write8(TILES_106018, 0u);
	io_write8(TILES_106019, 0u);
	flip = 0u;
	if ((io_read8(IO_0A0019) & 1u) == 0u)
	{
		flip |= 1u;
		io_write8(0x140000u, 0x18u);
	}
	io_write8(TILES_106D00, flip);
	write_status(0x2500u);
	take_interrupts();
	bad_chips = 0u;
	chip_bits = palette_ram_chip_bits_ADDRESS;
	ram_start = 0x80000u;
	blocks = 0u;
	bad_chips = test_palette_ram(blocks, bad_chips, ram_start, chip_bits);
	chip_bits = tile_ram_chip_bits_ADDRESS;
	ram_start = 0x100000u;
	blocks = 0x17u;
	bad_chips = test_ram(blocks, bad_chips, ram_start, chip_bits);
	start_boot_screen();
	chip_bits = work_ram_chip_bits_ADDRESS;
	ram_start = 0x60000u;
	blocks = 0xFu;
	bad_chips = test_ram(blocks, bad_chips, ram_start, chip_bits);
	count_3 = 0x1000u;
	do
	{
		t1 = count_3 - 1u;
		count_3 = t1;
	} while (t1 != 0xFFFFu);
	chip_bits = sprite_ram_chip_bits_ADDRESS;
	ram_start = SPRITE_RAM;
	blocks = 0u;
	bad_chips = test_ram(blocks, bad_chips, ram_start, chip_bits);
	sum_program_chips();
	sums = sound_queue_write_ADDRESS;
	expected = good_chip_sums_ADDRESS;
	count_3 = 3u;
	do
	{
		sum = mem_read16(sums);
		t2 = expected;
		sums += 2u;
		expected += 2u;
		if (sum != CHIP_SUM(t2)->sum)
			bad_chips = (bad_chips & ~0xFFFFu) | (uint16_t)(bad_chips + 1u);
		bad_chips = (bad_chips & ~0xFFFFu) | (uint16_t)(bad_chips << 1);
		t3 = count_3 - 1u;
		count_3 = t3;
	} while (t3 != 0xFFFFu);
	job_queue_write = bad_chips;
	bad_chips = show_boot_check(bad_chips);
	if (bad_chips != 0u)
	{
		sum_place = sound_queue_write_ADDRESS;
		place = chip_sum_places_ADDRESS;
		count_3 = 3u;
		do
		{
			place_2 = TEXT_PLACE(place)->place;
			printed_sum = mem_read16(sum_place);
			place += 4u;
			sum_place += 2u;
			print_boot_hex(printed_sum, place_2);
			t4 = count_3 - 1u;
			count_3 = t4;
		} while (t4 != 0xFFFFu);
	}
	count_3 = 0x2Fu;
	if (bad_chips != 0u)
		count_3 = 0x2Fu;
	do
	{
		count = 0x7FFFu;
		do
		{
			t5 = count - 1u;
			count = t5;
		} while (t5 != 0xFFFFu);
		io_write8(IO_0A0011, (uint8_t)count_3);
		t6 = count_3 - 1u;
		count_3 = t6;
	} while (t6 != 0xFFFFu);
	if (bad_chips == 0u)
	{
		io_write8(0x140000u, 0x10u);
		io_write16(TILES_106B00, 0u);
		io_write16(TILES_106E00, 0u);
		clear_sprite_chip_registers();
		clear_sprite_ram();
		clear_objects();
		clear_tile_chip_registers();
		clear_tile_ram();
		palettes = boot_palettes_ADDRESS;
		banks = 1u;
		palette_second_half = 0u;
		palette_request_set(banks, palettes);
		tile_place = TILE_RAM;
		tile_attribute = 0u;
		bottom_left_tile = 0x2Cu;
		bottom_right_tile = 0x2Du;
		top_left_tile = 0x2Eu;
		top_right_tile = 0x2Fu;
		count_4 = 0xFu;
		do
		{
			count_2 = 0x1Fu;
			do
			{
				io_write8(tile_place + 0x80u, tile_attribute);
				io_write8(tile_place + 0x81u, bottom_left_tile);
				io_write8(tile_place + 0x82u, tile_attribute);
				io_write8(tile_place + 0x83u, bottom_right_tile);
				io_write8(tile_place, tile_attribute);
				tile_place++;
				io_write8(tile_place, top_left_tile);
				tile_place++;
				io_write8(tile_place, tile_attribute);
				tile_place++;
				io_write8(tile_place, top_right_tile);
				t7 = count_2 - 1u;
				count_2 = t7;
				tile_place++;
			} while (t7 != 0xFFFFu);
			t8 = count_4 - 1u;
			count_4 = t8;
			tile_place += 0x80u;
		} while (t8 != 0xFFFFu);
		io_write8(IO_0A0011, tile_attribute);
		count_3 = 0x80u;
		do
		{
			delay = 0x3000u;
			do
			{
				t9 = delay;
				delay = (uint16_t)(t9 - 1u);
			} while (t9 != 1u);
			io_write8(IO_0A0011, (uint8_t)count_3);
			t10 = count_3 - 1u;
			count_3 = t10;
		} while (t10 != 0xFFFFu);
		ram_end = 0x64000u;
		count_2 = 0x1FFu;
		zero_boot_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
		distance_low_byte = fill_5;
		fade_banks = fill_6;
		do
		{
			t11 = ram_end - 4u;
			mem_write16(t11 + 2u, fill_8);
			mem_write16(t11, fill_8 >> 16);
			t12 = t11 - 4u;
			mem_write16(t12 + 2u, fill_7);
			mem_write16(t12, fill_7 >> 16);
			t13 = t12 - 4u;
			t14 = 0u;
			mem_write16(t13 + 2u, t14);
			mem_write16(t13, t14 >> 16);
			t15 = t13 - 4u;
			t16 = 0u;
			mem_write16(t15 + 2u, t16);
			mem_write16(t15, t16 >> 16);
			t17 = t15 - 4u;
			mem_write16(t17 + 2u, fill_4);
			mem_write16(t17, fill_4 >> 16);
			t18 = t17 - 4u;
			mem_write16(t18 + 2u, fill_3);
			mem_write16(t18, fill_3 >> 16);
			t19 = t18 - 4u;
			mem_write16(t19 + 2u, fill_2);
			mem_write16(t19, fill_2 >> 16);
			t20 = t19 - 4u;
			mem_write16(t20 + 2u, fill);
			mem_write16(t20, fill >> 16);
			ram_end = t20;
			t21 = count_2 - 1u;
			count_2 = t21;
		} while (t21 != 0xFFFFu);
		io_write8(IO_0A0011, (uint8_t)count_3);
		sound_queue_read = sound_queue_ADDRESS;
		sound_queue_write = sound_queue_ADDRESS;
		palette_queue_write = PALETTE_REQUEST_ADDRESS(0);
		palette_queue_read = PALETTE_REQUEST_ADDRESS(0);
		palette_area_banks = 0u;
		palette_set_banks = 0u;
		palette_black_banks = 0u;
		palette_second_half = 0u;
		palette_set = 0u;
		job_slot = job_queue_ADDRESS;
		job_queue_read = job_queue_ADDRESS;
		job_queue_write = job_queue_ADDRESS;
		empty_jobs = 0xFFFFFFFFu;
		count = 0x1Fu;
		do
		{
			AT32(QUEUED_JOB(job_slot)->job, -2) = empty_jobs;
			t22 = count - 1u;
			count = t22;
			job_slot += 4u;
		} while (t22 != 0xFFFFu);
		reset_ranking();
		dip_switches_1 = ~io_read8(IO_0A0011);
		dip_switches_2 = ~io_read8(IO_0A0013);
		dip_switches_3 = ~io_read8(IO_0A0019);
		free_play_setting = 0u;
		coinage_offset = dip_switches_1;
		coinage_2_offset = (uint16_t)coinage_offset;
		coinage_offset = coinage_offset & 0xFu;
		free_play = free_play_setting;
		coinage_offset = (uint16_t)((uint32_t)coinage_offset << 1);
		COIN_SLOT(COIN_SLOT_ADDRESS(0))->coins_per_credit = coinage_coins[coinage_offset];
		COIN_SLOT(COIN_SLOT_ADDRESS(0))->credits_per_coins = coinage_credits[coinage_offset];
		coinage_2_offset = (coinage_2_offset & ~0xFu) >> 3;
		COIN_SLOT(COIN_SLOT_ADDRESS(1))->coins_per_credit = coinage_2_coins[coinage_2_offset];
		COIN_SLOT(COIN_SLOT_ADDRESS(1))->credits_per_coins = coinage_2_credits[coinage_2_offset];
		switches = dip_switches_2;
		switches = switches & 3u;
		lives_per_credit = lives_settings[switches];
		unused_flag = 0u;
		if ((dip_switches_2 & 4u) == 0u)
			unused_flag = (uint8_t)(unused_flag + 1u);
		unused_dip_flag = unused_flag;
		version = 0u;
		setting_offset = dip_switches_2;
		setting_offset = setting_offset & 0x18u;
		quarter_offset = setting_offset;
		screen_x_high_2 = quarter_offset;
		quarter_offset = quarter_offset >> 2;
		screen_x_high = quarter_offset;
		setting = unused_dip_settings_ADDRESS + (uint16_t)(setting_offset + quarter_offset);
		unused_dip_setting_flag = (uint8_t)DIP_SETTING(setting)->flag;
		setting += 2u;
		unused_dip_setting_value = DIP_SETTING(setting - 0x2u)->value;
		AT32(unused_dip_setting, -2) = DIP_SETTING(setting - 0x2u)->value_2;
		difficulty_setting = (dip_switches_2 & 0x60u) >> 5;
		demo_sounds = dip_switches_2 & 0x80u;
		dip_number = 4u;
		if ((dip_switches_3 & 8u) != 0u)
			dip_number = 6u;
		unused_dip_number = dip_number;
		board_control = 0u;
		io_write8(IO_0A0009, 0u);
		io_write8(IO_0A0001, 8u);
		priority = 0u;
		control = 0u;
		if ((dip_switches_3 & 1u) != 0u)
		{
			priority |= 7u;
			control |= 8u;
		}
		layer_priority = priority;
		io_write8(PRIORITY_0C0001, priority);
		sprite_control = control;
		io_write8(0x140000u, control);
		write_status(0u);
		take_interrupts();
		board_control |= 0x20u;
		io_write8(IO_0A0001, board_control);
		for (;;)
		{
			animate_fire_tiles(&fill_5, &fill_6);
			job_slot = job_queue_read;
			frame_wait();
			job_2 = QUEUED_JOB(job_slot)->job;
			negative = job_2 >> 15;
			job = job_2;
			screen_x_high = (screen_x_high & ~0xFFFFu) | job;
			take_interrupts();
			if (negative)
				continue;
			offset_high_word = 0u;
			job_parameter = (uint8_t)job;
			job = (job & 0x3F00u) >> 6;
			screen_x_high = (screen_x_high & ~0xFFFFu) | job;
			QUEUED_JOB(job_slot)->job = 0xFFFFu;
			job_slot += 2u;
			if (job_slot > job_queue_ADDRESS + 0x7Fu)
				job_slot = job_queue_ADDRESS;
			job_queue_read = job_slot;
			switch (job)
			{
			case 0:
				clear_layers_job((uint8_t)job_parameter);
				break;
			case 4:
				print_game_text(job_parameter);
				break;
			case 8:
				set_text_colour(job_parameter);
				break;
			case 12:
				set_spare_text_colour(job_parameter);
				break;
			case 16:
				fill_4 = add_point(fill_4, job_parameter);
				break;
			case 20:
				clear_scores((uint8_t)job_parameter);
				break;
			case 24:
				draw_score_labels((uint8_t)job_parameter);
				break;
			case 28:
				job_none_before_clear_layers();
				break;
			case 32:
				job_none_after_print_colour_text();
				break;
			case 36:
				fill_4 = draw_tile_picture(job_parameter);
				break;
			case 40:
				job_none_after_draw_tile_picture();
				break;
			case 44:
				draw_player_boxes(job_parameter, &fill_4, &fill_5, &fill_6);
				break;
			case 48:
				print_stage_title();
				break;
			case 52:
				draw_player_message((uint8_t)job_parameter, &fill_4, &fill_6);
				break;
			case 56:
				print_demo_text(job_parameter);
				break;
			case 60:
				print_tall_text((uint8_t)job_parameter, &fill_4, &fill_6);
				break;
			case 64:
				print_test_text(job_parameter);
				break;
			case 68:
				sum_mask_roms(fill_4, fill_5, fill_6, job_parameter);
				break;
			}
		}
	}
	for (;;)
	{
	}
}

/* the sprite chip's registers cleared */
void clear_sprite_chip_registers(void)
{
	io_write8(SPRITE_CONTROL_140001, 0u);
	io_write8(SPRITE_CONTROL_140003, 0u);
	io_write8(SPRITE_CONTROL_140002, 0u);
}

/* the tile chip's registers cleared */
void clear_tile_chip_registers(void)
{
	offset_high_word = 0u;
	io_write8(TILES_106900, 0u);
	io_write8(TILES_106B00, 0u);
	io_write8(TILES_106E00, 0u);
	io_write8(TILES_106C00, 0u);
}

/* the three tile layers blank and their scroll at 0 */
void clear_tile_ram(void)
{
	uint8_t blank_attribute = 0, space_tile = 0;
	uint16_t count = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	blank_attribute = 0u;
	space_tile = 0x10u;
	tile_place = TILE_RAM;
	count = 0x7FFu;
	do
	{
		io_write8(tile_place, blank_attribute);
		tile_place++;
		io_write8(tile_place, space_tile);
		t1 = count - 1u;
		count = t1;
		tile_place++;
	} while (t1 != 0xFFFFu);
	io_write8(IO_0A0011, blank_attribute);
	tile_place = TILE_RAM + 0x2000u;
	count = 0x1FFFu;
	do
	{
		io_write8(tile_place, blank_attribute);
		tile_place++;
		io_write8(tile_place, space_tile);
		t2 = count - 1u;
		count = t2;
		tile_place++;
	} while (t2 != 0xFFFFu);
	io_write8(TILES_106402, 0u);
	io_write8(TILES_106400, 0u);
	io_write8(TILES_106403, 0u);
	io_write8(TILES_106401, 0u);
	io_write8(TILES_106018, 0u);
	io_write8(TILES_106019, 0u);
	io_write8(IO_0A0011, 0u);
}

/* zeroes for the boot's clearing of the work RAM */
void zero_boot_fill_registers(uint32_t *p_fill, uint32_t *p_fill_2, uint32_t *p_fill_3, uint32_t *p_fill_4, uint32_t *p_fill_5, uint32_t *p_fill_6, uint32_t *p_fill_7, uint32_t *p_fill_8)
{
	*p_fill = 0u;
	*p_fill_2 = 0u;
	*p_fill_3 = 0u;
	*p_fill_4 = 0u;
	*p_fill_5 = 0u;
	*p_fill_6 = 0u;
	*p_fill_7 = 0u;
	*p_fill_8 = 0u;
}

/* the boot's screen: a palette, the fixed layer blank and "RAM ROM CHECK" */
void start_boot_screen(void)
{
	uint8_t black = 0, text_number = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t colour = 0, colour_place = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	colour_place = PALETTE_RAM;
	count = 0xFu;
	do
	{
		colour = boot_colours_ADDRESS;
		count_2 = 0xFu;
		do
		{
			io_write8(colour_place + 1u, PALETTE_COLOUR(colour)->high);
			colour++;
			io_write8(colour_place + 3u, PALETTE_COLOUR(colour - 0x1u)->low);
			colour_place += 4u;
			t1 = count_2 - 1u;
			count_2 = t1;
			colour++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	colour_place = PALETTE_RAM + 0x400u;
	count = 0x1FFu;
	black = 0u;
	do
	{
		io_write8(colour_place + 1u, black);
		io_write8(colour_place + 3u, black);
		colour_place += 4u;
		t3 = count - 1u;
		count = t3;
	} while (t3 != 0xFFFFu);
	io_write8(IO_0A0011, (uint8_t)count);
	tile_place = TILE_RAM;
	count = 0x3FFu;
	do
	{
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, 0x10u);
		tile_place++;
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, 0x10u);
		t4 = count - 1u;
		count = t4;
		tile_place++;
	} while (t4 != 0xFFFFu);
	text_number = 0u;
	print_boot_text(text_number);
}

/* a number as four hexadecimal digits, for a bad program chip's sum */
void print_boot_hex(uint16_t number, uint32_t place_2)
{
	uint16_t count = 0, digit_tile = 0;
	uint16_t t1 = 0;

	count = 3u;
	do
	{
		digit_tile = number & 0xFu;
		if (digit_tile >= 0xAu)
			digit_tile += 7u;
		place_2--;
		io_write8(place_2, (uint8_t)digit_tile);
		place_2--;
		io_write8(place_2, 0u);
		number = number >> 4;
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
}

/* a RAM written and read back with four patterns, its bad chips marked */
uint32_t test_ram(uint16_t blocks, uint32_t bad_chips, uint32_t ram_start, uint32_t chip_bits)
{
	uint8_t pattern = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t test_place = 0;
	uint8_t t1 = 0, t2 = 0;
	uint16_t t3 = 0, t4 = 0;

	bad_chips = bad_chips << 2;
	pattern = 0u;
	for (;;)
	{
		test_place = ram_start;
		count = (uint16_t)blocks;
		do
		{
			count_2 = 0x1FFu;
			do
			{
				mem_write8(test_place, pattern);
				t1 = mem_read8(test_place);
				(void)t1;
				if (t1 != pattern)
					bad_chips = bad_chips | mem_read8(chip_bits);
				mem_write8(test_place + 1u, pattern);
				t2 = mem_read8(test_place + 1u);
				(void)t2;
				if (t2 != pattern)
					bad_chips = bad_chips | mem_read8(chip_bits + 1u);
				t3 = count_2 - 1u;
				count_2 = t3;
				test_place += 2u;
			} while (t3 != 0xFFFFu);
			io_write8(IO_0A0011, (uint8_t)blocks);
			t4 = count - 1u;
			count = t4;
		} while (t4 != 0xFFFFu);
		if (pattern == 0xFFu)
			break;
		pattern += 0x55u;
	}
	return bad_chips;
}

/* the palette RAM written and read back with four patterns */
uint32_t test_palette_ram(uint16_t blocks, uint32_t bad_chips, uint32_t ram_start, uint32_t chip_bits)
{
	uint16_t count = 0, count_2 = 0;
	uint32_t pattern = 0, test_place = 0;
	uint16_t t1 = 0, t2 = 0;

	bad_chips = bad_chips << 2;
	pattern = 0u;
	for (;;)
	{
		test_place = ram_start;
		count = (uint16_t)blocks;
		do
		{
			count_2 = 0x3FFu;
			do
			{
				io_write8(test_place + 1u, pattern);
				if (io_read8(test_place + 1u) != (uint8_t)pattern)
					bad_chips = bad_chips | mem_read8(chip_bits);
				io_write8(test_place + 3u, pattern);
				if (io_read8(test_place + 3u) != (uint8_t)pattern)
					bad_chips = bad_chips | mem_read8(chip_bits + 1u);
				test_place += 4u;
				t1 = count_2 - 1u;
				count_2 = t1;
			} while (t1 != 0xFFFFu);
			io_write8(IO_0A0011, (uint8_t)blocks);
			t2 = count - 1u;
			count = t2;
		} while (t2 != 0xFFFFu);
		if (pattern == 0xFFu)
			break;
		pattern += 0x55u;
	}
	return bad_chips;
}

/* the boot's ROM check: each program chip's bytes summed, read twice to be sure; the board gives the
   sums in its place */
void BOARD_ORIGINAL(sum_program_chips)(void)
{
	uint16_t chip_byte = 0, count = 0, count_2 = 0, sum = 0;
	uint32_t byte_place = 0, chip = 0, chip_start = 0, sum_place = 0;
	uint16_t t1 = 0, t2 = 0;
	uint32_t start = 0;

	sum_place = sound_queue_write_ADDRESS;
	chip = program_chips_ADDRESS;
	for (;;)
	{
		start = PROGRAM_CHIP(chip)->start;
		chip_start = start;
		chip += 4u;
		if ((int32_t)start < 0)
			break;
		byte_place = chip_start;
		count = (uint16_t)(PROGRAM_CHIP(chip - 0x4u)->blocks - 1u);
		sum = 0u;
		chip_byte = 0u;
		chip += 2u;
		do
		{
			count_2 = 0xFFFu;
			for (;;)
			{
				chip_byte = mem_read8(byte_place);
				if (chip_byte != mem_read8(byte_place))
					continue;
				sum += chip_byte;
				byte_place += 2u;
				t1 = count_2 - 1u;
				count_2 = t1;
				if (t1 == 0xFFFFu)
					break;
			}
			io_write8(IO_0A0011, (uint8_t)count);
			t2 = count - 1u;
			count = t2;
		} while (t2 != 0xFFFFu);
		mem_write16(sum_place, sum);
		sum_place += 2u;
	}
}

/* the check's results: each chip OK or BAD */
uint32_t show_boot_check(uint32_t bad_chips)
{
	uint32_t carry = 0;
	uint8_t text_number = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t bad_bits = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = TILE_RAM;
	count = 0x3FFu;
	do
	{
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, 0x10u);
		tile_place++;
		io_write8(tile_place, 0u);
		tile_place++;
		io_write8(tile_place, 0x10u);
		t1 = count - 1u;
		count = t1;
		tile_place++;
	} while (t1 != 0xFFFFu);
	text_number = 1u;
	print_boot_text(text_number);
	count_2 = 0xBu;
	text_number = 2u;
	bad_chips = bad_chips >> 1;
	bad_bits = bad_chips;
	do
	{
		carry = bad_bits & 1u;
		bad_bits = bad_bits >> 1;
		if (carry)
			print_boot_text(text_number);
		text_number++;
		t2 = count_2 - 1u;
		count_2 = t2;
	} while (t2 != 0xFFFFu);
	return bad_chips;
}

/* a text of the boot's screen */
void print_boot_text(uint8_t text_number)
{
	uint8_t text = 0;
	uint32_t text_record = 0, tile_place = 0;

	text_record = boot_texts[text_number & 0x7Fu];
	for (;;)
	{
		tile_place = TILE_RAM + 0x18u + (int16_t)TEXT_16(text_record);
		text_record += 2u;
		for (;;)
		{
			text = TEXT(text_record);
			text_record++;
			if (text == 0x3Fu)
				return;
			if (text == 0x2Fu)
				break;
			io_write8(tile_place, 0u);
			tile_place++;
			io_write8(tile_place, (uint8_t)(text - 0x30u));
			tile_place++;
		}
	}
}
