/**
 *    Name:        table.c
 *    Description: the code the board runs by address (the vectors' handlers), the reset entry, and the tables the game loads by name
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* what the code carries in its registers (game.h) */
uint32_t screen_x_high = 0u, distance_high_word = 0u, distance_low_byte = 0u, fade_banks = 0u, offset_high_word = 0u, palette_place = 0u, object_loop_slot = 0u, object_loop_leftover = 0u;

/* the same, for the runtime to put back what an interrupt's handler found there when it returns, as the
   handler does */
uint32_t *const game_context[] = { &screen_x_high, &distance_high_word, &distance_low_byte, &fade_banks, &offset_high_word, &palette_place, &object_loop_slot, &object_loop_leftover };
const size_t game_context_count = 8;

/* the handlers of the vectors (the reset, the exceptions, the interrupts), by address */
const game_code_entry game_code[] =
{
	{ 0x001000u, reset },
	{ 0x00193Eu, exception_hang },
	{ 0x003DC4u, vblank_interrupt },
	{ 0x004190u, irq6_interrupt },
};

const size_t game_code_count = 4;

/* the code at the reset vector */
void game_entry(void)
{
	reset();
}

const void *job_handlers_data;
const void *lives_settings_data;
const void *coinage_coins_data;
const void *coinage_credits_data;
const void *coinage_2_coins_data;
const void *coinage_2_credits_data;
const void *boot_texts_data;
const void *text_colours_data;
const void *player_message_handlers_data;
const void *text_layer_places_data;
const void *big_font_place_data;
const void *big_font_tiles_data;
const void *big_font_tiles_table_data;
const void *colour_texts_data;
const void *tile_pictures_data;
const void *player_box_places_data;
const void *box_labels_data;
const void *lives_digit_top_data;
const void *lives_digit_bottom_data;
const void *lives_icon_top_data;
const void *lives_icon_bottom_data;
const void *stage_titles_data;
const void *positioned_texts_data;
const void *tall_texts_data;
const void *game_state_handlers_data;
const void *startup_steps_data;
const void *attract_steps_data;
const void *game_scene_handlers_data;
const void *demo_settings_table_data;
const void *demo_input_streams_data;
const void *color_test_places_data;
const void *player_input_ports_data;
const void *player_input_places_data;
const void *coin_input_port_data;
const void *coin_input_places_data;
const void *dip_switch_ports_data;
const void *dip_switch_places_data;
const void *coin_counter_places_data;
const void *sound_test_codes_data;
const void *area_continue_scenes_data;
const void *area_stages_data;
const void *area_tile_banks_table_data;
const void *area_layer_priorities_data;
const void *area_entry_without_blink_data;
const void *fire_priority_flicker_data;
const void *area_music_data;
const void *technodrome_explosion_sounds_data;
const void *ending_palette_fades_data;
const void *technodrome_explosion_x_data;
const void *technodrome_explosion_y_data;
const void *staff_roll_portrait_places_data;
const void *credit_name_tiles_1_data;
const void *credit_name_tiles_2_data;
const void *credit_name_tiles_3_data;
const void *credit_name_tiles_4_data;
const void *ranking_background_tiles_data;
const void *ranking_frame_places_data;
const void *ranking_label_places_data;
const void *ranking_place_places_data;
const void *ranking_initials_places_data;
const void *ranking_place_tiles_1_data;
const void *ranking_place_tiles_2_data;
const void *animation_viewer_animations_data;
const void *animation_viewer_palettes_data;
const void *animation_viewer_counts_data;
const void *turtle_picture_sprites_data;
const void *ranking_turtle_picture_sprites_data;
const void *best_players_places_data;
const void *best_players_colours_data;
const void *palette_request_handlers_data;
const void *palette_request_second_halves_data;
const void *palette_bank_counts_data;
const void *palette_bank_addresses_data;
const void *palette_banks_data;
const void *palette_effect_banks_data;
const void *palette_effect_stops_data;
const void *game_step_palettes_data;
const void *game_step_palette_effects_data;
const void *area_palette_effects_data;
const void *palette_editor_states_data;
const void *palette_editor_limits_data;
const void *palette_editor_banks_data;
const void *palette_editor_masks_data;
const void *palette_editor_steps_data;
const void *palette_editor_parts_data;
const void *palette_editor_place_data;
const void *palette_editor_title_data;
const void *palette_editor_number_places_data;
const void *palette_editor_colour_places_data;
const void *hex_digit_tiles_data;
const void *area_palettes_data;
const void *krang_appear_states_data;
const void *krang_walk_states_data;
const void *krang_melee_states_data;
const void *krang_lasers_states_data;
const void *krang_explode_states_data;
const void *krang_escape_states_data;
const void *krang_hover_heights_data;
const void *krang_move_chances_data;
const void *krang_attacks_in_front_data;
const void *krang_attacks_close_by_data;
const void *krang_move_times_data;
const void *krang_laser_place_x_data;
const void *krang_laser_place_height_data;
const void *krang_laser_x_speeds_data;
const void *krang_laser_height_speeds_data;
const void *krang_animations_data;
const void *krang_fist_animations_data;
const void *krang_laser_animations_data;
const void *ball_drop_waits_data;
const void *rat_start_offsets_data;
const void *rat_run_times_data;
const void *pizza_stats_by_kind_data;
const void *billboard_trigger_x_data;
const void *turret_wreck_states_data;
const void *turret_waits_data;
const void *nozzle_row_routines_data;
const void *nozzle_rows_data;
const void *freeze_nozzle_spray_states_data;
const void *turret_wreck_sounds_data;
const void *turret_wreck_waits_data;
const void *boss_explosion_x_data;
const void *boss_explosion_height_data;
const void *explosion_sounds_data;
const void *stockman_explosion_x_data;
const void *stockman_explosion_height_data;
const void *sewer_launcher_drift_x_data;
const void *sewer_launcher_drift_y_data;
const void *rock_soldier_site_enter_states_data;
const void *rock_soldier_site_attack_reaches_data;
const void *rock_soldier_site_states_data;
const void *rock_soldier_defeated_states_data;
const void *rock_soldier_crumble_times_data;
const void *rock_soldier_site_moves_data;
const void *rock_soldier_site_waits_data;
const void *rock_soldier_site_move_times_data;
const void *rubble_place_x_data;
const void *rubble_place_y_data;
const void *rock_soldier_technodrome_enter_states_data;
const void *rock_soldier_technodrome_states_data;
const void *rock_soldier_technodrome_moves_data;
const void *rock_soldier_technodrome_move_times_data;
const void *car_soldier_stop_y_data;
const void *helicopter_drift_y_data;
const void *helicopter_shake_heights_data;
const void *helicopter_bob_heights_data;
const void *flying_drone_starts_data;
const void *flying_drone_enter_states_data;
const void *flying_drone_shake_heights_data;
const void *flying_drone_bob_heights_data;
const void *flying_drone_aims_data;
const void *flying_drone_waits_data;
const void *flying_drone_x_speeds_data;
const void *flying_drone_y_speeds_data;
const void *flying_drone_sound_waits_data;
const void *drone_shot_place_x_data;
const void *drone_shot_place_height_data;
const void *drone_shot_x_speeds_data;
const void *drone_shot_y_speeds_data;
const void *stockman_enter_states_data;
const void *stockman_fly_moves_data;
const void *stockman_defeated_states_data;
const void *stockman_shake_heights_data;
const void *stockman_height_steps_data;
const void *stockman_step_heights_data;
const void *stockman_bob_heights_data;
const void *stockman_drop_times_data;
const void *stockman_x_speeds_data;
const void *stockman_y_speeds_data;
const void *profile_animations_data;
const void *hydrant_animations_data;
const void *hydrant_water_animations_data;
const void *rat_animations_data;
const void *alligator_animations_data;
const void *skateboarder_animations_data;
const void *pizza_animations_data;
const void *billboard_animations_data;
const void *billboard_soldier_animations_data;
const void *oil_drum_animations_data;
const void *turret_animations_data;
const void *turret_shot_animations_data;
const void *freeze_nozzle_animations_data;
const void *electric_beam_animations_data;
const void *explosion_small_animations_data;
const void *explosion_big_animations_data;
const void *explosion_drum_animations_data;
const void *rock_soldier_site_animations_data;
const void *gun_blast_animations_data;
const void *parked_car_animations_data;
const void *electric_column_animations_data;
const void *grille_animations_data;
const void *rubble_animations_data;
const void *rocket_animations_data;
const void *helicopter_animations_data;
const void *falling_missile_animations_data;
const void *copter_bullet_animations_data;
const void *rock_soldier_technodrome_animations_data;
const void *missile_animations_data;
const void *wall_slab_animations_data;
const void *nozzle_row_animations_data;
const void *waiting_soldier_animations_data;
const void *shock_bars_animations_data;
const void *staff_roll_portrait_animations_data;
const void *sewer_launcher_animations_data;
const void *spiked_pole_animations_data;
const void *flying_drone_animations_data;
const void *drone_shot_animations_data;
const void *explosion_medium_animations_data;
const void *one_way_sign_animations_data;
const void *stockman_animations_data;
const void *falling_mouser_animations_data;
const void *ball_animations_data;
const void *krang_curtain_animations_data;
const void *road_barrier_animations_data;
const void *tire_stack_animations_data;
const void *hex_digits_data;
const void *boss_axis_directions_data;
const void *boss_slope_directions_data;
const void *box_opening_place_data;
const void *unreached_box_opening_place_data;
const void *profile_texts_data;
const void *area_title_texts_data;
const void *area_turtle_starts_data;
const void *turtle_jump_in_x_data;
const void *turtle_jump_in_y_data;
const void *turtle_jump_in_speeds_data;
const void *turtle_marker_animations_data;
const void *area_entry_handlers_data;
const void *fall_in_entry_states_data;
const void *jump_in_entry_states_data;
const void *turtle_jump_in_heights_data;
const void *stage_scroll_locks_data;
const void *area_cleared_checks_data;
const void *turtle_wall_checks_data;
const void *turtle_walk_speeds_data;
const void *turtle_actions_data;
const void *turtle_jump_states_data;
const void *turtle_throw_moves_data;
const void *turtle_throw_counters_data;
const void *turtle_hit_animation_tables_data;
const void *turtle_attack_firsts_data;
const void *turtle_hit_handlers_data;
const void *turtle_hit_speeds_data;
const void *turtle_hit_waits_data;
const void *turtle_knocked_down_states_data;
const void *turtle_knocked_down_steps_data;
const void *turtle_knocked_back_states_data;
const void *turtle_knocked_back_steps_data;
const void *turtle_knocked_flying_states_data;
const void *turtle_knocked_flying_steps_data;
const void *turtle_knocked_flying_from_behind_states_data;
const void *turtle_knocked_flying_from_behind_steps_data;
const void *turtle_frozen_states_data;
const void *turtle_shocked_states_data;
const void *turtle_fall_in_manhole_states_data;
const void *turtle_held_states_data;
const void *turtle_held_briefly_states_data;
const void *turtle_held_knock_off_states_data;
const void *turtle_held_throw_off_states_data;
const void *turtle_struggle_bits_data;
const void *turtle_hit_reactions_data;
const void *turtle_attack_sounds_by_move_data;
const void *turtle_hit_sounds_data;
const void *turtle_attack_damages_data;
const void *turtle_combos_data;
const void *area_layer_starts_data;
const void *area_layer_a_x_data;
const void *area_layer_a_y_data;
const void *area_layer_b_x_data;
const void *area_layer_b_y_data;
const void *area_layer_a_maps_data;
const void *area_layer_a_blocks_data;
const void *area_layer_b_maps_data;
const void *area_layer_b_blocks_data;
const void *area_layer_a_map_widths_data;
const void *area_layer_b_map_widths_data;
const void *area_layer_updates_data;
const void *street_tv_pictures_data;
const void *technodrome_lift_steps_data;
const void *highway_car_steps_data;
const void *skateboard_road_steps_data;
const void *fire_tiles_data;
const void *fire_tiles_area_0_data;
const void *area_scroll_segments_data;
const void *turtle_animation_sets_data;
const void *area_exit_starts_data;
const void *area_exit_handlers_data;
const void *exit_apartment_steps_data;
const void *turtle_exit_street_steps_data;
const void *exit_street_jump_animations_data;
const void *exit_sewer_steps_data;
const void *turtle_exit_sewer_steps_data;
const void *exit_car_street_steps_data;
const void *turtle_exit_car_street_steps_data;
const void *car_street_exit_x_data;
const void *car_street_exit_y_data;
const void *van_start_x_shakes_data;
const void *van_start_y_shakes_data;
const void *exit_skateboard_steps_data;
const void *exit_places_x_data;
const void *exit_places_y_data;
const void *exit_building_site_steps_data;
const void *building_site_exit_x_data;
const void *building_site_exit_y_data;
const void *building_site_exit_heights_data;
const void *building_site_exit_drifts_data;
const void *exit_building_site_rise_animations_data;
const void *exit_final_steps_data;
const void *area_scroll_holds_data;
const void *enemy_limits_data;
const void *opening_steps_data;
const void *opening_turtle_y_speeds_data;
const void *balloon_records_data;
const void *drill_arrival_steps_data;
const void *shake_x_data;
const void *shake_y_data;
const void *intro_steps_data;
const void *intro_fire_frames_data;
const void *attack_hit_jobs_data;
const void *attack_check_objects_data;
const void *object_group_counts_data;
const void *object_groups_data;
const void *floor_contact_masks_data;
const void *area_hop_heights_data;
const void *area_stairs_top_heights_data;
const void *hit_reaction_down_data;
const void *enemy_edge_steps_data;
const void *scenery_fire_animations_data;
const void *whip_arm_follow_animations_data;
const void *whip_arm_offsets_data;
const void *rocksteady_bullet_animations_data;
const void *dynamite_flash_colours_data;
const void *door_piece_animations_data;
const void *door_piece_speeds_data;
const void *technodrome_door_soldiers_data;
const void *spear_soldier_hits_data;
const void *skateboard_soldier_colours_data;
const void *enemy_skateboard_animations_data;
const void *skateboard_soldier_definitions_data;
const void *rocksteady_moves_data;
const void *rocksteady_mode_times_data;
const void *rocksteady_mode_frames_data;
const void *rocksteady_hits_to_strike_back_by_difficulty_data;
const void *rocksteady_close_attack_waits_data;
const void *rocksteady_far_attack_waits_data;
const void *rocksteady_move_choices_data;
const void *rocksteady_hit_reactions_data;
const void *enemy_rocksteady_shoot_around_animations_data;
const void *rocksteady_bursts_data;
const void *enemy_rocksteady_flinch_animations_data;
const void *enemy_rocksteady_reel_animations_data;
const void *enemy_rocksteady_flinch_from_behind_animations_data;
const void *enemy_bebop_stand_animations_data;
const void *enemy_mouser_enter_animations_data;
const void *mouser_enter_y_speeds_data;
const void *mouser_enter_height_speeds_data;
const void *mouser_enter_heights_data;
const void *mouser_moves_data;
const void *mouser_edge_steps_data;
const void *enemy_mouser_sit_animations_data;
const void *mouser_hole_animation_tables_data;
const void *mouser_hole_counts_data;
const void *rodney_approach_moves_data;
const void *whip_arm_animations_data;
const void *shredder_clone_limits_data;
const void *shredder_guard_waits_data;
const void *shredder_attack_waits_data;
const void *shredder_ray_waits_data;
const void *shredder_approach_moves_data;
const void *object_routine_tables_data;
const void *enemy_attacks_data;
const void *enemy_ways_in_data;

/* the tables the game loads from its assets: their names, addresses, pointers and the bytes of
   their elements (the loader puts each element in the host's byte order) */
const game_table game_tables[] =
{
	{ "job_handlers", 0x0013DEu, &job_handlers_data, 4 },
	{ "lives_settings", 0x001426u, &lives_settings_data, 1 },
	{ "coinage_coins", 0x001452u, &coinage_coins_data, 1 },
	{ "coinage_credits", 0x001453u, &coinage_credits_data, 1 },
	{ "coinage_2_coins", 0x001472u, &coinage_2_coins_data, 1 },
	{ "coinage_2_credits", 0x001473u, &coinage_2_credits_data, 1 },
	{ "boot_texts", 0x0015ACu, &boot_texts_data, 4 },
	{ "text_colours", 0x001F1Au, &text_colours_data, 1 },
	{ "player_message_handlers", 0x002678u, &player_message_handlers_data, 4 },
	{ "text_layer_places", 0x0027A2u, &text_layer_places_data, 4 },
	{ "big_font_place", 0x002802u, &big_font_place_data, 4 },
	{ "big_font_tiles", 0x002806u, &big_font_tiles_data, 1 },
	{ "big_font_tiles_table", 0x002884u, &big_font_tiles_table_data, 1 },
	{ "colour_texts", 0x002F36u, &colour_texts_data, 4 },
	{ "tile_pictures", 0x0031F2u, &tile_pictures_data, 4 },
	{ "player_box_places", 0x0032C2u, &player_box_places_data, 4 },
	{ "box_labels", 0x00332Au, &box_labels_data, 4 },
	{ "lives_digit_top", 0x0033A2u, &lives_digit_top_data, 1 },
	{ "lives_digit_bottom", 0x0033A3u, &lives_digit_bottom_data, 1 },
	{ "lives_icon_top", 0x0033B6u, &lives_icon_top_data, 1 },
	{ "lives_icon_bottom", 0x0033B7u, &lives_icon_bottom_data, 1 },
	{ "stage_titles", 0x003402u, &stage_titles_data, 4 },
	{ "positioned_texts", 0x0035ACu, &positioned_texts_data, 4 },
	{ "tall_texts", 0x00375Cu, &tall_texts_data, 4 },
	{ "game_state_handlers", 0x003E60u, &game_state_handlers_data, 4 },
	{ "startup_steps", 0x003F8Eu, &startup_steps_data, 4 },
	{ "attract_steps", 0x004100u, &attract_steps_data, 4 },
	{ "game_scene_handlers", 0x00413Au, &game_scene_handlers_data, 4 },
	{ "demo_settings_table", 0x004E88u, &demo_settings_table_data, 1 },
	{ "demo_input_streams", 0x004E94u, &demo_input_streams_data, 4 },
	{ "color_test_places", 0x00584Cu, &color_test_places_data, 4 },
	{ "player_input_ports", 0x0059A6u, &player_input_ports_data, 4 },
	{ "player_input_places", 0x0059B6u, &player_input_places_data, 4 },
	{ "coin_input_port", 0x0059C6u, &coin_input_port_data, 4 },
	{ "coin_input_places", 0x0059CAu, &coin_input_places_data, 4 },
	{ "dip_switch_ports", 0x0059D2u, &dip_switch_ports_data, 4 },
	{ "dip_switch_places", 0x0059DEu, &dip_switch_places_data, 4 },
	{ "coin_counter_places", 0x005B40u, &coin_counter_places_data, 4 },
	{ "sound_test_codes", 0x006146u, &sound_test_codes_data, 1 },
	{ "area_continue_scenes", 0x00634Cu, &area_continue_scenes_data, 1 },
	{ "area_stages", 0x00634Du, &area_stages_data, 1 },
	{ "area_tile_banks_table", 0x00676Eu, &area_tile_banks_table_data, 1 },
	{ "area_layer_priorities", 0x006782u, &area_layer_priorities_data, 1 },
	{ "area_entry_without_blink", 0x00678Cu, &area_entry_without_blink_data, 1 },
	{ "fire_priority_flicker", 0x006A18u, &fire_priority_flicker_data, 1 },
	{ "area_music", 0x006B0Cu, &area_music_data, 1 },
	{ "technodrome_explosion_sounds", 0x006DFCu, &technodrome_explosion_sounds_data, 1 },
	{ "ending_palette_fades", 0x0071DCu, &ending_palette_fades_data, 6 },
	{ "technodrome_explosion_x", 0x0072E2u, &technodrome_explosion_x_data, 2 },
	{ "technodrome_explosion_y", 0x0072E4u, &technodrome_explosion_y_data, 2 },
	{ "staff_roll_portrait_places", 0x00738Cu, &staff_roll_portrait_places_data, 2 },
	{ "credit_name_tiles_1", 0x007592u, &credit_name_tiles_1_data, 1 },
	{ "credit_name_tiles_2", 0x007593u, &credit_name_tiles_2_data, 1 },
	{ "credit_name_tiles_3", 0x007594u, &credit_name_tiles_3_data, 1 },
	{ "credit_name_tiles_4", 0x007595u, &credit_name_tiles_4_data, 1 },
	{ "ranking_background_tiles", 0x00807Eu, &ranking_background_tiles_data, 1 },
	{ "ranking_frame_places", 0x0083DAu, &ranking_frame_places_data, 4 },
	{ "ranking_label_places", 0x0083EAu, &ranking_label_places_data, 4 },
	{ "ranking_place_places", 0x0083FAu, &ranking_place_places_data, 4 },
	{ "ranking_initials_places", 0x00840Au, &ranking_initials_places_data, 4 },
	{ "ranking_place_tiles_1", 0x008436u, &ranking_place_tiles_1_data, 1 },
	{ "ranking_place_tiles_2", 0x008437u, &ranking_place_tiles_2_data, 1 },
	{ "animation_viewer_animations", 0x0085BCu, &animation_viewer_animations_data, 4 },
	{ "animation_viewer_palettes", 0x0085C0u, &animation_viewer_palettes_data, 4 },
	{ "animation_viewer_counts", 0x008850u, &animation_viewer_counts_data, 1 },
	{ "turtle_picture_sprites", 0x008BEAu, &turtle_picture_sprites_data, 1 },
	{ "ranking_turtle_picture_sprites", 0x008C2Au, &ranking_turtle_picture_sprites_data, 1 },
	{ "best_players_places", 0x00917Eu, &best_players_places_data, 4 },
	{ "best_players_colours", 0x009186u, &best_players_colours_data, 1 },
	{ "palette_request_handlers", 0x009310u, &palette_request_handlers_data, 4 },
	{ "palette_request_second_halves", 0x009334u, &palette_request_second_halves_data, 4 },
	{ "palette_bank_counts", 0x0095D2u, &palette_bank_counts_data, 1 },
	{ "palette_bank_addresses", 0x0095D6u, &palette_bank_addresses_data, 4 },
	{ "palette_banks", 0x00975Cu, &palette_banks_data, 4 },
	{ "palette_effect_banks", 0x0098F2u, &palette_effect_banks_data, 4 },
	{ "palette_effect_stops", 0x0099F0u, &palette_effect_stops_data, 4 },
	{ "game_step_palettes", 0x009A7Cu, &game_step_palettes_data, 4 },
	{ "game_step_palette_effects", 0x009AD2u, &game_step_palette_effects_data, 4 },
	{ "area_palette_effects", 0x009B28u, &area_palette_effects_data, 4 },
	{ "palette_editor_states", 0x009C30u, &palette_editor_states_data, 4 },
	{ "palette_editor_limits", 0x009D40u, &palette_editor_limits_data, 1 },
	{ "palette_editor_banks", 0x009EFAu, &palette_editor_banks_data, 4 },
	{ "palette_editor_masks", 0x009F0Au, &palette_editor_masks_data, 2 },
	{ "palette_editor_steps", 0x009F0Cu, &palette_editor_steps_data, 2 },
	{ "palette_editor_parts", 0x009F16u, &palette_editor_parts_data, 1 },
	{ "palette_editor_place", 0x009F1Au, &palette_editor_place_data, 4 },
	{ "palette_editor_title", 0x009F1Eu, &palette_editor_title_data, 1 },
	{ "palette_editor_number_places", 0x009F22u, &palette_editor_number_places_data, 4 },
	{ "palette_editor_colour_places", 0x009F2Eu, &palette_editor_colour_places_data, 4 },
	{ "hex_digit_tiles", 0x009F46u, &hex_digit_tiles_data, 1 },
	{ "area_palettes", 0x009F56u, &area_palettes_data, 4 },
	{ "krang_appear_states", 0x00DF92u, &krang_appear_states_data, 4 },
	{ "krang_walk_states", 0x00E082u, &krang_walk_states_data, 4 },
	{ "krang_melee_states", 0x00E13Cu, &krang_melee_states_data, 4 },
	{ "krang_lasers_states", 0x00E1C0u, &krang_lasers_states_data, 4 },
	{ "krang_explode_states", 0x00E2F0u, &krang_explode_states_data, 4 },
	{ "krang_escape_states", 0x00E3B0u, &krang_escape_states_data, 4 },
	{ "krang_hover_heights", 0x00E48Au, &krang_hover_heights_data, 2 },
	{ "krang_move_chances", 0x00E4FEu, &krang_move_chances_data, 1 },
	{ "krang_attacks_in_front", 0x00E546u, &krang_attacks_in_front_data, 4 },
	{ "krang_attacks_close_by", 0x00E582u, &krang_attacks_close_by_data, 4 },
	{ "krang_move_times", 0x00E700u, &krang_move_times_data, 1 },
	{ "krang_laser_place_x", 0x00EA9Eu, &krang_laser_place_x_data, 2 },
	{ "krang_laser_place_height", 0x00EAA0u, &krang_laser_place_height_data, 2 },
	{ "krang_laser_x_speeds", 0x00EB1Eu, &krang_laser_x_speeds_data, 2 },
	{ "krang_laser_height_speeds", 0x00EB20u, &krang_laser_height_speeds_data, 2 },
	{ "krang_animations", 0x00EC98u, &krang_animations_data, 4 },
	{ "krang_fist_animations", 0x00F674u, &krang_fist_animations_data, 4 },
	{ "krang_laser_animations", 0x00F760u, &krang_laser_animations_data, 4 },
	{ "ball_drop_waits", 0x00F938u, &ball_drop_waits_data, 2 },
	{ "rat_start_offsets", 0x00FF60u, &rat_start_offsets_data, 4 },
	{ "rat_run_times", 0x010012u, &rat_run_times_data, 1 },
	{ "pizza_stats_by_kind", 0x010148u, &pizza_stats_by_kind_data, 4 },
	{ "billboard_trigger_x", 0x010232u, &billboard_trigger_x_data, 2 },
	{ "turret_wreck_states", 0x010616u, &turret_wreck_states_data, 4 },
	{ "turret_waits", 0x0107AEu, &turret_waits_data, 1 },
	{ "nozzle_row_routines", 0x010A0Eu, &nozzle_row_routines_data, 4 },
	{ "nozzle_rows", 0x010AB2u, &nozzle_rows_data, 4 },
	{ "freeze_nozzle_spray_states", 0x010B7Eu, &freeze_nozzle_spray_states_data, 4 },
	{ "turret_wreck_sounds", 0x010EAAu, &turret_wreck_sounds_data, 1 },
	{ "turret_wreck_waits", 0x010EAEu, &turret_wreck_waits_data, 2 },
	{ "boss_explosion_x", 0x010F1Cu, &boss_explosion_x_data, 2 },
	{ "boss_explosion_height", 0x010F1Eu, &boss_explosion_height_data, 2 },
	{ "explosion_sounds", 0x010F44u, &explosion_sounds_data, 1 },
	{ "stockman_explosion_x", 0x010F62u, &stockman_explosion_x_data, 2 },
	{ "stockman_explosion_height", 0x010F64u, &stockman_explosion_height_data, 2 },
	{ "sewer_launcher_drift_x", 0x0113BEu, &sewer_launcher_drift_x_data, 2 },
	{ "sewer_launcher_drift_y", 0x0113C0u, &sewer_launcher_drift_y_data, 2 },
	{ "rock_soldier_site_enter_states", 0x011562u, &rock_soldier_site_enter_states_data, 4 },
	{ "rock_soldier_site_attack_reaches", 0x01175Au, &rock_soldier_site_attack_reaches_data, 1 },
	{ "rock_soldier_site_states", 0x011782u, &rock_soldier_site_states_data, 4 },
	{ "rock_soldier_defeated_states", 0x0119F2u, &rock_soldier_defeated_states_data, 4 },
	{ "rock_soldier_crumble_times", 0x011AD2u, &rock_soldier_crumble_times_data, 1 },
	{ "rock_soldier_site_moves", 0x011D1Au, &rock_soldier_site_moves_data, 1 },
	{ "rock_soldier_site_waits", 0x011DEEu, &rock_soldier_site_waits_data, 1 },
	{ "rock_soldier_site_move_times", 0x011DF2u, &rock_soldier_site_move_times_data, 1 },
	{ "rubble_place_x", 0x012108u, &rubble_place_x_data, 1 },
	{ "rubble_place_y", 0x012109u, &rubble_place_y_data, 1 },
	{ "rock_soldier_technodrome_enter_states", 0x0121ACu, &rock_soldier_technodrome_enter_states_data, 4 },
	{ "rock_soldier_technodrome_states", 0x012360u, &rock_soldier_technodrome_states_data, 4 },
	{ "rock_soldier_technodrome_moves", 0x01255Eu, &rock_soldier_technodrome_moves_data, 1 },
	{ "rock_soldier_technodrome_move_times", 0x012628u, &rock_soldier_technodrome_move_times_data, 1 },
	{ "car_soldier_stop_y", 0x012CFEu, &car_soldier_stop_y_data, 2 },
	{ "helicopter_drift_y", 0x013092u, &helicopter_drift_y_data, 2 },
	{ "helicopter_shake_heights", 0x0130F4u, &helicopter_shake_heights_data, 2 },
	{ "helicopter_bob_heights", 0x0130FCu, &helicopter_bob_heights_data, 2 },
	{ "flying_drone_starts", 0x013486u, &flying_drone_starts_data, 1 },
	{ "flying_drone_enter_states", 0x0134CAu, &flying_drone_enter_states_data, 4 },
	{ "flying_drone_shake_heights", 0x0138ACu, &flying_drone_shake_heights_data, 2 },
	{ "flying_drone_bob_heights", 0x0138B4u, &flying_drone_bob_heights_data, 2 },
	{ "flying_drone_aims", 0x0139A2u, &flying_drone_aims_data, 1 },
	{ "flying_drone_waits", 0x0139D6u, &flying_drone_waits_data, 1 },
	{ "flying_drone_x_speeds", 0x0139DAu, &flying_drone_x_speeds_data, 2 },
	{ "flying_drone_y_speeds", 0x0139DCu, &flying_drone_y_speeds_data, 2 },
	{ "flying_drone_sound_waits", 0x013A74u, &flying_drone_sound_waits_data, 1 },
	{ "drone_shot_place_x", 0x013AE4u, &drone_shot_place_x_data, 2 },
	{ "drone_shot_place_height", 0x013AE6u, &drone_shot_place_height_data, 2 },
	{ "drone_shot_x_speeds", 0x013B72u, &drone_shot_x_speeds_data, 2 },
	{ "drone_shot_y_speeds", 0x013B74u, &drone_shot_y_speeds_data, 2 },
	{ "stockman_enter_states", 0x013C46u, &stockman_enter_states_data, 4 },
	{ "stockman_fly_moves", 0x013E30u, &stockman_fly_moves_data, 1 },
	{ "stockman_defeated_states", 0x013F08u, &stockman_defeated_states_data, 4 },
	{ "stockman_shake_heights", 0x01416Eu, &stockman_shake_heights_data, 2 },
	{ "stockman_height_steps", 0x014176u, &stockman_height_steps_data, 2 },
	{ "stockman_step_heights", 0x014194u, &stockman_step_heights_data, 2 },
	{ "stockman_bob_heights", 0x0141B4u, &stockman_bob_heights_data, 2 },
	{ "stockman_drop_times", 0x01424Au, &stockman_drop_times_data, 1 },
	{ "stockman_x_speeds", 0x01426Au, &stockman_x_speeds_data, 2 },
	{ "stockman_y_speeds", 0x01426Cu, &stockman_y_speeds_data, 2 },
	{ "profile_animations", 0x0143D4u, &profile_animations_data, 4 },
	{ "hydrant_animations", 0x01443Eu, &hydrant_animations_data, 4 },
	{ "hydrant_water_animations", 0x0144E8u, &hydrant_water_animations_data, 4 },
	{ "rat_animations", 0x014598u, &rat_animations_data, 4 },
	{ "alligator_animations", 0x0145FCu, &alligator_animations_data, 4 },
	{ "skateboarder_animations", 0x01461Au, &skateboarder_animations_data, 4 },
	{ "pizza_animations", 0x014636u, &pizza_animations_data, 4 },
	{ "billboard_animations", 0x01480Eu, &billboard_animations_data, 4 },
	{ "billboard_soldier_animations", 0x014860u, &billboard_soldier_animations_data, 4 },
	{ "oil_drum_animations", 0x01488Au, &oil_drum_animations_data, 4 },
	{ "turret_animations", 0x014A26u, &turret_animations_data, 4 },
	{ "turret_shot_animations", 0x014AA2u, &turret_shot_animations_data, 4 },
	{ "freeze_nozzle_animations", 0x014AD2u, &freeze_nozzle_animations_data, 4 },
	{ "electric_beam_animations", 0x014BA0u, &electric_beam_animations_data, 4 },
	{ "explosion_small_animations", 0x014C3Au, &explosion_small_animations_data, 4 },
	{ "explosion_big_animations", 0x014C86u, &explosion_big_animations_data, 4 },
	{ "explosion_drum_animations", 0x014D1Cu, &explosion_drum_animations_data, 4 },
	{ "rock_soldier_site_animations", 0x014DA0u, &rock_soldier_site_animations_data, 4 },
	{ "gun_blast_animations", 0x014FBEu, &gun_blast_animations_data, 4 },
	{ "parked_car_animations", 0x015042u, &parked_car_animations_data, 4 },
	{ "electric_column_animations", 0x0150E8u, &electric_column_animations_data, 4 },
	{ "grille_animations", 0x01519Cu, &grille_animations_data, 4 },
	{ "rubble_animations", 0x0151AAu, &rubble_animations_data, 4 },
	{ "rocket_animations", 0x015258u, &rocket_animations_data, 4 },
	{ "helicopter_animations", 0x0152EEu, &helicopter_animations_data, 4 },
	{ "falling_missile_animations", 0x01534Au, &falling_missile_animations_data, 4 },
	{ "copter_bullet_animations", 0x015360u, &copter_bullet_animations_data, 4 },
	{ "rock_soldier_technodrome_animations", 0x0153D8u, &rock_soldier_technodrome_animations_data, 4 },
	{ "missile_animations", 0x015692u, &missile_animations_data, 4 },
	{ "wall_slab_animations", 0x0156DAu, &wall_slab_animations_data, 4 },
	{ "nozzle_row_animations", 0x0156F0u, &nozzle_row_animations_data, 4 },
	{ "waiting_soldier_animations", 0x0156FEu, &waiting_soldier_animations_data, 4 },
	{ "shock_bars_animations", 0x01574Eu, &shock_bars_animations_data, 4 },
	{ "staff_roll_portrait_animations", 0x015764u, &staff_roll_portrait_animations_data, 4 },
	{ "sewer_launcher_animations", 0x01579Cu, &sewer_launcher_animations_data, 4 },
	{ "spiked_pole_animations", 0x015838u, &spiked_pole_animations_data, 4 },
	{ "flying_drone_animations", 0x01584Eu, &flying_drone_animations_data, 4 },
	{ "drone_shot_animations", 0x015938u, &drone_shot_animations_data, 4 },
	{ "explosion_medium_animations", 0x015AA0u, &explosion_medium_animations_data, 4 },
	{ "one_way_sign_animations", 0x015B36u, &one_way_sign_animations_data, 4 },
	{ "stockman_animations", 0x015C70u, &stockman_animations_data, 4 },
	{ "falling_mouser_animations", 0x015D4Au, &falling_mouser_animations_data, 4 },
	{ "ball_animations", 0x015D68u, &ball_animations_data, 4 },
	{ "krang_curtain_animations", 0x015E02u, &krang_curtain_animations_data, 4 },
	{ "road_barrier_animations", 0x015EBEu, &road_barrier_animations_data, 4 },
	{ "tire_stack_animations", 0x015EEAu, &tire_stack_animations_data, 4 },
	{ "hex_digits", 0x0184C2u, &hex_digits_data, 1 },
	{ "boss_axis_directions", 0x018550u, &boss_axis_directions_data, 1 },
	{ "boss_slope_directions", 0x018560u, &boss_slope_directions_data, 1 },
	{ "box_opening_place", 0x01867Eu, &box_opening_place_data, 4 },
	{ "unreached_box_opening_place", 0x018710u, &unreached_box_opening_place_data, 4 },
	{ "profile_texts", 0x019272u, &profile_texts_data, 4 },
	{ "area_title_texts", 0x019866u, &area_title_texts_data, 4 },
	{ "area_turtle_starts", 0x019D54u, &area_turtle_starts_data, 4 },
	{ "turtle_jump_in_x", 0x019EB2u, &turtle_jump_in_x_data, 2 },
	{ "turtle_jump_in_y", 0x019EB4u, &turtle_jump_in_y_data, 2 },
	{ "turtle_jump_in_speeds", 0x019EB6u, &turtle_jump_in_speeds_data, 2 },
	{ "turtle_marker_animations", 0x019F24u, &turtle_marker_animations_data, 4 },
	{ "area_entry_handlers", 0x019F92u, &area_entry_handlers_data, 4 },
	{ "fall_in_entry_states", 0x019FDEu, &fall_in_entry_states_data, 4 },
	{ "jump_in_entry_states", 0x01A14Eu, &jump_in_entry_states_data, 4 },
	{ "turtle_jump_in_heights", 0x01A2D2u, &turtle_jump_in_heights_data, 1 },
	{ "stage_scroll_locks", 0x01A3C0u, &stage_scroll_locks_data, 1 },
	{ "area_cleared_checks", 0x01A88Cu, &area_cleared_checks_data, 4 },
	{ "turtle_wall_checks", 0x01AC9Au, &turtle_wall_checks_data, 4 },
	{ "turtle_walk_speeds", 0x01B5B2u, &turtle_walk_speeds_data, 2 },
	{ "turtle_actions", 0x01B714u, &turtle_actions_data, 4 },
	{ "turtle_jump_states", 0x01BA88u, &turtle_jump_states_data, 4 },
	{ "turtle_throw_moves", 0x01C072u, &turtle_throw_moves_data, 2 },
	{ "turtle_throw_counters", 0x01C074u, &turtle_throw_counters_data, 4 },
	{ "turtle_hit_animation_tables", 0x01C08Au, &turtle_hit_animation_tables_data, 4 },
	{ "turtle_attack_firsts", 0x01C20Eu, &turtle_attack_firsts_data, 1 },
	{ "turtle_hit_handlers", 0x01C272u, &turtle_hit_handlers_data, 4 },
	{ "turtle_hit_speeds", 0x01C546u, &turtle_hit_speeds_data, 2 },
	{ "turtle_hit_waits", 0x01C596u, &turtle_hit_waits_data, 2 },
	{ "turtle_knocked_down_states", 0x01C678u, &turtle_knocked_down_states_data, 4 },
	{ "turtle_knocked_down_steps", 0x01C764u, &turtle_knocked_down_steps_data, 4 },
	{ "turtle_knocked_back_states", 0x01C7D0u, &turtle_knocked_back_states_data, 4 },
	{ "turtle_knocked_back_steps", 0x01C8CAu, &turtle_knocked_back_steps_data, 4 },
	{ "turtle_knocked_flying_states", 0x01C8F8u, &turtle_knocked_flying_states_data, 4 },
	{ "turtle_knocked_flying_steps", 0x01CA82u, &turtle_knocked_flying_steps_data, 4 },
	{ "turtle_knocked_flying_from_behind_states", 0x01CAC2u, &turtle_knocked_flying_from_behind_states_data, 4 },
	{ "turtle_knocked_flying_from_behind_steps", 0x01CC6Eu, &turtle_knocked_flying_from_behind_steps_data, 4 },
	{ "turtle_frozen_states", 0x01CCAAu, &turtle_frozen_states_data, 4 },
	{ "turtle_shocked_states", 0x01CD44u, &turtle_shocked_states_data, 4 },
	{ "turtle_fall_in_manhole_states", 0x01CDBCu, &turtle_fall_in_manhole_states_data, 4 },
	{ "turtle_held_states", 0x01CE52u, &turtle_held_states_data, 4 },
	{ "turtle_held_briefly_states", 0x01CF4Eu, &turtle_held_briefly_states_data, 4 },
	{ "turtle_held_knock_off_states", 0x01D028u, &turtle_held_knock_off_states_data, 4 },
	{ "turtle_held_throw_off_states", 0x01D100u, &turtle_held_throw_off_states_data, 4 },
	{ "turtle_struggle_bits", 0x01D200u, &turtle_struggle_bits_data, 1 },
	{ "turtle_hit_reactions", 0x01D3FCu, &turtle_hit_reactions_data, 1 },
	{ "turtle_attack_sounds_by_move", 0x01D526u, &turtle_attack_sounds_by_move_data, 1 },
	{ "turtle_hit_sounds", 0x01D556u, &turtle_hit_sounds_data, 1 },
	{ "turtle_attack_damages", 0x01D592u, &turtle_attack_damages_data, 2 },
	{ "turtle_combos", 0x01D5F2u, &turtle_combos_data, 4 },
	{ "area_layer_starts", 0x01E2DAu, &area_layer_starts_data, 4 },
	{ "area_layer_a_x", 0x01E5A0u, &area_layer_a_x_data, 4 },
	{ "area_layer_a_y", 0x01E5A4u, &area_layer_a_y_data, 4 },
	{ "area_layer_b_x", 0x01E5A8u, &area_layer_b_x_data, 4 },
	{ "area_layer_b_y", 0x01E5ACu, &area_layer_b_y_data, 4 },
	{ "area_layer_a_maps", 0x01E5B0u, &area_layer_a_maps_data, 4 },
	{ "area_layer_a_blocks", 0x01E5B4u, &area_layer_a_blocks_data, 4 },
	{ "area_layer_b_maps", 0x01E5B8u, &area_layer_b_maps_data, 4 },
	{ "area_layer_b_blocks", 0x01E5BCu, &area_layer_b_blocks_data, 4 },
	{ "area_layer_a_map_widths", 0x01E5C0u, &area_layer_a_map_widths_data, 2 },
	{ "area_layer_b_map_widths", 0x01E5C2u, &area_layer_b_map_widths_data, 2 },
	{ "area_layer_updates", 0x01ECECu, &area_layer_updates_data, 4 },
	{ "street_tv_pictures", 0x01F158u, &street_tv_pictures_data, 4 },
	{ "technodrome_lift_steps", 0x01F43Au, &technodrome_lift_steps_data, 4 },
	{ "highway_car_steps", 0x01F82Cu, &highway_car_steps_data, 4 },
	{ "skateboard_road_steps", 0x01FC96u, &skateboard_road_steps_data, 4 },
	{ "fire_tiles", 0x0200F2u, &fire_tiles_data, 4 },
	{ "fire_tiles_area_0", 0x0203D2u, &fire_tiles_area_0_data, 4 },
	{ "area_scroll_segments", 0x0205F2u, &area_scroll_segments_data, 4 },
	{ "turtle_animation_sets", 0x0208C0u, &turtle_animation_sets_data, 4 },
	{ "area_exit_starts", 0x02B2C6u, &area_exit_starts_data, 4 },
	{ "area_exit_handlers", 0x02B62Au, &area_exit_handlers_data, 4 },
	{ "exit_apartment_steps", 0x02B728u, &exit_apartment_steps_data, 4 },
	{ "turtle_exit_street_steps", 0x02BB16u, &turtle_exit_street_steps_data, 4 },
	{ "exit_street_jump_animations", 0x02BC56u, &exit_street_jump_animations_data, 4 },
	{ "exit_sewer_steps", 0x02BC98u, &exit_sewer_steps_data, 4 },
	{ "turtle_exit_sewer_steps", 0x02BD44u, &turtle_exit_sewer_steps_data, 4 },
	{ "exit_car_street_steps", 0x02BF10u, &exit_car_street_steps_data, 4 },
	{ "turtle_exit_car_street_steps", 0x02C04Cu, &turtle_exit_car_street_steps_data, 4 },
	{ "car_street_exit_x", 0x02C0F0u, &car_street_exit_x_data, 2 },
	{ "car_street_exit_y", 0x02C0F2u, &car_street_exit_y_data, 2 },
	{ "van_start_x_shakes", 0x02C3BAu, &van_start_x_shakes_data, 2 },
	{ "van_start_y_shakes", 0x02C3BCu, &van_start_y_shakes_data, 2 },
	{ "exit_skateboard_steps", 0x02C93Au, &exit_skateboard_steps_data, 4 },
	{ "exit_places_x", 0x02CE62u, &exit_places_x_data, 2 },
	{ "exit_places_y", 0x02CE64u, &exit_places_y_data, 2 },
	{ "exit_building_site_steps", 0x02CE8Eu, &exit_building_site_steps_data, 4 },
	{ "building_site_exit_x", 0x02D304u, &building_site_exit_x_data, 2 },
	{ "building_site_exit_y", 0x02D306u, &building_site_exit_y_data, 2 },
	{ "building_site_exit_heights", 0x02D308u, &building_site_exit_heights_data, 2 },
	{ "building_site_exit_drifts", 0x02D30Au, &building_site_exit_drifts_data, 2 },
	{ "exit_building_site_rise_animations", 0x02D4F4u, &exit_building_site_rise_animations_data, 4 },
	{ "exit_final_steps", 0x02D616u, &exit_final_steps_data, 4 },
	{ "area_scroll_holds", 0x02F654u, &area_scroll_holds_data, 4 },
	{ "enemy_limits", 0x02F67Cu, &enemy_limits_data, 1 },
	{ "opening_steps", 0x02F8C6u, &opening_steps_data, 4 },
	{ "opening_turtle_y_speeds", 0x02FBE0u, &opening_turtle_y_speeds_data, 2 },
	{ "balloon_records", 0x030350u, &balloon_records_data, 1 },
	{ "drill_arrival_steps", 0x0303EEu, &drill_arrival_steps_data, 4 },
	{ "shake_x", 0x030652u, &shake_x_data, 1 },
	{ "shake_y", 0x030653u, &shake_y_data, 1 },
	{ "intro_steps", 0x030996u, &intro_steps_data, 4 },
	{ "intro_fire_frames", 0x031152u, &intro_fire_frames_data, 4 },
	{ "attack_hit_jobs", 0x0471F0u, &attack_hit_jobs_data, 1 },
	{ "attack_check_objects", 0x0471F4u, &attack_check_objects_data, 4 },
	{ "object_group_counts", 0x047BDEu, &object_group_counts_data, 1 },
	{ "object_groups", 0x047BE2u, &object_groups_data, 4 },
	{ "floor_contact_masks", 0x048818u, &floor_contact_masks_data, 1 },
	{ "area_hop_heights", 0x04881Cu, &area_hop_heights_data, 2 },
	{ "area_stairs_top_heights", 0x048830u, &area_stairs_top_heights_data, 1 },
	{ "hit_reaction_down", 0x048C5Eu, &hit_reaction_down_data, 1 },
	{ "enemy_edge_steps", 0x048F36u, &enemy_edge_steps_data, 4 },
	{ "scenery_fire_animations", 0x04A8A4u, &scenery_fire_animations_data, 4 },
	{ "whip_arm_follow_animations", 0x04B742u, &whip_arm_follow_animations_data, 4 },
	{ "whip_arm_offsets", 0x04B7DEu, &whip_arm_offsets_data, 2 },
	{ "rocksteady_bullet_animations", 0x04BFD6u, &rocksteady_bullet_animations_data, 4 },
	{ "dynamite_flash_colours", 0x04C3EEu, &dynamite_flash_colours_data, 1 },
	{ "door_piece_animations", 0x04C5D4u, &door_piece_animations_data, 4 },
	{ "door_piece_speeds", 0x04C5E4u, &door_piece_speeds_data, 2 },
	{ "technodrome_door_soldiers", 0x04D764u, &technodrome_door_soldiers_data, 1 },
	{ "spear_soldier_hits", 0x04DD2Cu, &spear_soldier_hits_data, 1 },
	{ "skateboard_soldier_colours", 0x04EFD0u, &skateboard_soldier_colours_data, 1 },
	{ "enemy_skateboard_animations", 0x04EFD4u, &enemy_skateboard_animations_data, 4 },
	{ "skateboard_soldier_definitions", 0x04EFE0u, &skateboard_soldier_definitions_data, 4 },
	{ "rocksteady_moves", 0x04F58Cu, &rocksteady_moves_data, 2 },
	{ "rocksteady_mode_times", 0x04F58Eu, &rocksteady_mode_times_data, 2 },
	{ "rocksteady_mode_frames", 0x04F594u, &rocksteady_mode_frames_data, 2 },
	{ "rocksteady_hits_to_strike_back_by_difficulty", 0x04F5B4u, &rocksteady_hits_to_strike_back_by_difficulty_data, 1 },
	{ "rocksteady_close_attack_waits", 0x04F5C4u, &rocksteady_close_attack_waits_data, 1 },
	{ "rocksteady_far_attack_waits", 0x04F5CCu, &rocksteady_far_attack_waits_data, 1 },
	{ "rocksteady_move_choices", 0x04F5D4u, &rocksteady_move_choices_data, 1 },
	{ "rocksteady_hit_reactions", 0x04F6B6u, &rocksteady_hit_reactions_data, 1 },
	{ "enemy_rocksteady_shoot_around_animations", 0x04F9DAu, &enemy_rocksteady_shoot_around_animations_data, 4 },
	{ "rocksteady_bursts", 0x04F9DEu, &rocksteady_bursts_data, 4 },
	{ "enemy_rocksteady_flinch_animations", 0x04FE56u, &enemy_rocksteady_flinch_animations_data, 4 },
	{ "enemy_rocksteady_reel_animations", 0x04FEBEu, &enemy_rocksteady_reel_animations_data, 4 },
	{ "enemy_rocksteady_flinch_from_behind_animations", 0x04FEF6u, &enemy_rocksteady_flinch_from_behind_animations_data, 4 },
	{ "enemy_bebop_stand_animations", 0x050692u, &enemy_bebop_stand_animations_data, 4 },
	{ "enemy_mouser_enter_animations", 0x05101Eu, &enemy_mouser_enter_animations_data, 4 },
	{ "mouser_enter_y_speeds", 0x05102Au, &mouser_enter_y_speeds_data, 2 },
	{ "mouser_enter_height_speeds", 0x05102Cu, &mouser_enter_height_speeds_data, 2 },
	{ "mouser_enter_heights", 0x051036u, &mouser_enter_heights_data, 1 },
	{ "mouser_moves", 0x0512E4u, &mouser_moves_data, 2 },
	{ "mouser_edge_steps", 0x0514C6u, &mouser_edge_steps_data, 4 },
	{ "enemy_mouser_sit_animations", 0x0518CCu, &enemy_mouser_sit_animations_data, 4 },
	{ "mouser_hole_animation_tables", 0x051AE2u, &mouser_hole_animation_tables_data, 4 },
	{ "mouser_hole_counts", 0x051AEEu, &mouser_hole_counts_data, 1 },
	{ "rodney_approach_moves", 0x051F9Cu, &rodney_approach_moves_data, 1 },
	{ "whip_arm_animations", 0x052450u, &whip_arm_animations_data, 4 },
	{ "shredder_clone_limits", 0x052918u, &shredder_clone_limits_data, 1 },
	{ "shredder_guard_waits", 0x052A60u, &shredder_guard_waits_data, 1 },
	{ "shredder_attack_waits", 0x052ADEu, &shredder_attack_waits_data, 1 },
	{ "shredder_ray_waits", 0x052B3Au, &shredder_ray_waits_data, 2 },
	{ "shredder_approach_moves", 0x052B7Cu, &shredder_approach_moves_data, 2 },
	{ "object_routine_tables", 0x053896u, &object_routine_tables_data, 4 },
	{ "enemy_attacks", 0x053D2Au, &enemy_attacks_data, 4 },
	{ "enemy_ways_in", 0x053ED6u, &enemy_ways_in_data, 4 },
};

const size_t game_table_count = 372;
