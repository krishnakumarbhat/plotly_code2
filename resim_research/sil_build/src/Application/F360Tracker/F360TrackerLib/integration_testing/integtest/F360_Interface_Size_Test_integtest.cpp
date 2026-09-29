/** \file
   This file contains test cases for interface data sizes
*/
#include "f360_tracker.h"
#include "f360_get_wall_time.h"
#include "f360_update_relative_timestamps.h"
#include "f360_internal_preprocessing.h"
#include "f360_time_update_tracks.h"
#include "f360_pre_association_track_management.h"
#include "f360_detection_to_track_association.h"
#include "f360_clustering.h"
#include "f360_cluster_grouping.h"
#include "f360_track_grouping.h"
#include "f360_internal_preprocessing.h"
#include "f360_inputs_preprocessing.h"
#include "f360_initialize_tracks.h"
#include "f360_measurement_update_tracks.h"
#include "f360_post_update_track_adjustments.h"
#include "f360_track_classification.h"
#include "f360_track_validity.h"
#include "f360_track_downselection.h"
#include "f360_e2e_protection.h"
#include "f360_static_environment_class.h"
#include "f360_trailer_detector_core.h"
#include "f360_sanity_check.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>
#include "init_tracker_info_T.h"
#include "init_object_tracks_T.h"
#include "init_clusters_T.h"
#include "init_core_T.h"
#include "init_det_hist_T.h"

#include "init_det_props_T.h"
#include "init_globals_T.h"
#include "init_host_props_T.h"
#include "init_host_T.h"
#include "init_raw_detect_T.h"
#include "init_sensor_calib_T.h"
#include "init_sensor_props_T.h"
#include "init_sensors_T.h"
#include "init_timing_info_T.h"
#include "init_static_env_class_T.h"
#include "init_trailer_detector_core.h"
#include "init_occupancy_grid_T.h"
#include "f360_calibrations.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

/*F360_Input_Init header file contains input_init() macro  which call input test vector initialization */
#include "F360_Input_Init.h"

using namespace f360_variant_A;

/** \defgroup  f360_tracker_integration_size_test
 *  @{
 */

/** \brief
*  this test suits aim to test the data type of interface signals
**/
TEST_GROUP(f360_tracker_integration_size_test)
{   
   #include "F360_Interface_Declaration.h"
    
   /** \setup
   * init the test input vector
   **/
   TEST_SETUP()
   {   
       //input_init
       input_init();
   }   

   /** \teardown
   * Nothing to teardown in this test group
   **/
 
};

/**
 *\purpose
 *  Purpose of this test is to verify all the interfaces are up to date and not modified.
 *  If this test fails then the code of this test case has to be modified in the following way:
 *   1) The assignment of a value for example to  expected_size_of_obj_trk to be changed based on the new modification.
 *   2) Check the failed interface modifications and for the deleted signals delete them from the input_init for example init_object_tracks_T.cpp 
 *      ,and for the newly added signals make sure to add it to the input init for example init_object_tracks_T.cpp with their required init value.
 *\req
 * NA
 */
TEST(f360_tracker_integration_size_test, check_verification_of_interfaces_size)
{
   /** \precond
    **/
   uint32_t expected_size_of_obj_track = 948U;                     /* should be equal to sizeof(obj_tracks)*/
   const uint32_t expected_size_of_host = 52U;                           /* should be equal to sizeof(host)*/
   const uint32_t expected_size_of_core_info= 24U;                       /* should be equal to sizeof(core_info)*/
   const uint32_t expected_size_of_sensors = 552U;                       /* should be equal to sizeof(sensors[0])*/
   const uint32_t expected_size_of_globals = 204U;                       /* should be equal to sizeof(globals)*/
   const uint32_t expected_size_of_host_props = 112U;                    /* should be equal to sizeof(host_props)*/
   const uint32_t expected_size_of_detections = 76U;                     /* should be equal to sizeof(detections[0])*/
   const uint32_t expected_size_of_det_hist = 122620U;                    /* should be equal to sizeof(det_hist)*/
   const uint32_t expected_size_of_det_hist_det_data= 60U;               /* should be equal to sizeof(det_hist.det_data[0]))*/
   const uint32_t expected_size_of_timing_info = 256U;                   /* should be equal to sizeof(timing_info)*/
   const uint32_t expected_size_of_clusters = 380U;                      /* should be equal to sizeof(clusters[0])*/
   const uint32_t expected_size_of_raw_detection_list = 1542988U;         /* should be equal to sizeof(raw_detection_list)*/
   const uint32_t expected_size_of_raw_detection_list_detections = 164U; /* should be equal to sizeof(raw_detection_list.detections[0])*/
   uint32_t expected_size_of_tracker_info = 55848U;                      /* should be equal to sizeof(tracker_info)*/
   const uint32_t expected_size_of_sensor_props = 1616U;                 /* should be equal to sizeof(sensor_props[0])*/
   uint32_t expected_size_of_static_env_class = 2404U;                   /* should be equal to sizeof(static_env_class)*/
   const uint32_t expected_size_of_trailer_detector_core = 1876U;        /* should be equal to sizeof(trailer_detector_core)*/
   const uint32_t expected_size_of_occupancy_grid = 256U;
   
   // If compiling 64-bit architecture, pointers are 8 bytes instead of 4 so we need to update the expected size
   if(sizeof(void*) == 8)
   {
      // Update structs containing pointers.
      expected_size_of_obj_track += 20;
      expected_size_of_tracker_info += 24016;
      expected_size_of_static_env_class += 204;
   }
   /** \action
    * get the size of interfaces
    */
    
   uint32_t size_of_obj_track = sizeof(object_tracks[0]);
   uint32_t size_of_host = sizeof(host);
   uint32_t size_of_core_info = sizeof(core_info);
   uint32_t size_of_sensors = sizeof(sensors[0]);
   uint32_t size_of_globals = sizeof(globals);
   uint32_t size_host_props = sizeof(host_props);
   uint32_t size_of_detections = sizeof(detections[0]);
   uint32_t size_of_det_hist = sizeof(det_hist);
   uint32_t size_of_det_hist_det_data = sizeof(det_hist.det_data[0]);
   uint32_t size_of_timing_info = sizeof(timing_info);
   uint32_t size_of_clusters = sizeof(clusters[0]);
   uint32_t size_of_raw_detection_list = sizeof(raw_detection_list);
   uint32_t size_of_raw_detection_list_detections = sizeof(raw_detection_list.detections[0]);
   uint32_t size_of_tracker_info = sizeof(tracker_info);
   uint32_t size_of_sensor_props = sizeof(sensor_props[0]);
   uint32_t size_of_static_env_class = sizeof(static_env_class);
   uint32_t size_of_trailer_detector_core = sizeof(trailer_detector_core);
   uint32_t size_of_occupancy_grid = sizeof(occupancy_grid);


   /** \result
   * Verify that F360_Object_Track_T interface has not been modified.
   **/
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_obj_track, size_of_obj_track,
         "Check failed to verify that F360_Object_Track_T interface has not been modified, then check if equality operator of object tracks is up to date, then update expected_size_of_obj_track and init_object_tracks_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_host, size_of_host,
         "Check failed to verify that F360_Host_Tag interface has not been modified. Please update expected_size_of_host and init_host_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_core_info, size_of_core_info,
         "Check failed to verify that F360_Core_Info_T interface has not been modified. Please update expected_size_of_core_info and init_core_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_sensors, size_of_sensors,
         "Check failed to verify that F360_Radar_Sensor_T interface has not been modified. Please update expected_size_of_sensors and init_sensors_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_sensors, size_of_sensors,
         "Check failed to verify that F360_Radar_Sensor_T interface has not been modified. Please update expected_size_of_sensors and init_sensor_calib_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_globals, size_of_globals,
         "Check failed to verify that F360_Globals_T interface has not been modified. Please update expected_size_of_globals and init_globals_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_host_props, size_host_props,
         "Check failed to verify that F360_Host_Props_T interface has not been modified. Please update expected_size_of_host_props and init_host_props_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_detections, size_of_detections,
         "Check failed to verify that F360_Detection_Props_T interface has not been modified. Please update expected_size_of_detections and init_det_props_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_det_hist, size_of_det_hist,
         "Check failed to verify that F360_Detection_Hist_T interface has not been modified. Please update expected_size_of_det_hist and init_det_hist_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_det_hist_det_data, size_of_det_hist_det_data,
         "Check failed to verify that F360_Detection_Hist_Data_T interface has not been modified. Please update expected_size_of_det_hist_det_data and init_det_hist_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_timing_info, size_of_timing_info,
         "Check failed to verify that F360_TRKR_TIMING_INFO_T interface has not been modified. Please update expected_size_of_timing_info and init_timing_info_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_clusters, size_of_clusters,
         "Check failed to verify that F360_Cluster_T interface has not been modified. Please update expected_size_of_clusters and init_clusters_T.cpp file");    
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_raw_detection_list, size_of_raw_detection_list,
         "Check failed to verify that rspp_variant_A::RSPP_Detection_List_T interface has not been modified. Please update expected_size_of_raw_detection_list and init_raw_detect_T.cpp file"); 
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_raw_detection_list_detections, size_of_raw_detection_list_detections,
         "Check failed to verify that F360_Detection_T interface has not been modified. Please update expected_size_of_raw_detection_list_detections and init_raw_detect_T.cpp file"); 
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_tracker_info, size_of_tracker_info,
         "Check failed to verify that F360_Tracker_Info_T interface has not been modified. Please update expected_size_of_tracker_info and init_tracker_info_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_sensor_props, size_of_sensor_props,
         "Check failed  to verify that F360_Radar_Sensor_Props_T interface has not been modified. Please update expected_size_of_sensor_props and init_sensor_props_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_static_env_class, size_of_static_env_class,
            "Check failed  to verify that Static_Env_Class_T interface has not been modified. Please update expected_size_of_static_env_class and init_static_env_class_T.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_trailer_detector_core, size_of_trailer_detector_core,
            "Check failed  to verify that Trailer_Detector_Core interface has not been modified. Please update expected_size_of_trailer_detector_core and init_trailer_detector_core.cpp file");
   UNSIGNED_LONGS_EQUAL_TEXT(expected_size_of_occupancy_grid, size_of_occupancy_grid,
         "Check failed to verify that Occupancy_Grid_T interface has not been modified. Please update expected_size_of_occupancy_grid and init_occupancy_grid_T.cpp file"); 
}
