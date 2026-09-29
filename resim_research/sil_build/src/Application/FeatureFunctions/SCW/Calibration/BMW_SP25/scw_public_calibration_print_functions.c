/**
* @file scw_public_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in scw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "scw_public_calibration_t.h" // IWYU pragma: keep
#include "scw_public_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Scw_Public_Cal_Print(FILE* c_file_ptr, const Scw_Public_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_host_speed,%f\n", p_cals->k_scw_min_host_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_host_speed_hys,%f\n", p_cals->k_scw_min_host_speed_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_lat_pos_ratio,%f\n", p_cals->k_scw_max_lat_pos_ratio);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_heading._0_,%f\n", p_cals->k_scw_candidate_heading[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_heading._1_,%f\n", p_cals->k_scw_candidate_heading[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_heading_hys,%f\n", p_cals->k_scw_candidate_heading_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_yawrate,%f\n", p_cals->k_scw_candidate_yawrate);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_yawrate_hys,%f\n", p_cals->k_scw_candidate_yawrate_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_velocity._0_,%f\n", p_cals->k_scw_candidate_velocity[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_velocity._1_,%f\n", p_cals->k_scw_candidate_velocity[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_velocity_hys,%f\n", p_cals->k_scw_candidate_velocity_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_relative_velocity._0_,%f\n", p_cals->k_scw_candidate_relative_velocity[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_relative_velocity._1_,%f\n", p_cals->k_scw_candidate_relative_velocity[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_relative_vel_hys,%f\n", p_cals->k_scw_candidate_relative_vel_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_candidate_existence_probability,%f\n", p_cals->k_scw_min_candidate_existence_probability);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_exist_prob_radar_guardrail,%f\n", p_cals->k_scw_min_exist_prob_radar_guardrail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_dynamic_lat_ttc,%f\n", p_cals->k_scw_min_dynamic_lat_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_dynamic_lat_ttc,%f\n", p_cals->k_scw_max_dynamic_lat_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_guardrail_lat_ttc,%f\n", p_cals->k_scw_min_guardrail_lat_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_guardrail_lat_ttc,%f\n", p_cals->k_scw_max_guardrail_lat_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_trailer_lat_ttc_extension,%f\n", p_cals->k_scw_trailer_lat_ttc_extension);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_dynamic_lat_distance,%f\n", p_cals->k_scw_min_dynamic_lat_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_dynamic_lat_distance,%f\n", p_cals->k_scw_max_dynamic_lat_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_guardrail_lat_distance,%f\n", p_cals->k_scw_min_guardrail_lat_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_guardrail_lat_distance,%f\n", p_cals->k_scw_max_guardrail_lat_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_critical_lat_ttc_hys,%f\n", p_cals->k_scw_critical_lat_ttc_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_critical_lat_distance_hys,%f\n", p_cals->k_scw_critical_lat_distance_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_x._0_,%f\n", p_cals->k_scw_initial_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_x._1_,%f\n", p_cals->k_scw_initial_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_x._2_,%f\n", p_cals->k_scw_initial_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_x._3_,%f\n", p_cals->k_scw_initial_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_x._4_,%f\n", p_cals->k_scw_initial_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_x._5_,%f\n", p_cals->k_scw_initial_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_y._0_,%f\n", p_cals->k_scw_initial_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_y._1_,%f\n", p_cals->k_scw_initial_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_y._2_,%f\n", p_cals->k_scw_initial_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_y._3_,%f\n", p_cals->k_scw_initial_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_y._4_,%f\n", p_cals->k_scw_initial_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_initial_zone_y._5_,%f\n", p_cals->k_scw_initial_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_x_offset._0_,%f\n", p_cals->k_scw_hys_zone_x_offset[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_x_offset._1_,%f\n", p_cals->k_scw_hys_zone_x_offset[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_x_offset._2_,%f\n", p_cals->k_scw_hys_zone_x_offset[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_x_offset._3_,%f\n", p_cals->k_scw_hys_zone_x_offset[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_x_offset._4_,%f\n", p_cals->k_scw_hys_zone_x_offset[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_x_offset._5_,%f\n", p_cals->k_scw_hys_zone_x_offset[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_y_offset._0_,%f\n", p_cals->k_scw_hys_zone_y_offset[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_y_offset._1_,%f\n", p_cals->k_scw_hys_zone_y_offset[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_y_offset._2_,%f\n", p_cals->k_scw_hys_zone_y_offset[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_y_offset._3_,%f\n", p_cals->k_scw_hys_zone_y_offset[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_y_offset._4_,%f\n", p_cals->k_scw_hys_zone_y_offset[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_hys_zone_y_offset._5_,%f\n", p_cals->k_scw_hys_zone_y_offset[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_lateral_distance_default,%f\n", p_cals->k_scw_lateral_distance_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_lateral_ttc_max,%f\n", p_cals->k_scw_lateral_ttc_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_lateral_ttc_default,%f\n", p_cals->k_scw_lateral_ttc_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_ttle_max,%f\n", p_cals->k_scw_ttle_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_ttle_default,%f\n", p_cals->k_scw_ttle_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_ttp_max,%f\n", p_cals->k_scw_ttp_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_ttp_default,%f\n", p_cals->k_scw_ttp_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_trailer_zone_ext_safety_margin,%f\n", p_cals->k_scw_trailer_zone_ext_safety_margin);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_trailer_zone_ext_safety_margin_lat,%f\n", p_cals->k_scw_trailer_zone_ext_safety_margin_lat);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_zone_length,%f\n", p_cals->k_scw_max_zone_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_max_zone_width,%f\n", p_cals->k_scw_max_zone_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_adjust_zones_to_ego_size,%d\n", p_cals->k_scw_f_adjust_zones_to_ego_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_enable_via_cal,%d\n", p_cals->k_scw_f_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_dynamic_enable_via_cal,%d\n", p_cals->k_scw_f_dynamic_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_guardrail_enable_via_cal,%d\n", p_cals->k_scw_f_guardrail_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_enable,%d\n", p_cals->k_scw_f_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_enable_dynamic,%d\n", p_cals->k_scw_f_enable_dynamic);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_enable_guardrail,%d\n", p_cals->k_scw_f_enable_guardrail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_enable_trailer_zone_extension,%d\n", p_cals->k_scw_f_enable_trailer_zone_extension);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_f_enable_trailer_ttc_extension,%d\n", p_cals->k_scw_f_enable_trailer_ttc_extension);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_guardrail_age,%d\n", p_cals->k_scw_min_guardrail_age);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_guardrail_freeze_period,%d\n", p_cals->k_scw_guardrail_freeze_period);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_min_candidate_age,%d\n", p_cals->k_scw_min_candidate_age);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_candidate_mature_cycles_in_zone_threshold,%d\n", p_cals->k_scw_candidate_mature_cycles_in_zone_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_scw_guardrail_cycles_in_zone_threshold,%d\n", p_cals->k_scw_guardrail_cycles_in_zone_threshold);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
