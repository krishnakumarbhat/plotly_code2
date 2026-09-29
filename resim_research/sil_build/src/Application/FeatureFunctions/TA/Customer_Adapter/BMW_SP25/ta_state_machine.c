/**
 * @file tap_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contapins the BMW SP2025 Stapte Machine logic for TAP.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
/*===========================================================================
 * Includes
 *=========================================================================*/
#include "ta_state_machine.h"
#include "fbk_macros.h"
#include "reuse.h"


/*============================================================================*\
 * GLOBAL  VARIABLES
\*============================================================================*/

/**
 * @brief Checks if Speed is less than lower limit of minimum velocity of ego vehicle.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ta_Is_Speed_Less_Than_Equal_To_Minimum_Velocity_Lower_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                       const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Checks if Speed is less than lower limit of minimum velocity of ego vehicle
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ta_Is_Speed_Greater_Than_Equal_To_Minimum_Velocity_Upper_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                          const Fbk_Vehicle_Data_T *p_vehicle_data);


/**
 * @brief Checks if Speed is less than lower limit of maximum velocity of ego vehicle.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ta_Is_Speed_Less_Than_Equal_To_Maximum_Velocity_Lower_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                       const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief Checks if Speed is more than upper limit of maximum velocity of ego vehicle
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ta_Is_Speed_Greater_Than_Equal_To_Maximum_Velocity_Upper_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                          const Fbk_Vehicle_Data_T *p_vehicle_data);


/**
 * @brief TA State Machine Transitions From Not-Available State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ta_State_Machine_Transitions_From_Not_Available_State(const Ta_Input_T *p_ta_input, TA_FF_State_T *p_ta_current_state);


/* @brief TA Stapte Machine Transitions From Inactive State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ta_State_Machine_Transitions_From_InActive_State(const Ta_Input_T *p_ta_input,
                                                             TA_FF_State_T *p_ta_current_state,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data);


/* @brief TA State Machine Transitions From Active State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ta_State_Machine_Transitions_From_Active_State(const Ta_Input_T *p_ta_input,
                                                           TA_FF_State_T *p_ta_current_state,
                                                           const Fbk_Vehicle_Data_T *p_vehicle_data);


/* @brief TA State Machine Transitions From Error State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ta_State_Machine_Transitions_From_Error_State(const Ta_Input_T *p_ta_input, TA_FF_State_T *p_ta_current_state);


static boolean_T Ta_Is_Speed_Less_Than_Equal_To_Maximum_Velocity_Lower_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                       const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const float32_T k_max_vel_lower_limit_ta = p_ta_input->ta_coding_parameters.c_ta_max_vel_lower_limit;
   boolean_T f_speed = (boolean_T) (Fbk_Abs_F(p_vehicle_data->host_speed * TA_MPS_2_KPH) <= k_max_vel_lower_limit_ta);
   return f_speed;
}

static boolean_T Ta_Is_Speed_Greater_Than_Equal_To_Minimum_Velocity_Upper_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const float32_T k_min_vel_upper_limit_ta = p_ta_input->ta_coding_parameters.c_ta_min_vel_upper_limit;
   boolean_T f_speed = (boolean_T) (Fbk_Abs_F(p_vehicle_data->host_speed * TA_MPS_2_KPH) >= k_min_vel_upper_limit_ta);
   return f_speed;
}

static boolean_T Ta_Is_Speed_Less_Than_Equal_To_Minimum_Velocity_Lower_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                       const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const float32_T k_min_vel_lower_limit_ta = p_ta_input->ta_coding_parameters.c_ta_min_vel_lower_limit;
   boolean_T f_speed = (boolean_T) (Fbk_Abs_F(p_vehicle_data->host_speed * TA_MPS_2_KPH) <= k_min_vel_lower_limit_ta);
   return f_speed;
}

static boolean_T Ta_Is_Speed_Greater_Than_Equal_To_Maximum_Velocity_Upper_Limit_Ego_Speed(const Ta_Input_T *p_ta_input,
                                                                                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const float32_T k_max_vel_upper_limit_ta = p_ta_input->ta_coding_parameters.c_ta_max_vel_upper_limit;
   boolean_T f_speed = (boolean_T) (Fbk_Abs_F(p_vehicle_data->host_speed * TA_MPS_2_KPH) >= k_max_vel_upper_limit_ta);
   return f_speed;
}

static void Ta_State_Machine_Transitions_From_Not_Available_State(const Ta_Input_T *p_ta_input, TA_FF_State_T *p_ta_current_state)
{
   if (Fbk_Is_True(p_ta_input->ta_coding_parameters.c_f_ta_enabled))
   {
      *p_ta_current_state = TA_STATE_INACTIVE;
   }
   else
   {
      /*do nothing*/
   }
}

static void Ta_State_Machine_Transitions_From_InActive_State(const Ta_Input_T *p_ta_input,
                                                             TA_FF_State_T *p_ta_current_state,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_is_speed_greater_than_equal_to_minimum_velocity_upper_limit_ego_speed =
      (boolean_T) (Fbk_Is_True(Ta_Is_Speed_Greater_Than_Equal_To_Minimum_Velocity_Upper_Limit_Ego_Speed(p_ta_input, p_vehicle_data)));

   boolean_T f_is_speed_less_than_equal_to_maximum_velocity_lower_limit_ego_speed =
      (boolean_T) (Fbk_Is_True(Ta_Is_Speed_Less_Than_Equal_To_Maximum_Velocity_Lower_Limit_Ego_Speed(p_ta_input, p_vehicle_data)));

   boolean_T f_is_vehicle_driving = (boolean_T) (p_ta_input->ta_input_signals.pwf_state == TA_FAHREN);

   boolean_T f_is_vehicle_moving_forward =
      (boolean_T) (p_ta_input->ta_input_signals.vehicle_driving_direction == TA_VEHICLE_MOVES_FORWARD);

   boolean_T f_is_there_no_error = (boolean_T) (FBK_FALSE == p_ta_input->ta_input_signals.ta_function_error);

   boolean_T f_is_status_dynamometer_not_active = (boolean_T) (p_ta_input->ta_input_signals.status_dynamometer_mode == NO_DYNAMOMETER);

   boolean_T f_is_status_end_of_line_not_active =
      (boolean_T) (p_ta_input->ta_input_signals.status_end_of_line_mode == TA_END_OF_LINE_MODE_NOT_SET);

   if (Fbk_Is_False(p_ta_input->ta_coding_parameters.c_f_ta_enabled))
   {
      *p_ta_current_state = TA_STATE_NOT_AVAILABLE;
   }
   else if (f_is_speed_greater_than_equal_to_minimum_velocity_upper_limit_ego_speed
            && f_is_speed_less_than_equal_to_maximum_velocity_lower_limit_ego_speed && f_is_vehicle_moving_forward
            && f_is_vehicle_driving && f_is_status_dynamometer_not_active && f_is_status_end_of_line_not_active && f_is_there_no_error)
   {
      *p_ta_current_state = TA_STATE_ACTIVE;
   }
   else if (Fbk_Is_True(p_ta_input->ta_input_signals.ta_function_error))
   {
      *p_ta_current_state = TA_STATE_ERROR;
   }

   else
   {
      /*do nothing*/
   }
}

static void Ta_State_Machine_Transitions_From_Active_State(const Ta_Input_T *p_ta_input,
                                                           TA_FF_State_T *p_ta_current_state,
                                                           const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_is_speed_less_than_equal_to_minimum_velocity_lower_limit_ego_speed =
      (boolean_T) (Fbk_Is_True(Ta_Is_Speed_Less_Than_Equal_To_Minimum_Velocity_Lower_Limit_Ego_Speed(p_ta_input, p_vehicle_data)));

   boolean_T f_is_speed_greater_than_equal_to_max_velocity_upper_limit_ego_speed =
      (boolean_T) (Fbk_Is_True(Ta_Is_Speed_Greater_Than_Equal_To_Maximum_Velocity_Upper_Limit_Ego_Speed(p_ta_input, p_vehicle_data)));

   boolean_T f_is_vehicle_not_driving = (boolean_T) (p_ta_input->ta_input_signals.pwf_state != TA_FAHREN);

   boolean_T f_is_vehicle_not_moving_forward =
      (boolean_T) (p_ta_input->ta_input_signals.vehicle_driving_direction != TA_VEHICLE_MOVES_FORWARD);

   boolean_T f_is_there_no_error = (boolean_T) (FBK_FALSE == p_ta_input->ta_input_signals.ta_function_error);

   boolean_T f_is_status_dynamometer_active = (boolean_T) (p_ta_input->ta_input_signals.status_dynamometer_mode != NO_DYNAMOMETER);

   boolean_T f_is_status_end_of_line_active =
      (boolean_T) (p_ta_input->ta_input_signals.status_end_of_line_mode != TA_END_OF_LINE_MODE_NOT_SET);

   if (Fbk_Is_False(p_ta_input->ta_coding_parameters.c_f_ta_enabled))
   {
      *p_ta_current_state = TA_STATE_NOT_AVAILABLE;
   }
   else if ((f_is_speed_less_than_equal_to_minimum_velocity_lower_limit_ego_speed
             || f_is_speed_greater_than_equal_to_max_velocity_upper_limit_ego_speed || f_is_vehicle_not_moving_forward
             || f_is_vehicle_not_driving || f_is_status_dynamometer_active || f_is_status_end_of_line_active)
            && (f_is_there_no_error))
   {
      *p_ta_current_state = TA_STATE_INACTIVE;
   }
   else if (Fbk_Is_True(p_ta_input->ta_input_signals.ta_function_error))
   {
      *p_ta_current_state = TA_STATE_ERROR;
   }

   else
   {
      /*do nothing*/
   }
}

static void Ta_State_Machine_Transitions_From_Error_State(const Ta_Input_T *p_ta_input, TA_FF_State_T *p_ta_current_state)
{
   if (Fbk_Is_False(p_ta_input->ta_coding_parameters.c_f_ta_enabled))
   {
      *p_ta_current_state = TA_STATE_NOT_AVAILABLE;
   }
   else if (Fbk_Is_False(p_ta_input->ta_input_signals.ta_function_error))
   {
      *p_ta_current_state = TA_STATE_INACTIVE;
   }
   else
   {
      /*do nothing*/
   }
}


void Ta_Set_Current_Ta_Functional_State(const Ta_Input_T *p_ta_input,
                                        TA_FF_State_T *p_ta_current_state,
                                        const Fbk_Vehicle_Data_T *p_vehicle_data)
{

   /*State Transitions from Not Available State*/
   if (*p_ta_current_state == TA_STATE_NOT_AVAILABLE)
   {
      Ta_State_Machine_Transitions_From_Not_Available_State(p_ta_input, p_ta_current_state);
   }
   else
   {
      /* do nothing*/
   }
   /*State Transitions From InActive State*/
   if (*p_ta_current_state == TA_STATE_INACTIVE)
   {
      Ta_State_Machine_Transitions_From_InActive_State(p_ta_input, p_ta_current_state, p_vehicle_data);
   }
   else
   {
      /*do nothing*/
   }
   /*State Transitions From Active State*/
   if (*p_ta_current_state == TA_STATE_ACTIVE)
   {
      Ta_State_Machine_Transitions_From_Active_State(p_ta_input, p_ta_current_state, p_vehicle_data);
   }
   else
   {
      /*do nothing*/
   }
   /*State Transitions From Error State*/
   if (*p_ta_current_state == TA_STATE_ERROR)
   {
      Ta_State_Machine_Transitions_From_Error_State(p_ta_input, p_ta_current_state);
   }
   else
   {
      /*do nothing*/
   }
}
