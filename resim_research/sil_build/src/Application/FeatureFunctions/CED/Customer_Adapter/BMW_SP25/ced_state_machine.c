/**
 * @file ced_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SP25 State Machine logic for CED.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_state_machine.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*============================================================================*\
 * GLOBAL  VARIABLES
\*============================================================================*/

/*============================================================================*\
 * GLOBAL STATIC VARIABLES
\*============================================================================*/

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

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
static boolean_T Ced_Is_Ced_Working_In_Test_Mode(const Ced_Input_T *p_ced_input);


/**
 * @brief CED State Machine Transitions From Not-Available State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ced_State_Machine_Transitions_From_Not_Available_State(const Ced_Input_T *p_ced_input,
                                                                   CED_FF_STATE_T *p_ced_current_state);

/**
 * @brief CED State Machine Transitions From Ready State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ced_State_Machine_Transitions_From_Ready_State(const Ced_Input_T *p_ced_input,
                                                           const Ced_Core_Calibration_T *p_ced_cal,
                                                           CED_FF_STATE_T *p_ced_current_state,
                                                           const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief CED State Machine Transitions From Active State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ced_State_Machine_Transitions_From_Active_State(const Ced_Input_T *p_ced_input,
                                                            const Ced_Core_Calibration_T *p_ced_cal,
                                                            CED_FF_STATE_T *p_ced_current_state,
                                                            const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief CED State Machine Transitions From Degraded State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ced_State_Machine_Transitions_From_Degraded_State(const Ced_Input_T *p_ced_input,
                                                              const Ced_Core_Calibration_T *p_ced_cal,
                                                              CED_FF_STATE_T *p_ced_current_state,
                                                              const Fbk_Vehicle_Data_T *p_vehicle_data);

/**
 * @brief CED State Machine Transitions From Error State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Ced_State_Machine_Transitions_From_Error_State(const Ced_Input_T *p_ced_input, CED_FF_STATE_T *p_ced_current_state);


/**
 * @brief Condition check for Speed Less than or equal to threshold.
 *
 * @return boolean_T
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ced_Is_Speed_Less_Than_Threshold(const Fbk_Vehicle_Data_T *p_vehicle_data, const Ced_Core_Calibration_T *p_ced_cal);

/**
 * @brief Condition check for Speed Greater than or less than threshold with hysteresis.
 *
 * @return boolean_T
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                     const Ced_Core_Calibration_T *p_ced_cal);


static boolean_T Ced_Is_Speed_Less_Than_Threshold(const Fbk_Vehicle_Data_T *p_vehicle_data, const Ced_Core_Calibration_T *p_ced_cal)
{
   const float32_T speed_threshold = p_ced_cal->k_ced_ego_abs_speed_max;
   boolean_T f_speed_thresh        = (boolean_T) (Fbk_Abs_F(p_vehicle_data->host_speed) < speed_threshold);
   return f_speed_thresh;
}

static boolean_T Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis(const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                     const Ced_Core_Calibration_T *p_ced_cal)
{
   const float32_T speed_threshold_with_hysteresis = p_ced_cal->k_ced_ego_abs_speed_max + p_ced_cal->k_bmw_ced_speed_max_hysteresis;
   boolean_T f_speed_hysteresis = (boolean_T) (Fbk_Abs_F(p_vehicle_data->host_speed) > speed_threshold_with_hysteresis);
   return f_speed_hysteresis;
}


static boolean_T Ced_Is_Ced_Working_In_Test_Mode(const Ced_Input_T *p_ced_input)
{
   boolean_T f_ced_test_mode;
   if (((BMW_CED_FRONT_AXLE_ON_DYNAMOMETER == p_ced_input->ced_input_bus_signals.ced_status_roller_dynamometer)
        || (BMW_CED_BACK_AXLE_ON_DYNAMOMETER == p_ced_input->ced_input_bus_signals.ced_status_roller_dynamometer)
        || (BMW_CED_TWO_AXLE_DYNAMOMETER == p_ced_input->ced_input_bus_signals.ced_status_roller_dynamometer))
       || (BMW_CED_END_OF_LINE_MODE_SET == p_ced_input->ced_input_bus_signals.ced_status_end_of_line))
   {
      /*Setting Flag to TRUE */
      f_ced_test_mode = FBK_TRUE;
   }
   else
   {
      /*Setting Flag to FALSE */
      f_ced_test_mode = FBK_FALSE;
   }
   return f_ced_test_mode;
}

static void Ced_State_Machine_Transitions_From_Not_Available_State(const Ced_Input_T *p_ced_input, CED_FF_STATE_T *p_ced_current_state)
{
   if ((Fbk_Is_True(p_ced_input->ced_coding_parameters.c_f_sfe_function_enabled))
       || (Fbk_Is_True(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_READY;
   }
   else
   {
      /*do nothing*/
   }
}


static void Ced_State_Machine_Transitions_From_Ready_State(const Ced_Input_T *p_ced_input,
                                                           const Ced_Core_Calibration_T *p_ced_cal,
                                                           CED_FF_STATE_T *p_ced_current_state,
                                                           const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   if ((Fbk_Is_False(p_ced_input->ced_coding_parameters.c_f_sfe_function_enabled))
       && (Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_NOTAVAILABLE;
   }
   else if ((Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_activation))
            && (Fbk_Is_True(Ced_Is_Speed_Less_Than_Threshold(p_vehicle_data, p_ced_cal)))
            && (Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_ACTIVE;
   }
   else if (Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state)
            && Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_activation)
            && Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input)))
   {
      *p_ced_current_state = CED_STATE_ERROR;
   }
   else
   {
      /*Do nothing*/
   }
}


static void Ced_State_Machine_Transitions_From_Active_State(const Ced_Input_T *p_ced_input,
                                                            const Ced_Core_Calibration_T *p_ced_cal,
                                                            CED_FF_STATE_T *p_ced_current_state,
                                                            const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const boolean_T is_input_signal_ced_function_activation_disabled =
      (boolean_T) (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_activation));

   const boolean_T is_speed_greater_than_threshold_value_with_hysteresis =
      (boolean_T) (Fbk_Is_True(Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis(p_vehicle_data, p_ced_cal)));

   const boolean_T is_input_signal_ced_function_fault_state_disabled =
      (boolean_T) (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state));

   const boolean_T is_ced_in_test_mode = (boolean_T) (Fbk_Is_True(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input)));

   if ((is_input_signal_ced_function_activation_disabled || is_speed_greater_than_threshold_value_with_hysteresis)
       && (is_input_signal_ced_function_fault_state_disabled || is_ced_in_test_mode))
   {
      *p_ced_current_state = CED_STATE_READY;
   }
   else if ((Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_activation))
            && (Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state))
            && (Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_ERROR;
   }
   else if ((Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_degraded))
            && (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state))
            && (Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_DEGRADED;
   }
   else
   {
      /*do nothing*/
   }
}

static void Ced_State_Machine_Transitions_From_Degraded_State(const Ced_Input_T *p_ced_input,
                                                              const Ced_Core_Calibration_T *p_ced_cal,
                                                              CED_FF_STATE_T *p_ced_current_state,
                                                              const Fbk_Vehicle_Data_T *p_vehicle_data)
{

   const boolean_T is_input_signal_ced_function_activation_disabled =
      (boolean_T) (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_activation));

   const boolean_T is_speed_greater_than_threshold_value_with_hysteresis =
      (boolean_T) (Fbk_Is_True(Ced_Is_Speed_Greater_Than_Threshold_With_Hysteresis(p_vehicle_data, p_ced_cal)));

   const boolean_T is_input_signal_ced_function_fault_state_disabled =
      (boolean_T) (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state));

   const boolean_T is_ced_in_test_mode = (boolean_T) (Fbk_Is_True(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input)));

   if (((is_input_signal_ced_function_activation_disabled || is_speed_greater_than_threshold_value_with_hysteresis)
        && (is_input_signal_ced_function_fault_state_disabled || is_ced_in_test_mode)))
   {
      *p_ced_current_state = CED_STATE_READY;
   }
   else if ((Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_activation)
             && (Fbk_Is_True(Ced_Is_Speed_Less_Than_Threshold(p_vehicle_data, p_ced_cal)))
             && (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state)))
            && (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_degraded))
            && (Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_ACTIVE;
   }
   else if ((Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state))
            && (Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_function_activation))
            && (Fbk_Is_False(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_ERROR;
   }
   else
   {
      /*do nothing*/
   }
}


static void Ced_State_Machine_Transitions_From_Error_State(const Ced_Input_T *p_ced_input, CED_FF_STATE_T *p_ced_current_state)
{
   if (((Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_fault_state))
        && (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_function_activation)))
       || (Fbk_Is_True(Ced_Is_Ced_Working_In_Test_Mode(p_ced_input))))
   {
      *p_ced_current_state = CED_STATE_READY;
   }
   else
   {
      /*do nothing*/
   }
}

void Ced_Set_Current_Ced_Functional_State(const Ced_Input_T *p_ced_input,
                                          const Ced_Core_Calibration_T *p_ced_cal,
                                          CED_FF_STATE_T *p_ced_current_state,
                                          const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   if (CED_STATE_NOTAVAILABLE == *p_ced_current_state)
   {
      Ced_State_Machine_Transitions_From_Not_Available_State(p_ced_input, p_ced_current_state);
   }
   if (CED_STATE_READY == *p_ced_current_state)
   {
      Ced_State_Machine_Transitions_From_Ready_State(p_ced_input, p_ced_cal, p_ced_current_state, p_vehicle_data);
   }
   if (CED_STATE_ACTIVE == *p_ced_current_state)
   {
      Ced_State_Machine_Transitions_From_Active_State(p_ced_input, p_ced_cal, p_ced_current_state, p_vehicle_data);
   }
   if (CED_STATE_DEGRADED == *p_ced_current_state)
   {
      Ced_State_Machine_Transitions_From_Degraded_State(p_ced_input, p_ced_cal, p_ced_current_state, p_vehicle_data);
   }
   if (CED_STATE_ERROR == *p_ced_current_state)
   {
      Ced_State_Machine_Transitions_From_Error_State(p_ced_input, p_ced_current_state);
   }
}
