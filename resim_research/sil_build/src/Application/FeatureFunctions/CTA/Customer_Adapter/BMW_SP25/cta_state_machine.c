/**
 * @file cta_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SP25 State Machine logic for CTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
/*===========================================================================
 * Includes
 *=========================================================================*/
#include "cta_state_machine.h"
#include "fbk_macros.h"
#include "pa_shared_types.h" // for PA_VEH_PRNDL_STATE_DRIVE, PA_VEH_PRNDL_...
#define CTA_MPS_2_KPH (3.6f)
/*
 * Function Definitions
 */
/**
 * @brief State Machine Transitions from READY.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Transitions_From_Ready(const Ctb_Function_Error_T *p_cta_errors,
                                       const Ctb_Flag_Output_T ctb_state_machine_flags,
                                       Ctb_State_Output_T *p_ctb_current_state);
/**
 * @brief State Machine Transitions from RCTA Active.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Transitions_From_Rcta_Active(const Ctb_Function_Error_T *p_cta_errors,
                                             const Ctb_Flag_Output_T ctb_state_machine_flags,
                                             Ctb_State_Output_T *p_ctb_current_state);
/**
 * @brief State Machine Transitions from FCTA Active.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Transitions_From_Fcta_Active(const Ctb_Function_Error_T *p_cta_errors,
                                             const Ctb_Flag_Output_T ctb_state_machine_flags,
                                             Ctb_State_Output_T *p_ctb_current_state);
/**
 * @brief State Machine Activate Flag Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Check_Ctb_Flag_Ctb_Activate(const Cta_Input_T *p_cta_input, Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/**
 * @brief State Machine Test Mode Flag Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Check_Ctb_Test_Mode(const Cta_Input_T *p_cta_input, Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/**
 * @brief State Machine Speed Flag Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Speed_Check(const Fbk_Vehicle_Data_T *p_vehicle_data,
                            const Cta_Core_Calibration_T *p_cta_cal,
                            Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/**
 * @brief State Machine Speed Hysteresis Flag Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Speed_Check_Hys(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Cta_Core_Calibration_T *p_cta_cal,
                                const Cta_Customer_Calibration_T *p_cta_custom_cal,
                                Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/**
 * @brief State Machine Fcta Activation Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Fcta_Activate_Check(const Cta_Input_T *p_cta_input,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data,
                                    Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/**
 * @brief State Machine Rcta Activation Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Rcta_Activate_Check(const Cta_Input_T *p_cta_input,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data,
                                    Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/**
 * @brief State Machine Ready to Active Transition.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Ready_to_Active(const Ctb_Flag_Output_T ctb_state_machine_flags, Ctb_State_Output_T *p_ctb_current_state);
/**
 * @brief State Machine Cta_Brake_Override_Check.
 *
 * @return void
 * @return void
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Cta_Brake_Override_Check(const Cta_Input_T *p_cta_input,
                                     const Cta_Core_Calibration_T *p_cta_cal,
                                     Ctb_Flag_Output_T *p_ctb_state_machine_flags);
/*
 * Cta State Machine Implementation
 *
 */
void Cta_State_Machine(Ctb_State_Output_T *p_ctb_current_state,
                       const Ctb_Function_Error_T *p_cta_errors,
                       const Ctb_Flag_Output_T ctb_state_machine_flags)
{
   /*State Transitions*/
   /*From and To Not Available*/
   if (Fbk_Is_False(ctb_state_machine_flags.f_ctb_flag_ctb_enabled))
   {
      *p_ctb_current_state = CTB_STATE_NOT_AVAILABLE;
   }
   /*Not Available to Ready*/
   if (*p_ctb_current_state == CTB_STATE_NOT_AVAILABLE)
   {
      if (Fbk_Is_True(ctb_state_machine_flags.f_ctb_flag_ctb_enabled))
      {
         *p_ctb_current_state = CTB_STATE_READY;
      }
   }
   /*Ready*/
   if (*p_ctb_current_state == CTB_STATE_READY)
   {
      Cta_Transitions_From_Ready(p_cta_errors, ctb_state_machine_flags, p_ctb_current_state);
   }
   /*Rcta Active*/
   if (*p_ctb_current_state == CTB_STATE_RCTA_ACTIVE)
   {
      Cta_Transitions_From_Rcta_Active(p_cta_errors, ctb_state_machine_flags, p_ctb_current_state);
   }
   /*Fcta Active*/
   if (*p_ctb_current_state == CTB_STATE_FCTA_ACTIVE)
   {
      Cta_Transitions_From_Fcta_Active(p_cta_errors, ctb_state_machine_flags, p_ctb_current_state);
   }
   /*CTB Error*/
   if (*p_ctb_current_state == CTB_STATE_ERROR)
   {
      if ((BMW_CTA_NO_ERROR == *p_cta_errors))
      {
         *p_ctb_current_state = CTB_STATE_READY;
      }
   }
}
static void Cta_Transitions_From_Ready(const Ctb_Function_Error_T *p_cta_errors,
                                       const Ctb_Flag_Output_T ctb_state_machine_flags,
                                       Ctb_State_Output_T *p_ctb_current_state)
{
   /*Ready to Active */
   Cta_Ready_to_Active(ctb_state_machine_flags, p_ctb_current_state);
   if (BMW_CTA_ERROR == *p_cta_errors)
   {
      *p_ctb_current_state = CTB_STATE_ERROR;
   }
}
static void Cta_Ready_to_Active(const Ctb_Flag_Output_T ctb_state_machine_flags, Ctb_State_Output_T *p_ctb_current_state)
{
   /*Ready to Active*/
   if (Fbk_Is_True(ctb_state_machine_flags.f_speed_check) && Fbk_Is_True(ctb_state_machine_flags.f_rcta_activate)
       && Fbk_Is_False(ctb_state_machine_flags.f_ctb_test_mode))
   {
      *p_ctb_current_state = CTB_STATE_RCTA_ACTIVE;
   }
   /*Ready to FCTA Active*/
   if (Fbk_Is_True(ctb_state_machine_flags.f_speed_check) && Fbk_Is_True(ctb_state_machine_flags.f_fcta_activate)
       && Fbk_Is_False(ctb_state_machine_flags.f_ctb_test_mode))
   {
      *p_ctb_current_state = CTB_STATE_FCTA_ACTIVE;
   }
}
static void Cta_Transitions_From_Rcta_Active(const Ctb_Function_Error_T *p_cta_errors,
                                             const Ctb_Flag_Output_T ctb_state_machine_flags,
                                             Ctb_State_Output_T *p_ctb_current_state)
{
   /*To Ready*/
   if (((Fbk_Is_True(ctb_state_machine_flags.f_speed_check_hys) || Fbk_Is_False(ctb_state_machine_flags.f_rcta_activate))
        && (BMW_CTA_NO_ERROR == *p_cta_errors))
       || Fbk_Is_True(ctb_state_machine_flags.f_ctb_test_mode))
   {
      *p_ctb_current_state = CTB_STATE_READY;
   }
   if (BMW_CTA_NO_ERROR != *p_cta_errors)
   {
      *p_ctb_current_state = CTB_STATE_ERROR;
   }
}
static void Cta_Transitions_From_Fcta_Active(const Ctb_Function_Error_T *p_cta_errors,
                                             const Ctb_Flag_Output_T ctb_state_machine_flags,
                                             Ctb_State_Output_T *p_ctb_current_state)
{
   /*To Ready*/
   if (((Fbk_Is_True(ctb_state_machine_flags.f_speed_check_hys) || Fbk_Is_False(ctb_state_machine_flags.f_fcta_activate))
        && (BMW_CTA_NO_ERROR == *p_cta_errors))
       || Fbk_Is_True(ctb_state_machine_flags.f_ctb_test_mode))
   {
      *p_ctb_current_state = CTB_STATE_READY;
   }
   if (BMW_CTA_NO_ERROR != *p_cta_errors)
   {
      *p_ctb_current_state = CTB_STATE_ERROR;
   }
}
void Cta_Update_Flags(const Cta_Input_T *p_cta_input,
                      const Cta_Core_Calibration_T *p_cta_cal,
                      const Cta_Customer_Calibration_T *p_cta_custom_cal,
                      Ctb_Flag_Output_T *p_ctb_state_machine_flags,
                      const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /*Enabled Check*/
   p_ctb_state_machine_flags->f_ctb_flag_ctb_enabled = p_cta_input->bmw_ctb_coding_parameters.c_ctb_enabled;
   /*Activation Check */
   Cta_Check_Ctb_Flag_Ctb_Activate(p_cta_input, p_ctb_state_machine_flags);
   /*Test Mode Check*/
   Cta_Check_Ctb_Test_Mode(p_cta_input, p_ctb_state_machine_flags);
   /*Speed Check*/
   Cta_Speed_Check(p_vehicle_data, p_cta_cal, p_ctb_state_machine_flags);
   /*Speed Hys Check*/
   Cta_Speed_Check_Hys(p_vehicle_data, p_cta_cal, p_cta_custom_cal, p_ctb_state_machine_flags);
   /*FCTA Activation check*/
   Cta_Fcta_Activate_Check(p_cta_input, p_vehicle_data, p_ctb_state_machine_flags);
   /*Rcta Activation Check*/
   Cta_Rcta_Activate_Check(p_cta_input, p_vehicle_data, p_ctb_state_machine_flags);
   /*brake_override Check*/
   Cta_Brake_Override_Check(p_cta_input, p_cta_cal, p_ctb_state_machine_flags);
}
static void Cta_Check_Ctb_Flag_Ctb_Activate(const Cta_Input_T *p_cta_input, Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   if ((BMW_CTB_ACTIVATED == p_cta_input->bmw_ctb_input_signals.setting_cross_traffic_brake)
       || (BMW_CTB_ACTIVATED == p_cta_input->bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_ctb_flag_ctb_activated = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_ctb_flag_ctb_activated = FBK_FALSE;
   }
}
static void Cta_Check_Ctb_Test_Mode(const Cta_Input_T *p_cta_input, Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   if (((BMW_CTB_FRONT_AXLE_ON_DYNAMOMETER == p_cta_input->bmw_ctb_input_signals.status_roller_dynamometer)
        || (BMW_CTB_BACK_AXLE_ON_DYNAMOMETER == p_cta_input->bmw_ctb_input_signals.status_roller_dynamometer)
        || (BMW_CTB_TWO_AXLE_DYNAMOMETER == p_cta_input->bmw_ctb_input_signals.status_roller_dynamometer))
       || (END_OF_LINE_MODE_SET == p_cta_input->bmw_ctb_input_signals.status_end_of_line))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_ctb_test_mode = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_ctb_test_mode = FBK_FALSE;
   }
}
static void Cta_Speed_Check(const Fbk_Vehicle_Data_T *p_vehicle_data,
                            const Cta_Core_Calibration_T *p_cta_cal,
                            Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   if (Fbk_Abs_F(p_vehicle_data->host_speed) <= (p_cta_cal->k_cta_ego_abs_speed_max))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_speed_check = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_speed_check = FBK_FALSE;
   }
}
static void Cta_Speed_Check_Hys(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                const Cta_Core_Calibration_T *p_cta_cal,
                                const Cta_Customer_Calibration_T *p_cta_custom_cal,
                                Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   if (Fbk_Abs_F(p_vehicle_data->host_speed)
       > ((p_cta_cal->k_cta_ego_abs_speed_max) + (p_cta_custom_cal->k_bmw_sp25_ego_abs_speed_max_hys / CTA_MPS_2_KPH)))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_speed_check_hys = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_speed_check_hys = FBK_FALSE;
   }
}
static void Cta_Fcta_Activate_Check(const Cta_Input_T *p_cta_input,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data,
                                    Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   if (Fbk_Is_True(p_ctb_state_machine_flags->f_ctb_flag_ctb_enabled)
       && Fbk_Is_True(p_ctb_state_machine_flags->f_ctb_flag_ctb_activated) && (PA_VEH_PRNDL_STATE_DRIVE == p_vehicle_data->prndl)
       && Fbk_Is_True(p_cta_input->bmw_ctb_input_signals.parking_context_active)
       && Fbk_Is_True(p_cta_input->bmw_ctb_input_signals.control_cross_traffic_alert_front))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_fcta_activate = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_fcta_activate = FBK_FALSE;
   }
}
static void Cta_Rcta_Activate_Check(const Cta_Input_T *p_cta_input,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data,
                                    Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   /*Rcta Activation Check */
   if (Fbk_Is_True(p_ctb_state_machine_flags->f_ctb_flag_ctb_enabled)
       && Fbk_Is_True(p_ctb_state_machine_flags->f_ctb_flag_ctb_activated) && (PA_VEH_PRNDL_STATE_REVERSE == p_vehicle_data->prndl)
       && Fbk_Is_True(p_cta_input->bmw_ctb_input_signals.parking_context_active)
       && Fbk_Is_True(p_cta_input->bmw_ctb_input_signals.control_cross_traffic_alert_rear))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_rcta_activate = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_rcta_activate = FBK_FALSE;
   }
}
static void Cta_Brake_Override_Check(const Cta_Input_T *p_cta_input,
                                     const Cta_Core_Calibration_T *p_cta_cal,
                                     Ctb_Flag_Output_T *p_ctb_state_machine_flags)
{
   if (Fbk_Is_True(p_cta_cal->k_cta_f_brake_overriding_ctb)
       && (p_cta_input->bmw_ctb_input_signals.gradient_angle_acceleratorpedal
           > (p_cta_cal->k_cta_accelerationpedal_gradient_threshold_ctb)))
   {
      /*Setting Flag to TRUE */
      p_ctb_state_machine_flags->f_brake_override = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      p_ctb_state_machine_flags->f_brake_override = FBK_FALSE;
   }
}
