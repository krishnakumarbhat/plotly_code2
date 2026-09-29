/*===========================================================================*\
* FILE: f360_detection_angle_jumps.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*--------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functionality to flag detections as a suspected angle jump from
* stationary barriers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_detection_angle_jumps.h"
#include "f360_det_angle_jump_internals.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Detect_Angle_Jumps()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *  const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   *  const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   *  const rspp_variant_A::RSPP_Detection_List_T& det_raw
   *  const F360_Calibrations_T& calib
   *  F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Detects angle jumps from stationary barriers.
   * It finds and mark detection as angle jump (f_potential_angle_jump).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Detect_Angle_Jumps(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const rspp_variant_A::RSPP_Detection_List_T& det_raw,
      const F360_Calibrations_T& calib,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      Close_SEPs close_seps_per_sensor[MAX_NUMBER_OF_SENSORS] = { };
      Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps_per_sensor);

      Detect_Angle_Jump_From_SEP(close_seps_per_sensor, det_raw, calib, det_props);
   }

}
