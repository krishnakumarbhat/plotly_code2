/*===================================================================================*\
* FILE: f360_det_is_seen_by_sensor.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of function Is_Det_Seen_By_Sensor();
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DET_IS_SEEN_BY_SENSOR_H
#define DET_IS_SEEN_BY_SENSOR_H

#include "f360_point.h"
#include "f360_radar_sensor_props.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   bool Is_Det_Seen_By_Sensor(
      const Point& det_pos,
      const F360_Radar_Sensor_Props_T& radar_sensor_props,
      const float32_t lat_th_ext);
}
#endif
