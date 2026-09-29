/**
* @file lcda_core_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "lcda_core_calibration_t.h" // IWYU pragma: keep
#include "lcda_core_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Lcda_Core_Cal_Print(FILE* c_file_ptr, const Lcda_Core_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone,%f\n", p_cals->k_lcda_lc_intention_guardrail_min_lat_pos_to_use_small_zone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_lane_change_intention_vel_lat_hys,%f\n", p_cals->k_bsw_lane_change_intention_vel_lat_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_warntrigger_early,%f\n", p_cals->k_bsw_warntrigger_early);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_warntrigger_late,%f\n", p_cals->k_bsw_warntrigger_late);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_trailer_zone_min_width,%f\n", p_cals->k_bsw_trailer_zone_min_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_guardrail_distance_safety_margin,%f\n", p_cals->k_bsw_guardrail_distance_safety_margin);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_warntrigger_early,%f\n", p_cals->k_cvw_warntrigger_early);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_warntrigger_late,%f\n", p_cals->k_cvw_warntrigger_late);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_zone_hys_obj_width_correction,%f\n", p_cals->k_zone_hys_obj_width_correction);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_curve_zone_factor_outer,%f\n", p_cals->k_cvw_curve_zone_factor_outer);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_curve_zone_factor_inner,%f\n", p_cals->k_cvw_curve_zone_factor_inner);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_time_obj_start_decel_after_lane_change,%f\n", p_cals->k_cvw_time_obj_start_decel_after_lane_change);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_time_diff_after_obj_decel,%f\n", p_cals->k_cvw_time_diff_after_obj_decel);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_crit_dist_hys_factor,%f\n", p_cals->k_cvw_crit_dist_hys_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_crit_dist_additive_hys,%f\n", p_cals->k_cvw_crit_dist_additive_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_critical_obj_decel_after_lane_change,%f\n", p_cals->k_cvw_critical_obj_decel_after_lane_change);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_x._0_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_x._1_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_x._2_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_x._3_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_x._4_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_x._5_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_y._0_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_y._1_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_y._2_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_y._3_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_y._4_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_small_y._5_,%f\n", p_cals->k_cvw_lane_change_intention_zone_small_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_hys_y._0_,%f\n", p_cals->k_cvw_lane_change_intention_zone_hys_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_hys_y._1_,%f\n", p_cals->k_cvw_lane_change_intention_zone_hys_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_hys_y._2_,%f\n", p_cals->k_cvw_lane_change_intention_zone_hys_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_hys_y._3_,%f\n", p_cals->k_cvw_lane_change_intention_zone_hys_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_hys_y._4_,%f\n", p_cals->k_cvw_lane_change_intention_zone_hys_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_hys_y._5_,%f\n", p_cals->k_cvw_lane_change_intention_zone_hys_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_y1_hys,%f\n", p_cals->k_bsw_y1_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_y0_hys,%f\n", p_cals->k_bsw_y0_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_x1_hys,%f\n", p_cals->k_bsw_x1_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_x0_hys,%f\n", p_cals->k_bsw_x0_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y_hys._0_,%f\n", p_cals->k_slc_zone_y_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y_hys._1_,%f\n", p_cals->k_slc_zone_y_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y_hys._2_,%f\n", p_cals->k_slc_zone_y_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y_hys._3_,%f\n", p_cals->k_slc_zone_y_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y_hys._4_,%f\n", p_cals->k_slc_zone_y_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y_hys._5_,%f\n", p_cals->k_slc_zone_y_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_max_obj_eclipse,%f\n", p_cals->k_slc_max_obj_eclipse);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_critical_lat_ttc_hys,%f\n", p_cals->k_slc_critical_lat_ttc_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_critical_lon_ttc_hys,%f\n", p_cals->k_slc_critical_lon_ttc_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_warntrigger_TTC_lat_late,%f\n", p_cals->k_slc_warntrigger_TTC_lat_late);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_warntrigger_TTC_lat_early,%f\n", p_cals->k_slc_warntrigger_TTC_lat_early);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_warntrigger_TTC_lon_late,%f\n", p_cals->k_slc_warntrigger_TTC_lon_late);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_warntrigger_TTC_lon_early,%f\n", p_cals->k_slc_warntrigger_TTC_lon_early);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_obj_lc_effective_speed_min,%f\n", p_cals->k_slc_obj_lc_effective_speed_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_x._0_,%f\n", p_cals->k_elc_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_x._1_,%f\n", p_cals->k_elc_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_x._2_,%f\n", p_cals->k_elc_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_x._3_,%f\n", p_cals->k_elc_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_x._4_,%f\n", p_cals->k_elc_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_x._5_,%f\n", p_cals->k_elc_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y._0_,%f\n", p_cals->k_elc_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y._1_,%f\n", p_cals->k_elc_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y._2_,%f\n", p_cals->k_elc_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y._3_,%f\n", p_cals->k_elc_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y._4_,%f\n", p_cals->k_elc_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y._5_,%f\n", p_cals->k_elc_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y_hys._0_,%f\n", p_cals->k_elc_zone_y_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y_hys._1_,%f\n", p_cals->k_elc_zone_y_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y_hys._2_,%f\n", p_cals->k_elc_zone_y_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y_hys._3_,%f\n", p_cals->k_elc_zone_y_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y_hys._4_,%f\n", p_cals->k_elc_zone_y_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_zone_y_hys._5_,%f\n", p_cals->k_elc_zone_y_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_max_curvi_heading_abs,%f\n", p_cals->k_elc_max_curvi_heading_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_min_obj_curvi_long_vel_abs,%f\n", p_cals->k_elc_min_obj_curvi_long_vel_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_critical_longitudinal_ttc,%f\n", p_cals->k_elc_critical_longitudinal_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_critical_longitudinal_ttc_hys,%f\n", p_cals->k_elc_critical_longitudinal_ttc_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_obj_safe_deceleration_threshold,%f\n", p_cals->k_elc_obj_safe_deceleration_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_obj_safe_deceleration_threshold_hys,%f\n", p_cals->k_elc_obj_safe_deceleration_threshold_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_lane_width_city,%f\n", p_cals->k_lm_lane_width_city);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_lane_width_highway,%f\n", p_cals->k_lm_lane_width_highway);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lm_lane_width_us_default,%f\n", p_cals->k_lcda_lm_lane_width_us_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lm_lane_width_japan_default,%f\n", p_cals->k_lcda_lm_lane_width_japan_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lm_lane_width_china_default,%f\n", p_cals->k_lcda_lm_lane_width_china_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lm_lane_width_korea_default,%f\n", p_cals->k_lcda_lm_lane_width_korea_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lm_lane_width_germany_default,%f\n", p_cals->k_lcda_lm_lane_width_germany_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lm_lane_width_defaultcountry_default,%f\n", p_cals->k_lcda_lm_lane_width_defaultcountry_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_lane_center_offset_default,%f\n", p_cals->k_lm_lane_center_offset_default);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_speed_hway,%f\n", p_cals->k_lm_min_speed_hway);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_hys_delta_speed_hway,%f\n", p_cals->k_lm_hys_delta_speed_hway);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_yawrate_city_abs,%f\n", p_cals->k_lm_min_yawrate_city_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_hys_delta_yawrate_city_abs,%f\n", p_cals->k_lm_hys_delta_yawrate_city_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_lane_exist_prob_percent,%f\n", p_cals->k_lm_min_lane_exist_prob_percent);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_plausible_lane_width,%f\n", p_cals->k_lm_min_plausible_lane_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_max_plausible_lane_width,%f\n", p_cals->k_lm_max_plausible_lane_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_max_plausible_lc_offset_factor,%f\n", p_cals->k_lm_max_plausible_lc_offset_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_exist_prop,%f\n", p_cals->k_lcda_min_exist_prop);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_host_activation_speed_min,%f\n", p_cals->k_lcda_host_activation_speed_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_host_activation_speed_min_hys,%f\n", p_cals->k_lcda_host_activation_speed_min_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_host_activation_speed_max,%f\n", p_cals->k_lcda_host_activation_speed_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_host_activation_speed_max_hys,%f\n", p_cals->k_lcda_host_activation_speed_max_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_distance_traveled_scale_factor,%f\n", p_cals->k_lcda_distance_traveled_scale_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_curve_radius,%f\n", p_cals->k_lcda_min_curve_radius);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_curve_radius_hys,%f\n", p_cals->k_lcda_min_curve_radius_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_curve_radius_threshold_for_zone_adaptation,%f\n", p_cals->k_lcda_curve_radius_threshold_for_zone_adaptation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_lane_width,%f\n", p_cals->k_lcda_min_lane_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_max_lane_width,%f\n", p_cals->k_lcda_max_lane_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_ego_vehicle_width,%f\n", p_cals->k_lcda_min_ego_vehicle_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_max_ego_vehicle_width,%f\n", p_cals->k_lcda_max_ego_vehicle_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_ego_vehicle_length,%f\n", p_cals->k_lcda_min_ego_vehicle_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_max_ego_vehicle_length,%f\n", p_cals->k_lcda_max_ego_vehicle_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_zone_intersect_critical_point_lateral_ratio,%f\n", p_cals->k_lcda_zone_intersect_critical_point_lateral_ratio);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_exist_prob_lc_intention_hys_offset,%f\n", p_cals->k_lcda_exist_prob_lc_intention_hys_offset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_exist_prob_hys_offset,%f\n", p_cals->k_lcda_exist_prob_hys_offset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lane_change_intention_vel_lat_thresh,%f\n", p_cals->k_lcda_lane_change_intention_vel_lat_thresh);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_lane_change_intention_pos_lat_thres,%f\n", p_cals->k_bsw_lane_change_intention_pos_lat_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_lane_change_intention_pos_long_thres,%f\n", p_cals->k_bsw_lane_change_intention_pos_long_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_ego_lane_effective_lane_width_factor,%f\n", p_cals->k_lcda_ego_lane_effective_lane_width_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_exist_prob_lc_intention,%f\n", p_cals->k_lcda_min_exist_prob_lc_intention);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x._0_,%f\n", p_cals->k_bsw_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x._1_,%f\n", p_cals->k_bsw_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x._2_,%f\n", p_cals->k_bsw_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x._3_,%f\n", p_cals->k_bsw_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x._4_,%f\n", p_cals->k_bsw_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x._5_,%f\n", p_cals->k_bsw_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y._0_,%f\n", p_cals->k_bsw_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y._1_,%f\n", p_cals->k_bsw_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y._2_,%f\n", p_cals->k_bsw_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y._3_,%f\n", p_cals->k_bsw_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y._4_,%f\n", p_cals->k_bsw_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y._5_,%f\n", p_cals->k_bsw_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x_hys._0_,%f\n", p_cals->k_bsw_zone_x_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x_hys._1_,%f\n", p_cals->k_bsw_zone_x_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x_hys._2_,%f\n", p_cals->k_bsw_zone_x_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x_hys._3_,%f\n", p_cals->k_bsw_zone_x_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x_hys._4_,%f\n", p_cals->k_bsw_zone_x_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_x_hys._5_,%f\n", p_cals->k_bsw_zone_x_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys._0_,%f\n", p_cals->k_bsw_zone_y_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys._1_,%f\n", p_cals->k_bsw_zone_y_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys._2_,%f\n", p_cals->k_bsw_zone_y_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys._3_,%f\n", p_cals->k_bsw_zone_y_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys._4_,%f\n", p_cals->k_bsw_zone_y_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys._5_,%f\n", p_cals->k_bsw_zone_y_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys_min,%f\n", p_cals->k_bsw_zone_y_hys_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_y_hys_max,%f\n", p_cals->k_bsw_zone_y_hys_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x._0_,%f\n", p_cals->k_bsw_fixed_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x._1_,%f\n", p_cals->k_bsw_fixed_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x._2_,%f\n", p_cals->k_bsw_fixed_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x._3_,%f\n", p_cals->k_bsw_fixed_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x._4_,%f\n", p_cals->k_bsw_fixed_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x._5_,%f\n", p_cals->k_bsw_fixed_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y._0_,%f\n", p_cals->k_bsw_fixed_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y._1_,%f\n", p_cals->k_bsw_fixed_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y._2_,%f\n", p_cals->k_bsw_fixed_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y._3_,%f\n", p_cals->k_bsw_fixed_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y._4_,%f\n", p_cals->k_bsw_fixed_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y._5_,%f\n", p_cals->k_bsw_fixed_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x_hys._0_,%f\n", p_cals->k_bsw_fixed_zone_x_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x_hys._1_,%f\n", p_cals->k_bsw_fixed_zone_x_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x_hys._2_,%f\n", p_cals->k_bsw_fixed_zone_x_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x_hys._3_,%f\n", p_cals->k_bsw_fixed_zone_x_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x_hys._4_,%f\n", p_cals->k_bsw_fixed_zone_x_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_x_hys._5_,%f\n", p_cals->k_bsw_fixed_zone_x_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y_hys._0_,%f\n", p_cals->k_bsw_fixed_zone_y_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y_hys._1_,%f\n", p_cals->k_bsw_fixed_zone_y_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y_hys._2_,%f\n", p_cals->k_bsw_fixed_zone_y_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y_hys._3_,%f\n", p_cals->k_bsw_fixed_zone_y_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y_hys._4_,%f\n", p_cals->k_bsw_fixed_zone_y_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fixed_zone_y_hys._5_,%f\n", p_cals->k_bsw_fixed_zone_y_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_lateral_distance_zone,%f\n", p_cals->k_bsw_lateral_distance_zone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_overlap_area_threshold,%f\n", p_cals->k_bsw_overlap_area_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fallback_rel_vel_thres,%f\n", p_cals->k_bsw_fallback_rel_vel_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fallback_rel_vel_thres_hys,%f\n", p_cals->k_bsw_fallback_rel_vel_thres_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed._0_,%f\n", p_cals->k_bsw_dynzone_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed._1_,%f\n", p_cals->k_bsw_dynzone_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed._2_,%f\n", p_cals->k_bsw_dynzone_speed[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed._3_,%f\n", p_cals->k_bsw_dynzone_speed[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed._4_,%f\n", p_cals->k_bsw_dynzone_speed[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed._5_,%f\n", p_cals->k_bsw_dynzone_speed[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range._0_,%f\n", p_cals->k_bsw_dynzone_range[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range._1_,%f\n", p_cals->k_bsw_dynzone_range[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range._2_,%f\n", p_cals->k_bsw_dynzone_range[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range._3_,%f\n", p_cals->k_bsw_dynzone_range[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range._4_,%f\n", p_cals->k_bsw_dynzone_range[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range._5_,%f\n", p_cals->k_bsw_dynzone_range[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_min_exist_prob_radar_guardrail,%f\n", p_cals->k_min_exist_prob_radar_guardrail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_min_exist_prob_camera_guardrail,%f\n", p_cals->k_min_exist_prob_camera_guardrail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._0_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._1_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._2_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._3_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._4_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._5_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._6_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback._7_,%f\n", p_cals->k_bsw_dynzone_speed_dropback[7]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_speed_dropback_max,%f\n", p_cals->k_bsw_dynzone_speed_dropback_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._0_,%f\n", p_cals->k_bsw_dynzone_range_dropback[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._1_,%f\n", p_cals->k_bsw_dynzone_range_dropback[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._2_,%f\n", p_cals->k_bsw_dynzone_range_dropback[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._3_,%f\n", p_cals->k_bsw_dynzone_range_dropback[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._4_,%f\n", p_cals->k_bsw_dynzone_range_dropback[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._5_,%f\n", p_cals->k_bsw_dynzone_range_dropback[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._6_,%f\n", p_cals->k_bsw_dynzone_range_dropback[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_range_dropback._7_,%f\n", p_cals->k_bsw_dynzone_range_dropback[7]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_n_line_position_for_long_object_sot_scenario,%f\n", p_cals->k_bsw_n_line_position_for_long_object_sot_scenario);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_line_to_stop_TOS_alert,%f\n", p_cals->k_bsw_line_to_stop_TOS_alert);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_min_length_long_object,%f\n", p_cals->k_bsw_min_length_long_object);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_min_length_long_object_hys,%f\n", p_cals->k_bsw_min_length_long_object_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_suppress_late_warning_max_time_till_leave,%f\n", p_cals->k_bsw_suppress_late_warning_max_time_till_leave);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_suppress_late_warning_min_pos_behind_host,%f\n", p_cals->k_bsw_suppress_late_warning_min_pos_behind_host);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_suppress_late_warning_max_rel_vel,%f\n", p_cals->k_bsw_suppress_late_warning_max_rel_vel);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_max_heading_abs,%f\n", p_cals->k_bsw_max_heading_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_max_heading_abs_hysteresis,%f\n", p_cals->k_bsw_max_heading_abs_hysteresis);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_min_obj_long_vel,%f\n", p_cals->k_bsw_min_obj_long_vel);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_max_obj_long_vel,%f\n", p_cals->k_bsw_max_obj_long_vel);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_min_obj_long_vel_hysteresis,%f\n", p_cals->k_bsw_min_obj_long_vel_hysteresis);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_max_obj_long_vel_hysteresis,%f\n", p_cals->k_bsw_max_obj_long_vel_hysteresis);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_trailer_zone_ext_safety_margin,%f\n", p_cals->k_bsw_trailer_zone_ext_safety_margin);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_trailer_zone_ext_safety_margin_hys,%f\n", p_cals->k_bsw_trailer_zone_ext_safety_margin_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_x._0_,%f\n", p_cals->k_cvw_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_x._1_,%f\n", p_cals->k_cvw_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_x._2_,%f\n", p_cals->k_cvw_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_x._3_,%f\n", p_cals->k_cvw_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_x._4_,%f\n", p_cals->k_cvw_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_x._5_,%f\n", p_cals->k_cvw_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y._0_,%f\n", p_cals->k_cvw_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y._1_,%f\n", p_cals->k_cvw_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y._2_,%f\n", p_cals->k_cvw_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y._3_,%f\n", p_cals->k_cvw_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y._4_,%f\n", p_cals->k_cvw_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y._5_,%f\n", p_cals->k_cvw_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys._0_,%f\n", p_cals->k_cvw_zone_y_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys._1_,%f\n", p_cals->k_cvw_zone_y_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys._2_,%f\n", p_cals->k_cvw_zone_y_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys._3_,%f\n", p_cals->k_cvw_zone_y_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys._4_,%f\n", p_cals->k_cvw_zone_y_hys[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys._5_,%f\n", p_cals->k_cvw_zone_y_hys[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys_max,%f\n", p_cals->k_cvw_zone_y_hys_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_y_hys_min,%f\n", p_cals->k_cvw_zone_y_hys_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_max_range,%f\n", p_cals->k_lcda_max_range);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_gap_bridge,%f\n", p_cals->k_cvw_gap_bridge);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_candidate_ttc,%f\n", p_cals->k_cvw_candidate_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_ttc,%f\n", p_cals->k_cvw_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_ttc_hys,%f\n", p_cals->k_cvw_ttc_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_max_curvi_heading_abs,%f\n", p_cals->k_cvw_max_curvi_heading_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_min_obj_curvi_long_vel,%f\n", p_cals->k_cvw_min_obj_curvi_long_vel);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_x._0_,%f\n", p_cals->k_cvw_lane_change_intention_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_x._1_,%f\n", p_cals->k_cvw_lane_change_intention_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_x._2_,%f\n", p_cals->k_cvw_lane_change_intention_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_x._3_,%f\n", p_cals->k_cvw_lane_change_intention_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_x._4_,%f\n", p_cals->k_cvw_lane_change_intention_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_x._5_,%f\n", p_cals->k_cvw_lane_change_intention_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_y._0_,%f\n", p_cals->k_cvw_lane_change_intention_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_y._1_,%f\n", p_cals->k_cvw_lane_change_intention_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_y._2_,%f\n", p_cals->k_cvw_lane_change_intention_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_y._3_,%f\n", p_cals->k_cvw_lane_change_intention_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_y._4_,%f\n", p_cals->k_cvw_lane_change_intention_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_lane_change_intention_zone_y._5_,%f\n", p_cals->k_cvw_lane_change_intention_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_y_width1,%f\n", p_cals->k_cvw_y_width1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_x_length1,%f\n", p_cals->k_cvw_x_length1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_y1,%f\n", p_cals->k_cvw_y1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_x_length0,%f\n", p_cals->k_cvw_x_length0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_y_width0,%f\n", p_cals->k_cvw_y_width0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_y0,%f\n", p_cals->k_cvw_y0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_x0,%f\n", p_cals->k_cvw_x0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_y_width,%f\n", p_cals->k_bsw_y_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_y0,%f\n", p_cals->k_bsw_y0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_x_length,%f\n", p_cals->k_bsw_x_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_x0,%f\n", p_cals->k_bsw_x0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lka_ov_zone_width,%f\n", p_cals->k_lka_ov_zone_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_x._0_,%f\n", p_cals->k_slc_zone_x[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_x._1_,%f\n", p_cals->k_slc_zone_x[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_x._2_,%f\n", p_cals->k_slc_zone_x[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_x._3_,%f\n", p_cals->k_slc_zone_x[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_x._4_,%f\n", p_cals->k_slc_zone_x[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_x._5_,%f\n", p_cals->k_slc_zone_x[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y._0_,%f\n", p_cals->k_slc_zone_y[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y._1_,%f\n", p_cals->k_slc_zone_y[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y._2_,%f\n", p_cals->k_slc_zone_y[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y._3_,%f\n", p_cals->k_slc_zone_y[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y._4_,%f\n", p_cals->k_slc_zone_y[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_zone_y._5_,%f\n", p_cals->k_slc_zone_y[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_max_curvi_heading_abs,%f\n", p_cals->k_slc_max_curvi_heading_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_min_obj_curvi_long_vel_abs,%f\n", p_cals->k_slc_min_obj_curvi_long_vel_abs);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_critical_lat_ttc,%f\n", p_cals->k_slc_critical_lat_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_critical_lon_ttc,%f\n", p_cals->k_slc_critical_lon_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lateral_ttc_lookup._0_,%f\n", p_cals->k_slc_lateral_ttc_lookup[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lateral_ttc_lookup._1_,%f\n", p_cals->k_slc_lateral_ttc_lookup[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lateral_ttc_lookup._2_,%f\n", p_cals->k_slc_lateral_ttc_lookup[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lateral_ttc_lookup._3_,%f\n", p_cals->k_slc_lateral_ttc_lookup[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lateral_ttc_lookup._4_,%f\n", p_cals->k_slc_lateral_ttc_lookup[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lateral_ttc_lookup._5_,%f\n", p_cals->k_slc_lateral_ttc_lookup[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lane_change_prob_lookup._0_,%f\n", p_cals->k_slc_lane_change_prob_lookup[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lane_change_prob_lookup._1_,%f\n", p_cals->k_slc_lane_change_prob_lookup[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lane_change_prob_lookup._2_,%f\n", p_cals->k_slc_lane_change_prob_lookup[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lane_change_prob_lookup._3_,%f\n", p_cals->k_slc_lane_change_prob_lookup[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lane_change_prob_lookup._4_,%f\n", p_cals->k_slc_lane_change_prob_lookup[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lane_change_prob_lookup._5_,%f\n", p_cals->k_slc_lane_change_prob_lookup[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lookup_ego_speed._0_,%f\n", p_cals->k_slc_lookup_ego_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lookup_ego_speed._1_,%f\n", p_cals->k_slc_lookup_ego_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lookup_ego_speed._2_,%f\n", p_cals->k_slc_lookup_ego_speed[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lookup_ego_overlap_offset._0_,%f\n", p_cals->k_slc_lookup_ego_overlap_offset[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lookup_ego_overlap_offset._1_,%f\n", p_cals->k_slc_lookup_ego_overlap_offset[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_lookup_ego_overlap_offset._2_,%f\n", p_cals->k_slc_lookup_ego_overlap_offset[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_obj_max_rel_vel_thresh,%f\n", p_cals->k_bsw_obj_max_rel_vel_thresh);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_obj_max_rel_vel_hys,%f\n", p_cals->k_bsw_obj_max_rel_vel_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_front_ego_side_x,%f\n", p_cals->k_bsw_zone_front_ego_side_x);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_front_ego_side_y,%f\n", p_cals->k_bsw_zone_front_ego_side_y);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_rear_outer_side_x,%f\n", p_cals->k_bsw_zone_rear_outer_side_x);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_rear_outer_side_y,%f\n", p_cals->k_bsw_zone_rear_outer_side_y);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_front_ego_side_x_hys,%f\n", p_cals->k_bsw_zone_front_ego_side_x_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_front_ego_side_y_hys,%f\n", p_cals->k_bsw_zone_front_ego_side_y_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_rear_outer_side_x_hys,%f\n", p_cals->k_bsw_zone_rear_outer_side_x_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_zone_rear_outer_side_y_hys,%f\n", p_cals->k_bsw_zone_rear_outer_side_y_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_pedestrian_min_size,%f\n", p_cals->k_lcda_pedestrian_min_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_pedestrian_min_speed,%f\n", p_cals->k_lcda_pedestrian_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_2wheel_min_size,%f\n", p_cals->k_lcda_2wheel_min_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_2wheel_min_speed,%f\n", p_cals->k_lcda_2wheel_min_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_object_position_correction_delay_time._0_,%f\n", p_cals->k_bsw_object_position_correction_delay_time[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_object_position_correction_delay_time._1_,%f\n", p_cals->k_bsw_object_position_correction_delay_time[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_min_speed_for_tos_scenario,%f\n", p_cals->k_bsw_min_speed_for_tos_scenario);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_min_object_curvi_relative_speed._0_,%f\n", p_cals->k_cvw_min_object_curvi_relative_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_min_object_curvi_relative_speed._1_,%f\n", p_cals->k_cvw_min_object_curvi_relative_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_min_object_curvi_relative_speed._2_,%f\n", p_cals->k_cvw_min_object_curvi_relative_speed[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_max_object_curvi_relative_speed,%f\n", p_cals->k_cvw_max_object_curvi_relative_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_object_curvi_relative_speed_hys,%f\n", p_cals->k_cvw_object_curvi_relative_speed_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_cvw_ttc_const._0_,%f\n", p_cals->k_lcda_cvw_ttc_const[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_cvw_ttc_const._1_,%f\n", p_cals->k_lcda_cvw_ttc_const[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_cvw_ttc_const._2_,%f\n", p_cals->k_lcda_cvw_ttc_const[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_cvw_ttc_accel._0_,%f\n", p_cals->k_lcda_cvw_ttc_accel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_cvw_ttc_accel._1_,%f\n", p_cals->k_lcda_cvw_ttc_accel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_cvw_ttc_accel._2_,%f\n", p_cals->k_lcda_cvw_ttc_accel[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_dyn_cvw_ttc_speed_parameter,%f\n", p_cals->k_lcda_dyn_cvw_ttc_speed_parameter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_object_position_correction_threshold,%f\n", p_cals->k_bsw_object_position_correction_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh,%f\n", p_cals->k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_dyn_cvw_ttc_compens_time._0_,%f\n", p_cals->k_lcda_dyn_cvw_ttc_compens_time[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_dyn_cvw_ttc_compens_time._1_,%f\n", p_cals->k_lcda_dyn_cvw_ttc_compens_time[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._0_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._1_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._2_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._3_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._4_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._5_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._6_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._7_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[7]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._8_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[8]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_rel_vel._9_,%f\n", p_cals->k_bsw_dynzone_object_rel_vel[9]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._0_,%f\n", p_cals->k_bsw_dynzone_object_range[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._1_,%f\n", p_cals->k_bsw_dynzone_object_range[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._2_,%f\n", p_cals->k_bsw_dynzone_object_range[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._3_,%f\n", p_cals->k_bsw_dynzone_object_range[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._4_,%f\n", p_cals->k_bsw_dynzone_object_range[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._5_,%f\n", p_cals->k_bsw_dynzone_object_range[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._6_,%f\n", p_cals->k_bsw_dynzone_object_range[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._7_,%f\n", p_cals->k_bsw_dynzone_object_range[7]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._8_,%f\n", p_cals->k_bsw_dynzone_object_range[8]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_dynzone_object_range._9_,%f\n", p_cals->k_bsw_dynzone_object_range[9]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_count_in_state,%d\n", p_cals->k_lm_min_count_in_state);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present,%d\n", p_cals->k_lcda_f_lc_intention_use_small_zone_if_no_guardrail_present);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer,%d\n", p_cals->k_bsw_f_zone_extension_by_diff_width_host_vs_trailer);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side,%d\n", p_cals->k_bsw_f_enable_trailer_zone_adjustment_on_ego_side);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side,%d\n", p_cals->k_bsw_f_enable_trailer_zone_adjustment_on_outer_side);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_enable,%d\n", p_cals->k_elc_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_enable_via_cal,%d\n", p_cals->k_elc_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_use_default_warntrigger_hmi,%d\n", p_cals->k_lcda_use_default_warntrigger_hmi);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_use_default_lane_information,%d\n", p_cals->k_lm_use_default_lane_information);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_enable_use_navigation_data,%d\n", p_cals->k_lm_enable_use_navigation_data);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_enable_use_camera_data,%d\n", p_cals->k_lm_enable_use_camera_data);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_enable_use_vehicle_dyn,%d\n", p_cals->k_lm_enable_use_vehicle_dyn);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_enable,%d\n", p_cals->k_lcda_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_enable_via_cal,%d\n", p_cals->k_lcda_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_disable_due_to_small_curve_radius,%d\n", p_cals->k_lcda_f_disable_due_to_small_curve_radius);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_allow_track_status_new,%d\n", p_cals->k_lcda_allow_track_status_new);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_obj_in_ego_lane_check,%d\n", p_cals->k_lcda_f_enable_obj_in_ego_lane_check);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_suppress_alert_object_no_lane_change_intention,%d\n", p_cals->k_lcda_f_enable_suppress_alert_object_no_lane_change_intention);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge,%d\n", p_cals->k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_enable_adv_pos_data_lane_change_intention,%d\n", p_cals->k_bsw_f_enable_adv_pos_data_lane_change_intention);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_alert_obj_in_ego_lane,%d\n", p_cals->k_lcda_f_enable_alert_obj_in_ego_lane);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_ego_lane_check_center_point_only,%d\n", p_cals->k_lcda_ego_lane_check_center_point_only);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_uses_cvw_alert_state_enabled,%d\n", p_cals->k_bsw_uses_cvw_alert_state_enabled);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_overlap_area_check_enable,%d\n", p_cals->k_bsw_overlap_area_check_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_use_curvi_coordinates,%d\n", p_cals->k_bsw_use_curvi_coordinates);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable,%d\n", p_cals->k_bsw_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable_via_cal,%d\n", p_cals->k_bsw_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable_zone_front_boundary_specific_conditions,%d\n", p_cals->k_bsw_enable_zone_front_boundary_specific_conditions);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable_specific_front_sot_conditions,%d\n", p_cals->k_bsw_enable_specific_front_sot_conditions);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_hold_alert_long_object,%d\n", p_cals->k_bsw_hold_alert_long_object);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable_factor_based_host_speed_adjustment,%d\n", p_cals->k_bsw_enable_factor_based_host_speed_adjustment);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable_dynspeed_zone,%d\n", p_cals->k_bsw_enable_dynspeed_zone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_enable_trailer_zone_extension,%d\n", p_cals->k_bsw_enable_trailer_zone_extension);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_enable,%d\n", p_cals->k_cvw_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_enable_via_cal,%d\n", p_cals->k_cvw_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_enable_cvw_curve_zone_adaptation,%d\n", p_cals->k_enable_cvw_curve_zone_adaptation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_f_most_crit_obj_must_be_closest_relevant_obj,%d\n", p_cals->k_cvw_f_most_crit_obj_must_be_closest_relevant_obj);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_enable,%d\n", p_cals->k_slc_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_enable_via_cal,%d\n", p_cals->k_slc_enable_via_cal);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_rel_vel_logic_in_ego_lane_check,%d\n", p_cals->k_lcda_f_enable_rel_vel_logic_in_ego_lane_check);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_fallback_default_status_slow,%d\n", p_cals->k_bsw_f_fallback_default_status_slow);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_obj_reflection_flag_check,%d\n", p_cals->k_lcda_f_enable_obj_reflection_flag_check);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_fallback_handler,%d\n", p_cals->k_lcda_f_enable_fallback_handler);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_dyn_cvw_ttc_threshold,%d\n", p_cals->k_lcda_f_enable_dyn_cvw_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_shrink_zone_method,%d\n", p_cals->k_bsw_shrink_zone_method);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_use_front_zone_as_n_line,%d\n", p_cals->k_bsw_f_use_front_zone_as_n_line);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_enable_object_rel_vel_dynzone,%d\n", p_cals->k_bsw_f_enable_object_rel_vel_dynzone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_f_use_zone_without_hysteresis_lane_change_intention,%d\n", p_cals->k_bsw_f_use_zone_without_hysteresis_lane_change_intention);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_alert_holding_cycles,%d\n", p_cals->k_elc_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_turn_signal_coast_cycles,%d\n", p_cals->k_lcda_turn_signal_coast_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_lc_intention_cycles_for_zone_change_threshold,%d\n", p_cals->k_lcda_lc_intention_cycles_for_zone_change_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_min_mature_cycles_lc_intention,%d\n", p_cals->k_cvw_min_mature_cycles_lc_intention);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_elc_min_mature_cycles,%d\n", p_cals->k_elc_min_mature_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_default_warntrigger_hmi,%d\n", p_cals->k_lcda_default_warntrigger_hmi);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_qualification_cycle_count,%d\n", p_cals->k_lm_min_qualification_cycle_count);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lm_min_output_hold_cycles,%d\n", p_cals->k_lm_min_output_hold_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_alert_holding_cycles,%d\n", p_cals->k_bsw_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_alert_holding_cycles,%d\n", p_cals->k_cvw_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_alert_holding_cycles,%d\n", p_cals->k_slc_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_zone_check_method,%d\n", p_cals->k_lcda_zone_check_method);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_fallback_fast_to_slow_qual_thres,%d\n", p_cals->k_bsw_fallback_fast_to_slow_qual_thres);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_min_mature_cycles,%d\n", p_cals->k_bsw_min_mature_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_min_track_age,%d\n", p_cals->k_lcda_min_track_age);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_alert_track_age,%d\n", p_cals->k_bsw_alert_track_age);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bsw_stop_alert_reaching_front_custom_limit_mode,%d\n", p_cals->k_bsw_stop_alert_reaching_front_custom_limit_mode);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_min_mature_cycles,%d\n", p_cals->k_cvw_min_mature_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_cvw_zone_calculation_mode,%d\n", p_cals->k_cvw_zone_calculation_mode);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_min_mature_cycles,%d\n", p_cals->k_slc_min_mature_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_slc_alert_qualifying_counter,%d\n", p_cals->k_slc_alert_qualifying_counter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_f_enable_camera_based_guardrail,%d\n", p_cals->k_lcda_f_enable_camera_based_guardrail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
