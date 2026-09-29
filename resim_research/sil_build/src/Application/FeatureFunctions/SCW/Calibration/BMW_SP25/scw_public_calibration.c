
/**
* @file scw_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in scw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "scw_public_calibration_t.h"
#include "scw_public_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Scw_Public_Cal_Update_Defaults(Scw_Public_Calibration_T* cal_dst)
{
    Scw_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)13,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_scw_min_host_speed*/ (float32_T) 0 /**< 0 m/s | 0.0 km/h */,
   /**<k_scw_min_host_speed_hys*/ (float32_T) 0 /**< 0 m/s | 0.0 km/h */,
   /**<k_scw_max_lat_pos_ratio*/ (float32_T) 0.5,
   /**<k_scw_candidate_heading*/ {(float32_T)-0.8f /**< -0.8 rad | -45.84 deg */,(float32_T)0.8f /**< 0.8 rad | 45.84 deg */},
   /**<k_scw_candidate_heading_hys*/ (float32_T) 0.1f /**< 0.1 rad | 5.73 deg */,
   /**<k_scw_candidate_yawrate*/ (float32_T) 1.0f /**< 1.0 rad/s | 57.3 deg/s */,
   /**<k_scw_candidate_yawrate_hys*/ (float32_T) 0.1f /**< 0.1 rad/s | 5.73 deg/s */,
   /**<k_scw_candidate_velocity*/ {(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */,(float32_T)100.0f /**< 100.0 m/s | 360.0 km/h */},
   /**<k_scw_candidate_velocity_hys*/ (float32_T) 1.0 /**< 1.0 m/s | 3.6 km/h */,
   /**<k_scw_candidate_relative_velocity*/ {(float32_T)-2.75 /**< -2.75 m/s | -9.9 km/h */,(float32_T)10.0 /**< 10.0 m/s | 36.0 km/h */},
   /**<k_scw_candidate_relative_vel_hys*/ (float32_T) 0.5 /**< 0.5 m/s | 1.8 km/h */,
   /**<k_scw_min_candidate_existence_probability*/ (float32_T) 0.9,
   /**<k_scw_min_exist_prob_radar_guardrail*/ (float32_T) 0.6,
   /**<k_scw_min_dynamic_lat_ttc*/ (float32_T) 0.9,
   /**<k_scw_max_dynamic_lat_ttc*/ (float32_T) 2.0,
   /**<k_scw_min_guardrail_lat_ttc*/ (float32_T) 0.9,
   /**<k_scw_max_guardrail_lat_ttc*/ (float32_T) 2.0,
   /**<k_scw_trailer_lat_ttc_extension*/ (float32_T) 0.5,
   /**<k_scw_min_dynamic_lat_distance*/ (float32_T) 0.35,
   /**<k_scw_max_dynamic_lat_distance*/ (float32_T) 0.8,
   /**<k_scw_min_guardrail_lat_distance*/ (float32_T) 0.35,
   /**<k_scw_max_guardrail_lat_distance*/ (float32_T) 0.8,
   /**<k_scw_critical_lat_ttc_hys*/ (float32_T) 0.2,
   /**<k_scw_critical_lat_distance_hys*/ (float32_T) 0.2,
   /**<k_scw_initial_zone_x*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)1.0 /**< 1.0 m */},
   /**<k_scw_initial_zone_y*/ {(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_scw_hys_zone_x_offset*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)1.0 /**< 1.0 m */},
   /**<k_scw_hys_zone_y_offset*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)1.0 /**< 1.0 m */,(float32_T)1.0 /**< 1.0 m */,(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */},
   /**<k_scw_lateral_distance_default*/ (float32_T) 30 /**< 30 m */,
   /**<k_scw_lateral_ttc_max*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_lateral_ttc_default*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_ttle_max*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_ttle_default*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_ttp_max*/ (float32_T) 100 /**< 100 s */,
   /**<k_scw_ttp_default*/ (float32_T) 100 /**< 100 s */,
   /**<k_scw_trailer_zone_ext_safety_margin*/ (float32_T) 2 /**< 2 m */,
   /**<k_scw_trailer_zone_ext_safety_margin_lat*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_scw_max_zone_length*/ (float32_T) 20 /**< 20 m */,
   /**<k_scw_max_zone_width*/ (float32_T) 5 /**< 5 m */,
   /**<k_scw_f_adjust_zones_to_ego_size*/ (boolean_T) 1u,
   /**<k_scw_f_enable_via_cal*/ (boolean_T) 0,
   /**<k_scw_f_dynamic_enable_via_cal*/ (boolean_T) 0,
   /**<k_scw_f_guardrail_enable_via_cal*/ (boolean_T) 0,
   /**<k_scw_f_enable*/ (boolean_T) 1u,
   /**<k_scw_f_enable_dynamic*/ (boolean_T) 1u,
   /**<k_scw_f_enable_guardrail*/ (boolean_T) 1u,
   /**<k_scw_f_enable_trailer_zone_extension*/ (boolean_T) 1,
   /**<k_scw_f_enable_trailer_ttc_extension*/ (boolean_T) 1,
   /**<k_scw_min_guardrail_age*/ (uint8_t) 5u,
   /**<k_scw_guardrail_freeze_period*/ (uint8_t) 10u,
   /**<k_scw_min_candidate_age*/ (uint8_t) 0,
   /**<k_scw_candidate_mature_cycles_in_zone_threshold*/ (uint8_t) 3,
   /**<k_scw_guardrail_cycles_in_zone_threshold*/ (uint8_t) 2
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_scw_guardrail_cycles_in_zone_threshold*/ (uint8_t) 2,
   /**<k_scw_candidate_mature_cycles_in_zone_threshold*/ (uint8_t) 3,
   /**<k_scw_min_candidate_age*/ (uint8_t) 0,
   /**<k_scw_guardrail_freeze_period*/ (uint8_t) 10u,
   /**<k_scw_min_guardrail_age*/ (uint8_t) 5u,
   /**<k_scw_f_enable_trailer_ttc_extension*/ (boolean_T) 1,
   /**<k_scw_f_enable_trailer_zone_extension*/ (boolean_T) 1,
   /**<k_scw_f_enable_guardrail*/ (boolean_T) 1u,
   /**<k_scw_f_enable_dynamic*/ (boolean_T) 1u,
   /**<k_scw_f_enable*/ (boolean_T) 1u,
   /**<k_scw_f_guardrail_enable_via_cal*/ (boolean_T) 0,
   /**<k_scw_f_dynamic_enable_via_cal*/ (boolean_T) 0,
   /**<k_scw_f_enable_via_cal*/ (boolean_T) 0,
   /**<k_scw_f_adjust_zones_to_ego_size*/ (boolean_T) 1u,
   /**<k_scw_max_zone_width*/ (float32_T) 5 /**< 5 m */,
   /**<k_scw_max_zone_length*/ (float32_T) 20 /**< 20 m */,
   /**<k_scw_trailer_zone_ext_safety_margin_lat*/ (float32_T) 0.5 /**< 0.5 m */,
   /**<k_scw_trailer_zone_ext_safety_margin*/ (float32_T) 2 /**< 2 m */,
   /**<k_scw_ttp_default*/ (float32_T) 100 /**< 100 s */,
   /**<k_scw_ttp_max*/ (float32_T) 100 /**< 100 s */,
   /**<k_scw_ttle_default*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_ttle_max*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_lateral_ttc_default*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_lateral_ttc_max*/ (float32_T) 10 /**< 10 s */,
   /**<k_scw_lateral_distance_default*/ (float32_T) 30 /**< 30 m */,
   /**<k_scw_hys_zone_y_offset*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)1.0 /**< 1.0 m */,(float32_T)1.0 /**< 1.0 m */,(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */,(float32_T)-0.5 /**< -0.5 m */},
   /**<k_scw_hys_zone_x_offset*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)1.0 /**< 1.0 m */},
   /**<k_scw_initial_zone_y*/ {(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */,(float32_T)5.0 /**< 5.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)0.0 /**< 0.0 m */},
   /**<k_scw_initial_zone_x*/ {(float32_T)1.0 /**< 1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)-1.0 /**< -1.0 m */,(float32_T)0.0 /**< 0.0 m */,(float32_T)1.0 /**< 1.0 m */},
   /**<k_scw_critical_lat_distance_hys*/ (float32_T) 0.2,
   /**<k_scw_critical_lat_ttc_hys*/ (float32_T) 0.2,
   /**<k_scw_max_guardrail_lat_distance*/ (float32_T) 0.8,
   /**<k_scw_min_guardrail_lat_distance*/ (float32_T) 0.35,
   /**<k_scw_max_dynamic_lat_distance*/ (float32_T) 0.8,
   /**<k_scw_min_dynamic_lat_distance*/ (float32_T) 0.35,
   /**<k_scw_trailer_lat_ttc_extension*/ (float32_T) 0.5,
   /**<k_scw_max_guardrail_lat_ttc*/ (float32_T) 2.0,
   /**<k_scw_min_guardrail_lat_ttc*/ (float32_T) 0.9,
   /**<k_scw_max_dynamic_lat_ttc*/ (float32_T) 2.0,
   /**<k_scw_min_dynamic_lat_ttc*/ (float32_T) 0.9,
   /**<k_scw_min_exist_prob_radar_guardrail*/ (float32_T) 0.6,
   /**<k_scw_min_candidate_existence_probability*/ (float32_T) 0.9,
   /**<k_scw_candidate_relative_vel_hys*/ (float32_T) 0.5 /**< 0.5 m/s | 1.8 km/h */,
   /**<k_scw_candidate_relative_velocity*/ {(float32_T)-2.75 /**< -2.75 m/s | -9.9 km/h */,(float32_T)10.0 /**< 10.0 m/s | 36.0 km/h */},
   /**<k_scw_candidate_velocity_hys*/ (float32_T) 1.0 /**< 1.0 m/s | 3.6 km/h */,
   /**<k_scw_candidate_velocity*/ {(float32_T)1.0f /**< 1.0 m/s | 3.6 km/h */,(float32_T)100.0f /**< 100.0 m/s | 360.0 km/h */},
   /**<k_scw_candidate_yawrate_hys*/ (float32_T) 0.1f /**< 0.1 rad/s | 5.73 deg/s */,
   /**<k_scw_candidate_yawrate*/ (float32_T) 1.0f /**< 1.0 rad/s | 57.3 deg/s */,
   /**<k_scw_candidate_heading_hys*/ (float32_T) 0.1f /**< 0.1 rad | 5.73 deg */,
   /**<k_scw_candidate_heading*/ {(float32_T)-0.8f /**< -0.8 rad | -45.84 deg */,(float32_T)0.8f /**< 0.8 rad | 45.84 deg */},
   /**<k_scw_max_lat_pos_ratio*/ (float32_T) 0.5,
   /**<k_scw_min_host_speed_hys*/ (float32_T) 0 /**< 0 m/s | 0.0 km/h */,
   /**<k_scw_min_host_speed*/ (float32_T) 0 /**< 0 m/s | 0.0 km/h */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)13,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Scw_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Scw_Public_Cal_Reverse_Array_Scw_Cal(Scw_Public_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_candidate_heading[0], sizeof(cal_dst->k_scw_candidate_heading), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_candidate_velocity[0], sizeof(cal_dst->k_scw_candidate_velocity), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_candidate_relative_velocity[0], sizeof(cal_dst->k_scw_candidate_relative_velocity), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_initial_zone_x[0], sizeof(cal_dst->k_scw_initial_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_initial_zone_y[0], sizeof(cal_dst->k_scw_initial_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_hys_zone_x_offset[0], sizeof(cal_dst->k_scw_hys_zone_x_offset), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_scw_hys_zone_y_offset[0], sizeof(cal_dst->k_scw_hys_zone_y_offset), CT_FOUR_BYTE);
}
#endif /* CT_BIG_ENDIAN */


