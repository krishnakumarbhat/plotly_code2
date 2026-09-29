/** \file
 * This file aims to test sensor_postprocessing function in a pure unit test methode.
 */

#include "f360_sensor_postprocessing.h"
#include "f360_set_variant.h"
#include "f360_iterator.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup f360_Find_Next_Hist_Det_Idx
 *  @{
 */
/** \brief
*  This test suit aims  to test Find_Next_Hist_Det_Idx function
* Note: other sensor_postprocessing functions are being tested at the f360_sensor_postprocessing_qualtest_unittest.cpp file.
**/
TEST_GROUP(f360_Find_Next_Hist_Det_Idx)
{
    //set up variables for sensor postprocessing
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T     clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T    timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   float32_t test_tolerance = 1.0e-6F;

   /** \setup
    * Set common variables used in all tests.
    * Setup one detection and its properties as follows:
    *    range_rate_dealiased to 0.5F
    *    vcs_az to 0.2F
    *    time_since_measurement to 0.01F
    **/
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. exact values are not important
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      tracker_info.num_active_clusters = 1;
      clusters[0].ndets=1;
      clusters[0].detids[0]=1;
      tracker_info.active_cluster_ids[0]=1;
      det_hist.n_occupied=1;
      raw_detection_list.number_of_valid_detections = 1;
      raw_detection_list.detections[0].raw.det_id = 1;
      raw_detection_list.detections[0].raw.sensor_id = 1;
      raw_detection_list.detections[0].processed.f_ok_to_use = true;
      raw_detection_list.detections[0].processed.vcs_az = 0.2F;

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = 1.0F;
      sensors[0].constant.r_wrapping[0] = 1.0F;
      sensors[0].refined.time_since_measurement_s = 0.01F;
   }
};
/**
*\purpose  Verify that detection history max occupation is updated correctly
*\req    NA
*/
TEST(f360_Find_Next_Hist_Det_Idx, Test_Find_Next_Hist_Det_Idx_max_occupation_lower_than_n_occupied)
{
   /** \precond
    *- Set the detection history n_occupied higher than max occupation
    **/
   det_hist.n_occupied=3;
   det_hist.max_occupation=2;
   int expected_max_occupation_value = 4; // because n_occupied will be incremented by 1

   /** \action
    *-Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    *- detection history max occupation is updated with the higher value
    **/
   DOUBLES_EQUAL(expected_max_occupation_value,det_hist.max_occupation,test_tolerance)
}
/**
*\purpose  Verify that detection history max occupation is updated correctly
*\req    NA
*/
TEST(f360_Find_Next_Hist_Det_Idx, Test_Find_Next_Hist_Det_Idx_max_occupation_greater_than_n_occupied )
{
   /** \precond
    *- Set the detection history number of occupation lower than max occupation
    **/
   det_hist.n_occupied=1;
   det_hist.max_occupation=2;
   int expected_max_occupation_value = 2;

   /** \action
    *-Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    *- detection history max occupation is updated with the higher value
    **/
   DOUBLES_EQUAL(expected_max_occupation_value,det_hist.max_occupation,test_tolerance)
}
/**
*\purpose Verify that no new detections is added when there is no empty space in detections history
*\req   NA
*/
TEST(f360_Find_Next_Hist_Det_Idx, Test_Find_Next_Hist_Det_Idx_no_empty_space)
{
   /** \precond
    *- Set all the detection history f_idx_occupied to true, leave no empty spaces
   **/
   det_hist.n_occupied=1;
   for (uint32_t k = 0U; (k < MAX_NUMBER_OF_HISTORIC_DETECTIONS);k++)
   {
   det_hist.f_idx_occupied[k]=true;
   }
   float expected_rdot_history_data = 0;
   float expected_vcs_az_history_data = 0;
   float expected_time_since_meas_history_data = 0;

   /** \action
    *-Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    *- no new detection is added
    **/
   DOUBLES_EQUAL(expected_rdot_history_data,det_hist.det_data[0].rdot ,test_tolerance)
   DOUBLES_EQUAL(expected_vcs_az_history_data,det_hist.det_data[0].vcs_az ,test_tolerance)
   DOUBLES_EQUAL(expected_time_since_meas_history_data,det_hist.det_data[0].time_since_meas ,test_tolerance)
}

/**
*\purpose Verify that no new detections is added when there is no empty space in detections history
*\req   NA
*/
TEST(f360_Find_Next_Hist_Det_Idx, Test_to_many_detections_unique_time)
{
   /** \precond
    *- Set all the detection history f_idx_occupied to true, leave no empty spaces
   **/
   det_hist.n_occupied=MAX_NUMBER_OF_HISTORIC_DETECTIONS;
   clusters[0].ndets = 1;

   for (uint32_t k = 0U; (k < MAX_NUMBER_OF_HISTORIC_DETECTIONS);k++)
   {
      det_hist.f_idx_occupied[k]=true;
      det_hist.det_data[k].time_since_meas = 1;
   }
   float expected_rdot_history_data = 0;
   float expected_vcs_az_history_data = 0;
   float expected_time_since_meas_history_data = 1;

   /** \action
    *-Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    *- no new detection is added
    **/
   DOUBLES_EQUAL(expected_rdot_history_data,det_hist.det_data[0].rdot ,test_tolerance)
   DOUBLES_EQUAL(expected_vcs_az_history_data,det_hist.det_data[0].vcs_az ,test_tolerance)
   DOUBLES_EQUAL(expected_time_since_meas_history_data,det_hist.det_data[0].time_since_meas ,test_tolerance)
}

/** \defgroup  Mark_Long_Coasting_Clusters_To_Be_Killed
 *  @{
 */
/** \brief
* This test suit aims to test the functionality which should remove historical detections from clusters
* that have been coasting for a long time and mark them as f_to_be_killed 
**/
TEST_GROUP(Mark_Long_Coasting_Clusters_To_Be_Killed)
{
   // Set up variables for sensor postprocessing call
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   const float32_t allowed_coasting_time = 0.375F; // This should correspond to the expected allawed maximum coasting time of clusters

   /** \setup
    * Set up a default scenario with
    *    - some valid new detections
    *    - one cluster with one associated historic detection but no new detection associations
    * The historic detection should be recent (i.e. its time since measurement
    * should be small). This is to ensure that the cluster is not killed due to
    * removing all its old detections but actually from coasting.
    **/
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. exact values are not important
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      // Set up tracker info to contain one active cluster
      tracker_info.num_active_clusters = 1;
      const int16_t cluster_id = 12;
      tracker_info.active_cluster_ids[0] = cluster_id;

      // Set up a coasting cluster with 1 historical detection and 0 new detections.
      // The cluster is set up to have a coasting time that is too short for it to be killed.
      const int16_t cluster_idx = cluster_id - 1;
      clusters[cluster_idx].f_to_be_killed = false;
      clusters[cluster_idx].time_since_cluster_updated = allowed_coasting_time - F360_EPSILON;
      clusters[cluster_idx].ndets = 0;
      clusters[cluster_idx].detids[0] = 0;
      clusters[cluster_idx].num_old_dets = 1;
      const int16_t hist_det_idx = 5;
      clusters[cluster_idx].old_det_idx[0] = hist_det_idx;
      for(int32_t i = 1; i < 40; i++)
      {
        clusters[cluster_idx].old_det_idx[1] = -1; // Fill all elements with invalid numbers
      }

      // Set up the historical detection
      det_hist.n_occupied = 1;
      det_hist.f_idx_occupied[hist_det_idx] = true;
      det_hist.det_data[hist_det_idx].time_since_meas = 0.0F; // Small to make sure cluster is not killed do to all its historical detections being too old

      // Set up some valid new detections
      raw_detection_list.number_of_valid_detections = 10U;
      for(uint16_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
      {
         raw_detection_list.detections[det_idx].processed.f_ok_to_use = true;
         raw_detection_list.detections[det_idx].raw.sensor_id = 1;
      }
      sensors[1].refined.time_since_measurement_s = 0.0F;
   }
};

/**
*\purpose  Verify that the cluster is not being killed when its coasting time is too short
*\req    NA
*/
TEST(Mark_Long_Coasting_Clusters_To_Be_Killed, Test_Not_Killed_When_Coasting_Short_Time)
{
   /** \precond
    * Use the deafult setup from test group whete the cluster coasting time is set to just below
    * the threshold for being killed due to coasting.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is still alive and is still associated to its historical detection
    **/
   CHECK_FALSE_TEXT(clusters[11].f_to_be_killed, "Cluster is killed despite being set up to stay alive.");
   const bool f_historical_detection_still_alive = (clusters[11].num_old_dets == 1) && (clusters[11].old_det_idx[0] == 5) && (det_hist.f_idx_occupied[5]) && (det_hist.n_occupied == 1);
   CHECK_TRUE_TEXT(f_historical_detection_still_alive, "The historical detection is being killed despite being set up to stay alive");
}

/**
*\purpose  Verify that the cluster is being killed when its coasting time is too long
*\req    NA
*/
TEST(Mark_Long_Coasting_Clusters_To_Be_Killed, Test_Killed_When_Coasting_Long_Time)
{
   /** \precond
    * Set the coasting time of the cluster to be long (Note: Number of current detections of
    * the cluster has already been set to 0 in test group).
    **/
   clusters[11].time_since_cluster_updated = allowed_coasting_time + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is marked as f_to_be_killed and that its historical detection has been removed
    **/
   CHECK_TRUE_TEXT(clusters[11].f_to_be_killed, "Cluster is not marked as f_to_be_killed despite being set up to be killed.");
   const bool f_historical_detection_properly_removed = (clusters[11].num_old_dets == 0) && (clusters[11].old_det_idx[0] == 0) && (!det_hist.f_idx_occupied[5]) && (det_hist.n_occupied == 0);
   CHECK_TRUE_TEXT(f_historical_detection_properly_removed, "The historical detection of the cluster has not been properly removed");
}

/**
*\purpose  Verify that the cluster is not being killed when its coasting time is too long but the cluster is associated to a new detection
*\req    NA
*/
TEST(Mark_Long_Coasting_Clusters_To_Be_Killed, Test_NOT_Killed_When_Coasting_Long_Time_But_Has_New_Det)
{
   /** \precond
    * Set the coasting time of the cluster to be long.
    * Associate the cluster to one new detection.
    **/
   clusters[11].time_since_cluster_updated = allowed_coasting_time + F360_EPSILON;
   clusters[11].ndets = 1;
   clusters[11].detids[0] = 1;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is still alive and is still associated to its first historical detection and that its new detections has also been moved to the historical detection buffer
    **/
   CHECK_FALSE_TEXT(clusters[11].f_to_be_killed, "Cluster is not marked as f_to_be_killed despite being set up to be killed.");
   const bool f_num_hist_dets_correct = (clusters[11].num_old_dets == 2) && (det_hist.n_occupied == 2);
   CHECK_TRUE_TEXT(f_num_hist_dets_correct, "Number of historical detections is incorrect");
   const bool f_prev_historical_detection_still_alive = (det_hist.f_idx_occupied[5]) && (clusters[11].old_det_idx[0] == 5);
   CHECK_TRUE_TEXT(f_prev_historical_detection_still_alive, "The historical detection of the cluster is still kept");
   const bool f_new_det_moved_to_hist_det = (clusters[11].old_det_idx[1] != -1) && (det_hist.f_idx_occupied[clusters[11].old_det_idx[1]]);

   CHECK_TRUE_TEXT(f_new_det_moved_to_hist_det, "The new detection has not been moved to historical detection buffer properly");
}
/** @}*/


/** \defgroup  Remove_Old_Dets_From_Clusters
 *  @{
 */
/** \brief
* This test suit aims to test the functionality which should remove too old historical detections from clusters.
**/
TEST_GROUP(Remove_Old_Dets_From_Clusters)
{
   // Set up variables for sensor postprocessing call
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   /** \setup
    * Set up a default scenario with
    *    - 1 valid new detection
    *    - 1 cluster with one associated historic detection but no new detection associations
    * The cluster time_since_updated should be small. This is to ensure that the cluster is not
    * killed due to coasting for too long.
    **/
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. Exact values are not important
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      // Set up tracker info to contain one active cluster
      tracker_info.num_active_clusters = 1;
      const int16_t cluster_id = 5;
      tracker_info.active_cluster_ids[0] = cluster_id;

      // Set up a cluster with 1 historical detection and 0 new detections.
      // The cluster is set up to have small timesince_updated time such that it is not killed due to coasting
      const int16_t cluster_idx = cluster_id - 1;
      clusters[cluster_idx].f_to_be_killed = false;
      clusters[cluster_idx].time_since_cluster_updated = 0.0F;
      clusters[cluster_idx].ndets = 0;
      clusters[cluster_idx].detids[0] = 0;
      clusters[cluster_idx].num_old_dets = 1;
      const int16_t hist_det_idx = 2;
      clusters[cluster_idx].old_det_idx[0] = hist_det_idx;
      for(int32_t i = 1; i < 40; i++)
      {
        clusters[cluster_idx].old_det_idx[1] = -1; // Fill all elements with invalid numbers
      }

      // Set up the historical detection
      det_hist.n_occupied = 1;
      det_hist.f_idx_occupied[hist_det_idx] = true;
      det_hist.det_data[hist_det_idx].cluster_idx = cluster_idx;
      // rest of the properties will be set for each of the specific tests

      // Set up one valid new detections
      raw_detection_list.number_of_valid_detections = 1U;
      raw_detection_list.detections[0].processed.f_ok_to_use = true;
      raw_detection_list.detections[0].raw.sensor_id = 1;
      sensors[1].refined.time_since_measurement_s = 0.0F;
   }
};

/**
*\purpose Test that for slow moving clusters, close ambigous detetions in all radar looks are not killed when below the age of 0.3s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Amb_Close_AllLooks__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement below 0.3s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.3F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
    * Check such that the cluster is still alive and that the historical detection is not killed
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for slow moving clusters, close ambigous detetions in all radar looks are killed when above the age of 0.3s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Amb_Close_AllLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.3s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.3F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that cluster is not killed if one of its detections are killed but it still has one remaining historical detection
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Not_Killed_When_Has_Remaing_Hist_Dets)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to (set up to be killed)
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.3s
    * Add one additional historical detection with very small time_since_meas to make sure it is not being killed
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.3F + F360_EPSILON;

   clusters[4].num_old_dets = 2;
   const int16_t hist_det_idx = 7;
   clusters[4].old_det_idx[1] = hist_det_idx;
   det_hist.n_occupied = 2;
   det_hist.f_idx_occupied[hist_det_idx] = true;
   det_hist.det_data[hist_det_idx].cluster_idx = 4;
   det_hist.det_data[hist_det_idx].time_since_meas = 0.0F;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is not killed despite one of its detections being killed
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(7, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
   CHECK_TRUE(det_hist.f_idx_occupied[7]);
   CHECK_EQUAL(4, det_hist.det_data[7].cluster_idx);
}

/**
*\purpose Test that cluster is not killed if its only historical detection is killed but it still has a current detection associated
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Not_Killed_When_Has_Remaing_Current_Dets)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to (set up to be killed)
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.3s
    * Add one additional current associated detection
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.3F + F360_EPSILON;

   clusters[4].ndets = 1;
   clusters[4].detids[0] = 1; // Note this detetion was already set up to be valid in the test setup

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is not killed all of its historicla detectins are being killed since it has a current detection still.
    * The first hist det should be killed and the current detection should be moved to the hist det buffer so in total there should be one hist det.
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_FALSE(clusters[4].old_det_idx[0] == 2); // Not the first hist det
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]); // The first hist det should be killed
}

/**
*\purpose Test that close ambigous detetions in all radar looks are killed when above the age of 0.3s even when the cluster is moving fast.
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Fast_Det_Amb_Close_AllLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate above 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.3s
    **/
   clusters[4].rep_rdotcomp = 10.0F + 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.3F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that for a slow moving cluster, far away ambigous detetions in all radar looks are not killed when below the age of 0.75s 
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Amb_Far_AllLooks__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host above 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement below 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F + 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.75F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
   * Check such that the cluster is still alive and that the historical detection is not killed
   **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for slow moving clusters, far away ambigous detetions in all radar looks are killed when above the age of 0.75s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Amb_Far_AllLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host avove 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.75F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that for a slow moving cluster, close ambigous detetions in few radar looks are not killed when below the age of 0.75s 
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Amb_Close_FewLooks__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks false
    * - time_since_measurement below 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = false;
   det_hist.det_data[2].time_since_meas = 0.75F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
   * Check such that the cluster is still alive and that the historical detection is not killed
   **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for slow moving clusters, close ambigous detetions in few radar looks are killed when above the age of 0.75s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Amb_Close_FewLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: AMBIGOUS
    * - distance from host below 150m
    * - f_is_range_in_all_looks false
    * - time_since_measurement above 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].vcs_position_x = 150.0F - 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = false;
   det_hist.det_data[2].time_since_meas = 0.75F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}


/**
*\purpose Test that for slow moving clusters, close moving detetions in all radar looks are not killed when below the age of 0.6s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Mov_Close_AllLooks__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: Moving
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement below 0.6s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].vcs_position_x = 150.0F - 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.6F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
    * Check such that the cluster is still alive and that the historical detection is not killed
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for slow moving clusters, close moving detetions in all radar looks are killed when above the age of 0.6s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Mov_Close_AllLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: MOVING
    * - distance from host below 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.6s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].vcs_position_x = 150.0F - 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.6F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that for slow moving clusters, far away moving detetions in all radar looks are not killed when below the age of 0.75s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Mov_Far_AllLooks__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: Moving
    * - distance from host above 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement below 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].vcs_position_x = 150.0F + 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.75F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
    * Check such that the cluster is still alive and that the historical detection is not killed
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for slow moving clusters, far away moving detetions in all radar looks are killed when above the age of 0.75s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Mov_Far_AllLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: MOVING
    * - distance from host above 150m
    * - f_is_range_in_all_looks true
    * - time_since_measurement above 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].vcs_position_x = 150.0F + F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = true;
   det_hist.det_data[2].time_since_meas = 0.75F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that for slow moving clusters, close moving detetions in few radar looks are not killed when below the age of 0.75s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Mov_Close_FewLooks__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: Moving
    * - distance from host below 150m
    * - f_is_range_in_all_looks false
    * - time_since_measurement below 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].vcs_position_x = 150.0F - F360_EPSILON;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = false;
   det_hist.det_data[2].time_since_meas = 0.75F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
    * Check such that the cluster is still alive and that the historical detection is not killed
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for slow moving clusters, close moving detetions in few radar looks are killed when above the age of 0.75s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Slow_Det_Mov_Close_FewLooks__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate below 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: MOVING
    * - distance from host below 150m
    * - f_is_range_in_all_looks false
    * - time_since_measurement above 0.75s
    **/
   clusters[4].rep_rdotcomp = 10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].vcs_position_x = 150.0F - 0.1F;
   det_hist.det_data[2].vcs_position_y = 0.0F;
   det_hist.det_data[2].f_is_range_in_all_looks = false;
   det_hist.det_data[2].time_since_meas = 0.75F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that for fast moving clusters, moving detetions are not killed when below the age of 0.9s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Fast_Det_Mov__Det_Not_Removed_When_Recent)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate above 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: Moving
    * - time_since_measurement below 0.9s
    **/
   clusters[4].rep_rdotcomp = -10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].time_since_meas = 0.9F - F360_EPSILON;


   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);


   /** \result
    * Check such that the cluster is still alive and that the historical detection is not killed
    **/
   CHECK_FALSE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(1, clusters[4].num_old_dets);
   CHECK_EQUAL(2, clusters[4].old_det_idx[0]);
   CHECK_EQUAL(1, det_hist.n_occupied);
   CHECK_TRUE(det_hist.f_idx_occupied[2]);
   CHECK_EQUAL(4, det_hist.det_data[2].cluster_idx);
}

/**
*\purpose Test that for fast moving clusters, moving detetions are killed when above the age of 0.9s
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Fast_Det_Mov__Det_Removed_When_Old)
{
   /** \precond
    * Set the cluster according to
    * - magnitude of compensated range rate above 10m/s
    * Set the historical detection of cluster according to 
    * - motion status: MOVING
    * - time_since_measurement above 0.9s
    **/
   clusters[4].rep_rdotcomp = -10.0F - 0.1F;

   det_hist.det_data[2].motion_status = F360_DET_MOTION_MOVING;
   det_hist.det_data[2].time_since_meas = 0.9F + F360_EPSILON;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the cluster is killed and that the historical detection is killed
    **/
   CHECK_TRUE(clusters[4].f_to_be_killed);
   CHECK_EQUAL(0, clusters[4].num_old_dets);
   CHECK_EQUAL(0, det_hist.n_occupied);
   CHECK_FALSE(det_hist.f_idx_occupied[2]);
}

/**
*\purpose Test that cluster num_types_of_dets are correctly updated when detections are removed
*\req    NA
*/
TEST(Remove_Old_Dets_From_Clusters, Test_Cluster_Num_Types_Of_Dets_Updated)
{
   /** \precond
    * Use cluster settings from default test group (exact setup vakues are not important)
    * Set the cluster num_types_of_dets to [2 2] (i.e. 2 MOVING and 2 AMBIGOUS detections)
    * Assign 4 detections to the cluster with following properties (Note: 1 detections is already assigned from test group so only 3 more needed)
    *    - Detection 1: Motion status AMBIGOUS and time_since_measurement == 0 (to ensure detection IS NOT killed due to being old)
    *    - Detection 2: Motion status AMBIGOUS and time_since_measurement == very large (to ensure detection IS killed due to being old)
    *    - Detection 3: Motion status MOVING and time_since_measurement == 0 (to ensure detection IS NOT killed due to being old)
    *    - Detection 4: Motion status MOVING and time_since_measurement == very large (to ensure detection IS killed due to being old)
    **/
   clusters[4].num_types_of_dets[0] = 2;
   clusters[4].num_types_of_dets[1] = 2;

   // Detection 1: Motion status AMBIGOUS and time_since_measurement == 0 (to ensure detection IS NOT killed due to being old)
   det_hist.det_data[2].motion_status = F360_DET_MOTION_AMBIGUOUS;
   det_hist.det_data[2].time_since_meas = 0.0F;

   // Detection 2: Motion status AMBIGOUS and time_since_measurement == very large (to ensure detection IS killed due to being old)
   int16_t hist_det_idx = 7;
   clusters[4].old_det_idx[clusters[4].num_old_dets] = hist_det_idx;
   clusters[4].num_old_dets++;
   det_hist.n_occupied++;
   det_hist.f_idx_occupied[hist_det_idx] = true;
   det_hist.det_data[hist_det_idx].cluster_idx = 4;
   det_hist.det_data[hist_det_idx].time_since_meas = 100.0F;
   det_hist.det_data[hist_det_idx].motion_status = F360_DET_MOTION_AMBIGUOUS;

   // Detection 3: Motion status MOVING and time_since_measurement == 0 (to ensure detection IS NOT killed due to being old)
   hist_det_idx = 12;
   clusters[4].old_det_idx[clusters[4].num_old_dets] = hist_det_idx;
   clusters[4].num_old_dets++;
   det_hist.n_occupied++;
   det_hist.f_idx_occupied[hist_det_idx] = true;
   det_hist.det_data[hist_det_idx].cluster_idx = 4;
   det_hist.det_data[hist_det_idx].time_since_meas = 0.0F;
   det_hist.det_data[hist_det_idx].motion_status = F360_DET_MOTION_MOVING;

   // Detection 4: Motion status MOVING and time_since_measurement == very large (to ensure detection IS killed due to being old)
   hist_det_idx = 6;
   clusters[4].old_det_idx[clusters[4].num_old_dets] = hist_det_idx;
   clusters[4].num_old_dets++;
   det_hist.n_occupied++;
   det_hist.f_idx_occupied[hist_det_idx] = true;
   det_hist.det_data[hist_det_idx].cluster_idx = 4;
   det_hist.det_data[hist_det_idx].time_since_meas = 100.0F;
   det_hist.det_data[hist_det_idx].motion_status = F360_DET_MOTION_MOVING;

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that 2 detections has been removed from the cluster (i.e. the cluster only has 2 detections left)
    * and that the cluster num_types_of_dets has been correctky updated (expected is for it be [1 1])
    **/
   CHECK_EQUAL(2, clusters[4].num_old_dets);
   CHECK_EQUAL(1, clusters[4].num_types_of_dets[0]);
   CHECK_EQUAL(1, clusters[4].num_types_of_dets[1]);
}



/** @}*/


/** \defgroup  To_Be_Killed_Objects_Are_Cleaned_Of_Detections
 *  @{
 */
/** \brief
* This test suit aims to test the functionality that removes all historical detections and adds no new detections from clusters
* that are marked as f_to_be_killed
**/
TEST_GROUP(To_Be_Killed_Objects_Are_Cleaned_Of_Detections)
{
   // Set up variables for sensor postprocessing call
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   /** \setup
    * Set up one cluster with one historic detection.
    * Let the historic detection have small time_since_meas and the cluster have small
    * time_since_updated to make sure they are not killed due to being too old or coasted for too long.
    * Mark the cluster as f_to_be_killed
    **/
   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. exact values are not important
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      // Set up tracker info to contain one active cluster
      tracker_info.num_active_clusters = 1;
      const int16_t cluster_id = 7;
      tracker_info.active_cluster_ids[0] = cluster_id;

      // Set up a coasting cluster with 1 historical detection and 0 new detections.
      // The cluster is set up to have a coasting time that is too short for it to be killed.
      const int16_t cluster_idx = cluster_id - 1;
      clusters[cluster_idx].f_to_be_killed = true;
      clusters[cluster_idx].time_since_cluster_updated = 0.0F; // Small such that the cluster is not killed due to being coasted for too long
      clusters[cluster_idx].ndets = 1;
      const int32_t new_det_id = 6; // Note: Must be  smaller than 10 since we are only seeting up 10 valid detections here below
      clusters[cluster_idx].detids[0] = new_det_id;
      clusters[cluster_idx].num_old_dets = 1;
      clusters[cluster_idx].num_types_of_dets[0] = 1; // The historical detection is moving (not important for the test if moving or not, just any of it)
      clusters[cluster_idx].num_types_of_dets[1] = 0;
      const int16_t hist_det_idx = 3;
      clusters[cluster_idx].old_det_idx[0] = hist_det_idx;

      // Set up the historical detection
      det_hist.n_occupied = 1;
      det_hist.f_idx_occupied[hist_det_idx] = true;
      det_hist.det_data[hist_det_idx].time_since_meas = 0.0F; // Small to make sure detection is not removed due to being too old
      det_hist.det_data[hist_det_idx].motion_status = 1; // Moving (not improtnat for the test if moving or not, this is just to correspond to the cluster setup)

      // Set up some valid new detections
      raw_detection_list.number_of_valid_detections = 10U;
      for(uint16_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
      {
         raw_detection_list.detections[det_idx].processed.f_ok_to_use = true;
         raw_detection_list.detections[det_idx].raw.sensor_id = 1;
      }
      sensors[1].refined.time_since_measurement_s = 0.0F;
   };
};

/**
*\purpose Make sure clusters marked as f_to_be_killed gets all its historical detections removed and that none of its new
* associated detections gets added to the historical detection buffer
*\req    NA
*/
TEST(To_Be_Killed_Objects_Are_Cleaned_Of_Detections, Test_Cluster_Cleared)
{
   /** \precond
    * Use default settings from the test setup
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all historical detections from the cluster has been removed and that none of its new historical detections
    * has been added to the historical detection buffer
    **/
   CHECK_TRUE_TEXT(clusters[6].f_to_be_killed, "Cluster has been demarked as f_to_be_killed");
   CHECK_EQUAL_TEXT(0, clusters[6].num_old_dets, "The cluster is still associated to one historical detection");
   CHECK_EQUAL_TEXT(0, clusters[6].ndets, "The cluster is still associated to one current detection");
   CHECK_TRUE_TEXT(((0 == clusters[6].num_types_of_dets[0]) && (0 == clusters[6].num_types_of_dets[1])), "The cluster num_types_of_dets has not been cleared");
   bool f_any_f_idx_occupied = false;
   for(int32_t i = 0; i < MAX_NUMBER_OF_HISTORIC_DETECTIONS; i++)
   {
      if(det_hist.f_idx_occupied[i])
      {
         f_any_f_idx_occupied = true;
         break;
      }
   }
   CHECK_TRUE_TEXT(((det_hist.n_occupied == 0) || f_any_f_idx_occupied), "The historical detection buffer still contains some detections from the killed cluster");
}

/** @}*/


/** \defgroup  Update_Detection_History_Hist_Det_Buffer_Not_Saturated
 *  @{
 */
/** \brief
* This test suit aims to test the functionality of Update_Detection_History_Hist that moves new detections to the historical detection buffer when
* there is no risk of saturating the historical detection buffer. I.e. all clusters should be able to add detections
* to the historical detection buffer.
**/
TEST_GROUP(Update_Detection_History_Hist_Det_Buffer_Not_Saturated)
{
   // Set up variables for sensor postprocessing call
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   /** \setup
    * Set up the following clusters
    *    - Cluster 1: Only new detections, no old detections. Number of new detections are maxed out
    *    - Cluster 2: Only old detections, no new detections. Number of old detections are maxed out
    *    - Cluster 3: Both new and old detections, total sum of detections does not exceed max number of old detections for a single cluster
    *    - Cluster 4: Both new and old detections. Number of old detections are already maxed out.
    *    - Cluster 5: Both new and old detections. Number of old detections are not maxed out but the sum of new+old exceeds the max number of old detections for a single cluster
    * Let all the historic detection have small time_since_meas and all the cluster have small
    * time_since_updated to make sure they are not killed due to being too old or coasted for too long.
    * Let part of the detections have motion status AMBIGOUS and part of it have motions status MOVING (not improtant exactly which detections that have which status. Just need to have some of both)
    * Mark the cluster as f_to_be_killed == false
    **/
   TEST_SETUP()
   {
            // Clear tracker info, clusters, det_hist and detections
      (void)memset(&(tracker_info), 0, sizeof(tracker_info));
      (void)memset(&(clusters), 0, sizeof(clusters));
      (void)memset(&(det_hist), 0, sizeof(det_hist));
      (void)memset(&(raw_detection_list), 0, sizeof(raw_detection_list));
      
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. Exact values are not important
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      // Cluster 1: Only new detections, no old detections. Number of new detections are maxed out
      const int16_t num_new_dets_cluster1 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster1 = 0;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster1, num_hist_dets_cluster1, tracker_info, clusters, det_hist, raw_detection_list);

      // Cluster 2: Only old detections, no new detections. Number of old detections are maxed out
      const int16_t num_new_dets_cluster2 = 0;
      const int16_t num_hist_dets_cluster2 = tracker_info.variant.num_hist_dets_in_cluster;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster2, num_hist_dets_cluster2, tracker_info, clusters, det_hist, raw_detection_list);

      // Cluster 3: Both new and old detections, total sum of detections does not exceed max number of old detections for a single cluster
      const int16_t num_new_dets_cluster3 = 5;
      const int16_t num_hist_dets_cluster3 = 3;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster3, num_hist_dets_cluster3, tracker_info, clusters, det_hist, raw_detection_list);

      // Cluster 4: Both new and old detections. Number of old detections are already maxed out.
      const int16_t num_new_dets_cluster4 = 6;
      const int16_t num_hist_dets_cluster4 = tracker_info.variant.num_hist_dets_in_cluster;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster4, num_hist_dets_cluster4, tracker_info, clusters, det_hist, raw_detection_list);

      // Cluster 5: Cluster 5: Both new and old detections. Number of old detections are not maxed out but the sum of new+old exceeds the max number of old detections for a single cluster
      const int16_t num_new_dets_cluster5 = tracker_info.variant.num_dets_in_track - 2;
      const int16_t num_hist_dets_cluster5 = tracker_info.variant.num_hist_dets_in_cluster - 10;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster5, num_hist_dets_cluster5, tracker_info, clusters, det_hist, raw_detection_list);
      

      // Set up sensor timestamp
      sensors[1].refined.time_since_measurement_s = 0.0F;
   };

   void Set_Up_Cluster_And_Detections(
      const int16_t num_new_dets_for_new_cluster,
      const int16_t num_hist_dets_for_new_cluster,
      F360_Tracker_Info_T & tracker_info,
      F360_Cluster_T (&clusters) [NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & det_hist,
      rspp_variant_A::RSPP_Detection_List_T & raw_detection_list)
   {
      tracker_info.num_active_clusters++;
      const int16_t cluster_id = tracker_info.num_active_clusters;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters-1] = cluster_id;
      
      const int16_t cluster_idx = cluster_id - 1;
      clusters[cluster_idx].id = cluster_id;
      clusters[cluster_idx].f_to_be_killed = false;
      clusters[cluster_idx].time_since_cluster_updated = 0.0F; // Small such that the cluster is not killed due to being coasted for too long
      clusters[cluster_idx].ndets = num_new_dets_for_new_cluster;
      clusters[cluster_idx].num_types_of_dets[0] = 0; // Init to zero and fill later inside of the loop
      clusters[cluster_idx].num_types_of_dets[1] = 0; // Init to zero and fill later inside of the loop
      std::fill(cmn::begin(clusters[cluster_idx].detids), cmn::end(clusters[cluster_idx].detids), static_cast<int16_t>(-1)); // Fill with invalid values
      for(int16_t i = 0; i < num_new_dets_for_new_cluster; i++)
      {
         const int16_t det_id = static_cast<int16_t>(raw_detection_list.number_of_valid_detections + i + 1);
         clusters[cluster_idx].detids[i] = det_id;
         if( (i % 2))
         {
            clusters[cluster_idx].num_types_of_dets[0]++;
         }
         else
         {
            clusters[cluster_idx].num_types_of_dets[1]++;
         }
      }
      clusters[cluster_idx].num_old_dets = num_hist_dets_for_new_cluster;
      std::fill(cmn::begin(clusters[cluster_idx].old_det_idx), cmn::end(clusters[cluster_idx].old_det_idx), static_cast<int16_t>(-1)); // Fill with invalid values
      for(int16_t i = 0; i < num_hist_dets_for_new_cluster; i++)
      {
         const int16_t hist_det_idx = det_hist.n_occupied + i;
         clusters[cluster_idx].old_det_idx[i] = hist_det_idx;
         if( (i % 2))
         {
            clusters[cluster_idx].num_types_of_dets[0]++;
         }
         else
         {
            clusters[cluster_idx].num_types_of_dets[1]++;
         }
      }

      // Set up the new detections
      for(int16_t i = 0; i < num_new_dets_for_new_cluster; i++)
      {
         const int16_t det_idx = raw_detection_list.number_of_valid_detections;
         raw_detection_list.detections[det_idx].raw.det_id = det_idx + 1;
         raw_detection_list.detections[det_idx].processed.f_ok_to_use = true;
         raw_detection_list.detections[det_idx].raw.sensor_id = 1;
         if( (i % 2))
         {
            raw_detection_list.detections[det_idx].processed.motion_status = F360_DET_MOTION_MOVING;
         }
         else
         {
            raw_detection_list.detections[det_idx].processed.motion_status = F360_DET_MOTION_AMBIGUOUS;
         }
         raw_detection_list.number_of_valid_detections++;
      }

      // Set up the historical detections
      for(int16_t i = 0; i < num_hist_dets_for_new_cluster; i++)
      {
         const int16_t hist_det_idx = det_hist.n_occupied;
         det_hist.f_idx_occupied[hist_det_idx] = true;
         det_hist.det_data[hist_det_idx].cluster_idx = cluster_idx;
         det_hist.det_data[hist_det_idx].time_since_meas = 0.0F; // Small to make sure detection is not removed due to being too old
         if( (i % 2))
         {
             det_hist.det_data[hist_det_idx].motion_status = 1; // Moving
         }
         else
         {
           det_hist.det_data[hist_det_idx].motion_status = 2; // Ambigous
         }
         det_hist.n_occupied++;
      }
   };

   void Check_Cluster(
      const F360_Cluster_T & cluster,
      const F360_Tracker_Info_T & tracker_info,
      const int16_t expected_num_old_dets)
   {
      CHECK_FALSE_TEXT(cluster.f_to_be_killed, "Cluster has been unexpectedly marked as f_to_be_killed");

      bool f_any_remaining_new_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_dets_in_track; i++)
      {
         if(cluster.detids[i] > 0)
         {
            f_any_remaining_new_dets = true;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.ndets == 0) && (!f_any_remaining_new_dets), "The cluster still has some remaining new detections");

      bool f_enough_old_dets = true;
      for(int32_t i = 0; i < expected_num_old_dets; i++)
      {
         if(cluster.old_det_idx[i] < 0)
         {
            f_enough_old_dets = false;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.num_old_dets == expected_num_old_dets) && f_enough_old_dets, "The cluster has too few historical detections");

      CHECK_TRUE_TEXT(((cluster.num_old_dets + cluster.ndets) == (cluster.num_types_of_dets[0] + cluster.num_types_of_dets[1])), "The cluster num_types_of_dets has not been properly updated.");
   
      bool f_correct_linkage_to_hist_det_buffer = true;
      for(int32_t i = 0; i < expected_num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         if(!det_hist.f_idx_occupied[det_idx] || (det_hist.det_data[det_idx].cluster_idx != cluster.id - 1))
         {
            f_correct_linkage_to_hist_det_buffer = false;
            break;
         }
      }
      CHECK_TRUE_TEXT(f_correct_linkage_to_hist_det_buffer, "The linkage bewteen cluster detections and hist det buffer is incorrect");
   };
};

/**
*\purpose Make sure that cluster 1 was able to take up the maximum allowed slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Not_Saturated, Test_Cluster1)
{
   /** \precond
    * Use default settings from the test setup. Cluster 1 has only new detections, no old detections. Number of new detections are maxed out.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that cluster 1 was able to take up the maximum allowed slots in the historical detection buffer and that the new detections has
    * been properly moved to the historical detection buffer
    **/
   const int16_t expected_num_hist_dets = tracker_info.variant.num_hist_dets_in_cluster;
   Check_Cluster(clusters[0], tracker_info, expected_num_hist_dets);
}

/**
*\purpose Make sure that cluster 2 was able to take up the maximum allowed slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Not_Saturated, Test_Cluster2)
{
   /** \precond
    * Use default settings from the test setup. Cluster 2 has only old detections, no new detections. Number of old detections are maxed out
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that cluster 2 was able to take up the maximum allowed slots in the historical detection buffer and that the new detections has
    * been properly moved to the historical detection buffer
    **/
   const int16_t expected_num_hist_dets = tracker_info.variant.num_hist_dets_in_cluster;
   Check_Cluster(clusters[1], tracker_info, expected_num_hist_dets);
}

/**
*\purpose Make sure that cluster 3 is able to keep all of its old and new detections in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Not_Saturated, Test_Cluster3)
{
   /** \precond
    * Use default settings from the test setup. Cluster 3 has both new and old detections, total sum of detections
    * does not exceed max number of old detections for a single cluster.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that cluster 3 was able to keep all of its old and new detections in the historical detection buffer and that the new detections has
    * been properly moved to the historical detection buffer
    **/
   const int16_t expected_num_hist_dets = 8;
   Check_Cluster(clusters[2], tracker_info, expected_num_hist_dets);
}

/**
*\purpose Make sure that cluster 4 was able to take up the maximum allowed slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Not_Saturated, Test_Cluster4)
{
   /** \precond
    * Use default settings from the test setup. Cluster 4 has both new and old detections. Number of old detections are already maxed out.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that cluster 4 was able to take up the maximum allowed slots in the historical detection buffer and that the new detections has
    * been properly moved to the historical detection buffer
    **/
   const int16_t expected_num_hist_dets = tracker_info.variant.num_hist_dets_in_cluster;
   Check_Cluster(clusters[3], tracker_info, expected_num_hist_dets);
}

/**
*\purpose Make sure that cluster 5 was able to take up the maximum allowed slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Not_Saturated, Test_Cluster5)
{
   /** \precond
    * Use default settings from the test setup. Cluster 5 has Both new and old detections. Number of old detections are not
    * maxed out but the sum of new+old exceeds the max number of old detections for a single cluster.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that cluster 5 was able to take up the maximum allowed slots in the historical detection buffer and that the new detections has
    * been properly moved to the historical detection buffer
    **/
   const int16_t expected_num_hist_dets = tracker_info.variant.num_hist_dets_in_cluster;
   Check_Cluster(clusters[4], tracker_info, expected_num_hist_dets);
}

/** @}*/


/** \defgroup  Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets
 *  @{
 */
/** \brief
* This test suit aims to test the functionality of Update_Detection_History_Hist that moves new detections to the historical detection buffer when
* the total number of current and historical detection exceeds the maximum number of detections that can fit into the historical detection buffer.
* In this test group all detections have motion status AMBIGOUS. When detections have same motions status it is easy to know their relative priority.
* Note: This test group only gives code coverage for the part of the code that sets the ambgigous part of the cluster.num_types_dets. Therefore a similar
* test group but for MOVING detections is also implemented in this file
**/
TEST_GROUP(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets)
{
   // Set up variables for sensor postprocessing call
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   const float32_t low_prio_vcs_long_pos = 30.0F;
   const float32_t mid_prio_vcs_long_pos = 20.0F;
   const float32_t high_prio_vcs_long_pos = 10.0F;
   /** \setup
    * Set up the following clusters
    *    - Lowest prio clusters: 12 clusters in total
    *       - Cluster type 1: 11 clusters with maximum number of new and old detections 
    *       - Cluster type 2: 1 cluster with maximum number of new detections but no old detections
    *    - Mid prio clusters: Only 1 cluster in total
    *       - Cluster type 3: 1 cluster with maximum number of new and old detections
    *    - Highest prio clusters: 25 clusters in total
    *       - Cluster type 4: 13 clusters with maximum number of new and old detections
    *       - Cluster type 5: 12 clusters with maximum number of new detections but no old detections
    * 
    * This setup ensures that the historical detection buffer is almost full before calling Sensor_Postprocessing()
    * and that it will be completely full after the call. It also ensures a good mixture of older clusters that have
    * previously been allowed to occupy the historical detection buffer and completely new clusters.
    * 
    * Let all the clusters have the following properties to ensure it is not being killed due to coasting for too long or
    * detections being too old:
    *    - cluster f_to_be_killed == false
    *    - small time_since_updated
    *    - all historical detections have small time_since_measurement
    * 
    * Also let all the clusters have the following properties to make sure that the priority is solely determined by the vcs long pos
    *    - f_dealiased == false
    *    - compensated rr small in magnitude
    *    - no associated old or new moving detections
    *    - vcs lat pos  == 0 (host curvature rear is aet to 0 so lat pos == 0 means in host path)
    * 
    * Let clusters with higest prio have vcs long pos == 10m.
    * Let clusters of mid prio have vcs long pos == 20m
    * Let clusters of lowest prio have vcs long pos = 30m
    * 
    * The expected outcome of this setup is that the 25 higest prio clusters are allowed to occupy max number of slots per cluster each
    * (80) in the historical detection buffer. I.e. in total they occupy 25*80 = 2000 out of 2010 slots. This leaves 10 slots to be
    * occupied by the single mid prio cluster. And the 13 clusters of lowest prio should all be killed and don't occupy any slots in the
    * historical detection buffer.
    **/
   TEST_SETUP()
   {
      // Clear tracker info, clusters, det_hist and detections
      (void)memset(&(tracker_info), 0, sizeof(tracker_info));
      (void)memset(&(clusters), 0, sizeof(clusters));
      (void)memset(&(det_hist), 0, sizeof(det_hist));
      (void)memset(&(raw_detection_list), 0, sizeof(raw_detection_list));
      (void)memset(&(det_props), 0, sizeof(det_props));
      
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. Exact speed value is not important but curveture should be 0 such that host is driving straight
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      // Set up clusters of type 1:  12 clusters of lowest prio with maximum number of new and old detections
      const int16_t num_new_dets_cluster1 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster1 = tracker_info.variant.num_hist_dets_in_cluster;
      const float32_t vcs_long_pos_cluster1 = low_prio_vcs_long_pos;
      for(int16_t i = 0; i < 11; i ++)
      {
         Set_Up_Cluster_And_Detections(num_new_dets_cluster1, num_hist_dets_cluster1, vcs_long_pos_cluster1, tracker_info, clusters, det_hist, raw_detection_list, det_props);
      }

      // Set up clusters of type 2:  1 cluster of lowest prio with maximum number of new detections but no old detections
      const int16_t num_new_dets_cluster2 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster2 = 0;
      const float32_t vcs_long_pos_cluster2 = low_prio_vcs_long_pos;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster2, num_hist_dets_cluster2, vcs_long_pos_cluster2, tracker_info, clusters, det_hist, raw_detection_list, det_props);
      
      // Set up clusters of type 3: 1 cluster of mid prio with maximum number of new detections but no old detections
      const int16_t num_new_dets_cluster3 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster3 = tracker_info.variant.num_hist_dets_in_cluster;
      const float32_t vcs_long_pos_cluster3 = mid_prio_vcs_long_pos;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster3, num_hist_dets_cluster3, vcs_long_pos_cluster3, tracker_info, clusters, det_hist, raw_detection_list, det_props);

      // Set up clusters of type 4: 13 clusters of highest prio with maximum number of new and old detections
      const int16_t num_new_dets_cluster4 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster4 = tracker_info.variant.num_hist_dets_in_cluster;
      const float32_t vcs_long_pos_cluster4 = high_prio_vcs_long_pos;
      for(int16_t i = 0; i < 13; i ++)
      {
         Set_Up_Cluster_And_Detections(num_new_dets_cluster4, num_hist_dets_cluster4, vcs_long_pos_cluster4, tracker_info, clusters, det_hist, raw_detection_list, det_props);
      }

      // Set up clusters of type 5: 12 clusters of highest prio with maximum number of new detections but no old detections
      const int16_t num_new_dets_cluster5 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster5 = 0;
      const float32_t vcs_long_pos_cluster5 = high_prio_vcs_long_pos;
      for(int16_t i = 0; i < 12; i ++)
      {
         Set_Up_Cluster_And_Detections(num_new_dets_cluster5, num_hist_dets_cluster5, vcs_long_pos_cluster5, tracker_info, clusters, det_hist, raw_detection_list, det_props);
      }

      // Set up sensor timestamp
      sensors[1].refined.time_since_measurement_s = 0.0F;
   };

   void Set_Up_Cluster_And_Detections(
      const int16_t num_new_dets_for_new_cluster,
      const int16_t num_hist_dets_for_new_cluster,
      const float32_t vcs_long_pos_for_new_cluster,
      F360_Tracker_Info_T & tracker_info,
      F360_Cluster_T (&clusters) [NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & det_hist,
      rspp_variant_A::RSPP_Detection_List_T & raw_detection_list,
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      tracker_info.num_active_clusters++;
      const int16_t cluster_id = tracker_info.num_active_clusters;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters-1] = cluster_id;
      
      const int16_t cluster_idx = cluster_id - 1;
      clusters[cluster_idx].id = cluster_id;
      clusters[cluster_idx].vcs_position_x = vcs_long_pos_for_new_cluster;
      clusters[cluster_idx].vcs_position_y = 0.0F;
      clusters[cluster_idx].f_to_be_killed = false;
      clusters[cluster_idx].time_since_cluster_updated = 0.0F;
      clusters[cluster_idx].f_dealiased = false;
      clusters[cluster_idx].rep_rdotcomp = 0.0F;
      clusters[cluster_idx].num_types_of_dets[0] = 0;
      clusters[cluster_idx].num_types_of_dets[1] = num_new_dets_for_new_cluster + num_hist_dets_for_new_cluster;
      clusters[cluster_idx].ndets = num_new_dets_for_new_cluster;
      std::fill(cmn::begin(clusters[cluster_idx].detids), cmn::end(clusters[cluster_idx].detids), static_cast<int16_t>(-1)); // Fill with invalid values
      for(int16_t i = 0; i < num_new_dets_for_new_cluster; i++)
      {
         const int32_t new_det_id = raw_detection_list.number_of_valid_detections + i;
         clusters[cluster_idx].detids[i] = new_det_id;
      }
      clusters[cluster_idx].num_old_dets = num_hist_dets_for_new_cluster;
      std::fill(cmn::begin(clusters[cluster_idx].old_det_idx), cmn::end(clusters[cluster_idx].old_det_idx), static_cast<int16_t>(-1)); // Fill with invalid values
      for(int16_t i = 0; i < num_hist_dets_for_new_cluster; i++)
      {
         const int16_t hist_det_idx = det_hist.n_occupied + i;
         clusters[cluster_idx].old_det_idx[i] = hist_det_idx;
      }

      // Set up the new detections
      for(int16_t i = 0; i < num_new_dets_for_new_cluster; i++)
      {
         const int16_t det_idx = raw_detection_list.number_of_valid_detections;
         det_props[det_idx].f_ok_to_use = true;
         det_props[det_idx].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
         raw_detection_list.detections[det_idx].raw.sensor_id = 1;
         raw_detection_list.number_of_valid_detections++;
      }

      // Set up the historical detections
      for(int16_t i = 0; i < num_hist_dets_for_new_cluster; i++)
      {
         const int16_t hist_det_idx = det_hist.n_occupied;
         det_hist.f_idx_occupied[hist_det_idx] = true;
         det_hist.det_data[hist_det_idx].cluster_idx = cluster_idx;
         det_hist.det_data[hist_det_idx].time_since_meas = 0.0F;
         det_hist.det_data[hist_det_idx].motion_status = F360_DET_MOTION_AMBIGUOUS;
         det_hist.n_occupied++;
      }
   };

   void Check_Cluster_Killed(
      const F360_Cluster_T & cluster,
      const F360_Tracker_Info_T & tracker_info)
   {
      CHECK_TRUE_TEXT(cluster.f_to_be_killed, "Cluster is not marked as f_to_be_killed");

      bool f_any_remaining_new_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_dets_in_track; i++)
      {
         if(cluster.detids[i] > 0)
         {
            f_any_remaining_new_dets = true;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.ndets == 0) && (!f_any_remaining_new_dets), "The cluster still has some remaining new detections");

      bool f_any_old_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_hist_dets_in_cluster; i++)
      {
         if(cluster.old_det_idx[i] > 0)
         {
            f_any_old_dets = false;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.num_old_dets == 0) && !f_any_old_dets, "The cluster still has some remaining old detections");
   
      CHECK_TRUE_TEXT(((cluster.num_types_of_dets[0] == 0) && (cluster.num_types_of_dets[1] == 0)), "Cluster num_types_of_dets has not been cleared");

      bool f_no_hist_det_linked_to_cluster = false;
      for(int32_t i = 0; i < tracker_info.variant.num_hist_dets; i++)
      {
         if(det_hist.f_idx_occupied[i] && (det_hist.det_data[i].cluster_idx == (cluster.id - 1)))
         {
            f_no_hist_det_linked_to_cluster = true;
            break;
         }
      }
      CHECK_FALSE_TEXT(f_no_hist_det_linked_to_cluster, "There are some detections in the historical detection buffer which is linked to the killed cluster");
   };

   void Check_Cluster_Alive(
      const F360_Cluster_T & cluster,
      const F360_Tracker_Info_T & tracker_info,
      const int16_t expected_num_old_dets)
   {
      CHECK_FALSE_TEXT(cluster.f_to_be_killed, "Cluster has been unexpectedly marked as f_to_be_killed");

      bool f_any_remaining_new_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_dets_in_track; i++)
      {
         if(cluster.detids[i] > 0)
         {
            f_any_remaining_new_dets = true;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.ndets == 0) && (!f_any_remaining_new_dets), "The cluster still has some remaining new detections");

      bool f_enough_old_dets = true;
      for(int32_t i = 0; i < expected_num_old_dets; i++)
      {
         if(cluster.old_det_idx[i] < 0)
         {
            f_enough_old_dets = false;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.num_old_dets == expected_num_old_dets) && f_enough_old_dets, "The cluster has too few historical detections");

      CHECK_TRUE_TEXT(((cluster.num_types_of_dets[0] == 0) && (cluster.num_types_of_dets[1] == cluster.num_old_dets)), "Cluster num_types_of_dets has not been properly set");
   
      bool f_correct_linkage_to_hist_det_buffer = true;
      for(int32_t i = 0; i < expected_num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         if(!det_hist.f_idx_occupied[det_idx] || (det_hist.det_data[det_idx].cluster_idx != cluster.id - 1))
         {
            f_correct_linkage_to_hist_det_buffer = false;
            break;
         }
      }
      CHECK_TRUE_TEXT(f_correct_linkage_to_hist_det_buffer, "The linkage bewteen cluster detections and hist det buffer is incorrect");
   };
};

/**
*\purpose Make sure all slots in the historical detection buffer has been occupied
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets, Test_Hist_Det_Saturated)
{
   /** \precond
    * Use default settings from the test setup.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the historical detection buffer is saturated
    **/
   CHECK_EQUAL_TEXT(tracker_info.variant.num_hist_dets, det_hist.n_occupied, "det_hist.n_occupied in det hist buffer is too small");

   bool f_all_idx_occupied = true;
   for(int32_t i = 0; i < tracker_info.variant.num_hist_dets; i++)
   {
      if(!det_hist.f_idx_occupied[i])
      {
         f_all_idx_occupied = false;
         break;
      }
   }
   CHECK_TRUE_TEXT(f_all_idx_occupied, "There are unoccupied slots in det_hist.f_idx_occupied array");
}

/**
*\purpose Make sure that low prio clusters of type 1 was marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets, Test_Cluster_Type1)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 1 are of lowest prio with maximum number of new and old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clusters of type 1 are marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
    **/
   for(int16_t i = 0; i < 11; i++)
   {
      Check_Cluster_Killed(clusters[i], tracker_info);
   }
}

/**
*\purpose Make sure that low prio clusters of type 2 was marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets, Test_Cluster_Type2)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 2 are of lowest prio with maximum number of new detections but no old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clustes of type 2 are marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
    **/
   Check_Cluster_Killed(clusters[11], tracker_info);
}

/**
*\purpose Make sure that the mid prio cluster of type 3 is allowed to live on but can only occupy a small number of slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets, Test_Cluster_Typ3)
{
   /** \precond
    * Use default settings from the test setup. The cluster of type 3 are of mid prio with maximum number of new detections but no old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clustes of type 3 is kept alive and that it is allowed to occupy a small number of detections in the historical detection buffer.
    **/
   const int16_t expected_num_old_dets = 10;
   Check_Cluster_Alive(clusters[12], tracker_info, expected_num_old_dets);
}

/**
*\purpose Make sure that the high prio clusters of type 4 are allowed to live on and can occupy the maximum allowed number of slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets, Test_Cluster_Type4)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 4 are of highest prio with maximum number of new and old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clusters of type 4 are are kept alive and that they are allowed to occupy maximum allowed number of slots in the historical detection buffer.
    **/
   const int16_t expected_num_old_dets = tracker_info.variant.num_hist_dets_in_cluster;
   for(int16_t i = 13; i < 26; i++)
   {
      Check_Cluster_Alive(clusters[i], tracker_info, expected_num_old_dets);
   }
}

/**
*\purpose Make sure that the high prio clusters of type 5 are allowed to live on and can occupy the maximum allowed number of slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Amb_Dets, Test_Cluster_Type5)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 5 are of highest prio with maximum number of new detections but no old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clusters of type 5 are are kept alive and that they are allowed to occupy maximum allowed number of slots in the historical detection buffer.
    **/
   const int16_t expected_num_old_dets = tracker_info.variant.num_hist_dets_in_cluster;
   for(int16_t i = 26; i < 38; i++)
   {
      Check_Cluster_Alive(clusters[i], tracker_info, expected_num_old_dets);
   }
}
/** @}*/


/** \defgroup  Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets
 *  @{
 */
/** \brief
* This test suit aims to test the functionality of Update_Detection_History_Hist that moves new detections to the historical detection buffer when
* the total number of current and historical detection exceeds the maximum number of detections that can fit into the historical detection buffer.
* In this test group all detections have motion status MOVING. When detections have same motions status it is easy to know their relative priority.
* Note: This test group only gives code coverage for the part of the code that sets the moving part of the cluster.num_types_dets. Therefore a similar
* test group but for AMBIGOUS detections is also implemented in this file.
**/
TEST_GROUP(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets)
{
   // Set up variables for sensor postprocessing call
   F360_Host_T host_props = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Hist_T det_hist={};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS]={};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]={};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info={};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   const float32_t low_prio_vcs_long_pos = 30.0F;
   const float32_t mid_prio_vcs_long_pos = 20.0F;
   const float32_t high_prio_vcs_long_pos = 10.0F;
   /** \setup
    * Set up the following clusters
    *    - Lowest prio clusters: 12 clusters in total
    *       - Cluster type 1: 11 clusters with maximum number of new and old detections 
    *       - Cluster type 2: 1 cluster with maximum number of new detections but no old detections
    *    - Mid prio clusters: Only 1 cluster in total
    *       - Cluster type 3: 1 cluster with maximum number of new and old detections
    *    - Highest prio clusters: 25 clusters in total
    *       - Cluster type 4: 13 clusters with maximum number of new and old detections
    *       - Cluster type 5: 12 clusters with maximum number of new detections but no old detections
    * 
    * This setup ensures that the historical detection buffer is almost full before calling Sensor_Postprocessing()
    * and that it will be completely full after the call. It also ensures a good mixture of older clusters that have
    * previously been allowed to occupy the historical detection buffer and completely new clusters.
    * 
    * Let all the clusters have the following properties to ensure it is not being killed due to coasting for too long or
    * detections being too old:
    *    - cluster f_to_be_killed == false
    *    - small time_since_updated
    *    - all historical detections have small time_since_measurement
    * 
    * Also let all the clusters have the following properties to make sure that the priority is solely determined by the vcs long pos
    *    - f_dealiased == false
    *    - compensated rr small in magnitude
    *    - no associated old or new ambigous detections
    *    - vcs lat pos  == 0 (host curvature rear is aet to 0 so lat pos == 0 means in host path)
    * 
    * Let clusters with higest prio have vcs long pos == 10m.
    * Let clusters of mid prio have vcs long pos == 20m
    * Let clusters of lowest prio have vcs long pos = 30m
    * 
    * The expected outcome of this setup is that the 25 higest prio clusters are allowed to occupy max number of slots per cluster each
    * (80) in the historical detection buffer. I.e. in total they occupy 25*80 = 2000 out of 2010 slots. This leaves 10 slots to be
    * occupied by the single mid prio cluster. And the 13 clusters of lowest prio should all be killed and don't occupy any slots in the
    * historical detection buffer.
    **/
   TEST_SETUP()
   {
      // Clear tracker info, clusters, det_hist and detections
      (void)memset(&(tracker_info), 0, sizeof(tracker_info));
      (void)memset(&(clusters), 0, sizeof(clusters));
      (void)memset(&(det_hist), 0, sizeof(det_hist));
      (void)memset(&(raw_detection_list), 0, sizeof(raw_detection_list));
      
      Set_Tracker_Variant(tracker_info.variant);

      // Set up host. Exact speed value is not important but curveture should be 0 such that host is driving straight
      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;

      // Set up clusters of type 1:  12 clusters of lowest prio with maximum number of new and old detections
      const int16_t num_new_dets_cluster1 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster1 = tracker_info.variant.num_hist_dets_in_cluster;
      const float32_t vcs_long_pos_cluster1 = low_prio_vcs_long_pos;
      for(int16_t i = 0; i < 11; i ++)
      {
         Set_Up_Cluster_And_Detections(num_new_dets_cluster1, num_hist_dets_cluster1, vcs_long_pos_cluster1, tracker_info, clusters, det_hist, raw_detection_list);
      }

      // Set up clusters of type 2:  1 cluster of lowest prio with maximum number of new detections but no old detections
      const int16_t num_new_dets_cluster2 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster2 = 0;
      const float32_t vcs_long_pos_cluster2 = low_prio_vcs_long_pos;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster2, num_hist_dets_cluster2, vcs_long_pos_cluster2, tracker_info, clusters, det_hist, raw_detection_list);
      
      // Set up clusters of type 3: 1 cluster of mid prio with maximum number of new detections but no old detections
      const int16_t num_new_dets_cluster3 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster3 = tracker_info.variant.num_hist_dets_in_cluster;
      const float32_t vcs_long_pos_cluster3 = mid_prio_vcs_long_pos;
      Set_Up_Cluster_And_Detections(num_new_dets_cluster3, num_hist_dets_cluster3, vcs_long_pos_cluster3, tracker_info, clusters, det_hist, raw_detection_list);

      // Set up clusters of type 4: 13 clusters of highest prio with maximum number of new and old detections
      const int16_t num_new_dets_cluster4 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster4 = tracker_info.variant.num_hist_dets_in_cluster;
      const float32_t vcs_long_pos_cluster4 = high_prio_vcs_long_pos;
      for(int16_t i = 0; i < 13; i ++)
      {
         Set_Up_Cluster_And_Detections(num_new_dets_cluster4, num_hist_dets_cluster4, vcs_long_pos_cluster4, tracker_info, clusters, det_hist, raw_detection_list);
      }

      // Set up clusters of type 5: 12 clusters of highest prio with maximum number of new detections but no old detections
      const int16_t num_new_dets_cluster5 = tracker_info.variant.num_dets_in_track;
      const int16_t num_hist_dets_cluster5 = 0;
      const float32_t vcs_long_pos_cluster5 = high_prio_vcs_long_pos;
      for(int16_t i = 0; i < 12; i ++)
      {
         Set_Up_Cluster_And_Detections(num_new_dets_cluster5, num_hist_dets_cluster5, vcs_long_pos_cluster5, tracker_info, clusters, det_hist, raw_detection_list);
      }

      // Set up sensor timestamp
      sensors[1].refined.time_since_measurement_s = 0.0F;
   };

   void Set_Up_Cluster_And_Detections(
      const int16_t num_new_dets_for_new_cluster,
      const int16_t num_hist_dets_for_new_cluster,
      const float32_t vcs_long_pos_for_new_cluster,
      F360_Tracker_Info_T & tracker_info,
      F360_Cluster_T (&clusters) [NUMBER_OF_CLUSTERS],
      F360_Detection_Hist_T & det_hist,
      rspp_variant_A::RSPP_Detection_List_T & raw_detection_list)
   {
      tracker_info.num_active_clusters++;
      const int16_t cluster_id = tracker_info.num_active_clusters;
      tracker_info.active_cluster_ids[tracker_info.num_active_clusters-1] = cluster_id;
      
      const int16_t cluster_idx = cluster_id - 1;
      clusters[cluster_idx].id = cluster_id;
      clusters[cluster_idx].vcs_position_x = vcs_long_pos_for_new_cluster;
      clusters[cluster_idx].vcs_position_y = 0.0F;
      clusters[cluster_idx].f_to_be_killed = false;
      clusters[cluster_idx].time_since_cluster_updated = 0.0F;
      clusters[cluster_idx].f_dealiased = false;
      clusters[cluster_idx].rep_rdotcomp = 0.0F;
      clusters[cluster_idx].num_types_of_dets[0] = num_new_dets_for_new_cluster + num_hist_dets_for_new_cluster;
      clusters[cluster_idx].num_types_of_dets[1] = 0;
      clusters[cluster_idx].ndets = num_new_dets_for_new_cluster;
      std::fill(cmn::begin(clusters[cluster_idx].detids), cmn::end(clusters[cluster_idx].detids), static_cast<int16_t>(-1)); // Fill with invalid values
      for(int16_t i = 0; i < num_new_dets_for_new_cluster; i++)
      {
         const int32_t new_det_id = raw_detection_list.number_of_valid_detections + i;
         clusters[cluster_idx].detids[i] = new_det_id;
      }
      clusters[cluster_idx].num_old_dets = num_hist_dets_for_new_cluster;
      std::fill(cmn::begin(clusters[cluster_idx].old_det_idx), cmn::end(clusters[cluster_idx].old_det_idx), static_cast<int16_t>(-1)); // Fill with invalid values
      for(int16_t i = 0; i < num_hist_dets_for_new_cluster; i++)
      {
         const int16_t hist_det_idx = det_hist.n_occupied + i;
         clusters[cluster_idx].old_det_idx[i] = hist_det_idx;
      }

      // Set up the new detections
      for(int16_t i = 0; i < num_new_dets_for_new_cluster; i++)
      {
         const int16_t det_idx = raw_detection_list.number_of_valid_detections;
         det_props[det_idx].f_ok_to_use = true;
         det_props[det_idx].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         raw_detection_list.detections[det_idx].raw.sensor_id = 1;
         raw_detection_list.number_of_valid_detections++;
      }

      // Set up the historical detections
      for(int16_t i = 0; i < num_hist_dets_for_new_cluster; i++)
      {
         const int16_t hist_det_idx = det_hist.n_occupied;
         det_hist.f_idx_occupied[hist_det_idx] = true;
         det_hist.det_data[hist_det_idx].cluster_idx = cluster_idx;
         det_hist.det_data[hist_det_idx].time_since_meas = 0.0F;
         det_hist.det_data[hist_det_idx].motion_status = F360_DET_MOTION_MOVING;
         det_hist.n_occupied++;
      }
   };

   void Check_Cluster_Killed(
      const F360_Cluster_T & cluster,
      const F360_Tracker_Info_T & tracker_info)
   {
      CHECK_TRUE_TEXT(cluster.f_to_be_killed, "Cluster is not marked as f_to_be_killed");

      bool f_any_remaining_new_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_dets_in_track; i++)
      {
         if(cluster.detids[i] > 0)
         {
            f_any_remaining_new_dets = true;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.ndets == 0) && (!f_any_remaining_new_dets), "The cluster still has some remaining new detections");

      bool f_any_old_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_hist_dets_in_cluster; i++)
      {
         if(cluster.old_det_idx[i] > 0)
         {
            f_any_old_dets = false;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.num_old_dets == 0) && !f_any_old_dets, "The cluster still has some remaining old detections");
   
      CHECK_TRUE_TEXT(((cluster.num_types_of_dets[0] == 0) && (cluster.num_types_of_dets[1] == 0)), "Cluster num_types_of_dets has not been cleared");

      bool f_no_hist_det_linked_to_cluster = false;
      for(int32_t i = 0; i < tracker_info.variant.num_hist_dets; i++)
      {
         if(det_hist.f_idx_occupied[i] && (det_hist.det_data[i].cluster_idx == (cluster.id - 1)))
         {
            f_no_hist_det_linked_to_cluster = true;
            break;
         }
      }
      CHECK_FALSE_TEXT(f_no_hist_det_linked_to_cluster, "There are some detections in the historical detection buffer which is linked to the killed cluster");
   };

   void Check_Cluster_Alive(
      const F360_Cluster_T & cluster,
      const F360_Tracker_Info_T & tracker_info,
      const int16_t expected_num_old_dets)
   {
      CHECK_FALSE_TEXT(cluster.f_to_be_killed, "Cluster has been unexpectedly marked as f_to_be_killed");

      bool f_any_remaining_new_dets = false;
      for(int32_t i = 0; i < tracker_info.variant.num_dets_in_track; i++)
      {
         if(cluster.detids[i] > 0)
         {
            f_any_remaining_new_dets = true;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.ndets == 0) && (!f_any_remaining_new_dets), "The cluster still has some remaining new detections");

      bool f_enough_old_dets = true;
      for(int32_t i = 0; i < expected_num_old_dets; i++)
      {
         if(cluster.old_det_idx[i] < 0)
         {
            f_enough_old_dets = false;
            break;
         }
      }
      CHECK_TRUE_TEXT((cluster.num_old_dets == expected_num_old_dets) && f_enough_old_dets, "The cluster has too few historical detections");

      CHECK_TRUE_TEXT(((cluster.num_types_of_dets[1] == 0) && (cluster.num_types_of_dets[0] == cluster.num_old_dets)), "Cluster num_types_of_dets has not been properly set");
   
      bool f_correct_linkage_to_hist_det_buffer = true;
      for(int32_t i = 0; i < expected_num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         if(!det_hist.f_idx_occupied[det_idx] || (det_hist.det_data[det_idx].cluster_idx != cluster.id - 1))
         {
            f_correct_linkage_to_hist_det_buffer = false;
            break;
         }
      }
      CHECK_TRUE_TEXT(f_correct_linkage_to_hist_det_buffer, "The linkage bewteen cluster detections and hist det buffer is incorrect");
   };
};

/**
*\purpose Make sure all slots in the historical detection buffer has been occupied
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets, Test_Hist_Det_Saturated)
{
   /** \precond
    * Use default settings from the test setup.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Check such that the historical detection buffer is saturated
    **/
   CHECK_EQUAL_TEXT(tracker_info.variant.num_hist_dets, det_hist.n_occupied, "det_hist.n_occupied in det hist buffer is too small");

   bool f_all_idx_occupied = true;
   for(int32_t i = 0; i < tracker_info.variant.num_hist_dets; i++)
   {
      if(!det_hist.f_idx_occupied[i])
      {
         f_all_idx_occupied = false;
         break;
      }
   }
   CHECK_TRUE_TEXT(f_all_idx_occupied, "There are unoccupied slots in det_hist.f_idx_occupied array");
}

/**
*\purpose Make sure that low prio clusters of type 1 was marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets, Test_Cluster_Type1)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 1 are of lowest prio with maximum number of new and old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clusters of type 1 are marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
    **/
   for(int16_t i = 0; i < 11; i++)
   {
      Check_Cluster_Killed(clusters[i], tracker_info);
   }
}

/**
*\purpose Make sure that low prio clusters of type 2 was marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets, Test_Cluster_Type2)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 2 are of lowest prio with maximum number of new detections but no old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clustes of type 2 are marked as f_to_be_killed and that they don't occupy any slots in the historical detection buffer
    **/
   Check_Cluster_Killed(clusters[11], tracker_info);
}

/**
*\purpose Make sure that the mid prio cluster of type 3 is allowed to live on but can only occupy a small number of slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets, Test_Cluster_Typ3)
{
   /** \precond
    * Use default settings from the test setup. The cluster of type 3 are of mid prio with maximum number of new detections but no old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clustes of type 3 is kept alive and that it is allowed to occupy a small number of detections in the historical detection buffer.
    **/
   const int16_t expected_num_old_dets = 10;
   Check_Cluster_Alive(clusters[12], tracker_info, expected_num_old_dets);
}

/**
*\purpose Make sure that the high prio clusters of type 4 are allowed to live on and can occupy the maximum allowed number of slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets, Test_Cluster_Type4)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 4 are of highest prio with maximum number of new and old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clusters of type 4 are are kept alive and that they are allowed to occupy maximum allowed number of slots in the historical detection buffer.
    **/
   const int16_t expected_num_old_dets = tracker_info.variant.num_hist_dets_in_cluster;
   for(int16_t i = 13; i < 26; i++)
   {
      Check_Cluster_Alive(clusters[i], tracker_info, expected_num_old_dets);
   }
}

/**
*\purpose Make sure that the high prio clusters of type 5 are allowed to live on and can occupy the maximum allowed number of slots in the historical detection buffer
*\req    NA
*/
TEST(Update_Detection_History_Hist_Det_Buffer_Saturated_Mov_Dets, Test_Cluster_Type5)
{
   /** \precond
    * Use default settings from the test setup. Clusters of type 5 are of highest prio with maximum number of new detections but no old detections.
    **/

   /** \action
    * Call Sensor_Postprocessing module function
    **/
   Sensor_Postprocessing(host_props, det_props, raw_detection_list, sensors, tracker_info, det_hist, clusters, timing_info);

   /** \result
    * Make sure that all clusters of type 5 are are kept alive and that they are allowed to occupy maximum allowed number of slots in the historical detection buffer.
    **/
   const int16_t expected_num_old_dets = tracker_info.variant.num_hist_dets_in_cluster;
   for(int16_t i = 26; i < 38; i++)
   {
      Check_Cluster_Alive(clusters[i], tracker_info, expected_num_old_dets);
   }
}
/** @}*/
