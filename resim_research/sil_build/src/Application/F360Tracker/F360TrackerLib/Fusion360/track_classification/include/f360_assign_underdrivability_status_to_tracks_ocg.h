/*===================================================================================*\
* FILE:  f360_assign_underdrivability_status_to_tracks_ocg.h
*====================================================================================

* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.

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

#ifndef ASSIGN_UNDERDRIVABILITY_STATUS_TO_TRACKS_OCG_H
#define ASSIGN_UNDERDRIVABILITY_STATUS_TO_TRACKS_OCG_H

#include "f360_host.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_point.h"
#include "f360_angle.h"
#include "ocg_occupancy_grid_types.h"
#include "f360_timing_info.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   typedef struct F360_OCG_INTERNAL
   {
      float32_t ocg_min_x_mid;
      float32_t ocg_max_x_mid;
      float32_t ocg_min_cell_width;
      float32_t ocg_max_cell_width;
      float32_t ocg_max_grid_width_half;
      uint16_t ocg_num_cells_x;
      float32_t underdrive_max_zone_long_posn;
      Point host_ocgcs_comp_position;
      Angle host_ocgcs_comp_yaw;
   }F360_OCG_INTERNAL_T;

   void Assign_Underdrivability_Status_To_Stationary_Object_OCG(
      const F360_Calibrations_T& calib,
      const ocg::OCG_Outputs_T& occupancy_grid,
      const F360_OCG_INTERNAL_T& ocg_internal, 
      const F360_Host_T& host,
      F360_Object_Track_T& object,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Pick_Wanted_Underdrivability_Status_OCG(
      const ocg::OCG_Cell_Classification& first_class,
      const ocg::OCG_Cell_Classification& second_class,
      ocg::OCG_Cell_Classification& new_class);

   bool Object_Should_Not_Be_Considered_OCG(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib,
      const F360_Host_T& host_props,
      const F360_OCG_INTERNAL_T& ocg_internal);

   void Calc_In_Which_Zone_Track_Is_Located_OCG(
      const float32_t obj_ocgccs_x,
      const float32_t obj_ocgccs_y,
      const float32_t cell_leftmost_point_ocgccs_y,
      const float32_t ocg_extended_cell_width,
      const ocg::OCG_Definition_T ocg_definition,
      int16_t& zone_idx_long,
      int16_t& zone_idx_lat);

   void Determine_Underdrivable_Status_OCG(
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const Point& obj_vcs_position,
      const ocg::OCG_Outputs_T& occupancy_grid,
      const F360_OCG_INTERNAL_T& ocg_internal,
      ocg::OCG_Cell_Classification& new_underdrivable_classification);

   void Convert_VCS_Posn_To_OCGCS_Posn(
      const float32_t vcs_x,
      const float32_t vcs_y,
      const float32_t host_ocgcs_x,
      const float32_t host_ocgcs_y,
      const Angle& host_ocgcs_yaw,
      float32_t& ocgcs_x,
      float32_t& ocgcs_y);

   void Convert_Cartesian_To_Curvilinear_Coordinates(
      const float32_t cart_x,
      const float32_t cart_y,
      const float32_t curvature,
      const float32_t small_curvature_th,
      const bool f_ocg_use_curvilinear_simplification,
      float32_t& curv_x,
      float32_t& curv_y);

   int16_t Calc_Secondary_Zone_Index(
      const float32_t dist_from_index_start_point,
      const float32_t cell_dimension,
      const uint16_t num_cells,
      const int16_t zone_idx);

   bool Preprocess_OCG(
      const F360_Host_T& host,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const float32_t tracker_time_us,
      F360_OCG_INTERNAL_T& ocg_internal);

   void Host_State_Update_OCG(
      const F360_Host_T& host,
      const float32_t f360_ocg_time_diff,
      const float32_t host_ocgcs_x,
      const float32_t host_ocgcs_y,
      const float32_t host_ocgcs_yaw,
      Point& host_ocgcs_comp_position,
      Angle& host_ocgcs_comp_yaw);

   void Extract_Internal_OCG_Grid_Data(
      const ocg::OCG_Outputs_T& occupancy_grid,
      F360_OCG_INTERNAL_T& ocg_internal);
}
#endif
