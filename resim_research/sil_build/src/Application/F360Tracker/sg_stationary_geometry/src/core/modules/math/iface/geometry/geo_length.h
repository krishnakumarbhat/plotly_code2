/*=============================================================================================*\
* FILE: geo_length.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations and definitions of geometry length functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef GEO_LENGTH_H
#define GEO_LENGTH_H

#include <cmath>

#include "geometry/geo_segment.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief         Function calulate length of the segment
       *
       * @param [in]    segment
       *
       * @return        segment length
       **/
      inline float length(const Segment2D_T &segment)
      {
         const Point2D_T diff = segment.first - segment.second;
         return std::hypot(diff.x, diff.y);
      }

      /**
       * @brief         Calculate length of the vector attached to the
       *                origin of the Cartesian coordinate system
       *
       * @param [in]    vector_end
       *
       * @return        vector length
       **/
      inline float length(const Point2D_T &vector_end)
      {
         return std::hypot(vector_end.x, vector_end.y);
      }

      /**
       * @brief         Calculate sqared length of the vector attached to
       *                the origin of the Cartesian coordinate system
       *
       * @param [in]    vector_end
       *
       * @return        squared length
       **/
      inline float length_sq(const Point2D_T &vector_end)
      {
         return vector_end.x * vector_end.x + vector_end.y * vector_end.y;
      }
   }
}
#endif
