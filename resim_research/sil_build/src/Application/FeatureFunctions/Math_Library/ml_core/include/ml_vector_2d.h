#ifndef ML_VECTOR_2D_H
#define ML_VECTOR_2D_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


/***
 * There is functionality that needs the structures Vector_2d_T and Angle_T.
 * Because user expectations for where these functions can be found may differ
 * these reside in a separate header st_vector_2d_angle.h
 * Both st_vector_2d.h and st_angle.h include this combined header to ensure that
 * users always find the corresponding functions.
 */
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup Vector_2d_algebra 2D Vector Algebra
* \brief Vector operations
*
*  Vector operations. \n
*  \n
*  Also, Vector operations can be accelerated by using SPE2 Assembly in NXP's processors which are based on PowerPC Core. \n
*  Here are the informations about SPE2 and how use it. \n
*  \ref Vector_2d_SPE2_Doc \n
*  \ref Vector_2d_SPE2
*
*
* \section Vector_2d_SPE2_Doc What is SPE2
*
* \subsection SPE2Def Definition of SPE2
*
* SPE2 is SIMD instruction that can be used in NXP's PowerPC Architecture.
* It computes two 32 bit data types at the same time.
* So, Get 2 results in 1 operation. It is the core of SPE2, and how to reduce processing time.
*
* \subsection SPE2Lim Limitation
* <b>1. It is Hardware dependent</b>
*
* It only works in NXP's PowerPC Core that support SPE/SPE2 Assembly language. Therefore its not portable to any other platform easily.
* But, most modern processors have SIMD instructions, you can make it for other platforms.
*
* <b>2. Data type is hardly strict.</b>
*
* Assembly Language CAN NOT identify data type.
* Current SPE2 Version "Vector 2D Toolbox" support only float type (IEEE754 single precision).
* If you want to use another type, such as integer or byte, It must be implemented by other SPE2 Assembly.
*
* <b>3. Two 32bit data's position must be continuous position.</b>
*
* When SPE2 loads data into register,  Read 64bit sequentially.
* So if data is not continuous position, data will not be loaded correctly.
*
* \subsection SPE2Ref Reference
* SPE Manual : https:\\www.nxp.com/docs/en/reference-manual/SPEPEM.pdf \n
* SPE2 Manual : https:\\www.nxp.com/docs/en/reference-manual/SPE2PIM.pdf
*
*/

/**
* Functions calculating a cosine will return this if calculation fails.
* \sa Vector_2d_Alg_Calculate_Cos_Between_Two_Vec()
* \ingroup Vector_2d_algebra
* \sdd{WI-13943}
*/
#define VECTOR_2D_ALGEBRA_NOT_A_COSINE (-2.0f)

/**
* Returns a vector formed by the given x and y coordinates
* \return         vector formed by the given x and y coordinates
* \ingroup Vector_2d_algebra
* \sdd{WI-13925}
*/
Vector_2d_T Create_2d_Vector_Coordinates(
   const float32_T x, /**< [in] First component of a vector */
   const float32_T y/**< [in] Second component of a vector */);

/**
* Returns the origin
* \return         origin vector
* \ingroup Vector_2d_algebra
* \sdd{WI-13934}
*/
Vector_2d_T Create_2d_Vector_Origin(void);

/**
* Returns a normal vector pointing in x-direction
* \return         normal vector pointing in x-direction
* \ingroup Vector_2d_algebra
* \sdd{WI-13933}
*/
Vector_2d_T Create_2d_Vector_X_Normal(void);

/**
* Returns a normal vector pointing in y-direction
* \return         normal vector pointing in y-direction
* \ingroup Vector_2d_algebra
* \sdd{WI-13930}
*/
Vector_2d_T Create_2d_Vector_Y_Normal(void);





/**
* Returns a Vector that is multiplied by the given factor
* \return           Vector that is multiplied by the given factor
* \ingroup Vector_2d_algebra
* \sdd{WI-13917}
*/
Vector_2d_T Vector_2d_Alg_Multiply_Scalar(
   const Vector_2d_T * const p_vector_a, /**< [in] Vector to be multiplied with factor */
   const float32_T factor /**< [in] factor to multiply vector by*/);

/**
* Returns a Vector that is the addition of the provided vectors
* \return           Vector that is the addition of the provided vectors
* \ingroup Vector_2d_algebra
* \sdd{WI-13919}
*/
Vector_2d_T Vector_2d_Alg_Add(
   const Vector_2d_T * const p_vector_a, /**< [in] Vector to be added */
   const Vector_2d_T * const p_vector_b /**< [in] Vector to be added */);

/**
* Returns a Vector that points to the middle point of the provided vectors
* \return           Vector that points to the middle point of the provided vectors
* \ingroup Vector_2d_algebra
* \sdd{WI-13923}
*/
Vector_2d_T Vector_2d_Alg_Middle(
   const Vector_2d_T * const p_vector_a, /**< [in] Vector to find a middle point for */
   const Vector_2d_T * const p_vector_b /**< [in] Vector to find a middle point for */);

/**
* Computes the 2d scalar product between two vectors
* \return           2d scalar product between two vectors
* \sa http:\\mathworld.wolfram.com/DotProduct.html
* \ingroup Vector_2d_algebra
* \sdd{WI-13924}
*/
float32_T Vector_2d_Alg_Scalar_Product(
   const Vector_2d_T * const p_vector_a, /**< [in] Vector to compute the scalar product for */
   const Vector_2d_T * const p_vector_b /**< [in] Vector to compute the scalar product for */);

/**
* Calculates the distance between two points defined by two 2d vectors
* The function is commutative and the result is always bigger then zero
* \return           distance between given two points defined by two 2d vectors
* \ingroup Vector_2d_algebra
* \sdd{WI-13921}
*/
float32_T Vector_2d_Alg_Distance(
   const Vector_2d_T * const p_vector_a, /**< [in] One of the points to calculate the distance between */
   const Vector_2d_T * const p_vector_b /**< [in] One of the points to calculate the distance between */);

/**
* Computes a perpendicular Vector by rotating the given vector by 90deg
* \return           given p_vector rotated by 90deg
* \ingroup Vector_2d_algebra
* \sdd{WI-13936}
*/
Vector_2d_T Vector_2d_Alg_Rotate_Half_Pi(const Vector_2d_T * const p_vector /**< [in] Vector to be rotated */);

/**
* Computes the normalized version of the given vector
* \sa http:\\mathworld.wolfram.com/NormalizedVector.html
* \return           normalized vector
* \ingroup Vector_2d_algebra
* \sdd{WI-13914}
*/
Vector_2d_T Vector_2d_Alg_Normalize_Vector(const Vector_2d_T * const p_vector /**< [in] Vector to be normalized */);

/**
* Returns the absolute value (length) of a vector.
* \return the absolute value (length) of a vector.
* \sa https:\\en.wikipedia.org/wiki/Absolute_value
* \ingroup Vector_2d_algebra
* \sdd{WI-13937}
*/
float32_T Vector_2d_Alg_Abs(const Vector_2d_T * const p_vector /**< [in] Vector to compute the absolute value (length) for */);

/**
* Returns the squared absolute value (length^2) of a vector.
* \return           squared absolute value (length^2) of a vector.
* \ingroup Vector_2d_algebra
* \sdd{WI-13935}
*/
float32_T Vector_2d_Alg_Abs_Squared(const Vector_2d_T * const p_vector /**< [in] Vector to compute the squared absolute value (squared length) for */);

/**
* Computes the subtraction of two vectors.
* \return   Difference between given vectors
* \sa http:\\mathworld.wolfram.com/VectorDifference.html
* \ingroup Vector_2d_algebra
* \sdd{WI-13915}
*/
Vector_2d_T Vector_2d_Alg_Diff(
   const Vector_2d_T * const p_vector_a, /**< [in] minuend */
   const Vector_2d_T * const p_vector_b /**< [in] Subtrahend */);

/**
* Computes the absolute value of each component in the input vector.
* \return   Given vector with each component in its absolute (positive) value.
* \ingroup Vector_2d_algebra
* \sdd{WI-13938}
*/
Vector_2d_T Vector_2d_Alg_Abs_Component_Wise(const Vector_2d_T * const p_vector_a /**< [in] Vector to build the component wise absolute form of */);

/**
* Computes the square root of each component of a vector
* \return   Given vector with each component being the root of the given component in the given vector
* \ingroup Vector_2d_algebra
* \sdd{WI-13929}
*/
Vector_2d_T Vector_2d_Alg_Sqrt_Component_Wise(const Vector_2d_T * const p_vector_a /**< [in] Vector to build the component wise root for */);

/**
* Computes the cosine of the included angle between the two input vectors.
* To do the calculation a division by both vector length is needed. Therefore the vector lengths need to be nonzero
* \return   Cosine between given two vectors or \ref VECTOR_2D_ALGEBRA_NOT_A_COSINE if calculation is not possible
* \attention
* To do the calculation a division by both vector length is needed. Therefore the vector lengths need to be nonzero (longer than a threshold).
* If this isn't the case and the calculation can't be done a default value of \ref VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned.
* \ingroup Vector_2d_algebra
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
float32_T Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(
   const Vector_2d_T * const p_vector_a, /**< [in] First vector to find the cosine of the enclosed angle for */
   const Vector_2d_T * const p_vector_b /**< [in] Second vector to find the cosine of the enclosed angle for */);

/**
* Returns a vector that is limited to be within a square defined by p_limit_max_in and p_limit_min_in
* \return   vector that is limited to be within a square defined by p_limit_max_in and p_limit_min_in
* \ingroup Vector_2d_algebra
* \sdd{WI-13927}
*/
Vector_2d_T Vector_2d_Alg_Limit_Vector(
   const Vector_2d_T * const p_vect_in, /**< [in] Vector to be limited */
   const Vector_2d_T * const p_limit_max_in, /**< [in] Maximal vector */
   const Vector_2d_T * const p_limit_min_in /**< [in] Minimal vector */);

#ifdef __cplusplus
}
#endif
#endif
