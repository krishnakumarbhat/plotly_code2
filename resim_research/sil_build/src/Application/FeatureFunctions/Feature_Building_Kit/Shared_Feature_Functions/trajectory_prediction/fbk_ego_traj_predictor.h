#ifndef FBK_EGO_TRAJ_PREDICTOR_H
#define FBK_EGO_TRAJ_PREDICTOR_H

/**
 * @file fbk_ego_traj_predictor.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements functions related to predicting the ego waypoints around the objects coordinates.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_traj_predictor_t.h"
#include "fbk_vehicle_data_t.h"

/**
 * @brief Predict the ego trajectory using vehicle data.
 *
 * @return void
 *
 * @SRS{SF-304}
 * @SAE{SF-2552}
 * @SDD{SF-4229}
 * @verification{Create a trajectory with an amount of prediction steps greater than 0. Only for this configuration a valid host
 * trajectory shall be returned.}
 */
void Fbk_Predict_Ego_Trajectory(Fbk_Ego_Traj_Predictor_Instance_T *p_instance,
                                Fbk_Trajectory_T *p_ego_trajectory /**< TA Ego Trajectory */,
                                const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                const Fbk_Ego_Predict_Data_T *p_fbk_ego_data /**< Host data */);

#endif /* FBK_EGO_TRAJ_PREDICTOR_H */
