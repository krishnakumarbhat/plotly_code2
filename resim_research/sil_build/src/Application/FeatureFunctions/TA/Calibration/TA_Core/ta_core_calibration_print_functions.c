/**
* @file ta_core_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ta_core_calibration_t.h" // IWYU pragma: keep
#include "ta_core_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Ta_Core_Cal_Print(FILE* c_file_ptr, const Ta_Core_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_1_ttp_threshold._0_,%f\n", p_cals->k_ta_alert_lvl_1_ttp_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_1_ttp_threshold._1_,%f\n", p_cals->k_ta_alert_lvl_1_ttp_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_1_ttp_threshold._2_,%f\n", p_cals->k_ta_alert_lvl_1_ttp_threshold[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_active_obj_ttp_offset,%f\n", p_cals->k_ta_active_obj_ttp_offset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max,%f\n", p_cals->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max,%f\n", p_cals->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_2_ttc_threshold,%f\n", p_cals->k_ta_alert_lvl_2_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_3_ttc_threshold,%f\n", p_cals->k_ta_alert_lvl_3_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_3_ttb_threshold,%f\n", p_cals->k_ta_alert_lvl_3_ttb_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_4_ttc_threshold,%f\n", p_cals->k_ta_alert_lvl_4_ttc_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_lvl_4_decel_threshold,%f\n", p_cals->k_ta_alert_lvl_4_decel_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_critical_approach_min_safe_distance,%f\n", p_cals->k_ta_critical_approach_min_safe_distance);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_critical_approach_angle_diff_min,%f\n", p_cals->k_ta_critical_approach_angle_diff_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_max_pred_yaw_angle,%f\n", p_cals->k_ta_ego_max_pred_yaw_angle);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_obj_pred_speed_min,%f\n", p_cals->k_ta_obj_pred_speed_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_obj_acceleration_long_weight,%f\n", p_cals->k_ta_obj_acceleration_long_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_obj_acceleration_lat_weight,%f\n", p_cals->k_ta_obj_acceleration_lat_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_shape_gain_per_pred_step,%f\n", p_cals->k_ta_ego_shape_gain_per_pred_step);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_obj_shape_gain_per_pred_step,%f\n", p_cals->k_ta_obj_shape_gain_per_pred_step);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_shape_gain_fixed,%f\n", p_cals->k_ta_ego_shape_gain_fixed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_obj_shape_gain_fixed,%f\n", p_cals->k_ta_obj_shape_gain_fixed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_speed._0_,%f\n", p_cals->k_ta_ego_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_speed._1_,%f\n", p_cals->k_ta_ego_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_speed_ofst._0_,%f\n", p_cals->k_ta_ego_speed_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_speed_ofst._1_,%f\n", p_cals->k_ta_ego_speed_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_yawrate._0_,%f\n", p_cals->k_ta_ego_yawrate[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_yawrate._1_,%f\n", p_cals->k_ta_ego_yawrate[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_yawrate_ofst._0_,%f\n", p_cals->k_ta_ego_yawrate_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_yawrate_ofst._1_,%f\n", p_cals->k_ta_ego_yawrate_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_long_acceleration._0_,%f\n", p_cals->k_ta_ego_long_acceleration[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_long_acceleration._1_,%f\n", p_cals->k_ta_ego_long_acceleration[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_long_acceleration_ofst._0_,%f\n", p_cals->k_ta_ego_long_acceleration_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_long_acceleration_ofst._1_,%f\n", p_cals->k_ta_ego_long_acceleration_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_acceleration_weight,%f\n", p_cals->k_ta_ego_acceleration_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_deceleration_weight,%f\n", p_cals->k_ta_ego_deceleration_weight);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_circle_offset,%f\n", p_cals->k_ta_ego_circle_offset);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_circle_host_length_factor,%f\n", p_cals->k_ta_ego_circle_host_length_factor);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_yawangle_integration_yawrate_min,%f\n", p_cals->k_ta_ego_yawangle_integration_yawrate_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_exist_prblty._0_,%f\n", p_cals->k_fta_obj_exist_prblty[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_exist_prblty._1_,%f\n", p_cals->k_fta_obj_exist_prblty[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_exist_prblty_ofst._0_,%f\n", p_cals->k_fta_obj_exist_prblty_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_exist_prblty_ofst._1_,%f\n", p_cals->k_fta_obj_exist_prblty_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel_rel._0_,%f\n", p_cals->k_fta_obj_vcs_long_vel_rel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel_rel._1_,%f\n", p_cals->k_fta_obj_vcs_long_vel_rel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel_rel_ofst._0_,%f\n", p_cals->k_fta_obj_vcs_long_vel_rel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel_rel_ofst._1_,%f\n", p_cals->k_fta_obj_vcs_long_vel_rel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel_rel._0_,%f\n", p_cals->k_fta_obj_vcs_lat_vel_rel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel_rel._1_,%f\n", p_cals->k_fta_obj_vcs_lat_vel_rel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel_rel_ofst._0_,%f\n", p_cals->k_fta_obj_vcs_lat_vel_rel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel_rel_ofst._1_,%f\n", p_cals->k_fta_obj_vcs_lat_vel_rel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel._0_,%f\n", p_cals->k_fta_obj_vcs_long_vel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel._1_,%f\n", p_cals->k_fta_obj_vcs_long_vel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel_ofst._0_,%f\n", p_cals->k_fta_obj_vcs_long_vel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_vel_ofst._1_,%f\n", p_cals->k_fta_obj_vcs_long_vel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel._0_,%f\n", p_cals->k_fta_obj_vcs_lat_vel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel._1_,%f\n", p_cals->k_fta_obj_vcs_lat_vel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel_ofst._0_,%f\n", p_cals->k_fta_obj_vcs_lat_vel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_lat_vel_ofst._1_,%f\n", p_cals->k_fta_obj_vcs_lat_vel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_straight_host_curvature_max,%f\n", p_cals->k_ta_straight_host_curvature_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_speed._0_,%f\n", p_cals->k_ta_lookup_turning_host_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_speed._1_,%f\n", p_cals->k_ta_lookup_turning_host_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_speed._2_,%f\n", p_cals->k_ta_lookup_turning_host_speed[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_speed._3_,%f\n", p_cals->k_ta_lookup_turning_host_speed[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_curvature_min._0_,%f\n", p_cals->k_ta_lookup_turning_host_curvature_min[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_curvature_min._1_,%f\n", p_cals->k_ta_lookup_turning_host_curvature_min[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_curvature_min._2_,%f\n", p_cals->k_ta_lookup_turning_host_curvature_min[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_lookup_turning_host_curvature_min._3_,%f\n", p_cals->k_ta_lookup_turning_host_curvature_min[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vcs_long_pos_straight_min,%f\n", p_cals->k_fta_obj_vcs_long_pos_straight_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading._0_,%f\n", p_cals->k_fta_obj_heading[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading._1_,%f\n", p_cals->k_fta_obj_heading[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_straight._0_,%f\n", p_cals->k_fta_obj_heading_straight[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_straight._1_,%f\n", p_cals->k_fta_obj_heading_straight[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_ofst._0_,%f\n", p_cals->k_fta_obj_heading_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_ofst._1_,%f\n", p_cals->k_fta_obj_heading_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_rate._0_,%f\n", p_cals->k_fta_obj_heading_rate[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_rate._1_,%f\n", p_cals->k_fta_obj_heading_rate[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_rate_straight._0_,%f\n", p_cals->k_fta_obj_heading_rate_straight[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_rate_straight._1_,%f\n", p_cals->k_fta_obj_heading_rate_straight[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_rate_ofst._0_,%f\n", p_cals->k_fta_obj_heading_rate_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_heading_rate_ofst._1_,%f\n", p_cals->k_fta_obj_heading_rate_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_speed._0_,%f\n", p_cals->k_fta_obj_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_speed._1_,%f\n", p_cals->k_fta_obj_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_speed_straight._0_,%f\n", p_cals->k_fta_obj_speed_straight[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_speed_straight._1_,%f\n", p_cals->k_fta_obj_speed_straight[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_speed_ofst._0_,%f\n", p_cals->k_fta_obj_speed_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_speed_ofst._1_,%f\n", p_cals->k_fta_obj_speed_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_length._0_,%f\n", p_cals->k_fta_obj_length[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_length._1_,%f\n", p_cals->k_fta_obj_length[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_length_ofst._0_,%f\n", p_cals->k_fta_obj_length_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_length_ofst._1_,%f\n", p_cals->k_fta_obj_length_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_width._0_,%f\n", p_cals->k_fta_obj_width[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_width._1_,%f\n", p_cals->k_fta_obj_width[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_width_ofst._0_,%f\n", p_cals->k_fta_obj_width_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_width_ofst._1_,%f\n", p_cals->k_fta_obj_width_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_area._0_,%f\n", p_cals->k_fta_obj_area[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_area._1_,%f\n", p_cals->k_fta_obj_area[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_area_ofst._0_,%f\n", p_cals->k_fta_obj_area_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_area_ofst._1_,%f\n", p_cals->k_fta_obj_area_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vru_class_prob._0_,%f\n", p_cals->k_fta_obj_vru_class_prob[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vru_class_prob._1_,%f\n", p_cals->k_fta_obj_vru_class_prob[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vru_class_prob_ofst._0_,%f\n", p_cals->k_fta_obj_vru_class_prob_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_vru_class_prob_ofst._1_,%f\n", p_cals->k_fta_obj_vru_class_prob_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_ego_obj_heading_diff._0_,%f\n", p_cals->k_fta_ego_obj_heading_diff[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_ego_obj_heading_diff._1_,%f\n", p_cals->k_fta_ego_obj_heading_diff[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_ego_obj_heading_diff_ofst._0_,%f\n", p_cals->k_fta_ego_obj_heading_diff_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_ego_obj_heading_diff_ofst._1_,%f\n", p_cals->k_fta_ego_obj_heading_diff_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_eclipse_value._0_,%f\n", p_cals->k_fta_obj_eclipse_value[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_eclipse_value._1_,%f\n", p_cals->k_fta_obj_eclipse_value[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_eclipse_value_ofst._0_,%f\n", p_cals->k_fta_obj_eclipse_value_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_eclipse_value_ofst._1_,%f\n", p_cals->k_fta_obj_eclipse_value_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_velocity_heading_diff_max,%f\n", p_cals->k_fta_obj_velocity_heading_diff_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_brake_deceleration_max,%f\n", p_cals->k_fta_brake_deceleration_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_brake_dead_time,%f\n", p_cals->k_fta_brake_dead_time);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_brake_gradient,%f\n", p_cals->k_fta_brake_gradient);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._0_,%f\n", p_cals->k_fta_danger_zone_left_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._1_,%f\n", p_cals->k_fta_danger_zone_left_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._2_,%f\n", p_cals->k_fta_danger_zone_left_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._3_,%f\n", p_cals->k_fta_danger_zone_left_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._4_,%f\n", p_cals->k_fta_danger_zone_left_long[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._5_,%f\n", p_cals->k_fta_danger_zone_left_long[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_long._6_,%f\n", p_cals->k_fta_danger_zone_left_long[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._0_,%f\n", p_cals->k_fta_danger_zone_left_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._1_,%f\n", p_cals->k_fta_danger_zone_left_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._2_,%f\n", p_cals->k_fta_danger_zone_left_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._3_,%f\n", p_cals->k_fta_danger_zone_left_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._4_,%f\n", p_cals->k_fta_danger_zone_left_lat[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._5_,%f\n", p_cals->k_fta_danger_zone_left_lat[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_left_lat._6_,%f\n", p_cals->k_fta_danger_zone_left_lat[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._0_,%f\n", p_cals->k_fta_danger_zone_right_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._1_,%f\n", p_cals->k_fta_danger_zone_right_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._2_,%f\n", p_cals->k_fta_danger_zone_right_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._3_,%f\n", p_cals->k_fta_danger_zone_right_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._4_,%f\n", p_cals->k_fta_danger_zone_right_long[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._5_,%f\n", p_cals->k_fta_danger_zone_right_long[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_long._6_,%f\n", p_cals->k_fta_danger_zone_right_long[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._0_,%f\n", p_cals->k_fta_danger_zone_right_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._1_,%f\n", p_cals->k_fta_danger_zone_right_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._2_,%f\n", p_cals->k_fta_danger_zone_right_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._3_,%f\n", p_cals->k_fta_danger_zone_right_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._4_,%f\n", p_cals->k_fta_danger_zone_right_lat[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._5_,%f\n", p_cals->k_fta_danger_zone_right_lat[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_right_lat._6_,%f\n", p_cals->k_fta_danger_zone_right_lat[6]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_ego_speed._0_,%f\n", p_cals->k_pfgs_ego_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_ego_speed._1_,%f\n", p_cals->k_pfgs_ego_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_qualification_ttc_min,%f\n", p_cals->k_pfgs_qualification_ttc_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_tap_lvl_2_host_curvature_min,%f\n", p_cals->k_tap_lvl_2_host_curvature_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_exist_prblty._0_,%f\n", p_cals->k_rta_obj_exist_prblty[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_exist_prblty._1_,%f\n", p_cals->k_rta_obj_exist_prblty[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_exist_prblty_ofst._0_,%f\n", p_cals->k_rta_obj_exist_prblty_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_exist_prblty_ofst._1_,%f\n", p_cals->k_rta_obj_exist_prblty_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel_rel._0_,%f\n", p_cals->k_rta_obj_vcs_long_vel_rel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel_rel._1_,%f\n", p_cals->k_rta_obj_vcs_long_vel_rel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel_rel_ofst._0_,%f\n", p_cals->k_rta_obj_vcs_long_vel_rel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel_rel_ofst._1_,%f\n", p_cals->k_rta_obj_vcs_long_vel_rel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel_rel._0_,%f\n", p_cals->k_rta_obj_vcs_lat_vel_rel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel_rel._1_,%f\n", p_cals->k_rta_obj_vcs_lat_vel_rel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel_rel_ofst._0_,%f\n", p_cals->k_rta_obj_vcs_lat_vel_rel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel_rel_ofst._1_,%f\n", p_cals->k_rta_obj_vcs_lat_vel_rel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel._0_,%f\n", p_cals->k_rta_obj_vcs_long_vel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel._1_,%f\n", p_cals->k_rta_obj_vcs_long_vel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel_ofst._0_,%f\n", p_cals->k_rta_obj_vcs_long_vel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_long_vel_ofst._1_,%f\n", p_cals->k_rta_obj_vcs_long_vel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel._0_,%f\n", p_cals->k_rta_obj_vcs_lat_vel[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel._1_,%f\n", p_cals->k_rta_obj_vcs_lat_vel[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel_ofst._0_,%f\n", p_cals->k_rta_obj_vcs_lat_vel_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vcs_lat_vel_ofst._1_,%f\n", p_cals->k_rta_obj_vcs_lat_vel_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_heading._0_,%f\n", p_cals->k_rta_obj_heading[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_heading._1_,%f\n", p_cals->k_rta_obj_heading[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_heading_ofst._0_,%f\n", p_cals->k_rta_obj_heading_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_heading_ofst._1_,%f\n", p_cals->k_rta_obj_heading_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_speed._0_,%f\n", p_cals->k_rta_obj_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_speed._1_,%f\n", p_cals->k_rta_obj_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_speed_ofst._0_,%f\n", p_cals->k_rta_obj_speed_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_speed_ofst._1_,%f\n", p_cals->k_rta_obj_speed_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_length._0_,%f\n", p_cals->k_rta_obj_length[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_length._1_,%f\n", p_cals->k_rta_obj_length[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_length_ofst._0_,%f\n", p_cals->k_rta_obj_length_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_length_ofst._1_,%f\n", p_cals->k_rta_obj_length_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_width._0_,%f\n", p_cals->k_rta_obj_width[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_width._1_,%f\n", p_cals->k_rta_obj_width[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_width_ofst._0_,%f\n", p_cals->k_rta_obj_width_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_width_ofst._1_,%f\n", p_cals->k_rta_obj_width_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vru_class_prob._0_,%f\n", p_cals->k_rta_obj_vru_class_prob[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vru_class_prob._1_,%f\n", p_cals->k_rta_obj_vru_class_prob[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vru_class_prob_ofst._0_,%f\n", p_cals->k_rta_obj_vru_class_prob_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_vru_class_prob_ofst._1_,%f\n", p_cals->k_rta_obj_vru_class_prob_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_eclipse_value._0_,%f\n", p_cals->k_rta_obj_eclipse_value[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_eclipse_value._1_,%f\n", p_cals->k_rta_obj_eclipse_value[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_eclipse_value_ofst._0_,%f\n", p_cals->k_rta_obj_eclipse_value_ofst[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_eclipse_value_ofst._1_,%f\n", p_cals->k_rta_obj_eclipse_value_ofst[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_ttp_obj_abs_lat_vel_rel_max,%f\n", p_cals->k_rta_ttp_obj_abs_lat_vel_rel_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_ttp_obj_abs_heading_diff_max,%f\n", p_cals->k_rta_ttp_obj_abs_heading_diff_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_ttp_curve_suppression_obj_distance_min,%f\n", p_cals->k_rta_ttp_curve_suppression_obj_distance_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long._0_,%f\n", p_cals->k_rta_info_zone_left_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long._1_,%f\n", p_cals->k_rta_info_zone_left_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long._2_,%f\n", p_cals->k_rta_info_zone_left_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long._3_,%f\n", p_cals->k_rta_info_zone_left_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat._0_,%f\n", p_cals->k_rta_info_zone_left_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat._1_,%f\n", p_cals->k_rta_info_zone_left_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat._2_,%f\n", p_cals->k_rta_info_zone_left_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat._3_,%f\n", p_cals->k_rta_info_zone_left_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long._0_,%f\n", p_cals->k_rta_info_zone_right_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long._1_,%f\n", p_cals->k_rta_info_zone_right_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long._2_,%f\n", p_cals->k_rta_info_zone_right_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long._3_,%f\n", p_cals->k_rta_info_zone_right_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat._0_,%f\n", p_cals->k_rta_info_zone_right_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat._1_,%f\n", p_cals->k_rta_info_zone_right_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat._2_,%f\n", p_cals->k_rta_info_zone_right_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat._3_,%f\n", p_cals->k_rta_info_zone_right_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long_hys._0_,%f\n", p_cals->k_rta_info_zone_left_long_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long_hys._1_,%f\n", p_cals->k_rta_info_zone_left_long_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long_hys._2_,%f\n", p_cals->k_rta_info_zone_left_long_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_long_hys._3_,%f\n", p_cals->k_rta_info_zone_left_long_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat_hys._0_,%f\n", p_cals->k_rta_info_zone_left_lat_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat_hys._1_,%f\n", p_cals->k_rta_info_zone_left_lat_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat_hys._2_,%f\n", p_cals->k_rta_info_zone_left_lat_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_left_lat_hys._3_,%f\n", p_cals->k_rta_info_zone_left_lat_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long_hys._0_,%f\n", p_cals->k_rta_info_zone_right_long_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long_hys._1_,%f\n", p_cals->k_rta_info_zone_right_long_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long_hys._2_,%f\n", p_cals->k_rta_info_zone_right_long_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_long_hys._3_,%f\n", p_cals->k_rta_info_zone_right_long_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat_hys._0_,%f\n", p_cals->k_rta_info_zone_right_lat_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat_hys._1_,%f\n", p_cals->k_rta_info_zone_right_lat_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat_hys._2_,%f\n", p_cals->k_rta_info_zone_right_lat_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_right_lat_hys._3_,%f\n", p_cals->k_rta_info_zone_right_lat_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long._0_,%f\n", p_cals->k_rta_wing_zone_left_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long._1_,%f\n", p_cals->k_rta_wing_zone_left_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long._2_,%f\n", p_cals->k_rta_wing_zone_left_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long._3_,%f\n", p_cals->k_rta_wing_zone_left_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat._0_,%f\n", p_cals->k_rta_wing_zone_left_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat._1_,%f\n", p_cals->k_rta_wing_zone_left_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat._2_,%f\n", p_cals->k_rta_wing_zone_left_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat._3_,%f\n", p_cals->k_rta_wing_zone_left_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long._0_,%f\n", p_cals->k_rta_wing_zone_right_long[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long._1_,%f\n", p_cals->k_rta_wing_zone_right_long[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long._2_,%f\n", p_cals->k_rta_wing_zone_right_long[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long._3_,%f\n", p_cals->k_rta_wing_zone_right_long[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat._0_,%f\n", p_cals->k_rta_wing_zone_right_lat[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat._1_,%f\n", p_cals->k_rta_wing_zone_right_lat[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat._2_,%f\n", p_cals->k_rta_wing_zone_right_lat[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat._3_,%f\n", p_cals->k_rta_wing_zone_right_lat[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long_hys._0_,%f\n", p_cals->k_rta_wing_zone_left_long_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long_hys._1_,%f\n", p_cals->k_rta_wing_zone_left_long_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long_hys._2_,%f\n", p_cals->k_rta_wing_zone_left_long_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_long_hys._3_,%f\n", p_cals->k_rta_wing_zone_left_long_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat_hys._0_,%f\n", p_cals->k_rta_wing_zone_left_lat_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat_hys._1_,%f\n", p_cals->k_rta_wing_zone_left_lat_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat_hys._2_,%f\n", p_cals->k_rta_wing_zone_left_lat_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_left_lat_hys._3_,%f\n", p_cals->k_rta_wing_zone_left_lat_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long_hys._0_,%f\n", p_cals->k_rta_wing_zone_right_long_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long_hys._1_,%f\n", p_cals->k_rta_wing_zone_right_long_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long_hys._2_,%f\n", p_cals->k_rta_wing_zone_right_long_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_long_hys._3_,%f\n", p_cals->k_rta_wing_zone_right_long_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat_hys._0_,%f\n", p_cals->k_rta_wing_zone_right_lat_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat_hys._1_,%f\n", p_cals->k_rta_wing_zone_right_lat_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat_hys._2_,%f\n", p_cals->k_rta_wing_zone_right_lat_hys[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_right_lat_hys._3_,%f\n", p_cals->k_rta_wing_zone_right_lat_hys[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_always_overwrite_ta_mode_to_both,%d\n", p_cals->k_ta_always_overwrite_ta_mode_to_both);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_f_only_allow_consecutive_ttc_based_alert_levels,%d\n", p_cals->k_ta_f_only_allow_consecutive_ttc_based_alert_levels);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_f_skip_holding_for_single_alert_level_drop,%d\n", p_cals->k_ta_f_skip_holding_for_single_alert_level_drop);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj,%d\n", p_cals->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_f_apply_ttp_hysteresis_globally,%d\n", p_cals->k_ta_f_apply_ttp_hysteresis_globally);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_ta_enable_debug_mode,%d\n", p_cals->k_f_ta_enable_debug_mode);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_fta_enable,%d\n", p_cals->k_f_fta_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_fta_enable_brake_gradient_logic,%d\n", p_cals->k_f_fta_enable_brake_gradient_logic);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_fta_enable_danger_zones,%d\n", p_cals->k_f_fta_enable_danger_zones);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_symbol_request_sides_enabled,%d\n", p_cals->k_pfgs_symbol_request_sides_enabled);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_qualification_check_f_stationary,%d\n", p_cals->k_pfgs_qualification_check_f_stationary);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_rta_enable,%d\n", p_cals->k_f_rta_enable);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_rta_enable_info_zones,%d\n", p_cals->k_f_rta_enable_info_zones);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_f_higher_obj_crit_based_on_lower_ttp,%d\n", p_cals->k_rta_f_higher_obj_crit_based_on_lower_ttp);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_f_rta_enable_wing_zones,%d\n", p_cals->k_f_rta_enable_wing_zones);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_qualifying_cycles,%d\n", p_cals->k_ta_alert_qualifying_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_alert_holding_cycles,%d\n", p_cals->k_ta_alert_holding_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_prediction_steps_max,%d\n", p_cals->k_ta_prediction_steps_max);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_critical_approach_check_ego_circles._0_,%d\n", p_cals->k_ta_critical_approach_check_ego_circles[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_critical_approach_check_ego_circles._1_,%d\n", p_cals->k_ta_critical_approach_check_ego_circles[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_critical_approach_check_ego_circles._2_,%d\n", p_cals->k_ta_critical_approach_check_ego_circles[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_ta_ego_pred_const_velocity_pred_steps_min,%d\n", p_cals->k_ta_ego_pred_const_velocity_pred_steps_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_obj_age_min,%d\n", p_cals->k_fta_obj_age_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_fta_danger_zone_point_size,%d\n", p_cals->k_fta_danger_zone_point_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_qualification_counter_fast_obj,%d\n", p_cals->k_pfgs_qualification_counter_fast_obj);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_pfgs_qualification_counter_slow_obj,%d\n", p_cals->k_pfgs_qualification_counter_slow_obj);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_obj_age_min,%d\n", p_cals->k_rta_obj_age_min);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_info_zone_point_size,%d\n", p_cals->k_rta_info_zone_point_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_rta_wing_zone_point_size,%d\n", p_cals->k_rta_wing_zone_point_size);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_0,%d\n", p_cals->k_unused_padding_byte_0);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_1,%d\n", p_cals->k_unused_padding_byte_1);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_unused_padding_byte_2,%d\n", p_cals->k_unused_padding_byte_2);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
