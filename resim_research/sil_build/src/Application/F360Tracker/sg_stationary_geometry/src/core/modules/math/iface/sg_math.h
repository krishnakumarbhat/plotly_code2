/*=============================================================================================*\
* FILE: sg_math.h
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of functions that do mathematical calculations widely used in
* Stationary Geometries functionality. Also contains some mathematical constant values.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_MATH_H
#define SG_MATH_H

#include <math.h>

#include <cmath>
#include <cstddef>

#include "geometry/geo_point.h"
#include "sg_matrix_operations.h"

namespace sg
{
   static constexpr std::size_t COV_2D_IDX_X = 0U;
   static constexpr std::size_t COV_2D_IDX_Y = 1U;

   // Constant values for mathemetical conversion
   static constexpr float PI         = 3.14159265358979323846F;  // pi
   static constexpr float TWO_PI     = 6.28318530717958647693F;  // 2*pi
   static constexpr float PI_2       = 1.57079632679489661923F;  // pi/2
   static constexpr float INVERSE_PI = 0.318309886183790671538F; // 1/pi
   static constexpr float DEG2RAD(const float x)
   {
      return (x * 0.0174532925F);
   } // converts from degree to radian
   static constexpr float RAD2DEG(const float x)
   {
      return ((180.0F / PI) * x);
   } // converts from radian to degree

   /*=============================================================================================*\
    * Function      normalize_angle( const float angle_in, const float interval_center)
    *
    * Description   Adaptive normalization of heading angle. 'Adaptive' means that the
    *               center of the normalized interval can be adjusted. The reason for doing
    *               this is when calculating (and subsequent averaging of) sigma - points,
    *               don't want any of them to go outside of the normalized angle interval.
    *
    * Parameters    const float angle_in,
    *               const float interval_center
    *
    * Returns       float
    *=============================================================================================*/
   inline float normalize_angle(const float angle_in, const float interval_center)
   {
      const float temp_val = (angle_in + (PI - interval_center)) * (0.5F * INVERSE_PI);
      const float mod_val  = (temp_val - floorf(temp_val)) * (TWO_PI);
      const float norm     = mod_val - (PI - interval_center);
      return norm;
   }

   /*=============================================================================================*\
    * Function      uncertainty_propagation_2d(const float(&jacobian)[2][2], const float(&input_variance)[2][2],
    *float(&output_variance)[2][2])
    *
    * Description   Propagate uncertainty using 2d uncertainty propagation matrix.
    *
    * Parameters    const float(&jacobian)[2][2],
    *               const float(&input_variance)[2][2],
    *                     float(&output_variance)[2][2]
    *
    * Returns       N/A
    *=============================================================================================*/
   void propagate_uncertainty(const float (&jacobian)[2][2], const float (&input_variance)[2][2], float (&output_variance)[2][2]);

   /*=============================================================================================*\
    * Function      constant_uncertainty_propagation_2d(const float constant, float(&covariance)[2][2])
    *
    * Description   Multiply uncertainty by constant.
    *
    * Parameters    const float constant,
    *                     float(&covariance)[2][2]
    *
    * Returns       N/A
    *=============================================================================================*/
   void propagate_uncertainty(const float constant, float (&covariance)[2][2]);

   /*=============================================================================================*\
    * Function      calculate_point_distance
    *
    * Description   Calculates point distance from origin of cartesian coordinate system.
    *
    * Parameters    const float x
    *               const float y
    *
    * Returns       float
    *=============================================================================================*/
   inline float calculate_point_distance(const float x, const float y)
   {
      return std::hypotf(x, y);
   }

   /*============================================================================================*\
    * Function      calculate_atan_positive
    *
    * Description   Calculates angle value and when negative shift it to positive.
    *
    * Parameters    const float x
    *               const float y
    *
    * Returns       float
    *=============================================================================================*/
   inline float calculate_atan_positive(const float x, const float y)
   {
      const float azimuth = atan2f(x, y);
      return (azimuth < 0.0F) ? azimuth + TWO_PI : azimuth;
   }

   /*============================================================================================*\
    * Function      calculate_azimuth
    *
    * Description   Calculates azimuth.
    *
    * Parameters    const float vcs_x
    *               const float vcs_y
    *
    * Returns       float
    *=============================================================================================*/
   inline float calculate_azimuth(const float x, const float y)
   {
      return calculate_atan_positive(x, y);
   }

   /*=============================================================================================*\
    * Function      logistic_function_value
    *
    * Description   Computes logistic function_value.
    *
    * Parameters    const float x
    *               const float shift
    *
    * Returns       float
    *=============================================================================================*/
   inline float logistic_function_value(const float x, const float shift)
   {
      return (1.0F / (1.0F + (expf(-x + shift))));
   }

   /*=============================================================================================*\
    * Function      sign
    *
    * Description   Computes sign function.
    *
    * Parameters    const float x
    *
    * Returns       float
    *=============================================================================================*/
   inline float sign(const float x)
   {
      float result{};
      if (x == 0.0F)
      {
         result = 0.0F;
      }
      else if (x > 0.0F)
      {
         result = 1.0F;
      }
      else
      {
         result = -1.0F;
      }

      return result;
   }

   /**
    * @brief    Calculates rotation matrix.
    *
    * @param [out]: rotation_matrix
    * @param [in]: rotation_angle
    *
    * @return   void
    **/
   inline void get_rotation_matrix2d(Matrix<float, 2, 2> &rotation_matrix, const float rotation_angle)
   {
      const float cos_angle = cosf(rotation_angle);
      const float sin_angle = sinf(rotation_angle);
      rotation_matrix[0][0] = cos_angle;
      rotation_matrix[0][1] = -sin_angle;
      rotation_matrix[1][0] = sin_angle;
      rotation_matrix[1][1] = cos_angle;
   }

   /**
    * @brief       This function calculates a lateral position in curvilinear system.
    *
    *
    * @param[in]   host_curvature_rear - estimated Aptiv VCS curvature of historic host path assuming circular motion model
    * @param[in]   position - vertex position
    *
    * @return      reduced y coordinate for the point
    **/
   static inline float calculate_curvi_lat_pos(const float host_curvature_rear, const geometry::Point2D_T &position)
   {
      const float x_vcs_squared   = position.x * position.x;
      const float y_curvi_lat_pos = position.y - (0.5F * host_curvature_rear * x_vcs_squared);

      return y_curvi_lat_pos;
   }

   /**
    * @brief    Restricts a value to be within a specified range between min_value and max_value.
    *
    * @param    value
    * @param    min_value
    * @param    max_value
    *
    * @return   value within specified range
    **/
   template <typename T>
   T clamp(const T value, const T min_value, const T max_value)
   {
      T result = 0.0F;

      if (value > max_value)
      {
         result = max_value;
      }
      else if (value < min_value)
      {
         result = min_value;
      }
      else
      {
         result = value;
      }

      return result;
   }
}
#endif
