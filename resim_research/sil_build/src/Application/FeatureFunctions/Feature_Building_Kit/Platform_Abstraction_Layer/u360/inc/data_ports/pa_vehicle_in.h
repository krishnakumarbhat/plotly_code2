#ifndef PA_VEHICLE_IN_H
#define PA_VEHICLE_IN_H

/**
 * @file pa_vehicle_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA vehicle macros for the U360 tracker
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "fbk_macros.h"
#include "pa_context.h"
#include "pa_mock_functions.h"
#include "pa_shared_types.h"

/*============================================================================*\
* Mapping functions
\*============================================================================*/

#ifdef __GNUC__
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static Pa_Veh_Prndl_State_T Pa_U360_Map_Prndl_State(PRNDL_state_T u360_prndl_state) __attribute__((unused));
#endif

static Pa_Veh_Prndl_State_T Pa_U360_Map_Prndl_State(PRNDL_state_T u360_prndl_state)
{
   Pa_Veh_Prndl_State_T pa_prndl_state;

   switch (u360_prndl_state)
   {
      case PRNDL_STATE_PARK:
         pa_prndl_state = PA_VEH_PRNDL_STATE_PARK;
         break;
      case PRNDL_STATE_REVERSE:
         pa_prndl_state = PA_VEH_PRNDL_STATE_REVERSE;
         break;
      case PRNDL_STATE_NEUTRAL:
         pa_prndl_state = PA_VEH_PRNDL_STATE_NEUTRAL;
         break;
      case PRNDL_STATE_DRIVE:
         pa_prndl_state = PA_VEH_PRNDL_STATE_DRIVE;
         break;
      case PRNDL_STATE_LOW:
         pa_prndl_state = PA_VEH_PRNDL_STATE_LOW;
         break;
      default:
         pa_prndl_state = PA_VEH_PRNDL_STATE_PARK;
         break;
   }

   return pa_prndl_state;
}

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns the length of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Length(p_context) ((float32_T) (p_context)->p_vehicle_config_data->host_vehicle_length)

/**
 * @brief Returns the width of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Width(p_context) ((float32_T) (p_context)->p_vehicle_config_data->host_vehicle_width)

/**
 * @brief Returns the rear axle position of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Rear_Axle_Position(p_context) ((float32_T) (p_context)->p_vehicle_data->c_rear_axle_posn)

/**
 * @brief Returns hosts speed signal in [m/s].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Speed(p_context) ((float32_T) (p_context)->p_vehicle_data->speed)

/**
 * @brief Returns the wheelbase distance of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Wheelbase(p_context) (Pa_Mock_Float(p_context, 0u, 0.0f))

/**
 * @brief Returns hosts steering angle in [rad].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Steering_Angle(p_context) (Pa_Mock_Float(p_context, 0u, 0.0f))

/**
 * @brief Returns hosts yawrate in [rad / s]. Turn to right shall be positive
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Yawrate(p_context) ((float32_T) (p_context)->p_vehicle_data->yawrate)

/**
 * @brief Returns hosts longitudinal velocity in [m/s].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Long_Vel(p_context) ((float32_T) (p_context)->p_vehicle_data->vcs_long_vel)

/**
 * @brief Returns hosts longitudinal acceleration in [m/s^2].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Long_Acc(p_context) ((float32_T) (p_context)->p_vehicle_data->accel)

/**
 * @brief Returns hosts lateral acceleration in [m/s^2].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Lat_Acc(p_context) (Pa_Mock_Float(p_context, 0u, 0.0f))

/**
 * @brief Returns hosts position of gear.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Prndl(p_context) (Pa_U360_Map_Prndl_State((p_context)->p_vehicle_data->prndl))

/**
 * @brief Returns turn signal.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Turn_Signal(p_context) (Pa_Mock_Uint(p_context, 0u, 0u))

/**
 * @brief Returns host curvature.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Curvature(p_context) (Pa_Mock_Float(p_context, 0u, 1.0f))

/**
 * @brief Returns lane width.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Lane_Width(p_context) (Pa_Mock_Float(p_context, 0u, 0.0f))

/**
 * @brief Returns lane center offset.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Lane_Center_Offset(p_context) (Pa_Mock_Float(p_context, 0u, 0.0f))

/**
 * @brief Returns reverse gear flag.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Reverse_Gear_Flag(p_context) (Pa_Mock_Boolean(p_context, 0u, FBK_FALSE))

#endif
