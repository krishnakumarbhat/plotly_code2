/**
 * @file recw_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for RECW.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_pre_run.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "recw_core_input_t.h"
#include "recw_input_t.h"

#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_recw_instance" points to a non-constant type.] */
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Recw_Init_Input(Recw_Input_T *p_recw_input /**< RECW Input */)
{
   assert(NULL != p_recw_input);
   p_recw_input->f_recw_enable = FBK_ONE_UINT;
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_recw_instance" points to a non-constant type.] */
void Recw_Pre_Run_Init(Recw_Instance_T *p_recw_instance)
{
   assert(NULL != p_recw_instance);
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed]  */
void Recw_Pre_Run(Recw_Instance_T *p_recw_instance, const Recw_Input_T *p_recw_input, const Fbk_Output_T *p_fbk_output)
{
   /* check input parameters */
   assert(NULL != p_recw_instance);
   assert(NULL != p_recw_input);
   assert(NULL != p_fbk_output);

   p_recw_instance->core_input.p_pa_data     = p_fbk_output->p_pa_data;
   p_recw_instance->core_input.f_enable_recw = (boolean_T) Fbk_Is_True(p_recw_input->f_recw_enable);
}
