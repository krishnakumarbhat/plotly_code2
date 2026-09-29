/**
 * @file esa_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for ESA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "esa_post_run.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_instance_t.h"
#include "esa_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"


#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "esa_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Esa_Output(const Esa_Output_T *p_esa_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Update output from core output.
 *
 * @return void
 */
static void Esa_Update_Output(Esa_Output_T *p_esa_output /**< ESA output */,
                              const Esa_Instance_T *p_esa_instance /**< ESA instance */);

/**
 * @brief Reset Generic Esa_Output_T structure.
 * @SRD{CSCSA-122861}
 * @SAD{CSCSA-83856}
 * @SDD{CSCSA-123054}
 * @verification{Set generic Esa_Output_T by non-defaults, run function, verify if Generic Esa_Output_T is filled by defaults.}
 * @return void
 */
static void Esa_Reset_Output(Esa_Output_T *p_esa_output /**< ESA output data */);


/*============================================================================*\
* Global Function Definitions
\*============================================================================*/


/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_esa_instance" points to a non-constant type] */
void Esa_Post_Run_Init(Esa_Instance_T *p_esa_instance)
{
   /* Assert */
   assert(NULL != p_esa_instance);
}

/**
 * @brief Reset Generic ESA Post-Run initialization to match Core Esa_Core_Output_T with Generic Esa_Output_T structue.
 *
 * @return void
 *
 * @SRD{CSCSA-122861}
 * @SAD{CSCSA-83856}
 * @SDD{CSCSA-123052}
 * @verification{Set generic Esa_Core_Output_T and tracker objects by non-defaults, run function, verify if Esa_Output_T is filled
 * by coresponding values.}
 */
// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Esa_Post_Run(const Esa_Instance_T *p_esa_instance, const Esa_Input_T *p_esa_input, Esa_Output_T *p_esa_output)
/* clang-format on */
{
   assert(NULL != p_esa_instance);
   assert(NULL != p_esa_output);
   assert(NULL != p_esa_input);

   Esa_Reset_Output(p_esa_output);
   Esa_Update_Output(p_esa_output, p_esa_instance);

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Esa_Output(p_esa_output);
#endif /* BINARY_DEBUG */
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Esa_Reset_Output(Esa_Output_T *p_esa_output)
{
   uint8_t side;

   assert(NULL != p_esa_output);

   p_esa_output->esa_status = ESA_CORE_STATUS_DISABLED_BY_INPUT;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {

      p_esa_output->f_esa_alert[side] = FBK_FALSE;

      p_esa_output->esa_object[side].id                             = PA_INVALID_OBJ_ID;
      p_esa_output->esa_object[side].index                          = PA_INVALID_OBJ_INDEX;
      p_esa_output->esa_object[side].width_m                        = FBK_ZERO_F;
      p_esa_output->esa_object[side].length_m                       = FBK_ZERO_F;
      p_esa_output->esa_object[side].long_pos_m                     = FBK_ZERO_F;
      p_esa_output->esa_object[side].lat_pos_m                      = FBK_ZERO_F;
      p_esa_output->esa_object[side].long_speed_mps                 = FBK_ZERO_F;
      p_esa_output->esa_object[side].lat_speed_mps                  = FBK_ZERO_F;
      p_esa_output->esa_object[side].ttc_s                          = ESA_DEFAULT_LARGE_TTC;
      p_esa_output->esa_object[side].ttp_s                          = FBK_ZERO_F;
      p_esa_output->esa_object[side].decel_to_reach_host_speed_mps2 = FBK_ZERO_F;
      p_esa_output->esa_object[side].long_distance_m                = -ESA_DEFAULT_OBJ_DIST;
      p_esa_output->esa_object[side].existence_prob                 = FBK_ZERO_F;
   }
}

static void Esa_Update_Output(Esa_Output_T *p_esa_output, const Esa_Instance_T *p_esa_instance)
{

   uint8_t obj_index;
   uint8_t side;
   Fbk_Object_Data_T tracker_object;
   const Esa_Core_Output_T *p_esa_core_output;
   const Pa_Data_T *p_pa_data;

   /* Check if all input pointers are valid */
   assert(NULL != p_esa_output);
   assert(NULL != p_esa_instance);

   p_esa_core_output = &p_esa_instance->core_output;
   p_pa_data         = p_esa_instance->core_input.p_pa_data;

   p_esa_output->esa_status = p_esa_core_output->esa_core_status;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_index = p_esa_core_output->esa_index[side];

      if (PA_INVALID_OBJ_INDEX != obj_index)
      {

         tracker_object = p_pa_data->object_data[obj_index];

         p_esa_output->f_esa_alert[side] = p_esa_core_output->esa_alert[side];

         p_esa_output->esa_object[side].index                          = tracker_object.index;
         p_esa_output->esa_object[side].id                             = tracker_object.id;
         p_esa_output->esa_object[side].width_m                        = tracker_object.width;
         p_esa_output->esa_object[side].length_m                       = tracker_object.length;
         p_esa_output->esa_object[side].long_pos_m                     = tracker_object.curvi_pos.x;
         p_esa_output->esa_object[side].lat_pos_m                      = tracker_object.curvi_pos.y;
         p_esa_output->esa_object[side].long_speed_mps                 = tracker_object.curvi_vel.x;
         p_esa_output->esa_object[side].lat_speed_mps                  = tracker_object.curvi_vel.y;
         p_esa_output->esa_object[side].ttc_s                          = p_esa_core_output->esa_ttc[side];
         p_esa_output->esa_object[side].ttp_s                          = p_esa_core_output->esa_ttp[side];
         p_esa_output->esa_object[side].decel_to_reach_host_speed_mps2 = p_esa_core_output->esa_decel_to_reach_host_speed[side];
         p_esa_output->esa_object[side].long_distance_m                = p_esa_core_output->esa_long_distance[side];
         p_esa_output->esa_object[side].existence_prob                 = tracker_object.existence_probability;
      }
   }
}

#ifdef BINARY_DEBUG
static void Write_Esa_Output(const Esa_Output_T *p_esa_output)
{
   /* check input parameters */
   assert(NULL != p_esa_output);

   /* Log Safe Exit specific data*/
   /* ESA STATUS */
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_esa_status", p_esa_output->esa_status);

   /* LEFT SIDE */
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_f_esa_alert_left", p_esa_output->f_esa_alert[FBK_SIDE_LEFT]);

   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_id", p_esa_output->esa_object[FBK_SIDE_LEFT].id);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_index", p_esa_output->esa_object[FBK_SIDE_LEFT].index);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_width_m", p_esa_output->esa_object[FBK_SIDE_LEFT].width_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_length_m", p_esa_output->esa_object[FBK_SIDE_LEFT].length_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_long_pos_m", p_esa_output->esa_object[FBK_SIDE_LEFT].long_pos_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_lat_pos_m", p_esa_output->esa_object[FBK_SIDE_LEFT].lat_pos_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_long_speed_mps", p_esa_output->esa_object[FBK_SIDE_LEFT].long_speed_mps);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_lat_speed_mps", p_esa_output->esa_object[FBK_SIDE_LEFT].lat_speed_mps);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_ttc_s", p_esa_output->esa_object[FBK_SIDE_LEFT].ttc_s);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_ttp_s", p_esa_output->esa_object[FBK_SIDE_LEFT].ttp_s);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_decel_to_reach_host_speed",
                         p_esa_output->esa_object[FBK_SIDE_LEFT].decel_to_reach_host_speed_mps2);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_long_distance_m", p_esa_output->esa_object[FBK_SIDE_LEFT].long_distance_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_left_existence_prob", p_esa_output->esa_object[FBK_SIDE_LEFT].existence_prob);

   /* RIGHT SIDE */
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_f_esa_alert_right", p_esa_output->f_esa_alert[FBK_SIDE_RIGHT]);

   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_id", p_esa_output->esa_object[FBK_SIDE_RIGHT].id);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_index", p_esa_output->esa_object[FBK_SIDE_RIGHT].index);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_width_m", p_esa_output->esa_object[FBK_SIDE_RIGHT].width_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_length_m", p_esa_output->esa_object[FBK_SIDE_RIGHT].length_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_long_pos_m", p_esa_output->esa_object[FBK_SIDE_RIGHT].long_pos_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_lat_pos_m", p_esa_output->esa_object[FBK_SIDE_RIGHT].lat_pos_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_long_speed_mps", p_esa_output->esa_object[FBK_SIDE_RIGHT].long_speed_mps);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_lat_speed_mps", p_esa_output->esa_object[FBK_SIDE_RIGHT].lat_speed_mps);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_ttc_s", p_esa_output->esa_object[FBK_SIDE_RIGHT].ttc_s);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_ttp_s", p_esa_output->esa_object[FBK_SIDE_RIGHT].ttp_s);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_decel_to_reach_host_speed",
                         p_esa_output->esa_object[FBK_SIDE_RIGHT].decel_to_reach_host_speed_mps2);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_long_distance_m", p_esa_output->esa_object[FBK_SIDE_RIGHT].long_distance_m);
   ESA_STORE_VAL_MGR_WPR("ESA_Gen_obj_right_existence_prob", p_esa_output->esa_object[FBK_SIDE_RIGHT].existence_prob);
}
#endif /* BINARY_DEBUG */
