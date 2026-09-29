#ifndef CED_STATE_MACHINE_H
#define CED_STATE_MACHINE_H

/**
 * @file ced_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for the BMW SP25 State Machine logic for CED.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ced_bmw_sp25_types.h"
#include "ced_core_calibration_t.h"
#include "ced_input_t.h"
#include "fbk_vehicle_data_t.h"


/**
 * @brief Getter function for ced states
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
CED_FF_STATE_T *Ced_Get_State_Output_Ptr(void);

/**
 * @brief Set Current State of CED.
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Ced_Set_Current_Ced_Functional_State(const Ced_Input_T *p_ced_input,
                                          const Ced_Core_Calibration_T *p_ced_cal,
                                          CED_FF_STATE_T *p_ced_current_state,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data);

#endif /* CED_STATE_MACHINE_H */
