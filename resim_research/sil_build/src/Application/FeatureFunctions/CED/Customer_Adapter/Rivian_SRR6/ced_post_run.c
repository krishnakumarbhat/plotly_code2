/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 post run logic for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "ced_post_run.h"
#include "ced_core_output_t.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

/*============================================================================*\
* EXPORTED FUNCTIONS
\*============================================================================*/

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
}

// clang-format off
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run(Ced_Instance_T *p_ced_instance, const Ced_Input_T *p_ced_input,  Ced_Output_T *p_ced_output)
// clang-format on
{
   assert(NULL != p_ced_output);
   assert(NULL != p_ced_input);
   assert(NULL != p_ced_instance);

   /* Map core output to customer output */
   p_ced_output->ced_id_left    = p_ced_instance->core_output.ced_id[FBK_SIDE_LEFT];
   p_ced_output->ced_alert_left = (uint8_t) p_ced_instance->core_output.ced_alert[FBK_SIDE_LEFT];
   p_ced_output->ced_ttc_left   = p_ced_instance->core_output.ced_ttc[FBK_SIDE_LEFT];
   p_ced_output->ced_ttp_left   = p_ced_instance->core_output.ced_ttp[FBK_SIDE_LEFT];

   p_ced_output->ced_id_right    = p_ced_instance->core_output.ced_id[FBK_SIDE_RIGHT];
   p_ced_output->ced_alert_right = (uint8_t) p_ced_instance->core_output.ced_alert[FBK_SIDE_RIGHT];
   p_ced_output->ced_ttc_right   = p_ced_instance->core_output.ced_ttc[FBK_SIDE_RIGHT];
   p_ced_output->ced_ttp_right   = p_ced_instance->core_output.ced_ttp[FBK_SIDE_RIGHT];
}
