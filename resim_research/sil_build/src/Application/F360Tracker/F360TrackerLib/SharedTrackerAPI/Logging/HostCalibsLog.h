#ifndef HOST_CALIBS_LOG_T_H
#define HOST_CALIBS_LOG_T_H
/*===================================================================================*\
* FILE: HostCalibsLog.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
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

static const uint8_t HOST_CALIBS_LOG_STREAM_NUM = 12U;
static const uint8_t HOST_CALIBS_LOG_STREAM_VERSION = 3U;

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

typedef struct Host_Calibs_Log_Tag
{
   //Host Calibs
   float dist_rear_axle_to_vcs_m;   // [m] distance bewteen center of rear axis to VCS origin
   float rear_cornering_compliance; // [rad*m/s*s] The rear cornering compliance.
   float steer_gear_ratio;          // Ratio of steering wheel angle to wheel angle
   float wheelbase_m;               // [m] wheelbase in meters
   float understeer_coefficient;    // Understeer coefficient
   float vehicle_width_m;           // [m] Width of the vehicle
   float vehicle_length_m;          // [m] Length of the vehicle
   float cog_x;                     // [m] Center of gravity
   float cog_y;                     // [m] Center of gravity
   float front_wheel_radius_m;      // [m] Wheel radius when vehicle is loaded to GVW
   float front_track_width_m;       // [m] Front Track Width

   // raw host latency
   uint32_t raw_host_signal_latency; // [ms] latency of raw host signal

   // Host configuration
   uint8_t f_enable_internal_reflections_func;         // [-] Flag to enable the internal reflections function using general tuning
   uint8_t f_enable_internal_reflections_func_trailer; // [-] Flag to allow the internal reflections function to be enabled when a trailer is connected to host using trailer tuning
   uint8_t host_type;                   // enum F360_HOST_TYPE_PASSENGER_VEHICLE = 0, F360_HOST_TYPE_COMMERCIAL_VEHICLE = 1
   uint8_t reserved;

} Host_Calibs_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Host_Calibs_Log_T, 52U);

#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif

#endif
