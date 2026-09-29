#ifndef FBK_MACROS_H
#define FBK_MACROS_H

/**
 * @file fbk_macros.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Useful macros within feature function scope.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ml_math.h"
#include <string.h>

/*===========================================================================*\
* Constant Macros
\*===========================================================================*/

/* Defines for boolean values */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_FALSE (0)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_TRUE (1)

/* Defines for zero values */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_ZERO_INT (0)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_ZERO_UINT (0u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_ZERO_F (0.0f)

/* Defines for one values */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_ONE_INT (1)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_ONE_UINT (1u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_ONE_F (1.0f)

/* Defines of side specific macros */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_NUMBER_OF_SIDES (2u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_SIDE_LEFT (0u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_SIDE_RIGHT (1u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_SIDE_FRONT (0u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_SIDE_REAR (1u)

/* The undefined macros is currently shared for left, right and front, rear. */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_SIDE_UNDEFINED (2u)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_INVALID_TIME (100.0f)

/* coverity[misra_c_2012_rule_2_5_violation] */
#define FBK_INVALID_DISTANCE (1000.0f)

/*===========================================================================*\
* Function-like Macros
\*===========================================================================*/

/**
 * @brief Returns if input value is true.
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Is_True(x) (FBK_FALSE != (x))

/**
 * @brief Returns if input value is false.
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Is_False(x) (FBK_FALSE == (x))

/**
 * @brief Returns absolute value of input value of given type.
 *
 * @SRS{SF-283}
 * @SAE{SF-2552}
 * @SDD{n/a}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Abs(x, T) (((x) < (T) 0) ? -(x) : (x))

/**
 * @brief Returns absolute value of float input value.
 *
 * @SRS{SF-283}
 * @SAE{SF-2552}
 * @SDD{SF-4125}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Abs_F(x) (Fbk_Abs(x, float32_T))

/**
 * @brief Returns true if input value is NaN.
 *
 * @SRS{SF-282}
 * @SAE{SF-2552}
 * @SDD{SF-4126}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Is_Nan(x) ((x) != (x))

/**
 * @brief Returns true if input values are considered equal.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Equal_F(x, y) (THRESHOLD_IS_ZERO >= Fbk_Abs_F(x - y))

/**
 * @brief Returns true if input values are not considered equal.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Not_Equal_F(x, y) (Fbk_Is_False(Fbk_Equal_F(x, y)))

/**
 * @brief Returns minimum of the input values.
 *
 * @SRS{SF-281}
 * @SAE{SF-2552}
 * @SDD{SF-4123}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Min(x, y) (((x) < (y)) ? (x) : (y))

/**
 * @brief Returns maximum of the input values.
 *
 * @SRS{SF-280}
 * @SAE{SF-2552}
 * @SDD{SF-4124}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Max(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @brief Returns input value forced into given range.
 *
 * @SRS{SF-279}
 * @SAE{SF-2552}
 * @SDD{SF-4117}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Clamp(x, lower, upper) (Fbk_Max(lower, Fbk_Min(upper, x)))

/**
 * @brief Returns input value divided by two.
 *
 * @SRS{SF-285}
 * @SAE{SF-2552}
 * @SDD{SF-4119}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Half(x) ((x) *0.5f)

/**
 * @brief Returns -1 for negative, 0 for zero and 1 for positive inputs.
 *
 * @SRS{SF-284}
 * @SAE{SF-2552}
 * @SDD{SF-4156}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Sign(x) (((x) < FBK_ZERO_F) ? -1 : (((x) > FBK_ZERO_F) ? 1 : 0))

/**
 * @brief Returns unsigned integer equivalent to input boolean value.
 *
 * @SRS{SF-286}
 * @SAE{SF-2552}
 * @SDD{SF-4114}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Convert_Bool_To_Uint(x) ((x) ? (1u) : (0u))

/**
 * @brief Convert angle from degrees to radians.
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Deg_To_Rad(deg) ((deg) *PI / 180.0f)

/**
 * @brief Convert angle from radians to degrees.
 *
 * @SRS{}
 * @SAE{SF-2552}
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Fbk_Rad_To_Deg(rad) ((rad) *180.0f / PI)

#endif /* FBK_MACROS_H */
