#ifndef ML_VERSION_H
#define ML_VERSION_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_version_number_check.h"
#include "ml_version_number_t.h"

/**
 * \defgroup Ml_Math_Library_version Shared_Toolbox version
 * Macros and functions to check the current Shared_Toolbox release version towards required versions
 */

/**
 * The current Shared_Toolbox release version year
 * \ingroup Ml_Math_Library_version
 * \sdd{WI-14029}
 */
#define ML_MATH_LIBRARY_VERSION_YEAR         (22)

/**
 * The current Shared_Toolbox release version month
 * \ingroup Ml_Math_Library_version
 * \sdd{WI-14028}
 */
#define ML_MATH_LIBRARY_VERSION_MONTH (2)
/**
 * The current Shared_Toolbox release version day
 * \ingroup Ml_Math_Library_version
 * \sdd{WI-14032}
 */
#define ML_MATH_LIBRARY_VERSION_DAY (23)
/**
 * The current Shared_Toolbox release version iteration
 * \ingroup Ml_Math_Library_version
 * \sdd{WI-14031}
 */
#define ML_MATH_LIBRARY_VERSION_ITERATION    (0)

/* PRQA S 3453 ++*/ /* macro is used to steer macros => impossible to use a function */

/**
 * The current Shared_Toolbox release version integer
 * \ingroup Ml_Math_Library_version
 * \sdd{WI-14030}
 */
#define ML_MATH_LIBRARY_VERSION_INTEGER    Ml_Compute_Version_Integer_From_Name(ML_MATH_LIBRARY)

/**
 * Macro that checks if the current Shared_Toolbox version number is sufficient when compared to the given version number.
 * \ingroup Ml_Math_Library_version
 * \section Ml_Math_Library_insufficient_usage Usage
 * \code
 *  #if Ml_Math_Library_Version_Insufficient(20, 02, 27, 0)
 *  #error The Shared_Toolbox used is too old. Please update the Shared_Toolbox.
 *  #endif
 * \endcode
 * The above code snippet checks if the Shared_Toolbox version is at least 20.02.27.0 and breaks the build if this is not the case.
 * \sdd{WI-14026}
 */
#define Ml_Math_Library_Version_Insufficient(REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION)    (Ml_Version_Insufficient_Check((ML_MATH_LIBRARY_VERSION_INTEGER), REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION))

/**
 * Macro that checks if the current Shared_Toolbox version number is equal to the given version number.
 * \ingroup Ml_Math_Library_version
 * \section Ml_Math_Library_equals_usage Usage
 * \code
 *  #if Ml_Math_Library_Version_Equals(20, 02, 27, 0)
 *  #error The Shared_Toolbox does not match. Please change the Shared_Toolbox.
 *  #endif
 * \endcode
 * The above code snippet checks if the Shared_Toolbox version equal to 20.02.27.0 and breaks the build if this is not the case.
 * \sdd{WI-14027}
 */
#define Ml_Math_Library_Version_Equals(REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION)          (Ml_Version_Equals_Check((ML_MATH_LIBRARY_VERSION_INTEGER), REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY, REQUIRED_VERSION_ITERATION))

/* PRQA S 3453 --*/ /* macro is used to steer macros => impossible to use a function */

/**
 * Returns a version number structure containing the current Shared-Toolbox version number.
 * \ingroup Ml_Math_Library_version
 * \sdd{WI-14033}
 */
extern Version_Number_T Get_Ml_Math_Library_Version(void);

#ifdef __cplusplus
}
#endif
#endif

