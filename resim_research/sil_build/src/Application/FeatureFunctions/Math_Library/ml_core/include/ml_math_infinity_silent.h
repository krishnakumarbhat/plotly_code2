#ifndef ML_MATH_INFINITY_SILENT_H
#define ML_MATH_INFINITY_SILENT_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===================================================================*\
* Copyright 2021, Aptiv, All Rights Reserved.
* Confidential.
\*===================================================================*/

#include <math.h>

/**
 * \brief At least a really big number, not guaranteed to be infinity.
 *
 * Not all platforms provide a definition of an infinity value. AS_TOOLBOX_INFINITY tries to
 * get the best value available:
 * - INFINITY if available
 * - HUGE_VALF if INFINITY is not available
 * - ((float32_T)1E+36) If INFINITY is not available and HUGE_VALF is also not available
 *
 * \attention
 * An IEEE float INFINITY has the following properties. These are not guaranteed by AS_TOOLBOX_INFINITY.
 * If your implementation needs any of these properties AS_TOOLBOX_INFINITY should NOT be used!
 * Operation                | Result
 * -------------------------|--------
 * n / +-Infinity           | 0
 * +-Infinity * +-Infinity  | +-Infinity
 * +-nonZero / +-0          | +-Infinity
 * +-Infinite * +-Infinity    | +-Infinity
 * Infinity + Infinity      | +Infinity
 * Infinity - -Infinity     | +Infinity
 * -Infinity - Infinity     | NaN
 * -Infinity + - Infinity   | NaN
 * +-0 / +-0                | NaN
 * +-Infinity / +-Infinity  | NaN
 * +-Infinity * 0           | NaN
 * NaN == NaN               | False
 * \ingroup math
 */

#ifdef __TASKING__
   #include "reuse.h"
   /* The Tasking compiler actually does define INFINITY.
    * However, it does not allow assigning INFINITY to a float value.
    * To be able to do so, we set it to a very high float value. */
   #define AS_TOOLBOX_INFINITY ((float32_T)1E+36)
#elif defined(INFINITY)
   #define AS_TOOLBOX_INFINITY (INFINITY)
#else
   #ifdef HUGE_VALF
      /* On this compiler INFINITY is not defined.*/
      #define AS_TOOLBOX_INFINITY (HUGE_VALF)
   #else
      #include "reuse.h"
      /* On this compiler neither INFINITY nor HUGE_VALF are defined. */
      #define AS_TOOLBOX_INFINITY ((float32_T)1E+36)
   #endif
#endif

#ifdef __cplusplus
}
#endif
#endif

