/*===================================================================================*\
* FILE: ocg_test_highly_critical_zones.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Test_Highly_Critical_Zones() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

#include "cmn_math_func.h"
#include "cmn_math_constants.h"
#include "ocg_test_highly_critical_zones.h"
#include "ocg_test_sufficient_probabilities.h"

namespace ocg
{
   /*===========================================================================*\
   * FUNCTION: Test_Highly_Critical_Zones()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const OCG_Zones_Probabilities_T& probabilities
   * const float n_meas_is_likely_to_pass
   * const OCG_Underdrivable_Status_T current_underdrivability_status
   * const int32_t& zone_idx
   * const int32_t& circ_buff_zone_idx
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
   * This function performs underdrivability tests on high critical zones.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Test_Highly_Critical_Zones(
      const OCG_Zones_Probabilities_T& probabilities,
      const float& n_meas_is_likely_to_pass,
      const uint32_t& zone_idx,
      const uint32_t& circ_buff_zone_idx,
      const OCG_Calibrations_T& calib,
      OCG_Cell_Classification& cell_classification)
   {
      float can_pass_likelihood = 0.0F;
      float can_not_pass_likelihood = 0.0F;

      const bool f_can_pass_strict = Test_Sufficient_Probabilities(
         probabilities.can_pass_height[circ_buff_zone_idx],
         probabilities.can_pass_RCS[circ_buff_zone_idx],
         probabilities.can_not_pass[zone_idx],
         calib.underdrive_breakpoints_height_can_pass_zoneHi_lev2,
         calib.underdrive_breakpoints_slope_can_pass_zoneHi_lev2,
         calib.underdrive_th_p_can_not_pass_zoneHi_lev2,
         calib.underdrive_max_slope_vs_height_index_diff);

      if (f_can_pass_strict)
      {
         can_pass_likelihood += probabilities.can_pass_height[circ_buff_zone_idx] + probabilities.can_pass_RCS[circ_buff_zone_idx];
      }
      else
      {
         can_pass_likelihood += 0.15F * probabilities.can_pass_height[circ_buff_zone_idx];
         can_not_pass_likelihood += 0.6F + probabilities.can_not_pass[zone_idx];
      }

      const bool f_is_likely_to_pass_strict = (calib.underdrive_high_crit_zone_is_likely_to_pass_min_number_of_dets < n_meas_is_likely_to_pass) &&
         (Test_Sufficient_Probabilities(probabilities.is_likely_to_pass_height[circ_buff_zone_idx],
            probabilities.is_likely_to_pass_RCS[circ_buff_zone_idx],
            probabilities.can_not_pass[zone_idx],
            calib.underdrive_breakpoints_height_is_likely_to_pass_zoneHi_lev2,
            calib.underdrive_breakpoints_slope_is_likely_to_pass_zoneHi_lev2,
            calib.underdrive_th_p_can_not_pass_zoneHi_lev2,
            calib.underdrive_max_slope_vs_height_index_diff));

      if (f_is_likely_to_pass_strict)
      {
         can_pass_likelihood += probabilities.is_likely_to_pass_height[circ_buff_zone_idx] + probabilities.is_likely_to_pass_RCS[circ_buff_zone_idx];
      }
      else
      {
         can_pass_likelihood += 0.15F * probabilities.is_likely_to_pass_height[circ_buff_zone_idx];
         can_not_pass_likelihood += 0.3F + probabilities.can_not_pass[zone_idx];
      }

      const float mean_can_not_pass = cmn::Mean(probabilities.can_not_pass, zone_idx + 1U);
      const float mean_is_likely_to_pass = cmn::Mean(probabilities.is_likely_to_pass, zone_idx + 1U);
      const bool f_tunnel_detected_strict = (UNDERDRIVABLE_STATUS_CAN_PASS_UNDER == cell_classification.underdrivability_status) &&
         (calib.underdrive_tunnel_strict_min_is_likely_to_pass_mean_val < mean_is_likely_to_pass) &&
         (mean_can_not_pass < calib.underdrive_tunnel_strict_max_can_not_pass_mean_val_high_zone);

      if (f_tunnel_detected_strict)
      {
         can_pass_likelihood += mean_is_likely_to_pass;
         can_not_pass_likelihood *= 0.5F;
      }
      else
      {
         can_not_pass_likelihood += 0.1F + mean_can_not_pass;
      }

      const bool f_can_pass = (f_can_pass_strict || f_is_likely_to_pass_strict || f_tunnel_detected_strict);
      if (f_can_pass)
      {
         cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
      }
      else
      {
         cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
      }

      // Normalize likelihoods
      const float likelihoods_sum = can_pass_likelihood + can_not_pass_likelihood;
      cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] = can_not_pass_likelihood / likelihoods_sum;
      cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] = cmn::OCG_MIN_PROBABILITY;
      cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] = cmn::OCG_MAX_PROBABILITY - cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER];
      cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER] = cmn::OCG_MIN_PROBABILITY;

   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
