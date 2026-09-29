/*=============================================================================================*\
* FILE: geo_point.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of geometry point type.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef GEO_POINT_H
#define GEO_POINT_H

namespace sg
{
   namespace geometry
   {
      /**
       * @brief The stucture defines point 2D
       **/
      struct Point2D_T
      {
         /**
          * @brief default constructor, coordinates not initialized
          **/
         Point2D_T() = default;

         /**
          * @brief Parametric constructor
          * @param [in] x_in: longitudinal coordinate
          * @param [in] y_in: lateral coordinate
          **/
         Point2D_T(float x_in, float y_in) : x{x_in}, y{y_in} {};

         float x; // [-] longitudinal component
         float y; // [-] lateral component
      };

      /**
       * @brief The stucture defines point 3D
       **/
      struct Point3D_T : public Point2D_T
      {
         /**
          * @brief default constructor, coordinates not initialized
          **/
         Point3D_T() = default;

         /**
          * @brief Parametric constructor
          * @param [in] x_in: longitudinal coordinate
          * @param [in] y_in: lateral coordinate
          * @param [in] z_in: height coordinate
          **/
         Point3D_T(float x_in, float y_in, float z_in = 0.0F) : Point2D_T{x_in, y_in}, z{z_in} {};

         float z; // [-] height component
      };

      /**
       * @brief Substraction operator - calculates the differences of two 2D points: lhs - rhs
       * @param [in] lhs: left hand side point
       * @param [in] rhs: right hand side point
       *
       * @return Point2D_T
       **/
      inline Point2D_T operator-(const Point2D_T &lhs, const Point2D_T &rhs)
      {
         return {lhs.x - rhs.x, lhs.y - rhs.y};
      }

      /**
       * @brief Addition operator - calculates the sum of two 2D points: lhs + rhs
       * @param [in] lhs: left hand side point
       * @param [in] rhs: right hand side point
       *
       * @return Point2D_T
       **/
      inline Point2D_T operator+(const Point2D_T &lhs, const Point2D_T &rhs)
      {
         return {lhs.x + rhs.x, lhs.y + rhs.y};
      }

      /**
       * @brief Multiplication operator - calculates algebraic definition of a dot/inner product: lhs * rhs
       * @param [in] lhs: left hand side point
       * @param [in] rhs: right hand side point
       *
       * @return float
       **/
      inline float operator*(const Point2D_T &lhs, const Point2D_T &rhs)
      {
         return ((lhs.x * rhs.x) + (lhs.y * rhs.y));
      }

      /**
       * @brief Division operator - calculates scalar division (right side scalar): point / divider
       * @param [in] point: point to be divided
       * @param [in] divider: value dividing the point
       *
       * @return Point2D_T
       **/
      inline Point2D_T operator/(const Point2D_T &point, const float &divider)
      {
         return {point.x / divider, point.y / divider};
      }

      /**
       * @brief Multiplication operator - calculates scalar multiplication (left side scalar): lhs * rhs
       * @param [in] lhs: value multiplying the point
       * @param [in] rhs: point to be multipied
       *
       * @return Point2D_T
       **/
      inline Point2D_T operator*(const float &lhs, const Point2D_T &rhs)
      {
         return {lhs * rhs.x, lhs * rhs.y};
      }

      /**
       * @brief Multiplication opertor - calculates scalar multiplication (right side scalar): lhs * rhs
       * @param [in] lhs: point to be multipied
       * @param [in] rhs: value multiplying the point
       *
       * @return Point2D_T
       **/
      inline Point2D_T operator*(const Point2D_T &lhs, const float &rhs)
      {
         return {lhs.x * rhs, lhs.y * rhs};
      }
   }
}
#endif
