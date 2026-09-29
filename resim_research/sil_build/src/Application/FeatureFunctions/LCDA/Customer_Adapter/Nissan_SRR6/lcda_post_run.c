/**
 * @file lcda_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Nissan SRR6 post run logic for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_post_run.h"
#include "fbk_macros.h"
#include "lcda_core_output_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>
#include <string.h>

#ifdef BINARY_DEBUG
#include "lcda_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define LCDA_NISSAN_DEFAULT_TTC (7.875f)

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Resets the LCDA Nissan SRR6 specific output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-6993}
 * @verification{}
 */
static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output);

/*===========================================================================*\
* Global Function Definitions
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

   /*Reset Lcda output*/
   Lcda_Reset_Output(p_lcda_output);

   /* Check general LCDA state */
   if (LCDA_STATUS_ACTIVE == p_lcda_core_output->lcda_status)
   {
      p_lcda_output->f_lcda_enabled = FBK_ONE_UINT;
   }

   /* Check enable flags */
   p_lcda_output->f_bsw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->bsw_core_output.f_bsw_is_enabled);
   p_lcda_output->f_cvw_enabled = Fbk_Convert_Bool_To_Uint(p_lcda_core_output->cvw_core_output.f_cvw_is_enabled);

   /* Map BSW core outputs to Nissan outputs */
   if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_LEFT])
   {
      p_lcda_output->bsw_alert_left = FBK_ONE_UINT;
      p_lcda_output->bsw_id_left    = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_LEFT];
   }


   if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->bsw_core_output.bsw_alert[FBK_SIDE_RIGHT])
   {
      p_lcda_output->bsw_alert_right = FBK_ONE_UINT;
      p_lcda_output->bsw_id_right    = p_lcda_core_output->bsw_core_output.bsw_id[FBK_SIDE_RIGHT];
   }


   /* Map CVW core outputs to Nissan outputs */
   if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_LEFT])
   {
      p_lcda_output->cvw_alert_left = FBK_ONE_UINT;
      p_lcda_output->cvw_id_left    = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_LEFT];
      p_lcda_output->cvw_ttc_left   = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_LEFT];
   }

   if (LCDA_ALERT_STATE_NONE != p_lcda_core_output->cvw_core_output.cvw_alert[FBK_SIDE_RIGHT])
   {
      p_lcda_output->cvw_alert_right = FBK_ONE_UINT;
      p_lcda_output->cvw_id_right    = p_lcda_core_output->cvw_core_output.cvw_id[FBK_SIDE_RIGHT];
      p_lcda_output->cvw_ttc_right   = p_lcda_core_output->cvw_core_output.cvw_ttc[FBK_SIDE_RIGHT];
   }

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Lcda_Output(p_lcda_output);
#endif
}


/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Lcda_Reset_Output(Lcda_Output_T *p_lcda_output)
{
   /*Reset flags*/
   p_lcda_output->f_lcda_enabled = FBK_ZERO_UINT;
   p_lcda_output->f_bsw_enabled  = FBK_ZERO_UINT;
   p_lcda_output->f_cvw_enabled  = FBK_ZERO_UINT;

   /*Alert level*/
   p_lcda_output->bsw_alert_left  = FBK_ZERO_UINT;
   p_lcda_output->bsw_alert_right = FBK_ZERO_UINT;
   p_lcda_output->cvw_alert_left  = FBK_ZERO_UINT;
   p_lcda_output->cvw_alert_right = FBK_ZERO_UINT;

   /*Additional properties*/
   p_lcda_output->cvw_id_left   = PA_INVALID_OBJ_ID;
   p_lcda_output->cvw_id_right  = PA_INVALID_OBJ_ID;
   p_lcda_output->bsw_id_left   = PA_INVALID_OBJ_ID;
   p_lcda_output->bsw_id_right  = PA_INVALID_OBJ_ID;
   p_lcda_output->cvw_ttc_left  = LCDA_NISSAN_DEFAULT_TTC;
   p_lcda_output->cvw_ttc_right = LCDA_NISSAN_DEFAULT_TTC;
}

#ifdef BINARY_DEBUG
static void Write_Lcda_Output(const Lcda_Output_T *p_lcda_output)
{
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_f_lcda_enabled", p_lcda_output->f_lcda_enabled);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_f_bsw_enabled", p_lcda_output->f_bsw_enabled);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_f_cvw_enabled", p_lcda_output->f_cvw_enabled);

   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_bsw_alert_left", p_lcda_output->bsw_alert_left);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_bsw_id_left", p_lcda_output->bsw_id_left);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_bsw_alert_right", p_lcda_output->bsw_alert_right);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_bsw_id_right", p_lcda_output->bsw_id_right);

   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_cvw_alert_left", p_lcda_output->cvw_alert_left);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_cvw_id_left", p_lcda_output->cvw_id_left);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_cvw_ttc_left", p_lcda_output->cvw_ttc_left);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_cvw_alert_right", p_lcda_output->cvw_alert_right);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_cvw_id_right", p_lcda_output->cvw_id_right);
   LCDA_STORE_VAL_MGR_WPR("nissan_srr6_cvw_ttc_right", p_lcda_output->cvw_ttc_right);
}
#endif
