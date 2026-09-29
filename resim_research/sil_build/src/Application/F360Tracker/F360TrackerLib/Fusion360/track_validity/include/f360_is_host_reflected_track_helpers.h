/*===================================================================================*\
* FILE: f360_is_host_reflected_track_helpers.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential � Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of supporting functions used in Is_Host_Reflected_Track().
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_IS_HOST_MIRROR_TRACK_HELPERS_H
#define F360_IS_HOST_MIRROR_TRACK_HELPERS_H

#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_point.h"
#include "f360_reuse.h"
#include "f360_static_env_poly_types.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_trailer_detector_flt_fus_output.h"

namespace f360_variant_A
{

   bool Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox(
      const F360_Object_Track_T& object,
      const Point& sensor_mirror_tcs_pos,
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const F360_Trailer_Estimator_Output_T& trailer_data);

   Point Calc_Predicted_Reflected_Track_TCS_Position(
      const F360_Object_Track_T& object,
      const float32_t sensor_long_pos,
      const float32_t sensor_lat_pos,
      const float32_t sep_lat_pos_sensor,
      const float32_t rot_angle);

   void Determine_Heading_And_Speed_Threshold(
      const float32_t host_speed,
      const float32_t rot_angle,
      const F360_Calibrations_T& calib,
      float32_t& max_heading,
      float32_t& max_speed_diff);

   bool Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      const F360_Host_T& host,
      const F360_Trailer_Estimator_Output_T& trailer_data,
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);

   bool Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(
      const F360_Host_T& host,
      const F360_Object_Track_T& object);

   bool Is_SEP_Valid_For_Host_Mirror_Ghost(
      const Static_Env_Poly_T& sep,
      const F360_Calibrations_T& calib);

   bool Is_Sensor_Adjacent_To_SEP(
       const Static_Env_Poly_T& sep,
       const float32_t& sensor_long_pos,
       const float32_t& sensor_lat_pos,
       const F360_Calibrations_T& calib);

   bool Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      const F360_Object_Track_T& ghost_candidate,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Host_T& host);
}
#endif
