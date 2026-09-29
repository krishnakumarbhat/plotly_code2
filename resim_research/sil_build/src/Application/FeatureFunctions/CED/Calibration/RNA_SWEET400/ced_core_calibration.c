
/**
* @file ced_core_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the RNA_SWEET400 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ced_core_calibration_t.h"
#include "ced_core_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ced_Core_Cal_Update_Defaults(Ced_Core_Calibration_T* cal_dst)
{
    Ced_Core_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)320,
   /**<version*/ (uint16_t)42,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)14281,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_ced_object_acceleration_weight*/ (float32_T) 0.0f,
   /**<k_ced_object_heading_predicted_weight*/ (float32_T) 1.0f,
   /**<k_ced_object_existence_probability_min*/ (float32_T) 0.95,
   /**<k_ced_object_heading_abs_angle_max*/ (float32_T) 0.6 /**< 0.6 rad | 34.38 deg */,
   /**<k_ced_object_long_vel_rel_min*/ (float32_T) 0.0 /**< 0.0 m/s | 0.0 km/h */,
   /**<k_ced_object_long_vel_min*/ (float32_T) 1.95 /**< 1.95 m/s | 7.02 km/h */,
   /**<k_ced_object_lat_vel_max*/ (float32_T) 10.0 /**< 10.0 m/s | 36.0 km/h */,
   /**<k_ced_object_max_width_increase_factor_with_path_match*/ (float32_T) 0.0,
   /**<k_ced_object_max_width_increase_factor_without_path_match*/ (float32_T) 1.5,
   /**<k_ced_object_heading_exp_moving_average_alpha*/ (float32_T) 0.5,
   /**<k_ced_object_ftm_existence_probability_min*/ (float32_T) 0.9f,
   /**<k_ced_object_ftm_heading_abs_angle_min*/ (float32_T) 2.75f /**< 2.75 rad | 157.56 deg */,
   /**<k_ced_object_ftm_long_vel_rel_min*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_ced_object_ftm_long_vel_min*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_ced_object_ftm_lat_vel_max*/ (float32_T) 10.0f /**< 10.0 m/s | 36.0 km/h */,
   /**<k_ced_first_warning_ttc_threshold*/ {(float32_T)3.15f /**< 3.15 sec */,(float32_T)3.15f /**< 3.15 sec */},
   /**<k_ced_second_warning_ttc_threshold*/ {(float32_T)1.0f /**< 1.0 sec */,(float32_T)1.0f /**< 1.0 sec */},
   /**<k_ced_third_warning_ttc_threshold*/ {(float32_T)1.5f /**< 1.5 sec */,(float32_T)1.5f /**< 1.5 sec */},
   /**<k_ced_honda_srr6_custom_ttc_alert_threshold*/ {(float32_T)20.0f /**< 20.0 sec */,(float32_T)20.0f /**< 20.0 sec */},
   /**<k_ced_honda_srr6_custom_ttc_alert_hysteresis*/ {(float32_T)0.25f /**< 0.25 sec */,(float32_T)0.25f /**< 0.25 sec */},
   /**<k_ced_honda_srr6_long_dist_threshold*/ {(float32_T)40.0f /**< 40.0 m */,(float32_T)40.0f /**< 40.0 m */},
   /**<k_ced_second_warning_pred_lat_dist_max*/ (float32_T) 2.3f /**< 2.3 m */,
   /**<k_ced_third_warning_pred_lat_dist_max*/ (float32_T) 1.5f /**< 1.5 m */,
   /**<k_ced_alert_ttp_min*/ {(float32_T)0.0f /**< 0.0 s */,(float32_T)0.0f /**< 0.0 s */},
   /**<k_ced_ego_abs_speed_max*/ (float32_T) 10 /**< 10 m/s | 36.0 km/h */,
   /**<k_ced_crash_line_host_length_percentage*/ {(float32_T)1.0f,(float32_T)1.0f},
   /**<k_ced_object_width_safety_margin_for_active_alert*/ (float32_T) 0.30f /**< 0.30 m */,
   /**<k_ced_object_width_safety_margin_for_critical_path_match*/ (float32_T) 0.30f /**< 0.30 m */,
   /**<k_ced_object_min_dist_to_crash_line_for_path_match*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_ced_offset_to_path_weight*/ (float32_T) 1.0f /**< 1.0 % */,
   /**<k_ced_collision_zone_width*/ (float32_T) 2.3 /**< 2.3 m */,
   /**<k_ced_funnel_zone_length*/ (float32_T) 60.0 /**< 60.0 m */,
   /**<k_ced_funnel_zone_width*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_ced_slow_objects_long_vel_max*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_ced_ego_lane_width*/ (float32_T) 1.8 /**< 1.8 m */,
   /**<k_ced_ego_lane_parking_range*/ (float32_T) 20.0f /**< 20.0 m */,
   /**<k_ced_ego_lane_parking_maneuver_speed*/ (float32_T) 4.5f /**< 4.5 m/s | 16.2 km/h */,
   /**<k_ced_alert_holding_obj_abs_heading_max*/ (float32_T) 0.4f /**< 0.4 rad | 22.92 deg */,
   /**<k_ced_alert_holding_obj_long_vel_min*/ (float32_T) 1.56f /**< 1.56 m/s | 5.62 km/h */,
   /**<k_ced_suppress_pt_heading_diff_ced_alert_max*/ (float32_T) 0.35f /**< 0.35 rad | 20.05 deg */,
   /**<k_ced_suppress_range_to_nearest_path_max*/ (float32_T) 3.5f /**< 3.5 m */,
   /**<k_ced_object_long_vel_rel_max*/ (float32_T) 79.0f /**< 79.0 m/s | 284.4 km/h */,
   /**<k_ced_object_ftm_long_vel_rel_max*/ (float32_T) 79.0f /**< 79.0 m/s | 284.4 km/h */,
   /**<k_ced_object_vel_max*/ (float32_T) 80.0f /**< 80.0 m/s | 288.0 km/h */,
   /**<k_honda_min_alert_duration*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_honda_elatch_zones_width_table*/ {(float32_T)2.0f /**< 2.0 m */,(float32_T)1.3f /**< 1.3 m */},
   /**<k_honda_min_eratch_alert_duration*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ced_slight_turn_lat_vel_table*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */},
   /**<k_ced_slight_turn_position_limits*/ {(float32_T)100.0f,(float32_T)100.0f},
   /**<k_ced_lat_pos_of_border*/ (float32_T) 0u,
   /**<k_ced_lat_pos_max_shift*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ced_lat_pos_shift_long_dist_thresholds*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)5.0f /**< 5.0 m */},
   /**<k_ced_lat_pos_shift_lat_dist_thresholds*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)5.0f /**< 5.0 m */},
   /**<k_bmw_ced_speed_max_hysteresis*/ (float32_T) 2.77778f /**< 2.77778 m/s | 10.0 km/h */,
   /**<k_ced_object_predicted_max_width_slope_reduce_factor*/ (float32_T) 1.0f,
   /**<k_ced_object_predicted_max_width_slope_offset*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ced_lat_pos_shift_width_thresh*/ (float32_T) 1.2f /**< 1.2 m */,
   /**<k_ced_warning_pred_lat_dist_max_histeresis*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ced_f_enable_heading_exp_moving_average*/ (boolean_T) 1u,
   /**<k_ced_f_second_warning_level_enable*/ (boolean_T) 1u,
   /**<k_ced_f_third_warning_level_enable*/ (boolean_T) 0u,
   /**<k_ced_f_suppress_alert_holding_for_uncritical_objects*/ (boolean_T) 1,
   /**<k_ced_f_suppress_alert_holding_for_obj_below_min_ttp*/ (boolean_T) 0,
   /**<k_ced_f_path_tracking_enable*/ (boolean_T) 0,
   /**<k_ced_f_handle_both_side_alerts_as_object_side*/ (boolean_T) 1,
   /**<k_ced_f_allow_ego_lane_alerts*/ (boolean_T) 0,
   /**<k_ced_f_use_only_mature_paths*/ (boolean_T) 0u,
   /**<k_ced_f_allow_coasted_object_alerts*/ (boolean_T) 0,
   /**<k_ced_f_honda_use_alert_ttc_threshold*/ (boolean_T) 0,
   /**<k_ced_f_adapt_heading_ego_lane*/ (boolean_T) 0,
   /**<k_ced_f_object_lat_on_one_side_of_border*/ (boolean_T) 0u,
   /**<k_ced_lat_pos_shift_enable*/ (boolean_T) 0,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_ced_alert_qualifying_cycles*/ (uint8_t) 3,
   /**<k_ced_alert_qualifying_cycles_slow_objects*/ (uint8_t) 6u,
   /**<k_ced_alert_holding_cycles*/ (uint8_t) 20,
   /**<k_ced_allow_opposite_side_alerts*/ (uint8_t) 0,
   /**<k_ced_suppress_alert_object_age_max*/ (uint8_t) 20u,
   /**<k_ced_min_cycles_for_path_match_for_no_suppress*/ (uint8_t) 3u,
   /**<k_ced_object_age_min*/ (uint8_t) 3u,
   /**<k_ced_object_ftm_age_min*/ (uint8_t) 3u,
   /**<k_ced_f_choose_ref_point_funnel_check*/ (uint8_t) 0
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_ced_f_choose_ref_point_funnel_check*/ (uint8_t) 0,
   /**<k_ced_object_ftm_age_min*/ (uint8_t) 3u,
   /**<k_ced_object_age_min*/ (uint8_t) 3u,
   /**<k_ced_min_cycles_for_path_match_for_no_suppress*/ (uint8_t) 3u,
   /**<k_ced_suppress_alert_object_age_max*/ (uint8_t) 20u,
   /**<k_ced_allow_opposite_side_alerts*/ (uint8_t) 0,
   /**<k_ced_alert_holding_cycles*/ (uint8_t) 20,
   /**<k_ced_alert_qualifying_cycles_slow_objects*/ (uint8_t) 6u,
   /**<k_ced_alert_qualifying_cycles*/ (uint8_t) 3,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_ced_lat_pos_shift_enable*/ (boolean_T) 0,
   /**<k_ced_f_object_lat_on_one_side_of_border*/ (boolean_T) 0u,
   /**<k_ced_f_adapt_heading_ego_lane*/ (boolean_T) 0,
   /**<k_ced_f_honda_use_alert_ttc_threshold*/ (boolean_T) 0,
   /**<k_ced_f_allow_coasted_object_alerts*/ (boolean_T) 0,
   /**<k_ced_f_use_only_mature_paths*/ (boolean_T) 0u,
   /**<k_ced_f_allow_ego_lane_alerts*/ (boolean_T) 0,
   /**<k_ced_f_handle_both_side_alerts_as_object_side*/ (boolean_T) 1,
   /**<k_ced_f_path_tracking_enable*/ (boolean_T) 0,
   /**<k_ced_f_suppress_alert_holding_for_obj_below_min_ttp*/ (boolean_T) 0,
   /**<k_ced_f_suppress_alert_holding_for_uncritical_objects*/ (boolean_T) 1,
   /**<k_ced_f_third_warning_level_enable*/ (boolean_T) 0u,
   /**<k_ced_f_second_warning_level_enable*/ (boolean_T) 1u,
   /**<k_ced_f_enable_heading_exp_moving_average*/ (boolean_T) 1u,
   /**<k_ced_warning_pred_lat_dist_max_histeresis*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ced_lat_pos_shift_width_thresh*/ (float32_T) 1.2f /**< 1.2 m */,
   /**<k_ced_object_predicted_max_width_slope_offset*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ced_object_predicted_max_width_slope_reduce_factor*/ (float32_T) 1.0f,
   /**<k_bmw_ced_speed_max_hysteresis*/ (float32_T) 2.77778f /**< 2.77778 m/s | 10.0 km/h */,
   /**<k_ced_lat_pos_shift_lat_dist_thresholds*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)5.0f /**< 5.0 m */},
   /**<k_ced_lat_pos_shift_long_dist_thresholds*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)5.0f /**< 5.0 m */},
   /**<k_ced_lat_pos_max_shift*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ced_lat_pos_of_border*/ (float32_T) 0u,
   /**<k_ced_slight_turn_position_limits*/ {(float32_T)100.0f,(float32_T)100.0f},
   /**<k_ced_slight_turn_lat_vel_table*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */},
   /**<k_honda_min_eratch_alert_duration*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_honda_elatch_zones_width_table*/ {(float32_T)2.0f /**< 2.0 m */,(float32_T)1.3f /**< 1.3 m */},
   /**<k_honda_min_alert_duration*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ced_object_vel_max*/ (float32_T) 80.0f /**< 80.0 m/s | 288.0 km/h */,
   /**<k_ced_object_ftm_long_vel_rel_max*/ (float32_T) 79.0f /**< 79.0 m/s | 284.4 km/h */,
   /**<k_ced_object_long_vel_rel_max*/ (float32_T) 79.0f /**< 79.0 m/s | 284.4 km/h */,
   /**<k_ced_suppress_range_to_nearest_path_max*/ (float32_T) 3.5f /**< 3.5 m */,
   /**<k_ced_suppress_pt_heading_diff_ced_alert_max*/ (float32_T) 0.35f /**< 0.35 rad | 20.05 deg */,
   /**<k_ced_alert_holding_obj_long_vel_min*/ (float32_T) 1.56f /**< 1.56 m/s | 5.62 km/h */,
   /**<k_ced_alert_holding_obj_abs_heading_max*/ (float32_T) 0.4f /**< 0.4 rad | 22.92 deg */,
   /**<k_ced_ego_lane_parking_maneuver_speed*/ (float32_T) 4.5f /**< 4.5 m/s | 16.2 km/h */,
   /**<k_ced_ego_lane_parking_range*/ (float32_T) 20.0f /**< 20.0 m */,
   /**<k_ced_ego_lane_width*/ (float32_T) 1.8 /**< 1.8 m */,
   /**<k_ced_slow_objects_long_vel_max*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_ced_funnel_zone_width*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_ced_funnel_zone_length*/ (float32_T) 60.0 /**< 60.0 m */,
   /**<k_ced_collision_zone_width*/ (float32_T) 2.3 /**< 2.3 m */,
   /**<k_ced_offset_to_path_weight*/ (float32_T) 1.0f /**< 1.0 % */,
   /**<k_ced_object_min_dist_to_crash_line_for_path_match*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_ced_object_width_safety_margin_for_critical_path_match*/ (float32_T) 0.30f /**< 0.30 m */,
   /**<k_ced_object_width_safety_margin_for_active_alert*/ (float32_T) 0.30f /**< 0.30 m */,
   /**<k_ced_crash_line_host_length_percentage*/ {(float32_T)1.0f,(float32_T)1.0f},
   /**<k_ced_ego_abs_speed_max*/ (float32_T) 10 /**< 10 m/s | 36.0 km/h */,
   /**<k_ced_alert_ttp_min*/ {(float32_T)0.0f /**< 0.0 s */,(float32_T)0.0f /**< 0.0 s */},
   /**<k_ced_third_warning_pred_lat_dist_max*/ (float32_T) 1.5f /**< 1.5 m */,
   /**<k_ced_second_warning_pred_lat_dist_max*/ (float32_T) 2.3f /**< 2.3 m */,
   /**<k_ced_honda_srr6_long_dist_threshold*/ {(float32_T)40.0f /**< 40.0 m */,(float32_T)40.0f /**< 40.0 m */},
   /**<k_ced_honda_srr6_custom_ttc_alert_hysteresis*/ {(float32_T)0.25f /**< 0.25 sec */,(float32_T)0.25f /**< 0.25 sec */},
   /**<k_ced_honda_srr6_custom_ttc_alert_threshold*/ {(float32_T)20.0f /**< 20.0 sec */,(float32_T)20.0f /**< 20.0 sec */},
   /**<k_ced_third_warning_ttc_threshold*/ {(float32_T)1.5f /**< 1.5 sec */,(float32_T)1.5f /**< 1.5 sec */},
   /**<k_ced_second_warning_ttc_threshold*/ {(float32_T)1.0f /**< 1.0 sec */,(float32_T)1.0f /**< 1.0 sec */},
   /**<k_ced_first_warning_ttc_threshold*/ {(float32_T)3.15f /**< 3.15 sec */,(float32_T)3.15f /**< 3.15 sec */},
   /**<k_ced_object_ftm_lat_vel_max*/ (float32_T) 10.0f /**< 10.0 m/s | 36.0 km/h */,
   /**<k_ced_object_ftm_long_vel_min*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_ced_object_ftm_long_vel_rel_min*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_ced_object_ftm_heading_abs_angle_min*/ (float32_T) 2.75f /**< 2.75 rad | 157.56 deg */,
   /**<k_ced_object_ftm_existence_probability_min*/ (float32_T) 0.9f,
   /**<k_ced_object_heading_exp_moving_average_alpha*/ (float32_T) 0.5,
   /**<k_ced_object_max_width_increase_factor_without_path_match*/ (float32_T) 1.5,
   /**<k_ced_object_max_width_increase_factor_with_path_match*/ (float32_T) 0.0,
   /**<k_ced_object_lat_vel_max*/ (float32_T) 10.0 /**< 10.0 m/s | 36.0 km/h */,
   /**<k_ced_object_long_vel_min*/ (float32_T) 1.95 /**< 1.95 m/s | 7.02 km/h */,
   /**<k_ced_object_long_vel_rel_min*/ (float32_T) 0.0 /**< 0.0 m/s | 0.0 km/h */,
   /**<k_ced_object_heading_abs_angle_max*/ (float32_T) 0.6 /**< 0.6 rad | 34.38 deg */,
   /**<k_ced_object_existence_probability_min*/ (float32_T) 0.95,
   /**<k_ced_object_heading_predicted_weight*/ (float32_T) 1.0f,
   /**<k_ced_object_acceleration_weight*/ (float32_T) 0.0f,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)14281,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)42,
   /**<Section_Size*/ (uint32_t)320
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Ced_Core_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ced_Core_Cal_Reverse_Array_Ced_Cal(Ced_Core_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_first_warning_ttc_threshold[0], sizeof(cal_dst->k_ced_first_warning_ttc_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_second_warning_ttc_threshold[0], sizeof(cal_dst->k_ced_second_warning_ttc_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_third_warning_ttc_threshold[0], sizeof(cal_dst->k_ced_third_warning_ttc_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold[0], sizeof(cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0], sizeof(cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_honda_srr6_long_dist_threshold[0], sizeof(cal_dst->k_ced_honda_srr6_long_dist_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_alert_ttp_min[0], sizeof(cal_dst->k_ced_alert_ttp_min), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_crash_line_host_length_percentage[0], sizeof(cal_dst->k_ced_crash_line_host_length_percentage), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_honda_elatch_zones_width_table[0], sizeof(cal_dst->k_honda_elatch_zones_width_table), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_slight_turn_lat_vel_table[0], sizeof(cal_dst->k_ced_slight_turn_lat_vel_table), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_slight_turn_position_limits[0], sizeof(cal_dst->k_ced_slight_turn_position_limits), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_lat_pos_shift_long_dist_thresholds[0], sizeof(cal_dst->k_ced_lat_pos_shift_long_dist_thresholds), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_lat_pos_shift_lat_dist_thresholds[0], sizeof(cal_dst->k_ced_lat_pos_shift_lat_dist_thresholds), CT_FOUR_BYTE);
}
#endif /* CT_BIG_ENDIAN */


