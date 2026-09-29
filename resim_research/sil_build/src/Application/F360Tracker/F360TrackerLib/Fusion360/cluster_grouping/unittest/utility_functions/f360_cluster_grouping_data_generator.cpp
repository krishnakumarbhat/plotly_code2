/*===========================================================================*\
* FILE: f360_cluster_grouping_data_generator.cpp
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
*   This file contains support functions for detections data generation for unit testing
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

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_cluster_grouping_data_generator.h"

#include "f360_data_generator.h"
#include "f360_cluster_grouping.h"
#include "f360_constants.h"
#include "f360_math.h"

namespace f360_variant_A
{
   void Add_Simple_Cluster_To_Tracker_Status(
      int32_t local_det_idx,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Tracker_Info_T &tracker_info
      )
   {
      if (local_det_idx == 0)
      {
         tracker_info.vcslong_sorted_cluster_start = &clusters[local_det_idx];
      }
      else
      {
         tracker_info.vcslong_sorted_cluster_next[local_det_idx - 1] = &clusters[local_det_idx];
         tracker_info.vcslong_sorted_cluster_prev[local_det_idx] = &clusters[local_det_idx - 1];
      }
      tracker_info.num_active_clusters += 1;
      tracker_info.active_cluster_ids[local_det_idx] = local_det_idx + 1;

      // handling inactive_cluster_ids and removing cluster id that had been moved to active ones
      uint32_t num_inactive = NUMBER_OF_CLUSTERS - tracker_info.num_active_clusters;
      for (uint32_t i = 0U; i < num_inactive - 1U; i++)
      {
         tracker_info.inactive_cluster_ids[i] = tracker_info.inactive_cluster_ids[i + 1U];
      }
      tracker_info.inactive_cluster_ids[num_inactive - 1U] = 0U;
   }

   void Add_Det(
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Cluster_T& cluster,
      int32_t sensor_id,
      float32_t range_rate_compensated,
      float32_t vcs_position_long,
      float32_t vcs_position_lat)
   {
      for (uint16_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         if (raw_detection_list.detections[i].raw.det_id == 0)
         {
            raw_detection_list.detections[i].raw.det_id = i + 1;
            raw_detection_list.detections[i].raw.sensor_id = sensor_id;
            det_props[i].range_rate_compensated = range_rate_compensated;
            det_props[i].range_rate_dealiased = range_rate_compensated;
            det_props[i].vcs_position.x = vcs_position_long;
            det_props[i].vcs_position.y = vcs_position_lat;

            //adding det to cluster
            cluster.ndets++;
            for (uint16_t j = 0; j < MAX_DETS_IN_OBJ_TRK; j++)
            {
               if (cluster.detids[j] == 0)
               {
                  cluster.detids[j] = raw_detection_list.detections[i].raw.det_id;
                  break;
               }
            }
            break;
         }
      }
   }

   void Add_Hist_Det(
      F360_Detection_Hist_T& detections_hist,
      F360_Cluster_T& cluster,
      const float32_t rngrate_interval_width,
      const float32_t range_rate_compensated,
      const float32_t time_since_measurement)
   {
      detections_hist.f_idx_occupied[detections_hist.n_occupied] = true;
      detections_hist.det_data[detections_hist.n_occupied].v_wrapping = rngrate_interval_width;
      detections_hist.det_data[detections_hist.n_occupied].rdot_comp = range_rate_compensated;
      detections_hist.det_data[detections_hist.n_occupied].rdot = range_rate_compensated;
      detections_hist.det_data[detections_hist.n_occupied].time_since_meas = time_since_measurement;

      detections_hist.n_occupied++;
      detections_hist.max_occupation++;

      //adding old det to cluster
      cluster.num_old_dets++;
      for (uint16_t i = cluster.num_old_dets - 1; i < MAX_DETS_IN_OBJ_TRK; i++)
      {
         if (cluster.old_det_idx[i] == 0)
         {
            cluster.old_det_idx[i] = detections_hist.n_occupied - 1;
            break;
         }
      }
   }

   void Fill_Cluster(
      const float32_t posn_lat,
      const float32_t posn_long,
      const float32_t rep_rdotcomp,
      const float32_t time_since_cluster_updated,
      F360_Cluster_T& cluster
   )
   {
      cluster.vcs_position_y = posn_lat;
      cluster.vcs_position_x = posn_long;
      cluster.rep_rdotcomp = rep_rdotcomp;
      cluster.time_since_cluster_updated = time_since_cluster_updated;
      cluster.rep_vcs_az = Get_Azimuth_From_Cart_Pos(cluster.vcs_position_y, cluster.vcs_position_x);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
   }
}
