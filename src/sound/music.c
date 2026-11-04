/**
 *    Name:        music.c
 *    Description: the music: the random bytes, the tracks' tick, their commands and notes, the note hooks
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* Reads the next of the random bytes and moves the place on; the FM effects that call it take the place,
   not the byte. */
uint16_t next_random_byte(void)
{
	uint16_t random_place = 0;

	z80_block(53);
	random_place = random_read;
	random_place = (random_place & ~0xFFu) | (uint8_t)(random_place + 1u);
	random_read = random_place;
	return random_place;
}

/* A music channel's first pass: keyed off (but the music's channels 6 and 7 while an FM effect has
   them), its record cleared, its track from the song's header. */
uint32_t music_track_start(uint16_t channel)
{
	uint32_t zero = 0, carry = 0;
	uint8_t chip_channel = 0, count = 0, entry_offset = 0, sound = 0;
	uint16_t handler = 0, place = 0, song_entry = 0;
	uint16_t saved_channel = 0;

	z80_block(36);
	chip_channel = CHANNEL(channel)->chip_channel;
	if (chip_channel < 8u)
	{
		z80_block(17);
		zero = (uint8_t)(chip_channel - 6u) == 0u;
		if (chip_channel < 6u)
		{
			z80_block(17);
			fm_key_off(channel);
			z80_block(17);
			carry = fm_fast_release(channel);
		}
		else
		{
			z80_block(23);
			sound = CHANNEL(FM_EFFECT_6)->sound;
			if (zero == 0u)
			{
				z80_block(13);
				sound = CHANNEL(FM_EFFECT_7)->sound;
			}
			z80_block(14);
			if (sound == 0u)
			{
				z80_block(17);
				fm_key_off(channel);
				z80_block(17);
				carry = fm_fast_release(channel);
			}
		}
	}
	z80_block(53);
	saved_channel = channel;
	count = 0x2Eu;
	place = saved_channel + 0x12u;
	for (;;)
	{
		z80_block(24);
		z80_write8(place, 0u);
		place++;
		count--;
		if (count == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(139);
	entry_offset = CHANNEL(channel)->chip_channel;
	entry_offset = ((uint32_t)entry_offset << 1) | (entry_offset >> 7);
	song_entry = song_header;
	carry = (uint16_t)(song_entry + entry_offset) < song_entry;
	song_entry += entry_offset;
	AS8(CHANNEL(channel)->track) = z80_read8(song_entry);
	AT8(CHANNEL(channel)->track, 1) = z80_read8((uint16_t)(song_entry + 1u));
	handler = music_track_first_tick_ADDRESS;
	set_channel_handler(handler, channel);
	return carry;
}

/* Reads the track's first commands; from the next pass on the tempo runs the channel. */
uint32_t music_track_first_tick(uint16_t channel)
{
	uint16_t handler = 0;

	z80_block(27);
	handler = music_track_tick_ADDRESS;
	set_channel_handler(handler, channel);
	z80_block(10);
	return read_track(channel);
}

/* A music channel's pass: the tempo, and at a note's end the track's next commands; else the note's
   key-off, its attenuation again when the music's changes, the portamento and the LFO sensitivity's
   ramp, or a PCM note's decay. */
uint32_t music_track_tick(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t attenuation = 0, count = 0, distance_high = 0, distance_low = 0, fraction = 0, key_off_ticks = 0, last_tempo = 0, lfo_ramp = 0, lfo_sensitivity = 0, pcm_level = 0, portamento_fraction = 0, portamento_fraction_2 = 0, sensitivity_bits = 0, sound = 0, volume = 0;
	uint16_t distance = 0, fm_write = 0, ignored16 = 0, pitch = 0;
	uint8_t t1 = 0, tempo_count = 0, t2 = 0, ticks_left = 0, ramp_wait = 0, ticks_left_2 = 0, t3 = 0, t4 = 0, portamento = 0, ramp_wait_2 = 0;
	uint32_t jump_target = 0;

	z80_block(36);
	if (CHANNEL(channel)->chip_channel == 1u)
	{
		z80_cycles(7);
		accelerando_tick();
	}
	z80_block(44);
	t1 = tempo_increase + tempo;
	last_tempo = t1;
	if (t1 == 0u)
		return stop_channel(channel);
	z80_block(67);
	CHANNEL(channel)->last_tempo = last_tempo;
	tempo_count = CHANNEL(channel)->tempo_count;
	t2 = last_tempo;
	CHANNEL(channel)->tempo_count = t2 + tempo_count;
	if ((uint8_t)(t2 + tempo_count) < t2)
	{
		z80_block(33);
		ticks_left = CHANNEL(channel)->ticks_left;
		CHANNEL(channel)->ticks_left = ticks_left - 1u;
		if (ticks_left == 1u)
			return read_track(channel);
	}
	z80_block(30);
	if (((CHANNEL(channel)->chip_channel >> 3) & 1u) != 0u)
	{
		z80_block(43);
		sound = CHANNEL(PCM_EFFECT_A)->sound;
		if (CHANNEL(channel)->chip_channel & 1u)
		{
			z80_block(13);
			sound = CHANNEL(PCM_EFFECT_B)->sound;
		}
		z80_block(9);
		carry = 0u;
		if (sound != 0u)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(28);
		ramp_wait = CHANNEL(channel)->ramp_wait;
		CHANNEL(channel)->ramp_wait = ramp_wait - 1u;
		if (ramp_wait != 1u)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(69);
		CHANNEL(channel)->ramp_wait = 3u;
		key_off_ticks = CHANNEL(channel)->key_off_ticks;
		ticks_left_2 = CHANNEL(channel)->ticks_left;
		carry = key_off_ticks < ticks_left_2;
		if (key_off_ticks < ticks_left_2)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(31);
		pcm_level = CHANNEL(channel)->pcm_level;
		carry = pcm_level < 2u;
		if (pcm_level < 2u)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(37);
		pcm_level--;
		CHANNEL(channel)->pcm_level = pcm_level;
		volume = pcm_level;
		return set_pcm_volume(volume, channel);
	}
	z80_block(17);
	carry = check_fm_effect_override(channel);
	z80_block(25);
	if ((CHANNEL(channel)->flags & 1u) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(30);
	if ((CHANNEL(channel)->mode & 1u) != 0u)
	{
		z80_block(42);
		jump_target = CHANNEL(channel)->note_hook;
		switch (jump_target)
		{
		case note_hook_none_ADDRESS:
			break;
		default:
			sound_invalid_jump(jump_target);
		}
		note_hook_none();
		return carry;
	}
	z80_block(27);
	if (music_attenuation_changed != 0u)
	{
		z80_block(141);
		t3 = music_volume + (fade_step + music_attenuation) + CHANNEL(channel)->volume_offset + CHANNEL(channel)->loop_volume_offset + CHANNEL(channel)->attenuation + CHANNEL(channel)->volume;
		attenuation = t3;
		if ((int8_t)t3 < 0)
		{
			z80_block(7);
			attenuation = 0x7Fu;
		}
		z80_block(17);
		carry = set_fm_volume(channel, attenuation);
	}
	z80_block(48);
	if (CHANNEL(channel)->ticks_left == CHANNEL(channel)->key_off_ticks)
	{
		z80_cycles(7);
		fm_key_off(channel);
	}
	z80_block(33);
	if (CHANNEL(channel)->portamento != 0u)
	{
		z80_block(147);
		fraction = CHANNEL(channel)->portamento_fraction;
		carry = fraction > 0u;
		distance = CHANNEL(channel)->target_pitch - CHANNEL(channel)->portamento_pitch - carry;
		fraction = 0u - fraction;
		distance_high = (uint8_t)(distance >> 8);
		distance_low = (uint8_t)distance;
		portamento_fraction = 0u;
		pitch = 0u;
		count = 8u;
		for (;;)
		{
			z80_block(57);
			carry = distance_high & 1u;
			distance_high = (int8_t)distance_high >> 1;
			t4 = (distance_low >> 1) | (carry << 7);
			carry = distance_low & 1u;
			distance_low = t4;
			fraction = (fraction >> 1) | (carry << 7);
			portamento = CHANNEL(channel)->portamento;
			CHANNEL(channel)->portamento = ((uint32_t)portamento << 1) | (portamento >> 7);
			if (portamento >> 7)
			{
				z80_block(19);
				carry = (uint8_t)(portamento_fraction + fraction) < portamento_fraction;
				portamento_fraction += fraction;
				pitch = pitch + (((uint32_t)distance_high << 8) | distance_low) + carry;
			}
			z80_block(8);
			count--;
			if (count == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(150);
		portamento_fraction_2 = CHANNEL(channel)->portamento_fraction;
		carry = (uint8_t)(portamento_fraction + portamento_fraction_2) < portamento_fraction;
		portamento_fraction += portamento_fraction_2;
		pitch = pitch + CHANNEL(channel)->portamento_pitch + carry;
		CHANNEL(channel)->portamento_fraction = portamento_fraction;
		CHANNEL(channel)->portamento_pitch = pitch;
		if ((uint8_t)(portamento_fraction - 0x80u) < portamento_fraction)
		{
			z80_block(6);
			pitch++;
		}
		z80_block(17);
		set_fm_pitch(pitch, channel, &ignored16, &carry);
	}
	z80_block(28);
	lfo_ramp = CHANNEL(channel)->lfo_ramp;
	carry = 0u;
	if (lfo_ramp == 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(28);
	ramp_wait_2 = CHANNEL(channel)->ramp_wait;
	CHANNEL(channel)->ramp_wait = ramp_wait_2 - 1u;
	if (ramp_wait_2 != 1u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(62);
	CHANNEL(channel)->ramp_wait = lfo_ramp;
	lfo_sensitivity = CHANNEL(channel)->lfo_sensitivity + 0x11u;
	if (lfo_sensitivity >= 0x88u)
	{
		z80_block(7);
		lfo_sensitivity = 0x77u;
	}
	z80_block(85);
	CHANNEL(channel)->lfo_sensitivity = lfo_sensitivity;
	sensitivity_bits = lfo_sensitivity & 0x70u;
	lfo_sensitivity = CHANNEL(channel)->lfo_sensitivity;
	fm_write = 0x3800u | (((lfo_sensitivity >> 1) | ((uint32_t)lfo_sensitivity << 7)) & 3u) | sensitivity_bits;
	write_fm_channel_register(channel, &fm_write, &carry);
	return carry;
}

/* Puts the note hook that does nothing back on the channel. */
void clear_note_hook(uint16_t channel)
{
	z80_block(48);
	AS8(CHANNEL(channel)->note_hook) = 0xF5u;
	AT8(CHANNEL(channel)->note_hook, 1) = 9u;
	note_hook_none();
}

/* The note hook that does nothing. */
void note_hook_none(void)
{
	z80_block(10);
}

/* The music speeds up: the tempo one faster every so many passes, as the accelerando command set it. */
void accelerando_tick(void)
{
	uint8_t rate = 0;
	uint8_t t1 = 0, t2 = 0;

	z80_block(25);
	t1 = (uint8_t)accelerando & 0xFu;
	rate = t1;
	if (t1 == 0u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(26);
	t2 = (uint8_t)(accelerando >> 8u);
	AT8(accelerando, 1) = t2 - 1u;
	if (t2 != 1u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(70);
	AT8(accelerando, 1) = ((((uint32_t)rate << 1) << 1) << 1) << 1;
	tempo_increase++;
}

/* On the music's FM channels 6 and 7: while an FM effect plays there, the music keeps its note keyed off
   and its instrument to load again. */
uint32_t check_fm_effect_override(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t sound = 0;
	uint16_t fm_write = 0;
	uint8_t chip_channel = 0;

	z80_block(49);
	sound = CHANNEL(FM_EFFECT_6)->sound;
	if (6u != CHANNEL(channel)->chip_channel)
	{
		z80_block(31);
		chip_channel = CHANNEL(channel)->chip_channel;
		carry = chip_channel > 7u;
		if (7u != chip_channel)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(13);
		sound = CHANNEL(FM_EFFECT_7)->sound;
	}
	z80_block(9);
	carry = 0u;
	if (sound == 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(25);
	if ((CHANNEL(channel)->flags & 1u) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(66);
	CHANNEL(channel)->flags |= 1u;
	CHANNEL(channel)->flags |= 0x80u;
	fm_write = 0x3800u;
	write_fm_channel_register(channel, &fm_write, &carry);
	return carry;
}

/* Reads the track's commands up to the next note and plays it: a note, a rest, or the end of the track. */
uint32_t read_track(uint16_t channel)
{
	bool play_note = false;
	uint32_t zero = 0, carry = 0;
	uint8_t attenuation = 0, bank = 0, bank_2 = 0, byte = 0, chip_channel = 0, command = 0, count = 0, fm_extra = 0, instrument = 0, key_off_ticks = 0, length_code = 0, length_codes = 0, level = 0, note_index = 0, octave = 0, pcm_level = 0, sound = 0, total_attenuation = 0, transpose = 0, value = 0, volume = 0, volume_offset = 0;
	uint16_t command_entry = 0, fm_write = 0, gate = 0, gated_length = 0, ignored16 = 0, pcm_sound = 0, pitch = 0, pitch_2 = 0, place = 0, start = 0, track = 0;
	uint16_t saved_place = 0;
	uint8_t t1 = 0, t2 = 0, transposed_loop_count = 0, t3 = 0, loop_exit_count = 0, t4 = 0, loop_count = 0, loop_2_count = 0, repeat_count = 0, t5 = 0, t6 = 0, t7 = 0, t9 = 0, t10 = 0;
	uint16_t t8 = 0;
	uint32_t jump_target = 0;

	z80_block(40);
	CHANNEL(channel)->flags &= ~1u;
	carry = check_fm_effect_override(channel);
	for (;;)
	{
		z80_block(70);
		track = CHANNEL(channel)->track;
		byte = z80_read8(track);
		command = byte;
		t1 = byte & ~0xFu;
		value = t1;
		place = (uint32_t)value << 8;
		if (t1 == 0u)
		{
			z80_block(45);
			place = track;
			octave = command & 0xFu;
			if (((CHANNEL(channel)->chip_channel >> 3) & 1u) == 0u)
			{
				z80_block(40);
				octave = (((uint32_t)octave << 1) << 1) << 1;
				octave += (uint32_t)octave << 1;
			}
			z80_block(29);
			CHANNEL(channel)->octave = octave;
			z80_block(6);
			place++;
			z80_block(48);
			CHANNEL(channel)->track = place;
			continue;
		}
		z80_block(17);
		if (value >= 0xE0u)
		{
			z80_block(31);
			place = track + 1u;
			t2 = value;
			value = command;
			if (t2 == 0xF0u)
			{
				z80_block(25);
				value = (value & 0xFu) << 1;
				command_entry = track_commands_f0_ADDRESS;
			}
			else
			{
				z80_block(35);
				value = (value & 0xFu) << 1;
				command_entry = track_commands_e0_ADDRESS;
			}
			z80_block(50);
			command_entry += value;
			jump_target = TRACK_COMMAND_HANDLER(command_entry)[0];
			switch (jump_target)
			{
			case track_set_mode_ADDRESS:
				z80_block(36);
				CHANNEL(channel)->mode = z80_read8(place);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_tempo_ADDRESS:
				z80_block(30);
				tempo = z80_read8(place);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_instrument_ADDRESS:
				z80_block(56);
				instrument = z80_read8(place);
				CHANNEL(channel)->instrument = instrument;
				if (((CHANNEL(channel)->chip_channel >> 3) & 1u) != 0u)
				{
					z80_block(6);
					place++;
					z80_block(48);
					CHANNEL(channel)->track = place;
					continue;
				}
				z80_block(30);
				if (CHANNEL(channel)->flags >> 7 == 0u)
				{
					z80_cycles(7);
					carry = load_instrument(channel, instrument);
				}
				z80_block(10);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_volume_ADDRESS:
				z80_block(44);
				value = z80_read8(place);
				fm_extra = 0u;
				if (((CHANNEL(channel)->chip_channel >> 3) & 1u) == 0u)
				{
					z80_block(7);
					fm_extra = 0xAu;
				}
				z80_block(52);
				CHANNEL(channel)->volume = value + fm_extra;
				CHANNEL(channel)->attenuation = 0u;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_gate_ADDRESS:
				z80_block(36);
				CHANNEL(channel)->gate = z80_read8(place);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_held_note_ADDRESS:
				break;
			case track_set_detune_ADDRESS:
				z80_block(36);
				CHANNEL(channel)->detune = z80_read8(place);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_portamento_ADDRESS:
				z80_block(36);
				CHANNEL(channel)->portamento = z80_read8(place);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_music_transpose_ADDRESS:
				z80_block(17);
				value = read_transpose(place);
				z80_block(23);
				music_transpose = value;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_transpose_ADDRESS:
				z80_block(17);
				transpose = read_transpose(place);
				z80_block(29);
				CHANNEL(channel)->transpose = transpose;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_music_volume_ADDRESS:
				z80_block(17);
				value = read_volume_offset(place);
				z80_block(23);
				music_volume = value;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_volume_offset_ADDRESS:
				z80_block(17);
				volume_offset = read_volume_offset(place);
				z80_block(29);
				CHANNEL(channel)->volume_offset = volume_offset;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_loop_transposed_ADDRESS:
				z80_block(33);
				transposed_loop_count = CHANNEL(channel)->transposed_loop_count;
				CHANNEL(channel)->transposed_loop_count = transposed_loop_count - 1u;
				if (transposed_loop_count == 1u)
				{
					z80_block(60);
					CHANNEL(channel)->loop_transpose = 0u;
					CHANNEL(channel)->loop_volume_offset = 0u;
					place += 2u;
					z80_block(12);
					place += 2u;
					z80_block(6);
					place++;
					z80_block(48);
					CHANNEL(channel)->track = place;
					continue;
				}
				z80_block(33);
				t3 = CHANNEL(channel)->transposed_loop_count + 1u;
				CHANNEL(channel)->transposed_loop_count = t3;
				if (t3 == 0u)
				{
					z80_block(64);
					CHANNEL(channel)->transposed_loop_count = z80_read8(place);
					CHANNEL(channel)->loop_transpose = 0u;
					CHANNEL(channel)->loop_volume_offset = 0u;
				}
				z80_block(46);
				CHANNEL(channel)->transposed_loop_count--;
				place++;
				value = read_transpose(place);
				z80_block(61);
				CHANNEL(channel)->loop_transpose += value;
				place++;
				value = read_volume_offset(place);
				z80_block(54);
				CHANNEL(channel)->loop_volume_offset += value;
				place++;
				z80_block(68);
				AS8(CHANNEL(channel)->track) = z80_read8(place);
				AT8(CHANNEL(channel)->track, 1) = z80_read8((uint16_t)(place + 1u));
				continue;
			case track_set_lfo_ADDRESS:
				z80_block(35);
				fm_write = 0x1800u | z80_read8(place);
				carry = write_fm_register(fm_write);
				z80_block(48);
				place++;
				fm_write = 0x1900u | z80_read8(place) | 0x80u;
				carry = write_fm_register(fm_write);
				z80_block(31);
				fm_write = 0x1900u;
				carry = write_fm_register(fm_write);
				z80_block(31);
				fm_write = 0x1B02u;
				carry = write_fm_register(fm_write);
				z80_block(10);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_lfo_ramp_ADDRESS:
				z80_block(36);
				CHANNEL(channel)->lfo_ramp = z80_read8(place);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_nop_ADDRESS:
				z80_block(10);
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_jump_ADDRESS:
				z80_block(68);
				AS8(CHANNEL(channel)->track) = z80_read8(place);
				AT8(CHANNEL(channel)->track, 1) = z80_read8((uint16_t)(place + 1u));
				continue;
			case track_loop_exit_ADDRESS:
				z80_block(33);
				loop_exit_count = CHANNEL(channel)->loop_exit_count;
				CHANNEL(channel)->loop_exit_count = loop_exit_count - 1u;
				if (loop_exit_count == 1u)
				{
					z80_block(16);
					place++;
					z80_block(68);
					AS8(CHANNEL(channel)->track) = z80_read8(place);
					AT8(CHANNEL(channel)->track, 1) = z80_read8((uint16_t)(place + 1u));
					continue;
				}
				z80_block(33);
				t4 = CHANNEL(channel)->loop_exit_count + 1u;
				CHANNEL(channel)->loop_exit_count = t4;
				if (t4 == 0u)
				{
					z80_block(26);
					CHANNEL(channel)->loop_exit_count = z80_read8(place);
				}
				z80_block(33);
				CHANNEL(channel)->loop_exit_count--;
				z80_block(12);
				place += 2u;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_loop_start_ADDRESS:
				z80_block(80);
				CHANNEL(channel)->loop_count = z80_read8(place);
				place++;
				CHANNEL(channel)->loop_start = place;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_loop_end_ADDRESS:
				z80_block(33);
				loop_count = CHANNEL(channel)->loop_count;
				CHANNEL(channel)->loop_count = loop_count - 1u;
				if (loop_count == 1u)
				{
					z80_block(48);
					CHANNEL(channel)->track = place;
					continue;
				}
				z80_block(48);
				place = CHANNEL(channel)->loop_start;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_loop_2_start_ADDRESS:
				z80_block(80);
				CHANNEL(channel)->loop_2_count = z80_read8(place);
				place++;
				CHANNEL(channel)->loop_2_start = place;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_loop_2_end_ADDRESS:
				z80_block(33);
				loop_2_count = CHANNEL(channel)->loop_2_count;
				CHANNEL(channel)->loop_2_count = loop_2_count - 1u;
				if (loop_2_count == 1u)
				{
					z80_block(48);
					CHANNEL(channel)->track = place;
					continue;
				}
				z80_block(48);
				place = CHANNEL(channel)->loop_2_start;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_call_repeat_ADDRESS:
				z80_block(49);
				CHANNEL(channel)->repeat_count = z80_read8(place);
				place++;
				place = read_track_address(place, channel);
				z80_block(48);
				CHANNEL(channel)->repeat_return = place;
				continue;
			case track_return_repeat_ADDRESS:
				z80_block(71);
				repeat_count = CHANNEL(channel)->repeat_count;
				CHANNEL(channel)->repeat_count = repeat_count - 1u;
				place = CHANNEL(channel)->repeat_return;
				if (repeat_count == 1u)
				{
					z80_block(48);
					CHANNEL(channel)->track = place;
					continue;
				}
				z80_block(74);
				place--;
				AT8(CHANNEL(channel)->track, 1) = z80_read8(place);
				AS8(CHANNEL(channel)->track) = z80_read8((uint16_t)(place - 1u));
				continue;
			case track_call_ADDRESS:
				z80_block(17);
				place = read_track_address(place, channel);
				z80_block(48);
				CHANNEL(channel)->call_return = place;
				continue;
			case track_return_ADDRESS:
				z80_block(48);
				place = CHANNEL(channel)->call_return;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_call_2_ADDRESS:
				z80_block(17);
				place = read_track_address(place, channel);
				z80_block(48);
				CHANNEL(channel)->call_2_return = place;
				continue;
			case track_return_2_ADDRESS:
				z80_block(48);
				place = CHANNEL(channel)->call_2_return;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_set_accelerando_ADDRESS:
				z80_block(99);
				value = z80_read8(place);
				AS8(accelerando) = value;
				AT8(accelerando, 1) = ((((value & 0xFu) << 1) << 1) << 1) << 1;
				tempo_increase = 0u;
				z80_block(6);
				place++;
				z80_block(48);
				CHANNEL(channel)->track = place;
				continue;
			case track_end_ADDRESS:
				z80_block(36);
				chip_channel = CHANNEL(channel)->chip_channel;
				zero = (uint8_t)(chip_channel - 8u) == 0u;
				if (chip_channel >= 8u)
				{
					z80_block(23);
					value = CHANNEL(PCM_EFFECT_B)->sound;
					if (zero)
					{
						z80_block(13);
						value = music_pcm_hold;
					}
					z80_block(14);
					carry = 0u;
					if (value != 0u)
					{
						free_channel(channel);
						return carry;
					}
					z80_block(10);
					return stop_pcm_channel(channel);
				}
				z80_block(27);
				fm_write = 0x3800u;
				write_fm_channel_register(channel, &fm_write, &carry);
				z80_block(36);
				chip_channel = CHANNEL(channel)->chip_channel;
				zero = (uint8_t)(chip_channel - 6u) == 0u;
				if (chip_channel < 6u)
					return stop_channel(channel);
				z80_block(23);
				sound = CHANNEL(FM_EFFECT_6)->sound;
				if (zero == 0u)
				{
					z80_block(13);
					sound = CHANNEL(FM_EFFECT_7)->sound;
				}
				z80_block(14);
				carry = 0u;
				if (sound != 0u)
				{
					free_channel(channel);
					return carry;
				}
				z80_block(10);
				return stop_channel(channel);
			default:
				sound_invalid_jump(jump_target);
			}
			break;
		}
		z80_block(17);
		if (value == 0x70u)
		{
			z80_block(44);
			place = track;
			CHANNEL(channel)->attenuation = command & 0xFu;
			z80_block(6);
			place++;
			z80_block(48);
			CHANNEL(channel)->track = place;
			continue;
		}
		z80_block(25);
		t5 = command & 0xFu;
		place |= t5;
		if (t5 == 0u)
		{
			z80_block(30);
			if ((CHANNEL(channel)->flags & 1u) == 0u)
			{
				z80_block(17);
				fm_key_off(channel);
			}
			z80_block(48);
			AS8(CHANNEL(channel)->note_hook) = 0xF5u;
			AT8(CHANNEL(channel)->note_hook, 1) = 9u;
			return next_note_length(place, track, channel);
		}
		z80_block(17);
		carry = next_note_length(place, track, channel);
		play_note = true;
		break;
	}
	if (!play_note)
	{
		z80_block(61);
		saved_place = place;
		CHANNEL(channel)->ticks_left = 0u;
		length_codes = z80_read8(place) & ~0xFu;
		add_note_length_high(channel, length_codes);
		for (;;)
		{
			z80_block(30);
			place++;
			t6 = z80_read8(place) & ~0xFu;
			length_codes = t6;
			if (t6 == 0u)
				break;
			z80_block(17);
			add_note_length_high(channel, length_codes);
			z80_block(24);
			t7 = z80_read8(place) & 0xFu;
			length_code = t7;
			if (t7 == 0u)
				break;
			z80_block(17);
			add_note_length(channel, length_code);
			z80_block(10);
		}
		z80_block(82);
		place++;
		CHANNEL(channel)->track = place;
		carry = 0u;
		place = z80_read8(saved_place) & 0xFu;
	}
	z80_block(30);
	if (((CHANNEL(channel)->chip_channel >> 3) & 1u) != 0u)
	{
		z80_block(43);
		sound = CHANNEL(PCM_EFFECT_A)->sound;
		if (CHANNEL(channel)->chip_channel & 1u)
		{
			z80_block(13);
			sound = CHANNEL(PCM_EFFECT_B)->sound;
		}
		z80_block(9);
		carry = 0u;
		if (sound != 0u)
		{
			z80_cycles(6);
			return carry;
		}
		z80_block(59);
		note_index = (uint8_t)place;
		gated_length = 0u;
		gate = CHANNEL(channel)->gate;
		count = CHANNEL(channel)->ticks_left;
		for (;;)
		{
			z80_block(19);
			gated_length += gate;
			count--;
			if (count == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(11);
		place = note_index;
		key_off_ticks = 0xFFu;
		do
		{
			z80_block(43);
			key_off_ticks = (key_off_ticks + 1u) ^ (place >> 8);
			t8 = gated_length;
			gated_length = t8 - 0xFFu;
		} while (t8 >= 0xFFu);
		z80_block(75);
		CHANNEL(channel)->key_off_ticks = key_off_ticks;
		CHANNEL(channel)->ramp_wait = 3u;
		if ((CHANNEL(channel)->mode & 1u) != 0u)
		{
			z80_block(41);
			value = ((uint32_t)place << 1) | (place >> 7);
			place = (uint8_t)((((uint32_t)value << 1) | (value >> 7)) + place + place);
			pcm_sound = pcm_drums_ADDRESS - 0x6u;
		}
		else
		{
			z80_block(66);
			place = (uint8_t)(((uint32_t)CHANNEL(channel)->instrument << 1) << 1);
			pcm_sound = pcm_instruments_ADDRESS;
		}
		z80_block(67);
		pcm_sound += place;
		start = (uint8_t)PCM_DRUM(pcm_sound)->start;
		pcm_sound++;
		start |= (uint32_t)(uint8_t)(PCM_DRUM(pcm_sound - 0x1u)->start >> 8u) << 8;
		pcm_sound++;
		bank = PCM_DRUM(pcm_sound - 0x2u)->bank;
		pcm_sound++;
		bank_2 = bank;
		set_pcm_start(bank_2, start, channel);
		z80_block(122);
		total_attenuation = (uint8_t)(fade_attenuation + music_attenuation + CHANNEL(channel)->volume + CHANNEL(channel)->attenuation + CHANNEL(channel)->loop_volume_offset);
		level = PCM_DRUM(pcm_sound - 0x3u)->level;
		pcm_sound++;
		t9 = level;
		pcm_level = t9 - total_attenuation;
		if (t9 < total_attenuation)
		{
			z80_block(4);
			pcm_level = 0u;
		}
		z80_block(40);
		CHANNEL(channel)->pcm_level = pcm_level;
		volume = pcm_level;
		carry = set_pcm_volume(volume, channel);
		z80_block(30);
		if (CHANNEL(channel)->mode & 1u)
		{
			z80_block(37);
			pitch_2 = PCM_DRUM(pcm_sound - 0x4u)->pitch;
			set_pcm_pitch(pitch_2, channel);
			z80_block(10);
			start_pcm(channel);
			return carry;
		}
		z80_block(81);
		value = z80_read8((uint16_t)(CHANNEL(channel)->track - 1u)) & 0xFu;
		count = CHANNEL(channel)->octave + 1u;
		for (;;)
		{
			z80_block(15);
			value += 0xCu;
			count--;
			if (count == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(63);
		place = value;
		carry = (uint16_t)(pcm_note_pitches_ADDRESS - 0xCu + place) < pcm_note_pitches_ADDRESS - 0xCu;
		pitch_2 = TABLE8(pcm_note_pitches, (uint16_t)(place - 0xCu)) | 0xF00u;
		set_pcm_pitch(pitch_2, channel);
		z80_block(10);
		start_pcm(channel);
		return carry;
	}
	z80_block(25);
	if ((CHANNEL(channel)->flags & 1u) != 0u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(30);
	if (CHANNEL(channel)->flags >> 7)
	{
		z80_block(36);
		instrument = CHANNEL(channel)->instrument;
		carry = load_instrument(channel, instrument);
		z80_block(23);
		CHANNEL(channel)->flags &= 0x7Fu;
	}
	z80_block(30);
	if ((CHANNEL(channel)->mode & 1u) != 0u)
	{
		z80_block(10);
		clear_note_hook(channel);
		return carry;
	}
	z80_block(17);
	carry = fm_fast_release(channel);
	z80_block(243);
	note_index = music_transpose + ((((uint32_t)place << 1) | ((uint8_t)place >> 7)) + CHANNEL(channel)->octave + CHANNEL(channel)->transpose) + CHANNEL(channel)->loop_transpose;
	pitch = TABLE16(fm_note_pitches, (uint16_t)(note_index - 2u)) + CHANNEL(channel)->detune;
	CHANNEL(channel)->target_pitch = pitch;
	if (CHANNEL(channel)->portamento == 0u)
	{
		z80_block(55);
		CHANNEL(channel)->portamento_pitch = pitch;
		set_fm_pitch(pitch, channel, &ignored16, &carry);
	}
	z80_block(27);
	fm_write = 0x3800u;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(94);
	CHANNEL(channel)->ramp_wait = CHANNEL(channel)->lfo_ramp;
	CHANNEL(channel)->lfo_sensitivity = 0u;
	key_off_ticks = CHANNEL(channel)->gate;
	if (key_off_ticks != 0u)
	{
		z80_block(40);
		gated_length = 0u;
		place = CHANNEL(channel)->ticks_left;
		count = 8u;
		for (;;)
		{
			z80_block(18);
			carry = key_off_ticks & 1u;
			key_off_ticks = (key_off_ticks >> 1) | ((uint32_t)key_off_ticks << 7);
			if (carry)
			{
				z80_block(11);
				gated_length += place;
			}
			z80_block(24);
			carry = (uint8_t)place >> 7;
			place = IN_PAGE(place, (uint8_t)((uint32_t)place << 1));
			place = (place & 0xFFu) | ((uint32_t)(uint8_t)(((uint32_t)(place >> 8) << 1) | carry) << 8);
			count--;
			if (count == 0u)
				break;
			z80_cycles(5);
		}
		z80_block(8);
		key_off_ticks = (gated_length >> 8) + 1u;
	}
	z80_block(160);
	CHANNEL(channel)->key_off_ticks = key_off_ticks;
	t10 = music_volume + (fade_step + music_attenuation) + CHANNEL(channel)->volume_offset + CHANNEL(channel)->loop_volume_offset + CHANNEL(channel)->attenuation + CHANNEL(channel)->volume;
	attenuation = t10;
	if ((int8_t)t10 < 0)
	{
		z80_block(7);
		attenuation = 0x7Fu;
	}
	z80_block(17);
	carry = set_fm_volume(channel, attenuation);
	z80_block(17);
	fm_restore_release(channel);
	z80_block(10);
	return fm_key_on(channel);
}

/* Moves the track past the note and sets its length in ticks. */
uint32_t next_note_length(uint16_t note, uint16_t note_place, uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t length_code = 0;

	z80_block(148);
	note_place++;
	CHANNEL(channel)->track = note_place;
	length_code = ((((note >> 8) >> 1) >> 1) >> 1) >> 1;
	carry = (uint16_t)(note_lengths_ADDRESS - 0x1u + length_code) < note_lengths_ADDRESS - 0x1u;
	CHANNEL(channel)->ticks_left = note_lengths[(uint16_t)(length_code - 1u)];
	return carry;
}

/* Loads the FM instrument the track asks for. */
uint32_t load_instrument(uint16_t channel, uint8_t instrument)
{
	uint16_t offset = 0, voice = 0;

	z80_block(74);
	offset = (uint8_t)((uint32_t)instrument << 1);
	voice = fm_voice_pointers[offset / 2u];
	return load_fm_voice(voice, channel);
}

/* Adds the length its high nibble gives to the note's ticks. */
void add_note_length_high(uint16_t channel, uint8_t length_codes)
{
	z80_block(32);
	length_codes = (((length_codes >> 1) >> 1) >> 1) >> 1;
	add_note_length(channel, length_codes);
}

/* Adds a length to the note's ticks. */
void add_note_length(uint16_t channel, uint8_t length_code)
{
	z80_block(87);
	CHANNEL(channel)->ticks_left += note_lengths[(uint16_t)(length_code - 1u)];
}

/* Reads a transposition argument: semitones and octaves, down with its high bit. */
uint8_t read_transpose(uint16_t argument)
{
	uint8_t bits = 0, count = 0, transposition = 0;
	uint8_t t1 = 0;

	z80_block(60);
	bits = z80_read8(argument) & 0xFu;
	transposition = ((uint32_t)bits << 1) | (bits >> 7);
	bits = z80_read8(argument) & 0x70u;
	bits = (bits >> 1) | ((uint32_t)bits << 7);
	bits = (bits >> 1) | ((uint32_t)bits << 7);
	bits = (bits >> 1) | ((uint32_t)bits << 7);
	count = (bits >> 1) | ((uint32_t)bits << 7);
	bits = 0u;
	for (;;)
	{
		z80_block(15);
		bits += 0xCu;
		count--;
		if (count == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(36);
	t1 = z80_read8(argument);
	bits = (((uint32_t)bits << 1) | (bits >> 7)) + transposition;
	if (t1 >> 7 == 0u)
	{
		z80_cycles(6);
		return bits;
	}
	z80_block(18);
	bits = 0u - bits;
	return bits;
}

/* Reads an attenuation offset argument: negative with its high bit. */
uint8_t read_volume_offset(uint16_t argument)
{
	uint8_t offset = 0;

	z80_block(20);
	offset = z80_read8(argument);
	if (offset >> 7 == 0u)
	{
		z80_cycles(6);
		return offset;
	}
	z80_block(25);
	offset = 0u - (offset & 0x7Fu);
	return offset;
}

/* Reads the address the track goes on at, and moves past it. */
uint16_t read_track_address(uint16_t argument, uint16_t channel)
{
	z80_block(74);
	AS8(CHANNEL(channel)->track) = z80_read8(argument);
	argument++;
	AT8(CHANNEL(channel)->track, 1) = z80_read8(argument);
	argument++;
	return argument;
}

/* A note hook no track sets: it puts the one that does nothing back. */
void unused_note_hook(uint16_t channel)
{
	z80_block(10);
	clear_note_hook(channel);
}
