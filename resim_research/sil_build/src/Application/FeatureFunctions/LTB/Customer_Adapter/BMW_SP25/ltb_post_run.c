/**
 * @file ltb_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 post run logic for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_post_run.h"
#include "fbk_macros.h"
#include "ltb_bmw_sp25_types.h" // for LTB_STATE_ACTIVE, Ltb_State_T
#include "ltb_core_output_t.h"
#include "ltb_state_machine.h"
#include "ltb_types.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ltb_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Defines
\*===========================================================================*/


/*===========================================================================*\
* Local Data Prototypes
\*===========================================================================*/

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ltb_Output(const Ltb_Output_T *p_ltb_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Reset all signals in BMW specific output.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ltb_Reset_Output(Ltb_Output_T *p_ltb_output /**< LTB output data */);

/**
 * @brief Transfer information from Algo-State to BMW specific Bus representation.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ltb_Set_Output(Ltb_Output_T *p_ltb_output /**< BMW output data */,
                           const Ltb_Core_Output_T *p_ltb_core_output /**< LTB core output */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

// clang-format off
void Ltb_Post_Run(const Ltb_Instance_T *p_ltb_instance,
                  Ltb_Output_T *p_ltb_output ,
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
                  const Ltb_Input_T *p_ltb_input )
// clang-format on
{
   /*Get the Current State Machine Output*/
   const Ltb_State_T *p_ltb_current_state = Ltb_Get_Current_State();
   assert(NULL != p_ltb_input);
   assert(NULL != p_ltb_output);

   /* Set LTB output to default */
   Ltb_Reset_Output(p_ltb_output);
   if (LTB_STATE_ACTIVE == *p_ltb_current_state)
   {
      /* Set new internal (Algo-Post-Run) BMW output */
      Ltb_Set_Output(p_ltb_output, &p_ltb_instance->core_output);
   }
   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Ltb_Output(p_ltb_output);
#endif
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/


static void Ltb_Reset_Output(Ltb_Output_T *p_ltb_output)
{
   assert(NULL != p_ltb_output);

   p_ltb_output->LTB_bmw_sp25_alert_left  = NO_ALERT;
   p_ltb_output->LTB_bmw_sp25_alert_right = NO_ALERT;

   p_ltb_output->LTB_bmw_sp25_ttc_left  = LTB_INVALID_TTC;
   p_ltb_output->LTB_bmw_sp25_ttc_right = LTB_INVALID_TTC;
   p_ltb_output->LTB_bmw_sp25_ttb_left  = LTB_INVALID_TTB;
   p_ltb_output->LTB_bmw_sp25_ttb_right = LTB_INVALID_TTB;
}

static void Ltb_Set_Output(Ltb_Output_T *p_ltb_output, const Ltb_Core_Output_T *p_ltb_core_output)
{
   p_ltb_output->LTB_bmw_sp25_alert_left  = p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT];
   p_ltb_output->LTB_bmw_sp25_alert_right = p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT];
   p_ltb_output->LTB_bmw_sp25_ttc_left    = p_ltb_core_output->ltb_ttc[FBK_SIDE_LEFT];
   p_ltb_output->LTB_bmw_sp25_ttc_right   = p_ltb_core_output->ltb_ttc[FBK_SIDE_RIGHT];
   p_ltb_output->LTB_bmw_sp25_ttb_left    = p_ltb_core_output->ltb_ttb[FBK_SIDE_LEFT];
   p_ltb_output->LTB_bmw_sp25_ttb_right   = p_ltb_core_output->ltb_ttb[FBK_SIDE_RIGHT];
}

#ifdef BINARY_DEBUG
static void Write_Ltb_Output(const Ltb_Output_T *p_ltb_output)
{
   /* check input parameters */
   assert(NULL != p_ltb_output);

   /* Log Safe Exit specific data*/
   LTB_STORE_VAL_MGR_WPR("LTB_bmw_sp25_alert_left", p_ltb_output->LTB_bmw_sp25_alert_left);
   LTB_STORE_VAL_MGR_WPR("LTB_bmw_sp25_alert_right", p_ltb_output->LTB_bmw_sp25_alert_right);
   LTB_STORE_VAL_MGR_WPR("LTB_bmw_sp25_ttc_left", p_ltb_output->LTB_bmw_sp25_ttc_left);
   LTB_STORE_VAL_MGR_WPR("LTB_bmw_sp25_ttc_right", p_ltb_output->LTB_bmw_sp25_ttc_right);
   LTB_STORE_VAL_MGR_WPR("LTB_bmw_sp25_ttb_left", p_ltb_output->LTB_bmw_sp25_ttb_left);
   LTB_STORE_VAL_MGR_WPR("LTB_bmw_sp25_ttb_right", p_ltb_output->LTB_bmw_sp25_ttb_right);
}
#endif /* BINARY_DEBUG */
