#ifndef PA_VEHICLE_IN_H
#define PA_VEHICLE_IN_H

/**
 * @file pa_vehicle_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA vehicle macros for the generic interface
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "pa_context.h"

/*============================================================================*\
 * Defines
\*============================================================================*/

/**
 * @brief Returns the length of the ego vehicle in [m].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Length(p_context) ((p_context)->p_data->vehicle_data.host_length)

/**
 * @brief Returns the width of the ego vehicle in [m].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Width(p_context) ((p_context)->p_data->vehicle_data.host_width)

/**
 * @brief Returns the rear axle position of the ego vehicle in [m].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Rear_Axle_Position(p_context) ((p_context)->p_data->vehicle_data.rear_axle_position)

/**
 * @brief Returns the wheelbase ego vehicle in [m].
 */
#define Pa_Veh_Get_Wheelbase(p_context) ((p_context)->p_data->vehicle_data.wheelbase)

/**
 * @brief Returns hosts speed signal in [m/s].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Speed(p_context) ((p_context)->p_data->vehicle_data.host_speed)

/**
 * @brief Returns hosts steering angle in [rad].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Steering_Angle(p_context) ((p_context)->p_data->vehicle_data.steering_angle)

/**
 * @brief Returns hosts yawrate in [rad / s]. Turn to right shall be positive
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Yawrate(p_context) ((p_context)->p_data->vehicle_data.yawrate)

/**
 * @brief Returns hosts longitudinal velocity in [m/s].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Long_Vel(p_context) ((p_context)->p_data->vehicle_data.long_vel)

/**
 * @brief Returns hosts longitudinal acceleration in [m/s^2].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Long_Acc(p_context) ((p_context)->p_data->vehicle_data.long_acc)

/**
 * @brief Returns hosts lateral acceleration in [m/s^2].
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Lat_Acc(p_context) ((p_context)->p_data->vehicle_data.lat_acc)

/**
 * @brief Returns hosts position of gear.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Prndl(p_context) ((p_context)->p_data->vehicle_data.prndl)

/**
 * @brief Returns turn signal.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Turn_Signal(p_context) ((p_context)->p_data->vehicle_data.turn_signal)

/**
 * @brief Returns host curvature.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Curvature(p_context) ((p_context)->p_data->vehicle_data.curvature)

/**
 * @brief Returns lane width.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Lane_Width(p_context) ((p_context)->p_data->vehicle_data.lane_width)

/**
 * @brief Returns lane center offset.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Lane_Center_Offset(p_context) ((p_context)->p_data->vehicle_data.lane_center_offset)

/**
 * @brief Returns reverse gear flag.
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Reverse_Gear_Flag(p_context) ((p_context)->p_data->vehicle_data.f_reverse)

#endif
