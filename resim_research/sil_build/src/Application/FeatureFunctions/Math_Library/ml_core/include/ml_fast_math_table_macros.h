#ifndef ML_FAST_MATH_TABLE_MACROS_H
#define ML_FAST_MATH_TABLE_MACROS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "ml_math.h"


   /**
   * MathLibrary is configured to use tables for fast math that are computed at runtime
   * \sa ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE
   * \sdd{WI-14671}
   * \sdd{WI-14673}
   */
#define ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE (2)

/**
 * MathLibrary is configured to use tables for fast math that are computed at compile time
 * \sa ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE
 * \sdd{WI-14670}
 * \sdd{WI-14674}
 */
#define ML_MATH_LIBRARY_FAST_MATH_USE_TABLE (1)
/**
 * MathLibrary is configured NOT to use tables for fast math
 * \sa ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE
 */
#define ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION (0)

/* Maintain backwards compatibility:
 * - If SHARED_TOOLBOX_FAST_MATH_EXP_TABLE is defined issue a warning and use definition
 * - If SHARED_TOOLBOX_FAST_MATH_EXP_TABLE and ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE are defined break the build
 */
#ifdef SHARED_TOOLBOX_FAST_MATH_EXP_TABLE
 /* PRQA S 1008,3115 EOF */
#warning SHARED_TOOLBOX_FAST_MATH_EXP_TABLE is deprecated, use ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE instead
#define ML_MATH_LIBRARY_FAST_MATH_BACKWARD
#ifdef ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE
#warning SHARED_TOOLBOX_FAST_MATH_EXP_TABLE is deprecated and can not be used if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE is defined
#else
#define ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE SHARED_TOOLBOX_FAST_MATH_EXP_TABLE
#endif
#endif

/* Maintain backwards compatibility:
 * - If SHARED_TOOLBOX_FAST_MATH_TRIG_TABLE is defined issue a warning and use definition
 * - If SHARED_TOOLBOX_FAST_MATH_TRIG_TABLE and ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE are defined break the build
 */
#ifdef SHARED_TOOLBOX_FAST_MATH_TRIG_TABLE
#warning SHARED_TOOLBOX_FAST_MATH_TRIG_TABLE is deprecated, use ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE instead
#define ML_MATH_LIBRARY_FAST_MATH_BACKWARD
#ifdef ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE
#warning SHARED_TOOLBOX_FAST_MATH_TRIG_TABLE is deprecated and can not be used if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE is defined
#else
#define ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE SHARED_TOOLBOX_FAST_MATH_TRIG_TABLE
#endif
#endif

#ifdef ML_MATH_LIBRARY_FAST_MATH_BACKWARD

/** Backwards compatibility */
#define SHARED_TOOLBOX_FAST_MATH_USE_RUNTIME_TABLE ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE

/** Backwards compatibility */
#define SHARED_TOOLBOX_FAST_MATH_USE_TABLE ML_MATH_LIBRARY_FAST_MATH_USE_TABLE

/** Backwards compatibility */
#define SHARED_TOOLBOX_FAST_MATH_USE_FUNCTION ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION

#endif

/**
 * To safe memory it is possible to choose if a table for exp results shall be used.
 * There is no default for this setting, the macro must be set.
 * If ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE is set to
 * - ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION: the expf() will be called from math.h
 * - ML_MATH_LIBRARY_FAST_MATH_USE_TABLE: precomputed EXP tables will be used
 * - ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE: EXP tables computed when Compute_Exp_Table() is called will be used
 * \sdd{WI-14674}
 * \sdd{WI-14673}
 */
#ifndef ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE
#error Preprocessor macro ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE must be defined
#endif
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE!=ML_MATH_LIBRARY_FAST_MATH_USE_TABLE
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE!=ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE!=ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
#error Preprocessor macro ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE must be defined
#endif
#endif
#endif


 /**
 * To safe memory it is possible to choose if a table for trigonometric computations shall be used.
 * There is no default for this setting, the macro must be set.
 * If ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE is set to
 * - ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION: the trigonometric functions will be called from math.h
 * - ML_MATH_LIBRARY_FAST_MATH_USE_TABLE: precomputed trigonometric function tables will be used
 * - ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE: trigonometric function tables computed when Compute_Trig_Tables() is called will be used
 * \sdd{WI-14670}
 * \sdd{WI-14671}
 */
#ifndef ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE
#error Preprocessor macro ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE must be defined
#endif
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE!=ML_MATH_LIBRARY_FAST_MATH_USE_TABLE
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE!=ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE!=ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
#error Preprocessor macro ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE must be defined
#endif
#endif
#endif

#define CS_FULL_TABLE_SIZE (16384)
#define CS_FULL_TABLE_MASK (CS_FULL_TABLE_SIZE - 1)
#define CS_HALF_TABLE_MAX_INDEX (CS_FULL_TABLE_SIZE / 2)
#define CS_QUARTER_TABLE_MAX_INDEX (CS_FULL_TABLE_SIZE / 4)
#define CS_QUARTER_TABLE_MAX_INDEX_FLOAT (CS_FULL_TABLE_SIZE / 4.0f)
#define CS_QUARTER_TABLE_SIZE (CS_QUARTER_TABLE_MAX_INDEX + 1)
#define CS_INV_PREC ((2.f*CS_QUARTER_TABLE_MAX_INDEX_FLOAT)/PI)


#define ATAN_INV_PREC (8191)
#define ATAN_TABLE_SIZE (8192)
#define EXP_INV_PREC (100)
#define EXP_TABLE_SIZE (2900)
#define LOWEST_DOMAIN_FLOAT (-18.99f)
#define MAG_LOWEST_DOMAIN (1899)
#define HIGHEST_DOMAIN_FLOAT (10.00f)
#define HIGHEST_DOMAIN (1000)

#ifdef __cplusplus
}
#endif
#endif
