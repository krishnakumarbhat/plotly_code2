/**
 * @file fbk_circular_shape_calculator.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the defined functions mentioned in header,
 * which are related to calculating the circles around the objects coordinates.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_circular_shape_calculator.h"
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include <assert.h>

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

float32_T Fbk_Get_Circle_Radius(const uint8_t prediction_step,
                                const float32_T base_radius,
                                const float32_T base_gain,
                                const float32_T grow_gain)
{
   float32_T result_value = base_radius * base_gain;
   uint8_t i;
   /* Interval with small eps value is used */
   if ((grow_gain <= (FBK_ONE_F - EPSILON)) || (grow_gain >= (FBK_ONE_F + EPSILON)))
   {
      for (i = FBK_ZERO_UINT; i < prediction_step; i++)
      {
         result_value = result_value * grow_gain;
      }
   }
   return result_value;
}

Fbk_Circle_Center_Offset_T Fbk_Calculate_Ego_Circle_Center_Offsets(const float32_T host_length,
                                                                   const float32_T host_width,
                                                                   const Fbk_Ego_Predict_Data_T *p_fbk_ego_data)
{
   /* Calculate ego circle center offsets based on ego length and width. */
   Fbk_Circle_Center_Offset_T circle_center_offset_result;
   Fbk_Init_Circle_Center_Offset_Structure(&circle_center_offset_result);
   circle_center_offset_result.offset_front_x = (-0.5f * host_width) + p_fbk_ego_data->ego_circle_offset;
   circle_center_offset_result.offset_middle_x =
      -(0.5f * host_length * p_fbk_ego_data->ego_circle_host_length_factor) + p_fbk_ego_data->ego_circle_offset;
   circle_center_offset_result.offset_rear_x = -(host_length * p_fbk_ego_data->ego_circle_host_length_factor)
                                               - circle_center_offset_result.offset_front_x
                                               + (2.0f * p_fbk_ego_data->ego_circle_offset);

   return circle_center_offset_result;
}

Fbk_Circle_Center_Offset_T Fbk_Calculate_Obj_Circle_Center_Offsets(const float32_T obj_length, const float32_T obj_width)
{
   /* Calculate object circle center offsets based on object length and width. */
   Fbk_Circle_Center_Offset_T circle_center_offset_result;
   Fbk_Init_Circle_Center_Offset_Structure(&circle_center_offset_result);

   if (obj_length > obj_width)
   {
      /* Apply offsets from object center for front, middle and rear circles */
      circle_center_offset_result.offset_front_x  = ((obj_width / (-2.0f)) + (obj_length / 2.0f));
      circle_center_offset_result.offset_middle_x = 0.0f;
      circle_center_offset_result.offset_rear_x   = (circle_center_offset_result.offset_front_x * (-1.0f));
   }

   return circle_center_offset_result;
}

void Fbk_Fill_Circle_Center_Coordinates(Fbk_Waypoint_with_Circle_Centers_T *p_result,
                                        const float32_T offset_x_front,
                                        const float32_T offset_x_middle,
                                        const float32_T offset_x_rear)
{
   Angle_T yaw_angle = p_result->waypoint_yaw_angle;

   /* Assert */
   assert(NULL != p_result);

   /* calculate circle center of front circle (used later for criticality calculation) */
   p_result->circle_center_front.x = (p_result->waypoint_coordinates.x + (offset_x_front * yaw_angle.cos));
   p_result->circle_center_front.y = (p_result->waypoint_coordinates.y + (offset_x_front * yaw_angle.sin));

   /* calculate circle center of middle circle (used later for criticality calculation) */
   p_result->circle_center_middle.x = (p_result->waypoint_coordinates.x + (offset_x_middle * yaw_angle.cos));
   p_result->circle_center_middle.y = (p_result->waypoint_coordinates.y + (offset_x_middle * yaw_angle.sin));

   /* calculate circle center of rear circle (used later for criticality calculation) */
   p_result->circle_center_rear.x = (p_result->waypoint_coordinates.x + (offset_x_rear * yaw_angle.cos));
   p_result->circle_center_rear.y = (p_result->waypoint_coordinates.y + (offset_x_rear * yaw_angle.sin));
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Init_Circle_Center_Offset_Structure(Fbk_Circle_Center_Offset_T *p_circle_center_offset_structure)
{
   /* Assert */
   assert(NULL != p_circle_center_offset_structure);

   /* Reset the circle offsets */
   p_circle_center_offset_structure->offset_front_x  = FBK_ZERO_F;
   p_circle_center_offset_structure->offset_middle_x = FBK_ZERO_F;
   p_circle_center_offset_structure->offset_rear_x   = FBK_ZERO_F;
}

void Fbk_Init_Waypoint_with_Circle_Centers_Structure(Fbk_Waypoint_with_Circle_Centers_T *p_waypoint_with_circle_centers_structure)
{
   /* Assert */
   assert(NULL != p_waypoint_with_circle_centers_structure);

   /* Reset the waypoint circle properties */
   p_waypoint_with_circle_centers_structure->waypoint_coordinates = Create_2d_Vector_Origin();
   p_waypoint_with_circle_centers_structure->waypoint_yaw_angle   = Create_Angle(FBK_ZERO_F);
   p_waypoint_with_circle_centers_structure->waypoint_speed       = FBK_ZERO_F;
   p_waypoint_with_circle_centers_structure->circle_center_front  = Create_2d_Vector_Origin();
   p_waypoint_with_circle_centers_structure->circle_center_middle = Create_2d_Vector_Origin();
   p_waypoint_with_circle_centers_structure->circle_center_rear   = Create_2d_Vector_Origin();
   p_waypoint_with_circle_centers_structure->circle_radius        = FBK_ZERO_F;
   p_waypoint_with_circle_centers_structure->f_waypoint_valid     = FBK_FALSE;
}
