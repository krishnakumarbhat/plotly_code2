/**
 * @file fbk_fill_pa_data.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions to get the platform abstraction data.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
* Includes
\*============================================================================*/

#include "fbk_guardrail_data_t.h"
#include "fbk_object_data_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"


#include "fbk_fill_pa_data.h"
#include "pa_selector.h"
#include <assert.h>

#include "fbk_guardrail_validation.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_validation.h"
#include "pa_obj_in.h"

Sfl_Status_T Fbk_Fill_Pa_Data(Pa_Data_T *output, Pa_Context_T *p_context)
{
   uint8_t obj_index;
   uint8_t side;
   Sfl_Status_T sfl_status = SFL_STATUS_OK;

   assert(NULL != output);
   assert(NULL != p_context);
#if defined PA_Generic
   if (p_context->p_data == output)
   {
      return sfl_status;
   }
#else
   Pa_Get_Perception_Data(p_context);
#endif

   /* Iterate through all objects to validate and fill object data */
   for (obj_index = 0; obj_index < PA_OBJ_NUMBER_OF_OBJECTS; obj_index++)
   {
      /* Fill the object information structure with data from context */
      Fbk_Fill_Object_Information(&output->object_data[obj_index], p_context, obj_index);
      /* Verify that each object's data is within valid ranges */
      if (Fbk_Is_False(Fbk_Verify_Object_Data_Range(&output->object_data[obj_index])))
      {
         /* If any object data is invalid, set error status and exit the loop */
         sfl_status = SFL_STATUS_OBJ_DATA_ERROR;
      }
   }

   /* Fill vehicle information structure with data from context */
   Fbk_Fill_Vehicle_Information(&output->vehicle_data, p_context);

   /* Validate vehicle data and update status if invalid */
   if (Fbk_Is_False(Fbk_Verify_Vehicle_Data_Range(&output->vehicle_data)))
   {
      sfl_status = SFL_STATUS_VEH_DATA_ERROR;
   }

   /* Process guardrail data for all sides */
   for (side = 0; side < PA_OBJ_NUMBER_OF_GUARDRAILS; side++)
   {
      /* Fill guardrail information structure with data from context */
      Fbk_Fill_Guardrail_Information(&output->guardrail_data[side], p_context, side);
   }
   /* Store the time difference since the last processing cycle */
   output->time_diff_to_last_cycle = Pa_Get_Time_Diff_To_Last_Cycle(p_context);

   /* Return the status code indicating success or specific error condition */
   return sfl_status;
}
