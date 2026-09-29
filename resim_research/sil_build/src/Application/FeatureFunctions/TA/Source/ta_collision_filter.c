/**
 * @file ta_collision_filter.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module filters the nearest collision object from all objects.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_collision_filter.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_traj_predictor_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "ta_constants.h"
#include "ta_intersection_analyzer.h"
#include <assert.h>

/*============================================================================*\
 * LOCAL FUNCTION PROTOTYPES
\*============================================================================*/
/**
 * @brief Retrieves the host speed that we calculated after the dead time period has passed.
 *
 * @return host speed including acceleration induced change during dead time
 *
 * @SRS{SF-2323}
 * @SAE{SF-3238}
 * @SDD{SF-8792}
 * @verification{Check whether host speed after dead time of brake is calculated correctly.}
 */
static float32_T Ta_Get_Host_Speed_After_Dead_Time(const Fbk_Trajectory_T *p_ego_trajectory /**< TA Trajectory for ego */,
                                                   const Ta_Persistent_T *p_ta_persistent /**< TA Persistent data */,
                                                   const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Calculates the deceleration necessary to avoid a collision while also considering a limiting
 * deceleration gradient or jerk value.
 *
 * @return positive deceleration value that is corrected for a limited deceleration gradient or jerk
 *
 * @SRS{SF-2323}
 * @SAE{SF-3238}
 * @SDD{SF-8793}
 * @verification{Create a test where the jerk value is greater than zero. Also the discriminant in the formular needs to be greater
 * than 0. Only in that case a non default jerk value shall be returned.}
 */
static float32_T
Ta_Get_Gradient_Restricted_Deceleration(const float32_T t_brake /**< [s] Time left to decelerate host vehicle */,
                                        const float32_T v_0 /**< [m/s] Host vehicle velocity at start of deceleration */,
                                        const boolean_T f_enable_gradient /**< Enable flag to activate gradient logic */,
                                        const float32_T j_0 /**< [m/s^3] Jerk value used to ramp up deceleration */);

/*============================================================================*\
 * LOCAL FUNCTIONS

\*============================================================================*/
static float32_T Ta_Get_Host_Speed_After_Dead_Time(const Fbk_Trajectory_T *p_ego_trajectory,
                                                   const Ta_Persistent_T *p_ta_persistent,
                                                   const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Return value */
   float32_T host_speed_after_dead_time = FBK_ZERO_F;

   /* Asserts */
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

   /* Get the nearest prediction step that matches the calibrated brake dead time
    * and return the previously calculated host velocity for it */
   if (FBK_ZERO_F < p_ta_persistent->ta_pred_step_dt)
   {
      float32_T n_pred_steps_float       = p_ta_cal->k_fta_brake_dead_time / p_ta_persistent->ta_pred_step_dt;
      uint8_t brake_dead_time_pred_steps = (uint8_t) n_pred_steps_float;

      /* Prevent an array access out of bounds */
      if (brake_dead_time_pred_steps < p_ego_trajectory->n_prediction_steps)
      {
         host_speed_after_dead_time = p_ego_trajectory->waypoint[brake_dead_time_pred_steps].waypoint_speed;
      }
   }

   return host_speed_after_dead_time;
}

static float32_T Ta_Get_Gradient_Restricted_Deceleration(const float32_T t_brake,
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

/**
 * @brief Check different ego vehicle and target information to decide if the TTP calculation shall be suppressed.
 *
 * @return Flag indicating if TTP calculation shall be suppressed.
 *
 * @SRS{SF-2318}
 * @SAE{}
 * @SDD{SF-8613}
 * @verification{}
 */
static boolean_T Ta_Suppress_Ttp_Calculation(const Ta_Object_T *p_ta_object /**< TA Object */,
                                             const float32_T host_curvature /**< Curvature of host vehicle */,
                                             const float32_T obj_lat_vel_rel /**< Lateral relative velocity of object */,
                                             const float32_T obj_heading /**< Object heading */,
                                             const float32_T obj_distance /**< Object distance */,
                                             const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Get_Object_Ttc(Ta_Object_T *p_ta_object,
                       const Fbk_Trajectory_T *p_ego_trajectory,
                       const Ta_Persistent_T *p_ta_persistent,
                       const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Iterator variables */
   uint8_t pred_step;
   boolean_T f_skip_further_steps = FBK_FALSE;

   /* Pointer to object tracjectory */
   const Fbk_Trajectory_T *p_obj_trajectory = &p_ta_object->attributes.trajectory;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

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
            if (Ta_Is_Critical_Approach(p_ta_cal, &(p_ego_trajectory->waypoint[pred_step]), &(p_obj_trajectory->waypoint[pred_step])))
            {
               /* Critical approach detected -> calculate ttc based on prediction step and dT */
               p_ta_object->attributes.ttc                   = (float32_T) pred_step * p_ta_persistent->ta_pred_step_dt;
               p_ta_object->attributes.waypoint_at_collision = p_obj_trajectory->waypoint[pred_step].waypoint_coordinates;

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

void Ta_Get_Object_Distance_to_Vcs_Origin(Ta_Object_T *p_ta_object)
{
   /* Assert */
   assert(NULL != p_ta_object);

   /* Calculate euclidean distance from object position to VCS origin */
   p_ta_object->attributes.distance_to_ego = Vector_2d_Alg_Abs(&(p_ta_object->tracker_data.vcs_pos));
}

void Ta_Get_Ego_Deceleration_To_Avoid_Collision(Ta_Object_T *p_ta_object,
                                                const Fbk_Trajectory_T *p_ego_trajectory,
                                                const Ta_Persistent_T *p_ta_persistent,
                                                const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ego_trajectory);
   assert(NULL != p_ta_cal);

   /* calculate current deceleration estimate that avoids a collision (also considers brake dead time and gradient)*/
   if (TA_INVALID_TTC > p_ta_object->attributes.ttc)
   {
      /* Calculate the time left to decelerate the host vehicle */
      float32_T t_brake = p_ta_object->attributes.ttc - p_ta_cal->k_fta_brake_dead_time;

      if (t_brake > EPSILON)
      {
         /* considers brake dead time to correct deceleration request estimate */
         float32_T host_speed_after_dead_time = Ta_Get_Host_Speed_After_Dead_Time(p_ego_trajectory, p_ta_persistent, p_ta_cal);

         /* Get deceleration value corrected for brake dead time and a limiting deceleration gradient */
         p_ta_object->attributes.decel_to_avoid_coll = Ta_Get_Gradient_Restricted_Deceleration(
            t_brake, host_speed_after_dead_time, p_ta_cal->k_f_fta_enable_brake_gradient_logic, p_ta_cal->k_fta_brake_gradient);
      }
      else
      {
         /* for a combined TTC of zero or below set the maximum calibrated deceleration value */
         p_ta_object->attributes.decel_to_avoid_coll = p_ta_cal->k_fta_brake_deceleration_max;
      }

      /* limit deceleration value to calibrated max value */
      p_ta_object->attributes.decel_to_avoid_coll =
         Fbk_Clamp(p_ta_object->attributes.decel_to_avoid_coll, FBK_ZERO_F, p_ta_cal->k_fta_brake_deceleration_max);
   }
}

void Ta_Get_Object_Ttb(Ta_Object_T *p_ta_object,
                       const Fbk_Trajectory_T *p_ego_trajectory,
                       const Ta_Persistent_T *p_ta_persistent,
                       const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Calculate time-to-brake (TTB). This is the time until a brake request is sent. */
   float32_T t_brake_at_threshold, t_brake_total, host_speed_after_dead_time;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   /* Get the host speed including host speed changes during dead time */
   host_speed_after_dead_time = Ta_Get_Host_Speed_After_Dead_Time(p_ego_trajectory, p_ta_persistent, p_ta_cal);

   if (p_ta_cal->k_fta_brake_gradient >= FBK_ZERO_F)
   {
      /* No gradient considered for braking time */
      t_brake_at_threshold = host_speed_after_dead_time / p_ta_cal->k_ta_alert_lvl_4_decel_threshold;
   }
   else
   {
      /* t_brake is equal (a_brake / 2 * j_0) - (v_0 / a_brake) */
      t_brake_at_threshold = (host_speed_after_dead_time / p_ta_cal->k_ta_alert_lvl_4_decel_threshold)
                             - (p_ta_cal->k_ta_alert_lvl_4_decel_threshold / (2.0f * p_ta_cal->k_fta_brake_gradient));
   }

   /* Set t_brake_total, which is the total duration of the brake request including dead time */
   t_brake_total = t_brake_at_threshold + p_ta_cal->k_fta_brake_dead_time;

   /* Set TTB based on TTC, braking time when decelerating with threshold value and dead time */
   p_ta_object->attributes.ttb = p_ta_object->attributes.ttc - t_brake_total;

   if (t_brake_total > p_ta_cal->k_ta_alert_lvl_4_ttc_threshold)
   {
      /* Special case for total brake time above TTC threshold for brake request. */
      p_ta_object->attributes.ttb = p_ta_object->attributes.ttc - p_ta_cal->k_ta_alert_lvl_4_ttc_threshold;
   }
}

static boolean_T Ta_Suppress_Ttp_Calculation(const Ta_Object_T *p_ta_object,
                                             const float32_T host_curvature,
                                             const float32_T obj_lat_vel_rel,
                                             const float32_T obj_heading,
                                             const float32_T obj_distance,
                                             const Ta_Core_Calibration_T *p_ta_cal)
{
   boolean_T f_suppress_ttp = FBK_FALSE;

   if ((Fbk_Abs_F(obj_lat_vel_rel) > p_ta_cal->k_rta_ttp_obj_abs_lat_vel_rel_max)
       && (Fbk_Abs_F(host_curvature)
           > p_ta_cal->k_ta_lookup_turning_host_curvature_min[TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0 - FBK_ONE_UINT]))
   {
      /* The TTP calculation for objects that have a fast lateral movement away from the info zone will be suppressed. This
       * suppression is only applied in case the ego vehicle is turning more than the last lookup table entry. */
      f_suppress_ttp = FBK_TRUE;
   }

   if (Fbk_Abs_F(obj_heading) > p_ta_cal->k_rta_ttp_obj_abs_heading_diff_max)
   {
      /* The object heading diverges too far from the info zone. */
      f_suppress_ttp = FBK_TRUE;
   }

   if (Fbk_Is_False(p_ta_object->attributes.f_curvi_available)
       && (Fbk_Abs_F(host_curvature) > p_ta_cal->k_ta_straight_host_curvature_max)
       && (Fbk_Abs_F(obj_distance) >= p_ta_cal->k_rta_ttp_curve_suppression_obj_distance_min))
   {
      /* Suppress TTP calculation for objects that do not have curvi info available and the host vehicle not driving straight. */
      f_suppress_ttp = FBK_TRUE;
   }

   return f_suppress_ttp;
}

void Ta_Get_Object_Ttp(Ta_Object_T *p_ta_object, const float32_T host_curvature, const Ta_Core_Calibration_T *p_ta_cal)
{
   float32_T obj_distance;
   float32_T obj_long_vel_rel;
   float32_T obj_lat_vel_rel;
   float32_T obj_heading;

   /* Asserts */
   assert(NULL != p_ta_object);
   assert(NULL != p_ta_cal);

   /* Prediction of object */
   if (Fbk_Is_True(p_ta_object->attributes.f_curvi_available))
   {
      /* Use curvi coordinates to set the current object distance */
      obj_distance = p_ta_object->tracker_data.curvi_pos.x;

      /* Set relative velocity for ttp calculation */
      obj_long_vel_rel = p_ta_object->tracker_data.curvi_vel_rel.x;
      obj_lat_vel_rel  = p_ta_object->tracker_data.curvi_vel_rel.y;

      /* Set object heading */
      obj_heading = p_ta_object->tracker_data.curvi_heading;
   }
   else
   {
      /* Use vcs coordinates to set the current object distance */
      obj_distance = p_ta_object->tracker_data.vcs_pos.x;

      /* Set relative velocity for ttp calculation */
      obj_long_vel_rel = p_ta_object->tracker_data.vcs_vel_rel.x;
      obj_lat_vel_rel  = p_ta_object->tracker_data.vcs_vel_rel.y;

      /* Set object heading */
      obj_heading = p_ta_object->tracker_data.vcs_heading;
   }

   /* Check if object has longitudinal velocity */
   if (obj_long_vel_rel >= EPSILON)
   {

      /* Check if TTP calculation shall be suppressed. */
      if (Fbk_Is_False(Ta_Suppress_Ttp_Calculation(p_ta_object, host_curvature, obj_lat_vel_rel, obj_heading, obj_distance, p_ta_cal)))
      {
         /* Calculate time-to-pass (TTP) based on longitudinal distance and relative object velocity */
         p_ta_object->attributes.ttp = Fbk_Abs_F(obj_distance / obj_long_vel_rel);
      }
   }
}
