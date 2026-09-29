#ifndef VECTOR_2D_ALGEBRA_H
#define VECTOR_2D_ALGEBRA_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "st_vector_2d.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_Project_on_Angle(a, b) (Vector_2d_Alg_Project_On_Angle(a, b))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_Project_on_rotated_xAxis(a, b) (Vector_2d_Alg_Project_On_Rotated_X_Axis(a, b))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_Abs_squared(a) (Vector_2d_Alg_Abs_Squared(a))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_RotateNegative(a, b) (Vector_2d_Alg_Rotate_Negative(a, b))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_Abs_ComponentWise(a) (Vector_2d_Alg_Abs_Component_Wise(a))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_Sqrt_ComponentWise(a) (Vector_2d_Alg_Sqrt_Component_Wise(a))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_Scalar_Product_with_angle(a, b) (Vector_2d_Alg_Scalar_Product_With_Angle(a, b))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_CalculateCosBetweenTwoVec(a, b) (Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(a, b))

/**
* Used for backwards compatibility. Use function in st_vector_2d.h directly.
* \ingroup Vector_2d_algebra
*/
#define Vector_2d_Alg_LimitVector(a, b, c) (Vector_2d_Alg_Limit_Vector(a, b, c))

#ifdef __cplusplus
}
#endif
#endif 
