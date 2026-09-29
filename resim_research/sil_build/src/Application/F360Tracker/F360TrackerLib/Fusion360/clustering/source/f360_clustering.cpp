/*===================================================================================*\
* FILE: f360_clustering.cpp
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
\*===================================================================================*/

#include "f360_clustering.h"
#include "f360_local_clusters.h"
#include "f360_find_and_prioritize_detections.h"
#include "f360_dbscan.h"
#include "f360_initialize_clusters.h"
#include "f360_get_wall_time.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Clustering()
   *===========================================================================
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Group non-associated detections which are close to each other into clusters.
   * Process detections in order of priority when starting clusters from a 
   * single detection. Process detections classified as moving separately.
   \*===========================================================================*/

   void Clustering(
      const F360_Calibrations_T &calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      F360_Tracker_Info_T &tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T& det_hist,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      F360_Local_Clusters_T local_cluster_data;
      int16_t valid_det_sorted_idxs[MAX_NUMBER_OF_DETECTIONS];
      int16_t valid_det_count = 0;
      bool valid_dets[MAX_NUMBER_OF_DETECTIONS];

      // Cluster moving detections
      bool f_cluster_moving = true;
      if (raw_detection_list.number_of_valid_detections > 0U)
      {
         Find_And_Prioritize_Detections(raw_detection_list, sensors, host, detection_props, f_cluster_moving, valid_det_count, valid_det_sorted_idxs, valid_dets);
      }

      if (0 < valid_det_count)
      {
         DBscan(tracker_info, raw_detection_list, sensors, valid_dets, f_cluster_moving, valid_det_sorted_idxs, valid_det_count, detection_props, local_cluster_data);

         Initialize_Clusters(raw_detection_list, calibrations, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);
      }

      // Cluster ambiguous detections
      f_cluster_moving = false;
      if (raw_detection_list.number_of_valid_detections > 0U)
      {
         Find_And_Prioritize_Detections(raw_detection_list, sensors, host, detection_props, f_cluster_moving, valid_det_count, valid_det_sorted_idxs, valid_dets);
      }

      if (0 < valid_det_count)
      {
         DBscan(tracker_info, raw_detection_list, sensors, valid_dets, f_cluster_moving, valid_det_sorted_idxs, valid_det_count, detection_props, local_cluster_data);

         Initialize_Clusters(raw_detection_list, calibrations, sensors, host, local_cluster_data, tracker_info, detection_props, det_hist, clusters);
      }

      timing_info.clustering = get_wall_time() - start_time;
   }
}
