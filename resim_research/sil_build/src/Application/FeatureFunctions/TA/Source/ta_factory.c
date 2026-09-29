/**
 * @file ta_factory.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module makes the global variables accessible to other modules and implements
 * the functions declared in its header.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_factory.h"
#include "fbk_circular_shape_calculator.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "fbk_traj_predictor_t.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "ta_types.h"
#include <assert.h>

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Reset_Trajectory(Fbk_Trajectory_T *p_ta_trajectory, const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Iterator */
   uint8_t pred_step;

   /* Asserts */
   assert(NULL != p_ta_trajectory);
   assert(NULL != p_ta_cal);

   /* Resets the ego trajectory properties */
   p_ta_trajectory->f_trajectory_valid = FBK_FALSE;
   p_ta_trajectory->n_prediction_steps = p_ta_cal->k_ta_prediction_steps_max;

   for (pred_step = FBK_ZERO_INT; pred_step < p_ta_trajectory->n_prediction_steps; pred_step++)
   {
      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&(p_ta_trajectory->waypoint[pred_step]));
   }
}

void Ta_Create_Danger_Zones(Fbk_Field_Of_Interest_T *p_danger_zone_left,
                            Fbk_Field_Of_Interest_T *p_danger_zone_right,
                            const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Iterator */
   uint8_t i_point;

   /* Asserts */
   assert(NULL != p_danger_zone_left);
   assert(NULL != p_danger_zone_right);
   assert(NULL != p_ta_cal);

   /* Example for size =  4 */
   /* set up danger zones */
   /*  x = long, y = lat  */
   /*  0 ---- 1 0 ---- 1  */
   /*  |      | |      |  */
   /*  | left | |right |  */
   /*  |      | |      |  */
   /*  3 ---- 2 3 ---- 2  */
   /*        [ego]        */

   p_danger_zone_left->size  = p_ta_cal->k_fta_danger_zone_point_size;
   p_danger_zone_right->size = p_ta_cal->k_fta_danger_zone_point_size;

   /* Zone constructed from k_fta_danger_zone_point_size number of points */
   for (i_point = FBK_ZERO_INT; i_point < p_ta_cal->k_fta_danger_zone_point_size; i_point++)
   {
      p_danger_zone_left->points[i_point] = Create_2d_Vector_Coordinates(p_ta_cal->k_fta_danger_zone_left_long[i_point],
                                                                         p_ta_cal->k_fta_danger_zone_left_lat[i_point]);

      p_danger_zone_right->points[i_point] = Create_2d_Vector_Coordinates(p_ta_cal->k_fta_danger_zone_right_long[i_point],
                                                                          p_ta_cal->k_fta_danger_zone_right_lat[i_point]);
   }
}

void Ta_Create_Info_Zones(Fbk_Field_Of_Interest_T *p_info_zone_left,
                          Fbk_Field_Of_Interest_T *p_info_zone_right,
                          const Ta_Core_Calibration_T *p_ta_cal,
                          const boolean_T f_zone_hysteresis)
{
   /* Iterator */
   uint8_t i_point;

   /* Temporary zones */
   Fbk_Field_Of_Interest_T hysteresis_zone_left;
   Fbk_Field_Of_Interest_T hysteresis_zone_right;
   float32_T area_info_zone_left;
   float32_T area_info_zone_right;
   float32_T area_info_hys_zone_left;
   float32_T area_info_hys_zone_right;

   /* Asserts */
   assert(NULL != p_info_zone_left);
   assert(NULL != p_info_zone_right);
   assert(NULL != p_ta_cal);

   /* Example for size =  4 */
   /*   set up info zones   */
   /*   x = long, y = lat   */
   /* 0 ---- 1[ego]0 ---- 1 */
   /* |      |	    |      | */
   /* | left |	    |right | */
   /* |      |	    |      | */
   /* 3 ---- 2     3 ---- 2 */

   p_info_zone_left->size     = p_ta_cal->k_rta_info_zone_point_size;
   p_info_zone_right->size    = p_ta_cal->k_rta_info_zone_point_size;
   hysteresis_zone_left.size  = p_ta_cal->k_rta_info_zone_point_size;
   hysteresis_zone_right.size = p_ta_cal->k_rta_info_zone_point_size;

   /* Zone constructed from k_rta_info_zone_point_size number of points */
   for (i_point = FBK_ZERO_INT; i_point < p_ta_cal->k_rta_info_zone_point_size; i_point++)
   {
      /* Regular zone */
      float32_T rta_info_zone_left_long_point  = p_ta_cal->k_rta_info_zone_left_long[i_point];
      float32_T rta_info_zone_left_lat_point   = p_ta_cal->k_rta_info_zone_left_lat[i_point];
      float32_T rta_info_zone_right_long_point = p_ta_cal->k_rta_info_zone_right_long[i_point];
      float32_T rta_info_zone_right_lat_point  = p_ta_cal->k_rta_info_zone_right_lat[i_point];

      /* Set vectors for left and right zones */
      p_info_zone_left->points[i_point] = Create_2d_Vector_Coordinates(rta_info_zone_left_long_point, rta_info_zone_left_lat_point);
      p_info_zone_right->points[i_point] = Create_2d_Vector_Coordinates(rta_info_zone_right_long_point, rta_info_zone_right_lat_point);

      if (Fbk_Is_True(f_zone_hysteresis))
      {
         /* Hysteresis zone */
         float32_T rta_info_hys_zone_left_long_point =
            rta_info_zone_left_long_point + p_ta_cal->k_rta_info_zone_left_long_hys[i_point];
         float32_T rta_info_hys_zone_left_lat_point = rta_info_zone_left_lat_point + p_ta_cal->k_rta_info_zone_left_lat_hys[i_point];
         float32_T rta_info_hys_zone_right_long_point =
            rta_info_zone_right_long_point + p_ta_cal->k_rta_info_zone_right_long_hys[i_point];
         float32_T rta_info_hys_zone_right_lat_point =
            rta_info_zone_right_lat_point + p_ta_cal->k_rta_info_zone_right_lat_hys[i_point];

         /* Set vectors for left and right hysteresis zones */
         hysteresis_zone_left.points[i_point] =
            Create_2d_Vector_Coordinates(rta_info_hys_zone_left_long_point, rta_info_hys_zone_left_lat_point);
         hysteresis_zone_right.points[i_point] =
            Create_2d_Vector_Coordinates(rta_info_hys_zone_right_long_point, rta_info_hys_zone_right_lat_point);
      }
   }

   if (Fbk_Is_True(f_zone_hysteresis))
   {
      /* Compare the areas for regular and hysteresis zone on the left side */
      area_info_zone_left     = Fbk_Get_Area_Field_Of_Interest(p_info_zone_left);
      area_info_hys_zone_left = Fbk_Get_Area_Field_Of_Interest(&hysteresis_zone_left);

      if (area_info_hys_zone_left > area_info_zone_left)
      {
         /* Only set hysteresis zone if area is larger than regular zone */
         *p_info_zone_left = hysteresis_zone_left;
      }

      /* Compare the areas for regular and hysteresis zone on the right side */
      area_info_zone_right     = Fbk_Get_Area_Field_Of_Interest(p_info_zone_right);
      area_info_hys_zone_right = Fbk_Get_Area_Field_Of_Interest(&hysteresis_zone_right);

      if (area_info_hys_zone_right > area_info_zone_right)
      {
         /* Only set hysteresis zone if area is larger than regular zone */
         *p_info_zone_right = hysteresis_zone_right;
      }
   }
}

void Ta_Create_Wing_Zones(Fbk_Field_Of_Interest_T *p_wing_zone_left,
                          Fbk_Field_Of_Interest_T *p_wing_zone_right,
                          const Ta_Core_Calibration_T *p_ta_cal,
                          const boolean_T f_zone_hysteresis)
{
   /* Iterator */
   uint8_t i_point;

   /* Temporary zones */
   Fbk_Field_Of_Interest_T hysteresis_zone_left;
   Fbk_Field_Of_Interest_T hysteresis_zone_right;
   float32_T area_wing_zone_left;
   float32_T area_wing_zone_right;
   float32_T area_wing_hys_zone_left;
   float32_T area_wing_hys_zone_right;

   /* Asserts */
   assert(NULL != p_wing_zone_left);
   assert(NULL != p_wing_zone_right);
   assert(NULL != p_ta_cal);

   /* Example for size =  4 */
   /*   set up wing zones   */
   /*   x = long, y = lat   */
   /* 0 ---- 1     0 ---- 1 */
   /* |      |[ego]|      | */
   /* | left |	    |right | */
   /* |      |	    |      | */
   /* 3 ---- 2     3 ---- 2 */

   p_wing_zone_left->size     = p_ta_cal->k_rta_wing_zone_point_size;
   p_wing_zone_right->size    = p_ta_cal->k_rta_wing_zone_point_size;
   hysteresis_zone_left.size  = p_ta_cal->k_rta_info_zone_point_size;
   hysteresis_zone_right.size = p_ta_cal->k_rta_info_zone_point_size;

   /* Zone constructed from k_rta_wing_zone_point_size number of points */
   for (i_point = FBK_ZERO_INT; i_point < p_ta_cal->k_rta_wing_zone_point_size; i_point++)
   {
      /* Regular zone */
      float32_T rta_wing_zone_left_long_point  = p_ta_cal->k_rta_wing_zone_left_long[i_point];
      float32_T rta_wing_zone_left_lat_point   = p_ta_cal->k_rta_wing_zone_left_lat[i_point];
      float32_T rta_wing_zone_right_long_point = p_ta_cal->k_rta_wing_zone_right_long[i_point];
      float32_T rta_wing_zone_right_lat_point  = p_ta_cal->k_rta_wing_zone_right_lat[i_point];

      /* Set vectors for left and right zones */
      p_wing_zone_left->points[i_point] = Create_2d_Vector_Coordinates(rta_wing_zone_left_long_point, rta_wing_zone_left_lat_point);
      p_wing_zone_right->points[i_point] = Create_2d_Vector_Coordinates(rta_wing_zone_right_long_point, rta_wing_zone_right_lat_point);

      if (Fbk_Is_True(f_zone_hysteresis))
      {
         /* Add hysteresis values */
         float32_T rta_wing_hys_zone_left_long_point =
            rta_wing_zone_left_long_point + p_ta_cal->k_rta_wing_zone_left_long_hys[i_point];
         float32_T rta_wing_hys_zone_left_lat_point = rta_wing_zone_left_lat_point + p_ta_cal->k_rta_wing_zone_left_lat_hys[i_point];
         float32_T rta_wing_hys_zone_right_long_point =
            rta_wing_zone_right_long_point + p_ta_cal->k_rta_wing_zone_right_long_hys[i_point];
         float32_T rta_wing_hys_zone_right_lat_point =
            rta_wing_zone_right_lat_point + p_ta_cal->k_rta_wing_zone_right_lat_hys[i_point];

         /* Set vectors for left and right hysteresis zones */
         hysteresis_zone_left.points[i_point] =
            Create_2d_Vector_Coordinates(rta_wing_hys_zone_left_long_point, rta_wing_hys_zone_left_lat_point);
         hysteresis_zone_right.points[i_point] =
            Create_2d_Vector_Coordinates(rta_wing_hys_zone_right_long_point, rta_wing_hys_zone_right_lat_point);
      }
   }

   if (Fbk_Is_True(f_zone_hysteresis))
   {
      /* Compare the areas for regular and hysteresis zone on the left side */
      area_wing_zone_left     = Fbk_Get_Area_Field_Of_Interest(p_wing_zone_left);
      area_wing_hys_zone_left = Fbk_Get_Area_Field_Of_Interest(&hysteresis_zone_left);

      if (area_wing_hys_zone_left > area_wing_zone_left)
      {
         /* Only set hysteresis zone if area is larger than regular zone */
         *p_wing_zone_left = hysteresis_zone_left;
      }

      /* Compare the areas for regular and hysteresis zone on the right side */
      area_wing_zone_right     = Fbk_Get_Area_Field_Of_Interest(p_wing_zone_right);
      area_wing_hys_zone_right = Fbk_Get_Area_Field_Of_Interest(&hysteresis_zone_right);

      if (area_wing_hys_zone_right > area_wing_zone_right)
      {
         /* Only set hysteresis zone if area is larger than regular zone */
         *p_wing_zone_right = hysteresis_zone_right;
      }
   }
}

void Ta_Fill_Fbk_Predict_Ego_Struct(Fbk_Ego_Predict_Data_T *p_fbk_ego_data,
                                    const Ta_Persistent_T *p_ta_persistent,
                                    const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Assert */
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

   /* Fill ego properties */
   p_fbk_ego_data->ego_acceleration_weight                = p_ta_cal->k_ta_ego_acceleration_weight;
   p_fbk_ego_data->ego_shape_gain_fixed                   = p_ta_cal->k_ta_ego_shape_gain_fixed;
   p_fbk_ego_data->ego_circle_offset                      = p_ta_cal->k_ta_ego_circle_offset;
   p_fbk_ego_data->ego_circle_host_length_factor          = p_ta_cal->k_ta_ego_circle_host_length_factor;
   p_fbk_ego_data->ego_pred_const_velocity_pred_steps_min = p_ta_cal->k_ta_ego_pred_const_velocity_pred_steps_min;
   p_fbk_ego_data->ego_deceleration_weight                = p_ta_cal->k_ta_ego_deceleration_weight;
   p_fbk_ego_data->ego_yaw_angle_to_last_straight_section = p_ta_persistent->ta_ego_yaw_angle_to_last_straight_section;
   p_fbk_ego_data->ego_max_pred_yaw_angle                 = p_ta_cal->k_ta_ego_max_pred_yaw_angle;
   p_fbk_ego_data->ego_shape_gain_per_pred_step           = p_ta_cal->k_ta_ego_shape_gain_per_pred_step;
   p_fbk_ego_data->pred_step_dt                           = p_ta_persistent->ta_pred_step_dt;
   p_fbk_ego_data->prediction_steps_max                   = p_ta_cal->k_ta_prediction_steps_max;

   p_fbk_ego_data->acc_weight_depend_on_alert_lvl = FBK_FALSE;

   if ((TA_ALERT_STATE_LEVEL_4 == p_ta_persistent->ta_side_alert_prev_cycle[FBK_SIDE_LEFT])
       || (TA_ALERT_STATE_LEVEL_4 == p_ta_persistent->ta_side_alert_prev_cycle[FBK_SIDE_RIGHT]))
   {
      /* Host vehicle is presumably decelerating due to TA brake request.
       * Predict using constant velocity model after applicable number of prediction steps. */
      p_fbk_ego_data->acc_weight_depend_on_alert_lvl = FBK_TRUE;
   }
}

void Ta_Fill_Fbk_Predict_Obj_Struct(Fbk_Object_Predict_Data_T *p_fbk_obj_data,
                                    const Ta_Persistent_T *p_ta_persistent,
                                    const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Fill object properties */
   p_fbk_obj_data->obj_pred_speed_min           = p_ta_cal->k_ta_obj_pred_speed_min;
   p_fbk_obj_data->obj_shape_gain_fixed         = p_ta_cal->k_ta_obj_shape_gain_fixed;
   p_fbk_obj_data->obj_shape_gain_per_pred_step = p_ta_cal->k_ta_obj_shape_gain_per_pred_step;
   p_fbk_obj_data->pred_step_dt                 = p_ta_persistent->ta_pred_step_dt;
}
