#ifndef ML_CHECKED_ROUNDING_H
#define ML_CHECKED_ROUNDING_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"
#include <math.h> /* for ceilf() and floorf() */

/** Compiler independent round function. To round to integers use one of the
* Roundf_Checked_* functions provided in Basic_Math.h
* Values >= 0.5 are rounded up.
* \ingroup checked_rounding
* \sdd{WI-13859}
*/
#define Ml_Roundf(x) (((x) < 0.0f) ? ceilf((x) - 0.5f) : floorf((x) + 0.5f)) /* PRQA S 3453 */

/**
* \defgroup checked_rounding Checked Rounding
* \brief Rounding while type casting can go wrong if the resulting rounded value does not fit into the target type.
*
* Therefore utility functions exist that assert if rounding from float leads to a value that does not fit into the target type.
* Values
*/
/**
* Rounds a given float32_T to uint8. Throws an assertion if the given value does not fit into uint8.
* \return rounded value
* \throws Assertion if the given value is outside the target type range
* \ingroup checked_rounding
* \sdd{WI-13860}
* \sdd{WI-13866}
*/
uint8_t Roundf_Checked_Uint8(const float32_T value /**< [in] value to be rounded */);



/**
* Rounds a given float32_T to int8. Throws an assertion if the given value does not fit into int8.
* \return rounded value
* \throws Assertion if the given value is outside the target type range
* \ingroup checked_rounding
* \sdd{WI-13860}
* \sdd{WI-13863}
*/
int8_t Roundf_Checked_Int8(const float32_T value /**< [in] value to be rounded */);


/**
* Rounds a given float32_T to uint16. Throws an assertion if the given value does not fit into uint16.
* \return           rounded value
* \throws Assertion if the given value is outside the target type range
* \ingroup checked_rounding
* \sdd{WI-13860}
* \sdd{WI-13865}
*/
uint16_t Roundf_Checked_Uint16(const float32_T value /**< [in] value to be rounded */);


/**
* Rounds a given float32_T to int16. Throws an assertion if the given value does not fit into int16.
* \return rounded value
* \throws Assertion if the given value is outside the target type range
* \ingroup checked_rounding
* \sdd{WI-13860}
* \sdd{WI-13861}
*/
int16_t Roundf_Checked_Int16(const float32_T value /**< [in] value to be rounded */);


/**
* Rounds a given float32_T to uint32. Throws an assertion if the given value does not fit into uint32.
* \return rounded value
* \throws Assertion if the given value is outside the target type range
* \ingroup checked_rounding
* \sdd{WI-13860}
* \sdd{WI-13864}
*/
uint32_t Roundf_Checked_Uint32(const float32_T value /**< [in] value to be rounded */);


/**
* Rounds a given float32_T to int32. Throws an assertion if the given value does not fit into int32.
* \return rounded value
* \throws Assertion if the given value is outside the target type range
* \ingroup checked_rounding
* \sdd{WI-13860}
* \sdd{WI-13862}
*/
int32_t Roundf_Checked_Int32(const float32_T value /**< [in] value to be rounded */);


#ifdef __cplusplus
}
#endif
#endif
