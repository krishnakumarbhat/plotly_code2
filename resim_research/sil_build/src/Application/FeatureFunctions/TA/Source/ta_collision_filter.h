#ifndef TA_COLLISION_FILTER_H
#define TA_COLLISION_FILTER_H

/**
 * @file ta_collision_filter.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module filters the nearest collision object from all objects.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_traj_predictor_t.h"
#include "pa_reuse.h"
#include "ta_core_calibration_t.h"
#include "ta_persistent_t.h"
#include "ta_types.h"

/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief Calculates the object TTC based on the ego and object trajectory.
 *
 * @return void
 *
 * @SRS{SF-2314,SF-2313}
 * @SAE{SF-3238}
 * @SDD{SF-8666}
 * @verification{Create a test where the host and object trajectories as well as the waypoints are valid. The object which is
 * approaching shall be critical the result shall be dependend on the prediction step: ttc = pred_step * dt waypoint_at_collision =
 * waypoint[pred_step].waypoint_coordinates}
 */
void Ta_Get_Object_Ttc(Ta_Object_T *p_ta_object /**< TA Object */,
                       const Fbk_Trajectory_T *p_ego_trajectory /**< TA Trajectory for ego */,
                       const Ta_Persistent_T *p_ta_persistent /**< TA Persistent Data */,
                       const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Calculates the objects distance to the VCS origin.
 *
 * @return void
 *
 * @SRS{SF-2323}
 * @SAE{SF-3238}
 * @SDD{SF-8664}
 * @verification{Setup a test with a object position unequal to zero. The expected distance is the euclidean distance for the
 * target objects VCS coordinates.}
 */
void Ta_Get_Object_Distance_to_Vcs_Origin(Ta_Object_T *p_ta_object /**< TA Object */);

/**
 * @brief Calculates the deceleration necessary to avoid the collision with this object.
 *
 * @return void
 *
 * @SRS{SF-2323}
 * @SAE{SF-3238}
 * @SDD{SF-8663}
 * @verification{Create a test with host speed greater than zero and a valid ttc. Here two scenarios can occure When the time left
 * to decelerate the host vehicle is greater than zero a non default deceleration shall be calculated.}
 */
void Ta_Get_Ego_Deceleration_To_Avoid_Collision(Ta_Object_T *p_ta_object /**< TA Object */,
                                                const Fbk_Trajectory_T *p_ego_trajectory /**< TA Trajectory for ego */,
                                                const Ta_Persistent_T *p_ta_persistent /**< TA Persistent Data */,
                                                const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

/**
 * @brief Calculates the time-to-brake (TTB) for this object. TTB is the time until a brake request is sent.
 *
 * @return void
 *
 * @SRS{SF-2367}
 * @SAE{SF-3238}
 * @SDD{SF-8665}
 * @verification{Create a test with given inputs. When the total brake time is not exceeding k_ta_alert_lvl_4_ttc_threshold, ttb =
 * ttc - t_brake_total is shall be expected. Otherwise ttb = ttc - k_ta_alert_lvl_4_ttc_threshold is returned.}
 */
void Ta_Get_Object_Ttb(Ta_Object_T *p_ta_object /**< TA Object */,
                       const Fbk_Trajectory_T *p_ego_trajectory /**< TA Trajectory for ego */,
                       const Ta_Persistent_T *p_ta_persistent /**< TA Persistent Data */,
                       const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);


/**
 * @brief Calculates the TTP (time to pass / time to overtake) for the given object
 *
 * @return void
 *
 * @SRS{SF-2318}
 * @SAE{SF-3238}
 * @SDD{SF-8667}
 * @verification{Create an object with longitudinal velocity greater than 0. The object needs to be on the same side as the
 * predicted intersection point. Only then a ttp shall be calculated.}
 */
void Ta_Get_Object_Ttp(Ta_Object_T *p_ta_object /**< TA Object */,
                       const float32_T host_curvature /**< Curvature of host vehicle */,
                       const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

#endif /* TA_COLLISION_FILTER_H */
