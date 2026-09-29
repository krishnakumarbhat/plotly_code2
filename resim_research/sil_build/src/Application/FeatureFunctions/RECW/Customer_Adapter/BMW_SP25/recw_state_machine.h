#ifndef RECW_STATE_MACHINE_H
#define RECW_STATE_MACHINE_H

/**
 * @file recw_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW state machine header file shared by all customers.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "recw_bmw_sp25_types.h"
#include "recw_core_output_t.h"
#include "recw_input_t.h"
#include "recw_output_t.h"

/*===========================================================================*\
* Local Function Prototypess
\*===========================================================================*/

Recw_SM_State_T Recw_Get_State(void);

void Recw_Set_State(Recw_SM_State_T state);

void Recw_State_Machine_Pre_Run(const Recw_Input_T *p_recw_input);

void Recw_State_Machine_Post_Run(const Recw_Input_T *p_recw_input,
                                 const Recw_Core_Output_T *p_core_output,
                                 const Pa_Data_T *p_pa_data,
                                 Recw_Output_T *p_recw_output);

#endif
