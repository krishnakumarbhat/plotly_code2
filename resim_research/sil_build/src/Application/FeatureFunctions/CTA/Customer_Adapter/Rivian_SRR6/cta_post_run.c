/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Rivian_SRR6 post run logic for CTA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "cta_post_run.h"
#include "cta_core_output_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/


/**
 * @brief Reset Rivian CTA output to the default values
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
static void Cta_Reset_Output(Cta_Output_T *p_cta_output);


/**
 * @brief Maps the CTA status from core output to the Rivian specific CTA status
 *
 * @return Rivian CTA status
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
static Cta_Rivian_Status_T Cta_Map_Cta_Status_To_Rivian(Cta_Status_T cta_status);

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Rivian_Output(const Cta_Output_T *p_cta_output);
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run_Init(Cta_Instance_T *p_cta_instance)
{
   assert(NULL != p_cta_instance);
}

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type but does not modify the object it points to. Consider adding const qualifier to the points-to type.] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
// clang-format on
{
   /* Asserts */
   assert(NULL != p_cta_output);
   assert(NULL != p_cta_input);
   assert(NULL != p_cta_instance);

   Cta_Reset_Output(p_cta_output);

   /* Front CTA signals */
   if (p_cta_input->f_front_cta_enable)
   {
      p_cta_output->front_cta_obj_ttc_left     = p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->front_cta_obj_ttc_right    = p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      p_cta_output->front_cta_alert_level_left = (uint8_t) p_cta_instance->core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->front_cta_alert_level_right =
         (uint8_t) p_cta_instance->core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      p_cta_output->front_cta_id_left                = p_cta_instance->core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->front_cta_id_right               = p_cta_instance->core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      p_cta_output->f_front_cta_brake_qualifier_left = p_cta_instance->core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->f_front_cta_brake_qualifier_right = p_cta_instance->core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      p_cta_output->front_cta_long_intersection_left =
         p_cta_instance->core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->front_cta_long_intersection_right =
         p_cta_instance->core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      p_cta_output->front_cta_heading_left        = p_cta_instance->core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->front_cta_heading_right       = p_cta_instance->core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      p_cta_output->front_cta_warn_hold_cnt_left  = p_cta_instance->core_output.cta_warn_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      p_cta_output->front_cta_warn_hold_cnt_right = p_cta_instance->core_output.cta_warn_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
   }

   /* Rear CTA signals */
   if (p_cta_input->f_rear_cta_enable)
   {
      p_cta_output->rear_cta_obj_ttc_left     = p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->rear_cta_obj_ttc_right    = p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->rear_cta_alert_level_left = (uint8_t) p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->rear_cta_alert_level_right = (uint8_t) p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->rear_cta_id_left           = p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->rear_cta_id_right          = p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->f_rear_cta_brake_qualifier_left  = p_cta_instance->core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->f_rear_cta_brake_qualifier_right = p_cta_instance->core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->rear_cta_long_intersection_left = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->rear_cta_long_intersection_right =
         p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->rear_cta_heading_left        = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->rear_cta_heading_right       = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->rear_cta_warn_hold_cnt_left  = p_cta_instance->core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->rear_cta_warn_hold_cnt_right = p_cta_instance->core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   }

   /* General signals */
   p_cta_output->cta_status = Cta_Map_Cta_Status_To_Rivian(p_cta_instance->core_output.cta_status);

#ifdef BINARY_DEBUG
   Write_Debug_Cta_Rivian_Output(p_cta_output);
#endif /* BINARY_DEBUG */
}

/*============================================================================*\
 * Local Function Definition
\*============================================================================*/

static Cta_Rivian_Status_T Cta_Map_Cta_Status_To_Rivian(Cta_Status_T cta_status)
{
   Cta_Rivian_Status_T cta_rivian_status;

   switch (cta_status)
   {
      case CTA_STATUS_ACTIVE:
         cta_rivian_status = RIVIAN_CTA_ACTIVE;
         break;
      case CTA_STATUS_DEACTIVATED_EGO_SPEED:
         cta_rivian_status = RIVIAN_CTA_DEACTIVATED_EGO_SPEED;
         break;
      case CTA_STATUS_DISABLED:
      default:
         cta_rivian_status = RIVIAN_CTA_DISABLED;
         break;
   }
   return cta_rivian_status;
}

static void Cta_Reset_Output(Cta_Output_T *p_cta_output)
{
   /* Assert */
   assert(NULL != p_cta_output);

   /* Front CTA signals */
   p_cta_output->front_cta_obj_ttc_left            = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->front_cta_obj_ttc_right           = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->front_cta_alert_level_left        = FBK_ZERO_UINT;
   p_cta_output->front_cta_alert_level_right       = FBK_ZERO_UINT;
   p_cta_output->front_cta_id_left                 = FBK_ZERO_UINT;
   p_cta_output->front_cta_id_right                = FBK_ZERO_UINT;
   p_cta_output->f_front_cta_brake_qualifier_left  = FBK_FALSE;
   p_cta_output->f_front_cta_brake_qualifier_right = FBK_FALSE;
   p_cta_output->front_cta_long_intersection_left  = FBK_ZERO_F;
   p_cta_output->front_cta_long_intersection_right = FBK_ZERO_F;
   p_cta_output->front_cta_heading_left            = FBK_ZERO_F;
   p_cta_output->front_cta_heading_right           = FBK_ZERO_F;
   p_cta_output->front_cta_warn_hold_cnt_left      = FBK_ZERO_UINT;
   p_cta_output->front_cta_warn_hold_cnt_right     = FBK_ZERO_UINT;

   /* Rear CTA signals */
   p_cta_output->rear_cta_obj_ttc_left            = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->rear_cta_obj_ttc_right           = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->rear_cta_alert_level_left        = FBK_ZERO_UINT;
   p_cta_output->rear_cta_alert_level_right       = FBK_ZERO_UINT;
   p_cta_output->rear_cta_id_left                 = FBK_ZERO_UINT;
   p_cta_output->rear_cta_id_right                = FBK_ZERO_UINT;
   p_cta_output->f_rear_cta_brake_qualifier_left  = FBK_FALSE;
   p_cta_output->f_rear_cta_brake_qualifier_right = FBK_FALSE;
   p_cta_output->rear_cta_long_intersection_left  = FBK_ZERO_F;
   p_cta_output->rear_cta_long_intersection_right = FBK_ZERO_F;
   p_cta_output->rear_cta_heading_left            = FBK_ZERO_F;
   p_cta_output->rear_cta_heading_right           = FBK_ZERO_F;
   p_cta_output->rear_cta_warn_hold_cnt_left      = FBK_ZERO_UINT;
   p_cta_output->rear_cta_warn_hold_cnt_right     = FBK_ZERO_UINT;

   /* General signals */
   p_cta_output->cta_status = RIVIAN_CTA_DISABLED;
}


#ifdef BINARY_DEBUG

static void Write_Debug_Cta_Rivian_Output(const Cta_Output_T *p_cta_output)
{
   /* Check input parameters. */
   assert(NULL != p_cta_output);

   /* Log CTA core ouput. */
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_alert_level_right", p_cta_output->front_cta_alert_level_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_alert_level_left", p_cta_output->front_cta_alert_level_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_alert_level_left", p_cta_output->rear_cta_alert_level_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_alert_level_right", p_cta_output->rear_cta_alert_level_right);

   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_obj_ttc_left", p_cta_output->front_cta_obj_ttc_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_obj_ttc_right", p_cta_output->front_cta_obj_ttc_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_alert_level_left", p_cta_output->front_cta_alert_level_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_alert_level_right", p_cta_output->front_cta_alert_level_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_id_left", p_cta_output->front_cta_id_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_id_right", p_cta_output->front_cta_id_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_f_fcta_cta_brake_qualifier_left", p_cta_output->f_front_cta_brake_qualifier_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_f_fcta_cta_brake_qualifier_right", p_cta_output->f_front_cta_brake_qualifier_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_long_intersection_left", p_cta_output->front_cta_long_intersection_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_long_intersection_right", p_cta_output->front_cta_long_intersection_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_heading_left", p_cta_output->front_cta_heading_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_heading_right", p_cta_output->front_cta_heading_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_warn_hold_cnt_left", p_cta_output->front_cta_warn_hold_cnt_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_fcta_cta_warn_hold_cnt_right", p_cta_output->front_cta_warn_hold_cnt_right);

   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_obj_ttc_left", p_cta_output->rear_cta_obj_ttc_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_obj_ttc_right", p_cta_output->rear_cta_obj_ttc_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_alert_level_left", p_cta_output->rear_cta_alert_level_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_alert_level_right", p_cta_output->rear_cta_alert_level_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_id_left", p_cta_output->rear_cta_id_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_id_right", p_cta_output->rear_cta_id_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_f_rcta_cta_brake_qualifier_left", p_cta_output->f_rear_cta_brake_qualifier_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_f_rcta_cta_brake_qualifier_right", p_cta_output->f_rear_cta_brake_qualifier_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_long_intersection_left", p_cta_output->rear_cta_long_intersection_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_long_intersection_right", p_cta_output->rear_cta_long_intersection_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_heading_left", p_cta_output->rear_cta_heading_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_heading_right", p_cta_output->rear_cta_heading_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_warn_hold_cnt_left", p_cta_output->rear_cta_warn_hold_cnt_left);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_rcta_cta_warn_hold_cnt_right", p_cta_output->rear_cta_warn_hold_cnt_right);
   CTA_STORE_VAL_MGR_WPR("cta_rivian_out_cta_status", p_cta_output->cta_status);
}
#endif /* BINARY_DEBUG */
