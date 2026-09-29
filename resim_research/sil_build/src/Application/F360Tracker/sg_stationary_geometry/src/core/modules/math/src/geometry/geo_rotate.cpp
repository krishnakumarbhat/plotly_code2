/*=============================================================================================*\
* FILE: geo_rotate.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions of geometry rotation functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "geometry/geo_rotate.h"

#include <limits>

#include "sg_math.h"

namespace sg
{
   namespace geometry
   {
      void rotate(Point2D_T &point, const float angle, const Point2D_T &pivot_point)
      {
         if (((std::fabs(point.x - pivot_point.x) >= std::numeric_limits<float>::epsilon()))
             || ((std::fabs(point.y - pivot_point.y) >= std::numeric_limits<float>::epsilon())))
         {
            const Point2D_T original_point = point;
            const float cos_angle          = cosf(angle);
            const float sin_angle          = sinf(angle);

            point.x           = original_point.x - pivot_point.x;
            point.y           = original_point.y - pivot_point.y;
            const float new_x = (point.x * cos_angle) - (point.y * sin_angle);
            const float new_y = (point.x * sin_angle) + (point.y * cos_angle);

            point.x = new_x + pivot_point.x;
            point.y = new_y + pivot_point.y;
         }
      }

      void rotate(Point2D_T &point, const Matrix<float, 2, 2> &rotation_matrix)
      {
         const Point2D_T original_point = point;
         point.x                        = (original_point.x * rotation_matrix[0][0]) + (original_point.y * rotation_matrix[0][1]);
         point.y                        = (original_point.x * rotation_matrix[1][0]) + (original_point.y * rotation_matrix[1][1]);
      }

      void rotate(Rectangle_T &rectangle, const float angle, const Point2D_T &pivot_point)
      {
         Point2D_T tmp_center     = rectangle.center();
         float tmp_rotation_angle = rectangle.rotation_angle();

         rotate(tmp_center, angle, pivot_point);

         tmp_rotation_angle = tmp_rotation_angle + angle;
         if (tmp_rotation_angle > TWO_PI)
         {
            tmp_rotation_angle = tmp_rotation_angle - TWO_PI;
         }

         if (tmp_rotation_angle < -TWO_PI)
         {
            tmp_rotation_angle = tmp_rotation_angle + TWO_PI;
         }

         rectangle = Rectangle_T(tmp_center, rectangle.width(), rectangle.length(), tmp_rotation_angle);
      }
   }
}