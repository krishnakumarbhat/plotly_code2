/**
 * @file ltb_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ltb_pre_run.h"
#include "fbk_macros.h"
#include "ltb_core_input_t.h"
#include "pa_reuse.h"
#include <assert.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ltb_Init_Input(Ltb_Input_T *p_ltb_input)
{
   p_ltb_input->f_ltb_enable = FBK_TRUE;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ltb_Pre_Run(Ltb_Instance_T *p_ltb_instance, const Ltb_Input_T *p_ltb_input, const Fbk_Output_T *p_fbk_output)
{
   assert(NULL != p_ltb_instance);
   assert(NULL != p_ltb_input);
   assert(NULL != p_fbk_output);

   p_ltb_instance->core_input.p_pa_data    = p_fbk_output->p_pa_data;
   p_ltb_instance->core_input.f_ltb_enable = p_ltb_input->f_ltb_enable;
}
