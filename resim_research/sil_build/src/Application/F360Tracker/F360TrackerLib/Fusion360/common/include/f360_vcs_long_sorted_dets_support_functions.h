/*===========================================================================*\
* FILE: f360_vcs_long_sorted_dets_support_functions.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of functions related to sorting
*   of detections in VCS longitudinal order.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_VCS_LONG_SORTED_DETS_SUPPORT_FUNCTIONS_H
#define F360_VCS_LONG_SORTED_DETS_SUPPORT_FUNCTIONS_H

#include "f360_detection_props.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{

   void Sort_Detections_Vcs_Long(
      rspp_variant_A::RSPP_Detection_List_T &raw_detections
   );

   void Update_Dets_Vcs_Long_Ref_Sorted_Info(
      const float32_t det_vcs_long,
      const uint32_t det_idx,
      uint32_t &vcs_long_ref_start_idx,
      rspp_variant_A::RSPP_Detection_List_T &raw_detections
   );

   void Clear_Dets_Vcs_Long_Sorted_Info(
      rspp_variant_A::RSPP_Detection_List_T &raw_detections
   );

   int32_t Get_First_Relevant_Long_Sorted_Det_Idx(
      const float32_t vcs_long_value,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections
   );

   void Get_Det_Indexes_In_Vcs_Zone(
      const float32_t vcs_x_min,
      const float32_t vcs_x_max,
      const float32_t vcs_y_min,
      const float32_t vcs_y_max,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS],
      int32_t (&relevant_det_idx)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t &num_relevant_dets
   );

   void Get_Det_Indexes_In_Vcs_Circular_Zone(
      const Point& center,
      const float32_t radius,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t (&relevant_det_idx)[MAX_NUMBER_OF_DETECTIONS],
      uint32_t &num_relevant_dets);

   bool Is_More_Relevant_Dets_In_Vcs_Zone(
      const int32_t det_idx,
      const float32_t vcs_x_max,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   );

}

#endif
