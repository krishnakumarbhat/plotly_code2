/*===================================================================================*\
* FILE:  f360_check_if_object_is_suspected_stationary_helpers.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains implementation of Is_Object_Suspected_Stationary() function.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef CHECK_IF_OBJECT_IS_SUSPECTED_STATIONARY_HELPERS_H
#define CHECK_IF_OBJECT_IS_SUSPECTED_STATIONARY_HELPERS_H

#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_host.h"
#include "f360_calibrations.h"
#include "f360_point.h"
#include "f360_radar_sensor.h"
#include "f360_occlusion_types.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   bool Is_Object_Stationary_By_Num_Dets(
      const F360_Object_Track_T& object,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const float32_t min_moving_dets_percentage);

   void Get_Object_Vel_Cov(
      float32_t(&vel_cov)[2][2],
      const float32_t(&statecov)[STATE_DIMENSION][STATE_DIMENSION]);

   float32_t Calc_NEES_Value_For_Vel_Diff(
      const F360_VCS_Velocity_T& vcs_vel,
      const float32_t(&vel_cov)[2][2]);

   bool Is_Object_Stationary_By_Vel_NEES(
      F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);

   bool Is_Object_Stationary_By_Vel_Sigma(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib);

   bool Is_Object_Parallel_Moving(
      const F360_Object_Track_T& obj,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib);

   bool Is_Object_Cross_Radial_Moving(
       const F360_Object_Track_T& object);

   float32_t Calc_P_Value_Threshold(
      const Point& obj_vcs_pos,
      const float32_t base_min_p_value,
      const float32_t range_scaling_factor);

   float32_t Calc_Moving_Speed_Threshold(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& object,
      const F360_Occlusion_Data_T(&occlusion_data)[MAX_NUMBER_OF_SENSORS]);

   float32_t Calc_Cross_Moving_Weight(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detect_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& object);
}

#endif
