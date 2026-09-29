/*===================================================================================*\
* FILE: f360_find_and_prioritize_detections.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
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
*   This file contains function signature of find and prioritize detections and its helper functions
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
#ifndef F360_FIND_AND_PRIORITIZE_DETECTIONS
#define F360_FIND_AND_PRIORITIZE_DETECTIONS

#include "rspp_detection_list.h"
#include "f360_radar_sensor.h"
#include "f360_constants.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "f360_detection_props.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
   void Find_And_Prioritize_Detections(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      int16_t& valid_det_count,
      int16_t(&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS],
      bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS]);
   
   bool Detection_Clustering_Validity_Check(
      const F360_Radar_Sensor_T& sensor,
      const F360_Detection_Props_T& det_p,
      const rspp_variant_A::RSPP_Detection_T& det,
      const bool f_cluster_moving,
      const float32_t host_vcs_speed);

   bool Is_Detection_In_Third_Priority_Zone(
      const F360_Detection_Props_T& det_p,
      const F360_Host_T& host);

   struct Traverse_Starting_Det_Indexes
   {
      int16_t forward_det_idx;
      int16_t backward_det_idx;
   };
   
   Traverse_Starting_Det_Indexes Find_Forward_And_Backward_Starting_Det_Indexes(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]);
}
#endif
