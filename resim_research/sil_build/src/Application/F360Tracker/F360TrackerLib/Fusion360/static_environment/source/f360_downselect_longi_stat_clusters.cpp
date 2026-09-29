/*===========================================================================*\
* FILE: f360_downselect_longi_stat_clusters.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Downselect_Longi_Stat_Clusters()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_downselect_longi_stat_clusters.h"
#include "f360_math_func.h"


namespace f360_variant_A
{
 /*===========================================================================*\
 * FUNCTION: Downselect_Longi_Stat_Clusters()
 * ===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 *   const uint16_t nr_valid_clusters,
 *   const F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS],
 *   const F360_Calibrations_T& calibs,
 *   uint16_t& nr_downselected_clusters,
 *   F360_Longi_Stat_Cluster_T(&downselected_clusters)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES]
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
 * This function downselects which clusters should be used to fit a polynomial to and
 * thus become valid longi stat curves for this tracker iteration.
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 * None
 *
 \*===========================================================================*/
   void Downselect_Longi_Stat_Clusters(
      const uint16_t nr_valid_clusters,
      const F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS],
      const F360_Calibrations_T& calibs,
      uint16_t& nr_downselected_clusters,
      F360_Longi_Stat_Cluster_T(&downselected_clusters)[MAX_NR_OF_LONGITUDINAL_STAT_CURVES])
   {
      nr_downselected_clusters = 0U;
      float32_t cluster_score_array[NR_LONGI_STAT_CLUSTERS];
      std::fill(cmn::begin(cluster_score_array), cmn::end(cluster_score_array), 0.0F);
      // Find special LSC's and set their score to -INFTY to make sure they will be downselected
      Find_Special_Longi_Clusters(nr_valid_clusters, valid_clusters, cluster_score_array);

      // Calculate score for the rest of LCS's
      constexpr float32_t non_special_cluster_score_threshold = -1.0F;
      for (uint16_t i = 0U; i < nr_valid_clusters; i++)
      {
         // Make sure to calculate score only for non special clusters
         if (cluster_score_array[i] > non_special_cluster_score_threshold)
         {
            cluster_score_array[i] = Calc_Longi_Stat_Cluster_Score(valid_clusters[i], calibs);
         }
      }
      // Sort all clusters by score and downselect clusters with lowest score
      uint32_t cluster_score_array_sorted[NR_LONGI_STAT_CLUSTERS] = {};
      (void)F360_Sort(static_cast<uint32_t>(nr_valid_clusters), true, cluster_score_array, cluster_score_array_sorted);
      for (uint16_t i = 0U; (i < nr_valid_clusters) && (i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES); i++)
      {
         const F360_Longi_Stat_Cluster_T cluster = valid_clusters[cluster_score_array_sorted[i]];
         downselected_clusters[nr_downselected_clusters] = cluster;
         nr_downselected_clusters++;
      }
   }

 /*===========================================================================*\
 * FUNCTION: Calc_Longi_Stat_Cluster_Score()
 * ===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 *   const F360_Longi_Stat_Cluster_T& lsc_cluster
 *   const F360_Calibrations_T& calibs
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
 * This function calculates an "importance score" of a longi stat cluster that
 * wants to create a polynomial. Score is dependent on distance from VCS origin.
 * Function uses logic gates for the only three possible scenarios. 
 *  - Both end points of cluster is negative, curve is behind host
 *  - Both end points of cluster is positive, curve is in front of host
 *  - Lower end point is negative and upper end point is positive, curve is adjacent to host
 * 
 * The clusters longitudinal spread is weigthed in. A longer longi stat curve
 * is considered more important. 
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 * None
 *
 \*===========================================================================*/
   float32_t Calc_Longi_Stat_Cluster_Score(
      const F360_Longi_Stat_Cluster_T& lsc_cluster,
      const F360_Calibrations_T& calibs)
   {
      float32_t cluster_score;

      const float32_t x_max = Get_Cluster_Max_Long_Pos(lsc_cluster);
      const float32_t x_min = Get_Cluster_Min_Long_Pos(lsc_cluster);
      const float32_t delta_x = x_max - x_min;
      // Reference point of the host is half of the host length for this case.
      const float32_t host_center_long_pos = -calibs.k_host_refl_half_host_length;

      if (delta_x > F360_EPSILON)
      {
         // Put punishing score on short curves
         cluster_score = calibs.k_lsc_length_score_gain / delta_x;

         // Distance score is evaluated as either direct lateral distance from middle point of the host or to closest valid x-point, 
         if ((x_min < host_center_long_pos) && (x_max > host_center_long_pos))
         {
            cluster_score += std::abs(lsc_cluster.lat_mean);
         }
         else if (x_min > 0.0F)
         {
            // Cluster is in positive longitudinal quadrants
            cluster_score += F360_Get_Hypotenuse(x_min - host_center_long_pos, lsc_cluster.lat_mean);
         }
         else
         {
            // Cluster is in negative longitudinal quadrants
            cluster_score += F360_Get_Hypotenuse(x_max - host_center_long_pos, lsc_cluster.lat_mean);
         }

      }
      else
      {
         cluster_score = INFTY;
      }

      return cluster_score;
   }

   /*===========================================================================*\
   * FUNCTION: Find_Special_Longi_Clusters()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const uint16_t nr_valid_clusters,
   *   const F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS],
   *   float32_t (&cluster_score_array)[NR_LONGI_STAT_CLUSTERS]
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
   * This function finds special clusters and sets their score to -INFTY to make sure they will be
   * downselected later. Those special clusters are:
   *  - closest to the host on the right if it is in the same longi position as host
   *  - closest to the host on the left if it is in the same longi position as host
   *  - longest one
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Find_Special_Longi_Clusters(
      const uint16_t nr_valid_clusters,
      const F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS],
      float32_t (&cluster_score_array)[NR_LONGI_STAT_CLUSTERS])
   {
      // Select a region where we want to find closest LSC to the left and right of the host
      float32_t left_closest_cluster_lsc_lat_pos = -6.0F;
      float32_t right_closest_cluster_lsc_lat_pos = 6.0F;
      constexpr float32_t lat_mean_limit = 0.0F;
      constexpr float32_t max_longitudinal_limit = -5.0F;
      constexpr float32_t min_longitudinal_limit = 0.0F;

      // Set minimum value for longest LSC
      float32_t longest_cluster_lsc = 30.0F;

      uint16_t left_lsc_idx = nr_valid_clusters;
      uint16_t right_lsc_idx = nr_valid_clusters;
      uint16_t longest_lsc_idx = nr_valid_clusters;

      bool f_right_cluster_valid = false;
      bool f_left_cluster_valid = false;
      bool f_longest_cluster_valid = false;
      for (uint16_t i = 0U; (i < nr_valid_clusters); i++)
      {
         const F360_Longi_Stat_Cluster_T cluster = valid_clusters[i];
         const float32_t max_longitudinal = Get_Cluster_Max_Long_Pos(cluster);
         const float32_t min_longitudinal = Get_Cluster_Min_Long_Pos(cluster);
         const float32_t cluster_length = max_longitudinal - min_longitudinal;
         // Find the LSC that is closest to the right, which is longitudinally within max_longitudinal_limit and min_longitudinal_limit
         if ((cluster.lat_mean < right_closest_cluster_lsc_lat_pos) 
            && (cluster.lat_mean > lat_mean_limit) 
            && (max_longitudinal > max_longitudinal_limit)
            && (min_longitudinal < min_longitudinal_limit))
         {
            right_closest_cluster_lsc_lat_pos = cluster.lat_mean;
            right_lsc_idx = i;
            f_right_cluster_valid = true;
         }
         // Find the LSC that is closest to the left, which is longitudinally within max_longitudinal_limit and min_longitudinal_limit
         else if ((cluster.lat_mean > left_closest_cluster_lsc_lat_pos)
            && (cluster.lat_mean < lat_mean_limit)
            && (max_longitudinal > max_longitudinal_limit)
            && (min_longitudinal < min_longitudinal_limit))
         {
            left_closest_cluster_lsc_lat_pos = cluster.lat_mean;
            left_lsc_idx = i;
            f_left_cluster_valid = true;
         }
         // Find longest LSC, with no consideration to lateral position limit
         else if (cluster_length > longest_cluster_lsc)
         {
            longest_cluster_lsc = cluster_length;
            longest_lsc_idx = i;
            f_longest_cluster_valid = true;
         }
         else
         {
            //Do nothing
         }
      }
      if (f_right_cluster_valid)
      {
         cluster_score_array[right_lsc_idx] = -INFTY;
      }
      if (f_left_cluster_valid)
      {
         cluster_score_array[left_lsc_idx] = -INFTY;
      }
      if (f_longest_cluster_valid)
      {
         cluster_score_array[longest_lsc_idx] = -INFTY;
      }
   }
}
