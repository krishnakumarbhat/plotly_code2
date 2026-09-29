/*===================================================================================*\
* FILE: f360_define_simplified_dead_zone.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of Define_Simplified_Dead_Zone() function.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_DEFINE_SIMPLIFIED_DEAD_ZONE
#define F360_DEFINE_SIMPLIFIED_DEAD_ZONE

#include "f360_interval.h"
#include "f360_radar_sensor.h"
namespace f360_variant_A
{
   Interval<float32_t> Define_Simplified_Dead_Zone(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]);
}

#endif
