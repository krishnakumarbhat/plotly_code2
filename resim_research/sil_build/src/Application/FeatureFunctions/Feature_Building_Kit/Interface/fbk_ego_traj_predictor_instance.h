#ifndef FBK_EGO_TRAJ_PREDICTOR_INSTANCE_H
#define FBK_EGO_TRAJ_PREDICTOR_INSTANCE_H

/**
 * @file fbk_ego_traj_predictor_instance.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains the FBK output data types.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

#include "fbk_iface_types.h"
#include "fbk_traj_predictor_t.h"

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Fbk_Host_Circles_Props_T structure
 *
 * Contains the ego circles properties needed for prediction of host path
 *
 * @SDD{SF-4222}
 * @verification{}
 */
typedef struct
{
   Fbk_Circle_Center_Offset_T offsets; /**< offset values of all 3 ego circles relative to host vcs */
   float32_T radius;                   /**< radius of all ego circles */

} Fbk_Host_Circles_Props_T;

/**
 * @brief Fbk_Ego_Traj_Predictor_Instance_T structure
 *
 * @SDD{}
 * @verification{}
 */
typedef struct
{
   Fbk_Host_Circles_Props_T Host_Circles_Props;
   float32_T Host_Curve_Radius;
   float32_T Integral_Predicted_Velocity;
   float32_T Integral_Predicted_Arc_Length;
   boolean_T F_Host_Circle_Props_Initialized;
   /* coverity[misra_c_2012_rule_1_1_violation][typedef name has already been declared (with same type)] */
} Fbk_Ego_Traj_Predictor_Instance_T;

#endif /* FBK_EGO_TRAJ_PREDICTOR_INSTANCE_H */
