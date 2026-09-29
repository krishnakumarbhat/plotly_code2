/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the generic post run logic for CTA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */
#include "cta_post_run.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

#ifdef BINARY_DEBUG
static void Write_Cta_Output(const Cta_Output_T *p_cta_output);
#endif /* BINARY_DEBUG */

static void Cta_Reset_Critical_Object(Cta_Critical_Object_T *p_critical_object);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run_Init(Cta_Instance_T *p_cta_instance)
{
   /* Assert */
   assert(NULL != p_cta_instance);
}

/* clang-format off */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type but does not modify the object it points to. Consider adding const qualifier to the points-to type.] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
/* clang-format on */
{
   uint8_t mode_idx;
   uint8_t side_idx;
   const Cta_Core_Output_T *p_cta_core_output;
   /* Asserts */
   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_cta_instance->core_input.p_pa_data);
   assert(NULL != p_cta_output);

   p_cta_core_output = &p_cta_instance->core_output;

   p_cta_output->f_cta_enabled = (CTA_STATUS_ACTIVE == p_cta_core_output->cta_status) ? FBK_TRUE : FBK_FALSE;

   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < (uint8_t) FBK_NUMBER_OF_SIDES; side_idx++)
      {
         Cta_Critical_Object_T *p_critical_object = &p_cta_output->most_critical_object_by_sides[mode_idx][side_idx];

         uint8_t index = p_cta_core_output->cta_index[mode_idx][side_idx];
         if (PA_INVALID_OBJ_INDEX != index)
         {
            const Fbk_Object_Data_T *p_tracker_object = &(p_cta_instance->core_input.p_pa_data->object_data[index]);
            p_critical_object->id                     = p_tracker_object->id;
            p_critical_object->unique_id              = p_tracker_object->unique_id;
            p_critical_object->objPoseX_m             = p_tracker_object->vcs_pos.x;
            p_critical_object->objPoseY_m             = p_tracker_object->vcs_pos.y;
            p_critical_object->objVelocityX_mps       = p_tracker_object->vcs_vel.x;
            p_critical_object->objVelocityY_mps       = p_tracker_object->vcs_vel.y;
         }
         else
         {
            Cta_Reset_Critical_Object(p_critical_object);
         }
         p_critical_object->heading_rad            = p_cta_core_output->cta_heading[mode_idx][side_idx];
         p_critical_object->alert_level            = p_cta_core_output->cta_alert_level[mode_idx][side_idx];
         p_critical_object->ttc_s                  = p_cta_core_output->cta_obj_ttc[mode_idx][side_idx];
         p_critical_object->intersection_point_x_m = p_cta_core_output->cta_long_intersection[mode_idx][side_idx];
         p_critical_object->f_brake_qualifier      = p_cta_core_output->f_brake_qualifier[mode_idx][side_idx];
         p_critical_object->brake_deceleration     = p_cta_core_output->brake_deceleration[mode_idx][side_idx];
         p_critical_object->f_standstill_qualifier = p_cta_core_output->f_standstill_qualifier[mode_idx][side_idx];
      }
   }
   /* Write bin file output */
#ifdef BINARY_DEBUG
   Write_Cta_Output(p_cta_output);
#endif
}


static void Cta_Reset_Critical_Object(Cta_Critical_Object_T *p_critical_object)
{
   p_critical_object->alert_level            = CTA_CRIT_LEVEL_NONE;
   p_critical_object->intersection_point_x_m = FBK_ZERO_F;
   p_critical_object->ttc_s                  = CTA_HIGH_DEFAULT_VAL;
   p_critical_object->f_brake_qualifier      = FBK_FALSE;

   p_critical_object->id                     = 0;
   p_critical_object->heading_rad            = FBK_ZERO_F;
   p_critical_object->objPoseX_m             = FBK_ZERO_F;
   p_critical_object->objPoseY_m             = FBK_ZERO_F;
   p_critical_object->objVelocityX_mps       = FBK_ZERO_F;
   p_critical_object->objVelocityY_mps       = FBK_ZERO_F;
   p_critical_object->brake_deceleration     = FBK_ZERO_F;
   p_critical_object->f_standstill_qualifier = FBK_FALSE;
}


#ifdef BINARY_DEBUG
static void Write_Cta_Output(const Cta_Output_T *p_cta_output)
{
   /* check input parameters */
   assert(NULL != p_cta_output);

   /* Log Safe Exit specific data*/
   CTA_STORE_VAL_MGR_WPR("cta_out_f_cta_enabled", p_cta_output->f_cta_enabled);
   {
      const Cta_Critical_Object_T *p_critical_object = &p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT];
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "id",
                            p_critical_object->id);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "alert_level",
                            p_critical_object->alert_level);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "f_brake_qualifier",
                            p_critical_object->f_brake_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "ttc_s",
                            p_critical_object->ttc_s);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "intersection_point_x_m",
                            p_critical_object->intersection_point_x_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "heading_rad",
                            p_critical_object->heading_rad);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "objPoseX_m",
                            p_critical_object->objPoseX_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "objPoseY_m",
                            p_critical_object->objPoseY_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "objVelocityX_mps",
                            p_critical_object->objVelocityX_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_"
                            "objVelocityY_mps",
                            p_critical_object->objVelocityY_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_standstill_qualifier", p_critical_object->f_standstill_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_left_brake_deceleration", p_critical_object->brake_deceleration);
   }
   {
      const Cta_Critical_Object_T *p_critical_object = &p_cta_output->most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "id",
                            p_critical_object->id);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "alert_level",
                            p_critical_object->alert_level);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "f_brake_qualifier",
                            p_critical_object->f_brake_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "ttc_s",
                            p_critical_object->ttc_s);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "intersection_point_x_m",
                            p_critical_object->intersection_point_x_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "heading_rad",
                            p_critical_object->heading_rad);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "objPoseX_m",
                            p_critical_object->objPoseX_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "objPoseY_m",
                            p_critical_object->objPoseY_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "objVelocityX_mps",
                            p_critical_object->objVelocityX_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_"
                            "objVelocityY_mps",
                            p_critical_object->objVelocityY_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_standstill_qualifier", p_critical_object->f_standstill_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_rear_right_brake_deceleration", p_critical_object->brake_deceleration);
   }
   {
      const Cta_Critical_Object_T *p_critical_object = &p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_LEFT];
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "id",
                            p_critical_object->id);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "alert_level",
                            p_critical_object->alert_level);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "f_brake_qualifier",
                            p_critical_object->f_brake_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "ttc_s",
                            p_critical_object->ttc_s);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "intersection_point_x_m",
                            p_critical_object->intersection_point_x_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "heading_rad",
                            p_critical_object->heading_rad);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "objPoseX_m",
                            p_critical_object->objPoseX_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "objPoseY_m",
                            p_critical_object->objPoseY_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "objVelocityX_mps",
                            p_critical_object->objVelocityX_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_"
                            "objVelocityY_mps",
                            p_critical_object->objVelocityY_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_standstill_qualifier", p_critical_object->f_standstill_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_left_brake_deceleration", p_critical_object->brake_deceleration);
   }
   {
      const Cta_Critical_Object_T *p_critical_object = &p_cta_output->most_critical_object_by_sides[CTA_MODE_FRONT][FBK_SIDE_RIGHT];
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "id",
                            p_critical_object->id);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "alert_level",
                            p_critical_object->alert_level);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "f_brake_qualifier",
                            p_critical_object->f_brake_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "ttc_s",
                            p_critical_object->ttc_s);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "intersection_point_x_m",
                            p_critical_object->intersection_point_x_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "heading_rad",
                            p_critical_object->heading_rad);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "objPoseX_m",
                            p_critical_object->objPoseX_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "objPoseY_m",
                            p_critical_object->objPoseY_m);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "objVelocityX_mps",
                            p_critical_object->objVelocityX_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_"
                            "objVelocityY_mps",
                            p_critical_object->objVelocityY_mps);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_standstill_qualifier", p_critical_object->f_standstill_qualifier);
      CTA_STORE_VAL_MGR_WPR("cta_out_front_right_brake_deceleration", p_critical_object->brake_deceleration);
   }
}
#endif /* BINARY_DEBUG */
