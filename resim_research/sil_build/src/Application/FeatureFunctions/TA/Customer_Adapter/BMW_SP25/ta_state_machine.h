#ifndef TA_STATE_MACHINE_H
#define TA_STATE_MACHINE_H


/**
 * @file ta_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for the BMW SP25 State Machine logic for TA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================
 * Includes
 *=========================================================================*/
#include "fbk_vehicle_data_t.h"
#include "ta_bmw_sp25_types.h"
#include "ta_input_t.h"

/**

 * @brief Getter function for ta states
 *
 * @return TA_FF_State_T*
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 * @verification{}
 */
#define TA_MPS_2_KPH (3.6f)

TA_FF_State_T *Ta_Get_State_Output_Ptr(void);

/**

 * @brief Ta State Machine Implementation
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 * @verification{}
 */
void Ta_Set_Current_Ta_Functional_State(const Ta_Input_T *p_ta_input,
                                        TA_FF_State_T *p_ta_current_state,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data);


#endif /* TA_STATE_MACHINE_H */
