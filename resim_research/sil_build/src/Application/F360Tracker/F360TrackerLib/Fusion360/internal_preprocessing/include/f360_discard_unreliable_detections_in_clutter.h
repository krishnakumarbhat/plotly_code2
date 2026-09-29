/*===================================================================================*\
* FILE: f360_discard_unreliable_detections_in_clutter.h
*====================================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of function Discard_Unreliable_Detections_In_Clutter.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards" [06-Sep-2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef f360_DISCARD_UNRELIABLE_DETECTIONS_IN_CLUTTER_H
#define f360_DISCARD_UNRELIABLE_DETECTIONS_IN_CLUTTER_H

#include "f360_radar_sensor.h"
#include "f360_detection_props.h"
#include "f360_tracker_info.h"
#include "rspp_detection.h"

namespace f360_variant_A
{
   void Discard_Unreliable_Detections_In_Clutter(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Radar_Sensor_T& sensor,
      F360_Detection_Props_T& det_prop
   );
}
#endif
