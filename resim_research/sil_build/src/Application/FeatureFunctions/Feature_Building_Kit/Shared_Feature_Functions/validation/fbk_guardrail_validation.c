/**
 * @file fbk_guardrail_validation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file with functions for guardrail data validation.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_guardrail_validation.h"
#include "pa_env_in.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Fill_Guardrail_Information(Fbk_Guardrail_Data_T *p_fbk_guardrail_data, const Pa_Context_T *p_context, const uint8_t side)
{
   /* Asserts */
   assert(NULL != p_fbk_guardrail_data);
   assert(NULL != p_context);

   /* Fill vehicle data from PA */
   p_fbk_guardrail_data->f_active              = Pa_Get_Guardrail_Active_Flag(p_context, side);
   p_fbk_guardrail_data->f_present             = Pa_Get_Guardrail_Present_Flag(p_context, side);
   p_fbk_guardrail_data->status                = Pa_Get_Guardrail_Status(p_context, side);
   p_fbk_guardrail_data->lat_pos               = Pa_Get_Guardrail_Lateral_Position(p_context, side);
   p_fbk_guardrail_data->existence_probability = Pa_Get_Guardrail_Existence_Probability(p_context, side);
   p_fbk_guardrail_data->age                   = Pa_Get_Guardrail_Age(p_context, side);
}
