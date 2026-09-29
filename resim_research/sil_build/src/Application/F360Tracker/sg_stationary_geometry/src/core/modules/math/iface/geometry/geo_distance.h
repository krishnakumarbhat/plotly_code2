/*=============================================================================================*\
* FILE: geo_distance.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations and definitions of geometry functions that calculate distance.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef GEO_DISTANCE_H
#define GEO_DISTANCE_H

#include <cmath>

#include "geometry/geo_point.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief   Function calculates euclidean distance between two 2D points.
       *
       * @param   const Point2D_T& p1 - 2 dimensional point
       * @param   const Point2D_T& p2 - 2 dimensional point
       *
       * @return  euclidian distance
       **/
      inline float euclidean_distance(const Point2D_T &p1, const Point2D_T &p2)
      {
         return sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
      }

      /**
       * @brief   Function calculates squared euclidean distance between two 2D points.
       *
       * @param   const Point2D_T& p1 - 2 dimensional point
       * @param   const Point2D_T& p2 - 2 dimensional point
       *
       * @return  squared euclidian distance
       **/
      inline float squared_euclidean_distance(const Point2D_T &p1, const Point2D_T &p2)
      {
         const auto diff = p1 - p2;
         return diff.x * diff.x + diff.y * diff.y;
      }
   }
}
#endif
