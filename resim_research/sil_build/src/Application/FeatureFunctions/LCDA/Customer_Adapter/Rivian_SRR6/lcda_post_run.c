/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 post run logic for LCDA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_macros.h"
#include "lcda_core_output_t.h"
#include "pa_reuse.h"
#include <assert.h>
#include <string.h>

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/**
 * @brief Maps the LCDA status from core output to the Rivian specific LCDA status
 *
 * @return Rivian LCDA status
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
static Lcda_Rivian_Status_T Lcda_Map_Lcda_Status_To_Rivian(Lcda_Status_T lcda_status);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Lcda_Init_Output(Lcda_Output_T *p_lcda_output)
{
   /* coverity[misra_c_2012_rule_17_7_violation][Intentionally ignored return value of memset function since it is not required.] */
   memset(p_lcda_output, 0, sizeof(Lcda_Output_T));
}

void Lcda_Post_Run_Init(void)
{
}


/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_lcda_instance" points to a non-constant type.] */
void Lcda_Post_Run(Lcda_Instance_T *p_lcda_instance, const Lcda_Input_T *p_lcda_input, Lcda_Output_T *p_lcda_output, const Fbk_Output_T *p_fbk_output)
/* clang-format on */
{
   const Lcda_Core_Output_T *p_lcda_core_output;
   /* Check if all input pointers are valid */
   assert(NULL != p_lcda_instance);
   assert(NULL != p_lcda_input);
   assert(NULL != p_lcda_output);
   assert(NULL != p_fbk_output);

   p_lcda_core_output = &p_lcda_instance->core_output;

   /* Fill customer output from core output */
   p_lcda_output->lcda_status   = Lcda_Map_Lcda_Status_To_Rivian(p_lcda_core_output->lcda_status);
   p_lcda_output->f_bsw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->bsw_core_output.f_bsw_is_enabled);
   p_lcda_output->f_cvw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->cvw_core_output.f_cvw_is_enabled);

   p_lcda_output->bsw_alert_left  = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT];
   p_lcda_output->bsw_alert_right = (uint8_t) p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT];
   p_lcda_output->cvw_alert_left  = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT];
   p_lcda_output->cvw_alert_right = (uint8_t) p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT];

   p_lcda_output->bsw_id_left   = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_LEFT];
   p_lcda_output->bsw_id_right  = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];
   p_lcda_output->cvw_id_left   = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
   p_lcda_output->cvw_id_right  = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
   p_lcda_output->cvw_ttc_left  = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
   p_lcda_output->cvw_ttc_right = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
}

/*============================================================================*\
 * Local Function Definition
\*============================================================================*/

static Lcda_Rivian_Status_T Lcda_Map_Lcda_Status_To_Rivian(Lcda_Status_T lcda_status)
{
   Lcda_Rivian_Status_T lcda_rivian_status;

   switch (lcda_status)
   {
      case LCDA_STATUS_ACTIVE:
         lcda_rivian_status = RIVIAN_LCDA_ACTIVE;
         break;
      case LCDA_STATUS_DISABLED_BY_INPUT:
      case LCDA_STATUS_DISABLED_BY_CAL:
         lcda_rivian_status = RIVIAN_LCDA_DISABLED;
         break;
      case LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED:
         lcda_rivian_status = RIVIAN_LCDA_DEACTIVATED_LOW_EGO_SPEED;
         break;
      case LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED:
         lcda_rivian_status = RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED;
         break;
      case LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS:
         lcda_rivian_status = RIVIAN_LCDA_DEACTIVATED_LOW_CURVE_RADIUS;
         break;
      case LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR:
      default:
         lcda_rivian_status = RIVIAN_LCDA_DEACTIVATED_INTERNAL_ERROR;
         break;
   }

   return lcda_rivian_status;
}
