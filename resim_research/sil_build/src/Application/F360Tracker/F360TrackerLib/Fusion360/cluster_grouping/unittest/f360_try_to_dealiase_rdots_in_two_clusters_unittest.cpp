/** \file
   File contains unit tests for try_to_dealiase_rdots_in_two_clusters function
*/

#include "f360_try_to_dealiase_rdots_in_two_clusters.h"
#include "f360_cluster_grouping_data_generator.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_try_to_dealiase_rdots_in_two_clusters
 *  @{
 */

/** \brief
*  Test group for try_to_dealiase_rdots_in_two_clusters function
**/
TEST_GROUP(f360_try_to_dealiase_rdots_in_two_clusters)
{
   /** \setup
   * Setting up arguments for try_to_dealiase_rdots_in_two_clusters function and assigning them with basic values
   **/
   F360_Calibrations_T calib = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Cluster_T &cluster_older = clusters[0];
   F360_Cluster_T &cluster_newer = clusters[1];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Detection_Hist_T detections_hist = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Globals_T globals = {};
   float32_t rngrate_interval_width_older = 0.0F;
   float32_t rngrate_interval_width_newer = 0.0F;
   float32_t interval_older = 0.0F;
   float32_t interval_newer = 0.0F;

   float32_t tolerance = 1e-6;
   float32_t rdot_interval_width_1 = 69.8359375;
   float32_t rdot_interval_width_2 = 59.6132813;

   bool f_success = false;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      globals.max_otg_speed = 70.0F;

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = rdot_interval_width_1;
      sensors[0].refined.time_since_measurement_s = 0.05F;

      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = rdot_interval_width_2;
      sensors[1].refined.time_since_measurement_s = 0.04F;

      tracker_info.num_unique_rdot_interval_widths = 2;
      tracker_info.unique_rdot_interval_widths[0] = rdot_interval_width_1;
      tracker_info.unique_rdot_interval_widths[1] = rdot_interval_width_2;

      tracker_info.rdot_interval_compatibility[0][1] = true;
      tracker_info.rdot_interval_compatibility[1][0] = true;
   }
};

/**
*\purpose  Checking nominal case for cluster with very low range rates
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoStationaryClustersLowDetsNum)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.2F);


   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if stationary cluster gets dealiased
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two dealiasted clusters with low amounnt of detections
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersMovingAndDealiasedLowDetsNum)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   cluster_newer.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 11.1F, 0.1F);
   cluster_newer.rep_rdotcomp = 10.6;

   cluster_older.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 13.0F, 0.2F);
   cluster_older.rep_rdotcomp = 12.55;


   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if two dealiased clusters could be deaaliased with 0,0 intervals
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, newer dealiasted with low amount of detections
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersMovingNewerDealiasedLowDetsNum)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   cluster_newer.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 12.1F, 0.1F);
   cluster_newer.rep_rdotcomp = 11.1F;

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 13.1F, 0.2F);


   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters cluld be dealiased if newer cluster is dealiased
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking two clusters, older dealiasted with low amount of detections
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersMovingOlderDealiasedLowDetsNum)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 12.1F, 0.1F);

   cluster_older.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 13.1F, 0.2F);
   cluster_older.rep_rdotcomp = 12.6F;

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters cluld be dealiased if older cluster is dealiased
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking two clusters with low amount of detections
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersMovingSameLooks)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 12.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 13.1F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if to moving clusters was not dealiased dealiased with all detections in same interval
   **/
   CHECK_FALSE(f_success);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, older with many detections
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersMovingOneWithLowDetsNum)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 13.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 11.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 13.1F, 0.3F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters can be dealiased when one cluster has one det and other six with coresponding range rates
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, opposite range rates
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersOpositeRRLowDetsNum)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 13.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, -12.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, -13.1F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters were not dealiased if both didnt have coresponding range rates
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking two clusters, edge intervals range rates
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersDoubleRR)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 2, 60.0F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 59.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 2, 0.1);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 1.1F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters were dealiased when some range rates are on the edge of interval
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(-1.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, edge intervals range rates, newer cluster dealiased
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersDoubleRROneDealiased1)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   cluster_newer.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_newer, 2, 60.0F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 59.1F, 0.1F);
   cluster_newer.rep_rdotcomp = 59.55;

   Add_Det(det_props, raw_detection_list, cluster_older, 2, 0.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 1.1F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters were dealiased when some range rates are on the edge of interval with one dealised cluster
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(1.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, edge intervals range rates, older cluster dealiased
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersDoubleRROneDealiased2)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 2, 60.0);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 59.1F, 0.1F);

   cluster_older.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_older, 2, 0.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 1.1F, 0.2F);
   cluster_older.rep_rdotcomp = 0.6;

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if clusters were dealiased when some range rates are on the edge of interval with one dealised cluster
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(0.0, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(-1.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, many dets in clusters with different range rates
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersManyDetsHighRRDiversity)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 2.0F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 12.0F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 38.0F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 1.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 9.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 65.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 2, 0.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.0F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 12.0F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 38.0F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 1.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 9.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 65.1F, 0.3F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 2.1F, 0.3F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if cluster were not dealiased when both had random dets with random range rates
   **/
   CHECK_FALSE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, one false range rate detection in older cluster in different look
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersOneDetOutlierDiffLook)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 12.0F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 52.3F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 12.1F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if cluster were dealiased if two detections matches and one is outlier with matched interval
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking that two clusters with roughly similar range rate are properly de-aliased with interval shift that matches the range rate better
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersSimilarRangeRate)
{
   /** \precond
   Configuring clusters and adding detections to them
   Adjust interval widths
   **/
   float32_t rdot_interval_width_1 = 25.0F;
   float32_t rdot_interval_width_2 = 29.0F;
   sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
   sensors[0].constant.v_wrapping[0] = rdot_interval_width_1;

   sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
   sensors[1].constant.v_wrapping[0] = rdot_interval_width_2;

   tracker_info.unique_rdot_interval_widths[0] = rdot_interval_width_1;
   tracker_info.unique_rdot_interval_widths[1] = rdot_interval_width_2;

   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 12.5F);

   Add_Det(det_props, raw_detection_list, cluster_older, 2, 10.0F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if cluster were dealiased with correct aliasing interval
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL_TEXT(interval_older, 1.0F, tolerance, "Cluster is de-aliased with not expected aliasing interval")
   DOUBLES_EQUAL_TEXT(interval_newer, 1.0F, tolerance, "Cluster is de-aliased with not expected aliasing interval")

}

/**
*\purpose  Checking two clusters, one false range rate detection in older cluster in same look
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoClustersOneDetOutlierSameLook)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 12.0F);

   Add_Det(det_props, raw_detection_list, cluster_older, 2, 52.3F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 12.1F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if cluster were not dealiased if two detections matches and one is outlier without matched interval
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  Checking two clusters, two different range rates mean in opposite looks
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoIdenticalClustersManyDets2RRTypes)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.5F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 12.0F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_1, 10.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 29.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 31.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 30.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 29.0F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 31.0F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_1, 29.6F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 10.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 11.0F, 0.2F);

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if cluster were dealiased with high range rated differences but with matching mean
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}

/**
*\purpose  Checking two clusters, same pointer
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoIdenticalClustersNotDealiased)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 12.1F, 0.1F);


   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_newer, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if same cluster were dealiased
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking two clusters, same pointer, dealiased
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestTwoSameClustersDealiased)
{
   /** \precond
   Configuring clusters and adding detections to them
   **/
   cluster_newer.f_dealiased = true;
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 10.1);
   cluster_newer.rep_rdotcomp = 10.1;

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_newer, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if same dealiased cluster with one detections were not dealiased
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  Checking nominal case for cluster with very low range rates
*\req    NA
*/
TEST(f360_try_to_dealiase_rdots_in_two_clusters, TestDetectionsStaturation)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   for (uint16_t i = 0; i < MAX_DETS_IN_OBJ_TRK; i++)
   {
      Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
      Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

      Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1F);
      Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.2F);
   }

   /** \action
   *Call try_to_dealiase_rdots_in_two_clusters
   **/
   f_success = Try_To_Dealiase_Rdots_In_Two_Clusters(cluster_older, cluster_newer, sensors, raw_detection_list, det_props, detections_hist, tracker_info,
      globals, calib, rngrate_interval_width_older, rngrate_interval_width_newer, interval_older, interval_newer);

   /** \result
   *Checking if stationary cluster gets dealiased
   **/
   CHECK_TRUE(f_success);
   DOUBLES_EQUAL(rdot_interval_width_1, rngrate_interval_width_older, tolerance);
   DOUBLES_EQUAL(rdot_interval_width_2, rngrate_interval_width_newer, tolerance);
   DOUBLES_EQUAL(0.0, interval_older, tolerance);
   DOUBLES_EQUAL(0.0, interval_newer, tolerance);
}
/** @}*/
