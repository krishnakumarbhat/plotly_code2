#ifndef LTB_STATE_MACHINE_H
#define LTB_STATE_MACHINE_H

/**
 * @file ltb_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 state machine declarations
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "fbk_vehicle_data_t.h"
#include "ltb_bmw_sp25_types.h"
#include "ltb_core_calibration_t.h"
#include "ltb_input_t.h"
/**
 * @brief State Machine Flag Computation for LTB.
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Ltb_Update_Flags(const Ltb_Input_T *p_ltb_input,
                      const Ltb_Core_Calibration_T *p_ltb_cal,
                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                      Ltb_State_Flags_T *p_ltb_state_flags);

/**
 * @brief State Machine for LTB.
 *
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Ltb_State_Machine(const Ltb_State_Flags_T *p_ltb_state_flags);

Ltb_State_T *Ltb_Get_Current_State(void);
#endif /* LTB_STATE_MACHINE_H */
