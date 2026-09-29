#ifndef ML_MACROS_H
#define ML_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <stddef.h>
#include "ml_compiler_warning.h"
/**
 * \defgroup macros Macros
 * Some macros that do not fit anywhere else
 */

/* Since this file defines a bunch of function-like macros the QAC check
"A function could probably be used instead of this function-like macro."
Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */


/**
* Ensure a definition of NULL exists
* \ingroup macros
*/
#ifndef NULL
#define NULL 0
#endif

/**
* Swaps two values x and y of type T
* \ingroup macros
* \sdd{WI-13996}
*/
#define Swap(x, y, T)                                                                  \
do { T SWAP = x; x = y; y = SWAP;  /* PRQA S 3410 */                                   \
Msvs_Disable_Warning(4127); /*Disabling warning'conditional expression is constant '*/ \
}                                                                                      \
while (0)/* PRQA S 3412 */

#ifdef __cplusplus
}
#endif
#endif
