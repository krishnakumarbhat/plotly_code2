/** \file
  This file test the propagation of cluster interface signals
*/
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cfloat>
#include "f360_tracker.h"
#include "f360_update_relative_timestamps.h"
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
#include "f360_sensor_postprocessing.h"
#include "f360_get_wall_time.h"
#include "f360_occlusion.h"
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
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*F360_Input_Init header file contains input_init() macro  which call input test vector initialization */
#include "F360_Input_Init.h"

using namespace f360_variant_A;

/** \defgroup  f360_tracker_propagation_clusters
 *  @{
 */

 /** \brief
 *  This test suit aims to test the propagation of cluster interface signals
 **/
TEST_GROUP(f360_tracker_propagation_clusters)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"

   /** \setup
   * init the input test vector
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
**********************************************************************************************************
*                                            Initialize_Tracks                                           *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check the  propagation of  clusters.time_since_created to Initialize_Tracks.
 * \req
 * CPR-4532, CPR-4463
 **/
TEST(f360_tracker_propagation_clusters, check_Propagation_of_clusters_time_since_created_to_Initialize_track)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/

   /** \precond
    *- Changing  core_info.elapsed_time_s will result in a change to clusters[0].time_since_created that will change the output object_tracks[id].time_since_cluster_created
    *- set inputs to their original unchanged value 
    *- 
    **/
   /* the input default value from input vector is used */ 
   int id = tracker_info.inactive_obj_ids[0] - 1;

   /** \action
    * Call modules functions up to the Initialize_Tracks with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_NewObjInit_Module_call);
   float time_since_cluster_created_default = object_tracks[id].time_since_cluster_created;

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(object_tracks[id].time_since_cluster_created == time_since_cluster_created_default );
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
     *- Changing  core_info.elapsed_time_s will result in a change to the input clusters.time_since_created that will change the output object_tracks[id].time_since_cluster_created
    **/
   input_init();
   core_info.elapsed_time_s=0.09F;
   id = tracker_info.inactive_obj_ids[0] - 1;

   /** \action
    * Call modules functions up to the Initialize_Tracks  that will affect object_tracks[id].time_since_cluster_created 
    **/
   F360_Tracker_Modules_Call(Up_To_NewObjInit_Module_call);

   /** \result
     *- check that object_tracks[id].time_since_cluster_created  sensed the change and  propagated with the correct value which not equal to time_since_cluster_created_default
    **/
   CHECK(object_tracks[id].time_since_cluster_created !=time_since_cluster_created_default);
}
/** @}*/
