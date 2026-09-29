#ifndef F360_COMPUTE_WRAPPING_AWARE_SPREAD_H
#define F360_COMPUTE_WRAPPING_AWARE_SPREAD_H
/*===========================================================================*\
* FILE: f360_compute_wrapping_aware_spread.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Compute_Wrapping_Aware_Spread().
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Requirements Document(s):
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

#include "rspp_detection.h"
#include "f360_radar_sensor.h"
#include "f360_constants.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   float32_t Compute_Wrapping_Aware_Spread(
      const float32_t rng_rate1,
      const float32_t rng_rate2,
      const rspp_variant_A::RSPP_Detection_T &det1,
      const rspp_variant_A::RSPP_Detection_T &det2,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]);

   void F360_Compensate_Det_Range(
      const float32_t dealiasing_interval,
      const float32_t r_wrapping,
      const rspp_variant_A::RSPP_Detection_T &detection,
      F360_Detection_Props_T &detection_prop);
}
#endif
