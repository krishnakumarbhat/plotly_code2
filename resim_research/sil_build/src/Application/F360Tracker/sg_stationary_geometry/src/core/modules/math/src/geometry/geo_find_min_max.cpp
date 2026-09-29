/*=============================================================================================*\
* FILE: geo_find_min_max.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of geometry find_min functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include <algorithm>

#include "geometry/geo_rectangle.h"

namespace sg
{
   namespace geometry
   {
      // Returns min value of vertices x coordinates
      float find_min_x(const Rectangle_T &rectangle)
      {
         return std::min({rectangle.vertex_a().x, rectangle.vertex_b().x, rectangle.vertex_c().x, rectangle.vertex_d().x});
      }

      // Returns max value of vertices x coordinates
      float find_max_x(const Rectangle_T &rectangle)
      {
         return std::max({rectangle.vertex_a().x, rectangle.vertex_b().x, rectangle.vertex_c().x, rectangle.vertex_d().x});
      }

      // Returns min and max values of vertices x coordinates
      std::pair<float, float> find_min_max_x(const Rectangle_T &rectangle)
      {
         std::pair<float, float> result{};
         result.first  = find_min_x(rectangle);
         result.second = find_max_x(rectangle);
         return result;
      }
   }
}