#ifndef ML_EXP_H
#define ML_EXP_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include "ml_fast_math_table_macros.h"
#include "ml_serial_buffer_t.h"
#include "ml_serialization_error_t.h"

/**
 * \defgroup exponential Exponential Function
 * The Shared-Toolbox provides an exponential function speed up by a precomputed table.
 */

/**
 *\defgroup exponential_serialization Serialization
 * Serialization of the exponential function
 * \ingroup exponential
 */

/* Since this file defines a bunch of function-like macros the QAC check
* "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION

/**
* Computes the [exponential](https:\\en.wikipedia.org/wiki/Exponential_function) of the given float
* \return         exponential of given x
* \ingroup exponential
* \sdd{WI-14675}
*/
#define Fast_Exp(x)    (expf(x))

#endif

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE

/**
* Compute the runtime tables for the exp() function.
* \ingroup exponential
*/
void Compute_Exp_Table(void);

/**
* Serializes the exponential table into given buffer.
* \return error code
* \ingroup exponential_serialization
*/
Shared_Toolbox_Serialization_Error_T Serialize_Exp_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Serialization buffer */);

/**
* Deserializes the exponential table into given buffer.
* \return error code
* \ingroup exponential_serialization
*/
Shared_Toolbox_Serialization_Error_T Deserialize_Exp_Table(Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Serialization buffer */);

#else

/**
* Compute the runtime tables for the exp() function.
* \ingroup exponential_serialization
*/
#define Compute_Exp_Table()

/**
* Serializes the exponential table into given buffer.
* \return error code
* \ingroup exponential_serialization
*/
#define Serialize_Exp_Table(buffer) (SHARED_TOOLBOX_SRL_SUCCESS)

/**
* Deserializes the exponential table into given buffer.
* \return error code
* \ingroup exponential_serialization
*/
#define Deserialize_Exp_Table(buffer) (SHARED_TOOLBOX_SRL_SUCCESS)

#endif

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE != ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
/**
* Computes the [exponential](https:\\en.wikipedia.org/wiki/Exponential_function) of the given float
* \return         exponential of given x
* \ingroup exponential
* \sdd{WI-14675}
*/
extern float32_T Fast_Exp(float32_T x /**< Value to compute the exponential for */);

#endif

#ifdef __cplusplus
}
#endif
#endif
