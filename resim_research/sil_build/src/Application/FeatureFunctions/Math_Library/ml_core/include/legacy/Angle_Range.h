#ifndef ANGLE_RANGE_H
#define ANGLE_RANGE_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "st_angle_range.h"

/**
* Used for backwards compatibility. Use Angle_Range_T instead.
* \ingroup angle_range
*/
typedef struct Angle_Range_Tag ANGLE_RANGE_T;

/**
* Used for backwards compatibility. Use Overlapping_Angle_Range_T instead.
* \ingroup angle_range
*/
typedef struct Overlapping_Angle_Range_Tag OVERLAPPING_ANGLE_RANGE_T;

/**
* Macro to ensure that the given angle_to_test is inside \f$+-\pi\f$
* \throws Assertion if the given angle_to_test is outside \f$+-\pi\f$
* This macro should not be used, it is no longer provided by the MathLibrary.
* \ingroup angle_range
*/
#define ASSERT_ANGLE_NORMALIZED(angle_to_test) assert(((angle_to_test) >= -PI) && ((angle_to_test) <= PI)) /* PRQA S 3453 */

#ifdef __cplusplus
}
#endif
#endif
