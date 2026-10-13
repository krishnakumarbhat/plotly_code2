#ifndef F360_HOST_RAW_VARIANT_A_H
#define F360_HOST_RAW_VARIANT_A_H
/*===================================================================================*\
* FILE: f360_host_raw.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains Raw_Host  structure related declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s):  PRNDL_STATE.m
 *
 *   Requirements Document(s):
 *
 *  Applicable Standards (in order of precedence: highest first):
 *  - https://confluence.asux.aptiv.com/spaces/F360Core/pages/129995883/Coding+Guidelines
 *  - ESGW_4-2_PE-SWX_00-01-A01_EN - C++ Coding Standards [20190526]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
typedef struct F360_Host_Raw_Tag
{
   // Time
   int64_t global_time_sync_us; // [us, signed delta] external_clock = Tracker PC clock + global_time_sync_us; negative when PC
                                // clock is ahead
   uint64_t timestamp_us;       // [us] Timestamp when the vehicle data was read

   // Veh state
   float raw_speed;                  // [m/s] vehicle speed
   float raw_yaw_rate_rad;           // [rad] vehicle yaw rate, (+) increase clockwise to the right
   float steering_wheel_angle_rad;   // [rad] vehicle steering wheel angle, (+) increase clockwise to the right
   float road_wheel_angle_rad;       // [rad] vehicle raod wheel angle, (+) increase clockwise to the right
   float lat_accel;                  // [m/s^2] lateral acceleration
   float long_accel;                 // [m/s^2] longitudinal acceleration
   uint8_t prndl;                    // PRNDL states
   uint8_t reverse_gear;             // TRUE when vehicle is in reverse
   bool f_trailer_presence_hardware; // 1: trailer presence detected by the host vehicle electronically

   // Quality flags
   uint8_t speed_qf;
   uint8_t yaw_rate_qf;
   uint8_t steering_wheel_angle_qf;
   uint8_t road_wheel_angle_qf;
   uint8_t lat_accel_qf;
   uint8_t long_accel_qf;

   uint8_t padding[7];
} F360_Host_Raw_T;

static_assert(sizeof(F360_Host_Raw_T) == 56, "F360_Host_Raw_T: Wrong size.");
} // namespace f360_variant_A

#endif
