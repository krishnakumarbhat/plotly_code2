/*===================================================================================*\
* FILE:  f360_assign_underdrivability_status_to_tracks_ocg.cpp
*====================================================================================

* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Assign_Underdrivability_Status_To_Tracks_OCG() function implementation.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_assign_underdrivability_status_to_tracks_ocg.h"
#include "f360_calculate_curvi_position.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Assign_Underdrivability_Status_To_Stationary_Object()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib
   * const ocg::OCG_Outputs_T& occupancy_grid
   * const F360_OCG_INTERNAL_T& ocg_internal,
   * const F360_Host_T& host
   * F360_Object_Track_T& object
   * F360_TRKR_TIMING_INFO_T& timing_info
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function assigns underdrivability status to tracks.
   *
   * PRECONDITIONS:
   * Object should be a stationary object.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Assign_Underdrivability_Status_To_Stationary_Object_OCG(
      const F360_Calibrations_T& calib,
      const ocg::OCG_Outputs_T& occupancy_grid,
      const F360_OCG_INTERNAL_T& ocg_internal,
      const F360_Host_T& host,
      F360_Object_Track_T& object,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      if (Object_Should_Not_Be_Considered_OCG(object, calib, host, ocg_internal))
      {
         object.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
         object.probability_underdrivable_ocg = 0.0F;
      }
      else
      {
         ocg::OCG_Cell_Classification new_underdrivable_classification;
         Determine_Underdrivable_Status_OCG(calib, host, object.vcs_position, occupancy_grid, ocg_internal, new_underdrivable_classification);
         // Hysteresis: if level was previously set to CAN_PASS_UNDER, do not lower until new suggested status is
         // CAN_NOT_PASS_UNDER (do not lower from CAN_PASS_UNDER to IS_LIKELY_TO_PASS_UNDER).
         if ((ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER != object.underdrivable_status_ocg) ||
            (ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER == new_underdrivable_classification.underdrivability_status))
         {
            if (new_underdrivable_classification.underdrivability_status == ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER)
            {
               object.underdrivable_status_ocg = new_underdrivable_classification.underdrivability_status;
               object.probability_underdrivable_ocg = 0.0F;
            }
            else
            {
               object.underdrivable_status_ocg = new_underdrivable_classification.underdrivability_status;
               object.probability_underdrivable_ocg = new_underdrivable_classification.probs[ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + new_underdrivable_classification.probs[ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER];
            }
         }
      }
      timing_info.assign_underdrivability_status_to_tracks_ocg += get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Preprocess_OCG()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * const ocg::OCG_Outputs_T* const p_occupancy_grid
   * const float32_t tracker_time_us
   * F360_OCG_INTERNAL_T& ocg_internal
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function performs some preprocessing of the OCG to compute and extract some
   * common information needed to map OCG grid undersrivable status to that of 
   * object underdrivable status.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Preprocess_OCG(
      const F360_Host_T& host,
      const ocg::OCG_Outputs_T* const p_occupancy_grid,
      const float32_t tracker_time_us,
      F360_OCG_INTERNAL_T& ocg_internal)
   {
      bool f_ocg_is_available = (NULL != p_occupancy_grid);
      if (f_ocg_is_available)
      {
         f_ocg_is_available = p_occupancy_grid->f_valid;
      }

      if (f_ocg_is_available)
      {
         // Time difference between the Tracker and the OCG input in Seconds 
         const float32_t f360_ocg_time_diff = (tracker_time_us - static_cast<float32_t>(p_occupancy_grid->timestamp)) * 1e-6F;
         Host_State_Update_OCG(
            host,
            f360_ocg_time_diff,
            p_occupancy_grid->underdrivability.ogcs_host_rear_axle_position.x,
            p_occupancy_grid->underdrivability.ogcs_host_rear_axle_position.y,
            p_occupancy_grid->underdrivability.ogcs_host_rear_axle_position.yaw,
            ocg_internal.host_ocgcs_comp_position,
            ocg_internal.host_ocgcs_comp_yaw);

         Extract_Internal_OCG_Grid_Data(*p_occupancy_grid, ocg_internal);
      }

      return f_ocg_is_available;
   }

   /*===========================================================================*\
   * FUNCTION: Host_State_Update_OCG()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host
   * const float32_t f360_ocg_time_diff
   * const float32_t host_ocgcs_x
   * const float32_t host_ocgcs_y
   * const float32_t host_ocgcs_yaw
   * Point& host_ocgcs_comp_position
   * Angle& host_ocgcs_comp_yaw
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function updates the host states according to the constant velocity motion model
   * and time difference between the OCG module and F360 tracker
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Host_State_Update_OCG(
      const F360_Host_T& host,
      const float32_t f360_ocg_time_diff,
      const float32_t host_ocgcs_x,
      const float32_t host_ocgcs_y,
      const float32_t host_ocgcs_yaw,
      Point& host_ocgcs_comp_position,
      Angle& host_ocgcs_comp_yaw)
   {
      if (std::abs(host.yaw_rate_rad) >= F360_EPSILON)
      {
         const float32_t radius = host.speed / host.yaw_rate_rad;
         host_ocgcs_comp_yaw = Angle{ (host_ocgcs_yaw + (host.yaw_rate_rad * f360_ocg_time_diff)) }.Normalize();
         host_ocgcs_comp_position.x = host_ocgcs_x + (radius * (-F360_Sinf(host_ocgcs_yaw) + host_ocgcs_comp_yaw.Sin()));
         host_ocgcs_comp_position.y = host_ocgcs_y + (radius * (F360_Cosf(host_ocgcs_yaw) - host_ocgcs_comp_yaw.Cos()));
      }
      else
      {
         const float32_t host_dist_travelled = (host.speed * f360_ocg_time_diff);
         host_ocgcs_comp_yaw = Angle{ (host_ocgcs_yaw) };
         host_ocgcs_comp_position.x = host_ocgcs_x + (host_dist_travelled * host_ocgcs_comp_yaw.Cos());
         host_ocgcs_comp_position.y = host_ocgcs_y + (host_dist_travelled * host_ocgcs_comp_yaw.Sin());
      }
   }

   /*===========================================================================*\
   * FUNCTION: Extract_Internal_OCG_Grid_Data()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const ocg::OCG_Outputs_T& occupancy_grid,
   * F360_OCG_INTERNAL_T& ocg_internal
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function extracts and computes some cell information from the OCG.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Extract_Internal_OCG_Grid_Data(
      const ocg::OCG_Outputs_T& occupancy_grid,
      F360_OCG_INTERNAL_T& ocg_internal)
   {
      ocg_internal.ocg_min_x_mid = static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_x_close) * occupancy_grid.grid_definition.cell_length;
      ocg_internal.ocg_max_x_mid = (static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_x_close) + static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_x_mid)) * occupancy_grid.grid_definition.cell_length;
      ocg_internal.ocg_min_cell_width = occupancy_grid.grid_definition.cell_width;
      ocg_internal.ocg_max_cell_width = occupancy_grid.grid_definition.cell_width * occupancy_grid.grid_definition.cell_width_extension_factor;
      ocg_internal.ocg_num_cells_x = occupancy_grid.grid_definition.num_cells_x_close + occupancy_grid.grid_definition.num_cells_x_mid + occupancy_grid.grid_definition.num_cells_x_far;
      ocg_internal.ocg_max_grid_width_half = static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_y) * ocg_internal.ocg_max_cell_width * 0.5F;
      ocg_internal.underdrive_max_zone_long_posn = static_cast<float32_t>(ocg_internal.ocg_num_cells_x) * occupancy_grid.grid_definition.cell_length;
   }

   /*===========================================================================*\
   * FUNCTION: Object_Should_Not_Be_Considered_OCG()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object
   * const F360_Calibrations_T& calib
   * const float32_t underdrive_max_zone_long_posn
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function checks whether object is valid.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   bool Object_Should_Not_Be_Considered_OCG(
      const F360_Object_Track_T& object,
      const F360_Calibrations_T& calib,
      const F360_Host_T& host_props,
      const F360_OCG_INTERNAL_T& ocg_internal)
   {
      const bool f_object_too_far_behind_host = (object.vcs_position.x < calib.k_underdrive_min_trk_long_posn);
      const bool f_object_is_not_within_ocg_longitudinally = ((object.vcs_position.x < calib.k_underdrive_min_zone_long_posn) || (ocg_internal.underdrive_max_zone_long_posn <= object.vcs_position.x));
      const bool f_object_is_not_within_ocg_laterally = ((ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor) < std::abs(Calculate_Curvi_Lat_Pos(host_props, object.vcs_position.x, object.vcs_position.y)));

      return (f_object_too_far_behind_host || f_object_is_not_within_ocg_longitudinally || f_object_is_not_within_ocg_laterally);
   }

   /*===========================================================================*\
   * FUNCTION: Determine_Underdrivable_Status_OCG()
   *===========================================================================
   * RETURN VALUE:
   * void
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calib
   * const F360_Host_T& host
   * const Point& obj_vcs_position
   * const ocg::OCG_Outputs_T& occupancy_grid
   * const F360_OCG_INTERNAL_T& ocg_internal
   * ocg::OCG_Cell_Classification& object_underdrivability_classification
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function finds the cells where the track is located in the occupancy grid,
   * then it returns a underdrivable status, which is equal to the status of cell
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Determine_Underdrivable_Status_OCG(
      const F360_Calibrations_T& calib,
      const F360_Host_T& host,
      const Point& obj_vcs_position,
      const ocg::OCG_Outputs_T& occupancy_grid,
      const F360_OCG_INTERNAL_T& ocg_internal,
      ocg::OCG_Cell_Classification& new_underdrivable_classification)
   {
      float32_t obj_ocgcs_x;
      float32_t obj_ocgcs_y;
      float32_t obj_ocgccs_x;
      float32_t obj_ocgccs_y;

      // Compute object vcs position in OCGCS(Occupancy Grid Coordinate System)
      // In the OCGCS, the host rear axle is treated as the origin for Host coordinate system, thus the object's vcs.x position needs to be adjusted before the transform
      Convert_VCS_Posn_To_OCGCS_Posn(
         (obj_vcs_position.x + host.dist_rear_axle_to_vcs_m),
         obj_vcs_position.y,
         ocg_internal.host_ocgcs_comp_position.x,
         ocg_internal.host_ocgcs_comp_position.y,
         ocg_internal.host_ocgcs_comp_yaw,
         obj_ocgcs_x,
         obj_ocgcs_y);

      // Transform obj_ogcs from OCGCS to OCGCCS (OCG Curvilinear Coordinate System)
      Convert_Cartesian_To_Curvilinear_Coordinates(
         obj_ocgcs_x,
         obj_ocgcs_y,
         occupancy_grid.underdrivability.grid_curvature,
         calib.k_ocg_underdrive_small_curvature_th,
         calib.f_ocg_use_curvilinear_simplification,
         obj_ocgccs_x,
         obj_ocgccs_y);


      // Calaulate the extended cell width, according to object longitudinal position
      const float32_t ocg_extended_cell_width = F360_Linear_Equation_With_Saturation(
         obj_ocgccs_x,
         ocg_internal.ocg_min_x_mid,
         ocg_internal.ocg_max_x_mid,
         ocg_internal.ocg_min_cell_width,
         ocg_internal.ocg_max_cell_width);


      // Lateral position of occupancy grid lower left corner, adjusted according to cell extension
      const float32_t cell_leftmost_point_ocgccs_y = -ocg_extended_cell_width * static_cast<float32_t>(occupancy_grid.grid_definition.num_cells_y) * 0.5F;

      int16_t zone_idx_long = 10000;
      int16_t zone_idx_lat = 10000;

      // Calculate the lateral and longitudinal index for the occupancy grid cell where the object is located
      Calc_In_Which_Zone_Track_Is_Located_OCG(
         obj_ocgccs_x,
         obj_ocgccs_y,
         cell_leftmost_point_ocgccs_y,
         ocg_extended_cell_width,
         occupancy_grid.grid_definition,
         zone_idx_long,
         zone_idx_lat);

      const bool f_outside_grid_long = (zone_idx_long < 0) || (static_cast<int16_t>(ocg_internal.ocg_num_cells_x) <= zone_idx_long);
      const bool f_outside_grid_lat = (zone_idx_lat < 0) || (static_cast<int16_t>(occupancy_grid.grid_definition.num_cells_y) <= zone_idx_lat);

      if (f_outside_grid_long || f_outside_grid_lat)
      {
         new_underdrivable_classification.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      }
      else
      {
         const int16_t zone_idx_long_2 = Calc_Secondary_Zone_Index(obj_ocgccs_x, occupancy_grid.grid_definition.cell_length, ocg_internal.ocg_num_cells_x, zone_idx_long);
         const int16_t zone_idx_lat_2 = Calc_Secondary_Zone_Index((obj_ocgccs_y - cell_leftmost_point_ocgccs_y), ocg_extended_cell_width, occupancy_grid.grid_definition.num_cells_y, zone_idx_lat);

         const ocg::OCG_Cell_Classification& first_cell_class = occupancy_grid.underdrivability.underdrivability_classification[zone_idx_long][zone_idx_lat];
         const ocg::OCG_Cell_Classification& second_cell_class = occupancy_grid.underdrivability.underdrivability_classification[zone_idx_long_2][zone_idx_lat_2];
         
         Pick_Wanted_Underdrivability_Status_OCG(first_cell_class, second_cell_class, new_underdrivable_classification);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_In_Which_Zone_Track_Is_Located_OCG()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t obj_ocgccs_x
   * const float32_t obj_ocgccs_y
   * const float32_t ocg_lower_left_y
   * const float32_t ocg_extended_cell_width
   * const OCG_Definition_T ocg_definition
   * int32_t& zone_idx_long
   * int32_t& zone_idx_lat
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates the indices to the occupancy grid/underdrivability zones in which the track is located
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Calc_In_Which_Zone_Track_Is_Located_OCG(
      const float32_t obj_ocgccs_x,
      const float32_t obj_ocgccs_y,
      const float32_t cell_leftmost_point_ocgccs_y,
      const float32_t ocg_extended_cell_width,
      const ocg::OCG_Definition_T ocg_definition,
      int16_t& zone_idx_long,
      int16_t& zone_idx_lat)
   {
      zone_idx_long = static_cast<int16_t>(F360_Floorf((obj_ocgccs_x / ocg_definition.cell_length)));
      zone_idx_lat = static_cast<int16_t>(F360_Floorf((obj_ocgccs_y - cell_leftmost_point_ocgccs_y) / ocg_extended_cell_width));
   }

   /*===========================================================================*\
   * FUNCTION: Convert_VCS_Posn_To_OCGCS_Posn()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t vcs_x
   * const float32_t vcs_y
   * const float32_t host_ocgcs_x
   * const float32_t host_ocgcs_y
   * const float32_t& host_ocgcs_yaw
   * float32_t& ocgcs_x
   * float32_t& ocgcs_y
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function transforms the object coordinates in VCS to OGCS (Occupancy Grid Coordinate System)
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Convert_VCS_Posn_To_OCGCS_Posn(
      const float32_t vcs_x,
      const float32_t vcs_y,
      const float32_t host_ocgcs_x,
      const float32_t host_ocgcs_y,
      const Angle& host_ocgcs_yaw,
      float32_t& ocgcs_x,
      float32_t& ocgcs_y)
   {
      float32_t temp_x;
      float32_t temp_y;

      F360_Rotate_2D_Vector(vcs_x, vcs_y, host_ocgcs_yaw.Cos(), host_ocgcs_yaw.Sin(), temp_x, temp_y);
      F360_Translate_2D_Position(temp_x, temp_y, host_ocgcs_x, host_ocgcs_y, ocgcs_x, ocgcs_y);
   }


   /*===========================================================================*\
   * FUNCTION: Convert_Cartesian_To_Curvilinear_Coordinates()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t cart_x
   * const float32_t cart_y
   * const float32_t curvature
   * const float32_t small_curvature_th
   * const bool f_ocg_use_curvilinear_simplification
   * float32_t& curv_x
   * float32_t& curv_y
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function transforms the object coordinates from cartesian to curvilinear coordinate system
   * The curvilinear coordinate system is defined as follows
   * Any point on the right side of curve has positive y value
   * Any point on the left side of curve has negative y value
   * Any point that is logitudinally above the origin, has positive x value
   * Any point that is logitudinally below the origin, has negative x value
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Convert_Cartesian_To_Curvilinear_Coordinates(
      const float32_t cart_x,
      const float32_t cart_y,
      const float32_t curvature,
      const float32_t small_curvature_th,
      const bool f_ocg_use_curvilinear_simplification,
      float32_t& curv_x,
      float32_t& curv_y)
   {
      if ((small_curvature_th < std::abs(curvature)))
      {
         const float32_t radius = 1.0F / curvature;
         const float32_t abs_radius = std::abs(radius);
         const float32_t lat_dist_from_center = radius - cart_y;
         const float32_t radial_dist_from_center = F360_Get_Hypotenuse(cart_x, lat_dist_from_center);

         curv_y = (0.0F < radius) ? (abs_radius - radial_dist_from_center) : (radial_dist_from_center - abs_radius);

         if (f_ocg_use_curvilinear_simplification)
         {
            curv_x = cart_x;
         }
         else
         {
            float32_t alpha = F360_Atan2f(cart_x, lat_dist_from_center);
            if (0.0F > radius)
            {
               alpha = (0.0F > alpha) ? (-(F360_PI + alpha)) : (F360_PI - alpha);
            }
            curv_x = alpha * abs_radius;
         }
      }
      else
      {
         curv_x = cart_x;
         curv_y = cart_y - (0.5F * (cart_x * cart_x) * curvature);
      }
   }


   /*===========================================================================*\
   * FUNCTION: Calc_Secondary_Zone_Index()
   *===========================================================================
   * RETURN VALUE:
   * int16_t - Secondary Zone Index
   *
   * PARAMETERS:
   * const float32_t dist_from_index_start_point
   * const float32_t cell_dimension
   * const uint16_t num_cells
   * const int32_t zone_idx
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates the secondary zone index according to the distance from cell boundaries
   * The secondary zone index indicates the next likely cell to which the object can belong
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   int16_t Calc_Secondary_Zone_Index(
      const float32_t dist_from_index_start_point,
      const float32_t cell_dimension,
      const uint16_t num_cells,
      const int16_t zone_idx)
   {
      const float32_t dist_closer_zone = dist_from_index_start_point - (static_cast<float32_t>(zone_idx) * cell_dimension);
      const float32_t dist_further_zone = ((static_cast<float32_t>(zone_idx) + 1.0F) * cell_dimension) - dist_from_index_start_point;

      int16_t zone_idx_2;
      if (dist_closer_zone < dist_further_zone)
      {
         const int16_t first_cell_idx = 0;
         const int16_t previous_adjacent_cell_idx = zone_idx - 1;
         zone_idx_2 = std::max(first_cell_idx, previous_adjacent_cell_idx);
      }
      else
      {
         const int16_t last_cell_idx = static_cast<int16_t>(num_cells) - 1;
         const int16_t next_adjacent_cell_idx = zone_idx + 1;
         zone_idx_2 = std::min(next_adjacent_cell_idx, last_cell_idx);
      }
      return zone_idx_2;
   }

   /*===========================================================================*\
   * FUNCTION: Pick_Wanted_Underdrivability_Status_OCG()
   *===========================================================================
   * RETURN VALUE:
   * ocg::OCG_Underdrivable_Status_T
   *
   * PARAMETERS:
   * const ocg::OCG_Cell_Classification first_status
   * const ocg::OCG_Cell_Classification second_status
   * ocg::OCG_Cell_Classification& new_class
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Given the first and second underdrivability status, this function assigns
   * the new status to be equal to the highest of the two given statuses.
   * 
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Pick_Wanted_Underdrivability_Status_OCG(
      const ocg::OCG_Cell_Classification& first_class,
      const ocg::OCG_Cell_Classification& second_class,
      ocg::OCG_Cell_Classification& new_class
      )
   {
      const ocg::OCG_Underdrivable_Status_T& first_status = first_class.underdrivability_status;
      const ocg::OCG_Underdrivable_Status_T& second_status = second_class.underdrivability_status;

      if ((first_status == ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER) || (second_status == ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER))
      {
         new_class = first_status == ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER ? first_class : second_class;
      }
      else if ((first_status == ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER) || (second_status == ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER))
      {
         new_class = first_status == ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER ? first_class : second_class;
      }
      else if ((first_status == ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER) || (second_status == ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER))
      {
         new_class = first_status == ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER ? first_class : second_class;
      }
      else
      {
         new_class.underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      }
   }
}
