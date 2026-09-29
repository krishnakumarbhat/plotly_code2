/*===================================================================================*\
* FILE: f360_try_to_dealiase_rdots_in_two_clusters_support_functions.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This file contains function declarations of 
*     Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased()
*     Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased()
*     Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased()
*     Find_All_Rdot_Widths()
*     Calc_Rep_Rdot_Comp()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_TRY_TO_DEALIAS_CLUSTERS_SUPPORT_FUNCTIONS_H
#define F360_TRY_TO_DEALIAS_CLUSTERS_SUPPORT_FUNCTIONS_H

#include "f360_reuse.h"
#include "f360_cluster.h"
#include "f360_tracker_info.h"
#include "f360_globals.h"
#include "f360_radar_sensor.h"
#include "f360_detection_props.h"
#include "rspp_detection_list.h"
#include "f360_detection_hist.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Try_To_Dealiase_Rdots_When_None_Clusters_Are_Dealiased(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T &detections_hist,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Globals_T& globals,
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &cluster_older,
      const F360_Cluster_T &cluster_newer,
      float32_t& rngrate_interval_width_older,
      float32_t& rngrate_interval_width_newer,
      float32_t& interval_older,
      float32_t& interval_newer,
      bool& f_success);

   void Try_To_Dealiase_Rdots_When_One_Cluster_Is_Dealiased(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T &detections_hist,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      const F360_Globals_T& globals,
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &dealiased_cluster,
      const F360_Cluster_T &cluster2,
      float32_t& rngrate_interval_width,
      float32_t& rngrate_interval,
      bool& f_success);

   void Try_To_Dealiase_Rdots_When_Both_Clusters_Are_Dealiased(
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &cluster_older,
      const F360_Cluster_T &cluster_newer,
      bool& f_success);

   void Find_All_Rdot_Widths(
      const F360_Cluster_T &cluster,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T &detections_hist,
      int32_t &ndets_older,
      float32_t(&rdot_width)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]);

   float32_t Calc_Rep_Rdot_Comp(
      const F360_Cluster_T &cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T &detections_hist,
      const int32_t(&unique_rdot_interval_ids)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
      const int32_t &unique_rdot_interval_index,
      const int32_t(&rdot_interval_ids)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]);
}
#endif
