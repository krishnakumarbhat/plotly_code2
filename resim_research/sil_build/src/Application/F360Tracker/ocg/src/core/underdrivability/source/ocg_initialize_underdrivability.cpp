/*===================================================================================*\
* FILE: ocg_initialize_underdrivability.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Initialize_Underdrivability() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "ocg_initialize_underdrivability.h"

namespace ocg
{
   /*===========================================================================*\
   * FUNCTION: Initialize_Underdrivability()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp::RSPP_Host_T &host,
   * OCG_Underdrivability_Internal_T &underdrivability)
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
   * This function initializes underdrivability
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Initialize_Underdrivability(
       const RSPP_Host_T &host,
       OCG_Underdrivability_Internal_T &in_underdrivability)
   {
      for (uint32_t idx = 0U; idx < NUM_CELLS_X; idx++)
      {
         in_underdrivability.global_zone_idx[idx] = uint16_t(idx);
         OCG_Single_Underdrivability_Zone_T &underdrivability_zone = in_underdrivability.zones[idx];
         for (uint32_t state_idx = 0U; state_idx < UD_HEIGHT_STATE_SIZE; state_idx++)
         {
            underdrivability_zone.state_height_can_pass[state_idx] = 0.0F;
            underdrivability_zone.state_height_is_likely_to_pass[state_idx] = 0.0F;
            underdrivability_zone.state_height_can_not_pass_upper[state_idx] = 0.0F;
            underdrivability_zone.state_height_can_not_pass_lower[state_idx] = 0.0F;
         }

         for (uint32_t state_idx = 0U; state_idx < UD_RCS_STATE_SIZE; state_idx++)
         {
            underdrivability_zone.state_RCS_slope_can_pass[state_idx] = 0.0F;
            underdrivability_zone.state_RCS_slope_is_likely_to_pass[state_idx] = 0.0F;
            underdrivability_zone.state_RCS_slope_can_not_pass_upper[state_idx] = 0.0F;
            underdrivability_zone.state_RCS_slope_can_not_pass_lower[state_idx] = 0.0F;
         }

         underdrivability_zone.p_height_can_pass = 0.0F;
         underdrivability_zone.p_height_is_likely_to_pass = 0.0F;
         underdrivability_zone.p_height_can_not_pass_upper = 0.0F;
         underdrivability_zone.p_height_can_not_pass_lower = 0.0F;

         underdrivability_zone.p_RCS_slope_can_pass = 0.0F;
         underdrivability_zone.p_RCS_slope_is_likely_to_pass = 0.0F;
         underdrivability_zone.p_RCS_slope_can_not_pass_upper = 0.0F;
         underdrivability_zone.p_RCS_slope_can_not_pass_lower = 0.0F;

         underdrivability_zone.p_can_pass = 0.0F;
         underdrivability_zone.p_is_likely_to_pass = 0.0F;
         underdrivability_zone.p_can_not_pass = 0.0F;

         underdrivability_zone.cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         underdrivability_zone.cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] = 1.0F;
         underdrivability_zone.cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] = 0.0F;
         underdrivability_zone.cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] = 0.0F;
         underdrivability_zone.cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER] = 0.0F;
      }
      in_underdrivability.props.circular_buffer_idx = 0U;
      in_underdrivability.props.host_travel_distance = 0.0F;
      in_underdrivability.props.grid_curvature = 0.0F;
      in_underdrivability.props.ogcs_host_rear_axle_initial_position.x = -(host.dist_rear_axle_to_vcs_m + GRID_MIN_X_DIST);
      in_underdrivability.props.ogcs_host_rear_axle_initial_position.y = 0.0F;
      in_underdrivability.props.ogcs_host_rear_axle_initial_position.z = 0.0F;
      in_underdrivability.props.ogcs_host_rear_axle_initial_position.yaw = 0.0F;
   }
}
