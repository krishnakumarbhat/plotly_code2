/*===================================================================================*\
* FILE: f360_handle_bistatic.cpp
*====================================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of function Handle_Bistatic_Dets.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards" [06-Sep-2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_handle_bistatic.h"
#include "f360_det_is_seen_by_sensor.h"
#include "f360_det_is_in_bistatic_fp_area.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Handle_Bistatic_Dets()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &detection
   * const F360_Calibrations_T &calib
   * const F360_Radar_Sensor_Props_T &sensor_prop
   * F360_Detection_Props_T &detection_prop
   *
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
   * This functions marks bistatic detection as not ok to use if various conditions are met.
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Handle_Bistatic_Dets(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Calibrations_T &calib,
      const F360_Radar_Sensor_Props_T &sensor_prop,
      F360_Detection_Props_T &detection_prop)
   {
      if (detection.raw.f_bistatic)
      {
         if ((detection_prop.on_sep_id != F360_INVALID_UNSIGNED_ID) || (detection_prop.behind_sep_id != F360_INVALID_UNSIGNED_ID))
         {
            detection_prop.f_ok_to_use = false;
         }
         else if (Is_Det_In_Bistatic_FP_Area(detection_prop.vcs_position, calib))
         {
            if (!Is_Det_Seen_By_Sensor(detection_prop.vcs_position, sensor_prop, calib.k_bistatic_lat_th_extension))
            {
               detection_prop.f_ok_to_use = false;
            }
         }
         else // if bistatic detection is not in bistatic FP area we do not want to use it
         {
            detection_prop.f_ok_to_use = false;
         }
      }
   }
}

