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
   Ct_Is_Uint8_In_Bondaries(&f_lcda_calibration_in_boundaries, 0, p_calibration->k_bmw_sp25_camera_lane_plausibilisation_counter_max, LCDA_MAX_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_COUNTER_MAX);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_EXIST_PROB_MIN, p_calibration->k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min, LCDA_MAX_K_BMW_SP25_CAMERA_LANE_PLAUSIBILISATION_EXIST_PROB_MIN);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_CVW_LIMIT_ZONE_RANGE_HYS, p_calibration->k_bmw_sp25_cvw_limit_zone_range_hys, LCDA_MAX_K_BMW_SP25_CVW_LIMIT_ZONE_RANGE_HYS);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_EXIST_PROB_LC_INTENTION, p_calibration->k_bmw_sp25_exist_prob_lc_intention, LCDA_MAX_K_BMW_SP25_EXIST_PROB_LC_INTENTION);
Ct_Is_Bool_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_F_ENABLE_LANE_CHANGE_DETECTION, p_calibration->k_bmw_sp25_f_enable_lane_change_detection, LCDA_MAX_K_BMW_SP25_F_ENABLE_LANE_CHANGE_DETECTION);
Ct_Is_Uint8_In_Bondaries(&f_lcda_calibration_in_boundaries, 0, p_calibration->k_bmw_sp25_guardrail_age_stage_thresh, LCDA_MAX_K_BMW_SP25_GUARDRAIL_AGE_STAGE_THRESH);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_GUARDRAIL_REL_DIFF_THRESH, p_calibration->k_bmw_sp25_guardrail_rel_diff_thresh, LCDA_MAX_K_BMW_SP25_GUARDRAIL_REL_DIFF_THRESH);
Ct_Is_Uint8_In_Bondaries(&f_lcda_calibration_in_boundaries, 0, p_calibration->k_bmw_sp25_lane_change_counter_max, LCDA_MAX_K_BMW_SP25_LANE_CHANGE_COUNTER_MAX);
Ct_Is_Uint8_In_Bondaries(&f_lcda_calibration_in_boundaries, 0, p_calibration->k_bmw_sp25_lane_change_counter_min, LCDA_MAX_K_BMW_SP25_LANE_CHANGE_COUNTER_MIN);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_LANE_CHANGE_DETECTION_HOST_SPEED_MIN, p_calibration->k_bmw_sp25_lane_change_detection_host_speed_min, LCDA_MAX_K_BMW_SP25_LANE_CHANGE_DETECTION_HOST_SPEED_MIN);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_LANE_CHANGE_DIST_TO_LANELINE_MAX, p_calibration->k_bmw_sp25_lane_change_dist_to_laneline_max, LCDA_MAX_K_BMW_SP25_LANE_CHANGE_DIST_TO_LANELINE_MAX);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_LOWEST_PROBABILTY_PERCENTAGE_CAL_FOR_ADJUSTMENT, p_calibration->k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment, LCDA_MAX_K_BMW_SP25_LOWEST_PROBABILTY_PERCENTAGE_CAL_FOR_ADJUSTMENT);
Ct_Is_Bool_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_SMOOTH_CAMERA_SIGNALS, p_calibration->k_bmw_sp25_smooth_camera_signals, LCDA_MAX_K_BMW_SP25_SMOOTH_CAMERA_SIGNALS);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_BUFFER, p_calibration->k_bmw_sp25_trailer_mode_max_bike_carrier_buffer, LCDA_MAX_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_BUFFER);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_DISTANCE, p_calibration->k_bmw_sp25_trailer_mode_max_bike_carrier_distance, LCDA_MAX_K_BMW_SP25_TRAILER_MODE_MAX_BIKE_CARRIER_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_lcda_calibration_in_boundaries, LCDA_MIN_K_BMW_SP25_TRAILER_MODE_MAX_TRAILER_LENGTH, p_calibration->k_bmw_sp25_trailer_mode_max_trailer_length, LCDA_MAX_K_BMW_SP25_TRAILER_MODE_MAX_TRAILER_LENGTH);


   return f_lcda_calibration_in_boundaries;
}

