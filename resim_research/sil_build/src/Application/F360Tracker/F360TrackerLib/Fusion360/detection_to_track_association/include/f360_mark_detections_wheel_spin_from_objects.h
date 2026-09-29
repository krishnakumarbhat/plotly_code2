/*===========================================================================*\
* FILE: f360_mark_detections_wheel_spin_from_objects.h
*============================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Mark_Detections_Wheel_Spin_From_Objects() declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_MARK_DETECTIONS_WHEEL_SPIN_FROM_OBJECTS_H
#define F360_MARK_DETECTIONS_WHEEL_SPIN_FROM_OBJECTS_H

#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_bounding_box.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Mark_Detections_Wheel_Spin_From_Objects(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t nr_valid_dets,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T & calibrations,
      F360_Object_Track_T & object_track,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );

   void Determine_Bounding_Box_Extension(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calibrations,
      BoundingBox & box
   );

   bool Is_Det_Wheel_Spin_From_Object(
      const F360_Object_Track_T & object_track,
      const rspp_variant_A::RSPP_Detection_T & det,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T & calibrations,
      const BoundingBox & wheel_spin_zone,
      const F360_Detection_Props_T & det_prop
   );
}
#endif
