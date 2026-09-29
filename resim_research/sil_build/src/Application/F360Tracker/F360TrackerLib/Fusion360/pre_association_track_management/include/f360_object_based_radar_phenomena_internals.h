/*===================================================================================*\
* FILE: f360_object_based_radar_phenomena_internals.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains declarations of functions related to object based 
*    angle jump detector.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_OBJECT_BASED_RADAR_PHENOMENA_INTERNALS_H
#define F360_OBJECT_BASED_RADAR_PHENOMENA_INTERNALS_H

#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"


namespace f360_variant_A
{
   bool Can_Object_Be_A_Reference(
      const F360_Object_Track_T &object_track,
      const float32_t max_object_lateral_distance,
      const float32_t max_pointing_disagreement,
      const float32_t min_confidence_level);

   void Check_Dets_Wrt_Reference(
      const F360_Object_Track_T &reference_track,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibs,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);

   struct Sensor_Angle_Ambiguity_T{
      bool f_sensor_relevant;
      float32_t angle_ambiguity_candidates_rad[MAX_NUM_ANGLE_JUMPS];
   };

   void Determine_Precond_For_Angle_Jump_And_MultiBounce(
      const F360_Radar_Sensor_T &sensor,
      const uint32_t sen_idx,
      const BoundingBox bbox,
      const float32_t longitudinal_margin,
      bool (&relevant_sensors_for_multi_bounce)[MAX_NUMBER_OF_SENSORS],
      Sensor_Angle_Ambiguity_T (&sensors_angle_amb)[MAX_NUMBER_OF_SENSORS]
   );
}
#endif
