/**
 * @file ta_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ta_debug_writer.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_traj_predictor_t.h"
#include "pa_vehicle_in.h"
#include "ta_debug_interface.h"
#include "ta_types.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Ta_Write_Bin_File(void)
{
   uint8_t index;
   const uint8_t ta_array_two = 2u;

   /* Get debug data. */
   Ta_Debug_Data_T *ta_debug_data = Ta_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != ta_debug_data);

   /* Log the TA core input. */
   TA_STORE_VAL_MGR_WPR("ta_core_in_f_ta_enable", ta_debug_data->ta_core_input.f_ta_enable);
   TA_STORE_VAL_MGR_WPR("ta_core_in_f_fta_enable", ta_debug_data->ta_core_input.f_fta_enable);
   TA_STORE_VAL_MGR_WPR("ta_core_in_f_rta_enable", ta_debug_data->ta_core_input.f_rta_enable);
   TA_STORE_VAL_MGR_WPR("ta_core_in_f_enable_debug_mode", ta_debug_data->ta_core_input.f_enable_debug_mode);
   TA_STORE_VAL_MGR_WPR("ta_core_in_debug_mode_obj_pos_long_offset", ta_debug_data->ta_core_input.debug_mode_obj_pos_long_offset);
   TA_STORE_VAL_MGR_WPR("ta_core_in_debug_mode_obj_pos_lat_offset", ta_debug_data->ta_core_input.debug_mode_obj_pos_lat_offset);

   /* Log the TA core output. */
   TA_STORE_VAL_MGR_WPR("ta_core_out_most_critical_side", ta_debug_data->ta_core_output.ta_most_critical_side);

   TA_STORE_VAL_MGR_WPR("ta_core_out_n_valid_objects", ta_debug_data->ta_core_output.ta_n_valid_objects);
   TA_STORE_VAL_MGR_WPR("ta_core_out_n_relevant_objects", ta_debug_data->ta_core_output.ta_n_relevant_objects);
   TA_STORE_VAL_MGR_WPR("ta_core_out_n_critical_objects", ta_debug_data->ta_core_output.ta_n_critical_objects);

   for (index = FBK_ZERO_UINT; index < FBK_NUMBER_OF_SIDES; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_waypoint_at_collision_x",
                                  ta_debug_data->ta_core_output.ta_waypoint_at_collision[index].x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_waypoint_at_collision_y",
                                  ta_debug_data->ta_core_output.ta_waypoint_at_collision[index].y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_ttc", ta_debug_data->ta_core_output.ta_ttc[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_ttp", ta_debug_data->ta_core_output.ta_ttp[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_ttb", ta_debug_data->ta_core_output.ta_ttb[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_decel_estimate", ta_debug_data->ta_core_output.ta_decel_estimate[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_distance", ta_debug_data->ta_core_output.ta_distance[index], index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_alert_level", ta_debug_data->ta_core_output.ta_alert_level[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_id", ta_debug_data->ta_core_output.ta_id[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_index", ta_debug_data->ta_core_output.ta_index[index], index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_f_obj_in_danger_zone", ta_debug_data->ta_core_output.ta_f_obj_in_danger_zone[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_f_obj_in_info_zone", ta_debug_data->ta_core_output.ta_f_obj_in_info_zone[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_core_out_f_obj_in_wing_zone", ta_debug_data->ta_core_output.ta_f_obj_in_wing_zone[index], index);
   }

   TA_STORE_VAL_MGR_WPR("f_fta_obj_in_danger_zone_left", ta_debug_data->ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_LEFT]);
   TA_STORE_VAL_MGR_WPR("f_fta_obj_in_danger_zone_right", ta_debug_data->ta_core_output.ta_f_obj_in_danger_zone[FBK_SIDE_RIGHT]);
   TA_STORE_VAL_MGR_WPR("f_rta_obj_in_dynamic_area_left", ta_debug_data->ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_LEFT]);
   TA_STORE_VAL_MGR_WPR("f_rta_obj_in_dynamic_area_right", ta_debug_data->ta_core_output.ta_f_obj_in_info_zone[FBK_SIDE_RIGHT]);
   TA_STORE_VAL_MGR_WPR("f_rta_obj_in_turning_area_left", ta_debug_data->ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_LEFT]);
   TA_STORE_VAL_MGR_WPR("f_rta_obj_in_turning_area_right", ta_debug_data->ta_core_output.ta_f_obj_in_wing_zone[FBK_SIDE_RIGHT]);

   /* Log the persistent data.*/
   for (index = FBK_ZERO_UINT; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_persistent_ta_alert_mode", ta_debug_data->ta_persistent.ta_alert_mode[index], index);
   }

   for (index = FBK_ZERO_UINT; index < FBK_NUMBER_OF_SIDES; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_persistent_side_alert_prev_cycle",
                                  ta_debug_data->ta_persistent.ta_side_alert_prev_cycle[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_persistent_side_id_prev_cycle", ta_debug_data->ta_persistent.ta_side_id_prev_cycle[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_persistent_index_prev_cycle", ta_debug_data->ta_persistent.ta_side_index_prev_cycle[index],
                                  index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_persistent_side_alert_qualifying_counter",
                                  ta_debug_data->ta_persistent.ta_side_alert_qualifying_counter[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_persistent_side_alert_holding_counter",
                                  ta_debug_data->ta_persistent.ta_side_alert_holding_counter[index], index);
   }

   /* Write additional persistent debug output */
   TA_STORE_VAL_MGR_WPR("ta_persistent_pred_step_dt", ta_debug_data->ta_persistent.ta_pred_step_dt);
   TA_STORE_VAL_MGR_WPR("ta_persistent_ego_yaw_angle_to_last_straight_section",
                        ta_debug_data->ta_persistent.ta_ego_yaw_angle_to_last_straight_section);

   /* Cal & SW version */
   TA_STORE_VAL_MGR_WPR("k_ta_cal_version", ta_debug_data->ta_calibration.Header.version);
   TA_STORE_VAL_MGR_WPR("Ta_Sw_Major_Version", ta_debug_data->ta_version.ta_sw_major_version);
   TA_STORE_VAL_MGR_WPR("Ta_Sw_Minor_Version", ta_debug_data->ta_version.ta_sw_minor_version);

   TA_STORE_VAL_MGR_WPR("k_ta_alert_qualifying_cycles", ta_debug_data->ta_calibration.k_ta_alert_qualifying_cycles);
   TA_STORE_VAL_MGR_WPR("k_ta_alert_holding_cycles", ta_debug_data->ta_calibration.k_ta_alert_holding_cycles);
   TA_STORE_VAL_MGR_WPR("k_ta_prediction_steps_max", ta_debug_data->ta_calibration.k_ta_prediction_steps_max);
   TA_STORE_VAL_MGR_WPR("k_ta_always_overwrite_ta_mode_to_both", ta_debug_data->ta_calibration.k_ta_always_overwrite_ta_mode_to_both);

   for (index = FBK_ZERO_UINT; index < TA_K_TA_ALERT_LVL_1_TTP_THRESHOLD_ARRAY_SIZE_DIM0; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_alert_lvl_1_ttp_threshold",
                                  ta_debug_data->ta_calibration.k_ta_alert_lvl_1_ttp_threshold[index], index);
   }
   TA_STORE_VAL_MGR_WPR("k_ta_alert_lvl_2_ttc_threshold", ta_debug_data->ta_calibration.k_ta_alert_lvl_2_ttc_threshold);
   TA_STORE_VAL_MGR_WPR("k_ta_alert_lvl_3_ttc_threshold", ta_debug_data->ta_calibration.k_ta_alert_lvl_3_ttc_threshold);
   TA_STORE_VAL_MGR_WPR("k_ta_alert_lvl_3_ttb_threshold", ta_debug_data->ta_calibration.k_ta_alert_lvl_3_ttb_threshold);
   TA_STORE_VAL_MGR_WPR("k_ta_alert_lvl_4_ttc_threshold", ta_debug_data->ta_calibration.k_ta_alert_lvl_4_ttc_threshold);
   TA_STORE_VAL_MGR_WPR("k_ta_alert_lvl_4_decel_threshold", ta_debug_data->ta_calibration.k_ta_alert_lvl_4_decel_threshold);
   TA_STORE_VAL_MGR_WPR("k_ta_active_obj_ttp_offset", ta_debug_data->ta_calibration.k_ta_active_obj_ttp_offset);
   TA_STORE_VAL_MGR_WPR("k_pfgs_qualification_counter_slow_obj", ta_debug_data->ta_calibration.k_pfgs_qualification_counter_slow_obj);
   TA_STORE_VAL_MGR_WPR("k_pfgs_qualification_counter_fast_obj", ta_debug_data->ta_calibration.k_pfgs_qualification_counter_fast_obj);
   TA_STORE_VAL_MGR_WPR("k_pfgs_qualification_ttc_min", ta_debug_data->ta_calibration.k_pfgs_qualification_ttc_min);
   TA_STORE_VAL_MGR_WPR("k_pfgs_qualification_check_f_stationary",
                        ta_debug_data->ta_calibration.k_pfgs_qualification_check_f_stationary);
   TA_STORE_VAL_MGR_WPR("k_pfgs_symbol_request_sides_enabled", ta_debug_data->ta_calibration.k_pfgs_symbol_request_sides_enabled);
   TA_STORE_VAL_MGR_WPR("k_tap_lvl_2_host_curvature_min", ta_debug_data->ta_calibration.k_tap_lvl_2_host_curvature_min);

   for (index = 0; index < TA_K_PFGS_EGO_SPEED_ARRAY_SIZE_DIM0; ++index)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_pfgs_ego_speed", ta_debug_data->ta_calibration.k_pfgs_ego_speed[index], index);
   }


   /* Flags */
   TA_STORE_VAL_MGR_WPR("k_f_ta_enable_debug_mode", ta_debug_data->ta_calibration.k_f_ta_enable_debug_mode);
   TA_STORE_VAL_MGR_WPR("k_f_fta_enable", ta_debug_data->ta_calibration.k_f_fta_enable);
   TA_STORE_VAL_MGR_WPR("k_f_rta_enable", ta_debug_data->ta_calibration.k_f_rta_enable);
   TA_STORE_VAL_MGR_WPR("k_ta_f_only_allow_consecutive_ttc_based_alert_levels",
                        ta_debug_data->ta_calibration.k_ta_f_only_allow_consecutive_ttc_based_alert_levels);
   TA_STORE_VAL_MGR_WPR("k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj",
                        ta_debug_data->ta_calibration.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj);
   TA_STORE_VAL_MGR_WPR("k_ta_f_skip_holding_for_single_alert_level_drop",
                        ta_debug_data->ta_calibration.k_ta_f_skip_holding_for_single_alert_level_drop);
   TA_STORE_VAL_MGR_WPR("k_rta_f_higher_obj_crit_based_on_lower_ttp",
                        ta_debug_data->ta_calibration.k_rta_f_higher_obj_crit_based_on_lower_ttp);

   TA_STORE_VAL_MGR_WPR("k_ta_critical_approach_angle_diff_min", ta_debug_data->ta_calibration.k_ta_critical_approach_angle_diff_min);
   TA_STORE_VAL_MGR_WPR("k_ta_critical_approach_min_safe_distance",
                        ta_debug_data->ta_calibration.k_ta_critical_approach_min_safe_distance);
   TA_STORE_VAL_MGR_WPR("k_ta_critical_approach_check_ego_circles_front",
                        ta_debug_data->ta_calibration.k_ta_critical_approach_check_ego_circles[TA_CIRCLE_FRONT]);
   TA_STORE_VAL_MGR_WPR("k_ta_critical_approach_check_ego_circles_middle",
                        ta_debug_data->ta_calibration.k_ta_critical_approach_check_ego_circles[TA_CIRCLE_MIDDLE]);
   TA_STORE_VAL_MGR_WPR("k_ta_critical_approach_check_ego_circles_rear",
                        ta_debug_data->ta_calibration.k_ta_critical_approach_check_ego_circles[TA_CIRCLE_REAR]);

   TA_STORE_VAL_MGR_WPR("k_ta_obj_pred_speed_min", ta_debug_data->ta_calibration.k_ta_obj_pred_speed_min);
   TA_STORE_VAL_MGR_WPR("k_ta_obj_acceleration_long_weight", ta_debug_data->ta_calibration.k_ta_obj_acceleration_long_weight);
   TA_STORE_VAL_MGR_WPR("k_ta_obj_acceleration_lat_weight", ta_debug_data->ta_calibration.k_ta_obj_acceleration_lat_weight);

   TA_STORE_VAL_MGR_WPR("k_ta_ego_shape_gain_per_pred_step", ta_debug_data->ta_calibration.k_ta_ego_shape_gain_per_pred_step);
   TA_STORE_VAL_MGR_WPR("k_ta_obj_shape_gain_per_pred_step", ta_debug_data->ta_calibration.k_ta_obj_shape_gain_per_pred_step);
   TA_STORE_VAL_MGR_WPR("k_ta_ego_shape_gain_fixed", ta_debug_data->ta_calibration.k_ta_ego_shape_gain_fixed);
   TA_STORE_VAL_MGR_WPR("k_ta_obj_shape_gain_fixed", ta_debug_data->ta_calibration.k_ta_obj_shape_gain_fixed);

   TA_STORE_VAL_MGR_WPR("k_ta_ego_acceleration_weight", ta_debug_data->ta_calibration.k_ta_ego_acceleration_weight);
   TA_STORE_VAL_MGR_WPR("k_ta_ego_deceleration_weight", ta_debug_data->ta_calibration.k_ta_ego_deceleration_weight);
   TA_STORE_VAL_MGR_WPR("k_ta_ego_pred_const_velocity_pred_steps_min",
                        ta_debug_data->ta_calibration.k_ta_ego_pred_const_velocity_pred_steps_min);

   TA_STORE_VAL_MGR_WPR("k_ta_ego_yawangle_integration_yawrate_min",
                        ta_debug_data->ta_calibration.k_ta_ego_yawangle_integration_yawrate_min);
   TA_STORE_VAL_MGR_WPR("k_ta_ego_max_pred_yaw_angle", ta_debug_data->ta_calibration.k_ta_ego_max_pred_yaw_angle);

   TA_STORE_VAL_MGR_WPR("k_fta_brake_deceleration_max", ta_debug_data->ta_calibration.k_fta_brake_deceleration_max);
   TA_STORE_VAL_MGR_WPR("k_fta_brake_dead_time", ta_debug_data->ta_calibration.k_fta_brake_dead_time);

   TA_STORE_VAL_MGR_WPR("k_f_fta_enable_brake_gradient_logic", ta_debug_data->ta_calibration.k_f_fta_enable_brake_gradient_logic);
   TA_STORE_VAL_MGR_WPR("k_fta_brake_gradient", ta_debug_data->ta_calibration.k_fta_brake_gradient);

   TA_STORE_VAL_MGR_WPR("k_ta_straight_host_curvature_max", ta_debug_data->ta_calibration.k_ta_straight_host_curvature_max);
   for (index = FBK_ZERO_UINT; index < TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_lookup_turning_host_curvature_min",
                                  ta_debug_data->ta_calibration.k_ta_lookup_turning_host_curvature_min[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_lookup_turning_host_speed",
                                  ta_debug_data->ta_calibration.k_ta_lookup_turning_host_speed[index], index);
   }

   TA_STORE_VAL_MGR_WPR("k_rta_ttp_obj_abs_lat_vel_rel_max", ta_debug_data->ta_calibration.k_rta_ttp_obj_abs_lat_vel_rel_max);
   TA_STORE_VAL_MGR_WPR("k_rta_ttp_obj_abs_heading_diff_max", ta_debug_data->ta_calibration.k_rta_ttp_obj_abs_heading_diff_max);
   TA_STORE_VAL_MGR_WPR("k_rta_ttp_curve_suppression_obj_distance_min",
                        ta_debug_data->ta_calibration.k_rta_ttp_curve_suppression_obj_distance_min);

   /* Zones */
   TA_STORE_VAL_MGR_WPR("k_f_fta_enable_danger_zones", ta_debug_data->ta_calibration.k_f_fta_enable_danger_zones);
   TA_STORE_VAL_MGR_WPR("k_fta_danger_zone_point_size", ta_debug_data->ta_calibration.k_fta_danger_zone_point_size);
   TA_STORE_VAL_MGR_WPR("k_f_rta_enable_info_zones", ta_debug_data->ta_calibration.k_f_rta_enable_info_zones);
   TA_STORE_VAL_MGR_WPR("k_rta_info_zone_point_size", ta_debug_data->ta_calibration.k_rta_info_zone_point_size);
   TA_STORE_VAL_MGR_WPR("k_f_rta_enable_wing_zones", ta_debug_data->ta_calibration.k_f_rta_enable_wing_zones);
   TA_STORE_VAL_MGR_WPR("k_rta_wing_zone_point_size", ta_debug_data->ta_calibration.k_rta_wing_zone_point_size);

   TA_STORE_VAL_MGR_WPR("k_fta_obj_age_min", ta_debug_data->ta_calibration.k_fta_obj_age_min);
   TA_STORE_VAL_MGR_WPR("k_rta_obj_age_min", ta_debug_data->ta_calibration.k_rta_obj_age_min);
   TA_STORE_VAL_MGR_WPR("k_fta_obj_velocity_heading_diff_max", ta_debug_data->ta_calibration.k_fta_obj_velocity_heading_diff_max);

   TA_STORE_VAL_MGR_WPR("k_fta_obj_vcs_long_pos_straight_min", ta_debug_data->ta_calibration.k_fta_obj_vcs_long_pos_straight_min);

   for (index = FBK_ZERO_UINT; index < ta_array_two; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_ego_speed", ta_debug_data->ta_calibration.k_ta_ego_speed[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_ego_speed_ofst", ta_debug_data->ta_calibration.k_ta_ego_speed_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_ego_yawrate", ta_debug_data->ta_calibration.k_ta_ego_yawrate[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_ego_yawrate_ofst", ta_debug_data->ta_calibration.k_ta_ego_yawrate_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_ego_long_acceleration", ta_debug_data->ta_calibration.k_ta_ego_long_acceleration[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_ta_ego_long_acceleration_ofst",
                                  ta_debug_data->ta_calibration.k_ta_ego_long_acceleration_ofst[index], index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_exist_prblty", ta_debug_data->ta_calibration.k_fta_obj_exist_prblty[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_exist_prblty_ofst", ta_debug_data->ta_calibration.k_fta_obj_exist_prblty_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_long_vel_rel", ta_debug_data->ta_calibration.k_fta_obj_vcs_long_vel_rel[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_long_vel_rel_ofst",
                                  ta_debug_data->ta_calibration.k_fta_obj_vcs_long_vel_rel_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_lat_vel_rel", ta_debug_data->ta_calibration.k_fta_obj_vcs_lat_vel_rel[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_lat_vel_rel_ofst",
                                  ta_debug_data->ta_calibration.k_fta_obj_vcs_lat_vel_rel_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_long_vel", ta_debug_data->ta_calibration.k_fta_obj_vcs_long_vel[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_long_vel_ofst", ta_debug_data->ta_calibration.k_fta_obj_vcs_long_vel_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_lat_vel", ta_debug_data->ta_calibration.k_fta_obj_vcs_lat_vel[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vcs_lat_vel_ofst", ta_debug_data->ta_calibration.k_fta_obj_vcs_lat_vel_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_heading", ta_debug_data->ta_calibration.k_fta_obj_heading[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_heading_straight", ta_debug_data->ta_calibration.k_fta_obj_heading_straight[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_heading_ofst", ta_debug_data->ta_calibration.k_fta_obj_heading_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_heading_rate", ta_debug_data->ta_calibration.k_fta_obj_heading_rate[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_heading_rate_straight",
                                  ta_debug_data->ta_calibration.k_fta_obj_heading_rate_straight[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_heading_rate_ofst", ta_debug_data->ta_calibration.k_fta_obj_heading_rate_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_speed", ta_debug_data->ta_calibration.k_fta_obj_speed[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_speed_straight", ta_debug_data->ta_calibration.k_fta_obj_speed_straight[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_speed_ofst", ta_debug_data->ta_calibration.k_fta_obj_speed_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vru_class_prob", ta_debug_data->ta_calibration.k_fta_obj_vru_class_prob[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_vru_class_prob_ofst",
                                  ta_debug_data->ta_calibration.k_fta_obj_vru_class_prob_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_length", ta_debug_data->ta_calibration.k_fta_obj_length[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_length_ofst", ta_debug_data->ta_calibration.k_fta_obj_length_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_width", ta_debug_data->ta_calibration.k_fta_obj_width[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_width_ofst", ta_debug_data->ta_calibration.k_fta_obj_width_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_area", ta_debug_data->ta_calibration.k_fta_obj_area[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_area_ofst", ta_debug_data->ta_calibration.k_fta_obj_area_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_ego_obj_heading_diff", ta_debug_data->ta_calibration.k_fta_ego_obj_heading_diff[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_ego_obj_heading_diff_ofst",
                                  ta_debug_data->ta_calibration.k_fta_ego_obj_heading_diff_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_eclipse_value", ta_debug_data->ta_calibration.k_fta_obj_eclipse_value[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_obj_eclipse_value_ofst",
                                  ta_debug_data->ta_calibration.k_fta_obj_eclipse_value_ofst[index], index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_exist_prblty", ta_debug_data->ta_calibration.k_rta_obj_exist_prblty[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_exist_prblty_ofst", ta_debug_data->ta_calibration.k_rta_obj_exist_prblty_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_long_vel_rel", ta_debug_data->ta_calibration.k_rta_obj_vcs_long_vel_rel[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_long_vel_rel_ofst",
                                  ta_debug_data->ta_calibration.k_rta_obj_vcs_long_vel_rel_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_lat_vel_rel", ta_debug_data->ta_calibration.k_rta_obj_vcs_lat_vel_rel[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_lat_vel_rel_ofst",
                                  ta_debug_data->ta_calibration.k_rta_obj_vcs_lat_vel_rel_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_long_vel", ta_debug_data->ta_calibration.k_rta_obj_vcs_long_vel[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_long_vel_ofst", ta_debug_data->ta_calibration.k_rta_obj_vcs_long_vel_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_lat_vel", ta_debug_data->ta_calibration.k_rta_obj_vcs_lat_vel[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vcs_lat_vel_ofst", ta_debug_data->ta_calibration.k_rta_obj_vcs_lat_vel_ofst[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_heading", ta_debug_data->ta_calibration.k_rta_obj_heading[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_heading_ofst", ta_debug_data->ta_calibration.k_rta_obj_heading_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_speed", ta_debug_data->ta_calibration.k_rta_obj_speed[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_speed_ofst", ta_debug_data->ta_calibration.k_rta_obj_speed_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vru_class_prob", ta_debug_data->ta_calibration.k_rta_obj_vru_class_prob[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_vru_class_prob_ofst",
                                  ta_debug_data->ta_calibration.k_rta_obj_vru_class_prob_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_length", ta_debug_data->ta_calibration.k_rta_obj_length[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_length_ofst", ta_debug_data->ta_calibration.k_rta_obj_length_ofst[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_width", ta_debug_data->ta_calibration.k_rta_obj_width[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_obj_width_ofst", ta_debug_data->ta_calibration.k_rta_obj_width_ofst[index], index);
   }

   /* Iterate over arrays with four entries */
   for (index = FBK_ZERO_UINT; index < ta_debug_data->ta_calibration.k_fta_danger_zone_point_size; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_danger_zone_left_long", ta_debug_data->ta_calibration.k_fta_danger_zone_left_long[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_danger_zone_left_lat", ta_debug_data->ta_calibration.k_fta_danger_zone_left_lat[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_danger_zone_right_long",
                                  ta_debug_data->ta_calibration.k_fta_danger_zone_right_long[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_fta_danger_zone_right_lat", ta_debug_data->ta_calibration.k_fta_danger_zone_right_lat[index],
                                  index);
   }
   for (index = FBK_ZERO_UINT; index < ta_debug_data->ta_calibration.k_rta_info_zone_point_size; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_left_long", ta_debug_data->ta_calibration.k_rta_info_zone_left_long[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_left_lat", ta_debug_data->ta_calibration.k_rta_info_zone_left_lat[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_right_long", ta_debug_data->ta_calibration.k_rta_info_zone_right_long[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_right_lat", ta_debug_data->ta_calibration.k_rta_info_zone_right_lat[index], index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_left_long_hys",
                                  ta_debug_data->ta_calibration.k_rta_info_zone_left_long_hys[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_left_lat_hys",
                                  ta_debug_data->ta_calibration.k_rta_info_zone_left_lat_hys[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_right_long_hys",
                                  ta_debug_data->ta_calibration.k_rta_info_zone_right_long_hys[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_info_zone_right_lat_hys",
                                  ta_debug_data->ta_calibration.k_rta_info_zone_right_lat_hys[index], index);
   }
   for (index = FBK_ZERO_UINT; index < ta_debug_data->ta_calibration.k_rta_wing_zone_point_size; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_left_long", ta_debug_data->ta_calibration.k_rta_wing_zone_left_long[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_left_lat", ta_debug_data->ta_calibration.k_rta_wing_zone_left_lat[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_right_long", ta_debug_data->ta_calibration.k_rta_wing_zone_right_long[index],
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_right_lat", ta_debug_data->ta_calibration.k_rta_wing_zone_right_lat[index], index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_left_long_hys",
                                  ta_debug_data->ta_calibration.k_rta_wing_zone_left_long_hys[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_left_lat_hys",
                                  ta_debug_data->ta_calibration.k_rta_wing_zone_left_lat_hys[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_right_long_hys",
                                  ta_debug_data->ta_calibration.k_rta_wing_zone_right_long_hys[index], index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("k_rta_wing_zone_right_lat_hys",
                                  ta_debug_data->ta_calibration.k_rta_wing_zone_right_lat_hys[index], index);
   }

   /* Write Ta ego trajectory data */
   TA_STORE_VAL_MGR_WPR("ego_trajectory_valid", ta_debug_data->ta_debug_output.ego_trajectory.f_trajectory_valid);
   TA_STORE_VAL_MGR_WPR("ego_trajectory_n_prediction_steps", ta_debug_data->ta_debug_output.ego_trajectory.n_prediction_steps);

   for (index = FBK_ZERO_UINT; index < FBK_MAX_PREDICTION_STEPS; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_waypoint_coordinates_x",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].waypoint_coordinates.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_waypoint_coordinates_y",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].waypoint_coordinates.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_waypoint_yaw_angle",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].waypoint_yaw_angle.angle, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_waypoint_speed",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].waypoint_speed, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_center_front_x",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_center_front.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_center_front_y",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_center_front.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_center_middle_x",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_center_middle.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_center_middle_y",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_center_middle.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_center_rear_x",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_center_rear.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_center_rear_y",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_center_rear.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_circle_radius",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].circle_radius, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ego_trajectory_waypoint_valid",
                                  ta_debug_data->ta_debug_output.ego_trajectory.waypoint[index].f_waypoint_valid, index);
   }

   /* Write Ta object data */
   for (index = FBK_ZERO_UINT; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_alert_level", ta_debug_data->ta_object_attributes[index].alert_level, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_alert_side", ta_debug_data->ta_object_attributes[index].alert_side, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_ttc", ta_debug_data->ta_object_attributes[index].ttc, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_ttp", ta_debug_data->ta_object_attributes[index].ttp, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_ttb", ta_debug_data->ta_object_attributes[index].ttb, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_decel_to_avoid_coll", ta_debug_data->ta_object_attributes[index].decel_to_avoid_coll, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_distance_to_ego", ta_debug_data->ta_object_attributes[index].distance_to_ego, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_ta_alert_mode", ta_debug_data->ta_object_attributes[index].ta_alert_mode, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_ego_heading_diff", ta_debug_data->ta_object_attributes[index].ego_heading_diff, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_velocity_heading", ta_debug_data->ta_object_attributes[index].velocity_heading, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_f_curvi_available", ta_debug_data->ta_object_attributes[index].f_curvi_available, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_f_vehicle_state_relevant",
                                  ta_debug_data->ta_object_attributes[index].f_vehicle_state_relevant, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_f_obj_ta_relevant", ta_debug_data->ta_object_attributes[index].f_obj_ta_relevant, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_f_obj_in_danger_zone", ta_debug_data->ta_object_attributes[index].f_obj_in_danger_zone,
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_f_obj_in_info_zone", ta_debug_data->ta_object_attributes[index].f_obj_in_info_zone, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_f_obj_in_wing_zone", ta_debug_data->ta_object_attributes[index].f_obj_in_wing_zone, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_waypoint_at_collision_x",
                                  ta_debug_data->ta_object_attributes[index].waypoint_at_collision.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_waypoint_at_collision_y",
                                  ta_debug_data->ta_object_attributes[index].waypoint_at_collision.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_vru_class_prob", ta_debug_data->ta_object_attributes[index].object_class_probability_vru,
                                  index);
      TA_STORE_ARRAY_ELEM_MGR_WPR("ta_obj_area", ta_debug_data->ta_object_attributes[index].area, index);
   }

   /* Write Ta obj trajectory data for most critical object or potentially critical object on left side */
   TA_STORE_VAL_MGR_WPR("obj_traj_left_valid", ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.f_trajectory_valid);
   TA_STORE_VAL_MGR_WPR("obj_traj_left_n_prediction_steps",
                        ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.n_prediction_steps);

   for (index = FBK_ZERO_UINT; index < FBK_MAX_PREDICTION_STEPS; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_waypoint_coordinates_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_coordinates.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_waypoint_coordinates_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_coordinates.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_waypoint_yaw_angle",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_yaw_angle.angle, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("obj_traj_left_waypoint_speed",
                                  ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].waypoint_speed,
                                  index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_circle_center_front_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_front.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_circle_center_front_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_front.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_circle_center_middle_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_middle.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_circle_center_middle_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_middle.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_circle_center_rear_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_rear.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_left_circle_center_rear_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_center_rear.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("obj_traj_left_circle_radius",
                                  ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].circle_radius,
                                  index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("obj_traj_left_waypoint_valid",
                                  ta_debug_data->ta_debug_output.most_crit_object_trajectory_left.waypoint[index].f_waypoint_valid,
                                  index);
   }

   /* Write Ta obj trajectory data for most critical object or potentially critical object on right side */
   TA_STORE_VAL_MGR_WPR("obj_traj_right_valid", ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.f_trajectory_valid);
   TA_STORE_VAL_MGR_WPR("obj_traj_right_n_prediction_steps",
                        ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.n_prediction_steps);

   for (index = FBK_ZERO_UINT; index < FBK_MAX_PREDICTION_STEPS; index++)
   {
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_waypoint_coordinates_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_coordinates.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_waypoint_coordinates_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_coordinates.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_waypoint_yaw_angle",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_yaw_angle.angle, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("obj_traj_right_waypoint_speed",
                                  ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].waypoint_speed,
                                  index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_circle_center_front_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_front.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_circle_center_front_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_front.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_circle_center_middle_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_middle.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_circle_center_middle_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_middle.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_circle_center_rear_x",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_rear.x, index);
      TA_STORE_ARRAY_ELEM_MGR_WPR(
         "obj_traj_right_circle_center_rear_y",
         ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_center_rear.y, index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("obj_traj_right_circle_radius",
                                  ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].circle_radius,
                                  index);

      TA_STORE_ARRAY_ELEM_MGR_WPR("obj_traj_right_waypoint_valid",
                                  ta_debug_data->ta_debug_output.most_crit_object_trajectory_right.waypoint[index].f_waypoint_valid,
                                  index);
   }
}

#endif /* BINARY_DEBUG */
