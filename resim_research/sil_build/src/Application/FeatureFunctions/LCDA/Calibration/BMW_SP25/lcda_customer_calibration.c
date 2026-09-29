
/**
* @file lcda_customer_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "lcda_customer_calibration_t.h"
#include "lcda_customer_calibration.h"
#include <string.h>

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Lcda_Customer_Cal_Update_Defaults(Lcda_Customer_Calibration_T* cal_dst)
{
    Lcda_Customer_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)60,
   /**<version*/ (uint16_t)91,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)2973,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_bmw_sp25_guardrail_rel_diff_thresh*/ (float32_T) 0.5f,
   /**<k_bmw_sp25_exist_prob_lc_intention*/ (float32_T) 0.91f,
   /**<k_bmw_sp25_cvw_limit_zone_range_hys*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment*/ (float32_T) 0.5f,
   /**<k_bmw_sp25_trailer_mode_max_trailer_length*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_bmw_sp25_trailer_mode_max_bike_carrier_distance*/ (float32_T) 0.5f /**< 0.5 m */,
   /**<k_bmw_sp25_trailer_mode_max_bike_carrier_buffer*/ (float32_T) 0.4f /**< 0.4 m */,
   /**<k_bmw_sp25_lane_change_detection_host_speed_min*/ (float32_T) 16.6f /**< 16.6 m/s | 59.76 km/h */,
   /**<k_bmw_sp25_lane_change_dist_to_laneline_max*/ (float32_T) 0.25f /**< 0.25 m */,
   /**<k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min*/ (float32_T) 25.0f /**< 25.0 % */,
   /**<k_bmw_sp25_smooth_camera_signals*/ (boolean_T) 0,
   /**<k_bmw_sp25_f_enable_lane_change_detection*/ (boolean_T) 1,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_bmw_sp25_guardrail_age_stage_thresh*/ (uint8_t) 4u,
   /**<k_bmw_sp25_max_bad_guardrail_holding_counter*/ (uint8_t) 3,
   /**<k_bmw_sp25_lane_change_counter_min*/ (uint8_t) 3,
   /**<k_bmw_sp25_lane_change_counter_max*/ (uint8_t) 10,
   /**<k_bmw_sp25_camera_lane_plausibilisation_counter_max*/ (uint8_t) 5
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_bmw_sp25_camera_lane_plausibilisation_counter_max*/ (uint8_t) 5,
   /**<k_bmw_sp25_lane_change_counter_max*/ (uint8_t) 10,
   /**<k_bmw_sp25_lane_change_counter_min*/ (uint8_t) 3,
   /**<k_bmw_sp25_max_bad_guardrail_holding_counter*/ (uint8_t) 3,
   /**<k_bmw_sp25_guardrail_age_stage_thresh*/ (uint8_t) 4u,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_bmw_sp25_f_enable_lane_change_detection*/ (boolean_T) 1,
   /**<k_bmw_sp25_smooth_camera_signals*/ (boolean_T) 0,
   /**<k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min*/ (float32_T) 25.0f /**< 25.0 % */,
   /**<k_bmw_sp25_lane_change_dist_to_laneline_max*/ (float32_T) 0.25f /**< 0.25 m */,
   /**<k_bmw_sp25_lane_change_detection_host_speed_min*/ (float32_T) 16.6f /**< 16.6 m/s | 59.76 km/h */,
   /**<k_bmw_sp25_trailer_mode_max_bike_carrier_buffer*/ (float32_T) 0.4f /**< 0.4 m */,
   /**<k_bmw_sp25_trailer_mode_max_bike_carrier_distance*/ (float32_T) 0.5f /**< 0.5 m */,
   /**<k_bmw_sp25_trailer_mode_max_trailer_length*/ (float32_T) 5.0f /**< 5.0 m */,
   /**<k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment*/ (float32_T) 0.5f,
   /**<k_bmw_sp25_cvw_limit_zone_range_hys*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_bmw_sp25_exist_prob_lc_intention*/ (float32_T) 0.91f,
   /**<k_bmw_sp25_guardrail_rel_diff_thresh*/ (float32_T) 0.5f,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)2973,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)91,
   /**<Section_Size*/ (uint32_t)60
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Lcda_Customer_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Lcda_Customer_Cal_Reverse_Array_Lcda_Cal(Lcda_Customer_Calibration_T* cal_dst)
{

}
#endif /* CT_BIG_ENDIAN */


