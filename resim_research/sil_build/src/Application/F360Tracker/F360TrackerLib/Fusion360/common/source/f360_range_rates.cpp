/*===================================================================================*\
* FILE: f360_range_rates.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions declarations for different types/approaches
*   range rate calculations.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_range_rates.h"
#include "f360_math_func.h"
#include "f360_vector.h"
#include <limits>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calculate_Det_Range_Rate_Comp()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - compensated range rate.
   *
   * PARAMETERS:
   *  const float32_t cos_azimuth_vcs,
   *  const float32_t sin_azimuth_vcs,
   *  const float32_t det_range_rate_raw,
   *  const float32_t sensor_lat_vel_vcs,
   *  const float32_t sensor_long_vel_vcs
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
   * Calculate detection compensated range rate.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calculate_Det_Range_Rate_Comp(
      const float32_t cos_azimuth_vcs,
      const float32_t sin_azimuth_vcs,
      const float32_t det_range_rate_raw,
      const float32_t sensor_lat_vel_vcs,
      const float32_t sensor_long_vel_vcs
   )
   {
      const float32_t rdot_pred = Calculate_OTG_Range_Rate_From_Velocity(cos_azimuth_vcs, sin_azimuth_vcs, sensor_long_vel_vcs, sensor_lat_vel_vcs);
      return (det_range_rate_raw + rdot_pred);
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_OTG_Range_Rate_From_Velocity()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - OTG (over-the-ground) range rate.
   *
   * PARAMETERS:
   *  const float32_t cos_azimuth_vcs,
   *  const float32_t sin_azimuth_vcs,
   *  const float32_t otg_long_vel_vcs,
   *  const float32_t otg_lat_vel_vcs
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
   * Calculate OTG range rate from OTG velocity vector.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calculate_OTG_Range_Rate_From_Velocity(
      const float32_t cos_azimuth_vcs,
      const float32_t sin_azimuth_vcs,
      const float32_t otg_long_vel_vcs,
      const float32_t otg_lat_vel_vcs
   )
   {
      return (otg_long_vel_vcs * cos_azimuth_vcs + otg_lat_vel_vcs * sin_azimuth_vcs);
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Projected_Range_Rate()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - OTG (over-the-ground) range rate or NaN.
   *
   * PARAMETERS:
   *  const Point& view_point,
   *  const Point& item_position
   *  const F360_VCS_Velocity_T& item_velocity
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
   * Calculate value of projected range rate. Projection is done on the line 
   * described by points view_point and item_position.
   *
   * PRECONDITIONS:
   * view_point != item_position
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calculate_Projected_Range_Rate(
      const Point& view_point,
      const Point& item_position,
      const F360_VCS_Velocity_T& item_velocity
   )
   {
      const Vector_T velocity = { item_velocity };     
      const Vector_T view_vector = { view_point, item_position };

      return velocity.Calc_Signed_Magnitude_Projected_On(view_vector);
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Range_Rate_Difference_From_Expectations()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - range rate difference.
   *
   * PARAMETERS:
   *  const F360_VCS_Velocity_T &obj_velocity_vcs,
   *  const float32_t cos_det_az_vcs,
   *  const float32_t sin_det_az_vcs,
   *  const float32_t det_rr_comp
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
   * Calculate range rate difference between range rate compansated and range
   * rate expected from an object
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Range_Rate_Difference_From_Expectations(
      const F360_VCS_Velocity_T &obj_velocity_vcs,
      const float32_t cos_det_az_vcs,
      const float32_t sin_det_az_vcs,
      const float32_t det_rr_comp
   )
   {
      const float32_t obj_based_expected_rr_comp = Calculate_OTG_Range_Rate_From_Velocity(cos_det_az_vcs, sin_det_az_vcs, obj_velocity_vcs.longitudinal, obj_velocity_vcs.lateral);
      return std::abs(det_rr_comp - obj_based_expected_rr_comp);
   }
}

