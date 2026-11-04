/**
 *    Name:        commands.c
 *    Description: the 68000's sound commands: the reset and its tests, the command's dispatch, the title music, the speech, the starts of the music and the effects, the fade, silence, the channels' stops
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* The reset: tests the RAM with four patterns and then the ROM; a failure beeps and starts again. */
void sound_reset(void)
{
	uint8_t pattern = 0;
	uint16_t place = 0;
	uint8_t t1 = 0;

	z80_block(7);
	pattern = 0u;
	for (;;)
	{
		z80_block(10);
		place = RAM_START;
		for (;;)
		{
			z80_block(28);
			z80_write8(place, pattern);
			if (z80_read8(place) != pattern)
			{
				self_test_failed();
				return;
			}
			z80_block(24);
			place++;
			if (place >> 8 == 0x88u)
				break;
			z80_cycles(5);
		}
		z80_block(22);
		t1 = pattern;
		pattern = t1 + 0x55u;
		if ((uint8_t)(t1 + 0x55u) < t1)
			break;
		z80_cycles(5);
	}
	check_rom();
}

/* Adds up the program ROM and compares the sum with the one it should have, then starts the program (the
   game's board puts its own in place of it: there is no ROM to add up). */
void BOARD_ORIGINAL(check_rom)(void)
{
	uint16_t byte = 0, place = 0, sum = 0;

	z80_block(27);
	place = 0u;
	sum = 0u;
	byte = 0u;
	for (;;)
	{
		z80_block(46);
		byte = z80_read8(place);
		sum += byte;
		place++;
		if (place >> 8 == 0x80u)
			break;
		z80_cycles(5);
	}
	z80_block(21);
	if (sum >> 8 != 0x78u)
	{
		self_test_failed();
		return;
	}
	z80_block(18);
	if ((uint8_t)sum == 0x3Cu)
	{
		z80_block(10);
		sound_main();
		return;
	}
	z80_cycles(5);
	self_test_failed();
}

/* The interrupt: takes the 68000's command and starts what it asks for: a reset, silence, the music's
   fade-out, the title music, an FM effect, a song, a PCM effect or a speech. */
void sound_command(void)
{
	uint32_t carry = 0;
	uint8_t command = 0, count = 0;
	uint16_t channel = 0, handler = 0, record_size = 0, song_offset = 0;
	uint16_t saved_song_offset = 0;
	uint8_t t1 = 0;

	z80_block(31);
	interrupts_off();
	t1 = z80_read8(sound_latch);
	command = t1;
	if (command == 0u)
	{
		reset_sound();
		return;
	}
	z80_block(17);
	if (command == 0xFFu)
	{
		silence();
		return;
	}
	z80_block(17);
	if (command == 0xFEu)
	{
		z80_block(30);
		fade_timer = 0x16u;
		return;
	}
	z80_block(17);
	if (command == 0x7Du)
	{
		z80_block(95);
		title_music_on = 1u;
		title_music_rounds = 0xDu;
		title_music_passes = 0xFAu;
		z80_write8(sound_control, z80_read8(sound_control) | 4u);
		return;
	}
	z80_block(17);
	if (command == 0x7Eu)
	{
		stop_title_music();
		return;
	}
	z80_block(17);
	if (command < 0x60u)
	{
		carry = start_fm_effect(command);
		return;
	}
	z80_block(17);
	if (command < 0x80u)
	{
		z80_block(19);
		command -= 0x60u;
		if (command >= 0x19u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(27);
		if ((fm_effect_34_on & 1u) != 0u)
		{
			z80_cycles(6);
			return;
		}
		z80_block(41);
		handler = music_track_start_ADDRESS;
		record_size = 0x40u;
		count = 9u;
		channel = MUSIC_FM_1;
		for (;;)
		{
			z80_block(99);
			CHANNEL(channel)->sound = 0xFFu;
			CHANNEL(channel)->handler = handler;
			CHANNEL(channel)->priority = 0xFFu;
			channel += record_size;
			count--;
			if (count == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(255);
		song_offset = command * 2u;
		saved_song_offset = song_offset;
		song_header = song_offset * 2u * 2u * 2u + saved_song_offset + (songs_ADDRESS - 0x2u);
		music_attenuation_changed = 0u;
		fade_timer = 0u;
		fade_step = 0u;
		fade_attenuation = 0u;
		music_attenuation = 0u;
		music_transpose = 0u;
		music_volume = 0u;
		AS8(accelerando) = 0u;
		tempo_increase = 0u;
		return;
	}
	z80_block(17);
	if (command < 0xA0u)
	{
		start_pcm_effect(command);
		return;
	}
	z80_block(17);
	if (command < 0xE0u)
	{
		start_speech(command);
		return;
	}
	z80_block(10);
}

/* A failed test: a beep on the K007232, then the reset again. */
void self_test_failed(void)
{
	uint8_t t1 = 0;

	z80_block(143);
	z80_write16(pcm_a_start, 0x8750u);
	z80_write8(pcm_a_bank, 0u);
	z80_write16(pcm_a_pitch, 0xFEAu);
	z80_write8(pcm_volume_port, 0xF0u);
	t1 = z80_read8(pcm_a_key);
	z80_write8(pcm_loop_flags, 1u);
	sound_reset();
}

/* Stops the title music, and with it a speech (they share the line that resets the speech chip). */
void stop_title_music(void)
{
	z80_block(53);
	title_music_on = 0u;
	title_music_passes = 0u;
	title_music_rounds = 0u;
	stop_speech();
}

/* Counts the main loop's passes while the title music plays: after 13 rounds of 250 it stops it. */
void title_music_timer(void)
{
	uint8_t t1 = 0, t2 = 0;

	z80_block(26);
	t1 = title_music_passes;
	title_music_passes = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(36);
	title_music_passes = 0xFAu;
	t2 = title_music_rounds;
	title_music_rounds = t2 - 1u;
	if (t2 != 1u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(10);
	stop_title_music();
}

/* Starts a speech on the speech channel when its priority is not below the one speaking (the speech chip
   reset first); one of priority 12 that cannot plays PCM effect 87 instead (the test meant for 13
   compares the effect's number, so it never holds). Command DF stops the speech. */
void start_speech(uint8_t command)
{
	uint8_t fallback_effect = 0, priority = 0, sound = 0;
	uint8_t t1 = 0;

	z80_block(24);
	command += 0x60u;
	if (command == 0x3Fu)
	{
		stop_speech();
		return;
	}
	z80_block(12);
	if (command >= 0x24u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(66);
	sound = command;
	priority = speech_priorities[sound];
	if (speech_priority <= priority)
	{
		z80_block(134);
		speech_priority = priority;
		CHANNEL(SPEECH)->sound = sound;
		z80_write8(sound_control, z80_read8(sound_control) & ~2u);
		AS8(CHANNEL(SPEECH)->handler) = 0x78u;
		AT8(CHANNEL(SPEECH)->handler, 1) = 1u;
		if (sound == 9u)
		{
			z80_block(35);
			z80_write8(sound_control, z80_read8(sound_control) | 2u);
			return;
		}
		z80_block(30);
		z80_write8(sound_control, 2u);
		return;
	}
	z80_block(24);
	t1 = priority;
	fallback_effect = 0x87u;
	if (t1 == 0x12u)
	{
		start_pcm_effect(fallback_effect);
		return;
	}
	z80_block(12);
	if (fallback_effect != 0x13u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(17);
	command = 0x8Au;
	start_pcm_effect(command);
}

/* Stops the speech: the speech chip reset, the title music off, the speech channel free, the music back
   to its attenuation. */
void stop_speech(void)
{
	z80_block(161);
	z80_write8(sound_control, 0u);
	speech_priority = 0u;
	CHANNEL(SPEECH)->sound = 0u;
	CHANNEL(SPEECH)->priority = 0u;
	music_attenuation = 0u;
	AS8(CHANNEL(SPEECH)->handler) = 0xE7u;
	AT8(CHANNEL(SPEECH)->handler, 1) = 5u;
	z80_write8(sound_control, 2u);
}

/* The speech channel's first pass: the speech's number to the speech chip and a pulse on its start line;
   the music turned down while it speaks. */
uint32_t speech_play(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, count = 0, sound = 0;
	uint16_t handler = 0;

	z80_block(39);
	z80_write8(speech_sample, CHANNEL(channel)->sound);
	count = 3u;
	for (;;)
	{
		z80_block(8);
		count--;
		if (count == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(27);
	z80_write8(speech_start, 1u);
	count = 6u;
	for (;;)
	{
		z80_block(8);
		count--;
		if (count == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(114);
	z80_write8(speech_start, 0u);
	sound = CHANNEL(channel)->sound;
	carry = (uint16_t)(speech_music_attenuation_ADDRESS + sound) < speech_music_attenuation_ADDRESS;
	attenuation = speech_music_attenuation[sound];
	music_attenuation = attenuation;
	music_attenuation_changed = attenuation;
	handler = speech_wait_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Waits for the speech chip to finish; then frees the channel and turns the music back up. */
uint32_t speech_wait(uint16_t channel, uint32_t carry)
{
	uint8_t t1 = 0;

	z80_block(26);
	t1 = z80_read8(speech_busy);
	if ((t1 & 1u) == 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(139);
	carry = 0u;
	CHANNEL(channel)->sound = 0u;
	CHANNEL(channel)->priority = 0u;
	speech_priority = 0u;
	music_attenuation = 0u;
	music_attenuation_changed = 0u;
	AS8(CHANNEL(channel)->handler) = 0xE7u;
	AT8(CHANNEL(channel)->handler, 1) = 5u;
	return carry;
}

/* Starts a PCM effect on the PCM effects' channel B (effect 86 on channel A) when its priority is not
   below the one playing there; an effect that plays already starts again. */
void start_pcm_effect(uint8_t command)
{
	uint8_t handler_high = 0, priority = 0, sound = 0, sound_2 = 0;
	uint16_t channel = 0;
	uint8_t t1 = 0;

	z80_block(12);
	t1 = command;
	sound = t1 + 0x80u;
	if (t1 == 0x80u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(12);
	if (sound >= 0x14u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(74);
	sound_2 = sound;
	priority = pcm_effect_priorities[sound];
	channel = PCM_EFFECT_A;
	if (sound != 6u)
	{
		z80_block(43);
		channel = PCM_EFFECT_B;
		if (sound != CHANNEL(PCM_EFFECT_B)->sound)
		{
			z80_block(28);
			if (priority < CHANNEL(PCM_EFFECT_B)->priority)
			{
				z80_cycles(6);
				return;
			}
		}
	}
	z80_block(142);
	CHANNEL(channel)->priority = priority;
	CHANNEL(channel)->sound = sound_2;
	sound_2 = (uint32_t)sound_2 << 1;
	handler_high = TABLE8(pcm_effect_handlers, 0x1u + sound_2);
	AS8(CHANNEL(channel)->handler) = TABLE8(pcm_effect_handlers, sound_2);
	AT8(CHANNEL(channel)->handler, 1) = handler_high;
}

/* Starts an FM effect: one of priority 20 or more on the first FM effects' channel; effects 0A and 21 on
   the music's channel 6, and one that plays already on its own channel; the others on whichever of the
   three has the lowest priority, when theirs is not below it. */
uint32_t start_fm_effect(uint8_t command)
{
	uint32_t carry = 0;
	uint8_t handler_high = 0, lowest_priority = 0, lowest_priority_2 = 0, new_priority = 0, priority = 0, sound = 0, sound_2 = 0;
	uint16_t channel = 0, lowest_channel = 0;
	uint16_t saved_lowest_channel = 0;
	uint8_t priority_2 = 0;

	z80_block(12);
	carry = command < 0x42u;
	if (command >= 0x42u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(69);
	sound = command;
	priority = fm_effect_priorities[sound];
	new_effect_priority = priority;
	if (priority >= 0x20u)
	{
		z80_block(38);
		channel = FM_EFFECT_0;
		priority_2 = CHANNEL(FM_EFFECT_0)->priority;
		carry = priority < priority_2;
		if (priority < priority_2)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(10);
		z80_block(155);
		priority = new_effect_priority;
		CHANNEL(channel)->sound = sound;
		CHANNEL(channel)->priority = priority;
		sound = (uint32_t)sound << 1;
		carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
		handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
		AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
		AT8(CHANNEL(channel)->handler, 1) = handler_high;
		return carry;
	}
	z80_block(35);
	sound_2 = sound;
	channel = FM_EFFECT_6;
	if (sound_2 == 0x21u)
	{
		z80_block(155);
		priority = new_effect_priority;
		CHANNEL(channel)->sound = sound;
		CHANNEL(channel)->priority = priority;
		sound = (uint32_t)sound << 1;
		carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
		handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
		AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
		AT8(CHANNEL(channel)->handler, 1) = handler_high;
		return carry;
	}
	z80_block(17);
	if (sound_2 == 0xAu)
	{
		z80_block(155);
		priority = new_effect_priority;
		CHANNEL(channel)->sound = sound;
		CHANNEL(channel)->priority = priority;
		sound = (uint32_t)sound << 1;
		carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
		handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
		AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
		AT8(CHANNEL(channel)->handler, 1) = handler_high;
		return carry;
	}
	z80_block(29);
	if (sound_2 == CHANNEL(FM_EFFECT_6)->sound)
	{
		z80_block(155);
		priority = new_effect_priority;
		CHANNEL(channel)->sound = sound;
		CHANNEL(channel)->priority = priority;
		sound = (uint32_t)sound << 1;
		carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
		handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
		AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
		AT8(CHANNEL(channel)->handler, 1) = handler_high;
		return carry;
	}
	z80_block(43);
	channel = FM_EFFECT_7;
	if (sound_2 == CHANNEL(FM_EFFECT_7)->sound)
	{
		z80_block(155);
		priority = new_effect_priority;
		CHANNEL(channel)->sound = sound;
		CHANNEL(channel)->priority = priority;
		sound = (uint32_t)sound << 1;
		carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
		handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
		AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
		AT8(CHANNEL(channel)->handler, 1) = handler_high;
		return carry;
	}
	z80_block(43);
	channel = FM_EFFECT_0;
	if (sound_2 == CHANNEL(FM_EFFECT_0)->sound)
	{
		z80_block(155);
		priority = new_effect_priority;
		CHANNEL(channel)->sound = sound;
		CHANNEL(channel)->priority = priority;
		sound = (uint32_t)sound << 1;
		carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
		handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
		AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
		AT8(CHANNEL(channel)->handler, 1) = handler_high;
		return carry;
	}
	z80_block(54);
	lowest_channel = FM_EFFECT_0;
	lowest_priority = CHANNEL(FM_EFFECT_0)->priority;
	channel = FM_EFFECT_6;
	lower_priority_channel(channel, &lowest_channel, &lowest_priority);
	z80_block(31);
	channel = FM_EFFECT_7;
	lower_priority_channel(channel, &lowest_channel, &lowest_priority);
	z80_block(26);
	lowest_priority_2 = lowest_priority;
	new_priority = new_effect_priority;
	carry = new_priority < lowest_priority_2;
	if (new_priority < lowest_priority_2)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(35);
	saved_lowest_channel = lowest_channel;
	channel = saved_lowest_channel;
	z80_block(155);
	priority = new_effect_priority;
	CHANNEL(channel)->sound = sound;
	CHANNEL(channel)->priority = priority;
	sound = (uint32_t)sound << 1;
	carry = (uint16_t)(fm_effect_handlers_ADDRESS - 0x2u + sound) < fm_effect_handlers_ADDRESS - 0x2u;
	handler_high = TABLE8(fm_effect_handlers, (uint16_t)(sound - 1u));
	AS8(CHANNEL(channel)->handler) = TABLE8(fm_effect_handlers, (uint16_t)(sound - 2u));
	AT8(CHANNEL(channel)->handler, 1) = handler_high;
	return carry;
}

/* Of the channel found so far and this one, the one with the lower priority. */
void lower_priority_channel(uint16_t channel, uint16_t *p_lowest_channel, uint8_t *p_lowest_priority)
{
	uint16_t lowest_channel = (uint16_t)*p_lowest_channel;
	uint8_t lowest_priority = (uint8_t)*p_lowest_priority;
	uint16_t saved_channel = 0;

	z80_block(24);
	if (lowest_priority < CHANNEL(channel)->priority)
		z80_cycles(6);
	else
	{
		z80_block(54);
		saved_channel = channel;
		lowest_channel = saved_channel;
		lowest_priority = CHANNEL(channel)->priority;
	}
	*p_lowest_channel = lowest_channel;
	*p_lowest_priority = lowest_priority;
}

/* Command 00 and the start: the speech, the music's fade and attenuation and the FM effects' flags
   cleared, then silence. */
void reset_sound(void)
{
	z80_block(154);
	z80_write8(sound_control, 0u);
	speech_priority = 0u;
	music_attenuation_changed = 0u;
	fade_timer = 0u;
	fade_step = 0u;
	fade_attenuation = 0u;
	music_attenuation = 0u;
	fm_effect_34_on = 0u;
	fm_effect_0a_on = 0u;
	fm_effect_21_on = 0u;
	z80_write8(sound_control, 2u);
	silence();
}

/* Command FF: no LFO on the 8 FM channels and every channel stopped (the music's PCM channels only freed
   while an effect holds them). */
void silence(void)
{
	uint8_t fm_channel = 0, no_sensitivity = 0, value = 0;
	uint16_t fm_register = 0;

	z80_block(17);
	no_sensitivity = 0u;
	fm_register = 0x3800u;
	fm_channel = 0u;
	do
	{
		z80_block(17);
		value = no_sensitivity;
		fm_register = (fm_register & ~0xFFu) | fm_channel;
		write_fm_register_de(value, fm_register);
		z80_block(22);
		fm_channel++;
	} while (((fm_channel >> 3) & 1u) == 0u);
	z80_block(27);
	if (music_pcm_hold == 0u)
	{
		z80_block(17);
		stop_music_pcm_a();
		z80_block(17);
		stop_pcm_effect_b();
		z80_block(17);
		stop_music_pcm_b();
		z80_block(17);
		stop_pcm_effect_a();
	}
	else
	{
		z80_block(17);
		free_music_pcm_a();
		z80_block(17);
		free_music_pcm_b();
		z80_block(10);
	}
	z80_block(17);
	stop_fm_effect_channels();
	z80_block(17);
	stop_music_fm_6_7();
	z80_block(17);
	stop_music_fm_1_5();
	z80_block(10);
	free_speech_channel();
}

/* Stops the three FM effects' channels at their next pass. */
void stop_fm_effect_channels(void)
{
	z80_block(68);
	CHANNEL(FM_EFFECT_0)->handler = stop_channel_ADDRESS;
	CHANNEL(FM_EFFECT_6)->handler = stop_channel_ADDRESS;
	CHANNEL(FM_EFFECT_7)->handler = stop_channel_ADDRESS;
}

/* Stops the music's FM channels 6 and 7 at their next pass. */
void stop_music_fm_6_7(void)
{
	z80_block(52);
	CHANNEL(MUSIC_FM_6)->handler = stop_channel_ADDRESS;
	CHANNEL(MUSIC_FM_7)->handler = stop_channel_ADDRESS;
}

/* Stops the music's FM channels 1 to 5 at their next pass. */
void stop_music_fm_1_5(void)
{
	z80_block(100);
	CHANNEL(MUSIC_FM_1)->handler = stop_channel_ADDRESS;
	CHANNEL(MUSIC_FM_2)->handler = stop_channel_ADDRESS;
	CHANNEL(MUSIC_FM_3)->handler = stop_channel_ADDRESS;
	CHANNEL(MUSIC_FM_4)->handler = stop_channel_ADDRESS;
	CHANNEL(MUSIC_FM_5)->handler = stop_channel_ADDRESS;
}

/* Frees the music's FM channels 6 and 7 at their next pass, without keying them off. */
void free_music_fm_6_7(void)
{
	z80_block(52);
	CHANNEL(MUSIC_FM_6)->handler = free_channel_ADDRESS;
	CHANNEL(MUSIC_FM_7)->handler = free_channel_ADDRESS;
}

/* Stops the music's PCM channel A at its next pass. */
void stop_music_pcm_a(void)
{
	z80_block(36);
	CHANNEL(MUSIC_PCM_A)->handler = stop_pcm_channel_ADDRESS;
}

/* Frees the music's PCM channel A at its next pass, without stopping the sample. */
void free_music_pcm_a(void)
{
	z80_block(36);
	CHANNEL(MUSIC_PCM_A)->handler = free_channel_ADDRESS;
}

/* Stops the PCM effects' channel B at its next pass. */
void stop_pcm_effect_b(void)
{
	z80_block(36);
	CHANNEL(PCM_EFFECT_B)->handler = stop_pcm_channel_ADDRESS;
}

/* Frees the music's PCM channel B at its next pass, without stopping the sample. */
void free_music_pcm_b(void)
{
	z80_block(36);
	CHANNEL(MUSIC_PCM_B)->handler = free_channel_ADDRESS;
}

/* Stops the music's PCM channel B at its next pass. */
void stop_music_pcm_b(void)
{
	z80_block(36);
	CHANNEL(MUSIC_PCM_B)->handler = stop_pcm_channel_ADDRESS;
}

/* Stops the PCM effects' channel A at its next pass. */
void stop_pcm_effect_a(void)
{
	z80_block(36);
	CHANNEL(PCM_EFFECT_A)->handler = stop_pcm_channel_ADDRESS;
}

/* Frees the speech channel at its next pass. */
void free_speech_channel(void)
{
	z80_block(36);
	CHANNEL(SPEECH)->handler = free_channel_ADDRESS;
}
