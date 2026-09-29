#ifndef FBK_VEHICLE_DATA_T_H
#define FBK_VEHICLE_DATA_T_H

/**
 * @file fbk_vehicle_data_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for fbk vehicle data type definition.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Include
\*===========================================================================*/

#include "pa_reuse.h"
#include "pa_shared_types.h"

/*===========================================================================*\
* Typedef
\*===========================================================================*/

/**
 * @brief Fbk_Vehicle_Data_T structure
 *
 * Summarizes the consumed vehicle data of the Sfl stack.
 *
 */
typedef struct
{
   float32_T host_length;        /**< [m] Length of host vehicle */
   float32_T host_width;         /**< [m] Width of host vehicle */
   float32_T rear_axle_position; /**< [m] Rear axle position of host vehicle (to VCS origin at front center)*/
   float32_T host_speed;         /**< [m/s] Over ground speed of host vehicle */
   float32_T steering_angle;     /**< [rad] Steering angle of host vehicle (in VCS, clockwise positive) */
   float32_T yawrate;            /**< [rad/s] Yawrate of host vehicle */
   float32_T long_vel;           /**< [m/s] Longitudinal velocity of host vehicle */
   float32_T long_acc;           /**< [m/s^2] Longitudinal acceleration of host vehicle */
   float32_T lat_acc;            /**< [m/s^2] Lateral acceleration of host vehicle */
   Pa_Veh_Prndl_State_T prndl;   /**< Park-Reverse-Neutral-Drive-Low setting (enum 0-1-2-3-4) */
   float32_T lane_width;         /**< [m] Lane width of host vehicle lane */
   float32_T lane_center_offset; /**< [m] Lane center offset */
   uint8_t turn_signal;          /**< Turn signal: None-Left-Right (enum 0-1-2) */
   float32_T curvature;          /**< [1/m] Host curvature */
   boolean_T f_reverse;          /**< Flag indicating if host vehicle is in reverse gear */
   float32_T wheelbase;          /**< [m] Wheelbase distance of host vehicle */
} Fbk_Vehicle_Data_T;

#endif /* FBK_VEHICLE_DATA_T_H */
