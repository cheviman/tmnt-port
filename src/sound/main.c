/**
 *    Name:        main.c
 *    Description: the program's frame: sound_main, the main loop, the channels' handlers and their stop
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* The program's start: clears the RAM, sets up the K007232 and the YM2151 (no LFO), the random numbers
   and each channel's chip channel, and starts the frame's timer. */
void sound_main(void)
{
	uint32_t carry = 0;
	uint8_t cleared = 0;
	uint16_t fm_write = 0, place = 0;

	z80_block(17);
	cleared = 0u;
	place = RAM_START;
	do
	{
		z80_block(34);
		z80_write8(place, cleared);
		place++;
	} while (place >> 8 != 0x88u);
	z80_block(104);
	z80_write8(pcm_loop_flags, 0u);
	pcm_loops = 0u;
	z80_write8(pcm_volume_port, 0u);
	pcm_volumes = 0u;
	music_pcm_hold = 0u;
	reset_sound();
	z80_block(27);
	fm_write = 0x1900u;
	carry = write_fm_register(fm_write);
	z80_block(27);
	fm_write = 0x1980u;
	carry = write_fm_register(fm_write);
	z80_block(411);
	random_seed = 0x305u;
	random_source = 0u;
	random_write = random_pool_ADDRESS;
	random_read = random_pool_ADDRESS;
	CHANNEL(RAM_START)->chip_channel = 0u;
	CHANNEL(MUSIC_FM_1)->chip_channel = 1u;
	CHANNEL(MUSIC_FM_2)->chip_channel = 2u;
	CHANNEL(MUSIC_FM_3)->chip_channel = 3u;
	CHANNEL(MUSIC_FM_4)->chip_channel = 4u;
	CHANNEL(MUSIC_FM_5)->chip_channel = 5u;
	CHANNEL(MUSIC_FM_6)->chip_channel = 6u;
	CHANNEL(MUSIC_FM_7)->chip_channel = 7u;
	CHANNEL(MUSIC_PCM_A)->chip_channel = 8u;
	CHANNEL(MUSIC_PCM_B)->chip_channel = 9u;
	CHANNEL(PCM_EFFECT_A)->chip_channel = 0xAu;
	CHANNEL(PCM_EFFECT_B)->chip_channel = 0xBu;
	CHANNEL(SPEECH)->chip_channel = 0xCu;
	CHANNEL(FM_EFFECT_6)->chip_channel = 6u;
	CHANNEL(FM_EFFECT_7)->chip_channel = 7u;
	restart_frame_timer();
}

/* A pass of the main loop: random bytes while it waits for the YM2151's timer, then a step of the
   music's fade-out, every channel's handler and the title music's timer. */
void sound_main_loop(void)
{
	uint32_t carry = 0;
	uint8_t random_byte = 0;
	uint16_t channel = 0, old_seed = 0, pool_place = 0, seed = 0;
	uint16_t saved_seed = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(8);
	interrupts_on();
	z80_block(16);
	interrupts_off();
	for (;;)
	{
		z80_block(200);
		seed = random_seed;
		saved_seed = seed;
		old_seed = saved_seed;
		seed = seed * 2u + old_seed;
		random_byte = old_seed + (seed >> 8);
		random_seed = (seed & 0xFFu) | ((uint32_t)random_byte << 8);
		pool_place = random_source;
		random_byte += z80_read8(pool_place);
		random_source = IN_PAGE(pool_place, (uint8_t)(pool_place + 1u));
		pool_place = random_write;
		z80_write8(pool_place, random_byte);
		random_write = IN_PAGE(pool_place, (uint8_t)(pool_place + 1u));
		t1 = z80_read8(fm_data);
		if ((t1 & 2u) != 0u)
			break;
		z80_cycles(5);
	}
	z80_block(17);
	restart_frame_timer();
	z80_block(31);
	carry = 0u;
	if (fade_timer != 0u)
	{
		z80_block(21);
		t2 = fade_timer;
		fade_timer = t2 - 1u;
		if (t2 == 1u)
		{
			z80_block(71);
			fade_timer = 0x16u;
			music_attenuation_changed = 1u;
			fade_step++;
			if ((fade_step & 3u) == 0u)
			{
				z80_block(30);
				fade_attenuation++;
			}
			z80_block(24);
			t3 = fade_step;
			carry = t3 > 0x30u;
			if (0x30u == t3)
			{
				z80_block(60);
				fade_timer = 0u;
				fade_step = 0u;
				fade_attenuation = 0u;
				free_music_pcm_a();
				z80_block(17);
				free_music_pcm_b();
				z80_block(27);
				if (CHANNEL(PCM_EFFECT_A)->sound == 0u)
				{
					z80_block(27);
					if (CHANNEL(PCM_EFFECT_B)->sound == 0u)
					{
						z80_block(17);
						stop_music_pcm_b();
					}
					z80_block(17);
					stop_music_pcm_a();
				}
				z80_block(17);
				free_music_fm_6_7();
				z80_block(27);
				carry = 0u;
				if (fm_effect_34_on == 0u)
				{
					z80_block(17);
					stop_music_fm_6_7();
				}
				z80_block(17);
				stop_music_fm_1_5();
			}
		}
	}
	z80_block(31);
	channel = FM_EFFECT_0;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_1;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_2;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_3;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_4;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_5;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_6;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_FM_7;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_PCM_A;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = MUSIC_PCM_B;
	carry = run_channel_handler(channel, carry);
	z80_block(48);
	carry = 0u;
	music_attenuation_changed = 0u;
	channel = PCM_EFFECT_B;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = PCM_EFFECT_A;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = SPEECH;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = FM_EFFECT_6;
	carry = run_channel_handler(channel, carry);
	z80_block(31);
	channel = FM_EFFECT_7;
	carry = run_channel_handler(channel, carry);
	z80_block(27);
	if (title_music_on != 0u)
	{
		z80_cycles(7);
		title_music_timer();
	}
	z80_block(10);
}

/* Runs the channel through the handler its record keeps. */
uint32_t run_channel_handler(uint16_t channel, uint32_t carry)
{
	uint32_t jump_target = 0;

	z80_block(42);
	jump_target = CHANNEL(channel)->handler;
	switch (jump_target)
	{
	case speech_play_ADDRESS:
		return speech_play(channel);
	case speech_wait_ADDRESS:
		return speech_wait(channel, carry);
	case stop_channel_ADDRESS:
		return stop_channel(channel);
	case free_channel_ADDRESS:
		free_channel(channel);
		return carry;
	case channel_idle_ADDRESS:
		channel_idle();
		return carry;
	case stop_pcm_channel_ADDRESS:
		return stop_pcm_channel(channel);
	case music_track_start_ADDRESS:
		return music_track_start(channel);
	case music_track_first_tick_ADDRESS:
		return music_track_first_tick(channel);
	case music_track_tick_ADDRESS:
		return music_track_tick(channel);
	case pcm_effect_countdown_ADDRESS:
		return pcm_effect_countdown(channel);
	case pcm_effect_81_ADDRESS:
		return pcm_effect_81(channel);
	case pcm_effect_81_next_ADDRESS:
		return pcm_effect_81_next(channel);
	case pcm_effect_81_echo_ADDRESS:
		pcm_effect_81_echo(channel);
		return carry;
	case pcm_effect_81_echo_wait_ADDRESS:
		return pcm_effect_81_echo_wait(channel, carry);
	case pcm_effect_82_ADDRESS:
		return pcm_effect_82(channel);
	case pcm_effect_ducking_fade_ADDRESS:
		return pcm_effect_ducking_fade(channel);
	case pcm_effect_83_ADDRESS:
		return pcm_effect_83(channel);
	case pcm_effect_84_ADDRESS:
		return pcm_effect_84(channel);
	case pcm_effect_85_ADDRESS:
		return pcm_effect_85(channel);
	case pcm_effect_86_ADDRESS:
		return pcm_effect_86(channel);
	case pcm_effect_86_countdown_ADDRESS:
		return pcm_effect_86_countdown(channel);
	case pcm_effect_8d_ADDRESS:
		pcm_effect_8d(channel);
		return carry;
	case pcm_effect_8d_wait_ADDRESS:
		return pcm_effect_8d_wait(channel, carry);
	case pcm_effect_87_ADDRESS:
		return pcm_effect_87(channel);
	case pcm_effect_88_ADDRESS:
		return pcm_effect_88(channel);
	case pcm_effect_88_play_ADDRESS:
		return pcm_effect_88_play(channel);
	case pcm_effect_88_echo_ADDRESS:
		pcm_effect_88_echo(channel);
		return carry;
	case pcm_effect_88_echo_wait_ADDRESS:
		return pcm_effect_88_echo_wait(channel, carry);
	case pcm_effect_89_ADDRESS:
		return pcm_effect_89(channel);
	case pcm_effect_89_next_ADDRESS:
		return pcm_effect_89_next(channel, carry);
	case pcm_effect_8a_ADDRESS:
		return pcm_effect_8a(channel);
	case pcm_effect_8a_next_ADDRESS:
		return pcm_effect_8a_next(channel, carry);
	case pcm_effect_8b_ADDRESS:
		return pcm_effect_8b(channel);
	case pcm_effect_8b_next_ADDRESS:
		return pcm_effect_8b_next(channel, carry);
	case pcm_effect_8b_echo_ADDRESS:
		pcm_effect_8b_echo(channel);
		return carry;
	case pcm_effect_8b_echo_wait_ADDRESS:
		return pcm_effect_8b_echo_wait(channel, carry);
	case pcm_effect_8c_ADDRESS:
		return pcm_effect_8c(channel);
	case pcm_effect_8e_ADDRESS:
		return pcm_effect_8e(channel);
	case pcm_effect_8f_ADDRESS:
		return pcm_effect_8f(channel);
	case pcm_effect_90_ADDRESS:
		return pcm_effect_90(channel);
	case pcm_effect_91_ADDRESS:
		return pcm_effect_91(channel);
	case pcm_effect_92_ADDRESS:
		return pcm_effect_92(channel);
	case pcm_effect_93_ADDRESS:
		return pcm_effect_93(channel);
	case fm_effect_01_ADDRESS:
		return fm_effect_01(channel);
	case fm_effect_01_hold_ADDRESS:
		return fm_effect_01_hold(channel);
	case fm_effect_02_ADDRESS:
		return fm_effect_02(channel);
	case fm_effect_02_slide_ADDRESS:
		return fm_effect_02_slide(channel);
	case fm_effect_02_pause_ADDRESS:
		return fm_effect_02_pause(channel, carry);
	case fm_effect_03_ADDRESS:
		return fm_effect_03(channel);
	case fm_effect_03_fall_ADDRESS:
		return fm_effect_03_fall(channel);
	case fm_effect_03_trill_ADDRESS:
		return fm_effect_03_trill(channel);
	case fm_effect_04_ADDRESS:
		return fm_effect_04(channel);
	case fm_effect_05_ADDRESS:
		return fm_effect_05(channel);
	case fm_effect_wobble_down_ADDRESS:
		return fm_effect_wobble_down(channel);
	case fm_effect_wobble_hold_ADDRESS:
		return fm_effect_wobble_hold(channel, carry);
	case fm_effect_37_ADDRESS:
		fm_effect_37(channel);
		return carry;
	case fm_effect_37_wait_ADDRESS:
		return fm_effect_37_wait(channel, carry);
	case fm_effect_06_ADDRESS:
		return fm_effect_06(channel);
	case fm_effect_06_fall_ADDRESS:
		return fm_effect_06_fall(channel);
	case fm_effect_06_trill_ADDRESS:
		return fm_effect_06_trill(channel);
	case fm_effect_07_ADDRESS:
		return fm_effect_07(channel);
	case fm_effect_07_step_ADDRESS:
		return fm_effect_07_step(channel);
	case fm_effect_08_ADDRESS:
		return fm_effect_08(channel);
	case fm_effect_08_rise_and_warble_ADDRESS:
		return fm_effect_08_rise_and_warble(channel);
	case fm_effect_09_ADDRESS:
		return fm_effect_09(channel);
	case fm_effect_0a_ADDRESS:
		return fm_effect_0a(channel);
	case fm_effect_0a_rise_ADDRESS:
		return fm_effect_0a_rise(channel);
	case fm_effect_33_hold_ADDRESS:
		return fm_effect_33_hold(channel);
	case fm_effect_0b_ADDRESS:
		return fm_effect_0b(channel);
	case fm_effect_0b_hold_ADDRESS:
		return fm_effect_0b_hold(channel);
	case fm_effect_0b_slide_ADDRESS:
		return fm_effect_0b_slide(channel);
	case fm_effect_0c_ADDRESS:
		return fm_effect_0c(channel);
	case fm_effect_zigzag_steps_ADDRESS:
		return fm_effect_zigzag_steps(channel);
	case fm_effect_0d_ADDRESS:
		return fm_effect_0d(channel);
	case fm_effect_0d_fall_ADDRESS:
		return fm_effect_0d_fall(channel);
	case fm_effect_0d_trill_ADDRESS:
		return fm_effect_0d_trill(channel);
	case fm_effect_0d_pause_ADDRESS:
		return fm_effect_0d_pause(channel, carry);
	case fm_effect_0e_ADDRESS:
		return fm_effect_0e(channel);
	case fm_effect_0e_zigzag_down_ADDRESS:
		return fm_effect_0e_zigzag_down(channel);
	case fm_effect_0e_zigzag_up_ADDRESS:
		return fm_effect_0e_zigzag_up(channel);
	case fm_effect_0f_ADDRESS:
		return fm_effect_0f(channel);
	case fm_effect_0f_drop_ADDRESS:
		return fm_effect_0f_drop(channel);
	case fm_effect_burst_step_ADDRESS:
		return fm_effect_burst_step(channel);
	case fm_effect_10_ADDRESS:
		return fm_effect_10(channel);
	case fm_effect_10_fall_ADDRESS:
		return fm_effect_10_fall(channel);
	case fm_effect_10_trill_ADDRESS:
		return fm_effect_10_trill(channel);
	case fm_effect_11_ADDRESS:
		return fm_effect_11(channel);
	case fm_effect_drop_and_pcm_ADDRESS:
		return fm_effect_drop_and_pcm(channel);
	case fm_effect_12_ADDRESS:
		return fm_effect_12(channel);
	case fm_effect_13_ADDRESS:
		return fm_effect_13(channel);
	case fm_effect_14_ADDRESS:
		return fm_effect_14(channel);
	case fm_effect_14_fall_ADDRESS:
		return fm_effect_14_fall(channel, carry);
	case fm_effect_15_ADDRESS:
		return fm_effect_15(channel);
	case fm_effect_15_sink_ADDRESS:
		return fm_effect_15_sink(channel, carry);
	case fm_effect_15_echo_ADDRESS:
		fm_effect_15_echo(channel);
		return carry;
	case fm_effect_15_echo_wait_ADDRESS:
		return fm_effect_15_echo_wait(channel, carry);
	case fm_effect_16_ADDRESS:
		return fm_effect_16(channel);
	case fm_effect_long_rise_ADDRESS:
		return fm_effect_long_rise(channel);
	case fm_effect_16_echo_ADDRESS:
		return fm_effect_16_echo(channel);
	case fm_effect_16_echo_wait_ADDRESS:
		return fm_effect_16_echo_wait(channel, carry);
	case fm_effect_17_ADDRESS:
		return fm_effect_17(channel);
	case fm_effect_17_zigzag_ADDRESS:
		return fm_effect_17_zigzag(channel);
	case fm_effect_18_ADDRESS:
		return fm_effect_18(channel);
	case fm_effect_19_ADDRESS:
		return fm_effect_19(channel);
	case fm_effect_ladder_ADDRESS:
		return fm_effect_ladder(channel);
	case fm_effect_1a_ADDRESS:
		return fm_effect_1a(channel);
	case fm_effect_1a_fall_ADDRESS:
		return fm_effect_1a_fall(channel);
	case fm_effect_1a_burst_step_ADDRESS:
		return fm_effect_1a_burst_step(channel);
	case fm_effect_1a_second_start_ADDRESS:
		return fm_effect_1a_second_start(channel);
	case fm_effect_1a_second_fall_ADDRESS:
		return fm_effect_1a_second_fall(channel);
	case fm_effect_1a_second_burst_step_ADDRESS:
		return fm_effect_1a_second_burst_step(channel);
	case fm_effect_1b_ADDRESS:
		return fm_effect_1b(channel);
	case fm_effect_1b_rise_ADDRESS:
		return fm_effect_1b_rise(channel);
	case fm_effect_1b_hold_ADDRESS:
		return fm_effect_1b_hold(channel);
	case fm_effect_1c_ADDRESS:
		return fm_effect_1c(channel);
	case fm_effect_pulses_ADDRESS:
		return fm_effect_pulses(channel);
	case fm_effect_1c_echo_ADDRESS:
		fm_effect_1c_echo(channel);
		return carry;
	case fm_effect_1c_echo_wait_ADDRESS:
		return fm_effect_1c_echo_wait(channel, carry);
	case fm_effect_1d_ADDRESS:
		return fm_effect_1d(channel);
	case fm_effect_arpeggio_ADDRESS:
		return fm_effect_arpeggio(channel, carry);
	case fm_effect_arpeggio_ring_ADDRESS:
		return fm_effect_arpeggio_ring(channel, carry);
	case fm_effect_1d_echo_ADDRESS:
		fm_effect_1d_echo(channel);
		return carry;
	case fm_effect_1d_echo_wait_ADDRESS:
		return fm_effect_1d_echo_wait(channel, carry);
	case fm_effect_1e_ADDRESS:
		return fm_effect_1e(channel);
	case fm_effect_1e_wait_ADDRESS:
		return fm_effect_1e_wait(channel, carry);
	case fm_effect_1f_ADDRESS:
		return fm_effect_1f(channel);
	case fm_effect_1f_rise_ADDRESS:
		return fm_effect_1f_rise(channel);
	case fm_effect_1f_hold_ADDRESS:
		return fm_effect_1f_hold(channel, carry);
	case fm_effect_20_ADDRESS:
		return fm_effect_20(channel);
	case fm_effect_21_ADDRESS:
		return fm_effect_21(channel);
	case fm_effect_21_rise_ADDRESS:
		return fm_effect_21_rise(channel);
	case fm_effect_21_hold_ADDRESS:
		return fm_effect_21_hold(channel);
	case fm_effect_22_ADDRESS:
		return fm_effect_22(channel);
	case fm_effect_22_on_channel_7_ADDRESS:
		return fm_effect_22_on_channel_7(channel);
	case fm_effect_22_on_channel_6_ADDRESS:
		return fm_effect_22_on_channel_6(channel);
	case fm_effect_22_on_channel_5_ADDRESS:
		return fm_effect_22_on_channel_5(channel);
	case fm_effect_22_on_channel_4_ADDRESS:
		return fm_effect_22_on_channel_4(channel);
	case fm_effect_22_on_channel_3_ADDRESS:
		return fm_effect_22_on_channel_3(channel);
	case fm_effect_22_on_channel_2_ADDRESS:
		return fm_effect_22_on_channel_2(channel);
	case fm_effect_22_on_channel_1_ADDRESS:
		return fm_effect_22_on_channel_1(channel);
	case fm_effect_22_wait_ADDRESS:
		return fm_effect_22_wait(channel, carry);
	case fm_effect_23_ADDRESS:
		return fm_effect_23(channel);
	case fm_effect_23_fall_ADDRESS:
		return fm_effect_23_fall(channel, carry);
	case fm_effect_24_ADDRESS:
		return fm_effect_24(channel);
	case fm_effect_24_fall_ADDRESS:
		return fm_effect_24_fall(channel);
	case fm_effect_24_burst_step_ADDRESS:
		return fm_effect_24_burst_step(channel);
	case fm_effect_25_ADDRESS:
		return fm_effect_25(channel);
	case fm_effect_25_fall_ADDRESS:
		return fm_effect_25_fall(channel, carry);
	case fm_effect_25_bend_ADDRESS:
		return fm_effect_25_bend(channel);
	case fm_effect_26_ADDRESS:
		return fm_effect_26(channel);
	case fm_effect_26_play_ADDRESS:
		return fm_effect_26_play(channel);
	case fm_effect_27_ADDRESS:
		return fm_effect_27(channel);
	case fm_effect_27_swell_and_warble_ADDRESS:
		return fm_effect_27_swell_and_warble(channel);
	case fm_effect_28_ADDRESS:
		return fm_effect_28(channel);
	case fm_effect_28_jitter_ADDRESS:
		return fm_effect_28_jitter(channel);
	case fm_effect_29_ADDRESS:
		return fm_effect_29(channel);
	case fm_effect_29_fall_ADDRESS:
		return fm_effect_29_fall(channel);
	case fm_effect_29_tick_ADDRESS:
		return fm_effect_29_tick(channel);
	case fm_effect_29_drop_ADDRESS:
		return fm_effect_29_drop(channel);
	case fm_effect_2a_ADDRESS:
		return fm_effect_2a(channel);
	case fm_effect_2a_swell_and_warble_ADDRESS:
		return fm_effect_2a_swell_and_warble(channel);
	case fm_effect_2a_rest_ADDRESS:
		return fm_effect_2a_rest(channel, carry);
	case fm_effect_2b_ADDRESS:
		return fm_effect_2b(channel);
	case fm_effect_2b_drop_ADDRESS:
		return fm_effect_2b_drop(channel);
	case fm_effect_2c_ADDRESS:
		return fm_effect_2c(channel);
	case fm_effect_dive_ADDRESS:
		return fm_effect_dive(channel);
	case fm_effect_dive_climb_ADDRESS:
		return fm_effect_dive_climb(channel, carry);
	case fm_effect_2d_ADDRESS:
		return fm_effect_2d(channel);
	case fm_effect_2d_zigzag_ADDRESS:
		return fm_effect_2d_zigzag(channel, carry);
	case fm_effect_2e_ADDRESS:
		return fm_effect_2e(channel);
	case fm_effect_2e_fall_ADDRESS:
		return fm_effect_2e_fall(channel);
	case fm_effect_2e_trill_ADDRESS:
		return fm_effect_2e_trill(channel, carry);
	case fm_effect_2f_ADDRESS:
		return fm_effect_2f(channel);
	case fm_effect_2f_swell_and_warble_ADDRESS:
		return fm_effect_2f_swell_and_warble(channel);
	case fm_effect_30_ADDRESS:
		return fm_effect_30(channel);
	case fm_effect_30_sweep_ADDRESS:
		return fm_effect_30_sweep(channel);
	case fm_effect_30_slow_sweep_ADDRESS:
		return fm_effect_30_slow_sweep(channel);
	case fm_effect_31_ADDRESS:
		return fm_effect_31(channel);
	case fm_effect_31_fall_ADDRESS:
		return fm_effect_31_fall(channel);
	case fm_effect_31_trill_ADDRESS:
		return fm_effect_31_trill(channel, carry);
	case fm_effect_32_ADDRESS:
		return fm_effect_32(channel);
	case fm_effect_32_bend_ADDRESS:
		return fm_effect_32_bend(channel);
	case fm_effect_33_ADDRESS:
		return fm_effect_33(channel);
	case fm_effect_34_ADDRESS:
		return fm_effect_34(channel);
	case fm_effect_34_fall_ADDRESS:
		return fm_effect_34_fall(channel);
	case fm_effect_34_trill_ADDRESS:
		return fm_effect_34_trill(channel);
	case fm_effect_34_warbles_ADDRESS:
		return fm_effect_34_warbles(channel);
	case fm_effect_34_warble_ADDRESS:
		return fm_effect_34_warble(channel);
	case fm_effect_35_ADDRESS:
		return fm_effect_35(channel);
	case fm_effect_36_ADDRESS:
		return fm_effect_36(channel);
	case fm_effect_36_rise_ADDRESS:
		return fm_effect_36_rise(channel);
	case fm_effect_36_hold_ADDRESS:
		fm_effect_36_hold(channel);
		return carry;
	case fm_effect_36_fall_ADDRESS:
		return fm_effect_36_fall(channel);
	case fm_effect_38_ADDRESS:
		return fm_effect_38(channel);
	case fm_effect_38_trill_ADDRESS:
		return fm_effect_38_trill(channel);
	case fm_effect_39_ADDRESS:
		return fm_effect_39(channel);
	case fm_effect_39_next_note_ADDRESS:
		return fm_effect_39_next_note(channel, carry);
	case fm_effect_3a_ADDRESS:
		return fm_effect_3a(channel);
	case fm_effect_3a_warble_ADDRESS:
		return fm_effect_3a_warble(channel);
	case fm_effect_3b_ADDRESS:
		return fm_effect_3b(channel);
	case fm_effect_3b_swell_and_warble_ADDRESS:
		return fm_effect_3b_swell_and_warble(channel);
	case fm_effect_3c_ADDRESS:
		return fm_effect_3c(channel);
	case fm_effect_3c_warble_ADDRESS:
		return fm_effect_3c_warble(channel);
	case fm_effect_3d_ADDRESS:
		return fm_effect_3d(channel);
	case fm_effect_3d_on_channel_6_ADDRESS:
		return fm_effect_3d_on_channel_6(channel);
	case fm_effect_3d_on_channel_7_ADDRESS:
		return fm_effect_3d_on_channel_7(channel);
	case fm_effect_3d_wait_ADDRESS:
		return fm_effect_3d_wait(channel, carry);
	case fm_effect_3e_ADDRESS:
		return fm_effect_3e(channel);
	case fm_effect_3e_delay_ADDRESS:
		fm_effect_3e_delay(channel);
		return carry;
	case fm_effect_3e_wait_ADDRESS:
		return fm_effect_3e_wait(channel, carry);
	case fm_effect_3f_ADDRESS:
		return fm_effect_3f(channel);
	case fm_effect_fall_and_trills_fall_ADDRESS:
		return fm_effect_fall_and_trills_fall(channel);
	case fm_effect_fall_and_trills_ADDRESS:
		return fm_effect_fall_and_trills(channel, carry);
	case fm_effect_40_ADDRESS:
		return fm_effect_40(channel);
	case fm_effect_41_ADDRESS:
		return fm_effect_41(channel);
	case fm_effect_41_rise_ADDRESS:
		return fm_effect_41_rise(channel, carry);
	default:
		sound_invalid_jump(jump_target);
	}
}

/* Stops an FM channel: keyed off, its release at the fastest, then free. */
uint32_t stop_channel(uint16_t channel)
{
	uint32_t carry = 0;

	z80_block(17);
	fm_key_off(channel);
	z80_block(17);
	carry = fm_fast_release(channel);
	free_channel(channel);
	return carry;
}

/* Frees a channel: no sound, no priority, an idle handler. */
void free_channel(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(48);
	CHANNEL(channel)->sound = 0u;
	CHANNEL(channel)->priority = 0u;
	handler = channel_idle_ADDRESS;
	set_channel_handler(handler, channel);
}

/* The routine that runs the channel from its next pass on. */
void set_channel_handler(uint16_t handler, uint16_t channel)
{
	z80_block(38);
	CHANNEL(channel)->handler = handler;
	channel_idle();
}

/* A free channel's handler: nothing to do. */
void channel_idle(void)
{
	z80_block(10);
}

/* Stops a PCM channel: the music's attenuation back, its volume 0, no loop, then free. */
uint32_t stop_pcm_channel(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t volume = 0;

	z80_block(51);
	music_attenuation = 0u;
	music_attenuation_changed = 0u;
	volume = 0u;
	carry = set_pcm_volume(volume, channel);
	z80_block(17);
	carry = clear_pcm_loop(channel);
	z80_block(10);
	free_channel(channel);
	return carry;
}
