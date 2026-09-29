/*===========================================================================*\
* FILE: f360_det_angle_jump_internals.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Det_Angle_Jump_Internals()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_DET_ANGLE_JUMP_INTERNALS_H
#define F360_DET_ANGLE_JUMP_INTERNALS_H

#include "f360_radar_sensor.h"
#include "f360_static_env_poly_types.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_detection_angle_jumps.h"

namespace f360_variant_A
{
   struct SEP_Info
   {
      float32_t max_range;
      float32_t min_range;
      float32_t expected_range_rate;
      bool f_valid;
   };

   struct Close_SEPs
   {
      SEP_Info first_closest;
      SEP_Info second_closest;
   };

   void Get_SEP_Info_Per_Sensor(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib,
      Close_SEPs(&seps_info_per_sensor)[MAX_NUMBER_OF_SENSORS]);

   void Detect_Angle_Jump_From_SEP(
      const Close_SEPs(&close_seps_per_sensor)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& det_raw,
      const F360_Calibrations_T& calib,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);

   Closest_SEP_Info Find_Closest_SEP(
      const F360_Radar_Sensor_T& sensor,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const float32_t long_search_margin,
      const float32_t min_abs_lat_pos = 0.0F);

   bool Is_Det_Angle_Jump(
      const rspp_variant_A::RSPP_Detection_T& det_raw,
      const F360_Detection_Props_T& det_prop,
      const float32_t abs_det_vcs_az,
      const Close_SEPs& closest_seps,
      const F360_Calibrations_T& calib);
}


#endif
