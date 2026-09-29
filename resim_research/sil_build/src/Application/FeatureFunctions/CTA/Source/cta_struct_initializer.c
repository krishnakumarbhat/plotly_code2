/**
 * @file cta_struct_initializer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Definition of struct initialization functions of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_struct_initializer.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Cta_Reset_Core_Output(Cta_Core_Output_T *p_cta_core_output)
{
   /* Iterator variable */
   uint8_t side_index;
   uint8_t mode_index;

   /* Assert */
   assert(NULL != p_cta_core_output);

   p_cta_core_output->cta_status = CTA_STATUS_DISABLED;

   /* Reset the CTA core output for all couples of (mode, approach_side)*/
   for (mode_index = FBK_ZERO_UINT; mode_index < (uint8_t) CTA_NUM_MODES; mode_index++)
   {
      for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
      {
         p_cta_core_output->cta_id[mode_index][side_index]                 = PA_INVALID_OBJ_ID;
         p_cta_core_output->cta_unique_id[mode_index][side_index]          = PA_INVALID_OBJ_ID;
         p_cta_core_output->cta_index[mode_index][side_index]              = PA_INVALID_OBJ_INDEX;
         p_cta_core_output->cta_alert_level[mode_index][side_index]        = CTA_CRIT_LEVEL_NONE;
         p_cta_core_output->f_brake_qualifier[mode_index][side_index]      = FBK_FALSE;
         p_cta_core_output->cta_obj_ttc[mode_index][side_index]            = CTA_HIGH_DEFAULT_VAL;
         p_cta_core_output->cta_obj_ttp[mode_index][side_index]            = CTA_HIGH_DEFAULT_VAL;
         p_cta_core_output->cta_long_intersection[mode_index][side_index]  = CTA_HIGH_DEFAULT_VAL;
         p_cta_core_output->cta_heading[mode_index][side_index]            = FBK_ZERO_F;
         p_cta_core_output->cta_warn_hold_cnt[mode_index][side_index]      = FBK_ZERO_UINT;
         p_cta_core_output->cta_brake_hold_cnt[mode_index][side_index]     = FBK_ZERO_UINT;
         p_cta_core_output->cta_brake_supp_cnt[mode_index][side_index]     = FBK_ZERO_UINT;
         p_cta_core_output->f_standstill_qualifier[mode_index][side_index] = FBK_FALSE;
         p_cta_core_output->brake_deceleration[mode_index][side_index]     = FBK_ZERO_F;
      }
   }
}

void Cta_Reset_Persistent(Cta_Persistent_T *p_cta_persistent)
{
   uint8_t side_idx;
   uint8_t mode_index;

   /* Assert */
   assert(NULL != p_cta_persistent);

   p_cta_persistent->f_function_execution_state = FBK_FALSE;
   for (mode_index = FBK_ZERO_UINT; mode_index < (uint8_t) CTA_NUM_MODES; mode_index++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
      {
         p_cta_persistent->previous_brake_qualifier[mode_index][side_idx]             = FBK_FALSE;
         p_cta_persistent->brake_suppression_counter[mode_index][side_idx]            = FBK_ZERO_UINT;
         p_cta_persistent->warning_holding_counter[mode_index][side_idx]              = FBK_ZERO_UINT;
         p_cta_persistent->brake_holding_counter[mode_index][side_idx]                = FBK_ZERO_UINT;
         p_cta_persistent->previous_most_critical_obj_id[mode_index][side_idx]        = FBK_ZERO_UINT;
         p_cta_persistent->previous_most_critical_unique_obj_id[mode_index][side_idx] = FBK_ZERO_UINT;
         p_cta_persistent->previous_crit_level[mode_index][side_idx]                  = CTA_CRIT_LEVEL_NONE;
      }
   }
}
