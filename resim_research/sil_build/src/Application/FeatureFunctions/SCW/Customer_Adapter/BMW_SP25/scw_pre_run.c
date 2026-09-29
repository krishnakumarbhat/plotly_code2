/**
 * @file scw_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SP25 pre run logic for SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "scw_pre_run.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_persistent_t.h"
#include "scw_state_machine.h"
#include "scw_types.h"
#include <assert.h>

#define SCW_MIN_VEL_LOWER_LIMIT_DEFAULT (15.0f)
#define SCW_MIN_VEL_UPPER_LIMIT_DEFAULT (20.0f)
#define SCW_MAX_VEL_LOWER_LIMIT_DEFAULT (245.0f)
#define SCW_MAX_VEL_UPPER_LIMIT_DEFAULT (250.0f)
static SCW_States_T Scw_Cur_State = SCW_STATE_NOT_AVAILABLE; /*Initial State would be NOT AVAILABLE*/

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/
/**
 * @brief Getter function for ctb states
 *
 */
SCW_States_T *Scw_Get_State_Output_Ptr(void)
{
   return &Scw_Cur_State;
}

/**
 * @brief This function fills the core input guardrail data from the BMW SP25 input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2991}
 * @SDD{SF-8165}
 * @verification{}
 */
static void Scw_Set_Guardrail_Data(Scw_Instance_T *p_scw_instance /**< SCW core instance -> guardrail information */);

/**
 * @brief Helper function for Scw_Set_Guardrail_Data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2991}
 * @SDD{}
 * @verification{Create the test to check if the guardrail data is set correctly}
 */
static void Scw_Set_Guardrail_Data_Valid(Scw_Instance_T *p_scw_instance /**< SCW core instance -> guardrail information */,
                                         const Fbk_Guardrail_Data_T *p_guardrail_data /**< Guardrail data */,
                                         uint8_t side /**< Side */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Scw_Init_Input(Scw_Input_T *p_scw_input /**< SCW input */)
{
   /* Assert */
   assert(NULL != p_scw_input);

   /*BMW SP25 specific inputs*/
   /*Coding Parameters Initialization*/
   p_scw_input->scw_coding_parameters.c_scw_max_vel_lower_limit = SCW_MAX_VEL_LOWER_LIMIT_DEFAULT;
   p_scw_input->scw_coding_parameters.c_scw_min_vel_upper_limit = SCW_MIN_VEL_UPPER_LIMIT_DEFAULT;
   p_scw_input->scw_coding_parameters.c_scw_min_vel_lower_limit = SCW_MIN_VEL_LOWER_LIMIT_DEFAULT;
   p_scw_input->scw_coding_parameters.c_scw_max_vel_upper_limit = SCW_MAX_VEL_UPPER_LIMIT_DEFAULT;
   p_scw_input->scw_coding_parameters.country_variant           = SCW_BMW_COUNTRY_VARIANT_NOT_AVAILABLE;
   p_scw_input->scw_coding_parameters.c_scw_bike_carrier_mode   = FBK_FALSE;

   /*Input Parameters*/
   p_scw_input->scw_vehicle_input.vehicle_condition          = SCW_BMW_PWF_STATE_END_DRIVING_AVAILABILITY;
   p_scw_input->scw_vehicle_input.vehicle_driving_direction  = SCW_BMW_VEH_MOVING_DIR_UNFILLED;
   p_scw_input->scw_vehicle_input.vehicle_trailer_status     = SCW_BMW_NO_TRAILER_AVAILABLE;
   p_scw_input->scw_vehicle_input.vehicle_dynamometer_status = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED;
   p_scw_input->scw_vehicle_input.vehicle_end_of_line_status = SCW_BMW_STATUS_END_OF_LINE_SIGNAL_UNFILLED;

   /* Common inputs*/
   /* Enabling flags */
   p_scw_input->f_scw_enable           = FBK_ONE_UINT;
   p_scw_input->f_scw_enable_dynamic   = FBK_ONE_UINT;
   p_scw_input->f_scw_enable_guardrail = FBK_ONE_UINT;

   /* Trailer Data */
   p_scw_input->f_trailer_present = FBK_FALSE;
   p_scw_input->trailer_length    = FBK_ZERO_F;
   p_scw_input->trailer_width     = FBK_ZERO_F;
   p_scw_input->trailer_angle     = FBK_ZERO_F;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_scw_instance" points to a non-constant type.] */
void Scw_Pre_Run_Init(Scw_Instance_T *p_scw_instance)
{
   uint8_t side;

   /* Assert */
   assert(NULL != p_scw_instance);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_scw_instance->persistent.grail_lat_position[side]   = FBK_ZERO_F;
      p_scw_instance->persistent.grail_freeze_counter[side] = FBK_ZERO_UINT;
   }
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Scw_Pre_Run(Scw_Instance_T *p_scw_instance, const Scw_Input_T *p_scw_input, const Fbk_Output_T *p_fbk_output)
{
   Scw_Core_Input_T *p_core_input;
   const Scw_Core_Calibration_T *calibration;
   const Fbk_Vehicle_Data_T *p_vehicle_data;
   boolean_T scw_error = FBK_FALSE;
   Scw_State_Flags_T scw_state_flags;

   /* Asserts */
   assert(NULL != p_scw_instance);
   assert(NULL != p_scw_input);
   assert(NULL != p_fbk_output);

   calibration             = &(p_scw_instance->calibration);
   p_core_input            = &(p_scw_instance->core_input);
   p_core_input->p_pa_data = p_fbk_output->p_pa_data;
   p_vehicle_data          = &(p_scw_instance->core_input.p_pa_data->vehicle_data);

   Scw_Update_Flags(p_scw_input, &scw_state_flags, p_vehicle_data);

   Scw_State_Machine(&scw_state_flags, &scw_error, &Scw_Cur_State);

   /*Adding state machine inputs to enable the feature*/
   if (Scw_Cur_State == SCW_STATE_ACTIVE)
   {
      p_core_input->f_scw_enable = FBK_TRUE;
   }
   else
   {
      p_core_input->f_scw_enable = FBK_FALSE;
   }
   /* Set core input flags */
   if (Fbk_Is_True(calibration->k_scw_f_enable_via_cal))
   {
      p_core_input->f_scw_enable = calibration->k_scw_f_enable;
   }

   if (Fbk_Is_True(calibration->k_scw_f_dynamic_enable_via_cal))
   {
      p_core_input->f_scw_enable_dynamic = calibration->k_scw_f_enable_dynamic;
   }
   else
   {
      p_core_input->f_scw_enable_dynamic = (boolean_T) (Fbk_Is_True(p_scw_input->f_scw_enable_dynamic));
   }

   if (Fbk_Is_True(calibration->k_scw_f_guardrail_enable_via_cal))
   {
      p_core_input->f_scw_enable_guardrail = calibration->k_scw_f_enable_guardrail;
   }
   else
   {
      p_core_input->f_scw_enable_guardrail = (boolean_T) (Fbk_Is_True(p_scw_input->f_scw_enable_guardrail));
   }
   /* Set core input guardrail data */
   Scw_Set_Guardrail_Data(p_scw_instance);

   /* Trailer information */
   p_core_input->trailer.f_present = p_scw_input->f_trailer_present;
   p_core_input->trailer.length    = p_scw_input->trailer_length;
   p_core_input->trailer.width     = p_scw_input->trailer_width;
   p_core_input->trailer.angle     = p_scw_input->trailer_angle;
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/
static void Scw_Set_Guardrail_Data(Scw_Instance_T *p_scw_instance)
{
   uint8_t side;
   const Fbk_Guardrail_Data_T *p_guardrail_data;
   Scw_Core_Input_T *p_scw_core_input      = &(p_scw_instance->core_input);
   const Scw_Core_Calibration_T *p_scw_cal = &(p_scw_instance->calibration);
   Scw_Persistent_T *p_scw_persistent      = &(p_scw_instance->persistent);

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_guardrail_data = &(p_scw_core_input->p_pa_data->guardrail_data[side]);

      /* SCW does currently not use camera information */
      p_scw_core_input->guardrail_data[side].camera.lateral_position = FBK_ZERO_F;
      p_scw_core_input->guardrail_data[side].camera.confidence       = FBK_ZERO_F;
      p_scw_core_input->guardrail_data[side].camera.type             = SCW_GUARDRAIL_INVALID;

      /* use guardrail information */
      if (Fbk_Is_True(p_guardrail_data->f_active) && Fbk_Is_True(p_guardrail_data->f_present)
          && (p_guardrail_data->age >= p_scw_cal->k_scw_min_guardrail_age)
          && ((PA_OBJ_STATUS_COASTED == p_guardrail_data->status) || (PA_OBJ_STATUS_MATURE == p_guardrail_data->status)))
      {
         Scw_Set_Guardrail_Data_Valid(p_scw_instance, p_guardrail_data, side);
      }
      else
      {
         p_scw_core_input->guardrail_data[side].radar.lateral_position = FBK_ZERO_F;
         p_scw_core_input->guardrail_data[side].radar.confidence       = FBK_ZERO_F;
         p_scw_core_input->guardrail_data[side].radar.type             = SCW_GUARDRAIL_INVALID;
      }
      p_scw_persistent->grail_lat_position[side] = p_scw_core_input->guardrail_data[side].radar.lateral_position;
      p_scw_persistent->grail_confidence[side]   = p_scw_core_input->guardrail_data[side].radar.confidence;
   }
}


static void Scw_Set_Guardrail_Data_Valid(Scw_Instance_T *p_scw_instance, const Fbk_Guardrail_Data_T *p_guardrail_data, uint8_t side)
{
   Scw_Core_Input_T *p_scw_core_input      = &(p_scw_instance->core_input);
   const Scw_Core_Calibration_T *p_scw_cal = &(p_scw_instance->calibration);
   Scw_Persistent_T *p_scw_persistent      = &(p_scw_instance->persistent);
   float32_T lat_pos_ratio;

   lat_pos_ratio = Fbk_Abs_F(p_guardrail_data->lat_pos / Fbk_Max(Fbk_Abs_F(p_scw_persistent->grail_lat_position[side]), EPSILON));
   if ((FBK_ZERO_UINT == p_scw_persistent->grail_freeze_counter[side]) && (lat_pos_ratio < p_scw_cal->k_scw_max_lat_pos_ratio))
   {
      p_scw_persistent->grail_freeze_counter[side] = p_scw_cal->k_scw_guardrail_freeze_period;
   }
   if (p_scw_persistent->grail_freeze_counter[side] > FBK_ZERO_UINT)
   {
      p_scw_persistent->grail_freeze_counter[side]--;
      p_scw_core_input->guardrail_data[side].radar.lateral_position = p_scw_persistent->grail_lat_position[side];
      p_scw_core_input->guardrail_data[side].radar.confidence       = p_scw_persistent->grail_confidence[side];
   }
   else
   {
      p_scw_core_input->guardrail_data[side].radar.lateral_position = p_guardrail_data->lat_pos;
      p_scw_core_input->guardrail_data[side].radar.confidence       = p_guardrail_data->existence_probability;
   }
   p_scw_core_input->guardrail_data[side].radar.type = SCW_GUARDRAIL_VALID;
}
