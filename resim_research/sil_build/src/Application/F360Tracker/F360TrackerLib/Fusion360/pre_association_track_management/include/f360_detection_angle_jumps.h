#ifndef F360_DETECTION_ANGLE_JUMPS_H
#define F360_DETECTION_ANGLE_JUMPS_H

/*===========================================================================*\
* FILE: f360_detection_angle_jumps.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Det_Angle_Jumps()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_radar_sensor.h"
#include "f360_static_env_poly_types.h"
#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Detect_Angle_Jumps(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const rspp_variant_A::RSPP_Detection_List_T& det_raw,
      const F360_Calibrations_T& calib,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

}
#endif
