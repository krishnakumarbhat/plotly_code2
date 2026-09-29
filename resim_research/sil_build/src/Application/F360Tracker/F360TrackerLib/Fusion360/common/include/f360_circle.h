/*===================================================================================*\
* FILE: f360_cirlce.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains class declaration of class Circle
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_CIRCLE_H
#define F360_CIRCLE_H

#include "f360_reuse.h"
#include "f360_point.h"

namespace f360_variant_A
{
   class Circle
   {
   public:
      Circle(const Point(&position), const float32_t radius);

      uint32_t Y_Intersects_At_X(const float32_t x, float32_t(&result)[2]) const;
      uint32_t X_Intersects_At_Y(const float32_t y, float32_t(&result)[2]) const;
   private:
      Point m_position;
      float32_t m_radius;

      inline uint32_t Intersects_For_Given_Input(
         const float32_t input,
         const float32_t corresponding_param,
         const float32_t other_param,
         float32_t(&result)[2]) const;
   };
}

#endif
