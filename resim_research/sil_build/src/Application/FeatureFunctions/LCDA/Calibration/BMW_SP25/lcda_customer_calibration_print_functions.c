/**
* @file lcda_customer_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "lcda_customer_calibration_t.h" // IWYU pragma: keep
#include "lcda_customer_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Lcda_Customer_Cal_Print(FILE* c_file_ptr, const Lcda_Customer_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_guardrail_rel_diff_thresh,%f\n", p_cals->k_bmw_sp25_guardrail_rel_diff_thresh);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_exist_prob_lc_intention,%f\n", p_cals->k_bmw_sp25_exist_prob_lc_intention);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_cvw_limit_zone_range_hys,%f\n", p_cals->k_bmw_sp25_cvw_limit_zone_range_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment,%f\n", p_cals->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_trailer_mode_max_trailer_length,%f\n", p_cals->k_bmw_sp25_trailer_mode_max_trailer_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_distance,%f\n", p_cals->k_bmw_sp25_trailer_mode_max_bike_carrier_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_trailer_mode_max_bike_carrier_buffer,%f\n", p_cals->k_bmw_sp25_trailer_mode_max_bike_carrier_buffer);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_lane_change_detection_host_speed_min,%f\n", p_cals->k_bmw_sp25_lane_change_detection_host_speed_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_lane_change_dist_to_laneline_max,%f\n", p_cals->k_bmw_sp25_lane_change_dist_to_laneline_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min,%f\n", p_cals->k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_smooth_camera_signals,%d\n", p_cals->k_bmw_sp25_smooth_camera_signals);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_f_enable_lane_change_detection,%d\n", p_cals->k_bmw_sp25_f_enable_lane_change_detection);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_guardrail_age_stage_thresh,%d\n", p_cals->k_bmw_sp25_guardrail_age_stage_thresh);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_max_bad_guardrail_holding_counter,%d\n", p_cals->k_bmw_sp25_max_bad_guardrail_holding_counter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_lane_change_counter_min,%d\n", p_cals->k_bmw_sp25_lane_change_counter_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_lane_change_counter_max,%d\n", p_cals->k_bmw_sp25_lane_change_counter_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_camera_lane_plausibilisation_counter_max,%d\n", p_cals->k_bmw_sp25_camera_lane_plausibilisation_counter_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
