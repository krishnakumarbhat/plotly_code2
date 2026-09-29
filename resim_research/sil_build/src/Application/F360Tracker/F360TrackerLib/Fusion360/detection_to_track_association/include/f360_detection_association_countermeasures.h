/*===================================================================================*\
* FILE: f360_detection_association_countermeasures.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations of Detection_Association_Countermeasures() and related
* support functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_DETECTION_ASSOCIATION_COUNTERMEASURES_H
#define F360_DETECTION_ASSOCIATION_COUNTERMEASURES_H

#include "f360_tracker_info.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Detection_Association_Countermeasures(
      const F360_Tracker_Info_T & tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calibrations,
      const F360_Host_T & host,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );
   void Deassociate_And_Count_Double_Bounce_Detections(
       F360_Object_Track_T& object,
       F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );

   void Deassociate_Stationary_Range_Rate_Outlier_Dets(
      const rspp_variant_A::RSPP_Detection_T(&dets_raw)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS]
   );

   void Deassociate_Suspected_Ground_Detections(
      const rspp_variant_A::RSPP_Detection_T(&dets_raw)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS]
   );

   void Deassociate_Ambiguous_Detections(
      F360_Object_Track_T& obj,
      F360_Detection_Props_T(&dets_prop)[MAX_NUMBER_OF_DETECTIONS]);
   
   void Deassociate_Range_Rate_Outliers_Laterally_Far_From_Object(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t k_min_range_rate_error_threshold,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object);

   void Calc_Percentage_Of_Assoc_Dets(
      F360_Object_Track_T& object);
}
#endif
