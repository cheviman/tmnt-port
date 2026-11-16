/**
 *    Name:        objects/states.c
 *    Description: the objects' state dispatcher and the objects' handlers (provisional: by subject later)
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the object's current state run from the table of state handlers it is given */
void run_object_state(uint32_t object, uint32_t states)
{
	uint16_t state_offset = 0;
	uint32_t handler = 0, player_slot = 0, turtle = 0;

	state_offset = (uint16_t)((uint32_t)OBJECT(object)->state << 2);
	handler = ROUTINE_ENTRY(states + (state_offset / 4u) * 0x4u);
	switch (handler)
	{
	case enemy_hop_crouch_ADDRESS:
		enemy_hop_crouch(object);
		return;
	case enemy_hop_down_ADDRESS:
		enemy_hop_down(object);
		return;
	case enemy_hop_fall_ADDRESS:
		enemy_hop_fall(object);
		return;
	case enemy_hop_land_ADDRESS:
		enemy_hop_land(object);
		return;
	case enemy_hop_right_ADDRESS:
		enemy_hop_right(object);
		return;
	case enemy_hop_left_ADDRESS:
		enemy_hop_left(object);
		return;
	case enemy_hop_up_crouch_ADDRESS:
		enemy_hop_up_crouch(object);
		return;
	case enemy_hop_up_ADDRESS:
		enemy_hop_up(object);
		return;
	case enemy_hop_up_fall_ADDRESS:
		enemy_hop_up_fall(object);
		return;
	case enemy_hop_up_right_ADDRESS:
		enemy_hop_up_right(object);
		return;
	case enemy_hop_up_fall_blocked_ADDRESS:
		enemy_hop_up_fall_blocked(object);
		return;
	case enemy_hop_up_left_ADDRESS:
		enemy_hop_up_left(object);
		return;
	case enemy_shuffle_start_ADDRESS:
		enemy_shuffle_start(object);
		return;
	case enemy_shuffle_step_ADDRESS:
		enemy_shuffle_step(object);
		return;
	case enemy_shuffle_turn_ADDRESS:
		enemy_shuffle_turn(object);
		return;
	case enemy_shuffle_end_ADDRESS:
		enemy_shuffle_end(object);
		return;
	case enemy_retreat_start_ADDRESS:
		enemy_retreat_start(object);
		return;
	case enemy_retreat_step_ADDRESS:
		enemy_retreat_step(object);
		return;
	case enemy_circle_start_ADDRESS:
		enemy_circle_start(object);
		return;
	case enemy_circle_step_ADDRESS:
		enemy_circle_step(object);
		return;
	case enemy_charge_start_ADDRESS:
		enemy_charge_start(object);
		return;
	case enemy_charge_run_ADDRESS:
		enemy_charge_run(object);
		return;
	case enemy_charge_brake_ADDRESS:
		enemy_charge_brake(object);
		return;
	case enemy_crash_into_scenery_start_ADDRESS:
		enemy_crash_into_scenery_start(object);
		return;
	case enemy_crash_into_scenery_end_ADDRESS:
		enemy_crash_into_scenery_end(object);
		return;
	case enemy_crash_into_scenery_low_start_ADDRESS:
		enemy_crash_into_scenery_low_start(object);
		return;
	case enemy_walk_to_target_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_start(object, player_slot);
		return;
	case enemy_walk_to_target_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_step(object, player_slot);
		return;
	case enemy_walk_to_side_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_start(object, player_slot);
		return;
	case enemy_walk_to_side_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_step(object, player_slot);
		return;
	case enemy_walk_to_side_crouch_ADDRESS:
		enemy_walk_to_side_crouch(object);
		return;
	case enemy_walk_to_side_turn_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_turn(object, player_slot);
		return;
	case enemy_walk_to_side_hold_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_hold(object, player_slot);
		return;
	case enemy_keep_distance_start_ADDRESS:
		enemy_keep_distance_start(object);
		return;
	case enemy_keep_distance_watch_ADDRESS:
		player_slot = object_loop_slot;
		enemy_keep_distance_watch(object, player_slot);
		return;
	case enemy_keep_distance_back_off_ADDRESS:
		player_slot = object_loop_slot;
		enemy_keep_distance_back_off(object, player_slot);
		return;
	case enemy_keep_distance_close_in_ADDRESS:
		player_slot = object_loop_slot;
		enemy_keep_distance_close_in(object, player_slot);
		return;
	case enemy_back_off_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_start(object, player_slot);
		return;
	case enemy_back_off_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_walk(object, player_slot);
		return;
	case enemy_back_off_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_wait(object, player_slot);
		return;
	case enemy_run_away_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_run_away_start(object, player_slot);
		return;
	case enemy_run_away_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_run_away_step(object, player_slot);
		return;
	case enemy_walk_across_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_across_start(object, player_slot);
		return;
	case enemy_walk_across_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_across_step(object, player_slot);
		return;
	case enemy_jab_start_ADDRESS:
		enemy_jab_start(object);
		return;
	case enemy_attack_end_ADDRESS:
		enemy_attack_end(object);
		return;
	case enemy_chop_start_ADDRESS:
		enemy_chop_start(object);
		return;
	case enemy_swing_start_ADDRESS:
		enemy_swing_start(object);
		return;
	case enemy_high_kick_start_ADDRESS:
		enemy_high_kick_start(object);
		return;
	case enemy_jump_kick_crouch_ADDRESS:
		enemy_jump_kick_crouch(object);
		return;
	case enemy_jump_kick_leap_ADDRESS:
		enemy_jump_kick_leap(object);
		return;
	case enemy_jump_kick_fly_ADDRESS:
		enemy_jump_kick_fly(object);
		return;
	case enemy_long_jump_kick_crouch_ADDRESS:
		enemy_long_jump_kick_crouch(object);
		return;
	case enemy_long_jump_kick_leap_ADDRESS:
		enemy_long_jump_kick_leap(object);
		return;
	case enemy_long_jump_kick_fly_ADDRESS:
		enemy_long_jump_kick_fly(object);
		return;
	case enemy_grab_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_grab_start(object, player_slot);
		return;
	case enemy_grab_run_ADDRESS:
		enemy_grab_run(object);
		return;
	case enemy_grab_hold_ADDRESS:
		player_slot = object_loop_slot;
		enemy_grab_hold(object, player_slot);
		return;
	case enemy_grab_end_ADDRESS:
		enemy_grab_end(object);
		return;
	case enemy_flinch_back_start_ADDRESS:
		enemy_flinch_back_start(object);
		return;
	case enemy_flinch_end_ADDRESS:
		enemy_flinch_end(object);
		return;
	case enemy_flinch_bent_start_ADDRESS:
		enemy_flinch_bent_start(object);
		return;
	case enemy_flinch_reel_start_ADDRESS:
		enemy_flinch_reel_start(object);
		return;
	case enemy_flinch_hunched_start_ADDRESS:
		enemy_flinch_hunched_start(object);
		return;
	case enemy_killed_back_start_ADDRESS:
		enemy_killed_back_start(object);
		return;
	case enemy_killed_back_fly_ADDRESS:
		enemy_killed_back_fly(object);
		return;
	case enemy_killed_back_lie_ADDRESS:
		player_slot = object_loop_slot;
		enemy_killed_back_lie(object, player_slot);
		return;
	case enemy_killed_back_on_wall_ADDRESS:
		enemy_killed_back_on_wall(object);
		return;
	case enemy_killed_back_slide_down_ADDRESS:
		enemy_killed_back_slide_down(object);
		return;
	case enemy_killed_back_down_ADDRESS:
		player_slot = object_loop_slot;
		enemy_killed_back_down(object, player_slot);
		return;
	case enemy_killed_forward_start_ADDRESS:
		enemy_killed_forward_start(object);
		return;
	case enemy_killed_forward_fly_ADDRESS:
		enemy_killed_forward_fly(object);
		return;
	case enemy_killed_forward_lie_ADDRESS:
		player_slot = object_loop_slot;
		enemy_killed_forward_lie(object, player_slot);
		return;
	case enemy_killed_forward_on_wall_ADDRESS:
		enemy_killed_forward_on_wall(object);
		return;
	case enemy_killed_forward_slide_down_ADDRESS:
		enemy_killed_forward_slide_down(object);
		return;
	case enemy_killed_forward_down_ADDRESS:
		player_slot = object_loop_slot;
		enemy_killed_forward_down(object, player_slot);
		return;
	case enemy_knocked_forward_start_ADDRESS:
		enemy_knocked_forward_start(object);
		return;
	case enemy_knocked_forward_fly_ADDRESS:
		enemy_knocked_forward_fly(object);
		return;
	case enemy_knocked_forward_lie_ADDRESS:
		player_slot = object_loop_slot;
		enemy_knocked_forward_lie(object, player_slot);
		return;
	case enemy_knocked_forward_wait_ADDRESS:
		enemy_knocked_forward_wait(object);
		return;
	case enemy_thrown_by_donatello_start_ADDRESS:
		enemy_thrown_by_donatello_start(object);
		return;
	case enemy_thrown_by_donatello_swing_ADDRESS:
		enemy_thrown_by_donatello_swing(object);
		return;
	case enemy_thrown_fly_ADDRESS:
		enemy_thrown_fly(object);
		return;
	case enemy_thrown_by_michelangelo_start_ADDRESS:
		enemy_thrown_by_michelangelo_start(object);
		return;
	case enemy_thrown_by_michelangelo_swing_ADDRESS:
		enemy_thrown_by_michelangelo_swing(object);
		return;
	case enemy_thrown_by_raphael_start_ADDRESS:
		enemy_thrown_by_raphael_start(object);
		return;
	case enemy_thrown_by_raphael_swing_ADDRESS:
		enemy_thrown_by_raphael_swing(object);
		return;
	case enemy_thrown_by_leonardo_start_ADDRESS:
		enemy_thrown_by_leonardo_start(object);
		return;
	case enemy_thrown_by_leonardo_swing_ADDRESS:
		enemy_thrown_by_leonardo_swing(object);
		return;
	case enemy_knocked_back_start_ADDRESS:
		enemy_knocked_back_start(object);
		return;
	case enemy_knocked_back_fly_ADDRESS:
		enemy_knocked_back_fly(object);
		return;
	case enemy_knocked_back_lie_ADDRESS:
		player_slot = object_loop_slot;
		enemy_knocked_back_lie(object, player_slot);
		return;
	case enemy_knocked_back_wait_ADDRESS:
		enemy_knocked_back_wait(object);
		return;
	case enemy_knocked_back_get_up_ADDRESS:
		enemy_knocked_back_get_up(object);
		return;
	case enemy_die_start_ADDRESS:
		enemy_die_start(object);
		return;
	case enemy_die_vanish_ADDRESS:
		enemy_die_vanish(object);
		return;
	case enemy_die_on_back_start_ADDRESS:
		enemy_die_on_back_start(object);
		return;
	case enemy_walk_to_target_sword_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_sword_start(object, player_slot);
		return;
	case enemy_walk_to_side_sword_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_sword_start(object, player_slot);
		return;
	case enemy_walk_to_side_sword_crouch_ADDRESS:
		enemy_walk_to_side_sword_crouch(object);
		return;
	case enemy_walk_to_side_sword_turn_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_sword_turn(object, player_slot);
		return;
	case enemy_walk_past_sword_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_past_sword_start(object, player_slot);
		return;
	case enemy_walk_past_sword_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_past_sword_step(object, player_slot);
		return;
	case enemy_back_off_sword_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_sword_start(object, player_slot);
		return;
	case enemy_back_off_sword_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_sword_walk(object, player_slot);
		return;
	case enemy_back_off_sword_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_sword_wait(object, player_slot);
		return;
	case enemy_walk_to_line_sword_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_line_sword_start(object, player_slot);
		return;
	case enemy_walk_to_line_sword_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_line_sword_step(object, player_slot);
		return;
	case enemy_walk_to_target_hammer_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_hammer_start(object, player_slot);
		return;
	case enemy_walk_to_side_hammer_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_hammer_start(object, player_slot);
		return;
	case enemy_walk_to_side_far_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_far_step(object, player_slot);
		return;
	case enemy_walk_to_side_far_hold_ADDRESS:
		enemy_walk_to_side_far_hold(object);
		return;
	case enemy_stalk_hammer_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_stalk_hammer_start(object, player_slot);
		return;
	case enemy_stalk_hammer_approach_ADDRESS:
		player_slot = object_loop_slot;
		enemy_stalk_hammer_approach(object, player_slot);
		return;
	case enemy_stalk_hammer_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_stalk_hammer_step(object, player_slot);
		return;
	case enemy_walk_away_hammer_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_away_hammer_start(object, player_slot);
		return;
	case enemy_walk_away_hammer_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_away_hammer_walk(object, player_slot);
		return;
	case enemy_walk_away_hammer_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_away_hammer_wait(object, player_slot);
		return;
	case enemy_slash_sword_start_ADDRESS:
		enemy_slash_sword_start(object);
		return;
	case enemy_low_slash_sword_start_ADDRESS:
		enemy_low_slash_sword_start(object);
		return;
	case enemy_leaping_slash_sword_start_ADDRESS:
		enemy_leaping_slash_sword_start(object);
		return;
	case enemy_leaping_slash_sword_leap_ADDRESS:
		enemy_leaping_slash_sword_leap(object);
		return;
	case enemy_leaping_slash_sword_fly_ADDRESS:
		enemy_leaping_slash_sword_fly(object);
		return;
	case enemy_jump_slash_sword_start_ADDRESS:
		enemy_jump_slash_sword_start(object);
		return;
	case enemy_jump_slash_sword_leap_ADDRESS:
		enemy_jump_slash_sword_leap(object);
		return;
	case enemy_jump_slash_sword_fly_ADDRESS:
		enemy_jump_slash_sword_fly(object);
		return;
	case enemy_flying_slash_sword_start_ADDRESS:
		enemy_flying_slash_sword_start(object);
		return;
	case enemy_flying_slash_sword_leap_ADDRESS:
		enemy_flying_slash_sword_leap(object);
		return;
	case enemy_flying_slash_sword_fly_ADDRESS:
		enemy_flying_slash_sword_fly(object);
		return;
	case enemy_smash_hammer_start_ADDRESS:
		enemy_smash_hammer_start(object);
		return;
	case enemy_smash_hammer_end_ADDRESS:
		enemy_smash_hammer_end(object);
		return;
	case enemy_flinch_back_sword_start_ADDRESS:
		enemy_flinch_back_sword_start(object);
		return;
	case enemy_flinch_bent_sword_start_ADDRESS:
		enemy_flinch_bent_sword_start(object);
		return;
	case enemy_flinch_reel_sword_start_ADDRESS:
		enemy_flinch_reel_sword_start(object);
		return;
	case enemy_flinch_hunched_sword_start_ADDRESS:
		enemy_flinch_hunched_sword_start(object);
		return;
	case enemy_flinch_back_hammer_start_ADDRESS:
		enemy_flinch_back_hammer_start(object);
		return;
	case enemy_flinch_bent_hammer_start_ADDRESS:
		enemy_flinch_bent_hammer_start(object);
		return;
	case enemy_step_out_start_ADDRESS:
		enemy_step_out_start(object);
		return;
	case enemy_step_out_wait_ADDRESS:
		enemy_step_out_wait(object);
		return;
	case enemy_step_out_walk_ADDRESS:
		enemy_step_out_walk(object);
		return;
	case enemy_walk_in_on_ledge_start_ADDRESS:
		enemy_walk_in_on_ledge_start(object);
		return;
	case enemy_walk_in_on_ledge_walk_ADDRESS:
		enemy_walk_in_on_ledge_walk(object, handler);
		return;
	case enemy_jump_down_leap_ADDRESS:
		enemy_jump_down_leap(object);
		return;
	case enemy_jump_down_fall_stairs_ADDRESS:
		enemy_jump_down_fall_stairs(object);
		return;
	case enemy_jump_down_fall_ADDRESS:
		enemy_jump_down_fall(object);
		return;
	case enemy_jump_down_land_ADDRESS:
		enemy_jump_down_land(object);
		return;
	case enemy_step_down_start_ADDRESS:
		enemy_step_down_start(object);
		return;
	case enemy_step_down_wait_ADDRESS:
		enemy_step_down_wait(object);
		return;
	case enemy_leap_out_start_ADDRESS:
		enemy_leap_out_start(object);
		return;
	case enemy_leap_out_fly_ADDRESS:
		enemy_leap_out_fly(object);
		return;
	case enemy_leap_out_land_ADDRESS:
		enemy_leap_out_land(object);
		return;
	case enemy_hang_in_start_ADDRESS:
		enemy_hang_in_start(object);
		return;
	case enemy_hang_in_move_ADDRESS:
		enemy_hang_in_move(object);
		return;
	case enemy_hang_in_drop_ADDRESS:
		enemy_hang_in_drop(object);
		return;
	case enemy_hang_in_land_ADDRESS:
		enemy_hang_in_land(object);
		return;
	case enemy_kick_in_start_ADDRESS:
		enemy_kick_in_start(object);
		return;
	case enemy_kick_in_run_ADDRESS:
		enemy_kick_in_run(object);
		return;
	case enemy_kick_in_leap_ADDRESS:
		enemy_kick_in_leap(object);
		return;
	case enemy_kick_in_fly_ADDRESS:
		enemy_kick_in_fly(object);
		return;
	case enemy_kick_in_land_ADDRESS:
		enemy_kick_in_land(object);
		return;
	case enemy_run_in_start_ADDRESS:
		enemy_run_in_start(object);
		return;
	case enemy_run_in_run_ADDRESS:
		enemy_run_in_run(object);
		return;
	case enemy_dive_in_start_ADDRESS:
		enemy_dive_in_start(object);
		return;
	case enemy_dive_in_run_ADDRESS:
		enemy_dive_in_run(object, handler);
		return;
	case enemy_dive_in_leap_ADDRESS:
		enemy_dive_in_leap(object);
		return;
	case enemy_dive_in_dive_ADDRESS:
		enemy_dive_in_dive(object);
		return;
	case enemy_dive_in_curl_ADDRESS:
		enemy_dive_in_curl(object);
		return;
	case enemy_dive_in_uncurl_ADDRESS:
		enemy_dive_in_uncurl(object);
		return;
	case enemy_dive_in_land_ADDRESS:
		enemy_dive_in_land(object);
		return;
	case enemy_walk_in_start_ADDRESS:
		enemy_walk_in_start(object);
		return;
	case enemy_walk_in_walk_ADDRESS:
		enemy_walk_in_walk(object);
		return;
	case enemy_walk_in_ready_ADDRESS:
		enemy_walk_in_ready(object);
		return;
	case enemy_walk_to_target_spear_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_spear_start(object, player_slot);
		return;
	case enemy_walk_to_side_spear_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_spear_start(object, player_slot);
		return;
	case enemy_walk_off_line_spear_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_off_line_spear_start(object, player_slot);
		return;
	case enemy_walk_off_line_spear_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_off_line_spear_step(object, player_slot);
		return;
	case enemy_back_off_spear_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_spear_start(object, player_slot);
		return;
	case enemy_back_off_spear_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_spear_walk(object, player_slot);
		return;
	case enemy_step_aside_spear_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_step_aside_spear_start(object, player_slot);
		return;
	case enemy_step_aside_spear_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_step_aside_spear_step(object, player_slot);
		return;
	case enemy_walk_to_target_knife_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_knife_start(object, player_slot);
		return;
	case enemy_walk_to_side_knife_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_knife_start(object, player_slot);
		return;
	case enemy_circle_knife_start_ADDRESS:
		enemy_circle_knife_start(object);
		return;
	case enemy_circle_knife_step_ADDRESS:
		enemy_circle_knife_step(object);
		return;
	case enemy_keep_distance_knife_start_ADDRESS:
		enemy_keep_distance_knife_start(object);
		return;
	case enemy_keep_distance_knife_watch_ADDRESS:
		player_slot = object_loop_slot;
		enemy_keep_distance_knife_watch(object, player_slot);
		return;
	case enemy_keep_distance_knife_back_off_ADDRESS:
		player_slot = object_loop_slot;
		enemy_keep_distance_knife_back_off(object, player_slot);
		return;
	case enemy_keep_distance_knife_close_in_ADDRESS:
		player_slot = object_loop_slot;
		enemy_keep_distance_knife_close_in(object, player_slot);
		return;
	case enemy_back_off_knife_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_knife_start(object, player_slot);
		return;
	case enemy_back_off_knife_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_knife_walk(object, player_slot);
		return;
	case enemy_back_off_knife_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_knife_wait(object, player_slot);
		return;
	case enemy_step_aside_knife_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_step_aside_knife_start(object, player_slot);
		return;
	case enemy_step_aside_knife_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_step_aside_knife_step(object, player_slot);
		return;
	case enemy_spear_thrust_start_ADDRESS:
		enemy_spear_thrust_start(object);
		return;
	case enemy_spear_thrust_end_ADDRESS:
		enemy_spear_thrust_end(object);
		return;
	case enemy_spear_throw_start_ADDRESS:
		enemy_spear_throw_start(object);
		return;
	case enemy_spear_throw_release_ADDRESS:
		enemy_spear_throw_release(object);
		return;
	case enemy_spear_throw_end_ADDRESS:
		enemy_spear_throw_end(object, handler);
		return;
	case enemy_knife_stab_start_ADDRESS:
		enemy_knife_stab_start(object);
		return;
	case enemy_high_kick_knife_start_ADDRESS:
		enemy_high_kick_knife_start(object);
		return;
	case enemy_jump_kick_knife_crouch_ADDRESS:
		enemy_jump_kick_knife_crouch(object);
		return;
	case enemy_jump_kick_knife_leap_ADDRESS:
		enemy_jump_kick_knife_leap(object);
		return;
	case enemy_jump_kick_knife_fly_ADDRESS:
		enemy_jump_kick_knife_fly(object);
		return;
	case enemy_throw_knife_start_ADDRESS:
		enemy_throw_knife_start(object);
		return;
	case enemy_throw_knife_release_ADDRESS:
		enemy_throw_knife_release(object);
		return;
	case enemy_throw_knife_end_ADDRESS:
		enemy_throw_knife_end(object);
		return;
	case enemy_flinch_back_spear_start_ADDRESS:
		enemy_flinch_back_spear_start(object);
		return;
	case enemy_flinch_bent_spear_start_ADDRESS:
		enemy_flinch_bent_spear_start(object);
		return;
	case enemy_flinch_reel_spear_start_ADDRESS:
		enemy_flinch_reel_spear_start(object);
		return;
	case enemy_flinch_hunched_spear_start_ADDRESS:
		enemy_flinch_hunched_spear_start(object);
		return;
	case enemy_flinch_back_knife_start_ADDRESS:
		enemy_flinch_back_knife_start(object);
		return;
	case enemy_flinch_bent_knife_start_ADDRESS:
		enemy_flinch_bent_knife_start(object);
		return;
	case enemy_flinch_reel_knife_start_ADDRESS:
		enemy_flinch_reel_knife_start(object);
		return;
	case enemy_flinch_hunched_knife_start_ADDRESS:
		enemy_flinch_hunched_knife_start(object);
		return;
	case enemy_walk_to_target_gun_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_gun_start(object, player_slot);
		return;
	case enemy_walk_to_side_gun_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_gun_start(object, player_slot);
		return;
	case enemy_walk_to_range_gun_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_range_gun_start(object, player_slot);
		return;
	case enemy_walk_to_range_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_range_step(object, player_slot);
		return;
	case enemy_walk_to_range_hold_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_range_hold(object, player_slot);
		return;
	case enemy_back_off_gun_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_gun_start(object, player_slot);
		return;
	case enemy_back_off_gun_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_gun_walk(object, player_slot);
		return;
	case enemy_back_off_gun_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_gun_wait(object, player_slot);
		return;
	case enemy_run_away_gun_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_run_away_gun_start(object, player_slot);
		return;
	case enemy_run_away_gun_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_run_away_gun_step(object, player_slot);
		return;
	case enemy_walk_to_target_axe_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_target_axe_start(object, player_slot);
		return;
	case enemy_walk_to_side_axe_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_side_axe_start(object, player_slot);
		return;
	case enemy_walk_to_range_axe_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_walk_to_range_axe_start(object, player_slot);
		return;
	case enemy_back_off_axe_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_axe_start(object, player_slot);
		return;
	case enemy_back_off_axe_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_axe_walk(object, player_slot);
		return;
	case enemy_back_off_axe_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_back_off_axe_wait(object, player_slot);
		return;
	case enemy_run_away_axe_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_run_away_axe_start(object, player_slot);
		return;
	case enemy_run_away_axe_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_run_away_axe_step(object, player_slot);
		return;
	case enemy_throw_axes_start_ADDRESS:
		enemy_throw_axes_start(object);
		return;
	case enemy_throw_axes_release_ADDRESS:
		enemy_throw_axes_release(object);
		return;
	case enemy_high_kick_axe_start_ADDRESS:
		enemy_high_kick_axe_start(object);
		return;
	case enemy_gun_swing_start_ADDRESS:
		enemy_gun_swing_start(object);
		return;
	case enemy_shoot_gun_start_ADDRESS:
		enemy_shoot_gun_start(object);
		return;
	case enemy_shoot_gun_fire_ADDRESS:
		enemy_shoot_gun_fire(object);
		return;
	case enemy_flinch_back_gun_start_ADDRESS:
		enemy_flinch_back_gun_start(object);
		return;
	case enemy_flinch_bent_gun_start_ADDRESS:
		enemy_flinch_bent_gun_start(object);
		return;
	case enemy_flinch_back_axe_start_ADDRESS:
		enemy_flinch_back_axe_start(object);
		return;
	case enemy_flinch_bent_axe_start_ADDRESS:
		enemy_flinch_bent_axe_start(object);
		return;
	case enemy_flinch_reel_axe_start_ADDRESS:
		enemy_flinch_reel_axe_start(object);
		return;
	case enemy_throw_shuriken_start_ADDRESS:
		enemy_throw_shuriken_start(object);
		return;
	case enemy_throw_shuriken_release_ADDRESS:
		enemy_throw_shuriken_release(object);
		return;
	case enemy_boomerang_throw_wind_up_ADDRESS:
		enemy_boomerang_throw_wind_up(object);
		return;
	case enemy_boomerang_throw_aim_ADDRESS:
		enemy_boomerang_throw_aim(object);
		return;
	case enemy_boomerang_throw_release_ADDRESS:
		enemy_boomerang_throw_release(object);
		return;
	case enemy_boomerang_throw_wait_ADDRESS:
		enemy_boomerang_throw_wait(object);
		return;
	case enemy_rocksteady_enter_wait_ADDRESS:
		enemy_rocksteady_enter_wait(object);
		return;
	case enemy_rocksteady_enter_taunt_ADDRESS:
		enemy_rocksteady_enter_taunt(state_offset, object);
		return;
	case enemy_rocksteady_enter_ready_ADDRESS:
		enemy_rocksteady_enter_ready(object);
		return;
	case enemy_rocksteady_enter_walk_in_ADDRESS:
		enemy_rocksteady_enter_walk_in(object);
		return;
	case enemy_rocksteady_enter_wait_for_signal_ADDRESS:
		enemy_rocksteady_enter_wait_for_signal(object);
		return;
	case enemy_rocksteady_enter_walk_in_with_bebop_ADDRESS:
		enemy_rocksteady_enter_walk_in_with_bebop(object);
		return;
	case enemy_rocksteady_raise_fist_start_ADDRESS:
		enemy_rocksteady_raise_fist_start(object);
		return;
	case enemy_rocksteady_laugh_start_ADDRESS:
		enemy_rocksteady_laugh_start(object);
		return;
	case enemy_rocksteady_shake_fist_start_ADDRESS:
		enemy_rocksteady_shake_fist_start(object);
		return;
	case enemy_rocksteady_hold_pose_ADDRESS:
		enemy_rocksteady_hold_pose(object);
		return;
	case enemy_rocksteady_draw_gun_start_ADDRESS:
		enemy_rocksteady_draw_gun_start(object);
		return;
	case enemy_rocksteady_pose_end_ADDRESS:
		enemy_rocksteady_pose_end(object);
		return;
	case enemy_rocksteady_put_gun_away_start_ADDRESS:
		enemy_rocksteady_put_gun_away_start(object);
		return;
	case enemy_rocksteady_circle_start_ADDRESS:
		enemy_rocksteady_circle_start(object);
		return;
	case enemy_rocksteady_circle_step_ADDRESS:
		enemy_rocksteady_circle_step(object);
		return;
	case enemy_rocksteady_line_up_start_ADDRESS:
		enemy_rocksteady_line_up_start(object);
		return;
	case enemy_rocksteady_line_up_step_ADDRESS:
		enemy_rocksteady_line_up_step(object);
		return;
	case enemy_rocksteady_close_in_start_ADDRESS:
		enemy_rocksteady_close_in_start(object);
		return;
	case enemy_rocksteady_close_in_step_ADDRESS:
		turtle = object_loop_slot;
		enemy_rocksteady_close_in_step(object, turtle);
		return;
	case enemy_rocksteady_shoot_around_start_ADDRESS:
		enemy_rocksteady_shoot_around_start(object);
		return;
	case enemy_rocksteady_shoot_around_fire_ADDRESS:
		enemy_rocksteady_shoot_around_fire(object);
		return;
	case enemy_rocksteady_pincer_charge_start_ADDRESS:
		enemy_rocksteady_pincer_charge_start(object);
		return;
	case enemy_rocksteady_pincer_charge_run_to_edge_ADDRESS:
		enemy_rocksteady_pincer_charge_run_to_edge(object);
		return;
	case enemy_rocksteady_pincer_charge_wait_for_bebop_ADDRESS:
		enemy_rocksteady_pincer_charge_wait_for_bebop(object);
		return;
	case enemy_rocksteady_pincer_charge_get_ready_ADDRESS:
		enemy_rocksteady_pincer_charge_get_ready(object);
		return;
	case enemy_rocksteady_pincer_charge_run_ADDRESS:
		enemy_rocksteady_pincer_charge_run(object);
		return;
	case enemy_rocksteady_pincer_charge_crash_ADDRESS:
		enemy_rocksteady_pincer_charge_crash(object);
		return;
	case enemy_rocksteady_charge_start_ADDRESS:
		enemy_rocksteady_charge_start(object);
		return;
	case enemy_rocksteady_charge_get_ready_ADDRESS:
		enemy_rocksteady_charge_get_ready(object);
		return;
	case enemy_rocksteady_charge_run_ADDRESS:
		enemy_rocksteady_charge_run(object);
		return;
	case enemy_rocksteady_punch_start_ADDRESS:
		enemy_rocksteady_punch_start(object);
		return;
	case enemy_rocksteady_blow_end_ADDRESS:
		enemy_rocksteady_blow_end(object);
		return;
	case enemy_rocksteady_gun_butt_start_ADDRESS:
		enemy_rocksteady_gun_butt_start(object);
		return;
	case enemy_rocksteady_shoot_start_ADDRESS:
		turtle = object_loop_slot;
		enemy_rocksteady_shoot_start(object, turtle);
		return;
	case enemy_rocksteady_shoot_fire_ADDRESS:
		enemy_rocksteady_shoot_fire(object);
		return;
	case enemy_rocksteady_flinch_start_ADDRESS:
		enemy_rocksteady_flinch_start(object);
		return;
	case enemy_rocksteady_flinch_end_ADDRESS:
		enemy_rocksteady_flinch_end(object);
		return;
	case enemy_rocksteady_reel_start_ADDRESS:
		enemy_rocksteady_reel_start(object);
		return;
	case enemy_rocksteady_flinch_from_behind_start_ADDRESS:
		enemy_rocksteady_flinch_from_behind_start(object);
		return;
	case enemy_rocksteady_knocked_down_start_ADDRESS:
		enemy_rocksteady_knocked_down_start(object);
		return;
	case enemy_rocksteady_knocked_down_fall_ADDRESS:
		enemy_rocksteady_knocked_down_fall(object);
		return;
	case enemy_rocksteady_knocked_down_slide_ADDRESS:
		enemy_rocksteady_knocked_down_slide(object);
		return;
	case enemy_rocksteady_knocked_down_lie_ADDRESS:
		enemy_rocksteady_knocked_down_lie(object);
		return;
	case enemy_rocksteady_knocked_down_get_up_ADDRESS:
		enemy_rocksteady_knocked_down_get_up(object);
		return;
	case enemy_rocksteady_defeated_start_ADDRESS:
		enemy_rocksteady_defeated_start(object);
		return;
	case enemy_rocksteady_defeated_fall_ADDRESS:
		enemy_rocksteady_defeated_fall(object);
		return;
	case enemy_rocksteady_defeated_slide_ADDRESS:
		enemy_rocksteady_defeated_slide(object);
		return;
	case enemy_rocksteady_defeated_blink_ADDRESS:
		enemy_rocksteady_defeated_blink(object);
		return;
	case enemy_bebop_enter_wait_for_scroll_ADDRESS:
		enemy_bebop_enter_wait_for_scroll(object);
		return;
	case enemy_bebop_enter_wait_ADDRESS:
		enemy_bebop_enter_wait(object);
		return;
	case enemy_bebop_enter_drop_ADDRESS:
		enemy_bebop_enter_drop(object);
		return;
	case enemy_bebop_enter_jump_ADDRESS:
		enemy_bebop_enter_jump(object);
		return;
	case enemy_bebop_enter_in_air_ADDRESS:
		enemy_bebop_enter_in_air(object);
		return;
	case enemy_bebop_enter_ready_ADDRESS:
		enemy_bebop_enter_ready(object);
		return;
	case enemy_bebop_enter_wait_for_signal_ADDRESS:
		enemy_bebop_enter_wait_for_signal(object);
		return;
	case enemy_bebop_enter_walk_in_ADDRESS:
		enemy_bebop_enter_walk_in(object);
		return;
	case enemy_bebop_stand_start_ADDRESS:
		enemy_bebop_stand_start(object);
		return;
	case enemy_bebop_stand_wait_ADDRESS:
		enemy_bebop_stand_wait(object);
		return;
	case enemy_bebop_walk_to_front_start_ADDRESS:
		enemy_bebop_walk_to_front_start(object);
		return;
	case enemy_bebop_walk_to_front_step_ADDRESS:
		turtle = object_loop_slot;
		enemy_bebop_walk_to_front_step(object, turtle);
		return;
	case enemy_bebop_walk_level_step_ADDRESS:
		enemy_bebop_walk_level_step(object);
		return;
	case enemy_bebop_walk_start_ADDRESS:
		enemy_bebop_walk_start(object);
		return;
	case enemy_bebop_close_in_step_ADDRESS:
		turtle = object_loop_slot;
		enemy_bebop_close_in_step(object, turtle);
		return;
	case enemy_bebop_pincer_charge_start_ADDRESS:
		enemy_bebop_pincer_charge_start(object);
		return;
	case enemy_bebop_pincer_charge_run_to_edge_ADDRESS:
		enemy_bebop_pincer_charge_run_to_edge(object);
		return;
	case enemy_bebop_pincer_charge_wait_for_rocksteady_ADDRESS:
		enemy_bebop_pincer_charge_wait_for_rocksteady(object);
		return;
	case enemy_bebop_pincer_charge_get_ready_ADDRESS:
		enemy_bebop_pincer_charge_get_ready(object);
		return;
	case enemy_bebop_pincer_charge_run_ADDRESS:
		enemy_bebop_pincer_charge_run(object);
		return;
	case enemy_bebop_pincer_charge_crash_ADDRESS:
		enemy_bebop_pincer_charge_crash(object);
		return;
	case enemy_bebop_punch_start_ADDRESS:
		enemy_bebop_punch_start(object);
		return;
	case enemy_bebop_punch_end_ADDRESS:
		enemy_bebop_punch_end(object);
		return;
	case enemy_bebop_charge_start_ADDRESS:
		enemy_bebop_charge_start(object);
		return;
	case enemy_bebop_charge_get_ready_ADDRESS:
		enemy_bebop_charge_get_ready(object);
		return;
	case enemy_bebop_charge_run_ADDRESS:
		enemy_bebop_charge_run(object);
		return;
	case enemy_bebop_shoot_start_ADDRESS:
		enemy_bebop_shoot_start(object);
		return;
	case enemy_bebop_shoot_fire_ADDRESS:
		enemy_bebop_shoot_fire(object);
		return;
	case enemy_bebop_flinch_start_ADDRESS:
		enemy_bebop_flinch_start(object);
		return;
	case enemy_bebop_strike_back_ADDRESS:
		enemy_bebop_strike_back(object);
		return;
	case enemy_bebop_flinch_from_behind_start_ADDRESS:
		enemy_bebop_flinch_from_behind_start(object);
		return;
	case enemy_bebop_flinch_from_behind_end_ADDRESS:
		enemy_bebop_flinch_from_behind_end(object);
		return;
	case enemy_bebop_knocked_down_start_ADDRESS:
		enemy_bebop_knocked_down_start(object);
		return;
	case enemy_bebop_knocked_down_fall_ADDRESS:
		enemy_bebop_knocked_down_fall(object);
		return;
	case enemy_bebop_knocked_down_slide_ADDRESS:
		enemy_bebop_knocked_down_slide(object);
		return;
	case enemy_bebop_knocked_down_lie_ADDRESS:
		enemy_bebop_knocked_down_lie(object);
		return;
	case enemy_bebop_defeated_start_ADDRESS:
		enemy_bebop_defeated_start(object);
		return;
	case enemy_bebop_defeated_fall_ADDRESS:
		enemy_bebop_defeated_fall(object);
		return;
	case enemy_bebop_defeated_slide_ADDRESS:
		enemy_bebop_defeated_slide(object);
		return;
	case enemy_bebop_defeated_blink_ADDRESS:
		enemy_bebop_defeated_blink(object);
		return;
	case enemy_mouser_enter_start_ADDRESS:
		enemy_mouser_enter_start(object);
		return;
	case enemy_mouser_enter_hop_ADDRESS:
		enemy_mouser_enter_hop(object);
		return;
	case enemy_mouser_enter_fall_ADDRESS:
		enemy_mouser_enter_fall(object);
		return;
	case enemy_mouser_enter_land_ADDRESS:
		enemy_mouser_enter_land(object);
		return;
	case enemy_mouser_bite_start_ADDRESS:
		enemy_mouser_bite_start(object);
		return;
	case enemy_mouser_bite_fly_ADDRESS:
		enemy_mouser_bite_fly(object);
		return;
	case enemy_mouser_bite_land_ADDRESS:
		enemy_mouser_bite_land(object);
		return;
	case enemy_mouser_bite_hold_ADDRESS:
		enemy_mouser_bite_hold(object);
		return;
	case enemy_mouser_hop_down_ADDRESS:
		enemy_mouser_hop_down(object);
		return;
	case enemy_mouser_hop_fall_ADDRESS:
		enemy_mouser_hop_fall(object);
		return;
	case enemy_mouser_hop_land_ADDRESS:
		enemy_mouser_hop_land(object);
		return;
	case enemy_mouser_hop_right_ADDRESS:
		enemy_mouser_hop_right(object);
		return;
	case enemy_mouser_hop_left_ADDRESS:
		enemy_mouser_hop_left(object);
		return;
	case enemy_mouser_hop_up_ADDRESS:
		enemy_mouser_hop_up(object);
		return;
	case enemy_mouser_hop_up_fall_ADDRESS:
		enemy_mouser_hop_up_fall(object);
		return;
	case enemy_mouser_hop_up_right_ADDRESS:
		enemy_mouser_hop_up_right(object);
		return;
	case enemy_mouser_hop_up_fall_blocked_ADDRESS:
		enemy_mouser_hop_up_fall_blocked(object);
		return;
	case enemy_mouser_hop_up_left_ADDRESS:
		enemy_mouser_hop_up_left(object);
		return;
	case enemy_mouser_chase_start_ADDRESS:
		enemy_mouser_chase_start((uint8_t)state_offset, object);
		return;
	case enemy_mouser_chase_run_ADDRESS:
		player_slot = object_loop_slot;
		enemy_mouser_chase_run(object, player_slot);
		return;
	case enemy_mouser_chase_pause_ADDRESS:
		enemy_mouser_chase_pause(object);
		return;
	case enemy_mouser_chase_wait_ADDRESS:
		enemy_mouser_chase_wait(object);
		return;
	case enemy_mouser_creep_to_side_start_ADDRESS:
		enemy_mouser_creep_to_side_start((uint8_t)state_offset, object);
		return;
	case enemy_mouser_creep_to_side_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_mouser_creep_to_side_step(object, player_slot);
		return;
	case enemy_mouser_back_off_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_mouser_back_off_start(object, player_slot);
		return;
	case enemy_mouser_back_off_run_ADDRESS:
		enemy_mouser_back_off_run((uint8_t)state_offset, object);
		return;
	case enemy_mouser_sit_ADDRESS:
		enemy_mouser_sit(object);
		return;
	case enemy_mouser_back_off_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_mouser_back_off_wait(object, player_slot);
		return;
	case enemy_mouser_thrown_off_start_ADDRESS:
		enemy_mouser_thrown_off_start(object);
		return;
	case enemy_mouser_break_down_start_ADDRESS:
		enemy_mouser_break_down_start(object);
		return;
	case enemy_mouser_knocked_flying_start_ADDRESS:
		enemy_mouser_knocked_flying_start(object);
		return;
	case enemy_mouser_fly_and_crash_ADDRESS:
		enemy_mouser_fly_and_crash(object);
		return;
	case enemy_mouser_remove_ADDRESS:
		player_slot = object_loop_slot;
		enemy_mouser_remove(object, player_slot);
		return;
	case enemy_rodney_enter_wait_ADDRESS:
		enemy_rodney_enter_wait(object);
		return;
	case enemy_rodney_enter_burst_out_ADDRESS:
		enemy_rodney_enter_burst_out(object);
		return;
	case enemy_rodney_enter_fall_ADDRESS:
		enemy_rodney_enter_fall(object);
		return;
	case enemy_rodney_enter_land_ADDRESS:
		enemy_rodney_enter_land(object);
		return;
	case enemy_rodney_rush_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_rodney_rush_start(object, player_slot);
		return;
	case enemy_rodney_rush_run_ADDRESS:
		enemy_rodney_rush_run(object);
		return;
	case enemy_rodney_brake_ADDRESS:
		enemy_rodney_brake(object);
		return;
	case enemy_rodney_rush_across_start_ADDRESS:
		enemy_rodney_rush_across_start(object);
		return;
	case enemy_rodney_rush_across_run_ADDRESS:
		enemy_rodney_rush_across_run(object);
		return;
	case enemy_rodney_back_off_start_ADDRESS:
		enemy_rodney_back_off_start(object);
		return;
	case enemy_rodney_slide_to_stop_ADDRESS:
		enemy_rodney_slide_to_stop(object);
		return;
	case enemy_rodney_back_off_turn_ADDRESS:
		enemy_rodney_back_off_turn(object);
		return;
	case enemy_rodney_back_off_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_rodney_back_off_wait(object, player_slot);
		return;
	case enemy_rodney_shoot_start_ADDRESS:
		enemy_rodney_shoot_start(object);
		return;
	case enemy_rodney_shoot_turn_ADDRESS:
		enemy_rodney_shoot_turn(object);
		return;
	case enemy_rodney_shoot_ready_ADDRESS:
		enemy_rodney_shoot_ready(object);
		return;
	case enemy_rodney_shoot_fire_ADDRESS:
		enemy_rodney_shoot_fire(object);
		return;
	case enemy_rodney_shoot_end_ADDRESS:
		enemy_rodney_shoot_end(object);
		return;
	case enemy_rodney_whip_start_ADDRESS:
		enemy_rodney_whip_start(object);
		return;
	case enemy_rodney_whip_slow_down_ADDRESS:
		enemy_rodney_whip_slow_down(object);
		return;
	case enemy_rodney_whip_wind_up_ADDRESS:
		enemy_rodney_whip_wind_up(object);
		return;
	case enemy_rodney_whip_throw_arm_ADDRESS:
		enemy_rodney_whip_throw_arm(object);
		return;
	case enemy_rodney_whip_reach_ADDRESS:
		enemy_rodney_whip_reach(object);
		return;
	case enemy_rodney_whip_lash_ADDRESS:
		enemy_rodney_whip_lash(object);
		return;
	case enemy_rodney_whip_retract_ADDRESS:
		enemy_rodney_whip_retract(object);
		return;
	case enemy_rodney_knocked_back_start_ADDRESS:
		enemy_rodney_knocked_back_start(object);
		return;
	case enemy_rodney_knocked_back_fall_ADDRESS:
		enemy_rodney_knocked_back_fall(object);
		return;
	case enemy_rodney_knocked_back_end_ADDRESS:
		enemy_rodney_knocked_back_end(object);
		return;
	case enemy_rodney_destroyed_start_ADDRESS:
		enemy_rodney_destroyed_start(object);
		return;
	case enemy_rodney_destroyed_fall_ADDRESS:
		enemy_rodney_destroyed_fall(object);
		return;
	case enemy_rodney_destroyed_land_ADDRESS:
		enemy_rodney_destroyed_land(object);
		return;
	case enemy_rodney_destroyed_end_ADDRESS:
		player_slot = object_loop_slot;
		enemy_rodney_destroyed_end(object, player_slot);
		return;
	case enemy_shredder_appear_wait_ADDRESS:
		enemy_shredder_appear_wait(object);
		return;
	case enemy_shredder_appear_step_out_ADDRESS:
		enemy_shredder_appear_step_out(object);
		return;
	case enemy_shredder_appear_walk_in_ADDRESS:
		enemy_shredder_appear_walk_in(object);
		return;
	case enemy_shredder_split_start_ADDRESS:
		enemy_shredder_split_start(object);
		return;
	case enemy_shredder_split_blink_ADDRESS:
		enemy_shredder_split_blink(object);
		return;
	case enemy_shredder_clone_appear_start_ADDRESS:
		enemy_shredder_clone_appear_start(object);
		return;
	case enemy_shredder_clone_appear_blink_ADDRESS:
		enemy_shredder_clone_appear_blink(object);
		return;
	case enemy_shredder_walk_to_side_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_walk_to_side_start(object, player_slot);
		return;
	case enemy_shredder_walk_to_side_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_walk_to_side_step(object, player_slot);
		return;
	case enemy_shredder_walk_to_side_crouch_ADDRESS:
		enemy_shredder_walk_to_side_crouch(object);
		return;
	case enemy_shredder_walk_to_side_turn_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_walk_to_side_turn(object, player_slot);
		return;
	case enemy_shredder_walk_to_side_hold_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_walk_to_side_hold(object, player_slot);
		return;
	case enemy_shredder_walk_to_target_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_walk_to_target_start(object, player_slot);
		return;
	case enemy_shredder_walk_to_target_step_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_walk_to_target_step(object, player_slot);
		return;
	case enemy_shredder_guard_start_ADDRESS:
		enemy_shredder_guard_start(object);
		return;
	case enemy_shredder_guard_counter_ADDRESS:
		enemy_shredder_guard_counter(object);
		return;
	case enemy_shredder_back_off_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_back_off_start(object, player_slot);
		return;
	case enemy_shredder_back_off_walk_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_back_off_walk(object, player_slot);
		return;
	case enemy_shredder_back_off_wait_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_back_off_wait(object, player_slot);
		return;
	case enemy_shredder_lose_bar_start_ADDRESS:
		enemy_shredder_lose_bar_start(object);
		return;
	case enemy_shredder_lose_bar_end_ADDRESS:
		enemy_shredder_lose_bar_end(object);
		return;
	case enemy_shredder_flinch_start_ADDRESS:
		enemy_shredder_flinch_start(object);
		return;
	case enemy_shredder_flinch_end_ADDRESS:
		enemy_shredder_flinch_end(object);
		return;
	case enemy_shredder_flinch_from_behind_start_ADDRESS:
		enemy_shredder_flinch_from_behind_start(object);
		return;
	case enemy_shredder_defeated_start_ADDRESS:
		enemy_shredder_defeated_start(object);
		return;
	case enemy_shredder_defeated_fall_ADDRESS:
		enemy_shredder_defeated_fall(object);
		return;
	case enemy_shredder_defeated_land_ADDRESS:
		enemy_shredder_defeated_land(object);
		return;
	case enemy_shredder_defeated_wait_ADDRESS:
		enemy_shredder_defeated_wait(object);
		return;
	case enemy_shredder_defeated_stay_ADDRESS:
		enemy_shredder_defeated_stay();
		return;
	case enemy_shredder_vanish_start_ADDRESS:
		enemy_shredder_vanish_start(object);
		return;
	case enemy_shredder_vanish_wait_ADDRESS:
		enemy_shredder_vanish_wait(object);
		return;
	case enemy_shredder_vanish_blink_ADDRESS:
		enemy_shredder_vanish_blink(object);
		return;
	case enemy_shredder_kick_start_ADDRESS:
		enemy_shredder_kick_start(object);
		return;
	case enemy_shredder_blow_end_ADDRESS:
		enemy_shredder_blow_end(object);
		return;
	case enemy_shredder_thrust_start_ADDRESS:
		enemy_shredder_thrust_start(object);
		return;
	case enemy_shredder_slash_start_ADDRESS:
		enemy_shredder_slash_start(object);
		return;
	case enemy_shredder_sweep_start_ADDRESS:
		enemy_shredder_sweep_start(object);
		return;
	case enemy_shredder_ray_start_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_ray_start(object, player_slot);
		return;
	case enemy_shredder_ray_line_up_ADDRESS:
		player_slot = object_loop_slot;
		enemy_shredder_ray_line_up(object, player_slot);
		return;
	case enemy_shredder_ray_charge_ADDRESS:
		enemy_shredder_ray_charge(object);
		return;
	case enemy_shredder_ray_fire_ADDRESS:
		enemy_shredder_ray_fire(object);
		return;
	case enemy_shredder_ray_end_ADDRESS:
		enemy_shredder_ray_end(object);
		return;
	}
}

/* the object moved by its speeds, its height too */
void move_object_with_height(uint32_t object)
{
	OBJECT(object)->height += OBJECT(object)->height_speed;
	move_object(object);
}

/* the object moved by its speeds across and up or down */
void move_object(uint32_t object)
{
	OBJECT(object)->x += OBJECT(object)->x_speed;
	OBJECT(object)->y += OBJECT(object)->y_speed;
}

/* the object moved by its speeds, its height too, but not into a side that is blocked */
void move_object_with_height_blocked(uint32_t object)
{
	OBJECT(object)->height += OBJECT(object)->height_speed;
	move_object_blocked(object);
}

/* the object moved by its speeds, but not into a side its contacts say is blocked */
void move_object_blocked(uint32_t object)
{
	uint32_t speed = 0;
	uint32_t x_speed = 0, y_speed = 0;

	x_speed = OBJECT(object)->x_speed;
	speed = x_speed;
	if ((int32_t)x_speed >= 0)
	{
		if ((OBJECT(object)->contacts & 8u) == 0u)
			OBJECT(object)->x += speed;
	}
	else if ((OBJECT(object)->contacts & 0x10u) == 0u)
	{
		OBJECT(object)->x += speed;
		y_speed = OBJECT(object)->y_speed;
		speed = y_speed;
		if ((int32_t)y_speed < 0)
		{
			if ((OBJECT(object)->contacts & 4u) == 0u)
				OBJECT(object)->y += speed;
			return;
		}
		if ((OBJECT(object)->contacts & 2u) != 0u)
			return;
		OBJECT(object)->y += speed;
		return;
	}
	y_speed = OBJECT(object)->y_speed;
	speed = y_speed;
	if ((int32_t)y_speed < 0)
	{
		if ((OBJECT(object)->contacts & 4u) == 0u)
			OBJECT(object)->y += speed;
		return;
	}
	if ((OBJECT(object)->contacts & 2u) != 0u)
		return;
	OBJECT(object)->y += speed;
}

/* the chance of the first of its definition's attacks whose reach holds its turtle; none out of reach */
void enemy_attack_chance(uint32_t object, uint32_t *p_chance, uint32_t *p_attack)
{
	uint32_t chance = *p_chance;
	uint32_t attack = *p_attack;
	uint16_t count = 0, target_dx = 0, target_dy = 0;
	uint16_t t1 = 0;

	target_dx = OBJECT(object)->enemy.target_dx;
	if (target_dx >= 0x100u)
		chance = 0xFFFFFFFFu;
	else
	{
		target_dy = OBJECT(object)->enemy.target_dy;
		if (target_dy >= 0x100u)
			chance = 0xFFFFFFFFu;
		else
		{
			count = 3u;
			offset_high_word = 3u;
			for (;;)
			{
				if (ATTACK_RECORD(attack)->reach_y < (uint8_t)target_dy)
				{
					chance = 0xFFFFFFFFu;
					break;
				}
				if ((uint8_t)target_dx < ATTACK_RECORD(attack)->reach_x_min || ATTACK_RECORD(attack)->reach_x_max < (uint8_t)target_dx)
				{
					t1 = count - 1u;
					count = t1;
					attack += 0xCu;
					if (t1 != 0xFFFFu)
						continue;
					chance = 0xFFFFFFFFu;
					break;
				}
				chance = (chance & ~0xFFu) | ATTACK_RECORD(attack)->chance;
				break;
			}
		}
	}
	*p_chance = chance;
	*p_attack = attack;
}

/* where its turtle is from it and which way it faces; a turtle close by with its back turned hears it
   behind */
void enemy_look_at_target(uint32_t object, uint32_t turtle, uint32_t *p_dx, uint32_t *p_dy)
{
	uint8_t faces = 0, target_direction = 0, target_faces = 0;
	uint16_t dx = 0, dy = 0, screen_y = 0;
	uint8_t t3 = 0;
	uint16_t t1 = 0, t2 = 0;

	screen_y = OBJECT(turtle)->screen_y;
	target_direction = 0u;
	t1 = OBJECT(turtle)->screen_x - OBJECT(object)->screen_x;
	dx = t1;
	if ((int16_t)t1 < 0)
	{
		dx = (uint16_t)(0u - dx);
		target_direction = 0x40u;
	}
	faces = OBJECT(turtle)->flags ^ target_direction;
	t2 = screen_y - OBJECT(object)->screen_y;
	dy = t2;
	if ((int16_t)t2 >= 0)
		target_direction ^= 0xC0u;
	else
		dy = (uint16_t)(0u - dy);
	if ((target_direction & 0x40u) != 0u)
	{
		if (dy <= dx)
			target_direction |= 0x20u;
	}
	else if (dx <= dy)
		target_direction |= 0x20u;
	OBJECT(object)->enemy.target_direction = target_direction;
	OBJECT(object)->enemy.target_dx = dx;
	OBJECT(object)->enemy.target_dy = dy;
	t3 = (uint32_t)faces << 1;
	target_faces = t3;
	if ((int8_t)t3 >= 0 && dy < 6u && dx < 0x40u)
		OBJECT(turtle)->turtle.enemy_behind = 0xFFu;
	OBJECT(object)->enemy.target_faces = target_faces;
	*p_dx = (*p_dx & 0xFFFF0000u) | dx;
	*p_dy = (*p_dy & 0xFFFF0000u) | dy;
}

/* the two turtles nearest to the object, kept for later; with no turtle in play, the ones kept before */
uint32_t enemy_nearest_turtles(uint32_t object, uint32_t closest)
{
	uint16_t closest_distance = 0, count = 0, distance = 0, next_distance = 0, y_distance = 0;
	uint32_t object_2 = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	if (turtles_in_play == 0u)
	{
		closest = nearest_turtle;
		object_loop_leftover = second_nearest_turtle;
		return closest;
	}
	count = 3u;
	offset_high_word = 3u;
	object_2 = OBJECT_ADDRESS(0);
	next_distance = 0xFFFFu;
	closest_distance = 0xFFFFu;
	do
	{
		if (OBJECT(object_2)->flags != 0u)
		{
			t1 = OBJECT(object)->screen_x - OBJECT(object_2)->screen_x;
			distance = t1;
			if ((int16_t)t1 < 0)
				distance = (uint16_t)(0u - distance);
			t2 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
			y_distance = t2;
			if ((int16_t)t2 < 0)
				y_distance = (uint16_t)(0u - y_distance);
			distance += y_distance;
			if ((uint16_t)closest_distance <= distance)
			{
				if (distance < (uint16_t)next_distance)
				{
					next_distance = (uint16_t)distance;
					object_loop_leftover = object_2;
				}
			}
			else
			{
				next_distance = (uint16_t)closest_distance;
				object_loop_leftover = closest;
				closest_distance = (uint16_t)distance;
				closest = object_2;
			}
		}
		t3 = count - 1u;
		count = t3;
		object_2 += 0x50u;
	} while (t3 != 0xFFFFu);
	nearest_turtle = closest;
	second_nearest_turtle = object_loop_leftover;
	distance_low_byte = next_distance;
	return closest;
}

/* the two players' slots whose turtles are nearest to the object, kept; with no turtle in play, the ones
   kept before */
uint32_t enemy_nearest_slots(uint32_t object, uint32_t closest)
{
	uint16_t closest_distance = 0, count = 0, distance = 0, next_distance = 0, y_distance = 0;
	uint32_t object_2 = 0, player_slot = 0;
	uint16_t t1 = 0, t2 = 0, t3 = 0;

	if (turtles_in_play == 0u)
	{
		closest = nearest_slot;
		object_loop_leftover = second_nearest_slot;
		return closest;
	}
	count = 3u;
	offset_high_word = 3u;
	player_slot = PLAYER_SLOT_ADDRESS(0);
	next_distance = 0xFFFFu;
	closest_distance = 0xFFFFu;
	do
	{
		object_2 = PLAYER_SLOT(player_slot)->turtle;
		if (OBJECT(object_2)->flags != 0u)
		{
			t1 = OBJECT(object)->screen_x - OBJECT(object_2)->screen_x;
			distance = t1;
			if ((int16_t)t1 < 0)
				distance = (uint16_t)(0u - distance);
			t2 = OBJECT(object)->screen_y - OBJECT(object_2)->screen_y;
			y_distance = t2;
			if ((int16_t)t2 < 0)
				y_distance = (uint16_t)(0u - y_distance);
			distance += y_distance;
			if ((uint16_t)closest_distance <= distance)
			{
				if (distance < (uint16_t)next_distance)
				{
					next_distance = (uint16_t)distance;
					object_loop_leftover = player_slot;
				}
			}
			else
			{
				next_distance = (uint16_t)closest_distance;
				object_loop_leftover = closest;
				closest_distance = (uint16_t)distance;
				closest = player_slot;
			}
		}
		t3 = count - 1u;
		count = t3;
		player_slot += 0x10u;
	} while (t3 != 0xFFFFu);
	nearest_slot = closest;
	second_nearest_slot = object_loop_leftover;
	distance_low_byte = next_distance;
	return closest;
}

/* a random number below the one given, its seed stirred by the frame count and the object's place */
uint32_t random_below(uint16_t range, uint32_t object)
{
	uint16_t range_copy = 0;
	uint32_t random = 0;

	range_copy = (uint16_t)range;
	random = (uint16_t)(random_seed + frame_count - (uint16_t)OBJECT(object)->x + OBJECT(object)->screen_y - (uint16_t)OBJECT(object)->y + OBJECT(object)->screen_x + 0x4ACBu) * 0x17u;
	random_seed = random;
	random = ((uint32_t)(uint16_t)random * range_copy) & ~0xFFFFu;
	random = (random << 16) | (random >> 16);
	offset_high_word = random;
	return random;
}

/* Unused: which of three distances in a list the larger of its distances to its turtle is under, eight a
   step, 0x20 more when the larger is up or down */
void enemy_target_distance_band(uint32_t enemy, uint32_t distances)
{
	uint16_t distance = 0, target_dy = 0;

	distance = OBJECT(enemy)->enemy.target_dx;
	target_dy = OBJECT(enemy)->enemy.target_dy;
	if (distance <= target_dy)
		distance = (uint16_t)target_dy;
}

/* the animation of the table toward the object, higher or lower on the screen, flipped to face it */
void enemy_face_object(uint8_t direction, uint32_t object, uint32_t object_2)
{
	uint32_t facing = 0;

	facing = enemy_facing_bits(direction, 0u, object, object_2);
	enemy_set_facing_animation((uint8_t)facing, object);
}

/* the animation of the table for the facing, one for each way up or down the screen, flipped to its side */
void enemy_set_facing_animation(uint8_t facing, uint32_t object)
{
	uint32_t animation_entry = 0;

	if ((int8_t)facing < 0)
		object_loop_leftover += 4u;
	animation_entry = ANIMATION_ENTRY(object_loop_leftover);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->flags &= 0xBFu;
	if ((facing & 0x40u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy.facing = facing;
}

/* the same toward the object, only when its facing changes */
void enemy_turn_to_object(uint8_t direction, uint32_t object, uint32_t object_2)
{
	uint32_t facing = 0;

	facing = enemy_facing_bits(direction, 0u, object, object_2);
	enemy_turn_animation((uint8_t)facing, object);
}

/* a new animation only when the facing changes between up and down the screen; the flip follows a turn
   across */
void enemy_turn_animation(uint8_t facing, uint32_t object)
{
	uint32_t animation_entry = 0;
	uint8_t t1 = 0;

	t1 = OBJECT(object)->enemy.facing ^ facing;
	OBJECT(object)->enemy.facing = t1;
	if ((int8_t)t1 >= 0)
	{
		if ((OBJECT(object)->enemy.facing & 0x40u) != 0u)
			OBJECT(object)->flags ^= 0x40u;
		OBJECT(object)->enemy.facing = facing;
		return;
	}
	if ((int8_t)facing < 0)
		object_loop_leftover += 4u;
	animation_entry = ANIMATION_ENTRY(object_loop_leftover);
	set_animation(0u, object, animation_entry);
	if ((OBJECT(object)->enemy.facing & 0x40u) != 0u)
		OBJECT(object)->flags ^= 0x40u;
	OBJECT(object)->enemy.facing = facing;
}

/* which way to face the object: by how far it is up or down the screen, and on which side */
uint32_t enemy_facing_bits(uint8_t direction, uint32_t facing_bits, uint32_t object, uint32_t object_2)
{
	uint16_t y_distance = 0;
	uint16_t t1 = 0;

	facing_bits &= ~0xFFFFu;
	t1 = OBJECT(object_2)->screen_y - OBJECT(object)->screen_y;
	y_distance = t1;
	if ((int16_t)t1 >= 0 && y_distance >= 4u)
		facing_bits |= 0x80u;
	if ((int8_t)(direction - 0x40u) < 0)
		facing_bits |= 0x40u;
	return facing_bits;
}

/* the animation of the table for a direction, flipped to face it */
void enemy_face_direction(uint8_t direction, uint32_t object)
{
	uint32_t facing = 0;

	facing = direction_facing_bits(direction, 0u);
	enemy_set_facing_animation((uint8_t)facing, object);
}

/* the same for a direction, only when its facing changes */
void enemy_turn_to_direction(uint8_t direction, uint32_t object)
{
	uint32_t facing = 0;

	facing = direction_facing_bits(direction, 0u);
	enemy_turn_animation((uint8_t)facing, object);
}

/* which way to face for a direction: up or down the screen, left or right */
uint32_t direction_facing_bits(uint8_t direction, uint32_t facing_bits)
{
	facing_bits &= ~0xFFu;
	if ((uint8_t)direction >= 0x84u && (uint8_t)direction < 0xFDu)
		facing_bits |= 0x80u;
	if ((int8_t)(direction - 0x40u) < 0)
		facing_bits |= 0x40u;
	return facing_bits;
}

/* the screen's left and right edges taken as walls for the enemy */
void CONSOLE_ORIGINAL(enemy_keep_on_screen)(uint32_t object)
{
	if (OBJECT(object)->screen_x >= 0x170u)
		OBJECT(object)->contacts |= 8u;
	if (OBJECT(object)->screen_x < 0x80u)
		OBJECT(object)->contacts |= 0x10u;
}

/* whether the enemy is on the screen across */
uint32_t CONSOLE_ORIGINAL(enemy_on_screen)(uint32_t object)
{
	uint32_t zero = 0;

	if (OBJECT(object)->screen_x >= 0x190u || OBJECT(object)->screen_x < 0x70u)
	{
		zero = 0xFFFFFFFFu == 0u;
		offset_high_word = 0xFFFFFFFFu;
		return zero;
	}
	zero = 1u;
	return zero;
}

/* its health bars and health from its table, by the difficulty setting */
void enemy_set_health(uint32_t object, uint32_t settings)
{
	uint32_t health_bars_halfword = 0;

	health_bars_halfword = HEALTH_SETTING(settings + (difficulty & 7u) * 0x2u);
	OBJECT(object)->health_bars = (uint8_t)(health_bars_halfword >> 8u);
	OBJECT(object)->health = (uint8_t)health_bars_halfword;
}

/* its health filled again from its table by the difficulty setting, the bars left as they are */
void enemy_refill_health(uint32_t object, uint32_t settings)
{
	OBJECT(object)->health = HEALTH_SETTING_8(settings + (difficulty & 7u) * 0x2u + 0x1u);
}

/* turns to its turtle and gives its attack the side it faces */
void enemy_face_target(uint8_t attack, uint32_t object)
{
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy.target_direction - 0x40u) < 0)
	{
		OBJECT(object)->flags |= 0x40u;
		attack |= 0x80u;
	}
	OBJECT(object)->attack = attack;
}

/* back to thinking, no longer attacking */
void enemy_back_to_thinking(uint32_t object)
{
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
}

/* back to thinking, and it can be hit again */
void enemy_back_to_thinking_hittable(uint32_t object)
{
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
}

/* whether it is down on its floor again, its height then set to the floor's */
uint32_t enemy_landed(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t height_whole = 0;

	height_whole = OBJECT(object)->height_whole;
	if (height_whole >= 0xF0u || height_whole < OBJECT(object)->floor_height)
	{
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		zero = 1u;
		OBJECT(object)->height &= 0xFF00u;
		return zero;
	}
	zero = 0xFFFFFFFFu == 0u;
	offset_high_word = 0xFFFFFFFFu;
	return zero;
}

/* starts a leap or a fall along an arc record: its speeds and a first step across unless blocked */
void enemy_start_arc(uint32_t object, uint32_t arc)
{
	uint16_t x_step = 0;
	uint32_t x_step_2 = 0;
	uint32_t t1 = 0;

	OBJECT(object)->x_speed = ARC(arc)->x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = ARC(arc)->height_speed;
	x_step = (uint16_t)ARC(arc)->x_step;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		x_step = (uint16_t)(0u - x_step);
	}
	if (area == 7u && OBJECT(object)->floor_height != 0u)
		return;
	t1 = (int16_t)x_step;
	x_step_2 = t1;
	offset_high_word = x_step_2;
	if ((int32_t)t1 >= 0)
	{
		if ((OBJECT(object)->contacts & 8u) == 0u)
			OBJECT(object)->x += x_step_2;
		return;
	}
	if ((OBJECT(object)->contacts & 0x10u) != 0u)
		return;
	OBJECT(object)->x += x_step_2;
}

/* a frame of an arc: the speed across slowed by the record's drag, the fall sped up to its limit */
void enemy_arc_step(uint32_t object, uint32_t arc)
{
	uint16_t height_speed = 0, old_x_speed = 0, x_speed = 0;

	x_speed = (uint16_t)ARC(arc)->x_drag;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		x_speed = (uint16_t)(0u - x_speed);
	old_x_speed = (uint16_t)x_speed;
	x_speed += OBJECT(object)->x_speed;
	if ((int16_t)(old_x_speed ^ x_speed) < 0)
		OBJECT(object)->x_speed = x_speed;
	height_speed = (uint16_t)(ARC(arc)->gravity + OBJECT(object)->height_speed);
	if (ARC(arc)->height_speed_limit <= (int16_t)height_speed)
		OBJECT(object)->height_speed = height_speed;
}

/* gives up its place beside its turtle, if it has one */
uint32_t enemy_leave_side(uint32_t object, uint32_t left_slot)
{
	uint32_t player_slot = 0;

	if (OBJECT(object)->enemy.approach != 1u)
		return left_slot;
	player_slot = object_loop_slot;
	enemy_give_side_back(object, player_slot);
	return player_slot;
}

/* its side of the players' slot given back, and back to its first approach */
void enemy_give_side_back(uint32_t object, uint32_t player_slot)
{
	uint8_t side_mask = 0;
	uint8_t side = 0;

	side_mask = 0x7Fu;
	side = OBJECT(object)->enemy.side;
	OBJECT(object)->enemy.side = side & 0x7Fu;
	if ((side & 0x80u) != 0u)
		side_mask = 0xBFu;
	PLAYER_SLOT(player_slot)->sides &= side_mask;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.move_time = 0u;
}

/* the animation of the table for the direction of its turtle, flipped to face it */
uint32_t enemy_face_target_animation(uint32_t direction, uint32_t object, uint32_t animations)
{
	uint32_t animation_entry = 0;

	direction = LINKED(direction, OBJECT(object)->enemy.target_direction);
	OBJECT(object)->enemy.facing = direction;
	animation_entry = ANIMATION_ENTRY(animations + ((uint16_t)direction >> 3));
	set_animation(0u, object, animation_entry);
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	return direction;
}

/* flashes in the colour given while it has 6 health bars or fewer */
void enemy_flash_when_weak(uint8_t colour, uint32_t object)
{
	if (OBJECT(object)->health_bars <= 6u)
	{
		OBJECT(object)->colour = colour;
		if ((frame_count & 4u) != 0u)
			OBJECT(object)->colour = 0xFFu;
	}
}

/* the floor height under it: raised in the far part of the sewers, else none */
void enemy_set_floor(uint32_t object)
{
	if (area != 3u || OBJECT(object)->y < 0x5800u)
	{
		OBJECT(object)->floor_height = 0u;
		return;
	}
	OBJECT(object)->floor_height = 0x18u;
}

/* the floor map where it stands: its walls and edges into the contacts */
void enemy_floor_contacts(uint32_t object, uint32_t *p_floor_right, uint32_t *p_floor_left, uint32_t *p_floor_up, uint32_t *p_floor_down)
{
	uint32_t floor_cell_place = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u || (int16_t)OBJECT(object)->screen_y < 8)
	{
		*p_floor_right = 1u;
		*p_floor_left = 1u;
		*p_floor_up = 1u;
		*p_floor_down = 1u;
	}
	else
	{
		floor_cell_place = read_floor_cells_on_screen(object);
		enemy_floor_cell_place = floor_cell_place;
		if (floor_cell >= 0xFDu)
		{
			enemy_floor_edges(object, p_floor_right, p_floor_left, p_floor_up, p_floor_down);
			OBJECT(object)->contacts &= TABLE8(floor_contact_masks, 0x3u + (uint32_t)(int16_t)(int8_t)floor_cell);
			object_loop_leftover = 0x4881Bu;
		}
		else
			enemy_floor_edges(object, p_floor_right, p_floor_left, p_floor_up, p_floor_down);
	}
}

/* the floor map's walls around it into the contacts */
void enemy_floor_edges(uint32_t object, uint32_t *p_floor_right, uint32_t *p_floor_left, uint32_t *p_floor_up, uint32_t *p_floor_down)
{
	uint8_t floor_down = 0, floor_left = 0, floor_right = 0, floor_up = 0, new_contacts = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	new_contacts = 0u;
	t1 = floor_cell_right;
	floor_right = t1;
	if ((int8_t)t1 < 0 && floor_right == height_cell_right)
	{
		new_contacts = 8u;
		offset_high_word = 8u;
	}
	t2 = floor_cell_left;
	floor_left = t2;
	if ((int8_t)t2 < 0 && floor_left == height_cell_left)
		new_contacts |= 0x10u;
	t3 = floor_cell_up;
	floor_up = t3;
	if ((int8_t)t3 < 0 && floor_up == height_cell_up)
		new_contacts |= 2u;
	t4 = floor_cell_down;
	floor_down = t4;
	if ((int8_t)t4 < 0)
		new_contacts |= 4u;
	OBJECT(object)->contacts |= new_contacts;
	*p_floor_right = (*p_floor_right & 0xFFFFFF00u) | floor_right;
	*p_floor_left = (*p_floor_left & 0xFFFFFF00u) | floor_left;
	*p_floor_up = (*p_floor_up & 0xFFFFFF00u) | floor_up;
	*p_floor_down = (*p_floor_down & 0xFFFFFF00u) | floor_down;
}

/* whether it steps over an edge: off an object it stands on, or up or down a ledge toward its turtle's
   floor */
uint32_t enemy_check_floor_edge(uint8_t floor_right, uint8_t floor_left, uint8_t floor_up, uint8_t floor_down, uint32_t object, uint32_t slot)
{
	uint32_t zero = 0;
	uint8_t floor_height = 0;
	uint32_t turtle = 0;

	floor_height = OBJECT(object)->floor_height;
	if (floor_height != OBJECT(object)->height_whole)
	{
		if (area != 3u)
		{
			zero = 1u;
			return zero;
		}
		if ((uint8_t)(OBJECT(object)->y >> 8u) >= 0x4Bu)
		{
			OBJECT(object)->height = 0x1800u;
			OBJECT(object)->floor_height = 0x18u;
		}
		else
		{
			OBJECT(object)->height = 0u;
			OBJECT(object)->floor_height = 0u;
		}
		zero = 1u;
		return zero;
	}
	if ((OBJECT(object)->contacts & 1u) != 0u)
	{
		OBJECT(object)->enemy.move = 0x18u;
		zero = 0x600u == 0u;
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		return zero;
	}
	turtle = PLAYER_SLOT(slot)->turtle;
	if (floor_height == OBJECT(turtle)->floor_height)
	{
		zero = 1u;
		return zero;
	}
	if (OBJECT(turtle)->floor_height <= floor_height)
	{
		if ((uint8_t)floor_down == 0xFCu)
		{
			OBJECT(object)->enemy.move = 0xCu;
			zero = 0x600u == 0u;
			OBJECT(object)->routine = 6u;
			OBJECT(object)->state = 0u;
			return zero;
		}
		if ((uint8_t)floor_right != 0xFAu)
		{
			if ((uint8_t)floor_left != 0xFBu)
			{
				zero = 1u;
				return zero;
			}
			OBJECT(object)->enemy.move = 0x14u;
			zero = 0x600u == 0u;
			OBJECT(object)->routine = 6u;
			OBJECT(object)->state = 0u;
			return zero;
		}
		OBJECT(object)->enemy.move = 0x10u;
		zero = 0x600u == 0u;
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		return zero;
	}
	if ((uint8_t)floor_right == 0xFBu)
	{
		OBJECT(object)->enemy.move = 4u;
		zero = 0x600u == 0u;
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		return zero;
	}
	if ((uint8_t)floor_left != 0xFAu)
	{
		if ((uint8_t)floor_up != 0xFCu)
		{
			zero = 1u;
			return zero;
		}
		OBJECT(object)->enemy.move = 0u;
		zero = 0x600u == 0u;
		OBJECT(object)->routine = 6u;
		OBJECT(object)->state = 0u;
		return zero;
	}
	OBJECT(object)->enemy.move = 8u;
	zero = 0x600u == 0u;
	OBJECT(object)->routine = 6u;
	OBJECT(object)->state = 0u;
	return zero;
}

/* on stairs, its floor height up or down with them as it moves */
void enemy_follow_stairs(uint32_t object)
{
	uint8_t floor_cell_here = 0;
	uint32_t floor_cell_place = 0;

	floor_cell_place = enemy_floor_cell_place;
	floor_cell_here = floor_cell;
	if (floor_cell_here < 0xFAu || floor_cell_here >= 0xFCu)
		return;
	if (floor_cell_here == floor_cell_up)
	{
		enemy_stairs_up(object, floor_cell_place);
		return;
	}
	enemy_stairs_down(object, floor_cell_place);
}

/* on stairs, its floor height down with them */
void enemy_follow_stairs_down(uint32_t object)
{
	uint8_t floor_cell_here = 0;
	uint32_t floor_cell_place = 0;

	floor_cell_place = enemy_floor_cell_place;
	floor_cell_here = floor_cell;
	if (floor_cell_here < 0xFAu || floor_cell_here >= 0xFDu)
		return;
	enemy_stairs_down(object, floor_cell_place);
}

/* its floor height down a step for every stairs row of the floor map below it */
void enemy_stairs_down(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_height = 0;

	do
	{
		floor_height = OBJECT(object)->floor_height;
		OBJECT(object)->floor_height = floor_height - 8u;
		if (floor_height < 8u)
			OBJECT(object)->floor_height = 0u;
		floor_cell_place += 0x40u;
		if (floor_cell_place >= OBJECT_ADDRESS(0))
			floor_cell_place -= 0x800u;
	} while ((int8_t)FLOOR_MAP_CELL(floor_cell_place)->cell < 0);
}

/* on stairs, its floor height up with them */
void enemy_follow_stairs_up(uint32_t object)
{
	uint8_t floor_cell_here = 0;
	uint32_t floor_cell_place = 0;

	floor_cell_place = enemy_floor_cell_place;
	floor_cell_here = floor_cell;
	if (floor_cell_here < 0xFAu || floor_cell_here >= 0xFDu)
		return;
	enemy_stairs_up(object, floor_cell_place);
}

/* its floor height up a step for every stairs row above it, no higher than the stairs' top */
void enemy_stairs_up(uint32_t object, uint32_t floor_cell_place)
{
	uint8_t floor_height = 0;

	do
	{
		OBJECT(object)->floor_height += 8u;
		floor_height = stairs_top_height;
		if (floor_height < OBJECT(object)->floor_height)
			OBJECT(object)->floor_height = floor_height;
		floor_cell_place -= 0x40u;
		if (floor_cell_place < floor_map_ADDRESS)
			floor_cell_place += 0x800u;
	} while ((int8_t)FLOOR_MAP_CELL(floor_cell_place)->cell < 0);
}

/* kind 00, the bare-handed Foot soldier: its floor, one more enemy on the screen, then its setup */
void enemy_foot_soldier_start(uint32_t object, uint32_t player_slot)
{
	enemy_set_floor(object);
	enemies_on_screen++;
	enemy_foot_soldier_setup(object, player_slot);
}

/* the bare-handed soldier's turtle, health, definition and shadow; its way in by its parameter, or
   straight to thinking */
void enemy_foot_soldier_setup(uint32_t object, uint32_t player_slot)
{
	uint8_t first_move = 0;
	uint32_t animation = 0;
	uint8_t parameter = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->throw = 0x84u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = enemy_definitions_ADDRESS;
	parameter = OBJECT(object)->parameter;
	first_move = parameter;
	if (parameter != 0u)
	{
		OBJECT(object)->enemy.move = (uint32_t)first_move << 2;
		OBJECT(object)->routine++;
		return;
	}
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->routine += 2u;
	animation = enemy_foot_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the way in its move picks, keeping track of its turtle, the objects and the floor */
void enemy_enter(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	states = TABLE32(enemy_ways_in, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	object_loop_slot = slot;
	run_object_state(object, states);
}

/* the thinking: a new turtle every 16 frames, a side by it or away, an attack by its definition's
   chances or its next move */
void enemy_think(uint32_t object, uint32_t second_slot)
{
	bool L_048BCA = false, L_048CC0 = false;
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint16_t count = 0, floor_offset = 0;
	uint32_t approach = 0, attack = 0, chance = 0, closest_slot = 0, dodge_chance = 0, floor_down = 0, floor_left = 0, floor_right = 0, floor_up = 0, ignored = 0, ignored_2 = 0, roll = 0, slot = 0, turtle = 0;
	uint8_t retarget_time = 0, t2 = 0, floor_height = 0, sides = 0, sides_2 = 0, move_chance = 0, t3 = 0, chance_2 = 0, t5 = 0;
	uint16_t t4 = 0, t6 = 0;
	uint32_t t1 = 0;

	slot = 0x60000u | OBJECT(object)->enemy.target;
	retarget_time = OBJECT(object)->enemy.retarget_time;
	OBJECT(object)->enemy.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			PLAYER_SLOT(slot)->enemies--;
			second_slot = enemy_nearest_slots(object, second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = object_loop_leftover;
				closest_slot = second_slot;
				object_loop_leftover = closest_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						object_loop_leftover = second_slot;
						second_slot = closest_slot;
					}
				}
			}
			if ((uint16_t)second_slot != OBJECT(object)->enemy.target)
			{
				object_loop_slot = slot;
				enemy_leave_side(object, slot);
			}
			OBJECT(object)->enemy.target = second_slot;
			slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	if (boss_defeated != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t2 = enemies_on_screen - 1u;
		enemies_on_screen = t2;
		if ((int8_t)t2 < 0)
			enemies_on_screen = 0u;
		object_loop_slot = slot;
		return;
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		object_loop_slot = slot;
		slot = enemy_leave_side(object, slot);
		PLAYER_SLOT(slot)->enemies--;
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t2 = enemies_on_screen - 1u;
		enemies_on_screen = t2;
		if ((int8_t)t2 < 0)
			enemies_on_screen = 0u;
		object_loop_slot = slot;
		return;
	}
	floor_height = OBJECT(object)->floor_height;
	floor_offset = floor_height;
	if (floor_height != 0u && (int16_t)(floor_offset + OBJECT(object)->screen_y) >= 0xF0)
	{
		object_loop_slot = slot;
		slot = enemy_leave_side(object, slot);
		PLAYER_SLOT(slot)->enemies--;
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t2 = enemies_on_screen - 1u;
		enemies_on_screen = t2;
		if ((int8_t)t2 < 0)
			enemies_on_screen = 0u;
		object_loop_slot = slot;
		return;
	}
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &floor_right, &floor_left, &floor_up, &floor_down);
	zero = enemy_check_floor_edge((uint8_t)floor_right, (uint8_t)floor_left, (uint8_t)floor_up, (uint8_t)floor_down, object, slot);
	if (zero == 0u)
	{
		object_loop_slot = slot;
		return;
	}
	if (OBJECT(object)->enemy.approach != 0u)
	{
		if (OBJECT(object)->enemy.approach != 1u)
		{
			if (OBJECT(object)->enemy.approach == 2u && PLAYER_SLOT(slot)->sides < 0xC0u)
			{
				OBJECT(object)->enemy.approach = 0u;
				OBJECT(object)->enemy.move_time = 0u;
			}
		}
		else if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x50u)
			enemy_give_side_back(object, slot);
	}
	else if (PLAYER_SLOT(slot)->sides < 0xC0u)
	{
		if (OBJECT(object)->enemy.target_dx < 0x40u && OBJECT(object)->enemy.target_dy < 0x40u)
		{
			OBJECT(object)->enemy.approach = 1u;
			OBJECT(object)->enemy.move_time = 0u;
			do
			{
				if (L_048BCA || (int8_t)(OBJECT(object)->enemy.target_direction - 0x40u) < 0)
				{
					for (;;)
					{
						L_048BCA = false;
						sides = PLAYER_SLOT(slot)->sides;
						PLAYER_SLOT(slot)->sides = sides | 0x40u;
						if ((sides & 0x40u) != 0u)
						{
							sides_2 = PLAYER_SLOT(slot)->sides;
							PLAYER_SLOT(slot)->sides = sides_2 | 0x80u;
							if ((sides_2 & 0x80u) != 0u)
								continue;
							OBJECT(object)->enemy.side &= 0x7Fu;
							break;
						}
						OBJECT(object)->enemy.side |= 0x80u;
						break;
					}
				}
				else
				{
					sides_2 = PLAYER_SLOT(slot)->sides;
					PLAYER_SLOT(slot)->sides = sides_2 | 0x80u;
					L_048BCA = (sides_2 & 0x80u) != 0u;
					if (!L_048BCA)
						OBJECT(object)->enemy.side &= 0x7Fu;
				}
			} while (L_048BCA);
		}
	}
	else
	{
		OBJECT(object)->enemy.approach = 2u;
		OBJECT(object)->enemy.move_time = 0u;
	}
	if (OBJECT(object)->enemy.approach != 1u)
		L_048CC0 = true;
	else
	{
		turtle = PLAYER_SLOT(slot)->turtle;
		if (OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u)
		{
			L_048CC0 = OBJECT(PLAYER_SLOT(slot)->turtle)->routine != 3u || OBJECT(object)->enemy.target_dy >= 8u || (int8_t)OBJECT(object)->enemy.target_faces >= 0;
			if (!L_048CC0)
			{
				dodge_chance = 0x64u;
				dodge_chance = random_below((uint16_t)dodge_chance, object);
				L_048CC0 = OBJECT(object)->enemy.dodge_chance <= (uint8_t)dodge_chance;
				if (!L_048CC0)
				{
					enemy_give_side_back(object, slot);
					OBJECT(object)->enemy.approach = 4u;
					OBJECT(object)->enemy.move_time = 0u;
				}
			}
		}
		else
		{
			enemy_give_side_back(object, slot);
			OBJECT(object)->enemy.approach = 3u;
			OBJECT(object)->enemy.move_time = 0u;
		}
	}
	if (L_048CC0)
	{
		zero = enemy_on_screen(object);
		if (zero)
		{
			turtle = PLAYER_SLOT(slot)->turtle;
			if (OBJECT(turtle)->routine != 4u || (int8_t)hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] < 0)
			{
				attack = OBJECT(object)->enemy.definition + 0x28u;
				enemy_attack_chance(object, &chance, &attack);
				if ((int8_t)chance >= 0)
				{
					roll = 0x64u;
					roll = random_below((uint16_t)roll, object);
					if ((uint8_t)roll < (uint8_t)chance)
					{
						roll = 0x64u;
						roll = random_below((uint16_t)roll, object);
						count = 3u;
						do
						{
							move_chance = ATTACK_RECORD(attack + (int16_t)count)->move_chance;
							t3 = roll;
							roll = (uint8_t)(t3 - move_chance);
							if (t3 < move_chance)
							{
								OBJECT(object)->enemy.move = ((count & ~0xFFu) | ATTACK_RECORD(attack + (int16_t)count)->move) << 2;
								OBJECT(object)->routine = 3u;
								OBJECT(object)->state = 0u;
								OBJECT(object)->flags &= ~8u;
								object_loop_slot = slot;
								return;
							}
							t4 = count - 1u;
							count = t4;
						} while (t4 != 0xFFFFu);
					}
				}
			}
		}
	}
	if (OBJECT(object)->enemy.move_time == 0u)
	{
		approach = OBJECT(object)->enemy.definition + (uint16_t)((uint32_t)OBJECT(object)->enemy.approach << 3);
		roll = 0x64u;
		roll = random_below((uint16_t)roll, object);
		count = 3u;
		for (;;)
		{
			chance_2 = APPROACH_MOVES(approach + (int16_t)count)->chance;
			t5 = roll;
			roll = (uint8_t)(t5 - chance_2);
			if (t5 < chance_2)
				break;
			t6 = count - 1u;
			count = t6;
			if (t6 == 0xFFFFu)
			{
				count = 0u;
				break;
			}
		}
		OBJECT(object)->enemy.move = ((count & ~0xFFu) | APPROACH_MOVES(approach + (int16_t)count)->move) << 2;
		OBJECT(object)->state = 0u;
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy.move)
	{
	case 20:
		object_loop_slot = slot;
		enemy_shuffle(object);
		return;
	case 28:
		object_loop_slot = slot;
		enemy_circle(object);
		return;
	case 36:
		object_loop_slot = slot;
		enemy_charge(object);
		return;
	case 0:
		object_loop_slot = slot;
		enemy_walk_to_target(object);
		return;
	case 4:
		object_loop_slot = slot;
		enemy_walk_to_side(object);
		return;
	case 8:
		object_loop_slot = slot;
		enemy_keep_distance(object);
		return;
	case 12:
		object_loop_slot = slot;
		enemy_back_off(object);
		return;
	case 16:
		object_loop_slot = slot;
		enemy_run_away(object);
		return;
	case 44:
		object_loop_slot = slot;
		enemy_walk_across(object);
		return;
	case 48:
		object_loop_slot = slot;
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		object_loop_slot = slot;
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		object_loop_slot = slot;
		enemy_walk_past_sword(object);
		return;
	case 32:
		object_loop_slot = slot;
		enemy_back_off_sword(object);
		return;
	case 40:
		object_loop_slot = slot;
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		object_loop_slot = slot;
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		object_loop_slot = slot;
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		object_loop_slot = slot;
		enemy_stalk_hammer(object);
		return;
	case 64:
		object_loop_slot = slot;
		enemy_walk_away_hammer(object);
		return;
	case 72:
		object_loop_slot = slot;
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		object_loop_slot = slot;
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		object_loop_slot = slot;
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		object_loop_slot = slot;
		enemy_back_off_spear(object);
		return;
	case 112:
		object_loop_slot = slot;
		enemy_step_aside_spear(object);
		return;
	case 80:
		object_loop_slot = slot;
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		object_loop_slot = slot;
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		object_loop_slot = slot;
		enemy_circle_knife(object);
		return;
	case 124:
		object_loop_slot = slot;
		enemy_keep_distance_knife(object);
		return;
	case 120:
		object_loop_slot = slot;
		enemy_back_off_knife(object);
		return;
	case 128:
		object_loop_slot = slot;
		enemy_step_aside_knife(object);
		return;
	case 88:
		object_loop_slot = slot;
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		object_loop_slot = slot;
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		object_loop_slot = slot;
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		object_loop_slot = slot;
		enemy_back_off_gun(object);
		return;
	case 144:
		object_loop_slot = slot;
		enemy_run_away_gun(object);
		return;
	case 96:
		object_loop_slot = slot;
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		object_loop_slot = slot;
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		object_loop_slot = slot;
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		object_loop_slot = slot;
		enemy_back_off_axe(object);
		return;
	case 136:
		object_loop_slot = slot;
		enemy_run_away_axe(object);
		return;
	case 196:
		object_loop_slot = slot;
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		object_loop_slot = slot;
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		object_loop_slot = slot;
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		object_loop_slot = slot;
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		object_loop_slot = slot;
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		object_loop_slot = slot;
		enemy_rocksteady_circle(object);
		return;
	case 208:
		object_loop_slot = slot;
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		object_loop_slot = slot;
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		object_loop_slot = slot;
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		object_loop_slot = slot;
		enemy_bebop_stand(object);
		return;
	case 216:
		object_loop_slot = slot;
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		object_loop_slot = slot;
		enemy_bebop_walk_level(object);
		return;
	case 220:
		object_loop_slot = slot;
		enemy_bebop_close_in(object);
		return;
	case 156:
		object_loop_slot = slot;
		enemy_mouser_chase(object);
		return;
	case 164:
		object_loop_slot = slot;
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		object_loop_slot = slot;
		enemy_mouser_back_off(object);
		return;
	case 172:
		object_loop_slot = slot;
		enemy_rodney_rush(object);
		return;
	case 176:
		object_loop_slot = slot;
		enemy_rodney_rush_across(object);
		return;
	case 180:
		object_loop_slot = slot;
		enemy_rodney_back_off(object);
		return;
	case 236:
		object_loop_slot = slot;
		enemy_shredder_appear(object);
		return;
	case 240:
		object_loop_slot = slot;
		enemy_shredder_split(object);
		return;
	case 168:
		object_loop_slot = slot;
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		object_loop_slot = slot;
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		object_loop_slot = slot;
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		object_loop_slot = slot;
		enemy_shredder_guard(object);
		return;
	case 256:
		object_loop_slot = slot;
		enemy_shredder_back_off(object);
		return;
	}
}

/* the attack its move picks, keeping track of its turtle, the objects and the floor */
void enemy_attack(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, turtle = 0;
	uint32_t jump_target = 0;

	slot = 0x60000u | OBJECT(object)->enemy.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	jump_target = TABLE32(enemy_attacks, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	switch (jump_target)
	{
	case enemy_jab_ADDRESS:
		object_loop_slot = slot;
		enemy_jab(object);
		return;
	case enemy_chop_ADDRESS:
		object_loop_slot = slot;
		enemy_chop(object);
		return;
	case enemy_swing_ADDRESS:
		object_loop_slot = slot;
		enemy_swing(object);
		return;
	case enemy_high_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_high_kick(object);
		return;
	case enemy_jump_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_jump_kick(object);
		return;
	case enemy_long_jump_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_long_jump_kick(object);
		return;
	case enemy_attack_none_after_long_jump_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_attack_none_after_long_jump_kick();
		return;
	case enemy_attack_none_before_grab_ADDRESS:
		object_loop_slot = slot;
		enemy_attack_none_before_grab();
		return;
	case enemy_grab_ADDRESS:
		object_loop_slot = slot;
		enemy_grab(object);
		return;
	case enemy_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_slash_sword(object);
		return;
	case enemy_low_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_low_slash_sword(object);
		return;
	case enemy_leaping_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_leaping_slash_sword(object);
		return;
	case enemy_jump_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_jump_slash_sword(object);
		return;
	case enemy_flying_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_flying_slash_sword(object);
		return;
	case enemy_smash_hammer_ADDRESS:
		object_loop_slot = slot;
		enemy_smash_hammer(object);
		return;
	case enemy_spear_thrust_ADDRESS:
		object_loop_slot = slot;
		enemy_spear_thrust(object);
		return;
	case enemy_spear_throw_ADDRESS:
		object_loop_slot = slot;
		enemy_spear_throw(object);
		return;
	case enemy_knife_stab_ADDRESS:
		object_loop_slot = slot;
		enemy_knife_stab(object);
		return;
	case enemy_high_kick_knife_ADDRESS:
		object_loop_slot = slot;
		enemy_high_kick_knife(object);
		return;
	case enemy_jump_kick_knife_ADDRESS:
		object_loop_slot = slot;
		enemy_jump_kick_knife(object);
		return;
	case enemy_throw_knife_ADDRESS:
		object_loop_slot = slot;
		enemy_throw_knife(object);
		return;
	case enemy_throw_axes_ADDRESS:
		object_loop_slot = slot;
		enemy_throw_axes(object);
		return;
	case enemy_high_kick_axe_ADDRESS:
		object_loop_slot = slot;
		enemy_high_kick_axe(object);
		return;
	case enemy_gun_swing_ADDRESS:
		object_loop_slot = slot;
		enemy_gun_swing(object);
		return;
	case enemy_shoot_gun_ADDRESS:
		object_loop_slot = slot;
		enemy_shoot_gun(object);
		return;
	case enemy_throw_shuriken_ADDRESS:
		object_loop_slot = slot;
		enemy_throw_shuriken(object);
		return;
	case enemy_boomerang_throw_ADDRESS:
		object_loop_slot = slot;
		enemy_boomerang_throw(object);
		return;
	case enemy_rocksteady_pincer_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_pincer_charge(object);
		return;
	case enemy_rocksteady_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_charge(object);
		return;
	case enemy_rocksteady_punch_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_punch(object);
		return;
	case enemy_rocksteady_gun_butt_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_gun_butt(object);
		return;
	case enemy_rocksteady_shoot_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_shoot(object);
		return;
	case enemy_bebop_pincer_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_pincer_charge(object);
		return;
	case enemy_bebop_punch_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_punch(object);
		return;
	case enemy_bebop_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_charge(object);
		return;
	case enemy_bebop_shoot_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_shoot(object);
		return;
	case enemy_rodney_shoot_ADDRESS:
		object_loop_slot = slot;
		enemy_rodney_shoot(object);
		return;
	case enemy_rodney_whip_ADDRESS:
		object_loop_slot = slot;
		enemy_rodney_whip(object);
		return;
	case enemy_shredder_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_kick(object);
		return;
	case enemy_shredder_thrust_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_thrust(object);
		return;
	case enemy_shredder_slash_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_slash(object);
		return;
	case enemy_shredder_sweep_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_sweep(object);
		return;
	case enemy_shredder_ray_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_ray(object);
		return;
	}
}

/* hit: the sound, then its reaction by where the blow came from, its health and its definition */
void enemy_hit(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t hit_side = 0;
	uint32_t reactions = 0, turtle = 0;

	OBJECT(object)->flags &= ~4u;
	sound = 0xA8u;
	queue_sound(sound);
	if (OBJECT(object)->enemy.move == 0x20u && (uint8_t)(OBJECT(object)->hit_routine >> 8u) == 3u && OBJECT(object)->hit_state != 0u)
	{
		OBJECT(object)->flags |= 0x80u;
		turtle = 0x60000u | OBJECT(object)->hit_object;
		if (OBJECT(turtle)->routine == 4u && (OBJECT(turtle)->turtle.hit_reaction == 0xEu || OBJECT(turtle)->turtle.hit_reaction == 0x15u))
			OBJECT(turtle)->turtle.escape_presses = 0x10u;
	}
	hit_side = OBJECT(object)->hit;
	if ((int8_t)(((uint32_t)OBJECT(object)->flags << 1) ^ hit_side) >= 0)
	{
		if (OBJECT(object)->health != 0u)
		{
			if (OBJECT(object)->floor_height != OBJECT(object)->height_whole)
				OBJECT(object)->enemy.move = 0x10u;
			else
			{
				hit_side &= 0x7Fu;
				reactions = OBJECT(object)->enemy.definition + 0x58u;
				OBJECT(object)->enemy.move = (uint32_t)REACTION(reactions + hit_side) << 2;
			}
			OBJECT(object)->routine++;
			enemy_react(object);
			return;
		}
		if (diagonal_scroll >= 0)
		{
			reactions = OBJECT(object)->enemy.definition + 0x58u;
			if ((int8_t)REACTION(reactions + (hit_side & 0x7Fu)) >= 0)
			{
				OBJECT(object)->enemy.move = 0x10u;
				OBJECT(object)->routine++;
				enemy_react(object);
				return;
			}
		}
		OBJECT(object)->enemy.move = 0x20u;
	}
	else
	{
		if (OBJECT(object)->health != 0u)
		{
			if (OBJECT(object)->floor_height != OBJECT(object)->height_whole)
			{
				OBJECT(object)->enemy.move = 0x14u;
				OBJECT(object)->routine++;
				enemy_react(object);
				return;
			}
			OBJECT(object)->enemy.move = 0xCu;
			OBJECT(object)->routine++;
			enemy_react(object);
			return;
		}
		if (diagonal_scroll >= 0)
		{
			reactions = OBJECT(object)->enemy.definition + 0x58u;
			if ((int8_t)REACTION(reactions + (hit_side & 0x7Fu)) >= 0)
			{
				OBJECT(object)->enemy.move = 0x14u;
				OBJECT(object)->routine++;
				enemy_react(object);
				return;
			}
		}
		OBJECT(object)->enemy.move = 0x24u;
	}
	OBJECT(object)->routine++;
	enemy_react(object);
}

/* the reaction to a hit its move picks, against the objects and the floor */
void enemy_react(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0;

	object_loop_slot = 0x60000u | OBJECT(object)->enemy.target;
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored, &ignored_2, &ignored_3, &ignored_4);
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy.move)
	{
	case 228:
		enemy_crash_into_scenery(object);
		return;
	case 232:
		enemy_crash_into_scenery_low(object);
		return;
	case 0:
		enemy_flinch_back(object);
		return;
	case 4:
		enemy_flinch_bent(object);
		return;
	case 8:
		enemy_flinch_reel(object);
		return;
	case 12:
		enemy_flinch_hunched(object);
		return;
	case 36:
		enemy_killed_back(object);
		return;
	case 32:
		enemy_killed_forward(object);
		return;
	case 16:
		enemy_knocked_forward(object);
		return;
	case 128:
		enemy_thrown_by_donatello(object);
		return;
	case 132:
		enemy_let_go_by_donatello(object);
		return;
	case 136:
		enemy_thrown_by_michelangelo(object);
		return;
	case 140:
		enemy_let_go_by_michelangelo(object);
		return;
	case 188:
		enemy_thrown_by_raphael(object);
		return;
	case 192:
		enemy_let_go_by_raphael(object);
		return;
	case 196:
		enemy_thrown_by_leonardo(object);
		return;
	case 200:
		enemy_let_go_by_leonardo(object);
		return;
	case 20:
		enemy_knocked_back(object);
		return;
	case 24:
		enemy_die(object);
		return;
	case 28:
		enemy_die_on_back(object);
		return;
	case 40:
		enemy_flinch_back_sword(object);
		return;
	case 44:
		enemy_flinch_bent_sword(object);
		return;
	case 48:
		enemy_flinch_reel_sword(object);
		return;
	case 52:
		enemy_flinch_hunched_sword(object);
		return;
	case 56:
		enemy_flinch_back_hammer(object);
		return;
	case 60:
		enemy_flinch_bent_hammer(object);
		return;
	case 68:
		enemy_flinch_hunched_hammer(object);
		return;
	case 72:
		enemy_flinch_back_spear(object);
		return;
	case 76:
		enemy_flinch_bent_spear(object);
		return;
	case 80:
		enemy_flinch_reel_spear(object);
		return;
	case 84:
		enemy_flinch_hunched_spear(object);
		return;
	case 88:
		enemy_flinch_back_knife(object);
		return;
	case 92:
		enemy_flinch_bent_knife(object);
		return;
	case 96:
		enemy_flinch_reel_knife(object);
		return;
	case 100:
		enemy_flinch_hunched_knife(object);
		return;
	case 120:
		enemy_flinch_back_gun(object);
		return;
	case 124:
		enemy_flinch_bent_gun(object);
		return;
	case 104:
		enemy_flinch_back_axe(object);
		return;
	case 108:
		enemy_flinch_bent_axe(object);
		return;
	case 112:
		enemy_flinch_reel_axe(object);
		return;
	case 164:
		enemy_rocksteady_flinch(object);
		return;
	case 168:
		enemy_rocksteady_reel(object);
		return;
	case 160:
		enemy_rocksteady_flinch_from_behind(object);
		return;
	case 152:
		enemy_rocksteady_knocked_down(object);
		return;
	case 156:
		enemy_rocksteady_defeated(object);
		return;
	case 172:
		enemy_bebop_flinch(object);
		return;
	case 176:
		enemy_bebop_flinch_from_behind(object);
		return;
	case 180:
		enemy_bebop_knocked_down(object);
		return;
	case 184:
		enemy_bebop_defeated(object);
		return;
	case 148:
		enemy_mouser_thrown_off(object);
		return;
	case 224:
		enemy_mouser_break_down(object);
		return;
	case 144:
		enemy_mouser_knocked_flying(object);
		return;
	case 64:
		enemy_rodney_knocked_back(object);
		return;
	case 116:
		enemy_rodney_destroyed(object);
		return;
	case 204:
		enemy_shredder_lose_bar(object);
		return;
	case 208:
		enemy_shredder_flinch(object);
		return;
	case 212:
		enemy_shredder_flinch_from_behind(object);
		return;
	case 216:
		enemy_shredder_defeated(object);
		return;
	case 220:
		enemy_shredder_vanish(object);
		return;
	}
}

/* the step over a floor edge its move picks, keeping track of its turtle and the floor */
void enemy_step_over_edge(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	states = TABLE32(enemy_edge_steps, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	object_loop_slot = slot;
	run_object_state(object, states);
}

/* crouches before a hop over a floor edge */
void enemy_hop_crouch(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, hops over an edge above or below it on the screen, as high as the area says */
void enemy_hop_down(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = area_hop_heights[area & 0xFu];
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x100;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the hop, its floor raised by stairs; on landing a crouch and 16 frames still */
void enemy_hop_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_up(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* still for a moment after landing, then back to thinking */
void enemy_hop_land(uint32_t object)
{
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* when the crouch ends, hops over an edge to the right, as high as the area says */
void enemy_hop_right(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = area_hop_heights[area & 0xFu];
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags |= 0x40u;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, hops over an edge to the left, as high as the area says */
void enemy_hop_left(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = area_hop_heights[area & 0xFu];
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags &= 0xBFu;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* crouches as before jumping down off a ledge */
void enemy_hop_up_crouch(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_crouch_to_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, jumps over an edge above or below it on the screen */
void enemy_hop_up(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = -0x80;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	animation = enemy_hop_up_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump, its floor lowered by stairs; on landing a drop pose and 16 frames still */
void enemy_hop_up_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, jumps over an edge to the right */
void enemy_hop_up_right(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags |= 0x40u;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump, not into walls, its floor lowered by stairs; on landing a crouch and 16 frames
   still */
void enemy_hop_up_fall_blocked(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, jumps over an edge to the left */
void enemy_hop_up_left(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags &= 0xBFu;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* sidesteps back and forth in its stance four times, then a new move */
void enemy_shuffle(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shuffle_states_ADDRESS;
	run_object_state(object, states);
}

/* four sidesteps to go, from its stance */
void enemy_shuffle_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_shuffle.steps = 4u;
	OBJECT(object)->state++;
	animation = enemy_shuffle_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the stance ends, sidesteps the way it faces */
void enemy_shuffle_step(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->x_speed = 0x100;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->state++;
	animation = enemy_shuffle_step_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the sidestep; at its end the other way, and after the fourth a last pose */
void enemy_shuffle_turn(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t steps = 0;

	if (OBJECT(object)->frame_time != 0u)
	{
		move_object_blocked(object);
		return;
	}
	steps = OBJECT(object)->enemy_shuffle.steps;
	OBJECT(object)->enemy_shuffle.steps = steps - 1u;
	if (steps != 1u)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		animation = enemy_shuffle_step_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_shuffle_turn_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when its last pose ends, its move and target timers cleared */
void enemy_shuffle_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_shuffle.move_time = 0u;
		OBJECT(object)->enemy_shuffle.retarget_time = 0u;
	}
}

/* Unused: a move run from a table of states that no move picks */
void enemy_retreat(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_retreat_states_ADDRESS;
	run_object_state(object, states);
}

/* Unused: the enemy backs away from its turtle facing it */
void enemy_retreat_start(uint32_t object)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0;

	OBJECT(object)->enemy.move_time = 0x54u;
	target_direction = OBJECT(object)->enemy.target_direction;
	OBJECT(object)->x_speed = 0x180;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(target_direction - 0x40u) < 0)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		OBJECT(object)->flags |= 0x40u;
	}
	OBJECT(object)->state++;
	animation = enemy_run_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Unused: the enemy backs away until its move is over */
void enemy_retreat_step(uint32_t object)
{
	OBJECT(object)->enemy.move_time--;
	move_object_blocked(object);
}

/* runs round its turtle for 180 frames, one way or the other at random, turning back when it bumps into
   something */
void enemy_circle(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_circle_states_ADDRESS;
	run_object_state(object, states);
}

/* 180 frames of running round its turtle, facing it, one way round or the other at random */
void enemy_circle_start(uint32_t object)
{
	uint32_t animations = 0, direction = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_circle.move_time = 0xB4u;
	animations = enemy_run_animations_ADDRESS;
	direction = enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->enemy_circle.clockwise = 0u;
	if ((int8_t)(uint8_t)(random_seed >> 8u) >= 0)
	{
		enemy_circle_aim((uint8_t)direction, object);
		return;
	}
	OBJECT(object)->enemy_circle.clockwise = 1u;
	enemy_circle_aim((uint8_t)direction, object);
}

/* its speeds for running round its turtle the way it goes */
void enemy_circle_aim(uint8_t direction, uint32_t object)
{
	uint16_t speed = 0;
	uint32_t x_speed = 0, y_speed = 0;

	if (OBJECT(object)->enemy_circle.clockwise != 0u)
		direction += 0x50u;
	else
		direction -= 0x30u;
	speed = 0x180u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
}

/* a frame of the run round its turtle: the other way when it bumps into something, aiming again as its
   turtle moves */
void enemy_circle_step(uint32_t object)
{
	uint32_t animations = 0, direction = 0;

	OBJECT(object)->enemy_circle.move_time--;
	if ((OBJECT(object)->contacts & 0x1Eu) != 0u)
	{
		OBJECT(object)->enemy_circle.clockwise ^= 1u;
		animations = enemy_run_animations_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_aim((uint8_t)direction, object);
		move_object_blocked(object);
		return;
	}
	if (OBJECT(object)->enemy_circle.target_direction != OBJECT(object)->enemy_circle.facing)
	{
		animations = enemy_run_animations_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_aim((uint8_t)direction, object);
	}
	move_object_blocked(object);
}

/* dashes across at its turtle and brakes past it */
void enemy_charge(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_charge_states_ADDRESS;
	run_object_state(object, states);
}

/* a dash across toward its turtle at full speed, for at most 180 frames */
void enemy_charge_start(uint32_t object)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, roll = 0;

	roll = 0x100u;
	roll = random_below((uint16_t)roll, object);
	OBJECT(object)->enemy_charge.roll = roll;
	target_direction = OBJECT(object)->enemy_charge.target_direction;
	OBJECT(object)->x_speed = 0x300;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->flags |= 0x40u;
	if ((int8_t)(target_direction - 0x40u) >= 0)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		OBJECT(object)->flags &= 0xBFu;
	}
	OBJECT(object)->state++;
	OBJECT(object)->enemy_charge.move_time = 0xB4u;
	animation = enemy_dash_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the dash until it is level with its turtle, then the braking time: longer, at random, when
   off its row */
void enemy_charge_run(uint32_t object)
{
	uint8_t move_time = 0;
	uint16_t target_dy = 0;

	target_dy = OBJECT(object)->enemy_charge.target_dy;
	if (OBJECT(object)->enemy_charge.target_dx > 0x28u)
	{
		move_object_blocked(object);
		return;
	}
	move_time = 7u;
	if (target_dy >= 0x10u && (int8_t)OBJECT(object)->enemy_charge.roll < 0)
		move_time = 0x20u;
	OBJECT(object)->enemy_charge.move_time = move_time;
	OBJECT(object)->state++;
	move_object_blocked(object);
}

/* runs on, then slows down in its last frames before the next move */
void enemy_charge_brake(uint32_t object)
{
	uint16_t brake_step = 0;

	OBJECT(object)->enemy_charge.move_time--;
	if (OBJECT(object)->enemy_charge.move_time >= 7u)
	{
		move_object_blocked(object);
		return;
	}
	brake_step = 0x80u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		brake_step = (uint16_t)(0u - brake_step);
	OBJECT(object)->x_speed += brake_step;
	move_object_blocked(object);
}

/* Unused: an enemy crashes into the scenery, which breaks, and dies */
void enemy_crash_into_scenery(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_crash_into_scenery_states_ADDRESS;
	run_object_state(object, states);
}

/* Unused: the scenery breaks where the enemy crashes, and it lies there */
void enemy_crash_into_scenery_start(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t place = 0;
	uint32_t animation = 0, block = 0;

	sound = 0x40u;
	queue_sound(sound);
	place = 0x14u;
	block = scenery_crash_tile_block_1_ADDRESS;
	draw_tile_block(place, block);
	place = 0x15u;
	block = scenery_crash_tile_block_2_ADDRESS;
	draw_tile_block(place, block);
	place = 0x16u;
	block = scenery_crash_tile_block_3_ADDRESS;
	draw_tile_block(place, block);
	place = 0x17u;
	block = scenery_crash_tile_block_4_ADDRESS;
	draw_tile_block(place, block);
	OBJECT(object)->state++;
	animation = enemy_crash_into_scenery_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Unused: the crashed enemy is gone when its animation ends */
void enemy_crash_into_scenery_end(uint32_t object)
{
	uint8_t sound = 0;
	uint8_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		sound = 0x10u;
		queue_sound(sound);
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* Unused: the same crash lower in the scenery */
void enemy_crash_into_scenery_low(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_crash_into_scenery_low_states_ADDRESS;
	run_object_state(object, states);
}

/* Unused: the scenery breaks lower down where the enemy crashes */
void enemy_crash_into_scenery_low_start(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t place = 0;
	uint32_t animation = 0, block = 0;

	sound = 0x40u;
	queue_sound(sound);
	place = 0x18u;
	block = scenery_crash_low_tile_block_1_ADDRESS;
	draw_tile_block(place, block);
	place = 0x19u;
	block = scenery_crash_low_tile_block_2_ADDRESS;
	draw_tile_block(place, block);
	place = 0x1Au;
	block = scenery_crash_tile_block_3_ADDRESS;
	draw_tile_block(place, block);
	place = 0x1Bu;
	block = scenery_crash_tile_block_4_ADDRESS;
	draw_tile_block(place, block);
	OBJECT(object)->state++;
	animation = enemy_crash_into_scenery_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks to its turtle, aiming again every 8 frames */
void enemy_walk_to_target(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk's speed and animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	OBJECT(object)->enemy_walk.animations = enemy_walk_animations_ADDRESS;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk to its turtle, aiming again every 8 frames, standing when close */
void enemy_walk_to_target_step(uint32_t object, uint32_t player_slot)
{
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0, y_speed = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_walk.move_time;
	OBJECT(object)->enemy_walk.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		turtle = PLAYER_SLOT(player_slot)->turtle;
		direction = direction_to_object(0u, object, turtle);
		object_loop_leftover = OBJECT(object)->enemy_walk.animations;
		enemy_turn_to_object((uint8_t)direction, object, turtle);
		speed = OBJECT(object)->enemy_walk.speed;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = y_speed;
		OBJECT(object)->enemy_walk.move_time = 8u;
	}
	if (OBJECT(object)->enemy_walk.target_dx >= 0x20u || OBJECT(object)->enemy_walk.target_dy >= 0x20u)
	{
		move_object_blocked(object);
		return;
	}
}

/* walks to its turtle's side, now and then crouching, and keeps level with it */
void enemy_walk_to_side(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side, facing it */
void enemy_walk_to_side_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk: across to its side or up or down to its turtle's row; level with it, the next
   state */
void enemy_walk_to_side_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_walk.target_dx < 0x20u || (int8_t)(target_direction - 0x40u - OBJECT(object)->enemy_walk.side) < 0)
	{
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
		if ((int8_t)OBJECT(object)->enemy_walk.side < 0)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = OBJECT(object)->enemy_walk.speed;
		if ((int8_t)OBJECT(object)->enemy_walk.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	if (OBJECT(object)->enemy_walk.target_dy < 3u)
		OBJECT(object)->state++;
	move_object_blocked(object);
}

/* one frame in 8 a crouch, else straight on to keeping level */
void enemy_walk_to_side_crouch(uint32_t object)
{
	uint32_t animation = 0;

	if ((frame_count & 7u) != 0u)
	{
		OBJECT(object)->state += 2u;
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, faces its turtle again */
void enemy_walk_to_side_turn(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* level with its turtle: walks across until 0x10 away; off its row again, back to walking to its side */
void enemy_walk_to_side_hold(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_walk.target_dx < 0x10u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0;
		if (OBJECT(object)->enemy_walk.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
	if ((int8_t)(target_direction - 0x40u) < 0)
	{
		if (OBJECT(object)->enemy_walk.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	if (OBJECT(object)->enemy_walk.target_dy >= 3u)
		OBJECT(object)->state -= 3u;
	move_object_blocked(object);
}

/* keeps 0x50 to 0x70 away from its turtle, backing off when nearer and closing in when farther */
void enemy_keep_distance(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_keep_distance_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle, for up to 255 frames of keeping its distance */
void enemy_keep_distance_start(uint32_t object)
{
	uint32_t animations = 0;

	OBJECT(object)->enemy.move_time = 0xFFu;
	OBJECT(object)->state++;
	animations = enemy_run_animations_ADDRESS;
	enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->frame_time = 0u;
}

/* stands facing its turtle; too near it backs off, too far it closes in */
void enemy_keep_distance_watch(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy.move_time--;
	if (OBJECT(object)->enemy.target_direction != OBJECT(object)->enemy.facing)
	{
		animations = enemy_run_animations_ADDRESS;
		enemy_face_target_animation(0u, object, animations);
		OBJECT(object)->frame_time = 0u;
	}
	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
	{
		if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
			return;
		OBJECT(object)->state = 3u;
		direction = OBJECT(object)->enemy.target_direction;
		object_loop_leftover = enemy_walk_animations_ADDRESS;
		turtle = PLAYER_SLOT(player_slot)->turtle;
		enemy_face_object(direction, object, turtle);
		speed = 0x100u;
		speeds_from_direction(direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = y_speed;
		return;
	}
	OBJECT(object)->state = 2u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
}

/* walks away from its turtle until far enough, then watches again */
void enemy_keep_distance_back_off(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks toward its turtle until near enough, then watches again */
void enemy_keep_distance_close_in(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks away from its turtle for 40 frames, crouches, and waits while the turtle is down */
void enemy_back_off(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_back_off_states_ADDRESS;
	run_object_state(object, states);
}

/* 40 frames of walking backwards away from its turtle, still facing it */
void enemy_back_off_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x28u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
void enemy_back_off_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_wait(object, player_slot);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits while its turtle is down; when it is up again, a new move */
void enemy_back_off_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_back_off.approach = 0u;
		OBJECT(object)->enemy_back_off.move_time = 0u;
	}
}

/* runs away from its turtle for 8 frames */
void enemy_run_away(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_run_away_states_ADDRESS;
	run_object_state(object, states);
}

/* 8 frames of running away from its turtle, still facing it */
void enemy_run_away_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy.move_time = 8u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	speed = 0x200u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
	enemy_run_away_step(object, player_slot);
}

/* a frame of running away; at its end, back to its first approach */
void enemy_run_away_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy.approach = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks across toward its turtle for 120 frames */
void enemy_walk_across(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_across_states_ADDRESS;
	run_object_state(object, states);
}

/* 120 frames of walking across toward its turtle, facing it */
void enemy_walk_across_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0x78u;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of walking across, turned to its turtle */
void enemy_walk_across_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = enemy_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* a jab, back to thinking when it ends */
void enemy_jab(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_jab_states_ADDRESS;
	run_object_state(object, states);
}

/* the jab's animation and damage, facing its turtle */
void enemy_jab_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_jab_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x201u;
	attack = 2u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* back to thinking when the attack's animation ends */
void enemy_attack_end(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemy_back_to_thinking(object);
}

/* a chop, back to thinking when it ends */
void enemy_chop(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_chop_states_ADDRESS;
	run_object_state(object, states);
}

/* the chop's animation and damage, facing its turtle */
void enemy_chop_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_chop_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x201u;
	attack = 1u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a swing, back to thinking when it ends */
void enemy_swing(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_swing_states_ADDRESS;
	run_object_state(object, states);
}

/* the swing's animation and damage, facing its turtle */
void enemy_swing_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_swing_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x201u;
	attack = 0u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a high kick, back to thinking when it ends */
void enemy_high_kick(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_high_kick_states_ADDRESS;
	run_object_state(object, states);
}

/* the high kick's animation and damage, facing its turtle */
void enemy_high_kick_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_high_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x201u;
	attack = 2u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a flying kick: a crouch, a leap along its arc and the landing */
void enemy_jump_kick(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_jump_kick_states_ADDRESS;
	run_object_state(object, states);
}

/* crouches for the flying kick, its damage set, facing its turtle */
void enemy_jump_kick_crouch(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x203u;
	attack = 4u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the crouch ends, leaps along the kick's arc */
void enemy_jump_kick_leap(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_jump_kick_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the flying kick along its arc, following stairs; on landing a crouch */
void enemy_jump_kick_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_jump_kick_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a flying kick along a longer arc */
void enemy_long_jump_kick(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_long_jump_kick_states_ADDRESS;
	run_object_state(object, states);
}

/* crouches for the long flying kick, its damage set, facing its turtle */
void enemy_long_jump_kick_crouch(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x203u;
	attack = 4u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the crouch ends, leaps along the long kick's arc */
void enemy_long_jump_kick_leap(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_long_jump_kick_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the long flying kick along its arc, following stairs; on landing a crouch */
void enemy_long_jump_kick_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_long_jump_kick_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* an enemy's attack 6: does nothing */
void enemy_attack_none_after_long_jump_kick(void)
{
}

/* an enemy's attack 7: does nothing */
void enemy_attack_none_before_grab(void)
{
}

/* runs at its turtle with its arms out and holds it, one grab at a time per turtle */
void enemy_grab(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_grab_states_ADDRESS;
	run_object_state(object, states);
}

/* unless its turtle faces it, is down or is being grabbed: runs at it with its arms out for 60 frames */
void enemy_grab_start(uint32_t object, uint32_t player_slot)
{
	uint8_t attack = 0;
	uint16_t speed = 0;
	uint32_t animation = 0, direction = 0, turtle = 0, x_speed = 0, y_speed = 0;

	if ((int8_t)OBJECT(object)->enemy_grab.target_faces < 0 || OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine == 4u || PLAYER_SLOT(player_slot)->grab_time != 0u)
	{
		enemy_back_to_thinking(object);
		return;
	}
	animation = enemy_grab_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x101u;
	attack = 0xEu;
	enemy_face_target(attack, object);
	PLAYER_SLOT(player_slot)->grab_time = 0x60u;
	OBJECT(object)->enemy_grab.time = 0x3Cu;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	direction = direction_to_object(0u, object, turtle);
	speed = 0x200u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
	move_object_blocked(object);
}

/* a frame of the run at its turtle; catching it, it hides in the hold; the turtle down or time up, it
   thinks again */
void enemy_grab_run(uint32_t object)
{
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0, y_speed = 0;
	uint8_t time = 0;

	PLAYER_SLOT(object_loop_slot)->grab_time = 0x60u;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_grab.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		enemy_leave_side(object, 0u);
		OBJECT(object)->flags &= 0x73u;
		OBJECT(object)->state++;
		return;
	}
	if (OBJECT(PLAYER_SLOT(object_loop_slot)->turtle)->routine != 4u)
	{
		time = OBJECT(object)->enemy_grab.time;
		OBJECT(object)->enemy_grab.time = time - 1u;
		if (time == 1u)
		{
			OBJECT(object)->flags &= ~4u;
			enemy_back_to_thinking(object);
			return;
		}
		turtle = PLAYER_SLOT(object_loop_slot)->turtle;
		direction = direction_to_object(0u, object, turtle);
		speed = 0x200u;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = y_speed;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->flags &= ~4u;
	enemy_back_to_thinking(object);
}

/* keeps its turtle held while the turtle struggles; the turtle loose, the next state; the turtle up
   again, the enemy is gone */
void enemy_grab_hold(uint32_t object, uint32_t player_slot)
{
	uint32_t turtle = 0;
	uint8_t t1 = 0;

	PLAYER_SLOT(player_slot)->grab_time = 0x60u;
	turtle = 0x60000u | OBJECT(object)->hit_object;
	if (OBJECT(turtle)->routine == 4u)
	{
		if (OBJECT(turtle)->turtle.hit_reaction == 0xEu || OBJECT(turtle)->turtle.hit_reaction == 0x15u)
		{
			OBJECT(turtle)->turtle.hit_by = object;
			return;
		}
		OBJECT(object)->state++;
		return;
	}
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
	t1 = enemies_on_screen - 1u;
	enemies_on_screen = t1;
	if ((int8_t)t1 < 0)
		enemies_on_screen = 0u;
}

/* lets go: shown again, hittable, and back to thinking */
void enemy_grab_end(uint32_t object)
{
	OBJECT(object)->flags |= 0x82u;
	enemy_back_to_thinking(object);
}

/* hit: a flinch with its head thrown back, then back to thinking */
void enemy_flinch_back(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_states_ADDRESS;
	run_object_state(object, states);
}

/* the flinch with its head thrown back */
void enemy_flinch_back_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the flinch ends, back to thinking, and it can be hit again */
void enemy_flinch_end(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemy_back_to_thinking_hittable(object);
}

/* hit: a flinch bent over, then back to thinking */
void enemy_flinch_bent(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_states_ADDRESS;
	run_object_state(object, states);
}

/* the flinch bent over */
void enemy_flinch_bent_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: a flinch reeling, then back to thinking */
void enemy_flinch_reel(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_reel_states_ADDRESS;
	run_object_state(object, states);
}

/* the flinch reeling */
void enemy_flinch_reel_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_reel_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: a flinch hunched, then back to thinking */
void enemy_flinch_hunched(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_hunched_states_ADDRESS;
	run_object_state(object, states);
}

/* the flinch hunched */
void enemy_flinch_hunched_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_hunched_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* killed: knocked flying on its back, splatting against a wall it meets, then its death */
void enemy_killed_back(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_killed_back_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked flying on its back along its arc, its side by the turtle given up */
void enemy_killed_back_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_killed_back_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_knocked_back_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, 0u);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the flight: against a wall a splat with its sound, else on landing it lies down */
void enemy_killed_back_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0, wall_mask = 0;
	uint32_t animation = 0, arc = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->state = 3u;
		sound = 0x89u;
		queue_sound(sound);
		animation = enemy_spin_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_killed_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop on its back, then its death, one enemy less after its turtle */
void enemy_killed_back_lie(uint32_t object, uint32_t player_slot)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_knocked_back_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy.move = 0x1Cu;
		OBJECT(object)->state = 0u;
		PLAYER_SLOT(player_slot)->enemies--;
	}
}

/* stuck on the wall while the splat lasts */
void enemy_killed_back_on_wall(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
		OBJECT(object)->state++;
}

/* slides down the wall to the floor, then lies there */
void enemy_killed_back_slide_down(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	OBJECT(object)->height -= 0x100u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_killed_back_slide_down_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when it has lain long enough, its death, one enemy less after its turtle */
void enemy_killed_back_down(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy.move = 0x1Cu;
		OBJECT(object)->state = 0u;
		PLAYER_SLOT(player_slot)->enemies--;
	}
}

/* killed: falling on its face, splatting against a wall it meets, then its death */
void enemy_killed_forward(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_killed_forward_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked flying forward along its arc, its side by the turtle given up */
void enemy_killed_forward_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_killed_forward_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_knocked_forward_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, 0u);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the flight: against a wall a splat with its sound, else on landing it lies down */
void enemy_killed_forward_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0, wall_mask = 0;
	uint32_t animation = 0, arc = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->state = 3u;
		sound = 0x89u;
		queue_sound(sound);
		animation = enemy_killed_forward_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_killed_forward_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_forward_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop on its face, then its death, one enemy less after its turtle */
void enemy_killed_forward_lie(uint32_t object, uint32_t player_slot)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_knocked_forward_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->enemy.move = 0x18u;
	OBJECT(object)->state = 0u;
	PLAYER_SLOT(player_slot)->enemies--;
}

/* stuck on the wall while the splat lasts */
void enemy_killed_forward_on_wall(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
		OBJECT(object)->state++;
}

/* slides down the wall to the floor, then lies there */
void enemy_killed_forward_slide_down(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	OBJECT(object)->height -= 0x100u;
	zero = enemy_landed(object);
	if (zero)
	{
		OBJECT(object)->state++;
		OBJECT(object)->contacts &= 0x7Fu;
		animation = enemy_killed_forward_slide_down_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
}

/* when it has lain long enough, its death, one enemy less after its turtle */
void enemy_killed_forward_down(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy.move = 0x18u;
		OBJECT(object)->state = 0u;
		PLAYER_SLOT(player_slot)->enemies--;
	}
}

/* knocked down on its face: lies 30 frames and gets up, or dies with no bar left */
void enemy_knocked_forward(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_knocked_forward_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked down forward along its arc, its side by the turtle given up */
void enemy_knocked_forward_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_knocked_forward_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_knocked_forward_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, 0u);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the fall; on landing it lies down */
void enemy_knocked_forward_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_knocked_forward_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_forward_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop; 30 frames on the ground, or its death with no bar left */
void enemy_knocked_forward_lie(uint32_t object, uint32_t player_slot)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_knocked_forward_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_knocked.down_time = 0x1Eu;
		OBJECT(object)->state++;
		if (OBJECT(object)->health_bars == 0u)
		{
			OBJECT(object)->enemy_knocked.move = 0x18u;
			OBJECT(object)->state = 0u;
			PLAYER_SLOT(player_slot)->enemies--;
		}
	}
}

/* lies on the ground for its time, then gets up */
void enemy_knocked_forward_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t down_time = 0;

	down_time = OBJECT(object)->enemy_knocked.down_time;
	OBJECT(object)->enemy_knocked.down_time = down_time - 1u;
	if (down_time == 1u)
	{
		OBJECT(object)->state++;
		animation = enemy_knocked_forward_lie_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
}

/* thrown by Donatello: hidden while he swings it, then placed by him, hurt and knocked flying on its
   back */
void enemy_thrown_by_donatello(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_thrown_by_donatello_states_ADDRESS;
	run_object_state(object, states);
}

/* hidden in Donatello's hands for the 12 frames of his swing */
void enemy_thrown_by_donatello_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0xCu;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Donatello, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_donatello_swing(uint32_t object)
{
	uint16_t damage = 0;
	uint32_t object_2 = 0, placement = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	object_loop_leftover = thrown_by_donatello_placement_ADDRESS;
	placement = thrown_by_donatello_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	damage = 0x306u;
	enemy_take_damage(damage, object);
	OBJECT(object)->flags |= 0x80u;
	enemy_knocked_back_start(object);
}

/* a frame of the flight after a throw: against a wall a splat with its sound, else on landing it lies
   down */
void enemy_thrown_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0, wall_mask = 0;
	uint32_t animation = 0, arc = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->state = 4u;
		sound = 0x89u;
		queue_sound(sound);
		animation = enemy_spin_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_knocked_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the damage taken off its health, and off its bars when the health runs out, down to none */
void enemy_take_damage(uint16_t damage, uint32_t object)
{
	uint16_t health_bars = 0;
	uint8_t t1 = 0;
	uint16_t t2 = 0;

	health_bars = (((uint32_t)OBJECT(object)->health_bars << 8u) | OBJECT(object)->health);
	t1 = (uint8_t)health_bars;
	health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)(t1 - damage));
	if (t1 <= (uint8_t)damage)
	{
		health_bars = (uint16_t)((health_bars & ~0xFFu) | (uint8_t)damage);
		t2 = health_bars;
		health_bars = (uint16_t)(t2 - damage);
		if (t2 < damage)
			health_bars = 0u;
	}
	OBJECT(object)->health_bars = (uint8_t)(health_bars >> 8u);
	OBJECT(object)->health = (uint8_t)health_bars;
}

/* placed at a record's offsets from another object, mirrored by its facing, at that object's height and
   floor */
void enemy_place_by(uint32_t object, uint32_t placement, uint32_t object_2)
{
	uint16_t dx = 0;

	dx = (uint16_t)(OBJECT(object_2)->screen_x - OBJECT(object)->screen_x + PLACEMENT(placement)->x);
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		dx = (uint16_t)(dx - PLACEMENT(placement)->x - PLACEMENT(placement)->x);
	OBJECT(object)->x += (uint32_t)(int16_t)dx << 8;
	OBJECT(object)->y += (uint32_t)(int16_t)(OBJECT(object_2)->screen_y - OBJECT(object)->screen_y + PLACEMENT(placement)->y) << 8;
	OBJECT(object)->height_whole = OBJECT(object_2)->height_whole + PLACEMENT(placement)->height;
	OBJECT(object)->floor_height = OBJECT(object_2)->floor_height;
}

/* let go by Donatello: placed by him, shown, hittable and back to thinking */
void enemy_let_go_by_donatello(uint32_t object)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, 0u);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	object_loop_leftover = let_go_by_donatello_placement_ADDRESS;
	placement = let_go_by_donatello_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
}

/* thrown by Michelangelo: hidden while he swings it, then placed by him, hurt and knocked flying on its
   back */
void enemy_thrown_by_michelangelo(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_thrown_by_michelangelo_states_ADDRESS;
	run_object_state(object, states);
}

/* hidden in Michelangelo's hands for the 18 frames of his swing */
void enemy_thrown_by_michelangelo_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0x12u;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Michelangelo, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_michelangelo_swing(uint32_t object)
{
	uint16_t damage = 0;
	uint32_t object_2 = 0, placement = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	object_loop_leftover = thrown_by_michelangelo_placement_ADDRESS;
	placement = thrown_by_michelangelo_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	damage = 0x306u;
	enemy_take_damage(damage, object);
	OBJECT(object)->flags |= 0x80u;
	enemy_knocked_back_start(object);
}

/* let go by Michelangelo: placed by him, shown, hittable and back to thinking */
void enemy_let_go_by_michelangelo(uint32_t object)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, 0u);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	object_loop_leftover = let_go_by_michelangelo_placement_ADDRESS;
	placement = let_go_by_michelangelo_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
}

/* thrown by Raphael: hidden while he swings it, then placed by him, hurt and knocked flying on its back */
void enemy_thrown_by_raphael(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_thrown_by_raphael_states_ADDRESS;
	run_object_state(object, states);
}

/* hidden in Raphael's hands for the 16 frames of his swing */
void enemy_thrown_by_raphael_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Raphael, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_raphael_swing(uint32_t object)
{
	uint16_t damage = 0;
	uint32_t object_2 = 0, placement = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	object_loop_leftover = thrown_by_raphael_placement_ADDRESS;
	placement = thrown_by_raphael_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	damage = 0x306u;
	enemy_take_damage(damage, object);
	OBJECT(object)->flags |= 0x80u;
	enemy_knocked_back_start(object);
}

/* let go by Raphael: placed by him, shown, hittable and back to thinking */
void enemy_let_go_by_raphael(uint32_t object)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, 0u);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	object_loop_leftover = let_go_by_raphael_placement_ADDRESS;
	placement = let_go_by_raphael_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
}

/* thrown by Leonardo: hidden while he swings it, then placed by him, hurt and knocked flying on its back */
void enemy_thrown_by_leonardo(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_thrown_by_leonardo_states_ADDRESS;
	run_object_state(object, states);
}

/* hidden in Leonardo's hands for the 20 frames of his swing */
void enemy_thrown_by_leonardo_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0x14u;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Leonardo, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_leonardo_swing(uint32_t object)
{
	uint16_t damage = 0;
	uint32_t object_2 = 0, placement = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	object_loop_leftover = thrown_by_leonardo_placement_ADDRESS;
	placement = thrown_by_leonardo_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	damage = 0x306u;
	enemy_take_damage(damage, object);
	OBJECT(object)->flags |= 0x80u;
	enemy_knocked_back_start(object);
}

/* let go by Leonardo: placed by him, shown, hittable and back to thinking */
void enemy_let_go_by_leonardo(uint32_t object)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, 0u);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	object_loop_leftover = let_go_by_leonardo_placement_ADDRESS;
	placement = let_go_by_leonardo_placement_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
}

/* knocked down on its back: lies 30 frames and gets up, or dies with no bar left */
void enemy_knocked_back(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_knocked_back_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked down backwards along its arc, its side by the turtle given up */
void enemy_knocked_back_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_knocked_back_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_knocked_back_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, 0u);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the fall; on landing it lies down */
void enemy_knocked_back_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_knocked_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop; 30 frames on the ground, or its death with no bar left */
void enemy_knocked_back_lie(uint32_t object, uint32_t player_slot)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_knocked_back_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->enemy_knocked.down_time = 0x1Eu;
	OBJECT(object)->state++;
	if (OBJECT(object)->health_bars == 0u)
	{
		OBJECT(object)->enemy_knocked.move = 0x1Cu;
		OBJECT(object)->state = 0u;
		PLAYER_SLOT(player_slot)->enemies--;
	}
}

/* lies on the ground for its time, then gets up */
void enemy_knocked_back_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t down_time = 0;

	down_time = OBJECT(object)->enemy_knocked.down_time;
	OBJECT(object)->enemy_knocked.down_time = down_time - 1u;
	if (down_time != 1u)
		return;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_lie_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when it is up, back to thinking, hittable, with its health back if it had run out */
void enemy_knocked_back_get_up(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		enemy_back_to_thinking_hittable(object);
		OBJECT(object)->enemy_knocked.approach = 0u;
		if (OBJECT(object)->health == 0u)
			OBJECT(object)->health = 6u;
	}
}

/* dies: its sound, its death animation, and it is gone */
void enemy_die(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_die_states_ADDRESS;
	run_object_state(object, states);
}

/* the death's sound and animation; it can no longer hit or be hit */
void enemy_die_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x10u;
	queue_sound(sound);
	OBJECT(object)->state++;
	OBJECT(object)->flags &= ~6u;
	animation = enemy_die_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the death animation ends, it is gone, one enemy less on the screen */
void enemy_die_vanish(uint32_t object)
{
	uint8_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* dies on its back: its sound, its death animation, and it is gone */
void enemy_die_on_back(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_die_on_back_states_ADDRESS;
	run_object_state(object, states);
}

/* the death on its back: its sound and animation; it can no longer hit or be hit */
void enemy_die_on_back_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x10u;
	queue_sound(sound);
	OBJECT(object)->state++;
	OBJECT(object)->flags &= ~6u;
	animation = enemy_die_on_back_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the closed roll-up door of area 4, with its colours */
void boss_shutter_start(uint32_t object)
{
	uint16_t request = 0;
	uint32_t animation = 0, data = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	request = 0x43Au;
	data = boss_shutter_palette_ADDRESS;
	post_palette_request(request, data);
	animation = boss_shutter_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the scroll reaches the door: the music stops and it rolls up */
void boss_shutter_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (LAYER(LAYER_B)->x < 0x3F000u)
		return;
	OBJECT(object)->boss_shutter.open_time = 0x5Au;
	OBJECT(object)->routine++;
	sound = 0xFEu;
	music = 0xFEu;
	queue_sound(sound);
	sound = 0x36u;
	queue_sound(sound);
	animation = boss_shutter_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the door stays open, then Bebop and Rocksteady are told to come out, with their music */
void boss_shutter_open(uint32_t object)
{
	uint8_t sound = 0;
	uint8_t open_time = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags &= 0x7Fu;
		open_time = OBJECT(object)->boss_shutter.open_time;
		OBJECT(object)->boss_shutter.open_time = open_time - 1u;
		if (open_time == 1u)
		{
			OBJECT(object)->flags = 0u;
			OBJECT(object)->contacts = 0u;
			bosses_come_out = 0xFFu;
			sound = 0x64u;
			music = 0x64u;
			queue_sound(sound);
			return;
		}
	}
}

/* a floor lamp */
void scenery_lamp_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->routine++;
	animation = scenery_lamp_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the scenery stays as it is */
void scenery_stay(void)
{
}

/* an open manhole a turtle can fall into */
void scenery_open_manhole_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->hit_sound = 0xDCu;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->attack = 0xCu;
	OBJECT(object)->damage = 0u;
	OBJECT(object)->routine++;
	animation = scenery_open_manhole_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the scenery is gone once it leaves the screen */
void scenery_gone_off_screen(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a toppled chair */
void scenery_chair_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->routine++;
	animation = scenery_chair_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* flames on the floor, of the size its parameter gives */
void scenery_fire_start(uint32_t object)
{
	uint32_t animation = 0;

	animation = TABLE32(scenery_fire_animations, OBJECT(object)->parameter);
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->depth = 4u;
}

/* the car full of Foot soldiers starts behind the screen's left edge */
void start_foot_car(void)
{
	uint32_t zero = 0;
	uint8_t group = 0;
	uint32_t object = 0;

	group = 2u;
	take_free_object_in_group(group, &object, &zero);
	if (zero)
	{
		OBJECT(object)->flags = 1u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0x3Bu;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->throw = 0u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->x = spawn_x - 0x18000u;
		OBJECT(object)->y = 0x6000u;
	}
}

/* the car drives across, knocking flying the turtles it runs into */
void foot_car_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->attack = 0x87u;
	OBJECT(object)->damage = 0x403u;
	OBJECT(object)->hit_sound = 0xD4u;
	animation = foot_car_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->depth = 0x10u;
	OBJECT(object)->routine++;
	foot_car_drive(object);
}

/* the car drives on until it is past the screen */
void foot_car_drive(uint32_t object)
{
	if ((int16_t)OBJECT(object)->screen_x >= 0x1E0)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	move_object(object);
}

/* a burning piece of the ceiling, hidden until it falls */
void falling_debris_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->attack = 0u;
	OBJECT(object)->damage = 0x103u;
	OBJECT(object)->depth = 3u;
	OBJECT(object)->routine++;
	OBJECT(object)->hit_sound = 0x70u;
	animation = falling_debris_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the debris falls when a turtle comes under it */
void falling_debris_wait(uint32_t object, uint32_t routine_entry)
{
	uint32_t dx = 0, dy = 0;

	if (OBJECT(object)->screen_x >= 0x100u)
	{
		routine_entry = enemy_nearest_turtles(object, routine_entry);
		enemy_look_at_target(object, routine_entry, &dx, &dy);
		if ((uint16_t)dx <= 0x10u && (uint16_t)dy <= 0x10u)
		{
			OBJECT(object)->flags = 0x85u;
			OBJECT(object)->height_speed = -0x100;
			OBJECT(object)->routine++;
		}
		falling_debris_burn(object);
		return;
	}
	OBJECT(object)->flags = 0x85u;
	OBJECT(object)->height_speed = -0x100;
	OBJECT(object)->routine++;
	falling_debris_burn(object);
}

/* the debris falls and burns on the floor */
void falling_debris_fall(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->height += OBJECT(object)->height_speed;
	OBJECT(object)->height_speed -= 0x40u;
	if ((int8_t)OBJECT(object)->height_whole >= 0 || OBJECT(object)->height_whole < 0xF0u)
		return;
	OBJECT(object)->height = 0u;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->routine++;
	sound = 0x31u;
	queue_sound(sound);
	animation = falling_debris_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the burning debris is gone once it leaves the screen */
void falling_debris_burn(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a Foot soldier rides in on a motorbike */
void motorbike_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->damage = 0x206u;
	OBJECT(object)->attack = 7u;
	OBJECT(object)->depth = 6u;
	OBJECT(object)->hit_sound = 0x44u;
	OBJECT(object)->health_bars = 1u;
	OBJECT(object)->health = 1u;
	sound = 0x3Au;
	queue_sound(sound);
	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->x_speed = -0x300;
	OBJECT(object)->y_speed = 0;
	if (OBJECT(object)->parameter != 0u)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->attack |= 0x80u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	animation = motorbike_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	OBJECT(object)->motorbike.aim_time = 1u;
	OBJECT(object)->motorbike.wheelie = 0u;
}

/* the motorbike rides on and rears up at a turtle in its way */
void motorbike_ride(uint32_t object, uint32_t target_turtle)
{
	uint32_t animation = 0, dx = 0, dy = 0;
	uint8_t aim_time = 0;

	aim_time = OBJECT(object)->motorbike.aim_time;
	OBJECT(object)->motorbike.aim_time = aim_time - 1u;
	if (aim_time == 1u)
	{
		OBJECT(object)->motorbike.aim_time = 8u;
		target_turtle = enemy_nearest_turtles(object, target_turtle);
		OBJECT(object)->motorbike.target_turtle = target_turtle;
	}
	target_turtle = OBJECT(object)->motorbike.target_turtle;
	enemy_look_at_target(object, target_turtle, &dx, &dy);
	if ((uint16_t)dy >= 8u || (uint16_t)dx >= 0x40u)
	{
		motorbike_ride_off(object);
		return;
	}
	OBJECT(object)->motorbike.wheelie = 1u;
	animation = motorbike_ride_animation_ADDRESS;
	set_animation(dx, object, animation);
	motorbike_ride_off(object);
}

/* the riderless motorbike rides on off the screen */
void motorbike_ride_off(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	move_object(object);
}

/* the rider is knocked off the motorbike */
void motorbike_struck(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;

	sound = 0xA8u;
	queue_sound(sound);
	animation = motorbike_struck_animation_ADDRESS;
	object_loop_leftover = motorbike_rider_record_ADDRESS;
	if (OBJECT(object)->motorbike.wheelie != 0u)
	{
		animation = motorbike_struck_wheelie_animation_ADDRESS;
		object_loop_leftover = motorbike_wheelie_rider_record_ADDRESS;
	}
	set_animation(0u, object, animation);
	record = object_loop_leftover;
	start_object_from_record(object, record, &object_2, &zero);
	if (zero)
	{
		OBJECT(object_2)->flags = OBJECT(object)->flags;
		OBJECT(object_2)->flags &= 0xC1u;
		OBJECT(object_2)->routine = 5u;
		OBJECT(object_2)->state = 0u;
		OBJECT(object_2)->health_bars = 0u;
		OBJECT(object_2)->health = 0u;
		OBJECT(object_2)->enemy.move = 0x10u;
		OBJECT(object_2)->shadow = common_shadow_ADDRESS;
		enemies_on_screen++;
	}
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
}

/* kind 01, the sword soldier: its turtle, health, colour, definition and shadow, straight to thinking */
void enemy_sword_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x87u;
	OBJECT(object)->colour = 7u;
	OBJECT(object)->floor_height = 0u;
	enemy_set_floor(object);
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = sword_soldier_definition_ADDRESS;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_sword_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* kind 02, the hammer soldier: its turtle, health, colour, definition and shadow, straight to thinking */
void enemy_hammer_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 0u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x85u;
	OBJECT(object)->colour = 5u;
	OBJECT(object)->floor_height = 0u;
	enemy_set_floor(object);
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = hammer_soldier_definition_ADDRESS;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_hammer_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks to its turtle with the sword */
void enemy_walk_to_target_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk with the sword's animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.animations = sword_soldier_walk_animations_ADDRESS;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its side of its turtle with the sword */
void enemy_walk_to_side_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side with the sword, facing it */
void enemy_walk_to_side_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* now and then, by the frame count, a crouch with the sword, else straight on to keeping level */
void enemy_walk_to_side_sword_crouch(uint32_t object)
{
	uint32_t animation = 0;

	if ((frame_count & 0x700u) != 0u)
	{
		OBJECT(object)->state += 2u;
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_sword_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, faces its turtle again */
void enemy_walk_to_side_sword_turn(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* walks across past its turtle while off its row, then up or down to it */
void enemy_walk_past_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_past_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* 180 frames of walking past its turtle with the sword, facing it */
void enemy_walk_past_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xB4u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
		OBJECT(object)->enemy_walk.speed = 0u - OBJECT(object)->enemy_walk.speed;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk: across past its turtle while off its row, turning back beyond 0x40, else up or
   down to it */
void enemy_walk_past_sword_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_walk.target_dy >= 0x20u)
	{
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
		if (OBJECT(object)->enemy_walk.target_dx >= 0x40u)
		{
			OBJECT(object)->enemy_walk.speed = 0x100u;
			if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
				OBJECT(object)->enemy_walk.speed = 0u - OBJECT(object)->enemy_walk.speed;
		}
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0x100;
		if ((int8_t)OBJECT(object)->enemy_walk.target_direction < 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	move_object_blocked(object);
}

/* walks away with the sword, crouches and waits while its turtle is down */
void enemy_back_off_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_back_off_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* 40 frames of walking backwards away from its turtle with the sword, still facing it */
void enemy_back_off_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x28u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
void enemy_back_off_sword_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_sword_wait(object, player_slot);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_sword_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits while its turtle is down; when it is up again, a new move */
void enemy_back_off_sword_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_back_off.approach = 0u;
		OBJECT(object)->enemy_back_off.move_time = 0u;
	}
}

/* walks up or down the screen toward its turtle's row for 16 frames */
void enemy_walk_to_line_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_line_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* 16 frames of walking up or down toward its turtle's row, facing it */
void enemy_walk_to_line_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0x10u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x100;
	if ((int8_t)OBJECT(object)->enemy_walk.target_direction < 0)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	OBJECT(object)->state++;
	move_object_blocked(object);
}

/* a frame of the walk toward its turtle's row; at its end, back to its first approach */
void enemy_walk_to_line_sword_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;
	uint8_t move_time = 0;

	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = sword_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	move_time = OBJECT(object)->enemy_walk.move_time;
	OBJECT(object)->enemy_walk.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy_walk.approach = 0u;
}

/* walks to its turtle with the hammer */
void enemy_walk_to_target_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk with the hammer's animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.animations = hammer_soldier_walk_animations_ADDRESS;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its side of its turtle with the hammer, stopping 0x40 across */
void enemy_walk_to_side_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side with the hammer, facing it */
void enemy_walk_to_side_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = hammer_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk: across to 0x40 by its turtle or up or down to its row; level with it, the next
   state */
void enemy_walk_to_side_far_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_walk.target_dx < 0x40u || (int8_t)(target_direction - 0x40u - OBJECT(object)->enemy_walk.side) < 0)
	{
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
		if ((int8_t)OBJECT(object)->enemy_walk.side < 0)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = OBJECT(object)->enemy_walk.speed;
		if ((int8_t)OBJECT(object)->enemy_walk.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	if (OBJECT(object)->enemy_walk.target_dy < 3u)
		OBJECT(object)->state++;
	move_object_blocked(object);
}

/* level with its turtle: walks across toward it; off its row again, back to walking to its side */
void enemy_walk_to_side_far_hold(uint32_t object)
{
	uint32_t player_slot = 0;

	player_slot = object_loop_slot;
	enemy_walk_to_side_hold(object, player_slot);
	if ((int8_t)OBJECT(object)->state < 0)
		OBJECT(object)->state = 1u;
}

/* the hammer soldier closes in across, then waits with the hammer up and steps to its turtle's row by
   turns */
void enemy_stalk_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_stalk_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* up to 255 frames of stalking its turtle with the hammer, facing it */
void enemy_stalk_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_stalk.move_time = 0xFFu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_stalk.turns = 0u;
	OBJECT(object)->enemy_stalk.time = 1u;
	target_direction = OBJECT(object)->enemy_stalk.target_direction;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* walks across toward its turtle until 0x40 away */
void enemy_stalk_hammer_approach(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	target_direction = OBJECT(object)->enemy_stalk.target_direction;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_stalk.target_dx < 0x40u)
		OBJECT(object)->state++;
	OBJECT(object)->enemy_stalk.time = 1u;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_stalk.target_direction - 0x40u) >= 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	move_object_blocked(object);
}

/* by turns of 48 frames, stands with the hammer up or steps toward its turtle's row; too far, it closes
   in again */
void enemy_stalk_hammer_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t time = 0;

	if (OBJECT(object)->enemy_stalk.target_dx >= 0x50u)
	{
		OBJECT(object)->state--;
		if ((OBJECT(object)->enemy_stalk.turns & 1u) == 0u)
		{
			target_direction = OBJECT(object)->enemy_stalk.target_direction;
			object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
			turtle = PLAYER_SLOT(player_slot)->turtle;
			enemy_face_object(target_direction, object, turtle);
			return;
		}
		return;
	}
	time = OBJECT(object)->enemy_stalk.time;
	OBJECT(object)->enemy_stalk.time = time - 1u;
	if (time != 1u)
	{
		if ((OBJECT(object)->enemy_stalk.turns & 1u) != 0u)
		{
			target_direction = OBJECT(object)->enemy_stalk.target_direction;
			object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
			turtle = PLAYER_SLOT(player_slot)->turtle;
			enemy_turn_to_object(target_direction, object, turtle);
		}
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->enemy_stalk.time = 0x30u;
	OBJECT(object)->enemy_stalk.turns++;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->enemy_stalk.turns & 1u) == 0u)
	{
		animation = enemy_hammer_walk_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->y_speed = 0x100;
	if ((int8_t)OBJECT(object)->enemy_stalk.target_direction < 0)
	{
		target_direction = OBJECT(object)->enemy_stalk.target_direction;
		object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
		turtle = PLAYER_SLOT(player_slot)->turtle;
		enemy_face_object(target_direction, object, turtle);
		return;
	}
	OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	target_direction = OBJECT(object)->enemy_stalk.target_direction;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* walks away for 64 frames, then waits with the hammer up while its turtle is down */
void enemy_walk_away_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_away_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* 64 frames of walking away from its turtle across, facing it */
void enemy_walk_away_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0x40u;
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) < 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk away; at its end, the hammer up */
void enemy_walk_away_hammer_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t move_time = 0;

	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = hammer_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	move_time = OBJECT(object)->enemy_walk.move_time;
	OBJECT(object)->enemy_walk.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->enemy_walk.move_time = 0xFFu;
	OBJECT(object)->state++;
	animation = enemy_hammer_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* with the hammer up while its turtle is down; when it is up again, a new move */
void enemy_walk_away_hammer_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->frame_time == 0u && OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_walk.move_time = 0u;
		OBJECT(object)->enemy_walk.approach = 0u;
	}
}

/* a slash with the sword */
void enemy_slash_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_slash_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the slash's animation and damage, facing its turtle */
void enemy_slash_sword_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_slash_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x302u;
	attack = 0u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a low slash with the sword */
void enemy_low_slash_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_low_slash_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the low slash's animation and damage, facing its turtle */
void enemy_low_slash_sword_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_low_slash_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x302u;
	attack = 1u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a leap at its turtle, as far as the turtle is, slashing */
void enemy_leaping_slash_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_leaping_slash_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* readies the leaping slash: its animation and damage, facing its turtle */
void enemy_leaping_slash_sword_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_leaping_slash_sword_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x403u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the pose ends, leaps along its arc, faster across the farther its turtle is */
void enemy_leaping_slash_sword_leap(uint32_t object)
{
	uint16_t distance = 0;
	uint32_t animation = 0, arc = 0, x_speed = 0;
	uint16_t target_dx = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_leaping_slash_sword_arc_ADDRESS;
	enemy_start_arc(object, arc);
	target_dx = OBJECT(object)->enemy.target_dx;
	distance = (uint16_t)(target_dx - 0x10u);
	if (target_dx < 0x10u)
		distance = 0u;
	x_speed = 6u * distance;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		x_speed = (uint16_t)(0u - x_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_leaping_slash_sword_leap_animation_ADDRESS;
	set_animation(x_speed, object, animation);
}

/* a frame of the leap along its arc, following stairs; on landing its last pose */
void enemy_leaping_slash_sword_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_leaping_slash_sword_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_leaping_slash_sword_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a jump slashing with the sword */
void enemy_jump_slash_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_jump_slash_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* readies the jumping slash: the sword up and its damage, facing its turtle */
void enemy_jump_slash_sword_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_sword_ready_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x303u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the pose ends, jumps along its arc slashing */
void enemy_jump_slash_sword_leap(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_jump_slash_sword_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_slash_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump along its arc, following stairs; on landing the sword up again */
void enemy_jump_slash_sword_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_jump_slash_sword_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_sword_ready_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a long leap slashing, turning over in the air */
void enemy_flying_slash_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flying_slash_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* readies the flying slash: the sword up and its damage, facing its turtle */
void enemy_flying_slash_sword_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_sword_ready_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x503u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the pose ends, leaps along its arc, faster across the farther its turtle is */
void enemy_flying_slash_sword_leap(uint32_t object)
{
	uint16_t distance = 0;
	uint32_t animation = 0, arc = 0, x_speed = 0;
	uint16_t target_dx = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_flying_slash_sword_arc_ADDRESS;
	enemy_start_arc(object, arc);
	target_dx = OBJECT(object)->enemy_flying_slash_sword.target_dx;
	distance = (uint16_t)(target_dx - 0x10u);
	if (target_dx < 0x10u)
		distance = 0u;
	x_speed = (0x2AAu * distance) >> 8;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		x_speed = (uint16_t)(0u - x_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->enemy_flying_slash_sword.turn_time = 0x48u;
	OBJECT(object)->state++;
	animation = enemy_flying_slash_sword_leap_animation_ADDRESS;
	set_animation(x_speed, object, animation);
}

/* a frame of the leap, turning over after 72 frames, following stairs; on landing a crouch */
void enemy_flying_slash_sword_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0, ignored = 0;
	uint8_t turn_time = 0;

	turn_time = OBJECT(object)->enemy_flying_slash_sword.turn_time;
	OBJECT(object)->enemy_flying_slash_sword.turn_time = turn_time - 1u;
	if (turn_time == 1u)
	{
		animation = enemy_flying_slash_sword_fly_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_flying_slash_sword_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_sword_crouch_walk_animation_ADDRESS;
	set_animation(ignored, object, animation);
}

/* a blow with the hammer, 58 frames */
void enemy_smash_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_smash_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* the blow's animation, damage and sound, facing its turtle */
void enemy_smash_hammer_start(uint32_t object)
{
	uint8_t attack = 0, sound = 0;
	uint32_t animation = 0;

	animation = enemy_smash_hammer_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x303u;
	attack = 7u;
	enemy_face_target(attack, object);
	sound = 8u;
	queue_sound(sound);
	OBJECT(object)->enemy_smash_hammer.time = 0x3Au;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* back to thinking when the blow's 58 frames are over */
void enemy_smash_hammer_end(uint32_t object)
{
	uint8_t time = 0;

	time = OBJECT(object)->enemy_smash_hammer.time;
	OBJECT(object)->enemy_smash_hammer.time = time - 1u;
	if (time != 1u)
		return;
	enemy_back_to_thinking(object);
}

/* hit: the sword soldier flinches with its head thrown back */
void enemy_flinch_back_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the sword soldier's flinch with its head thrown back */
void enemy_flinch_back_sword_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the sword soldier flinches bent over */
void enemy_flinch_bent_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the sword soldier's flinch bent over */
void enemy_flinch_bent_sword_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the sword soldier flinches reeling */
void enemy_flinch_reel_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_reel_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the sword soldier's flinch reeling */
void enemy_flinch_reel_sword_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_reel_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the sword soldier flinches hunched */
void enemy_flinch_hunched_sword(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_hunched_sword_states_ADDRESS;
	run_object_state(object, states);
}

/* the sword soldier's flinch hunched */
void enemy_flinch_hunched_sword_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_hunched_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the hammer soldier flinches with its head thrown back */
void enemy_flinch_back_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* the hammer soldier's flinch with its head thrown back */
void enemy_flinch_back_hammer_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_hammer_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the hammer soldier flinches bent over */
void enemy_flinch_bent_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* the hammer soldier's flinch bent over */
void enemy_flinch_bent_hammer_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_hammer_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit hunched: the hammer soldier runs the same flinch as with its head thrown back */
void enemy_flinch_hunched_hammer(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_hammer_states_ADDRESS;
	run_object_state(object, states);
}

/* Unused: the hammer soldier's flinch hunched over starts as the bent one does: the next state and the
   same animation */
void enemy_flinch_hunched_hammer_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_hammer_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Rodney's whip arm, whose grab holds a turtle */
void whip_arm_start(uint16_t routine_offset, uint32_t object)
{
	OBJECT(object)->damage = 0u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->attack = 0x8Fu;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		OBJECT(object)->attack &= 0x7Fu;
	OBJECT(object)->routine++;
	OBJECT(object)->whip_arm.unused_cleared = 0u;
	whip_arm_follow(routine_offset, object);
}

/* the arm reaches out, is drawn in or put away as Rodney does it */
void whip_arm_follow(uint32_t routine_offset, uint32_t object)
{
	uint32_t animation_entry = 0, animations = 0, offsets = 0, rodney = 0;

	rodney = OBJECT(object)->whip_arm.rodney;
	if ((OBJECT(rodney)->enemy_rodney_whip.arm_flags & 0x80u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		object_loop_slot = rodney;
		return;
	}
	if ((OBJECT(rodney)->enemy_rodney_whip.arm_flags & 0x40u) != 0u)
	{
		OBJECT(object)->flags &= ~0xEu;
		offsets = whip_arm_x_offsets_ADDRESS;
		routine_offset = whip_arm_place(routine_offset, object, rodney, offsets);
		animations = whip_arm_follow_animations_ADDRESS;
		animation_entry = ANIMATION_ENTRY(animations + (IN_PAGE(routine_offset, OBJECT(rodney)->enemy_rodney_whip.arm_length) & 0x3FFFu) * 0x4u);
		object_loop_slot = rodney;
		set_animation(0u, object, animation_entry);
		return;
	}
	if ((OBJECT(rodney)->enemy_rodney_whip.arm_flags & 0x20u) == 0u)
	{
		offsets = whip_arm_x_offsets_ADDRESS;
		routine_offset = whip_arm_place(routine_offset, object, rodney, offsets);
		animations = whip_arm_follow_animations_ADDRESS;
	}
	else
	{
		OBJECT(object)->routine = 3u;
		OBJECT(object)->flags &= ~0xEu;
		offsets = whip_arm_x_offsets_2_ADDRESS;
		routine_offset = whip_arm_place(routine_offset, object, rodney, offsets);
		animations = whip_arm_animations_2_ADDRESS;
	}
	animation_entry = ANIMATION_ENTRY(animations + (IN_PAGE(routine_offset, OBJECT(rodney)->enemy_rodney_whip.arm_length) & 0x3FFFu) * 0x4u);
	object_loop_slot = rodney;
	set_animation(0u, object, animation_entry);
}

/* the turtle broke free of the arm */
void whip_arm_let_go(uint32_t object)
{
	uint32_t rodney = 0;

	rodney = OBJECT(object)->whip_arm.rodney;
	OBJECT(rodney)->enemy_rodney_whip.arm_flags |= 4u;
	OBJECT(object)->routine = 1u;
	whip_arm_drag(object);
}

/* the arm drags the turtle it caught */
void whip_arm_drag(uint32_t object)
{
	uint32_t hit_turtle = 0, rodney = 0, screen_y = 0, turtle = 0, x_offset = 0;

	rodney = OBJECT(object)->whip_arm.rodney;
	hit_turtle = 0x60000u | OBJECT(object)->hit_object;
	turtle = hit_turtle;
	if (OBJECT(rodney)->routine == 5u)
	{
		OBJECT(turtle)->turtle.escape_presses = 0x10u;
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		offset_high_word = hit_turtle;
		object_loop_slot = rodney;
		return;
	}
	screen_y = OBJECT(object)->screen_y;
	OBJECT(turtle)->y = (uint32_t)(uint16_t)(screen_y - 2u) << 8;
	x_offset = (uint16_t)whip_arm_offsets[OBJECT(rodney)->enemy_rodney_whip.arm_length];
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		x_offset = (uint16_t)(0u - x_offset);
	OBJECT(turtle)->x = (uint32_t)(uint16_t)(x_offset + OBJECT(object)->screen_x) << 8;
	offset_high_word = hit_turtle;
	object_loop_slot = rodney;
}

/* a turtle's attack hits the arm, which lets go of a turtle it holds */
void whip_arm_struck(uint32_t routine_offset, uint32_t object)
{
	uint32_t animation = 0, offsets = 0, rodney = 0, turtle = 0;

	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		offset_high_word = OBJECT_ADDRESS(0);
		turtle = 0x60000u | OBJECT(object)->hit_object;
		OBJECT(turtle)->turtle.escape_presses = 0x10u;
		rodney = OBJECT(object)->whip_arm.rodney;
		OBJECT(rodney)->enemy_rodney_whip.arm_flags |= 4u;
	}
	OBJECT(object)->flags &= ~0xEu;
	rodney = OBJECT(object)->whip_arm.rodney;
	OBJECT(rodney)->enemy_rodney_whip.arm_flags |= 2u;
	OBJECT(object)->routine = 1u;
	offsets = whip_arm_x_offsets_ADDRESS;
	routine_offset = whip_arm_place(routine_offset, object, rodney, offsets);
	animation = whip_arm_follow_animations[IN_PAGE(routine_offset, OBJECT(rodney)->enemy_rodney_whip.arm_length) & 0x3FFFu];
	object_loop_slot = rodney;
	set_animation(0u, object, animation);
}

/* the arm at Rodney's side, as long as Rodney stretches it */
uint32_t whip_arm_place(uint32_t routine_offset, uint32_t object, uint32_t rodney, uint32_t offsets)
{
	uint16_t x_offset = 0;
	uint32_t step_offset = 0, turtle = 0;

	if (OBJECT(rodney)->routine != 5u)
	{
		step_offset = OBJECT(rodney)->enemy_rodney_whip.arm_length;
		step_offset = (uint16_t)(step_offset << 1);
		x_offset = (uint16_t)WHIP_OFFSET(offsets + ((int32_t)(step_offset / 2u)) * 0x2u);
		if ((OBJECT(object)->flags & 0x40u) != 0u)
			x_offset = (uint16_t)(0u - x_offset);
		OBJECT(object)->x = (int16_t)x_offset + OBJECT(rodney)->x;
		OBJECT(object)->y = OBJECT(rodney)->y;
		return step_offset;
	}
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		offset_high_word = OBJECT_ADDRESS(0);
		turtle = 0x60000u | OBJECT(object)->hit_object;
		OBJECT(turtle)->turtle.escape_presses = 0x10u;
	}
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
	return routine_offset;
}

/* a laser bolt flies the way Rodney faces */
void laser_bolt_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->depth = 3u;
	OBJECT(object)->flags |= 0x85u;
	OBJECT(object)->damage = 0x201u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x800;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->routine++;
	animation = laser_bolt_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the bolt flies until it hits a turtle or leaves the screen */
void laser_bolt_fly(uint32_t object)
{
	uint32_t animation = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->routine++;
		animation = laser_bolt_fly_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	move_object(object);
}

/* the bolt's burst ends and it is gone */
void laser_bolt_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a thrown manhole cover rolls at the turtles, knocking them flying */
void rolling_manhole_cover_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->damage = 0x306u;
	OBJECT(object)->depth = 6u;
	OBJECT(object)->hit_sound = 0x44u;
	OBJECT(object)->attack = 0x87u;
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)OBJECT(object)->parameter >= 0)
	{
		OBJECT(object)->attack &= 0x7Fu;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->rolling.hit_delay = 0xAu;
	OBJECT(object)->routine++;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	animation = rolling_manhole_cover_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* it rolls, and a turtle can hit it after a few frames */
void rolling_wait_hittable(uint32_t object)
{
	uint8_t hit_delay = 0;

	hit_delay = OBJECT(object)->rolling.hit_delay;
	OBJECT(object)->rolling.hit_delay = hit_delay - 1u;
	if (hit_delay != 1u)
	{
		rolling_roll(object);
		return;
	}
	OBJECT(object)->routine++;
	OBJECT(object)->flags |= 2u;
	rolling_roll(object);
}

/* it rolls on until it leaves the screen */
void rolling_roll(uint32_t object)
{
	uint8_t t1 = 0;

	if ((OBJECT(object)->flags & 0x20u) == 0u)
	{
		move_object(object);
		return;
	}
	t1 = enemies_on_screen - 1u;
	enemies_on_screen = t1;
	if ((int8_t)t1 < 0)
		enemies_on_screen = 0u;
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
	move_object(object);
}

/* a turtle's attack sends it rolling back the other way */
void rolling_struck(uint32_t object)
{
	uint8_t sound = 0;

	sound = 0x25u;
	if (OBJECT(object)->kind != 0x19u)
		sound = 0x41u;
	queue_sound(sound);
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->rolling.hit_delay = 0xAu;
	OBJECT(object)->attack ^= 0x80u;
	OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->routine = 1u;
}

/* a thrown knife flies the way its thrower faces; on the skateboards it is carried along with the ride */
void knife_start(uint32_t object)
{
	uint32_t animation = 0;

	if (area == 6u)
		OBJECT(object)->x += 0x600u;
	OBJECT(object)->damage = 0x202u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->routine++;
	animation = knife_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the knife flies until it hits a turtle or leaves the screen */
void knife_fly(uint32_t object)
{
	uint32_t animation = 0;

	if (area == 6u)
		OBJECT(object)->x += 0x600u;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->flags &= ~0xEu;
		OBJECT(object)->routine = 2u;
		animation = knife_struck_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	move_object(object);
}

/* the knife's burst ends and it is gone */
void knife_vanish(uint32_t object)
{
	if (area == 6u)
		OBJECT(object)->x += 0x600u;
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a turtle's attack knocks the knife away */
void knife_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x25u;
	queue_sound(sound);
	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->routine = 2u;
	animation = knife_struck_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a thrown spear flies the way its thrower faces */
void spear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->damage = 0x303u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x600;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->routine++;
	animation = spear_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the spear flies through what it hits until it leaves the screen */
void spear_fly(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	move_object(object);
}

/* the broken spear is gone */
void spear_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a turtle's attack breaks the spear */
void spear_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x25u;
	queue_sound(sound);
	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->routine = 2u;
	animation = spear_struck_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a thrown axe flies spinning the way its thrower faces; it cannot be hit */
void axe_start(uint32_t object)
{
	uint32_t animation = 0;

	if (area == 6u)
		OBJECT(object)->x += 0x600u;
	OBJECT(object)->damage = 0x301u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->routine++;
	animation = axe_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the axe flies through what it hits until it leaves the screen */
void axe_fly(uint32_t object)
{
	if (area == 6u)
		OBJECT(object)->x += 0x600u;
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	move_object(object);
}

/* a gun shot flies the way the soldier faces, dropping as it goes */
void gun_shot_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->damage = 0x301u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->flags |= 0x85u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x600;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->height_speed = -0x200;
	OBJECT(object)->routine++;
	animation = gun_shot_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	if (area == 6u)
		OBJECT(object)->x += 0x600u;
}

/* the shot flies until it hits a turtle, the ground, or leaves the screen */
void gun_shot_fly(uint32_t object)
{
	uint32_t animation = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		if (area == 6u)
			OBJECT(object)->x += 0x600u;
		return;
	}
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->flags &= ~0xEu;
		OBJECT(object)->routine = 2u;
		animation = gun_shot_burst_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	if (OBJECT(object)->height_whole == 0u)
	{
		OBJECT(object)->flags &= ~0xEu;
		OBJECT(object)->routine = 2u;
		animation = gun_shot_struck_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	move_object_with_height(object);
	if (area == 6u)
		OBJECT(object)->x += 0x600u;
}

/* the shot's burst ends and it is gone */
void gun_shot_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	if (area == 6u)
		OBJECT(object)->x += 0x600u;
}

/* the shot bursts; a turtle's attack can never strike it */
void gun_shot_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x25u;
	queue_sound(sound);
	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->routine = 2u;
	animation = gun_shot_burst_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a thrown boomerang flies out */
void boomerang_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->damage = 0x201u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x580;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->boomerang.flight_time = 0x20u;
	OBJECT(object)->routine++;
	animation = boomerang_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the boomerang flies out a while, then turns back */
void boomerang_fly_out(uint32_t object)
{
	uint32_t thrower = 0;
	uint8_t flight_time = 0;

	flight_time = OBJECT(object)->boomerang.flight_time;
	OBJECT(object)->boomerang.flight_time = flight_time - 1u;
	if (flight_time == 1u)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		OBJECT(object)->attack ^= 0x80u;
		OBJECT(object)->boomerang.flight_time = 0x20u;
		OBJECT(object)->routine++;
	}
	move_object(object);
	thrower = OBJECT(object)->boomerang.thrower;
	if (OBJECT(thrower)->routine == 5u)
		OBJECT(object)->routine = 3u;
}

/* the boomerang flies back a while and is gone */
void boomerang_fly_back(uint32_t object)
{
	uint32_t thrower = 0;
	uint8_t flight_time = 0;

	thrower = OBJECT(object)->boomerang.thrower;
	if (OBJECT(thrower)->routine != 5u)
	{
		flight_time = OBJECT(object)->boomerang.flight_time;
		OBJECT(object)->boomerang.flight_time = flight_time - 1u;
		if (flight_time == 1u)
		{
			OBJECT(object)->flags = 0u;
			OBJECT(object)->contacts = 0u;
		}
	}
	else
		OBJECT(object)->routine++;
	move_object(object);
}

/* the boomerang's thrower was hit: it flies on off the screen */
void boomerang_fly_on(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	move_object(object);
}

/* a turtle's attack breaks the boomerang */
void boomerang_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->routine++;
	sound = 0x25u;
	queue_sound(sound);
	animation = boomerang_struck_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the broken boomerang is gone */
void boomerang_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a thrown shuriken flies the way its thrower faces */
void shuriken_start(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t flags = 0;

	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->damage = 0x301u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x500;
	OBJECT(object)->y_speed = 0;
	flags = OBJECT(object)->flags;
	OBJECT(object)->flags = flags & 0xBFu;
	if ((flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->routine++;
	animation = shuriken_spin_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the shuriken flies until it hits a turtle or leaves the screen */
void shuriken_fly(uint32_t object)
{
	uint32_t animation = 0;

	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 8u) == 0u)
	{
		move_object(object);
		return;
	}
	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->routine++;
	animation = shuriken_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Unused: a shuriken struck: it no longer hits or can be hit, with its struck animation */
void shuriken_struck_without_sound(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->routine++;
	animation = shuriken_struck_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the shuriken's burst ends and it is gone */
void shuriken_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a turtle's attack knocks the shuriken away */
void shuriken_struck(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	OBJECT(object)->routine--;
	sound = 0x25u;
	queue_sound(sound);
	animation = shuriken_struck_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Rocksteady's bullet flies level, or up at a turtle in the air */
void rocksteady_bullet_start(uint32_t object)
{
	uint8_t aim = 0;
	uint16_t aim_number = 0;
	uint32_t aims = 0, animation = 0, speed = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->damage = 0x101u;
	OBJECT(object)->depth = 2u;
	OBJECT(object)->flags |= 0x85u;
	OBJECT(object)->routine++;
	aim_number = OBJECT(object)->parameter;
	aims = rocksteady_bullet_aims_2_ADDRESS;
	OBJECT(object)->attack = 0x80u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		aims = rocksteady_bullet_aims_ADDRESS;
		OBJECT(object)->attack = 0u;
	}
	aim = AIM(aims + aim_number);
	speed = 0x600u;
	speeds_from_direction(aim, (uint16_t)speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->height_speed = y_speed;
	OBJECT(object)->y_speed = 0;
	aim_number = (uint16_t)((uint32_t)aim_number << 3);
	animation = rocksteady_bullet_animations[aim_number / 4u];
	OBJECT(object)->rocksteady_bullet.burst = TABLE32(rocksteady_bullet_animations, 0x4u + aim_number);
	distance_low_byte = aim_number;
	object_loop_leftover = rocksteady_bullet_animations_ADDRESS;
	set_animation(speed, object, animation);
}

/* the bullet flies until it hits a turtle or leaves the screen */
void rocksteady_bullet_fly(uint32_t object)
{
	uint32_t burst = 0;

	if (OBJECT(object)->height_whole >= 0xF8u || (OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		OBJECT(object)->flags &= ~0xEu;
		OBJECT(object)->routine = 2u;
		burst = OBJECT(object)->rocksteady_bullet.burst;
		set_animation(0u, object, burst);
		return;
	}
	move_object_with_height(object);
}

/* the bullet's burst ends and it is gone */
void rocksteady_bullet_vanish(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* Rocksteady's bullet struck: it bursts with a sound, no longer hitting or hittable; never in the game,
   as nothing can strike the bullet */
void rocksteady_bullet_burst(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t burst = 0;

	sound = 0x25u;
	queue_sound(sound);
	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->routine = 2u;
	burst = OBJECT(object)->rocksteady_bullet.burst;
	set_animation(0u, object, burst);
}

/* Bebop's shot flies the way he faces */
void bebop_shot_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->damage = 0x401u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->routine++;
	animation = bebop_shot_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the shot flies through what it hits until it leaves the screen */
void bebop_shot_fly(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		return;
	}
	move_object(object);
}

/* a thrown tire rolls at the turtles, knocking them flying */
void rolling_tire_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->damage = 0x606u;
	OBJECT(object)->depth = 0x12u;
	OBJECT(object)->hit_sound = 0x44u;
	OBJECT(object)->attack = 0x80u;
	OBJECT(object)->x_speed = 0x400;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)OBJECT(object)->parameter >= 0)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->rolling.hit_delay = 0xAu;
	OBJECT(object)->routine++;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	animation = rolling_tire_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a piece of Shredder flies off */
void shredder_piece_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->shredder_piece.landings = 0xFFu;
	OBJECT(object)->x_speed = 0x300;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0x100;
	OBJECT(object)->routine++;
	animation = shredder_piece_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the piece bounces lower and lower until it leaves the screen */
void shredder_piece_bounce(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;
	uint8_t t1 = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero)
	{
		OBJECT(object)->height_speed = (int16_t)(0u - OBJECT(object)->height_speed) >> 1;
		t1 = OBJECT(object)->shredder_piece.landings + 1u;
		OBJECT(object)->shredder_piece.landings = t1;
		if (t1 == 0u)
		{
			animation = shredder_piece_bounce_animation_ADDRESS;
			set_animation(0u, object, animation);
		}
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* Shredder's ray */
void shredder_ray_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->damage = 0xA03u;
	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->attack = 0x98u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		OBJECT(object)->attack &= 0x7Fu;
	OBJECT(object)->shredder_ray.time_left = 0x3Cu;
	OBJECT(object)->routine++;
	animation = shredder_ray_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the ray lasts a second, less if Shredder is hit */
void shredder_ray_shine(uint32_t object)
{
	uint32_t shredder = 0;
	uint8_t time_left = 0;

	shredder = OBJECT(object)->shredder_ray.shredder;
	if (OBJECT(shredder)->routine != 5u)
	{
		time_left = OBJECT(object)->shredder_ray.time_left;
		OBJECT(object)->shredder_ray.time_left = time_left - 1u;
		if (time_left == 1u)
		{
			OBJECT(object)->flags = 0u;
			OBJECT(object)->contacts = 0u;
		}
		return;
	}
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
}

/* a rocket is fired the way the soldier faces, or dropped when the soldier is hit */
void rocket_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x84u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->damage = 0x102u;
	OBJECT(object)->depth = 3u;
	OBJECT(object)->hit_sound = 0x70u;
	OBJECT(object)->x_speed = 0x400;
	if (OBJECT(object)->parameter != 0u)
		OBJECT(object)->x_speed = 0;
	OBJECT(object)->attack = 0x80u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->attack = 0u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0x200;
	OBJECT(object)->routine++;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	animation = rocket_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the rocket comes down and explodes */
void rocket_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x80u;
	zero = enemy_landed(object);
	if (zero)
	{
		sound = 0x13u;
		queue_sound(sound);
		animation = rocket_animations[0];
		set_animation(0u, object, animation);
		OBJECT(object)->damage = 0x303u;
		OBJECT(object)->attack |= 7u;
		OBJECT(object)->routine++;
	}
}

/* the explosion ends and the rocket is gone */
void rocket_explode(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a stick of dynamite with its fuse burning */
void dynamite_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->damage = 0x206u;
	OBJECT(object)->attack = 7u;
	OBJECT(object)->flags |= 0x80u;
	animation = dynamite_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->hit_sound = 0x70u;
	OBJECT(object)->dynamite.bounces_left = 3u;
	OBJECT(object)->depth = 0u;
	OBJECT(object)->routine++;
}

/* the dynamite is thrown at the nearest turtle */
void dynamite_throw(uint32_t object, uint32_t routine_entry)
{
	uint16_t speed = 0;
	uint32_t direction = 0, x_speed = 0, y_speed = 0;

	routine_entry = enemy_nearest_turtles(object, routine_entry);
	direction = direction_to_object(0u, object, routine_entry);
	speed = 0x100u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->routine++;
	dynamite_bounce(object);
}

/* the dynamite bounces, flashing, and explodes on its last bounce */
void dynamite_bounce(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint8_t bounces_left = 0;

	OBJECT(object)->colour = dynamite_flash_colours[(uint8_t)frame_count & 4u];
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero)
	{
		OBJECT(object)->height_speed = (int16_t)(0u - OBJECT(object)->height_speed) >> 1;
		bounces_left = OBJECT(object)->dynamite.bounces_left;
		OBJECT(object)->dynamite.bounces_left = bounces_left - 1u;
		if (bounces_left == 1u)
		{
			sound = 0x11u;
			queue_sound(sound);
			animation = dynamite_bounce_animation_ADDRESS;
			set_animation(0u, object, animation);
			OBJECT(object)->colour = 0xFFu;
			OBJECT(object)->flags |= 4u;
			OBJECT(object)->routine++;
		}
	}
}

/* the explosion ends and the dynamite is gone */
void dynamite_explode(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* a closed door */
void kicked_door_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->routine++;
	animation = kicked_door_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* on the screen, a Foot soldier kicks the door down */
void kicked_door_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (OBJECT(object)->screen_x >= 0x100u)
		return;
	OBJECT(object)->routine++;
	sound = 0xDu;
	queue_sound(sound);
	animation = kicked_door_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the door's pieces fly off */
void kicked_door_break(uint32_t object)
{
	uint32_t zero = 0;
	uint16_t count = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint16_t t1 = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	count = 3u;
	record = kicked_door_pieces_ADDRESS;
	do
	{
		start_object_from_record(object, record, &ignored, &zero);
		t1 = count - 1u;
		count = t1;
		record += 8u;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->routine++;
	animation = enemy_step_down_animation_ADDRESS;
	object_loop_leftover = record;
	set_animation(0u, object, animation);
}

/* the soldier who kicked the door goes on as a Foot soldier */
void kicked_door_become_foot_soldier(uint32_t object)
{
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	OBJECT(object)->kind = 0u;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->routine = 0u;
	OBJECT(object)->state = 0u;
}

/* a piece of the kicked door flies off */
void door_piece_start(uint32_t object)
{
	uint8_t direction = 0;
	uint16_t piece_offset = 0, speed = 0;
	uint32_t animation = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	piece_offset = (uint16_t)((uint32_t)(int8_t)OBJECT(object)->parameter << 2);
	animation = TABLE32(door_piece_animations, (uint32_t)(int16_t)piece_offset);
	set_animation(0u, object, animation);
	OBJECT(object)->height_speed = TABLE16(door_piece_speeds, 0x2u + (uint32_t)(int16_t)piece_offset);
	direction = (uint8_t)TABLE16(door_piece_speeds, (uint32_t)(int16_t)piece_offset);
	speed = 0x500u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->routine++;
}

/* the door's piece flies until it leaves the screen */
void door_piece_fly(uint32_t object)
{
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	move_object(object);
}

/* an elevator with its doors closed */
void elevator_start(uint32_t object)
{
	uint32_t animation = 0, stop = 0;

	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	stop = elevator_stops_ADDRESS;
	if (OBJECT(object)->parameter != 0u)
		stop = elevator_stops_2_ADDRESS;
	OBJECT(object)->elevator.stop = stop;
	OBJECT(object)->routine++;
	animation = elevator_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the elevator waits until the scroll brings it to its place */
void elevator_wait_on_screen(uint32_t object)
{
	uint32_t stop = 0;

	stop = OBJECT(object)->elevator.stop;
	if (OBJECT(object)->screen_x <= ELEVATOR_STOP(stop)->x)
	{
		OBJECT(object)->elevator.wait = ELEVATOR_STOP(stop)->wait;
		OBJECT(object)->routine++;
	}
	object_loop_slot = stop;
}

/* the elevator's doors open and a Foot soldier comes out, two with three turtles or more in play */
void elevator_open(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint8_t wait = 0;

	wait = OBJECT(object)->elevator.wait;
	OBJECT(object)->elevator.wait = wait - 1u;
	if (wait != 1u)
		return;
	sound = 0x36u;
	queue_sound(sound);
	animation = elevator_doors_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	if (turtles_in_play < 3u)
	{
		object_loop_leftover = elevator_door_record_2_ADDRESS;
		record = elevator_door_record_2_ADDRESS;
		start_object_from_record(object, record, &ignored, &zero);
		return;
	}
	record = elevator_soldier_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	object_loop_leftover = elevator_door_record_ADDRESS;
	record = elevator_door_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
}

/* the open elevator is gone once it leaves the screen */
void elevator_scroll_off(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u && (OBJECT(object)->flags & 0x20u) != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
}

/* way in 2, out of a door or a lift: shown, hittable, its first pose for 24 frames */
void enemy_step_out_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->enemy_step_out.time = 0x18u;
	OBJECT(object)->state++;
	animation = enemy_step_out_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits in the doorway, then walks out for 32 frames */
void enemy_step_out_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t time = 0;

	time = OBJECT(object)->enemy_step_out.time;
	OBJECT(object)->enemy_step_out.time = time - 1u;
	if (time != 1u)
		return;
	OBJECT(object)->enemy_step_out.time = 0x20u;
	OBJECT(object)->state++;
	animation = enemy_step_out_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of walking out of the door; at its end, to thinking */
void enemy_step_out_walk(uint32_t object)
{
	uint8_t time = 0;

	time = OBJECT(object)->enemy_step_out.time;
	OBJECT(object)->enemy_step_out.time = time - 1u;
	if (time == 1u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_step_out.move_time = 0u;
	}
	OBJECT(object)->y -= 0x100u;
}

/* a tire with a Foot soldier behind it, counted as an enemy */
void tire_soldier_start(uint32_t object)
{
	uint32_t animation = 0;

	enemies_on_screen++;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->tire_soldier.trigger_x = 0x168u;
	if ((OBJECT(object)->parameter & 0x80u) != 0u)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->tire_soldier.trigger_x = 0xA0u;
	}
	OBJECT(object)->routine++;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	animation = rolling_tire_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier comes up out of the tire */
void tire_soldier_wait(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->tire_soldier.trigger_x < OBJECT(object)->screen_x)
		return;
	OBJECT(object)->y -= 0x1000u;
	OBJECT(object)->routine++;
	animation = tire_soldier_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier jumps */
void tire_soldier_jump(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x80;
	if ((OBJECT(object)->parameter & 0x40u) != 0u)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->routine++;
}

/* the soldier's jump comes down until it lands */
void tire_soldier_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->routine++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = tire_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier throws the tire */
void tire_soldier_throw(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	sound = 8u;
	queue_sound(sound);
	object_loop_leftover = thrown_tire_record_ADDRESS;
	record = thrown_tire_record_ADDRESS;
	start_object_from_record(object, record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->parameter;
	OBJECT(object)->routine++;
	animation = enemy_throw_low_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a manhole with a Foot soldier under its cover, counted as an enemy */
void manhole_soldier_start(uint32_t object)
{
	uint32_t animation = 0;

	enemies_on_screen++;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->manhole_soldier.trigger_x = 0x160u;
	if ((OBJECT(object)->parameter & 0x80u) != 0u)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->manhole_soldier.trigger_x = 0xA0u;
	}
	OBJECT(object)->routine++;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	animation = manhole_soldier_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier comes up out of the manhole holding its cover */
void manhole_soldier_wait(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->manhole_soldier.trigger_x < OBJECT(object)->screen_x)
		return;
	OBJECT(object)->y -= 0x1000u;
	OBJECT(object)->routine++;
	animation = manhole_soldier_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier jumps out and leaves the manhole open */
void manhole_soldier_jump_out(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x80;
	if ((OBJECT(object)->parameter & 0x40u) != 0u)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->routine++;
	object_loop_leftover = manhole_soldier_hole_record_ADDRESS;
	record = manhole_soldier_hole_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
}

/* the soldier's jump comes down to the ground */
void manhole_soldier_fly(uint32_t object)
{
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	if ((int8_t)OBJECT(object)->height_whole >= 0)
		return;
	OBJECT(object)->height = 0u;
	OBJECT(object)->routine++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = manhole_soldier_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier throws the manhole cover */
void manhole_soldier_throw(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	sound = 8u;
	queue_sound(sound);
	object_loop_leftover = thrown_manhole_cover_record_ADDRESS;
	record = thrown_manhole_cover_record_ADDRESS;
	start_object_from_record(object, record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->parameter;
	OBJECT(object)->routine++;
	animation = enemy_throw_low_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after its throw the soldier goes on as a Foot soldier */
void thrower_become_foot_soldier(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
	}
}

/* a Foot soldier behind a window, waiting for its place on the screen */
void window_soldier_drop_start(uint32_t object)
{
	uint32_t record = 0;
	uint32_t break_time_halfword = 0;

	record = object_tile_blocks_ADDRESS + (int16_t)TABLE16(object_tile_block_offsets, (uint32_t)(int16_t)((uint32_t)(int8_t)OBJECT(object)->parameter << 1));
	OBJECT(object)->window_soldier_drop.trigger_x = WINDOW_DROP(record)->trigger_x;
	record += 2u;
	break_time_halfword = WINDOW_DROP(record - 0x2u)->times;
	OBJECT(object)->window_soldier_drop.break_time = (uint8_t)(break_time_halfword >> 8u);
	OBJECT(object)->window_soldier_drop.delay = (uint8_t)break_time_halfword;
	record += 2u;
	OBJECT(object)->window_soldier_drop.blocks = record;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine++;
	object_loop_leftover = record;
}

/* the soldier jumps out through the window and the glass shatters */
void window_soldier_drop_jump_out(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint8_t delay = 0;

	if (OBJECT(object)->screen_x <= OBJECT(object)->window_soldier_drop.trigger_x)
	{
		delay = OBJECT(object)->window_soldier_drop.delay;
		OBJECT(object)->window_soldier_drop.delay = delay - 1u;
		if (delay != 1u)
			return;
		OBJECT(object)->flags |= 0x80u;
		OBJECT(object)->contacts |= 0x80u;
		OBJECT(object)->height_speed = 0;
		OBJECT(object)->window_soldier_drop.break_time = 4u;
		object_loop_leftover = window_drop_shards_record_ADDRESS;
		record = window_drop_shards_record_ADDRESS;
		start_object_from_record(object, record, &ignored, &zero);
		OBJECT(object)->routine++;
		animation = window_soldier_drop_jump_out_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
}

/* the window is drawn broken */
void window_soldier_drop_break_window(uint32_t object)
{
	uint16_t block_number = 0;
	uint32_t block = 0, blocks = 0;
	uint8_t break_time = 0;

	break_time = OBJECT(object)->window_soldier_drop.break_time;
	OBJECT(object)->window_soldier_drop.break_time = break_time - 1u;
	if (break_time != 1u)
	{
		window_soldier_drop_fall(object);
		return;
	}
	do
	{
		blocks = OBJECT(object)->window_soldier_drop.blocks;
		block = WINDOW_BREAK_STEP(blocks)->block;
		blocks += 4u;
		block_number = (uint16_t)(int8_t)(uint8_t)(WINDOW_BREAK_STEP(blocks)->block >> 24u);
		blocks++;
		OBJECT(object)->window_soldier_drop.break_time = (uint8_t)(WINDOW_BREAK_STEP(blocks)->block >> 24u);
		blocks++;
		OBJECT(object)->window_soldier_drop.blocks = blocks;
		draw_tile_block(block_number, block);
	} while (OBJECT(object)->window_soldier_drop.break_time != 0u);
	OBJECT(object)->routine++;
	object_loop_leftover = blocks;
	window_soldier_drop_fall(object);
}

/* the soldier comes down from the window until it lands */
void window_soldier_drop_fall(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->y -= 0x80u;
	OBJECT(object)->height += OBJECT(object)->height_speed;
	OBJECT(object)->height_speed -= 0x40u;
	if (OBJECT(object)->height_speed < -0x4000)
		OBJECT(object)->height_speed = -0x400;
	if (OBJECT(object)->height_whole < 0xF0u)
		return;
	OBJECT(object)->height = 0u;
	OBJECT(object)->routine++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier who came out of the window goes on as a Foot soldier */
void window_soldier_become_foot_soldier(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->kind = 0u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
	}
}

/* a Foot soldier behind a window, waiting for its place on the screen */
void window_soldier_dive_start(uint32_t object)
{
	uint32_t pair = 0;

	pair = object_tile_block_pairs_ADDRESS + (int16_t)((int8_t)OBJECT(object)->parameter * 0xCu);
	OBJECT(object)->window_soldier_dive.trigger_y = WINDOW_DIVE(pair)->trigger_y;
	OBJECT(object)->window_soldier_dive.blocks = pair + 2u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine++;
}

/* the window is drawn broken and the soldier dives out through the glass */
void window_soldier_dive_wait(uint32_t object)
{
	uint32_t zero = 0;
	uint16_t other_place = 0, place = 0;
	uint32_t animation = 0, block = 0, blocks = 0, ignored = 0, other_block = 0, record = 0;

	if ((int16_t)OBJECT(object)->screen_y < (int16_t)OBJECT(object)->window_soldier_dive.trigger_y)
		return;
	blocks = OBJECT(object)->window_soldier_dive.blocks;
	place = (uint16_t)(int8_t)DIVE_WINDOW(blocks)->place;
	block = DIVE_WINDOW(blocks)->block;
	draw_tile_block(place, block);
	other_place = (uint16_t)(int8_t)DIVE_WINDOW(blocks)->other_place;
	other_block = DIVE_WINDOW(blocks)->other_block;
	draw_tile_block(other_place, other_block);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->routine++;
	object_loop_leftover = window_dive_shards_record_ADDRESS;
	record = window_dive_shards_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	animation = window_soldier_dive_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier's dive comes down until it lands */
void window_soldier_dive_fall(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->x -= 0x280u;
	OBJECT(object)->height += OBJECT(object)->height_speed;
	OBJECT(object)->height_speed -= 0x40u;
	if (OBJECT(object)->height_speed < -0x4000)
		OBJECT(object)->height_speed = -0x400;
	if (OBJECT(object)->height_whole < 0xF0u)
		return;
	OBJECT(object)->height = 0u;
	OBJECT(object)->routine++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = window_soldier_dive_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a window's glass shatters into shards */
void glass_shards_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0, x_speed = 0;

	OBJECT(object)->flags = 0x81u;
	animation = glass_shards_start_animation_ADDRESS;
	x_speed = 0x80u;
	if ((int8_t)OBJECT(object)->parameter < 0)
	{
		animation = glass_shards_animation_ADDRESS;
		x_speed = 0xFF800000u;
	}
	OBJECT(object)->x_speed = (uint16_t)(x_speed >> 16u);
	OBJECT(object)->y_speed = (uint16_t)x_speed;
	set_animation(0u, object, animation);
	OBJECT(object)->height_speed = 0;
	sound = 0x40u;
	queue_sound(sound);
	OBJECT(object)->routine++;
}

/* the glass shards fall until they are gone */
void glass_shards_fall(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
}

/* way in 1: walks in from the right on the ledge */
void enemy_walk_in_on_ledge_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->floor_height = 0x50u;
	OBJECT(object)->state++;
	animation = enemy_foot_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks along the ledge; on the screen it crouches to jump down, or thinks up there if the nearest
   turtle is too */
void enemy_walk_in_on_ledge_walk(uint32_t object, uint32_t object_2)
{
	uint32_t animation = 0;

	move_object_blocked(object);
	if (OBJECT(object)->screen_x >= 0x120u)
		return;
	object_2 = enemy_nearest_turtles(object, object_2);
	if (OBJECT(object_2)->floor_height != 0x50u)
	{
		OBJECT(object)->state++;
		animation = enemy_crouch_to_hop_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->enemy_walk.move_time = 0u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
}

/* when the crouch ends, jumps down off the ledge */
void enemy_jump_down_leap(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = 0x200;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = -0x80;
	OBJECT(object)->state++;
	OBJECT(object)->contacts |= 0x80u;
	animation = enemy_hop_up_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump down, following stairs */
void enemy_jump_down_fall_stairs(uint32_t object)
{
	enemy_follow_stairs_down(object);
	enemy_jump_down_fall(object);
}

/* a frame of the fall off the ledge; on landing a drop pose */
void enemy_jump_down_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	if (OBJECT(object)->height_speed < -0x4000)
		OBJECT(object)->height_speed = -0x400;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the landing pose ends, to thinking */
void enemy_jump_down_land(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy.move_time = 0u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* way in 3: a step forward to the edge of its ledge for 16 frames */
void enemy_step_down_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_step_down.time = 0x10u;
	animation = enemy_step_down_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after the step, a crouch for the jump down */
void enemy_step_down_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t time = 0;

	time = OBJECT(object)->enemy_step_down.time;
	OBJECT(object)->enemy_step_down.time = time - 1u;
	if (time != 1u)
		return;
	OBJECT(object)->state++;
	animation = enemy_crouch_to_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a ripple on the sewer's water, a Foot soldier under it, counted as an enemy */
void sewer_ripple_start(uint32_t object)
{
	uint32_t animation = 0;

	enemies_on_screen++;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->x_speed = -0x80;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->sewer_ripple.swim_time = 0x5Au;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->routine++;
	animation = sewer_ripple_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the ripple moves back and forth, then splashes */
void sewer_ripple_swim(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint8_t swim_time = 0, t1 = 0;

	if (OBJECT(object)->screen_x >= 0xA0u)
	{
		if (OBJECT(object)->screen_x >= 0x160u)
			OBJECT(object)->x_speed = -0x80;
	}
	else
		OBJECT(object)->x_speed = 0x80;
	move_object(object);
	if ((OBJECT(object)->flags & 0x20u) == 0u)
	{
		swim_time = OBJECT(object)->sewer_ripple.swim_time;
		OBJECT(object)->sewer_ripple.swim_time = swim_time - 1u;
		if (swim_time != 1u)
			return;
		OBJECT(object)->routine++;
		sound = 0xEu;
		queue_sound(sound);
		animation = sewer_ripple_swim_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
	t1 = enemies_on_screen - 1u;
	enemies_on_screen = t1;
	if ((int8_t)t1 < 0)
		enemies_on_screen = 0u;
}

/* the soldier leaps out of the water */
void sewer_ripple_leap_out(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	animation = sewer_ripple_leap_out_animation_ADDRESS;
	set_animation(0u, object, animation);
	object_loop_leftover = sewer_ripple_soldier_record_ADDRESS;
	record = sewer_ripple_soldier_record_ADDRESS;
	start_object_from_record(object, record, &object_2, &zero);
	if (zero)
	{
		OBJECT(object_2)->flags = OBJECT(object)->flags;
		OBJECT(object_2)->flags &= 0x41u;
		OBJECT(object)->routine++;
		return;
	}
	OBJECT(object)->routine += 2u;
}

/* the splash ends and the ripple is gone */
void sewer_ripple_vanish(uint32_t object)
{
	uint8_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* no room for the soldier: the ripple swims on */
void sewer_ripple_swim_again(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->routine = 1u;
	OBJECT(object)->sewer_ripple.swim_time = 0x5Au;
	animation = sewer_ripple_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* way in 4: leaps up out of the sewer's water */
void enemy_leap_out_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0x600;
	OBJECT(object)->state++;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->contacts |= 0x80u;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the leap; on landing a crouch */
void enemy_leap_out_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	OBJECT(object)->floor_height = 0u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, to thinking */
void enemy_leap_out_land(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy.move_time = 0u;
	}
}

/* a Foot soldier behind cover with a stick of dynamite */
void bomber_start(uint32_t object)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t x_speed = 0, y_speed = 0;

	direction = 0x10u;
	speed = 0x180u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->routine++;
}

/* on the screen, the soldier leans out */
void bomber_wait(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->screen_x >= 0x160u)
		return;
	OBJECT(object)->bomber.timer = 0x4Au;
	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->routine++;
	animation = bomber_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier gets ready to throw */
void bomber_lean_out(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->bomber.timer;
	OBJECT(object)->bomber.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->routine++;
	OBJECT(object)->bomber.timer = 0x48u;
	animation = bomber_lean_out_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier throws the dynamite, then steps out */
void bomber_throw(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->bomber.timer;
	OBJECT(object)->bomber.timer = timer - 1u;
	if (timer == 1u)
	{
		object_loop_leftover = bomber_bomb_shot_ADDRESS;
		record = bomber_bomb_shot_ADDRESS;
		start_shot(object, record, &ignored, &zero);
	}
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->routine++;
	animation = bomber_throw_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier turns and walks out */
void bomber_step_out(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->x += 0x1000u;
	OBJECT(object)->bomber.timer = 0x1Eu;
	OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->routine++;
	animation = bomber_step_out_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier walks in and goes on as a Foot soldier */
void bomber_walk_in(uint32_t object)
{
	uint8_t timer = 0;

	move_object(object);
	timer = OBJECT(object)->bomber.timer;
	OBJECT(object)->bomber.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->kind = 0u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
	}
}

/* kind 56, the Foot soldier with a rocket on its shoulder: comes in slowly from the right */
void enemy_rocket_soldier_start(uint32_t object)
{
	uint32_t animation = 0;

	enemies_on_screen++;
	OBJECT(object)->flags |= 0x82u;
	animation = enemy_rocket_soldier_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = -0x40;
	OBJECT(object)->enemy_rocket_soldier.aim_time = 1u;
	OBJECT(object)->routine++;
}

/* comes in, moving up or down to the nearest turtle's row; on the screen and level with it, aims */
void enemy_rocket_soldier_line_up(uint32_t object)
{
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, turtle = 0;
	uint8_t aim_time = 0;

	turtle = OBJECT(object)->enemy_rocket_soldier.turtle;
	aim_time = OBJECT(object)->enemy_rocket_soldier.aim_time;
	OBJECT(object)->enemy_rocket_soldier.aim_time = aim_time - 1u;
	if (aim_time == 1u)
	{
		OBJECT(object)->enemy_rocket_soldier.aim_time = 8u;
		turtle = enemy_nearest_turtles(object, turtle);
		OBJECT(object)->enemy_rocket_soldier.turtle = turtle;
	}
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->y_speed = 0x80;
	if ((int8_t)OBJECT(object)->enemy_rocket_soldier.target_direction >= 0)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	move_object_blocked(object);
	if (OBJECT(object)->screen_x >= 0x170u || OBJECT(object)->enemy_rocket_soldier.target_dy >= 0x10u)
		return;
	OBJECT(object)->routine++;
	animation = enemy_rocket_soldier_line_up_animation_ADDRESS;
	set_animation(ignored, object, animation);
}

/* when the aim ends, fires the rocket with its sound; it can no longer be hit */
void enemy_rocket_soldier_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= ~2u;
	sound = 8u;
	queue_sound(sound);
	object_loop_leftover = rocket_shot_record_ADDRESS;
	record = rocket_shot_record_ADDRESS;
	start_shot(object, record, &ignored, &zero);
	OBJECT(object)->routine++;
	animation = enemy_throw_low_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the throw pose ends, it starts again as a bare-handed soldier */
void enemy_rocket_soldier_unarmed(uint32_t object)
{
	uint8_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0x81u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* hit with the rocket on: drops it and takes the hit as a bare-handed soldier */
void enemy_rocket_soldier_struck(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t object_2 = 0, reactions = 0, record = 0;
	uint8_t t1 = 0;

	object_loop_leftover = dropped_rocket_shot_record_ADDRESS;
	record = dropped_rocket_shot_record_ADDRESS;
	start_shot(object, record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = 1u;
	t1 = enemies_on_screen - 1u;
	enemies_on_screen = t1;
	if ((int8_t)t1 < 0)
		enemies_on_screen = 0u;
	enemy_foot_soldier_start(object, object_2);
	OBJECT(object)->kind = 0u;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->routine = 4u;
	OBJECT(object)->state = 0u;
	reactions = OBJECT(object)->enemy_rocket_soldier.definition + 0x58u;
	if ((int8_t)REACTION(reactions + (OBJECT(object)->hit & 0x7Fu)) < 0)
	{
		OBJECT(object)->health_bars = 0u;
		OBJECT(object)->health = 0u;
	}
	enemy_hit(object);
}

/* two closed doors */
void double_door_start(uint32_t object)
{
	OBJECT(object)->routine++;
}

/* the doors swing open and two Foot soldiers come out */
void double_door_wait(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint16_t place = 0;
	uint32_t animation = 0, block = 0, ignored = 0, record = 0;

	if (OBJECT(object)->screen_x >= 0x100u)
		return;
	OBJECT(object)->flags |= 0x80u;
	animation = double_door_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
	sound = 0x3Fu;
	queue_sound(sound);
	place = 0x22u;
	block = double_door_kicked_tile_block_ADDRESS;
	draw_tile_block(place, block);
	OBJECT(object)->routine++;
	record = double_door_piece_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	object_loop_leftover = double_door_piece_record_2_ADDRESS;
	record = double_door_piece_record_2_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
}

/* the open doorway stays in the layer and the doors are gone */
void double_door_open(uint32_t object)
{
	uint16_t place = 0;
	uint32_t block = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags = 0u;
	OBJECT(object)->contacts = 0u;
	place = 0x23u;
	block = double_door_open_tile_block_ADDRESS;
	draw_tile_block(place, block);
	place = 0x24u;
	block = double_door_open_tile_block_2_ADDRESS;
	draw_tile_block(place, block);
}

/* a closed door of the Technodrome */
void technodrome_door_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	animation = technodrome_door_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* on the screen, the door opens and the first Foot soldier comes out */
void technodrome_door_wait(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, record = 0;

	if (OBJECT(object)->screen_x >= 0x140u)
		return;
	OBJECT(object)->routine++;
	object_loop_leftover = technodrome_door_soldier_record_ADDRESS;
	record = technodrome_door_soldier_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	sound = 0x36u;
	queue_sound(sound);
	OBJECT(object)->technodrome_door.spawn_time = 0x18u;
	OBJECT(object)->technodrome_door.soldiers_left = technodrome_door_soldiers[difficulty & 7u];
	animation = technodrome_door_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the door is open */
void technodrome_door_open(uint32_t object)
{
	OBJECT(object)->technodrome_door.spawn_time--;
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags &= 0x7Fu;
		OBJECT(object)->routine++;
	}
}

/* more Foot soldiers come out, as many as the players' setting gives */
void technodrome_door_let_out(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, record = 0;
	uint8_t spawn_time = 0, soldiers_left = 0;

	spawn_time = OBJECT(object)->technodrome_door.spawn_time;
	OBJECT(object)->technodrome_door.spawn_time = spawn_time - 1u;
	if (spawn_time == 1u)
	{
		OBJECT(object)->technodrome_door.spawn_time = 0x18u;
		object_loop_leftover = technodrome_door_soldier_record_ADDRESS;
		record = technodrome_door_soldier_record_ADDRESS;
		start_object_from_record(object, record, &ignored, &zero);
		soldiers_left = OBJECT(object)->technodrome_door.soldiers_left;
		OBJECT(object)->technodrome_door.soldiers_left = soldiers_left - 1u;
		if (soldiers_left == 1u)
		{
			OBJECT(object)->flags = 0u;
			OBJECT(object)->contacts = 0u;
		}
	}
}

/* way in 5: hangs by its hands, moving across for 96 frames */
void enemy_hang_in_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_hang_in.time = 0x60u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	if ((uint8_t)(OBJECT(object)->screen_x >> 8u) == 0u)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	animation = enemy_hang_in_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* moves across hanging by its hands; at its end it lets go, hittable */
void enemy_hang_in_move(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t time = 0;

	move_object(object);
	time = OBJECT(object)->enemy_hang_in.time;
	OBJECT(object)->enemy_hang_in.time = time - 1u;
	if (time != 1u)
		return;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->state++;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = 0;
	OBJECT(object)->x_speed = 0;
	animation = enemy_hang_in_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the drop; on landing a drop pose */
void enemy_hang_in_drop(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	OBJECT(object)->floor_height = 0u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->state++;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the landing pose ends, to thinking */
void enemy_hang_in_land(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_hang_in.move_time = 0u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* way in 6: runs in at full speed for 40 frames */
void enemy_kick_in_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0xC2u;
	OBJECT(object)->x_speed = 0x300;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->flags |= 0x40u;
	if ((uint8_t)(OBJECT(object)->screen_x >> 8u) != 0u)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		OBJECT(object)->flags &= 0xBFu;
	}
	OBJECT(object)->state++;
	OBJECT(object)->enemy_kick_in.time = 0x28u;
	animation = enemy_dash_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* runs in, then crouches for the kick */
void enemy_kick_in_run(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t time = 0;

	time = OBJECT(object)->enemy_kick_in.time;
	OBJECT(object)->enemy_kick_in.time = time - 1u;
	if (time != 1u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, flies in with a kick along its arc, attacking */
void enemy_kick_in_leap(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_kick_in_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the flying kick; on landing a crouch, no longer attacking */
void enemy_kick_in_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_kick_in_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, to thinking */
void enemy_kick_in_land(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_kick_in.move_time = 0u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* way in 7: runs in across for 126 frames */
void enemy_run_in_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_run_in.time = 0x7Eu;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->x_speed = 0x180;
	OBJECT(object)->y_speed = 0;
	if ((uint8_t)(OBJECT(object)->screen_x >> 8u) != 0u)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		OBJECT(object)->flags |= 0x40u;
	}
	OBJECT(object)->state++;
	animation = enemy_run_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the run in; at its end, to thinking */
void enemy_run_in_run(uint32_t object)
{
	uint8_t time = 0;

	time = OBJECT(object)->enemy_run_in.time;
	OBJECT(object)->enemy_run_in.time = time - 1u;
	if (time == 1u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
	move_object_blocked(object);
}

/* way in 8: runs in on the ledge at full speed */
void enemy_dive_in_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->x_speed = -0x300;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_dive_in.time = 0x20u;
	OBJECT(object)->floor_height = 0x50u;
	animation = enemy_dash_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* runs along the ledge; on the screen it crouches to dive off, and thinks if the nearest turtle is up
   there */
void enemy_dive_in_run(uint32_t object, uint32_t object_2)
{
	uint32_t animation = 0;

	if (OBJECT(object)->screen_x >= 0x120u)
	{
		move_object_blocked(object);
		return;
	}
	object_2 = enemy_nearest_turtles(object, object_2);
	if (OBJECT(object_2)->floor_height == 0x50u)
	{
		OBJECT(object)->enemy_dive_in.move_time = 0u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, leaps off the ledge */
void enemy_dive_in_leap(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->x_speed = -0x80;
	OBJECT(object)->y_speed = -0x80;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->enemy_dive_in.time = 0xCu;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_leap_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the leap, following stairs; after 12 frames the dive */
void enemy_dive_in_dive(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t time = 0;

	time = OBJECT(object)->enemy_dive_in.time;
	OBJECT(object)->enemy_dive_in.time = time - 1u;
	if (time == 1u)
	{
		OBJECT(object)->state++;
		animation = enemy_dive_in_dive_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x38u;
}

/* a frame of the dive; when its pose ends, it curls up for 20 frames */
void enemy_dive_in_curl(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->state++;
		OBJECT(object)->enemy_dive_in.time = 0x14u;
		OBJECT(object)->animation_delay = 2u;
		animation = enemy_dive_in_curl_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x38u;
}

/* a frame of the fall curled up; at its end it uncurls */
void enemy_dive_in_uncurl(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t time = 0;

	time = OBJECT(object)->enemy_dive_in.time;
	OBJECT(object)->enemy_dive_in.time = time - 1u;
	if (time == 1u)
	{
		OBJECT(object)->state++;
		OBJECT(object)->animation_delay = 0u;
		animation = enemy_dive_in_uncurl_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x38u;
}

/* a frame of the fall, following stairs; on landing a crouch */
void enemy_dive_in_land(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x38u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* way in 9: walks in across for 26 frames */
void enemy_walk_in_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x81u;
	OBJECT(object)->x_speed = -0x140;
	if ((uint8_t)(OBJECT(object)->screen_x >> 8u) != 0u)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->enemy_walk.animations = (OBJECT(object)->enemy_walk.animations & 0xFFFFFFu) | 0x1A000000u;
	OBJECT(object)->state++;
	animation = enemy_foot_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the walk in; at its end it stands ready for 48 frames */
void enemy_walk_in_walk(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t t1 = 0;

	move_object_blocked(object);
	t1 = (uint8_t)(OBJECT(object)->enemy_walk.animations >> 24u);
	OBJECT(object)->enemy_walk.animations = (OBJECT(object)->enemy_walk.animations & 0xFFFFFFu) | ((t1 - 1u & 0xFFu) << 24u);
	if (t1 != 1u)
		return;
	OBJECT(object)->enemy_walk.animations = (OBJECT(object)->enemy_walk.animations & 0xFFFFFFu) | 0x30000000u;
	OBJECT(object)->state++;
	animation = enemy_walk_in_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* stands ready, then to thinking */
void enemy_walk_in_ready(uint32_t object)
{
	uint8_t t1 = 0;

	t1 = (uint8_t)(OBJECT(object)->enemy_walk.animations >> 24u);
	OBJECT(object)->enemy_walk.animations = (OBJECT(object)->enemy_walk.animations & 0xFFFFFFu) | ((t1 - 1u & 0xFFu) << 24u);
	if (t1 == 1u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_walk.move_time = 0u;
	}
}

/* a Foot soldier with knives: its definition, colour, bars and first move by its parameter */
void enemy_knife_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x87u;
	OBJECT(object)->colour = 7u;
	OBJECT(object)->floor_height = 0u;
	enemy_set_floor(object);
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = knife_soldier_definition_ADDRESS;
	OBJECT(object)->enemy.move = (OBJECT(object)->parameter & 0xF0u) << 2;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_knife_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a Foot soldier with a spear, and the hits it lands before it throws the spear */
void enemy_spear_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy_spear.target = player_slot;
	OBJECT(object)->enemy_spear.retarget_time = 0x10u;
	OBJECT(object)->enemy_spear.move_time = 0u;
	OBJECT(object)->enemy_spear.approach = 0u;
	OBJECT(object)->enemy_spear.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x84u;
	OBJECT(object)->colour = 4u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->enemy_spear.hits_left = spear_soldier_hits[difficulty & 7u];
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy_spear.definition = spear_soldier_definition_ADDRESS;
	OBJECT(object)->enemy_spear.move = (OBJECT(object)->parameter & 0xF0u) << 2;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_spear_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks to its turtle with the spear */
void enemy_walk_to_target_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk with the spear's animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.animations = spear_soldier_walk_animations_ADDRESS;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its side of its turtle with the spear */
void enemy_walk_to_side_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side with the spear, facing it */
void enemy_walk_to_side_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = spear_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* keeps off its turtle's line, crossing the screen */
void enemy_walk_off_line_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_off_line_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* 180 frames of walking off its turtle's row with the spear, facing it */
void enemy_walk_off_line_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xB4u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
		OBJECT(object)->enemy_walk.speed = 0u - OBJECT(object)->enemy_walk.speed;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk: across past its turtle while off its row, turning back beyond 0x40, else up or
   down */
void enemy_walk_off_line_spear_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_walk.target_dy >= 0x20u)
	{
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
		if (OBJECT(object)->enemy_walk.target_dx >= 0x40u)
		{
			OBJECT(object)->enemy_walk.speed = 0x100u;
			if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
				OBJECT(object)->enemy_walk.speed = 0u - OBJECT(object)->enemy_walk.speed;
		}
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0x100;
		if ((int8_t)OBJECT(object)->enemy_walk.target_direction < 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	move_object_blocked(object);
}

/* walks away, crouches and waits while its turtle is down */
void enemy_back_off_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_back_off_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* 32 frames of walking away across with the spear, facing its turtle */
void enemy_back_off_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_back_off.move_time = 0xFFu;
	OBJECT(object)->enemy_back_off.time = 0x20u;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_back_off.target_direction - 0x40u) < 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk away; at its end, a crouch */
void enemy_back_off_spear_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_turn_to_object(target_direction, object, turtle);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_back_off_spear_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* steps up or down out of the way of its turtle's attack */
void enemy_step_aside_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_step_aside_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* 16 frames of stepping up or down and away out of its turtle's attack, facing it */
void enemy_step_aside_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	direction = 0x60u;
	if ((int8_t)OBJECT(object)->enemy.target_direction >= 0)
		direction = (uint8_t)(0u - direction);
	if ((int8_t)(OBJECT(object)->enemy.target_direction - 0x40u) < 0)
		direction ^= 0x40u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	target_direction = OBJECT(object)->enemy.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the step aside; at its end, back to its first approach */
void enemy_step_aside_spear_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy.approach = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = spear_soldier_walk_animations_ADDRESS;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks to its turtle with the knife */
void enemy_walk_to_target_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk with the knife's animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.animations = knife_soldier_walk_animations_ADDRESS;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its side of its turtle with the knife */
void enemy_walk_to_side_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side with the knife, facing it */
void enemy_walk_to_side_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* runs round its turtle with the knife */
void enemy_circle_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_circle_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* 180 frames of running round its turtle with the knife, one way round or the other at random */
void enemy_circle_knife_start(uint32_t object)
{
	uint32_t animations = 0, direction = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_circle.move_time = 0xB4u;
	animations = knife_soldier_run_animations_ADDRESS;
	direction = enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->enemy_circle.clockwise = 0u;
	if ((int8_t)(uint8_t)(random_seed >> 8u) >= 0)
	{
		enemy_circle_knife_aim((uint8_t)direction, object);
		return;
	}
	OBJECT(object)->enemy_circle.clockwise = 1u;
	enemy_circle_knife_aim((uint8_t)direction, object);
}

/* its speeds for running round its turtle with the knife the way it goes */
void enemy_circle_knife_aim(uint8_t direction, uint32_t object)
{
	uint16_t speed = 0;
	uint32_t x_speed = 0, y_speed = 0;

	if (OBJECT(object)->enemy_circle.clockwise != 0u)
		direction += 0x50u;
	else
		direction -= 0x30u;
	speed = 0x140u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
}

/* a frame of the run round its turtle: the other way when it bumps into something, aiming again as its
   turtle moves */
void enemy_circle_knife_step(uint32_t object)
{
	uint32_t animations = 0, direction = 0;

	OBJECT(object)->enemy_circle.move_time--;
	if ((OBJECT(object)->contacts & 0x1Eu) != 0u)
	{
		OBJECT(object)->enemy_circle.clockwise ^= 1u;
		animations = knife_soldier_run_animations_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_knife_aim((uint8_t)direction, object);
		move_object_blocked(object);
		return;
	}
	if (OBJECT(object)->enemy_circle.target_direction != OBJECT(object)->enemy_circle.facing)
	{
		animations = knife_soldier_run_animations_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_knife_aim((uint8_t)direction, object);
	}
	move_object_blocked(object);
}

/* keeps its distance from its turtle with the knife */
void enemy_keep_distance_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_keep_distance_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle with the knife, for up to 255 frames of keeping its distance */
void enemy_keep_distance_knife_start(uint32_t object)
{
	uint32_t animations = 0;

	OBJECT(object)->enemy.move_time = 0xFFu;
	OBJECT(object)->state++;
	animations = knife_soldier_run_animations_ADDRESS;
	enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->frame_time = 0u;
}

/* stands facing its turtle with the knife; too near it backs off, too far it closes in */
void enemy_keep_distance_knife_watch(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy.move_time--;
	if (OBJECT(object)->enemy.target_direction != OBJECT(object)->enemy.facing)
	{
		animations = knife_soldier_run_animations_ADDRESS;
		enemy_face_target_animation(0u, object, animations);
		OBJECT(object)->frame_time = 0u;
	}
	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
	{
		if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
			return;
		OBJECT(object)->state = 3u;
		direction = OBJECT(object)->enemy.target_direction;
		object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
		turtle = PLAYER_SLOT(player_slot)->turtle;
		enemy_face_object(direction, object, turtle);
		speed = 0x100u;
		speeds_from_direction(direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = y_speed;
		return;
	}
	OBJECT(object)->state = 2u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
}

/* walks away from its turtle with the knife until far enough, then watches again */
void enemy_keep_distance_knife_back_off(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks toward its turtle with the knife until near enough, then watches again */
void enemy_keep_distance_knife_close_in(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks away, turns and waits while its turtle is down */
void enemy_back_off_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_back_off_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* 30 frames of walking backwards away from its turtle with the knife, still facing it */
void enemy_back_off_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x1Eu;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	direction -= 0x80u;
	speed = 0x140u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; at its end it turns to the turtle */
void enemy_back_off_knife_walk(uint32_t object, uint32_t player_slot)
{
	uint32_t animations = 0, direction = 0, turtle = 0;
	uint8_t time = 0;

	direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object((uint8_t)direction, object, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_knife_wait(object, player_slot);
		return;
	}
	animations = knife_soldier_run_animations_ADDRESS;
	enemy_face_target_animation(direction, object, animations);
	OBJECT(object)->frame_time = 0u;
	OBJECT(object)->state++;
	enemy_back_off_knife_wait(object, player_slot);
}

/* waits while its turtle is down; when it is up again, a new move */
void enemy_back_off_knife_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_back_off.approach = 0u;
		OBJECT(object)->enemy_back_off.move_time = 0u;
	}
}

/* steps up or down out of the way of its turtle's attack */
void enemy_step_aside_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_step_aside_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* 8 frames of stepping up or down out of its turtle's attack, facing it */
void enemy_step_aside_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x140;
	if ((int8_t)OBJECT(object)->enemy.target_direction < 0)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* a frame of the step aside; at its end, back to its first approach */
void enemy_step_aside_knife_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy.approach = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = knife_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* thrusts the spear; enough hits and it throws it next */
void enemy_spear_thrust(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_spear_thrust_states_ADDRESS;
	run_object_state(object, states);
}

/* the thrust's animation and damage, facing its turtle */
void enemy_spear_thrust_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_spear_thrust_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x202u;
	attack = 3u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the thrust ends: after enough hits the spear throw, else back to thinking */
void enemy_spear_thrust_end(uint32_t object)
{
	uint8_t hits_left = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		hits_left = OBJECT(object)->enemy_spear.hits_left;
		OBJECT(object)->enemy_spear.hits_left = hits_left - 1u;
		if (hits_left == 1u)
		{
			OBJECT(object)->enemy_spear.move = 0x4Cu;
			OBJECT(object)->routine = 3u;
			OBJECT(object)->state = 0u;
			OBJECT(object)->flags &= ~8u;
			return;
		}
	}
	enemy_back_to_thinking(object);
}

/* throws its spear and fights on bare-handed */
void enemy_spear_throw(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_spear_throw_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle with the throw's animation; the spear goes in 20 frames */
void enemy_spear_throw_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_spear.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_spear.move_time = 0x14u;
	OBJECT(object)->state++;
	animation = enemy_spear_throw_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after 20 frames the spear flies, and it cannot be hit */
void enemy_spear_throw_release(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, record = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_spear.move_time;
	OBJECT(object)->enemy_spear.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		object_loop_leftover = spear_shot_record_ADDRESS;
		record = spear_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->state++;
	}
}

/* when the throw ends, it starts again as a bare-handed soldier, thinking */
void enemy_spear_throw_end(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		enemy_leave_side(object, 0u);
		PLAYER_SLOT(object_loop_slot)->enemies--;
		OBJECT(object)->kind = 0u;
		OBJECT(object)->parameter = 0u;
		enemy_foot_soldier_setup(object, player_slot);
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* stabs with the knife */
void enemy_knife_stab(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_knife_stab_states_ADDRESS;
	run_object_state(object, states);
}

/* the stab's animation and damage, facing its turtle */
void enemy_knife_stab_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_knife_stab_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x202u;
	attack = 3u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a high kick */
void enemy_high_kick_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_high_kick_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* the high kick's animation and damage, facing its turtle */
void enemy_high_kick_knife_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_high_kick_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x302u;
	attack = 2u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* a flying kick */
void enemy_jump_kick_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_jump_kick_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* crouches for the flying kick, its damage set, facing its turtle */
void enemy_jump_kick_knife_crouch(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_knife_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x303u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* when the crouch ends, leaps along the kick's arc */
void enemy_jump_kick_knife_leap(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	arc = enemy_jump_kick_knife_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the flying kick along its arc, following stairs; on landing a crouch */
void enemy_jump_kick_knife_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_jump_kick_knife_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knife_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* throws a knife */
void enemy_throw_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_throw_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle with the throw's animation; the knife goes in 20 frames */
void enemy_throw_knife_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_throw.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_throw.throw_time = 0x14u;
	OBJECT(object)->state++;
	animation = enemy_throw_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after 20 frames the knife flies */
void enemy_throw_knife_release(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, record = 0;
	uint8_t throw_time = 0;

	throw_time = OBJECT(object)->enemy_throw.throw_time;
	OBJECT(object)->enemy_throw.throw_time = throw_time - 1u;
	if (throw_time != 1u)
		return;
	OBJECT(object)->state++;
	object_loop_leftover = knife_shot_record_ADDRESS;
	record = knife_shot_record_ADDRESS;
	start_shot(object, record, &ignored, &zero);
}

/* back to thinking when the throw's animation ends */
void enemy_throw_knife_end(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemy_back_to_thinking(object);
}

/* hit: its head thrown back */
void enemy_flinch_back_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* the spear soldier's flinch with its head thrown back */
void enemy_flinch_back_spear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_spear_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: bent over */
void enemy_flinch_bent_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* the spear soldier's flinch bent over */
void enemy_flinch_bent_spear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_spear_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: reeling */
void enemy_flinch_reel_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_reel_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* the spear soldier's flinch reeling */
void enemy_flinch_reel_spear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_reel_spear_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: hunched */
void enemy_flinch_hunched_spear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_hunched_spear_states_ADDRESS;
	run_object_state(object, states);
}

/* the spear soldier's flinch hunched */
void enemy_flinch_hunched_spear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_hunched_spear_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the knife soldier flinches with its head thrown back */
void enemy_flinch_back_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* the knife soldier's flinch with its head thrown back */
void enemy_flinch_back_knife_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the knife soldier flinches bent over */
void enemy_flinch_bent_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* the knife soldier's flinch bent over */
void enemy_flinch_bent_knife_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the knife soldier flinches reeling */
void enemy_flinch_reel_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_reel_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* the knife soldier's flinch reeling */
void enemy_flinch_reel_knife_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_reel_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the knife soldier flinches hunched */
void enemy_flinch_hunched_knife(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_hunched_knife_states_ADDRESS;
	run_object_state(object, states);
}

/* the knife soldier's flinch hunched */
void enemy_flinch_hunched_knife_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_hunched_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a Foot soldier with throwing axes */
void enemy_axe_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x85u;
	OBJECT(object)->colour = 5u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = axe_soldier_definition_ADDRESS;
	OBJECT(object)->enemy.move = (OBJECT(object)->parameter & 0xF0u) << 2;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_axe_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a Foot soldier with a gun */
void enemy_gun_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x85u;
	OBJECT(object)->colour = 5u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = gun_soldier_definition_ADDRESS;
	OBJECT(object)->enemy.move = (OBJECT(object)->parameter & 0xF0u) << 2;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_gun_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks to its turtle with the gun */
void enemy_walk_to_target_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk with the gun's animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x120u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.animations = gun_soldier_walk_animations_ADDRESS;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its side of its turtle with the gun */
void enemy_walk_to_side_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side with the gun, facing it */
void enemy_walk_to_side_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = gun_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its turtle's row at shooting distance and stands there */
void enemy_walk_to_range_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_range_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to shooting distance from its turtle, facing it */
void enemy_walk_to_range_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = gun_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its turtle's row at 0x5A across, then stands in its ready pose */
void enemy_walk_to_range_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation_entry = 0, animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_walk.target_dx < 0x5Au)
	{
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
		if ((int8_t)OBJECT(object)->enemy_walk.side < 0)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = OBJECT(object)->enemy_walk.speed;
		if ((int8_t)OBJECT(object)->enemy_walk.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	if (OBJECT(object)->enemy_walk.target_dy >= 2u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->state++;
	animations = OBJECT(object)->enemy_walk.animations;
	animation_entry = ANIMATION_ENTRY(animations + 0x8u);
	object_loop_leftover = animations;
	set_animation(0u, object, animation_entry);
}

/* stands ready while level with its turtle, walks again when off its row */
void enemy_walk_to_range_hold(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	if (OBJECT(object)->enemy_walk.target_dy < 3u)
		return;
	OBJECT(object)->state--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	object_loop_leftover = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
}

/* walks away, crouches and waits while its turtle is down */
void enemy_back_off_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_back_off_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* 32 frames of walking backwards away from its turtle with the gun, still facing it */
void enemy_back_off_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x20u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
void enemy_back_off_gun_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_gun_wait(object, player_slot);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_gun_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits while its turtle is down; when it is up again, a new move */
void enemy_back_off_gun_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_back_off.approach = 0u;
		OBJECT(object)->enemy_back_off.move_time = 0u;
	}
}

/* runs straight away from its turtle for a moment */
void enemy_run_away_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_run_away_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* 8 frames of running straight away from its turtle with the gun, still facing it */
void enemy_run_away_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy.move_time = 8u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
	enemy_run_away_gun_step(object, player_slot);
}

/* a frame of running away; at its end, back to its first approach and a new move */
void enemy_run_away_gun_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->enemy.approach = 0u;
		OBJECT(object)->enemy.move_time = 0u;
	}
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = gun_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* walks to its turtle with the axes */
void enemy_walk_to_target_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_target_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk with the axes' animations, facing its turtle; it aims again in 8 frames */
void enemy_walk_to_target_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.animations = axe_soldier_walk_animations_ADDRESS;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its side of its turtle with the axes */
void enemy_walk_to_side_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_side_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to its turtle's side with the axes, facing it */
void enemy_walk_to_side_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = axe_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks to its turtle's row at throwing distance and stands there */
void enemy_walk_to_range_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_walk_to_range_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to throwing distance from its turtle, facing it */
void enemy_walk_to_range_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	OBJECT(object)->enemy_walk.animations = axe_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* walks away, crouches and waits while its turtle is down */
void enemy_back_off_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_back_off_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* 40 frames of walking backwards away from its turtle with the axes, still facing it */
void enemy_back_off_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x28u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
void enemy_back_off_axe_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_axe_wait(object, player_slot);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_axe_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits while its turtle is down; when it is up again, a new move */
void enemy_back_off_axe_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_back_off.approach = 0u;
		OBJECT(object)->enemy_back_off.move_time = 0u;
	}
}

/* runs straight away from its turtle for a moment */
void enemy_run_away_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_run_away_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* 8 frames of running straight away from its turtle with the axes, still facing it */
void enemy_run_away_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy.move_time = 8u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	speed = 0x180u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, turtle);
	enemy_run_away_axe_step(object, player_slot);
}

/* a frame of running away; at its end, back to its first approach and a new move */
void enemy_run_away_axe_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->enemy.approach = 0u;
		OBJECT(object)->enemy.move_time = 0u;
	}
	target_direction = OBJECT(object)->enemy.target_direction;
	object_loop_leftover = axe_soldier_walk_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
}

/* throws three axes */
void enemy_throw_axes(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_throw_axes_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle with the throw's animation: three axes to go, 16 frames apart */
void enemy_throw_axes_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_throw.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_throw.throw_time = 0x10u;
	OBJECT(object)->enemy_throw.throws_left = 3u;
	OBJECT(object)->state++;
	animation = enemy_throw_axes_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* every 16 frames an axe with its sound; after the third, a crouch */
void enemy_throw_axes_release(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint8_t throw_time = 0, throws_left = 0;

	throw_time = OBJECT(object)->enemy_throw.throw_time;
	OBJECT(object)->enemy_throw.throw_time = throw_time - 1u;
	if (throw_time == 1u)
	{
		sound = 2u;
		queue_sound(sound);
		object_loop_leftover = axe_shot_record_ADDRESS;
		record = axe_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		OBJECT(object)->enemy_throw.throw_time = 0x10u;
		throws_left = OBJECT(object)->enemy_throw.throws_left;
		OBJECT(object)->enemy_throw.throws_left = throws_left - 1u;
		if (throws_left == 1u)
		{
			OBJECT(object)->state++;
			animation = enemy_axe_crouch_walk_animation_ADDRESS;
			set_animation(0u, object, animation);
			return;
		}
	}
}

/* a high kick */
void enemy_high_kick_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_high_kick_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* the high kick's animation and damage, facing its turtle */
void enemy_high_kick_axe_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_high_kick_axe_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x302u;
	attack = 2u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* swings the gun like a club */
void enemy_gun_swing(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_gun_swing_states_ADDRESS;
	run_object_state(object, states);
}

/* the swing's animation and damage, facing its turtle */
void enemy_gun_swing_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_gun_swing_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x302u;
	attack = 1u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* fires four shots */
void enemy_shoot_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shoot_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle with the gun up: four shots to go, 8 frames apart */
void enemy_shoot_gun_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_shoot.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_shoot.shot_time = 8u;
	OBJECT(object)->enemy_shoot.shots_left = 4u;
	OBJECT(object)->state++;
	animation = enemy_shoot_gun_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* every 8 frames a shot with its sound; after the fourth, a crouch */
void enemy_shoot_gun_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_shoot.shot_time;
	OBJECT(object)->enemy_shoot.shot_time = shot_time - 1u;
	if (shot_time == 1u)
	{
		sound = 0x18u;
		queue_sound(sound);
		object_loop_leftover = gun_shot_record_ADDRESS;
		record = gun_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		OBJECT(object)->enemy_shoot.shot_time = 8u;
		shots_left = OBJECT(object)->enemy_shoot.shots_left;
		OBJECT(object)->enemy_shoot.shots_left = shots_left - 1u;
		if (shots_left == 1u)
		{
			OBJECT(object)->state++;
			animation = enemy_gun_crouch_walk_animation_ADDRESS;
			set_animation(0u, object, animation);
			return;
		}
	}
}

/* hit: the gun soldier flinches with its head thrown back */
void enemy_flinch_back_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* the gun soldier's flinch with its head thrown back */
void enemy_flinch_back_gun_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_gun_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the gun soldier flinches bent over */
void enemy_flinch_bent_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* the gun soldier's flinch bent over */
void enemy_flinch_bent_gun_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_gun_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the axe soldier flinches with its head thrown back */
void enemy_flinch_back_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_back_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* the axe soldier's flinch with its head thrown back */
void enemy_flinch_back_axe_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_back_axe_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the axe soldier flinches bent over */
void enemy_flinch_bent_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_bent_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* the axe soldier's flinch bent over */
void enemy_flinch_bent_axe_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_axe_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hit: the axe soldier flinches reeling */
void enemy_flinch_reel_axe(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_flinch_reel_axe_states_ADDRESS;
	run_object_state(object, states);
}

/* the axe soldier's flinch reeling */
void enemy_flinch_reel_axe_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_reel_axe_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a bare-handed Foot soldier that also throws a boomerang */
void enemy_boomerang_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy_boomerang.target = player_slot;
	OBJECT(object)->enemy_boomerang.retarget_time = 0x10u;
	OBJECT(object)->enemy_boomerang.move_time = 0u;
	OBJECT(object)->enemy_boomerang.approach = 0u;
	OBJECT(object)->enemy_boomerang.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x86u;
	OBJECT(object)->colour = 6u;
	OBJECT(object)->floor_height = 0u;
	enemy_set_floor(object);
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy_boomerang.definition = boomerang_soldier_definition_ADDRESS;
	OBJECT(object)->enemy_boomerang.move = (OBJECT(object)->parameter & 0xF0u) << 2;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_foot_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a bare-handed Foot soldier that also throws shuriken */
void enemy_shuriken_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->throw = 0x85u;
	OBJECT(object)->colour = 5u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->enemy.definition = shuriken_soldier_definition_ADDRESS;
	OBJECT(object)->enemy.move = (OBJECT(object)->parameter & 0xF0u) << 2;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine += 2u;
	animation = enemy_foot_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a gun, axe or knife soldier on a rocket skateboard, by its parameter */
void enemy_skateboard_soldier_start(uint32_t object, uint32_t player_slot)
{
	uint16_t variant_offset = 0;
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy.target = player_slot;
	OBJECT(object)->enemy.retarget_time = 0x10u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	OBJECT(object)->enemy.dodge_chance = 5u;
	OBJECT(object)->depth = 3u;
	enemies_on_screen++;
	variant_offset = OBJECT(object)->parameter;
	OBJECT(object)->colour = skateboard_soldier_colours[variant_offset];
	variant_offset = (uint16_t)((uint32_t)variant_offset << 2);
	OBJECT(object)->enemy.definition = skateboard_soldier_definitions[variant_offset / 4u];
	animation = enemy_skateboard_animations[variant_offset / 4u];
	set_animation(0u, object, animation);
	OBJECT(object)->health_bars = 3u;
	OBJECT(object)->health = 6u;
	OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->routine += 2u;
}

/* the thinking, carried along with the ride, standing on the board */
void enemy_skateboard_think(uint32_t object, uint32_t second_slot)
{
	enemy_think(object, second_slot);
	OBJECT(object)->x += 0x600u;
	enemy_skateboard_stand(object);
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
	{
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
		return;
	}
	OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* the attack, carried along with the ride */
void enemy_skateboard_attack(uint32_t object)
{
	enemy_attack(object);
	OBJECT(object)->x += 0x600u;
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
	{
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
		return;
	}
	OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* the hit, carried along with the ride */
void enemy_skateboard_hit(uint32_t object)
{
	enemy_hit(object);
	OBJECT(object)->x += 0x600u;
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
	{
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
		return;
	}
	OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* the reaction to a hit, carried along with the ride */
void enemy_skateboard_react(uint32_t object)
{
	OBJECT(object)->x += 0x600u;
	enemy_react(object);
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
	{
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
		return;
	}
	OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* its standing pose, then the board with its flickering flame under it */
void enemy_skateboard_stand(uint32_t object)
{
	uint32_t animation = 0;

	animation = enemy_skateboard_animations[OBJECT(object)->parameter];
	set_animation(0u, object, animation);
}

/* throws a shuriken */
void enemy_throw_shuriken(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_throw_shuriken_states_ADDRESS;
	run_object_state(object, states);
}

/* faces its turtle with the throw's animation; the shuriken goes in 20 frames */
void enemy_throw_shuriken_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_throw.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_throw.throw_time = 0x14u;
	OBJECT(object)->state++;
	animation = enemy_throw_shuriken_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after 20 frames the shuriken flies, with its sound */
void enemy_throw_shuriken_release(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t ignored = 0, record = 0;
	uint8_t throw_time = 0;

	throw_time = OBJECT(object)->enemy_throw.throw_time;
	OBJECT(object)->enemy_throw.throw_time = throw_time - 1u;
	if (throw_time != 1u)
		return;
	OBJECT(object)->state++;
	sound = 0x2Au;
	queue_sound(sound);
	object_loop_leftover = shuriken_shot_record_ADDRESS;
	record = shuriken_shot_record_ADDRESS;
	start_shot(object, record, &ignored, &zero);
}

/* throws a boomerang and waits for it to come back */
void enemy_boomerang_throw(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_boomerang_throw_states_ADDRESS;
	run_object_state(object, states);
}

/* winds up for the boomerang throw */
void enemy_boomerang_throw_wind_up(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_boomerang_throw_wind_up_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the wind-up ends, faces its turtle; the boomerang goes in 16 frames */
void enemy_boomerang_throw_aim(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_boomerang.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_boomerang.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_boomerang_throw_aim_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after 16 frames the boomerang flies with its sound, knowing its thrower */
void enemy_boomerang_throw_release(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t boomerang = 0, record = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_boomerang.move_time;
	OBJECT(object)->enemy_boomerang.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	sound = 0x2Au;
	queue_sound(sound);
	object_loop_leftover = boomerang_shot_record_ADDRESS;
	record = boomerang_shot_record_ADDRESS;
	start_shot(object, record, &boomerang, &zero);
	if (zero)
	{
		OBJECT(boomerang)->boomerang.thrower = object;
		OBJECT(object)->enemy_boomerang.boomerang = boomerang;
	}
	OBJECT(object)->enemy_boomerang.move_time = 0x3Cu;
	OBJECT(object)->state++;
}

/* waits for its boomerang: back to thinking if a turtle breaks it, a pose after 60 frames */
void enemy_boomerang_throw_wait(uint32_t object)
{
	uint32_t animation = 0, boomerang = 0;
	uint8_t move_time = 0;

	boomerang = OBJECT(object)->enemy_boomerang.boomerang;
	if (OBJECT(boomerang)->routine == 4u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_boomerang.move_time = 0u;
		return;
	}
	move_time = OBJECT(object)->enemy_boomerang.move_time;
	OBJECT(object)->enemy_boomerang.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->state++;
	animation = enemy_boomerang_throw_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Rocksteady comes in alone or beside Bebop, counted as a boss */
void enemy_rocksteady_start(uint32_t object, uint32_t target)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	target = enemy_nearest_turtles(object, target);
	OBJECT(object)->enemy_rocksteady.target = target;
	OBJECT(object)->enemy_rocksteady.retarget_time = 0x10u;
	OBJECT(object)->enemy_rocksteady.move_time = 0u;
	OBJECT(object)->enemy_rocksteady.armed = 0u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->enemy_rocksteady.mode_time = 0x12Cu;
	OBJECT(object)->enemy_rocksteady.close_attack_wait = 0x10u;
	OBJECT(object)->enemy_rocksteady.far_attack_wait = 0x10u;
	enemies_on_screen++;
	bosses_left++;
	rocksteady_hits_to_strike_back = rocksteady_hits_to_strike_back_by_difficulty[difficulty & 7u];
	OBJECT(object)->shadow = big_boss_shadow_ADDRESS;
	if (OBJECT(object)->parameter != 0u)
	{
		OBJECT(object)->parameter = 0xAu;
		rocksteady = object;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 4u;
		OBJECT(object)->flags |= 0x80u;
		animation = enemy_rocksteady_raise_fist_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->parameter = 0xCu;
	sound = 0xFEu;
	music = 0xFEu;
	queue_sound(sound);
	OBJECT(object)->enemy_rocksteady.timer = 0xAu;
	OBJECT(object)->routine++;
	OBJECT(object)->flags |= 0x80u;
	animation = enemy_rocksteady_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* his way in: a laugh and his taunt, or waiting for Bebop, then walking in */
void enemy_rocksteady_enter(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_enter_states_ADDRESS;
	run_object_state(object, states);
}

/* waits a moment, then his laugh */
void enemy_rocksteady_enter_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_rocksteady.timer;
	OBJECT(object)->enemy_rocksteady.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->enemy_rocksteady.timer = 0x32u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_enter_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after the laugh, the boss music and his taunt in a balloon, on his row, laughing on */
void enemy_rocksteady_enter_taunt(uint16_t state_offset, uint32_t object)
{
	uint8_t sound = 0;
	uint16_t screen_y = 0;
	uint32_t animation = 0, screen_x = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_rocksteady.timer;
	OBJECT(object)->enemy_rocksteady.timer = timer - 1u;
	if (timer != 1u)
		return;
	sound = 0x64u;
	music = 0x64u;
	queue_sound(sound);
	state_offset = (uint16_t)(state_offset | 0xFFu);
	screen_x = (uint16_t)(OBJECT(object)->screen_x - 0x10u);
	screen_y = OBJECT(object)->screen_y;
	say_your_prayers_toitles(state_offset, (uint16_t)screen_x, screen_y);
	OBJECT(object)->y = 0x6C00u;
	OBJECT(object)->enemy_rocksteady.timer = 0x78u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_laugh_animation_ADDRESS;
	set_animation(screen_x, object, animation);
}

/* after the taunt, his health by the difficulty and his walk */
void enemy_rocksteady_enter_ready(uint32_t object)
{
	uint32_t animation = 0, settings = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_rocksteady.timer;
	OBJECT(object)->enemy_rocksteady.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->state++;
	settings = rocksteady_health_area_2_ADDRESS;
	enemy_set_health(object, settings);
	OBJECT(object)->enemy_rocksteady.timer = 0x10u;
	animation = enemy_rocksteady_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks in for 16 frames, then can be hit and thinks */
void enemy_rocksteady_enter_walk_in(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_rocksteady.timer;
	OBJECT(object)->enemy_rocksteady.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
	OBJECT(object)->y -= 0x100u;
}

/* beside Bebop: waits for the signal for the bosses to come out, then walks */
void enemy_rocksteady_enter_wait_for_signal(uint32_t object)
{
	uint32_t animation = 0;

	if (bosses_come_out == 0u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rocksteady.timer = 0x20u;
	animation = enemy_rocksteady_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks in for 32 frames, then his health and palette, hittable, and he thinks */
void enemy_rocksteady_enter_walk_in_with_bebop(uint32_t object)
{
	uint16_t request = 0;
	uint32_t data = 0, settings = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_rocksteady.timer;
	OBJECT(object)->enemy_rocksteady.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags |= 2u;
		settings = rocksteady_health_ADDRESS;
		enemy_set_health(object, settings);
		data = rocksteady_palette_ADDRESS;
		request = 0x43Au;
		post_palette_request(request, data);
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
	OBJECT(object)->y -= 0x100u;
}

/* his thinking: the gun drawn or put away in turns, blows close by, shots from afar, the pincer charge
   with Bebop */
void enemy_rocksteady_think(uint32_t object, uint32_t target)
{
	uint8_t parameter = 0;
	uint16_t armed_offset = 0, count = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, move_table = 0, moves = 0, range = 0, roll = 0, turtle = 0, turtle_2 = 0, waits = 0;
	uint8_t retarget_time = 0, far_attack_wait = 0, close_attack_wait = 0, move_choice = 0, t1 = 0;
	uint16_t mode_time = 0, t2 = 0;

	parameter = OBJECT(object)->parameter;
	enemy_flash_when_weak(parameter, object);
	retarget_time = OBJECT(object)->enemy_rocksteady.retarget_time;
	OBJECT(object)->enemy_rocksteady.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_rocksteady.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			target = enemy_nearest_turtles(object, target);
			OBJECT(object)->enemy_rocksteady.target = target;
		}
	}
	turtle = 0x60000u | OBJECT(object)->enemy_rocksteady.target;
	turtle_2 = turtle;
	enemy_look_at_target(object, turtle_2, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	enemy_keep_on_screen(object);
	if (pincer_charge != 0u)
	{
		OBJECT(object)->enemy_rocksteady.target = pincer_charge_target;
		OBJECT(object)->enemy_rocksteady.move = 0x98u;
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		object_loop_slot = turtle;
		return;
	}
	mode_time = OBJECT(object)->enemy_rocksteady.mode_time;
	OBJECT(object)->enemy_rocksteady.mode_time = mode_time - 1u;
	if (mode_time != 1u)
	{
		if ((OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u) && OBJECT(object)->enemy_rocksteady.target_dy <= 8u)
		{
			if (OBJECT(object)->enemy_rocksteady.target_dx > 0x32u)
			{
				far_attack_wait = OBJECT(object)->enemy_rocksteady.far_attack_wait;
				OBJECT(object)->enemy_rocksteady.far_attack_wait = far_attack_wait - 1u;
				if (far_attack_wait == 1u && (OBJECT(object)->enemy_rocksteady.armed != 0u || rocksteady_last_attack != 0x88u))
				{
					moves = rocksteady_far_attacks_ADDRESS;
					waits = rocksteady_far_attack_waits_ADDRESS;
					enemy_rocksteady_pick_attack(object, moves, &range, &waits);
					object_loop_leftover = waits;
					OBJECT(object)->enemy_rocksteady.far_attack_wait = WAIT_TIME(object_loop_leftover + (int32_t)range);
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					object_loop_slot = turtle;
					return;
				}
			}
			else
			{
				close_attack_wait = OBJECT(object)->enemy_rocksteady.close_attack_wait;
				OBJECT(object)->enemy_rocksteady.close_attack_wait = close_attack_wait - 1u;
				if (close_attack_wait == 1u)
				{
					moves = rocksteady_close_attacks_ADDRESS;
					waits = rocksteady_close_attack_waits_ADDRESS;
					enemy_rocksteady_pick_attack(object, moves, &range, &waits);
					object_loop_leftover = waits;
					OBJECT(object)->enemy_rocksteady.close_attack_wait = WAIT_TIME(object_loop_leftover + (int32_t)range);
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					object_loop_slot = turtle;
					return;
				}
			}
		}
		if (OBJECT(object)->enemy_rocksteady.move_time == 0u)
		{
			move_table = rocksteady_move_choices_ADDRESS + (uint16_t)((uint32_t)OBJECT(object)->enemy_rocksteady.armed << 3);
			roll = 0x64u;
			roll = random_below((uint16_t)roll, object);
			count = 3u;
			for (;;)
			{
				move_choice = MOVE_CHOICE(move_table + ((int16_t)count + 4u));
				t1 = roll;
				roll = (uint8_t)(t1 - move_choice);
				if (t1 < move_choice)
					break;
				t2 = count - 1u;
				count = t2;
				if (t2 != 0xFFFFu)
					continue;
				roll = 0x64u;
				roll = random_below((uint16_t)roll, object);
				count = 3u;
			}
			OBJECT(object)->enemy_rocksteady.move = ((count & ~0xFFu) | MOVE_CHOICE(move_table + (int16_t)count)) << 2;
			OBJECT(object)->state = 0u;
		}
	}
	else
	{
		OBJECT(object)->enemy_rocksteady.armed ^= 1u;
		armed_offset = (uint16_t)((uint32_t)OBJECT(object)->enemy_rocksteady.armed << 2);
		OBJECT(object)->enemy_rocksteady.move = rocksteady_moves[armed_offset / 2u];
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady.mode_time = TABLE16(rocksteady_mode_frames, (uint32_t)(int16_t)rocksteady_mode_times[armed_offset / 2u] + (uint16_t)((uint32_t)difficulty << 2));
		object_loop_leftover = rocksteady_close_attack_waits_ADDRESS;
		OBJECT(object)->enemy_rocksteady.close_attack_wait = rocksteady_close_attack_waits[armed_offset];
		OBJECT(object)->enemy_rocksteady.far_attack_wait = rocksteady_far_attack_waits[armed_offset];
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_rocksteady.move)
	{
	case 20:
		object_loop_slot = turtle;
		enemy_shuffle(object);
		return;
	case 28:
		object_loop_slot = turtle;
		enemy_circle(object);
		return;
	case 36:
		object_loop_slot = turtle;
		enemy_charge(object);
		return;
	case 0:
		object_loop_slot = turtle;
		enemy_walk_to_target(object);
		return;
	case 4:
		object_loop_slot = turtle;
		enemy_walk_to_side(object);
		return;
	case 8:
		object_loop_slot = turtle;
		enemy_keep_distance(object);
		return;
	case 12:
		object_loop_slot = turtle;
		enemy_back_off(object);
		return;
	case 16:
		object_loop_slot = turtle;
		enemy_run_away(object);
		return;
	case 44:
		object_loop_slot = turtle;
		enemy_walk_across(object);
		return;
	case 48:
		object_loop_slot = turtle;
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		object_loop_slot = turtle;
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		object_loop_slot = turtle;
		enemy_walk_past_sword(object);
		return;
	case 32:
		object_loop_slot = turtle;
		enemy_back_off_sword(object);
		return;
	case 40:
		object_loop_slot = turtle;
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		object_loop_slot = turtle;
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		object_loop_slot = turtle;
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		object_loop_slot = turtle;
		enemy_stalk_hammer(object);
		return;
	case 64:
		object_loop_slot = turtle;
		enemy_walk_away_hammer(object);
		return;
	case 72:
		object_loop_slot = turtle;
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		object_loop_slot = turtle;
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		object_loop_slot = turtle;
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		object_loop_slot = turtle;
		enemy_back_off_spear(object);
		return;
	case 112:
		object_loop_slot = turtle;
		enemy_step_aside_spear(object);
		return;
	case 80:
		object_loop_slot = turtle;
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		object_loop_slot = turtle;
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		object_loop_slot = turtle;
		enemy_circle_knife(object);
		return;
	case 124:
		object_loop_slot = turtle;
		enemy_keep_distance_knife(object);
		return;
	case 120:
		object_loop_slot = turtle;
		enemy_back_off_knife(object);
		return;
	case 128:
		object_loop_slot = turtle;
		enemy_step_aside_knife(object);
		return;
	case 88:
		object_loop_slot = turtle;
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		object_loop_slot = turtle;
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		object_loop_slot = turtle;
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		object_loop_slot = turtle;
		enemy_back_off_gun(object);
		return;
	case 144:
		object_loop_slot = turtle;
		enemy_run_away_gun(object);
		return;
	case 96:
		object_loop_slot = turtle;
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		object_loop_slot = turtle;
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		object_loop_slot = turtle;
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		object_loop_slot = turtle;
		enemy_back_off_axe(object);
		return;
	case 136:
		object_loop_slot = turtle;
		enemy_run_away_axe(object);
		return;
	case 196:
		object_loop_slot = turtle;
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		object_loop_slot = turtle;
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		object_loop_slot = turtle;
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		object_loop_slot = turtle;
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		object_loop_slot = turtle;
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		object_loop_slot = turtle;
		enemy_rocksteady_circle(object);
		return;
	case 208:
		object_loop_slot = turtle;
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		object_loop_slot = turtle;
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		object_loop_slot = turtle;
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		object_loop_slot = turtle;
		enemy_bebop_stand(object);
		return;
	case 216:
		object_loop_slot = turtle;
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		object_loop_slot = turtle;
		enemy_bebop_walk_level(object);
		return;
	case 220:
		object_loop_slot = turtle;
		enemy_bebop_close_in(object);
		return;
	case 156:
		object_loop_slot = turtle;
		enemy_mouser_chase(object);
		return;
	case 164:
		object_loop_slot = turtle;
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		object_loop_slot = turtle;
		enemy_mouser_back_off(object);
		return;
	case 172:
		object_loop_slot = turtle;
		enemy_rodney_rush(object);
		return;
	case 176:
		object_loop_slot = turtle;
		enemy_rodney_rush_across(object);
		return;
	case 180:
		object_loop_slot = turtle;
		enemy_rodney_back_off(object);
		return;
	case 236:
		object_loop_slot = turtle;
		enemy_shredder_appear(object);
		return;
	case 240:
		object_loop_slot = turtle;
		enemy_shredder_split(object);
		return;
	case 168:
		object_loop_slot = turtle;
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		object_loop_slot = turtle;
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		object_loop_slot = turtle;
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		object_loop_slot = turtle;
		enemy_shredder_guard(object);
		return;
	case 256:
		object_loop_slot = turtle;
		enemy_shredder_back_off(object);
		return;
	}
}

/* picks his attack by whether he has the gun out */
void enemy_rocksteady_pick_attack(uint32_t object, uint32_t moves, uint32_t *p_range, uint32_t *p_attack_waits)
{
	uint16_t armed_offset = 0;
	uint32_t range = 0;

	armed_offset = (uint16_t)((uint32_t)OBJECT(object)->enemy_rocksteady.armed << 1);
	OBJECT(object)->enemy_rocksteady.move = ATTACK_MOVE(moves + (armed_offset / 2u) * 0x2u);
	rocksteady_last_attack = OBJECT(object)->enemy_rocksteady.move;
	*p_attack_waits += (uint16_t)((uint32_t)armed_offset << 1);
	range = 4u;
	range = random_below((uint16_t)range, object);
	*p_range = range;
}

/* his attack, flashing when weak */
void enemy_rocksteady_attack(uint32_t object)
{
	uint8_t colour = 0;

	colour = OBJECT(object)->parameter;
	enemy_flash_when_weak(colour, object);
	enemy_boss_attack_move(object);
}

/* Rocksteady's and Bebop's attack by their move */
void enemy_boss_attack_move(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, turtle = 0, turtle_2 = 0;
	uint32_t jump_target = 0;

	turtle = 0x60000u | OBJECT(object)->enemy.target;
	turtle_2 = turtle;
	enemy_look_at_target(object, turtle_2, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	enemy_keep_on_screen(object);
	jump_target = TABLE32(enemy_attacks, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	switch (jump_target)
	{
	case enemy_jab_ADDRESS:
		object_loop_slot = turtle;
		enemy_jab(object);
		return;
	case enemy_chop_ADDRESS:
		object_loop_slot = turtle;
		enemy_chop(object);
		return;
	case enemy_swing_ADDRESS:
		object_loop_slot = turtle;
		enemy_swing(object);
		return;
	case enemy_high_kick_ADDRESS:
		object_loop_slot = turtle;
		enemy_high_kick(object);
		return;
	case enemy_jump_kick_ADDRESS:
		object_loop_slot = turtle;
		enemy_jump_kick(object);
		return;
	case enemy_long_jump_kick_ADDRESS:
		object_loop_slot = turtle;
		enemy_long_jump_kick(object);
		return;
	case enemy_attack_none_after_long_jump_kick_ADDRESS:
		object_loop_slot = turtle;
		enemy_attack_none_after_long_jump_kick();
		return;
	case enemy_attack_none_before_grab_ADDRESS:
		object_loop_slot = turtle;
		enemy_attack_none_before_grab();
		return;
	case enemy_grab_ADDRESS:
		object_loop_slot = turtle;
		enemy_grab(object);
		return;
	case enemy_slash_sword_ADDRESS:
		object_loop_slot = turtle;
		enemy_slash_sword(object);
		return;
	case enemy_low_slash_sword_ADDRESS:
		object_loop_slot = turtle;
		enemy_low_slash_sword(object);
		return;
	case enemy_leaping_slash_sword_ADDRESS:
		object_loop_slot = turtle;
		enemy_leaping_slash_sword(object);
		return;
	case enemy_jump_slash_sword_ADDRESS:
		object_loop_slot = turtle;
		enemy_jump_slash_sword(object);
		return;
	case enemy_flying_slash_sword_ADDRESS:
		object_loop_slot = turtle;
		enemy_flying_slash_sword(object);
		return;
	case enemy_smash_hammer_ADDRESS:
		object_loop_slot = turtle;
		enemy_smash_hammer(object);
		return;
	case enemy_spear_thrust_ADDRESS:
		object_loop_slot = turtle;
		enemy_spear_thrust(object);
		return;
	case enemy_spear_throw_ADDRESS:
		object_loop_slot = turtle;
		enemy_spear_throw(object);
		return;
	case enemy_knife_stab_ADDRESS:
		object_loop_slot = turtle;
		enemy_knife_stab(object);
		return;
	case enemy_high_kick_knife_ADDRESS:
		object_loop_slot = turtle;
		enemy_high_kick_knife(object);
		return;
	case enemy_jump_kick_knife_ADDRESS:
		object_loop_slot = turtle;
		enemy_jump_kick_knife(object);
		return;
	case enemy_throw_knife_ADDRESS:
		object_loop_slot = turtle;
		enemy_throw_knife(object);
		return;
	case enemy_throw_axes_ADDRESS:
		object_loop_slot = turtle;
		enemy_throw_axes(object);
		return;
	case enemy_high_kick_axe_ADDRESS:
		object_loop_slot = turtle;
		enemy_high_kick_axe(object);
		return;
	case enemy_gun_swing_ADDRESS:
		object_loop_slot = turtle;
		enemy_gun_swing(object);
		return;
	case enemy_shoot_gun_ADDRESS:
		object_loop_slot = turtle;
		enemy_shoot_gun(object);
		return;
	case enemy_throw_shuriken_ADDRESS:
		object_loop_slot = turtle;
		enemy_throw_shuriken(object);
		return;
	case enemy_boomerang_throw_ADDRESS:
		object_loop_slot = turtle;
		enemy_boomerang_throw(object);
		return;
	case enemy_rocksteady_pincer_charge_ADDRESS:
		object_loop_slot = turtle;
		enemy_rocksteady_pincer_charge(object);
		return;
	case enemy_rocksteady_charge_ADDRESS:
		object_loop_slot = turtle;
		enemy_rocksteady_charge(object);
		return;
	case enemy_rocksteady_punch_ADDRESS:
		object_loop_slot = turtle;
		enemy_rocksteady_punch(object);
		return;
	case enemy_rocksteady_gun_butt_ADDRESS:
		object_loop_slot = turtle;
		enemy_rocksteady_gun_butt(object);
		return;
	case enemy_rocksteady_shoot_ADDRESS:
		object_loop_slot = turtle;
		enemy_rocksteady_shoot(object);
		return;
	case enemy_bebop_pincer_charge_ADDRESS:
		object_loop_slot = turtle;
		enemy_bebop_pincer_charge(object);
		return;
	case enemy_bebop_punch_ADDRESS:
		object_loop_slot = turtle;
		enemy_bebop_punch(object);
		return;
	case enemy_bebop_charge_ADDRESS:
		object_loop_slot = turtle;
		enemy_bebop_charge(object);
		return;
	case enemy_bebop_shoot_ADDRESS:
		object_loop_slot = turtle;
		enemy_bebop_shoot(object);
		return;
	case enemy_rodney_shoot_ADDRESS:
		object_loop_slot = turtle;
		enemy_rodney_shoot(object);
		return;
	case enemy_rodney_whip_ADDRESS:
		object_loop_slot = turtle;
		enemy_rodney_whip(object);
		return;
	case enemy_shredder_kick_ADDRESS:
		object_loop_slot = turtle;
		enemy_shredder_kick(object);
		return;
	case enemy_shredder_thrust_ADDRESS:
		object_loop_slot = turtle;
		enemy_shredder_thrust(object);
		return;
	case enemy_shredder_slash_ADDRESS:
		object_loop_slot = turtle;
		enemy_shredder_slash(object);
		return;
	case enemy_shredder_sweep_ADDRESS:
		object_loop_slot = turtle;
		enemy_shredder_sweep(object);
		return;
	case enemy_shredder_ray_ADDRESS:
		object_loop_slot = turtle;
		enemy_shredder_ray(object);
		return;
	}
}

/* hit: his reaction by what is left of his health and where the hit came from */
void enemy_rocksteady_hit(uint32_t object)
{
	uint8_t hit = 0, sound = 0;
	uint16_t facing_bit = 0, move = 0;

	rocksteady_hits_to_strike_back--;
	sound = 0x82u;
	queue_sound(sound);
	pincer_charge = 0u;
	move = 0x9Cu;
	if ((OBJECT(object)->health_bars | OBJECT(object)->health) != 0u)
	{
		move = 0x98u;
		if (OBJECT(object)->health != 0u)
		{
			facing_bit = OBJECT(object)->flags;
			hit = OBJECT(object)->hit;
			facing_bit = (uint8_t)((uint32_t)facing_bit << 1);
			if ((int8_t)(facing_bit ^ hit) >= 0)
				move = (uint16_t)((uint32_t)rocksteady_hit_reactions[hit & 0x7Fu] << 2);
			else
				move = 0xA0u;
		}
	}
	OBJECT(object)->enemy_rocksteady.move = move;
	OBJECT(object)->routine++;
	enemy_rocksteady_react(object);
}

/* his reaction to a hit, flashing when weak */
void enemy_rocksteady_react(uint32_t object)
{
	uint8_t colour = 0;

	colour = OBJECT(object)->parameter;
	enemy_flash_when_weak(colour, object);
	enemy_boss_react_move(object);
}

/* Rocksteady's and Bebop's reaction by their move */
void enemy_boss_react_move(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0;

	object_loop_slot = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored, &ignored_2, &ignored_3, &ignored_4);
	enemy_keep_on_screen(object);
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy.move)
	{
	case 228:
		enemy_crash_into_scenery(object);
		return;
	case 232:
		enemy_crash_into_scenery_low(object);
		return;
	case 0:
		enemy_flinch_back(object);
		return;
	case 4:
		enemy_flinch_bent(object);
		return;
	case 8:
		enemy_flinch_reel(object);
		return;
	case 12:
		enemy_flinch_hunched(object);
		return;
	case 36:
		enemy_killed_back(object);
		return;
	case 32:
		enemy_killed_forward(object);
		return;
	case 16:
		enemy_knocked_forward(object);
		return;
	case 128:
		enemy_thrown_by_donatello(object);
		return;
	case 132:
		enemy_let_go_by_donatello(object);
		return;
	case 136:
		enemy_thrown_by_michelangelo(object);
		return;
	case 140:
		enemy_let_go_by_michelangelo(object);
		return;
	case 188:
		enemy_thrown_by_raphael(object);
		return;
	case 192:
		enemy_let_go_by_raphael(object);
		return;
	case 196:
		enemy_thrown_by_leonardo(object);
		return;
	case 200:
		enemy_let_go_by_leonardo(object);
		return;
	case 20:
		enemy_knocked_back(object);
		return;
	case 24:
		enemy_die(object);
		return;
	case 28:
		enemy_die_on_back(object);
		return;
	case 40:
		enemy_flinch_back_sword(object);
		return;
	case 44:
		enemy_flinch_bent_sword(object);
		return;
	case 48:
		enemy_flinch_reel_sword(object);
		return;
	case 52:
		enemy_flinch_hunched_sword(object);
		return;
	case 56:
		enemy_flinch_back_hammer(object);
		return;
	case 60:
		enemy_flinch_bent_hammer(object);
		return;
	case 68:
		enemy_flinch_hunched_hammer(object);
		return;
	case 72:
		enemy_flinch_back_spear(object);
		return;
	case 76:
		enemy_flinch_bent_spear(object);
		return;
	case 80:
		enemy_flinch_reel_spear(object);
		return;
	case 84:
		enemy_flinch_hunched_spear(object);
		return;
	case 88:
		enemy_flinch_back_knife(object);
		return;
	case 92:
		enemy_flinch_bent_knife(object);
		return;
	case 96:
		enemy_flinch_reel_knife(object);
		return;
	case 100:
		enemy_flinch_hunched_knife(object);
		return;
	case 120:
		enemy_flinch_back_gun(object);
		return;
	case 124:
		enemy_flinch_bent_gun(object);
		return;
	case 104:
		enemy_flinch_back_axe(object);
		return;
	case 108:
		enemy_flinch_bent_axe(object);
		return;
	case 112:
		enemy_flinch_reel_axe(object);
		return;
	case 164:
		enemy_rocksteady_flinch(object);
		return;
	case 168:
		enemy_rocksteady_reel(object);
		return;
	case 160:
		enemy_rocksteady_flinch_from_behind(object);
		return;
	case 152:
		enemy_rocksteady_knocked_down(object);
		return;
	case 156:
		enemy_rocksteady_defeated(object);
		return;
	case 172:
		enemy_bebop_flinch(object);
		return;
	case 176:
		enemy_bebop_flinch_from_behind(object);
		return;
	case 180:
		enemy_bebop_knocked_down(object);
		return;
	case 184:
		enemy_bebop_defeated(object);
		return;
	case 148:
		enemy_mouser_thrown_off(object);
		return;
	case 224:
		enemy_mouser_break_down(object);
		return;
	case 144:
		enemy_mouser_knocked_flying(object);
		return;
	case 64:
		enemy_rodney_knocked_back(object);
		return;
	case 116:
		enemy_rodney_destroyed(object);
		return;
	case 204:
		enemy_shredder_lose_bar(object);
		return;
	case 208:
		enemy_shredder_flinch(object);
		return;
	case 212:
		enemy_shredder_flinch_from_behind(object);
		return;
	case 216:
		enemy_shredder_defeated(object);
		return;
	case 220:
		enemy_shredder_vanish(object);
		return;
	}
}

/* Rocksteady put into a free object for his fight, at his place */
void start_rocksteady(void)
{
	uint32_t zero = 0;
	uint8_t group = 0;
	uint32_t object = 0;

	group = 0u;
	take_free_object_in_group(group, &object, &zero);
	if (zero)
	{
		OBJECT(object)->flags = 1u;
		OBJECT(object)->contacts = 0u;
		OBJECT(object)->kind = 0x11u;
		OBJECT(object)->parameter = 0u;
		OBJECT(object)->routine = 0u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->colour = 0xFFu;
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->throw = 0u;
		OBJECT(object)->height = 0u;
		OBJECT(object)->x = 0x16800u;
		OBJECT(object)->y = 0x11400u;
	}
}

/* raises his fist at his turtle */
void enemy_rocksteady_raise_fist(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_raise_fist_states_ADDRESS;
	run_object_state(object, states);
}

/* raises his fist at his turtle */
void enemy_rocksteady_raise_fist_start(uint32_t object)
{
	uint32_t animation = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 1u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_raise_fist_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* laughs at his turtle */
void enemy_rocksteady_laugh(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_laugh_states_ADDRESS;
	run_object_state(object, states);
}

/* laughs at his turtle for 136 frames */
void enemy_rocksteady_laugh_start(uint32_t object)
{
	uint32_t animation = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0x88u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_laugh_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* shakes his fist at his turtle */
void enemy_rocksteady_shake_fist(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_shake_fist_states_ADDRESS;
	run_object_state(object, states);
}

/* shakes his fist at his turtle for 64 frames */
void enemy_rocksteady_shake_fist_start(uint32_t object)
{
	uint32_t animation = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0x40u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_shake_fist_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* holds the pose until his move's time runs out */
void enemy_rocksteady_hold_pose(uint32_t object)
{
	OBJECT(object)->enemy_rocksteady.move_time--;
}

/* draws his gun */
void enemy_rocksteady_draw_gun(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_draw_gun_states_ADDRESS;
	run_object_state(object, states);
}

/* draws his gun, facing his turtle */
void enemy_rocksteady_draw_gun_start(uint32_t object)
{
	uint32_t animation = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0xFFu;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_draw_gun_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* the move is over when its animation ends */
void enemy_rocksteady_pose_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
		OBJECT(object)->enemy_rocksteady.move_time = 0u;
}

/* puts his gun away */
void enemy_rocksteady_put_gun_away(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_put_gun_away_states_ADDRESS;
	run_object_state(object, states);
}

/* puts his gun away, facing his turtle */
void enemy_rocksteady_put_gun_away_start(uint32_t object)
{
	uint32_t animation = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0xFFu;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_put_gun_away_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks round his turtle */
void enemy_rocksteady_circle(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_circle_states_ADDRESS;
	run_object_state(object, states);
}

/* 180 frames of walking round his turtle, one way or the other at random */
void enemy_rocksteady_circle_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_rocksteady_circle.move_time = 0xB4u;
	OBJECT(object)->enemy_rocksteady_circle.aim_time = 1u;
	OBJECT(object)->enemy_rocksteady_circle.turn = 0u;
	if ((random_seed & 0x80u) != 0u)
		OBJECT(object)->enemy_rocksteady_circle.turn = 0x40u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_rocksteady_circle_step(object);
}

/* a frame of the walk round his turtle, aiming again every 8 frames */
void enemy_rocksteady_circle_step(uint32_t object)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t target = 0, x_speed = 0, y_speed = 0;
	uint8_t aim_time = 0;

	OBJECT(object)->enemy_rocksteady_circle.move_time--;
	aim_time = OBJECT(object)->enemy_rocksteady_circle.aim_time;
	OBJECT(object)->enemy_rocksteady_circle.aim_time = aim_time - 1u;
	if (aim_time == 1u)
	{
		OBJECT(object)->enemy_rocksteady_circle.aim_time = 8u;
		target = object_loop_slot;
		enemy_boss_face_target(object, target);
		direction = (uint8_t)((OBJECT(object)->enemy_rocksteady_circle.target_direction & 0xC0u) + OBJECT(object)->enemy_rocksteady_circle.turn);
		speed = 0x160u;
		speeds_from_direction(direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = y_speed;
	}
	move_object_blocked(object);
}

/* lines up with his turtle with the gun up */
void enemy_rocksteady_line_up(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_line_up_states_ADDRESS;
	run_object_state(object, states);
}

/* with the gun up, 240 frames of lining up with his turtle */
void enemy_rocksteady_line_up_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_rocksteady.move_time = 0xF0u;
	OBJECT(object)->enemy_rocksteady.timer = 1u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_line_up_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_rocksteady_line_up_step(object);
}

/* a frame of lining up: away across while nearer than 0x50, then up or down to his turtle's row */
void enemy_rocksteady_line_up_step(uint32_t object)
{
	uint32_t target = 0;

	OBJECT(object)->enemy_rocksteady.move_time--;
	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	if (OBJECT(object)->enemy_rocksteady.target_dx < 0x50u)
	{
		OBJECT(object)->x_speed = 0x200;
		OBJECT(object)->y_speed = 0;
		if ((int8_t)(OBJECT(object)->enemy_rocksteady.target_direction - 0x40u) < 0)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		move_object_blocked(object);
		return;
	}
	if (OBJECT(object)->enemy_rocksteady.target_dy < 3u)
		return;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x200;
	if ((int8_t)OBJECT(object)->enemy_rocksteady.target_direction < 0)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	move_object_blocked(object);
}

/* walks up to his turtle with the gun level */
void enemy_rocksteady_close_in(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_close_in_states_ADDRESS;
	run_object_state(object, states);
}

/* with the gun level, 128 frames of closing in on his turtle */
void enemy_rocksteady_close_in_start(uint32_t object)
{
	uint32_t animation = 0, turtle = 0;

	OBJECT(object)->enemy_rocksteady_close_in.move_time = 0x80u;
	OBJECT(object)->enemy_rocksteady_close_in.aim_time = 1u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_gun_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	turtle = object_loop_slot;
	enemy_rocksteady_close_in_step(object, turtle);
}

/* a frame of closing in, aiming every 8 frames at a place just in front of his turtle */
void enemy_rocksteady_close_in_step(uint32_t object, uint32_t turtle)
{
	uint16_t speed = 0, target_x = 0, target_y = 0;
	uint32_t direction = 0, x_speed = 0, y_speed = 0;
	uint8_t aim_time = 0;

	OBJECT(object)->enemy_rocksteady_close_in.move_time--;
	aim_time = OBJECT(object)->enemy_rocksteady_close_in.aim_time;
	OBJECT(object)->enemy_rocksteady_close_in.aim_time = aim_time - 1u;
	if (aim_time != 1u)
	{
		move_object_blocked(object);
		return;
	}
	enemy_boss_face_target(object, turtle);
	target_x = 0x20u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		target_x = (uint16_t)(0u - target_x);
	target_x += OBJECT(turtle)->screen_x;
	target_y = OBJECT(turtle)->screen_y;
	direction = direction_to(0u, target_x, target_y, object);
	speed = 0x180u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = (int16_t)x_speed >> 1;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->enemy_rocksteady_close_in.aim_time = 8u;
	move_object_blocked(object);
}

/* looks at its turtle and turns to it */
void enemy_boss_face_target(uint32_t object, uint32_t target)
{
	uint32_t ignored = 0, ignored_2 = 0, turtle = 0;

	turtle = target;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
}

/* fires three shots each way round */
void enemy_rocksteady_shoot_around(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_shoot_around_states_ADDRESS;
	run_object_state(object, states);
}

/* six aims to go round, three shots each */
void enemy_rocksteady_shoot_around_start(uint32_t object)
{
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rocksteady_shoot_around.aims = rocksteady_shoot_around_aims_ADDRESS;
	OBJECT(object)->enemy_rocksteady_shoot_around.move_time = 7u;
	OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = 1u;
	OBJECT(object)->enemy_rocksteady_shoot_around.shots_left = 1u;
	enemy_rocksteady_shoot_around_fire(object);
}

/* every 8 frames a bullet with its sound; after three, the next aim with its pose */
void enemy_rocksteady_shoot_around_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint16_t aim = 0, aim_number = 0;
	uint32_t aims = 0, animation = 0, object_2 = 0, record = 0;
	uint8_t shot_time = 0, shots_left = 0, move_time = 0;
	uint32_t t1 = 0;

	shot_time = OBJECT(object)->enemy_rocksteady_shoot_around.shot_time;
	OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = shot_time - 1u;
	if (shot_time != 1u)
		return;
	shots_left = OBJECT(object)->enemy_rocksteady_shoot_around.shots_left;
	OBJECT(object)->enemy_rocksteady_shoot_around.shots_left = shots_left - 1u;
	if (shots_left != 1u)
	{
		OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = 8u;
		sound = 0x18u;
		queue_sound(sound);
		object_loop_leftover = OBJECT(object)->enemy_rocksteady_shoot_around.bullet;
		record = object_loop_leftover;
		start_shot(object, record, &object_2, &zero);
		if (zero)
			OBJECT(object_2)->parameter = OBJECT(object)->enemy_rocksteady_shoot_around.aim;
		return;
	}
	move_time = OBJECT(object)->enemy_rocksteady_shoot_around.move_time;
	OBJECT(object)->enemy_rocksteady_shoot_around.move_time = move_time - 1u;
	if (move_time == 1u)
		return;
	OBJECT(object)->enemy_rocksteady_shoot_around.shots_left = 3u;
	aims = OBJECT(object)->enemy_rocksteady_shoot_around.aims;
	aim = AIM(aims);
	OBJECT(object)->enemy_rocksteady_shoot_around.aims = aims + 1u;
	OBJECT(object)->flags &= 0xBFu;
	t1 = aim;
	aim_number = (uint16_t)(t1 & ~0x80u);
	if ((t1 & 0x80u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_rocksteady_shoot_around.aim = 0u;
	if (aim_number != 0u)
		OBJECT(object)->enemy_rocksteady_shoot_around.aim = 1u;
	aim_number = (uint16_t)((uint32_t)aim_number << 3);
	OBJECT(object)->enemy_rocksteady_shoot_around.bullet = rocksteady_bursts[aim_number / 4u];
	animation = enemy_rocksteady_shoot_around_animations[aim_number / 4u];
	set_animation(0u, object, animation);
	OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = 8u;
	sound = 0x18u;
	queue_sound(sound);
	object_loop_leftover = OBJECT(object)->enemy_rocksteady_shoot_around.bullet;
	record = object_loop_leftover;
	start_shot(object, record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->enemy_rocksteady_shoot_around.aim;
}

/* charges across from his edge while Bebop charges from the other */
void enemy_rocksteady_pincer_charge(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_pincer_charge_states_ADDRESS;
	run_object_state(object, states);
	if (pincer_charge == 0u && pincer_charge_crash == 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady.move_time = 0u;
	}
}

/* the pincer charge with Bebop: gun out or not, he runs to his edge of the screen */
void enemy_rocksteady_pincer_charge_start(uint32_t object)
{
	uint32_t animation = 0;

	animation = enemy_rocksteady_walk_animation_ADDRESS;
	if (OBJECT(object)->enemy_rocksteady.armed != 0u)
		animation = enemy_rocksteady_gun_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_pincer_charge_take_edge(object);
}

/* runs to his edge; at the wall he turns round and marks his edge taken */
void enemy_rocksteady_pincer_charge_run_to_edge(uint32_t object)
{
	uint8_t wall_mask = 0;

	move_object_blocked(object);
	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->flags ^= 0x40u;
		pincer_charge_at_edges |= OBJECT(object)->enemy_rocksteady.side;
		OBJECT(object)->state++;
	}
}

/* waits until Bebop holds the other edge, then readies the charge, no longer hittable */
void enemy_rocksteady_pincer_charge_wait_for_bebop(uint32_t object)
{
	uint32_t animation = 0;

	if (pincer_charge_at_edges < 0xC0u)
		return;
	animation = enemy_rocksteady_charge_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	OBJECT(object)->attack = 7u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->attack |= 0x80u;
	OBJECT(object)->flags &= ~0xAu;
	OBJECT(object)->enemy_rocksteady.move_time = 0x1Eu;
	OBJECT(object)->state++;
}

/* 30 frames, then the charge across */
void enemy_rocksteady_pincer_charge_get_ready(uint32_t object)
{
	enemy_rocksteady_charge_get_ready(object);
}

/* a frame of the charge, following his turtle's row; at the wall it ends, crashing into Bebop a fall */
void enemy_rocksteady_pincer_charge_run(uint32_t object)
{
	uint8_t wall_mask = 0;
	uint32_t animation = 0, y_speed = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		pincer_charge = 0u;
		return;
	}
	y_speed = 0u;
	if (OBJECT(object)->enemy_rocksteady.target_dy >= 3u)
	{
		y_speed = 0x200u;
		if ((int8_t)OBJECT(object)->enemy_rocksteady.target_direction >= 0)
			y_speed = (uint16_t)(0u - y_speed);
	}
	OBJECT(object)->y_speed = y_speed;
	move_object_blocked(object);
	if (pincer_charge_crash != 0u)
	{
		animation = enemy_rocksteady_pincer_charge_run_animation_ADDRESS;
		set_animation(y_speed, object, animation);
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->state++;
	}
}

/* when the crash into Bebop ends, he is knocked down */
void enemy_rocksteady_pincer_charge_crash(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_rocksteady.move = 0x98u;
		OBJECT(object)->routine = 5u;
		OBJECT(object)->state = 0u;
	}
}

/* charges across the screen */
void enemy_rocksteady_charge(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_charge_states_ADDRESS;
	run_object_state(object, states);
}

/* the charge's animation and damage, facing his turtle, not hittable; 30 frames to get ready */
void enemy_rocksteady_charge_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_rocksteady_charge_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags &= ~0xAu;
	OBJECT(object)->enemy_rocksteady.move_time = 0x1Eu;
	OBJECT(object)->state++;
}

/* after 30 frames, charges across at full speed, attacking */
void enemy_rocksteady_charge_get_ready(uint32_t object)
{
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_rocksteady.move_time;
	OBJECT(object)->enemy_rocksteady.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->x_speed = -0x380;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->state++;
}

/* a frame of the charge, following his turtle's row; at the wall, hittable again and back to thinking */
void enemy_rocksteady_charge_run(uint32_t object)
{
	uint8_t contacts = 0;
	uint16_t y_speed = 0;

	contacts = OBJECT(object)->contacts;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		if ((contacts & 8u) != 0u)
		{
			OBJECT(object)->flags &= ~4u;
			OBJECT(object)->flags |= 2u;
			OBJECT(object)->routine = 2u;
			OBJECT(object)->state = 0u;
			OBJECT(object)->enemy_rocksteady.move_time = 0u;
			return;
		}
		y_speed = 0u;
		if (OBJECT(object)->enemy_rocksteady.target_dy >= 3u)
		{
			y_speed = 0x200u;
			if ((int8_t)OBJECT(object)->enemy_rocksteady.target_direction >= 0)
				y_speed = (uint16_t)(0u - y_speed);
		}
		OBJECT(object)->y_speed = y_speed;
		move_object_blocked(object);
		return;
	}
	if ((contacts & 0x10u) != 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady.move_time = 0u;
		return;
	}
	y_speed = 0u;
	if (OBJECT(object)->enemy_rocksteady.target_dy >= 3u)
	{
		y_speed = 0x200u;
		if ((int8_t)OBJECT(object)->enemy_rocksteady.target_direction >= 0)
			y_speed = (uint16_t)(0u - y_speed);
	}
	OBJECT(object)->y_speed = y_speed;
	move_object_blocked(object);
}

/* punches */
void enemy_rocksteady_punch(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_punch_states_ADDRESS;
	run_object_state(object, states);
}

/* the punch's animation and damage, facing his turtle; hittable, unless he was not and two or more
   turtles play */
void enemy_rocksteady_punch_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;
	uint8_t flags = 0;

	animation = enemy_rocksteady_punch_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x203u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	flags = OBJECT(object)->flags;
	OBJECT(object)->flags = flags | 2u;
	if ((flags & 2u) == 0u && turtles_in_play >= 2u)
		OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
}

/* the blow over, back to thinking */
void enemy_rocksteady_blow_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady.move_time = 0u;
	}
}

/* hits with the gun's butt */
void enemy_rocksteady_gun_butt(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_gun_butt_states_ADDRESS;
	run_object_state(object, states);
}

/* a blow with the gun's butt: its animation and damage, facing his turtle */
void enemy_rocksteady_gun_butt_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_rocksteady_gun_butt_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x203u;
	attack = 0u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* shoots, up at a turtle in the air */
void enemy_rocksteady_shoot(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_shoot_states_ADDRESS;
	run_object_state(object, states);
}

/* four shots at his turtle, level, or up at one high in the air */
void enemy_rocksteady_shoot_start(uint32_t object, uint32_t turtle)
{
	uint32_t animation_entry = 0, animations = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rocksteady_shoot.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_rocksteady_shoot.shot_time = 1u;
	OBJECT(object)->enemy_rocksteady_shoot.shots_left = 4u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rocksteady_shoot.aim = 0u;
	animations = rocksteady_shoot_animations_ADDRESS;
	if (OBJECT(turtle)->height_whole >= 0x40u)
	{
		OBJECT(object)->enemy_rocksteady_shoot.aim = 1u;
		animations += 8u;
	}
	animation_entry = ANIMATION_ENTRY(animations);
	animations += 4u;
	OBJECT(object)->enemy_rocksteady_shoot.bullet = ANIMATION_ENTRY(animations);
	object_loop_leftover = animations;
	set_animation(0u, object, animation_entry);
}

/* every 8 frames a bullet with its sound; after the fourth, back to thinking */
void enemy_rocksteady_shoot_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t object_2 = 0, record = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_rocksteady_shoot.shot_time;
	OBJECT(object)->enemy_rocksteady_shoot.shot_time = shot_time - 1u;
	if (shot_time != 1u)
		return;
	sound = 0x18u;
	queue_sound(sound);
	object_loop_leftover = OBJECT(object)->enemy_rocksteady_shoot.bullet;
	record = object_loop_leftover;
	start_shot(object, record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->enemy_rocksteady_shoot.aim;
	OBJECT(object)->enemy_rocksteady_shoot.shot_time = 8u;
	shots_left = OBJECT(object)->enemy_rocksteady_shoot.shots_left;
	OBJECT(object)->enemy_rocksteady_shoot.shots_left = shots_left - 1u;
	if (shots_left == 1u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady_shoot.move_time = 0u;
	}
}

/* hit from the front */
void enemy_rocksteady_flinch(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_flinch_states_ADDRESS;
	run_object_state(object, states);
}

/* his flinch, gun out or not */
void enemy_rocksteady_flinch_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_rocksteady_flinch_animations[OBJECT(object)->enemy_rocksteady.armed];
	set_animation(0u, object, animation);
}

/* back to thinking, or striking back at once when he has taken enough hits */
void enemy_rocksteady_flinch_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady.move_time = 0u;
		if (rocksteady_hits_to_strike_back == 0u)
		{
			OBJECT(object)->flags &= ~2u;
			OBJECT(object)->enemy_rocksteady.close_attack_wait = 1u;
		}
	}
}

/* hit from the front, the other way */
void enemy_rocksteady_reel(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_reel_states_ADDRESS;
	run_object_state(object, states);
}

/* his reel, gun out or not */
void enemy_rocksteady_reel_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_rocksteady_reel_animations[OBJECT(object)->enemy_rocksteady.armed];
	set_animation(0u, object, animation);
}

/* hit from behind */
void enemy_rocksteady_flinch_from_behind(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_flinch_from_behind_states_ADDRESS;
	run_object_state(object, states);
}

/* his flinch from a hit from behind, gun out or not */
void enemy_rocksteady_flinch_from_behind_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_rocksteady_flinch_from_behind_animations[OBJECT(object)->enemy_rocksteady.armed];
	set_animation(0u, object, animation);
}

/* knocked down, lies, gets up with his health back */
void enemy_rocksteady_knocked_down(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_knocked_down_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked down along his arc, gun out or not, facing his turtle */
void enemy_rocksteady_knocked_down_start(uint32_t object)
{
	uint32_t animation_entry = 0, animations = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	animations = rocksteady_knocked_down_animations_2_ADDRESS;
	enemy_start_arc(object, animations);
	animations = rocksteady_knocked_down_animations_ADDRESS;
	if (OBJECT(object)->enemy_rocksteady_knocked_down.armed != 0u)
		animations += 0xCu;
	OBJECT(object)->enemy_rocksteady_knocked_down.animations = animations;
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the fall; on landing he slides */
void enemy_rocksteady_knocked_down_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation_entry = 0, animations = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = rocksteady_knocked_down_animations_2_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animations = OBJECT(object)->enemy_rocksteady_knocked_down.animations;
	animation_entry = ANIMATION_ENTRY(animations + 0x4u);
	set_animation(0u, object, animation_entry);
}

/* slides to a stop, then lies 30 frames */
void enemy_rocksteady_knocked_down_slide(uint32_t object)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = rocksteady_knocked_down_animations_2_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_rocksteady_knocked_down.lie_time = 0x1Eu;
		OBJECT(object)->state++;
	}
}

/* lies on the ground, then his health back by the difficulty and he gets up */
void enemy_rocksteady_knocked_down_lie(uint32_t object)
{
	uint32_t animation_entry = 0, animations = 0, settings = 0;
	uint8_t lie_time = 0;

	lie_time = OBJECT(object)->enemy_rocksteady_knocked_down.lie_time;
	OBJECT(object)->enemy_rocksteady_knocked_down.lie_time = lie_time - 1u;
	if (lie_time != 1u)
		return;
	OBJECT(object)->state++;
	settings = rocksteady_health_area_2_ADDRESS;
	if (area != 2u)
		settings = rocksteady_health_ADDRESS;
	enemy_refill_health(object, settings);
	animations = OBJECT(object)->enemy_rocksteady_knocked_down.animations;
	animation_entry = ANIMATION_ENTRY(animations + 0x8u);
	set_animation(0u, object, animation_entry);
}

/* when he is up: the hits he takes before striking back, by the difficulty, hittable, thinking */
void enemy_rocksteady_knocked_down_get_up(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		rocksteady_hits_to_strike_back = rocksteady_hits_to_strike_back_by_difficulty[difficulty & 7u];
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rocksteady_knocked_down.move_time = 0u;
		if (OBJECT(object)->enemy_rocksteady_knocked_down.armed == 0u)
			OBJECT(object)->enemy_rocksteady_knocked_down.mode_time = 1u;
	}
}

/* defeated: falls, the last boss clears the area, blinks and is gone */
void enemy_rocksteady_defeated(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rocksteady_defeated_states_ADDRESS;
	run_object_state(object, states);
}

/* defeated: no more pincer charge, thrown along his arc, gun out or not */
void enemy_rocksteady_defeated_start(uint32_t object)
{
	uint32_t animation_entry = 0, animations = 0, target = 0;

	no_pincer_charge = 0xFFu;
	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	animations = rocksteady_defeated_animations_2_ADDRESS;
	enemy_start_arc(object, animations);
	animations = rocksteady_defeated_animations_ADDRESS;
	if (OBJECT(object)->enemy_rocksteady_defeated.armed != 0u)
		animations += 8u;
	OBJECT(object)->enemy_rocksteady_defeated.animations = animations;
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the fall; on landing a sound and one boss less, the last one ending the fight and the music */
void enemy_rocksteady_defeated_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation_entry = 0, animations = 0, arc = 0;
	uint8_t t1 = 0;

	move_object_with_height_blocked(object);
	arc = rocksteady_defeated_animations_2_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	t1 = bosses_left;
	bosses_left = t1 - 1u;
	if (t1 == 1u)
	{
		boss_defeated = 1u;
		sound = 0u;
		music = 0u;
		queue_sound(sound);
	}
	sound = 0x8Bu;
	queue_sound(sound);
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animations = OBJECT(object)->enemy_rocksteady_defeated.animations;
	animation_entry = ANIMATION_ENTRY(animations + 0x4u);
	set_animation(0u, object, animation_entry);
}

/* slides to a stop, then blinks 120 frames */
void enemy_rocksteady_defeated_slide(uint32_t object)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = rocksteady_defeated_animations_2_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_rocksteady_defeated.blink_time = 0x78u;
		OBJECT(object)->state++;
	}
}

/* blinks, then he is gone */
void enemy_rocksteady_defeated_blink(uint32_t object)
{
	uint8_t blink_time = 0, t1 = 0;

	if ((OBJECT(object)->enemy_rocksteady_defeated.blink_time & 0x10u) == 0u)
		OBJECT(object)->flags ^= 0x80u;
	blink_time = OBJECT(object)->enemy_rocksteady_defeated.blink_time;
	OBJECT(object)->enemy_rocksteady_defeated.blink_time = blink_time - 1u;
	if (blink_time == 1u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* Bebop comes in alone or beside Rocksteady, counted as a boss */
void enemy_bebop_start(uint32_t object, uint32_t target)
{
	uint16_t kind = 0;
	uint32_t animation = 0, data = 0;

	target = enemy_nearest_turtles(object, target);
	OBJECT(object)->enemy_bebop.target = target;
	OBJECT(object)->enemy_bebop.retarget_time = 0x10u;
	OBJECT(object)->enemy_bebop.move_time = 0u;
	OBJECT(object)->enemy_bebop.plan = 0u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->enemy_bebop.punch_delay = 3u;
	OBJECT(object)->shadow = big_boss_shadow_ADDRESS;
	enemies_on_screen++;
	bosses_left++;
	if (OBJECT(object)->parameter != 0u)
	{
		OBJECT(object)->floor_height = 0u;
		pincer_charge_wait = 0x168u;
		no_pincer_charge = 0u;
		pincer_charge = 0u;
		OBJECT(object)->parameter = 0xEu;
		OBJECT(object)->routine = 1u;
		OBJECT(object)->state = 6u;
		OBJECT(object)->flags |= 0x80u;
		animation = enemy_bebop_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->flags |= 0x80u;
	animation = enemy_bebop_raise_gun_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->frame_time = 0u;
	OBJECT(object)->floor_height = 0x28u;
	no_pincer_charge = 0xFFu;
	OBJECT(object)->routine++;
	OBJECT(object)->parameter = 0xCu;
	data = bebop_palette_ADDRESS;
	kind = 0x43Cu;
	post_palette_request(kind, data);
}

/* his way in: drops in, then jumps forward to the ground, or waits for Rocksteady and walks in */
void enemy_bebop_enter(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_enter_states_ADDRESS;
	run_object_state(object, states);
}

/* waits until the scroll reaches him, then the music fades out and his health is set */
void enemy_bebop_enter_wait_for_scroll(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t settings = 0;

	if (LAYER(LAYER_B)->x < 0x8C000u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_bebop_enter.timer = 0x5Au;
	sound = 0xFEu;
	music = 0xFEu;
	queue_sound(sound);
	settings = bebop_health_ADDRESS;
	enemy_set_health(object, settings);
}

/* waits 90 frames, then stands still */
void enemy_bebop_enter_wait(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_bebop_enter.timer;
	OBJECT(object)->enemy_bebop_enter.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state++;
		OBJECT(object)->y_speed = 0;
		OBJECT(object)->height_speed = 0;
		OBJECT(object)->x_speed = 0;
	}
}

/* drops onto his floor; on landing a sound and his landing pose */
void enemy_bebop_enter_drop(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x60u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	sound = 0x3Eu;
	queue_sound(sound);
	OBJECT(object)->state++;
	animation = enemy_bebop_enter_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, jumps forward off his floor */
void enemy_bebop_enter_jump(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height_speed = 0x600;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = -0x100;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->state++;
	enemy_bebop_enter_in_air(object);
}

/* a frame of the jump; on landing a sound and a pose, then he gets ready */
void enemy_bebop_enter_in_air(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x60u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	sound = 0x3Fu;
	queue_sound(sound);
	OBJECT(object)->state++;
	animation = enemy_bebop_enter_in_air_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_bebop_enter_ready(object);
}

/* when the pose ends, hittable and thinking, with the boss music */
void enemy_bebop_enter_ready(uint32_t object)
{
	uint8_t sound = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->flags |= 2u;
	sound = 0x64u;
	music = 0x64u;
	queue_sound(sound);
}

/* beside Rocksteady: waits for the signal for the bosses to come out, then walks */
void enemy_bebop_enter_wait_for_signal(uint32_t object)
{
	uint32_t animation = 0;

	if (bosses_come_out == 0u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_bebop_enter.walk_time = 0x20u;
	animation = enemy_bebop_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks in for 32 frames, then his health and palette, hittable, and he thinks */
void enemy_bebop_enter_walk_in(uint32_t object)
{
	uint16_t kind = 0;
	uint32_t data = 0, settings = 0;
	uint8_t walk_time = 0;

	walk_time = OBJECT(object)->enemy_bebop_enter.walk_time;
	OBJECT(object)->enemy_bebop_enter.walk_time = walk_time - 1u;
	if (walk_time == 1u)
	{
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		settings = bebop_health_2_ADDRESS;
		enemy_set_health(object, settings);
		data = bebop_palette_ADDRESS;
		kind = 0x43Eu;
		post_palette_request(kind, data);
	}
	OBJECT(object)->y -= 0x100u;
}

/* his thinking: his two taunts, the pincer charge with Rocksteady, a punch when close, an attack or a
   move by plan */
void enemy_bebop_think(uint32_t on_right, uint32_t object, uint32_t target)
{
	uint32_t zero = 0;
	uint8_t parameter = 0, plan = 0;
	uint16_t screen_x = 0, screen_y = 0;
	uint32_t attack = 0, chance = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, move = 0, roll = 0, turtle = 0, turtle_2 = 0;
	uint8_t retarget_time = 0;
	uint16_t t1 = 0, chance_2 = 0, t2 = 0;

	parameter = OBJECT(object)->parameter;
	enemy_flash_when_weak(parameter, object);
	if (area == 2u)
	{
		if (bebop_exchange_lines == 0u && time_since_balloon >= 0x80u)
		{
			on_right = on_right | 0xFFu;
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				on_right &= ~0xFFu;
			screen_x = OBJECT(object)->screen_x;
			screen_y = OBJECT(object)->screen_y;
			say_youre_dead_shell_brain(screen_x, screen_y, &on_right, &target);
		}
		if (bebop_exchange_lines == 2u && time_since_balloon >= 0x60u)
		{
			on_right = on_right | 0xFFu;
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				on_right &= ~0xFFu;
			screen_x = OBJECT(object)->screen_x;
			screen_y = OBJECT(object)->screen_y;
			target = say_watch_your_mouth(on_right, screen_x, screen_y, target);
		}
	}
	retarget_time = OBJECT(object)->enemy_bebop.retarget_time;
	OBJECT(object)->enemy_bebop.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_bebop.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			target = enemy_nearest_turtles(object, target);
			OBJECT(object)->enemy_bebop.target = target;
		}
	}
	turtle_2 = 0x60000u | OBJECT(object)->enemy_bebop.target;
	turtle = turtle_2;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	enemy_keep_on_screen(object);
	if (no_pincer_charge == 0u)
	{
		t1 = pincer_charge_wait;
		pincer_charge_wait = t1 - 1u;
		if (t1 == 1u)
		{
			pincer_charge_wait = 0x1Eu;
			pincer_charge = 1u;
			pincer_charge_target = turtle_2;
			pincer_charge_edges = 0u;
			pincer_charge_at_edges = 0u;
			pincer_charge_crash = 0u;
			OBJECT(object)->enemy_bebop.move = 0x84u;
			OBJECT(object)->routine = 3u;
			OBJECT(object)->state = 0u;
			object_loop_slot = turtle_2;
			return;
		}
	}
	if (OBJECT(turtle_2)->routine != 4u || hit_reaction_down[OBJECT(turtle_2)->turtle.hit_reaction] != 0u)
	{
		zero = enemy_on_screen(object);
		if (zero)
		{
			if (OBJECT(object)->enemy_bebop.target_dx >= 0x28u)
			{
				attack = bebop_attacks_ADDRESS + (uint16_t)((OBJECT(object)->enemy_bebop.plan & 3u) << 3);
				chance = 0x64u;
				chance = random_below((uint16_t)chance, object);
				if (chance < BEBOP_ATTACK(attack)->chance && OBJECT(object)->enemy_bebop.target_dy <= BEBOP_ATTACK(attack)->reach_y && OBJECT(object)->enemy_bebop.target_dx <= BEBOP_ATTACK(attack)->reach_x)
				{
					OBJECT(object)->enemy_bebop.move = BEBOP_ATTACK(attack)->move;
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					object_loop_slot = turtle_2;
					return;
				}
			}
			else if (OBJECT(object)->enemy_bebop.target_dy < 5u)
			{
				OBJECT(object)->enemy_bebop.move = 0x7Cu;
				OBJECT(object)->routine = 3u;
				OBJECT(object)->state = 0u;
				OBJECT(object)->flags &= ~8u;
				object_loop_slot = turtle_2;
				return;
			}
		}
	}
	if (OBJECT(object)->enemy_bebop.move_time == 0u)
	{
		move = bebop_far_moves_ADDRESS;
		roll = 0x64u;
		roll = random_below((uint16_t)roll, object);
		plan = 0u;
		for (;;)
		{
			chance_2 = RANDOM_MOVE(move)->chance;
			t2 = roll;
			roll = (uint16_t)(t2 - chance_2);
			if (t2 < chance_2)
				break;
			plan++;
			move += 4u;
			if (plan >= 4u)
			{
				move = bebop_far_moves_ADDRESS;
				plan = 0u;
				break;
			}
		}
		OBJECT(object)->enemy_bebop.plan = plan;
		OBJECT(object)->enemy_bebop.move = RANDOM_MOVE(move)->move;
		OBJECT(object)->state = 0u;
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_bebop.move)
	{
	case 20:
		object_loop_slot = turtle_2;
		enemy_shuffle(object);
		return;
	case 28:
		object_loop_slot = turtle_2;
		enemy_circle(object);
		return;
	case 36:
		object_loop_slot = turtle_2;
		enemy_charge(object);
		return;
	case 0:
		object_loop_slot = turtle_2;
		enemy_walk_to_target(object);
		return;
	case 4:
		object_loop_slot = turtle_2;
		enemy_walk_to_side(object);
		return;
	case 8:
		object_loop_slot = turtle_2;
		enemy_keep_distance(object);
		return;
	case 12:
		object_loop_slot = turtle_2;
		enemy_back_off(object);
		return;
	case 16:
		object_loop_slot = turtle_2;
		enemy_run_away(object);
		return;
	case 44:
		object_loop_slot = turtle_2;
		enemy_walk_across(object);
		return;
	case 48:
		object_loop_slot = turtle_2;
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		object_loop_slot = turtle_2;
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		object_loop_slot = turtle_2;
		enemy_walk_past_sword(object);
		return;
	case 32:
		object_loop_slot = turtle_2;
		enemy_back_off_sword(object);
		return;
	case 40:
		object_loop_slot = turtle_2;
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		object_loop_slot = turtle_2;
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		object_loop_slot = turtle_2;
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		object_loop_slot = turtle_2;
		enemy_stalk_hammer(object);
		return;
	case 64:
		object_loop_slot = turtle_2;
		enemy_walk_away_hammer(object);
		return;
	case 72:
		object_loop_slot = turtle_2;
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		object_loop_slot = turtle_2;
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		object_loop_slot = turtle_2;
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		object_loop_slot = turtle_2;
		enemy_back_off_spear(object);
		return;
	case 112:
		object_loop_slot = turtle_2;
		enemy_step_aside_spear(object);
		return;
	case 80:
		object_loop_slot = turtle_2;
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		object_loop_slot = turtle_2;
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		object_loop_slot = turtle_2;
		enemy_circle_knife(object);
		return;
	case 124:
		object_loop_slot = turtle_2;
		enemy_keep_distance_knife(object);
		return;
	case 120:
		object_loop_slot = turtle_2;
		enemy_back_off_knife(object);
		return;
	case 128:
		object_loop_slot = turtle_2;
		enemy_step_aside_knife(object);
		return;
	case 88:
		object_loop_slot = turtle_2;
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		object_loop_slot = turtle_2;
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		object_loop_slot = turtle_2;
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		object_loop_slot = turtle_2;
		enemy_back_off_gun(object);
		return;
	case 144:
		object_loop_slot = turtle_2;
		enemy_run_away_gun(object);
		return;
	case 96:
		object_loop_slot = turtle_2;
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		object_loop_slot = turtle_2;
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		object_loop_slot = turtle_2;
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		object_loop_slot = turtle_2;
		enemy_back_off_axe(object);
		return;
	case 136:
		object_loop_slot = turtle_2;
		enemy_run_away_axe(object);
		return;
	case 196:
		object_loop_slot = turtle_2;
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		object_loop_slot = turtle_2;
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		object_loop_slot = turtle_2;
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		object_loop_slot = turtle_2;
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		object_loop_slot = turtle_2;
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		object_loop_slot = turtle_2;
		enemy_rocksteady_circle(object);
		return;
	case 208:
		object_loop_slot = turtle_2;
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		object_loop_slot = turtle_2;
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		object_loop_slot = turtle_2;
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		object_loop_slot = turtle_2;
		enemy_bebop_stand(object);
		return;
	case 216:
		object_loop_slot = turtle_2;
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		object_loop_slot = turtle_2;
		enemy_bebop_walk_level(object);
		return;
	case 220:
		object_loop_slot = turtle_2;
		enemy_bebop_close_in(object);
		return;
	case 156:
		object_loop_slot = turtle_2;
		enemy_mouser_chase(object);
		return;
	case 164:
		object_loop_slot = turtle_2;
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		object_loop_slot = turtle_2;
		enemy_mouser_back_off(object);
		return;
	case 172:
		object_loop_slot = turtle_2;
		enemy_rodney_rush(object);
		return;
	case 176:
		object_loop_slot = turtle_2;
		enemy_rodney_rush_across(object);
		return;
	case 180:
		object_loop_slot = turtle_2;
		enemy_rodney_back_off(object);
		return;
	case 236:
		object_loop_slot = turtle_2;
		enemy_shredder_appear(object);
		return;
	case 240:
		object_loop_slot = turtle_2;
		enemy_shredder_split(object);
		return;
	case 168:
		object_loop_slot = turtle_2;
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		object_loop_slot = turtle_2;
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		object_loop_slot = turtle_2;
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		object_loop_slot = turtle_2;
		enemy_shredder_guard(object);
		return;
	case 256:
		object_loop_slot = turtle_2;
		enemy_shredder_back_off(object);
		return;
	}
}

/* his attack, flashing when weak */
void enemy_bebop_attack(uint32_t object)
{
	uint8_t colour = 0;

	colour = OBJECT(object)->parameter;
	enemy_flash_when_weak(colour, object);
	enemy_boss_attack_move(object);
}

/* hit: his reaction by what is left of his health and where the hit came from */
void enemy_bebop_hit(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t move = 0;

	sound = 0x82u;
	queue_sound(sound);
	pincer_charge = 0u;
	move = 0xB8u;
	if ((OBJECT(object)->health_bars | OBJECT(object)->health) != 0u)
	{
		move = 0xB4u;
		if (OBJECT(object)->health != 0u)
		{
			if ((int8_t)(((uint32_t)OBJECT(object)->flags << 1) ^ OBJECT(object)->hit) >= 0)
				move = 0xACu;
			else
				move = 0xB0u;
		}
	}
	OBJECT(object)->enemy_bebop.move = move;
	OBJECT(object)->routine++;
	enemy_bebop_react(object);
}

/* his reaction to a hit, flashing when weak */
void enemy_bebop_react(uint32_t object)
{
	uint8_t colour = 0;

	colour = OBJECT(object)->parameter;
	enemy_flash_when_weak(colour, object);
	enemy_boss_react_move(object);
}

/* stands facing his turtle */
void enemy_bebop_stand(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_stand_states_ADDRESS;
	run_object_state(object, states);
}

/* 120 frames standing facing his turtle, in one of four poses at random */
void enemy_bebop_stand_start(uint32_t object)
{
	uint32_t animation = 0, range = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_bebop.move_time = 0x78u;
	OBJECT(object)->state++;
	range = 4u;
	range = random_below((uint16_t)range, object);
	animation = enemy_bebop_stand_animations[range & 3u];
	set_animation(0u, object, animation);
}

/* stands while the move's time runs out */
void enemy_bebop_stand_wait(uint32_t object)
{
	OBJECT(object)->enemy_bebop.move_time--;
}

/* walks to his turtle's front */
void enemy_bebop_walk_to_front(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_walk_to_front_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to a place 0x50 in front of his turtle */
void enemy_bebop_walk_to_front_start(uint32_t object)
{
	uint32_t animation = 0, turtle = 0;

	OBJECT(object)->enemy_bebop.move_time = 0xF0u;
	OBJECT(object)->enemy_bebop.timer = 1u;
	OBJECT(object)->state++;
	animation = enemy_bebop_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	turtle = object_loop_slot;
	enemy_bebop_walk_to_front_step(object, turtle);
}

/* a frame of the walk, aiming every 4 frames, slower when near or level with his turtle */
void enemy_bebop_walk_to_front_step(uint32_t object, uint32_t turtle)
{
	uint16_t speed = 0, target_x = 0, target_y = 0;
	uint32_t direction = 0, x_speed = 0, y_speed = 0;
	uint8_t timer = 0;

	OBJECT(object)->enemy_bebop.move_time--;
	timer = OBJECT(object)->enemy_bebop.timer;
	OBJECT(object)->enemy_bebop.timer = timer - 1u;
	if (timer != 1u)
	{
		move_object_blocked(object);
		return;
	}
	enemy_boss_face_target(object, turtle);
	target_x = 0x50u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		target_x = (uint16_t)(0u - target_x);
	target_x += OBJECT(turtle)->screen_x;
	target_y = OBJECT(turtle)->screen_y;
	direction = direction_to(0u, target_x, target_y, object);
	speed = 0x200u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	x_speed = (uint16_t)((int16_t)x_speed >> 1);
	if (OBJECT(object)->enemy_bebop.target_dx <= 0x50u)
		x_speed = (uint16_t)((int16_t)x_speed >> 1);
	if (OBJECT(object)->enemy_bebop.target_dy <= 2u)
		y_speed = (uint16_t)((int16_t)y_speed >> 1);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->enemy_bebop.timer = 4u;
	move_object_blocked(object);
}

/* walks level with his turtle, then to it */
void enemy_bebop_walk_level(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_walk_level_states_ADDRESS;
	run_object_state(object, states);
}

/* a frame of the walk: up or down to his turtle's row, then across toward it, faster when far */
void enemy_bebop_walk_level_step(uint32_t object)
{
	uint32_t target = 0;
	uint8_t timer = 0;

	OBJECT(object)->enemy_bebop.move_time--;
	timer = OBJECT(object)->enemy_bebop.timer;
	OBJECT(object)->enemy_bebop.timer = timer - 1u;
	if (timer != 1u)
	{
		move_object_blocked(object);
		return;
	}
	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	if (OBJECT(object)->enemy_bebop.target_dy > 4u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0x100;
		if ((int8_t)OBJECT(object)->enemy_bebop.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
		OBJECT(object)->enemy_bebop.timer = 4u;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->x_speed = 0x80;
	OBJECT(object)->y_speed = 0;
	if (OBJECT(object)->enemy_bebop.target_dx >= 0x50u)
		OBJECT(object)->x_speed = 0x100;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(object)->enemy_bebop.timer = 4u;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->enemy_bebop.timer = 4u;
	move_object_blocked(object);
}

/* walks up close to his turtle */
void enemy_bebop_close_in(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_close_in_states_ADDRESS;
	run_object_state(object, states);
}

/* starts one of his walks */
void enemy_bebop_walk_start(uint32_t object)
{
	uint32_t animation = 0, turtle = 0;

	OBJECT(object)->enemy_bebop.move_time = 0xF0u;
	OBJECT(object)->enemy_bebop.timer = 1u;
	OBJECT(object)->state++;
	animation = enemy_bebop_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	turtle = object_loop_slot;
	enemy_bebop_close_in_step(object, turtle);
}

/* a frame of closing in on a place 0x30 in front of his turtle, aiming every 4 frames; level with it he
   stands */
void enemy_bebop_close_in_step(uint32_t object, uint32_t turtle)
{
	uint16_t speed = 0, target_x = 0, target_y = 0;
	uint32_t direction = 0, x_speed = 0, y_speed = 0;
	uint8_t timer = 0;

	OBJECT(object)->enemy_bebop.move_time--;
	timer = OBJECT(object)->enemy_bebop.timer;
	OBJECT(object)->enemy_bebop.timer = timer - 1u;
	if (timer != 1u)
	{
		move_object_blocked(object);
		return;
	}
	enemy_boss_face_target(object, turtle);
	if (OBJECT(object)->enemy_bebop.target_dy > 4u)
	{
		target_x = 0x30u;
		if ((OBJECT(object)->flags & 0x40u) != 0u)
			target_x = (uint16_t)(0u - target_x);
		target_x += OBJECT(turtle)->screen_x;
		target_y = OBJECT(turtle)->screen_y;
		direction = direction_to(0u, target_x, target_y, object);
		speed = 0x180u;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = y_speed;
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0;
		if ((uint16_t)OBJECT(object)->x_speed >= 0x10u)
		{
			OBJECT(object)->x_speed = 0x100;
			if ((OBJECT(object)->flags & 0x40u) == 0u)
				OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		}
	}
	OBJECT(object)->enemy_bebop.timer = 4u;
	move_object_blocked(object);
}

/* charges across from his edge while Rocksteady charges from the other, crashing into him */
void enemy_bebop_pincer_charge(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_pincer_charge_states_ADDRESS;
	run_object_state(object, states);
	if (pincer_charge == 0u && pincer_charge_crash == 0u)
	{
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_bebop.move_time = 0u;
	}
}

/* the pincer charge with Rocksteady: he runs to his edge of the screen */
void enemy_bebop_pincer_charge_start(uint32_t object)
{
	uint32_t animation = 0;

	animation = enemy_bebop_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_pincer_charge_take_edge(object);
}

/* takes a free edge of the screen for the pincer charge and runs to it */
void CONSOLE_ORIGINAL(enemy_pincer_charge_take_edge)(uint32_t object)
{
	uint8_t t1 = 0, t2 = 0;

	OBJECT(object)->enemy.side = 0u;
	if (OBJECT(object)->screen_x < 0x100u)
	{
		t1 = pincer_charge_edges;
		pincer_charge_edges = t1 | 0x40u;
		if ((t1 & 0x40u) == 0u)
		{
			OBJECT(object)->enemy.side |= 0x40u;
			OBJECT(object)->flags &= 0xBFu;
			OBJECT(object)->x_speed = -0x200;
			OBJECT(object)->y_speed = 0;
			OBJECT(object)->state++;
			return;
		}
	}
	do
	{
		t2 = pincer_charge_edges;
		pincer_charge_edges = t2 | 0x80u;
		if ((t2 & 0x80u) == 0u)
		{
			OBJECT(object)->enemy.side |= 0x80u;
			OBJECT(object)->flags |= 0x40u;
			OBJECT(object)->x_speed = 0x200;
			OBJECT(object)->y_speed = 0;
			OBJECT(object)->state++;
			return;
		}
		t1 = pincer_charge_edges;
		pincer_charge_edges = t1 | 0x40u;
	} while ((t1 & 0x40u) != 0u);
	OBJECT(object)->enemy.side |= 0x40u;
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->x_speed = -0x200;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->state++;
}

/* runs to his edge; at the wall he turns round and marks his edge taken */
void enemy_bebop_pincer_charge_run_to_edge(uint32_t object)
{
	uint8_t wall_mask = 0;

	move_object_blocked(object);
	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->flags ^= 0x40u;
		pincer_charge_at_edges |= OBJECT(object)->enemy_bebop.side;
		OBJECT(object)->state++;
	}
}

/* waits until Rocksteady holds the other edge, then readies the charge, no longer hittable */
void enemy_bebop_pincer_charge_wait_for_rocksteady(uint32_t object)
{
	uint32_t animation = 0;

	if (pincer_charge_at_edges < 0xC0u)
		return;
	animation = enemy_bebop_charge_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	OBJECT(object)->attack = 7u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->attack |= 0x80u;
	OBJECT(object)->flags &= ~0xAu;
	OBJECT(object)->enemy_bebop.move_time = 0x1Eu;
	OBJECT(object)->state++;
}

/* 30 frames, then the charge across */
void enemy_bebop_pincer_charge_get_ready(uint32_t object)
{
	enemy_bebop_charge_get_ready(object);
}

/* a frame of the charge on his turtle's row; at a wall the next in 360 frames; meeting Rocksteady, they
   crash */
void enemy_bebop_pincer_charge_run(uint32_t object)
{
	uint8_t sound = 0, wall_mask = 0;
	uint16_t y_speed = 0;
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, object_2 = 0, x_offset = 0;
	uint8_t t1 = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		pincer_charge = 0u;
		pincer_charge_wait = 0x168u;
		return;
	}
	y_speed = 0u;
	if (OBJECT(object)->enemy_bebop.target_dy >= 3u)
	{
		y_speed = 0x200u;
		if ((int8_t)OBJECT(object)->enemy_bebop.target_direction >= 0)
			y_speed = (uint16_t)(0u - y_speed);
	}
	OBJECT(object)->y_speed = y_speed;
	move_object_blocked(object);
	object_2 = rocksteady;
	enemy_look_at_target(object, object_2, &ignored, &ignored_2);
	OBJECT(object)->enemy_bebop.target_direction -= 0x40u;
	t1 = OBJECT(object)->enemy_bebop.target_direction ^ ((uint32_t)OBJECT(object)->flags << 1);
	OBJECT(object)->enemy_bebop.target_direction = t1;
	if ((int8_t)t1 < 0 || OBJECT(object)->enemy_bebop.target_dx >= 0x38u || OBJECT(object)->enemy_bebop.target_dy >= 5u)
		return;
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->state++;
	pincer_charge_crash = 1u;
	OBJECT(object_2)->y = OBJECT(object)->y;
	OBJECT(object_2)->y += 0x200u;
	x_offset = 0x4000u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		x_offset = (uint16_t)(0u - x_offset);
	OBJECT(object_2)->x = (int16_t)x_offset + OBJECT(object)->x;
	OBJECT(object_2)->attacker = object;
	OBJECT(object)->attacker = object_2;
	animation = enemy_bebop_pincer_animation_ADDRESS;
	set_animation(x_offset, object, animation);
	sound = 0x3Eu;
	queue_sound(sound);
	sound = 0x8Fu;
	queue_sound(sound);
}

/* when the crash ends, the next pincer charge in 1800 frames, and he is knocked down */
void enemy_bebop_pincer_charge_crash(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		pincer_charge = 0u;
		pincer_charge_wait = 0x708u;
		OBJECT(object)->enemy_bebop.move = 0xB4u;
		OBJECT(object)->routine = 5u;
		OBJECT(object)->state = 0u;
	}
}

/* punches, lunging when he strikes back */
void enemy_bebop_punch(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_punch_states_ADDRESS;
	run_object_state(object, states);
}

/* the punch, slowed by his punch delay, facing his turtle; hittable, unless he was not and two or more
   turtles play */
void enemy_bebop_punch_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;
	uint8_t flags = 0;

	OBJECT(object)->animation_delay = OBJECT(object)->enemy_bebop.punch_delay;
	OBJECT(object)->damage = 0x203u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	flags = OBJECT(object)->flags;
	OBJECT(object)->flags = flags | 2u;
	if ((flags & 2u) == 0u && turtles_in_play >= 2u)
		OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	animation = enemy_bebop_punch_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the punch, lunging forward when he strikes back; at its end, back to thinking */
void enemy_bebop_punch_end(uint32_t object)
{
	uint16_t x_step = 0;

	if (OBJECT(object)->animation_delay != 1u || OBJECT(object)->enemy_bebop.target_dx < 0x28u)
	{
		if (OBJECT(object)->frame_time == 0u)
		{
			OBJECT(object)->animation_delay = 0u;
			OBJECT(object)->enemy_bebop.punch_delay = 3u;
			OBJECT(object)->flags &= ~4u;
			OBJECT(object)->flags |= 2u;
			OBJECT(object)->routine = 2u;
			OBJECT(object)->state = 0u;
			OBJECT(object)->enemy_bebop.move_time = 0u;
		}
		return;
	}
	x_step = 0x80u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		x_step = (uint16_t)(0u - x_step);
	OBJECT(object)->x += (int16_t)x_step;
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->animation_delay = 0u;
		OBJECT(object)->enemy_bebop.punch_delay = 3u;
		OBJECT(object)->flags &= ~4u;
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_bebop.move_time = 0u;
	}
}

/* charges across the screen */
void enemy_bebop_charge(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_charge_states_ADDRESS;
	run_object_state(object, states);
}

/* the charge's animation and damage, facing his turtle, not hittable; 30 frames to get ready */
void enemy_bebop_charge_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_bebop_charge_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags &= ~0xAu;
	OBJECT(object)->enemy_bebop.move_time = 0x1Eu;
	OBJECT(object)->state++;
}

/* after 30 frames, charges across at full speed, attacking */
void enemy_bebop_charge_get_ready(uint32_t object)
{
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_bebop.move_time;
	OBJECT(object)->enemy_bebop.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->x_speed = -0x380;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->state++;
}

/* a frame of the charge, following his turtle's row; at the wall, hittable again and a walk to his
   turtle's front */
void enemy_bebop_charge_run(uint32_t object)
{
	uint8_t contacts = 0;
	uint16_t y_speed = 0;

	contacts = OBJECT(object)->contacts;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		if ((contacts & 8u) == 0u)
		{
			y_speed = 0u;
			if (OBJECT(object)->enemy_bebop.target_dy >= 3u)
			{
				y_speed = 0x200u;
				if ((int8_t)OBJECT(object)->enemy_bebop.target_direction >= 0)
					y_speed = (uint16_t)(0u - y_speed);
			}
			OBJECT(object)->y_speed = y_speed;
			move_object_blocked(object);
			return;
		}
	}
	else if ((contacts & 0x10u) == 0u)
	{
		y_speed = 0u;
		if (OBJECT(object)->enemy_bebop.target_dy >= 3u)
		{
			y_speed = 0x200u;
			if ((int8_t)OBJECT(object)->enemy_bebop.target_direction >= 0)
				y_speed = (uint16_t)(0u - y_speed);
		}
		OBJECT(object)->y_speed = y_speed;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy_bebop.move = 0xD8u;
	OBJECT(object)->enemy_bebop.plan = 0u;
	OBJECT(object)->enemy_bebop.move_time = 1u;
}

/* fires three shots */
void enemy_bebop_shoot(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_shoot_states_ADDRESS;
	run_object_state(object, states);
}

/* three shots at his turtle: he faces it, the first in 20 frames */
void enemy_bebop_shoot_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_bebop_shoot.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy_bebop_shoot.shot_time = 0x14u;
	OBJECT(object)->enemy_bebop_shoot.shots_left = 3u;
	OBJECT(object)->state++;
	animation = enemy_bebop_shoot_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a shot with its sound, then every 12 frames; after the third, back to thinking */
void enemy_bebop_shoot_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t ignored = 0, record = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_bebop_shoot.shot_time;
	OBJECT(object)->enemy_bebop_shoot.shot_time = shot_time - 1u;
	if (shot_time == 1u)
	{
		sound = 2u;
		queue_sound(sound);
		object_loop_leftover = bebop_shot_record_ADDRESS;
		record = bebop_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		OBJECT(object)->enemy_bebop_shoot.shot_time = 0xCu;
		shots_left = OBJECT(object)->enemy_bebop_shoot.shots_left;
		OBJECT(object)->enemy_bebop_shoot.shots_left = shots_left - 1u;
		if (shots_left == 1u)
		{
			OBJECT(object)->routine = 2u;
			OBJECT(object)->state = 0u;
			OBJECT(object)->enemy_bebop_shoot.move_time = 0u;
		}
	}
}

/* hit from the front, then strikes back */
void enemy_bebop_flinch(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_flinch_states_ADDRESS;
	run_object_state(object, states);
}

/* his flinch, knocked back */
void enemy_bebop_flinch_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_bebop_flinch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* punches at once after a hit */
void enemy_bebop_strike_back(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_bebop.move = 0x7Cu;
		OBJECT(object)->enemy_bebop.plan = 1u;
		OBJECT(object)->enemy_bebop.punch_delay = 1u;
	}
}

/* hit from behind, then strikes back */
void enemy_bebop_flinch_from_behind(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_flinch_from_behind_states_ADDRESS;
	run_object_state(object, states);
}

/* hit from behind, he staggers forward for 48 frames */
void enemy_bebop_flinch_from_behind_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_bebop.move_time = 0x30u;
	animation = enemy_bebop_flinch_from_behind_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after the stagger he strikes back with a quick punch */
void enemy_bebop_flinch_from_behind_end(uint32_t object)
{
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_bebop.move_time;
	OBJECT(object)->enemy_bebop.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_bebop.move = 0x7Cu;
		OBJECT(object)->enemy_bebop.plan = 1u;
		OBJECT(object)->enemy_bebop.punch_delay = 1u;
	}
}

/* knocked down, lies, gets up with his health back and strikes back */
void enemy_bebop_knocked_down(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_knocked_down_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked down along his arc, facing his turtle */
void enemy_bebop_knocked_down_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0, target = 0;

	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	arc = enemy_bebop_knocked_down_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_bebop_knocked_down_start_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the fall; on landing he slides */
void enemy_bebop_knocked_down_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_bebop_knocked_down_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_bebop_knocked_down_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop, then lies 20 frames */
void enemy_bebop_knocked_down_slide(uint32_t object)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_bebop_knocked_down_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_bebop.timer = 0x14u;
		OBJECT(object)->state++;
	}
}

/* lies on the ground, then his health back by the difficulty and he gets up */
void enemy_bebop_knocked_down_lie(uint32_t object)
{
	uint32_t animation = 0, settings = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_bebop.timer;
	OBJECT(object)->enemy_bebop.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->state++;
	settings = bebop_health_ADDRESS;
	if (area != 3u)
		settings = bebop_health_2_ADDRESS;
	enemy_refill_health(object, settings);
	animation = enemy_bebop_knocked_down_lie_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* defeated: falls, the last boss clears the area, blinks and is gone */
void enemy_bebop_defeated(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_bebop_defeated_states_ADDRESS;
	run_object_state(object, states);
}

/* defeated: no more pincer charge, thrown along his arc */
void enemy_bebop_defeated_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0, target = 0;

	no_pincer_charge = 0xFFu;
	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	arc = enemy_bebop_defeated_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_bebop_defeated_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the fall; on landing a sound and one boss less, the last one ending the fight and the music */
void enemy_bebop_defeated_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, arc = 0;
	uint8_t t1 = 0;

	move_object_with_height_blocked(object);
	arc = enemy_bebop_defeated_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	t1 = bosses_left;
	bosses_left = t1 - 1u;
	if (t1 == 1u)
	{
		boss_defeated = 1u;
		sound = 0u;
		music = 0u;
		queue_sound(sound);
	}
	sound = 0x8Bu;
	queue_sound(sound);
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_bebop_defeated_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop, then blinks 120 frames */
void enemy_bebop_defeated_slide(uint32_t object)
{
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_bebop_defeated_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_bebop.timer = 0x78u;
		OBJECT(object)->state++;
	}
}

/* blinks, then he is gone */
void enemy_bebop_defeated_blink(uint32_t object)
{
	uint8_t timer = 0, t1 = 0;

	if ((OBJECT(object)->enemy_bebop.timer & 0x10u) == 0u)
		OBJECT(object)->flags ^= 0x80u;
	timer = OBJECT(object)->enemy_bebop.timer;
	OBJECT(object)->enemy_bebop.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* a Mouser: one bar, thinking at once, hopping in or on the floor by its parameter */
void enemy_mouser_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;
	uint8_t parameter = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy_mouser.target = player_slot;
	OBJECT(object)->enemy_mouser.retarget_time = 0x10u;
	OBJECT(object)->enemy_mouser.move_time = 0u;
	OBJECT(object)->enemy_mouser.approach = 0u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->health_bars = 1u;
	OBJECT(object)->health = 1u;
	OBJECT(object)->shadow = enemy_mouser_shadow_ADDRESS;
	parameter = OBJECT(object)->parameter;
	OBJECT(object)->parameter = parameter & 0x7Fu;
	if ((parameter & 0x80u) != 0u)
	{
		OBJECT(object)->flags |= 0x82u;
		OBJECT(object)->routine += 2u;
		animation = enemy_mouser_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	if (OBJECT(object)->parameter != 0u)
	{
		OBJECT(object)->routine++;
		return;
	}
	enemy_set_floor(object);
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->routine += 2u;
	animation = enemy_mouser_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hops in from its parameter's height onto the floor */
void enemy_mouser_enter(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_enter_states_ADDRESS;
	run_object_state(object, states);
}

/* a Mouser's way in: its floor and first pose by its parameter */
void enemy_mouser_enter_start(uint32_t object)
{
	uint16_t parameter = 0;
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->state++;
	parameter = OBJECT(object)->parameter;
	parameter = (uint8_t)(parameter - 1u);
	OBJECT(object)->parameter = (uint8_t)parameter;
	OBJECT(object)->floor_height = TABLE8(mouser_enter_heights, parameter);
	animation = TABLE32(enemy_mouser_enter_animations, (uint16_t)((uint32_t)parameter << 2));
	set_animation(0u, object, animation);
}

/* when the pose ends, hops out by its parameter, hittable */
void enemy_mouser_enter_hop(uint32_t object)
{
	uint16_t hop_offset = 0;
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags |= 2u;
	hop_offset = (uint16_t)((uint32_t)OBJECT(object)->parameter << 2);
	OBJECT(object)->y_speed = mouser_enter_y_speeds[hop_offset / 2u];
	OBJECT(object)->height_speed = mouser_enter_height_speeds[hop_offset / 2u];
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->height += 0x1000u;
	OBJECT(object)->state++;
	animation = enemy_mouser_hop_up_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the hop; on landing a sound and its landing pose */
void enemy_mouser_enter_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->state++;
	sound = 0x39u;
	queue_sound(sound);
	animation = enemy_mouser_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, to thinking */
void enemy_mouser_enter_land(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* goes after the turtle with the fewest enemies and bites it; breaks down when the boss is beaten */
void enemy_mouser_think(uint32_t object, uint32_t second_slot)
{
	bool L_051210 = false;
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint32_t closest_slot = 0, floor_down = 0, floor_left = 0, floor_right = 0, floor_up = 0, ignored = 0, ignored_2 = 0, slot = 0, turtle = 0;
	uint8_t retarget_time = 0, t2 = 0, mouser_sides = 0, mouser_sides_2 = 0;
	uint32_t t1 = 0;

	slot = 0x60000u | OBJECT(object)->enemy_mouser.target;
	retarget_time = OBJECT(object)->enemy_mouser.retarget_time;
	OBJECT(object)->enemy_mouser.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_mouser.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			PLAYER_SLOT(slot)->enemies--;
			second_slot = enemy_nearest_slots(object, second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = object_loop_leftover;
				closest_slot = second_slot;
				object_loop_leftover = closest_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						object_loop_leftover = second_slot;
						second_slot = closest_slot;
					}
				}
			}
			if ((uint16_t)second_slot != OBJECT(object)->enemy_mouser.target)
			{
				object_loop_slot = slot;
				enemy_mouser_leave_side(object, slot);
			}
			OBJECT(object)->enemy_mouser.target = second_slot;
			slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	if (mousers_break_down != 0u)
	{
		OBJECT(object)->enemy_mouser.move = 0xE0u;
		OBJECT(object)->routine = 5u;
		OBJECT(object)->state = 0u;
		object_loop_slot = slot;
		return;
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		object_loop_slot = slot;
		slot = enemy_mouser_leave_side(object, slot);
		PLAYER_SLOT(slot)->enemies--;
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t2 = enemies_on_screen - 1u;
		enemies_on_screen = t2;
		if ((int8_t)t2 < 0)
			enemies_on_screen = 0u;
		object_loop_slot = slot;
		return;
	}
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &floor_right, &floor_left, &floor_up, &floor_down);
	zero = enemy_check_floor_edge((uint8_t)floor_right, (uint8_t)floor_left, (uint8_t)floor_up, (uint8_t)floor_down, object, slot);
	if (zero == 0u)
	{
		object_loop_slot = slot;
		return;
	}
	if (OBJECT(object)->enemy_mouser.approach != 0u)
	{
		if (OBJECT(object)->enemy_mouser.approach == 1u && (OBJECT(object)->enemy_mouser.target_dx >= 0x54u || OBJECT(object)->enemy_mouser.target_dy >= 0x34u))
			enemy_mouser_give_side_back(object, slot);
		if (OBJECT(object)->enemy_mouser.approach != 2u)
		{
			turtle = PLAYER_SLOT(slot)->turtle;
			if (OBJECT(turtle)->routine == 4u && hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] == 0u)
			{
				object_loop_slot = slot;
				slot = enemy_mouser_leave_side(object, slot);
				OBJECT(object)->enemy_mouser.approach = 2u;
				OBJECT(object)->enemy_mouser.move_time = 0u;
			}
		}
	}
	else if (PLAYER_SLOT(slot)->mouser_sides >= 0xC0u || OBJECT(object)->enemy_mouser.target_dx >= 0x40u || OBJECT(object)->enemy_mouser.target_dy >= 0x20u)
	{
		if (OBJECT(object)->enemy_mouser.approach != 2u)
		{
			turtle = PLAYER_SLOT(slot)->turtle;
			if (OBJECT(turtle)->routine == 4u && hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] == 0u)
			{
				object_loop_slot = slot;
				slot = enemy_mouser_leave_side(object, slot);
				OBJECT(object)->enemy_mouser.approach = 2u;
				OBJECT(object)->enemy_mouser.move_time = 0u;
			}
		}
	}
	else
	{
		OBJECT(object)->enemy_mouser.approach = 1u;
		OBJECT(object)->enemy_mouser.move_time = 0u;
		do
		{
			if (L_051210 || (int8_t)(OBJECT(object)->enemy_mouser.target_direction - 0x40u) < 0)
			{
				for (;;)
				{
					L_051210 = false;
					mouser_sides = PLAYER_SLOT(slot)->mouser_sides;
					PLAYER_SLOT(slot)->mouser_sides = mouser_sides | 0x40u;
					if ((mouser_sides & 0x40u) != 0u)
					{
						mouser_sides_2 = PLAYER_SLOT(slot)->mouser_sides;
						PLAYER_SLOT(slot)->mouser_sides = mouser_sides_2 | 0x80u;
						if ((mouser_sides_2 & 0x80u) != 0u)
							continue;
						OBJECT(object)->enemy_mouser.side &= 0x7Fu;
						break;
					}
					OBJECT(object)->enemy_mouser.side |= 0x80u;
					break;
				}
			}
			else
			{
				mouser_sides_2 = PLAYER_SLOT(slot)->mouser_sides;
				PLAYER_SLOT(slot)->mouser_sides = mouser_sides_2 | 0x80u;
				L_051210 = (mouser_sides_2 & 0x80u) != 0u;
				if (!L_051210)
					OBJECT(object)->enemy_mouser.side &= 0x7Fu;
			}
		} while (L_051210);
	}
	zero = enemy_on_screen(object);
	if (zero && OBJECT(PLAYER_SLOT(slot)->turtle)->routine != 4u && OBJECT(object)->enemy_mouser.target_dy < 4u && OBJECT(object)->enemy_mouser.target_dx < 0x30u)
	{
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->flags &= ~8u;
		object_loop_slot = slot;
		return;
	}
	if (OBJECT(object)->enemy_mouser.move_time == 0u)
	{
		OBJECT(object)->enemy_mouser.move = TABLE16(mouser_moves, (uint32_t)(int16_t)((uint32_t)(int8_t)OBJECT(object)->enemy_mouser.approach << 1));
		OBJECT(object)->state = 0u;
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_mouser.move)
	{
	case 20:
		object_loop_slot = slot;
		enemy_shuffle(object);
		return;
	case 28:
		object_loop_slot = slot;
		enemy_circle(object);
		return;
	case 36:
		object_loop_slot = slot;
		enemy_charge(object);
		return;
	case 0:
		object_loop_slot = slot;
		enemy_walk_to_target(object);
		return;
	case 4:
		object_loop_slot = slot;
		enemy_walk_to_side(object);
		return;
	case 8:
		object_loop_slot = slot;
		enemy_keep_distance(object);
		return;
	case 12:
		object_loop_slot = slot;
		enemy_back_off(object);
		return;
	case 16:
		object_loop_slot = slot;
		enemy_run_away(object);
		return;
	case 44:
		object_loop_slot = slot;
		enemy_walk_across(object);
		return;
	case 48:
		object_loop_slot = slot;
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		object_loop_slot = slot;
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		object_loop_slot = slot;
		enemy_walk_past_sword(object);
		return;
	case 32:
		object_loop_slot = slot;
		enemy_back_off_sword(object);
		return;
	case 40:
		object_loop_slot = slot;
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		object_loop_slot = slot;
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		object_loop_slot = slot;
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		object_loop_slot = slot;
		enemy_stalk_hammer(object);
		return;
	case 64:
		object_loop_slot = slot;
		enemy_walk_away_hammer(object);
		return;
	case 72:
		object_loop_slot = slot;
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		object_loop_slot = slot;
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		object_loop_slot = slot;
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		object_loop_slot = slot;
		enemy_back_off_spear(object);
		return;
	case 112:
		object_loop_slot = slot;
		enemy_step_aside_spear(object);
		return;
	case 80:
		object_loop_slot = slot;
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		object_loop_slot = slot;
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		object_loop_slot = slot;
		enemy_circle_knife(object);
		return;
	case 124:
		object_loop_slot = slot;
		enemy_keep_distance_knife(object);
		return;
	case 120:
		object_loop_slot = slot;
		enemy_back_off_knife(object);
		return;
	case 128:
		object_loop_slot = slot;
		enemy_step_aside_knife(object);
		return;
	case 88:
		object_loop_slot = slot;
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		object_loop_slot = slot;
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		object_loop_slot = slot;
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		object_loop_slot = slot;
		enemy_back_off_gun(object);
		return;
	case 144:
		object_loop_slot = slot;
		enemy_run_away_gun(object);
		return;
	case 96:
		object_loop_slot = slot;
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		object_loop_slot = slot;
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		object_loop_slot = slot;
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		object_loop_slot = slot;
		enemy_back_off_axe(object);
		return;
	case 136:
		object_loop_slot = slot;
		enemy_run_away_axe(object);
		return;
	case 196:
		object_loop_slot = slot;
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		object_loop_slot = slot;
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		object_loop_slot = slot;
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		object_loop_slot = slot;
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		object_loop_slot = slot;
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		object_loop_slot = slot;
		enemy_rocksteady_circle(object);
		return;
	case 208:
		object_loop_slot = slot;
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		object_loop_slot = slot;
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		object_loop_slot = slot;
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		object_loop_slot = slot;
		enemy_bebop_stand(object);
		return;
	case 216:
		object_loop_slot = slot;
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		object_loop_slot = slot;
		enemy_bebop_walk_level(object);
		return;
	case 220:
		object_loop_slot = slot;
		enemy_bebop_close_in(object);
		return;
	case 156:
		object_loop_slot = slot;
		enemy_mouser_chase(object);
		return;
	case 164:
		object_loop_slot = slot;
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		object_loop_slot = slot;
		enemy_mouser_back_off(object);
		return;
	case 172:
		object_loop_slot = slot;
		enemy_rodney_rush(object);
		return;
	case 176:
		object_loop_slot = slot;
		enemy_rodney_rush_across(object);
		return;
	case 180:
		object_loop_slot = slot;
		enemy_rodney_back_off(object);
		return;
	case 236:
		object_loop_slot = slot;
		enemy_shredder_appear(object);
		return;
	case 240:
		object_loop_slot = slot;
		enemy_shredder_split(object);
		return;
	case 168:
		object_loop_slot = slot;
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		object_loop_slot = slot;
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		object_loop_slot = slot;
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		object_loop_slot = slot;
		enemy_shredder_guard(object);
		return;
	case 256:
		object_loop_slot = slot;
		enemy_shredder_back_off(object);
		return;
	}
}

/* gives its side of its turtle back when it stands by it */
uint32_t enemy_mouser_leave_side(uint32_t object, uint32_t left_slot)
{
	uint32_t player_slot = 0;

	if (OBJECT(object)->enemy_mouser.approach != 1u)
		return left_slot;
	player_slot = object_loop_slot;
	enemy_mouser_give_side_back(object, player_slot);
	return player_slot;
}

/* gives its side of its turtle back */
void enemy_mouser_give_side_back(uint32_t object, uint32_t player_slot)
{
	uint8_t side_mask = 0;
	uint8_t side = 0;

	side_mask = 0x7Fu;
	side = OBJECT(object)->enemy_mouser.side;
	OBJECT(object)->enemy_mouser.side = side & 0x7Fu;
	if ((side & 0x80u) != 0u)
		side_mask = 0xB7u;
	PLAYER_SLOT(player_slot)->mouser_sides &= side_mask;
	OBJECT(object)->enemy_mouser.approach = 0u;
	OBJECT(object)->enemy_mouser.move_time = 0u;
}

/* leaps at its turtle and hangs on to it, gone when the turtle is free */
void enemy_mouser_bite(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy_mouser.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	states = enemy_mouser_bite_states_ADDRESS;
	object_loop_slot = slot;
	run_object_state(object, states);
}

/* the bite: leaps at its turtle along its arc, attacking */
void enemy_mouser_bite_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0, arc = 0;

	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	attack = 0x10u;
	enemy_face_target(attack, object);
	arc = enemy_mouser_bite_arc_ADDRESS;
	enemy_start_arc(object, arc);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->flags &= ~8u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_mouser.move = 0xA0u;
}

/* a frame of the leap; biting, it hides and hangs on; missing, it lands and waits 16 frames */
void enemy_mouser_bite_fly(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;
	uint32_t flags_halfword = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_mouser_bite_arc_ADDRESS;
	enemy_arc_step(object, arc);
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		flags_halfword = (((uint32_t)OBJECT(object)->flags << 8u) | OBJECT(object)->contacts) & 0x7100u;
		OBJECT(object)->flags = (uint8_t)(flags_halfword >> 8u);
		OBJECT(object)->contacts = (uint8_t)flags_halfword;
		OBJECT(object)->state = 3u;
		enemy_mouser_leave_side(object, 0u);
		return;
	}
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state = 2u;
	animation = enemy_mouser_in_air_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits after the missed bite, then back to thinking */
void enemy_mouser_bite_land(uint32_t object)
{
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_mouser.move_time;
	OBJECT(object)->enemy_mouser.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	enemy_back_to_thinking(object);
}

/* hangs on its turtle; once the turtle is no longer held, the Mouser is gone */
void enemy_mouser_bite_hold(uint32_t object)
{
	uint8_t t1 = 0;

	if ((frame_count & 3u) == 0u && OBJECT(0x60000u | OBJECT(object)->hit_object)->routine != 4u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* hit: lets go of the turtle it held and is knocked flying */
void enemy_mouser_hit(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t turtle = 0;
	uint8_t flags = 0;

	sound = 0xA8u;
	queue_sound(sound);
	flags = OBJECT(object)->flags;
	OBJECT(object)->flags = flags & ~8u;
	if ((flags & 8u) != 0u)
	{
		turtle = 0x60000u | OBJECT(object)->hit_object;
		if (OBJECT(turtle)->routine == 4u)
			OBJECT(turtle)->turtle.escape_presses = 0x10u;
	}
	OBJECT(object)->enemy_mouser.move = 0x90u;
	OBJECT(object)->enemy_mouser.move_time = 0u;
	OBJECT(object)->routine++;
	enemy_react(object);
}

/* a step over the floor's edge */
void enemy_mouser_step_over_edge(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy_mouser.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	states = TABLE32(mouser_edge_steps, (uint32_t)(int16_t)OBJECT(object)->enemy_mouser.move);
	object_loop_slot = slot;
	run_object_state(object, states);
}

/* hops over an edge above or below it on the screen, as high as the area says */
void enemy_mouser_hop_down(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height_speed = area_hop_heights[area & 0xFu];
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x100;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the hop, its floor raised by stairs; on landing a pose and 16 frames still */
void enemy_mouser_hop_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_up(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_mouser_in_air_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* still for a moment after landing, then back to thinking */
void enemy_mouser_hop_land(uint32_t object)
{
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_mouser.move_time;
	OBJECT(object)->enemy_mouser.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
}

/* hops over an edge to the right, as high as the area says */
void enemy_mouser_hop_right(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height_speed = area_hop_heights[area & 0xFu];
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags |= 0x40u;
	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* hops over an edge to the left, as high as the area says */
void enemy_mouser_hop_left(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height_speed = area_hop_heights[area & 0xFu];
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags &= 0xBFu;
	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* jumps over an edge above or below it on the screen */
void enemy_mouser_hop_up(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = -0x80;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	animation = enemy_mouser_hop_up_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump, its floor lowered by stairs; on landing a pose and 16 frames still */
void enemy_mouser_hop_up_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_mouser_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* jumps over an edge to the right */
void enemy_mouser_hop_up_right(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags |= 0x40u;
	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump, not into walls, its floor lowered by stairs; on landing a pose and 16 frames
   still */
void enemy_mouser_hop_up_fall_blocked(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_mouser_in_air_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* jumps over an edge to the left */
void enemy_mouser_hop_up_left(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->flags &= ~2u;
	OBJECT(object)->state++;
	OBJECT(object)->flags &= 0xBFu;
	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* chases its turtle, sitting now and then */
void enemy_mouser_chase(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_chase_states_ADDRESS;
	run_object_state(object, states);
}

/* chases its turtle for a random time under 240 frames */
void enemy_mouser_chase_start(uint8_t direction, uint32_t object)
{
	uint32_t chase_time = 0;

	OBJECT(object)->enemy_mouser.move_time = 0xFFu;
	OBJECT(object)->enemy_mouser.timer = 8u;
	chase_time = 0xF0u;
	chase_time = random_below((uint16_t)chase_time, object);
	OBJECT(object)->enemy_mouser.chase_time = chase_time;
	OBJECT(object)->state++;
	object_loop_leftover = mouser_walk_animations_ADDRESS;
	enemy_face_direction(direction, object);
}

/* a frame of the chase, aiming every 8 frames; close to its turtle or the time up, it sits */
void enemy_mouser_chase_run(uint32_t object, uint32_t player_slot)
{
	uint16_t speed = 0;
	uint32_t chase_time = 0, direction = 0, turtle = 0, x_speed = 0, y_speed = 0;
	uint8_t chase_time_2 = 0, timer = 0;

	if (OBJECT(object)->enemy_mouser.target_dx < 0x3Eu && OBJECT(object)->enemy_mouser.target_dy < 0x1Eu)
	{
		OBJECT(object)->state++;
		return;
	}
	chase_time_2 = OBJECT(object)->enemy_mouser.chase_time;
	OBJECT(object)->enemy_mouser.chase_time = chase_time_2 - 1u;
	if (chase_time_2 == 1u)
	{
		chase_time = 0xF0u;
		chase_time = random_below((uint16_t)chase_time, object);
		OBJECT(object)->enemy_mouser.chase_time = chase_time;
		OBJECT(object)->state++;
		return;
	}
	timer = OBJECT(object)->enemy_mouser.timer;
	OBJECT(object)->enemy_mouser.timer = timer - 1u;
	if (timer == 1u)
	{
		turtle = PLAYER_SLOT(player_slot)->turtle;
		direction = direction_to_object(0u, object, turtle);
		object_loop_leftover = mouser_walk_animations_ADDRESS;
		enemy_turn_to_direction((uint8_t)direction, object);
		speed = 0xC0u;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = (int16_t)y_speed >> 1;
		OBJECT(object)->enemy_mouser.timer = 8u;
	}
	move_object_blocked(object);
}

/* sits for 120 frames */
void enemy_mouser_chase_pause(uint32_t object)
{
	OBJECT(object)->enemy_mouser.timer = 0x78u;
	enemy_mouser_sit(object);
}

/* sits, then chases again */
void enemy_mouser_chase_wait(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_mouser.timer;
	OBJECT(object)->enemy_mouser.timer = timer - 1u;
	if (timer == 1u)
		OBJECT(object)->state = 0u;
}

/* creeps to its side of its turtle */
void enemy_mouser_creep_to_side(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_creep_to_side_states_ADDRESS;
	run_object_state(object, states);
}

/* creeps to its side of its turtle, aiming in 8 frames */
void enemy_mouser_creep_to_side_start(uint8_t direction, uint32_t object)
{
	OBJECT(object)->enemy_mouser.move_time = 0xFFu;
	OBJECT(object)->enemy_mouser.timer = 8u;
	OBJECT(object)->state++;
	object_loop_leftover = mouser_walk_animations_ADDRESS;
	enemy_face_direction(direction, object);
}

/* a frame of creeping to 0x20 beside its turtle, aiming every 8 frames */
void enemy_mouser_creep_to_side_step(uint32_t object, uint32_t player_slot)
{
	uint16_t speed = 0, target_x = 0, target_y = 0;
	uint32_t direction = 0, object_2 = 0, x_speed = 0, y_speed = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_mouser.timer;
	OBJECT(object)->enemy_mouser.timer = timer - 1u;
	if (timer != 1u)
	{
		move_object_blocked(object);
		return;
	}
	object_2 = PLAYER_SLOT(player_slot)->turtle;
	target_x = 0x20u;
	if ((OBJECT(object)->enemy_mouser.side & 0x80u) != 0u)
		target_x = (uint16_t)(0u - target_x);
	target_x += OBJECT(object_2)->screen_x;
	target_y = OBJECT(object_2)->screen_y;
	direction = direction_to(0u, target_x, target_y, object);
	object_loop_leftover = mouser_walk_animations_ADDRESS;
	enemy_turn_to_direction((uint8_t)direction, object);
	speed = 0x80u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = (int16_t)y_speed >> 1;
	OBJECT(object)->enemy_mouser.timer = 8u;
	move_object_blocked(object);
}

/* runs away, sits and waits while its turtle is down */
void enemy_mouser_back_off(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_back_off_states_ADDRESS;
	run_object_state(object, states);
}

/* 32 frames of running away from its turtle */
void enemy_mouser_back_off_start(uint32_t object, uint32_t player_slot)
{
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_mouser.move_time = 0xFFu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_mouser.timer = 0x20u;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	direction = direction_to_object(0u, object, turtle);
	direction = (uint8_t)(direction - 0x80u);
	speed = 0x100u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	object_loop_leftover = mouser_walk_animations_ADDRESS;
	enemy_face_direction((uint8_t)direction, object);
}

/* a frame of running away; at its end it sits */
void enemy_mouser_back_off_run(uint8_t direction, uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_mouser.timer;
	OBJECT(object)->enemy_mouser.timer = timer - 1u;
	if (timer == 1u)
		OBJECT(object)->state++;
	object_loop_leftover = mouser_walk_animations_ADDRESS;
	enemy_turn_to_direction(direction, object);
	move_object_blocked(object);
}

/* sits in one of two poses */
void enemy_mouser_sit(uint32_t object)
{
	uint32_t animation = 0, range = 0;

	OBJECT(object)->state++;
	range = 2u;
	range = random_below((uint16_t)range, object);
	animation = TABLE32(enemy_mouser_sit_animations, (uint16_t)(range << 2));
	set_animation(0u, object, animation);
}

/* sits while its turtle is down; when it is up again, back to thinking with a new move */
void enemy_mouser_back_off_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_mouser.approach = 0u;
		OBJECT(object)->enemy_mouser.move_time = 0u;
	}
}

/* thrown off by the turtle it held */
void enemy_mouser_thrown_off(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_thrown_off_states_ADDRESS;
	run_object_state(object, states);
}

/* thrown off by the turtle it held: shown again, tumbling along its arc the way it faced */
void enemy_mouser_thrown_off_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->flags &= 0xBFu;
	if ((OBJECT(object)->enemy_mouser.facing & 0x80u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_mouser_knocked_flying_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_mouser_tumble_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* hops away and breaks when the boss is beaten */
void enemy_mouser_break_down(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_break_down_states_ADDRESS;
	run_object_state(object, states);
}

/* its boss down, it hops off tumbling along its arc and breaks */
void enemy_mouser_break_down_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_mouser_break_down_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_mouser_tumble_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* knocked flying by a hit */
void enemy_mouser_knocked_flying(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_mouser_knocked_flying_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked flying by a hit, tumbling along its arc, its side given up */
void enemy_mouser_knocked_flying_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	enemy_mouser_leave_side(object, 0u);
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_mouser.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_mouser_knocked_flying_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_mouser_tumble_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* flies until it hits the floor and breaks */
void enemy_mouser_fly_and_crash(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height(object);
	arc = enemy_mouser_knocked_flying_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	sound = 0xFu;
	queue_sound(sound);
	OBJECT(object)->state++;
	animation = enemy_mouser_fly_and_crash_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* gone when its wreck's animation ends */
void enemy_mouser_remove(uint32_t object, uint32_t player_slot)
{
	uint8_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		PLAYER_SLOT(player_slot)->enemies--;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* a wall with Mousers behind it */
void mouser_hole_start(uint32_t object)
{
	uint32_t animation_entry = 0, hole = 0;

	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->routine++;
	OBJECT(object)->mouser_hole.mousers_left = mouser_hole_counts[(uint32_t)(uint16_t)((OBJECT(object)->parameter & 0xFu) << 3) + difficulty];
	hole = TABLE32(mouser_hole_animation_tables, (OBJECT(object)->parameter & 0xF0u) >> 2);
	OBJECT(object)->mouser_hole.hole = hole;
	animation_entry = ANIMATION_ENTRY(hole);
	set_animation(0u, object, animation_entry);
}

/* on the screen, the wall breaks open */
void mouser_hole_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation_entry = 0, hole = 0;

	if (OBJECT(object)->screen_x < 0x100u)
	{
		enemies_on_screen += 6u;
		hole = OBJECT(object)->mouser_hole.hole;
		animation_entry = ANIMATION_ENTRY(hole + 0x4u);
		set_animation(0u, object, animation_entry);
		OBJECT(object)->mouser_hole.hole = hole + 8u;
		OBJECT(object)->routine++;
		sound = 0x31u;
		queue_sound(sound);
	}
}

/* the broken wall is down */
void mouser_hole_open(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->mouser_hole.spawn_time = 0x10u;
		OBJECT(object)->flags &= 1u;
		OBJECT(object)->routine++;
	}
}

/* Mousers come out of the hole one after another until the last */
void mouser_hole_let_out(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, record = 0;
	uint8_t spawn_time = 0, mousers_left = 0, t1 = 0;

	spawn_time = OBJECT(object)->mouser_hole.spawn_time;
	OBJECT(object)->mouser_hole.spawn_time = spawn_time - 1u;
	if (spawn_time == 1u)
	{
		object_loop_leftover = OBJECT(object)->mouser_hole.hole;
		record = object_loop_leftover;
		start_object_from_record(object, record, &ignored, &zero);
		OBJECT(object)->mouser_hole.spawn_time = 0x3Cu;
		mousers_left = OBJECT(object)->mouser_hole.mousers_left;
		OBJECT(object)->mouser_hole.mousers_left = mousers_left - 1u;
		if (mousers_left == 1u)
		{
			OBJECT(object)->flags = 0u;
			OBJECT(object)->contacts = 0u;
			t1 = enemies_on_screen - 6u;
			enemies_on_screen = t1;
			if ((int8_t)t1 < 0)
				enemies_on_screen = 0u;
		}
	}
}

/* a Roadkill Rodney: one bar, coming up through the floor */
void enemy_rodney_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy_rodney.target = player_slot;
	OBJECT(object)->enemy_rodney.retarget_time = 0x10u;
	OBJECT(object)->enemy_rodney.move_time = 0u;
	OBJECT(object)->enemy_rodney.approach = 0u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->floor_height = 0u;
	enemies_on_screen++;
	OBJECT(object)->health_bars = 1u;
	OBJECT(object)->health = 0xCu;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	if ((int8_t)OBJECT(object)->parameter >= 0)
	{
		OBJECT(object)->flags |= 0x82u;
		OBJECT(object)->routine += 2u;
		animation = enemy_rodney_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->frame_time = 0u;
	OBJECT(object)->routine++;
	OBJECT(object)->enemy_rodney.wait = 0xB4u;
}

/* comes up through the floor with a jump and lands */
void enemy_rodney_enter(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_enter_states_ADDRESS;
	run_object_state(object, states);
}

/* once on the screen, or when its wait runs out, a sound and the floor cracks */
void enemy_rodney_enter_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;
	uint8_t wait = 0;

	if (OBJECT(object)->screen_x >= 0x140u)
	{
		wait = OBJECT(object)->enemy_rodney.wait;
		OBJECT(object)->enemy_rodney.wait = wait - 1u;
		if (wait != 1u)
			return;
	}
	sound = 0x2Eu;
	queue_sound(sound);
	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_rodney_enter_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the cracks end, it bursts up out of the floor with a jump, hittable */
void enemy_rodney_enter_burst_out(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->height = 0x1800u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x10;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_rodney_enter_burst_out_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the jump; down on its floor, its landing pose */
void enemy_rodney_enter_fall(uint32_t object)
{
	uint8_t height_whole = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	height_whole = OBJECT(object)->height_whole;
	if (height_whole <= OBJECT(object)->floor_height || height_whole >= 0xF8u)
	{
		OBJECT(object)->height = 0u;
		OBJECT(object)->state++;
		OBJECT(object)->contacts &= 0x7Fu;
		animation = enemy_rodney_enter_fall_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
}

/* when the landing pose ends, to thinking */
void enemy_rodney_enter_land(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
		OBJECT(object)->routine++;
}

/* goes after the turtle with the fewest enemies: rushes at it, whips or shoots it; gone when the area is
   cleared */
void enemy_rodney_think(uint32_t object, uint32_t second_slot)
{
	bool L_051E78 = false;
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint16_t approach = 0, count = 0;
	uint32_t attack = 0, chance = 0, closest_slot = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, turtle = 0;
	uint8_t retarget_time = 0, t2 = 0, rodney_sides = 0, rodney_sides_2 = 0;
	uint16_t t3 = 0;
	uint32_t t1 = 0;

	slot = 0x60000u | OBJECT(object)->enemy_rodney.target;
	retarget_time = OBJECT(object)->enemy_rodney.retarget_time;
	OBJECT(object)->enemy_rodney.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_rodney.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			PLAYER_SLOT(slot)->enemies--;
			second_slot = enemy_nearest_slots(object, second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = object_loop_leftover;
				closest_slot = second_slot;
				object_loop_leftover = closest_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						object_loop_leftover = second_slot;
						second_slot = closest_slot;
					}
				}
			}
			if ((uint16_t)second_slot != OBJECT(object)->enemy_rodney.target)
			{
				object_loop_slot = slot;
				enemy_rodney_leave_side(object, slot);
			}
			OBJECT(object)->enemy_rodney.target = second_slot;
			slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	if (boss_defeated != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t2 = enemies_on_screen - 1u;
		enemies_on_screen = t2;
		if ((int8_t)t2 < 0)
			enemies_on_screen = 0u;
		object_loop_slot = slot;
		return;
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		object_loop_slot = slot;
		slot = enemy_rodney_leave_side(object, slot);
		PLAYER_SLOT(slot)->enemies--;
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t2 = enemies_on_screen - 1u;
		enemies_on_screen = t2;
		if ((int8_t)t2 < 0)
			enemies_on_screen = 0u;
		object_loop_slot = slot;
		return;
	}
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	if (OBJECT(object)->enemy_rodney.approach != 0u)
	{
		if (OBJECT(object)->enemy_rodney.approach == 1u && ((int16_t)OBJECT(object)->screen_x < 0x70 || (int16_t)OBJECT(object)->screen_x >= 0x190))
		{
			OBJECT(object)->enemy_rodney.approach = 0u;
			OBJECT(object)->enemy_rodney.move_time = 0u;
			enemy_rodney_give_side_back(object, slot);
		}
	}
	else if (PLAYER_SLOT(slot)->rodney_sides < 0xC0u && OBJECT(object)->enemy_rodney.target_dx < 0x20u)
	{
		OBJECT(object)->enemy_rodney.approach = 1u;
		OBJECT(object)->enemy_rodney.move_time = 0u;
		do
		{
			if (L_051E78 || (int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
			{
				for (;;)
				{
					L_051E78 = false;
					rodney_sides = PLAYER_SLOT(slot)->rodney_sides;
					PLAYER_SLOT(slot)->rodney_sides = rodney_sides | 0x40u;
					if ((rodney_sides & 0x40u) != 0u)
					{
						rodney_sides_2 = PLAYER_SLOT(slot)->rodney_sides;
						PLAYER_SLOT(slot)->rodney_sides = rodney_sides_2 | 0x80u;
						if ((rodney_sides_2 & 0x80u) != 0u)
							continue;
						OBJECT(object)->enemy_rodney.side &= 0x7Fu;
						break;
					}
					OBJECT(object)->enemy_rodney.side |= 0x80u;
					break;
				}
			}
			else
			{
				rodney_sides_2 = PLAYER_SLOT(slot)->rodney_sides;
				PLAYER_SLOT(slot)->rodney_sides = rodney_sides_2 | 0x80u;
				L_051E78 = (rodney_sides_2 & 0x80u) != 0u;
				if (!L_051E78)
					OBJECT(object)->enemy_rodney.side &= 0x7Fu;
			}
		} while (L_051E78);
	}
	if (OBJECT(object)->enemy_rodney.approach != 1u)
	{
		zero = enemy_on_screen(object);
		if (zero && OBJECT(object)->enemy_rodney.approach == 1u && OBJECT(PLAYER_SLOT(slot)->turtle)->routine != 4u && OBJECT(object)->enemy_rodney.target_dy < 6u)
		{
			chance = 0x64u;
			chance = random_below((uint16_t)chance, object);
			count = 1u;
			attack = rodney_attacks_ADDRESS;
			do
			{
				if (RODNEY_ATTACK(attack)->reach <= OBJECT(object)->enemy_rodney.target_dx && (uint8_t)chance < RODNEY_ATTACK(attack)->chance)
				{
					OBJECT(object)->enemy_rodney.move = (uint32_t)RODNEY_ATTACK(attack)->move << 2;
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					object_loop_slot = slot;
					return;
				}
				t3 = count - 1u;
				count = t3;
				attack += 4u;
			} while (t3 != 0xFFFFu);
		}
	}
	else
	{
		turtle = PLAYER_SLOT(slot)->turtle;
		if (OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u)
		{
			zero = enemy_on_screen(object);
			if (zero && OBJECT(object)->enemy_rodney.approach == 1u && OBJECT(PLAYER_SLOT(slot)->turtle)->routine != 4u && OBJECT(object)->enemy_rodney.target_dy < 6u)
			{
				chance = 0x64u;
				chance = random_below((uint16_t)chance, object);
				count = 1u;
				attack = rodney_attacks_ADDRESS;
				do
				{
					if (RODNEY_ATTACK(attack)->reach <= OBJECT(object)->enemy_rodney.target_dx && (uint8_t)chance < RODNEY_ATTACK(attack)->chance)
					{
						OBJECT(object)->enemy_rodney.move = (uint32_t)RODNEY_ATTACK(attack)->move << 2;
						OBJECT(object)->routine = 3u;
						OBJECT(object)->state = 0u;
						OBJECT(object)->flags &= ~8u;
						object_loop_slot = slot;
						return;
					}
					t3 = count - 1u;
					count = t3;
					attack += 4u;
				} while (t3 != 0xFFFFu);
			}
		}
		else
		{
			enemy_rodney_give_side_back(object, slot);
			OBJECT(object)->enemy_rodney.approach = 2u;
			OBJECT(object)->enemy_rodney.move_time = 0u;
		}
	}
	if (OBJECT(object)->enemy_rodney.move_time == 0u)
	{
		approach = OBJECT(object)->enemy_rodney.approach;
		OBJECT(object)->enemy_rodney.move = (uint32_t)TABLE8(rodney_approach_moves, approach) << 2;
		OBJECT(object)->state = 0u;
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_rodney.move)
	{
	case 20:
		object_loop_slot = slot;
		enemy_shuffle(object);
		return;
	case 28:
		object_loop_slot = slot;
		enemy_circle(object);
		return;
	case 36:
		object_loop_slot = slot;
		enemy_charge(object);
		return;
	case 0:
		object_loop_slot = slot;
		enemy_walk_to_target(object);
		return;
	case 4:
		object_loop_slot = slot;
		enemy_walk_to_side(object);
		return;
	case 8:
		object_loop_slot = slot;
		enemy_keep_distance(object);
		return;
	case 12:
		object_loop_slot = slot;
		enemy_back_off(object);
		return;
	case 16:
		object_loop_slot = slot;
		enemy_run_away(object);
		return;
	case 44:
		object_loop_slot = slot;
		enemy_walk_across(object);
		return;
	case 48:
		object_loop_slot = slot;
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		object_loop_slot = slot;
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		object_loop_slot = slot;
		enemy_walk_past_sword(object);
		return;
	case 32:
		object_loop_slot = slot;
		enemy_back_off_sword(object);
		return;
	case 40:
		object_loop_slot = slot;
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		object_loop_slot = slot;
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		object_loop_slot = slot;
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		object_loop_slot = slot;
		enemy_stalk_hammer(object);
		return;
	case 64:
		object_loop_slot = slot;
		enemy_walk_away_hammer(object);
		return;
	case 72:
		object_loop_slot = slot;
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		object_loop_slot = slot;
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		object_loop_slot = slot;
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		object_loop_slot = slot;
		enemy_back_off_spear(object);
		return;
	case 112:
		object_loop_slot = slot;
		enemy_step_aside_spear(object);
		return;
	case 80:
		object_loop_slot = slot;
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		object_loop_slot = slot;
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		object_loop_slot = slot;
		enemy_circle_knife(object);
		return;
	case 124:
		object_loop_slot = slot;
		enemy_keep_distance_knife(object);
		return;
	case 120:
		object_loop_slot = slot;
		enemy_back_off_knife(object);
		return;
	case 128:
		object_loop_slot = slot;
		enemy_step_aside_knife(object);
		return;
	case 88:
		object_loop_slot = slot;
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		object_loop_slot = slot;
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		object_loop_slot = slot;
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		object_loop_slot = slot;
		enemy_back_off_gun(object);
		return;
	case 144:
		object_loop_slot = slot;
		enemy_run_away_gun(object);
		return;
	case 96:
		object_loop_slot = slot;
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		object_loop_slot = slot;
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		object_loop_slot = slot;
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		object_loop_slot = slot;
		enemy_back_off_axe(object);
		return;
	case 136:
		object_loop_slot = slot;
		enemy_run_away_axe(object);
		return;
	case 196:
		object_loop_slot = slot;
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		object_loop_slot = slot;
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		object_loop_slot = slot;
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		object_loop_slot = slot;
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		object_loop_slot = slot;
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		object_loop_slot = slot;
		enemy_rocksteady_circle(object);
		return;
	case 208:
		object_loop_slot = slot;
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		object_loop_slot = slot;
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		object_loop_slot = slot;
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		object_loop_slot = slot;
		enemy_bebop_stand(object);
		return;
	case 216:
		object_loop_slot = slot;
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		object_loop_slot = slot;
		enemy_bebop_walk_level(object);
		return;
	case 220:
		object_loop_slot = slot;
		enemy_bebop_close_in(object);
		return;
	case 156:
		object_loop_slot = slot;
		enemy_mouser_chase(object);
		return;
	case 164:
		object_loop_slot = slot;
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		object_loop_slot = slot;
		enemy_mouser_back_off(object);
		return;
	case 172:
		object_loop_slot = slot;
		enemy_rodney_rush(object);
		return;
	case 176:
		object_loop_slot = slot;
		enemy_rodney_rush_across(object);
		return;
	case 180:
		object_loop_slot = slot;
		enemy_rodney_back_off(object);
		return;
	case 236:
		object_loop_slot = slot;
		enemy_shredder_appear(object);
		return;
	case 240:
		object_loop_slot = slot;
		enemy_shredder_split(object);
		return;
	case 168:
		object_loop_slot = slot;
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		object_loop_slot = slot;
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		object_loop_slot = slot;
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		object_loop_slot = slot;
		enemy_shredder_guard(object);
		return;
	case 256:
		object_loop_slot = slot;
		enemy_shredder_back_off(object);
		return;
	}
}

/* hit: knocked back, or destroyed with no health left */
void enemy_rodney_hit(uint32_t object)
{
	uint8_t sound = 0;
	uint16_t move = 0;

	sound = 5u;
	queue_sound(sound);
	move = 0x40u;
	if (OBJECT(object)->health == 0u)
		move = 0x74u;
	OBJECT(object)->enemy_rodney.move = move;
	OBJECT(object)->enemy_rodney.move_time = 0u;
	OBJECT(object)->routine++;
	enemy_react(object);
}

/* gives its side of its turtle back when it stands by it */
uint32_t enemy_rodney_leave_side(uint32_t object, uint32_t left_slot)
{
	uint32_t player_slot = 0;

	if (OBJECT(object)->enemy_rodney.approach != 1u)
		return left_slot;
	player_slot = object_loop_slot;
	enemy_rodney_give_side_back(object, player_slot);
	return player_slot;
}

/* gives its side of its turtle back */
void enemy_rodney_give_side_back(uint32_t object, uint32_t player_slot)
{
	uint8_t side_mask = 0;
	uint8_t side = 0;

	side_mask = 0x7Fu;
	side = OBJECT(object)->enemy_rodney.side;
	OBJECT(object)->enemy_rodney.side = side & 0x7Fu;
	if ((side & 0x80u) != 0u)
		side_mask = 0xB7u;
	PLAYER_SLOT(player_slot)->rodney_sides &= side_mask;
	OBJECT(object)->enemy_rodney.approach = 0u;
	OBJECT(object)->enemy_rodney.move_time = 0u;
}

/* rushes at and past its turtle, then brakes */
void enemy_rodney_rush(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_rush_states_ADDRESS;
	run_object_state(object, states);
}

/* rushes at its turtle for 180 frames, aiming just off its row when it is close to it */
void enemy_rodney_rush_start(uint32_t object, uint32_t player_slot)
{
	uint16_t screen_x = 0, target_y = 0;
	uint32_t animation = 0, direction = 0, object_2 = 0, speed = 0, x_speed = 0, y_speed = 0;

	direction = 0x80u;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
	{
		direction = 0u;
		OBJECT(object)->flags |= 0x40u;
	}
	if (OBJECT(object)->enemy_rodney.target_dy < 0x20u)
	{
		target_y = 0x20u;
		if ((int8_t)OBJECT(object)->enemy_rodney.target_direction < 0)
			target_y = (uint16_t)(0u - target_y);
		object_2 = PLAYER_SLOT(player_slot)->turtle;
		screen_x = OBJECT(object_2)->screen_x;
		target_y += OBJECT(object_2)->screen_y;
		direction = direction_to(direction, screen_x, target_y, object);
	}
	speed = 0x300u;
	speeds_from_direction((uint8_t)direction, (uint16_t)speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->enemy_rodney.move_time = 0xB4u;
	OBJECT(object)->state++;
	animation = enemy_rodney_animation_ADDRESS;
	set_animation(speed, object, animation);
}

/* a frame of the rush, sliding along walls; well past its turtle it brakes, or stops at a wall */
void enemy_rodney_rush_run(uint32_t object)
{
	uint8_t clear_ahead = 0;
	uint32_t animation = 0;

	OBJECT(object)->enemy_rodney.move_time--;
	clear_ahead = 0xFFu;
	offset_high_word = 0xFFFFFFFFu;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		if ((OBJECT(object)->contacts & 8u) != 0u)
		{
			OBJECT(object)->y -= 0x100u;
			clear_ahead = 0u;
		}
	}
	else if ((OBJECT(object)->contacts & 0x10u) != 0u)
	{
		OBJECT(object)->y -= 0x100u;
		clear_ahead = 0u;
	}
	if ((int8_t)((OBJECT(object)->enemy_rodney.target_direction - 0x40u) ^ ((uint32_t)OBJECT(object)->flags << 1)) >= 0)
	{
		move_object_blocked(object);
		return;
	}
	if ((int8_t)clear_ahead >= 0)
		OBJECT(object)->enemy_rodney.move_time = 0u;
	if (OBJECT(object)->enemy_rodney.target_dx < 0x50u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rodney.brake = 0x20;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->enemy_rodney.brake = 0u - OBJECT(object)->enemy_rodney.brake;
	OBJECT(object)->y_speed = -0x40;
	animation = enemy_rodney_rush_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->enemy_rodney.target_direction < 0)
	{
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
		animation = enemy_rodney_rush_dust_animation_ADDRESS;
	}
	set_animation(0u, object, animation);
}

/* brakes to a stop, then thinks */
void enemy_rodney_brake(uint32_t object)
{
	OBJECT(object)->x_speed += OBJECT(object)->enemy_rodney.brake;
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rodney.move_time = 0u;
	}
	move_object_blocked(object);
}

/* rushes across past its turtle, then brakes toward its row */
void enemy_rodney_rush_across(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_rush_across_states_ADDRESS;
	run_object_state(object, states);
}

/* rushes straight across at its turtle for 180 frames */
void enemy_rodney_rush_across_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->x_speed = -0x300;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		OBJECT(object)->flags |= 0x40u;
	}
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rodney.move_time = 0xB4u;
	animation = enemy_rodney_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the rush across, sliding along walls; well past its turtle it brakes toward its row */
void enemy_rodney_rush_across_run(uint32_t object)
{
	uint8_t clear_ahead = 0;
	uint32_t animation = 0;

	OBJECT(object)->enemy_rodney.move_time--;
	clear_ahead = 0xFFu;
	offset_high_word = 0xFFFFFFFFu;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		if ((OBJECT(object)->contacts & 8u) != 0u)
		{
			OBJECT(object)->y -= 0x100u;
			clear_ahead = 0u;
		}
	}
	else if ((OBJECT(object)->contacts & 0x10u) != 0u)
	{
		OBJECT(object)->y -= 0x100u;
		clear_ahead = 0u;
	}
	if ((int8_t)((OBJECT(object)->enemy_rodney.target_direction - 0x40u) ^ ((uint32_t)OBJECT(object)->flags << 1)) >= 0)
	{
		move_object_blocked(object);
		return;
	}
	if ((int8_t)clear_ahead >= 0)
		OBJECT(object)->enemy_rodney.move_time = 0u;
	if (OBJECT(object)->enemy_rodney.target_dx < 0x50u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rodney.brake = 0x20;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		OBJECT(object)->enemy_rodney.brake = 0u - OBJECT(object)->enemy_rodney.brake;
	OBJECT(object)->y_speed = (uint32_t)OBJECT(object)->enemy_rodney.target_dy << 3;
	animation = enemy_rodney_rush_dust_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->enemy_rodney.target_direction >= 0)
	{
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
		animation = enemy_rodney_rush_animation_ADDRESS;
	}
	set_animation(0u, object, animation);
}

/* slides back and waits while its turtle is down */
void enemy_rodney_back_off(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_back_off_states_ADDRESS;
	run_object_state(object, states);
}

/* slides back from its turtle with a sound */
void enemy_rodney_back_off_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x8Cu;
	queue_sound(sound);
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->enemy_rodney.brake = -0x20;
	OBJECT(object)->x_speed = 0x200;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
	{
		OBJECT(object)->enemy_rodney.brake = 0u - OBJECT(object)->enemy_rodney.brake;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rodney.move_time = 0xB4u;
	animation = enemy_rodney_back_off_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slows its slide to a stop, saying "Do not resist us" once */
void enemy_rodney_slide_to_stop(uint32_t object)
{
	uint16_t on_right = 0, screen_y = 0;
	uint32_t animation = 0, screen_x = 0;
	uint16_t t1 = 0;

	on_right = (uint16_t)OBJECT(object)->enemy_rodney.brake;
	t1 = OBJECT(object)->x_speed + on_right;
	OBJECT(object)->x_speed = t1;
	if (t1 != 0u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->state++;
	if (rodney_line_said == 0u)
	{
		on_right = (uint16_t)(on_right | 0xFFu);
		screen_x = OBJECT(object)->screen_x;
		screen_y = OBJECT(object)->screen_y;
		say_do_not_resist_us(on_right, (uint16_t)screen_x, screen_y);
		rodney_line_said = 0xFFu;
	}
	animation = enemy_rodney_slide_to_stop_animation_ADDRESS;
	set_animation(screen_x, object, animation);
}

/* when the slide ends, turns to its turtle */
void enemy_rodney_back_off_turn(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->state++;
}

/* waits while its turtle is down; when it is up again, a new move */
void enemy_rodney_back_off_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_rodney.approach = 0u;
		OBJECT(object)->enemy_rodney.move_time = 0u;
	}
}

/* slides back, turns and fires three laser bolts */
void enemy_rodney_shoot(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_shoot_states_ADDRESS;
	run_object_state(object, states);
}

/* the laser: slides back from its turtle with a sound */
void enemy_rodney_shoot_start(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	sound = 0x8Cu;
	queue_sound(sound);
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->flags &= 0xBFu;
	OBJECT(object)->enemy_rodney_shoot.brake = 0x20;
	OBJECT(object)->x_speed = -0x200;
	if ((int8_t)(OBJECT(object)->enemy_rodney_shoot.target_direction - 0x40u) < 0)
	{
		OBJECT(object)->flags |= 0x40u;
		OBJECT(object)->enemy_rodney_shoot.brake = 0u - OBJECT(object)->enemy_rodney_shoot.brake;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	OBJECT(object)->state++;
	animation = enemy_rodney_back_off_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the slide ends, turns to its turtle */
void enemy_rodney_shoot_turn(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney_shoot.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->state++;
	animation = enemy_rodney_shoot_turn_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the turn ends, a sound and three bolts to go */
void enemy_rodney_shoot_ready(uint32_t object)
{
	uint8_t sound = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_rodney_shoot.shot_time = 1u;
		OBJECT(object)->enemy_rodney_shoot.shots_left = 3u;
		OBJECT(object)->state++;
		sound = 0x14u;
		queue_sound(sound);
	}
}

/* every 4 frames a laser bolt the way it faces; after the third, or with no free object, its last pose */
void enemy_rodney_shoot_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_rodney_shoot.shot_time;
	OBJECT(object)->enemy_rodney_shoot.shot_time = shot_time - 1u;
	if (shot_time != 1u)
		return;
	OBJECT(object)->enemy_rodney_shoot.shot_time = 4u;
	object_loop_leftover = laser_bolt_shot_record_ADDRESS;
	record = laser_bolt_shot_record_ADDRESS;
	start_shot(object, record, &object_2, &zero);
	if (zero)
	{
		OBJECT(object_2)->parameter = (uint32_t)OBJECT(object)->flags << 1;
		OBJECT(object_2)->hit_sound = OBJECT(object)->enemy_rodney_shoot.move;
		shots_left = OBJECT(object)->enemy_rodney_shoot.shots_left;
		OBJECT(object)->enemy_rodney_shoot.shots_left = shots_left - 1u;
		if (shots_left != 1u)
			return;
	}
	OBJECT(object)->state++;
	animation = enemy_rodney_shoot_fire_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, back to thinking with a new move and target */
void enemy_rodney_shoot_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rodney_shoot.move_time = 0u;
		OBJECT(object)->enemy_rodney_shoot.retarget_time = 0u;
	}
}

/* throws its whip arm at its turtle and lashes it with it */
void enemy_rodney_whip(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_whip_states_ADDRESS;
	run_object_state(object, states);
}

/* the whip: following its turtle's row, it slides back */
void enemy_rodney_whip_start(uint32_t object)
{
	enemy_rodney_follow_row(object);
	enemy_rodney_shoot_start(object);
}

/* slides to a stop, following its turtle's row */
void enemy_rodney_whip_slow_down(uint32_t object)
{
	enemy_rodney_follow_row(object);
	enemy_rodney_slide_to_stop(object);
}

/* when the slide ends, faces its turtle and winds up with a sound */
void enemy_rodney_whip_wind_up(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	enemy_rodney_follow_row(object);
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney_whip.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->state++;
	sound = 0x2Au;
	queue_sound(sound);
	animation = enemy_rodney_whip_wind_up_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the wind-up ends, throws its arm out; with no free object, back to thinking */
void enemy_rodney_whip_throw_arm(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arm = 0, record = 0;

	enemy_rodney_follow_row(object);
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rodney_whip.arm_flags = 0u;
	OBJECT(object)->enemy_rodney_whip.arm_length = 0u;
	object_loop_leftover = whip_arm_shot_record_ADDRESS;
	record = whip_arm_shot_record_ADDRESS;
	start_shot(object, record, &arm, &zero);
	if (zero == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rodney_whip.move_time = 0u;
		OBJECT(object)->enemy_rodney_whip.retarget_time = 0u;
		return;
	}
	OBJECT(object)->enemy_rodney_whip.arm = arm;
	OBJECT(arm)->whip_arm.rodney = object;
	animation = enemy_rodney_whip_throw_arm_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* keeps toward its turtle's row */
void enemy_rodney_follow_row(uint32_t object)
{
	uint16_t y_step = 0;

	y_step = 0x200u;
	if (OBJECT(object)->enemy_rodney.target_dy < 3u)
		return;
	if ((int8_t)OBJECT(object)->enemy_rodney.target_direction >= 0)
		y_step = (uint16_t)(0u - y_step);
	OBJECT(object)->y += (int16_t)y_step;
}

/* the arm reaches on up to its full length and is drawn in; when it hits, the lash at the turtle's
   distance */
void enemy_rodney_whip_reach(uint32_t object)
{
	uint8_t arm_length = 0, x_distance = 0;
	uint32_t animation = 0, arm = 0, caught = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;

	enemy_rodney_follow_row(object);
	arm = OBJECT(object)->enemy_rodney_whip.arm;
	if ((OBJECT(arm)->flags & 8u) == 0u)
	{
		if ((OBJECT(object)->enemy_rodney_whip.arm_flags & 2u) != 0u)
		{
			enemy_rodney_whip_pull_in(object);
			return;
		}
		OBJECT(object)->enemy_rodney_whip.arm_length++;
		if (OBJECT(object)->enemy_rodney_whip.arm_length != 0xDu)
			return;
		OBJECT(object)->enemy_rodney_whip.arm_length--;
		enemy_rodney_whip_pull_in(object);
		return;
	}
	caught = 0x60000u | OBJECT(arm)->hit_object;
	t1 = OBJECT(caught)->screen_x - OBJECT(object)->screen_x;
	x_distance = (uint8_t)t1;
	if ((int16_t)t1 < 0)
		x_distance = (uint8_t)(uint16_t)(0u - x_distance);
	t2 = x_distance;
	arm_length = (uint8_t)(t2 - 0x30u);
	if (t2 < 0x30u)
		arm_length = 0u;
	arm_length = arm_length >> 3;
	if (arm_length >= 0xCu)
		arm_length = 0xCu;
	OBJECT(object)->enemy_rodney_whip.arm_length = arm_length;
	OBJECT(object)->enemy_rodney_whip.arm_flags |= 0x20u;
	OBJECT(object)->state = 5u;
	animation = enemy_rodney_whip_animation_ADDRESS;
	offset_high_word = caught;
	set_animation(0u, object, animation);
}

/* the arm drawn in, its pose by how far out it is */
void enemy_rodney_whip_pull_in(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_rodney_whip.arm_flags |= 0x40u;
	animation = TABLE32(whip_arm_animations, (uint32_t)(int16_t)((uint32_t)(int8_t)OBJECT(object)->enemy_rodney_whip.arm_length << 2));
	set_animation(0u, object, animation);
	OBJECT(object)->state = 6u;
}

/* the lash; when it is over, the arm drawn in */
void enemy_rodney_whip_lash(uint32_t object)
{
	offset_high_word = OBJECT_ADDRESS(0);
	if ((OBJECT(object)->enemy_rodney_whip.arm_flags & 4u) != 0u)
	{
		enemy_rodney_whip_pull_in(object);
		return;
	}
}

/* the arm shortened every frame; drawn in, back to thinking with a new move and target */
void enemy_rodney_whip_retract(uint32_t object)
{
	uint8_t t1 = 0;

	t1 = OBJECT(object)->enemy_rodney_whip.arm_length - 1u;
	OBJECT(object)->enemy_rodney_whip.arm_length = t1;
	if ((int8_t)t1 < 0)
	{
		OBJECT(object)->enemy_rodney_whip.arm_flags |= 0x80u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rodney_whip.move_time = 0u;
		OBJECT(object)->enemy_rodney_whip.retarget_time = 0u;
	}
}

/* knocked back by a hit */
void enemy_rodney_knocked_back(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_knocked_back_states_ADDRESS;
	run_object_state(object, states);
}

/* knocked back along its arc, facing its turtle, its side given up */
void enemy_rodney_knocked_back_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_rodney_destroyed_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_rodney_knocked_back_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	enemy_rodney_leave_side(object, 0u);
}

/* a frame of the fall; on landing its pose */
void enemy_rodney_knocked_back_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_rodney_knocked_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->state++;
	animation = enemy_rodney_knocked_back_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, back to thinking, hittable */
void enemy_rodney_knocked_back_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rodney.move_time = 0u;
		OBJECT(object)->flags |= 2u;
	}
}

/* destroyed: falls, lands as a wreck, gone */
void enemy_rodney_destroyed(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_rodney_destroyed_states_ADDRESS;
	run_object_state(object, states);
}

/* destroyed: thrown along its arc, facing its turtle, its side given up */
void enemy_rodney_destroyed_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_rodney_destroyed_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_rodney_destroyed_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	enemy_rodney_leave_side(object, 0u);
}

/* a frame of the fall; on landing a sound and its wreck */
void enemy_rodney_destroyed_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_rodney_destroyed_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	sound = 0xFu;
	queue_sound(sound);
	OBJECT(object)->state++;
	animation = enemy_rodney_destroyed_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, the wreck again */
void enemy_rodney_destroyed_land(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->state++;
	animation = enemy_rodney_destroyed_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the wreck's animation ends, it is gone, one enemy less after its turtle and on the screen */
void enemy_rodney_destroyed_end(uint32_t object, uint32_t player_slot)
{
	uint8_t t1 = 0;

	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		PLAYER_SLOT(player_slot)->enemies--;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* Shredder or one of his clones, by the parameter */
void enemy_shredder_start(uint32_t object, uint32_t player_slot)
{
	uint16_t difficulty_level = 0;

	player_slot = enemy_nearest_slots(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy_shredder.target = player_slot;
	OBJECT(object)->enemy_shredder.retarget_time = 0x10u;
	OBJECT(object)->enemy_shredder.move_time = 0u;
	OBJECT(object)->enemy_shredder.approach = 0u;
	OBJECT(object)->enemy_shredder.side = 0u;
	OBJECT(object)->enemy_shredder.hits_to_knock_back = 3u;
	difficulty_level = difficulty & 7u;
	OBJECT(object)->enemy_shredder.guard_wait = TABLE8(shredder_guard_waits, difficulty_level);
	OBJECT(object)->enemy_shredder.attack_wait = TABLE8(shredder_attack_waits, difficulty_level);
	OBJECT(object)->enemy_shredder.ray_wait = TABLE16(shredder_ray_waits, (uint16_t)((uint32_t)difficulty_level << 1));
	OBJECT(object)->depth = 4u;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine++;
	enemies_on_screen++;
	if ((int8_t)OBJECT(object)->parameter < 0)
	{
		OBJECT(object)->flags |= 0x80u;
		OBJECT(object)->enemy_shredder.move = 0xA8u;
		enemy_shredder_enter(object);
		return;
	}
	shredder_beaten = 0u;
	shredder_firing_ray = 0u;
	shredder_clones_made = 0u;
	OBJECT(object)->enemy_shredder.move = 0xECu;
}

/* his first move, without thinking */
void enemy_shredder_enter(uint32_t object)
{
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move)
	{
	case 20:
		enemy_shuffle(object);
		return;
	case 28:
		enemy_circle(object);
		return;
	case 36:
		enemy_charge(object);
		return;
	case 0:
		enemy_walk_to_target(object);
		return;
	case 4:
		enemy_walk_to_side(object);
		return;
	case 8:
		enemy_keep_distance(object);
		return;
	case 12:
		enemy_back_off(object);
		return;
	case 16:
		enemy_run_away(object);
		return;
	case 44:
		enemy_walk_across(object);
		return;
	case 48:
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		enemy_walk_past_sword(object);
		return;
	case 32:
		enemy_back_off_sword(object);
		return;
	case 40:
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		enemy_stalk_hammer(object);
		return;
	case 64:
		enemy_walk_away_hammer(object);
		return;
	case 72:
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		enemy_back_off_spear(object);
		return;
	case 112:
		enemy_step_aside_spear(object);
		return;
	case 80:
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		enemy_circle_knife(object);
		return;
	case 124:
		enemy_keep_distance_knife(object);
		return;
	case 120:
		enemy_back_off_knife(object);
		return;
	case 128:
		enemy_step_aside_knife(object);
		return;
	case 88:
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		enemy_back_off_gun(object);
		return;
	case 144:
		enemy_run_away_gun(object);
		return;
	case 96:
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		enemy_back_off_axe(object);
		return;
	case 136:
		enemy_run_away_axe(object);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		enemy_rocksteady_circle(object);
		return;
	case 208:
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		enemy_bebop_stand(object);
		return;
	case 216:
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		enemy_bebop_walk_level(object);
		return;
	case 220:
		enemy_bebop_close_in(object);
		return;
	case 156:
		enemy_mouser_chase(object);
		return;
	case 164:
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		enemy_mouser_back_off(object);
		return;
	case 172:
		enemy_rodney_rush(object);
		return;
	case 176:
		enemy_rodney_rush_across(object);
		return;
	case 180:
		enemy_rodney_back_off(object);
		return;
	case 236:
		enemy_shredder_appear(object);
		return;
	case 240:
		enemy_shredder_split(object);
		return;
	case 168:
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		enemy_shredder_guard(object);
		return;
	case 256:
		enemy_shredder_back_off(object);
		return;
	}
}

/* his thinking: clones, guards, blows by the distance and his ray */
void enemy_shredder_think(uint32_t object, uint32_t second_slot)
{
	bool L_052920 = false, L_052992 = false, L_052A68 = false;
	uint8_t enemies = 0, turtles = 0;
	uint16_t count = 0, target_dx = 0;
	uint32_t closest_slot = 0, distance = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, turtle = 0;
	uint8_t retarget_time = 0, sides = 0, sides_2 = 0, guard_wait = 0, attack_wait = 0;
	uint16_t t2 = 0, ray_wait = 0;
	uint32_t t1 = 0;

	slot = 0x60000u | OBJECT(object)->enemy_shredder.target;
	retarget_time = OBJECT(object)->enemy_shredder.retarget_time;
	OBJECT(object)->enemy_shredder.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_shredder.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			PLAYER_SLOT(slot)->enemies--;
			second_slot = enemy_nearest_slots(object, second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = object_loop_leftover;
				closest_slot = second_slot;
				object_loop_leftover = closest_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						object_loop_leftover = second_slot;
						second_slot = closest_slot;
					}
				}
			}
			if ((uint16_t)second_slot != OBJECT(object)->enemy_shredder.target)
			{
				object_loop_slot = slot;
				enemy_leave_side(object, slot);
			}
			OBJECT(object)->enemy_shredder.target = second_slot;
			slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	enemy_keep_on_screen(object);
	if ((OBJECT(object)->flags & 2u) != 0u)
	{
		L_052920 = (int8_t)OBJECT(object)->parameter < 0;
		if (!L_052920)
		{
			turtles = turtles_in_play;
			L_052920 = turtles < enemies_on_screen || shredder_clone_limits[turtles & 7u] < shredder_clones_made;
			if (!L_052920)
			{
				shredder_clones_made++;
				OBJECT(object)->enemy_shredder.move = 0xF0u;
				OBJECT(object)->state = 0u;
			}
		}
		if (L_052920)
		{
			if (shredder_beaten != 0u)
			{
				OBJECT(object)->flags |= 0x80u;
				OBJECT(object)->flags &= ~2u;
				OBJECT(object)->enemy_shredder.move = 0xDCu;
				OBJECT(object)->routine = 5u;
				OBJECT(object)->state = 0u;
				object_loop_slot = slot;
				return;
			}
			if (OBJECT(object)->enemy_shredder.approach != 0u || PLAYER_SLOT(slot)->sides >= 0xC0u)
			{
				if (OBJECT(object)->enemy_shredder.approach == 1u && (OBJECT(object)->enemy_shredder.target_dx >= 0x50u || OBJECT(object)->enemy_shredder.target_dy >= 0x50u))
					enemy_give_side_back(object, slot);
			}
			else if (OBJECT(object)->enemy_shredder.target_dx < 0x40u && OBJECT(object)->enemy_shredder.target_dy < 0x40u)
			{
				OBJECT(object)->enemy_shredder.approach = 1u;
				OBJECT(object)->enemy_shredder.move_time = 0u;
				do
				{
					if (L_052992 || (int8_t)(OBJECT(object)->enemy_shredder.target_direction - 0x40u) < 0)
					{
						for (;;)
						{
							L_052992 = false;
							sides = PLAYER_SLOT(slot)->sides;
							PLAYER_SLOT(slot)->sides = sides | 0x40u;
							if ((sides & 0x40u) != 0u)
							{
								sides_2 = PLAYER_SLOT(slot)->sides;
								PLAYER_SLOT(slot)->sides = sides_2 | 0x80u;
								if ((sides_2 & 0x80u) != 0u)
									continue;
								OBJECT(object)->enemy_shredder.side &= 0x7Fu;
								break;
							}
							OBJECT(object)->enemy_shredder.side |= 0x80u;
							break;
						}
					}
					else
					{
						sides_2 = PLAYER_SLOT(slot)->sides;
						PLAYER_SLOT(slot)->sides = sides_2 | 0x80u;
						L_052992 = (sides_2 & 0x80u) != 0u;
						if (!L_052992)
							OBJECT(object)->enemy_shredder.side &= 0x7Fu;
					}
				} while (L_052992);
			}
			if (OBJECT(object)->enemy_shredder.approach != 1u)
				L_052A68 = true;
			else
			{
				turtle = PLAYER_SLOT(slot)->turtle;
				if (OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u)
				{
					guard_wait = OBJECT(object)->enemy_shredder.guard_wait;
					OBJECT(object)->enemy_shredder.guard_wait = guard_wait - 1u;
					L_052A68 = guard_wait != 1u;
					if (!L_052A68)
					{
						OBJECT(object)->enemy_shredder.guard_wait = shredder_guard_waits[difficulty & 7u];
						L_052A68 = OBJECT(PLAYER_SLOT(slot)->turtle)->routine != 3u || OBJECT(object)->enemy_shredder.target_dy >= 6u || OBJECT(object)->enemy_shredder.target_dx >= 0x50u || (int8_t)OBJECT(object)->enemy_shredder.target_faces >= 0;
						if (!L_052A68)
						{
							enemy_give_side_back(object, slot);
							OBJECT(object)->enemy_shredder.approach = 2u;
							OBJECT(object)->enemy_shredder.move_time = 0u;
						}
					}
				}
				else
				{
					enemy_give_side_back(object, slot);
					OBJECT(object)->enemy_shredder.approach = 3u;
					OBJECT(object)->enemy_shredder.move_time = 0u;
				}
			}
			if (L_052A68)
			{
				if (OBJECT(PLAYER_SLOT(slot)->turtle)->routine != 4u)
				{
					attack_wait = OBJECT(object)->enemy_shredder.attack_wait;
					OBJECT(object)->enemy_shredder.attack_wait = attack_wait - 1u;
					if (attack_wait == 1u)
					{
						OBJECT(object)->enemy_shredder.attack_wait = shredder_attack_waits[difficulty & 7u];
						if (OBJECT(object)->enemy_shredder.target_dy <= 5u)
						{
							distance = shredder_distance_moves_ADDRESS;
							target_dx = OBJECT(object)->enemy_shredder.target_dx;
							count = 3u;
							do
							{
								if (target_dx < DISTANCE_MOVE(distance))
								{
									OBJECT(object)->enemy_shredder.move = DISTANCE_MOVE(distance + 0x8u);
									OBJECT(object)->routine = 3u;
									OBJECT(object)->state = 0u;
									OBJECT(object)->flags &= ~8u;
									object_loop_slot = slot;
									return;
								}
								t2 = count - 1u;
								count = t2;
								distance += 2u;
							} while (t2 != 0xFFFFu);
						}
					}
					if ((int8_t)OBJECT(object)->health_bars < 0 && shredder_firing_ray == 0u)
					{
						ray_wait = OBJECT(object)->enemy_shredder.ray_wait;
						OBJECT(object)->enemy_shredder.ray_wait = ray_wait - 1u;
						if (ray_wait == 1u)
						{
							OBJECT(object)->enemy_shredder.ray_wait = shredder_ray_waits[difficulty & 7u];
							OBJECT(object)->enemy_shredder.move = 0xB0u;
							OBJECT(object)->routine = 3u;
							OBJECT(object)->state = 0u;
							OBJECT(object)->flags &= ~8u;
							OBJECT(object)->enemy_shredder.side |= 1u;
							shredder_firing_ray = 0xFFu;
							object_loop_slot = slot;
							return;
						}
					}
				}
			}
			if (OBJECT(object)->enemy_shredder.move_time == 0u)
			{
				OBJECT(object)->enemy_shredder.move = shredder_approach_moves[OBJECT(object)->enemy_shredder.approach & 3u];
				OBJECT(object)->state = 0u;
			}
		}
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move)
	{
	case 20:
		object_loop_slot = slot;
		enemy_shuffle(object);
		return;
	case 28:
		object_loop_slot = slot;
		enemy_circle(object);
		return;
	case 36:
		object_loop_slot = slot;
		enemy_charge(object);
		return;
	case 0:
		object_loop_slot = slot;
		enemy_walk_to_target(object);
		return;
	case 4:
		object_loop_slot = slot;
		enemy_walk_to_side(object);
		return;
	case 8:
		object_loop_slot = slot;
		enemy_keep_distance(object);
		return;
	case 12:
		object_loop_slot = slot;
		enemy_back_off(object);
		return;
	case 16:
		object_loop_slot = slot;
		enemy_run_away(object);
		return;
	case 44:
		object_loop_slot = slot;
		enemy_walk_across(object);
		return;
	case 48:
		object_loop_slot = slot;
		enemy_walk_to_target_sword(object);
		return;
	case 52:
		object_loop_slot = slot;
		enemy_walk_to_side_sword(object);
		return;
	case 24:
		object_loop_slot = slot;
		enemy_walk_past_sword(object);
		return;
	case 32:
		object_loop_slot = slot;
		enemy_back_off_sword(object);
		return;
	case 40:
		object_loop_slot = slot;
		enemy_walk_to_line_sword(object);
		return;
	case 56:
		object_loop_slot = slot;
		enemy_walk_to_target_hammer(object);
		return;
	case 60:
		object_loop_slot = slot;
		enemy_walk_to_side_hammer(object);
		return;
	case 68:
		object_loop_slot = slot;
		enemy_stalk_hammer(object);
		return;
	case 64:
		object_loop_slot = slot;
		enemy_walk_away_hammer(object);
		return;
	case 72:
		object_loop_slot = slot;
		enemy_walk_to_target_spear(object);
		return;
	case 76:
		object_loop_slot = slot;
		enemy_walk_to_side_spear(object);
		return;
	case 104:
		object_loop_slot = slot;
		enemy_walk_off_line_spear(object);
		return;
	case 108:
		object_loop_slot = slot;
		enemy_back_off_spear(object);
		return;
	case 112:
		object_loop_slot = slot;
		enemy_step_aside_spear(object);
		return;
	case 80:
		object_loop_slot = slot;
		enemy_walk_to_target_knife(object);
		return;
	case 84:
		object_loop_slot = slot;
		enemy_walk_to_side_knife(object);
		return;
	case 116:
		object_loop_slot = slot;
		enemy_circle_knife(object);
		return;
	case 124:
		object_loop_slot = slot;
		enemy_keep_distance_knife(object);
		return;
	case 120:
		object_loop_slot = slot;
		enemy_back_off_knife(object);
		return;
	case 128:
		object_loop_slot = slot;
		enemy_step_aside_knife(object);
		return;
	case 88:
		object_loop_slot = slot;
		enemy_walk_to_target_gun(object);
		return;
	case 92:
		object_loop_slot = slot;
		enemy_walk_to_side_gun(object);
		return;
	case 152:
		object_loop_slot = slot;
		enemy_walk_to_range_gun(object);
		return;
	case 140:
		object_loop_slot = slot;
		enemy_back_off_gun(object);
		return;
	case 144:
		object_loop_slot = slot;
		enemy_run_away_gun(object);
		return;
	case 96:
		object_loop_slot = slot;
		enemy_walk_to_target_axe(object);
		return;
	case 100:
		object_loop_slot = slot;
		enemy_walk_to_side_axe(object);
		return;
	case 148:
		object_loop_slot = slot;
		enemy_walk_to_range_axe(object);
		return;
	case 132:
		object_loop_slot = slot;
		enemy_back_off_axe(object);
		return;
	case 136:
		object_loop_slot = slot;
		enemy_run_away_axe(object);
		return;
	case 196:
		object_loop_slot = slot;
		enemy_rocksteady_raise_fist(object);
		return;
	case 200:
		object_loop_slot = slot;
		enemy_rocksteady_laugh(object);
		return;
	case 192:
		object_loop_slot = slot;
		enemy_rocksteady_shake_fist(object);
		return;
	case 184:
		object_loop_slot = slot;
		enemy_rocksteady_draw_gun(object);
		return;
	case 188:
		object_loop_slot = slot;
		enemy_rocksteady_put_gun_away(object);
		return;
	case 204:
		object_loop_slot = slot;
		enemy_rocksteady_circle(object);
		return;
	case 208:
		object_loop_slot = slot;
		enemy_rocksteady_line_up(object);
		return;
	case 212:
		object_loop_slot = slot;
		enemy_rocksteady_close_in(object);
		return;
	case 232:
		object_loop_slot = slot;
		enemy_rocksteady_shoot_around(object);
		return;
	case 228:
		object_loop_slot = slot;
		enemy_bebop_stand(object);
		return;
	case 216:
		object_loop_slot = slot;
		enemy_bebop_walk_to_front(object);
		return;
	case 224:
		object_loop_slot = slot;
		enemy_bebop_walk_level(object);
		return;
	case 220:
		object_loop_slot = slot;
		enemy_bebop_close_in(object);
		return;
	case 156:
		object_loop_slot = slot;
		enemy_mouser_chase(object);
		return;
	case 164:
		object_loop_slot = slot;
		enemy_mouser_creep_to_side(object);
		return;
	case 160:
		object_loop_slot = slot;
		enemy_mouser_back_off(object);
		return;
	case 172:
		object_loop_slot = slot;
		enemy_rodney_rush(object);
		return;
	case 176:
		object_loop_slot = slot;
		enemy_rodney_rush_across(object);
		return;
	case 180:
		object_loop_slot = slot;
		enemy_rodney_back_off(object);
		return;
	case 236:
		object_loop_slot = slot;
		enemy_shredder_appear(object);
		return;
	case 240:
		object_loop_slot = slot;
		enemy_shredder_split(object);
		return;
	case 168:
		object_loop_slot = slot;
		enemy_shredder_clone_appear(object);
		return;
	case 244:
		object_loop_slot = slot;
		enemy_shredder_walk_to_side(object);
		return;
	case 248:
		object_loop_slot = slot;
		enemy_shredder_walk_to_target(object);
		return;
	case 252:
		object_loop_slot = slot;
		enemy_shredder_guard(object);
		return;
	case 256:
		object_loop_slot = slot;
		enemy_shredder_back_off(object);
		return;
	}
}

/* his attack, keeping his ray's time */
void enemy_shredder_attack(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, turtle = 0;
	uint16_t ray_wait = 0;
	uint32_t jump_target = 0;

	ray_wait = OBJECT(object)->enemy_shredder.ray_wait;
	OBJECT(object)->enemy_shredder.ray_wait = ray_wait - 1u;
	if (ray_wait == 1u)
		OBJECT(object)->enemy_shredder.ray_wait = 1u;
	slot = 0x60000u | OBJECT(object)->enemy_shredder.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4);
	enemy_keep_on_screen(object);
	jump_target = TABLE32(enemy_attacks, (uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move);
	switch (jump_target)
	{
	case enemy_jab_ADDRESS:
		object_loop_slot = slot;
		enemy_jab(object);
		return;
	case enemy_chop_ADDRESS:
		object_loop_slot = slot;
		enemy_chop(object);
		return;
	case enemy_swing_ADDRESS:
		object_loop_slot = slot;
		enemy_swing(object);
		return;
	case enemy_high_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_high_kick(object);
		return;
	case enemy_jump_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_jump_kick(object);
		return;
	case enemy_long_jump_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_long_jump_kick(object);
		return;
	case enemy_attack_none_after_long_jump_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_attack_none_after_long_jump_kick();
		return;
	case enemy_attack_none_before_grab_ADDRESS:
		object_loop_slot = slot;
		enemy_attack_none_before_grab();
		return;
	case enemy_grab_ADDRESS:
		object_loop_slot = slot;
		enemy_grab(object);
		return;
	case enemy_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_slash_sword(object);
		return;
	case enemy_low_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_low_slash_sword(object);
		return;
	case enemy_leaping_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_leaping_slash_sword(object);
		return;
	case enemy_jump_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_jump_slash_sword(object);
		return;
	case enemy_flying_slash_sword_ADDRESS:
		object_loop_slot = slot;
		enemy_flying_slash_sword(object);
		return;
	case enemy_smash_hammer_ADDRESS:
		object_loop_slot = slot;
		enemy_smash_hammer(object);
		return;
	case enemy_spear_thrust_ADDRESS:
		object_loop_slot = slot;
		enemy_spear_thrust(object);
		return;
	case enemy_spear_throw_ADDRESS:
		object_loop_slot = slot;
		enemy_spear_throw(object);
		return;
	case enemy_knife_stab_ADDRESS:
		object_loop_slot = slot;
		enemy_knife_stab(object);
		return;
	case enemy_high_kick_knife_ADDRESS:
		object_loop_slot = slot;
		enemy_high_kick_knife(object);
		return;
	case enemy_jump_kick_knife_ADDRESS:
		object_loop_slot = slot;
		enemy_jump_kick_knife(object);
		return;
	case enemy_throw_knife_ADDRESS:
		object_loop_slot = slot;
		enemy_throw_knife(object);
		return;
	case enemy_throw_axes_ADDRESS:
		object_loop_slot = slot;
		enemy_throw_axes(object);
		return;
	case enemy_high_kick_axe_ADDRESS:
		object_loop_slot = slot;
		enemy_high_kick_axe(object);
		return;
	case enemy_gun_swing_ADDRESS:
		object_loop_slot = slot;
		enemy_gun_swing(object);
		return;
	case enemy_shoot_gun_ADDRESS:
		object_loop_slot = slot;
		enemy_shoot_gun(object);
		return;
	case enemy_throw_shuriken_ADDRESS:
		object_loop_slot = slot;
		enemy_throw_shuriken(object);
		return;
	case enemy_boomerang_throw_ADDRESS:
		object_loop_slot = slot;
		enemy_boomerang_throw(object);
		return;
	case enemy_rocksteady_pincer_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_pincer_charge(object);
		return;
	case enemy_rocksteady_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_charge(object);
		return;
	case enemy_rocksteady_punch_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_punch(object);
		return;
	case enemy_rocksteady_gun_butt_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_gun_butt(object);
		return;
	case enemy_rocksteady_shoot_ADDRESS:
		object_loop_slot = slot;
		enemy_rocksteady_shoot(object);
		return;
	case enemy_bebop_pincer_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_pincer_charge(object);
		return;
	case enemy_bebop_punch_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_punch(object);
		return;
	case enemy_bebop_charge_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_charge(object);
		return;
	case enemy_bebop_shoot_ADDRESS:
		object_loop_slot = slot;
		enemy_bebop_shoot(object);
		return;
	case enemy_rodney_shoot_ADDRESS:
		object_loop_slot = slot;
		enemy_rodney_shoot(object);
		return;
	case enemy_rodney_whip_ADDRESS:
		object_loop_slot = slot;
		enemy_rodney_whip(object);
		return;
	case enemy_shredder_kick_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_kick(object);
		return;
	case enemy_shredder_thrust_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_thrust(object);
		return;
	case enemy_shredder_slash_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_slash(object);
		return;
	case enemy_shredder_sweep_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_sweep(object);
		return;
	case enemy_shredder_ray_ADDRESS:
		object_loop_slot = slot;
		enemy_shredder_ray(object);
		return;
	}
}

/* hit: shrugged off in the air, else his reaction by his health */
void enemy_shredder_hit(uint32_t object)
{
	uint8_t sound = 0;
	uint8_t side = 0;

	sound = 0x92u;
	queue_sound(sound);
	side = OBJECT(object)->enemy_shredder.side;
	OBJECT(object)->enemy_shredder.side = side & ~1u;
	if ((side & 1u) != 0u)
	{
		shredder_firing_ray = 0u;
		if (OBJECT(object)->hit_state < 4u)
			OBJECT(object)->enemy_shredder.ray_wait = 1u;
	}
	if ((OBJECT(object)->health_bars | OBJECT(object)->health) == 0u)
	{
		if ((int8_t)OBJECT(object)->parameter >= 0)
			OBJECT(object)->enemy_shredder.move = 0xD8u;
		else
			OBJECT(object)->enemy_shredder.move = 0xDCu;
		OBJECT(object)->routine++;
		enemy_shredder_react(object);
		return;
	}
	if (OBJECT(object)->health == 0u)
	{
		OBJECT(object)->enemy_shredder.move = 0xCCu;
		OBJECT(object)->routine++;
		enemy_shredder_react(object);
		return;
	}
	if (((((uint32_t)OBJECT(object)->floor_height << 8u) | OBJECT(object)->frame_time) & ~0xFFu) != OBJECT(object)->height)
	{
		OBJECT(object)->colour = 0xBu;
		OBJECT(object)->routine = 2u;
		return;
	}
	if ((int8_t)(((uint32_t)OBJECT(object)->flags << 1) ^ OBJECT(object)->hit) >= 0)
	{
		OBJECT(object)->enemy_shredder.move = 0xD0u;
		OBJECT(object)->routine++;
		enemy_shredder_react(object);
		return;
	}
	OBJECT(object)->enemy_shredder.move = 0xD4u;
	OBJECT(object)->routine++;
	enemy_shredder_react(object);
}

/* his reaction to a hit */
void enemy_shredder_react(uint32_t object)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0;

	object_loop_slot = 0x60000u | OBJECT(object)->enemy_shredder.target;
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored, &ignored_2, &ignored_3, &ignored_4);
	enemy_keep_on_screen(object);
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move)
	{
	case 228:
		enemy_crash_into_scenery(object);
		return;
	case 232:
		enemy_crash_into_scenery_low(object);
		return;
	case 0:
		enemy_flinch_back(object);
		return;
	case 4:
		enemy_flinch_bent(object);
		return;
	case 8:
		enemy_flinch_reel(object);
		return;
	case 12:
		enemy_flinch_hunched(object);
		return;
	case 36:
		enemy_killed_back(object);
		return;
	case 32:
		enemy_killed_forward(object);
		return;
	case 16:
		enemy_knocked_forward(object);
		return;
	case 128:
		enemy_thrown_by_donatello(object);
		return;
	case 132:
		enemy_let_go_by_donatello(object);
		return;
	case 136:
		enemy_thrown_by_michelangelo(object);
		return;
	case 140:
		enemy_let_go_by_michelangelo(object);
		return;
	case 188:
		enemy_thrown_by_raphael(object);
		return;
	case 192:
		enemy_let_go_by_raphael(object);
		return;
	case 196:
		enemy_thrown_by_leonardo(object);
		return;
	case 200:
		enemy_let_go_by_leonardo(object);
		return;
	case 20:
		enemy_knocked_back(object);
		return;
	case 24:
		enemy_die(object);
		return;
	case 28:
		enemy_die_on_back(object);
		return;
	case 40:
		enemy_flinch_back_sword(object);
		return;
	case 44:
		enemy_flinch_bent_sword(object);
		return;
	case 48:
		enemy_flinch_reel_sword(object);
		return;
	case 52:
		enemy_flinch_hunched_sword(object);
		return;
	case 56:
		enemy_flinch_back_hammer(object);
		return;
	case 60:
		enemy_flinch_bent_hammer(object);
		return;
	case 68:
		enemy_flinch_hunched_hammer(object);
		return;
	case 72:
		enemy_flinch_back_spear(object);
		return;
	case 76:
		enemy_flinch_bent_spear(object);
		return;
	case 80:
		enemy_flinch_reel_spear(object);
		return;
	case 84:
		enemy_flinch_hunched_spear(object);
		return;
	case 88:
		enemy_flinch_back_knife(object);
		return;
	case 92:
		enemy_flinch_bent_knife(object);
		return;
	case 96:
		enemy_flinch_reel_knife(object);
		return;
	case 100:
		enemy_flinch_hunched_knife(object);
		return;
	case 120:
		enemy_flinch_back_gun(object);
		return;
	case 124:
		enemy_flinch_bent_gun(object);
		return;
	case 104:
		enemy_flinch_back_axe(object);
		return;
	case 108:
		enemy_flinch_bent_axe(object);
		return;
	case 112:
		enemy_flinch_reel_axe(object);
		return;
	case 164:
		enemy_rocksteady_flinch(object);
		return;
	case 168:
		enemy_rocksteady_reel(object);
		return;
	case 160:
		enemy_rocksteady_flinch_from_behind(object);
		return;
	case 152:
		enemy_rocksteady_knocked_down(object);
		return;
	case 156:
		enemy_rocksteady_defeated(object);
		return;
	case 172:
		enemy_bebop_flinch(object);
		return;
	case 176:
		enemy_bebop_flinch_from_behind(object);
		return;
	case 180:
		enemy_bebop_knocked_down(object);
		return;
	case 184:
		enemy_bebop_defeated(object);
		return;
	case 148:
		enemy_mouser_thrown_off(object);
		return;
	case 224:
		enemy_mouser_break_down(object);
		return;
	case 144:
		enemy_mouser_knocked_flying(object);
		return;
	case 64:
		enemy_rodney_knocked_back(object);
		return;
	case 116:
		enemy_rodney_destroyed(object);
		return;
	case 204:
		enemy_shredder_lose_bar(object);
		return;
	case 208:
		enemy_shredder_flinch(object);
		return;
	case 212:
		enemy_shredder_flinch_from_behind(object);
		return;
	case 216:
		enemy_shredder_defeated(object);
		return;
	case 220:
		enemy_shredder_vanish(object);
		return;
	}
}

/* comes out of the screen when he is the last enemy, with his music */
void enemy_shredder_appear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_appear_states_ADDRESS;
	run_object_state(object, states);
}

/* when he is the only enemy left, he is shown with a sound in his first pose */
void enemy_shredder_appear_wait(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (enemies_on_screen != 1u)
		return;
	OBJECT(object)->state++;
	OBJECT(object)->flags |= 0x80u;
	sound = 0x16u;
	queue_sound(sound);
	animation = enemy_shredder_appear_wait_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, steps out with his music and his speech */
void enemy_shredder_appear_step_out(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = -0x100;
	OBJECT(object)->enemy_shredder.timer = 0x20u;
	OBJECT(object)->state++;
	sound = 0x6Au;
	music = 0x6Au;
	queue_sound(sound);
	sound = 0xA7u;
	queue_sound(sound);
	animation = enemy_shredder_appear_step_out_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks out for 32 frames, then hittable, his health, thinking */
void enemy_shredder_appear_walk_in(uint32_t object)
{
	uint32_t settings = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags |= 2u;
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		settings = shredder_health_ADDRESS;
		enemy_set_health(object, settings);
	}
	move_object(object);
}

/* makes a clone of himself, blinking */
void enemy_shredder_split(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_split_states_ADDRESS;
	run_object_state(object, states);
}

/* splits: not hittable, a clone of him with his bars and a sound, and he moves aside blinking 32 frames */
void enemy_shredder_split_start(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;

	OBJECT(object)->flags &= ~2u;
	animation = enemy_shredder_stand_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->enemy_shredder.timer = 0x20u;
	object_loop_leftover = shredder_clone_record_ADDRESS;
	record = shredder_clone_record_ADDRESS;
	start_object_from_record(object, record, &object_2, &zero);
	OBJECT(object_2)->health_bars = OBJECT(object)->health_bars;
	sound = 0x3Du;
	queue_sound(sound);
}

/* blinks moving aside; at its end, shown, hittable, and a new move */
void enemy_shredder_split_blink(uint32_t object)
{
	uint8_t timer = 0;

	OBJECT(object)->flags ^= 0x80u;
	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags |= 0x82u;
		OBJECT(object)->enemy_shredder.move_time = 0u;
	}
	move_object_blocked(object);
}

/* a clone comes in blinking */
void enemy_shredder_clone_appear(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_clone_appear_states_ADDRESS;
	run_object_state(object, states);
}

/* a clone's way in: shown, moving back for 32 frames */
void enemy_shredder_clone_appear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	animation = enemy_shredder_stand_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = -0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->enemy_shredder.move_time = 0x20u;
}

/* the clone blinks moving back; at its end, shown, hittable, its health, thinking */
void enemy_shredder_clone_appear_blink(uint32_t object)
{
	uint32_t settings = 0;
	uint8_t move_time = 0;

	OBJECT(object)->flags ^= 0x80u;
	move_time = OBJECT(object)->enemy_shredder.move_time;
	OBJECT(object)->enemy_shredder.move_time = move_time - 1u;
	if (move_time != 1u)
	{
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->flags |= 0x82u;
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	settings = shredder_clone_health_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		settings = shredder_clone_bar_health_ADDRESS;
	enemy_set_health(object, settings);
	move_object_blocked(object);
}

/* walks to his side of his turtle */
void enemy_shredder_walk_to_side(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_walk_to_side_states_ADDRESS;
	run_object_state(object, states);
}

/* 240 frames of walking to his turtle's side, facing it */
void enemy_shredder_walk_to_side_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_shredder.move_time = 0xF0u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk: across to 0x40 by his turtle or up or down to its row; level with it, the next
   state */
void enemy_shredder_walk_to_side_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_shredder.target_dx < 0x40u || (int8_t)(target_direction - 0x40u - OBJECT(object)->enemy_shredder.side) < 0)
	{
		OBJECT(object)->x_speed = 0x180;
		OBJECT(object)->y_speed = 0;
		if ((int8_t)OBJECT(object)->enemy_shredder.side < 0)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	else
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0x180;
		if ((int8_t)OBJECT(object)->enemy_shredder.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	if (OBJECT(object)->enemy_shredder.target_dy < 3u)
		OBJECT(object)->state++;
	move_object_blocked(object);
}

/* now and then, by the frame count, a crouch, else straight on to keeping level */
void enemy_shredder_walk_to_side_crouch(uint32_t object)
{
	uint32_t animation = 0;

	if ((frame_count & 0x700u) != 0u)
	{
		OBJECT(object)->state += 2u;
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_shredder_crouch_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the crouch ends, faces his turtle again */
void enemy_shredder_walk_to_side_turn(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* level with his turtle: walks across until 0x10 away; off its row again, back to walking to its side */
void enemy_shredder_walk_to_side_hold(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_shredder.target_dx < 0x10u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0;
		if (OBJECT(object)->enemy_shredder.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->x_speed = 0x180;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(target_direction - 0x40u) < 0)
	{
		if (OBJECT(object)->enemy_shredder.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return;
	}
	OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	if (OBJECT(object)->enemy_shredder.target_dy >= 3u)
		OBJECT(object)->state -= 3u;
	move_object_blocked(object);
}

/* walks to his turtle */
void enemy_shredder_walk_to_target(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_walk_to_target_states_ADDRESS;
	run_object_state(object, states);
}

/* the walk to his turtle, facing it, aiming at once */
void enemy_shredder_walk_to_target_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_shredder.move_time = 1u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of the walk to his turtle, aiming every 8 frames, standing when close */
void enemy_shredder_walk_to_target_step(uint32_t object, uint32_t player_slot)
{
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0, y_speed = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_shredder.move_time;
	OBJECT(object)->enemy_shredder.move_time = move_time - 1u;
	if (move_time != 1u)
	{
		if (OBJECT(object)->enemy_shredder.target_dx >= 0x3Cu || OBJECT(object)->enemy_shredder.target_dy >= 0x20u)
		{
			move_object_blocked(object);
			return;
		}
		return;
	}
	turtle = PLAYER_SLOT(player_slot)->turtle;
	direction = direction_to_object(0u, object, turtle);
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	enemy_turn_to_object((uint8_t)direction, object, turtle);
	speed = 0x180u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->enemy_shredder.move_time = 8u;
	if (OBJECT(object)->enemy_shredder.target_dx >= 0x3Cu || OBJECT(object)->enemy_shredder.target_dy >= 0x20u)
	{
		move_object_blocked(object);
		return;
	}
}

/* guards against his turtle's attack, then strikes back */
void enemy_shredder_guard(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_guard_states_ADDRESS;
	run_object_state(object, states);
}

/* guards against his turtle's attack, not hittable, for 30 frames */
void enemy_shredder_guard_start(uint32_t object)
{
	uint32_t animation = 0;

	animation = enemy_shredder_guard_start_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_guard_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->flags &= ~0xEu;
	OBJECT(object)->enemy_shredder.move_time = 0x1Eu;
	OBJECT(object)->state++;
}

/* after the guard, a counter blow picked by how far his turtle is */
void enemy_shredder_guard_counter(uint32_t object)
{
	uint16_t count = 0, target_dx = 0;
	uint32_t distance = 0;
	uint8_t move_time = 0;
	uint16_t t1 = 0;

	move_time = OBJECT(object)->enemy_shredder.move_time;
	OBJECT(object)->enemy_shredder.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->enemy_shredder.approach = 0u;
	distance = shredder_distance_moves_ADDRESS;
	target_dx = OBJECT(object)->enemy_shredder.target_dx;
	count = 2u;
	do
	{
		if (target_dx < DISTANCE_MOVE(distance))
			break;
		t1 = count - 1u;
		count = t1;
		distance += 2u;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->enemy_shredder.move = DISTANCE_MOVE(distance + 0x8u);
	OBJECT(object)->routine = 3u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->flags &= ~8u;
}

/* walks away and waits while his turtle is down */
void enemy_shredder_back_off(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_back_off_states_ADDRESS;
	run_object_state(object, states);
}

/* 40 frames of walking backwards away from his turtle, still facing it */
void enemy_shredder_back_off_start(uint32_t object, uint32_t player_slot)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t turtle = 0, x_speed = 0, y_speed = 0;

	OBJECT(object)->enemy_shredder.move_time = 0x40u;
	OBJECT(object)->enemy_shredder.timer = 0x28u;
	direction = OBJECT(object)->enemy_shredder.target_direction;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, &y_speed);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while his turtle is down; when the time runs out, his standing pose */
void enemy_shredder_back_off_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t timer = 0;

	object_loop_leftover = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		object_loop_leftover = shredder_walk_hurt_animations_ADDRESS;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_turn_to_object(target_direction, object, turtle);
	move_object_blocked(object);
	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer != 1u)
	{
		enemy_shredder_back_off_wait(object, player_slot);
		return;
	}
	OBJECT(object)->state++;
	animation = enemy_shredder_stand_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* waits while his turtle is down; when it is up again, a new move */
void enemy_shredder_back_off_wait(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		OBJECT(object)->enemy_shredder.approach = 0u;
		OBJECT(object)->enemy_shredder.move_time = 0u;
	}
}

/* a bar gone: a piece of him flies off, his health back */
void enemy_shredder_lose_bar(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_lose_bar_states_ADDRESS;
	run_object_state(object, states);
}

/* a health bar lost: he flashes in its pose */
void enemy_shredder_lose_bar_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->colour = 0xBu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_shredder.timer = 0xCu;
	animation = enemy_shredder_lose_bar_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* flashing, a piece of him flies off after 12 frames; at its end his health again, thinking, hittable */
void enemy_shredder_lose_bar_end(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, record = 0, settings = 0;
	uint8_t timer = 0;

	enemy_shredder_flash(object);
	if (OBJECT(object)->frame_time != 0u)
	{
		timer = OBJECT(object)->enemy_shredder.timer;
		OBJECT(object)->enemy_shredder.timer = timer - 1u;
		if (timer != 1u)
			return;
		object_loop_leftover = shredder_piece_shot_record_ADDRESS;
		record = shredder_piece_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		return;
	}
	OBJECT(object)->colour = 0xFFu;
	settings = shredder_bar_health_ADDRESS;
	if ((int8_t)OBJECT(object)->parameter < 0)
		settings = shredder_clone_bar_health_ADDRESS;
	enemy_set_health(object, settings);
	enemy_back_to_thinking_hittable(object);
}

/* his colour flashing while hit */
void enemy_shredder_flash(uint32_t object)
{
	uint8_t colour = 0;

	colour = 0xBu;
	if ((frame_count & 2u) != 0u)
		colour = 0xFFu;
	OBJECT(object)->colour = colour;
}

/* hit from the front, knocked back every third hit */
void enemy_shredder_flinch(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_flinch_states_ADDRESS;
	run_object_state(object, states);
}

/* hit from the front: he flashes, and every third hit knocks him back for 16 frames */
void enemy_shredder_flinch_start(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t hits_to_knock_back = 0;

	OBJECT(object)->colour = 0xBu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_shredder.timer = 0u;
	hits_to_knock_back = OBJECT(object)->enemy_shredder.hits_to_knock_back;
	OBJECT(object)->enemy_shredder.hits_to_knock_back = hits_to_knock_back - 1u;
	if (hits_to_knock_back == 1u)
	{
		OBJECT(object)->enemy_shredder.hits_to_knock_back = 3u;
		OBJECT(object)->enemy_shredder.timer = 0x10u;
		OBJECT(object)->x_speed = 0x200;
		OBJECT(object)->y_speed = 0;
		if ((OBJECT(object)->flags & 0x40u) != 0u)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	animation = enemy_shredder_recoil_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_flinch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* flashes, pushed while the knock lasts; at its end, thinking, hittable */
void enemy_shredder_flinch_end(uint32_t object)
{
	enemy_shredder_flash(object);
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->colour = 0xFFu;
		enemy_back_to_thinking_hittable(object);
		return;
	}
	if (OBJECT(object)->enemy_shredder.timer == 0u)
		return;
	OBJECT(object)->enemy_shredder.timer--;
	move_object_blocked(object);
}

/* hit from behind, knocked back every third hit */
void enemy_shredder_flinch_from_behind(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_flinch_from_behind_states_ADDRESS;
	run_object_state(object, states);
}

/* hit from behind: he flashes, and every third hit knocks him forward for 16 frames */
void enemy_shredder_flinch_from_behind_start(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t hits_to_knock_back = 0;

	OBJECT(object)->colour = 0xBu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_shredder.timer = 0u;
	hits_to_knock_back = OBJECT(object)->enemy_shredder.hits_to_knock_back;
	OBJECT(object)->enemy_shredder.hits_to_knock_back = hits_to_knock_back - 1u;
	if (hits_to_knock_back == 1u)
	{
		OBJECT(object)->enemy_shredder.hits_to_knock_back = 3u;
		OBJECT(object)->enemy_shredder.timer = 0x10u;
		OBJECT(object)->x_speed = 0x200;
		OBJECT(object)->y_speed = 0;
		if ((OBJECT(object)->flags & 0x40u) == 0u)
			OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	animation = enemy_shredder_flinch_from_behind_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_flinch_from_behind_2_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* beaten: falls, the music stops, his clones vanish */
void enemy_shredder_defeated(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_defeated_states_ADDRESS;
	run_object_state(object, states);
}

/* beaten: thrown along his arc, flashing, facing his turtle */
void enemy_shredder_defeated_start(uint32_t object)
{
	uint32_t animation = 0, arc = 0, target = 0;

	OBJECT(object)->colour = 0xBu;
	target = object_loop_slot;
	enemy_boss_face_target(object, target);
	arc = enemy_shredder_defeated_arc_ADDRESS;
	enemy_start_arc(object, arc);
	animation = enemy_shredder_defeated_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
}

/* a frame of the fall; on landing he slides */
void enemy_shredder_defeated_fall(uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->colour = 0xFFu;
	move_object_with_height_blocked(object);
	arc = enemy_shredder_defeated_arc_ADDRESS;
	enemy_arc_step(object, arc);
	zero = enemy_landed(object);
	if (zero == 0u)
		return;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_shredder_defeated_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* slides to a stop, then the music stops, a sound, his clones told to vanish */
void enemy_shredder_defeated_land(uint32_t object)
{
	uint8_t sound = 0;
	uint32_t arc = 0;

	if ((uint16_t)OBJECT(object)->x_speed != 0u)
	{
		move_object_blocked(object);
		arc = enemy_shredder_defeated_arc_ADDRESS;
		enemy_arc_step(object, arc);
	}
	if (OBJECT(object)->frame_time == 0u)
	{
		sound = 0u;
		music = 0u;
		queue_sound(sound);
		sound = 0x81u;
		queue_sound(sound);
		OBJECT(object)->enemy_shredder.timer = 0x78u;
		OBJECT(object)->state++;
		shredder_beaten = 0xFFu;
	}
}

/* lies 120 frames, then one enemy less, and he stays */
void enemy_shredder_defeated_wait(uint32_t object)
{
	uint8_t timer = 0, t1 = 0;

	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->state++;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
		{
			enemies_on_screen = 0u;
			enemy_shredder_defeated_stay();
			return;
		}
	}
	enemy_shredder_defeated_stay();
}

/* stays where he lies, doing nothing more */
void enemy_shredder_defeated_stay(void)
{
}

/* a clone beaten or told to vanish blinks and is gone */
void enemy_shredder_vanish(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_vanish_states_ADDRESS;
	run_object_state(object, states);
}

/* a clone beaten or told to vanish: gives up its side, in its flinch pose */
void enemy_shredder_vanish_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->colour = 0xAu;
	enemy_leave_side(object, 0u);
	OBJECT(object)->state++;
	animation = enemy_shredder_recoil_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_flinch_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* when the pose ends, 120 frames of blinking */
void enemy_shredder_vanish_wait(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
	{
		OBJECT(object)->enemy_shredder.timer = 0x78u;
		OBJECT(object)->state++;
	}
}

/* blinks, then the clone is gone, one enemy less on the screen */
void enemy_shredder_vanish_blink(uint32_t object)
{
	uint8_t timer = 0, t1 = 0;

	if ((OBJECT(object)->enemy_shredder.timer & 0x10u) == 0u)
		OBJECT(object)->flags ^= 0x80u;
	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer == 1u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t1 = enemies_on_screen - 1u;
		enemies_on_screen = t1;
		if ((int8_t)t1 < 0)
			enemies_on_screen = 0u;
	}
}

/* kicks */
void enemy_shredder_kick(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_kick_states_ADDRESS;
	run_object_state(object, states);
}

/* the kick's animation and damage, facing his turtle */
void enemy_shredder_kick_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_shredder_kick_start_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x301u;
	attack = 2u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* the blow over, back to thinking */
void enemy_shredder_blow_end(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->flags |= 2u;
	enemy_back_to_thinking(object);
}

/* thrusts the sword */
void enemy_shredder_thrust(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_thrust_states_ADDRESS;
	run_object_state(object, states);
}

/* the thrust's animation and damage, facing his turtle */
void enemy_shredder_thrust_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_shredder_thrust_start_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_thrust_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x301u;
	attack = 2u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* slashes with the sword, knocking down */
void enemy_shredder_slash(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_slash_states_ADDRESS;
	run_object_state(object, states);
}

/* the slash that knocks down: its animation and damage, facing his turtle */
void enemy_shredder_slash_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_shredder_slash_start_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_slash_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* sweeps low with the sword */
void enemy_shredder_sweep(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_sweep_states_ADDRESS;
	run_object_state(object, states);
}

/* the low sweep's animation and damage, facing his turtle */
void enemy_shredder_sweep_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = enemy_shredder_sweep_start_animation_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_sweep_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x203u;
	attack = 7u;
	enemy_face_target(attack, object);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->state++;
}

/* lines up with his turtle and fires his ray */
void enemy_shredder_ray(uint32_t object)
{
	uint32_t states = 0;

	states = enemy_shredder_ray_states_ADDRESS;
	run_object_state(object, states);
}

/* the ray: up to 240 frames of lining up with his turtle, facing it */
void enemy_shredder_ray_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_shredder.timer = 0xF0u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	enemy_face_object(target_direction, object, turtle);
}

/* a frame of lining up: away across while nearer than 0x50, else to his turtle's row; level or out of
   time, he charges */
void enemy_shredder_ray_line_up(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0, wall_mask = 0;
	uint32_t animation = 0, turtle = 0;
	uint8_t timer = 0;

	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	object_loop_leftover = shredder_walk_animations_ADDRESS;
	enemy_turn_to_object(target_direction, object, turtle);
	if (OBJECT(object)->enemy_shredder.target_dx >= 0x50u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0x280;
		if ((int8_t)OBJECT(object)->enemy_shredder.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
		move_object_blocked(object);
		timer = OBJECT(object)->enemy_shredder.timer;
		OBJECT(object)->enemy_shredder.timer = timer - 1u;
		if (timer == 1u || OBJECT(object)->enemy_shredder.target_dy < 3u)
		{
			OBJECT(object)->state++;
			animation = enemy_shredder_ray_line_up_animation_ADDRESS;
			set_animation(0u, object, animation);
			return;
		}
		return;
	}
	OBJECT(object)->y_speed = 0;
	wall_mask = 8u;
	OBJECT(object)->x_speed = 0x280;
	if ((int8_t)(OBJECT(object)->enemy_shredder.target_direction - 0x40u) < 0)
	{
		wall_mask = 0x10u;
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	}
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0x280;
		if ((int8_t)OBJECT(object)->enemy_shredder.target_direction >= 0)
			OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	}
	move_object_blocked(object);
	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer == 1u || OBJECT(object)->enemy_shredder.target_dy < 3u)
	{
		OBJECT(object)->state++;
		animation = enemy_shredder_ray_line_up_animation_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
}

/* when the pose ends, charges the ray for 12 frames */
void enemy_shredder_ray_charge(uint32_t object)
{
	uint32_t animation = 0;

	if (OBJECT(object)->frame_time != 0u)
		return;
	OBJECT(object)->enemy_shredder.timer = 0xCu;
	OBJECT(object)->state++;
	animation = enemy_shredder_ray_charge_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after 12 frames, faces his turtle and fires the ray with its sound, for 60 frames */
void enemy_shredder_ray_fire(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ray = 0, record = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer != 1u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_shredder.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	object_loop_leftover = shredder_ray_shot_record_ADDRESS;
	record = shredder_ray_shot_record_ADDRESS;
	start_shot(object, record, &ray, &zero);
	OBJECT(ray)->shredder_ray.shredder = object;
	sound = 0x1Cu;
	queue_sound(sound);
	OBJECT(object)->enemy_shredder.timer = 0x3Cu;
	OBJECT(object)->state++;
	animation = enemy_shredder_ray_fire_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* after 60 frames the ray is over and he goes back to thinking */
void enemy_shredder_ray_end(uint32_t object)
{
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer == 1u)
	{
		shredder_firing_ray = 0u;
		OBJECT(object)->enemy_shredder.side &= ~1u;
		enemy_back_to_thinking(object);
	}
}
