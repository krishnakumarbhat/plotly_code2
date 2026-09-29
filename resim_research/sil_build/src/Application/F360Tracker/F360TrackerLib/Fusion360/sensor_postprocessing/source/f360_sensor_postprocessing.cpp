/*===================================================================================*\
* FILE: f360_sensor_postprocessing.cpp
*====================================================================================
* Copyright 2018 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contain definitions for senor post-processing main function and some support functions
*
* ABBREVIATIONS:
*   OTG   Over-The-ground
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/


/******************************
* Includes
*******************************/

#include <algorithm>
#include "f360_reuse.h"
#include "f360_iterator.h"
#include "f360_constants.h"
#include "f360_sensor_postprocessing.h"
#include "f360_kill_cluster.h"
#include "f360_get_wall_time.h"
#include "f360_norm_heading_angle.h"
#include "f360_cluster_detection_downselection.h"
#include "f360_calculate_priority.h"
#include "f360_math_func.h"

namespace f360_variant_A
{

static void Update_Detection_History(
      const F360_Host_T& host_props,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Hist_T& detection_hist,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      const F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info);

static void Update_Single_Detection_Hist(
      const rspp_variant_A::RSPP_Detection_T& raw_detection,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T& det_props,
      const int16_t cluster_idx,
      F360_Detection_Hist_Data_T& det_to_update);

static bool Find_Next_Hist_Det_Idx(
   const int16_t max_tot_num_of_hist_dets,
   int16_t& hist_det_idx,
   F360_Detection_Hist_T& detection_hist,
   F360_Cluster_T& cluster);

static void Fill_Cluster_Combined_Detection_Struct(
   const F360_Cluster_T& cluster,
   const F360_Detection_Hist_T& detection_hist,
   const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   Detections_Set& combined_dets);

   static void Remove_Old_Dets_From_Clusters(
      const F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T& det_hist
   );

   static bool Is_Det_Valid_To_Keep(
      const bool f_cluster_moving_fast,
      const F360_Detection_Hist_Data_T& det_hist
   );

   static void Mark_Long_Coasting_Clusters_To_Be_Killed(
      const F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
   );

   static int16_t Compute_Cluster_Priority_And_Sort(
      const int16_t(&cluster_id_array)[NUMBER_OF_CLUSTERS],
      const int16_t num_clusters,
      const F360_Host_T& host_props,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      uint32_t(&sorted_id_permutation)[NUMBER_OF_CLUSTERS]
   );

   static void Count_Number_Of_Clusters_To_Sustain(
      const int16_t max_tot_num_of_hist_dets,
      const int16_t max_hist_dets_in_single_cluster,
      const int16_t(&cluster_id_array)[NUMBER_OF_CLUSTERS],
      const int16_t num_valid_clusters,
      const uint32_t(&prio_sorted_id_perm)[NUMBER_OF_CLUSTERS],
      const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      int16_t& num_sustained_clusters,
      int16_t& num_dets_in_least_prio_sustained_cluster
   );

   static void Remove_Excess_Of_Dets_From_Cluster(
      const int16_t max_hist_dets,
      F360_Cluster_T& cluster,
      F360_Detection_Hist_T& detection_hist
   );

   static void Remove_All_Hist_Dets_From_Cluster(
      F360_Cluster_T& cluster,
      F360_Detection_Hist_T& detection_hist
   );

   static void Move_Cluster_New_Dets_To_Hist_Det_Structure(
      const int16_t max_tot_num_of_hist_dets,
      const int16_t max_hist_dets_in_single_cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Cluster_T& cluster,
      F360_Detection_Hist_T& detection_hist
   );

   /*===========================================================================*\
   * FUNCTION: Sensor_Postprocessing()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T& host_props,
   * F360_Tracker_Info_T* const tracker_info,
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Detection_Hist_T & detection_hist,
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
   * F360_TRKR_TIMING_INFO_T &timing_info
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
   * This function is the main function for performing sensor post-processing
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Sensor_Postprocessing(
      const F360_Host_T& host_props,
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_Detection_Hist_T& detection_hist,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      const float32_t start_time = get_wall_time();

      Mark_Long_Coasting_Clusters_To_Be_Killed(tracker_info, clusters);
      Remove_Old_Dets_From_Clusters(tracker_info, clusters, detection_hist);

      // Update historical detection buffer. Move new detections to hist det buffer and remove hist dets that are no longer important enough to keep
      Update_Detection_History(host_props, det_props, raw_detection_list, sensors, detection_hist, clusters, tracker_info, timing_info);

      timing_info.sensor_postprocessing = get_wall_time() - start_time;
   }

   /*===========================================================================*\
   * FUNCTION: Update_Detection_History()
   *===========================================================================
   *
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Host_T & host_props,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * F360_Detection_Hist_T& detection_hist
   * F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
   * const F360_Tracker_Info_T& tracker_info
   * F360_TRKR_TIMING_INFO_T & timing_info
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
   * This function is responsible for updating the detection history and performing various
   * sensor post-processing tasks. It processes the raw detection list, updates the detection
   * history, and modifies cluster information based on the current detections and tracker info.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static void Update_Detection_History(
      const F360_Host_T& host_props,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Hist_T& detection_hist,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      const F360_Tracker_Info_T& tracker_info,
      F360_TRKR_TIMING_INFO_T & timing_info)
   {
      const float32_t start_time = get_wall_time();

      /* The historical detection buffer is of a fixed limited size and it might not be
      * possible to move all new detections for all clusters into it. Therefore three things
      * will be done in this function:
      * 1. Find the most prioritized clusters that will be allowed to occupy slots in the
      *    historical detection data structure.
      * 2. For clusters that are not prioritized enough:
      *    Remove historical detections and mark them as to be killed.
      *    (To enable space for the for more prioritized clusters)
      * 3. For clusters that are of high enough prio;
      *    Move new detections to the historical detection buffer */

      /* Start of step 1: Find the most prioritized clusters that will be allowed to occupy slots in the historical detection data structure. */
      
      // Compute cluster prio and sort it from most to least prioritized
      uint32_t prio_perm[NUMBER_OF_CLUSTERS];
      const int16_t num_valid_clusters = Compute_Cluster_Priority_And_Sort(tracker_info.active_cluster_ids, tracker_info.num_active_clusters, host_props, clusters, prio_perm);

      /* Computes how many clusters that can be sustained by the hist det buffer given that
      * the highest priority clusters are allowed to utilize as many slots in the hist det
      * buffer as needed up to the maximum limit of max_hist_dets_in_single_cluster. */
      int16_t num_dets_in_least_prio_sustained_cluster = 0;
      int16_t num_sustained_clusters = 0;
      Count_Number_Of_Clusters_To_Sustain(static_cast<int16_t>(tracker_info.variant.num_hist_dets), static_cast<int16_t>(tracker_info.variant.num_hist_dets_in_cluster),
         tracker_info.active_cluster_ids, num_valid_clusters, prio_perm, clusters,
         num_sustained_clusters, num_dets_in_least_prio_sustained_cluster);


      /* Start of step 2: For clusters that are not prioritized enough; Remove historical detections and mark them as to be killed. (To enable space for the for more prioritized clusters).
      * Note: This step has to be done before step 3 (moving new dets to hist det buffer) to make room for all new detections of high prio clusters */

      /* For the least prioritized cluster that can be sustained it is not guaranteed
      * that all its detections can fit in in the historical detection buffer.
      * Remove any excess of historical detections for this cluster that can not fit.
      * Remove the oldest detections and keep the youngest. */
      if (num_sustained_clusters > 0)
      {
         const uint32_t cluster_i = prio_perm[num_sustained_clusters - 1];
         const int16_t least_prio_sustained_cluster_idx = tracker_info.active_cluster_ids[cluster_i] - 1;
         Remove_Excess_Of_Dets_From_Cluster(num_dets_in_least_prio_sustained_cluster, clusters[least_prio_sustained_cluster_idx], detection_hist);
      }

      // For the lower prio clusters for which there are no hist det slots available; Clear all hist dets and mark the clusters as to be killed
      for (int16_t sorted_cluster_i = num_sustained_clusters; sorted_cluster_i < tracker_info.num_active_clusters; sorted_cluster_i++)
      {
         const uint32_t raw_cluster_i = prio_perm[sorted_cluster_i];
         const int16_t cluster_idx = tracker_info.active_cluster_ids[raw_cluster_i] - 1;
         Remove_All_Hist_Dets_From_Cluster(clusters[cluster_idx], detection_hist);
      }


      /* Start of step 3: Move new detections to the historical detection buffer for all clusters that are of high enough prio.
      * In case the total number of detections for the cluster is larger than the number of hist det slots that the
      * cluster is allowed to occupy we need to downselect which detections we want to store. */
      const int16_t max_total_num_hist_dets = static_cast<int16_t>(tracker_info.variant.num_hist_dets); // To avoid having to do the casting multiple times inside the for loop below
      const int16_t max_hist_dets_in_single_cluster = static_cast<int16_t>(tracker_info.variant.num_hist_dets_in_cluster); // To avoid having to do the casting multiple times inside the for loop below
      for (int16_t sorted_cluster_i = 0; sorted_cluster_i < num_sustained_clusters; sorted_cluster_i++)
      {
         const uint32_t raw_cluster_i = prio_perm[sorted_cluster_i];
         const int16_t cluster_idx = tracker_info.active_cluster_ids[raw_cluster_i] - 1;

         Move_Cluster_New_Dets_To_Hist_Det_Structure(max_total_num_hist_dets, max_hist_dets_in_single_cluster,
            det_props, raw_detection_list, sensors,
            clusters[cluster_idx], detection_hist);
         
         if (clusters[cluster_idx].num_old_dets == 0)
         {
            /* Mark cluster to be killed early in next tracker iteration because it has no detections
            * (We don't want to kill it now becasue we want to log it for debug purposes) */
            clusters[cluster_idx].f_to_be_killed = true;
         }
      }

      // Clean up information from previous tracker loop
      for (int16_t cluster_i = 0; cluster_i < tracker_info.num_active_clusters; cluster_i++)
      {
         const int16_t cluster_idx = tracker_info.active_cluster_ids[cluster_i] - 1;
         clusters[cluster_idx].ndets = 0;
         std::fill(cmn::begin(clusters[cluster_idx].detids), cmn::end(clusters[cluster_idx].detids), static_cast<int16_t>(0));
      }
      timing_info.update_det_hist = get_wall_time() - start_time;
   }


   /*===========================================================================*\
    * FUNCTION: Update_Single_Detection_Hist()
    *===========================================================================
    *
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const rspp_variant_A::RSPP_Detection_T& raw_detection,
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
    * const F360_Detection_Props_T& det_props,
    * const int16_t cluster_idx,
    * F360_Detection_Hist_Data_T& det_to_update
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
    * This function saves current detections into historical detections struct.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   static void Update_Single_Detection_Hist(
      const rspp_variant_A::RSPP_Detection_T& raw_detection,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Detection_Props_T& det_props,
      const int16_t cluster_idx,
      F360_Detection_Hist_Data_T& det_to_update)
   {
      const int32_t sensor_idx = raw_detection.raw.sensor_id - 1;
      const F360_Det_Look_ID_T look_id = sensors[sensor_idx].variable.look_id;
      float min_range = fminf(sensors[sensor_idx].constant.range_limits[0], sensors[sensor_idx].constant.range_limits[1]);
      min_range = fminf(min_range, sensors[sensor_idx].constant.range_limits[2]);
      min_range = fminf(min_range, sensors[sensor_idx].constant.range_limits[3]);
      const bool f_range_in_all_looks = (raw_detection.raw.range < min_range);
      det_to_update.vcs_position_x = det_props.vcs_position.x;
      det_to_update.vcs_position_y = det_props.vcs_position.y;
      det_to_update.vcs_position_z = raw_detection.processed.vcs_position_z;
      det_to_update.rdot = det_props.range_rate_dealiased;
      det_to_update.rdot_comp = det_props.range_rate_compensated;
      det_to_update.vcs_az = raw_detection.processed.vcs_az;
      det_to_update.time_since_meas = sensors[sensor_idx].refined.time_since_measurement_s;
      det_to_update.v_wrapping = sensors[sensor_idx].constant.v_wrapping[look_id];
      det_to_update.r_wrapping = sensors[sensor_idx].constant.r_wrapping[look_id];
      det_to_update.elevation = raw_detection.raw.elevation;
      det_to_update.motion_status = det_props.motion_status;
      det_to_update.f_dealiased = det_props.f_dealiased;
      det_to_update.f_is_range_in_all_looks = f_range_in_all_looks;
      det_to_update.f_potential_angle_jump = det_props.f_potential_angle_jump;
      det_to_update.f_super_res = raw_detection.raw.f_super_res;
      det_to_update.cluster_idx = cluster_idx;
      det_to_update.wheel_spin_type = det_props.wheel_spin_type;
      det_to_update.sensor_id = static_cast<uint8_t>(raw_detection.raw.sensor_id);
      det_to_update.az_conf = raw_detection.raw.confid_azimuth;
      det_to_update.el_conf = raw_detection.raw.confid_elevation;
   }

   /*===========================================================================*\
    * FUNCTION: Fill_Cluster_All_Detection_Struct()
    *===========================================================================
    *
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * const F360_Cluster_T& cluster,
    * const F360_Detection_Hist_T& detection_hist,
    * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
    * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
    * Detections_Set& combined_dets
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
    * This function copies properties of detections from a cluster to the
    * struct containing both historical and current properties.
    * Properties being copied:
    *    - idxs (hist) and ids(current) of detections
    *    - time since measurement
    *    - number of detections
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   static void Fill_Cluster_Combined_Detection_Struct(
      const F360_Cluster_T& cluster,
      const F360_Detection_Hist_T& detection_hist,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      Detections_Set& combined_dets)
   {

      combined_dets.num_dets = static_cast<uint16_t>(cluster.ndets) + static_cast<uint16_t>(cluster.num_old_dets);

      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         combined_dets.time_since_meas[i] = sensors[raw_detection_list.detections[det_idx].raw.sensor_id - 1].refined.time_since_measurement_s;
         combined_dets.det_indexes[i] = det_idx;
         combined_dets.f_historic[i] = false;
         combined_dets.f_downselected[i] = false;
      }

      for (int16_t i = cluster.ndets; i < static_cast<int16_t>(combined_dets.num_dets); i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i - cluster.ndets];
         combined_dets.time_since_meas[i] = detection_hist.det_data[det_idx].time_since_meas;
         combined_dets.det_indexes[i] = det_idx;
         combined_dets.f_historic[i] = true;
         combined_dets.f_downselected[i] = false;
      }
   }

    /*===========================================================================*\
    * FUNCTION: Find_Next_Hist_Det_Idx()
    *===========================================================================
    *
    * RETURN VALUE:
    * None
    *
    * PARAMETERS:
    * uint16_t& hist_det_idx
    * const F360_Tracker_Info_T& tracker_info
    * F360_Detection_Hist_T& detection_hist
    * F360_Cluster_T& cluster
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
    * This function searches for the next available detection index in the detection history
    * based on the provided cluster index and tracker information. It updates the detection
    * history and cluster information accordingly.
    *
    * PRECONDITIONS:
    * None
    *
    * POSTCONDITIONS:
    * None
    *
    \*===========================================================================*/
   static bool Find_Next_Hist_Det_Idx(
      const int16_t max_tot_num_of_hist_dets,
      int16_t& hist_det_idx,
      F360_Detection_Hist_T& detection_hist,
      F360_Cluster_T& cluster)
   {     
      bool f_slot_found = false;
      for (int16_t k = 0; (k < max_tot_num_of_hist_dets); k++)
      {
         // Find first empty space in detection_hist
         if (!detection_hist.f_idx_occupied[k])
         {
            hist_det_idx = k;
            detection_hist.n_occupied++;
            detection_hist.f_idx_occupied[k] = true;
            detection_hist.max_occupation = detection_hist.max_occupation < detection_hist.n_occupied ? detection_hist.n_occupied : detection_hist.max_occupation;
            cluster.old_det_idx[cluster.num_old_dets] = hist_det_idx;
            cluster.num_old_dets++;
            f_slot_found = true;
            break;
         }
      }
      return f_slot_found;
   }

   /*===========================================================================*\
   * FUNCTION: Remove_Old_Dets_From_Clusters()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T &tracker_info
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS]
   * F360_Detection_Hist_T &det_hist
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function removes old detections from clusters and kills clusters with no associated detection.
   *
   \*===========================================================================*/
   static void Remove_Old_Dets_From_Clusters(
      const F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T& det_hist)
   {
      for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
      {
         const int16_t cluster_idx = tracker_info.active_cluster_ids[i] - 1;
         F360_Cluster_T& cluster = clusters[cluster_idx];

         if (!cluster.f_to_be_killed) // Clusters to be killed has already previously been cleared of all detections so we don't need to check these again
         {
            int16_t new_num_old_dets = 0;

            constexpr float32_t rdotcomp_threshold_for_prolonging_keeping_older_dets = 10.0F;
            const bool f_cluster_moving_fast = std::fabs(cluster.rep_rdotcomp) > rdotcomp_threshold_for_prolonging_keeping_older_dets;

            // Remove old detections from cluster old_det_idx
            for (int16_t j = 0; j < cluster.num_old_dets; j++)
            {
               const int16_t hist_det_idx = cluster.old_det_idx[j];

               if (Is_Det_Valid_To_Keep(f_cluster_moving_fast, det_hist.det_data[hist_det_idx]))
               {
                  // Keep detection
                  cluster.old_det_idx[new_num_old_dets] = hist_det_idx;
                  new_num_old_dets++;
               }
               else
               {
                  // Remove detection
                  if (1 == det_hist.det_data[hist_det_idx].motion_status)
                  {
                     // Moving detection
                     cluster.num_types_of_dets[0]--;
                  }
                  else
                  {
                     // Ambigous detection
                     cluster.num_types_of_dets[1]--;
                  }
                  det_hist.det_data[hist_det_idx] = {};
                  det_hist.f_idx_occupied[hist_det_idx] = false;
                  det_hist.n_occupied--;
               }
            }

            // Clear unused slots of the cluster.old_det_idx array
            for (int16_t j = new_num_old_dets; j < cluster.num_old_dets; j++)
            {
               cluster.old_det_idx[new_num_old_dets] = 0;
            }

            // Set the new number of old historical detections for the cluster
            cluster.num_old_dets = new_num_old_dets;

            if ((cluster.num_old_dets == 0) && (cluster.ndets == 0))
            {
               /* Don't kill the clusters. We still want to log it's existence for debugging purposes.
               * But mark it as a cluster that should be killed in the beginning of next tracker iteration */
               cluster.f_to_be_killed = true;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Det_Valid_To_Keep()
   *===========================================================================
   * RETURN VALUE:
   * bool f_is_det_valid_to_keep
   *
   * PARAMETERS:
   * const bool f_cluster_moving_fast,
   * const F360_Detection_Hist_Data_T& det_his
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function checks if detections are valid to be kept based on its motion status, age and range
   *
   \*===========================================================================*/
   static bool Is_Det_Valid_To_Keep(
      const bool f_cluster_moving_fast,
      const F360_Detection_Hist_Data_T& det_hist)
   {
      constexpr float32_t long_range_threshold_squared = 22500.0F; // 150.0F squared - threshold used particularly in clusters preprocessing
      const float32_t delta_time = det_hist.time_since_meas;
      const float32_t offset_fast_moving = f_cluster_moving_fast ? 0.3F : 0.0F;

      constexpr float32_t k_max_age_of_amb_dets = 0.30F;      // Expected value (0.200F) + const to compensate difference of tracker time and measurement time (~0.150F)
      constexpr float32_t k_max_age_of_nonamb_dets = 0.60F;    // Expected value (0.500F) + const to compensate difference of tracker time and measurement time (~0.150F)
      constexpr float32_t k_max_age_of_long_range_dets = 0.75F;        // Expected value (0.650F) + const to compensate difference of tracker time and measurement time (~0.150F)

      const bool f_is_det_amb_and_not_old = (rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS == det_hist.motion_status) && (delta_time < k_max_age_of_amb_dets);
      const bool f_is_det_not_amb_stat_and_not_old = ((rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS != det_hist.motion_status) && (delta_time < k_max_age_of_nonamb_dets + offset_fast_moving));

      const bool f_long_range_det = ((!det_hist.f_is_range_in_all_looks) || ((det_hist.vcs_position_x * det_hist.vcs_position_x + det_hist.vcs_position_y * det_hist.vcs_position_y) > (long_range_threshold_squared)));
      const bool f_is_det_in_all_looks_and_not_old = f_long_range_det && (delta_time < k_max_age_of_long_range_dets);

      const bool f_is_det_valid_to_keep = (f_is_det_amb_and_not_old || f_is_det_not_amb_stat_and_not_old || f_is_det_in_all_looks_and_not_old);

      return f_is_det_valid_to_keep;
   }

   /*===========================================================================*\
   * FUNCTION: Mark_Long_Coasting_Clusters_To_Be_Killed()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T &tracker_info
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Mark clusters that have been coasting (i.e. not being updated with new detections) for a long time to be killed.
   * Note: We don't want to kill the clusters right now becasue we want to log it for debugging purposes.
   * But we mark clusters to be killed early on in next tracker iteration.
   \*===========================================================================*/
   static void Mark_Long_Coasting_Clusters_To_Be_Killed(
      const F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS])
   {
      for (int16_t i = 0; i < tracker_info.num_active_clusters; i++)
      {
         const int16_t cluster_idx = tracker_info.active_cluster_ids[i] - 1;

         if(!clusters[cluster_idx].f_to_be_killed) // We don't need to check clusters that have already been marked as f_to_be_killed
         {
            const float32_t max_coasting_time = 0.375F; // Give dealiased clusters a maximum of 8 scans (2 rounds of all look indexes) with no associated detections before killing them
            if ((clusters[cluster_idx].ndets == 0) && (clusters[cluster_idx].time_since_cluster_updated > max_coasting_time))
            {
               clusters[cluster_idx].f_to_be_killed = true;
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Cluster_Priority_And_Sort()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const int16_t (&cluster_id_array)[NUMBER_OF_CLUSTERS],
   * const int16_t num_clusters,
   * const F360_Host_T& host_props,
   * F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
   * uint32_t (&sorted_id_permutation)[NUMBER_OF_CLUSTERS]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes cluster priority and sorts them from most important to
   * least important. A permutation array is returned from the function to indicate
   * the sorting order. Clusters marked as f_to_be_killed vill be considered invalid
   * (least prioritized) and the function will return num_valid_clusters to indicate
   * how many clusters to keep alive there are present.
   \*===========================================================================*/
   static int16_t Compute_Cluster_Priority_And_Sort(
      const int16_t (&cluster_id_array)[NUMBER_OF_CLUSTERS],
      const int16_t num_clusters,
      const F360_Host_T& host_props,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      uint32_t (&sorted_id_permutation)[NUMBER_OF_CLUSTERS])
   {
      float32_t priority[NUMBER_OF_CLUSTERS]{};
      for (int16_t cluster_i = 0; cluster_i < num_clusters; cluster_i++)
      {
         // Compute cluster priority
         const int16_t cluster_idx = cluster_id_array[cluster_i] - 1;

         if (clusters[cluster_idx].f_to_be_killed)
         {
            priority[cluster_i] = -1.0F; // Negative priority to indicate that these objects are of least priority
         }
         else
         {
            priority[cluster_i] = Calculate_Priority_For_Cluster(host_props, clusters[cluster_idx]);
         }
      }

      // Sort clusters according to their priority from highest to lowest priority
      (void)F360_Sort(static_cast<uint32_t>(num_clusters), false, priority, sorted_id_permutation);

      int16_t num_valid_clusters = 0;
      for (int16_t cluster_i = 0; cluster_i < num_clusters; cluster_i++)
      {
         if (priority[cluster_i] < 0.0F)
         {
            break;
         }

         num_valid_clusters++;
      }

      return num_valid_clusters;
   }

   /*===========================================================================*\
   * FUNCTION: Count_Number_Of_Clusters_To_Sustain()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const int16_t max_tot_num_of_hist_dets,
   * const int16_t max_hist_dets_in_single_cluster,
   * const int16_t(&cluster_id_array)[NUMBER_OF_CLUSTERS],
   * const int16_t num_valid_clusters,
   * const uint32_t(&prio_sorted_id_perm)[NUMBER_OF_CLUSTERS],
   * const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
   * int16_t & num_sustained_clusters,
   * int16_t & num_dets_in_least_prio_sustained_cluster
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * The historical detection buffer is of a fixed limited size.
   * Therefore it might not be possible to move all new detections for all clusters
   * into the historical detection buffer. This function computes how many clusters
   * that we can sustain given that the highest priority clusters are allowed to
   * utilize as many slots in the hist det buffer as needed up to the maximum limit of
   * max_hist_dets_in_single_cluster. The value num_sustained_clusters is returned to
   * indicate the maximum number clusters that can fit into the historical detection
   * buffer under these conditions.
   * 
   * Note: For the least prioritized cluster that can be sustain it is not guaranteed
   * that max_hist_dets_in_single_cluster detections are availiale in the the historical
   * detection buffer. This function therefore returns the variable
   * num_dets_in_least_prio_sustained_cluster to indicate number of left over slots for
   * the least prioritized sustained cluster.
   * 
   * PRECONDITIONS:
   * The clusters must be pre-sorted according to their priority with most important
   * cluster first. The input array prio_sorted_id_perm is indicating the sorting order.
   \*===========================================================================*/
   static void Count_Number_Of_Clusters_To_Sustain(
      const int16_t max_tot_num_of_hist_dets,
      const int16_t max_hist_dets_in_single_cluster,
      const int16_t(&cluster_id_array)[NUMBER_OF_CLUSTERS],
      const int16_t num_valid_clusters,
      const uint32_t(&prio_sorted_id_perm)[NUMBER_OF_CLUSTERS],
      const F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      int16_t & num_sustained_clusters,
      int16_t & num_dets_in_least_prio_sustained_cluster)
   {
      int16_t tot_num_ds_dets = 0;
      num_dets_in_least_prio_sustained_cluster = 0;
      num_sustained_clusters = 0;
      for (int16_t cluster_i = 0; cluster_i < num_valid_clusters; cluster_i++)
      {
         num_sustained_clusters++;

         const int16_t cluster_idx = cluster_id_array[prio_sorted_id_perm[cluster_i]] - 1;

         const int16_t num_free_dets = max_tot_num_of_hist_dets - tot_num_ds_dets;
         const int16_t max_allowed_num_dets = std::min(num_free_dets, max_hist_dets_in_single_cluster);
         const int16_t num_wanted_dets = clusters[cluster_idx].ndets + clusters[cluster_idx].num_old_dets;
         const int16_t num_ds_dets_for_cluster = std::min(num_wanted_dets, max_allowed_num_dets);

         num_dets_in_least_prio_sustained_cluster = num_ds_dets_for_cluster;

         // Check if we have picked enough clusters or if there is still room for some more
         tot_num_ds_dets += num_ds_dets_for_cluster;
         if (tot_num_ds_dets >= max_tot_num_of_hist_dets)
         {
            // Hist det buffer is full. Break the loop
            break;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Remove_Excess_Of_Dets_From_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const int16_t num_sustained_dets,
   * F360_Cluster_T& cluster,
   * F360_Detection_Hist_T& detection_hist
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This cluster removes an excess of associated historical detections of a cluster
   * given that it is only allowed to have max_hist_dets historical detections
   *
   * PRECONDITIONS:
   * 
   \*===========================================================================*/
   static void Remove_Excess_Of_Dets_From_Cluster(
      const int16_t max_hist_dets,
      F360_Cluster_T& cluster,
      F360_Detection_Hist_T& detection_hist)
   {
      const int16_t safety_max_hist_dets = (max_hist_dets < 0) ? 0 : max_hist_dets; // For code safety, make sure max_hist_dets is not a negative number

      if (safety_max_hist_dets < cluster.num_old_dets)
      {
         /* This cluster is occupying too many hist dets so we need to discard of some of them.
         * Discard of the oldest detections and keep the newest. */
         float32_t det_timestamps[MAX_HIST_DETS_IN_CLUSTER]{};
         int16_t det_indexes[MAX_HIST_DETS_IN_CLUSTER]{};
         for (int16_t det_i = 0; det_i < cluster.num_old_dets; det_i++)
         {
            const int16_t det_idx = cluster.old_det_idx[det_i];
            det_indexes[det_i] = det_idx;
            det_timestamps[det_i] = detection_hist.det_data[det_idx].time_since_meas;
         }
         uint32_t perm[MAX_HIST_DETS_IN_CLUSTER];
         (void)F360_Sort(static_cast<uint32_t>(cluster.num_old_dets), false, det_timestamps, perm);

         for (int16_t sorted_det_i = 0; sorted_det_i < (cluster.num_old_dets - safety_max_hist_dets); sorted_det_i++)
         {
            // Remove detection

            const uint32_t raw_det_i = perm[sorted_det_i];
            const int16_t det_idx = det_indexes[raw_det_i];

            if (1 == detection_hist.det_data[det_idx].motion_status)
            {
               // Moving detection
               cluster.num_types_of_dets[0]--;
            }
            else
            {
               // Ambigous detection
               cluster.num_types_of_dets[1]--;
            }

            detection_hist.f_idx_occupied[det_idx] = false;
            detection_hist.det_data[det_idx] = {};
            detection_hist.n_occupied--;
         }

         // Update array of hist det index of cluster to mirror that some detectins has been removed
         const int16_t num_hist_dets_to_discard = (cluster.num_old_dets - safety_max_hist_dets);
         std::fill(cmn::begin(cluster.old_det_idx), cmn::end(cluster.old_det_idx), static_cast<int16_t>(0));
         const int16_t prev_num_old_dets = cluster.num_old_dets;
         cluster.num_old_dets = 0;
         for (int16_t sorted_det_i = num_hist_dets_to_discard; sorted_det_i < prev_num_old_dets; sorted_det_i++)
         {
            const uint32_t raw_det_i = perm[sorted_det_i];
            const int16_t det_idx = det_indexes[raw_det_i];
            cluster.old_det_idx[cluster.num_old_dets] = det_idx;
            cluster.num_old_dets++;
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Remove_All_Hist_Dets_From_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * F360_Cluster_T& cluster,
   * F360_Detection_Hist_T& detection_hist
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function removes all historical detections from a cluster and markes the
   * cluster as "to be killed".
   *
   * PRECONDITIONS:
   *
   \*===========================================================================*/
   static void Remove_All_Hist_Dets_From_Cluster(
      F360_Cluster_T& cluster,
      F360_Detection_Hist_T& detection_hist)
   {
      for (int16_t det_i = 0; det_i < cluster.num_old_dets; det_i++)
      {
         // Remove hist dets from hist det buffer
         const int16_t det_idx = cluster.old_det_idx[det_i];

         detection_hist.f_idx_occupied[det_idx] = false;
         detection_hist.det_data[det_idx] = {};
         detection_hist.n_occupied--;

         // Remove association from cluster
         cluster.old_det_idx[det_i] = 0;
      }

      cluster.num_old_dets = 0;
      cluster.num_types_of_dets[0] = 0;
      cluster.num_types_of_dets[1] = 0;

      // Mark cluster to be killed
      cluster.f_to_be_killed = true;
   }


   /*===========================================================================*\
   * FUNCTION: Move_Cluster_New_Dets_To_Hist_Det_Structure()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const int16_t max_tot_num_of_hist_dets,
   * const int16_t max_hist_dets_in_single_cluster,
   * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * F360_Cluster_T& cluster,
   * F360_Detection_Hist_T& detection_hist
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function moves new associted detections to the historical detection buffer for a cluster.
   * The cluster is allowed to have at max max_hist_dets_in_single_cluster number of detections in
   * the historical detection buffer. If the toal number of current and historical detections for the
   * cluster exceeds this number a downselection is made prior to moving the detections to chosse which
   * of the detections we want to keep in the hist det buffer and which to discard of.
   * 
   * Note: If the historical detection buffer is full then the cluster is only allowed to keep same amount
   * of historical detections as already have (i.e. number of occupied slots in hist det buffer
   * can't increase). Therefore the hist det buffer must have been clear before calling this function 
   * buffer if there is a need to increase the number of occupied slots.
   *
   * PRECONDITIONS:
   *
   \*===========================================================================*/
   static void Move_Cluster_New_Dets_To_Hist_Det_Structure(
      const int16_t max_tot_num_of_hist_dets,
      const int16_t max_hist_dets_in_single_cluster,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Cluster_T& cluster,
      F360_Detection_Hist_T& detection_hist)
   {
      // Downselect detections
      const int16_t num_unoccupied_detections = max_tot_num_of_hist_dets - static_cast<int16_t>(detection_hist.n_occupied);
      const int16_t num_availiable_slots = cluster.num_old_dets + num_unoccupied_detections;
      const int16_t max_num_dets_to_downselect = std::min(max_hist_dets_in_single_cluster, num_availiable_slots);

      const bool f_detections_to_be_moved_to_hist_structure_is_present = (cluster.ndets > 0);
      const bool f_there_is_room_in_hist_structure = (max_num_dets_to_downselect > 0);

      if (f_detections_to_be_moved_to_hist_structure_is_present && f_there_is_room_in_hist_structure) // We want to try and move some detections from the current detection structure to the historical detection structure
      {
         // Combine new and old detections into one struct
         Detections_Set combined_dets{}; // combined all detections
         Fill_Cluster_Combined_Detection_Struct(cluster, detection_hist, raw_detection_list, sensors, combined_dets);

         // Downselect which detections to store in historical detection buffer
         Downselect_Detections(static_cast<uint16_t>(max_num_dets_to_downselect), combined_dets);

         /* Associate downselected historic detections to cluster and discard of historic detections that are not downselected.
         * Note: This has to be done before associating and adding new detections to the buffer because we have to make room for all downselected new detections. */
         int16_t num_ds_moving_dets = 0;
         cluster.num_old_dets = 0;
         std::fill(cmn::begin(cluster.old_det_idx), cmn::end(cluster.old_det_idx), static_cast<int16_t>(0));
         for (uint16_t det_i = 0U; det_i < combined_dets.num_dets; det_i++)
         {
            if (combined_dets.f_historic[det_i])
            {
               // Detection is historical
               const int16_t det_idx = combined_dets.det_indexes[det_i];
               if (combined_dets.f_downselected[det_i])
               {
                  // Detection is downselected - associate it to the cluster
                  cluster.old_det_idx[cluster.num_old_dets] = det_idx;
                  cluster.num_old_dets++;

                  if (1 == detection_hist.det_data[det_idx].motion_status)
                  {
                     // Moving detection
                     num_ds_moving_dets++;
                  }
               }
               else
               {
                  // Detection is not downselected - discard of it
                  detection_hist.f_idx_occupied[det_idx] = false;
                  detection_hist.det_data[det_idx] = {};
                  detection_hist.n_occupied--;
               }
            }
         }

         // Add new downselected dets to cluster old_det_idx and historical detection data structure
         for (uint16_t det_i = 0U; det_i < combined_dets.num_dets; det_i++)
         {
            if (!combined_dets.f_historic[det_i])
            {
               // Detection is new
               if (combined_dets.f_downselected[det_i])
               {
                  int16_t next_available_det_idx_in_det_hist = max_tot_num_of_hist_dets; // Uninitialized value (too big)
                  if (Find_Next_Hist_Det_Idx(max_tot_num_of_hist_dets, next_available_det_idx_in_det_hist, detection_hist, cluster)) // Note: There should always be a free slot availiable in the historical detection buffer otherwise something has gone wrong
                  {
                     const int16_t assignee_idx = combined_dets.det_indexes[det_i];
                     Update_Single_Detection_Hist(raw_detection_list.detections[assignee_idx], sensors, det_props[assignee_idx], cluster.id - 1, detection_hist.det_data[next_available_det_idx_in_det_hist]);

                     if (1 == detection_hist.det_data[next_available_det_idx_in_det_hist].motion_status)
                     {
                        // Moving detection
                        num_ds_moving_dets++;
                     }
                  }
               }
            }
         }

         // Update num_types_of_dets for cluster based on downselected detections
         cluster.num_types_of_dets[0] = num_ds_moving_dets;
         cluster.num_types_of_dets[1] = cluster.num_old_dets - num_ds_moving_dets;

      }
   }
}
