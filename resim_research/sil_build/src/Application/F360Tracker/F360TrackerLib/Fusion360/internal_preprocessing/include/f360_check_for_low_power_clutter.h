/*===========================================================================*\
* FILE: f360_check_for_low_power_clutter.h
*============================================================================
* Copyright (C) 2019-2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Check_For_Low_Power_Clutter() and helper functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef CHECK_FOR_LOW_POWER_CLUTTER_H
#define CHECK_FOR_LOW_POWER_CLUTTER_H

#include "f360_host.h"
#include "rspp_detection_list.h"
#include "rspp_detection.h"
#include "f360_radar_sensor.h"
#include "f360_host_props.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor_props.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Check_For_Low_Power_Clutter(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T& tracker_info);

   struct low_power_clutter_thresholds
   {
      float32_t max_rcs;
      float32_t max_snr;
      int8_t detection_az_confid;
      bool f_flr7_sensor;
      bool f_srr7plus_sensor;
      bool f_gen6_sensor;
   };

   bool Is_Det_In_ROI(
      const float32_t min_vcs_longpos_threshold,
      const float32_t max_vcs_longpos_threshold,
      const F360_Detection_Props_T& detection_prop,
      const bool f_check_abs_longpos
   );

   bool Detection_Props_Match_Hypothesis(
      const rspp_variant_A::RSPP_Detection_T& detection,
      const F360_Detection_Props_T& detection_prop,
      const low_power_clutter_thresholds& thresholds
   );

   float Update_Max_Severity_Value(
      const uint32_t(&num_matching_dets)[MAX_NUMBER_OF_SENSORS],
      const uint32_t(&num_matching_inner_dets)[MAX_NUMBER_OF_SENSORS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS]);

   void Check_Radar_Rain_Flag(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info
   );

   void Set_Sensor_Dependent_Thresholds(
      const int8_t sensor_type,
      low_power_clutter_thresholds& thresholds
   );

}
#endif


