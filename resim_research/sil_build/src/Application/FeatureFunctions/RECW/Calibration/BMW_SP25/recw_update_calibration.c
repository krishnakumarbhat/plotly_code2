/**
* @file recw_update_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of update for the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "recw_update_calibration.h"
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "recw_core_calibration_check.h"
#include "recw_core_calibration_t.h"
#include "recw_customer_calibration_t.h"
#include "recw_public_calibration_check.h"
#include "recw_public_calibration_t.h"


/**************************************************
 * Global function definition
 **************************************************/


/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Recw_Update_Core_Cal_By_Public(Recw_Core_Calibration_T* cal_dst, const Recw_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Recw_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_recw_min_rel_velocity[0] = cal_src->k_recw_min_rel_velocity[0];
        cal_dst->k_recw_min_rel_velocity[1] = cal_src->k_recw_min_rel_velocity[1];
        cal_dst->k_recw_min_rel_velocity_hys[0] = cal_src->k_recw_min_rel_velocity_hys[0];
        cal_dst->k_recw_min_rel_velocity_hys[1] = cal_src->k_recw_min_rel_velocity_hys[1];
        cal_dst->k_recw_max_heading[0] = cal_src->k_recw_max_heading[0];
        cal_dst->k_recw_max_heading[1] = cal_src->k_recw_max_heading[1];
        cal_dst->k_recw_max_heading[2] = cal_src->k_recw_max_heading[2];
        cal_dst->k_recw_max_heading_hys = cal_src->k_recw_max_heading_hys;
        cal_dst->k_recw_factor_ego_width = cal_src->k_recw_factor_ego_width;
        cal_dst->k_recw_average_sensor_latency = cal_src->k_recw_average_sensor_latency;
        cal_dst->k_recw_min_crash_prob[0] = cal_src->k_recw_min_crash_prob[0];
        cal_dst->k_recw_min_crash_prob[1] = cal_src->k_recw_min_crash_prob[1];
        cal_dst->k_recw_min_existence_prob[0] = cal_src->k_recw_min_existence_prob[0];
        cal_dst->k_recw_min_existence_prob[1] = cal_src->k_recw_min_existence_prob[1];
        cal_dst->k_recw_min_existence_prob_hys = cal_src->k_recw_min_existence_prob_hys;
        cal_dst->k_recw_min_ttc_for_alert_level[0] = cal_src->k_recw_min_ttc_for_alert_level[0];
        cal_dst->k_recw_min_ttc_for_alert_level[1] = cal_src->k_recw_min_ttc_for_alert_level[1];
        cal_dst->k_recw_min_overlap_for_alert_level[0] = cal_src->k_recw_min_overlap_for_alert_level[0];
        cal_dst->k_recw_min_overlap_for_alert_level[1] = cal_src->k_recw_min_overlap_for_alert_level[1];
        cal_dst->k_recw_min_host_speed[0] = cal_src->k_recw_min_host_speed[0];
        cal_dst->k_recw_min_host_speed[1] = cal_src->k_recw_min_host_speed[1];
        cal_dst->k_recw_min_host_speed[2] = cal_src->k_recw_min_host_speed[2];
        cal_dst->k_recw_min_host_speed_hys = cal_src->k_recw_min_host_speed_hys;
        cal_dst->k_recw_max_host_speed[0] = cal_src->k_recw_max_host_speed[0];
        cal_dst->k_recw_max_host_speed[1] = cal_src->k_recw_max_host_speed[1];
        cal_dst->k_recw_max_host_speed[2] = cal_src->k_recw_max_host_speed[2];
        cal_dst->k_recw_max_host_speed_hys = cal_src->k_recw_max_host_speed_hys;
        cal_dst->k_recw_min_rel_velocity_for_max_ttc_threshold[0] = cal_src->k_recw_min_rel_velocity_for_max_ttc_threshold[0];
        cal_dst->k_recw_min_rel_velocity_for_max_ttc_threshold[1] = cal_src->k_recw_min_rel_velocity_for_max_ttc_threshold[1];
        cal_dst->k_recw_max_ttc_threshold[0] = cal_src->k_recw_max_ttc_threshold[0];
        cal_dst->k_recw_max_ttc_threshold[1] = cal_src->k_recw_max_ttc_threshold[1];
        cal_dst->k_recw_min_dist_for_young_slow_targets = cal_src->k_recw_min_dist_for_young_slow_targets;
        cal_dst->k_recw_min_abs_speed_for_young_close_targets = cal_src->k_recw_min_abs_speed_for_young_close_targets;
        cal_dst->k_recw_max_rel_velocity[0] = cal_src->k_recw_max_rel_velocity[0];
        cal_dst->k_recw_max_rel_velocity[1] = cal_src->k_recw_max_rel_velocity[1];
        cal_dst->k_recw_max_rel_velocity[2] = cal_src->k_recw_max_rel_velocity[2];
        cal_dst->k_recw_max_rel_velocity_hys = cal_src->k_recw_max_rel_velocity_hys;
        cal_dst->k_recw_max_rel_lon_vel_release_car_wash = cal_src->k_recw_max_rel_lon_vel_release_car_wash;
        cal_dst->k_recw_max_lon_distance_car_wash = cal_src->k_recw_max_lon_distance_car_wash;
        cal_dst->k_recw_max_lat_distance_car_wash = cal_src->k_recw_max_lat_distance_car_wash;
        cal_dst->k_recw_min_rel_lon_vel_car_wash = cal_src->k_recw_min_rel_lon_vel_car_wash;
        cal_dst->k_recw_max_speed_ego_car_wash = cal_src->k_recw_max_speed_ego_car_wash;
        cal_dst->k_recw_lane_filter_width = cal_src->k_recw_lane_filter_width;
        cal_dst->k_recw_lane_filter_width_hys = cal_src->k_recw_lane_filter_width_hys;
        cal_dst->k_recw_lane_filter_max_abs_ego_speed_vcs_coord = cal_src->k_recw_lane_filter_max_abs_ego_speed_vcs_coord;
        cal_dst->k_recw_lane_width_slope = cal_src->k_recw_lane_width_slope;
        cal_dst->k_recw_lookup_braking_deceleration[0] = cal_src->k_recw_lookup_braking_deceleration[0];
        cal_dst->k_recw_lookup_braking_deceleration[1] = cal_src->k_recw_lookup_braking_deceleration[1];
        cal_dst->k_recw_lookup_braking_deceleration[2] = cal_src->k_recw_lookup_braking_deceleration[2];
        cal_dst->k_recw_lookup_braking_deceleration[3] = cal_src->k_recw_lookup_braking_deceleration[3];
        cal_dst->k_recw_lookup_braking_deceleration[4] = cal_src->k_recw_lookup_braking_deceleration[4];
        cal_dst->k_recw_lookup_braking_deceleration[5] = cal_src->k_recw_lookup_braking_deceleration[5];
        cal_dst->k_recw_lookup_braking_probability[0] = cal_src->k_recw_lookup_braking_probability[0];
        cal_dst->k_recw_lookup_braking_probability[1] = cal_src->k_recw_lookup_braking_probability[1];
        cal_dst->k_recw_lookup_braking_probability[2] = cal_src->k_recw_lookup_braking_probability[2];
        cal_dst->k_recw_lookup_braking_probability[3] = cal_src->k_recw_lookup_braking_probability[3];
        cal_dst->k_recw_lookup_braking_probability[4] = cal_src->k_recw_lookup_braking_probability[4];
        cal_dst->k_recw_lookup_braking_probability[5] = cal_src->k_recw_lookup_braking_probability[5];
        cal_dst->k_recw_lookup_steering_acceleration[0] = cal_src->k_recw_lookup_steering_acceleration[0];
        cal_dst->k_recw_lookup_steering_acceleration[1] = cal_src->k_recw_lookup_steering_acceleration[1];
        cal_dst->k_recw_lookup_steering_acceleration[2] = cal_src->k_recw_lookup_steering_acceleration[2];
        cal_dst->k_recw_lookup_steering_acceleration[3] = cal_src->k_recw_lookup_steering_acceleration[3];
        cal_dst->k_recw_lookup_steering_acceleration[4] = cal_src->k_recw_lookup_steering_acceleration[4];
        cal_dst->k_recw_lookup_steering_acceleration[5] = cal_src->k_recw_lookup_steering_acceleration[5];
        cal_dst->k_recw_lookup_steering_probability[0] = cal_src->k_recw_lookup_steering_probability[0];
        cal_dst->k_recw_lookup_steering_probability[1] = cal_src->k_recw_lookup_steering_probability[1];
        cal_dst->k_recw_lookup_steering_probability[2] = cal_src->k_recw_lookup_steering_probability[2];
        cal_dst->k_recw_lookup_steering_probability[3] = cal_src->k_recw_lookup_steering_probability[3];
        cal_dst->k_recw_lookup_steering_probability[4] = cal_src->k_recw_lookup_steering_probability[4];
        cal_dst->k_recw_lookup_steering_probability[5] = cal_src->k_recw_lookup_steering_probability[5];
        cal_dst->k_recw_max_eclipse_value_for_valid_object = cal_src->k_recw_max_eclipse_value_for_valid_object;
        cal_dst->k_recw_max_object_width_warn_on = cal_src->k_recw_max_object_width_warn_on;
        cal_dst->k_recw_max_allowed_rel_vel_long_diff = cal_src->k_recw_max_allowed_rel_vel_long_diff;
        cal_dst->k_recw_max_allowed_rel_vel_lat_diff = cal_src->k_recw_max_allowed_rel_vel_lat_diff;
        cal_dst->k_recw_max_allowed_heading_diff = cal_src->k_recw_max_allowed_heading_diff;
        cal_dst->k_recw_rear_blockage_speed_threshold = cal_src->k_recw_rear_blockage_speed_threshold;
        cal_dst->k_recw_rear_blockage_width = cal_src->k_recw_rear_blockage_width;
        cal_dst->k_recw_rear_blockage_length = cal_src->k_recw_rear_blockage_length;
        cal_dst->k_recw_rear_blockage_ego_speed_threshold = cal_src->k_recw_rear_blockage_ego_speed_threshold;
        cal_dst->k_recw_heading_accuracy_threshold = cal_src->k_recw_heading_accuracy_threshold;
        cal_dst->k_recw_min_speed_not_stationary = cal_src->k_recw_min_speed_not_stationary;
        cal_dst->k_recb_max_host_speed = cal_src->k_recb_max_host_speed;
        cal_dst->k_recb_max_host_speed_hys = cal_src->k_recb_max_host_speed_hys;
        cal_dst->k_recb_max_velocity_host_standstill = cal_src->k_recb_max_velocity_host_standstill;
        cal_dst->k_recb_min_rel_velocity = cal_src->k_recb_min_rel_velocity;
        cal_dst->k_recb_min_rel_velocity_hys = cal_src->k_recb_min_rel_velocity_hys;
        cal_dst->k_recb_max_ttc = cal_src->k_recb_max_ttc;
        cal_dst->k_recb_nominal_acceleration_applied = cal_src->k_recb_nominal_acceleration_applied;
        cal_dst->k_recb_accelerator_pedal_gradient_threshold = cal_src->k_recb_accelerator_pedal_gradient_threshold;
        cal_dst->k_recw_f_make_use_of_guardrail = cal_src->k_recw_f_make_use_of_guardrail;
        cal_dst->k_recw_f_only_allow_consecutive_alert_levels = cal_src->k_recw_f_only_allow_consecutive_alert_levels;
        cal_dst->k_recw_f_allow_alert_on_coasted_objects[0] = cal_src->k_recw_f_allow_alert_on_coasted_objects[0];
        cal_dst->k_recw_f_allow_alert_on_coasted_objects[1] = cal_src->k_recw_f_allow_alert_on_coasted_objects[1];
        cal_dst->k_recw_f_use_rear_blockage[0] = cal_src->k_recw_f_use_rear_blockage[0];
        cal_dst->k_recw_f_use_rear_blockage[1] = cal_src->k_recw_f_use_rear_blockage[1];
        cal_dst->k_recw_f_apply_lane_filter = cal_src->k_recw_f_apply_lane_filter;
        cal_dst->k_recw_f_enable_heading_filter = cal_src->k_recw_f_enable_heading_filter;
        cal_dst->k_recw_f_enable_traffic_light_ghost_detection = cal_src->k_recw_f_enable_traffic_light_ghost_detection;
        cal_dst->k_recb_f_enable_recb = cal_src->k_recb_f_enable_recb;
        cal_dst->k_recw_alert_holding_cycles[0] = cal_src->k_recw_alert_holding_cycles[0];
        cal_dst->k_recw_alert_holding_cycles[1] = cal_src->k_recw_alert_holding_cycles[1];
        cal_dst->k_recw_alert_qualifying_cycles = cal_src->k_recw_alert_qualifying_cycles;
        cal_dst->k_recw_min_stage_age_for_alert_level[0] = cal_src->k_recw_min_stage_age_for_alert_level[0];
        cal_dst->k_recw_min_stage_age_for_alert_level[1] = cal_src->k_recw_min_stage_age_for_alert_level[1];
        cal_dst->k_recw_max_cycles_alert_duration[0] = cal_src->k_recw_max_cycles_alert_duration[0];
        cal_dst->k_recw_max_cycles_alert_duration[1] = cal_src->k_recw_max_cycles_alert_duration[1];
        cal_dst->k_recw_min_age_for_close_slow_targets = cal_src->k_recw_min_age_for_close_slow_targets;
        cal_dst->k_recw_min_object_age = cal_src->k_recw_min_object_age;
        cal_dst->k_recw_min_cycles_with_min_crash_prob = cal_src->k_recw_min_cycles_with_min_crash_prob;
        cal_dst->k_recw_en_active_car_wash_logic = cal_src->k_recw_en_active_car_wash_logic;
        cal_dst->k_recw_lane_filter_num_consecutive_cycles = cal_src->k_recw_lane_filter_num_consecutive_cycles;
        cal_dst->k_recw_max_allowed_consecutive_coasted_cycles = cal_src->k_recw_max_allowed_consecutive_coasted_cycles;
        cal_dst->k_recw_rear_blockage_qualifying_cycles = cal_src->k_recw_rear_blockage_qualifying_cycles;
        cal_dst->k_recb_min_cycles_host_standstill = cal_src->k_recb_min_cycles_host_standstill;
        cal_dst->k_recb_ssm_braking = cal_src->k_recb_ssm_braking;
        cal_dst->k_recb_ssm_request_cancelled = cal_src->k_recb_ssm_request_cancelled;
        cal_dst->k_recb_integrity = cal_src->k_recb_integrity;
        cal_dst->k_recb_qualifier_nominal_acceleration = cal_src->k_recb_qualifier_nominal_acceleration;
        cal_dst->k_recb_max_cycles_braking_request_duration = cal_src->k_recb_max_cycles_braking_request_duration;
        cal_dst->k_recw_f_suppress_alert_lvl_2_for_pedestrian = cal_src->k_recw_f_suppress_alert_lvl_2_for_pedestrian;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Recw_Update_Core_Cal_By_Core(Recw_Core_Calibration_T* cal_dst, const Recw_Core_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Recw_Core_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {
        cal_dst->k_recw_min_rel_velocity[0] = cal_src->k_recw_min_rel_velocity[0];
        cal_dst->k_recw_min_rel_velocity[1] = cal_src->k_recw_min_rel_velocity[1];
        cal_dst->k_recw_min_rel_velocity_hys[0] = cal_src->k_recw_min_rel_velocity_hys[0];
        cal_dst->k_recw_min_rel_velocity_hys[1] = cal_src->k_recw_min_rel_velocity_hys[1];
        cal_dst->k_recw_max_heading[0] = cal_src->k_recw_max_heading[0];
        cal_dst->k_recw_max_heading[1] = cal_src->k_recw_max_heading[1];
        cal_dst->k_recw_max_heading[2] = cal_src->k_recw_max_heading[2];
        cal_dst->k_recw_max_heading_hys = cal_src->k_recw_max_heading_hys;
        cal_dst->k_recw_factor_ego_width = cal_src->k_recw_factor_ego_width;
        cal_dst->k_recw_average_sensor_latency = cal_src->k_recw_average_sensor_latency;
        cal_dst->k_recw_min_crash_prob[0] = cal_src->k_recw_min_crash_prob[0];
        cal_dst->k_recw_min_crash_prob[1] = cal_src->k_recw_min_crash_prob[1];
        cal_dst->k_recw_min_existence_prob[0] = cal_src->k_recw_min_existence_prob[0];
        cal_dst->k_recw_min_existence_prob[1] = cal_src->k_recw_min_existence_prob[1];
        cal_dst->k_recw_min_existence_prob_hys = cal_src->k_recw_min_existence_prob_hys;
        cal_dst->k_recw_min_ttc_for_alert_level[0] = cal_src->k_recw_min_ttc_for_alert_level[0];
        cal_dst->k_recw_min_ttc_for_alert_level[1] = cal_src->k_recw_min_ttc_for_alert_level[1];
        cal_dst->k_recw_min_overlap_for_alert_level[0] = cal_src->k_recw_min_overlap_for_alert_level[0];
        cal_dst->k_recw_min_overlap_for_alert_level[1] = cal_src->k_recw_min_overlap_for_alert_level[1];
        cal_dst->k_recw_min_host_speed[0] = cal_src->k_recw_min_host_speed[0];
        cal_dst->k_recw_min_host_speed[1] = cal_src->k_recw_min_host_speed[1];
        cal_dst->k_recw_min_host_speed[2] = cal_src->k_recw_min_host_speed[2];
        cal_dst->k_recw_min_host_speed_hys = cal_src->k_recw_min_host_speed_hys;
        cal_dst->k_recw_max_host_speed[0] = cal_src->k_recw_max_host_speed[0];
        cal_dst->k_recw_max_host_speed[1] = cal_src->k_recw_max_host_speed[1];
        cal_dst->k_recw_max_host_speed[2] = cal_src->k_recw_max_host_speed[2];
        cal_dst->k_recw_max_host_speed_hys = cal_src->k_recw_max_host_speed_hys;
        cal_dst->k_recw_min_rel_velocity_for_max_ttc_threshold[0] = cal_src->k_recw_min_rel_velocity_for_max_ttc_threshold[0];
        cal_dst->k_recw_min_rel_velocity_for_max_ttc_threshold[1] = cal_src->k_recw_min_rel_velocity_for_max_ttc_threshold[1];
        cal_dst->k_recw_max_ttc_threshold[0] = cal_src->k_recw_max_ttc_threshold[0];
        cal_dst->k_recw_max_ttc_threshold[1] = cal_src->k_recw_max_ttc_threshold[1];
        cal_dst->k_recw_min_dist_for_young_slow_targets = cal_src->k_recw_min_dist_for_young_slow_targets;
        cal_dst->k_recw_min_abs_speed_for_young_close_targets = cal_src->k_recw_min_abs_speed_for_young_close_targets;
        cal_dst->k_recw_max_rel_velocity[0] = cal_src->k_recw_max_rel_velocity[0];
        cal_dst->k_recw_max_rel_velocity[1] = cal_src->k_recw_max_rel_velocity[1];
        cal_dst->k_recw_max_rel_velocity[2] = cal_src->k_recw_max_rel_velocity[2];
        cal_dst->k_recw_max_rel_velocity_hys = cal_src->k_recw_max_rel_velocity_hys;
        cal_dst->k_recw_max_rel_lon_vel_release_car_wash = cal_src->k_recw_max_rel_lon_vel_release_car_wash;
        cal_dst->k_recw_max_lon_distance_car_wash = cal_src->k_recw_max_lon_distance_car_wash;
        cal_dst->k_recw_max_lat_distance_car_wash = cal_src->k_recw_max_lat_distance_car_wash;
        cal_dst->k_recw_min_rel_lon_vel_car_wash = cal_src->k_recw_min_rel_lon_vel_car_wash;
        cal_dst->k_recw_max_speed_ego_car_wash = cal_src->k_recw_max_speed_ego_car_wash;
        cal_dst->k_recw_lane_filter_width = cal_src->k_recw_lane_filter_width;
        cal_dst->k_recw_lane_filter_width_hys = cal_src->k_recw_lane_filter_width_hys;
        cal_dst->k_recw_lane_filter_max_abs_ego_speed_vcs_coord = cal_src->k_recw_lane_filter_max_abs_ego_speed_vcs_coord;
        cal_dst->k_recw_lane_width_slope = cal_src->k_recw_lane_width_slope;
        cal_dst->k_recw_lookup_braking_deceleration[0] = cal_src->k_recw_lookup_braking_deceleration[0];
        cal_dst->k_recw_lookup_braking_deceleration[1] = cal_src->k_recw_lookup_braking_deceleration[1];
        cal_dst->k_recw_lookup_braking_deceleration[2] = cal_src->k_recw_lookup_braking_deceleration[2];
        cal_dst->k_recw_lookup_braking_deceleration[3] = cal_src->k_recw_lookup_braking_deceleration[3];
        cal_dst->k_recw_lookup_braking_deceleration[4] = cal_src->k_recw_lookup_braking_deceleration[4];
        cal_dst->k_recw_lookup_braking_deceleration[5] = cal_src->k_recw_lookup_braking_deceleration[5];
        cal_dst->k_recw_lookup_braking_probability[0] = cal_src->k_recw_lookup_braking_probability[0];
        cal_dst->k_recw_lookup_braking_probability[1] = cal_src->k_recw_lookup_braking_probability[1];
        cal_dst->k_recw_lookup_braking_probability[2] = cal_src->k_recw_lookup_braking_probability[2];
        cal_dst->k_recw_lookup_braking_probability[3] = cal_src->k_recw_lookup_braking_probability[3];
        cal_dst->k_recw_lookup_braking_probability[4] = cal_src->k_recw_lookup_braking_probability[4];
        cal_dst->k_recw_lookup_braking_probability[5] = cal_src->k_recw_lookup_braking_probability[5];
        cal_dst->k_recw_lookup_steering_acceleration[0] = cal_src->k_recw_lookup_steering_acceleration[0];
        cal_dst->k_recw_lookup_steering_acceleration[1] = cal_src->k_recw_lookup_steering_acceleration[1];
        cal_dst->k_recw_lookup_steering_acceleration[2] = cal_src->k_recw_lookup_steering_acceleration[2];
        cal_dst->k_recw_lookup_steering_acceleration[3] = cal_src->k_recw_lookup_steering_acceleration[3];
        cal_dst->k_recw_lookup_steering_acceleration[4] = cal_src->k_recw_lookup_steering_acceleration[4];
        cal_dst->k_recw_lookup_steering_acceleration[5] = cal_src->k_recw_lookup_steering_acceleration[5];
        cal_dst->k_recw_lookup_steering_probability[0] = cal_src->k_recw_lookup_steering_probability[0];
        cal_dst->k_recw_lookup_steering_probability[1] = cal_src->k_recw_lookup_steering_probability[1];
        cal_dst->k_recw_lookup_steering_probability[2] = cal_src->k_recw_lookup_steering_probability[2];
        cal_dst->k_recw_lookup_steering_probability[3] = cal_src->k_recw_lookup_steering_probability[3];
        cal_dst->k_recw_lookup_steering_probability[4] = cal_src->k_recw_lookup_steering_probability[4];
        cal_dst->k_recw_lookup_steering_probability[5] = cal_src->k_recw_lookup_steering_probability[5];
        cal_dst->k_recw_max_eclipse_value_for_valid_object = cal_src->k_recw_max_eclipse_value_for_valid_object;
        cal_dst->k_recw_max_object_width_warn_on = cal_src->k_recw_max_object_width_warn_on;
        cal_dst->k_recw_max_allowed_rel_vel_long_diff = cal_src->k_recw_max_allowed_rel_vel_long_diff;
        cal_dst->k_recw_max_allowed_rel_vel_lat_diff = cal_src->k_recw_max_allowed_rel_vel_lat_diff;
        cal_dst->k_recw_max_allowed_heading_diff = cal_src->k_recw_max_allowed_heading_diff;
        cal_dst->k_recw_rear_blockage_speed_threshold = cal_src->k_recw_rear_blockage_speed_threshold;
        cal_dst->k_recw_rear_blockage_width = cal_src->k_recw_rear_blockage_width;
        cal_dst->k_recw_rear_blockage_length = cal_src->k_recw_rear_blockage_length;
        cal_dst->k_recw_rear_blockage_ego_speed_threshold = cal_src->k_recw_rear_blockage_ego_speed_threshold;
        cal_dst->k_recw_heading_accuracy_threshold = cal_src->k_recw_heading_accuracy_threshold;
        cal_dst->k_recw_min_speed_not_stationary = cal_src->k_recw_min_speed_not_stationary;
        cal_dst->k_recb_max_host_speed = cal_src->k_recb_max_host_speed;
        cal_dst->k_recb_max_host_speed_hys = cal_src->k_recb_max_host_speed_hys;
        cal_dst->k_recb_max_velocity_host_standstill = cal_src->k_recb_max_velocity_host_standstill;
        cal_dst->k_recb_min_rel_velocity = cal_src->k_recb_min_rel_velocity;
        cal_dst->k_recb_min_rel_velocity_hys = cal_src->k_recb_min_rel_velocity_hys;
        cal_dst->k_recb_max_ttc = cal_src->k_recb_max_ttc;
        cal_dst->k_recb_nominal_acceleration_applied = cal_src->k_recb_nominal_acceleration_applied;
        cal_dst->k_recb_accelerator_pedal_gradient_threshold = cal_src->k_recb_accelerator_pedal_gradient_threshold;
        cal_dst->k_recw_f_make_use_of_guardrail = cal_src->k_recw_f_make_use_of_guardrail;
        cal_dst->k_recw_f_only_allow_consecutive_alert_levels = cal_src->k_recw_f_only_allow_consecutive_alert_levels;
        cal_dst->k_recw_f_allow_alert_on_coasted_objects[0] = cal_src->k_recw_f_allow_alert_on_coasted_objects[0];
        cal_dst->k_recw_f_allow_alert_on_coasted_objects[1] = cal_src->k_recw_f_allow_alert_on_coasted_objects[1];
        cal_dst->k_recw_f_use_rear_blockage[0] = cal_src->k_recw_f_use_rear_blockage[0];
        cal_dst->k_recw_f_use_rear_blockage[1] = cal_src->k_recw_f_use_rear_blockage[1];
        cal_dst->k_recw_f_apply_lane_filter = cal_src->k_recw_f_apply_lane_filter;
        cal_dst->k_recw_f_enable_heading_filter = cal_src->k_recw_f_enable_heading_filter;
        cal_dst->k_recw_f_enable_traffic_light_ghost_detection = cal_src->k_recw_f_enable_traffic_light_ghost_detection;
        cal_dst->k_recb_f_enable_recb = cal_src->k_recb_f_enable_recb;
        cal_dst->k_recw_alert_holding_cycles[0] = cal_src->k_recw_alert_holding_cycles[0];
        cal_dst->k_recw_alert_holding_cycles[1] = cal_src->k_recw_alert_holding_cycles[1];
        cal_dst->k_recw_alert_qualifying_cycles = cal_src->k_recw_alert_qualifying_cycles;
        cal_dst->k_recw_min_stage_age_for_alert_level[0] = cal_src->k_recw_min_stage_age_for_alert_level[0];
        cal_dst->k_recw_min_stage_age_for_alert_level[1] = cal_src->k_recw_min_stage_age_for_alert_level[1];
        cal_dst->k_recw_max_cycles_alert_duration[0] = cal_src->k_recw_max_cycles_alert_duration[0];
        cal_dst->k_recw_max_cycles_alert_duration[1] = cal_src->k_recw_max_cycles_alert_duration[1];
        cal_dst->k_recw_min_age_for_close_slow_targets = cal_src->k_recw_min_age_for_close_slow_targets;
        cal_dst->k_recw_min_object_age = cal_src->k_recw_min_object_age;
        cal_dst->k_recw_min_cycles_with_min_crash_prob = cal_src->k_recw_min_cycles_with_min_crash_prob;
        cal_dst->k_recw_en_active_car_wash_logic = cal_src->k_recw_en_active_car_wash_logic;
        cal_dst->k_recw_lane_filter_num_consecutive_cycles = cal_src->k_recw_lane_filter_num_consecutive_cycles;
        cal_dst->k_recw_max_allowed_consecutive_coasted_cycles = cal_src->k_recw_max_allowed_consecutive_coasted_cycles;
        cal_dst->k_recw_rear_blockage_qualifying_cycles = cal_src->k_recw_rear_blockage_qualifying_cycles;
        cal_dst->k_recb_min_cycles_host_standstill = cal_src->k_recb_min_cycles_host_standstill;
        cal_dst->k_recb_ssm_braking = cal_src->k_recb_ssm_braking;
        cal_dst->k_recb_ssm_request_cancelled = cal_src->k_recb_ssm_request_cancelled;
        cal_dst->k_recb_integrity = cal_src->k_recb_integrity;
        cal_dst->k_recb_qualifier_nominal_acceleration = cal_src->k_recb_qualifier_nominal_acceleration;
        cal_dst->k_recb_max_cycles_braking_request_duration = cal_src->k_recb_max_cycles_braking_request_duration;
        cal_dst->k_recw_f_suppress_alert_lvl_2_for_pedestrian = cal_src->k_recw_f_suppress_alert_lvl_2_for_pedestrian;
        cal_dst->k_unused_padding_byte_0 = cal_src->k_unused_padding_byte_0;

        f_result = (boolean_T) 1;
    }
    return f_result;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
boolean_T Recw_Update_Customer_Cal_By_Public(Recw_Customer_Calibration_T* cal_dst, const Recw_Public_Calibration_T* cal_src)
{
    boolean_T f_result;
    CAN_BE_UNUSED(cal_dst);
    CAN_BE_UNUSED(cal_src);
    if ( (cal_src == NULL) || (!Recw_Public_Cal_In_Boundary(cal_src)))
    {
        f_result = (boolean_T) 0;
    }
    else
    {

        f_result = (boolean_T) 1;
    }
    return f_result;
}


