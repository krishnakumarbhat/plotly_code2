/*===========================================================================*\
* FILE: f360_cluster_objects_for_lsc.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Cluster_Objects_For_LSC()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_cluster_objects_for_lsc.h" 
#include "f360_math_func.h"
#include "f360_calculate_curvi_position.h"

namespace f360_variant_A
{
   /*===========================================================================*\
 * FUNCTION: Cluster_Objects_For_LSC()
 * ===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 *    const F360_Calibrations_T &calibs
 *    const F360_Tracker_Info_T& tracker_info
 *    const F360_Host_T& host
 *    uint16_t& nr_next_ids_of_interest
 *    uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
 *    F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS]
 *    uint16_t& nr_valid_clusters
 *    F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS]
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
 * This function clusters objects to be used for polynomial fit
 *
 * PRECONDITIONS:
 *
 * POSTCONDITIONS:
 * None
 *
 \*===========================================================================*/
   void Cluster_Objects_For_LSC(
      const F360_Calibrations_T& calibs,
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      uint16_t& nr_next_ids_of_interest,
      uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS])
   {
      // Function support variables
      uint16_t ids_of_interest[NUMBER_OF_OBJECT_TRACKS] = {};
      uint16_t skipped_ids_of_interest[NUMBER_OF_OBJECT_TRACKS] = {};
      uint16_t clustered_ids[NUMBER_OF_OBJECT_TRACKS] = {};
      
      bool f_is_data_remaining = true;

      const uint32_t max_number_of_iterations = tracker_info.variant.num_tracks - calibs.k_lsc_min_points_in_cluster + 1U;
      for (uint32_t iteration = 0U; iteration < max_number_of_iterations; iteration++)
      {
         if (f_is_data_remaining)
         {
            // Initialize next iteration
            uint16_t nr_ids_of_interest; // Contains both skipped and not yet reached object id's for this iteration
            uint16_t nr_skipped_ids_of_interest; // Objects that are skipped this iteration is stored here
            uint16_t nr_clustered_ids; // Objects that are clustered this iteration is stored here
            uint16_t current_idx; // Index of ids_of_interest array we reached until we needed to start a new iteration
            
            Init_New_Iteration(
               nr_next_ids_of_interest,
               next_ids_of_interest,
               nr_skipped_ids_of_interest,
               nr_clustered_ids,
               nr_ids_of_interest,
               ids_of_interest);

            // Cluster data points
            Cluster_Longi_Stat_Objects(
               objects,
               nr_ids_of_interest,
               ids_of_interest,
               calibs,
               host,
               current_idx,
               nr_skipped_ids_of_interest,
               skipped_ids_of_interest,
               nr_clustered_ids,
               clustered_ids);

            // Check if cluster contains enough objects and derive useful properties of the cluster
            Handle_LSC_Cluster(
               nr_clustered_ids,
               clustered_ids,
               calibs,
               objects,
               nr_valid_clusters,
               valid_clusters);

            // Arrange remaining data points that have either been skipped or not reached yet
            f_is_data_remaining = Arrange_Remaining_Objects(
               nr_ids_of_interest,
               ids_of_interest,
               current_idx,
               nr_skipped_ids_of_interest,
               skipped_ids_of_interest,
               calibs,
               nr_next_ids_of_interest,
               next_ids_of_interest);
         }
         else
         {
            break;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Init_New_Iteration()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const uint16_t nr_next_ids_of_interest
   *   const uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
   *   uint16_t& nr_skipped_ids_of_interest
   *   uint16_t& nr_clustered_ids
   *   uint16_t& nr_ids_of_interest
   *   uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function resets the data for a new clustering iteration.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Init_New_Iteration(
      const uint16_t nr_next_ids_of_interest,
      const uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_skipped_ids_of_interest,
      uint16_t& nr_clustered_ids,
      uint16_t& nr_ids_of_interest,
      uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS])
   {
      // Reset iteration specific counters
      nr_skipped_ids_of_interest = 0U;
      nr_clustered_ids = 0U;

      // Initialize array of interesting ids
      nr_ids_of_interest = nr_next_ids_of_interest;
      for (uint32_t i = 0U; i < nr_ids_of_interest; i++)
      {
         ids_of_interest[i] = next_ids_of_interest[i];
      }
   }

   /*===========================================================================*\
   * FUNCTION: Cluster_Longi_Stat_Objects()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
   *   const uint16_t nr_ids_of_interest,
   *   const uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
   *   const F360_Calibrations_T& calibs,
   *   uint16_t& current_idx,
   *   uint16_t& nr_skipped_ids_of_interest,
   *   uint16_t(&skipped_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
   *   uint16_t& nr_clustered_ids,
   *   uint16_t(&clustered_ids)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function clusters stationary objects that are candidates to become a 
   * longi stat curve (LSC).
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Cluster_Longi_Stat_Objects(
      const F360_Object_Track_T(&objects)[NUMBER_OF_OBJECT_TRACKS],
      const uint16_t nr_ids_of_interest,
      const uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibs,
      const F360_Host_T& host,
      uint16_t& current_idx,
      uint16_t& nr_skipped_ids_of_interest,
      uint16_t(&skipped_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_clustered_ids,
      uint16_t(&clustered_ids)[NUMBER_OF_OBJECT_TRACKS])
   {

      for (current_idx = 0U; current_idx < nr_ids_of_interest; current_idx++)
      {
         // Extract current object index for easier readability
         const uint16_t obj_idx = ids_of_interest[current_idx] - 1U;

         if (nr_clustered_ids == 0U)
         {
            // Cluster is not started, init a new cluster
            clustered_ids[nr_clustered_ids] = obj_idx + 1U;
            nr_clustered_ids++;
         }
         else
         {
            // A cluster is already started, now check if data point is inside longitudinal gate towards latest added point in cluster
            const float32_t x_pos = objects[obj_idx].bbox.Get_Center().x;
            const uint16_t prev_clustered_idx = clustered_ids[nr_clustered_ids - 1U] - 1U;
            float32_t lsc_long_pos_gate = 0.0F;
            float32_t lsc_lat_pos_gate = 0.0F;

            constexpr float32_t long_lat_pos_gate_fast_speed_threshold = 22.2F; // around 80 KPH
            constexpr float32_t long_lat_pos_gate_low_speed_threshold = 2.8F; // around 10 KPH
            constexpr float32_t long_lat_pos_gate_distance_threshold = 15.0F;
            if ((host.speed > long_lat_pos_gate_fast_speed_threshold))
            {
               lsc_long_pos_gate = calibs.k_lsc_long_pos_gate * 2.0F;
               lsc_lat_pos_gate = calibs.k_lsc_lat_pos_gate * 1.5F;
            }
            else if ((host.speed < long_lat_pos_gate_low_speed_threshold) && (std::abs(x_pos) < long_lat_pos_gate_distance_threshold))
            {
               lsc_long_pos_gate = calibs.k_lsc_long_pos_gate * 0.75F;
               lsc_lat_pos_gate = calibs.k_lsc_lat_pos_gate * 0.75F;
            }
            else
            {
               lsc_long_pos_gate = calibs.k_lsc_long_pos_gate;
               lsc_lat_pos_gate = calibs.k_lsc_lat_pos_gate;
            }


            if ((x_pos - objects[prev_clustered_idx].bbox.Get_Center().x) > lsc_long_pos_gate)
            {
               // No more matches to current cluster possible 
               break;
            }
            else
            {
               // Data point matched in long gate, now check lateral
               const float32_t y_pos = objects[obj_idx].bbox.Get_Center().y;
               if (std::abs(y_pos - objects[prev_clustered_idx].bbox.Get_Center().y) > lsc_lat_pos_gate)
               {
                  // Point is too far away in lateral direction, add to ids of interest for next iteration
                  skipped_ids_of_interest[nr_skipped_ids_of_interest] = ids_of_interest[current_idx];
                  nr_skipped_ids_of_interest++;
               }
               else
               {
                  // Point matches cluster, add to cluster points array
                  clustered_ids[nr_clustered_ids] = obj_idx + 1U;
                  nr_clustered_ids++;
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Handle_LSC_Cluster()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   *   const uint16_t nr_clustered_ids
   *   const uint16_t(&clustered_ids)[NUMBER_OF_OBJECT_TRACKS]
   *   const F360_Calibrations_T& calibs
   *   F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *   uint16_t& nr_valid_clusters
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
   * This function checks if the current found cluster is valid and if so,
   * derives some useful data of the cluster used in downselection
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Handle_LSC_Cluster(
      const uint16_t nr_clustered_ids,
      const uint16_t(&clustered_ids)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      uint16_t& nr_valid_clusters,
      F360_Longi_Stat_Cluster_T(&valid_clusters)[NR_LONGI_STAT_CLUSTERS])
   {

      if (nr_clustered_ids >= calibs.k_lsc_min_points_in_cluster)
      {
         // Cluster contains enough data points to fit polynomial, we thus consider it valid
         valid_clusters[nr_valid_clusters].nr_objects = nr_clustered_ids;

         // Update objects in cluster
         uint16_t current_obj_idx = clustered_ids[0U] - 1U;
         uint16_t next_obj_idx = clustered_ids[1U] - 1U;
         uint16_t prev_obj_idx;
         valid_clusters[nr_valid_clusters].first_object = &object_tracks[current_obj_idx];
         object_tracks[current_obj_idx].lsc_next_in_cluster = &object_tracks[next_obj_idx];
         for (uint16_t i = 1U; i < (nr_clustered_ids - 1U); i++)
         {
            current_obj_idx = clustered_ids[i] - 1U;
            next_obj_idx = clustered_ids[i + 1U] - 1U;
            prev_obj_idx = clustered_ids[i - 1U] - 1U;

            object_tracks[current_obj_idx].lsc_next_in_cluster = &object_tracks[next_obj_idx];
            object_tracks[current_obj_idx].lsc_prev_in_cluster = &object_tracks[prev_obj_idx];
         }

         current_obj_idx = clustered_ids[nr_clustered_ids - 1U] - 1U;
         prev_obj_idx = clustered_ids[nr_clustered_ids - 2U] - 1U;
         object_tracks[current_obj_idx].lsc_prev_in_cluster = &object_tracks[prev_obj_idx];
         valid_clusters[nr_valid_clusters].last_object = &object_tracks[current_obj_idx];

         // Calculate lateral mean and height of cluster
         float32_t lat_pos_array[NUMBER_OF_OBJECT_TRACKS] = {};
         float32_t otg_height_array[NUMBER_OF_OBJECT_TRACKS] = {};
         uint32_t n_valid_obj_for_mean_height = 0U;
         F360_Object_Track_T* current_obj = valid_clusters[nr_valid_clusters].first_object;
         for (uint16_t i = 0U; i < nr_clustered_ids; i++)
         {
            lat_pos_array[i] = current_obj->bbox.Get_Center().y;
            if (current_obj->otg_height > 0.0F)
            {
               // Only use object's with non-zero otg_height since not all stationary object's heights are calculated in the Post Update module.
               otg_height_array[i] = current_obj->otg_height;
               n_valid_obj_for_mean_height++;
            }
            current_obj = current_obj->lsc_next_in_cluster;
         }
         valid_clusters[nr_valid_clusters].lat_mean = F360_Mean(lat_pos_array, static_cast<uint32_t>(nr_clustered_ids));
         valid_clusters[nr_valid_clusters].height_mean = F360_Mean(otg_height_array, n_valid_obj_for_mean_height);

         // Increase the number of total valid clusters
         nr_valid_clusters++;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Arrange_Remaining_Objects()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   const uint16_t nr_ids_of_interest,
   *   const uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
   *   const uint16_t current_idx,
   *   const uint16_t nr_skipped_ids_of_interest,
   *   const uint16_t(&skipped_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
   *   const F360_Calibrations_T& calibs,
   *   uint16_t& nr_next_ids_of_interest,
   *   uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS]
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
   * This function arranges data for the next iteration of clustering. It appends
   * the skipped ids with the remaining ids that haven't been reached yet.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Arrange_Remaining_Objects(
      const uint16_t nr_ids_of_interest,
      const uint16_t(&ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      const uint16_t current_idx,
      const uint16_t nr_skipped_ids_of_interest,
      const uint16_t(&skipped_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calibs,
      uint16_t& nr_next_ids_of_interest,
      uint16_t(&next_ids_of_interest)[NUMBER_OF_OBJECT_TRACKS])
   {

      bool f_data_remaining;

      // Append ids of interest with both skipped points and remaining points.
      const uint16_t nr_remaining_ids = nr_ids_of_interest - current_idx;
      nr_next_ids_of_interest = nr_skipped_ids_of_interest + nr_remaining_ids;

      if (nr_next_ids_of_interest >= calibs.k_lsc_min_points_in_cluster)
      {
         f_data_remaining = true;

         uint32_t i;
         for (i = 0U; i < nr_skipped_ids_of_interest; i++)
         {
            next_ids_of_interest[i] = skipped_ids_of_interest[i];
         }
         for (uint32_t k = current_idx; k < nr_ids_of_interest; k++)
         {
            next_ids_of_interest[i] = ids_of_interest[k];
            i++;
         }
      }
      else
      {
         f_data_remaining = false;
         nr_next_ids_of_interest = 0U;
      }
      
      return f_data_remaining;
   }
}

