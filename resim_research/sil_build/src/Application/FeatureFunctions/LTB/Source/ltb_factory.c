/**
 * @file ltb_factory.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module makes the global variables accessible to other modules and implements
 * the functions declared in its header.
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ltb_factory.h"
#include "fbk_circular_shape_calculator.h"
#include "fbk_macros.h"
#include "ltb_types.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

void Ltb_Reset_Trajectory(Fbk_Trajectory_T *p_ltb_trajectory, const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Iterator */
   uint8_t pred_step;

   /* Asserts */
   assert(NULL != p_ltb_trajectory);
   assert(NULL != p_ltb_cal);

   /* Resets the ego trajectory properties */
   p_ltb_trajectory->f_trajectory_valid = FBK_FALSE;
   p_ltb_trajectory->n_prediction_steps = p_ltb_cal->k_ltb_prediction_steps_max;

   for (pred_step = FBK_ZERO_INT; pred_step < p_ltb_trajectory->n_prediction_steps; pred_step++)
   {
      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&(p_ltb_trajectory->waypoint[pred_step]));
   }
}

void Ltb_Create_Zone(Fbk_Field_Of_Interest_T *p_zone_left, Fbk_Field_Of_Interest_T *p_zone_right, const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Asserts */
   assert(NULL != p_zone_left);
   assert(NULL != p_zone_right);
   assert(NULL != p_ltb_cal);

   /*  Example for size = 4  */
   /*      set up zones      */
   /*    x = long, y = lat   */
   /* 1 ---- 0[ego] 0 ---- 1 */
   /* |      |	    |      | */
   /* | left |	    |right | */
   /* |      |	    |      | */
   /* 2 ---- 3      3 ---- 2 */

   p_zone_left->size  = LTB_NUMBER_OF_ZONE_POINTS;
   p_zone_right->size = LTB_NUMBER_OF_ZONE_POINTS;

   /* Zone constructed from ltb_zone parameters */
   p_zone_left->points[0] = Create_2d_Vector_Coordinates(FBK_ZERO_F, FBK_ZERO_F);
   p_zone_left->points[1] = Create_2d_Vector_Coordinates(FBK_ZERO_F, p_ltb_cal->k_ltb_zone_width);
   p_zone_left->points[2] = Create_2d_Vector_Coordinates(-p_ltb_cal->k_ltb_zone_length, p_ltb_cal->k_ltb_zone_width);
   p_zone_left->points[3] = Create_2d_Vector_Coordinates(-p_ltb_cal->k_ltb_zone_length, FBK_ZERO_F);

   p_zone_right->points[0] = Create_2d_Vector_Coordinates(FBK_ZERO_F, FBK_ZERO_F);
   p_zone_right->points[1] = Create_2d_Vector_Coordinates(FBK_ZERO_F, -p_ltb_cal->k_ltb_zone_width);
   p_zone_right->points[2] = Create_2d_Vector_Coordinates(-p_ltb_cal->k_ltb_zone_length, -p_ltb_cal->k_ltb_zone_width);
   p_zone_right->points[3] = Create_2d_Vector_Coordinates(-p_ltb_cal->k_ltb_zone_length, FBK_ZERO_F);
}
