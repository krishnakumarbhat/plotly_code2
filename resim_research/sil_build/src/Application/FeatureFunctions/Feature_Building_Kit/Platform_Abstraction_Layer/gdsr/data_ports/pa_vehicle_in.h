#ifndef PA_VEHICLE_IN_H
#define PA_VEHICLE_IN_H

/**
 * @file pa_vehicle_in.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides the PA vehicle macros for the GDSR tracker
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "fbk_macros.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "PRNDL_state_T.h"

/*============================================================================*\
* Mapping functions
\*============================================================================*/

#ifdef __GNUC__
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static Pa_Veh_Prndl_State_T Pa_Gdsr_Map_Prndl_State(PRNDL_state_T gdsr_prndl_state) __attribute__((unused));
#endif

static Pa_Veh_Prndl_State_T Pa_Gdsr_Map_Prndl_State(PRNDL_state_T gdsr_prndl_state)
{
   Pa_Veh_Prndl_State_T pa_prndl_state;

   switch (gdsr_prndl_state)
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
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Length(p_context) ((float32_T) (p_context)->p_vehicle_data->host_vehicle_length)

/**
 * @brief Returns the width of the ego vehicle in [m].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Width(p_context) ((float32_T) (p_context)->p_vehicle_data->host_vehicle_width)

/**
 * @brief Returns the rear axle position of the ego vehicle in [m].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Rear_Axle_Position(p_context) ((float32_T) (p_context)->p_vehicle_data->rear_axle_position)

/**
 * @brief Returns the rear axle position of the ego vehicle in [m].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Wheelbase(p_context) ((float32_T) (p_context)->p_vehicle_data->wheelbase)

/**
 * @brief Returns hosts speed signal in [m/s].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Speed(p_context) ((float32_T) (p_context)->p_vehicle_data->speed)

/**
 * @brief Returns hosts steering angle in [rad].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Steering_Angle(p_context) ((float32_T) (p_context)->p_vehicle_data->steering_angle)

/**
 * @brief Returns hosts yawrate in [rad / s]. Turn to right shall be positive
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Yawrate(p_context) ((float32_T) (p_context)->p_vehicle_data->yawrate)

/**
 * @brief Returns hosts longitudinal velocity in [m/s].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Long_Vel(p_context) ((float32_T) (p_context)->p_vehicle_data->vcs_long_vel)

/**
 * @brief Returns hosts longitudinal acceleration in [m/s^2].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Long_Acc(p_context) ((float32_T) (p_context)->p_vehicle_data->vcs_long_acc)

/**
 * @brief Returns hosts lateral acceleration in [m/s^2].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Lat_Acc(p_context) ((float32_T) (p_context)->p_vehicle_data->vcs_lat_acc)

/**
 * @brief Returns hosts position of gear.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Prndl(p_context) (Pa_Gdsr_Map_Prndl_State((p_context)->p_vehicle_data->prndl))

/**
 * @brief Returns lane width received from vehicle in [m].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Lane_Width(p_context) ((float32_T) (p_context)->p_vehicle_data->lane_width_external)

/**
 * @brief Returns lane center offset received from vehicle in [m].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Lane_Center_Offset(p_context) ((float32_T) (p_context)->p_vehicle_data->lane_center_offset_external)

/**
 * @brief Returns turn signal.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Turn_Signal(p_context) ((uint8_t) (p_context)->p_vehicle_data->turn_signal)

/**
 * @brief Returns curvature of the host path in [1/m].
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Host_Curvature(p_context) ((float32_T) (p_context)->p_vehicle_data->host_curvature_fast)

/**
 * @brief Returns reverse gear flag.
 *
 * @SDD{}
 */
/* coverity[misra_c_2012_rule_2_5_violation][External use by features is intended] */
#define Pa_Veh_Get_Reverse_Gear_Flag(p_context) (boolean_T)(Fbk_Is_True((p_context)->p_vehicle_data->f_reverse_gear))

#endif /* PA_VEHICLE_IN_H */
