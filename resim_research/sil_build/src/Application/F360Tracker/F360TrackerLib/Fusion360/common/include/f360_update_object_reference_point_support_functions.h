#ifndef F360_UPDATE_OBJECT_REFERENCE_POINT_SUPPORT_FUNCTIONS_H
#define F360_UPDATE_OBJECT_REFERENCE_POINT_SUPPORT_FUNCTIONS_H
/*===================================================================================*\
* FILE: f360_update_object_reference_point_support_functions.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declarations for support functions related to updating
*   an object's reference point.
* 
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_calibrations.h"
#include "f360_object_track.h"
#include "f360_reference_point_candidate.h"
#include "f360_reference_point.h"
#include "f360_reuse.h"
#include "f360_radar_sensor.h"
#include "f360_globals.h"

namespace f360_variant_A
{
   void Update_Object_KF_States_After_Reference_Point_Change(
      F360_Object_Track_T& obj);

   void Increase_Object_Pos_Uncertainty_After_Reference_Point_Change(
      const F360_Reference_Point_T old_ref_pnt,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj);

   void Apply_Reference_Point_Hysteresis(
      const F360_Calibrations_T& calibrations,
      const F360_Object_Track_T& obj,
      Reference_Point_Data& ref_pnt_ignoring_visibility,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility);

   void Apply_Reference_Point_Hysteresis_Considering_Visibility(
      const float32_t hysteresis_factor,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility);

   void Apply_Reference_Point_Hysteresis_Ignoring_Visibility(
      const float32_t hysteresis_factor,
      const Reference_Point_Data_Ext& ref_pnt_considering_visibility,
      Reference_Point_Data& ref_pnt_ignoring_visibility);

   void Derive_Object_Reference_Point(
      const bool f_current_object_behind_another_object,
      const float32_t k_far_away_object_dist_sq_thr,
      const float32_t host_dist_rear_axle_to_vcs_m,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      const F360_Object_Track_T& object_track,
      Reference_Point_Data& ref_pnt_ignoring_visibility,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility);

   void Find_Best_Reference_Point_Ignoring_Visibility(
      const float32_t(&ref_pnt_vec_proj_val)[8],
      const uint8_t num_relevant_ref_points,
      Reference_Point_Candidate& ref_point_with_best_projection_value);

   void Find_Best_Reference_Point_Considering_Visibility(
      const float32_t(&ref_pnt_vec_proj_val)[8],
      const bool(&ref_pnt_f_visible)[8],
      const uint8_t num_relevant_ref_points,
      Reference_Point_Candidate& ref_point_with_best_projection_value_and_visible);

   bool Are_All_Reference_Points_Visible(
      const bool(&ref_pnt_f_visible)[8],
      const uint8_t num_relevant_ref_points);

   void Choose_New_Reference_Point_With_Hysteresis(
      const float32_t& k_reference_point_hysteresis_factor,
      const Reference_Point_Candidate& prev_ref_point,
      Reference_Point_Candidate& new_ref_point);

   F360_Reference_Point_T Convert_Ref_Point_Vector_Index_To_Ref_Point_Enum(
      const uint8_t ref_point_vector_idx);

   uint8_t Convert_Ref_Point_Enum_To_Ref_Point_Vector_Index(
      const F360_Reference_Point_T ref_point_enum);

   void Compute_Side_And_Corner_Normalized_Reference_Point_Vectors(
      const F360_Object_Track_T& object_track,
      float32_t(&normalized_ref_point_vectors)[8][2]);

   void Extract_Previous_Reference_Point_Data(
      const F360_Reference_Point_T& prev_reference_point,
      const float32_t(&ref_pnt_vec_proj_val)[8],
      const bool(&ref_pnt_f_visible)[8],
      Reference_Point_Candidate& prev_reference_point_candidate);

   uint8_t Get_Number_Of_Relevant_Reference_Points(
      const float32_t k_far_away_object_dist_sq_thr,
      const Point& obj_vcs_position);
   
   void Reference_Point_Blind_Spot_Countermeasure(
      const F360_Object_Track_T& obj,
      Reference_Point_Data_Ext& ref_pnt_considering_visibility);
}
#endif
