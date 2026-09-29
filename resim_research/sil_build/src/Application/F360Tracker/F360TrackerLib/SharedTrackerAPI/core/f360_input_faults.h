/*===================================================================================*\
* FILE: f360_input_faults.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
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

#ifndef F360_INPUT_FAULTS_VARIANT_A_H
#define F360_INPUT_FAULTS_VARIANT_A_H

#include "../Types/f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   struct Core_Info_Faults_T
   {
      bool time_us_no_increase;
      bool cnt_loops_no_increase;
      bool elapsed_time_below_lower_limit;
      bool elapsed_time_above_upper_limit;
   };

   struct Host_Info_Faults_T
   {
      bool vehicle_index_no_increase;
      bool host_speed_invalid;
      bool host_yawrate_invalid;
      bool host_longitudinal_acceleration_invalid;
      bool host_lateral_acceleration_invalid;
      uint8_t unused[3];
   };

   struct Raw_Detection_Faults_T
   {
       bool range_is_invalid;
       bool range_rate_is_invalid;
       bool azimuth_is_invalid;
       bool elevation_is_invalid;
       uint8_t unused[4];
   };

   struct Radar_Sensor_Faults_T
   {
      bool look_index_no_increase;
      bool sensor_vs_tracker_timestamp_divergence;
      uint8_t unused[6];
   };

   struct Radar_Sensor_Calib_Faults_T
   {
       bool look_index_no_increase;
       bool sensor_vs_tracker_timestamp_divergence;
       bool mounting_pos_is_invalid;
       bool polarity_is_invalid;
       bool boresight_angle_is_invalid;
       uint8_t unused[3];
   };

   struct Input_Faults_T
   {
      Host_Info_Faults_T host_info;
      Core_Info_Faults_T core_info;
      Raw_Detection_Faults_T raw_detection;
      Radar_Sensor_Faults_T sensors[MAX_NUMBER_OF_SENSORS];
      Radar_Sensor_Calib_Faults_T sensors_calibs[MAX_NUMBER_OF_SENSORS];
   };

   static_assert(8 == sizeof(Host_Info_Faults_T), "sizeof(Host_Info_Faults_T) not as expected. Remember to align padding if needed");
   static_assert(4 == sizeof(Core_Info_Faults_T), "sizeof(Core_Info_Faults_T) not as expected. Remember to align padding if needed");
   static_assert(8 == sizeof(Raw_Detection_Faults_T), "sizeof(Raw_Detection_Fault_T) not as expected. Remember to align padding if needed");
   static_assert(8 == sizeof(Radar_Sensor_Faults_T), "sizeof(Radar_Sensor_Faults_T) not as expected. Remember to align padding if needed");
   static_assert(8 == sizeof(Radar_Sensor_Calib_Faults_T), "sizeof(Radar_Sensor_Calib_Faults_T) not as expected. Remember to align padding if needed");
}

#endif
