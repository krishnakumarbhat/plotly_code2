/**
 * @file scw_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SP25 state machine logic for SCW.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "scw_state_machine.h"
#include "fbk_macros.h"
#include "scw_input_t.h"

#define SCW_MPS_2_KPH (3.6f)

/*====================================================================================*\
 *
 Function Definitions
 *
 \*===================================================================================*/
/**
 * @brief State Machine Check for Min Vel Upper Limit
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Min_Vel_Upper_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief State Machine Check for Max Vel Lower Limit
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Max_Vel_Lower_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief State Machine Check for Min Vel Lower Limit
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Min_Vel_Lower_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief State Machine Check for Max Vel Upper Limit
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Max_Vel_Upper_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief State Machine Check for Ego movement direction
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Ego_Move_Front_Check(const Scw_Input_T *p_scw_input);

/**
 * @brief State Machine Check for Pwf values
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Gear_In_Drive_Check(const Scw_Input_T *p_scw_input);

/**
 * @brief State Machine Check for Test Mode Activation (Dynamometer Status)
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Scw_Dynamometer_Status_Check(const Scw_Input_T *p_scw_input);

/**
 * @brief State Machine Transitions from Inactive.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Scw_Transitions_from_Inactive(const Scw_State_Flags_T *p_scw_state_flags,
                                          SCW_States_T *p_scw_cur_state,
                                          const boolean_T *p_scw_error);

/**
 * @brief State Machine Transitions from Active
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Scw_Transitions_from_Active(const Scw_State_Flags_T *p_scw_state_flags,
                                        SCW_States_T *p_scw_cur_state,
                                        const boolean_T *p_scw_error);

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

void Scw_State_Machine(const Scw_State_Flags_T *p_scw_state_flags, const boolean_T *p_scw_error, SCW_States_T *p_scw_cur_state)
{
   if ((SCW_STATE_NOT_AVAILABLE == *p_scw_cur_state) && Fbk_Is_True(p_scw_state_flags->f_scw_enable_check))
   {
      *p_scw_cur_state = SCW_STATE_INACTIVE;
   }
   if ((SCW_STATE_INACTIVE == *p_scw_cur_state))
   {
      Scw_Transitions_from_Inactive(p_scw_state_flags, p_scw_cur_state, p_scw_error);
   }
   if ((SCW_STATE_ACTIVE == *p_scw_cur_state))
   {
      Scw_Transitions_from_Active(p_scw_state_flags, p_scw_cur_state, p_scw_error);
   }
   if ((SCW_STATE_ERROR == *p_scw_cur_state) && Fbk_Is_False(*p_scw_error))
   {
      *p_scw_cur_state = SCW_STATE_INACTIVE;
   }
}

static void Scw_Transitions_from_Inactive(const Scw_State_Flags_T *p_scw_state_flags,
                                          SCW_States_T *p_scw_cur_state,
                                          const boolean_T *p_scw_error)
{
   /*Inactive to Active*/
   if (Fbk_Is_True(p_scw_state_flags->f_min_vel_upper_limit_check) && Fbk_Is_True(p_scw_state_flags->f_max_vel_lower_limit_check)
       && Fbk_Is_True(p_scw_state_flags->f_move_front_check) && Fbk_Is_True(p_scw_state_flags->f_drive_mode_check)
       && Fbk_Is_False(p_scw_state_flags->f_scw_test_mode_check))
   {
      *p_scw_cur_state = SCW_STATE_ACTIVE;
   }
   /*Inactive to Not Available*/
   if (Fbk_Is_False(p_scw_state_flags->f_scw_enable_check))
   {
      *p_scw_cur_state = SCW_STATE_NOT_AVAILABLE;
   }
   /*Inactive to Error*/
   if (Fbk_Is_True(*p_scw_error))
   {
      *p_scw_cur_state = SCW_STATE_ERROR;
   }
}

static void Scw_Transitions_from_Active(const Scw_State_Flags_T *p_scw_state_flags,
                                        SCW_States_T *p_scw_cur_state,
                                        const boolean_T *p_scw_error)
{
   /*Active to Inactive*/
   if (Fbk_Is_True(p_scw_state_flags->f_min_vel_lower_limit_check) || Fbk_Is_True(p_scw_state_flags->f_max_vel_upper_limit_check)
       || Fbk_Is_False(p_scw_state_flags->f_move_front_check) || Fbk_Is_False(p_scw_state_flags->f_drive_mode_check)
       || Fbk_Is_True(p_scw_state_flags->f_scw_test_mode_check))
   {
      *p_scw_cur_state = SCW_STATE_INACTIVE;
   }
   /*Active to Error*/
   if (Fbk_Is_True(*p_scw_error))
   {
      *p_scw_cur_state = SCW_STATE_ERROR;
   }
}

void Scw_Update_Flags(const Scw_Input_T *p_scw_input, Scw_State_Flags_T *p_scw_state_flags, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /*SCW enable Check flag */
   if (Fbk_Is_True(p_scw_input->f_scw_enable))
   {
      p_scw_state_flags->f_scw_enable_check = FBK_TRUE;
   }
   else
   {
      p_scw_state_flags->f_scw_enable_check = FBK_FALSE;
   }
   /*SCW enable Check flag */
   p_scw_state_flags->f_min_vel_upper_limit_check = Scw_Min_Vel_Upper_Limit_Check(p_scw_input, p_vehicle_data);
   p_scw_state_flags->f_max_vel_lower_limit_check = Scw_Max_Vel_Lower_Limit_Check(p_scw_input, p_vehicle_data);
   p_scw_state_flags->f_min_vel_lower_limit_check = Scw_Min_Vel_Lower_Limit_Check(p_scw_input, p_vehicle_data);
   p_scw_state_flags->f_max_vel_upper_limit_check = Scw_Max_Vel_Upper_Limit_Check(p_scw_input, p_vehicle_data);
   p_scw_state_flags->f_move_front_check          = Scw_Ego_Move_Front_Check(p_scw_input);
   p_scw_state_flags->f_drive_mode_check          = Scw_Gear_In_Drive_Check(p_scw_input);
   p_scw_state_flags->f_scw_test_mode_check       = Scw_Dynamometer_Status_Check(p_scw_input);
}

static boolean_T Scw_Min_Vel_Upper_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T ret_val = FBK_FALSE;
   if ((Fbk_Abs_F(p_vehicle_data->host_speed * SCW_MPS_2_KPH) > p_scw_input->scw_coding_parameters.c_scw_min_vel_upper_limit))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}

static boolean_T Scw_Max_Vel_Lower_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T ret_val = FBK_FALSE;
   if ((Fbk_Abs_F(p_vehicle_data->host_speed * SCW_MPS_2_KPH) < p_scw_input->scw_coding_parameters.c_scw_max_vel_lower_limit))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}

static boolean_T Scw_Min_Vel_Lower_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T ret_val = FBK_FALSE;
   if ((Fbk_Abs_F(p_vehicle_data->host_speed * SCW_MPS_2_KPH) < p_scw_input->scw_coding_parameters.c_scw_min_vel_lower_limit))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}

static boolean_T Scw_Max_Vel_Upper_Limit_Check(const Scw_Input_T *p_scw_input, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T ret_val = FBK_FALSE;
   if ((Fbk_Abs_F(p_vehicle_data->host_speed * SCW_MPS_2_KPH) > p_scw_input->scw_coding_parameters.c_scw_max_vel_upper_limit))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}

static boolean_T Scw_Ego_Move_Front_Check(const Scw_Input_T *p_scw_input)
{
   boolean_T ret_val = FBK_FALSE;
   if ((SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD == p_scw_input->scw_vehicle_input.vehicle_driving_direction))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}

static boolean_T Scw_Gear_In_Drive_Check(const Scw_Input_T *p_scw_input) /*Change with the pwf state*/
{
   boolean_T ret_val = FBK_FALSE;
   if ((SCW_BMW_PWF_STATE_DRIVING == p_scw_input->scw_vehicle_input.vehicle_condition))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}

static boolean_T Scw_Dynamometer_Status_Check(const Scw_Input_T *p_scw_input)
{
   boolean_T ret_val = FBK_FALSE;
   if ((SCW_BMW_STATUS_ROLLER_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER == p_scw_input->scw_vehicle_input.vehicle_dynamometer_status)
       || (SCW_BMW_STATUS_ROLLER_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER == p_scw_input->scw_vehicle_input.vehicle_dynamometer_status)
       || (SCW_BMW_STATUS_ROLLER_DYNAMOMETER_TWO_AXLE_DYNAMOMETER == p_scw_input->scw_vehicle_input.vehicle_dynamometer_status)
       || (SCW_BMW_STATUS_END_OF_LINE_MODE_SET == p_scw_input->scw_vehicle_input.vehicle_end_of_line_status))
   {
      ret_val = FBK_TRUE;
   }
   return ret_val;
}
