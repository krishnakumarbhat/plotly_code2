/*===================================================================================*\
* FILE:  f360_coarse_cluster_gate.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the definition of the function Coarse_Cluster_Gate() which can be used
* as a crude check if two clusters are positioned close by to each other
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/
#include "f360_coarse_cluster_gate.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Coarse_Cluster_Gate()
   *===========================================================================
   * RETURN VALUE:
   * bool f_success
   *
   * PARAMETERS:
   *  const F360_Calibrations_T &calib
   *  const F360_Cluster_T &cluster1
   *  const F360_Cluster_T &cluster2
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
   * It checks if coarse cluster merging conditions are met. It is done by 
   * checking euclidean distance between clusters and thier timestamp difference 
   * if needed.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Coarse_Cluster_Gate(
      const F360_Calibrations_T &calib,
      const F360_Cluster_T &cluster1,
      const F360_Cluster_T &cluster2)
   {
     // Coarse Position Gate
     const float32_t delta_long_pos = cluster1.vcs_position_x - cluster2.vcs_position_x;
     const float32_t delta_lat_pos = cluster1.vcs_position_y - cluster2.vcs_position_y;
     const float32_t delta_dist_sq = delta_long_pos * delta_long_pos + delta_lat_pos * delta_lat_pos;

     bool f_success;

     // order of conditions and scheme were developed based on runtime factor
     if (delta_dist_sq < calib.k_stat_clusters_dist_sq_coarse_gate) // checking first dist gate for stationary and moving clusters
     {
        f_success = true;
     }
     //TODO: DFT-750
     else if ((cluster1.num_types_of_dets[0] > 0) && (cluster2.num_types_of_dets[0] > 0)) // checking if both cluster are moving
     {
        if (delta_dist_sq < calib.k_moving_clusters_dist_sq_coarse_gate_1) // checking first, smaller dist gate for moving clusters with lower time_since_measurement diff
        {
           f_success = true;
        }
        else if (delta_dist_sq < calib.k_moving_clusters_dist_sq_coarse_gate_2) // checking larger dist gate for moving clusters with higher time_since_measurement diff
        {
           const float32_t time_diff = std::abs(cluster1.time_since_measurement - cluster2.time_since_measurement);
           if (time_diff > calib.k_moving_clusters_time_diff_coarse_gate)
           {
              f_success = true;
           }
           else
           {
               f_success = false;
           }
        }
        else
        {
            f_success = false;
        }
     }
     else
     {
         f_success = false;
     }

     return f_success;
   }
}

