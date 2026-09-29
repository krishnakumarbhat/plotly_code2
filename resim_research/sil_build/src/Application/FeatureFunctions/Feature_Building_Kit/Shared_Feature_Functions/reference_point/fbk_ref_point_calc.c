/**
 * @file fbk_ref_point_calc.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file with functions for reference point calculations.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_ref_point_calc.h"
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math_infinity_silent.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Calculate_Ref_Point(Fbk_Ref_Point_T *p_target_ref_point,
                             const Vector_2d_T *p_host_ref_point,
                             const Fbk_Object_Corners_T *p_target_corners,
                             const boolean_T f_use_lateral_approximation)
{
   float32_T min_ref_point_distance = AS_TOOLBOX_INFINITY;
   float32_T ref_point_distance;
   Fbk_Reference_Position_T index_min_distance = FBK_FRONT_LEFT_CORNER;
   uint8_t idx;

   assert(NULL != p_target_ref_point);
   assert(NULL != p_host_ref_point);
   assert(NULL != p_target_corners);

   for (idx = (uint8_t) FBK_FRONT_LEFT_CORNER; idx < (uint8_t) FBK_NUM_OF_OBJECT_CORNERS; idx++)
   {
      if (Fbk_Is_True(f_use_lateral_approximation))
      {
         /* For objects next to the ego only calculate lateral distance between ego and target reference points */
         ref_point_distance = Fbk_Abs_F(p_target_corners->points[idx].y);
      }
      else
      {
         /* Calculate euclidean distance between ego and target reference points */
         ref_point_distance = Vector_2d_Alg_Distance(&(p_target_corners->points[idx]), p_host_ref_point);
      }

      if (ref_point_distance < min_ref_point_distance)
      {
         /* Get index of closest point */
         min_ref_point_distance = ref_point_distance;
         /* coverity[misra_c_2012_rule_10_5_violation][Intentional cast back to enum type] */
         index_min_distance = (Fbk_Reference_Position_T) idx;
      }
   }

   p_target_ref_point->ref_point_index = index_min_distance;
   p_target_ref_point->distance        = min_ref_point_distance;
   p_target_ref_point->point           = p_target_corners->points[index_min_distance];
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Calculate_Target_Corners(Fbk_Object_Corners_T *p_target_corners,
                                  const Vector_2d_T *p_target_vcs_pos,
                                  const float32_T *p_heading,
                                  const float32_T *p_length,
                                  const float32_T *p_width)
{
   float32_T half_length;
   float32_T half_width;
   Angle_T heading_angle;

   /* Asserts */
   assert(NULL != p_target_corners);
   assert(NULL != p_target_vcs_pos);
   assert(NULL != p_heading);
   assert(NULL != p_length);
   assert(NULL != p_width);

   heading_angle = Create_Angle((*p_heading));
   half_length   = Fbk_Half(*p_length);
   half_width    = Fbk_Half(*p_width);

   /* First set unrotated corner points in origin of coordinate system */
   p_target_corners->points[FBK_FRONT_LEFT_CORNER]  = Create_2d_Vector_Coordinates(half_length, -half_width);
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER] = Create_2d_Vector_Coordinates(half_length, half_width);
   p_target_corners->points[FBK_REAR_RIGHT_CORNER]  = Create_2d_Vector_Coordinates(-half_length, half_width);
   p_target_corners->points[FBK_REAR_LEFT_CORNER]   = Create_2d_Vector_Coordinates(-half_length, -half_width);

   /* Then apply 2D rotation matrix */
   p_target_corners->points[FBK_FRONT_LEFT_CORNER] =
      Vector_2d_Alg_Rotate(&(heading_angle), &(p_target_corners->points[FBK_FRONT_LEFT_CORNER]));
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER] =
      Vector_2d_Alg_Rotate(&(heading_angle), &(p_target_corners->points[FBK_FRONT_RIGHT_CORNER]));
   p_target_corners->points[FBK_REAR_RIGHT_CORNER] =
      Vector_2d_Alg_Rotate(&(heading_angle), &(p_target_corners->points[FBK_REAR_RIGHT_CORNER]));
   p_target_corners->points[FBK_REAR_LEFT_CORNER] =
      Vector_2d_Alg_Rotate(&(heading_angle), &(p_target_corners->points[FBK_REAR_LEFT_CORNER]));

   /* Shifting rotated corner points depending on vehicle position */
   p_target_corners->points[FBK_FRONT_LEFT_CORNER] =
      Vector_2d_Alg_Add(p_target_vcs_pos, &(p_target_corners->points[FBK_FRONT_LEFT_CORNER]));
   p_target_corners->points[FBK_FRONT_RIGHT_CORNER] =
      Vector_2d_Alg_Add(p_target_vcs_pos, &(p_target_corners->points[FBK_FRONT_RIGHT_CORNER]));
   p_target_corners->points[FBK_REAR_RIGHT_CORNER] =
      Vector_2d_Alg_Add(p_target_vcs_pos, &(p_target_corners->points[FBK_REAR_RIGHT_CORNER]));
   p_target_corners->points[FBK_REAR_LEFT_CORNER] =
      Vector_2d_Alg_Add(p_target_vcs_pos, &(p_target_corners->points[FBK_REAR_LEFT_CORNER]));

   /* Calculation of midpoints between corners on nondiagonal connections */
   p_target_corners->points[FBK_FRONT_MID] =
      Vector_2d_Alg_Middle(&(p_target_corners->points[FBK_FRONT_LEFT_CORNER]), &(p_target_corners->points[FBK_FRONT_RIGHT_CORNER]));
   p_target_corners->points[FBK_RIGHT_MID] =
      Vector_2d_Alg_Middle(&(p_target_corners->points[FBK_FRONT_RIGHT_CORNER]), &(p_target_corners->points[FBK_REAR_RIGHT_CORNER]));
   p_target_corners->points[FBK_REAR_MID] =
      Vector_2d_Alg_Middle(&(p_target_corners->points[FBK_REAR_RIGHT_CORNER]), &(p_target_corners->points[FBK_REAR_LEFT_CORNER]));
   p_target_corners->points[FBK_LEFT_MID] =
      Vector_2d_Alg_Middle(&(p_target_corners->points[FBK_REAR_LEFT_CORNER]), &(p_target_corners->points[FBK_FRONT_LEFT_CORNER]));
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
Fbk_Reference_Position_T Fbk_Get_Opposite_Point(const Fbk_Reference_Position_T point_index)
{
   Fbk_Reference_Position_T opposite_point = point_index;
   uint8_t point_index_uint                = (uint8_t) point_index;

   switch (point_index_uint)
   {
      case 0u: // FBK_FRONT_LEFT_CORNER
         opposite_point = FBK_FRONT_RIGHT_CORNER;
         break;
      case 1u: // FBK_FRONT_MID
         opposite_point = FBK_FRONT_MID;
         break;
      case 2u: // FBK_FRONT_RIGHT_CORNER
         opposite_point = FBK_FRONT_LEFT_CORNER;
         break;
      case 3u: // FBK_RIGHT_MID
         opposite_point = FBK_RIGHT_MID;
         break;
      case 4u: // FBK_REAR_RIGHT_CORNER
         opposite_point = FBK_REAR_LEFT_CORNER;
         break;
      case 5u: // FBK_REAR_MID
         opposite_point = FBK_REAR_MID;
         break;
      case 6u: // FBK_REAR_LEFT_CORNER
         opposite_point = FBK_REAR_RIGHT_CORNER;
         break;
      case 7u: // FBK_LEFT_MID
         opposite_point = FBK_LEFT_MID;
         break;
      default:
         assert(0);
         break;
   }
   return opposite_point;
}
