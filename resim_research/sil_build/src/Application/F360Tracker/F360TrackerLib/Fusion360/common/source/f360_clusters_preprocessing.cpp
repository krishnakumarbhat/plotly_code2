/*===========================================================================*\
* FILE: f360_clusters_preprocessing.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function definition of Clusters_Preprocessing,
* Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion and helper functions.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#include "f360_math.h"
#include "f360_kill_obj_trk.h"
#include "f360_math_func.h"
#include "f360_clear_cluster.h"
#include "f360_detection_hist.h"
#include "f360_kill_cluster.h"
#include "f360_terminate_clusters.h"
#include "f360_host_props.h"
#include "f360_norm_heading_angle.h"
#include "f360_iterator.h"
#include "f360_clusters_preprocessing.h"
#include <algorithm>
#include <cstring>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Host_Props_T &host_props - a reference to host properties data structure
   * F360_Cluster_T &cluster - a reference to a single cluster
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
   * The host vehicle could possibly move (translate and rotate) between two tracker 
   * iterations. Since the VCS is fixed in host, the VCS system in one tracker 
   * iteration could therefore differ from the VCS in another tracker iteration. This
   * function corrects the estimated cluster VCS position and VCS azimuth according
   * to the host delta motion between two tracker iterations.
   *
   * PRECONDITIONS:
   * None
   * 
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(
      const F360_Host_Props_T &host_props,
      F360_Cluster_T &cluster)
   {
         // Update cluster vcs position
         const float32_t vcs_long_pos_accounted_for_host_translation = cluster.vcs_position_x - host_props.delta_position_x;
         const float32_t vcs_lat_pos_accounted_for_host_translation = cluster.vcs_position_y - host_props.delta_position_y;
         F360_Rotate_2D_Vector(vcs_long_pos_accounted_for_host_translation, vcs_lat_pos_accounted_for_host_translation,
            host_props.cos_delta_pointing, -host_props.sin_delta_pointing,
            cluster.vcs_position_x, cluster.vcs_position_y);

         // Update cluster vcs heading
         cluster.rep_vcs_az = Normalize_Heading_Angle(cluster.rep_vcs_az - host_props.delta_pointing, 0.0F);
   }

   /*===========================================================================*\
   * FUNCTION: Clusters_Preprocessing()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Calibrations_T &calib - Calibration structure
   * const F360_Host_Props_T &host_props - Host properties
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS] - Clusters
   * F360_Detection_Hist_T &det_hist - Historical detection structure
   * F360_Tracker_Info_T &tracker_info - Tracker info 
   * 
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function kills clusters with flag f_to_be_killed set to true, removes 
   * old detections from clusters and kills these with no associated ones. This 
   * function also corrects the VCS properties of clusters according to the host
   * delta motion between two tracker iterations.
   *
   \*===========================================================================*/
   void Clusters_Preprocessing(
      const F360_Host_Props_T &host_props,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T& det_hist,
      F360_Tracker_Info_T &tracker_info)
   {
      // Terminate clusters with f_to_be_killed flag set to true
      Terminate_Clusters(clusters, det_hist, tracker_info);

      // Adjusts the cluster VCS properties based on on how much host has moved since previous filter iteration
      for (uint32_t i = 0U; i < static_cast<uint32_t>(tracker_info.num_active_clusters); i++)
      {
         const uint32_t cluster_idx = static_cast<uint32_t>(tracker_info.active_cluster_ids[i]) - 1U;
         Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(host_props, clusters[cluster_idx]);
      }
   }
}
