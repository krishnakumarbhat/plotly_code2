/*===================================================================================*\
* FILE: f360_range_rates.h
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

#ifndef F360_RANGE_RATES_H
#define F360_RANGE_RATES_H

#include "f360_reuse.h"
#include "f360_point.h"
#include "f360_velocity.h"

namespace f360_variant_A
{
   float32_t Calculate_Det_Range_Rate_Comp(
      const float32_t cos_azimuth_vcs,
      const float32_t sin_azimuth_vcs,
      const float32_t det_range_rate_raw,
      const float32_t sensor_lat_vel_vcs,
      const float32_t sensor_long_vel_vcs
   );

   float32_t Calculate_OTG_Range_Rate_From_Velocity(
      const float32_t cos_azimuth_vcs,
      const float32_t sin_azimuth_vcs,
      const float32_t otg_long_vel_vcs,
      const float32_t otg_lat_vel_vcs
   );

   float32_t Calculate_Projected_Range_Rate(
      const Point& view_point,
      const Point& item_position,
      const F360_VCS_Velocity_T& item_velocity
   );

   float32_t Calc_Range_Rate_Difference_From_Expectations(
      const F360_VCS_Velocity_T &obj_velocity_vcs,
      const float32_t cos_det_az_vcs,
      const float32_t sin_det_az_vcs,
      const float32_t det_rr_comp
   );
}
#endif
