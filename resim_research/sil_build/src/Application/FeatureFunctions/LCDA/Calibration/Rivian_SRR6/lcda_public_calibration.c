
/**
* @file lcda_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the Rivian_SRR6 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "lcda_public_calibration_t.h"
#include "lcda_public_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Lcda_Public_Cal_Update_Defaults(Lcda_Public_Calibration_T* cal_dst)
{
    Lcda_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)91,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_lcda_min_exist_prop*/ (float32_T) 0.7f,
   /**<k_lcda_host_activation_speed_min*/ (float32_T) 2.8f /**< 2.8 m/s | 10.08 km/h */,
   /**<k_lcda_host_activation_speed_min_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_lcda_host_activation_speed_max*/ (float32_T) 48.61111f /**< 48.61111 m/s | 175.0 km/h */,
   /**<k_lcda_host_activation_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_lcda_distance_traveled_scale_factor*/ (float32_T) 0.6f,
   /**<k_lcda_min_curve_radius*/ (float32_T) 100.0f /**< 100.0 m */,
   /**<k_lcda_min_curve_radius_hys*/ (float32_T) 25.0f /**< 25.0 m */,
   /**<k_lcda_curve_radius_threshold_for_zone_adaptation*/ (float32_T) 100 /**< 100 m */,
   /**<k_lcda_min_lane_width*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_lcda_max_lane_width*/ (float32_T) 7.0 /**< 7.0 m */,
   /**<k_lcda_min_ego_vehicle_width*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_lcda_max_ego_vehicle_width*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_lcda_min_ego_vehicle_length*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_lcda_max_ego_vehicle_length*/ (float32_T) 8.0 /**< 8.0 m */,
   /**<k_lcda_zone_intersect_critical_point_lateral_ratio*/ (float32_T) 1.0,
   /**<k_lcda_exist_prob_lc_intention_hys_offset*/ (float32_T) 0.2f,
   /**<k_lcda_exist_prob_hys_offset*/ (float32_T) 0.1,
   /**<k_lcda_lane_change_intention_vel_lat_thresh*/ (float32_T) 0.5,
   /**<k_bsw_lane_change_intention_pos_lat_thres*/ (float32_T) 0.3,
   /**<k_bsw_lane_change_intention_pos_long_thres*/ (float32_T) 5,
   /**<k_lcda_ego_lane_effective_lane_width_factor*/ (float32_T) 0.95,
   /**<k_lcda_min_exist_prob_lc_intention*/ (float32_T) 0.2f,
   /**<k_bsw_zone_x*/ {(float32_T)0.8 /**< 0.8 m */,(float32_T)0 /**< 0 m */,(float32_T)-3 /**< -3 m */,(float32_T)-3 /**< -3 m */,(float32_T)0 /**< 0 m */,(float32_T)0.8 /**< 0.8 m */},
   /**<k_bsw_zone_y*/ {(float32_T)1.5 /**< 1.5 m */,(float32_T)1.5 /**< 1.5 m */,(float32_T)1.5 /**< 1.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_bsw_zone_x_hys*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)1.0 /**< 1.0 m */},
   /**<k_bsw_zone_y_hys*/ {(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */},
   /**<k_bsw_zone_y_hys_min*/ (float32_T) 0.1 /**< 0.1 m */,
   /**<k_bsw_zone_y_hys_max*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_bsw_fixed_zone_x*/ {(float32_T)0.80f /**< 0.80 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-3.0f /**< -3.0 m */,(float32_T)-3.0f /**< -3.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)0.8f /**< 0.8 m */},
   /**<k_bsw_fixed_zone_y*/ {(float32_T)5.4f /**< 5.4 m */,(float32_T)5.4f /**< 5.4 m */,(float32_T)5.4f /**< 5.4 m */,(float32_T)1.75f /**< 1.75 m */,(float32_T)1.75f /**< 1.75 m */,(float32_T)1.75f /**< 1.75 m */},
   /**<k_bsw_fixed_zone_x_hys*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_bsw_fixed_zone_y_hys*/ {(float32_T)0.4f /**< 0.4 m */,(float32_T)0.4f /**< 0.4 m */,(float32_T)0.4f /**< 0.4 m */,(float32_T)-0.4f /**< -0.4 m */,(float32_T)-0.4f /**< -0.4 m */,(float32_T)-0.4f /**< -0.4 m */},
   /**<k_bsw_lateral_distance_zone*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_overlap_area_threshold*/ (float32_T) 0.0f /**< 0.0 % */,
   /**<k_bsw_fallback_rel_vel_thres*/ (float32_T) -5.55f /**< -5.55 m/s | -19.98 km/h */,
   /**<k_bsw_fallback_rel_vel_thres_hys*/ (float32_T) 10 /**< 10 m/s | 36.0 km/h */,
   /**<k_bsw_dynzone_speed*/ {(float32_T)0 /**< 0 m/s | 0.0 km/h */,(float32_T)17.36 /**< 17.36 m/s | 62.5 km/h */,(float32_T)34.72 /**< 34.72 m/s | 124.99 km/h */,(float32_T)52.08 /**< 52.08 m/s | 187.49 km/h */,(float32_T)69.45 /**< 69.45 m/s | 250.02 km/h */,(float32_T)100 /**< 100 m/s | 360.0 km/h */},
   /**<k_bsw_dynzone_range*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)-5.0 /**< -5.0 m */,(float32_T)-8.5 /**< -8.5 m */,(float32_T)-12.0 /**< -12.0 m */,(float32_T)-15.0 /**< -15.0 m */,(float32_T)-15.0 /**< -15.0 m */},
   /**<k_min_exist_prob_radar_guardrail*/ (float32_T) 1.0,
   /**<k_min_exist_prob_camera_guardrail*/ (float32_T) 1.0,
   /**<k_bsw_dynzone_speed_dropback*/ {(float32_T)-2.75 /**< -2.75 m/s | -9.9 km/h */,(float32_T)-2.22 /**< -2.22 m/s | -7.99 km/h */,(float32_T)-1.95 /**< -1.95 m/s | -7.02 km/h */,(float32_T)-1.4 /**< -1.4 m/s | -5.04 km/h */,(float32_T)-1.11 /**< -1.11 m/s | -4.0 km/h */,(float32_T)-0.83 /**< -0.83 m/s | -2.99 km/h */,(float32_T)0 /**< 0 m/s | 0.0 km/h */,(float32_T)0 /**< 0 m/s | 0.0 km/h */},
   /**<k_bsw_dynzone_speed_dropback_max*/ (float32_T) 39 /**< 39 m/s | 140.4 km/h */,
   /**<k_bsw_dynzone_range_dropback*/ {(float32_T)0.55,(float32_T)0.65,(float32_T)0.7,(float32_T)0.8,(float32_T)0.9,(float32_T)0.95,(float32_T)1,(float32_T)1},
   /**<k_bsw_n_line_position_for_long_object_sot_scenario*/ (float32_T) -100.0 /**< -100.0 m */,
   /**<k_bsw_line_to_stop_TOS_alert*/ (float32_T) -1.30,
   /**<k_bsw_min_length_long_object*/ (float32_T) 8 /**< 8 m */,
   /**<k_bsw_min_length_long_object_hys*/ (float32_T) 0 /**< 0 m */,
   /**<k_bsw_suppress_late_warning_max_time_till_leave*/ (float32_T) 0 /**< 0 s */,
   /**<k_bsw_suppress_late_warning_min_pos_behind_host*/ (float32_T) 20 /**< 20 m */,
   /**<k_bsw_suppress_late_warning_max_rel_vel*/ (float32_T) -10 /**< -10 m/s | -36.0 km/h */,
   /**<k_bsw_max_heading_abs*/ (float32_T) 0.785f /**< 0.785 rad | 44.98 deg */,
   /**<k_bsw_max_heading_abs_hysteresis*/ (float32_T) 0.0f /**< 0.0 rad | 0.0 deg */,
   /**<k_bsw_min_obj_long_vel*/ (float32_T) 1.39f /**< 1.39 m/s | 5.0 km/h */,
   /**<k_bsw_max_obj_long_vel*/ (float32_T) 100.0f /**< 100.0 m/s | 360.0 km/h */,
   /**<k_bsw_min_obj_long_vel_hysteresis*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_bsw_max_obj_long_vel_hysteresis*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_bsw_trailer_zone_ext_safety_margin*/ (float32_T) 0 /**< 0 m */,
   /**<k_bsw_trailer_zone_ext_safety_margin_hys*/ (float32_T) 0 /**< 0 m */,
   /**<k_cvw_zone_x*/ {(float32_T)-5 /**< -5 m */,(float32_T)-40 /**< -40 m */,(float32_T)-90 /**< -90 m */,(float32_T)-90 /**< -90 m */,(float32_T)-40 /**< -40 m */,(float32_T)-5 /**< -5 m */},
   /**<k_cvw_zone_y*/ {(float32_T)1.5 /**< 1.5 m */,(float32_T)1.4 /**< 1.4 m */,(float32_T)1.4 /**< 1.4 m */,(float32_T)0.6 /**< 0.6 m */,(float32_T)0.6 /**< 0.6 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_cvw_zone_y_hys*/ {(float32_T)0.1 /**< 0.1 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.1 /**< 0.1 m */},
   /**<k_cvw_zone_y_hys_max*/ (float32_T) 5 /**< 5 m */,
   /**<k_cvw_zone_y_hys_min*/ (float32_T) 0 /**< 0 m */,
   /**<k_lcda_max_range*/ (float32_T) 100.0f /**< 100.0 m */,
   /**<k_cvw_gap_bridge*/ (float32_T) 2.0 /**< 2.0 sec */,
   /**<k_cvw_candidate_ttc*/ (float32_T) 11.0f /**< 11.0 second */,
   /**<k_cvw_ttc*/ (float32_T) 3.0f /**< 3.0 second */,
   /**<k_cvw_ttc_hys*/ (float32_T) 2.0f /**< 2.0 second */,
   /**<k_cvw_max_curvi_heading_abs*/ (float32_T) 0.262 /**< 0.262 rad | 15.01 deg */,
   /**<k_cvw_min_obj_curvi_long_vel*/ (float32_T) 2.78f /**< 2.78 m/s | 10.01 km/h */,
   /**<k_cvw_lane_change_intention_zone_x*/ {(float32_T)-5.0f /**< -5.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-90.0f /**< -90.0 m */,(float32_T)-90.0f /**< -90.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-5.0f /**< -5.0 m */},
   /**<k_cvw_lane_change_intention_zone_y*/ {(float32_T)1.5f /**< 1.5 m */,(float32_T)2.2f /**< 2.2 m */,(float32_T)2.2f /**< 2.2 m */,(float32_T)-0.2f /**< -0.2 m */,(float32_T)-0.2f /**< -0.2 m */,(float32_T)0.5f /**< 0.5 m */},
   /**<k_cvw_y_width1*/ (float32_T) 2.6 /**< 2.6 m */,
   /**<k_cvw_x_length1*/ (float32_T) -30.0 /**< -30.0 m */,
   /**<k_cvw_y1*/ (float32_T) 1.5 /**< 1.5 m */,
   /**<k_cvw_x_length0*/ (float32_T) -40.0 /**< -40.0 m */,
   /**<k_cvw_y_width0*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_cvw_y0*/ (float32_T) 0.6 /**< 0.6 m */,
   /**<k_cvw_x0*/ (float32_T) 0.0 /**< 0.0 m */,
   /**<k_bsw_y_width*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_bsw_y0*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_x_length*/ (float32_T) -7.0 /**< -7.0 m */,
   /**<k_bsw_x0*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_lka_ov_zone_width*/ (float32_T) 3 /**< 3 m */,
   /**<k_slc_zone_x*/ {(float32_T)0 /**< 0 m */,(float32_T)-20 /**< -20 m */,(float32_T)-40 /**< -40 m */,(float32_T)-40 /**< -40 m */,(float32_T)-20 /**< -20 m */,(float32_T)0 /**< 0 m */},
   /**<k_slc_zone_y*/ {(float32_T)4 /**< 4 m */,(float32_T)3 /**< 3 m */,(float32_T)2 /**< 2 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_slc_max_curvi_heading_abs*/ (float32_T) 0.785f /**< 0.785 rad | 44.98 deg */,
   /**<k_slc_min_obj_curvi_long_vel_abs*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_slc_critical_lat_ttc*/ (float32_T) 3.0 /**< 3.0 second */,
   /**<k_slc_critical_lon_ttc*/ (float32_T) 3.5 /**< 3.5 second */,
   /**<k_slc_lateral_ttc_lookup*/ {(float32_T)0 /**< 0 second */,(float32_T)1 /**< 1 second */,(float32_T)1.8 /**< 1.8 second */,(float32_T)2.5 /**< 2.5 second */,(float32_T)3 /**< 3 second */,(float32_T)5 /**< 5 second */},
   /**<k_slc_lane_change_prob_lookup*/ {(float32_T)1,(float32_T)0.9,(float32_T)0.8,(float32_T)0.7,(float32_T)0.6,(float32_T)0.5},
   /**<k_slc_lookup_ego_speed*/ {(float32_T)14.0f /**< 14.0 m/s | 50.4 km/h */,(float32_T)28.0f /**< 28.0 m/s | 100.8 km/h */,(float32_T)40.0f /**< 40.0 m/s | 144.0 km/h */},
   /**<k_slc_lookup_ego_overlap_offset*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)0.0f /**< 0.0 m */},
   /**<k_bsw_obj_max_rel_vel_thresh*/ (float32_T) 55.0f /**< 55.0 m */,
   /**<k_bsw_obj_max_rel_vel_hys*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_bsw_zone_front_ego_side_x*/ (float32_T) 0.0 /**< 0.0 m */,
   /**<k_bsw_zone_front_ego_side_y*/ (float32_T) 0.0 /**< 0.0 m */,
   /**<k_bsw_zone_rear_outer_side_x*/ (float32_T) -10.0 /**< -10.0 m */,
   /**<k_bsw_zone_rear_outer_side_y*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_bsw_zone_front_ego_side_x_hys*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_zone_front_ego_side_y_hys*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_zone_rear_outer_side_x_hys*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_bsw_zone_rear_outer_side_y_hys*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_lcda_pedestrian_min_size*/ (float32_T) 0.01,
   /**<k_lcda_pedestrian_min_speed*/ (float32_T) 0.0,
   /**<k_lcda_2wheel_min_size*/ (float32_T) 0.01,
   /**<k_lcda_2wheel_min_speed*/ (float32_T) 0,
   /**<k_bsw_object_position_correction_delay_time*/ {(float32_T)0.13f /**< 0.13 s */,(float32_T)0.13f /**< 0.13 s */},
   /**<k_bsw_min_speed_for_tos_scenario*/ (float32_T) 0.0,
   /**<k_cvw_min_object_curvi_relative_speed*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */},
   /**<k_cvw_max_object_curvi_relative_speed*/ (float32_T) 1000.0 /**< 1000.0 m/s | 3600.0 km/h */,
   /**<k_cvw_object_curvi_relative_speed_hys*/ (float32_T) 0.1 /**< 0.1 m/s | 0.36 km/h */,
   /**<k_lcda_cvw_ttc_const*/ {(float32_T)3.12f /**< 3.12 s */,(float32_T)2.32f /**< 2.32 s */,(float32_T)1.52f /**< 1.52 s */},
   /**<k_lcda_cvw_ttc_accel*/ {(float32_T)2.942f /**< 2.942 m/s2 */,(float32_T)3.922f /**< 3.922 m/s2 */,(float32_T)4.903f /**< 4.903 m/s2 */},
   /**<k_lcda_dyn_cvw_ttc_speed_parameter*/ (float32_T) 4.0f,
   /**<k_bsw_object_position_correction_threshold*/ (float32_T) 12.5f /**< 12.5 m/s | 45.0 km/h */,
   /**<k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh*/ (float32_T) 12.5 /**< 12.5 m/s | 45.0 km/h */,
   /**<k_lcda_dyn_cvw_ttc_compens_time*/ {(float32_T)0.0f /**< 0.0 s */,(float32_T)0.0f /**< 0.0 s */},
   /**<k_bsw_dynzone_object_rel_vel*/ {(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},
   /**<k_bsw_dynzone_object_range*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_lcda_enable*/ (boolean_T) 1,
   /**<k_lcda_enable_via_cal*/ (boolean_T) 0,
   /**<k_lcda_f_disable_due_to_small_curve_radius*/ (boolean_T) 1u,
   /**<k_lcda_allow_track_status_new*/ (boolean_T) 0u,
   /**<k_lcda_f_enable_obj_in_ego_lane_check*/ (boolean_T) 1,
   /**<k_lcda_f_enable_suppress_alert_object_no_lane_change_intention*/ (boolean_T) 0,
   /**<k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge*/ (boolean_T) 1u,
   /**<k_bsw_f_enable_adv_pos_data_lane_change_intention*/ (boolean_T) 0,
   /**<k_lcda_f_enable_alert_obj_in_ego_lane*/ (boolean_T) 0,
   /**<k_lcda_ego_lane_check_center_point_only*/ (boolean_T) 1u,
   /**<k_bsw_uses_cvw_alert_state_enabled*/ (boolean_T) 0,
   /**<k_bsw_overlap_area_check_enable*/ (boolean_T) 1,
   /**<k_bsw_use_curvi_coordinates*/ (boolean_T) 0,
   /**<k_bsw_enable*/ (boolean_T) 1,
   /**<k_bsw_enable_via_cal*/ (boolean_T) 0,
   /**<k_bsw_enable_zone_front_boundary_specific_conditions*/ (boolean_T) 0,
   /**<k_bsw_enable_specific_front_sot_conditions*/ (boolean_T) 0,
   /**<k_bsw_hold_alert_long_object*/ (boolean_T) 0,
   /**<k_bsw_enable_factor_based_host_speed_adjustment*/ (boolean_T) 0,
   /**<k_bsw_enable_dynspeed_zone*/ (boolean_T) 0,
   /**<k_bsw_enable_trailer_zone_extension*/ (boolean_T) 1u,
   /**<k_cvw_enable*/ (boolean_T) 1,
   /**<k_cvw_enable_via_cal*/ (boolean_T) 0,
   /**<k_enable_cvw_curve_zone_adaptation*/ (boolean_T) 1,
   /**<k_cvw_f_most_crit_obj_must_be_closest_relevant_obj*/ (boolean_T) 0,
   /**<k_slc_enable*/ (boolean_T) 0u,
   /**<k_slc_enable_via_cal*/ (boolean_T) 0u,
   /**<k_lcda_f_enable_rel_vel_logic_in_ego_lane_check*/ (boolean_T) 0,
   /**<k_bsw_f_fallback_default_status_slow*/ (boolean_T) 0,
   /**<k_lcda_f_enable_obj_reflection_flag_check*/ (boolean_T) 0,
   /**<k_lcda_f_enable_fallback_handler*/ (boolean_T) 0,
   /**<k_lcda_f_enable_dyn_cvw_ttc_threshold*/ (boolean_T) 0,
   /**<k_bsw_shrink_zone_method*/ (boolean_T) 0,
   /**<k_bsw_f_use_front_zone_as_n_line*/ (boolean_T) 0,
   /**<k_bsw_f_enable_object_rel_vel_dynzone*/ (boolean_T) 0,
   /**<k_bsw_f_use_zone_without_hysteresis_lane_change_intention*/ (boolean_T) 0,
   /**<k_bsw_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_cvw_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_slc_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_lcda_zone_check_method*/ (uint8_t) 0,
   /**<k_bsw_fallback_fast_to_slow_qual_thres*/ (uint8_t) 2,
   /**<k_bsw_min_mature_cycles*/ (uint8_t) 3,
   /**<k_lcda_min_track_age*/ (uint8_t) 2,
   /**<k_bsw_alert_track_age*/ (uint8_t) 10,
   /**<k_bsw_stop_alert_reaching_front_custom_limit_mode*/ (uint8_t) 0,
   /**<k_cvw_min_mature_cycles*/ (uint8_t) 3u,
   /**<k_cvw_zone_calculation_mode*/ (uint8_t) 2,
   /**<k_slc_min_mature_cycles*/ (uint8_t) 3u,
   /**<k_slc_alert_qualifying_counter*/ (uint8_t) 3u,
   /**<k_lcda_f_enable_camera_based_guardrail*/ (uint8_t) 0u
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_lcda_f_enable_camera_based_guardrail*/ (uint8_t) 0u,
   /**<k_slc_alert_qualifying_counter*/ (uint8_t) 3u,
   /**<k_slc_min_mature_cycles*/ (uint8_t) 3u,
   /**<k_cvw_zone_calculation_mode*/ (uint8_t) 2,
   /**<k_cvw_min_mature_cycles*/ (uint8_t) 3u,
   /**<k_bsw_stop_alert_reaching_front_custom_limit_mode*/ (uint8_t) 0,
   /**<k_bsw_alert_track_age*/ (uint8_t) 10,
   /**<k_lcda_min_track_age*/ (uint8_t) 2,
   /**<k_bsw_min_mature_cycles*/ (uint8_t) 3,
   /**<k_bsw_fallback_fast_to_slow_qual_thres*/ (uint8_t) 2,
   /**<k_lcda_zone_check_method*/ (uint8_t) 0,
   /**<k_slc_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_cvw_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_bsw_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_bsw_f_use_zone_without_hysteresis_lane_change_intention*/ (boolean_T) 0,
   /**<k_bsw_f_enable_object_rel_vel_dynzone*/ (boolean_T) 0,
   /**<k_bsw_f_use_front_zone_as_n_line*/ (boolean_T) 0,
   /**<k_bsw_shrink_zone_method*/ (boolean_T) 0,
   /**<k_lcda_f_enable_dyn_cvw_ttc_threshold*/ (boolean_T) 0,
   /**<k_lcda_f_enable_fallback_handler*/ (boolean_T) 0,
   /**<k_lcda_f_enable_obj_reflection_flag_check*/ (boolean_T) 0,
   /**<k_bsw_f_fallback_default_status_slow*/ (boolean_T) 0,
   /**<k_lcda_f_enable_rel_vel_logic_in_ego_lane_check*/ (boolean_T) 0,
   /**<k_slc_enable_via_cal*/ (boolean_T) 0u,
   /**<k_slc_enable*/ (boolean_T) 0u,
   /**<k_cvw_f_most_crit_obj_must_be_closest_relevant_obj*/ (boolean_T) 0,
   /**<k_enable_cvw_curve_zone_adaptation*/ (boolean_T) 1,
   /**<k_cvw_enable_via_cal*/ (boolean_T) 0,
   /**<k_cvw_enable*/ (boolean_T) 1,
   /**<k_bsw_enable_trailer_zone_extension*/ (boolean_T) 1u,
   /**<k_bsw_enable_dynspeed_zone*/ (boolean_T) 0,
   /**<k_bsw_enable_factor_based_host_speed_adjustment*/ (boolean_T) 0,
   /**<k_bsw_hold_alert_long_object*/ (boolean_T) 0,
   /**<k_bsw_enable_specific_front_sot_conditions*/ (boolean_T) 0,
   /**<k_bsw_enable_zone_front_boundary_specific_conditions*/ (boolean_T) 0,
   /**<k_bsw_enable_via_cal*/ (boolean_T) 0,
   /**<k_bsw_enable*/ (boolean_T) 1,
   /**<k_bsw_use_curvi_coordinates*/ (boolean_T) 0,
   /**<k_bsw_overlap_area_check_enable*/ (boolean_T) 1,
   /**<k_bsw_uses_cvw_alert_state_enabled*/ (boolean_T) 0,
   /**<k_lcda_ego_lane_check_center_point_only*/ (boolean_T) 1u,
   /**<k_lcda_f_enable_alert_obj_in_ego_lane*/ (boolean_T) 0,
   /**<k_bsw_f_enable_adv_pos_data_lane_change_intention*/ (boolean_T) 0,
   /**<k_lcda_f_enable_suppress_alert_object_overhangs_zone_edge*/ (boolean_T) 1u,
   /**<k_lcda_f_enable_suppress_alert_object_no_lane_change_intention*/ (boolean_T) 0,
   /**<k_lcda_f_enable_obj_in_ego_lane_check*/ (boolean_T) 1,
   /**<k_lcda_allow_track_status_new*/ (boolean_T) 0u,
   /**<k_lcda_f_disable_due_to_small_curve_radius*/ (boolean_T) 1u,
   /**<k_lcda_enable_via_cal*/ (boolean_T) 0,
   /**<k_lcda_enable*/ (boolean_T) 1,
   /**<k_bsw_dynzone_object_range*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_bsw_dynzone_object_rel_vel*/ {(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0 /**< 0.0 m/s | 0.0 km/h */},
   /**<k_lcda_dyn_cvw_ttc_compens_time*/ {(float32_T)0.0f /**< 0.0 s */,(float32_T)0.0f /**< 0.0 s */},
   /**<k_lcda_dyn_cvw_ttc_compens_rel_vel_thresh*/ (float32_T) 12.5 /**< 12.5 m/s | 45.0 km/h */,
   /**<k_bsw_object_position_correction_threshold*/ (float32_T) 12.5f /**< 12.5 m/s | 45.0 km/h */,
   /**<k_lcda_dyn_cvw_ttc_speed_parameter*/ (float32_T) 4.0f,
   /**<k_lcda_cvw_ttc_accel*/ {(float32_T)2.942f /**< 2.942 m/s2 */,(float32_T)3.922f /**< 3.922 m/s2 */,(float32_T)4.903f /**< 4.903 m/s2 */},
   /**<k_lcda_cvw_ttc_const*/ {(float32_T)3.12f /**< 3.12 s */,(float32_T)2.32f /**< 2.32 s */,(float32_T)1.52f /**< 1.52 s */},
   /**<k_cvw_object_curvi_relative_speed_hys*/ (float32_T) 0.1 /**< 0.1 m/s | 0.36 km/h */,
   /**<k_cvw_max_object_curvi_relative_speed*/ (float32_T) 1000.0 /**< 1000.0 m/s | 3600.0 km/h */,
   /**<k_cvw_min_object_curvi_relative_speed*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */},
   /**<k_bsw_min_speed_for_tos_scenario*/ (float32_T) 0.0,
   /**<k_bsw_object_position_correction_delay_time*/ {(float32_T)0.13f /**< 0.13 s */,(float32_T)0.13f /**< 0.13 s */},
   /**<k_lcda_2wheel_min_speed*/ (float32_T) 0,
   /**<k_lcda_2wheel_min_size*/ (float32_T) 0.01,
   /**<k_lcda_pedestrian_min_speed*/ (float32_T) 0.0,
   /**<k_lcda_pedestrian_min_size*/ (float32_T) 0.01,
   /**<k_bsw_zone_rear_outer_side_y_hys*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_bsw_zone_rear_outer_side_x_hys*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_bsw_zone_front_ego_side_y_hys*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_zone_front_ego_side_x_hys*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_zone_rear_outer_side_y*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_bsw_zone_rear_outer_side_x*/ (float32_T) -10.0 /**< -10.0 m */,
   /**<k_bsw_zone_front_ego_side_y*/ (float32_T) 0.0 /**< 0.0 m */,
   /**<k_bsw_zone_front_ego_side_x*/ (float32_T) 0.0 /**< 0.0 m */,
   /**<k_bsw_obj_max_rel_vel_hys*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_bsw_obj_max_rel_vel_thresh*/ (float32_T) 55.0f /**< 55.0 m */,
   /**<k_slc_lookup_ego_overlap_offset*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)0.0f /**< 0.0 m */},
   /**<k_slc_lookup_ego_speed*/ {(float32_T)14.0f /**< 14.0 m/s | 50.4 km/h */,(float32_T)28.0f /**< 28.0 m/s | 100.8 km/h */,(float32_T)40.0f /**< 40.0 m/s | 144.0 km/h */},
   /**<k_slc_lane_change_prob_lookup*/ {(float32_T)1,(float32_T)0.9,(float32_T)0.8,(float32_T)0.7,(float32_T)0.6,(float32_T)0.5},
   /**<k_slc_lateral_ttc_lookup*/ {(float32_T)0 /**< 0 second */,(float32_T)1 /**< 1 second */,(float32_T)1.8 /**< 1.8 second */,(float32_T)2.5 /**< 2.5 second */,(float32_T)3 /**< 3 second */,(float32_T)5 /**< 5 second */},
   /**<k_slc_critical_lon_ttc*/ (float32_T) 3.5 /**< 3.5 second */,
   /**<k_slc_critical_lat_ttc*/ (float32_T) 3.0 /**< 3.0 second */,
   /**<k_slc_min_obj_curvi_long_vel_abs*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_slc_max_curvi_heading_abs*/ (float32_T) 0.785f /**< 0.785 rad | 44.98 deg */,
   /**<k_slc_zone_y*/ {(float32_T)4 /**< 4 m */,(float32_T)3 /**< 3 m */,(float32_T)2 /**< 2 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_slc_zone_x*/ {(float32_T)0 /**< 0 m */,(float32_T)-20 /**< -20 m */,(float32_T)-40 /**< -40 m */,(float32_T)-40 /**< -40 m */,(float32_T)-20 /**< -20 m */,(float32_T)0 /**< 0 m */},
   /**<k_lka_ov_zone_width*/ (float32_T) 3 /**< 3 m */,
   /**<k_bsw_x0*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_bsw_x_length*/ (float32_T) -7.0 /**< -7.0 m */,
   /**<k_bsw_y0*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_y_width*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_cvw_x0*/ (float32_T) 0.0 /**< 0.0 m */,
   /**<k_cvw_y0*/ (float32_T) 0.6 /**< 0.6 m */,
   /**<k_cvw_y_width0*/ (float32_T) 3.5 /**< 3.5 m */,
   /**<k_cvw_x_length0*/ (float32_T) -40.0 /**< -40.0 m */,
   /**<k_cvw_y1*/ (float32_T) 1.5 /**< 1.5 m */,
   /**<k_cvw_x_length1*/ (float32_T) -30.0 /**< -30.0 m */,
   /**<k_cvw_y_width1*/ (float32_T) 2.6 /**< 2.6 m */,
   /**<k_cvw_lane_change_intention_zone_y*/ {(float32_T)1.5f /**< 1.5 m */,(float32_T)2.2f /**< 2.2 m */,(float32_T)2.2f /**< 2.2 m */,(float32_T)-0.2f /**< -0.2 m */,(float32_T)-0.2f /**< -0.2 m */,(float32_T)0.5f /**< 0.5 m */},
   /**<k_cvw_lane_change_intention_zone_x*/ {(float32_T)-5.0f /**< -5.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-90.0f /**< -90.0 m */,(float32_T)-90.0f /**< -90.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-5.0f /**< -5.0 m */},
   /**<k_cvw_min_obj_curvi_long_vel*/ (float32_T) 2.78f /**< 2.78 m/s | 10.01 km/h */,
   /**<k_cvw_max_curvi_heading_abs*/ (float32_T) 0.262 /**< 0.262 rad | 15.01 deg */,
   /**<k_cvw_ttc_hys*/ (float32_T) 2.0f /**< 2.0 second */,
   /**<k_cvw_ttc*/ (float32_T) 3.0f /**< 3.0 second */,
   /**<k_cvw_candidate_ttc*/ (float32_T) 11.0f /**< 11.0 second */,
   /**<k_cvw_gap_bridge*/ (float32_T) 2.0 /**< 2.0 sec */,
   /**<k_lcda_max_range*/ (float32_T) 100.0f /**< 100.0 m */,
   /**<k_cvw_zone_y_hys_min*/ (float32_T) 0 /**< 0 m */,
   /**<k_cvw_zone_y_hys_max*/ (float32_T) 5 /**< 5 m */,
   /**<k_cvw_zone_y_hys*/ {(float32_T)0.1 /**< 0.1 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.1 /**< 0.1 m */},
   /**<k_cvw_zone_y*/ {(float32_T)1.5 /**< 1.5 m */,(float32_T)1.4 /**< 1.4 m */,(float32_T)1.4 /**< 1.4 m */,(float32_T)0.6 /**< 0.6 m */,(float32_T)0.6 /**< 0.6 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_cvw_zone_x*/ {(float32_T)-5 /**< -5 m */,(float32_T)-40 /**< -40 m */,(float32_T)-90 /**< -90 m */,(float32_T)-90 /**< -90 m */,(float32_T)-40 /**< -40 m */,(float32_T)-5 /**< -5 m */},
   /**<k_bsw_trailer_zone_ext_safety_margin_hys*/ (float32_T) 0 /**< 0 m */,
   /**<k_bsw_trailer_zone_ext_safety_margin*/ (float32_T) 0 /**< 0 m */,
   /**<k_bsw_max_obj_long_vel_hysteresis*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_bsw_min_obj_long_vel_hysteresis*/ (float32_T) 0.0f /**< 0.0 m/s | 0.0 km/h */,
   /**<k_bsw_max_obj_long_vel*/ (float32_T) 100.0f /**< 100.0 m/s | 360.0 km/h */,
   /**<k_bsw_min_obj_long_vel*/ (float32_T) 1.39f /**< 1.39 m/s | 5.0 km/h */,
   /**<k_bsw_max_heading_abs_hysteresis*/ (float32_T) 0.0f /**< 0.0 rad | 0.0 deg */,
   /**<k_bsw_max_heading_abs*/ (float32_T) 0.785f /**< 0.785 rad | 44.98 deg */,
   /**<k_bsw_suppress_late_warning_max_rel_vel*/ (float32_T) -10 /**< -10 m/s | -36.0 km/h */,
   /**<k_bsw_suppress_late_warning_min_pos_behind_host*/ (float32_T) 20 /**< 20 m */,
   /**<k_bsw_suppress_late_warning_max_time_till_leave*/ (float32_T) 0 /**< 0 s */,
   /**<k_bsw_min_length_long_object_hys*/ (float32_T) 0 /**< 0 m */,
   /**<k_bsw_min_length_long_object*/ (float32_T) 8 /**< 8 m */,
   /**<k_bsw_line_to_stop_TOS_alert*/ (float32_T) -1.30,
   /**<k_bsw_n_line_position_for_long_object_sot_scenario*/ (float32_T) -100.0 /**< -100.0 m */,
   /**<k_bsw_dynzone_range_dropback*/ {(float32_T)0.55,(float32_T)0.65,(float32_T)0.7,(float32_T)0.8,(float32_T)0.9,(float32_T)0.95,(float32_T)1,(float32_T)1},
   /**<k_bsw_dynzone_speed_dropback_max*/ (float32_T) 39 /**< 39 m/s | 140.4 km/h */,
   /**<k_bsw_dynzone_speed_dropback*/ {(float32_T)-2.75 /**< -2.75 m/s | -9.9 km/h */,(float32_T)-2.22 /**< -2.22 m/s | -7.99 km/h */,(float32_T)-1.95 /**< -1.95 m/s | -7.02 km/h */,(float32_T)-1.4 /**< -1.4 m/s | -5.04 km/h */,(float32_T)-1.11 /**< -1.11 m/s | -4.0 km/h */,(float32_T)-0.83 /**< -0.83 m/s | -2.99 km/h */,(float32_T)0 /**< 0 m/s | 0.0 km/h */,(float32_T)0 /**< 0 m/s | 0.0 km/h */},
   /**<k_min_exist_prob_camera_guardrail*/ (float32_T) 1.0,
   /**<k_min_exist_prob_radar_guardrail*/ (float32_T) 1.0,
   /**<k_bsw_dynzone_range*/ {(float32_T)0.0 /**< 0.0 m */,(float32_T)-5.0 /**< -5.0 m */,(float32_T)-8.5 /**< -8.5 m */,(float32_T)-12.0 /**< -12.0 m */,(float32_T)-15.0 /**< -15.0 m */,(float32_T)-15.0 /**< -15.0 m */},
   /**<k_bsw_dynzone_speed*/ {(float32_T)0 /**< 0 m/s | 0.0 km/h */,(float32_T)17.36 /**< 17.36 m/s | 62.5 km/h */,(float32_T)34.72 /**< 34.72 m/s | 124.99 km/h */,(float32_T)52.08 /**< 52.08 m/s | 187.49 km/h */,(float32_T)69.45 /**< 69.45 m/s | 250.02 km/h */,(float32_T)100 /**< 100 m/s | 360.0 km/h */},
   /**<k_bsw_fallback_rel_vel_thres_hys*/ (float32_T) 10 /**< 10 m/s | 36.0 km/h */,
   /**<k_bsw_fallback_rel_vel_thres*/ (float32_T) -5.55f /**< -5.55 m/s | -19.98 km/h */,
   /**<k_bsw_overlap_area_threshold*/ (float32_T) 0.0f /**< 0.0 % */,
   /**<k_bsw_lateral_distance_zone*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_bsw_fixed_zone_y_hys*/ {(float32_T)0.4f /**< 0.4 m */,(float32_T)0.4f /**< 0.4 m */,(float32_T)0.4f /**< 0.4 m */,(float32_T)-0.4f /**< -0.4 m */,(float32_T)-0.4f /**< -0.4 m */,(float32_T)-0.4f /**< -0.4 m */},
   /**<k_bsw_fixed_zone_x_hys*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_bsw_fixed_zone_y*/ {(float32_T)5.4f /**< 5.4 m */,(float32_T)5.4f /**< 5.4 m */,(float32_T)5.4f /**< 5.4 m */,(float32_T)1.75f /**< 1.75 m */,(float32_T)1.75f /**< 1.75 m */,(float32_T)1.75f /**< 1.75 m */},
   /**<k_bsw_fixed_zone_x*/ {(float32_T)0.80f /**< 0.80 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-3.0f /**< -3.0 m */,(float32_T)-3.0f /**< -3.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)0.8f /**< 0.8 m */},
   /**<k_bsw_zone_y_hys_max*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_bsw_zone_y_hys_min*/ (float32_T) 0.1 /**< 0.1 m */,
   /**<k_bsw_zone_y_hys*/ {(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */},
   /**<k_bsw_zone_x_hys*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)1.0 /**< 1.0 m */},
   /**<k_bsw_zone_y*/ {(float32_T)1.5 /**< 1.5 m */,(float32_T)1.5 /**< 1.5 m */,(float32_T)1.5 /**< 1.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_bsw_zone_x*/ {(float32_T)0.8 /**< 0.8 m */,(float32_T)0 /**< 0 m */,(float32_T)-3 /**< -3 m */,(float32_T)-3 /**< -3 m */,(float32_T)0 /**< 0 m */,(float32_T)0.8 /**< 0.8 m */},
   /**<k_lcda_min_exist_prob_lc_intention*/ (float32_T) 0.2f,
   /**<k_lcda_ego_lane_effective_lane_width_factor*/ (float32_T) 0.95,
   /**<k_bsw_lane_change_intention_pos_long_thres*/ (float32_T) 5,
   /**<k_bsw_lane_change_intention_pos_lat_thres*/ (float32_T) 0.3,
   /**<k_lcda_lane_change_intention_vel_lat_thresh*/ (float32_T) 0.5,
   /**<k_lcda_exist_prob_hys_offset*/ (float32_T) 0.1,
   /**<k_lcda_exist_prob_lc_intention_hys_offset*/ (float32_T) 0.2f,
   /**<k_lcda_zone_intersect_critical_point_lateral_ratio*/ (float32_T) 1.0,
   /**<k_lcda_max_ego_vehicle_length*/ (float32_T) 8.0 /**< 8.0 m */,
   /**<k_lcda_min_ego_vehicle_length*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_lcda_max_ego_vehicle_width*/ (float32_T) 3.0 /**< 3.0 m */,
   /**<k_lcda_min_ego_vehicle_width*/ (float32_T) 1.0 /**< 1.0 m */,
   /**<k_lcda_max_lane_width*/ (float32_T) 7.0 /**< 7.0 m */,
   /**<k_lcda_min_lane_width*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_lcda_curve_radius_threshold_for_zone_adaptation*/ (float32_T) 100 /**< 100 m */,
   /**<k_lcda_min_curve_radius_hys*/ (float32_T) 25.0f /**< 25.0 m */,
   /**<k_lcda_min_curve_radius*/ (float32_T) 100.0f /**< 100.0 m */,
   /**<k_lcda_distance_traveled_scale_factor*/ (float32_T) 0.6f,
   /**<k_lcda_host_activation_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_lcda_host_activation_speed_max*/ (float32_T) 48.61111f /**< 48.61111 m/s | 175.0 km/h */,
   /**<k_lcda_host_activation_speed_min_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_lcda_host_activation_speed_min*/ (float32_T) 2.8f /**< 2.8 m/s | 10.08 km/h */,
   /**<k_lcda_min_exist_prop*/ (float32_T) 0.7f,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)91,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Lcda_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Lcda_Public_Cal_Reverse_Array_Lcda_Cal(Lcda_Public_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_zone_x[0], sizeof(cal_dst->k_bsw_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_zone_y[0], sizeof(cal_dst->k_bsw_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_zone_x_hys[0], sizeof(cal_dst->k_bsw_zone_x_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_zone_y_hys[0], sizeof(cal_dst->k_bsw_zone_y_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_fixed_zone_x[0], sizeof(cal_dst->k_bsw_fixed_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_fixed_zone_y[0], sizeof(cal_dst->k_bsw_fixed_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_fixed_zone_x_hys[0], sizeof(cal_dst->k_bsw_fixed_zone_x_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_fixed_zone_y_hys[0], sizeof(cal_dst->k_bsw_fixed_zone_y_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_dynzone_speed[0], sizeof(cal_dst->k_bsw_dynzone_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_dynzone_range[0], sizeof(cal_dst->k_bsw_dynzone_range), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_dynzone_speed_dropback[0], sizeof(cal_dst->k_bsw_dynzone_speed_dropback), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_dynzone_range_dropback[0], sizeof(cal_dst->k_bsw_dynzone_range_dropback), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cvw_zone_x[0], sizeof(cal_dst->k_cvw_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cvw_zone_y[0], sizeof(cal_dst->k_cvw_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cvw_zone_y_hys[0], sizeof(cal_dst->k_cvw_zone_y_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cvw_lane_change_intention_zone_x[0], sizeof(cal_dst->k_cvw_lane_change_intention_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cvw_lane_change_intention_zone_y[0], sizeof(cal_dst->k_cvw_lane_change_intention_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_slc_zone_x[0], sizeof(cal_dst->k_slc_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_slc_zone_y[0], sizeof(cal_dst->k_slc_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_slc_lateral_ttc_lookup[0], sizeof(cal_dst->k_slc_lateral_ttc_lookup), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_slc_lane_change_prob_lookup[0], sizeof(cal_dst->k_slc_lane_change_prob_lookup), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_slc_lookup_ego_speed[0], sizeof(cal_dst->k_slc_lookup_ego_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_slc_lookup_ego_overlap_offset[0], sizeof(cal_dst->k_slc_lookup_ego_overlap_offset), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_object_position_correction_delay_time[0], sizeof(cal_dst->k_bsw_object_position_correction_delay_time), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_cvw_min_object_curvi_relative_speed[0], sizeof(cal_dst->k_cvw_min_object_curvi_relative_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_lcda_cvw_ttc_const[0], sizeof(cal_dst->k_lcda_cvw_ttc_const), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_lcda_cvw_ttc_accel[0], sizeof(cal_dst->k_lcda_cvw_ttc_accel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_lcda_dyn_cvw_ttc_compens_time[0], sizeof(cal_dst->k_lcda_dyn_cvw_ttc_compens_time), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_dynzone_object_rel_vel[0], sizeof(cal_dst->k_bsw_dynzone_object_rel_vel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_bsw_dynzone_object_range[0], sizeof(cal_dst->k_bsw_dynzone_object_range), CT_FOUR_BYTE);
}
#endif /* CT_BIG_ENDIAN */


