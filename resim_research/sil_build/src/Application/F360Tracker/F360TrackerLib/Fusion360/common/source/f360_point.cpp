/*===================================================================================*\
* FILE: f360_point.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definitions of Point class methods.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/

#include "f360_point.h"
#include "f360_math.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Point::Point()
   *
   * Description    Default constructor.
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Point::Point(): x(0.0F), y(0.0F) {}

   /*=========================================================================
   * Method         Point::Point()
   *
   * Description    Create Point by given x,y position values.
   *
   * Parameters
   * const float32_t point_x - position longitudinal
   * const float32_t point_y - position lateral
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Point::Point(const float32_t point_x, const float32_t point_y) : x(point_x), y(point_y) {}

   /*=========================================================================
   * Method         Point::Set_Position
   *
   * Description    Set new position of point by given x,y.
   *
   * Parameters
   * const float32_t point_x - position longitudinal
   * const float32_t point_y - position lateral
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Point::Set_Position(const float32_t point_x, const float32_t point_y)
   {
      x = point_x;
      y = point_y;
   }

   /*=========================================================================
   * Method         Point::Translate()
   *
   * Description    Translate Point by given vector.
   *
   * Parameters
   * const float32_t dx - longitudinal step
   * const float32_t dy - lateral step
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Point::Translate(const float32_t dx, const float32_t dy)
   {
      x += dx;
      y += dy;
   }

   /*=========================================================================
   * Method         Point::Saturate_On_0()
   *
   * Description    Prevent from negative point's coordinates. If current coordinates is negative - set it to 0.0.
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Point::Saturate_On_0()
   {
      if (x < 0.0F)
      {
         x = 0.0F;
      }
      if (y < 0.0F)
      {
         y = 0.0F;
      }
   }

   /*=========================================================================
   * Method         Point::Rotate_About_Origin()
   *
   * Description    Rotate point by given rotation angle.
   *
   * Parameters
   * const Angle & rotation_angle
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Point::Rotate_About_Origin(const Angle & rotation_angle)
   {
      F360_Rotate_2D_Vector(x, y, rotation_angle.Cos(), rotation_angle.Sin(), x, y);
   }

   /*=========================================================================
   * Method         Point::Absolute()
   *
   * Description    Set new coordinates which are absolute values of current coordinates
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Point::Absolute()
   {
      if (x < 0.0F)
      {
         x = -x;
      }
      if (y < 0.0F)
      {
         y = -y;
      }
   }

   /*=========================================================================
   * Method         Point::Distance_To_Origin_Squared()
   *
   * Description    Get squared distance to point (0, 0)
   *
   * Parameters     None.
   *
   * Returns
   * float32_t - distance to (0, 0) squared
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Point::Distance_To_Origin_Squared() const
   {
      return (x*x + y*y);
   }

   /*=========================================================================
   * Method         Point::get_slope_between_points()
   *
   * Description    Calculates the slope of line joining 2 points
   *
   * Parameters     None.
   *
   * Returns
   * float32_t - slope of line joining 2 points
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/


   float32_t Point::get_slope_between_points(const Point& point1, const Point& point2)
   {
      float32_t slope;

      /* Check if the points are the same to avoid division by zero */
      if (std::fabs(point1.x - point2.x)<= F360_EPSILON)
      {
         /* Handle the case where the points have the same x - coordinate */
         slope = INFTY;
      }
      else
      {
         /* Calculate and return the slope */
         slope = (point2.y - point1.y) / (point2.x - point1.x);
      }

      return slope;
   }

   float32_t Point::get_distance_between_points(const Point& point1, const Point& point2)
   {
      const float32_t dx = point2.x - point1.x;
      const float32_t dy = point2.y - point1.y;
      return F360_Sqrtf(dx * dx + dy * dy);
   }


   /*=========================================================================
   * Method         Point::Transform_To_Relative_Coordinate_System()
   *
   * Description    It transforms point to new coordiante system defined by
   *                relative translation vector and rotation angle
   *
   * Parameters     const Point & cs_center - translation vector from old to
   *                                          new coordinate system
   *                const Angle & cs_orientation - relative rotation angle
   *                                               from old to new coordinate
   *                                               system
   *
   * Returns        void
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   void Point::Transform_To_Relative_Coordinate_System(
      const Point & cs_center,
      const Angle & cs_orientation
   )
   {
      this->Translate(-cs_center.x, -cs_center.y);
      this->Rotate_About_Origin(-cs_orientation);
   }

   Point Point::Calculate_Point_Fraction_Of_The_Way_To_Given_Point(const Point& p, const float32_t fraction) const
   {
      const Point result_point(this->x + ((p.x - this->x) * fraction), this->y + ((p.y - this->y) * fraction));
      return result_point;
   }


}
