/*===================================================================================*\
* FILE: f360_dbscan.cpp
*====================================================================================
*Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
\*===================================================================================*/
#include "f360_dbscan.h"
#include "f360_math.h"
#include "f360_compute_wrapping_aware_spread.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: DBscan()
   *===========================================================================
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Create detection clusters and prepare the output_data structure.
   * Only create up to MAX_TRACKER_POSN_CLUSTERS per call to this function.
   \*===========================================================================*/
   void DBscan(
      const F360_Tracker_Info_T& tracker_info,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      const int16_t(&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS],
      const int16_t num_sorted_dets,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Local_Clusters_T& output_data)
   {
      bool f_detection_clustered[MAX_NUMBER_OF_DETECTIONS];
      (void)memset(&f_detection_clustered[0], 0, sizeof(f_detection_clustered));

      output_data.num_of_associated_dets = 0U;
      output_data.num_clusters = 0U;

      for (int16_t i = 0; i < num_sorted_dets; i++)
      {
         if (output_data.num_clusters < tracker_info.variant.num_posn_clusters)
         {
            const int16_t detection_index = sorted_det_idxs[i];

            if (!f_detection_clustered[detection_index])
            {
               // If this detection is not clustered yet
               Cluster_Expand(raw_detections, sensors, valid_dets, f_cluster_moving, tracker_info.variant.num_dets_in_track, detection_index, det_props, f_detection_clustered, output_data);
            }
         }
         else
         {
            // Only create a maximum of num_posn_clusters new clusters
            break;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Cluster_Expand()
   *===========================================================================
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * For a cluster with a single detection, find additional detections within
   * the range, and if applicable, range-rate criteria, and add them to the
   * cluster. Only allow up to MAX_DETS_IN_OBJ_TRK number of detections in
   * a cluster.
   \*===========================================================================*/
   void Cluster_Expand(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      const uint8_t max_num_dets,
      const int16_t detection_index,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      bool(&f_detection_clustered)[MAX_NUMBER_OF_DETECTIONS],
      F360_Local_Clusters_T& output_data)
   {
      int16_t cluster_dets_idx[MAX_DETS_IN_OBJ_TRK];
      uint8_t n_cluster_dets = 1U;

      cluster_dets_idx[0] = detection_index;
      f_detection_clustered[detection_index] = true;

      // For each cluster memeber, call the region query with them as base
      for (uint8_t i = 0U; i < (max_num_dets - 1U); i++)
      {
         if ((n_cluster_dets < max_num_dets) && (i < n_cluster_dets))
         {
            // Find the neighbor points
            const int16_t det_idx = cluster_dets_idx[i];
            Cluster_Region_Query(raw_detections, sensors, valid_dets, f_cluster_moving, max_num_dets, det_idx, det_props, f_detection_clustered, cluster_dets_idx, n_cluster_dets);
         }
         else
         {
            // The cluster has grown to it's maximum allowed size, or no additional eligible detections are nearby
            break;
         }
      }

      // Update output
      output_data.array_of_first_det_idx_in_clusters[output_data.num_clusters] = output_data.num_of_associated_dets;
      for (uint8_t i = 0U; i < n_cluster_dets; i++)
      {
         const int16_t det_idx = cluster_dets_idx[i];
         output_data.array_of_det_idxs_in_clusters[output_data.num_of_associated_dets] = det_idx;
         output_data.num_of_associated_dets++;
      }
      output_data.num_dets_in_clusters[output_data.num_clusters] = n_cluster_dets;
      output_data.num_clusters++;
   }

   /*===========================================================================*\
   * FUNCTION: Cluster_Region_Query()
   *===========================================================================
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * For a detection in a cluster, find nearby detections and add them to the
   * cluster if they fulfill the relevant criteria. Iterate through nearby
   * detections using the longitudinal sorted list.
   \*===========================================================================*/
   void Cluster_Region_Query(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      const uint8_t max_num_dets,
      const int16_t detection_index,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      bool(&f_detection_clustered)[MAX_NUMBER_OF_DETECTIONS],
      int16_t(&cluster_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      uint8_t& n_cluster_dets)
   {
      const float32_t moving_dets_clustering_radius = 2.0F;
      const float32_t ambig_dets_clustering_radius = 1.5F;
      const float32_t clustering_radius = f_cluster_moving ? moving_dets_clustering_radius : ambig_dets_clustering_radius;
      const float32_t clustering_range_sq = clustering_radius * clustering_radius;
      const F360_Detection_Props_T& det_base = det_props[detection_index];

      // Precompute base detection's sensor info for interval-aware range rate comparison
      const int32_t base_sens_idx = raw_detections.detections[detection_index].raw.sensor_id - 1;
      const F360_Radar_Sensor_T& sensor_base = sensors[base_sens_idx];
      const float32_t base_v_wrapping = sensor_base.constant.v_wrapping[sensor_base.variable.look_id];
      const float32_t base_r_wrapping = sensor_base.constant.r_wrapping[sensor_base.variable.look_id];

      for (int8_t direction = 0; direction < 2; direction++)
      {
         bool f_continue = true;
         int16_t candidate_index = detection_index;

         for (uint16_t i = 0U; i < raw_detections.number_of_valid_detections; i++)
         {
            if (direction == 0)
            {
               candidate_index = raw_detections.detections[candidate_index].processed.next_sorted_idx; // iterate forward
            }
            else
            {
               candidate_index = raw_detections.detections[candidate_index].processed.prev_sorted_idx; // iterate backwards
            }

            if ((candidate_index == F360_INVALID_ID) || (n_cluster_dets == max_num_dets))
            {
               f_continue = false;
            }
            else
            {
               const F360_Detection_Props_T& det_candidate = det_props[candidate_index];
               const float32_t longpos_diff = det_candidate.vcs_position.x - det_base.vcs_position.x;
               if (fabsf(longpos_diff) > clustering_radius)
               {
                  f_continue = false;
               }
               else if(valid_dets[candidate_index] && (!f_detection_clustered[candidate_index]))
               {
                  // Check position gate
                  const float32_t latpos_diff = det_candidate.vcs_position.y - det_base.vcs_position.y;
                  const float32_t range_diff_sq = latpos_diff * latpos_diff + longpos_diff * longpos_diff;
                  const bool f_range_pass = range_diff_sq < clustering_range_sq;

                  // Check range-rate gate when clustering moving detections, and automatically pass the gate otherwise
                  bool f_rdot_pass = false;
                  bool f_wrapped_match = false;
                  if (f_cluster_moving)
                  {
                     const float32_t range_rate_gate = 2.0F;
                     const float32_t base_range_rate_diff = std::abs(det_candidate.range_rate_dealiased - det_base.range_rate_dealiased);
                     f_rdot_pass = (base_range_rate_diff < range_rate_gate);

                     // Interval-aware range rate comparison using wrapping-aware spread
                     if (!f_rdot_pass)
                     {
                        const float32_t wrapped_spread = Compute_Wrapping_Aware_Spread(
                           det_candidate.range_rate_dealiased, det_base.range_rate_dealiased,
                           raw_detections.detections[candidate_index], raw_detections.detections[detection_index],
                           sensors);
                        if (wrapped_spread < range_rate_gate)
                        {
                           f_rdot_pass = true;
                           f_wrapped_match = true;
                        }
                     }
                  }
                  else
                  {
                     f_rdot_pass = true;
                  }

                  if (f_range_pass && f_rdot_pass)
                  {
                     // If the detection was matched via interval wrapping, adjust its range rate and range to be numerically close
                     if (f_wrapped_match)
                     {
                        // Shift candidate by one interval toward the base detection
                        const float32_t sign = (det_base.range_rate_dealiased > det_candidate.range_rate_dealiased) ? 1.0F : -1.0F;
                        det_props[candidate_index].range_rate_dealiased += sign * base_v_wrapping;
                        det_props[candidate_index].range_rate_compensated += sign * base_v_wrapping;

                        // Compensate range for Doppler wrapping
                        F360_Compensate_Det_Range(sign, base_r_wrapping,
                           raw_detections.detections[candidate_index],
                           det_props[candidate_index]);
                     }

                     f_detection_clustered[candidate_index] = true;
                     cluster_dets_idx[n_cluster_dets] = candidate_index;
                     n_cluster_dets++;
                  }
               }
               else
               {
                  // continue
               }
            }

            if (!f_continue)
            {
               // break the inner loop and change direction, or exit if done
               break;
            }
         }
      }
   }
}
