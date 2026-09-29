#ifndef F360_CLUSTERING_H
#define F360_CLUSTERING_H
/*===================================================================================*\
* FILE: f360_clustering.h
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
\*===================================================================================*/

#include "f360_calibrations.h"
#include "f360_radar_sensor.h"
#include "f360_host.h"
#include "f360_tracker_info.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_cluster.h"
#include "f360_timing_info.h"
#include "f360_detection_hist.h"

namespace f360_variant_A
{
   void Clustering(
      const F360_Calibrations_T &calibrations,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      F360_Tracker_Info_T &tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T& det_hist,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T &timing_info);
}

#endif
