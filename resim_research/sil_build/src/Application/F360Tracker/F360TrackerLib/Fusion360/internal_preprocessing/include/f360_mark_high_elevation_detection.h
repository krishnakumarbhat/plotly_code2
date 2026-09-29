#ifndef F360_MARK_HIGH_ELEVATION_DETECTION_H
#define F360_MARK_HIGH_ELEVATION_DETECTION_H
/*===========================================================================*\
* FILE: f360_mark_high_elevation_detection.h
*============================================================================
* Copyright (C) 2023 Aptiv. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "rspp_detection.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Mark_High_Elevation_Detection(
      const F360_Radar_Sensor_T& sensor,
      const F360_Calibrations_T& f360_calib,
      const float32_t host_vcs_speed,
      const rspp_variant_A::RSPP_Detection_T& detection,
      F360_Detection_Props_T& detection_prop
   );
}
#endif
