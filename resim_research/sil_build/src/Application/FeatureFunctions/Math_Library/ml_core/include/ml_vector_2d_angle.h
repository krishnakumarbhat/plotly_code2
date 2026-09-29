#ifndef ML_VECTOR_2D_ANGLE_H
#define ML_VECTOR_2D_ANGLE_H

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"

#ifdef __cplusplus
extern "C"
{
#endif
/**
 * Returns a Vector that is rotated around the origin by the provided angle
 * \return Vector that is rotated around the origin by the provided angle
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13939}
 */
Vector_2d_T Vector_2d_Alg_Rotate(
   const Angle_T *const     p_angle, /**< [in] Angle to rotate p_vector by */
   const Vector_2d_T *const p_vector /**< [in] Vector to be rotated */
   );

/**
 * \return Angle_T structure that points into the direction of the given vector
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13928}
 */
Angle_T Vector_2d_Alg_Angle_From_Vector(const Vector_2d_T *const p_vector /**< [in] Vector to be converted to angle */
                                        );

/**
 * Computes the projection of a vector in a direction defined by an angle
 * \return projection of a vector in a direction defined by an angle
 * \sa https:\\en.wikipedia.org/wiki/Vector_projection
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13922}
 */
float32_T Vector_2d_Alg_Project_On_Angle(
   const Vector_2d_T *const p_vector_to_project, /**< [in] Vector to be projected */
   const Angle_T *const     p_angle              /**< [in] Direction to project p_vector_to_project to */
   );

/**
 * Computes the projection of a vector in a unit vector defined by an angle relative to the y-Axis
 * \return projection of a vector in a unit vector defined by an angle relative to the y-Axis
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13916}
 */
float32_T Vector_2d_Alg_Project_On_Rotated_X_Axis(
   const Vector_2d_T *const p_vector_to_project, /**< [in] Vector to be projected */
   const Angle_T *const     p_angle              /**< [in] unit vector to project to */
   );

/**
 * Rotates a point around the coordinate systems origin by a inverted (negative) angle.
 * \return rotated point
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13940}
 */
Vector_2d_T Vector_2d_Alg_Rotate_Negative(
   const Angle_T *const     p_angle, /**< [in] Angle to rotate by */
   const Vector_2d_T *const p_vector /**< [in] Vector to be rotated */
   );

/**
 * Computes the scalar product of a vector with an angle
 * \return Scalar product of given vector and given angle
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13932}
 */
float32_T Vector_2d_Alg_Scalar_Product_With_Angle(
   const Vector_2d_T *const p_vector, /**< [in] Vector to build a scalar product for */
   const Angle_T *const     p_angle   /**< [in] Angle to build a scalar product for */
   );

/**
 * Returns a unit vector that points into the direction given by angle
 * \return a unit vector that points into the direction given by angle
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13920}
 */
Vector_2d_T Vector_2d_Alg_Angle_To_Vector(const Angle_T *const p_angle /**< [in] Angle to get corresponding unit vector for */
                                          );

/**
 * Returns a unit vector that points into the direction given by angle + pi/2
 * \return unit vector that points into the direction given by angle + pi/2
 * \ingroup Vector_2d_algebra
 * \sdd{WI-13918}
 */
Vector_2d_T Vector_2d_Alg_Angle_To_Perpendicular_Vector(const Angle_T *const p_angle /**< [in] Angle to get corresponding rotated unit vector for */
                                                        );

#ifdef __cplusplus
}
#endif
#endif

