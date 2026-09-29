/** \file
   File with set of qualification tests (which are also unit test) for Merge_Two_Clusters function
*/

#include "f360_merge_two_clusters.h"
#include "f360_cluster_grouping_data_generator.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
/** \defgroup  f360_merge_two_clusters
 *  @{
 */

/** \brief
*  Test group for Merge_Two_Clusters function
**/
TEST_GROUP(f360_merge_two_clusters)
{
   /** \setup
   * Nothing to setup in this test group
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
   float32_t rngrate_interval_width_older;
   float32_t rngrate_interval_width_newer;
   float32_t interval_older;
   float32_t interval_newer;

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
      sensors[1].refined.time_since_measurement_s = 0.45F;

      for (int16_t i = 0; i < NUMBER_OF_CLUSTERS; i++)
      {
         clusters[i].id = i + 1;
      }
   }
};

/**
*\purpose  Test to cover req if dealiased flag for detetions is properly set where all detection from two cluster are valid
*\req    CPR-3846
*/
TEST(f360_merge_two_clusters, Test_Merge_Two_Clusters_Check_If_All_4_Dets_Are_Dealiased)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 59.6F, 0.2F);


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
}

/**
*\purpose  Test to cover req if dealiased flag for detetions is properly set where one detection is valid and should be dealiased
*\req    CPR-3846
*/
TEST(f360_merge_two_clusters, Test_Merge_Two_Clusters_Check_If_Dets_4_Are_Dealiased_1_Not_Dealiased)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 40.6F, 0.3F);


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
   CHECK_FALSE(detections_hist.det_data[2].f_dealiased);
}

/**
*\purpose  Test to cover req if dealiased range rate for detetions is properly calculated
*\req    CPR-3846
*/
TEST(f360_merge_two_clusters, Test_Merge_Two_Clusters_Check_Range_Rate_Is_Dealiased_Correctly_1)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 69.9F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);

   Add_Det(det_props, raw_detection_list, cluster_older, 1, 2.1F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 3.1F, 0.2F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 60.0F, 0.2F);


   /** \action
   *Call Merge_Two_Clusters
   **/
   Merge_Two_Clusters(tracker_info, sensors, raw_detection_list, clusters, det_props, detections_hist, cluster_older.id, cluster_newer.id,
      rdot_interval_width_1, rdot_interval_width_2, interval_older, interval_newer, k_max_dealiased_rr_diff);

   /** \result
   *Checking if stationary cluster gets dealiased
   **/
   DOUBLES_EQUAL(0.1, det_props[0].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(0.064, det_props[1].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(2.1, det_props[2].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(2.1, detections_hist.det_data[0].rdot_comp, tolerance);
   DOUBLES_EQUAL(3.1, detections_hist.det_data[1].rdot_comp, tolerance);
   DOUBLES_EQUAL(0.387, detections_hist.det_data[2].rdot_comp, tolerance);
}

/** @}*/
