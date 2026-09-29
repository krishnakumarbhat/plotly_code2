
/**
* @file fbk_public_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the STLA_Thunder specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in fbk_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "fbk_public_calibration_t.h"
#include "fbk_public_calibration.h"
#include <string.h>

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Fbk_Public_Cal_Update_Defaults(Fbk_Public_Calibration_T* cal_dst)
{
    Fbk_Public_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)1,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_fbk_host_trail_max_recording_speed*/ (float32_T) 15.0f /**< 15.0 m/s | 54.0 km/h */,
   /**<k_fbk_host_trail_dist_separation*/ (float32_T) 5.0f /**< 5.0 s */,
   /**<k_fbk_host_trail_heading_separation*/ (float32_T) 0.2617f /**< 0.2617 rad | 14.99 deg */
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_fbk_host_trail_heading_separation*/ (float32_T) 0.2617f /**< 0.2617 rad | 14.99 deg */,
   /**<k_fbk_host_trail_dist_separation*/ (float32_T) 5.0f /**< 5.0 s */,
   /**<k_fbk_host_trail_max_recording_speed*/ (float32_T) 15.0f /**< 15.0 m/s | 54.0 km/h */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)1,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Fbk_Public_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Fbk_Public_Cal_Reverse_Array_Fbk_Cal(Fbk_Public_Calibration_T* cal_dst)
{

}
#endif /* CT_BIG_ENDIAN */


