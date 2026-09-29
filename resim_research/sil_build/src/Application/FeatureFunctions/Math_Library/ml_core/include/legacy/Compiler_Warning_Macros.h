#ifndef COMPILER_WARNING_MACROS_H
#define COMPILER_WARNING_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/* Since this file defines a bunch of function-like macros the QAC check
"A function could probably be used instead of this function-like macro."
Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

#include "st_compiler_warning.h"

/**
* Used for backwards compatibility. Use Msvs_Disable_Warning instead.
* \ingroup compiler_warning
*/
#define MSVS_DISABLE_WARNING(warning) Msvs_Disable_Warning(warning)

/**
* Used for backwards compatibility. Use Msvs_Enable_Warning instead.
* \ingroup compiler_warning
*/
#define MSVS_ENABLE_WARNING(warning) Msvs_Enable_Warning(warning)

/**
* Used for backwards compatibility. Use As_Compiler_Warning instead.
* \ingroup compiler_warning
*/
#define AS_COMPILER_WARNING(desc) (As_Compiler_Warning(desc))

#ifdef __cplusplus
}
#endif
#endif 
