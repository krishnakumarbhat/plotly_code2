/*===================================================================================*\
* FILE:  f360_update_cluster_state.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the definition of the function Update_Cluster_State() and 
* subfunctions which can be used to update the states of clusters over time when
* new detections are added or old detections are removed.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_update_cluster_state.h"
#include "f360_norm_heading_angle.h"
#include "f360_try_to_dealiase_rdots_in_two_clusters.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Cluster_State()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Detection_Hist_T & det_hist
   * F360_Cluster_T & cluster
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Update representative properties and check if the track can be de-aliased 
   * using associated detections. These detections may come from different 
   * sensors, which enables us to do this.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Cluster_State(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Hist_T & det_hist,
      F360_Cluster_T & cluster)
   {      
      cluster.f_dealiased = true;

      float32_t z_pos_sum = 0.0F;
      // Calculating mean of x and y position of object detection properties */
      if (cluster.ndets > 0)
      {
         float32_t vcs_az_sum = 0.0F;
         float32_t long_pos_sum = 0.0F;
         float32_t lat_pos_sum = 0.0F;
         float32_t rdot_sum = 0.0F;
         for (int16_t i = 0; i < cluster.ndets; i++)
         {
            const int16_t det_idx = cluster.detids[i] - 1;

            long_pos_sum += det_props[det_idx].vcs_position.x;
            lat_pos_sum += det_props[det_idx].vcs_position.y;
            z_pos_sum += raw_detection_list.detections[det_idx].processed.vcs_position_z;
            vcs_az_sum += Normalize_Heading_Angle(raw_detection_list.detections[det_idx].processed.vcs_az, cluster.rep_vcs_az);
            rdot_sum += det_props[det_idx].range_rate_compensated;

            if (!det_props[det_idx].f_dealiased)
            {
               cluster.f_dealiased = false;
            }
         }
         const float32_t ndet_inv = 1.0F / static_cast<float32_t>(cluster.ndets);
         cluster.rep_vcs_az = Normalize_Heading_Angle(vcs_az_sum * ndet_inv, 0.0F);
         cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
         cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
         cluster.vcs_position_x = (long_pos_sum * ndet_inv);
         cluster.vcs_position_y = (lat_pos_sum * ndet_inv);
         cluster.rep_rdotcomp = (rdot_sum * ndet_inv);
      }

      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
     
         if (!det_hist.det_data[det_idx].f_dealiased)
         {
            cluster.f_dealiased = false;
         }
         z_pos_sum += det_hist.det_data[det_idx].vcs_position_z;
      }
      const int16_t sum_dets = cluster.ndets + cluster.num_old_dets;
      if (sum_dets > 0)
      {
          cluster.vcs_position_z = z_pos_sum / static_cast<float32_t>(sum_dets);
      }
   }
}
