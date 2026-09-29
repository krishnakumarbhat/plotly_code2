/*===================================================================================*\
 * FILE: f360_handle_host_vehicle_clutter.cpp
 *====================================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 * This file contains functions for handling detection flagged as "host vehicle clutter" by raw signal processing
 *
 * ABBREVIATIONS:
 * HVC: host vehicle clutter
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [06-Sep-2020]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 *
 \*====================================================================================*/

#include "f360_handle_host_vehicle_clutter.h"
#include "f360_handle_host_vehicle_clutter_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Handle_Host_Vehicle_Clutter()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &detection - reference to raw detection
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS] - reference to sensor properties
   * const F360_Calibrations_T &calib - tracker calibrations
   * F360_Detection_Props_T &detection_prop - reference to estimated detection properties
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
   * Function updates detection f_ok_to_use flag basing on it's HVC flag and VCS position.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Handle_Host_Vehicle_Clutter(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T &calib,
      F360_Detection_Props_T &detection_prop)
   {
      if (detection.raw.f_host_veh_clutter)
      {
         const Pos_Limits_T hvc_tp_area = Get_HVC_TP_Area(sensors, calib.max_range_flagging_hvc_dets);

         const bool hvc_det = (detection_prop.behind_sep_id != F360_INVALID_UNSIGNED_ID) || (detection_prop.on_sep_id != F360_INVALID_UNSIGNED_ID) ||
            (Is_Det_In_HVC_TP_Area(detection_prop.vcs_position, hvc_tp_area));

         if (hvc_det)
         {
            detection_prop.f_ok_to_use = false;
         }
      }
   }
}

