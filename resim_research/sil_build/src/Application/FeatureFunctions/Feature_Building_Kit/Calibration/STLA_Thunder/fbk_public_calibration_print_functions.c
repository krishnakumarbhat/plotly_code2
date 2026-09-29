/**
* @file fbk_public_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "fbk_public_calibration_t.h" // IWYU pragma: keep
#include "fbk_public_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Fbk_Public_Cal_Print(FILE* c_file_ptr, const Fbk_Public_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fbk_host_trail_max_recording_speed,%f\n", p_cals->k_fbk_host_trail_max_recording_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fbk_host_trail_dist_separation,%f\n", p_cals->k_fbk_host_trail_dist_separation);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fbk_host_trail_heading_separation,%f\n", p_cals->k_fbk_host_trail_heading_separation);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
