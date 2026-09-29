#ifndef FBK_OBJ_TRAJ_PREDICTOR_H
#define FBK_OBJ_TRAJ_PREDICTOR_H

/**
 * @file fbk_obj_traj_predictor.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module calculates the object trajectory prediction with the Kinematic Motion Model.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_object_data_t.h"
#include "fbk_traj_predictor_t.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Predicts the object trajectory based on object data.
 *
 * @return void
 *
 * @SRS{SF-302, SF-304}
 * @SAE{SF-2552}
 * @SDD{SF-4241}
 * @verification{Create tests in which last valid step is greater than zero. Only in that case a valid trajectory shall be
 * returned.}
 */
void Fbk_Predict_Obj_Trajectory(Fbk_Trajectory_T *p_obj_trajectory /**< object trajectory */,
                                const Fbk_Object_Data_T *p_object_tracker_data /**< FBK Tracker info */,
                                const Fbk_Object_Predict_Data_T *p_fbk_obj_data /**< FBK object data */);

#endif /* FBK_OBJ_TRAJ_PREDICTOR_H */
