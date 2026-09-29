/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the RNA_SWEET400 post run logic for CTA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
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
#include "fbk_output.h" // IWYU pragma: keep
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_interval.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h" // IWYU pragma: keep
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Resets all signals in RNA_SWEET400 specific output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2458}
 * @SDD{SF-4021}
 */
static void Cta_Reset_Output(Cta_Output_T *p_cta_output /**< CTA output pointer*/);


/**
 * @brief This function is implemented to set output data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2458}
 * @SDD{SF-4019}
 */
static void Cta_Set_Output(Cta_Output_T *p_cta_output /**<CTA Output Pointer*/,
                           Cta_Instance_T *p_cta_instance /**<CTA Instance Pointer*/,
                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);

/**
 * @brief This function is implemented to calculate specific customer relevant corner points of target.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2458}
 * @SDD{SF-4020}
 */
static void Cta_Calculate_Relevant_Target_Corners(Fbk_Object_Corners_T *p_target_corners /**<CTA Object Corners Pointer*/,
                                                  Cta_Instance_T *cta_instance /**<CTA Instance Pointer*/,
                                                  const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                                                  const uint8_t approach_side /**< approach side*/);

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Rna_Output(const Cta_Output_T *p_cta_output);
#define Binary_Write_Cta_Customer_Output(p_cta_output) Write_Debug_Cta_Rna_Output(p_cta_output)
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
}

// clang-format off
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
// clang-format on
{
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_cta_instance->core_input.p_pa_data->vehicle_data;

   assert(NULL != p_cta_instance);
   assert(NULL != p_cta_input);
   assert(NULL != p_cta_output);

   Cta_Reset_Output(p_cta_output);

   Cta_Set_Output(p_cta_output, p_cta_instance, p_vehicle_data);

   Binary_Write_Cta_Customer_Output(p_cta_output);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Cta_Reset_Output(Cta_Output_T *p_cta_output)
{
   p_cta_output->cta_id_right                 = FBK_ZERO_INT;
   p_cta_output->cta_ttc_right                = FBK_ZERO_F;
   p_cta_output->cta_intersectionX_right      = FBK_ZERO_F;
   p_cta_output->cta_radialDistance_right     = FBK_ZERO_F;
   p_cta_output->cta_objPoseX_right           = 20.0024205f;
   p_cta_output->cta_objPoseY_right           = 100.0062665f;
   p_cta_output->cta_objVelocityX_right       = 50.00641f;
   p_cta_output->cta_objVelocityY_right       = 50.00641f;
   p_cta_output->f_cta_alert_right            = FBK_ZERO_UINT;
   p_cta_output->f_cta_warn_right             = FBK_ZERO_UINT;
   p_cta_output->f_cta_prefill_req_right      = FBK_ZERO_UINT;
   p_cta_output->f_cta_braking_req_right      = FBK_ZERO_UINT;
   p_cta_output->f_cta_hold_supp_right        = FBK_ZERO_UINT;
   p_cta_output->RCTA_Criticality_level_right = FBK_ZERO_UINT; /* Non critical */
   p_cta_output->cta_heading_rear_left        = FBK_ZERO_F;

   p_cta_output->cta_id_left                 = FBK_ZERO_INT;
   p_cta_output->cta_ttc_left                = FBK_ZERO_F;
   p_cta_output->cta_intersectionX_left      = FBK_ZERO_F;
   p_cta_output->cta_radialDistance_left     = FBK_ZERO_F;
   p_cta_output->cta_objPoseX_left           = 20.0024205f;
   p_cta_output->cta_objPoseY_left           = 100.0062665f;
   p_cta_output->cta_objVelocityX_left       = 50.00641f;
   p_cta_output->cta_objVelocityY_left       = 50.00641f;
   p_cta_output->f_cta_alert_left            = FBK_ZERO_UINT;
   p_cta_output->f_cta_warn_left             = FBK_ZERO_UINT;
   p_cta_output->f_cta_prefill_req_left      = FBK_ZERO_UINT;
   p_cta_output->f_cta_braking_req_left      = FBK_ZERO_UINT;
   p_cta_output->f_cta_hold_supp_left        = FBK_ZERO_UINT;
   p_cta_output->RCTA_Criticality_level_left = FBK_ZERO_UINT; /* Non critical */
   p_cta_output->cta_heading_rear_right      = FBK_ZERO_F;

   /* obsolete error flags are beeing mocked */
   p_cta_output->error_flags.f_error_flag_unused   = FBK_ZERO_UINT;
   p_cta_output->error_flags.f_cals_ptr_null       = FBK_ZERO_UINT;
   p_cta_output->error_flags.f_input_ptr_null      = FBK_ZERO_UINT;
   p_cta_output->error_flags.f_persistant_ptr_null = FBK_ZERO_UINT;
   p_cta_output->error_flags.f_tracker_ptr_null    = FBK_ZERO_UINT;
   p_cta_output->error_flags.f_vehicle_ptr_null    = FBK_ZERO_UINT;
}

static void Cta_Set_Output(Cta_Output_T *p_cta_output, Cta_Instance_T *p_cta_instance, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Fbk_Object_Corners_T target_corners;
   Vector_2d_T rad_distance_origin;
   uint8_t obj_idx;

   rad_distance_origin = Create_2d_Vector_Coordinates(-FBK_ONE_F * p_vehicle_data->host_length, FBK_ZERO_F);
   /* XCP, UDP output */
   /* check for alerts and set (XCP, UDP) output depending on approach side */

   p_cta_output->f_cta_enabled = (CTA_STATUS_ACTIVE == p_cta_instance->core_output.cta_status) ? 1u : 0u;

   obj_idx = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT];
   if ((PA_INVALID_OBJ_INDEX != obj_idx) && p_cta_instance->core_input.p_pa_data->object_data[obj_idx].f_is_in_rl_sensor_fov)
   {
      p_cta_output->f_cta_alert_left =
         ((p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] >= CTA_CRIT_LEVEL_2) ? 1u : 0u);
      p_cta_output->f_cta_warn_left =
         ((p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] >= CTA_CRIT_LEVEL_1) ? 1u : 0u);
      p_cta_output->RCTA_Criticality_level_left =
         ((uint8_t) p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   }

   if ((PA_INVALID_OBJ_INDEX != obj_idx)
       && (CTA_CRIT_LEVEL_NONE != p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]))
   {
      Vector_2d_T target_pos;
      Cta_Calculate_Relevant_Target_Corners(&target_corners, p_cta_instance, p_vehicle_data, FBK_SIDE_LEFT);

      target_pos = Create_2d_Vector_Coordinates(p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_pos.x,
                                                p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_pos.y);

      p_cta_output->cta_id_left = (int32_t) p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->cta_ttc_left = Enforce_Range(p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT], -10.0f, 60.0f);
      p_cta_output->cta_intersectionX_left  = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->cta_radialDistance_left = Vector_2d_Alg_Distance(&(target_pos), &(rad_distance_origin));
      p_cta_output->cta_objPoseX_left       = target_corners.points[0].x;
      p_cta_output->cta_objPoseY_left       = target_corners.points[0].y;
      p_cta_output->cta_objVelocityX_left   = p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_vel_rel.x;
      p_cta_output->cta_objVelocityY_left   = p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_vel_rel.y;
      p_cta_output->cta_heading_rear_left   = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT];
   }
   else
   {
      /* turn off alerts                 */
      /* set XCP, UDP outputs to default */
      p_cta_output->cta_id_left             = FBK_ZERO_INT;
      p_cta_output->cta_ttc_left            = FBK_ZERO_F;
      p_cta_output->cta_intersectionX_left  = FBK_ZERO_F;
      p_cta_output->cta_radialDistance_left = FBK_ZERO_F;
      p_cta_output->cta_objPoseX_left       = 20.0024205f;
      p_cta_output->cta_objPoseY_left       = 100.0062665f;
      p_cta_output->cta_objVelocityX_left   = 50.00641f;
      p_cta_output->cta_objVelocityY_left   = 50.00641f;
      p_cta_output->cta_heading_rear_left   = FBK_ZERO_F;
   }

   obj_idx = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   if ((PA_INVALID_OBJ_INDEX != obj_idx) && p_cta_instance->core_input.p_pa_data->object_data[obj_idx].f_is_in_rr_sensor_fov)
   {
      p_cta_output->f_cta_alert_right =
         ((p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] >= CTA_CRIT_LEVEL_2) ? 1u : 0u);
      p_cta_output->f_cta_warn_right =
         ((p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] >= CTA_CRIT_LEVEL_1) ? 1u : 0u);
      p_cta_output->RCTA_Criticality_level_right =
         (uint8_t) p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   }

   if ((PA_INVALID_OBJ_INDEX != obj_idx)
       && (CTA_CRIT_LEVEL_NONE != p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]))
   {
      Vector_2d_T target_pos;
      Cta_Calculate_Relevant_Target_Corners(&target_corners, p_cta_instance, p_vehicle_data, FBK_SIDE_RIGHT);

      target_pos = Create_2d_Vector_Coordinates(p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_pos.x,
                                                p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_pos.y);

      p_cta_output->cta_id_right = (int32_t) p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->cta_ttc_right =
         Enforce_Range(p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT], -10.0f, 60.0f);
      p_cta_output->cta_intersectionX_right  = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->cta_radialDistance_right = Vector_2d_Alg_Distance(&(target_pos), &(rad_distance_origin));
      p_cta_output->cta_objPoseX_right       = target_corners.points[2].x;
      p_cta_output->cta_objPoseY_right       = target_corners.points[2].y;
      p_cta_output->cta_objVelocityX_right   = p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_vel_rel.x;
      p_cta_output->cta_objVelocityY_right   = p_cta_instance->core_input.p_pa_data->object_data[obj_idx].vcs_vel_rel.y;
      p_cta_output->cta_heading_rear_right   = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   }
   else
   {
      /* turn off alerts                 */
      /* turn off alerts                 */
      /* set XCP, UDP outputs to default */
      p_cta_output->cta_id_right             = FBK_ZERO_INT;
      p_cta_output->cta_ttc_right            = FBK_ZERO_F;
      p_cta_output->cta_intersectionX_right  = FBK_ZERO_F;
      p_cta_output->cta_radialDistance_right = FBK_ZERO_F;
      p_cta_output->cta_objPoseX_right       = 20.0024205f;
      p_cta_output->cta_objPoseY_right       = 100.0062665f;
      p_cta_output->cta_objVelocityX_right   = 50.00641f;
      p_cta_output->cta_objVelocityY_right   = 50.00641f;
      p_cta_output->cta_heading_rear_right   = FBK_ZERO_F;
   }
}
/* clang-format off */
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type.] */
static void Cta_Calculate_Relevant_Target_Corners(Fbk_Object_Corners_T *p_target_corners, Cta_Instance_T *cta_instance, const Fbk_Vehicle_Data_T *p_vehicle_data, const uint8_t approach_side)
/* clang-format on */
{
   float32_T half_length;
   float32_T half_width;
   Vector_2d_T fl_corner;
   Vector_2d_T fr_corner;
   Angle_T heading;
   Vector_2d_T target_pos_vcs;

   uint8_t object_index;

   assert(p_target_corners != NULL);
   assert(cta_instance != NULL);

   object_index = cta_instance->core_output.cta_index[CTA_MODE_REAR][approach_side];

   heading     = Create_Angle(cta_instance->core_output.cta_heading[CTA_MODE_REAR][approach_side]);
   half_length = Fbk_Half(cta_instance->core_input.p_pa_data->object_data[object_index].length);
   half_width  = Fbk_Half(cta_instance->core_input.p_pa_data->object_data[object_index].width);

   /* set unrotated corner points in origin of coordinate system*/
   fl_corner = Create_2d_Vector_Coordinates(half_length, -half_width);
   fr_corner = Create_2d_Vector_Coordinates(half_length, half_width);

   /*Application of 2D rotation matrix */
   p_target_corners->points[0] = Vector_2d_Alg_Rotate(&(heading), &(fl_corner));
   p_target_corners->points[2] = Vector_2d_Alg_Rotate(&(heading), &(fr_corner));

   /*Shifting rotated corner points depending on vehicle position*/
   target_pos_vcs.x            = cta_instance->core_input.p_pa_data->object_data[object_index].vcs_pos.x;
   target_pos_vcs.y            = cta_instance->core_input.p_pa_data->object_data[object_index].vcs_pos.y;
   p_target_corners->points[0] = Vector_2d_Alg_Add(&(target_pos_vcs), &(p_target_corners->points[0]));
   p_target_corners->points[2] = Vector_2d_Alg_Add(&(target_pos_vcs), &(p_target_corners->points[2]));

   /*Convert to customer coordinate system - origin in the middle of rear bumper and positive value of y axis is to the left*/
   p_target_corners->points[0].x = p_target_corners->points[0].x + p_vehicle_data->host_length;
   p_target_corners->points[2].x = p_target_corners->points[2].x + p_vehicle_data->host_length;
   p_target_corners->points[0].y = -p_target_corners->points[0].y;
   p_target_corners->points[2].y = -p_target_corners->points[2].y;
}

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Rna_Output(const Cta_Output_T *p_cta_output)
{
   CTA_STORE_VAL_MGR_WPR("f_cta_alert_left", p_cta_output->f_cta_alert_left);
   CTA_STORE_VAL_MGR_WPR("f_cta_warn_left", p_cta_output->f_cta_warn_left);
   CTA_STORE_VAL_MGR_WPR("f_cta_prefill_req_left", p_cta_output->f_cta_prefill_req_left);
   CTA_STORE_VAL_MGR_WPR("f_cta_braking_req_left", p_cta_output->f_cta_braking_req_left);
   CTA_STORE_VAL_MGR_WPR("f_cta_hold_supp_left", p_cta_output->f_cta_hold_supp_left);
   CTA_STORE_VAL_MGR_WPR("cta_id_left", p_cta_output->cta_id_left);
   CTA_STORE_VAL_MGR_WPR("cta_ttc_left", p_cta_output->cta_ttc_left);
   CTA_STORE_VAL_MGR_WPR("cta_radialDistance_left", p_cta_output->cta_radialDistance_left);
   CTA_STORE_VAL_MGR_WPR("cta_intersectionX_left", p_cta_output->cta_intersectionX_left);
   CTA_STORE_VAL_MGR_WPR("RCTA_Criticality_level_left", p_cta_output->RCTA_Criticality_level_left);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lon_pos_left", p_cta_output->cta_objPoseX_left);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lat_pos_left", p_cta_output->cta_objPoseY_left);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lon_vel_left", p_cta_output->cta_objVelocityX_left);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lat_vel_left", p_cta_output->cta_objVelocityY_left);
   CTA_STORE_VAL_MGR_WPR("cta_heading_rear_left", p_cta_output->cta_heading_rear_left);

   CTA_STORE_VAL_MGR_WPR("f_cta_alert_right", p_cta_output->f_cta_alert_right);
   CTA_STORE_VAL_MGR_WPR("f_cta_warn_right", p_cta_output->f_cta_warn_right);
   CTA_STORE_VAL_MGR_WPR("f_cta_prefill_req_right", p_cta_output->f_cta_prefill_req_right);
   CTA_STORE_VAL_MGR_WPR("f_cta_braking_req_right", p_cta_output->f_cta_braking_req_right);
   CTA_STORE_VAL_MGR_WPR("f_cta_hold_supp_right", p_cta_output->f_cta_hold_supp_right);
   CTA_STORE_VAL_MGR_WPR("cta_id_right", p_cta_output->cta_id_right);
   CTA_STORE_VAL_MGR_WPR("cta_ttc_right", p_cta_output->cta_ttc_right);
   CTA_STORE_VAL_MGR_WPR("cta_radialDistance_right", p_cta_output->cta_radialDistance_right);
   CTA_STORE_VAL_MGR_WPR("cta_intersectionX_right", p_cta_output->cta_intersectionX_right);
   CTA_STORE_VAL_MGR_WPR("RCTA_Criticality_level_right", p_cta_output->RCTA_Criticality_level_right);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lon_pos_right", p_cta_output->cta_objPoseX_right);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lat_pos_right", p_cta_output->cta_objPoseY_right);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lon_vel_right", p_cta_output->cta_objVelocityX_right);
   CTA_STORE_VAL_MGR_WPR("cta_obj_lat_vel_right", p_cta_output->cta_objVelocityY_right);
   CTA_STORE_VAL_MGR_WPR("cta_heading_rear_right", p_cta_output->cta_heading_rear_right);
}

#endif /* BINARY_DEBUG */
