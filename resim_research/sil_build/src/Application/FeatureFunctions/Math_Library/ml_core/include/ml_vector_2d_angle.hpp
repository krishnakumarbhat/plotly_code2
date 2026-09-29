#ifndef ML_VECTOR_2D_ANGLE_HPP
#define ML_VECTOR_2D_ANGLE_HPP

/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_vector_2d_t.h"
#include "ml_angle_t.h"
#include "ml_trigonometry.h"
#include <cmath>

namespace ml {
   /**
    * Returns a Vector that is rotated around the origin by the provided angle
    * \return Vector that is rotated around the origin by the provided angle
    * \ingroup Vector_2d_algebra_cpp
    * \sdd{WI-13939}
    * \sa Vector_2d_Alg_Rotate()
    */
   inline Vector_2d_T Rotate(
      const Vector_2d_T &vector, /**< [in] Vector to be rotated */
      const Angle_T     &angle   /**< [in] Angle to rotate p_vector by */
   );

   /**
   * Returns a Vector that is rotated around the origin by the provided angle
   * \return Vector that is rotated around the origin by the provided angle
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13939}
   * \sa Vector_2d_Alg_Rotate()
   */
   inline Vector_2d_T Rotate(
      const Vector_2d_T &vector, /**< [in] Vector to be rotated */
      const float32_T    angle   /**< [in] Angle to rotate vector by */
   );

   /**
   * Computes the projection of a vector in a direction defined by an angle
   * \return projection of a vector in a direction defined by an angle
   * \sa https:\\en.wikipedia.org/wiki/Vector_projection
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13922}
   */
   inline float32_T Project(
      const Vector_2d_T &vector_to_project, /**< [in] Vector to be projected */
      const Angle_T     &angle              /**< [in] Direction to project p_vector_to_project to */
   );

   /**
   * Computes the scalar product of a vector with an angle
   * \return Scalar product of given vector and given angle
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13932}
   */
   inline float32_T operator*(
      const Vector_2d_T &vector, /**< [in] Vector to build a scalar product for */
      const Angle_T &angle   /**< [in] Angle to build a scalar product for */
      );

   /**
   * Computes the scalar product of an angle with a vector
   * \return Scalar product of given vector and given angle
   * \ingroup Vector_2d_algebra_cpp
   * \sdd{WI-13932}
   */
   inline float32_T operator*(
      const Angle_T     &angle, /**< [in] Angle to build a scalar product for */
      const Vector_2d_T &vector /**< [in] Vector to build a scalar product for */
      );

   inline Vector_2d_T Rotate(
      const Vector_2d_T &vector,
      const Angle_T     &angle
   )
   {
      Vector_2d_T ret_value;
      ret_value.x = (vector.x * angle.cos) - (vector.y * angle.sin);
      ret_value.y = (vector.x * angle.sin) + (vector.y * angle.cos);
      return ret_value;
   }

   inline Vector_2d_T Rotate(
      const Vector_2d_T &vector,
      const float32_T    angle
   )
   {
      Vector_2d_T ret_value;
      ret_value.x = (vector.x * Fast_Cos(angle)) - (vector.y * Fast_Sin(angle));
      ret_value.y = (vector.x * Fast_Sin(angle)) + (vector.y * Fast_Cos(angle));
      return ret_value;
   }


   inline float32_T Project(
      const Vector_2d_T &vector_to_project,
      const Angle_T     &angle
   )
   {
      float32_T ret_value;

      ret_value = std::sqrt((vector_to_project.x * vector_to_project.x) + (vector_to_project.y * vector_to_project.y)) * angle.cos;

      return ret_value;
   }

   inline float32_T operator*(
      const Vector_2d_T &vector,
      const Angle_T &angle
      )
   {
      return (vector.x * angle.cos) + (vector.y * angle.sin);
   }

   inline float32_T operator*(
      const Angle_T     &angle,
      const Vector_2d_T &vector
      )
   {
      return vector * angle;
   }
}
#endif

