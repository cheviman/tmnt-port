/**
 *    Name:        fm_effects.c
 *    Description: the FM effects (commands 01-41)
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* A note played three times, from the loudest. */
uint32_t fm_effect_01(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(27);
	fm_effect_01_notes = 3u;
	attenuation = 7u;
	return fm_effect_01_note(channel, attenuation);
}

/* The note keyed on at the attenuation it is given, for 52 passes. */
uint32_t fm_effect_01_note(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(40);
	fm_effect_01_attenuation = attenuation;
	voice = 0x161Fu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(30);
	attenuation = fm_effect_01_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(40);
	fm_effect_01_timer = 0x34u;
	handler = fm_effect_01_hold_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Holds the note, almost silent for its last 5 passes; then it plays again 16 quieter, three times in
   all. */
uint32_t fm_effect_01_hold(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_01_timer;
	fm_effect_01_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(31);
		t2 = fm_effect_01_notes;
		fm_effect_01_notes = t2 - 1u;
		if (t2 == 1u)
			return stop_channel(channel);
		z80_block(30);
		attenuation = fm_effect_01_attenuation + 0x10u;
		return fm_effect_01_note(channel, attenuation);
	}
	z80_block(19);
	timer = fm_effect_01_timer;
	carry = timer < 5u;
	if (timer != 5u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(17);
	attenuation = 0x29u;
	return set_fm_volume(channel, attenuation);
}

/* A note sliding down a little, struck again step after step, each step longer: 13 steps, then after a
   pause a quieter round of 9. */
uint32_t fm_effect_02(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t steps = 0;
	uint16_t voice = 0;

	z80_block(47);
	fm_effect_02_attenuation = 0x19u;
	voice = 0x1681u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	fm_effect_02_rounds = 2u;
	steps = 0xDu;
	return fm_effect_02_round(channel, steps);
}

/* Starts a round of the steps it is given: the note from its first pitch, 8 passes a step. */
uint32_t fm_effect_02_round(uint16_t channel, uint8_t steps)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(56);
	fm_effect_02_steps = steps;
	pitch = 0x1400u;
	fm_effect_02_pitch = 0x1400u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(63);
	fm_effect_02_timer = 8u;
	fm_effect_02_period = 8u;
	attenuation = fm_effect_02_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_02_slide_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note slides down each pass; each step starts it again from its first pitch one pass longer, 1
   louder while 7 or more steps are left and 3 quieter after; after the round, two passes almost silent. */
uint32_t fm_effect_02_slide(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, period = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	z80_block(31);
	t1 = fm_effect_02_timer;
	fm_effect_02_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(63);
		pitch = fm_effect_02_pitch - 2u;
		fm_effect_02_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(31);
	t2 = fm_effect_02_steps;
	fm_effect_02_steps = t2 - 1u;
	if (t2 == 1u)
	{
		z80_block(31);
		t3 = fm_effect_02_rounds;
		fm_effect_02_rounds = t3 - 1u;
		if (t3 == 1u)
			return stop_channel(channel);
		z80_block(44);
		fm_effect_02_timer = 2u;
		attenuation = 0x31u;
		carry = set_fm_volume(channel, attenuation);
		z80_block(20);
		handler = fm_effect_02_pause_ADDRESS;
		set_channel_handler(handler, channel);
		return carry;
	}
	z80_block(37);
	t4 = fm_effect_02_steps;
	attenuation = fm_effect_02_attenuation;
	if (t4 >= 7u)
	{
		z80_block(7);
		attenuation -= 4u;
	}
	z80_block(37);
	attenuation += 3u;
	fm_effect_02_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(79);
	period = fm_effect_02_period + 1u;
	fm_effect_02_timer = period;
	fm_effect_02_period = period;
	pitch = 0x1400u;
	fm_effect_02_pitch = 0x1400u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* After the two passes, the second round: 9 steps from attenuation 59. */
uint32_t fm_effect_02_pause(uint16_t channel, uint32_t carry)
{
	uint8_t steps = 0;
	uint8_t t1 = 0;

	z80_block(26);
	t1 = fm_effect_02_timer;
	fm_effect_02_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(37);
	fm_effect_02_attenuation = 0x3Bu;
	steps = 9u;
	return fm_effect_02_round(channel, steps);
}

/* Speech A6, and a falling note, then a trill that fades out. */
uint32_t fm_effect_03(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;
	uint16_t saved_channel = 0;

	z80_block(53);
	saved_channel = channel;
	command = 0xA6u;
	start_speech(command);
	z80_block(41);
	channel = saved_channel;
	voice = 0x1736u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xC00u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x1Cu;
	attenuation = 0xBu;
	CHANNEL(channel)->fm_effect.base_attenuation = 0xBu;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_03_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass for 28 passes, dying away near its end; then the trill of two
   notes. */
uint32_t fm_effect_03_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, die_away_pass = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 8u;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(27);
	pitch = 0xC00u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(129);
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 0x10u;
	CHANNEL(channel)->fm_effect.timer = 0xCu;
	CHANNEL(channel)->fm_effect.count = 8u;
	attenuation = 0xBu;
	CHANNEL(channel)->fm_effect.attenuation = 0xBu;
	carry = set_fm_volume(channel, attenuation);
	z80_block(20);
	handler = fm_effect_03_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, both sliding down; every 12 passes 4 quieter, 8 times. */
uint32_t fm_effect_03_trill(uint16_t channel)
{
	uint8_t attenuation = 0;
	uint16_t step = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(20);
		step = 0xFFA0u;
		return fm_effect_trill_slide(step, channel);
	}
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 0xCu;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(55);
	attenuation = CHANNEL(channel)->fm_effect.attenuation + 4u;
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* The three-note arpeggio of effect 1D, from the loudest, with another voice and slower. */
uint32_t fm_effect_04(uint16_t channel)
{
	uint16_t pitch = 0, voice = 0;

	z80_block(68);
	CHANNEL(channel)->fm_effect.attenuation = 0u;
	CHANNEL(channel)->fm_effect.period = 0x1Cu;
	pitch = pcm_instruments_ADDRESS - 0x2u;
	voice = 0x17CEu;
	return fm_effect_arpeggio_start(voice, pitch, channel);
}

/* Starts effect 37 (its echo, 8 passes later on another channel) and plays a note that wobbles down,
   then another note held, twice. */
uint32_t fm_effect_05(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x37u;
	carry = start_fm_effect(command);
	z80_block(14);
	channel = saved_channel;
	return fm_effect_wobble_twice(channel);
}

/* Twice, from the loudest: the note that wobbles down and the note held after it. */
uint32_t fm_effect_wobble_twice(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(26);
	CHANNEL(channel)->fm_effect.count = 2u;
	attenuation = 0u;
	return fm_effect_wobble_note(channel, attenuation);
}

/* The note keyed on at the attenuation it is given, for 32 passes. */
uint32_t fm_effect_wobble_note(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(46);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	voice = 0x17F8u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x1200u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 0x20u;
	handler = fm_effect_wobble_down_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones down on the odd passes and one up on the even ones; then another note, held 64 passes. */
uint32_t fm_effect_wobble_down(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, step = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
	{
		z80_block(46);
		CHANNEL(channel)->fm_effect.timer = 0x40u;
		pitch = 0x1020u;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		z80_block(17);
		carry = set_fm_volume_from_record(channel);
		z80_block(20);
		handler = fm_effect_wobble_hold_ADDRESS;
		set_channel_handler(handler, channel);
		return carry;
	}
	z80_block(40);
	step = 0xFF80u;
	if ((CHANNEL(channel)->fm_effect.timer & 1u) != 0u)
	{
		fm_effect_move_pitch(channel, &step, &carry);
		return carry;
	}
	z80_block(20);
	step = 0x40u;
	fm_effect_move_pitch(channel, &step, &carry);
	return carry;
}

/* Holds that note; then the second time, 20 quieter, and the channel stops. */
uint32_t fm_effect_wobble_hold(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint8_t timer = 0, count = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(17);
	attenuation = 0x14u;
	return fm_effect_wobble_note(channel, attenuation);
}

/* Effect 05's wobbling notes 8 passes late, on a channel of their own (effect 05 starts it). */
void fm_effect_37(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 8u;
	handler = fm_effect_37_wait_ADDRESS;
	set_channel_handler(handler, channel);
}

/* Waits the 8 passes, then the wobbling notes. */
uint32_t fm_effect_37_wait(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return fm_effect_wobble_twice(channel);
}

/* A falling note, then a trill that fades out (effect 03's without the speech, longer). */
uint32_t fm_effect_06(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x1736u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xC00u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x20u;
	attenuation = 5u;
	CHANNEL(channel)->fm_effect.base_attenuation = 5u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_06_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass for 32 passes, dying away near its end; then the trill of two
   notes. */
uint32_t fm_effect_06_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, die_away_pass = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 0xCu;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(46);
	CHANNEL(channel)->fm_effect.timer = 0x10u;
	pitch = 0xC00u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(110);
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 0x10u;
	CHANNEL(channel)->fm_effect.count = 0xEu;
	attenuation = 5u;
	CHANNEL(channel)->fm_effect.attenuation = 5u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(20);
	handler = fm_effect_06_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, both sliding down slowly; every 16 passes 3 quieter, 14 times. */
uint32_t fm_effect_06_trill(uint16_t channel)
{
	uint8_t attenuation = 0;
	uint16_t step = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(20);
		step = 0xFFF0u;
		return fm_effect_trill_slide(step, channel);
	}
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 0x10u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(55);
	attenuation = CHANNEL(channel)->fm_effect.attenuation + 3u;
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* Low notes of 24 or 25 passes by the random bytes' place, the later ones each ending on a higher blip
   with another voice, in two rounds: 12 notes, then 6 quieter. */
uint32_t fm_effect_07(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(47);
	fm_effect_07_rounds = 2u;
	fm_effect_07_notes = 0xCu;
	attenuation = 0u;
	return fm_effect_07_round(channel, attenuation);
}

/* Starts a round at the attenuation it is given. */
uint32_t fm_effect_07_round(uint16_t channel, uint8_t attenuation)
{
	z80_block(13);
	fm_effect_07_attenuation = attenuation;
	return fm_effect_07_note(channel);
}

/* A low note keyed on for 24 or 25 passes, by the random bytes' place (the place taken for the byte: 18
   to 25 meant). */
uint32_t fm_effect_07_note(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, random_place = 0, voice = 0;

	z80_block(17);
	random_place = next_random_byte();
	z80_block(62);
	fm_effect_07_timer = ((random_place | (random_place >> 8)) & 7u) + 0x12u;
	voice = 0x18FAu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xA00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(30);
	attenuation = fm_effect_07_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_07_step_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the note down: the first notes of a round are cut 8 passes before their end by the next; in the
   later ones a higher blip, 4 quieter, takes those 8 passes; the last of a round is 8 quieter; then the
   second round from attenuation 28. */
uint32_t fm_effect_07_step(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint16_t ignored16 = 0, pitch = 0, voice = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_07_timer;
	fm_effect_07_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(31);
		t2 = fm_effect_07_notes;
		fm_effect_07_notes = t2 - 1u;
		if (t2 == 1u)
		{
			z80_block(31);
			t3 = fm_effect_07_rounds;
			fm_effect_07_rounds = t3 - 1u;
			if (t3 == 1u)
				return stop_channel(channel);
			z80_block(37);
			fm_effect_07_notes = 6u;
			attenuation = 0x1Cu;
			return fm_effect_07_round(channel, attenuation);
		}
		z80_block(24);
		if (fm_effect_07_notes != 1u)
			return fm_effect_07_note(channel);
		z80_block(43);
		fm_effect_07_attenuation += 8u;
		return fm_effect_07_note(channel);
	}
	z80_block(19);
	timer = fm_effect_07_timer;
	carry = timer < 8u;
	if (timer != 8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(45);
	fm_effect_07_notes--;
	if (fm_effect_07_notes >= 9u)
		return fm_effect_07_note(channel);
	z80_block(38);
	fm_effect_07_notes++;
	voice = 0x1913u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(37);
	attenuation = fm_effect_07_attenuation + 4u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(10);
	return fm_key_on(channel);
}

/* A low note that rises and then warbles, three times, each quieter and a little longer. */
uint32_t fm_effect_08(uint16_t channel)
{
	uint8_t length = 0;

	z80_block(45);
	CHANNEL(channel)->fm_effect.count = 3u;
	CHANNEL(channel)->fm_effect.attenuation = 0xFu;
	length = 0x48u;
	return fm_effect_08_note(channel, length);
}

/* The note keyed on for the passes it is given. */
uint32_t fm_effect_08_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = length;
	CHANNEL(channel)->fm_effect.period = length;
	voice = 0x19B8u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x200u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_08_rise_and_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* A semitone up each pass while 64 or more passes are left, then a warble; then the next note, 26
   quieter and 2 passes longer. */
uint32_t fm_effect_08_rise_and_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t length = 0;
	uint16_t ignored16 = 0, step = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
	{
		z80_block(33);
		count = CHANNEL(channel)->fm_effect.count;
		CHANNEL(channel)->fm_effect.count = count - 1u;
		if (count == 1u)
			return stop_channel(channel);
		z80_block(81);
		CHANNEL(channel)->fm_effect.attenuation += 0x1Au;
		length = CHANNEL(channel)->fm_effect.period + 2u;
		return fm_effect_08_note(channel, length);
	}
	z80_block(36);
	if (CHANNEL(channel)->fm_effect.timer < 0x40u)
	{
		fm_effect_warble(channel, &ignored16, &carry);
		return carry;
	}
	z80_block(20);
	step = 0x40u;
	fm_effect_move_pitch(channel, &step, &carry);
	return carry;
}

/* Stops effect 0A and the drone of effect 33 (clears their flag); its own channel stops at once. */
uint32_t fm_effect_09(uint16_t channel)
{
	z80_block(27);
	fm_effect_0a_on = 0u;
	return stop_channel(channel);
}

/* Starts effect 33 (a low drone) and plays rounds of low notes that rise, at one of four pitches by the
   random bytes' place, until effect 09 stops it or its 28 rounds end. */
uint32_t fm_effect_0a(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x33u;
	carry = start_fm_effect(command);
	z80_block(61);
	channel = saved_channel;
	fm_effect_0a_on = 1u;
	fm_effect_0a_rounds = 0x1Cu;
	attenuation = 6u;
	return fm_effect_0a_round(channel, attenuation);
}

/* Starts a round of 16 notes at the attenuation it is given. */
uint32_t fm_effect_0a_round(uint16_t channel, uint8_t attenuation)
{
	z80_block(33);
	fm_effect_0a_round_attenuation = attenuation;
	fm_effect_0a_notes = 0x10u;
	return fm_effect_0a_pair(channel);
}

/* Loads the voice again for a pair of notes. */
uint32_t fm_effect_0a_pair(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t voice = 0;

	z80_block(47);
	fm_effect_0a_pair_notes = 2u;
	voice = 0x1A2Au;
	carry = load_fm_voice(voice, channel);
	z80_block(13);
	attenuation = fm_effect_0a_round_attenuation;
	return fm_effect_0a_note(channel, attenuation);
}

/* A note at one of four low pitches by the random bytes' place (up to 3 quieter with the higher ones),
   keyed on for 12 passes at the attenuation it is given. */
uint32_t fm_effect_0a_note(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint8_t pitch_choice = 0;
	uint16_t handler = 0, ignored16 = 0, random_place = 0;

	z80_block(30);
	fm_effect_0a_attenuation = attenuation;
	random_place = next_random_byte();
	z80_block(73);
	pitch_choice = random_place & 3u;
	random_place = 0x80u + (((uint32_t)pitch_choice << 8) | (random_place >> 8));
	fm_effect_0a_pitch = random_place;
	set_fm_pitch(random_place, channel, &ignored16, &carry);
	z80_block(54);
	fm_effect_0a_timer = 0xCu;
	attenuation = fm_effect_0a_attenuation + pitch_choice;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_0a_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note rises a semitone a pass; then the next one, the second of a pair 16 quieter; each round 1
   quieter than the last; it stops when its flag is cleared. */
uint32_t fm_effect_0a_rise(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	z80_block(31);
	t1 = fm_effect_0a_timer;
	fm_effect_0a_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(63);
		pitch = fm_effect_0a_pitch + 0x40u;
		fm_effect_0a_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(27);
	if (fm_effect_0a_on == 0u)
		return stop_channel(channel);
	z80_block(31);
	t2 = fm_effect_0a_notes;
	fm_effect_0a_notes = t2 - 1u;
	if (t2 == 1u)
	{
		z80_block(31);
		t3 = fm_effect_0a_rounds;
		fm_effect_0a_rounds = t3 - 1u;
		if (t3 == 1u)
			return stop_channel(channel);
		z80_block(30);
		attenuation = fm_effect_0a_round_attenuation + 1u;
		return fm_effect_0a_round(channel, attenuation);
	}
	z80_block(31);
	t4 = fm_effect_0a_pair_notes;
	fm_effect_0a_pair_notes = t4 - 1u;
	if (t4 == 1u)
		return fm_effect_0a_pair(channel);
	z80_block(30);
	attenuation = fm_effect_0a_attenuation + 0x10u;
	return fm_effect_0a_note(channel, attenuation);
}

/* The low note keyed on at the attenuation it is given, for 96 passes. */
uint32_t fm_effect_33_drone(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(40);
	fm_effect_33_attenuation = attenuation;
	voice = 0x1AD8u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xA0u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(50);
	fm_effect_33_timer = 0x60u;
	attenuation = fm_effect_33_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_33_hold_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Holds it; then strikes it again 1 quieter; stops when the flag is cleared or the 20 times end. */
uint32_t fm_effect_33_hold(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(27);
	carry = 0u;
	if (fm_effect_0a_on == 0u)
		return stop_channel(channel);
	z80_block(26);
	t1 = fm_effect_33_timer;
	fm_effect_33_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(31);
	t2 = fm_effect_33_repeats;
	fm_effect_33_repeats = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(30);
	attenuation = fm_effect_33_attenuation + 1u;
	return fm_effect_33_drone(channel, attenuation);
}

/* A low note held, then a long slow slide down, twice (the second quieter). */
uint32_t fm_effect_0b(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(67);
	fm_effect_0b_timer = 0x1Cu;
	fm_effect_0b_slides = 2u;
	voice = 0x1B36u;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x400u;
	fm_effect_0b_pitch = 0x400u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(37);
	attenuation = 8u;
	fm_effect_0b_attenuation = 8u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_0b_hold_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Holds the note 28 passes, much quieter for the last 12; then the slide. */
uint32_t fm_effect_0b_hold(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_0b_timer;
	fm_effect_0b_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(7);
		attenuation = 0xAu;
		return fm_effect_0b_slide_start(channel, attenuation);
	}
	z80_block(19);
	timer = fm_effect_0b_timer;
	carry = timer < 0xCu;
	if (timer != 0xCu)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(17);
	attenuation = 0x27u;
	return set_fm_volume(channel, attenuation);
}

/* Sets the attenuation it is given for a slide of 96 passes. */
uint32_t fm_effect_0b_slide_start(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(30);
	fm_effect_0b_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(40);
	fm_effect_0b_slide_timer = 0x60u;
	handler = fm_effect_0b_slide_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note slides down an eighth of a semitone each pass; then again from its first pitch, at
   attenuation 23. */
uint32_t fm_effect_0b_slide(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_0b_slide_timer;
	fm_effect_0b_slide_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(63);
		pitch = fm_effect_0b_pitch - 8u;
		fm_effect_0b_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(31);
	t2 = fm_effect_0b_slides;
	fm_effect_0b_slides = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(43);
	pitch = 0x400u;
	fm_effect_0b_pitch = 0x400u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	attenuation = 0x17u;
	return fm_effect_0b_slide_start(channel, attenuation);
}

/* PCM effect 90 and a long run of zigzag notes (152 steps), at full while many steps are left, then
   sinking and fading. */
uint32_t fm_effect_0c(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x90u;
	start_pcm_effect(command);
	z80_block(157);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect_0c.period = 0xAu;
	CHANNEL(channel)->fm_effect_0c.steps = 0x98u;
	CHANNEL(channel)->fm_effect_0c.round_attenuation = 1u;
	AS8(CHANNEL(channel)->fm_effect_0c.voice) = 0xC5u;
	AT8(CHANNEL(channel)->fm_effect_0c.voice, 1) = 0x1Bu;
	CHANNEL(channel)->fm_effect_0c.fade_start = 0x30u;
	CHANNEL(channel)->fm_effect_0c.fading_step = 2u;
	return fm_effect_zigzag_steps_start(channel);
}

/* Starts the steps the effect gives, with the sinking note at its first pitch. */
uint32_t fm_effect_zigzag_steps_start(uint16_t channel)
{
	z80_block(86);
	CHANNEL(channel)->fm_effect_zigzag_steps.count = CHANNEL(channel)->fm_effect_zigzag_steps.steps;
	AS8(CHANNEL(channel)->fm_effect_zigzag_steps.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect_zigzag_steps.base_pitch, 1) = 7u;
	return fm_effect_zigzag_steps_reload(channel);
}

/* The note again from its first pitch at full: the voice reloaded, the effect's attenuation over the
   music's. */
uint32_t fm_effect_zigzag_steps_reload(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t pitch = 0, voice = 0;

	z80_block(106);
	CHANNEL(channel)->fm_effect_zigzag_steps.attenuation = music_attenuation + CHANNEL(channel)->fm_effect_zigzag_steps.round_attenuation;
	voice = CHANNEL(channel)->fm_effect_zigzag_steps.voice;
	carry = load_fm_voice(voice, channel);
	z80_block(10);
	pitch = 0x500u;
	return fm_effect_zigzag_steps_note(pitch, channel);
}

/* The note at the pitch it is given and the record's attenuation. */
uint32_t fm_effect_zigzag_steps_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0;

	z80_block(17);
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(19);
	attenuation = CHANNEL(channel)->fm_effect_zigzag_steps.attenuation;
	return fm_effect_zigzag_steps_key_on(channel, attenuation);
}

/* Keys the note on at the attenuation it is given, for a step of the effect's period. */
uint32_t fm_effect_zigzag_steps_key_on(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(17);
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(58);
	CHANNEL(channel)->fm_effect_zigzag_steps.timer = CHANNEL(channel)->fm_effect_zigzag_steps.period;
	handler = fm_effect_zigzag_steps_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note zigzags down (four semitones up and eight down in turn) for a step; then an odd step keys the
   first pitch again 24 quieter, an even one starts at full again while the steps left are not below the
   fade's start, else from the sinking pitch (two semitones lower each time), quieter by the fade's step. */
uint32_t fm_effect_zigzag_steps(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, pitch_high = 0, pitch_low = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0, voice = 0;
	uint8_t timer = 0, count = 0, fading_step = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect_zigzag_steps.timer;
	CHANNEL(channel)->fm_effect_zigzag_steps.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(40);
		step = 0x100u;
		if ((CHANNEL(channel)->fm_effect_zigzag_steps.timer & 1u) != 0u)
		{
			fm_effect_move_pitch(channel, &step, &carry);
			return carry;
		}
		z80_block(20);
		step = 0xFE00u;
		fm_effect_move_pitch(channel, &step, &carry);
		return carry;
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect_zigzag_steps.count;
	CHANNEL(channel)->fm_effect_zigzag_steps.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(30);
	if ((CHANNEL(channel)->fm_effect_zigzag_steps.count & 1u) == 0u)
	{
		z80_block(48);
		if (CHANNEL(channel)->fm_effect_zigzag_steps.fade_start <= CHANNEL(channel)->fm_effect_zigzag_steps.count)
			return fm_effect_zigzag_steps_reload(channel);
		z80_block(55);
		voice = CHANNEL(channel)->fm_effect_zigzag_steps.voice;
		carry = load_fm_voice(voice, channel);
		z80_block(168);
		attenuation = CHANNEL(channel)->fm_effect_zigzag_steps.attenuation;
		fading_step = CHANNEL(channel)->fm_effect_zigzag_steps.fading_step;
		carry = (uint8_t)(attenuation + fading_step) < attenuation;
		CHANNEL(channel)->fm_effect_zigzag_steps.attenuation = attenuation + fading_step;
		t1 = CHANNEL(channel)->fm_effect_zigzag_steps.base_pitch - 0x80u - carry;
		pitch_high = (uint8_t)(t1 >> 8);
		pitch_low = (uint8_t)t1;
		AS8(CHANNEL(channel)->fm_effect_zigzag_steps.base_pitch) = pitch_low;
		AT8(CHANNEL(channel)->fm_effect_zigzag_steps.base_pitch, 1) = pitch_high;
		pitch = ((uint32_t)pitch_high << 8) | pitch_low;
		return fm_effect_zigzag_steps_note(pitch, channel);
	}
	z80_block(27);
	pitch = 0x500u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(36);
	attenuation = CHANNEL(channel)->fm_effect_zigzag_steps.attenuation + 0x18u;
	return fm_effect_zigzag_steps_key_on(channel, attenuation);
}

/* A falling note, then rounds of a trill that fades, each round quieter, 4 times. */
uint32_t fm_effect_0d(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x1736u;
	carry = load_fm_voice(voice, channel);
	z80_block(46);
	CHANNEL(channel)->fm_effect.rounds = 4u;
	pitch = 0xD00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(110);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 8u;
	CHANNEL(channel)->fm_effect.timer = 0x18u;
	attenuation = 1u;
	CHANNEL(channel)->fm_effect.base_attenuation = 1u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_0d_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass for 24 passes, dying away near its end; then the first round of
   the trill. */
uint32_t fm_effect_0d_fall(uint16_t channel)
{
	uint8_t attenuation = 0, die_away_pass = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
	{
		z80_block(7);
		attenuation = 1u;
		return fm_effect_0d_trill_round(channel, attenuation);
	}
	z80_block(17);
	die_away_pass = 0xCu;
	return fm_effect_fall(channel, die_away_pass);
}

/* Starts a round of 6 trill steps at the attenuation it is given. */
uint32_t fm_effect_0d_trill_round(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.round_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = 0x10u;
	CHANNEL(channel)->fm_effect.count = 6u;
	pitch = 0xD00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(116);
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 8u;
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xAu;
	handler = fm_effect_0d_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, sliding down; every 16 passes they start again from their pitches, 2 quieter;
   after the round, a pause of 10 passes almost silent. */
uint32_t fm_effect_0d_trill(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0, rounds = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(20);
		step = 0xFFC0u;
		return fm_effect_trill_slide(step, channel);
	}
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 0x10u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
	{
		z80_block(33);
		rounds = CHANNEL(channel)->fm_effect.rounds;
		CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
		if (rounds == 1u)
			return stop_channel(channel);
		z80_block(43);
		CHANNEL(channel)->fm_effect.count = 0xAu;
		attenuation = 0x31u;
		carry = set_fm_volume(channel, attenuation);
		z80_block(20);
		handler = fm_effect_0d_pause_ADDRESS;
		set_channel_handler(handler, channel);
		return carry;
	}
	z80_block(62);
	attenuation = CHANNEL(channel)->fm_effect.attenuation + 2u;
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(68);
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 0xAu;
	pitch = 0x800u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* After the pause, the next round, 18 quieter. */
uint32_t fm_effect_0d_pause(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint8_t count = 0;

	z80_block(28);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(36);
	attenuation = CHANNEL(channel)->fm_effect.round_attenuation + 0x12u;
	return fm_effect_0d_trill_round(channel, attenuation);
}

/* A note that zigzags down, then a low note that zigzags up, 16 times, each 4 quieter. */
uint32_t fm_effect_0e(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x1D5Bu;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	fm_effect_0e_pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(44);
	fm_effect_0e_timer = 0x14u;
	attenuation = 0u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_0e_zigzag_down_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Eight semitones down and four up in turn for 20 passes, quieter for the last 6; then the low note. */
uint32_t fm_effect_0e_zigzag_down(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0, voice = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_0e_timer;
	fm_effect_0e_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(57);
		fm_effect_0e_timer = 0x60u;
		fm_effect_0e_repeats = 0x10u;
		voice = 0x1D5Bu;
		carry = load_fm_voice(voice, channel);
		z80_block(43);
		pitch = 0x600u;
		fm_effect_0e_pitch = 0x600u;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		z80_block(7);
		attenuation = 0u;
		return fm_effect_0e_zigzag_up_start(channel, attenuation);
	}
	z80_block(24);
	timer = fm_effect_0e_timer;
	if (timer == 6u)
	{
		z80_block(17);
		attenuation = 0x10u;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(44);
	pitch = fm_effect_0e_pitch;
	step = 0xFE00u;
	if (timer & 1u)
	{
		z80_block(10);
		step = 0x100u;
	}
	z80_block(37);
	pitch += step;
	fm_effect_0e_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* Keys the low note on at the attenuation it is given. */
uint32_t fm_effect_0e_zigzag_up_start(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(30);
	fm_effect_0e_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_0e_zigzag_up_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Four semitones down and eight up in turn: 96 passes the first time, then 32; each time again from the
   low note, 4 quieter, 16 times. */
uint32_t fm_effect_0e_zigzag_up(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_0e_timer;
	fm_effect_0e_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(41);
		fm_effect_0e_timer = 0x20u;
		t2 = fm_effect_0e_repeats;
		fm_effect_0e_repeats = t2 - 1u;
		if (t2 == 1u)
			return stop_channel(channel);
		z80_block(43);
		pitch = 0x600u;
		fm_effect_0e_pitch = 0x600u;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		z80_block(30);
		attenuation = fm_effect_0e_attenuation + 4u;
		return fm_effect_0e_zigzag_up_start(channel, attenuation);
	}
	z80_block(48);
	pitch = fm_effect_0e_pitch;
	step = 0xFF00u;
	if (fm_effect_0e_timer & 1u)
	{
		z80_block(10);
		step = 0x200u;
	}
	z80_block(37);
	pitch += step;
	fm_effect_0e_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* PCM effect 91 and a falling note, then PCM effect 91 again and 17 bursts of notes that fade. */
uint32_t fm_effect_0f(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0;
	uint16_t handler = 0, voice = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x91u;
	start_pcm_effect(command);
	z80_block(60);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.base_attenuation = 3u;
	voice = 0x1E1Au;
	carry = fm_effect_drop_start(voice, channel);
	z80_block(20);
	handler = fm_effect_0f_drop_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall for 32 passes; then PCM effect 91 again and the bursts. */
uint32_t fm_effect_0f_drop(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t saved_channel = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_drop(channel);
	z80_block(39);
	saved_channel = channel;
	command = 0x91u;
	start_pcm_effect(command);
	z80_block(24);
	channel = saved_channel;
	return fm_effect_bursts_start(channel);
}

/* Loads the voice it is given and keys a note on at the effect's base attenuation, for a fall of 32
   passes. */
uint32_t fm_effect_drop_start(uint16_t voice, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t base_attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;

	z80_block(17);
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(141);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 6u;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 6u;
	CHANNEL(channel)->fm_effect.timer = 0x20u;
	base_attenuation = CHANNEL(channel)->fm_effect.base_attenuation;
	carry = set_fm_volume(channel, base_attenuation);
	z80_block(10);
	return fm_key_on(channel);
}

/* A pass of the fall: two semitones down, the note dying away when 16 passes are left. */
uint32_t fm_effect_drop(uint16_t channel)
{
	uint8_t die_away_pass = 0;

	z80_block(17);
	die_away_pass = 0x10u;
	return fm_effect_fall(channel, die_away_pass);
}

/* Starts 17 bursts from the base attenuation. */
uint32_t fm_effect_bursts_start(uint16_t channel)
{
	z80_block(19);
	CHANNEL(channel)->fm_effect.rounds = 0x11u;
	return fm_effect_burst_from_base(channel);
}

/* A burst at the base attenuation. */
uint32_t fm_effect_burst_from_base(uint16_t channel)
{
	uint8_t base_attenuation = 0;

	z80_block(19);
	base_attenuation = CHANNEL(channel)->fm_effect.base_attenuation;
	return fm_effect_burst(channel, base_attenuation);
}

/* A burst of 5 steps at the attenuation it is given: two notes in turn sliding down, 5 passes a step. */
uint32_t fm_effect_burst(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.round_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = 5u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0xC00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(68);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xAu;
	handler = fm_effect_burst_step_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Each step the two notes at lower pitches that move with the random bytes' place, 1 quieter; after the
   burst the next one, 3 quieter (or again from the base attenuation, by a flag no effect sets). */
uint32_t fm_effect_burst_step(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t base_pitch = 0, offset = 0, pitch = 0;
	uint8_t timer = 0, count = 0, rounds = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 5u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count != 1u)
	{
		z80_block(17);
		fm_effect_quieter_pool_offset(channel, &offset, &carry);
		z80_block(83);
		t1 = 0xA00u - offset - carry;
		carry = offset + carry > 0xA00u;
		base_pitch = t1;
		CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
		pitch = 0x600u;
		return fm_effect_set_pitch_minus(offset, pitch, channel, carry);
	}
	z80_block(33);
	rounds = CHANNEL(channel)->fm_effect.rounds;
	CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
	if (rounds == 1u)
		return stop_channel(channel);
	z80_block(30);
	if (((CHANNEL(channel)->fm_effect.burst_flags >> 4) & 1u) != 0u)
		return fm_effect_burst_from_base(channel);
	z80_block(36);
	attenuation = CHANNEL(channel)->fm_effect.round_attenuation + 3u;
	return fm_effect_burst(channel, attenuation);
}

/* PCM effect 84 and a falling note, then a trill that fades out. */
uint32_t fm_effect_10(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x84u;
	start_pcm_effect(command);
	z80_block(41);
	channel = saved_channel;
	voice = 0x1EEEu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x800u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect_10.timer = 0x20u;
	attenuation = 5u;
	CHANNEL(channel)->fm_effect_10.unread_attenuation = 5u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_10_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass for 32 passes, dying away near its end; then PCM effect 84 again
   on the PCM effects' second channel (unless effect 92 or 93 plays there) and the trill. */
uint32_t fm_effect_10_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, die_away_pass = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect_10.timer;
	CHANNEL(channel)->fm_effect_10.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 0xCu;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(30);
	if (CHANNEL(PCM_EFFECT_B)->sound < 0x12u)
	{
		z80_block(66);
		CHANNEL(PCM_EFFECT_B)->handler = pcm_effect_84_ADDRESS;
		CHANNEL(PCM_EFFECT_B)->sound = 4u;
		CHANNEL(PCM_EFFECT_B)->priority = 0x11u;
	}
	z80_block(43);
	attenuation = 5u;
	CHANNEL(channel)->fm_effect_10.attenuation = 5u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(65);
	CHANNEL(channel)->fm_effect_10.timer = 0x10u;
	CHANNEL(channel)->fm_effect_10.count = 0x10u;
	pitch = 0xE00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(68);
	AS8(CHANNEL(channel)->fm_effect_10.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect_10.pitch, 1) = 0xAu;
	handler = fm_effect_10_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, sliding down; every 16 passes they start again from their pitches, 2 quieter
   for the first 8 steps and 3 for the last, 16 steps. */
uint32_t fm_effect_10_trill(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect_10.timer;
	CHANNEL(channel)->fm_effect_10.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect_10.timer = 0x10u;
	count = CHANNEL(channel)->fm_effect_10.count;
	CHANNEL(channel)->fm_effect_10.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(49);
	attenuation = CHANNEL(channel)->fm_effect_10.attenuation;
	if (((CHANNEL(channel)->fm_effect_10.count >> 3) & 1u) == 0u)
	{
		z80_block(7);
		attenuation++;
	}
	z80_block(43);
	attenuation += 2u;
	CHANNEL(channel)->fm_effect_10.attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(68);
	AS8(CHANNEL(channel)->fm_effect_10.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect_10.base_pitch, 1) = 0xAu;
	pitch = 0x800u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* PCM effect 93 and a falling note, then PCM effect 93 again and 17 bursts of notes that fade. */
uint32_t fm_effect_11(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t voice = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x93u;
	start_pcm_effect(command);
	z80_block(43);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.base_attenuation = 0u;
	voice = 0x1FB2u;
	return fm_effect_drop_and_pcm_start(voice, channel);
}

/* Keys the falling note on with the voice it is given. */
uint32_t fm_effect_drop_and_pcm_start(uint16_t voice, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(17);
	carry = fm_effect_drop_start(voice, channel);
	z80_block(20);
	handler = fm_effect_drop_and_pcm_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall for 32 passes; then PCM effect 93 again and the bursts. */
uint32_t fm_effect_drop_and_pcm(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t saved_channel = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_drop(channel);
	z80_block(39);
	saved_channel = channel;
	command = 0x93u;
	start_pcm_effect(command);
	z80_block(24);
	channel = saved_channel;
	return fm_effect_bursts_start(channel);
}

/* Nothing: its channel stops at once. */
uint32_t fm_effect_12(uint16_t channel)
{
	z80_block(10);
	return stop_channel(channel);
}

/* Effect 11's sound with another voice. */
uint32_t fm_effect_13(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t voice = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x93u;
	start_pcm_effect(command);
	z80_block(53);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.base_attenuation = 0u;
	voice = 0x1FF9u;
	return fm_effect_drop_and_pcm_start(voice, channel);
}

/* A long fall from a high note, quieter step by step, 8 times. */
uint32_t fm_effect_14(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x2025u;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x1700u;
	fm_effect_14_pitch = 0x1700u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(89);
	fm_effect_14_timer = 8u;
	fm_effect_14_steps = 8u;
	fm_effect_14_rounds = 8u;
	attenuation = 0xEu;
	fm_effect_14_attenuation = 0xEu;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_14_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones down each pass (every other pass after the first 8); every 16 passes 5 quieter; it stops
   the eighth time. */
uint32_t fm_effect_14_fall(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_14_timer;
	fm_effect_14_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(63);
		pitch = fm_effect_14_pitch - 0x80u;
		fm_effect_14_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(32);
	fm_effect_14_timer = 2u;
	t2 = fm_effect_14_steps;
	fm_effect_14_steps = t2 - 1u;
	if (t2 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(37);
	fm_effect_14_steps = 8u;
	t3 = fm_effect_14_rounds;
	fm_effect_14_rounds = t3 - 1u;
	if (t3 == 1u)
		return stop_channel(channel);
	z80_block(43);
	attenuation = fm_effect_14_attenuation + 5u;
	fm_effect_14_attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* Starts effect 2D, puts an echo on the channel the FM effects take from the music's channel 7, and
   plays a low note twice that then sinks in growing steps as it fades. */
uint32_t fm_effect_15(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0;
	uint16_t saved_channel = 0;

	z80_block(105);
	CHANNEL(FM_EFFECT_7)->handler = fm_effect_15_echo_ADDRESS;
	CHANNEL(FM_EFFECT_7)->sound = 0x15u;
	CHANNEL(FM_EFFECT_7)->priority = 0x13u;
	saved_channel = channel;
	command = 0x2Du;
	carry = start_fm_effect(command);
	z80_block(33);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect_15.round_attenuation = 0xBu;
	return fm_effect_15_start(channel);
}

/* The 20 steps from the attenuation the effect starts with: the note held 44 passes. */
uint32_t fm_effect_15_start(uint16_t channel)
{
	uint8_t length = 0;

	z80_block(45);
	CHANNEL(channel)->fm_effect_15.fall_step = 4u;
	CHANNEL(channel)->fm_effect_15.count = 0x14u;
	length = 0x2Cu;
	return fm_effect_15_note(channel, length);
}

/* The note keyed on for the passes it is given, at the attenuation the effect starts with. */
uint32_t fm_effect_15_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint8_t round_attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(46);
	CHANNEL(channel)->fm_effect_15.timer = length;
	voice = 0x2094u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x400u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(55);
	round_attenuation = CHANNEL(channel)->fm_effect_15.round_attenuation;
	CHANNEL(channel)->fm_effect_15.attenuation = round_attenuation;
	carry = set_fm_volume(channel, round_attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_15_sink_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* After the first note, the note again for 88 passes; then every 14 passes it sinks by a step that grows
   each time, 3 quieter. */
uint32_t fm_effect_15_sink(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0, fall_step = 0, length = 0;
	uint16_t pitch = 0, step = 0;
	uint8_t timer = 0, count = 0, count_2 = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect_15.timer;
	CHANNEL(channel)->fm_effect_15.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(52);
	CHANNEL(channel)->fm_effect_15.timer = 0xEu;
	count = CHANNEL(channel)->fm_effect_15.count;
	CHANNEL(channel)->fm_effect_15.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(43);
	count_2 = CHANNEL(channel)->fm_effect_15.count;
	length = 0x58u;
	if (count_2 == 0x13u)
		return fm_effect_15_note(channel, length);
	z80_block(111);
	fall_step = CHANNEL(channel)->fm_effect_15.fall_step;
	carry = (uint8_t)(fall_step + 2u) < fall_step;
	fall_step += 2u;
	CHANNEL(channel)->fm_effect_15.fall_step = fall_step;
	step = fall_step;
	pitch = CHANNEL(channel)->fm_effect_15.pitch;
	carry = fm_effect_set_pitch_minus(step, pitch, channel, carry);
	z80_block(55);
	attenuation = CHANNEL(channel)->fm_effect_15.attenuation + 3u;
	CHANNEL(channel)->fm_effect_15.attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* The echo's channel waits 16 passes. */
void fm_effect_15_echo(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect_15.timer = 0x10u;
	handler = fm_effect_15_echo_wait_ADDRESS;
	set_channel_handler(handler, channel);
}

/* Then plays the same steps, at attenuation 23 (the first at 11). */
uint32_t fm_effect_15_echo_wait(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect_15.timer;
	CHANNEL(channel)->fm_effect_15.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(29);
	CHANNEL(channel)->fm_effect_15.round_attenuation = 0x17u;
	return fm_effect_15_start(channel);
}

/* A very low note that rises slowly for 496 passes, fading in its last 256; the same starts 80 passes
   later on the channel the FM effects take from the music's channel 6. */
uint32_t fm_effect_16(uint16_t channel)
{
	z80_block(66);
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_16_echo_ADDRESS;
	CHANNEL(FM_EFFECT_6)->sound = 0x16u;
	CHANNEL(FM_EFFECT_6)->priority = 0x12u;
	return fm_effect_16_rise(channel);
}

/* The rise at attenuation 12, an eighth of a semitone a pass, for 496 passes. */
uint32_t fm_effect_16_rise(uint16_t channel)
{
	uint16_t length = 0;

	z80_block(48);
	CHANNEL(channel)->fm_effect.rise_step = 8u;
	CHANNEL(channel)->fm_effect.attenuation = 0xCu;
	length = 0x1F0u;
	return fm_effect_long_rise_start(length, channel);
}

/* Keys a very low note on for the passes it is given. */
uint32_t fm_effect_long_rise_start(uint16_t length, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(84);
	CHANNEL(channel)->fm_effect_long_rise.long_timer = length;
	CHANNEL(channel)->fm_effect_long_rise.count = 8u;
	voice = 0x2141u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x100u;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_long_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the passes down; each pass the note rises by the effect's step and, in the last 256 passes, it
   gets 1 quieter every 8. */
uint32_t fm_effect_long_rise(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t timer_high = 0, timer_low = 0;
	uint16_t step = 0;
	uint8_t count = 0;
	uint16_t t1 = 0;

	z80_block(100);
	t1 = CHANNEL(channel)->fm_effect_long_rise.long_timer - 1u;
	timer_high = (uint8_t)(t1 >> 8);
	timer_low = (uint8_t)t1;
	AS8(CHANNEL(channel)->fm_effect_long_rise.long_timer) = timer_low;
	AT8(CHANNEL(channel)->fm_effect_long_rise.long_timer, 1) = timer_high;
	if ((timer_high | timer_low) == 0u)
		return stop_channel(channel);
	z80_block(18);
	if (timer_high == 0u)
	{
		z80_block(33);
		count = CHANNEL(channel)->fm_effect_long_rise.count;
		CHANNEL(channel)->fm_effect_long_rise.count = count - 1u;
		if (count == 1u)
		{
			z80_block(59);
			CHANNEL(channel)->fm_effect_long_rise.count = 8u;
			CHANNEL(channel)->fm_effect_long_rise.attenuation++;
			carry = set_fm_volume_from_record(channel);
		}
	}
	z80_block(36);
	step = CHANNEL(channel)->fm_effect_long_rise.rise_step;
	fm_effect_move_pitch(channel, &step, &carry);
	return carry;
}

/* The echo's channel: keyed off at once, then waits 80 passes. */
uint32_t fm_effect_16_echo(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(17);
	fm_key_off(channel);
	z80_block(17);
	carry = fm_fast_release(channel);
	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 0x50u;
	handler = fm_effect_16_echo_wait_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Then the same rise. */
uint32_t fm_effect_16_echo_wait(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return fm_effect_16_rise(channel);
}

/* Zigzag notes, each followed by a quieter echo of the top note: at the top for the first steps, then
   sinking and fading; 20 steps. */
uint32_t fm_effect_17(uint16_t channel)
{
	z80_block(20);
	fm_effect_17_steps = 0x14u;
	return fm_effect_17_from_top(channel);
}

/* Starts again from the top note at attenuation 7. */
uint32_t fm_effect_17_from_top(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t pitch = 0, voice = 0;

	z80_block(47);
	fm_effect_17_attenuation = 7u;
	voice = 0x21DDu;
	carry = load_fm_voice(voice, channel);
	z80_block(10);
	pitch = 0x500u;
	return fm_effect_17_note(pitch, channel);
}

/* A note at the pitch it is given, for 9 passes. */
uint32_t fm_effect_17_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0;

	z80_block(49);
	fm_effect_17_pitch = pitch;
	fm_effect_17_base_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(33);
	fm_effect_17_timer = 9u;
	attenuation = fm_effect_17_attenuation;
	return fm_effect_17_key_on(channel, attenuation);
}

/* Keys the note on at the attenuation it is given. */
uint32_t fm_effect_17_key_on(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(17);
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_17_zigzag_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* One semitone up and two down in turn; then the next step: an odd one is the top note again 20 quieter;
   an even one starts at the top again while 10 or more steps are left, after that two semitones below
   the last and 8 quieter. */
uint32_t fm_effect_17_zigzag(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0, voice = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_17_timer;
	fm_effect_17_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(48);
		pitch = fm_effect_17_pitch;
		step = 0x40u;
		if ((fm_effect_17_timer & 1u) == 0u)
		{
			z80_block(10);
			step = 0xFF80u;
		}
		z80_block(37);
		pitch += step;
		fm_effect_17_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(41);
	fm_effect_17_timer = 8u;
	t2 = fm_effect_17_steps;
	fm_effect_17_steps = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(22);
	if ((fm_effect_17_steps & 1u) == 0u)
	{
		z80_block(24);
		if (fm_effect_17_steps >= 0xAu)
			return fm_effect_17_from_top(channel);
		z80_block(27);
		voice = 0x21DDu;
		carry = load_fm_voice(voice, channel);
		z80_block(84);
		attenuation = fm_effect_17_attenuation;
		carry = (uint8_t)(attenuation + 8u) < attenuation;
		fm_effect_17_attenuation = attenuation + 8u;
		pitch = fm_effect_17_base_pitch - 0x80u - carry;
		return fm_effect_17_note(pitch, channel);
	}
	z80_block(43);
	pitch = 0x500u;
	fm_effect_17_pitch = 0x500u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(30);
	attenuation = fm_effect_17_attenuation + 0x14u;
	return fm_effect_17_key_on(channel, attenuation);
}

/* Effect 0C's zigzag notes without the PCM effect: 16 steps, another voice, quieter, sinking sooner and
   fading faster. */
uint32_t fm_effect_18(uint16_t channel)
{
	z80_block(153);
	CHANNEL(channel)->fm_effect_18.period = 9u;
	CHANNEL(channel)->fm_effect_18.steps = 0x10u;
	CHANNEL(channel)->fm_effect_18.round_attenuation = 7u;
	AS8(CHANNEL(channel)->fm_effect_18.voice) = 0x80u;
	AT8(CHANNEL(channel)->fm_effect_18.voice, 1) = 0x22u;
	CHANNEL(channel)->fm_effect_18.fade_start = 0xAu;
	CHANNEL(channel)->fm_effect_18.fading_step = 8u;
	return fm_effect_zigzag_steps_start(channel);
}

/* Starts effect 1E (its echo) and plays a ladder of notes rising a semitone a step, 19 steps. */
uint32_t fm_effect_19(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x1Eu;
	carry = start_fm_effect(command);
	z80_block(21);
	channel = saved_channel;
	attenuation = 0u;
	return fm_effect_ladder_start(channel, attenuation);
}

/* Starts the 19 steps at the attenuation it is given. */
uint32_t fm_effect_ladder_start(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t pitch = 0, voice = 0;

	z80_block(65);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.count = 0x13u;
	voice = 0x22B9u;
	carry = load_fm_voice(voice, channel);
	z80_block(48);
	pitch = 0xF00u;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 0xFu;
	return fm_effect_ladder_step(pitch, channel);
}

/* A step's note at the pitch it is given, for 10 passes. */
uint32_t fm_effect_ladder_step(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0;

	z80_block(17);
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 0xAu;
	handler = fm_effect_ladder_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* In a step's last two passes the note before it sounds again, 16 quieter; then the next step a semitone
   higher (1 quieter every other step in the last 10). */
uint32_t fm_effect_ladder(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, pitch_high = 0, pitch_low = 0;
	uint16_t base_pitch = 0, ignored16 = 0, pitch = 0;
	uint8_t timer = 0, timer_2 = 0, count = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(31);
		timer_2 = CHANNEL(channel)->fm_effect.timer;
		carry = timer_2 > 2u;
		if (2u != timer_2)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(55);
		base_pitch = CHANNEL(channel)->fm_effect.base_pitch;
		set_fm_pitch(base_pitch, channel, &ignored16, &carry);
		z80_block(36);
		attenuation = CHANNEL(channel)->fm_effect.attenuation + 0x10u;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(36);
	if (CHANNEL(channel)->fm_effect.count < 0xAu)
	{
		z80_block(30);
		if ((CHANNEL(channel)->fm_effect.count & 1u) == 0u)
		{
			z80_block(23);
			CHANNEL(channel)->fm_effect.attenuation++;
		}
	}
	z80_block(107);
	pitch_low = (uint8_t)CHANNEL(channel)->fm_effect.pitch;
	pitch_high = (uint8_t)(CHANNEL(channel)->fm_effect.pitch >> 8u);
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = pitch_low;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = pitch_high;
	t1 = (((uint32_t)pitch_high << 8) | pitch_low) + 0x40u;
	pitch = t1;
	return fm_effect_ladder_step(pitch, channel);
}

/* Three channels: effect 13's sound on the channel the FM effects take from the music's channel 6, 8
   falling notes and bursts on the one from channel 7, and here a falling note then 32 bursts of notes. */
uint32_t fm_effect_1a(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(152);
	CHANNEL(FM_EFFECT_7)->handler = fm_effect_1a_second_start_ADDRESS;
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_13_ADDRESS;
	CHANNEL(FM_EFFECT_7)->sound = 0x1Au;
	CHANNEL(FM_EFFECT_6)->sound = 0x13u;
	CHANNEL(FM_EFFECT_7)->priority = 0x30u;
	CHANNEL(FM_EFFECT_6)->priority = 0x30u;
	voice = 0x1EEEu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x1080u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x680u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x18u;
	attenuation = 0u;
	CHANNEL(channel)->fm_effect.base_attenuation = 0u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_1a_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass for 24 passes, dying away near its end; then the bursts. */
uint32_t fm_effect_1a_fall(uint16_t channel)
{
	uint8_t attenuation = 0, die_away_pass = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
	{
		z80_block(26);
		CHANNEL(channel)->fm_effect.rounds = 0x20u;
		attenuation = 0u;
		return fm_effect_1a_burst(channel, attenuation);
	}
	z80_block(17);
	die_away_pass = 8u;
	return fm_effect_fall(channel, die_away_pass);
}

/* A burst of 5 steps at the attenuation it is given: two notes in turn sliding down, 6 passes a step. */
uint32_t fm_effect_1a_burst(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.round_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = 6u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0xD80u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(68);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0x80u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xAu;
	handler = fm_effect_1a_burst_step_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Each step the two notes at lower pitches that move with the random bytes' place, 1 quieter; then the
   next burst, 1 quieter while 16 or more are left, then 2. */
uint32_t fm_effect_1a_burst_step(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t base_pitch = 0, offset = 0, pitch = 0;
	uint8_t timer = 0, count = 0, rounds = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 6u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count != 1u)
	{
		z80_block(17);
		fm_effect_quieter_pool_offset(channel, &offset, &carry);
		z80_block(83);
		t1 = 0xA80u - offset - carry;
		carry = offset + carry > 0xA80u;
		base_pitch = t1;
		CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
		pitch = 0x680u;
		return fm_effect_set_pitch_minus(offset, pitch, channel, carry);
	}
	z80_block(33);
	rounds = CHANNEL(channel)->fm_effect.rounds;
	CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
	if (rounds == 1u)
		return stop_channel(channel);
	z80_block(49);
	attenuation = CHANNEL(channel)->fm_effect.round_attenuation;
	if (((CHANNEL(channel)->fm_effect.rounds >> 4) & 1u) == 0u)
	{
		z80_block(7);
		attenuation++;
	}
	z80_block(17);
	attenuation++;
	return fm_effect_1a_burst(channel, attenuation);
}

/* A pass of a falling note: two semitones down, except on the pass of the timer it is given, when the
   note dies away (32 over the base attenuation). */
uint32_t fm_effect_fall(uint16_t channel, uint8_t die_away_pass)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t step = 0;

	z80_block(29);
	if (die_away_pass == CHANNEL(channel)->fm_effect.timer)
	{
		z80_block(36);
		attenuation = CHANNEL(channel)->fm_effect.base_attenuation + 0x20u;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(20);
	step = 0xFF80u;
	fm_effect_move_pitch(channel, &step, &carry);
	return carry;
}

/* A pass of a trill whose two notes slide two semitones down each time. */
uint32_t fm_effect_trill_slide_down(uint16_t channel)
{
	uint16_t step = 0;

	z80_block(10);
	step = 0xFF80u;
	return fm_effect_trill_slide(step, channel);
}

/* Two notes in turn, both sliding: on odd passes the note's pitch moves by the step, on even ones the
   base pitch moves by it and sounds. */
uint32_t fm_effect_trill_slide(uint16_t step, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t pitch_high = 0, pitch_low = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint16_t t1 = 0;

	z80_block(30);
	if ((CHANNEL(channel)->fm_effect.timer & 1u) != 0u)
	{
		fm_effect_move_pitch(channel, &step, &carry);
		return carry;
	}
	z80_block(97);
	t1 = CHANNEL(channel)->fm_effect.base_pitch + step;
	pitch_high = (uint8_t)(t1 >> 8);
	pitch_low = (uint8_t)t1;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = pitch_low;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = pitch_high;
	pitch = ((uint32_t)pitch_high << 8) | pitch_low;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* The note 1 quieter, and an offset for the next pitches from the random bytes' place (24 to 28
   semitones: the place is taken for the byte). */
void fm_effect_quieter_pool_offset(uint16_t channel, uint16_t *p_offset, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t offset = 0, random_place = 0;

	z80_block(40);
	CHANNEL(channel)->fm_effect.attenuation++;
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	random_place = next_random_byte();
	z80_block(29);
	carry = 0u;
	offset = (((random_place >> 8) & 0xFu) << 8) | (uint8_t)random_place;
	*p_offset = offset;
	*p_carry = carry;
}

/* Keeps the pitch it is given as both the note's pitch and the base pitch. */
void fm_effect_keep_pitch(uint16_t pitch, uint16_t channel)
{
	z80_block(86);
	CHANNEL(channel)->fm_effect.pitch = pitch;
	CHANNEL(channel)->fm_effect.base_pitch = pitch;
}

/* The second channel's part: 8 falling notes, then 32 bursts. */
uint32_t fm_effect_1a_second_start(uint16_t channel)
{
	z80_block(19);
	CHANNEL(channel)->fm_effect.count = 8u;
	return fm_effect_1a_second_note(channel);
}

/* A note keyed on high that falls for 32 passes. */
uint32_t fm_effect_1a_second_note(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x1EEEu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x1100u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x700u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x20u;
	attenuation = 4u;
	CHANNEL(channel)->fm_effect.base_attenuation = 4u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_1a_second_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall, two semitones a pass, dying away when 16 passes are left; after the 8 notes, the bursts. */
uint32_t fm_effect_1a_second_fall(uint16_t channel)
{
	uint8_t attenuation = 0, die_away_pass = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 0x10u;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count != 1u)
		return fm_effect_1a_second_note(channel);
	z80_block(26);
	CHANNEL(channel)->fm_effect.rounds = 0x20u;
	attenuation = 6u;
	return fm_effect_1a_second_burst(channel, attenuation);
}

/* A burst of 5 steps at the attenuation it is given: two notes in turn sliding down, 6 passes a step. */
uint32_t fm_effect_1a_second_burst(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.round_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = 6u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0xE00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(68);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xBu;
	handler = fm_effect_1a_second_burst_step_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Each step the two notes at lower pitches that move with the random bytes' place, 1 quieter; then the
   next burst, 1 louder for the first two, then 2 quieter. */
uint32_t fm_effect_1a_second_burst_step(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t base_pitch = 0, offset = 0, pitch = 0;
	uint8_t timer = 0, count = 0, rounds = 0, rounds_2 = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 6u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count != 1u)
	{
		z80_block(17);
		fm_effect_quieter_pool_offset(channel, &offset, &carry);
		z80_block(83);
		t1 = 0xB00u - offset - carry;
		carry = offset + carry > 0xB00u;
		base_pitch = t1;
		CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
		pitch = 0x700u;
		return fm_effect_set_pitch_minus(offset, pitch, channel, carry);
	}
	z80_block(33);
	rounds = CHANNEL(channel)->fm_effect.rounds;
	CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
	if (rounds == 1u)
		return stop_channel(channel);
	z80_block(55);
	rounds_2 = CHANNEL(channel)->fm_effect.rounds;
	carry = rounds_2 < 0x1Eu;
	attenuation = CHANNEL(channel)->fm_effect.round_attenuation;
	if (rounds_2 < 0x1Eu)
	{
		z80_block(17);
		attenuation += 2u;
		return fm_effect_1a_second_burst(channel, attenuation);
	}
	z80_block(17);
	attenuation = attenuation - 1u - carry;
	return fm_effect_1a_second_burst(channel, attenuation);
}

/* A very low note that rises two semitones a pass up to a ceiling and stays there, then quieter; 320
   passes in all. */
uint32_t fm_effect_1b(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x24F6u;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x240u;
	fm_effect_1b_pitch = 0x240u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(44);
	fm_effect_1b_timer = 0xB0u;
	attenuation = 9u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_1b_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Rises for 176 passes (staying once it is at its highest); then much quieter. */
uint32_t fm_effect_1b_rise(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, semitone = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_1b_timer;
	fm_effect_1b_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(34);
		fm_effect_1b_timer = 0x90u;
		attenuation = 0x2Du;
		carry = set_fm_volume(channel, attenuation);
		z80_block(20);
		handler = fm_effect_1b_hold_ADDRESS;
		set_channel_handler(handler, channel);
		return carry;
	}
	z80_block(53);
	pitch = fm_effect_1b_pitch + 0x80u;
	semitone = (uint8_t)(pitch >> 8);
	carry = semitone < 0x11u;
	if (semitone >= 0x11u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(26);
	fm_effect_1b_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* Holds the note 144 passes, a little louder after the first 8. */
uint32_t fm_effect_1b_hold(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_1b_timer;
	fm_effect_1b_timer = t1 - 1u;
	if (t1 == 1u)
		return stop_channel(channel);
	z80_block(19);
	timer = fm_effect_1b_timer;
	carry = timer < 0x88u;
	if (timer != 0x88u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(17);
	attenuation = 0x19u;
	return set_fm_volume(channel, attenuation);
}

/* Repeated notes in two rounds, with an echo 22 passes later on the channel the FM effects take from the
   music's channel 6. */
uint32_t fm_effect_1c(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(73);
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_1c_echo_ADDRESS;
	CHANNEL(FM_EFFECT_6)->sound = 0x1Cu;
	CHANNEL(FM_EFFECT_6)->priority = 0x14u;
	attenuation = 5u;
	return fm_effect_pulses_start(channel, attenuation);
}

/* Two rounds of 14 notes, from the attenuation it is given. */
uint32_t fm_effect_pulses_start(uint16_t channel, uint8_t attenuation)
{
	z80_block(19);
	CHANNEL(channel)->fm_effect.rounds = 2u;
	return fm_effect_pulses_round(channel, attenuation);
}

/* A round of 14 notes at the attenuation it is given. */
uint32_t fm_effect_pulses_round(uint16_t channel, uint8_t attenuation)
{
	z80_block(38);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.count = 0xEu;
	return fm_effect_pulse(channel);
}

/* A note keyed on for 22 passes. */
uint32_t fm_effect_pulse(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(46);
	CHANNEL(channel)->fm_effect.timer = 0x16u;
	voice = 0x2566u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xA00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_pulses_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The first 4 notes are cut 8 passes before their end by the next; the later ones fall silent for their
   last 8 passes; the last of a round is 8 quieter; the second round at attenuation 29. */
uint32_t fm_effect_pulses(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint8_t timer_2 = 0, count = 0, rounds = 0;

	z80_block(33);
	timer_2 = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer_2 - 1u;
	if (timer_2 == 1u)
	{
		z80_block(33);
		count = CHANNEL(channel)->fm_effect.count;
		CHANNEL(channel)->fm_effect.count = count - 1u;
		if (count == 1u)
		{
			z80_block(33);
			rounds = CHANNEL(channel)->fm_effect.rounds;
			CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
			if (rounds == 1u)
				return stop_channel(channel);
			z80_block(17);
			attenuation = 0x1Du;
			return fm_effect_pulses_round(channel, attenuation);
		}
		z80_block(36);
		if (CHANNEL(channel)->fm_effect.count != 1u)
			return fm_effect_pulse(channel);
		z80_block(55);
		CHANNEL(channel)->fm_effect.attenuation += 8u;
		return fm_effect_pulse(channel);
	}
	z80_block(31);
	timer = CHANNEL(channel)->fm_effect.timer;
	carry = timer < 8u;
	if (timer != 8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(59);
	CHANNEL(channel)->fm_effect.count--;
	if (CHANNEL(channel)->fm_effect.count >= 0xAu)
		return fm_effect_pulse(channel);
	z80_block(59);
	CHANNEL(channel)->fm_effect.count++;
	attenuation = CHANNEL(channel)->fm_effect.attenuation + 0x30u;
	return set_fm_volume(channel, attenuation);
}

/* The echo's channel waits 22 passes. */
void fm_effect_1c_echo(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 0x16u;
	handler = fm_effect_1c_echo_wait_ADDRESS;
	set_channel_handler(handler, channel);
}

/* Then the repeated notes at attenuation 8. */
uint32_t fm_effect_1c_echo_wait(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(17);
	attenuation = 8u;
	return fm_effect_pulses_start(channel, attenuation);
}

/* A three-note arpeggio that rises and rings, with an echo 8 passes later on the channel the FM effects
   take from the music's channel 7, a little sharp. */
uint32_t fm_effect_1d(uint16_t channel)
{
	uint16_t pitch = 0;

	z80_block(88);
	CHANNEL(FM_EFFECT_7)->handler = fm_effect_1d_echo_ADDRESS;
	CHANNEL(FM_EFFECT_7)->sound = 0x1Du;
	CHANNEL(FM_EFFECT_7)->priority = 0x1Du;
	CHANNEL(channel)->fm_effect.attenuation = 0u;
	pitch = pcm_instruments_ADDRESS - 0x2u;
	return fm_effect_1d_arpeggio(pitch, channel);
}

/* The arpeggio from the pitch it is given, 26 passes a note, with its voice. */
uint32_t fm_effect_1d_arpeggio(uint16_t pitch, uint16_t channel)
{
	uint16_t voice = 0;

	z80_block(29);
	CHANNEL(channel)->fm_effect.period = 0x1Au;
	voice = 0x2611u;
	return fm_effect_arpeggio_start(voice, pitch, channel);
}

/* The arpeggio with the voice it is given: three notes from the pitch it is given. */
uint32_t fm_effect_arpeggio_start(uint16_t voice, uint16_t pitch, uint16_t channel)
{
	z80_block(57);
	CHANNEL(channel)->fm_effect.voice = voice;
	CHANNEL(channel)->fm_effect.count = 3u;
	return fm_effect_arpeggio_note(pitch, channel);
}

/* A note at the pitch it is given, keyed on for the effect's period. */
uint32_t fm_effect_arpeggio_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, voice = 0;

	z80_block(131);
	CHANNEL(channel)->fm_effect.pitch = pitch;
	CHANNEL(channel)->fm_effect.timer = CHANNEL(channel)->fm_effect.period;
	voice = CHANNEL(channel)->fm_effect.voice;
	carry = load_fm_voice(voice, channel);
	z80_block(55);
	pitch = CHANNEL(channel)->fm_effect.pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_arpeggio_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Holds the note; then the next one, a fifth higher, then a fourth (an octave over the first); the last
   rings 192 passes. */
uint32_t fm_effect_arpeggio(uint16_t channel, uint32_t carry)
{
	uint16_t handler = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
	{
		z80_block(39);
		CHANNEL(channel)->fm_effect.timer = 0xC0u;
		handler = fm_effect_arpeggio_ring_ADDRESS;
		set_channel_handler(handler, channel);
		return carry;
	}
	z80_block(78);
	pitch = CHANNEL(channel)->fm_effect.pitch;
	step = 0x1C0u;
	if (CHANNEL(channel)->fm_effect.count & 1u)
	{
		z80_block(10);
		step = 0x140u;
	}
	z80_block(21);
	pitch += step;
	return fm_effect_arpeggio_note(pitch, channel);
}

/* The last note rings, then the channel stops. */
uint32_t fm_effect_arpeggio_ring(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
		return stop_channel(channel);
	z80_block(10);
	return carry;
}

/* The echo's channel waits 8 passes. */
void fm_effect_1d_echo(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 8u;
	handler = fm_effect_1d_echo_wait_ADDRESS;
	set_channel_handler(handler, channel);
}

/* Then the arpeggio 8 quieter and a sixty-fourth of a semitone sharp. */
uint32_t fm_effect_1d_echo_wait(uint16_t channel, uint32_t carry)
{
	uint16_t pitch = 0;
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(39);
	CHANNEL(channel)->fm_effect.attenuation = 8u;
	pitch = pcm_instruments_ADDRESS - 0x1u;
	return fm_effect_1d_arpeggio(pitch, channel);
}

/* Effect 19's ladder 48 passes later, 12 quieter, on its own channel (effect 19 starts it). */
uint32_t fm_effect_1e(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(17);
	fm_key_off(channel);
	z80_block(17);
	carry = fm_fast_release(channel);
	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 0x30u;
	handler = fm_effect_1e_wait_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Waits the 48 passes, then the ladder. */
uint32_t fm_effect_1e_wait(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(17);
	attenuation = 0xCu;
	return fm_effect_ladder_start(channel, attenuation);
}

/* A note that rises two semitones a pass, then a high note held and struck again as it fades. */
uint32_t fm_effect_1f(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x26DDu;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x500u;
	fm_effect_1f_pitch = 0x500u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(24);
	attenuation = 1u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_1f_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Rises for 22 passes; then the high note. */
uint32_t fm_effect_1f_rise(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0, voice = 0;

	z80_block(58);
	pitch = fm_effect_1f_pitch + 0x80u;
	if (pitch >> 8 != 0x10u)
	{
		z80_block(26);
		fm_effect_1f_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(47);
	fm_effect_1f_repeats = 0x10u;
	voice = 0x26DDu;
	carry = load_fm_voice(voice, channel);
	z80_block(7);
	attenuation = 1u;
	return fm_effect_1f_strike(channel, attenuation);
}

/* Strikes the high note at the attenuation it is given, for 32 passes. */
uint32_t fm_effect_1f_strike(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(30);
	fm_effect_1f_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(37);
	fm_effect_1f_timer = 0x20u;
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_1f_hold_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Holds it 256 passes, then strikes it again every 32 passes, 1 quieter each time, 7 times. */
uint32_t fm_effect_1f_hold(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0, repeats = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(26);
	t1 = fm_effect_1f_timer;
	fm_effect_1f_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(41);
	fm_effect_1f_timer = 0x20u;
	t2 = fm_effect_1f_repeats;
	fm_effect_1f_repeats = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(19);
	repeats = fm_effect_1f_repeats;
	carry = repeats < 8u;
	if (repeats >= 8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(30);
	attenuation = fm_effect_1f_attenuation + 1u;
	return fm_effect_1f_strike(channel, attenuation);
}

/* Effect 1C's repeated notes alone, at attenuation 8. */
uint32_t fm_effect_20(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(17);
	attenuation = 8u;
	return fm_effect_pulses_start(channel, attenuation);
}

/* A slow rise, then a held note that fades very slowly until effect 2B stops it. */
uint32_t fm_effect_21(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(47);
	fm_effect_21_on = 1u;
	voice = 0x276Cu;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x400u;
	fm_effect_21_pitch = 0x400u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(24);
	attenuation = 9u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_21_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* A quarter of a semitone up each pass for 112 passes; then the held note, keyed on again. */
uint32_t fm_effect_21_rise(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(58);
	pitch = fm_effect_21_pitch + 0x10u;
	if (pitch >> 8 != 0xBu)
	{
		z80_block(26);
		fm_effect_21_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(53);
	fm_effect_21_timer = 0xF00u;
	voice = 0x276Cu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = fm_note_pitches_ADDRESS - 0xCu;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(37);
	attenuation = 0x11u;
	fm_effect_21_attenuation = 0x11u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_21_hold_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Every 3840 passes 1 quieter, until it is silent or its flag is cleared. */
uint32_t fm_effect_21_hold(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t timer = 0;

	z80_block(27);
	if (fm_effect_21_on == 0u)
		return stop_channel(channel);
	z80_block(51);
	timer = fm_effect_21_timer - 1u;
	fm_effect_21_timer = timer;
	carry = 0u;
	if ((uint8_t)((timer >> 8) | timer) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(64);
	fm_effect_21_timer = 0xF00u;
	attenuation = fm_effect_21_attenuation + 1u;
	if (((attenuation >> 6) & 1u) != 0u)
		return stop_channel(channel);
	z80_block(23);
	fm_effect_21_attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* Takes every FM channel: here a slow rise at once, and on the music's channels 1 to 5 and the FM
   effects' channels 6 and 7 the same rise in turn, 80 passes apart and 3 quieter each. */
uint32_t fm_effect_22(uint16_t channel)
{
	uint8_t count = 0;
	uint16_t record_size = 0;
	uint16_t saved_channel = 0;

	z80_block(228);
	CHANNEL(MUSIC_FM_1)->handler = fm_effect_22_on_channel_1_ADDRESS;
	CHANNEL(MUSIC_FM_2)->handler = fm_effect_22_on_channel_2_ADDRESS;
	CHANNEL(MUSIC_FM_3)->handler = fm_effect_22_on_channel_3_ADDRESS;
	CHANNEL(MUSIC_FM_4)->handler = fm_effect_22_on_channel_4_ADDRESS;
	CHANNEL(MUSIC_FM_5)->handler = fm_effect_22_on_channel_5_ADDRESS;
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_22_on_channel_6_ADDRESS;
	CHANNEL(FM_EFFECT_7)->handler = fm_effect_22_on_channel_7_ADDRESS;
	saved_channel = channel;
	count = 7u;
	channel = MUSIC_FM_1;
	record_size = 0x40u;
	for (;;)
	{
		z80_block(75);
		CHANNEL(channel)->sound = 0x22u;
		CHANNEL(channel)->priority = 0x12u;
		channel += record_size;
		count--;
		if (count == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(33);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.attenuation = 0x14u;
	return fm_effect_22_rise(channel);
}

/* The slow rise: 704 passes, five sixty-fourths of a semitone a pass. */
uint32_t fm_effect_22_rise(uint16_t channel)
{
	uint16_t length = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect.rise_step = 5u;
	length = 0x2C0u;
	return fm_effect_long_rise_start(length, channel);
}

/* The part on channel 7: the rise after 7 times 80 passes. */
uint32_t fm_effect_22_on_channel_7(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(17);
	waits = 7u;
	return fm_effect_22_delay(channel, waits);
}

/* The part on channel 6: the rise after 6 times 80 passes. */
uint32_t fm_effect_22_on_channel_6(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(17);
	waits = 6u;
	return fm_effect_22_delay(channel, waits);
}

/* The part on the music's channel 5: the rise after 5 times 80 passes. */
uint32_t fm_effect_22_on_channel_5(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(17);
	waits = 5u;
	return fm_effect_22_delay(channel, waits);
}

/* The part on the music's channel 4: the rise after 4 times 80 passes. */
uint32_t fm_effect_22_on_channel_4(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(17);
	waits = 4u;
	return fm_effect_22_delay(channel, waits);
}

/* The part on the music's channel 3: the rise after 3 times 80 passes. */
uint32_t fm_effect_22_on_channel_3(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(17);
	waits = 3u;
	return fm_effect_22_delay(channel, waits);
}

/* The part on the music's channel 2: the rise after 2 times 80 passes. */
uint32_t fm_effect_22_on_channel_2(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(17);
	waits = 2u;
	return fm_effect_22_delay(channel, waits);
}

/* The part on the music's channel 1: the rise after 80 passes. */
uint32_t fm_effect_22_on_channel_1(uint16_t channel)
{
	uint8_t waits = 0;

	z80_block(7);
	waits = 1u;
	return fm_effect_22_delay(channel, waits);
}

/* Waits as many times 80 passes as it is given, at an attenuation of 20 plus 3 for each. */
uint32_t fm_effect_22_delay(uint16_t channel, uint8_t waits)
{
	uint32_t carry = 0;
	uint16_t handler = 0;
	uint8_t count = 0;

	z80_block(111);
	CHANNEL(channel)->fm_effect.count = waits;
	waits = ((uint32_t)waits << 1) + 0x14u;
	count = CHANNEL(channel)->fm_effect.count;
	carry = (uint8_t)(waits + count) < waits;
	CHANNEL(channel)->fm_effect.attenuation = waits + count;
	CHANNEL(channel)->fm_effect.timer = 0x50u;
	handler = fm_effect_22_wait_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the waits of 80 passes; then the rise. */
uint32_t fm_effect_22_wait(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0, count = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(47);
	CHANNEL(channel)->fm_effect.timer = 0x50u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return fm_effect_22_rise(channel);
}

/* A long fall from a high note, quieter step by step, 12 times. */
uint32_t fm_effect_23(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x289Du;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x1700u;
	fm_effect_23_pitch = 0x1700u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(89);
	fm_effect_23_timer = 8u;
	fm_effect_23_steps = 8u;
	fm_effect_23_rounds = 0xCu;
	attenuation = 6u;
	fm_effect_23_attenuation = 6u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_23_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* A little over two semitones down each pass (two passes of every three after the first 8); every 24
   passes 4 quieter; it stops the twelfth time. */
uint32_t fm_effect_23_fall(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_23_timer;
	fm_effect_23_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(63);
		pitch = fm_effect_23_pitch - 0x90u;
		fm_effect_23_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(32);
	fm_effect_23_timer = 3u;
	t2 = fm_effect_23_steps;
	fm_effect_23_steps = t2 - 1u;
	if (t2 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(37);
	fm_effect_23_steps = 8u;
	t3 = fm_effect_23_rounds;
	fm_effect_23_rounds = t3 - 1u;
	if (t3 == 1u)
		return stop_channel(channel);
	z80_block(43);
	attenuation = fm_effect_23_attenuation + 4u;
	fm_effect_23_attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* PCM effect 93 and a falling note, then 16 bursts of low notes, each 4 quieter. */
uint32_t fm_effect_24(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x93u;
	start_pcm_effect(command);
	z80_block(41);
	channel = saved_channel;
	voice = 0x290Cu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xC00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(110);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 6u;
	CHANNEL(channel)->fm_effect.timer = 0x10u;
	attenuation = 2u;
	CHANNEL(channel)->fm_effect.base_attenuation = 2u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_24_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass for 16 passes, dying away near its end; then the bursts. */
uint32_t fm_effect_24_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, die_away_pass = 0;
	uint16_t voice = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 8u;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(94);
	CHANNEL(channel)->fm_effect.rounds = 0x10u;
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 6u;
	voice = 0x290Cu;
	carry = load_fm_voice(voice, channel);
	z80_block(7);
	attenuation = 2u;
	return fm_effect_24_burst(channel, attenuation);
}

/* A burst of 5 steps at the attenuation it is given, keyed on low. */
uint32_t fm_effect_24_burst(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.round_attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = 4u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0x200u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_24_burst_step_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The note falls two semitones a pass; every 4 passes it jumps to a pitch that moves with the random
   bytes' place, 1 quieter; then the next burst, 4 quieter. */
uint32_t fm_effect_24_burst_step(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, offset = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0, rounds = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(20);
		step = 0xFF80u;
		fm_effect_move_pitch(channel, &step, &carry);
		return carry;
	}
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 4u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
	{
		z80_block(33);
		rounds = CHANNEL(channel)->fm_effect.rounds;
		CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
		if (rounds == 1u)
			return stop_channel(channel);
		z80_block(36);
		attenuation = CHANNEL(channel)->fm_effect.round_attenuation + 4u;
		return fm_effect_24_burst(channel, attenuation);
	}
	z80_block(17);
	fm_effect_quieter_pool_offset(channel, &offset, &carry);
	z80_block(98);
	t1 = 0xA00u - offset - carry;
	carry = offset + carry > 0xA00u;
	pitch = t1;
	CHANNEL(channel)->fm_effect.pitch = pitch;
	pitch = 0x600u - offset - carry;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* A short fall, then a held note that bends up a little. */
uint32_t fm_effect_25(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x29D2u;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x1100u;
	fm_effect_25_pitch = 0x1100u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(24);
	attenuation = 0u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(40);
	fm_effect_25_timer = 8u;
	handler = fm_effect_25_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* A semitone down each pass for 8 passes; then another note, held 240 passes. */
uint32_t fm_effect_25_fall(uint16_t channel, uint32_t carry)
{
	uint16_t handler = 0, ignored16 = 0, pitch = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_25_timer;
	fm_effect_25_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(67);
		pitch = fm_effect_25_pitch - 0x40u - carry;
		fm_effect_25_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(53);
	fm_effect_25_timer = 0xF0u;
	pitch = 0xF40u;
	fm_effect_25_pitch = 0xF40u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(20);
	handler = fm_effect_25_bend_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* After 24 passes the note bends up slowly (a thirty-second of a semitone a pass, 20 passes); the
   channel stops when the 240 passes end. */
uint32_t fm_effect_25_bend(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t pitch_fraction = 0, timer = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_25_timer;
	fm_effect_25_timer = t1 - 1u;
	if (t1 == 1u)
		return stop_channel(channel);
	z80_block(19);
	timer = fm_effect_25_timer;
	carry = timer < 0xD8u;
	if (timer >= 0xD8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(32);
	pitch = fm_effect_25_pitch;
	pitch_fraction = (uint8_t)pitch;
	carry = pitch_fraction < 0x68u;
	if (pitch_fraction == 0x68u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(47);
	pitch += 2u;
	fm_effect_25_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* Starts effect 28 and plays a tune of 9 notes (the first, then the 8 of fm_effect_26_pitches from the
   last), each with an echo of the one before, twice; then effect 34 again if it was playing. */
uint32_t fm_effect_26(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x28u;
	carry = start_fm_effect(command);
	z80_block(41);
	channel = saved_channel;
	fm_effect_26_rounds = 2u;
	attenuation = 0u;
	return fm_effect_26_round(channel, attenuation);
}

/* Starts the tune at the attenuation it is given. */
uint32_t fm_effect_26_round(uint16_t channel, uint8_t attenuation)
{
	uint16_t pitch = 0;

	z80_block(63);
	fm_effect_26_attenuation = attenuation;
	fm_effect_26_notes = 9u;
	pitch = 0x1200u;
	fm_effect_26_previous_pitch = 0x1200u;
	return fm_effect_26_note(pitch, channel);
}

/* A note of the tune at the pitch it is given, keyed on for 28 passes. */
uint32_t fm_effect_26_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch_2 = 0, voice = 0;

	z80_block(47);
	fm_effect_26_pitch = pitch;
	voice = 0x2A4Fu;
	carry = load_fm_voice(voice, channel);
	z80_block(33);
	pitch_2 = fm_effect_26_pitch;
	set_fm_pitch(pitch_2, channel, &ignored16, &carry);
	z80_block(30);
	attenuation = fm_effect_26_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(40);
	fm_effect_26_timer = 0x1Cu;
	handler = fm_effect_26_play_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* For the last 12 passes of a note, the note before it sounds again 16 quieter; then the next note of
   the table; after the tune, the second time 21 quieter; at the end effect 34 starts again if its flag
   is set. */
uint32_t fm_effect_26_play(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, command = 0, note_offset = 0, timer = 0;
	uint16_t ignored16 = 0, pitch = 0, pitch_2 = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_26_timer;
	fm_effect_26_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(19);
		timer = fm_effect_26_timer;
		carry = timer < 0xCu;
		if (timer != 0xCu)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(33);
		pitch = fm_effect_26_previous_pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		z80_block(30);
		attenuation = fm_effect_26_attenuation + 0x10u;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(31);
	t2 = fm_effect_26_notes;
	fm_effect_26_notes = t2 - 1u;
	if (t2 != 1u)
	{
		z80_block(115);
		fm_effect_26_previous_pitch = fm_effect_26_pitch;
		note_offset = (uint32_t)fm_effect_26_notes << 1;
		pitch_2 = TABLE16(fm_effect_26_pitches, (uint16_t)(note_offset - 2u));
		return fm_effect_26_note(pitch_2, channel);
	}
	z80_block(31);
	t3 = fm_effect_26_rounds;
	fm_effect_26_rounds = t3 - 1u;
	if (t3 != 1u)
	{
		z80_block(30);
		attenuation = fm_effect_26_attenuation + 0x15u;
		return fm_effect_26_round(channel, attenuation);
	}
	z80_block(17);
	carry = stop_channel(channel);
	z80_block(22);
	carry = 0u;
	if (fm_effect_34_on == 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(39);
	command = 0x34u;
	carry = start_fm_effect(command);
	z80_block(24);
	return carry;
}

/* A note that swells and then warbles, and a second one four semitones higher and quieter. */
uint32_t fm_effect_27(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t length = 0;
	uint16_t voice = 0;

	z80_block(113);
	CHANNEL(channel)->fm_effect.count = 2u;
	CHANNEL(channel)->fm_effect.attenuation = 0x1Bu;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 5u;
	voice = 0x2B14u;
	carry = load_fm_voice(voice, channel);
	z80_block(7);
	length = 0x28u;
	return fm_effect_27_note(channel, length);
}

/* The note from the base pitch, keyed on for the passes it is given. */
uint32_t fm_effect_27_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(36);
	CHANNEL(channel)->fm_effect.timer = length;
	carry = fm_effect_restart_note(channel);
	z80_block(20);
	handler = fm_effect_27_swell_and_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two louder each pass for the first 4 passes, then a warble; then the second note, four semitones up
   and 38 quieter. */
uint32_t fm_effect_27_swell_and_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, length = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(36);
		carry = CHANNEL(channel)->fm_effect.timer < 0x24u;
		return fm_effect_swell_or_warble(channel, carry);
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(17);
	attenuation = fm_effect_raise_base_pitch(channel);
	z80_block(43);
	CHANNEL(channel)->fm_effect.attenuation = attenuation + 0x26u;
	length = 0x2Cu;
	return fm_effect_27_note(channel, length);
}

/* A step of a warble: two semitones up on even passes, four down on odd ones. */
void fm_effect_warble(uint16_t channel, uint16_t *p_step, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t step = 0;

	z80_block(40);
	step = 0x80u;
	if ((CHANNEL(channel)->fm_effect.timer & 1u) == 0u)
		fm_effect_move_pitch(channel, &step, &carry);
	else
	{
		z80_block(10);
		step = 0xFF00u;
		fm_effect_move_pitch(channel, &step, &carry);
	}
	*p_step = step;
	*p_carry = carry;
}

/* The note's pitch moved by the step it is given. */
void fm_effect_move_pitch(uint16_t channel, uint16_t *p_step, uint32_t *p_carry)
{
	uint16_t step = (uint16_t)*p_step;
	uint32_t carry = *p_carry;
	uint16_t pitch = 0;

	z80_block(38);
	pitch = CHANNEL(channel)->fm_effect.pitch;
	*p_step = step;
	fm_effect_set_pitch_plus(pitch, channel, p_step, &carry);
	*p_carry = carry;
}

/* The note's pitch set to the pitch it is given plus the step. */
void fm_effect_set_pitch_plus(uint16_t pitch, uint16_t channel, uint16_t *p_step, uint32_t *p_carry)
{
	uint16_t step = (uint16_t)*p_step;
	uint32_t carry = *p_carry;
	uint16_t fm_write = 0;

	z80_block(11);
	pitch += step;
	*p_step = fm_write;
	fm_effect_set_pitch(pitch, channel, p_step, &carry);
	*p_carry = carry;
}

/* The note's pitch set to the pitch it is given, kept in the record and on the chip. */
void fm_effect_set_pitch(uint16_t pitch, uint16_t channel, uint16_t *p_fm_write, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint16_t fm_write = 0;

	z80_block(48);
	CHANNEL(channel)->fm_effect.pitch = pitch;
	*p_fm_write = fm_write;
	set_fm_pitch(pitch, channel, p_fm_write, &carry);
	*p_carry = carry;
}

/* The note's pitch set to the pitch it is given less the step. */
uint32_t fm_effect_set_pitch_minus(uint16_t step, uint16_t pitch, uint16_t channel, uint32_t carry)
{
	uint16_t ignored16 = 0;

	z80_block(25);
	pitch = pitch - step - carry;
	fm_effect_set_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* The note again from its base pitch at the record's attenuation, keyed on. */
uint32_t fm_effect_restart_note(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t base_pitch = 0, ignored16 = 0;

	z80_block(55);
	base_pitch = CHANNEL(channel)->fm_effect.base_pitch;
	fm_effect_set_pitch(base_pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(10);
	return fm_key_on(channel);
}

/* With the carry its caller leaves (the timer below the swell's end), a warble step; without it, the
   note 2 louder. */
uint32_t fm_effect_swell_or_warble(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0;

	z80_block(10);
	if (carry)
	{
		fm_effect_warble(channel, &ignored16, &carry);
		return carry;
	}
	z80_block(55);
	attenuation = CHANNEL(channel)->fm_effect.attenuation - 2u;
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* The base pitch four semitones higher; gives back the note's attenuation. */
uint8_t fm_effect_raise_base_pitch(uint16_t channel)
{
	uint8_t attenuation = 0;
	uint16_t t1 = 0;

	z80_block(126);
	t1 = CHANNEL(channel)->fm_effect.base_pitch + 0x100u;
	CHANNEL(channel)->fm_effect.base_pitch = t1;
	attenuation = CHANNEL(channel)->fm_effect.attenuation;
	return attenuation;
}

/* A jittering note that rises for its first 4 steps and then sinks, its steps getting longer; 28 steps. */
uint32_t fm_effect_28(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t pitch = 0, voice = 0;

	z80_block(80);
	fm_effect_28_steps = 0x1Cu;
	voice = 0x2BC6u;
	fm_effect_28_timer = 1u;
	fm_effect_28_period = 1u;
	carry = load_fm_voice(voice, channel);
	z80_block(10);
	pitch = 0x1500u;
	return fm_effect_28_step(pitch, channel);
}

/* Keys the note on at the pitch it is given, attenuation 10. */
uint32_t fm_effect_28_step(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0;

	z80_block(33);
	fm_effect_28_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(24);
	attenuation = 0xAu;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_28_jitter_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Each pass the pitch jumps above the note by the random bytes' place (up to four semitones); each step
   the next note two semitones up (first 4 steps) or four down, 1 or 2 passes longer. */
uint32_t fm_effect_28_jitter(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t period = 0;
	uint16_t ignored16 = 0, pitch = 0, random_place = 0, step = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_28_timer;
	fm_effect_28_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(17);
		random_place = next_random_byte();
		z80_block(48);
		random_place = fm_effect_28_pitch + (uint8_t)random_place;
		set_fm_pitch(random_place, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(31);
	t2 = fm_effect_28_steps;
	fm_effect_28_steps = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(35);
	period = fm_effect_28_period;
	if (((fm_effect_28_steps >> 4) & 1u) == 0u)
	{
		z80_block(7);
		period++;
	}
	z80_block(67);
	period++;
	fm_effect_28_timer = period;
	fm_effect_28_period = period;
	step = 0x80u;
	if (fm_effect_28_steps < 0x18u)
	{
		z80_block(10);
		step = 0xFF00u;
	}
	z80_block(37);
	pitch = fm_effect_28_pitch + step;
	return fm_effect_28_step(pitch, channel);
}

/* A note that falls, then a low note that ticks down ever faster until effect 2B stops it; then a loud,
   quick fall. */
uint32_t fm_effect_29(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, pitch = 0, voice = 0;

	z80_block(47);
	fm_effect_29_on = 1u;
	voice = 0x276Cu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_drums_ADDRESS - 0x2u;
	carry = fm_effect_29_strike(pitch, channel);
	z80_block(20);
	handler = fm_effect_29_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Half a semitone down each pass, from high to low; then the low note. */
uint32_t fm_effect_29_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(58);
	pitch = fm_effect_29_pitch - 0x20u;
	if (pitch >> 8 != 8u)
	{
		z80_block(26);
		fm_effect_29_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(60);
	fm_effect_29_timer = 0x33u;
	fm_effect_29_period = 0x33u;
	voice = 0x276Cu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x910u;
	carry = fm_effect_29_strike(pitch, channel);
	z80_block(20);
	handler = fm_effect_29_tick_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Every few passes an eighth of a semitone lower, the waits shrinking from 51 passes to 1 and starting
   again at 16; when its flag is cleared, at full at once and the quick fall. */
uint32_t fm_effect_29_tick(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, period = 0;
	uint16_t handler = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(27);
	carry = 0u;
	if (fm_effect_29_on == 0u)
	{
		z80_block(44);
		fm_effect_29_timer = 0xB0u;
		attenuation = 0u;
		carry = set_fm_volume(channel, attenuation);
		z80_block(20);
		handler = fm_effect_29_drop_ADDRESS;
		set_channel_handler(handler, channel);
		return carry;
	}
	z80_block(26);
	t1 = fm_effect_29_timer;
	fm_effect_29_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(30);
	t2 = fm_effect_29_period - 1u;
	period = t2;
	if (t2 == 0u)
	{
		z80_block(7);
		period = 0x10u;
	}
	z80_block(30);
	fm_effect_29_period = period;
	fm_effect_29_timer = period;
	return fm_effect_29_lower(channel);
}

/* The last 176 passes: an eighth of a semitone lower every pass, then the channel stops. */
uint32_t fm_effect_29_drop(uint16_t channel)
{
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_29_timer;
	fm_effect_29_timer = t1 - 1u;
	if (t1 == 1u)
		return stop_channel(channel);
	return fm_effect_29_lower(channel);
}

/* The note an eighth of a semitone lower. */
uint32_t fm_effect_29_lower(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t ignored16 = 0, pitch = 0;

	z80_block(63);
	pitch = fm_effect_29_pitch - 8u;
	fm_effect_29_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* Keys the note on at the pitch it is given, attenuation 16. */
uint32_t fm_effect_29_strike(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0;

	z80_block(33);
	fm_effect_29_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(24);
	attenuation = 0x10u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(10);
	return fm_key_on(channel);
}

/* Three notes that swell and warble, each a semitone higher, quieter and longer, with a rest between
   them. */
uint32_t fm_effect_2a(uint16_t channel)
{
	uint8_t length = 0;

	z80_block(93);
	CHANNEL(channel)->fm_effect.count = 3u;
	CHANNEL(channel)->fm_effect.attenuation = 0x14u;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 5u;
	length = 0x1Cu;
	return fm_effect_2a_note(channel, length);
}

/* The note from the base pitch, keyed on for the passes it is given. */
uint32_t fm_effect_2a_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint16_t handler = 0, voice = 0;

	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = length;
	CHANNEL(channel)->fm_effect.period = length;
	voice = 0x2CDCu;
	carry = load_fm_voice(voice, channel);
	z80_block(17);
	carry = fm_effect_restart_note(channel);
	z80_block(20);
	handler = fm_effect_2a_swell_and_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two louder each pass for the first 4 passes, then a warble; then 8 passes almost silent. */
uint32_t fm_effect_2a_swell_and_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(36);
		carry = CHANNEL(channel)->fm_effect.timer < 0x18u;
		return fm_effect_swell_or_warble(channel, carry);
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(43);
	CHANNEL(channel)->fm_effect.timer = 8u;
	attenuation = 0x34u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(20);
	handler = fm_effect_2a_rest_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* After the rest, the next note a semitone higher, 34 quieter and 4 passes longer. */
uint32_t fm_effect_2a_rest(uint16_t channel, uint32_t carry)
{
	uint8_t length = 0;
	uint8_t timer = 0;
	uint16_t t1 = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(178);
	t1 = CHANNEL(channel)->fm_effect.base_pitch + 0x40u;
	CHANNEL(channel)->fm_effect.base_pitch = t1;
	CHANNEL(channel)->fm_effect.attenuation += 0x22u;
	length = CHANNEL(channel)->fm_effect.period + 4u;
	return fm_effect_2a_note(channel, length);
}

/* Stops effects 21 and 29 (clears their flags) and plays PCM effect 85 and a falling note, then 17
   bursts of notes that fade. */
uint32_t fm_effect_2b(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0;
	uint16_t handler = 0, voice = 0;
	uint16_t saved_channel = 0;

	z80_block(69);
	fm_effect_29_on = 0u;
	fm_effect_21_on = 0u;
	saved_channel = channel;
	command = 0x85u;
	start_pcm_effect(command);
	z80_block(60);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.base_attenuation = 5u;
	voice = 0x2D64u;
	carry = fm_effect_drop_start(voice, channel);
	z80_block(20);
	handler = fm_effect_2b_drop_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall for 32 passes; then the bursts. */
uint32_t fm_effect_2b_drop(uint16_t channel)
{
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_drop(channel);
	z80_block(10);
	return fm_effect_bursts_start(channel);
}

/* A note that dives, then 18 notes climbing two semitones a step, plus 2 to 14 more by the random bytes'
   place. */
uint32_t fm_effect_2c(uint16_t channel)
{
	uint8_t dives = 0;

	z80_block(112);
	CHANNEL(channel)->fm_effect_2c.period = 0xFu;
	CHANNEL(channel)->fm_effect_2c.dive_period = 0x18u;
	AS8(CHANNEL(channel)->fm_effect_2c.start_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect_2c.start_pitch, 1) = 0xDu;
	CHANNEL(channel)->fm_effect_2c.climb_notes = 0x12u;
	dives = 1u;
	return fm_effect_dive_times(channel, dives);
}

/* As many dives as it is given, then the climb. */
uint32_t fm_effect_dive_times(uint16_t channel, uint8_t dives)
{
	z80_block(19);
	CHANNEL(channel)->fm_effect_dive.count = dives;
	return fm_effect_dive_start(channel);
}

/* A dive: the voice loaded again and the note keyed on at full, eight semitones below the start pitch,
   for the dive's passes. */
uint32_t fm_effect_dive_start(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, start_pitch = 0, step = 0;

	z80_block(17);
	start_pitch = fm_effect_dive_reload(channel);
	z80_block(27);
	step = 0xFE00u;
	fm_effect_set_pitch_plus(start_pitch, channel, &step, &carry);
	z80_block(62);
	CHANNEL(channel)->fm_effect_dive.timer = CHANNEL(channel)->fm_effect_dive.dive_period;
	attenuation = 0u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_dive_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones down each pass, quiet for the last 4; then the next dive, or the climb from the start
   pitch. */
uint32_t fm_effect_dive(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t start_pitch = 0, step = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect_dive.timer;
	CHANNEL(channel)->fm_effect_dive.timer = timer - 1u;
	if (timer == 1u)
	{
		z80_block(33);
		count = CHANNEL(channel)->fm_effect_dive.count;
		CHANNEL(channel)->fm_effect_dive.count = count - 1u;
		if (count != 1u)
			return fm_effect_dive_start(channel);
		z80_block(55);
		CHANNEL(channel)->fm_effect_dive.count = CHANNEL(channel)->fm_effect_dive.climb_notes;
		start_pitch = fm_effect_dive_reload(channel);
		return fm_effect_dive_climb_note(start_pitch, channel);
	}
	z80_block(36);
	if (CHANNEL(channel)->fm_effect_dive.timer == 4u)
	{
		z80_block(17);
		attenuation = 0x10u;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(20);
	step = 0xFF7Fu;
	fm_effect_move_pitch(channel, &step, &carry);
	return carry;
}

/* A note of the climb at the pitch it is given, at full, for the effect's period. */
uint32_t fm_effect_dive_climb_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0;

	z80_block(17);
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(62);
	CHANNEL(channel)->fm_effect_dive.timer = CHANNEL(channel)->fm_effect_dive.period;
	attenuation = 0u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_dive_climb_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The next note two semitones above the last plus 2 to 14 semitones by the random bytes' place; while 16
   or more are left the voice and the pitch start again from the start pitch. */
uint32_t fm_effect_dive_climb(uint16_t channel, uint32_t carry)
{
	uint8_t place_low = 0, place_page = 0;
	uint16_t pitch = 0, random_place = 0;
	uint8_t timer = 0, count = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect_dive.timer;
	CHANNEL(channel)->fm_effect_dive.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect_dive.count;
	CHANNEL(channel)->fm_effect_dive.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(30);
	if (((CHANNEL(channel)->fm_effect_dive.count >> 4) & 1u) != 0u)
	{
		z80_cycles(7);
		fm_effect_dive_reload(channel);
	}
	z80_block(17);
	random_place = next_random_byte();
	z80_block(145);
	place_low = (uint8_t)random_place;
	place_page = (uint8_t)(random_place >> 8);
	pitch = CHANNEL(channel)->fm_effect_dive.pitch + 0x80u;
	CHANNEL(channel)->fm_effect_dive.pitch = pitch;
	pitch += place_page | ((place_low & 3u) << 8);
	return fm_effect_dive_climb_note(pitch, channel);
}

/* Loads the voice again and puts the note back at the start pitch. */
uint16_t fm_effect_dive_reload(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t pitch_high = 0, pitch_low = 0;
	uint16_t start_pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x2DA6u;
	carry = load_fm_voice(voice, channel);
	z80_block(86);
	pitch_low = (uint8_t)CHANNEL(channel)->fm_effect_dive.start_pitch;
	pitch_high = (uint8_t)(CHANNEL(channel)->fm_effect_dive.start_pitch >> 8u);
	AS8(CHANNEL(channel)->fm_effect_dive.pitch) = pitch_low;
	AT8(CHANNEL(channel)->fm_effect_dive.pitch, 1) = pitch_high;
	start_pitch = ((uint32_t)pitch_high << 8) | pitch_low;
	return start_pitch;
}

/* 32 zigzag notes: the first 5 two semitones higher each and louder, the rest a semitone lower each and
   quieter. */
uint32_t fm_effect_2d(uint16_t channel)
{
	uint16_t pitch = 0;

	z80_block(50);
	fm_effect_2d_notes = 0x20u;
	fm_effect_2d_attenuation = 0xCu;
	pitch = 0x600u;
	return fm_effect_2d_note(pitch, channel);
}

/* A note at the pitch it is given, keyed on for 10 passes. */
uint32_t fm_effect_2d_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, voice = 0;

	z80_block(59);
	fm_effect_2d_base_pitch = pitch;
	fm_effect_2d_pitch = pitch;
	voice = 0x1EEEu;
	carry = load_fm_voice(voice, channel);
	z80_block(33);
	pitch = fm_effect_2d_base_pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(50);
	fm_effect_2d_timer = 0xAu;
	attenuation = fm_effect_2d_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_2d_zigzag_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones up and four down in turn; then the next note: two semitones up and 1 louder for the
   first 5, then a semitone down and 1 quieter (2 in the second half). */
uint32_t fm_effect_2d_zigzag(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_2d_timer;
	fm_effect_2d_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(48);
		pitch = fm_effect_2d_pitch;
		step = 0xFF80u;
		if (fm_effect_2d_timer & 1u)
		{
			z80_block(10);
			step = 0x100u;
		}
		z80_block(41);
		pitch = pitch - step - carry;
		fm_effect_2d_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(31);
	t2 = fm_effect_2d_notes;
	fm_effect_2d_notes = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(37);
	t3 = fm_effect_2d_notes;
	carry = t3 < 0x1Bu;
	attenuation = fm_effect_2d_attenuation;
	if (t3 < 0x1Bu)
	{
		z80_block(29);
		attenuation++;
		if (((fm_effect_2d_notes >> 4) & 1u) == 0u)
		{
			z80_block(7);
			attenuation++;
		}
		z80_block(23);
		fm_effect_2d_attenuation = attenuation;
		step = 0xFFC0u;
	}
	else
	{
		z80_block(40);
		fm_effect_2d_attenuation = attenuation - 1u - carry;
		step = 0x80u;
	}
	z80_block(37);
	pitch = fm_effect_2d_base_pitch + step;
	return fm_effect_2d_note(pitch, channel);
}

/* Two falling notes, then 24 trills that fade. */
uint32_t fm_effect_2e(uint16_t channel)
{
	z80_block(19);
	CHANNEL(channel)->fm_effect.rounds = 2u;
	return fm_effect_2e_note(channel);
}

/* A note keyed on high, which falls for 32 passes. */
uint32_t fm_effect_2e_note(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x2EF8u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x1200u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x400u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x20u;
	attenuation = 4u;
	CHANNEL(channel)->fm_effect.base_attenuation = 4u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_2e_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall, two semitones a pass, dying away near its end; after the second note, the trills. */
uint32_t fm_effect_2e_fall(uint16_t channel)
{
	uint8_t attenuation = 0, die_away_pass = 0;
	uint8_t timer = 0, rounds = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 4u;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(33);
	rounds = CHANNEL(channel)->fm_effect.rounds;
	CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
	if (rounds != 1u)
		return fm_effect_2e_note(channel);
	z80_block(26);
	CHANNEL(channel)->fm_effect.rounds = 0x18u;
	attenuation = 4u;
	return fm_effect_2e_trill_start(channel, attenuation);
}

/* A trill of 5 steps at the attenuation it is given. */
uint32_t fm_effect_2e_trill_start(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(84);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.timer = 6u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0xE00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(65);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xCu;
	carry = set_fm_volume_from_record(channel);
	z80_block(20);
	handler = fm_effect_2e_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, sliding down; every 6 passes the pair starts again lower; then the next trill,
   6 quieter. */
uint32_t fm_effect_2e_trill(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t base_pitch = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0, rounds = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 6u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
	{
		z80_block(33);
		rounds = CHANNEL(channel)->fm_effect.rounds;
		CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
		if (rounds == 1u)
			return stop_channel(channel);
		z80_block(36);
		attenuation = CHANNEL(channel)->fm_effect.attenuation + 6u;
		return fm_effect_2e_trill_start(channel, attenuation);
	}
	z80_block(93);
	step = 0x200u;
	t1 = 0xA00u - carry;
	carry = 0x200u + carry > 0xC00u;
	base_pitch = t1;
	CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
	pitch = 0x600u;
	return fm_effect_set_pitch_minus(step, pitch, channel, carry);
}

/* Three notes that swell and warble, each four semitones higher, quieter and longer. */
uint32_t fm_effect_2f(uint16_t channel)
{
	uint8_t length = 0;

	z80_block(93);
	CHANNEL(channel)->fm_effect.count = 3u;
	CHANNEL(channel)->fm_effect.attenuation = 0x1Eu;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 5u;
	length = 0x2Cu;
	return fm_effect_2f_note(channel, length);
}

/* The note from the base pitch, keyed on for the passes it is given. */
uint32_t fm_effect_2f_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint16_t handler = 0, voice = 0;

	z80_block(65);
	CHANNEL(channel)->fm_effect.timer = length;
	CHANNEL(channel)->fm_effect.period = length;
	voice = 0x2CDCu;
	carry = load_fm_voice(voice, channel);
	z80_block(17);
	carry = fm_effect_restart_note(channel);
	z80_block(20);
	handler = fm_effect_2f_swell_and_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two louder each pass for the first 4 passes, then a warble; then the next note four semitones up, 42
   quieter and 3 passes longer. */
uint32_t fm_effect_2f_swell_and_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, length = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(36);
		carry = CHANNEL(channel)->fm_effect.timer < 0x28u;
		return fm_effect_swell_or_warble(channel, carry);
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(17);
	attenuation = fm_effect_raise_base_pitch(channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.attenuation = attenuation + 0x2Au;
	length = CHANNEL(channel)->fm_effect.period + 3u;
	return fm_effect_2f_note(channel, length);
}

/* A low note keyed on for the passes it is given. */
uint32_t fm_effect_30_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0, voice = 0;

	z80_block(40);
	fm_effect_30_timer = length;
	voice = 0x2FF4u;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x400u;
	fm_effect_30_pitch = 0x400u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(30);
	attenuation = fm_effect_30_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(10);
	return fm_key_on(channel);
}

/* The note's pitch up by the step it is given. */
uint32_t fm_effect_30_raise(uint16_t step, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t ignored16 = 0, pitch = 0;

	z80_block(53);
	pitch = fm_effect_30_pitch + step;
	fm_effect_30_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* A low note that sweeps up fast, then 5 slower sweeps up, each 16 quieter. */
uint32_t fm_effect_30(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t length = 0;
	uint16_t handler = 0;

	z80_block(44);
	fm_effect_30_attenuation = 2u;
	length = 0x14u;
	carry = fm_effect_30_note(channel, length);
	z80_block(20);
	handler = fm_effect_30_sweep_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Four semitones up each pass for 20 passes, quiet for the last 4; then the slower sweeps. */
uint32_t fm_effect_30_sweep(uint16_t channel)
{
	uint8_t attenuation = 0;
	uint16_t step = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_30_timer;
	fm_effect_30_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(27);
		fm_effect_30_repeats = 5u;
		attenuation = 2u;
		return fm_effect_30_slow_sweep_start(channel, attenuation);
	}
	z80_block(24);
	if (fm_effect_30_timer == 4u)
	{
		z80_block(17);
		attenuation = 0x22u;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(20);
	step = 0x100u;
	return fm_effect_30_raise(step, channel);
}

/* A slow sweep of 48 passes at the attenuation it is given. */
uint32_t fm_effect_30_slow_sweep_start(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint8_t length = 0;
	uint16_t handler = 0;

	z80_block(37);
	fm_effect_30_attenuation = attenuation;
	length = 0x30u;
	carry = fm_effect_30_note(channel, length);
	z80_block(20);
	handler = fm_effect_30_slow_sweep_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones up each pass; then the next sweep, 16 quieter, 5 in all. */
uint32_t fm_effect_30_slow_sweep(uint16_t channel)
{
	uint8_t attenuation = 0;
	uint16_t step = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_30_timer;
	fm_effect_30_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(20);
		step = 0x80u;
		return fm_effect_30_raise(step, channel);
	}
	z80_block(31);
	t2 = fm_effect_30_repeats;
	fm_effect_30_repeats = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(30);
	attenuation = fm_effect_30_attenuation + 0x10u;
	return fm_effect_30_slow_sweep_start(channel, attenuation);
}

/* Two falling notes, then 16 trills that fade. */
uint32_t fm_effect_31(uint16_t channel)
{
	z80_block(19);
	CHANNEL(channel)->fm_effect.rounds = 2u;
	return fm_effect_31_note(channel);
}

/* A note keyed on high, which falls for 24 passes. */
uint32_t fm_effect_31_note(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x308Bu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x400u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x18u;
	attenuation = 7u;
	CHANNEL(channel)->fm_effect.base_attenuation = 7u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_31_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall, two semitones a pass, dying away near its end; after the second note, the trills. */
uint32_t fm_effect_31_fall(uint16_t channel)
{
	uint8_t attenuation = 0, die_away_pass = 0;
	uint8_t timer = 0, rounds = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(17);
		die_away_pass = 4u;
		return fm_effect_fall(channel, die_away_pass);
	}
	z80_block(33);
	rounds = CHANNEL(channel)->fm_effect.rounds;
	CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
	if (rounds != 1u)
		return fm_effect_31_note(channel);
	z80_block(26);
	CHANNEL(channel)->fm_effect.rounds = 0x10u;
	attenuation = 7u;
	return fm_effect_31_trill_start(channel, attenuation);
}

/* A trill of 5 steps at the attenuation it is given. */
uint32_t fm_effect_31_trill_start(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(84);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.timer = 6u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0xC00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(65);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xAu;
	carry = set_fm_volume_from_record(channel);
	z80_block(20);
	handler = fm_effect_31_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, sliding down; every 6 passes the pair starts again lower; then the next trill,
   8 quieter. */
uint32_t fm_effect_31_trill(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t base_pitch = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0, rounds = 0;
	uint16_t t1 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 6u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
	{
		z80_block(33);
		rounds = CHANNEL(channel)->fm_effect.rounds;
		CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
		if (rounds == 1u)
			return stop_channel(channel);
		z80_block(36);
		attenuation = CHANNEL(channel)->fm_effect.attenuation + 8u;
		return fm_effect_31_trill_start(channel, attenuation);
	}
	z80_block(93);
	step = 0x200u;
	t1 = 0x800u - carry;
	carry = 0x200u + carry > 0xA00u;
	base_pitch = t1;
	CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
	pitch = 0x400u;
	return fm_effect_set_pitch_minus(step, pitch, channel, carry);
}

/* A note that bends up a little and then down, twice (the second quieter). */
uint32_t fm_effect_32(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t voice = 0;

	z80_block(47);
	fm_effect_32_repeats = 2u;
	voice = 0x313Bu;
	carry = load_fm_voice(voice, channel);
	z80_block(7);
	attenuation = 0xBu;
	return fm_effect_32_note(channel, attenuation);
}

/* The note keyed on at the attenuation it is given, for 48 passes. */
uint32_t fm_effect_32_note(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(56);
	fm_effect_32_attenuation = attenuation;
	pitch = 0xF00u;
	fm_effect_32_pitch = 0xF00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(30);
	attenuation = fm_effect_32_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(40);
	fm_effect_32_timer = 0x30u;
	handler = fm_effect_32_bend_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* A sixteenth of a semitone up each pass for 4 passes, then an eighth down for 36, then held 8; then the
   second note, at attenuation 39. */
uint32_t fm_effect_32_bend(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, timer = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(31);
	t1 = fm_effect_32_timer;
	fm_effect_32_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(31);
		t2 = fm_effect_32_repeats;
		fm_effect_32_repeats = t2 - 1u;
		if (t2 == 1u)
			return stop_channel(channel);
		z80_block(17);
		attenuation = 0x27u;
		return fm_effect_32_note(channel, attenuation);
	}
	z80_block(19);
	timer = fm_effect_32_timer;
	carry = timer < 8u;
	if (timer < 8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(43);
	pitch = fm_effect_32_pitch;
	step = 0xFFF8u;
	if (timer >= 0x2Cu)
	{
		z80_block(10);
		step = 4u;
	}
	z80_block(37);
	pitch += step;
	fm_effect_32_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* A very low drone, struck 20 times, each 1 quieter, while effect 0A's flag stays set (effect 0A starts
   it). */
uint32_t fm_effect_33(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(30);
	fm_effect_0a_on = 1u;
	z80_block(27);
	fm_effect_33_repeats = 0x14u;
	attenuation = 0xBu;
	return fm_effect_33_drone(channel, attenuation);
}

/* A sound that goes on until effect 35 stops it: here a falling note and then trills over and over; on
   the channel the FM effects take from the music's channel 6, warbling notes whose length moves with the
   random bytes' place; no song starts while it plays. */
uint32_t fm_effect_34(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(137);
	fm_effect_34_on = 1u;
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_34_warbles_ADDRESS;
	CHANNEL(FM_EFFECT_6)->sound = CHANNEL(channel)->sound;
	CHANNEL(FM_EFFECT_6)->priority = CHANNEL(channel)->priority;
	voice = 0x31B5u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x1080u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x400u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(60);
	CHANNEL(channel)->fm_effect.timer = 0x18u;
	attenuation = music_attenuation + 6u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_34_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones down each pass for 24 passes, at attenuation 12 for the last 8; then the trills. */
uint32_t fm_effect_34_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t step = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
		return fm_effect_34_trill_start(channel);
	z80_block(36);
	if (8u == CHANNEL(channel)->fm_effect.timer)
	{
		z80_block(17);
		attenuation = 0xCu;
		return set_fm_volume(channel, attenuation);
	}
	z80_block(20);
	step = 0xFF80u;
	fm_effect_move_pitch(channel, &step, &carry);
	return carry;
}

/* A trill of 5 steps at 6 over the music's attenuation. */
uint32_t fm_effect_34_trill_start(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x31B5u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0xE80u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(108);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xAu;
	attenuation = music_attenuation + 6u;
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(58);
	CHANNEL(channel)->fm_effect.timer = 6u;
	CHANNEL(channel)->fm_effect.count = 5u;
	handler = fm_effect_34_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, sliding down; every 6 passes a warble step and the pair again lower; after 5
   steps the trill again from its start; it stops when its flag is cleared. */
uint32_t fm_effect_34_trill(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t base_pitch = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0;
	uint16_t t1 = 0;

	z80_block(27);
	if (fm_effect_34_on == 0u)
		return stop_channel(channel);
	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 6u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return fm_effect_34_trill_start(channel);
	z80_block(17);
	fm_effect_warble(channel, &step, &carry);
	z80_block(83);
	t1 = 0xA00u - step - carry;
	carry = step + carry > 0xA00u;
	base_pitch = t1;
	CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
	pitch = 0x400u;
	return fm_effect_set_pitch_minus(step, pitch, channel, carry);
}

/* The second channel's part: 6 warbling notes from 32 passes, at 8 over the music's attenuation. */
uint32_t fm_effect_34_warbles(uint16_t channel)
{
	uint8_t length = 0;

	z80_block(47);
	fm_effect_34_notes = 6u;
	fm_effect_34_attenuation = 8u;
	length = 0x20u;
	return fm_effect_34_warble_note(channel, length);
}

/* A low note keyed on for the passes it is given, at the music's attenuation plus the note's. */
uint32_t fm_effect_34_warble_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(53);
	fm_effect_34_timer = length;
	fm_effect_34_length = length;
	voice = 0x3283u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x1100u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(77);
	fm_effect_34_pitch = 0x700u;
	attenuation = music_attenuation + fm_effect_34_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_34_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two semitones up and four down in turn (wrapping within the lowest 32 semitones); then the next note,
   0 to 7 passes longer by the random bytes' place and 4 to 11 over the music's attenuation; after 6
   notes, again from the first; it stops when its flag is cleared. */
uint32_t fm_effect_34_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t extra = 0, length = 0, step = 0, step_high = 0;
	uint16_t ignored16 = 0, pitch = 0, random_place = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(27);
	if (fm_effect_34_on == 0u)
		return stop_channel(channel);
	z80_block(31);
	t1 = fm_effect_34_timer;
	fm_effect_34_timer = t1 - 1u;
	if (t1 == 1u)
	{
		z80_block(31);
		t2 = fm_effect_34_notes;
		fm_effect_34_notes = t2 - 1u;
		if (t2 == 1u)
			return fm_effect_34_warbles(channel);
		z80_block(17);
		random_place = next_random_byte();
		z80_block(66);
		extra = random_place & 7u;
		fm_effect_34_attenuation = 4u + extra;
		length = fm_effect_34_length + extra;
		return fm_effect_34_warble_note(channel, length);
	}
	z80_block(32);
	step = 0x80u;
	step_high = 0u;
	if (fm_effect_34_timer & 1u)
	{
		z80_block(10);
		step = 0u;
		step_high = 0xFFu;
	}
	z80_block(68);
	pitch = fm_effect_34_pitch + (((uint32_t)step_high << 8) | (step & 0xFFu));
	fm_effect_34_pitch = pitch;
	pitch = (pitch & 0xFFu) | (((pitch >> 8) & 7u) << 8);
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* Stops effect 34 (clears its flag); its own channel stops at once. */
uint32_t fm_effect_35(uint16_t channel)
{
	z80_block(27);
	fm_effect_34_on = 0u;
	return stop_channel(channel);
}

/* A note that rises a semitone a pass, is held, then falls. */
uint32_t fm_effect_36(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x331Du;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x700u;
	fm_effect_36_pitch = 0x700u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(24);
	attenuation = 0xAu;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_36_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Rises for 24 passes; then the high note, keyed on again. */
uint32_t fm_effect_36_rise(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(58);
	pitch = fm_effect_36_pitch + 0x40u;
	if (pitch >> 8 != 0xDu)
	{
		z80_block(26);
		fm_effect_36_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(27);
	voice = 0x331Du;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0xD10u;
	fm_effect_36_pitch = 0xD10u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(44);
	fm_effect_36_timer = 0xA0u;
	attenuation = 0xAu;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_36_hold_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Holds it 160 passes. */
void fm_effect_36_hold(uint16_t channel)
{
	uint16_t handler = 0;
	uint8_t t1 = 0;

	z80_block(26);
	t1 = fm_effect_36_timer;
	fm_effect_36_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(30);
	fm_effect_36_timer = 0x30u;
	handler = fm_effect_36_fall_ADDRESS;
	set_channel_handler(handler, channel);
}

/* A semitone down each pass for 48 passes, then the channel stops. */
uint32_t fm_effect_36_fall(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0;

	z80_block(31);
	t1 = fm_effect_36_timer;
	fm_effect_36_timer = t1 - 1u;
	if (t1 == 1u)
		return stop_channel(channel);
	z80_block(63);
	pitch = fm_effect_36_pitch - 0x40u;
	fm_effect_36_pitch = pitch;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	return carry;
}

/* A fast trill between two notes, swelling then fading: 48 steps of 12 passes. */
uint32_t fm_effect_38(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(47);
	fm_effect_38_steps = 0x30u;
	voice = 0x33A9u;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = 0x600u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(57);
	fm_effect_38_timer = 0xCu;
	attenuation = 0x12u;
	fm_effect_38_attenuation = 0x12u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_38_trill_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, a pass each; every 12 passes 1 louder for the first 7 steps, then 1 quieter. */
uint32_t fm_effect_38_trill(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_38_timer;
	fm_effect_38_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(32);
		pitch = 0x600u;
		if ((fm_effect_38_timer & 1u) != 0u)
		{
			set_fm_pitch(pitch, channel, &ignored16, &carry);
			return carry;
		}
		z80_block(20);
		pitch = pcm_instruments_ADDRESS - 0x2u;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(41);
	fm_effect_38_timer = 0xCu;
	t2 = fm_effect_38_steps;
	fm_effect_38_steps = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(37);
	t3 = fm_effect_38_steps;
	attenuation = fm_effect_38_attenuation;
	if (t3 < 0x29u)
	{
		z80_block(7);
		attenuation += 2u;
	}
	z80_block(27);
	attenuation--;
	fm_effect_38_attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}

/* Six notes climbing two semitones a step, plus 2 to 14 more by the random bytes' place, each 6 quieter. */
uint32_t fm_effect_39(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t pitch = 0, voice = 0;

	z80_block(67);
	fm_effect_39_attenuation = 2u;
	fm_effect_39_notes = 6u;
	voice = 0x3419u;
	carry = load_fm_voice(voice, channel);
	z80_block(26);
	pitch = 0xA00u;
	fm_effect_39_pitch = 0xA00u;
	return fm_effect_39_note(pitch, channel);
}

/* A note at the pitch it is given, keyed on for 16 passes. */
uint32_t fm_effect_39_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0;

	z80_block(17);
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(50);
	fm_effect_39_timer = 0x10u;
	attenuation = fm_effect_39_attenuation;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_39_next_note_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* After the 16 passes the next note: two semitones higher plus 2 to 14 more by the random bytes' place,
   6 quieter. */
uint32_t fm_effect_39_next_note(uint16_t channel, uint32_t carry)
{
	uint8_t place_low = 0, place_page = 0;
	uint16_t pitch = 0, random_place = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(26);
	t1 = fm_effect_39_timer;
	fm_effect_39_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(31);
	t2 = fm_effect_39_notes;
	fm_effect_39_notes = t2 - 1u;
	if (t2 == 1u)
		return stop_channel(channel);
	z80_block(50);
	fm_effect_39_attenuation += 6u;
	random_place = next_random_byte();
	z80_block(101);
	place_low = (uint8_t)random_place;
	place_page = (uint8_t)(random_place >> 8);
	pitch = fm_effect_39_pitch + 0x80u;
	fm_effect_39_pitch = pitch;
	pitch += place_page | ((place_low & 3u) << 8);
	return fm_effect_39_note(pitch, channel);
}

/* PCM effect 8C and a warbling note that steps up 8 times getting louder, holds two steps, then sinks
   slowly as it fades: 74 steps. */
uint32_t fm_effect_3a(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t pitch = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x8Cu;
	start_pcm_effect(command);
	z80_block(88);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.count = 0x4Au;
	CHANNEL(channel)->fm_effect.timer = 5u;
	CHANNEL(channel)->fm_effect.attenuation = 0xAu;
	pitch = 0x500u;
	return fm_effect_3a_note(pitch, channel);
}

/* Keys the warbling note on at the pitch it is given. */
uint32_t fm_effect_3a_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t base_pitch = 0, handler = 0, ignored16 = 0, voice = 0;

	z80_block(17);
	fm_effect_keep_pitch(pitch, channel);
	z80_block(27);
	voice = 0x1EEEu;
	carry = load_fm_voice(voice, channel);
	z80_block(55);
	base_pitch = CHANNEL(channel)->fm_effect.base_pitch;
	set_fm_pitch(base_pitch, channel, &ignored16, &carry);
	z80_block(17);
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_3a_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Warbles; each step the note again: a semitone up and 1 louder for the first 8 (5 passes each), the
   same for the next 2, then a quarter of a semitone down and 1 or 2 quieter (12 passes each). */
uint32_t fm_effect_3a_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t timer = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0;
	uint8_t timer_2 = 0, count = 0;

	z80_block(33);
	timer_2 = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer_2 - 1u;
	if (timer_2 != 1u)
	{
		fm_effect_warble(channel, &ignored16, &carry);
		return carry;
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(36);
	if (CHANNEL(channel)->fm_effect.count >= 0x42u)
	{
		z80_block(50);
		CHANNEL(channel)->fm_effect.attenuation--;
		step = 0x40u;
		timer = 5u;
		z80_block(78);
		CHANNEL(channel)->fm_effect.timer = timer;
		pitch = CHANNEL(channel)->fm_effect.base_pitch + step;
		return fm_effect_3a_note(pitch, channel);
	}
	z80_block(40);
	step = 0u;
	if (((CHANNEL(channel)->fm_effect.count >> 6) & 1u) != 0u)
	{
		z80_block(7);
		timer = 0xCu;
		z80_block(78);
		CHANNEL(channel)->fm_effect.timer = timer;
		pitch = CHANNEL(channel)->fm_effect.base_pitch + step;
		return fm_effect_3a_note(pitch, channel);
	}
	z80_block(30);
	if ((CHANNEL(channel)->fm_effect.count & 1u) == 0u)
	{
		z80_block(23);
		CHANNEL(channel)->fm_effect.attenuation++;
	}
	z80_block(33);
	CHANNEL(channel)->fm_effect.attenuation++;
	step = 0xFFF0u;
	z80_block(7);
	timer = 0xCu;
	z80_block(78);
	CHANNEL(channel)->fm_effect.timer = timer;
	pitch = CHANNEL(channel)->fm_effect.base_pitch + step;
	return fm_effect_3a_note(pitch, channel);
}

/* Four notes that swell and warble, each four semitones higher, quieter and longer. */
uint32_t fm_effect_3b(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t length = 0;
	uint16_t voice = 0;

	z80_block(113);
	CHANNEL(channel)->fm_effect.count = 4u;
	CHANNEL(channel)->fm_effect.attenuation = 0x16u;
	AS8(CHANNEL(channel)->fm_effect.base_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.base_pitch, 1) = 5u;
	voice = 0x350Bu;
	carry = load_fm_voice(voice, channel);
	z80_block(7);
	length = 0x24u;
	return fm_effect_3b_note(channel, length);
}

/* The note from the base pitch, keyed on for the passes it is given. */
uint32_t fm_effect_3b_note(uint16_t channel, uint8_t length)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.timer = length;
	CHANNEL(channel)->fm_effect.period = length;
	carry = fm_effect_restart_note(channel);
	z80_block(20);
	handler = fm_effect_3b_swell_and_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Two louder each pass for the first 2 passes, then a warble; then the next note four semitones up, 26
   quieter and 3 passes longer. */
uint32_t fm_effect_3b_swell_and_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, length = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_block(36);
		carry = CHANNEL(channel)->fm_effect.timer < 0x22u;
		return fm_effect_swell_or_warble(channel, carry);
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(17);
	attenuation = fm_effect_raise_base_pitch(channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.attenuation = attenuation + 0x1Au;
	length = CHANNEL(channel)->fm_effect.period + 3u;
	return fm_effect_3b_note(channel, length);
}

/* PCM effect 83 and a warbling note that steps up 5 times getting louder, then sinks as it fades: 32
   steps. */
uint32_t fm_effect_3c(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t pitch = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x83u;
	start_pcm_effect(command);
	z80_block(62);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect.count = 0x20u;
	CHANNEL(channel)->fm_effect.attenuation = 0xCu;
	pitch = 0x500u;
	return fm_effect_3c_note(pitch, channel);
}

/* Keys the warbling note on at the pitch it is given, for 12 passes. */
uint32_t fm_effect_3c_note(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t base_pitch = 0, handler = 0, ignored16 = 0, voice = 0;

	z80_block(17);
	fm_effect_keep_pitch(pitch, channel);
	z80_block(27);
	voice = 0x1EEEu;
	carry = load_fm_voice(voice, channel);
	z80_block(55);
	base_pitch = CHANNEL(channel)->fm_effect.base_pitch;
	set_fm_pitch(base_pitch, channel, &ignored16, &carry);
	z80_block(36);
	CHANNEL(channel)->fm_effect.timer = 0xCu;
	carry = set_fm_volume_from_record(channel);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_3c_warble_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Warbles; each step the note again: two semitones up and 1 louder for the first 5, then a little under
   two semitones down and 1 quieter. */
uint32_t fm_effect_3c_warble(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t ignored16 = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		fm_effect_warble(channel, &ignored16, &carry);
		return carry;
	}
	z80_block(33);
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
		return stop_channel(channel);
	z80_block(36);
	if (CHANNEL(channel)->fm_effect.count < 0x1Bu)
	{
		z80_block(33);
		CHANNEL(channel)->fm_effect.attenuation++;
		step = 0xFF90u;
	}
	else
	{
		z80_block(43);
		CHANNEL(channel)->fm_effect.attenuation--;
		step = 0x80u;
	}
	z80_block(59);
	pitch = CHANNEL(channel)->fm_effect.base_pitch + step;
	return fm_effect_3c_note(pitch, channel);
}

/* A slow rise on three channels: here at once, and on the FM effects' channels 6 and 7 after 128 and 255
   passes, quieter. */
uint32_t fm_effect_3d(uint16_t channel)
{
	z80_block(137);
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_3d_on_channel_6_ADDRESS;
	CHANNEL(FM_EFFECT_7)->handler = fm_effect_3d_on_channel_7_ADDRESS;
	CHANNEL(FM_EFFECT_6)->sound = 0x22u;
	CHANNEL(FM_EFFECT_7)->sound = 0x22u;
	CHANNEL(FM_EFFECT_6)->priority = 0x15u;
	CHANNEL(FM_EFFECT_7)->priority = 0x15u;
	CHANNEL(channel)->fm_effect.attenuation = 8u;
	return fm_effect_3d_rise(channel);
}

/* The slow rise: 320 passes, a quarter of a semitone a pass. */
uint32_t fm_effect_3d_rise(uint16_t channel)
{
	uint16_t length = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect.rise_step = 0x10u;
	length = 0x140u;
	return fm_effect_long_rise_start(length, channel);
}

/* The part on channel 6: the rise after 128 passes, at attenuation 20. */
uint32_t fm_effect_3d_on_channel_6(uint16_t channel)
{
	uint8_t wait = 0;
	uint16_t attenuation = 0;

	z80_block(24);
	wait = 0x80u;
	attenuation = 0x1400u;
	return fm_effect_3d_delay(attenuation, channel, wait);
}

/* The part on channel 7: the rise after 255 passes, at attenuation 32. */
uint32_t fm_effect_3d_on_channel_7(uint16_t channel)
{
	uint8_t wait = 0;
	uint16_t attenuation = 0;

	z80_block(14);
	wait = 0xFFu;
	attenuation = 0x2000u;
	return fm_effect_3d_delay(attenuation, channel, wait);
}

/* Keys the channel off and waits the passes it is given, then plays at the attenuation it is given. */
uint32_t fm_effect_3d_delay(uint16_t attenuation, uint16_t channel, uint8_t wait)
{
	uint32_t carry = 0;
	uint16_t handler = 0;

	z80_block(55);
	CHANNEL(channel)->fm_effect.timer = wait;
	CHANNEL(channel)->fm_effect.attenuation = attenuation >> 8;
	fm_key_off(channel);
	z80_block(17);
	carry = fm_fast_release(channel);
	z80_block(20);
	handler = fm_effect_3d_wait_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the wait down; then the rise. */
uint32_t fm_effect_3d_wait(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return fm_effect_3d_rise(channel);
}

/* Starts effect 3F and plays its falling note and trills 16 passes later, here and on the channel the FM
   effects take from the music's channel 6. */
uint32_t fm_effect_3e(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0;
	uint16_t saved_channel = 0;

	z80_block(105);
	CHANNEL(FM_EFFECT_6)->handler = fm_effect_3e_delay_ADDRESS;
	CHANNEL(FM_EFFECT_6)->sound = 0x3Eu;
	CHANNEL(FM_EFFECT_6)->priority = 0x15u;
	saved_channel = channel;
	command = 0x3Fu;
	carry = start_fm_effect(command);
	z80_block(14);
	channel = saved_channel;
	fm_effect_3e_delay(channel);
	return carry;
}

/* Waits 16 passes. */
void fm_effect_3e_delay(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->fm_effect.timer = 0x10u;
	handler = fm_effect_3e_wait_ADDRESS;
	set_channel_handler(handler, channel);
}

/* Then effect 3F's falling note and trills. */
uint32_t fm_effect_3e_wait(uint16_t channel, uint32_t carry)
{
	uint8_t timer = 0;

	z80_block(28);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return fm_effect_fall_and_trills_start(channel);
}

/* PCM effect 85 and a falling note, then 16 trills that fade. */
uint32_t fm_effect_3f(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x85u;
	start_pcm_effect(command);
	z80_block(14);
	channel = saved_channel;
	return fm_effect_fall_and_trills_start(channel);
}

/* A note keyed on high at full, which falls for 22 passes. */
uint32_t fm_effect_fall_and_trills_start(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x364Bu;
	carry = load_fm_voice(voice, channel);
	z80_block(27);
	pitch = pcm_instruments_ADDRESS - 0x2u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(27);
	pitch = 0x400u;
	fm_effect_keep_pitch(pitch, channel);
	z80_block(62);
	CHANNEL(channel)->fm_effect.timer = 0x16u;
	attenuation = 0u;
	CHANNEL(channel)->fm_effect.base_attenuation = 0u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_fall_and_trills_fall_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The fall, two semitones a pass, dying away when 6 passes are left; then the trills. */
uint32_t fm_effect_fall_and_trills_fall(uint16_t channel)
{
	uint8_t attenuation = 0, die_away_pass = 0;
	uint8_t timer = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer == 1u)
	{
		z80_block(26);
		CHANNEL(channel)->fm_effect.rounds = 0x10u;
		attenuation = 0u;
		return fm_effect_fall_and_trills_round(channel, attenuation);
	}
	z80_block(17);
	die_away_pass = 6u;
	return fm_effect_fall(channel, die_away_pass);
}

/* A trill of 5 steps at the attenuation it is given. */
uint32_t fm_effect_fall_and_trills_round(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0;

	z80_block(84);
	CHANNEL(channel)->fm_effect.attenuation = attenuation;
	CHANNEL(channel)->fm_effect.timer = 6u;
	CHANNEL(channel)->fm_effect.count = 5u;
	pitch = 0xC00u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(65);
	AS8(CHANNEL(channel)->fm_effect.pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect.pitch, 1) = 0xAu;
	carry = set_fm_volume_from_record(channel);
	z80_block(20);
	handler = fm_effect_fall_and_trills_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* The two notes in turn, sliding down; every 6 passes the pair starts again lower; then the next trill,
   10 quieter (the round's bit 3 it tests first decides nothing: the add after it sets the flag the jump
   reads). */
uint32_t fm_effect_fall_and_trills(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t base_pitch = 0, pitch = 0, step = 0;
	uint8_t timer = 0, count = 0, rounds = 0, t1 = 0;
	uint16_t t2 = 0;

	z80_block(33);
	timer = CHANNEL(channel)->fm_effect.timer;
	CHANNEL(channel)->fm_effect.timer = timer - 1u;
	if (timer != 1u)
		return fm_effect_trill_slide_down(channel);
	z80_block(52);
	CHANNEL(channel)->fm_effect.timer = 6u;
	count = CHANNEL(channel)->fm_effect.count;
	CHANNEL(channel)->fm_effect.count = count - 1u;
	if (count == 1u)
	{
		z80_block(33);
		rounds = CHANNEL(channel)->fm_effect.rounds;
		CHANNEL(channel)->fm_effect.rounds = rounds - 1u;
		if (rounds == 1u)
			return stop_channel(channel);
		z80_block(56);
		t1 = CHANNEL(channel)->fm_effect.attenuation + 3u;
		attenuation = t1;
		if (t1 == 0u)
			return fm_effect_fall_and_trills_round(channel, attenuation);
		z80_block(17);
		attenuation += 7u;
		return fm_effect_fall_and_trills_round(channel, attenuation);
	}
	z80_block(93);
	step = 0x200u;
	t2 = 0x800u - carry;
	carry = 0x200u + carry > 0xA00u;
	base_pitch = t2;
	CHANNEL(channel)->fm_effect.base_pitch = base_pitch;
	pitch = 0x400u;
	return fm_effect_set_pitch_minus(step, pitch, channel, carry);
}

/* Starts effect 2C and plays two dives from a lower start, then 19 climbing notes. */
uint32_t fm_effect_40(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0, dives = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0x2Cu;
	carry = start_fm_effect(command);
	z80_block(136);
	channel = saved_channel;
	CHANNEL(channel)->fm_effect_40.period = 0x10u;
	CHANNEL(channel)->fm_effect_40.dive_period = 0x14u;
	CHANNEL(channel)->fm_effect_40.climb_notes = 0x13u;
	AS8(CHANNEL(channel)->fm_effect_40.start_pitch) = 0u;
	AT8(CHANNEL(channel)->fm_effect_40.start_pitch, 1) = 0xCu;
	dives = 2u;
	return fm_effect_dive_times(channel, dives);
}

/* A long rise from a high note, four semitones a pass, quieter step by step, 20 times. */
uint32_t fm_effect_41(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0;
	uint16_t handler = 0, ignored16 = 0, pitch = 0, voice = 0;

	z80_block(27);
	voice = 0x3726u;
	carry = load_fm_voice(voice, channel);
	z80_block(43);
	pitch = 0x1200u;
	fm_effect_41_pitch = 0x1200u;
	set_fm_pitch(pitch, channel, &ignored16, &carry);
	z80_block(89);
	fm_effect_41_timer = 8u;
	fm_effect_41_steps = 4u;
	fm_effect_41_rounds = 0x14u;
	attenuation = 4u;
	fm_effect_41_attenuation = 4u;
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	carry = fm_key_on(channel);
	z80_block(20);
	handler = fm_effect_41_rise_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Four semitones up each pass (two passes of every three after the first 8); every 12 passes 2 quieter;
   it stops the twentieth time. */
uint32_t fm_effect_41_rise(uint16_t channel, uint32_t carry)
{
	uint8_t attenuation = 0;
	uint16_t ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0;

	z80_block(31);
	t1 = fm_effect_41_timer;
	fm_effect_41_timer = t1 - 1u;
	if (t1 != 1u)
	{
		z80_block(63);
		pitch = fm_effect_41_pitch + 0x100u;
		fm_effect_41_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
		return carry;
	}
	z80_block(32);
	fm_effect_41_timer = 3u;
	t2 = fm_effect_41_steps;
	fm_effect_41_steps = t2 - 1u;
	if (t2 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(37);
	fm_effect_41_steps = 4u;
	t3 = fm_effect_41_rounds;
	fm_effect_41_rounds = t3 - 1u;
	if (t3 == 1u)
		return stop_channel(channel);
	z80_block(43);
	attenuation = fm_effect_41_attenuation + 2u;
	fm_effect_41_attenuation = attenuation;
	return set_fm_volume(channel, attenuation);
}
