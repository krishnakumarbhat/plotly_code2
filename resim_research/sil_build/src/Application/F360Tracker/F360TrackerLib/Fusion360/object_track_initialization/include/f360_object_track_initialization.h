#ifndef F360_OBJECT_TRACK_INITIALIZATION_H
#define F360_OBJECT_TRACK_INITIALIZATION_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE:  f360_object_track_initialization.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Object_Track_Initialization()
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#include "f360_reuse.h"
#include "f360_globals.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_static_env_poly_types.h"
#include "f360_occlusion_types.h"
#include "f360_radar_sensor.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_cluster.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Update_Cluster_In_Clutter_Probability(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]);

   void Object_Track_Initialization(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T& det_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info);
}

#endif
