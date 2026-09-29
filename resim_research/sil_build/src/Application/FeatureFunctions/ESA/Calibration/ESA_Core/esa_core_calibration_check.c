/**
* @file esa_core_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in esa_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "esa_core_calibration_check.h" // IWYU pragma: keep
#include "esa_core_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Esa_Core_Cal_In_Boundary(const Esa_Core_Calibration_T *p_calibration)
{
   boolean_T f_esa_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   {
    uint8_t x;
    for (x = 0u; x < ESA_K_ESA_ZONE_X_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_ZONE_X, p_calibration->k_esa_zone_x[x], ESA_MAX_K_ESA_ZONE_X);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_ZONE_X_HYS, p_calibration->k_esa_zone_x_hys[x], ESA_MAX_K_ESA_ZONE_X_HYS);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_ZONE_Y, p_calibration->k_esa_zone_y[x], ESA_MAX_K_ESA_ZONE_Y);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_ZONE_Y_HYS, p_calibration->k_esa_zone_y_hys[x], ESA_MAX_K_ESA_ZONE_Y_HYS);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Uint8_In_Bondaries(&f_esa_calibration_in_boundaries, 0, p_calibration->k_esa_alert_holding_cycles, ESA_MAX_K_ESA_ALERT_HOLDING_CYCLES);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_CRITICAL_LONGITUDINAL_TTC, p_calibration->k_esa_critical_longitudinal_ttc, ESA_MAX_K_ESA_CRITICAL_LONGITUDINAL_TTC);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_CRITICAL_LONGITUDINAL_TTC_HYS, p_calibration->k_esa_critical_longitudinal_ttc_hys, ESA_MAX_K_ESA_CRITICAL_LONGITUDINAL_TTC_HYS);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ALLOW_MIN_CURVE_RADIUS, p_calibration->k_esa_f_allow_min_curve_radius, ESA_MAX_K_ESA_F_ALLOW_MIN_CURVE_RADIUS);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ALLOW_OBJ_CRITICAL_TTC_AND_DECELERATION, p_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration, ESA_MAX_K_ESA_F_ALLOW_OBJ_CRITICAL_TTC_AND_DECELERATION);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ALLOW_OBJ_SELECTION_DECELERATION, p_calibration->k_esa_f_allow_obj_selection_deceleration, ESA_MAX_K_ESA_F_ALLOW_OBJ_SELECTION_DECELERATION);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ALLOW_OBJ_SELECTION_LONG_DISTANCE, p_calibration->k_esa_f_allow_obj_selection_long_distance, ESA_MAX_K_ESA_F_ALLOW_OBJ_SELECTION_LONG_DISTANCE);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ALLOW_OBJ_SELECTION_TTC, p_calibration->k_esa_f_allow_obj_selection_ttc, ESA_MAX_K_ESA_F_ALLOW_OBJ_SELECTION_TTC);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ENABLE, p_calibration->k_esa_f_enable, ESA_MAX_K_ESA_F_ENABLE);
Ct_Is_Bool_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_F_ENABLE_VIA_CAL, p_calibration->k_esa_f_enable_via_cal, ESA_MAX_K_ESA_F_ENABLE_VIA_CAL);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MAX, p_calibration->k_esa_host_activation_speed_max, ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MAX);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MAX_HYS, p_calibration->k_esa_host_activation_speed_max_hys, ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MAX_HYS);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MIN, p_calibration->k_esa_host_activation_speed_min, ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MIN);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MIN_HYS, p_calibration->k_esa_host_activation_speed_min_hys, ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MIN_HYS);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MAX_CURVI_HEADING_ABS, p_calibration->k_esa_max_curvi_heading_abs, ESA_MAX_K_ESA_MAX_CURVI_HEADING_ABS);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MAX_LANE_WIDTH, p_calibration->k_esa_max_lane_width, ESA_MAX_K_ESA_MAX_LANE_WIDTH);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MAX_RANGE, p_calibration->k_esa_max_range, ESA_MAX_K_ESA_MAX_RANGE);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MIN_CURVE_RADIUS, p_calibration->k_esa_min_curve_radius, ESA_MAX_K_ESA_MIN_CURVE_RADIUS);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MIN_CURVE_RADIUS_HYS, p_calibration->k_esa_min_curve_radius_hys, ESA_MAX_K_ESA_MIN_CURVE_RADIUS_HYS);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MIN_EXIST_PROB, p_calibration->k_esa_min_exist_prob, ESA_MAX_K_ESA_MIN_EXIST_PROB);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MIN_LANE_WIDTH, p_calibration->k_esa_min_lane_width, ESA_MAX_K_ESA_MIN_LANE_WIDTH);
Ct_Is_Uint8_In_Bondaries(&f_esa_calibration_in_boundaries, 0, p_calibration->k_esa_min_mature_cycles, ESA_MAX_K_ESA_MIN_MATURE_CYCLES);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_MIN_OBJ_CURVI_LONG_VEL_ABS, p_calibration->k_esa_min_obj_curvi_long_vel_abs, ESA_MAX_K_ESA_MIN_OBJ_CURVI_LONG_VEL_ABS);
Ct_Is_Uint8_In_Bondaries(&f_esa_calibration_in_boundaries, 0, p_calibration->k_esa_min_track_age, ESA_MAX_K_ESA_MIN_TRACK_AGE);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD, p_calibration->k_esa_obj_safe_deceleration_threshold, ESA_MAX_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_esa_calibration_in_boundaries, ESA_MIN_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD_HYS, p_calibration->k_esa_obj_safe_deceleration_threshold_hys, ESA_MAX_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD_HYS);


   return f_esa_calibration_in_boundaries;
}

