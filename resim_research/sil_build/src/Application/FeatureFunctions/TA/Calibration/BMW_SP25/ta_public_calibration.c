
/**
* @file ta_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ta_public_calibration_t.h"
#include "ta_public_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ta_Public_Cal_Update_Defaults(Ta_Public_Calibration_T* cal_dst)
{
    Ta_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)46,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_ta_alert_lvl_1_ttp_threshold*/ {(float32_T)4.0f /**< 4.0 s */,(float32_T)3.0f /**< 3.0 s */,(float32_T)2.0f /**< 2.0 s */},
   /**<k_ta_active_obj_ttp_offset*/ (float32_T) 1.0f /**< 1.0 s */,
   /**<k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max*/ (float32_T) 1.3889f /**< 1.3889 m/s | 5.0 km/h */,
   /**<k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max*/ (float32_T) 2.778f /**< 2.778 m/s | 10.0 km/h */,
   /**<k_ta_alert_lvl_2_ttc_threshold*/ (float32_T) 2.0f /**< 2.0 s */,
   /**<k_ta_alert_lvl_3_ttc_threshold*/ (float32_T) 1.4f /**< 1.4 s */,
   /**<k_ta_alert_lvl_3_ttb_threshold*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ta_alert_lvl_4_ttc_threshold*/ (float32_T) 1.0f /**< 1.0 s */,
   /**<k_ta_alert_lvl_4_decel_threshold*/ (float32_T) 8.0f /**< 8.0 m/s^2 */,
   /**<k_ta_critical_approach_min_safe_distance*/ (float32_T) 0.5f /**< 0.5 m */,
   /**<k_ta_critical_approach_angle_diff_min*/ (float32_T) 0.05f /**< 0.05 rad | 2.86 deg */,
   /**<k_ta_ego_max_pred_yaw_angle*/ (float32_T) 1.66f /**< 1.66 rad | 95.11 deg */,
   /**<k_ta_obj_pred_speed_min*/ (float32_T) 0.7f /**< 0.7 m/s | 2.52 km/h */,
   /**<k_ta_obj_acceleration_long_weight*/ (float32_T) 0.0f,
   /**<k_ta_obj_acceleration_lat_weight*/ (float32_T) 0.0f,
   /**<k_ta_ego_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ta_obj_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ta_ego_shape_gain_fixed*/ (float32_T) 1.1f,
   /**<k_ta_obj_shape_gain_fixed*/ (float32_T) 1.0f,
   /**<k_ta_ego_speed*/ {(float32_T)0.28f /**< 0.28 m/s | 1.01 km/h */,(float32_T)14.0f /**< 14.0 m/s | 50.4 km/h */},
   /**<k_ta_ego_speed_ofst*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_ta_ego_yawrate*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)1.0f /**< 1.0 rad/s | 57.3 deg/s */},
   /**<k_ta_ego_yawrate_ofst*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */},
   /**<k_ta_ego_long_acceleration*/ {(float32_T)-20.0f /**< -20.0 m/s^2 */,(float32_T)2.1f /**< 2.1 m/s^2 */},
   /**<k_ta_ego_long_acceleration_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s^2 */,(float32_T)1.0f /**< 1.0 m/s^2 */},
   /**<k_ta_ego_acceleration_weight*/ (float32_T) 0.0f,
   /**<k_ta_ego_deceleration_weight*/ (float32_T) 1.0f,
   /**<k_ta_ego_circle_offset*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ta_ego_circle_host_length_factor*/ (float32_T) 1.0f,
   /**<k_ta_ego_yawangle_integration_yawrate_min*/ (float32_T) 0.01f /**< 0.01 rad/s | 0.57 deg/s */,
   /**<k_fta_obj_exist_prblty*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_fta_obj_exist_prblty_ofst*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_fta_obj_vcs_long_vel_rel*/ {(float32_T)-10.f /**< -10. m/s | -36.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_fta_obj_vcs_long_vel_rel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_lat_vel_rel*/ {(float32_T)-10.f /**< -10. m/s | -36.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_fta_obj_vcs_lat_vel_rel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_long_vel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_fta_obj_vcs_long_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_lat_vel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_fta_obj_vcs_lat_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_ta_straight_host_curvature_max*/ (float32_T) 0.0085f /**< 0.0085 1/m */,
   /**<k_ta_lookup_turning_host_speed*/ {(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */,(float32_T)3.5f /**< 3.5 m/s | 12.6 km/h */,(float32_T)7.0f /**< 7.0 m/s | 25.2 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_ta_lookup_turning_host_curvature_min*/ {(float32_T)0.05f /**< 0.05 1/m */,(float32_T)0.037f /**< 0.037 1/m */,(float32_T)0.028f /**< 0.028 1/m */,(float32_T)0.023f /**< 0.023 1/m */},
   /**<k_fta_obj_vcs_long_pos_straight_min*/ (float32_T) -0.4f /**< -0.4 m */,
   /**<k_fta_obj_heading*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)1.7f /**< 1.7 rad | 97.4 deg */},
   /**<k_fta_obj_heading_straight*/ {(float32_T)1.414f /**< 1.414 rad | 81.02 deg */,(float32_T)1.74f /**< 1.74 rad | 99.69 deg */},
   /**<k_fta_obj_heading_ofst*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.1f /**< 0.1 rad | 5.73 deg */},
   /**<k_fta_obj_heading_rate*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)1.0f /**< 1.0 rad/s | 57.3 deg/s */},
   /**<k_fta_obj_heading_rate_straight*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)1.0f /**< 1.0 rad/s | 57.3 deg/s */},
   /**<k_fta_obj_heading_rate_ofst*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)0.1f /**< 0.1 rad/s | 5.73 deg/s */},
   /**<k_fta_obj_speed*/ {(float32_T)0.75f /**< 0.75 m/s | 2.7 km/h */,(float32_T)7.5f /**< 7.5 m/s | 27.0 km/h */},
   /**<k_fta_obj_speed_straight*/ {(float32_T)1.2f /**< 1.2 m/s | 4.32 km/h */,(float32_T)7.5f /**< 7.5 m/s | 27.0 km/h */},
   /**<k_fta_obj_speed_ofst*/ {(float32_T)-0.05f /**< -0.05 m/s | -0.18 km/h */,(float32_T)0.3f /**< 0.3 m/s | 1.08 km/h */},
   /**<k_fta_obj_length*/ {(float32_T)0.1f /**< 0.1 m */,(float32_T)3.0f /**< 3.0 m */},
   /**<k_fta_obj_length_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_fta_obj_width*/ {(float32_T)0.1f /**< 0.1 m */,(float32_T)1.9f /**< 1.9 m */},
   /**<k_fta_obj_width_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_fta_obj_area*/ {(float32_T)0.0f /**< 0.0 m^2 */,(float32_T)4.5f /**< 4.5 m^2 */},
   /**<k_fta_obj_area_ofst*/ {(float32_T)0.0f /**< 0.0 m^2 */,(float32_T)1.0f /**< 1.0 m^2 */},
   /**<k_fta_obj_vru_class_prob*/ {(float32_T)0.45f /**< 0.45 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_fta_obj_vru_class_prob_ofst*/ {(float32_T)-0.05f /**< -0.05 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_fta_ego_obj_heading_diff*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.33f /**< 0.33 rad | 18.91 deg */},
   /**<k_fta_ego_obj_heading_diff_ofst*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.05f /**< 0.05 rad | 2.86 deg */},
   /**<k_fta_obj_eclipse_value*/ {(float32_T)0.0f,(float32_T)0.2f},
   /**<k_fta_obj_eclipse_value_ofst*/ {(float32_T)0.0f,(float32_T)0.1f},
   /**<k_fta_obj_velocity_heading_diff_max*/ (float32_T) 0.5f /**< 0.5 rad | 28.65 deg */,
   /**<k_fta_brake_deceleration_max*/ (float32_T) 10.0f /**< 10.0 m/s^2 */,
   /**<k_fta_brake_dead_time*/ (float32_T) 0.3f /**< 0.3 s */,
   /**<k_fta_brake_gradient*/ (float32_T) -40.0f /**< -40.0 m/s^3 */,
   /**<k_fta_danger_zone_left_long*/ {(float32_T)15.0f /**< 15.0 m */,(float32_T)13.5f /**< 13.5 m */,(float32_T)4.7f /**< 4.7 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-8.0f /**< -8.0 m */,(float32_T)-9.0f /**< -9.0 m */,(float32_T)6.0f /**< 6.0 m */},
   /**<k_fta_danger_zone_left_lat*/ {(float32_T)-10.0f /**< -10.0 m */,(float32_T)-4.0f /**< -4.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-4.2f /**< -4.2 m */,(float32_T)-8.5f /**< -8.5 m */,(float32_T)-12.5f /**< -12.5 m */},
   /**<k_fta_danger_zone_right_long*/ {(float32_T)4.7f /**< 4.7 m */,(float32_T)13.5f /**< 13.5 m */,(float32_T)15.0f /**< 15.0 m */,(float32_T)6.0f /**< 6.0 m */,(float32_T)-9.0f /**< -9.0 m */,(float32_T)-8.0f /**< -8.0 m */,(float32_T)0.0f /**< 0.0 m */},
   /**<k_fta_danger_zone_right_lat*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)4.0f /**< 4.0 m */,(float32_T)10.0f /**< 10.0 m */,(float32_T)12.5f /**< 12.5 m */,(float32_T)8.5f /**< 8.5 m */,(float32_T)4.2f /**< 4.2 m */,(float32_T)0.0f /**< 0.0 m */},
   /**<k_pfgs_ego_speed*/ {(float32_T)0.87f /**< 0.87 m/s | 3.13 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_pfgs_qualification_ttc_min*/ (float32_T) 0.5f /**< 0.5 s */,
   /**<k_tap_lvl_2_host_curvature_min*/ (float32_T) 0.012f /**< 0.012 1/m */,
   /**<k_rta_obj_exist_prblty*/ {(float32_T)0.7f /**< 0.7 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_rta_obj_exist_prblty_ofst*/ {(float32_T)-0.3f /**< -0.3 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_rta_obj_vcs_long_vel_rel*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_long_vel_rel_ofst*/ {(float32_T)-2.0f /**< -2.0 m/s | -7.2 km/h */,(float32_T)2.0f /**< 2.0 m/s | 7.2 km/h */},
   /**<k_rta_obj_vcs_lat_vel_rel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_lat_vel_rel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_vcs_long_vel*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_long_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_vcs_lat_vel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_lat_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_heading*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)1.0f /**< 1.0 rad | 57.3 deg */},
   /**<k_rta_obj_heading_ofst*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.2f /**< 0.2 rad | 11.46 deg */},
   /**<k_rta_obj_speed*/ {(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */,(float32_T)16.0f /**< 16.0 m/s | 57.6 km/h */},
   /**<k_rta_obj_speed_ofst*/ {(float32_T)-0.1f /**< -0.1 m/s | -0.36 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_length*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)15.0f /**< 15.0 m */},
   /**<k_rta_obj_length_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_rta_obj_width*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)5.0f /**< 5.0 m */},
   /**<k_rta_obj_width_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_rta_obj_vru_class_prob*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_rta_obj_vru_class_prob_ofst*/ {(float32_T)-0.1f /**< -0.1 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_rta_obj_eclipse_value*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)0.5f /**< 0.5 % */},
   /**<k_rta_obj_eclipse_value_ofst*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)0.1f /**< 0.1 % */},
   /**<k_rta_ttp_obj_abs_lat_vel_rel_max*/ (float32_T) 1.9f /**< 1.9 m/s | 6.84 km/h */,
   /**<k_rta_ttp_obj_abs_heading_diff_max*/ (float32_T) 0.7f /**< 0.7 rad | 40.11 deg */,
   /**<k_rta_ttp_curve_suppression_obj_distance_min*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_rta_info_zone_left_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-40.0f /**< -40.0 m */},
   /**<k_rta_info_zone_left_lat*/ {(float32_T)-4.0f /**< -4.0 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-4.0f /**< -4.0 m */},
   /**<k_rta_info_zone_right_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-40.0f /**< -40.0 m */},
   /**<k_rta_info_zone_right_lat*/ {(float32_T)1.5f /**< 1.5 m */,(float32_T)4.0f /**< 4.0 m */,(float32_T)4.0f /**< 4.0 m */,(float32_T)1.5f /**< 1.5 m */},
   /**<k_rta_info_zone_left_long_hys*/ {(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_info_zone_left_lat_hys*/ {(float32_T)-0.5f /**< -0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-0.05 /**< -0.05 m */},
   /**<k_rta_info_zone_right_long_hys*/ {(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_info_zone_right_lat_hys*/ {(float32_T)-0.5f /**< -0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-0.5f /**< -0.5 m */},
   /**<k_rta_wing_zone_left_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-10.0f /**< -10.0 m */,(float32_T)-10.0f /**< -10.0 m */},
   /**<k_rta_wing_zone_left_lat*/ {(float32_T)-10.0f /**< -10.0 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-10.0f /**< -10.0 m */},
   /**<k_rta_wing_zone_right_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-10.0f /**< -10.0 m */,(float32_T)-10.0f /**< -10.0 m */},
   /**<k_rta_wing_zone_right_lat*/ {(float32_T)1.5f /**< 1.5 m */,(float32_T)10.0f /**< 10.0 m */,(float32_T)10.0f /**< 10.0 m */,(float32_T)1.5f /**< 1.5 m */},
   /**<k_rta_wing_zone_left_long_hys*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_wing_zone_left_lat_hys*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_wing_zone_right_long_hys*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_wing_zone_right_lat_hys*/ {(float32_T)-0.5f /**< -0.5 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)-0.5f /**< -0.5 m */},
   /**<k_ta_always_overwrite_ta_mode_to_both*/ (boolean_T) 0,
   /**<k_ta_f_only_allow_consecutive_ttc_based_alert_levels*/ (boolean_T) 1u,
   /**<k_ta_f_skip_holding_for_single_alert_level_drop*/ (boolean_T) 1u,
   /**<k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj*/ (boolean_T) 0u,
   /**<k_ta_f_apply_ttp_hysteresis_globally*/ (boolean_T) 1u,
   /**<k_f_ta_enable_debug_mode*/ (boolean_T) 0u,
   /**<k_f_fta_enable*/ (boolean_T) 1u,
   /**<k_f_fta_enable_brake_gradient_logic*/ (boolean_T) 1u,
   /**<k_f_fta_enable_danger_zones*/ (boolean_T) 1u,
   /**<k_pfgs_symbol_request_sides_enabled*/ (boolean_T) 1u,
   /**<k_pfgs_qualification_check_f_stationary*/ (boolean_T) 0u,
   /**<k_f_rta_enable*/ (boolean_T) 1u,
   /**<k_f_rta_enable_info_zones*/ (boolean_T) 1u,
   /**<k_rta_f_higher_obj_crit_based_on_lower_ttp*/ (boolean_T) 0,
   /**<k_f_rta_enable_wing_zones*/ (boolean_T) 1u,
   /**<k_ta_alert_qualifying_cycles*/ (uint8_t) 3u,
   /**<k_ta_alert_holding_cycles*/ (uint8_t) 3u,
   /**<k_ta_prediction_steps_max*/ (uint8_t) 20u,
   /**<k_ta_critical_approach_check_ego_circles*/ {(uint8_t)1,(uint8_t)0,(uint8_t)0},
   /**<k_ta_ego_pred_const_velocity_pred_steps_min*/ (uint8_t) 4u,
   /**<k_fta_obj_age_min*/ (uint8_t) 18,
   /**<k_fta_danger_zone_point_size*/ (uint8_t) 7u,
   /**<k_pfgs_qualification_counter_fast_obj*/ (uint8_t) 1u,
   /**<k_pfgs_qualification_counter_slow_obj*/ (uint8_t) 10u,
   /**<k_rta_obj_age_min*/ (uint8_t) 3u,
   /**<k_rta_info_zone_point_size*/ (uint8_t) 4u,
   /**<k_rta_wing_zone_point_size*/ (uint8_t) 4u
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_rta_wing_zone_point_size*/ (uint8_t) 4u,
   /**<k_rta_info_zone_point_size*/ (uint8_t) 4u,
   /**<k_rta_obj_age_min*/ (uint8_t) 3u,
   /**<k_pfgs_qualification_counter_slow_obj*/ (uint8_t) 10u,
   /**<k_pfgs_qualification_counter_fast_obj*/ (uint8_t) 1u,
   /**<k_fta_danger_zone_point_size*/ (uint8_t) 7u,
   /**<k_fta_obj_age_min*/ (uint8_t) 18,
   /**<k_ta_ego_pred_const_velocity_pred_steps_min*/ (uint8_t) 4u,
   /**<k_ta_critical_approach_check_ego_circles*/ {(uint8_t)1,(uint8_t)0,(uint8_t)0},
   /**<k_ta_prediction_steps_max*/ (uint8_t) 20u,
   /**<k_ta_alert_holding_cycles*/ (uint8_t) 3u,
   /**<k_ta_alert_qualifying_cycles*/ (uint8_t) 3u,
   /**<k_f_rta_enable_wing_zones*/ (boolean_T) 1u,
   /**<k_rta_f_higher_obj_crit_based_on_lower_ttp*/ (boolean_T) 0,
   /**<k_f_rta_enable_info_zones*/ (boolean_T) 1u,
   /**<k_f_rta_enable*/ (boolean_T) 1u,
   /**<k_pfgs_qualification_check_f_stationary*/ (boolean_T) 0u,
   /**<k_pfgs_symbol_request_sides_enabled*/ (boolean_T) 1u,
   /**<k_f_fta_enable_danger_zones*/ (boolean_T) 1u,
   /**<k_f_fta_enable_brake_gradient_logic*/ (boolean_T) 1u,
   /**<k_f_fta_enable*/ (boolean_T) 1u,
   /**<k_f_ta_enable_debug_mode*/ (boolean_T) 0u,
   /**<k_ta_f_apply_ttp_hysteresis_globally*/ (boolean_T) 1u,
   /**<k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj*/ (boolean_T) 0u,
   /**<k_ta_f_skip_holding_for_single_alert_level_drop*/ (boolean_T) 1u,
   /**<k_ta_f_only_allow_consecutive_ttc_based_alert_levels*/ (boolean_T) 1u,
   /**<k_ta_always_overwrite_ta_mode_to_both*/ (boolean_T) 0,
   /**<k_rta_wing_zone_right_lat_hys*/ {(float32_T)-0.5f /**< -0.5 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)-0.5f /**< -0.5 m */},
   /**<k_rta_wing_zone_right_long_hys*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_wing_zone_left_lat_hys*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_wing_zone_left_long_hys*/ {(float32_T)1.0f /**< 1.0 m */,(float32_T)1.0f /**< 1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_wing_zone_right_lat*/ {(float32_T)1.5f /**< 1.5 m */,(float32_T)10.0f /**< 10.0 m */,(float32_T)10.0f /**< 10.0 m */,(float32_T)1.5f /**< 1.5 m */},
   /**<k_rta_wing_zone_right_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-10.0f /**< -10.0 m */,(float32_T)-10.0f /**< -10.0 m */},
   /**<k_rta_wing_zone_left_lat*/ {(float32_T)-10.0f /**< -10.0 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-10.0f /**< -10.0 m */},
   /**<k_rta_wing_zone_left_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-10.0f /**< -10.0 m */,(float32_T)-10.0f /**< -10.0 m */},
   /**<k_rta_info_zone_right_lat_hys*/ {(float32_T)-0.5f /**< -0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-0.5f /**< -0.5 m */},
   /**<k_rta_info_zone_right_long_hys*/ {(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_info_zone_left_lat_hys*/ {(float32_T)-0.5f /**< -0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-0.05 /**< -0.05 m */},
   /**<k_rta_info_zone_left_long_hys*/ {(float32_T)0.5f /**< 0.5 m */,(float32_T)0.5f /**< 0.5 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */},
   /**<k_rta_info_zone_right_lat*/ {(float32_T)1.5f /**< 1.5 m */,(float32_T)4.0f /**< 4.0 m */,(float32_T)4.0f /**< 4.0 m */,(float32_T)1.5f /**< 1.5 m */},
   /**<k_rta_info_zone_right_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-40.0f /**< -40.0 m */},
   /**<k_rta_info_zone_left_lat*/ {(float32_T)-4.0f /**< -4.0 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-1.5f /**< -1.5 m */,(float32_T)-4.0f /**< -4.0 m */},
   /**<k_rta_info_zone_left_long*/ {(float32_T)-1.0f /**< -1.0 m */,(float32_T)-1.0f /**< -1.0 m */,(float32_T)-40.0f /**< -40.0 m */,(float32_T)-40.0f /**< -40.0 m */},
   /**<k_rta_ttp_curve_suppression_obj_distance_min*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_rta_ttp_obj_abs_heading_diff_max*/ (float32_T) 0.7f /**< 0.7 rad | 40.11 deg */,
   /**<k_rta_ttp_obj_abs_lat_vel_rel_max*/ (float32_T) 1.9f /**< 1.9 m/s | 6.84 km/h */,
   /**<k_rta_obj_eclipse_value_ofst*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)0.1f /**< 0.1 % */},
   /**<k_rta_obj_eclipse_value*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)0.5f /**< 0.5 % */},
   /**<k_rta_obj_vru_class_prob_ofst*/ {(float32_T)-0.1f /**< -0.1 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_rta_obj_vru_class_prob*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_rta_obj_width_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_rta_obj_width*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)5.0f /**< 5.0 m */},
   /**<k_rta_obj_length_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_rta_obj_length*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)15.0f /**< 15.0 m */},
   /**<k_rta_obj_speed_ofst*/ {(float32_T)-0.1f /**< -0.1 m/s | -0.36 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_speed*/ {(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */,(float32_T)16.0f /**< 16.0 m/s | 57.6 km/h */},
   /**<k_rta_obj_heading_ofst*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.2f /**< 0.2 rad | 11.46 deg */},
   /**<k_rta_obj_heading*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)1.0f /**< 1.0 rad | 57.3 deg */},
   /**<k_rta_obj_vcs_lat_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_vcs_lat_vel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_long_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_vcs_long_vel*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_lat_vel_rel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_rta_obj_vcs_lat_vel_rel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_vcs_long_vel_rel_ofst*/ {(float32_T)-2.0f /**< -2.0 m/s | -7.2 km/h */,(float32_T)2.0f /**< 2.0 m/s | 7.2 km/h */},
   /**<k_rta_obj_vcs_long_vel_rel*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_rta_obj_exist_prblty_ofst*/ {(float32_T)-0.3f /**< -0.3 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_rta_obj_exist_prblty*/ {(float32_T)0.7f /**< 0.7 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_tap_lvl_2_host_curvature_min*/ (float32_T) 0.012f /**< 0.012 1/m */,
   /**<k_pfgs_qualification_ttc_min*/ (float32_T) 0.5f /**< 0.5 s */,
   /**<k_pfgs_ego_speed*/ {(float32_T)0.87f /**< 0.87 m/s | 3.13 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_fta_danger_zone_right_lat*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)4.0f /**< 4.0 m */,(float32_T)10.0f /**< 10.0 m */,(float32_T)12.5f /**< 12.5 m */,(float32_T)8.5f /**< 8.5 m */,(float32_T)4.2f /**< 4.2 m */,(float32_T)0.0f /**< 0.0 m */},
   /**<k_fta_danger_zone_right_long*/ {(float32_T)4.7f /**< 4.7 m */,(float32_T)13.5f /**< 13.5 m */,(float32_T)15.0f /**< 15.0 m */,(float32_T)6.0f /**< 6.0 m */,(float32_T)-9.0f /**< -9.0 m */,(float32_T)-8.0f /**< -8.0 m */,(float32_T)0.0f /**< 0.0 m */},
   /**<k_fta_danger_zone_left_lat*/ {(float32_T)-10.0f /**< -10.0 m */,(float32_T)-4.0f /**< -4.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-4.2f /**< -4.2 m */,(float32_T)-8.5f /**< -8.5 m */,(float32_T)-12.5f /**< -12.5 m */},
   /**<k_fta_danger_zone_left_long*/ {(float32_T)15.0f /**< 15.0 m */,(float32_T)13.5f /**< 13.5 m */,(float32_T)4.7f /**< 4.7 m */,(float32_T)0.0f /**< 0.0 m */,(float32_T)-8.0f /**< -8.0 m */,(float32_T)-9.0f /**< -9.0 m */,(float32_T)6.0f /**< 6.0 m */},
   /**<k_fta_brake_gradient*/ (float32_T) -40.0f /**< -40.0 m/s^3 */,
   /**<k_fta_brake_dead_time*/ (float32_T) 0.3f /**< 0.3 s */,
   /**<k_fta_brake_deceleration_max*/ (float32_T) 10.0f /**< 10.0 m/s^2 */,
   /**<k_fta_obj_velocity_heading_diff_max*/ (float32_T) 0.5f /**< 0.5 rad | 28.65 deg */,
   /**<k_fta_obj_eclipse_value_ofst*/ {(float32_T)0.0f,(float32_T)0.1f},
   /**<k_fta_obj_eclipse_value*/ {(float32_T)0.0f,(float32_T)0.2f},
   /**<k_fta_ego_obj_heading_diff_ofst*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.05f /**< 0.05 rad | 2.86 deg */},
   /**<k_fta_ego_obj_heading_diff*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.33f /**< 0.33 rad | 18.91 deg */},
   /**<k_fta_obj_vru_class_prob_ofst*/ {(float32_T)-0.05f /**< -0.05 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_fta_obj_vru_class_prob*/ {(float32_T)0.45f /**< 0.45 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_fta_obj_area_ofst*/ {(float32_T)0.0f /**< 0.0 m^2 */,(float32_T)1.0f /**< 1.0 m^2 */},
   /**<k_fta_obj_area*/ {(float32_T)0.0f /**< 0.0 m^2 */,(float32_T)4.5f /**< 4.5 m^2 */},
   /**<k_fta_obj_width_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_fta_obj_width*/ {(float32_T)0.1f /**< 0.1 m */,(float32_T)1.9f /**< 1.9 m */},
   /**<k_fta_obj_length_ofst*/ {(float32_T)0.0f /**< 0.0 m */,(float32_T)1.0f /**< 1.0 m */},
   /**<k_fta_obj_length*/ {(float32_T)0.1f /**< 0.1 m */,(float32_T)3.0f /**< 3.0 m */},
   /**<k_fta_obj_speed_ofst*/ {(float32_T)-0.05f /**< -0.05 m/s | -0.18 km/h */,(float32_T)0.3f /**< 0.3 m/s | 1.08 km/h */},
   /**<k_fta_obj_speed_straight*/ {(float32_T)1.2f /**< 1.2 m/s | 4.32 km/h */,(float32_T)7.5f /**< 7.5 m/s | 27.0 km/h */},
   /**<k_fta_obj_speed*/ {(float32_T)0.75f /**< 0.75 m/s | 2.7 km/h */,(float32_T)7.5f /**< 7.5 m/s | 27.0 km/h */},
   /**<k_fta_obj_heading_rate_ofst*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)0.1f /**< 0.1 rad/s | 5.73 deg/s */},
   /**<k_fta_obj_heading_rate_straight*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)1.0f /**< 1.0 rad/s | 57.3 deg/s */},
   /**<k_fta_obj_heading_rate*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)1.0f /**< 1.0 rad/s | 57.3 deg/s */},
   /**<k_fta_obj_heading_ofst*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)0.1f /**< 0.1 rad | 5.73 deg */},
   /**<k_fta_obj_heading_straight*/ {(float32_T)1.414f /**< 1.414 rad | 81.02 deg */,(float32_T)1.74f /**< 1.74 rad | 99.69 deg */},
   /**<k_fta_obj_heading*/ {(float32_T)0.0f /**< 0.0 rad | 0.0 deg */,(float32_T)1.7f /**< 1.7 rad | 97.4 deg */},
   /**<k_fta_obj_vcs_long_pos_straight_min*/ (float32_T) -0.4f /**< -0.4 m */,
   /**<k_ta_lookup_turning_host_curvature_min*/ {(float32_T)0.05f /**< 0.05 1/m */,(float32_T)0.037f /**< 0.037 1/m */,(float32_T)0.028f /**< 0.028 1/m */,(float32_T)0.023f /**< 0.023 1/m */},
   /**<k_ta_lookup_turning_host_speed*/ {(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */,(float32_T)3.5f /**< 3.5 m/s | 12.6 km/h */,(float32_T)7.0f /**< 7.0 m/s | 25.2 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_ta_straight_host_curvature_max*/ (float32_T) 0.0085f /**< 0.0085 1/m */,
   /**<k_fta_obj_vcs_lat_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_lat_vel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_fta_obj_vcs_long_vel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_long_vel*/ {(float32_T)-20.0f /**< -20.0 m/s | -72.0 km/h */,(float32_T)20.0f /**< 20.0 m/s | 72.0 km/h */},
   /**<k_fta_obj_vcs_lat_vel_rel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_lat_vel_rel*/ {(float32_T)-10.f /**< -10. m/s | -36.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_fta_obj_vcs_long_vel_rel_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s | -3.6 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_fta_obj_vcs_long_vel_rel*/ {(float32_T)-10.f /**< -10. m/s | -36.0 km/h */,(float32_T)10.0f /**< 10.0 m/s | 36.0 km/h */},
   /**<k_fta_obj_exist_prblty_ofst*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)0.0f /**< 0.0 % */},
   /**<k_fta_obj_exist_prblty*/ {(float32_T)0.0f /**< 0.0 % */,(float32_T)1.0f /**< 1.0 % */},
   /**<k_ta_ego_yawangle_integration_yawrate_min*/ (float32_T) 0.01f /**< 0.01 rad/s | 0.57 deg/s */,
   /**<k_ta_ego_circle_host_length_factor*/ (float32_T) 1.0f,
   /**<k_ta_ego_circle_offset*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_ta_ego_deceleration_weight*/ (float32_T) 1.0f,
   /**<k_ta_ego_acceleration_weight*/ (float32_T) 0.0f,
   /**<k_ta_ego_long_acceleration_ofst*/ {(float32_T)-1.0f /**< -1.0 m/s^2 */,(float32_T)1.0f /**< 1.0 m/s^2 */},
   /**<k_ta_ego_long_acceleration*/ {(float32_T)-20.0f /**< -20.0 m/s^2 */,(float32_T)2.1f /**< 2.1 m/s^2 */},
   /**<k_ta_ego_yawrate_ofst*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */},
   /**<k_ta_ego_yawrate*/ {(float32_T)0.0f /**< 0.0 rad/s | 0.0 deg/s */,(float32_T)1.0f /**< 1.0 rad/s | 57.3 deg/s */},
   /**<k_ta_ego_speed_ofst*/ {(float32_T)0.0f /**< 0.0 m/s | 0.0 km/h */,(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */},
   /**<k_ta_ego_speed*/ {(float32_T)0.28f /**< 0.28 m/s | 1.01 km/h */,(float32_T)14.0f /**< 14.0 m/s | 50.4 km/h */},
   /**<k_ta_obj_shape_gain_fixed*/ (float32_T) 1.0f,
   /**<k_ta_ego_shape_gain_fixed*/ (float32_T) 1.1f,
   /**<k_ta_obj_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ta_ego_shape_gain_per_pred_step*/ (float32_T) 1.0f,
   /**<k_ta_obj_acceleration_lat_weight*/ (float32_T) 0.0f,
   /**<k_ta_obj_acceleration_long_weight*/ (float32_T) 0.0f,
   /**<k_ta_obj_pred_speed_min*/ (float32_T) 0.7f /**< 0.7 m/s | 2.52 km/h */,
   /**<k_ta_ego_max_pred_yaw_angle*/ (float32_T) 1.66f /**< 1.66 rad | 95.11 deg */,
   /**<k_ta_critical_approach_angle_diff_min*/ (float32_T) 0.05f /**< 0.05 rad | 2.86 deg */,
   /**<k_ta_critical_approach_min_safe_distance*/ (float32_T) 0.5f /**< 0.5 m */,
   /**<k_ta_alert_lvl_4_decel_threshold*/ (float32_T) 8.0f /**< 8.0 m/s^2 */,
   /**<k_ta_alert_lvl_4_ttc_threshold*/ (float32_T) 1.0f /**< 1.0 s */,
   /**<k_ta_alert_lvl_3_ttb_threshold*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ta_alert_lvl_3_ttc_threshold*/ (float32_T) 1.4f /**< 1.4 s */,
   /**<k_ta_alert_lvl_2_ttc_threshold*/ (float32_T) 2.0f /**< 2.0 s */,
   /**<k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max*/ (float32_T) 2.778f /**< 2.778 m/s | 10.0 km/h */,
   /**<k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max*/ (float32_T) 1.3889f /**< 1.3889 m/s | 5.0 km/h */,
   /**<k_ta_active_obj_ttp_offset*/ (float32_T) 1.0f /**< 1.0 s */,
   /**<k_ta_alert_lvl_1_ttp_threshold*/ {(float32_T)4.0f /**< 4.0 s */,(float32_T)3.0f /**< 3.0 s */,(float32_T)2.0f /**< 2.0 s */},
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)46,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Ta_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ta_Public_Cal_Reverse_Array_Ta_Cal(Ta_Public_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_alert_lvl_1_ttp_threshold[0], sizeof(cal_dst->k_ta_alert_lvl_1_ttp_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_ego_speed[0], sizeof(cal_dst->k_ta_ego_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_ego_speed_ofst[0], sizeof(cal_dst->k_ta_ego_speed_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_ego_yawrate[0], sizeof(cal_dst->k_ta_ego_yawrate), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_ego_yawrate_ofst[0], sizeof(cal_dst->k_ta_ego_yawrate_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_ego_long_acceleration[0], sizeof(cal_dst->k_ta_ego_long_acceleration), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_ego_long_acceleration_ofst[0], sizeof(cal_dst->k_ta_ego_long_acceleration_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_exist_prblty[0], sizeof(cal_dst->k_fta_obj_exist_prblty), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_exist_prblty_ofst[0], sizeof(cal_dst->k_fta_obj_exist_prblty_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_long_vel_rel[0], sizeof(cal_dst->k_fta_obj_vcs_long_vel_rel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_long_vel_rel_ofst[0], sizeof(cal_dst->k_fta_obj_vcs_long_vel_rel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_lat_vel_rel[0], sizeof(cal_dst->k_fta_obj_vcs_lat_vel_rel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_lat_vel_rel_ofst[0], sizeof(cal_dst->k_fta_obj_vcs_lat_vel_rel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_long_vel[0], sizeof(cal_dst->k_fta_obj_vcs_long_vel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_long_vel_ofst[0], sizeof(cal_dst->k_fta_obj_vcs_long_vel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_lat_vel[0], sizeof(cal_dst->k_fta_obj_vcs_lat_vel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vcs_lat_vel_ofst[0], sizeof(cal_dst->k_fta_obj_vcs_lat_vel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_lookup_turning_host_speed[0], sizeof(cal_dst->k_ta_lookup_turning_host_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_lookup_turning_host_curvature_min[0], sizeof(cal_dst->k_ta_lookup_turning_host_curvature_min), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_heading[0], sizeof(cal_dst->k_fta_obj_heading), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_heading_straight[0], sizeof(cal_dst->k_fta_obj_heading_straight), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_heading_ofst[0], sizeof(cal_dst->k_fta_obj_heading_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_heading_rate[0], sizeof(cal_dst->k_fta_obj_heading_rate), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_heading_rate_straight[0], sizeof(cal_dst->k_fta_obj_heading_rate_straight), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_heading_rate_ofst[0], sizeof(cal_dst->k_fta_obj_heading_rate_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_speed[0], sizeof(cal_dst->k_fta_obj_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_speed_straight[0], sizeof(cal_dst->k_fta_obj_speed_straight), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_speed_ofst[0], sizeof(cal_dst->k_fta_obj_speed_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_length[0], sizeof(cal_dst->k_fta_obj_length), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_length_ofst[0], sizeof(cal_dst->k_fta_obj_length_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_width[0], sizeof(cal_dst->k_fta_obj_width), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_width_ofst[0], sizeof(cal_dst->k_fta_obj_width_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_area[0], sizeof(cal_dst->k_fta_obj_area), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_area_ofst[0], sizeof(cal_dst->k_fta_obj_area_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vru_class_prob[0], sizeof(cal_dst->k_fta_obj_vru_class_prob), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_vru_class_prob_ofst[0], sizeof(cal_dst->k_fta_obj_vru_class_prob_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_ego_obj_heading_diff[0], sizeof(cal_dst->k_fta_ego_obj_heading_diff), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_ego_obj_heading_diff_ofst[0], sizeof(cal_dst->k_fta_ego_obj_heading_diff_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_eclipse_value[0], sizeof(cal_dst->k_fta_obj_eclipse_value), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_obj_eclipse_value_ofst[0], sizeof(cal_dst->k_fta_obj_eclipse_value_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_danger_zone_left_long[0], sizeof(cal_dst->k_fta_danger_zone_left_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_danger_zone_left_lat[0], sizeof(cal_dst->k_fta_danger_zone_left_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_danger_zone_right_long[0], sizeof(cal_dst->k_fta_danger_zone_right_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_fta_danger_zone_right_lat[0], sizeof(cal_dst->k_fta_danger_zone_right_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_pfgs_ego_speed[0], sizeof(cal_dst->k_pfgs_ego_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_exist_prblty[0], sizeof(cal_dst->k_rta_obj_exist_prblty), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_exist_prblty_ofst[0], sizeof(cal_dst->k_rta_obj_exist_prblty_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_long_vel_rel[0], sizeof(cal_dst->k_rta_obj_vcs_long_vel_rel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_long_vel_rel_ofst[0], sizeof(cal_dst->k_rta_obj_vcs_long_vel_rel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_lat_vel_rel[0], sizeof(cal_dst->k_rta_obj_vcs_lat_vel_rel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_lat_vel_rel_ofst[0], sizeof(cal_dst->k_rta_obj_vcs_lat_vel_rel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_long_vel[0], sizeof(cal_dst->k_rta_obj_vcs_long_vel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_long_vel_ofst[0], sizeof(cal_dst->k_rta_obj_vcs_long_vel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_lat_vel[0], sizeof(cal_dst->k_rta_obj_vcs_lat_vel), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vcs_lat_vel_ofst[0], sizeof(cal_dst->k_rta_obj_vcs_lat_vel_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_heading[0], sizeof(cal_dst->k_rta_obj_heading), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_heading_ofst[0], sizeof(cal_dst->k_rta_obj_heading_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_speed[0], sizeof(cal_dst->k_rta_obj_speed), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_speed_ofst[0], sizeof(cal_dst->k_rta_obj_speed_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_length[0], sizeof(cal_dst->k_rta_obj_length), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_length_ofst[0], sizeof(cal_dst->k_rta_obj_length_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_width[0], sizeof(cal_dst->k_rta_obj_width), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_width_ofst[0], sizeof(cal_dst->k_rta_obj_width_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vru_class_prob[0], sizeof(cal_dst->k_rta_obj_vru_class_prob), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_vru_class_prob_ofst[0], sizeof(cal_dst->k_rta_obj_vru_class_prob_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_eclipse_value[0], sizeof(cal_dst->k_rta_obj_eclipse_value), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_obj_eclipse_value_ofst[0], sizeof(cal_dst->k_rta_obj_eclipse_value_ofst), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_left_long[0], sizeof(cal_dst->k_rta_info_zone_left_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_left_lat[0], sizeof(cal_dst->k_rta_info_zone_left_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_right_long[0], sizeof(cal_dst->k_rta_info_zone_right_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_right_lat[0], sizeof(cal_dst->k_rta_info_zone_right_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_left_long_hys[0], sizeof(cal_dst->k_rta_info_zone_left_long_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_left_lat_hys[0], sizeof(cal_dst->k_rta_info_zone_left_lat_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_right_long_hys[0], sizeof(cal_dst->k_rta_info_zone_right_long_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_info_zone_right_lat_hys[0], sizeof(cal_dst->k_rta_info_zone_right_lat_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_left_long[0], sizeof(cal_dst->k_rta_wing_zone_left_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_left_lat[0], sizeof(cal_dst->k_rta_wing_zone_left_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_right_long[0], sizeof(cal_dst->k_rta_wing_zone_right_long), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_right_lat[0], sizeof(cal_dst->k_rta_wing_zone_right_lat), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_left_long_hys[0], sizeof(cal_dst->k_rta_wing_zone_left_long_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_left_lat_hys[0], sizeof(cal_dst->k_rta_wing_zone_left_lat_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_right_long_hys[0], sizeof(cal_dst->k_rta_wing_zone_right_long_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_rta_wing_zone_right_lat_hys[0], sizeof(cal_dst->k_rta_wing_zone_right_lat_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ta_critical_approach_check_ego_circles[0], sizeof(cal_dst->k_ta_critical_approach_check_ego_circles), CT_ONE_BYTE);
}
#endif /* CT_BIG_ENDIAN */


