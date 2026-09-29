#ifndef F360_DBSCAN_H
#define F360_DBSCAN_H
/*===================================================================================*\
* FILE: f360_dbscan.h
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
\*===================================================================================*/

#include "f360_reuse.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_local_clusters.h"
#include "f360_tracker_info.h"
#include "f360_radar_sensor.h"

namespace f360_variant_A
{
   void DBscan(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      const int16_t(&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS],
      const int16_t num_sorted_dets,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Local_Clusters_T& output_data);

   void Cluster_Expand(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      const uint8_t max_num_dets,
      const int16_t detection_index,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      bool(&f_detection_clustered)[MAX_NUMBER_OF_DETECTIONS],
      F360_Local_Clusters_T& output_data);

   void Cluster_Region_Query(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      const uint8_t max_num_dets,
      const int16_t detection_index,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      bool(&f_detection_clustered)[MAX_NUMBER_OF_DETECTIONS],
      int16_t(&cluster_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint8_t& n_cluster_dets);
}
#endif
