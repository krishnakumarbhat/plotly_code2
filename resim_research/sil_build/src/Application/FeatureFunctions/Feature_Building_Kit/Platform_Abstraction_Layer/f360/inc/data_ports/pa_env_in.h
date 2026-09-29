#ifndef PA_ENV_IN_H
#define PA_ENV_IN_H

/**
 * @file pa_env_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA guardrail macros for the F360 tracker
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "fbk_macros.h"
#include "pa_context.h"
#include "pa_mock_functions.h"

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns if the guardrail module is active for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Pa_Get_Guardrail_Active_Flag(p_context, side) (Pa_Mock_Boolean(p_context, side, FBK_FALSE))

/**
 * @brief Returns if the guardrail is present for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Pa_Get_Guardrail_Present_Flag(p_context, side) (Pa_Mock_Boolean(p_context, side, FBK_FALSE))

/**
 * @brief Returns the status of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Pa_Get_Guardrail_Status(p_context, side) (Pa_Mock_Obj_Status(p_context, side, PA_OBJ_STATUS_INVALID))

/**
 * @brief Returns the lateral position of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Pa_Get_Guardrail_Lateral_Position(p_context, side) (Pa_Mock_Float(p_context, side, 0.0f))

/**
 * @brief Returns the existence probability of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Pa_Get_Guardrail_Existence_Probability(p_context, side) (Pa_Mock_Float(p_context, side, 0.0f))

/**
 * @brief Returns the age of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define Pa_Get_Guardrail_Age(p_context, side) (Pa_Mock_Uint(p_context, side, 0u))

#endif /* PA_ENV_IN_H */
