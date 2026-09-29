#ifndef BASIC_MATH_H
#define BASIC_MATH_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

#include "Basic_Math_Structs.h"
#include "Vector_2d.h"
#include "Math_Selector.h"

#include "st_interval.h"
#include "st_checked_rounding.h"
#include "st_line.h"
#include "st_angle.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

 /**
 * Returns value limited to be within min_value and max_value.
 * This macro purely exists for backwards compatibility, use Enforce_Range() directly.
 * \return Clipped value
 * \ingroup interval
 * \sdd{WI-13852}
 */
#define  ForceRange(a,b,c) (Enforce_Range(a,b,c))

/**
* This macro purely exists for backwards compatibility, use Normalize_Angle_Struct() directly.
* \return         Normalized angle
* \ingroup angle
* \sdd{WI-13807}
*/
#define NormalizeAngleStruct(a, b) (Normalize_Angle_Struct(a, b))

/**
* This macro purely exists for backwards compatibility, use Normalize_Angle() directly.
* \return         Normalized angle
* \ingroup angle
* \sdd{WI-13810}
*/
#define NormalizeAngle(a, b) (Normalize_Angle(a, b))

/**
* This macro purely exists for backwards compatibility, use Angle_Diff() directly.
* \return         Normalized angle
* \ingroup angle
* \sdd{WI-13810}
*/
#define Angle_diff(a, b) (Angle_Diff(a, b))


/**
* This macro purely exists for backwards compatibility, use As_Roundf() directly.
* \ingroup checked_rounding
*/
#define AS_ROUNDF(x) (As_Roundf(x))

#ifdef __cplusplus
}
#endif
#endif

