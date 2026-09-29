#ifndef ML_VERSION_NUMBER_CHECK_H
#define ML_VERSION_NUMBER_CHECK_H
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

/* Using multiple ## in a macro definition is needed because multiple strings are concatenated*/
/* PRQA S 881 EOF */

/**
 * \defgroup versioning Versioning
 * \brief Check the available MathLibrary version.
 *
 * To use the version check include this h-file:
 * \code
 * #include "ml_version_number_check.h"
 * \endcode
 *
 * Then use these few lines to check for your minimum version number:
 * \code
 * #if Ml_Math_Library_Version_Insufficient(17, 05, 05, 0)
 * #error update shared toolbox
 * #endif
 * \endcode
 * The parameters are:
 *  - year, two digits
 *  - month, two digits
 *  - day, two digits
 *  - iteration, one digit
 *
 * The 'error' will cause the build to break during compile time and throw the error message "update MathLibrary"
 *
 ****************************************/

 /**
 * Macro used to shift the year in the integer representation of a version number to the correct decimal place
 * \ingroup versioning
 */
#define VERSION_FACTOR_YEAR         (100000)

 /**
 * Macro used to shift the month in the integer representation of a version number to the correct decimal place
 * \ingroup versioning
 */
#define VERSION_FACTOR_MONTH        (1000)

 /**
 * Macro used to shift the day in the integer representation of a version number to the correct decimal place
 * \ingroup versioning
 */
#define VERSION_FACTOR_DAY          (10)

 /**
 * Macro used to shift the iteration in the integer representation of a version number to the correct decimal place
 * \ingroup versioning
 */
#define VERSION_FACTOR_ITERATION    (1)

/**
 * Computes an integer representation of the given YEAR, MONTH, DAY, ITERATION
 * \ingroup versioning
 * \sdd{WI-14021}
 */
#define Ml_Compute_Version_Integer_From_Date(YEAR, MONTH, DAY, ITERATION) \
   (((YEAR) * VERSION_FACTOR_YEAR) +                                                    \
    ((MONTH) * VERSION_FACTOR_MONTH) +                                                  \
    ((DAY) * VERSION_FACTOR_DAY) +                                                      \
    ((ITERATION) * VERSION_FACTOR_ITERATION))

/**
* Computes an integer representation of the given YEAR, MONTH, DAY, ITERATION of a version number named prefix (e.g. prefix_YEAR)
 * \ingroup versioning
 * \sdd{WI-14025}
*/
#define Ml_Compute_Version_Integer_From_Name(prefix)                \
   Ml_Compute_Version_Integer_From_Date((prefix ## _VERSION_YEAR),  \
                                                    (prefix ## _VERSION_MONTH), \
                                                    (prefix ## _VERSION_DAY),   \
                                                    (prefix ## _VERSION_ITERATION))

/**
 * Evaluates to true if the current MathLibrary version number is at least the required REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY and REQUIRED_VERSION_ITERATION
  * \ingroup versioning
  * \sdd{WI-14024}
 */
#define Ml_Version_Insufficient_Check(                                         \
      module_version_integer,                                                  \
      REQUIRED_VERSION_YEAR,                                                   \
      REQUIRED_VERSION_MONTH,                                                  \
      REQUIRED_VERSION_DAY,                                                    \
      REQUIRED_VERSION_ITERATION)                                              \
   ((module_version_integer)                                                   \
    < Ml_Compute_Version_Integer_From_Date(REQUIRED_VERSION_YEAR,  \
                                                       REQUIRED_VERSION_MONTH, \
                                                       REQUIRED_VERSION_DAY,   \
                                                       REQUIRED_VERSION_ITERATION))

/**
* Evaluates to true if the current MathLibrary version number is equal to the required REQUIRED_VERSION_YEAR, REQUIRED_VERSION_MONTH, REQUIRED_VERSION_DAY and REQUIRED_VERSION_ITERATION
 * \ingroup versioning
 * \sdd{WI-14023}
*/
#define Ml_Version_Equals_Check(                                                      \
      module_version_integer,                                                         \
      REQUIRED_VERSION_YEAR,                                                          \
      REQUIRED_VERSION_MONTH,                                                         \
      REQUIRED_VERSION_DAY,                                                           \
      REQUIRED_VERSION_ITERATION)                                                     \
   ((module_version_integer)                                                          \
    == Ml_Compute_Version_Integer_From_Date((REQUIRED_VERSION_YEAR),      \
                                                        (REQUIRED_VERSION_MONTH),     \
                                                        (REQUIRED_VERSION_DAY),       \
                                                        (REQUIRED_VERSION_ITERATION)))

#ifdef __cplusplus
}
#endif
#endif

