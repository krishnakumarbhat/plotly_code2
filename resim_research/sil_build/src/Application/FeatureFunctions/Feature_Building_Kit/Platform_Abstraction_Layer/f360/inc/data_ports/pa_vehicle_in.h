#ifndef PA_VEHICLE_IN_H
#define PA_VEHICLE_IN_H

/**
 * @file pa_vehicle_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA vehicle macros for the F360 tracker
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
static inline Pa_Veh_Prndl_State_T Pa_F360_Map_Prndl(uint8_t f360_prndl) __attribute__((unused));
#endif


static inline Pa_Veh_Prndl_State_T Pa_F360_Map_Prndl(uint8_t f360_prndl)
{
   Pa_Veh_Prndl_State_T pa_prndl;

   switch (f360_prndl)
   {
      case 0u:
         pa_prndl = PA_VEH_PRNDL_STATE_PARK;
         break;
      case 1u:
         pa_prndl = PA_VEH_PRNDL_STATE_REVERSE;
         break;
      case 2u:
         pa_prndl = PA_VEH_PRNDL_STATE_NEUTRAL;
         break;
      case 3u:
         pa_prndl = PA_VEH_PRNDL_STATE_DRIVE;
         break;
      case 4u:
         pa_prndl = PA_VEH_PRNDL_STATE_DRIVE;
         break;
      case 5u:
         pa_prndl = PA_VEH_PRNDL_STATE_DRIVE;
         break;
      case 6u:
         pa_prndl = PA_VEH_PRNDL_STATE_LOW;
         break;
      default:
         pa_prndl = PA_VEH_PRNDL_STATE_PARK;
         break;
   }

   return pa_prndl;
}

/*============================================================================*\
* Function like macro
\*============================================================================*/

/**
 * @brief Returns the length of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Length(p_context) (Pa_Mock_Float(p_context, 0u, 4.5933f))

/**
 * @brief Returns the width of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Width(p_context) (Pa_Mock_Float(p_context, 0u, 1.8908f))

/**
 * @brief Returns hosts speed signal in [m/s].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Host_Speed(p_context) ((float32_T) (p_context)->p_vehicle_data->speed)

/**
 * @brief Returns the rear axle position of the ego vehicle in [m].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Rear_Axle_Position(p_context) ((float32_T) (p_context)->p_vehicle_data->dist_rear_axle_to_vcs_m)

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
#define Pa_Veh_Get_Steering_Angle(p_context) ((float32_T) (p_context)->p_vehicle_data->steering_angle_rad)

/**
 * @brief Returns hosts yawrate in [rad / s]. Turn to right shall be positive
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Yawrate(p_context) ((float32_T) (p_context)->p_vehicle_data->raw_yaw_rate_rad)


/**
 * @brief Returns hosts longitudinal velocity in [m/s].
 * Assumption: Lateral movement is seen as negligible in most cases.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Long_Vel(p_context) (Pa_Veh_Get_Host_Speed(p_context))


/**
 * @brief Returns hosts longitudinal acceleration in [m/s^2].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Long_Acc(p_context) ((float32_T) (p_context)->p_vehicle_data->vcs_long_acceleration)

/**
 * @brief Returns hosts lateral acceleration in [m/s^2].
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Lat_Acc(p_context) ((float32_T) (p_context)->p_vehicle_data->vcs_lat_acceleration)

/**
 * @brief Returns hosts position of gear.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Prndl(p_context) (Pa_F360_Map_Prndl((p_context)->p_vehicle_data->prndl))

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
 * @brief Returns reverse gear flag.
 *
 * @SDD{}
 */
#define Pa_Veh_Get_Reverse_Gear_Flag(p_context) ((boolean_T) (Fbk_Is_True(p_context->p_vehicle_data->f_reverse_gear)))

#endif
