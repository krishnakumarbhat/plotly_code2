/** \file
   File contains test cases for Clusters_Preprocessing() function
*/

#include "f360_clusters_preprocessing.h"
#include "f360_sensor_postprocessing.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

using namespace f360_variant_A;

/** \defgroup  f360_clusters_preprocessing
 *  @{
 */

/** \brief
 *  Group of test for Clusters_Preprocessing function
 */

TEST_GROUP(f360_clusters_preprocessing)
{
   /** \setup
   * Seting up default tolerance, calibrations, clusters, historical detections, tracker info
   */
   float32_t tolerance = 10e-6F;
   F360_Calibrations_T calib{};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS];
   F360_Detection_Hist_T det_hist;
   F360_Tracker_Info_T tracker_info;
   F360_Host_Props_T host_props;

   TEST_SETUP()
   {
      for (int32_t cluester_idx = 0; cluester_idx < NUMBER_OF_CLUSTERS; cluester_idx++)
      {
         clusters[cluester_idx] = {};
      }
      det_hist = {};
      tracker_info = {};

      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      // Preparing some clusters
      tracker_info.num_active_clusters = 6;
      tracker_info.active_cluster_ids[0] = 2;
      tracker_info.active_cluster_ids[1] = 4;
      tracker_info.active_cluster_ids[2] = 6;
      tracker_info.active_cluster_ids[3] = 7;
      tracker_info.active_cluster_ids[4] = 10;
      tracker_info.active_cluster_ids[5] = 16;

      clusters[1].f_to_be_killed = false;
      clusters[1].id = 2;
      clusters[1].vcs_position_x = 11.4F;
      clusters[1].vcs_position_y = 12.4F;
      clusters[1].rep_vcs_az =  0.8F;

      clusters[3].f_to_be_killed = true;
      clusters[3].id = 4;
      clusters[3].vcs_position_x = 11.0F;
      clusters[3].vcs_position_y = 12.0F;
      clusters[3].rep_vcs_az = 0.8F;

      clusters[5].f_to_be_killed = false;
      clusters[5].id = 6;
      clusters[5].vcs_position_x = 5.5F;
      clusters[5].vcs_position_y = 4.4F;
      clusters[5].rep_vcs_az = 0.6F;

      clusters[6].f_to_be_killed = true;
      clusters[6].id = 7;
      clusters[6].vcs_position_x = 23.3F;
      clusters[6].vcs_position_y = -45.8F;
      clusters[6].rep_vcs_az = -1.1F;

      clusters[9].f_to_be_killed = true;
      clusters[9].id = 10;
      clusters[9].vcs_position_x = 9.9F;
      clusters[9].vcs_position_y = 9.9F;
      clusters[9].rep_vcs_az = 0.7F;

      clusters[15].f_to_be_killed = false;
      clusters[15].id = 16;
      clusters[15].vcs_position_x = 20.0F;
      clusters[15].vcs_position_y = 20.0F;
      clusters[15].rep_vcs_az = -0.7F;

      // Preparing some historical detections
      det_hist.n_occupied = 5;
      det_hist.max_occupation = 5;

      det_hist.det_data[0].vcs_position_x = 10.0F;
      det_hist.det_data[0].vcs_position_y = 11.0F;
      det_hist.det_data[0].time_since_meas = 0.3F;
      det_hist.det_data[0].f_is_range_in_all_looks = false;
      det_hist.f_idx_occupied[0] = true;

      det_hist.det_data[1].vcs_position_x = 5.2F;
      det_hist.det_data[1].vcs_position_y = 1.5F;
      det_hist.det_data[1].time_since_meas = 0.3F;
      det_hist.det_data[1].f_is_range_in_all_looks = false;
      det_hist.f_idx_occupied[1] = true;

      det_hist.det_data[2].vcs_position_x = 20.0F;
      det_hist.det_data[2].vcs_position_y = 19.9F;
      det_hist.det_data[2].time_since_meas = 0.9F;
      det_hist.det_data[2].f_is_range_in_all_looks = true;
      det_hist.f_idx_occupied[2] = true;

      det_hist.det_data[3].vcs_position_x = 22.0F;
      det_hist.det_data[3].vcs_position_y = 12.9F;
      det_hist.det_data[3].time_since_meas = 0.5F;
      det_hist.det_data[3].f_is_range_in_all_looks = false;
      det_hist.f_idx_occupied[3] = true;

      det_hist.det_data[4].vcs_position_x = 22.0F;
      det_hist.det_data[4].vcs_position_y = 12.9F;
      det_hist.det_data[4].time_since_meas = 0.9F;
      det_hist.det_data[4].f_is_range_in_all_looks = false;
      det_hist.f_idx_occupied[4] = true;

      det_hist.det_data[5].vcs_position_x = 12.0F;
      det_hist.det_data[5].vcs_position_y = 13.9F;
      det_hist.det_data[5].time_since_meas = 0.750F;
      det_hist.det_data[5].f_is_range_in_all_looks = false;
      det_hist.f_idx_occupied[5] = true;
      det_hist.det_data[5].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

      // Preparing host properties
      host_props.delta_pointing = 0.0125F;
      host_props.cos_delta_pointing = 0.999921876017247F;
      host_props.sin_delta_pointing = 0.012499674481710F;
      host_props.delta_position_x = 1.2498F;
      host_props.delta_position_y = 0.0164F;
   }

   /** \teardown
    * Nothing to teardown in this test group
    */
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Check that the VCS properties of all clusters that are not killed have been updated
*\req    NA
*/
TEST(f360_clusters_preprocessing, test_vcs_props_updated)
{
   /** \precond
    * Assigning detections to clusters that should not be killed.
    * Setting up expected data for vcs properties of clusters
    */
   clusters[1].num_old_dets = 1;
   clusters[1].old_det_idx[0] = 0;
   float32_t expected_az_2 = 0.787500000000000F;
   float32_t expected_long_pos_2 = 10.304197994861966F;
   float32_t expected_lat_pos_2 = 12.255758347922933F;

   clusters[15].num_old_dets = 1;
   clusters[15].old_det_idx[0] = 1;
   float32_t expected_az_16 = -0.712500000000000F;
   float32_t expected_long_pos_16 = 18.998523654671285F;
   float32_t expected_lat_pos_16 = 19.747667405111308F;

   /** \action
    * Calling Clusters_Preprocessing()
    */
   Clusters_Preprocessing(host_props, clusters, det_hist, tracker_info);

   /** \result
    * The VCS properties of clusters that are still active should have updated vcs position and vcs azimuth
    */
   DOUBLES_EQUAL_TEXT(expected_az_2, clusters[1].rep_vcs_az, tolerance, "Unexpected vcs azimuth for cluster with id 2");
   DOUBLES_EQUAL_TEXT(expected_long_pos_2, clusters[1].vcs_position_x, tolerance, "Unexpected vcs longitudinal position for cluster with id 2");
   DOUBLES_EQUAL_TEXT(expected_lat_pos_2, clusters[1].vcs_position_y, tolerance, "Unexpected vcs lateral position for cluster with id 2");

   DOUBLES_EQUAL_TEXT(expected_az_16, clusters[15].rep_vcs_az, tolerance, "Unexpected vcs azimuth for cluster with id 16");
   DOUBLES_EQUAL_TEXT(expected_long_pos_16, clusters[15].vcs_position_x, tolerance, "Unexpected vcs longitudinal position for cluster with id 16");
   DOUBLES_EQUAL_TEXT(expected_lat_pos_16, clusters[15].vcs_position_y, tolerance, "Unexpected vcs lateral position for cluster with id 16");
}
/** @}*/


/** \defgroup  f360_clusters_preprocessing_correct_cluster_vcs_props
 *  @{
 */

/** \brief
 *  Test group for the Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion() function
 */

TEST_GROUP(f360_clusters_preprocessing_correct_cluster_vcs_props)
{
   /** \setup
   * Setting up default tolerance, calibrations, cluster, and host props
   */
   float32_t tolerance = 10e-6F;
   F360_Cluster_T cluster = {};
   F360_Host_Props_T host_props = {};

   TEST_SETUP()
   {
      cluster.vcs_position_x = -30.6F;
      cluster.vcs_position_y = 3.2F;
      cluster.rep_vcs_az = 3.0374F;
   }

   /** \teardown
    * Nothing to tear down in this test group
    */
   TEST_TEARDOWN()
   {
   }

};

/**
*\purpose  Check if the VCS properties of a cluster is correctly modified when host is driving forward and turning right
*\req    NA
*/
TEST(f360_clusters_preprocessing_correct_cluster_vcs_props, test_host_driving_forward_turning_right)
{
   /** \precond
    * Setting up host props to correspond to a case where host is driving forward and turning right.
    * Also setting up expected output
    */
   host_props.delta_pointing = 0.0125F;
   host_props.cos_delta_pointing = 0.999921876017247F;
   host_props.sin_delta_pointing = 0.012499674481710F;
   host_props.delta_position_x = 1.2498F;
   host_props.delta_position_y = 0.0164F;

   float32_t expected_azimuth = 3.024900000000000F;
   float32_t expected_long_pos = -31.807517803094154F;
   float32_t expected_lat_pos = 3.581463416796069F;



   /** \action
    * Calling Clusters_Preprocessing()
    */
   Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(host_props, cluster);

   /** \result
    * Check that cluster VCS properties correspond to expected data
    */
   DOUBLES_EQUAL_TEXT(expected_azimuth, cluster.rep_vcs_az, tolerance, "Unexpected cluster VCS azimuth angle");
   DOUBLES_EQUAL_TEXT(expected_long_pos, cluster.vcs_position_x, tolerance, "Unexpected cluster VCS longitudinal position");
   DOUBLES_EQUAL_TEXT(expected_lat_pos, cluster.vcs_position_y, tolerance, "Unexpected cluster VCS lateral position");
}

/**
*\purpose  Check if the VCS properties of a cluster is correctly modified when host is driving forward and turning left
*\req    NA
*/
TEST(f360_clusters_preprocessing_correct_cluster_vcs_props, test_host_driving_forward_turning_left)
{
   /** \precond
    * Setting up host props to correspond to a case where host is driving forward and turning left.
    * Also setting up expected output
    */
   host_props.delta_pointing = -0.0125F;
   host_props.cos_delta_pointing = 0.999921876017247F;
   host_props.sin_delta_pointing = -0.012499674481710F;
   host_props.delta_position_x = 1.2498F;
   host_props.delta_position_y = -0.0164F;

   float32_t expected_azimuth = 3.049900000000000F;
   float32_t expected_long_pos = -31.887515719777099F;
   float32_t expected_lat_pos = 2.818036589714314F;



   /** \action
    * Calling Clusters_Preprocessing()
    */
   Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(host_props, cluster);

   /** \result
    * Check that cluster VCS properties correspond to expected data
    */
   DOUBLES_EQUAL_TEXT(expected_azimuth, cluster.rep_vcs_az, tolerance, "Unexpected cluster VCS azimuth angle");
   DOUBLES_EQUAL_TEXT(expected_long_pos, cluster.vcs_position_x, tolerance, "Unexpected cluster VCS longitudinal position");
   DOUBLES_EQUAL_TEXT(expected_lat_pos, cluster.vcs_position_y, tolerance, "Unexpected cluster VCS lateral position");
}

/**
*\purpose  Check if the VCS properties of a cluster is correctly modified when host is reversing and turning right
*\req    NA
*/
TEST(f360_clusters_preprocessing_correct_cluster_vcs_props, test_host_reversing_turning_right)
{
   /** \precond
    * Setting up host props to correspond to a case where host is reversing and turning right.
    * Also setting up expected output
    */
   host_props.delta_pointing = -0.0070F;
   host_props.cos_delta_pointing = 0.999975500100042F;
   host_props.sin_delta_pointing = -0.006999942833473F;
   host_props.delta_position_x = -0.1373F;
   host_props.delta_position_y = 0.0267F;

   float32_t expected_azimuth = 3.044400000000000F;
   float32_t expected_long_pos = -30.484166585490996F;
   float32_t expected_lat_pos = 2.959985095914212F;



   /** \action
    * Calling Clusters_Preprocessing()
    */
   Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(host_props, cluster);

   /** \result
    * Check that cluster VCS properties correspond to expected data
    */
   DOUBLES_EQUAL_TEXT(expected_azimuth, cluster.rep_vcs_az, tolerance, "Unexpected cluster VCS azimuth angle");
   DOUBLES_EQUAL_TEXT(expected_long_pos, cluster.vcs_position_x, tolerance, "Unexpected cluster VCS longitudinal position");
   DOUBLES_EQUAL_TEXT(expected_lat_pos, cluster.vcs_position_y, tolerance, "Unexpected cluster VCS lateral position");
}

/**
*\purpose  Check if the VCS properties of a cluster is correctly modified when host is reversing and turning left
*\req    NA
*/
TEST(f360_clusters_preprocessing_correct_cluster_vcs_props, test_host_reversing_turning_left)
{
   /** \precond
    * Setting up host props to correspond to a case where host is reversing and turning left.
    * Also setting up expected output
    */
   host_props.delta_pointing = 0.0070F;
   host_props.cos_delta_pointing = 0.999975500100042F;
   host_props.sin_delta_pointing = 0.006999942833473F;
   host_props.delta_position_x = -0.1373F;
   host_props.delta_position_y = 0.0272F;

   float32_t expected_azimuth = 3.030400000000000F;
   float32_t expected_long_pos = -30.439744248275492F;
   float32_t expected_lat_pos = 3.385959425270662F;



   /** \action
    * Calling Clusters_Preprocessing()
    */
   Correct_Cluster_VCS_Props_Based_On_Host_Delta_Motion(host_props, cluster);

   /** \result
    * Check that cluster VCS properties correspond to expected data
    */
   DOUBLES_EQUAL_TEXT(expected_azimuth, cluster.rep_vcs_az, tolerance, "Unexpected cluster VCS azimuth angle");
   DOUBLES_EQUAL_TEXT(expected_long_pos, cluster.vcs_position_x, tolerance, "Unexpected cluster VCS longitudinal position");
   DOUBLES_EQUAL_TEXT(expected_lat_pos, cluster.vcs_position_y, tolerance, "Unexpected cluster VCS lateral position");
}
/** @}*/
