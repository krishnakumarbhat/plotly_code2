/**
 * @file ta_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 specific pre run logic for TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "ta_pre_run.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include <assert.h>

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ta_Init_Input(Ta_Input_T *p_ta_input, const Radar_Position_T radar_position)
{
   /* Assert */
   assert(NULL != p_ta_input);

   /* Check mounting position for enabling TA */
   if (INVALID_POSITION != radar_position)
   {
      /* Enable the TA Algorithm */
      p_ta_input->f_ta_enable        = FBK_TRUE;
      p_ta_input->f_fta_enable       = FBK_TRUE;
      p_ta_input->f_rta_enable       = FBK_TRUE;
      p_ta_input->ta_warntrigger_hmi = TA_RIVIAN_SRR6_WARNTRIGGER_NORMAL;
   }
}

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ta_Pre_Run(Ta_Instance_T *p_ta_instance /**< TA Instance */,
                const Ta_Input_T *p_ta_input /**< TA Input */,
                const Fbk_Output_T *p_fbk_output)
{
   Ta_Core_Input_T *p_ta_core_input;
   const Ta_Core_Calibration_T *p_ta_cal;

   /* Check if all input pointers are valid */
   assert(NULL != p_ta_instance);
   assert(NULL != p_ta_input);
   assert(NULL != p_fbk_output);

   p_ta_core_input = &p_ta_instance->core_input;
   p_ta_cal        = &p_ta_instance->calibration;

   /* Update context pointer. */
   p_ta_core_input->p_pa_data = p_fbk_output->p_pa_data;

   /* Enable the Core TA Algorithm */
   p_ta_core_input->f_ta_enable  = p_ta_input->f_ta_enable;
   p_ta_core_input->f_fta_enable = p_ta_input->f_fta_enable;
   p_ta_core_input->f_rta_enable = p_ta_input->f_rta_enable;

   /* Set ttp_threshold dependent on the given Hmi */
   if (TA_RIVIAN_SRR6_WARNTRIGGER_LATE == p_ta_input->ta_warntrigger_hmi)
   {
      p_ta_core_input->alert_ttp_threshold = p_ta_cal->k_ta_alert_lvl_1_ttp_threshold[TA_RIVIAN_SRR6_WARNTRIGGER_LATE];
   }
   else if (TA_RIVIAN_SRR6_WARNTRIGGER_EARLY == p_ta_input->ta_warntrigger_hmi)
   {
      p_ta_core_input->alert_ttp_threshold = p_ta_cal->k_ta_alert_lvl_1_ttp_threshold[TA_RIVIAN_SRR6_WARNTRIGGER_EARLY];
   }
   else
   {
      p_ta_core_input->alert_ttp_threshold = p_ta_cal->k_ta_alert_lvl_1_ttp_threshold[TA_RIVIAN_SRR6_WARNTRIGGER_NORMAL];
   }
}
