/**
 * @file fbk_obj_traj_predictor.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module calculates the object trajectory prediction with the Kinematic Motion Model.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_obj_traj_predictor.h"
#include "fbk_circular_shape_calculator.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_traj_predictor_t.h"
#include "ml_angle.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

/*============================================================================*\
 * Local function declarations
\*============================================================================*/

/**
 * @brief Calculates the waypoint of the predicted object trajectory.
 *
 * The result way point is calculated using the objects Kinematic Motion Model
 *
 * @return waypoint for given prediction step
 *
 * @SRS{SF-301}
 * @SAE{}
 * @SDD{SF-4238}
 * @verification{Create tests where the validity flag of objects waypoints is set on True. Only then further attributes are allowed
 to be returned.}

 */
static Fbk_Waypoint_with_Circle_Centers_T
Fbk_Get_Predicted_Object_Waypoint(const Fbk_Object_Data_T *p_object_tracker_data /**< object trajectory */,
                                  const Fbk_Object_Predict_Data_T *p_fbk_obj_data /**< object data */,
                                  const uint8_t prediction_step /**< prediction step */);

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Predict_Obj_Trajectory(Fbk_Trajectory_T *p_obj_trajectory,
                                const Fbk_Object_Data_T *p_object_tracker_data,
                                const Fbk_Object_Predict_Data_T *p_fbk_obj_data)
{
   /* Iterator variables */
   uint8_t prev_pred_step = FBK_ZERO_INT;
   uint8_t pred_step;

   /* Last valid step gets saved */
   uint8_t last_valid_step = FBK_ZERO_INT;

   /* Asserts */
   assert(NULL != p_object_tracker_data);
   assert(NULL != p_fbk_obj_data);
   assert(p_obj_trajectory->n_prediction_steps <= FBK_MAX_PREDICTION_STEPS);

   /* Iterate through all prediction steps */
   for (pred_step = FBK_ZERO_INT; pred_step < p_obj_trajectory->n_prediction_steps; pred_step++)
   {
      /* Check if previous step was the last valid step */
      if (FBK_ZERO_UINT == (last_valid_step - prev_pred_step))
      {
         /* Calculate obj waypoint positions for current prediction step */
         p_obj_trajectory->waypoint[pred_step] = Fbk_Get_Predicted_Object_Waypoint(p_object_tracker_data, p_fbk_obj_data, pred_step);
      }
      else
      {
         /* Previous step was invalid -> current waypoint will be invalid as well */
      }

      if (Fbk_Is_True(p_obj_trajectory->waypoint[pred_step].f_waypoint_valid))
      {
         /* Prediction step is valid. Save it as last valid step. */
         last_valid_step = pred_step;
      }
      else
      {
         /* Current waypoint is invalid. Get last valid object position for this prediction step. */
         p_obj_trajectory->waypoint[pred_step] = p_obj_trajectory->waypoint[last_valid_step];
      }

      /* Save current prediction step as previous prediction step for next cycle */
      prev_pred_step = pred_step;
   }

   /* Check that at least one valid timestep was reached */
   if (last_valid_step > FBK_ZERO_UINT)
   {
      /* Set trajectory validity flag */
      p_obj_trajectory->f_trajectory_valid = FBK_TRUE;
   }
}

/*============================================================================*\
 * Local function definitions
\*============================================================================*/

static Fbk_Waypoint_with_Circle_Centers_T Fbk_Get_Predicted_Object_Waypoint(const Fbk_Object_Data_T *p_object_tracker_data,
                                                                            const Fbk_Object_Predict_Data_T *p_fbk_obj_data,
                                                                            const uint8_t prediction_step)
{
   /* resulting object waypoint */
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_result;

   /* variables for the offsets of the circles that approximate the shape of the object */
   Fbk_Circle_Center_Offset_T offsets;

   /* predicted velocities */
   Vector_2d_T predicted_vel;

   /* calculate p_t and p_t^2 based on dT and time step */
   float32_T p_t   = (float32_T) prediction_step * p_fbk_obj_data->pred_step_dt;
   float32_T p_t_2 = p_t * p_t;

   /* Asserts */
   assert(NULL != p_object_tracker_data);
   assert(NULL != p_fbk_obj_data);

   /* Initialize waypoint struct */
   Fbk_Init_Waypoint_with_Circle_Centers_Structure(&waypoint_with_circle_centers_result);

   /*
    *   object velocity model:
    *   v_t+1 = V_t + a_t*dT    so object's velocity in a specific prediction step would be:
    *   v_step = v_0 + a_t*(dT*step)
    *          = v_0 + a_t*p_t
    *
    *   where p_t = prediction_step * dT
    */
   predicted_vel.x = p_object_tracker_data->vcs_vel.x + (p_object_tracker_data->vcs_accel.x * p_t);
   predicted_vel.y = p_object_tracker_data->vcs_vel.y + (p_object_tracker_data->vcs_accel.y * p_t);

   /* Set waypoint speed based on predicted object velocity */
   waypoint_with_circle_centers_result.waypoint_speed = Vector_2d_Alg_Abs(&predicted_vel);

   /* Check for both axes if the sign of current velocity and predicted velocity changes */
   if (((p_object_tracker_data->vcs_vel.x * predicted_vel.x) < FBK_ZERO_F)
       || ((p_object_tracker_data->vcs_vel.y * predicted_vel.y) < FBK_ZERO_F))
   {
      /* Handle case that the object predicted velocity vector for this time step
         is opposite to current velocity and the object model might predict the direction reversing. */
      if (waypoint_with_circle_centers_result.waypoint_speed >= p_fbk_obj_data->obj_pred_speed_min)
      {
         /* Predicted velocity vector length is still above threshold.
            Object is not standing still in this timestep. */
         waypoint_with_circle_centers_result.f_waypoint_valid = FBK_TRUE;
      }
      else
      {
         /* Based on the predicted velocity vector length the object comes close to a standstill in this timestep.
            The validity flag stays false and the waypoint calculation can be skipped. */
      }
   }
   else
   {
      /* waypoint is valid -> set validity flag */
      waypoint_with_circle_centers_result.f_waypoint_valid = FBK_TRUE;
   }

   if (Fbk_Is_True(waypoint_with_circle_centers_result.f_waypoint_valid))
   {
      /*  Object kinematic motion model (with acceleration):
       *
       *   P_t+1 = P_t + v_t*dT + 1/2*a_t*dT^2    so object's position in a specific prediction step would be:
       *
       *   P_step = P_0 + v_t*(dT*step) + 1/2*a_t*(dT*step)^2
       *          = P_0 + v_t*p_t + 1/2*a_t*p_t_2
       *
       *   where p_t = prediction_step * dT
       */

      waypoint_with_circle_centers_result.waypoint_coordinates.x = (p_object_tracker_data->vcs_pos.x)
                                                                   + (p_object_tracker_data->vcs_vel.x * p_t)
                                                                   + (0.5f * p_object_tracker_data->vcs_accel.x * p_t_2);

      waypoint_with_circle_centers_result.waypoint_coordinates.y = (p_object_tracker_data->vcs_pos.y)
                                                                   + (p_object_tracker_data->vcs_vel.y * p_t)
                                                                   + (0.5f * p_object_tracker_data->vcs_accel.y * p_t_2);

      /* Set waypoint yaw angle based on object heading */
      waypoint_with_circle_centers_result.waypoint_yaw_angle = Create_Angle(p_object_tracker_data->vcs_heading);

      offsets = Fbk_Calculate_Obj_Circle_Center_Offsets(p_object_tracker_data->length, p_object_tracker_data->width);

      /* calculate the circle center positions based on the offsets */
      Fbk_Fill_Circle_Center_Coordinates(&waypoint_with_circle_centers_result, offsets.offset_front_x, offsets.offset_middle_x,
                                         offsets.offset_rear_x);

      waypoint_with_circle_centers_result.circle_radius =
         Fbk_Get_Circle_Radius(prediction_step, p_object_tracker_data->width / 2.0f, p_fbk_obj_data->obj_shape_gain_fixed,
                               p_fbk_obj_data->obj_shape_gain_per_pred_step);
   }
   else
   {
      /* Object is predicted to come close to a standstill in this prediction step.
         The trajectory evaluation must use the previous valid position for subsequent steps. */
   }

   return waypoint_with_circle_centers_result;
}
