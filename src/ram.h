/**
 *    Name:        ram.h
 *    Description: the game's variables, tables, board registers and structures
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    03-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#ifndef TMNT_RAM_H
#define TMNT_RAM_H

/* the game's variables in work RAM (060000-063FFF) */
#define WORKING_LAYER 0x063300u
#define LAYER_A 0x063340u
#define LAYER_B 0x063380u
#define game_state_ADDRESS 0x060001u
#define game_state RAM8(game_state_ADDRESS)
#define step_timer_ADDRESS 0x060002u
#define step_timer RAM8(step_timer_ADDRESS)
#define game_step_ADDRESS 0x060003u
#define game_step RAM8(game_step_ADDRESS)
#define scene_timer_ADDRESS 0x060004u
#define scene_timer RAM8(scene_timer_ADDRESS)
#define scene_ADDRESS 0x060005u
#define scene RAM8(scene_ADDRESS)
#define continue_count_ADDRESS 0x060006u
#define continue_count RAM8(continue_count_ADDRESS)
#define scene_step_ADDRESS 0x060007u
#define scene_step RAM8(scene_step_ADDRESS)
#define continue_count_time_ADDRESS 0x060008u
#define continue_count_time RAM8(continue_count_time_ADDRESS)
#define scene_substep_ADDRESS 0x060009u
#define scene_substep RAM8(scene_substep_ADDRESS)
#define scene_subsubstep_ADDRESS 0x06000Bu
#define scene_subsubstep RAM8(scene_subsubstep_ADDRESS)
#define job_queue_write_ADDRESS 0x060010u
#define job_queue_write RAM32(job_queue_write_ADDRESS)
#define job_queue_read_ADDRESS 0x060014u
#define job_queue_read RAM32(job_queue_read_ADDRESS)
#define sound_queue_write_ADDRESS 0x060018u
#define sound_queue_write RAM32(sound_queue_write_ADDRESS)
#define sound_queue_read_ADDRESS 0x06001Cu
#define sound_queue_read RAM32(sound_queue_read_ADDRESS)
#define turtle_inputs_ADDRESS 0x060020u
#define turtle_inputs(index) RAM8(turtle_inputs_ADDRESS + (index))
#define turtle_inputs_last_frame_ADDRESS 0x060024u
#define turtle_inputs_last_frame RAM32(turtle_inputs_last_frame_ADDRESS)
#define turtle_inputs_2_frames_ago_ADDRESS 0x060028u
#define turtle_inputs_2_frames_ago RAM32(turtle_inputs_2_frames_ago_ADDRESS)
#define turtle_inputs_3_frames_ago_ADDRESS 0x06002Cu
#define turtle_inputs_3_frames_ago RAM32(turtle_inputs_3_frames_ago_ADDRESS)
#define dip_switches_1_ADDRESS 0x060030u
#define dip_switches_1 RAM8(dip_switches_1_ADDRESS)
#define dip_switches_2_ADDRESS 0x060031u
#define dip_switches_2 RAM8(dip_switches_2_ADDRESS)
#define dip_switches_3_ADDRESS 0x060032u
#define dip_switches_3 RAM8(dip_switches_3_ADDRESS)
#define player_inputs_ADDRESS 0x060034u
#define player_inputs(index) RAM8(player_inputs_ADDRESS + (index))
#define player_inputs_last_frame_ADDRESS 0x060038u
#define player_inputs_last_frame(index) RAM8(player_inputs_last_frame_ADDRESS + (index))
#define player_inputs_2_frames_ago_ADDRESS 0x06003Cu
#define player_inputs_2_frames_ago RAM32(player_inputs_2_frames_ago_ADDRESS)
#define player_inputs_3_frames_ago_ADDRESS 0x060040u
#define player_inputs_3_frames_ago RAM32(player_inputs_3_frames_ago_ADDRESS)
#define coin_switches_ADDRESS 0x060044u
#define coin_switches RAM8(coin_switches_ADDRESS)
#define coin_switches_last_frame_ADDRESS 0x060045u
#define coin_switches_last_frame RAM8(coin_switches_last_frame_ADDRESS)
#define coin_switches_2_frames_ago_ADDRESS 0x060046u
#define coin_switches_2_frames_ago RAM8(coin_switches_2_frames_ago_ADDRESS)
#define coin_switches_3_frames_ago_ADDRESS 0x060047u
#define coin_switches_3_frames_ago RAM8(coin_switches_3_frames_ago_ADDRESS)
#define demo_held_inputs_ADDRESS 0x060048u
#define demo_held_inputs(index) RAM8(demo_held_inputs_ADDRESS + (index))
#define turtle_presses_ADDRESS 0x06004Cu
#define turtle_presses(index) RAM8(turtle_presses_ADDRESS + (index))
#define player_presses_ADDRESS 0x060050u
#define player_presses(index) RAM8(player_presses_ADDRESS + (index))
#define unused_scroll_reset_bytes_ADDRESS 0x060054u
#define unused_scroll_reset_bytes(index) RAM8(unused_scroll_reset_bytes_ADDRESS + (index))
#define board_control_ADDRESS 0x060058u
#define board_control RAM8(board_control_ADDRESS)
#define layer_priority_ADDRESS 0x060059u
#define layer_priority RAM8(layer_priority_ADDRESS)
#define sprite_control_ADDRESS 0x06005Au
#define sprite_control RAM8(sprite_control_ADDRESS)
#define tile_flip_ADDRESS 0x06005Bu
#define tile_flip RAM8(tile_flip_ADDRESS)
#define layer_a_scroll_x_ADDRESS 0x06005Cu
#define layer_a_scroll_x RAM16(layer_a_scroll_x_ADDRESS)
#define layer_a_scroll_y_ADDRESS 0x06005Eu
#define layer_a_scroll_y RAM16(layer_a_scroll_y_ADDRESS)
#define layer_b_scroll_x_ADDRESS 0x060060u
#define layer_b_scroll_x RAM16(layer_b_scroll_x_ADDRESS)
#define layer_b_scroll_y_ADDRESS 0x060062u
#define layer_b_scroll_y RAM16(layer_b_scroll_y_ADDRESS)
#define tile_banks_0_1_ADDRESS 0x060064u
#define tile_banks_0_1 RAM8(tile_banks_0_1_ADDRESS)
#define tile_banks_2_3_ADDRESS 0x060065u
#define tile_banks_2_3 RAM8(tile_banks_2_3_ADDRESS)
#define scratch_ADDRESS 0x060066u
#define scratch RAM32(scratch_ADDRESS)
#define scratch_byte_ADDRESS 0x060066u
#define scratch_byte RAM8(scratch_byte_ADDRESS)
#define scratch_word_ADDRESS 0x060066u
#define scratch_word RAM16(scratch_word_ADDRESS)
#define balloon_kept_y_ADDRESS 0x060068u
#define balloon_kept_y RAM16(balloon_kept_y_ADDRESS)
#define balloon_kept_line_ADDRESS 0x06006Au
#define balloon_kept_line RAM16(balloon_kept_line_ADDRESS)
#define balloon_kept_bank_ADDRESS 0x06006Cu
#define balloon_kept_bank RAM16(balloon_kept_bank_ADDRESS)
#define top_score_ADDRESS 0x060076u
#define top_score RAM16(top_score_ADDRESS)
#define text_colour_ADDRESS 0x06007Au
#define text_colour RAM8(text_colour_ADDRESS)
#define spare_text_colour_ADDRESS 0x06007Bu
#define spare_text_colour RAM8(spare_text_colour_ADDRESS)
#define music_ADDRESS 0x060080u
#define music RAM8(music_ADDRESS)
#define unused_exit_wait_ADDRESS 0x060081u
#define unused_exit_wait RAM8(unused_exit_wait_ADDRESS)
#define jobs_pending_ADDRESS 0x060082u
#define jobs_pending RAM8(jobs_pending_ADDRESS)
#define palettes_busy_ADDRESS 0x060083u
#define palettes_busy RAM8(palettes_busy_ADDRESS)
#define lives_per_credit_ADDRESS 0x060090u
#define lives_per_credit RAM8(lives_per_credit_ADDRESS)
#define free_play_ADDRESS 0x060091u
#define free_play RAM8(free_play_ADDRESS)
#define demo_sounds_ADDRESS 0x060092u
#define demo_sounds RAM8(demo_sounds_ADDRESS)
#define unused_dip_flag_ADDRESS 0x060093u
#define unused_dip_flag RAM8(unused_dip_flag_ADDRESS)
#define difficulty_setting_ADDRESS 0x060094u
#define difficulty_setting RAM8(difficulty_setting_ADDRESS)
#define unused_dip_setting_flag_ADDRESS 0x060095u
#define unused_dip_setting_flag RAM8(unused_dip_setting_flag_ADDRESS)
#define unused_dip_setting_value_ADDRESS 0x060096u
#define unused_dip_setting_value RAM32(unused_dip_setting_value_ADDRESS)
#define unused_dip_setting_ADDRESS 0x06009Au
#define unused_dip_setting RAM16(unused_dip_setting_ADDRESS)
#define version_ADDRESS 0x06009Fu
#define version RAM8(version_ADDRESS)
#define players_knocked_out_ADDRESS 0x0600A0u
#define players_knocked_out RAM8(players_knocked_out_ADDRESS)
#define players_in_play_ADDRESS 0x0600A1u
#define players_in_play RAM8(players_in_play_ADDRESS)
#define players_credited_ADDRESS 0x0600A2u
#define players_credited RAM8(players_credited_ADDRESS)
#define game_on_ADDRESS 0x0600A3u
#define game_on RAM8(game_on_ADDRESS)
#define frame_count_ADDRESS 0x0600A4u
#define frame_count RAM16(frame_count_ADDRESS)
#define players_joining_ADDRESS 0x0600A6u
#define players_joining RAM8(players_joining_ADDRESS)
#define attract_timer_ADDRESS 0x0600A8u
#define attract_timer RAM8(attract_timer_ADDRESS)
#define demo_hold_2_ADDRESS 0x0600A9u
#define demo_hold_2 RAM8(demo_hold_2_ADDRESS)
#define demo_inputs_1_ADDRESS 0x0600AAu
#define demo_inputs_1 RAM32(demo_inputs_1_ADDRESS)
#define demo_inputs_2_ADDRESS 0x0600AEu
#define demo_inputs_2 RAM32(demo_inputs_2_ADDRESS)
#define players_out_of_lives_ADDRESS 0x0600B3u
#define players_out_of_lives RAM8(players_out_of_lives_ADDRESS)
#define demo_ADDRESS 0x0600B6u
#define demo RAM8(demo_ADDRESS)
#define players_with_lives_ADDRESS 0x0600B7u
#define players_with_lives RAM8(players_with_lives_ADDRESS)
#define hide_press_start_ADDRESS 0x0600B8u
#define hide_press_start RAM8(hide_press_start_ADDRESS)
#define unused_dip_number_ADDRESS 0x0600B9u
#define unused_dip_number RAM8(unused_dip_number_ADDRESS)
#define area_cleared_ADDRESS 0x0600BDu
#define area_cleared RAM8(area_cleared_ADDRESS)
#define saved_frame_count_ADDRESS 0x0600BEu
#define saved_frame_count RAM16(saved_frame_count_ADDRESS)
#define demo_frames_ADDRESS 0x0600C0u
#define demo_frames RAM16(demo_frames_ADDRESS)
#define game_scene_timer_ADDRESS 0x0600C2u
#define game_scene_timer RAM8(game_scene_timer_ADDRESS)
#define player_turtles_ADDRESS 0x0600C4u
#define player_turtles(index) RAM8(player_turtles_ADDRESS + (index))
#define unused_layer_a_scroll_x_copy_ADDRESS 0x0600C8u
#define unused_layer_a_scroll_x_copy RAM16(unused_layer_a_scroll_x_copy_ADDRESS)
#define unused_layer_a_scroll_y_copy_ADDRESS 0x0600CAu
#define unused_layer_a_scroll_y_copy RAM16(unused_layer_a_scroll_y_copy_ADDRESS)
#define unused_layer_b_scroll_x_copy_ADDRESS 0x0600CCu
#define unused_layer_b_scroll_x_copy RAM16(unused_layer_b_scroll_x_copy_ADDRESS)
#define unused_layer_b_scroll_y_copy_ADDRESS 0x0600CEu
#define unused_layer_b_scroll_y_copy RAM16(unused_layer_b_scroll_y_copy_ADDRESS)
#define demo_hold_3_ADDRESS 0x0600D0u
#define demo_hold_3 RAM8(demo_hold_3_ADDRESS)
#define demo_hold_4_ADDRESS 0x0600D1u
#define demo_hold_4 RAM8(demo_hold_4_ADDRESS)
#define demo_inputs_3_ADDRESS 0x0600D2u
#define demo_inputs_3 RAM32(demo_inputs_3_ADDRESS)
#define demo_inputs_4_ADDRESS 0x0600D6u
#define demo_inputs_4 RAM32(demo_inputs_4_ADDRESS)
#define unk_060100_ADDRESS 0x060100u
#define unk_060100(index) RAM16(unk_060100_ADDRESS + (index) * 2u)
#define unk_060104_ADDRESS 0x060104u
#define unk_060104 RAM16(unk_060104_ADDRESS)
#define unk_060106_ADDRESS 0x060106u
#define unk_060106(index) RAM8(unk_060106_ADDRESS + (index))
#define unk_060108_ADDRESS 0x060108u
#define unk_060108 RAM8(unk_060108_ADDRESS)
#define unk_06010C_ADDRESS 0x06010Cu
#define unk_06010C RAM16(unk_06010C_ADDRESS)
#define unk_06010E_ADDRESS 0x06010Eu
#define unk_06010E RAM8(unk_06010E_ADDRESS)
#define unk_060110_ADDRESS 0x060110u
#define unk_060110 RAM8(unk_060110_ADDRESS)
#define unk_060114_ADDRESS 0x060114u
#define unk_060114 RAM16(unk_060114_ADDRESS)
#define unk_060116_ADDRESS 0x060116u
#define unk_060116 RAM8(unk_060116_ADDRESS)
#define unk_060118_ADDRESS 0x060118u
#define unk_060118 RAM8(unk_060118_ADDRESS)
#define unk_06011C_ADDRESS 0x06011Cu
#define unk_06011C RAM16(unk_06011C_ADDRESS)
#define unk_06011E_ADDRESS 0x06011Eu
#define unk_06011E RAM8(unk_06011E_ADDRESS)
#define area_ADDRESS 0x060120u
#define area RAM8(area_ADDRESS)
#define stage_ADDRESS 0x060121u
#define stage RAM8(stage_ADDRESS)
#define area_continues_scene_ADDRESS 0x060122u
#define area_continues_scene RAM8(area_continues_scene_ADDRESS)
#define unused_area_setup_bytes_ADDRESS 0x060123u
#define unused_area_setup_bytes(index) RAM8(unused_area_setup_bytes_ADDRESS + (index))
#define entry_without_blink_ADDRESS 0x060127u
#define entry_without_blink RAM8(entry_without_blink_ADDRESS)
#define profile_character_ADDRESS 0x060128u
#define profile_character RAM8(profile_character_ADDRESS)
#define exit_cutscene_ADDRESS 0x060129u
#define exit_cutscene RAM8(exit_cutscene_ADDRESS)
#define difficulty_ADDRESS 0x060130u
#define difficulty RAM8(difficulty_ADDRESS)
#define prompt_shown_ADDRESS 0x06014Cu
#define prompt_shown(index) RAM8(prompt_shown_ADDRESS + (index))
#define time_without_knockout_ADDRESS 0x060150u
#define time_without_knockout RAM16(time_without_knockout_ADDRESS)
#define sound_stopped_by_coin_ADDRESS 0x060152u
#define sound_stopped_by_coin RAM8(sound_stopped_by_coin_ADDRESS)
#define unk_060302_ADDRESS 0x060302u
#define unk_060302 RAM8(unk_060302_ADDRESS)
#define unk_060303_ADDRESS 0x060303u
#define unk_060303 RAM8(unk_060303_ADDRESS)
#define unk_060304_ADDRESS 0x060304u
#define unk_060304 RAM8(unk_060304_ADDRESS)
#define unk_06030A_ADDRESS 0x06030Au
#define unk_06030A RAM8(unk_06030A_ADDRESS)
#define unk_06030B_ADDRESS 0x06030Bu
#define unk_06030B RAM8(unk_06030B_ADDRESS)
#define unk_06030C_ADDRESS 0x06030Cu
#define unk_06030C RAM8(unk_06030C_ADDRESS)
#define unk_060312_ADDRESS 0x060312u
#define unk_060312 RAM8(unk_060312_ADDRESS)
#define unk_06031A_ADDRESS 0x06031Au
#define unk_06031A RAM8(unk_06031A_ADDRESS)
#define extra_credits_ADDRESS 0x060320u
#define extra_credits RAM8(extra_credits_ADDRESS)
#define new_coin_switches_ADDRESS 0x060325u
#define new_coin_switches RAM8(new_coin_switches_ADDRESS)
#define new_credit_ADDRESS 0x060327u
#define new_credit RAM8(new_credit_ADDRESS)
#define job_queue_ADDRESS 0x060380u
#define job_queue(index) RAM16(job_queue_ADDRESS + (index) * 2u)
#define sound_queue_ADDRESS 0x060400u
#define sound_queue(index) RAM8(sound_queue_ADDRESS + (index))
#define vblank_registers_ADDRESS 0x060440u
#define vblank_registers(index) RAM32(vblank_registers_ADDRESS + (index) * 4u)
#define palette_queue_write_ADDRESS 0x060BF0u
#define palette_queue_write RAM32(palette_queue_write_ADDRESS)
#define palette_queue_read_ADDRESS 0x060BF4u
#define palette_queue_read RAM32(palette_queue_read_ADDRESS)
#define palette_area_banks_ADDRESS 0x060BF8u
#define palette_area_banks RAM8(palette_area_banks_ADDRESS)
#define palette_set_banks_ADDRESS 0x060BF9u
#define palette_set_banks RAM8(palette_set_banks_ADDRESS)
#define palette_black_banks_ADDRESS 0x060BFAu
#define palette_black_banks RAM8(palette_black_banks_ADDRESS)
#define palette_second_half_ADDRESS 0x060BFBu
#define palette_second_half RAM8(palette_second_half_ADDRESS)
#define palette_set_ADDRESS 0x060BFCu
#define palette_set RAM32(palette_set_ADDRESS)
#define marker_times_ADDRESS 0x061100u
#define marker_times(index) RAM8(marker_times_ADDRESS + (index))
#define time_since_balloon_ADDRESS 0x061104u
#define time_since_balloon RAM16(time_since_balloon_ADDRESS)
#define balloons_said_ADDRESS 0x061106u
#define balloons_said RAM8(balloons_said_ADDRESS)
#define diagonal_scroll_ADDRESS 0x061107u
#define diagonal_scroll RAM8S(diagonal_scroll_ADDRESS)
#define intro_over_ADDRESS 0x061108u
#define intro_over RAM8(intro_over_ADDRESS)
#define area_entry_done_ADDRESS 0x061109u
#define area_entry_done RAM8(area_entry_done_ADDRESS)
#define unused_area_frames_ADDRESS 0x06110Au
#define unused_area_frames(index) RAM16(unused_area_frames_ADDRESS + (index) * 2u)
#define drill_arrival_time_ADDRESS 0x06110Eu
#define drill_arrival_time RAM8(drill_arrival_time_ADDRESS)
#define shake_step_ADDRESS 0x06110Fu
#define shake_step RAM8(shake_step_ADDRESS)
#define tv_time_ADDRESS 0x061110u
#define tv_time RAM16(tv_time_ADDRESS)
#define bebop_exchange_lines_ADDRESS 0x061112u
#define bebop_exchange_lines RAM8(bebop_exchange_lines_ADDRESS)
#define balloon_on_right_ADDRESS 0x061113u
#define balloon_on_right RAM8(balloon_on_right_ADDRESS)
#define unused_area_frames_copy_ADDRESS 0x061114u
#define unused_area_frames_copy(index) RAM16(unused_area_frames_copy_ADDRESS + (index) * 2u)
#define floor_cell_down_ADDRESS 0x061118u
#define floor_cell_down RAM8(floor_cell_down_ADDRESS)
#define difficulty_from_time_ADDRESS 0x06111Eu
#define difficulty_from_time RAM8(difficulty_from_time_ADDRESS)
#define spawn_next_ADDRESS 0x061200u
#define spawn_next RAM32(spawn_next_ADDRESS)
#define draw_first_ADDRESS 0x061204u
#define draw_first RAM16(draw_first_ADDRESS)
#define draw_count_ADDRESS 0x061206u
#define draw_count RAM8(draw_count_ADDRESS)
#define turtles_in_play_ADDRESS 0x061207u
#define turtles_in_play RAM8(turtles_in_play_ADDRESS)
#define least_crowded_slot_ADDRESS 0x061208u
#define least_crowded_slot RAM32(least_crowded_slot_ADDRESS)
#define enemies_on_screen_ADDRESS 0x06120Cu
#define enemies_on_screen RAM8(enemies_on_screen_ADDRESS)
#define enemies_per_turtle_ADDRESS 0x06120Du
#define enemies_per_turtle RAM8(enemies_per_turtle_ADDRESS)
#define random_seed_ADDRESS 0x06120Eu
#define random_seed RAM16(random_seed_ADDRESS)
#define spawn_speeds_ADDRESS 0x061210u
#define spawn_speeds RAM32(spawn_speeds_ADDRESS)
#define spawn_directions_ADDRESS 0x061214u
#define spawn_directions RAM32(spawn_directions_ADDRESS)
#define spawn_x_ADDRESS 0x061218u
#define spawn_x RAM32(spawn_x_ADDRESS)
#define spawn_y_ADDRESS 0x06121Cu
#define spawn_y RAM32(spawn_y_ADDRESS)
#define rocksteady_ADDRESS 0x061220u
#define rocksteady RAM32(rocksteady_ADDRESS)
#define pincer_charge_wait_ADDRESS 0x061224u
#define pincer_charge_wait RAM16(pincer_charge_wait_ADDRESS)
#define nearest_slot_ADDRESS 0x061226u
#define nearest_slot RAM32(nearest_slot_ADDRESS)
#define second_nearest_slot_ADDRESS 0x06122Au
#define second_nearest_slot RAM32(second_nearest_slot_ADDRESS)
#define nearest_turtle_ADDRESS 0x06122Eu
#define nearest_turtle RAM32(nearest_turtle_ADDRESS)
#define second_nearest_turtle_ADDRESS 0x061232u
#define second_nearest_turtle RAM32(second_nearest_turtle_ADDRESS)
#define bosses_left_ADDRESS 0x061236u
#define bosses_left RAM8(bosses_left_ADDRESS)
#define rocksteady_hits_to_strike_back_ADDRESS 0x061237u
#define rocksteady_hits_to_strike_back RAM8(rocksteady_hits_to_strike_back_ADDRESS)
#define rocksteady_last_attack_ADDRESS 0x061238u
#define rocksteady_last_attack RAM16(rocksteady_last_attack_ADDRESS)
#define shredder_beaten_ADDRESS 0x06123Au
#define shredder_beaten RAM8(shredder_beaten_ADDRESS)
#define shredder_firing_ray_ADDRESS 0x06123Bu
#define shredder_firing_ray RAM8(shredder_firing_ray_ADDRESS)
#define shredder_clones_made_ADDRESS 0x06123Cu
#define shredder_clones_made RAM8(shredder_clones_made_ADDRESS)
#define bosses_come_out_ADDRESS 0x06123Du
#define bosses_come_out RAM8(bosses_come_out_ADDRESS)
#define pincer_charge_target_ADDRESS 0x06123Eu
#define pincer_charge_target RAM16(pincer_charge_target_ADDRESS)
#define pincer_charge_ADDRESS 0x061240u
#define pincer_charge RAM8(pincer_charge_ADDRESS)
#define pincer_charge_edges_ADDRESS 0x061241u
#define pincer_charge_edges RAM8(pincer_charge_edges_ADDRESS)
#define pincer_charge_at_edges_ADDRESS 0x061242u
#define pincer_charge_at_edges RAM8(pincer_charge_at_edges_ADDRESS)
#define no_pincer_charge_ADDRESS 0x061243u
#define no_pincer_charge RAM8(no_pincer_charge_ADDRESS)
#define rodney_line_said_ADDRESS 0x061244u
#define rodney_line_said RAM8(rodney_line_said_ADDRESS)
#define pincer_charge_crash_ADDRESS 0x061247u
#define pincer_charge_crash RAM8(pincer_charge_crash_ADDRESS)
#define enemy_floor_cell_place_ADDRESS 0x061248u
#define enemy_floor_cell_place RAM32(enemy_floor_cell_place_ADDRESS)
#define stairs_top_height_ADDRESS 0x06124Cu
#define stairs_top_height RAM8(stairs_top_height_ADDRESS)
#define hydrant_sprays_left_ADDRESS 0x061300u
#define hydrant_sprays_left RAM8(hydrant_sprays_left_ADDRESS)
#define unused_grille_ADDRESS 0x061302u
#define unused_grille RAM32(unused_grille_ADDRESS)
#define boss_target_choices_ADDRESS 0x061306u
#define boss_target_choices(index) RAM8(boss_target_choices_ADDRESS + (index))
#define helicopter_count_ADDRESS 0x06130Au
#define helicopter_count RAM8(helicopter_count_ADDRESS)
#define missile_out_ADDRESS 0x06130Bu
#define missile_out RAM8(missile_out_ADDRESS)
#define hold_spent_missiles_ADDRESS 0x06130Cu
#define hold_spent_missiles RAM8(hold_spent_missiles_ADDRESS)
#define spiked_poles_phase_ADDRESS 0x061310u
#define spiked_poles_phase RAM8(spiked_poles_phase_ADDRESS)
#define flying_drones_released_ADDRESS 0x061311u
#define flying_drones_released RAM8(flying_drones_released_ADDRESS)
#define flying_drone_count_ADDRESS 0x061312u
#define flying_drone_count RAM8(flying_drone_count_ADDRESS)
#define mousers_break_down_ADDRESS 0x061313u
#define mousers_break_down RAM8(mousers_break_down_ADDRESS)
#define krang_curtain_ADDRESS 0x061314u
#define krang_curtain RAM32(krang_curtain_ADDRESS)
#define drone_sound_on_ADDRESS 0x061318u
#define drone_sound_on RAM8(drone_sound_on_ADDRESS)
#define drone_sound_wait_ADDRESS 0x061319u
#define drone_sound_wait RAM8(drone_sound_wait_ADDRESS)
#define krang_lasers_waiting_ADDRESS 0x06131Au
#define krang_lasers_waiting RAM8(krang_lasers_waiting_ADDRESS)
#define krang_fist_flying_ADDRESS 0x06131Bu
#define krang_fist_flying RAM8(krang_fist_flying_ADDRESS)
#define krang_posed_last_ADDRESS 0x06131Cu
#define krang_posed_last RAM8(krang_posed_last_ADDRESS)
#define boss_defeated_ADDRESS 0x06131Du
#define boss_defeated RAM8(boss_defeated_ADDRESS)
#define draw_list_ADDRESS 0x061400u
#define draw_list(index) RAM16(draw_list_ADDRESS + (index) * 2u)
#define floor_map_ADDRESS 0x061800u
#define floor_map(index) RAM8(floor_map_ADDRESS + (index))
#define unk_062000_ADDRESS 0x062000u
#define unk_062000 RAM8(unk_062000_ADDRESS)
#define unk_062001_ADDRESS 0x062001u
#define unk_062001 RAM8(unk_062001_ADDRESS)
#define unk_062008_ADDRESS 0x062008u
#define unk_062008 RAM8(unk_062008_ADDRESS)
#define unk_06200C_ADDRESS 0x06200Cu
#define unk_06200C RAM16(unk_06200C_ADDRESS)
#define unk_06200E_ADDRESS 0x06200Eu
#define unk_06200E RAM16(unk_06200E_ADDRESS)
#define unk_062010_ADDRESS 0x062010u
#define unk_062010 RAM16(unk_062010_ADDRESS)
#define unk_062012_ADDRESS 0x062012u
#define unk_062012 RAM32(unk_062012_ADDRESS)
#define unk_062016_ADDRESS 0x062016u
#define unk_062016 RAM32(unk_062016_ADDRESS)
#define unk_06202C_ADDRESS 0x06202Cu
#define unk_06202C RAM8(unk_06202C_ADDRESS)
#define unk_062042_ADDRESS 0x062042u
#define unk_062042 RAM8(unk_062042_ADDRESS)
#define unk_062050_ADDRESS 0x062050u
#define unk_062050 RAM8(unk_062050_ADDRESS)
#define unk_062051_ADDRESS 0x062051u
#define unk_062051 RAM8(unk_062051_ADDRESS)
#define unk_062058_ADDRESS 0x062058u
#define unk_062058 RAM8(unk_062058_ADDRESS)
#define unk_06205C_ADDRESS 0x06205Cu
#define unk_06205C RAM16(unk_06205C_ADDRESS)
#define unk_06205E_ADDRESS 0x06205Eu
#define unk_06205E RAM16(unk_06205E_ADDRESS)
#define unk_062060_ADDRESS 0x062060u
#define unk_062060 RAM16(unk_062060_ADDRESS)
#define unk_062062_ADDRESS 0x062062u
#define unk_062062 RAM32(unk_062062_ADDRESS)
#define unk_062066_ADDRESS 0x062066u
#define unk_062066 RAM32(unk_062066_ADDRESS)
#define unk_06207C_ADDRESS 0x06207Cu
#define unk_06207C RAM8(unk_06207C_ADDRESS)
#define unk_062092_ADDRESS 0x062092u
#define unk_062092 RAM8(unk_062092_ADDRESS)
#define unk_0620A0_ADDRESS 0x0620A0u
#define unk_0620A0 RAM8(unk_0620A0_ADDRESS)
#define unk_0620A1_ADDRESS 0x0620A1u
#define unk_0620A1 RAM8(unk_0620A1_ADDRESS)
#define unk_0620A8_ADDRESS 0x0620A8u
#define unk_0620A8 RAM8(unk_0620A8_ADDRESS)
#define unk_0620AC_ADDRESS 0x0620ACu
#define unk_0620AC RAM16(unk_0620AC_ADDRESS)
#define unk_0620B2_ADDRESS 0x0620B2u
#define unk_0620B2 RAM32(unk_0620B2_ADDRESS)
#define unk_0620B6_ADDRESS 0x0620B6u
#define unk_0620B6 RAM32(unk_0620B6_ADDRESS)
#define unk_0620CC_ADDRESS 0x0620CCu
#define unk_0620CC RAM8(unk_0620CC_ADDRESS)
#define unk_0620F0_ADDRESS 0x0620F0u
#define unk_0620F0 RAM8(unk_0620F0_ADDRESS)
#define unk_0620F1_ADDRESS 0x0620F1u
#define unk_0620F1 RAM8(unk_0620F1_ADDRESS)
#define unk_0620F8_ADDRESS 0x0620F8u
#define unk_0620F8 RAM8(unk_0620F8_ADDRESS)
#define unk_062102_ADDRESS 0x062102u
#define unk_062102 RAM32(unk_062102_ADDRESS)
#define unk_062106_ADDRESS 0x062106u
#define unk_062106 RAM32(unk_062106_ADDRESS)
#define unk_06211C_ADDRESS 0x06211Cu
#define unk_06211C RAM8(unk_06211C_ADDRESS)
#define unk_0621E0_ADDRESS 0x0621E0u
#define unk_0621E0 RAM16(unk_0621E0_ADDRESS)
#define unk_0621EC_ADDRESS 0x0621ECu
#define unk_0621EC RAM32(unk_0621EC_ADDRESS)
#define unk_062213_ADDRESS 0x062213u
#define unk_062213 RAM8(unk_062213_ADDRESS)
#define unk_062222_ADDRESS 0x062222u
#define unk_062222 RAM8(unk_062222_ADDRESS)
#define unk_062230_ADDRESS 0x062230u
#define unk_062230 RAM16(unk_062230_ADDRESS)
#define unk_06223C_ADDRESS 0x06223Cu
#define unk_06223C RAM32(unk_06223C_ADDRESS)
#define unk_062272_ADDRESS 0x062272u
#define unk_062272 RAM16(unk_062272_ADDRESS)
#define unk_0623C0_ADDRESS 0x0623C0u
#define unk_0623C0 RAM8(unk_0623C0_ADDRESS)
#define unk_0623CC_ADDRESS 0x0623CCu
#define unk_0623CC RAM16(unk_0623CC_ADDRESS)
#define unk_0623CE_ADDRESS 0x0623CEu
#define unk_0623CE RAM16(unk_0623CE_ADDRESS)
#define unk_0623DA_ADDRESS 0x0623DAu
#define unk_0623DA RAM16(unk_0623DA_ADDRESS)
#define unk_0623DC_ADDRESS 0x0623DCu
#define unk_0623DC RAM16(unk_0623DC_ADDRESS)
#define unk_0623E0_ADDRESS 0x0623E0u
#define unk_0623E0 RAM16(unk_0623E0_ADDRESS)
#define unk_0623E2_ADDRESS 0x0623E2u
#define unk_0623E2 RAM16(unk_0623E2_ADDRESS)
#define unk_062402_ADDRESS 0x062402u
#define unk_062402 RAM8(unk_062402_ADDRESS)
#define unk_062410_ADDRESS 0x062410u
#define unk_062410 RAM8(unk_062410_ADDRESS)
#define unk_06241F_ADDRESS 0x06241Fu
#define unk_06241F RAM8(unk_06241F_ADDRESS)
#define unk_062452_ADDRESS 0x062452u
#define unk_062452 RAM8(unk_062452_ADDRESS)
#define unk_062460_ADDRESS 0x062460u
#define unk_062460 RAM8(unk_062460_ADDRESS)
#define unk_06246E_ADDRESS 0x06246Eu
#define unk_06246E RAM16(unk_06246E_ADDRESS)
#define unk_0624B0_ADDRESS 0x0624B0u
#define unk_0624B0 RAM8(unk_0624B0_ADDRESS)
#define unk_0624BE_ADDRESS 0x0624BEu
#define unk_0624BE RAM16(unk_0624BE_ADDRESS)
#define unk_062C80_ADDRESS 0x062C80u
#define unk_062C80 RAM16(unk_062C80_ADDRESS)
#define unk_062C8C_ADDRESS 0x062C8Cu
#define unk_062C8C RAM16(unk_062C8C_ADDRESS)
#define unk_062C8E_ADDRESS 0x062C8Eu
#define unk_062C8E RAM16(unk_062C8E_ADDRESS)
#define unk_062C90_ADDRESS 0x062C90u
#define unk_062C90 RAM16(unk_062C90_ADDRESS)
#define unk_062C92_ADDRESS 0x062C92u
#define unk_062C92 RAM32(unk_062C92_ADDRESS)
#define unk_062C96_ADDRESS 0x062C96u
#define unk_062C96 RAM32(unk_062C96_ADDRESS)
#define unk_062C9A_ADDRESS 0x062C9Au
#define unk_062C9A RAM16(unk_062C9A_ADDRESS)
#define unk_062CA0_ADDRESS 0x062CA0u
#define unk_062CA0 RAM8(unk_062CA0_ADDRESS)
#define unk_062CC2_ADDRESS 0x062CC2u
#define unk_062CC2 RAM8(unk_062CC2_ADDRESS)
#define unk_062CD0_ADDRESS 0x062CD0u
#define unk_062CD0 RAM16(unk_062CD0_ADDRESS)
#define unk_062CDC_ADDRESS 0x062CDCu
#define unk_062CDC RAM16(unk_062CDC_ADDRESS)
#define unk_062CDE_ADDRESS 0x062CDEu
#define unk_062CDE RAM16(unk_062CDE_ADDRESS)
#define unk_062CE0_ADDRESS 0x062CE0u
#define unk_062CE0 RAM16(unk_062CE0_ADDRESS)
#define unk_062CE2_ADDRESS 0x062CE2u
#define unk_062CE2 RAM32(unk_062CE2_ADDRESS)
#define unk_062CE6_ADDRESS 0x062CE6u
#define unk_062CE6 RAM32(unk_062CE6_ADDRESS)
#define unk_062D12_ADDRESS 0x062D12u
#define unk_062D12 RAM8(unk_062D12_ADDRESS)
#define unk_062DC0_ADDRESS 0x062DC0u
#define unk_062DC0 RAM8(unk_062DC0_ADDRESS)
#define unk_062DCC_ADDRESS 0x062DCCu
#define unk_062DCC RAM16(unk_062DCC_ADDRESS)
#define unk_062DCE_ADDRESS 0x062DCEu
#define unk_062DCE RAM16(unk_062DCE_ADDRESS)
#define unk_062DD0_ADDRESS 0x062DD0u
#define unk_062DD0 RAM16(unk_062DD0_ADDRESS)
#define unk_062DD2_ADDRESS 0x062DD2u
#define unk_062DD2 RAM32(unk_062DD2_ADDRESS)
#define unk_062DD6_ADDRESS 0x062DD6u
#define unk_062DD6 RAM32(unk_062DD6_ADDRESS)
#define unk_062E02_ADDRESS 0x062E02u
#define unk_062E02 RAM16(unk_062E02_ADDRESS)
#define unk_062E10_ADDRESS 0x062E10u
#define unk_062E10 RAM8(unk_062E10_ADDRESS)
#define unk_062E1C_ADDRESS 0x062E1Cu
#define unk_062E1C RAM16(unk_062E1C_ADDRESS)
#define unk_062E1E_ADDRESS 0x062E1Eu
#define unk_062E1E RAM16(unk_062E1E_ADDRESS)
#define unk_062E20_ADDRESS 0x062E20u
#define unk_062E20 RAM16(unk_062E20_ADDRESS)
#define unk_062E22_ADDRESS 0x062E22u
#define unk_062E22 RAM32(unk_062E22_ADDRESS)
#define unk_062E26_ADDRESS 0x062E26u
#define unk_062E26 RAM32(unk_062E26_ADDRESS)
#define unk_062E52_ADDRESS 0x062E52u
#define unk_062E52 RAM8(unk_062E52_ADDRESS)
#define unk_062E60_ADDRESS 0x062E60u
#define unk_062E60 RAM8(unk_062E60_ADDRESS)
#define unk_062E6C_ADDRESS 0x062E6Cu
#define unk_062E6C RAM16(unk_062E6C_ADDRESS)
#define unk_062E6E_ADDRESS 0x062E6Eu
#define unk_062E6E RAM16(unk_062E6E_ADDRESS)
#define unk_062E72_ADDRESS 0x062E72u
#define unk_062E72 RAM32(unk_062E72_ADDRESS)
#define unk_062E76_ADDRESS 0x062E76u
#define unk_062E76 RAM32(unk_062E76_ADDRESS)
#define unk_062EA2_ADDRESS 0x062EA2u
#define unk_062EA2 RAM8(unk_062EA2_ADDRESS)
#define unk_062EB0_ADDRESS 0x062EB0u
#define unk_062EB0 RAM8(unk_062EB0_ADDRESS)
#define unk_062EBC_ADDRESS 0x062EBCu
#define unk_062EBC RAM16(unk_062EBC_ADDRESS)
#define unk_062EBE_ADDRESS 0x062EBEu
#define unk_062EBE RAM16(unk_062EBE_ADDRESS)
#define unk_062EC2_ADDRESS 0x062EC2u
#define unk_062EC2 RAM32(unk_062EC2_ADDRESS)
#define unk_062EC6_ADDRESS 0x062EC6u
#define unk_062EC6 RAM32(unk_062EC6_ADDRESS)
#define unk_062ECA_ADDRESS 0x062ECAu
#define unk_062ECA RAM8(unk_062ECA_ADDRESS)
#define unk_062EF2_ADDRESS 0x062EF2u
#define unk_062EF2 RAM16(unk_062EF2_ADDRESS)
#define unk_062F00_ADDRESS 0x062F00u
#define unk_062F00 RAM8(unk_062F00_ADDRESS)
#define unk_062F0C_ADDRESS 0x062F0Cu
#define unk_062F0C RAM16(unk_062F0C_ADDRESS)
#define unk_062F0E_ADDRESS 0x062F0Eu
#define unk_062F0E RAM16(unk_062F0E_ADDRESS)
#define unk_062F10_ADDRESS 0x062F10u
#define unk_062F10 RAM16(unk_062F10_ADDRESS)
#define unk_062F12_ADDRESS 0x062F12u
#define unk_062F12 RAM32(unk_062F12_ADDRESS)
#define unk_062F16_ADDRESS 0x062F16u
#define unk_062F16 RAM32(unk_062F16_ADDRESS)
#define unk_062F32_ADDRESS 0x062F32u
#define unk_062F32 RAM8(unk_062F32_ADDRESS)
#define unk_062F33_ADDRESS 0x062F33u
#define unk_062F33 RAM8(unk_062F33_ADDRESS)
#define unk_062F42_ADDRESS 0x062F42u
#define unk_062F42 RAM16(unk_062F42_ADDRESS)
#define unk_062F50_ADDRESS 0x062F50u
#define unk_062F50 RAM8(unk_062F50_ADDRESS)
#define unk_062FA0_ADDRESS 0x062FA0u
#define unk_062FA0 RAM32(unk_062FA0_ADDRESS)
#define unk_062FA4_ADDRESS 0x062FA4u
#define unk_062FA4 RAM32(unk_062FA4_ADDRESS)
#define unk_062FA8_ADDRESS 0x062FA8u
#define unk_062FA8 RAM32(unk_062FA8_ADDRESS)
#define unk_062FAC_ADDRESS 0x062FACu
#define unk_062FAC RAM32(unk_062FAC_ADDRESS)
#define unk_062FB0_ADDRESS 0x062FB0u
#define unk_062FB0 RAM32(unk_062FB0_ADDRESS)
#define unk_062FB4_ADDRESS 0x062FB4u
#define unk_062FB4 RAM32(unk_062FB4_ADDRESS)
#define unk_062FB8_ADDRESS 0x062FB8u
#define unk_062FB8 RAM32(unk_062FB8_ADDRESS)
#define unk_062FBC_ADDRESS 0x062FBCu
#define unk_062FBC RAM32(unk_062FBC_ADDRESS)
#define unk_062FC0_ADDRESS 0x062FC0u
#define unk_062FC0 RAM32(unk_062FC0_ADDRESS)
#define unk_062FC4_ADDRESS 0x062FC4u
#define unk_062FC4 RAM32(unk_062FC4_ADDRESS)
#define unk_062FC8_ADDRESS 0x062FC8u
#define unk_062FC8 RAM32(unk_062FC8_ADDRESS)
#define unk_062FCC_ADDRESS 0x062FCCu
#define unk_062FCC RAM32(unk_062FCC_ADDRESS)
#define unk_062FD0_ADDRESS 0x062FD0u
#define unk_062FD0 RAM32(unk_062FD0_ADDRESS)
#define unk_062FD4_ADDRESS 0x062FD4u
#define unk_062FD4 RAM32(unk_062FD4_ADDRESS)
#define unk_062FD8_ADDRESS 0x062FD8u
#define unk_062FD8 RAM32(unk_062FD8_ADDRESS)
#define unk_062FDC_ADDRESS 0x062FDCu
#define unk_062FDC RAM32(unk_062FDC_ADDRESS)
#define unk_063300_ADDRESS 0x063300u
#define unk_063300 RAM32(unk_063300_ADDRESS)
#define unk_063304_ADDRESS 0x063304u
#define unk_063304 RAM32(unk_063304_ADDRESS)
#define unk_063308_ADDRESS 0x063308u
#define unk_063308 RAM16(unk_063308_ADDRESS)
#define unk_06330A_ADDRESS 0x06330Au
#define unk_06330A RAM16(unk_06330A_ADDRESS)
#define unk_06330C_ADDRESS 0x06330Cu
#define unk_06330C RAM16(unk_06330C_ADDRESS)
#define unk_06330E_ADDRESS 0x06330Eu
#define unk_06330E RAM16(unk_06330E_ADDRESS)
#define unk_063310_ADDRESS 0x063310u
#define unk_063310 RAM16(unk_063310_ADDRESS)
#define unk_063312_ADDRESS 0x063312u
#define unk_063312 RAM32(unk_063312_ADDRESS)
#define unk_063316_ADDRESS 0x063316u
#define unk_063316 RAM32(unk_063316_ADDRESS)
#define unk_06331A_ADDRESS 0x06331Au
#define unk_06331A RAM8(unk_06331A_ADDRESS)
#define unk_06331B_ADDRESS 0x06331Bu
#define unk_06331B RAM8(unk_06331B_ADDRESS)
#define unk_06331C_ADDRESS 0x06331Cu
#define unk_06331C RAM32(unk_06331C_ADDRESS)
#define unk_063320_ADDRESS 0x063320u
#define unk_063320 RAM32(unk_063320_ADDRESS)
#define unk_063324_ADDRESS 0x063324u
#define unk_063324 RAM32(unk_063324_ADDRESS)
#define unk_063328_ADDRESS 0x063328u
#define unk_063328 RAM32(unk_063328_ADDRESS)
#define unk_06332C_ADDRESS 0x06332Cu
#define unk_06332C RAM32(unk_06332C_ADDRESS)
#define unk_063330_ADDRESS 0x063330u
#define unk_063330 RAM32(unk_063330_ADDRESS)
#define unk_063334_ADDRESS 0x063334u
#define unk_063334 RAM8(unk_063334_ADDRESS)
#define unk_063335_ADDRESS 0x063335u
#define unk_063335 RAM8(unk_063335_ADDRESS)
#define unk_063340_ADDRESS 0x063340u
#define unk_063340 RAM32(unk_063340_ADDRESS)
#define unk_063344_ADDRESS 0x063344u
#define unk_063344 RAM32(unk_063344_ADDRESS)
#define unk_063348_ADDRESS 0x063348u
#define unk_063348 RAM16(unk_063348_ADDRESS)
#define unk_06334A_ADDRESS 0x06334Au
#define unk_06334A RAM16(unk_06334A_ADDRESS)
#define unk_06334C_ADDRESS 0x06334Cu
#define unk_06334C RAM16(unk_06334C_ADDRESS)
#define unk_06334E_ADDRESS 0x06334Eu
#define unk_06334E RAM16(unk_06334E_ADDRESS)
#define unk_063350_ADDRESS 0x063350u
#define unk_063350 RAM16(unk_063350_ADDRESS)
#define unk_063352_ADDRESS 0x063352u
#define unk_063352 RAM32(unk_063352_ADDRESS)
#define unk_063356_ADDRESS 0x063356u
#define unk_063356 RAM32(unk_063356_ADDRESS)
#define unk_06335A_ADDRESS 0x06335Au
#define unk_06335A RAM8(unk_06335A_ADDRESS)
#define unk_06335B_ADDRESS 0x06335Bu
#define unk_06335B RAM8(unk_06335B_ADDRESS)
#define unk_06335C_ADDRESS 0x06335Cu
#define unk_06335C RAM32(unk_06335C_ADDRESS)
#define unk_063360_ADDRESS 0x063360u
#define unk_063360 RAM32(unk_063360_ADDRESS)
#define unk_063364_ADDRESS 0x063364u
#define unk_063364 RAM32(unk_063364_ADDRESS)
#define unk_063368_ADDRESS 0x063368u
#define unk_063368 RAM32(unk_063368_ADDRESS)
#define unk_06336C_ADDRESS 0x06336Cu
#define unk_06336C RAM32(unk_06336C_ADDRESS)
#define unk_063370_ADDRESS 0x063370u
#define unk_063370 RAM32(unk_063370_ADDRESS)
#define unk_063374_ADDRESS 0x063374u
#define unk_063374 RAM8(unk_063374_ADDRESS)
#define unk_063375_ADDRESS 0x063375u
#define unk_063375 RAM8(unk_063375_ADDRESS)
#define unk_063380_ADDRESS 0x063380u
#define unk_063380 RAM32(unk_063380_ADDRESS)
#define unk_063384_ADDRESS 0x063384u
#define unk_063384 RAM32(unk_063384_ADDRESS)
#define unk_063388_ADDRESS 0x063388u
#define unk_063388 RAM16(unk_063388_ADDRESS)
#define unk_06338A_ADDRESS 0x06338Au
#define unk_06338A RAM16(unk_06338A_ADDRESS)
#define unk_06338C_ADDRESS 0x06338Cu
#define unk_06338C RAM16(unk_06338C_ADDRESS)
#define unk_06338E_ADDRESS 0x06338Eu
#define unk_06338E RAM16(unk_06338E_ADDRESS)
#define unk_063390_ADDRESS 0x063390u
#define unk_063390 RAM16(unk_063390_ADDRESS)
#define unk_063392_ADDRESS 0x063392u
#define unk_063392 RAM32(unk_063392_ADDRESS)
#define unk_063396_ADDRESS 0x063396u
#define unk_063396 RAM32(unk_063396_ADDRESS)
#define unk_06339A_ADDRESS 0x06339Au
#define unk_06339A RAM8(unk_06339A_ADDRESS)
#define unk_06339B_ADDRESS 0x06339Bu
#define unk_06339B RAM8(unk_06339B_ADDRESS)
#define unk_06339C_ADDRESS 0x06339Cu
#define unk_06339C RAM32(unk_06339C_ADDRESS)
#define unk_0633A0_ADDRESS 0x0633A0u
#define unk_0633A0 RAM32(unk_0633A0_ADDRESS)
#define unk_0633A4_ADDRESS 0x0633A4u
#define unk_0633A4 RAM32(unk_0633A4_ADDRESS)
#define unk_0633A8_ADDRESS 0x0633A8u
#define unk_0633A8 RAM32(unk_0633A8_ADDRESS)
#define unk_0633AC_ADDRESS 0x0633ACu
#define unk_0633AC RAM32(unk_0633AC_ADDRESS)
#define unk_0633B0_ADDRESS 0x0633B0u
#define unk_0633B0 RAM32(unk_0633B0_ADDRESS)
#define unk_0633B4_ADDRESS 0x0633B4u
#define unk_0633B4 RAM8(unk_0633B4_ADDRESS)
#define unk_0633B5_ADDRESS 0x0633B5u
#define unk_0633B5 RAM8(unk_0633B5_ADDRESS)
#define scroll_segment_ADDRESS 0x0633C0u
#define scroll_segment RAM16(scroll_segment_ADDRESS)
#define scroll_held_ADDRESS 0x0633C2u
#define scroll_held RAM32(scroll_held_ADDRESS)
#define scroll_held_left_ADDRESS 0x0633C2u
#define scroll_held_left RAM8(scroll_held_left_ADDRESS)
#define scroll_held_right_ADDRESS 0x0633C3u
#define scroll_held_right RAM8(scroll_held_right_ADDRESS)
#define scroll_held_up_ADDRESS 0x0633C4u
#define scroll_held_up RAM8(scroll_held_up_ADDRESS)
#define scroll_held_down_ADDRESS 0x0633C5u
#define scroll_held_down RAM8(scroll_held_down_ADDRESS)
#define apartment_fire_spot_ADDRESS 0x0633C6u
#define apartment_fire_spot RAM8(apartment_fire_spot_ADDRESS)
#define apartment_fire_frame_ADDRESS 0x0633C7u
#define apartment_fire_frame RAM8(apartment_fire_frame_ADDRESS)
#define area_beaten_ADDRESS 0x0633C8u
#define area_beaten RAM8(area_beaten_ADDRESS)
#define scroll_mode_ADDRESS 0x0633C9u
#define scroll_mode RAM8S(scroll_mode_ADDRESS)
#define turtles_busy_ADDRESS 0x0633CAu
#define turtles_busy RAM8(turtles_busy_ADDRESS)
#define turtles_on_the_way_ADDRESS 0x0633CBu
#define turtles_on_the_way RAM8(turtles_on_the_way_ADDRESS)
#define working_layer_tiles_ADDRESS 0x0633CCu
#define working_layer_tiles RAM32(working_layer_tiles_ADDRESS)
#define layer_loops_ADDRESS 0x0633D0u
#define layer_loops RAM8S(layer_loops_ADDRESS)
#define layers_step_ADDRESS 0x0633D2u
#define layers_step RAM8(layers_step_ADDRESS)
#define floor_cell_ADDRESS 0x0633D3u
#define floor_cell RAM8(floor_cell_ADDRESS)
#define floor_cell_up_ADDRESS 0x0633D4u
#define floor_cell_up RAM8(floor_cell_up_ADDRESS)
#define floor_cell_right_ADDRESS 0x0633D5u
#define floor_cell_right RAM8(floor_cell_right_ADDRESS)
#define floor_cell_left_ADDRESS 0x0633D6u
#define floor_cell_left RAM8(floor_cell_left_ADDRESS)
#define floor_cell_up_right_ADDRESS 0x0633D7u
#define floor_cell_up_right RAM8(floor_cell_up_right_ADDRESS)
#define floor_cell_up_left_ADDRESS 0x0633D8u
#define floor_cell_up_left RAM8(floor_cell_up_left_ADDRESS)
#define height_cell_up_ADDRESS 0x0633D9u
#define height_cell_up RAM8(height_cell_up_ADDRESS)
#define height_cell_right_ADDRESS 0x0633DAu
#define height_cell_right RAM8(height_cell_right_ADDRESS)
#define height_cell_left_ADDRESS 0x0633DBu
#define height_cell_left RAM8(height_cell_left_ADDRESS)
#define height_cell_up_right_ADDRESS 0x0633DCu
#define height_cell_up_right RAM8(height_cell_up_right_ADDRESS)
#define height_cell_up_left_ADDRESS 0x0633DDu
#define height_cell_up_left RAM8(height_cell_up_left_ADDRESS)
#define saved_turtle_ADDRESS 0x0633DEu
#define saved_turtle RAM32(saved_turtle_ADDRESS)
#define cutscene_wait_ADDRESS 0x0633DEu
#define cutscene_wait RAM8(cutscene_wait_ADDRESS)
#define cutscene_time_ADDRESS 0x0633DEu
#define cutscene_time RAM16(cutscene_time_ADDRESS)
#define cutscene_count_ADDRESS 0x0633DFu
#define cutscene_count RAM8(cutscene_count_ADDRESS)
#define cutscene_counter_ADDRESS 0x0633E2u
#define cutscene_counter RAM8(cutscene_counter_ADDRESS)
#define hug_turtle_colours_ADDRESS 0x0633E6u
#define hug_turtle_colours(index) RAM8(hug_turtle_colours_ADDRESS + (index))
#define cutscene_step_ADDRESS 0x0633EAu
#define cutscene_step RAM8(cutscene_step_ADDRESS)
#define turtles_not_waiting_ADDRESS 0x0633EBu
#define turtles_not_waiting RAM8(turtles_not_waiting_ADDRESS)
#define last_segment_ADDRESS 0x0633ECu
#define last_segment RAM8(last_segment_ADDRESS)
#define building_clear_wait_ADDRESS 0x0633EDu
#define building_clear_wait RAM8(building_clear_wait_ADDRESS)
#define unused_area_start_byte_ADDRESS 0x0633F0u
#define unused_area_start_byte RAM8(unused_area_start_byte_ADDRESS)
#define fire_last_frame_ADDRESS 0x0633F1u
#define fire_last_frame RAM8(fire_last_frame_ADDRESS)
#define building_fire_spot_ADDRESS 0x0633F2u
#define building_fire_spot RAM8(building_fire_spot_ADDRESS)
#define building_fire_frame_ADDRESS 0x0633F3u
#define building_fire_frame RAM8(building_fire_frame_ADDRESS)
#define height_cell_ADDRESS 0x0633F4u
#define height_cell RAM8(height_cell_ADDRESS)
#define building_scroll_hold_ADDRESS 0x0633F5u
#define building_scroll_hold RAM8(building_scroll_hold_ADDRESS)
#define hurry_time_ADDRESS 0x0633F6u
#define hurry_time RAM16(hurry_time_ADDRESS)
#define balloon_place_ADDRESS 0x0633F8u
#define balloon_place RAM32(balloon_place_ADDRESS)
#define balloon_time_ADDRESS 0x0633FCu
#define balloon_time RAM8(balloon_time_ADDRESS)
#define floor_end_walled_ADDRESS 0x0633FDu
#define floor_end_walled RAM8(floor_end_walled_ADDRESS)
#define drill_arrival_ADDRESS 0x0633FEu
#define drill_arrival RAM8S(drill_arrival_ADDRESS)
#define drill_arrival_step_ADDRESS 0x0633FFu
#define drill_arrival_step RAM8(drill_arrival_step_ADDRESS)
#define palette_editor_numbers_ADDRESS 0x063400u
#define palette_editor_numbers(index) RAM8(palette_editor_numbers_ADDRESS + (index))
#define palette_editor_pick_ADDRESS 0x063404u
#define palette_editor_pick RAM8(palette_editor_pick_ADDRESS)
#define palette_editor_last_pick_ADDRESS 0x063405u
#define palette_editor_last_pick RAM8(palette_editor_last_pick_ADDRESS)
#define palette_editor_part_ADDRESS 0x063407u
#define palette_editor_part RAM8(palette_editor_part_ADDRESS)
#define palette_editor_step_ADDRESS 0x063408u
#define palette_editor_step RAM8(palette_editor_step_ADDRESS)
#define fade_state_ADDRESS 0x063410u
#define fade_state RAM32(fade_state_ADDRESS)
#define fading_ADDRESS 0x063410u
#define fading RAM8(fading_ADDRESS)
#define fade_pass_step_ADDRESS 0x063411u
#define fade_pass_step RAM8(fade_pass_step_ADDRESS)
#define fade_part_ADDRESS 0x063412u
#define fade_part RAM8(fade_part_ADDRESS)
#define fade_lists_left_ADDRESS 0x063413u
#define fade_lists_left RAM8(fade_lists_left_ADDRESS)
#define fade_target_ADDRESS 0x063414u
#define fade_target RAM32(fade_target_ADDRESS)
#define fade_part_mask_ADDRESS 0x063418u
#define fade_part_mask RAM8(fade_part_mask_ADDRESS)
#define fade_rate_ADDRESS 0x063419u
#define fade_rate RAM8(fade_rate_ADDRESS)
#define fade_buffer_place_ADDRESS 0x06341Au
#define fade_buffer_place RAM32(fade_buffer_place_ADDRESS)
#define typed_text_place_ADDRESS 0x063420u
#define typed_text_place RAM32(typed_text_place_ADDRESS)
#define typed_text_ADDRESS 0x063424u
#define typed_text RAM32(typed_text_ADDRESS)
#define typed_text_wait_ADDRESS 0x063428u
#define typed_text_wait RAM8(typed_text_wait_ADDRESS)
#define typed_text_delay_ADDRESS 0x063429u
#define typed_text_delay RAM8(typed_text_delay_ADDRESS)
#define unused_typed_text_flag_ADDRESS 0x06342Au
#define unused_typed_text_flag RAM8(unused_typed_text_flag_ADDRESS)
#define typed_text_colour_ADDRESS 0x06342Bu
#define typed_text_colour RAM8(typed_text_colour_ADDRESS)
#define staff_roll_credit_ADDRESS 0x06342Eu
#define staff_roll_credit RAM8(staff_roll_credit_ADDRESS)
#define typed_passes_left_ADDRESS 0x06342Fu
#define typed_passes_left RAM8(typed_passes_left_ADDRESS)
#define unk_063434_ADDRESS 0x063434u
#define unk_063434 RAM32(unk_063434_ADDRESS)
#define technodrome_explosions_ADDRESS 0x063480u
#define technodrome_explosions RAM8(technodrome_explosions_ADDRESS)
#define drill_rumbling_ADDRESS 0x063488u
#define drill_rumbling RAM8(drill_rumbling_ADDRESS)
#define box_place_ADDRESS 0x063490u
#define box_place RAM32(box_place_ADDRESS)
#define box_width_ADDRESS 0x063494u
#define box_width RAM8(box_width_ADDRESS)
#define box_height_ADDRESS 0x063495u
#define box_height RAM8(box_height_ADDRESS)
#define box_tile_ADDRESS 0x063496u
#define box_tile RAM8(box_tile_ADDRESS)
#define box_frames_left_ADDRESS 0x063497u
#define box_frames_left RAM8(box_frames_left_ADDRESS)
#define name_entered_ADDRESS 0x0634A0u
#define name_entered RAM8(name_entered_ADDRESS)
#define ranking_scores_ADDRESS 0x063500u
#define ranking_scores(index) RAM16(ranking_scores_ADDRESS + (index) * 2u)
#define mask_rom_bank_ADDRESS 0x06350Eu
#define mask_rom_bank RAM8(mask_rom_bank_ADDRESS)
#define mask_rom_bits_ADDRESS 0x06350Fu
#define mask_rom_bits RAM8(mask_rom_bits_ADDRESS)
#define mask_rom_part_sums_ADDRESS 0x063510u
#define mask_rom_part_sums(index) RAM16(mask_rom_part_sums_ADDRESS + (index) * 2u)
#define mask_rom_first_bank_ADDRESS 0x063518u
#define mask_rom_first_bank RAM8(mask_rom_first_bank_ADDRESS)
#define mask_rom_last_bank_ADDRESS 0x063519u
#define mask_rom_last_bank RAM8(mask_rom_last_bank_ADDRESS)
#define ranking_new_score_ADDRESS 0x0635C6u
#define ranking_new_score RAM16(ranking_new_score_ADDRESS)
#define ranking_initials_ADDRESS 0x0635C8u
#define ranking_initials(index) RAM8(ranking_initials_ADDRESS + (index))
#define ranking_new_initials_ADDRESS 0x0635F0u
#define ranking_new_initials RAM32(ranking_new_initials_ADDRESS)
#define ranking_places_ADDRESS 0x0635F4u
#define ranking_places(index) RAM8(ranking_places_ADDRESS + (index))
#define ranking_time_ADDRESS 0x0635F8u
#define ranking_time RAM16(ranking_time_ADDRESS)
#define turtles_in_black_ADDRESS 0x0635F8u
#define turtles_in_black RAM8(turtles_in_black_ADDRESS)
#define initials_entered_ADDRESS 0x0635FAu
#define initials_entered(index) RAM8(initials_entered_ADDRESS + (index))
#define players_entering_names_ADDRESS 0x0635FEu
#define players_entering_names RAM8(players_entering_names_ADDRESS)
#define initials_buttons_held_ADDRESS 0x0635FFu
#define initials_buttons_held RAM8(initials_buttons_held_ADDRESS)

/* the program's data the code reads by name, which the game loads from its assets */
#define indexed_table_of_32_bit_values_0013DE_ADDRESS 0x0013DEu
extern const void *indexed_table_of_32_bit_values_0013DE_data;
#define indexed_table_of_32_bit_values_0013DE ((const uint32_t *)indexed_table_of_32_bit_values_0013DE_data)
#define lives_settings_ADDRESS 0x001426u
extern const void *lives_settings_data;
#define lives_settings ((const uint8_t *)lives_settings_data)
#define coinage_coins_ADDRESS 0x001452u
extern const void *coinage_coins_data;
#define coinage_coins ((const uint8_t *)coinage_coins_data)
#define coinage_credits_ADDRESS 0x001453u
extern const void *coinage_credits_data;
#define coinage_credits ((const uint8_t *)coinage_credits_data)
#define coinage_2_coins_ADDRESS 0x001472u
extern const void *coinage_2_coins_data;
#define coinage_2_coins ((const uint8_t *)coinage_2_coins_data)
#define coinage_2_credits_ADDRESS 0x001473u
extern const void *coinage_2_credits_data;
#define coinage_2_credits ((const uint8_t *)coinage_2_credits_data)
#define boot_texts_ADDRESS 0x0015ACu
extern const void *boot_texts_data;
#define boot_texts ((const uint32_t *)boot_texts_data)
#define text_colours_ADDRESS 0x001F1Au
extern const void *text_colours_data;
#define text_colours ((const uint8_t *)text_colours_data)
#define indexed_table_of_32_bit_values_002678_ADDRESS 0x002678u
extern const void *indexed_table_of_32_bit_values_002678_data;
#define indexed_table_of_32_bit_values_002678 ((const uint32_t *)indexed_table_of_32_bit_values_002678_data)
#define text_layer_places_ADDRESS 0x0027A2u
extern const void *text_layer_places_data;
#define text_layer_places ((const uint32_t *)text_layer_places_data)
#define big_font_place_ADDRESS 0x002802u
extern const void *big_font_place_data;
#define big_font_place (*(const uint32_t *)big_font_place_data)
#define big_font_tiles_ADDRESS 0x002806u
extern const void *big_font_tiles_data;
#define big_font_tiles ((const uint8_t *)big_font_tiles_data)
#define big_font_tiles_table_ADDRESS 0x002884u
extern const void *big_font_tiles_table_data;
#define big_font_tiles_table ((const uint8_t *)big_font_tiles_table_data)
#define colour_texts_ADDRESS 0x002F36u
extern const void *colour_texts_data;
#define colour_texts ((const uint32_t *)colour_texts_data)
#define tile_pictures_ADDRESS 0x0031F2u
extern const void *tile_pictures_data;
#define tile_pictures ((const uint32_t *)tile_pictures_data)
#define player_box_places_ADDRESS 0x0032C2u
extern const void *player_box_places_data;
#define player_box_places ((const uint32_t *)player_box_places_data)
#define box_labels_ADDRESS 0x00332Au
extern const void *box_labels_data;
#define box_labels (*(const uint32_t *)box_labels_data)
#define lives_digit_top_ADDRESS 0x0033A2u
extern const void *lives_digit_top_data;
#define lives_digit_top ((const uint8_t *)lives_digit_top_data)
#define lives_digit_bottom_ADDRESS 0x0033A3u
extern const void *lives_digit_bottom_data;
#define lives_digit_bottom ((const uint8_t *)lives_digit_bottom_data)
#define lives_icon_top_ADDRESS 0x0033B6u
extern const void *lives_icon_top_data;
#define lives_icon_top ((const uint8_t *)lives_icon_top_data)
#define lives_icon_bottom_ADDRESS 0x0033B7u
extern const void *lives_icon_bottom_data;
#define lives_icon_bottom ((const uint8_t *)lives_icon_bottom_data)
#define stage_titles_ADDRESS 0x003402u
extern const void *stage_titles_data;
#define stage_titles ((const uint32_t *)stage_titles_data)
#define positioned_texts_ADDRESS 0x0035ACu
extern const void *positioned_texts_data;
#define positioned_texts ((const uint32_t *)positioned_texts_data)
#define tall_texts_ADDRESS 0x00375Cu
extern const void *tall_texts_data;
#define tall_texts ((const uint32_t *)tall_texts_data)
#define indexed_table_of_32_bit_values_003E60_ADDRESS 0x003E60u
extern const void *indexed_table_of_32_bit_values_003E60_data;
#define indexed_table_of_32_bit_values_003E60 ((const uint32_t *)indexed_table_of_32_bit_values_003E60_data)
#define indexed_table_of_32_bit_values_003F8E_ADDRESS 0x003F8Eu
extern const void *indexed_table_of_32_bit_values_003F8E_data;
#define indexed_table_of_32_bit_values_003F8E ((const uint32_t *)indexed_table_of_32_bit_values_003F8E_data)
#define indexed_table_of_32_bit_values_004100_ADDRESS 0x004100u
extern const void *indexed_table_of_32_bit_values_004100_data;
#define indexed_table_of_32_bit_values_004100 ((const uint32_t *)indexed_table_of_32_bit_values_004100_data)
#define indexed_table_of_32_bit_values_00413A_ADDRESS 0x00413Au
extern const void *indexed_table_of_32_bit_values_00413A_data;
#define indexed_table_of_32_bit_values_00413A ((const uint32_t *)indexed_table_of_32_bit_values_00413A_data)
#define demo_settings_table_ADDRESS 0x004E88u
extern const void *demo_settings_table_data;
#define demo_settings_table ((const uint8_t *)demo_settings_table_data)
#define demo_input_streams_ADDRESS 0x004E94u
extern const void *demo_input_streams_data;
#define demo_input_streams ((const uint32_t *)demo_input_streams_data)
#define color_test_places_ADDRESS 0x00584Cu
extern const void *color_test_places_data;
#define color_test_places ((const uint32_t *)color_test_places_data)
#define player_input_ports_ADDRESS 0x0059A6u
extern const void *player_input_ports_data;
#define player_input_ports ((const uint32_t *)player_input_ports_data)
#define player_input_places_ADDRESS 0x0059B6u
extern const void *player_input_places_data;
#define player_input_places ((const uint32_t *)player_input_places_data)
#define coin_input_port_ADDRESS 0x0059C6u
extern const void *coin_input_port_data;
#define coin_input_port (*(const uint32_t *)coin_input_port_data)
#define coin_input_places_ADDRESS 0x0059CAu
extern const void *coin_input_places_data;
#define coin_input_places ((const uint32_t *)coin_input_places_data)
#define dip_switch_ports_ADDRESS 0x0059D2u
extern const void *dip_switch_ports_data;
#define dip_switch_ports ((const uint32_t *)dip_switch_ports_data)
#define dip_switch_places_ADDRESS 0x0059DEu
extern const void *dip_switch_places_data;
#define dip_switch_places ((const uint32_t *)dip_switch_places_data)
#define coin_counter_places_ADDRESS 0x005B40u
extern const void *coin_counter_places_data;
#define coin_counter_places ((const uint32_t *)coin_counter_places_data)
#define sound_test_codes_ADDRESS 0x006146u
extern const void *sound_test_codes_data;
#define sound_test_codes ((const uint8_t *)sound_test_codes_data)
#define area_continue_scenes_ADDRESS 0x00634Cu
extern const void *area_continue_scenes_data;
#define area_continue_scenes ((const uint8_t *)area_continue_scenes_data)
#define area_stages_ADDRESS 0x00634Du
extern const void *area_stages_data;
#define area_stages ((const uint8_t *)area_stages_data)
#define area_tile_banks_table_ADDRESS 0x00676Eu
extern const void *area_tile_banks_table_data;
#define area_tile_banks_table ((const uint8_t *)area_tile_banks_table_data)
#define area_layer_priorities_ADDRESS 0x006782u
extern const void *area_layer_priorities_data;
#define area_layer_priorities ((const uint8_t *)area_layer_priorities_data)
#define area_entry_without_blink_ADDRESS 0x00678Cu
extern const void *area_entry_without_blink_data;
#define area_entry_without_blink ((const uint8_t *)area_entry_without_blink_data)
#define fire_priority_flicker_ADDRESS 0x006A18u
extern const void *fire_priority_flicker_data;
#define fire_priority_flicker ((const uint8_t *)fire_priority_flicker_data)
#define area_music_ADDRESS 0x006B0Cu
extern const void *area_music_data;
#define area_music (*(const uint8_t *)area_music_data)
#define technodrome_explosion_sounds_ADDRESS 0x006DFCu
extern const void *technodrome_explosion_sounds_data;
#define technodrome_explosion_sounds ((const uint8_t *)technodrome_explosion_sounds_data)
#define rom_0071EE_ADDRESS 0x0071EEu
extern const void *rom_0071EE_data;
#define rom_0071EE (*(const uint16_t *)rom_0071EE_data)
#define rom_0071F0_ADDRESS 0x0071F0u
extern const void *rom_0071F0_data;
#define rom_0071F0 (*(const uint32_t *)rom_0071F0_data)
#define rom_0071F4_ADDRESS 0x0071F4u
extern const void *rom_0071F4_data;
#define rom_0071F4 (*(const uint16_t *)rom_0071F4_data)
#define rom_0071F6_ADDRESS 0x0071F6u
extern const void *rom_0071F6_data;
#define rom_0071F6 (*(const uint32_t *)rom_0071F6_data)
#define technodrome_explosion_x_ADDRESS 0x0072E2u
extern const void *technodrome_explosion_x_data;
#define technodrome_explosion_x ((const int16_t *)technodrome_explosion_x_data)
#define technodrome_explosion_y_ADDRESS 0x0072E4u
extern const void *technodrome_explosion_y_data;
#define technodrome_explosion_y ((const int16_t *)technodrome_explosion_y_data)
#define staff_roll_portrait_places_ADDRESS 0x00738Cu
extern const void *staff_roll_portrait_places_data;
#define staff_roll_portrait_places (*(const int16_t *)staff_roll_portrait_places_data)
#define credit_name_tiles_1_ADDRESS 0x007592u
extern const void *credit_name_tiles_1_data;
#define credit_name_tiles_1 ((const uint8_t *)credit_name_tiles_1_data)
#define credit_name_tiles_2_ADDRESS 0x007593u
extern const void *credit_name_tiles_2_data;
#define credit_name_tiles_2 ((const uint8_t *)credit_name_tiles_2_data)
#define credit_name_tiles_3_ADDRESS 0x007594u
extern const void *credit_name_tiles_3_data;
#define credit_name_tiles_3 ((const uint8_t *)credit_name_tiles_3_data)
#define credit_name_tiles_4_ADDRESS 0x007595u
extern const void *credit_name_tiles_4_data;
#define credit_name_tiles_4 ((const uint8_t *)credit_name_tiles_4_data)
#define ranking_background_tiles_ADDRESS 0x00807Eu
extern const void *ranking_background_tiles_data;
#define ranking_background_tiles (*(const uint8_t *)ranking_background_tiles_data)
#define ranking_frame_places_ADDRESS 0x0083DAu
extern const void *ranking_frame_places_data;
#define ranking_frame_places ((const uint32_t *)ranking_frame_places_data)
#define ranking_label_places_ADDRESS 0x0083EAu
extern const void *ranking_label_places_data;
#define ranking_label_places ((const uint32_t *)ranking_label_places_data)
#define ranking_place_places_ADDRESS 0x0083FAu
extern const void *ranking_place_places_data;
#define ranking_place_places ((const uint32_t *)ranking_place_places_data)
#define ranking_initials_places_ADDRESS 0x00840Au
extern const void *ranking_initials_places_data;
#define ranking_initials_places ((const uint32_t *)ranking_initials_places_data)
#define ranking_place_tiles_1_ADDRESS 0x008436u
extern const void *ranking_place_tiles_1_data;
#define ranking_place_tiles_1 ((const uint8_t *)ranking_place_tiles_1_data)
#define ranking_place_tiles_2_ADDRESS 0x008437u
extern const void *ranking_place_tiles_2_data;
#define ranking_place_tiles_2 ((const uint8_t *)ranking_place_tiles_2_data)
#define turtle_picture_sprites_ADDRESS 0x008BEAu
extern const void *turtle_picture_sprites_data;
#define turtle_picture_sprites (*(const uint8_t *)turtle_picture_sprites_data)
#define ranking_turtle_picture_sprites_ADDRESS 0x008C2Au
extern const void *ranking_turtle_picture_sprites_data;
#define ranking_turtle_picture_sprites (*(const uint8_t *)ranking_turtle_picture_sprites_data)
#define best_players_places_ADDRESS 0x00917Eu
extern const void *best_players_places_data;
#define best_players_places ((const uint32_t *)best_players_places_data)
#define best_players_colours_ADDRESS 0x009186u
extern const void *best_players_colours_data;
#define best_players_colours ((const uint8_t *)best_players_colours_data)
#define indexed_table_of_32_bit_values_009310_ADDRESS 0x009310u
extern const void *indexed_table_of_32_bit_values_009310_data;
#define indexed_table_of_32_bit_values_009310 ((const uint32_t *)indexed_table_of_32_bit_values_009310_data)
#define rom_009334_ADDRESS 0x009334u
extern const void *rom_009334_data;
#define rom_009334 ((const uint32_t *)rom_009334_data)
#define palette_bank_counts_ADDRESS 0x0095D2u
extern const void *palette_bank_counts_data;
#define palette_bank_counts ((const uint8_t *)palette_bank_counts_data)
#define palette_bank_addresses_ADDRESS 0x0095D6u
extern const void *palette_bank_addresses_data;
#define palette_bank_addresses ((const uint32_t *)palette_bank_addresses_data)
#define palette_banks_ADDRESS 0x00975Cu
extern const void *palette_banks_data;
#define palette_banks ((const uint32_t *)palette_banks_data)
#define palette_effect_banks_ADDRESS 0x0098F2u
extern const void *palette_effect_banks_data;
#define palette_effect_banks ((const uint32_t *)palette_effect_banks_data)
#define indexed_table_of_32_bit_values_0099F0_ADDRESS 0x0099F0u
extern const void *indexed_table_of_32_bit_values_0099F0_data;
#define indexed_table_of_32_bit_values_0099F0 ((const uint32_t *)indexed_table_of_32_bit_values_0099F0_data)
#define game_step_palettes_ADDRESS 0x009A7Cu
extern const void *game_step_palettes_data;
#define game_step_palettes ((const uint32_t *)game_step_palettes_data)
#define game_step_palette_effects_ADDRESS 0x009AD2u
extern const void *game_step_palette_effects_data;
#define game_step_palette_effects ((const uint32_t *)game_step_palette_effects_data)
#define area_palette_effects_ADDRESS 0x009B28u
extern const void *area_palette_effects_data;
#define area_palette_effects ((const uint32_t *)area_palette_effects_data)
#define indexed_table_of_32_bit_values_009C30_ADDRESS 0x009C30u
extern const void *indexed_table_of_32_bit_values_009C30_data;
#define indexed_table_of_32_bit_values_009C30 ((const uint32_t *)indexed_table_of_32_bit_values_009C30_data)
#define palette_editor_limits_ADDRESS 0x009D40u
extern const void *palette_editor_limits_data;
#define palette_editor_limits ((const uint8_t *)palette_editor_limits_data)
#define palette_editor_banks_ADDRESS 0x009EFAu
extern const void *palette_editor_banks_data;
#define palette_editor_banks ((const uint32_t *)palette_editor_banks_data)
#define palette_editor_masks_ADDRESS 0x009F0Au
extern const void *palette_editor_masks_data;
#define palette_editor_masks ((const uint16_t *)palette_editor_masks_data)
#define palette_editor_steps_ADDRESS 0x009F0Cu
extern const void *palette_editor_steps_data;
#define palette_editor_steps ((const uint16_t *)palette_editor_steps_data)
#define palette_editor_parts_ADDRESS 0x009F16u
extern const void *palette_editor_parts_data;
#define palette_editor_parts ((const uint8_t *)palette_editor_parts_data)
#define palette_editor_place_ADDRESS 0x009F1Au
extern const void *palette_editor_place_data;
#define palette_editor_place (*(const uint32_t *)palette_editor_place_data)
#define palette_editor_title_ADDRESS 0x009F1Eu
extern const void *palette_editor_title_data;
#define palette_editor_title ((const uint8_t *)palette_editor_title_data)
#define palette_editor_number_places_ADDRESS 0x009F22u
extern const void *palette_editor_number_places_data;
#define palette_editor_number_places ((const uint32_t *)palette_editor_number_places_data)
#define palette_editor_colour_places_ADDRESS 0x009F2Eu
extern const void *palette_editor_colour_places_data;
#define palette_editor_colour_places ((const uint32_t *)palette_editor_colour_places_data)
#define hex_digit_tiles_ADDRESS 0x009F46u
extern const void *hex_digit_tiles_data;
#define hex_digit_tiles ((const uint8_t *)hex_digit_tiles_data)
#define area_palettes_ADDRESS 0x009F56u
extern const void *area_palettes_data;
#define area_palettes ((const uint32_t *)area_palettes_data)
#define indexed_table_of_32_bit_values_00DF92_ADDRESS 0x00DF92u
extern const void *indexed_table_of_32_bit_values_00DF92_data;
#define indexed_table_of_32_bit_values_00DF92 ((const uint32_t *)indexed_table_of_32_bit_values_00DF92_data)
#define indexed_table_of_32_bit_values_00E082_ADDRESS 0x00E082u
extern const void *indexed_table_of_32_bit_values_00E082_data;
#define indexed_table_of_32_bit_values_00E082 ((const uint32_t *)indexed_table_of_32_bit_values_00E082_data)
#define indexed_table_of_32_bit_values_00E13C_ADDRESS 0x00E13Cu
extern const void *indexed_table_of_32_bit_values_00E13C_data;
#define indexed_table_of_32_bit_values_00E13C ((const uint32_t *)indexed_table_of_32_bit_values_00E13C_data)
#define indexed_table_of_32_bit_values_00E1C0_ADDRESS 0x00E1C0u
extern const void *indexed_table_of_32_bit_values_00E1C0_data;
#define indexed_table_of_32_bit_values_00E1C0 ((const uint32_t *)indexed_table_of_32_bit_values_00E1C0_data)
#define indexed_table_of_32_bit_values_00E2F0_ADDRESS 0x00E2F0u
extern const void *indexed_table_of_32_bit_values_00E2F0_data;
#define indexed_table_of_32_bit_values_00E2F0 ((const uint32_t *)indexed_table_of_32_bit_values_00E2F0_data)
#define indexed_table_of_32_bit_values_00E3B0_ADDRESS 0x00E3B0u
extern const void *indexed_table_of_32_bit_values_00E3B0_data;
#define indexed_table_of_32_bit_values_00E3B0 ((const uint32_t *)indexed_table_of_32_bit_values_00E3B0_data)
#define krang_hover_heights_ADDRESS 0x00E48Au
extern const void *krang_hover_heights_data;
#define krang_hover_heights ((const uint16_t *)krang_hover_heights_data)
#define krang_move_chances_ADDRESS 0x00E4FEu
extern const void *krang_move_chances_data;
#define krang_move_chances (*(const uint8_t *)krang_move_chances_data)
#define indexed_table_of_32_bit_values_00E546_ADDRESS 0x00E546u
extern const void *indexed_table_of_32_bit_values_00E546_data;
#define indexed_table_of_32_bit_values_00E546 ((const uint32_t *)indexed_table_of_32_bit_values_00E546_data)
#define indexed_table_of_32_bit_values_00E582_ADDRESS 0x00E582u
extern const void *indexed_table_of_32_bit_values_00E582_data;
#define indexed_table_of_32_bit_values_00E582 ((const uint32_t *)indexed_table_of_32_bit_values_00E582_data)
#define krang_move_times_ADDRESS 0x00E700u
extern const void *krang_move_times_data;
#define krang_move_times ((const uint8_t *)krang_move_times_data)
#define krang_laser_place_x_ADDRESS 0x00EA9Eu
extern const void *krang_laser_place_x_data;
#define krang_laser_place_x ((const int16_t *)krang_laser_place_x_data)
#define krang_laser_place_height_ADDRESS 0x00EAA0u
extern const void *krang_laser_place_height_data;
#define krang_laser_place_height ((const int16_t *)krang_laser_place_height_data)
#define krang_laser_x_speeds_ADDRESS 0x00EB1Eu
extern const void *krang_laser_x_speeds_data;
#define krang_laser_x_speeds ((const int16_t *)krang_laser_x_speeds_data)
#define krang_laser_height_speeds_ADDRESS 0x00EB20u
extern const void *krang_laser_height_speeds_data;
#define krang_laser_height_speeds ((const int16_t *)krang_laser_height_speeds_data)
#define krang_animations_ADDRESS 0x00EC98u
extern const void *krang_animations_data;
#define krang_animations ((const uint32_t *)krang_animations_data)
#define krang_fist_animations_ADDRESS 0x00F674u
extern const void *krang_fist_animations_data;
#define krang_fist_animations ((const uint32_t *)krang_fist_animations_data)
#define krang_laser_animations_ADDRESS 0x00F760u
extern const void *krang_laser_animations_data;
#define krang_laser_animations ((const uint32_t *)krang_laser_animations_data)
#define ball_drop_waits_ADDRESS 0x00F938u
extern const void *ball_drop_waits_data;
#define ball_drop_waits ((const uint16_t *)ball_drop_waits_data)
#define rat_start_offsets_ADDRESS 0x00FF60u
extern const void *rat_start_offsets_data;
#define rat_start_offsets ((const uint32_t *)rat_start_offsets_data)
#define rat_run_times_ADDRESS 0x010012u
extern const void *rat_run_times_data;
#define rat_run_times ((const uint8_t *)rat_run_times_data)
#define pizza_stats_by_kind_ADDRESS 0x010148u
extern const void *pizza_stats_by_kind_data;
#define pizza_stats_by_kind ((const uint32_t *)pizza_stats_by_kind_data)
#define billboard_trigger_x_ADDRESS 0x010232u
extern const void *billboard_trigger_x_data;
#define billboard_trigger_x ((const uint16_t *)billboard_trigger_x_data)
#define indexed_table_of_32_bit_values_010616_ADDRESS 0x010616u
extern const void *indexed_table_of_32_bit_values_010616_data;
#define indexed_table_of_32_bit_values_010616 ((const uint32_t *)indexed_table_of_32_bit_values_010616_data)
#define turret_waits_ADDRESS 0x0107AEu
extern const void *turret_waits_data;
#define turret_waits ((const uint8_t *)turret_waits_data)
#define nozzle_row_routines_ADDRESS 0x010A0Eu
extern const void *nozzle_row_routines_data;
#define nozzle_row_routines ((const uint32_t *)nozzle_row_routines_data)
#define nozzle_rows_ADDRESS 0x010AB2u
extern const void *nozzle_rows_data;
#define nozzle_rows ((const uint32_t *)nozzle_rows_data)
#define indexed_table_of_32_bit_values_010B7E_ADDRESS 0x010B7Eu
extern const void *indexed_table_of_32_bit_values_010B7E_data;
#define indexed_table_of_32_bit_values_010B7E ((const uint32_t *)indexed_table_of_32_bit_values_010B7E_data)
#define turret_wreck_sounds_ADDRESS 0x010EAAu
extern const void *turret_wreck_sounds_data;
#define turret_wreck_sounds ((const uint8_t *)turret_wreck_sounds_data)
#define turret_wreck_waits_ADDRESS 0x010EAEu
extern const void *turret_wreck_waits_data;
#define turret_wreck_waits ((const uint16_t *)turret_wreck_waits_data)
#define boss_explosion_x_ADDRESS 0x010F1Cu
extern const void *boss_explosion_x_data;
#define boss_explosion_x ((const int16_t *)boss_explosion_x_data)
#define boss_explosion_height_ADDRESS 0x010F1Eu
extern const void *boss_explosion_height_data;
#define boss_explosion_height ((const int16_t *)boss_explosion_height_data)
#define explosion_sounds_ADDRESS 0x010F44u
extern const void *explosion_sounds_data;
#define explosion_sounds ((const uint8_t *)explosion_sounds_data)
#define stockman_explosion_x_ADDRESS 0x010F62u
extern const void *stockman_explosion_x_data;
#define stockman_explosion_x ((const int16_t *)stockman_explosion_x_data)
#define stockman_explosion_height_ADDRESS 0x010F64u
extern const void *stockman_explosion_height_data;
#define stockman_explosion_height ((const int16_t *)stockman_explosion_height_data)
#define sewer_launcher_drift_x_ADDRESS 0x0113BEu
extern const void *sewer_launcher_drift_x_data;
#define sewer_launcher_drift_x ((const int16_t *)sewer_launcher_drift_x_data)
#define sewer_launcher_drift_y_ADDRESS 0x0113C0u
extern const void *sewer_launcher_drift_y_data;
#define sewer_launcher_drift_y ((const int16_t *)sewer_launcher_drift_y_data)
#define indexed_table_of_32_bit_values_011562_ADDRESS 0x011562u
extern const void *indexed_table_of_32_bit_values_011562_data;
#define indexed_table_of_32_bit_values_011562 ((const uint32_t *)indexed_table_of_32_bit_values_011562_data)
#define rock_soldier_site_attack_reaches_ADDRESS 0x01175Au
extern const void *rock_soldier_site_attack_reaches_data;
#define rock_soldier_site_attack_reaches ((const uint8_t *)rock_soldier_site_attack_reaches_data)
#define rom_011782_ADDRESS 0x011782u
extern const void *rom_011782_data;
#define rom_011782 ((const uint32_t *)rom_011782_data)
#define indexed_table_of_32_bit_values_0119F2_ADDRESS 0x0119F2u
extern const void *indexed_table_of_32_bit_values_0119F2_data;
#define indexed_table_of_32_bit_values_0119F2 ((const uint32_t *)indexed_table_of_32_bit_values_0119F2_data)
#define rock_soldier_crumble_times_ADDRESS 0x011AD2u
extern const void *rock_soldier_crumble_times_data;
#define rock_soldier_crumble_times (*(const uint8_t *)rock_soldier_crumble_times_data)
#define rock_soldier_site_moves_ADDRESS 0x011D1Au
extern const void *rock_soldier_site_moves_data;
#define rock_soldier_site_moves ((const uint8_t *)rock_soldier_site_moves_data)
#define rock_soldier_site_waits_ADDRESS 0x011DEEu
extern const void *rock_soldier_site_waits_data;
#define rock_soldier_site_waits ((const uint8_t *)rock_soldier_site_waits_data)
#define rock_soldier_site_move_times_ADDRESS 0x011DF2u
extern const void *rock_soldier_site_move_times_data;
#define rock_soldier_site_move_times ((const uint8_t *)rock_soldier_site_move_times_data)
#define rubble_place_x_ADDRESS 0x012108u
extern const void *rubble_place_x_data;
#define rubble_place_x ((const int8_t *)rubble_place_x_data)
#define rubble_place_y_ADDRESS 0x012109u
extern const void *rubble_place_y_data;
#define rubble_place_y ((const int8_t *)rubble_place_y_data)
#define indexed_table_of_32_bit_values_0121AC_ADDRESS 0x0121ACu
extern const void *indexed_table_of_32_bit_values_0121AC_data;
#define indexed_table_of_32_bit_values_0121AC ((const uint32_t *)indexed_table_of_32_bit_values_0121AC_data)
#define rom_012360_ADDRESS 0x012360u
extern const void *rom_012360_data;
#define rom_012360 ((const uint32_t *)rom_012360_data)
#define rock_soldier_technodrome_moves_ADDRESS 0x01255Eu
extern const void *rock_soldier_technodrome_moves_data;
#define rock_soldier_technodrome_moves ((const uint8_t *)rock_soldier_technodrome_moves_data)
#define rock_soldier_technodrome_move_times_ADDRESS 0x012628u
extern const void *rock_soldier_technodrome_move_times_data;
#define rock_soldier_technodrome_move_times ((const uint8_t *)rock_soldier_technodrome_move_times_data)
#define car_soldier_stop_y_ADDRESS 0x012CFEu
extern const void *car_soldier_stop_y_data;
#define car_soldier_stop_y ((const uint16_t *)car_soldier_stop_y_data)
#define helicopter_drift_y_ADDRESS 0x013092u
extern const void *helicopter_drift_y_data;
#define helicopter_drift_y ((const int16_t *)helicopter_drift_y_data)
#define helicopter_shake_heights_ADDRESS 0x0130F4u
extern const void *helicopter_shake_heights_data;
#define helicopter_shake_heights ((const uint16_t *)helicopter_shake_heights_data)
#define helicopter_bob_heights_ADDRESS 0x0130FCu
extern const void *helicopter_bob_heights_data;
#define helicopter_bob_heights ((const uint16_t *)helicopter_bob_heights_data)
#define flying_drone_starts_ADDRESS 0x013486u
extern const void *flying_drone_starts_data;
#define flying_drone_starts ((const uint8_t *)flying_drone_starts_data)
#define indexed_table_of_32_bit_values_0134CA_ADDRESS 0x0134CAu
extern const void *indexed_table_of_32_bit_values_0134CA_data;
#define indexed_table_of_32_bit_values_0134CA ((const uint32_t *)indexed_table_of_32_bit_values_0134CA_data)
#define flying_drone_shake_heights_ADDRESS 0x0138ACu
extern const void *flying_drone_shake_heights_data;
#define flying_drone_shake_heights ((const uint16_t *)flying_drone_shake_heights_data)
#define flying_drone_bob_heights_ADDRESS 0x0138B4u
extern const void *flying_drone_bob_heights_data;
#define flying_drone_bob_heights ((const uint16_t *)flying_drone_bob_heights_data)
#define flying_drone_aims_ADDRESS 0x0139A2u
extern const void *flying_drone_aims_data;
#define flying_drone_aims ((const uint8_t *)flying_drone_aims_data)
#define flying_drone_waits_ADDRESS 0x0139D6u
extern const void *flying_drone_waits_data;
#define flying_drone_waits ((const uint8_t *)flying_drone_waits_data)
#define flying_drone_x_speeds_ADDRESS 0x0139DAu
extern const void *flying_drone_x_speeds_data;
#define flying_drone_x_speeds ((const int16_t *)flying_drone_x_speeds_data)
#define flying_drone_y_speeds_ADDRESS 0x0139DCu
extern const void *flying_drone_y_speeds_data;
#define flying_drone_y_speeds ((const int16_t *)flying_drone_y_speeds_data)
#define flying_drone_sound_waits_ADDRESS 0x013A74u
extern const void *flying_drone_sound_waits_data;
#define flying_drone_sound_waits ((const uint8_t *)flying_drone_sound_waits_data)
#define drone_shot_place_x_ADDRESS 0x013AE4u
extern const void *drone_shot_place_x_data;
#define drone_shot_place_x ((const int16_t *)drone_shot_place_x_data)
#define drone_shot_place_height_ADDRESS 0x013AE6u
extern const void *drone_shot_place_height_data;
#define drone_shot_place_height ((const int16_t *)drone_shot_place_height_data)
#define drone_shot_x_speeds_ADDRESS 0x013B72u
extern const void *drone_shot_x_speeds_data;
#define drone_shot_x_speeds ((const int16_t *)drone_shot_x_speeds_data)
#define drone_shot_y_speeds_ADDRESS 0x013B74u
extern const void *drone_shot_y_speeds_data;
#define drone_shot_y_speeds ((const int16_t *)drone_shot_y_speeds_data)
#define indexed_table_of_32_bit_values_013C46_ADDRESS 0x013C46u
extern const void *indexed_table_of_32_bit_values_013C46_data;
#define indexed_table_of_32_bit_values_013C46 ((const uint32_t *)indexed_table_of_32_bit_values_013C46_data)
#define stockman_fly_moves_ADDRESS 0x013E30u
extern const void *stockman_fly_moves_data;
#define stockman_fly_moves ((const uint8_t *)stockman_fly_moves_data)
#define indexed_table_of_32_bit_values_013F08_ADDRESS 0x013F08u
extern const void *indexed_table_of_32_bit_values_013F08_data;
#define indexed_table_of_32_bit_values_013F08 ((const uint32_t *)indexed_table_of_32_bit_values_013F08_data)
#define stockman_shake_heights_ADDRESS 0x01416Eu
extern const void *stockman_shake_heights_data;
#define stockman_shake_heights ((const uint16_t *)stockman_shake_heights_data)
#define stockman_height_steps_ADDRESS 0x014176u
extern const void *stockman_height_steps_data;
#define stockman_height_steps ((const uint16_t *)stockman_height_steps_data)
#define stockman_bob_heights_ADDRESS 0x0141B4u
extern const void *stockman_bob_heights_data;
#define stockman_bob_heights ((const uint16_t *)stockman_bob_heights_data)
#define stockman_drop_times_ADDRESS 0x01424Au
extern const void *stockman_drop_times_data;
#define stockman_drop_times ((const uint8_t *)stockman_drop_times_data)
#define stockman_x_speeds_ADDRESS 0x01426Au
extern const void *stockman_x_speeds_data;
#define stockman_x_speeds ((const int16_t *)stockman_x_speeds_data)
#define stockman_y_speeds_ADDRESS 0x01426Cu
extern const void *stockman_y_speeds_data;
#define stockman_y_speeds ((const int16_t *)stockman_y_speeds_data)
#define profile_animations_ADDRESS 0x0143D4u
extern const void *profile_animations_data;
#define profile_animations ((const uint32_t *)profile_animations_data)
#define hydrant_animations_ADDRESS 0x01443Eu
extern const void *hydrant_animations_data;
#define hydrant_animations ((const uint32_t *)hydrant_animations_data)
#define hydrant_water_animations_ADDRESS 0x0144E8u
extern const void *hydrant_water_animations_data;
#define hydrant_water_animations ((const uint32_t *)hydrant_water_animations_data)
#define rat_animations_ADDRESS 0x014598u
extern const void *rat_animations_data;
#define rat_animations ((const uint32_t *)rat_animations_data)
#define alligator_animations_ADDRESS 0x0145FCu
extern const void *alligator_animations_data;
#define alligator_animations ((const uint32_t *)alligator_animations_data)
#define skateboarder_animations_ADDRESS 0x01461Au
extern const void *skateboarder_animations_data;
#define skateboarder_animations ((const uint32_t *)skateboarder_animations_data)
#define pizza_animations_ADDRESS 0x014636u
extern const void *pizza_animations_data;
#define pizza_animations ((const uint32_t *)pizza_animations_data)
#define billboard_animations_ADDRESS 0x01480Eu
extern const void *billboard_animations_data;
#define billboard_animations ((const uint32_t *)billboard_animations_data)
#define billboard_soldier_animations_ADDRESS 0x014860u
extern const void *billboard_soldier_animations_data;
#define billboard_soldier_animations ((const uint32_t *)billboard_soldier_animations_data)
#define oil_drum_animations_ADDRESS 0x01488Au
extern const void *oil_drum_animations_data;
#define oil_drum_animations ((const uint32_t *)oil_drum_animations_data)
#define turret_animations_ADDRESS 0x014A26u
extern const void *turret_animations_data;
#define turret_animations ((const uint32_t *)turret_animations_data)
#define turret_shot_animations_ADDRESS 0x014AA2u
extern const void *turret_shot_animations_data;
#define turret_shot_animations ((const uint32_t *)turret_shot_animations_data)
#define freeze_nozzle_animations_ADDRESS 0x014AD2u
extern const void *freeze_nozzle_animations_data;
#define freeze_nozzle_animations ((const uint32_t *)freeze_nozzle_animations_data)
#define electric_beam_animations_ADDRESS 0x014BA0u
extern const void *electric_beam_animations_data;
#define electric_beam_animations ((const uint32_t *)electric_beam_animations_data)
#define explosion_small_animations_ADDRESS 0x014C3Au
extern const void *explosion_small_animations_data;
#define explosion_small_animations ((const uint32_t *)explosion_small_animations_data)
#define explosion_big_animations_ADDRESS 0x014C86u
extern const void *explosion_big_animations_data;
#define explosion_big_animations ((const uint32_t *)explosion_big_animations_data)
#define explosion_drum_animations_ADDRESS 0x014D1Cu
extern const void *explosion_drum_animations_data;
#define explosion_drum_animations ((const uint32_t *)explosion_drum_animations_data)
#define rock_soldier_site_animations_ADDRESS 0x014DA0u
extern const void *rock_soldier_site_animations_data;
#define rock_soldier_site_animations ((const uint32_t *)rock_soldier_site_animations_data)
#define gun_blast_animations_ADDRESS 0x014FBEu
extern const void *gun_blast_animations_data;
#define gun_blast_animations ((const uint32_t *)gun_blast_animations_data)
#define parked_car_animations_ADDRESS 0x015042u
extern const void *parked_car_animations_data;
#define parked_car_animations ((const uint32_t *)parked_car_animations_data)
#define electric_column_animations_ADDRESS 0x0150E8u
extern const void *electric_column_animations_data;
#define electric_column_animations ((const uint32_t *)electric_column_animations_data)
#define grille_animations_ADDRESS 0x01519Cu
extern const void *grille_animations_data;
#define grille_animations ((const uint32_t *)grille_animations_data)
#define rubble_animations_ADDRESS 0x0151AAu
extern const void *rubble_animations_data;
#define rubble_animations ((const uint32_t *)rubble_animations_data)
#define rocket_animations_ADDRESS 0x015258u
extern const void *rocket_animations_data;
#define rocket_animations ((const uint32_t *)rocket_animations_data)
#define helicopter_animations_ADDRESS 0x0152EEu
extern const void *helicopter_animations_data;
#define helicopter_animations ((const uint32_t *)helicopter_animations_data)
#define falling_missile_animations_ADDRESS 0x01534Au
extern const void *falling_missile_animations_data;
#define falling_missile_animations ((const uint32_t *)falling_missile_animations_data)
#define copter_bullet_animations_ADDRESS 0x015360u
extern const void *copter_bullet_animations_data;
#define copter_bullet_animations ((const uint32_t *)copter_bullet_animations_data)
#define rock_soldier_technodrome_animations_ADDRESS 0x0153D8u
extern const void *rock_soldier_technodrome_animations_data;
#define rock_soldier_technodrome_animations ((const uint32_t *)rock_soldier_technodrome_animations_data)
#define missile_animations_ADDRESS 0x015692u
extern const void *missile_animations_data;
#define missile_animations ((const uint32_t *)missile_animations_data)
#define wall_slab_animations_ADDRESS 0x0156DAu
extern const void *wall_slab_animations_data;
#define wall_slab_animations ((const uint32_t *)wall_slab_animations_data)
#define nozzle_row_animations_ADDRESS 0x0156F0u
extern const void *nozzle_row_animations_data;
#define nozzle_row_animations ((const uint32_t *)nozzle_row_animations_data)
#define waiting_soldier_animations_ADDRESS 0x0156FEu
extern const void *waiting_soldier_animations_data;
#define waiting_soldier_animations ((const uint32_t *)waiting_soldier_animations_data)
#define shock_bars_animations_ADDRESS 0x01574Eu
extern const void *shock_bars_animations_data;
#define shock_bars_animations ((const uint32_t *)shock_bars_animations_data)
#define staff_roll_portrait_animations_ADDRESS 0x015764u
extern const void *staff_roll_portrait_animations_data;
#define staff_roll_portrait_animations ((const uint32_t *)staff_roll_portrait_animations_data)
#define sewer_launcher_animations_ADDRESS 0x01579Cu
extern const void *sewer_launcher_animations_data;
#define sewer_launcher_animations ((const uint32_t *)sewer_launcher_animations_data)
#define spiked_pole_animations_ADDRESS 0x015838u
extern const void *spiked_pole_animations_data;
#define spiked_pole_animations ((const uint32_t *)spiked_pole_animations_data)
#define flying_drone_animations_ADDRESS 0x01584Eu
extern const void *flying_drone_animations_data;
#define flying_drone_animations ((const uint32_t *)flying_drone_animations_data)
#define drone_shot_animations_ADDRESS 0x015938u
extern const void *drone_shot_animations_data;
#define drone_shot_animations ((const uint32_t *)drone_shot_animations_data)
#define explosion_medium_animations_ADDRESS 0x015AA0u
extern const void *explosion_medium_animations_data;
#define explosion_medium_animations ((const uint32_t *)explosion_medium_animations_data)
#define one_way_sign_animations_ADDRESS 0x015B36u
extern const void *one_way_sign_animations_data;
#define one_way_sign_animations ((const uint32_t *)one_way_sign_animations_data)
#define stockman_animations_ADDRESS 0x015C70u
extern const void *stockman_animations_data;
#define stockman_animations ((const uint32_t *)stockman_animations_data)
#define falling_mouser_animations_ADDRESS 0x015D4Au
extern const void *falling_mouser_animations_data;
#define falling_mouser_animations ((const uint32_t *)falling_mouser_animations_data)
#define ball_animations_ADDRESS 0x015D68u
extern const void *ball_animations_data;
#define ball_animations ((const uint32_t *)ball_animations_data)
#define krang_curtain_animations_ADDRESS 0x015E02u
extern const void *krang_curtain_animations_data;
#define krang_curtain_animations ((const uint32_t *)krang_curtain_animations_data)
#define road_barrier_animations_ADDRESS 0x015EBEu
extern const void *road_barrier_animations_data;
#define road_barrier_animations ((const uint32_t *)road_barrier_animations_data)
#define tire_stack_animations_ADDRESS 0x015EEAu
extern const void *tire_stack_animations_data;
#define tire_stack_animations ((const uint32_t *)tire_stack_animations_data)
#define hex_digits_ADDRESS 0x0184C2u
extern const void *hex_digits_data;
#define hex_digits ((const uint8_t *)hex_digits_data)
#define boss_axis_directions_ADDRESS 0x018550u
extern const void *boss_axis_directions_data;
#define boss_axis_directions ((const uint8_t *)boss_axis_directions_data)
#define boss_slope_directions_ADDRESS 0x018560u
extern const void *boss_slope_directions_data;
#define boss_slope_directions ((const uint8_t *)boss_slope_directions_data)
#define box_opening_place_ADDRESS 0x01867Eu
extern const void *box_opening_place_data;
#define box_opening_place (*(const uint32_t *)box_opening_place_data)
#define unreached_box_opening_place_ADDRESS 0x018710u
extern const void *unreached_box_opening_place_data;
#define unreached_box_opening_place (*(const uint32_t *)unreached_box_opening_place_data)
#define profile_texts_ADDRESS 0x019272u
extern const void *profile_texts_data;
#define profile_texts ((const uint32_t *)profile_texts_data)
#define area_title_texts_ADDRESS 0x019866u
extern const void *area_title_texts_data;
#define area_title_texts ((const uint32_t *)area_title_texts_data)
#define indexed_table_of_32_bit_values_019D54_ADDRESS 0x019D54u
extern const void *indexed_table_of_32_bit_values_019D54_data;
#define indexed_table_of_32_bit_values_019D54 ((const uint32_t *)indexed_table_of_32_bit_values_019D54_data)
#define turtle_jump_in_x_ADDRESS 0x019EB2u
extern const void *turtle_jump_in_x_data;
#define turtle_jump_in_x ((const int16_t *)turtle_jump_in_x_data)
#define turtle_jump_in_y_ADDRESS 0x019EB4u
extern const void *turtle_jump_in_y_data;
#define turtle_jump_in_y ((const int16_t *)turtle_jump_in_y_data)
#define turtle_jump_in_speeds_ADDRESS 0x019EB6u
extern const void *turtle_jump_in_speeds_data;
#define turtle_jump_in_speeds ((const int16_t *)turtle_jump_in_speeds_data)
#define turtle_marker_animations_ADDRESS 0x019F24u
extern const void *turtle_marker_animations_data;
#define turtle_marker_animations ((const uint32_t *)turtle_marker_animations_data)
#define indexed_table_of_32_bit_values_019F92_ADDRESS 0x019F92u
extern const void *indexed_table_of_32_bit_values_019F92_data;
#define indexed_table_of_32_bit_values_019F92 ((const uint32_t *)indexed_table_of_32_bit_values_019F92_data)
#define indexed_table_of_32_bit_values_019FDE_ADDRESS 0x019FDEu
extern const void *indexed_table_of_32_bit_values_019FDE_data;
#define indexed_table_of_32_bit_values_019FDE ((const uint32_t *)indexed_table_of_32_bit_values_019FDE_data)
#define indexed_table_of_32_bit_values_01A14E_ADDRESS 0x01A14Eu
extern const void *indexed_table_of_32_bit_values_01A14E_data;
#define indexed_table_of_32_bit_values_01A14E ((const uint32_t *)indexed_table_of_32_bit_values_01A14E_data)
#define turtle_jump_in_heights_ADDRESS 0x01A2D2u
extern const void *turtle_jump_in_heights_data;
#define turtle_jump_in_heights ((const uint8_t *)turtle_jump_in_heights_data)
#define stage_scroll_locks_ADDRESS 0x01A3C0u
extern const void *stage_scroll_locks_data;
#define stage_scroll_locks ((const uint8_t *)stage_scroll_locks_data)
#define indexed_table_of_32_bit_values_01A88C_ADDRESS 0x01A88Cu
extern const void *indexed_table_of_32_bit_values_01A88C_data;
#define indexed_table_of_32_bit_values_01A88C ((const uint32_t *)indexed_table_of_32_bit_values_01A88C_data)
#define indexed_table_of_32_bit_values_01AC9A_ADDRESS 0x01AC9Au
extern const void *indexed_table_of_32_bit_values_01AC9A_data;
#define indexed_table_of_32_bit_values_01AC9A ((const uint32_t *)indexed_table_of_32_bit_values_01AC9A_data)
#define turtle_walk_speeds_ADDRESS 0x01B5B2u
extern const void *turtle_walk_speeds_data;
#define turtle_walk_speeds (*(const uint16_t *)turtle_walk_speeds_data)
#define indexed_table_of_32_bit_values_01B714_ADDRESS 0x01B714u
extern const void *indexed_table_of_32_bit_values_01B714_data;
#define indexed_table_of_32_bit_values_01B714 ((const uint32_t *)indexed_table_of_32_bit_values_01B714_data)
#define indexed_table_of_32_bit_values_01BA88_ADDRESS 0x01BA88u
extern const void *indexed_table_of_32_bit_values_01BA88_data;
#define indexed_table_of_32_bit_values_01BA88 ((const uint32_t *)indexed_table_of_32_bit_values_01BA88_data)
#define turtle_throw_moves_ADDRESS 0x01C072u
extern const void *turtle_throw_moves_data;
#define turtle_throw_moves ((const uint16_t *)turtle_throw_moves_data)
#define turtle_throw_counters_ADDRESS 0x01C074u
extern const void *turtle_throw_counters_data;
#define turtle_throw_counters ((const uint32_t *)turtle_throw_counters_data)
#define turtle_hit_animation_tables_ADDRESS 0x01C08Au
extern const void *turtle_hit_animation_tables_data;
#define turtle_hit_animation_tables ((const uint32_t *)turtle_hit_animation_tables_data)
#define turtle_attack_firsts_ADDRESS 0x01C20Eu
extern const void *turtle_attack_firsts_data;
#define turtle_attack_firsts ((const uint8_t *)turtle_attack_firsts_data)
#define indexed_table_of_32_bit_values_01C272_ADDRESS 0x01C272u
extern const void *indexed_table_of_32_bit_values_01C272_data;
#define indexed_table_of_32_bit_values_01C272 ((const uint32_t *)indexed_table_of_32_bit_values_01C272_data)
#define turtle_hit_speeds_ADDRESS 0x01C546u
extern const void *turtle_hit_speeds_data;
#define turtle_hit_speeds (*(const int16_t *)turtle_hit_speeds_data)
#define turtle_hit_waits_ADDRESS 0x01C596u
extern const void *turtle_hit_waits_data;
#define turtle_hit_waits ((const uint16_t *)turtle_hit_waits_data)
#define indexed_table_of_32_bit_values_01C678_ADDRESS 0x01C678u
extern const void *indexed_table_of_32_bit_values_01C678_data;
#define indexed_table_of_32_bit_values_01C678 ((const uint32_t *)indexed_table_of_32_bit_values_01C678_data)
#define turtle_knocked_down_steps_ADDRESS 0x01C764u
extern const void *turtle_knocked_down_steps_data;
#define turtle_knocked_down_steps ((const uint32_t *)turtle_knocked_down_steps_data)
#define indexed_table_of_32_bit_values_01C7D0_ADDRESS 0x01C7D0u
extern const void *indexed_table_of_32_bit_values_01C7D0_data;
#define indexed_table_of_32_bit_values_01C7D0 ((const uint32_t *)indexed_table_of_32_bit_values_01C7D0_data)
#define turtle_knocked_back_steps_ADDRESS 0x01C8CAu
extern const void *turtle_knocked_back_steps_data;
#define turtle_knocked_back_steps ((const uint32_t *)turtle_knocked_back_steps_data)
#define indexed_table_of_32_bit_values_01C8F8_ADDRESS 0x01C8F8u
extern const void *indexed_table_of_32_bit_values_01C8F8_data;
#define indexed_table_of_32_bit_values_01C8F8 ((const uint32_t *)indexed_table_of_32_bit_values_01C8F8_data)
#define turtle_knocked_flying_steps_ADDRESS 0x01CA82u
extern const void *turtle_knocked_flying_steps_data;
#define turtle_knocked_flying_steps ((const uint32_t *)turtle_knocked_flying_steps_data)
#define indexed_table_of_32_bit_values_01CAC2_ADDRESS 0x01CAC2u
extern const void *indexed_table_of_32_bit_values_01CAC2_data;
#define indexed_table_of_32_bit_values_01CAC2 ((const uint32_t *)indexed_table_of_32_bit_values_01CAC2_data)
#define turtle_knocked_flying_from_behind_steps_ADDRESS 0x01CC6Eu
extern const void *turtle_knocked_flying_from_behind_steps_data;
#define turtle_knocked_flying_from_behind_steps ((const uint32_t *)turtle_knocked_flying_from_behind_steps_data)
#define indexed_table_of_32_bit_values_01CCAA_ADDRESS 0x01CCAAu
extern const void *indexed_table_of_32_bit_values_01CCAA_data;
#define indexed_table_of_32_bit_values_01CCAA ((const uint32_t *)indexed_table_of_32_bit_values_01CCAA_data)
#define indexed_table_of_32_bit_values_01CD44_ADDRESS 0x01CD44u
extern const void *indexed_table_of_32_bit_values_01CD44_data;
#define indexed_table_of_32_bit_values_01CD44 ((const uint32_t *)indexed_table_of_32_bit_values_01CD44_data)
#define indexed_table_of_32_bit_values_01CDBC_ADDRESS 0x01CDBCu
extern const void *indexed_table_of_32_bit_values_01CDBC_data;
#define indexed_table_of_32_bit_values_01CDBC ((const uint32_t *)indexed_table_of_32_bit_values_01CDBC_data)
#define indexed_table_of_32_bit_values_01CE52_ADDRESS 0x01CE52u
extern const void *indexed_table_of_32_bit_values_01CE52_data;
#define indexed_table_of_32_bit_values_01CE52 ((const uint32_t *)indexed_table_of_32_bit_values_01CE52_data)
#define indexed_table_of_32_bit_values_01CF4E_ADDRESS 0x01CF4Eu
extern const void *indexed_table_of_32_bit_values_01CF4E_data;
#define indexed_table_of_32_bit_values_01CF4E ((const uint32_t *)indexed_table_of_32_bit_values_01CF4E_data)
#define indexed_table_of_32_bit_values_01D028_ADDRESS 0x01D028u
extern const void *indexed_table_of_32_bit_values_01D028_data;
#define indexed_table_of_32_bit_values_01D028 ((const uint32_t *)indexed_table_of_32_bit_values_01D028_data)
#define indexed_table_of_32_bit_values_01D100_ADDRESS 0x01D100u
extern const void *indexed_table_of_32_bit_values_01D100_data;
#define indexed_table_of_32_bit_values_01D100 ((const uint32_t *)indexed_table_of_32_bit_values_01D100_data)
#define turtle_struggle_bits_ADDRESS 0x01D200u
extern const void *turtle_struggle_bits_data;
#define turtle_struggle_bits ((const uint8_t *)turtle_struggle_bits_data)
#define turtle_hit_reactions_ADDRESS 0x01D3FCu
extern const void *turtle_hit_reactions_data;
#define turtle_hit_reactions ((const uint8_t *)turtle_hit_reactions_data)
#define turtle_attack_sounds_by_move_ADDRESS 0x01D526u
extern const void *turtle_attack_sounds_by_move_data;
#define turtle_attack_sounds_by_move ((const uint8_t *)turtle_attack_sounds_by_move_data)
#define turtle_hit_sounds_ADDRESS 0x01D556u
extern const void *turtle_hit_sounds_data;
#define turtle_hit_sounds ((const uint8_t *)turtle_hit_sounds_data)
#define turtle_attack_damages_ADDRESS 0x01D592u
extern const void *turtle_attack_damages_data;
#define turtle_attack_damages ((const uint16_t *)turtle_attack_damages_data)
#define turtle_combos_ADDRESS 0x01D5F2u
extern const void *turtle_combos_data;
#define turtle_combos ((const uint32_t *)turtle_combos_data)
#define indexed_table_of_32_bit_values_01E2DA_ADDRESS 0x01E2DAu
extern const void *indexed_table_of_32_bit_values_01E2DA_data;
#define indexed_table_of_32_bit_values_01E2DA ((const uint32_t *)indexed_table_of_32_bit_values_01E2DA_data)
#define area_layer_a_x_ADDRESS 0x01E5A0u
extern const void *area_layer_a_x_data;
#define area_layer_a_x ((const uint32_t *)area_layer_a_x_data)
#define area_layer_a_y_ADDRESS 0x01E5A4u
extern const void *area_layer_a_y_data;
#define area_layer_a_y ((const uint32_t *)area_layer_a_y_data)
#define area_layer_b_x_ADDRESS 0x01E5A8u
extern const void *area_layer_b_x_data;
#define area_layer_b_x ((const uint32_t *)area_layer_b_x_data)
#define area_layer_b_y_ADDRESS 0x01E5ACu
extern const void *area_layer_b_y_data;
#define area_layer_b_y ((const uint32_t *)area_layer_b_y_data)
#define area_layer_a_maps_ADDRESS 0x01E5B0u
extern const void *area_layer_a_maps_data;
#define area_layer_a_maps ((const uint32_t *)area_layer_a_maps_data)
#define area_layer_a_blocks_ADDRESS 0x01E5B4u
extern const void *area_layer_a_blocks_data;
#define area_layer_a_blocks ((const uint32_t *)area_layer_a_blocks_data)
#define area_layer_b_maps_ADDRESS 0x01E5B8u
extern const void *area_layer_b_maps_data;
#define area_layer_b_maps ((const uint32_t *)area_layer_b_maps_data)
#define area_layer_b_blocks_ADDRESS 0x01E5BCu
extern const void *area_layer_b_blocks_data;
#define area_layer_b_blocks ((const uint32_t *)area_layer_b_blocks_data)
#define area_layer_a_map_widths_ADDRESS 0x01E5C0u
extern const void *area_layer_a_map_widths_data;
#define area_layer_a_map_widths ((const uint16_t *)area_layer_a_map_widths_data)
#define area_layer_b_map_widths_ADDRESS 0x01E5C2u
extern const void *area_layer_b_map_widths_data;
#define area_layer_b_map_widths ((const uint16_t *)area_layer_b_map_widths_data)
#define indexed_table_of_32_bit_values_01ECEC_ADDRESS 0x01ECECu
extern const void *indexed_table_of_32_bit_values_01ECEC_data;
#define indexed_table_of_32_bit_values_01ECEC ((const uint32_t *)indexed_table_of_32_bit_values_01ECEC_data)
#define street_tv_pictures_ADDRESS 0x01F158u
extern const void *street_tv_pictures_data;
#define street_tv_pictures ((const uint32_t *)street_tv_pictures_data)
#define indexed_table_of_32_bit_values_01F43A_ADDRESS 0x01F43Au
extern const void *indexed_table_of_32_bit_values_01F43A_data;
#define indexed_table_of_32_bit_values_01F43A ((const uint32_t *)indexed_table_of_32_bit_values_01F43A_data)
#define indexed_table_of_32_bit_values_01F82C_ADDRESS 0x01F82Cu
extern const void *indexed_table_of_32_bit_values_01F82C_data;
#define indexed_table_of_32_bit_values_01F82C ((const uint32_t *)indexed_table_of_32_bit_values_01F82C_data)
#define indexed_table_of_32_bit_values_01FC96_ADDRESS 0x01FC96u
extern const void *indexed_table_of_32_bit_values_01FC96_data;
#define indexed_table_of_32_bit_values_01FC96 ((const uint32_t *)indexed_table_of_32_bit_values_01FC96_data)
#define fire_tiles_ADDRESS 0x0200F2u
extern const void *fire_tiles_data;
#define fire_tiles ((const uint32_t *)fire_tiles_data)
#define fire_tiles_area_0_ADDRESS 0x0203D2u
extern const void *fire_tiles_area_0_data;
#define fire_tiles_area_0 ((const uint32_t *)fire_tiles_area_0_data)
#define area_scroll_segments_ADDRESS 0x0205F2u
extern const void *area_scroll_segments_data;
#define area_scroll_segments ((const uint32_t *)area_scroll_segments_data)
#define turtle_animation_sets_ADDRESS 0x0208C0u
extern const void *turtle_animation_sets_data;
#define turtle_animation_sets ((const uint32_t *)turtle_animation_sets_data)
#define indexed_table_of_32_bit_values_02B2C6_ADDRESS 0x02B2C6u
extern const void *indexed_table_of_32_bit_values_02B2C6_data;
#define indexed_table_of_32_bit_values_02B2C6 ((const uint32_t *)indexed_table_of_32_bit_values_02B2C6_data)
#define indexed_table_of_32_bit_values_02B62A_ADDRESS 0x02B62Au
extern const void *indexed_table_of_32_bit_values_02B62A_data;
#define indexed_table_of_32_bit_values_02B62A ((const uint32_t *)indexed_table_of_32_bit_values_02B62A_data)
#define indexed_table_of_32_bit_values_02B728_ADDRESS 0x02B728u
extern const void *indexed_table_of_32_bit_values_02B728_data;
#define indexed_table_of_32_bit_values_02B728 ((const uint32_t *)indexed_table_of_32_bit_values_02B728_data)
#define indexed_table_of_32_bit_values_02BB16_ADDRESS 0x02BB16u
extern const void *indexed_table_of_32_bit_values_02BB16_data;
#define indexed_table_of_32_bit_values_02BB16 ((const uint32_t *)indexed_table_of_32_bit_values_02BB16_data)
#define exit_street_jump_animations_ADDRESS 0x02BC56u
extern const void *exit_street_jump_animations_data;
#define exit_street_jump_animations ((const uint32_t *)exit_street_jump_animations_data)
#define indexed_table_of_32_bit_values_02BC98_ADDRESS 0x02BC98u
extern const void *indexed_table_of_32_bit_values_02BC98_data;
#define indexed_table_of_32_bit_values_02BC98 ((const uint32_t *)indexed_table_of_32_bit_values_02BC98_data)
#define indexed_table_of_32_bit_values_02BD44_ADDRESS 0x02BD44u
extern const void *indexed_table_of_32_bit_values_02BD44_data;
#define indexed_table_of_32_bit_values_02BD44 ((const uint32_t *)indexed_table_of_32_bit_values_02BD44_data)
#define indexed_table_of_32_bit_values_02BF10_ADDRESS 0x02BF10u
extern const void *indexed_table_of_32_bit_values_02BF10_data;
#define indexed_table_of_32_bit_values_02BF10 ((const uint32_t *)indexed_table_of_32_bit_values_02BF10_data)
#define indexed_table_of_32_bit_values_02C04C_ADDRESS 0x02C04Cu
extern const void *indexed_table_of_32_bit_values_02C04C_data;
#define indexed_table_of_32_bit_values_02C04C ((const uint32_t *)indexed_table_of_32_bit_values_02C04C_data)
#define car_street_exit_x_ADDRESS 0x02C0F0u
extern const void *car_street_exit_x_data;
#define car_street_exit_x ((const uint16_t *)car_street_exit_x_data)
#define car_street_exit_y_ADDRESS 0x02C0F2u
extern const void *car_street_exit_y_data;
#define car_street_exit_y ((const uint16_t *)car_street_exit_y_data)
#define van_start_x_shakes_ADDRESS 0x02C3BAu
extern const void *van_start_x_shakes_data;
#define van_start_x_shakes ((const int16_t *)van_start_x_shakes_data)
#define van_start_y_shakes_ADDRESS 0x02C3BCu
extern const void *van_start_y_shakes_data;
#define van_start_y_shakes ((const int16_t *)van_start_y_shakes_data)
#define indexed_table_of_32_bit_values_02C93A_ADDRESS 0x02C93Au
extern const void *indexed_table_of_32_bit_values_02C93A_data;
#define indexed_table_of_32_bit_values_02C93A ((const uint32_t *)indexed_table_of_32_bit_values_02C93A_data)
#define exit_places_x_ADDRESS 0x02CE62u
extern const void *exit_places_x_data;
#define exit_places_x ((const uint16_t *)exit_places_x_data)
#define exit_places_y_ADDRESS 0x02CE64u
extern const void *exit_places_y_data;
#define exit_places_y ((const uint16_t *)exit_places_y_data)
#define indexed_table_of_32_bit_values_02CE8E_ADDRESS 0x02CE8Eu
extern const void *indexed_table_of_32_bit_values_02CE8E_data;
#define indexed_table_of_32_bit_values_02CE8E ((const uint32_t *)indexed_table_of_32_bit_values_02CE8E_data)
#define building_site_exit_x_ADDRESS 0x02D304u
extern const void *building_site_exit_x_data;
#define building_site_exit_x ((const uint16_t *)building_site_exit_x_data)
#define building_site_exit_y_ADDRESS 0x02D306u
extern const void *building_site_exit_y_data;
#define building_site_exit_y ((const uint16_t *)building_site_exit_y_data)
#define building_site_exit_heights_ADDRESS 0x02D308u
extern const void *building_site_exit_heights_data;
#define building_site_exit_heights ((const uint16_t *)building_site_exit_heights_data)
#define building_site_exit_drifts_ADDRESS 0x02D30Au
extern const void *building_site_exit_drifts_data;
#define building_site_exit_drifts ((const uint16_t *)building_site_exit_drifts_data)
#define exit_building_site_rise_animations_ADDRESS 0x02D4F4u
extern const void *exit_building_site_rise_animations_data;
#define exit_building_site_rise_animations ((const uint32_t *)exit_building_site_rise_animations_data)
#define indexed_table_of_32_bit_values_02D616_ADDRESS 0x02D616u
extern const void *indexed_table_of_32_bit_values_02D616_data;
#define indexed_table_of_32_bit_values_02D616 ((const uint32_t *)indexed_table_of_32_bit_values_02D616_data)
#define indexed_table_of_32_bit_values_02F654_ADDRESS 0x02F654u
extern const void *indexed_table_of_32_bit_values_02F654_data;
#define indexed_table_of_32_bit_values_02F654 ((const uint32_t *)indexed_table_of_32_bit_values_02F654_data)
#define enemy_limits_ADDRESS 0x02F67Cu
extern const void *enemy_limits_data;
#define enemy_limits ((const uint8_t *)enemy_limits_data)
#define indexed_table_of_32_bit_values_02F8C6_ADDRESS 0x02F8C6u
extern const void *indexed_table_of_32_bit_values_02F8C6_data;
#define indexed_table_of_32_bit_values_02F8C6 ((const uint32_t *)indexed_table_of_32_bit_values_02F8C6_data)
#define opening_turtle_y_speeds_ADDRESS 0x02FBE0u
extern const void *opening_turtle_y_speeds_data;
#define opening_turtle_y_speeds ((const int16_t *)opening_turtle_y_speeds_data)
#define balloon_records_ADDRESS 0x030350u
extern const void *balloon_records_data;
#define balloon_records ((const uint8_t *)balloon_records_data)
#define indexed_table_of_32_bit_values_0303EE_ADDRESS 0x0303EEu
extern const void *indexed_table_of_32_bit_values_0303EE_data;
#define indexed_table_of_32_bit_values_0303EE ((const uint32_t *)indexed_table_of_32_bit_values_0303EE_data)
#define shake_x_ADDRESS 0x030652u
extern const void *shake_x_data;
#define shake_x ((const int8_t *)shake_x_data)
#define shake_y_ADDRESS 0x030653u
extern const void *shake_y_data;
#define shake_y ((const int8_t *)shake_y_data)
#define indexed_table_of_32_bit_values_030996_ADDRESS 0x030996u
extern const void *indexed_table_of_32_bit_values_030996_data;
#define indexed_table_of_32_bit_values_030996 ((const uint32_t *)indexed_table_of_32_bit_values_030996_data)
#define intro_fire_frames_ADDRESS 0x031152u
extern const void *intro_fire_frames_data;
#define intro_fire_frames ((const uint32_t *)intro_fire_frames_data)
#define attack_hit_jobs_ADDRESS 0x0471F0u
extern const void *attack_hit_jobs_data;
#define attack_hit_jobs (*(const uint8_t *)attack_hit_jobs_data)
#define attack_check_objects_ADDRESS 0x0471F4u
extern const void *attack_check_objects_data;
#define attack_check_objects ((const uint32_t *)attack_check_objects_data)
#define object_group_counts_ADDRESS 0x047BDEu
extern const void *object_group_counts_data;
#define object_group_counts ((const uint8_t *)object_group_counts_data)
#define object_groups_ADDRESS 0x047BE2u
extern const void *object_groups_data;
#define object_groups ((const uint32_t *)object_groups_data)
#define floor_contact_masks_ADDRESS 0x048818u
extern const void *floor_contact_masks_data;
#define floor_contact_masks (*(const int8_t *)floor_contact_masks_data)
#define area_hop_heights_ADDRESS 0x04881Cu
extern const void *area_hop_heights_data;
#define area_hop_heights ((const uint16_t *)area_hop_heights_data)
#define area_stairs_top_heights_ADDRESS 0x048830u
extern const void *area_stairs_top_heights_data;
#define area_stairs_top_heights ((const uint8_t *)area_stairs_top_heights_data)
#define hit_reaction_down_ADDRESS 0x048C5Eu
extern const void *hit_reaction_down_data;
#define hit_reaction_down ((const uint8_t *)hit_reaction_down_data)
#define enemy_edge_steps_ADDRESS 0x048F36u
extern const void *enemy_edge_steps_data;
#define enemy_edge_steps (*(const uint32_t *)enemy_edge_steps_data)
#define scenery_fire_animations_ADDRESS 0x04A8A4u
extern const void *scenery_fire_animations_data;
#define scenery_fire_animations ((const uint32_t *)scenery_fire_animations_data)
#define whip_arm_follow_animations_ADDRESS 0x04B742u
extern const void *whip_arm_follow_animations_data;
#define whip_arm_follow_animations ((const uint32_t *)whip_arm_follow_animations_data)
#define whip_arm_offsets_ADDRESS 0x04B7DEu
extern const void *whip_arm_offsets_data;
#define whip_arm_offsets ((const int16_t *)whip_arm_offsets_data)
#define rocksteady_bullet_animations_ADDRESS 0x04BFD6u
extern const void *rocksteady_bullet_animations_data;
#define rocksteady_bullet_animations ((const uint32_t *)rocksteady_bullet_animations_data)
#define dynamite_flash_colours_ADDRESS 0x04C3EEu
extern const void *dynamite_flash_colours_data;
#define dynamite_flash_colours ((const uint8_t *)dynamite_flash_colours_data)
#define door_piece_animations_ADDRESS 0x04C5D4u
extern const void *door_piece_animations_data;
#define door_piece_animations ((const uint32_t *)door_piece_animations_data)
#define door_piece_speeds_ADDRESS 0x04C5E4u
extern const void *door_piece_speeds_data;
#define door_piece_speeds (*(const uint16_t *)door_piece_speeds_data)
#define technodrome_door_soldiers_ADDRESS 0x04D764u
extern const void *technodrome_door_soldiers_data;
#define technodrome_door_soldiers ((const uint8_t *)technodrome_door_soldiers_data)
#define spear_soldier_hits_ADDRESS 0x04DD2Cu
extern const void *spear_soldier_hits_data;
#define spear_soldier_hits ((const uint8_t *)spear_soldier_hits_data)
#define skateboard_soldier_colours_ADDRESS 0x04EFD0u
extern const void *skateboard_soldier_colours_data;
#define skateboard_soldier_colours ((const uint8_t *)skateboard_soldier_colours_data)
#define enemy_skateboard_animations_ADDRESS 0x04EFD4u
extern const void *enemy_skateboard_animations_data;
#define enemy_skateboard_animations ((const uint32_t *)enemy_skateboard_animations_data)
#define skateboard_soldier_definitions_ADDRESS 0x04EFE0u
extern const void *skateboard_soldier_definitions_data;
#define skateboard_soldier_definitions ((const uint32_t *)skateboard_soldier_definitions_data)
#define rocksteady_moves_ADDRESS 0x04F58Cu
extern const void *rocksteady_moves_data;
#define rocksteady_moves ((const uint16_t *)rocksteady_moves_data)
#define rocksteady_mode_times_ADDRESS 0x04F58Eu
extern const void *rocksteady_mode_times_data;
#define rocksteady_mode_times ((const uint16_t *)rocksteady_mode_times_data)
#define rocksteady_mode_frames_ADDRESS 0x04F594u
extern const void *rocksteady_mode_frames_data;
#define rocksteady_mode_frames (*(const uint16_t *)rocksteady_mode_frames_data)
#define rocksteady_hits_to_strike_back_by_difficulty_ADDRESS 0x04F5B4u
extern const void *rocksteady_hits_to_strike_back_by_difficulty_data;
#define rocksteady_hits_to_strike_back_by_difficulty ((const uint8_t *)rocksteady_hits_to_strike_back_by_difficulty_data)
#define rocksteady_close_attack_waits_ADDRESS 0x04F5C4u
extern const void *rocksteady_close_attack_waits_data;
#define rocksteady_close_attack_waits ((const uint8_t *)rocksteady_close_attack_waits_data)
#define rocksteady_far_attack_waits_ADDRESS 0x04F5CCu
extern const void *rocksteady_far_attack_waits_data;
#define rocksteady_far_attack_waits ((const uint8_t *)rocksteady_far_attack_waits_data)
#define rocksteady_move_choices_ADDRESS 0x04F5D4u
extern const void *rocksteady_move_choices_data;
#define rocksteady_move_choices (*(const uint8_t *)rocksteady_move_choices_data)
#define rocksteady_hit_reactions_ADDRESS 0x04F6B6u
extern const void *rocksteady_hit_reactions_data;
#define rocksteady_hit_reactions ((const uint8_t *)rocksteady_hit_reactions_data)
#define enemy_rocksteady_shoot_around_animations_ADDRESS 0x04F9DAu
extern const void *enemy_rocksteady_shoot_around_animations_data;
#define enemy_rocksteady_shoot_around_animations ((const uint32_t *)enemy_rocksteady_shoot_around_animations_data)
#define rocksteady_bursts_ADDRESS 0x04F9DEu
extern const void *rocksteady_bursts_data;
#define rocksteady_bursts ((const uint32_t *)rocksteady_bursts_data)
#define enemy_rocksteady_flinch_animations_ADDRESS 0x04FE56u
extern const void *enemy_rocksteady_flinch_animations_data;
#define enemy_rocksteady_flinch_animations ((const uint32_t *)enemy_rocksteady_flinch_animations_data)
#define enemy_rocksteady_reel_animations_ADDRESS 0x04FEBEu
extern const void *enemy_rocksteady_reel_animations_data;
#define enemy_rocksteady_reel_animations ((const uint32_t *)enemy_rocksteady_reel_animations_data)
#define enemy_rocksteady_flinch_from_behind_animations_ADDRESS 0x04FEF6u
extern const void *enemy_rocksteady_flinch_from_behind_animations_data;
#define enemy_rocksteady_flinch_from_behind_animations ((const uint32_t *)enemy_rocksteady_flinch_from_behind_animations_data)
#define enemy_bebop_stand_animations_ADDRESS 0x050692u
extern const void *enemy_bebop_stand_animations_data;
#define enemy_bebop_stand_animations ((const uint32_t *)enemy_bebop_stand_animations_data)
#define enemy_mouser_enter_animations_ADDRESS 0x05101Eu
extern const void *enemy_mouser_enter_animations_data;
#define enemy_mouser_enter_animations ((const uint32_t *)enemy_mouser_enter_animations_data)
#define mouser_enter_y_speeds_ADDRESS 0x05102Au
extern const void *mouser_enter_y_speeds_data;
#define mouser_enter_y_speeds ((const int16_t *)mouser_enter_y_speeds_data)
#define mouser_enter_height_speeds_ADDRESS 0x05102Cu
extern const void *mouser_enter_height_speeds_data;
#define mouser_enter_height_speeds ((const int16_t *)mouser_enter_height_speeds_data)
#define mouser_enter_heights_ADDRESS 0x051036u
extern const void *mouser_enter_heights_data;
#define mouser_enter_heights ((const uint8_t *)mouser_enter_heights_data)
#define mouser_moves_ADDRESS 0x0512E4u
extern const void *mouser_moves_data;
#define mouser_moves (*(const uint16_t *)mouser_moves_data)
#define mouser_edge_steps_ADDRESS 0x0514C6u
extern const void *mouser_edge_steps_data;
#define mouser_edge_steps (*(const uint32_t *)mouser_edge_steps_data)
#define enemy_mouser_sit_animations_ADDRESS 0x0518CCu
extern const void *enemy_mouser_sit_animations_data;
#define enemy_mouser_sit_animations ((const uint32_t *)enemy_mouser_sit_animations_data)
#define mouser_hole_animation_tables_ADDRESS 0x051AE2u
extern const void *mouser_hole_animation_tables_data;
#define mouser_hole_animation_tables ((const uint32_t *)mouser_hole_animation_tables_data)
#define mouser_hole_counts_ADDRESS 0x051AEEu
extern const void *mouser_hole_counts_data;
#define mouser_hole_counts ((const uint8_t *)mouser_hole_counts_data)
#define rodney_approach_moves_ADDRESS 0x051F9Cu
extern const void *rodney_approach_moves_data;
#define rodney_approach_moves ((const uint8_t *)rodney_approach_moves_data)
#define whip_arm_animations_ADDRESS 0x052450u
extern const void *whip_arm_animations_data;
#define whip_arm_animations ((const uint32_t *)whip_arm_animations_data)
#define shredder_clone_limits_ADDRESS 0x052918u
extern const void *shredder_clone_limits_data;
#define shredder_clone_limits ((const uint8_t *)shredder_clone_limits_data)
#define shredder_guard_waits_ADDRESS 0x052A60u
extern const void *shredder_guard_waits_data;
#define shredder_guard_waits ((const uint8_t *)shredder_guard_waits_data)
#define shredder_attack_waits_ADDRESS 0x052ADEu
extern const void *shredder_attack_waits_data;
#define shredder_attack_waits ((const uint8_t *)shredder_attack_waits_data)
#define shredder_ray_waits_ADDRESS 0x052B3Au
extern const void *shredder_ray_waits_data;
#define shredder_ray_waits ((const uint16_t *)shredder_ray_waits_data)
#define shredder_approach_moves_ADDRESS 0x052B7Cu
extern const void *shredder_approach_moves_data;
#define shredder_approach_moves ((const uint16_t *)shredder_approach_moves_data)
#define object_routine_tables_ADDRESS 0x053896u
extern const void *object_routine_tables_data;
#define object_routine_tables ((const uint32_t *)object_routine_tables_data)
#define enemy_attacks_ADDRESS 0x053D2Au
extern const void *enemy_attacks_data;
#define enemy_attacks (*(const uint32_t *)enemy_attacks_data)
#define enemy_ways_in_ADDRESS 0x053ED6u
extern const void *enemy_ways_in_data;
#define enemy_ways_in (*(const uint32_t *)enemy_ways_in_data)

/* the board's registers (read and written through the bus) */
#define PALETTE_08000D 0x08000Du
#define PALETTE_08000F 0x08000Fu
#define IO_0A0001 0x0A0001u
#define IO_0A0003 0x0A0003u
#define IO_0A0005 0x0A0005u
#define IO_0A0007 0x0A0007u
#define IO_0A0009 0x0A0009u
#define IO_0A0011 0x0A0011u
#define IO_0A0013 0x0A0013u
#define IO_0A0015 0x0A0015u
#define IO_0A0019 0x0A0019u
#define PRIORITY_0C0001 0x0C0001u
#define TILES_10032E 0x10032Eu
#define TILES_10032F 0x10032Fu
#define TILES_100340 0x100340u
#define TILES_100341 0x100341u
#define TILES_100838 0x100838u
#define TILES_100839 0x100839u
#define TILES_102030 0x102030u
#define TILES_1027A9 0x1027A9u
#define TILES_102EAE 0x102EAEu
#define TILES_102EAF 0x102EAFu
#define TILES_102EB8 0x102EB8u
#define TILES_102EB9 0x102EB9u
#define TILES_106018 0x106018u
#define TILES_106019 0x106019u
#define TILES_106400 0x106400u
#define TILES_106401 0x106401u
#define TILES_106402 0x106402u
#define TILES_106403 0x106403u
#define TILES_106900 0x106900u
#define TILES_106B00 0x106B00u
#define TILES_106C00 0x106C00u
#define TILES_106D00 0x106D00u
#define TILES_106E00 0x106E00u
#define UNMAPPED_10E801 0x10E801u
#define SPRITE_CONTROL_140000 0x140000u
#define SPRITE_CONTROL_140001 0x140001u
#define SPRITE_CONTROL_140002 0x140002u
#define SPRITE_CONTROL_140003 0x140003u
#define SPRITE_CONTROL_140004 0x140004u
#define SPRITE_CONTROL_140005 0x140005u
#define SPRITE_CONTROL_140006 0x140006u
#define SPRITE_CONTROL_140007 0x140007u

/* data in the program symbols.txt and the assets label */
#define vectors_000000_ADDRESS 0x000000u
#define unused_dip_settings_ADDRESS 0x00142Au
#define program_chips_ADDRESS 0x0016CCu
#define chip_sum_places_ADDRESS 0x0016E8u
#define good_chip_sums_ADDRESS 0x0016F8u
#define boot_colours_ADDRESS 0x00191Eu
#define indexed_table_of_32_bit_values_001958_ADDRESS 0x001958u
#define good_mask_rom_sums_ADDRESS 0x001E34u
#define one_point_ADDRESS 0x002030u
#define table_loaded_by_lea_0020EC_ADDRESS 0x0020ECu
#define table_loaded_by_lea_0021EE_ADDRESS 0x0021EEu
#define game_texts_ADDRESS 0x00292Au
#define unexplained_002989_ADDRESS 0x002989u
#define texts_00298A_ADDRESS 0x00298Au
#define unexplained_002999_ADDRESS 0x002999u
#define texts_00299A_ADDRESS 0x00299Au
#define unexplained_0029E5_ADDRESS 0x0029E5u
#define texts_0029E6_ADDRESS 0x0029E6u
#define unexplained_002AC7_ADDRESS 0x002AC7u
#define texts_002AC8_ADDRESS 0x002AC8u
#define unexplained_002AD3_ADDRESS 0x002AD3u
#define texts_002AD4_ADDRESS 0x002AD4u
#define unexplained_002AF7_ADDRESS 0x002AF7u
#define texts_002AF8_ADDRESS 0x002AF8u
#define unexplained_002CFF_ADDRESS 0x002CFFu
#define texts_002D00_ADDRESS 0x002D00u
#define staff_roll_blank_text_ADDRESS 0x002F26u
#define demo_texts_ADDRESS 0x0030AEu
#define unexplained_0030EB_ADDRESS 0x0030EBu
#define texts_0030EC_ADDRESS 0x0030ECu
#define unexplained_0030FD_ADDRESS 0x0030FDu
#define texts_0030FE_ADDRESS 0x0030FEu
#define unexplained_00310D_ADDRESS 0x00310Du
#define texts_00310E_ADDRESS 0x00310Eu
#define unexplained_00311D_ADDRESS 0x00311Du
#define texts_00311E_ADDRESS 0x00311Eu
#define box_frame_tiles_ADDRESS 0x003302u
#define health_bar_tiles_ADDRESS 0x00334Au
#define unreached_box_frame_tiles_ADDRESS 0x0033DEu
#define big_font_ADDRESS 0x00388Cu
#define channel_records_ADDRESS 0x0047C8u
#define indexed_table_of_32_bit_values_0049DA_ADDRESS 0x0049DAu
#define indexed_table_of_32_bit_values_0049F8_ADDRESS 0x0049F8u
#define indexed_table_of_32_bit_values_004AB4_ADDRESS 0x004AB4u
#define indexed_table_of_16_bit_values_004B58_ADDRESS 0x004B58u
#define indexed_table_of_32_bit_values_004B88_ADDRESS 0x004B88u
#define table_loaded_by_lea_004C76_ADDRESS 0x004C76u
#define indexed_table_of_32_bit_values_004CD2_ADDRESS 0x004CD2u
#define indexed_table_of_32_bit_values_004EE8_ADDRESS 0x004EE8u
#define indexed_table_of_32_bit_values_004F06_ADDRESS 0x004F06u
#define indexed_table_of_32_bit_values_005000_ADDRESS 0x005000u
#define profile_motions_ADDRESS 0x005078u
#define indexed_table_of_32_bit_values_0051CE_ADDRESS 0x0051CEu
#define unexplained_00525A_ADDRESS 0x00525Au
#define unreached_sprite_bytes_ADDRESS 0x005294u
#define demos_0052D4_ADDRESS 0x0052D4u
#define indexed_table_of_32_bit_values_00569E_ADDRESS 0x00569Eu
#define indexed_table_of_32_bit_values_0056D8_ADDRESS 0x0056D8u
#define indexed_table_of_32_bit_values_005736_ADDRESS 0x005736u
#define indexed_table_of_32_bit_values_005794_ADDRESS 0x005794u
#define table_loaded_by_lea_005846_ADDRESS 0x005846u
#define indexed_table_of_32_bit_values_00586A_ADDRESS 0x00586Au
#define table_loaded_by_lea_005896_ADDRESS 0x005896u
#define indexed_table_of_32_bit_values_005A12_ADDRESS 0x005A12u
#define table_loaded_by_lea_005A58_ADDRESS 0x005A58u
#define indexed_table_of_32_bit_values_005B5E_ADDRESS 0x005B5Eu
#define table_loaded_by_lea_005B9C_ADDRESS 0x005B9Cu
#define indexed_table_of_32_bit_values_005C32_ADDRESS 0x005C32u
#define table_loaded_by_lea_005C5E_ADDRESS 0x005C5Eu
#define test_mode_texts_ADDRESS 0x005E1Eu
#define unexplained_005E8D_ADDRESS 0x005E8Du
#define texts_005E8E_ADDRESS 0x005E8Eu
#define unexplained_006021_ADDRESS 0x006021u
#define texts_006022_ADDRESS 0x006022u
#define unexplained_006035_ADDRESS 0x006035u
#define texts_006036_ADDRESS 0x006036u
#define unexplained_006049_ADDRESS 0x006049u
#define texts_00604A_ADDRESS 0x00604Au
#define unexplained_00605D_ADDRESS 0x00605Du
#define texts_00605E_ADDRESS 0x00605Eu
#define unexplained_00611D_ADDRESS 0x00611Du
#define texts_00611E_ADDRESS 0x00611Eu
#define indexed_table_of_32_bit_values_0061E8_ADDRESS 0x0061E8u
#define indexed_table_of_32_bit_values_006282_ADDRESS 0x006282u
#define indexed_table_of_32_bit_values_0062E2_ADDRESS 0x0062E2u
#define indexed_table_of_32_bit_values_0063A0_ADDRESS 0x0063A0u
#define indexed_table_of_32_bit_values_0063D8_ADDRESS 0x0063D8u
#define indexed_table_of_32_bit_values_0064C6_ADDRESS 0x0064C6u
#define indexed_table_of_32_bit_values_006688_ADDRESS 0x006688u
#define indexed_table_of_32_bit_values_0066A6_ADDRESS 0x0066A6u
#define indexed_table_of_32_bit_values_006B42_ADDRESS 0x006B42u
#define indexed_table_of_32_bit_values_006BFA_ADDRESS 0x006BFAu
#define indexed_table_of_32_bit_values_006C18_ADDRESS 0x006C18u
#define indexed_table_of_32_bit_values_006CDE_ADDRESS 0x006CDEu
#define indexed_table_of_32_bit_values_006EE6_ADDRESS 0x006EE6u
#define table_loaded_by_lea_007018_ADDRESS 0x007018u
#define indexed_table_of_32_bit_values_0070BE_ADDRESS 0x0070BEu
#define record_0071DC_ADDRESS 0x0071DCu
#define technodrome_wreck_parts_ADDRESS 0x007276u
#define staff_roll_credits_ADDRESS 0x007612u
#define staff_roll_names_ADDRESS 0x0076F2u
#define indexed_table_of_32_bit_values_00783A_ADDRESS 0x00783Au
#define indexed_table_of_32_bit_values_007860_ADDRESS 0x007860u
#define indexed_table_of_32_bit_values_0078F8_ADDRESS 0x0078F8u
#define indexed_table_of_32_bit_values_007A0E_ADDRESS 0x007A0Eu
#define indexed_table_of_32_bit_values_007B22_ADDRESS 0x007B22u
#define indexed_table_of_32_bit_values_007B58_ADDRESS 0x007B58u
#define ranking_defaults_ADDRESS 0x0082E6u
#define ranking_frame_tiles_ADDRESS 0x00841Au
#define ranking_frame_tiles_2_ADDRESS 0x00842Cu
#define indexed_table_of_32_bit_values_0088F0_ADDRESS 0x0088F0u
#define indexed_table_of_32_bit_values_00890E_ADDRESS 0x00890Eu
#define indexed_table_of_32_bit_values_008A0A_ADDRESS 0x008A0Au
#define table_loaded_by_lea_008ABE_ADDRESS 0x008ABEu
#define indexed_table_of_32_bit_values_008D12_ADDRESS 0x008D12u
#define indexed_table_of_32_bit_values_008D30_ADDRESS 0x008D30u
#define indexed_table_of_32_bit_values_008E6A_ADDRESS 0x008E6Au
#define indexed_table_of_32_bit_values_008E88_ADDRESS 0x008E88u
#define palettes_008F58_ADDRESS 0x008F58u
#define unexplained_008F7D_ADDRESS 0x008F7Du
#define indexed_table_of_32_bit_values_008F94_ADDRESS 0x008F94u
#define indexed_table_of_32_bit_values_009632_ADDRESS 0x009632u
#define table_loaded_by_lea_009872_ADDRESS 0x009872u
#define table_loaded_by_lea_009C5C_ADDRESS 0x009C5Cu
#define unexplained_00A1DE_ADDRESS 0x00A1DEu
#define palettes_00A1FE_ADDRESS 0x00A1FEu
#define unexplained_00B606_ADDRESS 0x00B606u
#define palettes_00B608_ADDRESS 0x00B608u
#define unexplained_00CD44_ADDRESS 0x00CD44u
#define palettes_00CD48_ADDRESS 0x00CD48u
#define unexplained_00CD68_ADDRESS 0x00CD68u
#define palettes_00CD6C_ADDRESS 0x00CD6Cu
#define unexplained_00CD8C_ADDRESS 0x00CD8Cu
#define palettes_00CD90_ADDRESS 0x00CD90u
#define unexplained_00CDB0_ADDRESS 0x00CDB0u
#define palettes_00CDB4_ADDRESS 0x00CDB4u
#define palette_effects_00D134_ADDRESS 0x00D134u
#define code_table_00DF5E_ADDRESS 0x00DF5Eu
#define indexed_table_of_16_bit_values_00E750_ADDRESS 0x00E750u
#define code_table_00E956_ADDRESS 0x00E956u
#define code_table_00E9C6_ADDRESS 0x00E9C6u
#define code_table_00EB80_ADDRESS 0x00EB80u
#define sprite_lists_00F03C_ADDRESS 0x00F03Cu
#define sprite_lists_00F6E8_ADDRESS 0x00F6E8u
#define sprite_lists_00F862_ADDRESS 0x00F862u
#define code_table_00F8D2_ADDRESS 0x00F8D2u
#define code_table_00FA14_ADDRESS 0x00FA14u
#define code_table_00FA60_ADDRESS 0x00FA60u
#define code_table_00FAA0_ADDRESS 0x00FAA0u
#define code_table_00FB9C_ADDRESS 0x00FB9Cu
#define code_table_00FCC8_ADDRESS 0x00FCC8u
#define code_table_00FDB6_ADDRESS 0x00FDB6u
#define code_table_00FED2_ADDRESS 0x00FED2u
#define code_table_010098_ADDRESS 0x010098u
#define code_table_0100FC_ADDRESS 0x0100FCu
#define code_table_0101E8_ADDRESS 0x0101E8u
#define code_table_0102F6_ADDRESS 0x0102F6u
#define code_table_010410_ADDRESS 0x010410u
#define code_table_01053C_ADDRESS 0x01053Cu
#define turret_slides_ADDRESS 0x010776u
#define code_table_0108C0_ADDRESS 0x0108C0u
#define code_table_01091A_ADDRESS 0x01091Au
#define code_table_0109C6_ADDRESS 0x0109C6u
#define code_table_010AD8_ADDRESS 0x010AD8u
#define code_table_010C4A_ADDRESS 0x010C4Au
#define code_table_010CCE_ADDRESS 0x010CCEu
#define indexed_table_of_8_bit_values_010D5A_ADDRESS 0x010D5Au
#define electric_waits_ADDRESS 0x010D62u
#define code_table_010E48_ADDRESS 0x010E48u
#define code_table_011080_ADDRESS 0x011080u
#define code_table_0110FE_ADDRESS 0x0110FEu
#define code_table_01128C_ADDRESS 0x01128Cu
#define code_table_011432_ADDRESS 0x011432u
#define code_table_011532_ADDRESS 0x011532u
#define table_loaded_by_lea_011C42_ADDRESS 0x011C42u
#define rock_soldier_site_speeds_ADDRESS 0x011EB2u
#define boss_speeds_ADDRESS 0x011EF2u
#define code_table_011FC4_ADDRESS 0x011FC4u
#define code_table_0120A8_ADDRESS 0x0120A8u
#define code_table_01217C_ADDRESS 0x01217Cu
#define rock_soldier_technodrome_reaches_ADDRESS 0x012338u
#define rock_soldier_technodrome_reaches_2_ADDRESS 0x01234Cu
#define table_loaded_by_lea_012486_ADDRESS 0x012486u
#define unexplained_012624_ADDRESS 0x012624u
#define code_table_012660_ADDRESS 0x012660u
#define code_table_0126FE_ADDRESS 0x0126FEu
#define code_table_012884_ADDRESS 0x012884u
#define code_table_01297C_ADDRESS 0x01297Cu
#define code_table_012A16_ADDRESS 0x012A16u
#define code_table_012BC4_ADDRESS 0x012BC4u
#define car_hit_box_places_ADDRESS 0x012C10u
#define code_table_012C76_ADDRESS 0x012C76u
#define code_table_012D32_ADDRESS 0x012D32u
#define helicopter_waits_ADDRESS 0x013150u
#define helicopter_waits_2_ADDRESS 0x013154u
#define code_table_01326E_ADDRESS 0x01326Eu
#define code_table_0133E2_ADDRESS 0x0133E2u
#define code_table_013C16_ADDRESS 0x013C16u
#define code_table_0142C2_ADDRESS 0x0142C2u
#define parking_meter_animations_ADDRESS 0x01452Au
#define speed_sign_animations_ADDRESS 0x01456Au
#define title_logo_animations_ADDRESS 0x0151D8u
#define cone_animations_ADDRESS 0x015AF6u
#define sprite_lists_015FE0_ADDRESS 0x015FE0u
#define ball_shadow_ADDRESS 0x016660u
#define turret_shot_shadow_ADDRESS 0x016668u
#define turret_shot_technodrome_shadow_ADDRESS 0x016670u
#define rock_soldier_shadow_ADDRESS 0x016C50u
#define missile_shadow_ADDRESS 0x016F20u
#define helicopter_shadow_ADDRESS 0x0171A0u
#define helicopter_flipped_shadow_ADDRESS 0x0171A8u
#define flying_drone_shadow_ADDRESS 0x017958u
#define stockman_shadow_ADDRESS 0x017C20u
#define mouser_shadow_ADDRESS 0x017C38u
#define hydrant_stats_ADDRESS 0x0180FAu
#define hydrant_struck_stats_ADDRESS 0x018104u
#define hydrant_water_stats_ADDRESS 0x01810Eu
#define roadside_pole_stats_ADDRESS 0x018118u
#define alligator_stats_ADDRESS 0x018122u
#define billboard_stats_ADDRESS 0x01812Cu
#define oil_drum_stats_ADDRESS 0x01815Eu
#define explosion_stats_ADDRESS 0x018168u
#define skateboarder_stats_ADDRESS 0x018172u
#define turret_stats_ADDRESS 0x01817Cu
#define turret_struck_stats_ADDRESS 0x018186u
#define turret_shot_stats_ADDRESS 0x018190u
#define waiting_soldier_stats_ADDRESS 0x01819Au
#define inert_stats_ADDRESS 0x0181A4u
#define freeze_nozzle_stats_ADDRESS 0x0181AEu
#define electric_beam_stats_ADDRESS 0x0181C2u
#define rock_soldier_site_stats_ADDRESS 0x0181CCu
#define gun_blast_stats_ADDRESS 0x0181D6u
#define car_hit_box_stats_ADDRESS 0x0181E0u
#define electric_column_stats_ADDRESS 0x0181EAu
#define helicopter_stats_ADDRESS 0x0181FEu
#define copter_bullet_stats_ADDRESS 0x018208u
#define falling_missile_stats_ADDRESS 0x018212u
#define falling_missile_at_turtle_stats_ADDRESS 0x01821Cu
#define rock_soldier_technodrome_stats_ADDRESS 0x018226u
#define missile_stats_ADDRESS 0x018230u
#define wall_slab_stats_ADDRESS 0x01823Au
#define shock_bars_hit_box_stats_ADDRESS 0x018244u
#define sewer_missile_stats_ADDRESS 0x01824Eu
#define spiked_pole_stats_ADDRESS 0x018258u
#define flying_drone_stats_ADDRESS 0x018262u
#define drone_shot_stats_ADDRESS 0x01826Cu
#define roadside_cone_stats_ADDRESS 0x018276u
#define one_way_sign_stats_ADDRESS 0x018280u
#define stockman_stats_ADDRESS 0x01828Au
#define falling_mouser_stats_ADDRESS 0x018294u
#define ball_stats_ADDRESS 0x01829Eu
#define technodrome_ball_stats_ADDRESS 0x0182A8u
#define ball_dropper_stats_ADDRESS 0x0182B2u
#define road_barrier_stats_ADDRESS 0x0182BCu
#define tire_stack_stats_ADDRESS 0x0182C6u
#define krang_stats_ADDRESS 0x0182D0u
#define krang_laser_stats_ADDRESS 0x0182DAu
#define krang_fist_stats_ADDRESS 0x0182E4u
#define tile_block_places_ADDRESS 0x0187BCu
#define billboard_fallen_tile_block_ADDRESS 0x018854u
#define scenery_crash_tile_block_1_ADDRESS 0x0189D8u
#define scenery_crash_tile_block_2_ADDRESS 0x018A12u
#define scenery_crash_low_tile_block_1_ADDRESS 0x018A24u
#define scenery_crash_low_tile_block_2_ADDRESS 0x018A5Eu
#define scenery_crash_tile_block_3_ADDRESS 0x018A70u
#define scenery_crash_tile_block_4_ADDRESS 0x018A8Au
#define drill_hole_area_1_tile_block_ADDRESS 0x018AECu
#define drill_hole_area_1_tile_block_2_ADDRESS 0x018BAEu
#define parked_car_tile_block_ADDRESS 0x018C28u
#define drill_hole_tile_block_ADDRESS 0x018F16u
#define drill_hole_tile_block_2_ADDRESS 0x018FB8u
#define double_door_kicked_tile_block_ADDRESS 0x01904Au
#define double_door_open_tile_block_ADDRESS 0x019100u
#define double_door_open_tile_block_2_ADDRESS 0x019152u
#define wall_slab_tile_block_ADDRESS 0x0191A4u
#define unexplained_01948B_ADDRESS 0x01948Bu
#define texts_01948C_ADDRESS 0x01948Cu
#define unexplained_019577_ADDRESS 0x019577u
#define texts_019578_ADDRESS 0x019578u
#define unexplained_019657_ADDRESS 0x019657u
#define texts_019658_ADDRESS 0x019658u
#define unexplained_01977D_ADDRESS 0x01977Du
#define texts_01977E_ADDRESS 0x01977Eu
#define unexplained_0198C3_ADDRESS 0x0198C3u
#define texts_0198C4_ADDRESS 0x0198C4u
#define unexplained_0198FD_ADDRESS 0x0198FDu
#define texts_0198FE_ADDRESS 0x0198FEu
#define unexplained_019937_ADDRESS 0x019937u
#define texts_019938_ADDRESS 0x019938u
#define unexplained_01996D_ADDRESS 0x01996Du
#define texts_01996E_ADDRESS 0x01996Eu
#define epilogue_typed_text_ADDRESS 0x0199A4u
#define turtle_start_places_ADDRESS 0x019E62u
#define turtle_start_places_2_ADDRESS 0x019E72u
#define turtle_fall_in_places_ADDRESS 0x019E82u
#define turtle_fall_in_places_stage_3_ADDRESS 0x019E9Au
#define skateboard_shadows_ADDRESS 0x01A308u
#define prompt_tiles_ADDRESS 0x01D35Cu
#define prompt_tiles_2_ADDRESS 0x01D3ACu
#define record_01F0D8_ADDRESS 0x01F0D8u
#define tv_picture_tiles_ADDRESS 0x01F118u
#define sewer_water_tiles_ADDRESS 0x01F242u
#define screen_picture_tiles_ADDRESS 0x01F970u
#define screen_picture_colours_ADDRESS 0x01FAA8u
#define foot_car_tiles_ADDRESS 0x01FBE0u
#define foot_car_tiles_b_ADDRESS 0x01FBF0u
#define foot_car_tiles_2_ADDRESS 0x01FC00u
#define foot_car_tiles_2b_ADDRESS 0x01FC10u
#define sprite_lists_021920_ADDRESS 0x021920u
#define animations_0231F8_ADDRESS 0x0231F8u
#define sprite_lists_02422C_ADDRESS 0x02422Cu
#define animations_025CD4_ADDRESS 0x025CD4u
#define sprite_lists_026CBE_ADDRESS 0x026CBEu
#define animations_02883E_ADDRESS 0x02883Eu
#define sprite_lists_02983C_ADDRESS 0x02983Cu
#define kidnap_picture_animations_ADDRESS 0x02BA9Au
#define april_hug_animations_ADDRESS 0x02C640u
#define april_hug_animations_2_ADDRESS 0x02C650u
#define skateboard_crash_picture_animations_ADDRESS 0x02CD74u
#define splinter_picture_tiles_ADDRESS 0x02D164u
#define animations_02D796_ADDRESS 0x02D796u
#define drill_ceiling_area_1_animation_ADDRESS 0x02D7BAu
#define drill_ceiling_animation_ADDRESS 0x02D7F8u
#define drill_pod_animation_ADDRESS 0x02D836u
#define shredder_appears_animation_ADDRESS 0x02D842u
#define shredder_grabs_april_animation_ADDRESS 0x02D84Cu
#define shredder_carries_april_animation_ADDRESS 0x02D88Eu
#define april_standing_animation_ADDRESS 0x02D8AEu
#define turtle_van_animation_ADDRESS 0x02D8C8u
#define turtle_van_rear_animation_ADDRESS 0x02D8E2u
#define opening_manhole_cover_animation_ADDRESS 0x02DA26u
#define opening_street_post_animation_ADDRESS 0x02DA30u
#define opening_turtles_leap_out_animation_ADDRESS 0x02DA3Au
#define opening_turtles_ready_animation_ADDRESS 0x02DAB8u
#define opening_smoke_animation_ADDRESS 0x02DAD2u
#define opening_smoke_and_turtle_animation_ADDRESS 0x02DADCu
#define splinter_struggles_animation_ADDRESS 0x02DAE6u
#define splinter_tied_up_animation_ADDRESS 0x02DB0Au
#define splinter_freed_animation_ADDRESS 0x02DB1Eu
#define splinter_stretches_animation_ADDRESS 0x02DB28u
#define april_on_rooftop_animation_ADDRESS 0x02DB4Cu
#define technodrome_lift_animation_ADDRESS 0x02DB6Cu
#define hurry_arrow_animation_ADDRESS 0x02DB76u
#define hurry_arrow_down_animation_ADDRESS 0x02DBAEu
#define april_kidnapped_picture_animation_ADDRESS 0x02DC36u
#define splinter_thanks_animation_ADDRESS 0x02DEE6u
#define intro_turtle_lands_animation_ADDRESS 0x02DF32u
#define unexplained_02DF52_ADDRESS 0x02DF52u
#define animations_02DF54_ADDRESS 0x02DF54u
#define turtles_fall_in_animation_ADDRESS 0x02E0ECu
#define car_street_bystanders_animation_ADDRESS 0x02E11Au
#define turtle_cheer_animation_ADDRESS 0x02E134u
#define april_tied_up_picture_animation_ADDRESS 0x02E162u
#define unexplained_02E16E_ADDRESS 0x02E16Eu
#define animations_02E192_ADDRESS 0x02E192u
#define turtle_jump_in_animation_ADDRESS 0x02E1C0u
#define sprite_lists_02E1E0_ADDRESS 0x02E1E0u
#define common_shadow_ADDRESS 0x02E928u
#define opening_portraits_ADDRESS 0x02FDC0u
#define intro_rooftop_animations_ADDRESS 0x030B16u
#define animations_030B1A_ADDRESS 0x030B1Au
#define rooftop_picture_tiles_ADDRESS 0x030B26u
#define intro_rooftop_turtle_animations_ADDRESS 0x030D7Eu
#define intro_object_animations_ADDRESS 0x030DFCu
#define described_table_030E00_ADDRESS 0x030E00u
#define layers_031280_ADDRESS 0x031280u
#define splinter_picture_blocks_ADDRESS 0x031380u
#define unexplained_0314C3_ADDRESS 0x0314C3u
#define layers_0314C4_ADDRESS 0x0314C4u
#define villains_picture_blocks_ADDRESS 0x0315C4u
#define unexplained_03163B_ADDRESS 0x03163Bu
#define layers_03163C_ADDRESS 0x03163Cu
#define villains_picture_blocks_2_ADDRESS 0x03173Cu
#define unexplained_032311_ADDRESS 0x032311u
#define fire_blocks_ADDRESS 0x032312u
#define unexplained_032C9B_ADDRESS 0x032C9Bu
#define layers_032C9C_ADDRESS 0x032C9Cu
#define unexplained_03372C_ADDRESS 0x03372Cu
#define layers_03373C_ADDRESS 0x03373Cu
#define unexplained_034A29_ADDRESS 0x034A29u
#define layers_034A2A_ADDRESS 0x034A2Au
#define unexplained_035241_ADDRESS 0x035241u
#define layers_035242_ADDRESS 0x035242u
#define unexplained_03AB05_ADDRESS 0x03AB05u
#define layers_03AB06_ADDRESS 0x03AB06u
#define unexplained_03E989_ADDRESS 0x03E989u
#define layers_03E98A_ADDRESS 0x03E98Au
#define opening_layer_b_blocks_ADDRESS 0x03EB8Au
#define unexplained_03F195_ADDRESS 0x03F195u
#define layers_03F196_ADDRESS 0x03F196u
#define opening_layer_a_blocks_ADDRESS 0x03F796u
#define unexplained_044C8F_ADDRESS 0x044C8Fu
#define layers_044C90_ADDRESS 0x044C90u
#define unexplained_0458F7_ADDRESS 0x0458F7u
#define layers_0458F8_ADDRESS 0x0458F8u
#define technodrome_screen_blocks_ADDRESS 0x0459F8u
#define technodrome_screen_map_ADDRESS 0x045D4Au
#define technodrome_screen_blocks_2_ADDRESS 0x045E4Au
#define unexplained_04618B_ADDRESS 0x04618Bu
#define layers_04618C_ADDRESS 0x04618Cu
#define intro_roof_blocks_ADDRESS 0x04628Cu
#define april_hug_map_ADDRESS 0x046622u
#define april_hug_map_2_ADDRESS 0x046722u
#define april_hug_blocks_ADDRESS 0x046822u
#define unexplained_046C51_ADDRESS 0x046C51u
#define layers_046C52_ADDRESS 0x046C52u
#define april_hug_first_blocks_ADDRESS 0x046D52u
#define sprite_sizes_ADDRESS 0x04716Eu
#define area_spawn_lists_ADDRESS 0x04796Au
#define area_spawn_scrolls_ADDRESS 0x047992u
#define area_spawn_starts_ADDRESS 0x0479E2u
#define quarter_sine_ADDRESS 0x047D50u
#define quarter_sine_last_ADDRESS 0x047D8Fu
#define direction_rows_ADDRESS 0x047DF6u
#define direction_table_ADDRESS 0x047E36u
#define code_table_0489C6_ADDRESS 0x0489C6u
#define second_player_through_a_stale_pointer_ADDRESS 0x048A98u
#define code_table_04900A_ADDRESS 0x04900Au
#define code_table_049068_ADDRESS 0x049068u
#define code_table_0490C6_ADDRESS 0x0490C6u
#define code_table_04915C_ADDRESS 0x04915Cu
#define code_table_0491E8_ADDRESS 0x0491E8u
#define indexed_table_of_32_bit_values_049240_ADDRESS 0x049240u
#define animations_049258_ADDRESS 0x049258u
#define indexed_table_of_32_bit_values_0492F8_ADDRESS 0x0492F8u
#define indexed_table_of_32_bit_values_049350_ADDRESS 0x049350u
#define animations_049358_ADDRESS 0x049358u
#define indexed_table_of_32_bit_values_049414_ADDRESS 0x049414u
#define indexed_table_of_32_bit_values_0494D2_ADDRESS 0x0494D2u
#define indexed_table_of_32_bit_values_049568_ADDRESS 0x049568u
#define indexed_table_of_32_bit_values_0495D6_ADDRESS 0x0495D6u
#define indexed_table_of_32_bit_values_04965A_ADDRESS 0x04965Au
#define indexed_table_of_32_bit_values_049796_ADDRESS 0x049796u
#define indexed_table_of_32_bit_values_0498A6_ADDRESS 0x0498A6u
#define indexed_table_of_32_bit_values_049940_ADDRESS 0x049940u
#define indexed_table_of_32_bit_values_0499B0_ADDRESS 0x0499B0u
#define indexed_table_of_32_bit_values_049A12_ADDRESS 0x049A12u
#define indexed_table_of_32_bit_values_049A5E_ADDRESS 0x049A5Eu
#define indexed_table_of_32_bit_values_049A9A_ADDRESS 0x049A9Au
#define indexed_table_of_32_bit_values_049AD6_ADDRESS 0x049AD6u
#define indexed_table_of_32_bit_values_049B12_ADDRESS 0x049B12u
#define enemy_jump_kick_arc_ADDRESS 0x049B22u
#define indexed_table_of_32_bit_values_049BC8_ADDRESS 0x049BC8u
#define enemy_long_jump_kick_arc_ADDRESS 0x049BD8u
#define indexed_table_of_32_bit_values_049C82_ADDRESS 0x049C82u
#define indexed_table_of_32_bit_values_049DE8_ADDRESS 0x049DE8u
#define indexed_table_of_32_bit_values_049E1C_ADDRESS 0x049E1Cu
#define indexed_table_of_32_bit_values_049E40_ADDRESS 0x049E40u
#define indexed_table_of_32_bit_values_049E64_ADDRESS 0x049E64u
#define indexed_table_of_32_bit_values_049E88_ADDRESS 0x049E88u
#define enemy_killed_back_arc_ADDRESS 0x049EA0u
#define indexed_table_of_32_bit_values_049FC4_ADDRESS 0x049FC4u
#define enemy_killed_forward_arc_ADDRESS 0x049FDCu
#define indexed_table_of_32_bit_values_04A100_ADDRESS 0x04A100u
#define enemy_knocked_forward_arc_ADDRESS 0x04A114u
#define indexed_table_of_32_bit_values_04A1E0_ADDRESS 0x04A1E0u
#define thrown_by_donatello_placement_ADDRESS 0x04A1FCu
#define record_04A332_ADDRESS 0x04A332u
#define indexed_table_of_32_bit_values_04A374_ADDRESS 0x04A374u
#define thrown_by_michelangelo_placement_ADDRESS 0x04A390u
#define record_04A3EE_ADDRESS 0x04A3EEu
#define indexed_table_of_32_bit_values_04A432_ADDRESS 0x04A432u
#define thrown_by_raphael_placement_ADDRESS 0x04A44Eu
#define record_04A4AC_ADDRESS 0x04A4ACu
#define indexed_table_of_32_bit_values_04A4F0_ADDRESS 0x04A4F0u
#define thrown_by_leonardo_placement_ADDRESS 0x04A50Cu
#define record_04A56A_ADDRESS 0x04A56Au
#define indexed_table_of_32_bit_values_04A5AE_ADDRESS 0x04A5AEu
#define enemy_knocked_back_arc_ADDRESS 0x04A5C2u
#define indexed_table_of_32_bit_values_04A6BE_ADDRESS 0x04A6BEu
#define indexed_table_of_32_bit_values_04A710_ADDRESS 0x04A710u
#define code_table_04A738_ADDRESS 0x04A738u
#define code_table_04A7DE_ADDRESS 0x04A7DEu
#define animations_04A7E6_ADDRESS 0x04A7E6u
#define sprite_lists_04A7F0_ADDRESS 0x04A7F0u
#define code_table_04A816_ADDRESS 0x04A816u
#define animations_04A81E_ADDRESS 0x04A81Eu
#define code_table_04A866_ADDRESS 0x04A866u
#define animations_04A86E_ADDRESS 0x04A86Eu
#define sprite_lists_04A878_ADDRESS 0x04A878u
#define code_table_04A89C_ADDRESS 0x04A89Cu
#define sprite_lists_04A8CE_ADDRESS 0x04A8CEu
#define code_table_04A954_ADDRESS 0x04A954u
#define animations_04A95C_ADDRESS 0x04A95Cu
#define sprite_lists_04A96E_ADDRESS 0x04A96Eu
#define code_table_04A9CA_ADDRESS 0x04A9CAu
#define animations_04A9DA_ADDRESS 0x04A9DAu
#define falling_debris_animation_ADDRESS 0x04AA16u
#define sprite_lists_04AA3A_ADDRESS 0x04AA3Au
#define code_table_04AB3A_ADDRESS 0x04AB3Au
#define motorbike_rider_record_ADDRESS 0x04AB4Eu
#define motorbike_wheelie_rider_record_ADDRESS 0x04AB56u
#define code_table_04AC94_ADDRESS 0x04AC94u
#define code_table_04AD20_ADDRESS 0x04AD20u
#define indexed_table_of_32_bit_values_04ADB8_ADDRESS 0x04ADB8u
#define animations_04ADC0_ADDRESS 0x04ADC0u
#define indexed_table_of_32_bit_values_04ADFA_ADDRESS 0x04ADFAu
#define indexed_table_of_32_bit_values_04AE84_ADDRESS 0x04AE84u
#define indexed_table_of_32_bit_values_04AF30_ADDRESS 0x04AF30u
#define indexed_table_of_32_bit_values_04AFCA_ADDRESS 0x04AFCAu
#define indexed_table_of_32_bit_values_04B03E_ADDRESS 0x04B03Eu
#define animations_04B046_ADDRESS 0x04B046u
#define indexed_table_of_32_bit_values_04B080_ADDRESS 0x04B080u
#define indexed_table_of_32_bit_values_04B13A_ADDRESS 0x04B13Au
#define indexed_table_of_32_bit_values_04B244_ADDRESS 0x04B244u
#define indexed_table_of_32_bit_values_04B2E6_ADDRESS 0x04B2E6u
#define indexed_table_of_32_bit_values_04B322_ADDRESS 0x04B322u
#define indexed_table_of_32_bit_values_04B35E_ADDRESS 0x04B35Eu
#define enemy_leaping_slash_sword_arc_ADDRESS 0x04B36Eu
#define indexed_table_of_32_bit_values_04B436_ADDRESS 0x04B436u
#define enemy_jump_slash_sword_arc_ADDRESS 0x04B446u
#define indexed_table_of_32_bit_values_04B4EC_ADDRESS 0x04B4ECu
#define enemy_flying_slash_sword_arc_ADDRESS 0x04B4FCu
#define indexed_table_of_32_bit_values_04B5E2_ADDRESS 0x04B5E2u
#define indexed_table_of_32_bit_values_04B63E_ADDRESS 0x04B63Eu
#define indexed_table_of_32_bit_values_04B662_ADDRESS 0x04B662u
#define indexed_table_of_32_bit_values_04B686_ADDRESS 0x04B686u
#define indexed_table_of_32_bit_values_04B6AA_ADDRESS 0x04B6AAu
#define indexed_table_of_32_bit_values_04B6CE_ADDRESS 0x04B6CEu
#define indexed_table_of_32_bit_values_04B6F2_ADDRESS 0x04B6F2u
#define code_table_04B72E_ADDRESS 0x04B72Eu
#define whip_arm_x_offsets_ADDRESS 0x04B776u
#define whip_arm_animations_2_ADDRESS 0x04B790u
#define whip_arm_x_offsets_2_ADDRESS 0x04B7C4u
#define code_table_04B9B6_ADDRESS 0x04B9B6u
#define code_table_04BA42_ADDRESS 0x04BA42u
#define code_table_04BB1E_ADDRESS 0x04BB1Eu
#define code_table_04BBFA_ADDRESS 0x04BBFAu
#define code_table_04BC90_ADDRESS 0x04BC90u
#define code_table_04BD18_ADDRESS 0x04BD18u
#define code_table_04BDFA_ADDRESS 0x04BDFAu
#define code_table_04BEF6_ADDRESS 0x04BEF6u
#define code_table_04BFBE_ADDRESS 0x04BFBEu
#define rocksteady_bullet_aims_ADDRESS 0x04BFD2u
#define rocksteady_bullet_aims_2_ADDRESS 0x04BFD4u
#define code_table_04C0A6_ADDRESS 0x04C0A6u
#define code_table_04C106_ADDRESS 0x04C106u
#define code_table_04C174_ADDRESS 0x04C174u
#define code_table_04C1FC_ADDRESS 0x04C1FCu
#define code_table_04C258_ADDRESS 0x04C258u
#define code_table_04C31A_ADDRESS 0x04C31Au
#define flash_colour_past_its_table_ADDRESS 0x04C3F2u
#define code_table_04C400_ADDRESS 0x04C400u
#define animations_04C410_ADDRESS 0x04C410u
#define kicked_door_animation_ADDRESS 0x04C41Au
#define enemy_step_down_animation_ADDRESS 0x04C444u
#define sprite_lists_04C44E_ADDRESS 0x04C44Eu
#define record_04C4C6_ADDRESS 0x04C4C6u
#define code_table_04C564_ADDRESS 0x04C564u
#define animations_04C56C_ADDRESS 0x04C56Cu
#define sprite_lists_04C5B4_ADDRESS 0x04C5B4u
#define code_table_04C64E_ADDRESS 0x04C64Eu
#define animations_04C65E_ADDRESS 0x04C65Eu
#define elevator_doors_animation_ADDRESS 0x04C668u
#define sprite_lists_04C688_ADDRESS 0x04C688u
#define record_04C6C8_ADDRESS 0x04C6C8u
#define elevator_door_record_ADDRESS 0x04C6D0u
#define elevator_door_record_2_ADDRESS 0x04C6D8u
#define elevator_stops_ADDRESS 0x04C6E0u
#define elevator_stops_2_ADDRESS 0x04C6E4u
#define code_table_04C79C_ADDRESS 0x04C79Cu
#define animations_04C7B2_ADDRESS 0x04C7B2u
#define sprite_lists_04C7F4_ADDRESS 0x04C7F4u
#define code_table_04C87C_ADDRESS 0x04C87Cu
#define thrown_tire_record_ADDRESS 0x04C894u
#define code_table_04C99E_ADDRESS 0x04C99Eu
#define thrown_manhole_cover_record_ADDRESS 0x04C9B6u
#define manhole_soldier_hole_record_ADDRESS 0x04C9BEu
#define code_table_04CAEA_ADDRESS 0x04CAEAu
#define animations_04CAFE_ADDRESS 0x04CAFEu
#define enemy_drop_animation_ADDRESS 0x04CB1Eu
#define sprite_lists_04CB34_ADDRESS 0x04CB34u
#define record_04CBAC_ADDRESS 0x04CBACu
#define object_tile_block_offsets_ADDRESS 0x04CBB4u
#define object_tile_blocks_ADDRESS 0x04CBC0u
#define code_table_04CD00_ADDRESS 0x04CD00u
#define animations_04CD10_ADDRESS 0x04CD10u
#define window_soldier_dive_animation_ADDRESS 0x04CD26u
#define record_04CD3C_ADDRESS 0x04CD3Cu
#define object_tile_block_pairs_ADDRESS 0x04CD44u
#define code_table_04CE52_ADDRESS 0x04CE52u
#define animations_04CE5A_ADDRESS 0x04CE5Au
#define glass_shards_animation_ADDRESS 0x04CE7Au
#define sprite_lists_04CE9A_ADDRESS 0x04CE9Au
#define code_table_04CF2A_ADDRESS 0x04CF2Au
#define animations_04CF3E_ADDRESS 0x04CF3Eu
#define enemy_hop_up_animation_ADDRESS 0x04CF5Eu
#define enemy_hang_in_animation_ADDRESS 0x04CF68u
#define code_table_04D05E_ADDRESS 0x04D05Eu
#define code_table_04D0A8_ADDRESS 0x04D0A8u
#define animations_04D0BC_ADDRESS 0x04D0BCu
#define sewer_ripple_swim_animation_ADDRESS 0x04D0D6u
#define sewer_ripple_leap_out_animation_ADDRESS 0x04D100u
#define sprite_lists_04D13E_ADDRESS 0x04D13Eu
#define record_04D1A6_ADDRESS 0x04D1A6u
#define code_table_04D2C8_ADDRESS 0x04D2C8u
#define code_table_04D344_ADDRESS 0x04D344u
#define animations_04D35C_ADDRESS 0x04D35Cu
#define bomber_throw_animation_ADDRESS 0x04D3A8u
#define sprite_lists_04D3BE_ADDRESS 0x04D3BEu
#define record_04D466_ADDRESS 0x04D466u
#define code_table_04D544_ADDRESS 0x04D544u
#define rocket_shot_record_ADDRESS 0x04D558u
#define dropped_rocket_shot_record_ADDRESS 0x04D55Eu
#define code_table_04D6A4_ADDRESS 0x04D6A4u
#define double_door_piece_record_ADDRESS 0x04D6B8u
#define double_door_piece_record_2_ADDRESS 0x04D6C0u
#define code_table_04D74C_ADDRESS 0x04D74Cu
#define technodrome_door_soldier_record_ADDRESS 0x04D75Cu
#define code_table_04D810_ADDRESS 0x04D810u
#define code_table_04D8C0_ADDRESS 0x04D8C0u
#define enemy_kick_in_arc_ADDRESS 0x04D8D4u
#define code_table_04D9BA_ADDRESS 0x04D9BAu
#define code_table_04DA0C_ADDRESS 0x04DA0Cu
#define code_table_04DB6A_ADDRESS 0x04DB6Au
#define code_table_04DBE8_ADDRESS 0x04DBE8u
#define code_table_04DC82_ADDRESS 0x04DC82u
#define indexed_table_of_32_bit_values_04DD40_ADDRESS 0x04DD40u
#define animations_04DD48_ADDRESS 0x04DD48u
#define indexed_table_of_32_bit_values_04DD82_ADDRESS 0x04DD82u
#define indexed_table_of_32_bit_values_04DDC0_ADDRESS 0x04DDC0u
#define indexed_table_of_32_bit_values_04DE6C_ADDRESS 0x04DE6Cu
#define indexed_table_of_32_bit_values_04DEEE_ADDRESS 0x04DEEEu
#define indexed_table_of_32_bit_values_04DF70_ADDRESS 0x04DF70u
#define animations_04DF78_ADDRESS 0x04DF78u
#define indexed_table_of_32_bit_values_04DFB2_ADDRESS 0x04DFB2u
#define indexed_table_of_32_bit_values_04DFF0_ADDRESS 0x04DFF0u
#define animations_04DFF8_ADDRESS 0x04DFF8u
#define indexed_table_of_32_bit_values_04E0B4_ADDRESS 0x04E0B4u
#define indexed_table_of_32_bit_values_04E1C4_ADDRESS 0x04E1C4u
#define indexed_table_of_32_bit_values_04E260_ADDRESS 0x04E260u
#define indexed_table_of_32_bit_values_04E2D0_ADDRESS 0x04E2D0u
#define indexed_table_of_32_bit_values_04E342_ADDRESS 0x04E342u
#define spear_shot_record_ADDRESS 0x04E34Eu
#define indexed_table_of_32_bit_values_04E3D0_ADDRESS 0x04E3D0u
#define indexed_table_of_32_bit_values_04E40C_ADDRESS 0x04E40Cu
#define indexed_table_of_32_bit_values_04E448_ADDRESS 0x04E448u
#define enemy_jump_kick_knife_arc_ADDRESS 0x04E458u
#define indexed_table_of_32_bit_values_04E4FE_ADDRESS 0x04E4FEu
#define knife_shot_record_ADDRESS 0x04E50Au
#define indexed_table_of_32_bit_values_04E572_ADDRESS 0x04E572u
#define indexed_table_of_32_bit_values_04E596_ADDRESS 0x04E596u
#define indexed_table_of_32_bit_values_04E5BA_ADDRESS 0x04E5BAu
#define indexed_table_of_32_bit_values_04E5DE_ADDRESS 0x04E5DEu
#define indexed_table_of_32_bit_values_04E602_ADDRESS 0x04E602u
#define indexed_table_of_32_bit_values_04E626_ADDRESS 0x04E626u
#define indexed_table_of_32_bit_values_04E64A_ADDRESS 0x04E64Au
#define indexed_table_of_32_bit_values_04E66E_ADDRESS 0x04E66Eu
#define code_table_04E686_ADDRESS 0x04E686u
#define code_table_04E71A_ADDRESS 0x04E71Au
#define indexed_table_of_32_bit_values_04E7BA_ADDRESS 0x04E7BAu
#define animations_04E7C2_ADDRESS 0x04E7C2u
#define indexed_table_of_32_bit_values_04E800_ADDRESS 0x04E800u
#define indexed_table_of_32_bit_values_04E83E_ADDRESS 0x04E83Eu
#define indexed_table_of_32_bit_values_04E906_ADDRESS 0x04E906u
#define indexed_table_of_32_bit_values_04E99E_ADDRESS 0x04E99Eu
#define indexed_table_of_32_bit_values_04EA12_ADDRESS 0x04EA12u
#define animations_04EA1A_ADDRESS 0x04EA1Au
#define indexed_table_of_32_bit_values_04EA58_ADDRESS 0x04EA58u
#define indexed_table_of_32_bit_values_04EA96_ADDRESS 0x04EA96u
#define indexed_table_of_32_bit_values_04EAD4_ADDRESS 0x04EAD4u
#define indexed_table_of_32_bit_values_04EB6E_ADDRESS 0x04EB6Eu
#define indexed_table_of_32_bit_values_04EBE2_ADDRESS 0x04EBE2u
#define axe_shot_record_ADDRESS 0x04EBEEu
#define indexed_table_of_32_bit_values_04EC6A_ADDRESS 0x04EC6Au
#define indexed_table_of_32_bit_values_04ECA6_ADDRESS 0x04ECA6u
#define indexed_table_of_32_bit_values_04ECE2_ADDRESS 0x04ECE2u
#define gun_shot_record_ADDRESS 0x04ECEEu
#define indexed_table_of_32_bit_values_04ED6A_ADDRESS 0x04ED6Au
#define indexed_table_of_32_bit_values_04ED8E_ADDRESS 0x04ED8Eu
#define indexed_table_of_32_bit_values_04EDB2_ADDRESS 0x04EDB2u
#define indexed_table_of_32_bit_values_04EDD6_ADDRESS 0x04EDD6u
#define indexed_table_of_32_bit_values_04EDFA_ADDRESS 0x04EDFAu
#define code_table_04EE12_ADDRESS 0x04EE12u
#define code_table_04EEAC_ADDRESS 0x04EEACu
#define code_table_04EF40_ADDRESS 0x04EF40u
#define indexed_table_of_32_bit_values_04F082_ADDRESS 0x04F082u
#define shuriken_shot_record_ADDRESS 0x04F08Eu
#define indexed_table_of_32_bit_values_04F0F0_ADDRESS 0x04F0F0u
#define boomerang_shot_record_ADDRESS 0x04F104u
#define code_table_04F1BA_ADDRESS 0x04F1BAu
#define indexed_table_of_32_bit_values_04F296_ADDRESS 0x04F296u
#define rocksteady_close_attacks_ADDRESS 0x04F5BCu
#define rocksteady_far_attacks_ADDRESS 0x04F5C0u
#define indexed_table_of_32_bit_values_04F72E_ADDRESS 0x04F72Eu
#define indexed_table_of_32_bit_values_04F75C_ADDRESS 0x04F75Cu
#define indexed_table_of_32_bit_values_04F78A_ADDRESS 0x04F78Au
#define indexed_table_of_32_bit_values_04F7BE_ADDRESS 0x04F7BEu
#define indexed_table_of_32_bit_values_04F7FA_ADDRESS 0x04F7FAu
#define indexed_table_of_32_bit_values_04F828_ADDRESS 0x04F828u
#define indexed_table_of_32_bit_values_04F8A8_ADDRESS 0x04F8A8u
#define indexed_table_of_32_bit_values_04F92E_ADDRESS 0x04F92Eu
#define indexed_table_of_32_bit_values_04F9CC_ADDRESS 0x04F9CCu
#define rocksteady_shoot_around_aims_ADDRESS 0x04F9D4u
#define indexed_table_of_32_bit_values_04FACE_ADDRESS 0x04FACEu
#define indexed_table_of_32_bit_values_04FC10_ADDRESS 0x04FC10u
#define indexed_table_of_32_bit_values_04FCE0_ADDRESS 0x04FCE0u
#define indexed_table_of_32_bit_values_04FD58_ADDRESS 0x04FD58u
#define indexed_table_of_32_bit_values_04FD94_ADDRESS 0x04FD94u
#define rocksteady_shoot_animations_ADDRESS 0x04FDA8u
#define unexplained_04FDAC_ADDRESS 0x04FDACu
#define animations_04FDB0_ADDRESS 0x04FDB0u
#define unexplained_04FDB4_ADDRESS 0x04FDB4u
#define indexed_table_of_32_bit_values_04FE4E_ADDRESS 0x04FE4Eu
#define indexed_table_of_32_bit_values_04FEB6_ADDRESS 0x04FEB6u
#define indexed_table_of_32_bit_values_04FEEE_ADDRESS 0x04FEEEu
#define indexed_table_of_32_bit_values_04FF26_ADDRESS 0x04FF26u
#define rocksteady_knocked_down_animations_ADDRESS 0x04FF3Au
#define rocksteady_knocked_down_animations_2_ADDRESS 0x04FF52u
#define indexed_table_of_32_bit_values_050082_ADDRESS 0x050082u
#define rocksteady_defeated_animations_ADDRESS 0x050092u
#define rocksteady_defeated_animations_2_ADDRESS 0x0500A2u
#define code_table_0501AE_ADDRESS 0x0501AEu
#define indexed_table_of_32_bit_values_050286_ADDRESS 0x050286u
#define bebop_attacks_ADDRESS 0x0505E2u
#define bebop_far_moves_ADDRESS 0x050602u
#define indexed_table_of_32_bit_values_05068A_ADDRESS 0x05068Au
#define indexed_table_of_32_bit_values_0506E4_ADDRESS 0x0506E4u
#define indexed_table_of_32_bit_values_05077C_ADDRESS 0x05077Cu
#define indexed_table_of_32_bit_values_0507FA_ADDRESS 0x0507FAu
#define indexed_table_of_32_bit_values_0508D8_ADDRESS 0x0508D8u
#define indexed_table_of_32_bit_values_050AE8_ADDRESS 0x050AE8u
#define indexed_table_of_32_bit_values_050B98_ADDRESS 0x050B98u
#define indexed_table_of_32_bit_values_050C74_ADDRESS 0x050C74u
#define bebop_shot_record_ADDRESS 0x050C7Cu
#define indexed_table_of_32_bit_values_050CF2_ADDRESS 0x050CF2u
#define indexed_table_of_32_bit_values_050D38_ADDRESS 0x050D38u
#define indexed_table_of_32_bit_values_050D84_ADDRESS 0x050D84u
#define enemy_bebop_knocked_down_arc_ADDRESS 0x050D98u
#define indexed_table_of_32_bit_values_050E72_ADDRESS 0x050E72u
#define enemy_bebop_defeated_arc_ADDRESS 0x050E82u
#define code_table_050F7C_ADDRESS 0x050F7Cu
#define indexed_table_of_32_bit_values_05100E_ADDRESS 0x05100Eu
#define indexed_table_of_32_bit_values_051340_ADDRESS 0x051340u
#define enemy_mouser_bite_arc_ADDRESS 0x051350u
#define code_table_05157C_ADDRESS 0x05157Cu
#define code_table_0515CC_ADDRESS 0x0515CCu
#define code_table_05161C_ADDRESS 0x05161Cu
#define code_table_051694_ADDRESS 0x051694u
#define code_table_051712_ADDRESS 0x051712u
#define indexed_table_of_32_bit_values_05175C_ADDRESS 0x05175Cu
#define animations_05176C_ADDRESS 0x05176Cu
#define indexed_table_of_32_bit_values_051836_ADDRESS 0x051836u
#define indexed_table_of_32_bit_values_0518BC_ADDRESS 0x0518BCu
#define indexed_table_of_32_bit_values_051974_ADDRESS 0x051974u
#define indexed_table_of_32_bit_values_0519CC_ADDRESS 0x0519CCu
#define enemy_mouser_break_down_arc_ADDRESS 0x0519D8u
#define indexed_table_of_32_bit_values_051A14_ADDRESS 0x051A14u
#define enemy_mouser_knocked_flying_arc_ADDRESS 0x051A20u
#define code_table_051AD2_ADDRESS 0x051AD2u
#define animations_051B26_ADDRESS 0x051B26u
#define unexplained_051B2E_ADDRESS 0x051B2Eu
#define animations_051B36_ADDRESS 0x051B36u
#define unexplained_051B3E_ADDRESS 0x051B3Eu
#define animations_051B46_ADDRESS 0x051B46u
#define unexplained_051B4E_ADDRESS 0x051B4Eu
#define code_table_051C2C_ADDRESS 0x051C2Cu
#define indexed_table_of_32_bit_values_051CB8_ADDRESS 0x051CB8u
#define rodney_attacks_ADDRESS 0x051F94u
#define indexed_table_of_32_bit_values_052002_ADDRESS 0x052002u
#define indexed_table_of_32_bit_values_052142_ADDRESS 0x052142u
#define indexed_table_of_32_bit_values_052234_ADDRESS 0x052234u
#define indexed_table_of_32_bit_values_052312_ADDRESS 0x052312u
#define laser_bolt_shot_record_ADDRESS 0x05232Au
#define indexed_table_of_32_bit_values_05242E_ADDRESS 0x05242Eu
#define whip_arm_shot_record_ADDRESS 0x05244Au
#define indexed_table_of_32_bit_values_05260C_ADDRESS 0x05260Cu
#define enemy_rodney_knocked_back_arc_ADDRESS 0x052618u
#define indexed_table_of_32_bit_values_0526B0_ADDRESS 0x0526B0u
#define enemy_rodney_destroyed_arc_ADDRESS 0x0526C0u
#define code_table_052778_ADDRESS 0x052778u
#define shredder_distance_moves_ADDRESS 0x052ACEu
#define indexed_table_of_32_bit_values_052CA2_ADDRESS 0x052CA2u
#define indexed_table_of_32_bit_values_052D52_ADDRESS 0x052D52u
#define shredder_clone_record_ADDRESS 0x052D5Au
#define indexed_table_of_32_bit_values_052DDC_ADDRESS 0x052DDCu
#define indexed_table_of_32_bit_values_052E60_ADDRESS 0x052E60u
#define shredder_walk_animations_ADDRESS 0x052E74u
#define shredder_walk_hurt_animations_ADDRESS 0x052E7Cu
#define indexed_table_of_32_bit_values_052FE4_ADDRESS 0x052FE4u
#define indexed_table_of_32_bit_values_05307C_ADDRESS 0x05307Cu
#define indexed_table_of_32_bit_values_0530F6_ADDRESS 0x0530F6u
#define indexed_table_of_32_bit_values_0531BA_ADDRESS 0x0531BAu
#define shredder_piece_shot_record_ADDRESS 0x0531C2u
#define indexed_table_of_32_bit_values_05324E_ADDRESS 0x05324Eu
#define indexed_table_of_32_bit_values_0532E0_ADDRESS 0x0532E0u
#define indexed_table_of_32_bit_values_053348_ADDRESS 0x053348u
#define enemy_shredder_defeated_arc_ADDRESS 0x05335Cu
#define indexed_table_of_32_bit_values_05344C_ADDRESS 0x05344Cu
#define indexed_table_of_32_bit_values_0534D0_ADDRESS 0x0534D0u
#define indexed_table_of_32_bit_values_053530_ADDRESS 0x053530u
#define indexed_table_of_32_bit_values_05357A_ADDRESS 0x05357Au
#define indexed_table_of_32_bit_values_0535C4_ADDRESS 0x0535C4u
#define indexed_table_of_32_bit_values_05360E_ADDRESS 0x05360Eu
#define shredder_ray_shot_record_ADDRESS 0x053622u
#define body_boxes_ADDRESS 0x053756u
#define spawn_offsets_ADDRESS 0x053A5Eu
#define unexplained_053B6E_ADDRESS 0x053B6Eu
#define enemy_definitions_ADDRESS 0x053EFEu
#define sword_soldier_definition_ADDRESS 0x053F86u
#define hammer_soldier_definition_ADDRESS 0x05400Eu
#define knife_soldier_definition_ADDRESS 0x054096u
#define spear_soldier_definition_ADDRESS 0x05411Eu
#define axe_soldier_definition_ADDRESS 0x0541A6u
#define gun_soldier_definition_ADDRESS 0x05422Eu
#define boomerang_soldier_definition_ADDRESS 0x0542B6u
#define shuriken_soldier_definition_ADDRESS 0x05433Eu
#define unused_enemy_definitions_ADDRESS 0x0543C6u
#define table_loaded_by_lea_054746_ADDRESS 0x054746u
#define bebop_health_ADDRESS 0x054756u
#define rocksteady_health_ADDRESS 0x054766u
#define bebop_health_2_ADDRESS 0x054776u
#define shredder_health_ADDRESS 0x054786u
#define shredder_bar_health_ADDRESS 0x054796u
#define shredder_clone_health_ADDRESS 0x0547A6u
#define shredder_clone_bar_health_ADDRESS 0x0547B6u
#define animations_0547C6_ADDRESS 0x0547C6u
#define bomber_step_out_animation_ADDRESS 0x0547F8u
#define enemy_grab_animation_ADDRESS 0x054872u
#define enemy_run_animation_ADDRESS 0x054908u
#define enemy_crouch_walk_animation_ADDRESS 0x0549E4u
#define enemy_shuffle_start_animation_ADDRESS 0x054A18u
#define enemy_shuffle_step_animation_ADDRESS 0x054A38u
#define enemy_shuffle_turn_animation_ADDRESS 0x054A62u
#define enemy_dash_animation_ADDRESS 0x054A78u
#define enemy_crouch_animation_ADDRESS 0x054ACEu
#define enemy_leap_animation_ADDRESS 0x054ADAu
#define enemy_dive_in_uncurl_animation_ADDRESS 0x054AE4u
#define enemy_dive_in_curl_animation_ADDRESS 0x054AEEu
#define enemy_jump_kick_animation_ADDRESS 0x054B1Cu
#define enemy_flinch_back_animation_ADDRESS 0x054B4Eu
#define enemy_flinch_bent_animation_ADDRESS 0x054B82u
#define enemy_flinch_reel_animation_ADDRESS 0x054BACu
#define enemy_flinch_hunched_animation_ADDRESS 0x054BD6u
#define enemy_knocked_forward_animation_ADDRESS 0x054BECu
#define enemy_knocked_forward_fly_animation_ADDRESS 0x054BF6u
#define enemy_knocked_forward_lie_animation_ADDRESS 0x054C16u
#define enemy_knocked_back_animation_ADDRESS 0x054C54u
#define enemy_knocked_back_fly_animation_ADDRESS 0x054C6Au
#define enemy_knocked_back_lie_animation_ADDRESS 0x054C94u
#define enemy_jab_animation_ADDRESS 0x054CC8u
#define enemy_chop_animation_ADDRESS 0x054D02u
#define enemy_swing_animation_ADDRESS 0x054D3Cu
#define enemy_high_kick_animation_ADDRESS 0x054D76u
#define enemy_die_animation_ADDRESS 0x054DBAu
#define enemy_die_on_back_animation_ADDRESS 0x054E16u
#define enemy_killed_forward_animation_ADDRESS 0x054E62u
#define enemy_killed_forward_slide_down_animation_ADDRESS 0x054E82u
#define enemy_spin_animation_ADDRESS 0x054EACu
#define enemy_killed_back_slide_down_animation_ADDRESS 0x054ECCu
#define manhole_soldier_wait_animation_ADDRESS 0x054EECu
#define manhole_soldier_fly_animation_ADDRESS 0x054F20u
#define enemy_throw_low_animation_ADDRESS 0x054F36u
#define manhole_soldier_start_animation_ADDRESS 0x054F56u
#define rolling_manhole_cover_animation_ADDRESS 0x054F60u
#define sprite_lists_054F72_ADDRESS 0x054F72u
#define animations_055722_ADDRESS 0x055722u
#define enemy_sword_ready_animation_ADDRESS 0x055824u
#define enemy_flying_slash_sword_leap_animation_ADDRESS 0x055830u
#define enemy_flying_slash_sword_fly_animation_ADDRESS 0x055840u
#define enemy_sword_crouch_walk_animation_ADDRESS 0x055852u
#define enemy_jump_slash_sword_animation_ADDRESS 0x055878u
#define enemy_flinch_back_sword_animation_ADDRESS 0x0558AAu
#define enemy_flinch_bent_sword_animation_ADDRESS 0x0558DEu
#define enemy_flinch_reel_sword_animation_ADDRESS 0x055908u
#define enemy_flinch_hunched_sword_animation_ADDRESS 0x055932u
#define enemy_slash_sword_animation_ADDRESS 0x055948u
#define enemy_low_slash_sword_animation_ADDRESS 0x05598Cu
#define enemy_leaping_slash_sword_start_animation_ADDRESS 0x0559C8u
#define enemy_leaping_slash_sword_leap_animation_ADDRESS 0x0559DEu
#define enemy_leaping_slash_sword_fly_animation_ADDRESS 0x055A18u
#define enemy_throw_shuriken_animation_ADDRESS 0x055A24u
#define shuriken_spin_animation_ADDRESS 0x055A62u
#define shuriken_struck_animation_ADDRESS 0x055A9Eu
#define shuriken_fly_animation_ADDRESS 0x055AA8u
#define enemy_boomerang_throw_wind_up_animation_ADDRESS 0x055ABEu
#define enemy_boomerang_throw_aim_animation_ADDRESS 0x055AF2u
#define enemy_boomerang_throw_wait_animation_ADDRESS 0x055B3Au
#define boomerang_start_animation_ADDRESS 0x055B96u
#define boomerang_struck_animation_ADDRESS 0x055C2Cu
#define enemy_spear_soldier_animation_ADDRESS 0x055C60u
#define enemy_back_off_spear_animation_ADDRESS 0x055D62u
#define enemy_spear_thrust_animation_ADDRESS 0x055D6Cu
#define enemy_spear_throw_animation_ADDRESS 0x055DB6u
#define spear_start_animation_ADDRESS 0x055DF4u
#define spear_struck_animation_ADDRESS 0x055E06u
#define enemy_flinch_back_spear_animation_ADDRESS 0x055E30u
#define enemy_flinch_bent_spear_animation_ADDRESS 0x055E64u
#define enemy_flinch_reel_spear_animation_ADDRESS 0x055E8Eu
#define enemy_flinch_hunched_spear_animation_ADDRESS 0x055EB8u
#define sprite_lists_055ECE_ADDRESS 0x055ECEu
#define animations_0569FE_ADDRESS 0x0569FEu
#define enemy_gun_soldier_animation_ADDRESS 0x056A08u
#define enemy_gun_swing_animation_ADDRESS 0x056AB4u
#define enemy_shoot_gun_animation_ADDRESS 0x056AF0u
#define enemy_gun_crouch_walk_animation_ADDRESS 0x056B1Eu
#define gun_shot_start_animation_ADDRESS 0x056B2Au
#define gun_shot_struck_animation_ADDRESS 0x056B3Cu
#define gun_shot_burst_animation_ADDRESS 0x056B5Cu
#define enemy_flinch_back_gun_animation_ADDRESS 0x056B7Cu
#define enemy_flinch_bent_gun_animation_ADDRESS 0x056BB0u
#define sprite_lists_056BDA_ADDRESS 0x056BDAu
#define animations_056F42_ADDRESS 0x056F42u
#define enemy_knife_crouch_animation_ADDRESS 0x057088u
#define enemy_knife_stab_animation_ADDRESS 0x0570AAu
#define enemy_throw_knife_animation_ADDRESS 0x0570DCu
#define knife_start_animation_ADDRESS 0x05711Au
#define knife_struck_animation_ADDRESS 0x05712Cu
#define enemy_high_kick_knife_animation_ADDRESS 0x05714Cu
#define enemy_jump_kick_knife_animation_ADDRESS 0x057190u
#define enemy_flinch_back_knife_animation_ADDRESS 0x0571C2u
#define enemy_flinch_bent_knife_animation_ADDRESS 0x0571F6u
#define enemy_flinch_reel_knife_animation_ADDRESS 0x057220u
#define enemy_flinch_hunched_knife_animation_ADDRESS 0x05724Au
#define sprite_lists_057260_ADDRESS 0x057260u
#define animations_0575A0_ADDRESS 0x0575A0u
#define enemy_axe_soldier_animation_ADDRESS 0x0575AAu
#define enemy_throw_axes_animation_ADDRESS 0x0576ACu
#define enemy_axe_crouch_walk_animation_ADDRESS 0x0576E4u
#define axe_start_animation_ADDRESS 0x0576F0u
#define enemy_high_kick_axe_animation_ADDRESS 0x05773Au
#define enemy_flinch_back_axe_animation_ADDRESS 0x05777Eu
#define enemy_flinch_bent_axe_animation_ADDRESS 0x0577B2u
#define enemy_flinch_reel_axe_animation_ADDRESS 0x0577DCu
#define sprite_lists_057806_ADDRESS 0x057806u
#define animations_057B46_ADDRESS 0x057B46u
#define enemy_hammer_walk_animation_ADDRESS 0x057BF2u
#define enemy_smash_hammer_animation_ADDRESS 0x057BFEu
#define enemy_flinch_back_hammer_animation_ADDRESS 0x057C62u
#define enemy_flinch_bent_hammer_animation_ADDRESS 0x057C96u
#define sprite_lists_057CD6_ADDRESS 0x057CD6u
#define animations_058076_ADDRESS 0x058076u
#define enemy_mouser_animation_ADDRESS 0x05811Au
#define enemy_mouser_hop_animation_ADDRESS 0x058134u
#define enemy_mouser_tumble_animation_ADDRESS 0x058146u
#define enemy_mouser_hop_up_animation_ADDRESS 0x0581BAu
#define enemy_mouser_fall_animation_ADDRESS 0x0581C4u
#define enemy_mouser_fly_and_crash_animation_ADDRESS 0x0581D0u
#define sprite_lists_0582B4_ADDRESS 0x0582B4u
#define animations_0585C4_ADDRESS 0x0585C4u
#define enemy_rodney_back_off_animation_ADDRESS 0x0585DEu
#define enemy_rodney_slide_to_stop_animation_ADDRESS 0x05860Cu
#define enemy_rodney_rush_animation_ADDRESS 0x058636u
#define enemy_rodney_rush_dust_animation_ADDRESS 0x058660u
#define enemy_rodney_knocked_back_start_animation_ADDRESS 0x05877Eu
#define enemy_rodney_knocked_back_fall_animation_ADDRESS 0x058788u
#define enemy_rodney_destroyed_animation_ADDRESS 0x058794u
#define enemy_rodney_destroyed_fall_animation_ADDRESS 0x05879Eu
#define enemy_rodney_shoot_turn_animation_ADDRESS 0x0587FAu
#define enemy_rodney_shoot_fire_animation_ADDRESS 0x058856u
#define laser_bolt_start_animation_ADDRESS 0x058880u
#define laser_bolt_fly_animation_ADDRESS 0x058892u
#define enemy_rodney_whip_wind_up_animation_ADDRESS 0x0588BCu
#define enemy_rodney_whip_throw_arm_animation_ADDRESS 0x058918u
#define enemy_rodney_enter_wait_animation_ADDRESS 0x058960u
#define enemy_rodney_enter_burst_out_animation_ADDRESS 0x0589A8u
#define enemy_rodney_enter_fall_animation_ADDRESS 0x0589CCu
#define sprite_lists_0589F6_ADDRESS 0x0589F6u
#define animations_059066_ADDRESS 0x059066u
#define enemy_rodney_whip_animation_ADDRESS 0x059150u
#define sprite_lists_0592C0_ADDRESS 0x0592C0u
#define animations_059618_ADDRESS 0x059618u
#define sprite_lists_05965A_ADDRESS 0x05965Au
#define animations_0596BA_ADDRESS 0x0596BAu
#define sprite_lists_0596FC_ADDRESS 0x0596FCu
#define animations_05979C_ADDRESS 0x05979Cu
#define sprite_lists_0597DE_ADDRESS 0x0597DEu
#define animations_05983E_ADDRESS 0x05983Eu
#define sprite_lists_05988C_ADDRESS 0x05988Cu
#define animations_0598E4_ADDRESS 0x0598E4u
#define sprite_lists_059A18_ADDRESS 0x059A18u
#define animations_059CB0_ADDRESS 0x059CB0u
#define enemy_rocksteady_animation_ADDRESS 0x059CBAu
#define enemy_rocksteady_shake_fist_animation_ADDRESS 0x059CC4u
#define enemy_rocksteady_raise_fist_animation_ADDRESS 0x059CFCu
#define enemy_rocksteady_laugh_animation_ADDRESS 0x059D58u
#define sprite_lists_059D9A_ADDRESS 0x059D9Au
#define animations_059FAA_ADDRESS 0x059FAAu
#define enemy_rocksteady_gun_butt_animation_ADDRESS 0x059FD2u
#define sprite_lists_059FFA_ADDRESS 0x059FFAu
#define animations_05A08A_ADDRESS 0x05A08Au
#define enemy_rocksteady_put_gun_away_animation_ADDRESS 0x05A0D2u
#define sprite_lists_05A1E0_ADDRESS 0x05A1E0u
#define animations_05A430_ADDRESS 0x05A430u
#define sprite_lists_05A472_ADDRESS 0x05A472u
#define animations_05A562_ADDRESS 0x05A562u
#define enemy_bebop_animation_ADDRESS 0x05A5AEu
#define enemy_bebop_raise_gun_animation_ADDRESS 0x05A5E6u
#define enemy_bebop_enter_drop_animation_ADDRESS 0x05A61Eu
#define enemy_bebop_enter_in_air_animation_ADDRESS 0x05A648u
#define sprite_lists_05A65E_ADDRESS 0x05A65Eu
#define animations_05A77E_ADDRESS 0x05A77Eu
#define bebop_shot_animation_ADDRESS 0x05A7CAu
#define sprite_lists_05A838_ADDRESS 0x05A838u
#define animations_05A930_ADDRESS 0x05A930u
#define sprite_lists_05A97E_ADDRESS 0x05A97Eu
#define animations_05A9BE_ADDRESS 0x05A9BEu
#define sprite_lists_05AA16_ADDRESS 0x05AA16u
#define animations_05AAF6_ADDRESS 0x05AAF6u
#define enemy_bebop_flinch_animation_ADDRESS 0x05AB38u
#define enemy_bebop_knocked_down_start_animation_ADDRESS 0x05AB58u
#define enemy_bebop_knocked_down_fall_animation_ADDRESS 0x05AB6Eu
#define enemy_bebop_knocked_down_lie_animation_ADDRESS 0x05AB7Au
#define enemy_bebop_defeated_start_animation_ADDRESS 0x05AB9Au
#define enemy_bebop_defeated_fall_animation_ADDRESS 0x05ABBAu
#define sprite_lists_05ABC6_ADDRESS 0x05ABC6u
#define animations_05AD06_ADDRESS 0x05AD06u
#define enemy_bebop_pincer_animation_ADDRESS 0x05AD12u
#define sprite_lists_05AD28_ADDRESS 0x05AD28u
#define animations_05AD40_ADDRESS 0x05AD40u
#define sprite_lists_05AD92_ADDRESS 0x05AD92u
#define animations_05B032_ADDRESS 0x05B032u
#define enemy_shredder_animation_ADDRESS 0x05B03Cu
#define sprite_lists_05B046_ADDRESS 0x05B046u
#define animations_05B07E_ADDRESS 0x05B07Eu
#define sprite_lists_05B102_ADDRESS 0x05B102u
#define animations_05B22A_ADDRESS 0x05B22Au
#define sprite_lists_05B2AE_ADDRESS 0x05B2AEu
#define animations_05B3AE_ADDRESS 0x05B3AEu
#define sprite_lists_05B3F2_ADDRESS 0x05B3F2u
#define animations_05B4F2_ADDRESS 0x05B4F2u
#define enemy_shredder_flinch_animation_ADDRESS 0x05B512u
#define enemy_shredder_flinch_from_behind_animation_ADDRESS 0x05B532u
#define enemy_shredder_flinch_from_behind_2_animation_ADDRESS 0x05B552u
#define enemy_shredder_lose_bar_animation_ADDRESS 0x05B572u
#define enemy_shredder_defeated_start_animation_ADDRESS 0x05B592u
#define enemy_shredder_defeated_fall_animation_ADDRESS 0x05B5A8u
#define shredder_piece_start_animation_ADDRESS 0x05B5BEu
#define shredder_piece_bounce_animation_ADDRESS 0x05B5C8u
#define sprite_lists_05B5F6_ADDRESS 0x05B5F6u
#define animations_05B6EE_ADDRESS 0x05B6EEu
#define enemy_shredder_guard_animation_ADDRESS 0x05B6F8u
#define sprite_lists_05B702_ADDRESS 0x05B702u
#define animations_05B73A_ADDRESS 0x05B73Au
#define enemy_shredder_slash_animation_ADDRESS 0x05B784u
#define sprite_lists_05B7CE_ADDRESS 0x05B7CEu
#define animations_05B91E_ADDRESS 0x05B91Eu
#define enemy_shredder_kick_animation_ADDRESS 0x05B968u
#define enemy_shredder_crouch_animation_ADDRESS 0x05B9B2u
#define enemy_shredder_crouch_walk_animation_ADDRESS 0x05B9BEu
#define sprite_lists_05B9CA_ADDRESS 0x05B9CAu
#define animations_05BB0A_ADDRESS 0x05BB0Au
#define enemy_shredder_thrust_animation_ADDRESS 0x05BB54u
#define sprite_lists_05BB9E_ADDRESS 0x05BB9Eu
#define animations_05BD3E_ADDRESS 0x05BD3Eu
#define enemy_shredder_sweep_animation_ADDRESS 0x05BD88u
#define sprite_lists_05BDD2_ADDRESS 0x05BDD2u
#define animations_05BF42_ADDRESS 0x05BF42u
#define enemy_shredder_ray_line_up_animation_ADDRESS 0x05BF4Cu
#define enemy_shredder_ray_charge_animation_ADDRESS 0x05BF8Au
#define enemy_shredder_ray_fire_animation_ADDRESS 0x05BFAEu
#define sprite_lists_05BFC8_ADDRESS 0x05BFC8u
#define animations_05C220_ADDRESS 0x05C220u
#define sprite_lists_05C2EC_ADDRESS 0x05C2ECu
#define animations_05C52C_ADDRESS 0x05C52Cu
#define sprite_lists_05C546_ADDRESS 0x05C546u
#define animations_05C566_ADDRESS 0x05C566u
#define sprite_lists_05C57C_ADDRESS 0x05C57Cu
#define animations_05C5A4_ADDRESS 0x05C5A4u
#define sprite_lists_05C5D2_ADDRESS 0x05C5D2u
#define animations_05C632_ADDRESS 0x05C632u
#define motorbike_struck_animation_ADDRESS 0x05C65Cu
#define motorbike_ride_animation_ADDRESS 0x05C686u
#define motorbike_struck_wheelie_animation_ADDRESS 0x05C6B0u
#define sprite_lists_05C6DA_ADDRESS 0x05C6DAu
#define animations_05C7CA_ADDRESS 0x05C7CAu
#define sprite_lists_05C7EE_ADDRESS 0x05C7EEu
#define animations_05C846_ADDRESS 0x05C846u
#define tire_soldier_animation_ADDRESS 0x05C87Au
#define rolling_tire_animation_ADDRESS 0x05C890u
#define sprite_lists_05C8A2_ADDRESS 0x05C8A2u
#define animations_05C92A_ADDRESS 0x05C92Au
#define enemy_rocket_soldier_line_up_animation_ADDRESS 0x05C944u
#define rocket_animation_ADDRESS 0x05C95Au
#define sprite_lists_05C96C_ADDRESS 0x05C96Cu
#define animations_05C9C4_ADDRESS 0x05C9C4u
#define sprite_lists_05CA02_ADDRESS 0x05CA02u
#define animations_05CA3A_ADDRESS 0x05CA3Au
#define skateboard_shadow_ADDRESS 0x05CA54u
#define skateboard_shadow_2_ADDRESS 0x05CA5Cu
#define animations_05CA64_ADDRESS 0x05CA64u
#define dynamite_animation_ADDRESS 0x05CB10u
#define sprite_lists_05CBB6_ADDRESS 0x05CBB6u
#define animations_05CD46_ADDRESS 0x05CD46u
#define sprite_lists_05CD60_ADDRESS 0x05CD60u
#define animations_05CDA0_ADDRESS 0x05CDA0u
#define sprite_lists_05CDB6_ADDRESS 0x05CDB6u
#define animations_05CE06_ADDRESS 0x05CE06u
#define boss_shutter_animation_ADDRESS 0x05CE10u
#define sprite_lists_05CE26_ADDRESS 0x05CE26u
#define animations_05CE9E_ADDRESS 0x05CE9Eu
#define technodrome_door_animation_ADDRESS 0x05CEA8u
#define sprite_lists_05CEBE_ADDRESS 0x05CEBEu
#define animations_05CEEE_ADDRESS 0x05CEEEu
#define sprite_lists_05CF92_ADDRESS 0x05CF92u
#define unused_animation_list_ADDRESS 0x05D03Au
#define enemies_05D506_ADDRESS 0x05D506u
#define unexplained_05F402_ADDRESS 0x05F402u

/* numbers the code uses, by name (symbols.txt constant lines) */
#define HIT_NEAR_LEFT_EDGE_X 0xE000u
#define HIT_NEAR_RIGHT_EDGE_X 0x12000u
#define LIFT_EDGE_RANGE 0x1A000u
#define DRILL_POD_X_AREA_1 0x1D6FFu
#define DRILL_POD_OPEN_X_AREA_7 0x5B6FFu
#define DRILL_POD_X_AREA_7 0x5B7FFu

/* the game's structures in work RAM */
#pragma pack(push, 1)
struct object
{
	union
	{
		struct
		{
			uint16_t hit_object;
			uint16_t attacker;
			uint8_t throw;
			uint8_t unk_4A;
			uint8_t unk_49;
			uint8_t unk_48;
			uint8_t unk_47;
			uint8_t unk_46;
			uint8_t unk_45;
			uint8_t unk_44;
			uint8_t animation_delay;
			uint8_t colour;
			union
			{
				uint16_t hit_routine;
				uint8_t hit_state;
			};
			uint16_t draw_next;
			uint32_t shadow;
			uint32_t frame;
			uint16_t depth;
			uint8_t frame_time;
			uint8_t floor_height;
			uint8_t unk_31;
			uint8_t unk_30;
			uint8_t count;
			uint8_t unk_2E;
			uint16_t unk_2C;
			uint8_t unk_2B;
			uint8_t unk_2A;
			uint16_t unk_28;
			uint8_t unk_27;
			uint8_t unk_26;
			uint8_t unk_25;
			uint8_t unk_24;
			uint8_t unk_23;
			uint8_t unk_22;
			uint16_t unk_20;
			int16_t height_speed;
			int16_t y_speed;
			int16_t x_speed;
			uint32_t y;
			uint32_t x;
			union
			{
				uint16_t height;
				struct
				{
					uint8_t pad_3E[0x1];
					uint8_t height_whole;
				};
			};
			uint16_t screen_y;
			uint16_t screen_x;
			uint16_t damage;
			uint8_t health;
			uint8_t health_bars;
			uint8_t hit;
			uint8_t attack;
			uint8_t state;
			uint8_t routine;
			uint8_t parameter;
		};
		struct
		{
			uint8_t pad_00[0x35];
			uint8_t stepping_left;
		} april;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t balls_left;
		} ball_dropper;
		struct
		{
			uint8_t pad_00[0x2E];
			uint16_t trigger_x;
		} billboard;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t timer;
			uint8_t flipped;
			uint8_t pad_21[0xD];
			uint16_t trigger_x;
		} billboard_soldier;
		struct
		{
			uint8_t pad_00[0x2D];
			uint8_t timer;
		} bomber;
		struct
		{
			uint8_t pad_00[0x9];
			uint8_t flight_time;
			uint8_t pad_0A[0x22];
			uint32_t thrower;
		} boomerang;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t explosions;
			uint8_t pad_21[0xB];
			uint32_t target;
		} boss;
		struct
		{
			uint8_t pad_00[0x9];
			uint8_t open_time;
			uint8_t pad_0A[0x16];
			uint8_t explosions;
			uint8_t pad_21[0xB];
			uint32_t target;
		} boss_shutter;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t number;
			uint8_t pad_21[0xD];
			uint16_t stop_y;
		} car_soldier;
		struct
		{
			uint8_t pad_00[0x2F];
			uint8_t once;
		} car_street_van;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t aim;
		} drone_shot;
		struct
		{
			uint8_t pad_00[0x5];
			uint8_t bounces_left;
			uint8_t pad_06[0x20];
			uint16_t hit_sound;
		} dynamite;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t timer;
			uint8_t column_done;
			uint8_t pad_21[0xB];
			uint32_t vent;
		} electric;
		struct
		{
			uint8_t pad_00[0x9];
			uint8_t wait;
			uint8_t pad_0A[0x1E];
			uint32_t stop;
		} elevator;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x5];
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_back_off;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t punch_delay;
			uint8_t pad_06[0x3];
			uint8_t timer;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t plan;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_bebop;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t punch_delay;
			uint8_t pad_06[0x3];
			uint8_t timer;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x3];
			uint8_t walk_time;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t plan;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_bebop_enter;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t punch_delay;
			uint8_t pad_06[0x3];
			uint8_t timer;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x2];
			uint8_t shots_left;
			uint8_t shot_time;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t plan;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_bebop_shoot;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x1];
			uint32_t boomerang;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_boomerang;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t roll;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_charge;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t clockwise;
			uint8_t pad_06[0x4];
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_circle;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_dive_in;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t turn_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_flying_slash_sword;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_grab;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_hang_in;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_kick_in;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t down_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_knocked;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x3];
			uint8_t chase_time;
			uint8_t timer;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_mouser;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t aim_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint32_t turtle;
		} enemy_rocket_soldier;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x1];
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x3];
			uint8_t timer;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rocksteady;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t turn;
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x3];
			uint8_t aim_time;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rocksteady_circle;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x1];
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x3];
			uint8_t timer;
			uint8_t pad_10[0xE];
			uint8_t aim_time;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rocksteady_close_in;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t blink_time;
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x3];
			uint8_t timer;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t animations;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rocksteady_defeated;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t lie_time;
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x3];
			uint8_t timer;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t animations;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rocksteady_knocked_down;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t aim;
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x2];
			uint8_t shots_left;
			uint8_t shot_time;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t bullet;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rocksteady_shoot;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t aim;
			uint8_t far_attack_wait;
			uint8_t close_attack_wait;
			uint16_t mode_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x2];
			uint8_t shots_left;
			uint8_t shot_time;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t armed;
			uint16_t move;
			uint32_t bullet;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
			uint8_t pad_30[0x2];
			uint32_t aims;
		} enemy_rocksteady_shoot_around;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x1];
			int16_t brake;
			uint8_t pad_08[0x1];
			uint8_t wait;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rodney;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x1];
			int16_t brake;
			uint8_t shots_left;
			uint8_t shot_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rodney_shoot;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t arm_flags;
			int16_t brake;
			uint8_t pad_08[0x1];
			uint8_t arm_length;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t arm;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_rodney_whip;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_run_in;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x3];
			uint8_t shots_left;
			uint8_t shot_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_shoot;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t attack_wait;
			uint8_t pad_06[0x1];
			uint8_t timer;
			uint16_t ray_wait;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t guard_wait;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint8_t pad_28[0x3];
			uint8_t hits_to_knock_back;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_shredder;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t steps;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_shuffle;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_smash_hammer;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t hits_left;
			uint8_t pad_06[0x4];
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_spear;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x3];
			uint8_t time;
			uint8_t turns;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_stalk;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_step_down;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x4];
			uint8_t time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_step_out;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x3];
			uint8_t throws_left;
			uint8_t throw_time;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x12];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_throw;
		struct
		{
			uint8_t pad_00[0x4];
			uint8_t throw;
			uint8_t pad_05[0x1];
			uint32_t animations;
			uint8_t side;
			uint8_t target_faces;
			uint8_t pad_0C[0x2];
			uint16_t speed;
			uint8_t pad_10[0xE];
			uint8_t dodge_chance;
			uint8_t facing;
			uint16_t target_dy;
			uint16_t target_dx;
			uint8_t target_direction;
			uint8_t approach;
			uint16_t move;
			uint32_t definition;
			uint8_t retarget_time;
			uint8_t move_time;
			uint16_t target;
		} enemy_walk;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
		} falling_debris;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t shots_left;
			uint8_t timer;
			uint8_t aim;
			uint8_t turning;
			union
			{
				uint16_t heading;
				struct
				{
					uint8_t pad_22[0x1];
					uint8_t direction;
				};
			};
			uint8_t turn_speed;
			uint8_t bob;
			uint8_t pad_26[0x4];
			uint16_t time_left;
			uint32_t target;
		} flying_drone;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
		} foot_car;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t timer;
			uint8_t pad_20[0xC];
			uint32_t row_status;
		} freeze_nozzle;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t flies_left;
			uint8_t pad_21[0xB];
			uint32_t shooter;
		} gun_blast;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t shots_left;
			uint8_t timer;
			uint8_t drift;
			uint8_t pad_21[0x1];
			uint16_t stay_time;
		} helicopter;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t jets_left;
			uint8_t timer;
		} hydrant;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t jets_left;
			uint8_t timer;
			uint8_t jet;
		} hydrant_water;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t move_time;
			uint8_t timer;
			uint8_t laser_angle;
			uint8_t close_turtle;
			uint8_t hurt;
			uint8_t pad_23[0x2];
			uint8_t hit_delay;
			uint8_t pad_26[0x5];
			uint8_t hits_left;
		} krang;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t move_time;
			uint8_t timer;
			uint8_t explosions;
			uint8_t close_turtle;
			uint8_t hurt;
			uint8_t pad_23[0x2];
			uint8_t hit_delay;
			uint8_t pad_26[0x5];
			uint8_t hits_left;
		} krang_explode;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t move_time;
			uint8_t timer;
			uint8_t flipped;
			uint8_t close_turtle;
			uint8_t hurt;
			uint8_t pad_23[0x2];
			uint8_t hit_delay;
			uint8_t pad_26[0x5];
			uint8_t hits_left;
			uint32_t shooter;
		} krang_fist;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t last;
			uint8_t delay;
			uint8_t flipped;
			uint8_t close_turtle;
			uint8_t hurt;
			uint8_t pad_23[0x2];
			uint8_t hit_delay;
			uint8_t pad_26[0x5];
			uint8_t hits_left;
			uint32_t shooter;
		} krang_laser;
		struct
		{
			uint8_t pad_00[0x2E];
			uint16_t trigger_x;
		} manhole_soldier;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t flies_left;
			uint8_t pad_21[0xB];
			uint32_t shooter;
		} missile;
		struct
		{
			uint8_t pad_00[0x5];
			uint8_t wheelie;
			uint8_t pad_06[0x3];
			uint8_t aim_time;
			uint8_t pad_0A[0x1C];
			uint16_t hit_sound;
			uint8_t pad_28[0x4];
			uint32_t target_turtle;
		} motorbike;
		struct
		{
			uint8_t pad_00[0x9];
			uint8_t mousers_left;
			uint8_t pad_0A[0x1E];
			uint32_t hole;
			uint8_t pad_2C[0x1];
			uint8_t spawn_time;
		} mouser_hole;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t nozzle_status;
		} nozzle_row;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t timer;
		} oil_drum;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t soldier_time;
			uint8_t counter;
			uint8_t pad_20[0x4];
			uint16_t last_screen_x;
			uint8_t pad_26[0x2];
			uint16_t scroll_y;
			uint16_t scroll_x;
			uint32_t blank_row;
		} parked_car;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t announced;
		} pizza;
		struct
		{
			uint8_t pad_00[0x2C];
			uint16_t end_y;
			uint16_t end_x;
		} profile;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t timer;
		} rat;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t move_time;
			uint8_t timer;
			uint8_t explosions;
			uint8_t close_turtle;
			uint8_t hurt;
			uint8_t shot_hit;
			uint8_t players;
			uint8_t hit_delay;
			uint8_t pad_26[0x3];
			uint8_t blows;
			uint8_t pad_2A[0x1];
			uint8_t hits_left;
		} rock_soldier;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
		} rocket;
		struct
		{
			uint8_t pad_00[0x28];
			uint32_t burst;
		} rocksteady_bullet;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
			uint8_t pad_28[0x5];
			uint8_t hit_delay;
		} rolling;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
		} scenery_open_manhole;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
		} set_object_stats;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t missiles_left;
			uint8_t delay;
			uint8_t drift;
		} sewer_launcher;
		struct
		{
			uint8_t pad_00[0x20];
			uint8_t goes_right;
		} sewer_missile;
		struct
		{
			uint8_t pad_00[0x9];
			uint8_t swim_time;
		} sewer_ripple;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t blink_timer;
			uint8_t pad_20[0x1];
			uint8_t blink_period;
			uint8_t pad_22[0xA];
			uint32_t leader;
		} shock_bars;
		struct
		{
			uint8_t pad_00[0x5];
			uint8_t landings;
		} shredder_piece;
		struct
		{
			uint8_t pad_00[0x28];
			uint32_t shredder;
			uint8_t pad_2C[0x1];
			uint8_t time_left;
		} shredder_ray;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t timer;
		} spiked_pole;
		struct
		{
			uint8_t pad_00[0x26];
			uint16_t hit_sound;
		} start_shot;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t unused_cleared;
			uint8_t timer;
			uint8_t explosions;
			uint8_t turning;
			union
			{
				uint16_t heading;
				struct
				{
					uint8_t pad_22[0x1];
					uint8_t direction;
				};
			};
			uint8_t turn_speed;
			uint8_t bob;
			uint8_t pad_26[0x4];
			uint16_t time_left;
			uint32_t target;
		} stockman;
		struct
		{
			uint8_t pad_00[0x1F];
			uint8_t shadow_time;
		} technodrome_ball;
		struct
		{
			uint8_t pad_00[0x8];
			uint8_t soldiers_left;
			uint8_t spawn_time;
		} technodrome_door;
		struct
		{
			uint8_t pad_00[0x2E];
			uint16_t trigger_x;
		} tire_soldier;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t shots_left;
			uint8_t timer;
			uint8_t phase;
		} turret;
		struct
		{
			uint8_t pad_00[0x1E];
			uint8_t shots_left;
			uint8_t timer;
			uint8_t flipped;
		} turret_shot;
		struct
		{
			uint16_t prompt_place;
			uint32_t hit_target;
			uint8_t prompt_frame;
			uint8_t prompt_time;
			uint16_t combo;
			uint8_t has_gun;
			uint8_t enemy_behind;
			uint8_t pad_0C[0x12];
			uint8_t attack_press_time;
			uint8_t pad_1F[0x2];
			uint8_t combo_step;
			uint8_t invulnerable_time;
			uint8_t exit_step;
			uint32_t hit_by;
			uint8_t escape_presses;
			uint8_t wait_timer;
			uint8_t reaction_variant;
			uint8_t reaction_step;
			uint8_t hit_reaction;
			uint8_t knockback_flags;
			uint16_t drift_speed;
			uint8_t jump_flip;
			uint8_t idle_time;
			uint8_t move_timer;
			uint8_t falling;
			uint16_t jump_speed;
			uint8_t pad_36[0x16];
			uint8_t scroll_push;
		} turtle;
		struct
		{
			uint16_t prompt_place;
			uint32_t hit_target;
			uint8_t prompt_frame;
			uint8_t prompt_time;
			uint16_t combo;
			uint8_t has_gun;
			uint8_t enemy_behind;
			uint8_t pad_0C[0x12];
			uint8_t attack_press_time;
			uint8_t pad_1F[0x2];
			uint8_t combo_step;
			uint8_t invulnerable_time;
			uint8_t exit_step;
			uint32_t hit_by;
			uint8_t escape_presses;
			uint8_t wait_timer;
			uint8_t reaction_variant;
			uint8_t reaction_step;
			uint8_t hit_reaction;
			uint8_t knockback_flags;
			uint16_t drift_speed;
			int16_t jump_in_speed;
			uint8_t move_timer;
			uint8_t falling;
			uint16_t jump_speed;
			uint8_t pad_36[0x16];
			uint8_t scroll_push;
		} turtle_jump_in;
		struct
		{
			uint8_t pad_00[0x8];
			uint16_t unused_cleared;
			uint8_t pad_0A[0x1E];
			uint32_t rodney;
		} whip_arm;
		struct
		{
			uint8_t pad_00[0x28];
			uint32_t blocks;
			uint8_t pad_2C[0x2];
			uint16_t trigger_y;
		} window_soldier_dive;
		struct
		{
			uint8_t pad_00[0x8];
			uint8_t delay;
			uint8_t break_time;
			uint8_t pad_0A[0x1E];
			uint32_t blocks;
			uint8_t pad_2C[0x2];
			uint16_t trigger_x;
		} window_soldier_drop;
	};
	uint8_t kind;
	uint8_t contacts;
	uint8_t flags;
};

struct animation_frame
{
	int16_t attack_depth;
	uint16_t body_box;
	uint32_t sprites;
	uint8_t last_sprite;
	uint8_t time;
};

struct attack_frame
{
	int16_t attack_height;
	int16_t attack_width;
	int16_t attack_y;
	int16_t attack_x;
	int16_t attack_depth;
	uint16_t body_box;
	uint32_t sprites;
	uint8_t last_sprite;
	uint8_t time;
};

struct sprite
{
	int16_t y;
	int16_t x;
	uint8_t flips;
	uint8_t colour;
	uint8_t code;
	uint8_t size_code;
};

struct sprite_size
{
	uint8_t pad_00[0x2];
	int16_t width;
	int16_t bottom;
	int16_t left;
};

struct body_box
{
	int16_t height;
	int16_t half_width;
};

struct player_slot
{
	uint32_t unused;
	uint8_t pad_04[0x3];
	uint8_t grab_time;
	uint8_t mouser_sides;
	uint8_t rodney_sides;
	uint8_t sides;
	uint8_t enemies;
	uint32_t turtle;
};

struct player
{
	uint8_t message;
	uint8_t health_bars;
	uint16_t score;
	uint16_t unused_setting_copy;
	union
	{
		uint16_t lives;
		uint8_t lives_low;
	};
};

struct coin_slot
{
	uint16_t credits;
	uint8_t counter_on;
	uint8_t credits_per_coins;
	uint8_t coins_per_credit;
	uint8_t counter_pulse;
	uint8_t coins;
	uint8_t coins_to_count;
};

struct palette_effect
{
	uint32_t colour;
	uint32_t step;
	uint8_t time;
	uint8_t flags;
};

struct palette_request
{
	uint32_t data;
	uint16_t kind;
};

struct layer
{
	uint8_t pad_00[0xA];
	uint8_t new_column;
	uint8_t new_row;
	uint32_t new_row_y;
	uint32_t new_column_x;
	uint32_t y_min;
	uint32_t x_min;
	uint32_t y_max;
	uint32_t x_max;
	int8_t x_direction;
	int8_t y_direction;
	uint32_t blocks;
	uint32_t map;
	uint16_t map_width;
	uint16_t y_speed;
	uint16_t x_speed;
	uint16_t row_phase;
	uint16_t column_phase;
	union
	{
		uint32_t y;
		struct
		{
			uint8_t pad_38[0x1];
			uint8_t y_pixel;
			uint8_t y_page;
		};
	};
	union
	{
		uint32_t x;
		struct
		{
			uint8_t pad_3C[0x2];
			uint8_t x_page;
		};
	};
};

struct typed_pass
{
	uint8_t delay;
	uint8_t wait;
	uint32_t text;
	uint32_t place;
};

struct object_stats
{
	uint16_t hit_sound;
	uint16_t depth;
	uint16_t damage;
	uint8_t health;
	uint8_t health_bars;
	uint8_t hit;
	uint8_t attack;
};

struct arc
{
	int16_t height_speed_limit;
	int16_t x_step;
	int16_t gravity;
	int16_t height_speed;
	int16_t x_drag;
	int16_t x_speed;
};

struct placement
{
	uint8_t height;
	int16_t y;
	int16_t x;
};

struct shot_record
{
	uint8_t kind;
	uint8_t height;
	int16_t y;
	int16_t x;
};

struct object_record
{
	uint16_t kind_and_parameter;
	uint8_t group;
	uint8_t height;
	int16_t y;
	int16_t x;
};

struct intro_object
{
	int16_t y_speed;
	uint32_t animation;
};

struct portrait
{
	uint32_t animation;
	uint32_t y;
	uint32_t x;
};

struct offset_pair
{
	int16_t y;
	int16_t x;
};

struct fall_in_place
{
	uint16_t height;
	int16_t y;
	int16_t x;
};

struct bebop_attack
{
	uint16_t chance;
	uint16_t move;
	uint16_t reach_x;
	uint16_t reach_y;
};

struct random_move
{
	uint16_t move;
	uint16_t chance;
};

struct rodney_attack
{
	uint8_t move;
	uint8_t chance;
	uint16_t reach;
};

struct profile_motion
{
	uint8_t pad_00[0x4];
	int16_t y_speed;
	int16_t x_speed;
	uint16_t end_y;
	uint16_t end_x;
	uint16_t y;
	uint16_t x;
};

struct wreck_part
{
	uint8_t pad_00[0x1];
	uint8_t colour;
	uint8_t rows;
	uint8_t tile;
	uint32_t place;
};

struct palette_effect_step
{
	uint16_t colour;
	uint16_t time;
};

struct enemy_definition
{
	uint8_t pad_00[0x2F];
	uint8_t reactions;
	uint8_t pad_30[0x2F];
	uint8_t attacks;
	uint8_t pad_60[0x27];
	uint8_t approach_moves;
};

struct approach_moves
{
	uint8_t pad_00[0x3];
	uint8_t chance;
	uint8_t pad_04[0x3];
	uint8_t move;
};

struct attack_record
{
	uint8_t pad_00[0x3];
	uint8_t move_chance;
	uint8_t pad_04[0x3];
	uint8_t move;
	uint8_t chance;
	uint8_t reach_x_max;
	uint8_t reach_x_min;
	uint8_t reach_y;
};

struct screen_margin
{
	uint16_t y;
	uint16_t x;
};

struct spawn_record
{
	uint16_t kind_and_parameter;
	uint8_t flags;
	uint8_t height;
	uint16_t y;
	uint16_t x;
};

struct scroll_setting
{
	uint32_t b_y_min;
	uint32_t b_x_min;
	uint32_t a_y_min;
	uint32_t a_x_min;
	uint32_t b_y_max;
	uint32_t b_x_max;
	union
	{
		uint32_t a_y_max;
		struct
		{
			uint8_t pad_18[0x2];
			uint8_t x_direction;
			uint8_t y_direction;
		};
	};
	union
	{
		uint32_t a_x_max;
		uint16_t b_y_speed;
		struct
		{
			uint8_t pad_1C[0x2];
			uint16_t b_x_speed;
		};
	};
	union
	{
		uint32_t held;
		uint16_t a_y_speed;
		struct
		{
			uint8_t pad_20[0x2];
			uint16_t a_x_speed;
		};
	};
	uint32_t end;
	uint8_t end_axis;
	uint8_t mode;
};

struct dip_setting
{
	uint32_t value_2;
	uint32_t value;
	uint16_t flag;
};

struct program_chip
{
	uint16_t blocks;
	uint32_t start;
};

struct chip_sum
{
	uint16_t sum;
};

struct text_place
{
	uint32_t place;
};

struct palette_colour
{
	uint8_t low;
	uint8_t high;
};

struct palette_fade
{
	uint32_t palettes;
	uint16_t banks;
};

struct window_break_step
{
	uint8_t time;
	uint8_t place;
	uint32_t block;
};

struct dive_window
{
	uint32_t other_block;
	uint32_t block;
	uint8_t other_place;
	uint8_t place;
};

struct spawn_speed
{
	uint16_t y;
	uint16_t x;
};

struct spawn_direction
{
	uint8_t x;
	uint8_t y;
};

#pragma pack(pop)

_Static_assert(sizeof(struct object) == 0x50, "struct object has the size of the game's blocks");
_Static_assert(sizeof(struct animation_frame) == 0xA, "struct animation_frame has the size of the program ROM's records");
_Static_assert(sizeof(struct attack_frame) == 0x12, "struct attack_frame has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite) == 0x8, "struct sprite has the size of the program ROM's records");
_Static_assert(sizeof(struct sprite_size) == 0x8, "struct sprite_size has the size of the program ROM's records");
_Static_assert(sizeof(struct body_box) == 0x4, "struct body_box has the size of the program ROM's records");
_Static_assert(sizeof(struct player_slot) == 0x10, "struct player_slot has the size of the game's blocks");
_Static_assert(sizeof(struct player) == 0x8, "struct player has the size of the game's blocks");
_Static_assert(sizeof(struct coin_slot) == 0x8, "struct coin_slot has the size of the game's blocks");
_Static_assert(sizeof(struct palette_effect) == 0xA, "struct palette_effect has the size of the game's blocks");
_Static_assert(sizeof(struct palette_request) == 0x6, "struct palette_request has the size of the game's blocks");
_Static_assert(sizeof(struct layer) == 0x40, "struct layer has the size of the game's blocks");
_Static_assert(sizeof(struct typed_pass) == 0xA, "struct typed_pass has the size of the game's blocks");
_Static_assert(sizeof(struct object_stats) == 0xA, "struct object_stats has the size of the program ROM's records");
_Static_assert(sizeof(struct arc) == 0xC, "struct arc has the size of the program ROM's records");
_Static_assert(sizeof(struct placement) == 0x5, "struct placement has the size of the program ROM's records");
_Static_assert(sizeof(struct shot_record) == 0x6, "struct shot_record has the size of the program ROM's records");
_Static_assert(sizeof(struct object_record) == 0x8, "struct object_record has the size of the program ROM's records");
_Static_assert(sizeof(struct intro_object) == 0x6, "struct intro_object has the size of the program ROM's records");
_Static_assert(sizeof(struct portrait) == 0xC, "struct portrait has the size of the program ROM's records");
_Static_assert(sizeof(struct offset_pair) == 0x4, "struct offset_pair has the size of the program ROM's records");
_Static_assert(sizeof(struct fall_in_place) == 0x6, "struct fall_in_place has the size of the program ROM's records");
_Static_assert(sizeof(struct bebop_attack) == 0x8, "struct bebop_attack has the size of the program ROM's records");
_Static_assert(sizeof(struct random_move) == 0x4, "struct random_move has the size of the program ROM's records");
_Static_assert(sizeof(struct rodney_attack) == 0x4, "struct rodney_attack has the size of the program ROM's records");
_Static_assert(sizeof(struct profile_motion) == 0x10, "struct profile_motion has the size of the program ROM's records");
_Static_assert(sizeof(struct wreck_part) == 0x8, "struct wreck_part has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_effect_step) == 0x4, "struct palette_effect_step has the size of the program ROM's records");
_Static_assert(sizeof(struct enemy_definition) == 0x88, "struct enemy_definition has the size of the program ROM's records");
_Static_assert(sizeof(struct approach_moves) == 0x8, "struct approach_moves has the size of the program ROM's records");
_Static_assert(sizeof(struct attack_record) == 0xC, "struct attack_record has the size of the program ROM's records");
_Static_assert(sizeof(struct screen_margin) == 0x4, "struct screen_margin has the size of the program ROM's records");
_Static_assert(sizeof(struct spawn_record) == 0x8, "struct spawn_record has the size of the program ROM's records");
_Static_assert(sizeof(struct scroll_setting) == 0x2A, "struct scroll_setting has the size of the program ROM's records");
_Static_assert(sizeof(struct dip_setting) == 0xA, "struct dip_setting has the size of the program ROM's records");
_Static_assert(sizeof(struct program_chip) == 0x6, "struct program_chip has the size of the program ROM's records");
_Static_assert(sizeof(struct chip_sum) == 0x2, "struct chip_sum has the size of the program ROM's records");
_Static_assert(sizeof(struct text_place) == 0x4, "struct text_place has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_colour) == 0x2, "struct palette_colour has the size of the program ROM's records");
_Static_assert(sizeof(struct palette_fade) == 0x6, "struct palette_fade has the size of the program ROM's records");
_Static_assert(sizeof(struct window_break_step) == 0x6, "struct window_break_step has the size of the program ROM's records");
_Static_assert(sizeof(struct dive_window) == 0xA, "struct dive_window has the size of the program ROM's records");
_Static_assert(sizeof(struct spawn_speed) == 0x4, "struct spawn_speed has the size of the program ROM's records");
_Static_assert(sizeof(struct spawn_direction) == 0x2, "struct spawn_direction has the size of the program ROM's records");

#define OBJECT(address) ((struct object *)ram((address), sizeof(struct object)))
#define ANIMATION_FRAME(address) ((const struct animation_frame *)data_record((address), sizeof(struct animation_frame)))
#define ATTACK_FRAME(address) ((const struct attack_frame *)data_record((address), sizeof(struct attack_frame)))
#define SPRITE(address) ((const struct sprite *)data_record((address), sizeof(struct sprite)))
#define SPRITE_SIZE(address) ((const struct sprite_size *)data_record((address), sizeof(struct sprite_size)))
#define BODY_BOX(address) ((const struct body_box *)data_record((address), sizeof(struct body_box)))
#define PLAYER_SLOT(address) ((struct player_slot *)ram((address), sizeof(struct player_slot)))
#define PLAYER(address) ((struct player *)ram((address), sizeof(struct player)))
#define COIN_SLOT(address) ((struct coin_slot *)ram((address), sizeof(struct coin_slot)))
#define PALETTE_EFFECT(address) ((struct palette_effect *)ram((address), sizeof(struct palette_effect)))
#define PALETTE_REQUEST(address) ((struct palette_request *)ram((address), sizeof(struct palette_request)))
#define LAYER(address) ((struct layer *)ram((address), sizeof(struct layer)))
#define TYPED_PASS(address) ((struct typed_pass *)ram((address), sizeof(struct typed_pass)))
#define OBJECT_STATS(address) ((const struct object_stats *)data_record((address), sizeof(struct object_stats)))
#define ARC(address) ((const struct arc *)data_record((address), sizeof(struct arc)))
#define PLACEMENT(address) ((const struct placement *)data_record((address), sizeof(struct placement)))
#define SHOT_RECORD(address) ((const struct shot_record *)data_record((address), sizeof(struct shot_record)))
#define OBJECT_RECORD(address) ((const struct object_record *)data_record((address), sizeof(struct object_record)))
#define TEXT(address) (*(const uint8_t *)data_record((address), 1))
#define TEXT_16(address) (*(const uint16_t *)data_record((address), 2))
#define TEXT_32(address) (*(const uint32_t *)data_record((address), 4))
#define TILE_PICTURE(address) (*(const uint8_t *)data_record((address), 1))
#define TILE_PICTURE_16(address) (*(const uint16_t *)data_record((address), 2))
#define TILE_PICTURE_32(address) (*(const uint32_t *)data_record((address), 4))
#define BLOCK_BYTE(address) (*(const uint8_t *)data_record((address), 1))
#define BLOCK_BYTE_16(address) (*(const uint16_t *)data_record((address), 2))
#define BLOCK_BYTE_32(address) (*(const uint32_t *)data_record((address), 4))
#define MAP_ENTRY(address) (*(const uint16_t *)data_record((address), 2))
#define MAP_ENTRY_8(address) (*(const uint8_t *)data_record((address), 1))
#define MAP_ENTRY_32(address) (*(const uint32_t *)data_record((address), 4))
#define ANIMATION_ENTRY(address) (*(const uint32_t *)data_record((address), 4))
#define ANIMATION_ENTRY_8(address) (*(const uint8_t *)data_record((address), 1))
#define ANIMATION_ENTRY_16(address) (*(const uint16_t *)data_record((address), 2))
#define ANIMATION_TABLE_ENTRY(address) (*(const uint32_t *)data_record((address), 4))
#define ANIMATION_TABLE_ENTRY_8(address) (*(const uint8_t *)data_record((address), 1))
#define ANIMATION_TABLE_ENTRY_16(address) (*(const uint16_t *)data_record((address), 2))
#define INTRO_OBJECT(address) ((const struct intro_object *)data_record((address), sizeof(struct intro_object)))
#define PORTRAIT(address) ((const struct portrait *)data_record((address), sizeof(struct portrait)))
#define WHIP_OFFSET(address) (*(const int16_t *)data_record((address), 2))
#define WHIP_OFFSET_8(address) (*(const uint8_t *)data_record((address), 1))
#define WHIP_OFFSET_32(address) (*(const uint32_t *)data_record((address), 4))
#define SHADOW_ENTRY(address) (*(const uint32_t *)data_record((address), 4))
#define SHADOW_ENTRY_8(address) (*(const uint8_t *)data_record((address), 1))
#define SHADOW_ENTRY_16(address) (*(const uint16_t *)data_record((address), 2))
#define REACH(address) (*(const uint8_t *)data_record((address), 1))
#define REACH_16(address) (*(const uint16_t *)data_record((address), 2))
#define REACH_32(address) (*(const uint32_t *)data_record((address), 4))
#define WAIT_TIME(address) (*(const uint8_t *)data_record((address), 1))
#define WAIT_TIME_16(address) (*(const uint16_t *)data_record((address), 2))
#define WAIT_TIME_32(address) (*(const uint32_t *)data_record((address), 4))
#define AIM(address) (*(const uint8_t *)data_record((address), 1))
#define AIM_16(address) (*(const uint16_t *)data_record((address), 2))
#define AIM_32(address) (*(const uint32_t *)data_record((address), 4))
#define OFFSET_PAIR(address) ((const struct offset_pair *)data_record((address), sizeof(struct offset_pair)))
#define FALL_IN_PLACE(address) ((const struct fall_in_place *)data_record((address), sizeof(struct fall_in_place)))
#define BEBOP_ATTACK(address) ((const struct bebop_attack *)data_record((address), sizeof(struct bebop_attack)))
#define RANDOM_MOVE(address) ((const struct random_move *)data_record((address), sizeof(struct random_move)))
#define RODNEY_ATTACK(address) ((const struct rodney_attack *)data_record((address), sizeof(struct rodney_attack)))
#define DISTANCE_MOVE(address) (*(const uint16_t *)data_record((address), 2))
#define DISTANCE_MOVE_8(address) (*(const uint8_t *)data_record((address), 1))
#define DISTANCE_MOVE_32(address) (*(const uint32_t *)data_record((address), 4))
#define COMBO_STEP(address) (*(const uint8_t *)data_record((address), 1))
#define COMBO_STEP_16(address) (*(const uint16_t *)data_record((address), 2))
#define COMBO_STEP_32(address) (*(const uint32_t *)data_record((address), 4))
#define KNOCK_STEP(address) (*(const uint8_t *)data_record((address), 1))
#define KNOCK_STEP_16(address) (*(const uint16_t *)data_record((address), 2))
#define KNOCK_STEP_32(address) (*(const uint32_t *)data_record((address), 4))
#define PROMPT_BYTE(address) (*(const uint8_t *)data_record((address), 1))
#define PROMPT_BYTE_16(address) (*(const uint16_t *)data_record((address), 2))
#define PROMPT_BYTE_32(address) (*(const uint32_t *)data_record((address), 4))
#define ROUTINE_ENTRY(address) (*(const uint32_t *)data_record((address), 4))
#define ROUTINE_ENTRY_8(address) (*(const uint8_t *)data_record((address), 1))
#define ROUTINE_ENTRY_16(address) (*(const uint16_t *)data_record((address), 2))
#define NOZZLE_PLACE(address) (*(const uint8_t *)data_record((address), 1))
#define NOZZLE_PLACE_16(address) (*(const uint16_t *)data_record((address), 2))
#define NOZZLE_PLACE_32(address) (*(const uint32_t *)data_record((address), 4))
#define PICTURE_TILE(address) (*(const uint8_t *)data_record((address), 1))
#define PICTURE_TILE_16(address) (*(const uint16_t *)data_record((address), 2))
#define PICTURE_TILE_32(address) (*(const uint32_t *)data_record((address), 4))
#define PROFILE_MOTION(address) ((const struct profile_motion *)data_record((address), sizeof(struct profile_motion)))
#define WRECK_PART(address) ((const struct wreck_part *)data_record((address), sizeof(struct wreck_part)))
#define RANKING_DEFAULT(address) (*(const uint32_t *)data_record((address), 4))
#define RANKING_DEFAULT_8(address) (*(const uint8_t *)data_record((address), 1))
#define RANKING_DEFAULT_16(address) (*(const uint16_t *)data_record((address), 2))
#define PALETTE_EFFECT_STEP(address) ((const struct palette_effect_step *)data_record((address), sizeof(struct palette_effect_step)))
#define MASK_ROM_SUM(address) (*(const uint16_t *)data_record((address), 2))
#define MASK_ROM_SUM_8(address) (*(const uint8_t *)data_record((address), 1))
#define MASK_ROM_SUM_32(address) (*(const uint32_t *)data_record((address), 4))
#define BCD_DIGITS(address) (*(const uint8_t *)data_record((address), 1))
#define BCD_DIGITS_16(address) (*(const uint16_t *)data_record((address), 2))
#define BCD_DIGITS_32(address) (*(const uint32_t *)data_record((address), 4))
#define SINE(address) (*(const uint8_t *)data_record((address), 1))
#define SINE_16(address) (*(const uint16_t *)data_record((address), 2))
#define SINE_32(address) (*(const uint32_t *)data_record((address), 4))
#define ATTACK_MOVE(address) (*(const uint16_t *)data_record((address), 2))
#define ATTACK_MOVE_8(address) (*(const uint8_t *)data_record((address), 1))
#define ATTACK_MOVE_32(address) (*(const uint32_t *)data_record((address), 4))
#define HEALTH_SETTING(address) (*(const uint16_t *)data_record((address), 2))
#define HEALTH_SETTING_8(address) (*(const uint8_t *)data_record((address), 1))
#define HEALTH_SETTING_32(address) (*(const uint32_t *)data_record((address), 4))
#define ENEMY_DEFINITION(address) ((const struct enemy_definition *)data_record((address), sizeof(struct enemy_definition)))
#define APPROACH_MOVES(address) ((const struct approach_moves *)data_record((address), sizeof(struct approach_moves)))
#define ATTACK_RECORD(address) ((const struct attack_record *)data_record((address), sizeof(struct attack_record)))
#define PALETTE_POINTER(address) (*(const uint32_t *)data_record((address), 4))
#define PALETTE_POINTER_8(address) (*(const uint8_t *)data_record((address), 1))
#define PALETTE_POINTER_16(address) (*(const uint16_t *)data_record((address), 2))
#define PALETTE_EFFECT_POINTER(address) (*(const uint32_t *)data_record((address), 4))
#define PALETTE_EFFECT_POINTER_8(address) (*(const uint8_t *)data_record((address), 1))
#define PALETTE_EFFECT_POINTER_16(address) (*(const uint16_t *)data_record((address), 2))
#define SCREEN_MARGIN(address) ((const struct screen_margin *)data_record((address), sizeof(struct screen_margin)))
#define SPAWN_RECORD(address) ((const struct spawn_record *)data_record((address), sizeof(struct spawn_record)))
#define SCROLL_SETTING(address) ((const struct scroll_setting *)data_record((address), sizeof(struct scroll_setting)))
#define SCROLL_SETTING_LIST(address) (*(const uint32_t *)data_record((address), 4))
#define SCROLL_SETTING_LIST_8(address) (*(const uint8_t *)data_record((address), 1))
#define SCROLL_SETTING_LIST_16(address) (*(const uint16_t *)data_record((address), 2))
#define DIP_SETTING(address) ((const struct dip_setting *)data_record((address), sizeof(struct dip_setting)))
#define PROGRAM_CHIP(address) ((const struct program_chip *)data_record((address), sizeof(struct program_chip)))
#define CHIP_SUM(address) ((const struct chip_sum *)data_record((address), sizeof(struct chip_sum)))
#define TEXT_PLACE(address) ((const struct text_place *)data_record((address), sizeof(struct text_place)))
#define PALETTE_COLOUR(address) ((const struct palette_colour *)data_record((address), sizeof(struct palette_colour)))
#define PALETTE_FADE(address) ((const struct palette_fade *)data_record((address), sizeof(struct palette_fade)))
#define TEXT_POINTER(address) (*(const uint32_t *)data_record((address), 4))
#define TEXT_POINTER_8(address) (*(const uint8_t *)data_record((address), 1))
#define TEXT_POINTER_16(address) (*(const uint16_t *)data_record((address), 2))
#define TILE_BLOCK_BYTE(address) (*(const uint8_t *)data_record((address), 1))
#define TILE_BLOCK_BYTE_16(address) (*(const uint16_t *)data_record((address), 2))
#define TILE_BLOCK_BYTE_32(address) (*(const uint32_t *)data_record((address), 4))
#define COLOUR_BYTE(address) (*(const uint8_t *)data_record((address), 1))
#define COLOUR_BYTE_16(address) (*(const uint16_t *)data_record((address), 2))
#define COLOUR_BYTE_32(address) (*(const uint32_t *)data_record((address), 4))
#define MOVE_CHANCE(address) (*(const uint8_t *)data_record((address), 1))
#define MOVE_CHANCE_16(address) (*(const uint16_t *)data_record((address), 2))
#define MOVE_CHANCE_32(address) (*(const uint32_t *)data_record((address), 4))
#define PICTURE_SPRITE_BYTE(address) (*(const uint8_t *)data_record((address), 1))
#define PICTURE_SPRITE_BYTE_16(address) (*(const uint16_t *)data_record((address), 2))
#define PICTURE_SPRITE_BYTE_32(address) (*(const uint32_t *)data_record((address), 4))
#define WINDOW_BREAK_STEP(address) ((const struct window_break_step *)data_record((address), sizeof(struct window_break_step)))
#define DIVE_WINDOW(address) ((const struct dive_window *)data_record((address), sizeof(struct dive_window)))
#define SPAWN_SPEED(address) ((const struct spawn_speed *)data_record((address), sizeof(struct spawn_speed)))
#define SPAWN_DIRECTION(address) ((const struct spawn_direction *)data_record((address), sizeof(struct spawn_direction)))
#define FIRE_SPOT_LONG(address) (*(const uint32_t *)data_record((address), 4))
#define FIRE_SPOT_LONG_8(address) (*(const uint8_t *)data_record((address), 1))
#define FIRE_SPOT_LONG_16(address) (*(const uint16_t *)data_record((address), 2))
#define FIRE_BLOCK_OFFSET(address) (*(const uint16_t *)data_record((address), 2))
#define FIRE_BLOCK_OFFSET_8(address) (*(const uint8_t *)data_record((address), 1))
#define FIRE_BLOCK_OFFSET_32(address) (*(const uint32_t *)data_record((address), 4))
#define DEMO_INPUT(address) (*(const uint8_t *)data_record((address), 1))
#define DEMO_INPUT_16(address) (*(const uint16_t *)data_record((address), 2))
#define DEMO_INPUT_32(address) (*(const uint32_t *)data_record((address), 4))
#define WALK_SPEED(address) (*(const uint16_t *)data_record((address), 2))
#define WALK_SPEED_8(address) (*(const uint8_t *)data_record((address), 1))
#define WALK_SPEED_32(address) (*(const uint32_t *)data_record((address), 4))
#define JOB_WORD(address) (*(const uint16_t *)data_record((address), 2))
#define JOB_WORD_8(address) (*(const uint8_t *)data_record((address), 1))
#define JOB_WORD_32(address) (*(const uint32_t *)data_record((address), 4))
#define PALETTE_LIST_POINTER(address) (*(const uint32_t *)data_record((address), 4))
#define PALETTE_LIST_POINTER_8(address) (*(const uint8_t *)data_record((address), 1))
#define PALETTE_LIST_POINTER_16(address) (*(const uint16_t *)data_record((address), 2))
#define KRANG_SPEED(address) (*(const uint16_t *)data_record((address), 2))
#define KRANG_SPEED_8(address) (*(const uint8_t *)data_record((address), 1))
#define KRANG_SPEED_32(address) (*(const uint32_t *)data_record((address), 4))
#define MOVE_CHOICE(address) (*(const uint8_t *)data_record((address), 1))
#define MOVE_CHOICE_16(address) (*(const uint16_t *)data_record((address), 2))
#define MOVE_CHOICE_32(address) (*(const uint32_t *)data_record((address), 4))

/* the program ROM's tables of records, by name: name[i].field */
#define ending_palette_fades_ADDRESS 0x0071DCu
extern const void *ending_palette_fades_data;
#define ending_palette_fades ((const struct palette_fade *)(ending_palette_fades_data))
#define OBJECT_ADDRESS(n) (0x62000u + (n) * 0x50u)    /* the 50 of them, from 0 */
#define PLAYER_ADDRESS(n) (0x60100u + (n) * 0x8u)    /* the 4 of them, from 0 */
#define COIN_SLOT_ADDRESS(n) (0x60300u + (n) * 0x8u)    /* the 4 of them, from 0 */
#define PALETTE_EFFECT_ADDRESS(n) (0x60480u + (n) * 0xAu)    /* the 64 of them, from 0 */
#define PALETTE_REQUEST_ADDRESS(n) (0x60B00u + (n) * 0x6u)    /* the 40 of them, from 0 */
#define PLAYER_SLOT_ADDRESS(n) (0x62FA0u + (n) * 0x10u)    /* the 4 of them, from 0 */
#define LAYER_ADDRESS(n) (0x63300u + (n) * 0x40u)    /* the 3 of them, from 0 */
#define TYPED_PASS_ADDRESS(n) (0x63430u + (n) * 0xAu)    /* the 8 of them, from 0 */

#endif
