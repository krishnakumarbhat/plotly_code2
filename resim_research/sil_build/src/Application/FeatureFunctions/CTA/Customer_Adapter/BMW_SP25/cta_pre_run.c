/**
 * @file cta_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 pre run logic for CTA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
#include "cta_pre_run.h"            // for Cta_Pre_Run, Cta_Pre_Run_Init
#include "cta_bmw_sp25_types.h"     // for Bmw_Ctb_Input_Bus_Signals_T, Bmw...
#include "cta_core_calibration_t.h" // for Cta_Core_Calibration_T
#include "cta_core_input_t.h"       // for Cta_Core_Input_T
#include "cta_state_machine.h"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h" // for Fbk_Vehicle_Data_T
#include "ml_trigonometry.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>

static Ctb_State_Output_T Ctb_Current_State = CTB_STATE_NOT_AVAILABLE;

/**
 * @brief Getter function for ctb states
 *
 */
Ctb_State_Output_T *Cta_Get_State_Output_Ptr(void)
{
   return &Ctb_Current_State;
}


/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Constructs the CTA zone and fills it into the CTA core input.
 *
 * @return void
 *
 * @SRS{SF-210}
 * @SAE{}
 * @SDD{SF-3956}
 * @verification{}
 */
static void Cta_Construct_Zone(Fbk_Field_Of_Interest_T *p_cta_zone /**< CTA core input of the zone */,
                               const Cta_Core_Calibration_T *p_cta_cal /**< CTA calibrations */,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Cta_Init_Input(Cta_Input_T *p_cta_input /**< CTA Input */)
{
   assert(NULL != p_cta_input);
   /* Initialize the BMW specific Bus-Signals to Standstill state */
   p_cta_input->bmw_ctb_input_signals.gradient_angle_acceleratorpedal           = FBK_ZERO_F;
   p_cta_input->bmw_ctb_input_signals.qualifier_gradient_angle_acceleratorpedal = BMW_CTB_UNFILLED;
   /*Gear Status and Vehicle Signals*/
   p_cta_input->bmw_ctb_input_signals.vehicle_moving_direction                      = BMW_CTB_VEHICLE_SIGNAL_UNFILLED;
   p_cta_input->bmw_ctb_input_signals.status_arbitration_longitudinal_low_integrity = CTA_ACCELERATION_PRIORITIZED;
   p_cta_input->bmw_ctb_input_signals.status_acceleration_long_prioritization       = CTA_FASOCLI_ACCELERATION_PRIORITIZED;
   /*Trailer Status*/
   p_cta_input->bmw_ctb_input_signals.status_trailer                           = BMW_CTB_NO_TRAILER_AVAILABLE;
   p_cta_input->bmw_ctb_input_signals.parking_context_active                   = FBK_TRUE;
   p_cta_input->bmw_ctb_input_signals.control_cross_traffic_alert_front        = FBK_TRUE;
   p_cta_input->bmw_ctb_input_signals.control_cross_traffic_alert_rear         = FBK_TRUE;
   p_cta_input->bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = FBK_TRUE;
   /*Ctb Braking Variant Selection*/
   p_cta_input->bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_NOT_CONFIGURABLE;
   p_cta_input->bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_NOT_CONFIGURABLE;
   /*Vehicle Dynamometer Status*/
   p_cta_input->bmw_ctb_input_signals.status_roller_dynamometer = BMW_CTB_SIGNAL_UNFILLED;
   p_cta_input->bmw_ctb_input_signals.status_end_of_line        = SIGNAL_UNFILLED;
   /*Qualifier Vehicle Speed*/
   p_cta_input->bmw_ctb_input_signals.qualifier_vehicle_speed = BMW_CTB_INITIALIZATION;
   /* Initialize the BMW specific Coding parameters */
   p_cta_input->bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   p_cta_input->bmw_ctb_coding_parameters.c_ctb_braking_enabled_front                        = FBK_FALSE;
   p_cta_input->bmw_ctb_coding_parameters.c_ctb_braking_enabled_rear                         = FBK_FALSE;
   p_cta_input->bmw_ctb_coding_parameters.c_ctb_enabled                                      = FBK_FALSE;
   p_cta_input->bmw_ctb_coding_parameters.c_ctb_ttc_ttp_use                                  = BMW_CTB_TTC_TTP_USE_TTC;
   p_cta_input->bmw_ctb_coding_parameters.c_ctb_variant                                      = BMW_CTB_VARIANT_NO_CTB;
   p_cta_input->bmw_ctb_coding_parameters.c_rctb_extended_pre_alert_zone                     = FBK_FALSE;
}

void Cta_Pre_Run_Init(Cta_Instance_T *p_cta_instance)
{
   uint8_t side_idx;
   uint8_t mode_idx;

   assert(NULL != p_cta_instance);

   p_cta_instance->core_input.f_cta_switch = FBK_FALSE;

   /* Initialize comparison data of CTA for all different modes and sides. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
      {
         Fbk_Reset_Object_Data(&(p_cta_instance->cta_obj_tracker_high_crit[mode_idx][side_idx]));
      }
   }
}

void Cta_Pre_Run(Cta_Instance_T *p_cta_instance,
                 const Cta_Input_T *p_cta_input,
                 const Fbk_Output_T *p_fbk_output,
                 const Pt_Output_T *p_pt_output)
{
   uint8_t mode_idx;
   uint8_t level_idx;
   Ctb_Flag_Output_T ctb_state_machine_flags;
   Ctb_Function_Error_T cta_function_errors;
   const Fbk_Vehicle_Data_T *p_vehicle_data;

   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_fbk_output);
   assert(NULL != p_pt_output);

   cta_function_errors = p_cta_input->bmw_ctb_input_signals.ctb_function_error;
   p_vehicle_data      = &p_fbk_output->p_pa_data->vehicle_data;

   /*Get Perception Data*/
   p_cta_instance->core_input.p_pa_data   = p_fbk_output->p_pa_data;
   p_cta_instance->core_input.p_pt_output = p_pt_output;

   /*Update State Machine Flags*/
   Cta_Update_Flags(p_cta_input, &p_cta_instance->calibration, &p_cta_instance->customer_calibration, &ctb_state_machine_flags,
                    p_vehicle_data);

   /*Run the CTB State Machine*/
   Cta_State_Machine(&Ctb_Current_State, &cta_function_errors, ctb_state_machine_flags);
   /*Enable Ctb based on the State Machine Outputs*/
   if ((CTB_STATE_FCTA_ACTIVE == Ctb_Current_State) || (CTB_STATE_RCTA_ACTIVE == Ctb_Current_State))
   {
      p_cta_instance->core_input.f_cta_switch = FBK_TRUE;
   }
   /*Cta switch while in debug mode*/
   if (Fbk_Is_True(p_cta_instance->calibration.k_cta_DEBUG_MODE))
   {
      if (Fbk_Is_True(p_cta_instance->calibration.k_cta_switch))
      {
         p_cta_instance->core_input.f_cta_switch = FBK_TRUE;
      }
   }

   /* Set the Core-Internal CTA-Zone-Points from the calibrations */
   Cta_Construct_Zone(&(p_cta_instance->core_input.cta_zone), &p_cta_instance->calibration, p_vehicle_data);

   /**< Map TTC thresholds to the provided ones from calibration structure */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         p_cta_instance->core_input.ttc_criticality_level[mode_idx][level_idx] =
            p_cta_instance->calibration.k_cta_ttc_criticality_level[mode_idx][level_idx];
      }
   }

   /* Set the alert suppresion mode (by TTC or TTP) */
   p_cta_instance->core_input.cta_stop_mode =
      (BMW_CTB_TTC_TTP_USE_TTP == p_cta_input->bmw_ctb_coding_parameters.c_ctb_ttc_ttp_use) ? CTA_STOP_MODE_TTP : CTA_STOP_MODE_TTC;
}
/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/
static void Cta_Construct_Zone(Fbk_Field_Of_Interest_T *p_cta_zone,
                               const Cta_Core_Calibration_T *p_cta_cal,
                               const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /*First Point*/
   p_cta_zone->points[0].x = (-1.0f * p_vehicle_data->host_length) + p_cta_cal->k_cta_butterfly_long[0u];
   p_cta_zone->points[0].y = -Fbk_Half(p_vehicle_data->host_width) + p_cta_cal->k_cta_butterfly_lat[0u];
   /*Second Point*/
   p_cta_zone->points[1].x = (-1.0f * p_vehicle_data->host_length) + p_cta_cal->k_cta_butterfly_long[1u];
   p_cta_zone->points[1].y = -Fbk_Half(p_vehicle_data->host_width) + p_cta_cal->k_cta_butterfly_lat[1u];
   /*Third Point*/
   p_cta_zone->points[2].x = (-1.0f * p_vehicle_data->host_length) + p_cta_cal->k_cta_butterfly_long[2u];
   p_cta_zone->points[2].y = p_cta_cal->k_cta_butterfly_lat[2u];
   /*Fourth Point*/
   p_cta_zone->points[3].y = p_cta_cal->k_cta_max_length_fov;
   p_cta_zone->points[3].x =
      p_cta_zone->points[2].x + (p_cta_zone->points[3].y * 1.0f / Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[1u]));
   /*Fifth Point*/
   p_cta_zone->points[4].y = p_cta_cal->k_cta_max_length_fov;
   p_cta_zone->points[4].x = p_cta_zone->points[4].y * 1.0f / Fast_Tan(p_cta_cal->k_cta_angles_zone_definition[0u]);
   /*Sixth point*/
   p_cta_zone->points[5].x = p_cta_cal->k_cta_butterfly_long[5u];
   p_cta_zone->points[5].y = p_cta_cal->k_cta_butterfly_lat[5u];
   /*Seventh point*/
   p_cta_zone->points[6].x = (-1.0f * p_vehicle_data->host_length) + p_cta_cal->k_cta_butterfly_long[6u];
   p_cta_zone->points[6].y = p_cta_cal->k_cta_butterfly_lat[6u];

   p_cta_zone->size = p_cta_cal->k_cta_amount_butterfly_points_in_use;
}
