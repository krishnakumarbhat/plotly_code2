
/**
* @file ltb_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in ltb_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ltb_public_calibration_t.h"
#include "ltb_public_calibration.h"
#include <string.h>

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ltb_Public_Cal_Update_Defaults(Ltb_Public_Calibration_T* cal_dst)
{
    Ltb_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)3,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_ltb_zone_length*/ (float32_T) 25.0f /**< 25.0 m */,
   /**<k_ltb_zone_width*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_ltb_ego_acceleration_weight*/ (float32_T) 1.0f,
   /**<k_ltb_ego_shape_gain_fixed*/ (float32_T) 1.0f,
   /**<k_ltb_ego_circle_offset*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ltb_ego_circle_host_length_factor*/ (float32_T) 1.0f,
   /**<k_ltb_ego_deceleration_weight*/ (float32_T) 1.0f,
   /**<k_ltb_ego_max_pred_yaw_angle*/ (float32_T) 2.0f /**< 2.0 rad | 114.59 deg */,
   /**<k_ltb_ego_yawangle_integration_yawrate_min*/ (float32_T) 0.01f /**< 0.01 rad/s | 0.57 deg/s */,
   /**<k_ltb_ego_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ltb_obj_pred_speed_min*/ (float32_T) 0.7f /**< 0.7 m/s | 2.52 km/h */,
   /**<k_ltb_obj_shape_gain_fixed*/ (float32_T) 1.0f,
   /**<k_ltb_obj_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ltb_critical_approach_min_safe_distance*/ (float32_T) 0.7f /**< 0.7 m */,
   /**<k_ltb_critical_approach_angle_diff_min*/ (float32_T) 0.03f /**< 0.03 rad | 1.72 deg */,
   /**<k_ltb_alert_lvl_1_ttc_threshold*/ (float32_T) 2.0f /**< 2.0 s */,
   /**<k_ltb_alert_lvl_2_ttc_threshold*/ (float32_T) 1.2f /**< 1.2 s */,
   /**<k_ltb_alert_lvl_2_ttb_threshold*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ltb_alert_lvl_3_ttc_threshold*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ltb_alert_lvl_3_decel_threshold*/ (float32_T) 3.0f /**< 3.0 m/s^2 */,
   /**<k_ltb_brake_deceleration_max*/ (float32_T) 10.0f /**< 10.0 m/s^2 */,
   /**<k_ltb_brake_dead_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_ltb_brake_gradient*/ (float32_T) -40.0f /**< -40.0 m/s^3 */,
   /**<k_ltb_object_long_vel_min*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_ltb_bmw_sp25_v_ego_max*/ (float32_T) 40.0f /**< 40.0 m/s | 144.0 km/h */,
   /**<k_ltb_bmw_sp25_v_ego_max_hys*/ (float32_T) 0.277f /**< 0.277 m/s | 1.0 km/h */,
   /**<k_ltb_f_only_allow_consecutive_ttc_based_alert_levels*/ (boolean_T) 0u,
   /**<k_ltb_f_skip_holding_for_single_alert_level_drop*/ (boolean_T) 0u,
   /**<k_f_ltb_enable_brake_gradient_logic*/ (boolean_T) 0,
   /**<k_ltb_ego_pred_const_velocity_pred_steps_min*/ (uint8_t) 40u,
   /**<k_ltb_prediction_steps_max*/ (uint8_t) 20u,
   /**<k_ltb_alert_qualifying_cycles*/ (uint8_t) 2u,
   /**<k_ltb_alert_holding_cycles*/ (uint8_t) 3u
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_ltb_alert_holding_cycles*/ (uint8_t) 3u,
   /**<k_ltb_alert_qualifying_cycles*/ (uint8_t) 2u,
   /**<k_ltb_prediction_steps_max*/ (uint8_t) 20u,
   /**<k_ltb_ego_pred_const_velocity_pred_steps_min*/ (uint8_t) 40u,
   /**<k_f_ltb_enable_brake_gradient_logic*/ (boolean_T) 0,
   /**<k_ltb_f_skip_holding_for_single_alert_level_drop*/ (boolean_T) 0u,
   /**<k_ltb_f_only_allow_consecutive_ttc_based_alert_levels*/ (boolean_T) 0u,
   /**<k_ltb_bmw_sp25_v_ego_max_hys*/ (float32_T) 0.277f /**< 0.277 m/s | 1.0 km/h */,
   /**<k_ltb_bmw_sp25_v_ego_max*/ (float32_T) 40.0f /**< 40.0 m/s | 144.0 km/h */,
   /**<k_ltb_object_long_vel_min*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_ltb_brake_gradient*/ (float32_T) -40.0f /**< -40.0 m/s^3 */,
   /**<k_ltb_brake_dead_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_ltb_brake_deceleration_max*/ (float32_T) 10.0f /**< 10.0 m/s^2 */,
   /**<k_ltb_alert_lvl_3_decel_threshold*/ (float32_T) 3.0f /**< 3.0 m/s^2 */,
   /**<k_ltb_alert_lvl_3_ttc_threshold*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ltb_alert_lvl_2_ttb_threshold*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ltb_alert_lvl_2_ttc_threshold*/ (float32_T) 1.2f /**< 1.2 s */,
   /**<k_ltb_alert_lvl_1_ttc_threshold*/ (float32_T) 2.0f /**< 2.0 s */,
   /**<k_ltb_critical_approach_angle_diff_min*/ (float32_T) 0.03f /**< 0.03 rad | 1.72 deg */,
   /**<k_ltb_critical_approach_min_safe_distance*/ (float32_T) 0.7f /**< 0.7 m */,
   /**<k_ltb_obj_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ltb_obj_shape_gain_fixed*/ (float32_T) 1.0f,
   /**<k_ltb_obj_pred_speed_min*/ (float32_T) 0.7f /**< 0.7 m/s | 2.52 km/h */,
   /**<k_ltb_ego_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ltb_ego_yawangle_integration_yawrate_min*/ (float32_T) 0.01f /**< 0.01 rad/s | 0.57 deg/s */,
   /**<k_ltb_ego_max_pred_yaw_angle*/ (float32_T) 2.0f /**< 2.0 rad | 114.59 deg */,
   /**<k_ltb_ego_deceleration_weight*/ (float32_T) 1.0f,
   /**<k_ltb_ego_circle_host_length_factor*/ (float32_T) 1.0f,
   /**<k_ltb_ego_circle_offset*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ltb_ego_shape_gain_fixed*/ (float32_T) 1.0f,
   /**<k_ltb_ego_acceleration_weight*/ (float32_T) 1.0f,
   /**<k_ltb_zone_width*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_ltb_zone_length*/ (float32_T) 25.0f /**< 25.0 m */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)3,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Ltb_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ltb_Public_Cal_Reverse_Array_Ltb_Cal(Ltb_Public_Calibration_T* cal_dst)
{

}
#endif /* CT_BIG_ENDIAN */


