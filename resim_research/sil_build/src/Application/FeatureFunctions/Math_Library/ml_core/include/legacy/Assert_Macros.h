#ifndef ASSERT_MACROS_H
#define ASSERT_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include "st_vector_2d_t.h"
#include "st_angle_t.h"
#include "st_int_range_t.h"
#include "st_float_range_t.h"
#include "st_vector_2d_t.h"
#include "st_line_parameter_t.h"
#include "st_line_segment_t.h"
#include "Geometric_2d_Structs.h"


/* Since this file defines a bunch of function-like macros the QAC check
   "A function could probably be used instead of this function-like macro."
   Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

#ifdef _MSC_VER
/* Variant that gives no compiler warning in visual studio */
#define SHARED_TOOLBOX_ASSERT_NOOP ((void)0)
#else
/* Variant that gives no warning in QAC */
#define SHARED_TOOLBOX_ASSERT_NOOP do { ; } while (0)
#endif

#ifdef _MSC_VER
#define MSC_SUPPRESS_INFINITY_WARNING __pragma(warning(suppress:4127))
#else
#define MSC_SUPPRESS_INFINITY_WARNING
#endif

#ifdef NDEBUG
#define TOOLBOX_ASSERT(_Expression)     SHARED_TOOLBOX_ASSERT_NOOP
#else
#define TOOLBOX_ASSERT assert
#endif

#ifdef NDEBUG
#define TOOLBOX_ASSERT_NO_NAN(x)     SHARED_TOOLBOX_ASSERT_NOOP
#else
#define	TOOLBOX_ASSERT_NO_NAN(x) TOOLBOX_ASSERT((x)==(x))
#endif

#ifdef NDEBUG
#define TOOLBOX_ASSERT_NO_NAN_RANGE(range)      SHARED_TOOLBOX_ASSERT_NOOP
#else
#define TOOLBOX_ASSERT_NO_NAN_RANGE(range) \
do { \
	TOOLBOX_ASSERT_NO_NAN((range)->min); \
	TOOLBOX_ASSERT_NO_NAN((range)->max); \
    /* Suppress warning for while(false) */ \
    MSC_SUPPRESS_INFINITY_WARNING \
} while(0)
#endif

#ifdef NDEBUG
#define TOOLBOX_ASSERT_NO_NAN_VECTOR(vector)      SHARED_TOOLBOX_ASSERT_NOOP
#else
#define TOOLBOX_ASSERT_NO_NAN_VECTOR(vector) \
do { \
	TOOLBOX_ASSERT_NO_NAN((vector).x); \
	TOOLBOX_ASSERT_NO_NAN((vector).y); \
    /* Suppress warning for while(false) */ \
    MSC_SUPPRESS_INFINITY_WARNING \
} while(0)
#endif

#ifdef NDEBUG
#define TOOLBOX_ASSERT_VECTOR_NQ(vectorA, vectorB)      SHARED_TOOLBOX_ASSERT_NOOP
#else
#define TOOLBOX_ASSERT_VECTOR_NQ(vectorA,vectorB) TOOLBOX_ASSERT(((vectorA).x!=(vectorB).x) || ((vectorA).y!=(vectorB).y))
#endif

#ifdef NDEBUG
#define TOOLBOX_ASSERT_NO_NAN_ANGLE(angle_input)      SHARED_TOOLBOX_ASSERT_NOOP
#else
#define TOOLBOX_ASSERT_NO_NAN_ANGLE(angle_input) \
do { \
	TOOLBOX_ASSERT_NO_NAN((angle_input).angle); \
	TOOLBOX_ASSERT_NO_NAN((angle_input).sin); \
	TOOLBOX_ASSERT_NO_NAN((angle_input).cos); \
    /* Suppress warning for while(false) */ \
    MSC_SUPPRESS_INFINITY_WARNING \
} while(0)
#endif

#ifdef __cplusplus
}
#endif
#endif /* Assert_Macros_H */
