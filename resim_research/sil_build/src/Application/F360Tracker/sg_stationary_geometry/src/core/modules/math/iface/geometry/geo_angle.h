/*=============================================================================================*\
* FILE: geo_angle.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations and definitions of geometry angle type.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef GEO_ANGLE_H
#define GEO_ANGLE_H

#include <array>
#include <cmath>

#include "geo_length.h"
#include "geo_point.h"
#include "sg_math.h"

namespace sg
{
   namespace geometry
   {
      /**
       * @brief The class defines Angle 2D. The angle is represented by two
       * points which are the ends of vectors attached to the origin of the Cartesian
       * coordinate system
       **/
      class Angle2D_T
      {
        private:
         Point2D_T vec1;
         Point2D_T vec2;

        public:
         /**
          * @brief Parametric constructor with two vectors
          *
          * @param [in] v1:       first vector
          * @param [in] v2:       second vector
          **/
         Angle2D_T(const Point2D_T p1, const Point2D_T p2) : vec1{p1}, vec2{p2}
         {
         }

         /**
          * @brief Parametric constructor with two vectors
          *
          * @param [in] array_vec_1:   first vector
          * @param [in] array_vec_2:   second vector
          **/
         Angle2D_T(std::array<float, 2> const &array_vec_1, std::array<float, 2> const &array_vec_2)
             : vec1{array_vec_1[0], array_vec_1[1]}, vec2{array_vec_2[0], array_vec_2[1]}
         {
         }

         //

         /**
          * @brief    Function returns angle value in deg
          *
          * @param   None
          *
          * @return  deg angle value
          **/
         inline float deg() const
         {
            return RAD2DEG(rad());
         }

         /**
          * @brief    Function returns angle value in rad
          *
          * @param   None
          *
          * @return  rad angle value
          **/
         inline float rad() const
         {
            const float dot_product = (vec1.x * vec2.x) + (vec1.y * vec2.y);
            float arg               = (dot_product / (geometry::length(vec1) * geometry::length(vec2)));
            if (arg > 1.0F)
            {
               arg = 1.0F;
            }
            if (arg < -1.0F)
            {
               arg = -1.0F;
            }
            return std::acos(arg);
         }

         /**
          * @brief    Function returns signed angle value in deg
          *
          * @param   None
          *
          * @return  deg signed angle value
          **/
         inline float signed_deg() const
         {
            return RAD2DEG(signed_rad());
         }

         /**
          * @brief    Function returns signed angle value in rad
          *
          * @param   None
          *
          * @return  rad signed angle value
          **/
         inline float signed_rad() const
         {
            const float y = (vec1.x * vec2.y) - (vec2.x * vec1.y);
            const float x = (vec1.x * vec2.x) + (vec1.y * vec2.y);
            return std::atan2(y, x);
         }
      };
   }
}
#endif
