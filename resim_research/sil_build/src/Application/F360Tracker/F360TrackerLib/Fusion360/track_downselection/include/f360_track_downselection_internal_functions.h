/*===================================================================================*\
* FILE: f360_track_downselection_internal_functions.h
*====================================================================================
* Copyright (C) 2020 Aptiv. All Rights Reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains signatures of functions used in Track_Downselection()
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_TRACK_DOWNSELECTION_INTERNAL_FUNCTIONS_H
#define F360_TRACK_DOWNSELECTION_INTERNAL_FUNCTIONS_H

#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_bounding_box.h"
#include "f360_static_env_poly_types.h"
#include "f360_constants.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   float32_t Calc_Track_Priority(
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const F360_Tracker_Info_T& tracker_info,
      const BoundingBox& overall_confidence_exclusion_box,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool f_limited_FOV,
      F360_Object_Track_T& obj_trk);

   void Increase_Priority_and_Adjust_EP_Variant_Based(
       const F360_Tracker_Variant_T tracker_variant,
       F360_Object_Track_T& obj_trk,
       float32_t& priority);

   int32_t Pop_Reduced_Id(F360_Tracker_Info_T& tracker_info);

   void Select_Obj_Tracks_to_Downselect(
      const F360_Host_T& host,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      float32_t(&priorities)[NUMBER_OF_OBJECT_TRACKS],
      int32_t(&candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t& candidates_cnt);

   void Assign_Reduced_Idxs_To_Prioritized_Tracks(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      const int32_t(&candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t(&ids_of_objs_sorted_by_priority)[NUMBER_OF_OBJECT_TRACKS],
      const uint32_t candidates_cnt);

   void Deselect_Existing_Reduced_Tracks(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      int32_t(&candidates_idxs)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t(&ids_of_objs_sorted_by_priority)[NUMBER_OF_OBJECT_TRACKS],
      uint32_t& candidates_cnt);

   void Decrease_Priority_and_Confidence_for_Implausible_Tracks(
      const F360_Tracker_Info_T &tracker_info,
      const F360_Calibrations_T &calib,
      F360_Object_Track_T& obj_trk,
      float32_t& priority);

   bool Is_Unreliable_Low_Conf_Moveable_Track(
      const F360_Object_Track_T& obj_trk,
      const F360_Host_T& host,
      const F360_Calibrations_T& calib,
      const BoundingBox& exclusion_box,
      const bool f_recently_entered_FOV);

   BoundingBox Define_Overall_Confidence_Exclusion_Box_Around_Host(
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& calib,
      const float32_t box_longpos_shift);

   bool Is_Outside_Triangular_Zone_Behind_Host(
      const Point& track_posn,
      const float32_t triangular_zone_long_shift);

   bool Is_Heading_Different_Than_Host(
      const F360_Object_Track_T& obj_trk,
      const F360_Calibrations_T &calib);

   bool Is_Outside_Exclusion_Box(
      const F360_Host_T host,
      const F360_Object_Track_T& obj_trk,
      const BoundingBox& exclusion_box,
      const bool f_recently_entered_FOV);

   bool Has_Low_TTC(
      const float32_t host_speed,
      const F360_VCS_Velocity_T& track_velocity,
      const Point& track_pos,
      const float32_t k_low_conf_unreliability_max_ttc);

   void Cond_LP_Filter_Reduced_Det_Num(
      const F360_Tracker_Info_T& tracker_info,
      const float32_t &filtration_factor,
      F360_Object_Track_T(&obj_trks)[NUMBER_OF_OBJECT_TRACKS]);

   bool Determine_FOV_Status(
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]);

   bool Has_Object_Recently_Entered_FOV(
       const F360_Host_T& host,
       const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
       const F360_Object_Track_T& obj_trk);
}
#endif
