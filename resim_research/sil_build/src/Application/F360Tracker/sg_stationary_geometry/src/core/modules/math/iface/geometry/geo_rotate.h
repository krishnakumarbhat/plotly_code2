/*=============================================================================================*\
* FILE: geo_rotate.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry rotation functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef GEO_ROTATE_H
#define GEO_ROTATE_H

#include "geometry/geo_point.h"
#include "geometry/geo_rectangle.h"
#include "sg_matrix_operations.h"

namespace sg
{
   namespace geometry
   {
      /*=============================================================================================*\
      * Function      rotate(Point2D_T& point, const float angle)
      *
      * Description   Rotate 2D point around "z" axis from "x" to "y" direction (positive angle).
                      Center of the coordinate system is rotation origin.
      *
      * Parameters    Point2D_T& point - 2 dimensional point (reference)
      *               const float angle - [rad] rotation angle
      *
      * Returns       None
      *=============================================================================================*/
      inline void rotate(Point2D_T &point, const float angle)
      {
         const Point2D_T original_point = point;
         const float cos_angle          = cosf(angle);
         const float sin_angle          = sinf(angle);

         point.x = (original_point.x * cos_angle) - (original_point.y * sin_angle);
         point.y = (original_point.x * sin_angle) + (original_point.y * cos_angle);
      }

      /*=============================================================================================*\
      * Function      rotate(Point2D_T& point, const Matrix<float, 2, 2>& rotation_matrix)
      *
      * Description   Rotate 2D point around "z" axis.
                      Center of the coordinate system is rotation origin.
      *
      * Parameters    Point2D_T& point - 2 dimensional point (reference)
      *               const Matrix<float, 2, 2>& rotation_matrix - rotation matrix
      *
      * Returns       None
      *=============================================================================================*/
      void rotate(Point2D_T &point, const Matrix<float, 2, 2> &rotation_matrix);

      /*=============================================================================================*\
       * Function      rotate(Point2D_T& point, const float angle, const Point2D_T& pivot_point)
       *
       * Description   Rotate 2D point around pivot point.
       *
       * Parameters    Point2D_T& point - 2 dimensional point (reference)
       *               const float angle - [rad] rotation angle
       *               const Point2D_T& pivot_point - rotation pivot point
       *
       * Returns       None
       *=============================================================================================*/
      void rotate(Point2D_T &point, const float angle, const Point2D_T &pivot_point);

      /*=============================================================================================*\
       * Function      rotate(Rectangle_T& rectangle, const float angle, const Point2D_T& pivot_point)
       *
       * Description   Rotate rectangle around pivot point.
       *
       * Parameters    Rectangle_T& rectangle - reference to rectangle
       *               const float angle - [rad] rotation angle
       *               const Point2D_T& pivot_point - rotation pivot point (default {0.0F,0.0F})
       *
       * Returns       None
       *=============================================================================================*/
      void rotate(Rectangle_T &rectangle, const float angle, const Point2D_T &pivot_point = Point2D_T(0.0F, 0.0F));
   }
}
#endif
