/*===================================================================================*\
* FILE:  f360_check_if_object_is_cross_moving.cpp
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential – Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of Is_Object_Cross_Moving() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_determine_cross_moving_obj.h"
#include "f360_math_func.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Object_Cross_Moving()
   *===========================================================================
   * RETURN VALUE:
   * Boolean indicating if the object is cross moving according to some basic criteria
   *
   * PARAMETERS:
   * F360_Object_Track_T& object - reference to object track
   * const F360_Calibrations_T& calib - reference to calibrations struct
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
   * This function detects the cross moving object.
   * Here cross moving is defined when:
   * - object speed is greater than min_abs_vcs_speed
   * - object heading is greater than min_abs_vcs_heading
   * - object azimuth in VCS is lower than max_abs_vcs_azimuth
   * - object azimuth in VCS is greater than min_rear_abs_vcs_azimuth
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Object_Cross_Moving(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {

       const float32_t vcs_heading = F360_Atan2f(object.vcs_velocity.lateral, object.vcs_velocity.longitudinal);
       const float32_t vcs_azimuth = F360_Atan2f(object.vcs_position.y, object.vcs_position.x);
       const float32_t min_rear_abs_vcs_azimuth = F360_PI - calib.k_object_motion_cross_moving_max_abs_vcs_azimuth_th;

      const bool f_cross_mov = ((std::abs(vcs_heading) > calib.k_object_motion_cross_moving_min_abs_vcs_heading_th) && 
          ((std::abs(vcs_azimuth) < calib.k_object_motion_cross_moving_max_abs_vcs_azimuth_th)||
          (std::abs(vcs_azimuth) > min_rear_abs_vcs_azimuth)));

      return f_cross_mov;
   }
}
