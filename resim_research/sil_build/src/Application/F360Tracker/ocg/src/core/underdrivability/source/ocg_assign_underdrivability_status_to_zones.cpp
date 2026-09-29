/*===================================================================================*\
* FILE: ocg_assign_underdrivability_status_to_zones.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Underdrivability_Status_To_Zones() function definition
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

#include <algorithm>
#include "cmn_math_func.h"
#include "cmn_math_constants.h"
#include "ocg_assign_underdrivability_status_to_zones.h"
#include "ocg_initialize_underdrivability.h"
#include "ocg_test_sufficient_probabilities.h"
#include "ocg_test_highly_critical_zones.h"
#include "ocg_test_med_critical_zones.h"
#include "ocg_test_low_critical_zones.h"
#include "cmn_calc_circular_zone_idx.h"

namespace ocg
{
   /*===========================================================================*\
   * FUNCTION: Calc_Range_Limits_For_TTC_Criticality_Zone()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const RSPP_Host_T& vehicle_data
   * const OCG_Calibrations_T& calib
   * float& max_dist_for_crit_zone
   * float& max_dist_for_med_zone
   * float& max_dist_for_low_zone
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
   * Function calculates range limits for TTC thresholds.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Calc_Range_Limits_For_TTC_Criticality_Zone(
       const RSPP_Host_T &host,
       const OCG_Calibrations_T &calib,
       float &max_dist_for_crit_zone,
       float &max_dist_for_med_zone,
       float &max_dist_for_low_zone)
   {
      // [m] Limit how small highly critical zone can be. Minimum corresponds to TTC = 2.0s at 45kph or at least one range zone if zone width is large
      const float single_zone_range = std::max(calib.underdrive_time_zone_crit_hi * calib.underdrive_slow_host_speed_th, GRID_MIN_X_DIST + CELL_LENGTH);

      // [m] Limit how small medium critical zone can be. Minimum corresponds to TTC = 2.9s at 45kph or at least two range zones if zone width is large
      const float double_zone_range = std::max(calib.underdrive_time_zone_crit_med * calib.underdrive_slow_host_speed_th, GRID_MIN_X_DIST + 2.0F * CELL_LENGTH);

      // [m] Limit how small low critical can be. Minimum corresponds to TTC = 4.0s at 45kph or at least three range zones if zone width is large
      const float triple_zone_range = std::max(calib.underdrive_time_zone_crit_low * calib.underdrive_slow_host_speed_th, GRID_MIN_X_DIST + 3.0F * CELL_LENGTH);

      // Compute range to critical zones
      const float range_zone_crit_hi = calib.underdrive_time_zone_crit_hi * host.speed;
      const float range_zone_crit_med = calib.underdrive_time_zone_crit_med * host.speed;
      const float range_zone_crit_low = calib.underdrive_time_zone_crit_low * host.speed;
      max_dist_for_crit_zone = std::max(range_zone_crit_hi, single_zone_range);
      max_dist_for_med_zone = std::max(range_zone_crit_med, double_zone_range);
      max_dist_for_low_zone = std::max(range_zone_crit_low, triple_zone_range);
   }

   /*===========================================================================*\
   * FUNCTION: Extract_Probabilities()
   *===========================================================================
   * RETURN VALUE:
   * OCG_Zones_Probabilities_T
   *
   * PARAMETERS:
   * const OCG_Single_Underdrivability_Zone_T (&zones)[NUM_CELLS_X] - Underdrivability zones
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
   * Function extracts and saturates probabilities.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   static OCG_Zones_Probabilities_T Extract_Probabilities(
       const OCG_Single_Underdrivability_Zone_T (&zones)[NUM_CELLS_X])
   {
      OCG_Zones_Probabilities_T probabilities;
      for (uint32_t zone_idx = 0U; zone_idx < NUM_CELLS_X; zone_idx++)
      {
         // Negative probabilities is an indication of invalidity. Set probabilities to 0 if negative
         probabilities.can_pass_height[zone_idx] = ocg::cmn::Saturate(zones[zone_idx].p_height_can_pass, ocg::cmn::OCG_MIN_PROBABILITY, ocg::cmn::OCG_MAX_PROBABILITY);
         probabilities.can_pass_RCS[zone_idx] = cmn::Saturate(zones[zone_idx].p_RCS_slope_can_pass, ocg::cmn::OCG_MIN_PROBABILITY, ocg::cmn::OCG_MAX_PROBABILITY);
         probabilities.is_likely_to_pass[zone_idx] = cmn::Saturate(zones[zone_idx].p_is_likely_to_pass, ocg::cmn::OCG_MIN_PROBABILITY, ocg::cmn::OCG_MAX_PROBABILITY);
         probabilities.is_likely_to_pass_height[zone_idx] = cmn::Saturate(zones[zone_idx].p_height_is_likely_to_pass, ocg::cmn::OCG_MIN_PROBABILITY, ocg::cmn::OCG_MAX_PROBABILITY);
         probabilities.is_likely_to_pass_RCS[zone_idx] = cmn::Saturate(zones[zone_idx].p_RCS_slope_is_likely_to_pass, ocg::cmn::OCG_MIN_PROBABILITY, ocg::cmn::OCG_MAX_PROBABILITY);
         probabilities.can_not_pass[zone_idx] = cmn::Saturate(zones[zone_idx].p_can_not_pass, ocg::cmn::OCG_MIN_PROBABILITY, ocg::cmn::OCG_MAX_PROBABILITY);
      }
      return probabilities;
   }

   /*===========================================================================*\
   * FUNCTION: Test_All_Zones()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Underdrivability_Internal_T& underdrivability
   * const OCG_Calibrations_T& calib
   * const OCG_Zones_Probabilities_T& probabilities
   * const float max_dist_for_crit_zone
   * const float max_dist_for_med_zone
   * const float max_dist_for_low_zone
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
   * Function determines zone criticality level and calls proper test function.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Test_All_Zones(
       OCG_Underdrivability_Internal_T &underdrivability,
       const OCG_Calibrations_T &calib,
       const OCG_Zones_Probabilities_T &probabilities,
       const float max_dist_for_crit_zone,
       const float max_dist_for_med_zone,
       const float max_dist_for_low_zone)
   {
      OCG_Single_Underdrivability_Zone_T(&zones)[NUM_CELLS_X] = underdrivability.zones;

      for (uint32_t zone_idx = 0U; zone_idx < NUM_CELLS_X; zone_idx++)
      {
         const float zone_start_dist = GRID_MIN_X_DIST + CELL_LENGTH * static_cast<float>(zone_idx);

         if (max_dist_for_low_zone <= zone_start_dist)
         {
            zones[zone_idx].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
            zones[zone_idx].cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] = cmn::OCG_MIN_PROBABILITY;
            zones[zone_idx].cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] = cmn::OCG_MIN_PROBABILITY;
            zones[zone_idx].cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] = cmn::OCG_MIN_PROBABILITY;
            zones[zone_idx].cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER] = cmn::OCG_MAX_PROBABILITY;

         }
         else
         {
            const uint32_t circ_buff_zone_idx = cmn::Calc_Circular_Zone_Idx(underdrivability.props.circular_buffer_idx, zone_idx, NUM_CELLS_X);

            // High criticality zone(44 meters @ 80 kph)
            if (zone_start_dist < max_dist_for_crit_zone)
            {
               const float n_meas_is_likely_to_pass = std::min(
                   zones[circ_buff_zone_idx].state_height_is_likely_to_pass[0],
                   zones[circ_buff_zone_idx].state_RCS_slope_is_likely_to_pass[0]);

               Test_Highly_Critical_Zones(
                   probabilities,
                   n_meas_is_likely_to_pass,
                   zone_idx,
                   circ_buff_zone_idx,
                   calib,
                   zones[zone_idx].cell_classification);
            }

            // Medium criticality zone(64 meters @ 80kph)
            else if (zone_start_dist < max_dist_for_med_zone)
            {
               const float n_meas_is_likely_to_pass = std::min(
                   zones[circ_buff_zone_idx].state_height_is_likely_to_pass[0],
                   zones[circ_buff_zone_idx].state_RCS_slope_is_likely_to_pass[0]);

               Test_Med_Critical_Zones(
                   probabilities,
                   n_meas_is_likely_to_pass,
                   zone_idx,
                   circ_buff_zone_idx,
                   calib,
                   zones[zone_idx].cell_classification);
            }
            // Low criticality zone(89 meters @ 80kph)
            else
            {
               Test_Low_Critical_Zones(
                   probabilities,
                   zone_idx,
                   circ_buff_zone_idx,
                   calib,
                   zones[zone_idx].cell_classification);
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Underdrivability_Status_To_Zones()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Underdrivability_Internal_T& underdrivability
   * const RSPP_Host_T& vehicle_data
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
   * Function assigns underdrivability status to zones.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Assign_Underdrivability_Status_To_Zones(
       OCG_Underdrivability_Internal_T &in_underdrivability,
       const RSPP_Host_T &host,
       const OCG_Calibrations_T &calib)
   {
      float max_dist_for_crit_zone = 0.0F;
      float max_dist_for_med_zone = 0.0F;
      float max_dist_for_low_zone = 0.0F;

      Calc_Range_Limits_For_TTC_Criticality_Zone(host, calib, max_dist_for_crit_zone, max_dist_for_med_zone, max_dist_for_low_zone);

      const OCG_Zones_Probabilities_T probabilities = Extract_Probabilities(in_underdrivability.zones);

      Test_All_Zones(in_underdrivability, calib, probabilities, max_dist_for_crit_zone, max_dist_for_med_zone, max_dist_for_low_zone);
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
