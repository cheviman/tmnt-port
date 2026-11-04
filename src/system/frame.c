/**
 *    Name:        system/frame.c
 *    Description: the VBLANK interrupt: the inputs, the frame's work, the game's states and the job queue
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the coins: the coin counters, the coins inserted turned into credits */
void update_coins(void)
{
	uint32_t carry = 0;
	uint8_t coin_presses = 0, control = 0, service_presses = 0;
	uint16_t count = 0;
	uint32_t coin_slot = 0, counted_slot = 0, presses = 0;
	uint16_t t1 = 0, t2 = 0, credits = 0, t3 = 0;

	new_credit = 0u;
	control = board_control | 1u;
	coin_slot = COIN_SLOT_ADDRESS(0);
	count = 3u;
	if (COIN_SLOT(COIN_SLOT_ADDRESS(0))->counter_pulse == 0u)
	{
		count = (uint8_t)(count - 1u);
		coin_slot += 8u;
		if (COIN_SLOT(COIN_SLOT_ADDRESS(1))->counter_pulse == 0u)
		{
			count = (uint8_t)(count - 1u);
			coin_slot += 8u;
			if (COIN_SLOT(COIN_SLOT_ADDRESS(2))->counter_pulse == 0u)
			{
				count = (uint8_t)(count - 1u);
				coin_slot += 8u;
				if (COIN_SLOT(COIN_SLOT_ADDRESS(3))->counter_pulse == 0u)
				{
					coin_slot = COIN_SLOT_ADDRESS(0);
					count = 3u;
				}
			}
		}
	}
	for (;;)
	{
		pulse_coin_counter(coin_slot);
		if (COIN_SLOT(coin_slot)->counter_on != 0u)
			break;
		if (COIN_SLOT(coin_slot)->counter_pulse == 0u)
		{
			t1 = count - 1u;
			count = t1;
			coin_slot += 8u;
			if (t1 != 0xFFFFu)
				continue;
		}
		control &= ~1u;
		break;
	}
	board_control = control;
	io_write8(IO_0A0001, control);
	presses = new_coin_presses(0u);
	new_coin_switches = presses;
	coin_presses = new_coin_switches;
	counted_slot = COIN_SLOT_ADDRESS(0);
	count = 3u;
	do
	{
		carry = coin_presses & 1u;
		coin_presses = coin_presses >> 1;
		if (carry)
			count_coin(counted_slot);
		t2 = count - 1u;
		count = t2;
		counted_slot += 8u;
	} while (t2 != 0xFFFFu);
	service_presses = new_coin_switches >> 4;
	coin_slot = COIN_SLOT_ADDRESS(0);
	count = 3u;
	do
	{
		carry = service_presses & 1u;
		service_presses = service_presses >> 1;
		if (carry)
		{
			new_credit = 1u;
			credits = COIN_SLOT(coin_slot)->credits;
			carry = (uint16_t)(credits + 1u) < credits;
			COIN_SLOT(coin_slot)->credits = credits + 1u;
			if (carry)
				COIN_SLOT(coin_slot)->credits--;
		}
		t3 = count - 1u;
		count = t3;
		coin_slot += 8u;
	} while (t3 != 0xFFFFu);
}

/* the coin switches just pressed, with the coin's sound */
uint32_t new_coin_presses(uint32_t presses)
{
	uint8_t command = 0;
	uint8_t t1 = 0;

	presses = (presses & ~0xFFu) | coin_switches;
	presses = presses | coin_switches_last_frame;
	presses = (presses & ~0xFFu) | (uint8_t)~presses;
	t1 = presses & coin_switches_2_frames_ago;
	presses = (presses & ~0xFFu) | t1;
	if (t1 == 0u || game_state == 3u)
		return presses;
	if (game_on != 0u)
	{
		command = 0x86u;
		queue_sound_command(command);
	}
	else
	{
		command = 0xFFu;
		queue_sound_command(command);
		command = 0x86u;
		queue_sound_command(command);
		sound_stopped_by_coin = 1u;
	}
	return presses;
}

/* a coin slot's counter pulsed for each coin */
void pulse_coin_counter(uint32_t coin_slot)
{
	uint8_t counter_on = 0, counter_pulse = 0;
	uint8_t counter_pulse_2 = 0;

	counter_on = 0u;
	counter_pulse_2 = COIN_SLOT(coin_slot)->counter_pulse;
	counter_pulse = counter_pulse_2;
	if (counter_pulse_2 == 0u)
	{
		if (COIN_SLOT(coin_slot)->coins_to_count != 0u)
		{
			COIN_SLOT(coin_slot)->coins_to_count--;
			COIN_SLOT(coin_slot)->counter_on = counter_on + 1u;
			COIN_SLOT(coin_slot)->counter_pulse = 0x3Cu;
		}
		return;
	}
	counter_pulse--;
	COIN_SLOT(coin_slot)->counter_pulse = counter_pulse;
	if (counter_pulse != 0x1Eu)
		return;
	COIN_SLOT(coin_slot)->counter_on = counter_on;
}

/* a coin inserted, counted towards the slot's credits */
void count_coin(uint32_t coin_slot)
{
	uint32_t carry = 0;
	uint16_t credits = 0;
	uint16_t credits_2 = 0;

	COIN_SLOT(coin_slot)->coins_to_count++;
	COIN_SLOT(coin_slot)->coins++;
	if (COIN_SLOT(coin_slot)->coins != COIN_SLOT(COIN_SLOT_ADDRESS(0))->coins_per_credit)
		return;
	new_credit = 1u;
	credits = COIN_SLOT(COIN_SLOT_ADDRESS(0))->credits_per_coins;
	credits_2 = COIN_SLOT(coin_slot)->credits;
	carry = (uint16_t)(credits + credits_2) < credits;
	credits += credits_2;
	if (carry == 0u)
		COIN_SLOT(coin_slot)->credits = credits;
	COIN_SLOT(coin_slot)->coins = 0u;
}

/* each slot's credits added to its player's lives */
uint32_t add_credit_lives(void)
{
	uint32_t carry = 0, extend = 0;
	uint8_t slot = 0;
	uint16_t count = 0, count_2 = 0, lives = 0, players = 0;
	uint32_t coin_slot = 0, player = 0, player_2 = 0, player_number = 0;
	uint16_t credits = 0, t1 = 0, t2 = 0, t3 = 0;

	coin_slot = COIN_SLOT_ADDRESS(0);
	player_2 = PLAYER_ADDRESS(0);
	count = 3u;
	slot = 0u;
	do
	{
		credits = COIN_SLOT(coin_slot)->credits;
		count_2 = credits;
		if (credits != 0u)
		{
			players_credited |= shl32(1u, (int)(slot & 7u));
			count_2 = (uint16_t)((count_2 & ~0xFFu) | (uint8_t)(count_2 - 1u));
			do
			{
				distance_low_byte = lives_per_credit;
				lives = PLAYER(player_2)->lives;
				extend = 0u;
				carry = m68k_abcd_cy((uint8_t)lives, distance_low_byte, extend);
				lives = (uint16_t)((lives & ~0xFFu) | m68k_abcd((uint8_t)lives, distance_low_byte, extend));
				if (carry)
				{
					lives += 0x100u;
					if (lives >= 0x999u)
						lives = 0x999u;
				}
				PLAYER(player_2)->lives = lives;
				COIN_SLOT(coin_slot)->credits--;
				t1 = count_2 - 1u;
				count_2 = t1;
			} while (t1 != 0xFFFFu);
		}
		slot++;
		t2 = count - 1u;
		count = t2;
		coin_slot += 8u;
		player_2 += 8u;
	} while (t2 != 0xFFFFu);
	player = PLAYER_ADDRESS(3);
	player_number = 3u;
	offset_high_word = 3u;
	players = 0u;
	do
	{
		if (PLAYER(player)->lives != 0u)
		{
			players |= shl32(1u, (int)(player_number & 0x1Fu));
			players_out_of_lives &= ~shl32(1u, (int)(player_number & 7u));
		}
		t3 = player_number - 1u;
		player_number = t3;
		player -= 8u;
	} while (t3 != 0xFFFFu);
	players_with_lives = (uint8_t)players;
	fade_banks = players;
	return player_number;
}

/* Unused: a column of 28 tiles set to the one given */
void unreached_003DAA(uint8_t tile_code)
{
	uint16_t count = 0;
	uint32_t tile_place = 0;
	uint16_t t1 = 0;

	tile_place = TILE_RAM + 0x11Au;
	count = 0x1Bu;
	do
	{
		io_write8(tile_place, 0u);
		io_write8(tile_place + 1u, tile_code);
		tile_place += 0x80u;
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
}

/* the frame's interrupt: the inputs, the coins, the sound, the palettes and the game state's frame */
void vblank_interrupt(void)
{
	uint32_t d7 = 0;

	board_control &= 0xDFu;
	io_write8(IO_0A0001, board_control);
	io_write8(IO_0A0011, 0u);
	write_status(0u);
	take_interrupts();
	frame_count++;
	sound_stopped_by_coin = 0u;
	send_sound_command();
	d7 = read_inputs(0u);
	if (game_state != 3u)
	{
		update_coins();
		d7 = add_credit_lives();
	}
	run_palette_requests((uint16_t)d7);
	step_palette_effects();
	switch (game_state)
	{
	case 0:
		run_startup();
		break;
	case 1:
		run_attract();
		break;
	case 2:
		run_game();
		break;
	case 3:
		run_test_mode();
		break;
	}
	board_control |= 0x20u;
	io_write8(IO_0A0001, board_control);
}

/* the coins and the four players' inputs read, and the layers' scroll written */
uint32_t read_inputs(uint32_t d7)
{
	uint8_t inputs = 0;
	uint32_t scroll_x = 0;

	player_inputs_3_frames_ago = player_inputs_2_frames_ago;
	player_inputs_2_frames_ago = (((uint32_t)player_inputs_last_frame(0) << 24u) | ((uint32_t)player_inputs_last_frame(1) << 16u) | ((uint32_t)player_inputs_last_frame(2) << 8u) | player_inputs_last_frame(3));
	player_inputs_last_frame(0) = player_inputs(0);
	player_inputs_last_frame(1) = player_inputs(1);
	player_inputs_last_frame(2) = player_inputs(2);
	player_inputs_last_frame(3) = player_inputs(3);
	coin_switches_3_frames_ago = coin_switches_2_frames_ago;
	coin_switches_2_frames_ago = coin_switches_last_frame;
	coin_switches_last_frame = coin_switches;
	coin_switches = ~io_read8(IO_0A0001);
	inputs = (uint8_t)~io_read8(IO_0A0003);
	player_inputs(0) = inputs;
	player_presses(0) = inputs & ~player_inputs_last_frame(0);
	inputs = (uint8_t)~io_read8(IO_0A0005);
	player_inputs(1) = inputs;
	player_presses(1) = inputs & ~player_inputs_last_frame(1);
	inputs = (uint8_t)~io_read8(IO_0A0007);
	player_inputs(2) = inputs;
	player_presses(2) = inputs & ~player_inputs_last_frame(2);
	inputs = (uint8_t)~io_read8(IO_0A0015);
	player_inputs(3) = inputs;
	player_presses(3) = inputs & ~player_inputs_last_frame(3);
	io_write8(TILES_106018, (uint8_t)layer_a_scroll_y);
	scroll_x = (d7 & ~0xFFFFu) | layer_a_scroll_x;
	scroll_x = (scroll_x & ~0xFFFFu) | (uint16_t)(scroll_x - 0x60u);
	io_write8(TILES_106400, scroll_x);
	scroll_x = (scroll_x & ~0xFFFFu) | ((uint16_t)scroll_x >> 8);
	io_write8(TILES_106402, scroll_x);
	io_write8(TILES_106019, (uint8_t)layer_b_scroll_y);
	scroll_x = (scroll_x & ~0xFFFFu) | layer_b_scroll_x;
	scroll_x = (scroll_x & ~0xFFFFu) | (uint16_t)(scroll_x - 0x60u);
	io_write8(TILES_106401, scroll_x);
	scroll_x = (scroll_x & ~0xFFFFu) | ((uint16_t)scroll_x >> 8);
	io_write8(TILES_106403, scroll_x);
	return scroll_x;
}

/* the steps after the reset */
void run_startup(void)
{
	switch (game_step)
	{
	case 0:
		startup_begin();
		return;
	case 1:
		startup_wait();
		return;
	case 2:
		startup_end();
		return;
	}
}

/* the video chips set, the test mode when its switch is on, else the screen cleared */
void startup_begin(void)
{
	uint16_t job = 0, kind = 0;
	uint32_t data = 0;

	tile_banks_0_1 = 0u;
	io_write8(TILES_106B00, 0u);
	tile_banks_2_3 = 0u;
	io_write8(TILES_106E00, 0u);
	io_write8(0x140000u, sprite_control);
	set_screen_flip();
	game_on = 0u;
	if ((dip_switches_3 & 4u) != 0u)
	{
		game_state = 3u;
		return;
	}
	game_step++;
	jobs_pending = 1u;
	job = 1u;
	queue_job(job);
	kind = 0x20Fu;
	data = 0xA05Eu;
	post_palette_request(kind, data);
}

/* the screen cleared */
void startup_wait(void)
{
	if (jobs_pending == 0u && palettes_busy == 0u)
	{
		step_timer = 1u;
		game_step++;
	}
}

/* on to the attract mode */
void startup_end(void)
{
	uint8_t t1 = 0;

	t1 = step_timer;
	step_timer = t1 - 1u;
	if (t1 == 1u || new_credit != 0u || (coin_switches != 0u && free_play != 0u))
	{
		game_state++;
		game_step = 0u;
		return;
	}
}

/* Unused: the screen's flip set */
void unreached_004068(void)
{
	set_screen_flip();
}

/* the screen flipped by its DIP switch */
void set_screen_flip(void)
{
	uint8_t control = 0, flip = 0;

	flip = tile_flip & ~1u;
	control = sprite_control & ~8u;
	if ((dip_switches_3 & 1u) != 0u)
	{
		flip ^= 1u;
		control ^= 8u;
	}
	tile_flip = flip;
	io_write8(TILES_106D00, flip);
	sprite_control = control;
	io_write8(0x140000u, control);
}

/* the attract mode's frame, and a game when a player starts */
void run_attract(void)
{
	uint8_t sound = 0;

	switch (game_step)
	{
	case 0:
		attract_begin();
		break;
	case 1:
		attract_coin_screen();
		break;
	case 2:
		attract_opening();
		break;
	case 3:
		attract_title();
		break;
	case 4:
		attract_demo();
		break;
	case 5:
		attract_profile();
		break;
	case 6:
		attract_best_players();
		break;
	case 7:
		attract_spare_step();
		break;
	case 8:
		attract_end();
		break;
	}
	check_starts();
	if (scratch_byte != 0u)
	{
		sound = 0u;
		queue_sound(sound);
		game_step = 0u;
		scene = 0u;
		scene_step = 0u;
		scene_substep = 0u;
		game_state++;
	}
}

/* the game's frame by its scene */
void run_game(void)
{
	switch (scene)
	{
	case 0:
		scene_new_game();
		return;
	case 1:
		scene_choose_turtles();
		return;
	case 2:
		scene_area_setup();
		return;
	case 3:
		scene_area_title();
		return;
	case 4:
		scene_play();
		return;
	case 5:
		scene_area_exit();
		return;
	case 6:
		scene_ending();
		return;
	case 7:
		scene_continue();
		return;
	case 8:
		scene_ranking();
		return;
	}
}

/* the test mode's frame */
void run_test_mode(void)
{
	switch ((uint32_t)(int8_t)scene)
	{
	case 0:
		test_mode_start();
		break;
	case 1:
		mask_rom_check_page();
		break;
	case 2:
		cross_hatch_page();
		break;
	case 3:
		color_test_page();
		break;
	case 4:
		io_test_page();
		break;
	case 5:
		skip_coin_counter_test();
		break;
	case 6:
		sound_test_page();
		break;
	case 7:
		dip_switch_page();
		break;
	case 8:
		test_mode_restart();
		break;
	}
	read_inputs(0u);
}

/* a job into the main loop's queue */
void queue_job(uint16_t job)
{
	uint32_t queue_place = 0;

	queue_place = job_queue_write;
	mem_write16(queue_place, job);
	queue_place += 2u;
	if (queue_place > job_queue_ADDRESS + 0x7Fu)
		queue_place = job_queue_ADDRESS;
	job_queue_write = queue_place;
}

/* a list of jobs into the main loop's queue */
void queue_jobs(uint32_t jobs)
{
	uint16_t job = 0;

	for (;;)
	{
		job = JOB_WORD(jobs);
		jobs += 2u;
		if (job == 0xFFFFu)
			break;
		queue_job(job);
	}
}

/* the second interrupt, which the board never raises */
void irq6_interrupt(void)
{
	demo_hold_3 = 0u;
	return_from_exception();
}

/* in the attract mode, the players with lives who press a button start the game */
void check_starts(void)
{
	uint8_t players = 0;
	uint16_t player_number = 0;
	uint32_t inputs = 0, player = 0;
	uint8_t t1 = 0;

	scratch_byte = 0u;
	player_number = 0u;
	player = PLAYER_ADDRESS(0);
	inputs = player_inputs_ADDRESS;
	do
	{
		if (PLAYER(player)->lives != 0u && (player_inputs(player_number) & 0x30u) != 0u && (player_inputs_last_frame(player_number) & 0x30u) == 0u)
			scratch_byte |= shl32(1u, (int)(player_number & 7u));
		player_number = (uint8_t)(player_number + 1u);
		player += 8u;
	} while (player_number < 4u);
	t1 = scratch_byte;
	players = t1;
	if (t1 != 0u)
	{
		players_in_play = players;
		players_joining = players;
		game_on = 1u;
	}
}

/* in a game, the players with lives who press a button join it */
void check_joins(void)
{
	uint8_t players = 0;
	uint16_t player_number = 0;
	uint32_t inputs = 0, player = 0;
	uint8_t t1 = 0;
	uint16_t lives = 0;

	if (game_on == 0u)
		return;
	scratch_byte = 0u;
	player_number = 0u;
	player = PLAYER_ADDRESS(0);
	inputs = player_inputs_ADDRESS;
	do
	{
		if ((players_in_play & shl32(1u, (int)(player_number & 7u))) == 0u)
		{
			lives = PLAYER(player)->lives;
			screen_x_high = lives;
			if (lives != 0u && (player_inputs(player_number) & 0x30u) != 0u && (player_inputs_last_frame(player_number) & 0x30u) == 0u)
				scratch_byte |= shl32(1u, (int)(player_number & 7u));
		}
		player_number = (uint8_t)(player_number + 1u);
		player += 8u;
	} while (player_number < 4u);
	t1 = scratch_byte;
	players = t1;
	if (t1 != 0u)
	{
		players_in_play |= players;
		players_joining |= players;
		game_on = 1u;
	}
}

/* the first player's score and health reset */
uint32_t reset_player_1(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(0);
	reset_score(player);
	PLAYER(PLAYER_ADDRESS(0))->health_bars = 0u;
	return player;
}

/* the second player's score and health reset */
uint32_t reset_player_2(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(1);
	reset_score(player);
	PLAYER(PLAYER_ADDRESS(1))->health_bars = 0u;
	return player;
}

/* the third player's score and health reset */
uint32_t reset_player_3(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(2);
	reset_score(player);
	PLAYER(PLAYER_ADDRESS(2))->health_bars = 0u;
	return player;
}

/* the fourth player's score and health reset */
uint32_t reset_player_4(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(3);
	reset_score(player);
	PLAYER(PLAYER_ADDRESS(3))->health_bars = 0u;
	return player;
}

/* a life of the first player's used */
void use_life_player_1(void)
{
	uint8_t player_number = 0;
	uint32_t player = 0;

	player = PLAYER_ADDRESS(0);
	player_number = 0u;
	use_life(player_number, player);
}

/* a life of the second player's used */
void use_life_player_2(void)
{
	uint8_t player_number = 0;
	uint32_t player = 0;

	player = PLAYER_ADDRESS(1);
	player_number = 1u;
	use_life(player_number, player);
}

/* a life of the third player's used */
void use_life_player_3(void)
{
	uint8_t player_number = 0;
	uint32_t player = 0;

	player = PLAYER_ADDRESS(2);
	player_number = 2u;
	use_life(player_number, player);
}

/* a life of the fourth player's used */
void use_life_player_4(void)
{
	uint8_t player_number = 0;
	uint32_t player = 0;

	player = PLAYER_ADDRESS(3);
	player_number = 3u;
	use_life(player_number, player);
}

/* a life used and the health full; with none left the player is out */
void use_life(uint8_t player_number, uint32_t player)
{
	uint32_t carry = 0, extend = 0;
	uint8_t lives_low = 0;

	extend = 0u;
	lives_low = PLAYER(player)->lives_low;
	carry = m68k_sbcd_cy(lives_low, 1u, extend);
	lives_low = m68k_sbcd(lives_low, 1u, extend);
	if (carry == 0u)
	{
		PLAYER(player)->lives_low = lives_low;
		PLAYER(player)->health_bars = 0xAu;
		return;
	}
	if ((uint8_t)(PLAYER(player)->lives >> 8u) == 0u)
	{
		players_in_play &= ~shl32(1u, (int)(player_number));
		players_out_of_lives |= shl32(1u, (int)(player_number));
		return;
	}
	PLAYER(player)->lives = (PLAYER(player)->lives & 0xFFu) | (((uint8_t)(PLAYER(player)->lives >> 8u) - 1u & 0xFFu) << 8u);
	lives_low = 0x99u;
	PLAYER(player)->lives_low = lives_low;
	PLAYER(player)->health_bars = 0xAu;
}

/* a player's score reset */
void reset_score(uint32_t player)
{
	PLAYER(player)->score = 0u;
	PLAYER(player)->unused_setting_copy = unused_dip_setting;
}

/* the players who join or come back, their boxes and messages */
uint32_t update_players_in_game(uint32_t marker)
{
	marker = join_players(marker);
	redraw_new_lives();
	marker = respawn_players(marker);
	update_player_messages();
	return marker;
}

/* the boxes whose lives a coin changed */
void redraw_new_lives(void)
{
	uint16_t box_flags = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	box_flags = (uint16_t)(0xB00u | players_credited);
	box_flags = (uint16_t)(box_flags | 0x30u);
	draw_player_boxes(box_flags, &ignored, &ignored_2, &ignored_3);
	players_credited = 0u;
}

/* the players who join: their turtles in, a life each */
uint32_t join_players(uint32_t marker)
{
	uint8_t players = 0;
	uint16_t box_flags = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	if (players_joining == 0u)
		return marker;
	players = players_joining;
	marker = turtle_join(players);
	if ((players_joining & 1u) != 0u)
		use_life_player_1();
	if ((players_joining & 2u) != 0u)
		use_life_player_2();
	if ((players_joining & 4u) != 0u)
		use_life_player_3();
	if ((players_joining & 8u) != 0u)
		use_life_player_4();
	box_flags = (uint16_t)(0xB00u | players_joining);
	box_flags = (uint16_t)(box_flags | 0x30u);
	draw_player_boxes(box_flags, &ignored, &ignored_2, &ignored_3);
	players_joining = 0u;
	return marker;
}

/* the players whose turtle was knocked out: a life each, the turtle back */
uint32_t respawn_players(uint32_t marker)
{
	uint8_t players = 0;
	uint16_t box_flags = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0;

	if (players_knocked_out == 0u)
		return marker;
	if ((players_knocked_out & 1u) != 0u)
		use_life_player_1();
	if ((players_knocked_out & 2u) != 0u)
		use_life_player_2();
	if ((players_knocked_out & 4u) != 0u)
		use_life_player_3();
	if ((players_knocked_out & 8u) != 0u)
		use_life_player_4();
	players = players_in_play & players_knocked_out;
	marker = turtle_respawn(players);
	box_flags = (uint16_t)(0xB00u | players_knocked_out);
	box_flags = (uint16_t)(box_flags | 0x30u);
	draw_player_boxes(box_flags, &ignored, &ignored_2, &ignored_3);
	players_knocked_out = 0u;
	return marker;
}

/* the messages under the four boxes */
void update_player_messages(void)
{
	uint16_t count = 0;
	uint16_t t1 = 0;

	count = 3u;
	do
	{
		update_player_message((uint8_t)count);
		t1 = count - 1u;
		count = t1;
	} while (t1 != 0xFFFFu);
}

/* the message under a player's box by how he stands */
void update_player_message(uint8_t count)
{
	uint8_t player_message = 0;
	uint32_t ignored = 0, ignored_2 = 0;

	if (((players_with_lives | players_joining | players_in_play | players_knocked_out | (screen_x_high & ~0xFFu)) & shl32(1u, (int)(count & 0x1Fu))) != 0u)
	{
		if (((players_joining | players_in_play | players_knocked_out | players_out_of_lives | (screen_x_high & ~0xFFu)) & shl32(1u, (int)(count & 0x1Fu))) != 0u || (players_with_lives & shl32(1u, (int)(count & 7u))) == 0u)
		{
			offset_high_word = 0u;
			player_message = 0xCu;
		}
		else
		{
			offset_high_word = 0u;
			player_message = 4u;
		}
	}
	else if ((players_out_of_lives & shl32(1u, (int)(count & 7u))) != 0u)
	{
		offset_high_word = 0u;
		player_message = 8u;
	}
	else
	{
		player_message = 0u;
		offset_high_word = 0u;
	}
	player_message = (uint8_t)(uint16_t)((uint8_t)(player_message | count) + 0xD00u);
	draw_player_message(player_message, &ignored, &ignored_2);
}

/* Unused: queues the drawing of the players' boxes the bits given pick */
void unreached_0044EE(uint16_t box_flags)
{
	box_flags |= 0xB30u;
	queue_job(box_flags);
}

/* the objects, the sprites, the palette effects and the play's variables cleared */
void clear_play(void)
{
	clear_palette_effects();
	clear_objects();
	clear_sprite_ram();
	clear_engine_variables();
	clear_area_variables();
	clear_engine_ram();
}

/* a stretch of work RAM zeroed */
void zero_ram_below(uint16_t count, uint32_t ram_end)
{
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0;
	uint16_t t65 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0, t25 = 0, t26 = 0, t27 = 0, t28 = 0, t29 = 0, t30 = 0, t31 = 0, t32 = 0, t33 = 0, t34 = 0, t35 = 0, t36 = 0, t37 = 0, t38 = 0, t39 = 0, t40 = 0, t41 = 0, t42 = 0, t43 = 0, t44 = 0, t45 = 0, t46 = 0, t47 = 0, t48 = 0, t49 = 0, t50 = 0, t51 = 0, t52 = 0, t53 = 0, t54 = 0, t55 = 0, t56 = 0, t57 = 0, t58 = 0, t59 = 0, t60 = 0, t61 = 0, t62 = 0, t63 = 0, t64 = 0;

	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = ram_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		t9 = t8 - 4u;
		mem_write16(t9 + 2u, fill_8);
		mem_write16(t9, fill_8 >> 16);
		t10 = t9 - 4u;
		mem_write16(t10 + 2u, fill_7);
		mem_write16(t10, fill_7 >> 16);
		t11 = t10 - 4u;
		mem_write16(t11 + 2u, fill_6);
		mem_write16(t11, fill_6 >> 16);
		t12 = t11 - 4u;
		mem_write16(t12 + 2u, fill_5);
		mem_write16(t12, fill_5 >> 16);
		t13 = t12 - 4u;
		mem_write16(t13 + 2u, fill_4);
		mem_write16(t13, fill_4 >> 16);
		t14 = t13 - 4u;
		mem_write16(t14 + 2u, fill_3);
		mem_write16(t14, fill_3 >> 16);
		t15 = t14 - 4u;
		mem_write16(t15 + 2u, fill_2);
		mem_write16(t15, fill_2 >> 16);
		t16 = t15 - 4u;
		mem_write16(t16 + 2u, fill);
		mem_write16(t16, fill >> 16);
		t17 = t16 - 4u;
		mem_write16(t17 + 2u, fill_8);
		mem_write16(t17, fill_8 >> 16);
		t18 = t17 - 4u;
		mem_write16(t18 + 2u, fill_7);
		mem_write16(t18, fill_7 >> 16);
		t19 = t18 - 4u;
		mem_write16(t19 + 2u, fill_6);
		mem_write16(t19, fill_6 >> 16);
		t20 = t19 - 4u;
		mem_write16(t20 + 2u, fill_5);
		mem_write16(t20, fill_5 >> 16);
		t21 = t20 - 4u;
		mem_write16(t21 + 2u, fill_4);
		mem_write16(t21, fill_4 >> 16);
		t22 = t21 - 4u;
		mem_write16(t22 + 2u, fill_3);
		mem_write16(t22, fill_3 >> 16);
		t23 = t22 - 4u;
		mem_write16(t23 + 2u, fill_2);
		mem_write16(t23, fill_2 >> 16);
		t24 = t23 - 4u;
		mem_write16(t24 + 2u, fill);
		mem_write16(t24, fill >> 16);
		t25 = t24 - 4u;
		mem_write16(t25 + 2u, fill_8);
		mem_write16(t25, fill_8 >> 16);
		t26 = t25 - 4u;
		mem_write16(t26 + 2u, fill_7);
		mem_write16(t26, fill_7 >> 16);
		t27 = t26 - 4u;
		mem_write16(t27 + 2u, fill_6);
		mem_write16(t27, fill_6 >> 16);
		t28 = t27 - 4u;
		mem_write16(t28 + 2u, fill_5);
		mem_write16(t28, fill_5 >> 16);
		t29 = t28 - 4u;
		mem_write16(t29 + 2u, fill_4);
		mem_write16(t29, fill_4 >> 16);
		t30 = t29 - 4u;
		mem_write16(t30 + 2u, fill_3);
		mem_write16(t30, fill_3 >> 16);
		t31 = t30 - 4u;
		mem_write16(t31 + 2u, fill_2);
		mem_write16(t31, fill_2 >> 16);
		t32 = t31 - 4u;
		mem_write16(t32 + 2u, fill);
		mem_write16(t32, fill >> 16);
		t33 = t32 - 4u;
		mem_write16(t33 + 2u, fill_8);
		mem_write16(t33, fill_8 >> 16);
		t34 = t33 - 4u;
		mem_write16(t34 + 2u, fill_7);
		mem_write16(t34, fill_7 >> 16);
		t35 = t34 - 4u;
		mem_write16(t35 + 2u, fill_6);
		mem_write16(t35, fill_6 >> 16);
		t36 = t35 - 4u;
		mem_write16(t36 + 2u, fill_5);
		mem_write16(t36, fill_5 >> 16);
		t37 = t36 - 4u;
		mem_write16(t37 + 2u, fill_4);
		mem_write16(t37, fill_4 >> 16);
		t38 = t37 - 4u;
		mem_write16(t38 + 2u, fill_3);
		mem_write16(t38, fill_3 >> 16);
		t39 = t38 - 4u;
		mem_write16(t39 + 2u, fill_2);
		mem_write16(t39, fill_2 >> 16);
		t40 = t39 - 4u;
		mem_write16(t40 + 2u, fill);
		mem_write16(t40, fill >> 16);
		t41 = t40 - 4u;
		mem_write16(t41 + 2u, fill_8);
		mem_write16(t41, fill_8 >> 16);
		t42 = t41 - 4u;
		mem_write16(t42 + 2u, fill_7);
		mem_write16(t42, fill_7 >> 16);
		t43 = t42 - 4u;
		mem_write16(t43 + 2u, fill_6);
		mem_write16(t43, fill_6 >> 16);
		t44 = t43 - 4u;
		mem_write16(t44 + 2u, fill_5);
		mem_write16(t44, fill_5 >> 16);
		t45 = t44 - 4u;
		mem_write16(t45 + 2u, fill_4);
		mem_write16(t45, fill_4 >> 16);
		t46 = t45 - 4u;
		mem_write16(t46 + 2u, fill_3);
		mem_write16(t46, fill_3 >> 16);
		t47 = t46 - 4u;
		mem_write16(t47 + 2u, fill_2);
		mem_write16(t47, fill_2 >> 16);
		t48 = t47 - 4u;
		mem_write16(t48 + 2u, fill);
		mem_write16(t48, fill >> 16);
		t49 = t48 - 4u;
		mem_write16(t49 + 2u, fill_8);
		mem_write16(t49, fill_8 >> 16);
		t50 = t49 - 4u;
		mem_write16(t50 + 2u, fill_7);
		mem_write16(t50, fill_7 >> 16);
		t51 = t50 - 4u;
		mem_write16(t51 + 2u, fill_6);
		mem_write16(t51, fill_6 >> 16);
		t52 = t51 - 4u;
		mem_write16(t52 + 2u, fill_5);
		mem_write16(t52, fill_5 >> 16);
		t53 = t52 - 4u;
		mem_write16(t53 + 2u, fill_4);
		mem_write16(t53, fill_4 >> 16);
		t54 = t53 - 4u;
		mem_write16(t54 + 2u, fill_3);
		mem_write16(t54, fill_3 >> 16);
		t55 = t54 - 4u;
		mem_write16(t55 + 2u, fill_2);
		mem_write16(t55, fill_2 >> 16);
		t56 = t55 - 4u;
		mem_write16(t56 + 2u, fill);
		mem_write16(t56, fill >> 16);
		t57 = t56 - 4u;
		mem_write16(t57 + 2u, fill_8);
		mem_write16(t57, fill_8 >> 16);
		t58 = t57 - 4u;
		mem_write16(t58 + 2u, fill_7);
		mem_write16(t58, fill_7 >> 16);
		t59 = t58 - 4u;
		mem_write16(t59 + 2u, fill_6);
		mem_write16(t59, fill_6 >> 16);
		t60 = t59 - 4u;
		mem_write16(t60 + 2u, fill_5);
		mem_write16(t60, fill_5 >> 16);
		t61 = t60 - 4u;
		mem_write16(t61 + 2u, fill_4);
		mem_write16(t61, fill_4 >> 16);
		t62 = t61 - 4u;
		mem_write16(t62 + 2u, fill_3);
		mem_write16(t62, fill_3 >> 16);
		t63 = t62 - 4u;
		mem_write16(t63 + 2u, fill_2);
		mem_write16(t63, fill_2 >> 16);
		t64 = t63 - 4u;
		mem_write16(t64 + 2u, fill);
		mem_write16(t64, fill >> 16);
		ram_end = t64;
		t65 = count - 1u;
		count = t65;
	} while (t65 != 0xFFFFu);
}

/* zeroes for the clearing of the work RAM */
void zero_fill_registers(uint32_t *p_fill, uint32_t *p_fill_2, uint32_t *p_fill_3, uint32_t *p_fill_4, uint32_t *p_fill_5, uint32_t *p_fill_6, uint32_t *p_fill_7, uint32_t *p_fill_8)
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

/* the area's and the play's variables cleared */
void clear_area_variables(void)
{
	clear_area_counters();
	clear_enemy_variables();
	clear_spare_ram();
}

/* Unused: the four players cleared and two stretches of work RAM zeroed */
void unreached_00456A(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	clear_player_1();
	clear_player_2();
	clear_player_3();
	clear_player_4();
	ram_end = 0x60200u;
	count = 0u;
	zero_ram_below(count, ram_end);
	ram_end = marker_times_ADDRESS;
	count = 0u;
	zero_ram_below(count, ram_end);
}

/* the first player's record cleared */
void clear_player_1(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(0);
	PLAYER(player)->lives = 0u;
	PLAYER(player)->unused_setting_copy = 0u;
	PLAYER(player)->score = 0u;
	PLAYER(player)->health_bars = 0u;
	PLAYER(player)->message = 0u;
}

/* the second player's record cleared */
void clear_player_2(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(1);
	PLAYER(player)->lives = 0u;
	PLAYER(player)->unused_setting_copy = 0u;
	PLAYER(player)->score = 0u;
	PLAYER(player)->health_bars = 0u;
	PLAYER(player)->message = 0u;
}

/* the third player's record cleared */
void clear_player_3(void)
{
	uint32_t player = 0;

	player = PLAYER_ADDRESS(2);
	PLAYER(player)->lives = 0u;
	PLAYER(player)->unused_setting_copy = 0u;
	PLAYER(player)->score = 0u;
	PLAYER(player)->health_bars = 0u;
	PLAYER(player)->message = 0u;
}

/* the fourth player's record cleared */
void clear_player_4(void)
{
	PLAYER(PLAYER_ADDRESS(3))->lives = 0u;
	PLAYER(PLAYER_ADDRESS(3))->unused_setting_copy = 0u;
	PLAYER(PLAYER_ADDRESS(3))->score = 0u;
	PLAYER(PLAYER_ADDRESS(3))->health_bars = 0u;
	PLAYER(PLAYER_ADDRESS(3))->message = 0u;
}

/* the area's counters cleared */
void clear_area_counters(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	ram_end = spawn_next_ADDRESS;
	count = 0u;
	zero_ram_below(count, ram_end);
}

/* the spawns' and the enemies' variables cleared */
void clear_enemy_variables(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	ram_end = hydrant_sprays_left_ADDRESS;
	count = 0u;
	zero_ram_below(count, ram_end);
}

/* the engine's variables cleared */
void clear_engine_variables(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	ram_end = draw_list_ADDRESS;
	count = 2u;
	zero_ram_below(count, ram_end);
}

/* the engine's variables and the draw list cleared */
void clear_engine_ram(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	ram_end = floor_map_ADDRESS;
	count = 7u;
	zero_ram_below(count, ram_end);
}

/* a stretch of work RAM nothing uses cleared */
void clear_spare_ram(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	ram_end = 0x63200u;
	count = 0u;
	zero_ram_below(count, ram_end);
}

/* every object cleared */
void clear_objects(void)
{
	uint16_t count = 0;
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, objects_end = 0;
	uint16_t t9 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;

	objects_end = PLAYER_SLOT_ADDRESS(0);
	count = 0x7Cu;
	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = objects_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		objects_end = t8;
		t9 = count - 1u;
		count = t9;
	} while (t9 != 0xFFFFu);
}

/* the sprite chip's sprites cleared */
void clear_sprite_ram(void)
{
	uint16_t count = 0;
	uint32_t ram_end = 0;

	ram_end = 0x140800u;
	count = 3u;
	zero_ram_below(count, ram_end);
}

/* Unused: the 384 bytes of work RAM below the first object zeroed */
void unreached_004620(uint16_t d0)
{
	uint16_t count = 0;
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, ram_end = 0;
	uint16_t t25 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0;

	ram_end = OBJECT_ADDRESS(0);
	count = (uint16_t)((d0 & ~0xFFu) | 3u);
	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = ram_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		t9 = t8 - 4u;
		mem_write16(t9 + 2u, fill_8);
		mem_write16(t9, fill_8 >> 16);
		t10 = t9 - 4u;
		mem_write16(t10 + 2u, fill_7);
		mem_write16(t10, fill_7 >> 16);
		t11 = t10 - 4u;
		mem_write16(t11 + 2u, fill_6);
		mem_write16(t11, fill_6 >> 16);
		t12 = t11 - 4u;
		mem_write16(t12 + 2u, fill_5);
		mem_write16(t12, fill_5 >> 16);
		t13 = t12 - 4u;
		mem_write16(t13 + 2u, fill_4);
		mem_write16(t13, fill_4 >> 16);
		t14 = t13 - 4u;
		mem_write16(t14 + 2u, fill_3);
		mem_write16(t14, fill_3 >> 16);
		t15 = t14 - 4u;
		mem_write16(t15 + 2u, fill_2);
		mem_write16(t15, fill_2 >> 16);
		t16 = t15 - 4u;
		mem_write16(t16 + 2u, fill);
		mem_write16(t16, fill >> 16);
		t17 = t16 - 4u;
		mem_write16(t17 + 2u, fill_8);
		mem_write16(t17, fill_8 >> 16);
		t18 = t17 - 4u;
		mem_write16(t18 + 2u, fill_7);
		mem_write16(t18, fill_7 >> 16);
		t19 = t18 - 4u;
		mem_write16(t19 + 2u, fill_6);
		mem_write16(t19, fill_6 >> 16);
		t20 = t19 - 4u;
		mem_write16(t20 + 2u, fill_5);
		mem_write16(t20, fill_5 >> 16);
		t21 = t20 - 4u;
		mem_write16(t21 + 2u, fill_4);
		mem_write16(t21, fill_4 >> 16);
		t22 = t21 - 4u;
		mem_write16(t22 + 2u, fill_3);
		mem_write16(t22, fill_3 >> 16);
		t23 = t22 - 4u;
		mem_write16(t23 + 2u, fill_2);
		mem_write16(t23, fill_2 >> 16);
		t24 = t23 - 4u;
		mem_write16(t24 + 2u, fill);
		mem_write16(t24, fill >> 16);
		ram_end = t24;
		t25 = count - 1u;
		count = t25;
	} while (t25 != 0xFFFFu);
}

/* Unused: the 1152 bytes of work RAM below object 12 zeroed */
void unreached_00462E(uint16_t d0)
{
	uint16_t count = 0;
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, ram_end = 0;
	uint16_t t25 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0;

	ram_end = OBJECT_ADDRESS(12);
	count = (uint16_t)((d0 & ~0xFFu) | 0xBu);
	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = ram_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		t9 = t8 - 4u;
		mem_write16(t9 + 2u, fill_8);
		mem_write16(t9, fill_8 >> 16);
		t10 = t9 - 4u;
		mem_write16(t10 + 2u, fill_7);
		mem_write16(t10, fill_7 >> 16);
		t11 = t10 - 4u;
		mem_write16(t11 + 2u, fill_6);
		mem_write16(t11, fill_6 >> 16);
		t12 = t11 - 4u;
		mem_write16(t12 + 2u, fill_5);
		mem_write16(t12, fill_5 >> 16);
		t13 = t12 - 4u;
		mem_write16(t13 + 2u, fill_4);
		mem_write16(t13, fill_4 >> 16);
		t14 = t13 - 4u;
		mem_write16(t14 + 2u, fill_3);
		mem_write16(t14, fill_3 >> 16);
		t15 = t14 - 4u;
		mem_write16(t15 + 2u, fill_2);
		mem_write16(t15, fill_2 >> 16);
		t16 = t15 - 4u;
		mem_write16(t16 + 2u, fill);
		mem_write16(t16, fill >> 16);
		t17 = t16 - 4u;
		mem_write16(t17 + 2u, fill_8);
		mem_write16(t17, fill_8 >> 16);
		t18 = t17 - 4u;
		mem_write16(t18 + 2u, fill_7);
		mem_write16(t18, fill_7 >> 16);
		t19 = t18 - 4u;
		mem_write16(t19 + 2u, fill_6);
		mem_write16(t19, fill_6 >> 16);
		t20 = t19 - 4u;
		mem_write16(t20 + 2u, fill_5);
		mem_write16(t20, fill_5 >> 16);
		t21 = t20 - 4u;
		mem_write16(t21 + 2u, fill_4);
		mem_write16(t21, fill_4 >> 16);
		t22 = t21 - 4u;
		mem_write16(t22 + 2u, fill_3);
		mem_write16(t22, fill_3 >> 16);
		t23 = t22 - 4u;
		mem_write16(t23 + 2u, fill_2);
		mem_write16(t23, fill_2 >> 16);
		t24 = t23 - 4u;
		mem_write16(t24 + 2u, fill);
		mem_write16(t24, fill >> 16);
		ram_end = t24;
		t25 = count - 1u;
		count = t25;
	} while (t25 != 0xFFFFu);
}

/* Unused: the 768 bytes of work RAM below object 24 zeroed */
void unreached_00463C(uint16_t d0)
{
	uint16_t count = 0;
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, ram_end = 0;
	uint16_t t25 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0;

	ram_end = OBJECT_ADDRESS(24);
	count = (uint16_t)((d0 & ~0xFFu) | 7u);
	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = ram_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		t9 = t8 - 4u;
		mem_write16(t9 + 2u, fill_8);
		mem_write16(t9, fill_8 >> 16);
		t10 = t9 - 4u;
		mem_write16(t10 + 2u, fill_7);
		mem_write16(t10, fill_7 >> 16);
		t11 = t10 - 4u;
		mem_write16(t11 + 2u, fill_6);
		mem_write16(t11, fill_6 >> 16);
		t12 = t11 - 4u;
		mem_write16(t12 + 2u, fill_5);
		mem_write16(t12, fill_5 >> 16);
		t13 = t12 - 4u;
		mem_write16(t13 + 2u, fill_4);
		mem_write16(t13, fill_4 >> 16);
		t14 = t13 - 4u;
		mem_write16(t14 + 2u, fill_3);
		mem_write16(t14, fill_3 >> 16);
		t15 = t14 - 4u;
		mem_write16(t15 + 2u, fill_2);
		mem_write16(t15, fill_2 >> 16);
		t16 = t15 - 4u;
		mem_write16(t16 + 2u, fill);
		mem_write16(t16, fill >> 16);
		t17 = t16 - 4u;
		mem_write16(t17 + 2u, fill_8);
		mem_write16(t17, fill_8 >> 16);
		t18 = t17 - 4u;
		mem_write16(t18 + 2u, fill_7);
		mem_write16(t18, fill_7 >> 16);
		t19 = t18 - 4u;
		mem_write16(t19 + 2u, fill_6);
		mem_write16(t19, fill_6 >> 16);
		t20 = t19 - 4u;
		mem_write16(t20 + 2u, fill_5);
		mem_write16(t20, fill_5 >> 16);
		t21 = t20 - 4u;
		mem_write16(t21 + 2u, fill_4);
		mem_write16(t21, fill_4 >> 16);
		t22 = t21 - 4u;
		mem_write16(t22 + 2u, fill_3);
		mem_write16(t22, fill_3 >> 16);
		t23 = t22 - 4u;
		mem_write16(t23 + 2u, fill_2);
		mem_write16(t23, fill_2 >> 16);
		t24 = t23 - 4u;
		mem_write16(t24 + 2u, fill);
		mem_write16(t24, fill >> 16);
		ram_end = t24;
		t25 = count - 1u;
		count = t25;
	} while (t25 != 0xFFFFu);
}

/* Unused: the 768 bytes of work RAM below object 4 zeroed, the turtles' objects among them */
void unreached_00464A(uint16_t d0)
{
	uint16_t count = 0;
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, ram_end = 0;
	uint16_t t25 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0;

	ram_end = OBJECT_ADDRESS(4);
	count = (uint16_t)((d0 & ~0xFFu) | 7u);
	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = ram_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		t9 = t8 - 4u;
		mem_write16(t9 + 2u, fill_8);
		mem_write16(t9, fill_8 >> 16);
		t10 = t9 - 4u;
		mem_write16(t10 + 2u, fill_7);
		mem_write16(t10, fill_7 >> 16);
		t11 = t10 - 4u;
		mem_write16(t11 + 2u, fill_6);
		mem_write16(t11, fill_6 >> 16);
		t12 = t11 - 4u;
		mem_write16(t12 + 2u, fill_5);
		mem_write16(t12, fill_5 >> 16);
		t13 = t12 - 4u;
		mem_write16(t13 + 2u, fill_4);
		mem_write16(t13, fill_4 >> 16);
		t14 = t13 - 4u;
		mem_write16(t14 + 2u, fill_3);
		mem_write16(t14, fill_3 >> 16);
		t15 = t14 - 4u;
		mem_write16(t15 + 2u, fill_2);
		mem_write16(t15, fill_2 >> 16);
		t16 = t15 - 4u;
		mem_write16(t16 + 2u, fill);
		mem_write16(t16, fill >> 16);
		t17 = t16 - 4u;
		mem_write16(t17 + 2u, fill_8);
		mem_write16(t17, fill_8 >> 16);
		t18 = t17 - 4u;
		mem_write16(t18 + 2u, fill_7);
		mem_write16(t18, fill_7 >> 16);
		t19 = t18 - 4u;
		mem_write16(t19 + 2u, fill_6);
		mem_write16(t19, fill_6 >> 16);
		t20 = t19 - 4u;
		mem_write16(t20 + 2u, fill_5);
		mem_write16(t20, fill_5 >> 16);
		t21 = t20 - 4u;
		mem_write16(t21 + 2u, fill_4);
		mem_write16(t21, fill_4 >> 16);
		t22 = t21 - 4u;
		mem_write16(t22 + 2u, fill_3);
		mem_write16(t22, fill_3 >> 16);
		t23 = t22 - 4u;
		mem_write16(t23 + 2u, fill_2);
		mem_write16(t23, fill_2 >> 16);
		t24 = t23 - 4u;
		mem_write16(t24 + 2u, fill);
		mem_write16(t24, fill >> 16);
		ram_end = t24;
		t25 = count - 1u;
		count = t25;
	} while (t25 != 0xFFFFu);
}

/* Unused: the 768 bytes of work RAM below object 32 zeroed */
void unreached_004658(uint16_t d0)
{
	uint16_t count = 0;
	uint32_t fill = 0, fill_2 = 0, fill_3 = 0, fill_4 = 0, fill_5 = 0, fill_6 = 0, fill_7 = 0, fill_8 = 0, ram_end = 0;
	uint16_t t25 = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0, t9 = 0, t10 = 0, t11 = 0, t12 = 0, t13 = 0, t14 = 0, t15 = 0, t16 = 0, t17 = 0, t18 = 0, t19 = 0, t20 = 0, t21 = 0, t22 = 0, t23 = 0, t24 = 0;

	ram_end = OBJECT_ADDRESS(32);
	count = (uint16_t)((d0 & ~0xFFu) | 7u);
	zero_fill_registers(&fill, &fill_2, &fill_3, &fill_4, &fill_5, &fill_6, &fill_7, &fill_8);
	do
	{
		t1 = ram_end - 4u;
		mem_write16(t1 + 2u, fill_8);
		mem_write16(t1, fill_8 >> 16);
		t2 = t1 - 4u;
		mem_write16(t2 + 2u, fill_7);
		mem_write16(t2, fill_7 >> 16);
		t3 = t2 - 4u;
		mem_write16(t3 + 2u, fill_6);
		mem_write16(t3, fill_6 >> 16);
		t4 = t3 - 4u;
		mem_write16(t4 + 2u, fill_5);
		mem_write16(t4, fill_5 >> 16);
		t5 = t4 - 4u;
		mem_write16(t5 + 2u, fill_4);
		mem_write16(t5, fill_4 >> 16);
		t6 = t5 - 4u;
		mem_write16(t6 + 2u, fill_3);
		mem_write16(t6, fill_3 >> 16);
		t7 = t6 - 4u;
		mem_write16(t7 + 2u, fill_2);
		mem_write16(t7, fill_2 >> 16);
		t8 = t7 - 4u;
		mem_write16(t8 + 2u, fill);
		mem_write16(t8, fill >> 16);
		t9 = t8 - 4u;
		mem_write16(t9 + 2u, fill_8);
		mem_write16(t9, fill_8 >> 16);
		t10 = t9 - 4u;
		mem_write16(t10 + 2u, fill_7);
		mem_write16(t10, fill_7 >> 16);
		t11 = t10 - 4u;
		mem_write16(t11 + 2u, fill_6);
		mem_write16(t11, fill_6 >> 16);
		t12 = t11 - 4u;
		mem_write16(t12 + 2u, fill_5);
		mem_write16(t12, fill_5 >> 16);
		t13 = t12 - 4u;
		mem_write16(t13 + 2u, fill_4);
		mem_write16(t13, fill_4 >> 16);
		t14 = t13 - 4u;
		mem_write16(t14 + 2u, fill_3);
		mem_write16(t14, fill_3 >> 16);
		t15 = t14 - 4u;
		mem_write16(t15 + 2u, fill_2);
		mem_write16(t15, fill_2 >> 16);
		t16 = t15 - 4u;
		mem_write16(t16 + 2u, fill);
		mem_write16(t16, fill >> 16);
		t17 = t16 - 4u;
		mem_write16(t17 + 2u, fill_8);
		mem_write16(t17, fill_8 >> 16);
		t18 = t17 - 4u;
		mem_write16(t18 + 2u, fill_7);
		mem_write16(t18, fill_7 >> 16);
		t19 = t18 - 4u;
		mem_write16(t19 + 2u, fill_6);
		mem_write16(t19, fill_6 >> 16);
		t20 = t19 - 4u;
		mem_write16(t20 + 2u, fill_5);
		mem_write16(t20, fill_5 >> 16);
		t21 = t20 - 4u;
		mem_write16(t21 + 2u, fill_4);
		mem_write16(t21, fill_4 >> 16);
		t22 = t21 - 4u;
		mem_write16(t22 + 2u, fill_3);
		mem_write16(t22, fill_3 >> 16);
		t23 = t22 - 4u;
		mem_write16(t23 + 2u, fill_2);
		mem_write16(t23, fill_2 >> 16);
		t24 = t23 - 4u;
		mem_write16(t24 + 2u, fill);
		mem_write16(t24, fill >> 16);
		ram_end = t24;
		t25 = count - 1u;
		count = t25;
	} while (t25 != 0xFFFFu);
}
