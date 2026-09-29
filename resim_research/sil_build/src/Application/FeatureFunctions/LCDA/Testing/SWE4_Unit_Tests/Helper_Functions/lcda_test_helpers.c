/**
 * @file c_testing_main.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file for LCDA test helper functions.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "lcda_test_helpers.h"
#include "fbk_macros.h"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

boolean_T Lcda_Is_Core_Output_Default(Lcda_Core_Output_T *p_core_output)
{
   return ((p_core_output->bsw_core_output.f_bsw_is_enabled == 0u) && (p_core_output->cvw_core_output.f_cvw_is_enabled == 0u)
           && (p_core_output->slc_core_output.f_slc_is_enabled == 0u) && (p_core_output->elc_core_output.f_elc_is_enabled == 0u)
           && (Lcda_Is_Bsw_Output_Default(&p_core_output->bsw_core_output))
           && (Lcda_Is_Cvw_Output_Default(&p_core_output->cvw_core_output))
           && (Lcda_Is_Slc_Output_Default(&p_core_output->slc_core_output))
           && (Lcda_Is_Elc_Output_Default(&p_core_output->elc_core_output)));
}

boolean_T Lcda_Is_Bsw_Output_Default(Lcda_Bsw_Core_Output_T *p_bsw_core_output)
{
   uint8_t i_side;

   for (i_side = 0; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      if (p_bsw_core_output->bsw_alert[i_side] != LCDA_ALERT_STATE_NONE
          || p_bsw_core_output->bsw_index[i_side] != PA_INVALID_OBJ_INDEX || p_bsw_core_output->bsw_id[i_side] != PA_INVALID_OBJ_ID)
      {
         return FBK_FALSE;
      }
   }
   return FBK_TRUE;
}

boolean_T Lcda_Is_Cvw_Output_Default(Lcda_Cvw_Core_Output_T *p_cvw_core_output)
{
   uint8_t i_side;

   for (i_side = 0; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      if (p_cvw_core_output->cvw_alert[i_side] != LCDA_ALERT_STATE_NONE
          || p_cvw_core_output->cvw_index[i_side] != PA_INVALID_OBJ_INDEX || p_cvw_core_output->cvw_id[i_side] != PA_INVALID_OBJ_ID)
      {
         return FBK_FALSE;
      }
   }
   return FBK_TRUE;
}

boolean_T Lcda_Is_Slc_Output_Default(Lcda_Slc_Core_Output_T *p_slc_core_output)
{
   uint8_t i_side;

   for (i_side = 0; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      if (p_slc_core_output->slc_alert[i_side] != 0u || p_slc_core_output->slc_index[i_side] != PA_INVALID_OBJ_INDEX
          || p_slc_core_output->slc_id[i_side] != PA_INVALID_OBJ_ID || p_slc_core_output->slc_lane_change_prob[i_side] != 0.0f
          || p_slc_core_output->slc_lat_ttc[i_side] != LCDA_DEFAULT_LARGE_TTC
          || p_slc_core_output->slc_lon_ttc[i_side] != LCDA_DEFAULT_LARGE_TTC)
      {
         return FBK_FALSE;
      }
   }
   return FBK_TRUE;
}

boolean_T Lcda_Is_Elc_Output_Default(Lcda_Elc_Core_Output_T *p_elc_core_output)
{
   uint8_t i_side;

   for (i_side = 0; i_side < FBK_NUMBER_OF_SIDES; i_side++)
   {
      if (p_elc_core_output->elc_alert[i_side] != 0u || p_elc_core_output->elc_index[i_side] != PA_INVALID_OBJ_INDEX
          || p_elc_core_output->elc_id[i_side] != PA_INVALID_OBJ_ID || p_elc_core_output->elc_decel_to_reach_host_speed[i_side] != 0.0f
          || p_elc_core_output->elc_ttc[i_side] != LCDA_DEFAULT_LARGE_TTC)
      {
         return FBK_FALSE;
      }
   }
   return FBK_TRUE;
}
