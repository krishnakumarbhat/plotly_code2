/**
 * @file recw_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 pre run logic for RECW.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_pre_run.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "recw_bmw_sp25_types.h"
#include "recw_core_input_t.h"
#include "recw_input_t.h"
#include "recw_state_machine.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Recw_Init_Input(Recw_Input_T *p_recw_input /**< RECW Input */)
{
   assert(NULL != p_recw_input);

   p_recw_input->accelerator_pedal_gradient = FBK_ZERO_F;
   p_recw_input->vehicle_movement_status    = RECW_VEHICLE_STANDSTILL;
   p_recw_input->status_trailer             = RECW_NO_TRAILER_AVAILABLE;
   p_recw_input->status_roller_dynamometer  = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   p_recw_input->status_end_of_line         = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;
   p_recw_input->c_recw_enable              = RECW_STATE_DISABLED;
   p_recw_input->recw_type                  = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   p_recw_input->recw_error                 = RECW_NO_ERROR;
   p_recw_input->f_recw_enable_m_drive      = FBK_ZERO_UINT;

   Recw_Set_State(RECW_SM_NOT_AVAILABLE);
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_recw_instance" points to a non-constant type.] */
void Recw_Pre_Run_Init(Recw_Instance_T *p_recw_instance)
{
   assert(NULL != p_recw_instance);
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Recw_Pre_Run(Recw_Instance_T *p_recw_instance, const Recw_Input_T *p_recw_input, const Fbk_Output_T *p_fbk_output)
{
   assert(NULL != p_recw_instance);
   assert(NULL != p_recw_input);
   assert(NULL != p_fbk_output);

   p_recw_instance->core_input.p_pa_data = p_fbk_output->p_pa_data;
   Recw_State_Machine_Pre_Run(p_recw_input);

   if (RECW_SM_ACTIVE == Recw_Get_State())
   {
      p_recw_instance->core_input.f_enable_recw = (boolean_T) (Fbk_Is_True(p_recw_input->c_recw_enable));
   }
   else
   {
      p_recw_instance->core_input.f_enable_recw = FBK_FALSE;
   }
}
