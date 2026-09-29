/** \file
   File with set of qualification tests (which are also unit test) for initialize_clusters function
*/

#include "f360_initialize_clusters.h"
#include "f360_clustering_data_generator.h"
#include "f360_math.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
/** \defgroup  initialize_clusters
 *  @{
 */


/** \brief
*  Test group for initialize_clusters function
**/
TEST_GROUP(initialize_qualtest_clusters)
{
   /** \setup
   * Setting up arguments for initialize_clusters function and assigning them with basic values
   **/
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Local_Clusters_T local_cluster_data = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Calibrations_T calibs;
   F360_Host_T host_props = {}; // Added for new interface
   F360_Detection_Hist_T det_hist = {}; // Added for new interface

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      Setup_TrackerInfo_Cluster_Detp(tracker_info, clusters, raw_detection_list);

      host_props.vcs_speed = 10.0F;
      host_props.curvature_rear = 0.0F;
   }
};

/**
*\purpose  Test to cover req if cluster id is properly set for one detetions from which cluster shall is generated
*\req    CPR-3851
*/
TEST(initialize_qualtest_clusters, Test_Initialize_Clusters_One_Detection_To_Cluster)
{
   /** \precond
   Preparing initial clusters data and detections
   **/
   bool f_take_new_cluster_to_init = true;
   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster_to_init, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   /** \action
   * Calling Initialize_Clusters
   **/

   Initialize_Clusters(raw_detection_list, calibs, sensors, host_props, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Checking if one detections has correct set cluster id
   **/
   CHECK_EQUAL(1, detection_props[0].cluster_id);
}

/**
*\purpose  Test to cover req if cluster ids are properly set for 10 detetions from which one cluster shall be generated
*\req    CPR-3851
*/
TEST(initialize_qualtest_clusters, Test_Initialize_Clusters_Many_Detection_To_Cluster)
{
   /** \precond
   Preparing initial clusters data and detections
   **/
   bool f_take_new_cluster_to_init = true;
   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster_to_init, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);

   f_take_new_cluster_to_init = false;
   for (int16_t i = 0; i < 10; i++)
   {
      det_vcs_pos_long += 0.01;
      det_vcs_pos_lat += 0.01;
      det_range_rate += 0.01;
      Simple_Add_Det_for_Initialize_Clusters(f_take_new_cluster_to_init, det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, raw_detection_list, local_cluster_data, detection_props);
   }

   /** \action
   * Calling Initialize_Clusters
   **/
   Initialize_Clusters(raw_detection_list, calibs, sensors, host_props, local_cluster_data, tracker_info, detection_props, det_hist, clusters);

   /** \result
   * Checking if detections have correct set cluster ids
   **/
   for (int16_t i = 0; i < 11; i++)
   {
      CHECK_EQUAL(1, detection_props[i].cluster_id);
   }
}
/** @}*/
