/*===================================================================================*\
* FILE:  f360_predict_exist_prob.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of function that is used to time update the existance probability
* of objects
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_PREDICTEXISTPROB_H
#define F360_PREDICTEXISTPROB_H

#include "f360_object_track.h"
#include "f360_radar_sensor.h"
#include "f360_timing_info.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Predict_Existence_Probability(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T& timing_info);

   float32_t Calculate_P_Persist(
      const F360_Object_Track_T &obj,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibs);
}
#endif
