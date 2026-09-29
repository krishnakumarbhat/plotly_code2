#ifndef ST_VECTOR_3D_H
#define ST_VECTOR_3D_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_vector_3d_t.h"
#include "ml_vector_3d_tag.h"
#include "reuse.h"


/**
* \defgroup Vector_3d_algebra 3D Vector Algebra
* \brief Vector operations
*
*  Vector operations.
*/

/**
* Returns a vector formed by the given x and y coordinates
* \return         vector formed by the given x and y coordinates
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Create_3d_Vector_Coordinates(
   const float32_T x, /**< [in] First component of a vector */
   const float32_T y,/**< [in] Second component of a vector */
   const float32_T z/**< [in] Third component of a vector */
   );

/**
* Returns the origin
* \return         origin vector
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Create_3d_Vector_Origin(void);

/**
* Returns a normal vector pointing in x-direction
* \return         normal vector pointing in x-direction
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Create_3d_Vector_X_Normal(void);

/**
* Returns a normal vector pointing in y-direction
* \return         normal vector pointing in y-direction
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Create_3d_Vector_Y_Normal(void);

/**
* Returns a normal vector pointing in z-direction
* \return         normal vector pointing in z-direction
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Create_3d_Vector_Z_Normal(void);





/**
* Returns a Vector that is multiplied by the given factor
* \return           Vector that is multiplied by the given factor
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Multiply_Scalar(
   const Vector_3d_T * const p_vector_a, /**< [in] Vector to be multiplied with factor */
   const float32_T factor /**< [in] factor to multiply vector by*/
   );

/**
* Returns a Vector that is the addition of the provided vectors
* \return           Vector that is the addition of the provided vectors
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Add(
   const Vector_3d_T * const p_vector_a, /**< [in] Vector to be added */
   const Vector_3d_T * const p_vector_b /**< [in] Vector to be added */);

/**
* Returns a Vector that points to the middle point of the provided vectors
* \return           Vector that points to the middle point of the provided vectors
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Middle(
   const Vector_3d_T * const p_vector_a, /**< [in] Vector to find a middle point for */
   const Vector_3d_T * const p_vector_b /**< [in] Vector to find a middle point for */);

/**
* Computes the 2d scalar product between two vectors
* \return           2d scalar product between two vectors
* \sa http:\\mathworld.wolfram.com/DotProduct.html
* \ingroup Vector_3d_algebra
*/
float32_T Vector_3d_Alg_Scalar_Product(
   const Vector_3d_T * const p_vector_a, /**< [in] Vector to compute the scalar product for */
   const Vector_3d_T * const p_vector_b /**< [in] Vector to compute the scalar product for */);

/**
* Computes the normalized version of the given vector
* \sa http:\\mathworld.wolfram.com/NormalizedVector.html
* \return           normalized vector
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Normalize_Vector(const Vector_3d_T * const p_vector /**< [in] Vector to be normalized */);

/**
* Returns the absolute value (length) of a vector.
* \return the absolute value (length) of a vector.
* \sa https:\\en.wikipedia.org/wiki/Absolute_value
* \ingroup Vector_3d_algebra
*/
float32_T Vector_3d_Alg_Abs(const Vector_3d_T * const p_vector /**< [in] Vector to compute the absolute value (length) for */);

/**
* Returns the squared absolute value (length^2) of a vector.
* \return           squared absolute value (length^2) of a vector.
* \ingroup Vector_3d_algebra
*/
float32_T Vector_3d_Alg_Abs_Squared(const Vector_3d_T * const p_vector /**< [in] Vector to compute the squared absolute value (squared length) for */);

/**
* Computes the subtraction of two vectors.
* \return   Difference between given vectors
* \sa http:\\mathworld.wolfram.com/VectorDifference.html
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Diff(
   const Vector_3d_T * p_vector_a, /**< [in] minuend */
   const Vector_3d_T * p_vector_b /**< [in] Subtrahend */);

/**
* Computes the absolute value of each component in the input vector.
* \return   Given vector with each component in its absolute (positive) value.
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Abs_Component_Wise(const Vector_3d_T * const p_vector_a /**< [in] Vector to build the component wise absolute form of */);

/**
* Computes the square root of each component of a vector
* \return   Given vector with each component being the root of the given component in the given vector
* \ingroup Vector_3d_algebra
*/
Vector_3d_T Vector_3d_Alg_Sqrt_Component_Wise(const Vector_3d_T * const p_vector_a /**< [in] Vector to build the component wise root for */);

#ifdef __cplusplus
}
#endif
#endif
