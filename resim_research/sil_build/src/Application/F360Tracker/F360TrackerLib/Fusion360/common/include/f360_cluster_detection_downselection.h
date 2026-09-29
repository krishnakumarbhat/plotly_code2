/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_cluster_detection_downselection.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declarations of:
* Downselect_Detections()
* Determine_Positions_In_Set_Of_Dets_To_Clear()
* Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each()
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#ifndef F360_CLUSTER_DETECTION_DOWNSELECTION_H
#define F360_CLUSTER_DETECTION_DOWNSELECTION_H

#include "f360_math_func.h"
#include "f360_detection_props.h"
#include "f360_detection_hist.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   static constexpr uint16_t WORST_CASE_NUM_DETS_IN_CLUSTER =
      (2U * MAX_DETS_IN_OBJ_TRK >= 2U * MAX_HIST_DETS_IN_CLUSTER)
         ? (2U * MAX_DETS_IN_OBJ_TRK)
         : (2U * MAX_HIST_DETS_IN_CLUSTER);

   struct Detections_Set
   {
      uint16_t num_dets = 0U;
      int16_t det_indexes[WORST_CASE_NUM_DETS_IN_CLUSTER]{};
      float32_t time_since_meas[WORST_CASE_NUM_DETS_IN_CLUSTER]{};
      bool f_historic[WORST_CASE_NUM_DETS_IN_CLUSTER]{};
      bool f_downselected[WORST_CASE_NUM_DETS_IN_CLUSTER]{};
   };

   struct Cluster_Dets
   {
      Detections_Set new_dets{};
      Detections_Set hist_dets{};
   };

   struct Unique_Time_Detection_Data
   {
      uint16_t num_unique_tsm{}; // Total number of unique timestamps
      uint16_t num_dets_per_unique_tsm[WORST_CASE_NUM_DETS_IN_CLUSTER]{}; // Array for storing how many detections there are in total for each unique timestamp
      uint16_t num_downselected_dets_per_unique_tsm[WORST_CASE_NUM_DETS_IN_CLUSTER]{}; // Array for storing how many detections we want to downselect from each unique time.
   };

   void Downselect_Detections(
      const uint16_t& max_dets_to_downselect,
      Detections_Set& set_of_detections);

   void Count_Unique_Tsm_And_Update_Num_Of_Dets_For_Each(
      const float32_t(&timestamp_array)[WORST_CASE_NUM_DETS_IN_CLUSTER],
      const uint16_t num_dets,
      Unique_Time_Detection_Data& unique_time_det_data);
}
#endif
