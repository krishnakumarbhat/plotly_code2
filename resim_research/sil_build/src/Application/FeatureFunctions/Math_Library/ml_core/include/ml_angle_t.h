#ifndef ML_ANGLE_T_H
#define ML_ANGLE_T_H
/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


/**
* \defgroup angle Angle
* \brief Functions for manipulating angles.
*
* All angles are in radian. See \ref unit_conversion).
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
* \brief An angle with the corresponding sin and cos already calculated. The angle is defines in rad.
* \ingroup angle
*/
typedef struct Angle_Tag
{
   float32_T angle; /**< Angle in [radian](https:\\en.wikipedia.org/wiki/Radian)*/
   float32_T sin;   /**< [sine](https:\\en.wikipedia.org/wiki/Sin) of angle*/   /* PRQA S 0781 */
   float32_T cos;   /**< [cosine](https:\\en.wikipedia.org/wiki/Cos) of angle*/ /* PRQA S 0781 */
} Angle_T;
#ifdef __cplusplus
}
}
typedef ml::Angle_T Angle_T;
#endif
#endif
