/*===================================================================================*\
* FILE: f360_kill_cluster.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*  This file contains function definition of kill_cluster()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

/******************************
* Includes
*******************************/
#include "f360_kill_cluster.h"
#include "f360_iterator.h"
#include "f360_clear_cluster.h"
#include "f360_sorted_clusters_mgmt.h"
#include "f360_reuse.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: kill_cluster()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * F360_Cluster_T & cluster - The cluster to be killed
   * F360_Detection_Hist_T & det_hist - Historical detection structure
   * F360_Tracker_Info_T & tracker_info - Tracker info
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function kills a particular cluster and removes historical detections associated to the cluster. 
   *
   \*===========================================================================*/
   void kill_cluster(
      F360_Cluster_T & cluster,
      F360_Detection_Hist_T & det_hist,
      F360_Tracker_Info_T & tracker_info)
   {

      const int16_t* const begin = cmn::begin(tracker_info.active_cluster_ids);
      const int16_t* const end = cmn::end(tracker_info.active_cluster_ids);
      const int16_t* const p_active_cluster_id_found = std::find(begin, end, cluster.id);

      const bool f_found_cluster_among_active_clusters = p_active_cluster_id_found != end;

      const int16_t num_active_clusters = tracker_info.num_active_clusters;
      const bool f_valid_cluster_found = ((num_active_clusters > 0) && (0 < cluster.id) && (tracker_info.variant.num_clusters >= static_cast<uint32_t>(cluster.id)) && f_found_cluster_among_active_clusters);
      if (f_valid_cluster_found)
      {
         const uint32_t cluster_idx = static_cast<uint32_t>(std::distance(begin, p_active_cluster_id_found));
         for (uint32_t idx = cluster_idx; (idx < (static_cast<uint32_t>(tracker_info.variant.num_clusters) - 1U)); idx++)
         {
            tracker_info.active_cluster_ids[idx] = tracker_info.active_cluster_ids[idx + 1U];
         }

         tracker_info.active_cluster_ids[tracker_info.variant.num_clusters - 1U] = 0;
         tracker_info.inactive_cluster_ids[tracker_info.variant.num_clusters - static_cast<uint16_t>(num_active_clusters)] = cluster.id;
         tracker_info.num_active_clusters--;

         Sorted_Clusters_Remove(tracker_info, cluster);

         for (int16_t idx = 0; idx < cluster.num_old_dets; idx++)
         {
            const int16_t det_idx = cluster.old_det_idx[idx];

            det_hist.det_data[det_idx] = {}; // Clear det data 
            det_hist.f_idx_occupied[det_idx] = false;
            det_hist.n_occupied--;
         }
         Clear_Cluster(cluster);
      }
   }
}
