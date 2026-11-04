/**
 *    Name:        system/sound_commands.c
 *    Description: the commands to the sound board, queued and sent one a frame
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* queues a sound or a voice for the sound board, when the game lets sounds play; never a music */
void queue_sound(uint8_t sound)
{
	if ((dip_switches_3 & 4u) != 0u || game_on != 0u || ((sound < 0x60u || sound > 0x6Bu) && demo_sounds != 0u))
	{
		queue_sound_command(sound);
		return;
	}
}

/* puts a command into the sound board's queue */
void queue_sound_command(uint8_t command)
{
	uint32_t queue_place = 0;

	queue_place = sound_queue_write;
	QUEUED_SOUND(queue_place)->command = command;
	queue_place++;
	if (queue_place > sound_queue_ADDRESS + 0x3Fu)
		queue_place = sound_queue_ADDRESS;
	sound_queue_write = queue_place;
}

/* queue_sound with the sound in another register */
void queue_sound_d0(uint8_t sound)
{
	uint8_t sound_to_queue = 0;

	sound_to_queue = (uint8_t)sound;
	queue_sound(sound_to_queue);
}

/* the next sound command to the sound board */
void send_sound_command(void)
{
	uint8_t command = 0, control = 0;
	uint32_t queue_place = 0;

	queue_place = sound_queue_read;
	if (queue_place == sound_queue_write)
		return;
	command = QUEUED_SOUND(queue_place)->command;
	queue_place++;
	if (queue_place > sound_queue_ADDRESS + 0x3Fu)
		queue_place = sound_queue_ADDRESS;
	sound_queue_read = queue_place;
	io_write8(IO_0A0009, command);
	control = board_control | 8u;
	fade_banks = control;
	io_write8(IO_0A0001, control);
}
