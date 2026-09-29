/**
* @file ced_public_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ced_public_calibration_t.h" // IWYU pragma: keep
#include "ced_public_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Ced_Public_Cal_Print(FILE* c_file_ptr, const Ced_Public_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_acceleration_weight,%f\n", p_cals->k_ced_object_acceleration_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_heading_predicted_weight,%f\n", p_cals->k_ced_object_heading_predicted_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_existence_probability_min,%f\n", p_cals->k_ced_object_existence_probability_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_heading_abs_angle_max,%f\n", p_cals->k_ced_object_heading_abs_angle_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_long_vel_rel_min,%f\n", p_cals->k_ced_object_long_vel_rel_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_long_vel_min,%f\n", p_cals->k_ced_object_long_vel_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_lat_vel_max,%f\n", p_cals->k_ced_object_lat_vel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_max_width_increase_factor_with_path_match,%f\n", p_cals->k_ced_object_max_width_increase_factor_with_path_match);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_max_width_increase_factor_without_path_match,%f\n", p_cals->k_ced_object_max_width_increase_factor_without_path_match);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_heading_exp_moving_average_alpha,%f\n", p_cals->k_ced_object_heading_exp_moving_average_alpha);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_existence_probability_min,%f\n", p_cals->k_ced_object_ftm_existence_probability_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_heading_abs_angle_min,%f\n", p_cals->k_ced_object_ftm_heading_abs_angle_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_long_vel_rel_min,%f\n", p_cals->k_ced_object_ftm_long_vel_rel_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_long_vel_min,%f\n", p_cals->k_ced_object_ftm_long_vel_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_lat_vel_max,%f\n", p_cals->k_ced_object_ftm_lat_vel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_first_warning_ttc_threshold._0_,%f\n", p_cals->k_ced_first_warning_ttc_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_first_warning_ttc_threshold._1_,%f\n", p_cals->k_ced_first_warning_ttc_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_second_warning_ttc_threshold._0_,%f\n", p_cals->k_ced_second_warning_ttc_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_second_warning_ttc_threshold._1_,%f\n", p_cals->k_ced_second_warning_ttc_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_third_warning_ttc_threshold._0_,%f\n", p_cals->k_ced_third_warning_ttc_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_third_warning_ttc_threshold._1_,%f\n", p_cals->k_ced_third_warning_ttc_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_second_warning_pred_lat_dist_max,%f\n", p_cals->k_ced_second_warning_pred_lat_dist_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_third_warning_pred_lat_dist_max,%f\n", p_cals->k_ced_third_warning_pred_lat_dist_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_ttp_min._0_,%f\n", p_cals->k_ced_alert_ttp_min[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_ttp_min._1_,%f\n", p_cals->k_ced_alert_ttp_min[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_ego_abs_speed_max,%f\n", p_cals->k_ced_ego_abs_speed_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_crash_line_host_length_percentage._0_,%f\n", p_cals->k_ced_crash_line_host_length_percentage[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_crash_line_host_length_percentage._1_,%f\n", p_cals->k_ced_crash_line_host_length_percentage[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_width_safety_margin_for_active_alert,%f\n", p_cals->k_ced_object_width_safety_margin_for_active_alert);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_width_safety_margin_for_critical_path_match,%f\n", p_cals->k_ced_object_width_safety_margin_for_critical_path_match);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_min_dist_to_crash_line_for_path_match,%f\n", p_cals->k_ced_object_min_dist_to_crash_line_for_path_match);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_offset_to_path_weight,%f\n", p_cals->k_ced_offset_to_path_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_collision_zone_width,%f\n", p_cals->k_ced_collision_zone_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_funnel_zone_length,%f\n", p_cals->k_ced_funnel_zone_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_funnel_zone_width,%f\n", p_cals->k_ced_funnel_zone_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_slow_objects_long_vel_max,%f\n", p_cals->k_ced_slow_objects_long_vel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_ego_lane_width,%f\n", p_cals->k_ced_ego_lane_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_ego_lane_parking_range,%f\n", p_cals->k_ced_ego_lane_parking_range);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_ego_lane_parking_maneuver_speed,%f\n", p_cals->k_ced_ego_lane_parking_maneuver_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_holding_obj_abs_heading_max,%f\n", p_cals->k_ced_alert_holding_obj_abs_heading_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_holding_obj_long_vel_min,%f\n", p_cals->k_ced_alert_holding_obj_long_vel_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_suppress_pt_heading_diff_ced_alert_max,%f\n", p_cals->k_ced_suppress_pt_heading_diff_ced_alert_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_suppress_range_to_nearest_path_max,%f\n", p_cals->k_ced_suppress_range_to_nearest_path_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_long_vel_rel_max,%f\n", p_cals->k_ced_object_long_vel_rel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_long_vel_rel_max,%f\n", p_cals->k_ced_object_ftm_long_vel_rel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_vel_max,%f\n", p_cals->k_ced_object_vel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_slight_turn_lat_vel_table._0_,%f\n", p_cals->k_ced_slight_turn_lat_vel_table[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_slight_turn_lat_vel_table._1_,%f\n", p_cals->k_ced_slight_turn_lat_vel_table[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_slight_turn_position_limits._0_,%f\n", p_cals->k_ced_slight_turn_position_limits[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_slight_turn_position_limits._1_,%f\n", p_cals->k_ced_slight_turn_position_limits[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_of_border,%f\n", p_cals->k_ced_lat_pos_of_border);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_max_shift,%f\n", p_cals->k_ced_lat_pos_max_shift);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_shift_long_dist_thresholds._0_,%f\n", p_cals->k_ced_lat_pos_shift_long_dist_thresholds[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_shift_long_dist_thresholds._1_,%f\n", p_cals->k_ced_lat_pos_shift_long_dist_thresholds[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_shift_lat_dist_thresholds._0_,%f\n", p_cals->k_ced_lat_pos_shift_lat_dist_thresholds[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_shift_lat_dist_thresholds._1_,%f\n", p_cals->k_ced_lat_pos_shift_lat_dist_thresholds[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_ced_speed_max_hysteresis,%f\n", p_cals->k_bmw_ced_speed_max_hysteresis);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_predicted_max_width_slope_reduce_factor,%f\n", p_cals->k_ced_object_predicted_max_width_slope_reduce_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_predicted_max_width_slope_offset,%f\n", p_cals->k_ced_object_predicted_max_width_slope_offset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_shift_width_thresh,%f\n", p_cals->k_ced_lat_pos_shift_width_thresh);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_warning_pred_lat_dist_max_histeresis,%f\n", p_cals->k_ced_warning_pred_lat_dist_max_histeresis);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_enable_heading_exp_moving_average,%d\n", p_cals->k_ced_f_enable_heading_exp_moving_average);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_second_warning_level_enable,%d\n", p_cals->k_ced_f_second_warning_level_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_third_warning_level_enable,%d\n", p_cals->k_ced_f_third_warning_level_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_suppress_alert_holding_for_uncritical_objects,%d\n", p_cals->k_ced_f_suppress_alert_holding_for_uncritical_objects);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_suppress_alert_holding_for_obj_below_min_ttp,%d\n", p_cals->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_path_tracking_enable,%d\n", p_cals->k_ced_f_path_tracking_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_handle_both_side_alerts_as_object_side,%d\n", p_cals->k_ced_f_handle_both_side_alerts_as_object_side);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_allow_ego_lane_alerts,%d\n", p_cals->k_ced_f_allow_ego_lane_alerts);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_use_only_mature_paths,%d\n", p_cals->k_ced_f_use_only_mature_paths);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_allow_coasted_object_alerts,%d\n", p_cals->k_ced_f_allow_coasted_object_alerts);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_adapt_heading_ego_lane,%d\n", p_cals->k_ced_f_adapt_heading_ego_lane);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_object_lat_on_one_side_of_border,%d\n", p_cals->k_ced_f_object_lat_on_one_side_of_border);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_lat_pos_shift_enable,%d\n", p_cals->k_ced_lat_pos_shift_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_qualifying_cycles,%d\n", p_cals->k_ced_alert_qualifying_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_qualifying_cycles_slow_objects,%d\n", p_cals->k_ced_alert_qualifying_cycles_slow_objects);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_alert_holding_cycles,%d\n", p_cals->k_ced_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_allow_opposite_side_alerts,%d\n", p_cals->k_ced_allow_opposite_side_alerts);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_suppress_alert_object_age_max,%d\n", p_cals->k_ced_suppress_alert_object_age_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_min_cycles_for_path_match_for_no_suppress,%d\n", p_cals->k_ced_min_cycles_for_path_match_for_no_suppress);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_age_min,%d\n", p_cals->k_ced_object_age_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_object_ftm_age_min,%d\n", p_cals->k_ced_object_ftm_age_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_choose_ref_point_funnel_check,%d\n", p_cals->k_ced_f_choose_ref_point_funnel_check);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
