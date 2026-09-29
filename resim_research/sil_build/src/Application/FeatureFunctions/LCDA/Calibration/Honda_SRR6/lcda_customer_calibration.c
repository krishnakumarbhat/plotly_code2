
/**
* @file lcda_customer_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the Honda_SRR6 specific values according to the corresponding customer specific customer xml-sheet
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
   /**<Section_Size*/ (uint32_t)64,
   /**<version*/ (uint16_t)91,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)3541,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_honda_max_hold_time_after_out_of_fov*/ (float32_T) 0.25 /**< 0.25 s */,
   /**<k_honda_ego_speed_stop_holding*/ (float32_T) 0.83 /**< 0.83 m/s | 2.99 km/h */,
   /**<k_honda_min_relative_speed_for_alert_level_two*/ (float32_T) 0.35 /**< 0.35 m/s | 1.26 km/h */,
   /**<k_honda_ego_lat_overlap_slide_through_zone*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_honda_object_lat_overlap_slide_through_zone*/ (float32_T) 0.2f /**< 0.2 m */,
   /**<k_honda_beeper_zone_length*/ (float32_T) 3.5f /**< 3.5 m */,
   /**<k_honda_beeper_zone_width*/ (float32_T) 1.5f /**< 1.5 m */,
   /**<k_honda_beeper_zone_long_hys*/ (float32_T) 3.5f /**< 3.5 m */,
   /**<k_honda_beeper_zone_lat_hys*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_honda_alert_level_two_holding_time*/ (float32_T) 1.98f /**< 1.98 s */,
   /**<k_lcda_honda_narrow_beeper_max_speed_h*/ (float32_T) 9.72f /**< 9.72 m/s2 */,
   /**<k_lcda_honda_narrow_beeper_max_speed_l*/ (float32_T) 8.3f /**< 8.3 m/s2 */,
   /**<k_honda_srr6_enable_alert_hold_due_slow_down*/ (boolean_T) 0,
   /**<k_honda_srr6_enable_alert_hold_due_out_of_fov*/ (boolean_T) 1,
   /**<k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two*/ (boolean_T) 1,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two*/ (boolean_T) 1,
   /**<k_honda_srr6_enable_alert_hold_due_out_of_fov*/ (boolean_T) 1,
   /**<k_honda_srr6_enable_alert_hold_due_slow_down*/ (boolean_T) 0,
   /**<k_lcda_honda_narrow_beeper_max_speed_l*/ (float32_T) 8.3f /**< 8.3 m/s2 */,
   /**<k_lcda_honda_narrow_beeper_max_speed_h*/ (float32_T) 9.72f /**< 9.72 m/s2 */,
   /**<k_honda_alert_level_two_holding_time*/ (float32_T) 1.98f /**< 1.98 s */,
   /**<k_honda_beeper_zone_lat_hys*/ (float32_T) 2.0f /**< 2.0 m */,
   /**<k_honda_beeper_zone_long_hys*/ (float32_T) 3.5f /**< 3.5 m */,
   /**<k_honda_beeper_zone_width*/ (float32_T) 1.5f /**< 1.5 m */,
   /**<k_honda_beeper_zone_length*/ (float32_T) 3.5f /**< 3.5 m */,
   /**<k_honda_object_lat_overlap_slide_through_zone*/ (float32_T) 0.2f /**< 0.2 m */,
   /**<k_honda_ego_lat_overlap_slide_through_zone*/ (float32_T) 0.0f /**< 0.0 m */,
   /**<k_honda_min_relative_speed_for_alert_level_two*/ (float32_T) 0.35 /**< 0.35 m/s | 1.26 km/h */,
   /**<k_honda_ego_speed_stop_holding*/ (float32_T) 0.83 /**< 0.83 m/s | 2.99 km/h */,
   /**<k_honda_max_hold_time_after_out_of_fov*/ (float32_T) 0.25 /**< 0.25 s */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)3541,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)91,
   /**<Section_Size*/ (uint32_t)64
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


