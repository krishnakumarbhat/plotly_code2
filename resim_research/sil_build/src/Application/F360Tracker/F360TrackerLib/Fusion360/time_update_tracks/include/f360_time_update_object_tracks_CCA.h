#ifndef F360_TIME_UPDATE_OBJECT_TRACK_CCA_H
#define F360_TIME_UPDATE_OBJECT_TRACK_CCA_H
/*===================================================================================*\
* FILE:  f360_time_update_object_track_CCA.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_timing_info.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Time_Update_Object_Tracks_CCA(
      const float32_t T,
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calibrations,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_TRKR_TIMING_INFO_T &timing_info);

   void Time_Update_Pointing_Heading_Rate_CCA(
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj);

   void Compute_Process_Noise_Covariance_CCA(
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const float32_t T4,
      const float32_t T5,
      const float32_t(&default_low_speed_Q)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&Q)[STATE_DIMENSION][STATE_DIMENSION]);

   void Compute_Q_For_Slow_Moving_Object(
      const F360_Calibrations_T& calibs,
      const float32_t T,
      const float32_t T2,
      const float32_t T3,
      const float32_t T4,
      const float32_t T5,
      float32_t(&default_low_speed_Q)[STATE_DIMENSION][STATE_DIMENSION]);
}
#endif
