/*===================================================================================*\
* FILE: ocg_test_med_critical_zones.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Test_Med_Critical_Zones() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
   #pragma warning(push)
   #pragma warning(disable:5045)
#endif

#include "ocg_test_med_critical_zones.h"
#include "cmn_math_constants.h"
#include "cmn_math_func.h"
#include "ocg_test_sufficient_probabilities.h"
#include <algorithm>

namespace ocg
{

/*===========================================================================*\
* FUNCTION: Test_Med_Critical_Zones()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const OCG_Zones_Probabilities_T& probabilities
* const OCG_Underdrivable_Status_T current_underdrivability_status
* const int32_t& circ_buff_zone_idx,
* const OCG_Calibrations_T& calib
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
* This function performs underdrivability tests on medium critical zones.
*
* PRECONDITIONS:
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
void Test_Med_Critical_Zones(const OCG_Zones_Probabilities_T &probabilities, const uint32_t &circ_buff_zone_idx,
                             const OCG_Calibrations_T &calib, OCG_Cell_Classification &cell_classification)
{
   float pr_can_pass_h      = probabilities.can_pass_height[circ_buff_zone_idx];
   float pr_can_not_pass_hu = probabilities.can_not_pass_height_upper[circ_buff_zone_idx];
   (void)pr_can_not_pass_hu; // MISRA
   float pr_likely_to_pass_h = probabilities.is_likely_to_pass_height[circ_buff_zone_idx];
   (void)pr_likely_to_pass_h; // MISRA
   float tolerance = 0.001F;
   (void)tolerance; // MISRA

   cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER] = cmn::OCG_MIN_PROBABILITY;
   cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]  = cmn::OCG_MIN_PROBABILITY;

   if (pr_can_pass_h < calib.underdrive_decision_tree_thr_can_pass_height)
   {
      if (pr_can_not_pass_hu > calib.underdrive_decision_tree_thr_can_not_pass_height_upper)
      {
         cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

         cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] = (1.0F - pr_can_pass_h) * pr_can_not_pass_hu;
         cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] =
            1.0F - cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER];
      }
      else
      {
         if (pr_likely_to_pass_h < calib.underdrive_decision_tree_thr_is_likely_to_pass_height)
         {
            cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

            if ((pr_can_pass_h < tolerance) && (pr_can_not_pass_hu < tolerance) && (pr_likely_to_pass_h < tolerance))
            {
               cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]      = cmn::OCG_MIN_PROBABILITY;
               cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] = cmn::OCG_MIN_PROBABILITY;
               cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER]         = cmn::OCG_MAX_PROBABILITY;
            }
            else
            {
               cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] =
                  (1.0F - pr_can_pass_h) * pr_can_not_pass_hu * (1.0F - pr_likely_to_pass_h);
               cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] =
                  1.0F - cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER];
            }
         }
         else
         {
            cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;

            cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] =
               (1.0F - pr_can_not_pass_hu) * pr_likely_to_pass_h;
            cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] =
               1.0F - cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER];
         }
      }
   }
   else
   {
      cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;

      cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] = pr_can_pass_h;
      cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] =
         1.0F - cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER];
      cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] = cmn::OCG_MIN_PROBABILITY;
   }
}
} // namespace ocg

#ifdef _MSC_VER
   #pragma warning(pop)
#endif
