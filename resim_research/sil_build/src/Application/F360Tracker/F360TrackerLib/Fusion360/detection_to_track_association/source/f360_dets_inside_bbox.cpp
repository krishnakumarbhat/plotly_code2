/*===================================================================================*\
* FILE: f360_dets_inside_bbox.cpp
*====================================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions to check if a detection is within an object's extended bounding box
*   as well as functions to calculate an extended bounding box of an object.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_dets_inside_bbox.h"
#include "f360_math_func.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_update_extended_bbox_offsets_for_object_in_dead_zone.h"
#include "f360_bounding_box.h"
#include <algorithm>
#include "f360_detection_association_support_functions.h"


namespace f360_variant_A
{
   static bool Is_Point_Inside_Reduced_Extended_Bounding_Box(const Point& point_vcs,
      const F360_Object_Track_T& obj);
   /*===========================================================================*\
   * FUNCTION: Calculate_Ext_Bbox_And_Find_Dets_Inside()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Calibrations_T & calibrations,
   *  const float32_t dist_rear_axle_to_vcs_m,
   *  const float32_t host_speed,
   *  const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
   *  const Dead_Zone_T& dead_zone,
   *  F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   *  F360_Object_Track_T & object_track,
   *  uint32_t (&dets_in_extbox)[MAX_NUMBER_OF_DETECTIONS],
   *  uint32_t & num_dets_in_extbox
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
   * Calculate_Ext_Bbox_And_Find_Dets_Inside calculates an extended bounding box 
   * and counts the number of detectionsinside the extended bounding box. The extended 
   * bounding box is dependant on object speed, range and azimuth. 
   * The number of detections are stored in num_dets_in_extbox, and the corresponding  
   * detection indices are stored in dets_in_extbox.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Calculate_Ext_Bbox_And_Find_Dets_Inside(
      const F360_Calibrations_T & calibrations,
      const float32_t dist_rear_axle_to_vcs_m,
      const float32_t host_speed,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const Dead_Zone_T& dead_zone,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T & object_track,
      uint32_t (&dets_in_extbox)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t & num_dets_in_extbox)
   {
      // Calculate buffer zones for extended bounding box
      Calculate_Ext_Bbox_Buffer_Zones(calibrations, dist_rear_axle_to_vcs_m, host_speed, dead_zone, object_track);

      // Find detections in object vicinity to not loop over detections that are not of interest
      uint32_t num_dets_of_interest;
      int32_t det_idx_of_interest[MAX_NUMBER_OF_DETECTIONS] = {};
      Find_Dets_In_Object_Vicinity(object_track, raw_detections, detection_props, num_dets_of_interest, det_idx_of_interest);
      num_dets_in_extbox = 0U;

      // Loop over detections of interest found during first crude filtering and check if they are inside extended bounding box
      for (uint32_t i = 0U; i < num_dets_of_interest; i++)
      {
         const uint32_t det_idx = static_cast<uint32_t>(det_idx_of_interest[i]);
         F360_Detection_Props_T &det_prop = detection_props[det_idx];

         if (Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(det_prop.vcs_position, object_track, det_prop.f_water_spray, calibrations))
         {
             dets_in_extbox[num_dets_in_extbox] = det_idx;
             det_prop.f_inside_gate = true;
             num_dets_in_extbox++;
             if (Is_Point_Inside_Reduced_Extended_Bounding_Box(det_prop.vcs_position, object_track))
             {
                 det_prop.f_inside_mov_gate = true;
             }
             else
             {
                 // Do nothing, to avoid MISRA warning
             }
         }
         else
         {
             // Do nothing, to avoid MISRA warning
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations            - Reference to calibration struct
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   * const float32_t host_dist_rear_axle_to_vcs_m - host distance to rear axle
   * F360_Object_Track_T & object_track                  - Object track
   * F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS] - Detection properties
   * uint32_t (&dets_in_extbox)[MAX_DETS_IN_OBJ_TRK] - List of detection indices for detections 
   *                                                       inside an extended bounding box around the object
   * uint32_t & num_dets_in_extbox                   - Number of detections inside an extended 
   *                                                       bounding box around the object
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
   * Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside calculates an extended bounding 
   * box and counts the number of detections inside the extended bounding box. The 
   * extended bounding box is dependant on the object's dimensions.
   * The number of detections are stored in num_dets_in_extbox, and the corresponding
   * detection indexes are stored in dets_in_extbox.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Calculate_Simple_Ext_Bbox_And_Find_Dets_Inside(
      const F360_Calibrations_T & calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const float32_t host_dist_rear_axle_to_vcs_m,
      F360_Object_Track_T& object_track,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t (&dets_in_extbox)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t & num_dets_in_extbox)
   {
      Calculate_Simple_Ext_Bbox_Buffer_Zones(calibrations, host_dist_rear_axle_to_vcs_m, object_track);

      // Center of bounding box in VCS
      const Point obj_center = object_track.bbox.Get_Center();
      const float32_t obj_extended_radius = (object_track.bbox.Get_Length() * 0.5F) + object_track.long_buffer_zone_len1;
      Get_Det_Indexes_In_Vcs_Circular_Zone(
         obj_center,
         obj_extended_radius,
         raw_detections,
         detection_props,
         dets_in_extbox,
         num_dets_in_extbox);

   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Ext_Bbox_Buffer_Zones()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations   - Reference to calibration structure
   * const float32_t dist_rear_axle_to_vcs_m   - Distance from rear axle to VCS origin [m]
   * const float32_t host_speed                - Host OTG speed
   * const Dead_Zone_T& dead_zone               - Longitudinal limits of basic and extended dead zone
   * F360_Object_Track_T & object_track         - Object track stucture
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
   * Calculate_Ext_Bbox_Buffer_Zones calculates longitudinal and lateral buffers defining the
   * extended bounding box, based on speed, range and azimuth. The calculated buffers are 
   * stored in the current object's long_buffer_zone_len1/2 and lat_buffer_zone_wid1/wid2.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Calculate_Ext_Bbox_Buffer_Zones(
      const F360_Calibrations_T & calibrations,
      const float32_t dist_rear_axle_to_vcs_m,
      const float32_t host_speed,
      const Dead_Zone_T& dead_zone,
      F360_Object_Track_T & object_track)
   {
      // Minimum association gates
      float32_t long_buffer1 = calibrations.k_min_assoc_gate_long_buffer_moveable_objs;
      float32_t lat_buffer = calibrations.k_min_assoc_gate_lat_buffer_moveable_objs;

      // Add a distance and azimuth dependant component to the association buffer
      Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(calibrations, object_track, dist_rear_axle_to_vcs_m, long_buffer1, lat_buffer);

      float32_t long_buffer2 = long_buffer1;

      // Modifications of association buffer for CTCA and high speed CCA objects to ensure that the association longitudinal buffer is large enough in the object parallel direction which is less visible from host
      Increase_Buffer_Zone_In_Occluded_Parallel_Direction(
         object_track,
         dist_rear_axle_to_vcs_m,
         calibrations,
         long_buffer1,
         long_buffer2);

      // Modifications of association buffer for objects in dead zone. Extend bounding box in longitudinal directions.
      Update_Extended_BBox_Offsets_For_Object_In_Dead_Zone(calibrations, host_speed, dead_zone, object_track, long_buffer1, long_buffer2);
      
      object_track.long_buffer_zone_len1 = long_buffer1;
      object_track.long_buffer_zone_len2 = long_buffer2;

      // Modifications of latitudinal association buffer for slow and not crossing objects
      Decrease_Association_Gates_Width_For_Slow_Objects(calibrations, object_track.bbox.Get_Width(), object_track.vcs_position.x, object_track.speed, object_track.vcs_heading.Value(), lat_buffer);

      object_track.lat_buffer_zone_wid1 = lat_buffer;
      object_track.lat_buffer_zone_wid2 = lat_buffer;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Simple_Ext_Bbox_Buffer_Zones()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Object_Track_T & object_track - Object track
   * const float32_t host_dist_rear_axle_to_vcs_m - host distance to rear axle
   * const float32_t radial_buffer_zone - Default radial buffer zone
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
   * Calculate_Simple_Ext_Bbox_Buffer_Zones calculates longitudinal and lateral buffers 
   * defining the extended bounding box, based on object size.
   * The calculated buffers are stored in the current object's long_buffer_zone_len1/len2 
   * and lat_buffer_zone_wid1/wid2.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Calculate_Simple_Ext_Bbox_Buffer_Zones(
      const F360_Calibrations_T& calibs,
      const float32_t host_dist_rear_axle_to_vcs_m,
      F360_Object_Track_T & object_track)
   {
      const float32_t host_center_vcs_x_pos = 0.6F * host_dist_rear_axle_to_vcs_m;

      const float32_t obj_dist_from_host = F360_Get_Hypotenuse(object_track.vcs_position.x + host_center_vcs_x_pos, object_track.vcs_position.y);

      const float32_t coarse_buffer = F360_Linear_Equation(obj_dist_from_host, calibs.k_obj_dist_for_min_assoc_gate_extension_non_moveable, calibs.k_obj_dist_for_max_assoc_gate_extension_non_moveable, calibs.k_min_assoc_gate_extension_non_moveable, calibs.k_max_assoc_gate_extension_non_moveable) + object_track.speed * calibs.k_spd_dependent_assoc_gate_extension_factor_non_moveable;
      const float32_t ext_buffer = std::min(coarse_buffer, calibs.k_max_assoc_gate_extension_non_moveable);

      object_track.long_buffer_zone_len1 = ext_buffer;
      object_track.long_buffer_zone_len2 = ext_buffer;
      object_track.lat_buffer_zone_wid2 = ext_buffer;
      object_track.lat_buffer_zone_wid1 = ext_buffer;
   }

   /*===========================================================================*\
   * FUNCTION: Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations   - Reference to calibration structure
   * const F360_Object_Track_T & object_track   - Object track
   * const float32_t dist_rear_axle_to_vcs_m   - Distance from rear axle to VCS origin [m]
   * float32_t & long_buffer                   - Longitudinal buffer zone
   * float32_t & lat_buffer                    - Lateral buffer zone
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
   * This function calculates a component of the longitudinal and lateral buffer 
   * zones defining the extended bounding box. The component is based on distance 
   * to object and difference between the object's vcs pointing angle and vcs 
   * aspect angle from host center to object center.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Calculate_Buffer_Zones_Dependant_On_Distance_And_Azimuth_Diff(
      const F360_Calibrations_T & calibrations,
      const F360_Object_Track_T & object_track,
      const float32_t dist_rear_axle_to_vcs_m,
      float32_t & long_buffer,
      float32_t & lat_buffer)
   {

      // Define thresholds for max objext dimensions 
       constexpr float32_t object_dimension_treshold = 1.2F;
       constexpr float32_t max_azimuth_buffer = 2.0F;

      // Calculate distance from center of host to center of object.
      const float32_t vcs_pos_x_dist = object_track.bbox.Get_Center().x + (dist_rear_axle_to_vcs_m * 0.5F);
      const float32_t vcs_pos_y_dist = object_track.bbox.Get_Center().y;
      const float32_t dist_to_obj_from_host_center = F360_Get_Hypotenuse(vcs_pos_x_dist, vcs_pos_y_dist);

      // Set buffer depending on distance. Object far away results in a larger buffer.
      const float32_t range_buffer = F360_Linear_Equation_With_Saturation(dist_to_obj_from_host_center, 0.0F, calibrations.k_range_buffer_max_dist, calibrations.k_range_buffer_min_val, calibrations.k_range_buffer_max_val);
      float32_t azimuth_buffer = F360_Linear_Equation_With_Saturation(dist_to_obj_from_host_center, 0.0F, calibrations.k_az_buffer_max_dist, calibrations.k_az_buffer_min_val, calibrations.k_az_buffer_max_val);

      const float32_t min_dist_for_tightened_azimuth_buffer = 20.0F; // [m]
      if (Is_Object_Valid_For_Tightened_Gates(object_track) &&
         (dist_to_obj_from_host_center > min_dist_for_tightened_azimuth_buffer))
      {
         // Constants for tightened azimuth buffer calculation
         const float32_t min_tightened_azimuth_buffer = 0.666F; // [m] corresponds to azimuth_buffer at range 20m
         const float32_t max_tightened_azimuth_buffer = 2.6F; // [m] corresponds to approximately 1.5 degree azimuth spread at 100m

         const float32_t tightened_azimuth_buffer = F360_Linear_Equation_With_Saturation(dist_to_obj_from_host_center, min_dist_for_tightened_azimuth_buffer, calibrations.k_az_buffer_max_dist, min_tightened_azimuth_buffer, max_tightened_azimuth_buffer);

         // Parameters for object with heading rate increase
         const float32_t min_heading_rate = 0.0F; // [rad/s]
         const float32_t max_heading_rate = 0.5F; // [rad/s]
         const float32_t min_heading_rate_increase = 0.0F; // [m]
         const float32_t max_heading_rate_increase = 0.8F; // [m]
         // Increase azimuth buffer for objects with heading rate
         const float32_t heading_rate_increase = F360_Linear_Equation_With_Saturation(std::abs(object_track.heading_rate), min_heading_rate, max_heading_rate, min_heading_rate_increase, max_heading_rate_increase);
         azimuth_buffer = std::min(azimuth_buffer, tightened_azimuth_buffer + heading_rate_increase);
      }
      // Reduce the allowed azimuth gate size for small objects
      const float32_t max_dim = fmaxf(object_track.bbox.Get_Length(), object_track.bbox.Get_Width());
      if ((max_dim < object_dimension_treshold) && (azimuth_buffer > max_azimuth_buffer))
      {
          // Parameters for linear-equation
          constexpr float32_t scale_factor_min_dist = 0.7F;
          constexpr float32_t scale_factor_max_dist = 1.2F;
          constexpr float32_t scale_factor_min_value = 0.6F;
          constexpr float32_t scale_factor_max_value = 1.0F;

          const float32_t scale_factor = F360_Linear_Equation_With_Saturation(max_dim, scale_factor_min_dist, scale_factor_max_dist, scale_factor_min_value, scale_factor_max_value);
          azimuth_buffer = fmaxf(max_azimuth_buffer, scale_factor * azimuth_buffer);
      }

      // Rotate az and range buffer to object parallell-orthogonal aligned coordinate system.
      const Angle vcs_aspect_angle_from_host_center = Angle{ F360_Atan2f(vcs_pos_y_dist, vcs_pos_x_dist) };
      const Angle total_angle = object_track.bbox.Get_Orientation() - vcs_aspect_angle_from_host_center;

      const float32_t abs_cos_tot = std::abs(total_angle.Cos());
      const float32_t abs_sin_tot = std::abs(total_angle.Sin());
      long_buffer += range_buffer * abs_cos_tot + azimuth_buffer * abs_sin_tot;
      lat_buffer += range_buffer * abs_sin_tot + azimuth_buffer * abs_cos_tot;
   }

   /*===========================================================================*\
      * FUNCTION: Increase_Buffer_Zone_In_Occluded_Parallel_Direction()
      *===========================================================================
      * RETURN VALUE:
      * None.
      *
      * PARAMETERS:
      * const F360_Object_Track_T& object_track    - Object track structure
      * const float32_t& dist_rear_axle_to_vcs_m  - Distance from rear axel to VCS origin
      * const F360_Calibrations_T& calibs          - Calibration structure
      * float32_t& long_buffer1                   - Association gates in positive long direction
      * float32_t& long_buffer2                   - Association gates in negative long direction
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
      * This function increases the association buffer zone in the object aligned parallel
      * direction (front/rear) that is least visble from host perspective. The least visible
      * of front and rear object sides are determined with help of the object aspect angle
      * (difference in angle between "the line from host center to object" and the "object 
      * pointing angle"). The size of the increase is also dependant on this aspect angle.
      * For an object with aspect angle 0 or 180 degrees (driving away or towards host) the
      * increase will be large while for crossing objects (with left or right side towards
      * host such that rear and front are equally visible) the increase is 0.
      * PRECONDITIONS:
      * None.
      *
      * POSTCONDITIONS:
      * None.
      \*===========================================================================*/

   void Increase_Buffer_Zone_In_Occluded_Parallel_Direction(
      const F360_Object_Track_T& object_track,
      const float32_t& dist_rear_axle_to_vcs_m,
      const F360_Calibrations_T& calibs,
      float32_t& long_buffer1,
      float32_t& long_buffer2)
   {
      if (object_track.speed > calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer)
      {
         constexpr float32_t extra_buffer_increase = 1.0F;

         float32_t breakpoints_speed[4];
         breakpoints_speed[0] = calibs.k_min_speed_for_increasing_occluded_long_assoc_buffer;
         breakpoints_speed[1] = calibs.k_max_speed_for_saturating_occluded_long_assoc_buffer_increase;
         breakpoints_speed[2] = calibs.k_second_min_speed_for_increasing_occluded_long_assoc_buffer;
         breakpoints_speed[3] = calibs.k_second_max_speed_for_saturating_occluded_long_assoc_buffer_increase;

         float32_t breakpoints_buffer[4];
         breakpoints_buffer[0] = calibs.k_min_occluded_long_buffer_increase;
         breakpoints_buffer[1] = calibs.k_max_occluded_long_buffer_increase;
         breakpoints_buffer[2] = calibs.k_max_occluded_long_buffer_increase;
         breakpoints_buffer[3] = calibs.k_max_occluded_long_buffer_increase + extra_buffer_increase;

         const float32_t buffer_increase = F360_Piecewise_Linear_Equation(object_track.speed, breakpoints_speed, breakpoints_buffer);

         // Compute object aspect angle
         const float32_t center_vcs_pos_x_dist = object_track.bbox.Get_Center().x + (dist_rear_axle_to_vcs_m * 0.5F);  // Distance from center of host to objects center
         const float32_t center_vcs_pos_y_dist = object_track.bbox.Get_Center().y;                                     // Distance from center of host to objects center
         const Angle vcs_aspect_angle_from_host_center = Angle{ F360_Atan2f(center_vcs_pos_y_dist, center_vcs_pos_x_dist) };
         const Angle total_angle = (vcs_aspect_angle_from_host_center - object_track.bbox.Get_Orientation()).Normalize();

         // Only increase in occluded direction
         if ((-F360_PI_2 < total_angle) && (total_angle <= F360_PI_2))
         {
            // Front is least visible
            long_buffer2 += buffer_increase * std::abs(total_angle.Cos()); // Increase buffer smoothly with help of aspect angle (i.e. based on how occluded the front is)
         }
         else
         {
            // Rear is least visible
            long_buffer1 += buffer_increase * std::abs(total_angle.Cos()); // Increase buffer smoothly with help of aspect angle (i.e. based on how occluded the rear is)
         }
      }
   }


   /*===========================================================================*\
   * FUNCTION: Find_Dets_In_Object_Vicinity()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track
   *   const rspp_variant_A::RSPP_Detection_List_T &raw_detections - raw detections list
   * const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Tracker_Info_T & tracker_info
   * uint32_t & num_dets_of_interest
   * int32_t (&det_idx_of_interest)[MAX_NUMBER_OF_DETECTIONS]
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
   * Find detections inside bounding box aligned with VCS axes that fully encloses 
   * extended bounding box of the object
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Find_Dets_In_Object_Vicinity(
      const F360_Object_Track_T & object_track,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t & num_dets_of_interest,
      int32_t (&det_idx_of_interest)[MAX_NUMBER_OF_DETECTIONS])
   {
      BoundingBox box{ object_track.bbox };
      Determine_Extended_Bounding_Box(object_track, box);
      const BboxCorners extended_bounding_box_vcs = box.Get_Corners();

      float32_t min_max_extended_box_vcs[2][2] = {};
      Get_Min_And_Max_Of_VCS_Box(extended_bounding_box_vcs, min_max_extended_box_vcs);

      // Get detections of interest, i.e. from VCS orientated box that covers the extended bounding box.
      Get_Det_Indexes_In_Vcs_Zone(
         min_max_extended_box_vcs[0][0],
         min_max_extended_box_vcs[0][1],
         min_max_extended_box_vcs[1][0],
         min_max_extended_box_vcs[1][1],
         raw_detections,
         detection_props,
         det_idx_of_interest,
         num_dets_of_interest);
   }

   static bool Is_Point_Inside_Reduced_Extended_Bounding_Box(const Point& point_vcs,
      const F360_Object_Track_T& obj)
   {

      BoundingBox box{ obj.bbox };
      constexpr float32_t width_extend_factor = 0.55F;
      box.Extend_Boundaries(
         obj.bbox.Get_Width() * width_extend_factor,
         obj.bbox.Get_Width() * width_extend_factor,
         obj.long_buffer_zone_len1,
         obj.long_buffer_zone_len2);
      const bool f_inside_reduced_extended_boundary_box = box.Contains(point_vcs);
      
      return f_inside_reduced_extended_boundary_box;
   }

   /*===========================================================================*\
   * FUNCTION: Decrease_Assosiaction_Gates_Width_For_Slow_Objects()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T & calibrations - calibration
   * const float32_t obj_width - width of the object
   * const float32_t obj_vsc_long_pos - longtitudinal position of the object in vcs coordinates
   * const float32_t obj_speed - speed of the object
   * float32_t obj_heading - heading of the object in radians
   * float32_t & lat_buffer - buffer for the increase of the width in assoc gates
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
   * Decrease_Association_Gates_Width_For_Slow_Objects decreases the width of the assocation
   * gates of an object by a substraction term, depended on the speed of the object.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   \*===========================================================================*/
   void Decrease_Association_Gates_Width_For_Slow_Objects(
      const F360_Calibrations_T& calibs,
      const float32_t obj_width,
      const float32_t obj_vsc_long_pos,
      const float32_t obj_speed,
      float32_t obj_heading,
      float32_t& lat_buffer)
   {
       constexpr float32_t vsc_long_pos_thres = 100.0F;
       constexpr float32_t heading_thres = 0.209F; // equivalent to 12 degrees 
       constexpr float32_t min_buff_thresh = 0.7F; // The minimum value that width of the association gates of the object can be

       // To handle angles that are bigger than 90 degress by their absolute value
       obj_heading = std::abs(obj_heading);
       if (obj_heading > F360_PI * 0.5F)
       {
           obj_heading = F360_PI - obj_heading;
       }


       /* Conditions to ensure the we reduce the width of the object's association gates
          only if it has slow speed  and that the object's width is above a threshold
          to avoid reducing the width of the assocation gates of very small objects
       */
       constexpr float32_t width_margin = 0.2F; // Margin to ensure that we modify only the desirable objects
       if ((obj_width > calibs.k_nonmoveable_target_diameter + width_margin) // To ensure we dont reduce the association gates for stationary objects and pedestrians and reduce the assc gates for thin objects
          && (std::abs(obj_vsc_long_pos) < vsc_long_pos_thres)
          && (obj_heading < heading_thres)
          && (lat_buffer > min_buff_thresh))
       {
          constexpr float32_t min_speed_thresh = 0.0F; // The opposite sigmoid function of the lateral buffer reduction will be saturated before 0
          constexpr float32_t max_speed_thresh = 3.0F; // The opposite sigmoid function of the lateral buffer reduction will be saturated before 0
          constexpr float32_t min_lat_buffer_reduction = 0.0F; // After max speed thresh the opposite sigmoid function will saturate at this value

          const float32_t lat_buffer_reduction = F360_Linear_Equation_With_Saturation(std::abs(obj_speed),
             min_speed_thresh,
             max_speed_thresh,
             lat_buffer, // lat_buffer1 and lat_buffer2 are equal so thats why we use only lat_buffer1
             min_lat_buffer_reduction);

          const float32_t lat_buff_new = std::abs(lat_buffer - lat_buffer_reduction);

          // This function sets a minimum lateral buffer limit of 0.7
          lat_buffer = F360_Saturate(lat_buff_new, min_buff_thresh, lat_buffer);    
       }
   }
}
