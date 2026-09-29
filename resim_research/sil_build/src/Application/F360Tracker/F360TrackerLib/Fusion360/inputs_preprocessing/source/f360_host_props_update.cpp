/*===================================================================================*\
* FILE: f360_host_state_update.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions which updates host properties with calculated value.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


/******************************
* Includes
*******************************/
#include "f360_math.h"

#include "f360_matrix_dimension.h"
#include "f360_host_props_update.h"
#include "f360_norm_heading_angle.h"
#include "f360_math_func.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Host_Props_Update()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t elapsed_time - Time elapsed since last time Tracker executed
   * const F360_Host_T* const host - Host Info Structure
   * F360_Host_T* const host_props - Host properties Structure
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
   * Host_State_Update function updates host properties with calculated value.
   *
   * PRECONDITIONS:
   * All the Pointer should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Host_Props_Update(
      const float32_t elapsed_time,
      const F360_Host_T& host,
      F360_Host_Props_T& host_props)
   {
      // Calculate pointing angle delta from previous tracker iteration
      host_props.delta_pointing = host.yaw_rate_rad * elapsed_time;
      host_props.cos_delta_pointing = F360_Cosf(host_props.delta_pointing);
      host_props.sin_delta_pointing = F360_Sinf(host_props.delta_pointing);

      /* Calculate distance that host has moved from previous tracker iteration in x (denoted as delta_position.x) and
      *  y (denoted as delta_position.y) direction for a coordinate system aligned with the heading angle from previous tracker iteration.
      *  Note that these calculations assumes that host is driving on a circle */
      float32_t deltaXY[2][1];
      if (std::abs(host.yaw_rate_rad) > 0.0001F)
      {
         const float32_t curve_radius = host.vcs_speed / host.yaw_rate_rad;

         deltaXY[F360_2D_IDX_X][F360_2D_IDX_X] = curve_radius * host_props.sin_delta_pointing;
         deltaXY[F360_2D_IDX_Y][F360_2D_IDX_X] = curve_radius * (1.0F - host_props.cos_delta_pointing);
      }
      else
      {
         // Same calculation as in if statement but using small angle approximation of sin and cos
         deltaXY[F360_2D_IDX_X][F360_2D_IDX_X] = host.vcs_speed * elapsed_time;
         deltaXY[F360_2D_IDX_Y][F360_2D_IDX_X] = 0.5F * host.yaw_rate_rad * host.vcs_speed * (elapsed_time * elapsed_time);
      }
      /* In host props we store distance that host has moved from previous tracker given in the old VCS coordinate system
      from previous scan so we need to do a rotation of deltaXY (from old heading aligned to old pointing aligned system)
      Calculation of delta_pos_x and delta_pos_y assumes a circular motion--> constant sideslip-->
      previous vcs_sideslip is not required to obtain heading angle from previous tracker iteration */
      const float32_t c_prev_side_slip = F360_Cosf(host.vcs_sideslip);
      const float32_t s_prev_side_slip = F360_Sinf(host.vcs_sideslip);
      host_props.delta_position_x = c_prev_side_slip * deltaXY[F360_2D_IDX_X][F360_2D_IDX_X] - s_prev_side_slip * deltaXY[F360_2D_IDX_Y][F360_2D_IDX_X];
      host_props.delta_position_y = s_prev_side_slip * deltaXY[F360_2D_IDX_X][F360_2D_IDX_X] + c_prev_side_slip * deltaXY[F360_2D_IDX_Y][F360_2D_IDX_X];

      /* Heading angle in previous tracker iteration
      Calculation of delta_pos_x and delta_pos_y assumes a circular motion --> constant sideslip -->
      previous vcs_sideslip is not required to obtain heading angle from previous tracker iteration
      Note: host_props.heading_angle is filled with host pointing angle */
      const float32_t prev_heading_angle = host_props.heading_angle + host.vcs_sideslip;

      const float32_t cangle = F360_Cosf(prev_heading_angle);
      const float32_t sangle = F360_Sinf(prev_heading_angle);

      // Initialize Rotation matrix to rotate delta_pos_x and delta_pos_y to WCS
      float32_t rot_mat[F360_2D_IDX_DIMENSION][F360_2D_IDX_DIMENSION];
      rot_mat[F360_2D_IDX_X][F360_2D_IDX_X] = cangle;
      rot_mat[F360_2D_IDX_X][F360_2D_IDX_Y] = -sangle;
      rot_mat[F360_2D_IDX_Y][F360_2D_IDX_X] = sangle;
      rot_mat[F360_2D_IDX_Y][F360_2D_IDX_Y] = cangle;

      float32_t pos_inc_wcs[2][1];
      // Rotational transformation of delta_pos_x and delta_pos_y to WCS
      F360_matmul_2x2_2x1(rot_mat, deltaXY, pos_inc_wcs);

      // Host position update
      host_props.position_x += pos_inc_wcs[F360_2D_IDX_X][F360_2D_IDX_X];
      host_props.position_y += pos_inc_wcs[F360_2D_IDX_Y][F360_2D_IDX_X];

      // Calculate cumulative pointing angle, i.e. pointing angle for current tracker iteration
      // Note: host_props.heading_angle is filled with host pointing angle
      host_props.heading_angle = Normalize_Heading_Angle(host_props.heading_angle + host_props.delta_pointing, 0.0F);

      // Update cos and sin of host pointing angle
      host_props.cos_heading = F360_Cosf(host_props.heading_angle);
      host_props.sin_heading = F360_Sinf(host_props.heading_angle);
   }
}
