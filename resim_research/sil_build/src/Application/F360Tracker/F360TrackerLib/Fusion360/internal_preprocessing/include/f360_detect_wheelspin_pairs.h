/*===========================================================================*\
* FILE: f360_detect_wheelspin_pairs.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Detect_Wheel_Spin_Pairs()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef DETECT_WHEELSPIN_PAIRS_H
#define DETECT_WHEELSPIN_PAIRS_H

#include "f360_detection_props.h"
#include "f360_timing_info.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   void Detect_Wheel_Spin_Pairs(
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T (&detProps)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T &timing_info
   );
}

#endif


