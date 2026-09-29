/**
 * @file scw_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for SCW.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "scw_post_run.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_input_t.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"
#include "scw_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "scw_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Resets SCW output to its default values.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{CSCSA-125690}
 * @verification{Test that Scw output is initialized correctly}
 */
static void Scw_Reset_Output(Scw_Output_T *p_scw_output /**< Scw output */,
                             const Scw_Core_Calibration_T *p_scw_cal /**< Scw calibration */);


/**
 * @brief Copies Scw Core output (basic information) to Scw output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{CSCSA-125691}
 * @verification{Test that Scw Core output is copied correctly}
 */
static void Scw_Copy_Core_Output(Scw_Output_T *p_scw_output /**< Scw output */,
                                 const Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
                                 uint8_t side /**< Ego side */);


/**
 * @brief Sets most critical dynamic objects information to Scw output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{CSCSA-125692}
 * @verification{Test that critical dynamic object output data is set correctly}
 */
static void Scw_Set_Dynamic_Object_Output(Scw_Output_T *p_scw_output /**< Scw output */,
                                          const Pa_Data_T *p_pa_data /**< PA context */,
                                          const Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
                                          const uint8_t side /**< Ego side */);


/**
 * @brief Sets most critical guardrails information to Scw output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{CSCSA-125693}
 * @verification{Test that critical guardrail output data is set correctly}
 */
static void Scw_Set_Guardrail_Output(Scw_Output_T *p_scw_output /**< Scw output */,
                                     const Pa_Data_T *p_pa_data,
                                     const Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
                                     const uint8_t side /**< Ego side */);

#ifdef BINARY_DEBUG
static void Write_Scw_Output(const Scw_Output_T *p_scw_output);
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameters are used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_scw_instance" points to a non-constant type] */
void Scw_Post_Run_Init(Scw_Instance_T *p_scw_instance)
{
   /* Assert */
   assert(NULL != p_scw_instance);
}


/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameters are used by other customer and cannot be removed] */
void Scw_Post_Run(const Scw_Instance_T *p_scw_instance, const Scw_Input_T *p_scw_input, Scw_Output_T *p_scw_output)
/* clang-format on */
{
   uint8_t side;
   const Scw_Core_Output_T *p_scw_core_output;
   const Scw_Core_Calibration_T *p_scw_cal;
   const Pa_Data_T *p_pa_data;

   /* Asserts */
   assert(NULL != p_scw_instance);
   assert(NULL != p_scw_input);
   assert(NULL != p_scw_output);

   p_scw_core_output = &p_scw_instance->core_output;
   p_scw_cal         = &p_scw_instance->calibration;
   p_pa_data         = p_scw_instance->core_input.p_pa_data;

   Scw_Reset_Output(p_scw_output, p_scw_cal);

   /* Map core to generic output */
   p_scw_output->f_scw_enabled           = p_scw_input->f_scw_enable;
   p_scw_output->f_scw_dyn_enabled       = p_scw_input->f_scw_enable_dynamic;
   p_scw_output->f_scw_guardrail_enabled = p_scw_input->f_scw_enable_guardrail;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      /* In case of any non default object type, update the respective sides object properties. */
      if (SCW_OBJECT_TYPE_DYNAMIC == p_scw_core_output->obj_type[side])
      {
         Scw_Set_Dynamic_Object_Output(p_scw_output, p_pa_data, p_scw_core_output, side);
      }
      else if (SCW_OBJECT_TYPE_GUARDRAIL == p_scw_core_output->obj_type[side])
      {
         Scw_Set_Guardrail_Output(p_scw_output, p_pa_data, p_scw_core_output, side);
      }
      else
      {
         /*Keep the previously set default for the respective side*/
      }
   }
   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Scw_Output(p_scw_output);
#endif
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Scw_Reset_Output(Scw_Output_T *p_scw_output, const Scw_Core_Calibration_T *p_scw_cal)
{
   uint8_t side;

   p_scw_output->f_scw_enabled           = FBK_FALSE;
   p_scw_output->f_scw_dyn_enabled       = FBK_FALSE;
   p_scw_output->f_scw_guardrail_enabled = FBK_FALSE;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_scw_output->scw_object[side].alert_level           = SCW_NO_ALERT;
      p_scw_output->scw_object[side].id                    = PA_INVALID_OBJ_ID;
      p_scw_output->scw_object[side].unique_id             = PA_INVALID_OBJ_ID;
      p_scw_output->scw_object[side].type                  = SCW_OBJECT_TYPE_NONE;
      p_scw_output->scw_object[side].lateral_ttc_s         = p_scw_cal->k_scw_lateral_ttc_default;
      p_scw_output->scw_object[side].lateral_distance_m    = p_scw_cal->k_scw_lateral_distance_default;
      p_scw_output->scw_object[side].position_m.x          = -SCW_BIG_VALUE;
      p_scw_output->scw_object[side].position_m.y          = Fbk_Convert_Obj_Side_To_Sign(side) * SCW_BIG_VALUE;
      p_scw_output->scw_object[side].velocity_mps.x        = FBK_ZERO_F;
      p_scw_output->scw_object[side].velocity_mps.y        = FBK_ZERO_F;
      p_scw_output->scw_object[side].acceleration_mps2.x   = FBK_ZERO_F;
      p_scw_output->scw_object[side].acceleration_mps2.y   = FBK_ZERO_F;
      p_scw_output->scw_object[side].width_m               = FBK_ZERO_F;
      p_scw_output->scw_object[side].length_m              = FBK_ZERO_F;
      p_scw_output->scw_object[side].heading_rad           = FBK_ZERO_F;
      p_scw_output->scw_object[side].yawrate_radps         = FBK_ZERO_F;
      p_scw_output->scw_object[side].existence_probability = FBK_ZERO_F;
      p_scw_output->scw_object[side].age                   = FBK_ZERO_UINT;

      p_scw_output->scw_object[side].ttle_s = p_scw_cal->k_scw_ttle_default;
      p_scw_output->scw_object[side].ttp_s  = p_scw_cal->k_scw_ttp_default;
   }
}


static void Scw_Copy_Core_Output(Scw_Output_T *p_scw_output, const Scw_Core_Output_T *p_scw_core_output, uint8_t side)
{
   if ((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side))
   {
      p_scw_output->scw_object[side].alert_level         = p_scw_core_output->alert_level[side];
      p_scw_output->scw_object[side].id                  = p_scw_core_output->obj_id[side];
      p_scw_output->scw_object[side].unique_id           = p_scw_core_output->obj_unique_id[side];
      p_scw_output->scw_object[side].type                = p_scw_core_output->obj_type[side];
      p_scw_output->scw_object[side].lateral_ttc_s       = p_scw_core_output->obj_lateral_ttc[side];
      p_scw_output->scw_object[side].lateral_distance_m  = p_scw_core_output->obj_lateral_distance[side];
      p_scw_output->scw_object[side].velocity_mps.y      = p_scw_core_output->obj_lateral_velocity[side];
      p_scw_output->scw_object[side].acceleration_mps2.y = p_scw_core_output->obj_lateral_acceleration[side];

      p_scw_output->scw_object[side].ttle_s = p_scw_core_output->obj_ttle[side];
      p_scw_output->scw_object[side].ttp_s  = p_scw_core_output->obj_ttp[side];
   }
}


static void Scw_Set_Dynamic_Object_Output(Scw_Output_T *p_scw_output,
                                          const Pa_Data_T *p_pa_data,
                                          const Scw_Core_Output_T *p_scw_core_output,
                                          const uint8_t side)
{
   if ((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side))
   {
      Fbk_Object_Data_T object_data = p_pa_data->object_data[p_scw_core_output->obj_index[side]];

      Scw_Copy_Core_Output(p_scw_output, p_scw_core_output, side);

      p_scw_output->scw_object[side].position_m.x          = object_data.vcs_pos.x;
      p_scw_output->scw_object[side].position_m.y          = object_data.vcs_pos.y;
      p_scw_output->scw_object[side].velocity_mps.x        = object_data.vcs_vel.x;
      p_scw_output->scw_object[side].acceleration_mps2.x   = object_data.vcs_accel.x;
      p_scw_output->scw_object[side].width_m               = object_data.width;
      p_scw_output->scw_object[side].length_m              = object_data.length;
      p_scw_output->scw_object[side].heading_rad           = object_data.vcs_heading;
      p_scw_output->scw_object[side].yawrate_radps         = object_data.heading_rate;
      p_scw_output->scw_object[side].existence_probability = object_data.existence_probability;
      p_scw_output->scw_object[side].age                   = object_data.age;
   }
}


static void Scw_Set_Guardrail_Output(Scw_Output_T *p_scw_output,
                                     const Pa_Data_T *p_pa_data,
                                     const Scw_Core_Output_T *p_scw_core_output,
                                     const uint8_t side)
{
   if ((FBK_SIDE_LEFT == side) || (FBK_SIDE_RIGHT == side))
   {
      Fbk_Guardrail_Data_T guardrail_data;

      /* Fill guardrail data */
      guardrail_data = p_pa_data->guardrail_data[side];
      Scw_Copy_Core_Output(p_scw_output, p_scw_core_output, side);

      p_scw_output->scw_object[side].position_m.x          = FBK_ZERO_F;
      p_scw_output->scw_object[side].position_m.y          = guardrail_data.lat_pos;
      p_scw_output->scw_object[side].velocity_mps.x        = FBK_ZERO_F;
      p_scw_output->scw_object[side].acceleration_mps2.x   = FBK_ZERO_F;
      p_scw_output->scw_object[side].width_m               = FBK_ZERO_F;
      p_scw_output->scw_object[side].length_m              = FBK_ZERO_F;
      p_scw_output->scw_object[side].heading_rad           = FBK_ZERO_F;
      p_scw_output->scw_object[side].existence_probability = guardrail_data.existence_probability;
      p_scw_output->scw_object[side].age                   = guardrail_data.age;
   }
}


#ifdef BINARY_DEBUG

static void Write_Scw_Output(const Scw_Output_T *p_scw_output)
{
   /* check input parameters */
   assert(NULL != p_scw_output);

   SCW_STORE_VAL_MGR_WPR("SCW_alert_level_left", p_scw_output->scw_object[FBK_SIDE_LEFT].alert_level);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_id_left", p_scw_output->scw_object[FBK_SIDE_LEFT].id);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_type_left", p_scw_output->scw_object[FBK_SIDE_LEFT].type);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_distance_left", p_scw_output->scw_object[FBK_SIDE_LEFT].lateral_distance_m);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_ttc_left", p_scw_output->scw_object[FBK_SIDE_LEFT].lateral_ttc_s);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_x_left", p_scw_output->scw_object[FBK_SIDE_LEFT].position_m.x);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_y_left", p_scw_output->scw_object[FBK_SIDE_LEFT].position_m.y);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_x_left", p_scw_output->scw_object[FBK_SIDE_LEFT].velocity_mps.x);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_y_left", p_scw_output->scw_object[FBK_SIDE_LEFT].velocity_mps.y);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_x_left", p_scw_output->scw_object[FBK_SIDE_LEFT].acceleration_mps2.x);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_y_left", p_scw_output->scw_object[FBK_SIDE_LEFT].acceleration_mps2.y);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_width_left", p_scw_output->scw_object[FBK_SIDE_LEFT].width_m);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_length_left", p_scw_output->scw_object[FBK_SIDE_LEFT].length_m);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_heading_left", p_scw_output->scw_object[FBK_SIDE_LEFT].heading_rad);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_yawrate_left", p_scw_output->scw_object[FBK_SIDE_LEFT].yawrate_radps);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_exist_prob_left", p_scw_output->scw_object[FBK_SIDE_LEFT].existence_probability);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_age_left", p_scw_output->scw_object[FBK_SIDE_LEFT].age);

   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttle_left", p_scw_output->scw_object[FBK_SIDE_LEFT].ttle_s);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttp_left", p_scw_output->scw_object[FBK_SIDE_LEFT].ttp_s);


   SCW_STORE_VAL_MGR_WPR("SCW_alert_level_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].alert_level);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_id_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].id);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_type_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].type);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_distance_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].lateral_distance_m);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_lateral_ttc_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].lateral_ttc_s);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_x_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].position_m.x);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_pos_y_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].position_m.y);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_x_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].velocity_mps.x);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_vel_y_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].velocity_mps.y);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_x_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].acceleration_mps2.x);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_acc_y_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].acceleration_mps2.y);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_width_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].width_m);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_length_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].length_m);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_heading_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].heading_rad);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_yawrate_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].yawrate_radps);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_exist_prob_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].existence_probability);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_age_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].age);

   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttle_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].ttle_s);
   SCW_STORE_VAL_MGR_WPR("SCW_critical_obj_ttp_right", p_scw_output->scw_object[FBK_SIDE_RIGHT].ttp_s);
}

#endif
