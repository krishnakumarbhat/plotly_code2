/*===================================================================================*\
* FILE: f360_vector.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains class declaration of class Vector
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_VECTOR_H
#define F360_VECTOR_H


#include "f360_reuse.h"
#include "f360_point.h"
#include "f360_velocity.h"

namespace f360_variant_A
{
   class Vector_T
   {
   public:
      // Constructors
      Vector_T();    // Default constructor
      Vector_T(const Point &point_A, const Point &point_B);    // Create vector from point A to B
      Vector_T(const Point &p) : m_vector(p) {};    // Create vector (p.x, p.y)
      Vector_T(const float32_t x, const float32_t y);    // Create vector (x, y)
      Vector_T(const F360_VCS_Velocity_T &velocity);    // Create vector (velocity.longitudinal, velocity.lateral)

      // Getters 
      float32_t Get_X() const { return m_vector.x; };    //Return longitudinal part
      float32_t Get_Y() const { return m_vector.y; };    //Return lateral part
      float32_t Magnitude() const;    // Return length of vector

      float32_t Cos_Angle_Between(const Vector_T &other_vector) const;    // Calculate value of angle's cosinus between vectors
      float32_t Calc_Signed_Magnitude_Projected_On(const Vector_T &vector_to_project_on) const;    // Calculate length of vector projection on other vector
      float32_t operator*(const Vector_T &other_vector) const;    // Calculate cross-product between vectors
      float32_t Cross_Product_Magnitude(const Vector_T &other_vector) const;    // Calculate magnitude of cross-product between vectors

   private:
      Point m_vector;    // Representation of longitudinal and lateral part of vector
   };
}

#endif
