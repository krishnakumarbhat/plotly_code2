/**
 * @file ced_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 pre run logic for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_pre_run.h"
#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "ced_core_calibration_t.h"
#include "ced_core_input_t.h"
#include "ced_instance.h"
#include "ced_state_machine.h"
#include "fbk_macros.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"

#include <assert.h>

/*============================================================================*\
 * GLOBAL STATIC VARIABLES
\*============================================================================*/

static CED_FF_STATE_T Ced_Current_State = CED_STATE_NOTAVAILABLE;

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/**
 * @brief Getter function for ced states
 *
 */
CED_FF_STATE_T *Ced_Get_State_Output_Ptr(void)
{
   return &Ced_Current_State;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ced_Init_Input(Ced_Input_T *p_ced_input)
{
   assert(NULL != p_ced_input);

   p_ced_input->f_ced_enable     = FBK_TRUE;
   p_ced_input->f_ced_front_mode = FBK_TRUE;
   p_ced_input->f_ced_rear_mode  = FBK_TRUE;

   Ced_Init_Coding_Parameters(&p_ced_input->ced_coding_parameters);
   Ced_Init_Boardnet_Signals(&p_ced_input->bmw_boardnet_signals);
   Ced_Init_Input_Bus_Signals(&p_ced_input->ced_input_bus_signals);
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
void Ced_Pre_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Pre_Run(Ced_Instance_T *p_ced_instance ,
                 const Ced_Input_T *p_ced_input ,
                 const Pt_Output_T *p_pt_output ,
                 const Fbk_Output_T *p_fbk_output                  )
/* clang-format on */
{
   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_input);
   assert(NULL != p_pt_output);
   assert(NULL != p_fbk_output);

   if (((Ced_Current_State == CED_STATE_ACTIVE) || (Ced_Current_State == CED_STATE_DEGRADED))
       && (Fbk_Is_True(p_ced_input->f_ced_enable)
           && (Fbk_Abs_F(p_fbk_output->p_pa_data->vehicle_data.host_speed) <= p_ced_instance->calibration.k_ced_ego_abs_speed_max)))
   {
      p_ced_instance->core_input.f_ced_enable = FBK_TRUE;
   }
   else
   {
      p_ced_instance->core_input.f_ced_enable = FBK_FALSE;
   }
   p_ced_instance->core_input.f_ced_front_mode = p_ced_input->f_ced_front_mode;
   p_ced_instance->core_input.f_ced_rear_mode  = p_ced_input->f_ced_rear_mode;
   p_ced_instance->core_input.p_pt_output      = p_pt_output;
   p_ced_instance->core_input.p_pa_data        = p_fbk_output->p_pa_data;

   Ced_Set_Current_Ced_Functional_State(p_ced_input, &(p_ced_instance->calibration), &Ced_Current_State,
                                        &(p_ced_instance->core_input.p_pa_data->vehicle_data));
}
