#ifndef LCDA_STATE_MACHINE_H
#define LCDA_STATE_MACHINE_H


/**
 * @file lcda_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for the BMW SP25 State Machine logic for LCDA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================
 * Includes
 *=========================================================================*/
#include "fbk_vehicle_data_t.h"
#include "lcda_bmw_sp25_types.h"
#include "lcda_input_t.h"
/**
 * Defines
 */
#define LCDA_MPS_2_KPH (3.6f)

/**
 * @brief Getter function for lcda states
 *
 */
LCDA_FF_State_T *Lcda_Get_State_Output_Ptr(void);

/**
 * @brief State Machine for LCDA.
 *
 * @return void
 *
 * @verification{}
 */
void Lcda_Set_Current_Lcda_Functional_State(const Lcda_Input_T *p_lcda_input,
                                            LCDA_FF_State_T *p_lcda_current_state,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data);

#endif /* LCDA_STATE_MACHINE_H */
