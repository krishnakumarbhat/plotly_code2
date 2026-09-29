/** \file
   File with set of qualification tests (which are also unit test) for cluster_grouping function
*/

#include "f360_cluster_grouping.h"
#include "f360_cluster_grouping_data_generator.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_kill_cluster.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_clear_cluster.h"
using namespace f360_variant_A;

/** \defgroup  f360_cluster_grouping_qualtest
 *  @{
 */

/** \brief
 *  Test group for f360_cluster_grouping_qualtest
 */
TEST_GROUP(f360_cluster_grouping_qualtest)
{
   /** \setup
   * Setting up arguments for cluster_grouping function and assigning them with basic values
   **/
   F360_Calibrations_T calib = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Hist_T detections_hist = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   float32_t rngrate_interval_width_older;
   float32_t rngrate_interval_width_newer;
   float32_t interval_older;
   float32_t interval_newer;

   float32_t tolerance = 1e-3;
   float32_t rdot_interval_width_1 = 69.8359375;
   float32_t rdot_interval_width_2 = 59.6132813;

   bool f_success = false;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      globals.max_otg_speed = 70.0F;

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = rdot_interval_width_1;
      sensors[0].refined.time_since_measurement_s = 0.05F;

      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = rdot_interval_width_2;
      sensors[1].refined.time_since_measurement_s = 0.05F;

      tracker_info.num_unique_rdot_interval_widths = 2;
      tracker_info.unique_rdot_interval_widths[0] = rdot_interval_width_1;
      tracker_info.unique_rdot_interval_widths[1] = rdot_interval_width_2;

      tracker_info.rdot_interval_compatibility[0][1] = true;
      tracker_info.rdot_interval_compatibility[1][0] = true;

      for (int16_t i = 0; i < NUMBER_OF_CLUSTERS; i++)
      {
         tracker_info.inactive_cluster_ids[i] = i + 1;
         clusters[i].id = i + 1;
      }
   }
};

/**
*\purpose  Test for fulfilling requirements of clustering for detections' range rate dealiasing for set of 6 valid detections
*\req    CPR-3846
*/
TEST(f360_cluster_grouping_qualtest, Test_Cluster_Grouping_Check_If_Range_Rates_Are_Dealiased_Correctly)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   F360_Cluster_T &cluster_older = clusters[0];
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 1.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 69.8F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 1.5F, 0.2F);

   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   F360_Cluster_T &cluster_newer = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 59.6F, 0.1F);

   /** \action
   *Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   *Checking if range rates are dealiased
   **/
   DOUBLES_EQUAL(1.1, det_props[0].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(-0.036, det_props[1].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(0.1, det_props[2].range_rate_dealiased, tolerance);
   DOUBLES_EQUAL(1.5, detections_hist.det_data[0].rdot_comp, tolerance);
   DOUBLES_EQUAL(2.1, detections_hist.det_data[1].rdot_comp, tolerance);
   DOUBLES_EQUAL(-0.013, detections_hist.det_data[2].rdot_comp, tolerance);
}

/**
*\purpose  Test for fulfilling requirements of clustering for detections' range rate dealiasing flag for set of 6 valid detections
*\req    CPR-3846
*/
TEST(f360_cluster_grouping_qualtest, Test_Cluster_Grouping_Check_If_Dealiased_Flag_Is_Set_Correctly_1)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   F360_Cluster_T &cluster_older = clusters[0];
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 1.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 69.8F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 1.5F, 0.2F);

   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   F360_Cluster_T &cluster_newer = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1F, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 59.6F, 0.1F);

   /** \action
   *Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   *Checking if dealiased flag is set correctly
   **/
   CHECK_TRUE(det_props[0].f_dealiased);
   CHECK_TRUE(det_props[1].f_dealiased);
   CHECK_TRUE(det_props[2].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[0].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[1].f_dealiased);
   CHECK_TRUE(detections_hist.det_data[2].f_dealiased);
}

/**
*\purpose  Test for fulfilling requirements of clustering for detections' range rate dealiasing flag where two clusters shall not be merged/dealiased
*\req    CPR-3846
*/
TEST(f360_cluster_grouping_qualtest, Test_Cluster_Grouping_Check_If_Dealiased_Flag_Is_Set_Correctly_2)
{
   /** \precond
   *Configuring clusters and adding detections to them
   **/
   Add_Simple_Cluster_To_Tracker_Status(0, clusters, tracker_info);
   F360_Cluster_T &cluster_older = clusters[0];
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 1.1F);
   Add_Det(det_props, raw_detection_list, cluster_older, 1, 69.8F);
   Add_Hist_Det(detections_hist, cluster_older, rdot_interval_width_2, 1.5F, 0.2F);

   Add_Simple_Cluster_To_Tracker_Status(1, clusters, tracker_info);
   F360_Cluster_T &cluster_newer = clusters[1];
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 0.1F);
   Add_Det(det_props, raw_detection_list, cluster_newer, 1, 30.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 2.1, 0.1F);
   Add_Hist_Det(detections_hist, cluster_newer, rdot_interval_width_2, 59.6F, 0.1F);

   /** \action
   *Call Cluster_Grouping
   **/
   Cluster_Grouping(globals, calib, sensors, raw_detection_list, tracker_info, det_props, detections_hist, clusters, timing_info);

   /** \result
   *Checking if dealiased flag is set correctly
   **/
   CHECK_FALSE(det_props[0].f_dealiased);
   CHECK_FALSE(det_props[1].f_dealiased);
   CHECK_FALSE(det_props[2].f_dealiased);
   CHECK_FALSE(det_props[3].f_dealiased);
   CHECK_FALSE(detections_hist.det_data[0].f_dealiased);
   CHECK_FALSE(detections_hist.det_data[1].f_dealiased);
   CHECK_FALSE(detections_hist.det_data[2].f_dealiased);
}
/** @}*/
