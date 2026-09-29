#ifndef PA_ENV_IN_H
#define PA_ENV_IN_H

/**
 * @file pa_env_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA guardrail macros for the generic interface
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "pa_context.h"

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns if the guardrail module is active for the specified side
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Active_Flag(p_context, side) ((p_context)->p_data->guardrail_data[(uint8_t) (side)].f_active)

/**
 * @brief Returns if the guardrail is present for the specified side
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Present_Flag(p_context, side) ((p_context)->p_data->guardrail_data[(uint8_t) (side)].f_present)

/**
 * @brief Returns the status of the guardrail for the specified side
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Status(p_context, side) ((p_context)->p_data->guardrail_data[(uint8_t) (side)].status)

/**
 * @brief Returns the lateral position of the guardrail for the specified side
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Lateral_Position(p_context, side) ((p_context)->p_data->guardrail_data[(uint8_t) (side)].lat_pos)

/**
 * @brief Returns the existence probability of the guardrail for the specified side
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Existence_Probability(p_context, side) \
   ((p_context)->p_data->guardrail_data[(uint8_t) (side)].existence_probability)

/**
 * @brief Returns the age of the guardrail for the specified side
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Age(p_context, side) ((p_context)->p_data->guardrail_data[(uint8_t) (side)].age)

#endif /* PA_ENV_IN_H */
