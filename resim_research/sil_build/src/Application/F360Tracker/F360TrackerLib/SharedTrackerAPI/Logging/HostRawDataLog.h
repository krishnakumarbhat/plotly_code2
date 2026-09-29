#ifndef HOST_RAW_DATA_LOG_T_H
#define HOST_RAW_DATA_LOG_T_H
/*===================================================================================*\
* FILE: HostRawDataLog.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* 
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_reuse.h"
#include "T360_Types.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/

static const uint8_t HOST_RAW_DATA_LOG_STREAM_NUM = 13U;
static const uint8_t HOST_RAW_DATA_LOG_STREAM_VERSION = 3U;

#ifndef __TASKING__
#  pragma pack(push)
#  pragma pack(4)
#endif

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

typedef struct Host_Raw_Data_Log_Tag
{
   // Time
   float global_time_sync_s; // [s] Global clock = Tracker PC clock + global_time_sync_s
   float timestamp_s;        // [s] TImestamp when the vehicle data was read

   // Veh state
   float raw_speed;                // [m/s] vehicle speed
   float raw_yaw_rate_rad;         // [rad] vehicle yaw rate, (+) increase clockwise to the right
   float steering_wheel_angle_rad; // [rad] vehicle steering wheel angle, (+) increase clockwise to the right
   float road_wheel_angle_rad;     // [rad] vehicle raod wheel angle, (+) increase clockwise to the right
   float lat_accel;                // [m/s^2] lateral acceleration
   float long_accel;               // [m/s^2] longitudinal acceleration
   float speed_correction_factor;  // [NA] factor used to correct raw_speed. raw_speed * speed_correction_factor = true_speed
   uint8_t reverse_gear;           // TRUE when vehicle is in reverse
   uint8_t prndl;                  // [NA] Vehicle gear selection. Values defined in enum F360_PRNDL_STATE
   uint8_t f_trailer_presence_hardware;         // TRUE when a trailer is connected to host

   // Quality flags
   uint8_t speed_qf;                    // [NA] speed quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t yaw_rate_qf;                 // [NA] yaw rate quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t steering_wheel_angle_qf;     // [NA] steering wheel angle quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t road_wheel_angle_qf;         // [NA] road wheel angle quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t lat_accel_qf;                // [NA] lateral acceleration quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t long_accel_qf;               // [NA] longitudinal acceleration quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t speed_correction_factor_qf;  // [NA] speed correction factor quality factor. Values defined in enum F360_QUALITY_FACTOR
   uint8_t padding[2];                  // [NA] Padding bytes

} Host_Raw_Data_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Host_Raw_Data_Log_T, 48U);

#ifndef __TASKING__
#  pragma pack(pop)
#else
#endif

#endif
