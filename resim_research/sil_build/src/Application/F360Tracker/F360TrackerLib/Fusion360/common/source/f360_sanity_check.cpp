/*===================================================================================*\
* FILE: f360_sanity_check.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains function definitions for functions related to the overall confidence.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_math.h"
#include "f360_sanity_check.h"
#include "f360_get_wall_time.h"
namespace f360_variant_A
{

   /*===========================================================================*\
   * FUNCTION: Sanity_Check()
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Detection_Hist_T& detection_hist
   * const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
   * const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   * const F360_Tracker_Info_T& tracker_info
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
   * Crosscheck available information in internal tracker data structures to 
   * determine that information is consistent and there are no indications of
   * data corruption.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   bool Sanity_Check(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& detection_hist,
      const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      const F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
#ifdef NDEBUG
      (void)raw_detection_list;
      (void)det_props;
      (void)detection_hist;
      (void)clusters;
      (void)object_tracks;
      (void)tracker_info;
      timing_info.sanity_check = 0.0F;
      const bool f_any_fault = false;
#else
      const float32_t start_time = get_wall_time();
      bool cluster_detection_sum_fault = false;
      bool historic_detection_slot_occupation_fault = false;
      bool historic_detection_cluster_association_fault = false;
      bool detection_cluster_association_fault = false;

      int16_t sum_old_dets = 0;
      for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
      {
         const int16_t cluster_idx = tracker_info.active_cluster_ids[i] - 1;
         cluster_detection_sum_fault = ((clusters[cluster_idx].num_old_dets + clusters[cluster_idx].ndets) <= 0) && (!clusters[cluster_idx].f_to_be_killed);

         for (int16_t j = 0; j < clusters[cluster_idx].num_old_dets; j++)
         {
            const int16_t det_idx = clusters[cluster_idx].old_det_idx[j];
            if (!detection_hist.f_idx_occupied[det_idx])
            {
               historic_detection_slot_occupation_fault = true;
            }
            if (detection_hist.det_data[det_idx].cluster_idx != cluster_idx)
            {
               historic_detection_cluster_association_fault = true;
            }
            sum_old_dets++;

            if (historic_detection_cluster_association_fault || historic_detection_slot_occupation_fault)
            {
               break;
            }
         }

         for (int16_t j = 0; j < clusters[cluster_idx].ndets; j++)
         {
            const int16_t det_idx = clusters[cluster_idx].detids[j] - 1;
            if (det_props[det_idx].cluster_id != (cluster_idx + 1))
            {
               detection_cluster_association_fault = true;
               break;
            }
         }

         if (cluster_detection_sum_fault ||
            historic_detection_cluster_association_fault ||
            historic_detection_slot_occupation_fault ||
            detection_cluster_association_fault)
         {
            break;
         }
      }
      const bool historic_detection_count_fault = sum_old_dets != detection_hist.n_occupied;

      bool active_obj_id_range_fault = false;
      bool active_obj_status_fault = false;
      bool det_id_range_fault = false;
      bool state_bound_fault = false;

      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         if ((obj_idx < 0) || (obj_idx >= static_cast<int32_t>(tracker_info.variant.num_tracks)))
         {
            active_obj_id_range_fault = true;
         }
         else
         {
            const F360_Object_Track_T &obj = object_tracks[obj_idx];

            if (F360_OBJECT_STATUS_INVALID == obj.status)
            {
               active_obj_status_fault = true;
            }

            if (obj.ndets > 0U)
            {
               for (uint32_t j = 0U; j < obj.ndets; j++)
               {
                  const uint32_t det_id = obj.detids[j];
                  if ((det_id < 1U) || (det_id > raw_detection_list.number_of_valid_detections))
                  {
                     det_id_range_fault = true;
                  }
               }
            }

            const float32_t k_max_vcs_pos = 350.0F;
            const float32_t k_max_vcs_vel = 100.0F;
            const float32_t k_max_vcs_acc = 300.0F;
            if ((std::abs(obj.vcs_position.x) > k_max_vcs_pos) ||
               (std::abs(obj.vcs_position.y) > k_max_vcs_pos) ||
               (std::abs(obj.vcs_velocity.longitudinal) > k_max_vcs_vel) ||
               (std::abs(obj.vcs_velocity.lateral) > k_max_vcs_vel) ||
               (std::abs(obj.vcs_accel.longitudinal) > k_max_vcs_acc) ||
               (std::abs(obj.vcs_accel.lateral) > k_max_vcs_acc))
            {
               state_bound_fault = true;
            }
         }
      }

      bool inactive_obj_id_range_fault = false;
      bool inactive_obj_pos_fault = false;
      bool inactive_obj_status_fault = false;
      bool inactive_obj_ndets_fault = false;
      for (int32_t i = 0; i < (static_cast<int32_t>(tracker_info.variant.num_tracks) - tracker_info.num_active_objs); i++)
      {
         const int32_t obj_idx = tracker_info.inactive_obj_ids[i] - 1;
         if ((obj_idx < 0) || (obj_idx >= static_cast<int32_t>(tracker_info.variant.num_tracks)))
         {
            inactive_obj_id_range_fault = true;
         }
         else
         {
            if (object_tracks[obj_idx].status > F360_OBJECT_STATUS_INVALID)
            {
               inactive_obj_status_fault = true;
            }

            if (object_tracks[obj_idx].ndets > 0U)
            {
               inactive_obj_ndets_fault = true;
            }

            if (std::abs(object_tracks[obj_idx].vcs_position.x) > 0.0F)
            {
               inactive_obj_pos_fault = true;
            }
         }
      }

      bool sorted_tracks_fault = false;
      F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_start;
      int32_t n_tracks = 0;
      for (uint32_t i = 0U; i < tracker_info.variant.num_tracks; i++)
      {
         if (NULL != curr_trk)
         {
            if (curr_trk->status < F360_OBJECT_STATUS_NEW)
            {
               sorted_tracks_fault = true;
            }
            else
            {
               const float32_t prev_longpos = curr_trk->vcs_position.x;
               curr_trk = tracker_info.vcslong_sorted_next_track[curr_trk->id - 1];
               if ((NULL != curr_trk) && (prev_longpos > curr_trk->vcs_position.x))
               {
                  sorted_tracks_fault = true;
               }
            }
         }
         else
         {
            break;
         }
         n_tracks++;
      }

      if (n_tracks != tracker_info.num_active_objs)
      {
         sorted_tracks_fault = true;
      }

      const bool f_any_fault =
         cluster_detection_sum_fault ||
         historic_detection_slot_occupation_fault ||
         historic_detection_cluster_association_fault ||
         detection_cluster_association_fault ||
         historic_detection_count_fault ||
         active_obj_id_range_fault ||
         active_obj_status_fault ||
         det_id_range_fault ||
         state_bound_fault ||
         inactive_obj_id_range_fault ||
         inactive_obj_status_fault ||
         inactive_obj_ndets_fault ||
         inactive_obj_pos_fault ||
         sorted_tracks_fault;

      
      timing_info.sanity_check = get_wall_time() - start_time;

      assert(!f_any_fault);
#endif

      return f_any_fault;
   }
}
