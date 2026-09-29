/*===================================================================================*\
* FILE: f360_point.h
*====================================================================================
* Copyright (C) 2021-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Point class.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#ifndef F360_POINT_H
#define F360_POINT_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_math.h"
#include "f360_angle.h"

namespace f360_variant_A
{
   class Point
   {
   public:
      // Constructors
      Point();
      Point(const float32_t point_x, const float32_t point_y);

      // Setters
      void Set_Position(const float32_t point_x, const float32_t point_y);

      void Translate(const float32_t dx, const float32_t dy);
      void Saturate_On_0();
      void Rotate_About_Origin(const Angle & rotation_angle);
      void Transform_To_Relative_Coordinate_System(const Point & cs_center, const Angle & cs_orientation);
      void Absolute();
      float32_t Distance_To_Origin_Squared() const;
      Point Calculate_Point_Fraction_Of_The_Way_To_Given_Point(const Point& p, const float32_t fraction) const;

      // Static function to get the slope between two points
      static float32_t get_slope_between_points(const Point& point1, const Point& point2);

      // Static function to get the distance between two points
      static float32_t get_distance_between_points(const Point& point1, const Point& point2);

      float32_t x;
      float32_t y;
   };

   inline bool operator==(const Point& point_lhs, const Point& point_rhs)
   {
      return (std::abs(point_lhs.x - point_rhs.x) < F360_EPSILON) &&
         (std::abs(point_lhs.y - point_rhs.y) < F360_EPSILON);
   }
}
#endif
