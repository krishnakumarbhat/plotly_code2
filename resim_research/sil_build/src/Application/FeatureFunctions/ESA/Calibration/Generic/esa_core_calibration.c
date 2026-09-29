
/**
* @file esa_core_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the Generic specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in esa_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "esa_core_calibration_t.h"
#include "esa_core_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Esa_Core_Cal_Update_Defaults(Esa_Core_Calibration_T* cal_dst)
{
    Esa_Core_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)184,
   /**<version*/ (uint16_t)2,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)10576,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_esa_zone_x*/ {(float32_T)-1 /**< -1 m */,(float32_T)-40 /**< -40 m */,(float32_T)-90 /**< -90 m */,(float32_T)-90 /**< -90 m */,(float32_T)-40 /**< -40 m */,(float32_T)-1 /**< -1 m */},
   /**<k_esa_zone_x_hys*/ {(float32_T)1 /**< 1 m */,(float32_T)0 /**< 0 m */,(float32_T)5 /**< 5 m */,(float32_T)5 /**< 5 m */,(float32_T)0 /**< 0 m */,(float32_T)1 /**< 1 m */},
   /**<k_esa_zone_y*/ {(float32_T)2.5 /**< 2.5 m */,(float32_T)2.5 /**< 2.5 m */,(float32_T)2.5 /**< 2.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_esa_zone_y_hys*/ {(float32_T)0.1 /**< 0.1 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.1 /**< 0.1 m */},
   /**<k_esa_max_range*/ (float32_T) 100.0f /**< 100.0 m */,
   /**<k_esa_min_lane_width*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_esa_max_lane_width*/ (float32_T) 7.0 /**< 7.0 m */,
   /**<k_esa_min_exist_prob*/ (float32_T) 0.7f,
   /**<k_esa_min_curve_radius*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_esa_min_curve_radius_hys*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_esa_max_curvi_heading_abs*/ (float32_T) 0.785f /**< 0.785 rad | 44.98 deg */,
   /**<k_esa_min_obj_curvi_long_vel_abs*/ (float32_T) 1.4f /**< 1.4 m/s | 5.04 km/h */,
   /**<k_esa_critical_longitudinal_ttc*/ (float32_T) 5 /**< 5 second */,
   /**<k_esa_critical_longitudinal_ttc_hys*/ (float32_T) 6 /**< 6 second */,
   /**<k_esa_obj_safe_deceleration_threshold*/ (float32_T) 5 /**< 5 m/s^2 */,
   /**<k_esa_obj_safe_deceleration_threshold_hys*/ (float32_T) 4 /**< 4 m/s^2 */,
   /**<k_esa_host_activation_speed_min*/ (float32_T) 5.0f /**< 5.0 m/s | 18.0 km/h */,
   /**<k_esa_host_activation_speed_min_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_esa_host_activation_speed_max*/ (float32_T) 100.0f /**< 100.0 m/s | 360.0 km/h */,
   /**<k_esa_host_activation_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_esa_f_enable_via_cal*/ (boolean_T) 0u,
   /**<k_esa_f_enable*/ (boolean_T) 0u,
   /**<k_esa_f_allow_min_curve_radius*/ (boolean_T) 1u,
   /**<k_esa_f_allow_obj_critical_ttc_and_deceleration*/ (boolean_T) 0u,
   /**<k_esa_f_allow_obj_selection_ttc*/ (boolean_T) 0u,
   /**<k_esa_f_allow_obj_selection_deceleration*/ (boolean_T) 0u,
   /**<k_esa_f_allow_obj_selection_long_distance*/ (boolean_T) 1u,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_unused_padding_byte_1*/ (uint8_t) 0,
   /**<k_esa_min_track_age*/ (uint8_t) 2u,
   /**<k_esa_min_mature_cycles*/ (uint8_t) 3u,
   /**<k_esa_alert_holding_cycles*/ (uint8_t) 0u
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_esa_alert_holding_cycles*/ (uint8_t) 0u,
   /**<k_esa_min_mature_cycles*/ (uint8_t) 3u,
   /**<k_esa_min_track_age*/ (uint8_t) 2u,
   /**<k_unused_padding_byte_1*/ (uint8_t) 0,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_esa_f_allow_obj_selection_long_distance*/ (boolean_T) 1u,
   /**<k_esa_f_allow_obj_selection_deceleration*/ (boolean_T) 0u,
   /**<k_esa_f_allow_obj_selection_ttc*/ (boolean_T) 0u,
   /**<k_esa_f_allow_obj_critical_ttc_and_deceleration*/ (boolean_T) 0u,
   /**<k_esa_f_allow_min_curve_radius*/ (boolean_T) 1u,
   /**<k_esa_f_enable*/ (boolean_T) 0u,
   /**<k_esa_f_enable_via_cal*/ (boolean_T) 0u,
   /**<k_esa_host_activation_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_esa_host_activation_speed_max*/ (float32_T) 100.0f /**< 100.0 m/s | 360.0 km/h */,
   /**<k_esa_host_activation_speed_min_hys*/ (float32_T) 1.0f /**< 1.0 m/s | 3.6 km/h */,
   /**<k_esa_host_activation_speed_min*/ (float32_T) 5.0f /**< 5.0 m/s | 18.0 km/h */,
   /**<k_esa_obj_safe_deceleration_threshold_hys*/ (float32_T) 4 /**< 4 m/s^2 */,
   /**<k_esa_obj_safe_deceleration_threshold*/ (float32_T) 5 /**< 5 m/s^2 */,
   /**<k_esa_critical_longitudinal_ttc_hys*/ (float32_T) 6 /**< 6 second */,
   /**<k_esa_critical_longitudinal_ttc*/ (float32_T) 5 /**< 5 second */,
   /**<k_esa_min_obj_curvi_long_vel_abs*/ (float32_T) 1.4f /**< 1.4 m/s | 5.04 km/h */,
   /**<k_esa_max_curvi_heading_abs*/ (float32_T) 0.785f /**< 0.785 rad | 44.98 deg */,
   /**<k_esa_min_curve_radius_hys*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_esa_min_curve_radius*/ (float32_T) 10.0f /**< 10.0 m */,
   /**<k_esa_min_exist_prob*/ (float32_T) 0.7f,
   /**<k_esa_max_lane_width*/ (float32_T) 7.0 /**< 7.0 m */,
   /**<k_esa_min_lane_width*/ (float32_T) 2.0 /**< 2.0 m */,
   /**<k_esa_max_range*/ (float32_T) 100.0f /**< 100.0 m */,
   /**<k_esa_zone_y_hys*/ {(float32_T)0.1 /**< 0.1 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.4 /**< 0.4 m */,(float32_T)0.3 /**< 0.3 m */,(float32_T)0.1 /**< 0.1 m */},
   /**<k_esa_zone_y*/ {(float32_T)2.5 /**< 2.5 m */,(float32_T)2.5 /**< 2.5 m */,(float32_T)2.5 /**< 2.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */,(float32_T)0.5 /**< 0.5 m */},
   /**<k_esa_zone_x_hys*/ {(float32_T)1 /**< 1 m */,(float32_T)0 /**< 0 m */,(float32_T)5 /**< 5 m */,(float32_T)5 /**< 5 m */,(float32_T)0 /**< 0 m */,(float32_T)1 /**< 1 m */},
   /**<k_esa_zone_x*/ {(float32_T)-1 /**< -1 m */,(float32_T)-40 /**< -40 m */,(float32_T)-90 /**< -90 m */,(float32_T)-90 /**< -90 m */,(float32_T)-40 /**< -40 m */,(float32_T)-1 /**< -1 m */},
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)10576,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)2,
   /**<Section_Size*/ (uint32_t)184
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Esa_Core_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Esa_Core_Cal_Reverse_Array_Esa_Cal(Esa_Core_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_esa_zone_x[0], sizeof(cal_dst->k_esa_zone_x), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_esa_zone_x_hys[0], sizeof(cal_dst->k_esa_zone_x_hys), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_esa_zone_y[0], sizeof(cal_dst->k_esa_zone_y), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_esa_zone_y_hys[0], sizeof(cal_dst->k_esa_zone_y_hys), CT_FOUR_BYTE);
}
#endif /* CT_BIG_ENDIAN */


