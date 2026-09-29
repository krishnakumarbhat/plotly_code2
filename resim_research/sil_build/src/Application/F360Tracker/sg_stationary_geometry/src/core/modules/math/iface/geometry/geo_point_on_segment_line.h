/*=============================================================================================*\
* FILE: geo_point_on_segment_line.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration and  of definition of calculate_point_on_segment_line().
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_POINT_ON_SEGMENT_LINE_H
#define GEO_POINT_ON_SEGMENT_LINE_H
#include "geo_point.h"
#include "geo_segment.h"

namespace sg
{
   namespace geometry
   {

      /**
       * @brief         Calculates point on segment line
       *
       * @param [in]    segment
       * @param [in]    spline_coordinate
       **/
      inline Point2D_T calculate_point_on_segment_line(const Segment2D_T &segment, const float spline_coordinate)
      {
         return Point2D_T{((1.0F - spline_coordinate) * segment.first.x) + (spline_coordinate * segment.second.x),
                          ((1.0F - spline_coordinate) * segment.first.y) + (spline_coordinate * segment.second.y)};
      }
   }
}
#endif