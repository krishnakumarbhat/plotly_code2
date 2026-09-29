#ifndef MATH_SELECTOR_H
#define MATH_SELECTOR_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <math.h>

#include "st_math.h"
#include "st_exp.h"
#include "st_trigonometry.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* Compute the [square root](https:\\en.wikipedia.org/wiki/Square_root)
* Note: This macro exists purely for backwards compatibility. Use Fast_Sqrt() directly.
* \ingroup math
* \return square root of given x
*/
#define FAST_SQRT(x)        (Fast_Sqrt(x))

/**
* Compute the [square root](https:\\en.wikipedia.org/wiki/Square_root)
* Note: This macro exists purely for backwards compatibility. Use Fast_Sqrt() directly.
* \ingroup math
* \return square root of given x
*/
#define fast_sqrt(x)        (Fast_Sqrt(x))

/**
* Compute the [hypotenuse](https:\\en.wikipedia.org/wiki/Hypotenuse) of given x and y
* Note: This macro exists purely for backwards compatibility. Use Fast_Sqrt() directly.
* \ingroup math
* \return hypothenuse of given x and y
*/
#define FAST_HYPOT(x, y)    (Fast_Hypot(x, y))

/**
* Compute the [hypotenuse](https:\\en.wikipedia.org/wiki/Hypotenuse) of given x and y
* Note: This macro exists purely for backwards compatibility. Use Fast_Sqrt() directly.
* \ingroup math
* \return hypothenuse of given x and y
*/
#define fast_hypot(x, y)    (Fast_Hypot(x, y))

/**
* Note: This macro exists purely for backwards compatibility. Use Fast_Absf() directly.
* \ingroup math
* \return absolute value of given x
* \sdd{WI-13990}
*/
#define FAST_ABSF(x)     Abs(x)

/**
* Note: This macro exists purely for backwards compatibility. Use Fast_Absf() directly.
* \ingroup math
* \return absolute value of given x
* \sdd{WI-13990}
*/
#define fast_absf(x)     Abs(x)

/**
 * Note: This macro exists purely for backwards compatibility. Use Fast_Absf() directly.
 * \ingroup math
 * \return absolute value of given x
 * \sdd{WI-13990}
 */
#define FAST_ABS(x)     Abs(x)

/**
 * Note: This macro exists purely for backwards compatibility. Use Fast_Absf() directly.
 * \ingroup math
 * \return absolute value of given x
 * \sdd{WI-13990}
 */
#define fast_abs(x)     Abs(x)


/**
* Computes the [cosine](https:\\en.wikipedia.org/wiki/Cos) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Cos() directly.
* \ingroup trigonometric_functions
* \return         cosine of given x
*/
#define FAST_COS(x)         (Fast_Cos(x))

/**
* Computes the [cosine](https:\\en.wikipedia.org/wiki/Cos) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Cos() directly.
* \ingroup trigonometric_functions
* \return         cosine of given x
*/
#define fast_cos(x)         (Fast_Cos(x))

/**
* Computes the [arc cosine](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* \return         arc cosine of given x
* Note: This macro exists purely for backwards compatibility. Use Fast_Acos() directly.
* \ingroup trigonometric_functions
*/
#define FAST_ACOS(x)        (Fast_Acos(x))

/**
* Computes the [arc cosine](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* \return         arc cosine of given x
* Note: This macro exists purely for backwards compatibility. Use Fast_Acos() directly.
* \ingroup trigonometric_functions
*/
#define fast_acos(x)        (Fast_Acos(x))

/**
* Computes the [sine](https:\\en.wikipedia.org/wiki/Sin) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Sin() directly.
* \ingroup trigonometric_functions
* \return         sine of given x
*/
#define FAST_SIN(x)         (Fast_Sin(x))

/**
* Computes the [sine](https:\\en.wikipedia.org/wiki/Sin) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Sin() directly.
* \ingroup trigonometric_functions
* \return         sine of given x
*/
#define fast_sin(x)         (Fast_Sin(x))

/**
* Computes the [arcus sinus](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions#arcsin) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Asin() directly.
* \return arcus sinus of given x
* \ingroup trigonometric_functions
*/
#define FAST_ASIN(x)        (Fast_Asin(x))

/**
* Computes the [arcus sinus](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions#arcsin) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Asin() directly.
* \return arcus sinus of given x
* \ingroup trigonometric_functions
*/
#define fast_asin(x)        (Fast_Asin(x))

/**
* Computes the [tangent](https:\\en.wikipedia.org/wiki/Tangent_function) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Tan() directly.
* \ingroup trigonometric_functions
* \return         tangent of given x
*/
#define FAST_TAN(x)         (Fast_Tan(x))

/**
* Computes the [tangent](https:\\en.wikipedia.org/wiki/Tangent_function) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Tan() directly.
* \ingroup trigonometric_functions
* \return         tangent of given x
*/
#define fast_tan(x)         (Fast_Tan(x))

/**
* Computes the [arctangent](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Cos() directly.
* \return         arctangent of given x
* \ingroup trigonometric_functions
*/
#define FAST_ATAN(x)        (Fast_Atan(x))

/**
* Computes the [arctangent](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Cos() directly.
* \return         arctangent of given x
* \ingroup trigonometric_functions
*/
#define fast_atan(x)        (Fast_Atan(x))

/**
* Computes the [atan2](https:\\en.wikipedia.org/wiki/Atan2) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Atan2() directly.
* \return         atan2 of given x
* \ingroup trigonometric_functions
*/
#define FAST_ATAN2(y, x)    (Fast_Atan2(y, x))

/**
* Computes the [atan2](https:\\en.wikipedia.org/wiki/Atan2) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Atan2() directly.
* \return         atan2 of given x
* \ingroup trigonometric_functions
*/
#define fast_atan2(y, x)    (Fast_Atan2(y, x))


/**
* Computes the [exponential](https:\\en.wikipedia.org/wiki/Exponential_function) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Exp() directly.
* \return         exponential of given x
* \ingroup trigonometric_functions
*/
#define FAST_EXP(x) (Fast_Exp(x))

/**
* Computes the [exponential](https:\\en.wikipedia.org/wiki/Exponential_function) of the given float
* Note: This macro exists purely for backwards compatibility. Use Fast_Exp() directly.
* \return         exponential of given x
* \ingroup trigonometric_functions
*/
#define fast_exp(x) (Fast_Exp(x))


#ifdef __cplusplus
}
#endif
#endif
