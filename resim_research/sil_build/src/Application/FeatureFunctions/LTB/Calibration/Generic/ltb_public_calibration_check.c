/**
* @file ltb_public_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in ltb_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "ltb_public_calibration_check.h" // IWYU pragma: keep
#include "ltb_public_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Ltb_Public_Cal_In_Boundary(const Ltb_Public_Calibration_T *p_calibration)
{
   boolean_T f_ltb_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   
   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Bool_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_F_LTB_ENABLE_BRAKE_GRADIENT_LOGIC, p_calibration->k_f_ltb_enable_brake_gradient_logic, LTB_MAX_K_F_LTB_ENABLE_BRAKE_GRADIENT_LOGIC);
Ct_Is_Uint8_In_Bondaries(&f_ltb_calibration_in_boundaries, 0, p_calibration->k_ltb_alert_holding_cycles, LTB_MAX_K_LTB_ALERT_HOLDING_CYCLES);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ALERT_LVL_1_TTC_THRESHOLD, p_calibration->k_ltb_alert_lvl_1_ttc_threshold, LTB_MAX_K_LTB_ALERT_LVL_1_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ALERT_LVL_2_TTB_THRESHOLD, p_calibration->k_ltb_alert_lvl_2_ttb_threshold, LTB_MAX_K_LTB_ALERT_LVL_2_TTB_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ALERT_LVL_2_TTC_THRESHOLD, p_calibration->k_ltb_alert_lvl_2_ttc_threshold, LTB_MAX_K_LTB_ALERT_LVL_2_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ALERT_LVL_3_DECEL_THRESHOLD, p_calibration->k_ltb_alert_lvl_3_decel_threshold, LTB_MAX_K_LTB_ALERT_LVL_3_DECEL_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ALERT_LVL_3_TTC_THRESHOLD, p_calibration->k_ltb_alert_lvl_3_ttc_threshold, LTB_MAX_K_LTB_ALERT_LVL_3_TTC_THRESHOLD);
Ct_Is_Uint8_In_Bondaries(&f_ltb_calibration_in_boundaries, 0, p_calibration->k_ltb_alert_qualifying_cycles, LTB_MAX_K_LTB_ALERT_QUALIFYING_CYCLES);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_BMW_SP25_V_EGO_MAX, p_calibration->k_ltb_bmw_sp25_v_ego_max, LTB_MAX_K_LTB_BMW_SP25_V_EGO_MAX);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_BMW_SP25_V_EGO_MAX_HYS, p_calibration->k_ltb_bmw_sp25_v_ego_max_hys, LTB_MAX_K_LTB_BMW_SP25_V_EGO_MAX_HYS);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_BRAKE_DEAD_TIME, p_calibration->k_ltb_brake_dead_time, LTB_MAX_K_LTB_BRAKE_DEAD_TIME);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_BRAKE_DECELERATION_MAX, p_calibration->k_ltb_brake_deceleration_max, LTB_MAX_K_LTB_BRAKE_DECELERATION_MAX);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_BRAKE_GRADIENT, p_calibration->k_ltb_brake_gradient, LTB_MAX_K_LTB_BRAKE_GRADIENT);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_CRITICAL_APPROACH_ANGLE_DIFF_MIN, p_calibration->k_ltb_critical_approach_angle_diff_min, LTB_MAX_K_LTB_CRITICAL_APPROACH_ANGLE_DIFF_MIN);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_CRITICAL_APPROACH_MIN_SAFE_DISTANCE, p_calibration->k_ltb_critical_approach_min_safe_distance, LTB_MAX_K_LTB_CRITICAL_APPROACH_MIN_SAFE_DISTANCE);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_ACCELERATION_WEIGHT, p_calibration->k_ltb_ego_acceleration_weight, LTB_MAX_K_LTB_EGO_ACCELERATION_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_CIRCLE_HOST_LENGTH_FACTOR, p_calibration->k_ltb_ego_circle_host_length_factor, LTB_MAX_K_LTB_EGO_CIRCLE_HOST_LENGTH_FACTOR);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_CIRCLE_OFFSET, p_calibration->k_ltb_ego_circle_offset, LTB_MAX_K_LTB_EGO_CIRCLE_OFFSET);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_DECELERATION_WEIGHT, p_calibration->k_ltb_ego_deceleration_weight, LTB_MAX_K_LTB_EGO_DECELERATION_WEIGHT);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_MAX_PRED_YAW_ANGLE, p_calibration->k_ltb_ego_max_pred_yaw_angle, LTB_MAX_K_LTB_EGO_MAX_PRED_YAW_ANGLE);
Ct_Is_Uint8_In_Bondaries(&f_ltb_calibration_in_boundaries, 0, p_calibration->k_ltb_ego_pred_const_velocity_pred_steps_min, LTB_MAX_K_LTB_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_SHAPE_GAIN_FIXED, p_calibration->k_ltb_ego_shape_gain_fixed, LTB_MAX_K_LTB_EGO_SHAPE_GAIN_FIXED);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_SHAPE_GAIN_PER_PRED_STEP, p_calibration->k_ltb_ego_shape_gain_per_pred_step, LTB_MAX_K_LTB_EGO_SHAPE_GAIN_PER_PRED_STEP);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN, p_calibration->k_ltb_ego_yawangle_integration_yawrate_min, LTB_MAX_K_LTB_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN);
Ct_Is_Bool_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS, p_calibration->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels, LTB_MAX_K_LTB_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS);
Ct_Is_Bool_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP, p_calibration->k_ltb_f_skip_holding_for_single_alert_level_drop, LTB_MAX_K_LTB_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_OBJ_PRED_SPEED_MIN, p_calibration->k_ltb_obj_pred_speed_min, LTB_MAX_K_LTB_OBJ_PRED_SPEED_MIN);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_OBJ_SHAPE_GAIN_FIXED, p_calibration->k_ltb_obj_shape_gain_fixed, LTB_MAX_K_LTB_OBJ_SHAPE_GAIN_FIXED);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_OBJ_SHAPE_GAIN_PER_PRED_STEP, p_calibration->k_ltb_obj_shape_gain_per_pred_step, LTB_MAX_K_LTB_OBJ_SHAPE_GAIN_PER_PRED_STEP);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_OBJECT_LONG_VEL_MIN, p_calibration->k_ltb_object_long_vel_min, LTB_MAX_K_LTB_OBJECT_LONG_VEL_MIN);
Ct_Is_Uint8_In_Bondaries(&f_ltb_calibration_in_boundaries, 0, p_calibration->k_ltb_prediction_steps_max, LTB_MAX_K_LTB_PREDICTION_STEPS_MAX);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ZONE_LENGTH, p_calibration->k_ltb_zone_length, LTB_MAX_K_LTB_ZONE_LENGTH);
Ct_Is_Float_In_Bondaries(&f_ltb_calibration_in_boundaries, LTB_MIN_K_LTB_ZONE_WIDTH, p_calibration->k_ltb_zone_width, LTB_MAX_K_LTB_ZONE_WIDTH);


   return f_ltb_calibration_in_boundaries;
}

