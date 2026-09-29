/*===================================================================================*\
 * FILE: f360_handle_host_vehicle_clutter_helpers.cpp
 *====================================================================================
 * Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential  Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * This file contains definition of supporting functions used in HVC detections handling.
 *
 * ABBREVIATIONS:
 * HVC: host vehicle clutter
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [06-Sep-2020]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
 \*==========================================================================================*/

#include "f360_handle_host_vehicle_clutter_helpers.h"
#include <algorithm>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Det_In_HVC_TP_Area()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_in_tp_area - flag indicating whether detection is in HVC TP area.
   *
   * PARAMETERS:
   * const Point& - detection position.
   * const Pos_Limits_T& hvc_tp_area - structure containing information about zone which we consider that HVC are TP.
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
   * Function verifies whether detection is placed in zone that we treat HVC detections as TP.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Is_Det_In_HVC_TP_Area(
      const Point& vcs_pos,
      const Pos_Limits_T& hvc_tp_area)
   {
      const bool f_is_in_tp_area = (hvc_tp_area.min_lat_pos < vcs_pos.y) &&
         (vcs_pos.y < hvc_tp_area.max_lat_pos) &&
         (hvc_tp_area.min_long_pos < vcs_pos.x) &&
         (vcs_pos.x < hvc_tp_area.max_long_pos);

      return f_is_in_tp_area;
   }

   /*===========================================================================*\
   * FUNCTION: Get_HVC_TP_Area()
   *===========================================================================
   * RETURN VALUE:
   * Pos_Limits_T hvc_tp_area - structure containing information about zone which we consider that HVC are TP
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS] - sensors parameters
   * const float32_t& max_range_flagging_hvc_dets - range used to extend zone defined by sensors positions
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
   * Function defines zone in which we treat HVC detections as TP. 
   * Zone is defined using sensors positions.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   Pos_Limits_T Get_HVC_TP_Area(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const float32_t& max_range_flagging_hvc_dets)
   {
      Pos_Limits_T hvc_tp_area{};

      for (uint32_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
      {
         const F360_Radar_Sensor_T& radar = sensors[i];
         if (radar.variable.is_valid)
         {
            hvc_tp_area.min_lat_pos = std::min(hvc_tp_area.min_lat_pos, radar.constant.mounting_position.vcs_position.lateral);
            hvc_tp_area.max_lat_pos = std::max(hvc_tp_area.max_lat_pos, radar.constant.mounting_position.vcs_position.lateral);

            hvc_tp_area.min_long_pos = std::min(hvc_tp_area.min_long_pos, radar.constant.mounting_position.vcs_position.longitudinal);
            hvc_tp_area.max_long_pos = std::max(hvc_tp_area.max_long_pos, radar.constant.mounting_position.vcs_position.longitudinal);
         }
      }

      hvc_tp_area.max_lat_pos = hvc_tp_area.max_lat_pos + max_range_flagging_hvc_dets;
      hvc_tp_area.min_lat_pos = hvc_tp_area.min_lat_pos - max_range_flagging_hvc_dets;

      hvc_tp_area.max_long_pos = hvc_tp_area.max_long_pos + max_range_flagging_hvc_dets;
      hvc_tp_area.min_long_pos = hvc_tp_area.min_long_pos - max_range_flagging_hvc_dets;
      
      return hvc_tp_area;
   }
}

