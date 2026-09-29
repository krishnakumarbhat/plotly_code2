#ifndef BASIC_MACROS_H
#define BASIC_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "Assert_Macros.h"

#include "st_bool.h"
#include "st_math.h"
#include "st_macros.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* This macro purely exists for backwards compatibility, use Sign() directly.
* \ingroup math
* \sdd{WI-13992}
*/
#define SIGN(number) (Sign(number))

/**
* This macro purely exists for backwards compatibility, use Abs() directly.
* \ingroup math
* \sdd{WI-13990}
*/
#define ABS(x) (Abs(x))

/**
* This macro purely exists for backwards compatibility, use Swap() directly.
* \ingroup macros
* \sdd{WI-13996}
*/
#define SWAP(x, y, T) Swap(x, y, T)

/**
* This macro purely exists for backwards compatibility, use Is_True() directly.
* \ingroup boolean
* \sdd{WI-13837}
*/
#define IS_TRUE(a) (Is_True(a))

/**
* This macro purely exists for backwards compatibility, use Is_False() directly.
* \ingroup boolean
* \sdd{WI-13836}
*/
#define IS_FALSE(a) (Is_False(a))


/**
* Macro to get the max value of two numbers
* There may be other macros like MAX or max already defined in the build environment.
* The coding guidelines want the capitalization like this macro is defined. Therefore
* no other MAX or max macros are provided by the MathLibrary. If these exist they
* should not be used.
* \ingroup math
* \sdd{WI-13986}
*/
#ifdef MAX
    #ifndef Max
        #define Max(a,b) (MAX(a,b))
    #endif
    #ifndef __cplusplus
       #ifndef max
           #define max(a,b) (MAX(a,b))
       #endif
    #endif
#else
    #ifdef Max
        #define MAX(a,b) (Max(a,b))
        #ifndef __cplusplus
           #ifndef max
               #define max(a,b) (Max(a,b))
           #endif
        #endif
    #else
        #ifdef max
            #define MAX(a,b) (max(a,b))
            #define Max(a,b) (max(a,b))
        #else
            #define MAX(a, b)  (((a) > (b)) ? (a) : (b))
            #ifndef __cplusplus
               #define max(a,b) (MAX(a,b))
            #endif
            #define Max(a,b) (MAX(a,b))
        #endif
    #endif
#endif

/**
* Macro to get the min value of two numbers
* There may be other macros like MIN or min already defined in the build environment.
* The coding guidelines want the capitalization like this macro is defined. Therefore
* no other MIN or min macros are provided by the MathLibrary. If these exist they
* should not be used.
* \ingroup math
* \sdd{WI-13988}
*/
#ifdef MIN
    #ifndef Min
        #define Min(a,b) (MIN(a,b))
    #endif
    #ifndef __cplusplus
       #ifndef min
           #define min(a,b) (MIN(a,b))
       #endif
    #endif
#else
    #ifdef Min
        #define MIN(a,b) (Min(a,b))
        #ifndef __cplusplus
           #ifndef min
               #define min(a,b) (Min(a,b))
           #endif
        #endif
    #else
        #ifdef min
            #define MIN(a,b) (min(a,b))
            #define Min(a,b) (min(a,b))
        #else
            #define MIN(a, b)  (((a) < (b)) ? (a) : (b))
            #ifndef __cplusplus
               #define min(a,b) (MIN(a,b))
            #endif
            #define Min(a,b) (MIN(a,b))
        #endif
    #endif
#endif

#ifdef __cplusplus
}
#endif
#endif

