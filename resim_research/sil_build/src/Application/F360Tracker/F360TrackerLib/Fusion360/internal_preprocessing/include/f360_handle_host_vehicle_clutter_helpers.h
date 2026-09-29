/*===================================================================================*\
 * FILE: f360_handle_host_vehicle_clutter_helpers.h
 *====================================================================================
 * Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential  Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * This file contains declaration of supporting functions used in HVC detections handling.
 *
 * ABBREVIATIONS:
 * HVC host vehicle clutter
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [06-Sep-2020]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
 \*==========================================================================================*/
#ifndef F360_HANDLE_HOST_VEHICLE_CLUTTER_HELPERS_H
#define F360_HANDLE_HOST_VEHICLE_CLUTTER_HELPERS_H

#include "f360_point.h"
#include "f360_constants.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   struct Pos_Limits_T
   {
      float32_t min_lat_pos;
      float32_t max_lat_pos;
      float32_t min_long_pos;
      float32_t max_long_pos;
   };

   bool Is_Det_In_HVC_TP_Area(
      const Point& vcs_pos,
      const Pos_Limits_T& hvc_tp_area);

   Pos_Limits_T Get_HVC_TP_Area(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t& max_range_flagging_hvc_dets);
}
#endif

