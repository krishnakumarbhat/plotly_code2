/*=============================================================================================*\
* FILE: geo_projection.h
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry projection functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_PROJECTION_H
#define GEO_PROJECTION_H

#include "geometry/geo_footpoint.h"
#include "geometry/geo_point.h"
#include "geometry/geo_segment.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief    Calculate the perpendicular foot on a segment in two-dimensional space.
       *
       * @param    segment - segment on which footpoint will be calculated
       * @param    point - point for which footpoint will be calculated
       * @param    f_limit_footpoint - if flag is true, footpoint will equal the nearest segment vertex when the calculated
       *footpoint lies beyond the segment
       *
       * @return   Footpoint_T with coordinates of the perpendicular foot point
       *           on the segment from the given point and s parameter which is proportion the length of
       *           footpoint to segment.
       **/
      Footpoint_T make_projection(const geometry::Segment2D_T &segment, const geometry::Point2D_T &point, const bool f_limit_footpoint);
   }
}
#endif
