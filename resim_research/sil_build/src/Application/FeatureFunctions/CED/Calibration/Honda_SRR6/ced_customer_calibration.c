
/**
* @file ced_customer_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the Honda_SRR6 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "ced_customer_calibration_t.h"
#include "ced_customer_calibration.h"
#include <string.h>

#ifdef CT_BIG_ENDIAN
   #include "ct_endianness_switch.h"
#endif /* CT_BIG_ENDIAN */

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ced_Customer_Cal_Update_Defaults(Ced_Customer_Calibration_T* cal_dst)
{
    Ced_Customer_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)60,
   /**<version*/ (uint16_t)44,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)2056,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_ced_honda_srr6_custom_ttc_alert_threshold*/ {(float32_T)2.8f /**< 2.8 sec */,(float32_T)2.8f /**< 2.8 sec */},
   /**<k_ced_honda_srr6_custom_ttc_alert_hysteresis*/ {(float32_T)0.25f /**< 0.25 sec */,(float32_T)0.25f /**< 0.25 sec */},
   /**<k_ced_honda_srr6_long_dist_threshold*/ {(float32_T)40.0f /**< 40.0 m */,(float32_T)40.0f /**< 40.0 m */},
   /**<k_honda_min_alert_duration*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_honda_elatch_zones_width_table*/ {(float32_T)2.0f /**< 2.0 m */,(float32_T)1.3f /**< 1.3 m */},
   /**<k_honda_min_eratch_alert_duration*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_ced_honda_object_acceleration_weight*/ (float32_T) 0.0f,
   /**<k_ced_f_honda_use_alert_ttc_threshold*/ (boolean_T) 1,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_unused_padding_byte_1*/ (uint8_t) 0,
   /**<k_unused_padding_byte_2*/ (uint8_t) 0
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_unused_padding_byte_2*/ (uint8_t) 0,
   /**<k_unused_padding_byte_1*/ (uint8_t) 0,
   /**<k_unused_padding_byte_0*/ (uint8_t) 0,
   /**<k_ced_f_honda_use_alert_ttc_threshold*/ (boolean_T) 1,
   /**<k_ced_honda_object_acceleration_weight*/ (float32_T) 0.0f,
   /**<k_honda_min_eratch_alert_duration*/ (float32_T) 0.0f /**< 0.0 s */,
   /**<k_honda_elatch_zones_width_table*/ {(float32_T)2.0f /**< 2.0 m */,(float32_T)1.3f /**< 1.3 m */},
   /**<k_honda_min_alert_duration*/ (float32_T) 0.8f /**< 0.8 s */,
   /**<k_ced_honda_srr6_long_dist_threshold*/ {(float32_T)40.0f /**< 40.0 m */,(float32_T)40.0f /**< 40.0 m */},
   /**<k_ced_honda_srr6_custom_ttc_alert_hysteresis*/ {(float32_T)0.25f /**< 0.25 sec */,(float32_T)0.25f /**< 0.25 sec */},
   /**<k_ced_honda_srr6_custom_ttc_alert_threshold*/ {(float32_T)2.8f /**< 2.8 sec */,(float32_T)2.8f /**< 2.8 sec */},
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)2056,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)44,
   /**<Section_Size*/ (uint32_t)60
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Ced_Customer_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ced_Customer_Cal_Reverse_Array_Ced_Cal(Ced_Customer_Calibration_T* cal_dst)
{
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold[0], sizeof(cal_dst->k_ced_honda_srr6_custom_ttc_alert_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis[0], sizeof(cal_dst->k_ced_honda_srr6_custom_ttc_alert_hysteresis), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_ced_honda_srr6_long_dist_threshold[0], sizeof(cal_dst->k_ced_honda_srr6_long_dist_threshold), CT_FOUR_BYTE);
   Ct_Reverse_Array((uint8_t*)&cal_dst->k_honda_elatch_zones_width_table[0], sizeof(cal_dst->k_honda_elatch_zones_width_table), CT_FOUR_BYTE);
}
#endif /* CT_BIG_ENDIAN */


