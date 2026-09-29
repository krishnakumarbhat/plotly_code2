/**
* @file recw_core_calibration_check.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides implementation of boundary checks for the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/**************************************************
 * Includes
 **************************************************/

#include "recw_core_calibration_check.h" // IWYU pragma: keep
#include "recw_core_calibration.h" // IWYU pragma: keep
#include "ct_boundaries_check_function_helpers.h" // IWYU pragma: keep
#include "ct_calibration_header_t.h" // IWYU pragma: keep

/**************************************************
 * Global function definition
 **************************************************/

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
boolean_T Recw_Core_Cal_In_Boundary(const Recw_Core_Calibration_T *p_calibration)
{
   boolean_T f_recw_calibration_in_boundaries = (boolean_T) 1;
   
   CAN_BE_UNUSED(p_calibration);

   /**< Check boundaries of all calibrations. In case of multidimensional arrays for loops are shared across
   calibrations with the same dimension. */
   
   {
    uint8_t x;
    for (x = 0u; x < RECW_K_RECW_ALERT_HOLDING_CYCLES_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recw_alert_holding_cycles[x], RECW_MAX_K_RECW_ALERT_HOLDING_CYCLES);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS, p_calibration->k_recw_f_allow_alert_on_coasted_objects[x], RECW_MAX_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_USE_REAR_BLOCKAGE, p_calibration->k_recw_f_use_rear_blockage[x], RECW_MAX_K_RECW_F_USE_REAR_BLOCKAGE);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_TTC_THRESHOLD, p_calibration->k_recw_max_ttc_threshold[x], RECW_MAX_K_RECW_MAX_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_CRASH_PROB, p_calibration->k_recw_min_crash_prob[x], RECW_MAX_K_RECW_MIN_CRASH_PROB);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_EXISTENCE_PROB, p_calibration->k_recw_min_existence_prob[x], RECW_MAX_K_RECW_MIN_EXISTENCE_PROB);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL, p_calibration->k_recw_min_overlap_for_alert_level[x], RECW_MAX_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_REL_VELOCITY, p_calibration->k_recw_min_rel_velocity[x], RECW_MAX_K_RECW_MIN_REL_VELOCITY);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD, p_calibration->k_recw_min_rel_velocity_for_max_ttc_threshold[x], RECW_MAX_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_REL_VELOCITY_HYS, p_calibration->k_recw_min_rel_velocity_hys[x], RECW_MAX_K_RECW_MIN_REL_VELOCITY_HYS);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recw_min_stage_age_for_alert_level[x], RECW_MAX_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_TTC_FOR_ALERT_LEVEL, p_calibration->k_recw_min_ttc_for_alert_level[x], RECW_MAX_K_RECW_MIN_TTC_FOR_ALERT_LEVEL);

        }
}
{
    uint8_t x;
    for (x = 0u; x < RECW_K_RECW_MAX_HEADING_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_HEADING, p_calibration->k_recw_max_heading[x], RECW_MAX_K_RECW_MAX_HEADING);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_HOST_SPEED, p_calibration->k_recw_max_host_speed[x], RECW_MAX_K_RECW_MAX_HOST_SPEED);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_REL_VELOCITY, p_calibration->k_recw_max_rel_velocity[x], RECW_MAX_K_RECW_MAX_REL_VELOCITY);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_HOST_SPEED, p_calibration->k_recw_min_host_speed[x], RECW_MAX_K_RECW_MIN_HOST_SPEED);

        }
}
{
    uint8_t x;
    for (x = 0u; x < RECW_K_RECW_LOOKUP_BRAKING_DECELERATION_ARRAY_SIZE_DIM0; x++)
        {
        Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LOOKUP_BRAKING_DECELERATION, p_calibration->k_recw_lookup_braking_deceleration[x], RECW_MAX_K_RECW_LOOKUP_BRAKING_DECELERATION);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LOOKUP_BRAKING_PROBABILITY, p_calibration->k_recw_lookup_braking_probability[x], RECW_MAX_K_RECW_LOOKUP_BRAKING_PROBABILITY);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LOOKUP_STEERING_ACCELERATION, p_calibration->k_recw_lookup_steering_acceleration[x], RECW_MAX_K_RECW_LOOKUP_STEERING_ACCELERATION);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LOOKUP_STEERING_PROBABILITY, p_calibration->k_recw_lookup_steering_probability[x], RECW_MAX_K_RECW_LOOKUP_STEERING_PROBABILITY);

        }
}

   /* coverity[misra_c_2012_rule_14_3_violation][The condition must be true] */
   Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_ACCELERATOR_PEDAL_GRADIENT_THRESHOLD, p_calibration->k_recb_accelerator_pedal_gradient_threshold, RECW_MAX_K_RECB_ACCELERATOR_PEDAL_GRADIENT_THRESHOLD);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_F_ENABLE_RECB, p_calibration->k_recb_f_enable_recb, RECW_MAX_K_RECB_F_ENABLE_RECB);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recb_integrity, RECW_MAX_K_RECB_INTEGRITY);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_MAX_HOST_SPEED, p_calibration->k_recb_max_host_speed, RECW_MAX_K_RECB_MAX_HOST_SPEED);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_MAX_HOST_SPEED_HYS, p_calibration->k_recb_max_host_speed_hys, RECW_MAX_K_RECB_MAX_HOST_SPEED_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_MAX_TTC, p_calibration->k_recb_max_ttc, RECW_MAX_K_RECB_MAX_TTC);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_MAX_VELOCITY_HOST_STANDSTILL, p_calibration->k_recb_max_velocity_host_standstill, RECW_MAX_K_RECB_MAX_VELOCITY_HOST_STANDSTILL);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recb_min_cycles_host_standstill, RECW_MAX_K_RECB_MIN_CYCLES_HOST_STANDSTILL);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_MIN_REL_VELOCITY, p_calibration->k_recb_min_rel_velocity, RECW_MAX_K_RECB_MIN_REL_VELOCITY);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_MIN_REL_VELOCITY_HYS, p_calibration->k_recb_min_rel_velocity_hys, RECW_MAX_K_RECB_MIN_REL_VELOCITY_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECB_NOMINAL_ACCELERATION_APPLIED, p_calibration->k_recb_nominal_acceleration_applied, RECW_MAX_K_RECB_NOMINAL_ACCELERATION_APPLIED);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recb_qualifier_nominal_acceleration, RECW_MAX_K_RECB_QUALIFIER_NOMINAL_ACCELERATION);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recb_ssm_braking, RECW_MAX_K_RECB_SSM_BRAKING);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recb_ssm_request_cancelled, RECW_MAX_K_RECB_SSM_REQUEST_CANCELLED);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_AVERAGE_SENSOR_LATENCY, p_calibration->k_recw_average_sensor_latency, RECW_MAX_K_RECW_AVERAGE_SENSOR_LATENCY);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recw_en_active_car_wash_logic, RECW_MAX_K_RECW_EN_ACTIVE_CAR_WASH_LOGIC);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_APPLY_LANE_FILTER, p_calibration->k_recw_f_apply_lane_filter, RECW_MAX_K_RECW_F_APPLY_LANE_FILTER);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_ENABLE_HEADING_FILTER, p_calibration->k_recw_f_enable_heading_filter, RECW_MAX_K_RECW_F_ENABLE_HEADING_FILTER);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_ENABLE_TRAFFIC_LIGHT_GHOST_DETECTION, p_calibration->k_recw_f_enable_traffic_light_ghost_detection, RECW_MAX_K_RECW_F_ENABLE_TRAFFIC_LIGHT_GHOST_DETECTION);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_MAKE_USE_OF_GUARDRAIL, p_calibration->k_recw_f_make_use_of_guardrail, RECW_MAX_K_RECW_F_MAKE_USE_OF_GUARDRAIL);
Ct_Is_Bool_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_F_ONLY_ALLOW_CONSECUTIVE_ALERT_LEVELS, p_calibration->k_recw_f_only_allow_consecutive_alert_levels, RECW_MAX_K_RECW_F_ONLY_ALLOW_CONSECUTIVE_ALERT_LEVELS);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recw_f_suppress_alert_lvl_2_for_pedestrian, RECW_MAX_K_RECW_F_SUPPRESS_ALERT_LVL_2_FOR_PEDESTRIAN);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_FACTOR_EGO_WIDTH, p_calibration->k_recw_factor_ego_width, RECW_MAX_K_RECW_FACTOR_EGO_WIDTH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_HEADING_ACCURACY_THRESHOLD, p_calibration->k_recw_heading_accuracy_threshold, RECW_MAX_K_RECW_HEADING_ACCURACY_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LANE_FILTER_MAX_ABS_EGO_SPEED_VCS_COORD, p_calibration->k_recw_lane_filter_max_abs_ego_speed_vcs_coord, RECW_MAX_K_RECW_LANE_FILTER_MAX_ABS_EGO_SPEED_VCS_COORD);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recw_lane_filter_num_consecutive_cycles, RECW_MAX_K_RECW_LANE_FILTER_NUM_CONSECUTIVE_CYCLES);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LANE_FILTER_WIDTH, p_calibration->k_recw_lane_filter_width, RECW_MAX_K_RECW_LANE_FILTER_WIDTH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LANE_FILTER_WIDTH_HYS, p_calibration->k_recw_lane_filter_width_hys, RECW_MAX_K_RECW_LANE_FILTER_WIDTH_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_LANE_WIDTH_SLOPE, p_calibration->k_recw_lane_width_slope, RECW_MAX_K_RECW_LANE_WIDTH_SLOPE);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_ALLOWED_HEADING_DIFF, p_calibration->k_recw_max_allowed_heading_diff, RECW_MAX_K_RECW_MAX_ALLOWED_HEADING_DIFF);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_ALLOWED_REL_VEL_LAT_DIFF, p_calibration->k_recw_max_allowed_rel_vel_lat_diff, RECW_MAX_K_RECW_MAX_ALLOWED_REL_VEL_LAT_DIFF);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_ALLOWED_REL_VEL_LONG_DIFF, p_calibration->k_recw_max_allowed_rel_vel_long_diff, RECW_MAX_K_RECW_MAX_ALLOWED_REL_VEL_LONG_DIFF);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_ECLIPSE_VALUE_FOR_VALID_OBJECT, p_calibration->k_recw_max_eclipse_value_for_valid_object, RECW_MAX_K_RECW_MAX_ECLIPSE_VALUE_FOR_VALID_OBJECT);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_HEADING_HYS, p_calibration->k_recw_max_heading_hys, RECW_MAX_K_RECW_MAX_HEADING_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_HOST_SPEED_HYS, p_calibration->k_recw_max_host_speed_hys, RECW_MAX_K_RECW_MAX_HOST_SPEED_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_LAT_DISTANCE_CAR_WASH, p_calibration->k_recw_max_lat_distance_car_wash, RECW_MAX_K_RECW_MAX_LAT_DISTANCE_CAR_WASH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_LON_DISTANCE_CAR_WASH, p_calibration->k_recw_max_lon_distance_car_wash, RECW_MAX_K_RECW_MAX_LON_DISTANCE_CAR_WASH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_OBJECT_WIDTH_WARN_ON, p_calibration->k_recw_max_object_width_warn_on, RECW_MAX_K_RECW_MAX_OBJECT_WIDTH_WARN_ON);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_REL_LON_VEL_RELEASE_CAR_WASH, p_calibration->k_recw_max_rel_lon_vel_release_car_wash, RECW_MAX_K_RECW_MAX_REL_LON_VEL_RELEASE_CAR_WASH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_REL_VELOCITY_HYS, p_calibration->k_recw_max_rel_velocity_hys, RECW_MAX_K_RECW_MAX_REL_VELOCITY_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MAX_SPEED_EGO_CAR_WASH, p_calibration->k_recw_max_speed_ego_car_wash, RECW_MAX_K_RECW_MAX_SPEED_EGO_CAR_WASH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_ABS_SPEED_FOR_YOUNG_CLOSE_TARGETS, p_calibration->k_recw_min_abs_speed_for_young_close_targets, RECW_MAX_K_RECW_MIN_ABS_SPEED_FOR_YOUNG_CLOSE_TARGETS);
Ct_Is_Uint8_In_Bondaries(&f_recw_calibration_in_boundaries, 0, p_calibration->k_recw_min_age_for_close_slow_targets, RECW_MAX_K_RECW_MIN_AGE_FOR_CLOSE_SLOW_TARGETS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_DIST_FOR_YOUNG_SLOW_TARGETS, p_calibration->k_recw_min_dist_for_young_slow_targets, RECW_MAX_K_RECW_MIN_DIST_FOR_YOUNG_SLOW_TARGETS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_EXISTENCE_PROB_HYS, p_calibration->k_recw_min_existence_prob_hys, RECW_MAX_K_RECW_MIN_EXISTENCE_PROB_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_HOST_SPEED_HYS, p_calibration->k_recw_min_host_speed_hys, RECW_MAX_K_RECW_MIN_HOST_SPEED_HYS);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_REL_LON_VEL_CAR_WASH, p_calibration->k_recw_min_rel_lon_vel_car_wash, RECW_MAX_K_RECW_MIN_REL_LON_VEL_CAR_WASH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_MIN_SPEED_NOT_STATIONARY, p_calibration->k_recw_min_speed_not_stationary, RECW_MAX_K_RECW_MIN_SPEED_NOT_STATIONARY);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_REAR_BLOCKAGE_EGO_SPEED_THRESHOLD, p_calibration->k_recw_rear_blockage_ego_speed_threshold, RECW_MAX_K_RECW_REAR_BLOCKAGE_EGO_SPEED_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_REAR_BLOCKAGE_LENGTH, p_calibration->k_recw_rear_blockage_length, RECW_MAX_K_RECW_REAR_BLOCKAGE_LENGTH);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_REAR_BLOCKAGE_SPEED_THRESHOLD, p_calibration->k_recw_rear_blockage_speed_threshold, RECW_MAX_K_RECW_REAR_BLOCKAGE_SPEED_THRESHOLD);
Ct_Is_Float_In_Bondaries(&f_recw_calibration_in_boundaries, RECW_MIN_K_RECW_REAR_BLOCKAGE_WIDTH, p_calibration->k_recw_rear_blockage_width, RECW_MAX_K_RECW_REAR_BLOCKAGE_WIDTH);


   return f_recw_calibration_in_boundaries;
}

