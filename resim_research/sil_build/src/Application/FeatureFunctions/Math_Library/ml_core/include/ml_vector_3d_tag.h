#ifndef ML_VECTOR_3D_TAG_H
#define ML_VECTOR_3D_TAG_H
/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

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
* \brief Defines a 3d vector using the normal naming convention: (x, y, z)
* \ingroup Vector_3d_algebra
*/
struct Vector_3d_Tag
{
   float32_T x; /**< First component of a vector */
   float32_T y; /**< Second component of a vector */
   float32_T z; /**< Third component of a vector */
};

#ifdef __cplusplus
}
}
#endif
#endif
