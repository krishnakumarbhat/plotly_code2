/*===========================================================================*\
* FILE: f360_mark_suspicious_angle_jump_detections.h
*============================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Mark_Suspicious_Stationary_Angle_Jump_Detection
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef MARK_SUSPICIOUS_ANGLE_JUMP_DETECTIONS
#define MARK_SUSPICIOUS_ANGLE_JUMP_DETECTIONS

#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_host.h"

namespace f360_variant_A
{
   void Mark_Suspicious_Stationary_Angle_Jump_Detection(
      const F360_Radar_Sensor_T& sensor,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      F360_Detection_Props_T& det_prop);

   struct Sensor_Stationary_Angle_Ambiguity_T {
      bool f_sensor_relevant;
      uint8_t nr_of_candidates;
      float32_t angle_ambiguity_candidates_rad[MAX_NUM_ANGLE_JUMPS];
   };

   void Determine_Precond_For_Angle_Jumps(
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Detection_Props_T& det_prop,
      const bool f_moving_angle_jump_context,
      Sensor_Stationary_Angle_Ambiguity_T(& sensors_angle_amb));

   void Get_SRR7p_Amb_Angles(
      const F360_Radar_Sensor_T& sensor,
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      const F360_Detection_Props_T& det_prop,
      Sensor_Stationary_Angle_Ambiguity_T(& sensors_angle_amb));

   void Get_FLR7_Amb_Angles(
      const rspp_variant_A::RSPP_Detection_T& rspp_det,
      Sensor_Stationary_Angle_Ambiguity_T(& sensors_angle_amb));

   inline bool Is_Rear_Corner_Sensor(const F360_Radar_Sensor_T& sensor)
   {
      return (F360_MOUNTING_LOCATION_LEFT_REAR  == sensor.constant.mounting_location) ||
             (F360_MOUNTING_LOCATION_RIGHT_REAR == sensor.constant.mounting_location);
   }
}
#endif
