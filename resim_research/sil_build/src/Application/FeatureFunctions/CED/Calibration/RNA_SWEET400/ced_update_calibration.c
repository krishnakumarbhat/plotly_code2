/**
* @file ced_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "ced_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "ced_core_calibration_check.h"
#include "ced_core_calibration_t.h"
#include "ced_customer_calibration_t.h"
#include "ced_public_calibration_check.h"
#include "ced_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ced_Update_Core_Cal_By_Public(Ced_Core_Calibration_T* cal_dst, const Ced_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ced_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_ced_object_acceleration_weight = cal_src->k_ced_object_acceleration_weight;
        cal_dst->k_ced_object_heading_predicted_weight = cal_src->k_ced_object_heading_predicted_weight;
        cal_dst->k_ced_object_existence_probability_min = cal_src->k_ced_object_existence_probability_min;
        cal_dst->k_ced_object_heading_abs_angle_max = cal_src->k_ced_object_heading_abs_angle_max;
        cal_dst->k_ced_object_long_vel_rel_min = cal_src->k_ced_object_long_vel_rel_min;
        cal_dst->k_ced_object_long_vel_min = cal_src->k_ced_object_long_vel_min;
        cal_dst->k_ced_object_lat_vel_max = cal_src->k_ced_object_lat_vel_max;
        cal_dst->k_ced_object_max_width_increase_factor_with_path_match = cal_src->k_ced_object_max_width_increase_factor_with_path_match;
        cal_dst->k_ced_object_max_width_increase_factor_without_path_match = cal_src->k_ced_object_max_width_increase_factor_without_path_match;
        cal_dst->k_ced_object_heading_exp_moving_average_alpha = cal_src->k_ced_object_heading_exp_moving_average_alpha;
        cal_dst->k_ced_object_ftm_existence_probability_min = cal_src->k_ced_object_ftm_existence_probability_min;
        cal_dst->k_ced_object_ftm_heading_abs_angle_min = cal_src->k_ced_object_ftm_heading_abs_angle_min;
        cal_dst->k_ced_object_ftm_long_vel_rel_min = cal_src->k_ced_object_ftm_long_vel_rel_min;
        cal_dst->k_ced_object_ftm_long_vel_min = cal_src->k_ced_object_ftm_long_vel_min;
        cal_dst->k_ced_object_ftm_lat_vel_max = cal_src->k_ced_object_ftm_lat_vel_max;
        cal_dst->k_ced_first_warning_ttc_threshold[0] = cal_src->k_ced_first_warning_ttc_threshold[0];
        cal_dst->k_ced_first_warning_ttc_threshold[1] = cal_src->k_ced_first_warning_ttc_threshold[1];
        cal_dst->k_ced_second_warning_ttc_threshold[0] = cal_src->k_ced_second_warning_ttc_threshold[0];
        cal_dst->k_ced_second_warning_ttc_threshold[1] = cal_src->k_ced_second_warning_ttc_threshold[1];
        cal_dst->k_ced_third_warning_ttc_threshold[0] = cal_src->k_ced_third_warning_ttc_threshold[0];
        cal_dst->k_ced_third_warning_ttc_threshold[1] = cal_src->k_ced_third_warning_ttc_threshold[1];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold[0] = cal_src->k_ced_honda_srr6_custom_ttc_alert_threshold[0];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold[1] = cal_src->k_ced_honda_srr6_custom_ttc_alert_threshold[1];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0] = cal_src->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis[1] = cal_src->k_ced_honda_srr6_custom_ttc_alert_hysteresis[1];
        cal_dst->k_ced_honda_srr6_long_dist_threshold[0] = cal_src->k_ced_honda_srr6_long_dist_threshold[0];
        cal_dst->k_ced_honda_srr6_long_dist_threshold[1] = cal_src->k_ced_honda_srr6_long_dist_threshold[1];
        cal_dst->k_ced_second_warning_pred_lat_dist_max = cal_src->k_ced_second_warning_pred_lat_dist_max;
        cal_dst->k_ced_third_warning_pred_lat_dist_max = cal_src->k_ced_third_warning_pred_lat_dist_max;
        cal_dst->k_ced_alert_ttp_min[0] = cal_src->k_ced_alert_ttp_min[0];
        cal_dst->k_ced_alert_ttp_min[1] = cal_src->k_ced_alert_ttp_min[1];
        cal_dst->k_ced_ego_abs_speed_max = cal_src->k_ced_ego_abs_speed_max;
        cal_dst->k_ced_crash_line_host_length_percentage[0] = cal_src->k_ced_crash_line_host_length_percentage[0];
        cal_dst->k_ced_crash_line_host_length_percentage[1] = cal_src->k_ced_crash_line_host_length_percentage[1];
        cal_dst->k_ced_object_width_safety_margin_for_active_alert = cal_src->k_ced_object_width_safety_margin_for_active_alert;
        cal_dst->k_ced_object_width_safety_margin_for_critical_path_match = cal_src->k_ced_object_width_safety_margin_for_critical_path_match;
        cal_dst->k_ced_object_min_dist_to_crash_line_for_path_match = cal_src->k_ced_object_min_dist_to_crash_line_for_path_match;
        cal_dst->k_ced_offset_to_path_weight = cal_src->k_ced_offset_to_path_weight;
        cal_dst->k_ced_collision_zone_width = cal_src->k_ced_collision_zone_width;
        cal_dst->k_ced_funnel_zone_length = cal_src->k_ced_funnel_zone_length;
        cal_dst->k_ced_funnel_zone_width = cal_src->k_ced_funnel_zone_width;
        cal_dst->k_ced_slow_objects_long_vel_max = cal_src->k_ced_slow_objects_long_vel_max;
        cal_dst->k_ced_ego_lane_width = cal_src->k_ced_ego_lane_width;
        cal_dst->k_ced_ego_lane_parking_range = cal_src->k_ced_ego_lane_parking_range;
        cal_dst->k_ced_ego_lane_parking_maneuver_speed = cal_src->k_ced_ego_lane_parking_maneuver_speed;
        cal_dst->k_ced_alert_holding_obj_abs_heading_max = cal_src->k_ced_alert_holding_obj_abs_heading_max;
        cal_dst->k_ced_alert_holding_obj_long_vel_min = cal_src->k_ced_alert_holding_obj_long_vel_min;
        cal_dst->k_ced_suppress_pt_heading_diff_ced_alert_max = cal_src->k_ced_suppress_pt_heading_diff_ced_alert_max;
        cal_dst->k_ced_suppress_range_to_nearest_path_max = cal_src->k_ced_suppress_range_to_nearest_path_max;
        cal_dst->k_ced_object_long_vel_rel_max = cal_src->k_ced_object_long_vel_rel_max;
        cal_dst->k_ced_object_ftm_long_vel_rel_max = cal_src->k_ced_object_ftm_long_vel_rel_max;
        cal_dst->k_ced_object_vel_max = cal_src->k_ced_object_vel_max;
        cal_dst->k_honda_min_alert_duration = cal_src->k_honda_min_alert_duration;
        cal_dst->k_honda_elatch_zones_width_table[0] = cal_src->k_honda_elatch_zones_width_table[0];
        cal_dst->k_honda_elatch_zones_width_table[1] = cal_src->k_honda_elatch_zones_width_table[1];
        cal_dst->k_honda_min_eratch_alert_duration = cal_src->k_honda_min_eratch_alert_duration;
        cal_dst->k_ced_slight_turn_lat_vel_table[0] = cal_src->k_ced_slight_turn_lat_vel_table[0];
        cal_dst->k_ced_slight_turn_lat_vel_table[1] = cal_src->k_ced_slight_turn_lat_vel_table[1];
        cal_dst->k_ced_slight_turn_position_limits[0] = cal_src->k_ced_slight_turn_position_limits[0];
        cal_dst->k_ced_slight_turn_position_limits[1] = cal_src->k_ced_slight_turn_position_limits[1];
        cal_dst->k_ced_lat_pos_of_border = cal_src->k_ced_lat_pos_of_border;
        cal_dst->k_ced_lat_pos_max_shift = cal_src->k_ced_lat_pos_max_shift;
        cal_dst->k_ced_lat_pos_shift_long_dist_thresholds[0] = cal_src->k_ced_lat_pos_shift_long_dist_thresholds[0];
        cal_dst->k_ced_lat_pos_shift_long_dist_thresholds[1] = cal_src->k_ced_lat_pos_shift_long_dist_thresholds[1];
        cal_dst->k_ced_lat_pos_shift_lat_dist_thresholds[0] = cal_src->k_ced_lat_pos_shift_lat_dist_thresholds[0];
        cal_dst->k_ced_lat_pos_shift_lat_dist_thresholds[1] = cal_src->k_ced_lat_pos_shift_lat_dist_thresholds[1];
        cal_dst->k_bmw_ced_speed_max_hysteresis = cal_src->k_bmw_ced_speed_max_hysteresis;
        cal_dst->k_ced_object_predicted_max_width_slope_reduce_factor = cal_src->k_ced_object_predicted_max_width_slope_reduce_factor;
        cal_dst->k_ced_object_predicted_max_width_slope_offset = cal_src->k_ced_object_predicted_max_width_slope_offset;
        cal_dst->k_ced_lat_pos_shift_width_thresh = cal_src->k_ced_lat_pos_shift_width_thresh;
        cal_dst->k_ced_warning_pred_lat_dist_max_histeresis = cal_src->k_ced_warning_pred_lat_dist_max_histeresis;
        cal_dst->k_ced_f_enable_heading_exp_moving_average = cal_src->k_ced_f_enable_heading_exp_moving_average;
        cal_dst->k_ced_f_second_warning_level_enable = cal_src->k_ced_f_second_warning_level_enable;
        cal_dst->k_ced_f_third_warning_level_enable = cal_src->k_ced_f_third_warning_level_enable;
        cal_dst->k_ced_f_suppress_alert_holding_for_uncritical_objects = cal_src->k_ced_f_suppress_alert_holding_for_uncritical_objects;
        cal_dst->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp = cal_src->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp;
        cal_dst->k_ced_f_path_tracking_enable = cal_src->k_ced_f_path_tracking_enable;
        cal_dst->k_ced_f_handle_both_side_alerts_as_object_side = cal_src->k_ced_f_handle_both_side_alerts_as_object_side;
        cal_dst->k_ced_f_allow_ego_lane_alerts = cal_src->k_ced_f_allow_ego_lane_alerts;
        cal_dst->k_ced_f_use_only_mature_paths = cal_src->k_ced_f_use_only_mature_paths;
        cal_dst->k_ced_f_allow_coasted_object_alerts = cal_src->k_ced_f_allow_coasted_object_alerts;
        cal_dst->k_ced_f_honda_use_alert_ttc_threshold = cal_src->k_ced_f_honda_use_alert_ttc_threshold;
        cal_dst->k_ced_f_adapt_heading_ego_lane = cal_src->k_ced_f_adapt_heading_ego_lane;
        cal_dst->k_ced_f_object_lat_on_one_side_of_border = cal_src->k_ced_f_object_lat_on_one_side_of_border;
        cal_dst->k_ced_lat_pos_shift_enable = cal_src->k_ced_lat_pos_shift_enable;
        cal_dst->k_ced_alert_qualifying_cycles = cal_src->k_ced_alert_qualifying_cycles;
        cal_dst->k_ced_alert_qualifying_cycles_slow_objects = cal_src->k_ced_alert_qualifying_cycles_slow_objects;
        cal_dst->k_ced_alert_holding_cycles = cal_src->k_ced_alert_holding_cycles;
        cal_dst->k_ced_allow_opposite_side_alerts = cal_src->k_ced_allow_opposite_side_alerts;
        cal_dst->k_ced_suppress_alert_object_age_max = cal_src->k_ced_suppress_alert_object_age_max;
        cal_dst->k_ced_min_cycles_for_path_match_for_no_suppress = cal_src->k_ced_min_cycles_for_path_match_for_no_suppress;
        cal_dst->k_ced_object_age_min = cal_src->k_ced_object_age_min;
        cal_dst->k_ced_object_ftm_age_min = cal_src->k_ced_object_ftm_age_min;
        cal_dst->k_ced_f_choose_ref_point_funnel_check = cal_src->k_ced_f_choose_ref_point_funnel_check;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ced_Update_Core_Cal_By_Core(Ced_Core_Calibration_T* cal_dst, const Ced_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ced_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_ced_object_acceleration_weight = cal_src->k_ced_object_acceleration_weight;
        cal_dst->k_ced_object_heading_predicted_weight = cal_src->k_ced_object_heading_predicted_weight;
        cal_dst->k_ced_object_existence_probability_min = cal_src->k_ced_object_existence_probability_min;
        cal_dst->k_ced_object_heading_abs_angle_max = cal_src->k_ced_object_heading_abs_angle_max;
        cal_dst->k_ced_object_long_vel_rel_min = cal_src->k_ced_object_long_vel_rel_min;
        cal_dst->k_ced_object_long_vel_min = cal_src->k_ced_object_long_vel_min;
        cal_dst->k_ced_object_lat_vel_max = cal_src->k_ced_object_lat_vel_max;
        cal_dst->k_ced_object_max_width_increase_factor_with_path_match = cal_src->k_ced_object_max_width_increase_factor_with_path_match;
        cal_dst->k_ced_object_max_width_increase_factor_without_path_match = cal_src->k_ced_object_max_width_increase_factor_without_path_match;
        cal_dst->k_ced_object_heading_exp_moving_average_alpha = cal_src->k_ced_object_heading_exp_moving_average_alpha;
        cal_dst->k_ced_object_ftm_existence_probability_min = cal_src->k_ced_object_ftm_existence_probability_min;
        cal_dst->k_ced_object_ftm_heading_abs_angle_min = cal_src->k_ced_object_ftm_heading_abs_angle_min;
        cal_dst->k_ced_object_ftm_long_vel_rel_min = cal_src->k_ced_object_ftm_long_vel_rel_min;
        cal_dst->k_ced_object_ftm_long_vel_min = cal_src->k_ced_object_ftm_long_vel_min;
        cal_dst->k_ced_object_ftm_lat_vel_max = cal_src->k_ced_object_ftm_lat_vel_max;
        cal_dst->k_ced_first_warning_ttc_threshold[0] = cal_src->k_ced_first_warning_ttc_threshold[0];
        cal_dst->k_ced_first_warning_ttc_threshold[1] = cal_src->k_ced_first_warning_ttc_threshold[1];
        cal_dst->k_ced_second_warning_ttc_threshold[0] = cal_src->k_ced_second_warning_ttc_threshold[0];
        cal_dst->k_ced_second_warning_ttc_threshold[1] = cal_src->k_ced_second_warning_ttc_threshold[1];
        cal_dst->k_ced_third_warning_ttc_threshold[0] = cal_src->k_ced_third_warning_ttc_threshold[0];
        cal_dst->k_ced_third_warning_ttc_threshold[1] = cal_src->k_ced_third_warning_ttc_threshold[1];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold[0] = cal_src->k_ced_honda_srr6_custom_ttc_alert_threshold[0];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold[1] = cal_src->k_ced_honda_srr6_custom_ttc_alert_threshold[1];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0] = cal_src->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0];
        cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis[1] = cal_src->k_ced_honda_srr6_custom_ttc_alert_hysteresis[1];
        cal_dst->k_ced_honda_srr6_long_dist_threshold[0] = cal_src->k_ced_honda_srr6_long_dist_threshold[0];
        cal_dst->k_ced_honda_srr6_long_dist_threshold[1] = cal_src->k_ced_honda_srr6_long_dist_threshold[1];
        cal_dst->k_ced_second_warning_pred_lat_dist_max = cal_src->k_ced_second_warning_pred_lat_dist_max;
        cal_dst->k_ced_third_warning_pred_lat_dist_max = cal_src->k_ced_third_warning_pred_lat_dist_max;
        cal_dst->k_ced_alert_ttp_min[0] = cal_src->k_ced_alert_ttp_min[0];
        cal_dst->k_ced_alert_ttp_min[1] = cal_src->k_ced_alert_ttp_min[1];
        cal_dst->k_ced_ego_abs_speed_max = cal_src->k_ced_ego_abs_speed_max;
        cal_dst->k_ced_crash_line_host_length_percentage[0] = cal_src->k_ced_crash_line_host_length_percentage[0];
        cal_dst->k_ced_crash_line_host_length_percentage[1] = cal_src->k_ced_crash_line_host_length_percentage[1];
        cal_dst->k_ced_object_width_safety_margin_for_active_alert = cal_src->k_ced_object_width_safety_margin_for_active_alert;
        cal_dst->k_ced_object_width_safety_margin_for_critical_path_match = cal_src->k_ced_object_width_safety_margin_for_critical_path_match;
        cal_dst->k_ced_object_min_dist_to_crash_line_for_path_match = cal_src->k_ced_object_min_dist_to_crash_line_for_path_match;
        cal_dst->k_ced_offset_to_path_weight = cal_src->k_ced_offset_to_path_weight;
        cal_dst->k_ced_collision_zone_width = cal_src->k_ced_collision_zone_width;
        cal_dst->k_ced_funnel_zone_length = cal_src->k_ced_funnel_zone_length;
        cal_dst->k_ced_funnel_zone_width = cal_src->k_ced_funnel_zone_width;
        cal_dst->k_ced_slow_objects_long_vel_max = cal_src->k_ced_slow_objects_long_vel_max;
        cal_dst->k_ced_ego_lane_width = cal_src->k_ced_ego_lane_width;
        cal_dst->k_ced_ego_lane_parking_range = cal_src->k_ced_ego_lane_parking_range;
        cal_dst->k_ced_ego_lane_parking_maneuver_speed = cal_src->k_ced_ego_lane_parking_maneuver_speed;
        cal_dst->k_ced_alert_holding_obj_abs_heading_max = cal_src->k_ced_alert_holding_obj_abs_heading_max;
        cal_dst->k_ced_alert_holding_obj_long_vel_min = cal_src->k_ced_alert_holding_obj_long_vel_min;
        cal_dst->k_ced_suppress_pt_heading_diff_ced_alert_max = cal_src->k_ced_suppress_pt_heading_diff_ced_alert_max;
        cal_dst->k_ced_suppress_range_to_nearest_path_max = cal_src->k_ced_suppress_range_to_nearest_path_max;
        cal_dst->k_ced_object_long_vel_rel_max = cal_src->k_ced_object_long_vel_rel_max;
        cal_dst->k_ced_object_ftm_long_vel_rel_max = cal_src->k_ced_object_ftm_long_vel_rel_max;
        cal_dst->k_ced_object_vel_max = cal_src->k_ced_object_vel_max;
        cal_dst->k_honda_min_alert_duration = cal_src->k_honda_min_alert_duration;
        cal_dst->k_honda_elatch_zones_width_table[0] = cal_src->k_honda_elatch_zones_width_table[0];
        cal_dst->k_honda_elatch_zones_width_table[1] = cal_src->k_honda_elatch_zones_width_table[1];
        cal_dst->k_honda_min_eratch_alert_duration = cal_src->k_honda_min_eratch_alert_duration;
        cal_dst->k_ced_slight_turn_lat_vel_table[0] = cal_src->k_ced_slight_turn_lat_vel_table[0];
        cal_dst->k_ced_slight_turn_lat_vel_table[1] = cal_src->k_ced_slight_turn_lat_vel_table[1];
        cal_dst->k_ced_slight_turn_position_limits[0] = cal_src->k_ced_slight_turn_position_limits[0];
        cal_dst->k_ced_slight_turn_position_limits[1] = cal_src->k_ced_slight_turn_position_limits[1];
        cal_dst->k_ced_lat_pos_of_border = cal_src->k_ced_lat_pos_of_border;
        cal_dst->k_ced_lat_pos_max_shift = cal_src->k_ced_lat_pos_max_shift;
        cal_dst->k_ced_lat_pos_shift_long_dist_thresholds[0] = cal_src->k_ced_lat_pos_shift_long_dist_thresholds[0];
        cal_dst->k_ced_lat_pos_shift_long_dist_thresholds[1] = cal_src->k_ced_lat_pos_shift_long_dist_thresholds[1];
        cal_dst->k_ced_lat_pos_shift_lat_dist_thresholds[0] = cal_src->k_ced_lat_pos_shift_lat_dist_thresholds[0];
        cal_dst->k_ced_lat_pos_shift_lat_dist_thresholds[1] = cal_src->k_ced_lat_pos_shift_lat_dist_thresholds[1];
        cal_dst->k_bmw_ced_speed_max_hysteresis = cal_src->k_bmw_ced_speed_max_hysteresis;
        cal_dst->k_ced_object_predicted_max_width_slope_reduce_factor = cal_src->k_ced_object_predicted_max_width_slope_reduce_factor;
        cal_dst->k_ced_object_predicted_max_width_slope_offset = cal_src->k_ced_object_predicted_max_width_slope_offset;
        cal_dst->k_ced_lat_pos_shift_width_thresh = cal_src->k_ced_lat_pos_shift_width_thresh;
        cal_dst->k_ced_warning_pred_lat_dist_max_histeresis = cal_src->k_ced_warning_pred_lat_dist_max_histeresis;
        cal_dst->k_ced_f_enable_heading_exp_moving_average = cal_src->k_ced_f_enable_heading_exp_moving_average;
        cal_dst->k_ced_f_second_warning_level_enable = cal_src->k_ced_f_second_warning_level_enable;
        cal_dst->k_ced_f_third_warning_level_enable = cal_src->k_ced_f_third_warning_level_enable;
        cal_dst->k_ced_f_suppress_alert_holding_for_uncritical_objects = cal_src->k_ced_f_suppress_alert_holding_for_uncritical_objects;
        cal_dst->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp = cal_src->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp;
        cal_dst->k_ced_f_path_tracking_enable = cal_src->k_ced_f_path_tracking_enable;
        cal_dst->k_ced_f_handle_both_side_alerts_as_object_side = cal_src->k_ced_f_handle_both_side_alerts_as_object_side;
        cal_dst->k_ced_f_allow_ego_lane_alerts = cal_src->k_ced_f_allow_ego_lane_alerts;
        cal_dst->k_ced_f_use_only_mature_paths = cal_src->k_ced_f_use_only_mature_paths;
        cal_dst->k_ced_f_allow_coasted_object_alerts = cal_src->k_ced_f_allow_coasted_object_alerts;
        cal_dst->k_ced_f_honda_use_alert_ttc_threshold = cal_src->k_ced_f_honda_use_alert_ttc_threshold;
        cal_dst->k_ced_f_adapt_heading_ego_lane = cal_src->k_ced_f_adapt_heading_ego_lane;
        cal_dst->k_ced_f_object_lat_on_one_side_of_border = cal_src->k_ced_f_object_lat_on_one_side_of_border;
        cal_dst->k_ced_lat_pos_shift_enable = cal_src->k_ced_lat_pos_shift_enable;
        cal_dst->k_ced_alert_qualifying_cycles = cal_src->k_ced_alert_qualifying_cycles;
        cal_dst->k_ced_alert_qualifying_cycles_slow_objects = cal_src->k_ced_alert_qualifying_cycles_slow_objects;
        cal_dst->k_ced_alert_holding_cycles = cal_src->k_ced_alert_holding_cycles;
        cal_dst->k_ced_allow_opposite_side_alerts = cal_src->k_ced_allow_opposite_side_alerts;
        cal_dst->k_ced_suppress_alert_object_age_max = cal_src->k_ced_suppress_alert_object_age_max;
        cal_dst->k_ced_min_cycles_for_path_match_for_no_suppress = cal_src->k_ced_min_cycles_for_path_match_for_no_suppress;
        cal_dst->k_ced_object_age_min = cal_src->k_ced_object_age_min;
        cal_dst->k_ced_object_ftm_age_min = cal_src->k_ced_object_ftm_age_min;
        cal_dst->k_ced_f_choose_ref_point_funnel_check = cal_src->k_ced_f_choose_ref_point_funnel_check;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ced_Update_Customer_Cal_By_Public(Ced_Customer_Calibration_T* cal_dst, const Ced_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ced_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


