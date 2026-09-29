#ifndef ML_MATH_H
#define ML_MATH_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/* stdlib.h defines a min and a max macro in the MSVS version but not in some of the embedded compilers. */
#include <stdlib.h>
#include <math.h>
#include "reuse.h"

/* Since this file defines a bunch of function-like macros the QAC check
 * "A function could probably be used instead of this function-like macro."
 * Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/* The racerunner offers some optimized functions */
#ifdef PLATFORM_RACERUNNER_MATH
#include "optimised_basic_ops.h"

/**
 * Compute the [square root](https:\\en.wikipedia.org/wiki/Square_root)
 * \ingroup math
 * \return square root of given x
 * \sdd{WI-14656}
 */
#define Fast_Sqrt(x)    (mrr_base_ops_sqrt(x))
#else

/**
 * Compute the [square root](https:\\en.wikipedia.org/wiki/Square_root)
 * \ingroup math
 * \return square root of given x
 * \sdd{WI-14656}
 */
#define Fast_Sqrt(x)    (sqrtf(x))
#endif

/**
 * \defgroup math Math
 * Functionality helping with mathematical operations
 */

#ifndef Sign

/**
 * Macro to get the sign of a number
 * - Positive numbers return  1
 * - Negative numbers return -1
 * - Zero returns Zero
 * \ingroup math
 * \sdd{WI-13992}
 */
#define Sign(number)    (((number) > 0) ? 1 : (((number) < 0) ? -1 : 0))
#endif

#ifndef PI

/**
 * Constant of PI
 * \sa https:\\en.wikipedia.org/wiki/Pi
 * \ingroup math
 * \sdd{WI-13991}
 */
#define PI    (3.14159265359f)
#endif


#ifndef Abs
/**
 * Macro to process the absolute value of a number
 * \ingroup math
 * \sdd{WI-13990}
 */
#define Abs(x)    (((x) < 0) ? -(x) : (x))
#endif

#ifndef Max

/**
 * Macro to get the max value of two numbers
 * There may be other macros like MAX or max already defined in the build environment.
 * The coding guidelines want the capitalization like this macro is defined. Therefore
 * no other MAX or max macros are provided by the MathLibrary. If these exist they
 * should not be used.
 * \ingroup math
 * \sdd{WI-13986}
 * Note: If you get compiler errors when using gtest in combination with this header try to include gtest/gtest.h before including ml_math.h
 */
#define Max(a, b)    (((a) > (b)) ? (a) : (b))
#endif

#ifndef Min

/**
 * Macro to get the min value of two numbers
 * There may be other macros like MIN or min already defined in the build environment.
 * The coding guidelines want the capitalization like this macro is defined. Therefore
 * no other MIN or min macros are provided by the MathLibrary. If these exist they
 * should not be used.
 * \ingroup math
 * \sdd{WI-13988}
 * Note: If you get compiler errors when using gtest in combination with this header try to include gtest/gtest.h before including ml_math.h
 */
#define Min(a, b)    (((a) < (b)) ? (a) : (b))
#endif

/**
 * The 'epsilon' value used in the floating point calculations
 * \ingroup math
 */
#ifndef EPSILON
#define EPSILON    0.001f
#endif

/**
 * Threshold to check if a float is zero
 * \ingroup math
 * \sdd{WI-13989}
 */
#ifndef THRESHOLD_IS_ZERO
#define THRESHOLD_IS_ZERO    ((float32_T)1E-10)
#endif

/**
 * Returns the ceiled quotient. E.g.: for numerator = 10, divisor  = 4 the result will be 3.
 * Only use for positive integer values.
 * Only use with integer types, not with floating point types.
 * \ingroup math
 * \sdd{WI-13999}
 */

/* PRQA S 3453 1*/
#define Quotient_Ceiled(numerator, divisor)    ((((numerator) + (divisor)) - 1) / (divisor))

#if defined(__GNUC__)

#define Is_Not_Nan(x) !isnan(x)
#define Is_Nan(x)    (isnan(x))

#else

/**
 * \return true if given x is not NaN.
 * Can be used in assertions like so:
 * \code
 * assert(St_Is_Not_Nan(x));
 * \endcode
 * \ingroup math
 */
#define Is_Not_Nan(x)    ((x) == (x)) /* cppcheck-suppress duplicateExpression */

/**
 * \return true if given x is NaN.
 * Can be used in assertions like so:
 * \code
 * assert(Is_Nan(x));
 * \endcode
 * \ingroup math
 */
#define Is_Nan(x)    ((x) != (x)) /* cppcheck-suppress duplicateExpression */

#endif

 /**
  * WARNING: use this function with care! Only use if you understand the implication of comparing two float values with each other!
  * \return true if given x,y are equal
  * Can be used in assertions like so:
  * \code
  * assert(Is_Equal(x,y));
  * \endcode
  * \ingroup math
  */
boolean_T Is_Equal(const float32_T x, const float32_T y);

/**
 * WARNING: use this function with care! Only use if you understand the implication of comparing two float values with each other!
 * \return true if given x,y are not equal
 * Can be used in assertions like so:
 * \code
 * assert(Is_Not_Equal(x,y));
 * \endcode
 * \ingroup math
 */
boolean_T Is_Not_Equal(const float32_T x, const float32_T y);

#ifdef __cplusplus
}
#endif
#endif

