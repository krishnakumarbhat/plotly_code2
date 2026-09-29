#ifndef FILTER_DETECTIONS_WITH_BAD_AZIMUTH_CONFIDENCE_H
#define FILTER_DETECTIONS_WITH_BAD_AZIMUTH_CONFIDENCE_H
/*===========================================================================*\
* FILE: f360_filter_detections_with_bad_azimuth_confidence.h
*============================================================================
* Copyright (C) 2023 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Filter_Detections_With_Bad_Azimuth_Confidence(
      const F360_Host_T&host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &f360_calib,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );
}
#endif
