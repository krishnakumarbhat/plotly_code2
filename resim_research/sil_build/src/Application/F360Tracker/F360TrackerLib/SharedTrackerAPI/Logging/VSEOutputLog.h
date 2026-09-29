#ifndef VSE_OUTPUT_LOG_T_H
#define VSE_OUTPUT_LOG_T_H
/*===================================================================================*\
* FILE: VSEOutputLog.h
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

static const uint8_t VSE_OUTPUT_LOG_STREAM_NUM = 15U;
static const uint8_t VSE_OUTPUT_LOG_STREAM_VERSION = 3U;

#ifndef __TASKING__
#  pragma pack(push)
#  pragma pack(4)
#endif

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

typedef struct VSE_Output_Log_Tag
{
  uint64_t timestamp_us;
  uint32_t veh_index;
  float raw_speed_mps;
  float speed_compensation_factor;
  float filt_veh_speed_over_ground;
  float signed_filt_veh_speed_over_ground;
  float raw_lat_accel;
  float raw_long_accel;
  float raw_yaw_rate_rps;
  float raw_steering_angle_deg;
  float road_wheel_angle_deg;
  float yaw_rate_sa;
  float yaw_rate_raw_bias;
  float comp_yaw_rate_unfiltered;
  float comp_yaw_rate_filtered;
  float curvature_rear_axle;
  float sideslip_rear_axle;
  float vcs_sideslip;
  float vcs_long_velocity;
  float vcs_lat_velocity;
  float sensor_sideslip;
  float sensor_long_velocity;
  float sensor_lat_velocity;
  float k_dist_rear_axle_to_vcs;
  float vcs_lat_accel;
  float vcs_long_accel;
  float accel_rear_axle;
  uint8_t raw_speed_qf;
  uint8_t speed_compensation_factor_qf;
  uint8_t filt_veh_speed_over_ground_qf;
  uint8_t raw_lat_accel_qf;
  uint8_t raw_long_accel_qf;
  uint8_t raw_yaw_rate_qf;
  uint8_t raw_steering_angle_qf;
  uint8_t road_wheel_angle_qf;
  uint8_t yaw_rate_sa_qf;
  uint8_t yaw_rate_bias_qf;
  uint8_t comp_yaw_rate_qf;
  uint8_t vcs_lat_accel_qf;
  uint8_t vcs_long_accel_qf;
  uint8_t stationary;
  uint8_t reserved[2];
} VSE_Output_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************
LOGSIZE_ASSERT(VSE_Output_Log_T, 128U);

#ifndef __TASKING__
#  pragma pack(pop)
#else
#endif

#endif
