/*===========================================================================*\
* FILE: f360_cluster_grouping_data_generator.h
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
*   This file contains detections data generation for unit testing
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
#ifndef F360_CLUSTER_GROUPING_DATA_GENERATORS_SUPPORT_FUNCTIONS_H
#define F360_CLUSTER_GROUPING_DATA_GENERATORS_SUPPORT_FUNCTIONS_H

#include "f360_cluster_grouping.h"

namespace f360_variant_A
{
   void Add_Simple_Cluster_To_Tracker_Status(
      int32_t local_det_idx,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Tracker_Info_T &tracker_info
   );

   void Add_Det(
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Cluster_T& cluster,
      int32_t sensor_id,
      float32_t range_rate_compensated,
      float32_t vcs_position_long = 0.0F,
      float32_t vcs_position_lat = 0.0F);

   void Add_Hist_Det(
      F360_Detection_Hist_T& detections_hist,
      F360_Cluster_T& cluster,
      const float32_t rngrate_interval_width,
      const float32_t range_rate_compensated,
      const float32_t time_since_measurement);

   void Fill_Cluster(
      const float32_t posn_lat,
      const float32_t posn_long,
      const float32_t rep_rdotcomp,
      const float32_t time_since_cluster_updated,
      F360_Cluster_T& cluster
   );
}
#endif
