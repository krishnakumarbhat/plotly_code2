/**
 * @file recw_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for RECW.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_post_run.h"
#include "pa_reuse.h"
#include "recw_core_output_t.h"
#include "recw_input_t.h"
#include "recw_output_t.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "recw_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Function Declarations
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Recw_Output(const Recw_Output_T *p_recw_output);
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Recw_Post_Run(const Recw_Instance_T *p_recw_instance, const Recw_Input_T *p_recw_input, Recw_Output_T *p_recw_output)
// clang-format on
{
   const Recw_Core_Output_T *p_core_output;
   /* check NULL pointers */
   assert(NULL != p_recw_instance);
   assert(NULL != p_recw_input);
   assert(NULL != p_recw_output);

   p_core_output = &p_recw_instance->core_output;

   p_recw_output->recw_crash_probability        = p_core_output->recw_crash_prob_combined;
   p_recw_output->recw_ttc_s                    = p_core_output->recw_ttc;
   p_recw_output->recw_id                       = p_core_output->recw_id;
   p_recw_output->recw_unique_id                = p_core_output->recw_unique_id;
   p_recw_output->recw_alert_level              = p_core_output->recw_alert_level;
   p_recw_output->ttc_threshold_alert_level_1_s = p_core_output->ttc_threshold_alert_level_1;
   p_recw_output->ttc_threshold_alert_level_2_s = p_core_output->ttc_threshold_alert_level_2;

#ifdef BINARY_DEBUG
   Write_Recw_Output(p_recw_output);
#endif
}

#ifdef BINARY_DEBUG
static void Write_Recw_Output(const Recw_Output_T *p_recw_output)
{
   RECW_STORE_VAL_MGR_WPR("RECW_output_crash_probability", p_recw_output->recw_crash_probability);
   RECW_STORE_VAL_MGR_WPR("RECW_output_ttc_s", p_recw_output->recw_ttc_s);
   RECW_STORE_VAL_MGR_WPR("RECW_output_id", p_recw_output->recw_id);
   RECW_STORE_VAL_MGR_WPR("RECW_output_alert_level", p_recw_output->recw_alert_level);
   RECW_STORE_VAL_MGR_WPR("RECW_output_ttc_threshold_alert_level_1_s", p_recw_output->ttc_threshold_alert_level_1_s);
   RECW_STORE_VAL_MGR_WPR("RECW_output_ttc_threshold_alert_level_2_s", p_recw_output->ttc_threshold_alert_level_1_s);
}
#endif /* BINARY_DEBUG */
