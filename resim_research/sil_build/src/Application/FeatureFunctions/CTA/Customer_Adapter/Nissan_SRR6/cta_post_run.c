/**
 * @file cta_post_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the Nissan SRR6 post run logic for CTA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "cta_post_run.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_interval.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include <assert.h>

#ifdef BINARY_DEBUG
#include "cta_debug_writer.h"
#endif /* BINARY_DEBUG */

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define CTA_DEFAULT_NISSAN_SRR6_POS_LONG (20.0024205f)
#define CTA_DEFAULT_NISSAN_SRR6_POS_LAT (100.0062665f)
#define CTA_DEFAULT_NISSAN_SRR6_VEL_LONG (50.00641f)
#define CTA_DEFAULT_NISSAN_SRR6_VEL_LAT (50.00641f)

#define CTA_MIN_NISSAN_SRR6_POS_LONG (-120.0f)
#define CTA_MIN_NISSAN_SRR6_POS_LAT (-100.0f)
#define CTA_MIN_NISSAN_SRR6_VEL_LONG (-50.0f)
#define CTA_MIN_NISSAN_SRR6_VEL_LAT (-50.0f)

#define CTA_MAX_NISSAN_SRR6_POS_LONG (20.0024205f)
#define CTA_MAX_NISSAN_SRR6_POS_LAT (100.0062665f)
#define CTA_MAX_NISSAN_SRR6_VEL_LONG (50.00641f)
#define CTA_MAX_NISSAN_SRR6_VEL_LAT (50.00641f)

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Resets all signals in generic CTA output.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3990}
 * @verification{}
 */
static void Cta_Reset_Output(Cta_Output_T *p_cta_output /**< CTA Output */);

/**
 * @brief This function is implemented to set or clear output data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3988}
 * @verification{}
 */
static void Cta_Set_Output(Cta_Output_T *p_cta_output /**< CTA Output */,
                           const Cta_Instance_T *p_cta_instance /**< CTA instance */,
                           const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/);

/**
 * @brief This function is implemented to calculate Nissan Cta target relevant point coordiantes.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3989}
 * @verification{}
 */
static void Cta_Calculate_Nissan_Cta_Target_Corners(Fbk_Object_Corners_T *p_target_corners /**< FBK object corners */,
                                                    const Cta_Instance_T *p_cta_instance /**< CTA Instance */,
                                                    const Fbk_Vehicle_Data_T *p_vehicle_data /**< host vehicle data*/,
                                                    const uint8_t approach_side /**< CTA approach side */);

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Nissan_SRR6_Output(const Cta_Output_T *p_CTA_output);
#define Binary_Write_Cta_Customer_Output(p_cta_output) Write_Debug_Cta_Nissan_SRR6_Output(p_cta_output)
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
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed]*/
/* coverity[misra_c_2012_rule_8_13_violation][The pointer variable "p_cta_instance" points to a non-constant type but does not modify the object it points to. Consider adding const qualifier to the points-to type.] */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output)
// clang-format on
{
   const Fbk_Vehicle_Data_T *p_vehicle_data = &p_cta_instance->core_input.p_pa_data->vehicle_data;

   /* Asserts */
   assert(NULL != p_cta_output);
   assert(NULL != p_cta_input);
   assert(NULL != p_cta_instance);

   /* Reset output */
   Cta_Reset_Output(p_cta_output);

   /* Set new output */
   Cta_Set_Output(p_cta_output, p_cta_instance, p_vehicle_data);

   /* Write debug output */
   Binary_Write_Cta_Customer_Output(p_cta_output);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Cta_Reset_Output(Cta_Output_T *p_cta_output)
{
   p_cta_output->f_cta_enabled = FBK_ZERO_UINT;

   p_cta_output->f_cta_alert_left              = FBK_ZERO_UINT;
   p_cta_output->cta_id_left                   = FBK_ZERO_UINT;
   p_cta_output->cta_ttc_left                  = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objPoseX_left             = CTA_DEFAULT_NISSAN_SRR6_POS_LONG;
   p_cta_output->cta_objPoseY_left             = CTA_DEFAULT_NISSAN_SRR6_POS_LAT;
   p_cta_output->cta_objVelocityX_left         = CTA_DEFAULT_NISSAN_SRR6_VEL_LONG;
   p_cta_output->cta_objVelocityY_left         = CTA_DEFAULT_NISSAN_SRR6_VEL_LAT;
   p_cta_output->cta_heading_left              = FBK_ZERO_F;
   p_cta_output->cta_intersection_point_x_left = FBK_ZERO_F;

   p_cta_output->f_cta_alert_right              = FBK_ZERO_UINT;
   p_cta_output->cta_id_right                   = FBK_ZERO_UINT;
   p_cta_output->cta_ttc_right                  = CTA_HIGH_DEFAULT_VAL;
   p_cta_output->cta_objPoseX_right             = CTA_DEFAULT_NISSAN_SRR6_POS_LONG;
   p_cta_output->cta_objPoseY_right             = CTA_DEFAULT_NISSAN_SRR6_POS_LAT;
   p_cta_output->cta_objVelocityX_right         = CTA_DEFAULT_NISSAN_SRR6_VEL_LONG;
   p_cta_output->cta_objVelocityY_right         = CTA_DEFAULT_NISSAN_SRR6_VEL_LAT;
   p_cta_output->cta_heading_right              = FBK_ZERO_F;
   p_cta_output->cta_intersection_point_x_right = FBK_ZERO_F;
}

static void Cta_Set_Output(Cta_Output_T *p_cta_output, const Cta_Instance_T *p_cta_instance, const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   Fbk_Object_Corners_T obj_corners;
   float32_T cta_obj_vel_long;
   float32_T cta_obj_vel_lat;
   uint8_t cta_index;

   /* set enabled flag */
   if (CTA_STATUS_ACTIVE == p_cta_instance->core_output.cta_status)
   {
      p_cta_output->f_cta_enabled = FBK_ONE_UINT;
   }
   else
   {
      p_cta_output->f_cta_enabled = FBK_ZERO_UINT;
   }

   /* cta alert may be triggered by higher criticality level */
   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] > CTA_CRIT_LEVEL_1)
   {
      cta_index = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT];
      Cta_Calculate_Nissan_Cta_Target_Corners(&obj_corners, p_cta_instance, p_vehicle_data, FBK_SIDE_LEFT);
      cta_obj_vel_long = p_cta_instance->core_input.p_pa_data->object_data[cta_index].vcs_vel.x;
      /* The customer's coordinate system has an inverted lateral axis -> change the sign of the lateral velocity */
      cta_obj_vel_lat = -p_cta_instance->core_input.p_pa_data->object_data[cta_index].vcs_vel.y;

      p_cta_output->f_cta_alert_left = FBK_ONE_UINT;
      p_cta_output->cta_id_left      = p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->cta_ttc_left =
         Enforce_Range(p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT], FBK_ZERO_F, CTA_HIGH_DEFAULT_VAL);
      p_cta_output->cta_objPoseX_left =
         Enforce_Range(obj_corners.points[FBK_FRONT_LEFT_CORNER].x, CTA_MIN_NISSAN_SRR6_POS_LONG, CTA_MAX_NISSAN_SRR6_POS_LONG);
      p_cta_output->cta_objPoseY_left =
         Enforce_Range(obj_corners.points[FBK_FRONT_LEFT_CORNER].y, CTA_MIN_NISSAN_SRR6_POS_LAT, CTA_MAX_NISSAN_SRR6_POS_LAT);
      p_cta_output->cta_objVelocityX_left =
         Enforce_Range(cta_obj_vel_long, CTA_MIN_NISSAN_SRR6_VEL_LONG, CTA_MAX_NISSAN_SRR6_VEL_LONG);
      p_cta_output->cta_objVelocityY_left = Enforce_Range(cta_obj_vel_lat, CTA_MIN_NISSAN_SRR6_VEL_LAT, CTA_MAX_NISSAN_SRR6_VEL_LAT);
      p_cta_output->cta_heading_left      = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT];
      p_cta_output->cta_intersection_point_x_left = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT];
   }

   if (p_cta_instance->core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] > CTA_CRIT_LEVEL_1)
   {
      Cta_Calculate_Nissan_Cta_Target_Corners(&obj_corners, p_cta_instance, p_vehicle_data, FBK_SIDE_RIGHT);
      cta_index        = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      cta_obj_vel_long = p_cta_instance->core_input.p_pa_data->object_data[cta_index].vcs_vel.x;
      /* The customer's coordinate system has an inverted lateral axis -> change the sign of the lateral velocity */
      cta_obj_vel_lat = -p_cta_instance->core_input.p_pa_data->object_data[cta_index].vcs_vel.y;

      p_cta_output->f_cta_alert_right = FBK_ONE_UINT;
      p_cta_output->cta_id_right      = p_cta_instance->core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->cta_ttc_right =
         Enforce_Range(p_cta_instance->core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT], FBK_ZERO_F, CTA_HIGH_DEFAULT_VAL);
      p_cta_output->cta_objPoseX_right =
         Enforce_Range(obj_corners.points[FBK_FRONT_RIGHT_CORNER].x, CTA_MIN_NISSAN_SRR6_POS_LONG, CTA_MAX_NISSAN_SRR6_POS_LONG);
      p_cta_output->cta_objPoseY_right =
         Enforce_Range(obj_corners.points[FBK_FRONT_RIGHT_CORNER].y, CTA_MIN_NISSAN_SRR6_POS_LAT, CTA_MAX_NISSAN_SRR6_POS_LAT);
      p_cta_output->cta_objVelocityX_right =
         Enforce_Range(cta_obj_vel_long, CTA_MIN_NISSAN_SRR6_VEL_LONG, CTA_MAX_NISSAN_SRR6_VEL_LONG);
      p_cta_output->cta_objVelocityY_right = Enforce_Range(cta_obj_vel_lat, CTA_MIN_NISSAN_SRR6_VEL_LAT, CTA_MAX_NISSAN_SRR6_VEL_LAT);
      p_cta_output->cta_heading_right      = p_cta_instance->core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT];
      p_cta_output->cta_intersection_point_x_right = p_cta_instance->core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT];
   }
}

static void Cta_Calculate_Nissan_Cta_Target_Corners(Fbk_Object_Corners_T *p_target_corners,
                                                    const Cta_Instance_T *p_cta_instance,
                                                    const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                    const uint8_t approach_side)
{
   float32_T half_length;
   float32_T half_width;
   float32_T lateral_offset;
   Vector_2d_T fl_corner;
   Vector_2d_T fr_corner;
   Angle_T heading;
   Vector_2d_T target_pos_vcs;

   uint8_t object_index;

   assert(p_target_corners != NULL);
   assert(p_cta_instance != NULL);

   object_index = p_cta_instance->core_output.cta_index[CTA_MODE_REAR][approach_side];

   heading     = Create_Angle(p_cta_instance->core_input.p_pa_data->object_data[object_index].vcs_heading);
   half_length = Fbk_Half(p_cta_instance->core_input.p_pa_data->object_data[object_index].length);
   half_width  = Fbk_Half(p_cta_instance->core_input.p_pa_data->object_data[object_index].width);

   if (approach_side == FBK_SIDE_LEFT)
   {
      lateral_offset = -Fbk_Half(p_vehicle_data->host_width);
   }
   else
   {
      lateral_offset = Fbk_Half(p_vehicle_data->host_width);
   }

   /* set unrotated corner points in origin of coordinate system*/
   fl_corner = Create_2d_Vector_Coordinates(half_length, -half_width);
   fr_corner = Create_2d_Vector_Coordinates(half_length, half_width);

   /*Application of 2D rotation matrix */
   p_target_corners->points[FBK_FRONT_LEFT_CORNER]  = Vector_2d_Alg_Rotate(&(heading), &(fl_corner));
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER] = Vector_2d_Alg_Rotate(&(heading), &(fr_corner));

   /*Shifting rotated corner points depending on vehicle position*/
   target_pos_vcs.x = p_cta_instance->core_input.p_pa_data->object_data[object_index].vcs_pos.x;
   target_pos_vcs.y = p_cta_instance->core_input.p_pa_data->object_data[object_index].vcs_pos.y;
   p_target_corners->points[FBK_FRONT_LEFT_CORNER] =
      Vector_2d_Alg_Add(&(target_pos_vcs), &(p_target_corners->points[FBK_FRONT_LEFT_CORNER]));
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER] =
      Vector_2d_Alg_Add(&(target_pos_vcs), &(p_target_corners->points[FBK_FRONT_RIGHT_CORNER]));

   /*Convert to customer coordinate system, rear bumper corner of vehicle on the objects approach side, inverted y-axis */
   p_target_corners->points[FBK_FRONT_LEFT_CORNER].x = p_target_corners->points[FBK_FRONT_LEFT_CORNER].x + p_vehicle_data->host_length;
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER].x =
      p_target_corners->points[FBK_FRONT_RIGHT_CORNER].x + p_vehicle_data->host_length;
   p_target_corners->points[FBK_FRONT_LEFT_CORNER].y  = -(p_target_corners->points[FBK_FRONT_LEFT_CORNER].y - lateral_offset);
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER].y = -(p_target_corners->points[FBK_FRONT_RIGHT_CORNER].y - lateral_offset);
}

#ifdef BINARY_DEBUG
static void Write_Debug_Cta_Nissan_SRR6_Output(const Cta_Output_T *p_CTA_output)
{
   CTA_STORE_VAL_MGR_WPR("f_cta_alert_left", p_CTA_output->f_cta_alert_left);
   CTA_STORE_VAL_MGR_WPR("cta_id_left", p_CTA_output->cta_id_left);
   CTA_STORE_VAL_MGR_WPR("cta_ttc_left", p_CTA_output->cta_ttc_left);
   CTA_STORE_VAL_MGR_WPR("cta_objPoseX_left", p_CTA_output->cta_objPoseX_left);
   CTA_STORE_VAL_MGR_WPR("cta_objPoseY_left", p_CTA_output->cta_objPoseY_left);
   CTA_STORE_VAL_MGR_WPR("cta_objVelocityX_left", p_CTA_output->cta_objVelocityX_left);
   CTA_STORE_VAL_MGR_WPR("cta_objVelocityY_left", p_CTA_output->cta_objVelocityY_left);
   CTA_STORE_VAL_MGR_WPR("cta_heading_left", p_CTA_output->cta_heading_left);
   CTA_STORE_VAL_MGR_WPR("cta_intersection_point_x_left", p_CTA_output->cta_intersection_point_x_left);

   CTA_STORE_VAL_MGR_WPR("f_cta_alert_right", p_CTA_output->f_cta_alert_right);
   CTA_STORE_VAL_MGR_WPR("cta_id_right", p_CTA_output->cta_id_right);
   CTA_STORE_VAL_MGR_WPR("cta_ttc_right", p_CTA_output->cta_ttc_right);
   CTA_STORE_VAL_MGR_WPR("cta_objPoseX_right", p_CTA_output->cta_objPoseX_right);
   CTA_STORE_VAL_MGR_WPR("cta_objPoseY_right", p_CTA_output->cta_objPoseY_right);
   CTA_STORE_VAL_MGR_WPR("cta_objVelocityX_right", p_CTA_output->cta_objVelocityX_right);
   CTA_STORE_VAL_MGR_WPR("cta_objVelocityY_right", p_CTA_output->cta_objVelocityY_right);
   CTA_STORE_VAL_MGR_WPR("cta_heading_right", p_CTA_output->cta_heading_right);
   CTA_STORE_VAL_MGR_WPR("cta_intersection_point_x_right", p_CTA_output->cta_intersection_point_x_right);
}
#endif /* BINARY_DEBUG */
