/******************************************************************************
 * Copyright 2024 Aptiv, All Rights Reserved.
 * Aptiv Confidential
 ******************************************************************************/
/*===========================================================================*\
* FILE: f360_object_track_initialization.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function definitions of Object_Track_Initialization().
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_object_track_initialization.h"
#include "f360_prioritize_clusters.h"
#include "f360_initial_detection_checks.h"
#include "f360_test_stationary_hypothesis.h"
#include "f360_estimate_velocity_by_cloud.h"
#include "f360_estimate_velocity_by_position_change.h"
#include "f360_determine_final_vel_estimate.h"
#include "f360_post_estimate_cloud_only_init_countermeasure.h"
#include "f360_allocate_id_for_initialized_object.h"
#include "f360_populate_track_properties.h"
#include "f360_associate_detection_to_object.h"
#include "f360_sort_priority.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_calculate_priority.h"
#include "f360_occlusion.h"

#if 1
#include "f360_xtrk_logging.h"
#endif

namespace f360_variant_A
{
static void Update_Clutter_Counter(
    const float32_t longpos_diff,
    const float32_t latpos_diff,
    const int16_t det_cluster_id,
    const rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status,
    const float32_t k_max_distance_sq,
    const int16_t cluster_id,
    int8_t (&clutter_cnt)[4]) {
   if ((det_cluster_id != cluster_id) &&
       (det_motion_status != rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) &&
       ((longpos_diff * longpos_diff + latpos_diff * latpos_diff) < k_max_distance_sq)) {
      if ((longpos_diff >= 0.0F) && (latpos_diff >= 0.0F)) {
         clutter_cnt[0]++; // NE
      } else if ((longpos_diff < 0.0F) && (latpos_diff >= 0.0F)) {
         clutter_cnt[1]++; // SE
      } else if ((longpos_diff < 0.0F) && (latpos_diff < 0.0F)) {
         clutter_cnt[2]++; // SW
      } else {
         clutter_cnt[3]++; // NW
      }
   }
}

void Update_Cluster_In_Clutter_Probability(
    const F360_Tracker_Info_T &tracker_info,
    const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
    const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
    F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS]) {
   for (int32_t i = 0; i < tracker_info.num_active_clusters; i++) {
      const int16_t cluster_idx       = tracker_info.active_cluster_ids[i] - 1;
      F360_Cluster_T &cluster         = clusters[cluster_idx];
      const float32_t cluster_longpos = cluster.vcs_position_x;
      const float32_t cluster_latpos  = cluster.vcs_position_y;

      if ((cluster.ndets > 0) &&
          (cluster.num_types_of_dets[1] > 0) &&
          (cluster.vcs_position_x > -20.0F)) {
         const int16_t ref_det_idx         = cluster.detids[0] - 1;
         const uint32_t k_max_iter         = raw_detections.number_of_valid_detections;
         const float32_t k_max_distance    = 4.0F;
         const float32_t k_max_distance_sq = k_max_distance * k_max_distance;
         int16_t det_idx                   = raw_detections.detections[ref_det_idx].processed.prev_sorted_idx;
         int8_t clutter_cnt[4]             = {0};

         if (det_idx >= 0) {
            for (uint32_t j = 0U; j < k_max_iter; j++) {
               const float32_t longpos_diff = det_props[det_idx].vcs_position.x - cluster_longpos;
               const float32_t latpos_diff  = det_props[det_idx].vcs_position.y - cluster_latpos;
               Update_Clutter_Counter(
                   longpos_diff,
                   latpos_diff,
                   det_props[det_idx].cluster_id,
                   det_props[det_idx].motion_status,
                   k_max_distance_sq,
                   cluster.id,
                   clutter_cnt);

               det_idx = raw_detections.detections[det_idx].processed.prev_sorted_idx;
               if ((fabsf(longpos_diff) > k_max_distance) || (det_idx < 0)) {
                  break;
               }
            }
         }

         det_idx = raw_detections.detections[ref_det_idx].processed.next_sorted_idx;
         if (det_idx >= 0) {
            for (uint32_t j = 0U; j < k_max_iter; j++) {
               const float32_t longpos_diff = det_props[det_idx].vcs_position.x - cluster_longpos;
               const float32_t latpos_diff  = det_props[det_idx].vcs_position.y - cluster_latpos;
               Update_Clutter_Counter(
                   longpos_diff,
                   latpos_diff,
                   det_props[det_idx].cluster_id,
                   det_props[det_idx].motion_status,
                   k_max_distance_sq,
                   cluster.id,
                   clutter_cnt);

               det_idx = raw_detections.detections[det_idx].processed.next_sorted_idx;
               if ((fabsf(longpos_diff) > k_max_distance) || (det_idx < 0)) {
                  break;
               }
            }
         }

         const int8_t sum_clutter_cnt    = clutter_cnt[0] + clutter_cnt[1] + clutter_cnt[2] + clutter_cnt[3];
         const bool f_opposing_quadrants = ((clutter_cnt[0] > 0) && (clutter_cnt[2] > 0)) || ((clutter_cnt[1] > 0) && (clutter_cnt[3] > 0));
         int8_t num_quadrants            = 0;
         num_quadrants += (clutter_cnt[0] > 0) ? 1 : 0;
         num_quadrants += (clutter_cnt[1] > 0) ? 1 : 0;
         num_quadrants += (clutter_cnt[2] > 0) ? 1 : 0;
         num_quadrants += (clutter_cnt[3] > 0) ? 1 : 0;

         if ((sum_clutter_cnt > 5) && (num_quadrants > 3)) {
            cluster.clutter_counter += 2;
         } else if ((sum_clutter_cnt > 3) && f_opposing_quadrants) {
            cluster.clutter_counter++;
         } else if (sum_clutter_cnt < 2) {
            cluster.clutter_counter--;
         } else {
            // do nothing
         }

         const int16_t counter_max = 10;
         const int16_t counter_min = 0;
         cluster.clutter_counter   = std::max(counter_min, std::min(counter_max, cluster.clutter_counter));
      }
   }
}

/*===========================================================================*\
* FUNCTION: Object_Track_Initialization()
*===========================================================================
* RETURN VALUE:
* NONE
*
* PARAMETERS:
* const F360_Globals_T& globals
* const F360_Calibrations_T& calibs
* const F360_Host_T& host
* const Static_Env_Poly_T(&sep)[F360_NUM_OF_STATIC_ENV_POLYS]
* const F360_Occlusion_Data_T& occlusion_data
* const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
* const F360_Detection_Hist_T& det_hist
* const rspp_variant_A::RSPP_Detection_List_T& raw_detections
* F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
* F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS]
* F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
* F360_Tracker_Info_T& tracker_info)
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
* This function performs object initialization. I.e. it takes clusters and transforms them into objects.
* If number of objects are saturated the clusters are prioritized and compared with priority of already
* existing prior to initialization to make sure that the most critical objects are always tracked.
* Cluster 2D velocities (longitudinal and lateral) are computed by one of the following paths:
*    1) Clusters with detections with small compensated range rate and small detection position spread over
*       time are initialized through Test_Stationary_Hypothesis()
*  2) If cluster does not pass Test_Stationary_Hypothesis() then it it is only being initialized if it is not occluded.
*     2 different IRLS agorithms are run in order to estimate object velocity based on the detection range rates and
*     position difference over time respectively. The two results are then combined into one by considering the confidence
*     of the output from each each algorithm.
*
* As a last step all new object is created, all its states are filled and the cluster is marked as ready to be killed.
*
* PRECONDITIONS:
* None
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
void Object_Track_Initialization(
    const F360_Globals_T &globals,
    const F360_Calibrations_T &calibs,
    const F360_Host_T &host,
    const Static_Env_Poly_T (&sep)[F360_NUM_OF_STATIC_ENV_POLYS],
    const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
    const F360_Detection_Hist_T &det_hist,
    const rspp_variant_A::RSPP_Detection_List_T &raw_detections,
    const F360_Occlusion_Data_T (&occlusion_data)[MAX_NUMBER_OF_SENSORS],
    F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
    F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
    F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
    F360_Tracker_Info_T &tracker_info) {
   int32_t prioritized_cluster_ids[NUMBER_OF_CLUSTERS];
   uint32_t num_clusters = 0U;
   Prioritize_Clusters(calibs, host, tracker_info, det_hist, raw_detections, det_props, clusters, prioritized_cluster_ids, num_clusters);

   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detections, det_props, clusters);

   for (uint32_t i = 0U; i < num_clusters; i++) {
      const int32_t cluster_idx = prioritized_cluster_ids[i] - 1;
      F360_Cluster_T &cluster   = clusters[cluster_idx];

      struct
      {
         float32_t longvel_estimate;
         float32_t latvel_estimate;
         float32_t longvel_by_cloud;
         float32_t longvel_by_position;
         float32_t latvel_by_cloud;
         float32_t latvel_by_position;
         F360_Track_Init_T init_type              = F360_TRACK_INIT_INVALID;
         F360_Occlusion_Status_T occlusion_status = OCCLUSION_STATUS_UNDEFINED;
         bool f_ambiguous_motion_in_clutter       = false;
         CONF3_T cloud_confidence                 = CONF3_NONE;
         CONF3_T posdiff_confidence               = CONF3_NONE;
      } init_data                                                     = {};
      constexpr float32_t suspected_stationary_rdotcomp_max_threshold = 1.0F;

      if (std::abs(cluster.rep_rdotcomp) < suspected_stationary_rdotcomp_max_threshold) {
         init_data.init_type = Test_Stationary_Hypothesis(calibs, host, det_hist, raw_detections, det_props, cluster, init_data.longvel_estimate, init_data.latvel_estimate);
      }

      if (init_data.init_type != F360_TRACK_INIT_STATIONARY) {
         init_data.occlusion_status = Get_Point_Occlusion_Status(sensors, occlusion_data, cluster.vcs_position_x, cluster.vcs_position_y);

         init_data.f_ambiguous_motion_in_clutter = (cluster.clutter_counter >= 5) &&
                                                   ((cluster.num_types_of_dets[1] > (4 * cluster.num_types_of_dets[0])) || (fabsf(cluster.rep_rdotcomp) < 1.5F));

         if (((init_data.occlusion_status == OCCLUSION_STATUS_VISIBLE) || (cluster.ndets > 6)) && (!init_data.f_ambiguous_motion_in_clutter)) {
            init_data.cloud_confidence   = Estimate_Velocity_By_Cloud(det_hist, raw_detections, det_props, cluster, init_data.longvel_by_cloud, init_data.latvel_by_cloud);
            init_data.posdiff_confidence = Estimate_Velocity_By_Position_Change(sensors, det_hist, raw_detections, det_props, cluster, init_data.longvel_by_position, init_data.latvel_by_position);

            init_data.init_type = Determine_Final_Vel_Estimate(init_data.longvel_by_position, init_data.latvel_by_position, init_data.longvel_by_cloud, init_data.latvel_by_cloud,
                                                               init_data.posdiff_confidence, init_data.cloud_confidence, init_data.longvel_estimate, init_data.latvel_estimate);

            Post_Estimate_Cloud_Only_Init_Countermeasure(host, cluster, init_data.posdiff_confidence, init_data.longvel_estimate, init_data.init_type);
         }
      }

#if 1
      // Additional xtrk logging can be turned on by setting XTRKLOG_ADDITIONAL_INIT_DATA to true inside file OT_ObjectTracking\modules\F360Core\sw\F360TrackerLib\CMakeLists.txt
      Prepare_Init_Cluster_Debug_Data(cluster,
                                      raw_detections, det_props, sensors, det_hist,
                                      init_data.occlusion_status,
                                      init_data.cloud_confidence, init_data.posdiff_confidence,
                                      init_data.f_ambiguous_motion_in_clutter,
                                      init_data.init_type,
                                      init_data.longvel_by_cloud, init_data.latvel_by_cloud,
                                      init_data.longvel_by_position, init_data.latvel_by_position);
#endif

      if (init_data.init_type > F360_TRACK_INIT_INVALID) {

         /* Recompute cluster priority since initially we did not know if correspnding object would be moving or not so we used moving assumption as default
          * Now after running initialization algorithm we know more (we know new cluster velocity) and calculated priority should be recomputed. This prevents
          * that a stationary cluster at some position gets a higher priority compared to an already existing stationary object at same position. */
         bool f_new_obj_better_priority_than_old_obj = true;
         if (tracker_info.num_active_objs >= static_cast<int16_t>(tracker_info.variant.num_tracks)) {
            if (nullptr != tracker_info.p_lowest_priority_track) {
               const float32_t new_obj_speed_sq       = F360_Get_Hypotenuse_Squared(init_data.longvel_estimate, init_data.latvel_estimate);
               const float32_t new_obj_movable_prob   = (new_obj_speed_sq > globals.obj_mov_stat_spd_thresh * globals.obj_mov_stat_spd_thresh) ? 1.0F : 0.0F; // Note: This threshold is the same as is used inside Fill_Init_Obj_Track_Props() here below
               const float32_t new_obj_priority       = Calculate_Priority(host, new_obj_movable_prob, calibs.k_init_default_confidence, cluster.vcs_position_x, cluster.vcs_position_y);
               f_new_obj_better_priority_than_old_obj = (new_obj_priority > tracker_info.p_lowest_priority_track->priority);
            }
         }

         if (f_new_obj_better_priority_than_old_obj) {
            const int32_t new_id                      = Allocate_Id_For_Initialized_Object(tracker_info, object_tracks, det_props);
            F360_Object_Track_T &object_track_to_init = object_tracks[new_id - 1];
            object_track_to_init.init_scheme          = init_data.init_type;
            cluster.f_to_be_killed                    = true;

#if 1
            // Additional xtrk logging can be turned on by setting XTRKLOG_ADDITIONAL_INIT_DATA to true inside file OT_ObjectTracking\modules\F360Core\sw\F360TrackerLib\CMakeLists.txt
            Prepare_Init_Debug_Data(new_id, init_data.longvel_by_cloud, init_data.latvel_by_cloud, init_data.longvel_by_position, init_data.latvel_by_position, cluster);
#endif

            for (int32_t j = 0; j < cluster.ndets; j++) {
               const uint32_t det_idx = static_cast<uint32_t>(cluster.detids[j]) - 1U;
               (void)Associate_Detection_To_Object(tracker_info, object_track_to_init, det_props[det_idx], det_idx + 1U);
            }

            Populate_Track_Properties(globals, calibs, host, cluster, det_props, raw_detections, det_hist, sep, sensors, tracker_info.num_unique_objs, init_data.longvel_estimate, init_data.latvel_estimate, object_track_to_init);

            Sort_Priority_With_New_Track(tracker_info, &object_track_to_init);

            Sorted_Tracks_Insert(tracker_info, &object_track_to_init);
         } else if (F360_TRACK_INIT_STATIONARY == init_data.init_type) {
            cluster.stationary_cluster_cnt++;
            if (cluster.stationary_cluster_cnt > 3U) {
               // Stationary clusters that does not create objects for more than 3 scans will be killed. Further treatment is in Sensor Postprocessing
               cluster.f_to_be_killed = true;
            }
         } else {
            // Nothing, MISRA
         }
      }
   }
}
} // namespace f360_variant_A
