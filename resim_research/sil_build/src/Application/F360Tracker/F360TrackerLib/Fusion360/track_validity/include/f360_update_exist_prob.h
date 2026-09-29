/*===================================================================================*\
* FILE: f360_update_exist_prob.h
*====================================================================================
*Copyright (C) 2017 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration for Update_Existence_Probability()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_UPDATEEXISTPROB_H
#define F360_UPDATEEXISTPROB_H

#include "f360_object_track.h"
#include "f360_radar_sensor_props.h"
#include "f360_radar_sensor.h"
#include "f360_timing_info.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "rspp_detection.h"

namespace f360_variant_A
{
   void Update_Existence_Probability(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info);
}
#endif
