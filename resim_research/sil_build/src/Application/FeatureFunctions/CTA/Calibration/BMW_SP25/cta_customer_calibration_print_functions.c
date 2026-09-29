/**
* @file cta_customer_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "cta_customer_calibration_t.h" // IWYU pragma: keep
#include "cta_customer_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Cta_Customer_Cal_Print(FILE* c_file_ptr, const Cta_Customer_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_ego_abs_speed_max_hys,%f\n", p_cals->k_bmw_sp25_ego_abs_speed_max_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_banner_criteria_check_time,%f\n", p_cals->k_bmw_sp25_banner_criteria_check_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_banner_time,%f\n", p_cals->k_bmw_sp25_banner_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_bmw_sp25_banner_criteria_check,%d\n", p_cals->k_bmw_sp25_banner_criteria_check);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_1,%d\n", p_cals->k_unused_padding_byte_1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_2,%d\n", p_cals->k_unused_padding_byte_2);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
