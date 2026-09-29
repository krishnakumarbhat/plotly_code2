/*===================================================================================*\
* FILE: f360_try_to_merge_two_objects.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definitions of Try_To_Merge_With_Other_Object() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#include "f360_try_to_merge_two_objects.h"

#include "f360_are_two_objects_preconditions_ok_for_merge.h"
#include "f360_update_merged_objects_properties.h"
#include "f360_move_dets_from_killed_to_kept_object.h"
#include "f360_verify_object_size.h"
#include "f360_calculate_merged_object_dimensions.h"
#include "f360_kill_obj_trk.h"
#include "f360_dimensions.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_object_sides.h"
#include "f360_get_reference_point_para_side.h"
#include "f360_update_object_maximum_rcs.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"

namespace f360_variant_A
{
    static void Determine_Maximum_Width_And_Adapt(
      const F360_Calibrations_T& calib,
      float32_t& width,
      float32_t& wid1,
      float32_t& wid2);

    static void Determine_Min_Max_Det_POS_in_Object_TCS(
       const Point& object1_center_vcs_position,
       const Angle& object1_vcs_pointing,
       const uint32_t obj_ndets,
       const uint32_t(&obj_detids)[MAX_DETS_IN_OBJ_TRK],
       const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
       float32_t& min_para,
       float32_t& max_para,
       float32_t& min_orth,
       float32_t& max_orth);

    static float32_t Determine_Det_Gap_Ratio(
       const float32_t& k_min_allowed_detection_spread,
       const float32_t& min1,
       const float32_t& max1,
       const float32_t& min2,
       const float32_t& max2);

   /*===========================================================================*\
   * FUNCTION: Check_If_Detection_Gaps_Support_Merge()
   *===========================================================================
   * RETURN VALUE:
   * bool f_dets_support_merge
   *
   * PARAMETERS:
   * const F360_Object_Track_T & obj1
   * const F360_Object_Track_T & obj2
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * Verifies detection continuity between two objects being considered for merge.
   * Projects all detections from both objects onto a longitudinal axis and
   * calculates the total spread and gap between detection clusters. This helps
   * identify suspicious merges where two separate objects (e.g., two bicyclists)
   * would be incorrectly merged.
   *
   * PRECONDITIONS:
   * Both objects must have valid associated detections
   *
   * POSTCONDITIONS:
   * Returns structure with continuity metrics
   \*===========================================================================*/
   bool Check_If_Detection_Gaps_Support_Merge(
      const F360_Object_Track_T& obj1,
      const F360_Object_Track_T& obj2,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      const float32_t k_min_allowed_detection_spread = 1.0F; /* minimum allowed detection spread threshold in either para or orth direction to avoid unstable ratios while computing gap to spread ratio */
      const float32_t k_max_allowed_para_gap_to_len_ratio = 0.25F; /* Max allowed threshold for longitudinal gap ratio */
      const float32_t k_max_allowed_orth_gap_to_width_ratio = 0.5F;  /* Max allowed threshold for orthogonal gap ratio */
      
      // Transform detections from obj1 to TCS and get longitudinal (x) and orthogonal (y) extents
      float32_t min1_para;
      float32_t max1_para;
      float32_t min1_orth;
      float32_t max1_orth;
      Determine_Min_Max_Det_POS_in_Object_TCS(
         obj1.bbox.Get_Center(),
         obj1.bbox.Get_Orientation(),
         obj1.ndets,
         obj1.detids,
         detection_props,
         min1_para,
         max1_para,
         min1_orth,
         max1_orth);
      
      // Transform detections from obj2 to TCS and get longitudinal (x) and orthogonal (y) extents
      float32_t min2_para;
      float32_t max2_para;
      float32_t min2_orth;
      float32_t max2_orth;
      Determine_Min_Max_Det_POS_in_Object_TCS(
         obj1.bbox.Get_Center(),
         obj1.bbox.Get_Orientation(),
         obj2.ndets,
         obj2.detids,
         detection_props,
         min2_para,
         max2_para,
         min2_orth,
         max2_orth);
      
      // Calculate longitudinal gap ratio(gap as fraction of total spread)
      const float32_t para_gap_ratio = Determine_Det_Gap_Ratio(
         k_min_allowed_detection_spread,
         min1_para,
         max1_para,
         min2_para,
         max2_para);

      // Calculate lateral gap ratio (gap as fraction of total spread)
      const float32_t orth_gap_ratio = Determine_Det_Gap_Ratio(
         k_min_allowed_detection_spread,
         min1_orth,
         max1_orth,
         min2_orth,
         max2_orth);
      
      
      // Check both longitudinal and orthogonal gaps
      const bool f_dets_support_merge = ((para_gap_ratio < k_max_allowed_para_gap_to_len_ratio) && (orth_gap_ratio < k_max_allowed_orth_gap_to_width_ratio));
            
      return f_dets_support_merge;
   }

   /*===========================================================================*\
   * FUNCTION: Try_To_Merge_Two_Objects()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T & host
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list
   * const F360_Calibrations_T & calib
   * const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
   * const int32_t idx1
   * const int32_t idx2
   * const F360_Globals_T& globals
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * F360_Tracker_Info_T & tracker_info
   * int32_t & kill_idx
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
   * Functions check whether two object could be merged and merges them if it 
   * is possible. 
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Try_To_Merge_Two_Objects(
      const F360_Host_T & host,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      const F360_Calibrations_T & calib,
      const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
      const int32_t idx1,
      const int32_t idx2,
      const F360_Globals_T& globals,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Tracker_Info_T & tracker_info,
      int32_t & kill_idx)
   {
      float32_t coarse_gate_score = 0.0F;
      if (Are_Two_Objects_Preconditions_OK_For_Merge(calib, object_tracks[idx1], object_tracks[idx2], detection_props, coarse_gate_score))
      {
         int32_t keep_idx;
         Choose_Obj_Idx_To_Be_Kept(globals, idx1, idx2, object_tracks[idx1], object_tracks[idx2], keep_idx, kill_idx);
         
         bool f_dets_support_merge = true;

         /* For slow moving objects (<5 m/s) with a weak coarse gating match, verify that detection gaps are small enough to justify merging 
            Note that this only affects objects with speed between 4 and 5 m/s, since if speed is below 4 m/s, the coarse gate above threshold blocks the merge. */
         const float32_t abs_mean_speed = std::abs(0.5F * (object_tracks[keep_idx].speed + object_tracks[kill_idx].speed));
         if ((abs_mean_speed < 5.0F) && (coarse_gate_score > calib.k_merging_coarse_score_gate))
         {
            f_dets_support_merge = Check_If_Detection_Gaps_Support_Merge(
               object_tracks[keep_idx],
               object_tracks[kill_idx],
               detection_props);
         }
         
         F360_Dimensions_T merged_obj_dimensions = Calculate_Merged_Object_Dimensions(detection_props, object_tracks[keep_idx], object_tracks[kill_idx]);

         if (f_dets_support_merge && Verify_Object_Size(object_tracks[keep_idx], object_tracks[kill_idx], calib, tracker_info, merged_obj_dimensions))
         {

            Determine_Maximum_Width_And_Adapt(calib, merged_obj_dimensions.width, merged_obj_dimensions.wid1, merged_obj_dimensions.wid2);
            const Point merged_object_center_vcs = Calculate_Merged_Obj_Center_And_Update_Width(object_tracks[keep_idx], merged_obj_dimensions);
            
            BoundingBox merged_bbox;
            merged_bbox.Set_Center(merged_object_center_vcs.x, merged_object_center_vcs.y);
            merged_bbox.Set_Length(merged_obj_dimensions.length);
            merged_bbox.Set_Width(merged_obj_dimensions.width);
            merged_bbox.Set_Orientation(object_tracks[keep_idx].bbox.Get_Orientation());

            const float32_t abs_closest_y_merged_obj = Get_Closest_Corner_VCS_Y(merged_bbox);
            const float32_t abs_closest_y_kill_obj = Get_Closest_Corner_VCS_Y(object_tracks[kill_idx].bbox);
            const float32_t abs_closest_y_keep_obj = Get_Closest_Corner_VCS_Y(object_tracks[keep_idx].bbox);

            //  Block the merge if no detections fall in within the bounding box of the merged object
            BoundingBox extended_merged_bbox{ merged_bbox };

            extended_merged_bbox.Extend_Boundaries(
               object_tracks[keep_idx].lat_buffer_zone_wid1,
               object_tracks[keep_idx].lat_buffer_zone_wid2,
               object_tracks[keep_idx].long_buffer_zone_len1,
               object_tracks[keep_idx].long_buffer_zone_len2);

            const bool f_no_detections_in_merged_ext_BBox = !Any_Detection_Within_Merged_Extended_BBox(
               detection_props,
               object_tracks[kill_idx],
               object_tracks[keep_idx],
               extended_merged_bbox);

            // Block merge if neither of the objects' closest point to host path is within 1.8m in y axis
            // and the potentialy new merged object's closest point to host path is within 1.8m in y axis
            constexpr float32_t in_host_path_tresh = 1.8F;
            const bool f_keep_or_kill_obj_already_in_path = (abs_closest_y_keep_obj < in_host_path_tresh) || (abs_closest_y_kill_obj < in_host_path_tresh);
            const bool f_merged_obj_in_host_path = abs_closest_y_merged_obj < in_host_path_tresh;
            const bool f_enters_host_path_after_merge = (!f_keep_or_kill_obj_already_in_path) && f_merged_obj_in_host_path;

            const bool f_not_allow_merge = f_enters_host_path_after_merge || f_no_detections_in_merged_ext_BBox;

            if (f_not_allow_merge)
            {
                // don't allow merge
            }
            else
            {               
                Update_Merged_Objects_Properties(object_tracks[kill_idx], calib, sep, host, sensors, globals, merged_object_center_vcs, merged_obj_dimensions, object_tracks[keep_idx]);

                Move_Dets_From_Killed_To_Kept_Object(host, tracker_info, sensors, raw_detection_list, calib, object_tracks[kill_idx], object_tracks[keep_idx], detection_props);
                
                object_tracks[keep_idx].maximum_rcs = std::fmaxf(object_tracks[kill_idx].maximum_rcs, object_tracks[keep_idx].maximum_rcs);
                object_tracks[keep_idx].average_rcs = std::fmaxf(object_tracks[kill_idx].average_rcs, object_tracks[keep_idx].average_rcs);
                
                const bool f_both_obj_freeze_timer_active = (object_tracks[keep_idx].time_since_obj_considered_veh_for_class_freeze > 0.0F) && (object_tracks[kill_idx].time_since_obj_considered_veh_for_class_freeze > 0.0F);
                const float32_t max_timer_value = std::fmaxf(object_tracks[keep_idx].time_since_obj_considered_veh_for_class_freeze, object_tracks[kill_idx].time_since_obj_considered_veh_for_class_freeze);
                const float32_t min_timer_value = std::fminf(object_tracks[keep_idx].time_since_obj_considered_veh_for_class_freeze, object_tracks[kill_idx].time_since_obj_considered_veh_for_class_freeze);
                object_tracks[keep_idx].time_since_obj_considered_veh_for_class_freeze = f_both_obj_freeze_timer_active ? min_timer_value : max_timer_value;

                // Detections associated to killed object should not be cleared since they have been moved to the kept object.
                Kill_Obj_Trk(kill_idx + 1, object_tracks, tracker_info);
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Choose_Obj_Idx_To_Be_Kept()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:

   * const F360_Globals_T& globals
   * const int32_t idx1
   * const int32_t idx2
   * const F360_Object_Track_T& obj1,
   * const F360_Object_Track_T& obj2,
   * uint32_t & keep_idx
   * uint32_t & kill_idx
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
   * Function decides, which object ID shall be assigned to newly merged object.
   * For objects that are old enough,
   *     - the CTCA object is selected if the other is CCA.
   *     - If both have the same filter type, the object closest to the average
   *       sensor position (from globals) is selected.
   * If either of the objects is not old enough,
   *     - the oldest is selected.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   void Choose_Obj_Idx_To_Be_Kept(
      const F360_Globals_T& globals,
      const int32_t idx1,
      const int32_t idx2,
      const F360_Object_Track_T& obj1,
      const F360_Object_Track_T& obj2,
      int32_t & keep_idx,
      int32_t & kill_idx)
   {
      constexpr float32_t min_obj_age_for_keep_obj_selection = 2.0F; // CCA objects need about 2 seconds after intialization to be allowed to transition to CTCA. Before 2 seconds they are considered too immature.
      const bool f_both_objects_old_enough = (obj1.time_since_initialization > min_obj_age_for_keep_obj_selection) && (obj2.time_since_initialization > min_obj_age_for_keep_obj_selection);
      if (f_both_objects_old_enough)
      {
         constexpr float32_t k_slow_moving_merge_speed_threshold = 3.0F;
         const bool f_both_objects_slow_moving = (obj1.speed < k_slow_moving_merge_speed_threshold) && (obj2.speed < k_slow_moving_merge_speed_threshold);
         // If the objects have different filter types and are slow moving, the CTCA object is kept
         if (f_both_objects_slow_moving && (F360_TRACKER_TRKFLTR_CTCA == obj1.trk_fltr_type) && (F360_TRACKER_TRKFLTR_CCA == obj2.trk_fltr_type))
         {
            keep_idx = idx1;
            kill_idx = idx2;
         }
         else if (f_both_objects_slow_moving && (F360_TRACKER_TRKFLTR_CTCA == obj2.trk_fltr_type) && (F360_TRACKER_TRKFLTR_CCA == obj1.trk_fltr_type))
         {
            keep_idx = idx2;
            kill_idx = idx1;
         }
         else
         {
            // If both objects are old enough and of the same filter type, keep object with closest reference point if the distance square is not close enough
            // If the distance square diff of two objects are close enough, then keep object with a larger bbox length
            // Distance is calculated from the average sensor position.
            const float32_t avg_sensor_pos_x = globals.average_sensor_position_x;
            const float32_t avg_sensor_pos_y = globals.average_sensor_position_y;

            const float32_t obj1_dx = obj1.vcs_position.x - avg_sensor_pos_x;
            const float32_t obj1_dy = obj1.vcs_position.y - avg_sensor_pos_y;
            const float32_t obj2_dx = obj2.vcs_position.x - avg_sensor_pos_x;
            const float32_t obj2_dy = obj2.vcs_position.y - avg_sensor_pos_y;
            
            const float32_t obj1_dist_sq = obj1_dx * obj1_dx + obj1_dy * obj1_dy;
            const float32_t obj2_dist_sq = obj2_dx * obj2_dx + obj2_dy * obj2_dy;

            constexpr float32_t k_dist_sq_threshold = 0.2F;
            const bool f_dist_diff_large_enough = std::abs(obj1_dist_sq - obj2_dist_sq) > k_dist_sq_threshold;

            if (f_dist_diff_large_enough && (obj2_dist_sq < obj1_dist_sq))
            {
               keep_idx = idx2;
               kill_idx = idx1;
            }
            else if (f_dist_diff_large_enough && (obj2_dist_sq > obj1_dist_sq))
            {
               keep_idx = idx1;
               kill_idx = idx2;
            }
            else if (obj1.bbox.Get_Length() > obj2.bbox.Get_Length())
            {
               keep_idx = idx1;
               kill_idx = idx2;
            }
            else
            {
               keep_idx = idx2;
               kill_idx = idx1;
            }
         }
      }
      else
      {
         // If any of the objects are newly created, keep the oldest
         if (obj2.time_since_initialization > obj1.time_since_initialization)
         {
            keep_idx = idx2;
            kill_idx = idx1;
         }
         else
         {
            keep_idx = idx1;
            kill_idx = idx2;
         }
      }

   }

   /*===========================================================================*\
    * FUNCTION: Get_Closest_Corner_VCS_Y()
    *===========================================================================
    * RETURN VALUE:
    * The closest y coordinate in vcs of the given bounding box
    *
    * PARAMETERS:
    * const BoundingBox& bbox
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
    * Returns the closest y coordinate in vcs of a given bounding box.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
    float32_t Get_Closest_Corner_VCS_Y(
       const BoundingBox& bbox)
    {
       float32_t closest_y_vcs_abs = INFTY;
       const BboxCorners obj_corners = bbox.Get_Corners();
       for (uint32_t i = 0U; i < 4U; ++i) {
          const float32_t y_vcs_abs = std::abs(obj_corners.points[i].y);
          if (y_vcs_abs < closest_y_vcs_abs) 
          {
             closest_y_vcs_abs = y_vcs_abs;
          }
       }
       return closest_y_vcs_abs;
    }

    /*===========================================================================*\
    * FUNCTION: Calculate_Merged_Obj_Center_And_Update_Width()
     *===========================================================================
    * RETURN VALUE:
    * the new center in vcs of the (potentially new) merged object.
    *
    * PARAMETERS:
    * const F360_Object_Track_T& obj,
    * const F360_Dimensions_T& dimensions,
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
    * Calculates the new center in vcs of the (potentially new) merged object
    * and updates the width.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
    Point Calculate_Merged_Obj_Center_And_Update_Width(
       const F360_Object_Track_T& obj,
       F360_Dimensions_T& dimensions)
    {
       // Compute center of new merged object bounding box
       float32_t new_center_vcs_x;
       float32_t new_center_vcs_y;

       const float32_t new_center_para = 0.5F * (dimensions.len2 - dimensions.len1);

       float32_t new_center_orth = 0.5F * (dimensions.wid2 - dimensions.wid1);
       // If the rear or front of the keep object is visible, use keep object's width (i.e. orth center in keep object's TCS = 0)
       float32_t new_width = dimensions.width;
       const F360_Object_Sides_T rear_or_front_side = Get_Reference_Point_Para_Side(obj.reference_point);
       const bool f_rear_or_front_visible = (F360_OBJECT_SIDES_INVALID != rear_or_front_side);
       if (f_rear_or_front_visible)
       {
          new_center_orth = 0.0F;
          new_width = obj.bbox.Get_Width();
       }
       dimensions.wid1 = new_width * 0.5F;
       dimensions.wid2 = dimensions.wid1;
       dimensions.width = new_width;

       Convert_TCS_Posn_To_VCS_Posn(
          new_center_para,
          new_center_orth,
          obj.bbox.Get_Center().x,
          obj.bbox.Get_Center().y,
          obj.bbox.Get_Orientation(),
          new_center_vcs_x,
          new_center_vcs_y);

        return Point(new_center_vcs_x, new_center_vcs_y);
    }
    /*===========================================================================*\
    * FUNCTION: Determine_Maximum_Width_And_Adapt()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Calibrations_T & calib
    * float32_t & width
    * float32_t & wid1
    * float32_t & wid2
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
    * Determine the maximum allowed width for the track based on its motion
    * properties (f_vehicular_trk and movable_prob). Depending on the result,
    * adapt wid1 and wid2 correspondingly.
    *
    * PRECONDITIONS:
    * All the Pointers should Point to valid structures.
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
    static void Determine_Maximum_Width_And_Adapt(
        const F360_Calibrations_T& calib,
        float32_t& width,
        float32_t& wid1,
        float32_t& wid2)
    {
        // Determine max width of merged object based on motion flags 
        const float32_t max_width = calib.k_movable_max_target_width;

        // Update width/wid1/wid2 if previous width is larger than maximum allowed width
        if (width > max_width)
        {
            const float32_t factor = 1.0F / width;
            wid1 = wid1 * factor * max_width;
            wid2 = wid2 * factor * max_width;
            width = max_width;
        }
    }

    /*===========================================================================*\
    * FUNCTION: Any_Detection_Within_Merged_Extended_BBox()
    *===========================================================================
    * RETURN VALUE:
    * True if at least one detection from either object falls within the merged
    * bounding box, false otherwise.
    *
    * PARAMETERS:
    * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
    * const F360_Object_Track_T& kill_obj,
    * const F360_Object_Track_T& keep_obj,
    * const BoundingBox& merged_bbox
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
    * Checks whether any detection associated with either the kept or killed
    * object is within the merged bounding box. This validation is used to
    * ensure that the merged object is supported by actual detections.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
    bool Any_Detection_Within_Merged_Extended_BBox(
       const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
       const F360_Object_Track_T& kill_obj,
       const F360_Object_Track_T& keep_obj,
       const BoundingBox& merged_bbox)

    {
       uint32_t det_idx{};
       bool detection_confirmation = false;

       // check obj1
       for (uint32_t i = 0U; (i < keep_obj.ndets) && (!detection_confirmation); i++)
        {
          det_idx = keep_obj.detids[i] - 1U;
          detection_confirmation = merged_bbox.Contains(detection_props[det_idx].vcs_position);
        }
 
        // check obj2
       for (uint32_t i = 0U; (i < kill_obj.ndets) && (!detection_confirmation); i++)
        {
          det_idx = kill_obj.detids[i] - 1U;
          detection_confirmation = merged_bbox.Contains(detection_props[det_idx].vcs_position);
        }
        return detection_confirmation;
     }

    /*===========================================================================*\
    * FUNCTION: Determine_Min_Max_Det_POS_in_Object_TCS()
    *===========================================================================
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const Point& object1_center_vcs_position
    * const Angle& object1_vcs_pointing
    * const uint32_t obj_ndets
    * const uint32_t(&obj_detids)[MAX_DETS_IN_OBJ_TRK]
    * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
    * float32_t& min_para
    * float32_t& max_para
    * float32_t& min_orth
    * float32_t& max_orth
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
    * Transforms all detections passed to the function from VCS to the object's TCS positions
    * From the transformed positions it determines the minimum and maximum longitudinal (para) and
    * orthogonal (orth) positions of the detections in the object's TCS.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
    static void Determine_Min_Max_Det_POS_in_Object_TCS(
       const Point& object1_center_vcs_position,
       const Angle& object1_vcs_pointing,
       const uint32_t obj_ndets,
       const uint32_t(&obj_detids)[MAX_DETS_IN_OBJ_TRK],
       const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
       float32_t& min_para,
       float32_t& max_para,
       float32_t& min_orth,
       float32_t& max_orth)
    {
       // Transform detections from obj2 to TCS and get longitudinal (x) and orthogonal (y) extents
       // The TCS position of detections should always be within 1000m, with respect to this usecase
       min_para = 1000.0F;
       max_para = -1000.0F;
       min_orth = 1000.0F;
       max_orth = -1000.0F;

       for (uint32_t i = 0U; i < obj_ndets; ++i)
       {
          const uint32_t raw_id = obj_detids[i];
          const uint32_t det_idx = raw_id - 1U;
          const F360_Detection_Props_T& det_props = detection_props[det_idx];

          float32_t det_tcs_x;  // Longitudinal
          float32_t det_tcs_y; // Orthogonal

          // Transform to obj1's TCS
          Convert_VCS_Posn_To_TCS_Posn(
             det_props.vcs_position.x, det_props.vcs_position.y,
             object1_center_vcs_position.x, object1_center_vcs_position.y,
             object1_vcs_pointing, det_tcs_x, det_tcs_y);

          min_para = std::min(min_para, det_tcs_x);
          max_para = std::max(max_para, det_tcs_x);
          min_orth = std::min(min_orth, det_tcs_y);
          max_orth = std::max(max_orth, det_tcs_y);
       }
    }

    /*===========================================================================*\
    * FUNCTION: Determine_Det_Gap_Ratio()
    *===========================================================================
    * RETURN VALUE:
    * float32_t gap_ratio
    *
    * PARAMETERS:
    * const float32_t & min_spread_for_ratio
    * const float32_t & min1
    * const float32_t & max1
    * const float32_t & min2
    * const float32_t & max2
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
    * Calculates the gap ratio between two detections of obj1 and obj2.
    * The gap is calculated along a single axis (either longitudinal or orthogonal).
    * The gap ratio is defined as the gap between
    * the detection clusters divided by the total spread of the clusters.
    * 
    * The function computes:
    * - Total spread: distance from the global minimum to global maximum position
    * - Gap: distance between the two clusters if they don't overlap (zero otherwise)
    * - Gap ratio: gap / spread (if spread exceeds min_spread_for_ratio threshold)
    *
    * A gap ratio of zero indicates overlapping clusters (favorable for merge), while
    * a high ratio indicates separated clusters (unfavorable for merge). The
    * min_spread_for_ratio threshold prevents unstable ratios for tightly clustered detections.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    \*===========================================================================*/
   static float32_t Determine_Det_Gap_Ratio(
      const float32_t & k_min_allowed_detection_spread,
      const float32_t & min1,
      const float32_t & max1,
      const float32_t & min2,
      const float32_t & max2)
   {
      // For para calculation: /Calculate longitudinal (parallel) spread and gap
      // For orth calculation: Calculate orthogonal (lateral) spread and gap
      const float32_t global_min = std::min(min1, min2);
      const float32_t global_max = std::max(max1, max2);
      const float32_t det_spread = global_max - global_min;

      // Calculate gap between longitudinal/orthogonal detection clusters
      // Gap exists only if clusters don't overlap
      float32_t det_gap;
      if (max1 < min2)
      {
         // For para calculation: obj1 is rear, obj2 is front
         // For orth calculation: obj1 detections on one side, obj2 on the other
         det_gap = min2 - max1;
      }
      else if (max2 < min1)
      {
         // For para calculation: obj2 is rear, obj1 is front
         // For orth calculation: obj2 detections on one side, obj1 on the other
         det_gap = min1 - max2;
      }
      else
      {
         // For para calculation: clusters overlap longitudinally - no gap (good for merge)
         // For orth calculation: clusters overlap laterally - no gap (good for merge)
         det_gap = 0.0F;
      }

      // For para calculation:  Calculate longitudinal gap ratio (gap as fraction of total spread)
      // For orth calculation:  Calculate lateral gap ratio (gap as fraction of total spread)
      float32_t gap_ratio;
      if (det_spread > k_min_allowed_detection_spread)
      {
         gap_ratio = det_gap / det_spread;
      }
      else
      {
         gap_ratio = 0.0F;
      }

      return gap_ratio;
   }
}
