/**
 * @file recw_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 post run logic for RECW.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
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

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Recw_Post_Run(const Recw_Instance_T *p_recw_instance, const Recw_Input_T *p_recw_input, Recw_Output_T *p_recw_output)
// clang-format on
{
   /* check NULL pointers */
   assert(NULL != p_recw_instance);
   assert(NULL != p_recw_input);
   assert(NULL != p_recw_output);

   /* Map core output to customer output */
   p_recw_output->recw_alert_level         = (uint8_t) p_recw_instance->core_output.recw_alert_level;
   p_recw_output->recw_id                  = p_recw_instance->core_output.recw_id;
   p_recw_output->recw_index               = p_recw_instance->core_output.recw_index;
   p_recw_output->recw_ttc                 = p_recw_instance->core_output.recw_ttc;
   p_recw_output->recw_crash_prob_braking  = p_recw_instance->core_output.recw_crash_prob_braking;
   p_recw_output->recw_crash_prob_combined = p_recw_instance->core_output.recw_crash_prob_combined;
   p_recw_output->recw_crash_prob_steering = p_recw_instance->core_output.recw_crash_prob_steering;
}
