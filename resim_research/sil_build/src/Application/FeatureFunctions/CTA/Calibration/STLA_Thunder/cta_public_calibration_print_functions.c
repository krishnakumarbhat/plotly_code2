/**
* @file cta_public_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "cta_public_calibration_t.h" // IWYU pragma: keep
#include "cta_public_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Cta_Public_Cal_Print(FILE* c_file_ptr, const Cta_Public_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ego_abs_speed_max,%f\n", p_cals->k_cta_ego_abs_speed_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_stop_alert_ttc,%f\n", p_cals->k_cta_stop_alert_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_stop_alert_ttp,%f\n", p_cals->k_cta_stop_alert_ttp);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_speed,%f\n", p_cals->k_cta_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._0_,%f\n", p_cals->k_cta_butterfly_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._1_,%f\n", p_cals->k_cta_butterfly_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._2_,%f\n", p_cals->k_cta_butterfly_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._3_,%f\n", p_cals->k_cta_butterfly_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._4_,%f\n", p_cals->k_cta_butterfly_long[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._5_,%f\n", p_cals->k_cta_butterfly_long[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._6_,%f\n", p_cals->k_cta_butterfly_long[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_long._7_,%f\n", p_cals->k_cta_butterfly_long[7]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._0_,%f\n", p_cals->k_cta_butterfly_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._1_,%f\n", p_cals->k_cta_butterfly_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._2_,%f\n", p_cals->k_cta_butterfly_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._3_,%f\n", p_cals->k_cta_butterfly_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._4_,%f\n", p_cals->k_cta_butterfly_lat[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._5_,%f\n", p_cals->k_cta_butterfly_lat[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._6_,%f\n", p_cals->k_cta_butterfly_lat[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_butterfly_lat._7_,%f\n", p_cals->k_cta_butterfly_lat[7]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ttc_criticality_level._0_0_,%f\n", p_cals->k_cta_ttc_criticality_level[0][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ttc_criticality_level._0_1_,%f\n", p_cals->k_cta_ttc_criticality_level[0][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ttc_criticality_level._1_0_,%f\n", p_cals->k_cta_ttc_criticality_level[1][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ttc_criticality_level._1_1_,%f\n", p_cals->k_cta_ttc_criticality_level[1][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_speed_criticality_level._0_0_,%f\n", p_cals->k_cta_speed_criticality_level[0][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_speed_criticality_level._0_1_,%f\n", p_cals->k_cta_speed_criticality_level[0][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_speed_criticality_level._1_0_,%f\n", p_cals->k_cta_speed_criticality_level[1][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_speed_criticality_level._1_1_,%f\n", p_cals->k_cta_speed_criticality_level[1][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_long_point_criticality_level._0_0_,%f\n", p_cals->k_cta_max_long_point_criticality_level[0][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_long_point_criticality_level._0_1_,%f\n", p_cals->k_cta_max_long_point_criticality_level[0][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_long_point_criticality_level._1_0_,%f\n", p_cals->k_cta_max_long_point_criticality_level[1][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_long_point_criticality_level._1_1_,%f\n", p_cals->k_cta_max_long_point_criticality_level[1][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_long_point_criticality_level._0_0_,%f\n", p_cals->k_cta_min_long_point_criticality_level[0][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_long_point_criticality_level._0_1_,%f\n", p_cals->k_cta_min_long_point_criticality_level[0][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_long_point_criticality_level._1_0_,%f\n", p_cals->k_cta_min_long_point_criticality_level[1][0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_long_point_criticality_level._1_1_,%f\n", p_cals->k_cta_min_long_point_criticality_level[1][1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_lateral_approach_speed,%f\n", p_cals->k_cta_min_lateral_approach_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_rel_existence_probability,%f\n", p_cals->k_cta_min_rel_existence_probability);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_rel_warning_hysteresis,%f\n", p_cals->k_cta_rel_warning_hysteresis);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_rcta_host_speed_factor,%f\n", p_cals->k_cta_rcta_host_speed_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_speed,%f\n", p_cals->k_cta_max_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_length_fov,%f\n", p_cals->k_cta_max_length_fov);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_heading_range._0_,%f\n", p_cals->k_cta_heading_range[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_heading_range._1_,%f\n", p_cals->k_cta_heading_range[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_angles_zone_definition._0_,%f\n", p_cals->k_cta_angles_zone_definition[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_angles_zone_definition._1_,%f\n", p_cals->k_cta_angles_zone_definition[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_host_speed_to_discard_pt_info,%f\n", p_cals->k_cta_min_host_speed_to_discard_pt_info);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_obj_dist_to_discard_pt_info,%f\n", p_cals->k_cta_obj_dist_to_discard_pt_info);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ghost_condition_max_heading_diff_path_tracker,%f\n", p_cals->k_cta_ghost_condition_max_heading_diff_path_tracker);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_ttc_additional_mature_qualification,%f\n", p_cals->k_cta_min_ttc_additional_mature_qualification);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_intersection_line_host_width_percentage,%f\n", p_cals->k_cta_intersection_line_host_width_percentage);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_lower_safety_distance_thres,%f\n", p_cals->k_ctb_lower_safety_distance_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_upper_safety_distance_thres_lut._0_,%f\n", p_cals->k_ctb_upper_safety_distance_thres_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_upper_safety_distance_thres_lut._1_,%f\n", p_cals->k_ctb_upper_safety_distance_thres_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_upper_safety_distance_thres_lut._2_,%f\n", p_cals->k_ctb_upper_safety_distance_thres_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_upper_safety_distance_thres_lut._3_,%f\n", p_cals->k_ctb_upper_safety_distance_thres_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_safety_dist_host_vel_lut._0_,%f\n", p_cals->k_ctb_safety_dist_host_vel_lut[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_safety_dist_host_vel_lut._1_,%f\n", p_cals->k_ctb_safety_dist_host_vel_lut[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_safety_dist_host_vel_lut._2_,%f\n", p_cals->k_ctb_safety_dist_host_vel_lut[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_safety_dist_host_vel_lut._3_,%f\n", p_cals->k_ctb_safety_dist_host_vel_lut[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_event_time_buffer,%f\n", p_cals->k_ctb_event_time_buffer);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_min_braking_time,%f\n", p_cals->k_ctb_min_braking_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_max_braking_time,%f\n", p_cals->k_ctb_max_braking_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_responsetime_brake_actuation,%f\n", p_cals->k_ctb_responsetime_brake_actuation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_ramp_in_time,%f\n", p_cals->k_ctb_ramp_in_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_const_decel_after_ramp_in,%f\n", p_cals->k_ctb_const_decel_after_ramp_in);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_braking_jerk,%f\n", p_cals->k_ctb_braking_jerk);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_host_acc_weight,%f\n", p_cals->k_ctb_host_acc_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_time_to_ask_for_final_brake_decel,%f\n", p_cals->k_ctb_time_to_ask_for_final_brake_decel);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_dist_thres_crit_level_reset,%f\n", p_cals->k_cta_dist_thres_crit_level_reset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_heading_variance,%f\n", p_cals->k_cta_max_heading_variance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_seg_heading_diff_no_ghost,%f\n", p_cals->k_cta_max_seg_heading_diff_no_ghost);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_range_to_path_segment_ghost_qualif,%f\n", p_cals->k_cta_range_to_path_segment_ghost_qualif);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_object_heading_exp_moving_average_alpha,%f\n", p_cals->k_cta_object_heading_exp_moving_average_alpha);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_pedestrian_min_size,%f\n", p_cals->k_cta_pedestrian_min_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_pedestrian_min_speed,%f\n", p_cals->k_cta_pedestrian_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_2wheel_min_size,%f\n", p_cals->k_cta_2wheel_min_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_2wheel_min_speed,%f\n", p_cals->k_cta_2wheel_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_deceleration_value,%f\n", p_cals->k_cta_min_deceleration_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_deceleration_value,%f\n", p_cals->k_cta_max_deceleration_value);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ttc_calc_positive_ref_point,%f\n", p_cals->k_cta_ttc_calc_positive_ref_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_speed_thresh_for_rel_vel_calc,%f\n", p_cals->k_cta_speed_thresh_for_rel_vel_calc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_adapt_intersect_lines_by_steering_angle,%d\n", p_cals->k_cta_f_adapt_intersect_lines_by_steering_angle);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_adapt_intersect_lines_by_obj_heading,%d\n", p_cals->k_cta_f_adapt_intersect_lines_by_obj_heading);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_adapt_intersect_lines_by_host_speed,%d\n", p_cals->k_cta_f_adapt_intersect_lines_by_host_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_prevent_fall_back_to_critlevel_1,%d\n", p_cals->k_cta_f_prevent_fall_back_to_critlevel_1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_apply_heading_compensation_on_intersection_point,%d\n", p_cals->k_cta_f_apply_heading_compensation_on_intersection_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_calc_ttc_ego_side_enabled,%d\n", p_cals->k_cta_f_calc_ttc_ego_side_enabled);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_calc_ttp_ego_side_enabled,%d\n", p_cals->k_cta_f_calc_ttp_ego_side_enabled);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_heading_for_relative_velocity_calculation,%d\n", p_cals->k_cta_f_use_heading_for_relative_velocity_calculation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_apply_path_tracking,%d\n", p_cals->k_cta_f_apply_path_tracking);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_discard_pt_heading_when_moving,%d\n", p_cals->k_cta_f_discard_pt_heading_when_moving);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_object_supress_counter,%d\n", p_cals->k_cta_f_use_object_supress_counter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_ghost_detector,%d\n", p_cals->k_cta_f_use_ghost_detector);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_object_min_object_age_in_cycles,%d\n", p_cals->k_cta_f_use_object_min_object_age_in_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_rel_vel_isect_point_calc,%d\n", p_cals->k_cta_f_use_rel_vel_isect_point_calc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_enable_thres_crit_level_reset,%d\n", p_cals->k_cta_f_enable_thres_crit_level_reset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_front_corners_dist_stop,%d\n", p_cals->k_cta_f_use_front_corners_dist_stop);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_enable_ctb,%d\n", p_cals->k_cta_enable_ctb);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_brake_overriding_ctb,%d\n", p_cals->k_cta_f_brake_overriding_ctb);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_brake_gradient,%d\n", p_cals->k_cta_f_use_brake_gradient);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_enable_heading_exp_moving_average,%d\n", p_cals->k_cta_f_enable_heading_exp_moving_average);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_stop_mode_ttp,%d\n", p_cals->k_cta_f_stop_mode_ttp);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_amount_butterfly_points_in_use,%d\n", p_cals->k_cta_amount_butterfly_points_in_use);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_enable_modes._0_,%d\n", p_cals->k_cta_enable_modes[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_enable_modes._1_,%d\n", p_cals->k_cta_enable_modes[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_cycle_count_suppress_true_warning,%d\n", p_cals->k_cta_cycle_count_suppress_true_warning);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_cycle_count_hold_true_warning,%d\n", p_cals->k_cta_cycle_count_hold_true_warning);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_object_age_check_valid,%d\n", p_cals->k_cta_min_object_age_check_valid);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_object_age_thres,%d\n", p_cals->k_cta_min_object_age_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_cycles_coasted_to_ignore,%d\n", p_cals->k_cta_cycles_coasted_to_ignore);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_object_supress_counter,%d\n", p_cals->k_cta_object_supress_counter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ghost_validation_min_age,%d\n", p_cals->k_cta_ghost_validation_min_age);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_ghost_validation_min_mature,%d\n", p_cals->k_cta_ghost_validation_min_mature);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_mature_cycles_level_qualifiction,%d\n", p_cals->k_cta_min_mature_cycles_level_qualifiction);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_additional_qualification_mature_cycles,%d\n", p_cals->k_cta_additional_qualification_mature_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_min_brake_qual_ctr_thres,%d\n", p_cals->k_ctb_min_brake_qual_ctr_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_min_brake_hold_ctr_thres,%d\n", p_cals->k_ctb_min_brake_hold_ctr_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_qual_age_obj_crossing_paths,%d\n", p_cals->k_cta_min_qual_age_obj_crossing_paths);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_cycles_valid_match_of_pot_ghost,%d\n", p_cals->k_cta_cycles_valid_match_of_pot_ghost);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_age_for_new_creation_below_long_intersection,%d\n", p_cals->k_cta_age_for_new_creation_below_long_intersection);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_G_E_line,%f\n", p_cals->k_stla_crit_zone_G_E_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_D_C_line,%f\n", p_cals->k_stla_crit_zone_D_C_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_N_Q_line,%f\n", p_cals->k_stla_crit_zone_N_Q_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_Q_QH_line,%f\n", p_cals->k_stla_crit_zone_Q_QH_line);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
