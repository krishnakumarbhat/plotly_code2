#ifndef SHARED_TOOLBOX_VERSION_DATE_H
#define SHARED_TOOLBOX_VERSION_DATE_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "shared_toolbox_version.h"
#include "VERSION_NUMBER_T.h"

/* PRQA S 3453 ++*/ /* macro is used to steer macros => impossible to use a function */
#define SHARED_TOOLBOX_VERSION_INSUFFICIENT(REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION) (Ml_Math_Library_Version_Insufficient(REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION))

#define SHARED_TOOLBOX_VERSION_EQUALS(REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION) (Ml_Math_Library_Version_Equals(REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION))

/* PRQA S 3453 --*/ /* macro is used to steer macros => impossible to use a function */

#ifdef __cplusplus
}
#endif
#endif
