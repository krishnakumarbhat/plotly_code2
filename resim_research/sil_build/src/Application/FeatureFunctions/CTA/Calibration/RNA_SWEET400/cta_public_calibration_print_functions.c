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
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_ttc_additional_mature_qualification,%f\n", p_cals->k_cta_min_ttc_additional_mature_qualification);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_object_eclipse_for_level_qualification,%f\n", p_cals->k_cta_max_object_eclipse_for_level_qualification);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_intersection_line_host_width_percentage,%f\n", p_cals->k_cta_intersection_line_host_width_percentage);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ctb_min_ttp,%f\n", p_cals->k_ctb_min_ttp);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_sensor_fov_border._0_,%f\n", p_cals->k_cta_sensor_fov_border[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_sensor_fov_border._1_,%f\n", p_cals->k_cta_sensor_fov_border[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_max_heading_variance,%f\n", p_cals->k_cta_max_heading_variance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_range_to_path_segment_ghost_qualif,%f\n", p_cals->k_cta_range_to_path_segment_ghost_qualif);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_ego_abs_speed_max_hys,%f\n", p_cals->k_bmw_sp25_ego_abs_speed_max_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_banner_criteria_check_time,%f\n", p_cals->k_bmw_sp25_banner_criteria_check_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_banner_time,%f\n", p_cals->k_bmw_sp25_banner_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_G_E_line,%f\n", p_cals->k_stla_crit_zone_G_E_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_D_C_line,%f\n", p_cals->k_stla_crit_zone_D_C_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_N_Q_line,%f\n", p_cals->k_stla_crit_zone_N_Q_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_stla_crit_zone_Q_QH_line,%f\n", p_cals->k_stla_crit_zone_Q_QH_line);
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
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_adapt_intersect_lines_by_steering_angle,%d\n", p_cals->k_cta_f_adapt_intersect_lines_by_steering_angle);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_adapt_intersect_lines_by_obj_heading,%d\n", p_cals->k_cta_f_adapt_intersect_lines_by_obj_heading);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_apply_heading_compensation_on_intersection_point,%d\n", p_cals->k_cta_f_apply_heading_compensation_on_intersection_point);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_calc_ttc_ego_side_enabled,%d\n", p_cals->k_cta_f_calc_ttc_ego_side_enabled);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_heading_for_relative_velocity_calculation,%d\n", p_cals->k_cta_f_use_heading_for_relative_velocity_calculation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_ghost_detector,%d\n", p_cals->k_cta_f_use_ghost_detector);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_object_min_object_age_in_cycles,%d\n", p_cals->k_cta_f_use_object_min_object_age_in_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_enable_thres_crit_level_reset,%d\n", p_cals->k_cta_f_enable_thres_crit_level_reset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_use_front_corners_dist_stop,%d\n", p_cals->k_cta_f_use_front_corners_dist_stop);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_banner_criteria_check,%d\n", p_cals->k_bmw_sp25_banner_criteria_check);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_f_stop_mode_ttp,%d\n", p_cals->k_cta_f_stop_mode_ttp);
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
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_mature_cycles_level_qualifiction,%d\n", p_cals->k_cta_min_mature_cycles_level_qualifiction);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_additional_qualification_mature_cycles,%d\n", p_cals->k_cta_additional_qualification_mature_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_age_obj_outside_sensor_fov,%d\n", p_cals->k_cta_min_age_obj_outside_sensor_fov);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cta_min_qual_age_obj_crossing_paths,%d\n", p_cals->k_cta_min_qual_age_obj_crossing_paths);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
