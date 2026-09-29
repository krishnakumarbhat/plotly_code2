/*===================================================================================*\
* FILE: f360_merge_two_clusters.cpp
* ====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function to merge two clusters
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

/******************************
* Includes
*******************************/
#include <algorithm>

#include "f360_math.h"
#include "f360_iterator.h"
#include "f360_math_func.h"
#include "f360_merge_two_clusters.h"
#include "f360_get_unique_rdot_interval_ids.h"
#include "f360_is_two_look_type_ok_combine.h"
#include "f360_sorted_clusters_mgmt.h"
#include "f360_norm_heading_angle.h"
#include "f360_cluster_detection_downselection.h"
#include "f360_compute_wrapping_aware_spread.h"

namespace f360_variant_A
{
   static float32_t Dealias_Range_Rates_In_A_Cluster(
      const F360_Cluster_T & cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      const float32_t dealiasing_interval_width,
      const float32_t dealiasing_interval,
      float32_t cluster_rdotcomp,
      int32_t n_rdot_ests);

   static void Collect_Dealiased_Dets_In_Older_Cluster(
      const F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T& cluster_older,
      F360_Cluster_T& cluster_newer,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Hist_T& det_hist);

   static void Update_Cluster_Dets_Associations(
      const Cluster_Dets& cluster_detections,
      F360_Cluster_T& cluster,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T& det_hist);

   static void Update_Cluster_Det_Types(
      F360_Cluster_T& cluster,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T & det_hist);

   static void Dealias_Detections_To_Target_Range_Rate(
      const F360_Cluster_T & cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      const float32_t target_range_rate,
      const float32_t k_max_dealiased_range_rate_diff);

   static inline void F360_Compensate_Detection_Hist_Range(
      const float32_t dealiasing_interval,
      F360_Detection_Hist_Data_T &det_data);

   static void Manage_Low_RCS_Dets_Counters(
      const F360_Cluster_T& cluster_newer,
      F360_Cluster_T& cluster_older);

   static void Determine_Where_Detections_Go(
      const F360_Cluster_T& cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& det_hist,
      Cluster_Dets& old_cluster_dets,
      Cluster_Dets& new_cluster_dets);


   static float32_t Find_Largest_Detection_Time_Since_Meas(
      const F360_Cluster_T& cluster,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T& det_hist);

   /*===========================================================================*\
    * FUNCTION: Merge_Two_Clusters()
    *===========================================================================
    * RETURN VALUE:
    * f_need_to_kill_id_older
    *
    * PARAMETERS:
    * const F360_Tracker_Info_T& tracker_info,
    * const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
    * F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
    * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
    * F360_Detection_Hist_T & det_hist
    * const int32_t cluster_id_1
    * const int32_t cluster_id_2
    * const float32_t rngrate_interval_width_1
    * const float32_t rngrate_interval_width_2
    * const float32_t interval_1
    * const float32_t interval_2
    * const float32_t k_max_dealiased_range_rate_diff
    *
    * EXTERNAL REFERENCES: mergeTwoUnconfTrks.m
    * None.
    *
    * DEVIATIONS FROM STANDARDS:
    * None.
    *
    * --------------------------------------------------------------------------
    * ABSTRACT:
    * --------------------------------------------------------------------------
    * This function merge two unconform tracks
    *
    * PRECONDITIONS:
    * None.
    *
    * POSTCONDITIONS:
    * None.
    *
    \*===========================================================================*/
   void Merge_Two_Clusters(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      const int16_t cluster_id_1,
      const int16_t cluster_id_2,
      const float32_t rngrate_interval_width_1,
      const float32_t rngrate_interval_width_2,
      const float32_t interval_1,
      const float32_t interval_2,
      const float32_t k_max_dealiased_range_rate_diff)
   {
      float32_t cluster_rdotcomp_older;
      float32_t cluster_rdotcomp_newer;

      float32_t rngrate_interval_width_older;
      float32_t rngrate_interval_width_newer;
      float32_t interval_older;
      float32_t interval_newer;

      const float32_t largest_time_since_meas_cluster1 = Find_Largest_Detection_Time_Since_Meas(clusters[cluster_id_1 - 1], raw_detection_list, sensors, det_hist);
      const float32_t largest_time_since_meas_cluster2 = Find_Largest_Detection_Time_Since_Meas(clusters[cluster_id_2 - 1], raw_detection_list, sensors, det_hist);

      int16_t cluster_newer_idx;
      int16_t cluster_older_idx;
      if (largest_time_since_meas_cluster1 > largest_time_since_meas_cluster2)
      {
         cluster_newer_idx = cluster_id_2 - 1;
         cluster_older_idx = cluster_id_1 - 1;
         interval_newer = interval_2;
         interval_older = interval_1;
         rngrate_interval_width_newer = rngrate_interval_width_2;
         rngrate_interval_width_older = rngrate_interval_width_1;
      }
      else
      {
         cluster_newer_idx = cluster_id_1 - 1;
         cluster_older_idx = cluster_id_2 - 1;
         interval_newer = interval_1;
         interval_older = interval_2;
         rngrate_interval_width_newer = rngrate_interval_width_1;
         rngrate_interval_width_older = rngrate_interval_width_2;
      }

      const bool f_use_cluster_props_2_for_old_cluster = (clusters[cluster_id_1 - 1].time_since_cluster_updated > clusters[cluster_id_2 - 1].time_since_cluster_updated);
      const int16_t id_to_use_when_updating_older_cluster_props = f_use_cluster_props_2_for_old_cluster ? cluster_id_2 : cluster_id_1;
      const int16_t id_to_use_when_updating_newer_cluster_props = f_use_cluster_props_2_for_old_cluster ? cluster_id_1 : cluster_id_2;

      F360_Cluster_T& cluster_newer = clusters[cluster_newer_idx];
      F360_Cluster_T& cluster_older = clusters[cluster_older_idx];

      // Dealias detections with the specific range-rate interval width.
      cluster_rdotcomp_older = Dealias_Range_Rates_In_A_Cluster(cluster_older, sensors, raw_detection_list, detection_props, det_hist, rngrate_interval_width_older, interval_older, 0.0F, 0);
      cluster_rdotcomp_newer = Dealias_Range_Rates_In_A_Cluster(cluster_newer, sensors, raw_detection_list, detection_props, det_hist, rngrate_interval_width_newer, interval_newer, 0.0F, 0);

      const float32_t avg_rep_rrcomp = (cluster_rdotcomp_newer + cluster_rdotcomp_older) * 0.5F;

      // Attempt dealiasing of detections with different range-rate intervals
      Dealias_Detections_To_Target_Range_Rate(cluster_older, sensors, raw_detection_list, detection_props, det_hist, avg_rep_rrcomp, k_max_dealiased_range_rate_diff);
      Dealias_Detections_To_Target_Range_Rate(cluster_newer, sensors, raw_detection_list, detection_props, det_hist, avg_rep_rrcomp, k_max_dealiased_range_rate_diff);

      Collect_Dealiased_Dets_In_Older_Cluster(tracker_info, cluster_older, cluster_newer, detection_props, raw_detection_list, sensors, det_hist);

      /* Shuffle some cluster information around between older and newer clusters. This is done such that we can maintian cluster id over time when we merge older an newer clusters.
      * The information previously contained in the old cluster will be moved to the new cluster. (We need to preserve the info from the older cluster in case some old detections was not dealiased and remains in it.)
      * Older cluster will be updated with new information since it has been updated with new detections.
      */
      const float32_t vcs_x_position_cluster_older = clusters[id_to_use_when_updating_older_cluster_props - 1].vcs_position_x;
      const float32_t vcs_x_position_cluster_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].vcs_position_x;
      const float32_t vcs_y_position_cluster_older = clusters[id_to_use_when_updating_older_cluster_props - 1].vcs_position_y;
      const float32_t vcs_y_position_cluster_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].vcs_position_y;
      const float32_t rep_vcs_az_cluster_older = clusters[id_to_use_when_updating_older_cluster_props - 1].rep_vcs_az;
      const float32_t rep_vcs_az_cluster_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].rep_vcs_az;
      const float32_t cos_vcs_az_cluster_older = clusters[id_to_use_when_updating_older_cluster_props - 1].cos_vcs_az;
      const float32_t cos_vcs_az_cluster_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].cos_vcs_az;
      const float32_t sin_vcs_az_cluster_older = clusters[id_to_use_when_updating_older_cluster_props - 1].sin_vcs_az;
      const float32_t sin_vcs_az_cluster_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].sin_vcs_az;
      const float32_t rep_rdotcomp_cluster_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].rep_rdotcomp;
      cluster_older.vcs_position_x = vcs_x_position_cluster_older;
      cluster_older.vcs_position_y = vcs_y_position_cluster_older;
      cluster_older.rep_vcs_az = rep_vcs_az_cluster_older;
      cluster_older.cos_vcs_az = cos_vcs_az_cluster_older;
      cluster_older.sin_vcs_az = sin_vcs_az_cluster_older;
      cluster_older.rep_rdotcomp = avg_rep_rrcomp;
      cluster_newer.vcs_position_x = vcs_x_position_cluster_newer;
      cluster_newer.vcs_position_y = vcs_y_position_cluster_newer;
      cluster_newer.rep_vcs_az = rep_vcs_az_cluster_newer;
      cluster_newer.cos_vcs_az = cos_vcs_az_cluster_newer;
      cluster_newer.sin_vcs_az = sin_vcs_az_cluster_newer;
      cluster_newer.rep_rdotcomp = rep_rdotcomp_cluster_newer;

      Update_Cluster_Det_Types(cluster_older, detection_props, det_hist);
      Update_Cluster_Det_Types(cluster_newer, detection_props, det_hist);

      cluster_older.f_dealiased = true;
      cluster_newer.f_dealiased = false;

      const float32_t time_since_measurement_older = clusters[id_to_use_when_updating_older_cluster_props - 1].time_since_measurement;
      const float32_t time_since_cluster_updated_older = clusters[id_to_use_when_updating_older_cluster_props - 1].time_since_cluster_updated;
      const float32_t time_since_measurement_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].time_since_measurement;
      const float32_t time_since_cluster_updated_newer = clusters[id_to_use_when_updating_newer_cluster_props - 1].time_since_cluster_updated;
      cluster_older.time_since_cluster_updated = time_since_cluster_updated_older;
      cluster_older.time_since_measurement = time_since_measurement_older;
      cluster_newer.time_since_cluster_updated = time_since_cluster_updated_newer;
      cluster_newer.time_since_measurement = time_since_measurement_newer;

      const bool f_need_to_kill_id_newer = (0 == (cluster_newer.ndets + cluster_newer.num_old_dets)); // If new cluster has no detections we want to kill it
      if (f_need_to_kill_id_newer)
      {
         // kill the older track in caller function
         cluster_newer.f_to_be_killed = true;
      }

      const uint8_t low_rcs_dets_cnt_newer = clusters[id_to_use_when_updating_older_cluster_props - 1].low_rcs_dets_cnt;
      Manage_Low_RCS_Dets_Counters(cluster_newer, cluster_older);
      cluster_newer.low_rcs_dets_cnt = low_rcs_dets_cnt_newer;

      const int16_t clutter_counter_newer = clusters[id_to_use_when_updating_older_cluster_props - 1].clutter_counter;
      cluster_older.clutter_counter = std::max(cluster_newer.clutter_counter, cluster_older.clutter_counter);
      cluster_newer.clutter_counter = clutter_counter_newer;
   }

   static float32_t Dealias_Range_Rates_In_A_Cluster(
      const F360_Cluster_T & cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      const float32_t dealiasing_interval_width,
      const float32_t dealiasing_interval,
      float32_t cluster_rdotcomp,
      int32_t n_rdot_ests)
   {

      if (!cluster.f_dealiased)
      {
         // correct range rate for look types confirmed to be dealiased
         for (int16_t i = 0; i < cluster.ndets; i++)
         {
            const int16_t det_idx = cluster.detids[i] - 1;
            const int32_t sens_idx = raw_detection_list.detections[det_idx].raw.sensor_id - 1;
            const F360_Det_Look_ID_T look_id = sensors[sens_idx].variable.look_id;
            const float32_t range_rate_interval_width = sensors[sens_idx].constant.v_wrapping[look_id];

            if (F360_EPSILON > std::abs(dealiasing_interval_width - range_rate_interval_width))
            {
               detection_props[det_idx].range_rate_compensated = detection_props[det_idx].range_rate_compensated + (dealiasing_interval_width * dealiasing_interval);
               detection_props[det_idx].range_rate_dealiased = detection_props[det_idx].range_rate_dealiased + (dealiasing_interval_width * dealiasing_interval);
               F360_Compensate_Det_Range(dealiasing_interval,
                                         sensors[sens_idx].constant.r_wrapping[look_id],
                                         raw_detection_list.detections[det_idx],
                                         detection_props[det_idx]);
               detection_props[det_idx].f_dealiased = true;
               cluster_rdotcomp += detection_props[det_idx].range_rate_compensated;
               n_rdot_ests += 1;
            }
         }

         for (int16_t i = 0; i < cluster.num_old_dets; i++)
         {
            const int16_t det_idx = cluster.old_det_idx[i];
            if (F360_EPSILON > std::abs(dealiasing_interval_width - det_hist.det_data[det_idx].v_wrapping))
            {
               det_hist.det_data[det_idx].rdot = det_hist.det_data[det_idx].rdot + (dealiasing_interval_width * dealiasing_interval);
               det_hist.det_data[det_idx].rdot_comp = det_hist.det_data[det_idx].rdot_comp + (dealiasing_interval_width * dealiasing_interval);

               F360_Compensate_Detection_Hist_Range(dealiasing_interval, det_hist.det_data[det_idx]);
               det_hist.det_data[det_idx].f_dealiased = true;
               cluster_rdotcomp += det_hist.det_data[det_idx].rdot_comp;
               n_rdot_ests += 1;
            }
         }
      }

      return ((n_rdot_ests > 0) ? (cluster_rdotcomp/ static_cast<float32_t>(n_rdot_ests)) : cluster.rep_rdotcomp);
   }

   /*===========================================================================*\
   * FUNCTION: Collect_Dealiased_Dets_In_Older_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info
   * F360_Cluster_T& cluster_older
   * F360_Cluster_T& cluster_newer
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS]
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Detection_Hist_T& det_hist
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
   * This function manages the process of collecting and redistributing dealiased
   * detections from two clusters (older and newer) into the older cluster. It performs
   * the following main tasks:
   *
   * 1. Determines where detections from both clusters should go based on their dealiasing status.
   * 2. Downselects detections if the number exceeds the maximum allowed per cluster.
   * 3. Updates the newer and older clusters with the redistributed detections.
   *
   * The function uses several helper functions to accomplish these tasks:
   * - Determine_Where_Detections_Go: Sorts detections into new/old and dealiased/non-dealiased categories.
   * - Downselect_Detections: Reduces the number of detections if it exceeds the maximum allowed.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static void Collect_Dealiased_Dets_In_Older_Cluster(
      const F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T& cluster_older,
      F360_Cluster_T& cluster_newer,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Hist_T& det_hist)
   {
      Cluster_Dets dets_in_old_cluster{};
      Cluster_Dets dets_in_new_cluster{};

      // Determine where detections from newest cluster go (it can go either to new or old cluster)
      Determine_Where_Detections_Go(
         cluster_newer,
         sensors,
         raw_detection_list,
         detection_props,
         det_hist,
         dets_in_old_cluster,
         dets_in_new_cluster);

      // Determine where detections from older cluster go (it can go either to new or old cluster)
      Determine_Where_Detections_Go(
         cluster_older,
         sensors,
         raw_detection_list,
         detection_props,
         det_hist,
         dets_in_old_cluster,
         dets_in_new_cluster);

      const uint16_t max_num_new_dets_to_downselect = std::min(static_cast<uint16_t>(tracker_info.variant.num_dets_in_track), WORST_CASE_NUM_DETS_IN_CLUSTER);
      const uint16_t max_num_old_dets_to_downselect = std::min(static_cast<uint16_t>(tracker_info.variant.num_hist_dets_in_cluster), WORST_CASE_NUM_DETS_IN_CLUSTER);

      // Downselection of new detections in new cluster (in case too many)
      Downselect_Detections(max_num_new_dets_to_downselect, dets_in_new_cluster.new_dets);

      // Downselection of old detections in new cluster (in case too many)
      Downselect_Detections(max_num_old_dets_to_downselect, dets_in_new_cluster.hist_dets);

      // Downselection of new detections in old cluster (in case too many)
      Downselect_Detections(max_num_new_dets_to_downselect, dets_in_old_cluster.new_dets);

      // Downselection of old detections in old cluster (in case too many)
      Downselect_Detections(max_num_old_dets_to_downselect, dets_in_old_cluster.hist_dets);

      // Update clusters with the new detection associations
      Update_Cluster_Dets_Associations(dets_in_new_cluster, cluster_newer, detection_props, det_hist);
      Update_Cluster_Dets_Associations(dets_in_old_cluster, cluster_older, detection_props, det_hist);
   }

   /*===========================================================================*\
   * FUNCTION: Update_Cluster_Dets_Associations()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const Cluster_Dets& cluster_detections,
   * F360_Cluster_T& cluster,
   * F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Detection_Hist_T& det_hist
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
   * This function copies the data from array of downselected ids to cluster and updates det props and det hist
   * accordingly.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static void Update_Cluster_Dets_Associations(
      const Cluster_Dets& cluster_detections,
      F360_Cluster_T& cluster,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T& det_hist)
   {
      // Update association for new detections
      cluster.ndets = 0;
      std::fill(cmn::begin(cluster.detids), cmn::end(cluster.detids), static_cast<int16_t>(0));
      for (uint16_t det_i = 0U; det_i < cluster_detections.new_dets.num_dets; det_i++)
      {
         const int16_t detection_idx = cluster_detections.new_dets.det_indexes[det_i];
         if (cluster_detections.new_dets.f_downselected[det_i])
         {
            // Detection is downselected - associate it to the cluster
            cluster.detids[cluster.ndets] = detection_idx + 1;
            det_props[detection_idx].cluster_id = cluster.id;
            cluster.ndets++;
         }
         else
         {
            // Detection is not downselected - deassociate it from clusters
            det_props[detection_idx].cluster_id = 0;
         }
      }

      // Update association for historical detections
      cluster.num_old_dets = 0;
      std::fill(cmn::begin(cluster.old_det_idx), cmn::end(cluster.old_det_idx), static_cast<int16_t>(0));
      for (uint16_t det_i = 0U; det_i < cluster_detections.hist_dets.num_dets; det_i++)
      {
         const int16_t detection_idx = cluster_detections.hist_dets.det_indexes[det_i];
         if (cluster_detections.hist_dets.f_downselected[det_i])
         {
            // Detection is downselected - associate it to the cluster
            cluster.old_det_idx[cluster.num_old_dets] = detection_idx;
            det_hist.det_data[detection_idx].cluster_idx = cluster.id - 1;
            cluster.num_old_dets++;
         }
         else
         {
            // Detection is not downselected - deassociate it from clusters and remove frm historical detection buffer
            det_hist.f_idx_occupied[detection_idx] = false;
            det_hist.det_data[detection_idx] = {};
            det_hist.n_occupied--;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Dealias_Detections_To_Target_Range_Rate()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const F360_Cluster_T & cluster,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * F360_Detection_Hist_T & det_hist,
   * const float32_t target_range_rate,
   * const float32_t k_max_dealiased_range_rate_diff
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
   * This function check whether detection should be dealiased based
   * on this detection range rate, target range rate and range rate interval width.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static void Dealias_Detections_To_Target_Range_Rate(
      const F360_Cluster_T & cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Detection_Hist_T & det_hist,
      const float32_t target_range_rate,
      const float32_t k_max_dealiased_range_rate_diff)
   {

      float32_t rdot_diff;
      float32_t dealiased_rdot_diff;
      float32_t n_alias_intervals;

      // correct range rate for look types confirmed to be dealiased
      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const int32_t sens_idx = raw_detection_list.detections[det_idx].raw.sensor_id - 1;
         const F360_Det_Look_ID_T look_id = sensors[sens_idx].variable.look_id;
         const float32_t range_rate_interval_width = sensors[sens_idx].constant.v_wrapping[look_id];

         if ((!detection_props[det_idx].f_dealiased) && (range_rate_interval_width > 0.0F))
         {
            rdot_diff = target_range_rate - detection_props[det_idx].range_rate_compensated;
            n_alias_intervals = F360_Roundf(rdot_diff / range_rate_interval_width);

            dealiased_rdot_diff = rdot_diff - n_alias_intervals * range_rate_interval_width;
            if (std::abs(dealiased_rdot_diff) < k_max_dealiased_range_rate_diff)
            {
               detection_props[det_idx].range_rate_compensated += n_alias_intervals * range_rate_interval_width;
               detection_props[det_idx].range_rate_dealiased += n_alias_intervals * range_rate_interval_width;
               F360_Compensate_Det_Range(n_alias_intervals,
                                         sensors[sens_idx].constant.r_wrapping[look_id],
                                         raw_detection_list.detections[det_idx],
                                         detection_props[det_idx]);
               detection_props[det_idx].f_dealiased = true;
            }
         }
      }

      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         if ((!det_hist.det_data[det_idx].f_dealiased) && (det_hist.det_data[det_idx].v_wrapping > 0.0F))
         {
            rdot_diff = target_range_rate - det_hist.det_data[det_idx].rdot_comp;
            n_alias_intervals = F360_Roundf(rdot_diff / det_hist.det_data[det_idx].v_wrapping);

            dealiased_rdot_diff = rdot_diff - n_alias_intervals * det_hist.det_data[det_idx].v_wrapping;
            if (std::abs(dealiased_rdot_diff) < k_max_dealiased_range_rate_diff)
            {
               det_hist.det_data[det_idx].rdot_comp += n_alias_intervals * det_hist.det_data[det_idx].v_wrapping;
               det_hist.det_data[det_idx].rdot += n_alias_intervals * det_hist.det_data[det_idx].v_wrapping;
               F360_Compensate_Detection_Hist_Range(n_alias_intervals, det_hist.det_data[det_idx]);
               det_hist.det_data[det_idx].f_dealiased = true;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Update_Cluster_Det_Types()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Cluster_T& cluster
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Detection_Hist_T & det_hist
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
   * This function updates detection motion status base on dealiased range rate.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static void Update_Cluster_Det_Types(
      F360_Cluster_T& cluster,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T & det_hist)
   {
      std::fill(cmn::begin(cluster.num_types_of_dets), cmn::end(cluster.num_types_of_dets), static_cast<int16_t>(0));

      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         if (det_props[det_idx].motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING)
         {
            cluster.num_types_of_dets[0]++;
         }
         else
         {
            cluster.num_types_of_dets[1]++;
         }
      }
      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         if (det_hist.det_data[det_idx].motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING)
         {
            cluster.num_types_of_dets[0]++;
         }
         else
         {
            cluster.num_types_of_dets[1]++;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Manage_Low_RCS_Dets_Counters()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * F360_Cluster_T& cluster_newer,
   * F360_Cluster_T& cluster_older
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
   * This function updates the low_rcs_dets_cnt of the clusters that are being merged.
   * The for the newest/youngest cluster the counter value from the old cluster is simply copied.
   * For the older cluster the counter there are two different  cases
   *    1:  There is only one new detection present in total for the both clusters and it has been placed in the old cluster + both counters are larger than 0:
   *           - The old counter value is set to the sum of both counters
   *    2:  Otherwise:
   *           - Counter is reset to 0.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static void Manage_Low_RCS_Dets_Counters(
      const F360_Cluster_T& cluster_newer,
      F360_Cluster_T& cluster_older)
   {
      /* The newer cluster should not have any detections associated from the current time,
      and the older one should have exactly one. The result is always stored in the older cluster. */
      if ((cluster_older.low_rcs_dets_cnt > 0U) && (cluster_newer.low_rcs_dets_cnt > 0U)
         && (0 == cluster_newer.ndets) && (1 == cluster_older.ndets))
      {
         cluster_older.low_rcs_dets_cnt += cluster_newer.low_rcs_dets_cnt;
      }
      else
      {
         cluster_older.low_rcs_dets_cnt = 0U;
      }
   }


   /*===========================================================================*\
   * FUNCTION: F360_Compensate_Detection_Hist_Range()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const float32_t dealiasing_interval,
   * F360_Detection_Hist_Data_T &det_data
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
   * This function updates detection hist's vcs position after
   * range rate dealiasing (SFW)
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static inline void F360_Compensate_Detection_Hist_Range(
      const float32_t dealiasing_interval,
      F360_Detection_Hist_Data_T &det_data)
   {
      const float32_t delta_range = det_data.r_wrapping * dealiasing_interval;
      det_data.vcs_position_x += delta_range * F360_Cosf(det_data.vcs_az);
      det_data.vcs_position_y += delta_range * F360_Sinf(det_data.vcs_az);
   }

 /*===========================================================================*\
   * FUNCTION: Determine_Where_Detections_Go()
   *===========================================================================
   *
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Cluster_T& cluster,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Detection_Hist_T& det_hist,
   * Cluster_Dets& old_cluster_dets,
   * Cluster_Dets& new_cluster_dets
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   *
   * ABSRTACT:
   *
   * This function processes detections associated with a cluster and arranges them in new and
   * old cluster detection sets.
   * Arrangement is done based on their dealiasing status.
   * Dealiased detections are put into the older cluster and non-dealiased detections are put into the newer cluster.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   static void Determine_Where_Detections_Go(
      const F360_Cluster_T& cluster,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Hist_T& det_hist,
      Cluster_Dets& old_cluster_dets,
      Cluster_Dets& new_cluster_dets)
   {
      // Determine where detections go for newer detections
      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         if (detection_props[det_idx].f_dealiased)
         {
            // Detection goes to oldest cluster
            old_cluster_dets.new_dets.det_indexes[old_cluster_dets.new_dets.num_dets] = det_idx;
            old_cluster_dets.new_dets.time_since_meas[old_cluster_dets.new_dets.num_dets] = sensors[raw_detection_list.detections[det_idx].raw.sensor_id - 1].refined.time_since_measurement_s;
            old_cluster_dets.new_dets.f_historic[old_cluster_dets.new_dets.num_dets] = false;
            old_cluster_dets.new_dets.f_downselected[old_cluster_dets.new_dets.num_dets] = false;
            old_cluster_dets.new_dets.num_dets++;
         }
         else
         {
            // Detection goes to youngest cluster
            new_cluster_dets.new_dets.det_indexes[new_cluster_dets.new_dets.num_dets] = det_idx;
            new_cluster_dets.new_dets.time_since_meas[new_cluster_dets.new_dets.num_dets] = sensors[raw_detection_list.detections[det_idx].raw.sensor_id - 1].refined.time_since_measurement_s;
            new_cluster_dets.new_dets.f_historic[new_cluster_dets.new_dets.num_dets] = false;
            new_cluster_dets.new_dets.f_downselected[new_cluster_dets.new_dets.num_dets] = false;
            new_cluster_dets.new_dets.num_dets++;
         }
      }
      // Determine where detections go for older detections
      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         if (det_hist.det_data[det_idx].f_dealiased)
         {
            // Detection goes to oldest cluster
            old_cluster_dets.hist_dets.det_indexes[old_cluster_dets.hist_dets.num_dets] = det_idx;
            old_cluster_dets.hist_dets.time_since_meas[old_cluster_dets.hist_dets.num_dets] = det_hist.det_data[det_idx].time_since_meas;
            old_cluster_dets.hist_dets.f_historic[old_cluster_dets.hist_dets.num_dets] = true;
            old_cluster_dets.hist_dets.f_downselected[old_cluster_dets.hist_dets.num_dets] = false;
            old_cluster_dets.hist_dets.num_dets++;
         }
         else
         {
            // Detection goes to newest cluster
            new_cluster_dets.hist_dets.det_indexes[new_cluster_dets.hist_dets.num_dets] = det_idx;
            new_cluster_dets.hist_dets.time_since_meas[new_cluster_dets.hist_dets.num_dets] = det_hist.det_data[det_idx].time_since_meas;
            new_cluster_dets.hist_dets.f_historic[new_cluster_dets.hist_dets.num_dets] = true;
            new_cluster_dets.hist_dets.f_downselected[new_cluster_dets.hist_dets.num_dets] = false;
            new_cluster_dets.hist_dets.num_dets++;
         }
      }
   }


   /*===========================================================================*\
   * FUNCTION: Find_Largest_Detection_Time_Since_Meas()
   *===========================================================================
   *
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Cluster_T& cluster,
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Detection_Hist_T& det_hist
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   *
   * ABSRTACT:
   *
   * This function finds the oldest timestamp of all historical and new detections in a cluster.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static float32_t Find_Largest_Detection_Time_Since_Meas(
      const F360_Cluster_T& cluster,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Hist_T& det_hist)
   {
      float32_t largest_time_since_meas = -1.0F; // Initialize to something negative to make sure at least one detection resets this value

      // Loop through current detections
      for (int16_t det_i = 0; det_i < cluster.ndets; det_i++)
      {
         const int16_t det_idx = cluster.detids[det_i] - 1;
         const int32_t sensor_idx = raw_detection_list.detections[det_idx].raw.sensor_id - 1;
         const float32_t curr_time_since_meas = sensors[sensor_idx].refined.time_since_measurement_s;
         if (curr_time_since_meas > largest_time_since_meas)
         {
            largest_time_since_meas = curr_time_since_meas;
         }
      }

      // Loop through old detections
      for (int16_t det_i = 0; det_i < cluster.num_old_dets; det_i++)
      {
         const int16_t det_idx = cluster.old_det_idx[det_i];
         const float32_t curr_time_since_meas = det_hist.det_data[det_idx].time_since_meas;
         if (curr_time_since_meas > largest_time_since_meas)
         {
            largest_time_since_meas = curr_time_since_meas;
         }
      }

      return largest_time_since_meas;
   }
}
