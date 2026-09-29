/*=============================================================================================*\
* FILE: geo_footpoint.h
* ====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry footpoint type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_FOOTPOINT_H
#define GEO_FOOTPOINT_H

#include "geo_point.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief The structure defines a footpoint in 2D with position and s parameter
       **/
      struct Footpoint_T
      {
         /**
          * @brief default constructor, coordinates and s parameter not initialized
          **/
         Footpoint_T() = default;

         /**
          * @brief Parametric constructor
          * @param [in] point_in: 2D point coordinates
          * @param [in] s_in: s parameter representing proportion along segment
          **/
         Footpoint_T(const Point2D_T &point_in, const float s_in) : point(point_in), s(s_in)
         {
         }

         /**
          * @brief Parametric constructor
          * @param [in] x_in: longitudinal coordinate
          * @param [in] y_in: lateral coordinate
          * @param [in] s_in: s parameter representing proportion along segment
          **/
         Footpoint_T(const float x_in, const float y_in, const float s_in) : point(x_in, y_in), s(s_in)
         {
         }

         /**
          * @brief The 2D point representing the footpoint location
          **/
         Point2D_T point;

         /**
          * @brief The s parameter which represents the proportion along the segment
          **/
         float s;
      };
   }
}
#endif
