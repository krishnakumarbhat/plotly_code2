#ifndef ML_VECTOR_3D_T_H
#define ML_VECTOR_3D_T_H
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
typedef struct Vector_3d_Tag Vector_3d_T;

#ifdef __cplusplus
}
}
/**
* \brief Defines a 2d vector using the normal naming convention: (x,y)
* \ingroup Vector_3d_algebra
*/
typedef ml::Vector_3d_T Vector_3d_T;
#endif
#endif
