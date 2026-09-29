/*===================================================================================*\
* FILE:  f360_points_with_extreme_azimuth.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential  Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function signature of Determine_Points_With_Extreme_Azimuth()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#ifndef F360_POINTS_WITH_EXTREME_AZIMUTH_H
#define F360_POINTS_WITH_EXTREME_AZIMUTH_H

#include "f360_math_func.h"
#include "f360_point.h"
#include "f360_norm_heading_angle.h"

namespace f360_variant_A
{
   struct Extreme_Azimuth_Points
   {
      Point min_az_point;
      Point max_az_point;
   };

   /*===========================================================================*\
   * FUNCTION: Determine_Points_With_Extreme_Azimuth()
   *===========================================================================
   * RETURN VALUE:
   * Extreme_Azimuth_Points - point with extreme azimuth.
   *
   * PARAMETERS:
   *  const Point view_point
   *  const float32_t boresight
   *  const Point(&points)[N]
   *  const unsigned num_points
   *  Point(&result)[2] - indirect output
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
   * Looks for most azimuth spread points, looking from view_boresight
   *
   * PRECONDITIONS:
   * num_points > 0
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   template<uint32_t N>
   Extreme_Azimuth_Points Determine_Points_With_Extreme_Azimuth(
      const Point view_point,
      const float32_t view_boresight,
      const Point(&points)[N],
      const uint32_t num_points
   )
   {
      assert(num_points <= N);

      float32_t azimuth[N] = {};
      Point points_vp_cs[N] = {};
      for (uint32_t k = 0U; k < num_points; k++)
      {
         points_vp_cs[k].x = points[k].x - view_point.x;
         points_vp_cs[k].y = points[k].y - view_point.y;

         const float32_t aspect_angle = F360_Atan2f(points_vp_cs[k].y, points_vp_cs[k].x);
         azimuth[k] = Normalize_Heading_Angle(view_boresight - aspect_angle, 0.0F);
      }

      const uint32_t idx_min_az = F360_Min_Index(azimuth, num_points);
      const uint32_t idx_max_az = F360_Max_Index(azimuth, num_points);

      Extreme_Azimuth_Points extreme_points;
      extreme_points.min_az_point = points[idx_min_az];
      extreme_points.max_az_point = points[idx_max_az];

      return extreme_points;
   }
}
#endif
