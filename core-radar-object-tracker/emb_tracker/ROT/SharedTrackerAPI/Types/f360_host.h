#ifndef F360_HOST_VARIANT_A_H
#define F360_HOST_VARIANT_A_H
/*===================================================================================*\
* FILE: f360_host.h
*====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
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
/******************************************************************************
 * Name:  F360_Host_Type
 *   Enumeration defining the type of host vehicle.
 ******************************************************************************/
typedef enum F360_Host_Type_Tag : uint8_t
{
   F360_HOST_TYPE_PASSENGER_VEHICLE  = 0,
   F360_HOST_TYPE_COMMERCIAL_VEHICLE = 1
} F360_Host_Type;

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/
/******************************************************************************
 * Name:  F360_Host_T
 *   Structure containing host vehicle state and dynamics information.
 *
 * Struct Members:
 *   vehicle_index               - Index that increments every time the vehicle state estimator is run.
 *   speed                       - [m/s] Compensated speed measured on the host rear axle
 *   vcs_speed                   - [m/s] Compensated speed in VCS coordinates.
 *   acceleration                - [m/s^2] Acceleration of host vehicle.
 *   vcs_lat_acceleration        - [m/s^2] Lateral acceleration component in VCS.
 *   vcs_long_acceleration       - [m/s^2] Longitudinal acceleration component in VCS.
 *   yaw_rate_rad                - [rad] Yaw rate in radians, compensated for the bias in yaw rate.
 *   vcs_sideslip                - [rad] Side slip in VCS coordinates.
 *   curvature_rear              - [1/m] Curvature on the host rear axle.
 *   dist_rear_axle_to_vcs_m     - [m] Distance from the front bumper to the rear axle.
 *   vehicle_length              - [m] Length of the host vehicle.
 *   vehicle_width               - [m] Width of the host vehicle.
 *   rear_cornering_compliance   - [rad*m/s^2] Rear cornering compliance of the vehicle.
 *   speed_correction_factor     - [-] Compensation factor for the raw speed, as computed in the vehicle state estimator.
 *   host_type                   - [-] Describes what type the ego vehicle is.
 *   f_trailer_presence_hardware - 1: trailer presence detected by the host vehicle electronically
 *   speed_qf                    - [F360_QUALITY_FACTOR]: Quality factor for speed measurement.
 *   yaw_rate_qf                 - [F360_QUALITY_FACTOR]: Quality factor for yaw rate measurement.
 *   lat_accel_qf                - [F360_QUALITY_FACTOR]: Quality factor for lateral acceleration measurement.
 *   long_accel_qf               - [F360_QUALITY_FACTOR]: Quality factor for longitudinal acceleration measurement.
 *   enum F360_QUALITY_FACTOR: 0 - UNDEF, 1 - TEMP_UNDEF, 2 - INACCURATE, 3 - ACCURATE
 ******************************************************************************/
typedef struct F360_Host_Tag
{
   uint32_t vehicle_index;

   float32_t speed;
   float32_t vcs_speed;
   float32_t acceleration;
   float32_t vcs_lat_acceleration;
   float32_t vcs_long_acceleration;
   float32_t yaw_rate_rad;
   float32_t vcs_sideslip;
   float32_t curvature_rear;
   float32_t dist_rear_axle_to_vcs_m;
   float32_t vehicle_length;
   float32_t vehicle_width;
   float32_t rear_cornering_compliance;
   float32_t speed_correction_factor;
   F360_Host_Type host_type;
   bool f_trailer_presence_hardware;
   uint8_t speed_qf;
   uint8_t yaw_rate_qf;
   uint8_t lat_accel_qf;
   uint8_t long_accel_qf;

   uint8_t padding[2];
} F360_Host_T;

static_assert(64 == sizeof(F360_Host_T), "sizeof(F360_Host_T) not as expected. Remember to align padding if needed");
} // namespace f360_variant_A

#endif
