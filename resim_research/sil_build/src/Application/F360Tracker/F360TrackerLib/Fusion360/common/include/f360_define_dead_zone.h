/*===================================================================================*\
* FILE: f360_define_dead_zone.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Define_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_DEFINE_DEAD_ZONE
#define F360_DEFINE_DEAD_ZONE

#include "f360_dead_zone.h"
#include "f360_calibrations.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   Dead_Zone_T Define_Dead_Zone(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t dead_zone_long_limit_extension);
}

#endif
