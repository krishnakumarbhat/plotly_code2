/**
* @file lcda_customer_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "lcda_customer_calibration_check.h" // IWYU pragma: keep
#include "lcda_customer_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Lcda_Customer_Cal_In_Boundary(const Lcda_Customer_Calibration_T *p_calibration)
{
   boolean_T f_lcda_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   
   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_ALERT_LEVEL_TWO_HOLDING_TIME, p_calibration->k_honda_alert_level_two_holding_time, LCDA_MAX_K_HONDA_ALERT_LEVEL_TWO_HOLDING_TIME);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_BEEPER_ZONE_LAT_HYS, p_calibration->k_honda_beeper_zone_lat_hys, LCDA_MAX_K_HONDA_BEEPER_ZONE_LAT_HYS);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_BEEPER_ZONE_LENGTH, p_calibration->k_honda_beeper_zone_length, LCDA_MAX_K_HONDA_BEEPER_ZONE_LENGTH);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_BEEPER_ZONE_LONG_HYS, p_calibration->k_honda_beeper_zone_long_hys, LCDA_MAX_K_HONDA_BEEPER_ZONE_LONG_HYS);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_BEEPER_ZONE_WIDTH, p_calibration->k_honda_beeper_zone_width, LCDA_MAX_K_HONDA_BEEPER_ZONE_WIDTH);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_EGO_LAT_OVERLAP_SLIDE_THROUGH_ZONE, p_calibration->k_honda_ego_lat_overlap_slide_through_zone, LCDA_MAX_K_HONDA_EGO_LAT_OVERLAP_SLIDE_THROUGH_ZONE);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_EGO_SPEED_STOP_HOLDING, p_calibration->k_honda_ego_speed_stop_holding, LCDA_MAX_K_HONDA_EGO_SPEED_STOP_HOLDING);
Ct_Is_Bool_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_IS_SLIDE_THROUGH_ZONE_CONSIDERED_FOR_CVW_ALERT_LEVEL_TWO, p_calibration->k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two, LCDA_MAX_K_HONDA_IS_SLIDE_THROUGH_ZONE_CONSIDERED_FOR_CVW_ALERT_LEVEL_TWO);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_MAX_HOLD_TIME_AFTER_OUT_OF_FOV, p_calibration->k_honda_max_hold_time_after_out_of_fov, LCDA_MAX_K_HONDA_MAX_HOLD_TIME_AFTER_OUT_OF_FOV);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_MIN_RELATIVE_SPEED_FOR_ALERT_LEVEL_TWO, p_calibration->k_honda_min_relative_speed_for_alert_level_two, LCDA_MAX_K_HONDA_MIN_RELATIVE_SPEED_FOR_ALERT_LEVEL_TWO);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_OBJECT_LAT_OVERLAP_SLIDE_THROUGH_ZONE, p_calibration->k_honda_object_lat_overlap_slide_through_zone, LCDA_MAX_K_HONDA_OBJECT_LAT_OVERLAP_SLIDE_THROUGH_ZONE);
Ct_Is_Bool_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_OUT_OF_FOV, p_calibration->k_honda_srr6_enable_alert_hold_due_out_of_fov, LCDA_MAX_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_OUT_OF_FOV);
Ct_Is_Bool_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_SLOW_DOWN, p_calibration->k_honda_srr6_enable_alert_hold_due_slow_down, LCDA_MAX_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_SLOW_DOWN);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_H, p_calibration->k_lcda_honda_narrow_beeper_max_speed_h, LCDA_MAX_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_H);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_L, p_calibration->k_lcda_honda_narrow_beeper_max_speed_l, LCDA_MAX_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_L);


   return f_lcda_calibration_in_boundaries;
}

