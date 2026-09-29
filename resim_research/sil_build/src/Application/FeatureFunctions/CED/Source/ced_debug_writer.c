/**
 * @file ced_debug_writer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for writing out debug information into bin files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced_debug_writer.h"
#include "ced_debug_interface.h"
#include "ced_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include "pt_output_t.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#ifdef BINARY_DEBUG

void Ced_Write_Bin_File(void)
{
   uint8_t i;

   /* Get debug data. */
   Ced_Debug_Data_T *ced_debug_data = Ced_Get_Debug_Data();

   /* Check input parameters. */
   assert(NULL != ced_debug_data);

   /* Log the CED core input. */
   CED_STORE_VAL_MGR_WPR("ced_core_in_f_ced_enable", ced_debug_data->ced_core_input.f_ced_enable);
   CED_STORE_VAL_MGR_WPR("ced_core_in_f_ced_front_mode", ced_debug_data->ced_core_input.f_ced_front_mode);
   CED_STORE_VAL_MGR_WPR("ced_core_in_f_ced_rear_mode", ced_debug_data->ced_core_input.f_ced_rear_mode);

   /* Log the CED core output. */
   CED_STORE_VAL_MGR_WPR("ced_core_out_id_left", ced_debug_data->ced_core_output.ced_id[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_id_right", ced_debug_data->ced_core_output.ced_id[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_index_left", ced_debug_data->ced_core_output.ced_index[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_index_right", ced_debug_data->ced_core_output.ced_index[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_ttc_left", ced_debug_data->ced_core_output.ced_ttc[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_ttc_right", ced_debug_data->ced_core_output.ced_ttc[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_ttp_left", ced_debug_data->ced_core_output.ced_ttp[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_ttp_right", ced_debug_data->ced_core_output.ced_ttp[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_alert_left", ced_debug_data->ced_core_output.ced_alert[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_alert_right", ced_debug_data->ced_core_output.ced_alert[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_direction_left", ced_debug_data->ced_core_output.ced_object_direction[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_direction_right", ced_debug_data->ced_core_output.ced_object_direction[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_path_match_index_left",
                         ced_debug_data->ced_core_output.ced_object_path_match_index[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_path_match_index_right",
                         ced_debug_data->ced_core_output.ced_object_path_match_index[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_predicted_lat_pos_left",
                         ced_debug_data->ced_core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_predicted_lat_pos_right",
                         ced_debug_data->ced_core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_front_bumper_pos_long_left",
                         ced_debug_data->ced_core_output.ced_front_bumper_pos_long[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_front_bumper_pos_long_right",
                         ced_debug_data->ced_core_output.ced_front_bumper_pos_long[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_vcs_vel_rel_x_left", ced_debug_data->ced_core_output.ced_vcs_vel_rel_x[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_core_out_object_vcs_vel_rel_x_right", ced_debug_data->ced_core_output.ced_vcs_vel_rel_x[FBK_SIDE_RIGHT]);

   /* Log the persistent data.*/
   CED_STORE_VAL_MGR_WPR("ced_pers_side_alert_qualifying_counter_left",
                         ced_debug_data->ced_persistent.ced_side_alert_qualifying_counter[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_pers_side_alert_qualifying_counter_right",
                         ced_debug_data->ced_persistent.ced_side_alert_qualifying_counter[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_pers_side_alert_holding_counter_left",
                         ced_debug_data->ced_persistent.ced_side_alert_holding_counter[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_pers_side_alert_holding_counter_right",
                         ced_debug_data->ced_persistent.ced_side_alert_holding_counter[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_pers_side_id_prev_cycle_left", ced_debug_data->ced_persistent.ced_side_id_prev_cycle[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_pers_side_id_prev_cycle_right", ced_debug_data->ced_persistent.ced_side_id_prev_cycle[FBK_SIDE_RIGHT]);

   for (i = 0; i < CED_OBJ_MAX_ARRAY_SIZE; i++)
   {
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_pers_object_heading_predicted",
                                   ced_debug_data->ced_persistent.ced_object_heading_predicted[i], i);
   }

   /* Cal & SW version */
   CED_STORE_VAL_MGR_WPR("k_ced_cal_version", ced_debug_data->ced_calibration.Header.version);
   CED_STORE_VAL_MGR_WPR("Ced_Sw_Major_Version", ced_debug_data->ced_version.ced_sw_major_version);
   CED_STORE_VAL_MGR_WPR("Ced_Sw_Minor_Version", ced_debug_data->ced_version.ced_sw_minor_version);

   /* object filter */
   CED_STORE_VAL_MGR_WPR("k_ced_ego_abs_speed_max", ced_debug_data->ced_calibration.k_ced_ego_abs_speed_max);
   CED_STORE_VAL_MGR_WPR("k_ced_object_width_safety_margin_for_active_alert",
                         ced_debug_data->ced_calibration.k_ced_object_width_safety_margin_for_active_alert);
   CED_STORE_VAL_MGR_WPR("k_ced_object_width_safety_margin_for_critical_path_match",
                         ced_debug_data->ced_calibration.k_ced_object_width_safety_margin_for_critical_path_match);
   CED_STORE_VAL_MGR_WPR("k_ced_object_min_dist_to_crash_line_for_path_match",
                         ced_debug_data->ced_calibration.k_ced_object_min_dist_to_crash_line_for_path_match);
   CED_STORE_VAL_MGR_WPR("k_ced_object_ftm_lat_vel_max", ced_debug_data->ced_calibration.k_ced_object_ftm_lat_vel_max);
   CED_STORE_VAL_MGR_WPR("k_ced_object_ftm_long_vel_min", ced_debug_data->ced_calibration.k_ced_object_ftm_long_vel_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_ftm_long_vel_rel_min", ced_debug_data->ced_calibration.k_ced_object_ftm_long_vel_rel_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_ftm_heading_abs_angle_min",
                         ced_debug_data->ced_calibration.k_ced_object_ftm_heading_abs_angle_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_ftm_existence_probability_min",
                         ced_debug_data->ced_calibration.k_ced_object_ftm_existence_probability_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_lat_vel_max", ced_debug_data->ced_calibration.k_ced_object_lat_vel_max);
   CED_STORE_VAL_MGR_WPR("k_ced_object_long_vel_min", ced_debug_data->ced_calibration.k_ced_object_long_vel_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_long_vel_rel_min", ced_debug_data->ced_calibration.k_ced_object_long_vel_rel_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_heading_abs_angle_max", ced_debug_data->ced_calibration.k_ced_object_heading_abs_angle_max);
   CED_STORE_VAL_MGR_WPR("k_ced_object_existence_probability_min",
                         ced_debug_data->ced_calibration.k_ced_object_existence_probability_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_max_width_increase_factor_with_path_match",
                         ced_debug_data->ced_calibration.k_ced_object_max_width_increase_factor_with_path_match);
   CED_STORE_VAL_MGR_WPR("k_ced_object_max_width_increase_factor_without_path_match",
                         ced_debug_data->ced_calibration.k_ced_object_max_width_increase_factor_without_path_match);
   CED_STORE_VAL_MGR_WPR("k_ced_object_age_min", ced_debug_data->ced_calibration.k_ced_object_age_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_ftm_age_min", ced_debug_data->ced_calibration.k_ced_object_ftm_age_min);
   CED_STORE_VAL_MGR_WPR("k_ced_object_acceleration_weight", ced_debug_data->ced_calibration.k_ced_object_acceleration_weight);
   CED_STORE_VAL_MGR_WPR("k_ced_object_heading_predicted_weight",
                         ced_debug_data->ced_calibration.k_ced_object_heading_predicted_weight);

   /* alert settings */
   CED_STORE_VAL_MGR_WPR("k_ced_alert_holding_cycles", ced_debug_data->ced_calibration.k_ced_alert_holding_cycles);
   CED_STORE_VAL_MGR_WPR("k_ced_alert_qualifying_cycles", ced_debug_data->ced_calibration.k_ced_alert_qualifying_cycles);
   CED_STORE_VAL_MGR_WPR("k_ced_alert_qualifying_cycles_slow_objects",
                         ced_debug_data->ced_calibration.k_ced_alert_qualifying_cycles_slow_objects);
   CED_STORE_VAL_MGR_WPR("k_ced_slow_objects_long_vel_max", ced_debug_data->ced_calibration.k_ced_slow_objects_long_vel_max);
   CED_STORE_VAL_MGR_WPR("k_ced_first_warning_ttc_threshold_front",
                         ced_debug_data->ced_calibration.k_ced_first_warning_ttc_threshold[FBK_SIDE_FRONT]);
   CED_STORE_VAL_MGR_WPR("k_ced_first_warning_ttc_threshold_rear",
                         ced_debug_data->ced_calibration.k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]);
   CED_STORE_VAL_MGR_WPR("k_ced_second_warning_ttc_threshold_front",
                         ced_debug_data->ced_calibration.k_ced_second_warning_ttc_threshold[FBK_SIDE_FRONT]);
   CED_STORE_VAL_MGR_WPR("k_ced_second_warning_ttc_threshold_rear",
                         ced_debug_data->ced_calibration.k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR]);
   CED_STORE_VAL_MGR_WPR("k_ced_third_warning_ttc_threshold_front",
                         ced_debug_data->ced_calibration.k_ced_third_warning_ttc_threshold[FBK_SIDE_FRONT]);
   CED_STORE_VAL_MGR_WPR("k_ced_third_warning_ttc_threshold_rear",
                         ced_debug_data->ced_calibration.k_ced_third_warning_ttc_threshold[FBK_SIDE_REAR]);
   CED_STORE_VAL_MGR_WPR("k_ced_second_warning_pred_lat_dist_max",
                         ced_debug_data->ced_calibration.k_ced_second_warning_pred_lat_dist_max);
   CED_STORE_VAL_MGR_WPR("k_ced_alert_ttp_min_front", ced_debug_data->ced_calibration.k_ced_alert_ttp_min[FBK_SIDE_FRONT]);
   CED_STORE_VAL_MGR_WPR("k_ced_alert_ttp_min_rear", ced_debug_data->ced_calibration.k_ced_alert_ttp_min[FBK_SIDE_REAR]);
   CED_STORE_VAL_MGR_WPR("k_ced_warning_pred_lat_dist_max_histeresis",
                         ced_debug_data->ced_calibration.k_ced_warning_pred_lat_dist_max_histeresis);

   /* zones */
   CED_STORE_VAL_MGR_WPR("k_ced_funnel_zone_width", ced_debug_data->ced_calibration.k_ced_funnel_zone_width);
   CED_STORE_VAL_MGR_WPR("k_ced_funnel_zone_length", ced_debug_data->ced_calibration.k_ced_funnel_zone_length);
   CED_STORE_VAL_MGR_WPR("k_ced_collision_zone_width", ced_debug_data->ced_calibration.k_ced_collision_zone_width);
   CED_STORE_ARRAY_ELEM_MGR_WPR("k_ced_crash_line_host_length_percentage",
                                ced_debug_data->ced_calibration.k_ced_crash_line_host_length_percentage[0], 0);
   CED_STORE_ARRAY_ELEM_MGR_WPR("k_ced_crash_line_host_length_percentage",
                                ced_debug_data->ced_calibration.k_ced_crash_line_host_length_percentage[1], 1);

   CED_STORE_VAL_MGR_WPR("k_ced_ego_lane_width", ced_debug_data->ced_calibration.k_ced_ego_lane_width);

   /* enable flags */
   CED_STORE_VAL_MGR_WPR("k_ced_f_path_tracking_enable", ced_debug_data->ced_calibration.k_ced_f_path_tracking_enable);
   CED_STORE_VAL_MGR_WPR("k_ced_f_second_warning_level_enable", ced_debug_data->ced_calibration.k_ced_f_second_warning_level_enable);
   CED_STORE_VAL_MGR_WPR("k_ced_f_third_warning_level_enable", ced_debug_data->ced_calibration.k_ced_f_third_warning_level_enable);
   CED_STORE_VAL_MGR_WPR("k_ced_allow_opposite_side_alerts", ced_debug_data->ced_calibration.k_ced_allow_opposite_side_alerts);
   CED_STORE_VAL_MGR_WPR("k_ced_f_allow_ego_lane_alerts", ced_debug_data->ced_calibration.k_ced_f_allow_ego_lane_alerts);
   CED_STORE_VAL_MGR_WPR("k_ced_f_allow_coasted_object_alerts", ced_debug_data->ced_calibration.k_ced_f_allow_coasted_object_alerts);
   CED_STORE_VAL_MGR_WPR("k_ced_f_suppress_alert_holding_for_uncritical_objects",
                         ced_debug_data->ced_calibration.k_ced_f_suppress_alert_holding_for_uncritical_objects);
   CED_STORE_VAL_MGR_WPR("k_ced_f_handle_both_side_alerts_as_object_side",
                         ced_debug_data->ced_calibration.k_ced_f_handle_both_side_alerts_as_object_side);
   CED_STORE_VAL_MGR_WPR("k_ced_f_suppress_alert_holding_for_obj_below_min_ttp",
                         ced_debug_data->ced_calibration.k_ced_f_suppress_alert_holding_for_obj_below_min_ttp);

   /* exponential moving average filter */
   CED_STORE_VAL_MGR_WPR("k_ced_f_enable_heading_exp_moving_average",
                         ced_debug_data->ced_calibration.k_ced_f_enable_heading_exp_moving_average);
   CED_STORE_VAL_MGR_WPR("k_ced_object_heading_exp_moving_average_alpha",
                         ced_debug_data->ced_calibration.k_ced_object_heading_exp_moving_average_alpha);

   /* other */
   CED_STORE_VAL_MGR_WPR("k_ced_offset_to_path_weight", ced_debug_data->ced_calibration.k_ced_offset_to_path_weight);
   CED_STORE_VAL_MGR_WPR("k_ced_lat_pos_shift_enable", ced_debug_data->ced_calibration.k_ced_lat_pos_shift_enable);
   CED_STORE_VAL_MGR_WPR("k_ced_lat_pos_max_shift", ced_debug_data->ced_calibration.k_ced_lat_pos_max_shift);
   CED_STORE_ARRAY_ELEM_MGR_WPR("k_ced_lat_pos_shift_lat_dist_thresholds",
                                ced_debug_data->ced_calibration.k_ced_lat_pos_shift_lat_dist_thresholds[0], 0);
   CED_STORE_ARRAY_ELEM_MGR_WPR("k_ced_lat_pos_shift_lat_dist_thresholds",
                                ced_debug_data->ced_calibration.k_ced_lat_pos_shift_lat_dist_thresholds[1], 1);
   CED_STORE_ARRAY_ELEM_MGR_WPR("k_ced_lat_pos_shift_long_dist_thresholds",
                                ced_debug_data->ced_calibration.k_ced_lat_pos_shift_long_dist_thresholds[0], 0);
   CED_STORE_ARRAY_ELEM_MGR_WPR("k_ced_lat_pos_shift_long_dist_thresholds",
                                ced_debug_data->ced_calibration.k_ced_lat_pos_shift_long_dist_thresholds[1], 1);

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_importance_counter",
                                   ced_debug_data->ced_debug_output.ced_object_importance_counter[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_alert_suppression_reason",
                                   ced_debug_data->ced_debug_output.ced_object_alert_suppression_reason[i], i);

      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_ttc", ced_debug_data->ced_object_attributes[i].time_to_crash_line, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_ttp", ced_debug_data->ced_object_attributes[i].time_to_pass_crash_line, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_distance", ced_debug_data->ced_object_attributes[i].distance_to_crash_line, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_alert", ced_debug_data->ced_object_attributes[i].alert_level, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_criticality_side", ced_debug_data->ced_object_attributes[i].alert_side, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_current_location_side", ced_debug_data->ced_object_attributes[i].ego_side, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_direction", ced_debug_data->ced_object_attributes[i].direction, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_position_predicted_long",
                                   ced_debug_data->ced_object_attributes[i].position_predicted.x, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_position_predicted_lat",
                                   ced_debug_data->ced_object_attributes[i].position_predicted.y, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_heading_predicted", ced_debug_data->ced_object_attributes[i].heading_predicted, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_length_predicted", ced_debug_data->ced_object_attributes[i].length_predicted, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_width_predicted", ced_debug_data->ced_object_attributes[i].width_predicted, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_closest_lat_dist_predicted",
                                   ced_debug_data->ced_object_attributes[i].closest_lat_dist_predicted, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_front_bumper_pos_long",
                                   ced_debug_data->ced_object_attributes[i].front_bumper_pos_long, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_rear_bumper_pos_long",
                                   ced_debug_data->ced_object_attributes[i].rear_bumper_pos_long, i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_zone_width_hys_0", ced_debug_data->ced_object_attributes[i].zone_width_hys[0], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_zone_width_hys_1", ced_debug_data->ced_object_attributes[i].zone_width_hys[1], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_zone_width_hys_2", ced_debug_data->ced_object_attributes[i].zone_width_hys[2], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_zone_width_hys_3", ced_debug_data->ced_object_attributes[i].zone_width_hys[3], i);


      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_path_match_index", ced_debug_data->ced_debug_output.ced_path_match_index[i], i);
   }

   for (i = 0; i < CED_NUMBER_OF_ZONE_POINTS; i++)
   {
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_funnel_zone_x", ced_debug_data->ced_debug_output.ced_funnel_zone_x[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_funnel_zone_y", ced_debug_data->ced_debug_output.ced_funnel_zone_y[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_collision_zone_x", ced_debug_data->ced_debug_output.ced_collision_zone_x[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_collision_zone_y", ced_debug_data->ced_debug_output.ced_collision_zone_y[i], i);
   }

   CED_STORE_VAL_MGR_WPR("ced_crash_line_front", ced_debug_data->ced_debug_output.ced_crash_line_front);
   CED_STORE_VAL_MGR_WPR("ced_crash_line_rear", ced_debug_data->ced_debug_output.ced_crash_line_rear);

   CED_STORE_VAL_MGR_WPR("ced_side_object_id_internal_left",
                         ced_debug_data->ced_debug_output.ced_side_object_id_internal[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_side_object_id_internal_right",
                         ced_debug_data->ced_debug_output.ced_side_object_id_internal[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_side_alert_internal_left", ced_debug_data->ced_debug_output.ced_side_alert_internal[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_side_alert_internal_right", ced_debug_data->ced_debug_output.ced_side_alert_internal[FBK_SIDE_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_max_alertlevel", ced_debug_data->ced_debug_output.ced_max_alertlevel);
}

#endif /* BINARY_DEBUG */
