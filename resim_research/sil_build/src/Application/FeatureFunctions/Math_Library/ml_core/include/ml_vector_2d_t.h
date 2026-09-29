#ifndef ML_VECTOR_2D_T_H
#define ML_VECTOR_2D_T_H
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/** \file
 * Strcuture definition for a zwo-dimensional vector. This structure shall be used
 * in C and in C++ code, both shall be able to pass instances and pointers of
 * Vector_2d_T back and forth.
 * All C++ functionality resides in the namespace 'st'. Since C does not have
 * namespaces Vector_2d_T is in the global C-namespace.
 * C++ uses the type definition st::Vector_2d_T, a typedef makes the Vector_2d_T
 * type available in global namespace to ensure interoperability between C and C++.
 */
#ifdef __cplusplus
extern "C"
{
#endif
#include "reuse.h"
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace ml {
extern "C"
{
#endif

/**
* \brief Defines a 2d vector using the normal naming convention: (x,y)
* \ingroup Vector_2d_algebra
*/
typedef struct Vector_2d_Tag
{
   float32_T x; /**< First component of a vector */
   float32_T y; /**< Second component of a vector */
} Vector_2d_T;

#ifdef __cplusplus
}
}
/**
* \brief Defines a 2d vector using the normal naming convention: (x,y)
* \ingroup Vector_2d_algebra
*/
typedef ml::Vector_2d_T Vector_2d_T;
#endif
#endif
