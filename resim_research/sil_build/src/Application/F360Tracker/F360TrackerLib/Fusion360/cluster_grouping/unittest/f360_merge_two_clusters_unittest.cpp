/** \file
 * This file contains unit tests for content of f360_merge_two_clusters.cpp file
 */

#include "f360_merge_two_clusters.h"
#include "f360_cluster_grouping_data_generator.h"
#include "f360_set_variant.h"

#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_merge_two_clusters_ut
 *  @{
 */

/** \brief
 *  Test group for unitesting Merge_Two_Clusters function.
 */
TEST_GROUP(f360_merge_two_clusters_ut)
{
   /** \setup
   * Setup essential parameters for tesing Merge_Two_Clusters.
   * Initialize tracker calibartions.
   **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calib = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Cluster_T &cluster_older = clusters[0];
   F360_Cluster_T &cluster_newer = clusters[1];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Hist_T detections_hist = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   float32_t rngrate_interval_width_older = 0.0F;
   float32_t rngrate_interval_width_newer = 0.0F;
   float32_t interval_older = 0.0F;
   float32_t interval_newer = 0.0F;

   float32_t tolerance = 1e-3;
   float32_t rdot_interval_width_1 = 69.8359375;
   float32_t rdot_interval_width_2 = 59.6132813;
   float32_t k_max_dealiased_rr_diff = 3.0F;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = rdot_interval_width_1;
      sensors[0].refined.time_since_measurement_s = 0.05F;

      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = rdot_interval_width_2;
      sensors[1].refined.time_since_measurement_s = 0.04F;

      sensors[2].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[2].constant.v_wrapping[0] = -rdot_interval_width_2;
      sensors[2].refined.time_since_measurement_s = 0.04F;

      for (int16_t i = 0; i < NUMBER_OF_CLUSTERS; i++)
      {
         clusters[i].id = i + 1;
      }
   }
};

/**
*\purpose  Testing if dealiased flag for detetions is properly set where all detection from two cluster are valid
*\req    N/A
*/
TEST(f360_merge_two_clusters_ut, Test_Merge_Two_Clusters)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/

   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 0.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 2.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2,0.6F, 0.3F);

   clusters[cluster_older.id - 1].time_since_cluster_updated = 0.1F;
   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *    Checking if all detections from clusters were dealiased
   **/
   CHECK_TRUE(det_props[0].f_dealiased);
   CHECK_TRUE(det_props[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[0].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[2].f_dealiased);
}


/**
*\purpose  Checking if not dealiased detections were not dealiased.
*\req    N/A
*/
TEST(f360_merge_two_clusters_ut, Test_Dectecions_In_Cluster_Shall_Not_Be_Dealiased)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/

   // Will not be dealiased because compensated rr of detection matches compensated  rr of clusters poorly
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);

   // Will not be dealiased because compensated rr of detection matches compensated  rr of clusters poorly
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 10.1F, 0.1F);

   // Will not be dealiased because compensated rr of detection matches compensated  rr of clusters poorly
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 10.1F);

   // Will not be dealiased despite having a compensated range rate that matches clusters well. This is because the interval width is negative which is unexpected (to achieve branch coverage for a "code safety" branch)
   Add_Det(det_props, raw_detection_list, cluster_older, 3, 1.0F);

    // Will not be dealiased despite having a compensated range rate that matches clusters well. This is because the interval width is negative which is unexpected (to achieve branch coverage for a "code safety" branch)
   Add_Hist_Det(detections_hist, cluster_older, -rdot_interval_width_2, 2.1F, 0.2F);

   // Will not be dealiased because compensated  rr of detection matches compensated  rr of clusters poorly
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 19.6F, 0.3F);

   clusters[cluster_older.id - 1].f_dealiased = true;
   clusters[cluster_older.id - 1].rep_rdotcomp = 1.0F;
   clusters[cluster_newer.id - 1].f_dealiased = true;
   clusters[cluster_newer.id - 1].rep_rdotcomp = 1.0F;

   clusters[cluster_older.id - 1].time_since_cluster_updated = 0.1F;
   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *    Checking if no detection from clusters was dealiased.
   **/
   CHECK_FALSE(det_props[0].f_dealiased);
   CHECK_FALSE(det_props[1].f_dealiased);
   CHECK_FALSE(det_props[2].f_dealiased);
   CHECK_FALSE(detections_hist.det_data[0].f_dealiased);
   CHECK_FALSE(detections_hist.det_data[1].f_dealiased);
   CHECK_FALSE(detections_hist.det_data[2].f_dealiased);
}


/**
*\purpose  Test to cover if dealiased flag for detetions is properly set when clusers were previously dealiased.
*          All detectiosn from two clusters supposte to be dealiased. Cluster with oldest update suppose to be killed.
*\req    N/A
*/
TEST(f360_merge_two_clusters_ut, Test_Cluster_Already_Dealiased)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/

   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 59.6F, 0.3F);

   clusters[cluster_older.id - 1].time_since_cluster_updated = 0.1F;

   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *Checking if stationary cluster gets dealiased
   **/
   CHECK_TRUE(det_props[0].f_dealiased);
   CHECK_TRUE(det_props[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[0].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[2].f_dealiased);
   DOUBLES_EQUAL(2.1F, clusters[cluster_older.id - 1].rep_rdotcomp, F360_EPSILON);
   CHECK_TRUE(cluster_newer.f_to_be_killed);
}

/**
*\purpose  Test to cover if dealiased flag for detetions is properly set when clusers were previously dealiased.
*          All detections from two clusters supposte to be dealiased.
*          Newer cluster, which has oldest update suppose to be killed.
*\req    N/A
*/
TEST(f360_merge_two_clusters_ut, Test_Cluster_Already_Dealiased_And_Newer_Is_Killed)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 59.6F, 0.3F);
   detections_hist.det_data[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   clusters[cluster_older.id - 1].f_dealiased = true;
   clusters[cluster_older.id - 1].rep_rdotcomp = 1.0F;
   clusters[cluster_newer.id - 1].f_dealiased = true;
   clusters[cluster_newer.id - 1].rep_rdotcomp = 1.0F;

   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *    Checking if stationary cluster gets dealiased
   *    Newer cluster suppose to have f_to_be_killed_flag_set.
   **/
   CHECK_TRUE(det_props[0].f_dealiased);
   CHECK_TRUE(det_props[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[0].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[2].f_dealiased);
   DOUBLES_EQUAL(1.0F, clusters[cluster_older.id - 1].rep_rdotcomp,F360_EPSILON);
   CHECK_TRUE(cluster_newer.f_to_be_killed);
}

/**
*\purpose  Test to cover To many detections in each cluster.
*\req    N/A
*/
IGNORE_TEST(f360_merge_two_clusters_ut, Test_Too_Many_detections_In_Cluster)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   for (int16_t i = 0; i < 81; i++)
   {
      Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.4F);
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2,0.5F, 0.1F);
   }

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 0.1F, 0.2F);

   raw_detection_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   clusters[cluster_older.id - 1].f_dealiased = true;
   clusters[cluster_older.id - 1].rep_rdotcomp = 1.0F;
   clusters[cluster_newer.id - 1].f_dealiased = true;
   clusters[cluster_newer.id - 1].rep_rdotcomp = 1.0F;

   const uint8_t expected_number_of_detections = 1;
   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *    Checking if number of detections in older cluser is equal to max number of detection in object trk and
   *    expected number of dets in new cluster is 1
   **/
   CHECK_EQUAL(MAX_DETS_IN_OBJ_TRK, cluster_older.ndets);
   CHECK_EQUAL(expected_number_of_detections, cluster_newer.ndets);
}

/**
*\purpose  Test to cover unique time measurement for each detection.
*\req    N/A
*/
TEST(f360_merge_two_clusters_ut, Test_Unique_time_measurement_for_dets)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   for (int16_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.4F);
      detections_hist.det_data[i].time_since_meas = i;
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2,0.5F, 0.1F + 0.01F*static_cast<float32_t>(i));
   }

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 0.1F, 0.2F + 0.01F*static_cast<float32_t>(MAX_DETS_IN_OBJ_TRK));

   raw_detection_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   clusters[cluster_older.id - 1].f_dealiased = true;
   clusters[cluster_older.id - 1].rep_rdotcomp = 1.0F;
   clusters[cluster_newer.id - 1].f_dealiased = true;
   clusters[cluster_newer.id - 1].rep_rdotcomp = 1.0F;

   const uint8_t expected_number_of_detections = 0;
   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *    Checking if number of detections in older cluster is equal to max number of detection in object trk and that
   *    expected number of dets in new cluster is equal to 0
   **/
   CHECK_EQUAL(MAX_DETS_IN_OBJ_TRK, cluster_older.ndets);
   CHECK_EQUAL(expected_number_of_detections, cluster_newer.ndets);
}

/**
*\purpose  Test to cover unique time measurement for each tedection.
*\req    N/A
*/
TEST(f360_merge_two_clusters_ut, Test_Dets_with_unique_measurement_time_number_higher_than_max_num_dets_in_cluster)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   for (int16_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.4F);
      detections_hist.det_data[i].time_since_meas = i;
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2,0.5F, 0.1F);
   }

   for (int16_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      Add_Det(det_props, raw_detection_list, cluster_older, 1, 0.1F);
      detections_hist.det_data[i].time_since_meas = i+0.1F;
      Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 0.1F, 0.2F);
   }
   raw_detection_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   clusters[cluster_older.id - 1].f_dealiased = true;
   clusters[cluster_older.id - 1].rep_rdotcomp = 1.0F;
   clusters[cluster_newer.id - 1].f_dealiased = true;
   clusters[cluster_newer.id - 1].rep_rdotcomp = 1.0F;

   const uint8_t expected_number_of_detections = 0;
   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *    Checking if number of detections in older cluster is equal to 0 and that
   *    expected number of dets in new cluster is  max number of detection in object trk
   **/
   CHECK_EQUAL(expected_number_of_detections, cluster_older.ndets);
   CHECK_EQUAL(MAX_DETS_IN_OBJ_TRK, cluster_newer.ndets);
}

/** @}*/
