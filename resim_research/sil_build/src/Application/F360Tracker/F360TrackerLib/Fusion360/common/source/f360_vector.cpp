/*===================================================================================*\
* FILE: f360_vector.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of class Vector_T
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_vector.h"
#include <limits>
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Vector_T
   *
   * Description    Default constructor.
   *
   * Parameters     None.
   *
   * Returns        Vector_T.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Vector_T::Vector_T() : m_vector()
   {
   }

   /*=========================================================================
   * Method         Vector_T
   *
   * Description    Custom constructor.
   *
   * Parameters     Point &point_A
   *                Point &point_B
   *
   * Returns        Vector_T.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Vector_T::Vector_T(const Point &point_A, const Point &point_B)
   {
      m_vector = { point_B.x - point_A.x, point_B.y - point_A.y };
   }

   /*=========================================================================
   * Method         Vector_T
   *
   * Description    Custom constructor.
   *
   * Parameters     const float32_t x - longitudinal part
   *                const float32_t y - lateral part
   *
   * Returns        Vector_T.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Vector_T::Vector_T(const float32_t x, const float32_t y)
   {
      m_vector.Set_Position(x, y);
   }

   /*=========================================================================
   * Method         Vector_T
   *
   * Description    Custom constructor.
   *
   * Parameters     F360_VCS_Velocity_T &velocity
   *
   * Returns        Vector_T.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Vector_T::Vector_T(const F360_VCS_Velocity_T &velocity)
   {
      m_vector.Set_Position(velocity.longitudinal, velocity.lateral);
   }

   /*=========================================================================
   * Method         Cos_Angle_Between
   *
   * Description    Calculate cos(angle) between two vectors.
   *
   * Parameters     const Vector_T & other_vector
   *
   * Returns        float32_t - [rad/NaN] cos(angle) -> angle between vectors.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Vector_T::Cos_Angle_Between(const Vector_T & other_vector) const
   {
      const float32_t magnitude_product = this->Magnitude() * other_vector.Magnitude();

      float32_t cos_val;
      if (magnitude_product > F360_EPSILON)
      {
         const float32_t dot_product = *this * other_vector;
         cos_val = dot_product / magnitude_product;
      }
      else
      {
         cos_val = std::numeric_limits<float32_t>::quiet_NaN();
      }

      return cos_val;
   }

   /*=========================================================================
   * Method         Calc_Signed_Magnitude_Projected_On
   *
   * Description    Calculate signed magnitude of vector projected on vector_to_project_on.
   *                Magnitude is positive if angle between vectors is in <-90, 90> degree.
   *                In other words its behavior is similar to range rate. Negative value means 
   *                it aproaches to us and positive it goes away. 
   *
   * Parameters     const Vector_T & vector_to_project_on
   *
   * Returns        float32_t - signed magnitude (NaN in case of vector_to_project_on is zero vector).
   *
   * Externals:     None.
   *
   * Precondition   vector_to_project_on is not zero vector.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Vector_T::Calc_Signed_Magnitude_Projected_On(const Vector_T & vector_to_project_on) const
   {
      const float32_t vector_magnitude = this->Magnitude();

      float32_t signed_projected_magnitude;
      if (vector_magnitude > F360_EPSILON)
      {
         const float32_t cos_angle = this->Cos_Angle_Between(vector_to_project_on);
         signed_projected_magnitude = cos_angle * vector_magnitude;
      }
      else
      {
         signed_projected_magnitude = 0.0F;
      }

      return signed_projected_magnitude;
   }

   /*=========================================================================
   * Method         Magnitude
   *
   * Description    Calculate vector magnitude.
   *
   * Parameters     None.
   *
   * Returns        float32_t - magnitude.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Vector_T::Magnitude() const
   {
      return F360_Get_Hypotenuse(m_vector.x, m_vector.y);
   }

   /*=========================================================================
   * Method         operator*
   *
   * Description    Calculate multiplication scalar product of two vectors.
   *
   * Parameters     const Vector_T & other_vector.
   *
   * Returns        float32_t - multiplication scalar product.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Vector_T::operator*(const Vector_T & other_vector) const
   {
      return m_vector.x * other_vector.Get_X() + m_vector.y * other_vector.Get_Y();
   }

   /*=========================================================================
   * Method         Cross_Product_Magnitude
   *
   * Description    Calculate magnitude of cross product between vectors result.
   *
   * Parameters     const Vector_T & other_vector.
   *
   * Returns        float32_t - magnitude of cross product between vectors result.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   float32_t Vector_T::Cross_Product_Magnitude(const Vector_T & other_vector) const
   {
      return ((this->m_vector.x * other_vector.m_vector.y) - (this->m_vector.y * other_vector.m_vector.x));
   }
}
