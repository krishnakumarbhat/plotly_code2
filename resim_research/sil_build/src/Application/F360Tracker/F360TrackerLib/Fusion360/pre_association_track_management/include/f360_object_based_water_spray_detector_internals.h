/*===================================================================================*\
* FILE f360_object_based_water_spray_detector_internals.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION
* This file contains declarations of Object_Water_Spray_Detector() and related
* support functions.
*
* Applicable Standards (in order of precedence highest first)
*     ESGW_4-2_PE-SWX_00-01-A01_EN, APTIV C++ Coding Standards [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN APTIV C Coding Standards [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_OBJECT_BASED_WATER_SPRAY_DETECTOR_INTERNALS_H
#define F360_OBJECT_BASED_WATER_SPRAY_DETECTOR_INTERNALS_H

#include "f360_calibrations.h"
#include "f360_detection_props.h"
#include "rspp_detection.h"
#include "f360_object_track.h"
#include "f360_radar_sensor.h"


namespace f360_variant_A
{
   bool Is_Object_Valid_For_Water_Spray(
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T & calibs
   );

   void Derive_Box_For_Water_Spray_Dets(
      const F360_Object_Track_T & obj,
      const F360_Calibrations_T & calibs,
      float32_t (&box)[2][2]
   );

   bool Is_Det_Valid_For_Water_Spray(
      const rspp_variant_A::RSPP_Detection_T & det,
      const F360_Detection_Props_T & det_p,
      const F360_Calibrations_T & calibs,
      const float32_t ref_obj_rcs,
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   );

   bool Does_RR_Fit_Water_Spray_Hypothesis(
      const F360_VCS_Velocity_T &reference_velocity_otg,
      const float32_t det_azimuth_sin,
      const float32_t det_azimuth_cos,
      const float32_t det_range_rate_otg,
      const float32_t rr_factor_min,
      const float32_t rr_factor_max
   );
}
#endif
