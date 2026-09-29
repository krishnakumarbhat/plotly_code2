/**
 * @file esa_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic pre run logic for ESA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "esa_pre_run.h"
#include "esa_core_input_t.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "esa_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Esa_Input(const Esa_Input_T *p_esa_input);
#endif /* BINARY_DEBUG */

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Esa_Init_Input(Esa_Input_T *p_esa_input)
{
   assert(NULL != p_esa_input);

   p_esa_input->f_esa_enabled = FBK_TRUE;
}


/**
 * @brief Runs the Generic ESA pre-run.
 *
 * @return void
 *
 * @SRD{CSCSA-122858}
 * @SAD{CSCSA-83854}
 * @SDD{CSCSA-123065}
 * @verification{Set generic Esa_Input_T and tracker data by non-defaults, run Esa_Pre_Run function, verify Esa_Core_Input_T if
 * values are set to corrending values.}
 */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_esa_instance" points to a non-constant type.] */
void Esa_Pre_Run_Init(Esa_Instance_T *p_esa_instance)
{
   assert(NULL != p_esa_instance);
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Esa_Pre_Run(Esa_Instance_T *p_esa_instance, const Esa_Input_T *p_esa_input, const Fbk_Output_T *p_fbk_output)
{
   assert(NULL != p_esa_instance);
   assert(NULL != p_esa_input);
   assert(NULL != p_fbk_output);

   p_esa_instance->core_input.f_esa_enabled = p_esa_input->f_esa_enabled;
   p_esa_instance->core_input.p_pa_data     = p_fbk_output->p_pa_data;

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Esa_Input(p_esa_input);
#endif /* BINARY_DEBUG */
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Esa_Input(const Esa_Input_T *p_esa_input)
{
   /* check input parameters */
   assert(NULL != p_esa_input);

   ESA_STORE_VAL_MGR_WPR("ESA_Gen_in_f_esa_enabled", p_esa_input->f_esa_enabled);
}
#endif /* BINARY_DEBUG */
