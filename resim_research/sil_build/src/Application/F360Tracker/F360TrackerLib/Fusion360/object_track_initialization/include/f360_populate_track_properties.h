#ifndef F360_POPULATE_TRACK_PROPERTIES_H
#define F360_POPULATE_TRACK_PROPERTIES_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE:  f360_populate_track_properties.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Populate_Track_Properties()
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
#include "f360_cluster.h"
#include "f360_detection_props.h"
#include "f360_static_env_poly_types.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Populate_Track_Properties(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const F360_Cluster_T& cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Detection_Hist_T& det_hist,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const uint32_t new_unique_id,
      const float32_t init_longvel,
      const float32_t init_latvel,
      F360_Object_Track_T& obj);

   bool Is_Pca_Principal_Dir_Close_To_Heading(
       const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Object_Track_T& obj);

   float32_t Compute_Length_Line(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T& obj);
}

#endif
