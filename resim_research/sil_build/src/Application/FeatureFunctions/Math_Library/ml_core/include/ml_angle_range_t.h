#ifndef ML_Angle_Range_T_H
#define ML_Angle_Range_T_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
/**
* A structure defining an angle range using a start and an end angle.
*
* [SI Units](https:\\en.wikipedia.org/wiki/International_System_of_Units) are used, therefore
* all angles are in [radians](https:\\en.wikipedia.org/wiki/Radian)
* \ingroup angle_range
*/
typedef struct Angle_Range_Tag
{
   float32_T start; /**< start angle of angle range */
   float32_T end;   /**< end angle of angle range */
} Angle_Range_T;

#ifdef __cplusplus
}
#endif
#endif
