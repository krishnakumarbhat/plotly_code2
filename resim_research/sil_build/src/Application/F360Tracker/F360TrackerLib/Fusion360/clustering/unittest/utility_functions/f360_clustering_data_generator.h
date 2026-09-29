/*===========================================================================*\
* FILE: f360_clustering_data_generator.h
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
#ifndef F360_CLUSTERING_DATA_GENERATORS_SUPPORT_FUNCTIONS_H
#define F360_CLUSTERING_DATA_GENERATORS_SUPPORT_FUNCTIONS_H

#include "f360_initialize_clusters.h"

namespace f360_variant_A
{
   void Setup_TrackerInfo_Cluster_Detp(
      F360_Tracker_Info_T &tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      rspp_variant_A::RSPP_Detection_List_T &raw_detection_list
   );

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
   );

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
   );

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
   );
}
#endif
