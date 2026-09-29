/*===================================================================================*\
* FILE: f360_host_raw.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
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
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_HOST_RAW_VARIANT_A_H
#define F360_HOST_RAW_VARIANT_A_H

#include "f360_reuse.h"
#include "T360_Types.h"

namespace f360_variant_A
{
   typedef struct F360_Host_Raw_Tag
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
      uint8_t prndl;                  // PRNDL states
      uint8_t reverse_gear;           // TRUE when vehicle is in reverse
      bool f_trailer_presence_hardware;         // 1: trailer presence detected by the host vehicle electronically

      // Quality flags
      uint8_t speed_qf;
      uint8_t yaw_rate_qf;
      uint8_t steering_wheel_angle_qf;
      uint8_t road_wheel_angle_qf;
      uint8_t lat_accel_qf;
      uint8_t long_accel_qf;

      uint8_t padding[3];
   } F360_Host_Raw_T;

   static_assert(sizeof(F360_Host_Raw_T) == 44, "F360_Host_Raw_T: Wrong size.");
}

#endif
