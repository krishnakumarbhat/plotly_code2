/*===================================================================================*\
 * FILE: f360_handle_host_vehicle_clutter.h
 *====================================================================================
 * Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * This file contains functions for handling detections flagged as "host vehicle clutter" by raw signal processing
 *
 * ABBREVIATIONS:
 *  HVC host vehicle clutter
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [06-Sep-2020]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
 \*==========================================================================================*/
#ifndef F360_HANDLE_HOST_VEHICLE_CLUTTER_H
#define F360_HANDLE_HOST_VEHICLE_CLUTTER_H

#include "f360_reuse.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_constants.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   void Handle_Host_Vehicle_Clutter(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calib,
      F360_Detection_Props_T &detection_prop
   );
}
#endif

