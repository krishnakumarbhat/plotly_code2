/**
 * @file fbk_vehicle_validation.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Source file with functions for vehicle data validation.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_validation.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_vehicle_in.h"
#include <assert.h>

/*===========================================================================*\
* Defines
\*===========================================================================*/
/* Host vehicle dimensions */
#define FBK_HOST_LENGTH_MIN_VAL (0.1F)          /* m */
#define FBK_HOST_LENGTH_MAX_VAL (10.0F)         /* m */
#define FBK_HOST_WIDTH_MIN_VAL (0.1F)           /* m */
#define FBK_HOST_WIDTH_MAX_VAL (3.0F)           /* m */
#define FBK_REAR_AXLE_POSITION_MIN_VAL (-10.0F) /* m */
#define FBK_REAR_AXLE_POSITION_MAX_VAL (-0.1F)  /* m */
#define FBK_WHEELBASE_MIN_VAL (0.0F)            /* m  - To be determined*/
#define FBK_WHEELBASE_MAX_VAL (7.0F)            /* m */

/* Vehicle dynamics */
#define FBK_HOST_SPEED_MIN_VAL (-100.0F)    /* m/s, allowing for reverse */
#define FBK_HOST_SPEED_MAX_VAL (300.0F)     /* m/s */
#define FBK_STEERING_ANGLE_MIN_VAL (-10.0F) /* rad */
#define FBK_STEERING_ANGLE_MAX_VAL (10.0F)  /* rad */
#define FBK_YAWRATE_MIN_VAL (-5.0F)         /* rad/s */
#define FBK_YAWRATE_MAX_VAL (5.0F)          /* rad/s */
#define FBK_LONG_VEL_MIN_VAL (-50.0F)       /* m/s */
#define FBK_LONG_VEL_MAX_VAL (300.0F)       /* m/s */
#define FBK_LONG_ACC_MIN_VAL (-50.0F)       /* m/s^2 */
#define FBK_LONG_ACC_MAX_VAL (50.0F)        /* m/s^2 */
#define FBK_LAT_ACC_MIN_VAL (-50.0F)        /* m/s^2 */
#define FBK_LAT_ACC_MAX_VAL (50.0F)         /* m/s^2 */

/* Transmission state */
#define FBK_PRNDL_MAX_VAL (4U) /* Adjust based on Pa_Veh_Prndl_State_T enum */

/* Lane information */
#define FBK_LANE_WIDTH_MIN_VAL (0.0F)           /* m  - To be determined*/
#define FBK_LANE_WIDTH_MAX_VAL (7.0F)           /* m */
#define FBK_LANE_CENTER_OFFSET_MIN_VAL (-10.0F) /* m */
#define FBK_LANE_CENTER_OFFSET_MAX_VAL (10.0F)  /* m */

/* Turn signals */
#define FBK_TURN_SIGNAL_MAX_VAL (2U)

/* Road properties */
#define FBK_CURVATURE_MIN_VAL (-100000.0F) /* 1/m  */
#define FBK_CURVATURE_MAX_VAL (100000.0F)  /* 1/m */

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][External use by features is intended] */
void Fbk_Fill_Vehicle_Information(Fbk_Vehicle_Data_T *p_fbk_vehicle_data, const Pa_Context_T *p_context)
{
   /* Asserts */
   assert(NULL != p_fbk_vehicle_data);
   assert(NULL != p_context);

   /* Fill vehicle data from PA */
   p_fbk_vehicle_data->host_length        = Pa_Veh_Get_Host_Length(p_context);
   p_fbk_vehicle_data->host_width         = Pa_Veh_Get_Host_Width(p_context);
   p_fbk_vehicle_data->rear_axle_position = Pa_Veh_Get_Rear_Axle_Position(p_context);
   p_fbk_vehicle_data->wheelbase          = Pa_Veh_Get_Wheelbase(p_context);
   p_fbk_vehicle_data->host_speed         = Pa_Veh_Get_Host_Speed(p_context);
   p_fbk_vehicle_data->steering_angle     = Pa_Veh_Get_Steering_Angle(p_context);
   p_fbk_vehicle_data->yawrate            = Pa_Veh_Get_Yawrate(p_context);
   p_fbk_vehicle_data->long_vel           = Pa_Veh_Get_Long_Vel(p_context);
   p_fbk_vehicle_data->long_acc           = Pa_Veh_Get_Long_Acc(p_context);
   p_fbk_vehicle_data->lat_acc            = Pa_Veh_Get_Lat_Acc(p_context);
   p_fbk_vehicle_data->prndl              = Pa_Veh_Get_Prndl(p_context);
   p_fbk_vehicle_data->lane_width         = Pa_Veh_Get_Lane_Width(p_context);
   p_fbk_vehicle_data->lane_center_offset = Pa_Veh_Get_Lane_Center_Offset(p_context);
   p_fbk_vehicle_data->turn_signal        = Pa_Veh_Get_Turn_Signal(p_context);
   p_fbk_vehicle_data->curvature          = Pa_Veh_Get_Host_Curvature(p_context);
   p_fbk_vehicle_data->f_reverse          = Pa_Veh_Get_Reverse_Gear_Flag(p_context);
}

/* coverity[HIS_CCM][High CCM in this auto-generated function is expected] */
boolean_T Fbk_Verify_Vehicle_Data_Range(const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T is_valid = FBK_TRUE;

   assert(NULL != p_vehicle_data);

   /* Vehicle dimensions */
   if ((p_vehicle_data->host_length < FBK_HOST_LENGTH_MIN_VAL) || (p_vehicle_data->host_length > FBK_HOST_LENGTH_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->host_width < FBK_HOST_WIDTH_MIN_VAL) || (p_vehicle_data->host_width > FBK_HOST_WIDTH_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->rear_axle_position < FBK_REAR_AXLE_POSITION_MIN_VAL)
       || (p_vehicle_data->rear_axle_position > FBK_REAR_AXLE_POSITION_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->wheelbase < FBK_WHEELBASE_MIN_VAL) || (p_vehicle_data->wheelbase > FBK_WHEELBASE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Vehicle dynamics */
   if ((p_vehicle_data->host_speed < FBK_HOST_SPEED_MIN_VAL) || (p_vehicle_data->host_speed > FBK_HOST_SPEED_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->steering_angle < FBK_STEERING_ANGLE_MIN_VAL) || (p_vehicle_data->steering_angle > FBK_STEERING_ANGLE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->yawrate < FBK_YAWRATE_MIN_VAL) || (p_vehicle_data->yawrate > FBK_YAWRATE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->long_vel < FBK_LONG_VEL_MIN_VAL) || (p_vehicle_data->long_vel > FBK_LONG_VEL_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->long_acc < FBK_LONG_ACC_MIN_VAL) || (p_vehicle_data->long_acc > FBK_LONG_ACC_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->lat_acc < FBK_LAT_ACC_MIN_VAL) || (p_vehicle_data->lat_acc > FBK_LAT_ACC_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Transmission state */
   if ((uint8_t) p_vehicle_data->prndl > FBK_PRNDL_MAX_VAL)
   {
      is_valid = FBK_FALSE;
   }

   /* Lane information */
   if ((p_vehicle_data->lane_width < FBK_LANE_WIDTH_MIN_VAL) || (p_vehicle_data->lane_width > FBK_LANE_WIDTH_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   if ((p_vehicle_data->lane_center_offset < FBK_LANE_CENTER_OFFSET_MIN_VAL)
       || (p_vehicle_data->lane_center_offset > FBK_LANE_CENTER_OFFSET_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Turn signals */
   if (p_vehicle_data->turn_signal > FBK_TURN_SIGNAL_MAX_VAL)
   {
      is_valid = FBK_FALSE;
   }

   /* Road properties */
   if ((p_vehicle_data->curvature < FBK_CURVATURE_MIN_VAL) || (p_vehicle_data->curvature > FBK_CURVATURE_MAX_VAL))
   {
      is_valid = FBK_FALSE;
   }

   /* Boolean fields */
   if ((p_vehicle_data->f_reverse != FBK_FALSE) && (p_vehicle_data->f_reverse != FBK_TRUE))
   {
      is_valid = FBK_FALSE;
   }

   return is_valid;
}
