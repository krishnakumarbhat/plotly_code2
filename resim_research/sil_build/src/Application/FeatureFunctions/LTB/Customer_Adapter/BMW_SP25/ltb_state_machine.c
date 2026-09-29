/**
 * @file ltb_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 state machine logic for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_state_machine.h"
#include "fbk_macros.h"         // for Fbk_Is_True, FBK_FALSE, FBK_TRUE
#include "fbk_vehicle_data_t.h" // for Fbk_Vehicle_Data_T
#include "pa_reuse.h"           // for boolean_T

static Ltb_State_T Ltb_Current_State = LTB_STATE_NOT_AVAILABLE;

/*=============================================================================
* Local Function Declarations
\*============================================================================*/
/**
 * @brief State Machine Flag Speed Check
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ltb_Update_Vehicle_Speed_Check_Flag(const Ltb_Core_Calibration_T *p_ltb_cal,
                                                     const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief State Machine Flag Speed Hys Check
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ltb_Update_Vehicle_Speed_Check_Hys_Flag(const Ltb_Core_Calibration_T *p_ltb_cal,
                                                         const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief State Machine Movement Direction Check
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ltb_Update_Vehicle_Moving_Forward_Check_Flag(const Ltb_Input_T *p_ltb_input);

/**
 * @brief State Machine Vehicle Condition Check
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ltb_Update_Pwf_Check_Flag(const Ltb_Input_T *p_ltb_input);

/**
 * @brief State Machine Test Mode Check
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ltb_Update_Test_Mode_Check_Flag(const Ltb_Input_T *p_ltb_input);

/**
 * @brief State Machine Error Flag Update
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ltb_Update_Error_Flag(const Ltb_Input_T *p_ltb_input);

/**
 * @brief State Machine Transition from Ready to Active
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ltb_State_Transition_From_Ready_to_Active(const Ltb_State_Flags_T *p_ltb_state_flags);

/**
 * @brief State Machine Transition from Active to Ready
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ltb_State_Transition_From_Active_to_Ready(const Ltb_State_Flags_T *p_ltb_state_flags);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/
/**
 * Getter Function Definition for Ltb Current State
 */
Ltb_State_T *Ltb_Get_Current_State(void)
{
   return &Ltb_Current_State;
}

void Ltb_Update_Flags(const Ltb_Input_T *p_ltb_input,
                      const Ltb_Core_Calibration_T *p_ltb_cal,
                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                      Ltb_State_Flags_T *p_ltb_state_flags)
{
   p_ltb_state_flags->Ltb_Enable_Check         = p_ltb_input->f_ltb_enable;
   p_ltb_state_flags->Ltb_Speed_Check_Flag     = Ltb_Update_Vehicle_Speed_Check_Flag(p_ltb_cal, p_vehicle_data);
   p_ltb_state_flags->Ltb_Speed_Check_Hys_Flag = Ltb_Update_Vehicle_Speed_Check_Hys_Flag(p_ltb_cal, p_vehicle_data);
   p_ltb_state_flags->Ltb_Check_Forward_Driving_Direction_Flag = Ltb_Update_Vehicle_Moving_Forward_Check_Flag(p_ltb_input);
   p_ltb_state_flags->Ltb_Check_Pwf_State_Driving_Flag         = Ltb_Update_Pwf_Check_Flag(p_ltb_input);
   p_ltb_state_flags->Ltb_Test_Mode_Check_Flag                 = Ltb_Update_Test_Mode_Check_Flag(p_ltb_input);
   p_ltb_state_flags->Ltb_Error_Check_Flag                     = Ltb_Update_Error_Flag(p_ltb_input);
}


void Ltb_State_Machine(const Ltb_State_Flags_T *p_ltb_state_flags)
{

   /*Transition from Error State to Ready*/
   if (LTB_STATE_ERROR == Ltb_Current_State)
   {
      if (Fbk_Is_False(p_ltb_state_flags->Ltb_Error_Check_Flag))
      {
         Ltb_Current_State = LTB_STATE_READY;
      }
   }
   if (LTB_STATE_NOT_AVAILABLE == Ltb_Current_State)
   {
      /* Transition from Not Available to Ready */
      if (Fbk_Is_True(p_ltb_state_flags->Ltb_Enable_Check))
      {
         Ltb_Current_State = LTB_STATE_READY;
      }
   }
   if (LTB_STATE_READY == Ltb_Current_State)
   {
      /*Transition form Ready to Active*/
      Ltb_State_Transition_From_Ready_to_Active(p_ltb_state_flags);
   }
   if (LTB_STATE_ACTIVE == Ltb_Current_State)
   {
      /*Transition form Active to Ready*/
      Ltb_State_Transition_From_Active_to_Ready(p_ltb_state_flags);
   }
   /*Transition to Error State*/
   if (Fbk_Is_True(p_ltb_state_flags->Ltb_Error_Check_Flag))
   {
      Ltb_Current_State = LTB_STATE_ERROR;
   }
   /*Transition to Not Available State*/
   if (Fbk_Is_False(p_ltb_state_flags->Ltb_Enable_Check))
   {
      Ltb_Current_State = LTB_STATE_NOT_AVAILABLE;
   }
}

static void Ltb_State_Transition_From_Ready_to_Active(const Ltb_State_Flags_T *p_ltb_state_flags)
{
   if (Fbk_Is_True(p_ltb_state_flags->Ltb_Speed_Check_Flag) && Fbk_Is_True(p_ltb_state_flags->Ltb_Check_Forward_Driving_Direction_Flag)
       && Fbk_Is_True(p_ltb_state_flags->Ltb_Check_Pwf_State_Driving_Flag)
       && Fbk_Is_False(p_ltb_state_flags->Ltb_Test_Mode_Check_Flag))
   {
      Ltb_Current_State = LTB_STATE_ACTIVE;
   }
}

static void Ltb_State_Transition_From_Active_to_Ready(const Ltb_State_Flags_T *p_ltb_state_flags)
{
   if (Fbk_Is_True(p_ltb_state_flags->Ltb_Speed_Check_Hys_Flag)
       || Fbk_Is_False(p_ltb_state_flags->Ltb_Check_Forward_Driving_Direction_Flag)
       || Fbk_Is_False(p_ltb_state_flags->Ltb_Check_Pwf_State_Driving_Flag)
       || Fbk_Is_True(p_ltb_state_flags->Ltb_Test_Mode_Check_Flag))
   {
      Ltb_Current_State = LTB_STATE_READY;
   }
}

static boolean_T Ltb_Update_Vehicle_Speed_Check_Flag(const Ltb_Core_Calibration_T *p_ltb_cal, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T return_flag = FBK_FALSE;
   if (p_vehicle_data->host_speed < p_ltb_cal->k_ltb_bmw_sp25_v_ego_max)
   {
      return_flag = FBK_TRUE;
   }
   return return_flag;
}

static boolean_T Ltb_Update_Vehicle_Speed_Check_Hys_Flag(const Ltb_Core_Calibration_T *p_ltb_cal,
                                                         const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T return_flag = FBK_FALSE;
   if (p_vehicle_data->host_speed > (p_ltb_cal->k_ltb_bmw_sp25_v_ego_max + p_ltb_cal->k_ltb_bmw_sp25_v_ego_max_hys))
   {
      return_flag = FBK_TRUE;
   }
   return return_flag;
}

static boolean_T Ltb_Update_Vehicle_Moving_Forward_Check_Flag(const Ltb_Input_T *p_ltb_input)
{
   boolean_T return_flag = FBK_FALSE;
   if (BMW_CTB_VEH_MOVING_DIR_MOVES_FORWARD == p_ltb_input->ltb_vehicle_parameters.vehicle_moving_direction)
   {
      return_flag = FBK_TRUE;
   }
   return return_flag;
}

static boolean_T Ltb_Update_Pwf_Check_Flag(const Ltb_Input_T *p_ltb_input)
{
   boolean_T return_flag = FBK_FALSE;
   if (DRIVING == p_ltb_input->ltb_vehicle_parameters.pwf_state)
   {
      return_flag = FBK_TRUE;
   }
   return return_flag;
}

static boolean_T Ltb_Update_Test_Mode_Check_Flag(const Ltb_Input_T *p_ltb_input)
{
   boolean_T return_flag = FBK_FALSE;
   if ((BMW_LTB_FRONT_AXLE_ON_DYNAMOMETER == p_ltb_input->ltb_vehicle_parameters.status_roller_dynamometer)
       || (BMW_LTB_BACK_AXLE_ON_DYNAMOMETER == p_ltb_input->ltb_vehicle_parameters.status_roller_dynamometer)
       || (BMW_LTB_TWO_AXLE_DYNAMOMETER == p_ltb_input->ltb_vehicle_parameters.status_roller_dynamometer)
       || (LTB_END_OF_LINE_MODE_SET == p_ltb_input->ltb_vehicle_parameters.status_end_of_lne))
   {
      return_flag = FBK_TRUE;
   }
   return return_flag;
}

static boolean_T Ltb_Update_Error_Flag(const Ltb_Input_T *p_ltb_input)
{
   boolean_T return_flag = FBK_FALSE;
   if (LTB_ERROR == p_ltb_input->ltb_error)
   {
      return_flag = FBK_TRUE;
   }
   return return_flag;
}
