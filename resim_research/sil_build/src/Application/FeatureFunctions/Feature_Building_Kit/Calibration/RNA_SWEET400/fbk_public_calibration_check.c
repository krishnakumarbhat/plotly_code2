/**
* @file fbk_public_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "fbk_public_calibration_check.h" // IWYU pragma: keep
#include "fbk_public_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Fbk_Public_Cal_In_Boundary(const Fbk_Public_Calibration_T *p_calibration)
{
   boolean_T f_fbk_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   
   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_fbk_calibration_in_boundaries, FBK_MIN_K_FBK_HOST_TRAIL_DIST_SEPARATION, p_calibration->k_fbk_host_trail_dist_separation, FBK_MAX_K_FBK_HOST_TRAIL_DIST_SEPARATION);
Ct_Is_Float_In_Bondaries(&f_fbk_calibration_in_boundaries, FBK_MIN_K_FBK_HOST_TRAIL_HEADING_SEPARATION, p_calibration->k_fbk_host_trail_heading_separation, FBK_MAX_K_FBK_HOST_TRAIL_HEADING_SEPARATION);
Ct_Is_Float_In_Bondaries(&f_fbk_calibration_in_boundaries, FBK_MIN_K_FBK_HOST_TRAIL_MAX_RECORDING_SPEED, p_calibration->k_fbk_host_trail_max_recording_speed, FBK_MAX_K_FBK_HOST_TRAIL_MAX_RECORDING_SPEED);


   return f_fbk_calibration_in_boundaries;
}

