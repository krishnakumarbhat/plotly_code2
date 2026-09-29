/**
 * @file fbk_ego_traj_predictor.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements functions related to predicting the ego waypoints around the objects coordinates.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_ego_traj_predictor.h"
#include "fbk_circular_shape_calculator.h"
#include "fbk_macros.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

/* EXPORTED DEFINES FOR CONSTANTS --------------------------------------------*/

/* FBK maximum curve radius that is still considered driving a curve */
#define FBK_MAX_HOST_CURVE_RADIUS (300.0f)
/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/
/**
 * @brief Calculates the host curve radius based on current speed and yaw rate.
 *
 * @return host curve radius (zero for driving straight, positive for right turn, negative for left turn)
 *
 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4224}
 * @verification{Create a test with a yawrate greater than zero. Only then a non-default host curve radius shall be returned.}
 */
static float32_T Fbk_Get_Host_Curve_Radius(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */);

/**
 * @brief Initializes properties of host circles once.
 *
 * @return void
 *
 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4226}
 * @verification{For valid input parameters host circle properties shall be filled.}
 */
static void Fbk_Init_Ego_Circle_Props(Fbk_Ego_Traj_Predictor_Instance_T *p_instance /**< TA Context */,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                      const Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data */,
                                      Fbk_Host_Circles_Props_T *p_host_circles_props /**< Host circle properties */);


/**
 * @brief Calculates the predicted ego waypoint.
 *
 *  - It calculates the longitudinal and lateral velocity using the predicted ego yaw angle.
 *  - Calculates the ego waypoint (relative to origin of ego VCS at current time)
 *  - Calculates center positions of the ego circles
 *
 * @return The predicted ego waypoint for the given prediction step
 *
 * @SRS{SF-303}
 * @SAE{SF-2552}
 * @SDD{SF-4225}
 * @verification{The integral predicted velocity shall be greater than zero so that a valid waypoint can be created. Only then non
 * default values are set for the waypoint.}
 */
static Fbk_Waypoint_with_Circle_Centers_T
Fbk_Get_Predicted_Ego_Waypoint(Fbk_Ego_Traj_Predictor_Instance_T *p_instance /**< TA Context */,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                               const Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data */,
                               const uint8_t prediction_step /**< current prediction step */);

/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/
static float32_T Fbk_Get_Host_Curve_Radius(const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   float32_T curve_radius = FBK_ZERO_F;

   /* Assert */
   assert(NULL != p_vehicle_data);

   /* Calculate host vehicle curve radius */
   if (Fbk_Abs_F(p_vehicle_data->yawrate) > EPSILON)
   {
      curve_radius = Fbk_Abs_F(p_vehicle_data->host_speed) / p_vehicle_data->yawrate;
   }

   return curve_radius;
}

static void Fbk_Init_Ego_Circle_Props(Fbk_Ego_Traj_Predictor_Instance_T *p_instance,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Fbk_Ego_Predict_Data_T *p_fbk_ego_data,
                                      Fbk_Host_Circles_Props_T *p_host_circles_props)
{
   /* Asserts */
   assert(NULL != p_instance);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_fbk_ego_data);
   assert(NULL != p_host_circles_props);

   if ((Fbk_Is_False(p_instance->F_Host_Circle_Props_Initialized)) && Fbk_Not_Equal_F(p_vehicle_data->host_length, FBK_ZERO_F)
       && Fbk_Not_Equal_F(p_vehicle_data->host_width, FBK_ZERO_F))
   {
      /* Calculate circle center point x coordinates relative to ego VCS O: */
      p_host_circles_props->offsets =
         Fbk_Calculate_Ego_Circle_Center_Offsets(p_vehicle_data->host_length, p_vehicle_data->host_width, p_fbk_ego_data);

      p_host_circles_props->radius = (p_vehicle_data->host_width / 2.0f) * p_fbk_ego_data->ego_shape_gain_fixed;

      p_instance->F_Host_Circle_Props_Initialized = FBK_TRUE;
   }
}

static Fbk_Waypoint_with_Circle_Centers_T Fbk_Get_Predicted_Ego_Waypoint(Fbk_Ego_Traj_Predictor_Instance_T *p_instance,
                                                                         const Fbk_Vehicle_Data_T *p_vehicle_data,
                                                                         const Fbk_Ego_Predict_Data_T *p_fbk_ego_data,
                                                                         const uint8_t prediction_step)
{
   /* Initialize variables */
   Angle_T predicted_host_yaw_angle  = Create_Angle(FBK_ZERO_F);
   float32_T predicted_host_long_pos = FBK_ZERO_F;
   float32_T predicted_host_lat_pos  = FBK_ZERO_F;
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_result;
   Fbk_Host_Circles_Props_T *p_host_circles_props;
   float32_T delta_predicted_velocity;
   float32_T delta_predicted_arc_length;
   float32_T host_acceleration_long;
   boolean_T f_radius_min;
   boolean_T f_radius_max;

   /* Asserts */
   assert(NULL != p_instance);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_fbk_ego_data);

   /* Prepare result (empty waypoint) */
   Fbk_Init_Waypoint_with_Circle_Centers_Structure(&waypoint_with_circle_centers_result);
   p_host_circles_props = &p_instance->Host_Circles_Props;
   Fbk_Init_Ego_Circle_Props(p_instance, p_vehicle_data, p_fbk_ego_data, p_host_circles_props);

   /* Integrate position changes */
   if (FBK_ZERO_UINT == prediction_step)
   {
      /* Reset those values at the beginning of each prediction run. */
      p_instance->Integral_Predicted_Arc_Length            = FBK_ZERO_F;
      p_instance->Integral_Predicted_Velocity              = p_vehicle_data->host_speed;
      p_instance->Host_Curve_Radius                        = Fbk_Get_Host_Curve_Radius(p_vehicle_data);
      waypoint_with_circle_centers_result.f_waypoint_valid = FBK_TRUE;
   }
   else
   {
      /* Calculate weighted host vehicle acceleration value for positive or negative acceleration */
      if (p_vehicle_data->long_acc >= FBK_ZERO_F)
      {
         host_acceleration_long = p_vehicle_data->long_acc * p_fbk_ego_data->ego_acceleration_weight;
      }
      else
      {
         if ((p_fbk_ego_data->acc_weight_depend_on_alert_lvl)
             && (prediction_step >= p_fbk_ego_data->ego_pred_const_velocity_pred_steps_min))
         {
            /* Host vehicle is presumably decelerating due to TA brake request.
             * Predict using constant velocity model after applicable number of prediction steps. */
            host_acceleration_long = FBK_ZERO_F;
         }
         else
         {
            host_acceleration_long = p_vehicle_data->long_acc * p_fbk_ego_data->ego_deceleration_weight;
         }
      }

      /* Integrate velocity changes */
      delta_predicted_velocity                = host_acceleration_long * p_fbk_ego_data->pred_step_dt;
      p_instance->Integral_Predicted_Velocity = p_instance->Integral_Predicted_Velocity + delta_predicted_velocity;
   }

   /* Only change predicted position if predicted velocity is still positive */
   if ((p_instance->Integral_Predicted_Velocity > EPSILON) && (FBK_ZERO_UINT != prediction_step))
   {
      /* Calculate traveled arc length in this prediction step based on the integrated predicted velocity */
      delta_predicted_arc_length = p_instance->Integral_Predicted_Velocity * p_fbk_ego_data->pred_step_dt;

      /* Add delta arc length, the distance traveled in this prediction step, to integrated arc length */
      p_instance->Integral_Predicted_Arc_Length = p_instance->Integral_Predicted_Arc_Length + delta_predicted_arc_length;

      f_radius_min = (boolean_T) (Fbk_Abs_F(p_instance->Host_Curve_Radius) > EPSILON);
      f_radius_max = (boolean_T) (Fbk_Abs_F(p_instance->Host_Curve_Radius) < FBK_MAX_HOST_CURVE_RADIUS);
      if (f_radius_min && f_radius_max)
      {
         /* Calculate ego yaw angle for current prediction step based on integrated arc length and curve radius */
         predicted_host_yaw_angle = Create_Angle(p_instance->Integral_Predicted_Arc_Length / p_instance->Host_Curve_Radius);

         /* Calculate predicted positions based on predicted yaw angle and host curve radius */
         predicted_host_long_pos = predicted_host_yaw_angle.sin * p_instance->Host_Curve_Radius;
         predicted_host_lat_pos  = p_instance->Host_Curve_Radius - (predicted_host_yaw_angle.cos * p_instance->Host_Curve_Radius);
      }
      else
      {
         /* Host is predicted to drive approximately straight */
         predicted_host_long_pos = p_instance->Integral_Predicted_Arc_Length;
      }

      /* Continue host prediction only if the predicted yaw angle to the last straight section is below the threshold
       * to reflect prediction uncertainty for tight and fast curves */
      if (Fbk_Abs_F(predicted_host_yaw_angle.angle + p_fbk_ego_data->ego_yaw_angle_to_last_straight_section)
          <= p_fbk_ego_data->ego_max_pred_yaw_angle)
      {
         /* Set validity of current waypoint */
         waypoint_with_circle_centers_result.f_waypoint_valid = FBK_TRUE;
      }
   }
   else
   {
      /* No position change expected for this timestep. First prediction step or ego comes to a standstill. */
   }

   if (Fbk_Is_True(waypoint_with_circle_centers_result.f_waypoint_valid))
   {
      /* Calculate ego waypoint (relative to origin of ego VCS minus the offest to the middle of the ego vehicle at current time) */
      waypoint_with_circle_centers_result.waypoint_coordinates.x = p_vehicle_data->rear_axle_position + predicted_host_long_pos;
      waypoint_with_circle_centers_result.waypoint_coordinates.y = predicted_host_lat_pos;
      waypoint_with_circle_centers_result.waypoint_yaw_angle     = predicted_host_yaw_angle;
      waypoint_with_circle_centers_result.waypoint_speed         = p_instance->Integral_Predicted_Velocity;

      /* Calculate center positions of the ego circles */
      Fbk_Fill_Circle_Center_Coordinates(&waypoint_with_circle_centers_result,
                                         p_host_circles_props->offsets.offset_front_x - p_vehicle_data->rear_axle_position,
                                         p_host_circles_props->offsets.offset_middle_x - p_vehicle_data->rear_axle_position,
                                         p_host_circles_props->offsets.offset_rear_x - p_vehicle_data->rear_axle_position);

      /* Calculate waypoint circle radius */
      waypoint_with_circle_centers_result.circle_radius = Fbk_Get_Circle_Radius(prediction_step, p_host_circles_props->radius,
                                                                                p_fbk_ego_data->ego_shape_gain_fixed,
                                                                                p_fbk_ego_data->ego_shape_gain_per_pred_step);
   }

   return waypoint_with_circle_centers_result;
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Predict_Ego_Trajectory(Fbk_Ego_Traj_Predictor_Instance_T *p_instance,
                                Fbk_Trajectory_T *p_ego_trajectory /**< TA Ego Trajectory */,
                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                const Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data */)
{
   /* Iterator */
   uint8_t pred_step;

   /* Asserts */
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_instance);
   assert(NULL != p_fbk_ego_data);
   assert(p_ego_trajectory->n_prediction_steps <= FBK_MAX_PREDICTION_STEPS);

   /* Calculate all prediction step waypoints and fill ego_trajectory */
   for (pred_step = FBK_ZERO_INT; pred_step < p_ego_trajectory->n_prediction_steps; pred_step++)
   {
      p_ego_trajectory->waypoint[pred_step] = Fbk_Get_Predicted_Ego_Waypoint(p_instance, p_vehicle_data, p_fbk_ego_data, pred_step);

      if (Fbk_Is_False(p_ego_trajectory->waypoint[pred_step].f_waypoint_valid))
      {
         /* Break the loop if last waypoint was invalid. */
         break;
      }
   }

   if (pred_step > FBK_ZERO_UINT)
   {
      /* Set trajectory validity flag */
      p_ego_trajectory->f_trajectory_valid = FBK_TRUE;
   }
}
