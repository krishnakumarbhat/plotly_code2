/*===================================================================================*\
* FILE: f360_line.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains class definition of class Line
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_line.h"
#include "f360_vector.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Line
   *
   * Description    Custom constructor. Set line parameters to be equal to given ones, but first normalize it.
   *
   * Parameters     const float32_t a_factor
   *                const float32_t b_factor
   *                const float32_t c_factor
   *
   * Returns        Line.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Line::Line(const float32_t a_factor, const float32_t b_factor, const float32_t c_factor)
      : a(a_factor), b(b_factor), c(c_factor)
   {
      this->Normalize();
   }

   /*=========================================================================
   * Method         Line
   *
   * Description    Custom constructor. Create line which goes through two given points.
   *
   * Parameters     const Point & point_a - first point which line needs to go through
   *                const Point & point_b - second point which line needs to go through
   *
   * Returns        Line.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Line::Line(const Point& point_a, const Point& point_b)
   {
      this->a = point_b.y - point_a.y;
      this->b = point_a.x - point_b.x;
      this->c = (point_b.x * point_a.y) - (point_a.x * point_b.y);  // cross-product magnitude

      this->Normalize();
   }

   /*=========================================================================
   * Method         Find_Intersection
   *
   * Description    Find intersection between this and given line.
   *
   * Parameters     const Point & point_a - first point which line needs to go through
   *                const Point & point_b - second point which line needs to go through
   *
   * Returns        Line.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Lines_Intersection Line::Find_Intersection(const Line& other) const
   {
      Lines_Intersection intersect{ {}, false };
      const float32_t w = (this->a * other.b) - (this->b * other.a);
      if (F360_EPSILON < std::abs(w))
      {
         const float32_t w_inv = 1.F / w;
         intersect.exists = true;
         intersect.coordinates.x = ((this->b * other.c) - (this->c * other.b)) * w_inv;
         intersect.coordinates.y = ((this->c * other.a) - (this->a * other.c)) * w_inv;
      }
      return intersect;
   }

   /*=========================================================================
   * Method         Signed_Distance_To
   *
   * Description    Calculate signed distance from this line to given point. Sign of line determines on which side of line
   *                point lays. For all points which met condition a*p.x + b*p.y + c > 0 this function return positive value - this side of line would be
   *                considered as 'positive side'. All points which met condition a*p.x + b*p.y + c < 0, will be considered as laying on 'negative side'
   *                This can be also interpreted as left and right side of line - if line was constructed by Line(pointA, pointB) for all points laying on
   *                left side of vector (pointA, pointB) function will return positive values. If line would be constructed by Line(pointB, pointA) direction
   *                of constructing vector will be opposite, thus same set of point will be laying on right side - function will return negative values.
   *
   *
   * Parameters     const Point & p - point
   *
   * Returns        float32_t distance from line to point.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Line::Signed_Distance_To(const Point& p) const
   {
      // This equation returns distance since condition sqrt(a^2 + b^2) == 1 is met
      return this->a * p.x + this->b * p.y + this->c;
   }

   /*=========================================================================
   * Method         Normalize
   *
   * Description    Normalize a,b,c parameters to make sqrt(a^2 + b^2) == 1 identity true
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
   void Line::Normalize()
   {
      // Normalize to make sqrt(a^2 + b^2) == 1 identity true
      const float32_t ab_squares_sum = this->a * this->a + this->b * this->b;
      if (F360_EPSILON < ab_squares_sum)
      {
         const float32_t ab_sqrt_inv = 1.0F / F360_Sqrtf(ab_squares_sum);  // consider implementing fast inv of sqrt function
         this->a *= ab_sqrt_inv;
         this->b *= ab_sqrt_inv;
         this->c *= ab_sqrt_inv;
      }
   }


   /*===========================================================================*\
   * FUNCTION: Get_Line_Perpendicular_At_Point()
   *===========================================================================
   * RETURN VALUE:
   * Line perpendicular_line_at_point
   *
   * PARAMETERS:
   * const Point& p
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function returns line perpendicular to the one on which this method was called,
   * passing through point p.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   Line Line::Get_Line_Perpendicular_At_Point(const Point& p) const
   {
      // For a line with equation 0 = ax + by + c, the direction vector is (b, -a)
      // For a perpendicular line, the direction vector is (a, b)
      // So the perpendicular line has coefficients (a', b') = (lineB, -lineA)

      // The perpendicular line coefficients
      const Line perpendicular_line_at_point(this->b, -this->a, -(this->b * p.x - this->a * p.y));
      return perpendicular_line_at_point;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Lines_Equally_Distant_From_Original()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t distance
   * Line& ub_line
   * Line& lb_line
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes parameters of lines parallel to the one that the method was called on,
   * equally distant (distance) from original, in both directions.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Line::Get_Lines_Equally_Distant_From_Original(const float32_t distance, Line& ub_line, Line& lb_line) const
   {
      // The parallel lines have the same a and b coefficients, only c changes
      // The distance from a point (x,y) to the line ax + by + c = 0 is |ax + by + c| / sqrt(a^2 + b^2)
      // For parallel lines at distance d, we have c' = c +/- d*sqrt(a^2 + b^2)
      // since line is normalized prior to call of this function this applies: c' = c +/- d

      ub_line.a = this->a;
      ub_line.b = this->b;
      ub_line.c = this->c + distance;

      lb_line.a = this->a;
      lb_line.b = this->b;
      lb_line.c = this->c - distance;
   }

   /*===========================================================================*\
   * FUNCTION: isPointBetweenParallelLines()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const Point& p
   * const Line& line_1
   * const Line& line_2
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks whether given point p is between two parallel lines- line_1 and line_2.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Line::Is_Point_Between_Parallel_Lines(const Point& p, const Line& line_1, const Line& line_2) const
   {
      // Calculate the signed distance from the point to each line
      // Distance formula: d = (ax + by + c) / sqrt(a^2 + b^2)
      // The point is between the lines if the distances have opposite signs
      // or one of the distances is zero (point is on one of the lines)
      //
      // since sqrt(a^2 + b^2) is always positive, and we care only for the sign, we'll omit it afterwards
      // to reduce computations

      const float32_t d1 = (line_1.Get_a() * p.x + line_1.Get_b() * p.y + line_1.Get_c());
      float32_t d2 = (line_2.Get_a() * p.x + line_2.Get_b() * p.y + line_2.Get_c());

      // Ensure consistent sign convention for the normal vectors
      // If the normal vectors point in opposite directions, flip one "distance"
      if ((line_1.Get_a() * line_2.Get_a() + line_1.Get_b() * line_2.Get_b()) < 0.0F) {
         d2 = -d2;
      }
      return (d1 * d2 <= 0.0F);
   }
   /*===========================================================================*\
   * FUNCTION: Get_Distance_To_Point()
   *===========================================================================
   * RETURN VALUE:
   * float32_t
   *
   * PARAMETERS:
   * const Point& p
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function returns distance of the given point p to the line that this method was called on.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
}
