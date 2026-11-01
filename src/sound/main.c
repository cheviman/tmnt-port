/**
 *    Name:        main.c
 *    Description: the program's frame: sound_main, the main loop, the channels' handlers and their stop
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
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
	case 0x000178u:
		return speech_play(channel);
	case 0x0001A4u:
		return speech_wait(channel, carry);
	case 0x0005D0u:
		return stop_channel(channel);
	case 0x0005D6u:
		free_channel(channel);
		return carry;
	case 0x0005E7u:
		channel_idle();
		return carry;
	case 0x0005E8u:
		return stop_pcm_channel(channel);
	case 0x000885u:
		return music_track_start(channel);
	case 0x0008CDu:
		return music_track_first_tick(channel);
	case 0x0008D6u:
		return music_track_tick(channel);
	case 0x0010AFu:
		return pcm_effect_countdown(channel);
	case 0x0010D0u:
		return pcm_effect_81(channel);
	case 0x0010F8u:
		return pcm_effect_81_next(channel);
	case 0x001114u:
		pcm_effect_81_echo(channel);
		return carry;
	case 0x00111Eu:
		return pcm_effect_81_echo_wait(channel, carry);
	case 0x001125u:
		return pcm_effect_82(channel);
	case 0x00114Bu:
		return pcm_effect_ducking_fade(channel);
	case 0x001165u:
		return pcm_effect_83(channel);
	case 0x001183u:
		return pcm_effect_84(channel);
	case 0x0011A1u:
		return pcm_effect_85(channel);
	case 0x0011AEu:
		return pcm_effect_86(channel);
	case 0x0011C8u:
		return pcm_effect_86_countdown(channel);
	case 0x0011F4u:
		pcm_effect_8d(channel);
		return carry;
	case 0x0011FEu:
		return pcm_effect_8d_wait(channel, carry);
	case 0x00120Eu:
		return pcm_effect_87(channel);
	case 0x00122Cu:
		return pcm_effect_88(channel);
	case 0x001232u:
		return pcm_effect_88_play(channel);
	case 0x001250u:
		pcm_effect_88_echo(channel);
		return carry;
	case 0x00125Au:
		return pcm_effect_88_echo_wait(channel, carry);
	case 0x001261u:
		return pcm_effect_89(channel);
	case 0x00128Cu:
		return pcm_effect_89_next(channel, carry);
	case 0x00129Cu:
		return pcm_effect_8a(channel);
	case 0x0012BFu:
		return pcm_effect_8a_next(channel, carry);
	case 0x0012D1u:
		return pcm_effect_8b(channel);
	case 0x0012FCu:
		return pcm_effect_8b_next(channel, carry);
	case 0x001338u:
		pcm_effect_8b_echo(channel);
		return carry;
	case 0x001342u:
		return pcm_effect_8b_echo_wait(channel, carry);
	case 0x00134Cu:
		return pcm_effect_8c(channel);
	case 0x001359u:
		return pcm_effect_8e(channel);
	case 0x00137Du:
		return pcm_effect_8f(channel);
	case 0x0013A3u:
		return pcm_effect_90(channel);
	case 0x0013C1u:
		return pcm_effect_91(channel);
	case 0x0013CEu:
		return pcm_effect_92(channel);
	case 0x0013D4u:
		return pcm_effect_93(channel);
	case 0x001638u:
		return fm_effect_01(channel);
	case 0x001662u:
		return fm_effect_01_hold(channel);
	case 0x00169Au:
		return fm_effect_02(channel);
	case 0x0016CFu:
		return fm_effect_02_slide(channel);
	case 0x001727u:
		return fm_effect_02_pause(channel, carry);
	case 0x00174Fu:
		return fm_effect_03(channel);
	case 0x00177Du:
		return fm_effect_03_fall(channel);
	case 0x0017ADu:
		return fm_effect_03_trill(channel);
	case 0x0017E7u:
		return fm_effect_04(channel);
	case 0x001811u:
		return fm_effect_05(channel);
	case 0x00183Fu:
		return fm_effect_wobble_down(channel);
	case 0x001868u:
		return fm_effect_wobble_hold(channel, carry);
	case 0x001877u:
		fm_effect_37(channel);
		return carry;
	case 0x001881u:
		return fm_effect_37_wait(channel, carry);
	case 0x001888u:
		return fm_effect_06(channel);
	case 0x0018A9u:
		return fm_effect_06_fall(channel);
	case 0x0018D9u:
		return fm_effect_06_trill(channel);
	case 0x00192Cu:
		return fm_effect_07(channel);
	case 0x001962u:
		return fm_effect_07_step(channel);
	case 0x0019D1u:
		return fm_effect_08(channel);
	case 0x0019F9u:
		return fm_effect_08_rise_and_warble(channel);
	case 0x001A23u:
		return fm_effect_09(channel);
	case 0x001A43u:
		return fm_effect_0a(channel);
	case 0x001A98u:
		return fm_effect_0a_rise(channel);
	case 0x001B1Bu:
		return fm_effect_33_hold(channel);
	case 0x001B4Fu:
		return fm_effect_0b(channel);
	case 0x001B79u:
		return fm_effect_0b_hold(channel);
	case 0x001B9Cu:
		return fm_effect_0b_slide(channel);
	case 0x001BDEu:
		return fm_effect_0c(channel);
	case 0x001C40u:
		return fm_effect_zigzag_steps(channel);
	case 0x001CA0u:
		return fm_effect_0d(channel);
	case 0x001CCEu:
		return fm_effect_0d_fall(channel);
	case 0x001D0Au:
		return fm_effect_0d_trill(channel);
	case 0x001D4Fu:
		return fm_effect_0d_pause(channel, carry);
	case 0x001D74u:
		return fm_effect_0e(channel);
	case 0x001D96u:
		return fm_effect_0e_zigzag_down(channel);
	case 0x001DE4u:
		return fm_effect_0e_zigzag_up(channel);
	case 0x001E33u:
		return fm_effect_0f(channel);
	case 0x001E4Cu:
		return fm_effect_0f_drop(channel);
	case 0x001EB5u:
		return fm_effect_burst_step(channel);
	case 0x001F07u:
		return fm_effect_10(channel);
	case 0x001F37u:
		return fm_effect_10_fall(channel);
	case 0x001F7Fu:
		return fm_effect_10_trill(channel);
	case 0x001FCBu:
		return fm_effect_11(channel);
	case 0x001FE4u:
		return fm_effect_drop_and_pcm(channel);
	case 0x001FF6u:
		return fm_effect_12(channel);
	case 0x002012u:
		return fm_effect_13(channel);
	case 0x00203Eu:
		return fm_effect_14(channel);
	case 0x002069u:
		return fm_effect_14_fall(channel, carry);
	case 0x0020ADu:
		return fm_effect_15(channel);
	case 0x0020F5u:
		return fm_effect_15_sink(channel, carry);
	case 0x00212Cu:
		fm_effect_15_echo(channel);
		return carry;
	case 0x002136u:
		return fm_effect_15_echo_wait(channel, carry);
	case 0x00215Au:
		return fm_effect_16(channel);
	case 0x002197u:
		return fm_effect_long_rise(channel);
	case 0x0021C6u:
		return fm_effect_16_echo(channel);
	case 0x0021D6u:
		return fm_effect_16_echo_wait(channel, carry);
	case 0x0021F6u:
		return fm_effect_17(channel);
	case 0x002226u:
		return fm_effect_17_zigzag(channel);
	case 0x002299u:
		return fm_effect_18(channel);
	case 0x0022D2u:
		return fm_effect_19(channel);
	case 0x002306u:
		return fm_effect_ladder(channel);
	case 0x00234Eu:
		return fm_effect_1a(channel);
	case 0x002393u:
		return fm_effect_1a_fall(channel);
	case 0x0023CAu:
		return fm_effect_1a_burst_step(channel);
	case 0x00244Fu:
		return fm_effect_1a_second_start(channel);
	case 0x00247Au:
		return fm_effect_1a_second_fall(channel);
	case 0x0024B7u:
		return fm_effect_1a_second_burst_step(channel);
	case 0x00250Fu:
		return fm_effect_1b(channel);
	case 0x002531u:
		return fm_effect_1b_rise(channel);
	case 0x002556u:
		return fm_effect_1b_hold(channel);
	case 0x00257Fu:
		return fm_effect_1c(channel);
	case 0x0025B8u:
		return fm_effect_pulses(channel);
	case 0x0025FEu:
		fm_effect_1c_echo(channel);
		return carry;
	case 0x002608u:
		return fm_effect_1c_echo_wait(channel, carry);
	case 0x00262Au:
		return fm_effect_1d(channel);
	case 0x00267Au:
		return fm_effect_arpeggio(channel, carry);
	case 0x0026A5u:
		return fm_effect_arpeggio_ring(channel, carry);
	case 0x0026ACu:
		fm_effect_1d_echo(channel);
		return carry;
	case 0x0026B6u:
		return fm_effect_1d_echo_wait(channel, carry);
	case 0x0026C4u:
		return fm_effect_1e(channel);
	case 0x0026D4u:
		return fm_effect_1e_wait(channel, carry);
	case 0x0026F6u:
		return fm_effect_1f(channel);
	case 0x002713u:
		return fm_effect_1f_rise(channel);
	case 0x00274Du:
		return fm_effect_1f_hold(channel, carry);
	case 0x002767u:
		return fm_effect_20(channel);
	case 0x002785u:
		return fm_effect_21(channel);
	case 0x0027A7u:
		return fm_effect_21_rise(channel);
	case 0x0027DDu:
		return fm_effect_21_hold(channel);
	case 0x002804u:
		return fm_effect_22(channel);
	case 0x002857u:
		return fm_effect_22_on_channel_7(channel);
	case 0x00285Cu:
		return fm_effect_22_on_channel_6(channel);
	case 0x002861u:
		return fm_effect_22_on_channel_5(channel);
	case 0x002866u:
		return fm_effect_22_on_channel_4(channel);
	case 0x00286Bu:
		return fm_effect_22_on_channel_3(channel);
	case 0x002870u:
		return fm_effect_22_on_channel_2(channel);
	case 0x002875u:
		return fm_effect_22_on_channel_1(channel);
	case 0x00288Eu:
		return fm_effect_22_wait(channel, carry);
	case 0x0028B6u:
		return fm_effect_23(channel);
	case 0x0028E1u:
		return fm_effect_23_fall(channel, carry);
	case 0x002925u:
		return fm_effect_24(channel);
	case 0x002958u:
		return fm_effect_24_fall(channel);
	case 0x002998u:
		return fm_effect_24_burst_step(channel);
	case 0x0029EBu:
		return fm_effect_25(channel);
	case 0x002A0Du:
		return fm_effect_25_fall(channel, carry);
	case 0x002A33u:
		return fm_effect_25_bend(channel);
	case 0x002A68u:
		return fm_effect_26(channel);
	case 0x002AABu:
		return fm_effect_26_play(channel);
	case 0x002B2Du:
		return fm_effect_27(channel);
	case 0x002B52u:
		return fm_effect_27_swell_and_warble(channel);
	case 0x002BDFu:
		return fm_effect_28(channel);
	case 0x002C09u:
		return fm_effect_28_jitter(channel);
	case 0x002C49u:
		return fm_effect_29(channel);
	case 0x002C60u:
		return fm_effect_29_fall(channel);
	case 0x002C8Du:
		return fm_effect_29_tick(channel);
	case 0x002CBAu:
		return fm_effect_29_drop(channel);
	case 0x002CF5u:
		return fm_effect_2a(channel);
	case 0x002D1Du:
		return fm_effect_2a_swell_and_warble(channel);
	case 0x002D40u:
		return fm_effect_2a_rest(channel, carry);
	case 0x002D7Du:
		return fm_effect_2b(channel);
	case 0x002D9Du:
		return fm_effect_2b_drop(channel);
	case 0x002DBFu:
		return fm_effect_2c(channel);
	case 0x002DF6u:
		return fm_effect_dive(channel);
	case 0x002E35u:
		return fm_effect_dive_climb(channel, carry);
	case 0x002E77u:
		return fm_effect_2d(channel);
	case 0x002EAAu:
		return fm_effect_2d_zigzag(channel, carry);
	case 0x002F11u:
		return fm_effect_2e(channel);
	case 0x002F3Cu:
		return fm_effect_2e_fall(channel);
	case 0x002F76u:
		return fm_effect_2e_trill(channel, carry);
	case 0x002FA8u:
		return fm_effect_2f(channel);
	case 0x002FD0u:
		return fm_effect_2f_swell_and_warble(channel);
	case 0x003032u:
		return fm_effect_30(channel);
	case 0x003042u:
		return fm_effect_30_sweep(channel);
	case 0x00306Fu:
		return fm_effect_30_slow_sweep(channel);
	case 0x0030A4u:
		return fm_effect_31(channel);
	case 0x0030CFu:
		return fm_effect_31_fall(channel);
	case 0x003109u:
		return fm_effect_31_trill(channel, carry);
	case 0x003154u:
		return fm_effect_32(channel);
	case 0x003181u:
		return fm_effect_32_bend(channel);
	case 0x0031ADu:
		return fm_effect_33(channel);
	case 0x0031CEu:
		return fm_effect_34(channel);
	case 0x00320Du:
		return fm_effect_34_fall(channel);
	case 0x003258u:
		return fm_effect_34_trill(channel);
	case 0x00329Cu:
		return fm_effect_34_warbles(channel);
	case 0x0032D4u:
		return fm_effect_34_warble(channel);
	case 0x003316u:
		return fm_effect_35(channel);
	case 0x003336u:
		return fm_effect_36(channel);
	case 0x003353u:
		return fm_effect_36_rise(channel);
	case 0x003388u:
		fm_effect_36_hold(channel);
		return carry;
	case 0x003395u:
		return fm_effect_36_fall(channel);
	case 0x0033C2u:
		return fm_effect_38(channel);
	case 0x0033E9u:
		return fm_effect_38_trill(channel);
	case 0x003432u:
		return fm_effect_39(channel);
	case 0x00345Fu:
		return fm_effect_39_next_note(channel, carry);
	case 0x00348Bu:
		return fm_effect_3a(channel);
	case 0x0034C3u:
		return fm_effect_3a_warble(channel);
	case 0x003524u:
		return fm_effect_3b(channel);
	case 0x00354Cu:
		return fm_effect_3b_swell_and_warble(channel);
	case 0x003570u:
		return fm_effect_3c(channel);
	case 0x0035A6u:
		return fm_effect_3c_warble(channel);
	case 0x0035D3u:
		return fm_effect_3d(channel);
	case 0x0035FDu:
		return fm_effect_3d_on_channel_6(channel);
	case 0x003604u:
		return fm_effect_3d_on_channel_7(channel);
	case 0x00361Au:
		return fm_effect_3d_wait(channel, carry);
	case 0x003621u:
		return fm_effect_3e(channel);
	case 0x00363Au:
		fm_effect_3e_delay(channel);
		return carry;
	case 0x003644u:
		return fm_effect_3e_wait(channel, carry);
	case 0x003664u:
		return fm_effect_3f(channel);
	case 0x003694u:
		return fm_effect_fall_and_trills_fall(channel);
	case 0x0036C8u:
		return fm_effect_fall_and_trills(channel, carry);
	case 0x003703u:
		return fm_effect_40(channel);
	case 0x00373Fu:
		return fm_effect_41(channel);
	case 0x003769u:
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
	handler = 0x5E7u;
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
