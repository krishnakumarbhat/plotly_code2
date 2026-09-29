/*===========================================================================*\
* FILE: f360_internal_preprocessing.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declaration of Internal_Preprocessing and support functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/


#ifndef INTERNAL_PREPROCESSING_H
#define INTERNAL_PREPROCESSING_H

#include "f360_host.h"
#include "rspp_detection_list.h"
#include "rspp_detection.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_globals.h"
#include "f360_host_props.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor_props.h"
#include "f360_tracker_info.h"
#include "f360_static_env_poly_types.h"
#include "f360_detection_hist.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{

   void Internal_Preprocessing(
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibrations,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Host_Props_T& host_props,
      F360_Globals_T& globals,
      F360_Detection_Hist_T& det_hist,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Radar_Sensor_Props_T(&sensor_props)[MAX_NUMBER_OF_SENSORS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Update_Detections_History(
      const F360_Host_Props_T& host_props,
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Hist_T& det_hist);

   void Update_Detection_Position(
      const float32_t delta_x,
      const float32_t delta_y,
      const float32_t(&rot_mat)[2][2],
      float32_t& det_x_pos,
      float32_t& det_y_pos);

   void Update_Detection_History_Azimuth(
      const F360_Host_Props_T& host_props,
      F360_Detection_Hist_Data_T& detection_history_data);

   void Copy_Detections_Info(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);
}

#endif
