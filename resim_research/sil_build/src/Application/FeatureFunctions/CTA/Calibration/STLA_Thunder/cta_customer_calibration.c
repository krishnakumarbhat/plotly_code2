
/**
* @file cta_customer_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the STLA_Thunder specific values according to the corresponding customer specific customer xml-sheet
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
   /**<Cal_Chk_Sum*/ (uint16_t)865,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */
   /**<k_stla_crit_zone_G_E_line*/ (float32_T) 3.0f /**< 3.0 m */,
   /**<k_stla_crit_zone_D_C_line*/ (float32_T) 2.35f /**< 2.35 m */,
   /**<k_stla_crit_zone_N_Q_line*/ (float32_T) 6.0f /**< 6.0 m */,
   /**<k_stla_crit_zone_Q_QH_line*/ (float32_T) 1.0f /**< 1.0 m */
#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */
   /**<k_stla_crit_zone_Q_QH_line*/ (float32_T) 1.0f /**< 1.0 m */,
   /**<k_stla_crit_zone_N_Q_line*/ (float32_T) 6.0f /**< 6.0 m */,
   /**<k_stla_crit_zone_D_C_line*/ (float32_T) 2.35f /**< 2.35 m */,
   /**<k_stla_crit_zone_G_E_line*/ (float32_T) 3.0f /**< 3.0 m */,
   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)865,
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


