#ifndef SHARED_TOOLBOX_VERSION_CHECK_H
#define SHARED_TOOLBOX_VERSION_CHECK_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "st_version_number_check.h"

#define SHARED_TOOLBOX_COMPUTE_VERSION_INTEGER_FROM_DATE(YEAR, MONTH, DAY, ITERATION) (Ml_Compute_Version_Integer_From_Name(YEAR, MONTH, DAY, ITERATION))

#define SHARED_TOOLBOX_COMPUTE_VERSION_INTEGER_FROM_NAME(prefix) (Ml_Compute_Version_Integer_From_Name(prefix))

#define SHARED_TOOLBOX_VERSION_INSUFFICIENT_CHECK(                             \
      module_version_integer,                                                  \
      REQUIRED_VERSION_YEAR,                                                   \
      REQUIRED_VERSION_MONTH,                                                  \
      REQUIRED_VERSION_DAY,                                                    \
      REQUIRED_VERSION_ITERATION)                                              \
      (Ml_Version_Insufficient_Check(                                          \
      module_version_integer,                                                  \
      REQUIRED_VERSION_YEAR,                                                   \
      REQUIRED_VERSION_MONTH,                                                  \
      REQUIRED_VERSION_DAY,                                                    \
      REQUIRED_VERSION_ITERATION))

#define SHARED_TOOLBOX_VERSION_EQUALS_CHECK(                                          \
      module_version_integer,                                                         \
      REQUIRED_VERSION_YEAR,                                                          \
      REQUIRED_VERSION_MONTH,                                                         \
      REQUIRED_VERSION_DAY,                                                           \
      REQUIRED_VERSION_ITERATION)                                                     \
      (Ml_Version_Equals_Check(                                                       \
      module_version_integer,                                                         \
      REQUIRED_VERSION_YEAR,                                                          \
      REQUIRED_VERSION_MONTH,                                                         \
      REQUIRED_VERSION_DAY,                                                           \
      REQUIRED_VERSION_ITERATION))

#ifdef __cplusplus
}
#endif
#endif

