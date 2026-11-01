/**
 *    Name:        pcm_effects.c
 *    Description: the PCM effects (commands 80-93) and their samples
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* Plays a sample on the effect's K007232 channel: its start, the volume and pitch the effect keeps, for
   so many passes. */
uint32_t play_pcm_sample(uint8_t bank, uint16_t start, uint16_t ticks, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t volume = 0;
	uint16_t pitch = 0;

	z80_block(55);
	CHANNEL(channel)->pcm_effect.ticks = ticks;
	set_pcm_start(bank, start, channel);
	z80_block(36);
	volume = CHANNEL(channel)->pcm_effect.volume;
	carry = set_pcm_volume(volume, channel);
	z80_block(55);
	pitch = CHANNEL(channel)->pcm_effect.pitch;
	set_pcm_pitch(pitch, channel);
	z80_block(10);
	start_pcm(channel);
	return carry;
}

/* A PCM effect's last handler: counts the sample's passes down, then stops the channel. */
uint32_t pcm_effect_countdown(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t short_ticks = 0, ticks_high = 0;
	uint16_t t1 = 0;

	z80_block(95);
	t1 = CHANNEL(channel)->pcm_effect.ticks - 1u;
	ticks_high = (uint8_t)(t1 >> 8);
	short_ticks = (uint8_t)t1;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks;
	AT8(CHANNEL(channel)->pcm_effect.ticks, 1) = ticks_high;
	carry = 0u;
	if ((short_ticks | ticks_high) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return stop_pcm_channel(channel);
}

/* Puts the PCM effects' channel A on a handler of the effect's (as sound 88), unless an effect holds the
   music's PCM channels. */
void queue_pcm_effect_a(uint16_t handler)
{
	z80_block(22);
	if (music_pcm_hold != 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(46);
	CHANNEL(PCM_EFFECT_A)->handler = handler;
	CHANNEL(PCM_EFFECT_A)->sound = 0x88u;
}

/* The sample of effects 81, 82, 88 and 8B twice on channel B (240 and 544 passes), and the same on
   channel A 32 passes later. */
uint32_t pcm_effect_81(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(27);
	handler = 0x1114u;
	queue_pcm_effect_a(handler);
	return pcm_effect_81_twice(channel);
}

/* Two plays of the sample, the first 240 passes long. */
uint32_t pcm_effect_81_twice(uint16_t channel)
{
	uint16_t ticks = 0;

	z80_block(29);
	CHANNEL(channel)->pcm_effect.repeats = 2u;
	ticks = 0xF0u;
	return pcm_effect_81_play(ticks, channel);
}

/* The sample of effects 81, 82, 88 and 8B at its pitch and full volume, for the passes it is given. */
uint32_t pcm_effect_81_play(uint16_t ticks, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0;

	z80_block(101);
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0xAEu;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xFu;
	start = 0u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10F8u;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the play down; then the second, 544 passes long, or the end. */
uint32_t pcm_effect_81_next(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t short_ticks = 0, ticks_high = 0;
	uint16_t ticks = 0;
	uint8_t repeats = 0;
	uint16_t t1 = 0;

	z80_block(95);
	t1 = CHANNEL(channel)->pcm_effect.ticks - 1u;
	ticks_high = (uint8_t)(t1 >> 8);
	short_ticks = (uint8_t)t1;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks;
	AT8(CHANNEL(channel)->pcm_effect.ticks, 1) = ticks_high;
	carry = 0u;
	if ((short_ticks | ticks_high) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	repeats = CHANNEL(channel)->pcm_effect.repeats;
	CHANNEL(channel)->pcm_effect.repeats = repeats - 1u;
	if (repeats == 1u)
		return stop_pcm_channel(channel);
	z80_block(20);
	ticks = 0x220u;
	return pcm_effect_81_play(ticks, channel);
}

/* Channel A's part: a wait of 32 passes. */
void pcm_effect_81_echo(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->pcm_effect.wait = 0x20u;
	handler = 0x111Eu;
	set_channel_handler(handler, channel);
}

/* After the wait, the two plays on channel A. */
uint32_t pcm_effect_81_echo_wait(uint16_t channel, uint32_t carry)
{
	uint8_t wait = 0;

	z80_block(28);
	wait = CHANNEL(channel)->pcm_effect.wait;
	CHANNEL(channel)->pcm_effect.wait = wait - 1u;
	if (wait != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return pcm_effect_81_twice(channel);
}

/* The sample of effects 81, 82, 88 and 8B, pitch F84, 72 passes, the music a step quieter, fading out at
   its end. */
uint32_t pcm_effect_82(uint16_t channel)
{
	uint16_t pitch = 0;

	z80_block(10);
	pitch = 0xF84u;
	return pcm_effect_play_ducking(pitch, channel);
}

/* The sample of effects 81, 82, 88 and 8B at the pitch it is given and full volume for 72 passes, with
   the music a step quieter. */
uint32_t pcm_effect_play_ducking(uint16_t pitch, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0, pitch_low = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	pitch_low = (uint8_t)pitch;
	z80_block(134);
	AS8(CHANNEL(channel)->pcm_effect.pitch) = pitch_low;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = ((pitch & ~0xFFu) | pitch_low) >> 8;
	music_attenuation = 1u;
	music_attenuation_changed = 1u;
	ticks = 0x48u;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	start = 0u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x114Bu;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the play down; over its last 30 passes the volume goes down by one every other pass. */
uint32_t pcm_effect_ducking_fade(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t short_ticks = 0, volume = 0;
	uint8_t short_ticks_2 = 0;

	z80_block(33);
	short_ticks_2 = CHANNEL(channel)->pcm_effect.short_ticks;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks_2 - 1u;
	if (short_ticks_2 == 1u)
		return stop_pcm_channel(channel);
	z80_block(31);
	short_ticks = CHANNEL(channel)->pcm_effect.short_ticks;
	carry = short_ticks < 0x1Eu;
	if (short_ticks >= 0x1Eu)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(25);
	if ((CHANNEL(channel)->pcm_effect.short_ticks & 1u) == 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(52);
	CHANNEL(channel)->pcm_effect.volume--;
	volume = CHANNEL(channel)->pcm_effect.volume;
	return set_pcm_volume(volume, channel);
}

/* The sample of effects 83 and 8C, pitch FA3, volume D, 378 passes. */
uint32_t pcm_effect_83(uint16_t channel)
{
	uint16_t pitch = 0, ticks = 0;

	z80_block(39);
	ticks = 0x17Au;
	CHANNEL(channel)->pcm_effect.volume = 0xDu;
	pitch = 0xFA3u;
	return pcm_effect_83_play(pitch, ticks, channel);
}

/* The sample of effects 83 and 8C at the pitch and for the passes it is given (effect 8C's too). */
uint32_t pcm_effect_83_play(uint16_t pitch, uint16_t ticks, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0, pitch_low = 0;
	uint16_t handler = 0, start = 0;

	pitch_low = (uint8_t)pitch;
	z80_block(72);
	AS8(CHANNEL(channel)->pcm_effect.pitch) = pitch_low;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = ((pitch & ~0xFFu) | pitch_low) >> 8;
	start = 0x6320u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 84, 85, 91 and 93, pitch FA0, volume 9, 252 passes. */
uint32_t pcm_effect_84(uint16_t channel)
{
	z80_block(19);
	CHANNEL(channel)->pcm_effect.volume = 9u;
	return pcm_effect_84_sound(channel);
}

/* Effect 84's sound at the volume the effect set (effect 93's too). */
uint32_t pcm_effect_84_sound(uint16_t channel)
{
	uint16_t pitch = 0, ticks = 0;

	z80_block(20);
	ticks = 0xFCu;
	pitch = 0xFA0u;
	return pcm_effect_84_play(pitch, ticks, channel);
}

/* The sample of effects 84, 85, 91 and 93 at the pitch and for the passes it is given (effects 84, 85,
   91 and 93). */
uint32_t pcm_effect_84_play(uint16_t pitch, uint16_t ticks, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0, pitch_low = 0;
	uint16_t handler = 0, start = 0;

	pitch_low = (uint8_t)pitch;
	z80_block(72);
	AS8(CHANNEL(channel)->pcm_effect.pitch) = pitch_low;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = ((pitch & ~0xFFu) | pitch_low) >> 8;
	start = 0x5620u;
	bank = 0u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 84, 85, 91 and 93, pitch F90, full volume, 255 passes. */
uint32_t pcm_effect_85(uint16_t channel)
{
	uint16_t pitch = 0, ticks = 0;

	z80_block(49);
	ticks = 0xFFu;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	pitch = 0xF90u;
	return pcm_effect_84_play(pitch, ticks, channel);
}

/* Holds the music's PCM channels, starts PCM effect 8D and plays the sample of effects 86, 8D and 90,
   pitch FD3, 304 passes on channel A. */
uint32_t pcm_effect_86(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t command = 0;
	uint16_t handler = 0, pitch = 0;
	uint16_t saved_channel = 0;

	z80_block(59);
	music_pcm_hold = 1u;
	saved_channel = channel;
	command = 0x8Du;
	start_pcm_effect(command);
	z80_block(41);
	channel = saved_channel;
	pitch = 0xFD3u;
	carry = pcm_effect_86_play(pitch, channel);
	z80_block(20);
	handler = 0x11C8u;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the play down; at its end the music's PCM channels are let go and the channel stops. */
uint32_t pcm_effect_86_countdown(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t short_ticks = 0, ticks_high = 0;
	uint16_t t1 = 0;

	z80_block(95);
	t1 = CHANNEL(channel)->pcm_effect.ticks - 1u;
	ticks_high = (uint8_t)(t1 >> 8);
	short_ticks = (uint8_t)t1;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks;
	AT8(CHANNEL(channel)->pcm_effect.ticks, 1) = ticks_high;
	carry = 0u;
	if ((short_ticks | ticks_high) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(27);
	music_pcm_hold = 0u;
	return stop_pcm_channel(channel);
}

/* The sample of effects 86, 8D and 90 at the pitch it is given, full volume, 304 passes (effect 8D's
   too). */
uint32_t pcm_effect_86_play(uint16_t pitch, uint16_t channel)
{
	uint8_t bank = 0, pitch_low = 0;
	uint16_t start = 0, ticks = 0;

	pitch_low = (uint8_t)pitch;
	z80_block(94);
	AS8(CHANNEL(channel)->pcm_effect.pitch) = pitch_low;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = ((pitch & ~0xFFu) | pitch_low) >> 8;
	ticks = speech_priorities_ADDRESS;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	start = 0x9BE0u;
	bank = 1u;
	return play_pcm_sample(bank, start, ticks, channel);
}

/* Waits 8 passes, then plays effect 86's sample a little higher. */
void pcm_effect_8d(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->pcm_effect.wait = 8u;
	handler = 0x11FEu;
	set_channel_handler(handler, channel);
}

/* After the wait, the sample of effects 86, 8D and 90, pitch FD5, then its countdown. */
uint32_t pcm_effect_8d_wait(uint16_t channel, uint32_t carry)
{
	uint16_t handler = 0, pitch = 0;
	uint8_t wait = 0;

	z80_block(28);
	wait = CHANNEL(channel)->pcm_effect.wait;
	CHANNEL(channel)->pcm_effect.wait = wait - 1u;
	if (wait != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(27);
	pitch = 0xFD5u;
	carry = pcm_effect_86_play(pitch, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 87, 89 and 8A, pitch FA3, full volume, 72 passes. */
uint32_t pcm_effect_87(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	z80_block(111);
	ticks = 0x48u;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0xA3u;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xFu;
	start = 0x5550u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 81, 82, 88 and 8B, pitch F94, 672 passes, and the same on channel A 2 passes
   later. */
uint32_t pcm_effect_88(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(27);
	handler = 0x1250u;
	queue_pcm_effect_a(handler);
	return pcm_effect_88_play(channel);
}

/* The sample of effects 81, 82, 88 and 8B, pitch F94, full volume, 672 passes, then its countdown. */
uint32_t pcm_effect_88_play(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	z80_block(111);
	ticks = 0x2A0u;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0x94u;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xFu;
	start = 0u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* Channel A's part: a wait of 2 passes. */
void pcm_effect_88_echo(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->pcm_effect.short_ticks = 2u;
	handler = 0x125Au;
	set_channel_handler(handler, channel);
}

/* After the wait, the sample on channel A. */
uint32_t pcm_effect_88_echo_wait(uint16_t channel, uint32_t carry)
{
	uint8_t short_ticks = 0;

	z80_block(28);
	short_ticks = CHANNEL(channel)->pcm_effect.short_ticks;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks - 1u;
	if (short_ticks != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(10);
	return pcm_effect_88_play(channel);
}

/* Speech A3 and the sample of effects 87, 89 and 8A, pitch F88, twice (20 and 96 passes). */
uint32_t pcm_effect_89(uint16_t channel)
{
	uint8_t command = 0;
	uint16_t ticks = 0;
	uint16_t saved_channel = 0;

	z80_block(39);
	saved_channel = channel;
	command = 0xA3u;
	start_speech(command);
	z80_block(43);
	channel = saved_channel;
	CHANNEL(channel)->pcm_effect.repeats = 2u;
	ticks = 0x14u;
	return pcm_effect_89_play(ticks, channel);
}

/* The sample of effects 87, 89 and 8A, pitch F88, full volume, for the passes it is given. */
uint32_t pcm_effect_89_play(uint16_t ticks, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0;

	z80_block(101);
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0x88u;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xFu;
	start = 0x5550u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x128Cu;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the play down; then the second, 96 passes long, or the end. */
uint32_t pcm_effect_89_next(uint16_t channel, uint32_t carry)
{
	uint16_t ticks = 0;
	uint8_t short_ticks = 0, repeats = 0;

	z80_block(28);
	short_ticks = CHANNEL(channel)->pcm_effect.short_ticks;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks - 1u;
	if (short_ticks != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	repeats = CHANNEL(channel)->pcm_effect.repeats;
	CHANNEL(channel)->pcm_effect.repeats = repeats - 1u;
	if (repeats == 1u)
		return stop_pcm_channel(channel);
	z80_block(20);
	ticks = 0x60u;
	return pcm_effect_89_play(ticks, channel);
}

/* The sample of effects 87, 89 and 8A, pitch FAA, 52 passes, twice, the second 10 quieter. */
uint32_t pcm_effect_8a(uint16_t channel)
{
	uint8_t volume = 0;

	z80_block(26);
	CHANNEL(channel)->pcm_effect.repeats = 2u;
	volume = 0xFu;
	return pcm_effect_8a_play(channel, volume);
}

/* The sample of effects 87, 89 and 8A, pitch FAA, 52 passes, at the volume it is given. */
uint32_t pcm_effect_8a_play(uint16_t channel, uint8_t volume)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	z80_block(111);
	CHANNEL(channel)->pcm_effect.volume = volume;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0xAAu;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xFu;
	ticks = 0x34u;
	start = 0x5550u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x12BFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the play down; then the second, 10 quieter, or the end. */
uint32_t pcm_effect_8a_next(uint16_t channel, uint32_t carry)
{
	uint8_t volume = 0;
	uint8_t short_ticks = 0, repeats = 0;

	z80_block(28);
	short_ticks = CHANNEL(channel)->pcm_effect.short_ticks;
	CHANNEL(channel)->pcm_effect.short_ticks = short_ticks - 1u;
	if (short_ticks != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	repeats = CHANNEL(channel)->pcm_effect.repeats;
	CHANNEL(channel)->pcm_effect.repeats = repeats - 1u;
	if (repeats == 1u)
		return stop_pcm_channel(channel);
	z80_block(36);
	volume = CHANNEL(channel)->pcm_effect.volume - 0xAu;
	return pcm_effect_8a_play(channel, volume);
}

/* The sample of effects 81, 82, 88 and 8B, pitch FA8, four times, each 12 passes longer and quieter; and
   on channel A, 8 passes later, the same at pitch F68. */
uint32_t pcm_effect_8b(uint16_t channel)
{
	uint16_t handler = 0, pitch = 0;

	z80_block(27);
	handler = 0x1338u;
	queue_pcm_effect_a(handler);
	z80_block(10);
	pitch = 0xFA8u;
	return pcm_effect_8b_start(pitch, channel);
}

/* Four plays of the sample at the pitch it is given, the first at full volume for 128 passes. */
uint32_t pcm_effect_8b_start(uint16_t pitch, uint16_t channel)
{
	uint16_t ticks = 0;

	z80_block(86);
	CHANNEL(channel)->pcm_effect_8b.pitch = pitch;
	CHANNEL(channel)->pcm_effect_8b.repeats = 4u;
	CHANNEL(channel)->pcm_effect_8b.volume = 0xFu;
	ticks = 0x80u;
	return pcm_effect_8b_play(ticks, channel);
}

/* The sample of effects 81, 82, 88 and 8B for the passes it is given, which it keeps. */
uint32_t pcm_effect_8b_play(uint16_t ticks, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0;

	z80_block(53);
	CHANNEL(channel)->pcm_effect_8b.play_length = (uint8_t)ticks;
	start = 0u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x12FCu;
	set_channel_handler(handler, channel);
	return carry;
}

/* Counts the play down; then the next, silenced first, 12 passes longer and 5 quieter (3 in the last,
   which also gives up the channel's sound); at the end the channel stops, or only frees itself while the
   music's PCM channel A plays. */
uint32_t pcm_effect_8b_next(uint16_t channel, uint32_t carry)
{
	uint8_t volume = 0, volume_2 = 0;
	uint16_t ticks = 0;
	uint8_t short_ticks = 0, repeats = 0;

	z80_block(28);
	short_ticks = CHANNEL(channel)->pcm_effect_8b.short_ticks;
	CHANNEL(channel)->pcm_effect_8b.short_ticks = short_ticks - 1u;
	if (short_ticks != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(33);
	repeats = CHANNEL(channel)->pcm_effect_8b.repeats;
	CHANNEL(channel)->pcm_effect_8b.repeats = repeats - 1u;
	if (repeats == 1u)
	{
		z80_block(27);
		carry = 0u;
		if (CHANNEL(MUSIC_PCM_A)->sound == 0u)
			return stop_pcm_channel(channel);
		z80_block(10);
		free_channel(channel);
		return carry;
	}
	z80_block(24);
	volume = 0u;
	carry = set_pcm_volume(volume, channel);
	z80_block(17);
	carry = clear_pcm_loop(channel);
	z80_block(49);
	volume_2 = CHANNEL(channel)->pcm_effect_8b.volume;
	if (((CHANNEL(channel)->pcm_effect_8b.repeats >> 1) & 1u) == 0u)
	{
		z80_block(26);
		CHANNEL(channel)->sound = 0u;
		volume_2 += 2u;
	}
	z80_block(73);
	CHANNEL(channel)->pcm_effect_8b.volume = volume_2 - 5u;
	ticks = (uint8_t)(CHANNEL(channel)->pcm_effect_8b.play_length + 0xCu);
	return pcm_effect_8b_play(ticks, channel);
}

/* Channel A's part: a wait of 8 passes. */
void pcm_effect_8b_echo(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(39);
	CHANNEL(channel)->pcm_effect_8b.repeats = 8u;
	handler = 0x1342u;
	set_channel_handler(handler, channel);
}

/* After the wait, the four plays at pitch F68. */
uint32_t pcm_effect_8b_echo_wait(uint16_t channel, uint32_t carry)
{
	uint16_t pitch = 0;
	uint8_t repeats = 0;

	z80_block(28);
	repeats = CHANNEL(channel)->pcm_effect_8b.repeats;
	CHANNEL(channel)->pcm_effect_8b.repeats = repeats - 1u;
	if (repeats != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(20);
	pitch = 0xF68u;
	return pcm_effect_8b_start(pitch, channel);
}

/* The sample of effects 83 and 8C, pitch FB0, volume B, 80 passes. */
uint32_t pcm_effect_8c(uint16_t channel)
{
	uint16_t pitch = 0, ticks = 0;

	z80_block(49);
	ticks = 0x50u;
	CHANNEL(channel)->pcm_effect.volume = 0xBu;
	pitch = 0xFB0u;
	return pcm_effect_83_play(pitch, ticks, channel);
}

/* The sample of effects 8E and 8F, pitch EF0, full volume, 640 passes, and effect 88's sample on channel
   A. */
uint32_t pcm_effect_8e(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	z80_block(27);
	handler = 0x1232u;
	queue_pcm_effect_a(handler);
	z80_block(111);
	ticks = 0x280u;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0xF0u;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xEu;
	start = 0xD281u;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	bank = 0u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 8E and 8F, pitch F7C, full volume, 272 passes, the music a step quieter. */
uint32_t pcm_effect_8f(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	z80_block(144);
	music_attenuation = 1u;
	music_attenuation_changed = 1u;
	ticks = 0x110u;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0x7Cu;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xFu;
	start = 0xD281u;
	bank = 0u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 86, 8D and 90, pitch E40, full volume, 1344 passes. */
uint32_t pcm_effect_90(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t bank = 0;
	uint16_t handler = 0, start = 0, ticks = 0;

	z80_block(111);
	ticks = 0x540u;
	CHANNEL(channel)->pcm_effect.volume = 0xFu;
	AS8(CHANNEL(channel)->pcm_effect.pitch) = 0x40u;
	AT8(CHANNEL(channel)->pcm_effect.pitch, 1) = 0xEu;
	start = 0x9BE0u;
	bank = 1u;
	carry = play_pcm_sample(bank, start, ticks, channel);
	z80_block(20);
	handler = 0x10AFu;
	set_channel_handler(handler, channel);
	return carry;
}

/* The sample of effects 84, 85, 91 and 93, pitch FB0, volume 9, 248 passes. */
uint32_t pcm_effect_91(uint16_t channel)
{
	uint16_t pitch = 0, ticks = 0;

	z80_block(49);
	ticks = 0xF8u;
	CHANNEL(channel)->pcm_effect.volume = 9u;
	pitch = 0xFB0u;
	return pcm_effect_84_play(pitch, ticks, channel);
}

/* Effect 82's sound at pitch F98. */
uint32_t pcm_effect_92(uint16_t channel)
{
	uint16_t pitch = 0;

	z80_block(20);
	pitch = 0xF98u;
	return pcm_effect_play_ducking(pitch, channel);
}

/* Effect 84's sound at volume C. */
uint32_t pcm_effect_93(uint16_t channel)
{
	z80_block(29);
	CHANNEL(channel)->pcm_effect.volume = 0xCu;
	return pcm_effect_84_sound(channel);
}
