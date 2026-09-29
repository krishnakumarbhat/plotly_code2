/**
 * @file esa_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 pre run logic for ESA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_pre_run.h"
#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Esa_Init_Input(Esa_Input_T *p_esa_input)
{
   /* Assert */
   assert(NULL != p_esa_input);

   p_esa_input->f_esa_enabled = FBK_TRUE;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_esa_instance" points to a non-constant type.] */
void Esa_Pre_Run_Init(Esa_Instance_T *p_esa_instance)
{
   /* Assert */
   assert(NULL != p_esa_instance);
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Esa_Pre_Run(Esa_Instance_T *p_esa_instance, const Esa_Input_T *p_esa_input, const Fbk_Output_T *p_fbk_output)
{
   assert(NULL != p_esa_instance);
   assert(NULL != p_esa_input);
   assert(NULL != p_fbk_output);

   p_esa_instance->core_input.p_pa_data = p_fbk_output->p_pa_data;

   if (Fbk_Is_True(p_esa_instance->calibration.k_esa_f_enable_via_cal))
   {
      p_esa_instance->core_input.f_esa_enabled = p_esa_instance->calibration.k_esa_f_enable;
   }
   else
   {
      p_esa_instance->core_input.f_esa_enabled = p_esa_input->f_esa_enabled;
   }
}
