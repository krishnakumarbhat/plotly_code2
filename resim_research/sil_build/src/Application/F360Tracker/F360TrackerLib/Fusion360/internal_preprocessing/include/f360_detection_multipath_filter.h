#ifndef F360_DETECTION_MULTIPATH_FILTER_H
#define F360_DETECTION_MULTIPATH_FILTER_H
/*===========================================================================*\
* FILE: f360_detection_multipath_filter.h
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
#include "rspp_detection.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Filter_Out_Multipath_Detections(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T& sensor,
      const F360_Calibrations_T& f360_calibrations,
      const uint32_t first_det_list_idx,
      const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   );
}
#endif
