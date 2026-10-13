#ifndef FASTMATH_H
#define FASTMATH_H

#include "reuse.h"

/**
 * \defgroup trigonometric_functions Trigonometric Functions
 * \brief The shared toolbox offers fast lookup table based trigonometric functions.
 *
 */
/**
 * Computes the [cosine](https://en.wikipedia.org/wiki/Cos) of the given float
 * \ingroup trigonometric_functions
 * \return         cosine of given x
 */
extern float32_t fast_cos(float32_t x /**< Value to compute the cosine for */);

/**
 * Computes the [sine](https://en.wikipedia.org/wiki/Sin) of the given float
 * \ingroup trigonometric_functions
 * \return         sine of given x
 */
extern float32_t fast_sin(float32_t x /**< Value to compute the sine for */);

/**
 * Computes the [tangent](https://en.wikipedia.org/wiki/Tangent_function) of the given float
 * \ingroup trigonometric_functions
 * \return         tangent of given x
 */
extern float32_t fast_tan(float32_t x /**< Value to compute the tangent for */);

/**
 * Computes the [arctangent](https://en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
 * \return         arctangent of given x
 * \ingroup trigonometric_functions
 */
extern float32_t fast_atan(float32_t x /**< Value to compute the arctangent for */);

/**
 * Computes the [atan2](https://en.wikipedia.org/wiki/Atan2) of the given float
 * \return         atan2 of given x
 * \ingroup trigonometric_functions
 */
extern float32_t fast_atan2(float32_t y /**< X-coordinate of the point to compute the atan2 for */,
                            float32_t x /**< Y-coordinate of the point to compute the atan2 for */);

/**
 * Computes the [arcus sinus](https://en.wikipedia.org/wiki/Inverse_trigonometric_functions#arcsin) of the given float
 * \return arcus sinus of given x
 * \ingroup trigonometric_functions
 */
extern float32_t fast_asin(float32_t x /**< Value to compute the cosine for */);

/**
 * Computes the [arc cosine](https://en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
 * \return         arc cosine of given x
 * \ingroup trigonometric_functions
 */
extern float32_t fast_acos(float32_t x /**< Value to compute the arc cosine for */);

/**
 * Computes the [exponential](https://en.wikipedia.org/wiki/Exponential_function) of the given float
 * \return         exponential of given x
 * \ingroup trigonometric_functions
 */
extern float32_t fast_exp(float32_t x /**< Value to compute the exponential for */);

#endif
