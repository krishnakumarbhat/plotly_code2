#ifndef ML_TRIGONOMETRY_H
#define ML_TRIGONOMETRY_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_fast_math_table_macros.h"
#include "reuse.h"

/* The racerunner offers some optimized functions */
#ifdef PLATFORM_RACERUNNER_MATH
#include "optimised_basic_ops.h"
/**
* Compute the [hypotenuse](https:\\en.wikipedia.org/wiki/Hypotenuse) of given x and y
* \ingroup math
* \return hypotenuse of given x and y
* \sdd{WI-14658}
*/
#define Fast_Hypot(x, y)    (mrr_opt_hypot(x, y))
#else
#include <math.h>
/**
* Compute the [hypotenuse](https:\\en.wikipedia.org/wiki/Hypotenuse) of given x and y
* \ingroup trigonometric_functions
* \return hypotenuse of given x and y
* \sdd{WI-14658}
*/
#define Fast_Hypot(x, y)    (hypotf(x, y)) /* PRQA S 3453 */ /* Macro used to switch between implementations */
#endif

/**
* \defgroup trigonometric_functions_serialization Serialization of fast math tables
* In case the math tables are computed at run time the values in the table
* depend on the compiler and the float processing hardware used.
* Since this hinders reproducing the results of trigonometric calculations
* on a different hardware (e.g. in a SIL environment) a possibility to read out
* the table content is needed.
* See \ref fast_math_serialization
* \ingroup trigonometric_functions
*/

/* Since this file defines a bunch of function-like macros the QAC check
* "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* \defgroup trigonometric_functions Trigonometric Functions
* \brief The MathLibrary offers fast lookup table based trigonometric functions.
* See [Fast Math](fast_math.md) For more information.
*
* \section Design considerations
* The option to compute the table content at runtime introduces a few consequences:
* The Fast_*() functions need to know the table. Therefore they
* - need to receive a pointer to the table as a parameter
* - tables need to be accessible globally
* - the tables must be statics in the same source file as the fast_math functions
*
* The last of these options has been chosen.
*
* To ensure that a SIL environment can produce the same results as an embedded build
* serialization of the table content is offered.
*/

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
#include <math.h>

/**
* Computes the [cosine](https:\\en.wikipedia.org/wiki/Cos) of the given float
* \ingroup trigonometric_functions
* \return         cosine of given x
* \sdd{WI-14657}
*/
#define Fast_Cos(x)         (cosf(x))

/**
* Computes the [arc cosine](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* \return         arc cosine of given x
* \ingroup trigonometric_functions
* \sdd{WI-14665}
*/
#define Fast_Acos(x)        (acosf(x))

/**
* Computes the [sine](https:\\en.wikipedia.org/wiki/Sin) of the given float
* \ingroup trigonometric_functions
* \return         sine of given x
* \sdd{WI-14664}
*/
#define Fast_Sin(x)         (sinf(x))

/**
* Computes the [arcus sinus](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions#arcsin) of the given float
* \return arcus sinus of given x
* \ingroup trigonometric_functions
* \sdd{WI-14663}
*/
#define Fast_Asin(x)        (asinf(x))

/**
* Computes the [tangent](https:\\en.wikipedia.org/wiki/Tangent_function) of the given float
* \ingroup trigonometric_functions
* \return         tangent of given x
* \sdd{WI-14662}
*/
#define Fast_Tan(x)         (tanf(x))

/**
* Computes the [arctangent](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* \return         arctangent of given x
* \ingroup trigonometric_functions
* \sdd{WI-14661}
*/
#define Fast_Atan(x)        (atanf(x))

/**
* Computes the [atan2](https:\\en.wikipedia.org/wiki/Atan2) of the given float
* \return         atan2 of given x
* \ingroup trigonometric_functions
* \sdd{WI-14659}
*/
#define Fast_Atan2(y, x)    (atan2f(y, x))

#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE

#include "ml_serial_buffer_t.h"
#include "ml_serialization_error_t.h"

/**
* Compute the runtime tables for the trigonometric functions.
* \ingroup trigonometric_functions
* \sdd{WI-14671}
*/
void Compute_Trig_Tables(void);

/**
* Serializes the trigonometric tables into given buffer.
* \return error code
* \ingroup trigonometric_functions_serialization
*/
Shared_Toolbox_Serialization_Error_T Serialize_Trig_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Serialization buffer */);

/**
* Serializes the checksums of the trigonometric tables into given buffer.
* The resulting data stream can be passed to Set_Trig_Table_By_Checksum()
* \return error code
* \ingroup trigonometric_functions_serialization
*/
Shared_Toolbox_Serialization_Error_T Serialize_Trig_Table_Checksum(Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Serialization buffer */);

/**
* Deserializes the trigonometric tables into given buffer.
* \return error code
* \ingroup trigonometric_functions_serialization
*/
Shared_Toolbox_Serialization_Error_T Deserialize_Trig_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Serialization buffer */);

#else

/**
* Compute the runtime tables for the trigonometric functions.
* \ingroup trigonometric_functions
*/
#define Compute_Trig_Tables()

/**
* Serializes the trigonometric tables into given buffer.
* \return error code
* \ingroup trigonometric_functions_serialization
*/
#define Serialize_Trig_Table(buffer) (SHARED_TOOLBOX_SRL_SUCCESS)

/**
* Deserializes the trigonometric tables into given buffer.
* \return error code
* \ingroup trigonometric_functions_serialization
*/
#define Deserialize_Trig_Table(buffer) (SHARED_TOOLBOX_SRL_SUCCESS)

/**
* Serializes the trigonometric tables checksums into given buffer.
* \return error code
* \ingroup trigonometric_functions_serialization
*/
#define Serialize_Trig_Table_Checksum(buffer) (SHARED_TOOLBOX_SRL_ERR_UNKNOWN)

#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE != ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION

/**
* Computes the [cosine](https:\\en.wikipedia.org/wiki/Cos) of the given float
* \ingroup trigonometric_functions
* \return         cosine of given x
* \sdd{WI-14657}
*/
extern float32_T Fast_Cos(float32_T x /**< Value to compute the cosine for */);

/**
* Computes the [sine](https:\\en.wikipedia.org/wiki/Sin) of the given float
* \ingroup trigonometric_functions
* \return         sine of given x
* \sdd{WI-14664}
*/
extern float32_T Fast_Sin(float32_T x /**< Value to compute the sine for */);

/**
* Computes the [tangent](https:\\en.wikipedia.org/wiki/Tangent_function) of the given float
* \ingroup trigonometric_functions
* \return         tangent of given x
* \sdd{WI-14662}
*/
extern float32_T Fast_Tan(float32_T x /**< Value to compute the tangent for */);

/**
* Computes the [arctangent](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* \return         arctangent of given x
* \ingroup trigonometric_functions
* \sdd{WI-14661}
*/
extern float32_T Fast_Atan(float32_T x /**< Value to compute the arctangent for */);

/**
* Computes the [atan2](https:\\en.wikipedia.org/wiki/Atan2) of the given float
* \return         atan2 of given x
* \ingroup trigonometric_functions
* \sdd{WI-14659}
*/
extern float32_T Fast_Atan2(
   float32_T y /**< X-coordinate of the point to compute the atan2 for */,
   float32_T x /**< Y-coordinate of the point to compute the atan2 for */);

/**
* Computes the [arcus sinus](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions#arcsin) of the given float
* \return arcus sinus of given x
* \ingroup trigonometric_functions
* \sdd{WI-14663}
*/
extern float32_T Fast_Asin(float32_T x /**< Value to compute the cosine for */);

/**
* Computes the [arc cosine](https:\\en.wikipedia.org/wiki/Inverse_trigonometric_functions) of the given float
* \return         arc cosine of given x
* \ingroup trigonometric_functions
* \sdd{WI-14665}
*/
extern float32_T Fast_Acos(float32_T x /**< Value to compute the arc cosine for */);

#endif

/**
 * \defgroup trigonometric_functions_triangles Triangles
 * \ingroup trigonometric_functions
 * This module collects trigonometric functions.
 * Triangles are defined as follows:
 * The corners are named A, B and C counter clockwise.
 * The side that is opposite to a corner accordingly is named a, b and c
 * The angle at the corners A, B and C are named alpha, beta and gamma
 */

/**
* Returns the angle alpha for given sides a, b and c
*
* \return    float alpha
* \ingroup trigonometric_functions_triangles
* \sdd{WI-28394}
*/
float Triangle_Alpha_From_Abc(
   float a, /**< Side opposite angle Alpha */
   float b, /**< Side opposite angle Beta */
   float c  /**< Side opposite angle Gamma */
);

/**
* Returns the angle beta for given sides a, b and c
*
* \return    float beta
* \ingroup trigonometric_functions_triangles
* \sdd{WI-28394}
*/
float Triangle_Beta_From_Abc(
   float a, /**< Side opposite angle Alpha */
   float b, /**< Side opposite angle Beta */
   float c  /**< Side opposite angle Gamma */
);

/**
* Returns the angle gamma for given sides a, b and c
*
* \return    float gamma
* \ingroup trigonometric_functions_triangles
* \sdd{WI-28394}
*/
float Triangle_Gamma_From_Abc(
   float a, /**< Side opposite angle Alpha */
   float b, /**< Side opposite angle Beta */
   float c  /**< Side opposite angle Gamma */
);
#ifdef __cplusplus
}
#endif
#endif
