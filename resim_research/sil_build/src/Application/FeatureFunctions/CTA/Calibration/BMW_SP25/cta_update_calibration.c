/**
* @file cta_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "cta_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "cta_core_calibration_check.h"
#include "cta_core_calibration_t.h"
#include "cta_customer_calibration_t.h"
#include "cta_public_calibration_check.h"
#include "cta_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Cta_Update_Core_Cal_By_Public(Cta_Core_Calibration_T* cal_dst, const Cta_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Cta_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_cta_ego_abs_speed_max = cal_src->k_cta_ego_abs_speed_max;
        cal_dst->k_cta_stop_alert_ttc = cal_src->k_cta_stop_alert_ttc;
        cal_dst->k_cta_stop_alert_ttp = cal_src->k_cta_stop_alert_ttp;
        cal_dst->k_cta_min_speed = cal_src->k_cta_min_speed;
        cal_dst->k_cta_butterfly_long[0] = cal_src->k_cta_butterfly_long[0];
        cal_dst->k_cta_butterfly_long[1] = cal_src->k_cta_butterfly_long[1];
        cal_dst->k_cta_butterfly_long[2] = cal_src->k_cta_butterfly_long[2];
        cal_dst->k_cta_butterfly_long[3] = cal_src->k_cta_butterfly_long[3];
        cal_dst->k_cta_butterfly_long[4] = cal_src->k_cta_butterfly_long[4];
        cal_dst->k_cta_butterfly_long[5] = cal_src->k_cta_butterfly_long[5];
        cal_dst->k_cta_butterfly_long[6] = cal_src->k_cta_butterfly_long[6];
        cal_dst->k_cta_butterfly_long[7] = cal_src->k_cta_butterfly_long[7];
        cal_dst->k_cta_butterfly_lat[0] = cal_src->k_cta_butterfly_lat[0];
        cal_dst->k_cta_butterfly_lat[1] = cal_src->k_cta_butterfly_lat[1];
        cal_dst->k_cta_butterfly_lat[2] = cal_src->k_cta_butterfly_lat[2];
        cal_dst->k_cta_butterfly_lat[3] = cal_src->k_cta_butterfly_lat[3];
        cal_dst->k_cta_butterfly_lat[4] = cal_src->k_cta_butterfly_lat[4];
        cal_dst->k_cta_butterfly_lat[5] = cal_src->k_cta_butterfly_lat[5];
        cal_dst->k_cta_butterfly_lat[6] = cal_src->k_cta_butterfly_lat[6];
        cal_dst->k_cta_butterfly_lat[7] = cal_src->k_cta_butterfly_lat[7];
        cal_dst->k_cta_ttc_criticality_level[0][0] =         cal_src->k_cta_ttc_criticality_level[0][0];
        cal_dst->k_cta_ttc_criticality_level[0][1] =         cal_src->k_cta_ttc_criticality_level[0][1];
        cal_dst->k_cta_ttc_criticality_level[1][0] =         cal_src->k_cta_ttc_criticality_level[1][0];
        cal_dst->k_cta_ttc_criticality_level[1][1] =         cal_src->k_cta_ttc_criticality_level[1][1];
        cal_dst->k_cta_speed_criticality_level[0][0] =         cal_src->k_cta_speed_criticality_level[0][0];
        cal_dst->k_cta_speed_criticality_level[0][1] =         cal_src->k_cta_speed_criticality_level[0][1];
        cal_dst->k_cta_speed_criticality_level[1][0] =         cal_src->k_cta_speed_criticality_level[1][0];
        cal_dst->k_cta_speed_criticality_level[1][1] =         cal_src->k_cta_speed_criticality_level[1][1];
        cal_dst->k_cta_max_long_point_criticality_level[0][0] =         cal_src->k_cta_max_long_point_criticality_level[0][0];
        cal_dst->k_cta_max_long_point_criticality_level[0][1] =         cal_src->k_cta_max_long_point_criticality_level[0][1];
        cal_dst->k_cta_max_long_point_criticality_level[1][0] =         cal_src->k_cta_max_long_point_criticality_level[1][0];
        cal_dst->k_cta_max_long_point_criticality_level[1][1] =         cal_src->k_cta_max_long_point_criticality_level[1][1];
        cal_dst->k_cta_min_long_point_criticality_level[0][0] =         cal_src->k_cta_min_long_point_criticality_level[0][0];
        cal_dst->k_cta_min_long_point_criticality_level[0][1] =         cal_src->k_cta_min_long_point_criticality_level[0][1];
        cal_dst->k_cta_min_long_point_criticality_level[1][0] =         cal_src->k_cta_min_long_point_criticality_level[1][0];
        cal_dst->k_cta_min_long_point_criticality_level[1][1] =         cal_src->k_cta_min_long_point_criticality_level[1][1];
        cal_dst->k_cta_min_lateral_approach_speed = cal_src->k_cta_min_lateral_approach_speed;
        cal_dst->k_cta_min_rel_existence_probability = cal_src->k_cta_min_rel_existence_probability;
        cal_dst->k_cta_rel_warning_hysteresis = cal_src->k_cta_rel_warning_hysteresis;
        cal_dst->k_cta_rcta_host_speed_factor = cal_src->k_cta_rcta_host_speed_factor;
        cal_dst->k_cta_max_speed = cal_src->k_cta_max_speed;
        cal_dst->k_cta_max_length_fov = cal_src->k_cta_max_length_fov;
        cal_dst->k_cta_heading_range[0] = cal_src->k_cta_heading_range[0];
        cal_dst->k_cta_heading_range[1] = cal_src->k_cta_heading_range[1];
        cal_dst->k_cta_angles_zone_definition[0] = cal_src->k_cta_angles_zone_definition[0];
        cal_dst->k_cta_angles_zone_definition[1] = cal_src->k_cta_angles_zone_definition[1];
        cal_dst->k_cta_min_host_speed_to_discard_pt_info = cal_src->k_cta_min_host_speed_to_discard_pt_info;
        cal_dst->k_cta_obj_dist_to_discard_pt_info = cal_src->k_cta_obj_dist_to_discard_pt_info;
        cal_dst->k_cta_ghost_condition_max_heading_diff_path_tracker = cal_src->k_cta_ghost_condition_max_heading_diff_path_tracker;
        cal_dst->k_cta_min_ttc_additional_mature_qualification = cal_src->k_cta_min_ttc_additional_mature_qualification;
        cal_dst->k_cta_intersection_line_host_width_percentage = cal_src->k_cta_intersection_line_host_width_percentage;
        cal_dst->k_ctb_lower_safety_distance_thres = cal_src->k_ctb_lower_safety_distance_thres;
        cal_dst->k_ctb_upper_safety_distance_thres_lut[0] = cal_src->k_ctb_upper_safety_distance_thres_lut[0];
        cal_dst->k_ctb_upper_safety_distance_thres_lut[1] = cal_src->k_ctb_upper_safety_distance_thres_lut[1];
        cal_dst->k_ctb_upper_safety_distance_thres_lut[2] = cal_src->k_ctb_upper_safety_distance_thres_lut[2];
        cal_dst->k_ctb_upper_safety_distance_thres_lut[3] = cal_src->k_ctb_upper_safety_distance_thres_lut[3];
        cal_dst->k_ctb_safety_dist_host_vel_lut[0] = cal_src->k_ctb_safety_dist_host_vel_lut[0];
        cal_dst->k_ctb_safety_dist_host_vel_lut[1] = cal_src->k_ctb_safety_dist_host_vel_lut[1];
        cal_dst->k_ctb_safety_dist_host_vel_lut[2] = cal_src->k_ctb_safety_dist_host_vel_lut[2];
        cal_dst->k_ctb_safety_dist_host_vel_lut[3] = cal_src->k_ctb_safety_dist_host_vel_lut[3];
        cal_dst->k_ctb_event_time_buffer = cal_src->k_ctb_event_time_buffer;
        cal_dst->k_ctb_min_braking_time = cal_src->k_ctb_min_braking_time;
        cal_dst->k_ctb_max_braking_time = cal_src->k_ctb_max_braking_time;
        cal_dst->k_ctb_responsetime_brake_actuation = cal_src->k_ctb_responsetime_brake_actuation;
        cal_dst->k_ctb_ramp_in_time = cal_src->k_ctb_ramp_in_time;
        cal_dst->k_ctb_const_decel_after_ramp_in = cal_src->k_ctb_const_decel_after_ramp_in;
        cal_dst->k_ctb_braking_jerk = cal_src->k_ctb_braking_jerk;
        cal_dst->k_ctb_host_acc_weight = cal_src->k_ctb_host_acc_weight;
        cal_dst->k_ctb_time_to_ask_for_final_brake_decel = cal_src->k_ctb_time_to_ask_for_final_brake_decel;
        cal_dst->k_cta_dist_thres_crit_level_reset = cal_src->k_cta_dist_thres_crit_level_reset;
        cal_dst->k_cta_max_heading_variance = cal_src->k_cta_max_heading_variance;
        cal_dst->k_cta_max_seg_heading_diff_no_ghost = cal_src->k_cta_max_seg_heading_diff_no_ghost;
        cal_dst->k_cta_range_to_path_segment_ghost_qualif = cal_src->k_cta_range_to_path_segment_ghost_qualif;
        cal_dst->k_cta_object_heading_exp_moving_average_alpha = cal_src->k_cta_object_heading_exp_moving_average_alpha;
        cal_dst->k_cta_pedestrian_min_size = cal_src->k_cta_pedestrian_min_size;
        cal_dst->k_cta_pedestrian_min_speed = cal_src->k_cta_pedestrian_min_speed;
        cal_dst->k_cta_2wheel_min_size = cal_src->k_cta_2wheel_min_size;
        cal_dst->k_cta_2wheel_min_speed = cal_src->k_cta_2wheel_min_speed;
        cal_dst->k_cta_min_deceleration_value = cal_src->k_cta_min_deceleration_value;
        cal_dst->k_cta_max_deceleration_value = cal_src->k_cta_max_deceleration_value;
        cal_dst->k_cta_ttc_calc_positive_ref_point = cal_src->k_cta_ttc_calc_positive_ref_point;
        cal_dst->k_cta_speed_thresh_for_rel_vel_calc = cal_src->k_cta_speed_thresh_for_rel_vel_calc;
        cal_dst->k_cta_f_adapt_intersect_lines_by_steering_angle = cal_src->k_cta_f_adapt_intersect_lines_by_steering_angle;
        cal_dst->k_cta_f_adapt_intersect_lines_by_obj_heading = cal_src->k_cta_f_adapt_intersect_lines_by_obj_heading;
        cal_dst->k_cta_f_adapt_intersect_lines_by_host_speed = cal_src->k_cta_f_adapt_intersect_lines_by_host_speed;
        cal_dst->k_cta_f_prevent_fall_back_to_critlevel_1 = cal_src->k_cta_f_prevent_fall_back_to_critlevel_1;
        cal_dst->k_cta_f_apply_heading_compensation_on_intersection_point = cal_src->k_cta_f_apply_heading_compensation_on_intersection_point;
        cal_dst->k_cta_f_calc_ttc_ego_side_enabled = cal_src->k_cta_f_calc_ttc_ego_side_enabled;
        cal_dst->k_cta_f_calc_ttp_ego_side_enabled = cal_src->k_cta_f_calc_ttp_ego_side_enabled;
        cal_dst->k_cta_f_use_heading_for_relative_velocity_calculation = cal_src->k_cta_f_use_heading_for_relative_velocity_calculation;
        cal_dst->k_cta_f_apply_path_tracking = cal_src->k_cta_f_apply_path_tracking;
        cal_dst->k_cta_f_discard_pt_heading_when_moving = cal_src->k_cta_f_discard_pt_heading_when_moving;
        cal_dst->k_cta_f_use_object_supress_counter = cal_src->k_cta_f_use_object_supress_counter;
        cal_dst->k_cta_f_use_ghost_detector = cal_src->k_cta_f_use_ghost_detector;
        cal_dst->k_cta_f_use_object_min_object_age_in_cycles = cal_src->k_cta_f_use_object_min_object_age_in_cycles;
        cal_dst->k_cta_f_use_rel_vel_isect_point_calc = cal_src->k_cta_f_use_rel_vel_isect_point_calc;
        cal_dst->k_cta_f_enable_thres_crit_level_reset = cal_src->k_cta_f_enable_thres_crit_level_reset;
        cal_dst->k_cta_f_use_front_corners_dist_stop = cal_src->k_cta_f_use_front_corners_dist_stop;
        cal_dst->k_cta_enable_ctb = cal_src->k_cta_enable_ctb;
        cal_dst->k_cta_f_brake_overriding_ctb = cal_src->k_cta_f_brake_overriding_ctb;
        cal_dst->k_cta_f_use_brake_gradient = cal_src->k_cta_f_use_brake_gradient;
        cal_dst->k_cta_f_enable_heading_exp_moving_average = cal_src->k_cta_f_enable_heading_exp_moving_average;
        cal_dst->k_cta_f_stop_mode_ttp = cal_src->k_cta_f_stop_mode_ttp;
        cal_dst->k_cta_amount_butterfly_points_in_use = cal_src->k_cta_amount_butterfly_points_in_use;
        cal_dst->k_cta_enable_modes[0] = cal_src->k_cta_enable_modes[0];
        cal_dst->k_cta_enable_modes[1] = cal_src->k_cta_enable_modes[1];
        cal_dst->k_cta_cycle_count_suppress_true_warning = cal_src->k_cta_cycle_count_suppress_true_warning;
        cal_dst->k_cta_cycle_count_hold_true_warning = cal_src->k_cta_cycle_count_hold_true_warning;
        cal_dst->k_cta_min_object_age_check_valid = cal_src->k_cta_min_object_age_check_valid;
        cal_dst->k_cta_min_object_age_thres = cal_src->k_cta_min_object_age_thres;
        cal_dst->k_cta_cycles_coasted_to_ignore = cal_src->k_cta_cycles_coasted_to_ignore;
        cal_dst->k_cta_object_supress_counter = cal_src->k_cta_object_supress_counter;
        cal_dst->k_cta_ghost_validation_min_age = cal_src->k_cta_ghost_validation_min_age;
        cal_dst->k_cta_ghost_validation_min_mature = cal_src->k_cta_ghost_validation_min_mature;
        cal_dst->k_cta_min_mature_cycles_level_qualifiction = cal_src->k_cta_min_mature_cycles_level_qualifiction;
        cal_dst->k_cta_additional_qualification_mature_cycles = cal_src->k_cta_additional_qualification_mature_cycles;
        cal_dst->k_ctb_min_brake_qual_ctr_thres = cal_src->k_ctb_min_brake_qual_ctr_thres;
        cal_dst->k_ctb_min_brake_hold_ctr_thres = cal_src->k_ctb_min_brake_hold_ctr_thres;
        cal_dst->k_cta_min_qual_age_obj_crossing_paths = cal_src->k_cta_min_qual_age_obj_crossing_paths;
        cal_dst->k_cta_cycles_valid_match_of_pot_ghost = cal_src->k_cta_cycles_valid_match_of_pot_ghost;
        cal_dst->k_cta_age_for_new_creation_below_long_intersection = cal_src->k_cta_age_for_new_creation_below_long_intersection;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Cta_Update_Core_Cal_By_Core(Cta_Core_Calibration_T* cal_dst, const Cta_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Cta_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_cta_min_park_angle = cal_src->k_cta_min_park_angle;
        cal_dst->k_cta_fcta_steer_angle_table[0] = cal_src->k_cta_fcta_steer_angle_table[0];
        cal_dst->k_cta_fcta_steer_angle_table[1] = cal_src->k_cta_fcta_steer_angle_table[1];
        cal_dst->k_cta_fcta_steer_angle_table[2] = cal_src->k_cta_fcta_steer_angle_table[2];
        cal_dst->k_cta_fcta_steer_angle_table[3] = cal_src->k_cta_fcta_steer_angle_table[3];
        cal_dst->k_cta_fcta_steer_angle_table[4] = cal_src->k_cta_fcta_steer_angle_table[4];
        cal_dst->k_cta_fcta_steer_factor_table[0] = cal_src->k_cta_fcta_steer_factor_table[0];
        cal_dst->k_cta_fcta_steer_factor_table[1] = cal_src->k_cta_fcta_steer_factor_table[1];
        cal_dst->k_cta_fcta_steer_factor_table[2] = cal_src->k_cta_fcta_steer_factor_table[2];
        cal_dst->k_cta_fcta_steer_factor_table[3] = cal_src->k_cta_fcta_steer_factor_table[3];
        cal_dst->k_cta_fcta_steer_factor_table[4] = cal_src->k_cta_fcta_steer_factor_table[4];
        cal_dst->k_cta_rcta_steer_angle_table[0] = cal_src->k_cta_rcta_steer_angle_table[0];
        cal_dst->k_cta_rcta_steer_angle_table[1] = cal_src->k_cta_rcta_steer_angle_table[1];
        cal_dst->k_cta_rcta_steer_angle_table[2] = cal_src->k_cta_rcta_steer_angle_table[2];
        cal_dst->k_cta_rcta_steer_angle_table[3] = cal_src->k_cta_rcta_steer_angle_table[3];
        cal_dst->k_cta_rcta_steer_angle_table[4] = cal_src->k_cta_rcta_steer_angle_table[4];
        cal_dst->k_cta_rcta_steer_factor_table[0] = cal_src->k_cta_rcta_steer_factor_table[0];
        cal_dst->k_cta_rcta_steer_factor_table[1] = cal_src->k_cta_rcta_steer_factor_table[1];
        cal_dst->k_cta_rcta_steer_factor_table[2] = cal_src->k_cta_rcta_steer_factor_table[2];
        cal_dst->k_cta_rcta_steer_factor_table[3] = cal_src->k_cta_rcta_steer_factor_table[3];
        cal_dst->k_cta_rcta_steer_factor_table[4] = cal_src->k_cta_rcta_steer_factor_table[4];
        cal_dst->k_cta_max_obstruction_probability = cal_src->k_cta_max_obstruction_probability;
        cal_dst->k_cta_max_object_eclipse_for_level_qualification = cal_src->k_cta_max_object_eclipse_for_level_qualification;
        cal_dst->k_cta_accelerationpedal_gradient_threshold_ctb = cal_src->k_cta_accelerationpedal_gradient_threshold_ctb;
        cal_dst->k_cta_host_width_sensor_fov_suppr_factor = cal_src->k_cta_host_width_sensor_fov_suppr_factor;
        cal_dst->k_cta_sensor_fov_border[0] = cal_src->k_cta_sensor_fov_border[0];
        cal_dst->k_cta_sensor_fov_border[1] = cal_src->k_cta_sensor_fov_border[1];
        cal_dst->k_cta_ttc_warntrigger_early = cal_src->k_cta_ttc_warntrigger_early;
        cal_dst->k_cta_ttc_warntrigger_late = cal_src->k_cta_ttc_warntrigger_late;
        cal_dst->k_cta_ego_abs_speed_max = cal_src->k_cta_ego_abs_speed_max;
        cal_dst->k_cta_stop_alert_ttc = cal_src->k_cta_stop_alert_ttc;
        cal_dst->k_cta_stop_alert_ttp = cal_src->k_cta_stop_alert_ttp;
        cal_dst->k_cta_min_speed = cal_src->k_cta_min_speed;
        cal_dst->k_cta_butterfly_long[0] = cal_src->k_cta_butterfly_long[0];
        cal_dst->k_cta_butterfly_long[1] = cal_src->k_cta_butterfly_long[1];
        cal_dst->k_cta_butterfly_long[2] = cal_src->k_cta_butterfly_long[2];
        cal_dst->k_cta_butterfly_long[3] = cal_src->k_cta_butterfly_long[3];
        cal_dst->k_cta_butterfly_long[4] = cal_src->k_cta_butterfly_long[4];
        cal_dst->k_cta_butterfly_long[5] = cal_src->k_cta_butterfly_long[5];
        cal_dst->k_cta_butterfly_long[6] = cal_src->k_cta_butterfly_long[6];
        cal_dst->k_cta_butterfly_long[7] = cal_src->k_cta_butterfly_long[7];
        cal_dst->k_cta_butterfly_lat[0] = cal_src->k_cta_butterfly_lat[0];
        cal_dst->k_cta_butterfly_lat[1] = cal_src->k_cta_butterfly_lat[1];
        cal_dst->k_cta_butterfly_lat[2] = cal_src->k_cta_butterfly_lat[2];
        cal_dst->k_cta_butterfly_lat[3] = cal_src->k_cta_butterfly_lat[3];
        cal_dst->k_cta_butterfly_lat[4] = cal_src->k_cta_butterfly_lat[4];
        cal_dst->k_cta_butterfly_lat[5] = cal_src->k_cta_butterfly_lat[5];
        cal_dst->k_cta_butterfly_lat[6] = cal_src->k_cta_butterfly_lat[6];
        cal_dst->k_cta_butterfly_lat[7] = cal_src->k_cta_butterfly_lat[7];
        cal_dst->k_cta_ttc_criticality_level[0][0] =         cal_src->k_cta_ttc_criticality_level[0][0];
        cal_dst->k_cta_ttc_criticality_level[0][1] =         cal_src->k_cta_ttc_criticality_level[0][1];
        cal_dst->k_cta_ttc_criticality_level[1][0] =         cal_src->k_cta_ttc_criticality_level[1][0];
        cal_dst->k_cta_ttc_criticality_level[1][1] =         cal_src->k_cta_ttc_criticality_level[1][1];
        cal_dst->k_cta_speed_criticality_level[0][0] =         cal_src->k_cta_speed_criticality_level[0][0];
        cal_dst->k_cta_speed_criticality_level[0][1] =         cal_src->k_cta_speed_criticality_level[0][1];
        cal_dst->k_cta_speed_criticality_level[1][0] =         cal_src->k_cta_speed_criticality_level[1][0];
        cal_dst->k_cta_speed_criticality_level[1][1] =         cal_src->k_cta_speed_criticality_level[1][1];
        cal_dst->k_cta_max_long_point_criticality_level[0][0] =         cal_src->k_cta_max_long_point_criticality_level[0][0];
        cal_dst->k_cta_max_long_point_criticality_level[0][1] =         cal_src->k_cta_max_long_point_criticality_level[0][1];
        cal_dst->k_cta_max_long_point_criticality_level[1][0] =         cal_src->k_cta_max_long_point_criticality_level[1][0];
        cal_dst->k_cta_max_long_point_criticality_level[1][1] =         cal_src->k_cta_max_long_point_criticality_level[1][1];
        cal_dst->k_cta_min_long_point_criticality_level[0][0] =         cal_src->k_cta_min_long_point_criticality_level[0][0];
        cal_dst->k_cta_min_long_point_criticality_level[0][1] =         cal_src->k_cta_min_long_point_criticality_level[0][1];
        cal_dst->k_cta_min_long_point_criticality_level[1][0] =         cal_src->k_cta_min_long_point_criticality_level[1][0];
        cal_dst->k_cta_min_long_point_criticality_level[1][1] =         cal_src->k_cta_min_long_point_criticality_level[1][1];
        cal_dst->k_cta_min_lateral_approach_speed = cal_src->k_cta_min_lateral_approach_speed;
        cal_dst->k_cta_min_rel_existence_probability = cal_src->k_cta_min_rel_existence_probability;
        cal_dst->k_cta_rel_warning_hysteresis = cal_src->k_cta_rel_warning_hysteresis;
        cal_dst->k_cta_rcta_host_speed_factor = cal_src->k_cta_rcta_host_speed_factor;
        cal_dst->k_cta_max_speed = cal_src->k_cta_max_speed;
        cal_dst->k_cta_max_length_fov = cal_src->k_cta_max_length_fov;
        cal_dst->k_cta_heading_range[0] = cal_src->k_cta_heading_range[0];
        cal_dst->k_cta_heading_range[1] = cal_src->k_cta_heading_range[1];
        cal_dst->k_cta_angles_zone_definition[0] = cal_src->k_cta_angles_zone_definition[0];
        cal_dst->k_cta_angles_zone_definition[1] = cal_src->k_cta_angles_zone_definition[1];
        cal_dst->k_cta_min_host_speed_to_discard_pt_info = cal_src->k_cta_min_host_speed_to_discard_pt_info;
        cal_dst->k_cta_obj_dist_to_discard_pt_info = cal_src->k_cta_obj_dist_to_discard_pt_info;
        cal_dst->k_cta_ghost_condition_max_heading_diff_path_tracker = cal_src->k_cta_ghost_condition_max_heading_diff_path_tracker;
        cal_dst->k_cta_min_ttc_additional_mature_qualification = cal_src->k_cta_min_ttc_additional_mature_qualification;
        cal_dst->k_cta_intersection_line_host_width_percentage = cal_src->k_cta_intersection_line_host_width_percentage;
        cal_dst->k_ctb_lower_safety_distance_thres = cal_src->k_ctb_lower_safety_distance_thres;
        cal_dst->k_ctb_upper_safety_distance_thres_lut[0] = cal_src->k_ctb_upper_safety_distance_thres_lut[0];
        cal_dst->k_ctb_upper_safety_distance_thres_lut[1] = cal_src->k_ctb_upper_safety_distance_thres_lut[1];
        cal_dst->k_ctb_upper_safety_distance_thres_lut[2] = cal_src->k_ctb_upper_safety_distance_thres_lut[2];
        cal_dst->k_ctb_upper_safety_distance_thres_lut[3] = cal_src->k_ctb_upper_safety_distance_thres_lut[3];
        cal_dst->k_ctb_safety_dist_host_vel_lut[0] = cal_src->k_ctb_safety_dist_host_vel_lut[0];
        cal_dst->k_ctb_safety_dist_host_vel_lut[1] = cal_src->k_ctb_safety_dist_host_vel_lut[1];
        cal_dst->k_ctb_safety_dist_host_vel_lut[2] = cal_src->k_ctb_safety_dist_host_vel_lut[2];
        cal_dst->k_ctb_safety_dist_host_vel_lut[3] = cal_src->k_ctb_safety_dist_host_vel_lut[3];
        cal_dst->k_ctb_event_time_buffer = cal_src->k_ctb_event_time_buffer;
        cal_dst->k_ctb_min_braking_time = cal_src->k_ctb_min_braking_time;
        cal_dst->k_ctb_max_braking_time = cal_src->k_ctb_max_braking_time;
        cal_dst->k_ctb_responsetime_brake_actuation = cal_src->k_ctb_responsetime_brake_actuation;
        cal_dst->k_ctb_ramp_in_time = cal_src->k_ctb_ramp_in_time;
        cal_dst->k_ctb_const_decel_after_ramp_in = cal_src->k_ctb_const_decel_after_ramp_in;
        cal_dst->k_ctb_braking_jerk = cal_src->k_ctb_braking_jerk;
        cal_dst->k_ctb_host_acc_weight = cal_src->k_ctb_host_acc_weight;
        cal_dst->k_ctb_time_to_ask_for_final_brake_decel = cal_src->k_ctb_time_to_ask_for_final_brake_decel;
        cal_dst->k_cta_dist_thres_crit_level_reset = cal_src->k_cta_dist_thres_crit_level_reset;
        cal_dst->k_cta_max_heading_variance = cal_src->k_cta_max_heading_variance;
        cal_dst->k_cta_max_seg_heading_diff_no_ghost = cal_src->k_cta_max_seg_heading_diff_no_ghost;
        cal_dst->k_cta_range_to_path_segment_ghost_qualif = cal_src->k_cta_range_to_path_segment_ghost_qualif;
        cal_dst->k_cta_object_heading_exp_moving_average_alpha = cal_src->k_cta_object_heading_exp_moving_average_alpha;
        cal_dst->k_cta_pedestrian_min_size = cal_src->k_cta_pedestrian_min_size;
        cal_dst->k_cta_pedestrian_min_speed = cal_src->k_cta_pedestrian_min_speed;
        cal_dst->k_cta_2wheel_min_size = cal_src->k_cta_2wheel_min_size;
        cal_dst->k_cta_2wheel_min_speed = cal_src->k_cta_2wheel_min_speed;
        cal_dst->k_cta_min_deceleration_value = cal_src->k_cta_min_deceleration_value;
        cal_dst->k_cta_max_deceleration_value = cal_src->k_cta_max_deceleration_value;
        cal_dst->k_cta_ttc_calc_positive_ref_point = cal_src->k_cta_ttc_calc_positive_ref_point;
        cal_dst->k_cta_speed_thresh_for_rel_vel_calc = cal_src->k_cta_speed_thresh_for_rel_vel_calc;
        cal_dst->k_cta_DEBUG_MODE = cal_src->k_cta_DEBUG_MODE;
        cal_dst->k_cta_f_check_reflection_signal = cal_src->k_cta_f_check_reflection_signal;
        cal_dst->k_cta_f_check_obstruction_probability_signal = cal_src->k_cta_f_check_obstruction_probability_signal;
        cal_dst->k_cta_switch = cal_src->k_cta_switch;
        cal_dst->k_cta_f_adapt_intersect_lines_by_steering_angle = cal_src->k_cta_f_adapt_intersect_lines_by_steering_angle;
        cal_dst->k_cta_f_adapt_intersect_lines_by_obj_heading = cal_src->k_cta_f_adapt_intersect_lines_by_obj_heading;
        cal_dst->k_cta_f_adapt_intersect_lines_by_host_speed = cal_src->k_cta_f_adapt_intersect_lines_by_host_speed;
        cal_dst->k_cta_f_prevent_fall_back_to_critlevel_1 = cal_src->k_cta_f_prevent_fall_back_to_critlevel_1;
        cal_dst->k_cta_f_apply_heading_compensation_on_intersection_point = cal_src->k_cta_f_apply_heading_compensation_on_intersection_point;
        cal_dst->k_cta_f_calc_ttc_ego_side_enabled = cal_src->k_cta_f_calc_ttc_ego_side_enabled;
        cal_dst->k_cta_f_calc_ttp_ego_side_enabled = cal_src->k_cta_f_calc_ttp_ego_side_enabled;
        cal_dst->k_cta_f_use_heading_for_relative_velocity_calculation = cal_src->k_cta_f_use_heading_for_relative_velocity_calculation;
        cal_dst->k_cta_f_apply_path_tracking = cal_src->k_cta_f_apply_path_tracking;
        cal_dst->k_cta_f_discard_pt_heading_when_moving = cal_src->k_cta_f_discard_pt_heading_when_moving;
        cal_dst->k_cta_f_use_object_supress_counter = cal_src->k_cta_f_use_object_supress_counter;
        cal_dst->k_cta_f_use_ghost_detector = cal_src->k_cta_f_use_ghost_detector;
        cal_dst->k_cta_f_use_object_min_object_age_in_cycles = cal_src->k_cta_f_use_object_min_object_age_in_cycles;
        cal_dst->k_cta_f_use_rel_vel_isect_point_calc = cal_src->k_cta_f_use_rel_vel_isect_point_calc;
        cal_dst->k_cta_f_enable_thres_crit_level_reset = cal_src->k_cta_f_enable_thres_crit_level_reset;
        cal_dst->k_cta_f_use_front_corners_dist_stop = cal_src->k_cta_f_use_front_corners_dist_stop;
        cal_dst->k_cta_enable_ctb = cal_src->k_cta_enable_ctb;
        cal_dst->k_cta_f_brake_overriding_ctb = cal_src->k_cta_f_brake_overriding_ctb;
        cal_dst->k_cta_f_use_brake_gradient = cal_src->k_cta_f_use_brake_gradient;
        cal_dst->k_cta_f_enable_heading_exp_moving_average = cal_src->k_cta_f_enable_heading_exp_moving_average;
        cal_dst->k_cta_f_stop_mode_ttp = cal_src->k_cta_f_stop_mode_ttp;
        cal_dst->k_cta_addit_mature_cycles_outside_sensor_fov = cal_src->k_cta_addit_mature_cycles_outside_sensor_fov;
        cal_dst->k_cta_min_age_obj_outside_sensor_fov = cal_src->k_cta_min_age_obj_outside_sensor_fov;
        cal_dst->k_cta_amount_butterfly_points_in_use = cal_src->k_cta_amount_butterfly_points_in_use;
        cal_dst->k_cta_enable_modes[0] = cal_src->k_cta_enable_modes[0];
        cal_dst->k_cta_enable_modes[1] = cal_src->k_cta_enable_modes[1];
        cal_dst->k_cta_cycle_count_suppress_true_warning = cal_src->k_cta_cycle_count_suppress_true_warning;
        cal_dst->k_cta_cycle_count_hold_true_warning = cal_src->k_cta_cycle_count_hold_true_warning;
        cal_dst->k_cta_min_object_age_check_valid = cal_src->k_cta_min_object_age_check_valid;
        cal_dst->k_cta_min_object_age_thres = cal_src->k_cta_min_object_age_thres;
        cal_dst->k_cta_cycles_coasted_to_ignore = cal_src->k_cta_cycles_coasted_to_ignore;
        cal_dst->k_cta_object_supress_counter = cal_src->k_cta_object_supress_counter;
        cal_dst->k_cta_ghost_validation_min_age = cal_src->k_cta_ghost_validation_min_age;
        cal_dst->k_cta_ghost_validation_min_mature = cal_src->k_cta_ghost_validation_min_mature;
        cal_dst->k_cta_min_mature_cycles_level_qualifiction = cal_src->k_cta_min_mature_cycles_level_qualifiction;
        cal_dst->k_cta_additional_qualification_mature_cycles = cal_src->k_cta_additional_qualification_mature_cycles;
        cal_dst->k_ctb_min_brake_qual_ctr_thres = cal_src->k_ctb_min_brake_qual_ctr_thres;
        cal_dst->k_ctb_min_brake_hold_ctr_thres = cal_src->k_ctb_min_brake_hold_ctr_thres;
        cal_dst->k_cta_min_qual_age_obj_crossing_paths = cal_src->k_cta_min_qual_age_obj_crossing_paths;
        cal_dst->k_cta_cycles_valid_match_of_pot_ghost = cal_src->k_cta_cycles_valid_match_of_pot_ghost;
        cal_dst->k_cta_age_for_new_creation_below_long_intersection = cal_src->k_cta_age_for_new_creation_below_long_intersection;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;
        cal_dst->k_unused_padding_byte_1 = cal_src->k_unused_padding_byte_1;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Cta_Update_Customer_Cal_By_Public(Cta_Customer_Calibration_T* cal_dst, const Cta_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Cta_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_bmw_sp25_ego_abs_speed_max_hys = cal_src->k_bmw_sp25_ego_abs_speed_max_hys;
        cal_dst->k_bmw_sp25_banner_criteria_check_time = cal_src->k_bmw_sp25_banner_criteria_check_time;
        cal_dst->k_bmw_sp25_banner_time = cal_src->k_bmw_sp25_banner_time;
        cal_dst->k_bmw_sp25_banner_criteria_check = cal_src->k_bmw_sp25_banner_criteria_check;

        f_result = (boolean_T) 1;
    }
    return f_result;
}


