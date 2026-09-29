/*===========================================================================*\
* FILE: f360_post_process_longi_stat_clusters.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Post_Process_Longi_Stat_Clusters()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_post_process_longi_stat_clusters.h"
#include "f360_iterator.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include <algorithm>


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Post_Process_Longi_Stat_Clusters()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Calibrations_T& calibs,
   *   uint16_t&nr_valid_clusters,
   *   F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
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
   * Performs post processing on clusters of longitudinal sorted objects.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Post_Process_Longi_Stat_Clusters(
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      const float32_t host_turn_radius,
      uint16_t&nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS])
   {
      Remove_Overhead_Longi_Stat_Clusters(nr_valid_clusters, valid_clusters);

      Merge_Longi_Stat_Clusters(calibs, tracker_info, nr_valid_clusters, valid_clusters);

      Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(calibs, host_turn_radius, nr_valid_clusters, valid_clusters);
   }

   /*===========================================================================*\
   * FUNCTION: Check_If_Object_In_Path_Of_Host
   * ===========================================================================
   * RETURN VALUE:
   * bool f_not_in_allowed_zone
   *
   * PARAMETERS:
   * const F360_Calibrations_T& calibs
   * const float32_t host_turn_radius
   * const F360_Object_Track_T& obj
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
   * This function checks if an object is in host path, based on its distance to
   * host path
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Check_If_Object_In_Path_Of_Host(
      const F360_Calibrations_T& calibs,
      const float32_t host_turn_radius,
      const F360_Object_Track_T& obj
   )
   {
      float dist_to_circle;
      if (host_turn_radius < INFTY)
      {
         dist_to_circle = std::abs(F360_Sqrtf((obj.vcs_position.y - host_turn_radius) * (obj.vcs_position.y - host_turn_radius) + obj.vcs_position.x * obj.vcs_position.x) - std::abs(host_turn_radius));
      }
      else
      {
         dist_to_circle = std::abs(obj.vcs_position.y);
      }
      const bool f_in_allowed_zone = ((dist_to_circle > calibs.k_distance_to_circle_thr) || ((std::abs(obj.vcs_position.x + calibs.k_host_refl_half_host_length) < 7.0F) && (dist_to_circle > 1.0F)));
      return (!f_in_allowed_zone);
   }

   /*===========================================================================*\
   * FUNCTION: Remove_Overhead_Longi_Stat_Clusters()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   uint16_t&nr_valid_clusters,
   *   F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
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
   * This function iterates over clusters and removes those that have a mean height above
   * the allowed threshold.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Remove_Overhead_Longi_Stat_Clusters(
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS])
   {
      constexpr float32_t max_height_for_downselection = 4.0F;
      uint16_t nr_ok_height_clusters = 0U;
      F360_Longi_Stat_Cluster_T ok_height_clusters[NR_LONGI_STAT_CLUSTERS] = {};
      (void)std::copy(&valid_clusters[0], &valid_clusters[nr_valid_clusters], cmn::begin(ok_height_clusters));
      for (uint16_t i = 0U; i < nr_valid_clusters; i++)
      {
         const float32_t cluster_mean_height = valid_clusters[i].height_mean;
         if (cluster_mean_height < max_height_for_downselection)
         {
            ok_height_clusters[nr_ok_height_clusters] = valid_clusters[i];
            nr_ok_height_clusters++;
         }
      }
      nr_valid_clusters = nr_ok_height_clusters;
      (void)std::copy(&ok_height_clusters[0], &ok_height_clusters[nr_valid_clusters], cmn::begin(valid_clusters));
   }

   /*===========================================================================*\
   * FUNCTION: Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Calibrations_T& calibs,
   *   const float32_t host_turn_radius,
   *   uint16_t& nr_valid_clusters,
   *   F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
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
   * This function filters out LSC clusters based on 
   * two criteria: the percentage of objects in the path of the host and the 
   * mean height of the cluster. The function iterates through each cluster 
   * and evaluates its objects to determine if they are in the path of the host 
   * using the `Check_If_Object_In_Path_Of_Host` function. For each cluster, 
   * the percentage of objects in the path is calculated. If this percentage 
   * below 40% or if the mean height of the cluster is below 2.0 meters, the 
   * cluster is kept, otherwise the LSC cluster is removed.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Allow_LSCs_With_Low_Height_Or_Most_Objects_Not_In_Path(
      const F360_Calibrations_T& calibs,
      const float32_t host_turn_radius,
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS])
   {
      uint16_t nr_of_ok_clusters = 0U; // Counter for LSC clusters that pass the criteria.
      F360_Longi_Stat_Cluster_T ok_clusters[NR_LONGI_STAT_CLUSTERS] = {};
      (void)std::copy(&valid_clusters[0], &valid_clusters[nr_valid_clusters], cmn::begin(ok_clusters)); // Copy all clusters to the temporary array.
      // Iterate through each valid cluster to evaluate its objects.
      for (uint16_t i = 0U; i < nr_valid_clusters; i++)
      {

         const float32_t first_obj_long_pos = valid_clusters[i].first_object->vcs_position.x;
         const float32_t last_obj_long_pos = valid_clusters[i].last_object->vcs_position.x;
         const float32_t lsc_mid_long_pos = first_obj_long_pos + (last_obj_long_pos - first_obj_long_pos)*0.5F;
         
         // If the cluster's mean height is below 2.0 meters, and the mid longitdiinal position of LSC is less than 60m, it is automatically retained.
         // lsc_mid_long_pos is checked because beyond 80m the otg_height is not calculated and in such case the LSC only calulates the height based on the objects that have non_zero otg_height
         // So, it is possible that the height of the LSC is just based on a few objects, which could be misrepresentative
         // Addtionally, at far ranges the height estimation tends to become less reliable
         // For some LSC clusters, height mean can be 0.0F, because it is formed from objects whose otg_height is not calculated (i.e zero)
         if ((valid_clusters[i].height_mean > 0.0F) && (valid_clusters[i].height_mean < 2.0F) && (std::abs(lsc_mid_long_pos) < 60.0F))
         {
            ok_clusters[nr_of_ok_clusters] = valid_clusters[i];
            nr_of_ok_clusters++;
            continue;
         }
         else
         {
            const F360_Object_Track_T* p_obj = valid_clusters[i].first_object; // pointer to the first object in lSC cluster
            uint16_t nr_objects_in_path_in_current_cluster = 0U; // Counter for objects in the path of the host within the current cluster.
            float32_t percentage_of_objects_in_path_in_current_cluster = 0.0F; // Percentage of objects in the path of the host within the current cluster.

            // if the cluster height is above 2m, check how many objects are in path of host
            // Iterate through all objects in the current cluster.
            for (uint32_t k=0U; k < valid_clusters[i].nr_objects; k++) 
            {
               // Break the loop if there are no more objects in the cluster or if the first object points to a nullptr.
               if (p_obj == nullptr)
               {
                  break;
               }
               // Check if the object is in the path of the host.
               if (Check_If_Object_In_Path_Of_Host(calibs, host_turn_radius, *p_obj))
               {
                  // Increment the counter for number of objects in path
                  nr_objects_in_path_in_current_cluster++;
               }
               // Move to the next object in the cluster.
               p_obj = p_obj->lsc_next_in_cluster;
            }

            // Calculate the percentage if the object is in the path of the host.
            percentage_of_objects_in_path_in_current_cluster = static_cast<float32_t>(nr_objects_in_path_in_current_cluster) / static_cast<float32_t>(valid_clusters[i].nr_objects);

            // Keep clusters that have less than 40% of objects in path of the host and height is above 2m (check before)
            if (percentage_of_objects_in_path_in_current_cluster < 0.4F)
            {
               ok_clusters[nr_of_ok_clusters] = valid_clusters[i];
               nr_of_ok_clusters++;
            }
         }
      }
      // Update the number of valid clusters and copy the valid clusters that passed the criteria back to the original array.
      nr_valid_clusters = nr_of_ok_clusters;
      (void)std::copy(&ok_clusters[0], &ok_clusters[nr_valid_clusters], cmn::begin(valid_clusters));
   }

   /*===========================================================================*\
   * FUNCTION: Merge_Longi_Stat_Clusters()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Calibrations_T& calibs,
   *   uint16_t&nr_valid_clusters,
   *   F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
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
   * This function iterates over clusters and attempts to 
   * merge clusters. Function is done when no more merges was done by
   * Try_To_Merge_Longi_Stat_Clusters() function.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Merge_Longi_Stat_Clusters(
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS])
   {

      uint16_t nr_next_clusters = nr_valid_clusters;
      F360_Longi_Stat_Cluster_T next_clusters[NR_LONGI_STAT_CLUSTERS] = {};
      (void)std::copy(&valid_clusters[0], &valid_clusters[nr_next_clusters], cmn::begin(next_clusters));

      // Find max number of comparisons that can be done using an arithmetic sum formula
      const uint16_t max_iterations = (nr_next_clusters * (nr_next_clusters - 1U)) / 2U;

      uint16_t next_idx = 1U;
      F360_Longi_Stat_Cluster_T clusters[NR_LONGI_STAT_CLUSTERS] = {};
      for (uint16_t iter = 0U; iter < max_iterations; iter++)
      {
         const uint16_t nr_clusters = nr_next_clusters;
         (void)std::copy(&next_clusters[0], &next_clusters[nr_clusters], cmn::begin(clusters));

         const bool f_merge_occurred = Try_To_Merge_Longi_Stat_Clusters(nr_clusters, clusters, calibs, tracker_info, next_idx, nr_next_clusters, next_clusters);

         if (!f_merge_occurred)
         {
            //If no merge occurred, the merge logic is done
            break;
         }
      }

      // Update cluster array after merge logic have completed
      nr_valid_clusters = nr_next_clusters;
      (void)std::copy(&next_clusters[0], &next_clusters[nr_valid_clusters], cmn::begin(valid_clusters));

   }

   /*===========================================================================*\
   * FUNCTION: Try_To_Merge_Longi_Stat_Clusters()
   * ===========================================================================
   * RETURN VALUE:
   * bool f_merged_occurred
   *
   * PARAMETERS:
   *   const uint16_t nr_clusters,
   *   const F360_Longi_Stat_Cluster_T(&clusters)[NR_LONGI_STAT_CLUSTERS],
   *   const F360_Calibrations_T& calibs,
   *   uint16_t& next_idx,
   *   uint16_t& nr_next_clusters,
   *   F360_Longi_Stat_Cluster_T(&next_clusters)[NR_LONGI_STAT_CLUSTERS]
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
   * Returns true if two clusters was merged. If clusters are merged the function 
   * also arranges the updated cluster information in "next clusters" that should
   * be used in next iteration. Idea is that this function should be called until 
   * it returns false. Meaning that algo has run through all clusters and no more
   * merges was done.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Try_To_Merge_Longi_Stat_Clusters(
      const uint16_t nr_clusters,
      const F360_Longi_Stat_Cluster_T(&clusters)[NR_LONGI_STAT_CLUSTERS],
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      uint16_t& next_idx,
      uint16_t& nr_next_clusters,
      F360_Longi_Stat_Cluster_T(&next_clusters)[NR_LONGI_STAT_CLUSTERS])
   {
      bool f_merge_occurred = false;

      for (uint16_t i = 0U; i < (nr_clusters - 1U); i++)
      {
         // This cluster will always have a lower x_min value than cluster k since cluster have been created in ascending longitudinal VCS order
         const F360_Longi_Stat_Cluster_T cluster_i = clusters[i];

         // Create longi distance threshold multiplier for merging clusters that are far away
         constexpr float32_t longi_pos_threshold_gap_coef = 40.0F;
         const float32_t far_clusters_multiplier = tracker_info.f_highway_suspected ? 5.0F : 2.0F;
         const float32_t close_cluster_multiplier = tracker_info.f_highway_suspected ? 2.5F : 1.0F;
         const float32_t longi_gap_coef = Get_Cluster_Max_Long_Pos(cluster_i) > longi_pos_threshold_gap_coef ? far_clusters_multiplier : close_cluster_multiplier;

         for (uint16_t k = next_idx; k < nr_clusters; k++)
         {
            const F360_Longi_Stat_Cluster_T cluster_k = clusters[k];

            if ((std::abs(cluster_i.last_object->vcs_position.y - cluster_k.first_object->vcs_position.y) < calibs.k_lsc_lat_merging_gate) &&
               (std::abs(Get_Cluster_Max_Long_Pos(cluster_i) - Get_Cluster_Min_Long_Pos(cluster_k)) < calibs.k_lsc_long_merging_gate * longi_gap_coef))
            {
               // Clusters have roughly the same start/end position, do detailed check on the mean lateral position
               // of the objects in each cluster close to the end points
               const F360_Object_Track_T* obj_i = cluster_i.last_object;
               const F360_Object_Track_T* obj_k = cluster_k.first_object;

               float32_t cluster_i_lat_pos_array[NUMBER_OF_OBJECT_TRACKS] = {};
               float32_t cluster_k_lat_pos_array[NUMBER_OF_OBJECT_TRACKS] = {};
               for (uint32_t cluster_point = 0U; cluster_point < calibs.k_lsc_min_points_in_cluster; cluster_point++)
               {
                  cluster_i_lat_pos_array[cluster_point] = obj_i->bbox.Get_Center().y;
                  cluster_k_lat_pos_array[cluster_point] = obj_k->bbox.Get_Center().y;

                  obj_i = obj_i->lsc_prev_in_cluster;
                  obj_k = obj_k->lsc_next_in_cluster;
               }
               const float32_t mean_cluster_i = F360_Mean(cluster_i_lat_pos_array, calibs.k_lsc_min_points_in_cluster);
               const float32_t mean_cluster_k = F360_Mean(cluster_k_lat_pos_array, calibs.k_lsc_min_points_in_cluster);

               if (std::abs(mean_cluster_i - mean_cluster_k) < calibs.k_lsc_cluster_merge_thr)
               {
                  f_merge_occurred = true;
                  next_idx = k;
                  // Merge clusters
                  Merge_Longi_Stat_Cluster_Pair(nr_clusters, clusters, i, k, nr_next_clusters, next_clusters);

                  // We need another merge try iteration, break inner loop
                  break;
               }
            }
         }

         if (f_merge_occurred)
         {
            // Inner loop have merged two clusters, we need to run a new merge try iteration
            break;
         }
         else
         {
            // Inner loop did not merge two clusters, keep checking by increasing outer loop
            // and resetting inner loop counter
            next_idx = (i + 2U);
         }
      }

      return f_merge_occurred;

   }

   /*===========================================================================*\
   * FUNCTION: Merge_Longi_Stat_Cluster_Pair()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const uint16_t nr_clusters
   *   const F360_Longi_Stat_Cluster_T(&clusters)[NR_LONGI_STAT_CLUSTERS]
   *   const uint16_t primary_cluster_idx
   *   const uint16_t secondary_cluster_idx
   *   uint16_t &nr_next_clusters
   *   F360_Longi_Stat_Cluster_T(&next_clusters)[NR_LONGI_STAT_CLUSTERS]
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
   * This function merges two clusters. The primary cluster will inherit all objects
   * of secondary cluster. Primary cluster properties are updated and secondary
   * cluster is removed from the array of interesting clusters for next iteration.
   *
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Merge_Longi_Stat_Cluster_Pair(
      const uint16_t nr_clusters,
      const F360_Longi_Stat_Cluster_T(&clusters)[NR_LONGI_STAT_CLUSTERS],
      const uint16_t primary_cluster_idx,
      const uint16_t secondary_cluster_idx,
      uint16_t&nr_next_clusters,
      F360_Longi_Stat_Cluster_T(&next_clusters)[NR_LONGI_STAT_CLUSTERS])
   {

      // Stitch together the clustered objects from each cluster
      F360_Object_Track_T* const object_primary = clusters[primary_cluster_idx].last_object;
      F360_Object_Track_T* const object_secondary = clusters[secondary_cluster_idx].first_object;
      object_primary->lsc_next_in_cluster = object_secondary;
      object_secondary->lsc_prev_in_cluster = object_primary;
      next_clusters[primary_cluster_idx].nr_objects += clusters[secondary_cluster_idx].nr_objects;
      next_clusters[primary_cluster_idx].first_object = clusters[primary_cluster_idx].first_object;
      next_clusters[primary_cluster_idx].last_object = clusters[secondary_cluster_idx].last_object;

      // Update lateral mean
      const float32_t nr_obj_primary = static_cast<float32_t>(clusters[primary_cluster_idx].nr_objects);
      const float32_t nr_obj_secondary = static_cast<float32_t>(clusters[secondary_cluster_idx].nr_objects);
      
      next_clusters[primary_cluster_idx].lat_mean = ((nr_obj_primary * clusters[primary_cluster_idx].lat_mean) +
         (nr_obj_secondary * clusters[secondary_cluster_idx].lat_mean)) /
         (nr_obj_primary + nr_obj_secondary);

      // Remove secondary cluster
      for (uint16_t i = secondary_cluster_idx; i < (nr_clusters - 1U); i++)
      {
         next_clusters[i] = clusters[i + 1U];
      }
      nr_next_clusters--;

   }
}

