/**
 * @file cta_counters.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides holding logic for CTA of object criticality level.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_counters.h"
#include "cta_core_calibration_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_saturated_math.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

Cta_Crit_Level_T Cta_Process_Current_Alert_Level(Cta_Persistent_T *p_cta_persistent,
                                                 const Cta_Crit_Level_T warning_level,
                                                 const Cta_Mode_T cta_mode,
                                                 const uint8_t approach_direction,
                                                 const Cta_Object_Data_T *p_object_data,
                                                 const Cta_Core_Calibration_T *p_cta_cal)
{
   /* Asserts */
   assert(approach_direction < FBK_NUMBER_OF_SIDES);
   assert(warning_level <= CTA_NUM_CRIT_LEVEL);
   assert(NULL != p_cta_persistent);
   assert(NULL != p_object_data);
   assert(NULL != p_cta_cal);

   /*Update holding counter in case of an increasing or constant criticality level*/
   if (warning_level >= p_cta_persistent->previous_crit_level[cta_mode][approach_direction])
   {
      p_cta_persistent->warning_holding_counter[cta_mode][approach_direction] = FBK_ZERO_UINT;

      if (PA_INVALID_OBJ_ID != p_object_data->tracker_data.id)
      {
         p_cta_persistent->previous_crit_level[cta_mode][approach_direction]                  = warning_level;
         p_cta_persistent->previous_most_critical_obj_id[cta_mode][approach_direction]        = p_object_data->tracker_data.id;
         p_cta_persistent->previous_most_critical_unique_obj_id[cta_mode][approach_direction] = p_object_data->tracker_data.unique_id;
      }
   }
   else
   {
      /*Increase warning holding counter by 1*/
      Sat_Inc_Uint8(&p_cta_persistent->warning_holding_counter[cta_mode][approach_direction]);

      /*Check whether holding of warning exceeds the allowed number of cycle.*/
      if (p_cta_persistent->warning_holding_counter[cta_mode][approach_direction] > p_cta_cal->k_cta_cycle_count_hold_true_warning)
      {
         if ((Fbk_Is_True(p_cta_cal->k_cta_f_prevent_fall_back_to_critlevel_1)) && (CTA_CRIT_LEVEL_1 == warning_level))
         {
            /* Fallback prevention of criticality level as long as warning level was greater before and now equals level 1.*/
            p_cta_persistent->previous_crit_level[cta_mode][approach_direction] = CTA_CRIT_LEVEL_2;
         }
         else
         {
            p_cta_persistent->previous_crit_level[cta_mode][approach_direction] = warning_level;
            if (PA_INVALID_OBJ_ID != p_object_data->tracker_data.id)
            {
               if (CTA_CRIT_LEVEL_NONE != warning_level)
               {
                  /* Reset holding counter. */
                  p_cta_persistent->warning_holding_counter[cta_mode][approach_direction]       = FBK_ZERO_UINT;
                  p_cta_persistent->previous_most_critical_obj_id[cta_mode][approach_direction] = p_object_data->tracker_data.id;
                  p_cta_persistent->previous_most_critical_unique_obj_id[cta_mode][approach_direction] =
                     p_object_data->tracker_data.unique_id;
               }
               else
               {
                  /*Reset id when warning level is default. */
                  p_cta_persistent->previous_most_critical_obj_id[cta_mode][approach_direction]        = FBK_ZERO_UINT;
                  p_cta_persistent->previous_most_critical_unique_obj_id[cta_mode][approach_direction] = FBK_ZERO_UINT;
               }
            }
         }
      }
      else /*Hold the last side level*/
      {
         if ((NULL != p_object_data->persistent)
             && (p_cta_persistent->previous_most_critical_obj_id[cta_mode][approach_direction] == p_object_data->tracker_data.id))
         {
            /*When the same object causes the fall back of criticality level, it is given the permission to not be suppressed for a
            specified amount of cycles (at least for the previously highest crit level) within the crit level determination
            function.*/
            p_object_data->persistent
               ->crit_level_suppression_counter[cta_mode][(uint8_t) p_cta_persistent->previous_crit_level[cta_mode][approach_direction]
                                                          - FBK_ONE_UINT] = CTA_COUNTER_MAX;
         }
      }
   }

   return p_cta_persistent->previous_crit_level[cta_mode][approach_direction];
}


void Cta_Check_Stop_Level_Holding(Cta_Persistent_T *p_cta_persistent,
                                  const Cta_Object_Data_T *p_object,
                                  const boolean_T f_within_field_of_interest)
{
   uint8_t cta_mode;
   /* Asserts */
   assert(NULL != p_cta_persistent);
   assert(NULL != p_object);
   assert(FBK_SIDE_UNDEFINED != p_object->attributes->approach_side);

   for (cta_mode = FBK_ZERO_UINT; cta_mode < (uint8_t) CTA_NUM_MODES; cta_mode++)
   {
      if (p_object->tracker_data.id == p_cta_persistent->previous_most_critical_obj_id[cta_mode][p_object->attributes->approach_side])
      {
         if ((Fbk_Is_False(f_within_field_of_interest)) && (p_object->attributes->ttc < FBK_ZERO_F))
         {
            p_cta_persistent->warning_holding_counter[cta_mode][p_object->attributes->approach_side] = CTA_COUNTER_MAX;
         }
      }
   }
}
