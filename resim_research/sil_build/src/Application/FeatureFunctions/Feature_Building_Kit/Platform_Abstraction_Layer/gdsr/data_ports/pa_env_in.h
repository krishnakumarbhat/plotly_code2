#ifndef PA_ENV_IN_H
#define PA_ENV_IN_H

/**
 * @file pa_env_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA guardrail macros for the GDSR tracker
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "fbk_macros.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns if the guardrail module is active for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Active_Flag(p_context, side) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->guardrails[(uint8_t) (side)].f_active))

/**
 * @brief Returns if the guardrail is present for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Present_Flag(p_context, side) \
   (boolean_T)(Fbk_Is_True((p_context)->p_tracker_output->guardrails[(uint8_t) (side)].f_guardrail_present))

/**
 * @brief Returns the status of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Status(p_context, side) \
   (Pa_Gdsr_Map_Object_Status((p_context)->p_tracker_output->guardrails[(uint8_t) (side)].status))

/**
 * @brief Returns the lateral position of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Lateral_Position(p_context, side) \
   ((float32_T) (p_context)->p_tracker_output->guardrails[(uint8_t) (side)].lateral_position)

/**
 * @brief Returns the existence probability of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Existence_Probability(p_context, side) \
   ((float32_T) (p_context)->p_tracker_output->guardrails[(uint8_t) (side)].existence_probability)

/**
 * @brief Returns the age of the guardrail for the specified side
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Get_Guardrail_Age(p_context, side) ((uint8_t) (p_context)->p_tracker_output->guardrails[(uint8_t) (side)].age)

#endif /* PA_ENV_IN_H */
