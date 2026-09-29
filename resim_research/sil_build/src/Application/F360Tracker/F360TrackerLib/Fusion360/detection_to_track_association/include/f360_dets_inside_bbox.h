/*===================================================================================*\
* FILE: f360_dets_inside_bbox.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions to check if a detection is within an object's extended bounding box
*   as well as functions to calculate an extended bounding box of an object.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*
\*==========================================================================================*/
#ifndef F360_DETS_INSIDE_BBOX_H
#define F360_DETS_INSIDE_BBOX_H

#include "f360_calibrations.h"
#include "f360_reuse.h"
#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "rspp_detection_list.h"
#include "f360_dead_zone.h"

namespace f360_variant_A
{
   void Calculate_Ext_Bbox_And_Find_Dets_Inside(
      const F360_Calibrations_T & calibrations,
      const float32_t dist_rear_axle_to_vcs_m,
      const float32_t host_speed,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const Dead_Zone_T& dead_zone,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T & object_track,
      uint32_t(&dets_in_extbox)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t & num_dets_in_extbox);

   void Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside(
      const F360_Calibrations_T & calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const float32_t host_dist_rear_axle_to_vcs_m,
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t (&dets_in_extbox)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t & num_dets_in_extbox);

   void Calculate_Ext_Bbox_Buffer_Zones(
      const F360_Calibrations_T & calibrations,
      const float32_t dist_rear_axle_to_vcs_m,
      const float32_t host_speed,
      const Dead_Zone_T& dead_zone,
      F360_Object_Track_T & object_track);

   void Calculate_Simple_Ext_Bbox_Buffer_Zones(
      const F360_Calibrations_T& calibs,
      const float32_t host_dist_rear_axle_to_vcs_m,
      F360_Object_Track_T & object_track);

   void Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(
      const F360_Calibrations_T & calibrations,
      const F360_Object_Track_T & object_track,
      const float32_t dist_rear_axle_to_vcs_m,
      float32_t & long_buffer,
      float32_t & lat_buffer);

   void Increase_Buffer_Zone_In_Occluded_Parallel_Direction(
      const F360_Object_Track_T& object_track,
      const float32_t& dist_rear_axle_to_vcs_m,
      const F360_Calibrations_T& calibs,
      float32_t& long_buffer1,
      float32_t& long_buffer2);

   void Find_Dets_In_Object_Vicinity(
      const F360_Object_Track_T & object_track,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t & num_dets_of_interest,
      int32_t(&det_idx_of_interest)[MAX_NUMBER_OF_DETECTIONS]);

   void Decrease_Association_Gates_Width_For_Slow_Objects(
      const F360_Calibrations_T& calibs,
      const float32_t obj_width,
      const float32_t obj_vsc_long_pos,
      const float32_t obj_speed,
      float32_t obj_heading,
      float32_t& lat_buffer);
}
#endif
