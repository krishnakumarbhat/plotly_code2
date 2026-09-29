/** \file
   File contains unit tests for Clustering function
*/

#include "f360_clustering.h"
#include "f360_clustering_data_generator.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_math.h"
#include "f360_set_variant.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;
/** \defgroup  clustering
 *  @{
 */


/** \brief
*  Test group for clustering function
**/
TEST_GROUP(f360_clustering)
{
   /** \setup
   * Setting up arguments for clustering function and assigning them with basic values
   **/
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Calibrations_T calib = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Host_T host = {};
   F360_Detection_Hist_T det_hist = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      Setup_TrackerInfo_Cluster_Detp(tracker_info, clusters, raw_detection_list);

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = 50.0F;

      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = 60.0F;

      host.vcs_speed = 5.0F;
      host.curvature_rear = 0.0F;

      memset(&det_hist, 0, sizeof(det_hist));
   }
};

/**
*\purpose  Test to cover req if cluster id will be proeprly set for one moving detetion from which cluster shall be formualted
*\req    CPR-3851
*/
TEST(f360_clustering, Test_Clustering_Single_Moving_Det)
{
   /** \precond
   Preparing valid moving detection data for clustering
   **/
   int16_t local_det_idx = 0;

   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   bool f_double_bounce = false;
   bool f_close_target = false;
   bool f_det_pair = false;
   bool f_FOV_edge = false;
   bool f_ok_to_use = true;
   uint8_t on_sep_id = F360_INVALID_UNSIGNED_ID;
   bool f_host_veh_clutter = false;
   int32_t object_track_id = 0;
   F360_Detection_Wheelspin_Type_T wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
      det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, detection_props);

   Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
      f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);

   Sort_Detections_Vcs_Long(raw_detection_list);

   /** \action
   * Calling Clustering
   **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
   * Checking if one detection has correct set cluster id
   **/
   CHECK_EQUAL(1, detection_props[0].cluster_id);
}

/**
*\purpose  Test to cover req if cluster id will be not set for one detetion if its already associated to object
*\req    CPR-3851
*/
TEST(f360_clustering, Test_Clustering_Valid_Det_Assined_To_Track)
{
   /** \precond
   Preparing valid moving detection already assinged to track for clustering
   **/
   int16_t local_det_idx = 0;

   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   bool f_double_bounce = false;
   bool f_close_target = false;
   bool f_det_pair = false;
   bool f_FOV_edge = false;
   bool f_ok_to_use = true;
   uint8_t on_sep_id = F360_INVALID_UNSIGNED_ID;
   bool f_host_veh_clutter = false;
   int32_t object_track_id = 1;
   F360_Detection_Wheelspin_Type_T wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
      det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, detection_props);

   Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
      f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);

   Sort_Detections_Vcs_Long(raw_detection_list);

   /** \action
   * Calling Clustering
   **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
   * Checking if no detection has cluster id set
   **/
   bool f_success = true;
   for (int16_t i = 0; i < 10; i++)
   {
      if (detection_props[0].cluster_id != 0)
      {
         f_success = false;
         break;
      }
   }
   CHECK_TRUE(f_success);
}

/**
*\purpose  Test to cover req if cluster id will not be set for one invalid detection from which cluster shall not be formulated
*\req    CPR-3851
*/
TEST(f360_clustering, Test_Clustering_Single_Invalid_Det)
{
   /** \precond
   Preparing invalid detection data for clustering
   **/
   int16_t local_det_idx = 0;

   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   bool f_double_bounce = true;
   bool f_close_target = false;
   bool f_det_pair = false;
   bool f_FOV_edge = false;
   bool f_ok_to_use = true;
   uint8_t on_sep_id = F360_INVALID_UNSIGNED_ID;
   bool f_host_veh_clutter = false;
   int32_t object_track_id = 0;
   F360_Detection_Wheelspin_Type_T wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
      det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, detection_props);

   Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
      f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);

   Sort_Detections_Vcs_Long(raw_detection_list);

   /** \action
   * Calling Clustering
   **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
   * Checking if one detection has correct set cluster id
   **/
   CHECK_EQUAL(0, detection_props[0].cluster_id);
}

/**
*\purpose  Test to cover req if cluster id will be properly set for one detection from which cluster shall be formulated and the other, invalid one
*\from which cluster shall not be formulated
*\req    CPR-3851
*/
TEST(f360_clustering, Test_Clustering_One_Valid_One_Invalid_Det)
{
   /** \precond
   Preparing detections data for clustering
   **/
   int16_t local_det_idx = 0;

   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   bool f_double_bounce = true;
   bool f_close_target = false;
   bool f_det_pair = false;
   bool f_FOV_edge = false;
   bool f_ok_to_use = true;
   uint8_t on_sep_id = F360_INVALID_UNSIGNED_ID;
   bool f_host_veh_clutter = false;
   int32_t object_track_id = 0;
   F360_Detection_Wheelspin_Type_T wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   // Adding invalid det
   Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
      det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, detection_props);

   Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
      f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);

   // Adding valid det
   local_det_idx = 1;
   f_double_bounce = false;
   Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
      det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
      det_f_dealiased, det_motion_status, detection_props);

   Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
      f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);

   Sort_Detections_Vcs_Long(raw_detection_list);

   /** \action
   * Calling Clustering
   **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
   * Checking if one detection has correct set cluster id
   **/
   CHECK_EQUAL(0, detection_props[0].cluster_id);
   CHECK_EQUAL(1, detection_props[1].cluster_id);
}

/**
*\purpose  Test to cover req if cluster ids will be properly set for 10 detections from which one cluster shall be formulated
*\req    CPR-3851
*/
TEST(f360_clustering, Test_Clustering_Multiple_Moving_Det)
{
   /** \precond
   Preparing multiple valid moving detection data for clustering
   **/
   int16_t local_det_idx = 0;

   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   bool f_double_bounce = false;
   bool f_close_target = false;
   bool f_det_pair = false;
   bool f_FOV_edge = false;
   bool f_ok_to_use = true;
   uint8_t on_sep_id = F360_INVALID_UNSIGNED_ID;
   bool f_host_veh_clutter = false;
   int32_t object_track_id = 0;
   F360_Detection_Wheelspin_Type_T wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   for (int16_t i = 0; i < 10; i++)
   {
      local_det_idx = i;
      det_vcs_pos_long += 0.01;
      det_vcs_pos_lat += 0.01;
      det_range_rate += 0.01;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
         det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, detection_props);

      Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
         f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);
   }

   Sort_Detections_Vcs_Long(raw_detection_list);

   /** \action
   * Calling Clustering
   **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
   * Checking if each detection has correct set cluster id
   **/
   for (int16_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(1, detection_props[i].cluster_id);
   }
}

/**
*\purpose  Test to cover req if cluster id would be properly set for 10 valid moving detections and 10 stationary valid detection and 
*\from moving dets one cluster shall be formulated, from stationary dets one cluster shall be formulated
*\req    CPR-3851
*/
TEST(f360_clustering, Test_Clustering_Multiple_Moving_and_Stat_Det)
{
   /** \precond
   Preparing multiple valid moving and stationary detection data for clustering
   **/
   int16_t local_det_idx = 0;
   
   float32_t det_vcs_pos_long = 1.0;
   float32_t det_vcs_pos_lat = 0.0;
   float32_t det_range_rate = 10.0;
   bool det_f_dealiased = true;
   rspp_variant_A::RSPP_Detection_Motion_Status_T det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   bool f_double_bounce = false;
   bool f_close_target = false;
   bool f_det_pair = false;
   bool f_FOV_edge = false;
   bool f_ok_to_use = true;
   uint8_t on_sep_id = F360_INVALID_UNSIGNED_ID;
   bool f_host_veh_clutter = false;
   int32_t object_track_id = 0;
   F360_Detection_Wheelspin_Type_T wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   // assigning moving detections
   for (int16_t i = 0; i < 10; i++)
   {
      local_det_idx = i;
      det_vcs_pos_long += 0.01;
      det_vcs_pos_lat += 0.01;
      det_range_rate += 0.01;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
         det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, detection_props);

      Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
         f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);
   }
   
   // assigning stationary detections
   det_vcs_pos_long = -30.0;
   det_vcs_pos_lat = -30.0;
   det_range_rate = 0.01;
   det_motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY;
   for (int16_t i = 10; i < 20; i++)
   {
      local_det_idx = i;
      det_vcs_pos_long += 0.01;
      det_vcs_pos_lat += 0.01;
      det_range_rate += 0.01;

      Add_Simple_Det_Data(raw_detection_list, raw_detection_list.detections[local_det_idx], local_det_idx,
         det_vcs_pos_long, det_vcs_pos_lat, det_range_rate,
         det_f_dealiased, det_motion_status, detection_props);

      Add_Det_Data_For_Validity(detection_props[local_det_idx], raw_detection_list.detections[local_det_idx], f_double_bounce, f_close_target,
         f_det_pair, f_FOV_edge, f_ok_to_use, on_sep_id, f_host_veh_clutter, object_track_id, wheel_spin_type);
   }

   Sort_Detections_Vcs_Long(raw_detection_list);
   /** \action
   * Calling Clustering
   **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
   * Checking if each detection has correct set cluster id
   **/
   for (int16_t i = 0; i < 10; i++)
   {
      CHECK_EQUAL(1, detection_props[i].cluster_id);
   }

   for (int16_t i = 10; i < 20; i++)
   {
      CHECK_EQUAL(2, detection_props[i].cluster_id);
   }
}

/**
 *\purpose  Test that no clusters are created if there are no detections
 *\req NA
 */
TEST(f360_clustering, Test_Clustering_Zero_Detections)
{
   /** \precond
   * Set the number of detections to zero
   **/
   raw_detection_list.number_of_valid_detections = 0U;

   /** \action
    * Calling Clustering
    **/
   Clustering(calib, sensors, host, tracker_info, raw_detection_list, detection_props, det_hist, clusters, timing_info);

   /** \result
    * Checking that no clusters are created
    **/
   CHECK_EQUAL(0, tracker_info.num_active_clusters);
}

/** @}*/
