/**
* @file ltb_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in ltb_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "ltb_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "ltb_core_calibration_check.h"
#include "ltb_core_calibration_t.h"
#include "ltb_customer_calibration_t.h"
#include "ltb_public_calibration_check.h"
#include "ltb_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ltb_Update_Core_Cal_By_Public(Ltb_Core_Calibration_T* cal_dst, const Ltb_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ltb_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_ltb_zone_length = cal_src->k_ltb_zone_length;
        cal_dst->k_ltb_zone_width = cal_src->k_ltb_zone_width;
        cal_dst->k_ltb_ego_acceleration_weight = cal_src->k_ltb_ego_acceleration_weight;
        cal_dst->k_ltb_ego_shape_gain_fixed = cal_src->k_ltb_ego_shape_gain_fixed;
        cal_dst->k_ltb_ego_circle_offset = cal_src->k_ltb_ego_circle_offset;
        cal_dst->k_ltb_ego_circle_host_length_factor = cal_src->k_ltb_ego_circle_host_length_factor;
        cal_dst->k_ltb_ego_deceleration_weight = cal_src->k_ltb_ego_deceleration_weight;
        cal_dst->k_ltb_ego_max_pred_yaw_angle = cal_src->k_ltb_ego_max_pred_yaw_angle;
        cal_dst->k_ltb_ego_yawangle_integration_yawrate_min = cal_src->k_ltb_ego_yawangle_integration_yawrate_min;
        cal_dst->k_ltb_ego_shape_gain_per_pred_step = cal_src->k_ltb_ego_shape_gain_per_pred_step;
        cal_dst->k_ltb_obj_pred_speed_min = cal_src->k_ltb_obj_pred_speed_min;
        cal_dst->k_ltb_obj_shape_gain_fixed = cal_src->k_ltb_obj_shape_gain_fixed;
        cal_dst->k_ltb_obj_shape_gain_per_pred_step = cal_src->k_ltb_obj_shape_gain_per_pred_step;
        cal_dst->k_ltb_critical_approach_min_safe_distance = cal_src->k_ltb_critical_approach_min_safe_distance;
        cal_dst->k_ltb_critical_approach_angle_diff_min = cal_src->k_ltb_critical_approach_angle_diff_min;
        cal_dst->k_ltb_alert_lvl_1_ttc_threshold = cal_src->k_ltb_alert_lvl_1_ttc_threshold;
        cal_dst->k_ltb_alert_lvl_2_ttc_threshold = cal_src->k_ltb_alert_lvl_2_ttc_threshold;
        cal_dst->k_ltb_alert_lvl_2_ttb_threshold = cal_src->k_ltb_alert_lvl_2_ttb_threshold;
        cal_dst->k_ltb_alert_lvl_3_ttc_threshold = cal_src->k_ltb_alert_lvl_3_ttc_threshold;
        cal_dst->k_ltb_alert_lvl_3_decel_threshold = cal_src->k_ltb_alert_lvl_3_decel_threshold;
        cal_dst->k_ltb_brake_deceleration_max = cal_src->k_ltb_brake_deceleration_max;
        cal_dst->k_ltb_brake_dead_time = cal_src->k_ltb_brake_dead_time;
        cal_dst->k_ltb_brake_gradient = cal_src->k_ltb_brake_gradient;
        cal_dst->k_ltb_object_long_vel_min = cal_src->k_ltb_object_long_vel_min;
        cal_dst->k_ltb_bmw_sp25_v_ego_max = cal_src->k_ltb_bmw_sp25_v_ego_max;
        cal_dst->k_ltb_bmw_sp25_v_ego_max_hys = cal_src->k_ltb_bmw_sp25_v_ego_max_hys;
        cal_dst->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels = cal_src->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels;
        cal_dst->k_ltb_f_skip_holding_for_single_alert_level_drop = cal_src->k_ltb_f_skip_holding_for_single_alert_level_drop;
        cal_dst->k_f_ltb_enable_brake_gradient_logic = cal_src->k_f_ltb_enable_brake_gradient_logic;
        cal_dst->k_ltb_ego_pred_const_velocity_pred_steps_min = cal_src->k_ltb_ego_pred_const_velocity_pred_steps_min;
        cal_dst->k_ltb_prediction_steps_max = cal_src->k_ltb_prediction_steps_max;
        cal_dst->k_ltb_alert_qualifying_cycles = cal_src->k_ltb_alert_qualifying_cycles;
        cal_dst->k_ltb_alert_holding_cycles = cal_src->k_ltb_alert_holding_cycles;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ltb_Update_Core_Cal_By_Core(Ltb_Core_Calibration_T* cal_dst, const Ltb_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ltb_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_ltb_zone_length = cal_src->k_ltb_zone_length;
        cal_dst->k_ltb_zone_width = cal_src->k_ltb_zone_width;
        cal_dst->k_ltb_ego_acceleration_weight = cal_src->k_ltb_ego_acceleration_weight;
        cal_dst->k_ltb_ego_shape_gain_fixed = cal_src->k_ltb_ego_shape_gain_fixed;
        cal_dst->k_ltb_ego_circle_offset = cal_src->k_ltb_ego_circle_offset;
        cal_dst->k_ltb_ego_circle_host_length_factor = cal_src->k_ltb_ego_circle_host_length_factor;
        cal_dst->k_ltb_ego_deceleration_weight = cal_src->k_ltb_ego_deceleration_weight;
        cal_dst->k_ltb_ego_max_pred_yaw_angle = cal_src->k_ltb_ego_max_pred_yaw_angle;
        cal_dst->k_ltb_ego_yawangle_integration_yawrate_min = cal_src->k_ltb_ego_yawangle_integration_yawrate_min;
        cal_dst->k_ltb_ego_shape_gain_per_pred_step = cal_src->k_ltb_ego_shape_gain_per_pred_step;
        cal_dst->k_ltb_obj_pred_speed_min = cal_src->k_ltb_obj_pred_speed_min;
        cal_dst->k_ltb_obj_shape_gain_fixed = cal_src->k_ltb_obj_shape_gain_fixed;
        cal_dst->k_ltb_obj_shape_gain_per_pred_step = cal_src->k_ltb_obj_shape_gain_per_pred_step;
        cal_dst->k_ltb_critical_approach_min_safe_distance = cal_src->k_ltb_critical_approach_min_safe_distance;
        cal_dst->k_ltb_critical_approach_angle_diff_min = cal_src->k_ltb_critical_approach_angle_diff_min;
        cal_dst->k_ltb_alert_lvl_1_ttc_threshold = cal_src->k_ltb_alert_lvl_1_ttc_threshold;
        cal_dst->k_ltb_alert_lvl_2_ttc_threshold = cal_src->k_ltb_alert_lvl_2_ttc_threshold;
        cal_dst->k_ltb_alert_lvl_2_ttb_threshold = cal_src->k_ltb_alert_lvl_2_ttb_threshold;
        cal_dst->k_ltb_alert_lvl_3_ttc_threshold = cal_src->k_ltb_alert_lvl_3_ttc_threshold;
        cal_dst->k_ltb_alert_lvl_3_decel_threshold = cal_src->k_ltb_alert_lvl_3_decel_threshold;
        cal_dst->k_ltb_brake_deceleration_max = cal_src->k_ltb_brake_deceleration_max;
        cal_dst->k_ltb_brake_dead_time = cal_src->k_ltb_brake_dead_time;
        cal_dst->k_ltb_brake_gradient = cal_src->k_ltb_brake_gradient;
        cal_dst->k_ltb_object_long_vel_min = cal_src->k_ltb_object_long_vel_min;
        cal_dst->k_ltb_bmw_sp25_v_ego_max = cal_src->k_ltb_bmw_sp25_v_ego_max;
        cal_dst->k_ltb_bmw_sp25_v_ego_max_hys = cal_src->k_ltb_bmw_sp25_v_ego_max_hys;
        cal_dst->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels = cal_src->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels;
        cal_dst->k_ltb_f_skip_holding_for_single_alert_level_drop = cal_src->k_ltb_f_skip_holding_for_single_alert_level_drop;
        cal_dst->k_f_ltb_enable_brake_gradient_logic = cal_src->k_f_ltb_enable_brake_gradient_logic;
        cal_dst->k_ltb_ego_pred_const_velocity_pred_steps_min = cal_src->k_ltb_ego_pred_const_velocity_pred_steps_min;
        cal_dst->k_ltb_prediction_steps_max = cal_src->k_ltb_prediction_steps_max;
        cal_dst->k_ltb_alert_qualifying_cycles = cal_src->k_ltb_alert_qualifying_cycles;
        cal_dst->k_ltb_alert_holding_cycles = cal_src->k_ltb_alert_holding_cycles;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ltb_Update_Customer_Cal_By_Public(Ltb_Customer_Calibration_T* cal_dst, const Ltb_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ltb_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


