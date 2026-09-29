/*===========================================================================*\
* FILE: f360_configure_rdot_interval_compatability.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declarations of functions that configure rdot interval compatibility.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_CONFIGURE_RDOT_INTERVAL_COMPATIBILITY_H
#define F360_CONFIGURE_RDOT_INTERVAL_COMPATIBILITY_H

#include "f360_reuse.h"
#include "f360_globals.h"
#include "f360_constants.h"
#include "f360_tracker_info.h"
#include "f360_radar_sensor.h"
#include "f360_timing_info.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Configure_Rdot_Interval_Compatibility(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T &globals,
      const F360_Calibrations_T &calibs,
      F360_Tracker_Info_T &tracker_info,
      F360_TRKR_TIMING_INFO_T &timing_info
   );
}
#endif 
