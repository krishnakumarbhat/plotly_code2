
/**
* @file recw_core_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "recw_core_calibration_t.h"
#include "recw_core_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Recw_Core_Cal_Update_Defaults(Recw_Core_Calibration_T* cal_dst)
{
    Recw_Core_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)400,
   /**<version*/ (uint16_t)16,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)21495,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_recw_min_rel_velocity*/ {(float32_T)2.77f /**< 2.77 m/s | 9.97 km/h */,(float32_T)5.55f /**< 5.55 m/s | 19.98 km/h */},
   /**<k_recw_min_rel_velocity_hys*/ {(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */,(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */},
   /**<k_recw_max_heading*/ {(float32_T)0.15f /**< 0.15 rad | 8.59 deg */,(float32_T)0.15f /**< 0.15 rad | 8.59 deg */,(float32_T)0.15f /**< 0.15 rad | 8.59 deg */},
   /**<k_recw_max_heading_hys*/ (float32_T) 0.01f /**< 0.01 rad | 0.57 deg */,
   /**<k_recw_factor_ego_width*/ (float32_T) 0.75f,
   /**<k_recw_average_sensor_latency*/ (float32_T) 0.0f /**< 0.0 sec */,
   /**<k_recw_min_crash_prob*/ {(float32_T)0.05f,(float32_T)0.99f},
   /**<k_recw_min_existence_prob*/ {(float32_T)0.5f,(float32_T)0.5f},
   /**<k_recw_min_existence_prob_hys*/ (float32_T) 0.1f,
   /**<k_recw_min_ttc_for_alert_level*/ {(float32_T)0.3f /**< 0.3 sec */,(float32_T)0.0f /**< 0.0 sec */},
   /**<k_recw_min_overlap_for_alert_level*/ {(float32_T)0.0f,(float32_T)0.0f},
   /**<k_recw_min_host_speed*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */},
   /**<k_recw_min_host_speed_hys*/ (float32_T) 0.5f /**< 0.5 m/s | 1.8 km/h */,
   /**<k_recw_max_host_speed*/ {(float32_T)70.0f /**< 70.0 m/s | 252.0 km/h */,(float32_T)70.0f /**< 70.0 m/s | 252.0 km/h */,(float32_T)70.0f /**< 70.0 m/s | 252.0 km/h */},
   /**<k_recw_max_host_speed_hys*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_min_rel_velocity_for_max_ttc_threshold*/ {(float32_T)8.33f /**< 8.33 m/s | 29.99 km/h */,(float32_T)5.55f /**< 5.55 m/s | 19.98 km/h */},
   /**<k_recw_max_ttc_threshold*/ {(float32_T)1.4f /**< 1.4 sec */,(float32_T)0.3f /**< 0.3 sec */},
   /**<k_recw_min_dist_for_young_slow_targets*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_recw_min_abs_speed_for_young_close_targets*/ (float32_T) 5.0f /**< 5.0 m/s | 18.0 km/h */,
   /**<k_recw_max_rel_velocity*/ {(float32_T)41.7f /**< 41.7 m/s | 150.12 km/h */,(float32_T)41.7f /**< 41.7 m/s | 150.12 km/h */,(float32_T)41.7f /**< 41.7 m/s | 150.12 km/h */},
   /**<k_recw_max_rel_velocity_hys*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_rel_lon_vel_release_car_wash*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_lon_distance_car_wash*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_recw_max_lat_distance_car_wash*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_recw_min_rel_lon_vel_car_wash*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_speed_ego_car_wash*/ (float32_T) 3.0f /**< 3.0 m/s | 10.8 km/h */,
   /**<k_recw_lane_filter_width*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_recw_lane_filter_width_hys*/ (float32_T) 0.2f /**< 0.2 m */,
   /**<k_recw_lane_filter_max_abs_ego_speed_vcs_coord*/ (float32_T) 2.0f /**< 2.0 m/s | 7.2 km/h */,
   /**<k_recw_lane_width_slope*/ (float32_T) 0.03f /**< 0.03 m */,
   /**<k_recw_lookup_braking_deceleration*/ {(float32_T)0.0 /**< 0.0 m/s**2 */,(float32_T)2.0 /**< 2.0 m/s**2 */,(float32_T)5.0 /**< 5.0 m/s**2 */,(float32_T)8.0 /**< 8.0 m/s**2 */,(float32_T)9.0 /**< 9.0 m/s**2 */,(float32_T)13.0 /**< 13.0 m/s**2 */},
   /**<k_recw_lookup_braking_probability*/ {(float32_T)0.0,(float32_T)0.2,(float32_T)0.5,(float32_T)0.95,(float32_T)0.99,(float32_T)1.0},
   /**<k_recw_lookup_steering_acceleration*/ {(float32_T)0.0 /**< 0.0 m/s**2 */,(float32_T)1.3 /**< 1.3 m/s**2 */,(float32_T)4.0 /**< 4.0 m/s**2 */,(float32_T)8.0 /**< 8.0 m/s**2 */,(float32_T)10.0 /**< 10.0 m/s**2 */,(float32_T)13.0 /**< 13.0 m/s**2 */},
   /**<k_recw_lookup_steering_probability*/ {(float32_T)0.0,(float32_T)0.2,(float32_T)0.5,(float32_T)0.9,(float32_T)0.99,(float32_T)1.0},
   /**<k_recw_max_eclipse_value_for_valid_object*/ (float32_T) 0.5f,
   /**<k_recw_max_object_width_warn_on*/ (float32_T) 4.0f /**< 4.0 m */,
   /**<k_recw_max_allowed_rel_vel_long_diff*/ (float32_T) 4.5f /**< 4.5 m/s | 16.2 km/h */,
   /**<k_recw_max_allowed_rel_vel_lat_diff*/ (float32_T) 2.0f /**< 2.0 m/s | 7.2 km/h */,
   /**<k_recw_max_allowed_heading_diff*/ (float32_T) 0.12f /**< 0.12 rad | 6.88 deg */,
   /**<k_recw_rear_blockage_speed_threshold*/ (float32_T) 2.0f /**< 2.0 m/s | 7.2 km/h */,
   /**<k_recw_rear_blockage_width*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_recw_rear_blockage_length*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_recw_rear_blockage_ego_speed_threshold*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_recw_heading_accuracy_threshold*/ (float32_T) 0.5f,
   /**<k_recw_min_speed_not_stationary*/ (float32_T) 1.5f /**< 1.5 m/s | 5.4 km/h */,
   /**<k_recb_max_host_speed*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recb_max_host_speed_hys*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recb_max_velocity_host_standstill*/ (float32_T) 0.3f /**< 0.3 m/s | 1.08 km/h */,
   /**<k_recb_min_rel_velocity*/ (float32_T) 5.55f /**< 5.55 m/s | 19.98 km/h */,
   /**<k_recb_min_rel_velocity_hys*/ (float32_T) 0.83f /**< 0.83 m/s | 2.99 km/h */,
   /**<k_recb_max_ttc*/ (float32_T) 0.3f /**< 0.3 s */,
   /**<k_recb_nominal_acceleration_applied*/ (float32_T) -10.0f /**< -10.0 m/s**2 */,
   /**<k_recb_accelerator_pedal_gradient_threshold*/ (float32_T) 200.0f /**< 200.0 percent/s */,
   /**<k_recw_f_make_use_of_guardrail*/ (boolean_T) 1u,
   /**<k_recw_f_only_allow_consecutive_alert_levels*/ (boolean_T) 1u,
   /**<k_recw_f_allow_alert_on_coasted_objects*/ {(boolean_T)0u,(boolean_T)0u},
   /**<k_recw_f_use_rear_blockage*/ {(boolean_T)1u,(boolean_T)1u},
   /**<k_recw_f_apply_lane_filter*/ (boolean_T) 1u,
   /**<k_recw_f_enable_heading_filter*/ (boolean_T) 0u,
   /**<k_recw_f_enable_traffic_light_ghost_detection*/ (boolean_T) 1u,
   /**<k_recb_f_enable_recb*/ (boolean_T) 0u,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_recw_alert_holding_cycles*/ {(uint8_t)10u /**< 10 cycles */,(uint8_t)3u /**< 3 cycles */},
   /**<k_recw_alert_qualifying_cycles*/ (uint8_t) 0 /**< 0 cycles */,
   /**<k_recw_min_stage_age_for_alert_level*/ {(uint8_t)6u,(uint8_t)0u},
   /**<k_recw_max_cycles_alert_duration*/ {(uint8_t)60u /**< 60 cycles */,(uint8_t)60u /**< 60 cycles */},
   /**<k_recw_min_age_for_close_slow_targets*/ (uint8_t) 20u /**< 20 cycles */,
   /**<k_recw_min_object_age*/ (uint8_t) 10u /**< 10 cycles */,
   /**<k_recw_min_cycles_with_min_crash_prob*/ (uint8_t) 3u /**< 3 cycles */,
   /**<k_recw_en_active_car_wash_logic*/ (uint8_t) 0u,
   /**<k_recw_lane_filter_num_consecutive_cycles*/ (uint8_t) 5u /**< 5 cycles */,
   /**<k_recw_max_allowed_consecutive_coasted_cycles*/ (uint8_t) 4u /**< 4 cycles */,
   /**<k_recw_rear_blockage_qualifying_cycles*/ (uint8_t) 3 /**< 3 cycles */,
   /**<k_recb_min_cycles_host_standstill*/ (uint8_t) 2u /**< 2 cycles */,
   /**<k_recb_ssm_braking*/ (uint8_t) 7u,
   /**<k_recb_ssm_request_cancelled*/ (uint8_t) 2u,
   /**<k_recb_integrity*/ (uint8_t) 0u,
   /**<k_recb_qualifier_nominal_acceleration*/ (uint8_t) 6u,
   /**<k_recb_max_cycles_braking_request_duration*/ (uint8_t) 4u /**< 4 cycles */,
   /**<k_recw_f_suppress_alert_lvl_2_for_pedestrian*/ (uint8_t) 1u /**< 1 boolean_T */
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_recw_f_suppress_alert_lvl_2_for_pedestrian*/ (uint8_t) 1u /**< 1 boolean_T */,
   /**<k_recb_max_cycles_braking_request_duration*/ (uint8_t) 4u /**< 4 cycles */,
   /**<k_recb_qualifier_nominal_acceleration*/ (uint8_t) 6u,
   /**<k_recb_integrity*/ (uint8_t) 0u,
   /**<k_recb_ssm_request_cancelled*/ (uint8_t) 2u,
   /**<k_recb_ssm_braking*/ (uint8_t) 7u,
   /**<k_recb_min_cycles_host_standstill*/ (uint8_t) 2u /**< 2 cycles */,
   /**<k_recw_rear_blockage_qualifying_cycles*/ (uint8_t) 3 /**< 3 cycles */,
   /**<k_recw_max_allowed_consecutive_coasted_cycles*/ (uint8_t) 4u /**< 4 cycles */,
   /**<k_recw_lane_filter_num_consecutive_cycles*/ (uint8_t) 5u /**< 5 cycles */,
   /**<k_recw_en_active_car_wash_logic*/ (uint8_t) 0u,
   /**<k_recw_min_cycles_with_min_crash_prob*/ (uint8_t) 3u /**< 3 cycles */,
   /**<k_recw_min_object_age*/ (uint8_t) 10u /**< 10 cycles */,
   /**<k_recw_min_age_for_close_slow_targets*/ (uint8_t) 20u /**< 20 cycles */,
   /**<k_recw_max_cycles_alert_duration*/ {(uint8_t)60u /**< 60 cycles */,(uint8_t)60u /**< 60 cycles */},
   /**<k_recw_min_stage_age_for_alert_level*/ {(uint8_t)6u,(uint8_t)0u},
   /**<k_recw_alert_qualifying_cycles*/ (uint8_t) 0 /**< 0 cycles */,
   /**<k_recw_alert_holding_cycles*/ {(uint8_t)10u /**< 10 cycles */,(uint8_t)3u /**< 3 cycles */},
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_recb_f_enable_recb*/ (boolean_T) 0u,
   /**<k_recw_f_enable_traffic_light_ghost_detection*/ (boolean_T) 1u,
   /**<k_recw_f_enable_heading_filter*/ (boolean_T) 0u,
   /**<k_recw_f_apply_lane_filter*/ (boolean_T) 1u,
   /**<k_recw_f_use_rear_blockage*/ {(boolean_T)1u,(boolean_T)1u},
   /**<k_recw_f_allow_alert_on_coasted_objects*/ {(boolean_T)0u,(boolean_T)0u},
   /**<k_recw_f_only_allow_consecutive_alert_levels*/ (boolean_T) 1u,
   /**<k_recw_f_make_use_of_guardrail*/ (boolean_T) 1u,
   /**<k_recb_accelerator_pedal_gradient_threshold*/ (float32_T) 200.0f /**< 200.0 percent/s */,
   /**<k_recb_nominal_acceleration_applied*/ (float32_T) -10.0f /**< -10.0 m/s**2 */,
   /**<k_recb_max_ttc*/ (float32_T) 0.3f /**< 0.3 s */,
   /**<k_recb_min_rel_velocity_hys*/ (float32_T) 0.83f /**< 0.83 m/s | 2.99 km/h */,
   /**<k_recb_min_rel_velocity*/ (float32_T) 5.55f /**< 5.55 m/s | 19.98 km/h */,
   /**<k_recb_max_velocity_host_standstill*/ (float32_T) 0.3f /**< 0.3 m/s | 1.08 km/h */,
   /**<k_recb_max_host_speed_hys*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recb_max_host_speed*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_min_speed_not_stationary*/ (float32_T) 1.5f /**< 1.5 m/s | 5.4 km/h */,
   /**<k_recw_heading_accuracy_threshold*/ (float32_T) 0.5f,
   /**<k_recw_rear_blockage_ego_speed_threshold*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_recw_rear_blockage_length*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_recw_rear_blockage_width*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_recw_rear_blockage_speed_threshold*/ (float32_T) 2.0f /**< 2.0 m/s | 7.2 km/h */,
   /**<k_recw_max_allowed_heading_diff*/ (float32_T) 0.12f /**< 0.12 rad | 6.88 deg */,
   /**<k_recw_max_allowed_rel_vel_lat_diff*/ (float32_T) 2.0f /**< 2.0 m/s | 7.2 km/h */,
   /**<k_recw_max_allowed_rel_vel_long_diff*/ (float32_T) 4.5f /**< 4.5 m/s | 16.2 km/h */,
   /**<k_recw_max_object_width_warn_on*/ (float32_T) 4.0f /**< 4.0 m */,
   /**<k_recw_max_eclipse_value_for_valid_object*/ (float32_T) 0.5f,
   /**<k_recw_lookup_steering_probability*/ {(float32_T)0.0,(float32_T)0.2,(float32_T)0.5,(float32_T)0.9,(float32_T)0.99,(float32_T)1.0},
   /**<k_recw_lookup_steering_acceleration*/ {(float32_T)0.0 /**< 0.0 m/s**2 */,(float32_T)1.3 /**< 1.3 m/s**2 */,(float32_T)4.0 /**< 4.0 m/s**2 */,(float32_T)8.0 /**< 8.0 m/s**2 */,(float32_T)10.0 /**< 10.0 m/s**2 */,(float32_T)13.0 /**< 13.0 m/s**2 */},
   /**<k_recw_lookup_braking_probability*/ {(float32_T)0.0,(float32_T)0.2,(float32_T)0.5,(float32_T)0.95,(float32_T)0.99,(float32_T)1.0},
   /**<k_recw_lookup_braking_deceleration*/ {(float32_T)0.0 /**< 0.0 m/s**2 */,(float32_T)2.0 /**< 2.0 m/s**2 */,(float32_T)5.0 /**< 5.0 m/s**2 */,(float32_T)8.0 /**< 8.0 m/s**2 */,(float32_T)9.0 /**< 9.0 m/s**2 */,(float32_T)13.0 /**< 13.0 m/s**2 */},
   /**<k_recw_lane_width_slope*/ (float32_T) 0.03f /**< 0.03 m */,
   /**<k_recw_lane_filter_max_abs_ego_speed_vcs_coord*/ (float32_T) 2.0f /**< 2.0 m/s | 7.2 km/h */,
   /**<k_recw_lane_filter_width_hys*/ (float32_T) 0.2f /**< 0.2 m */,
   /**<k_recw_lane_filter_width*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_recw_max_speed_ego_car_wash*/ (float32_T) 3.0f /**< 3.0 m/s | 10.8 km/h */,
   /**<k_recw_min_rel_lon_vel_car_wash*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_lat_distance_car_wash*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_recw_max_lon_distance_car_wash*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_recw_max_rel_lon_vel_release_car_wash*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_rel_velocity_hys*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_rel_velocity*/ {(float32_T)41.7f /**< 41.7 m/s | 150.12 km/h */,(float32_T)41.7f /**< 41.7 m/s | 150.12 km/h */,(float32_T)41.7f /**< 41.7 m/s | 150.12 km/h */},
   /**<k_recw_min_abs_speed_for_young_close_targets*/ (float32_T) 5.0f /**< 5.0 m/s | 18.0 km/h */,
   /**<k_recw_min_dist_for_young_slow_targets*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_recw_max_ttc_threshold*/ {(float32_T)1.4f /**< 1.4 sec */,(float32_T)0.3f /**< 0.3 sec */},
   /**<k_recw_min_rel_velocity_for_max_ttc_threshold*/ {(float32_T)8.33f /**< 8.33 m/s | 29.99 km/h */,(float32_T)5.55f /**< 5.55 m/s | 19.98 km/h */},
   /**<k_recw_max_host_speed_hys*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_recw_max_host_speed*/ {(float32_T)70.0f /**< 70.0 m/s | 252.0 km/h */,(float32_T)70.0f /**< 70.0 m/s | 252.0 km/h */,(float32_T)70.0f /**< 70.0 m/s | 252.0 km/h */},
   /**<k_recw_min_host_speed_hys*/ (float32_T) 0.5f /**< 0.5 m/s | 1.8 km/h */,
   /**<k_recw_min_host_speed*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */},
   /**<k_recw_min_overlap_for_alert_level*/ {(float32_T)0.0f,(float32_T)0.0f},
   /**<k_recw_min_ttc_for_alert_level*/ {(float32_T)0.3f /**< 0.3 sec */,(float32_T)0.0f /**< 0.0 sec */},
   /**<k_recw_min_existence_prob_hys*/ (float32_T) 0.1f,
   /**<k_recw_min_existence_prob*/ {(float32_T)0.5f,(float32_T)0.5f},
   /**<k_recw_min_crash_prob*/ {(float32_T)0.05f,(float32_T)0.99f},
   /**<k_recw_average_sensor_latency*/ (float32_T) 0.0f /**< 0.0 sec */,
   /**<k_recw_factor_ego_width*/ (float32_T) 0.75f,
   /**<k_recw_max_heading_hys*/ (float32_T) 0.01f /**< 0.01 rad | 0.57 deg */,
   /**<k_recw_max_heading*/ {(float32_T)0.15f /**< 0.15 rad | 8.59 deg */,(float32_T)0.15f /**< 0.15 rad | 8.59 deg */,(float32_T)0.15f /**< 0.15 rad | 8.59 deg */},
   /**<k_recw_min_rel_velocity_hys*/ {(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */,(float32_T)0.83f /**< 0.83 m/s | 2.99 km/h */},
   /**<k_recw_min_rel_velocity*/ {(float32_T)2.77f /**< 2.77 m/s | 9.97 km/h */,(float32_T)5.55f /**< 5.55 m/s | 19.98 km/h */},
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)21495,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)16,
   /**<Section_Size*/ (uint32_t)400
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Recw_Core_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Recw_Core_Cal_Reverse_Array_Recw_Cal(Recw_Core_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_rel_velocity[0], sizeof(cal_dst->k_recw_min_rel_velocity), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_rel_velocity_hys[0], sizeof(cal_dst->k_recw_min_rel_velocity_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_max_heading[0], sizeof(cal_dst->k_recw_max_heading), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_crash_prob[0], sizeof(cal_dst->k_recw_min_crash_prob), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_existence_prob[0], sizeof(cal_dst->k_recw_min_existence_prob), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_ttc_for_alert_level[0], sizeof(cal_dst->k_recw_min_ttc_for_alert_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_overlap_for_alert_level[0], sizeof(cal_dst->k_recw_min_overlap_for_alert_level), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_host_speed[0], sizeof(cal_dst->k_recw_min_host_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_max_host_speed[0], sizeof(cal_dst->k_recw_max_host_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_rel_velocity_for_max_ttc_threshold[0], sizeof(cal_dst->k_recw_min_rel_velocity_for_max_ttc_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_max_ttc_threshold[0], sizeof(cal_dst->k_recw_max_ttc_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_max_rel_velocity[0], sizeof(cal_dst->k_recw_max_rel_velocity), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_lookup_braking_deceleration[0], sizeof(cal_dst->k_recw_lookup_braking_deceleration), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_lookup_braking_probability[0], sizeof(cal_dst->k_recw_lookup_braking_probability), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_lookup_steering_acceleration[0], sizeof(cal_dst->k_recw_lookup_steering_acceleration), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_lookup_steering_probability[0], sizeof(cal_dst->k_recw_lookup_steering_probability), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_f_allow_alert_on_coasted_objects[0], sizeof(cal_dst->k_recw_f_allow_alert_on_coasted_objects), CT_ONE_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_f_use_rear_blockage[0], sizeof(cal_dst->k_recw_f_use_rear_blockage), CT_ONE_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_alert_holding_cycles[0], sizeof(cal_dst->k_recw_alert_holding_cycles), CT_ONE_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_min_stage_age_for_alert_level[0], sizeof(cal_dst->k_recw_min_stage_age_for_alert_level), CT_ONE_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_recw_max_cycles_alert_duration[0], sizeof(cal_dst->k_recw_max_cycles_alert_duration), CT_ONE_BYTE);
}
#endif /* CT_BIG_ENDIAN */


