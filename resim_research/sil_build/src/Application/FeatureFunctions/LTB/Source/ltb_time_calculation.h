#ifndef LTB_TIME_CALCULATION_H
#define LTB_TIME_CALCULATION_H

/**
 * @file ltb_time_calculation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module implements logic for determining if two objects have a high probability of collision
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_traj_predictor_t.h"
#include "ltb_core_calibration_t.h"
#include "ltb_persistent_t.h"
#include "ltb_types.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Init prediction time step
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53934}
 * @verification{If and only if k_ltb_prediction_steps_max is greater than zero and k_ltb_alert_lvl_2_ttc_threshold is greater than
 * zero, then a non default value for the prediction time step shall be set.}
 */
void Ltb_Init_Prediction_Time_Step(Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent data */,
                                   const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


/**
 * @brief Calculates the deceleration necessary to avoid the collision with this object.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53935}
 * @verification{Create a test with host speed greater than zero and a valid ttc. Here two scenarios can occure When the time left
 * to decelerate the host vehicle is greater than zero a non default deceleration shall be calculated.}
 */
void Ltb_Get_Ego_Deceleration_To_Avoid_Collision(Ltb_Object_T *p_ltb_object /**< LTB Object */,
                                                 const Fbk_Trajectory_T *p_ego_trajectory /**< LTB Trajectory for ego */,
                                                 const Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent Data */,
                                                 const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


/**
 * @brief Calculates the object TTC based on the ego and object trajectory.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53932}
 * @verification{Create a test where the host and object trajectories as well as the waypoints are valid. The object which is
 * approaching shall be critical the result shall be dependend on the prediction step: ttc = pred_step * dt waypoint_at_collision =
 * waypoint[pred_step].waypoint_coordinates}
 */
void Ltb_Get_Object_Ttc(Ltb_Object_T *p_ltb_object /**< LTB Object */,
                        const Fbk_Trajectory_T *p_ego_trajectory /**< LTB Trajectory for ego */,
                        const Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent Data */,
                        const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);
/**
 * @brief Calculates the time-to-brake (TTB) for this object. TTB is the time until a brake request is sent.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53933}
 * @verification{Create a test with given inputs. When the total brake time is not exceeding k_ltb_alert_lvl_3_ttc_threshold, ttb =
 * ttc - t_brake_total is shall be expected. Otherwise ttb = ttc - k_ltb_alert_lvl_3_ttc_threshold is returned.}
 */
void Ltb_Get_Object_Ttb(Ltb_Object_T *p_ltb_object /**< LTB Object */,
                        const Fbk_Trajectory_T *p_ego_trajectory /**< LTB Trajectory for ego */,
                        const Ltb_Persistent_T *p_ltb_persistent /**< LTB Persistent Data */,
                        const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


#endif /* LTB_TIME_CALCULATION_H */
