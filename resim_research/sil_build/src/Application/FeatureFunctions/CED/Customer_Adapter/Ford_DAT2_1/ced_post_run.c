/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Ford Dat2.1 post run logic for CED.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ced_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define CED_FORD_INVALID_TTC (5.0f)
#define CED_FORD_INVALID_TTP (5.0f)

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Sets ouput in arrays depending on side- and approach-indices.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3503}
 * @verification{Check that ced output is set.}
 */
static void Ced_Set_Output(Ced_Output_T *p_ced_output,
                           const Ced_Core_Input_T *p_ced_core_input,
                           const Ced_Core_Output_T *p_ced_core_output,
                           const Ced_Ford_Approach_Types_T approach_type,
                           const uint8_t side_index);


/**
 * @brief Converts the VCS heading to the Ford heading.
 *
 * @return Heading as defined in Ford requirements
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3475}
 * @verification{}
 */
static float32_T Ced_Convert_Vcs_Heading_To_Ford_Heading(const Ced_Ford_Approach_Types_T approach_direction,
                                                         const float32_T vcs_heading);

/**
 * @brief Limits TTC and TTP values to Ford specified range.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3473}
 * @verification{}
 */
static void Ced_Set_Limited_Ttc_And_Ttp_Values(Ced_Output_T *p_ced_output,
                                               const uint8_t approach_index,
                                               const float32_T ttc,
                                               const float32_T ttp);

/**
 * @brief Resets the ford specific output of Ced to its defaults.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3474}
 * @verification{}
 */
static void Ced_Reset_Output(Ced_Output_T *p_ced_output);

/*===========================================================================*\
* Global Functions	Definition
\*===========================================================================*/

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
   /* Asserts */
   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_output);
   assert(NULL != p_ced_input);

   /* Reset CED output */
   Ced_Reset_Output(p_ced_output);

   /* Check left side */
   if (FBK_SIDE_REAR == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_LEFT])
   {
      Ced_Set_Output(p_ced_output, &(p_ced_instance->core_input), &(p_ced_instance->core_output), CED_FORD_APPROACH_REAR_LEFT,
                     FBK_SIDE_LEFT);
   }
   else if (FBK_SIDE_FRONT == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_LEFT])
   {
      Ced_Set_Output(p_ced_output, &(p_ced_instance->core_input), &(p_ced_instance->core_output), CED_FORD_APPROACH_FRONT_LEFT,
                     FBK_SIDE_LEFT);
   }
   else
   {
      /* No alert on left side */
   }

   /* Check right side */
   if (FBK_SIDE_REAR == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_RIGHT])
   {
      Ced_Set_Output(p_ced_output, &(p_ced_instance->core_input), &(p_ced_instance->core_output), CED_FORD_APPROACH_REAR_RIGHT,
                     FBK_SIDE_RIGHT);
   }
   else if (FBK_SIDE_FRONT == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_RIGHT])
   {
      Ced_Set_Output(p_ced_output, &(p_ced_instance->core_input), &(p_ced_instance->core_output), CED_FORD_APPROACH_FRONT_RIGHT,
                     FBK_SIDE_RIGHT);
   }
   else
   {
      /* No alert on right side */
   }

   /* Map the internal ced output struct to the external struct provided in the Ford-specific input. */
   if (NULL != p_ced_input->p_ford_ced_output)
   {
      *p_ced_input->p_ford_ced_output = *p_ced_output;
   }

   /* Debug output */
#ifdef BINARY_DEBUG
   Write_Ced_Output(p_ced_output);
#endif
}

/*===========================================================================*\
* Local Functions Definition
\*===========================================================================*/

static void Ced_Set_Output(Ced_Output_T *p_ced_output,
                           const Ced_Core_Input_T *p_ced_core_input,
                           const Ced_Core_Output_T *p_ced_core_output,
                           const Ced_Ford_Approach_Types_T approach_type,
                           const uint8_t side_index)
{
   uint8_t approach_index = (uint8_t) approach_type;
   uint8_t obj_index;

   assert(NULL != p_ced_output);
   assert(NULL != p_ced_core_input);
   assert(NULL != p_ced_core_output);

   p_ced_output->ced_alert[approach_index] = (uint8_t) p_ced_core_output->ced_alert[side_index];
   p_ced_output->ced_id[approach_index]    = p_ced_core_output->ced_id[side_index];

   Ced_Set_Limited_Ttc_And_Ttp_Values(p_ced_output, approach_index, p_ced_core_output->ced_ttc[side_index],
                                      p_ced_core_output->ced_ttp[side_index]);

   obj_index = p_ced_core_output->ced_index[side_index];

   if (PA_OBJ_STATUS_INVALID != p_ced_core_input->p_pa_data->object_data[obj_index].status)
   {
      p_ced_output->ced_object_speed[approach_index] = p_ced_core_input->p_pa_data->object_data[obj_index].speed;
      p_ced_output->ced_object_heading[approach_index] =
         Ced_Convert_Vcs_Heading_To_Ford_Heading(approach_type, p_ced_core_input->p_pa_data->object_data[obj_index].vcs_heading);
      p_ced_output->ced_object_length[approach_index] = p_ced_core_input->p_pa_data->object_data[obj_index].length;
      p_ced_output->ced_object_width[approach_index]  = p_ced_core_input->p_pa_data->object_data[obj_index].width;
   }
}

static float32_T Ced_Convert_Vcs_Heading_To_Ford_Heading(const Ced_Ford_Approach_Types_T approach_direction, const float32_T vcs_heading)
{
   float32_T ford_heading = vcs_heading;

   /* Flip sign for objects on the right side */
   if ((CED_FORD_APPROACH_REAR_RIGHT == approach_direction) || (CED_FORD_APPROACH_FRONT_RIGHT == approach_direction))
   {
      ford_heading = -ford_heading;
   }

   /* Reduce angle to range (0, PI/2) for objects approaching from the front */
   if ((CED_FORD_APPROACH_FRONT_RIGHT == approach_direction) || (CED_FORD_APPROACH_FRONT_LEFT == approach_direction))
   {
      if (ford_heading < FBK_ZERO_F)
      {
         ford_heading = -PI - ford_heading;
      }
      else
      {
         ford_heading = PI - ford_heading;
      }
   }

   return ford_heading;
}

static void Ced_Set_Limited_Ttc_And_Ttp_Values(Ced_Output_T *p_ced_output,
                                               const uint8_t approach_index,
                                               const float32_T ttc,
                                               const float32_T ttp)
{
   /* Assert */
   assert(NULL != p_ced_output);

   /* Limit TTC and TTP values to Ford range. */
   if (approach_index < (uint8_t) CED_FORD_NUMBER_OF_APPROACH_TYPES)
   {
      p_ced_output->ced_ttc[approach_index] = Fbk_Clamp(ttc, -CED_FORD_INVALID_TTC, CED_FORD_INVALID_TTC);
      p_ced_output->ced_ttp[approach_index] = Fbk_Clamp(ttp, -CED_FORD_INVALID_TTP, CED_FORD_INVALID_TTP);
   }
}

static void Ced_Reset_Output(Ced_Output_T *p_ced_output)
{
   uint8_t side_idx;

   assert(NULL != p_ced_output);

   for (side_idx = FBK_ZERO_UINT; side_idx < (uint8_t) CED_FORD_NUMBER_OF_APPROACH_TYPES; side_idx++)
   {
      p_ced_output->ced_alert[side_idx]          = (uint8_t) CED_NO_ALERT;
      p_ced_output->ced_id[side_idx]             = FBK_ZERO_UINT;
      p_ced_output->ced_ttc[side_idx]            = CED_FORD_INVALID_TTC;
      p_ced_output->ced_ttp[side_idx]            = CED_FORD_INVALID_TTP;
      p_ced_output->ced_object_speed[side_idx]   = FBK_ZERO_F;
      p_ced_output->ced_object_heading[side_idx] = FBK_ZERO_F;
      p_ced_output->ced_object_length[side_idx]  = FBK_ZERO_F;
      p_ced_output->ced_object_width[side_idx]   = FBK_ZERO_F;
   }
}


#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output)
{
   uint8_t i;

   /* check input parameters */
   assert(NULL != p_ced_output);

   /* Log the CED customer output to bin file for MATE visualization. */
   CED_STORE_VAL_MGR_WPR("ced_alert_left", p_ced_output->ced_alert[CED_FORD_APPROACH_REAR_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_id_left", p_ced_output->ced_id[CED_FORD_APPROACH_REAR_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_ttc_left", p_ced_output->ced_ttc[CED_FORD_APPROACH_REAR_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_ttp_left", p_ced_output->ced_ttp[CED_FORD_APPROACH_REAR_LEFT]);

   CED_STORE_VAL_MGR_WPR("ced_alert_right", p_ced_output->ced_alert[CED_FORD_APPROACH_REAR_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_id_right", p_ced_output->ced_id[CED_FORD_APPROACH_REAR_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_ttc_right", p_ced_output->ced_ttc[CED_FORD_APPROACH_REAR_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_ttp_right", p_ced_output->ced_ttp[CED_FORD_APPROACH_REAR_RIGHT]);

   CED_STORE_VAL_MGR_WPR("ced_front_alert_left", p_ced_output->ced_alert[CED_FORD_APPROACH_FRONT_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_front_id_left", p_ced_output->ced_id[CED_FORD_APPROACH_FRONT_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_front_ttc_left", p_ced_output->ced_ttc[CED_FORD_APPROACH_FRONT_LEFT]);
   CED_STORE_VAL_MGR_WPR("ced_front_ttp_left", p_ced_output->ced_ttp[CED_FORD_APPROACH_FRONT_LEFT]);

   CED_STORE_VAL_MGR_WPR("ced_front_alert_right", p_ced_output->ced_alert[CED_FORD_APPROACH_FRONT_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_front_id_right", p_ced_output->ced_id[CED_FORD_APPROACH_FRONT_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_front_ttc_right", p_ced_output->ced_ttc[CED_FORD_APPROACH_FRONT_RIGHT]);
   CED_STORE_VAL_MGR_WPR("ced_front_ttp_right", p_ced_output->ced_ttp[CED_FORD_APPROACH_FRONT_RIGHT]);

   /* Log complete arrays as defined in CED Ford output interface. */
   for (i = FBK_ZERO_UINT; i < (uint8_t) CED_FORD_NUMBER_OF_APPROACH_TYPES; i++)
   {
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_alert", p_ced_output->ced_alert[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_id", p_ced_output->ced_id[i], i);

      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_ttc", p_ced_output->ced_ttc[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_ttp", p_ced_output->ced_ttp[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_speed", p_ced_output->ced_object_speed[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_heading", p_ced_output->ced_object_heading[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_length", p_ced_output->ced_object_length[i], i);
      CED_STORE_ARRAY_ELEM_MGR_WPR("ced_object_width", p_ced_output->ced_object_width[i], i);
   }
}
#endif /* BINARY_DEBUG */
