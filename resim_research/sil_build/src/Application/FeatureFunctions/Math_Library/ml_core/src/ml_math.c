/*===========================================================================*\
* Copyright 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_math.h"

#if defined(__GNUC__)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

boolean_T Is_Equal(const float32_T x, const float32_T y)
{
   return (boolean_T)((x) == (y)); /* PRQA S 3341 */  /* Comparing floating point expressions is done here on purpose */
}

boolean_T Is_Not_Equal(const float32_T x, const float32_T y)
{
   return (boolean_T)((x) != (y)); /* PRQA S 3341 */  /* Comparing floating point expressions is done here on purpose */
}

#pragma GCC diagnostic pop

#else

boolean_T Is_Equal(const float32_T x, const float32_T y)
{
   return (boolean_T)((x) == (y)); /* PRQA S 3341 */  /* Comparing floating point expressions is done here on purpose */
}

boolean_T Is_Not_Equal(const float32_T x, const float32_T y)
{
   return (boolean_T)((x) != (y)); /* PRQA S 3341 */  /* Comparing floating point expressions is done here on purpose */
}

#endif
