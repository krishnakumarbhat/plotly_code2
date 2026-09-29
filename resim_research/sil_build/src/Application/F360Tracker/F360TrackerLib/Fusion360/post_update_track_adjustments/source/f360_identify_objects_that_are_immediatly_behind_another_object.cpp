/*===================================================================================*\
* FILE: Identify_Objects_That_Are_Immediatly_Behind_Another_Object.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions related to Identify_Objects_That_Are_Immediatly_Behind_Another_Object()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_identify_objects_that_are_immediatly_behind_another_object.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_norm_heading_angle.h"

namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Is_Object_Oncoming_Or_Ongoing()
   *===========================================================================
   * RETURN VALUE:
   * f_obj_oncoming_or_ongoing
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks if an object is oncoming or ongoing, 
   * depending on it's heading
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Is_Object_Oncoming_Or_Ongoing(
      const float32_t abs_object_heading)
   {
      constexpr float32_t k_heading_threshold_for_object_to_be_ongoing_or_oncoming = F360_DEG2RAD(4.0F);
      // Check if the object is Oncoming or Ongoing
      const bool f_obj_oncoming_or_ongoing = ((abs_object_heading < k_heading_threshold_for_object_to_be_ongoing_or_oncoming)
         || ((F360_PI - abs_object_heading) < k_heading_threshold_for_object_to_be_ongoing_or_oncoming));

      return f_obj_oncoming_or_ongoing;
   }

   /*===========================================================================*\
   * FUNCTION: Check_Preconditions_For_An_Object_To_Be_Behind_Another_Object()
   *===========================================================================
   * RETURN VALUE:
   * f_preconditions_ok_for_an_object_to_be_behind_another_object
   *
   * PARAMETERS:
   * const F360_Object_Track_T& obj1
   * const F360_Object_Track_T& obj2
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function checks the preconditions to do further checks for an object to be behind another object
   * The following conditions are checked:
   * Obj1 and Obj2 are within distance threshold to each other
   * Obj1 and Obj2 have similar heading
   * Obj1 and Obj2 have similar speed
   * Obj1 and Obj2 are both either oncoming or ongoing
   * 
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Check_Preconditions_For_An_Object_To_Be_Behind_Another_Object(
      const F360_Object_Track_T& obj1,
      const F360_Object_Track_T& obj2)
   {
      constexpr float32_t k_dist_squared_between_objects_threshold = 900.0F; // 30.0 * 30.0F
      constexpr float32_t k_similar_heading_between_two_objects_threshold = F360_DEG2RAD(10.0F);
      constexpr float32_t k_similar_speed_between_two_objects_threshold = 1.0F; //1m/s

      // Check if distance between objects is within threhsold
      const float32_t distance_between_obj1_and_obj2 = F360_Get_Hypotenuse_Squared((obj1.bbox.Get_Center().x - obj2.bbox.Get_Center().x), (obj1.bbox.Get_Center().y - obj2.bbox.Get_Center().y));
      const bool f_distance_ok = (distance_between_obj1_and_obj2 < k_dist_squared_between_objects_threshold);

      const float32_t normalized_hdg = Normalize_Heading_Angle(obj2.vcs_heading.Value(), obj1.vcs_heading.Value()); // Makes second object vcs heading "near" first object heading. 
      // Check next object only if it has similar heading as the first one
      const bool f_hdg_similar_enough = std::abs(obj1.vcs_heading.Value() - normalized_hdg) <= k_similar_heading_between_two_objects_threshold;
      
      // Check next object only if it has similar speed as the first one
      const bool f_speed_similar_enough = std::abs(obj1.speed - obj2.speed) <= k_similar_speed_between_two_objects_threshold;

      const bool f_preconditions_ok_for_an_object_to_be_behind_another_object = ((f_hdg_similar_enough) && (f_speed_similar_enough) && (f_distance_ok));

      return f_preconditions_ok_for_an_object_to_be_behind_another_object;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Obj2_Points_Position_In_Obj1_Coordinate_System()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const Angle& obj1_orientation_angle
   * const Point& obj1_center_inVCS
   * const Point& first_corner_point_inVCS
   * const Point& second_corner_point_inVCS
   * Point(&obj2_points_in_obj1_CS)[3]
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function find the position of object_2's Front or Rear edge reference points
   * in object_1's coordinate system
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Get_Obj2_Points_Position_In_Obj1_Coordinate_System(
      const Angle& obj1_orientation_angle,
      const Point& obj1_center_inVCS,
      const Point& first_corner_point_inVCS,
      const Point& second_corner_point_inVCS,
      Point(&obj2_points_in_obj1_CS)[3])
   {
      Convert_VCS_Posn_To_TCS_Posn(
         first_corner_point_inVCS.x,
         first_corner_point_inVCS.y,
         obj1_center_inVCS.x,
         obj1_center_inVCS.y,
         obj1_orientation_angle,
         obj2_points_in_obj1_CS[0].x,
         obj2_points_in_obj1_CS[0].y);

      Convert_VCS_Posn_To_TCS_Posn(
         second_corner_point_inVCS.x,
         second_corner_point_inVCS.y,
         obj1_center_inVCS.x,
         obj1_center_inVCS.y,
         obj1_orientation_angle,
         obj2_points_in_obj1_CS[1].x,
         obj2_points_in_obj1_CS[1].y);

      obj2_points_in_obj1_CS[2].x = (obj2_points_in_obj1_CS[0].x + obj2_points_in_obj1_CS[1].x) * 0.5F;
      obj2_points_in_obj1_CS[2].y = (obj2_points_in_obj1_CS[0].y + obj2_points_in_obj1_CS[1].y) * 0.5F;
   }


   /*===========================================================================*\
   * FUNCTION: Identify_Objects_That_Are_Immediatly_Behind_Another_Object()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t k_far_away_object_dist_sq_thr,
   * const F360_Tracker_Info_T& tracker_info,
   * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * bool(&f_object_is_behind_another_object_array)[NUMBER_OF_OBJECT_TRACKS])
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function parses over all objects, in order to find object pairs, 
   * which are below 80m from host and are similar in speed (less than 1m/s diff),
   * heading (less than 10 deg diff), and are either ongoing (hdg ~= 0 deg)
   * or oncoming (i.e hdg ~= 180 deg).
   * 
   * For found pairs, the occlusion check is done by extending the bbox of rear object 
   * laterally on both sides and longitudinally towards the front, then it checks
   * if any of the trailing closest edge reference point is inside the extended bbox.
   * If so, then the object behind is considered to be occluded by its pair object and
   * f_object_is_behind_another_object_array[obj2_idx] is set.
   *
   * f_object_is_behind_another_object_array[obj2_idx] is used in Update_Object_Reference_Point(),
   * to only allow corner reference points as valid ref point choices for that object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Identify_Objects_That_Are_Immediatly_Behind_Another_Object(
      const float32_t k_far_away_object_dist_sq_thr,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      bool(&f_object_is_behind_another_object_array)[NUMBER_OF_OBJECT_TRACKS])
   {
      const F360_Object_Track_T* obj1 = tracker_info.vcslong_sorted_start;
      for (int32_t i = 0; i < (tracker_info.num_active_objs - 1); i++)
      {
         if (NULL == obj1)
         {
            break;
         }
         const int32_t idx1 = obj1->id - 1;
         // Note: The (obj1->vcs_position.x > 0.0F) is a must prerequisite condition for Check_If_Object_is_Behind_Another_Object() to work as designed
         // i.e This is not applicable to objects with negative vcs_position.x
         if (obj1->f_moving
            && (obj1->vcs_position.x > 0.0F)
            && (F360_Get_Hypotenuse_Squared(obj1->vcs_position.x, obj1->vcs_position.y) < k_far_away_object_dist_sq_thr)
            && (Is_Object_Oncoming_Or_Ongoing(std::abs(obj1->vcs_heading.Value()))))
         {
            Check_If_Object_is_Behind_Another_Object(k_far_away_object_dist_sq_thr, tracker_info, idx1, object_tracks, f_object_is_behind_another_object_array);
         }
         obj1 = tracker_info.vcslong_sorted_next_track[idx1];
      }
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Object_is_Behind_Another_Object()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t k_far_away_object_dist_sq_thr,
   * const F360_Tracker_Info_T& tracker_info,
   * const int32_t idx1,
   * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
   * bool(&f_object_is_behind_another_object_array)[NUMBER_OF_OBJECT_TRACKS])
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function parses over all objects, in order to find object pairs,
   * which are below 80m from host and are similar in speed (less than 1m/s diff),
   * heading (less than 10 deg diff), and are either ongoing (hdg ~= 0 deg)
   * or oncoming (i.e hdg ~= 180 deg).
   *
   * For found pairs, the occlusion check is done by extending the bbox of rear object
   * laterally on both sides and longitudinally towards the front, then it checks
   * if any of the trailing closest edge reference point is inside the extended bbox.
   * If so, then the object behind is considered to be occluded by its pair object and
   * f_object_is_behind_another_object_array[obj2_idx] is set.
   *
   * f_object_is_behind_another_object_array[obj2_idx] is used in Update_Object_Reference_Point(),
   * to only allow corner reference points as valid ref point choices for that object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Check_If_Object_is_Behind_Another_Object(
      const float32_t k_far_away_object_dist_sq_thr,
      const F360_Tracker_Info_T& tracker_info,
      const int32_t idx1,
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      bool(&f_object_is_behind_another_object_array)[NUMBER_OF_OBJECT_TRACKS])
   {
      int32_t idx2 = idx1;
      const F360_Object_Track_T obj1 = object_tracks[idx1];
      bool f_take_next_object = true;
      for (int32_t i = 0; ((i < tracker_info.num_active_objs) && (f_take_next_object)); i++)
      {
         const F360_Object_Track_T* const obj2 = tracker_info.vcslong_sorted_next_track[idx2];
         if (NULL != obj2)
         {
            idx2 = obj2->id - 1;
            if (obj2->f_moving
               && (F360_Get_Hypotenuse_Squared(obj2->vcs_position.x, obj2->vcs_position.y) < k_far_away_object_dist_sq_thr)
               && (Is_Object_Oncoming_Or_Ongoing(std::abs(obj2->vcs_heading.Value()))))
            {
               const bool f_preconditions_ok = Check_Preconditions_For_An_Object_To_Be_Behind_Another_Object(obj1, *obj2);
               if (f_preconditions_ok)
               {
                  // Create a box on the "other" side of the closest object to check if the other object is inside it. Based on object speed: faster object -> longer zone
                  // (Note that this loops over objects in ascending vcs x position order)
                  float32_t search_box[2][2] = {};
                  constexpr float32_t k_min_long_extension_for_high_speed_objects = 10.0F; // Applicable for objects with speed above 16.67m/s 
                  constexpr float32_t k_min_lat_extension_for_high_speed_objects = 1.3F; // Applicable for objects with speed above 13m/s
                  constexpr float32_t k_long_extension_speed_scale_factor = 0.6F;
                  constexpr float32_t k_lat_extension_speed_scale_factor = 0.1F;
                  const float32_t search_zone_long_extension = std::min(k_min_long_extension_for_high_speed_objects, obj1.speed * k_long_extension_speed_scale_factor);
                  const float32_t half_width_search_zone_extension = std::min(k_min_lat_extension_for_high_speed_objects, obj1.speed * k_lat_extension_speed_scale_factor);

                  const float32_t half_length = 0.5F * obj1.bbox.Get_Length();
                  const float32_t half_width = 0.5F * obj1.bbox.Get_Width();

                  const BboxCorners obj2_corners_vcs = obj2->bbox.Get_Corners();

                  constexpr uint8_t Num_Relevant_Obj2_Edge_Points = 3U;
                  Point obj2_points_in_obj1_CS[Num_Relevant_Obj2_Edge_Points] = {};
                  if (obj1.vcs_velocity.longitudinal < 0.0F)
                  {
                     // First object is oncoming -> create a zone behind it
                     search_box[0][0] = -(half_length + search_zone_long_extension);
                     search_box[0][1] = -half_length;
                     search_box[1][0] = -(half_width + half_width_search_zone_extension);
                     search_box[1][1] = (half_width + half_width_search_zone_extension);

                     // Get the obj2's points position in in obj1's coordinate system
                     Get_Obj2_Points_Position_In_Obj1_Coordinate_System(
                        obj1.bbox.Get_Orientation(),
                        obj1.bbox.Get_Center(),
                        obj2_corners_vcs.Front_Left(),
                        obj2_corners_vcs.Front_Right(),
                        obj2_points_in_obj1_CS);
                  }
                  else
                  {
                     // First object "ongoing", i.e. same direction as host -> look for second ("occluded") object behind it.
                     search_box[0][0] = half_length;
                     search_box[0][1] = half_length + search_zone_long_extension;
                     search_box[1][0] = -(half_width + half_width_search_zone_extension);
                     search_box[1][1] = (half_width + half_width_search_zone_extension);

                     // Get the obj2's points position in in obj1's coordinate system
                     Get_Obj2_Points_Position_In_Obj1_Coordinate_System(
                        obj1.bbox.Get_Orientation(),
                        obj1.bbox.Get_Center(),
                        obj2_corners_vcs.Rear_Left(),
                        obj2_corners_vcs.Rear_Right(),
                        obj2_points_in_obj1_CS);
                  }

                  for (uint8_t index = 0U; index < Num_Relevant_Obj2_Edge_Points; index++)
                  {
                     if (Check_If_Point_Is_Inside_Box_In_Same_CS(obj2_points_in_obj1_CS[index].x, obj2_points_in_obj1_CS[index].y, search_box))
                     {
                        f_object_is_behind_another_object_array[idx2] = true;
                        f_take_next_object = false;
                        break;
                     }
                  }
               }
            }
            if (f_take_next_object)
            {
               constexpr float32_t k_max_allowed_long_pos_diff_to_continue_search = 40.0F;
               f_take_next_object = (std::abs(obj2->vcs_position.x - obj1.vcs_position.x) < k_max_allowed_long_pos_diff_to_continue_search);
            }
         }
         else
         {
            f_take_next_object = false;
         }
      }
   }
}
