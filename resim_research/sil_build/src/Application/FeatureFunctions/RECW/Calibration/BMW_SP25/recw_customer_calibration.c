
/**
* @file recw_customer_calibration.c
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the BMW_SP25 specific values according to the corresponding customer specific customer xml-sheet
* for the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

#include "recw_customer_calibration_t.h"
#include "recw_customer_calibration.h"
#include <string.h>

#include "ct_calibration_header_t.h" // IWYU pragma: keep
#include "pa_reuse.h"

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Recw_Customer_Cal_Update_Defaults(Recw_Customer_Calibration_T* cal_dst)
{
    Recw_Customer_Calibration_T default_calibration= 
    {
#ifndef CT_BIG_ENDIAN
/* Assumed order for little endian */
   /* Ct_Header_T */
   {
   /**<Section_Size*/ (uint32_t)12,
   /**<version*/ (uint16_t)16,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Type*/ (uint8_t)3u
   },
   /* Component Calibration */

#endif /* CT_BIG_ENDIAN */

#ifdef CT_BIG_ENDIAN
/* Assumed order for big endian */
   /* Component Calibration */

   /* Ct_Header_T */
   {
   /**<Cal_Type*/ (uint8_t)3u,
   /**<Chk_sum_Version*/ (uint8_t)1u,
   /**<Cal_Chk_Sum*/ (uint16_t)0,
   /**<Section_Compatibility*/ (uint16_t)3u,
   /**<version*/ (uint16_t)16,
   /**<Section_Size*/ (uint32_t)12
   }
#endif /* CT_BIG_ENDIAN */
};

/* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memcpy function since it is not required.] */
/* coverity[store_writes_const_field][Intentionally override all with default values] */
    memcpy((void*)cal_dst, (void*)&default_calibration, sizeof(Recw_Customer_Calibration_T));
}

#ifdef CT_BIG_ENDIAN
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Recw_Customer_Cal_Reverse_Array_Recw_Cal(Recw_Customer_Calibration_T* cal_dst)
{

}
#endif /* CT_BIG_ENDIAN */


