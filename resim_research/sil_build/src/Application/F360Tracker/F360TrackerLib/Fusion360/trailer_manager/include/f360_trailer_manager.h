/*===========================================================================*\
 * FILE: f360_trailer_manager.h
 *============================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 *   This file defines trailer manager module to switch algos between commercial vehicles
 *   and passenger vehicles 
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*==========================================================================================*/
#ifndef TRAILER_MANAGER_H
#define TRAILER_MANAGER_H

#include "f360_host.h"
#include "f360_constants.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_radar_sensor_props.h"
#include "f360_timing_info.h"
#include "f360_tracker_info.h"
#include "f360_trailer_detector_flt_fus_output.h"
#include "f360_cvt_runner.h"
#include "f360_pvtrailer_data.h"

namespace f360_variant_A
{
   void Trailer_Manager(const F360_Calibrations_T& calibrations, 
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_CVT_State_T& cvt_state,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Parse_CV_Trailer_Output(const F360_CVT_State_T& cvt_state,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output);

   void Run_Trailer_Manager_Estimators(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&all_detections)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_CVT_State_T& cvt_state,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Reset_Trailer_Manager_Estimators(
      const F360_Host_T& host,
      F360_CVT_State_T& cvt_state,
      F360_PVTrailer_Data_T& pvtrailer_data,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output);

   void Get_Trailer_Manager_Output(
      const F360_Host_T& host,
      const F360_CVT_State_T& cvt_state,
      const F360_PVTrailer_Data_T& pvtrailer_data,
      F360_Trailer_Estimator_Output_T& trailer_estimator_output);

   void Run_Trailer_Related_Countermeasures(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Trailer_Estimator_Output_T& trailer_estimator_output,
      const bool bike_carrier_attached,
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);
}
#endif
