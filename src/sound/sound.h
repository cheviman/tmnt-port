/**
 *    Name:        sound.h
 *    Description: the sound program's ports, variables, tables, structures and functions
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the Z80 sound program decompiled into C by
 *    tools/decompile_sound.py, with the names of sound_symbols.txt.
 */
#ifndef TMNT_SOUND_H
#define TMNT_SOUND_H

#include "sound/support.h"

/* the sound program's variables and channel records, in the Z80's RAM (8000-87FF) */
#define FM_EFFECT_0 0x8000u
#define MUSIC_FM_1 0x8040u
#define MUSIC_FM_2 0x8080u
#define MUSIC_FM_3 0x80C0u
#define MUSIC_FM_4 0x8100u
#define MUSIC_FM_5 0x8140u
#define MUSIC_FM_6 0x8180u
#define MUSIC_FM_7 0x81C0u
#define MUSIC_PCM_A 0x8200u
#define MUSIC_PCM_B 0x8240u
#define PCM_EFFECT_A 0x8280u
#define PCM_EFFECT_B 0x82C0u
#define SPEECH 0x8300u
#define FM_EFFECT_6 0x8340u
#define FM_EFFECT_7 0x8380u
#define unk_8000_ADDRESS 0x8000u
#define unk_8000 SOUND_RAM8(unk_8000_ADDRESS)
#define unk_8001_ADDRESS 0x8001u
#define unk_8001 SOUND_RAM8(unk_8001_ADDRESS)
#define unk_8002_ADDRESS 0x8002u
#define unk_8002 SOUND_RAM16(unk_8002_ADDRESS)
#define unk_800F_ADDRESS 0x800Fu
#define unk_800F SOUND_RAM8(unk_800F_ADDRESS)
#define unk_8040_ADDRESS 0x8040u
#define unk_8040 SOUND_RAM8(unk_8040_ADDRESS)
#define unk_8042_ADDRESS 0x8042u
#define unk_8042 SOUND_RAM16(unk_8042_ADDRESS)
#define unk_8080_ADDRESS 0x8080u
#define unk_8080 SOUND_RAM8(unk_8080_ADDRESS)
#define unk_8082_ADDRESS 0x8082u
#define unk_8082 SOUND_RAM16(unk_8082_ADDRESS)
#define unk_80C0_ADDRESS 0x80C0u
#define unk_80C0 SOUND_RAM8(unk_80C0_ADDRESS)
#define unk_80C2_ADDRESS 0x80C2u
#define unk_80C2 SOUND_RAM16(unk_80C2_ADDRESS)
#define unk_8100_ADDRESS 0x8100u
#define unk_8100 SOUND_RAM8(unk_8100_ADDRESS)
#define unk_8102_ADDRESS 0x8102u
#define unk_8102 SOUND_RAM16(unk_8102_ADDRESS)
#define unk_8140_ADDRESS 0x8140u
#define unk_8140 SOUND_RAM8(unk_8140_ADDRESS)
#define unk_8142_ADDRESS 0x8142u
#define unk_8142 SOUND_RAM16(unk_8142_ADDRESS)
#define unk_8180_ADDRESS 0x8180u
#define unk_8180 SOUND_RAM8(unk_8180_ADDRESS)
#define unk_8182_ADDRESS 0x8182u
#define unk_8182 SOUND_RAM16(unk_8182_ADDRESS)
#define unk_81C0_ADDRESS 0x81C0u
#define unk_81C0 SOUND_RAM8(unk_81C0_ADDRESS)
#define unk_81C2_ADDRESS 0x81C2u
#define unk_81C2 SOUND_RAM16(unk_81C2_ADDRESS)
#define unk_8200_ADDRESS 0x8200u
#define unk_8200 SOUND_RAM8(unk_8200_ADDRESS)
#define unk_8201_ADDRESS 0x8201u
#define unk_8201 SOUND_RAM8(unk_8201_ADDRESS)
#define unk_8202_ADDRESS 0x8202u
#define unk_8202 SOUND_RAM16(unk_8202_ADDRESS)
#define unk_8240_ADDRESS 0x8240u
#define unk_8240 SOUND_RAM8(unk_8240_ADDRESS)
#define unk_8242_ADDRESS 0x8242u
#define unk_8242 SOUND_RAM16(unk_8242_ADDRESS)
#define unk_8280_ADDRESS 0x8280u
#define unk_8280 SOUND_RAM8(unk_8280_ADDRESS)
#define unk_8281_ADDRESS 0x8281u
#define unk_8281 SOUND_RAM8(unk_8281_ADDRESS)
#define unk_8282_ADDRESS 0x8282u
#define unk_8282 SOUND_RAM16(unk_8282_ADDRESS)
#define unk_82C0_ADDRESS 0x82C0u
#define unk_82C0 SOUND_RAM8(unk_82C0_ADDRESS)
#define unk_82C1_ADDRESS 0x82C1u
#define unk_82C1 SOUND_RAM8(unk_82C1_ADDRESS)
#define unk_82C2_ADDRESS 0x82C2u
#define unk_82C2 SOUND_RAM16(unk_82C2_ADDRESS)
#define unk_82CF_ADDRESS 0x82CFu
#define unk_82CF SOUND_RAM8(unk_82CF_ADDRESS)
#define unk_8300_ADDRESS 0x8300u
#define unk_8300 SOUND_RAM8(unk_8300_ADDRESS)
#define unk_8301_ADDRESS 0x8301u
#define unk_8301 SOUND_RAM8(unk_8301_ADDRESS)
#define unk_8302_ADDRESS 0x8302u
#define unk_8302 SOUND_RAM8(unk_8302_ADDRESS)
#define unk_8303_ADDRESS 0x8303u
#define unk_8303 SOUND_RAM8(unk_8303_ADDRESS)
#define unk_830F_ADDRESS 0x830Fu
#define unk_830F SOUND_RAM8(unk_830F_ADDRESS)
#define unk_8340_ADDRESS 0x8340u
#define unk_8340 SOUND_RAM8(unk_8340_ADDRESS)
#define unk_8341_ADDRESS 0x8341u
#define unk_8341 SOUND_RAM8(unk_8341_ADDRESS)
#define unk_8342_ADDRESS 0x8342u
#define unk_8342 SOUND_RAM16(unk_8342_ADDRESS)
#define unk_834F_ADDRESS 0x834Fu
#define unk_834F SOUND_RAM8(unk_834F_ADDRESS)
#define unk_8380_ADDRESS 0x8380u
#define unk_8380 SOUND_RAM8(unk_8380_ADDRESS)
#define unk_8381_ADDRESS 0x8381u
#define unk_8381 SOUND_RAM8(unk_8381_ADDRESS)
#define unk_8382_ADDRESS 0x8382u
#define unk_8382 SOUND_RAM16(unk_8382_ADDRESS)
#define unk_838F_ADDRESS 0x838Fu
#define unk_838F SOUND_RAM8(unk_838F_ADDRESS)
#define song_header_ADDRESS 0x83C0u
#define song_header SOUND_RAM16(song_header_ADDRESS)
#define pcm_volumes_ADDRESS 0x83C2u
#define pcm_volumes SOUND_RAM8(pcm_volumes_ADDRESS)
#define pcm_loops_ADDRESS 0x83C3u
#define pcm_loops SOUND_RAM8(pcm_loops_ADDRESS)
#define title_music_on_ADDRESS 0x83C4u
#define title_music_on SOUND_RAM8(title_music_on_ADDRESS)
#define title_music_passes_ADDRESS 0x83C5u
#define title_music_passes SOUND_RAM8(title_music_passes_ADDRESS)
#define title_music_rounds_ADDRESS 0x83C6u
#define title_music_rounds SOUND_RAM8(title_music_rounds_ADDRESS)
#define new_effect_priority_ADDRESS 0x83C7u
#define new_effect_priority SOUND_RAM8(new_effect_priority_ADDRESS)
#define speech_priority_ADDRESS 0x83C8u
#define speech_priority SOUND_RAM8(speech_priority_ADDRESS)
#define fade_timer_ADDRESS 0x83C9u
#define fade_timer SOUND_RAM8(fade_timer_ADDRESS)
#define fade_step_ADDRESS 0x83CAu
#define fade_step SOUND_RAM8(fade_step_ADDRESS)
#define fade_attenuation_ADDRESS 0x83CBu
#define fade_attenuation SOUND_RAM8(fade_attenuation_ADDRESS)
#define music_attenuation_changed_ADDRESS 0x83CCu
#define music_attenuation_changed SOUND_RAM8(music_attenuation_changed_ADDRESS)
#define random_seed_ADDRESS 0x83CDu
#define random_seed SOUND_RAM16(random_seed_ADDRESS)
#define random_source_ADDRESS 0x83CFu
#define random_source SOUND_RAM16(random_source_ADDRESS)
#define random_write_ADDRESS 0x83D1u
#define random_write SOUND_RAM16(random_write_ADDRESS)
#define random_read_ADDRESS 0x83D3u
#define random_read SOUND_RAM16(random_read_ADDRESS)
#define music_volume_ADDRESS 0x83D5u
#define music_volume SOUND_RAM8(music_volume_ADDRESS)
#define music_transpose_ADDRESS 0x83D6u
#define music_transpose SOUND_RAM8(music_transpose_ADDRESS)
#define accelerando_ADDRESS 0x83D7u
#define accelerando SOUND_RAM16(accelerando_ADDRESS)
#define tempo_ADDRESS 0x83D9u
#define tempo SOUND_RAM8(tempo_ADDRESS)
#define tempo_increase_ADDRESS 0x83DAu
#define tempo_increase SOUND_RAM8(tempo_increase_ADDRESS)
#define music_attenuation_ADDRESS 0x83DBu
#define music_attenuation SOUND_RAM8(music_attenuation_ADDRESS)
#define music_pcm_hold_ADDRESS 0x83DEu
#define music_pcm_hold SOUND_RAM8(music_pcm_hold_ADDRESS)
#define fm_effect_01_timer_ADDRESS 0x83DFu
#define fm_effect_01_timer SOUND_RAM8(fm_effect_01_timer_ADDRESS)
#define fm_effect_01_notes_ADDRESS 0x83E2u
#define fm_effect_01_notes SOUND_RAM8(fm_effect_01_notes_ADDRESS)
#define fm_effect_01_attenuation_ADDRESS 0x83E3u
#define fm_effect_01_attenuation SOUND_RAM8(fm_effect_01_attenuation_ADDRESS)
#define fm_effect_02_pitch_ADDRESS 0x83E4u
#define fm_effect_02_pitch SOUND_RAM16(fm_effect_02_pitch_ADDRESS)
#define fm_effect_02_timer_ADDRESS 0x83E6u
#define fm_effect_02_timer SOUND_RAM8(fm_effect_02_timer_ADDRESS)
#define fm_effect_02_steps_ADDRESS 0x83E7u
#define fm_effect_02_steps SOUND_RAM8(fm_effect_02_steps_ADDRESS)
#define fm_effect_02_period_ADDRESS 0x83E8u
#define fm_effect_02_period SOUND_RAM8(fm_effect_02_period_ADDRESS)
#define fm_effect_02_attenuation_ADDRESS 0x83E9u
#define fm_effect_02_attenuation SOUND_RAM8(fm_effect_02_attenuation_ADDRESS)
#define fm_effect_02_rounds_ADDRESS 0x83EAu
#define fm_effect_02_rounds SOUND_RAM8(fm_effect_02_rounds_ADDRESS)
#define fm_effect_07_timer_ADDRESS 0x83EFu
#define fm_effect_07_timer SOUND_RAM8(fm_effect_07_timer_ADDRESS)
#define fm_effect_07_notes_ADDRESS 0x83F0u
#define fm_effect_07_notes SOUND_RAM8(fm_effect_07_notes_ADDRESS)
#define fm_effect_07_rounds_ADDRESS 0x83F1u
#define fm_effect_07_rounds SOUND_RAM8(fm_effect_07_rounds_ADDRESS)
#define fm_effect_07_attenuation_ADDRESS 0x83F2u
#define fm_effect_07_attenuation SOUND_RAM8(fm_effect_07_attenuation_ADDRESS)
#define fm_effect_0a_on_ADDRESS 0x83F3u
#define fm_effect_0a_on SOUND_RAM8(fm_effect_0a_on_ADDRESS)
#define fm_effect_0a_timer_ADDRESS 0x83F4u
#define fm_effect_0a_timer SOUND_RAM8(fm_effect_0a_timer_ADDRESS)
#define fm_effect_0a_notes_ADDRESS 0x83F5u
#define fm_effect_0a_notes SOUND_RAM8(fm_effect_0a_notes_ADDRESS)
#define fm_effect_0a_attenuation_ADDRESS 0x83F6u
#define fm_effect_0a_attenuation SOUND_RAM8(fm_effect_0a_attenuation_ADDRESS)
#define fm_effect_0a_round_attenuation_ADDRESS 0x83F7u
#define fm_effect_0a_round_attenuation SOUND_RAM8(fm_effect_0a_round_attenuation_ADDRESS)
#define fm_effect_0a_pitch_ADDRESS 0x83F8u
#define fm_effect_0a_pitch SOUND_RAM16(fm_effect_0a_pitch_ADDRESS)
#define fm_effect_0a_pair_notes_ADDRESS 0x83FAu
#define fm_effect_0a_pair_notes SOUND_RAM8(fm_effect_0a_pair_notes_ADDRESS)
#define fm_effect_0a_rounds_ADDRESS 0x83FBu
#define fm_effect_0a_rounds SOUND_RAM8(fm_effect_0a_rounds_ADDRESS)
#define fm_effect_33_timer_ADDRESS 0x83FCu
#define fm_effect_33_timer SOUND_RAM8(fm_effect_33_timer_ADDRESS)
#define fm_effect_33_repeats_ADDRESS 0x83FDu
#define fm_effect_33_repeats SOUND_RAM8(fm_effect_33_repeats_ADDRESS)
#define fm_effect_33_attenuation_ADDRESS 0x83FEu
#define fm_effect_33_attenuation SOUND_RAM8(fm_effect_33_attenuation_ADDRESS)
#define fm_effect_0b_slide_timer_ADDRESS 0x83FFu
#define fm_effect_0b_slide_timer SOUND_RAM8(fm_effect_0b_slide_timer_ADDRESS)
#define fm_effect_0b_attenuation_ADDRESS 0x8400u
#define fm_effect_0b_attenuation SOUND_RAM8(fm_effect_0b_attenuation_ADDRESS)
#define fm_effect_0b_pitch_ADDRESS 0x8401u
#define fm_effect_0b_pitch SOUND_RAM16(fm_effect_0b_pitch_ADDRESS)
#define fm_effect_0b_slides_ADDRESS 0x8403u
#define fm_effect_0b_slides SOUND_RAM8(fm_effect_0b_slides_ADDRESS)
#define fm_effect_0b_timer_ADDRESS 0x8404u
#define fm_effect_0b_timer SOUND_RAM8(fm_effect_0b_timer_ADDRESS)
#define fm_effect_0e_timer_ADDRESS 0x8405u
#define fm_effect_0e_timer SOUND_RAM8(fm_effect_0e_timer_ADDRESS)
#define fm_effect_0e_attenuation_ADDRESS 0x8406u
#define fm_effect_0e_attenuation SOUND_RAM8(fm_effect_0e_attenuation_ADDRESS)
#define fm_effect_0e_pitch_ADDRESS 0x8407u
#define fm_effect_0e_pitch SOUND_RAM16(fm_effect_0e_pitch_ADDRESS)
#define fm_effect_0e_repeats_ADDRESS 0x8409u
#define fm_effect_0e_repeats SOUND_RAM8(fm_effect_0e_repeats_ADDRESS)
#define fm_effect_14_pitch_ADDRESS 0x840Au
#define fm_effect_14_pitch SOUND_RAM16(fm_effect_14_pitch_ADDRESS)
#define fm_effect_14_timer_ADDRESS 0x840Cu
#define fm_effect_14_timer SOUND_RAM8(fm_effect_14_timer_ADDRESS)
#define fm_effect_14_steps_ADDRESS 0x840Du
#define fm_effect_14_steps SOUND_RAM8(fm_effect_14_steps_ADDRESS)
#define fm_effect_14_rounds_ADDRESS 0x840Eu
#define fm_effect_14_rounds SOUND_RAM8(fm_effect_14_rounds_ADDRESS)
#define fm_effect_14_attenuation_ADDRESS 0x840Fu
#define fm_effect_14_attenuation SOUND_RAM8(fm_effect_14_attenuation_ADDRESS)
#define fm_effect_17_timer_ADDRESS 0x8410u
#define fm_effect_17_timer SOUND_RAM8(fm_effect_17_timer_ADDRESS)
#define fm_effect_17_attenuation_ADDRESS 0x8411u
#define fm_effect_17_attenuation SOUND_RAM8(fm_effect_17_attenuation_ADDRESS)
#define fm_effect_17_pitch_ADDRESS 0x8412u
#define fm_effect_17_pitch SOUND_RAM16(fm_effect_17_pitch_ADDRESS)
#define fm_effect_17_base_pitch_ADDRESS 0x8414u
#define fm_effect_17_base_pitch SOUND_RAM16(fm_effect_17_base_pitch_ADDRESS)
#define fm_effect_17_steps_ADDRESS 0x8416u
#define fm_effect_17_steps SOUND_RAM8(fm_effect_17_steps_ADDRESS)
#define fm_effect_1b_timer_ADDRESS 0x8417u
#define fm_effect_1b_timer SOUND_RAM8(fm_effect_1b_timer_ADDRESS)
#define fm_effect_1b_pitch_ADDRESS 0x8418u
#define fm_effect_1b_pitch SOUND_RAM16(fm_effect_1b_pitch_ADDRESS)
#define fm_effect_1f_timer_ADDRESS 0x841Au
#define fm_effect_1f_timer SOUND_RAM8(fm_effect_1f_timer_ADDRESS)
#define fm_effect_1f_attenuation_ADDRESS 0x841Bu
#define fm_effect_1f_attenuation SOUND_RAM8(fm_effect_1f_attenuation_ADDRESS)
#define fm_effect_1f_pitch_ADDRESS 0x841Cu
#define fm_effect_1f_pitch SOUND_RAM16(fm_effect_1f_pitch_ADDRESS)
#define fm_effect_1f_repeats_ADDRESS 0x841Eu
#define fm_effect_1f_repeats SOUND_RAM8(fm_effect_1f_repeats_ADDRESS)
#define fm_effect_21_on_ADDRESS 0x841Fu
#define fm_effect_21_on SOUND_RAM8(fm_effect_21_on_ADDRESS)
#define fm_effect_21_timer_ADDRESS 0x8420u
#define fm_effect_21_timer SOUND_RAM16(fm_effect_21_timer_ADDRESS)
#define fm_effect_21_attenuation_ADDRESS 0x8422u
#define fm_effect_21_attenuation SOUND_RAM8(fm_effect_21_attenuation_ADDRESS)
#define fm_effect_21_pitch_ADDRESS 0x8423u
#define fm_effect_21_pitch SOUND_RAM16(fm_effect_21_pitch_ADDRESS)
#define fm_effect_23_pitch_ADDRESS 0x8425u
#define fm_effect_23_pitch SOUND_RAM16(fm_effect_23_pitch_ADDRESS)
#define fm_effect_23_timer_ADDRESS 0x8427u
#define fm_effect_23_timer SOUND_RAM8(fm_effect_23_timer_ADDRESS)
#define fm_effect_23_steps_ADDRESS 0x8428u
#define fm_effect_23_steps SOUND_RAM8(fm_effect_23_steps_ADDRESS)
#define fm_effect_23_rounds_ADDRESS 0x8429u
#define fm_effect_23_rounds SOUND_RAM8(fm_effect_23_rounds_ADDRESS)
#define fm_effect_23_attenuation_ADDRESS 0x842Au
#define fm_effect_23_attenuation SOUND_RAM8(fm_effect_23_attenuation_ADDRESS)
#define fm_effect_25_timer_ADDRESS 0x842Bu
#define fm_effect_25_timer SOUND_RAM8(fm_effect_25_timer_ADDRESS)
#define fm_effect_25_pitch_ADDRESS 0x842Cu
#define fm_effect_25_pitch SOUND_RAM16(fm_effect_25_pitch_ADDRESS)
#define fm_effect_26_timer_ADDRESS 0x842Eu
#define fm_effect_26_timer SOUND_RAM8(fm_effect_26_timer_ADDRESS)
#define fm_effect_26_notes_ADDRESS 0x842Fu
#define fm_effect_26_notes SOUND_RAM8(fm_effect_26_notes_ADDRESS)
#define fm_effect_26_rounds_ADDRESS 0x8430u
#define fm_effect_26_rounds SOUND_RAM8(fm_effect_26_rounds_ADDRESS)
#define fm_effect_26_attenuation_ADDRESS 0x8431u
#define fm_effect_26_attenuation SOUND_RAM8(fm_effect_26_attenuation_ADDRESS)
#define fm_effect_26_pitch_ADDRESS 0x8432u
#define fm_effect_26_pitch SOUND_RAM16(fm_effect_26_pitch_ADDRESS)
#define fm_effect_26_previous_pitch_ADDRESS 0x8434u
#define fm_effect_26_previous_pitch SOUND_RAM16(fm_effect_26_previous_pitch_ADDRESS)
#define fm_effect_28_pitch_ADDRESS 0x8436u
#define fm_effect_28_pitch SOUND_RAM16(fm_effect_28_pitch_ADDRESS)
#define fm_effect_28_timer_ADDRESS 0x8438u
#define fm_effect_28_timer SOUND_RAM8(fm_effect_28_timer_ADDRESS)
#define fm_effect_28_period_ADDRESS 0x8439u
#define fm_effect_28_period SOUND_RAM8(fm_effect_28_period_ADDRESS)
#define fm_effect_28_steps_ADDRESS 0x843Au
#define fm_effect_28_steps SOUND_RAM8(fm_effect_28_steps_ADDRESS)
#define fm_effect_29_timer_ADDRESS 0x843Cu
#define fm_effect_29_timer SOUND_RAM8(fm_effect_29_timer_ADDRESS)
#define fm_effect_29_period_ADDRESS 0x843Du
#define fm_effect_29_period SOUND_RAM8(fm_effect_29_period_ADDRESS)
#define fm_effect_29_pitch_ADDRESS 0x843Eu
#define fm_effect_29_pitch SOUND_RAM16(fm_effect_29_pitch_ADDRESS)
#define fm_effect_29_on_ADDRESS 0x8440u
#define fm_effect_29_on SOUND_RAM8(fm_effect_29_on_ADDRESS)
#define fm_effect_2d_timer_ADDRESS 0x8441u
#define fm_effect_2d_timer SOUND_RAM8(fm_effect_2d_timer_ADDRESS)
#define fm_effect_2d_attenuation_ADDRESS 0x8442u
#define fm_effect_2d_attenuation SOUND_RAM8(fm_effect_2d_attenuation_ADDRESS)
#define fm_effect_2d_base_pitch_ADDRESS 0x8443u
#define fm_effect_2d_base_pitch SOUND_RAM16(fm_effect_2d_base_pitch_ADDRESS)
#define fm_effect_2d_pitch_ADDRESS 0x8445u
#define fm_effect_2d_pitch SOUND_RAM16(fm_effect_2d_pitch_ADDRESS)
#define fm_effect_2d_notes_ADDRESS 0x8447u
#define fm_effect_2d_notes SOUND_RAM8(fm_effect_2d_notes_ADDRESS)
#define fm_effect_30_timer_ADDRESS 0x8448u
#define fm_effect_30_timer SOUND_RAM8(fm_effect_30_timer_ADDRESS)
#define fm_effect_30_repeats_ADDRESS 0x8449u
#define fm_effect_30_repeats SOUND_RAM8(fm_effect_30_repeats_ADDRESS)
#define fm_effect_30_pitch_ADDRESS 0x844Au
#define fm_effect_30_pitch SOUND_RAM16(fm_effect_30_pitch_ADDRESS)
#define fm_effect_30_attenuation_ADDRESS 0x844Cu
#define fm_effect_30_attenuation SOUND_RAM8(fm_effect_30_attenuation_ADDRESS)
#define fm_effect_32_timer_ADDRESS 0x844Du
#define fm_effect_32_timer SOUND_RAM8(fm_effect_32_timer_ADDRESS)
#define fm_effect_32_attenuation_ADDRESS 0x844Eu
#define fm_effect_32_attenuation SOUND_RAM8(fm_effect_32_attenuation_ADDRESS)
#define fm_effect_32_pitch_ADDRESS 0x844Fu
#define fm_effect_32_pitch SOUND_RAM16(fm_effect_32_pitch_ADDRESS)
#define fm_effect_32_repeats_ADDRESS 0x8451u
#define fm_effect_32_repeats SOUND_RAM8(fm_effect_32_repeats_ADDRESS)
#define fm_effect_34_pitch_ADDRESS 0x8452u
#define fm_effect_34_pitch SOUND_RAM16(fm_effect_34_pitch_ADDRESS)
#define fm_effect_34_timer_ADDRESS 0x8454u
#define fm_effect_34_timer SOUND_RAM8(fm_effect_34_timer_ADDRESS)
#define fm_effect_34_length_ADDRESS 0x8455u
#define fm_effect_34_length SOUND_RAM8(fm_effect_34_length_ADDRESS)
#define fm_effect_34_notes_ADDRESS 0x8456u
#define fm_effect_34_notes SOUND_RAM8(fm_effect_34_notes_ADDRESS)
#define fm_effect_34_attenuation_ADDRESS 0x8457u
#define fm_effect_34_attenuation SOUND_RAM8(fm_effect_34_attenuation_ADDRESS)
#define fm_effect_34_on_ADDRESS 0x8458u
#define fm_effect_34_on SOUND_RAM8(fm_effect_34_on_ADDRESS)
#define fm_effect_36_timer_ADDRESS 0x8459u
#define fm_effect_36_timer SOUND_RAM8(fm_effect_36_timer_ADDRESS)
#define fm_effect_36_pitch_ADDRESS 0x845Bu
#define fm_effect_36_pitch SOUND_RAM16(fm_effect_36_pitch_ADDRESS)
#define fm_effect_38_timer_ADDRESS 0x845Eu
#define fm_effect_38_timer SOUND_RAM8(fm_effect_38_timer_ADDRESS)
#define fm_effect_38_attenuation_ADDRESS 0x845Fu
#define fm_effect_38_attenuation SOUND_RAM8(fm_effect_38_attenuation_ADDRESS)
#define fm_effect_38_steps_ADDRESS 0x8460u
#define fm_effect_38_steps SOUND_RAM8(fm_effect_38_steps_ADDRESS)
#define fm_effect_39_timer_ADDRESS 0x8461u
#define fm_effect_39_timer SOUND_RAM8(fm_effect_39_timer_ADDRESS)
#define fm_effect_39_attenuation_ADDRESS 0x8462u
#define fm_effect_39_attenuation SOUND_RAM8(fm_effect_39_attenuation_ADDRESS)
#define fm_effect_39_pitch_ADDRESS 0x8463u
#define fm_effect_39_pitch SOUND_RAM16(fm_effect_39_pitch_ADDRESS)
#define fm_effect_39_notes_ADDRESS 0x8465u
#define fm_effect_39_notes SOUND_RAM8(fm_effect_39_notes_ADDRESS)
#define fm_effect_41_pitch_ADDRESS 0x8466u
#define fm_effect_41_pitch SOUND_RAM16(fm_effect_41_pitch_ADDRESS)
#define fm_effect_41_timer_ADDRESS 0x8468u
#define fm_effect_41_timer SOUND_RAM8(fm_effect_41_timer_ADDRESS)
#define fm_effect_41_steps_ADDRESS 0x8469u
#define fm_effect_41_steps SOUND_RAM8(fm_effect_41_steps_ADDRESS)
#define fm_effect_41_rounds_ADDRESS 0x846Au
#define fm_effect_41_rounds SOUND_RAM8(fm_effect_41_rounds_ADDRESS)
#define fm_effect_41_attenuation_ADDRESS 0x846Bu
#define fm_effect_41_attenuation SOUND_RAM8(fm_effect_41_attenuation_ADDRESS)
#define random_pool_ADDRESS 0x8600u
#define random_pool ((uint8_t *)sound_ram_at(random_pool_ADDRESS))

/* data in the program ROM the code reads, which the game loads from its assets */
#define speech_priorities_ADDRESS 0x0130u
extern const void *speech_priorities_data;
#define speech_priorities ((const uint8_t *)speech_priorities_data)
#define speech_music_attenuation_ADDRESS 0x0154u
extern const void *speech_music_attenuation_data;
#define speech_music_attenuation ((const uint8_t *)speech_music_attenuation_data)
#define pcm_effect_priorities_ADDRESS 0x025Au
extern const void *pcm_effect_priorities_data;
#define pcm_effect_priorities ((const uint8_t *)pcm_effect_priorities_data)
#define fm_effect_priorities_ADDRESS 0x02FBu
extern const void *fm_effect_priorities_data;
#define fm_effect_priorities ((const uint8_t *)fm_effect_priorities_data)
#define fm_key_codes_ADDRESS 0x073Du
extern const void *fm_key_codes_data;
#define fm_key_codes ((const uint8_t *)fm_key_codes_data)
#define fm_carrier_masks_ADDRESS 0x07DEu
extern const void *fm_carrier_masks_data;
#define fm_carrier_masks ((const uint8_t *)fm_carrier_masks_data)
#define fm_note_pitches_ADDRESS 0x0B1Cu
extern const void *fm_note_pitches_data;
#define fm_note_pitches ((const uint16_t *)fm_note_pitches_data)
#define pcm_note_pitches_ADDRESS 0x0C96u
extern const void *pcm_note_pitches_data;
#define pcm_note_pitches (*(const uint8_t *)pcm_note_pitches_data)
#define note_lengths_ADDRESS 0x0CF9u
extern const void *note_lengths_data;
#define note_lengths ((const uint8_t *)note_lengths_data)
#define track_commands_e0_ADDRESS 0x0D4Du
extern const void *track_commands_e0_data;
#define track_commands_e0 (*(const uint16_t *)track_commands_e0_data)
#define track_commands_f0_ADDRESS 0x0D6Du
extern const void *track_commands_f0_data;
#define track_commands_f0 (*(const uint16_t *)track_commands_f0_data)
#define pcm_effect_handlers_ADDRESS 0x106Cu
extern const void *pcm_effect_handlers_data;
#define pcm_effect_handlers ((const uint16_t *)pcm_effect_handlers_data)
#define fm_effect_handlers_ADDRESS 0x13DBu
extern const void *fm_effect_handlers_data;
#define fm_effect_handlers (*(const uint16_t *)fm_effect_handlers_data)
#define fm_effect_26_pitches_ADDRESS 0x2B04u
extern const void *fm_effect_26_pitches_data;
#define fm_effect_26_pitches ((const uint16_t *)fm_effect_26_pitches_data)
#define fm_voice_pointers_ADDRESS 0x37B5u
extern const void *fm_voice_pointers_data;
#define fm_voice_pointers ((const uint16_t *)fm_voice_pointers_data)

/* the board's registers on the Z80's bus: 9000 (sres), the 68000's latch, the K007232, the
   YM2151 and the uPD7759 (read and written through the bus) */
#define sound_control 0x9000u
#define sound_latch 0xA000u
#define pcm_a_pitch 0xB000u
#define pcm_a_start 0xB002u
#define pcm_a_bank 0xB004u
#define pcm_a_key 0xB005u
#define pcm_b_pitch 0xB006u
#define pcm_b_start 0xB008u
#define pcm_b_bank 0xB00Au
#define pcm_b_key 0xB00Bu
#define pcm_volume_port 0xB00Cu
#define pcm_loop_flags 0xB00Du
#define fm_address 0xC000u
#define fm_data 0xC001u
#define speech_sample 0xD000u
#define speech_start 0xE000u
#define speech_busy 0xF000u

/* data in the program ROM sound_symbols.txt labels */
#define pcm_instruments_ADDRESS 0x1002u
#define pcm_drums_ADDRESS 0x1012u
#define songs_ADDRESS 0x145Du
#define unused_note_hooks_ADDRESS 0x3794u
#define unreached_end_4721_ADDRESS 0x4721u
#define unused_part_5eb2_ADDRESS 0x5EB2u
#define unused_part_5ec7_ADDRESS 0x5EC7u
#define unreached_end_63a5_ADDRESS 0x63A5u
#define unreached_return_6d46_ADDRESS 0x6D46u
#define unused_part_7caa_ADDRESS 0x7CAAu

/* where the code the program holds as values is (the handlers it stores and switches on) */
#define speech_play_ADDRESS 0x0178u
#define speech_wait_ADDRESS 0x01A4u
#define stop_channel_ADDRESS 0x05D0u
#define free_channel_ADDRESS 0x05D6u
#define channel_idle_ADDRESS 0x05E7u
#define stop_pcm_channel_ADDRESS 0x05E8u
#define music_track_start_ADDRESS 0x0885u
#define music_track_first_tick_ADDRESS 0x08CDu
#define music_track_tick_ADDRESS 0x08D6u
#define note_hook_none_ADDRESS 0x09F5u
#define track_set_mode_ADDRESS 0x0D8Du
#define track_set_tempo_ADDRESS 0x0D94u
#define track_set_instrument_ADDRESS 0x0D9Bu
#define track_set_volume_ADDRESS 0x0DC0u
#define track_set_gate_ADDRESS 0x0DD7u
#define track_held_note_ADDRESS 0x0DDEu
#define track_set_detune_ADDRESS 0x0E25u
#define track_set_portamento_ADDRESS 0x0E2Cu
#define track_set_music_transpose_ADDRESS 0x0E33u
#define track_set_transpose_ADDRESS 0x0E59u
#define track_set_music_volume_ADDRESS 0x0E62u
#define track_set_volume_offset_ADDRESS 0x0E74u
#define track_loop_transposed_ADDRESS 0x0E7Du
#define track_set_lfo_ADDRESS 0x0EBDu
#define track_set_lfo_ramp_ADDRESS 0x0EDFu
#define track_nop_ADDRESS 0x0EE6u
#define track_jump_ADDRESS 0x0EE9u
#define track_loop_exit_ADDRESS 0x0EF5u
#define track_loop_start_ADDRESS 0x0F0Fu
#define track_loop_end_ADDRESS 0x0F1Du
#define track_loop_2_start_ADDRESS 0x0F2Cu
#define track_loop_2_end_ADDRESS 0x0F3Au
#define track_call_repeat_ADDRESS 0x0F49u
#define track_return_repeat_ADDRESS 0x0F65u
#define track_call_ADDRESS 0x0F7Eu
#define track_return_ADDRESS 0x0F8Au
#define track_call_2_ADDRESS 0x0F93u
#define track_return_2_ADDRESS 0x0F9Fu
#define track_set_accelerando_ADDRESS 0x0FA8u
#define track_end_ADDRESS 0x0FC0u
#define pcm_effect_countdown_ADDRESS 0x10AFu
#define pcm_effect_81_ADDRESS 0x10D0u
#define pcm_effect_81_next_ADDRESS 0x10F8u
#define pcm_effect_81_echo_ADDRESS 0x1114u
#define pcm_effect_81_echo_wait_ADDRESS 0x111Eu
#define pcm_effect_82_ADDRESS 0x1125u
#define pcm_effect_ducking_fade_ADDRESS 0x114Bu
#define pcm_effect_83_ADDRESS 0x1165u
#define pcm_effect_84_ADDRESS 0x1183u
#define pcm_effect_85_ADDRESS 0x11A1u
#define pcm_effect_86_ADDRESS 0x11AEu
#define pcm_effect_86_countdown_ADDRESS 0x11C8u
#define pcm_effect_8d_ADDRESS 0x11F4u
#define pcm_effect_8d_wait_ADDRESS 0x11FEu
#define pcm_effect_87_ADDRESS 0x120Eu
#define pcm_effect_88_ADDRESS 0x122Cu
#define pcm_effect_88_play_ADDRESS 0x1232u
#define pcm_effect_88_echo_ADDRESS 0x1250u
#define pcm_effect_88_echo_wait_ADDRESS 0x125Au
#define pcm_effect_89_ADDRESS 0x1261u
#define pcm_effect_89_next_ADDRESS 0x128Cu
#define pcm_effect_8a_ADDRESS 0x129Cu
#define pcm_effect_8a_next_ADDRESS 0x12BFu
#define pcm_effect_8b_ADDRESS 0x12D1u
#define pcm_effect_8b_next_ADDRESS 0x12FCu
#define pcm_effect_8b_echo_ADDRESS 0x1338u
#define pcm_effect_8b_echo_wait_ADDRESS 0x1342u
#define pcm_effect_8c_ADDRESS 0x134Cu
#define pcm_effect_8e_ADDRESS 0x1359u
#define pcm_effect_8f_ADDRESS 0x137Du
#define pcm_effect_90_ADDRESS 0x13A3u
#define pcm_effect_91_ADDRESS 0x13C1u
#define pcm_effect_92_ADDRESS 0x13CEu
#define pcm_effect_93_ADDRESS 0x13D4u
#define fm_effect_01_ADDRESS 0x1638u
#define fm_effect_01_hold_ADDRESS 0x1662u
#define fm_effect_02_ADDRESS 0x169Au
#define fm_effect_02_slide_ADDRESS 0x16CFu
#define fm_effect_02_pause_ADDRESS 0x1727u
#define fm_effect_03_ADDRESS 0x174Fu
#define fm_effect_03_fall_ADDRESS 0x177Du
#define fm_effect_03_trill_ADDRESS 0x17ADu
#define fm_effect_04_ADDRESS 0x17E7u
#define fm_effect_05_ADDRESS 0x1811u
#define fm_effect_wobble_down_ADDRESS 0x183Fu
#define fm_effect_wobble_hold_ADDRESS 0x1868u
#define fm_effect_37_ADDRESS 0x1877u
#define fm_effect_37_wait_ADDRESS 0x1881u
#define fm_effect_06_ADDRESS 0x1888u
#define fm_effect_06_fall_ADDRESS 0x18A9u
#define fm_effect_06_trill_ADDRESS 0x18D9u
#define fm_effect_07_ADDRESS 0x192Cu
#define fm_effect_07_step_ADDRESS 0x1962u
#define fm_effect_08_ADDRESS 0x19D1u
#define fm_effect_08_rise_and_warble_ADDRESS 0x19F9u
#define fm_effect_09_ADDRESS 0x1A23u
#define fm_effect_0a_ADDRESS 0x1A43u
#define fm_effect_0a_rise_ADDRESS 0x1A98u
#define fm_effect_33_hold_ADDRESS 0x1B1Bu
#define fm_effect_0b_ADDRESS 0x1B4Fu
#define fm_effect_0b_hold_ADDRESS 0x1B79u
#define fm_effect_0b_slide_ADDRESS 0x1B9Cu
#define fm_effect_0c_ADDRESS 0x1BDEu
#define fm_effect_zigzag_steps_ADDRESS 0x1C40u
#define fm_effect_0d_ADDRESS 0x1CA0u
#define fm_effect_0d_fall_ADDRESS 0x1CCEu
#define fm_effect_0d_trill_ADDRESS 0x1D0Au
#define fm_effect_0d_pause_ADDRESS 0x1D4Fu
#define fm_effect_0e_ADDRESS 0x1D74u
#define fm_effect_0e_zigzag_down_ADDRESS 0x1D96u
#define fm_effect_0e_zigzag_up_ADDRESS 0x1DE4u
#define fm_effect_0f_ADDRESS 0x1E33u
#define fm_effect_0f_drop_ADDRESS 0x1E4Cu
#define fm_effect_burst_step_ADDRESS 0x1EB5u
#define fm_effect_10_ADDRESS 0x1F07u
#define fm_effect_10_fall_ADDRESS 0x1F37u
#define fm_effect_10_trill_ADDRESS 0x1F7Fu
#define fm_effect_11_ADDRESS 0x1FCBu
#define fm_effect_drop_and_pcm_ADDRESS 0x1FE4u
#define fm_effect_12_ADDRESS 0x1FF6u
#define fm_effect_13_ADDRESS 0x2012u
#define fm_effect_14_ADDRESS 0x203Eu
#define fm_effect_14_fall_ADDRESS 0x2069u
#define fm_effect_15_ADDRESS 0x20ADu
#define fm_effect_15_sink_ADDRESS 0x20F5u
#define fm_effect_15_echo_ADDRESS 0x212Cu
#define fm_effect_15_echo_wait_ADDRESS 0x2136u
#define fm_effect_16_ADDRESS 0x215Au
#define fm_effect_long_rise_ADDRESS 0x2197u
#define fm_effect_16_echo_ADDRESS 0x21C6u
#define fm_effect_16_echo_wait_ADDRESS 0x21D6u
#define fm_effect_17_ADDRESS 0x21F6u
#define fm_effect_17_zigzag_ADDRESS 0x2226u
#define fm_effect_18_ADDRESS 0x2299u
#define fm_effect_19_ADDRESS 0x22D2u
#define fm_effect_ladder_ADDRESS 0x2306u
#define fm_effect_1a_ADDRESS 0x234Eu
#define fm_effect_1a_fall_ADDRESS 0x2393u
#define fm_effect_1a_burst_step_ADDRESS 0x23CAu
#define fm_effect_1a_second_start_ADDRESS 0x244Fu
#define fm_effect_1a_second_fall_ADDRESS 0x247Au
#define fm_effect_1a_second_burst_step_ADDRESS 0x24B7u
#define fm_effect_1b_ADDRESS 0x250Fu
#define fm_effect_1b_rise_ADDRESS 0x2531u
#define fm_effect_1b_hold_ADDRESS 0x2556u
#define fm_effect_1c_ADDRESS 0x257Fu
#define fm_effect_pulses_ADDRESS 0x25B8u
#define fm_effect_1c_echo_ADDRESS 0x25FEu
#define fm_effect_1c_echo_wait_ADDRESS 0x2608u
#define fm_effect_1d_ADDRESS 0x262Au
#define fm_effect_arpeggio_ADDRESS 0x267Au
#define fm_effect_arpeggio_ring_ADDRESS 0x26A5u
#define fm_effect_1d_echo_ADDRESS 0x26ACu
#define fm_effect_1d_echo_wait_ADDRESS 0x26B6u
#define fm_effect_1e_ADDRESS 0x26C4u
#define fm_effect_1e_wait_ADDRESS 0x26D4u
#define fm_effect_1f_ADDRESS 0x26F6u
#define fm_effect_1f_rise_ADDRESS 0x2713u
#define fm_effect_1f_hold_ADDRESS 0x274Du
#define fm_effect_20_ADDRESS 0x2767u
#define fm_effect_21_ADDRESS 0x2785u
#define fm_effect_21_rise_ADDRESS 0x27A7u
#define fm_effect_21_hold_ADDRESS 0x27DDu
#define fm_effect_22_ADDRESS 0x2804u
#define fm_effect_22_on_channel_7_ADDRESS 0x2857u
#define fm_effect_22_on_channel_6_ADDRESS 0x285Cu
#define fm_effect_22_on_channel_5_ADDRESS 0x2861u
#define fm_effect_22_on_channel_4_ADDRESS 0x2866u
#define fm_effect_22_on_channel_3_ADDRESS 0x286Bu
#define fm_effect_22_on_channel_2_ADDRESS 0x2870u
#define fm_effect_22_on_channel_1_ADDRESS 0x2875u
#define fm_effect_22_wait_ADDRESS 0x288Eu
#define fm_effect_23_ADDRESS 0x28B6u
#define fm_effect_23_fall_ADDRESS 0x28E1u
#define fm_effect_24_ADDRESS 0x2925u
#define fm_effect_24_fall_ADDRESS 0x2958u
#define fm_effect_24_burst_step_ADDRESS 0x2998u
#define fm_effect_25_ADDRESS 0x29EBu
#define fm_effect_25_fall_ADDRESS 0x2A0Du
#define fm_effect_25_bend_ADDRESS 0x2A33u
#define fm_effect_26_ADDRESS 0x2A68u
#define fm_effect_26_play_ADDRESS 0x2AABu
#define fm_effect_27_ADDRESS 0x2B2Du
#define fm_effect_27_swell_and_warble_ADDRESS 0x2B52u
#define fm_effect_28_ADDRESS 0x2BDFu
#define fm_effect_28_jitter_ADDRESS 0x2C09u
#define fm_effect_29_ADDRESS 0x2C49u
#define fm_effect_29_fall_ADDRESS 0x2C60u
#define fm_effect_29_tick_ADDRESS 0x2C8Du
#define fm_effect_29_drop_ADDRESS 0x2CBAu
#define fm_effect_2a_ADDRESS 0x2CF5u
#define fm_effect_2a_swell_and_warble_ADDRESS 0x2D1Du
#define fm_effect_2a_rest_ADDRESS 0x2D40u
#define fm_effect_2b_ADDRESS 0x2D7Du
#define fm_effect_2b_drop_ADDRESS 0x2D9Du
#define fm_effect_2c_ADDRESS 0x2DBFu
#define fm_effect_dive_ADDRESS 0x2DF6u
#define fm_effect_dive_climb_ADDRESS 0x2E35u
#define fm_effect_2d_ADDRESS 0x2E77u
#define fm_effect_2d_zigzag_ADDRESS 0x2EAAu
#define fm_effect_2e_ADDRESS 0x2F11u
#define fm_effect_2e_fall_ADDRESS 0x2F3Cu
#define fm_effect_2e_trill_ADDRESS 0x2F76u
#define fm_effect_2f_ADDRESS 0x2FA8u
#define fm_effect_2f_swell_and_warble_ADDRESS 0x2FD0u
#define fm_effect_30_ADDRESS 0x3032u
#define fm_effect_30_sweep_ADDRESS 0x3042u
#define fm_effect_30_slow_sweep_ADDRESS 0x306Fu
#define fm_effect_31_ADDRESS 0x30A4u
#define fm_effect_31_fall_ADDRESS 0x30CFu
#define fm_effect_31_trill_ADDRESS 0x3109u
#define fm_effect_32_ADDRESS 0x3154u
#define fm_effect_32_bend_ADDRESS 0x3181u
#define fm_effect_33_ADDRESS 0x31ADu
#define fm_effect_34_ADDRESS 0x31CEu
#define fm_effect_34_fall_ADDRESS 0x320Du
#define fm_effect_34_trill_ADDRESS 0x3258u
#define fm_effect_34_warbles_ADDRESS 0x329Cu
#define fm_effect_34_warble_ADDRESS 0x32D4u
#define fm_effect_35_ADDRESS 0x3316u
#define fm_effect_36_ADDRESS 0x3336u
#define fm_effect_36_rise_ADDRESS 0x3353u
#define fm_effect_36_hold_ADDRESS 0x3388u
#define fm_effect_36_fall_ADDRESS 0x3395u
#define fm_effect_38_ADDRESS 0x33C2u
#define fm_effect_38_trill_ADDRESS 0x33E9u
#define fm_effect_39_ADDRESS 0x3432u
#define fm_effect_39_next_note_ADDRESS 0x345Fu
#define fm_effect_3a_ADDRESS 0x348Bu
#define fm_effect_3a_warble_ADDRESS 0x34C3u
#define fm_effect_3b_ADDRESS 0x3524u
#define fm_effect_3b_swell_and_warble_ADDRESS 0x354Cu
#define fm_effect_3c_ADDRESS 0x3570u
#define fm_effect_3c_warble_ADDRESS 0x35A6u
#define fm_effect_3d_ADDRESS 0x35D3u
#define fm_effect_3d_on_channel_6_ADDRESS 0x35FDu
#define fm_effect_3d_on_channel_7_ADDRESS 0x3604u
#define fm_effect_3d_wait_ADDRESS 0x361Au
#define fm_effect_3e_ADDRESS 0x3621u
#define fm_effect_3e_delay_ADDRESS 0x363Au
#define fm_effect_3e_wait_ADDRESS 0x3644u
#define fm_effect_3f_ADDRESS 0x3664u
#define fm_effect_fall_and_trills_fall_ADDRESS 0x3694u
#define fm_effect_fall_and_trills_ADDRESS 0x36C8u
#define fm_effect_40_ADDRESS 0x3703u
#define fm_effect_41_ADDRESS 0x373Fu
#define fm_effect_41_rise_ADDRESS 0x3769u

/* numbers the code uses, by name (sound_symbols.txt's constant lines) */
#define RAM_START 0x8000u

/* the sound program's structures in the shared RAM */
#pragma pack(push, 1)
struct channel
{
	uint8_t chip_channel;
	uint8_t sound;
	uint16_t handler;
	uint8_t fm_attenuation;
	uint8_t connection;
	uint8_t m1_total_level;
	uint8_t c1_total_level;
	uint8_t m2_total_level;
	uint8_t c2_total_level;
	uint8_t m1_release;
	uint8_t c1_release;
	uint8_t m2_release;
	uint8_t c2_release;
	uint8_t pad_0E[0x1];
	uint8_t priority;
	uint16_t track;
	uint8_t tempo_count;
	uint8_t ticks_left;
	uint8_t last_tempo;
	uint8_t detune;
	uint8_t lfo_ramp;
	uint8_t portamento;
	uint8_t mode;
	uint8_t transpose;
	uint8_t volume_offset;
	uint8_t loop_count;
	uint8_t loop_2_count;
	uint8_t key_off_ticks;
	union
	{
		struct
		{
			uint8_t octave;
			uint8_t volume;
			uint8_t gate;
			uint8_t ramp_wait;
			uint8_t lfo_sensitivity;
			uint8_t portamento_fraction;
			uint16_t portamento_pitch;
			uint16_t target_pitch;
			uint16_t note_hook;
			uint16_t repeat_return;
			uint16_t loop_start;
			uint16_t loop_2_start;
			uint8_t loop_exit_count;
			uint8_t transposed_loop_count;
			uint8_t loop_transpose;
			uint8_t loop_volume_offset;
			uint8_t unk_34;
			uint8_t repeat_count;
			uint16_t call_return;
			uint8_t instrument;
			uint8_t attenuation;
			uint16_t call_2_return;
			uint8_t pcm_level;
			uint8_t unk_3D;
			uint8_t pad_3E[0x1];
			uint8_t flags;
		};
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t rounds;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t steps;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_0c;
		struct
		{
			uint8_t unread_attenuation;
			uint8_t pad_1F[0x1];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t rounds;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_10;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t rounds;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint8_t fall_step;
			uint8_t pad_33[0x1];
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_15;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t steps;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_18;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t climb_notes;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint16_t start_pitch;
			uint8_t pad_36[0x2];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_2c;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t climb_notes;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint16_t start_pitch;
			uint8_t pad_36[0x2];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_40;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t climb_notes;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint16_t start_pitch;
			uint8_t pad_36[0x2];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_dive;
		struct
		{
			uint8_t pad_1E[0x2];
			uint16_t long_timer;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t rounds;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_long_rise;
		struct
		{
			uint8_t pad_1E[0x2];
			uint8_t timer;
			uint8_t period;
			uint8_t dive_period;
			uint8_t pad_23[0x5];
			uint8_t count;
			uint8_t steps;
			uint8_t burst_flags;
			uint8_t pad_2B[0x1];
			uint8_t attenuation;
			uint8_t round_attenuation;
			uint8_t base_attenuation;
			uint8_t pad_2F[0x1];
			uint16_t pitch;
			uint16_t base_pitch;
			uint8_t rise_step;
			uint8_t pad_35[0x3];
			uint16_t voice;
			uint8_t pad_3A[0x2];
			uint8_t fade_start;
			uint8_t fading_step;
		} fm_effect_zigzag_steps;
		struct
		{
			uint8_t pad_1E[0x12];
			union
			{
				uint16_t ticks;
				uint8_t short_ticks;
			};
			uint8_t volume;
			uint16_t pitch;
			uint8_t wait;
			uint8_t repeats;
		} pcm_effect;
		struct
		{
			uint8_t pad_1E[0x12];
			union
			{
				uint16_t ticks;
				uint8_t short_ticks;
			};
			uint8_t volume;
			uint16_t pitch;
			uint8_t play_length;
			uint8_t repeats;
		} pcm_effect_8b;
	};
};

struct fm_voice
{
	uint8_t connection;
	uint8_t m1_detune_multiple;
	uint8_t m1_total_level;
	uint8_t m1_attack;
	uint8_t m1_decay_1;
	uint8_t m1_decay_2;
	uint8_t m1_release;
	uint8_t c1_detune_multiple;
	uint8_t c1_total_level;
	uint8_t c1_attack;
	uint8_t c1_decay_1;
	uint8_t c1_decay_2;
	uint8_t c1_release;
	uint8_t m2_detune_multiple;
	uint8_t m2_total_level;
	uint8_t m2_attack;
	uint8_t m2_decay_1;
	uint8_t m2_decay_2;
	uint8_t m2_release;
	uint8_t c2_detune_multiple;
	uint8_t c2_total_level;
	uint8_t c2_attack;
	uint8_t c2_decay_1;
	uint8_t c2_decay_2;
	uint8_t c2_release;
};

struct pcm_instrument
{
	uint16_t start;
	uint8_t bank;
	uint8_t level;
};

struct pcm_drum
{
	uint16_t start;
	uint8_t bank;
	uint8_t level;
	uint16_t pitch;
};

struct song
{
	uint16_t tracks[0x9];
};

#pragma pack(pop)

_Static_assert(sizeof(struct channel) == 0x40, "struct channel has the size of the game's blocks");
_Static_assert(sizeof(struct fm_voice) == 0x19, "struct fm_voice has the size of the program ROM's records");
_Static_assert(sizeof(struct pcm_instrument) == 0x4, "struct pcm_instrument has the size of the program ROM's records");
_Static_assert(sizeof(struct pcm_drum) == 0x6, "struct pcm_drum has the size of the program ROM's records");
_Static_assert(sizeof(struct song) == 0x12, "struct song has the size of the program ROM's records");

#define CHANNEL(address) ((struct channel *)sound_ram_at(address))
#define FM_VOICE(address) ((const struct fm_voice *)sound_rom_at(address))
#define PCM_INSTRUMENT(address) ((const struct pcm_instrument *)sound_rom_at(address))
#define PCM_DRUM(address) ((const struct pcm_drum *)sound_rom_at(address))
#define SONG(address) ((const struct song *)sound_rom_at(address))
#define MUSIC_TRACK(address) ((const uint8_t *)sound_rom_at(address))
#define TRACK_COMMAND_HANDLER(address) ((const uint16_t *)sound_rom_at(address))
#define CHANNEL_ADDRESS(n) (0x8000u + (n) * 0x40u)    /* the 15 of them, from 0 */

/* chips.c: the chips' registers: the YM2151's writes and the frame's timer, its voices, keys, pitches and volumes; the K007232's start, pitch, volume and loop */
void write_fm_register_de(uint16_t value, uint16_t fm_register);
void write_fm_channel_register(uint16_t channel, uint16_t *p_fm_write, uint32_t *p_carry);
uint32_t write_fm_register(uint16_t fm_write);
void restart_frame_timer(void);
uint32_t load_fm_voice(uint16_t voice, uint16_t channel);
uint32_t fm_key_on(uint16_t channel);
void fm_key_off(uint16_t channel);
uint32_t fm_fast_release(uint16_t channel);
uint32_t fm_fast_release_operator(uint16_t fm_register, uint16_t channel, uint8_t release);
void fm_restore_release(uint16_t channel);
void set_fm_pitch(uint16_t pitch, uint16_t channel, uint16_t *p_fm_write, uint32_t *p_carry);
uint32_t set_fm_volume_from_record(uint16_t channel);
uint32_t set_fm_volume(uint16_t channel, uint8_t attenuation);
void set_fm_operator_level(uint16_t fm_register, uint16_t channel, uint8_t level, uint16_t *p_carriers, uint32_t *p_carry);
void set_pcm_start(uint8_t bank, uint16_t start, uint16_t channel);
void set_pcm_pitch(uint16_t pitch, uint16_t channel);
uint32_t set_pcm_volume(uint8_t volume, uint16_t channel);
void start_pcm(uint16_t channel);
void set_pcm_loop(uint16_t channel);
uint32_t clear_pcm_loop(uint16_t channel);

/* commands.c: the 68000's sound commands: the reset and its tests, the command's dispatch, the title music, the speech, the starts of the music and the effects, the fade, silence, the channels' stops */
void sound_reset(void);
void check_rom(void);
void BOARD_ORIGINAL(check_rom)(void);
void sound_command(void);
void self_test_failed(void);
void stop_title_music(void);
void title_music_timer(void);
void start_speech(uint8_t command);
void stop_speech(void);
uint32_t speech_play(uint16_t channel);
uint32_t speech_wait(uint16_t channel, uint32_t carry);
void start_pcm_effect(uint8_t command);
uint32_t start_fm_effect(uint8_t command);
void lower_priority_channel(uint16_t channel, uint16_t *p_lowest_channel, uint8_t *p_lowest_priority);
void reset_sound(void);
void silence(void);
void stop_fm_effect_channels(void);
void stop_music_fm_6_7(void);
void stop_music_fm_1_5(void);
void free_music_fm_6_7(void);
void stop_music_pcm_a(void);
void free_music_pcm_a(void);
void stop_pcm_effect_b(void);
void free_music_pcm_b(void);
void stop_music_pcm_b(void);
void stop_pcm_effect_a(void);
void free_speech_channel(void);

/* fm_effects.c: the FM effects (commands 01-41) */
uint32_t fm_effect_01(uint16_t channel);
uint32_t fm_effect_01_note(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_01_hold(uint16_t channel);
uint32_t fm_effect_02(uint16_t channel);
uint32_t fm_effect_02_round(uint16_t channel, uint8_t steps);
uint32_t fm_effect_02_slide(uint16_t channel);
uint32_t fm_effect_02_pause(uint16_t channel, uint32_t carry);
uint32_t fm_effect_03(uint16_t channel);
uint32_t fm_effect_03_fall(uint16_t channel);
uint32_t fm_effect_03_trill(uint16_t channel);
uint32_t fm_effect_04(uint16_t channel);
uint32_t fm_effect_05(uint16_t channel);
uint32_t fm_effect_wobble_twice(uint16_t channel);
uint32_t fm_effect_wobble_note(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_wobble_down(uint16_t channel);
uint32_t fm_effect_wobble_hold(uint16_t channel, uint32_t carry);
void fm_effect_37(uint16_t channel);
uint32_t fm_effect_37_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_06(uint16_t channel);
uint32_t fm_effect_06_fall(uint16_t channel);
uint32_t fm_effect_06_trill(uint16_t channel);
uint32_t fm_effect_07(uint16_t channel);
uint32_t fm_effect_07_round(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_07_note(uint16_t channel);
uint32_t fm_effect_07_step(uint16_t channel);
uint32_t fm_effect_08(uint16_t channel);
uint32_t fm_effect_08_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_08_rise_and_warble(uint16_t channel);
uint32_t fm_effect_09(uint16_t channel);
uint32_t fm_effect_0a(uint16_t channel);
uint32_t fm_effect_0a_round(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_0a_pair(uint16_t channel);
uint32_t fm_effect_0a_note(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_0a_rise(uint16_t channel);
uint32_t fm_effect_33_drone(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_33_hold(uint16_t channel);
uint32_t fm_effect_0b(uint16_t channel);
uint32_t fm_effect_0b_hold(uint16_t channel);
uint32_t fm_effect_0b_slide_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_0b_slide(uint16_t channel);
uint32_t fm_effect_0c(uint16_t channel);
uint32_t fm_effect_zigzag_steps_start(uint16_t channel);
uint32_t fm_effect_zigzag_steps_reload(uint16_t channel);
uint32_t fm_effect_zigzag_steps_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_zigzag_steps_key_on(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_zigzag_steps(uint16_t channel);
uint32_t fm_effect_0d(uint16_t channel);
uint32_t fm_effect_0d_fall(uint16_t channel);
uint32_t fm_effect_0d_trill_round(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_0d_trill(uint16_t channel);
uint32_t fm_effect_0d_pause(uint16_t channel, uint32_t carry);
uint32_t fm_effect_0e(uint16_t channel);
uint32_t fm_effect_0e_zigzag_down(uint16_t channel);
uint32_t fm_effect_0e_zigzag_up_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_0e_zigzag_up(uint16_t channel);
uint32_t fm_effect_0f(uint16_t channel);
uint32_t fm_effect_0f_drop(uint16_t channel);
uint32_t fm_effect_drop_start(uint16_t voice, uint16_t channel);
uint32_t fm_effect_drop(uint16_t channel);
uint32_t fm_effect_bursts_start(uint16_t channel);
uint32_t fm_effect_burst_from_base(uint16_t channel);
uint32_t fm_effect_burst(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_burst_step(uint16_t channel);
uint32_t fm_effect_10(uint16_t channel);
uint32_t fm_effect_10_fall(uint16_t channel);
uint32_t fm_effect_10_trill(uint16_t channel);
uint32_t fm_effect_11(uint16_t channel);
uint32_t fm_effect_drop_and_pcm_start(uint16_t voice, uint16_t channel);
uint32_t fm_effect_drop_and_pcm(uint16_t channel);
uint32_t fm_effect_12(uint16_t channel);
uint32_t fm_effect_13(uint16_t channel);
uint32_t fm_effect_14(uint16_t channel);
uint32_t fm_effect_14_fall(uint16_t channel, uint32_t carry);
uint32_t fm_effect_15(uint16_t channel);
uint32_t fm_effect_15_start(uint16_t channel);
uint32_t fm_effect_15_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_15_sink(uint16_t channel, uint32_t carry);
void fm_effect_15_echo(uint16_t channel);
uint32_t fm_effect_15_echo_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_16(uint16_t channel);
uint32_t fm_effect_16_rise(uint16_t channel);
uint32_t fm_effect_long_rise_start(uint16_t length, uint16_t channel);
uint32_t fm_effect_long_rise(uint16_t channel);
uint32_t fm_effect_16_echo(uint16_t channel);
uint32_t fm_effect_16_echo_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_17(uint16_t channel);
uint32_t fm_effect_17_from_top(uint16_t channel);
uint32_t fm_effect_17_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_17_key_on(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_17_zigzag(uint16_t channel);
uint32_t fm_effect_18(uint16_t channel);
uint32_t fm_effect_19(uint16_t channel);
uint32_t fm_effect_ladder_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_ladder_step(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_ladder(uint16_t channel);
uint32_t fm_effect_1a(uint16_t channel);
uint32_t fm_effect_1a_fall(uint16_t channel);
uint32_t fm_effect_1a_burst(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_1a_burst_step(uint16_t channel);
uint32_t fm_effect_fall(uint16_t channel, uint8_t die_away_pass);
uint32_t fm_effect_trill_slide_down(uint16_t channel);
uint32_t fm_effect_trill_slide(uint16_t step, uint16_t channel);
void fm_effect_quieter_pool_offset(uint16_t channel, uint16_t *p_offset, uint32_t *p_carry);
void fm_effect_keep_pitch(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_1a_second_start(uint16_t channel);
uint32_t fm_effect_1a_second_note(uint16_t channel);
uint32_t fm_effect_1a_second_fall(uint16_t channel);
uint32_t fm_effect_1a_second_burst(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_1a_second_burst_step(uint16_t channel);
uint32_t fm_effect_1b(uint16_t channel);
uint32_t fm_effect_1b_rise(uint16_t channel);
uint32_t fm_effect_1b_hold(uint16_t channel);
uint32_t fm_effect_1c(uint16_t channel);
uint32_t fm_effect_pulses_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_pulses_round(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_pulse(uint16_t channel);
uint32_t fm_effect_pulses(uint16_t channel);
void fm_effect_1c_echo(uint16_t channel);
uint32_t fm_effect_1c_echo_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_1d(uint16_t channel);
uint32_t fm_effect_1d_arpeggio(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_arpeggio_start(uint16_t voice, uint16_t pitch, uint16_t channel);
uint32_t fm_effect_arpeggio_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_arpeggio(uint16_t channel, uint32_t carry);
uint32_t fm_effect_arpeggio_ring(uint16_t channel, uint32_t carry);
void fm_effect_1d_echo(uint16_t channel);
uint32_t fm_effect_1d_echo_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_1e(uint16_t channel);
uint32_t fm_effect_1e_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_1f(uint16_t channel);
uint32_t fm_effect_1f_rise(uint16_t channel);
uint32_t fm_effect_1f_strike(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_1f_hold(uint16_t channel, uint32_t carry);
uint32_t fm_effect_20(uint16_t channel);
uint32_t fm_effect_21(uint16_t channel);
uint32_t fm_effect_21_rise(uint16_t channel);
uint32_t fm_effect_21_hold(uint16_t channel);
uint32_t fm_effect_22(uint16_t channel);
uint32_t fm_effect_22_rise(uint16_t channel);
uint32_t fm_effect_22_on_channel_7(uint16_t channel);
uint32_t fm_effect_22_on_channel_6(uint16_t channel);
uint32_t fm_effect_22_on_channel_5(uint16_t channel);
uint32_t fm_effect_22_on_channel_4(uint16_t channel);
uint32_t fm_effect_22_on_channel_3(uint16_t channel);
uint32_t fm_effect_22_on_channel_2(uint16_t channel);
uint32_t fm_effect_22_on_channel_1(uint16_t channel);
uint32_t fm_effect_22_delay(uint16_t channel, uint8_t waits);
uint32_t fm_effect_22_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_23(uint16_t channel);
uint32_t fm_effect_23_fall(uint16_t channel, uint32_t carry);
uint32_t fm_effect_24(uint16_t channel);
uint32_t fm_effect_24_fall(uint16_t channel);
uint32_t fm_effect_24_burst(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_24_burst_step(uint16_t channel);
uint32_t fm_effect_25(uint16_t channel);
uint32_t fm_effect_25_fall(uint16_t channel, uint32_t carry);
uint32_t fm_effect_25_bend(uint16_t channel);
uint32_t fm_effect_26(uint16_t channel);
uint32_t fm_effect_26_round(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_26_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_26_play(uint16_t channel);
uint32_t fm_effect_27(uint16_t channel);
uint32_t fm_effect_27_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_27_swell_and_warble(uint16_t channel);
void fm_effect_warble(uint16_t channel, uint16_t *p_step, uint32_t *p_carry);
void fm_effect_move_pitch(uint16_t channel, uint16_t *p_step, uint32_t *p_carry);
void fm_effect_set_pitch_plus(uint16_t pitch, uint16_t channel, uint16_t *p_step, uint32_t *p_carry);
void fm_effect_set_pitch(uint16_t pitch, uint16_t channel, uint16_t *p_fm_write, uint32_t *p_carry);
uint32_t fm_effect_set_pitch_minus(uint16_t step, uint16_t pitch, uint16_t channel, uint32_t carry);
uint32_t fm_effect_restart_note(uint16_t channel);
uint32_t fm_effect_swell_or_warble(uint16_t channel, uint32_t carry);
uint8_t fm_effect_raise_base_pitch(uint16_t channel);
uint32_t fm_effect_28(uint16_t channel);
uint32_t fm_effect_28_step(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_28_jitter(uint16_t channel);
uint32_t fm_effect_29(uint16_t channel);
uint32_t fm_effect_29_fall(uint16_t channel);
uint32_t fm_effect_29_tick(uint16_t channel);
uint32_t fm_effect_29_drop(uint16_t channel);
uint32_t fm_effect_29_lower(uint16_t channel);
uint32_t fm_effect_29_strike(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_2a(uint16_t channel);
uint32_t fm_effect_2a_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_2a_swell_and_warble(uint16_t channel);
uint32_t fm_effect_2a_rest(uint16_t channel, uint32_t carry);
uint32_t fm_effect_2b(uint16_t channel);
uint32_t fm_effect_2b_drop(uint16_t channel);
uint32_t fm_effect_2c(uint16_t channel);
uint32_t fm_effect_dive_times(uint16_t channel, uint8_t dives);
uint32_t fm_effect_dive_start(uint16_t channel);
uint32_t fm_effect_dive(uint16_t channel);
uint32_t fm_effect_dive_climb_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_dive_climb(uint16_t channel, uint32_t carry);
uint16_t fm_effect_dive_reload(uint16_t channel);
uint32_t fm_effect_2d(uint16_t channel);
uint32_t fm_effect_2d_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_2d_zigzag(uint16_t channel, uint32_t carry);
uint32_t fm_effect_2e(uint16_t channel);
uint32_t fm_effect_2e_note(uint16_t channel);
uint32_t fm_effect_2e_fall(uint16_t channel);
uint32_t fm_effect_2e_trill_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_2e_trill(uint16_t channel, uint32_t carry);
uint32_t fm_effect_2f(uint16_t channel);
uint32_t fm_effect_2f_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_2f_swell_and_warble(uint16_t channel);
uint32_t fm_effect_30_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_30_raise(uint16_t step, uint16_t channel);
uint32_t fm_effect_30(uint16_t channel);
uint32_t fm_effect_30_sweep(uint16_t channel);
uint32_t fm_effect_30_slow_sweep_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_30_slow_sweep(uint16_t channel);
uint32_t fm_effect_31(uint16_t channel);
uint32_t fm_effect_31_note(uint16_t channel);
uint32_t fm_effect_31_fall(uint16_t channel);
uint32_t fm_effect_31_trill_start(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_31_trill(uint16_t channel, uint32_t carry);
uint32_t fm_effect_32(uint16_t channel);
uint32_t fm_effect_32_note(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_32_bend(uint16_t channel);
uint32_t fm_effect_33(uint16_t channel);
uint32_t fm_effect_34(uint16_t channel);
uint32_t fm_effect_34_fall(uint16_t channel);
uint32_t fm_effect_34_trill_start(uint16_t channel);
uint32_t fm_effect_34_trill(uint16_t channel);
uint32_t fm_effect_34_warbles(uint16_t channel);
uint32_t fm_effect_34_warble_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_34_warble(uint16_t channel);
uint32_t fm_effect_35(uint16_t channel);
uint32_t fm_effect_36(uint16_t channel);
uint32_t fm_effect_36_rise(uint16_t channel);
void fm_effect_36_hold(uint16_t channel);
uint32_t fm_effect_36_fall(uint16_t channel);
uint32_t fm_effect_38(uint16_t channel);
uint32_t fm_effect_38_trill(uint16_t channel);
uint32_t fm_effect_39(uint16_t channel);
uint32_t fm_effect_39_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_39_next_note(uint16_t channel, uint32_t carry);
uint32_t fm_effect_3a(uint16_t channel);
uint32_t fm_effect_3a_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_3a_warble(uint16_t channel);
uint32_t fm_effect_3b(uint16_t channel);
uint32_t fm_effect_3b_note(uint16_t channel, uint8_t length);
uint32_t fm_effect_3b_swell_and_warble(uint16_t channel);
uint32_t fm_effect_3c(uint16_t channel);
uint32_t fm_effect_3c_note(uint16_t pitch, uint16_t channel);
uint32_t fm_effect_3c_warble(uint16_t channel);
uint32_t fm_effect_3d(uint16_t channel);
uint32_t fm_effect_3d_rise(uint16_t channel);
uint32_t fm_effect_3d_on_channel_6(uint16_t channel);
uint32_t fm_effect_3d_on_channel_7(uint16_t channel);
uint32_t fm_effect_3d_delay(uint16_t attenuation, uint16_t channel, uint8_t wait);
uint32_t fm_effect_3d_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_3e(uint16_t channel);
void fm_effect_3e_delay(uint16_t channel);
uint32_t fm_effect_3e_wait(uint16_t channel, uint32_t carry);
uint32_t fm_effect_3f(uint16_t channel);
uint32_t fm_effect_fall_and_trills_start(uint16_t channel);
uint32_t fm_effect_fall_and_trills_fall(uint16_t channel);
uint32_t fm_effect_fall_and_trills_round(uint16_t channel, uint8_t attenuation);
uint32_t fm_effect_fall_and_trills(uint16_t channel, uint32_t carry);
uint32_t fm_effect_40(uint16_t channel);
uint32_t fm_effect_41(uint16_t channel);
uint32_t fm_effect_41_rise(uint16_t channel, uint32_t carry);

/* main.c: the program's frame: sound_main, the main loop, the channels' handlers and their stop */
void sound_main(void);
void sound_main_loop(void);
uint32_t run_channel_handler(uint16_t channel, uint32_t carry);
uint32_t stop_channel(uint16_t channel);
void free_channel(uint16_t channel);
void set_channel_handler(uint16_t handler, uint16_t channel);
void channel_idle(void);
uint32_t stop_pcm_channel(uint16_t channel);

/* music.c: the music: the random bytes, the tracks' tick, their commands and notes, the note hooks */
uint16_t next_random_byte(void);
uint32_t music_track_start(uint16_t channel);
uint32_t music_track_first_tick(uint16_t channel);
uint32_t music_track_tick(uint16_t channel);
void clear_note_hook(uint16_t channel);
void note_hook_none(void);
void accelerando_tick(void);
uint32_t check_fm_effect_override(uint16_t channel);
uint32_t read_track(uint16_t channel);
uint32_t next_note_length(uint16_t note, uint16_t note_place, uint16_t channel);
uint32_t load_instrument(uint16_t channel, uint8_t instrument);
void add_note_length_high(uint16_t channel, uint8_t length_codes);
void add_note_length(uint16_t channel, uint8_t length_code);
uint8_t read_transpose(uint16_t argument);
uint8_t read_volume_offset(uint16_t argument);
uint16_t read_track_address(uint16_t argument, uint16_t channel);
void unused_note_hook(uint16_t channel);

/* pcm_effects.c: the PCM effects (commands 80-93) and their samples */
uint32_t play_pcm_sample(uint8_t bank, uint16_t start, uint16_t ticks, uint16_t channel);
uint32_t pcm_effect_countdown(uint16_t channel);
void queue_pcm_effect_a(uint16_t handler);
uint32_t pcm_effect_81(uint16_t channel);
uint32_t pcm_effect_81_twice(uint16_t channel);
uint32_t pcm_effect_81_play(uint16_t ticks, uint16_t channel);
uint32_t pcm_effect_81_next(uint16_t channel);
void pcm_effect_81_echo(uint16_t channel);
uint32_t pcm_effect_81_echo_wait(uint16_t channel, uint32_t carry);
uint32_t pcm_effect_82(uint16_t channel);
uint32_t pcm_effect_play_ducking(uint16_t pitch, uint16_t channel);
uint32_t pcm_effect_ducking_fade(uint16_t channel);
uint32_t pcm_effect_83(uint16_t channel);
uint32_t pcm_effect_83_play(uint16_t pitch, uint16_t ticks, uint16_t channel);
uint32_t pcm_effect_84(uint16_t channel);
uint32_t pcm_effect_84_sound(uint16_t channel);
uint32_t pcm_effect_84_play(uint16_t pitch, uint16_t ticks, uint16_t channel);
uint32_t pcm_effect_85(uint16_t channel);
uint32_t pcm_effect_86(uint16_t channel);
uint32_t pcm_effect_86_countdown(uint16_t channel);
uint32_t pcm_effect_86_play(uint16_t pitch, uint16_t channel);
void pcm_effect_8d(uint16_t channel);
uint32_t pcm_effect_8d_wait(uint16_t channel, uint32_t carry);
uint32_t pcm_effect_87(uint16_t channel);
uint32_t pcm_effect_88(uint16_t channel);
uint32_t pcm_effect_88_play(uint16_t channel);
void pcm_effect_88_echo(uint16_t channel);
uint32_t pcm_effect_88_echo_wait(uint16_t channel, uint32_t carry);
uint32_t pcm_effect_89(uint16_t channel);
uint32_t pcm_effect_89_play(uint16_t ticks, uint16_t channel);
uint32_t pcm_effect_89_next(uint16_t channel, uint32_t carry);
uint32_t pcm_effect_8a(uint16_t channel);
uint32_t pcm_effect_8a_play(uint16_t channel, uint8_t volume);
uint32_t pcm_effect_8a_next(uint16_t channel, uint32_t carry);
uint32_t pcm_effect_8b(uint16_t channel);
uint32_t pcm_effect_8b_start(uint16_t pitch, uint16_t channel);
uint32_t pcm_effect_8b_play(uint16_t ticks, uint16_t channel);
uint32_t pcm_effect_8b_next(uint16_t channel, uint32_t carry);
void pcm_effect_8b_echo(uint16_t channel);
uint32_t pcm_effect_8b_echo_wait(uint16_t channel, uint32_t carry);
uint32_t pcm_effect_8c(uint16_t channel);
uint32_t pcm_effect_8e(uint16_t channel);
uint32_t pcm_effect_8f(uint16_t channel);
uint32_t pcm_effect_90(uint16_t channel);
uint32_t pcm_effect_91(uint16_t channel);
uint32_t pcm_effect_92(uint16_t channel);
uint32_t pcm_effect_93(uint16_t channel);

#endif
