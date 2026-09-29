/*=============================================================================================*\
* FILE: geo_is_inside.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry functions to check if something is inside.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef GEO_IS_INSIDE_H
#define GEO_IS_INSIDE_H
#include "geometry/geo_circle.h"
#include "geometry/geo_intervals.h"
#include "geometry/geo_point.h"
#include "geometry/geo_rectangle.h"

namespace sg
{
   namespace geometry
   {
      /*=============================================================================================*\
       * Function      bool is_inside(const Circle_T& circle, const Point2D_T& point);
       *
       * Description   Function checks if point is inside a circle.
       *
       * Parameters    Point2D_T& point - 2 dimensional point
       *               Circle_T& circle - circle
       *
       * Returns       bool
       *=============================================================================================*/
      inline bool is_inside(const Circle_T &circle, const Point2D_T &point)
      {
         assert(circle.radius() > 0.0F);

         const auto diff = point - circle.center;

         return (diff.x * diff.x) + (diff.y * diff.y) <= circle.radius_squared();
      }

      /*=============================================================================================*\
       * Function      is_inside(const Rectangle_T& rectangle, const Point2D_T& point)
       *
       * Description   Function checks if point is inside a rectangle.
       *
       * Parameters    const Rectangle_T& rectangle - rectangle
       *               const Point2D_T& point - 2 dimensional point
       *
       * Returns       bool
       *=============================================================================================*/
      bool is_inside(const Rectangle_T &rectangle, const Point2D_T &point);

      /*=============================================================================================*\
       * Function      is_inside(const Intervals_T& intervals, const Point2D_T& point)
       *
       * Description   Function checks if point is inside coordinate system intervals.
       *
       * Parameters    const Intervals_T& intervals - long/lat intervals
       *               const Point2D_T& point - 2 dimensional point
       *
       * Returns       bool
       *=============================================================================================*/
      inline bool is_inside(const Intervals_T &intervals, const Point2D_T &point)
      {
         return ((intervals.min_x() < point.x) && (point.x < intervals.max_x()) && (intervals.min_y() < point.y)
                 && (point.y < intervals.max_y()));
      }
   }
}
#endif
