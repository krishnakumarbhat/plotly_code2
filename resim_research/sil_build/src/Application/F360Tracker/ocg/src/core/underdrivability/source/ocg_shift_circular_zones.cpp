/*===================================================================================*\
* FILE: ocg_shift_circular_zones.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Shift_Circular_Zones() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include <cmath>
#include "cmn_math_func.h"
#include "ocg_shift_circular_zones.h"

namespace ocg
{

   /*===========================================================================*\
   * FUNCTION: Shift_Circular_Zones()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Underdrivability_Internal_T& underdrivability
   * const RSPP_Host_Props_T& host_props
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
   * This function handles circular buffer idx.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Shift_Circular_Zones(
       OCG_Underdrivability_Internal_T &in_underdrivability,
       const RSPP_Host_T host)
   {
      // Calculate pointing angle delta from previous iteration
      const float delta_pointing = host.yaw_rate_rad * in_underdrivability.props.timestamp_delta_s;
      const float cos_delta_pointing = cosf(delta_pointing);
      const float sin_delta_pointing = sinf(delta_pointing);

      /* Calculate distance that host has moved from previous iteration in x (denoted as delta_position_x) and
       *  y (denoted as delta_position_y) direction for a coordinate system aligned with the heading angle from previous iteration.
       *  Note that these calculations assumes that host is driving on a circle */
      float deltaXY[2][1];
      if (std::fabs(host.yaw_rate_rad) > 0.0001F)
      {
         const float curve_radius = host.vcs_speed / host.yaw_rate_rad;

         deltaXY[0][0] = curve_radius * sin_delta_pointing;
         deltaXY[1][0] = curve_radius * (1.0F - cos_delta_pointing);
      }
      else
      {
         // Same calculation as in if statement but using small angle approximation of sin and cos
         deltaXY[0][0] = host.vcs_speed * in_underdrivability.props.timestamp_delta_s;
         deltaXY[1][0] = 0.5F * host.yaw_rate_rad * host.vcs_speed * (in_underdrivability.props.timestamp_delta_s * in_underdrivability.props.timestamp_delta_s);
      }
      /* We store distance that host has moved from previous iteration given in the old VCS coordinate system
      from previous scan so we need to do a rotation of deltaXY (from old heading aligned to old pointing aligned system)
      Calculation of delta_pos_x and delta_pos_y assumes a circular motion--> constant sideslip-->
      previous vcs_sideslip is not required to obtain heading angle from previous iteration */
      const float c_prev_side_slip = cosf(host.vcs_sideslip);
      const float s_prev_side_slip = sinf(host.vcs_sideslip);
      const float delta_position_x = c_prev_side_slip * deltaXY[0][0] - s_prev_side_slip * deltaXY[1][0];
      // float delta_position_y = s_prev_side_slip * deltaXY[0][0] + c_prev_side_slip * deltaXY[1][0];

      in_underdrivability.props.host_travel_distance += delta_position_x;

      const int number_of_cells_to_subtract = static_cast<int> (floor(in_underdrivability.props.host_travel_distance / CELL_LENGTH));

      if (number_of_cells_to_subtract > 0)
      {
          in_underdrivability.props.host_travel_distance -= number_of_cells_to_subtract * CELL_LENGTH;

          for (int i = 0; i < number_of_cells_to_subtract; i++)
          {
              OCG_Single_Underdrivability_Zone_T(&zones)[NUM_CELLS_X] = in_underdrivability.zones;
              //  Clear data that is no longer relevant since the zone is too close
              const uint16_t zone_to_clear_idx = in_underdrivability.props.circular_buffer_idx;
              for (uint32_t state_idx = 0U; state_idx < UD_HEIGHT_STATE_SIZE; state_idx++)
              {
                  zones[zone_to_clear_idx].state_height_can_pass[state_idx] = 0.0F;
                  zones[zone_to_clear_idx].state_height_is_likely_to_pass[state_idx] = 0.0F;
                  zones[zone_to_clear_idx].state_height_can_not_pass_upper[state_idx] = 0.0F;
                  zones[zone_to_clear_idx].state_height_can_not_pass_lower[state_idx] = 0.0F;
              }
              for (uint32_t state_idx = 0U; state_idx < UD_RCS_STATE_SIZE; state_idx++)
              {
                  zones[zone_to_clear_idx].state_RCS_slope_can_pass[state_idx] = 0.0F;
                  zones[zone_to_clear_idx].state_RCS_slope_is_likely_to_pass[state_idx] = 0.0F;
                  zones[zone_to_clear_idx].state_RCS_slope_can_not_pass_upper[state_idx] = 0.0F;
                  zones[zone_to_clear_idx].state_RCS_slope_can_not_pass_lower[state_idx] = 0.0F;
              }
              zones[zone_to_clear_idx].p_height_can_pass = 0.0F;
              zones[zone_to_clear_idx].p_height_is_likely_to_pass = 0.0F;
              zones[zone_to_clear_idx].p_height_can_not_pass_upper = 0.0F;
              zones[zone_to_clear_idx].p_height_can_not_pass_lower = 0.0F;
              zones[zone_to_clear_idx].p_RCS_slope_can_pass = 0.0F;
              zones[zone_to_clear_idx].p_RCS_slope_is_likely_to_pass = 0.0F;
              zones[zone_to_clear_idx].p_RCS_slope_can_not_pass_upper = 0.0F;
              zones[zone_to_clear_idx].p_RCS_slope_can_not_pass_lower = 0.0F;
              for (uint32_t zone_idx = 1U; zone_idx < NUM_CELLS_X; zone_idx++)
              {
                  zones[zone_idx - 1U].cell_classification.underdrivability_status = zones[zone_idx].cell_classification.underdrivability_status;
                  in_underdrivability.global_zone_idx[zone_idx - 1U] = in_underdrivability.global_zone_idx[zone_idx];
              }
              in_underdrivability.global_zone_idx[NUM_CELLS_X - 1] = in_underdrivability.global_zone_idx[NUM_CELLS_X - 1] + 1;
              zones[NUM_CELLS_X - 1U].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;

              in_underdrivability.props.circular_buffer_idx++;
              if (NUM_CELLS_X == in_underdrivability.props.circular_buffer_idx)
              {
                  in_underdrivability.props.circular_buffer_idx = 0U;
              }

          }
      
      }
   }

}
