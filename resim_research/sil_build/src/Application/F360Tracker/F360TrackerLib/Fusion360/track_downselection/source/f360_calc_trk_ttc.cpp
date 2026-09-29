/*===================================================================================*\
* FILE: f360_calc_trk_ttc.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv. All Rights Reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Calc_Trk_Ttc()
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#include "f360_math.h"
#include <cstring>
#include "f360_calc_trk_ttc.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
/*===========================================================================*\
   * FUNCTION: Calc_Trk_Ttc()
   *===========================================================================
   * RETURN VALUE:
   *  float32_t
   *
   * PARAMETERS:
   *  const F360_Object_Track_T& obj_trk
   *  const F360_Host_T& host
   *  const float32_t cos_host_vcs_sideslip,
   *  const float32_t sin_host_vcs_sideslip,
   *  const F360_Calibrations_T& calib
   *  const float32_t range
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
   * This function calculates track time to collision (ttc).
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
\*===========================================================================*/
   float32_t Calc_Trk_Ttc(
      const F360_Object_Track_T& obj_trk,
      const F360_Host_T& host,
      const float32_t cos_host_vcs_sideslip,
      const float32_t sin_host_vcs_sideslip,
      const F360_Calibrations_T& calib,
      const float32_t range)
   {
      float32_t rel_pos[2] = {};
      float32_t rel_vel[2] = {};
      float32_t proj_vel;

      rel_pos[0] = obj_trk.vcs_position.x;
      rel_pos[1] = obj_trk.vcs_position.y;

      rel_vel[0] = obj_trk.vcs_velocity.longitudinal - (cos_host_vcs_sideslip * host.vcs_speed);
      rel_vel[1] = obj_trk.vcs_velocity.lateral - (sin_host_vcs_sideslip * host.vcs_speed);

      if (0.0F < range)
      {
         proj_vel = F360_Vector_Multiplication(rel_pos, rel_vel) / range;
      }
      else
      {
         proj_vel = calib.k_calc_ttc_max_thresh_projected_velocity;
      }

      float32_t ttc;
      if (std::abs(proj_vel) < calib.k_calc_ttc_min_thresh_projected_velocity)
      {
         if (proj_vel >= 0.0F)
         {
            ttc = -range / calib.k_calc_ttc_min_thresh_projected_velocity;
         }
         else
         {
            ttc = range / calib.k_calc_ttc_min_thresh_projected_velocity;
         }
      }
      else
      {
         if (F360_EPSILON < std::abs(proj_vel))
         {
            ttc = -range / proj_vel;
         }
         else
         {
            ttc = -range / F360_EPSILON;
         }
      }
      return ttc;
   }
}
