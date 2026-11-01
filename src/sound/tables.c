/**
 *    Name:        tables.c
 *    Description: the tables the sound program reads by name, which the board sets
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#include "sound.h"

const void *speech_priorities_data;
const void *speech_music_attenuation_data;
const void *pcm_effect_priorities_data;
const void *fm_effect_priorities_data;
const void *fm_key_codes_data;
const void *fm_carrier_masks_data;
const void *fm_note_pitches_data;
const void *pcm_note_pitches_data;
const void *note_lengths_data;
const void *track_commands_e0_data;
const void *track_commands_f0_data;
const void *pcm_effect_handlers_data;
const void *fm_effect_handlers_data;
const void *fm_effect_26_pitches_data;
const void *fm_voice_pointers_data;

/* the tables the game loads from its assets: their names, addresses and pointers */
const sound_table sound_tables[] =
{
	{ "speech_priorities", 0x0130u, &speech_priorities_data },
	{ "speech_music_attenuation", 0x0154u, &speech_music_attenuation_data },
	{ "pcm_effect_priorities", 0x025Au, &pcm_effect_priorities_data },
	{ "fm_effect_priorities", 0x02FBu, &fm_effect_priorities_data },
	{ "fm_key_codes", 0x073Du, &fm_key_codes_data },
	{ "fm_carrier_masks", 0x07DEu, &fm_carrier_masks_data },
	{ "fm_note_pitches", 0x0B1Cu, &fm_note_pitches_data },
	{ "pcm_note_pitches", 0x0C96u, &pcm_note_pitches_data },
	{ "note_lengths", 0x0CF9u, &note_lengths_data },
	{ "track_commands_e0", 0x0D4Du, &track_commands_e0_data },
	{ "track_commands_f0", 0x0D6Du, &track_commands_f0_data },
	{ "pcm_effect_handlers", 0x106Cu, &pcm_effect_handlers_data },
	{ "fm_effect_handlers", 0x13DBu, &fm_effect_handlers_data },
	{ "fm_effect_26_pitches", 0x2B04u, &fm_effect_26_pitches_data },
	{ "fm_voice_pointers", 0x37B5u, &fm_voice_pointers_data },
};

const size_t sound_table_count = 15;
