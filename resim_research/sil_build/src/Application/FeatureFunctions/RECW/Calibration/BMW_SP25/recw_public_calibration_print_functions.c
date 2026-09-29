/**
* @file recw_public_calibration_print_functions.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides a printing function for the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "recw_public_calibration_t.h" // IWYU pragma: keep
#include "recw_public_calibration.h" // IWYU pragma: keep

#ifdef CT_ACTIVATE_CAL_PRINT
#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

#include <stdio.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable points to a non-constant type but does not modify the object it points to]*/
void Recw_Public_Cal_Print(FILE* c_file_ptr, const Recw_Public_Calibration_T* p_cals)
{
    CAN_BE_UNUSED(p_cals);
    CAN_BE_UNUSED(c_file_ptr);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_velocity._0_,%f\n", p_cals->k_recw_min_rel_velocity[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_velocity._1_,%f\n", p_cals->k_recw_min_rel_velocity[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_velocity_hys._0_,%f\n", p_cals->k_recw_min_rel_velocity_hys[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_velocity_hys._1_,%f\n", p_cals->k_recw_min_rel_velocity_hys[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_heading._0_,%f\n", p_cals->k_recw_max_heading[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_heading._1_,%f\n", p_cals->k_recw_max_heading[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_heading._2_,%f\n", p_cals->k_recw_max_heading[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_heading_hys,%f\n", p_cals->k_recw_max_heading_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_factor_ego_width,%f\n", p_cals->k_recw_factor_ego_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_average_sensor_latency,%f\n", p_cals->k_recw_average_sensor_latency);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_crash_prob._0_,%f\n", p_cals->k_recw_min_crash_prob[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_crash_prob._1_,%f\n", p_cals->k_recw_min_crash_prob[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_existence_prob._0_,%f\n", p_cals->k_recw_min_existence_prob[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_existence_prob._1_,%f\n", p_cals->k_recw_min_existence_prob[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_existence_prob_hys,%f\n", p_cals->k_recw_min_existence_prob_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_ttc_for_alert_level._0_,%f\n", p_cals->k_recw_min_ttc_for_alert_level[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_ttc_for_alert_level._1_,%f\n", p_cals->k_recw_min_ttc_for_alert_level[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_overlap_for_alert_level._0_,%f\n", p_cals->k_recw_min_overlap_for_alert_level[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_overlap_for_alert_level._1_,%f\n", p_cals->k_recw_min_overlap_for_alert_level[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_host_speed._0_,%f\n", p_cals->k_recw_min_host_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_host_speed._1_,%f\n", p_cals->k_recw_min_host_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_host_speed._2_,%f\n", p_cals->k_recw_min_host_speed[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_host_speed_hys,%f\n", p_cals->k_recw_min_host_speed_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_host_speed._0_,%f\n", p_cals->k_recw_max_host_speed[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_host_speed._1_,%f\n", p_cals->k_recw_max_host_speed[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_host_speed._2_,%f\n", p_cals->k_recw_max_host_speed[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_host_speed_hys,%f\n", p_cals->k_recw_max_host_speed_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_velocity_for_max_ttc_threshold._0_,%f\n", p_cals->k_recw_min_rel_velocity_for_max_ttc_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_velocity_for_max_ttc_threshold._1_,%f\n", p_cals->k_recw_min_rel_velocity_for_max_ttc_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_ttc_threshold._0_,%f\n", p_cals->k_recw_max_ttc_threshold[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_ttc_threshold._1_,%f\n", p_cals->k_recw_max_ttc_threshold[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_dist_for_young_slow_targets,%f\n", p_cals->k_recw_min_dist_for_young_slow_targets);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_abs_speed_for_young_close_targets,%f\n", p_cals->k_recw_min_abs_speed_for_young_close_targets);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_rel_velocity._0_,%f\n", p_cals->k_recw_max_rel_velocity[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_rel_velocity._1_,%f\n", p_cals->k_recw_max_rel_velocity[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_rel_velocity._2_,%f\n", p_cals->k_recw_max_rel_velocity[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_rel_velocity_hys,%f\n", p_cals->k_recw_max_rel_velocity_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_rel_lon_vel_release_car_wash,%f\n", p_cals->k_recw_max_rel_lon_vel_release_car_wash);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_lon_distance_car_wash,%f\n", p_cals->k_recw_max_lon_distance_car_wash);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_lat_distance_car_wash,%f\n", p_cals->k_recw_max_lat_distance_car_wash);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_rel_lon_vel_car_wash,%f\n", p_cals->k_recw_min_rel_lon_vel_car_wash);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_speed_ego_car_wash,%f\n", p_cals->k_recw_max_speed_ego_car_wash);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lane_filter_width,%f\n", p_cals->k_recw_lane_filter_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lane_filter_width_hys,%f\n", p_cals->k_recw_lane_filter_width_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lane_filter_max_abs_ego_speed_vcs_coord,%f\n", p_cals->k_recw_lane_filter_max_abs_ego_speed_vcs_coord);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lane_width_slope,%f\n", p_cals->k_recw_lane_width_slope);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_deceleration._0_,%f\n", p_cals->k_recw_lookup_braking_deceleration[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_deceleration._1_,%f\n", p_cals->k_recw_lookup_braking_deceleration[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_deceleration._2_,%f\n", p_cals->k_recw_lookup_braking_deceleration[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_deceleration._3_,%f\n", p_cals->k_recw_lookup_braking_deceleration[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_deceleration._4_,%f\n", p_cals->k_recw_lookup_braking_deceleration[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_deceleration._5_,%f\n", p_cals->k_recw_lookup_braking_deceleration[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_probability._0_,%f\n", p_cals->k_recw_lookup_braking_probability[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_probability._1_,%f\n", p_cals->k_recw_lookup_braking_probability[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_probability._2_,%f\n", p_cals->k_recw_lookup_braking_probability[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_probability._3_,%f\n", p_cals->k_recw_lookup_braking_probability[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_probability._4_,%f\n", p_cals->k_recw_lookup_braking_probability[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_braking_probability._5_,%f\n", p_cals->k_recw_lookup_braking_probability[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_acceleration._0_,%f\n", p_cals->k_recw_lookup_steering_acceleration[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_acceleration._1_,%f\n", p_cals->k_recw_lookup_steering_acceleration[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_acceleration._2_,%f\n", p_cals->k_recw_lookup_steering_acceleration[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_acceleration._3_,%f\n", p_cals->k_recw_lookup_steering_acceleration[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_acceleration._4_,%f\n", p_cals->k_recw_lookup_steering_acceleration[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_acceleration._5_,%f\n", p_cals->k_recw_lookup_steering_acceleration[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_probability._0_,%f\n", p_cals->k_recw_lookup_steering_probability[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_probability._1_,%f\n", p_cals->k_recw_lookup_steering_probability[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_probability._2_,%f\n", p_cals->k_recw_lookup_steering_probability[2]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_probability._3_,%f\n", p_cals->k_recw_lookup_steering_probability[3]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_probability._4_,%f\n", p_cals->k_recw_lookup_steering_probability[4]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lookup_steering_probability._5_,%f\n", p_cals->k_recw_lookup_steering_probability[5]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_eclipse_value_for_valid_object,%f\n", p_cals->k_recw_max_eclipse_value_for_valid_object);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_object_width_warn_on,%f\n", p_cals->k_recw_max_object_width_warn_on);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_allowed_rel_vel_long_diff,%f\n", p_cals->k_recw_max_allowed_rel_vel_long_diff);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_allowed_rel_vel_lat_diff,%f\n", p_cals->k_recw_max_allowed_rel_vel_lat_diff);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_allowed_heading_diff,%f\n", p_cals->k_recw_max_allowed_heading_diff);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_rear_blockage_speed_threshold,%f\n", p_cals->k_recw_rear_blockage_speed_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_rear_blockage_width,%f\n", p_cals->k_recw_rear_blockage_width);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_rear_blockage_length,%f\n", p_cals->k_recw_rear_blockage_length);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_rear_blockage_ego_speed_threshold,%f\n", p_cals->k_recw_rear_blockage_ego_speed_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_heading_accuracy_threshold,%f\n", p_cals->k_recw_heading_accuracy_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_speed_not_stationary,%f\n", p_cals->k_recw_min_speed_not_stationary);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_max_host_speed,%f\n", p_cals->k_recb_max_host_speed);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_max_host_speed_hys,%f\n", p_cals->k_recb_max_host_speed_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_max_velocity_host_standstill,%f\n", p_cals->k_recb_max_velocity_host_standstill);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_min_rel_velocity,%f\n", p_cals->k_recb_min_rel_velocity);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_min_rel_velocity_hys,%f\n", p_cals->k_recb_min_rel_velocity_hys);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_max_ttc,%f\n", p_cals->k_recb_max_ttc);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_nominal_acceleration_applied,%f\n", p_cals->k_recb_nominal_acceleration_applied);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_accelerator_pedal_gradient_threshold,%f\n", p_cals->k_recb_accelerator_pedal_gradient_threshold);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_make_use_of_guardrail,%d\n", p_cals->k_recw_f_make_use_of_guardrail);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_only_allow_consecutive_alert_levels,%d\n", p_cals->k_recw_f_only_allow_consecutive_alert_levels);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_allow_alert_on_coasted_objects._0_,%d\n", p_cals->k_recw_f_allow_alert_on_coasted_objects[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_allow_alert_on_coasted_objects._1_,%d\n", p_cals->k_recw_f_allow_alert_on_coasted_objects[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_use_rear_blockage._0_,%d\n", p_cals->k_recw_f_use_rear_blockage[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_use_rear_blockage._1_,%d\n", p_cals->k_recw_f_use_rear_blockage[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_apply_lane_filter,%d\n", p_cals->k_recw_f_apply_lane_filter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_enable_heading_filter,%d\n", p_cals->k_recw_f_enable_heading_filter);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_enable_traffic_light_ghost_detection,%d\n", p_cals->k_recw_f_enable_traffic_light_ghost_detection);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_f_enable_recb,%d\n", p_cals->k_recb_f_enable_recb);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_alert_holding_cycles._0_,%d\n", p_cals->k_recw_alert_holding_cycles[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_alert_holding_cycles._1_,%d\n", p_cals->k_recw_alert_holding_cycles[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_alert_qualifying_cycles,%d\n", p_cals->k_recw_alert_qualifying_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_stage_age_for_alert_level._0_,%d\n", p_cals->k_recw_min_stage_age_for_alert_level[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_stage_age_for_alert_level._1_,%d\n", p_cals->k_recw_min_stage_age_for_alert_level[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_cycles_alert_duration._0_,%d\n", p_cals->k_recw_max_cycles_alert_duration[0]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_cycles_alert_duration._1_,%d\n", p_cals->k_recw_max_cycles_alert_duration[1]);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_age_for_close_slow_targets,%d\n", p_cals->k_recw_min_age_for_close_slow_targets);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_object_age,%d\n", p_cals->k_recw_min_object_age);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_min_cycles_with_min_crash_prob,%d\n", p_cals->k_recw_min_cycles_with_min_crash_prob);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_en_active_car_wash_logic,%d\n", p_cals->k_recw_en_active_car_wash_logic);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_lane_filter_num_consecutive_cycles,%d\n", p_cals->k_recw_lane_filter_num_consecutive_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_max_allowed_consecutive_coasted_cycles,%d\n", p_cals->k_recw_max_allowed_consecutive_coasted_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_rear_blockage_qualifying_cycles,%d\n", p_cals->k_recw_rear_blockage_qualifying_cycles);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_min_cycles_host_standstill,%d\n", p_cals->k_recb_min_cycles_host_standstill);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_ssm_braking,%d\n", p_cals->k_recb_ssm_braking);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_ssm_request_cancelled,%d\n", p_cals->k_recb_ssm_request_cancelled);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_integrity,%d\n", p_cals->k_recb_integrity);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_qualifier_nominal_acceleration,%d\n", p_cals->k_recb_qualifier_nominal_acceleration);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recb_max_cycles_braking_request_duration,%d\n", p_cals->k_recb_max_cycles_braking_request_duration);
   /* coverity[misra_c_2012_rule_21_6_violation][Usage of fprintf is harmless as function is only used for debug purposes] */
   (void)fprintf(c_file_ptr,"p_cals.k_recw_f_suppress_alert_lvl_2_for_pedestrian,%d\n", p_cals->k_recw_f_suppress_alert_lvl_2_for_pedestrian);
}
#endif /*CT_ACTIVATE_CAL_PRINT*/
