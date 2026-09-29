/*=============================================================================================*\
* FILE: geo_rectangle.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry rectangle type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef GEO_RECTANGLE_H
#define GEO_RECTANGLE_H

#include <algorithm>
#include <cassert>
#include <cmath>

#include "geo_point.h"

namespace sg
{
   namespace geometry
   {
      class Rectangle_T
      {
        private:
         Point2D_T m_center;     // point located in center of rectangle
         float m_width;          // [m] rectangle width
         float m_length;         // [m] rectangle length
         float m_rotation_angle; // [rad] rectangle rotation angle

         // rectangle vertices
         Point2D_T m_vertex_a;
         Point2D_T m_vertex_b;
         Point2D_T m_vertex_c;
         Point2D_T m_vertex_d;

        public:
         Rectangle_T() = default;

         Rectangle_T(const Point2D_T center, const float width, const float length, const float rotation_angle)
             : m_center{center}, m_width(width), m_length{length}, m_rotation_angle{rotation_angle}
         {
            assert((width > 0.0F) && (length > 0.0F));
            update_vertices();
         };

         Point2D_T center() const
         {
            return m_center;
         }

         float width() const
         {
            return m_width;
         }

         float length() const
         {
            return m_length;
         }

         float rotation_angle() const
         {
            return m_rotation_angle;
         }

         Point2D_T vertex_a() const
         {
            return m_vertex_a;
         }

         Point2D_T vertex_b() const
         {
            return m_vertex_b;
         }

         Point2D_T vertex_c() const
         {
            return m_vertex_c;
         }

         Point2D_T vertex_d() const
         {
            return m_vertex_d;
         }

         // set center of rectangle and update coordinates of vertices
         void center(const Point2D_T center)
         {
            const Point2D_T translation = center - m_center;
            m_center                    = center;

            m_vertex_a = m_vertex_a + translation;
            m_vertex_b = m_vertex_b + translation;
            m_vertex_c = m_vertex_c + translation;
            m_vertex_d = m_vertex_d + translation;
         }

         // set width of rectangle and update coordinates of vertices
         void width(const float width)
         {
            assert(width > 0.0F);
            m_width = width;
            update_vertices();
         }

         // set length of rectangle and update coordinates of vertices
         void length(const float length)
         {
            assert(length > 0.0F);
            m_length = length;
            update_vertices();
         }

         void update_vertices()
         {
            const float half_length = m_length / 2.0F;
            const float half_width  = m_width / 2.0F;

            Point2D_T tmp_vertex_a{-half_length, half_width};
            Point2D_T tmp_vertex_b{-half_length, -half_width};
            Point2D_T tmp_vertex_c{half_length, -half_width};
            Point2D_T tmp_vertex_d{half_length, half_width};

            const float m_cos = cosf(m_rotation_angle);
            const float m_sin = sinf(m_rotation_angle);

            // rotation
            tmp_vertex_a = {tmp_vertex_a.x * m_cos - tmp_vertex_a.y * m_sin, tmp_vertex_a.x * m_sin + tmp_vertex_a.y * m_cos};
            tmp_vertex_b = {tmp_vertex_b.x * m_cos - tmp_vertex_b.y * m_sin, tmp_vertex_b.x * m_sin + tmp_vertex_b.y * m_cos};
            tmp_vertex_c = {tmp_vertex_c.x * m_cos - tmp_vertex_c.y * m_sin, tmp_vertex_c.x * m_sin + tmp_vertex_c.y * m_cos};
            tmp_vertex_d = {tmp_vertex_d.x * m_cos - tmp_vertex_d.y * m_sin, tmp_vertex_d.x * m_sin + tmp_vertex_d.y * m_cos};

            // coordinates translation
            m_vertex_a = tmp_vertex_a + m_center;
            m_vertex_b = tmp_vertex_b + m_center;
            m_vertex_c = tmp_vertex_c + m_center;
            m_vertex_d = tmp_vertex_d + m_center;
         }
      };

   }
}
#endif
