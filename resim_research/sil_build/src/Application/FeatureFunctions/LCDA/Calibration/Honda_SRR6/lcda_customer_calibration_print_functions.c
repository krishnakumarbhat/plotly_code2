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
   (void)fprintf(c_file_ptr,"p_cals.k_honda_max_hold_time_after_out_of_fov,%f\n", p_cals->k_honda_max_hold_time_after_out_of_fov);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_ego_speed_stop_holding,%f\n", p_cals->k_honda_ego_speed_stop_holding);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_min_relative_speed_for_alert_level_two,%f\n", p_cals->k_honda_min_relative_speed_for_alert_level_two);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_ego_lat_overlap_slide_through_zone,%f\n", p_cals->k_honda_ego_lat_overlap_slide_through_zone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_object_lat_overlap_slide_through_zone,%f\n", p_cals->k_honda_object_lat_overlap_slide_through_zone);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_beeper_zone_length,%f\n", p_cals->k_honda_beeper_zone_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_beeper_zone_width,%f\n", p_cals->k_honda_beeper_zone_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_beeper_zone_long_hys,%f\n", p_cals->k_honda_beeper_zone_long_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_beeper_zone_lat_hys,%f\n", p_cals->k_honda_beeper_zone_lat_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_alert_level_two_holding_time,%f\n", p_cals->k_honda_alert_level_two_holding_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_honda_narrow_beeper_max_speed_h,%f\n", p_cals->k_lcda_honda_narrow_beeper_max_speed_h);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_lcda_honda_narrow_beeper_max_speed_l,%f\n", p_cals->k_lcda_honda_narrow_beeper_max_speed_l);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_srr6_enable_alert_hold_due_slow_down,%d\n", p_cals->k_honda_srr6_enable_alert_hold_due_slow_down);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_srr6_enable_alert_hold_due_out_of_fov,%d\n", p_cals->k_honda_srr6_enable_alert_hold_due_out_of_fov);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two,%d\n", p_cals->k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
