/**
 * @file scw_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw_pre_run.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_persistent_t.h"
#include "scw_types.h"
#include <assert.h>

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief This function fills the core input guardrail data from the Generic customer input.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2991}
 * @SDD{CSCSA-121179}
 * @verification{Create the test to check if the guardrail data is set correctly}
 */
static void Scw_Set_Guardrail_Data(Scw_Instance_T *p_scw_instance /**< SCW core instance -> guardrail information */);

/**
 * @brief Helper function for Scw_Set_Guardrail_Data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2991}
 * @SDD{CSCSA-290616}
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

   /* Enable the SCW Algorithm */
   p_scw_input->f_scw_enable           = FBK_TRUE;
   p_scw_input->f_scw_enable_dynamic   = FBK_TRUE;
   p_scw_input->f_scw_enable_guardrail = FBK_TRUE;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_scw_instance" points to a non-constant type] */
void Scw_Pre_Run_Init(Scw_Instance_T *p_scw_instance /**< SCW instance */)
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

   /* Asserts */
   assert(NULL != p_scw_instance);
   assert(NULL != p_scw_input);
   assert(NULL != p_fbk_output);

   p_core_input = &(p_scw_instance->core_input);

   /* Set core input */
   p_core_input->f_scw_enable           = p_scw_input->f_scw_enable;
   p_core_input->f_scw_enable_dynamic   = p_scw_input->f_scw_enable_dynamic;
   p_core_input->f_scw_enable_guardrail = p_scw_input->f_scw_enable_guardrail;
   p_core_input->p_pa_data              = p_fbk_output->p_pa_data;

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
