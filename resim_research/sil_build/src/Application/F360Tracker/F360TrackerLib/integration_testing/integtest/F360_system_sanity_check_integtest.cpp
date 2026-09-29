/** \file
  This file test the sanity of the tracker module system
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
#include "f360_sanity_check.h"
#include "f360_sensor_postprocessing.h"
#include "f360_get_wall_time.h"
#include "f360_occlusion.h"
#include "ocg_occupancy_grid_types.h"
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

/** \defgroup  f360_tracker_propagation_system_sanity
 *  @{
 */

 /** \brief
 *  This test suit test the sanity of the tracker module system
 **/
TEST_GROUP(f360_tracker_propagation_system_sanity)
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

/** \purpose  
 *  This test will verify the sanity of the system  
 * \req
 * N/A
 */
 
 TEST(f360_tracker_propagation_system_sanity, check_Propagation_object_tracks_to_SanityCheck)
{
   /** \precond
    * Change det_hist.det_data[0].time_since_meas such that
    *    - the detection is removed from its corresponding cluster due to too large age
    *    - the cluster is killed due to lack of detections
    **/
   det_hist.det_data[0].time_since_meas = 2.0F;

   const int cluster_idx = det_hist.det_data[0].cluster_idx;
   
   CHECK_TRUE(det_hist.det_data[0].cluster_idx == cluster_idx); /* Pass here means that the data has been correctly passed */
   CHECK_TRUE(clusters[cluster_idx].num_old_dets == 1); /* Pass here means that the data has been correctly passed */
   CHECK_TRUE(clusters[cluster_idx].old_det_idx[0] == 0); /* Pass here means that the data has been correctly passed */

   /** \action
    * Call modules functions up to PostProcessor that will affect the cluster and the detection.
    **/    
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);

    /** \result
     * check that the system behaves as expected. I.e. that the data has been correctly propagated
     *  such that the cluster is killed and the historical detection is no longer associated to the cluster
    **/
   CHECK_TRUE(det_hist.det_data[0].cluster_idx != cluster_idx); /* Pass here means test values have propagated successfully */
   CHECK_TRUE(clusters[cluster_idx].num_old_dets == 0); /* Pass here means test values have propagated successfully */
   CHECK_TRUE(clusters[cluster_idx].old_det_idx[0] == 0); /* Pass here means test values have propagated successfully */ 
}
/** @}*/