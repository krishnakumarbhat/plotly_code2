/*===========================================================================*\
* FILE: f360_detection_double_bounce_detector.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Double_Bounce_Detection_Countermeasure() function declaration
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_DOUBLE_BOUNCE_DETECTION_COUNTERMEASURE_H
#define F360_DOUBLE_BOUNCE_DETECTION_COUNTERMEASURE_H

#include "f360_reuse.h"
#include "rspp_detection.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Double_Bounce_Detection_Countermeasure(
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t num_dets,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calibs);
}
#endif
