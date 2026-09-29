/**
* @file ta_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "ta_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "ta_core_calibration_check.h"
#include "ta_core_calibration_t.h"
#include "ta_customer_calibration_t.h"
#include "ta_public_calibration_check.h"
#include "ta_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ta_Update_Core_Cal_By_Public(Ta_Core_Calibration_T* cal_dst, const Ta_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ta_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_ta_alert_lvl_1_ttp_threshold[0] = cal_src->k_ta_alert_lvl_1_ttp_threshold[0];
        cal_dst->k_ta_alert_lvl_1_ttp_threshold[1] = cal_src->k_ta_alert_lvl_1_ttp_threshold[1];
        cal_dst->k_ta_alert_lvl_1_ttp_threshold[2] = cal_src->k_ta_alert_lvl_1_ttp_threshold[2];
        cal_dst->k_ta_active_obj_ttp_offset = cal_src->k_ta_active_obj_ttp_offset;
        cal_dst->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max = cal_src->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max;
        cal_dst->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max = cal_src->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max;
        cal_dst->k_ta_alert_lvl_2_ttc_threshold = cal_src->k_ta_alert_lvl_2_ttc_threshold;
        cal_dst->k_ta_alert_lvl_3_ttc_threshold = cal_src->k_ta_alert_lvl_3_ttc_threshold;
        cal_dst->k_ta_alert_lvl_3_ttb_threshold = cal_src->k_ta_alert_lvl_3_ttb_threshold;
        cal_dst->k_ta_alert_lvl_4_ttc_threshold = cal_src->k_ta_alert_lvl_4_ttc_threshold;
        cal_dst->k_ta_alert_lvl_4_decel_threshold = cal_src->k_ta_alert_lvl_4_decel_threshold;
        cal_dst->k_ta_critical_approach_min_safe_distance = cal_src->k_ta_critical_approach_min_safe_distance;
        cal_dst->k_ta_critical_approach_angle_diff_min = cal_src->k_ta_critical_approach_angle_diff_min;
        cal_dst->k_ta_ego_max_pred_yaw_angle = cal_src->k_ta_ego_max_pred_yaw_angle;
        cal_dst->k_ta_obj_pred_speed_min = cal_src->k_ta_obj_pred_speed_min;
        cal_dst->k_ta_obj_acceleration_long_weight = cal_src->k_ta_obj_acceleration_long_weight;
        cal_dst->k_ta_obj_acceleration_lat_weight = cal_src->k_ta_obj_acceleration_lat_weight;
        cal_dst->k_ta_ego_shape_gain_per_pred_step = cal_src->k_ta_ego_shape_gain_per_pred_step;
        cal_dst->k_ta_obj_shape_gain_per_pred_step = cal_src->k_ta_obj_shape_gain_per_pred_step;
        cal_dst->k_ta_ego_shape_gain_fixed = cal_src->k_ta_ego_shape_gain_fixed;
        cal_dst->k_ta_obj_shape_gain_fixed = cal_src->k_ta_obj_shape_gain_fixed;
        cal_dst->k_ta_ego_speed[0] = cal_src->k_ta_ego_speed[0];
        cal_dst->k_ta_ego_speed[1] = cal_src->k_ta_ego_speed[1];
        cal_dst->k_ta_ego_speed_ofst[0] = cal_src->k_ta_ego_speed_ofst[0];
        cal_dst->k_ta_ego_speed_ofst[1] = cal_src->k_ta_ego_speed_ofst[1];
        cal_dst->k_ta_ego_yawrate[0] = cal_src->k_ta_ego_yawrate[0];
        cal_dst->k_ta_ego_yawrate[1] = cal_src->k_ta_ego_yawrate[1];
        cal_dst->k_ta_ego_yawrate_ofst[0] = cal_src->k_ta_ego_yawrate_ofst[0];
        cal_dst->k_ta_ego_yawrate_ofst[1] = cal_src->k_ta_ego_yawrate_ofst[1];
        cal_dst->k_ta_ego_long_acceleration[0] = cal_src->k_ta_ego_long_acceleration[0];
        cal_dst->k_ta_ego_long_acceleration[1] = cal_src->k_ta_ego_long_acceleration[1];
        cal_dst->k_ta_ego_long_acceleration_ofst[0] = cal_src->k_ta_ego_long_acceleration_ofst[0];
        cal_dst->k_ta_ego_long_acceleration_ofst[1] = cal_src->k_ta_ego_long_acceleration_ofst[1];
        cal_dst->k_ta_ego_acceleration_weight = cal_src->k_ta_ego_acceleration_weight;
        cal_dst->k_ta_ego_deceleration_weight = cal_src->k_ta_ego_deceleration_weight;
        cal_dst->k_ta_ego_circle_offset = cal_src->k_ta_ego_circle_offset;
        cal_dst->k_ta_ego_circle_host_length_factor = cal_src->k_ta_ego_circle_host_length_factor;
        cal_dst->k_ta_ego_yawangle_integration_yawrate_min = cal_src->k_ta_ego_yawangle_integration_yawrate_min;
        cal_dst->k_fta_obj_exist_prblty[0] = cal_src->k_fta_obj_exist_prblty[0];
        cal_dst->k_fta_obj_exist_prblty[1] = cal_src->k_fta_obj_exist_prblty[1];
        cal_dst->k_fta_obj_exist_prblty_ofst[0] = cal_src->k_fta_obj_exist_prblty_ofst[0];
        cal_dst->k_fta_obj_exist_prblty_ofst[1] = cal_src->k_fta_obj_exist_prblty_ofst[1];
        cal_dst->k_fta_obj_vcs_long_vel_rel[0] = cal_src->k_fta_obj_vcs_long_vel_rel[0];
        cal_dst->k_fta_obj_vcs_long_vel_rel[1] = cal_src->k_fta_obj_vcs_long_vel_rel[1];
        cal_dst->k_fta_obj_vcs_long_vel_rel_ofst[0] = cal_src->k_fta_obj_vcs_long_vel_rel_ofst[0];
        cal_dst->k_fta_obj_vcs_long_vel_rel_ofst[1] = cal_src->k_fta_obj_vcs_long_vel_rel_ofst[1];
        cal_dst->k_fta_obj_vcs_lat_vel_rel[0] = cal_src->k_fta_obj_vcs_lat_vel_rel[0];
        cal_dst->k_fta_obj_vcs_lat_vel_rel[1] = cal_src->k_fta_obj_vcs_lat_vel_rel[1];
        cal_dst->k_fta_obj_vcs_lat_vel_rel_ofst[0] = cal_src->k_fta_obj_vcs_lat_vel_rel_ofst[0];
        cal_dst->k_fta_obj_vcs_lat_vel_rel_ofst[1] = cal_src->k_fta_obj_vcs_lat_vel_rel_ofst[1];
        cal_dst->k_fta_obj_vcs_long_vel[0] = cal_src->k_fta_obj_vcs_long_vel[0];
        cal_dst->k_fta_obj_vcs_long_vel[1] = cal_src->k_fta_obj_vcs_long_vel[1];
        cal_dst->k_fta_obj_vcs_long_vel_ofst[0] = cal_src->k_fta_obj_vcs_long_vel_ofst[0];
        cal_dst->k_fta_obj_vcs_long_vel_ofst[1] = cal_src->k_fta_obj_vcs_long_vel_ofst[1];
        cal_dst->k_fta_obj_vcs_lat_vel[0] = cal_src->k_fta_obj_vcs_lat_vel[0];
        cal_dst->k_fta_obj_vcs_lat_vel[1] = cal_src->k_fta_obj_vcs_lat_vel[1];
        cal_dst->k_fta_obj_vcs_lat_vel_ofst[0] = cal_src->k_fta_obj_vcs_lat_vel_ofst[0];
        cal_dst->k_fta_obj_vcs_lat_vel_ofst[1] = cal_src->k_fta_obj_vcs_lat_vel_ofst[1];
        cal_dst->k_ta_straight_host_curvature_max = cal_src->k_ta_straight_host_curvature_max;
        cal_dst->k_ta_lookup_turning_host_speed[0] = cal_src->k_ta_lookup_turning_host_speed[0];
        cal_dst->k_ta_lookup_turning_host_speed[1] = cal_src->k_ta_lookup_turning_host_speed[1];
        cal_dst->k_ta_lookup_turning_host_speed[2] = cal_src->k_ta_lookup_turning_host_speed[2];
        cal_dst->k_ta_lookup_turning_host_speed[3] = cal_src->k_ta_lookup_turning_host_speed[3];
        cal_dst->k_ta_lookup_turning_host_curvature_min[0] = cal_src->k_ta_lookup_turning_host_curvature_min[0];
        cal_dst->k_ta_lookup_turning_host_curvature_min[1] = cal_src->k_ta_lookup_turning_host_curvature_min[1];
        cal_dst->k_ta_lookup_turning_host_curvature_min[2] = cal_src->k_ta_lookup_turning_host_curvature_min[2];
        cal_dst->k_ta_lookup_turning_host_curvature_min[3] = cal_src->k_ta_lookup_turning_host_curvature_min[3];
        cal_dst->k_fta_obj_vcs_long_pos_straight_min = cal_src->k_fta_obj_vcs_long_pos_straight_min;
        cal_dst->k_fta_obj_heading[0] = cal_src->k_fta_obj_heading[0];
        cal_dst->k_fta_obj_heading[1] = cal_src->k_fta_obj_heading[1];
        cal_dst->k_fta_obj_heading_straight[0] = cal_src->k_fta_obj_heading_straight[0];
        cal_dst->k_fta_obj_heading_straight[1] = cal_src->k_fta_obj_heading_straight[1];
        cal_dst->k_fta_obj_heading_ofst[0] = cal_src->k_fta_obj_heading_ofst[0];
        cal_dst->k_fta_obj_heading_ofst[1] = cal_src->k_fta_obj_heading_ofst[1];
        cal_dst->k_fta_obj_heading_rate[0] = cal_src->k_fta_obj_heading_rate[0];
        cal_dst->k_fta_obj_heading_rate[1] = cal_src->k_fta_obj_heading_rate[1];
        cal_dst->k_fta_obj_heading_rate_straight[0] = cal_src->k_fta_obj_heading_rate_straight[0];
        cal_dst->k_fta_obj_heading_rate_straight[1] = cal_src->k_fta_obj_heading_rate_straight[1];
        cal_dst->k_fta_obj_heading_rate_ofst[0] = cal_src->k_fta_obj_heading_rate_ofst[0];
        cal_dst->k_fta_obj_heading_rate_ofst[1] = cal_src->k_fta_obj_heading_rate_ofst[1];
        cal_dst->k_fta_obj_speed[0] = cal_src->k_fta_obj_speed[0];
        cal_dst->k_fta_obj_speed[1] = cal_src->k_fta_obj_speed[1];
        cal_dst->k_fta_obj_speed_straight[0] = cal_src->k_fta_obj_speed_straight[0];
        cal_dst->k_fta_obj_speed_straight[1] = cal_src->k_fta_obj_speed_straight[1];
        cal_dst->k_fta_obj_speed_ofst[0] = cal_src->k_fta_obj_speed_ofst[0];
        cal_dst->k_fta_obj_speed_ofst[1] = cal_src->k_fta_obj_speed_ofst[1];
        cal_dst->k_fta_obj_length[0] = cal_src->k_fta_obj_length[0];
        cal_dst->k_fta_obj_length[1] = cal_src->k_fta_obj_length[1];
        cal_dst->k_fta_obj_length_ofst[0] = cal_src->k_fta_obj_length_ofst[0];
        cal_dst->k_fta_obj_length_ofst[1] = cal_src->k_fta_obj_length_ofst[1];
        cal_dst->k_fta_obj_width[0] = cal_src->k_fta_obj_width[0];
        cal_dst->k_fta_obj_width[1] = cal_src->k_fta_obj_width[1];
        cal_dst->k_fta_obj_width_ofst[0] = cal_src->k_fta_obj_width_ofst[0];
        cal_dst->k_fta_obj_width_ofst[1] = cal_src->k_fta_obj_width_ofst[1];
        cal_dst->k_fta_obj_area[0] = cal_src->k_fta_obj_area[0];
        cal_dst->k_fta_obj_area[1] = cal_src->k_fta_obj_area[1];
        cal_dst->k_fta_obj_area_ofst[0] = cal_src->k_fta_obj_area_ofst[0];
        cal_dst->k_fta_obj_area_ofst[1] = cal_src->k_fta_obj_area_ofst[1];
        cal_dst->k_fta_obj_vru_class_prob[0] = cal_src->k_fta_obj_vru_class_prob[0];
        cal_dst->k_fta_obj_vru_class_prob[1] = cal_src->k_fta_obj_vru_class_prob[1];
        cal_dst->k_fta_obj_vru_class_prob_ofst[0] = cal_src->k_fta_obj_vru_class_prob_ofst[0];
        cal_dst->k_fta_obj_vru_class_prob_ofst[1] = cal_src->k_fta_obj_vru_class_prob_ofst[1];
        cal_dst->k_fta_ego_obj_heading_diff[0] = cal_src->k_fta_ego_obj_heading_diff[0];
        cal_dst->k_fta_ego_obj_heading_diff[1] = cal_src->k_fta_ego_obj_heading_diff[1];
        cal_dst->k_fta_ego_obj_heading_diff_ofst[0] = cal_src->k_fta_ego_obj_heading_diff_ofst[0];
        cal_dst->k_fta_ego_obj_heading_diff_ofst[1] = cal_src->k_fta_ego_obj_heading_diff_ofst[1];
        cal_dst->k_fta_obj_eclipse_value[0] = cal_src->k_fta_obj_eclipse_value[0];
        cal_dst->k_fta_obj_eclipse_value[1] = cal_src->k_fta_obj_eclipse_value[1];
        cal_dst->k_fta_obj_eclipse_value_ofst[0] = cal_src->k_fta_obj_eclipse_value_ofst[0];
        cal_dst->k_fta_obj_eclipse_value_ofst[1] = cal_src->k_fta_obj_eclipse_value_ofst[1];
        cal_dst->k_fta_obj_velocity_heading_diff_max = cal_src->k_fta_obj_velocity_heading_diff_max;
        cal_dst->k_fta_brake_deceleration_max = cal_src->k_fta_brake_deceleration_max;
        cal_dst->k_fta_brake_dead_time = cal_src->k_fta_brake_dead_time;
        cal_dst->k_fta_brake_gradient = cal_src->k_fta_brake_gradient;
        cal_dst->k_fta_danger_zone_left_long[0] = cal_src->k_fta_danger_zone_left_long[0];
        cal_dst->k_fta_danger_zone_left_long[1] = cal_src->k_fta_danger_zone_left_long[1];
        cal_dst->k_fta_danger_zone_left_long[2] = cal_src->k_fta_danger_zone_left_long[2];
        cal_dst->k_fta_danger_zone_left_long[3] = cal_src->k_fta_danger_zone_left_long[3];
        cal_dst->k_fta_danger_zone_left_long[4] = cal_src->k_fta_danger_zone_left_long[4];
        cal_dst->k_fta_danger_zone_left_long[5] = cal_src->k_fta_danger_zone_left_long[5];
        cal_dst->k_fta_danger_zone_left_long[6] = cal_src->k_fta_danger_zone_left_long[6];
        cal_dst->k_fta_danger_zone_left_lat[0] = cal_src->k_fta_danger_zone_left_lat[0];
        cal_dst->k_fta_danger_zone_left_lat[1] = cal_src->k_fta_danger_zone_left_lat[1];
        cal_dst->k_fta_danger_zone_left_lat[2] = cal_src->k_fta_danger_zone_left_lat[2];
        cal_dst->k_fta_danger_zone_left_lat[3] = cal_src->k_fta_danger_zone_left_lat[3];
        cal_dst->k_fta_danger_zone_left_lat[4] = cal_src->k_fta_danger_zone_left_lat[4];
        cal_dst->k_fta_danger_zone_left_lat[5] = cal_src->k_fta_danger_zone_left_lat[5];
        cal_dst->k_fta_danger_zone_left_lat[6] = cal_src->k_fta_danger_zone_left_lat[6];
        cal_dst->k_fta_danger_zone_right_long[0] = cal_src->k_fta_danger_zone_right_long[0];
        cal_dst->k_fta_danger_zone_right_long[1] = cal_src->k_fta_danger_zone_right_long[1];
        cal_dst->k_fta_danger_zone_right_long[2] = cal_src->k_fta_danger_zone_right_long[2];
        cal_dst->k_fta_danger_zone_right_long[3] = cal_src->k_fta_danger_zone_right_long[3];
        cal_dst->k_fta_danger_zone_right_long[4] = cal_src->k_fta_danger_zone_right_long[4];
        cal_dst->k_fta_danger_zone_right_long[5] = cal_src->k_fta_danger_zone_right_long[5];
        cal_dst->k_fta_danger_zone_right_long[6] = cal_src->k_fta_danger_zone_right_long[6];
        cal_dst->k_fta_danger_zone_right_lat[0] = cal_src->k_fta_danger_zone_right_lat[0];
        cal_dst->k_fta_danger_zone_right_lat[1] = cal_src->k_fta_danger_zone_right_lat[1];
        cal_dst->k_fta_danger_zone_right_lat[2] = cal_src->k_fta_danger_zone_right_lat[2];
        cal_dst->k_fta_danger_zone_right_lat[3] = cal_src->k_fta_danger_zone_right_lat[3];
        cal_dst->k_fta_danger_zone_right_lat[4] = cal_src->k_fta_danger_zone_right_lat[4];
        cal_dst->k_fta_danger_zone_right_lat[5] = cal_src->k_fta_danger_zone_right_lat[5];
        cal_dst->k_fta_danger_zone_right_lat[6] = cal_src->k_fta_danger_zone_right_lat[6];
        cal_dst->k_pfgs_ego_speed[0] = cal_src->k_pfgs_ego_speed[0];
        cal_dst->k_pfgs_ego_speed[1] = cal_src->k_pfgs_ego_speed[1];
        cal_dst->k_pfgs_qualification_ttc_min = cal_src->k_pfgs_qualification_ttc_min;
        cal_dst->k_tap_lvl_2_host_curvature_min = cal_src->k_tap_lvl_2_host_curvature_min;
        cal_dst->k_rta_obj_exist_prblty[0] = cal_src->k_rta_obj_exist_prblty[0];
        cal_dst->k_rta_obj_exist_prblty[1] = cal_src->k_rta_obj_exist_prblty[1];
        cal_dst->k_rta_obj_exist_prblty_ofst[0] = cal_src->k_rta_obj_exist_prblty_ofst[0];
        cal_dst->k_rta_obj_exist_prblty_ofst[1] = cal_src->k_rta_obj_exist_prblty_ofst[1];
        cal_dst->k_rta_obj_vcs_long_vel_rel[0] = cal_src->k_rta_obj_vcs_long_vel_rel[0];
        cal_dst->k_rta_obj_vcs_long_vel_rel[1] = cal_src->k_rta_obj_vcs_long_vel_rel[1];
        cal_dst->k_rta_obj_vcs_long_vel_rel_ofst[0] = cal_src->k_rta_obj_vcs_long_vel_rel_ofst[0];
        cal_dst->k_rta_obj_vcs_long_vel_rel_ofst[1] = cal_src->k_rta_obj_vcs_long_vel_rel_ofst[1];
        cal_dst->k_rta_obj_vcs_lat_vel_rel[0] = cal_src->k_rta_obj_vcs_lat_vel_rel[0];
        cal_dst->k_rta_obj_vcs_lat_vel_rel[1] = cal_src->k_rta_obj_vcs_lat_vel_rel[1];
        cal_dst->k_rta_obj_vcs_lat_vel_rel_ofst[0] = cal_src->k_rta_obj_vcs_lat_vel_rel_ofst[0];
        cal_dst->k_rta_obj_vcs_lat_vel_rel_ofst[1] = cal_src->k_rta_obj_vcs_lat_vel_rel_ofst[1];
        cal_dst->k_rta_obj_vcs_long_vel[0] = cal_src->k_rta_obj_vcs_long_vel[0];
        cal_dst->k_rta_obj_vcs_long_vel[1] = cal_src->k_rta_obj_vcs_long_vel[1];
        cal_dst->k_rta_obj_vcs_long_vel_ofst[0] = cal_src->k_rta_obj_vcs_long_vel_ofst[0];
        cal_dst->k_rta_obj_vcs_long_vel_ofst[1] = cal_src->k_rta_obj_vcs_long_vel_ofst[1];
        cal_dst->k_rta_obj_vcs_lat_vel[0] = cal_src->k_rta_obj_vcs_lat_vel[0];
        cal_dst->k_rta_obj_vcs_lat_vel[1] = cal_src->k_rta_obj_vcs_lat_vel[1];
        cal_dst->k_rta_obj_vcs_lat_vel_ofst[0] = cal_src->k_rta_obj_vcs_lat_vel_ofst[0];
        cal_dst->k_rta_obj_vcs_lat_vel_ofst[1] = cal_src->k_rta_obj_vcs_lat_vel_ofst[1];
        cal_dst->k_rta_obj_heading[0] = cal_src->k_rta_obj_heading[0];
        cal_dst->k_rta_obj_heading[1] = cal_src->k_rta_obj_heading[1];
        cal_dst->k_rta_obj_heading_ofst[0] = cal_src->k_rta_obj_heading_ofst[0];
        cal_dst->k_rta_obj_heading_ofst[1] = cal_src->k_rta_obj_heading_ofst[1];
        cal_dst->k_rta_obj_speed[0] = cal_src->k_rta_obj_speed[0];
        cal_dst->k_rta_obj_speed[1] = cal_src->k_rta_obj_speed[1];
        cal_dst->k_rta_obj_speed_ofst[0] = cal_src->k_rta_obj_speed_ofst[0];
        cal_dst->k_rta_obj_speed_ofst[1] = cal_src->k_rta_obj_speed_ofst[1];
        cal_dst->k_rta_obj_length[0] = cal_src->k_rta_obj_length[0];
        cal_dst->k_rta_obj_length[1] = cal_src->k_rta_obj_length[1];
        cal_dst->k_rta_obj_length_ofst[0] = cal_src->k_rta_obj_length_ofst[0];
        cal_dst->k_rta_obj_length_ofst[1] = cal_src->k_rta_obj_length_ofst[1];
        cal_dst->k_rta_obj_width[0] = cal_src->k_rta_obj_width[0];
        cal_dst->k_rta_obj_width[1] = cal_src->k_rta_obj_width[1];
        cal_dst->k_rta_obj_width_ofst[0] = cal_src->k_rta_obj_width_ofst[0];
        cal_dst->k_rta_obj_width_ofst[1] = cal_src->k_rta_obj_width_ofst[1];
        cal_dst->k_rta_obj_vru_class_prob[0] = cal_src->k_rta_obj_vru_class_prob[0];
        cal_dst->k_rta_obj_vru_class_prob[1] = cal_src->k_rta_obj_vru_class_prob[1];
        cal_dst->k_rta_obj_vru_class_prob_ofst[0] = cal_src->k_rta_obj_vru_class_prob_ofst[0];
        cal_dst->k_rta_obj_vru_class_prob_ofst[1] = cal_src->k_rta_obj_vru_class_prob_ofst[1];
        cal_dst->k_rta_obj_eclipse_value[0] = cal_src->k_rta_obj_eclipse_value[0];
        cal_dst->k_rta_obj_eclipse_value[1] = cal_src->k_rta_obj_eclipse_value[1];
        cal_dst->k_rta_obj_eclipse_value_ofst[0] = cal_src->k_rta_obj_eclipse_value_ofst[0];
        cal_dst->k_rta_obj_eclipse_value_ofst[1] = cal_src->k_rta_obj_eclipse_value_ofst[1];
        cal_dst->k_rta_ttp_obj_abs_lat_vel_rel_max = cal_src->k_rta_ttp_obj_abs_lat_vel_rel_max;
        cal_dst->k_rta_ttp_obj_abs_heading_diff_max = cal_src->k_rta_ttp_obj_abs_heading_diff_max;
        cal_dst->k_rta_ttp_curve_suppression_obj_distance_min = cal_src->k_rta_ttp_curve_suppression_obj_distance_min;
        cal_dst->k_rta_info_zone_left_long[0] = cal_src->k_rta_info_zone_left_long[0];
        cal_dst->k_rta_info_zone_left_long[1] = cal_src->k_rta_info_zone_left_long[1];
        cal_dst->k_rta_info_zone_left_long[2] = cal_src->k_rta_info_zone_left_long[2];
        cal_dst->k_rta_info_zone_left_long[3] = cal_src->k_rta_info_zone_left_long[3];
        cal_dst->k_rta_info_zone_left_lat[0] = cal_src->k_rta_info_zone_left_lat[0];
        cal_dst->k_rta_info_zone_left_lat[1] = cal_src->k_rta_info_zone_left_lat[1];
        cal_dst->k_rta_info_zone_left_lat[2] = cal_src->k_rta_info_zone_left_lat[2];
        cal_dst->k_rta_info_zone_left_lat[3] = cal_src->k_rta_info_zone_left_lat[3];
        cal_dst->k_rta_info_zone_right_long[0] = cal_src->k_rta_info_zone_right_long[0];
        cal_dst->k_rta_info_zone_right_long[1] = cal_src->k_rta_info_zone_right_long[1];
        cal_dst->k_rta_info_zone_right_long[2] = cal_src->k_rta_info_zone_right_long[2];
        cal_dst->k_rta_info_zone_right_long[3] = cal_src->k_rta_info_zone_right_long[3];
        cal_dst->k_rta_info_zone_right_lat[0] = cal_src->k_rta_info_zone_right_lat[0];
        cal_dst->k_rta_info_zone_right_lat[1] = cal_src->k_rta_info_zone_right_lat[1];
        cal_dst->k_rta_info_zone_right_lat[2] = cal_src->k_rta_info_zone_right_lat[2];
        cal_dst->k_rta_info_zone_right_lat[3] = cal_src->k_rta_info_zone_right_lat[3];
        cal_dst->k_rta_info_zone_left_long_hys[0] = cal_src->k_rta_info_zone_left_long_hys[0];
        cal_dst->k_rta_info_zone_left_long_hys[1] = cal_src->k_rta_info_zone_left_long_hys[1];
        cal_dst->k_rta_info_zone_left_long_hys[2] = cal_src->k_rta_info_zone_left_long_hys[2];
        cal_dst->k_rta_info_zone_left_long_hys[3] = cal_src->k_rta_info_zone_left_long_hys[3];
        cal_dst->k_rta_info_zone_left_lat_hys[0] = cal_src->k_rta_info_zone_left_lat_hys[0];
        cal_dst->k_rta_info_zone_left_lat_hys[1] = cal_src->k_rta_info_zone_left_lat_hys[1];
        cal_dst->k_rta_info_zone_left_lat_hys[2] = cal_src->k_rta_info_zone_left_lat_hys[2];
        cal_dst->k_rta_info_zone_left_lat_hys[3] = cal_src->k_rta_info_zone_left_lat_hys[3];
        cal_dst->k_rta_info_zone_right_long_hys[0] = cal_src->k_rta_info_zone_right_long_hys[0];
        cal_dst->k_rta_info_zone_right_long_hys[1] = cal_src->k_rta_info_zone_right_long_hys[1];
        cal_dst->k_rta_info_zone_right_long_hys[2] = cal_src->k_rta_info_zone_right_long_hys[2];
        cal_dst->k_rta_info_zone_right_long_hys[3] = cal_src->k_rta_info_zone_right_long_hys[3];
        cal_dst->k_rta_info_zone_right_lat_hys[0] = cal_src->k_rta_info_zone_right_lat_hys[0];
        cal_dst->k_rta_info_zone_right_lat_hys[1] = cal_src->k_rta_info_zone_right_lat_hys[1];
        cal_dst->k_rta_info_zone_right_lat_hys[2] = cal_src->k_rta_info_zone_right_lat_hys[2];
        cal_dst->k_rta_info_zone_right_lat_hys[3] = cal_src->k_rta_info_zone_right_lat_hys[3];
        cal_dst->k_rta_wing_zone_left_long[0] = cal_src->k_rta_wing_zone_left_long[0];
        cal_dst->k_rta_wing_zone_left_long[1] = cal_src->k_rta_wing_zone_left_long[1];
        cal_dst->k_rta_wing_zone_left_long[2] = cal_src->k_rta_wing_zone_left_long[2];
        cal_dst->k_rta_wing_zone_left_long[3] = cal_src->k_rta_wing_zone_left_long[3];
        cal_dst->k_rta_wing_zone_left_lat[0] = cal_src->k_rta_wing_zone_left_lat[0];
        cal_dst->k_rta_wing_zone_left_lat[1] = cal_src->k_rta_wing_zone_left_lat[1];
        cal_dst->k_rta_wing_zone_left_lat[2] = cal_src->k_rta_wing_zone_left_lat[2];
        cal_dst->k_rta_wing_zone_left_lat[3] = cal_src->k_rta_wing_zone_left_lat[3];
        cal_dst->k_rta_wing_zone_right_long[0] = cal_src->k_rta_wing_zone_right_long[0];
        cal_dst->k_rta_wing_zone_right_long[1] = cal_src->k_rta_wing_zone_right_long[1];
        cal_dst->k_rta_wing_zone_right_long[2] = cal_src->k_rta_wing_zone_right_long[2];
        cal_dst->k_rta_wing_zone_right_long[3] = cal_src->k_rta_wing_zone_right_long[3];
        cal_dst->k_rta_wing_zone_right_lat[0] = cal_src->k_rta_wing_zone_right_lat[0];
        cal_dst->k_rta_wing_zone_right_lat[1] = cal_src->k_rta_wing_zone_right_lat[1];
        cal_dst->k_rta_wing_zone_right_lat[2] = cal_src->k_rta_wing_zone_right_lat[2];
        cal_dst->k_rta_wing_zone_right_lat[3] = cal_src->k_rta_wing_zone_right_lat[3];
        cal_dst->k_rta_wing_zone_left_long_hys[0] = cal_src->k_rta_wing_zone_left_long_hys[0];
        cal_dst->k_rta_wing_zone_left_long_hys[1] = cal_src->k_rta_wing_zone_left_long_hys[1];
        cal_dst->k_rta_wing_zone_left_long_hys[2] = cal_src->k_rta_wing_zone_left_long_hys[2];
        cal_dst->k_rta_wing_zone_left_long_hys[3] = cal_src->k_rta_wing_zone_left_long_hys[3];
        cal_dst->k_rta_wing_zone_left_lat_hys[0] = cal_src->k_rta_wing_zone_left_lat_hys[0];
        cal_dst->k_rta_wing_zone_left_lat_hys[1] = cal_src->k_rta_wing_zone_left_lat_hys[1];
        cal_dst->k_rta_wing_zone_left_lat_hys[2] = cal_src->k_rta_wing_zone_left_lat_hys[2];
        cal_dst->k_rta_wing_zone_left_lat_hys[3] = cal_src->k_rta_wing_zone_left_lat_hys[3];
        cal_dst->k_rta_wing_zone_right_long_hys[0] = cal_src->k_rta_wing_zone_right_long_hys[0];
        cal_dst->k_rta_wing_zone_right_long_hys[1] = cal_src->k_rta_wing_zone_right_long_hys[1];
        cal_dst->k_rta_wing_zone_right_long_hys[2] = cal_src->k_rta_wing_zone_right_long_hys[2];
        cal_dst->k_rta_wing_zone_right_long_hys[3] = cal_src->k_rta_wing_zone_right_long_hys[3];
        cal_dst->k_rta_wing_zone_right_lat_hys[0] = cal_src->k_rta_wing_zone_right_lat_hys[0];
        cal_dst->k_rta_wing_zone_right_lat_hys[1] = cal_src->k_rta_wing_zone_right_lat_hys[1];
        cal_dst->k_rta_wing_zone_right_lat_hys[2] = cal_src->k_rta_wing_zone_right_lat_hys[2];
        cal_dst->k_rta_wing_zone_right_lat_hys[3] = cal_src->k_rta_wing_zone_right_lat_hys[3];
        cal_dst->k_ta_always_overwrite_ta_mode_to_both = cal_src->k_ta_always_overwrite_ta_mode_to_both;
        cal_dst->k_ta_f_only_allow_consecutive_ttc_based_alert_levels = cal_src->k_ta_f_only_allow_consecutive_ttc_based_alert_levels;
        cal_dst->k_ta_f_skip_holding_for_single_alert_level_drop = cal_src->k_ta_f_skip_holding_for_single_alert_level_drop;
        cal_dst->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj = cal_src->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj;
        cal_dst->k_ta_f_apply_ttp_hysteresis_globally = cal_src->k_ta_f_apply_ttp_hysteresis_globally;
        cal_dst->k_f_ta_enable_debug_mode = cal_src->k_f_ta_enable_debug_mode;
        cal_dst->k_f_fta_enable = cal_src->k_f_fta_enable;
        cal_dst->k_f_fta_enable_brake_gradient_logic = cal_src->k_f_fta_enable_brake_gradient_logic;
        cal_dst->k_f_fta_enable_danger_zones = cal_src->k_f_fta_enable_danger_zones;
        cal_dst->k_pfgs_symbol_request_sides_enabled = cal_src->k_pfgs_symbol_request_sides_enabled;
        cal_dst->k_pfgs_qualification_check_f_stationary = cal_src->k_pfgs_qualification_check_f_stationary;
        cal_dst->k_f_rta_enable = cal_src->k_f_rta_enable;
        cal_dst->k_f_rta_enable_info_zones = cal_src->k_f_rta_enable_info_zones;
        cal_dst->k_rta_f_higher_obj_crit_based_on_lower_ttp = cal_src->k_rta_f_higher_obj_crit_based_on_lower_ttp;
        cal_dst->k_f_rta_enable_wing_zones = cal_src->k_f_rta_enable_wing_zones;
        cal_dst->k_ta_alert_qualifying_cycles = cal_src->k_ta_alert_qualifying_cycles;
        cal_dst->k_ta_alert_holding_cycles = cal_src->k_ta_alert_holding_cycles;
        cal_dst->k_ta_prediction_steps_max = cal_src->k_ta_prediction_steps_max;
        cal_dst->k_ta_critical_approach_check_ego_circles[0] = cal_src->k_ta_critical_approach_check_ego_circles[0];
        cal_dst->k_ta_critical_approach_check_ego_circles[1] = cal_src->k_ta_critical_approach_check_ego_circles[1];
        cal_dst->k_ta_critical_approach_check_ego_circles[2] = cal_src->k_ta_critical_approach_check_ego_circles[2];
        cal_dst->k_ta_ego_pred_const_velocity_pred_steps_min = cal_src->k_ta_ego_pred_const_velocity_pred_steps_min;
        cal_dst->k_fta_obj_age_min = cal_src->k_fta_obj_age_min;
        cal_dst->k_fta_danger_zone_point_size = cal_src->k_fta_danger_zone_point_size;
        cal_dst->k_pfgs_qualification_counter_fast_obj = cal_src->k_pfgs_qualification_counter_fast_obj;
        cal_dst->k_pfgs_qualification_counter_slow_obj = cal_src->k_pfgs_qualification_counter_slow_obj;
        cal_dst->k_rta_obj_age_min = cal_src->k_rta_obj_age_min;
        cal_dst->k_rta_info_zone_point_size = cal_src->k_rta_info_zone_point_size;
        cal_dst->k_rta_wing_zone_point_size = cal_src->k_rta_wing_zone_point_size;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ta_Update_Core_Cal_By_Core(Ta_Core_Calibration_T* cal_dst, const Ta_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ta_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_ta_alert_lvl_1_ttp_threshold[0] = cal_src->k_ta_alert_lvl_1_ttp_threshold[0];
        cal_dst->k_ta_alert_lvl_1_ttp_threshold[1] = cal_src->k_ta_alert_lvl_1_ttp_threshold[1];
        cal_dst->k_ta_alert_lvl_1_ttp_threshold[2] = cal_src->k_ta_alert_lvl_1_ttp_threshold[2];
        cal_dst->k_ta_active_obj_ttp_offset = cal_src->k_ta_active_obj_ttp_offset;
        cal_dst->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max = cal_src->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max;
        cal_dst->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max = cal_src->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max;
        cal_dst->k_ta_alert_lvl_2_ttc_threshold = cal_src->k_ta_alert_lvl_2_ttc_threshold;
        cal_dst->k_ta_alert_lvl_3_ttc_threshold = cal_src->k_ta_alert_lvl_3_ttc_threshold;
        cal_dst->k_ta_alert_lvl_3_ttb_threshold = cal_src->k_ta_alert_lvl_3_ttb_threshold;
        cal_dst->k_ta_alert_lvl_4_ttc_threshold = cal_src->k_ta_alert_lvl_4_ttc_threshold;
        cal_dst->k_ta_alert_lvl_4_decel_threshold = cal_src->k_ta_alert_lvl_4_decel_threshold;
        cal_dst->k_ta_critical_approach_min_safe_distance = cal_src->k_ta_critical_approach_min_safe_distance;
        cal_dst->k_ta_critical_approach_angle_diff_min = cal_src->k_ta_critical_approach_angle_diff_min;
        cal_dst->k_ta_ego_max_pred_yaw_angle = cal_src->k_ta_ego_max_pred_yaw_angle;
        cal_dst->k_ta_obj_pred_speed_min = cal_src->k_ta_obj_pred_speed_min;
        cal_dst->k_ta_obj_acceleration_long_weight = cal_src->k_ta_obj_acceleration_long_weight;
        cal_dst->k_ta_obj_acceleration_lat_weight = cal_src->k_ta_obj_acceleration_lat_weight;
        cal_dst->k_ta_ego_shape_gain_per_pred_step = cal_src->k_ta_ego_shape_gain_per_pred_step;
        cal_dst->k_ta_obj_shape_gain_per_pred_step = cal_src->k_ta_obj_shape_gain_per_pred_step;
        cal_dst->k_ta_ego_shape_gain_fixed = cal_src->k_ta_ego_shape_gain_fixed;
        cal_dst->k_ta_obj_shape_gain_fixed = cal_src->k_ta_obj_shape_gain_fixed;
        cal_dst->k_ta_ego_speed[0] = cal_src->k_ta_ego_speed[0];
        cal_dst->k_ta_ego_speed[1] = cal_src->k_ta_ego_speed[1];
        cal_dst->k_ta_ego_speed_ofst[0] = cal_src->k_ta_ego_speed_ofst[0];
        cal_dst->k_ta_ego_speed_ofst[1] = cal_src->k_ta_ego_speed_ofst[1];
        cal_dst->k_ta_ego_yawrate[0] = cal_src->k_ta_ego_yawrate[0];
        cal_dst->k_ta_ego_yawrate[1] = cal_src->k_ta_ego_yawrate[1];
        cal_dst->k_ta_ego_yawrate_ofst[0] = cal_src->k_ta_ego_yawrate_ofst[0];
        cal_dst->k_ta_ego_yawrate_ofst[1] = cal_src->k_ta_ego_yawrate_ofst[1];
        cal_dst->k_ta_ego_long_acceleration[0] = cal_src->k_ta_ego_long_acceleration[0];
        cal_dst->k_ta_ego_long_acceleration[1] = cal_src->k_ta_ego_long_acceleration[1];
        cal_dst->k_ta_ego_long_acceleration_ofst[0] = cal_src->k_ta_ego_long_acceleration_ofst[0];
        cal_dst->k_ta_ego_long_acceleration_ofst[1] = cal_src->k_ta_ego_long_acceleration_ofst[1];
        cal_dst->k_ta_ego_acceleration_weight = cal_src->k_ta_ego_acceleration_weight;
        cal_dst->k_ta_ego_deceleration_weight = cal_src->k_ta_ego_deceleration_weight;
        cal_dst->k_ta_ego_circle_offset = cal_src->k_ta_ego_circle_offset;
        cal_dst->k_ta_ego_circle_host_length_factor = cal_src->k_ta_ego_circle_host_length_factor;
        cal_dst->k_ta_ego_yawangle_integration_yawrate_min = cal_src->k_ta_ego_yawangle_integration_yawrate_min;
        cal_dst->k_fta_obj_exist_prblty[0] = cal_src->k_fta_obj_exist_prblty[0];
        cal_dst->k_fta_obj_exist_prblty[1] = cal_src->k_fta_obj_exist_prblty[1];
        cal_dst->k_fta_obj_exist_prblty_ofst[0] = cal_src->k_fta_obj_exist_prblty_ofst[0];
        cal_dst->k_fta_obj_exist_prblty_ofst[1] = cal_src->k_fta_obj_exist_prblty_ofst[1];
        cal_dst->k_fta_obj_vcs_long_vel_rel[0] = cal_src->k_fta_obj_vcs_long_vel_rel[0];
        cal_dst->k_fta_obj_vcs_long_vel_rel[1] = cal_src->k_fta_obj_vcs_long_vel_rel[1];
        cal_dst->k_fta_obj_vcs_long_vel_rel_ofst[0] = cal_src->k_fta_obj_vcs_long_vel_rel_ofst[0];
        cal_dst->k_fta_obj_vcs_long_vel_rel_ofst[1] = cal_src->k_fta_obj_vcs_long_vel_rel_ofst[1];
        cal_dst->k_fta_obj_vcs_lat_vel_rel[0] = cal_src->k_fta_obj_vcs_lat_vel_rel[0];
        cal_dst->k_fta_obj_vcs_lat_vel_rel[1] = cal_src->k_fta_obj_vcs_lat_vel_rel[1];
        cal_dst->k_fta_obj_vcs_lat_vel_rel_ofst[0] = cal_src->k_fta_obj_vcs_lat_vel_rel_ofst[0];
        cal_dst->k_fta_obj_vcs_lat_vel_rel_ofst[1] = cal_src->k_fta_obj_vcs_lat_vel_rel_ofst[1];
        cal_dst->k_fta_obj_vcs_long_vel[0] = cal_src->k_fta_obj_vcs_long_vel[0];
        cal_dst->k_fta_obj_vcs_long_vel[1] = cal_src->k_fta_obj_vcs_long_vel[1];
        cal_dst->k_fta_obj_vcs_long_vel_ofst[0] = cal_src->k_fta_obj_vcs_long_vel_ofst[0];
        cal_dst->k_fta_obj_vcs_long_vel_ofst[1] = cal_src->k_fta_obj_vcs_long_vel_ofst[1];
        cal_dst->k_fta_obj_vcs_lat_vel[0] = cal_src->k_fta_obj_vcs_lat_vel[0];
        cal_dst->k_fta_obj_vcs_lat_vel[1] = cal_src->k_fta_obj_vcs_lat_vel[1];
        cal_dst->k_fta_obj_vcs_lat_vel_ofst[0] = cal_src->k_fta_obj_vcs_lat_vel_ofst[0];
        cal_dst->k_fta_obj_vcs_lat_vel_ofst[1] = cal_src->k_fta_obj_vcs_lat_vel_ofst[1];
        cal_dst->k_ta_straight_host_curvature_max = cal_src->k_ta_straight_host_curvature_max;
        cal_dst->k_ta_lookup_turning_host_speed[0] = cal_src->k_ta_lookup_turning_host_speed[0];
        cal_dst->k_ta_lookup_turning_host_speed[1] = cal_src->k_ta_lookup_turning_host_speed[1];
        cal_dst->k_ta_lookup_turning_host_speed[2] = cal_src->k_ta_lookup_turning_host_speed[2];
        cal_dst->k_ta_lookup_turning_host_speed[3] = cal_src->k_ta_lookup_turning_host_speed[3];
        cal_dst->k_ta_lookup_turning_host_curvature_min[0] = cal_src->k_ta_lookup_turning_host_curvature_min[0];
        cal_dst->k_ta_lookup_turning_host_curvature_min[1] = cal_src->k_ta_lookup_turning_host_curvature_min[1];
        cal_dst->k_ta_lookup_turning_host_curvature_min[2] = cal_src->k_ta_lookup_turning_host_curvature_min[2];
        cal_dst->k_ta_lookup_turning_host_curvature_min[3] = cal_src->k_ta_lookup_turning_host_curvature_min[3];
        cal_dst->k_fta_obj_vcs_long_pos_straight_min = cal_src->k_fta_obj_vcs_long_pos_straight_min;
        cal_dst->k_fta_obj_heading[0] = cal_src->k_fta_obj_heading[0];
        cal_dst->k_fta_obj_heading[1] = cal_src->k_fta_obj_heading[1];
        cal_dst->k_fta_obj_heading_straight[0] = cal_src->k_fta_obj_heading_straight[0];
        cal_dst->k_fta_obj_heading_straight[1] = cal_src->k_fta_obj_heading_straight[1];
        cal_dst->k_fta_obj_heading_ofst[0] = cal_src->k_fta_obj_heading_ofst[0];
        cal_dst->k_fta_obj_heading_ofst[1] = cal_src->k_fta_obj_heading_ofst[1];
        cal_dst->k_fta_obj_heading_rate[0] = cal_src->k_fta_obj_heading_rate[0];
        cal_dst->k_fta_obj_heading_rate[1] = cal_src->k_fta_obj_heading_rate[1];
        cal_dst->k_fta_obj_heading_rate_straight[0] = cal_src->k_fta_obj_heading_rate_straight[0];
        cal_dst->k_fta_obj_heading_rate_straight[1] = cal_src->k_fta_obj_heading_rate_straight[1];
        cal_dst->k_fta_obj_heading_rate_ofst[0] = cal_src->k_fta_obj_heading_rate_ofst[0];
        cal_dst->k_fta_obj_heading_rate_ofst[1] = cal_src->k_fta_obj_heading_rate_ofst[1];
        cal_dst->k_fta_obj_speed[0] = cal_src->k_fta_obj_speed[0];
        cal_dst->k_fta_obj_speed[1] = cal_src->k_fta_obj_speed[1];
        cal_dst->k_fta_obj_speed_straight[0] = cal_src->k_fta_obj_speed_straight[0];
        cal_dst->k_fta_obj_speed_straight[1] = cal_src->k_fta_obj_speed_straight[1];
        cal_dst->k_fta_obj_speed_ofst[0] = cal_src->k_fta_obj_speed_ofst[0];
        cal_dst->k_fta_obj_speed_ofst[1] = cal_src->k_fta_obj_speed_ofst[1];
        cal_dst->k_fta_obj_length[0] = cal_src->k_fta_obj_length[0];
        cal_dst->k_fta_obj_length[1] = cal_src->k_fta_obj_length[1];
        cal_dst->k_fta_obj_length_ofst[0] = cal_src->k_fta_obj_length_ofst[0];
        cal_dst->k_fta_obj_length_ofst[1] = cal_src->k_fta_obj_length_ofst[1];
        cal_dst->k_fta_obj_width[0] = cal_src->k_fta_obj_width[0];
        cal_dst->k_fta_obj_width[1] = cal_src->k_fta_obj_width[1];
        cal_dst->k_fta_obj_width_ofst[0] = cal_src->k_fta_obj_width_ofst[0];
        cal_dst->k_fta_obj_width_ofst[1] = cal_src->k_fta_obj_width_ofst[1];
        cal_dst->k_fta_obj_area[0] = cal_src->k_fta_obj_area[0];
        cal_dst->k_fta_obj_area[1] = cal_src->k_fta_obj_area[1];
        cal_dst->k_fta_obj_area_ofst[0] = cal_src->k_fta_obj_area_ofst[0];
        cal_dst->k_fta_obj_area_ofst[1] = cal_src->k_fta_obj_area_ofst[1];
        cal_dst->k_fta_obj_vru_class_prob[0] = cal_src->k_fta_obj_vru_class_prob[0];
        cal_dst->k_fta_obj_vru_class_prob[1] = cal_src->k_fta_obj_vru_class_prob[1];
        cal_dst->k_fta_obj_vru_class_prob_ofst[0] = cal_src->k_fta_obj_vru_class_prob_ofst[0];
        cal_dst->k_fta_obj_vru_class_prob_ofst[1] = cal_src->k_fta_obj_vru_class_prob_ofst[1];
        cal_dst->k_fta_ego_obj_heading_diff[0] = cal_src->k_fta_ego_obj_heading_diff[0];
        cal_dst->k_fta_ego_obj_heading_diff[1] = cal_src->k_fta_ego_obj_heading_diff[1];
        cal_dst->k_fta_ego_obj_heading_diff_ofst[0] = cal_src->k_fta_ego_obj_heading_diff_ofst[0];
        cal_dst->k_fta_ego_obj_heading_diff_ofst[1] = cal_src->k_fta_ego_obj_heading_diff_ofst[1];
        cal_dst->k_fta_obj_eclipse_value[0] = cal_src->k_fta_obj_eclipse_value[0];
        cal_dst->k_fta_obj_eclipse_value[1] = cal_src->k_fta_obj_eclipse_value[1];
        cal_dst->k_fta_obj_eclipse_value_ofst[0] = cal_src->k_fta_obj_eclipse_value_ofst[0];
        cal_dst->k_fta_obj_eclipse_value_ofst[1] = cal_src->k_fta_obj_eclipse_value_ofst[1];
        cal_dst->k_fta_obj_velocity_heading_diff_max = cal_src->k_fta_obj_velocity_heading_diff_max;
        cal_dst->k_fta_brake_deceleration_max = cal_src->k_fta_brake_deceleration_max;
        cal_dst->k_fta_brake_dead_time = cal_src->k_fta_brake_dead_time;
        cal_dst->k_fta_brake_gradient = cal_src->k_fta_brake_gradient;
        cal_dst->k_fta_danger_zone_left_long[0] = cal_src->k_fta_danger_zone_left_long[0];
        cal_dst->k_fta_danger_zone_left_long[1] = cal_src->k_fta_danger_zone_left_long[1];
        cal_dst->k_fta_danger_zone_left_long[2] = cal_src->k_fta_danger_zone_left_long[2];
        cal_dst->k_fta_danger_zone_left_long[3] = cal_src->k_fta_danger_zone_left_long[3];
        cal_dst->k_fta_danger_zone_left_long[4] = cal_src->k_fta_danger_zone_left_long[4];
        cal_dst->k_fta_danger_zone_left_long[5] = cal_src->k_fta_danger_zone_left_long[5];
        cal_dst->k_fta_danger_zone_left_long[6] = cal_src->k_fta_danger_zone_left_long[6];
        cal_dst->k_fta_danger_zone_left_lat[0] = cal_src->k_fta_danger_zone_left_lat[0];
        cal_dst->k_fta_danger_zone_left_lat[1] = cal_src->k_fta_danger_zone_left_lat[1];
        cal_dst->k_fta_danger_zone_left_lat[2] = cal_src->k_fta_danger_zone_left_lat[2];
        cal_dst->k_fta_danger_zone_left_lat[3] = cal_src->k_fta_danger_zone_left_lat[3];
        cal_dst->k_fta_danger_zone_left_lat[4] = cal_src->k_fta_danger_zone_left_lat[4];
        cal_dst->k_fta_danger_zone_left_lat[5] = cal_src->k_fta_danger_zone_left_lat[5];
        cal_dst->k_fta_danger_zone_left_lat[6] = cal_src->k_fta_danger_zone_left_lat[6];
        cal_dst->k_fta_danger_zone_right_long[0] = cal_src->k_fta_danger_zone_right_long[0];
        cal_dst->k_fta_danger_zone_right_long[1] = cal_src->k_fta_danger_zone_right_long[1];
        cal_dst->k_fta_danger_zone_right_long[2] = cal_src->k_fta_danger_zone_right_long[2];
        cal_dst->k_fta_danger_zone_right_long[3] = cal_src->k_fta_danger_zone_right_long[3];
        cal_dst->k_fta_danger_zone_right_long[4] = cal_src->k_fta_danger_zone_right_long[4];
        cal_dst->k_fta_danger_zone_right_long[5] = cal_src->k_fta_danger_zone_right_long[5];
        cal_dst->k_fta_danger_zone_right_long[6] = cal_src->k_fta_danger_zone_right_long[6];
        cal_dst->k_fta_danger_zone_right_lat[0] = cal_src->k_fta_danger_zone_right_lat[0];
        cal_dst->k_fta_danger_zone_right_lat[1] = cal_src->k_fta_danger_zone_right_lat[1];
        cal_dst->k_fta_danger_zone_right_lat[2] = cal_src->k_fta_danger_zone_right_lat[2];
        cal_dst->k_fta_danger_zone_right_lat[3] = cal_src->k_fta_danger_zone_right_lat[3];
        cal_dst->k_fta_danger_zone_right_lat[4] = cal_src->k_fta_danger_zone_right_lat[4];
        cal_dst->k_fta_danger_zone_right_lat[5] = cal_src->k_fta_danger_zone_right_lat[5];
        cal_dst->k_fta_danger_zone_right_lat[6] = cal_src->k_fta_danger_zone_right_lat[6];
        cal_dst->k_pfgs_ego_speed[0] = cal_src->k_pfgs_ego_speed[0];
        cal_dst->k_pfgs_ego_speed[1] = cal_src->k_pfgs_ego_speed[1];
        cal_dst->k_pfgs_qualification_ttc_min = cal_src->k_pfgs_qualification_ttc_min;
        cal_dst->k_tap_lvl_2_host_curvature_min = cal_src->k_tap_lvl_2_host_curvature_min;
        cal_dst->k_rta_obj_exist_prblty[0] = cal_src->k_rta_obj_exist_prblty[0];
        cal_dst->k_rta_obj_exist_prblty[1] = cal_src->k_rta_obj_exist_prblty[1];
        cal_dst->k_rta_obj_exist_prblty_ofst[0] = cal_src->k_rta_obj_exist_prblty_ofst[0];
        cal_dst->k_rta_obj_exist_prblty_ofst[1] = cal_src->k_rta_obj_exist_prblty_ofst[1];
        cal_dst->k_rta_obj_vcs_long_vel_rel[0] = cal_src->k_rta_obj_vcs_long_vel_rel[0];
        cal_dst->k_rta_obj_vcs_long_vel_rel[1] = cal_src->k_rta_obj_vcs_long_vel_rel[1];
        cal_dst->k_rta_obj_vcs_long_vel_rel_ofst[0] = cal_src->k_rta_obj_vcs_long_vel_rel_ofst[0];
        cal_dst->k_rta_obj_vcs_long_vel_rel_ofst[1] = cal_src->k_rta_obj_vcs_long_vel_rel_ofst[1];
        cal_dst->k_rta_obj_vcs_lat_vel_rel[0] = cal_src->k_rta_obj_vcs_lat_vel_rel[0];
        cal_dst->k_rta_obj_vcs_lat_vel_rel[1] = cal_src->k_rta_obj_vcs_lat_vel_rel[1];
        cal_dst->k_rta_obj_vcs_lat_vel_rel_ofst[0] = cal_src->k_rta_obj_vcs_lat_vel_rel_ofst[0];
        cal_dst->k_rta_obj_vcs_lat_vel_rel_ofst[1] = cal_src->k_rta_obj_vcs_lat_vel_rel_ofst[1];
        cal_dst->k_rta_obj_vcs_long_vel[0] = cal_src->k_rta_obj_vcs_long_vel[0];
        cal_dst->k_rta_obj_vcs_long_vel[1] = cal_src->k_rta_obj_vcs_long_vel[1];
        cal_dst->k_rta_obj_vcs_long_vel_ofst[0] = cal_src->k_rta_obj_vcs_long_vel_ofst[0];
        cal_dst->k_rta_obj_vcs_long_vel_ofst[1] = cal_src->k_rta_obj_vcs_long_vel_ofst[1];
        cal_dst->k_rta_obj_vcs_lat_vel[0] = cal_src->k_rta_obj_vcs_lat_vel[0];
        cal_dst->k_rta_obj_vcs_lat_vel[1] = cal_src->k_rta_obj_vcs_lat_vel[1];
        cal_dst->k_rta_obj_vcs_lat_vel_ofst[0] = cal_src->k_rta_obj_vcs_lat_vel_ofst[0];
        cal_dst->k_rta_obj_vcs_lat_vel_ofst[1] = cal_src->k_rta_obj_vcs_lat_vel_ofst[1];
        cal_dst->k_rta_obj_heading[0] = cal_src->k_rta_obj_heading[0];
        cal_dst->k_rta_obj_heading[1] = cal_src->k_rta_obj_heading[1];
        cal_dst->k_rta_obj_heading_ofst[0] = cal_src->k_rta_obj_heading_ofst[0];
        cal_dst->k_rta_obj_heading_ofst[1] = cal_src->k_rta_obj_heading_ofst[1];
        cal_dst->k_rta_obj_speed[0] = cal_src->k_rta_obj_speed[0];
        cal_dst->k_rta_obj_speed[1] = cal_src->k_rta_obj_speed[1];
        cal_dst->k_rta_obj_speed_ofst[0] = cal_src->k_rta_obj_speed_ofst[0];
        cal_dst->k_rta_obj_speed_ofst[1] = cal_src->k_rta_obj_speed_ofst[1];
        cal_dst->k_rta_obj_length[0] = cal_src->k_rta_obj_length[0];
        cal_dst->k_rta_obj_length[1] = cal_src->k_rta_obj_length[1];
        cal_dst->k_rta_obj_length_ofst[0] = cal_src->k_rta_obj_length_ofst[0];
        cal_dst->k_rta_obj_length_ofst[1] = cal_src->k_rta_obj_length_ofst[1];
        cal_dst->k_rta_obj_width[0] = cal_src->k_rta_obj_width[0];
        cal_dst->k_rta_obj_width[1] = cal_src->k_rta_obj_width[1];
        cal_dst->k_rta_obj_width_ofst[0] = cal_src->k_rta_obj_width_ofst[0];
        cal_dst->k_rta_obj_width_ofst[1] = cal_src->k_rta_obj_width_ofst[1];
        cal_dst->k_rta_obj_vru_class_prob[0] = cal_src->k_rta_obj_vru_class_prob[0];
        cal_dst->k_rta_obj_vru_class_prob[1] = cal_src->k_rta_obj_vru_class_prob[1];
        cal_dst->k_rta_obj_vru_class_prob_ofst[0] = cal_src->k_rta_obj_vru_class_prob_ofst[0];
        cal_dst->k_rta_obj_vru_class_prob_ofst[1] = cal_src->k_rta_obj_vru_class_prob_ofst[1];
        cal_dst->k_rta_obj_eclipse_value[0] = cal_src->k_rta_obj_eclipse_value[0];
        cal_dst->k_rta_obj_eclipse_value[1] = cal_src->k_rta_obj_eclipse_value[1];
        cal_dst->k_rta_obj_eclipse_value_ofst[0] = cal_src->k_rta_obj_eclipse_value_ofst[0];
        cal_dst->k_rta_obj_eclipse_value_ofst[1] = cal_src->k_rta_obj_eclipse_value_ofst[1];
        cal_dst->k_rta_ttp_obj_abs_lat_vel_rel_max = cal_src->k_rta_ttp_obj_abs_lat_vel_rel_max;
        cal_dst->k_rta_ttp_obj_abs_heading_diff_max = cal_src->k_rta_ttp_obj_abs_heading_diff_max;
        cal_dst->k_rta_ttp_curve_suppression_obj_distance_min = cal_src->k_rta_ttp_curve_suppression_obj_distance_min;
        cal_dst->k_rta_info_zone_left_long[0] = cal_src->k_rta_info_zone_left_long[0];
        cal_dst->k_rta_info_zone_left_long[1] = cal_src->k_rta_info_zone_left_long[1];
        cal_dst->k_rta_info_zone_left_long[2] = cal_src->k_rta_info_zone_left_long[2];
        cal_dst->k_rta_info_zone_left_long[3] = cal_src->k_rta_info_zone_left_long[3];
        cal_dst->k_rta_info_zone_left_lat[0] = cal_src->k_rta_info_zone_left_lat[0];
        cal_dst->k_rta_info_zone_left_lat[1] = cal_src->k_rta_info_zone_left_lat[1];
        cal_dst->k_rta_info_zone_left_lat[2] = cal_src->k_rta_info_zone_left_lat[2];
        cal_dst->k_rta_info_zone_left_lat[3] = cal_src->k_rta_info_zone_left_lat[3];
        cal_dst->k_rta_info_zone_right_long[0] = cal_src->k_rta_info_zone_right_long[0];
        cal_dst->k_rta_info_zone_right_long[1] = cal_src->k_rta_info_zone_right_long[1];
        cal_dst->k_rta_info_zone_right_long[2] = cal_src->k_rta_info_zone_right_long[2];
        cal_dst->k_rta_info_zone_right_long[3] = cal_src->k_rta_info_zone_right_long[3];
        cal_dst->k_rta_info_zone_right_lat[0] = cal_src->k_rta_info_zone_right_lat[0];
        cal_dst->k_rta_info_zone_right_lat[1] = cal_src->k_rta_info_zone_right_lat[1];
        cal_dst->k_rta_info_zone_right_lat[2] = cal_src->k_rta_info_zone_right_lat[2];
        cal_dst->k_rta_info_zone_right_lat[3] = cal_src->k_rta_info_zone_right_lat[3];
        cal_dst->k_rta_info_zone_left_long_hys[0] = cal_src->k_rta_info_zone_left_long_hys[0];
        cal_dst->k_rta_info_zone_left_long_hys[1] = cal_src->k_rta_info_zone_left_long_hys[1];
        cal_dst->k_rta_info_zone_left_long_hys[2] = cal_src->k_rta_info_zone_left_long_hys[2];
        cal_dst->k_rta_info_zone_left_long_hys[3] = cal_src->k_rta_info_zone_left_long_hys[3];
        cal_dst->k_rta_info_zone_left_lat_hys[0] = cal_src->k_rta_info_zone_left_lat_hys[0];
        cal_dst->k_rta_info_zone_left_lat_hys[1] = cal_src->k_rta_info_zone_left_lat_hys[1];
        cal_dst->k_rta_info_zone_left_lat_hys[2] = cal_src->k_rta_info_zone_left_lat_hys[2];
        cal_dst->k_rta_info_zone_left_lat_hys[3] = cal_src->k_rta_info_zone_left_lat_hys[3];
        cal_dst->k_rta_info_zone_right_long_hys[0] = cal_src->k_rta_info_zone_right_long_hys[0];
        cal_dst->k_rta_info_zone_right_long_hys[1] = cal_src->k_rta_info_zone_right_long_hys[1];
        cal_dst->k_rta_info_zone_right_long_hys[2] = cal_src->k_rta_info_zone_right_long_hys[2];
        cal_dst->k_rta_info_zone_right_long_hys[3] = cal_src->k_rta_info_zone_right_long_hys[3];
        cal_dst->k_rta_info_zone_right_lat_hys[0] = cal_src->k_rta_info_zone_right_lat_hys[0];
        cal_dst->k_rta_info_zone_right_lat_hys[1] = cal_src->k_rta_info_zone_right_lat_hys[1];
        cal_dst->k_rta_info_zone_right_lat_hys[2] = cal_src->k_rta_info_zone_right_lat_hys[2];
        cal_dst->k_rta_info_zone_right_lat_hys[3] = cal_src->k_rta_info_zone_right_lat_hys[3];
        cal_dst->k_rta_wing_zone_left_long[0] = cal_src->k_rta_wing_zone_left_long[0];
        cal_dst->k_rta_wing_zone_left_long[1] = cal_src->k_rta_wing_zone_left_long[1];
        cal_dst->k_rta_wing_zone_left_long[2] = cal_src->k_rta_wing_zone_left_long[2];
        cal_dst->k_rta_wing_zone_left_long[3] = cal_src->k_rta_wing_zone_left_long[3];
        cal_dst->k_rta_wing_zone_left_lat[0] = cal_src->k_rta_wing_zone_left_lat[0];
        cal_dst->k_rta_wing_zone_left_lat[1] = cal_src->k_rta_wing_zone_left_lat[1];
        cal_dst->k_rta_wing_zone_left_lat[2] = cal_src->k_rta_wing_zone_left_lat[2];
        cal_dst->k_rta_wing_zone_left_lat[3] = cal_src->k_rta_wing_zone_left_lat[3];
        cal_dst->k_rta_wing_zone_right_long[0] = cal_src->k_rta_wing_zone_right_long[0];
        cal_dst->k_rta_wing_zone_right_long[1] = cal_src->k_rta_wing_zone_right_long[1];
        cal_dst->k_rta_wing_zone_right_long[2] = cal_src->k_rta_wing_zone_right_long[2];
        cal_dst->k_rta_wing_zone_right_long[3] = cal_src->k_rta_wing_zone_right_long[3];
        cal_dst->k_rta_wing_zone_right_lat[0] = cal_src->k_rta_wing_zone_right_lat[0];
        cal_dst->k_rta_wing_zone_right_lat[1] = cal_src->k_rta_wing_zone_right_lat[1];
        cal_dst->k_rta_wing_zone_right_lat[2] = cal_src->k_rta_wing_zone_right_lat[2];
        cal_dst->k_rta_wing_zone_right_lat[3] = cal_src->k_rta_wing_zone_right_lat[3];
        cal_dst->k_rta_wing_zone_left_long_hys[0] = cal_src->k_rta_wing_zone_left_long_hys[0];
        cal_dst->k_rta_wing_zone_left_long_hys[1] = cal_src->k_rta_wing_zone_left_long_hys[1];
        cal_dst->k_rta_wing_zone_left_long_hys[2] = cal_src->k_rta_wing_zone_left_long_hys[2];
        cal_dst->k_rta_wing_zone_left_long_hys[3] = cal_src->k_rta_wing_zone_left_long_hys[3];
        cal_dst->k_rta_wing_zone_left_lat_hys[0] = cal_src->k_rta_wing_zone_left_lat_hys[0];
        cal_dst->k_rta_wing_zone_left_lat_hys[1] = cal_src->k_rta_wing_zone_left_lat_hys[1];
        cal_dst->k_rta_wing_zone_left_lat_hys[2] = cal_src->k_rta_wing_zone_left_lat_hys[2];
        cal_dst->k_rta_wing_zone_left_lat_hys[3] = cal_src->k_rta_wing_zone_left_lat_hys[3];
        cal_dst->k_rta_wing_zone_right_long_hys[0] = cal_src->k_rta_wing_zone_right_long_hys[0];
        cal_dst->k_rta_wing_zone_right_long_hys[1] = cal_src->k_rta_wing_zone_right_long_hys[1];
        cal_dst->k_rta_wing_zone_right_long_hys[2] = cal_src->k_rta_wing_zone_right_long_hys[2];
        cal_dst->k_rta_wing_zone_right_long_hys[3] = cal_src->k_rta_wing_zone_right_long_hys[3];
        cal_dst->k_rta_wing_zone_right_lat_hys[0] = cal_src->k_rta_wing_zone_right_lat_hys[0];
        cal_dst->k_rta_wing_zone_right_lat_hys[1] = cal_src->k_rta_wing_zone_right_lat_hys[1];
        cal_dst->k_rta_wing_zone_right_lat_hys[2] = cal_src->k_rta_wing_zone_right_lat_hys[2];
        cal_dst->k_rta_wing_zone_right_lat_hys[3] = cal_src->k_rta_wing_zone_right_lat_hys[3];
        cal_dst->k_ta_always_overwrite_ta_mode_to_both = cal_src->k_ta_always_overwrite_ta_mode_to_both;
        cal_dst->k_ta_f_only_allow_consecutive_ttc_based_alert_levels = cal_src->k_ta_f_only_allow_consecutive_ttc_based_alert_levels;
        cal_dst->k_ta_f_skip_holding_for_single_alert_level_drop = cal_src->k_ta_f_skip_holding_for_single_alert_level_drop;
        cal_dst->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj = cal_src->k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj;
        cal_dst->k_ta_f_apply_ttp_hysteresis_globally = cal_src->k_ta_f_apply_ttp_hysteresis_globally;
        cal_dst->k_f_ta_enable_debug_mode = cal_src->k_f_ta_enable_debug_mode;
        cal_dst->k_f_fta_enable = cal_src->k_f_fta_enable;
        cal_dst->k_f_fta_enable_brake_gradient_logic = cal_src->k_f_fta_enable_brake_gradient_logic;
        cal_dst->k_f_fta_enable_danger_zones = cal_src->k_f_fta_enable_danger_zones;
        cal_dst->k_pfgs_symbol_request_sides_enabled = cal_src->k_pfgs_symbol_request_sides_enabled;
        cal_dst->k_pfgs_qualification_check_f_stationary = cal_src->k_pfgs_qualification_check_f_stationary;
        cal_dst->k_f_rta_enable = cal_src->k_f_rta_enable;
        cal_dst->k_f_rta_enable_info_zones = cal_src->k_f_rta_enable_info_zones;
        cal_dst->k_rta_f_higher_obj_crit_based_on_lower_ttp = cal_src->k_rta_f_higher_obj_crit_based_on_lower_ttp;
        cal_dst->k_f_rta_enable_wing_zones = cal_src->k_f_rta_enable_wing_zones;
        cal_dst->k_ta_alert_qualifying_cycles = cal_src->k_ta_alert_qualifying_cycles;
        cal_dst->k_ta_alert_holding_cycles = cal_src->k_ta_alert_holding_cycles;
        cal_dst->k_ta_prediction_steps_max = cal_src->k_ta_prediction_steps_max;
        cal_dst->k_ta_critical_approach_check_ego_circles[0] = cal_src->k_ta_critical_approach_check_ego_circles[0];
        cal_dst->k_ta_critical_approach_check_ego_circles[1] = cal_src->k_ta_critical_approach_check_ego_circles[1];
        cal_dst->k_ta_critical_approach_check_ego_circles[2] = cal_src->k_ta_critical_approach_check_ego_circles[2];
        cal_dst->k_ta_ego_pred_const_velocity_pred_steps_min = cal_src->k_ta_ego_pred_const_velocity_pred_steps_min;
        cal_dst->k_fta_obj_age_min = cal_src->k_fta_obj_age_min;
        cal_dst->k_fta_danger_zone_point_size = cal_src->k_fta_danger_zone_point_size;
        cal_dst->k_pfgs_qualification_counter_fast_obj = cal_src->k_pfgs_qualification_counter_fast_obj;
        cal_dst->k_pfgs_qualification_counter_slow_obj = cal_src->k_pfgs_qualification_counter_slow_obj;
        cal_dst->k_rta_obj_age_min = cal_src->k_rta_obj_age_min;
        cal_dst->k_rta_info_zone_point_size = cal_src->k_rta_info_zone_point_size;
        cal_dst->k_rta_wing_zone_point_size = cal_src->k_rta_wing_zone_point_size;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;
        cal_dst->k_unused_padding_byte_1 = cal_src->k_unused_padding_byte_1;
        cal_dst->k_unused_padding_byte_2 = cal_src->k_unused_padding_byte_2;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Ta_Update_Customer_Cal_By_Public(Ta_Customer_Calibration_T* cal_dst, const Ta_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Ta_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


