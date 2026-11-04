/**
 *    Name:        video/speech_balloons.c
 *    Description: the speech balloons in the fixed layer with their speeches, and the lines said with them
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the pointer of a narrow balloon on the left of its speaker */
uint32_t draw_balloon_pointer(uint32_t a6)
{
	uint32_t tile_place = 0;

	if (balloon_on_right != 0u)
		return a6;
	if (game_on == 0u)
		return a6;
	tile_place = balloon_place;
	io_write8(tile_place + 0x188u, 0xBFu);
	io_write8(tile_place + 0x18Au, 0xBFu);
	io_write8(tile_place + 0x184u, 0xBFu);
	io_write8(tile_place + 0x186u, 0xBFu);
	io_write8(tile_place + 0x189u, 0xEEu);
	io_write8(tile_place + 0x18Bu, 0xEFu);
	io_write8(tile_place + 0x185u, 0xFEu);
	io_write8(tile_place + 0x187u, 0xFFu);
	return tile_place;
}

/* the balloon and speech "We gotta save Splinter" */
uint32_t say_we_gotta_save_splinter(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 5u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
	return on_right;
}

/* the balloon and speech "Let's move it" */
void say_lets_move_it(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0xAu;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the balloon and speech "You're dead, shell-brain", counted among the lines said */
void say_youre_dead_shell_brain(uint16_t screen_x, uint16_t screen_y, uint32_t *p_on_right, uint32_t *p_a6)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0;

	bebop_exchange_lines++;
	line = 0x14u;
	show_balloon_beside(screen_x, screen_y, line, p_on_right, &ignored, &ignored_2, p_a6);
	*p_a6 = draw_balloon_pointer(*p_a6);
}

/* the balloon and speech "Tonight I dine on turtle soup" */
uint32_t say_turtle_soup(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x19u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
	return on_right;
}

/* the balloon and speech "I'm invincible!" */
void say_im_invincible(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x1Eu;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the balloon and speech "Crash the turtles!" */
void say_crash_the_turtles(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x23u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the balloon and speech "Do not resist us" */
void say_do_not_resist_us(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x28u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the balloon and speech "Help meeee...!" */
uint32_t say_help_me(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x2Du;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
	return on_right;
}

/* the balloon and speech "Attaaaack!" */
void say_attack(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x32u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the balloon and speech "Pizza time!" above the speaker's height */
void say_pizza_time(uint32_t on_right, uint16_t screen_x, uint16_t screen_y, uint32_t object)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	screen_y = (uint16_t)((screen_y & ~0xFFu) | (uint8_t)(screen_y + OBJECT(object)->height_whole));
	line = 0x46u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* Unused: speech balloon 0x4B beside the place given, with its pointer */
void unreached_030062(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x4Bu;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* Unused: speech balloon 0x50 beside the place given, with its pointer */
void unreached_03006C(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x50u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the balloon and speech "Shell Shock" above the speaker's height */
void say_shell_shock(uint32_t on_right, uint16_t screen_x, uint16_t screen_y, uint32_t object, uint32_t *p_d4, uint32_t *p_d6)
{
	uint32_t d6 = *p_d6;
	uint16_t line = 0;
	uint32_t ignored = 0;

	screen_y = (uint16_t)((screen_y & ~0xFFu) | (uint8_t)(screen_y + OBJECT(object)->height_whole));
	line = 0x55u;
	show_balloon_beside(screen_x, screen_y, line, &on_right, p_d4, &d6, &ignored);
	draw_balloon_pointer(ignored);
	*p_d6 = (*p_d6 & 0xFFFF0000u) | (d6 & 0xFFFFu);
}

/* the balloon and speech "Yippee-I-Ay!" */
void say_yippee_i_ay(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	line = 0x5Au;
	show_balloon_beside(screen_x, screen_y, line, &on_right, &ignored, &ignored_2, &ignored_3);
	draw_balloon_pointer(ignored_3);
}

/* the wide balloon and speech "Say your prayers, toitles" */
void say_your_prayers_toitles(uint32_t on_right, uint8_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t bank = 0, ignored = 0, ignored_2 = 0;

	line = 0x5Fu;
	bank = 0x20u;
	show_balloon(screen_x, screen_y, line, &on_right, &bank, &ignored, &ignored_2);
	draw_wide_balloon_pointer(ignored_2);
}

/* the wide balloon and speech "They ain't getting any prettier!" */
void say_they_aint_getting_prettier(uint32_t on_right, uint16_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t bank = 0, ignored = 0, ignored_2 = 0;

	bebop_exchange_lines++;
	bank = 0xFFFFFFD0u;
	screen_x -= 0x30u;
	screen_x_high = screen_x;
	line = 0x69u;
	show_balloon(screen_x, screen_y, line, &on_right, &bank, &ignored, &ignored_2);
	draw_wide_balloon_pointer(ignored_2);
}

/* the wide balloon and speech "Watch your mouth, slime ball!" */
uint32_t say_watch_your_mouth(uint32_t on_right, uint16_t screen_x, uint16_t screen_y, uint32_t target)
{
	uint16_t line = 0;
	uint32_t bank = 0, ignored = 0;

	bebop_exchange_lines++;
	bank = 0xFFFFFFD0u;
	screen_x -= 0x30u;
	screen_x_high = screen_x;
	line = 0x6Eu;
	show_balloon(screen_x, screen_y, line, &on_right, &bank, &ignored, &target);
	return draw_wide_balloon_pointer(target);
}

/* the wide balloon and speech "Oh no, it's that overgrown cork-screw again" */
void say_overgrown_cork_screw(uint32_t on_right, uint8_t screen_x, uint16_t screen_y)
{
	uint16_t line = 0;
	uint32_t bank = 0, ignored = 0, ignored_2 = 0, tile_place = 0;

	bank = 0xFFFFFFD0u;
	line = 0x73u;
	show_balloon(screen_x, screen_y, line, &on_right, &bank, &ignored, &ignored_2);
	if (game_on == 0u)
		return;
	tile_place = balloon_place;
	io_write8(tile_place + 0x198u, 0xBFu);
	io_write8(tile_place + 0x19Au, 0xBFu);
	io_write8(tile_place + 0x18Eu, 0xBFu);
	io_write8(tile_place + 0x190u, 0xBFu);
	io_write8(tile_place + 0x199u, 0xEEu);
	io_write8(tile_place + 0x19Bu, 0xEFu);
	io_write8(tile_place + 0x18Fu, 0xFEu);
	io_write8(tile_place + 0x191u, 0xFFu);
}

/* the wide balloon's pointer drawn into its bottom row */
uint32_t draw_wide_balloon_pointer(uint32_t a6)
{
	uint32_t tile_place = 0;

	if (game_on == 0u)
		return a6;
	tile_place = balloon_place;
	io_write8(tile_place + 0x190u, 0xBFu);
	io_write8(tile_place + 0x192u, 0xBFu);
	io_write8(tile_place + 0x18Au, 0xBFu);
	io_write8(tile_place + 0x18Cu, 0xBFu);
	io_write8(tile_place + 0x191u, 0xEEu);
	io_write8(tile_place + 0x193u, 0xEFu);
	io_write8(tile_place + 0x18Bu, 0xFEu);
	io_write8(tile_place + 0x18Du, 0xFFu);
	return tile_place;
}

/* a narrow balloon to one side of the speaker */
void show_balloon_beside(uint16_t screen_x, uint16_t screen_y, uint16_t line, uint32_t *p_on_right, uint32_t *p_bank, uint32_t *p_fourth_row_tile, uint32_t *p_a6)
{
	uint32_t fourth_row_tile = *p_fourth_row_tile;

	balloon_on_right = *p_on_right;
	if ((uint8_t)*p_on_right != 0u)
	{
		screen_x += 0x20u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | screen_x;
	}
	else
	{
		screen_x -= 0x20u;
		screen_x_high = (screen_x_high & ~0xFFFFu) | screen_x;
	}
	*p_bank = 0x20u;
	show_balloon(screen_x, screen_y, line, p_on_right, p_bank, &fourth_row_tile, p_a6);
	*p_fourth_row_tile = (*p_fourth_row_tile & 0xFFFF0000u) | (fourth_row_tile & 0xFFFFu);
}

/* a speech balloon on the screen by the speaker, with its speech and the time it stays */
void show_balloon(uint32_t screen_x, uint16_t screen_y, uint16_t line, uint32_t *p_on_right, uint32_t *p_bank, uint32_t *p_fourth_row_tile, uint32_t *p_a6)
{
	uint32_t on_right = *p_on_right;
	uint16_t bank = (uint16_t)*p_bank;
	uint16_t fourth_row_tile = (uint16_t)*p_fourth_row_tile;
	uint8_t first_row_tile = 0, sound = 0, tile_attribute = 0, tile_banks = 0;
	uint16_t count = 0, record_attribute = 0, width = 0;
	uint32_t d6 = 0, second_row_tile = 0;

	if (game_on == 0u)
		return;
	if (balloon_time != 0u)
	{
		scratch_word = on_right;
		balloon_kept_y = screen_y;
		balloon_kept_line = line;
		balloon_kept_bank = bank;
		erase_balloon();
		on_right = (on_right & ~0xFFFFu) | scratch_word;
		screen_y = balloon_kept_y;
		line = balloon_kept_line;
		bank = balloon_kept_bank;
	}
	tile_banks = (uint8_t)((tile_banks_2_3 & 0xFu) | bank);
	tile_banks_2_3 = tile_banks;
	io_write8(TILES_106E00, tile_banks);
	balloons_said++;
	time_since_balloon = 0u;
	sound = TABLE8(balloon_records, (uint32_t)(int16_t)line);
	record_attribute = (uint16_t)((bank & ~0xFFu) | TABLE8(balloon_records, 0x2u + (uint32_t)(int16_t)line));
	width = TABLE8(balloon_records, 0x3u + (uint32_t)(int16_t)line);
	balloon_time = TABLE8(balloon_records, 0x4u + (uint32_t)(int16_t)line);
	first_row_tile = TABLE8(balloon_records, 0x1u + (uint32_t)(int16_t)line);
	queue_sound(sound);
	if ((uint8_t)on_right != 0u)
	{
		if (((screen_x & ~0xFF00u) | (screen_x_high & 0xFF00u)) >= 0xB0u)
			screen_x = ((((screen_x & ~0xFF00u) | (screen_x_high & 0xFF00u)) - 0x50u) & 0x1F8u) >> 2;
		else
			screen_x = 0x18u;
	}
	else if (((screen_x & ~0xFF00u) | (screen_x_high & 0xFF00u)) < 0x130u)
		screen_x = ((((screen_x & ~0xFF00u) | (screen_x_high & 0xFF00u)) + 0x10u) & 0x1F8u) >> 2;
	else
		screen_x = 0x50u;
	if ((uint16_t)screen_y < 0x70u)
		screen_y = (uint16_t)(((0u - screen_y - 0x50u) & 0xF8u) << 4);
	else
		screen_y = 0x400u;
	screen_x = (uint16_t)(screen_x + screen_y) + 0x100000u;
	*p_a6 = screen_x;
	balloon_place = screen_x;
	count = (uint16_t)width;
	on_right &= ~0xFFu;
	tile_attribute = (uint8_t)record_attribute;
	second_row_tile = (record_attribute & ~0xFFu) | first_row_tile;
	d6 = first_row_tile;
	second_row_tile = (second_row_tile & ~0xFFu) | (uint8_t)(second_row_tile + 0x10u);
	distance_low_byte = (uint8_t)(first_row_tile + 0x20u);
	d6 = (uint8_t)(d6 + 0x30u);
	screen_x_high = screen_x;
	draw_balloon_rows((uint8_t)on_right, tile_attribute, first_row_tile, count, &second_row_tile, &d6, p_a6);
	*p_on_right = on_right;
	*p_bank = (*p_bank & 0xFFFF0000u) | (second_row_tile & 0xFFFFu);
	*p_fourth_row_tile = (*p_fourth_row_tile & 0xFFFF0000u) | (d6 & 0xFFFFu);
}

/* the speech balloon erased */
void erase_balloon(void)
{
	uint8_t erasing = 0, first_row_tile = 0, tile_attribute = 0;
	uint16_t count = 0;
	uint32_t fourth_row_tile = 0, second_row_tile = 0, tile_place = 0;

	tile_place = balloon_place;
	erasing = 0xFFu;
	count = 0xFu;
	offset_high_word = 0xFu;
	tile_attribute = 0u;
	first_row_tile = 0x10u;
	second_row_tile = 0x10u;
	distance_low_byte = 0x10u;
	fourth_row_tile = 0x10u;
	draw_balloon_rows(erasing, tile_attribute, first_row_tile, count, &second_row_tile, &fourth_row_tile, &tile_place);
}

/* the four rows of a balloon's tiles drawn across */
void draw_balloon_rows(uint8_t erasing, uint8_t tile_attribute, uint8_t first_row_tile, uint16_t count, uint32_t *p_second_row_tile, uint32_t *p_fourth_row_tile, uint32_t *p_tile_place)
{
	uint8_t second_row_tile = (uint8_t)*p_second_row_tile;
	uint8_t fourth_row_tile = (uint8_t)*p_fourth_row_tile;
	uint32_t tile_place = *p_tile_place;
	uint16_t t1 = 0;

	do
	{
		io_write8(tile_place + 0x80u, tile_attribute);
		io_write8(tile_place + 0x100u, tile_attribute);
		io_write8(tile_place + 0x180u, tile_attribute);
		io_write8(tile_place, tile_attribute);
		tile_place++;
		io_write8(tile_place + 0x80u, second_row_tile);
		io_write8(tile_place + 0x100u, distance_low_byte);
		io_write8(tile_place + 0x180u, fourth_row_tile);
		io_write8(tile_place, first_row_tile);
		tile_place++;
		if ((int8_t)erasing >= 0)
		{
			first_row_tile++;
			second_row_tile++;
			distance_low_byte = (uint8_t)(distance_low_byte + 1u);
			fourth_row_tile++;
		}
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
	*p_second_row_tile = (*p_second_row_tile & 0xFFFFFF00u) | second_row_tile;
	*p_fourth_row_tile = (*p_fourth_row_tile & 0xFFFFFF00u) | fourth_row_tile;
	*p_tile_place = tile_place;
}
