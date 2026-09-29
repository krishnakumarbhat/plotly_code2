#ifndef FBK_TRAJ_PREDICTOR_T_H
#define FBK_TRAJ_PREDICTOR_T_H

/**
 * @file fbk_traj_predictor_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the TA specific data types.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_core_calibration_t.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/* EXPORTED DEFINES FOR CONSTANTS --------------------------------------------*/

/* Define for trajectory */
#define FBK_MAX_PREDICTION_STEPS (20u)

/*============================================================================*\
 * EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Contains the FBK required parameters which are derived from FF calibration parameters and persitent values about object.
 *
 * @SDD{SF-2232}
 */
typedef struct
{
   float32_T ego_acceleration_weight; /**< Controls the influence of the positive acceleration value on the predicted velocity in
                                         each prediction step */
   float32_T ego_shape_gain_fixed; /**< Grows or shrinks the ego shape by multiplying the specified value with the ego dimensions
                                      once at the beginning of the prediction phase. */
   float32_T ego_circle_offset;    /**< Weight factor adds an additional offset to the host circles */
   float32_T ego_circle_host_length_factor; /**< Weight factor controls the influence of the host vehicle length when calculating
                                               the host vehicle circle offsets */
   float32_T ego_deceleration_weight; /**< Weight factor controls the influence of the negative acceleration / deceleration value
                                         on the predicted velocity in each prediction step */
   float32_T ego_yaw_angle_to_last_straight_section; /**< Persistent parameter, integrated ego yaw angle to last known straight
                                                        section using ego yaw rate and cycle time information */
   float32_T ego_max_pred_yaw_angle; /**< Continue the ego trajectory calculation if the (absolute) predicted yaw angle to the last
                                        straight section is below this threshold */
   float32_T ego_shape_gain_per_pred_step; /**< Grows or shrinks an object's shape by multiplying the specified value with the
                                              object's dimensions at every prediction step. */
   float32_T pred_step_dt;                 /**< Persistent parameter, delta time value between two prediction time steps */
   uint8_t ego_pred_const_velocity_pred_steps_min; /**< This value indicates at which prediction step to switch to a constant
                                                      velocity prediction model. */
   uint8_t prediction_steps_max; /**<Maximum number of steps to predict trajectories of the ego and the relevant objects.*/

   boolean_T acc_weight_depend_on_alert_lvl; /**< Host vehicle likely slowing down due to entering zone requesting braking. */
} Fbk_Ego_Predict_Data_T;

/**
 * @brief Contains the FBK required parameters which are derived from FF calibration parameters and persitent values about host
 * car.
 *
 * @SDD{SF-4212}
 */
typedef struct
{
   float32_T obj_pred_speed_min;   /**< continues object trajectory calculation if the predicted waypoint speed is equal or above
                                      this threshold. */
   float32_T obj_shape_gain_fixed; /**< grows or shrinks an object 's shape by multiplying the specified value with the object' s
                                    dimensions once at the beginning of the prediction phase.*/
   float32_T obj_shape_gain_per_pred_step; /**< grows or shrinks the ego shape by multiplying the specified value with the ego
                                            * dimensions at every prediction step. This means, the more in the future a prediction
                                            * step is, the more the shape will be grown or shrunk. This is in addition to the fixed
                                            * gain specified by obj_shape_gain_fixed. */
   float32_T pred_step_dt;                 /**< Persistent parameter, delta time value between two prediction time steps */

} Fbk_Object_Predict_Data_T;


/**
 * @brief Fbk_Waypoint_with_Circle_Centers_T structure
 *
 * Stores the Waypoint coordinates with 3 circles to calculate critical approaches.
 *
 * @SDD{SF-4218}
 */
typedef struct
{
   Vector_2d_T waypoint_coordinates; /**< Waypoint coordinates */
   Angle_T waypoint_yaw_angle;       /**< predicted yaw angle for the current timestep */
   float32_T waypoint_speed;         /**< predicted speed for the current timestep */
   Vector_2d_T circle_center_front;  /**< center of the front circle used to calculate critical approach to vehicle */
   Vector_2d_T circle_center_middle; /**< center of the middle circle used to calculate critical approach to vehicle */
   Vector_2d_T circle_center_rear;   /**< center of the rear circle used to calculate critical approach to vehicle */
   float32_T circle_radius;          /**< radius of the circles */
   boolean_T f_waypoint_valid;       /**< flag indicating the validity of this waypoint */
} Fbk_Waypoint_with_Circle_Centers_T;

/**
 * @brief Fbk_Trajectory_T structure
 *
 * Stores trajectory information.
 *
 * @SDD{SF-4219}
 */
typedef struct
{
   uint8_t n_prediction_steps;                                            /**< Number of predictions steps used */
   Fbk_Waypoint_with_Circle_Centers_T waypoint[FBK_MAX_PREDICTION_STEPS]; /**< Trajectory consists of waypoints */
   boolean_T f_trajectory_valid;                                          /**< flag indicating the validity of this trajectory*/
} Fbk_Trajectory_T;

/**
 * @brief Ta_Circle_Center_Offset_T structure
 *
 * Structure holding the circle offsets
 *
 * @SDD{SF-4220}
 */
typedef struct
{
   float32_T offset_front_x;  /**< offset of front circle center x */
   float32_T offset_middle_x; /**< offset of middle circle center x */
   float32_T offset_rear_x;   /**< offset of rear circle center x */
} Fbk_Circle_Center_Offset_T;

#endif /* FBK_TRAJ_PREDICTOR_T_H */
