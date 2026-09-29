#ifndef F360_CLUSTER_H
#define F360_CLUSTER_H
/*===================================================================================*\
* FILE: cluster.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*==========================================================================================*/

#include "f360_reuse.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   typedef struct F360_Cluster_Tag
   {
      float32_t vcs_position_x;
      float32_t vcs_position_y;
      float32_t vcs_position_z;
      float32_t rep_vcs_az;
      float32_t cos_vcs_az;
      float32_t sin_vcs_az;
      float32_t rep_rdotcomp;
      float32_t time_since_cluster_updated; // currently contains time relative to tracker execution time instead of measurement time 
      float32_t time_since_measurement; // measurement time relative to current time
      int16_t id;
      int16_t ndets;
      int16_t detids[MAX_DETS_IN_OBJ_TRK];
      int16_t old_det_idx[MAX_HIST_DETS_IN_CLUSTER];
      int16_t num_types_of_dets[2];
      int16_t num_old_dets;
      int16_t clutter_counter;
      bool f_dealiased;
      bool f_to_be_killed; // Flag indicating that cluster should be killed by it's not yet done.
      uint8_t low_rcs_dets_cnt;
      uint8_t stationary_cluster_cnt;
   } F360_Cluster_T;
}
#endif
