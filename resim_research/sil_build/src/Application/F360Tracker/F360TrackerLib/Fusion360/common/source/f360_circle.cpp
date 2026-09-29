/*===================================================================================*\
* FILE: f360_cirlce.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of class Circle
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_circle.h"
#include "f360_math.h"
#include "f360_constants.h"
#include <limits>

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Circle
   *
   * Description    Custom constructor
   *
   * Parameters
   *  const Point(&position)
   *  const float32_t radius
   *
   * Returns        Circle.
   *
   * Externals:     None.
   *
   * Precondition   radius >= 0.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Circle::Circle(const Point(&position), const float32_t radius) :
      m_position(position), m_radius(radius)
   {
   }

   /*=========================================================================
   * Method         Y_Intersects_At_X
   *
   * Description    Calculate y from equtation (x-a)^2 + (y-b)^2 = r^2 
   *                for given x
   *
   * Parameters
   *  const float32_t x
   *  float32_t(&result)[2]
   *
   * Returns        uint32_t - number of found y values (0, 1 or 2).
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   uint32_t Circle::Y_Intersects_At_X(
      const float32_t x, 
      float32_t(&result)[2]
   ) const
   {
      return Intersects_For_Given_Input(x, m_position.x, m_position.y, result);
   }

   /*=========================================================================
   * Method         X_Intersects_At_Y
   *
   * Description    Calculate x from equtation (x-a)^2 + (y-b)^2 = r^2
   *                for given y
   *
   * Parameters
   *  const float32_t y
   *  float32_t(&result)[2]
   *
   * Returns        uint32_t - number of found y values (0, 1 or 2).
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   uint32_t Circle::X_Intersects_At_Y(
      const float32_t y, 
      float32_t(&result)[2]
   ) const
   {
      return Intersects_For_Given_Input(y, m_position.y, m_position.x, result);
   }

   /*=========================================================================
   * Method         Intersects_For_Given_Input
   *
   * Description    Calculate x or y (depends on input parameters) from 
   *                equation (x-a)^2 + (y-b)^2 = r^2. 
   *                
   *
   * Parameters
   *  const float32_t input, 
   *  const float32_t corresponding_param   - "a" or "b" from circle equation
   *  const float32_t other_param           - "a" or "b" from circle equation 
                                               (if corresponding_param is "a" 
                                               then it is "b" and vice versa
   *  float32_t (&result)[2]                - indirect output
   *
   * Returns        uint32_t - number of found values (0, 1 or 2).
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   inline uint32_t Circle::Intersects_For_Given_Input(
      const float32_t input, 
      const float32_t corresponding_param, 
      const float32_t other_param, 
      float32_t (&result)[2]
   ) const
   {
      const float32_t radius_squared = m_radius * m_radius;
      const float32_t diff = input - corresponding_param;
      const float32_t temp = radius_squared - diff * diff;

      uint32_t values_cnt;

      if (0.0F < temp)
      {
         values_cnt = 2U;

         const float32_t temp_sqrt = F360_Sqrtf(temp);
         result[0] = other_param + temp_sqrt;
         result[1] = other_param - temp_sqrt;
      }
      else if (std::abs(temp) < F360_EPSILON) //when temp_squared is very small then its sqrt value is smaller. We assume then that one value is found
      {
         values_cnt = 1U;
         result[0] = other_param;
      }
      else
      {
         values_cnt = 0U;
      }

      return values_cnt;
   }
}
