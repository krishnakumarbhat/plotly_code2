/**
 * @file fbk_obj_ageing.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains functions that handle fbk object aging updates.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_obj_ageing.h"
#include "fbk_debug_interface.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "ml_saturated_math.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>
/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Fbk_Update_Object_Ageing(Fbk_Age_Ctr_T *fbk_obj_ages, Pa_Data_T *p_pa_data)
{
   uint8_t idx;

   assert(NULL != fbk_obj_ages);
   assert(NULL != p_pa_data);


   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      if ((PA_OBJ_STATUS_INVALID != p_pa_data->object_data[idx].status)
          && ((fbk_obj_ages->stage[idx] == p_pa_data->object_data[idx].status)
              || ((PA_OBJ_STATUS_MATURE == p_pa_data->object_data[idx].status)
                  && (PA_OBJ_STATUS_COASTED_IMPLAUSIBLE == fbk_obj_ages->stage[idx]))))
      {
         /*In case that stage remains the same compared to last cycle, increase it.*/
         Sat_Inc_Uint8(&(fbk_obj_ages->stage_age[idx]));
         fbk_obj_ages->stage[idx] = p_pa_data->object_data[idx].status;
      }
      else if (Fbk_Is_Obj_Coasted_Status_Implausible(&p_pa_data->object_data[idx]))
      {
         /*In case that the object coasted status is implausible, keep the previous stage age and only update the stage*/
         fbk_obj_ages->stage[idx] = PA_OBJ_STATUS_COASTED_IMPLAUSIBLE;
      }
      else
      {
         /*In all other cases the stage age shall be reset*/
         fbk_obj_ages->stage_age[idx] = FBK_ONE_UINT;
         fbk_obj_ages->stage[idx]     = p_pa_data->object_data[idx].status;
      }
      p_pa_data->object_data[idx].fbk_stage_age = fbk_obj_ages->stage_age[idx];
   }

   Binary_Fbk_Debug_Pass_Stage_Age(fbk_obj_ages);
}

void Fbk_Reset_Object_Ageing(Fbk_Age_Ctr_T *fbk_obj_ages)
{
   uint8_t idx;

   assert(NULL != fbk_obj_ages);

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      fbk_obj_ages->stage[idx]     = PA_OBJ_STATUS_INVALID;
      fbk_obj_ages->stage_age[idx] = FBK_ONE_UINT;
   }

   Binary_Fbk_Debug_Pass_Stage_Age(fbk_obj_ages);
}
