/*===================================================================================*\
* FILE: f360_det_is_seen_by_sensor.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of function Is_Det_Seen_By_Sensor();
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_get_wall_time.h"
#include "f360_math_func.h"
#include "f360_det_is_seen_by_sensor.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Det_Seen_By_Sensor()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_seen - flag indicating whether detection is seen by sensor
   *
   * PARAMETERS:
   * const Point& det_pos - detection VCS position
   * const F360_Radar_Sensor_Props_T& radar_sensor_props - radar properties
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   bool Is_Det_Seen_By_Sensor(
      const Point& det_pos, 
      const F360_Radar_Sensor_Props_T& radar_sensor_props,
      const float32_t lat_th_ext)
   {
      bool f_is_seen;
      if (radar_sensor_props.f_object_track_next_to_sensor)
      {
         const float32_t det_abs_lat_pos = std::abs(det_pos.y);
         const float32_t lat_pos_th = std::min(std::abs(radar_sensor_props.next_to_sensor_object_track_min_lat_pos), std::abs(radar_sensor_props.next_to_sensor_object_track_max_lat_pos)) + lat_th_ext;

         f_is_seen = det_abs_lat_pos < lat_pos_th;
      }
      else
      {
         f_is_seen = true;
      }

      return f_is_seen;
   }
}
