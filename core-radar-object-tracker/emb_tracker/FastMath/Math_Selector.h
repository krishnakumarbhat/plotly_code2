#ifndef MATH_SELECTOR_H
#define MATH_SELECTOR_H
/*===================================================================*\
* Copyright 2004, Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential.
*--------------------------------------------------------------------
*
* Description: See
http://pep.usinkok.northamerica.delphiauto.net/projectdb/public/?page=wiki-view&name=Trigonometric_Functions&pid=2850.6859
*
* Applicable Standards (in order of precedence: highest first):
*
* Deviations from Delco C Coding standards:
*
*
\*===================================================================*/
/*===========================================================================*\
* Includes
\*===========================================================================*/

#include <math.h>

/*===========================================================================*\
* Type definitions
\*===========================================================================*/

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/*===========================================================================*\
* MACROS
\*===========================================================================*/

/* Since this file defines a bunch of function-like macros the QAC check
   "A function could probably be used instead of this function-like macro."
   Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/* Currently we do not have a reliable way to set FAST_MATH_TABLES and FAST_ABSF_MACRO
   according to the platform the shared toolbox runs on. Since we do currently not have
   a project that needs to deactivate FAST_MATH_TABLES and FAST_ABSF_MACRO we can
   overcome this issue by permanently defining FAST_MATH_TABLES and FAST_ABSF_MACRO
   here: */
#ifndef FAST_MATH_TABLES
   #define FAST_MATH_TABLES
#endif
#ifndef FAST_ABSF_MACRO
   #define FAST_ABSF_MACRO
#endif

#ifdef PLATFORM_RACERUNNER_MATH /** use math functions optimized for Racerunner processor */
   #include "optimised_basic_ops.h"
   #define FAST_SQRT(x)     (mrr_base_ops_sqrt(x))
   #define FAST_HYPOT(x, y) (mrr_opt_hypot(x, y))
#else
   #include <math.h>
   #define FAST_SQRT(x)     (sqrtf(x))
   #define FAST_HYPOT(x, y) (hypotf(x, y))
#endif

#ifdef FAST_ABSF_MACRO /** use math functions optimized for fixed point numbers */
   #define FAST_ABSF(x) (((x) < 0) ? -(x) : (x))
#else
   #include <math.h>
   #define FAST_ABSF(x) (fabsf(x))
#endif
/* Since a lot of modules still use FAST_ABS define it as well */
#define FAST_ABS(x) FAST_ABSF(x)

#ifdef FAST_MATH_TABLES /** use math functions with look up tables */
   #include "fastMath.h"
   #define FAST_COS(x)      (fast_cos(x))
   #define FAST_ACOS(x)     (fast_acos(x))
   #define FAST_SIN(x)      (fast_sin(x))
   #define FAST_ASIN(x)     (fast_asin(x))
   #define FAST_TAN(x)      (fast_tan(x))
   #define FAST_ATAN(x)     (fast_atan(x))
   #define FAST_ATAN2(y, x) (fast_atan2(y, x))
   #define FAST_EXP(x)      (fast_exp(x))
#else
   #include <math.h>
   #define FAST_COS(x)      (cosf(x))
   #define FAST_ACOS(x)     (acosf(x))
   #define FAST_SIN(x)      (sinf(x))
   #define FAST_ASIN(x)     (asinf(x))
   #define FAST_TAN(x)      (tanf(x))
   #define FAST_ATAN(x)     (atanf(x))
   #define FAST_ATAN2(y, x) (atan2f(y, x))
   #define FAST_EXP(x)      (expf(x))
#endif

#endif /* Math_Selector_H */
