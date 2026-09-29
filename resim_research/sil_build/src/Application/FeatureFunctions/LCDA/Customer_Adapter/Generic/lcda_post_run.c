/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_macros.h"
#include "lcda_core_output_t.h"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "lcda_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Function Declarations
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output);
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output)
{
   uint8_t side_index;

   /* Assert */
   assert(NULL != p_lcda_output);

   p_lcda_output->lcda_status   = LCDA_STATUS_DISABLED_BY_INPUT;
   p_lcda_output->f_bsw_enabled = FBK_FALSE;
   p_lcda_output->f_cvw_enabled = FBK_FALSE;
   p_lcda_output->f_slc_enabled = FBK_FALSE;

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_lcda_output->bsw_alert[side_index]     = LCDA_ALERT_STATE_NONE;
      p_lcda_output->bsw_id[side_index]        = PA_INVALID_OBJ_ID;
      p_lcda_output->bsw_unique_id[side_index] = PA_INVALID_OBJ_ID;

      p_lcda_output->cvw_alert[side_index]     = LCDA_ALERT_STATE_NONE;
      p_lcda_output->cvw_id[side_index]        = PA_INVALID_OBJ_ID;
      p_lcda_output->cvw_unique_id[side_index] = PA_INVALID_OBJ_ID;
      p_lcda_output->cvw_ttc_s[side_index]     = LCDA_CVW_DEFAULT_NO_ALERT_TTC;

      p_lcda_output->slc_alert[side_index]                   = FBK_FALSE;
      p_lcda_output->slc_id[side_index]                      = PA_INVALID_OBJ_ID;
      p_lcda_output->slc_unique_id[side_index]               = PA_INVALID_OBJ_ID;
      p_lcda_output->slc_ttc_s[side_index]                   = LCDA_DEFAULT_LARGE_TTC;
      p_lcda_output->slc_lane_change_probability[side_index] = LCDA_SLC_PROBABILITY_NONE;
   }
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run_Init(void)
{
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   uint8_t side_index;
   const Lcda_Core_Output_T *p_lcda_core_output;

   /* Asserts */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);
   assert(NULL != p_fbk_output);

   p_lcda_core_output = &p_lcda_instance->core_output;
   /* Pass output vlaues */
   p_lcda_output->lcda_status   = p_lcda_core_output->lcda_status;
   p_lcda_output->f_bsw_enabled = p_lcda_core_output->bsw_core_output.f_bsw_is_enabled;
   p_lcda_output->f_cvw_enabled = p_lcda_core_output->cvw_core_output.f_cvw_is_enabled;
   p_lcda_output->f_slc_enabled = p_lcda_core_output->slc_core_output.f_slc_is_enabled;

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      p_lcda_output->bsw_alert[side_index]     = p_lcda_core_output->bsw_core_output.bsw_alert[side_index];
      p_lcda_output->bsw_id[side_index]        = p_lcda_core_output->bsw_core_output.bsw_id[side_index];
      p_lcda_output->bsw_unique_id[side_index] = p_lcda_core_output->bsw_core_output.bsw_unique_id[side_index];

      p_lcda_output->cvw_alert[side_index]     = p_lcda_core_output->cvw_core_output.cvw_alert[side_index];
      p_lcda_output->cvw_id[side_index]        = p_lcda_core_output->cvw_core_output.cvw_id[side_index];
      p_lcda_output->cvw_unique_id[side_index] = p_lcda_core_output->cvw_core_output.cvw_unique_id[side_index];
      p_lcda_output->cvw_ttc_s[side_index]     = p_lcda_core_output->cvw_core_output.cvw_ttc[side_index];

      p_lcda_output->slc_alert[side_index]                   = p_lcda_core_output->slc_core_output.slc_alert[side_index];
      p_lcda_output->slc_id[side_index]                      = p_lcda_core_output->slc_core_output.slc_id[side_index];
      p_lcda_output->slc_unique_id[side_index]               = p_lcda_core_output->slc_core_output.slc_unique_id[side_index];
      p_lcda_output->slc_ttc_s[side_index]                   = p_lcda_core_output->slc_core_output.slc_lat_ttc[side_index];
      p_lcda_output->slc_lane_change_probability[side_index] = p_lcda_core_output->slc_core_output.slc_lane_change_prob[side_index];
   }

#ifdef BINARY_DEBUG
   Write_Lcda_Output(p_lcda_output);
#endif
}

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output)
{
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_f_lcda_status", p_lcda_output->lcda_status);

   LCDA_STORE_VAL_MGR_WPR("Lcda_out_f_bsw_enabled", p_lcda_output->f_bsw_enabled);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_bsw_alert_left", p_lcda_output->bsw_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_bsw_alert_right", p_lcda_output->bsw_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_bsw_id_left", p_lcda_output->bsw_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_bsw_id_right", p_lcda_output->bsw_id[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("Lcda_out_f_cvw_enabled", p_lcda_output->f_cvw_enabled);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_cvw_alert_left", p_lcda_output->cvw_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_cvw_alert_right", p_lcda_output->cvw_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_cvw_id_left", p_lcda_output->cvw_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_cvw_id_right", p_lcda_output->cvw_id[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_cvw_ttc_s_left", p_lcda_output->cvw_ttc_s[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_cvw_ttc_s_right", p_lcda_output->cvw_ttc_s[FBK_SIDE_RIGHT]);

   LCDA_STORE_VAL_MGR_WPR("Lcda_out_f_slc_enabled", p_lcda_output->f_slc_enabled);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_alert_left", p_lcda_output->slc_alert[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_alert_right", p_lcda_output->slc_alert[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_id_left", p_lcda_output->slc_id[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_id_right", p_lcda_output->slc_id[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_ttc_s_left", p_lcda_output->slc_ttc_s[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_ttc_s_right", p_lcda_output->slc_ttc_s[FBK_SIDE_RIGHT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_lane_change_probability_left", p_lcda_output->slc_lane_change_probability[FBK_SIDE_LEFT]);
   LCDA_STORE_VAL_MGR_WPR("Lcda_out_slc_lane_change_probability_right", p_lcda_output->slc_lane_change_probability[FBK_SIDE_RIGHT]);
}
#endif /* BINARY_DEBUG */
