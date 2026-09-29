/**
 * @file ced_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 pre run logic for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "ced_pre_run.h"
#include "ced_core_calibration_t.h"
#include "ced_core_input_t.h"
#include "ced_instance.h"
#include "fbk_macros.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ced_Init_Input(Ced_Input_T *p_ced_input)
{
   assert(NULL != p_ced_input);

   p_ced_input->f_ced_enable     = FBK_TRUE;
   p_ced_input->f_ced_front_mode = FBK_TRUE;
   p_ced_input->f_ced_rear_mode  = FBK_TRUE;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
void Ced_Pre_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
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

   if (Fbk_Is_False(p_ced_input->f_ced_enable)
       || (Fbk_Abs_F(p_fbk_output->p_pa_data->vehicle_data.host_speed) > p_ced_instance->calibration.k_ced_ego_abs_speed_max))
   {
      p_ced_instance->core_input.f_ced_enable = FBK_FALSE;
   }
   else
   {
      p_ced_instance->core_input.f_ced_enable = FBK_TRUE;
   }

   p_ced_instance->core_input.f_ced_front_mode = p_ced_input->f_ced_front_mode;
   p_ced_instance->core_input.f_ced_rear_mode  = p_ced_input->f_ced_rear_mode;
   p_ced_instance->core_input.p_pa_data        = p_fbk_output->p_pa_data;
   p_ced_instance->core_input.p_pt_output      = p_pt_output;
}
