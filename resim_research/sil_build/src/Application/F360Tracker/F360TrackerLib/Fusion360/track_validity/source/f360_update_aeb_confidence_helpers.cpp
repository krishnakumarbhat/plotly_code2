/*===================================================================================*\
* FILE: f360_update_aeb_confidence_helpers.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains helper definition(s) for Update_AEB_Confidence().
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_update_aeb_confidence_helpers.h"
#include "f360_reference_point_support_functions.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Compute_Iso_Relative_X_Vel
   *===========================================================================
   * RETURN VALUE:
   * float32_t - ISO relative longitudinal velocity of the object with
   * respect to the host vehicle [m/s]
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object,
   * const F360_Host_T& host
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
   * Computes the ISO-defined relative longitudinal velocity between the
   * object and the host vehicle.
   *
   * For CTCA objects, the velocity is tracked at the rear center and must be
   * transformed to the reference point first:
   *   v_ref_pnt = v_rear_center + (yaw_rate x vec_from_rear_center_to_ref_pnt)
   * For CCA objects, velocity is already at the reference point.
   *
   * The relative velocity is then computed as:
   *   v_rel_x = v_obj_x - v_host_x - host_yaw_coupling_x
   * where:
   *   v_obj_x             = longitudinal VCS velocity at the object reference point
   *   v_host_x            = vcs_speed * cos(sideslip)
   *   host_yaw_coupling_x = -yaw_rate_vcs * y_obj_vcs
   *                         (x-component of cross product [0,0,omega] x [x,y,0])
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Compute_Iso_Relative_X_Vel(const F360_Object_Track_T& object, const F360_Host_T& host)
   {
      float32_t longi_vcs_velocity_in_ref_pnt = object.vcs_velocity.longitudinal;

      // Only transform velocity from rear center to reference point for CTCA objects.
      // CCA objects already have velocity at the reference point.
      if (F360_TRACKER_TRKFLTR_CTCA == object.trk_fltr_type)
      {
         const Point vec_from_center_to_ref_pnt_tcs = Get_Reference_Point_Pos_In_TCS(object.reference_point, object.bbox.Get_Length(), object.bbox.Get_Width());
         const float32_t vec_from_rear_center_to_ref_pnt_tcs[2] = { vec_from_center_to_ref_pnt_tcs.x + 0.5F * object.bbox.Get_Length(), vec_from_center_to_ref_pnt_tcs.y };
         float32_t vec_from_rear_center_to_ref_pnt_vcs[2];
         F360_Rotate_2D_Vector(vec_from_rear_center_to_ref_pnt_tcs[0], vec_from_rear_center_to_ref_pnt_tcs[1], object.bbox.Get_Orientation().Cos(), object.bbox.Get_Orientation().Sin(), vec_from_rear_center_to_ref_pnt_vcs[0], vec_from_rear_center_to_ref_pnt_vcs[1]);

         // v_ref_pnt_x = v_rear_center_x + (yaw_rate x vec)_x
         // where (yaw_rate x vec)_x = -yaw_rate * vec_y
         longi_vcs_velocity_in_ref_pnt += -object.heading_rate * vec_from_rear_center_to_ref_pnt_vcs[1];
      }

      // v_rel_x = v_obj_x - v_host_x - host_yaw_coupling_x
      const float32_t host_velocity_x = host.vcs_speed * F360_Cosf(host.vcs_sideslip);
      const float32_t host_yaw_coupling_x = -host.yaw_rate_rad * object.vcs_position.y;
      const float32_t iso_relative_x_vel = longi_vcs_velocity_in_ref_pnt - host_velocity_x - host_yaw_coupling_x;

      return iso_relative_x_vel;
   }
}
