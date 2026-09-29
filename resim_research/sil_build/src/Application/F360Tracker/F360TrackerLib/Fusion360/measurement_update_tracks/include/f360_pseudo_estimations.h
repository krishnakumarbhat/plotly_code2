/*===================================================================================*\
* FILE: f360_pseudo_estimations.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Pseudo_Estimations().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_PSEUDO_ESTIMATIONS_H
#define F360_PSEUDO_ESTIMATIONS_H
#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_timing_info.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"
#include "f360_host.h"


namespace f360_variant_A
{
   void Pseudo_Estimations(
      const F360_Calibrations_T & calibrations,
      const F360_Host_T& host,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& obj,
      F360_TRKR_TIMING_INFO_T & timing_info);
}

#endif
