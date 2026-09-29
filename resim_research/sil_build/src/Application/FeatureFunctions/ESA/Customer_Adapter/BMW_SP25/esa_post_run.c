/**
 * @file esa_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 post run logic for ESA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_post_run.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
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
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 * @verification{}
 */
static void Esa_Update_Output(Esa_Output_T *p_esa_output /**< ESA output data */,
                              const Esa_Core_Output_T *p_esa_core_output /**< ESA core output data */,
                              const Esa_Instance_T *p_esa_instance /**< ESA instance */);

/**
 * @brief Reset ESA output.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 * @verification{}
 */
static void Esa_Reset_Output(Esa_Output_T *p_esa_output /**< ESA output data */);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_esa_instance" points to a non-constant type] */
void Esa_Post_Run_Init(Esa_Instance_T *p_esa_instance)
{
   assert(NULL != p_esa_instance);
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Esa_Post_Run(const Esa_Instance_T *p_esa_instance, const Esa_Input_T *p_esa_input, Esa_Output_T *p_esa_output)
/* clang-format on */
{
   assert(NULL != p_esa_instance);
   assert(NULL != p_esa_input);
   assert(NULL != p_esa_output);

   Esa_Reset_Output(p_esa_output);
   Esa_Update_Output(p_esa_output, &(p_esa_instance->core_output), p_esa_instance);

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Esa_Output(p_esa_output);
#endif
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Esa_Update_Output(Esa_Output_T *p_esa_output, const Esa_Core_Output_T *p_esa_core_output, const Esa_Instance_T *p_esa_instance)
{
   uint8_t index;
   const Fbk_Object_Data_T *p_tracker_object;

   if (ESA_CORE_STATUS_ACTIVE == p_esa_core_output->esa_core_status)
   {
      if (Fbk_Is_True(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]))
      {
         index                                           = p_esa_core_output->esa_index[FBK_SIDE_LEFT];
         p_tracker_object                                = &(p_esa_instance->core_input.p_pa_data->object_data[index]);
         p_esa_output->Esa_Status_Left                   = ESA_STATUS_VEHICLE_IN_ZONE;
         p_esa_output->Object_ID_Left                    = p_esa_core_output->esa_id[FBK_SIDE_LEFT];
         p_esa_output->Object_Timestamp_Left             = FBK_ZERO_F; /* mocked temporarily due to no signal from the Tracker */
         p_esa_output->Object_Position_X_Left            = p_tracker_object->curvi_pos.x;
         p_esa_output->Object_Position_Y_Left            = p_tracker_object->curvi_pos.y;
         p_esa_output->Object_Width_Left                 = p_tracker_object->width;
         p_esa_output->Object_Length_Left                = p_tracker_object->length;
         p_esa_output->Object_Speed_X_Left               = p_tracker_object->curvi_vel.x;
         p_esa_output->Object_Speed_Y_Left               = p_tracker_object->curvi_vel.y;
         p_esa_output->Object_Time_To_Pass_Left          = p_esa_core_output->esa_ttp[FBK_SIDE_LEFT];
         p_esa_output->Object_Existence_Probability_Left = 100.0f * p_tracker_object->existence_probability;
      }

      if (Fbk_Is_True(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]))
      {
         index                                            = p_esa_core_output->esa_index[FBK_SIDE_RIGHT];
         p_tracker_object                                 = &(p_esa_instance->core_input.p_pa_data->object_data[index]);
         p_esa_output->Esa_Status_Right                   = ESA_STATUS_VEHICLE_IN_ZONE;
         p_esa_output->Object_ID_Right                    = p_esa_core_output->esa_id[FBK_SIDE_RIGHT];
         p_esa_output->Object_Timestamp_Right             = FBK_ZERO_F; /* mocked temporarily due to no signal from the Tracker */
         p_esa_output->Object_Position_X_Right            = p_tracker_object->curvi_pos.x;
         p_esa_output->Object_Position_Y_Right            = p_tracker_object->curvi_pos.y;
         p_esa_output->Object_Width_Right                 = p_tracker_object->width;
         p_esa_output->Object_Length_Right                = p_tracker_object->length;
         p_esa_output->Object_Speed_X_Right               = p_tracker_object->curvi_vel.x;
         p_esa_output->Object_Speed_Y_Right               = p_tracker_object->curvi_vel.y;
         p_esa_output->Object_Time_To_Pass_Right          = p_esa_core_output->esa_ttp[FBK_SIDE_RIGHT];
         p_esa_output->Object_Existence_Probability_Right = 100.0f * p_tracker_object->existence_probability;
      }
   }
   else
   {
      p_esa_output->Esa_Status_Left  = ESA_STATUS_OUTSIDE_SYSTEM_BOUNDARIES;
      p_esa_output->Esa_Status_Right = ESA_STATUS_OUTSIDE_SYSTEM_BOUNDARIES;
   }
}


static void Esa_Reset_Output(Esa_Output_T *p_esa_output)
{
   p_esa_output->Esa_Status_Left                    = ESA_STATUS_NO_VEHICLE_IN_ZONE;
   p_esa_output->Esa_Status_Right                   = ESA_STATUS_NO_VEHICLE_IN_ZONE;
   p_esa_output->Object_ID_Left                     = FBK_ZERO_UINT;
   p_esa_output->Object_ID_Right                    = FBK_ZERO_UINT;
   p_esa_output->Object_Timestamp_Left              = FBK_ZERO_F;
   p_esa_output->Object_Timestamp_Right             = FBK_ZERO_F;
   p_esa_output->Object_Position_X_Left             = FBK_ZERO_F;
   p_esa_output->Object_Position_X_Right            = FBK_ZERO_F;
   p_esa_output->Object_Position_Y_Left             = FBK_ZERO_F;
   p_esa_output->Object_Position_Y_Right            = FBK_ZERO_F;
   p_esa_output->Object_Width_Left                  = FBK_ZERO_F;
   p_esa_output->Object_Width_Right                 = FBK_ZERO_F;
   p_esa_output->Object_Length_Left                 = FBK_ZERO_F;
   p_esa_output->Object_Length_Right                = FBK_ZERO_F;
   p_esa_output->Object_Speed_X_Left                = FBK_ZERO_F;
   p_esa_output->Object_Speed_X_Right               = FBK_ZERO_F;
   p_esa_output->Object_Speed_Y_Left                = FBK_ZERO_F;
   p_esa_output->Object_Speed_Y_Right               = FBK_ZERO_F;
   p_esa_output->Object_Time_To_Pass_Left           = FBK_ZERO_F;
   p_esa_output->Object_Time_To_Pass_Right          = FBK_ZERO_F;
   p_esa_output->Object_Existence_Probability_Left  = FBK_ZERO_F;
   p_esa_output->Object_Existence_Probability_Right = FBK_ZERO_F;
}


#ifdef BINARY_DEBUG
static void Write_Esa_Output(const Esa_Output_T *p_esa_output)
{
   /* check input parameters */
   assert(NULL != p_esa_output);

   /* Log Safe Exit specific data*/
}
#endif /* BINARY_DEBUG */
