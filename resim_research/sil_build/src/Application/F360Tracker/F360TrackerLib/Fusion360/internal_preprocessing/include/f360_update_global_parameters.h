/*===================================================================================*\
 * FILE: f360_update_global_parameters.h
 *====================================================================================
 * Copyright (C) 2019-2023 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * This file contains the function definitions for Update_Global_Parameters()
 *
 * ABBREVIATIONS:
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [06-Sep-2020]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
 \*==========================================================================================*/
#ifndef F360_UPDATE_GLOBAL_PARAMETERS_H
#define F360_UPDATE_GLOBAL_PARAMETERS_H

#include "f360_constants.h"
#include "f360_radar_sensor.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_globals.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Update_Global_Parameters(
      const F360_Host_T& host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibrations,
      F360_Globals_T& globals,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Calculate_Shrinked_FOV_Normals(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calibrations,
      F360_Globals_T& globals);

   void Check_Sensor_Configuration(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Globals_T& globals);

   void Calculate_Four_Corner_Sensor_Config_Position_Bounds(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       F360_Globals_T& globals);

      void Calculate_Average_Sensor_Position(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       F360_Globals_T& globals);
}
#endif

