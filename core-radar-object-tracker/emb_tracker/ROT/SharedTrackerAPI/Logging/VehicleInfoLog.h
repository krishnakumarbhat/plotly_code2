#ifndef VEHICLE_INFO_LOG_T_H
#define VEHICLE_INFO_LOG_T_H
/*===========================================================================*\
 * FILE: VehicleInfoLog.h
 *===========================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 */

#include "T360_Types.h"
#include "f360_reuse.h"

/*===========================================================================*\
* Exported local (file scope) Constants
\*===========================================================================*/

#define VEHICLE_INFO_LOG_STREAM_NUM     4U
#define VEHICLE_INFO_LOG_STREAM_VERSION 10U

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
   #pragma warning(push)
   #pragma warning(error:4820)
#elif 0
   #pragma GCC diagnostic push
   #pragma GCC diagnostic error "-Wpadded"
#endif

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/

/*!
 * Per-iteration data the tracker receives from the surrounding wrapper:
 * post-VSE host state, plus DRA-estimated sensor alignment and DRA
 * speed-correction factor. Recorded together so VSE-bypass resim replay
 * can run from this log paired with Sync_Info_Log_T.
 *
 * Fields authoritative in other streams and NOT stored here:
 *   - Timing / iteration indices     -> Sync_Info_Log_T (stream #1)
 *   - World-frame host pose / deltas -> F360_Host_Props_Log_T (stream #14)
 *   - Raw vehicle-bus signals / QFs  -> Host_Raw_Data_Log_T (stream #13)
 */
typedef struct Vehicle_Info_Log_Tag
{
   // Time retained for VehicleInfo-only log replay when Sync_Info_Log_T is absent.
   int64_t global_time_sync_us; //!< [us, signed delta] external_clock = Tracker PC clock + global_time_sync_us; negative when PC
                                //!< clock is ahead
   uint64_t timestamp_us;       //!< [us] Timestamp of the raw vehicle data

   // DRA outputs (alignment estimates)
   float vacs_boresight_az_estimated[NUM_TOTAL_RADAR_SENSORS]; //!< [rad] sensor misalignment corrected azimuth boresight angle in
                                                               //!< vehicle aligned coordinate system
   float vacs_boresight_el_estimated[NUM_TOTAL_RADAR_SENSORS]; //!< [rad] sensor misalignment corrected elevation boresight angle
                                                               //!< in vehicle aligned coordinate system
   float speed_correction_factor; //!< speed correction factor used to correct the raw_speed (speed = raw_speed *
                                  //!< speed_correction_factor)

   // Post-VSE host state
   float speed;                     //!< [m/s] corrected signed speed of host at center of rear axis
   float acceleration;              //!< [m/s^2] signed acceleration of host at center of rear axis
   float curvature_rear;            //!< [m] curvature at center of rear axis
   float vcs_speed;                 //!< [m/s] signed speed of host at VCS origin
   float vcs_lat_acceleration;      //!< [m/s^2] lateral acceleration of host at VCS origin
   float vcs_long_acceleration;     //!< [m/s^2] longitudinal acceleration of host at VCS origin
   float vcs_sideslip;              //!< [rad] angle between VCS longitudinal direction and velocity vector of vcs_speed
   float rear_cornering_compliance; //!< [rad*m/s*s] The rear cornering compliance
   float filtered_yaw_rate_rad;     //!< [rad/s] filtered yaw rate that tracker consumed
   float dist_rear_axle_to_vcs_m;   //!< [m] distance between center of rear axis to VCS origin

   // Vehicle state quality factors. Encoding (F360_QUALITY_FACTOR enum):
   //   0 = F360_QF_UNDEFINED, 1 = F360_QF_TEMP_UNDEFINED,
   //   2 = F360_QF_INACCURATE, 3 = F360_QF_ACCURATE (highest quality).
   uint8_t speed_qf;      //!< [0-3] Over the ground speed quality factor
   uint8_t yaw_rate_qf;   //!< [0-3] Yaw rate quality factor
   uint8_t lat_accel_qf;  //!< [0-3] Lateral acceleration quality factor
   uint8_t long_accel_qf; //!< [0-3] Longitudinal acceleration quality factor

   uint8_t f_trailer_presence_hardware; //!< [0/1] 1: trailer presence detected by the host vehicle electronically
   uint8_t reserved[7];
} Vehicle_Info_Log_T;

//  **********************************************************************************************************
//  ************************ WARNING!!!!!! *******************************************************************
//  **********************************************************************************************************
//  The following compile-time assertion fails if the size of the log stream type does not equal the expected
//  size.  If it fails, then the size must be corrected AND the Stream LogVersion must changed.
//  If the version in this Stream LogVersion is NOT changed, then DV tool will not be able to decode the stream!
//  **********************************************************************************************************

LOGSIZE_ASSERT(Vehicle_Info_Log_T, 168U);

// Restore MSVC and GCC warning settings
#if defined _MSC_VER
   #pragma warning(pop)
#elif 0
   #pragma GCC diagnostic pop
#endif

#endif
