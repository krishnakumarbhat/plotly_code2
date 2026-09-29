#ifndef ML_BOOL_H
#define ML_BOOL_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
 * \defgroup boolean Boolean
 * \section Motivation
 * A boolean type was introduced in C99. If an older C standard has to be used the boolean type is
 * not available. To maintain portability some macros are provided to ease usage of boolean values
 * if no boolean type is available.
 *
 * The boolean_T type provided by reuse.h maps to an uint8_t. 0 is defined to be FALSE, all other
 * values are TRUE.
 *
 * Therefor you should not compare a boolean_T variable directly to TRUE:
 * A boolean_T f_some_flag=2 is also true, but if you compare it to TRUE the result will be false.
 * The Is_True/Is_False macros solve this issue,
 *
 * \code
 * #define Is_True(x) (FALSE != (x))
 * \endcode
 * This is checking that the boolean_T is NOT equal to 0, since 0 is the only value that means false.
 * \code
 * #define Is_False(x) (FALSE == (x))
 * \endcode
 *
 * This will only be true for the only false value: 0
 *
 * Additionally less things will go wrong if you use boolean logic with the results of Is_True/Is_False
 * than if you use boolean logic with the boolean_T directly.
 * \code
 * boolean_T a = 2; // This is also TRUE
 * boolean_T b = 1; // TRUE
 * boolean_T c = a & b; // results to FALSE because & is bitwise and
 * boolean_T d = Is_True(a) & Is_True(b); // TRUE
 * boolean_T e = a && b; // TRUE, && is logical
 * boolean_T f = Is_True(a) && Is_True(b); // TRUE, && is logical
 * \endcode
 */

#ifndef TRUE
/**
* TRUE constant
* Do not directly compare a boolean_T variable to TRUE, use Is_True() instead.
* \ingroup boolean
* \sdd{WI-13835}
*/
#define TRUE 1
#endif

#ifndef FALSE
/**
* FALSE constant
* Do not directly compare a boolean_T variable to FALSE, use Is_False() instead.
* \ingroup boolean
* \sdd{WI-13834}
*/
#define FALSE 0
#endif

/* Since this file defines a bunch of function-like macros the QAC check
"A function could probably be used instead of this function-like macro."
Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* Safely check if integer x is TRUE
* \ingroup boolean
* \sdd{WI-13837}
*/
#define Is_True(x) (FALSE != (x))

/**
* Safely check if integer x is FALSE
* \ingroup boolean
* \sdd{WI-13836}
*/
#define Is_False(x) (FALSE == (x))

#ifdef __cplusplus
}
#endif
#endif
