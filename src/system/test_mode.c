/**
 *    Name:        system/test_mode.c
 *    Description: the exceptions' handler and the test mode's pages: the RAM and ROM check, the mask ROM check, the inputs, the sounds, the DIP switches
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* an exception: the program stops until the watchdog resets the board */
void exception_hang(void)
{
	for (;;)
	{
	}
}

/* the test mode's mask ROM check */
void mask_rom_check_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		mask_rom_check_clear();
		return;
	case 1:
		draw_mask_rom_check();
		return;
	case 2:
		start_mask_rom_sums();
		return;
	case 3:
		show_mask_rom_check();
		return;
	case 4:
		mask_rom_check_wait();
		return;
	}
}

/* the layers cleared for the mask ROM check */
void mask_rom_check_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* the mask ROM check's heading and palette */
void draw_mask_rom_check(void)
{
	uint16_t kind = 0, text_number = 0;
	uint32_t data = 0;

	if (jobs_pending == 0u)
	{
		text_number = 8u;
		print_colour_text(text_number);
		data = 0xA07Eu;
		kind = 0x20Fu;
		post_palette_request(kind, data);
		scene_step++;
	}
}

/* the mask ROMs' sums started */
void start_mask_rom_sums(void)
{
	uint16_t count = 0, job = 0;
	uint32_t cleared = 0, sums = 0;
	uint16_t t1 = 0;

	if (palettes_busy != 0u)
		return;
	cleared = 0u;
	count = 5u;
	sums = ranking_scores_ADDRESS;
	do
	{
		mem_write32(sums, cleared);
		t1 = count - 1u;
		count = t1;
		sums += 4u;
	} while (t1 != 0xFFFFu);
	jobs_pending++;
	job = 0x1100u;
	queue_job(job);
	scene_step++;
}

/* each mask ROM's sum shown OK or BAD */
void show_mask_rom_check(void)
{
	uint16_t count = 0, count_2 = 0, mask_rom_sum = 0, rom_text = 0, text_number = 0;
	uint32_t bad_roms = 0, sum = 0, sums = 0;
	uint16_t t2 = 0, t3 = 0;
	uint32_t t1 = 0;

	if (jobs_pending != 0u)
		return;
	scene_step++;
	scene_timer = 0x80u;
	sum = good_mask_rom_sums_ADDRESS;
	sums = ranking_scores_ADDRESS;
	bad_roms = 0u;
	count = 5u;
	do
	{
		bad_roms = bad_roms << 1;
		mask_rom_sum = MASK_ROM_SUM(sum);
		t1 = sums;
		sums += 2u;
		sum += 2u;
		if (mask_rom_sum != mem_read16(t1))
			bad_roms |= 1u;
		t2 = count - 1u;
		count = t2;
	} while (t2 != 0xFFFFu);
	if (bad_roms == 0u)
	{
		text_number = 9u;
		print_colour_text(text_number);
		return;
	}
	scene_timer = 0u;
	text_number = 0xAu;
	print_colour_text(text_number);
	count_2 = 5u;
	rom_text = 0xBu;
	do
	{
		if ((bad_roms & shl32(1u, (int)(count_2 & 0x1Fu))) != 0u)
		{
			text_number = (uint16_t)rom_text;
			print_colour_text(text_number);
		}
		rom_text++;
		t3 = count_2 - 1u;
		count_2 = t3;
	} while (t3 != 0xFFFFu);
}

/* the results shown a while, then the next page */
void mask_rom_check_wait(void)
{
	uint8_t t1 = 0;

	if (scene_timer != 0u)
	{
		t1 = scene_timer;
		scene_timer = t1 - 1u;
		if (t1 == 1u)
		{
			scene++;
			scene_step = 0u;
		}
	}
	test_page_controls();
}

/* the graphics ROMs summed through the video chips */
void sum_mask_roms(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint32_t job_parameter)
{
	uint32_t sums = 0;

	sums = ranking_scores_ADDRESS;
	job_parameter = sum_sprite_roms_low(first_byte, second_byte, third_byte, job_parameter, sums);
	job_parameter = sum_sprite_roms_high(first_byte, second_byte, third_byte, job_parameter, sums);
	sum_tile_roms(first_byte, second_byte, third_byte, (uint16_t)job_parameter, sums);
	jobs_pending--;
}

/* the sprite ROMs' first half summed */
uint32_t sum_sprite_roms_low(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint32_t last_bank, uint32_t a0)
{
	uint32_t sum = 0;

	mask_rom_part_sums(0) = 0u;
	mask_rom_part_sums(1) = 0u;
	mask_rom_part_sums(2) = 0u;
	mask_rom_part_sums(3) = 0u;
	mask_rom_first_bank = 0u;
	mask_rom_last_bank = 3u;
	sum_sprite_rom_banks(first_byte, second_byte, third_byte, (uint16_t)last_bank);
	sum = mask_rom_part_sums(0);
	sum = (uint16_t)(sum + ranking_scores(9));
	ranking_scores(0) = sum;
	sum = ranking_scores(10);
	sum = (uint16_t)(sum + ranking_scores(11));
	ranking_scores(1) = sum;
	return sum;
}

/* the sprite ROMs' second half summed */
uint32_t sum_sprite_roms_high(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint32_t job_parameter, uint32_t a0)
{
	uint32_t sum = 0;

	mask_rom_part_sums(0) = 0u;
	mask_rom_part_sums(1) = 0u;
	mask_rom_part_sums(2) = 0u;
	mask_rom_part_sums(3) = 0u;
	mask_rom_first_bank = 0x40u;
	mask_rom_last_bank = 0x43u;
	sum_sprite_rom_banks(first_byte, second_byte, third_byte, (uint16_t)job_parameter);
	sum = (job_parameter & ~0xFFFFu) | mask_rom_part_sums(0);
	sum = (sum & ~0xFFFFu) | (uint16_t)(sum + ranking_scores(9));
	ranking_scores(2) = sum;
	sum = (sum & ~0xFFFFu) | ranking_scores(10);
	sum = (sum & ~0xFFFFu) | (uint16_t)(sum + ranking_scores(11));
	ranking_scores(3) = sum;
	return sum;
}

/* the sprite ROMs' banks read through the sprite chip and summed */
void sum_sprite_rom_banks(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint16_t last_bank)
{
	uint32_t carry = 0;
	uint8_t page = 0;
	uint16_t bank = 0, offset = 0;
	uint32_t read_base = 0, read_place = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	sprite_control |= 0x20u;
	io_write8(0x140000u, sprite_control);
	mask_rom_bank = mask_rom_first_bank;
	mask_rom_part_sums(0) = 0u;
	mask_rom_part_sums(1) = 0u;
	mask_rom_part_sums(2) = 0u;
	mask_rom_part_sums(3) = 0u;
	do
	{
		page = 0u;
		bank = (uint16_t)((last_bank & ~0xFFu) | mask_rom_bank);
		io_write8(SPRITE_CONTROL_140003, (uint8_t)bank);
		do
		{
			read_base = SPRITE_RAM;
			offset = 0u;
			io_write8(SPRITE_CONTROL_140002, page);
			do
			{
				read_place = read_base + (int16_t)offset;
				(void)mem_read8(read_place);
				(void)mem_read8(read_place);
				t1 = io_read8(SPRITE_CONTROL_140004);
				t2 = io_read8(SPRITE_CONTROL_140004);
				first_byte = (first_byte & ~0xFFu) | t2;
				t3 = io_read8(SPRITE_CONTROL_140005);
				t4 = io_read8(SPRITE_CONTROL_140005);
				second_byte = (second_byte & ~0xFFu) | t4;
				t5 = io_read8(SPRITE_CONTROL_140006);
				t6 = io_read8(SPRITE_CONTROL_140006);
				third_byte = (third_byte & ~0xFFu) | t6;
				t7 = io_read8(SPRITE_CONTROL_140007);
				t8 = io_read8(SPRITE_CONTROL_140007);
				bank = (uint16_t)((bank & ~0xFFu) | t8);
				add_to_rom_sums(first_byte, second_byte, third_byte, bank);
				offset += 4u;
			} while (offset < 0x400u);
			carry = (uint8_t)(page + 1u) < page;
			page++;
		} while (carry == 0u);
		mask_rom_bank++;
		last_bank = (uint16_t)((bank & ~0xFFu) | mask_rom_last_bank);
	} while (mask_rom_bank <= (uint8_t)last_bank);
	sprite_control &= 0xDFu;
	io_write8(0x140000u, sprite_control);
}

/* the tile ROMs read through the tile chip and summed */
void sum_tile_roms(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint16_t job_parameter, uint32_t a0)
{
	uint8_t page = 0;
	uint16_t offset = 0;
	uint32_t read_base = 0, read_place = 0;
	uint16_t t1 = 0;

	mask_rom_bank = 0u;
	mask_rom_part_sums(0) = 0u;
	mask_rom_part_sums(1) = 0u;
	mask_rom_part_sums(2) = 0u;
	mask_rom_part_sums(3) = 0u;
	do
	{
		page = 0u;
		job_parameter = (uint16_t)((job_parameter & ~0xFFu) | mask_rom_bank);
		tile_banks_0_1 = (uint8_t)job_parameter;
		io_write8(TILES_106B00, tile_banks_0_1);
		tile_banks_2_3 = (uint8_t)job_parameter;
		io_write8(TILES_106E00, tile_banks_2_3);
		do
		{
			read_base = 0x100000u;
			offset = 0u;
			io_write8(TILES_106C00, page);
			io_write8(TILES_106C00, page);
			io_write8(TILES_106C00, page);
			board_control |= 0x80u;
			io_write8(IO_0A0001, board_control);
			for (;;)
			{
				read_place = read_base + (int16_t)offset;
				do
				{
					first_byte = (first_byte & ~0xFFu) | mem_read8(read_place);
				} while ((uint8_t)first_byte != mem_read8(read_place));
				do
				{
					second_byte = (second_byte & ~0xFFu) | mem_read8(read_place + 2u);
				} while ((uint8_t)second_byte != mem_read8(read_place + 2u));
				do
				{
					third_byte = (third_byte & ~0xFFu) | mem_read8(read_place + 4u);
				} while ((uint8_t)third_byte != mem_read8(read_place + 4u));
				do
				{
					job_parameter = (uint16_t)((job_parameter & ~0xFFu) | mem_read8(read_place + 6u));
				} while ((uint8_t)job_parameter != mem_read8(read_place + 6u));
				add_to_rom_sums(first_byte, second_byte, third_byte, job_parameter);
				offset += 8u;
				if (offset >= 0x7000u)
					break;
				t1 = offset & 0x1000u;
				job_parameter = t1;
				if (t1 == 0u)
					continue;
				offset += 0x1000u;
			}
			board_control &= 0x7Fu;
			io_write8(IO_0A0001, board_control);
			page++;
		} while (page < 0x20u);
		mask_rom_bank += 0x44u;
	} while (mask_rom_bank != 0x10u);
	ranking_scores(4) = mask_rom_part_sums(0) + ranking_scores(9);
	ranking_scores(5) = ranking_scores(10) + ranking_scores(11);
	io_write8(TILES_106B00, 0u);
	tile_banks_0_1 = 0u;
	io_write8(TILES_106E00, 0u);
	tile_banks_2_3 = 0u;
}

/* four bytes of a graphics ROM added to its sums */
void add_to_rom_sums(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint16_t fourth_byte)
{
	uint8_t next_bit = 0;
	uint16_t bit = 0;
	uint32_t part_sum = 0;

	bit = 0u;
	part_sum = mask_rom_part_sums_ADDRESS;
	do
	{
		mask_rom_bits = 0u;
		next_bit = (uint8_t)(uint16_t)(bit + 1u);
		if ((first_byte & shl32(1u, (int)(bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 1u;
		if ((second_byte & shl32(1u, (int)(bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 2u;
		if ((((third_byte & ~0xFF00u) | (fade_banks & 0xFF00u)) & shl32(1u, (int)(bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 4u;
		if ((((fourth_byte & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 8u;
		if ((first_byte & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x10u;
		if ((second_byte & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x20u;
		if ((((third_byte & ~0xFF00u) | (fade_banks & 0xFF00u)) & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x40u;
		if ((((fourth_byte & 0xFFFFu) | (offset_high_word & ~0xFFFFu)) & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x80u;
		mem_write16(part_sum, mem_read16(part_sum) + mask_rom_bits);
		bit += 2u;
		part_sum += 2u;
	} while (bit < 8u);
}

/* Unused: the bits of four mask ROM reads put together and added to the parts' sums */
void unreached_001D9E(uint32_t first_byte, uint32_t second_byte, uint32_t third_byte, uint32_t fourth_byte)
{
	uint8_t next_bit = 0;
	uint16_t count = 0;
	uint32_t part_sum = 0;
	uint16_t t1 = 0;

	count = 3u;
	part_sum = mask_rom_part_sums_ADDRESS;
	do
	{
		mask_rom_bits = 0u;
		count = (uint16_t)((uint32_t)count << 1);
		next_bit = (uint8_t)(uint16_t)(count + 1u);
		if ((fourth_byte & shl32(1u, (int)(count & 0x1Fu))) != 0u)
			mask_rom_bits |= 1u;
		if ((third_byte & shl32(1u, (int)(count & 0x1Fu))) != 0u)
			mask_rom_bits |= 2u;
		if ((second_byte & shl32(1u, (int)(count & 0x1Fu))) != 0u)
			mask_rom_bits |= 4u;
		if ((first_byte & shl32(1u, (int)(count & 0x1Fu))) != 0u)
			mask_rom_bits |= 8u;
		if ((fourth_byte & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x10u;
		if ((third_byte & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x20u;
		if ((second_byte & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x40u;
		if ((first_byte & shl32(1u, (int)(next_bit & 0x1Fu))) != 0u)
			mask_rom_bits |= 0x80u;
		mem_write16(part_sum, mem_read16(part_sum) + mask_rom_bits);
		t1 = (count >> 1) - 1u;
		count = t1;
		part_sum += 2u;
	} while (t1 != 0xFFFFu);
}

/* the main loop's job 7, which no code queues: does nothing */
void job_none_before_clear_layers(void)
{
}

/* the tile layers chosen cleared */
void clear_layers_job(uint8_t layers)
{
	uint32_t blank_attribute = 0, space_tile = 0, tile_place = 0;

	blank_tile(&blank_attribute, &space_tile);
	if ((layers & 1u) != 0u)
	{
		tile_place = 0x100000u;
		clear_fixed_layer((uint8_t)blank_attribute, (uint8_t)space_tile, tile_place);
	}
	if ((layers & 2u) != 0u)
	{
		tile_place = TILE_RAM + 0x2000u;
		clear_scroll_layer((uint8_t)blank_attribute, (uint8_t)space_tile, tile_place);
	}
	if ((layers & 4u) != 0u)
	{
		tile_place = TILE_RAM + 0x4000u;
		clear_scroll_layer((uint8_t)blank_attribute, (uint8_t)space_tile, tile_place);
	}
	jobs_pending--;
}

/* the blank tile and its colour */
void blank_tile(uint32_t *p_blank_attribute, uint32_t *p_space_tile)
{
	*p_blank_attribute = 0u;
	*p_space_tile = 0x10u;
}

/* the fixed layer blank */
void clear_fixed_layer(uint8_t blank_attribute, uint8_t space_tile, uint32_t tile_place)
{
	uint16_t count = 0;
	uint16_t t1 = 0;

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
}

/* a scrolling layer blank */
void clear_scroll_layer(uint8_t blank_attribute, uint8_t space_tile, uint32_t tile_place)
{
	uint16_t count = 0;
	uint16_t t1 = 0;

	count = 0xFFFu;
	do
	{
		io_write8(tile_place, blank_attribute);
		tile_place++;
		io_write8(tile_place, space_tile);
		t1 = count - 1u;
		count = t1;
		tile_place++;
	} while (t1 != 0xFFFFu);
}

/* the test mode's start */
void test_mode_start(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		test_start_clear();
		return;
	case 1:
		test_start_palette();
		return;
	case 2:
		test_start_next();
		return;
	}
}

/* the layers cleared */
void test_start_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* the test mode's palette */
void test_start_palette(void)
{
	uint16_t kind = 0;
	uint32_t data = 0;

	if (jobs_pending == 0u)
	{
		data = 0xA07Eu;
		kind = 0x20Fu;
		post_palette_request(kind, data);
		scene_step++;
	}
}

/* on to the mask ROM check */
void test_start_next(void)
{
	if (palettes_busy == 0u)
	{
		scene++;
		scene_step = 0u;
	}
}

/* the test mode's cross hatch */
void cross_hatch_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		draw_cross_hatch();
		return;
	case 1:
		test_page_controls();
		return;
	}
}

/* the cross hatch drawn */
void draw_cross_hatch(void)
{
	uint8_t bottom_left_tile = 0, bottom_right_tile = 0, tile_attribute = 0, top_left_tile = 0, top_right_tile = 0;
	uint16_t count = 0, count_2 = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0, t2 = 0;

	tile_place = TILE_RAM;
	tile_attribute = 0u;
	bottom_left_tile = 0x2Cu;
	bottom_right_tile = 0x2Du;
	top_left_tile = 0x2Eu;
	top_right_tile = 0x2Fu;
	count = 0xFu;
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
			t1 = count_2 - 1u;
			count_2 = t1;
			tile_place++;
		} while (t1 != 0xFFFFu);
		t2 = count - 1u;
		count = t2;
		tile_place += 0x80u;
	} while (t2 != 0xFFFFu);
	scene_step++;
}

/* the test mode's colour test */
void color_test_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		color_test_clear();
		return;
	case 1:
		draw_color_test();
		return;
	case 2:
		test_page_controls();
		return;
	}
}

/* the layers cleared */
void color_test_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* the colour bars drawn */
void draw_color_test(void)
{
	uint8_t bar_colour = 0, tile_code = 0;
	uint16_t count = 0, count_2 = 0, count_3 = 0;
	uint32_t colour_number = 0, jobs = 0, row_place = 0, tile_place = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;

	if (jobs_pending != 0u)
		return;
	jobs = table_loaded_by_lea_005846_ADDRESS;
	queue_jobs(jobs);
	colour_number = 1u;
	colour_number = color_test_colour(colour_number);
	count = 7u;
	row_place = color_test_places[0];
	tile_place = row_place;
	do
	{
		count_2 = 7u;
		tile_code = 0x31u;
		do
		{
			count_3 = 3u;
			do
			{
				io_write8(tile_place, colour_number);
				tile_place++;
				io_write8(tile_place, tile_code);
				t1 = count_3 - 1u;
				count_3 = t1;
				tile_place++;
			} while (t1 != 0xFFFFu);
			tile_code++;
			t2 = count_2 - 1u;
			count_2 = t2;
		} while (t2 != 0xFFFFu);
		row_place += 0x80u;
		tile_place = row_place;
		t3 = count - 1u;
		count = t3;
	} while (t3 != 0xFFFFu);
	row_place = color_test_places[1];
	tile_place = row_place;
	count = 3u;
	do
	{
		count_2 = 0xFu;
		tile_code = 0x30u;
		do
		{
			bar_colour = TABLE8(text_colours, 0x2u + (uint32_t)(int16_t)count);
			mem_write8(tile_place, bar_colour);
			mem_write8(tile_place + 2u, bar_colour);
			mem_write8(tile_place + 0x80u, bar_colour);
			mem_write8(tile_place + 0x82u, bar_colour);
			mem_write8(tile_place + 1u, tile_code);
			mem_write8(tile_place + 3u, tile_code);
			mem_write8(tile_place + 0x81u, tile_code);
			mem_write8(tile_place + 0x83u, tile_code);
			tile_code++;
			t4 = count_2 - 1u;
			count_2 = t4;
			tile_place += 4u;
		} while (t4 != 0xFFFFu);
		row_place += 0x180u;
		tile_place = row_place;
		t5 = count - 1u;
		count = t5;
	} while (t5 != 0xFFFFu);
	scene_step++;
}

/* a colour of the colour test */
uint32_t color_test_colour(uint32_t colour_number)
{
	uint32_t colour = 0;

	colour = text_colours[colour_number];
	return colour;
}

/* the test mode's input test */
void io_test_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		io_test_clear();
		return;
	case 1:
		draw_io_test();
		return;
	case 2:
		show_inputs();
		return;
	}
}

/* the layers cleared */
void io_test_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* the input test's texts */
void draw_io_test(void)
{
	uint32_t jobs = 0;

	if (jobs_pending != 0u)
		return;
	jobs = table_loaded_by_lea_005896_ADDRESS;
	queue_jobs(jobs);
	scene_step++;
}

/* every input shown */
void show_inputs(void)
{
	show_player_inputs();
	show_coin_inputs();
	show_dip_switches();
	test_page_controls();
}

/* the four players' inputs shown */
void show_player_inputs(void)
{
	uint16_t count = 0, player_number = 0, player_offset = 0;
	uint32_t bit_place = 0, inputs = 0;
	uint16_t t1 = 0;

	player_number = 0u;
	do
	{
		player_offset = (uint16_t)((uint32_t)player_number << 2);
		inputs = (uint8_t)~io_read8(player_input_ports[player_offset / 4u]);
		bit_place = player_input_places[player_offset / 4u];
		count = 7u;
		do
		{
			if ((uint8_t)count != 1u)
			{
				if ((uint8_t)count != 0u)
					inputs = show_input_bit(inputs, bit_place);
			}
			else
				inputs = (uint8_t)inputs >> 1;
			t1 = count - 1u;
			count = t1;
			bit_place += 0x100u;
		} while (t1 != 0xFFFFu);
		player_number = (uint8_t)(player_number + 1u);
	} while (player_number < 4u);
}

/* the coin and service switches shown */
void show_coin_inputs(void)
{
	uint16_t count = 0;
	uint32_t bit_place = 0, inputs = 0;
	uint16_t t1 = 0, t2 = 0;

	inputs = (uint8_t)~io_read8(coin_input_port);
	bit_place = coin_input_places[0];
	count = 3u;
	do
	{
		inputs = show_input_bit(inputs, bit_place);
		t1 = count - 1u;
		count = t1;
		bit_place += 0x100u;
	} while (t1 != 0xFFFFu);
	inputs = (uint8_t)~io_read8(coin_input_port) >> 4;
	bit_place = coin_input_places[1];
	count = 3u;
	do
	{
		inputs = show_input_bit(inputs, bit_place);
		t2 = count - 1u;
		count = t2;
		bit_place += 0x100u;
	} while (t2 != 0xFFFFu);
}

/* the DIP switches shown */
void show_dip_switches(void)
{
	uint16_t bank = 0, bank_offset = 0, count = 0;
	uint32_t bit_place = 0, switches = 0;
	uint16_t t1 = 0;

	bank = 0u;
	do
	{
		bank_offset = (uint16_t)((uint32_t)bank << 2);
		switches = (uint8_t)~io_read8(dip_switch_ports[bank_offset / 4u]);
		bit_place = dip_switch_places[bank_offset / 4u];
		count = 7u;
		if (bank == 2u)
			count = 3u;
		do
		{
			switches = show_input_bit(switches, bit_place);
			t1 = count - 1u;
			count = t1;
			bit_place += 4u;
		} while (t1 != 0xFFFFu);
		bank = (uint8_t)(bank + 1u);
	} while (bank < 3u);
}

/* an input shown as 0 or 1 */
uint32_t show_input_bit(uint32_t inputs, uint32_t bit_place)
{
	uint32_t carry = 0;
	uint8_t bit_tile = 0;

	bit_tile = 0u;
	carry = inputs & 1u;
	inputs = (inputs & ~0xFFu) | ((uint8_t)inputs >> 1);
	if (carry)
		bit_tile = 1u;
	io_write8(bit_place, 0u);
	io_write8(bit_place + 1u, bit_tile);
	return inputs;
}

/* the coin counter test left out */
void skip_coin_counter_test(void)
{
	scene++;
	scene_step = 0u;
}

/* Unused: the test mode's coin counter test */
void coin_counter_test_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		coin_counter_test_clear();
		break;
	case 1:
		draw_coin_counter_test();
		break;
	case 2:
		pulse_coin_counters();
		break;
	}
	test_page_controls();
}

/* unused: the layers cleared */
void coin_counter_test_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* unused: the coin counter test's texts */
void draw_coin_counter_test(void)
{
	uint32_t jobs = 0;

	if (jobs_pending == 0u)
	{
		jobs = table_loaded_by_lea_005A58_ADDRESS;
		queue_jobs(jobs);
		scene_step++;
		turtles_in_black = 0u;
		initials_entered(0) = 0xFFu;
		initials_entered(1) = 0u;
		initials_entered(2) = 0u;
	}
}

/* unused: each coin counter pulsed in turn */
void pulse_coin_counters(void)
{
	uint8_t control = 0, digit = 0;
	uint32_t tile_place = 0;
	uint8_t t1 = 0;

	t1 = turtles_in_black;
	turtles_in_black = t1 - 1u;
	if (t1 != 0u)
	{
		if (turtles_in_black == 0x1Eu)
			board_control &= ~3u;
		return;
	}
	initials_entered(0)++;
	if (initials_entered(0) < 4u)
	{
		tile_place = coin_counter_places[0];
		digit = (uint8_t)(initials_entered(1) + 1u);
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, digit);
		tile_place = coin_counter_places[1];
		digit = (uint8_t)(initials_entered(0) + 1u);
		mem_write8(tile_place, 0u);
		mem_write8(tile_place + 1u, digit);
		control = (uint8_t)((board_control & ~3u) | shl32(1u, (int)(initials_entered(1) & 0x1Fu)));
		io_write8(IO_0A0001, control);
		board_control = control;
		turtles_in_black = 0x20u;
		return;
	}
	initials_entered(0) = 0u;
	initials_entered(1)++;
	if (initials_entered(1) < 2u)
	{
		tile_place = coin_counter_places[0];
		digit = (uint8_t)(initials_entered(1) + 1u);
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, digit);
		tile_place = coin_counter_places[1];
		digit = (uint8_t)(initials_entered(0) + 1u);
		mem_write8(tile_place, 0u);
		mem_write8(tile_place + 1u, digit);
		control = (uint8_t)((board_control & ~3u) | shl32(1u, (int)(initials_entered(1) & 0x1Fu)));
		io_write8(IO_0A0001, control);
		board_control = control;
		turtles_in_black = 0x20u;
		return;
	}
	initials_entered(1) = 0u;
	initials_entered(2)++;
	if (initials_entered(2) >= 2u)
	{
		initials_entered(2) = 0u;
		scene++;
		scene_step = 0u;
		return;
	}
	tile_flip ^= 7u;
	tile_place = coin_counter_places[0];
	digit = (uint8_t)(initials_entered(1) + 1u);
	io_write8(tile_place, 0u);
	io_write8(tile_place + 1u, digit);
	tile_place = coin_counter_places[1];
	digit = (uint8_t)(initials_entered(0) + 1u);
	mem_write8(tile_place, 0u);
	mem_write8(tile_place + 1u, digit);
	control = (uint8_t)((board_control & ~3u) | shl32(1u, (int)(initials_entered(1) & 0x1Fu)));
	io_write8(IO_0A0001, control);
	board_control = control;
	turtles_in_black = 0x20u;
}

/* the test mode's sound test */
void sound_test_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		sound_test_clear();
		return;
	case 1:
		draw_sound_test();
		return;
	case 2:
		sound_test_play();
		return;
	}
}

/* the layers cleared */
void sound_test_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* the sound test's texts */
void draw_sound_test(void)
{
	uint32_t jobs = 0;

	if (jobs_pending == 0u)
	{
		jobs = table_loaded_by_lea_005B9C_ADDRESS;
		queue_jobs(jobs);
		ranking_time = 0u;
		initials_entered(0) = 0u;
		initials_entered(1) = 0u;
		scene_step++;
	}
}

/* the next sound played, by itself or by the button */
void sound_test_play(void)
{
	uint8_t sound = 0;
	uint16_t count = 0;
	uint32_t number_shown = 0, position = 0, sound_2 = 0, tile_place = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;

	t1 = ranking_time;
	ranking_time = t1 - 1u;
	if (t1 != 0u && (player_presses(0) & 0x20u) == 0u)
	{
		test_page_controls();
		return;
	}
	sound = 0u;
	queue_test_sound(sound);
	position = initials_entered(0);
	t2 = sound_test_codes[position];
	sound_2 = t2;
	if (t2 == 0u)
	{
		scene++;
		scene_step = 0u;
		return;
	}
	queue_test_sound((uint8_t)sound_2);
	tile_place = TILE_RAM + 0x8CAu;
	count = 1u;
	number_shown = initials_entered(1);
	draw_hex_digits(count, number_shown, tile_place);
	initials_entered(0)++;
	initials_entered(1)++;
	ranking_time = 0x12Cu;
	test_page_controls();
}

/* Unused: an empty routine that returns at once */
void unreached_005C0C(void)
{
}

/* the test mode's DIP switch settings */
void dip_switch_page(void)
{
	switch ((uint32_t)(int8_t)scene_step)
	{
	case 0:
		dip_switch_clear();
		return;
	case 1:
		draw_dip_switch_page();
		return;
	case 2:
		show_dip_settings();
		return;
	}
}

/* the layers cleared */
void dip_switch_clear(void)
{
	uint16_t job = 0;

	jobs_pending++;
	job = 7u;
	queue_job(job);
	scene_step++;
}

/* the settings' names */
void draw_dip_switch_page(void)
{
	uint32_t jobs = 0;

	if (jobs_pending == 0u)
	{
		jobs = table_loaded_by_lea_005C5E_ADDRESS;
		queue_jobs(jobs);
		scene_step++;
	}
}

/* every setting shown */
void show_dip_settings(void)
{
	show_coinage();
	show_difficulty();
	show_lives_setting();
	show_demo_sound();
	show_screen_setting();
	show_test_switch();
	test_page_controls();
}

/* the coins and credits of each slot */
void show_coinage(void)
{
	uint8_t coins = 0, coins_plural = 0, credits = 0, credits_plural = 0;
	uint16_t coinage_offset = 0, job = 0;
	uint32_t tile_place = 0;

	coinage_offset = io_read8(IO_0A0011);
	coinage_offset = (uint8_t)~coinage_offset;
	coinage_offset = coinage_offset & 0xFu;
	coinage_offset = (uint8_t)((uint32_t)coinage_offset << 1);
	coins = coinage_coins[coinage_offset];
	credits = coinage_credits[coinage_offset];
	tile_place = TILES_10032E;
	io_write8(TILES_10032E, 0u);
	io_write8(TILES_10032F, coins);
	io_write8(TILES_100340, 0u);
	io_write8(TILES_100341, credits);
	coins_plural = 0x10u;
	credits_plural = 0x10u;
	if (coins != 1u)
		coins_plural = 0x23u;
	if (credits != 1u)
		credits_plural = 0x23u;
	io_write8(tile_place + 0xCu, 0u);
	io_write8(tile_place + 0xDu, coins_plural);
	io_write8(tile_place + 0x1Eu, 0u);
	io_write8(tile_place + 0x1Fu, credits_plural);
	job = 0x1007u;
	queue_job(job);
}

/* Unused: queues the test mode's text 0x13 */
void unreached_005CF6(void)
{
	uint16_t job = 0;

	job = 0x1013u;
	queue_job(job);
}

/* the difficulty */
void show_difficulty(void)
{
	uint16_t difficulty_switches = 0;

	difficulty_switches = io_read8(IO_0A0013);
	difficulty_switches = (uint8_t)~difficulty_switches;
	difficulty_switches = difficulty_switches & 0x60u;
	difficulty_switches = difficulty_switches >> 5;
	difficulty_switches = (uint8_t)(difficulty_switches + 9u);
	difficulty_switches += 0x1000u;
	queue_job(difficulty_switches);
}

/* the lives a credit gives */
void show_lives_setting(void)
{
	uint8_t lives_tile = 0;
	uint16_t switches = 0;

	switches = io_read8(IO_0A0013);
	switches = (uint8_t)~switches;
	lives_tile = lives_settings[switches & 3u];
	io_write8(TILES_100838, 0u);
	io_write8(TILES_100839, lives_tile);
}

/* the demo sound on or off */
void show_demo_sound(void)
{
	uint16_t text_number = 0;

	text_number = 0xDu;
	if ((io_read8(IO_0A0013) & 0x80u) == 0u)
		text_number = (uint8_t)(text_number + 1u);
	text_number += 0x1000u;
	queue_job(text_number);
}

/* the screen normal or upside down */
void show_screen_setting(void)
{
	uint16_t text_number = 0;

	text_number = 0xFu;
	if ((io_read8(IO_0A0019) & 1u) == 0u)
		text_number = (uint8_t)(text_number + 1u);
	text_number += 0x1000u;
	queue_job(text_number);
}

/* the test switch */
void show_test_switch(void)
{
	uint16_t text_number = 0;

	text_number = 0x11u;
	if ((io_read8(IO_0A0019) & 4u) == 0u)
		text_number = (uint8_t)(text_number + 1u);
	text_number += 0x1000u;
	queue_job(text_number);
}

/* back to the test mode's start */
void test_mode_restart(void)
{
	scene = 0u;
	scene_step = 0u;
}

/* the button to the next page, and the screen's flip */
void test_page_controls(void)
{
	uint8_t flip = 0;

	if ((player_presses(0) & 0x10u) != 0u)
	{
		scene++;
		scene_step = 0u;
		stop_test_sound();
	}
	flip = tile_flip;
	if (scene != 5u)
	{
		flip &= 2u;
		if ((io_read8(IO_0A0019) & 1u) == 0u)
			flip ^= 7u;
	}
	tile_flip = flip;
	io_write8(TILES_106D00, flip);
}

/* the sound stopped */
void stop_test_sound(void)
{
	uint8_t sound = 0;

	sound = 0u;
	queue_test_sound(sound);
}

/* a sound into the sound board's queue */
void queue_test_sound(uint8_t sound)
{
	uint32_t queue_place = 0;

	queue_place = sound_queue_write;
	mem_write8(queue_place, sound);
	queue_place++;
	if (queue_place > sound_queue_ADDRESS + 0x3Fu)
		queue_place = sound_queue_ADDRESS;
	sound_queue_write = queue_place;
}
