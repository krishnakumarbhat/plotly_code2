#ifndef F360_INITIALIZATION_DEBUG_DATA_H
#define F360_INITIALIZATION_DEBUG_DATA_H
/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE:  f360_initialization_debug_data.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of a data structure used for logging initialization debug data.
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/
#include "f360_reuse.h"
#include "f360_detection_hist.h"
#include "f360_detection_props.h"
#include "f360_occlusion_types.h"
#include "f360_conf.h"
#include "f360_track_init.h"

namespace f360_variant_A
{
   typedef struct F360_Initialization_Debug_Data_Tag
   {
      int32_t n_obj;
      int32_t trackID[20];
      float longvel_by_cloud[20];
      float latvel_by_cloud[20];
      float longvel_by_posdiff[20];
      float latvel_by_posdiff[20];
      int32_t n_det[20];
      int32_t n_old_det[20];
      int32_t det_id[20][MAX_DETS_IN_OBJ_TRK];
      int32_t old_det_id[20][MAX_HIST_DETS_IN_CLUSTER];
   }F360_Initialization_Debug_Data_T;

   typedef struct F360_Initialization_Cluster_Debug_Data_Tag
   {
      bool f_prioritized[NUMBER_OF_CLUSTERS];
      uint8_t occlusion_status[NUMBER_OF_CLUSTERS];
      uint8_t cloud_confidence[NUMBER_OF_CLUSTERS];
      uint8_t posdiff_confidence[NUMBER_OF_CLUSTERS];
      bool f_ambiguous_motion_in_clutter[NUMBER_OF_CLUSTERS];
      int8_t init_type[NUMBER_OF_CLUSTERS];
      float32_t longvel_by_cloud[NUMBER_OF_CLUSTERS];
      float32_t latvel_by_cloud[NUMBER_OF_CLUSTERS];
      float32_t longvel_by_posdiff[NUMBER_OF_CLUSTERS];
      float32_t latvel_by_posdiff[NUMBER_OF_CLUSTERS];
      int16_t num_dets[NUMBER_OF_CLUSTERS];
      float32_t det_vcs_posx[NUMBER_OF_CLUSTERS][MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t det_vcs_posy[NUMBER_OF_CLUSTERS][MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t det_vcs_az[NUMBER_OF_CLUSTERS][MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t det_rr_comp[NUMBER_OF_CLUSTERS][MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
      float32_t det_time_since_meas[NUMBER_OF_CLUSTERS][MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER];
   }F360_Initialization_Cluster_Debug_Data_T;
}

#endif
