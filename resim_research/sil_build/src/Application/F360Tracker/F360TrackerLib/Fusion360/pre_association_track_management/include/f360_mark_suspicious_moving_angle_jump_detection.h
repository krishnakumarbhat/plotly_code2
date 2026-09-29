/*===========================================================================*\
* FILE: f360_mark_suspicious_moving_angle_jump_detection.h
*============================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration for Mark_Suspicious_Moving_Angle_Jump_Detection()
*   and related functionality for detecting angle jumps from moving objects.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_MARK_SUSPICIOUS_MOVING_ANGLE_JUMP_DETECTION_H
#define F360_MARK_SUSPICIOUS_MOVING_ANGLE_JUMP_DETECTION_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_host.h"

namespace f360_variant_A
{
   void Mark_Suspicious_Moving_Angle_Jump_Detection(
      const F360_Radar_Sensor_T& sensor,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T& det_prop);

   void Process_Moving_Angle_Jump_Detections(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]);
}

#endif
