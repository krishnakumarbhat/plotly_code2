/*===========================================================================*\
* FILE: f360_update_object_average_rcs.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Update_Object_Average_Rcs()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_update_object_average_rcs.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Object_Average_Rcs()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const rspp_variant_A::RSPP_Detection_T(&raw_dets)[MAX_NUMBER_OF_DETECTIONS]
   *  const F360_Calibrations_T& calibs
   *  F360_Object_Track_T& object
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function lowpass filters the objects average RCS value based on
   * the objects associated detections. If no detections are associated the
   * average RCS value is not updated.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Object_Average_Rcs(
      const rspp_variant_A::RSPP_Detection_T(&raw_dets)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& object)
   {  
      if (object.ndets > 0U)
      {
         float32_t rcs_vector[MAX_DETS_IN_OBJ_TRK] = {};
         for (uint32_t i = 0U; i < object.ndets; i++)
         {
            const uint32_t det_idx = object.detids[i] - 1U;

            // Convert values into an intermediate format that would enable later computation of logarithmic mean insteaad of 
            // the arithmetic one
            rcs_vector[i] = F360_Powf(10.0F, raw_dets[det_idx].raw.rcs / 20.0F);
         }

         const float32_t intermediate_current_mean = F360_Mean(rcs_vector, object.ndets);

         // mean in decibels (dB)
         const float32_t current_mean = 20.0F * F360_Safe_Log10f(intermediate_current_mean);

         if (F360_OBJECT_STATUS_NEW_UPDATED == object.status)
         {
            object.average_rcs = current_mean;
         }
         else
         {
            object.average_rcs = F360_Low_Pass_Filter_First_Order(current_mean, object.average_rcs, calibs.k_average_rcs_filter_constant);
         }
      }
   }
}

