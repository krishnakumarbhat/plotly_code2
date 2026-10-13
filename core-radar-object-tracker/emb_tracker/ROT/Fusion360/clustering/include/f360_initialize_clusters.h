/*===================================================================================*\
* FILE: f360_initialize_clusters.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration for Initialize_Clusters()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/

#ifndef INITIALIZE_CLUSTERS_H
#define INITIALIZE_CLUSTERS_H

#include "f360_calibrations.h"
#include "f360_cluster.h"
#include "f360_detection_hist.h"
#include "f360_detection_list.h"
#include "f360_detection_props.h"
#include "f360_host.h"
#include "f360_local_clusters.h"
#include "f360_radar_sensor_props.h"
#include "f360_reuse.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
void Initialize_Clusters(const F360_Detection_List_T &raw_detection_list, const F360_Calibrations_T &calibrations,
                         const F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS], const F360_Host_T &host,
                         F360_Local_Clusters_T &local_clusters_data, F360_Tracker_Info_T &tracker_info,
                         F360_Detection_Props_T (&detection_props)[MAX_NUMBER_OF_DETECTIONS], F360_Detection_Hist_T &det_hist,
                         F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS]);

void Init_Cluster(const uint16_t first_det_idx_in_cluster,
                  const int16_t (&array_of_det_idxs_in_clusters)[MAX_NUMBER_OF_DETECTIONS],
                  const F360_Detection_List_T &raw_detection_list, const F360_Calibrations_T &calibrations,
                  const F360_Radar_Sensor_Props_T (&sensors_props)[MAX_NUMBER_OF_SENSORS], const uint8_t max_dets_in_obj_track,
                  const uint8_t number_of_dets_in_cluster, F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS],
                  F360_Cluster_T &cluster);

void Find_Least_Prioritized_Clusters(const F360_Tracker_Info_T &tracker_info, const F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
                                     const uint16_t max_num_clusters_to_sort, const float32_t highest_local_cluster_prio,
                                     float32_t (&priority)[NUMBER_OF_CLUSTERS],
                                     int16_t (&low_to_high_prio_perm)[NUMBER_OF_CLUSTERS], uint16_t &num_low_prio_clusters);

void Compute_Local_Cluster_Priorities(const F360_Local_Clusters_T &local_clusters, const F360_Host_T &host,
                                      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
                                      const F360_Tracker_Info_T &tracker_info,
                                      float32_t (&local_clusters_prio)[MAX_TRACKER_POSN_CLUSTERS]);
} // namespace f360_variant_A
#endif
