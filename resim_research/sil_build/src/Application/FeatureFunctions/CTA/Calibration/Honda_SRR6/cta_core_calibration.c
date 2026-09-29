
/**
* @file cta_core_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the Honda_SRR6 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "cta_core_calibration_t.h"
#include "cta_core_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Core_Cal_Update_Defaults(Cta_Core_Calibration_T* cal_dst)
{
    Cta_Core_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)504,
   /**<version*/ (uint16_t)73,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)27321,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_cta_min_park_angle*/ (float32_T) 0.64 /**< 0.64 rad | 36.67 deg */,
   /**<k_cta_fcta_steer_angle_table*/ {(float32_T)0.0 /**< 0.0 rad | 0.0 deg */,(float32_T)0.15 /**< 0.15 rad | 8.59 deg */,(float32_T)0.32 /**< 0.32 rad | 18.33 deg */,(float32_T)0.46 /**< 0.46 rad | 26.36 deg */,(float32_T)0.62 /**< 0.62 rad | 35.52 deg */},
   /**<k_cta_fcta_steer_factor_table*/ {(float32_T)1.0,(float32_T)0.9,(float32_T)0.9,(float32_T)0.8,(float32_T)0.7},
   /**<k_cta_rcta_steer_angle_table*/ {(float32_T)0.0 /**< 0.0 rad | 0.0 deg */,(float32_T)0.15 /**< 0.15 rad | 8.59 deg */,(float32_T)0.32 /**< 0.32 rad | 18.33 deg */,(float32_T)0.46 /**< 0.46 rad | 26.36 deg */,(float32_T)0.62 /**< 0.62 rad | 35.52 deg */},
   /**<k_cta_rcta_steer_factor_table*/ {(float32_T)1.0,(float32_T)0.9,(float32_T)0.9,(float32_T)0.8,(float32_T)0.8},
   /**<k_cta_max_obstruction_probability*/ (float32_T) 0.9,
   /**<k_cta_max_object_eclipse_for_level_qualification*/ (float32_T) 1.0,
   /**<k_cta_accelerationpedal_gradient_threshold_ctb*/ (float32_T) 30.0 /**< 30.0 m/s | 108.0 km/h */,
   /**<k_cta_host_width_sensor_fov_suppr_factor*/ (float32_T) 1.0,
   /**<k_cta_sensor_fov_border*/ {(float32_T)0.0 /**< 0.0 rad | 0.0 deg */,(float32_T)0.0 /**< 0.0 rad | 0.0 deg */},
   /**<k_cta_ttc_warntrigger_early*/ (float32_T) 5.0 /**< 5.0 s */,
   /**<k_cta_ttc_warntrigger_late*/ (float32_T) 3.0 /**< 3.0 s */,
   /**<k_cta_ego_abs_speed_max*/ (float32_T) 4.1667 /**< 4.1667 m/s | 15.0 km/h */,
   /**<k_cta_stop_alert_ttc*/ (float32_T) 0.1 /**< 0.1 s */,
   /**<k_cta_stop_alert_ttp*/ (float32_T) -3.0 /**< -3.0 s */,
   /**<k_cta_min_speed*/ (float32_T) 1.1 /**< 1.1 m/s | 3.96 km/h */,
   /**<k_cta_butterfly_long*/ {(float32_T)9.0 /**< 9.0 m */,(float32_T)-14.0 /**< -14.0 m */,(float32_T)-39.0 /**< -39.0 m */,(float32_T)34.0 /**< 34.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_butterfly_lat*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_ttc_criticality_level*/ {{(float32_T)3.10 /**< 3.10 s */,(float32_T)3.10 /**< 3.10 s */},{(float32_T)3.10 /**< 3.10 s */,(float32_T)3.10 /**< 3.10 s */}},
   /**<k_cta_speed_criticality_level*/ {{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */}},
   /**<k_cta_max_long_point_criticality_level*/ {{(float32_T)-0.5 /**< -0.5 m */,(float32_T)0.0 /**< 0.0 m */},{(float32_T)5.0 /**< 5.0 m */,(float32_T)7.0 /**< 7.0 m */}},
   /**<k_cta_min_long_point_criticality_level*/ {{(float32_T)5.0 /**< 5.0 m */,(float32_T)7.0 /**< 7.0 m */},{(float32_T)-0.5 /**< -0.5 m */,(float32_T)0.0 /**< 0.0 m */}},
   /**<k_cta_min_lateral_approach_speed*/ (float32_T) 0 /**< 0 m/s | 0.0 km/h */,
   /**<k_cta_min_rel_existence_probability*/ (float32_T) 0.85 /**< 0.85 % */,
   /**<k_cta_rel_warning_hysteresis*/ (float32_T) 0.2,
   /**<k_cta_rcta_host_speed_factor*/ (float32_T) 0.0,
   /**<k_cta_max_speed*/ (float32_T) 7.5 /**< 7.5 m/s | 27.0 km/h */,
   /**<k_cta_max_length_fov*/ (float32_T) 70.0 /**< 70.0 m */,
   /**<k_cta_heading_range*/ {(float32_T)0.52f /**< 0.52 rad | 29.79 deg */,(float32_T)2.62f /**< 2.62 rad | 150.11 deg */},
   /**<k_cta_angles_zone_definition*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)2.356f /**< 2.356 rad | 134.99 deg */},
   /**<k_cta_min_host_speed_to_discard_pt_info*/ (float32_T) 0.02f /**< 0.02 m/s | 0.07 km/h */,
   /**<k_cta_obj_dist_to_discard_pt_info*/ (float32_T) 15.0f /**< 15.0 m */,
   /**<k_cta_ghost_condition_max_heading_diff_path_tracker*/ (float32_T) 0.35 /**< 0.35 rad | 20.05 deg */,
   /**<k_cta_min_ttc_additional_mature_qualification*/ (float32_T) 100.0 /**< 100.0 s */,
   /**<k_cta_intersection_line_host_width_percentage*/ (float32_T) 1.0f,
   /**<k_ctb_lower_safety_distance_thres*/ (float32_T) 0.3 /**< 0.3 m */,
   /**<k_ctb_upper_safety_distance_thres_lut*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)1.25f /**< 1.25 m */,(float32_T)1.375f /**< 1.375 m */,(float32_T)1.5f /**< 1.5 m */},
   /**<k_ctb_safety_dist_host_vel_lut*/ {(float32_T)0.28f /**< 0.28 m/s | 1.01 km/h */,(float32_T)0.56f /**< 0.56 m/s | 2.02 km/h */,(float32_T)0.69f /**< 0.69 m/s | 2.48 km/h */,(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */},
   /**<k_ctb_event_time_buffer*/ (float32_T) 0.15 /**< 0.15 s */,
   /**<k_ctb_min_braking_time*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ctb_max_braking_time*/ (float32_T) 0.9f /**< 0.9 s */,
   /**<k_ctb_responsetime_brake_actuation*/ (float32_T) 0.15 /**< 0.15 s */,
   /**<k_ctb_ramp_in_time*/ (float32_T) 0.3 /**< 0.3 s */,
   /**<k_ctb_const_decel_after_ramp_in*/ (float32_T) 6.0 /**< 6.0 m/s**2 */,
   /**<k_ctb_braking_jerk*/ (float32_T) 5 /**< 5 m/s**3 */,
   /**<k_ctb_host_acc_weight*/ (float32_T) 1.0f,
   /**<k_ctb_time_to_ask_for_final_brake_decel*/ (float32_T) 0.0f,
   /**<k_cta_dist_thres_crit_level_reset*/ (float32_T) 0.5 /**< 0.5 % */,
   /**<k_cta_max_heading_variance*/ (float32_T) 2.0 /**< 2.0 rad^2 */,
   /**<k_cta_max_seg_heading_diff_no_ghost*/ (float32_T) 0.3,
   /**<k_cta_range_to_path_segment_ghost_qualif*/ (float32_T) 3.5,
   /**<k_cta_object_heading_exp_moving_average_alpha*/ (float32_T) 0.15f,
   /**<k_cta_pedestrian_min_size*/ (float32_T) 0.01,
   /**<k_cta_pedestrian_min_speed*/ (float32_T) 0.01,
   /**<k_cta_2wheel_min_size*/ (float32_T) 0.01,
   /**<k_cta_2wheel_min_speed*/ (float32_T) 0.01,
   /**<k_cta_min_deceleration_value*/ (float32_T) 0.0f /**< 0.0 m/s**2 */,
   /**<k_cta_max_deceleration_value*/ (float32_T) 10.0f /**< 10.0 m/s**2 */,
   /**<k_cta_ttc_calc_positive_ref_point*/ (float32_T) -100.0f /**< -100.0 s */,
   /**<k_cta_speed_thresh_for_rel_vel_calc*/ (float32_T) 2.7f /**< 2.7 m/s | 9.72 km/h */,
   /**<k_cta_DEBUG_MODE*/ (uint16_t) 0,
   /**<k_cta_f_check_reflection_signal*/ (boolean_T) 0,
   /**<k_cta_f_check_obstruction_probability_signal*/ (boolean_T) 0,
   /**<k_cta_switch*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_steering_angle*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_obj_heading*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_host_speed*/ (boolean_T) 0,
   /**<k_cta_f_prevent_fall_back_to_critlevel_1*/ (boolean_T) 0,
   /**<k_cta_f_apply_heading_compensation_on_intersection_point*/ (boolean_T) 1,
   /**<k_cta_f_calc_ttc_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_calc_ttp_ego_side_enabled*/ (boolean_T) 0,
   /**<k_cta_f_use_heading_for_relative_velocity_calculation*/ (boolean_T) 0,
   /**<k_cta_f_apply_path_tracking*/ (boolean_T) 0,
   /**<k_cta_f_discard_pt_heading_when_moving*/ (boolean_T) 0,
   /**<k_cta_f_use_object_supress_counter*/ (boolean_T) 1,
   /**<k_cta_f_use_ghost_detector*/ (boolean_T) 0,
   /**<k_cta_f_use_object_min_object_age_in_cycles*/ (boolean_T) 1,
   /**<k_cta_f_use_rel_vel_isect_point_calc*/ (boolean_T) 0,
   /**<k_cta_f_enable_thres_crit_level_reset*/ (boolean_T) 1,
   /**<k_cta_f_use_front_corners_dist_stop*/ (boolean_T) 1 /**< 1 m */,
   /**<k_cta_enable_ctb*/ (boolean_T) 1,
   /**<k_cta_f_brake_overriding_ctb*/ (boolean_T) 1,
   /**<k_cta_f_use_brake_gradient*/ (boolean_T) 1,
   /**<k_cta_f_enable_heading_exp_moving_average*/ (boolean_T) 1,
   /**<k_cta_f_stop_mode_ttp*/ (boolean_T) 0,
   /**<k_cta_addit_mature_cycles_outside_sensor_fov*/ (uint8_t) 0,
   /**<k_cta_min_age_obj_outside_sensor_fov*/ (uint8_t) 0,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_unused_padding_byte_1*/ (uint8_t) 0,
   /**<k_cta_amount_butterfly_points_in_use*/ (uint8_t) 4 /**< 4 m */,
   /**<k_cta_enable_modes*/ {(uint8_t)1,(uint8_t)0},
   /**<k_cta_cycle_count_suppress_true_warning*/ (uint8_t) 3,
   /**<k_cta_cycle_count_hold_true_warning*/ (uint8_t) 3,
   /**<k_cta_min_object_age_check_valid*/ (uint8_t) 4,
   /**<k_cta_min_object_age_thres*/ (uint8_t) 0,
   /**<k_cta_cycles_coasted_to_ignore*/ (uint8_t) 9,
   /**<k_cta_object_supress_counter*/ (uint8_t) 5,
   /**<k_cta_ghost_validation_min_age*/ (uint8_t) 4,
   /**<k_cta_ghost_validation_min_mature*/ (uint8_t) 10,
   /**<k_cta_min_mature_cycles_level_qualifiction*/ (uint8_t) 2,
   /**<k_cta_additional_qualification_mature_cycles*/ (uint8_t) 0,
   /**<k_ctb_min_brake_qual_ctr_thres*/ (uint8_t) 1u,
   /**<k_ctb_min_brake_hold_ctr_thres*/ (uint8_t) 2u,
   /**<k_cta_min_qual_age_obj_crossing_paths*/ (uint8_t) 10,
   /**<k_cta_cycles_valid_match_of_pot_ghost*/ (uint8_t) 3,
   /**<k_cta_age_for_new_creation_below_long_intersection*/ (uint8_t) 5
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_cta_age_for_new_creation_below_long_intersection*/ (uint8_t) 5,
   /**<k_cta_cycles_valid_match_of_pot_ghost*/ (uint8_t) 3,
   /**<k_cta_min_qual_age_obj_crossing_paths*/ (uint8_t) 10,
   /**<k_ctb_min_brake_hold_ctr_thres*/ (uint8_t) 2u,
   /**<k_ctb_min_brake_qual_ctr_thres*/ (uint8_t) 1u,
   /**<k_cta_additional_qualification_mature_cycles*/ (uint8_t) 0,
   /**<k_cta_min_mature_cycles_level_qualifiction*/ (uint8_t) 2,
   /**<k_cta_ghost_validation_min_mature*/ (uint8_t) 10,
   /**<k_cta_ghost_validation_min_age*/ (uint8_t) 4,
   /**<k_cta_object_supress_counter*/ (uint8_t) 5,
   /**<k_cta_cycles_coasted_to_ignore*/ (uint8_t) 9,
   /**<k_cta_min_object_age_thres*/ (uint8_t) 0,
   /**<k_cta_min_object_age_check_valid*/ (uint8_t) 4,
   /**<k_cta_cycle_count_hold_true_warning*/ (uint8_t) 3,
   /**<k_cta_cycle_count_suppress_true_warning*/ (uint8_t) 3,
   /**<k_cta_enable_modes*/ {(uint8_t)1,(uint8_t)0},
   /**<k_cta_amount_butterfly_points_in_use*/ (uint8_t) 4 /**< 4 m */,
   /**<k_unused_padding_byte_1*/ (uint8_t) 0,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_cta_min_age_obj_outside_sensor_fov*/ (uint8_t) 0,
   /**<k_cta_addit_mature_cycles_outside_sensor_fov*/ (uint8_t) 0,
   /**<k_cta_f_stop_mode_ttp*/ (boolean_T) 0,
   /**<k_cta_f_enable_heading_exp_moving_average*/ (boolean_T) 1,
   /**<k_cta_f_use_brake_gradient*/ (boolean_T) 1,
   /**<k_cta_f_brake_overriding_ctb*/ (boolean_T) 1,
   /**<k_cta_enable_ctb*/ (boolean_T) 1,
   /**<k_cta_f_use_front_corners_dist_stop*/ (boolean_T) 1 /**< 1 m */,
   /**<k_cta_f_enable_thres_crit_level_reset*/ (boolean_T) 1,
   /**<k_cta_f_use_rel_vel_isect_point_calc*/ (boolean_T) 0,
   /**<k_cta_f_use_object_min_object_age_in_cycles*/ (boolean_T) 1,
   /**<k_cta_f_use_ghost_detector*/ (boolean_T) 0,
   /**<k_cta_f_use_object_supress_counter*/ (boolean_T) 1,
   /**<k_cta_f_discard_pt_heading_when_moving*/ (boolean_T) 0,
   /**<k_cta_f_apply_path_tracking*/ (boolean_T) 0,
   /**<k_cta_f_use_heading_for_relative_velocity_calculation*/ (boolean_T) 0,
   /**<k_cta_f_calc_ttp_ego_side_enabled*/ (boolean_T) 0,
   /**<k_cta_f_calc_ttc_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_apply_heading_compensation_on_intersection_point*/ (boolean_T) 1,
   /**<k_cta_f_prevent_fall_back_to_critlevel_1*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_host_speed*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_obj_heading*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_steering_angle*/ (boolean_T) 0,
   /**<k_cta_switch*/ (boolean_T) 0,
   /**<k_cta_f_check_obstruction_probability_signal*/ (boolean_T) 0,
   /**<k_cta_f_check_reflection_signal*/ (boolean_T) 0,
   /**<k_cta_DEBUG_MODE*/ (uint16_t) 0,
   /**<k_cta_speed_thresh_for_rel_vel_calc*/ (float32_T) 2.7f /**< 2.7 m/s | 9.72 km/h */,
   /**<k_cta_ttc_calc_positive_ref_point*/ (float32_T) -100.0f /**< -100.0 s */,
   /**<k_cta_max_deceleration_value*/ (float32_T) 10.0f /**< 10.0 m/s**2 */,
   /**<k_cta_min_deceleration_value*/ (float32_T) 0.0f /**< 0.0 m/s**2 */,
   /**<k_cta_2wheel_min_speed*/ (float32_T) 0.01,
   /**<k_cta_2wheel_min_size*/ (float32_T) 0.01,
   /**<k_cta_pedestrian_min_speed*/ (float32_T) 0.01,
   /**<k_cta_pedestrian_min_size*/ (float32_T) 0.01,
   /**<k_cta_object_heading_exp_moving_average_alpha*/ (float32_T) 0.15f,
   /**<k_cta_range_to_path_segment_ghost_qualif*/ (float32_T) 3.5,
   /**<k_cta_max_seg_heading_diff_no_ghost*/ (float32_T) 0.3,
   /**<k_cta_max_heading_variance*/ (float32_T) 2.0 /**< 2.0 rad^2 */,
   /**<k_cta_dist_thres_crit_level_reset*/ (float32_T) 0.5 /**< 0.5 % */,
   /**<k_ctb_time_to_ask_for_final_brake_decel*/ (float32_T) 0.0f,
   /**<k_ctb_host_acc_weight*/ (float32_T) 1.0f,
   /**<k_ctb_braking_jerk*/ (float32_T) 5 /**< 5 m/s**3 */,
   /**<k_ctb_const_decel_after_ramp_in*/ (float32_T) 6.0 /**< 6.0 m/s**2 */,
   /**<k_ctb_ramp_in_time*/ (float32_T) 0.3 /**< 0.3 s */,
   /**<k_ctb_responsetime_brake_actuation*/ (float32_T) 0.15 /**< 0.15 s */,
   /**<k_ctb_max_braking_time*/ (float32_T) 0.9f /**< 0.9 s */,
   /**<k_ctb_min_braking_time*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ctb_event_time_buffer*/ (float32_T) 0.15 /**< 0.15 s */,
   /**<k_ctb_safety_dist_host_vel_lut*/ {(float32_T)0.28f /**< 0.28 m/s | 1.01 km/h */,(float32_T)0.56f /**< 0.56 m/s | 2.02 km/h */,(float32_T)0.69f /**< 0.69 m/s | 2.48 km/h */,(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */},
   /**<k_ctb_upper_safety_distance_thres_lut*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)1.25f /**< 1.25 m */,(float32_T)1.375f /**< 1.375 m */,(float32_T)1.5f /**< 1.5 m */},
   /**<k_ctb_lower_safety_distance_thres*/ (float32_T) 0.3 /**< 0.3 m */,
   /**<k_cta_intersection_line_host_width_percentage*/ (float32_T) 1.0f,
   /**<k_cta_min_ttc_additional_mature_qualification*/ (float32_T) 100.0 /**< 100.0 s */,
   /**<k_cta_ghost_condition_max_heading_diff_path_tracker*/ (float32_T) 0.35 /**< 0.35 rad | 20.05 deg */,
   /**<k_cta_obj_dist_to_discard_pt_info*/ (float32_T) 15.0f /**< 15.0 m */,
   /**<k_cta_min_host_speed_to_discard_pt_info*/ (float32_T) 0.02f /**< 0.02 m/s | 0.07 km/h */,
   /**<k_cta_angles_zone_definition*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)2.356f /**< 2.356 rad | 134.99 deg */},
   /**<k_cta_heading_range*/ {(float32_T)0.52f /**< 0.52 rad | 29.79 deg */,(float32_T)2.62f /**< 2.62 rad | 150.11 deg */},
   /**<k_cta_max_length_fov*/ (float32_T) 70.0 /**< 70.0 m */,
   /**<k_cta_max_speed*/ (float32_T) 7.5 /**< 7.5 m/s | 27.0 km/h */,
   /**<k_cta_rcta_host_speed_factor*/ (float32_T) 0.0,
   /**<k_cta_rel_warning_hysteresis*/ (float32_T) 0.2,
   /**<k_cta_min_rel_existence_probability*/ (float32_T) 0.85 /**< 0.85 % */,
   /**<k_cta_min_lateral_approach_speed*/ (float32_T) 0 /**< 0 m/s | 0.0 km/h */,
   /**<k_cta_min_long_point_criticality_level*/ {{(float32_T)5.0 /**< 5.0 m */,(float32_T)7.0 /**< 7.0 m */},{(float32_T)-0.5 /**< -0.5 m */,(float32_T)0.0 /**< 0.0 m */}},
   /**<k_cta_max_long_point_criticality_level*/ {{(float32_T)-0.5 /**< -0.5 m */,(float32_T)0.0 /**< 0.0 m */},{(float32_T)5.0 /**< 5.0 m */,(float32_T)7.0 /**< 7.0 m */}},
   /**<k_cta_speed_criticality_level*/ {{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */}},
   /**<k_cta_ttc_criticality_level*/ {{(float32_T)3.10 /**< 3.10 s */,(float32_T)3.10 /**< 3.10 s */},{(float32_T)3.10 /**< 3.10 s */,(float32_T)3.10 /**< 3.10 s */}},
   /**<k_cta_butterfly_lat*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)50.0 /**< 50.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_butterfly_long*/ {(float32_T)9.0 /**< 9.0 m */,(float32_T)-14.0 /**< -14.0 m */,(float32_T)-39.0 /**< -39.0 m */,(float32_T)34.0 /**< 34.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_min_speed*/ (float32_T) 1.1 /**< 1.1 m/s | 3.96 km/h */,
   /**<k_cta_stop_alert_ttp*/ (float32_T) -3.0 /**< -3.0 s */,
   /**<k_cta_stop_alert_ttc*/ (float32_T) 0.1 /**< 0.1 s */,
   /**<k_cta_ego_abs_speed_max*/ (float32_T) 4.1667 /**< 4.1667 m/s | 15.0 km/h */,
   /**<k_cta_ttc_warntrigger_late*/ (float32_T) 3.0 /**< 3.0 s */,
   /**<k_cta_ttc_warntrigger_early*/ (float32_T) 5.0 /**< 5.0 s */,
   /**<k_cta_sensor_fov_border*/ {(float32_T)0.0 /**< 0.0 rad | 0.0 deg */,(float32_T)0.0 /**< 0.0 rad | 0.0 deg */},
   /**<k_cta_host_width_sensor_fov_suppr_factor*/ (float32_T) 1.0,
   /**<k_cta_accelerationpedal_gradient_threshold_ctb*/ (float32_T) 30.0 /**< 30.0 m/s | 108.0 km/h */,
   /**<k_cta_max_object_eclipse_for_level_qualification*/ (float32_T) 1.0,
   /**<k_cta_max_obstruction_probability*/ (float32_T) 0.9,
   /**<k_cta_rcta_steer_factor_table*/ {(float32_T)1.0,(float32_T)0.9,(float32_T)0.9,(float32_T)0.8,(float32_T)0.8},
   /**<k_cta_rcta_steer_angle_table*/ {(float32_T)0.0 /**< 0.0 rad | 0.0 deg */,(float32_T)0.15 /**< 0.15 rad | 8.59 deg */,(float32_T)0.32 /**< 0.32 rad | 18.33 deg */,(float32_T)0.46 /**< 0.46 rad | 26.36 deg */,(float32_T)0.62 /**< 0.62 rad | 35.52 deg */},
   /**<k_cta_fcta_steer_factor_table*/ {(float32_T)1.0,(float32_T)0.9,(float32_T)0.9,(float32_T)0.8,(float32_T)0.7},
   /**<k_cta_fcta_steer_angle_table*/ {(float32_T)0.0 /**< 0.0 rad | 0.0 deg */,(float32_T)0.15 /**< 0.15 rad | 8.59 deg */,(float32_T)0.32 /**< 0.32 rad | 18.33 deg */,(float32_T)0.46 /**< 0.46 rad | 26.36 deg */,(float32_T)0.62 /**< 0.62 rad | 35.52 deg */},
   /**<k_cta_min_park_angle*/ (float32_T) 0.64 /**< 0.64 rad | 36.67 deg */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)27321,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)73,
   /**<Section_Size*/ (uint32_t)504
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Cta_Core_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Core_Cal_Reverse_Array_Cta_Cal(Cta_Core_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_fcta_steer_angle_table[0], sizeof(cal_dst->k_cta_fcta_steer_angle_table), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_fcta_steer_factor_table[0], sizeof(cal_dst->k_cta_fcta_steer_factor_table), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_rcta_steer_angle_table[0], sizeof(cal_dst->k_cta_rcta_steer_angle_table), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_rcta_steer_factor_table[0], sizeof(cal_dst->k_cta_rcta_steer_factor_table), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_sensor_fov_border[0], sizeof(cal_dst->k_cta_sensor_fov_border), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_butterfly_long[0], sizeof(cal_dst->k_cta_butterfly_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_butterfly_lat[0], sizeof(cal_dst->k_cta_butterfly_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_ttc_criticality_level[0], sizeof(cal_dst->k_cta_ttc_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_speed_criticality_level[0], sizeof(cal_dst->k_cta_speed_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_max_long_point_criticality_level[0], sizeof(cal_dst->k_cta_max_long_point_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_min_long_point_criticality_level[0], sizeof(cal_dst->k_cta_min_long_point_criticality_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_heading_range[0], sizeof(cal_dst->k_cta_heading_range), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_angles_zone_definition[0], sizeof(cal_dst->k_cta_angles_zone_definition), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ctb_upper_safety_distance_thres_lut[0], sizeof(cal_dst->k_ctb_upper_safety_distance_thres_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ctb_safety_dist_host_vel_lut[0], sizeof(cal_dst->k_ctb_safety_dist_host_vel_lut), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cta_enable_modes[0], sizeof(cal_dst->k_cta_enable_modes), CT_ONE_BYTE);
}
#endif /* CT_BIG_ENDIAN */


