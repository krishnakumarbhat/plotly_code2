/**
 * @file ced_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for CED.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced_pre_run.h"
#include "ced_core_input_t.h"
#include "ced_honda_instance.h"
#include "ced_instance.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>


/*===========================================================================*\
* Global Functions	Definition
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ced_Init_Input(Ced_Input_T *p_ced_input)
{
   assert(NULL != p_ced_input);

   p_ced_input->f_ced_enable                   = FBK_TRUE;
   p_ced_input->f_ced_front_mode               = FBK_FALSE;
   p_ced_input->f_ced_rear_mode                = FBK_TRUE;
   p_ced_input->threshold_timer_for_ced_enable = 4u;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
void Ced_Pre_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
   Ced_Get_Ced_Honda_Instance()->timer_for_ced_enable = FBK_ZERO_UINT;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Pre_Run(Ced_Instance_T *p_ced_instance,
                 const Ced_Input_T *p_ced_input,
                 const Pt_Output_T *p_pt_output,
                 const Fbk_Output_T *p_fbk_output)
{

   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_input);
   assert(NULL != p_pt_output);
   assert(NULL != p_fbk_output);


   p_ced_instance->core_input.f_ced_front_mode = p_ced_input->f_ced_front_mode;
   p_ced_instance->core_input.f_ced_rear_mode  = p_ced_input->f_ced_rear_mode;
   p_ced_instance->core_input.p_pt_output      = p_pt_output;
   p_ced_instance->core_input.p_pa_data        = p_fbk_output->p_pa_data;

   if (Fbk_Is_False(p_ced_input->f_ced_enable))
   {

      p_ced_instance->core_input.f_ced_enable = FBK_FALSE;
   }
   else
   {
      p_ced_instance->core_input.f_ced_enable = FBK_TRUE;
   }
}
