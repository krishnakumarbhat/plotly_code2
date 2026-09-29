
/**
* @file cta_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "cta_public_calibration_t.h"
#include "cta_public_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Public_Cal_Update_Defaults(Cta_Public_Calibration_T* cal_dst)
{
    Cta_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)28,
   /**<version*/ (uint16_t)73,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)1026,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_cta_ego_abs_speed_max*/ (float32_T) 1.94 /**< 1.94 m/s | 6.98 km/h */,
   /**<k_cta_stop_alert_ttc*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_cta_stop_alert_ttp*/ (float32_T) 0.05f /**< 0.05 s */,
   /**<k_cta_min_speed*/ (float32_T) 1.95 /**< 1.95 m/s | 7.02 km/h */,
   /**<k_cta_butterfly_long*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)-7.0 /**< -7.0 m */,(float32_T)-7.0 /**< -7.0 m */,(float32_T)-60.0 /**< -60.0 m */,(float32_T)60.0 /**< 60.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_butterfly_lat*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)85.0 /**< 85.0 m */,(float32_T)85.0 /**< 85.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_ttc_criticality_level*/ {{(float32_T)2.2 /**< 2.2 s */,(float32_T)1.4 /**< 1.4 s */},{(float32_T)2.2 /**< 2.2 s */,(float32_T)1.4 /**< 1.4 s */}},
   /**<k_cta_speed_criticality_level*/ {{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */}},
   /**<k_cta_max_long_point_criticality_level*/ {{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */},{(float32_T)3.2 /**< 3.2 m */,(float32_T)2.0 /**< 2.0 m */}},
   /**<k_cta_min_long_point_criticality_level*/ {{(float32_T)3.2 /**< 3.2 m */,(float32_T)2.0 /**< 2.0 m */},{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */}},
   /**<k_cta_min_lateral_approach_speed*/ (float32_T) 1.95 /**< 1.95 m/s | 7.02 km/h */,
   /**<k_cta_min_rel_existence_probability*/ (float32_T) 0.4 /**< 0.4 % */,
   /**<k_cta_rel_warning_hysteresis*/ (float32_T) 0.1,
   /**<k_cta_rcta_host_speed_factor*/ (float32_T) 0.0,
   /**<k_cta_max_speed*/ (float32_T) 100.0 /**< 100.0 m/s | 360.0 km/h */,
   /**<k_cta_max_length_fov*/ (float32_T) 85.0 /**< 85.0 m */,
   /**<k_cta_heading_range*/ {(float32_T)0.523599 /**< 0.523599 rad | 30.0 deg */,(float32_T)2.61799f /**< 2.61799 rad | 150.0 deg */},
   /**<k_cta_angles_zone_definition*/ {(float32_T)0.785398f /**< 0.785398 rad | 45.0 deg */,(float32_T)2.35619f /**< 2.35619 rad | 135.0 deg */},
   /**<k_cta_min_host_speed_to_discard_pt_info*/ (float32_T) 0.02f /**< 0.02 m/s | 0.07 km/h */,
   /**<k_cta_obj_dist_to_discard_pt_info*/ (float32_T) 10 /**< 10 m */,
   /**<k_cta_ghost_condition_max_heading_diff_path_tracker*/ (float32_T) 0.35 /**< 0.35 rad | 20.05 deg */,
   /**<k_cta_min_ttc_additional_mature_qualification*/ (float32_T) 100.0 /**< 100.0 s */,
   /**<k_cta_intersection_line_host_width_percentage*/ (float32_T) 0,
   /**<k_ctb_lower_safety_distance_thres*/ (float32_T) 0.3 /**< 0.3 m */,
   /**<k_ctb_upper_safety_distance_thres_lut*/ {(float32_T)1.25f /**< 1.25 m */,(float32_T)1.5f /**< 1.5 m */,(float32_T)1.625f /**< 1.625 m */,(float32_T)1.75f /**< 1.75 m */},
   /**<k_ctb_safety_dist_host_vel_lut*/ {(float32_T)0.28f /**< 0.28 m/s | 1.01 km/h */,(float32_T)0.56f /**< 0.56 m/s | 2.02 km/h */,(float32_T)0.69f /**< 0.69 m/s | 2.48 km/h */,(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */},
   /**<k_ctb_event_time_buffer*/ (float32_T) 0.2 /**< 0.2 s */,
   /**<k_ctb_min_braking_time*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ctb_max_braking_time*/ (float32_T) 0.3 /**< 0.3 s */,
   /**<k_ctb_responsetime_brake_actuation*/ (float32_T) 0.1 /**< 0.1 s */,
   /**<k_ctb_ramp_in_time*/ (float32_T) 0.1 /**< 0.1 s */,
   /**<k_ctb_const_decel_after_ramp_in*/ (float32_T) 6.0 /**< 6.0 m/s**2 */,
   /**<k_ctb_braking_jerk*/ (float32_T) 20.0 /**< 20.0 m/s**3 */,
   /**<k_ctb_host_acc_weight*/ (float32_T) 1.0f,
   /**<k_ctb_time_to_ask_for_final_brake_decel*/ (float32_T) 0.5,
   /**<k_cta_dist_thres_crit_level_reset*/ (float32_T) 0.0 /**< 0.0 % */,
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
   /**<k_cta_speed_thresh_for_rel_vel_calc*/ (float32_T) -1.0f /**< -1.0 m/s | -3.6 km/h */,
   /**<k_bmw_sp25_ego_abs_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 kph */,
   /**<k_bmw_sp25_banner_criteria_check_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_bmw_sp25_banner_time*/ (float32_T) 7.0f /**< 7.0 s */,
   /**<k_cta_f_adapt_intersect_lines_by_steering_angle*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_obj_heading*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_host_speed*/ (boolean_T) 1,
   /**<k_cta_f_prevent_fall_back_to_critlevel_1*/ (boolean_T) 1,
   /**<k_cta_f_apply_heading_compensation_on_intersection_point*/ (boolean_T) 1,
   /**<k_cta_f_calc_ttc_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_calc_ttp_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_use_heading_for_relative_velocity_calculation*/ (boolean_T) 0,
   /**<k_cta_f_apply_path_tracking*/ (boolean_T) 1,
   /**<k_cta_f_discard_pt_heading_when_moving*/ (boolean_T) 1,
   /**<k_cta_f_use_object_supress_counter*/ (boolean_T) 1,
   /**<k_cta_f_use_ghost_detector*/ (boolean_T) 1,
   /**<k_cta_f_use_object_min_object_age_in_cycles*/ (boolean_T) 1,
   /**<k_cta_f_use_rel_vel_isect_point_calc*/ (boolean_T) 0,
   /**<k_cta_f_enable_thres_crit_level_reset*/ (boolean_T) 0,
   /**<k_cta_f_use_front_corners_dist_stop*/ (boolean_T) 1 /**< 1 m */,
   /**<k_cta_enable_ctb*/ (boolean_T) 1,
   /**<k_cta_f_brake_overriding_ctb*/ (boolean_T) 1,
   /**<k_cta_f_use_brake_gradient*/ (boolean_T) 1,
   /**<k_cta_f_enable_heading_exp_moving_average*/ (boolean_T) 0,
   /**<k_cta_f_stop_mode_ttp*/ (boolean_T) 0,
   /**<k_bmw_sp25_banner_criteria_check*/ (boolean_T) 0,
   /**<k_cta_amount_butterfly_points_in_use*/ (uint8_t) 7 /**< 7 m */,
   /**<k_cta_enable_modes*/ {(uint8_t)1,(uint8_t)1},
   /**<k_cta_cycle_count_suppress_true_warning*/ (uint8_t) 1,
   /**<k_cta_cycle_count_hold_true_warning*/ (uint8_t) 4,
   /**<k_cta_min_object_age_check_valid*/ (uint8_t) 2,
   /**<k_cta_min_object_age_thres*/ (uint8_t) 3,
   /**<k_cta_cycles_coasted_to_ignore*/ (uint8_t) 3,
   /**<k_cta_object_supress_counter*/ (uint8_t) 8,
   /**<k_cta_ghost_validation_min_age*/ (uint8_t) 10,
   /**<k_cta_ghost_validation_min_mature*/ (uint8_t) 4,
   /**<k_cta_min_mature_cycles_level_qualifiction*/ (uint8_t) 2,
   /**<k_cta_additional_qualification_mature_cycles*/ (uint8_t) 0,
   /**<k_ctb_min_brake_qual_ctr_thres*/ (uint8_t) 1,
   /**<k_ctb_min_brake_hold_ctr_thres*/ (uint8_t) 2,
   /**<k_cta_min_qual_age_obj_crossing_paths*/ (uint8_t) 20,
   /**<k_cta_cycles_valid_match_of_pot_ghost*/ (uint8_t) 3,
   /**<k_cta_age_for_new_creation_below_long_intersection*/ (uint8_t) 5
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_cta_age_for_new_creation_below_long_intersection*/ (uint8_t) 5,
   /**<k_cta_cycles_valid_match_of_pot_ghost*/ (uint8_t) 3,
   /**<k_cta_min_qual_age_obj_crossing_paths*/ (uint8_t) 20,
   /**<k_ctb_min_brake_hold_ctr_thres*/ (uint8_t) 2,
   /**<k_ctb_min_brake_qual_ctr_thres*/ (uint8_t) 1,
   /**<k_cta_additional_qualification_mature_cycles*/ (uint8_t) 0,
   /**<k_cta_min_mature_cycles_level_qualifiction*/ (uint8_t) 2,
   /**<k_cta_ghost_validation_min_mature*/ (uint8_t) 4,
   /**<k_cta_ghost_validation_min_age*/ (uint8_t) 10,
   /**<k_cta_object_supress_counter*/ (uint8_t) 8,
   /**<k_cta_cycles_coasted_to_ignore*/ (uint8_t) 3,
   /**<k_cta_min_object_age_thres*/ (uint8_t) 3,
   /**<k_cta_min_object_age_check_valid*/ (uint8_t) 2,
   /**<k_cta_cycle_count_hold_true_warning*/ (uint8_t) 4,
   /**<k_cta_cycle_count_suppress_true_warning*/ (uint8_t) 1,
   /**<k_cta_enable_modes*/ {(uint8_t)1,(uint8_t)1},
   /**<k_cta_amount_butterfly_points_in_use*/ (uint8_t) 7 /**< 7 m */,
   /**<k_bmw_sp25_banner_criteria_check*/ (boolean_T) 0,
   /**<k_cta_f_stop_mode_ttp*/ (boolean_T) 0,
   /**<k_cta_f_enable_heading_exp_moving_average*/ (boolean_T) 0,
   /**<k_cta_f_use_brake_gradient*/ (boolean_T) 1,
   /**<k_cta_f_brake_overriding_ctb*/ (boolean_T) 1,
   /**<k_cta_enable_ctb*/ (boolean_T) 1,
   /**<k_cta_f_use_front_corners_dist_stop*/ (boolean_T) 1 /**< 1 m */,
   /**<k_cta_f_enable_thres_crit_level_reset*/ (boolean_T) 0,
   /**<k_cta_f_use_rel_vel_isect_point_calc*/ (boolean_T) 0,
   /**<k_cta_f_use_object_min_object_age_in_cycles*/ (boolean_T) 1,
   /**<k_cta_f_use_ghost_detector*/ (boolean_T) 1,
   /**<k_cta_f_use_object_supress_counter*/ (boolean_T) 1,
   /**<k_cta_f_discard_pt_heading_when_moving*/ (boolean_T) 1,
   /**<k_cta_f_apply_path_tracking*/ (boolean_T) 1,
   /**<k_cta_f_use_heading_for_relative_velocity_calculation*/ (boolean_T) 0,
   /**<k_cta_f_calc_ttp_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_calc_ttc_ego_side_enabled*/ (boolean_T) 1,
   /**<k_cta_f_apply_heading_compensation_on_intersection_point*/ (boolean_T) 1,
   /**<k_cta_f_prevent_fall_back_to_critlevel_1*/ (boolean_T) 1,
   /**<k_cta_f_adapt_intersect_lines_by_host_speed*/ (boolean_T) 1,
   /**<k_cta_f_adapt_intersect_lines_by_obj_heading*/ (boolean_T) 0,
   /**<k_cta_f_adapt_intersect_lines_by_steering_angle*/ (boolean_T) 0,
   /**<k_bmw_sp25_banner_time*/ (float32_T) 7.0f /**< 7.0 s */,
   /**<k_bmw_sp25_banner_criteria_check_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_bmw_sp25_ego_abs_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 kph */,
   /**<k_cta_speed_thresh_for_rel_vel_calc*/ (float32_T) -1.0f /**< -1.0 m/s | -3.6 km/h */,
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
   /**<k_cta_dist_thres_crit_level_reset*/ (float32_T) 0.0 /**< 0.0 % */,
   /**<k_ctb_time_to_ask_for_final_brake_decel*/ (float32_T) 0.5,
   /**<k_ctb_host_acc_weight*/ (float32_T) 1.0f,
   /**<k_ctb_braking_jerk*/ (float32_T) 20.0 /**< 20.0 m/s**3 */,
   /**<k_ctb_const_decel_after_ramp_in*/ (float32_T) 6.0 /**< 6.0 m/s**2 */,
   /**<k_ctb_ramp_in_time*/ (float32_T) 0.1 /**< 0.1 s */,
   /**<k_ctb_responsetime_brake_actuation*/ (float32_T) 0.1 /**< 0.1 s */,
   /**<k_ctb_max_braking_time*/ (float32_T) 0.3 /**< 0.3 s */,
   /**<k_ctb_min_braking_time*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ctb_event_time_buffer*/ (float32_T) 0.2 /**< 0.2 s */,
   /**<k_ctb_safety_dist_host_vel_lut*/ {(float32_T)0.28f /**< 0.28 m/s | 1.01 km/h */,(float32_T)0.56f /**< 0.56 m/s | 2.02 km/h */,(float32_T)0.69f /**< 0.69 m/s | 2.48 km/h */,(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */},
   /**<k_ctb_upper_safety_distance_thres_lut*/ {(float32_T)1.25f /**< 1.25 m */,(float32_T)1.5f /**< 1.5 m */,(float32_T)1.625f /**< 1.625 m */,(float32_T)1.75f /**< 1.75 m */},
   /**<k_ctb_lower_safety_distance_thres*/ (float32_T) 0.3 /**< 0.3 m */,
   /**<k_cta_intersection_line_host_width_percentage*/ (float32_T) 0,
   /**<k_cta_min_ttc_additional_mature_qualification*/ (float32_T) 100.0 /**< 100.0 s */,
   /**<k_cta_ghost_condition_max_heading_diff_path_tracker*/ (float32_T) 0.35 /**< 0.35 rad | 20.05 deg */,
   /**<k_cta_obj_dist_to_discard_pt_info*/ (float32_T) 10 /**< 10 m */,
   /**<k_cta_min_host_speed_to_discard_pt_info*/ (float32_T) 0.02f /**< 0.02 m/s | 0.07 km/h */,
   /**<k_cta_angles_zone_definition*/ {(float32_T)0.785398f /**< 0.785398 rad | 45.0 deg */,(float32_T)2.35619f /**< 2.35619 rad | 135.0 deg */},
   /**<k_cta_heading_range*/ {(float32_T)0.523599 /**< 0.523599 rad | 30.0 deg */,(float32_T)2.61799f /**< 2.61799 rad | 150.0 deg */},
   /**<k_cta_max_length_fov*/ (float32_T) 85.0 /**< 85.0 m */,
   /**<k_cta_max_speed*/ (float32_T) 100.0 /**< 100.0 m/s | 360.0 km/h */,
   /**<k_cta_rcta_host_speed_factor*/ (float32_T) 0.0,
   /**<k_cta_rel_warning_hysteresis*/ (float32_T) 0.1,
   /**<k_cta_min_rel_existence_probability*/ (float32_T) 0.4 /**< 0.4 % */,
   /**<k_cta_min_lateral_approach_speed*/ (float32_T) 1.95 /**< 1.95 m/s | 7.02 km/h */,
   /**<k_cta_min_long_point_criticality_level*/ {{(float32_T)3.2 /**< 3.2 m */,(float32_T)2.0 /**< 2.0 m */},{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */}},
   /**<k_cta_max_long_point_criticality_level*/ {{(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */},{(float32_T)3.2 /**< 3.2 m */,(float32_T)2.0 /**< 2.0 m */}},
   /**<k_cta_speed_criticality_level*/ {{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},{(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */}},
   /**<k_cta_ttc_criticality_level*/ {{(float32_T)2.2 /**< 2.2 s */,(float32_T)1.4 /**< 1.4 s */},{(float32_T)2.2 /**< 2.2 s */,(float32_T)1.4 /**< 1.4 s */}},
   /**<k_cta_butterfly_lat*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)85.0 /**< 85.0 m */,(float32_T)85.0 /**< 85.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_butterfly_long*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)-7.0 /**< -7.0 m */,(float32_T)-7.0 /**< -7.0 m */,(float32_T)-60.0 /**< -60.0 m */,(float32_T)60.0 /**< 60.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_cta_min_speed*/ (float32_T) 1.95 /**< 1.95 m/s | 7.02 km/h */,
   /**<k_cta_stop_alert_ttp*/ (float32_T) 0.05f /**< 0.05 s */,
   /**<k_cta_stop_alert_ttc*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_cta_ego_abs_speed_max*/ (float32_T) 1.94 /**< 1.94 m/s | 6.98 km/h */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)1026,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)73,
   /**<Section_Size*/ (uint32_t)28
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Cta_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Public_Cal_Reverse_Array_Cta_Cal(Cta_Public_Calibration_T* cal_dst)
{
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


