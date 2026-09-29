/*===================================================================================*\
* FILE: f360_host.h
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
*   This file contains Host  structure  declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s): defineFusion360Types.m
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_HOST_VARIANT_A_H
#define F360_HOST_VARIANT_A_H

#include "f360_reuse.h"
#include "T360_Types.h"

namespace f360_variant_A
{
   enum F360_Host_Type
   {
      F360_HOST_TYPE_PASSENGER_VEHICLE = 0,
      F360_HOST_TYPE_COMMERCIAL_VEHICLE = 1

   };
   
   typedef struct F360_Host_Tag
   {
      uint32_t vehicle_index;

      float speed;
      float vcs_speed;
      float acceleration;
      float vcs_lat_acceleration;
      float vcs_long_acceleration;
      float yaw_rate_rad;                 // Compensated for yaw_rate bias
      float vcs_sideslip;
      float curvature_rear;
      float dist_rear_axle_to_vcs_m;      // [m] The distance from the rear axle to VCS origin
      float vehicle_length;
      float vehicle_width;
      float rear_cornering_compliance;
      float speed_correction_factor;
      F360_Host_Type host_type;
      bool f_trailer_presence_hardware; // 1: trailer presence detected by the host vehicle electronically

      // enum F360_QUALITY_FACTOR: 0 - UNDEF, 1 - TEMP_UNDEF, 2 - INACCURATE, 3 - ACCURATE
      uint8_t speed_qf;
      uint8_t yaw_rate_qf;
      uint8_t lat_accel_qf;
      uint8_t long_accel_qf;

      uint8_t padding[3];
   } F360_Host_T;

   static_assert(68 == sizeof(F360_Host_T), "sizeof(F360_Host_T) not as expected. Remember to align padding if needed");
}

#endif
