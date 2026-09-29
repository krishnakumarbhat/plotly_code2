#ifndef ML_ANGLE_HPP
#define ML_ANGLE_HPP

/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_angle_t.h"
#include "ml_trigonometry.h"
#include "ml_angle_normalize.h"

/**
* \defgroup Angle_cpp Angle C++
* Functions that allow to work with \ref Angle_T structure in C++.
*/

/***
* There is functionality that needs the structures Vector_2d_T and Angle_T.
* Because user expectations for where these functions can be found may differ
* these reside in a separate header ml_vector_2d_angle.hpp
* Both st_vector_2d.hpp and st_angle.hpp include this combined header to ensure that
* users always find the corresponding functions.
*/
#include "ml_vector_2d_angle.hpp"

namespace ml
{
   namespace angle {
      /**
      * Creates an angle structure based upon an angle given in [rad](https:\\en.wikipedia.org/wiki/Radian)
      * \return           Angle_T based upon an angle given in rad
      * \sdd{WI-13809}
      * \sa Create_Angle()
      * \ingroup Angle_cpp
      */
      inline Angle_T Angle(
         const float angle_rad /**<* [in] angle to create an Angle_T from */
      );

      /**
       * \ingroup Angle_cpp
       */
      inline Angle_T Angle(
         const Vector_2d_T vector /**<* [in] vector to create an Angle_T from */
      );
   }

   /**
   * Resolves the \f$2\pi\f$ ambiguity in the first input by shifting it to be within \f$+-\pi\f$ of p_theta_ref.
   * \par Example
   * Let \f$\tau \f$ be a full rotation: \f$\tau = 2* \pi \f$
   * - Assume p_theta_in to be 17.5 * \f$\tau\f$
   * - Assume p_theta_ref to be 0.3 * \f$\tau\f$
   * - The output will be 0.5 * \f$\tau\f$
   *
   * - Assume p_theta_in to be 0.5 * \f$\tau\f$
   * - Assume p_theta_ref to be 17.3 * \f$\tau\f$
   * - The output will be 17.5 * \f$\tau\f$
   *
   * Be cautious when calling this function with greatly differing theta_in and theta_ref angles. The result will
   * be erroneous because of floating point inaccuracies.
   * \return         Normalized angle
   * \ingroup Angle_cpp
   * \sdd{WI-13807}
   * \sa Normalize_Angle_Struct()
   */
   inline Angle_T Normalize(
      const Angle_T  &theta_in /**< [in, out] input angle which should be shifted*/,
      const Angle_T  &theta_ref /**< [in] reference angle*/
   );

   /**
   * Computes the difference between given two angles
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator-(
      const Angle_T &angle_a, /**< [in] minuend */
      const Angle_T &angle_b  /**< [in] subtrahend */
      );

   /**
   * Computes the difference between given float and an angle
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline float operator-(
      const float   &angle_a, /**< [in] minuend */
      const Angle_T &angle_b /**< [in] subtrahend */
      );

   /**
   * Computes the difference between given float and an angle
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator-=(
      Angle_T &angle_a, /**< [in] minuend */
      const Angle_T &angle_b /**< [in] subtrahend */
      );

   /**
   * Computes the difference between given float and an angle
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline float operator-=(
      float   &angle_a, /**< [in] minuend */
      const Angle_T &angle_b /**< [in] subtrahend */
      );

   /**
   * Computes the sum of given float and angle
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline float operator+=(
      float   &angle_a, /**< [in] summand */
      const Angle_T &angle_b  /**< [in] summand */
      );

   /**
   * Computes the difference between given angle and a float
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator-(
      const Angle_T &angle_a, /**< [in] minuend */
      const float    angle_b  /**< [in] subtrahend */
      );

   /**
   * Computes the difference between given angle and a float
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator-=(
      Angle_T &angle_a, /**< [in] minuend */
      const float angle_b  /**< [in] subtrahend */
      );

   /**
   * Computes the sum of given two angles
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator+(
      const Angle_T &angle_a, /**< [in] summand */
      const Angle_T &angle_b  /**< [in] summand */
      );

   /**
   * Unary minus
   * \return negated angle
   * \ingroup Angle_cpp
   */
   inline Angle_T operator-(
      const Angle_T &angle /**< [in] summand */
      );

   /**
   * Computes the sum of given two angles
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator+=(
      Angle_T &angle_a, /**< [in] summand */
      const Angle_T &angle_b  /**< [in] summand */
      );

   /**
   * Computes the sum of given angle and float
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator+(
      const Angle_T &angle_a, /**< [in] summand */
      const float    angle_b  /**< [in] summand */
      );

   /**
   * Computes the sum of given angle and float
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline Angle_T operator+=(
      Angle_T &angle_a, /**< [in] summand */
      const float    angle_b  /**< [in] summand */
      );

   /**
   * Computes the sum of given float and angle
   * \return Difference between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13813}
   * \sa Angle_Diff()
   */
   inline float operator+(
      const float    angle_a, /**< [in] summand */
      const Angle_T &angle_b  /**< [in] summand */
      );

   /**
   * Computes the middle value between given two angles
   * \return Middle between the given angles
   * \ingroup Angle_cpp
   * \sdd{WI-13812}
   * \sa Angle_Mean()
   */
   inline Angle_T Middle(
      const Angle_T &angle_a, /**<  [in] first angle */
      const Angle_T &angle_b  /**<  [in] second angle */
   );

   /**
   * \return given angle rotated by given rotation
   * \ingroup Angle_cpp
   */
   inline Angle_T Rotate(
      const Angle_T &angle, /**<  [in] first angle */
      const Angle_T &rotation  /**<  [in] second angle */
   );

   /**
   * \return given angle rotated by given rotation
   * \ingroup Angle_cpp
   */
   inline Angle_T Rotate(
      const Angle_T &angle, /**<  [in] first angle */
      const float    rotation  /**<  [in] second angle */
   );

   namespace angle {
      inline Angle_T Angle(
         const float angle_rad
      )
      {
         Angle_T angle_return;
         angle_return.angle = angle_rad;
         angle_return.sin = Fast_Sin(angle_rad);
         angle_return.cos = Fast_Cos(angle_rad);
         return angle_return;
      }

      inline Angle_T Angle(const Vector_2d_T vector /**<* [in] vector to create an Angle_T from */)
      {
         Angle_T angle_return;
         angle_return.angle = Fast_Atan2(vector.y, vector.x);
         angle_return.sin = Fast_Sin(angle_return.angle);
         angle_return.cos = Fast_Cos(angle_return.angle);
         return angle_return;
      }
   }

   inline Angle_T Normalize(
      const Angle_T  &theta_in,
      const Angle_T  &theta_ref
   )
   {
      float normalized_angle;

      normalized_angle = Normalize_Angle(theta_in.angle, theta_ref.angle);

      return ml::angle::Angle(normalized_angle);
   }

   inline Angle_T operator-(
      const Angle_T &angle_a,
      const Angle_T &angle_b
      )
   {
      float32_T normalized_a;
      float32_T diff_of_angles;

      normalized_a = Normalize_Angle(angle_a.angle, angle_b.angle);
      diff_of_angles = normalized_a - angle_b.angle;

      return ml::angle::Angle(diff_of_angles);
   }

   inline float operator-(
      const float   &angle_a,
      const Angle_T &angle_b
      )
   {
      float32_T normalized_a;
      float32_T diff_of_angles;

      normalized_a = Normalize_Angle(angle_a, angle_b.angle);
      diff_of_angles = normalized_a - angle_b.angle;
      return diff_of_angles;
   }

   inline Angle_T operator-=(
            Angle_T &angle_a,
      const Angle_T &angle_b
      )
   {
      angle_a = angle_a - angle_b;
      return angle_a;
   }

   inline float operator-=(
            float   &angle_a,
      const Angle_T &angle_b
      )
   {
      float32_T normalized_a;

      normalized_a = Normalize_Angle(angle_a, angle_b.angle);
      angle_a      = normalized_a - angle_b.angle;
      return angle_a;
   }

   inline float operator+=(
            float   &angle_a,
      const Angle_T &angle_b
      )
   {
      float32_T normalized_a;

      normalized_a = Normalize_Angle(angle_a, angle_b.angle);
      angle_a = normalized_a + angle_b.angle;

      return angle_a;
   }

   inline Angle_T operator-(
      const Angle_T &angle_a,
      const float    angle_b
      )
   {
      float32_T normalized_a;
      float32_T diff_of_angles;

      normalized_a = Normalize_Angle(angle_a.angle, angle_b);
      diff_of_angles = normalized_a - angle_b;

      return ml::angle::Angle(diff_of_angles);
   }

   inline Angle_T operator-=(
            Angle_T &angle_a,
      const float angle_b
      )
   {
      angle_a = angle_a - angle_b;
      return angle_a;
   }

   inline Angle_T operator+(
      const Angle_T &angle_a,
      const Angle_T &angle_b
      )
   {
      float32_T normalized_a;
      float32_T sum_of_angles;

      normalized_a = Normalize_Angle(angle_a.angle, angle_b.angle);
      sum_of_angles = normalized_a + angle_b.angle;

      return ml::angle::Angle(sum_of_angles);
   }

   inline Angle_T operator-(
      const Angle_T &angle
      )
   {
      Angle_T   angle_return;

      angle_return.angle = -angle.angle;
      angle_return.sin   = -angle.sin;
      angle_return.cos   =  angle.cos;

      return angle_return;
   }

   inline Angle_T operator+=(
            Angle_T &angle_a,
      const Angle_T &angle_b
      )
   {
      angle_a = angle_a + angle_b;
      return angle_a;
   }

   inline Angle_T operator+(
      const Angle_T &angle_a,
      const float    angle_b
      )
   {
      float32_T normalized_a;
      float32_T sum_of_angles;

      normalized_a = Normalize_Angle(angle_a.angle, angle_b);
      sum_of_angles = normalized_a + angle_b;

      return ml::angle::Angle(sum_of_angles);
   }

   inline Angle_T operator+=(
            Angle_T &angle_a,
      const float    angle_b
      )
   {
      angle_a = angle_a + angle_b;
      return angle_a;
   }

   inline float operator+(
      const float    angle_a,
      const Angle_T &angle_b
      )
   {
      float32_T normalized_a;
      float32_T sum_of_angles;

      normalized_a = Normalize_Angle(angle_a, angle_b.angle);
      sum_of_angles = normalized_a + angle_b.angle;

      return sum_of_angles;
   }

   inline Angle_T Middle(
      const Angle_T &angle_a,
      const Angle_T &angle_b
   )
   {
      float32_T normalized_a;
      float32_T mean_of_angles;

      normalized_a = Normalize_Angle(angle_a.angle, angle_b.angle);
      mean_of_angles = 0.5f*(normalized_a + angle_b.angle);
      mean_of_angles = Normalize_Angle(mean_of_angles, 0.0f);

      return ml::angle::Angle(mean_of_angles);
   }

   inline Angle_T Rotate(
      const Angle_T &angle,
      const Angle_T &rotation
   )
   {
      return angle + rotation;
   }

   inline Angle_T Rotate(
      const Angle_T &angle,
      const float    rotation
   )
   {
      return angle + rotation;
   }

}
#endif
