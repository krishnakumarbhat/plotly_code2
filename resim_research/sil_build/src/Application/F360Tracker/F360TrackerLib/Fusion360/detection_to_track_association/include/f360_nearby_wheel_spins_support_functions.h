/*===========================================================================*\
* FILE: f360_nearby_wheel_spins_support_functions.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declarations of supporting functions for 
*   f360_nearby_wheel_spins.cpp
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef F360_NEAR_BY_WHEEL_SPINS_SUPPORT_FUNCTIONS_H
#define F360_NEAR_BY_WHEEL_SPINS_SUPPORT_FUNCTIONS_H


#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "f360_bounding_box.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   struct Wheel_Spin_Cluster_T
   {
      int32_t min_long_det_idx;
      int32_t max_long_det_idx;
      float32_t min_lat_pos;
      float32_t max_lat_pos;
      F360_Detection_Wheelspin_Type_T ws_type;
      bool f_valid;
      uint8_t padding[2];

      Wheel_Spin_Cluster_T ()
      {
         min_long_det_idx = F360_INVALID_ID;
         max_long_det_idx = F360_INVALID_ID;
         min_lat_pos = -INFTY;
         max_lat_pos = INFTY;
         ws_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
         f_valid = false;
         padding[0] = 0U;
         padding[1] = 0U;
      };
   };

   struct Wheel_Spin_Internals_T
   {
      int32_t next_starting_det_idx;
      int32_t prev_ws_det_idx;
      bool f_first_call;
      uint8_t padding[3];

      Wheel_Spin_Internals_T ()
      {
         next_starting_det_idx = F360_INVALID_ID;
         prev_ws_det_idx = F360_INVALID_ID;
         f_first_call = true;
         padding[0] = 0U;
         padding[1] = 0U;
         padding[2] = 0U;
      };
   };

   Wheel_Spin_Cluster_T Find_Next_Wheel_Spin_Cluster(
      const F360_Calibrations_T &calibrations,
      const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
      const BoundingBox &area_of_interest,
      F360_Detection_Props_T(&det_properties)[MAX_NUMBER_OF_DETECTIONS],
      Wheel_Spin_Internals_T &ws_internal
   );

   bool Is_Det_Vaild_For_WS_Clustering(
      const F360_Detection_Props_T &det_prop,
      const BoundingBox &area_of_interest
   );
}

#endif
