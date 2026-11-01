/**
 *    Name:        objects/states.c
 *    Description: the objects' state dispatcher and the objects' handlers (provisional: by subject later)
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    01-11-2026
 *    Author:      Sebastian Toyos
 *
 *    Teenage Mutant Ninja Turtles (arcade, Konami 1989): the game's 68000 program decompiled into C by
 *    tools/decompile.py, with the names of symbols.txt.
 */
#include "game.h"

/* the object's current state run from the table of state handlers it is given */
void run_object_state(uint32_t object, uint32_t slot, uint32_t states, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t state_offset = 0;
	uint32_t handler = 0;

	state_offset = (uint16_t)((uint32_t)OBJECT(object)->state << 2);
	handler = mem_read32(states + state_offset);
	switch (handler)
	{
	case 0x048F62u:
		enemy_hop_crouch(object);
		return;
	case 0x048F72u:
		enemy_hop_down(object);
		return;
	case 0x048FBAu:
		*p_d7 = enemy_hop_fall(*p_d7, object);
		return;
	case 0x048FFAu:
		enemy_hop_land(object);
		return;
	case 0x04901Au:
		enemy_hop_right(object);
		return;
	case 0x049078u:
		enemy_hop_left(object);
		return;
	case 0x0490D6u:
		enemy_hop_up_crouch(object);
		return;
	case 0x0490E6u:
		enemy_hop_up(object);
		return;
	case 0x04911Cu:
		*p_d7 = enemy_hop_up_fall(*p_d7, object);
		return;
	case 0x04916Cu:
		enemy_hop_up_right(object);
		return;
	case 0x0491A8u:
		*p_d7 = enemy_hop_up_fall_blocked(*p_d7, object);
		return;
	case 0x0491F8u:
		enemy_hop_up_left(object);
		return;
	case 0x049260u:
		enemy_shuffle_start(object);
		return;
	case 0x049276u:
		enemy_shuffle_step(object);
		return;
	case 0x0492A8u:
		enemy_shuffle_turn(object);
		return;
	case 0x0492DEu:
		enemy_shuffle_end(object);
		return;
	case 0x049300u:
		enemy_retreat_start(object);
		return;
	case 0x04933Au:
		enemy_retreat_step(object);
		return;
	case 0x049378u:
		enemy_circle_start(object, p_d7, p_a5);
		return;
	case 0x0493CAu:
		enemy_circle_step(object, p_d7, p_a5);
		return;
	case 0x049420u:
		*p_d7 = enemy_charge_start(object);
		return;
	case 0x049468u:
		enemy_charge_run(object);
		return;
	case 0x04949Eu:
		enemy_charge_brake(object);
		return;
	case 0x0494DAu:
		enemy_crash_into_scenery_start(object);
		return;
	case 0x049534u:
		enemy_crash_into_scenery_end(object);
		return;
	case 0x049570u:
		enemy_crash_into_scenery_low_start(object);
		return;
	case 0x0495DEu:
		*p_a5 = enemy_walk_to_target_start(object, slot);
		return;
	case 0x049604u:
		enemy_walk_to_target_step(object, slot, p_d7, p_a5);
		return;
	case 0x04966Eu:
		*p_a5 = enemy_walk_to_side_start(object, slot);
		return;
	case 0x049694u:
		*p_a5 = enemy_walk_to_side_step(object, slot);
		return;
	case 0x0496FAu:
		enemy_walk_to_side_crouch(object);
		return;
	case 0x04971Eu:
		*p_a5 = enemy_walk_to_side_turn(object, slot, *p_a5);
		return;
	case 0x04973Eu:
		*p_a5 = enemy_walk_to_side_hold(object, slot);
		return;
	case 0x0497A6u:
		enemy_keep_distance_start(object);
		return;
	case 0x0497C2u:
		enemy_keep_distance_watch(object, slot, p_d7, p_a5);
		return;
	case 0x04984Cu:
		*p_a5 = enemy_keep_distance_back_off(object, slot);
		return;
	case 0x049868u:
		*p_a5 = enemy_keep_distance_close_in(object, slot);
		return;
	case 0x0498B2u:
		enemy_back_off_start(object, slot, p_d7, p_a5);
		return;
	case 0x0498EEu:
		*p_a5 = enemy_back_off_walk(object, slot);
		return;
	case 0x04991Eu:
		enemy_back_off_wait(object, slot);
		return;
	case 0x049948u:
		enemy_run_away_start(object, slot, p_d7, p_a5);
		return;
	case 0x049980u:
		*p_a5 = enemy_run_away_step(object, slot);
		return;
	case 0x0499B8u:
		*p_a5 = enemy_walk_across_start(object, slot);
		return;
	case 0x0499EAu:
		*p_a5 = enemy_walk_across_step(object, slot);
		return;
	case 0x049A1Au:
		enemy_jab_start(object);
		return;
	case 0x049A42u:
		enemy_attack_end(object);
		return;
	case 0x049A66u:
		enemy_chop_start(object);
		return;
	case 0x049AA2u:
		enemy_swing_start(object);
		return;
	case 0x049ADEu:
		enemy_high_kick_start(object);
		return;
	case 0x049B2Eu:
		enemy_jump_kick_crouch(object);
		return;
	case 0x049B56u:
		*p_d7 = enemy_jump_kick_leap(*p_d7, object);
		return;
	case 0x049B82u:
		*p_d7 = enemy_jump_kick_fly(*p_d7, object);
		return;
	case 0x049BE4u:
		enemy_long_jump_kick_crouch(object);
		return;
	case 0x049C0Cu:
		*p_d7 = enemy_long_jump_kick_leap(*p_d7, object);
		return;
	case 0x049C38u:
		*p_d7 = enemy_long_jump_kick_fly(*p_d7, object);
		return;
	case 0x049C92u:
		enemy_grab_start(object, slot, p_d7, p_a5);
		return;
	case 0x049D06u:
		enemy_grab_run(object, slot, p_d7, p_a5);
		return;
	case 0x049D7Eu:
		enemy_grab_hold(object, slot);
		return;
	case 0x049DD0u:
		enemy_grab_end(object);
		return;
	case 0x049DF0u:
		enemy_flinch_back_start(object);
		return;
	case 0x049E00u:
		enemy_flinch_end(object);
		return;
	case 0x049E24u:
		enemy_flinch_bent_start(object);
		return;
	case 0x049E48u:
		enemy_flinch_reel_start(object);
		return;
	case 0x049E6Cu:
		enemy_flinch_hunched_start(object);
		return;
	case 0x049EACu:
		*p_d7 = enemy_killed_back_start(*p_d7, object, slot);
		return;
	case 0x049EC8u:
		*p_d7 = enemy_killed_back_fly(*p_d7, object);
		return;
	case 0x049F38u:
		enemy_killed_back_lie(object, slot);
		return;
	case 0x049F6Au:
		enemy_killed_back_on_wall(object);
		return;
	case 0x049F78u:
		*p_d7 = enemy_killed_back_slide_down(*p_d7, object);
		return;
	case 0x049FA0u:
		enemy_killed_back_down(object, slot);
		return;
	case 0x049FE8u:
		*p_d7 = enemy_killed_forward_start(*p_d7, object, slot);
		return;
	case 0x04A004u:
		*p_d7 = enemy_killed_forward_fly(*p_d7, object);
		return;
	case 0x04A074u:
		enemy_killed_forward_lie(object, slot);
		return;
	case 0x04A0A6u:
		enemy_killed_forward_on_wall(object);
		return;
	case 0x04A0B4u:
		*p_d7 = enemy_killed_forward_slide_down(*p_d7, object);
		return;
	case 0x04A0DCu:
		enemy_killed_forward_down(object, slot);
		return;
	case 0x04A120u:
		*p_d7 = enemy_knocked_forward_start(*p_d7, object, slot);
		return;
	case 0x04A13Cu:
		*p_d7 = enemy_knocked_forward_fly(*p_d7, object);
		return;
	case 0x04A176u:
		enemy_knocked_forward_lie(object, slot);
		return;
	case 0x04A1BAu:
		enemy_knocked_forward_wait(object);
		return;
	case 0x04A202u:
		enemy_thrown_by_donatello_start(object);
		return;
	case 0x04A214u:
		enemy_thrown_by_donatello_swing(object, slot, p_d7, p_a5);
		return;
	case 0x04A25Au:
		*p_d7 = enemy_thrown_fly(*p_d7, object);
		return;
	case 0x04A396u:
		enemy_thrown_by_michelangelo_start(object);
		return;
	case 0x04A3A8u:
		enemy_thrown_by_michelangelo_swing(object, slot, p_d7, p_a5);
		return;
	case 0x04A454u:
		enemy_thrown_by_raphael_start(object);
		return;
	case 0x04A466u:
		enemy_thrown_by_raphael_swing(object, slot, p_d7, p_a5);
		return;
	case 0x04A512u:
		enemy_thrown_by_leonardo_start(object);
		return;
	case 0x04A524u:
		enemy_thrown_by_leonardo_swing(object, slot, p_d7, p_a5);
		return;
	case 0x04A5CEu:
		*p_d7 = enemy_knocked_back_start(*p_d7, object, slot);
		return;
	case 0x04A5F8u:
		*p_d7 = enemy_knocked_back_fly(*p_d7, object);
		return;
	case 0x04A632u:
		enemy_knocked_back_lie(object, slot);
		return;
	case 0x04A676u:
		enemy_knocked_back_wait(object);
		return;
	case 0x04A690u:
		enemy_knocked_back_get_up(object);
		return;
	case 0x04A6C6u:
		enemy_die_start(object);
		return;
	case 0x04A6E6u:
		enemy_die_vanish(object);
		return;
	case 0x04A718u:
		enemy_die_on_back_start(object);
		return;
	case 0x04ADC8u:
		*p_a5 = enemy_walk_to_target_sword_start(object, slot);
		return;
	case 0x04AE0Eu:
		*p_a5 = enemy_walk_to_side_sword_start(object, slot);
		return;
	case 0x04AE34u:
		enemy_walk_to_side_sword_crouch(object);
		return;
	case 0x04AE58u:
		*p_a5 = enemy_walk_to_side_sword_turn(object, slot, *p_a5);
		return;
	case 0x04AE8Cu:
		*p_a5 = enemy_walk_past_sword_start(object, slot);
		return;
	case 0x04AEBCu:
		*p_a5 = enemy_walk_past_sword_step(object, slot);
		return;
	case 0x04AF3Cu:
		enemy_back_off_sword_start(object, slot, p_d7, p_a5);
		return;
	case 0x04AF78u:
		*p_a5 = enemy_back_off_sword_walk(object, slot);
		return;
	case 0x04AFA8u:
		enemy_back_off_sword_wait(object, slot);
		return;
	case 0x04AFD2u:
		*p_a5 = enemy_walk_to_line_sword_start(object, slot);
		return;
	case 0x04B00Au:
		*p_a5 = enemy_walk_to_line_sword_step(object, slot);
		return;
	case 0x04B04Eu:
		*p_a5 = enemy_walk_to_target_hammer_start(object, slot);
		return;
	case 0x04B08Cu:
		*p_a5 = enemy_walk_to_side_hammer_start(object, slot);
		return;
	case 0x04B0B2u:
		*p_a5 = enemy_walk_to_side_far_step(object, slot);
		return;
	case 0x04B118u:
		*p_a5 = enemy_walk_to_side_far_hold(object, slot);
		return;
	case 0x04B146u:
		*p_a5 = enemy_stalk_hammer_start(object, slot);
		return;
	case 0x04B168u:
		*p_a5 = enemy_stalk_hammer_approach(object, slot);
		return;
	case 0x04B1ACu:
		*p_a5 = enemy_stalk_hammer_step(object, slot, *p_a5);
		return;
	case 0x04B250u:
		*p_a5 = enemy_walk_away_hammer_start(object, slot);
		return;
	case 0x04B284u:
		*p_a5 = enemy_walk_away_hammer_walk(object, slot);
		return;
	case 0x04B2BCu:
		enemy_walk_away_hammer_wait(object, slot);
		return;
	case 0x04B2EEu:
		enemy_slash_sword_start(object);
		return;
	case 0x04B32Au:
		enemy_low_slash_sword_start(object);
		return;
	case 0x04B37Au:
		enemy_leaping_slash_sword_start(object);
		return;
	case 0x04B3A2u:
		*p_d7 = enemy_leaping_slash_sword_leap(*p_d7, object);
		return;
	case 0x04B3F0u:
		*p_d7 = enemy_leaping_slash_sword_fly(*p_d7, object);
		return;
	case 0x04B452u:
		enemy_jump_slash_sword_start(object);
		return;
	case 0x04B47Au:
		*p_d7 = enemy_jump_slash_sword_leap(*p_d7, object);
		return;
	case 0x04B4A6u:
		*p_d7 = enemy_jump_slash_sword_fly(*p_d7, object);
		return;
	case 0x04B508u:
		enemy_flying_slash_sword_start(object);
		return;
	case 0x04B530u:
		*p_d7 = enemy_flying_slash_sword_leap(*p_d7, object);
		return;
	case 0x04B588u:
		*p_d7 = enemy_flying_slash_sword_fly(*p_d7, object);
		return;
	case 0x04B5EAu:
		enemy_smash_hammer_start(object);
		return;
	case 0x04B622u:
		enemy_smash_hammer_end(object);
		return;
	case 0x04B646u:
		enemy_flinch_back_sword_start(object);
		return;
	case 0x04B66Au:
		enemy_flinch_bent_sword_start(object);
		return;
	case 0x04B68Eu:
		enemy_flinch_reel_sword_start(object);
		return;
	case 0x04B6B2u:
		enemy_flinch_hunched_sword_start(object);
		return;
	case 0x04B6D6u:
		enemy_flinch_back_hammer_start(object);
		return;
	case 0x04B6FAu:
		enemy_flinch_bent_hammer_start(object);
		return;
	case 0x04C824u:
		enemy_step_out_start(object);
		return;
	case 0x04C840u:
		enemy_step_out_wait(object);
		return;
	case 0x04C860u:
		enemy_step_out_walk(object);
		return;
	case 0x04CF74u:
		enemy_walk_in_on_ledge_start(object);
		return;
	case 0x04CF9Au:
		enemy_walk_in_on_ledge_walk(object, handler, p_d5, p_d7, p_a5);
		return;
	case 0x04CFD6u:
		enemy_jump_down_leap(object);
		return;
	case 0x04D006u:
		*p_d7 = enemy_jump_down_fall_stairs(*p_d7, object);
		return;
	case 0x04D00Cu:
		*p_d7 = enemy_jump_down_fall(*p_d7, object);
		return;
	case 0x04D04Au:
		enemy_jump_down_land(object);
		return;
	case 0x04D072u:
		enemy_step_down_start(object);
		return;
	case 0x04D08Eu:
		enemy_step_down_wait(object);
		return;
	case 0x04D2D4u:
		enemy_leap_out_start(object);
		return;
	case 0x04D2FEu:
		*p_d7 = enemy_leap_out_fly(*p_d7, object);
		return;
	case 0x04D330u:
		enemy_leap_out_land(object);
		return;
	case 0x04D820u:
		enemy_hang_in_start(object);
		return;
	case 0x04D856u:
		enemy_hang_in_move(object);
		return;
	case 0x04D884u:
		*p_d7 = enemy_hang_in_drop(*p_d7, object);
		return;
	case 0x04D8ACu:
		enemy_hang_in_land(object);
		return;
	case 0x04D8E0u:
		enemy_kick_in_start(object);
		return;
	case 0x04D91Cu:
		enemy_kick_in_run(object);
		return;
	case 0x04D93Au:
		*p_d7 = enemy_kick_in_leap(*p_d7, object);
		return;
	case 0x04D96Cu:
		*p_d7 = enemy_kick_in_fly(*p_d7, object);
		return;
	case 0x04D9A6u:
		enemy_kick_in_land(object);
		return;
	case 0x04D9C2u:
		enemy_run_in_start(object);
		return;
	case 0x04D9F8u:
		enemy_run_in_run(object);
		return;
	case 0x04DA2Cu:
		enemy_dive_in_start(object);
		return;
	case 0x04DA58u:
		enemy_dive_in_run(object, handler, p_d5, p_d7, p_a5);
		return;
	case 0x04DA92u:
		enemy_dive_in_leap(object);
		return;
	case 0x04DACAu:
		enemy_dive_in_dive(object);
		return;
	case 0x04DAF6u:
		enemy_dive_in_curl(object);
		return;
	case 0x04DB1Au:
		enemy_dive_in_uncurl(object);
		return;
	case 0x04DB36u:
		*p_d7 = enemy_dive_in_land(*p_d7, object);
		return;
	case 0x04DB76u:
		enemy_walk_in_start(object);
		return;
	case 0x04DBAEu:
		enemy_walk_in_walk(object);
		return;
	case 0x04DBD4u:
		enemy_walk_in_ready(object);
		return;
	case 0x04DD50u:
		*p_a5 = enemy_walk_to_target_spear_start(object, slot);
		return;
	case 0x04DD8Eu:
		*p_a5 = enemy_walk_to_side_spear_start(object, slot);
		return;
	case 0x04DDC8u:
		*p_a5 = enemy_walk_off_line_spear_start(object, slot);
		return;
	case 0x04DDF8u:
		*p_a5 = enemy_walk_off_line_spear_step(object, slot);
		return;
	case 0x04DE78u:
		*p_a5 = enemy_back_off_spear_start(object, slot);
		return;
	case 0x04DEB2u:
		*p_a5 = enemy_back_off_spear_walk(object, slot);
		return;
	case 0x04DEF6u:
		enemy_step_aside_spear_start(object, slot, p_d7, p_a5);
		return;
	case 0x04DF40u:
		*p_a5 = enemy_step_aside_spear_step(object, slot);
		return;
	case 0x04DF80u:
		*p_a5 = enemy_walk_to_target_knife_start(object, slot);
		return;
	case 0x04DFBEu:
		*p_a5 = enemy_walk_to_side_knife_start(object, slot);
		return;
	case 0x04E018u:
		enemy_circle_knife_start(object, p_d7, p_a5);
		return;
	case 0x04E06Au:
		enemy_circle_knife_step(object, p_d7, p_a5);
		return;
	case 0x04E0C4u:
		enemy_keep_distance_knife_start(object);
		return;
	case 0x04E0E0u:
		enemy_keep_distance_knife_watch(object, slot, p_d7, p_a5);
		return;
	case 0x04E16Au:
		*p_a5 = enemy_keep_distance_knife_back_off(object, slot);
		return;
	case 0x04E186u:
		*p_a5 = enemy_keep_distance_knife_close_in(object, slot);
		return;
	case 0x04E1D0u:
		enemy_back_off_knife_start(object, slot, p_d7, p_a5);
		return;
	case 0x04E20Au:
		*p_a5 = enemy_back_off_knife_walk(object, slot);
		return;
	case 0x04E23Eu:
		enemy_back_off_knife_wait(object, slot);
		return;
	case 0x04E268u:
		*p_a5 = enemy_step_aside_knife_start(object, slot);
		return;
	case 0x04E2A0u:
		*p_a5 = enemy_step_aside_knife_step(object, slot);
		return;
	case 0x04E2D8u:
		enemy_spear_thrust_start(object);
		return;
	case 0x04E300u:
		enemy_spear_thrust_end(object);
		return;
	case 0x04E354u:
		enemy_spear_throw_start(object);
		return;
	case 0x04E380u:
		*p_a5 = enemy_spear_throw_release(object, *p_a5);
		return;
	case 0x04E3A0u:
		enemy_spear_throw_end(object, slot, handler, p_d5, p_d7, p_a5);
		return;
	case 0x04E3D8u:
		enemy_knife_stab_start(object);
		return;
	case 0x04E414u:
		enemy_high_kick_knife_start(object);
		return;
	case 0x04E464u:
		enemy_jump_kick_knife_crouch(object);
		return;
	case 0x04E48Cu:
		*p_d7 = enemy_jump_kick_knife_leap(*p_d7, object);
		return;
	case 0x04E4B8u:
		*p_d7 = enemy_jump_kick_knife_fly(*p_d7, object);
		return;
	case 0x04E510u:
		enemy_throw_knife_start(object);
		return;
	case 0x04E53Cu:
		*p_a5 = enemy_throw_knife_release(object, *p_a5);
		return;
	case 0x04E556u:
		enemy_throw_knife_end(object);
		return;
	case 0x04E57Au:
		enemy_flinch_back_spear_start(object);
		return;
	case 0x04E59Eu:
		enemy_flinch_bent_spear_start(object);
		return;
	case 0x04E5C2u:
		enemy_flinch_reel_spear_start(object);
		return;
	case 0x04E5E6u:
		enemy_flinch_hunched_spear_start(object);
		return;
	case 0x04E60Au:
		enemy_flinch_back_knife_start(object);
		return;
	case 0x04E62Eu:
		enemy_flinch_bent_knife_start(object);
		return;
	case 0x04E652u:
		enemy_flinch_reel_knife_start(object);
		return;
	case 0x04E676u:
		enemy_flinch_hunched_knife_start(object);
		return;
	case 0x04E7CEu:
		*p_a5 = enemy_walk_to_target_gun_start(object, slot);
		return;
	case 0x04E80Cu:
		*p_a5 = enemy_walk_to_side_gun_start(object, slot);
		return;
	case 0x04E84Au:
		*p_a5 = enemy_walk_to_range_gun_start(object, slot);
		return;
	case 0x04E870u:
		*p_a5 = enemy_walk_to_range_step(object, slot);
		return;
	case 0x04E8D8u:
		*p_a5 = enemy_walk_to_range_hold(object, slot, *p_a5);
		return;
	case 0x04E912u:
		enemy_back_off_gun_start(object, slot, p_d7, p_a5);
		return;
	case 0x04E94Cu:
		*p_a5 = enemy_back_off_gun_walk(object, slot);
		return;
	case 0x04E97Cu:
		enemy_back_off_gun_wait(object, slot);
		return;
	case 0x04E9A6u:
		enemy_run_away_gun_start(object, slot, p_d7, p_a5);
		return;
	case 0x04E9DEu:
		*p_a5 = enemy_run_away_gun_step(object, slot);
		return;
	case 0x04EA26u:
		*p_a5 = enemy_walk_to_target_axe_start(object, slot);
		return;
	case 0x04EA64u:
		*p_a5 = enemy_walk_to_side_axe_start(object, slot);
		return;
	case 0x04EAA2u:
		*p_a5 = enemy_walk_to_range_axe_start(object, slot);
		return;
	case 0x04EAE0u:
		enemy_back_off_axe_start(object, slot, p_d7, p_a5);
		return;
	case 0x04EB1Cu:
		*p_a5 = enemy_back_off_axe_walk(object, slot);
		return;
	case 0x04EB4Cu:
		enemy_back_off_axe_wait(object, slot);
		return;
	case 0x04EB76u:
		enemy_run_away_axe_start(object, slot, p_d7, p_a5);
		return;
	case 0x04EBAEu:
		*p_a5 = enemy_run_away_axe_step(object, slot);
		return;
	case 0x04EBF4u:
		enemy_throw_axes_start(object);
		return;
	case 0x04EC20u:
		*p_a5 = enemy_throw_axes_release(object, *p_a5);
		return;
	case 0x04EC72u:
		enemy_high_kick_axe_start(object);
		return;
	case 0x04ECAEu:
		enemy_gun_swing_start(object);
		return;
	case 0x04ECF4u:
		enemy_shoot_gun_start(object);
		return;
	case 0x04ED20u:
		*p_a5 = enemy_shoot_gun_fire(object, *p_a5);
		return;
	case 0x04ED72u:
		enemy_flinch_back_gun_start(object);
		return;
	case 0x04ED96u:
		enemy_flinch_bent_gun_start(object);
		return;
	case 0x04EDBAu:
		enemy_flinch_back_axe_start(object);
		return;
	case 0x04EDDEu:
		enemy_flinch_bent_axe_start(object);
		return;
	case 0x04EE02u:
		enemy_flinch_reel_axe_start(object);
		return;
	case 0x04F094u:
		enemy_throw_shuriken_start(object);
		return;
	case 0x04F0C0u:
		*p_a5 = enemy_throw_shuriken_release(object, *p_a5);
		return;
	case 0x04F10Au:
		enemy_boomerang_throw_wind_up(object);
		return;
	case 0x04F11Au:
		enemy_boomerang_throw_aim(object);
		return;
	case 0x04F150u:
		*p_a5 = enemy_boomerang_throw_release(object, *p_a5);
		return;
	case 0x04F186u:
		enemy_boomerang_throw_wait(object);
		return;
	case 0x04F2AEu:
		enemy_rocksteady_enter_wait(object);
		return;
	case 0x04F2CEu:
		enemy_rocksteady_enter_taunt(state_offset, object, p_d5, p_d7);
		return;
	case 0x04F318u:
		enemy_rocksteady_enter_ready(object);
		return;
	case 0x04F340u:
		enemy_rocksteady_enter_walk_in(object);
		return;
	case 0x04F35Eu:
		enemy_rocksteady_enter_wait_for_signal(object);
		return;
	case 0x04F380u:
		*p_a5 = enemy_rocksteady_enter_walk_in_with_bebop(object, *p_a5);
		return;
	case 0x04F736u:
		enemy_rocksteady_raise_fist_start(object, slot);
		return;
	case 0x04F764u:
		enemy_rocksteady_laugh_start(object, slot);
		return;
	case 0x04F792u:
		enemy_rocksteady_shake_fist_start(object, slot);
		return;
	case 0x04F7ACu:
		enemy_rocksteady_hold_pose(object);
		return;
	case 0x04F7C6u:
		enemy_rocksteady_draw_gun_start(object, slot);
		return;
	case 0x04F7E0u:
		enemy_rocksteady_pose_end(object);
		return;
	case 0x04F802u:
		enemy_rocksteady_put_gun_away_start(object, slot);
		return;
	case 0x04F830u:
		enemy_rocksteady_circle_start(object, slot, p_d7, p_a5);
		return;
	case 0x04F862u:
		enemy_rocksteady_circle_step(object, slot, p_d7, p_a5);
		return;
	case 0x04F8B0u:
		enemy_rocksteady_line_up_start(object, slot);
		return;
	case 0x04F8CCu:
		enemy_rocksteady_line_up_step(object, slot);
		return;
	case 0x04F936u:
		enemy_rocksteady_close_in_start(object, slot, p_d7, p_a5);
		return;
	case 0x04F952u:
		enemy_rocksteady_close_in_step(object, slot, p_d7, p_a5);
		return;
	case 0x04F9F2u:
		*p_a5 = enemy_rocksteady_shoot_around_start(object, *p_a5);
		return;
	case 0x04FA0Au:
		*p_a5 = enemy_rocksteady_shoot_around_fire(object, *p_a5);
		return;
	case 0x04FAE6u:
		enemy_rocksteady_pincer_charge_start(object);
		return;
	case 0x04FB06u:
		enemy_rocksteady_pincer_charge_run_to_edge(object);
		return;
	case 0x04FB3Cu:
		enemy_rocksteady_pincer_charge_wait_for_bebop(object);
		return;
	case 0x04FB82u:
		enemy_rocksteady_pincer_charge_get_ready(object);
		return;
	case 0x04FB86u:
		enemy_rocksteady_pincer_charge_run(object);
		return;
	case 0x04FBEEu:
		enemy_rocksteady_pincer_charge_crash(object);
		return;
	case 0x04FC1Cu:
		enemy_rocksteady_charge_start(object);
		return;
	case 0x04FC4Au:
		enemy_rocksteady_charge_get_ready(object);
		return;
	case 0x04FC76u:
		enemy_rocksteady_charge_run(object);
		return;
	case 0x04FCE8u:
		enemy_rocksteady_punch_start(object);
		return;
	case 0x04FD2Cu:
		enemy_rocksteady_blow_end(object);
		return;
	case 0x04FD60u:
		enemy_rocksteady_gun_butt_start(object);
		return;
	case 0x04FDB8u:
		*p_a5 = enemy_rocksteady_shoot_start(object, slot);
		return;
	case 0x04FE02u:
		*p_a5 = enemy_rocksteady_shoot_fire(object, *p_a5);
		return;
	case 0x04FE5Eu:
		enemy_rocksteady_flinch_start(object);
		return;
	case 0x04FE7Au:
		enemy_rocksteady_flinch_end(object);
		return;
	case 0x04FEC6u:
		enemy_rocksteady_reel_start(object);
		return;
	case 0x04FEFEu:
		enemy_rocksteady_flinch_from_behind_start(object);
		return;
	case 0x04FF5Eu:
		*p_d7 = enemy_rocksteady_knocked_down_start(*p_d7, object, slot);
		return;
	case 0x04FF98u:
		*p_d7 = enemy_rocksteady_knocked_down_fall(*p_d7, object);
		return;
	case 0x04FFCEu:
		enemy_rocksteady_knocked_down_slide(object);
		return;
	case 0x04FFFCu:
		enemy_rocksteady_knocked_down_lie(object);
		return;
	case 0x050036u:
		enemy_rocksteady_knocked_down_get_up(object);
		return;
	case 0x0500AEu:
		*p_d7 = enemy_rocksteady_defeated_start(*p_d7, object, slot);
		return;
	case 0x0500F0u:
		*p_d7 = enemy_rocksteady_defeated_fall(*p_d7, object);
		return;
	case 0x050152u:
		enemy_rocksteady_defeated_slide(object);
		return;
	case 0x050180u:
		enemy_rocksteady_defeated_blink(object);
		return;
	case 0x0502A6u:
		enemy_bebop_enter_wait_for_scroll(object);
		return;
	case 0x0502DCu:
		enemy_bebop_enter_wait(object);
		return;
	case 0x0502F2u:
		*p_d7 = enemy_bebop_enter_drop(*p_d7, object);
		return;
	case 0x050320u:
		*p_d7 = enemy_bebop_enter_jump(*p_d7, object);
		return;
	case 0x050340u:
		*p_d7 = enemy_bebop_enter_in_air(*p_d7, object);
		return;
	case 0x05036Eu:
		enemy_bebop_enter_ready(object);
		return;
	case 0x050392u:
		enemy_bebop_enter_wait_for_signal(object);
		return;
	case 0x0503B2u:
		*p_a5 = enemy_bebop_enter_walk_in(object, *p_a5);
		return;
	case 0x0506A2u:
		*p_d7 = enemy_bebop_stand_start(object, slot);
		return;
	case 0x0506D2u:
		enemy_bebop_stand_wait(object);
		return;
	case 0x0506ECu:
		enemy_bebop_walk_to_front_start(object, slot, p_d7, p_a5);
		return;
	case 0x050708u:
		enemy_bebop_walk_to_front_step(object, slot, p_d7, p_a5);
		return;
	case 0x050784u:
		enemy_bebop_walk_level_step(object, slot);
		return;
	case 0x050802u:
		enemy_bebop_walk_start(object, slot, p_d7, p_a5);
		return;
	case 0x05081Eu:
		enemy_bebop_close_in_step(object, slot, p_d7, p_a5);
		return;
	case 0x0508F0u:
		enemy_bebop_pincer_charge_start(object);
		return;
	case 0x050952u:
		enemy_bebop_pincer_charge_run_to_edge(object);
		return;
	case 0x050988u:
		enemy_bebop_pincer_charge_wait_for_rocksteady(object);
		return;
	case 0x0509CEu:
		enemy_bebop_pincer_charge_get_ready(object);
		return;
	case 0x0509D2u:
		enemy_bebop_pincer_charge_run(object);
		return;
	case 0x050AB8u:
		enemy_bebop_pincer_charge_crash(object);
		return;
	case 0x050AF0u:
		enemy_bebop_punch_start(object);
		return;
	case 0x050B38u:
		enemy_bebop_punch_end(object);
		return;
	case 0x050BA4u:
		enemy_bebop_charge_start(object);
		return;
	case 0x050BD2u:
		enemy_bebop_charge_get_ready(object);
		return;
	case 0x050BFEu:
		enemy_bebop_charge_run(object);
		return;
	case 0x050C82u:
		enemy_bebop_shoot_start(object);
		return;
	case 0x050CAEu:
		*p_a5 = enemy_bebop_shoot_fire(object, *p_a5);
		return;
	case 0x050CFAu:
		enemy_bebop_flinch_start(object);
		return;
	case 0x050D0Au:
		enemy_bebop_strike_back(object);
		return;
	case 0x050D40u:
		enemy_bebop_flinch_from_behind_start(object);
		return;
	case 0x050D56u:
		enemy_bebop_flinch_from_behind_end(object);
		return;
	case 0x050DA4u:
		*p_d7 = enemy_bebop_knocked_down_start(*p_d7, object, slot);
		return;
	case 0x050DCCu:
		*p_d7 = enemy_bebop_knocked_down_fall(*p_d7, object);
		return;
	case 0x050E00u:
		enemy_bebop_knocked_down_slide(object);
		return;
	case 0x050E2Eu:
		enemy_bebop_knocked_down_lie(object);
		return;
	case 0x050E8Eu:
		*p_d7 = enemy_bebop_defeated_start(*p_d7, object, slot);
		return;
	case 0x050EC0u:
		*p_d7 = enemy_bebop_defeated_fall(*p_d7, object);
		return;
	case 0x050F20u:
		enemy_bebop_defeated_slide(object);
		return;
	case 0x050F4Eu:
		enemy_bebop_defeated_blink(object);
		return;
	case 0x05103Au:
		enemy_mouser_enter_start(object);
		return;
	case 0x05106Eu:
		enemy_mouser_enter_hop(object);
		return;
	case 0x0510B2u:
		*p_d7 = enemy_mouser_enter_fall(*p_d7, object);
		return;
	case 0x0510E4u:
		enemy_mouser_enter_land(object);
		return;
	case 0x05135Cu:
		*p_d7 = enemy_mouser_bite_start(*p_d7, object);
		return;
	case 0x0513A2u:
		*p_d7 = enemy_mouser_bite_fly(*p_d7, object, slot);
		return;
	case 0x051404u:
		enemy_mouser_bite_land(object);
		return;
	case 0x051414u:
		enemy_mouser_bite_hold(object);
		return;
	case 0x0514EEu:
		enemy_mouser_hop_down(object);
		return;
	case 0x05152Cu:
		*p_d7 = enemy_mouser_hop_fall(*p_d7, object);
		return;
	case 0x05156Cu:
		enemy_mouser_hop_land(object);
		return;
	case 0x051588u:
		enemy_mouser_hop_right(object);
		return;
	case 0x0515D8u:
		enemy_mouser_hop_left(object);
		return;
	case 0x051628u:
		enemy_mouser_hop_up(object);
		return;
	case 0x051654u:
		*p_d7 = enemy_mouser_hop_up_fall(*p_d7, object);
		return;
	case 0x0516A0u:
		enemy_mouser_hop_up_right(object);
		return;
	case 0x0516D2u:
		*p_d7 = enemy_mouser_hop_up_fall_blocked(*p_d7, object);
		return;
	case 0x05171Eu:
		enemy_mouser_hop_up_left(object);
		return;
	case 0x051774u:
		enemy_mouser_chase_start((uint8_t)state_offset, object, p_d7, p_a5);
		return;
	case 0x0517A0u:
		enemy_mouser_chase_run(object, slot, p_d7, p_a5);
		return;
	case 0x051812u:
		*p_d7 = enemy_mouser_chase_pause(object);
		return;
	case 0x05181Cu:
		enemy_mouser_chase_wait(object);
		return;
	case 0x05183Eu:
		*p_a5 = enemy_mouser_creep_to_side_start((uint8_t)state_offset, object);
		return;
	case 0x05185Cu:
		enemy_mouser_creep_to_side_step(object, slot, p_d7, p_a5);
		return;
	case 0x0518D4u:
		enemy_mouser_back_off_start(object, slot, p_d7, p_a5);
		return;
	case 0x05190Eu:
		*p_a5 = enemy_mouser_back_off_run((uint8_t)state_offset, object);
		return;
	case 0x05192Cu:
		*p_d7 = enemy_mouser_sit(object);
		return;
	case 0x05194Cu:
		enemy_mouser_back_off_wait(object, slot);
		return;
	case 0x051980u:
		*p_d7 = enemy_mouser_thrown_off_start(*p_d7, object);
		return;
	case 0x0519E4u:
		*p_d7 = enemy_mouser_break_down_start(*p_d7, object);
		return;
	case 0x051A2Cu:
		*p_d7 = enemy_mouser_knocked_flying_start(*p_d7, object, slot);
		return;
	case 0x051A6Cu:
		*p_d7 = enemy_mouser_fly_and_crash(*p_d7, object);
		return;
	case 0x051AB0u:
		enemy_mouser_remove(object, slot);
		return;
	case 0x051CC8u:
		enemy_rodney_enter_wait(object);
		return;
	case 0x051CFCu:
		enemy_rodney_enter_burst_out(object);
		return;
	case 0x051D36u:
		enemy_rodney_enter_fall(object);
		return;
	case 0x051D72u:
		enemy_rodney_enter_land(object);
		return;
	case 0x05200Eu:
		enemy_rodney_rush_start(object, slot, p_d7, p_a5);
		return;
	case 0x05207Au:
		*p_d7 = enemy_rodney_rush_run(object);
		return;
	case 0x052116u:
		enemy_rodney_brake(object);
		return;
	case 0x05214Eu:
		enemy_rodney_rush_across_start(object);
		return;
	case 0x052188u:
		*p_d7 = enemy_rodney_rush_across_run(object);
		return;
	case 0x052244u:
		enemy_rodney_back_off_start(object);
		return;
	case 0x052286u:
		enemy_rodney_slide_to_stop(object, p_d5, p_d7);
		return;
	case 0x0522CCu:
		enemy_rodney_back_off_turn(object);
		return;
	case 0x0522F0u:
		enemy_rodney_back_off_wait(object, slot);
		return;
	case 0x052330u:
		enemy_rodney_shoot_start(object);
		return;
	case 0x052378u:
		enemy_rodney_shoot_turn(object);
		return;
	case 0x0523A8u:
		enemy_rodney_shoot_ready(object);
		return;
	case 0x0523C6u:
		*p_a5 = enemy_rodney_shoot_fire(object, *p_a5);
		return;
	case 0x05240Eu:
		enemy_rodney_shoot_end(object);
		return;
	case 0x052484u:
		enemy_rodney_whip_start(object);
		return;
	case 0x05248Cu:
		enemy_rodney_whip_slow_down(object, p_d5, p_d7);
		return;
	case 0x052494u:
		enemy_rodney_whip_wind_up(object);
		return;
	case 0x0524D2u:
		*p_a5 = enemy_rodney_whip_throw_arm(object, *p_a5);
		return;
	case 0x052530u:
		*p_d7 = enemy_rodney_whip_reach(*p_d7, object);
		return;
	case 0x0525D0u:
		*p_d7 = enemy_rodney_whip_lash(object);
		return;
	case 0x0525E6u:
		enemy_rodney_whip_retract(object);
		return;
	case 0x052624u:
		*p_d7 = enemy_rodney_knocked_back_start(*p_d7, object, slot);
		return;
	case 0x05265Cu:
		*p_d7 = enemy_rodney_knocked_back_fall(*p_d7, object);
		return;
	case 0x05268Au:
		enemy_rodney_knocked_back_end(object);
		return;
	case 0x0526CCu:
		*p_d7 = enemy_rodney_destroyed_start(*p_d7, object, slot);
		return;
	case 0x052704u:
		*p_d7 = enemy_rodney_destroyed_fall(*p_d7, object);
		return;
	case 0x05273Cu:
		enemy_rodney_destroyed_land(object);
		return;
	case 0x052756u:
		enemy_rodney_destroyed_end(object, slot);
		return;
	case 0x052CAEu:
		enemy_shredder_appear_wait(object);
		return;
	case 0x052CDCu:
		enemy_shredder_appear_step_out(object);
		return;
	case 0x052D20u:
		enemy_shredder_appear_walk_in(object);
		return;
	case 0x052D62u:
		*p_a5 = enemy_shredder_split_start(object);
		return;
	case 0x052DB2u:
		enemy_shredder_split_blink(object);
		return;
	case 0x052DE4u:
		enemy_shredder_clone_appear_start(object);
		return;
	case 0x052E1Au:
		enemy_shredder_clone_appear_blink(object);
		return;
	case 0x052E84u:
		*p_a5 = enemy_shredder_walk_to_side_start(object, slot);
		return;
	case 0x052EAEu:
		*p_a5 = enemy_shredder_walk_to_side_step(object, slot);
		return;
	case 0x052F20u:
		enemy_shredder_walk_to_side_crouch(object);
		return;
	case 0x052F52u:
		*p_a5 = enemy_shredder_walk_to_side_turn(object, slot, *p_a5);
		return;
	case 0x052F80u:
		*p_a5 = enemy_shredder_walk_to_side_hold(object, slot);
		return;
	case 0x052FECu:
		*p_a5 = enemy_shredder_walk_to_target_start(object, slot);
		return;
	case 0x053016u:
		enemy_shredder_walk_to_target_step(object, slot, p_d7, p_a5);
		return;
	case 0x053084u:
		enemy_shredder_guard_start(object);
		return;
	case 0x0530B0u:
		enemy_shredder_guard_counter(object);
		return;
	case 0x053102u:
		enemy_shredder_back_off_start(object, slot, p_d7, p_a5);
		return;
	case 0x05314Cu:
		*p_a5 = enemy_shredder_back_off_walk(object, slot);
		return;
	case 0x053198u:
		enemy_shredder_back_off_wait(object, slot);
		return;
	case 0x0531C8u:
		enemy_shredder_lose_bar_start(object);
		return;
	case 0x0531E4u:
		*p_a5 = enemy_shredder_lose_bar_end(object, *p_a5);
		return;
	case 0x053256u:
		enemy_shredder_flinch_start(object);
		return;
	case 0x0532AAu:
		enemy_shredder_flinch_end(object);
		return;
	case 0x0532E8u:
		enemy_shredder_flinch_from_behind_start(object);
		return;
	case 0x053368u:
		*p_d7 = enemy_shredder_defeated_start(*p_d7, object, slot);
		return;
	case 0x053398u:
		*p_d7 = enemy_shredder_defeated_fall(*p_d7, object);
		return;
	case 0x0533D2u:
		enemy_shredder_defeated_land(object);
		return;
	case 0x053422u:
		enemy_shredder_defeated_wait(object);
		return;
	case 0x05343Eu:
		enemy_shredder_defeated_stay();
		return;
	case 0x053458u:
		enemy_shredder_vanish_start(object, slot);
		return;
	case 0x053482u:
		enemy_shredder_vanish_wait(object);
		return;
	case 0x053496u:
		enemy_shredder_vanish_blink(object);
		return;
	case 0x0534D8u:
		enemy_shredder_kick_start(object);
		return;
	case 0x05350Eu:
		enemy_shredder_blow_end(object);
		return;
	case 0x053538u:
		enemy_shredder_thrust_start(object);
		return;
	case 0x053582u:
		enemy_shredder_slash_start(object);
		return;
	case 0x0535CCu:
		enemy_shredder_sweep_start(object);
		return;
	case 0x053628u:
		*p_a5 = enemy_shredder_ray_start(object, slot);
		return;
	case 0x053644u:
		*p_a5 = enemy_shredder_ray_line_up(object, slot);
		return;
	case 0x0536CAu:
		enemy_shredder_ray_charge(object);
		return;
	case 0x0536EAu:
		*p_a5 = enemy_shredder_ray_fire(object, *p_a5);
		return;
	case 0x05373Au:
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
void enemy_attack_chance(uint32_t object, uint32_t *p_chance, uint32_t *p_count, uint32_t *p_attack)
{
	uint32_t chance = *p_chance;
	uint32_t count = *p_count;
	uint32_t attack = *p_attack;
	uint16_t target_dx = 0, target_dy = 0;
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
	*p_count = count;
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
void enemy_nearest_turtles(uint32_t object, uint32_t *p_next_distance, uint32_t *p_count, uint32_t *p_next_closest, uint32_t *p_closest)
{
	uint32_t next_distance = *p_next_distance;
	uint32_t count = *p_count;
	uint32_t next_closest = *p_next_closest;
	uint32_t closest = *p_closest;
	uint16_t distance = 0, y_distance = 0;
	uint32_t closest_distance = 0, object_2 = 0;
	uint16_t t1 = 0, t2 = 0, t4 = 0;
	uint32_t t3 = 0;

	if (turtles_in_play == 0u)
	{
		closest = nearest_turtle;
		next_closest = second_nearest_turtle;
	}
	else
	{
		count = 3u;
		object_2 = OBJECT_ADDRESS(0);
		next_distance = 0xFFFFFFFFu;
		closest_distance = 0xFFFFFFFFu;
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
						next_distance = (next_distance & ~0xFFFFu) | distance;
						next_closest = object_2;
					}
				}
				else
				{
					t3 = closest_distance;
					closest_distance = next_distance;
					next_distance = t3;
					next_closest = closest;
					closest_distance = (closest_distance & ~0xFFFFu) | distance;
					closest = object_2;
				}
			}
			t4 = count - 1u;
			count = t4;
			object_2 += 0x50u;
		} while (t4 != 0xFFFFu);
		nearest_turtle = closest;
		second_nearest_turtle = next_closest;
	}
	*p_next_distance = next_distance;
	*p_count = count;
	*p_next_closest = next_closest;
	*p_closest = closest;
}

/* the two players' slots whose turtles are nearest to the object, kept; with no turtle in play, the ones
   kept before */
void enemy_nearest_slots(uint32_t object, uint32_t *p_next_distance, uint32_t *p_count, uint32_t *p_next_closest, uint32_t *p_closest)
{
	uint32_t next_distance = *p_next_distance;
	uint32_t count = *p_count;
	uint32_t next_closest = *p_next_closest;
	uint32_t closest = *p_closest;
	uint16_t distance = 0, y_distance = 0;
	uint32_t closest_distance = 0, object_2 = 0, player_slot = 0;
	uint16_t t1 = 0, t2 = 0, t4 = 0;
	uint32_t t3 = 0;

	if (turtles_in_play == 0u)
	{
		closest = nearest_slot;
		next_closest = second_nearest_slot;
	}
	else
	{
		count = 3u;
		player_slot = PLAYER_SLOT_ADDRESS(0);
		next_distance = 0xFFFFFFFFu;
		closest_distance = 0xFFFFFFFFu;
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
						next_distance = (next_distance & ~0xFFFFu) | distance;
						next_closest = player_slot;
					}
				}
				else
				{
					t3 = closest_distance;
					closest_distance = next_distance;
					next_distance = t3;
					next_closest = closest;
					closest_distance = (closest_distance & ~0xFFFFu) | distance;
					closest = player_slot;
				}
			}
			t4 = count - 1u;
			count = t4;
			player_slot += 0x10u;
		} while (t4 != 0xFFFFu);
		nearest_slot = closest;
		second_nearest_slot = next_closest;
	}
	*p_next_distance = next_distance;
	*p_count = count;
	*p_next_closest = next_closest;
	*p_closest = closest;
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
	return random;
}

/* Unused: the larger of its distances to its turtle, across or up and down */
void unreached_048480(uint32_t enemy, uint32_t a5)
{
	uint16_t distance = 0, target_dy = 0;

	distance = OBJECT(enemy)->enemy.target_dx;
	target_dy = OBJECT(enemy)->enemy.target_dy;
	if (distance <= target_dy)
		distance = (uint16_t)target_dy;
}

/* the animation of the table toward the object, higher or lower on the screen, flipped to face it */
uint32_t enemy_face_object(uint8_t direction, uint32_t object, uint32_t animations, uint32_t object_2)
{
	uint32_t facing = 0;

	facing = enemy_facing_bits(direction, 0u, object, object_2);
	return enemy_set_facing_animation((uint8_t)facing, object, animations);
}

/* the animation of the table for the facing, one for each way up or down the screen, flipped to its side */
uint32_t enemy_set_facing_animation(uint8_t facing, uint32_t object, uint32_t animations)
{
	uint32_t animation_entry = 0;

	if ((int8_t)facing < 0)
		animations += 4u;
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->flags &= 0xBFu;
	if ((facing & 0x40u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	OBJECT(object)->enemy.facing = facing;
	return animations;
}

/* the same toward the object, only when its facing changes */
uint32_t enemy_turn_to_object(uint8_t direction, uint32_t object, uint32_t animations, uint32_t object_2)
{
	uint32_t facing = 0;

	facing = enemy_facing_bits(direction, 0u, object, object_2);
	return enemy_turn_animation((uint8_t)facing, object, animations);
}

/* a new animation only when the facing changes between up and down the screen; the flip follows a turn
   across */
uint32_t enemy_turn_animation(uint8_t facing, uint32_t object, uint32_t animations)
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
		return animations;
	}
	if ((int8_t)facing < 0)
		animations += 4u;
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	if ((OBJECT(object)->enemy.facing & 0x40u) != 0u)
		OBJECT(object)->flags ^= 0x40u;
	OBJECT(object)->enemy.facing = facing;
	return animations;
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
uint32_t enemy_face_direction(uint8_t direction, uint32_t object, uint32_t animations)
{
	uint32_t facing = 0;

	facing = direction_facing_bits(direction, 0u);
	return enemy_set_facing_animation((uint8_t)facing, object, animations);
}

/* the same for a direction, only when its facing changes */
uint32_t enemy_turn_to_direction(uint8_t direction, uint32_t object, uint32_t animations)
{
	uint32_t facing = 0;

	facing = direction_facing_bits(direction, 0u);
	return enemy_turn_animation((uint8_t)facing, object, animations);
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
void enemy_keep_on_screen(uint32_t object)
{
	if (OBJECT(object)->screen_x >= 0x170u)
		OBJECT(object)->contacts |= 8u;
	if (OBJECT(object)->screen_x < 0x80u)
		OBJECT(object)->contacts |= 0x10u;
}

/* whether the enemy is on the screen across */
void enemy_on_screen(uint32_t object, uint32_t *p_on_screen, uint32_t *p_zero)
{
	uint32_t zero = *p_zero;

	if (OBJECT(object)->screen_x >= 0x190u || OBJECT(object)->screen_x < 0x70u)
	{
		zero = 0xFFFFFFFFu == 0u;
		*p_on_screen = 0xFFFFFFFFu;
	}
	else
	{
		zero = 1u;
		*p_on_screen &= ~0xFFu;
	}
	*p_zero = zero;
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
void enemy_landed(uint32_t object, uint32_t *p_d7, uint32_t *p_zero)
{
	uint32_t d7 = *p_d7;
	uint32_t zero = *p_zero;
	uint32_t floor_height = 0;

	floor_height = (d7 & ~0xFFu) | OBJECT(object)->height_whole;
	if ((uint8_t)floor_height >= 0xF0u || (uint8_t)floor_height < OBJECT(object)->floor_height)
	{
		OBJECT(object)->height_whole = OBJECT(object)->floor_height;
		zero = 1u;
		OBJECT(object)->height &= 0xFF00u;
	}
	else
	{
		zero = 0xFFFFFFFFu == 0u;
		floor_height = 0xFFFFFFFFu;
	}
	*p_d7 = floor_height;
	*p_zero = zero;
}

/* starts a leap or a fall along an arc record: its speeds and a first step across unless blocked */
uint32_t enemy_start_arc(uint32_t d7, uint32_t object, uint32_t arc)
{
	uint32_t x_step = 0;
	uint32_t t1 = 0;

	OBJECT(object)->x_speed = ARC(arc)->x_speed;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->height_speed = ARC(arc)->height_speed;
	x_step = (d7 & ~0xFFFFu) | (uint16_t)ARC(arc)->x_step;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
		x_step = (x_step & ~0xFFFFu) | (uint16_t)(0u - x_step);
	}
	if (area == 7u && OBJECT(object)->floor_height != 0u)
		return x_step;
	t1 = (int16_t)x_step;
	x_step = t1;
	if ((int32_t)t1 >= 0)
	{
		if ((OBJECT(object)->contacts & 8u) == 0u)
			OBJECT(object)->x += x_step;
		return x_step;
	}
	if ((OBJECT(object)->contacts & 0x10u) != 0u)
		return x_step;
	OBJECT(object)->x += x_step;
	return x_step;
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
void enemy_leave_side(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->enemy.approach != 1u)
		return;
	enemy_give_side_back(object, player_slot);
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
void enemy_floor_contacts(uint32_t object, uint32_t *p_floor_right, uint32_t *p_floor_left, uint32_t *p_floor_up, uint32_t *p_floor_down, uint32_t *p_d5, uint32_t *p_new_contacts, uint32_t *p_a5)
{
	uint32_t d5 = *p_d5;
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
		read_floor_cells_on_screen(object, &d5, p_a5, &floor_cell_place);
		enemy_floor_cell_place = floor_cell_place;
		if (floor_cell >= 0xFDu)
		{
			enemy_floor_edges(object, p_floor_right, p_floor_left, p_floor_up, p_floor_down, p_new_contacts);
			*p_a5 = 0x4881Bu;
			OBJECT(object)->contacts &= TABLE8(floor_contact_masks, 0x3u + (uint32_t)(int16_t)(int8_t)floor_cell);
		}
		else
			enemy_floor_edges(object, p_floor_right, p_floor_left, p_floor_up, p_floor_down, p_new_contacts);
	}
	*p_d5 = (*p_d5 & 0xFFFF0000u) | (d5 & 0xFFFFu);
}

/* the floor map's walls around it into the contacts */
void enemy_floor_edges(uint32_t object, uint32_t *p_floor_right, uint32_t *p_floor_left, uint32_t *p_floor_up, uint32_t *p_floor_down, uint32_t *p_new_contacts)
{
	uint32_t new_contacts = *p_new_contacts;
	uint8_t floor_down = 0, floor_left = 0, floor_right = 0, floor_up = 0;
	uint8_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

	new_contacts &= ~0xFFu;
	t1 = floor_cell_right;
	floor_right = t1;
	if ((int8_t)t1 < 0 && floor_right == height_cell_right)
		new_contacts = 8u;
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
	*p_new_contacts = new_contacts;
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
	} while ((int8_t)mem_read8(floor_cell_place) < 0);
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
	} while ((int8_t)mem_read8(floor_cell_place) < 0);
}

/* kind 00, the bare-handed Foot soldier: its floor, one more enemy on the screen, then its setup */
void enemy_foot_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	enemy_set_floor(object);
	enemies_on_screen++;
	enemy_foot_soldier_setup(object, player_slot, p_d5, p_d7, p_a5);
}

/* the bare-handed soldier's turtle, health, definition and shadow; its way in by its parameter, or
   straight to thinking */
void enemy_foot_soldier_setup(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint8_t first_move = 0;
	uint32_t animation = 0;
	uint8_t parameter = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
	animation = animations_0547C6_ADDRESS;
	set_animation(0u, object, animation);
}

/* the way in its move picks, keeping track of its turtle, the objects and the floor */
void enemy_enter(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	states = TABLE32(enemy_ways_in, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
	*p_slot = slot;
}

/* the thinking: a new turtle every 16 frames, a side by it or away, an attack by its definition's
   chances or its next move */
void enemy_think(uint32_t object, uint32_t second_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	bool L_048BCA = false, L_048CC0 = false;
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint16_t count = 0;
	uint32_t approach = 0, attack = 0, chance = 0, floor_down = 0, floor_left = 0, floor_right = 0, floor_up = 0, ignored = 0, ignored_2 = 0, turtle = 0;
	uint8_t retarget_time = 0, t3 = 0, floor_height = 0, t4 = 0, t5 = 0, move_chance = 0, t6 = 0, chance_2 = 0, t8 = 0;
	uint16_t t7 = 0, t9 = 0;
	uint32_t t1 = 0, t2 = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy.target;
	retarget_time = OBJECT(object)->enemy.retarget_time;
	OBJECT(object)->enemy.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			PLAYER_SLOT(*p_slot)->enemies--;
			enemy_nearest_slots(object, p_d5, p_d7, p_a5, &second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = *p_a5;
				*p_a5 = second_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						t2 = *p_a5;
						*p_a5 = second_slot;
						second_slot = t2;
					}
				}
			}
			*p_d7 = (*p_d7 & ~0xFFFFu) | (uint16_t)second_slot;
			if ((uint16_t)*p_d7 != OBJECT(object)->enemy.target)
				enemy_leave_side(object, *p_slot);
			OBJECT(object)->enemy.target = second_slot;
			*p_slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	if (boss_defeated != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t3 = enemies_on_screen - 1u;
		enemies_on_screen = t3;
		if ((int8_t)t3 < 0)
			enemies_on_screen = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		enemy_leave_side(object, *p_slot);
		mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t3 = enemies_on_screen - 1u;
		enemies_on_screen = t3;
		if ((int8_t)t3 < 0)
			enemies_on_screen = 0u;
		return;
	}
	floor_height = OBJECT(object)->floor_height;
	*p_d7 = (*p_d7 & ~0xFFFFu) | floor_height;
	if (floor_height != 0u)
	{
		*p_d7 = (*p_d7 & ~0xFFFFu) | (uint16_t)(*p_d7 + OBJECT(object)->screen_y);
		if ((int16_t)*p_d7 >= 0xF0)
		{
			enemy_leave_side(object, *p_slot);
			mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
			OBJECT(object)->flags = 0u;
			OBJECT(object)->contacts = 0u;
			t3 = enemies_on_screen - 1u;
			enemies_on_screen = t3;
			if ((int8_t)t3 < 0)
				enemies_on_screen = 0u;
			return;
		}
	}
	turtle = mem_read32(*p_slot);
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &floor_right, &floor_left, &floor_up, &floor_down, p_d5, p_d7, p_a5);
	zero = enemy_check_floor_edge((uint8_t)floor_right, (uint8_t)floor_left, (uint8_t)floor_up, (uint8_t)floor_down, object, *p_slot);
	if (zero == 0u)
	{
		return;
	}
	if (OBJECT(object)->enemy.approach != 0u)
	{
		if (OBJECT(object)->enemy.approach != 1u)
		{
			if (OBJECT(object)->enemy.approach == 2u && mem_read8(*p_slot + 5u) < 0xC0u)
			{
				OBJECT(object)->enemy.approach = 0u;
				OBJECT(object)->enemy.move_time = 0u;
			}
		}
		else if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x50u)
			enemy_give_side_back(object, *p_slot);
	}
	else if (mem_read8(*p_slot + 5u) < 0xC0u)
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
						t4 = mem_read8(*p_slot + 5u);
						mem_write8(*p_slot + 5u, t4 | 0x40u);
						if ((t4 & 0x40u) != 0u)
						{
							t5 = mem_read8(*p_slot + 5u);
							mem_write8(*p_slot + 5u, t5 | 0x80u);
							if ((t5 & 0x80u) != 0u)
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
					t5 = mem_read8(*p_slot + 5u);
					mem_write8(*p_slot + 5u, t5 | 0x80u);
					L_048BCA = (t5 & 0x80u) != 0u;
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
		turtle = mem_read32(*p_slot);
		if (OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u)
		{
			L_048CC0 = mem_read8(mem_read32(*p_slot) + 4u) != 3u || OBJECT(object)->enemy.target_dy >= 8u || (int8_t)OBJECT(object)->enemy.target_faces >= 0;
			if (!L_048CC0)
			{
				*p_d7 = 0x64u;
				*p_d7 = random_below((uint16_t)*p_d7, object);
				L_048CC0 = OBJECT(object)->enemy.dodge_chance <= (uint8_t)*p_d7;
				if (!L_048CC0)
				{
					enemy_give_side_back(object, *p_slot);
					OBJECT(object)->enemy.approach = 4u;
					OBJECT(object)->enemy.move_time = 0u;
				}
			}
		}
		else
		{
			enemy_give_side_back(object, *p_slot);
			OBJECT(object)->enemy.approach = 3u;
			OBJECT(object)->enemy.move_time = 0u;
		}
	}
	if (L_048CC0)
	{
		enemy_on_screen(object, p_d7, &zero);
		if (zero)
		{
			turtle = mem_read32(*p_slot);
			if (OBJECT(turtle)->routine != 4u || (int8_t)hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] < 0)
			{
				attack = OBJECT(object)->enemy.definition + 0x28u;
				enemy_attack_chance(object, &chance, p_d7, &attack);
				if ((int8_t)chance >= 0)
				{
					*p_d7 = 0x64u;
					*p_d7 = random_below((uint16_t)*p_d7, object);
					if ((uint8_t)*p_d7 < (uint8_t)chance)
					{
						*p_d7 = 0x64u;
						*p_d7 = random_below((uint16_t)*p_d7, object);
						count = 3u;
						do
						{
							move_chance = ATTACK_RECORD(attack + (int16_t)count)->move_chance;
							t6 = *p_d7;
							*p_d7 = (*p_d7 & ~0xFFu) | (uint8_t)(t6 - move_chance);
							if (t6 < move_chance)
							{
								OBJECT(object)->enemy.move = ((count & ~0xFFu) | ATTACK_RECORD(attack + (int16_t)count)->move) << 2;
								OBJECT(object)->routine = 3u;
								OBJECT(object)->state = 0u;
								OBJECT(object)->flags &= ~8u;
								return;
							}
							t7 = count - 1u;
							count = t7;
						} while (t7 != 0xFFFFu);
					}
				}
			}
		}
	}
	if (OBJECT(object)->enemy.move_time == 0u)
	{
		approach = OBJECT(object)->enemy.definition + (uint16_t)((uint32_t)OBJECT(object)->enemy.approach << 3);
		*p_d7 = 0x64u;
		*p_d7 = random_below((uint16_t)*p_d7, object);
		count = 3u;
		for (;;)
		{
			chance_2 = APPROACH_MOVES(approach + (int16_t)count)->chance;
			t8 = *p_d7;
			*p_d7 = (*p_d7 & ~0xFFu) | (uint8_t)(t8 - chance_2);
			if (t8 < chance_2)
				break;
			t9 = count - 1u;
			count = t9;
			if (t9 == 0xFFFFu)
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
		enemy_shuffle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* the attack its move picks, keeping track of its turtle, the objects and the floor */
void enemy_attack(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, turtle = 0;
	uint32_t jump_target = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy.target;
	turtle = PLAYER_SLOT(*p_slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	jump_target = TABLE32(enemy_attacks, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	switch (jump_target)
	{
	case 0x049A06u:
		enemy_jab(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049A52u:
		enemy_chop(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049A8Eu:
		enemy_swing(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049ACAu:
		enemy_high_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049B06u:
		enemy_jump_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049BBCu:
		enemy_long_jump_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049C72u:
		enemy_attack_none_after_long_jump_kick();
		return;
	case 0x049C74u:
		enemy_attack_none_before_grab();
		return;
	case 0x049C76u:
		enemy_grab(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B2DAu:
		enemy_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B316u:
		enemy_low_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B352u:
		enemy_leaping_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B42Au:
		enemy_jump_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B4E0u:
		enemy_flying_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B5D6u:
		enemy_smash_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E2C4u:
		enemy_spear_thrust(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E336u:
		enemy_spear_throw(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E3C4u:
		enemy_knife_stab(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E400u:
		enemy_high_kick_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E43Cu:
		enemy_jump_kick_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E4F2u:
		enemy_throw_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04EBD6u:
		enemy_throw_axes(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04EC5Eu:
		enemy_high_kick_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04EC9Au:
		enemy_gun_swing(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04ECD6u:
		enemy_shoot_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04F076u:
		enemy_throw_shuriken(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04F0E4u:
		enemy_boomerang_throw(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FA96u:
		enemy_rocksteady_pincer_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FC04u:
		enemy_rocksteady_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FCD4u:
		enemy_rocksteady_punch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FD4Cu:
		enemy_rocksteady_gun_butt(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FD88u:
		enemy_rocksteady_shoot(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x0508A0u:
		enemy_bebop_pincer_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x050ADCu:
		enemy_bebop_punch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x050B8Cu:
		enemy_bebop_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x050C68u:
		enemy_bebop_shoot(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x052306u:
		enemy_rodney_shoot(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x052422u:
		enemy_rodney_whip(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x0534C4u:
		enemy_shredder_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x053524u:
		enemy_shredder_thrust(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x05356Eu:
		enemy_shredder_slash(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x0535B8u:
		enemy_shredder_sweep(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x053602u:
		enemy_shredder_ray(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* hit: the sound, then its reaction by where the blow came from, its health and its definition */
void enemy_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1, uint32_t *p_a5)
{
	uint8_t sound = 0;
	uint16_t hit_side = 0;
	uint32_t turtle = 0;

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
				OBJECT(object)->enemy.move = (uint32_t)mem_read8(OBJECT(object)->enemy.definition + 0x58u + hit_side) << 2;
			}
		}
		else
		{
			if (diagonal_scroll >= 0 && (int8_t)mem_read8(OBJECT(object)->enemy.definition + 0x58u + (hit_side & 0x7Fu)) >= 0)
			{
				OBJECT(object)->enemy.move = 0x10u;
				OBJECT(object)->routine++;
				enemy_react(object, p_d5, p_d7, p_a1, p_a5);
				return;
			}
			OBJECT(object)->enemy.move = 0x20u;
		}
	}
	else if (OBJECT(object)->health == 0u)
	{
		if (diagonal_scroll < 0 || (int8_t)mem_read8(OBJECT(object)->enemy.definition + 0x58u + (hit_side & 0x7Fu)) < 0)
			OBJECT(object)->enemy.move = 0x24u;
		else
			OBJECT(object)->enemy.move = 0x14u;
	}
	else
	{
		if (OBJECT(object)->floor_height != OBJECT(object)->height_whole)
		{
			OBJECT(object)->enemy.move = 0x14u;
			OBJECT(object)->routine++;
			enemy_react(object, p_d5, p_d7, p_a1, p_a5);
			return;
		}
		OBJECT(object)->enemy.move = 0xCu;
	}
	OBJECT(object)->routine++;
	enemy_react(object, p_d5, p_d7, p_a1, p_a5);
}

/* the reaction to a hit its move picks, against the objects and the floor */
void enemy_react(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy.target;
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored, &ignored_2, &ignored_3, &ignored_4, p_d5, p_d7, p_a5);
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy.move)
	{
	case 228:
		enemy_crash_into_scenery(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_crash_into_scenery_low(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_flinch_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_flinch_bent(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_flinch_reel(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_flinch_hunched(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_killed_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_killed_forward(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_knocked_forward(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_thrown_by_donatello(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		*p_a5 = enemy_let_go_by_donatello(object, *p_slot);
		return;
	case 136:
		enemy_thrown_by_michelangelo(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		*p_a5 = enemy_let_go_by_michelangelo(object, *p_slot);
		return;
	case 188:
		enemy_thrown_by_raphael(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		*p_a5 = enemy_let_go_by_raphael(object, *p_slot);
		return;
	case 196:
		enemy_thrown_by_leonardo(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		*p_a5 = enemy_let_go_by_leonardo(object, *p_slot);
		return;
	case 20:
		enemy_knocked_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_die(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_die_on_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_flinch_back_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_flinch_bent_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_flinch_reel_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_flinch_hunched_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_flinch_back_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_flinch_bent_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_flinch_hunched_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_flinch_back_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_flinch_bent_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_flinch_reel_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_flinch_hunched_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_flinch_back_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_flinch_bent_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_flinch_reel_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_flinch_hunched_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_flinch_back_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_flinch_bent_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_flinch_back_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_flinch_bent_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_flinch_reel_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_rocksteady_flinch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_rocksteady_reel(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_rocksteady_flinch_from_behind(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_rocksteady_knocked_down(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_rocksteady_defeated(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_bebop_flinch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_bebop_flinch_from_behind(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_bebop_knocked_down(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_bebop_defeated(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_mouser_thrown_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_mouser_break_down(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_mouser_knocked_flying(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_rodney_knocked_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_rodney_destroyed(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_shredder_lose_bar(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_shredder_flinch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_shredder_flinch_from_behind(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_shredder_defeated(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_shredder_vanish(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* the step over a floor edge its move picks, keeping track of its turtle and the floor */
void enemy_step_over_edge(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	states = TABLE32(enemy_edge_steps, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
	*p_slot = slot;
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
uint32_t enemy_hop_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_up(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
	animation = animations_04CF3E_ADDRESS;
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
uint32_t enemy_hop_up_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
uint32_t enemy_hop_up_fall_blocked(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_shuffle(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049240_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void unreached_0492EC(uint32_t object, uint32_t slot, uint32_t a5)
{
	uint32_t ignored = 0, ignored_2 = 0, states = 0;

	states = indexed_table_of_32_bit_values_0492F8_ADDRESS;
	run_object_state(object, slot, states, &ignored, &ignored_2, &a5);
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
void enemy_circle(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049350_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 180 frames of running round its turtle, facing it, one way round or the other at random */
void enemy_circle_start(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animations = 0, direction = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_circle.move_time = 0xB4u;
	animations = animations_049358_ADDRESS;
	direction = enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->enemy_circle.clockwise = 0u;
	if ((int8_t)(uint8_t)(random_seed >> 8u) >= 0)
	{
		enemy_circle_aim((uint8_t)direction, object, p_d7, p_a5);
		return;
	}
	OBJECT(object)->enemy_circle.clockwise = 1u;
	enemy_circle_aim((uint8_t)direction, object, p_d7, p_a5);
}

/* its speeds for running round its turtle the way it goes */
void enemy_circle_aim(uint8_t direction, uint32_t object, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint16_t speed = 0;
	uint32_t x_speed = 0;

	if (OBJECT(object)->enemy_circle.clockwise != 0u)
		direction += 0x50u;
	else
		direction -= 0x30u;
	speed = 0x180u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
}

/* a frame of the run round its turtle: the other way when it bumps into something, aiming again as its
   turtle moves */
void enemy_circle_step(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d7 = *p_d7;
	uint32_t animations = 0, d7_2 = 0, direction = 0;
	uint8_t t1 = 0;

	OBJECT(object)->enemy_circle.move_time--;
	d7_2 = (d7 & ~0xFFu) | OBJECT(object)->contacts;
	t1 = d7_2 & 0x1Eu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 != 0u)
	{
		OBJECT(object)->enemy_circle.clockwise ^= 1u;
		animations = animations_049358_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_aim((uint8_t)direction, object, &d7_2, p_a5);
		move_object_blocked(object);
		*p_d7 = d7_2;
		return;
	}
	if (OBJECT(object)->enemy_circle.target_direction != OBJECT(object)->enemy_circle.facing)
	{
		animations = animations_049358_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_aim((uint8_t)direction, object, &d7_2, p_a5);
	}
	move_object_blocked(object);
	*p_d7 = d7_2;
}

/* dashes across at its turtle and brakes past it */
void enemy_charge(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049414_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* a dash across toward its turtle at full speed, for at most 180 frames */
uint32_t enemy_charge_start(uint32_t object)
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
	return roll;
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
void enemy_crash_into_scenery(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0494D2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_05C9C4_ADDRESS;
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
void enemy_crash_into_scenery_low(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049568_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_05C9C4_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks to its turtle, aiming again every 8 frames */
void enemy_walk_to_target(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0495D6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk's speed and animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	animations = animations_049258_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_049258_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk to its turtle, aiming again every 8 frames, standing when close */
void enemy_walk_to_target_step(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_animations)
{
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_walk.move_time;
	OBJECT(object)->enemy_walk.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		turtle = PLAYER_SLOT(player_slot)->turtle;
		direction = direction_to_object(0u, object, turtle);
		*p_animations = OBJECT(object)->enemy_walk.animations;
		enemy_turn_to_object((uint8_t)direction, object, *p_animations, turtle);
		speed = OBJECT(object)->enemy_walk.speed;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, p_y_speed, p_animations);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_y_speed;
		OBJECT(object)->enemy_walk.move_time = 8u;
	}
	if (OBJECT(object)->enemy_walk.target_dx >= 0x20u || OBJECT(object)->enemy_walk.target_dy >= 0x20u)
		move_object_blocked(object);
}

/* walks to its turtle's side, now and then crouching, and keeps level with it */
void enemy_walk_to_side(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04965A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side, facing it */
uint32_t enemy_walk_to_side_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_049258_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk: across to its side or up or down to its turtle's row; level with it, the next
   state */
uint32_t enemy_walk_to_side_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
	return animations;
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
uint32_t enemy_walk_to_side_turn(uint32_t object, uint32_t player_slot, uint32_t animations)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->frame_time != 0u)
		return animations;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* level with its turtle: walks across until 0x10 away; off its row again, back to walking to its side */
uint32_t enemy_walk_to_side_hold(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	if (OBJECT(object)->enemy_walk.target_dx < 0x10u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0;
		if (OBJECT(object)->enemy_walk.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return animations;
	}
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->x_speed = OBJECT(object)->enemy_walk.speed;
	if ((int8_t)(target_direction - 0x40u) < 0)
	{
		if (OBJECT(object)->enemy_walk.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return animations;
	}
	OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	if (OBJECT(object)->enemy_walk.target_dy >= 3u)
		OBJECT(object)->state -= 3u;
	move_object_blocked(object);
	return animations;
}

/* keeps 0x50 to 0x70 away from its turtle, backing off when nearer and closing in when farther */
void enemy_keep_distance(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049796_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* faces its turtle, for up to 255 frames of keeping its distance */
void enemy_keep_distance_start(uint32_t object)
{
	uint32_t animations = 0;

	OBJECT(object)->enemy.move_time = 0xFFu;
	OBJECT(object)->state++;
	animations = animations_049358_ADDRESS;
	enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->frame_time = 0u;
}

/* stands facing its turtle; too near it backs off, too far it closes in */
void enemy_keep_distance_watch(uint32_t object, uint32_t player_slot, uint32_t *p_d7, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, animations_2 = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy.move_time--;
	if (OBJECT(object)->enemy.target_direction != OBJECT(object)->enemy.facing)
	{
		animations = animations_049358_ADDRESS;
		enemy_face_target_animation(0u, object, animations);
		OBJECT(object)->frame_time = 0u;
	}
	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
	{
		if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
			return;
		OBJECT(object)->state = 3u;
		direction = OBJECT(object)->enemy.target_direction;
		animations_2 = animations_049258_ADDRESS;
		turtle = PLAYER_SLOT(player_slot)->turtle;
		enemy_face_object(direction, object, animations_2, turtle);
		speed = 0x100u;
		speeds_from_direction(direction, speed, &x_speed, p_d7, p_sines);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_d7;
		return;
	}
	OBJECT(object)->state = 2u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	animations_2 = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations_2, turtle);
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_d7, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_d7;
}

/* walks away from its turtle until far enough, then watches again */
uint32_t enemy_keep_distance_back_off(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks toward its turtle until near enough, then watches again */
uint32_t enemy_keep_distance_close_in(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks away from its turtle for 40 frames, crouches, and waits while the turtle is down */
void enemy_back_off(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0498A6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 40 frames of walking backwards away from its turtle, still facing it */
void enemy_back_off_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x28u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
uint32_t enemy_back_off_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_wait(object, player_slot);
		return animations;
	}
	OBJECT(object)->state++;
	animation = enemy_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
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
void enemy_run_away(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049940_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 8 frames of running away from its turtle, still facing it */
void enemy_run_away_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_a5)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, ignored = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy.move_time = 8u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	speed = 0x200u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, &ignored);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, animations, turtle);
	*p_a5 = enemy_run_away_step(object, player_slot);
}

/* a frame of running away; at its end, back to its first approach */
uint32_t enemy_run_away_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy.approach = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks across toward its turtle for 120 frames */
void enemy_walk_across(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0499B0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 120 frames of walking across toward its turtle, facing it */
uint32_t enemy_walk_across_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0x78u;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of walking across, turned to its turtle */
uint32_t enemy_walk_across_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_049258_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* a jab, back to thinking when it ends */
void enemy_jab(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049A12_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_chop(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049A5E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_swing(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049A9A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_high_kick(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049AD6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_jump_kick(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049B12_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_jump_kick_leap(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_jump_kick_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a frame of the flying kick along its arc, following stairs; on landing a crouch */
uint32_t enemy_jump_kick_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_jump_kick_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a flying kick along a longer arc */
void enemy_long_jump_kick(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049BC8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_long_jump_kick_leap(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_long_jump_kick_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a frame of the long flying kick along its arc, following stairs; on landing a crouch */
uint32_t enemy_long_jump_kick_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_long_jump_kick_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_grab(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049C82_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* unless its turtle faces it, is down or is being grabbed: runs at it with its arms out for 60 frames */
void enemy_grab_start(uint32_t object, uint32_t player_slot, uint32_t *p_d7, uint32_t *p_a5)
{
	uint8_t attack = 0;
	uint16_t speed = 0;
	uint32_t animation = 0, direction = 0, turtle = 0, x_speed = 0;

	if ((int8_t)OBJECT(object)->enemy_grab.target_faces < 0 || OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine == 4u || PLAYER_SLOT(player_slot)->grab_time != 0u)
		enemy_back_to_thinking(object);
	else
	{
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
		speeds_from_direction((uint8_t)direction, speed, &x_speed, p_d7, p_a5);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_d7;
		OBJECT(object)->flags |= 4u;
		OBJECT(object)->state++;
		move_object_blocked(object);
	}
}

/* a frame of the run at its turtle; catching it, it hides in the hold; the turtle down or time up, it
   thinks again */
void enemy_grab_run(uint32_t object, uint32_t player_slot, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0;
	uint8_t time = 0;

	PLAYER_SLOT(player_slot)->grab_time = 0x60u;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_grab.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		enemy_leave_side(object, player_slot);
		OBJECT(object)->flags &= 0x73u;
		OBJECT(object)->state++;
		return;
	}
	if (OBJECT(PLAYER_SLOT(player_slot)->turtle)->routine != 4u)
	{
		time = OBJECT(object)->enemy_grab.time;
		OBJECT(object)->enemy_grab.time = time - 1u;
		if (time != 1u)
		{
			turtle = PLAYER_SLOT(player_slot)->turtle;
			direction = direction_to_object(0u, object, turtle);
			speed = 0x200u;
			speeds_from_direction((uint8_t)direction, speed, &x_speed, p_d7, p_a5);
			OBJECT(object)->x_speed = x_speed;
			OBJECT(object)->y_speed = *p_d7;
			move_object_blocked(object);
			return;
		}
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
void enemy_flinch_back(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049DE8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049E1C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_reel(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049E40_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_hunched(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049E64_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_killed_back(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049E88_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked flying on its back along its arc, its side by the turtle given up */
uint32_t enemy_killed_back_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_killed_back_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_knocked_back_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, player_slot);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the flight: against a wall a splat with its sound, else on landing it lies down */
uint32_t enemy_killed_back_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint8_t wall_mask = 0;
	uint32_t animation = 0, arc = 0, sound = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->state = 3u;
		sound = (d7 & ~0xFFu) | 0x89u;
		queue_sound((uint8_t)sound);
		animation = enemy_spin_animation_ADDRESS;
		set_animation(0u, object, animation);
		return sound;
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_killed_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
uint32_t enemy_killed_back_slide_down(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	OBJECT(object)->height -= 0x100u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->state++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_killed_back_slide_down_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_killed_forward(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_049FC4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked flying forward along its arc, its side by the turtle given up */
uint32_t enemy_killed_forward_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_killed_forward_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_knocked_forward_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, player_slot);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the flight: against a wall a splat with its sound, else on landing it lies down */
uint32_t enemy_killed_forward_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint8_t wall_mask = 0;
	uint32_t animation = 0, arc = 0, sound = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->state = 3u;
		sound = (d7 & ~0xFFu) | 0x89u;
		queue_sound((uint8_t)sound);
		animation = enemy_killed_forward_animation_ADDRESS;
		set_animation(0u, object, animation);
		return sound;
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_killed_forward_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_forward_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
uint32_t enemy_killed_forward_slide_down(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	OBJECT(object)->height -= 0x100u;
	enemy_landed(object, &d7, &zero);
	if (zero)
	{
		OBJECT(object)->state++;
		OBJECT(object)->contacts &= 0x7Fu;
		animation = enemy_killed_forward_slide_down_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	return d7;
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
void enemy_knocked_forward(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A100_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked down forward along its arc, its side by the turtle given up */
uint32_t enemy_knocked_forward_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_knocked_forward_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_knocked_forward_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, player_slot);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the fall; on landing it lies down */
uint32_t enemy_knocked_forward_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_knocked_forward_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_forward_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_thrown_by_donatello(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A1E0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* hidden in Donatello's hands for the 12 frames of his swing */
void enemy_thrown_by_donatello_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0xCu;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Donatello, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_donatello_swing(uint32_t object, uint32_t player_slot, uint32_t *p_damage, uint32_t *p_placement)
{
	uint32_t damage = *p_damage;
	uint32_t object_2 = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	*p_placement = thrown_by_donatello_placement_ADDRESS;
	enemy_place_by(object, *p_placement, object_2);
	damage = (damage & ~0xFFFFu) | 0x306u;
	enemy_take_damage((uint16_t)damage, object);
	OBJECT(object)->flags |= 0x80u;
	damage = enemy_knocked_back_start(damage, object, player_slot);
	*p_damage = damage;
}

/* a frame of the flight after a throw: against a wall a splat with its sound, else on landing it lies
   down */
uint32_t enemy_thrown_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint8_t wall_mask = 0;
	uint32_t animation = 0, arc = 0, sound = 0;

	wall_mask = 8u;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		wall_mask = 0x10u;
	if ((wall_mask & OBJECT(object)->contacts) != 0u)
	{
		OBJECT(object)->state = 4u;
		sound = (d7 & ~0xFFu) | 0x89u;
		queue_sound((uint8_t)sound);
		animation = enemy_spin_animation_ADDRESS;
		set_animation(0u, object, animation);
		return sound;
	}
	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_knocked_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
		t2 = (health_bars & ~0xFFu) | (uint8_t)damage;
		health_bars = (uint16_t)(t2 - damage);
		if (t2 < (uint16_t)damage)
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
uint32_t enemy_let_go_by_donatello(uint32_t object, uint32_t player_slot)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, player_slot);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	placement = record_04A332_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	return placement;
}

/* thrown by Michelangelo: hidden while he swings it, then placed by him, hurt and knocked flying on its
   back */
void enemy_thrown_by_michelangelo(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A374_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* hidden in Michelangelo's hands for the 18 frames of his swing */
void enemy_thrown_by_michelangelo_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0x12u;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Michelangelo, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_michelangelo_swing(uint32_t object, uint32_t player_slot, uint32_t *p_damage, uint32_t *p_placement)
{
	uint32_t damage = *p_damage;
	uint32_t object_2 = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	*p_placement = thrown_by_michelangelo_placement_ADDRESS;
	enemy_place_by(object, *p_placement, object_2);
	damage = (damage & ~0xFFFFu) | 0x306u;
	enemy_take_damage((uint16_t)damage, object);
	OBJECT(object)->flags |= 0x80u;
	damage = enemy_knocked_back_start(damage, object, player_slot);
	*p_damage = damage;
}

/* let go by Michelangelo: placed by him, shown, hittable and back to thinking */
uint32_t enemy_let_go_by_michelangelo(uint32_t object, uint32_t player_slot)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, player_slot);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	placement = record_04A3EE_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	return placement;
}

/* thrown by Raphael: hidden while he swings it, then placed by him, hurt and knocked flying on its back */
void enemy_thrown_by_raphael(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A432_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* hidden in Raphael's hands for the 16 frames of his swing */
void enemy_thrown_by_raphael_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Raphael, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_raphael_swing(uint32_t object, uint32_t player_slot, uint32_t *p_damage, uint32_t *p_placement)
{
	uint32_t damage = *p_damage;
	uint32_t object_2 = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	*p_placement = thrown_by_raphael_placement_ADDRESS;
	enemy_place_by(object, *p_placement, object_2);
	damage = (damage & ~0xFFFFu) | 0x306u;
	enemy_take_damage((uint16_t)damage, object);
	OBJECT(object)->flags |= 0x80u;
	damage = enemy_knocked_back_start(damage, object, player_slot);
	*p_damage = damage;
}

/* let go by Raphael: placed by him, shown, hittable and back to thinking */
uint32_t enemy_let_go_by_raphael(uint32_t object, uint32_t player_slot)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, player_slot);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	placement = record_04A4AC_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	return placement;
}

/* thrown by Leonardo: hidden while he swings it, then placed by him, hurt and knocked flying on its back */
void enemy_thrown_by_leonardo(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A4F0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* hidden in Leonardo's hands for the 20 frames of his swing */
void enemy_thrown_by_leonardo_start(uint32_t object)
{
	OBJECT(object)->flags &= 0x7Fu;
	OBJECT(object)->enemy.move_time = 0x14u;
	OBJECT(object)->state++;
}

/* when the swing ends: placed by Leonardo, hurt, shown again and knocked flying on its back */
void enemy_thrown_by_leonardo_swing(uint32_t object, uint32_t player_slot, uint32_t *p_damage, uint32_t *p_placement)
{
	uint32_t damage = *p_damage;
	uint32_t object_2 = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time != 1u)
		return;
	object_2 = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)OBJECT(object)->enemy.facing >= 0)
		OBJECT(object)->flags |= 0x40u;
	*p_placement = thrown_by_leonardo_placement_ADDRESS;
	enemy_place_by(object, *p_placement, object_2);
	damage = (damage & ~0xFFFFu) | 0x306u;
	enemy_take_damage((uint16_t)damage, object);
	OBJECT(object)->flags |= 0x80u;
	damage = enemy_knocked_back_start(damage, object, player_slot);
	*p_damage = damage;
}

/* let go by Leonardo: placed by him, shown, hittable and back to thinking */
uint32_t enemy_let_go_by_leonardo(uint32_t object, uint32_t player_slot)
{
	uint32_t object_2 = 0, placement = 0;

	OBJECT(object)->flags |= 0x82u;
	enemy_leave_side(object, player_slot);
	object_2 = 0x60000u | OBJECT(object)->attacker;
	placement = record_04A56A_ADDRESS;
	enemy_place_by(object, placement, object_2);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
	OBJECT(object)->enemy.move_time = 0u;
	OBJECT(object)->enemy.approach = 0u;
	return placement;
}

/* knocked down on its back: lies 30 frames and gets up, or dies with no bar left */
void enemy_knocked_back(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A5AE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked down backwards along its arc, its side by the turtle given up */
uint32_t enemy_knocked_back_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_knocked_back_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_knocked_back_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_leave_side(object, player_slot);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the fall; on landing it lies down */
uint32_t enemy_knocked_back_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_knocked_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knocked_back_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_die(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A6BE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_die_on_back(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04A710_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t boss_shutter_start(uint32_t object)
{
	uint16_t request = 0;
	uint32_t a5 = 0, animation = 0, data = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->routine++;
	request = 0x43Au;
	data = 0xC704u;
	a5 = post_palette_request(request, data);
	animation = animations_05CE06_ADDRESS;
	set_animation(0u, object, animation);
	return a5;
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
	animation = animations_04A7E6_ADDRESS;
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

	OBJECT(object)->scenery_open_manhole.hit_sound = 0xDCu;
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->attack = 0xCu;
	OBJECT(object)->damage = 0u;
	OBJECT(object)->routine++;
	animation = animations_04A81E_ADDRESS;
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
	animation = animations_04A86E_ADDRESS;
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
	OBJECT(object)->foot_car.hit_sound = 0xD4u;
	animation = animations_04A95C_ADDRESS;
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
	OBJECT(object)->falling_debris.hit_sound = 0x70u;
	animation = animations_04A9DA_ADDRESS;
	set_animation(0u, object, animation);
}

/* the debris falls when a turtle comes under it */
void falling_debris_wait(uint32_t object, uint32_t routine_entry, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t dx = 0, dy = 0;

	if (OBJECT(object)->screen_x >= 0x100u)
	{
		enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &routine_entry);
		enemy_look_at_target(object, routine_entry, &dx, &dy);
		if ((uint16_t)dx > 0x10u || (uint16_t)dy > 0x10u)
		{
			falling_debris_burn(object);
			return;
		}
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
	OBJECT(object)->motorbike.hit_sound = 0x44u;
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
	animation = animations_05C632_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	OBJECT(object)->motorbike.aim_time = 1u;
	OBJECT(object)->motorbike.wheelie = 0u;
}

/* the motorbike rides on and rears up at a turtle in its way */
void motorbike_ride(uint32_t object, uint32_t target_turtle, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0, dx = 0, dy = 0;
	uint8_t aim_time = 0;

	aim_time = OBJECT(object)->motorbike.aim_time;
	OBJECT(object)->motorbike.aim_time = aim_time - 1u;
	if (aim_time == 1u)
	{
		OBJECT(object)->motorbike.aim_time = 8u;
		enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &target_turtle);
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
uint32_t motorbike_struck(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, record = 0;

	sound = 0xA8u;
	queue_sound(sound);
	animation = motorbike_struck_animation_ADDRESS;
	record = motorbike_rider_record_ADDRESS;
	if (OBJECT(object)->motorbike.wheelie != 0u)
	{
		animation = motorbike_struck_wheelie_animation_ADDRESS;
		record = motorbike_wheelie_rider_record_ADDRESS;
	}
	set_animation(0u, object, animation);
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
	return record;
}

/* kind 01, the sword soldier: its turtle, health, colour, definition and shadow, straight to thinking */
void enemy_sword_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
	animation = animations_055722_ADDRESS;
	set_animation(0u, object, animation);
}

/* kind 02, the hammer soldier: its turtle, health, colour, definition and shadow, straight to thinking */
void enemy_hammer_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
	animation = animations_057B46_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks to its turtle with the sword */
void enemy_walk_to_target_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04ADB8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk with the sword's animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04ADC0_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04ADC0_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its side of its turtle with the sword */
void enemy_walk_to_side_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04ADFA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side with the sword, facing it */
uint32_t enemy_walk_to_side_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04ADC0_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
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
uint32_t enemy_walk_to_side_sword_turn(uint32_t object, uint32_t player_slot, uint32_t animations)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->frame_time != 0u)
		return animations;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks across past its turtle while off its row, then up or down to it */
void enemy_walk_past_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04AE84_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 180 frames of walking past its turtle with the sword, facing it */
uint32_t enemy_walk_past_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xB4u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
		OBJECT(object)->enemy_walk.speed = 0u - OBJECT(object)->enemy_walk.speed;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04ADC0_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk: across past its turtle while off its row, turning back beyond 0x40, else up or
   down to it */
uint32_t enemy_walk_past_sword_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
	return animations;
}

/* walks away with the sword, crouches and waits while its turtle is down */
void enemy_back_off_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04AF30_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 40 frames of walking backwards away from its turtle with the sword, still facing it */
void enemy_back_off_sword_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x28u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
uint32_t enemy_back_off_sword_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_sword_wait(object, player_slot);
		return animations;
	}
	OBJECT(object)->state++;
	animation = enemy_sword_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
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
void enemy_walk_to_line_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04AFCA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 16 frames of walking up or down toward its turtle's row, facing it */
uint32_t enemy_walk_to_line_sword_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0x10u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_face_object(target_direction, object, animations, turtle);
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x100;
	if ((int8_t)OBJECT(object)->enemy_walk.target_direction < 0)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	OBJECT(object)->state++;
	move_object_blocked(object);
	return animations;
}

/* a frame of the walk toward its turtle's row; at its end, back to its first approach */
uint32_t enemy_walk_to_line_sword_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;
	uint8_t move_time = 0;

	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04ADC0_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	move_time = OBJECT(object)->enemy_walk.move_time;
	OBJECT(object)->enemy_walk.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy_walk.approach = 0u;
	return animations;
}

/* walks to its turtle with the hammer */
void enemy_walk_to_target_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B03E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk with the hammer's animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04B046_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04B046_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its side of its turtle with the hammer, stopping 0x40 across */
void enemy_walk_to_side_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B080_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side with the hammer, facing it */
uint32_t enemy_walk_to_side_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04B046_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04B046_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk: across to 0x40 by its turtle or up or down to its row; level with it, the next
   state */
uint32_t enemy_walk_to_side_far_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
	return animations;
}

/* level with its turtle: walks across toward it; off its row again, back to walking to its side */
uint32_t enemy_walk_to_side_far_hold(uint32_t object, uint32_t player_slot)
{
	uint32_t a5 = 0;

	a5 = enemy_walk_to_side_hold(object, player_slot);
	if ((int8_t)OBJECT(object)->state < 0)
		OBJECT(object)->state = 1u;
	return a5;
}

/* the hammer soldier closes in across, then waits with the hammer up and steps to its turtle's row by
   turns */
void enemy_stalk_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B13A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* up to 255 frames of stalking its turtle with the hammer, facing it */
uint32_t enemy_stalk_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_stalk.move_time = 0xFFu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_stalk.turns = 0u;
	OBJECT(object)->enemy_stalk.time = 1u;
	target_direction = OBJECT(object)->enemy_stalk.target_direction;
	animations = animations_04B046_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks across toward its turtle until 0x40 away */
uint32_t enemy_stalk_hammer_approach(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	target_direction = OBJECT(object)->enemy_stalk.target_direction;
	animations = animations_04B046_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	if (OBJECT(object)->enemy_stalk.target_dx < 0x40u)
		OBJECT(object)->state++;
	OBJECT(object)->enemy_stalk.time = 1u;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_stalk.target_direction - 0x40u) >= 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	move_object_blocked(object);
	return animations;
}

/* by turns of 48 frames, stands with the hammer up or steps toward its turtle's row; too far, it closes
   in again */
uint32_t enemy_stalk_hammer_step(uint32_t object, uint32_t player_slot, uint32_t animations)
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
			animations = animations_04B046_ADDRESS;
			turtle = PLAYER_SLOT(player_slot)->turtle;
			return enemy_face_object(target_direction, object, animations, turtle);
		}
		return animations;
	}
	time = OBJECT(object)->enemy_stalk.time;
	OBJECT(object)->enemy_stalk.time = time - 1u;
	if (time != 1u)
	{
		if ((OBJECT(object)->enemy_stalk.turns & 1u) != 0u)
		{
			target_direction = OBJECT(object)->enemy_stalk.target_direction;
			animations = animations_04B046_ADDRESS;
			turtle = PLAYER_SLOT(player_slot)->turtle;
			animations = enemy_turn_to_object(target_direction, object, animations, turtle);
		}
		move_object_blocked(object);
		return animations;
	}
	OBJECT(object)->enemy_stalk.time = 0x30u;
	OBJECT(object)->enemy_stalk.turns++;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0;
	if ((OBJECT(object)->enemy_stalk.turns & 1u) == 0u)
	{
		animation = enemy_hammer_walk_animation_ADDRESS;
		set_animation(0u, object, animation);
		return animations;
	}
	OBJECT(object)->y_speed = 0x100;
	if ((int8_t)OBJECT(object)->enemy_stalk.target_direction < 0)
	{
		target_direction = OBJECT(object)->enemy_stalk.target_direction;
		animations = animations_04B046_ADDRESS;
		turtle = PLAYER_SLOT(player_slot)->turtle;
		return enemy_face_object(target_direction, object, animations, turtle);
	}
	OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	target_direction = OBJECT(object)->enemy_stalk.target_direction;
	animations = animations_04B046_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks away for 64 frames, then waits with the hammer up while its turtle is down */
void enemy_walk_away_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B244_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 64 frames of walking away from its turtle across, facing it */
uint32_t enemy_walk_away_hammer_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0x40u;
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) < 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04B046_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk away; at its end, the hammer up */
uint32_t enemy_walk_away_hammer_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t move_time = 0;

	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04B046_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	move_time = OBJECT(object)->enemy_walk.move_time;
	OBJECT(object)->enemy_walk.move_time = move_time - 1u;
	if (move_time != 1u)
		return animations;
	OBJECT(object)->enemy_walk.move_time = 0xFFu;
	OBJECT(object)->state++;
	animation = enemy_hammer_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
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
void enemy_slash_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B2E6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_low_slash_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B322_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_leaping_slash_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B35E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_leaping_slash_sword_leap(uint32_t d7, uint32_t object)
{
	uint16_t distance = 0;
	uint32_t animation = 0, arc = 0, x_speed = 0;
	uint16_t target_dx = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_leaping_slash_sword_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
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
	return d7;
}

/* a frame of the leap along its arc, following stairs; on landing its last pose */
uint32_t enemy_leaping_slash_sword_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_leaping_slash_sword_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_leaping_slash_sword_fly_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a jump slashing with the sword */
void enemy_jump_slash_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B436_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_jump_slash_sword_leap(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_jump_slash_sword_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_slash_sword_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a frame of the jump along its arc, following stairs; on landing the sword up again */
uint32_t enemy_jump_slash_sword_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_jump_slash_sword_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_sword_ready_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a long leap slashing, turning over in the air */
void enemy_flying_slash_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B4EC_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_flying_slash_sword_leap(uint32_t d7, uint32_t object)
{
	uint16_t distance = 0;
	uint32_t animation = 0, arc = 0, x_speed = 0;
	uint16_t target_dx = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_flying_slash_sword_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
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
	return d7;
}

/* a frame of the leap, turning over after 72 frames, following stairs; on landing a crouch */
uint32_t enemy_flying_slash_sword_fly(uint32_t d7, uint32_t object)
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
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_sword_crouch_walk_animation_ADDRESS;
	set_animation(ignored, object, animation);
	return d7;
}

/* a blow with the hammer, 58 frames */
void enemy_smash_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B5E2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_back_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B63E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B662_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_reel_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B686_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_hunched_sword(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B6AA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_back_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B6CE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B6F2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_hunched_hammer(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04B6CE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* Unused: a second start of the hammer soldier's flinch bent over */
void unreached_04B716(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	animation = enemy_flinch_bent_hammer_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* Rodney's whip arm, whose grab holds a turtle */
void whip_arm_start(uint16_t routine_offset, uint32_t object, uint32_t *p_d7, uint32_t *p_a1)
{
	OBJECT(object)->damage = 0u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->flags |= 0x86u;
	OBJECT(object)->attack = 0x8Fu;
	if ((OBJECT(object)->flags & 0x40u) == 0u)
		OBJECT(object)->attack &= 0x7Fu;
	OBJECT(object)->routine++;
	OBJECT(object)->whip_arm.unused_cleared = 0u;
	whip_arm_follow(routine_offset, object, p_d7, p_a1);
}

/* the arm reaches out, is drawn in or put away as Rodney does it */
void whip_arm_follow(uint32_t routine_offset, uint32_t object, uint32_t *p_d7, uint32_t *p_rodney)
{
	uint32_t animation_entry = 0, animations = 0, offsets = 0, rodney = 0;

	rodney = OBJECT(object)->whip_arm.rodney;
	if ((mem_read8(rodney + 0x4Au) & 0x80u) == 0u)
	{
		if ((mem_read8(rodney + 0x4Au) & 0x40u) == 0u)
		{
			if ((mem_read8(rodney + 0x4Au) & 0x20u) == 0u)
			{
				offsets = whip_arm_x_offsets_ADDRESS;
				whip_arm_place(object, rodney, offsets, &routine_offset, p_d7);
				animations = whip_arm_follow_animations_ADDRESS;
			}
			else
			{
				OBJECT(object)->routine = 3u;
				OBJECT(object)->flags &= ~0xEu;
				offsets = whip_arm_x_offsets_2_ADDRESS;
				whip_arm_place(object, rodney, offsets, &routine_offset, p_d7);
				animations = whip_arm_animations_2_ADDRESS;
			}
		}
		else
		{
			OBJECT(object)->flags &= ~0xEu;
			offsets = whip_arm_x_offsets_ADDRESS;
			whip_arm_place(object, rodney, offsets, &routine_offset, p_d7);
			animations = whip_arm_follow_animations_ADDRESS;
		}
		animation_entry = ANIMATION_ENTRY(animations + (IN_PAGE(routine_offset, mem_read8(rodney + 0x46u)) & 0x3FFFu) * 0x4u);
		set_animation(0u, object, animation_entry);
	}
	else
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	*p_rodney = rodney;
}

/* the turtle broke free of the arm */
void whip_arm_let_go(uint32_t object, uint32_t *p_d7, uint32_t *p_a1)
{
	uint32_t rodney = 0;

	rodney = OBJECT(object)->whip_arm.rodney;
	mem_write8(rodney + 0x4Au, mem_read8(rodney + 0x4Au) | 4u);
	OBJECT(object)->routine = 1u;
	whip_arm_drag(object, p_d7, p_a1);
}

/* the arm drags the turtle it caught */
void whip_arm_drag(uint32_t object, uint32_t *p_turtle, uint32_t *p_rodney)
{
	uint32_t rodney = 0, screen_y = 0, turtle_2 = 0, x_offset = 0;

	rodney = OBJECT(object)->whip_arm.rodney;
	*p_turtle = 0x60000u | OBJECT(object)->hit_object;
	turtle_2 = *p_turtle;
	if (mem_read8(rodney + 4u) != 5u)
	{
		screen_y = OBJECT(object)->screen_y;
		OBJECT(turtle_2)->y = (uint32_t)(uint16_t)(screen_y - 2u) << 8;
		x_offset = (uint16_t)whip_arm_offsets[mem_read8(rodney + 0x46u)];
		if ((OBJECT(object)->flags & 0x40u) != 0u)
			x_offset = (uint16_t)(0u - x_offset);
		OBJECT(turtle_2)->x = (uint32_t)(uint16_t)(x_offset + OBJECT(object)->screen_x) << 8;
	}
	else
	{
		OBJECT(turtle_2)->turtle.escape_presses = 0x10u;
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	*p_rodney = rodney;
}

/* a turtle's attack hits the arm, which lets go of a turtle it holds */
void whip_arm_struck(uint32_t routine_offset, uint32_t object, uint32_t *p_turtle, uint32_t *p_rodney)
{
	uint32_t animation = 0, offsets = 0, rodney = 0, turtle_2 = 0;

	if ((OBJECT(object)->flags & 8u) != 0u)
	{
		*p_turtle = 0x60000u | OBJECT(object)->hit_object;
		turtle_2 = *p_turtle;
		OBJECT(turtle_2)->turtle.escape_presses = 0x10u;
		rodney = OBJECT(object)->whip_arm.rodney;
		mem_write8(rodney + 0x4Au, mem_read8(rodney + 0x4Au) | 4u);
	}
	OBJECT(object)->flags &= ~0xEu;
	rodney = OBJECT(object)->whip_arm.rodney;
	mem_write8(rodney + 0x4Au, mem_read8(rodney + 0x4Au) | 2u);
	OBJECT(object)->routine = 1u;
	offsets = whip_arm_x_offsets_ADDRESS;
	whip_arm_place(object, rodney, offsets, &routine_offset, p_turtle);
	animation = whip_arm_follow_animations[IN_PAGE(routine_offset, mem_read8(rodney + 0x46u)) & 0x3FFFu];
	set_animation(0u, object, animation);
	*p_rodney = rodney;
}

/* the arm at Rodney's side, as long as Rodney stretches it */
void whip_arm_place(uint32_t object, uint32_t rodney, uint32_t offsets, uint32_t *p_step_offset, uint32_t *p_turtle)
{
	uint16_t step_offset = (uint16_t)*p_step_offset;
	uint16_t x_offset = 0;
	uint32_t turtle = 0;

	if (mem_read8(rodney + 4u) != 5u)
	{
		step_offset = (uint16_t)((uint32_t)mem_read8(rodney + 0x46u) << 1);
		x_offset = (uint16_t)WHIP_OFFSET(offsets + (step_offset / 2u) * 0x2u);
		if ((OBJECT(object)->flags & 0x40u) != 0u)
			x_offset = (uint16_t)(0u - x_offset);
		OBJECT(object)->x = (int16_t)x_offset + mem_read32(rodney + 0x12u);
		OBJECT(object)->y = mem_read32(rodney + 0x16u);
	}
	else
	{
		if ((OBJECT(object)->flags & 8u) != 0u)
		{
			*p_turtle = 0x60000u | OBJECT(object)->hit_object;
			turtle = *p_turtle;
			OBJECT(turtle)->turtle.escape_presses = 0x10u;
		}
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
	}
	*p_step_offset = (*p_step_offset & 0xFFFF0000u) | step_offset;
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
	OBJECT(object)->rolling.hit_sound = 0x44u;
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
	if (mem_read8(OBJECT(object)->boomerang.thrower + 4u) == 5u)
		OBJECT(object)->routine = 3u;
}

/* the boomerang flies back a while and is gone */
void boomerang_fly_back(uint32_t object)
{
	uint8_t flight_time = 0;

	if (mem_read8(OBJECT(object)->boomerang.thrower + 4u) != 5u)
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
void unreached_04BF5A(uint32_t object)
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
void rocksteady_bullet_start(uint32_t object, uint32_t *p_aim_number, uint32_t *p_height_speed, uint32_t *p_animations)
{
	uint8_t aim = 0;
	uint16_t aim_number = 0;
	uint32_t aims = 0, animation = 0, ignored = 0, speed = 0, x_speed = 0;

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
	speeds_from_direction(aim, (uint16_t)speed, &x_speed, p_height_speed, &ignored);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->height_speed = *p_height_speed;
	OBJECT(object)->y_speed = 0;
	aim_number = (uint16_t)((uint32_t)aim_number << 3);
	*p_animations = rocksteady_bullet_animations_ADDRESS;
	animation = rocksteady_bullet_animations[aim_number / 4u];
	OBJECT(object)->rocksteady_bullet.burst = TABLE32(rocksteady_bullet_animations, 0x4u + aim_number);
	set_animation(speed, object, animation);
	*p_aim_number = (*p_aim_number & 0xFFFF0000u) | aim_number;
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

/* Unused: Rocksteady's bullet bursts with a sound, no longer hitting or hittable */
void unreached_04C086(uint32_t object)
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
	OBJECT(object)->rolling.hit_sound = 0x44u;
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
uint32_t shredder_piece_bounce(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;
	uint8_t t1 = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
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
	return d7;
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
	animation = animations_05C220_ADDRESS;
	set_animation(0u, object, animation);
}

/* the ray lasts a second, less if Shredder is hit */
void shredder_ray_shine(uint32_t object)
{
	uint8_t time_left = 0;

	if (mem_read8(OBJECT(object)->shredder_ray.shredder + 4u) != 5u)
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
	OBJECT(object)->rocket.hit_sound = 0x70u;
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
uint32_t rocket_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x80u;
	enemy_landed(object, &d7, &zero);
	if (zero)
	{
		d7 = (d7 & ~0xFFu) | 0x13u;
		queue_sound((uint8_t)d7);
		animation = rocket_animations[0];
		set_animation(0u, object, animation);
		OBJECT(object)->damage = 0x303u;
		OBJECT(object)->attack |= 7u;
		OBJECT(object)->routine++;
	}
	return d7;
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
	OBJECT(object)->dynamite.hit_sound = 0x70u;
	OBJECT(object)->dynamite.bounces_left = 3u;
	OBJECT(object)->depth = 0u;
	OBJECT(object)->routine++;
}

/* the dynamite is thrown at the nearest turtle */
void dynamite_throw(uint32_t object, uint32_t routine_entry, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t speed = 0;
	uint32_t direction = 0, ignored = 0, x_speed = 0, y_speed = 0;

	enemy_nearest_turtles(object, p_d5, &ignored, p_a5, &routine_entry);
	direction = direction_to_object(0u, object, routine_entry);
	speed = 0x100u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, &y_speed, p_a5);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = y_speed;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->routine++;
	y_speed = dynamite_bounce(y_speed, object);
	*p_d7 = y_speed;
}

/* the dynamite bounces, flashing, and explodes on its last bounce */
uint32_t dynamite_bounce(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;
	uint8_t bounces_left = 0;

	OBJECT(object)->colour = dynamite_flash_colours[(uint8_t)frame_count & 4u];
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero)
	{
		OBJECT(object)->height_speed = (int16_t)(0u - OBJECT(object)->height_speed) >> 1;
		bounces_left = OBJECT(object)->dynamite.bounces_left;
		OBJECT(object)->dynamite.bounces_left = bounces_left - 1u;
		if (bounces_left == 1u)
		{
			d7 = (d7 & ~0xFFu) | 0x11u;
			queue_sound((uint8_t)d7);
			animation = animations_05CEEE_ADDRESS;
			set_animation(0u, object, animation);
			OBJECT(object)->colour = 0xFFu;
			OBJECT(object)->flags |= 4u;
			OBJECT(object)->routine++;
		}
	}
	return d7;
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
	animation = animations_04C410_ADDRESS;
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
uint32_t kicked_door_break(uint32_t object, uint32_t a5)
{
	uint32_t zero = 0;
	uint16_t count = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint16_t t1 = 0;

	if (OBJECT(object)->frame_time != 0u)
		return a5;
	count = 3u;
	record = record_04C4C6_ADDRESS;
	do
	{
		start_object_from_record(object, record, &ignored, &zero);
		t1 = count - 1u;
		count = t1;
		record += 8u;
	} while (t1 != 0xFFFFu);
	OBJECT(object)->routine++;
	animation = enemy_step_down_animation_ADDRESS;
	set_animation(0u, object, animation);
	return record;
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
void door_piece_start(uint32_t object, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t piece_offset = 0, speed = 0;
	uint32_t animation = 0, x_speed = 0;

	OBJECT(object)->flags = 0x81u;
	OBJECT(object)->contacts = 0u;
	piece_offset = (uint16_t)((uint32_t)(int8_t)OBJECT(object)->parameter << 2);
	animation = TABLE32(door_piece_animations, (uint32_t)(int16_t)piece_offset);
	set_animation(0u, object, animation);
	OBJECT(object)->height_speed = TABLE16(door_piece_speeds, 0x2u + (uint32_t)(int16_t)piece_offset);
	direction = (uint8_t)TABLE16(door_piece_speeds, (uint32_t)(int16_t)piece_offset);
	speed = 0x500u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
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
	animation = animations_04C65E_ADDRESS;
	set_animation(0u, object, animation);
}

/* the elevator waits until the scroll brings it to its place */
uint32_t elevator_wait_on_screen(uint32_t object)
{
	uint32_t stop = 0;

	stop = OBJECT(object)->elevator.stop;
	if (OBJECT(object)->screen_x <= mem_read16(stop))
	{
		OBJECT(object)->elevator.wait = mem_read8(stop + 2u);
		OBJECT(object)->routine++;
	}
	return stop;
}

/* the elevator's doors open and a Foot soldier comes out, two with three turtles or more in play */
uint32_t elevator_open(uint32_t object, uint32_t a5)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, record = 0;
	uint8_t wait = 0;

	wait = OBJECT(object)->elevator.wait;
	OBJECT(object)->elevator.wait = wait - 1u;
	if (wait != 1u)
		return a5;
	sound = 0x36u;
	queue_sound(sound);
	animation = elevator_doors_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->routine++;
	if (turtles_in_play < 3u)
	{
		record = elevator_door_record_2_ADDRESS;
		start_object_from_record(object, record, &ignored, &zero);
		return record;
	}
	record = record_04C6C8_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	record = elevator_door_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	return record;
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
	animation = animations_04C7B2_ADDRESS;
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
	animation = animations_04C7B2_ADDRESS;
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
	animation = animations_05C846_ADDRESS;
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
uint32_t tire_soldier_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->routine++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = tire_soldier_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* the soldier throws the tire */
uint32_t tire_soldier_throw(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, object_2 = 0, tire_record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return record;
	sound = 8u;
	queue_sound(sound);
	tire_record = thrown_tire_record_ADDRESS;
	start_object_from_record(object, tire_record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->parameter;
	OBJECT(object)->routine++;
	animation = enemy_throw_low_animation_ADDRESS;
	set_animation(0u, object, animation);
	return tire_record;
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
uint32_t manhole_soldier_jump_out(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0;

	if (OBJECT(object)->frame_time != 0u)
		return record;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x80;
	if ((OBJECT(object)->parameter & 0x40u) != 0u)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	OBJECT(object)->height_speed = 0x400;
	OBJECT(object)->routine++;
	record = manhole_soldier_hole_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	return record;
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
uint32_t manhole_soldier_throw(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, cover_record = 0, object_2 = 0;

	if (OBJECT(object)->frame_time != 0u)
		return record;
	sound = 8u;
	queue_sound(sound);
	cover_record = thrown_manhole_cover_record_ADDRESS;
	start_object_from_record(object, cover_record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->parameter;
	OBJECT(object)->routine++;
	animation = enemy_throw_low_animation_ADDRESS;
	set_animation(0u, object, animation);
	return cover_record;
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
uint32_t window_soldier_drop_start(uint32_t object)
{
	uint32_t blocks = 0;
	uint32_t break_time_halfword = 0;

	blocks = object_tile_blocks_ADDRESS + (int16_t)TABLE16(object_tile_block_offsets, (uint32_t)(int16_t)((uint32_t)(int8_t)OBJECT(object)->parameter << 1));
	OBJECT(object)->window_soldier_drop.trigger_x = mem_read16(blocks);
	blocks += 2u;
	break_time_halfword = mem_read16(blocks);
	OBJECT(object)->window_soldier_drop.break_time = (uint8_t)(break_time_halfword >> 8u);
	OBJECT(object)->window_soldier_drop.delay = (uint8_t)break_time_halfword;
	blocks += 2u;
	OBJECT(object)->window_soldier_drop.blocks = blocks;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine++;
	return blocks;
}

/* the soldier jumps out through the window and the glass shatters */
uint32_t window_soldier_drop_jump_out(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t animation = 0, ignored = 0, shards_record = 0;
	uint8_t delay = 0;

	if (OBJECT(object)->screen_x <= OBJECT(object)->window_soldier_drop.trigger_x)
	{
		delay = OBJECT(object)->window_soldier_drop.delay;
		OBJECT(object)->window_soldier_drop.delay = delay - 1u;
		if (delay != 1u)
			return record;
		OBJECT(object)->flags |= 0x80u;
		OBJECT(object)->contacts |= 0x80u;
		OBJECT(object)->height_speed = 0;
		OBJECT(object)->window_soldier_drop.break_time = 4u;
		shards_record = record_04CBAC_ADDRESS;
		start_object_from_record(object, shards_record, &ignored, &zero);
		OBJECT(object)->routine++;
		animation = animations_04CAFE_ADDRESS;
		set_animation(0u, object, animation);
		return shards_record;
	}
	return record;
}

/* the window is drawn broken */
uint32_t window_soldier_drop_break_window(uint32_t object, uint32_t a5)
{
	uint16_t block_number = 0;
	uint32_t block = 0, blocks = 0;
	uint8_t break_time = 0;

	break_time = OBJECT(object)->window_soldier_drop.break_time;
	OBJECT(object)->window_soldier_drop.break_time = break_time - 1u;
	if (break_time != 1u)
	{
		window_soldier_drop_fall(object);
		return a5;
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
	window_soldier_drop_fall(object);
	return blocks;
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
	OBJECT(object)->window_soldier_dive.trigger_y = mem_read16(pair);
	OBJECT(object)->window_soldier_dive.blocks = pair + 2u;
	OBJECT(object)->shadow = common_shadow_ADDRESS;
	OBJECT(object)->routine++;
}

/* the window is drawn broken and the soldier dives out through the glass */
uint32_t window_soldier_dive_wait(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint16_t other_place = 0, place = 0;
	uint32_t animation = 0, block = 0, blocks = 0, ignored = 0, other_block = 0, shards_record = 0;

	if ((int16_t)OBJECT(object)->screen_y < (int16_t)OBJECT(object)->window_soldier_dive.trigger_y)
		return record;
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
	shards_record = record_04CD3C_ADDRESS;
	start_object_from_record(object, shards_record, &ignored, &zero);
	animation = animations_04CD10_ADDRESS;
	set_animation(0u, object, animation);
	return shards_record;
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
	animation = animations_04CE5A_ADDRESS;
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
	animation = animations_0547C6_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks along the ledge; on the screen it crouches to jump down, or thinks up there if the nearest
   turtle is too */
void enemy_walk_in_on_ledge_walk(uint32_t object, uint32_t object_2, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	move_object_blocked(object);
	if (OBJECT(object)->screen_x >= 0x120u)
		return;
	enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &object_2);
	if (OBJECT(object_2)->floor_height != 0x50u)
	{
		OBJECT(object)->state++;
		animation = animations_04CF3E_ADDRESS;
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
uint32_t enemy_jump_down_fall_stairs(uint32_t d7, uint32_t object)
{
	enemy_follow_stairs_down(object);
	return enemy_jump_down_fall(d7, object);
}

/* a frame of the fall off the ledge; on landing a drop pose */
uint32_t enemy_jump_down_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	if (OBJECT(object)->height_speed < -0x4000)
		OBJECT(object)->height_speed = -0x400;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->state++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
	animation = animations_04CF3E_ADDRESS;
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
	animation = animations_04D0BC_ADDRESS;
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
uint32_t sewer_ripple_leap_out(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t animation = 0, object_2 = 0;

	if (OBJECT(object)->frame_time != 0u)
		return record;
	animation = sewer_ripple_leap_out_animation_ADDRESS;
	set_animation(0u, object, animation);
	record = record_04D1A6_ADDRESS;
	start_object_from_record(object, record, &object_2, &zero);
	if (zero)
	{
		OBJECT(object_2)->flags = OBJECT(object)->flags;
		OBJECT(object_2)->flags &= 0x41u;
		OBJECT(object)->routine++;
		return record;
	}
	OBJECT(object)->routine += 2u;
	return record;
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
	animation = animations_04D0BC_ADDRESS;
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
uint32_t enemy_leap_out_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	OBJECT(object)->floor_height = 0u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->state++;
	OBJECT(object)->contacts &= 0x7Fu;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void bomber_start(uint32_t object, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t x_speed = 0;

	direction = 0x10u;
	speed = 0x180u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
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
	animation = animations_04D35C_ADDRESS;
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
	animation = animations_05CA64_ADDRESS;
	set_animation(0u, object, animation);
}

/* the soldier throws the dynamite, then steps out */
uint32_t bomber_throw(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t animation = 0, ignored = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->bomber.timer;
	OBJECT(object)->bomber.timer = timer - 1u;
	if (timer == 1u)
	{
		record = record_04D466_ADDRESS;
		start_shot(object, record, &ignored, &zero);
	}
	if (OBJECT(object)->frame_time != 0u)
		return record;
	OBJECT(object)->routine++;
	animation = bomber_throw_animation_ADDRESS;
	set_animation(0u, object, animation);
	return record;
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
	animation = animations_05C92A_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->x_speed = -0x40;
	OBJECT(object)->enemy_rocket_soldier.aim_time = 1u;
	OBJECT(object)->routine++;
}

/* comes in, moving up or down to the nearest turtle's row; on the screen and level with it, aims */
void enemy_rocket_soldier_line_up(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0, ignored = 0, ignored_2 = 0, turtle = 0;
	uint8_t aim_time = 0;

	turtle = OBJECT(object)->enemy_rocket_soldier.turtle;
	aim_time = OBJECT(object)->enemy_rocket_soldier.aim_time;
	OBJECT(object)->enemy_rocket_soldier.aim_time = aim_time - 1u;
	if (aim_time == 1u)
	{
		OBJECT(object)->enemy_rocket_soldier.aim_time = 8u;
		enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &turtle);
		OBJECT(object)->enemy_rocket_soldier.turtle = turtle;
	}
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->y_speed = 0x80;
	if ((int8_t)OBJECT(object)->enemy_rocket_soldier.target_direction >= 0)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	move_object_blocked(object);
	if (OBJECT(object)->screen_x < 0x170u && OBJECT(object)->enemy_rocket_soldier.target_dy < 0x10u)
	{
		OBJECT(object)->routine++;
		animation = enemy_rocket_soldier_line_up_animation_ADDRESS;
		set_animation(ignored, object, animation);
	}
}

/* when the aim ends, fires the rocket with its sound; it can no longer be hit */
uint32_t enemy_rocket_soldier_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, rocket_record = 0;

	if (OBJECT(object)->frame_time != 0u)
		return record;
	OBJECT(object)->flags &= ~2u;
	sound = 8u;
	queue_sound(sound);
	rocket_record = rocket_shot_record_ADDRESS;
	start_shot(object, rocket_record, &ignored, &zero);
	OBJECT(object)->routine++;
	animation = enemy_throw_low_animation_ADDRESS;
	set_animation(0u, object, animation);
	return rocket_record;
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
void enemy_rocket_soldier_struck(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1, uint32_t *p_record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, object_2 = 0;
	uint8_t t1 = 0;

	*p_record = dropped_rocket_shot_record_ADDRESS;
	start_shot(object, *p_record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = 1u;
	t1 = enemies_on_screen - 1u;
	enemies_on_screen = t1;
	if ((int8_t)t1 < 0)
		enemies_on_screen = 0u;
	enemy_foot_soldier_start(object, object_2, p_d5, &ignored, p_record);
	OBJECT(object)->kind = 0u;
	OBJECT(object)->parameter = 0u;
	OBJECT(object)->routine = 4u;
	OBJECT(object)->state = 0u;
	if ((int8_t)mem_read8(OBJECT(object)->enemy_rocket_soldier.definition + 0x58u + (OBJECT(object)->hit & 0x7Fu)) < 0)
	{
		OBJECT(object)->health_bars = 0u;
		OBJECT(object)->health = 0u;
	}
	enemy_hit(object, p_d5, p_d7, p_a1, p_record);
}

/* two closed doors */
void double_door_start(uint32_t object)
{
	OBJECT(object)->routine++;
}

/* the doors swing open and two Foot soldiers come out */
uint32_t double_door_wait(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint16_t place = 0;
	uint32_t animation = 0, block = 0, ignored = 0;

	if (OBJECT(object)->screen_x >= 0x100u)
		return record;
	OBJECT(object)->flags |= 0x80u;
	animation = animations_05CDA0_ADDRESS;
	set_animation(0u, object, animation);
	sound = 0x3Fu;
	queue_sound(sound);
	place = 0x22u;
	block = double_door_kicked_tile_block_ADDRESS;
	draw_tile_block(place, block);
	OBJECT(object)->routine++;
	record = double_door_piece_record_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	record = double_door_piece_record_2_ADDRESS;
	start_object_from_record(object, record, &ignored, &zero);
	return record;
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
	animation = animations_05CE9E_ADDRESS;
	set_animation(0u, object, animation);
}

/* on the screen, the door opens and the first Foot soldier comes out */
uint32_t technodrome_door_wait(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0, soldier_record = 0;

	if (OBJECT(object)->screen_x >= 0x140u)
		return record;
	OBJECT(object)->routine++;
	soldier_record = technodrome_door_soldier_record_ADDRESS;
	start_object_from_record(object, soldier_record, &ignored, &zero);
	sound = 0x36u;
	queue_sound(sound);
	OBJECT(object)->technodrome_door.spawn_time = 0x18u;
	OBJECT(object)->technodrome_door.soldiers_left = technodrome_door_soldiers[difficulty & 7u];
	animation = technodrome_door_animation_ADDRESS;
	set_animation(0u, object, animation);
	return soldier_record;
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
uint32_t technodrome_door_let_out(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0;
	uint8_t spawn_time = 0, soldiers_left = 0;

	spawn_time = OBJECT(object)->technodrome_door.spawn_time;
	OBJECT(object)->technodrome_door.spawn_time = spawn_time - 1u;
	if (spawn_time == 1u)
	{
		OBJECT(object)->technodrome_door.spawn_time = 0x18u;
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
	return record;
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
	animation = animations_05C52C_ADDRESS;
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
uint32_t enemy_hang_in_drop(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	OBJECT(object)->floor_height = 0u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->state++;
	animation = enemy_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
uint32_t enemy_kick_in_leap(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_kick_in_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a frame of the flying kick; on landing a crouch, no longer attacking */
uint32_t enemy_kick_in_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_kick_in_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_dive_in_run(uint32_t object, uint32_t object_2, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	if (OBJECT(object)->screen_x >= 0x120u)
	{
		move_object_blocked(object);
		return;
	}
	enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &object_2);
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
		animation = animations_05C566_ADDRESS;
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
uint32_t enemy_dive_in_land(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x38u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
	animation = animations_0547C6_ADDRESS;
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
	animation = animations_05C5A4_ADDRESS;
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
void enemy_knife_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
	animation = animations_056F42_ADDRESS;
	set_animation(0u, object, animation);
}

/* a Foot soldier with a spear, and the hits it lands before it throws the spear */
void enemy_spear_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
void enemy_walk_to_target_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DD40_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk with the spear's animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DD48_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04DD48_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its side of its turtle with the spear */
void enemy_walk_to_side_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DD82_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side with the spear, facing it */
uint32_t enemy_walk_to_side_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04DD48_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04DD48_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* keeps off its turtle's line, crossing the screen */
void enemy_walk_off_line_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DDC0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 180 frames of walking off its turtle's row with the spear, facing it */
uint32_t enemy_walk_off_line_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xB4u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	if ((int8_t)(OBJECT(object)->enemy_walk.target_direction - 0x40u) >= 0)
		OBJECT(object)->enemy_walk.speed = 0u - OBJECT(object)->enemy_walk.speed;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DD48_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk: across past its turtle while off its row, turning back beyond 0x40, else up or
   down */
uint32_t enemy_walk_off_line_spear_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04DD48_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
	return animations;
}

/* walks away, crouches and waits while its turtle is down */
void enemy_back_off_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DE6C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 32 frames of walking away across with the spear, facing its turtle */
uint32_t enemy_back_off_spear_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_back_off.move_time = 0xFFu;
	OBJECT(object)->enemy_back_off.time = 0x20u;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(OBJECT(object)->enemy_back_off.target_direction - 0x40u) < 0)
		OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DD48_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk away; at its end, a crouch */
uint32_t enemy_back_off_spear_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DD48_ADDRESS;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		move_object_blocked(object);
		return animations;
	}
	OBJECT(object)->state++;
	animation = enemy_back_off_spear_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
}

/* steps up or down out of the way of its turtle's attack */
void enemy_step_aside_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DEEE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 16 frames of stepping up or down and away out of its turtle's attack, facing it */
void enemy_step_aside_spear_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_animations)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, ignored = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy.move_time = 0x10u;
	OBJECT(object)->state++;
	direction = 0x60u;
	if ((int8_t)OBJECT(object)->enemy.target_direction >= 0)
		direction = (uint8_t)(0u - direction);
	if ((int8_t)(OBJECT(object)->enemy.target_direction - 0x40u) < 0)
		direction ^= 0x40u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, &ignored);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	target_direction = OBJECT(object)->enemy.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DD48_ADDRESS;
	animations = enemy_face_object(target_direction, object, animations, turtle);
	*p_animations = animations;
}

/* a frame of the step aside; at its end, back to its first approach */
uint32_t enemy_step_aside_spear_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy.approach = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DD48_ADDRESS;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks to its turtle with the knife */
void enemy_walk_to_target_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DF70_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk with the knife's animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04DF78_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04DF78_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its side of its turtle with the knife */
void enemy_walk_to_side_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DFB2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side with the knife, facing it */
uint32_t enemy_walk_to_side_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04DF78_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* runs round its turtle with the knife */
void enemy_circle_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04DFF0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 180 frames of running round its turtle with the knife, one way round or the other at random */
void enemy_circle_knife_start(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animations = 0, direction = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_circle.move_time = 0xB4u;
	animations = animations_04DFF8_ADDRESS;
	direction = enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->enemy_circle.clockwise = 0u;
	if ((int8_t)(uint8_t)(random_seed >> 8u) >= 0)
	{
		enemy_circle_knife_aim((uint8_t)direction, object, p_d7, p_a5);
		return;
	}
	OBJECT(object)->enemy_circle.clockwise = 1u;
	enemy_circle_knife_aim((uint8_t)direction, object, p_d7, p_a5);
}

/* its speeds for running round its turtle with the knife the way it goes */
void enemy_circle_knife_aim(uint8_t direction, uint32_t object, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint16_t speed = 0;
	uint32_t x_speed = 0;

	if (OBJECT(object)->enemy_circle.clockwise != 0u)
		direction += 0x50u;
	else
		direction -= 0x30u;
	speed = 0x140u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
}

/* a frame of the run round its turtle: the other way when it bumps into something, aiming again as its
   turtle moves */
void enemy_circle_knife_step(uint32_t object, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t d7 = *p_d7;
	uint32_t animations = 0, d7_2 = 0, direction = 0;
	uint8_t t1 = 0;

	OBJECT(object)->enemy_circle.move_time--;
	d7_2 = (d7 & ~0xFFu) | OBJECT(object)->contacts;
	t1 = d7_2 & 0x1Eu;
	d7_2 = (d7_2 & ~0xFFu) | t1;
	if (t1 != 0u)
	{
		OBJECT(object)->enemy_circle.clockwise ^= 1u;
		animations = animations_04DFF8_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_knife_aim((uint8_t)direction, object, &d7_2, p_a5);
		move_object_blocked(object);
		*p_d7 = d7_2;
		return;
	}
	if (OBJECT(object)->enemy_circle.target_direction != OBJECT(object)->enemy_circle.facing)
	{
		animations = animations_04DFF8_ADDRESS;
		direction = enemy_face_target_animation(0u, object, animations);
		enemy_circle_knife_aim((uint8_t)direction, object, &d7_2, p_a5);
	}
	move_object_blocked(object);
	*p_d7 = d7_2;
}

/* keeps its distance from its turtle with the knife */
void enemy_keep_distance_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E0B4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* faces its turtle with the knife, for up to 255 frames of keeping its distance */
void enemy_keep_distance_knife_start(uint32_t object)
{
	uint32_t animations = 0;

	OBJECT(object)->enemy.move_time = 0xFFu;
	OBJECT(object)->state++;
	animations = animations_04DFF8_ADDRESS;
	enemy_face_target_animation(0u, object, animations);
	OBJECT(object)->frame_time = 0u;
}

/* stands facing its turtle with the knife; too near it backs off, too far it closes in */
void enemy_keep_distance_knife_watch(uint32_t object, uint32_t player_slot, uint32_t *p_d7, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, animations_2 = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy.move_time--;
	if (OBJECT(object)->enemy.target_direction != OBJECT(object)->enemy.facing)
	{
		animations = animations_04DFF8_ADDRESS;
		enemy_face_target_animation(0u, object, animations);
		OBJECT(object)->frame_time = 0u;
	}
	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
	{
		if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
			return;
		OBJECT(object)->state = 3u;
		direction = OBJECT(object)->enemy.target_direction;
		animations_2 = animations_04DF78_ADDRESS;
		turtle = PLAYER_SLOT(player_slot)->turtle;
		enemy_face_object(direction, object, animations_2, turtle);
		speed = 0x100u;
		speeds_from_direction(direction, speed, &x_speed, p_d7, p_sines);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_d7;
		return;
	}
	OBJECT(object)->state = 2u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	animations_2 = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations_2, turtle);
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_d7, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_d7;
}

/* walks away from its turtle with the knife until far enough, then watches again */
uint32_t enemy_keep_distance_knife_back_off(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	if (OBJECT(object)->enemy.target_dx >= 0x50u || OBJECT(object)->enemy.target_dy >= 0x20u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks toward its turtle with the knife until near enough, then watches again */
uint32_t enemy_keep_distance_knife_close_in(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	if (OBJECT(object)->enemy.target_dx < 0x70u && OBJECT(object)->enemy.target_dy < 0x40u)
		OBJECT(object)->state = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks away, turns and waits while its turtle is down */
void enemy_back_off_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E1C4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 30 frames of walking backwards away from its turtle with the knife, still facing it */
void enemy_back_off_knife_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x1Eu;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations, turtle);
	direction -= 0x80u;
	speed = 0x140u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; at its end it turns to the turtle */
uint32_t enemy_back_off_knife_walk(uint32_t object, uint32_t player_slot)
{
	uint32_t animations = 0, direction = 0, turtle = 0, walk_animations = 0;
	uint8_t time = 0;

	direction = OBJECT(object)->enemy_back_off.target_direction;
	walk_animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	walk_animations = enemy_turn_to_object((uint8_t)direction, object, walk_animations, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_knife_wait(object, player_slot);
		return walk_animations;
	}
	animations = animations_04DFF8_ADDRESS;
	enemy_face_target_animation(direction, object, animations);
	OBJECT(object)->frame_time = 0u;
	OBJECT(object)->state++;
	enemy_back_off_knife_wait(object, player_slot);
	return walk_animations;
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
void enemy_step_aside_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E260_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 8 frames of stepping up or down out of its turtle's attack, facing it */
uint32_t enemy_step_aside_knife_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = 0;
	OBJECT(object)->y_speed = 0x140;
	if ((int8_t)OBJECT(object)->enemy.target_direction < 0)
		OBJECT(object)->y_speed = 0u - OBJECT(object)->y_speed;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_face_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* a frame of the step aside; at its end, back to its first approach */
uint32_t enemy_step_aside_knife_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
		OBJECT(object)->enemy.approach = 0u;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04DF78_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* thrusts the spear; enough hits and it throws it next */
void enemy_spear_thrust(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E2D0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_spear_throw(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E342_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_spear_throw_release(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_spear.move_time;
	OBJECT(object)->enemy_spear.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		record = spear_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		OBJECT(object)->flags &= ~2u;
		OBJECT(object)->state++;
	}
	return record;
}

/* when the throw ends, it starts again as a bare-handed soldier, thinking */
void enemy_spear_throw_end(uint32_t object, uint32_t player_slot, uint32_t player_slot_2, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemy_leave_side(object, player_slot);
	PLAYER_SLOT(player_slot)->enemies--;
	OBJECT(object)->kind = 0u;
	OBJECT(object)->parameter = 0u;
	enemy_foot_soldier_setup(object, player_slot_2, p_d5, p_d7, p_a5);
	OBJECT(object)->routine = 2u;
	OBJECT(object)->state = 0u;
}

/* stabs with the knife */
void enemy_knife_stab(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E3D0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_high_kick_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E40C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_jump_kick_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E448_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_jump_kick_knife_leap(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	if (OBJECT(object)->frame_time != 0u)
		return d7;
	arc = enemy_jump_kick_knife_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_jump_kick_knife_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a frame of the flying kick along its arc, following stairs; on landing a crouch */
uint32_t enemy_jump_kick_knife_fly(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	enemy_follow_stairs(object);
	move_object_with_height_blocked(object);
	arc = enemy_jump_kick_knife_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_knife_crouch_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* throws a knife */
void enemy_throw_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E4FE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_throw_knife_release(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0;
	uint8_t throw_time = 0;

	throw_time = OBJECT(object)->enemy_throw.throw_time;
	OBJECT(object)->enemy_throw.throw_time = throw_time - 1u;
	if (throw_time != 1u)
		return record;
	OBJECT(object)->state++;
	record = knife_shot_record_ADDRESS;
	start_shot(object, record, &ignored, &zero);
	return record;
}

/* back to thinking when the throw's animation ends */
void enemy_throw_knife_end(uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return;
	enemy_back_to_thinking(object);
}

/* hit: its head thrown back */
void enemy_flinch_back_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E572_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E596_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_reel_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E5BA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_hunched_spear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E5DE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_back_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E602_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E626_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_reel_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E64A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_hunched_knife(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E66E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_axe_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
void enemy_gun_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
void enemy_walk_to_target_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E7BA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk with the gun's animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x120u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04E7C2_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04E7C2_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its side of its turtle with the gun */
void enemy_walk_to_side_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E800_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side with the gun, facing it */
uint32_t enemy_walk_to_side_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04E7C2_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04E7C2_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its turtle's row at shooting distance and stands there */
void enemy_walk_to_range_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E83E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to shooting distance from its turtle, facing it */
uint32_t enemy_walk_to_range_gun_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04E7C2_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04E7C2_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its turtle's row at 0x5A across, then stands in its ready pose */
uint32_t enemy_walk_to_range_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
		return animations;
	}
	OBJECT(object)->state++;
	animations = OBJECT(object)->enemy_walk.animations;
	animation = mem_read32(animations + 8u);
	set_animation(0u, object, animation);
	return animations;
}

/* stands ready while level with its turtle, walks again when off its row */
uint32_t enemy_walk_to_range_hold(uint32_t object, uint32_t player_slot, uint32_t animations)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	OBJECT(object)->enemy_walk.move_time--;
	if (OBJECT(object)->enemy_walk.target_dy < 3u)
		return animations;
	OBJECT(object)->state--;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = OBJECT(object)->enemy_walk.animations;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks away, crouches and waits while its turtle is down */
void enemy_back_off_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E906_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 32 frames of walking backwards away from its turtle with the gun, still facing it */
void enemy_back_off_gun_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x20u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04E7C2_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
uint32_t enemy_back_off_gun_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04E7C2_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_gun_wait(object, player_slot);
		return animations;
	}
	OBJECT(object)->state++;
	animation = enemy_gun_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
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
void enemy_run_away_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04E99E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 8 frames of running straight away from its turtle with the gun, still facing it */
void enemy_run_away_gun_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_a5)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, ignored = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy.move_time = 8u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, &ignored);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04E7C2_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, animations, turtle);
	*p_a5 = enemy_run_away_gun_step(object, player_slot);
}

/* a frame of running away; at its end, back to its first approach and a new move */
uint32_t enemy_run_away_gun_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->enemy.approach = 0u;
		OBJECT(object)->enemy.move_time = 0u;
	}
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04E7C2_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* walks to its turtle with the axes */
void enemy_walk_to_target_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EA12_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk with the axes' animations, facing its turtle; it aims again in 8 frames */
uint32_t enemy_walk_to_target_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 8u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = animations_04EA1A_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04EA1A_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its side of its turtle with the axes */
void enemy_walk_to_side_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EA58_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to its turtle's side with the axes, facing it */
uint32_t enemy_walk_to_side_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x140u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04EA1A_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04EA1A_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks to its turtle's row at throwing distance and stands there */
void enemy_walk_to_range_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EA96_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to throwing distance from its turtle, facing it */
uint32_t enemy_walk_to_range_axe_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_walk.move_time = 0xF0u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_walk.speed = 0x100u;
	target_direction = OBJECT(object)->enemy_walk.target_direction;
	animations = animations_04EA1A_ADDRESS;
	OBJECT(object)->enemy_walk.animations = animations_04EA1A_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* walks away, crouches and waits while its turtle is down */
void enemy_back_off_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EAD4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 40 frames of walking backwards away from its turtle with the axes, still facing it */
void enemy_back_off_axe_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_back_off.move_time = 0x40u;
	OBJECT(object)->enemy_back_off.time = 0x28u;
	direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04EA1A_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while its turtle is down; when the time runs out, a crouch */
uint32_t enemy_back_off_axe_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t time = 0;

	target_direction = OBJECT(object)->enemy_back_off.target_direction;
	animations = animations_04EA1A_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	time = OBJECT(object)->enemy_back_off.time;
	OBJECT(object)->enemy_back_off.time = time - 1u;
	if (time != 1u)
	{
		enemy_back_off_axe_wait(object, player_slot);
		return animations;
	}
	OBJECT(object)->state++;
	animation = enemy_axe_crouch_walk_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
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
void enemy_run_away_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EB6E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 8 frames of running straight away from its turtle with the axes, still facing it */
void enemy_run_away_axe_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_a5)
{
	uint8_t direction = 0, target_direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, ignored = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy.move_time = 8u;
	direction = (uint8_t)(OBJECT(object)->enemy.target_direction - 0x80u);
	speed = 0x180u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, &ignored);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04EA1A_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(target_direction, object, animations, turtle);
	*p_a5 = enemy_run_away_axe_step(object, player_slot);
}

/* a frame of running away; at its end, back to its first approach and a new move */
uint32_t enemy_run_away_axe_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy.move_time;
	OBJECT(object)->enemy.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		OBJECT(object)->enemy.approach = 0u;
		OBJECT(object)->enemy.move_time = 0u;
	}
	target_direction = OBJECT(object)->enemy.target_direction;
	animations = animations_04EA1A_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	return animations;
}

/* throws three axes */
void enemy_throw_axes(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EBE2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_throw_axes_release(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0;
	uint8_t throw_time = 0, throws_left = 0;

	throw_time = OBJECT(object)->enemy_throw.throw_time;
	OBJECT(object)->enemy_throw.throw_time = throw_time - 1u;
	if (throw_time == 1u)
	{
		sound = 2u;
		queue_sound(sound);
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
			return record;
		}
	}
	return record;
}

/* a high kick */
void enemy_high_kick_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EC6A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_gun_swing(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04ECA6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_shoot_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04ECE2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_shoot_gun_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ignored = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_shoot.shot_time;
	OBJECT(object)->enemy_shoot.shot_time = shot_time - 1u;
	if (shot_time == 1u)
	{
		sound = 0x18u;
		queue_sound(sound);
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
			return record;
		}
	}
	return record;
}

/* hit: the gun soldier flinches with its head thrown back */
void enemy_flinch_back_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04ED6A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04ED8E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_back_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EDB2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_bent_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EDD6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_flinch_reel_axe(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04EDFA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_boomerang_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
	animation = animations_0547C6_ADDRESS;
	set_animation(0u, object, animation);
}

/* a bare-handed Foot soldier that also throws shuriken */
void enemy_shuriken_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
	animation = animations_0547C6_ADDRESS;
	set_animation(0u, object, animation);
}

/* a gun, axe or knife soldier on a rocket skateboard, by its parameter */
void enemy_skateboard_soldier_start(uint32_t object, uint32_t player_slot, uint32_t *p_next_distance, uint32_t *p_count, uint32_t *p_next_closest)
{
	uint16_t variant_offset = 0;
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_next_distance, p_count, p_next_closest, &player_slot);
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
void enemy_skateboard_think(uint32_t object, uint32_t second_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	enemy_think(object, second_slot, p_d5, p_d7, p_slot, p_a5);
	OBJECT(object)->x += 0x600u;
	enemy_skateboard_stand(object);
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
	else
		OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* the attack, carried along with the ride */
void enemy_skateboard_attack(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	enemy_attack(object, p_d5, p_d7, p_slot, p_a5);
	OBJECT(object)->x += 0x600u;
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
	else
		OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* the hit, carried along with the ride */
void enemy_skateboard_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1, uint32_t *p_a5)
{
	enemy_hit(object, p_d5, p_d7, p_a1, p_a5);
	OBJECT(object)->x += 0x600u;
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
	else
		OBJECT(object)->shadow = skateboard_shadow_2_ADDRESS;
}

/* the reaction to a hit, carried along with the ride */
void enemy_skateboard_react(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	OBJECT(object)->x += 0x600u;
	enemy_react(object, p_d5, p_d7, p_slot, p_a5);
	OBJECT(object)->contacts |= 0x80u;
	if ((frame_count & 4u) == 0u)
		OBJECT(object)->shadow = skateboard_shadow_ADDRESS;
	else
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
void enemy_throw_shuriken(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F082_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_throw_shuriken_release(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t ignored = 0;
	uint8_t throw_time = 0;

	throw_time = OBJECT(object)->enemy_throw.throw_time;
	OBJECT(object)->enemy_throw.throw_time = throw_time - 1u;
	if (throw_time != 1u)
		return record;
	OBJECT(object)->state++;
	sound = 0x2Au;
	queue_sound(sound);
	record = shuriken_shot_record_ADDRESS;
	start_shot(object, record, &ignored, &zero);
	return record;
}

/* throws a boomerang and waits for it to come back */
void enemy_boomerang_throw(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F0F0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_boomerang_throw_release(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t boomerang = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_boomerang.move_time;
	OBJECT(object)->enemy_boomerang.move_time = move_time - 1u;
	if (move_time != 1u)
		return record;
	sound = 0x2Au;
	queue_sound(sound);
	record = boomerang_shot_record_ADDRESS;
	start_shot(object, record, &boomerang, &zero);
	if (zero)
	{
		OBJECT(boomerang)->boomerang.thrower = object;
		OBJECT(object)->enemy_boomerang.boomerang = boomerang;
	}
	OBJECT(object)->enemy_boomerang.move_time = 0x3Cu;
	OBJECT(object)->state++;
	return record;
}

/* waits for its boomerang: back to thinking if a turtle breaks it, a pose after 60 frames */
void enemy_boomerang_throw_wait(uint32_t object)
{
	uint32_t animation = 0;
	uint8_t move_time = 0;

	if (mem_read8(OBJECT(object)->enemy_boomerang.boomerang + 4u) == 4u)
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
void enemy_rocksteady_start(uint32_t object, uint32_t target, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &target);
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
	OBJECT(object)->shadow = sprite_lists_059A18_ADDRESS;
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
	*p_d7 = (*p_d7 & ~0xFFFFu) | 0xFEu;
	music = *p_d7;
	queue_sound((uint8_t)*p_d7);
	OBJECT(object)->enemy_rocksteady.timer = 0xAu;
	OBJECT(object)->routine++;
	OBJECT(object)->flags |= 0x80u;
	animation = enemy_rocksteady_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* his way in: a laugh and his taunt, or waiting for Bebop, then walking in */
void enemy_rocksteady_enter(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F296_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_059CB0_ADDRESS;
	set_animation(0u, object, animation);
}

/* after the laugh, the boss music and his taunt in a balloon, on his row, laughing on */
void enemy_rocksteady_enter_taunt(uint16_t state_offset, uint32_t object, uint32_t *p_d5, uint32_t *p_sound)
{
	uint32_t d5 = *p_d5;
	uint32_t sound = *p_sound;
	uint16_t on_right = 0, screen_y = 0;
	uint32_t animation = 0, screen_x = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_rocksteady.timer;
	OBJECT(object)->enemy_rocksteady.timer = timer - 1u;
	if (timer != 1u)
		return;
	sound = (sound & ~0xFFFFu) | 0x64u;
	music = sound;
	queue_sound((uint8_t)sound);
	on_right = (uint16_t)((state_offset & ~0xFFu) | 0xFFu);
	screen_x = (uint16_t)(OBJECT(object)->screen_x - 0x10u);
	screen_y = OBJECT(object)->screen_y;
	say_your_prayers_toitles(on_right, screen_x, screen_y, &d5, &sound);
	OBJECT(object)->y = 0x6C00u;
	OBJECT(object)->enemy_rocksteady.timer = 0x78u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_laugh_animation_ADDRESS;
	set_animation(screen_x, object, animation);
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
	*p_sound = sound;
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
	settings = table_loaded_by_lea_054746_ADDRESS;
	enemy_set_health(object, settings);
	OBJECT(object)->enemy_rocksteady.timer = 0x10u;
	animation = animations_0596BA_ADDRESS;
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
	animation = animations_0596BA_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks in for 32 frames, then his health and palette, hittable, and he thinks */
uint32_t enemy_rocksteady_enter_walk_in_with_bebop(uint32_t object, uint32_t a5)
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
		data = 0xD0B4u;
		request = 0x43Au;
		a5 = post_palette_request(request, data);
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
	}
	OBJECT(object)->y -= 0x100u;
	return a5;
}

/* his thinking: the gun drawn or put away in turns, blows close by, shots from afar, the pincer charge
   with Bebop */
void enemy_rocksteady_think(uint32_t object, uint32_t target, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_turtle, uint32_t *p_a5)
{
	uint16_t armed_offset = 0, count = 0;
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, move_table = 0, moves = 0, turtle = 0;
	uint8_t retarget_time = 0, far_attack_wait = 0, close_attack_wait = 0, move_choice = 0, t1 = 0;
	uint16_t mode_time = 0, t2 = 0;

	*p_d7 = (*p_d7 & ~0xFFu) | OBJECT(object)->parameter;
	enemy_flash_when_weak((uint8_t)*p_d7, object);
	retarget_time = OBJECT(object)->enemy_rocksteady.retarget_time;
	OBJECT(object)->enemy_rocksteady.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_rocksteady.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &target);
			OBJECT(object)->enemy_rocksteady.target = target;
		}
	}
	*p_turtle = 0x60000u | OBJECT(object)->enemy_rocksteady.target;
	turtle = *p_turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	enemy_keep_on_screen(object);
	if (pincer_charge != 0u)
	{
		OBJECT(object)->enemy_rocksteady.target = pincer_charge_target;
		OBJECT(object)->enemy_rocksteady.move = 0x98u;
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		return;
	}
	mode_time = OBJECT(object)->enemy_rocksteady.mode_time;
	OBJECT(object)->enemy_rocksteady.mode_time = mode_time - 1u;
	if (mode_time != 1u)
	{
		if ((OBJECT(*p_turtle)->routine != 4u || hit_reaction_down[OBJECT(*p_turtle)->turtle.hit_reaction] != 0u) && OBJECT(object)->enemy_rocksteady.target_dy <= 8u)
		{
			if (OBJECT(object)->enemy_rocksteady.target_dx > 0x32u)
			{
				far_attack_wait = OBJECT(object)->enemy_rocksteady.far_attack_wait;
				OBJECT(object)->enemy_rocksteady.far_attack_wait = far_attack_wait - 1u;
				if (far_attack_wait == 1u && (OBJECT(object)->enemy_rocksteady.armed != 0u || rocksteady_last_attack != 0x88u))
				{
					moves = rocksteady_far_attacks_ADDRESS;
					*p_a5 = rocksteady_far_attack_waits_ADDRESS;
					enemy_rocksteady_pick_attack(object, moves, p_d7, p_a5);
					OBJECT(object)->enemy_rocksteady.far_attack_wait = mem_read8(*p_a5 + *p_d7);
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
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
					*p_a5 = rocksteady_close_attack_waits_ADDRESS;
					enemy_rocksteady_pick_attack(object, moves, p_d7, p_a5);
					OBJECT(object)->enemy_rocksteady.close_attack_wait = mem_read8(*p_a5 + *p_d7);
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					return;
				}
			}
		}
		if (OBJECT(object)->enemy_rocksteady.move_time == 0u)
		{
			move_table = rocksteady_move_choices_ADDRESS + (uint16_t)((uint32_t)OBJECT(object)->enemy_rocksteady.armed << 3);
			*p_d7 = 0x64u;
			*p_d7 = random_below((uint16_t)*p_d7, object);
			count = 3u;
			for (;;)
			{
				move_choice = MOVE_CHOICE(move_table + ((int32_t)((int16_t)count + 4u)));
				t1 = *p_d7;
				*p_d7 = (*p_d7 & ~0xFFu) | (uint8_t)(t1 - move_choice);
				if (t1 < move_choice)
					break;
				t2 = count - 1u;
				count = t2;
				if (t2 != 0xFFFFu)
					continue;
				*p_d7 = 0x64u;
				*p_d7 = random_below((uint16_t)*p_d7, object);
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
		*p_a5 = rocksteady_close_attack_waits_ADDRESS;
		OBJECT(object)->enemy_rocksteady.close_attack_wait = rocksteady_close_attack_waits[armed_offset];
		OBJECT(object)->enemy_rocksteady.far_attack_wait = rocksteady_far_attack_waits[armed_offset];
	}
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_rocksteady.move)
	{
	case 20:
		enemy_shuffle(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
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
void enemy_rocksteady_attack(uint32_t object, uint32_t *p_d5, uint32_t *p_colour, uint32_t *p_a1, uint32_t *p_a5)
{
	uint32_t colour = *p_colour;

	colour = (colour & ~0xFFu) | OBJECT(object)->parameter;
	enemy_flash_when_weak((uint8_t)colour, object);
	*p_colour = colour;
	enemy_boss_attack_move(object, p_d5, p_colour, p_a1, p_a5);
}

/* Rocksteady's and Bebop's attack by their move */
void enemy_boss_attack_move(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_turtle, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, turtle = 0;
	uint32_t jump_target = 0;

	*p_turtle = 0x60000u | OBJECT(object)->enemy.target;
	turtle = *p_turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	enemy_keep_on_screen(object);
	jump_target = TABLE32(enemy_attacks, (uint32_t)(int16_t)OBJECT(object)->enemy.move);
	switch (jump_target)
	{
	case 0x049A06u:
		enemy_jab(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x049A52u:
		enemy_chop(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x049A8Eu:
		enemy_swing(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x049ACAu:
		enemy_high_kick(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x049B06u:
		enemy_jump_kick(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x049BBCu:
		enemy_long_jump_kick(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x049C72u:
		enemy_attack_none_after_long_jump_kick();
		return;
	case 0x049C74u:
		enemy_attack_none_before_grab();
		return;
	case 0x049C76u:
		enemy_grab(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04B2DAu:
		enemy_slash_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04B316u:
		enemy_low_slash_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04B352u:
		enemy_leaping_slash_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04B42Au:
		enemy_jump_slash_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04B4E0u:
		enemy_flying_slash_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04B5D6u:
		enemy_smash_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04E2C4u:
		enemy_spear_thrust(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04E336u:
		enemy_spear_throw(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04E3C4u:
		enemy_knife_stab(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04E400u:
		enemy_high_kick_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04E43Cu:
		enemy_jump_kick_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04E4F2u:
		enemy_throw_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04EBD6u:
		enemy_throw_axes(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04EC5Eu:
		enemy_high_kick_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04EC9Au:
		enemy_gun_swing(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04ECD6u:
		enemy_shoot_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04F076u:
		enemy_throw_shuriken(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04F0E4u:
		enemy_boomerang_throw(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04FA96u:
		enemy_rocksteady_pincer_charge(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04FC04u:
		enemy_rocksteady_charge(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04FCD4u:
		enemy_rocksteady_punch(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04FD4Cu:
		enemy_rocksteady_gun_butt(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x04FD88u:
		enemy_rocksteady_shoot(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x0508A0u:
		enemy_bebop_pincer_charge(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x050ADCu:
		enemy_bebop_punch(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x050B8Cu:
		enemy_bebop_charge(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x050C68u:
		enemy_bebop_shoot(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x052306u:
		enemy_rodney_shoot(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x052422u:
		enemy_rodney_whip(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x0534C4u:
		enemy_shredder_kick(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x053524u:
		enemy_shredder_thrust(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x05356Eu:
		enemy_shredder_slash(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x0535B8u:
		enemy_shredder_sweep(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0x053602u:
		enemy_shredder_ray(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	}
}

/* hit: his reaction by what is left of his health and where the hit came from */
void enemy_rocksteady_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_sound, uint32_t *p_a1, uint32_t *p_a5)
{
	uint32_t sound = *p_sound;
	uint8_t hit = 0;
	uint16_t facing_bit = 0, move = 0;

	rocksteady_hits_to_strike_back--;
	sound = (sound & ~0xFFu) | 0x82u;
	queue_sound((uint8_t)sound);
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
	*p_sound = sound;
	enemy_rocksteady_react(object, p_d5, p_sound, p_a1, p_a5);
}

/* his reaction to a hit, flashing when weak */
void enemy_rocksteady_react(uint32_t object, uint32_t *p_d5, uint32_t *p_colour, uint32_t *p_a1, uint32_t *p_a5)
{
	uint32_t colour = *p_colour;

	colour = (colour & ~0xFFu) | OBJECT(object)->parameter;
	enemy_flash_when_weak((uint8_t)colour, object);
	*p_colour = colour;
	enemy_boss_react_move(object, p_d5, p_colour, p_a1, p_a5);
}

/* Rocksteady's and Bebop's reaction by their move */
void enemy_boss_react_move(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_attacker, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0;

	*p_attacker = 0x60000u | OBJECT(object)->attacker;
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored, &ignored_2, &ignored_3, &ignored_4, p_d5, p_d7, p_a5);
	enemy_keep_on_screen(object);
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy.move)
	{
	case 228:
		enemy_crash_into_scenery(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_crash_into_scenery_low(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_flinch_back(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_flinch_bent(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_flinch_reel(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_flinch_hunched(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_killed_back(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_killed_forward(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_knocked_forward(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_thrown_by_donatello(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 132:
		*p_a5 = enemy_let_go_by_donatello(object, *p_attacker);
		return;
	case 136:
		enemy_thrown_by_michelangelo(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 140:
		*p_a5 = enemy_let_go_by_michelangelo(object, *p_attacker);
		return;
	case 188:
		enemy_thrown_by_raphael(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 192:
		*p_a5 = enemy_let_go_by_raphael(object, *p_attacker);
		return;
	case 196:
		enemy_thrown_by_leonardo(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 200:
		*p_a5 = enemy_let_go_by_leonardo(object, *p_attacker);
		return;
	case 20:
		enemy_knocked_back(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_die(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_die_on_back(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_flinch_back_sword(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_flinch_bent_sword(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_flinch_reel_sword(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_flinch_hunched_sword(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_flinch_back_hammer(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_flinch_bent_hammer(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_flinch_hunched_hammer(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_flinch_back_spear(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_flinch_bent_spear(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_flinch_reel_spear(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_flinch_hunched_spear(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_flinch_back_knife(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_flinch_bent_knife(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_flinch_reel_knife(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_flinch_hunched_knife(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_flinch_back_gun(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_flinch_bent_gun(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_flinch_back_axe(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_flinch_bent_axe(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_flinch_reel_axe(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_rocksteady_flinch(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_rocksteady_reel(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_rocksteady_flinch_from_behind(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_rocksteady_knocked_down(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_rocksteady_defeated(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_bebop_flinch(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_bebop_flinch_from_behind(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_bebop_knocked_down(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_bebop_defeated(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_mouser_thrown_off(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_mouser_break_down(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_mouser_knocked_flying(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_rodney_knocked_back(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_rodney_destroyed(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_shredder_lose_bar(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_shredder_flinch(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_shredder_flinch_from_behind(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_shredder_defeated(object, *p_attacker, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_shredder_vanish(object, *p_attacker, p_d5, p_d7, p_a5);
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
void enemy_rocksteady_raise_fist(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F72E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* raises his fist at his turtle */
void enemy_rocksteady_raise_fist_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0;

	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 1u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_raise_fist_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* laughs at his turtle */
void enemy_rocksteady_laugh(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F75C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* laughs at his turtle for 136 frames */
void enemy_rocksteady_laugh_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0;

	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0x88u;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_laugh_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* shakes his fist at his turtle */
void enemy_rocksteady_shake_fist(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F78A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* shakes his fist at his turtle for 64 frames */
void enemy_rocksteady_shake_fist_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0;

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
void enemy_rocksteady_draw_gun(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F7BE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* draws his gun, facing his turtle */
void enemy_rocksteady_draw_gun_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0;

	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0xFFu;
	OBJECT(object)->state++;
	animation = animations_05A08A_ADDRESS;
	set_animation(0u, object, animation);
}

/* the move is over when its animation ends */
void enemy_rocksteady_pose_end(uint32_t object)
{
	if (OBJECT(object)->frame_time == 0u)
		OBJECT(object)->enemy_rocksteady.move_time = 0u;
}

/* puts his gun away */
void enemy_rocksteady_put_gun_away(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F7FA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* puts his gun away, facing his turtle */
void enemy_rocksteady_put_gun_away_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0;

	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_rocksteady.move_time = 0xFFu;
	OBJECT(object)->state++;
	animation = enemy_rocksteady_put_gun_away_animation_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks round his turtle */
void enemy_rocksteady_circle(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F828_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 180 frames of walking round his turtle, one way or the other at random */
void enemy_rocksteady_circle_start(uint32_t object, uint32_t target, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_rocksteady_circle.move_time = 0xB4u;
	OBJECT(object)->enemy_rocksteady_circle.aim_time = 1u;
	OBJECT(object)->enemy_rocksteady_circle.turn = 0u;
	if ((random_seed & 0x80u) != 0u)
		OBJECT(object)->enemy_rocksteady_circle.turn = 0x40u;
	OBJECT(object)->state++;
	animation = animations_0596BA_ADDRESS;
	set_animation(0u, object, animation);
	enemy_rocksteady_circle_step(object, target, p_d7, p_a5);
}

/* a frame of the walk round his turtle, aiming again every 8 frames */
void enemy_rocksteady_circle_step(uint32_t object, uint32_t target, uint32_t *p_d7, uint32_t *p_a5)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t x_speed = 0;
	uint8_t aim_time = 0;

	OBJECT(object)->enemy_rocksteady_circle.move_time--;
	aim_time = OBJECT(object)->enemy_rocksteady_circle.aim_time;
	OBJECT(object)->enemy_rocksteady_circle.aim_time = aim_time - 1u;
	if (aim_time == 1u)
	{
		OBJECT(object)->enemy_rocksteady_circle.aim_time = 8u;
		enemy_boss_face_target(object, target);
		direction = (uint8_t)((OBJECT(object)->enemy_rocksteady_circle.target_direction & 0xC0u) + OBJECT(object)->enemy_rocksteady_circle.turn);
		speed = 0x160u;
		speeds_from_direction(direction, speed, &x_speed, p_d7, p_a5);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_d7;
	}
	move_object_blocked(object);
}

/* lines up with his turtle with the gun up */
void enemy_rocksteady_line_up(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F8A8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* with the gun up, 240 frames of lining up with his turtle */
void enemy_rocksteady_line_up_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_rocksteady.move_time = 0xF0u;
	OBJECT(object)->enemy_rocksteady.timer = 1u;
	OBJECT(object)->state++;
	animation = animations_05979C_ADDRESS;
	set_animation(0u, object, animation);
	enemy_rocksteady_line_up_step(object, target);
}

/* a frame of lining up: away across while nearer than 0x50, then up or down to his turtle's row */
void enemy_rocksteady_line_up_step(uint32_t object, uint32_t target)
{
	OBJECT(object)->enemy_rocksteady.move_time--;
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
void enemy_rocksteady_close_in(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F92E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* with the gun level, 128 frames of closing in on his turtle */
void enemy_rocksteady_close_in_start(uint32_t object, uint32_t turtle, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_rocksteady_close_in.move_time = 0x80u;
	OBJECT(object)->enemy_rocksteady_close_in.aim_time = 1u;
	OBJECT(object)->state++;
	animation = animations_059618_ADDRESS;
	set_animation(0u, object, animation);
	enemy_rocksteady_close_in_step(object, turtle, p_d7, p_a5);
}

/* a frame of closing in, aiming every 8 frames at a place just in front of his turtle */
void enemy_rocksteady_close_in_step(uint32_t object, uint32_t turtle, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t speed = 0;
	uint32_t direction = 0, target_x = 0, target_y = 0, x_speed = 0;
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
	target_x = (uint16_t)(target_x + OBJECT(turtle)->screen_x);
	target_y = OBJECT(turtle)->screen_y;
	direction_to(object, &direction, &target_x, &target_y);
	speed = 0x180u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, p_d7, p_a5);
	OBJECT(object)->x_speed = (int16_t)x_speed >> 1;
	OBJECT(object)->y_speed = *p_d7;
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
void enemy_rocksteady_shoot_around(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04F9CC_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* six aims to go round, three shots each */
uint32_t enemy_rocksteady_shoot_around_start(uint32_t object, uint32_t record)
{
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rocksteady_shoot_around.aims = rocksteady_shoot_around_aims_ADDRESS;
	OBJECT(object)->enemy_rocksteady_shoot_around.move_time = 7u;
	OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = 1u;
	OBJECT(object)->enemy_rocksteady_shoot_around.shots_left = 1u;
	return enemy_rocksteady_shoot_around_fire(object, record);
}

/* every 8 frames a bullet with its sound; after three, the next aim with its pose */
uint32_t enemy_rocksteady_shoot_around_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint16_t aim = 0, aim_number = 0;
	uint32_t aims = 0, animation = 0, object_2 = 0;
	uint8_t shot_time = 0, shots_left = 0, move_time = 0;
	uint32_t t1 = 0;

	shot_time = OBJECT(object)->enemy_rocksteady_shoot_around.shot_time;
	OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = shot_time - 1u;
	if (shot_time != 1u)
		return record;
	shots_left = OBJECT(object)->enemy_rocksteady_shoot_around.shots_left;
	OBJECT(object)->enemy_rocksteady_shoot_around.shots_left = shots_left - 1u;
	if (shots_left != 1u)
	{
		OBJECT(object)->enemy_rocksteady_shoot_around.shot_time = 8u;
		sound = 0x18u;
		queue_sound(sound);
		record = OBJECT(object)->enemy_rocksteady_shoot_around.bullet;
		start_shot(object, record, &object_2, &zero);
		if (zero)
			OBJECT(object_2)->parameter = OBJECT(object)->enemy_rocksteady_shoot_around.aim;
		return record;
	}
	move_time = OBJECT(object)->enemy_rocksteady_shoot_around.move_time;
	OBJECT(object)->enemy_rocksteady_shoot_around.move_time = move_time - 1u;
	if (move_time == 1u)
		return record;
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
	record = OBJECT(object)->enemy_rocksteady_shoot_around.bullet;
	start_shot(object, record, &object_2, &zero);
	if (zero)
		OBJECT(object_2)->parameter = OBJECT(object)->enemy_rocksteady_shoot_around.aim;
	return record;
}

/* charges across from his edge while Bebop charges from the other */
void enemy_rocksteady_pincer_charge(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FACE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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

	animation = animations_0596BA_ADDRESS;
	if (OBJECT(object)->enemy_rocksteady.armed != 0u)
		animation = animations_059618_ADDRESS;
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
	animation = animations_05983E_ADDRESS;
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
		animation = animations_05AD06_ADDRESS;
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
void enemy_rocksteady_charge(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FC10_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the charge's animation and damage, facing his turtle, not hittable; 30 frames to get ready */
void enemy_rocksteady_charge_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = animations_05983E_ADDRESS;
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
void enemy_rocksteady_punch(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FCE0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the punch's animation and damage, facing his turtle; hittable, unless he was not and two or more
   turtles play */
void enemy_rocksteady_punch_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;
	uint8_t flags = 0;

	animation = animations_059FAA_ADDRESS;
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
void enemy_rocksteady_gun_butt(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FD58_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_rocksteady_shoot(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FD94_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* four shots at his turtle, level, or up at one high in the air */
uint32_t enemy_rocksteady_shoot_start(uint32_t object, uint32_t turtle)
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
	set_animation(0u, object, animation_entry);
	return animations;
}

/* every 8 frames a bullet with its sound; after the fourth, back to thinking */
uint32_t enemy_rocksteady_shoot_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t object_2 = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_rocksteady_shoot.shot_time;
	OBJECT(object)->enemy_rocksteady_shoot.shot_time = shot_time - 1u;
	if (shot_time != 1u)
		return record;
	sound = 0x18u;
	queue_sound(sound);
	record = OBJECT(object)->enemy_rocksteady_shoot.bullet;
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
	return record;
}

/* hit from the front */
void enemy_rocksteady_flinch(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FE4E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_rocksteady_reel(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FEB6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_rocksteady_flinch_from_behind(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FEEE_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_rocksteady_knocked_down(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_04FF26_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked down along his arc, gun out or not, facing his turtle */
uint32_t enemy_rocksteady_knocked_down_start(uint32_t d7, uint32_t object, uint32_t target)
{
	uint32_t animation_entry = 0, animations = 0;

	enemy_boss_face_target(object, target);
	animations = rocksteady_knocked_down_animations_2_ADDRESS;
	d7 = enemy_start_arc(d7, object, animations);
	animations = rocksteady_knocked_down_animations_ADDRESS;
	if (OBJECT(object)->enemy_rocksteady_knocked_down.armed != 0u)
		animations += 0xCu;
	OBJECT(object)->enemy_rocksteady_knocked_down.animations = animations;
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the fall; on landing he slides */
uint32_t enemy_rocksteady_knocked_down_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = rocksteady_knocked_down_animations_2_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = mem_read32(OBJECT(object)->enemy_rocksteady_knocked_down.animations + 4u);
	set_animation(0u, object, animation);
	return d7;
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
	uint32_t animation = 0, settings = 0;
	uint8_t lie_time = 0;

	lie_time = OBJECT(object)->enemy_rocksteady_knocked_down.lie_time;
	OBJECT(object)->enemy_rocksteady_knocked_down.lie_time = lie_time - 1u;
	if (lie_time != 1u)
		return;
	OBJECT(object)->state++;
	settings = table_loaded_by_lea_054746_ADDRESS;
	if (area != 2u)
		settings = rocksteady_health_ADDRESS;
	enemy_refill_health(object, settings);
	animation = mem_read32(OBJECT(object)->enemy_rocksteady_knocked_down.animations + 8u);
	set_animation(0u, object, animation);
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
void enemy_rocksteady_defeated(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050082_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* defeated: no more pincer charge, thrown along his arc, gun out or not */
uint32_t enemy_rocksteady_defeated_start(uint32_t d7, uint32_t object, uint32_t target)
{
	uint32_t animation_entry = 0, animations = 0;

	no_pincer_charge = 0xFFu;
	enemy_boss_face_target(object, target);
	animations = rocksteady_defeated_animations_2_ADDRESS;
	d7 = enemy_start_arc(d7, object, animations);
	animations = rocksteady_defeated_animations_ADDRESS;
	if (OBJECT(object)->enemy_rocksteady_defeated.armed != 0u)
		animations += 8u;
	OBJECT(object)->enemy_rocksteady_defeated.animations = animations;
	animation_entry = ANIMATION_ENTRY(animations);
	set_animation(0u, object, animation_entry);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the fall; on landing a sound and one boss less, the last one ending the fight and the music */
uint32_t enemy_rocksteady_defeated_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0, sound = 0;
	uint8_t t1 = 0;

	move_object_with_height_blocked(object);
	arc = rocksteady_defeated_animations_2_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	t1 = bosses_left;
	bosses_left = t1 - 1u;
	if (t1 == 1u)
	{
		boss_defeated = 1u;
		d7 &= ~0xFFFFu;
		music = d7;
		queue_sound((uint8_t)d7);
	}
	sound = (d7 & ~0xFFu) | 0x8Bu;
	queue_sound((uint8_t)sound);
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = mem_read32(OBJECT(object)->enemy_rocksteady_defeated.animations + 4u);
	set_animation(0u, object, animation);
	return sound;
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
void enemy_bebop_start(uint32_t object, uint32_t target, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0, data = 0;

	enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &target);
	OBJECT(object)->enemy_bebop.target = target;
	OBJECT(object)->enemy_bebop.retarget_time = 0x10u;
	OBJECT(object)->enemy_bebop.move_time = 0u;
	OBJECT(object)->enemy_bebop.plan = 0u;
	OBJECT(object)->depth = 4u;
	OBJECT(object)->enemy_bebop.punch_delay = 3u;
	OBJECT(object)->shadow = sprite_lists_059A18_ADDRESS;
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
	data = 0xD094u;
	*p_d7 = (*p_d7 & ~0xFFFFu) | 0x43Cu;
	*p_a5 = post_palette_request((uint16_t)*p_d7, data);
}

/* his way in: drops in, then jumps forward to the ground, or waits for Rocksteady and walks in */
void enemy_bebop_enter(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050286_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_bebop_enter_drop(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, sound = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x60u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	sound = (d7 & ~0xFFu) | 0x3Eu;
	queue_sound((uint8_t)sound);
	OBJECT(object)->state++;
	animation = enemy_bebop_enter_drop_animation_ADDRESS;
	set_animation(0u, object, animation);
	return sound;
}

/* when the pose ends, jumps forward off his floor */
uint32_t enemy_bebop_enter_jump(uint32_t d7, uint32_t object)
{
	if (OBJECT(object)->frame_time != 0u)
		return d7;
	OBJECT(object)->height_speed = 0x600;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = -0x100;
	OBJECT(object)->floor_height = 0u;
	OBJECT(object)->state++;
	return enemy_bebop_enter_in_air(d7, object);
}

/* a frame of the jump; on landing a sound and a pose, then he gets ready */
uint32_t enemy_bebop_enter_in_air(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, sound = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x60u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	sound = (d7 & ~0xFFu) | 0x3Fu;
	queue_sound((uint8_t)sound);
	OBJECT(object)->state++;
	animation = enemy_bebop_enter_in_air_animation_ADDRESS;
	set_animation(0u, object, animation);
	enemy_bebop_enter_ready(object);
	return sound;
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
	animation = animations_05A430_ADDRESS;
	set_animation(0u, object, animation);
}

/* walks in for 32 frames, then his health and palette, hittable, and he thinks */
uint32_t enemy_bebop_enter_walk_in(uint32_t object, uint32_t a5)
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
		data = 0xD094u;
		kind = 0x43Eu;
		a5 = post_palette_request(kind, data);
	}
	OBJECT(object)->y -= 0x100u;
	return a5;
}

/* his thinking: his two taunts, the pincer charge with Rocksteady, a punch when close, an attack or a
   move by plan */
void enemy_bebop_think(uint32_t routine_offset, uint32_t object, uint32_t target, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_turtle, uint32_t *p_a5)
{
	uint32_t zero = 0;
	uint8_t plan = 0;
	uint16_t on_right = 0, screen_x = 0, screen_y = 0;
	uint32_t attack = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, move = 0, turtle = 0;
	uint8_t retarget_time = 0;
	uint16_t t1 = 0, chance = 0, t2 = 0;

	*p_d7 = (*p_d7 & ~0xFFu) | OBJECT(object)->parameter;
	enemy_flash_when_weak((uint8_t)*p_d7, object);
	if (area == 2u)
	{
		if (bebop_exchange_lines == 0u && time_since_balloon >= 0x80u)
		{
			routine_offset = (routine_offset & ~0xFFu) | 0xFFu;
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				routine_offset &= ~0xFFu;
			screen_x = OBJECT(object)->screen_x;
			screen_y = OBJECT(object)->screen_y;
			say_youre_dead_shell_brain(screen_x, screen_y, &routine_offset, p_d5, p_d7, &target);
		}
		if (bebop_exchange_lines == 2u && time_since_balloon >= 0x60u)
		{
			on_right = (uint16_t)((routine_offset & ~0xFFu) | 0xFFu);
			if ((OBJECT(object)->flags & 0x40u) != 0u)
				on_right &= ~0xFFu;
			screen_x = OBJECT(object)->screen_x;
			screen_y = OBJECT(object)->screen_y;
			say_watch_your_mouth(on_right, screen_x, screen_y, p_d5, p_d7, &target);
		}
	}
	retarget_time = OBJECT(object)->enemy_bebop.retarget_time;
	OBJECT(object)->enemy_bebop.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_bebop.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			enemy_nearest_turtles(object, p_d5, p_d7, p_a5, &target);
			OBJECT(object)->enemy_bebop.target = target;
		}
	}
	*p_turtle = 0x60000u | OBJECT(object)->enemy_bebop.target;
	turtle = *p_turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	enemy_keep_on_screen(object);
	if (no_pincer_charge == 0u)
	{
		t1 = pincer_charge_wait;
		pincer_charge_wait = t1 - 1u;
		if (t1 == 1u)
		{
			pincer_charge_wait = 0x1Eu;
			pincer_charge = 1u;
			pincer_charge_target = *p_turtle;
			pincer_charge_edges = 0u;
			pincer_charge_at_edges = 0u;
			pincer_charge_crash = 0u;
			OBJECT(object)->enemy_bebop.move = 0x84u;
			OBJECT(object)->routine = 3u;
			OBJECT(object)->state = 0u;
			return;
		}
	}
	if (OBJECT(*p_turtle)->routine != 4u || hit_reaction_down[OBJECT(*p_turtle)->turtle.hit_reaction] != 0u)
	{
		enemy_on_screen(object, p_d7, &zero);
		if (zero)
		{
			if (OBJECT(object)->enemy_bebop.target_dx >= 0x28u)
			{
				attack = bebop_attacks_ADDRESS + (uint16_t)((OBJECT(object)->enemy_bebop.plan & 3u) << 3);
				*p_d7 = 0x64u;
				*p_d7 = random_below((uint16_t)*p_d7, object);
				if (*p_d7 < BEBOP_ATTACK(attack)->chance && OBJECT(object)->enemy_bebop.target_dy <= BEBOP_ATTACK(attack)->reach_y && OBJECT(object)->enemy_bebop.target_dx <= BEBOP_ATTACK(attack)->reach_x)
				{
					OBJECT(object)->enemy_bebop.move = BEBOP_ATTACK(attack)->move;
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					return;
				}
			}
			else if (OBJECT(object)->enemy_bebop.target_dy < 5u)
			{
				OBJECT(object)->enemy_bebop.move = 0x7Cu;
				OBJECT(object)->routine = 3u;
				OBJECT(object)->state = 0u;
				OBJECT(object)->flags &= ~8u;
				return;
			}
		}
	}
	if (OBJECT(object)->enemy_bebop.move_time == 0u)
	{
		move = bebop_far_moves_ADDRESS;
		*p_d7 = 0x64u;
		*p_d7 = random_below((uint16_t)*p_d7, object);
		plan = 0u;
		for (;;)
		{
			chance = RANDOM_MOVE(move)->chance;
			t2 = *p_d7;
			*p_d7 = (uint16_t)(t2 - chance);
			if (t2 < chance)
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
		enemy_shuffle(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, *p_turtle, p_d5, p_d7, p_a5);
		return;
	}
}

/* his attack, flashing when weak */
void enemy_bebop_attack(uint32_t object, uint32_t *p_d5, uint32_t *p_colour, uint32_t *p_a1, uint32_t *p_a5)
{
	uint32_t colour = *p_colour;

	colour = (colour & ~0xFFu) | OBJECT(object)->parameter;
	enemy_flash_when_weak((uint8_t)colour, object);
	*p_colour = colour;
	enemy_boss_attack_move(object, p_d5, p_colour, p_a1, p_a5);
}

/* hit: his reaction by what is left of his health and where the hit came from */
void enemy_bebop_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_sound, uint32_t *p_a1, uint32_t *p_a5)
{
	uint32_t sound = *p_sound;
	uint16_t move = 0;

	sound = (sound & ~0xFFu) | 0x82u;
	queue_sound((uint8_t)sound);
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
	*p_sound = sound;
	enemy_bebop_react(object, p_d5, p_sound, p_a1, p_a5);
}

/* his reaction to a hit, flashing when weak */
void enemy_bebop_react(uint32_t object, uint32_t *p_d5, uint32_t *p_colour, uint32_t *p_a1, uint32_t *p_a5)
{
	uint32_t colour = *p_colour;

	colour = (colour & ~0xFFu) | OBJECT(object)->parameter;
	enemy_flash_when_weak((uint8_t)colour, object);
	*p_colour = colour;
	enemy_boss_react_move(object, p_d5, p_colour, p_a1, p_a5);
}

/* stands facing his turtle */
void enemy_bebop_stand(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05068A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 120 frames standing facing his turtle, in one of four poses at random */
uint32_t enemy_bebop_stand_start(uint32_t object, uint32_t target)
{
	uint32_t animation = 0, range = 0;

	enemy_boss_face_target(object, target);
	OBJECT(object)->enemy_bebop.move_time = 0x78u;
	OBJECT(object)->state++;
	range = 4u;
	range = random_below((uint16_t)range, object);
	range = range & 3u;
	range = (uint16_t)(range << 2);
	animation = enemy_bebop_stand_animations[range / 4u];
	set_animation(0u, object, animation);
	return range;
}

/* stands while the move's time runs out */
void enemy_bebop_stand_wait(uint32_t object)
{
	OBJECT(object)->enemy_bebop.move_time--;
}

/* walks to his turtle's front */
void enemy_bebop_walk_to_front(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0506E4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to a place 0x50 in front of his turtle */
void enemy_bebop_walk_to_front_start(uint32_t object, uint32_t turtle, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_bebop.move_time = 0xF0u;
	OBJECT(object)->enemy_bebop.timer = 1u;
	OBJECT(object)->state++;
	animation = animations_05A430_ADDRESS;
	set_animation(0u, object, animation);
	enemy_bebop_walk_to_front_step(object, turtle, p_d7, p_a5);
}

/* a frame of the walk, aiming every 4 frames, slower when near or level with his turtle */
void enemy_bebop_walk_to_front_step(uint32_t object, uint32_t turtle, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t speed = 0;
	uint32_t direction = 0, target_x = 0, target_y = 0, x_speed = 0;
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
	target_x = (uint16_t)(target_x + OBJECT(turtle)->screen_x);
	target_y = OBJECT(turtle)->screen_y;
	direction_to(object, &direction, &target_x, &target_y);
	speed = 0x200u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, p_d7, p_a5);
	x_speed = (uint16_t)((int16_t)x_speed >> 1);
	if (OBJECT(object)->enemy_bebop.target_dx <= 0x50u)
		x_speed = (uint16_t)((int16_t)x_speed >> 1);
	if (OBJECT(object)->enemy_bebop.target_dy <= 2u)
		*p_d7 = (uint16_t)((int16_t)*p_d7 >> 1);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_d7;
	OBJECT(object)->enemy_bebop.timer = 4u;
	move_object_blocked(object);
}

/* walks level with his turtle, then to it */
void enemy_bebop_walk_level(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05077C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* a frame of the walk: up or down to his turtle's row, then across toward it, faster when far */
void enemy_bebop_walk_level_step(uint32_t object, uint32_t target)
{
	uint8_t timer = 0;

	OBJECT(object)->enemy_bebop.move_time--;
	timer = OBJECT(object)->enemy_bebop.timer;
	OBJECT(object)->enemy_bebop.timer = timer - 1u;
	if (timer != 1u)
	{
		move_object_blocked(object);
		return;
	}
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
void enemy_bebop_close_in(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0507FA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* starts one of his walks */
void enemy_bebop_walk_start(uint32_t object, uint32_t turtle, uint32_t *p_y_speed, uint32_t *p_a5)
{
	uint32_t animation = 0;

	OBJECT(object)->enemy_bebop.move_time = 0xF0u;
	OBJECT(object)->enemy_bebop.timer = 1u;
	OBJECT(object)->state++;
	animation = animations_05A430_ADDRESS;
	set_animation(0u, object, animation);
	enemy_bebop_close_in_step(object, turtle, p_y_speed, p_a5);
}

/* a frame of closing in on a place 0x30 in front of his turtle, aiming every 4 frames; level with it he
   stands */
void enemy_bebop_close_in_step(uint32_t object, uint32_t turtle, uint32_t *p_y_speed, uint32_t *p_a5)
{
	uint16_t speed = 0;
	uint32_t direction = 0, target_x = 0, target_y = 0, x_speed = 0;
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
		target_x = (uint16_t)(target_x + OBJECT(turtle)->screen_x);
		target_y = OBJECT(turtle)->screen_y;
		direction_to(object, &direction, &target_x, &target_y);
		speed = 0x180u;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, p_y_speed, p_a5);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_y_speed;
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
void enemy_bebop_pincer_charge(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0508D8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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

	animation = animations_05A430_ADDRESS;
	set_animation(0u, object, animation);
	enemy_pincer_charge_take_edge(object);
}

/* takes a free edge of the screen for the pincer charge and runs to it */
void enemy_pincer_charge_take_edge(uint32_t object)
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
	animation = animations_05A930_ADDRESS;
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
void enemy_bebop_punch(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050AE8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_05A9BE_ADDRESS;
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
void enemy_bebop_charge(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050B98_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the charge's animation and damage, facing his turtle, not hittable; 30 frames to get ready */
void enemy_bebop_charge_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = animations_05A930_ADDRESS;
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
void enemy_bebop_shoot(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050C74_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_05A77E_ADDRESS;
	set_animation(0u, object, animation);
}

/* a shot with its sound, then every 12 frames; after the third, back to thinking */
uint32_t enemy_bebop_shoot_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t ignored = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_bebop_shoot.shot_time;
	OBJECT(object)->enemy_bebop_shoot.shot_time = shot_time - 1u;
	if (shot_time == 1u)
	{
		sound = 2u;
		queue_sound(sound);
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
	return record;
}

/* hit from the front, then strikes back */
void enemy_bebop_flinch(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050CF2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_bebop_flinch_from_behind(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050D38_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* hit from behind, he staggers forward for 48 frames */
void enemy_bebop_flinch_from_behind_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->state++;
	OBJECT(object)->enemy_bebop.move_time = 0x30u;
	animation = animations_05AAF6_ADDRESS;
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
void enemy_bebop_knocked_down(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050D84_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked down along his arc, facing his turtle */
uint32_t enemy_bebop_knocked_down_start(uint32_t d7, uint32_t object, uint32_t target)
{
	uint32_t animation = 0, arc = 0;

	enemy_boss_face_target(object, target);
	arc = enemy_bebop_knocked_down_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	animation = enemy_bebop_knocked_down_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
}

/* a frame of the fall; on landing he slides */
uint32_t enemy_bebop_knocked_down_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_bebop_knocked_down_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_bebop_knocked_down_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_bebop_defeated(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_050E72_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* defeated: no more pincer charge, thrown along his arc */
uint32_t enemy_bebop_defeated_start(uint32_t d7, uint32_t object, uint32_t target)
{
	uint32_t animation = 0, arc = 0;

	no_pincer_charge = 0xFFu;
	enemy_boss_face_target(object, target);
	arc = enemy_bebop_defeated_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_bebop_defeated_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the fall; on landing a sound and one boss less, the last one ending the fight and the music */
uint32_t enemy_bebop_defeated_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0, sound = 0;
	uint8_t t1 = 0;

	move_object_with_height_blocked(object);
	arc = enemy_bebop_defeated_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	t1 = bosses_left;
	bosses_left = t1 - 1u;
	if (t1 == 1u)
	{
		boss_defeated = 1u;
		d7 &= ~0xFFFFu;
		music = d7;
		queue_sound((uint8_t)d7);
	}
	sound = (d7 & ~0xFFu) | 0x8Bu;
	queue_sound((uint8_t)sound);
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_bebop_defeated_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return sound;
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
void enemy_mouser_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;
	uint8_t parameter = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
	PLAYER_SLOT(player_slot)->enemies++;
	OBJECT(object)->enemy_mouser.target = player_slot;
	OBJECT(object)->enemy_mouser.retarget_time = 0x10u;
	OBJECT(object)->enemy_mouser.move_time = 0u;
	OBJECT(object)->enemy_mouser.approach = 0u;
	OBJECT(object)->depth = 4u;
	enemies_on_screen++;
	OBJECT(object)->health_bars = 1u;
	OBJECT(object)->health = 1u;
	OBJECT(object)->shadow = sprite_lists_0582B4_ADDRESS;
	parameter = OBJECT(object)->parameter;
	OBJECT(object)->parameter = parameter & 0x7Fu;
	if ((parameter & 0x80u) != 0u)
	{
		OBJECT(object)->flags |= 0x82u;
		OBJECT(object)->routine += 2u;
		animation = enemy_mouser_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	else if (OBJECT(object)->parameter == 0u)
	{
		enemy_set_floor(object);
		OBJECT(object)->flags |= 0x82u;
		OBJECT(object)->routine += 2u;
		animation = enemy_mouser_animation_ADDRESS;
		set_animation(0u, object, animation);
	}
	else
		OBJECT(object)->routine++;
}

/* hops in from its parameter's height onto the floor */
void enemy_mouser_enter(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05100E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_mouser_enter_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, sound = 0;

	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->state++;
	sound = (d7 & ~0xFFu) | 0x39u;
	queue_sound((uint8_t)sound);
	animation = enemy_mouser_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return sound;
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
void enemy_mouser_think(uint32_t object, uint32_t second_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	bool L_051210 = false;
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint32_t floor_down = 0, floor_left = 0, floor_right = 0, floor_up = 0, ignored = 0, ignored_2 = 0, turtle = 0;
	uint8_t retarget_time = 0, t3 = 0, t4 = 0, t5 = 0;
	uint32_t t1 = 0, t2 = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy_mouser.target;
	retarget_time = OBJECT(object)->enemy_mouser.retarget_time;
	OBJECT(object)->enemy_mouser.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_mouser.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
			enemy_nearest_slots(object, p_d5, p_d7, p_a5, &second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = *p_a5;
				*p_a5 = second_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						t2 = *p_a5;
						*p_a5 = second_slot;
						second_slot = t2;
					}
				}
			}
			*p_d7 = (*p_d7 & ~0xFFFFu) | (uint16_t)second_slot;
			if ((uint16_t)*p_d7 != OBJECT(object)->enemy_mouser.target)
				enemy_mouser_leave_side(object, *p_slot);
			OBJECT(object)->enemy_mouser.target = second_slot;
			*p_slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	if (mousers_break_down != 0u)
	{
		OBJECT(object)->enemy_mouser.move = 0xE0u;
		OBJECT(object)->routine = 5u;
		OBJECT(object)->state = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		enemy_mouser_leave_side(object, *p_slot);
		mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t3 = enemies_on_screen - 1u;
		enemies_on_screen = t3;
		if ((int8_t)t3 >= 0)
		{
			return;
		}
		enemies_on_screen = 0u;
		return;
	}
	turtle = mem_read32(*p_slot);
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &floor_right, &floor_left, &floor_up, &floor_down, p_d5, p_d7, p_a5);
	zero = enemy_check_floor_edge((uint8_t)floor_right, (uint8_t)floor_left, (uint8_t)floor_up, (uint8_t)floor_down, object, *p_slot);
	if (zero == 0u)
	{
		return;
	}
	if (OBJECT(object)->enemy_mouser.approach != 0u)
	{
		if (OBJECT(object)->enemy_mouser.approach == 1u && (OBJECT(object)->enemy_mouser.target_dx >= 0x54u || OBJECT(object)->enemy_mouser.target_dy >= 0x34u))
			enemy_mouser_give_side_back(object, *p_slot);
		if (OBJECT(object)->enemy_mouser.approach != 2u)
		{
			turtle = mem_read32(*p_slot);
			if (OBJECT(turtle)->routine == 4u && hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] == 0u)
			{
				enemy_mouser_leave_side(object, *p_slot);
				OBJECT(object)->enemy_mouser.approach = 2u;
				OBJECT(object)->enemy_mouser.move_time = 0u;
			}
		}
	}
	else if (mem_read8(*p_slot + 7u) >= 0xC0u || OBJECT(object)->enemy_mouser.target_dx >= 0x40u || OBJECT(object)->enemy_mouser.target_dy >= 0x20u)
	{
		if (OBJECT(object)->enemy_mouser.approach != 2u)
		{
			turtle = mem_read32(*p_slot);
			if (OBJECT(turtle)->routine == 4u && hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] == 0u)
			{
				enemy_mouser_leave_side(object, *p_slot);
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
					t4 = mem_read8(*p_slot + 7u);
					mem_write8(*p_slot + 7u, t4 | 0x40u);
					if ((t4 & 0x40u) != 0u)
					{
						t5 = mem_read8(*p_slot + 7u);
						mem_write8(*p_slot + 7u, t5 | 0x80u);
						if ((t5 & 0x80u) != 0u)
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
				t5 = mem_read8(*p_slot + 7u);
				mem_write8(*p_slot + 7u, t5 | 0x80u);
				L_051210 = (t5 & 0x80u) != 0u;
				if (!L_051210)
					OBJECT(object)->enemy_mouser.side &= 0x7Fu;
			}
		} while (L_051210);
	}
	enemy_on_screen(object, p_d7, &zero);
	if (zero && mem_read8(mem_read32(*p_slot) + 4u) != 4u && OBJECT(object)->enemy_mouser.target_dy < 4u && OBJECT(object)->enemy_mouser.target_dx < 0x30u)
	{
		OBJECT(object)->routine = 3u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->flags &= ~8u;
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
		enemy_shuffle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* gives its side of its turtle back when it stands by it */
void enemy_mouser_leave_side(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->enemy_mouser.approach != 1u)
		return;
	enemy_mouser_give_side_back(object, player_slot);
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
void enemy_mouser_bite(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy_mouser.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	states = indexed_table_of_32_bit_values_051340_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
	*p_slot = slot;
}

/* the bite: leaps at its turtle along its arc, attacking */
uint32_t enemy_mouser_bite_start(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0, attack = 0;

	animation = enemy_mouser_hop_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->damage = 0x103u;
	attack = (d7 & ~0xFFu) | 0x10u;
	enemy_face_target((uint8_t)attack, object);
	arc = enemy_mouser_bite_arc_ADDRESS;
	attack = enemy_start_arc(attack, object, arc);
	OBJECT(object)->flags |= 4u;
	OBJECT(object)->flags &= ~8u;
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_mouser.move = 0xA0u;
	return attack;
}

/* a frame of the leap; biting, it hides and hangs on; missing, it lands and waits 16 frames */
uint32_t enemy_mouser_bite_fly(uint32_t d7, uint32_t object, uint32_t player_slot)
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
		enemy_mouser_leave_side(object, player_slot);
		return d7;
	}
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->flags &= ~4u;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state = 2u;
	animation = animations_058076_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_mouser_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1, uint32_t *p_a5)
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
	enemy_react(object, p_d5, p_d7, p_a1, p_a5);
}

/* a step over the floor's edge */
void enemy_mouser_step_over_edge(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, slot = 0, states = 0, turtle = 0;

	slot = 0x60000u | OBJECT(object)->enemy_mouser.target;
	turtle = PLAYER_SLOT(slot)->turtle;
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	states = TABLE32(mouser_edge_steps, (uint32_t)(int16_t)OBJECT(object)->enemy_mouser.move);
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
	*p_slot = slot;
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
uint32_t enemy_mouser_hop_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_up(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = animations_058076_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
uint32_t enemy_mouser_hop_up_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = enemy_mouser_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
uint32_t enemy_mouser_hop_up_fall_blocked(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0;

	enemy_follow_stairs_down(object);
	move_object_with_height_blocked(object);
	OBJECT(object)->height_speed -= 0x40u;
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->flags |= 2u;
	OBJECT(object)->enemy_mouser.move_time = 0x10u;
	OBJECT(object)->state++;
	animation = animations_058076_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_mouser_chase(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05175C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* chases its turtle for a random time under 240 frames */
void enemy_mouser_chase_start(uint8_t direction, uint32_t object, uint32_t *p_chase_time, uint32_t *p_animations)
{
	uint32_t animations = 0, chase_time = 0;

	OBJECT(object)->enemy_mouser.move_time = 0xFFu;
	OBJECT(object)->enemy_mouser.timer = 8u;
	chase_time = 0xF0u;
	chase_time = random_below((uint16_t)chase_time, object);
	OBJECT(object)->enemy_mouser.chase_time = chase_time;
	OBJECT(object)->state++;
	animations = animations_05176C_ADDRESS;
	animations = enemy_face_direction(direction, object, animations);
	*p_chase_time = chase_time;
	*p_animations = animations;
}

/* a frame of the chase, aiming every 8 frames; close to its turtle or the time up, it sits */
void enemy_mouser_chase_run(uint32_t object, uint32_t player_slot, uint32_t *p_d7, uint32_t *p_animations)
{
	uint32_t d7 = *p_d7;
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0;
	uint8_t chase_time = 0, timer = 0;

	if (OBJECT(object)->enemy_mouser.target_dx >= 0x3Eu || OBJECT(object)->enemy_mouser.target_dy >= 0x1Eu)
	{
		chase_time = OBJECT(object)->enemy_mouser.chase_time;
		OBJECT(object)->enemy_mouser.chase_time = chase_time - 1u;
		if (chase_time != 1u)
		{
			timer = OBJECT(object)->enemy_mouser.timer;
			OBJECT(object)->enemy_mouser.timer = timer - 1u;
			if (timer == 1u)
			{
				turtle = PLAYER_SLOT(player_slot)->turtle;
				direction = direction_to_object(0u, object, turtle);
				*p_animations = animations_05176C_ADDRESS;
				enemy_turn_to_direction((uint8_t)direction, object, *p_animations);
				speed = 0xC0u;
				speeds_from_direction((uint8_t)direction, speed, &x_speed, &d7, p_animations);
				OBJECT(object)->x_speed = x_speed;
				d7 = (uint16_t)((int16_t)d7 >> 1);
				OBJECT(object)->y_speed = d7;
				OBJECT(object)->enemy_mouser.timer = 8u;
			}
			move_object_blocked(object);
		}
		else
		{
			d7 = 0xF0u;
			d7 = random_below((uint16_t)d7, object);
			OBJECT(object)->enemy_mouser.chase_time = d7;
			OBJECT(object)->state++;
		}
	}
	else
		OBJECT(object)->state++;
	*p_d7 = d7;
}

/* sits for 120 frames */
uint32_t enemy_mouser_chase_pause(uint32_t object)
{
	OBJECT(object)->enemy_mouser.timer = 0x78u;
	return enemy_mouser_sit(object);
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
void enemy_mouser_creep_to_side(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_051836_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* creeps to its side of its turtle, aiming in 8 frames */
uint32_t enemy_mouser_creep_to_side_start(uint8_t direction, uint32_t object)
{
	uint32_t animations = 0;

	OBJECT(object)->enemy_mouser.move_time = 0xFFu;
	OBJECT(object)->enemy_mouser.timer = 8u;
	OBJECT(object)->state++;
	animations = animations_05176C_ADDRESS;
	return enemy_face_direction(direction, object, animations);
}

/* a frame of creeping to 0x20 beside its turtle, aiming every 8 frames */
void enemy_mouser_creep_to_side_step(uint32_t object, uint32_t player_slot, uint32_t *p_d7, uint32_t *p_animations)
{
	uint16_t speed = 0;
	uint32_t direction = 0, object_2 = 0, target_x = 0, target_y = 0, x_speed = 0;
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
	target_x = (uint16_t)(target_x + OBJECT(object_2)->screen_x);
	target_y = OBJECT(object_2)->screen_y;
	direction_to(object, &direction, &target_x, &target_y);
	*p_animations = animations_05176C_ADDRESS;
	enemy_turn_to_direction((uint8_t)direction, object, *p_animations);
	speed = 0x80u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, p_d7, p_animations);
	OBJECT(object)->x_speed = x_speed;
	*p_d7 = (uint16_t)((int16_t)*p_d7 >> 1);
	OBJECT(object)->y_speed = *p_d7;
	OBJECT(object)->enemy_mouser.timer = 8u;
	move_object_blocked(object);
}

/* runs away, sits and waits while its turtle is down */
void enemy_mouser_back_off(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0518BC_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 32 frames of running away from its turtle */
void enemy_mouser_back_off_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_animations)
{
	uint16_t speed = 0;
	uint32_t animations = 0, direction = 0, ignored = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_mouser.move_time = 0xFFu;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_mouser.timer = 0x20u;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	direction = direction_to_object(0u, object, turtle);
	direction = (uint8_t)(direction - 0x80u);
	speed = 0x100u;
	speeds_from_direction((uint8_t)direction, speed, &x_speed, p_y_speed, &ignored);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	animations = animations_05176C_ADDRESS;
	animations = enemy_face_direction((uint8_t)direction, object, animations);
	*p_animations = animations;
}

/* a frame of running away; at its end it sits */
uint32_t enemy_mouser_back_off_run(uint8_t direction, uint32_t object)
{
	uint32_t animations = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_mouser.timer;
	OBJECT(object)->enemy_mouser.timer = timer - 1u;
	if (timer == 1u)
		OBJECT(object)->state++;
	animations = animations_05176C_ADDRESS;
	animations = enemy_turn_to_direction(direction, object, animations);
	move_object_blocked(object);
	return animations;
}

/* sits in one of two poses */
uint32_t enemy_mouser_sit(uint32_t object)
{
	uint32_t animation = 0, range = 0;

	OBJECT(object)->state++;
	range = 2u;
	range = random_below((uint16_t)range, object);
	range = (uint16_t)(range << 2);
	animation = enemy_mouser_sit_animations[range / 4u];
	set_animation(0u, object, animation);
	return range;
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
void enemy_mouser_thrown_off(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_051974_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* thrown off by the turtle it held: shown again, tumbling along its arc the way it faced */
uint32_t enemy_mouser_thrown_off_start(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->flags |= 0x80u;
	OBJECT(object)->flags &= 0xBFu;
	if ((OBJECT(object)->enemy_mouser.facing & 0x80u) != 0u)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_mouser_knocked_flying_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_mouser_tumble_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* hops away and breaks when the boss is beaten */
void enemy_mouser_break_down(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0519CC_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* its boss down, it hops off tumbling along its arc and breaks */
uint32_t enemy_mouser_break_down_start(uint32_t d7, uint32_t object)
{
	uint32_t animation = 0, arc = 0;

	arc = enemy_mouser_break_down_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_mouser_tumble_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* knocked flying by a hit */
void enemy_mouser_knocked_flying(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_051A14_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked flying by a hit, tumbling along its arc, its side given up */
uint32_t enemy_mouser_knocked_flying_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	enemy_mouser_leave_side(object, player_slot);
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_mouser.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_mouser_knocked_flying_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_mouser_tumble_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* flies until it hits the floor and breaks */
uint32_t enemy_mouser_fly_and_crash(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0, sound = 0;

	enemy_follow_stairs(object);
	move_object_with_height(object);
	arc = enemy_mouser_knocked_flying_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	sound = (d7 & ~0xFFu) | 0xFu;
	queue_sound((uint8_t)sound);
	OBJECT(object)->state++;
	animation = enemy_mouser_fly_and_crash_animation_ADDRESS;
	set_animation(0u, object, animation);
	return sound;
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
	uint32_t animation = 0, hole = 0;

	if (OBJECT(object)->screen_x < 0x100u)
	{
		enemies_on_screen += 6u;
		hole = OBJECT(object)->mouser_hole.hole;
		animation = mem_read32(hole + 4u);
		set_animation(0u, object, animation);
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
uint32_t mouser_hole_let_out(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0;
	uint8_t spawn_time = 0, mousers_left = 0, t1 = 0;

	spawn_time = OBJECT(object)->mouser_hole.spawn_time;
	OBJECT(object)->mouser_hole.spawn_time = spawn_time - 1u;
	if (spawn_time == 1u)
	{
		record = OBJECT(object)->mouser_hole.hole;
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
	return record;
}

/* a Roadkill Rodney: one bar, coming up through the floor */
void enemy_rodney_start(uint32_t object, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t animation = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
		animation = animations_0585C4_ADDRESS;
		set_animation(0u, object, animation);
		return;
	}
	OBJECT(object)->frame_time = 0u;
	OBJECT(object)->routine++;
	OBJECT(object)->enemy_rodney.wait = 0xB4u;
}

/* comes up through the floor with a jump and lands */
void enemy_rodney_enter(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_051CB8_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_rodney_think(uint32_t object, uint32_t second_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	bool L_051E78 = false;
	uint32_t zero = 0;
	uint8_t enemies = 0;
	uint16_t approach = 0, count = 0;
	uint32_t attack = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, turtle = 0;
	uint8_t retarget_time = 0, t3 = 0, t4 = 0, t5 = 0;
	uint16_t t6 = 0;
	uint32_t t1 = 0, t2 = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy_rodney.target;
	retarget_time = OBJECT(object)->enemy_rodney.retarget_time;
	OBJECT(object)->enemy_rodney.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_rodney.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
			enemy_nearest_slots(object, p_d5, p_d7, p_a5, &second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = *p_a5;
				*p_a5 = second_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						t2 = *p_a5;
						*p_a5 = second_slot;
						second_slot = t2;
					}
				}
			}
			*p_d7 = (*p_d7 & ~0xFFFFu) | (uint16_t)second_slot;
			if ((uint16_t)*p_d7 != OBJECT(object)->enemy_rodney.target)
				enemy_rodney_leave_side(object, *p_slot);
			OBJECT(object)->enemy_rodney.target = second_slot;
			*p_slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	if (boss_defeated != 0u)
	{
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t3 = enemies_on_screen - 1u;
		enemies_on_screen = t3;
		if ((int8_t)t3 < 0)
			enemies_on_screen = 0u;
		return;
	}
	if ((OBJECT(object)->flags & 0x20u) != 0u)
	{
		enemy_rodney_leave_side(object, *p_slot);
		mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
		OBJECT(object)->flags = 0u;
		OBJECT(object)->contacts = 0u;
		t3 = enemies_on_screen - 1u;
		enemies_on_screen = t3;
		if ((int8_t)t3 < 0)
			enemies_on_screen = 0u;
		return;
	}
	turtle = mem_read32(*p_slot);
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	enemy_collide_with_objects(object, p_d5, p_d7, p_a5);
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	if (OBJECT(object)->enemy_rodney.approach != 0u)
	{
		if (OBJECT(object)->enemy_rodney.approach == 1u && ((int16_t)OBJECT(object)->screen_x < 0x70 || (int16_t)OBJECT(object)->screen_x >= 0x190))
		{
			OBJECT(object)->enemy_rodney.approach = 0u;
			OBJECT(object)->enemy_rodney.move_time = 0u;
			enemy_rodney_give_side_back(object, *p_slot);
		}
	}
	else if (mem_read8(*p_slot + 6u) < 0xC0u && OBJECT(object)->enemy_rodney.target_dx < 0x20u)
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
					t4 = mem_read8(*p_slot + 6u);
					mem_write8(*p_slot + 6u, t4 | 0x40u);
					if ((t4 & 0x40u) != 0u)
					{
						t5 = mem_read8(*p_slot + 6u);
						mem_write8(*p_slot + 6u, t5 | 0x80u);
						if ((t5 & 0x80u) != 0u)
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
				t5 = mem_read8(*p_slot + 6u);
				mem_write8(*p_slot + 6u, t5 | 0x80u);
				L_051E78 = (t5 & 0x80u) != 0u;
				if (!L_051E78)
					OBJECT(object)->enemy_rodney.side &= 0x7Fu;
			}
		} while (L_051E78);
	}
	if (OBJECT(object)->enemy_rodney.approach != 1u)
	{
		enemy_on_screen(object, p_d7, &zero);
		if (zero && OBJECT(object)->enemy_rodney.approach == 1u && mem_read8(mem_read32(*p_slot) + 4u) != 4u && OBJECT(object)->enemy_rodney.target_dy < 6u)
		{
			*p_d7 = 0x64u;
			*p_d7 = random_below((uint16_t)*p_d7, object);
			count = 1u;
			attack = rodney_attacks_ADDRESS;
			do
			{
				if (RODNEY_ATTACK(attack)->reach <= OBJECT(object)->enemy_rodney.target_dx && (uint8_t)*p_d7 < RODNEY_ATTACK(attack)->chance)
				{
					OBJECT(object)->enemy_rodney.move = (uint32_t)RODNEY_ATTACK(attack)->move << 2;
					OBJECT(object)->routine = 3u;
					OBJECT(object)->state = 0u;
					OBJECT(object)->flags &= ~8u;
					return;
				}
				t6 = count - 1u;
				count = t6;
				attack += 4u;
			} while (t6 != 0xFFFFu);
		}
	}
	else
	{
		turtle = mem_read32(*p_slot);
		if (OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u)
		{
			enemy_on_screen(object, p_d7, &zero);
			if (zero && OBJECT(object)->enemy_rodney.approach == 1u && mem_read8(mem_read32(*p_slot) + 4u) != 4u && OBJECT(object)->enemy_rodney.target_dy < 6u)
			{
				*p_d7 = 0x64u;
				*p_d7 = random_below((uint16_t)*p_d7, object);
				count = 1u;
				attack = rodney_attacks_ADDRESS;
				do
				{
					if (RODNEY_ATTACK(attack)->reach <= OBJECT(object)->enemy_rodney.target_dx && (uint8_t)*p_d7 < RODNEY_ATTACK(attack)->chance)
					{
						OBJECT(object)->enemy_rodney.move = (uint32_t)RODNEY_ATTACK(attack)->move << 2;
						OBJECT(object)->routine = 3u;
						OBJECT(object)->state = 0u;
						OBJECT(object)->flags &= ~8u;
						return;
					}
					t6 = count - 1u;
					count = t6;
					attack += 4u;
				} while (t6 != 0xFFFFu);
			}
		}
		else
		{
			enemy_rodney_give_side_back(object, *p_slot);
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
		enemy_shuffle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* hit: knocked back, or destroyed with no health left */
void enemy_rodney_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1, uint32_t *p_a5)
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
	enemy_react(object, p_d5, p_d7, p_a1, p_a5);
}

/* gives its side of its turtle back when it stands by it */
void enemy_rodney_leave_side(uint32_t object, uint32_t player_slot)
{
	if (OBJECT(object)->enemy_rodney.approach != 1u)
		return;
	enemy_rodney_give_side_back(object, player_slot);
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
void enemy_rodney_rush(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052002_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* rushes at its turtle for 180 frames, aiming just off its row when it is close to it */
void enemy_rodney_rush_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_a5)
{
	uint32_t animation = 0, direction = 0, object_2 = 0, screen_x = 0, speed = 0, target_y = 0, x_speed = 0;

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
		target_y = (uint16_t)(target_y + OBJECT(object_2)->screen_y);
		direction_to(object, &direction, &screen_x, &target_y);
	}
	speed = 0x300u;
	speeds_from_direction((uint8_t)direction, (uint16_t)speed, &x_speed, p_y_speed, p_a5);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->enemy_rodney.move_time = 0xB4u;
	OBJECT(object)->state++;
	animation = animations_0585C4_ADDRESS;
	set_animation(speed, object, animation);
}

/* a frame of the rush, sliding along walls; well past its turtle it brakes, or stops at a wall */
uint32_t enemy_rodney_rush_run(uint32_t object)
{
	uint32_t animation = 0, clear_ahead = 0;

	OBJECT(object)->enemy_rodney.move_time--;
	clear_ahead = 0xFFFFFFFFu;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		if ((OBJECT(object)->contacts & 8u) != 0u)
		{
			OBJECT(object)->y -= 0x100u;
			clear_ahead &= ~0xFFu;
		}
	}
	else if ((OBJECT(object)->contacts & 0x10u) != 0u)
	{
		OBJECT(object)->y -= 0x100u;
		clear_ahead &= ~0xFFu;
	}
	if ((int8_t)((OBJECT(object)->enemy_rodney.target_direction - 0x40u) ^ ((uint32_t)OBJECT(object)->flags << 1)) >= 0)
	{
		move_object_blocked(object);
		return clear_ahead;
	}
	if ((int8_t)clear_ahead >= 0)
		OBJECT(object)->enemy_rodney.move_time = 0u;
	if (OBJECT(object)->enemy_rodney.target_dx < 0x50u)
	{
		move_object_blocked(object);
		return clear_ahead;
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
	return clear_ahead;
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
void enemy_rodney_rush_across(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052142_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_0585C4_ADDRESS;
	set_animation(0u, object, animation);
}

/* a frame of the rush across, sliding along walls; well past its turtle it brakes toward its row */
uint32_t enemy_rodney_rush_across_run(uint32_t object)
{
	uint32_t animation = 0, clear_ahead = 0;

	OBJECT(object)->enemy_rodney.move_time--;
	clear_ahead = 0xFFFFFFFFu;
	if ((OBJECT(object)->flags & 0x40u) != 0u)
	{
		if ((OBJECT(object)->contacts & 8u) != 0u)
		{
			OBJECT(object)->y -= 0x100u;
			clear_ahead &= ~0xFFu;
		}
	}
	else if ((OBJECT(object)->contacts & 0x10u) != 0u)
	{
		OBJECT(object)->y -= 0x100u;
		clear_ahead &= ~0xFFu;
	}
	if ((int8_t)((OBJECT(object)->enemy_rodney.target_direction - 0x40u) ^ ((uint32_t)OBJECT(object)->flags << 1)) >= 0)
	{
		move_object_blocked(object);
		return clear_ahead;
	}
	if ((int8_t)clear_ahead >= 0)
		OBJECT(object)->enemy_rodney.move_time = 0u;
	if (OBJECT(object)->enemy_rodney.target_dx < 0x50u)
	{
		move_object_blocked(object);
		return clear_ahead;
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
	return clear_ahead;
}

/* slides back and waits while its turtle is down */
void enemy_rodney_back_off(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052234_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_rodney_slide_to_stop(uint32_t object, uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d5 = *p_d5;
	uint16_t brake = 0, on_right = 0, screen_y = 0;
	uint32_t animation = 0, screen_x = 0;
	uint16_t t1 = 0;

	brake = (uint16_t)OBJECT(object)->enemy_rodney.brake;
	t1 = OBJECT(object)->x_speed + brake;
	OBJECT(object)->x_speed = t1;
	if (t1 != 0u)
		move_object_blocked(object);
	else
	{
		OBJECT(object)->state++;
		if (rodney_line_said == 0u)
		{
			on_right = (uint16_t)((brake & ~0xFFu) | 0xFFu);
			screen_x = OBJECT(object)->screen_x;
			screen_y = OBJECT(object)->screen_y;
			say_do_not_resist_us(on_right, screen_x, screen_y, &d5, p_d7);
			rodney_line_said = 0xFFu;
		}
		animation = enemy_rodney_slide_to_stop_animation_ADDRESS;
		set_animation(screen_x, object, animation);
	}
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
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
void enemy_rodney_shoot(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052312_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_rodney_shoot_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t animation = 0, object_2 = 0;
	uint8_t shot_time = 0, shots_left = 0;

	shot_time = OBJECT(object)->enemy_rodney_shoot.shot_time;
	OBJECT(object)->enemy_rodney_shoot.shot_time = shot_time - 1u;
	if (shot_time != 1u)
		return record;
	OBJECT(object)->enemy_rodney_shoot.shot_time = 4u;
	record = laser_bolt_shot_record_ADDRESS;
	start_shot(object, record, &object_2, &zero);
	if (zero)
	{
		OBJECT(object_2)->parameter = (uint32_t)OBJECT(object)->flags << 1;
		OBJECT(object_2)->start_shot.hit_sound = OBJECT(object)->enemy_rodney_shoot.move;
		shots_left = OBJECT(object)->enemy_rodney_shoot.shots_left;
		OBJECT(object)->enemy_rodney_shoot.shots_left = shots_left - 1u;
		if (shots_left != 1u)
			return record;
	}
	OBJECT(object)->state++;
	animation = enemy_rodney_shoot_fire_animation_ADDRESS;
	set_animation(0u, object, animation);
	return record;
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
void enemy_rodney_whip(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05242E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the whip: following its turtle's row, it slides back */
void enemy_rodney_whip_start(uint32_t object)
{
	enemy_rodney_follow_row(object);
	enemy_rodney_shoot_start(object);
}

/* slides to a stop, following its turtle's row */
void enemy_rodney_whip_slow_down(uint32_t object, uint32_t *p_d5, uint32_t *p_d7)
{
	uint32_t d5 = *p_d5;

	enemy_rodney_follow_row(object);
	enemy_rodney_slide_to_stop(object, &d5, p_d7);
	*p_d5 = (*p_d5 & 0xFFFFFF00u) | (d5 & 0xFFu);
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
uint32_t enemy_rodney_whip_throw_arm(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arm = 0, arm_record = 0;

	enemy_rodney_follow_row(object);
	if (OBJECT(object)->frame_time != 0u)
		return record;
	OBJECT(object)->state++;
	OBJECT(object)->enemy_rodney_whip.arm_flags = 0u;
	OBJECT(object)->enemy_rodney_whip.arm_length = 0u;
	arm_record = whip_arm_shot_record_ADDRESS;
	start_shot(object, arm_record, &arm, &zero);
	if (zero == 0u)
	{
		OBJECT(object)->routine = 2u;
		OBJECT(object)->state = 0u;
		OBJECT(object)->enemy_rodney_whip.move_time = 0u;
		OBJECT(object)->enemy_rodney_whip.retarget_time = 0u;
		return arm_record;
	}
	OBJECT(object)->enemy_rodney_whip.arm = arm;
	OBJECT(arm)->whip_arm.rodney = object;
	animation = enemy_rodney_whip_throw_arm_animation_ADDRESS;
	set_animation(0u, object, animation);
	return arm_record;
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
uint32_t enemy_rodney_whip_reach(uint32_t d7, uint32_t object)
{
	uint8_t arm_length = 0, x_distance = 0;
	uint32_t animation = 0, arm = 0, caught = 0;
	uint8_t t2 = 0;
	uint16_t t1 = 0;

	enemy_rodney_follow_row(object);
	arm = OBJECT(object)->enemy_rodney_whip.arm;
	if ((mem_read8(arm) & 8u) == 0u)
	{
		if ((OBJECT(object)->enemy_rodney_whip.arm_flags & 2u) != 0u)
		{
			enemy_rodney_whip_pull_in(object);
			return d7;
		}
		OBJECT(object)->enemy_rodney_whip.arm_length++;
		if (OBJECT(object)->enemy_rodney_whip.arm_length != 0xDu)
			return d7;
		OBJECT(object)->enemy_rodney_whip.arm_length--;
		enemy_rodney_whip_pull_in(object);
		return d7;
	}
	caught = 0x60000u | mem_read16(arm + 0x4Eu);
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
	set_animation(0u, object, animation);
	return caught;
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
uint32_t enemy_rodney_whip_lash(uint32_t object)
{
	uint32_t struck = 0;

	struck = 0x60000u | OBJECT(object)->hit_object;
	if ((OBJECT(object)->enemy_rodney_whip.arm_flags & 4u) != 0u)
	{
		enemy_rodney_whip_pull_in(object);
		return struck;
	}
	return struck;
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
void enemy_rodney_knocked_back(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05260C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* knocked back along its arc, facing its turtle, its side given up */
uint32_t enemy_rodney_knocked_back_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_rodney_destroyed_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_rodney_knocked_back_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	enemy_rodney_leave_side(object, player_slot);
	return d7;
}

/* a frame of the fall; on landing its pose */
uint32_t enemy_rodney_knocked_back_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	move_object_with_height_blocked(object);
	arc = enemy_rodney_knocked_back_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->state++;
	animation = enemy_rodney_knocked_back_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_rodney_destroyed(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0526B0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* destroyed: thrown along its arc, facing its turtle, its side given up */
uint32_t enemy_rodney_destroyed_start(uint32_t d7, uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_rodney.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	arc = enemy_rodney_destroyed_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_rodney_destroyed_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	enemy_rodney_leave_side(object, player_slot);
	return d7;
}

/* a frame of the fall; on landing a sound and its wreck */
uint32_t enemy_rodney_destroyed_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0, sound = 0;

	move_object_with_height_blocked(object);
	arc = enemy_rodney_destroyed_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	sound = (d7 & ~0xFFu) | 0xFu;
	queue_sound((uint8_t)sound);
	OBJECT(object)->state++;
	animation = enemy_rodney_destroyed_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return sound;
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
void enemy_shredder_start(uint32_t object, uint32_t slot, uint32_t player_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint16_t difficulty_level = 0;

	enemy_nearest_slots(object, p_d5, p_d7, p_a5, &player_slot);
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
		enemy_shredder_enter(object, slot, p_d5, p_d7, p_a5);
		return;
	}
	shredder_beaten = 0u;
	shredder_firing_ray = 0u;
	shredder_clones_made = 0u;
	OBJECT(object)->enemy_shredder.move = 0xECu;
}

/* his first move, without thinking */
void enemy_shredder_enter(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move)
	{
	case 20:
		enemy_shuffle(object, slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, slot, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, slot, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, slot, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, slot, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, slot, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, slot, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, slot, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, slot, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, slot, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* his thinking: clones, guards, blows by the distance and his ray */
void enemy_shredder_think(uint32_t object, uint32_t second_slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	bool L_052920 = false, L_052992 = false, L_052A68 = false;
	uint8_t enemies = 0, turtles = 0;
	uint16_t count = 0, target_dx = 0;
	uint32_t distance = 0, ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, turtle = 0;
	uint8_t retarget_time = 0, t3 = 0, t4 = 0, guard_wait = 0, attack_wait = 0;
	uint16_t t5 = 0, ray_wait = 0;
	uint32_t t1 = 0, t2 = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy_shredder.target;
	retarget_time = OBJECT(object)->enemy_shredder.retarget_time;
	OBJECT(object)->enemy_shredder.retarget_time = retarget_time - 1u;
	if (retarget_time == 1u)
	{
		OBJECT(object)->enemy_shredder.retarget_time = 0x10u;
		if (turtles_in_play != 0u)
		{
			mem_write8(*p_slot + 4u, mem_read8(*p_slot + 4u) - 1u);
			enemy_nearest_slots(object, p_d5, p_d7, p_a5, &second_slot);
			enemies = enemies_per_turtle;
			if (enemies <= PLAYER_SLOT(second_slot)->enemies)
			{
				t1 = *p_a5;
				*p_a5 = second_slot;
				second_slot = t1;
				if (enemies <= mem_read8(second_slot + 4u))
				{
					second_slot = least_crowded_slot;
					if (enemies <= PLAYER_SLOT(second_slot)->enemies)
					{
						t2 = *p_a5;
						*p_a5 = second_slot;
						second_slot = t2;
					}
				}
			}
			*p_d7 = (*p_d7 & ~0xFFFFu) | (uint16_t)second_slot;
			if ((uint16_t)*p_d7 != OBJECT(object)->enemy_shredder.target)
				enemy_leave_side(object, *p_slot);
			OBJECT(object)->enemy_shredder.target = second_slot;
			*p_slot = second_slot;
			mem_write8(second_slot + 4u, mem_read8(second_slot + 4u) + 1u);
		}
	}
	turtle = mem_read32(*p_slot);
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
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
				return;
			}
			if (OBJECT(object)->enemy_shredder.approach != 0u || mem_read8(*p_slot + 5u) >= 0xC0u)
			{
				if (OBJECT(object)->enemy_shredder.approach == 1u && (OBJECT(object)->enemy_shredder.target_dx >= 0x50u || OBJECT(object)->enemy_shredder.target_dy >= 0x50u))
					enemy_give_side_back(object, *p_slot);
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
							t3 = mem_read8(*p_slot + 5u);
							mem_write8(*p_slot + 5u, t3 | 0x40u);
							if ((t3 & 0x40u) != 0u)
							{
								t4 = mem_read8(*p_slot + 5u);
								mem_write8(*p_slot + 5u, t4 | 0x80u);
								if ((t4 & 0x80u) != 0u)
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
						t4 = mem_read8(*p_slot + 5u);
						mem_write8(*p_slot + 5u, t4 | 0x80u);
						L_052992 = (t4 & 0x80u) != 0u;
						if (!L_052992)
							OBJECT(object)->enemy_shredder.side &= 0x7Fu;
					}
				} while (L_052992);
			}
			if (OBJECT(object)->enemy_shredder.approach != 1u)
				L_052A68 = true;
			else
			{
				turtle = mem_read32(*p_slot);
				if (OBJECT(turtle)->routine != 4u || hit_reaction_down[OBJECT(turtle)->turtle.hit_reaction] != 0u)
				{
					guard_wait = OBJECT(object)->enemy_shredder.guard_wait;
					OBJECT(object)->enemy_shredder.guard_wait = guard_wait - 1u;
					L_052A68 = guard_wait != 1u;
					if (!L_052A68)
					{
						OBJECT(object)->enemy_shredder.guard_wait = shredder_guard_waits[difficulty & 7u];
						L_052A68 = mem_read8(mem_read32(*p_slot) + 4u) != 3u || OBJECT(object)->enemy_shredder.target_dy >= 6u || OBJECT(object)->enemy_shredder.target_dx >= 0x50u || (int8_t)OBJECT(object)->enemy_shredder.target_faces >= 0;
						if (!L_052A68)
						{
							enemy_give_side_back(object, *p_slot);
							OBJECT(object)->enemy_shredder.approach = 2u;
							OBJECT(object)->enemy_shredder.move_time = 0u;
						}
					}
				}
				else
				{
					enemy_give_side_back(object, *p_slot);
					OBJECT(object)->enemy_shredder.approach = 3u;
					OBJECT(object)->enemy_shredder.move_time = 0u;
				}
			}
			if (L_052A68)
			{
				if (mem_read8(mem_read32(*p_slot) + 4u) != 4u)
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
									return;
								}
								t5 = count - 1u;
								count = t5;
								distance += 2u;
							} while (t5 != 0xFFFFu);
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
		enemy_shuffle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_keep_distance(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_run_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_walk_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_walk_to_target_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_walk_to_side_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_walk_past_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_back_off_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_walk_to_line_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_walk_to_target_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_walk_to_side_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_stalk_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_walk_away_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_walk_to_target_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_walk_to_side_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_walk_off_line_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_back_off_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_step_aside_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_walk_to_target_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_walk_to_side_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_circle_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_keep_distance_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_back_off_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_step_aside_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_walk_to_target_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_walk_to_side_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_walk_to_range_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		enemy_back_off_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_run_away_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_walk_to_target_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_walk_to_side_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_walk_to_range_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		enemy_back_off_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 136:
		enemy_run_away_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 196:
		enemy_rocksteady_raise_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		enemy_rocksteady_laugh(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		enemy_rocksteady_shake_fist(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_rocksteady_draw_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 188:
		enemy_rocksteady_put_gun_away(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_rocksteady_circle(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_rocksteady_line_up(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_rocksteady_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_rocksteady_shoot_around(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 228:
		enemy_bebop_stand(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_bebop_walk_to_front(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_bebop_walk_level(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_bebop_close_in(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_mouser_chase(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_mouser_creep_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_mouser_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_rodney_rush(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_rodney_rush_across(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_rodney_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 236:
		enemy_shredder_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 240:
		enemy_shredder_split(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_shredder_clone_appear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 244:
		enemy_shredder_walk_to_side(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 248:
		enemy_shredder_walk_to_target(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 252:
		enemy_shredder_guard(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 256:
		enemy_shredder_back_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* his attack, keeping his ray's time */
void enemy_shredder_attack(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0, turtle = 0;
	uint16_t ray_wait = 0;
	uint32_t jump_target = 0;

	ray_wait = OBJECT(object)->enemy_shredder.ray_wait;
	OBJECT(object)->enemy_shredder.ray_wait = ray_wait - 1u;
	if (ray_wait == 1u)
		OBJECT(object)->enemy_shredder.ray_wait = 1u;
	*p_slot = 0x60000u | OBJECT(object)->enemy_shredder.target;
	turtle = mem_read32(*p_slot);
	enemy_look_at_target(object, turtle, &ignored, &ignored_2);
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored_3, &ignored, &ignored_2, &ignored_4, p_d5, p_d7, p_a5);
	enemy_keep_on_screen(object);
	jump_target = TABLE32(enemy_attacks, (uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move);
	switch (jump_target)
	{
	case 0x049A06u:
		enemy_jab(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049A52u:
		enemy_chop(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049A8Eu:
		enemy_swing(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049ACAu:
		enemy_high_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049B06u:
		enemy_jump_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049BBCu:
		enemy_long_jump_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x049C72u:
		enemy_attack_none_after_long_jump_kick();
		return;
	case 0x049C74u:
		enemy_attack_none_before_grab();
		return;
	case 0x049C76u:
		enemy_grab(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B2DAu:
		enemy_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B316u:
		enemy_low_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B352u:
		enemy_leaping_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B42Au:
		enemy_jump_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B4E0u:
		enemy_flying_slash_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04B5D6u:
		enemy_smash_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E2C4u:
		enemy_spear_thrust(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E336u:
		enemy_spear_throw(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E3C4u:
		enemy_knife_stab(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E400u:
		enemy_high_kick_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E43Cu:
		enemy_jump_kick_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04E4F2u:
		enemy_throw_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04EBD6u:
		enemy_throw_axes(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04EC5Eu:
		enemy_high_kick_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04EC9Au:
		enemy_gun_swing(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04ECD6u:
		enemy_shoot_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04F076u:
		enemy_throw_shuriken(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04F0E4u:
		enemy_boomerang_throw(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FA96u:
		enemy_rocksteady_pincer_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FC04u:
		enemy_rocksteady_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FCD4u:
		enemy_rocksteady_punch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FD4Cu:
		enemy_rocksteady_gun_butt(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x04FD88u:
		enemy_rocksteady_shoot(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x0508A0u:
		enemy_bebop_pincer_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x050ADCu:
		enemy_bebop_punch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x050B8Cu:
		enemy_bebop_charge(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x050C68u:
		enemy_bebop_shoot(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x052306u:
		enemy_rodney_shoot(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x052422u:
		enemy_rodney_whip(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x0534C4u:
		enemy_shredder_kick(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x053524u:
		enemy_shredder_thrust(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x05356Eu:
		enemy_shredder_slash(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x0535B8u:
		enemy_shredder_sweep(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0x053602u:
		enemy_shredder_ray(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* hit: shrugged off in the air, else his reaction by his health */
void enemy_shredder_hit(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a1, uint32_t *p_a5)
{
	uint8_t side = 0;

	*p_d7 = (*p_d7 & ~0xFFu) | 0x92u;
	queue_sound((uint8_t)*p_d7);
	side = OBJECT(object)->enemy_shredder.side;
	OBJECT(object)->enemy_shredder.side = side & ~1u;
	if ((side & 1u) != 0u)
	{
		shredder_firing_ray = 0u;
		if (OBJECT(object)->hit_state < 4u)
			OBJECT(object)->enemy_shredder.ray_wait = 1u;
	}
	if ((OBJECT(object)->health_bars | OBJECT(object)->health) != 0u)
	{
		if (OBJECT(object)->health == 0u)
		{
			OBJECT(object)->enemy_shredder.move = 0xCCu;
			OBJECT(object)->routine++;
			enemy_shredder_react(object, p_d5, p_d7, p_a1, p_a5);
			return;
		}
		*p_d7 = ((*p_d7 & ~0xFFFFu) | (((uint32_t)OBJECT(object)->floor_height << 8u) | OBJECT(object)->frame_time)) & ~0xFFu;
		if ((uint16_t)*p_d7 != OBJECT(object)->height)
		{
			OBJECT(object)->colour = 0xBu;
			OBJECT(object)->routine = 2u;
			return;
		}
		if ((int8_t)(((uint32_t)OBJECT(object)->flags << 1) ^ OBJECT(object)->hit) >= 0)
			OBJECT(object)->enemy_shredder.move = 0xD0u;
		else
			OBJECT(object)->enemy_shredder.move = 0xD4u;
	}
	else if ((int8_t)OBJECT(object)->parameter >= 0)
		OBJECT(object)->enemy_shredder.move = 0xD8u;
	else
		OBJECT(object)->enemy_shredder.move = 0xDCu;
	OBJECT(object)->routine++;
	enemy_shredder_react(object, p_d5, p_d7, p_a1, p_a5);
}

/* his reaction to a hit */
void enemy_shredder_react(uint32_t object, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_slot, uint32_t *p_a5)
{
	uint32_t ignored = 0, ignored_2 = 0, ignored_3 = 0, ignored_4 = 0;

	*p_slot = 0x60000u | OBJECT(object)->enemy_shredder.target;
	OBJECT(object)->contacts &= 0xE0u;
	enemy_floor_contacts(object, &ignored, &ignored_2, &ignored_3, &ignored_4, p_d5, p_d7, p_a5);
	enemy_keep_on_screen(object);
	switch ((uint32_t)(int16_t)OBJECT(object)->enemy_shredder.move)
	{
	case 228:
		enemy_crash_into_scenery(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 232:
		enemy_crash_into_scenery_low(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 0:
		enemy_flinch_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 4:
		enemy_flinch_bent(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 8:
		enemy_flinch_reel(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 12:
		enemy_flinch_hunched(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 36:
		enemy_killed_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 32:
		enemy_killed_forward(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 16:
		enemy_knocked_forward(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 128:
		enemy_thrown_by_donatello(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 132:
		*p_a5 = enemy_let_go_by_donatello(object, *p_slot);
		return;
	case 136:
		enemy_thrown_by_michelangelo(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 140:
		*p_a5 = enemy_let_go_by_michelangelo(object, *p_slot);
		return;
	case 188:
		enemy_thrown_by_raphael(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 192:
		*p_a5 = enemy_let_go_by_raphael(object, *p_slot);
		return;
	case 196:
		enemy_thrown_by_leonardo(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 200:
		*p_a5 = enemy_let_go_by_leonardo(object, *p_slot);
		return;
	case 20:
		enemy_knocked_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 24:
		enemy_die(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 28:
		enemy_die_on_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 40:
		enemy_flinch_back_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 44:
		enemy_flinch_bent_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 48:
		enemy_flinch_reel_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 52:
		enemy_flinch_hunched_sword(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 56:
		enemy_flinch_back_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 60:
		enemy_flinch_bent_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 68:
		enemy_flinch_hunched_hammer(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 72:
		enemy_flinch_back_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 76:
		enemy_flinch_bent_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 80:
		enemy_flinch_reel_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 84:
		enemy_flinch_hunched_spear(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 88:
		enemy_flinch_back_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 92:
		enemy_flinch_bent_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 96:
		enemy_flinch_reel_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 100:
		enemy_flinch_hunched_knife(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 120:
		enemy_flinch_back_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 124:
		enemy_flinch_bent_gun(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 104:
		enemy_flinch_back_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 108:
		enemy_flinch_bent_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 112:
		enemy_flinch_reel_axe(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 164:
		enemy_rocksteady_flinch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 168:
		enemy_rocksteady_reel(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 160:
		enemy_rocksteady_flinch_from_behind(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 152:
		enemy_rocksteady_knocked_down(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 156:
		enemy_rocksteady_defeated(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 172:
		enemy_bebop_flinch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 176:
		enemy_bebop_flinch_from_behind(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 180:
		enemy_bebop_knocked_down(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 184:
		enemy_bebop_defeated(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 148:
		enemy_mouser_thrown_off(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 224:
		enemy_mouser_break_down(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 144:
		enemy_mouser_knocked_flying(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 64:
		enemy_rodney_knocked_back(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 116:
		enemy_rodney_destroyed(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 204:
		enemy_shredder_lose_bar(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 208:
		enemy_shredder_flinch(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 212:
		enemy_shredder_flinch_from_behind(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 216:
		enemy_shredder_defeated(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	case 220:
		enemy_shredder_vanish(object, *p_slot, p_d5, p_d7, p_a5);
		return;
	}
}

/* comes out of the screen when he is the last enemy, with his music */
void enemy_shredder_appear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052CA2_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_05AD40_ADDRESS;
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
	animation = animations_05B07E_ADDRESS;
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
void enemy_shredder_split(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052D52_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* splits: not hittable, a clone of him with his bars and a sound, and he moves aside blinking 32 frames */
uint32_t enemy_shredder_split_start(uint32_t object)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, clone_record = 0, object_2 = 0;

	OBJECT(object)->flags &= ~2u;
	animation = animations_05B032_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->state++;
	OBJECT(object)->x_speed = 0x100;
	OBJECT(object)->y_speed = 0;
	OBJECT(object)->enemy_shredder.timer = 0x20u;
	clone_record = shredder_clone_record_ADDRESS;
	start_object_from_record(object, clone_record, &object_2, &zero);
	OBJECT(object_2)->health_bars = OBJECT(object)->health_bars;
	sound = 0x3Du;
	queue_sound(sound);
	return clone_record;
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
void enemy_shredder_clone_appear(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052DDC_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* a clone's way in: shown, moving back for 32 frames */
void enemy_shredder_clone_appear_start(uint32_t object)
{
	uint32_t animation = 0;

	OBJECT(object)->flags |= 0x80u;
	animation = animations_05B032_ADDRESS;
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
void enemy_shredder_walk_to_side(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052E60_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 240 frames of walking to his turtle's side, facing it */
uint32_t enemy_shredder_walk_to_side_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_shredder.move_time = 0xF0u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk: across to 0x40 by his turtle or up or down to its row; level with it, the next
   state */
uint32_t enemy_shredder_walk_to_side_step(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
	return animations;
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
uint32_t enemy_shredder_walk_to_side_turn(uint32_t object, uint32_t player_slot, uint32_t animations)
{
	uint8_t target_direction = 0;
	uint32_t turtle = 0;

	if (OBJECT(object)->frame_time != 0u)
		return animations;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* level with his turtle: walks across until 0x10 away; off its row again, back to walking to its side */
uint32_t enemy_shredder_walk_to_side_hold(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	if (OBJECT(object)->enemy_shredder.target_dx < 0x10u)
	{
		OBJECT(object)->x_speed = 0;
		OBJECT(object)->y_speed = 0;
		if (OBJECT(object)->enemy_shredder.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return animations;
	}
	OBJECT(object)->x_speed = 0x180;
	OBJECT(object)->y_speed = 0;
	if ((int8_t)(target_direction - 0x40u) < 0)
	{
		if (OBJECT(object)->enemy_shredder.target_dy >= 3u)
			OBJECT(object)->state -= 3u;
		move_object_blocked(object);
		return animations;
	}
	OBJECT(object)->x_speed = 0u - OBJECT(object)->x_speed;
	if (OBJECT(object)->enemy_shredder.target_dy >= 3u)
		OBJECT(object)->state -= 3u;
	move_object_blocked(object);
	return animations;
}

/* walks to his turtle */
void enemy_shredder_walk_to_target(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_052FE4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the walk to his turtle, facing it, aiming at once */
uint32_t enemy_shredder_walk_to_target_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_shredder.move_time = 1u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of the walk to his turtle, aiming every 8 frames, standing when close */
void enemy_shredder_walk_to_target_step(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_animations)
{
	uint32_t animations = *p_animations;
	uint16_t speed = 0;
	uint32_t direction = 0, turtle = 0, x_speed = 0;
	uint8_t move_time = 0;

	move_time = OBJECT(object)->enemy_shredder.move_time;
	OBJECT(object)->enemy_shredder.move_time = move_time - 1u;
	if (move_time == 1u)
	{
		turtle = PLAYER_SLOT(player_slot)->turtle;
		direction = direction_to_object(0u, object, turtle);
		animations = shredder_walk_animations_ADDRESS;
		if ((int8_t)OBJECT(object)->health_bars >= 0)
			animations = shredder_walk_hurt_animations_ADDRESS;
		enemy_turn_to_object((uint8_t)direction, object, animations, turtle);
		speed = 0x180u;
		speeds_from_direction((uint8_t)direction, speed, &x_speed, p_y_speed, &animations);
		OBJECT(object)->x_speed = x_speed;
		OBJECT(object)->y_speed = *p_y_speed;
		OBJECT(object)->enemy_shredder.move_time = 8u;
	}
	if (OBJECT(object)->enemy_shredder.target_dx >= 0x3Cu || OBJECT(object)->enemy_shredder.target_dy >= 0x20u)
		move_object_blocked(object);
	*p_animations = animations;
}

/* guards against his turtle's attack, then strikes back */
void enemy_shredder_guard(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05307C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* guards against his turtle's attack, not hittable, for 30 frames */
void enemy_shredder_guard_start(uint32_t object)
{
	uint32_t animation = 0;

	animation = animations_05B6EE_ADDRESS;
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
void enemy_shredder_back_off(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0530F6_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* 40 frames of walking backwards away from his turtle, still facing it */
void enemy_shredder_back_off_start(uint32_t object, uint32_t player_slot, uint32_t *p_y_speed, uint32_t *p_sines)
{
	uint8_t direction = 0;
	uint16_t speed = 0;
	uint32_t animations = 0, turtle = 0, x_speed = 0;

	OBJECT(object)->enemy_shredder.move_time = 0x40u;
	OBJECT(object)->enemy_shredder.timer = 0x28u;
	direction = OBJECT(object)->enemy_shredder.target_direction;
	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	enemy_face_object(direction, object, animations, turtle);
	direction -= 0x80u;
	speed = 0x100u;
	speeds_from_direction(direction, speed, &x_speed, p_y_speed, p_sines);
	OBJECT(object)->x_speed = x_speed;
	OBJECT(object)->y_speed = *p_y_speed;
	OBJECT(object)->state++;
}

/* a frame of walking away while his turtle is down; when the time runs out, his standing pose */
uint32_t enemy_shredder_back_off_walk(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t timer = 0;

	animations = shredder_walk_animations_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animations = shredder_walk_hurt_animations_ADDRESS;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
	move_object_blocked(object);
	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer != 1u)
	{
		enemy_shredder_back_off_wait(object, player_slot);
		return animations;
	}
	OBJECT(object)->state++;
	animation = animations_05B032_ADDRESS;
	if ((int8_t)OBJECT(object)->health_bars >= 0)
		animation = enemy_shredder_animation_ADDRESS;
	set_animation(0u, object, animation);
	return animations;
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
void enemy_shredder_lose_bar(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0531BA_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
uint32_t enemy_shredder_lose_bar_end(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint32_t ignored = 0, settings = 0;
	uint8_t timer = 0;

	enemy_shredder_flash(object);
	if (OBJECT(object)->frame_time != 0u)
	{
		timer = OBJECT(object)->enemy_shredder.timer;
		OBJECT(object)->enemy_shredder.timer = timer - 1u;
		if (timer != 1u)
			return record;
		record = shredder_piece_shot_record_ADDRESS;
		start_shot(object, record, &ignored, &zero);
		return record;
	}
	OBJECT(object)->colour = 0xFFu;
	settings = shredder_bar_health_ADDRESS;
	if ((int8_t)OBJECT(object)->parameter < 0)
		settings = shredder_clone_bar_health_ADDRESS;
	enemy_set_health(object, settings);
	enemy_back_to_thinking_hittable(object);
	return record;
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
void enemy_shredder_flinch(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05324E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
	animation = animations_05B4F2_ADDRESS;
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
void enemy_shredder_flinch_from_behind(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0532E0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
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
void enemy_shredder_defeated(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_053348_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* beaten: thrown along his arc, flashing, facing his turtle */
uint32_t enemy_shredder_defeated_start(uint32_t d7, uint32_t object, uint32_t target)
{
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->colour = 0xBu;
	enemy_boss_face_target(object, target);
	arc = enemy_shredder_defeated_arc_ADDRESS;
	d7 = enemy_start_arc(d7, object, arc);
	animation = enemy_shredder_defeated_start_animation_ADDRESS;
	set_animation(0u, object, animation);
	OBJECT(object)->contacts |= 0x80u;
	OBJECT(object)->state++;
	return d7;
}

/* a frame of the fall; on landing he slides */
uint32_t enemy_shredder_defeated_fall(uint32_t d7, uint32_t object)
{
	uint32_t zero = 0;
	uint32_t animation = 0, arc = 0;

	OBJECT(object)->colour = 0xFFu;
	move_object_with_height_blocked(object);
	arc = enemy_shredder_defeated_arc_ADDRESS;
	enemy_arc_step(object, arc);
	enemy_landed(object, &d7, &zero);
	if (zero == 0u)
		return d7;
	OBJECT(object)->contacts &= 0x7Fu;
	OBJECT(object)->state++;
	animation = enemy_shredder_defeated_fall_animation_ADDRESS;
	set_animation(0u, object, animation);
	return d7;
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
void enemy_shredder_vanish(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05344C_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* a clone beaten or told to vanish: gives up its side, in its flinch pose */
void enemy_shredder_vanish_start(uint32_t object, uint32_t player_slot)
{
	uint32_t animation = 0;

	OBJECT(object)->colour = 0xAu;
	enemy_leave_side(object, player_slot);
	OBJECT(object)->state++;
	animation = animations_05B4F2_ADDRESS;
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
void enemy_shredder_kick(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0534D0_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the kick's animation and damage, facing his turtle */
void enemy_shredder_kick_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = animations_05B91E_ADDRESS;
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
void enemy_shredder_thrust(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_053530_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the thrust's animation and damage, facing his turtle */
void enemy_shredder_thrust_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = animations_05BB0A_ADDRESS;
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
void enemy_shredder_slash(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05357A_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the slash that knocks down: its animation and damage, facing his turtle */
void enemy_shredder_slash_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = animations_05B73A_ADDRESS;
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
void enemy_shredder_sweep(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_0535C4_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the low sweep's animation and damage, facing his turtle */
void enemy_shredder_sweep_start(uint32_t object)
{
	uint8_t attack = 0;
	uint32_t animation = 0;

	animation = animations_05BD3E_ADDRESS;
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
void enemy_shredder_ray(uint32_t object, uint32_t slot, uint32_t *p_d5, uint32_t *p_d7, uint32_t *p_a5)
{
	uint32_t states = 0;

	states = indexed_table_of_32_bit_values_05360E_ADDRESS;
	run_object_state(object, slot, states, p_d5, p_d7, p_a5);
}

/* the ray: up to 240 frames of lining up with his turtle, facing it */
uint32_t enemy_shredder_ray_start(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0;
	uint32_t animations = 0, turtle = 0;

	OBJECT(object)->enemy_shredder.timer = 0xF0u;
	OBJECT(object)->state++;
	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	return enemy_face_object(target_direction, object, animations, turtle);
}

/* a frame of lining up: away across while nearer than 0x50, else to his turtle's row; level or out of
   time, he charges */
uint32_t enemy_shredder_ray_line_up(uint32_t object, uint32_t player_slot)
{
	uint8_t target_direction = 0, wall_mask = 0;
	uint32_t animation = 0, animations = 0, turtle = 0;
	uint8_t timer = 0;

	target_direction = OBJECT(object)->enemy_shredder.target_direction;
	turtle = PLAYER_SLOT(player_slot)->turtle;
	animations = shredder_walk_animations_ADDRESS;
	animations = enemy_turn_to_object(target_direction, object, animations, turtle);
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
			return animations;
		}
		return animations;
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
		return animations;
	}
	return animations;
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
uint32_t enemy_shredder_ray_fire(uint32_t object, uint32_t record)
{
	uint32_t zero = 0;
	uint8_t sound = 0;
	uint32_t animation = 0, ray = 0, ray_record = 0;
	uint8_t timer = 0;

	timer = OBJECT(object)->enemy_shredder.timer;
	OBJECT(object)->enemy_shredder.timer = timer - 1u;
	if (timer != 1u)
		return record;
	OBJECT(object)->flags &= 0xBFu;
	if ((int8_t)(OBJECT(object)->enemy_shredder.target_direction - 0x40u) < 0)
		OBJECT(object)->flags |= 0x40u;
	ray_record = shredder_ray_shot_record_ADDRESS;
	start_shot(object, ray_record, &ray, &zero);
	OBJECT(ray)->shredder_ray.shredder = object;
	sound = 0x1Cu;
	queue_sound(sound);
	OBJECT(object)->enemy_shredder.timer = 0x3Cu;
	OBJECT(object)->state++;
	animation = enemy_shredder_ray_fire_animation_ADDRESS;
	set_animation(0u, object, animation);
	return ray_record;
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
