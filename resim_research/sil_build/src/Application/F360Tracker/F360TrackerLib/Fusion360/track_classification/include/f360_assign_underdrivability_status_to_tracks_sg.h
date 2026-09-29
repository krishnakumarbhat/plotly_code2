/*===================================================================================*\
* FILE:  f360_assign_underdrivability_status_to_tracks_sg.h
*====================================================================================

* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Assign_Underdrivability_Status_To_Tracks_OCG() function declaration.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef ASSIGN_UNDERDRIVABILITY_STATUS_TO_TRACKS_SG_H
#define ASSIGN_UNDERDRIVABILITY_STATUS_TO_TRACKS_SG_H

#include "f360_object_track.h"
#include "sg_output.h"
#include "f360_timing_info.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "f360_host_props.h"

namespace f360_variant_A
{
   struct SG_Vertex_VCS
   {
      float vcs_position_x;                      // [m] longitudinal position in VCS
      float vcs_position_y;                      // [m] lateral position in VCS
      float position_variance_x;             // [m^2] longitudinal position variance
      float position_variance_y;             // [m^2] lateral position variance
      float position_covariance_xy;          // [m^2] longitudinal/lateral position covariance
      sg::SG_Drivability_Class_T drivability;    // [-] drivability class saying if the host can drive through it
      uint8_t drivability_confidence;   // [%] confidence regarding drivability classification result (0..100)    
   };

   void Assign_Underdrivability_Status_To_Stationary_Object_SG(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_Props_T& host_props,
      const sg::SG_Output_T& sg_output,
      const float32_t host_dist_rear_axle_to_vcs_m,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

   BoundingBox Create_Classification_Gate_From_Segment(
      const SG_Vertex_VCS& prev_vertex,
      const SG_Vertex_VCS& curr_vertex,
      const float32_t classification_gate_width);

   void Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(
      const sg::SG_Drivability_Class_T segment_drivability_status,
      const uint8_t segment_drivability_confidence,
      const float32_t dist_to_segment_sq,
      F360_Object_Track_T& object);
}
#endif
