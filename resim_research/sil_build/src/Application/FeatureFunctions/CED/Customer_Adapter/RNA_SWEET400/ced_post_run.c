/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the RNA_SWEET400 post run logic for CED.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run.h"
#include "ced_core_output_t.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "ml_math_infinity_silent.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ced_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/*===========================================================================*\
 * Local Function Declaration
 \*===========================================================================*/

/**
 * @brief Resets RNA_SWEET400 specific postrun to its defaults
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3486}
 * @verification{}
 */
static void Ced_Reset_Output(Ced_Output_T *p_ced_output);

/*===========================================================================*\
 * Global Functions	Definition
 \*===========================================================================*/

/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type.] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Ced_Post_Run_Init(Ced_Instance_T *p_ced_instance)
{
   assert(NULL != p_ced_instance);
}

// clang-format off
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_ced_instance" points to a non-constant type] */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed]*/
void Ced_Post_Run(Ced_Instance_T *p_ced_instance, const Ced_Input_T *p_ced_input,  Ced_Output_T *p_ced_output)
// clang-format on
{
   assert(NULL != p_ced_instance);
   assert(NULL != p_ced_output);
   assert(NULL != p_ced_input);

   /* Reset Ced output */
   Ced_Reset_Output(p_ced_output);

   if (FBK_SIDE_REAR == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_LEFT])
   {
      p_ced_output->CED_alert_left       = (uint8_t) p_ced_instance->core_output.ced_alert[FBK_SIDE_LEFT];
      p_ced_output->CED_id_left          = p_ced_instance->core_output.ced_id[FBK_SIDE_LEFT];
      p_ced_output->CED_ttc_left         = Fbk_Max(FBK_ZERO_F, p_ced_instance->core_output.ced_ttc[FBK_SIDE_LEFT]);
      p_ced_output->CED_front_alert_left = (uint8_t) CED_NO_ALERT;
      p_ced_output->CED_front_id_left    = FBK_ZERO_UINT;
      p_ced_output->CED_front_ttc_left   = AS_TOOLBOX_INFINITY;
   }
   else if (FBK_SIDE_FRONT == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_LEFT])
   {
      p_ced_output->CED_alert_left       = (uint8_t) CED_NO_ALERT;
      p_ced_output->CED_id_left          = FBK_ZERO_UINT;
      p_ced_output->CED_ttc_left         = AS_TOOLBOX_INFINITY;
      p_ced_output->CED_front_alert_left = (uint8_t) p_ced_instance->core_output.ced_alert[FBK_SIDE_LEFT];
      p_ced_output->CED_front_id_left    = p_ced_instance->core_output.ced_id[FBK_SIDE_LEFT];
      p_ced_output->CED_front_ttc_left   = Fbk_Max(FBK_ZERO_F, p_ced_instance->core_output.ced_ttc[FBK_SIDE_LEFT]);
   }
   else
   {
      /* Do nothing */
   }

   if (FBK_SIDE_REAR == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_RIGHT])
   {
      p_ced_output->CED_alert_right       = (uint8_t) p_ced_instance->core_output.ced_alert[FBK_SIDE_RIGHT];
      p_ced_output->CED_id_right          = p_ced_instance->core_output.ced_id[FBK_SIDE_RIGHT];
      p_ced_output->CED_ttc_right         = Fbk_Max(FBK_ZERO_F, p_ced_instance->core_output.ced_ttc[FBK_SIDE_RIGHT]);
      p_ced_output->CED_front_alert_right = (uint8_t) CED_NO_ALERT;
      p_ced_output->CED_front_id_right    = FBK_ZERO_UINT;
      p_ced_output->CED_front_ttc_right   = AS_TOOLBOX_INFINITY;
   }
   else if (FBK_SIDE_FRONT == p_ced_instance->core_output.ced_object_direction[FBK_SIDE_RIGHT])
   {
      p_ced_output->CED_alert_right       = (uint8_t) CED_NO_ALERT;
      p_ced_output->CED_id_right          = FBK_ZERO_UINT;
      p_ced_output->CED_ttc_right         = AS_TOOLBOX_INFINITY;
      p_ced_output->CED_front_alert_right = (uint8_t) p_ced_instance->core_output.ced_alert[FBK_SIDE_RIGHT];
      p_ced_output->CED_front_id_right    = p_ced_instance->core_output.ced_id[FBK_SIDE_RIGHT];
      p_ced_output->CED_front_ttc_right   = Fbk_Max(FBK_ZERO_F, p_ced_instance->core_output.ced_ttc[FBK_SIDE_RIGHT]);
   }
   else
   {
      /* Do nothing */
   }

#ifdef BINARY_DEBUG
   Write_Ced_Output(p_ced_output);
#endif
}

/*===========================================================================*\
 * Local Function Definition
 \*===========================================================================*/

static void Ced_Reset_Output(Ced_Output_T *p_ced_output)
{
   assert(NULL != p_ced_output);


   p_ced_output->CED_alert_left        = (uint8_t) CED_NO_ALERT;
   p_ced_output->CED_id_left           = FBK_ZERO_UINT;
   p_ced_output->CED_ttc_left          = AS_TOOLBOX_INFINITY;
   p_ced_output->CED_front_alert_left  = (uint8_t) CED_NO_ALERT;
   p_ced_output->CED_front_id_left     = FBK_ZERO_UINT;
   p_ced_output->CED_front_ttc_left    = AS_TOOLBOX_INFINITY;
   p_ced_output->CED_alert_right       = (uint8_t) CED_NO_ALERT;
   p_ced_output->CED_id_right          = FBK_ZERO_UINT;
   p_ced_output->CED_ttc_right         = AS_TOOLBOX_INFINITY;
   p_ced_output->CED_front_alert_right = (uint8_t) CED_NO_ALERT;
   p_ced_output->CED_front_id_right    = FBK_ZERO_UINT;
   p_ced_output->CED_front_ttc_right   = AS_TOOLBOX_INFINITY;
}

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output)
{
   /* check input parameters */
   assert(NULL != p_ced_output);

   /* Log the CED customer output. */
   CED_STORE_VAL_MGR_WPR("CED_alert_left", p_ced_output->CED_alert_left);
   CED_STORE_VAL_MGR_WPR("CED_id_left", p_ced_output->CED_id_left);
   CED_STORE_VAL_MGR_WPR("CED_ttc_left", p_ced_output->CED_ttc_left);

   CED_STORE_VAL_MGR_WPR("CED_alert_right", p_ced_output->CED_alert_right);
   CED_STORE_VAL_MGR_WPR("CED_id_right", p_ced_output->CED_id_right);
   CED_STORE_VAL_MGR_WPR("CED_ttc_right", p_ced_output->CED_ttc_right);

   CED_STORE_VAL_MGR_WPR("CED_front_alert_left", p_ced_output->CED_front_alert_left);
   CED_STORE_VAL_MGR_WPR("CED_front_id_left", p_ced_output->CED_front_id_left);
   CED_STORE_VAL_MGR_WPR("CED_front_ttc_left", p_ced_output->CED_front_ttc_left);

   CED_STORE_VAL_MGR_WPR("CED_front_alert_right", p_ced_output->CED_front_alert_right);
   CED_STORE_VAL_MGR_WPR("CED_front_id_right", p_ced_output->CED_front_id_right);
   CED_STORE_VAL_MGR_WPR("CED_front_ttc_right", p_ced_output->CED_front_ttc_right);
}
#endif /* BINARY_DEBUG */
