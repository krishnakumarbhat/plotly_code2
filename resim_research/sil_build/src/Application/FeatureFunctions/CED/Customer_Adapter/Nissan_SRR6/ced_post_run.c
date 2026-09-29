/**
 * @file ced_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Nissan SRR6 post run logic for CED.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run.h"
#include "ced_core_output_t.h"
#include "ced_iface.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "ced_debug_writer.h"
#endif /* BINARY_DEBUG */

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static const Ced_Output_T *P_LAST_CED_OUTPUT;

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output);
#endif /* BINARY_DEBUG */

/**
 * @brief Update output from core output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3469}
 * @verification{Create test which will verify that outputs will be set correctly according do implementation}
 */
static void Ced_Update_Output(Ced_Output_T *p_ced_output /**< CED output data */,
                              const Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                              const Ced_Input_T *p_ced_input /**< CED input data */);

/**
 * @brief Reset CED output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3468}
 * @verification{}
 */
static void Ced_Reset_Output(Ced_Output_T *p_ced_output /**< CED output data */);

/**
 * @brief Map CED alert level to Nissan specific alert level enum.
 *
 * @return OSE alert level
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3470}
 * @verification{}
 */
static Ose_Alert_T Ced_Map_Alert_Level_To_Ose(const Ced_Alert_T ced_alert_level /**< CED alert level */);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

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
   assert(NULL != p_ced_input);
   assert(NULL != p_ced_output);
   assert(NULL != p_ced_instance);

   /* Set SFE ouput */
   Ced_Reset_Output(p_ced_output);
   Ced_Update_Output(p_ced_output, &p_ced_instance->core_output, p_ced_input);

   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Ced_Output(p_ced_output);
#endif
   P_LAST_CED_OUTPUT = Ced_Get_Output_Ptr();
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Ced_Update_Output(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output, const Ced_Input_T *p_ced_input)
{
   /* BMW SRR5 Safe Exit Outputs */
   p_ced_output->f_ced_enable = Fbk_Convert_Bool_To_Uint(p_ced_input->f_ced_enable);

   /* Warning for right side */
   p_ced_output->CED_ttc_right = Fbk_Max(FBK_ZERO_F, p_ced_core_output->ced_ttc[FBK_SIDE_RIGHT]);
   p_ced_output->CED_id_right  = p_ced_core_output->ced_id[FBK_SIDE_RIGHT];

   /* Only set alert level right for a TTC above the minimum threshold */
   if (p_ced_output->CED_ttc_right >= FBK_ZERO_F)
   {
      p_ced_output->CED_alert_right = Ced_Map_Alert_Level_To_Ose(p_ced_core_output->ced_alert[FBK_SIDE_RIGHT]);
   }

   /* Only set predicted lateral position if there is an active alert on the right */
   if (OSE_NO_ALERT != p_ced_output->CED_alert_right)
   {
      p_ced_output->CED_object_predicted_lat_pos_right = p_ced_core_output->ced_object_predicted_lat_pos[FBK_SIDE_RIGHT];
   }

   /* Warning for left side */
   p_ced_output->CED_ttc_left = Fbk_Max(FBK_ZERO_F, p_ced_core_output->ced_ttc[FBK_SIDE_LEFT]);
   p_ced_output->CED_id_left  = p_ced_core_output->ced_id[FBK_SIDE_LEFT];

   /* Only set alert level left for a TTC above the minimum threshold */
   if (p_ced_output->CED_ttc_left >= FBK_ZERO_F)
   {
      p_ced_output->CED_alert_left = Ced_Map_Alert_Level_To_Ose(p_ced_core_output->ced_alert[FBK_SIDE_LEFT]);
   }

   /* Only set predicted lateral position if there is an active alert on the left */
   if (OSE_NO_ALERT != p_ced_output->CED_alert_left)
   {
      p_ced_output->CED_object_predicted_lat_pos_left = p_ced_core_output->ced_object_predicted_lat_pos[FBK_SIDE_LEFT];
   }
}

static void Ced_Reset_Output(Ced_Output_T *p_ced_output)
{
   assert(NULL != p_ced_output);

   /* OSE/CED Output for State Machine */
   p_ced_output->CED_alert_right = OSE_NO_ALERT;
   p_ced_output->CED_alert_left  = OSE_NO_ALERT;

   /* Additional Signals via UDP for Debugging and ORCAS*/

   p_ced_output->f_ced_enable = FBK_ZERO_UINT;

   /* Right side */
   p_ced_output->CED_ttc_right                      = CED_INVALID_TIME;
   p_ced_output->CED_id_right                       = FBK_ZERO_UINT;
   p_ced_output->CED_object_predicted_lat_pos_right = CED_INVALID_DISTANCE;

   /* Left side */
   p_ced_output->CED_ttc_left                      = CED_INVALID_TIME;
   p_ced_output->CED_id_left                       = FBK_ZERO_UINT;
   p_ced_output->CED_object_predicted_lat_pos_left = CED_INVALID_DISTANCE;
}

static Ose_Alert_T Ced_Map_Alert_Level_To_Ose(const Ced_Alert_T ced_alert_level)
{
   Ose_Alert_T nissan_alert_level;

   switch (ced_alert_level)
   {
      case CED_NO_ALERT:
         nissan_alert_level = OSE_NO_ALERT;
         break;

      case CED_ALERT_ACTIVE_LEVEL_1:
         nissan_alert_level = OSE_ALERT_LEVEL_1;
         break;

      case CED_ALERT_ACTIVE_LEVEL_2:
         nissan_alert_level = OSE_ALERT_LEVEL_2;
         break;

      default:
         nissan_alert_level = OSE_NO_ALERT;
         break;
   }

   return nissan_alert_level;
}
#ifdef BINARY_DEBUG
static void Write_Ced_Output(const Ced_Output_T *p_ced_output)
{
   /* check input parameters */
   assert(NULL != p_ced_output);

   /* Log Safe Exit specific data*/
   CED_STORE_VAL_MGR_WPR("OSE_CED_alert_left", p_ced_output->CED_alert_left);
   CED_STORE_VAL_MGR_WPR("OSE_CED_alert_right", p_ced_output->CED_alert_right);
   CED_STORE_VAL_MGR_WPR("OSE_CED_id_left", p_ced_output->CED_id_left);
   CED_STORE_VAL_MGR_WPR("OSE_CED_id_right", p_ced_output->CED_id_right);
   CED_STORE_VAL_MGR_WPR("OSE_CED_ttc_left", p_ced_output->CED_ttc_left);
   CED_STORE_VAL_MGR_WPR("OSE_CED_ttc_right", p_ced_output->CED_ttc_right);
   CED_STORE_VAL_MGR_WPR("OSE_CED_obj_pred_lat_pos_left", p_ced_output->CED_object_predicted_lat_pos_left);
   CED_STORE_VAL_MGR_WPR("OSE_CED_obj_pred_lat_pos_right", p_ced_output->CED_object_predicted_lat_pos_right);
}
#endif /* BINARY_DEBUG */
