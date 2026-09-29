/*===========================================================================*\
* FILE: f360_clustering_data_generator.cpp
*============================================================================
* Copyright ? 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential ? Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains support functions for detections data generation for unit testing
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [30-Mar-2018]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_clustering_data_generator.h"

#include "f360_data_generator.h"
#include "f360_clustering.h"
#include "f360_constants.h"
#include "f360_math.h"

namespace f360_variant_A
{
   void Setup_TrackerInfo_Cluster_Detp(
      F360_Tracker_Info_T &tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      rspp_variant_A::RSPP_Detection_List_T &raw_detection_list
   )
   {
      tracker_info.vcslong_sorted_cluster_start = NULL;


      for (int16_t i = 0; i < NUMBER_OF_CLUSTERS; i++)
      {
         tracker_info.active_cluster_ids[i] = 0;
         tracker_info.inactive_cluster_ids[i] = i + 1;
         tracker_info.vcslong_sorted_cluster_next[i] = NULL;
         tracker_info.vcslong_sorted_cluster_prev[i] = NULL;

         clusters[i].id = i + 1;
      }

      raw_detection_list.vcslong_det_idx_max = -1;
      raw_detection_list.vcslong_det_idx_min = -1;
      for (int16_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         raw_detection_list.detections[i].raw.det_id = i + 1;
         raw_detection_list.detections[i].raw.sensor_id = 1U;
         raw_detection_list.detections[i].processed.next_sorted_idx = -1;
         raw_detection_list.detections[i].processed.prev_sorted_idx = -1;
      }
   }

   void Add_Simple_Det_Data(
      rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      rspp_variant_A::RSPP_Detection_T &det,
      int32_t local_det_idx,
      float32_t det_vcs_pos_long,
      float32_t det_vcs_pos_lat,
      float32_t det_range_rate,
      bool det_f_dealiased,
      rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      if ((0 < local_det_idx) && (local_det_idx < MAX_NUMBER_OF_DETECTIONS))
      {
         raw_detection_list.detections[local_det_idx].processed.prev_sorted_idx = local_det_idx - 1;
         raw_detection_list.detections[local_det_idx - 1].processed.next_sorted_idx = local_det_idx;
      }

      detection_props[local_det_idx].vcs_position.y = det_vcs_pos_lat;
      detection_props[local_det_idx].vcs_position.x = det_vcs_pos_long;
      detection_props[local_det_idx].motion_status = det_motion_status;
      raw_detection_list.detections[local_det_idx].processed.vcs_position_y = det_vcs_pos_lat;
      raw_detection_list.detections[local_det_idx].processed.vcs_position_x = det_vcs_pos_long;
      detection_props[local_det_idx].range_rate_dealiased = det_range_rate;
      detection_props[local_det_idx].f_dealiased = det_f_dealiased;

      raw_detection_list.number_of_valid_detections += 1;
      det.raw.range_rate = det_range_rate;
      det.raw.sensor_id = 1U;
   }

   void Add_Det_Data_For_Validity(
      F360_Detection_Props_T &det_p,
      rspp_variant_A::RSPP_Detection_T &det,
      bool f_double_bounce,
      bool f_close_target,
      bool f_det_pair,
      bool f_FOV_edge,
      bool f_ok_to_use,
      uint8_t on_sep_id,
      bool f_host_veh_clutter,
      int32_t object_track_id,
      F360_Detection_Wheelspin_Type_T wheel_spin_type
   )
   {
      det_p.f_double_bounce = f_double_bounce;
      det_p.f_close_target = f_close_target;
      det_p.f_det_pair = f_det_pair;
      det_p.f_FOV_edge = f_FOV_edge;
      det_p.f_ok_to_use = f_ok_to_use;
      det_p.wheel_spin_type = wheel_spin_type;
      det_p.object_track_id = object_track_id;
      det_p.on_sep_id = on_sep_id;
      det.raw.f_host_veh_clutter = f_host_veh_clutter;
   }

   void Simple_Add_Det_for_Initialize_Clusters(
      bool f_take_new_cluster_to_init,
      float32_t det_vcs_pos_long,
      float32_t det_vcs_pos_lat,
      float32_t det_range_rate,
      bool det_f_dealiased,
      rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status,
      rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      F360_Local_Clusters_T &local_clusters_data,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   )
   {
      if (f_take_new_cluster_to_init == true)
      {
         local_clusters_data.array_of_first_det_idx_in_clusters[local_clusters_data.num_clusters] = local_clusters_data.num_of_associated_dets;
         local_clusters_data.num_clusters += 1;
      }

      int16_t local_cluster_idx = local_clusters_data.num_clusters - 1;
      local_clusters_data.num_dets_in_clusters[local_cluster_idx] += 1;

      local_clusters_data.num_of_associated_dets += 1;
      int16_t local_det_idx = local_clusters_data.num_of_associated_dets - 1;
      local_clusters_data.array_of_det_idxs_in_clusters[local_det_idx] = local_det_idx;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
         det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, detection_props);
   }
}
