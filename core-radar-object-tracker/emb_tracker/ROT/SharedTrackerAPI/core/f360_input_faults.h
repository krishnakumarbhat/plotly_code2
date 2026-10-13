#ifndef F360_INPUT_FAULTS_VARIANT_A_H
#define F360_INPUT_FAULTS_VARIANT_A_H
/*===================================================================================*\
 * FILE: f360_input_faults.h
 *====================================================================================
 * Copyright (C) 2026 Aptiv. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains F360_Input_Faults_T structure declaration
 *
 * ABBREVIATIONS:
 *  None
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWx_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *
 * DEVIATIONS FROM STANDARDS:
 *   None.
 *
\*==========================================================================================*/

#include "f360_constants.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
typedef struct Core_Info_Faults_Tag
{
   bool time_us_no_increase;
   bool cnt_loops_no_increase;
   bool elapsed_time_below_lower_limit;
   bool elapsed_time_above_upper_limit;
} Core_Info_Faults_T;

typedef struct Host_Info_Faults_Tag
{
   bool vehicle_index_no_increase;
   bool host_speed_invalid;
   bool host_yawrate_invalid;
   bool host_longitudinal_acceleration_invalid;
   bool host_lateral_acceleration_invalid;
   uint8_t unused[3];
} Host_Info_Faults_T;

typedef struct Raw_Detection_Faults_Tag
{
   bool range_is_invalid;
   bool range_rate_is_invalid;
   bool azimuth_is_invalid;
   bool elevation_is_invalid;
   bool sensor_id_is_invalid;
   bool det_id_is_invalid;
   bool num_dets_is_invalid;
   uint8_t unused[1];
} Raw_Detection_Faults_T;

typedef struct Radar_Sensor_Faults_Tag
{
   bool look_index_no_increase;
   bool sensor_vs_tracker_timestamp_divergence;
   bool look_id_is_invalid;
   bool boresight_angle_is_invalid;
   bool num_dets_is_invalid;
   uint8_t unused[3];
} Radar_Sensor_Faults_T;

typedef struct Radar_Sensor_Calib_Faults_Tag
{
   bool mounting_pos_is_invalid;
   bool polarity_is_invalid;
   bool boresight_angle_is_invalid;
   bool fov_az_is_invalid;
   bool fov_el_is_invalid;
   bool range_limit_is_invalid;
   bool v_wrapping_is_invalid;
   bool min_aliased_range_rate_is_invalid;
} Radar_Sensor_Calib_Faults_T;

typedef struct Input_Faults_Tag
{
   Host_Info_Faults_T host_info;
   Core_Info_Faults_T core_info;
   Raw_Detection_Faults_T raw_detection;
   Radar_Sensor_Faults_T sensors[MAX_NUMBER_OF_SENSORS];
   Radar_Sensor_Calib_Faults_T sensors_calibs[MAX_NUMBER_OF_SENSORS];
} Input_Faults_T;

static_assert(8 == sizeof(Host_Info_Faults_T), "sizeof(Host_Info_Faults_T) not as expected. Remember to align padding if needed");
static_assert(4 == sizeof(Core_Info_Faults_T), "sizeof(Core_Info_Faults_T) not as expected. Remember to align padding if needed");
static_assert(8 == sizeof(Raw_Detection_Faults_T),
              "sizeof(Raw_Detection_Fault_T) not as expected. Remember to align padding if needed");
static_assert(8 == sizeof(Radar_Sensor_Faults_T),
              "sizeof(Radar_Sensor_Faults_T) not as expected. Remember to align padding if needed");
static_assert(8 == sizeof(Radar_Sensor_Calib_Faults_T),
              "sizeof(Radar_Sensor_Calib_Faults_T) not as expected. Remember to align padding if needed");
static_assert((20 + 16 * MAX_NUMBER_OF_SENSORS) == sizeof(Input_Faults_T),
              "sizeof(Input_Faults_T) not as expected. Remember to align padding if needed");
} // namespace f360_variant_A

#endif
