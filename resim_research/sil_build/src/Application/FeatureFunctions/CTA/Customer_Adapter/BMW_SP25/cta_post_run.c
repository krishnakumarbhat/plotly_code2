/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 post run logic for CTA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */
#include "cta_post_run.h"
#include "cta_bmw_sp25_types.h"
#include "cta_bmw_sp25_warn_state.h"
#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_customer_calibration_t.h"
#include "cta_state_machine.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>
#define CTA_MAX_FLOAT_TIME (255.0f)
#define CTA_ACOSTICS_WARNING_EARCON_ID (2u)
#define CTA_WARNING_FRONT_LEFT_HA (45.0f)
#define CTA_WARNING_RARE_LEFT_HA (135.0f)
#define CTA_WARNING_RARE_RIGHT_HA (225.0f)
#define CTA_WARNING_FRONT_RIGHT_HA (315.0f)
#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */


/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

static float32_T Current_Ctb_Brake_Deceleration;
static float32_T Cta_Banner_Time_Inc_Rcta = FBK_ZERO_F;
static float32_T Cta_Banner_Time_Inc_Fcta = FBK_ZERO_F;
/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/
/**
 * @brief Sets Bus Outputs while in Not Active States
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Cta_Set_Outputs_Not_Active_State(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals);

/**
 * @brief Resets all signals in BMW specific output.
 *
 * @return void
 *
 * @SRS{SF-165}
 * @SAE{SF-2458}
 * @SDD{SF-3958}
 * @verification{}
 */
static void Cta_Reset_Output(Cta_Output_T *p_cta_output /**< CTA output*/);

/**
 * @brief Resets all signals in BMW specific Algo-State output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4053}
 * @verification{}
 */
static void Cta_Reset_Algo_State_Output(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output /**< BMW Algo Output */);

/**
 * @brief Resets all signals in BMW specific Bus Signal output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4055}
 * @verification{}
 */
static void Cta_Reset_Bus_Signals_Output(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals /**< BMW Bus Signal Output */);

/**
 * @brief Resets all signals in BMW specific Event-Data-Recorder output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4054}
 * @verification{}
 */
static void Cta_Reset_Edr_Signals_Output(Bmw_Ctb_Output_Edr_T *p_edr_output /**< BMW EDR Signal Output */);

/**
 * @brief Applies BMW specific filtering whether the braking qualifier of core shall be passed further.
 *
 * @return FBK_TRUE when radar position is rear and an alert is active
 *
 * @SRS{SF-165}
 * @SAE{SF-2458}
 * @SDD{SF-3963}
 * @verification{}
 */
static boolean_T Cta_Check_Rctb_Qualifier(const Cta_Core_Output_T *p_cta_core_output /**< CTA core output */,
                                          const uint8_t approach_side, /**< Objects approach side */
                                          const uint8_t ctb_mode /**< Mode of CTB */);

/**
 * @brief Returns braking acceleration when CTB shall trigger.
 *
 * @return braking deceleration in [m/s^2] of type float
 *
 * @SRS{SF-166}
 * @SAE{SF-2458}
 * @SDD{SF-3962}
 * @verification{}
 */
static float32_T Cta_Get_Braking_Acceleration(const Pa_Data_T *p_pa_data /**< PA data */,
                                              const Cta_Core_Calibration_T *p_cta_cal /**< CTA calibrations */);

/**
 * @brief Transfer information from Algo-State to BMW specific Bus representation
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4028}
 * @verification{}
 */
static void Cta_Set_Output_Bus_Signals(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals /**< BMW Bus Signal Output */,
                                       const Bmw_Ctb_Output_Algo_State_T *p_algo_state /**< BMW Algo Output */,
                                       const Cta_Input_T *p_cta_input /**< BMW CTA Inputs*/,
                                       const Cta_Instance_T *p_cta_instance /**< CTA Instance */);

/**
 * @brief Determine the BMW specific bitfield representation of warning states
 *
 * @return all warning informations in BMW representation as bitfield of type uint_8
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4034}
 * @verification{}
 */
static uint8_t Cta_Get_Alert_Position_Encoding(
   const Bmw_Ctb_Warning_T p_ctb_warning[CTA_BMW_ALERT_POSITION_COUNT] /**< Array of warnings for (FL, FR, RL, RR) */,
   const Ctb_State_Output_T cta_state /**<Ctb State Output */);

/**
 * @brief Set the BMW-specific outputs in a structured/readable representation
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4033}
 * @verification{}
 */
static void Cta_Set_Output_Algo_State(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output /**< BMW Algo Output */,
                                      const Cta_Input_T *p_cta_input /**< CTA input*/,
                                      const Cta_Instance_T *p_cta_instance /**< CTA instance*/,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data, /**< Vehicle Data from Fbk*/
                                      const uint8_t ctb_mode /**< Mode of CTB */);

/**
 * @brief Set the BMW Event-Data-Recorder output
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4032}
 * @verification{}
 */
static void Cta_Set_Output_Edr(Bmw_Ctb_Output_Edr_T *p_edr_output /**< BMW Event-Data-Recorder Output */);

/**
 * @brief Set the BMW-specific Warning outputs based on the Warning-State
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4030}
 * @verification{}
 */
static void Cta_Set_Warning_Outputs(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output /**< BMW Algo Output */,
                                    Bmw_Ctb_Alert_Side_T side /**< Side information */);

/**
 * @brief Set the BMW-specific Braking outputs based on the Braking-State
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4029}
 * @verification{}
 */
static void Cta_Set_Braking_Outputs(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output /**< BMW Algo Output */,
                                    const Cta_Input_T *p_cta_input /**< CTA input*/,
                                    const Cta_Instance_T *p_cta_instance /**< CTA Instance */,
                                    const uint8_t ctb_mode /**< Mode of CTB*/);

/**
 * @brief Determine in which outputs of the core shall be mapped to the customer output. Decides whether Front or Rear
 * core output shall be mapped.
 *
 * @return mode of CTA which needs to be mapped to the output (front, rear, num_modes as default)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4052}
 * @verification{}
 */
static uint8_t Cta_Get_Variant_Position(const Fbk_Vehicle_Data_T *p_vehicle_data /**< Vehicle Data from Fbk*/,
                                        const Cta_Core_Calibration_T *p_cta_cal /**< CTA calibrations */);
/**
 * @brief Merging Sub states to CTA State Output
 *
 * @return(void)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4052}
 * @verification{}
 */

static void Cta_Merge_Sub_States(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                 const Cta_Input_T *p_cta_input,
                                 Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                 const Ctb_State_Output_T *p_cta_states,
                                 const boolean_T cta_braking_state,
                                 const Cta_Instance_T *p_cta_instance);
/**
 * @brief Setting Brake Output for CTA
 * @return(void)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4052}
 * @verification{}
 */
static void Cta_Set_Braking_and_Acceleration(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                             const Cta_Input_T *p_cta_input,
                                             Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                             const Ctb_State_Output_T *p_cta_states,
                                             const boolean_T cta_braking_state,
                                             const Cta_Instance_T *p_cta_instance);

/**
 * @brief Setting Mirror Warning Outputs for CTA
 * @return(void)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4052}
 * @verification{}
 */
static void Cta_Set_Mirror_Warning(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                   Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                   const Ctb_State_Output_T *p_cta_states);
/**
 * @brief Setting Display_Graphic_Warning output for CTA
 * @return(void)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4053}
 * @verification{}
 */
static Bmw_Ctb_Display_Warning_Graphically_T
Cta_Map_Display_Graphic_Warning_From_Algo_To_Bsw_Output_Display_Graphic_Warning(const uint8_t algo_display_warning_graphically);
/**
 * @brief Setting Warning output for CTA
 * @return(void)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4053}
 * @verification{}
 */
static Bmw_Output_Ctb_Warning_T Cta_Map_Ctb_Warning_From_Algo_To_Bsw_Output_Ctb_Warning(const uint8_t algo_warning_status);
/**
 * @brief Setting CTB Acostics Warning Information with EarconID,horizonatal angle
 * @return(void)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4053}
 * @verification{}
 */
static void Cta_Set_Output_Ctb_Acoustics_Warning(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                                 const Bmw_Output_Ctb_Warning_T ctb_acoustic_warning);
/**
 * @brief Setting Banner Check Flag
 * @return(boolean_T)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4052}
 * @verification{}
 */
static boolean_T Cta_Set_Banner_Check_Flag(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                           const Cta_Input_T *p_cta_input,
                                           const Cta_Instance_T *p_cta_instance);

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Bmw_Output(const Cta_Output_T *p_CTA_output);
#define Binary_Write_Cta_Customer_Output(p_cta_output) Write_Debug_Cta_Bmw_Output(p_cta_output)
#else
#define Binary_Write_Cta_Customer_Output(p_cta_output)
#endif /* BINARY_DEBUG */
/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run_Init(Cta_Instance_T *p_cta_instance)
{
   assert(NULL != p_cta_instance);

   /* Reset static variables of postrun */
   Current_Ctb_Brake_Deceleration = FBK_ZERO_F;
}

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
{
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_cta_instance->core_input.p_pa_data->vehicle_data;
   uint8_t ctb_mode;

   /* Reset the complete output struct */
   Cta_Reset_Output(p_cta_output);

   /* Get relevant index for front and rear CTB */
   ctb_mode = Cta_Get_Variant_Position(p_vehicle_data, &p_cta_instance->calibration);

   /* Set new internal (Algo-Post-Run) CTB output in case that a specific mode is given. */
   if ((uint8_t) CTA_NUM_MODES != ctb_mode)
   {
      Cta_Set_Output_Algo_State(&(p_cta_output->bmw_ctb_output_algo_state), p_cta_input, p_cta_instance, p_vehicle_data, ctb_mode);
   }
   Cta_Set_Output_Edr(&(p_cta_output->bmw_ctb_output_edr));

   /* Set external (Statemachine-Post-Run) CTB output */
   Cta_Set_Output_Bus_Signals(&(p_cta_output->bmw_ctb_output_bus_signals), &(p_cta_output->bmw_ctb_output_algo_state), p_cta_input,
                              p_cta_instance);

   Binary_Write_Cta_Customer_Output(p_cta_output);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Cta_Set_Output_Bus_Signals(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                       const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                       const Cta_Input_T *p_cta_input,
                                       const Cta_Instance_T *p_cta_instance)
{
   boolean_T cta_banner_check;
   uint8_t display_warning_graphically;
   uint8_t ctb_acute_warning;
   uint8_t ctb_warning;
   uint8_t warning_acoustics;
   /*Get the Pre Run State Machine Outputs*/
   const Ctb_State_Output_T *p_cta_status = Cta_Get_State_Output_Ptr();

   /* Set the Main CTB Qualifier (This normally should be set by SM-Core-Output) */
   p_bus_signals->qualifier_function_ctb = *p_cta_status;

   /*Setting the outputs when state is not ACTIVE*/
   if ((CTB_STATE_FCTA_ACTIVE != p_bus_signals->qualifier_function_ctb)
       && (CTB_STATE_RCTA_ACTIVE != p_bus_signals->qualifier_function_ctb))
   {
      Cta_Set_Outputs_Not_Active_State(p_bus_signals);
   }
   else
   {
      /*Setting the Graphical Warning Bitfields*/
      display_warning_graphically =
         Cta_Get_Alert_Position_Encoding(p_algo_state->display_warning_graphically, p_bus_signals->qualifier_function_ctb);
      p_bus_signals->display_warning_graphically =
         Cta_Map_Display_Graphic_Warning_From_Algo_To_Bsw_Output_Display_Graphic_Warning(display_warning_graphically);
      /*Setting the Acute Warning Bitfields*/
      ctb_acute_warning = Cta_Get_Alert_Position_Encoding(p_algo_state->ctb_acute_warning, p_bus_signals->qualifier_function_ctb);
      p_bus_signals->ctb_acute_warning = Cta_Map_Ctb_Warning_From_Algo_To_Bsw_Output_Ctb_Warning(ctb_acute_warning);
      /*Setting the Acoustic Warning Bitfields*/
      warning_acoustics = Cta_Get_Alert_Position_Encoding(p_algo_state->warning_acoustics, p_bus_signals->qualifier_function_ctb);
      p_bus_signals->warning_acoustics = Cta_Map_Ctb_Warning_From_Algo_To_Bsw_Output_Ctb_Warning(warning_acoustics);
      Cta_Set_Output_Ctb_Acoustics_Warning(p_bus_signals, p_bus_signals->warning_acoustics);
      /*Setting the CTB Warning Bitfields*/
      ctb_warning = Cta_Get_Alert_Position_Encoding(p_algo_state->ctb_warning, p_bus_signals->qualifier_function_ctb);
      p_bus_signals->ctb_warning = Cta_Map_Ctb_Warning_From_Algo_To_Bsw_Output_Ctb_Warning(ctb_warning);
      /* Set the tMirror warning output*/
      Cta_Set_Mirror_Warning(p_algo_state, p_bus_signals, p_cta_status);
   }

   cta_banner_check = Cta_Set_Banner_Check_Flag(p_algo_state, p_cta_input, p_cta_instance);

   /* Setting the ctb_braking flag output based on the State Machine Outputs */
   Cta_Set_Braking_and_Acceleration(p_algo_state, p_cta_input, p_bus_signals, p_cta_status, cta_banner_check, p_cta_instance);
   /*Merging Sub States with State Outputs */
   Cta_Merge_Sub_States(p_algo_state, p_cta_input, p_bus_signals, p_cta_status, cta_banner_check, p_cta_instance);
}
static void Cta_Set_Output_Ctb_Acoustics_Warning(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                                 const Bmw_Output_Ctb_Warning_T ctb_acoustic_warning)
{
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_earconId        = CTA_ACOSTICS_WARNING_EARCON_ID;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_replicasCount   = FBK_ONE_INT;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_horizontalAngle = FBK_ZERO_F;
   switch (ctb_acoustic_warning)
   {
      case BMW_OUTPUT_CTB_WARNING_AT_FRONT_LEFT:
         p_bus_signals->Bmw_ctb_warning_acoustics.ctb_horizontalAngle = CTA_WARNING_FRONT_LEFT_HA;
         break;
      case BMW_OUTPUT_CTB_WARNING_AT_REAR_LEFT:
         p_bus_signals->Bmw_ctb_warning_acoustics.ctb_horizontalAngle = CTA_WARNING_RARE_LEFT_HA;
         break;
      case BMW_OUTPUT_CTB_WARNING_AT_REAR_RIGHT:
         p_bus_signals->Bmw_ctb_warning_acoustics.ctb_horizontalAngle = CTA_WARNING_RARE_RIGHT_HA;
         break;
      case BMW_OUTPUT_CTB_WARNING_AT_FRONT_RIGHT:
         p_bus_signals->Bmw_ctb_warning_acoustics.ctb_horizontalAngle = CTA_WARNING_FRONT_RIGHT_HA;
         break;
      default:
         p_bus_signals->Bmw_ctb_warning_acoustics.ctb_replicasCount = FBK_ZERO_INT;
         p_bus_signals->Bmw_ctb_warning_acoustics.ctb_earconId      = FBK_ZERO_UINT;
         break;
   }
}
static Bmw_Ctb_Display_Warning_Graphically_T
Cta_Map_Display_Graphic_Warning_From_Algo_To_Bsw_Output_Display_Graphic_Warning(const uint8_t algo_display_warning_graphically)
{
   Bmw_Ctb_Display_Warning_Graphically_T display_warning_graphically;
   switch (algo_display_warning_graphically)
   {
      case 0u:
         display_warning_graphically = CTB_NO_WARNING;
         break;
      case 1u:
         display_warning_graphically = CTB_WARNING_FRONT_RIGHT;
         break;
      case 2u:
         display_warning_graphically = CTB_WARNING_FRONT_LEFT;
         break;
      case 3u:
         display_warning_graphically = CTB_WARNING_FRONT_RIGHT_AND_LEFT;
         break;
      case 4u:
         display_warning_graphically = CTB_WARNING_BACK_RIGHT;
         break;
      case 8u:
         display_warning_graphically = CTB_WARNING_BACK_LEFT;
         break;
      case 12u:
         display_warning_graphically = CTB_WARNING_BACK_RIGHT_AND_LEFT;
         break;
      default:
         display_warning_graphically = CTB_SIGNAL_UNFILLED;
         break;
   }
   return display_warning_graphically;
}
static Bmw_Output_Ctb_Warning_T Cta_Map_Ctb_Warning_From_Algo_To_Bsw_Output_Ctb_Warning(const uint8_t algo_warning_status)
{
   Bmw_Output_Ctb_Warning_T warning_status;
   switch (algo_warning_status)
   {
      case 0u:
         warning_status = BMW_OUTPUT_CTB_NO_WARNING;
         break;
      case 1u:
         warning_status = BMW_OUTPUT_CTB_WARNING_AT_FRONT_RIGHT;
         break;
      case 2u:
         warning_status = BMW_OUTPUT_CTB_WARNING_AT_FRONT_LEFT;
         break;
      case 4u:
         warning_status = BMW_OUTPUT_CTB_WARNING_AT_REAR_RIGHT;
         break;
      case 8u:
         warning_status = BMW_OUTPUT_CTB_WARNING_AT_REAR_LEFT;
         break;
      default:
         warning_status = BMW_OUTPUT_CTB_SIGNAL_UNFILLED;
         break;
   }
   return warning_status;
}
static void Cta_Set_Mirror_Warning(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                   Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                   const Ctb_State_Output_T *p_cta_states)
{
   if (CTB_STATE_RCTA_ACTIVE == *p_cta_states)
   {
      /* Set No ExtMirror warning output in RCTA ACTIVE state*/
      p_bus_signals->request_extmirror_warning_left  = p_algo_state->request_extmirror_warning[BMW_CTB_ALERT_SIDE_LEFT];
      p_bus_signals->request_extmirror_warning_right = p_algo_state->request_extmirror_warning[BMW_CTB_ALERT_SIDE_RIGHT];
   }
   else
   {
      if (CTB_STATE_FCTA_ACTIVE == *p_cta_states)
      {
         /* Set No ExtMirror warning output in FCTA ACTIVE state*/
         p_bus_signals->request_extmirror_warning_left  = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
         p_bus_signals->request_extmirror_warning_right = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
      }
   }
}
static void Cta_Set_Braking_and_Acceleration(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                             const Cta_Input_T *p_cta_input,
                                             Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                             const Ctb_State_Output_T *p_cta_states,
                                             const boolean_T cta_braking_state,
                                             const Cta_Instance_T *p_cta_instance)
{
   float32_T cta_sample_time = p_cta_instance->core_input.p_pa_data->time_diff_to_last_cycle;
   /*Set Braking and acceleration for RCTA */
   if ((CTB_STATE_RCTA_ACTIVE == *p_cta_states) && (p_algo_state->status_brake_requirement == BMW_CTB_STATUS_BRAKE_REQ_BRAKING)
       && (BMW_CTB_VARIANT_NO_CTB != p_cta_input->bmw_ctb_coding_parameters.c_ctb_variant) && Fbk_Is_True(cta_braking_state)
       && (Cta_Banner_Time_Inc_Rcta <= p_cta_instance->customer_calibration.k_bmw_sp25_banner_time))
   {
      /*Braking at Rear Set*/
      p_bus_signals->ctb_braking                      = BMW_CTB_BRAKING_AT_REAR;
      p_bus_signals->target_longitudinal_acceleration = p_algo_state->target_longitudinal_acceleration;
      p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_IMPLEMENTED;
      /*Increment the Banner Time if braking is set.To limit the time the brake is applied*/
      Cta_Banner_Time_Inc_Rcta += cta_sample_time;
      Cta_Banner_Time_Inc_Rcta = Min(Cta_Banner_Time_Inc_Rcta, CTA_MAX_FLOAT_TIME);
   }
   /*Set Braking and acceleration for FCTA */
   else if ((CTB_STATE_FCTA_ACTIVE == *p_cta_states) && (p_algo_state->status_brake_requirement == BMW_CTB_STATUS_BRAKE_REQ_BRAKING)
            && (BMW_CTB_VARIANT_CTB_REAR_AND_FRONT == p_cta_input->bmw_ctb_coding_parameters.c_ctb_variant)
            && Fbk_Is_True(cta_braking_state)
            && (Cta_Banner_Time_Inc_Fcta <= p_cta_instance->customer_calibration.k_bmw_sp25_banner_time))
   {
      /*Braking at Front Set*/
      p_bus_signals->ctb_braking                      = BMW_CTB_BRAKING_AT_FRONT;
      p_bus_signals->target_longitudinal_acceleration = p_algo_state->target_longitudinal_acceleration;
      p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_IMPLEMENTED;
      /*Increment the Banner Time if braking is set.To limit the time the brake is applied*/
      Cta_Banner_Time_Inc_Fcta += cta_sample_time;
      Cta_Banner_Time_Inc_Fcta = Min(Cta_Banner_Time_Inc_Fcta, CTA_MAX_FLOAT_TIME);
   }
   /*Set Braking and acceleration default */
   else
   {
      p_bus_signals->ctb_braking                      = BMW_CTB_NO_BRAKING;
      p_bus_signals->target_longitudinal_acceleration = FBK_ZERO_F;
      p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_NOT_IMPLEMENTED;
      /* Reset the banner time increment when status brake requirement is FALSE*/
      if ((p_algo_state->status_brake_requirement != BMW_CTB_STATUS_BRAKE_REQ_BRAKING))
      {
         Cta_Banner_Time_Inc_Fcta = FBK_ZERO_F;
         Cta_Banner_Time_Inc_Rcta = FBK_ZERO_F;
      }
   }
}

static boolean_T Cta_Set_Banner_Check_Flag(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                           const Cta_Input_T *p_cta_input,
                                           const Cta_Instance_T *p_cta_instance)
{
   static float32_T cta_counter_banner_check = FBK_ZERO_F;
   float32_T cta_sample_time                 = p_cta_instance->core_input.p_pa_data->time_diff_to_last_cycle;
   boolean_T cta_set_banner_check            = FBK_FALSE;
   if (Fbk_Is_True(p_cta_instance->customer_calibration.k_bmw_sp25_banner_criteria_check))
   {
      if ((CTA_ACCELERATION_PRIORITIZED == p_cta_input->bmw_ctb_input_signals.status_arbitration_longitudinal_low_integrity)
          && (CTA_FASOCLI_ACCELERATION_PRIORITIZED == p_cta_input->bmw_ctb_input_signals.status_acceleration_long_prioritization)
          && (BMW_CTB_STATUS_BRAKE_REQ_BRAKING == p_algo_state->status_brake_requirement))
      {
         cta_counter_banner_check += cta_sample_time;
         cta_counter_banner_check = Min(cta_counter_banner_check, CTA_MAX_FLOAT_TIME);
         if (cta_counter_banner_check >= p_cta_instance->customer_calibration.k_bmw_sp25_banner_criteria_check_time)
         {
            cta_set_banner_check = FBK_TRUE;
         }
      }
      else
      {
         cta_counter_banner_check = FBK_ZERO_F;
      }
   }
   else
   {
      cta_set_banner_check = FBK_TRUE;
   }
   return cta_set_banner_check;
}
static void Cta_Merge_Sub_States(const Bmw_Ctb_Output_Algo_State_T *p_algo_state,
                                 const Cta_Input_T *p_cta_input,
                                 Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals,
                                 const Ctb_State_Output_T *p_cta_states,
                                 const boolean_T cta_braking_state,
                                 const Cta_Instance_T *p_cta_instance)
{
   /*Merging ACTIVE States */
   if ((CTB_STATE_RCTA_ACTIVE == p_bus_signals->qualifier_function_ctb)
       || (CTB_STATE_FCTA_ACTIVE == p_bus_signals->qualifier_function_ctb))
   {
      if ((BMW_CTB_VARIANT_NO_CTB != p_cta_input->bmw_ctb_coding_parameters.c_ctb_variant)
          && (BMW_CTB_STATUS_BRAKE_REQ_BRAKING == p_algo_state->status_brake_requirement)
          && (CTB_STATE_RCTA_ACTIVE == *p_cta_states) && Fbk_Is_True(cta_braking_state)
          && (Cta_Banner_Time_Inc_Rcta <= p_cta_instance->customer_calibration.k_bmw_sp25_banner_time))
      {
         /*Setting Active Braking State for RCTA */
         p_bus_signals->qualifier_function_ctb = CTB_STATE_ACTIVE_BRAKING;
      }
      else if ((BMW_CTB_VARIANT_CTB_REAR_AND_FRONT == p_cta_input->bmw_ctb_coding_parameters.c_ctb_variant)
               && (BMW_CTB_STATUS_BRAKE_REQ_BRAKING == p_algo_state->status_brake_requirement)
               && (CTB_STATE_FCTA_ACTIVE == *p_cta_states) && Fbk_Is_True(cta_braking_state)
               && (Cta_Banner_Time_Inc_Fcta <= p_cta_instance->customer_calibration.k_bmw_sp25_banner_time))
      {
         /*Setting Active Braking State for FCTA */
         p_bus_signals->qualifier_function_ctb = CTB_STATE_ACTIVE_BRAKING;
      }
      else
      {
         p_bus_signals->qualifier_function_ctb = CTB_STATE_ACTIVE;
      }
   }
}
static void Cta_Set_Output_Algo_State(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output,
                                      const Cta_Input_T *p_cta_input,
                                      const Cta_Instance_T *p_cta_instance,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const uint8_t ctb_mode)
{
   /* Evaluate in which Warning-State the system shall be in */
   p_algo_state_output->warn_control_output_state[BMW_CTB_ALERT_SIDE_LEFT] = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(
      p_cta_input, &(p_cta_instance->core_output.cta_alert_level[ctb_mode][FBK_SIDE_LEFT]), p_vehicle_data);
   p_algo_state_output->warn_control_output_state[BMW_CTB_ALERT_SIDE_RIGHT] = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(
      p_cta_input, &(p_cta_instance->core_output.cta_alert_level[ctb_mode][FBK_SIDE_RIGHT]), p_vehicle_data);

   if (ctb_mode == (uint8_t) CTA_MODE_FRONT)
   {
      p_algo_state_output->unique_id[0] = p_cta_instance->core_output.cta_unique_id[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_algo_state_output->unique_id[1] = p_cta_instance->core_output.cta_unique_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
   }
   else
   {
      p_algo_state_output->unique_id[2] = p_cta_instance->core_output.cta_unique_id[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_algo_state_output->unique_id[3] = p_cta_instance->core_output.cta_unique_id[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   }

   /* Set the Algo internal BMW specific Warning outputs */
   /* Do not yet prevent Acoustic warning on both sides (This shall be ensured later when setting the bit-field) */
   Cta_Set_Warning_Outputs(p_algo_state_output, BMW_CTB_ALERT_SIDE_LEFT);
   Cta_Set_Warning_Outputs(p_algo_state_output, BMW_CTB_ALERT_SIDE_RIGHT);

   /* Set the Algo internal BMW specific Braking outputs */
   Cta_Set_Braking_Outputs(p_algo_state_output, p_cta_input, p_cta_instance, ctb_mode);
}

static void Cta_Set_Output_Edr(Bmw_Ctb_Output_Edr_T *p_edr_output)
{
   /* TBD: Set the outputs for the Event-Data-Recorder */
   p_edr_output->bmw_ctb_ego_speed = FBK_ZERO_F;
   /*Set Acute Braking*/
   p_edr_output->bmw_ctb_f_acute_braking = FBK_ZERO_F;
   p_edr_output->bmw_ctb_f_acute_warning = FBK_ZERO_F;
   /*Set Object Kinematics*/
   p_edr_output->bmw_ctb_object_direction_of_motion_angle = FBK_ZERO_F;
   p_edr_output->bmw_ctb_object_distance_x                = FBK_ZERO_F;
   p_edr_output->bmw_ctb_object_distance_y                = FBK_ZERO_F;
   p_edr_output->bmw_ctb_object_speed                     = FBK_ZERO_F;
   /*Set Other Properties*/
   p_edr_output->bmw_ctb_timestamp = FBK_ZERO_F;
   p_edr_output->bmw_ctb_ttc       = FBK_ZERO_F;
   p_edr_output->bmw_ctb_ttp       = FBK_ZERO_F;
   p_edr_output->bmw_ctb_warning   = FBK_ZERO_F;
}

static uint8_t Cta_Get_Alert_Position_Encoding(const Bmw_Ctb_Warning_T p_ctb_warning[CTA_BMW_ALERT_POSITION_COUNT],
                                               const Ctb_State_Output_T cta_state)
{
   /* Initialize the alert encoding to zero (No alert) */
   uint8_t alert_position_encoding = CTA_BMW_BIT_POSITION_0;
   if (cta_state == CTB_STATE_FCTA_ACTIVE)
   {
      /*Bit Position 1 Set*/
      if (BMW_CTB_WARNING == p_ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT])
      {
         alert_position_encoding |= CTA_BMW_BIT_POSITION_1;
      }
      /*Bit Position 2 Set*/
      if (BMW_CTB_WARNING == p_ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_LEFT])
      {
         alert_position_encoding |= CTA_BMW_BIT_POSITION_2;
      }
   }
   if (cta_state == CTB_STATE_RCTA_ACTIVE)
   {
      /*Bit Position 3 Set*/
      if (BMW_CTB_WARNING == p_ctb_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT])
      {
         alert_position_encoding |= CTA_BMW_BIT_POSITION_3;
      }
      /*Bit Position 4 Set*/
      if (BMW_CTB_WARNING == p_ctb_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT])
      {
         alert_position_encoding |= CTA_BMW_BIT_POSITION_4;
      }
   }
   return alert_position_encoding;
}

static void Cta_Set_Braking_Outputs(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output,
                                    const Cta_Input_T *p_cta_input,
                                    const Cta_Instance_T *p_cta_instance,
                                    const uint8_t ctb_mode)
{
   /* TBD: The content of this function is mostly carry over from BMW_SP21 and only was setting a brake request when in REVERSE.
    * Please carefully check the content and adapt according BMW_SP25 needs. Currently the Braking is suppressed by not mapping the
    * algo-state braking content to bus-signals output in another function */
   boolean_T f_rctb_qualifier_left;
   boolean_T f_rctb_qualifier_right;
   boolean_T f_accelerator_pedal_overriding;
   /* Set flag for overriding of brake request by accelerator pedal gradient (Prevent braking) */
   f_accelerator_pedal_overriding = (boolean_T) (Fbk_Is_True(p_cta_instance->calibration.k_cta_f_brake_overriding_ctb)
                                                 && (p_cta_input->bmw_ctb_input_signals.gradient_angle_acceleratorpedal
                                                     >= p_cta_instance->calibration.k_cta_accelerationpedal_gradient_threshold_ctb));

   /* Fill output for targets approaching from left side*/
   f_rctb_qualifier_left = Cta_Check_Rctb_Qualifier(&p_cta_instance->core_output, FBK_SIDE_LEFT, ctb_mode);

   /* Fill output for targets approaching from right side*/
   f_rctb_qualifier_right = Cta_Check_Rctb_Qualifier(&p_cta_instance->core_output, FBK_SIDE_RIGHT, ctb_mode);

   /* Set the braking signals */
   if ((f_rctb_qualifier_left || f_rctb_qualifier_right) && (Fbk_Is_False(f_accelerator_pedal_overriding)))
   {
      p_algo_state_output->status_brake_requirement = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
      Current_Ctb_Brake_Deceleration =
         Cta_Get_Braking_Acceleration(p_cta_instance->core_input.p_pa_data, &p_cta_instance->calibration);
      p_algo_state_output->target_longitudinal_acceleration = Current_Ctb_Brake_Deceleration;
   }
   else
   {
      /* Reset the complete braking output to No-Braking */
      p_algo_state_output->ctb_braking                      = BMW_CTB_NO_BRAKING;
      p_algo_state_output->status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING;
      Current_Ctb_Brake_Deceleration                        = FBK_ZERO_F;
      p_algo_state_output->target_longitudinal_acceleration = Current_Ctb_Brake_Deceleration;
   }
}

static void Cta_Set_Warning_Outputs(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output, Bmw_Ctb_Alert_Side_T side)
{

   Bmw_Ctb_Alert_Position_T alert_position_front;
   Bmw_Ctb_Alert_Position_T alert_position_rear;
   /*Left Warnings*/
   if (BMW_CTB_ALERT_SIDE_LEFT == side)
   {
      alert_position_front = BMW_CTB_ALERT_POSITION_FRONT_LEFT;
      alert_position_rear  = BMW_CTB_ALERT_POSITION_REAR_LEFT;
   }
   else /* Right side (This condition will be entered as well for the case that no valid side was given) */
   {
      alert_position_front = BMW_CTB_ALERT_POSITION_FRONT_RIGHT;
      alert_position_rear  = BMW_CTB_ALERT_POSITION_REAR_RIGHT;
   }

   /* Set the Warning flags according to the current state */
   switch (p_algo_state_output->warn_control_output_state[side])
   {
      case BMW_CTB_WARN_STATE_NO_WARNING:
         /* In case of No-Warning, none of the outputs need to be changed after the previous reset! */
         break;
         /*Info Warning Front and Rear*/
      case BMW_CTB_WARN_STATE_INFO_WARNING_FRONT:
         p_algo_state_output->display_warning_graphically[alert_position_front] = BMW_CTB_WARNING;
         p_algo_state_output->ctb_warning[alert_position_front]                 = BMW_CTB_WARNING;
         break;
      case BMW_CTB_WARN_STATE_INFO_WARNING_REAR:
         p_algo_state_output->display_warning_graphically[alert_position_rear] = BMW_CTB_WARNING;
         p_algo_state_output->ctb_warning[alert_position_rear]                 = BMW_CTB_WARNING;
         break;
         /*Acute Warning Front*/
      case BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT:
         p_algo_state_output->display_warning_graphically[alert_position_front] = BMW_CTB_WARNING;
         p_algo_state_output->warning_acoustics[alert_position_front]           = BMW_CTB_WARNING;
         p_algo_state_output->ctb_acute_warning[alert_position_front]           = BMW_CTB_WARNING;
         break;
         /*Acute Warning Rear*/
      case BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR:
         p_algo_state_output->request_extmirror_warning[side] = BMW_CTB_REQUEST_MIRROR_DISPLAY_SEGMENT_ON_FLASHING_LEVEL_1;
         p_algo_state_output->display_warning_graphically[alert_position_rear] = BMW_CTB_WARNING;
         p_algo_state_output->warning_acoustics[alert_position_rear]           = BMW_CTB_WARNING;
         p_algo_state_output->ctb_acute_warning[alert_position_rear]           = BMW_CTB_WARNING;
         break;
      default:
         /* No other states available -> Nothing to do! */
         break;
   }
}

static void Cta_Reset_Algo_State_Output(Bmw_Ctb_Output_Algo_State_T *p_algo_state_output)
{
   /*Reset CTB warning*/
   p_algo_state_output->ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_LEFT]  = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT]   = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT]  = BMW_CTB_NO_WARNING;
   /*Reset Acoustic warning*/
   p_algo_state_output->warning_acoustics[BMW_CTB_ALERT_POSITION_FRONT_LEFT]  = BMW_CTB_NO_WARNING;
   p_algo_state_output->warning_acoustics[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_NO_WARNING;
   p_algo_state_output->warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_LEFT]   = BMW_CTB_NO_WARNING;
   p_algo_state_output->warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_RIGHT]  = BMW_CTB_NO_WARNING;
   /*Graphical Warning*/
   p_algo_state_output->display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_LEFT]  = BMW_CTB_NO_WARNING;
   p_algo_state_output->display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_NO_WARNING;
   p_algo_state_output->display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_LEFT]   = BMW_CTB_NO_WARNING;
   p_algo_state_output->display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_RIGHT]  = BMW_CTB_NO_WARNING;
   /*No warning*/
   p_algo_state_output->ctb_acute_warning[BMW_CTB_ALERT_POSITION_FRONT_LEFT]  = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_acute_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_acute_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT]   = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_acute_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT]  = BMW_CTB_NO_WARNING;
   p_algo_state_output->ctb_braking                                           = BMW_CTB_NO_BRAKING;
   /*Mirror Warning*/
   p_algo_state_output->request_extmirror_warning[BMW_CTB_ALERT_SIDE_LEFT]  = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
   p_algo_state_output->request_extmirror_warning[BMW_CTB_ALERT_SIDE_RIGHT] = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
   /*Brake Requirement Status*/
   p_algo_state_output->status_brake_requirement = BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING;
   /*Longitudinal Acceleration Target*/
   p_algo_state_output->target_longitudinal_acceleration = FBK_ZERO_F;
}

static void Cta_Reset_Bus_Signals_Output(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals)
{
   /*Reset Warnings*/
   p_bus_signals->ctb_acute_warning           = BMW_OUTPUT_CTB_SIGNAL_UNFILLED;
   p_bus_signals->ctb_braking                 = BMW_CTB_BRAKING_SIGNAL_UNFILLED;
   p_bus_signals->ctb_warning                 = BMW_OUTPUT_CTB_SIGNAL_UNFILLED;
   p_bus_signals->display_warning_graphically = CTB_SIGNAL_UNFILLED;
   /*Reset Qualifier*/
   p_bus_signals->qualifier_function_ctb           = CTB_STATE_NOT_AVAILABLE;
   p_bus_signals->request_extmirror_warning_left   = BMW_CTB_REQUEST_MIRROR_SIGNAL_UNFILLED;
   p_bus_signals->request_extmirror_warning_right  = BMW_CTB_REQUEST_MIRROR_SIGNAL_UNFILLED;
   p_bus_signals->status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING;
   p_bus_signals->target_longitudinal_acceleration = FBK_ZERO_F;
   p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_NOT_AVAILABLE;
   /*Reset warning_acoustics Properties*/
   p_bus_signals->warning_acoustics                              = BMW_OUTPUT_CTB_SIGNAL_UNFILLED;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_earconId         = FBK_ZERO_UINT;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_handleID         = FBK_ZERO_UINT;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_period           = FBK_ZERO_UINT;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_presentationTime = FBK_ZERO_UINT;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_replicasCount    = FBK_ZERO_UINT;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_verticalAngle    = FBK_ZERO_F;
   p_bus_signals->Bmw_ctb_warning_acoustics.ctb_horizontalAngle  = FBK_ZERO_F;
   /*Reset rctb and fctb static counters*/
   Cta_Banner_Time_Inc_Rcta = FBK_ZERO_F;
   Cta_Banner_Time_Inc_Fcta = FBK_ZERO_F;
   /*Reset ctb_error_status*/
   p_bus_signals->Bmw_Ctb_error_status = (boolean_T) FBK_FALSE;
}

static void Cta_Reset_Edr_Signals_Output(Bmw_Ctb_Output_Edr_T *p_edr_output)
{
   /*Set Ego Speed*/
   p_edr_output->bmw_ctb_ego_speed = FBK_ZERO_F;
   /*Set Acute Braking */
   p_edr_output->bmw_ctb_f_acute_braking = FBK_ZERO_F;
   p_edr_output->bmw_ctb_f_acute_warning = FBK_ZERO_F;
   /*Set Object Kinematics*/
   p_edr_output->bmw_ctb_object_direction_of_motion_angle = FBK_ZERO_F;
   p_edr_output->bmw_ctb_object_distance_x                = FBK_ZERO_F;
   p_edr_output->bmw_ctb_object_distance_y                = FBK_ZERO_F;
   p_edr_output->bmw_ctb_object_speed                     = FBK_ZERO_F;
   /*Set Other Properties*/
   p_edr_output->bmw_ctb_timestamp = FBK_ZERO_F;
   p_edr_output->bmw_ctb_ttc       = FBK_ZERO_F;
   p_edr_output->bmw_ctb_ttp       = FBK_ZERO_F;
   p_edr_output->bmw_ctb_warning   = FBK_ZERO_F;
}

static void Cta_Reset_Output(Cta_Output_T *p_cta_output)
{

   p_cta_output->bmw_ctb_output_algo_state.unique_id[0] = PA_INVALID_OBJ_ID;
   p_cta_output->bmw_ctb_output_algo_state.unique_id[1] = PA_INVALID_OBJ_ID;
   p_cta_output->bmw_ctb_output_algo_state.unique_id[2] = PA_INVALID_OBJ_ID;
   p_cta_output->bmw_ctb_output_algo_state.unique_id[3] = PA_INVALID_OBJ_ID;

   /* Reset the internal BMW Active state output */
   Cta_Reset_Algo_State_Output(&(p_cta_output->bmw_ctb_output_algo_state));

   /* Reset the BMW final Statemachine output */
   Cta_Reset_Bus_Signals_Output(&(p_cta_output->bmw_ctb_output_bus_signals));

   /* Reset the BMW EventDataRecorder output */
   Cta_Reset_Edr_Signals_Output(&(p_cta_output->bmw_ctb_output_edr));
}

static boolean_T Cta_Check_Rctb_Qualifier(const Cta_Core_Output_T *p_cta_core_output, const uint8_t approach_side, const uint8_t ctb_mode)
{
   boolean_T f_ctb_qualifier = FBK_FALSE;

   assert(NULL != p_cta_core_output);

   /* Check whether alert is active for the respective side and if the brake qualifier is set*/
   if (Fbk_Is_True(p_cta_core_output->cta_alert_level[ctb_mode][approach_side] > CTA_CRIT_LEVEL_1) /* Check if braking-alert-level
                                                                                                      is reached */
       && Fbk_Is_True(p_cta_core_output->f_brake_qualifier[ctb_mode][approach_side]) /* Check if brake-qualifier is active */)
   {
      f_ctb_qualifier = FBK_TRUE;
   }
   else
   {
      /* Do nothing */
   }

   return f_ctb_qualifier;
}

static float32_T Cta_Get_Braking_Acceleration(const Pa_Data_T *p_pa_data, const Cta_Core_Calibration_T *p_cta_cal)
{
   float32_T rctb_brake_request;
   /*Calculate Brake Deceleration*/
   if (p_cta_cal->k_ctb_time_to_ask_for_final_brake_decel > EPSILON)
   {
      rctb_brake_request = Current_Ctb_Brake_Deceleration
                           - (p_cta_cal->k_ctb_const_decel_after_ramp_in
                              / (p_cta_cal->k_ctb_time_to_ask_for_final_brake_decel / p_pa_data->time_diff_to_last_cycle));
   }
   else
   {
      /*Calculate Brake Deceleration Alternate*/
      rctb_brake_request = Current_Ctb_Brake_Deceleration - p_cta_cal->k_ctb_const_decel_after_ramp_in;
   }
   /*Enforce max acceleration*/
   rctb_brake_request = Enforce_Range(rctb_brake_request, -p_cta_cal->k_ctb_const_decel_after_ramp_in, FBK_ZERO_F);

   return rctb_brake_request;
}

static uint8_t Cta_Get_Variant_Position(const Fbk_Vehicle_Data_T *p_vehicle_data, const Cta_Core_Calibration_T *p_cta_cal)
{
   /* Initialize the return value to the num of modes as default value.*/
   uint8_t position = (uint8_t) CTA_NUM_MODES;

   /* Depending of the DEBUG_MODE calibration the input signal radar position can be set based on cal_parameter. */
   if (Fbk_Is_False(p_cta_cal->k_cta_DEBUG_MODE))
   {
      if ((PA_VEH_PRNDL_STATE_DRIVE == p_vehicle_data->prndl))
      {
         position = (uint8_t) CTA_MODE_FRONT;
      }
      else if ((PA_VEH_PRNDL_STATE_REVERSE == p_vehicle_data->prndl))
      {
         position = (uint8_t) CTA_MODE_REAR;
      }
      else
      {
         /* Do nothing */
      }
   }
   else if (Fbk_Is_True(p_cta_cal->k_cta_DEBUG_MODE))
   {
      /*Debug Mode*/
      if (Fbk_Is_True(p_cta_cal->k_cta_enable_modes[CTA_MODE_FRONT]) && Fbk_Is_False(p_cta_cal->k_cta_enable_modes[CTA_MODE_REAR]))
      {
         position = (uint8_t) CTA_MODE_FRONT;
      }
      else if (Fbk_Is_False(p_cta_cal->k_cta_enable_modes[CTA_MODE_FRONT])
               && Fbk_Is_True(p_cta_cal->k_cta_enable_modes[CTA_MODE_REAR]))
      {
         position = (uint8_t) CTA_MODE_REAR;
      }
      else
      {
         /* Do nothing */
      }
   }
   else
   {
      /* Do nothing */
   }

   return position;
}

static void Cta_Set_Outputs_Not_Active_State(Bmw_Ctb_Output_Bus_Signals_T *p_bus_signals)
{
   p_bus_signals->warning_acoustics = BMW_OUTPUT_CTB_NO_WARNING;
   /* Set the Warning flags according to the current state */
   switch (p_bus_signals->qualifier_function_ctb)
   {
      case CTB_STATE_NOT_AVAILABLE:
         p_bus_signals->display_warning_graphically      = CTB_NO_WARNING;
         p_bus_signals->request_extmirror_warning_left   = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
         p_bus_signals->request_extmirror_warning_right  = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
         p_bus_signals->target_longitudinal_acceleration = FBK_ZERO_F; // Yet to update enum Value not availble
         p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_NOT_AVAILABLE;
         break;
      case CTB_STATE_READY:
         p_bus_signals->display_warning_graphically      = CTB_NO_WARNING;
         p_bus_signals->request_extmirror_warning_left   = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
         p_bus_signals->request_extmirror_warning_right  = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
         p_bus_signals->target_longitudinal_acceleration = FBK_ZERO_F;
         p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_NOT_IMPLEMENTED;
         break;
      case CTB_STATE_ERROR:
         /*Set No Mirror warning*/
         p_bus_signals->display_warning_graphically      = CTB_TARGETVALUE_NOT_AVAILABLE_ERROR;
         p_bus_signals->request_extmirror_warning_left   = BMW_CTB_REQUEST_MIRROR_FUNCTION_REPORTS_ERROR;
         p_bus_signals->request_extmirror_warning_right  = BMW_CTB_REQUEST_MIRROR_FUNCTION_REPORTS_ERROR;
         p_bus_signals->target_longitudinal_acceleration = FBK_ZERO_F; // Yet to update enum Value not availble
         p_bus_signals->ctb_status_braking_request       = BMW_CTB_TARGET_VALUE_NOT_AVAILABLE_ERROR;
         p_bus_signals->Bmw_Ctb_error_status             = (boolean_T) FBK_TRUE;
         break;
      default:
         /* No other states available -> Nothing to do! */
         break;
   }
}
#ifdef BINARY_DEBUG

static void Write_Debug_Cta_Bmw_Output(const Cta_Output_T *p_cta_output)
{
   uint8_t i;

   /* properties of the final BUS-SIGNALS output */
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_ctb_acute_warning", p_cta_output->bmw_ctb_output_bus_signals.ctb_acute_warning);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_ctb_braking", p_cta_output->bmw_ctb_output_bus_signals.ctb_braking);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_ctb_warning", p_cta_output->bmw_ctb_output_bus_signals.ctb_warning);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_display_warning_graphically",
                         p_cta_output->bmw_ctb_output_bus_signals.display_warning_graphically);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_qualifier_function_ctb",
                         p_cta_output->bmw_ctb_output_bus_signals.qualifier_function_ctb);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_request_extmirror_warning_left",
                         p_cta_output->bmw_ctb_output_bus_signals.request_extmirror_warning_left);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_request_extmirror_warning_right",
                         p_cta_output->bmw_ctb_output_bus_signals.request_extmirror_warning_right);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_status_brake_requirement",
                         p_cta_output->bmw_ctb_output_bus_signals.status_brake_requirement);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_target_longitudinal_acceleration",
                         p_cta_output->bmw_ctb_output_bus_signals.target_longitudinal_acceleration);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_bus_signals_warning_acoustics", p_cta_output->bmw_ctb_output_bus_signals.warning_acoustics);

   /* properties of the internal ALGO-STATE */
   for (i = FBK_ZERO_UINT; i < CTA_BMW_ALERT_POSITION_COUNT; i++)
   {
      CTA_STORE_ARRAY_ELEM_MGR_WPR("bmw_sp25_ctb_algo_state_ctb_acute_warning",
                                   p_cta_output->bmw_ctb_output_algo_state.ctb_acute_warning[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("bmw_sp25_ctb_algo_state_ctb_warning", p_cta_output->bmw_ctb_output_algo_state.ctb_warning[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("bmw_sp25_ctb_algo_state_display_warning_graphically",
                                   p_cta_output->bmw_ctb_output_algo_state.display_warning_graphically[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("bmw_sp25_ctb_algo_state_warning_acoustics",
                                   p_cta_output->bmw_ctb_output_algo_state.warning_acoustics[i], i);
   }

   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_algo_state_ctb_braking", (uint8_t) p_cta_output->bmw_ctb_output_algo_state.ctb_braking);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_algo_state_status_brake_requirement",
                         (uint8_t) p_cta_output->bmw_ctb_output_algo_state.status_brake_requirement);
   CTA_STORE_VAL_MGR_WPR("bmw_sp25_ctb_algo_state_target_longitudinal_acceleration",
                         p_cta_output->bmw_ctb_output_algo_state.target_longitudinal_acceleration);

   for (i = FBK_ZERO_UINT; i < CTA_BMW_SIDE_COUNT; i++)
   {
      CTA_STORE_ARRAY_ELEM_MGR_WPR("bmw_sp25_ctb_algo_state_request_extmirror_warning",
                                   (uint8_t) p_cta_output->bmw_ctb_output_algo_state.request_extmirror_warning[i], i);
      CTA_STORE_ARRAY_ELEM_MGR_WPR("bmw_sp25_ctb_algo_state_warn_control_output_state",
                                   (uint8_t) p_cta_output->bmw_ctb_output_algo_state.warn_control_output_state[i], i);
   }
}

#endif /* BINARY_DEBUG */
