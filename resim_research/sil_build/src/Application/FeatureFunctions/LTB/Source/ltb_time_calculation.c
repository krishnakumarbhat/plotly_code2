/**
 * @file ltb_time_calculation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module implements logic of its header for determining if
 * two objects have a high probability to collide
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_time_calculation.h"
#include "fbk_macros.h"
#include "ltb_types.h"
#include "ml_angle.h"
#include "ml_angle_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "pa_reuse.h"
#include <assert.h>

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/

/**
 * @brief Retrieves the host speed that we calculated after the dead time period has passed.
 *
 * @return host speed including acceleration induced change during dead time
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53936}
 * @verification{Check whether host speed after dead time of brake is calculated correctly.}
 */
static float32_T Ltb_Get_Host_Speed_After_Dead_Time(const Fbk_Trajectory_T *p_ego_trajectory /**< LTB Trajectory for ego */,
                                                    const Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent data */,
                                                    const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);

/**
 * @brief Checks the overlap of the given ego circle type and any object circle.
 *
 * @return true if circles do overlap
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53937}
 * @verification{Check that a circle center distance smaller than the minimum saftety distance is evaluated as overlap.}
 */
static boolean_T
Ltb_Does_Ego_Circle_Overlap_Any_Object_Circle(const Fbk_Waypoint_with_Circle_Centers_T *p_ego /**< Ego waypoint */,
                                              const Fbk_Waypoint_with_Circle_Centers_T *p_obj /**< Object waypoint */,
                                              const float32_T min_safe_distance /**< Minimum safety distance */);

/**
 * @brief Checks if given waypoints plus safety margin overlap.
 *
 * Checks whether or not Euclidean distance between ego circle centers at given timestep
 * and currently considered object waypoint is smaller than the predefined threshold,
 * meaning the object is in a circle neighborhood small enough for high collision probability.
 *
 * Assumes that data structure waypoints_ego as well as waypoint_curr_obj are filled
 * with valid and relevant data.
 *
 * @return         true if   there is a high collision probability meaning:
 *                           the Euclidean distance between any of the representing circles
 *                           is smaller than the threshold (min_safe_distance) and
 *                 false if  there is a low collision probability meaning:
 *                           the Euclidean distance between any of the representing circles
 *                           is >= than the threshold (min_safe_distance)
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53939}
 * @verification{Create tests in which the euclidian distance between circles is tested. Only when any of them is below a given
 * threshold, true shall be returned.}
 */
static boolean_T Ltb_Is_Critical_Approach(const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */,
                                          const Fbk_Waypoint_with_Circle_Centers_T *p_ego /**< Ego waypoints */,
                                          const Fbk_Waypoint_with_Circle_Centers_T *p_obj /**< Object waypoints */);

/**
 * @brief Calculates the deceleration necessary to avoid a collision while also considering a limiting
 * deceleration gradient or jerk value.
 *
 * @return positive deceleration value that is corrected for a limited deceleration gradient or jerk
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53940}
 * @verification{Create a test where the jerk value is greater than zero. Also the discriminant in the formular needs to be greater
 * than 0. Only in that case a non default jerk value shall be returned.}
 */
static float32_T
Ltb_Get_Gradient_Restricted_Deceleration(const float32_T t_brake /**< [s] Time left to decelerate host vehicle */,
                                         const float32_T v_0 /**< [m/s] Host vehicle velocity at start of deceleration */,
                                         const boolean_T f_enable_gradient /**< Enable flag to activate gradient logic */,
                                         const float32_T j_0 /**< [m/s^3] Jerk value used to ramp up deceleration */);


/*============================================================================*\
 * LOCAL FUNCTIONS
\*============================================================================*/

static float32_T Ltb_Get_Host_Speed_After_Dead_Time(const Fbk_Trajectory_T *p_ego_trajectory,
                                                    const Ltb_Persistent_T *p_ltb_persistent,
                                                    const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Return value */
   float32_T host_speed_after_dead_time = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);

   /* Get the nearest prediction step that matches the calibrated brake dead time
    * and return the previously calculated host velocity for it */
   if (FBK_ZERO_F < p_ltb_persistent->ltb_pred_step_dt)
   {
      float32_T n_pred_steps_float       = p_ltb_cal->k_ltb_brake_dead_time / p_ltb_persistent->ltb_pred_step_dt;
      uint8_t brake_dead_time_pred_steps = (uint8_t) n_pred_steps_float;

      /* Prevent an array access out of bounds */
      if (brake_dead_time_pred_steps < p_ego_trajectory->n_prediction_steps)
      {
         host_speed_after_dead_time = p_ego_trajectory->waypoint[brake_dead_time_pred_steps].waypoint_speed;
      }
   }

   return host_speed_after_dead_time;
}


static boolean_T Ltb_Does_Ego_Circle_Overlap_Any_Object_Circle(const Fbk_Waypoint_with_Circle_Centers_T *p_ego,
                                                               const Fbk_Waypoint_with_Circle_Centers_T *p_obj,
                                                               const float32_T min_safe_distance)
{
   /* Return value */
   boolean_T f_overlap = FBK_FALSE;

   /* Asserts */
   assert(NULL != p_ego);
   assert(NULL != p_obj);

   if (Vector_2d_Alg_Distance(&(p_ego->circle_center_front), &(p_obj->circle_center_front)) < min_safe_distance)
   {
      f_overlap = FBK_TRUE;
   }
   else if (Vector_2d_Alg_Distance(&(p_ego->circle_center_front), &(p_obj->circle_center_middle)) < min_safe_distance)
   {
      f_overlap = FBK_TRUE;
   }
   else if (Vector_2d_Alg_Distance(&(p_ego->circle_center_front), &(p_obj->circle_center_rear)) < min_safe_distance)
   {
      f_overlap = FBK_TRUE;
   }
   else if (Vector_2d_Alg_Distance(&(p_ego->circle_center_rear), &(p_obj->circle_center_front)) < min_safe_distance)
   {
      f_overlap = FBK_TRUE;
   }
   else if (Vector_2d_Alg_Distance(&(p_ego->circle_center_rear), &(p_obj->circle_center_middle)) < min_safe_distance)
   {
      f_overlap = FBK_TRUE;
   }
   else if (Vector_2d_Alg_Distance(&(p_ego->circle_center_rear), &(p_obj->circle_center_rear)) < min_safe_distance)
   {
      f_overlap = FBK_TRUE;
   }
   else
   {
      /* Circle type not valid. */
   }

   return f_overlap;
}

static float32_T Ltb_Get_Gradient_Restricted_Deceleration(const float32_T t_brake,
                                                          const float32_T v_0,
                                                          const boolean_T f_enable_gradient,
                                                          const float32_T j_0)
{
   float32_T gradient_restricted_decel;

   if (Fbk_Is_False(f_enable_gradient))
   {
      /* Gradient has no influence on deceleration value */
      gradient_restricted_decel = v_0 / t_brake;
   }
   else
   {
      /* Gradient or jerk has to be considered and the returned deceleration value needs to be corrected */
      float32_T p_half, q, discriminant;

      /* Quadratic equation
       * x^2 + px + q = 0 */
      p_half       = -1.0f * j_0 * t_brake;
      q            = -2.0f * j_0 * v_0;
      discriminant = (p_half * p_half) - q;

      if (discriminant >= FBK_ZERO_F)
      {
         /* deceleration is first root of quadratic formula */
         gradient_restricted_decel = (-1.0f * p_half) + Fast_Sqrt(discriminant);
      }
      else
      {
         /* no real roots found -> fall back to no-gradient calculation */
         gradient_restricted_decel = v_0 / t_brake;
      }
   }

   return Fbk_Abs_F(gradient_restricted_decel);
}

static boolean_T Ltb_Is_Critical_Approach(const Ltb_Core_Calibration_T *p_ltb_cal,
                                          const Fbk_Waypoint_with_Circle_Centers_T *p_ego,
                                          const Fbk_Waypoint_with_Circle_Centers_T *p_obj)
{
   boolean_T f_critical_approach = FBK_FALSE;

   /*  Here the minimum safe distance between two waypoints is calculated.
    *  It is then used as the threshold to verify a collision probability.
    *  The calculation is based on the sum of evaluating ego and object waypoint circle radii
    *  and an added safety margin defined as calibration parameter.
    */
   const float32_T min_safe_distance =
      (p_ltb_cal->k_ltb_critical_approach_min_safe_distance + p_ego->circle_radius + p_obj->circle_radius);

   /* Calculate difference between yaw angles for this prediction step. */
   const float32_T approach_angle_diff = Fbk_Abs_F(Angle_Diff(&p_ego->waypoint_yaw_angle, &p_obj->waypoint_yaw_angle).angle);

   /* Asserts */
   assert(NULL != p_ltb_cal);
   assert(NULL != p_ego);
   assert(NULL != p_obj);

   /* Preliminary check to verify that the collision angle is not too shallow. */
   if (approach_angle_diff >= p_ltb_cal->k_ltb_critical_approach_angle_diff_min)
   {
      /*  Calculating the Euclidean distances between any of the representing circles of the Ego and Object
       *  and checking if any distance is smaller than the critical threshold
       */

      /* Check ego front circles */
      if (Ltb_Does_Ego_Circle_Overlap_Any_Object_Circle(p_ego, p_obj, min_safe_distance))
      {
         f_critical_approach = FBK_TRUE;
      }
   }
   return f_critical_approach;
}

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ltb_Init_Prediction_Time_Step(Ltb_Persistent_T *p_ltb_persistent, const Ltb_Core_Calibration_T *p_ltb_cal)
{
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);

   /* Initialize value to zero */
   p_ltb_persistent->ltb_pred_step_dt = FBK_ZERO_F;

   /* Check if this value is not zero as well as not negativ, because this make no sense here. */
   if ((p_ltb_cal->k_ltb_prediction_steps_max > FBK_ZERO_UINT) && (p_ltb_cal->k_ltb_alert_lvl_2_ttc_threshold > FBK_ZERO_F))
   {
      /* calculate dT based on current CAL values  */
      p_ltb_persistent->ltb_pred_step_dt =
         p_ltb_cal->k_ltb_alert_lvl_1_ttc_threshold / ((float32_T) p_ltb_cal->k_ltb_prediction_steps_max);
   }
}

void Ltb_Get_Ego_Deceleration_To_Avoid_Collision(Ltb_Object_T *p_ltb_object,
                                                 const Fbk_Trajectory_T *p_ego_trajectory,
                                                 const Ltb_Persistent_T *p_ltb_persistent,
                                                 const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Asserts */
   assert(NULL != p_ltb_object);
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_ltb_cal);

   /* calculate current deceleration estimate that avoids a collision (also considers brake dead time and gradient)*/
   if (LTB_INVALID_TTC > p_ltb_object->attributes.ttc)
   {
      /* Calculate the time left to decelerate the host vehicle */
      float32_T t_brake = p_ltb_object->attributes.ttc - p_ltb_cal->k_ltb_brake_dead_time;

      if (t_brake > EPSILON)
      {
         /* considers brake dead time to correct deceleration request estimate */
         float32_T host_speed_after_dead_time = Ltb_Get_Host_Speed_After_Dead_Time(p_ego_trajectory, p_ltb_persistent, p_ltb_cal);

         /* Get deceleration value corrected for brake dead time and a limiting deceleration gradient */
         p_ltb_object->attributes.decel_to_avoid_coll = Ltb_Get_Gradient_Restricted_Deceleration(
            t_brake, host_speed_after_dead_time, p_ltb_cal->k_f_ltb_enable_brake_gradient_logic, p_ltb_cal->k_ltb_brake_gradient);
      }
      else
      {
         /* for a combined TTC of zero or below set the maximum calibrated deceleration value */
         p_ltb_object->attributes.decel_to_avoid_coll = p_ltb_cal->k_ltb_brake_deceleration_max;
      }

      /* limit deceleration value to calibrated max value */
      p_ltb_object->attributes.decel_to_avoid_coll =
         Fbk_Clamp(p_ltb_object->attributes.decel_to_avoid_coll, FBK_ZERO_F, p_ltb_cal->k_ltb_brake_deceleration_max);
   }
}

void Ltb_Get_Object_Ttc(Ltb_Object_T *p_ltb_object,
                        const Fbk_Trajectory_T *p_ego_trajectory,
                        const Ltb_Persistent_T *p_ltb_persistent,
                        const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Iterator variables */
   uint8_t pred_step;
   boolean_T f_skip_further_steps = FBK_FALSE;

   /* Pointer to object tracjectory */
   const Fbk_Trajectory_T *p_obj_trajectory = &p_ltb_object->attributes.trajectory;

   /* Asserts */
   assert(NULL != p_ltb_object);
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);

   /* Check validity of trajectories */
   if (Fbk_Is_True(p_ego_trajectory->f_trajectory_valid) && Fbk_Is_True(p_obj_trajectory->f_trajectory_valid))
   {
      /* Iterate through prediction steps until a collision is detected */
      for (pred_step = FBK_ZERO_INT; pred_step < p_ego_trajectory->n_prediction_steps; pred_step++)
      {
         /* Check validity of current waypoints */
         if (Fbk_Is_True(p_ego_trajectory->waypoint[pred_step].f_waypoint_valid)
             && Fbk_Is_True(p_obj_trajectory->waypoint[pred_step].f_waypoint_valid))
         {
            /* Check if any of the waypoint circles overlap for this timestep */
            if (Ltb_Is_Critical_Approach(p_ltb_cal, &(p_ego_trajectory->waypoint[pred_step]), &(p_obj_trajectory->waypoint[pred_step])))
            {
               /* Critical approach detected -> calculate ttc based on prediction step and dT */
               p_ltb_object->attributes.ttc                   = (float32_T) pred_step * p_ltb_persistent->ltb_pred_step_dt;
               p_ltb_object->attributes.waypoint_at_collision = p_obj_trajectory->waypoint[pred_step].waypoint_coordinates;

               /* skip any further prediction steps for this object */
               f_skip_further_steps = FBK_TRUE;
            }
         }
         else
         {
            /* waypoint invalid -> skip any further prediction steps */
            f_skip_further_steps = FBK_TRUE;
         }

         if (Fbk_Is_True(f_skip_further_steps))
         {
            /* skip any further prediction steps for this object */
            break;
         }
      }
   }
}

void Ltb_Get_Object_Ttb(Ltb_Object_T *p_ltb_object,
                        const Fbk_Trajectory_T *p_ego_trajectory,
                        const Ltb_Persistent_T *p_ltb_persistent,
                        const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Calculate time-to-brake (TTB). This is the time until a brake request is sent. */
   float32_T t_brake_at_threshold, t_brake_total, host_speed_after_dead_time;

   /* Asserts */
   assert(NULL != p_ltb_object);
   assert(NULL != p_ltb_cal);

   /* Get the host speed including host speed changes during dead time */
   host_speed_after_dead_time = Ltb_Get_Host_Speed_After_Dead_Time(p_ego_trajectory, p_ltb_persistent, p_ltb_cal);

   if Fbk_Is_False (p_ltb_cal->k_f_ltb_enable_brake_gradient_logic)
   {
      /* No gradient considered for braking time */
      t_brake_at_threshold = host_speed_after_dead_time / p_ltb_cal->k_ltb_alert_lvl_3_decel_threshold;
   }
   else
   {
      /* t_brake is equal (a_brake / 2 * j_0) - (v_0 / a_brake) */
      t_brake_at_threshold = (host_speed_after_dead_time / p_ltb_cal->k_ltb_alert_lvl_3_decel_threshold)
                             - (p_ltb_cal->k_ltb_alert_lvl_3_decel_threshold / (2.0f * p_ltb_cal->k_ltb_brake_gradient));
   }

   /* Set t_brake_total, which is the total duration of the brake request including dead time */
   t_brake_total = t_brake_at_threshold + p_ltb_cal->k_ltb_brake_dead_time;

   /* Set TTB based on TTC, braking time when decelerating with threshold value and dead time */
   p_ltb_object->attributes.ttb = p_ltb_object->attributes.ttc - t_brake_total;

   if (t_brake_total > p_ltb_cal->k_ltb_alert_lvl_3_ttc_threshold)
   {
      /* Special case for total brake time above TTC threshold for brake request. */
      p_ltb_object->attributes.ttb = p_ltb_object->attributes.ttc - p_ltb_cal->k_ltb_alert_lvl_3_ttc_threshold;
   }
}
