/*===================================================================================*\
* FILE:  f360_time_update_object_track_CTCA.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of function for executing prediction step of the 
* Kalman Filter for the Coordinated Turn Constant Acceleration (CTCA) motion model.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#ifndef F360_TIME_UPDATE_OBJECT_TRACKS_CTCA_H
#define F360_TIME_UPDATE_OBJECT_TRACKS_CTCA_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_timing_info.h"
namespace f360_variant_A
{
   void Time_Update_Object_Track_CTCA(
      const float32_t elapsed_time_s,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calibs,
      F360_TRKR_TIMING_INFO_T &timing_info);

   void Adaptive_Tuning_Of_Proc_Noise_Cov(
      const float32_t T,
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      float32_t(&q_mat_ctca)[STATE_DIMENSION][STATE_DIMENSION]);

   void Saturate_Curvature_Variance(
      const float32_t k_max_curvature_variance,
      float32_t (&obj_errcov)[STATE_DIMENSION][STATE_DIMENSION]);
}
#endif
