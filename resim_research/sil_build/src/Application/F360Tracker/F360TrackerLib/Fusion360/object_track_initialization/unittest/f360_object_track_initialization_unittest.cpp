/** \file
 * This file contains unit tests for content of f360_object_track_initialization.cpp file
 */

#include <CppUTest/TestHarness.h>
#include "f360_object_track_initialization.h"
#include "f360_occlusion.h"
#include "f360_set_variant.h"
#include "f360_calculate_priority.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_track_initialization
 *  @{
 */

/** \brief
 * Test high level functionality of the initialization algorithm.
 */
TEST_GROUP(f360_object_track_initialization)
{
   // Declare common variables used within all tests in this test group.
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];
   F360_Detection_Hist_T det_hist;
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibrations;
   F360_Globals_T globals;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Host_T host;
   F360_Tracker_Info_T tracker_info;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_TRKR_TIMING_INFO_T timing_info;
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS];

   /** \setup
    * Initialize tracker calibrations, variant and set up occlusion input
    */
   TEST_SETUP()
   {
      /** \setup
       * Initialize tracker calibrations
       **/
      for(int32_t i = 0; i < 4; i++)
      {
         sensors[0].constant.range_limits[i] = 200.0F;
         sensors[0].constant.fov_min_az_rad[i] = F360_DEG2RAD(-60.0F);
         sensors[0].constant.fov_max_az_rad[i] = F360_DEG2RAD(60.0F);
      }
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      sensors[0].refined.time_since_measurement_s = 0.00F;
      sensors[0].variable.is_valid = true;

      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);

      for(uint32_t i = 0; i < tracker_info.variant.num_tracks; i++)
      {
         tracker_info.inactive_obj_ids[i] = i+1;
      }
      tracker_info.num_active_clusters = 0;
      tracker_info.num_active_objs = 0;
      tracker_info.num_unique_objs = 0;
      tracker_info.p_lowest_priority_track = nullptr;
      tracker_info.vcslong_sorted_start = nullptr;
      for(uint32_t i = 0; i < tracker_info.variant.num_tracks; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = nullptr;
         tracker_info.vcslong_sorted_prev_track[i] = nullptr;
      }
      (void)memset(tracker_info.active_cluster_ids, 0, sizeof(tracker_info.active_cluster_ids));
      (void)memset(tracker_info.active_obj_ids, 0, sizeof(tracker_info.active_obj_ids));
      (void)memset(clusters, 0, sizeof(clusters));
      (void)memset(detections, 0, sizeof(detections));
      (void)memset(object_tracks, 0, sizeof(object_tracks));
      for(uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         object_tracks[i].id = static_cast<int32_t>(i + 1U);
      }
      (void)memset(&raw_detect_list, 0, sizeof(raw_detect_list));
      (void)memset(&det_hist, 0, sizeof(det_hist));
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));
   }

   // Define helper functions.
   void setup_stationary_cluster(
      F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos)
   {
      assert(tracker_info.num_active_clusters < tracker_info.variant.num_clusters);
      int32_t cluster_id = tracker_info.num_active_clusters + 1;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters] = cluster_id;
      tracker_info.num_active_clusters++;

      F360_Cluster_T& cluster = clusters[cluster_id - 1];
      cluster.id = cluster_id;
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = 0.0F;
      cluster.f_dealiased = true;

      for (int32_t i = 0; i < 4; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.det_id = static_cast<uint16_t>(n + 1);
         raw_detect_list.detections[n].raw.sensor_id = 1;
         raw_detect_list.detections[n].raw.azimuth = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = 0.0F;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = 0.0F;
      }
      for (int32_t i = 0; i < 4; i++)
      {
         int32_t n = det_hist.n_occupied;
         det_hist.n_occupied++;
         cluster.num_old_dets++;
         cluster.old_det_idx[i] = n;

         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y;
         det_hist.det_data[n].rdot_comp = 0.0F;
      }
   }

   void setup_moving_cluster_for_posdiff(
      F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos,
      const float xvel, const float yvel,
      const int32_t ndets)
   {
      assert(tracker_info.num_active_clusters < tracker_info.variant.num_clusters);
      int32_t cluster_id = tracker_info.num_active_clusters + 1;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters] = cluster_id;
      tracker_info.num_active_clusters++;

      F360_Cluster_T& cluster = clusters[cluster_id - 1];
      cluster.id = cluster_id;
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = F360_Atan2f(ypos, xpos);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel + cluster.sin_vcs_az * yvel;
      cluster.f_dealiased = true;

      for (int32_t i = 0; i < ndets; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.det_id = static_cast<uint16_t>(n + 1);
         raw_detect_list.detections[n].raw.sensor_id = 1;
         raw_detect_list.detections[n].raw.azimuth = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = cluster.rep_rdotcomp;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
      }

      for (int32_t i = 0; i < 10; i++)
      {
         int32_t i1 = i*2;
         int32_t i2 = i*2+1;
         float T = 0.05F;

         cluster.num_old_dets++;
         int32_t n = det_hist.n_occupied;
         cluster.old_det_idx[i1] = n;
         det_hist.n_occupied++;
         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * (i + 1) * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * (i + 1) * yvel;
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = (i + 1) * T;

         cluster.num_old_dets++;
         n = det_hist.n_occupied;
         cluster.old_det_idx[i2] = n;
         det_hist.n_occupied++;
         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * (i + 1) * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * (i + 1) * yvel;
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = (i + 1) * T;
      }
      cluster.num_types_of_dets[0] = cluster.num_old_dets + cluster.ndets;
   }

   void setup_moving_cluster_for_cloud(
      F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float xpos, const float ypos,
      const float xvel, const float yvel)
   {
      assert(tracker_info.num_active_clusters < tracker_info.variant.num_clusters);
      int32_t cluster_id = tracker_info.num_active_clusters + 1;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters] = cluster_id;
      tracker_info.num_active_clusters++;

      F360_Cluster_T& cluster = clusters[cluster_id - 1];
      cluster.id = cluster_id;
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = F360_Atan2f(ypos, xpos);
      cluster.cos_vcs_az = F360_Cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = F360_Sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel + cluster.sin_vcs_az * yvel;
      cluster.f_dealiased = true;

      float ypos_arr[8] = {-3.5, -2.5, -1.5, -0.5, 0.5, 1.5, 2.5, 3.5};
      for (int32_t i = 0; i < 8; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.sensor_id = 1;
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y + ypos_arr[i];
         raw_detect_list.detections[n].raw.azimuth = F360_Atan2f(raw_detect_list.detections[n].processed.vcs_position_y, raw_detect_list.detections[n].processed.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_az = F360_Atan2f(raw_detect_list.detections[n].processed.vcs_position_y, raw_detect_list.detections[n].processed.vcs_position_x);
         raw_detect_list.detections[n].processed.cos_vcs_az = F360_Cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = F360_Sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = raw_detect_list.detections[n].processed.cos_vcs_az * xvel + raw_detect_list.detections[n].processed.sin_vcs_az * yvel;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
      }

      for (int32_t i = 0; i < 6; i++)
      {
         float T = 0.05F;

         int32_t n = det_hist.n_occupied;
         cluster.num_old_dets++;
         det_hist.n_occupied++;
         cluster.old_det_idx[i] = n;
         det_hist.det_data[n].sensor_id = 1;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * yvel + ypos_arr[i];
         det_hist.det_data[n].vcs_az = F360_Atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = F360_Cosf(det_hist.det_data[n].vcs_az) * xvel + F360_Sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = T;
      }
   }
};

/** \purpose
 * Call Object_Track_Initialization() without any active objects or clusters.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, base_test)
{
   /** \precond
    * Set clusters and objects to zero.
    */
   tracker_info.num_active_objs = 0;
   tracker_info.num_active_clusters = 0;
   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect unchanged output.
    */
   CHECK_EQUAL(0, tracker_info.num_active_clusters);
   CHECK_EQUAL(0, tracker_info.num_active_objs);
}

/** \purpose
 * Call Object_Track_Initialization() with three different clusters setup for
 * successfull initialization.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, init_stationary_and_moving)
{
   /** \precond
    * Set up clusters for stationary, cloud and position difference init methods.
    */
   float xpos_stationary = 50.0F;
   float ypos_stationary = -2.0F;
   setup_stationary_cluster(tracker_info, clusters, raw_detect_list, det_hist, xpos_stationary, ypos_stationary);

   float xpos_posdiff = 50.0F;
   float ypos_posdiff = 2.0F;
   float xvel_posdiff = -5.0F;
   float yvel_posdiff = 0.0F;
   int32_t ndets_posdiff = 7;
   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_posdiff, ypos_posdiff, xvel_posdiff, yvel_posdiff, ndets_posdiff);

   float xpos_cloud = 10.0F;
   float ypos_cloud = 0.0F;
   float xvel_cloud = 30.0F;
   float yvel_cloud = 1.0F;
   setup_moving_cluster_for_cloud(tracker_info, clusters, raw_detect_list, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect 3 initialized tracks with properties consistent with the clusters properties
    */
   CHECK_EQUAL(3, tracker_info.num_active_objs);
   CHECK_EQUAL(F360_TRACK_INIT_STATIONARY, object_tracks[0].init_scheme);
   CHECK_EQUAL(F360_TRACK_INIT_POSDIFF, object_tracks[1].init_scheme);
   CHECK_EQUAL(F360_TRACK_INIT_CLOUD, object_tracks[2].init_scheme);

   DOUBLES_EQUAL(xpos_stationary, object_tracks[0].vcs_position.x, 0.001F);
   DOUBLES_EQUAL(ypos_stationary, object_tracks[0].vcs_position.y, 0.001F);
   DOUBLES_EQUAL(0.0F, object_tracks[0].vcs_velocity.longitudinal, 0.001F);
   DOUBLES_EQUAL(0.0F, object_tracks[0].vcs_velocity.lateral, 0.001F);
   DOUBLES_EQUAL(xpos_posdiff, object_tracks[1].vcs_position.x, 0.001F);
   DOUBLES_EQUAL(ypos_posdiff, object_tracks[1].vcs_position.y, 0.001F);
   DOUBLES_EQUAL(xvel_posdiff, object_tracks[1].vcs_velocity.longitudinal, 0.001F);
   DOUBLES_EQUAL(yvel_posdiff, object_tracks[1].vcs_velocity.lateral, 0.001F);
   DOUBLES_EQUAL(xpos_cloud, object_tracks[2].vcs_position.x, 0.2F);
   DOUBLES_EQUAL(ypos_cloud, object_tracks[2].vcs_position.y, 0.1F);
   DOUBLES_EQUAL(xvel_cloud, object_tracks[2].vcs_velocity.longitudinal, 0.001F);
   DOUBLES_EQUAL(yvel_cloud, object_tracks[2].vcs_velocity.lateral, 0.001F);
}

/** \purpose
 * Check if occluded cluster doesn't get initialized as an object.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, init_occluded)
{
   /** \precond
    * Occluding object must be initialized, its confidenceLevel must be > 0.8F
    * One occluded cluster with ndets = 2 should not be initialized
    * One occluded cluster with ndets = 7 should be initialized
    * In total 2 objects should be initialized
    */

   // set up first cluster
   float xpos_cloud = 2.0F;
   float ypos_cloud = 0.0F;
   float xvel_cloud = 10.0F;
   float yvel_cloud = 0.0F;
   setup_moving_cluster_for_cloud(tracker_info, clusters, raw_detect_list, det_hist, xpos_cloud, ypos_cloud, xvel_cloud, yvel_cloud);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   // update object confidenceLevel needed for occlusion
   object_tracks[0].confidenceLevel = 1.0F;
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   // remove cluster already initialized as an object
   (void)memset(clusters, 0, sizeof(clusters));

   // set up cluster that is occluded by the previous one
   float xpos_posdiff = 10.0F;
   float ypos_posdiff = 0.0F;
   float xvel_posdiff = 10.0F;
   float yvel_posdiff = 0.0F;
   int32_t ndets = 2;
   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_posdiff, ypos_posdiff, xvel_posdiff, yvel_posdiff, ndets);

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   // set up cluster that is ocluded by the first one, but now has ndets > 6
   ndets = 7;
   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_posdiff, ypos_posdiff, xvel_posdiff, yvel_posdiff, ndets);

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect 2 initialized tracks with the properties of the first cluster and third cluster.
    */
    CHECK_EQUAL(2, tracker_info.num_active_objs);
    DOUBLES_EQUAL(xpos_cloud, object_tracks[0].vcs_position.x, 0.001F);
    DOUBLES_EQUAL(ypos_cloud, object_tracks[0].vcs_position.y, 0.001F);
    DOUBLES_EQUAL(xvel_cloud, object_tracks[0].vcs_velocity.longitudinal, 0.001F);
    DOUBLES_EQUAL(yvel_cloud, object_tracks[0].vcs_velocity.lateral, 0.001F);
    DOUBLES_EQUAL(xpos_posdiff, object_tracks[1].vcs_position.x, 0.001F);
    DOUBLES_EQUAL(ypos_posdiff, object_tracks[1].vcs_position.y, 0.001F);
    DOUBLES_EQUAL(xvel_posdiff, object_tracks[1].vcs_velocity.longitudinal, 0.001F);
    DOUBLES_EQUAL(yvel_posdiff, object_tracks[1].vcs_velocity.lateral, 0.001F);
}

/** \purpose
 * Verify that when maximum number of objects is already being tracked, a stationary cluster does not initialize an
 * object if there is already an existing object closer to host (with higher priority).
 * \req
 * NA
 */
TEST(f360_object_track_initialization, dont_init_stationary_cluster_with_lower_prio_than_existing_obj)
{
   /** \precond
    * Set up a cluster for a stationary object at position (100, 0).
    * Set up an existing object at position (10, 0) with a low movable probability
    */
   float xpos_stationary = 100.0F;
   float ypos_stationary = 0.0F;
   setup_stationary_cluster(tracker_info, clusters, raw_detect_list, det_hist, xpos_stationary, ypos_stationary);

   tracker_info.variant.num_tracks = 1;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.p_lowest_priority_track = &object_tracks[0];

   object_tracks[0].id = 1;
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].movable_prob = 0.0F;
   tracker_info.p_lowest_priority_track->priority = Calculate_Priority(
      host,
      object_tracks[0].movable_prob,
      calibrations.k_init_default_confidence,
      object_tracks[0].vcs_position.x,
      object_tracks[0].vcs_position.y);

   const float32_t exp_obj_xpos = object_tracks[0].vcs_position.x;
   const float32_t exp_obj_ypos = object_tracks[0].vcs_position.y;

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that the new object is not initialized because its priority is lower than the existing object's priority
    * and the existing object remains unchanged.
    */
   DOUBLES_EQUAL_TEXT(exp_obj_xpos, object_tracks[0].vcs_position.x, 0.001F, "Existing object x position changed");
   DOUBLES_EQUAL_TEXT(exp_obj_ypos, object_tracks[0].vcs_position.y, 0.001F, "Existing object y position changed");
}

/** \purpose
 * Verify that when maximum number of objects is already being tracked, a stationary cluster is initialized even
 * though there is an existing object with lower priority.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, init_stationary_cluster_no_lowest_prio_obj)
{
   /** \precond
    * Set up a cluster for a stationary object at position (100, 0).
    */
   float xpos_stationary = 100.0F;
   float ypos_stationary = 0.0F;
   setup_stationary_cluster(tracker_info, clusters, raw_detect_list, det_hist, xpos_stationary, ypos_stationary);

   tracker_info.variant.num_tracks = 1;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.p_lowest_priority_track = &object_tracks[0];

   object_tracks[0].id = 1;
   object_tracks[0].vcs_position.x = 1000.0F;
   object_tracks[0].vcs_position.y = 1000.0F;
   object_tracks[0].movable_prob = 0.0F;
   tracker_info.p_lowest_priority_track->priority = Calculate_Priority(
      host,
      object_tracks[0].movable_prob,
      calibrations.k_init_default_confidence,
      object_tracks[0].vcs_position.x,
      object_tracks[0].vcs_position.y);

   const float32_t exp_obj_xpos = xpos_stationary;
   const float32_t exp_obj_ypos = ypos_stationary;

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that the new object is initialized because its priority is higher than the existing object's priority.
    */
   CHECK_EQUAL(1, tracker_info.num_active_objs);
   DOUBLES_EQUAL_TEXT(exp_obj_xpos, object_tracks[0].vcs_position.x, 0.001F, "Object x position should match initialized stationary cluster");
   DOUBLES_EQUAL_TEXT(exp_obj_ypos, object_tracks[0].vcs_position.y, 0.001F, "Object y position should match initialized stationary cluster");
}

/** \purpose
 * Verify that when maximum number of objects is already being tracked, a moving cluster does not initialize an
 * object if there is already an existing object closer to host (with higher priority).
 * \req
 * NA
 */
TEST(f360_object_track_initialization, dont_init_moving_cluster_with_lower_prio_than_existing_obj)
{
   /** \precond
    * Set up a cluster for a moving object at position (100, 0) with lateral speed 2.0.
    * Set up an existing object at position (10, 0) with a high movable probability
    */
   float xpos_moving = 100.0F;
   float ypos_moving = 0.0F;
   globals.obj_mov_stat_spd_thresh = 1.0F; // set threshold below the cluster speed to ensure higher priority.

   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_moving, ypos_moving, 0.0F, 2.0F, 7);

   tracker_info.variant.num_tracks = 1;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.p_lowest_priority_track = &object_tracks[0];

   object_tracks[0].id = 1;
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].movable_prob = 1.0F;
   tracker_info.p_lowest_priority_track->priority = Calculate_Priority(
      host,
      object_tracks[0].movable_prob,
      calibrations.k_init_default_confidence,
      object_tracks[0].vcs_position.x,
      object_tracks[0].vcs_position.y);

   const float32_t exp_obj_xpos = object_tracks[0].vcs_position.x;
   const float32_t exp_obj_ypos = object_tracks[0].vcs_position.y;

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that the new object is not initialized because its priority is lower than the existing object's priority
    * and the existing object remains unchanged.
    */
   DOUBLES_EQUAL_TEXT(exp_obj_xpos, object_tracks[0].vcs_position.x, 0.001F, "Existing object x position changed");
   DOUBLES_EQUAL_TEXT(exp_obj_ypos, object_tracks[0].vcs_position.y, 0.001F, "Existing object y position changed");
}

/** \purpose
 * Verify that when maximum number of objects is already being tracked, a moving cluster is initialized even though
 * there is an existing object with lower priority.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, init_moving_cluster_with_higher_prio_than_existing_obj)
{
   /** \precond
    * Set up a cluster for a moving object at position (10, 0) with longitudinal speed -10.0, heading towards host.(higher priority)
    * Set up an existing object at position (20, 0) with a high movable probability. (lower priority)
    */
   float xpos_moving = 10.0F;
   float ypos_moving = 0.0F;
   globals.obj_mov_stat_spd_thresh = 1.0F;

   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_moving, ypos_moving, -10.0F, 0.0F, 7);

   tracker_info.variant.num_tracks = 1;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.p_lowest_priority_track = &object_tracks[0];

   object_tracks[0].id = 1;
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].movable_prob = 1.0F;
   tracker_info.p_lowest_priority_track->priority = Calculate_Priority(
      host,
      object_tracks[0].movable_prob,
      calibrations.k_init_default_confidence,
      object_tracks[0].vcs_position.x,
      object_tracks[0].vcs_position.y);

   const float32_t exp_obj_xpos = xpos_moving;
   const float32_t exp_obj_ypos = ypos_moving;

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that the new object is initialized because its priority is higher than the existing object's priority.
    */
   DOUBLES_EQUAL_TEXT(exp_obj_xpos, object_tracks[0].vcs_position.x, 0.001F, "Existing object x position changed");
   DOUBLES_EQUAL_TEXT(exp_obj_ypos, object_tracks[0].vcs_position.y, 0.001F, "Existing object y position changed");
}

/** \purpose
 * Verify that object is not initialized when clutter counter is high and 
 * rep_rdot is positive but to low.
 * Test shall trigger f_ambiguous_motion_in_clutter condition.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, no_init_when_clutter_counter_too_high_rep_rdot_positive_too_low)
{
   /** \precond
    * Set up a moving cluster with clutter_counter = 5
    * rep_rdot < 1.5
    */
   float xpos_moving = 10.0F;
   float ypos_moving = 0.0F;
   float cluster_rep_rdot = 1.4F;

   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_moving, ypos_moving, cluster_rep_rdot, 0.0F, 7);
   clusters[0].clutter_counter = 5;
   clusters[0].f_to_be_killed = false;
   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that no object was initialized due to high clutter counter and rep_rdot too low
    */
   CHECK_EQUAL_TEXT(0, tracker_info.num_active_objs, "Object should not be initialized when clutter counter is high");
   CHECK_EQUAL_TEXT(false, clusters[0].f_to_be_killed, "Cluster should not be marked to be killed when clutter counter is high");
}

/** \purpose
 * Verify that object is not initialized when clutter counter is high and 
 * rep_rdot too low and negative.
 * Test shall trigger f_ambiguous_motion_in_clutter condition.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, no_init_when_clutter_counter_too_high_rep_rdot_too_low_and_negative)
{
   /** \precond
    * Set up a moving cluster with clutter_counter = 5
    * rep_rdot < -1.5
    */
   float xpos_moving = 10.0F;
   float ypos_moving = 0.0F;
   float cluster_rep_rdot = -1.4F;

   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_moving, ypos_moving, cluster_rep_rdot, 0.0F, 7);
   clusters[0].clutter_counter = 5;
   clusters[0].f_to_be_killed = false;
   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that no object was initialized due to high clutter counter and rep_rdot too low
    */
   CHECK_EQUAL_TEXT(0, tracker_info.num_active_objs, "Object should not be initialized when clutter counter is high");
   CHECK_EQUAL_TEXT(false, clusters[0].f_to_be_killed, "Cluster should not be marked to be killed when clutter counter is high");
}

/** \purpose
 * Verify that object is not initialized when clutter counter is high and 
 * number of stationary detections in cluster much higher than number of 
 * moving detections. Test shall trigger f_ambiguous_motion_in_clutter condition.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, no_init_when_clutter_counter_high_many_more_stationary_dets)
{
   /** \precond
    * Set up a moving cluster with clutter_counter = 6 and many more stationary detections than moving
    */
   float xpos_moving = 10.0F;
   float ypos_moving = 0.0F;

   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_moving, ypos_moving, 1.5F, 0.0F, 7);
   clusters[0].clutter_counter = 6;
   clusters[0].f_to_be_killed = false;
   clusters[0].num_types_of_dets[0] = 1; //Moving detections
   clusters[0].num_types_of_dets[1] = 5; //Stationary
   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * Expect that no object was initialized due to high clutter counter
    */
   CHECK_EQUAL_TEXT(0, tracker_info.num_active_objs, "Object should not be initialized when clutter counter is high");
   CHECK_EQUAL_TEXT(false, clusters[0].f_to_be_killed, "Cluster should not be marked to be killed when clutter counter is high");
}

/** \purpose
 * Test the stationary clusters that has been down-prioritized for more than 3 scans will be killed.
 * While moving cluster should not be impacted.
 * Call Object_Track_Initialization() with two different types of clusters setup.
 * The stationary cluster has stationary cluster counter = 3 and if it will not be used for object creation, it will be killed in Object_Track_Initialization()
 * The moving cluster already has stationary cluster counter > 3, but since it is moving, it will not be impacted.
 * \req
 * NA
 */
TEST(f360_object_track_initialization, kill_long_time_stationary_cluster)
{
   /** \precond
    * Set up 2 stationary clusters with different stationary_cluster_cnt
    */
   tracker_info.variant.num_tracks = 1U;  // Only allow 1 object to exist
   tracker_info.num_active_objs = 1U;
   tracker_info.p_lowest_priority_track = &object_tracks[0];
   object_tracks[0].id = 1;
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 3.0F;
   object_tracks[0].vcs_velocity.longitudinal = 6.0F;
   object_tracks[0].vcs_velocity.lateral = 3.0F;
   object_tracks[0].movable_prob = 0.0F;
   tracker_info.p_lowest_priority_track->priority = 0.72F;  // such that the cluster will not be downprioritized compared to object 0

   const float xpos_stationary_cluster_1 = -30.0F;
   const float ypos_stationary_cluster_1 = 6.0F;
   setup_stationary_cluster(tracker_info, clusters, raw_detect_list, det_hist, xpos_stationary_cluster_1, ypos_stationary_cluster_1);
   const int16_t cluster_id_1 = tracker_info.active_cluster_ids[tracker_info.num_active_clusters - 1];
   // Make the counter at the threshold. If the cluster is not used for object initialization by Object_Track_Initialization(), the counter will be incremented and the cluster should be killed.
   clusters[cluster_id_1 - 1].stationary_cluster_cnt = 3U;

   const float xpos_moving_cluster_2 = -30.0F;
   const float ypos_moving_cluster_2 = 6.0F;
   setup_moving_cluster_for_posdiff(tracker_info, clusters, raw_detect_list, det_hist, xpos_moving_cluster_2, ypos_moving_cluster_2, -10.0F, 0.0F, 7);
   const int16_t cluster_id_2 = tracker_info.active_cluster_ids[tracker_info.num_active_clusters - 1];
   // Make the counter exceed the threshold, although it doesn't matter for this moving cluster and the counter will not be incremented
   clusters[cluster_id_2 - 1].stationary_cluster_cnt = 4U;

   /** \action
    * Call Object_Track_Initialization().
    */
   Object_Track_Initialization(globals, calibrations, host, static_env_polys, sensors, det_hist, raw_detect_list, occlusion_data, detections, clusters, object_tracks, tracker_info);

   /** \result
    * No new object shall be initialized since max number of objects is reached and 
    * both of the 2 clusters are down-prioritized because their priorities are designed to be lower than 0.72 (the priority of the lowest priority object).
    * The stationary cluster shall be marked to be killed, while the moving cluster shall not be killed.
    */
   CHECK_EQUAL(1, tracker_info.num_active_objs);  // Still only allow 1 object
   CHECK_EQUAL(true, clusters[cluster_id_1 - 1].f_to_be_killed);  // The stationary cluster will be killed
   CHECK_EQUAL(false, clusters[cluster_id_2 - 1].f_to_be_killed);  // The moving cluster will not be killed
   CHECK_EQUAL(4U, clusters[cluster_id_1 - 1].stationary_cluster_cnt);  // The counter of stationary cluster got incremented
   CHECK_EQUAL(4U, clusters[cluster_id_2 - 1].stationary_cluster_cnt);  // The counter of moving cluster remains the same
   DOUBLES_EQUAL(10.0F, object_tracks[0].vcs_position.x, 0.001F);  // Object 0 is not replaced
   DOUBLES_EQUAL(3.0F, object_tracks[0].vcs_position.y, 0.001F);
   DOUBLES_EQUAL(6.0F, object_tracks[0].vcs_velocity.longitudinal, 0.001F);
   DOUBLES_EQUAL(3.0F, object_tracks[0].vcs_velocity.lateral, 0.001F);
}
/** @}*/

/** \defgroup  F360_Update_Cluster_In_Clutter_Probability
 *  @{
 */

/** \brief
 * Test Update_Cluster_In_Clutter_Probability function directly.
 */
TEST_GROUP(F360_Update_Cluster_In_Clutter_Probability)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Hist_T det_hist;
   F360_Calibrations_T calibrations;
   F360_Globals_T globals;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Host_T host;
   F360_Tracker_Info_T tracker_info;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS];

   /** \setup
    * Initialize tracker calibrations, variant and set up test fixtures
    */
   TEST_SETUP()
   {
      /** \setup
       * Initialize tracker calibrations
       **/
      for(int32_t i = 0; i < 4; i++)
      {
         sensors[0].constant.range_limits[i] = 200.0F;
         sensors[0].constant.fov_min_az_rad[i] = F360_DEG2RAD(-60.0F);
         sensors[0].constant.fov_max_az_rad[i] = F360_DEG2RAD(60.0F);
      }
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      sensors[0].refined.time_since_measurement_s = 0.00F;
      sensors[0].variable.is_valid = true;

      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);

      tracker_info.num_active_clusters = 0;
      tracker_info.num_active_objs = 0;
      (void)memset(tracker_info.active_cluster_ids, 0, sizeof(tracker_info.active_cluster_ids));
      (void)memset(tracker_info.active_obj_ids, 0, sizeof(tracker_info.active_obj_ids));
      (void)memset(clusters, 0, sizeof(clusters));
      (void)memset(detections, 0, sizeof(detections));
      (void)memset(&det_hist, 0, sizeof(det_hist));
   }

   // Define helper functions.
   void setup_cluster_with_clutter_detections(
      F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Props_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      const float xpos, const float ypos,
      const float cluster_det_x, const float cluster_det_y,
      const int num_cluster_dets,
      const int32_t num_clutter_dets_per_quadrant[4],
      rspp_variant_A::RSPP_Detection_Motion_Status_T clutter_motion_status, 
      const float dist_from_cluster)
   {
      int32_t cluster_id = 1;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters] = cluster_id;
      tracker_info.num_active_clusters = 1;

      F360_Cluster_T& cluster = clusters[cluster_id - 1];
      cluster.id = cluster_id;
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_rdotcomp = 0.5F;
      cluster.f_dealiased = true;

      for (int i = 0; i < num_cluster_dets; i++)
      {
         raw_detect_list.number_of_valid_detections = i + 1;
         cluster.detids[i] = i + 1;
         cluster.ndets++;
         cluster.num_types_of_dets[1]++;
         detections[i].cluster_id = cluster_id;
         detections[i].vcs_position.x = cluster_det_x;
         detections[i].vcs_position.y = cluster_det_y;
         detections[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

         raw_detect_list.detections[i].processed.vcs_position_x = cluster_det_x;
         raw_detect_list.detections[i].processed.vcs_position_y = cluster_det_y;
         raw_detect_list.detections[i].raw.sensor_id = 1;
      }
      // Add clutter detections in different quadrants around the cluster
      float offsets[4][2] = {{dist_from_cluster, dist_from_cluster}, //NE
                             {-dist_from_cluster, dist_from_cluster}, //SE
                             {-dist_from_cluster, -dist_from_cluster}, //SW
                             {dist_from_cluster, -dist_from_cluster}}; //NW
      for (int32_t i = 0; i < 4; i++)
      {
         for (int32_t j = 0; j < num_clutter_dets_per_quadrant[i]; j++)
         {
            int32_t det_idx = raw_detect_list.number_of_valid_detections;
            raw_detect_list.number_of_valid_detections++;

            float x_offset = offsets[i][0];
            float y_offset = offsets[i][1];

            raw_detect_list.detections[det_idx].raw.sensor_id = 1;
            raw_detect_list.detections[det_idx].processed.vcs_position_x = xpos + x_offset;
            raw_detect_list.detections[det_idx].processed.vcs_position_y = ypos + y_offset;
            
            detections[det_idx].cluster_id = -1; // Not part of the cluster
            detections[det_idx].vcs_position.x = xpos + x_offset;
            detections[det_idx].vcs_position.y = ypos + y_offset;
            detections[det_idx].motion_status = clutter_motion_status;
         }
      }
      Sort_Detections_Vcs_Long(raw_detect_list);
   }
};

/** \purpose
 * Verify that clutter counter is not updated when cluster has no detections.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_not_update_no_dets)
{
   /** \precond
    * Set up a cluster with 0 detections
    */
   tracker_info.num_active_clusters = 1;
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].ndets = 0;
   clusters[0].num_types_of_dets[0] = 0; // no moving detection
   clusters[0].num_types_of_dets[1] = 0; // no stationary/ambiguous detections
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter not have changed
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter, clusters[0].clutter_counter, "Clutter counter should not be updated for clusters with 0 detections");
}

/** \purpose
 * Verify that clutter counter is not updated when cluster has no stationary detections.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_not_update_no_stationary_dets)
{
   /** \precond
    * Set up a cluster with 1 detection but no stationary detections
    */
   tracker_info.num_active_clusters = 1;
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].ndets = 1;
   clusters[0].num_types_of_dets[0] = 1; // moving detection
   clusters[0].num_types_of_dets[1] = 0; // no stationary/ambiguous detections
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter not have changed
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter, clusters[0].clutter_counter, "Clutter counter should not be updated for clusters with no stationary detections");
}

/** \purpose
 * Verify that clutter counter is not updated when cluster has too low vcs x position.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_not_update_vcs_x_low)
{
   /** \precond
    * Set up a cluster with 
    * - At least 1 detection
    * - At least 1 detection of type stationary/ambiguous
    * - vcs x position below -20.0
    */
   float xpos = -20.1F;
   float ypos = 0.0F;
   tracker_info.num_active_clusters = 1;
   tracker_info.active_cluster_ids[0] = 1;
   clusters[0].id = 1;
   clusters[0].vcs_position_x = xpos;
   clusters[0].vcs_position_y = ypos;
   clusters[0].ndets = 1;
   clusters[0].num_types_of_dets[0] = 0; // no moving detection
   clusters[0].num_types_of_dets[1] = 1; // stationary/ambiguous detection
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to not have changed due to low vcs x position
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter, clusters[0].clutter_counter, "Clutter counter should not be updated for clusters with vcs x position below -20.0");
}

/** \purpose
 * Verify that clutter counter is incremented when cluster has many clutter detections in all 4 quadrants.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_increment_with_dets_in_all_quadrants)
{
   /** \precond
    * Set up a cluster with > 5 clutter detections spread out in all 4 quadrants
    */
   float xpos = 20.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {1, 2, 1, 2}; // total 6 detections
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info, 
      clusters,
      raw_detect_list,
      detections,
      xpos, ypos,
      xpos, ypos,
      1,
      num_clutter_dets_per_quadrant, 
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have increased by 2 due to > 5 detections in > 3 quadrants
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter + 2, clusters[0].clutter_counter, "Clutter counter should have increased by 2 for clusters with >5 clutter detections in all quadrants");
}

/** \purpose
 * Verify that clutter counter is not updated when cluster has many clutter detections in one quadrant.
 * 
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_not_changed_with_dets_in_one_only_quadrant)
{
   /** \precond
    * Set up a cluster with > 5 clutter detections in 1 quadrant
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {6, 0, 0, 0}; // total 6 detections
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info, 
      clusters,
      raw_detect_list,
      detections,
      xpos, ypos,
      xpos, ypos,
      1,
      num_clutter_dets_per_quadrant, 
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to not have changed due to > 5 detections in one quadrant only
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter, clusters[0].clutter_counter, "Clutter counter should not have changed when > 5 clutter detections in one quadrant");
}

/** \purpose
 * Verify that clutter counter is not updated when cluster has more than 3 clutter detections not in opposing quadrants.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_not_changed_with_more_than_three_dets_not_in_opposing_quadrants)
{
   /** \precond
    * Set up a cluster with 4 clutter detections in 2 quadrants not opposing each other
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {2, 2, 0, 0}; // total 6 detections
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info, 
      clusters,
      raw_detect_list,
      detections,
      xpos, ypos,
      xpos, ypos,
      1,
      num_clutter_dets_per_quadrant, 
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to not have changed due to detections not in opposing quadrants
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter, clusters[0].clutter_counter, "Clutter counter should not have changed when clutter detections are not in opposing quadrants");
}

/** \purpose
 * Verify that clutter counter is incremented when cluster has clutter detections in opposing quadrants NE and SW.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_increment_with_dets_in_opposing_quadrants_NE_SW)
{
   /** \precond
    * Set up a cluster with > 3 clutter detections spread out in 2 quadrants NE and SW
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {2, 0, 2, 0};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info, 
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have increased by 1 due to > 3 detections in opposing quadrants
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter + 1, clusters[0].clutter_counter, "Clutter counter should have increased by 1 for clusters with >3 clutter detections in opposing quadrants NE and SW");
}

/** \purpose
 * Verify that clutter counter is incremented when cluster has clutter detections in opposing quadrants SE and NW.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_increment_with_dets_in_opposing_quadrants_SE_NW)
{
   /** \precond
    * Set up a cluster with > 3 clutter detections spread out in 2 quadrants SE and NW
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {0, 2, 0, 2};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info, 
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 0;
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have increased by 1 due to > 3 detections in opposing quadrants
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter + 1, clusters[0].clutter_counter, "Clutter counter should have increased by 1 for clusters with >3 clutter detections in opposing quadrants SE and NW");
}

/** \purpose
 * Verify that clutter counter is decremented when cluster has only 1 clutter detection.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_decrement_one_clutter_det)
{
   /** \precond
    * Set up a cluster with < 2 clutter detections
    * Set clutter_counter to a value > 0
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {1, 0, 0, 0};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 5; // Set initial counter
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have decreased by 1 due to < 2 detections
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter - 1, clusters[0].clutter_counter, "Clutter counter should have decreased by 1 for clusters with <2 clutter detections");
}

/** \purpose
 * Verify that clutter counter is decremented when cluster has moving clutter detections.
 * Moving clutter detections are not counted as clutter.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_decrement_moving_clutter_dets)
{
   /** \precond
    * Set up a cluster with clutter detections
    * Set clutter_counter to a value > 0
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {1, 0, 0, 0};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING,
      dist_from_cluster);
   clusters[0].clutter_counter = 5; // Set initial counter
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have decreased by 1 due to moving clutter detections
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter - 1, clusters[0].clutter_counter, "Clutter counter should have decreased by 1 for clusters with moving clutter detections");
}

/** \purpose
 * Verify that clutter counter is decremented when cluster has clutter detections too far away from cluster.
 * Tests bidirectional sorted detection list navigation (prev and next).
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_decrement_clutter_dets_too_far_away)
{
   /** \precond
    * Set up a cluster with clutter detections too far away from cluster
    * Set clutter_counter to a value > 0
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {1, 0, 1, 0};
   float dist_from_cluster = 5.0F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 5; // Set initial counter
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have decreased by 1 (detections too far away)
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter - 1, clusters[0].clutter_counter, "Clutter counter should have decreased by 1 when detections are beyond max distance");
}

/** \purpose
 * Verify that clutter counter is decremented when cluster has 2 detections associated.
 * Tests that only detections with different cluster_id are counted as clutter.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_not_count_own_cluster_detections)
{
   /** \precond
    * Set up a cluster with 2 associated detections and clutter detections too far away from cluster
    * Set clutter_counter to a value > 0
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {1, 0, 1, 0};
   float dist_from_cluster = 4.1F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      2,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 5; // Set initial counter
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have decreased by 1 (only counting external clutter, not own cluster detections)
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter - 1, clusters[0].clutter_counter, "Clutter counter should have decreased by 1 when only external clutter detections are present");
}

/** \purpose
 * Verify that clutter counter is decremented when clutter detections appear before cluster detection in sorted list.
 * Tests navigation through prev_sorted_idx chain.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_decrement_clutter_dets_before_cluster_detection)
{
   /** \precond
    * Set up a cluster with clutter detections too far away from cluster
    * Set clutter_counter to a value > 0
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {0, 1, 0, 0};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos + 1.0F,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 5; // Set initial counter
   int16_t initial_clutter_counter = clusters[0].clutter_counter;

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to have decreased by 1 (< 2 clutter detections found)
    */
   CHECK_EQUAL_TEXT(initial_clutter_counter - 1, clusters[0].clutter_counter, "Clutter counter should have decreased by 1 when < 2 clutter detections are found");
}

/** \purpose
 * Verify that clutter counter is kept at zero if it would be decremented below 0.
 * Tests minimum saturation boundary.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_decrement_min_value_kept)
{
   /** \precond
    * Set up a cluster with clutter detections and clutter_counter = 0
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {0, 1, 0, 0};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos + 1.0F,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 0; // Set initial counter

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to remain at 0 (not decremented below minimum)
    */
   CHECK_EQUAL_TEXT(0, clusters[0].clutter_counter, "Clutter counter should remain at 0 when it would be decremented below minimum");
}

/** \purpose
 * Verify that clutter counter is not incremented above 10.
 * Tests maximum saturation boundary.
 * \req
 * NA
 */
TEST(F360_Update_Cluster_In_Clutter_Probability, clutter_counter_increment_max_value_saturated)
{
   /** \precond
    * Set up a cluster with many clutter detections in all quadrants
    * Set clutter_counter to 9 (one below maximum)
    */
   float xpos = 10.0F;
   float ypos = 0.0F;
   int32_t num_clutter_dets_per_quadrant[4] = {3, 3, 3, 3};
   float dist_from_cluster = 2.0F;
   setup_cluster_with_clutter_detections(tracker_info,
      clusters,
      raw_detect_list,
      detections,
      xpos,
      ypos,
      xpos,
      ypos,
      1,
      num_clutter_dets_per_quadrant,
      rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY,
      dist_from_cluster);
   clusters[0].clutter_counter = 9; // Set initial counter

   /** \action
    * Call Update_Cluster_In_Clutter_Probability().
    */
   Update_Cluster_In_Clutter_Probability(tracker_info, raw_detect_list, detections, clusters);

   /** \result
    * Expect clutter counter to be saturated at 10 (not incremented above maximum)
    */
   CHECK_EQUAL_TEXT(10, clusters[0].clutter_counter, "Clutter counter should be saturated at maximum value of 10");
}
/** @}*/
