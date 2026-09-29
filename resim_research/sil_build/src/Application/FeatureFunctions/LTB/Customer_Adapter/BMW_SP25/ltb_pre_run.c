/**
 * @file ltb_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 pre run logic for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_pre_run.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_bmw_sp25_types.h"
#include "ltb_core_input_t.h"
#include "ltb_state_machine.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ltb_Init_Input(Ltb_Input_T *p_ltb_input)
{
   p_ltb_input->f_ltb_enable = FBK_TRUE;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ltb_Pre_Run(Ltb_Instance_T *p_ltb_instance, const Ltb_Input_T *p_ltb_input, const Fbk_Output_T *p_fbk_output)
{
   Ltb_State_Flags_T ltb_state_flags;
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   const Ltb_State_T *p_ltb_current_state = Ltb_Get_Current_State();

   assert(NULL != p_ltb_instance);
   assert(NULL != p_ltb_input);
   assert(NULL != p_fbk_output);

   p_ltb_instance->core_input.p_pa_data = p_fbk_output->p_pa_data;
   p_vehicle_data                       = &p_fbk_output->p_pa_data->vehicle_data;

   Ltb_Update_Flags(p_ltb_input, &p_ltb_instance->calibration, p_vehicle_data, &ltb_state_flags);
   Ltb_State_Machine(&ltb_state_flags);

   /*Enable the Core only when the State is ACTIVE*/
   if (LTB_STATE_ACTIVE == *p_ltb_current_state)
   {
      p_ltb_instance->core_input.f_ltb_enable = FBK_TRUE;
   }
}
