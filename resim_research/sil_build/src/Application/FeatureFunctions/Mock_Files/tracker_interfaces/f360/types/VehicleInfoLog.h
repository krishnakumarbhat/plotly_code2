#ifndef VEHICLE_INFO_LOG_T_H
#define VEHICLE_INFO_LOG_T_H
/*===========================================================================*\
* FILE: VehicleInfoLog.h
*===========================================================================
* Copyright 2022 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains the F360 Tracker Vehicle Information
*
\*===========================================================================*/

#include "f360_reuse.h"
#include "T360_Types.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/

#define VEHICLE_INFO_LOG_STREAM_NUM 4
#define VEHICLE_INFO_LOG_STREAM_VERSION 5

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

typedef struct Vehicle_Info_Log_Tag
{
   // Time
   int32_t vehicle_index;    //!< index of the current vehicle iteration.
   float global_time_sync_s; //!< [s] Global clock = Tracker PC clock + global_time_sync_s
   float timestamp_s;        //!< [s] Timestamp of the raw vehicle data

   // Correction parameters
   float align_angle_az_rad[NUM_TOTAL_RADAR_SENSORS]; //!< [rad] azimuth correction angle used to correct the azimuth angle (az =
                                                      //!< az_raw - align_angle_az_rad)
   float align_angle_el_rad[NUM_TOTAL_RADAR_SENSORS]; //!< [rad] elevation correction angle used to correct the elevation angle (el
                                                      //!< = el_raw - align_angle_el_rad)
   float speed_correction_factor; //!< speed correction factor used to correct the raw_speed (speed = raw_speed * (1 -
                                  //!< speed_correction_factor)

   // Host props
   float world_x;        //!< [m] x-position of host vehicle (or VCS origin of host vehicle) in world coordinate
   float world_y;        //!< [m] y-position of host vehicle (or VCS origin of host vehicle) in world coordinate
   float heading;        //!< [rad] heading angle at VCS original in world coordinate (speed = raw_speed * speed_correction_factor)
   float delta_pointing; //!< [rad] Delta for host pointing angle between previous and current tracker iteration
   float delta_position_x; //!< [m] Delta x-position of host vehicle between previous and current tracker iteration
   float delta_position_y; //!< [m] Delta y-position of host vehicle between previous and current tracker iteration

   // Host
   float speed; //!< [m/s] corrected signed speed of host at center of rear axis (speed = raw_speed * speed_correction_factor)
   float acceleration;              //!< [m/s^2] signed acceleration of host at center of rear axis
   float curvature_rear;            //!< [m] curvature at center of rear axis
   float vcs_speed;                 //!< [m/s] signed speed of host at VCS origin
   float vcs_lat_acceleration;      //!< [m/s^2] lateral acceleration of host at VCS origin
   float vcs_long_acceleration;     //!< [m/s^2] longitudinal acceleration of host at VCS origin
   float vcs_sideslip;              //!< [rad] angle between VCS longitudinal direction and velocity vector of vcs_speed
   float rear_cornering_compliance; //!< [rad*m/s*s] The rear cornering compliance

   // Host raw
   float raw_speed;               //!< [m/s] signed speed of host at center of rear axis provided by vehicle
   float steering_angle_rad;      //!< [rad] vehicle steering angle, (+) increase clockwise to the right
   float raw_yaw_rate_rad;        //!< [rad/s] yaw-rate of host vehicle, a turn to the right is positive
   float dist_rear_axle_to_vcs_m; //!< [m] distance bewteen center of rear axis to VCS origin
   uint8_t prndl; //!< [0-6] (enum F360_PRNDL_STATE) Current gear selected; 0-PARK, 1-REVERSE, 2-NEUTRAL, 3-DRIVE, 4-FOURTH,
                  //!< 5-THIRD, 6-LOW
   uint8_t f_reverse_gear;    //!< flag indicating vehicle is in reverse
   uint8_t f_trailer_present; //!< flag indicating a trailer is connected to host
   uint8_t reserved[1];

} Vehicle_Info_Log_T;


#endif /* VEHICLE_INFO_LOG_T_H */
