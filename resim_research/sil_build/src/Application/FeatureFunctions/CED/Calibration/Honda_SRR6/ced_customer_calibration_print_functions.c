/**
* @file ced_customer_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ced_customer_calibration_t.h" // IWYU pragma: keep
#include "ced_customer_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Ced_Customer_Cal_Print(FILE* c_file_ptr, const Ced_Customer_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_srr6_custom_ttc_alert_threshold._0_,%f\n", p_cals->k_ced_honda_srr6_custom_ttc_alert_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_srr6_custom_ttc_alert_threshold._1_,%f\n", p_cals->k_ced_honda_srr6_custom_ttc_alert_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_srr6_custom_ttc_alert_hysteresis._0_,%f\n", p_cals->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_srr6_custom_ttc_alert_hysteresis._1_,%f\n", p_cals->k_ced_honda_srr6_custom_ttc_alert_hysteresis[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_srr6_long_dist_threshold._0_,%f\n", p_cals->k_ced_honda_srr6_long_dist_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_srr6_long_dist_threshold._1_,%f\n", p_cals->k_ced_honda_srr6_long_dist_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_min_alert_duration,%f\n", p_cals->k_honda_min_alert_duration);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_elatch_zones_width_table._0_,%f\n", p_cals->k_honda_elatch_zones_width_table[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_elatch_zones_width_table._1_,%f\n", p_cals->k_honda_elatch_zones_width_table[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_honda_min_eratch_alert_duration,%f\n", p_cals->k_honda_min_eratch_alert_duration);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_honda_object_acceleration_weight,%f\n", p_cals->k_ced_honda_object_acceleration_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ced_f_honda_use_alert_ttc_threshold,%d\n", p_cals->k_ced_f_honda_use_alert_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_1,%d\n", p_cals->k_unused_padding_byte_1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_2,%d\n", p_cals->k_unused_padding_byte_2);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
