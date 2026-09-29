/*=============================================================================================*\
* FILE: geo_circle.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry circle type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_CIRCLE_H
#define GEO_CIRCLE_H

#include "geo_point.h"

namespace sg
{
   namespace geometry
   {
      class Circle_T
      {
        public:
         Circle_T() = default;

         Circle_T(const float x_center_in, const float y_center_in, const float radius_in)
             : center{x_center_in, y_center_in}, m_radius{radius_in}, m_radius_squared{radius_in * radius_in} {};

         Circle_T(const Point2D_T &center_in, const float radius_in) : Circle_T{center_in.x, center_in.y, radius_in}
         {
         }

         float radius() const
         {
            return m_radius;
         }

         void radius(float radius_)
         {
            m_radius         = radius_;
            m_radius_squared = radius_ * radius_;
         }

         float radius_squared() const
         {
            return m_radius_squared;
         }

         Point2D_T center{}; // [-] center point

        private:
         float m_radius{};         // [-] radius component
         float m_radius_squared{}; // [-] radius squared component
      };
   }
}
#endif
