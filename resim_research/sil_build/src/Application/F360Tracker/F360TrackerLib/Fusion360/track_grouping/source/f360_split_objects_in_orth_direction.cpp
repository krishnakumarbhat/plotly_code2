/*===========================================================================*\
* FILE: f360_split_objects_in_orth_direction.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Split_Objects_In_Orth_Direction()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_split_objects_in_orth_direction.h"
#include "f360_allocate_id_for_initialized_object.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_sort_priority.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_static_env_polys_support_functions.h"
#include "f360_find_detection_inliers.h"
#include <algorithm>
#include "f360_iterator.h"
#include "f360_update_object_reference_point.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Split_Objects_In_Orth_Direction()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Host_T & host
   * const F360_Calibrations_T& calibs
   * const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS]
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Globals_T& globals
   * F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Tracker_Info_T& tracker_info
   * F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS]
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
   * This function identifies which objects needs to be splitted and performs
   * the actual split
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Split_Objects_In_Orth_Direction(
      const F360_Host_T & host,
      const F360_Calibrations_T& calibs,
      const Static_Env_Poly_T(&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS])
   {
      // For all objects, reset the timer indicating that an object has recently been split if time since split is large enough
      constexpr float32_t k_time_to_reset_split_timer = 5.0F;
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const uint32_t obj_idx = static_cast<uint32_t>(tracker_info.active_obj_ids[i]) - 1U;
         if (objects[obj_idx].time_since_split > k_time_to_reset_split_timer)
         {
            objects[obj_idx].time_since_split = -1.0F;
         }
      }

      uint32_t nr_objects_to_split;
      uint32_t obj_idx_to_split[NUMBER_OF_OBJECT_TRACKS] = {};
      Find_Objects_To_Split(
         calibs,
         objects,
         tracker_info,
         nr_objects_to_split,
         obj_idx_to_split);

      for (uint32_t i = 0U; i < nr_objects_to_split; i++)
      {
         const uint32_t idx_to_split = obj_idx_to_split[i];

         const int32_t new_obj_id = Split_Single_Object_In_Ortho_Direction(
            host,
            calibs,
            sensors,
            globals,
            objects[idx_to_split],
            objects,
            tracker_info,
            det_p);

         // Re-evaluate on/behind SEP information
         const int32_t new_obj_idx = new_obj_id - 1;
         Flag_Single_Object_On_And_Behind_SEP(static_env_polys, calibs, globals, objects[idx_to_split]);
         Flag_Single_Object_On_And_Behind_SEP(static_env_polys, calibs, globals, objects[new_obj_idx]);

         // For both objects, start timer after split has occured
         objects[idx_to_split].time_since_split = 0.0F;
         objects[new_obj_idx].time_since_split = 0.0F;
      }


      // Resort in longitudinal direction as we have shifted position of original objects
      if (nr_objects_to_split > 0U)
      {
         Sorted_Tracks_Re_Sort(tracker_info);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Find_Objects_To_Split()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibs,
   * F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
   * const F360_Tracker_Info_T& tracker_info,
   * uint32_t& nr_objects_to_split,
   * uint32_t(&obj_idx_to_split)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function identifies which objects needs to be splitted and returns
   * the number of objects and an array of object indexes which needs split.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Find_Objects_To_Split(
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      uint32_t& nr_objects_to_split,
      uint32_t(&obj_idx_to_split)[NUMBER_OF_OBJECT_TRACKS]
      )
   {
      nr_objects_to_split = 0U;
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const uint32_t obj_idx = static_cast<uint32_t>(tracker_info.active_obj_ids[i]) - 1U;

         if (Is_Object_Valid_For_Split(calibs, objects[obj_idx]))
         {
            obj_idx_to_split[nr_objects_to_split] = obj_idx;
            nr_objects_to_split++;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Object_Valid_For_Split()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_need_split
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibs,
   * F360_Object_Track_T& object
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
   * This function determines if an object should be splitted
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Object_Valid_For_Split(
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& object)
   {
      bool f_need_split = false;
      object.split_type = NONE;
      if (((F360_TRACKER_TRKFLTR_CTCA == object.trk_fltr_type) || (F360_TRACKER_TRKFLTR_CCA == object.trk_fltr_type)) &&
           (object.speed > calibs.k_orth_split_min_speed))
         {
            //Adjust range rate and gap threshold depending on object length
            float32_t range_rate_threshold = 0.0F;
            float32_t gap_threshold = 0.0F;
            if (object.length_processed < calibs.k_orth_split_max_length_short_object)
            {
               // if object is short we allow split with lower threshold for gap distance and range-rate
               range_rate_threshold = calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
               gap_threshold = calibs.k_orth_split_min_orth_gap_for_split_short_objects;
            }
            else
            {
               range_rate_threshold = calibs.k_orth_split_min_rr_diff_for_medium_orth_gap;
               gap_threshold = calibs.k_orth_split_min_orth_gap_for_split_medium;
            }

            const float32_t tcs_y_pos_mean_diff = std::abs(object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin - object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin);
            const float32_t saturated_width_threshold = std::max(1.0F, object.bbox.Get_Width());

            const bool f_mean_pos_spread_above_threshold = tcs_y_pos_mean_diff > saturated_width_threshold;
            const bool f_is_closer_than_60m_in_x = std::abs(object.vcs_position.x) < 60.0F;

            if (object.orth_gap_filtered > calibs.k_orth_split_min_orth_gap_for_split_high)
            {
               // if orth_gap is very wide we always split
               object.split_type = ORTH_GAP_BASED;
               f_need_split = true;
            }
            else if ((object.orth_gap_filtered > gap_threshold) && (std::abs(object.orth_range_rate_diff_filtered) > range_rate_threshold))
            {
               // if orth_gap is medium/short object, we demand range_rate on both sides to be different
               object.split_type = ORTH_GAP_BASED;
               f_need_split = true;
            }
            else if ((object.orth_gap_filtered > calibs.k_orth_split_min_orth_gap_for_split_low) && (std::abs(object.orth_range_rate_diff_filtered) > calibs.k_orth_split_min_rr_diff_for_low_orth_gap))
            {
               //under certain condition we allow split for lower orth_gap and big range_rate difference
               const float32_t obj_long_pos_host_center = object.vcs_position.x + calibs.k_host_refl_half_host_length;
               const float32_t dist_sq = F360_Get_Hypotenuse_Squared(obj_long_pos_host_center, object.vcs_position.y);
               if ((dist_sq < calibs.k_orth_split_max_distance_sq_for_low_orth_gap) && (dist_sq > calibs.k_orth_split_min_distance_sq_for_low_orth_gap) && (object.bbox.Get_Length() < calibs.k_orth_split_max_length_for_low_orth_gap))
               {
                  object.split_type = ORTH_GAP_BASED;
                  f_need_split = true;
               }
            }
            else if (f_is_closer_than_60m_in_x && f_mean_pos_spread_above_threshold)
            {
               object.split_type = RR_ERROR_BASED;
               f_need_split = true;
            }
            else
            {
               // we do not split
            }
      
      }

      return f_need_split;
   }

   /*===========================================================================*\
   * FUNCTION: Split_Single_Object_In_Ortho_Direction()
   * ===========================================================================
   * RETURN VALUE:
   * int32_t new_id
   *
   * PARAMETERS:
   * const F360_Host_T & host,
   * const F360_Calibrations_T& calibs
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Globals_T& globals
   * F360_Object_Track_T& object_to_split
   * F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
   * F360_Tracker_Info_T& tracker_info
   * F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS]
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
   * This function splits a given object in orthogonal direction.
   * The objects are displaced in position by by the objects signal
   * "orth_delta_filtered"/2. The given object will be shifted in position
   * by -"orth_delta_filtered"/2 and the new object is created at position
   * "orth_delta_filtered"/2 in TCS. The new object inherits all other
   * properties of the original object.
   * The function returns the id of the new created object.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * Since we modify the position of the original object that needs split.
   * Resort the longitudinal sorted list by calling function Sorted_Tracks_Re_Sort()
   * after call to this function.
   *
   \*===========================================================================*/
   int32_t Split_Single_Object_In_Ortho_Direction(
      const F360_Host_T & host,
      const F360_Calibrations_T& calibs,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& object_to_split,
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      F360_Tracker_Info_T& tracker_info,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS])
   {
      // Create a new object
      const int32_t new_id = Allocate_Id_For_Initialized_Object(tracker_info, objects, det_p);
      F360_Object_Track_T& new_obj = objects[new_id - 1];

      // Inherit object properties
      Fill_New_Object_Properties(object_to_split, new_id, tracker_info.num_unique_objs, new_obj);

      // Insert created object to the priority sorted list
      Sort_Priority_With_New_Track(tracker_info, &new_obj);

      // Re-associate detections between original and newly created object
      Re_Associate_Detections(
         host,
         calibs,
         det_p,
         object_to_split,
         new_obj);

      // Shift position and adapt both object properties
      Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, object_to_split, new_obj);

      // Insert the newly created track on the vcs-longitudinal sorted list
      Sorted_Tracks_Insert(tracker_info, &new_obj);

      return new_id;
   }

   /*===========================================================================*\
   * FUNCTION: Fill_New_Object_Properties()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_to_split,
   * const int32_t new_obj_id,
   * const uint32_t new_unique_id,
   * F360_Object_Track_T& new_obj
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
   * This function copies relevant properties of the object that was splitted
   * to the newly created object
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Fill_New_Object_Properties(
      const F360_Object_Track_T& object_to_split,
      const int32_t new_obj_id,
      const uint32_t new_unique_id,
      F360_Object_Track_T& new_obj)
   {
      // Copy all properties of the object to split
      new_obj = object_to_split;

      // Adapt certain fields
      new_obj.id = new_obj_id;
      new_obj.reduced_id = F360_INVALID_REDUCED_ID;
      new_obj.reduced_status = F360_OBJECT_STATUS_INVALID;
      new_obj.unique_id = new_unique_id;
      new_obj.lsc_next_in_cluster = NULL;
      new_obj.lsc_prev_in_cluster = NULL;
      new_obj.p_higher_priority_track = NULL;
      new_obj.p_lower_priority_track = NULL;
   }

   /*===========================================================================*\
   * FUNCTION: Re_Associate_Detections()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Host_T & host
   * const F360_Calibrations_T& calibs,
   * F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T& object_to_split,
   * F360_Object_Track_T& new_object
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
   * This function performs the association of detections that should be
   * associated to the original or the newly created object after a split.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Re_Associate_Detections(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibs,
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object_to_split,
      F360_Object_Track_T& new_object)
   {
      uint32_t nr_dets_org_obj;
      uint32_t det_ids_org_obj[MAX_DETS_IN_OBJ_TRK] = {};
      uint32_t nr_dets_new_obj;
      uint32_t det_ids_new_obj[MAX_DETS_IN_OBJ_TRK] = {};
      Find_Re_Associated_Detections_Ids(
         object_to_split,
         det_p,
         nr_dets_org_obj,
         det_ids_org_obj,
         nr_dets_new_obj,
         det_ids_new_obj);

      Re_Associate_Detections_Single_Object(
        host,
         calibs,
         nr_dets_org_obj,
         det_ids_org_obj,
         det_p,
         object_to_split);

      Re_Associate_Detections_Single_Object(
         host,
         calibs,
         nr_dets_new_obj,
         det_ids_new_obj,
         det_p,
         new_object);

   }

   /*===========================================================================*\
   * FUNCTION: Find_Re_Associated_Detections_Ids()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_to_split,
   * const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
   * uint32_t& nr_dets_org_obj,
   * uint32_t(&org_obj_det_ids)[MAX_DETS_IN_OBJ_TRK],
   * uint32_t& nr_dets_new_obj,
   * uint32_t(&new_obj_det_ids)[MAX_DETS_IN_OBJ_TRK]
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
   * This function evaluates which detections should be associated to which object
   * when an object needs to be splitted. Convention is that object that needs
   * to splitted will be shifted to the left in orthogonal direction while the
   * newly created object is shifted to the right. Thus, we determine detection
   * association based on the detections orthogonal position before the object is
   * splitted. Detections with negative ortho position should be associated to
   * the original object while positive ortho position should be associated to
   * the new object that will be created.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Find_Re_Associated_Detections_Ids(
      const F360_Object_Track_T& object_to_split,
      const F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t& nr_dets_org_obj,
      uint32_t(&org_obj_det_ids)[MAX_DETS_IN_OBJ_TRK],
      uint32_t& nr_dets_new_obj,
      uint32_t(&new_obj_det_ids)[MAX_DETS_IN_OBJ_TRK])
   {
      nr_dets_org_obj = 0U;
      nr_dets_new_obj = 0U;

      // Convert all associated detections into TCS
      float32_t orth_pos[MAX_DETS_IN_OBJ_TRK] = {};
      for (uint32_t i = 0U; i < object_to_split.ndets; i++)
      {
         const uint32_t det_idx = object_to_split.detids[i] - 1U;

         Point det_pos_tcs = {};
         Convert_VCS_Posn_To_TCS_Posn(
            det_p[det_idx].vcs_position.x,
            det_p[det_idx].vcs_position.y,
            object_to_split.bbox.Get_Center().x,
            object_to_split.bbox.Get_Center().y,
            object_to_split.bbox.Get_Orientation(),
            det_pos_tcs.x,
            det_pos_tcs.y);

         orth_pos[i] = det_pos_tcs.y;
      }

      for (uint32_t i = 0U; i < object_to_split.ndets; i++)
      {
         if (orth_pos[i] < 0.0F)
         {
            // Original object will shift to the left
            org_obj_det_ids[nr_dets_org_obj] = object_to_split.detids[i];
            nr_dets_org_obj++;
         }
         else
         {
            // New object will shift to the right
            new_obj_det_ids[nr_dets_new_obj] = object_to_split.detids[i];
            nr_dets_new_obj++;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Re_Associate_Detections_Single_Object()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Host_T & host,
   * const F360_Calibrations_T& calibs,
   * const uint32_t& nr_dets,
   * const int32_t(&obj_det_ids)[MAX_DETS_IN_OBJ_TRK],
   * F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Object_Track_T& object
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
   * This function performs the association of given detections to a
   * given object
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Re_Associate_Detections_Single_Object(
      const F360_Host_T& host,
      const F360_Calibrations_T& calibs,
      const uint32_t nr_dets,
      const uint32_t(&obj_det_ids)[MAX_DETS_IN_OBJ_TRK],
      F360_Detection_Props_T(&det_p)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& object)
   {
      object.ndets = nr_dets;
      object.num_types_of_dets[0] = 0;
      object.num_types_of_dets[1] = 0;
      std::fill(cmn::begin(object.detids), cmn::end(object.detids), 0U);
      for (uint32_t i = 0U; i < nr_dets; i++)
      {
         object.detids[i] = obj_det_ids[i];

         const uint32_t det_idx = obj_det_ids[i] - 1U;
         // Update counter of associated detection types
         if (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING == det_p[det_idx].motion_status)
         {
            object.num_types_of_dets[0]++;
         }
         else
         {
            object.num_types_of_dets[1]++;
         }

         // Associate detection to object
         det_p[det_idx].object_track_id = object.id;
      }

      Find_Detection_Inliers_For_Single_Object(host, calibs, object, det_p);
   }

  /*===========================================================================*\
  * FUNCTION: Adapt_Objects_Properties_After_Orth_Split()
  * ===========================================================================
  * RETURN VALUE:
  * None.
  *
  * PARAMETERS:
  * const F360_Host_T & host
  * const F360_Calibrations_T& calibrations
  * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
  * const F360_Globals_T& globals
  * F360_Object_Track_T& org_object
  * F360_Object_Track_T& new_object
  *
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
  * This function adapts both the splitted and the newly created object's
  * properties.
  *
  * PRECONDITIONS:
  * None
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
   void Adapt_Objects_Properties_After_Orth_Split(
      const F360_Host_T & host,
      const F360_Calibrations_T& calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T& globals,
      F360_Object_Track_T& org_object,
      F360_Object_Track_T& new_object)
   {
      org_object.reference_point = F360_REFERENCE_POINT_CENTER;
      org_object.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
      new_object.reference_point = F360_REFERENCE_POINT_CENTER;
      new_object.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;

      // Compute shift position of original and newly created object
      // When split with ORTH_GAP_BASED:
      // New object is shifted by orth_delta_filtered/2 in TCS to the right
      // Original object is shifted by orth_delta_filtered/2 in TCS to the left
      // 
      // When split with RR_ERROR_BASED: 
      // New object is shifted to the furthest mean y pos in TCS
      // Original object is shifted to the closest mean y pos in TCS

      Point new_updated_pos = {};
      const float32_t y_dist_higher_bin_to_bbox_center = std::abs(org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin);
      const float32_t y_dist_lower_bin_to_bbox_center = std::abs(org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin);
      const bool f_higher_bin_mean_further_to_bbox_center_y = (y_dist_higher_bin_to_bbox_center > y_dist_lower_bin_to_bbox_center);

      float32_t org_object_pos_shift = 0.0F;
      float32_t new_object_pos_shift = 0.0F;

      const float32_t updated_width = std::min(0.5F * org_object.orth_delta_filtered * calibrations.k_orth_split_width_gain, calibrations.k_orth_split_width_threshold);

      if (org_object.split_type == RR_ERROR_BASED)
      {
         if (updated_width < 1.1F * std::abs(org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin - org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin))
         {
            org_object_pos_shift = f_higher_bin_mean_further_to_bbox_center_y ? org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin : org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin;
            new_object_pos_shift = f_higher_bin_mean_further_to_bbox_center_y ? org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin : org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin;
         }
         else
         {
            const float32_t mean_bin_pos = (org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin + org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin) * 0.5F;
            org_object_pos_shift = mean_bin_pos + 0.7F * updated_width;
            new_object_pos_shift = mean_bin_pos - 0.7F * updated_width;
         }
      }
      else
      {
         org_object_pos_shift = -0.5F * org_object.orth_delta_filtered;
         new_object_pos_shift = 0.5F * org_object.orth_delta_filtered;
      }

      Convert_TCS_Posn_To_VCS_Posn(
         0.0F,
         new_object_pos_shift,
         org_object.bbox.Get_Center().x,
         org_object.bbox.Get_Center().y,
         org_object.bbox.Get_Orientation(),
         new_updated_pos.x,
         new_updated_pos.y);

      const Point new_object_delta_vcs_pos(new_updated_pos.x - new_object.vcs_position.x, new_updated_pos.y - new_object.vcs_position.y);
      new_object.vcs_position.x = new_updated_pos.x;
      new_object.vcs_position.y = new_updated_pos.y;

      // Shift the state vector for pseudo hdg estimator for new object
      float32_t new_state_vec[6];
      new_state_vec[0] = new_object.pseudo_hdg_state_vec[0];
      new_state_vec[1] = new_object.pseudo_hdg_state_vec[1] + new_object.pseudo_hdg_state_vec[0] * new_object_delta_vcs_pos.x;
      new_state_vec[2] = new_object.pseudo_hdg_state_vec[2] + new_object.pseudo_hdg_state_vec[0] * new_object_delta_vcs_pos.y;
      new_state_vec[3] = new_object.pseudo_hdg_state_vec[3] + 2.0F * new_object.pseudo_hdg_state_vec[1] * new_object_delta_vcs_pos.x + new_object.pseudo_hdg_state_vec[0] * new_object_delta_vcs_pos.x * new_object_delta_vcs_pos.x;
      new_state_vec[4] = new_object.pseudo_hdg_state_vec[4] + 2.0F * new_object.pseudo_hdg_state_vec[2] * new_object_delta_vcs_pos.y + new_object.pseudo_hdg_state_vec[0] * new_object_delta_vcs_pos.y * new_object_delta_vcs_pos.y;
      new_state_vec[5] = new_object.pseudo_hdg_state_vec[5] + new_object.pseudo_hdg_state_vec[2] * new_object_delta_vcs_pos.x + new_object.pseudo_hdg_state_vec[1] * new_object_delta_vcs_pos.y + new_object.pseudo_hdg_state_vec[0] * new_object_delta_vcs_pos.x * new_object_delta_vcs_pos.y;
      new_object.pseudo_hdg_state_vec[0] = new_state_vec[0];
      new_object.pseudo_hdg_state_vec[1] = new_state_vec[1];
      new_object.pseudo_hdg_state_vec[2] = new_state_vec[2];
      new_object.pseudo_hdg_state_vec[3] = new_state_vec[3];
      new_object.pseudo_hdg_state_vec[4] = new_state_vec[4];
      new_object.pseudo_hdg_state_vec[5] = new_state_vec[5];

      Point org_updated_pos = {};

      Convert_TCS_Posn_To_VCS_Posn(
         0.0F,
         org_object_pos_shift,
         org_object.bbox.Get_Center().x,
         org_object.bbox.Get_Center().y,
         org_object.bbox.Get_Orientation(),
         org_updated_pos.x,
         org_updated_pos.y);

      const Point org_object_delta_vcs_pos(org_updated_pos.x - org_object.vcs_position.x, org_updated_pos.y - org_object.vcs_position.y);
      org_object.vcs_position.x = org_updated_pos.x;
      org_object.vcs_position.y = org_updated_pos.y;

      // Shift the state vector for pseudo hdg estimator for original object
      new_state_vec[0] = org_object.pseudo_hdg_state_vec[0];
      new_state_vec[1] = (org_object.pseudo_hdg_state_vec[1] + org_object.pseudo_hdg_state_vec[0] * org_object_delta_vcs_pos.x);
      new_state_vec[2] = (org_object.pseudo_hdg_state_vec[2] + org_object.pseudo_hdg_state_vec[0] * org_object_delta_vcs_pos.y);
      new_state_vec[3] = (org_object.pseudo_hdg_state_vec[3] + 2.0F * org_object.pseudo_hdg_state_vec[1] * org_object_delta_vcs_pos.x + org_object.pseudo_hdg_state_vec[0] * org_object_delta_vcs_pos.x * org_object_delta_vcs_pos.x);
      new_state_vec[4] = (org_object.pseudo_hdg_state_vec[4] + 2.0F * org_object.pseudo_hdg_state_vec[2] * org_object_delta_vcs_pos.y + org_object.pseudo_hdg_state_vec[0] * org_object_delta_vcs_pos.y * org_object_delta_vcs_pos.y);
      new_state_vec[5] = (org_object.pseudo_hdg_state_vec[5] + org_object.pseudo_hdg_state_vec[2] * org_object_delta_vcs_pos.x + org_object.pseudo_hdg_state_vec[1] * org_object_delta_vcs_pos.y + org_object.pseudo_hdg_state_vec[0] * org_object_delta_vcs_pos.x * org_object_delta_vcs_pos.y);
      org_object.pseudo_hdg_state_vec[0] = new_state_vec[0];
      org_object.pseudo_hdg_state_vec[1] = new_state_vec[1];
      org_object.pseudo_hdg_state_vec[2] = new_state_vec[2];
      org_object.pseudo_hdg_state_vec[3] = new_state_vec[3];
      org_object.pseudo_hdg_state_vec[4] = new_state_vec[4];
      org_object.pseudo_hdg_state_vec[5] = new_state_vec[5];

      // Update predicted position to mitigate drop in position confidence downstream
      org_object.predicted_vcs_position = org_object.vcs_position;
      new_object.predicted_vcs_position = new_object.vcs_position;

      // Adapt width and saturate to 1.8 m
      
      org_object.bbox.Set_Width(updated_width);
      new_object.bbox.Set_Width(org_object.bbox.Get_Width());
      new_object.bbox.Set_Length(org_object.bbox.Get_Length());

      // Adapt heading, pointing and speed
      org_object.vcs_heading = Angle{ org_object.filtered_pos_diff_heading };
      org_object.Set_Bbox_Orientation(org_object.vcs_heading);
      org_object.hdg_ptng_disagmt = 0.0F;
      org_object.curvature = 0.0F;
      org_object.heading_rate = 0.0F;
      const float32_t proj_of_old_speed_on_new_hdg_dir = org_object.vcs_velocity.longitudinal * org_object.vcs_heading.Cos() + org_object.vcs_velocity.lateral * org_object.vcs_heading.Sin();
      org_object.speed = proj_of_old_speed_on_new_hdg_dir;
      org_object.vcs_velocity.longitudinal = org_object.speed * org_object.vcs_heading.Cos();
      org_object.vcs_velocity.lateral = org_object.speed * org_object.vcs_heading.Sin();


      new_object.vcs_heading = org_object.vcs_heading;
      new_object.Set_Bbox_Orientation(org_object.bbox.Get_Orientation());
      new_object.hdg_ptng_disagmt = 0.0F;
      new_object.curvature = 0.0F;
      new_object.heading_rate = 0.0F;
      new_object.speed = org_object.speed;
      new_object.vcs_velocity.longitudinal = org_object.vcs_velocity.longitudinal;
      new_object.vcs_velocity.lateral = org_object.vcs_velocity.lateral;

      (void)std::copy(cmn::begin(calibrations.init_cca_pnt_filter_cov), cmn::end(calibrations.init_cca_pnt_filter_cov), cmn::begin(org_object.cca_pnt_filter_cov));
      (void)std::copy(cmn::begin(calibrations.init_cca_pnt_filter_cov), cmn::end(calibrations.init_cca_pnt_filter_cov), cmn::begin(new_object.cca_pnt_filter_cov));

      // Adapt RCS and class freeze timer
      new_object.maximum_rcs = org_object.maximum_rcs;
      new_object.average_rcs = org_object.average_rcs;
      new_object.time_since_obj_considered_veh_for_class_freeze = -1.0F;

      // Reset split signals
      org_object.orth_delta_filtered = 0.0F;
      new_object.orth_delta_filtered = 0.0F;
      org_object.orth_gap_filtered = 0.0F;
      new_object.orth_gap_filtered = 0.0F;
      org_object.orth_range_rate_diff_filtered = 0.0F;
      new_object.orth_range_rate_diff_filtered = 0.0F;

      org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 0.0F;
      new_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 0.0F;
      org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.0F;
      new_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.0F;
      org_object.filtered_rr_err_max_gap = 0.0F;
      new_object.filtered_rr_err_max_gap = 0.0F;

      org_object.filtered_pos_diff_heading = INFTY;
      new_object.filtered_pos_diff_heading = INFTY;
      org_object.prev_vcs_center_pos.x = org_object.bbox.Get_Center().x;
      org_object.prev_vcs_center_pos.y = org_object.bbox.Get_Center().y;
      new_object.prev_vcs_center_pos.x = new_object.bbox.Get_Center().x;
      new_object.prev_vcs_center_pos.y = new_object.bbox.Get_Center().y;

      /*
      Update Reference Point - Update object position but don't adjust velocity estimates etc.
      Reason for not updating velocity etc is that curvature/yawrate estimate is not good for
      two objects tracked as one so we cant trust this value (curvature is set to 0 in above code)
      */
      constexpr bool f_update_obj_states = true;
      constexpr bool f_update_obj_pos_only = true;
      constexpr bool f_current_object_behind_another_object = false;
      Update_Object_Reference_Point(
         host.dist_rear_axle_to_vcs_m,
         f_update_obj_states,
         f_update_obj_pos_only,
         f_current_object_behind_another_object,
         calibrations,
         sensors,
         globals,
         org_object);

      Update_Object_Reference_Point(
         host.dist_rear_axle_to_vcs_m,
         f_update_obj_states,
         f_update_obj_pos_only,
         f_current_object_behind_another_object,
         calibrations,
         sensors,
         globals,
         new_object);
   }
}
