/**
* @file ltb_core_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in ltb_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ltb_core_calibration_t.h" // IWYU pragma: keep
#include "ltb_core_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Ltb_Core_Cal_Print(FILE* c_file_ptr, const Ltb_Core_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_zone_length,%f\n", p_cals->k_ltb_zone_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_zone_width,%f\n", p_cals->k_ltb_zone_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_acceleration_weight,%f\n", p_cals->k_ltb_ego_acceleration_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_shape_gain_fixed,%f\n", p_cals->k_ltb_ego_shape_gain_fixed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_circle_offset,%f\n", p_cals->k_ltb_ego_circle_offset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_circle_host_length_factor,%f\n", p_cals->k_ltb_ego_circle_host_length_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_deceleration_weight,%f\n", p_cals->k_ltb_ego_deceleration_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_max_pred_yaw_angle,%f\n", p_cals->k_ltb_ego_max_pred_yaw_angle);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_yawangle_integration_yawrate_min,%f\n", p_cals->k_ltb_ego_yawangle_integration_yawrate_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_shape_gain_per_pred_step,%f\n", p_cals->k_ltb_ego_shape_gain_per_pred_step);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_obj_pred_speed_min,%f\n", p_cals->k_ltb_obj_pred_speed_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_obj_shape_gain_fixed,%f\n", p_cals->k_ltb_obj_shape_gain_fixed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_obj_shape_gain_per_pred_step,%f\n", p_cals->k_ltb_obj_shape_gain_per_pred_step);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_critical_approach_min_safe_distance,%f\n", p_cals->k_ltb_critical_approach_min_safe_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_critical_approach_angle_diff_min,%f\n", p_cals->k_ltb_critical_approach_angle_diff_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_lvl_1_ttc_threshold,%f\n", p_cals->k_ltb_alert_lvl_1_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_lvl_2_ttc_threshold,%f\n", p_cals->k_ltb_alert_lvl_2_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_lvl_2_ttb_threshold,%f\n", p_cals->k_ltb_alert_lvl_2_ttb_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_lvl_3_ttc_threshold,%f\n", p_cals->k_ltb_alert_lvl_3_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_lvl_3_decel_threshold,%f\n", p_cals->k_ltb_alert_lvl_3_decel_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_brake_deceleration_max,%f\n", p_cals->k_ltb_brake_deceleration_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_brake_dead_time,%f\n", p_cals->k_ltb_brake_dead_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_brake_gradient,%f\n", p_cals->k_ltb_brake_gradient);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_object_long_vel_min,%f\n", p_cals->k_ltb_object_long_vel_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_bmw_sp25_v_ego_max,%f\n", p_cals->k_ltb_bmw_sp25_v_ego_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_bmw_sp25_v_ego_max_hys,%f\n", p_cals->k_ltb_bmw_sp25_v_ego_max_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_f_only_allow_consecutive_ttc_based_alert_levels,%d\n", p_cals->k_ltb_f_only_allow_consecutive_ttc_based_alert_levels);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_f_skip_holding_for_single_alert_level_drop,%d\n", p_cals->k_ltb_f_skip_holding_for_single_alert_level_drop);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_ltb_enable_brake_gradient_logic,%d\n", p_cals->k_f_ltb_enable_brake_gradient_logic);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_ego_pred_const_velocity_pred_steps_min,%d\n", p_cals->k_ltb_ego_pred_const_velocity_pred_steps_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_prediction_steps_max,%d\n", p_cals->k_ltb_prediction_steps_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_qualifying_cycles,%d\n", p_cals->k_ltb_alert_qualifying_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ltb_alert_holding_cycles,%d\n", p_cals->k_ltb_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
