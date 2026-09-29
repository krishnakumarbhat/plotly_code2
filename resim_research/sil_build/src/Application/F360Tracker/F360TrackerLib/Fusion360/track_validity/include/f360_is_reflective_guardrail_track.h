/*===================================================================================*\
* FILE:  f360_is_reflective_guardrail_track.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declaration of Is_Reflective_Guardrail_Track() function.
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/
#ifndef F360_IS_REFLECTIVE_GUARDRAIL_TRACK_H
#define F360_IS_REFLECTIVE_GUARDRAIL_TRACK_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_static_env_poly_types.h"
#include "f360_calibrations.h"
#include "f360_host.h"

namespace f360_variant_A
{
   bool Is_Reflective_Guardrail_Track(
      const F360_Tracker_Info_T& tracker_info,
      const int32_t ghost_candidate_idx,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Calibrations_T& cal,
      const F360_Host_T& host,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   );

   bool Is_Ghost_Obj_Valid_For_Reflection_Check(
       const F360_Object_Track_T& ghost_candidate,
       const F360_Calibrations_T& calib
   );

   bool Is_Source_Obj_Valid_For_Reflection_Check(
       const F360_Object_Track_T& ghost_candidate,
       const F360_Object_Track_T& source_candidate,
       const F360_Calibrations_T& calib
   );

   bool Is_Ghost_Reflected_By_SEP(
      const Static_Env_Poly_T& sep,
      const F360_Object_Track_T& ghost_candidate,
      const Point& ghost_cand_pos,
      const F360_Object_Track_T& source_candidate,
      const F360_Calibrations_T& cal
   );

   bool Is_Ghost_Reflected_Without_SEP(
       const F360_Object_Track_T& ghost_candidate,
       const F360_Object_Track_T& source_candidate,
       const F360_Tracker_Info_T& tracker_info,
       const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
       const F360_Host_T& host
   );

   Point Calculate_Center_Of_Symmetry_SEP(
      const Point& ghost_cand_pos,
      const Point& point_of_reflection,
      const Static_Env_Poly_T& sep
   );

   bool Is_Heading_Consistent_With_SEP_Reflection(
      const float32_t source_heading_vcs,
      const float32_t ghost_heading_vcs,
      const float32_t tangent_angle_vcs,
      const float32_t tolerance
   );

   bool Is_Source_Candidate_Similar_To_Ghost_Candidate(
      const F360_Object_Track_T& source_candidate,
      const F360_Object_Track_T& ghost_candidate,
      const Point& hypothetic_source_pos,
      const float32_t sep_tangent_angle,
      const F360_Calibrations_T& cal
   );

   bool Is_Hypot_Source_Pos_In_Source_Candidate_BBox(
      const Point& hypothetic_source_pos,
      const F360_Object_Track_T& source_candidate,
      const F360_Object_Track_T& ghost_candidate,
      const F360_Calibrations_T& cal
   );


}
#endif
