/*===================================================================================*\
* FILE:  f360_calc_predicted_range_rate.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains declarations of function Calc_Predicted_Range_Rate()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_CALC_PREDICTED_RANGE_RATE_H
#define F360_CALC_PREDICTED_RANGE_RATE_H

#include "f360_detection_props.h"
#include "rspp_detection.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"


namespace f360_variant_A
{
   float32_t Calc_Predicted_Range_Rate(
      const F360_Detection_Props_T & det_p,
      const rspp_variant_A::RSPP_Detection_T & det,
      const F360_Object_Track_T & obj,
      const F360_Radar_Sensor_T & sens
   );
}
        

#endif
