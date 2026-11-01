/**
 *    Name:        chips.c
 *    Description: the chips' registers: the YM2151's writes and the frame's timer, its voices, keys, pitches and volumes; the K007232's start, pitch, volume and loop
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

/* Writes a YM2151 register, a base plus an offset, with a value. */
void write_fm_register_de(uint8_t value, uint16_t fm_register)
{
	uint32_t carry = 0;
	uint16_t fm_write = 0;

	z80_block(22);
	fm_write = (value & 0xFFu) | ((uint32_t)(uint8_t)((fm_register >> 8) + fm_register) << 8);
	carry = write_fm_register(fm_write);
}

/* Writes the channel's own YM2151 register: the register given plus its chip channel. */
void write_fm_channel_register(uint16_t channel, uint16_t *p_fm_write, uint32_t *p_carry)
{
	uint16_t fm_write = (uint16_t)*p_fm_write;
	uint32_t carry = *p_carry;

	z80_block(27);
	fm_write = (fm_write & 0xFFu) | ((uint32_t)(uint8_t)(CHANNEL(channel)->chip_channel + (fm_write >> 8)) << 8);
	carry = write_fm_register(fm_write);
	*p_fm_write = (*p_fm_write & 0xFFu) | fm_write;
	*p_carry = carry;
}

/* Writes a YM2151 register, each write once the chip is not busy. */
uint32_t write_fm_register(uint16_t fm_write)
{
	uint32_t carry = 0;
	uint8_t t1 = 0, t2 = 0;

	for (;;)
	{
		z80_block(27);
		t1 = z80_read8(fm_data);
		if ((t1 & 0x80u) == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(17);
	z80_write8(fm_address, fm_write >> 8);
	for (;;)
	{
		z80_block(27);
		t2 = z80_read8(fm_data);
		carry = 0u;
		if ((t2 & 0x80u) == 0u)
			break;
		z80_cycles(5);
	}
	z80_block(27);
	z80_write8(fm_data, (uint8_t)fm_write);
	return carry;
}

/* Restarts the YM2151's timer B, the main loop's tick, and clears its flag. */
void restart_frame_timer(void)
{
	uint32_t carry = 0;
	uint16_t fm_write = 0;

	z80_block(27);
	fm_write = 0x1439u;
	carry = write_fm_register(fm_write);
	z80_block(27);
	fm_write = 0x12F2u;
	carry = write_fm_register(fm_write);
	z80_block(22);
	fm_write = 0x143Bu;
	carry = write_fm_register(fm_write);
}

/* Loads a voice into the channel (keyed off and released fast first), keeping its operators' total
   levels and releases in the record. */
uint32_t load_fm_voice(uint16_t voice, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t c1_release = 0, c2_release = 0, connection = 0, m1_release = 0, m2_release = 0;
	uint16_t fm_write = 0;

	z80_block(17);
	fm_key_off(channel);
	z80_block(17);
	carry = fm_fast_release(channel);
	z80_block(50);
	connection = FM_VOICE(voice)->connection;
	CHANNEL(channel)->connection = connection;
	fm_write = 0x2000u | connection;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0x4000u | FM_VOICE(voice - 0x1u)->m1_detune_multiple;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(69);
	voice++;
	CHANNEL(channel)->m1_total_level = FM_VOICE(voice - 0x2u)->m1_total_level;
	voice++;
	fm_write = 0x8000u | FM_VOICE(voice - 0x3u)->m1_attack;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xA000u | FM_VOICE(voice - 0x4u)->m1_decay_1;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xC000u | FM_VOICE(voice - 0x5u)->m1_decay_2;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(56);
	voice++;
	m1_release = FM_VOICE(voice - 0x6u)->m1_release;
	CHANNEL(channel)->m1_release = m1_release;
	fm_write = 0xE000u | m1_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0x5000u | FM_VOICE(voice - 0x7u)->c1_detune_multiple;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(69);
	voice++;
	CHANNEL(channel)->c1_total_level = FM_VOICE(voice - 0x8u)->c1_total_level;
	voice++;
	fm_write = 0x9000u | FM_VOICE(voice - 0x9u)->c1_attack;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xB000u | FM_VOICE(voice - 0xAu)->c1_decay_1;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xD000u | FM_VOICE(voice - 0xBu)->c1_decay_2;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(56);
	voice++;
	c1_release = FM_VOICE(voice - 0xCu)->c1_release;
	CHANNEL(channel)->c1_release = c1_release;
	fm_write = 0xF000u | c1_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0x4800u | FM_VOICE(voice - 0xDu)->m2_detune_multiple;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(69);
	voice++;
	CHANNEL(channel)->m2_total_level = FM_VOICE(voice - 0xEu)->m2_total_level;
	voice++;
	fm_write = 0x8800u | FM_VOICE(voice - 0xFu)->m2_attack;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xA800u | FM_VOICE(voice - 0x10u)->m2_decay_1;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xC800u | FM_VOICE(voice - 0x11u)->m2_decay_2;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(56);
	voice++;
	m2_release = FM_VOICE(voice - 0x12u)->m2_release;
	CHANNEL(channel)->m2_release = m2_release;
	fm_write = 0xE800u | m2_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0x5800u | FM_VOICE(voice - 0x13u)->c2_detune_multiple;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(69);
	voice++;
	CHANNEL(channel)->c2_total_level = FM_VOICE(voice - 0x14u)->c2_total_level;
	voice++;
	fm_write = 0x9800u | FM_VOICE(voice - 0x15u)->c2_attack;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xB800u | FM_VOICE(voice - 0x16u)->c2_decay_1;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(37);
	voice++;
	fm_write = 0xD800u | FM_VOICE(voice - 0x17u)->c2_decay_2;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(49);
	c2_release = FM_VOICE(voice - 0x17u)->c2_release;
	CHANNEL(channel)->c2_release = c2_release;
	fm_write = 0xF800u | c2_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	return carry;
}

/* Keys the channel's four operators on (an FM channel only). */
uint32_t fm_key_on(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t chip_channel = 0;
	uint16_t fm_write = 0;

	z80_block(31);
	chip_channel = CHANNEL(channel)->chip_channel;
	carry = chip_channel < 8u;
	if (chip_channel >= 8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(28);
	fm_write = 0x800u | chip_channel | 0x78u;
	return write_fm_register(fm_write);
}

/* Keys the channel's operators off (an FM channel only). */
void fm_key_off(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t chip_channel = 0;
	uint16_t fm_write = 0;

	z80_block(31);
	chip_channel = CHANNEL(channel)->chip_channel;
	if (chip_channel >= 8u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(21);
	fm_write = 0x800u | chip_channel;
	carry = write_fm_register(fm_write);
}

/* Sets the four operators' release to its fastest, so that a note keyed off stops at once. */
uint32_t fm_fast_release(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t c1_release = 0, c2_release = 0, chip_channel = 0, m1_release = 0, m2_release = 0;
	uint16_t fm_register = 0;

	z80_block(31);
	chip_channel = CHANNEL(channel)->chip_channel;
	carry = chip_channel < 8u;
	if (chip_channel >= 8u)
	{
		z80_cycles(6);
		return carry;
	}
	z80_block(43);
	m1_release = CHANNEL(channel)->m1_release;
	fm_register = speech_start;
	carry = fm_fast_release_operator(fm_register, channel, m1_release);
	z80_block(43);
	c1_release = CHANNEL(channel)->c1_release;
	fm_register = speech_busy;
	carry = fm_fast_release_operator(fm_register, channel, c1_release);
	z80_block(43);
	m2_release = CHANNEL(channel)->m2_release;
	fm_register = 0xE800u;
	carry = fm_fast_release_operator(fm_register, channel, m2_release);
	z80_block(26);
	c2_release = CHANNEL(channel)->c2_release;
	fm_register = 0xF800u;
	return fm_fast_release_operator(fm_register, channel, c2_release);
}

/* Writes an operator's release register with the release at its fastest. */
uint32_t fm_fast_release_operator(uint16_t fm_register, uint16_t channel, uint8_t release)
{
	uint32_t carry = 0;
	uint16_t fm_write = 0;

	z80_block(21);
	fm_write = (fm_register & ~0xFFu) | release | 0xFu;
	write_fm_channel_register(channel, &fm_write, &carry);
	return carry;
}

/* Puts the four operators' release back as the voice has it. */
void fm_restore_release(uint16_t channel)
{
	uint32_t carry = 0;
	uint16_t fm_write = 0;

	z80_block(31);
	if (CHANNEL(channel)->chip_channel >= 8u)
	{
		z80_cycles(6);
		return;
	}
	z80_block(43);
	fm_write = 0xE000u | CHANNEL(channel)->m1_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(43);
	fm_write = 0xF000u | CHANNEL(channel)->c1_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(43);
	fm_write = 0xE800u | CHANNEL(channel)->m2_release;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(36);
	fm_write = 0xF800u | CHANNEL(channel)->c2_release;
	write_fm_channel_register(channel, &fm_write, &carry);
}

/* Sets the channel's pitch: the key code from the semitone, the key fraction from its fraction. */
void set_fm_pitch(uint16_t pitch, uint16_t channel, uint16_t *p_fm_write, uint32_t *p_carry)
{
	uint32_t carry = *p_carry;
	uint8_t semitone = 0;
	uint16_t fm_write = 0;
	uint8_t t1 = 0;

	z80_block(11);
	semitone = (pitch >> 8) & 0x1Fu;
	do
	{
		z80_block(17);
		t1 = semitone;
		semitone = t1 - 0x18u;
	} while (t1 >= 0x18u);
	z80_block(61);
	pitch = ((pitch & 0xFFu) | ((uint32_t)(uint8_t)(semitone + 0x18u) << 8)) * 2u * 2u;
	fm_write = 0x3000u | (uint8_t)pitch;
	write_fm_channel_register(channel, &fm_write, &carry);
	z80_block(56);
	fm_write = 0x2800u | fm_key_codes[pitch >> 8];
	*p_fm_write = fm_write;
	write_fm_channel_register(channel, p_fm_write, &carry);
	*p_carry = carry;
}

/* Sets the FM effect's attenuation it keeps in its record. */
uint32_t set_fm_volume_from_record(uint16_t channel)
{
	uint8_t attenuation = 0;

	z80_block(19);
	attenuation = CHANNEL(channel)->fm_effect.attenuation;
	return set_fm_volume(channel, attenuation);
}

/* Sets the channel's attenuation: each carrier's total level is the voice's plus it (at most silence);
   the modulators keep the voice's. */
uint32_t set_fm_volume(uint16_t channel, uint8_t attenuation)
{
	uint32_t carry = 0;
	uint8_t c1_total_level = 0, c2_total_level = 0, m1_total_level = 0, m2_total_level = 0;
	uint16_t carriers = 0, fm_register = 0;

	z80_block(127);
	CHANNEL(channel)->fm_attenuation = attenuation;
	carriers = fm_carrier_masks[CHANNEL(channel)->connection & 7u];
	m1_total_level = CHANNEL(channel)->m1_total_level;
	fm_register = 0x6000u;
	set_fm_operator_level(fm_register, channel, m1_total_level, &carriers, &carry);
	z80_block(43);
	m2_total_level = CHANNEL(channel)->m2_total_level;
	fm_register = 0x6800u;
	set_fm_operator_level(fm_register, channel, m2_total_level, &carriers, &carry);
	z80_block(43);
	c1_total_level = CHANNEL(channel)->c1_total_level;
	fm_register = 0x7000u;
	set_fm_operator_level(fm_register, channel, c1_total_level, &carriers, &carry);
	z80_block(26);
	c2_total_level = CHANNEL(channel)->c2_total_level;
	fm_register = 0x7800u;
	set_fm_operator_level(fm_register, channel, c2_total_level, &carriers, &carry);
	return carry;
}

/* Writes an operator's total level: the voice's, plus the channel's attenuation when the operator is a
   carrier. */
void set_fm_operator_level(uint16_t fm_register, uint16_t channel, uint8_t level, uint16_t *p_carriers, uint32_t *p_carry)
{
	uint8_t carriers = (uint8_t)*p_carriers;
	uint32_t carry = *p_carry;
	uint8_t carrier_mask = 0;
	uint16_t carrier_mask_left = 0, fm_write = 0;
	uint8_t t1 = 0;

	carrier_mask = (uint8_t)carriers;
	z80_block(18);
	carry = carrier_mask >> 7;
	carrier_mask = (uint32_t)carrier_mask << 1;
	if (carry)
	{
		z80_block(29);
		t1 = level + CHANNEL(channel)->fm_attenuation;
		level = t1;
		if ((int8_t)t1 < 0)
		{
			z80_block(7);
			level = 0x7Fu;
		}
	}
	z80_block(14);
	fm_write = (fm_register & ~0xFFu) | level;
	carrier_mask_left = carrier_mask;
	write_fm_channel_register(channel, &fm_write, &carry);
	*p_carriers = (*p_carriers & 0xFF00u) | carrier_mask_left;
	*p_carry = carry;
}

/* Sets where the sample of the channel's K007232 channel starts. */
void set_pcm_start(uint8_t bank, uint16_t start, uint16_t channel)
{
	uint8_t bank_bit = 0;

	z80_block(34);
	bank_bit = (uint8_t)bank;
	if ((CHANNEL(channel)->chip_channel & 1u) != 0u)
	{
		z80_block(43);
		z80_write16(pcm_b_start, start);
		z80_write8(pcm_b_bank, bank_bit);
		return;
	}
	z80_block(43);
	z80_write16(pcm_a_start, start);
	z80_write8(pcm_a_bank, bank_bit);
}

/* Sets the pitch of the channel's K007232 channel. */
void set_pcm_pitch(uint16_t pitch, uint16_t channel)
{
	z80_block(30);
	if ((CHANNEL(channel)->chip_channel & 1u) != 0u)
	{
		z80_block(30);
		z80_write16(pcm_b_pitch, pitch);
		return;
	}
	z80_block(30);
	z80_write16(pcm_a_pitch, pitch);
}

/* Sets the volume of the channel's K007232 channel, keeping the other's. */
uint32_t set_pcm_volume(uint8_t volume, uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t port_value = 0, volume_bits = 0;

	volume_bits = (uint8_t)volume;
	z80_block(43);
	port_value = pcm_volumes;
	if ((CHANNEL(channel)->chip_channel & 1u) != 0u)
	{
		z80_block(47);
		carry = 0u;
		port_value = (port_value & ~0xFu) | volume_bits;
		pcm_volumes = port_value;
		z80_write8(pcm_volume_port, port_value);
		return carry;
	}
	z80_block(79);
	carry = 0u;
	port_value = (port_value & 0xFu) | (((((uint32_t)volume_bits << 1) << 1) << 1) << 1);
	pcm_volumes = port_value;
	z80_write8(pcm_volume_port, port_value);
	return carry;
}

/* Starts the channel's K007232 channel. */
void start_pcm(uint16_t channel)
{
	uint8_t t1 = 0, t2 = 0;

	z80_block(30);
	if ((CHANNEL(channel)->chip_channel & 1u) != 0u)
	{
		z80_block(23);
		t1 = z80_read8(pcm_b_key);
		return;
	}
	z80_block(23);
	t2 = z80_read8(pcm_a_key);
}

/* Makes the channel's K007232 channel loop its sample (nothing calls it). */
void set_pcm_loop(uint16_t channel)
{
	uint8_t loops = 0;

	z80_block(43);
	loops = pcm_loops;
	if ((CHANNEL(channel)->chip_channel & 1u) != 0u)
	{
		z80_block(43);
		loops |= 2u;
		pcm_loops = loops;
		z80_write8(pcm_loop_flags, loops);
		return;
	}
	z80_block(43);
	loops |= 1u;
	pcm_loops = loops;
	z80_write8(pcm_loop_flags, loops);
}

/* Makes the channel's K007232 channel play its sample once. */
uint32_t clear_pcm_loop(uint16_t channel)
{
	uint32_t carry = 0;
	uint8_t loops = 0;

	z80_block(43);
	loops = pcm_loops;
	if ((CHANNEL(channel)->chip_channel & 1u) != 0u)
	{
		z80_block(43);
		carry = 0u;
		loops &= 1u;
		pcm_loops = loops;
		z80_write8(pcm_loop_flags, loops);
		return carry;
	}
	z80_block(43);
	carry = 0u;
	loops &= 2u;
	pcm_loops = loops;
	z80_write8(pcm_loop_flags, loops);
	return carry;
}
