#ifndef SCW_STATE_MACHINE_H
#define SCW_STATE_MACHINE_H

/**
 * @file scw_state_machine.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW SP25 state machine structure for SCW.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"
#include "scw_input_t.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef enum
{
   SCW_NORMAL_OPERATION_MODE              = (0U),
   SCW_POWER_UP_OR_DOWN                   = (1U),
   SCW_SENSOR_NOT_CALIBRATED              = (2U),
   SCW_SENSOR_BLOCKED                     = (3U),
   SCW_SENSOR_MISALIGNED                  = (4U),
   SCW_BAD_SENSOR_ENVIRONMENTAL_CONDITION = (5U),
   SCW_REDUCED_FOV                        = (6U),
   SCW_INPUT_NOT_AVAILABLE                = (7U),
   SCW_INTERNAL_REASON                    = (8U),
   SCW_EXTERNAL_DISTORTION                = (9U),
   SCW_BEGINNING_BLOCKAGE                 = (10U),
   SCW_SELF_TEST                          = (11U),
   SCW_STANDBY                            = (12U),
   SCW_EXTERNAL_FAILURE                   = (13U),
   SCW_EVENT_EXT_DATA_INVALID_OR_TIMEOUT  = (14U)
} Scw_Extended_Qualifier_T;

typedef enum
{
   SCW_STATE_NOT_AVAILABLE = (0U),
   SCW_STATE_INACTIVE      = (1U),
   SCW_STATE_ACTIVE        = (2U),
   SCW_STATE_ERROR         = (3U)
} SCW_States_T;

typedef struct
{
   boolean_T f_scw_enable_check;
   boolean_T f_min_vel_upper_limit_check;
   boolean_T f_max_vel_lower_limit_check;
   boolean_T f_min_vel_lower_limit_check;
   boolean_T f_max_vel_upper_limit_check;
   boolean_T f_move_front_check;
   boolean_T f_drive_mode_check;
   boolean_T f_scw_test_mode_check;
   boolean_T f_scw_end_of_line_check;
   boolean_T f_function_scw_error_check;
} Scw_State_Flags_T;

SCW_States_T *Scw_Get_State_Output_Ptr(void);

/**
 * @brief State Machine Implementation for SCW
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Scw_State_Machine(const Scw_State_Flags_T *p_scw_state_flags, const boolean_T *p_scw_error, SCW_States_T *p_scw_cur_state);

/**
 * @brief SCW State Machine Flags Update Implementation.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
void Scw_Update_Flags(const Scw_Input_T *p_scw_input, Scw_State_Flags_T *p_scw_state_flags, const Fbk_Vehicle_Data_T *p_vehicle_data);

#endif /* SCW_STATE_MACHINE_H */
