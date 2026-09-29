/*===================================================================================*\
* FILE: f360_track_downselection.h
*====================================================================================
* Copyright (C) 2020 Aptiv. All Rights Reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function signature of Track_Downselection()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): downSelectObkTrks.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_TRACK_DOWNSELECTION_H
#define F360_TRACK_DOWNSELECTION_H

#include "f360_host.h"
#include "f360_calibrations.h"
#include "f360_static_env_poly_types.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_timing_info.h"
#include "f360_constants.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   void Track_Downselection(
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info);
}
#endif
