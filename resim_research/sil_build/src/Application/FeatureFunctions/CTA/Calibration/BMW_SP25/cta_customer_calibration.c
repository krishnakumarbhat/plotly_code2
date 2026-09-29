
/**
* @file cta_customer_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "cta_customer_calibration_t.h"
#include "cta_customer_calibration.h"
#include <string.h>

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Customer_Cal_Update_Defaults(Cta_Customer_Calibration_T* cal_dst)
{
    Cta_Customer_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)28,
   /**<version*/ (uint16_t)73,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)1026,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_bmw_sp25_ego_abs_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 kph */,
   /**<k_bmw_sp25_banner_criteria_check_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_bmw_sp25_banner_time*/ (float32_T) 7.0f /**< 7.0 s */,
   /**<k_bmw_sp25_banner_criteria_check*/ (boolean_T) 0,
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
   /**<k_bmw_sp25_banner_criteria_check*/ (boolean_T) 0,
   /**<k_bmw_sp25_banner_time*/ (float32_T) 7.0f /**< 7.0 s */,
   /**<k_bmw_sp25_banner_criteria_check_time*/ (float32_T) 0.2f /**< 0.2 s */,
   /**<k_bmw_sp25_ego_abs_speed_max_hys*/ (float32_T) 1.0f /**< 1.0 kph */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)1026,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)73,
   /**<Section_Size*/ (uint32_t)28
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Cta_Customer_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Cta_Customer_Cal_Reverse_Array_Cta_Cal(Cta_Customer_Calibration_T* cal_dst)
{

}
#endif /* CT_BIG_ENDIAN */


