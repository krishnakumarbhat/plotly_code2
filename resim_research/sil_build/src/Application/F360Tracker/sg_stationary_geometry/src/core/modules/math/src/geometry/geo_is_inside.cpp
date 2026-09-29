/*=============================================================================================*\
* FILE: geo_is_inside.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of geometry is_inside functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "geometry/geo_is_inside.h"

#include <sg_math.h>

#include <algorithm>
#include <cassert>

#include "geometry/geo_rotate.h"

namespace sg
{
   namespace geometry
   {
      bool is_inside(const Rectangle_T &rectangle, const Point2D_T &point)
      {
         Point2D_T tmp_point{point};
         Point2D_T tmp_point_center{rectangle.center()};
         const float width_half  = rectangle.width() / 2.0F;
         const float length_half = rectangle.length() / 2.0F;

         sg::Matrix<float, 2, 2> rotation_matrix{};

         get_rotation_matrix2d(rotation_matrix, -rectangle.rotation_angle());

         rotate(tmp_point, rotation_matrix);
         rotate(tmp_point_center, rotation_matrix);

         const float max_x = tmp_point_center.x + length_half;
         const float min_x = tmp_point_center.x - length_half;
         const float max_y = tmp_point_center.y + width_half;
         const float min_y = tmp_point_center.y - width_half;
         (void) max_x;
         (void) max_y;
         (void) min_x;
         (void) min_y;

         return ((min_x < tmp_point.x) && (tmp_point.x < max_x) && (min_y < tmp_point.y) && (tmp_point.y < max_y));
      }
   }
}