#ifndef MATH_MACROS_H
#define MATH_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "st_math.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

#define QUOTIENT_CEILED(x,y) (Quotient_Ceiled(x,y))

#ifdef __cplusplus
}
#endif
#endif
