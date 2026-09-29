/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Honda_SRR6 customer specific post run.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "cta_post_run.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_customer_calibration_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*============================================================================*\
* HONDA SPECIFIC TYPE DEFINITIONS
\*============================================================================*/

/**
 * @brief Contains the HONDA quasi-persistent data for ttc and id
 */
typedef struct
{
   uint8_t cta_prev_id_left;     /**< ID of the object responsible for CTA alert in last cycle, on left */
   uint8_t cta_prev_id_right;    /**< ID of the object responsible for CTA alert in last cycle, on right*/
   float32_T cta_prev_ttc_left;  /**< TTC time in the previous cycle, left side */
   float32_T cta_prev_ttc_right; /**< TTC time in the previous cycle, right side */
} Cta_Honda_Persistent_T;

/*===========================================================================*\
* File Scope variables
\*===========================================================================*/

static Cta_Honda_Persistent_T Cta_Honda_Persistent;

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief This function resets the honda specific customer output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2458}
 * @SDD{SF-4014}
 * @verification{}
 */
static void Cta_Reset_Output(Cta_Output_T *p_cta_output);

/**
 * @brief This function sets the honda specific customer output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2458}
 * @SDD{SF-4015}
 * @verification{}
 */
static void Cta_Set_Output(Cta_Output_T *p_cta_output /**< CTA Output Pointer*/,
                           Cta_Instance_T *p_cta_instance /**< CTA Instance Pointer*/);


/**
 * @brief This function sets the honda specific customer output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2458}
 * @SDD{}
 * @verification{}
 */
static void Cta_Map_Alert(Cta_Output_T *p_cta_output /**< CTA Output Pointer*/,
                          Cta_Instance_T *p_cta_instance /**< CTA Instance Pointer*/);


#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Honda_SRR6_Output(const Cta_Output_T *p_cta_output);
#define Binary_Write_Cta_Customer_Output(p_cta_output) Write_Debug_Cta_Honda_SRR6_Output(p_cta_output)
#else
#define Binary_Write_Cta_Customer_Output(p_cta_output)
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run_Init(Cta_Instance_T *p_cta_instance)
{
   assert(NULL != p_cta_instance);

   /* Initialize quasi-persistent data for ttc and id while alert is holding, it should not be reset every cycle. */
   Cta_Honda_Persistent.cta_prev_id_left   = FBK_ZERO_UINT;
   Cta_Honda_Persistent.cta_prev_id_right  = FBK_ZERO_UINT;
   Cta_Honda_Persistent.cta_prev_ttc_left  = CTA_HIGH_DEFAULT_VAL;
   Cta_Honda_Persistent.cta_prev_ttc_right = CTA_HIGH_DEFAULT_VAL;
}

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
// clang-format on
{
   /* Asserts */
   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_cta_output);

   /* Reset output */
   Cta_Reset_Output(p_cta_output);

   /* Set new output */
   Cta_Set_Output(p_cta_output, p_cta_instance);

   /* Write debug output */
   Binary_Write_Cta_Customer_Output(p_cta_output);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Cta_Reset_Output(Cta_Output_T *p_cta_output)
{
   /* Asserts */
   assert(NULL != p_cta_output);

   p_cta_output->f_cta_enabled                  = FBK_ZERO_UINT;
   p_cta_output->f_cta_warn_left                = FBK_ZERO_UINT;
   p_cta_output->f_cta_alert_left               = FBK_ZERO_UINT;
   p_cta_output->cta_id_left                    = FBK_ZERO_UINT;
   p_cta_output->cta_ttc_left                   = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objPoseX_left              = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objPoseY_left              = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objVelocityX_left          = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objVelocityY_left          = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_heading_left               = FBK_ZERO_F;
   p_cta_output->cta_intersection_point_x_left  = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->f_cta_warn_right               = FBK_ZERO_UINT;
   p_cta_output->f_cta_alert_right              = FBK_ZERO_UINT;
   p_cta_output->cta_id_right                   = FBK_ZERO_UINT;
   p_cta_output->cta_ttc_right                  = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objPoseX_right             = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objPoseY_right             = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objVelocityX_right         = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objVelocityY_right         = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_heading_right              = FBK_ZERO_F;
   p_cta_output->cta_intersection_point_x_right = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->f_brake_qualifier              = FBK_ZERO_UINT;
}


/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
static void Cta_Map_Alert(Cta_Output_T *p_cta_output, Cta_Instance_T *p_cta_instance)
{
   Fbk_Object_Corners_T obj_corners;
   uint8_t id_crit_obj;
   float32_T dist_to_crash_line;

   /* Asserts */
   assert(NULL != p_cta_output);
   assert(NULL != p_cta_instance);

   /* acoustic warning may be triggered by higher criticality level */
   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] > CTA_CRIT_LEVEL_1)
   {
      p_cta_output->f_cta_alert_left = FBK_ONE_UINT;
      id_crit_obj                    = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT];
      Fbk_Calculate_Target_Corners(&obj_corners, &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].vcs_pos),
                                   &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].vcs_heading),
                                   &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].length),
                                   &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].width));

      dist_to_crash_line =
         Fbk_Min(Fbk_Abs_F(obj_corners.points[FBK_FRONT_LEFT_CORNER].y), Fbk_Abs_F(obj_corners.points[FBK_FRONT_RIGHT_CORNER].y))
         - Fbk_Half(p_cta_instance->core_input.p_pa_data->vehicle_data.host_width);

      if (dist_to_crash_line > p_cta_instance->customer_calibration.k_honda_max_dist_crashline)
      {
         p_cta_output->f_cta_alert_left = FBK_ZERO_UINT;
      }
   }

   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] > CTA_CRIT_LEVEL_1)
   {
      p_cta_output->f_cta_alert_right = FBK_ONE_UINT;
      id_crit_obj                     = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      Fbk_Calculate_Target_Corners(&obj_corners, &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].vcs_pos),
                                   &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].vcs_heading),
                                   &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].length),
                                   &(p_cta_instance->core_input.p_pa_data->object_data[id_crit_obj].width));

      dist_to_crash_line =
         Fbk_Min(Fbk_Abs_F(obj_corners.points[FBK_FRONT_LEFT_CORNER].y), Fbk_Abs_F(obj_corners.points[FBK_FRONT_RIGHT_CORNER].y))
         - Fbk_Half(p_cta_instance->core_input.p_pa_data->vehicle_data.host_width);

      if (dist_to_crash_line > p_cta_instance->customer_calibration.k_honda_max_dist_crashline)
      {
         p_cta_output->f_cta_alert_right = FBK_ZERO_UINT;
      }
   }
}

/**
 * @brief This function is implemented to set or clear output data
 *
 * @return     void
 * @requirements{}
 */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
static void Cta_Set_Output(Cta_Output_T *p_cta_output, Cta_Instance_T *p_cta_instance)
{
   Fbk_Object_Corners_T obj_corners;

   /* Asserts */
   assert(NULL != p_cta_output);
   assert(NULL != p_cta_instance);

   /* At first set some basics */
   /* set enabled flag */
   if (CTA_STATUS_ACTIVE == p_cta_instance->core_output.cta_status)
   {
      p_cta_output->f_cta_enabled = FBK_ONE_UINT;
   }
   else
   {
      p_cta_output->f_cta_enabled = FBK_ZERO_UINT;
   }

   /* Mapping of warn level */
   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] == CTA_CRIT_LEVEL_1)
   {
      p_cta_output->f_cta_warn_left = FBK_ONE_UINT;
   }
   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] == CTA_CRIT_LEVEL_1)
   {
      p_cta_output->f_cta_warn_right = FBK_ONE_UINT;
   }

   Cta_Map_Alert(p_cta_output, p_cta_instance);


   /*Check whether alert is active for the respective side and if the brake qualifier is set*/
   if ((Fbk_Is_True(p_cta_output->f_cta_alert_right)
        && Fbk_Is_True(p_cta_instance->core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]))
       || (Fbk_Is_True(p_cta_output->f_cta_alert_left)
           && Fbk_Is_True(p_cta_instance->core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT])))
   {
      p_cta_output->f_brake_qualifier = FBK_ONE_UINT;
   }

   /*Mapping of object properties*/
   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] >= CTA_CRIT_LEVEL_1)
   {

      Vector_2d_T obj_pos = Create_2d_Vector_Coordinates(
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]]
            .vcs_pos.x,
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]]
            .vcs_pos.y);
      float32_T obj_length =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]].length;
      float32_T obj_width =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]].width;

      Fbk_Calculate_Target_Corners(&obj_corners, &obj_pos, &p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT],
                                   &obj_length, &obj_width);

      /* Set ID and TTV of the obcject responsible for CTA alert. If alert is ON but core ID is zero (in case of alert holding) use
       * previous values*/
      if (PA_INVALID_OBJ_ID == p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT])
      {
         p_cta_output->cta_id_left  = Cta_Honda_Persistent.cta_prev_id_left;
         p_cta_output->cta_ttc_left = Cta_Honda_Persistent.cta_prev_ttc_left;
      }
      else
      {
         p_cta_output->cta_id_left  = p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT];
         p_cta_output->cta_ttc_left = p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT];
      }
      /* Set quasi-persistent data */
      Cta_Honda_Persistent.cta_prev_id_left  = p_cta_output->cta_id_left;
      Cta_Honda_Persistent.cta_prev_ttc_left = p_cta_output->cta_ttc_left;

      p_cta_output->cta_objPoseX_left = obj_corners.points[FBK_FRONT_LEFT_CORNER].x;
      p_cta_output->cta_objPoseY_left = obj_corners.points[FBK_FRONT_LEFT_CORNER].y;
      p_cta_output->cta_objVelocityX_left =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]]
            .vcs_pos.x;
      p_cta_output->cta_objVelocityY_left =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]]
            .vcs_pos.y;
      p_cta_output->cta_heading_left              = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->cta_intersection_point_x_left = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT];
   }

   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] >= CTA_CRIT_LEVEL_1)
   {
      Vector_2d_T obj_pos = Create_2d_Vector_Coordinates(
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]]
            .vcs_pos.x,
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]]
            .vcs_pos.y);
      float32_T obj_length =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]].length;
      float32_T obj_width =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]].width;

      Fbk_Calculate_Target_Corners(&obj_corners, &obj_pos, &p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT],
                                   &obj_length, &obj_width);

      /* Set ID and TTV of the obcject responsible for CTA alert. If alert is ON but core ID is zero (in case of alert holding) use
       * previous values*/
      if (PA_INVALID_OBJ_ID == p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT])
      {
         p_cta_output->cta_id_right  = Cta_Honda_Persistent.cta_prev_id_right;
         p_cta_output->cta_ttc_right = Cta_Honda_Persistent.cta_prev_ttc_right;
      }
      else
      {
         p_cta_output->cta_id_right  = p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT];
         p_cta_output->cta_ttc_right = p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      }
      /* Set quasi-persistent data */
      Cta_Honda_Persistent.cta_prev_id_right  = p_cta_output->cta_id_right;
      Cta_Honda_Persistent.cta_prev_ttc_right = p_cta_output->cta_ttc_right;

      p_cta_output->cta_objPoseX_right = obj_corners.points[FBK_FRONT_RIGHT_CORNER].x;
      p_cta_output->cta_objPoseY_right = obj_corners.points[FBK_FRONT_RIGHT_CORNER].y;
      p_cta_output->cta_objVelocityX_right =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]]
            .vcs_vel.x;
      p_cta_output->cta_objVelocityY_right =
         p_cta_instance->core_input.p_pa_data->object_data[p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]]
            .vcs_vel.y;
      p_cta_output->cta_heading_right              = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->cta_intersection_point_x_right = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   }
}


#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Honda_SRR6_Output(const Cta_Output_T *p_cta_output)
{
   CTA_STORE_VAL_MGR_WPR("honda_srr6_f_cta_enabled", p_cta_output->f_cta_enabled);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_f_cta_warn_left", p_cta_output->f_cta_warn_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_f_cta_alert_left", p_cta_output->f_cta_alert_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_id_left", p_cta_output->cta_id_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_ttc_left", p_cta_output->cta_ttc_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objPoseX_left", p_cta_output->cta_objPoseX_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objPoseY_left", p_cta_output->cta_objPoseY_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objVelocityX_left", p_cta_output->cta_objVelocityX_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objVelocityY_left", p_cta_output->cta_objVelocityY_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_heading_left", p_cta_output->cta_heading_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_intersection_point_x_left", p_cta_output->cta_intersection_point_x_left);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_f_cta_warn_right", p_cta_output->f_cta_warn_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_f_cta_alert_right", p_cta_output->f_cta_alert_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_id_right", p_cta_output->cta_id_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_ttc_right", p_cta_output->cta_ttc_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objPoseX_right", p_cta_output->cta_objPoseX_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objPoseY_right", p_cta_output->cta_objPoseY_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objVelocityX_right", p_cta_output->cta_objVelocityX_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_objVelocityY_right", p_cta_output->cta_objVelocityY_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_heading_right", p_cta_output->cta_heading_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_cta_intersection_point_x_right", p_cta_output->cta_intersection_point_x_right);
   CTA_STORE_VAL_MGR_WPR("honda_srr6_f_brake_qualifier", p_cta_output->f_brake_qualifier);
}
#endif /* BINARY_DEBUG */
