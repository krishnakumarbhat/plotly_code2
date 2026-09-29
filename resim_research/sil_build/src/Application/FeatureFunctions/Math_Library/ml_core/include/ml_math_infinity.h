#ifndef ML_MATH_INFINITY_H
#define ML_MATH_INFINITY_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <math.h>

/**
* \brief If INFINITY is not available a huge value (\ref AS_TOOLBOX_INFINITY) is used and a compiler
* warning is issued.
*
* Not all platforms provide a definition of an infinity value. If the current platform does not
* provide an INFINITY constant INFINITY is set to \ref AS_TOOLBOX_INFINITY and a compiler warning is issued.
* This compiler warning is given because the user of INFINITY is not getting what he asked for.
* If your usage of INFINITY allows for a huge number to be used instead of infinity consider using
* \ref AS_TOOLBOX_INFINITY directly by including math_infinity_silent.h
*
* See \ref AS_TOOLBOX_INFINITY for its caveats.
* \ingroup math
 */
#ifndef INFINITY

#ifdef _MSC_VER
#include "ml_compiler_warning.h"
As_Compiler_Warning((INFINITY is not defined, setting 'INFINITY' to 'AS_TOOLBOX_INFINITY'))
#else
#warning INFINITY is not defined, setting 'INFINITY' to 'AS_TOOLBOX_INFINITY'
#endif

#include "ml_math_infinity_silent.h"
#define INFINITY (AS_TOOLBOX_INFINITY)

#endif

#ifdef __cplusplus
}
#endif
#endif
