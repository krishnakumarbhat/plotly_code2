/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for CED.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced_post_run.h"
#include "ced_core_output_t.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
#include <assert.h>
#include <string.h>

#ifdef BINARY_DEBUG
#include "ced_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */


/**
 * @brief Set ced output object properties to default values
 *
 * @return void
 *
 * @SRS{SF-106}
 * @SAD{SF-2404}
 * @SDD{CSCSA-126136}
 * @verification{ Run Ced_Reset_Critical_Object on non-default object and check if its parameter are reset}
 */
static void Ced_Reset_Critical_Object(Ced_Critical_Object_T *p_ced_object);

/**
 * @brief Map core core enum for CED approach side to generic enum.
 *
 * @return Ced_Target_Travel_Direction_T - direction of approaching object
 *
 * @SRS{SF-106}
 * @SAD{SF-2404}
 * @SDD{CSCSA-121735}
 * @verification{ Run Ced_Map_Object_Direction with direction parameter [uin8_t] and verify against the output
 * [Ced_Target_Travel_Direction_T]  }
 */
static Ced_Target_Travel_Direction_T Ced_Map_Object_Direction(uint8_t direction);

/*============================================================================*\
* EXPORTED FUNCTIONS
\*============================================================================*/

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
}

// clang-format off
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run(Ced_Instance_T *p_ced_instance, const Ced_Input_T *p_ced_input,  Ced_Output_T *p_ced_output)
// clang-format on
{
   // clang-format on
   uint8_t side_index;
   const Ced_Core_Output_T *p_ced_core_output;
   const Pa_Data_T *p_pa_data;

   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_input);
   assert(NULL != p_ced_output);

   p_ced_core_output = &p_ced_instance->core_output;
   p_pa_data         = p_ced_instance->core_input.p_pa_data;

   p_ced_output->f_ced_enable = p_ced_input->f_ced_enable;

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      uint8_t obj_index                     = p_ced_core_output->ced_index[side_index];
      const Fbk_Object_Data_T *p_fbk_object = &p_pa_data->object_data[obj_index];
      if (PA_INVALID_OBJ_INDEX != obj_index)
      {
         p_ced_output->ced_alert[side_index]              = p_ced_core_output->ced_alert[side_index];
         p_ced_output->ced_object[side_index].id          = p_ced_core_output->ced_id[side_index];
         p_ced_output->ced_object[side_index].unique_id   = p_ced_core_output->ced_unique_id[side_index];
         p_ced_output->ced_object[side_index].type        = p_fbk_object->obj_class;
         p_ced_output->ced_object[side_index].length_m    = p_fbk_object->length;
         p_ced_output->ced_object[side_index].width_m     = p_fbk_object->width;
         p_ced_output->ced_object[side_index].long_pos_m  = p_fbk_object->vcs_pos.x;
         p_ced_output->ced_object[side_index].lat_pos_m   = p_fbk_object->vcs_pos.y;
         p_ced_output->ced_object[side_index].speed_mps   = p_fbk_object->speed;
         p_ced_output->ced_object[side_index].heading_rad = p_fbk_object->vcs_heading;
         p_ced_output->ced_object[side_index].direction =
            Ced_Map_Object_Direction(p_ced_core_output->ced_object_direction[side_index]);
         p_ced_output->ced_object[side_index].predicted_lat_pos_m = p_ced_core_output->ced_object_predicted_lat_pos[side_index];
         p_ced_output->ced_object[side_index].ttc_s               = p_ced_core_output->ced_ttc[side_index];
         p_ced_output->ced_object[side_index].ttp_s               = p_ced_core_output->ced_ttp[side_index];
      }
      else
      {
         p_ced_output->ced_alert[side_index] = CED_NO_ALERT;
         Ced_Reset_Critical_Object(&(p_ced_output->ced_object[side_index]));
      }
   }

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Ced_Output(p_ced_output);
#endif
}

/*============================================================================*\
* LOCAL FUNCTIONS
\*============================================================================*/

static Ced_Target_Travel_Direction_T Ced_Map_Object_Direction(uint8_t direction)
{

   Ced_Target_Travel_Direction_T obj_direction;
   switch (direction)
   {
      case FBK_SIDE_FRONT:
         obj_direction = FRONT_DIRECTION;
         break;
      case FBK_SIDE_REAR:
         obj_direction = REAR_DIRECTION;
         break;
      case FBK_SIDE_UNDEFINED:
      default:
         obj_direction = UNDEF_DIRECTION;
         break;
   }

   return obj_direction;
}

static void Ced_Reset_Critical_Object(Ced_Critical_Object_T *p_ced_object)
{
   p_ced_object->id                  = PA_INVALID_OBJ_ID;
   p_ced_object->type                = PA_OBJ_CLASS_UNKNOWN;
   p_ced_object->length_m            = FBK_ZERO_F;
   p_ced_object->width_m             = FBK_ZERO_F;
   p_ced_object->long_pos_m          = FBK_ZERO_F;
   p_ced_object->lat_pos_m           = FBK_ZERO_F;
   p_ced_object->speed_mps           = FBK_ZERO_F;
   p_ced_object->heading_rad         = FBK_ZERO_F;
   p_ced_object->direction           = UNDEF_DIRECTION;
   p_ced_object->predicted_lat_pos_m = FBK_ZERO_F;
   p_ced_object->ttc_s               = FBK_ZERO_F;
   p_ced_object->ttp_s               = FBK_ZERO_F;
}


#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output)
{
   /* check input parameters */
   assert(NULL != p_ced_output);

   /* Log Safe Exit specific data*/
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_f_ced_enable", p_ced_output->f_ced_enable);

   CED_STORE_VAL_MGR_WPR("CED_Gen_out_alert_left", p_ced_output->ced_alert[FBK_SIDE_LEFT]);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_alert_right", p_ced_output->ced_alert[FBK_SIDE_RIGHT]);

   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_id", p_ced_output->ced_object[FBK_SIDE_LEFT].id);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_type", p_ced_output->ced_object[FBK_SIDE_LEFT].type);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_length_m", p_ced_output->ced_object[FBK_SIDE_LEFT].length_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_width_m", p_ced_output->ced_object[FBK_SIDE_LEFT].width_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_lat_pos_m", p_ced_output->ced_object[FBK_SIDE_LEFT].lat_pos_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_long_pos_m", p_ced_output->ced_object[FBK_SIDE_LEFT].long_pos_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_speed_mps", p_ced_output->ced_object[FBK_SIDE_LEFT].speed_mps);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_heading_rad", p_ced_output->ced_object[FBK_SIDE_LEFT].heading_rad);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_direction", p_ced_output->ced_object[FBK_SIDE_LEFT].direction);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_predicted_lat_pos_m", p_ced_output->ced_object[FBK_SIDE_LEFT].predicted_lat_pos_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_ttc_s", p_ced_output->ced_object[FBK_SIDE_LEFT].ttc_s);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_left_ttp_s", p_ced_output->ced_object[FBK_SIDE_LEFT].ttp_s);

   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_id", p_ced_output->ced_object[FBK_SIDE_RIGHT].id);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_type", p_ced_output->ced_object[FBK_SIDE_RIGHT].type);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_length_m", p_ced_output->ced_object[FBK_SIDE_RIGHT].length_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_width_m", p_ced_output->ced_object[FBK_SIDE_RIGHT].width_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_lat_pos_m", p_ced_output->ced_object[FBK_SIDE_RIGHT].lat_pos_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_long_pos_m", p_ced_output->ced_object[FBK_SIDE_RIGHT].long_pos_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_speed_mps", p_ced_output->ced_object[FBK_SIDE_RIGHT].speed_mps);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_heading_rad", p_ced_output->ced_object[FBK_SIDE_RIGHT].heading_rad);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_direction", p_ced_output->ced_object[FBK_SIDE_RIGHT].direction);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_predicted_lat_pos_m", p_ced_output->ced_object[FBK_SIDE_RIGHT].predicted_lat_pos_m);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_ttc_s", p_ced_output->ced_object[FBK_SIDE_RIGHT].ttc_s);
   CED_STORE_VAL_MGR_WPR("CED_Gen_out_obj_right_ttp_s", p_ced_output->ced_object[FBK_SIDE_RIGHT].ttp_s);
}
#endif /* BINARY_DEBUG */
