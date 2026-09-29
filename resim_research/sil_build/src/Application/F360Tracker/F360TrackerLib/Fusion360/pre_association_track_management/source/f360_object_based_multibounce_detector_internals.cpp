/*===================================================================================*\
* FILE: f360_object_based_multibounce_detector_internals.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains implementations of internal functions related to object based multibounce 
*    detector.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_object_based_multibounce_detector_internals.h"
#include "f360_math_func.h"
#include "f360_intersections.h"
#include "f360_range_rates.h"
#include "f360_bounding_box.h"
#include <limits>



namespace f360_variant_A
{
   static inline bool Is_Det_Inside_Area(
      const float32_t det_range,
      const float32_t det_cos_az_vcs,
      const float32_t det_sin_az_vcs,
      const Point &sensor_mount_pos_vcs,
      const BoundingBox &restricted_area);

   /*===========================================================================*\
   * FUNCTION: Create_Area_Of_Correct_Detections()
   *===========================================================================
   * RETURN VALUE:
   * BoundingBox - Area
   *
   * PARAMETERS:
   * const F360_Object_Track_T &obj_track
   * const float32_t area_width
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
   * Create area boundingbox from object. Within this area detections are assumed to be
   * correct in term of not being multibounces for sure. Restricted area is simplified 
   * by assuming 0 deg orientation (even if object poinitng is different!)
   * More information about extending the area can be found at presentation attached to the DFD-2841 ticket.
   *
   * PRECONDITIONS:
   * Object vcs pointing should be relatively small (-12, 12) degree. 
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   BoundingBox Create_Area_Of_Correct_Detections(
      const F360_Object_Track_T &obj_track,
      const float32_t area_width)
   {
      float32_t min_lat;
      float32_t max_lat;
      float32_t extended_area_width;
      constexpr float32_t extension_to_area_width_for_close_objects = 1.0F;
      constexpr float32_t bounding_box_center_lateral_threshold = 2.5F;
      constexpr float32_t bounding_box_center_longitudal_threshold = 3.0F;

      if (obj_track.bbox.Get_Center().y > 0.0F)
      {
         if ((obj_track.bbox.Get_Center().y < bounding_box_center_lateral_threshold) && (obj_track.bbox.Get_Center().x < bounding_box_center_longitudal_threshold))
         {
             min_lat = obj_track.bbox.Get_Center().y - obj_track.bbox.Get_Width() * 0.5F;
             max_lat = min_lat + area_width + extension_to_area_width_for_close_objects;
         }
         else
         {
             min_lat = obj_track.bbox.Get_Center().y - obj_track.bbox.Get_Width() * 0.5F;
             max_lat = min_lat + area_width;
         }
      }
      else
      {
         if ((obj_track.bbox.Get_Center().y > bounding_box_center_lateral_threshold) && (obj_track.bbox.Get_Center().x < bounding_box_center_longitudal_threshold))
         {
             max_lat = obj_track.bbox.Get_Center().y + obj_track.bbox.Get_Width() * 0.5F;
             min_lat = max_lat - area_width - extension_to_area_width_for_close_objects;
         }
         else
         {
             max_lat = obj_track.bbox.Get_Center().y + obj_track.bbox.Get_Width() * 0.5F;
             min_lat = max_lat - area_width;
         }
      }

      const float32_t min_long = obj_track.bbox.Get_Center().x - obj_track.bbox.Get_Length() * 0.5F;
      const float32_t max_long = obj_track.bbox.Get_Center().x + obj_track.bbox.Get_Length() * 0.5F;

      const float32_t area_center_pos_long = (min_long + max_long) * 0.5F;
      const float32_t area_center_pos_lat = (min_lat + max_lat) * 0.5F;
      const Point area_center_point = Point(area_center_pos_long, area_center_pos_lat);
      const float32_t area_length = std::abs(max_long - min_long);
      if ((std::abs(obj_track.bbox.Get_Center().y) < bounding_box_center_lateral_threshold) && (obj_track.bbox.Get_Center().x < bounding_box_center_longitudal_threshold))
      {
          extended_area_width = area_width + extension_to_area_width_for_close_objects;
      }
      else
      {
          extended_area_width = area_width;
      }
      const Angle orientation{ 0.0F };
      return BoundingBox(area_center_point, area_length, extended_area_width, orientation);
   }

   /*===========================================================================*\
   * FUNCTION: Find_Bounce_Origin_Point()
   *===========================================================================
   * RETURN VALUE:
   * Bounce_Origin bounce_origin_point - Bounce origin point
   *
   * PARAMETERS:
   *  const Point &detection_pos_vcs
   *  const Point &sensor_mount_pos_vcs
   *  const BoundingBox &restricted_area
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
   * Estiamtes potential origin of multibounce detection. It is done by looking for
   * intersection between line created by sensor position and detection position with
   * line created based on reference track (almost its closer border). 
   *
   * PRECONDITIONS:
   * Asumed 0 orientation of restricted_area
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Bounce_Origin Find_Bounce_Origin_Point(
      const Point &detection_pos_vcs,
      const Point &sensor_mount_pos_vcs,
      const BoundingBox &restricted_area)
   {
      assert(std::abs(restricted_area.Get_Orientation().Value()) < F360_EPSILON);

      const Point area_center = restricted_area.Get_Center();
      const float32_t half_area_length = restricted_area.Get_Length() * 0.5F;

      const float32_t sensor_pos[2] = { sensor_mount_pos_vcs.x, sensor_mount_pos_vcs.y };
      const float32_t detection_pos[2] = { detection_pos_vcs.x, detection_pos_vcs.y };

      const float32_t pseudo_border_A[2] = { area_center.x + half_area_length, area_center.y };
      const float32_t pseudo_border_B[2] = { area_center.x - half_area_length, area_center.y };
    
      Bounce_Origin bounce_origin_point{ Point{}, false };
      float32_t intersection_x;
      float32_t intersection_y;
      if (Determine_Segments_Intersection_Limited(sensor_pos, detection_pos, pseudo_border_A, pseudo_border_B, intersection_x, intersection_y))
      {
         bounce_origin_point.position.Set_Position(intersection_x, intersection_y);
         bounce_origin_point.f_found = true;
      }
 
      return bounce_origin_point;
   }

   /*===========================================================================*\
   * FUNCTION: Estimate_Num_Bounces()
   *===========================================================================
   * RETURN VALUE:
   * uint16_t num_bounces - Number of bounces
   *
   * PARAMETERS:
   * const Point &sensor_mount_pos_vcs,
   * const Point &bounce_origin_point,
   * const float32_t det_range
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
   * Estiamtes number of bounces for range and rage rate correction.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   uint16_t Estimate_Num_Bounces(
      const Point &sensor_mount_pos_vcs,
      const Point &bounce_origin_point,
      const float32_t det_range)
   {
      const float32_t ref_range = F360_Get_Hypotenuse(sensor_mount_pos_vcs.x - bounce_origin_point.x, sensor_mount_pos_vcs.y - bounce_origin_point.y);
      uint16_t num_bounces;

      if (ref_range < F360_EPSILON)
      {
         num_bounces = std::numeric_limits<uint16_t>::max();
      }
      else
      {
         const float32_t division_factor = det_range / ref_range;
         num_bounces = static_cast<uint16_t>(F360_Roundf(division_factor));
      }

      return num_bounces;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Object_Based_Multibounce()
   *===========================================================================
   * RETURN VALUE:
   * bool f_result - True if detection is multibounce
   *
   * PARAMETERS:
   *  const F360_VCS_Velocity_T &sensor_velocity
   *  const Point &sensor_mount_pos_vcs
   *  const F360_Detection_Props_T &det_prop
   *  const rspp_variant_A::RSPP_Detection_T &det_raw
   *  const F360_VCS_Velocity_T &obj_velocity_vcs
   *  const BoundingBox &restricted_area
   *  const F360_Calibrations_T& calibs
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
   * Checks range and range rate hypothesis for multibounce phenomena. It takes 
   * potential detection, estimates its factor and "moves" it back to potential 
   * original position and recaclcualte range rate.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Det_Object_Based_Multibounce(
      const F360_VCS_Velocity_T &sensor_velocity,
      const Point &sensor_mount_pos_vcs,
      const F360_Detection_Props_T &det_prop,
      const rspp_variant_A::RSPP_Detection_T &det_raw,
      const F360_VCS_Velocity_T &obj_velocity_vcs,
      const BoundingBox &restricted_area,
      const F360_Calibrations_T& calibs)
   {
      bool f_result = false;
      const Bounce_Origin bounce_origin_point = Find_Bounce_Origin_Point(det_prop.vcs_position, sensor_mount_pos_vcs, restricted_area);

      if (bounce_origin_point.f_found)
      {
         const uint16_t num_bounces = Estimate_Num_Bounces(sensor_mount_pos_vcs, bounce_origin_point.position, det_raw.raw.range);
         const uint16_t min_num_bounces = 2U;

         if ((min_num_bounces <= num_bounces) && (num_bounces <= calibs.mb_max_num_bounces))
         {
            // Zero-division protected by min_num_bounces
            const float32_t new_range = det_raw.raw.range / static_cast<float32_t>(num_bounces); 

            if (Is_Det_Inside_Area(new_range, det_raw.processed.cos_vcs_az, det_raw.processed.sin_vcs_az, sensor_mount_pos_vcs, restricted_area))
            {
               // Based on True multipath model and assumption that in multibounce case each relative range rate has same value then below range rate recalcualtion is done
               const float32_t corrected_rr = det_raw.raw.range_rate / static_cast<float32_t>(num_bounces);
               const float32_t rr_comp = Calculate_Det_Range_Rate_Comp(det_raw.processed.cos_vcs_az, det_raw.processed.sin_vcs_az, corrected_rr, sensor_velocity.lateral, sensor_velocity.longitudinal);
               const float32_t suspect_rr_diff = Calc_Range_Rate_Difference_From_Expectations(obj_velocity_vcs, det_raw.processed.cos_vcs_az, det_raw.processed.sin_vcs_az, rr_comp);

               f_result = (suspect_rr_diff < calibs.mb_range_rate_diff_th);
            }
         }            
      }

      return f_result;
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_On_New_Range_Inside_Area()
   *===========================================================================
   * RETURN VALUE:
   * bool - True if detection is inside area
   *
   * PARAMETERS:
   *  const float32_t det_range,
   *  const float32_t det_cos_az_vcs,
   *  const float32_t det_sin_az_vcs,
   *  const Point &sensor_mount_pos_vcs,
   *  const BoundingBox &restricted_area
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
   * Verify if detection with provided (new) range is inside area.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static inline bool Is_Det_Inside_Area(
      const float32_t det_range,
      const float32_t det_cos_az_vcs,
      const float32_t det_sin_az_vcs,
      const Point &sensor_mount_pos_vcs,
      const BoundingBox &restricted_area)
   {
      const float32_t long_component = det_range * det_cos_az_vcs;
      const float32_t lat_component = det_range * det_sin_az_vcs;
      const Point new_pos = {
         sensor_mount_pos_vcs.x + long_component,
         sensor_mount_pos_vcs.y + lat_component
      };

      return restricted_area.Contains(new_pos);
   }
}
