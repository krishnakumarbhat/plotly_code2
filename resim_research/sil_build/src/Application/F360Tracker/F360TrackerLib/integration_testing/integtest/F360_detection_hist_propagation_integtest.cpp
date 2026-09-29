/** \file
  This file aims to test the detections hist interface propagation
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

/** \defgroup  f360_tracker_propagation_Detection_hist
 *  @{
 */

 /** \brief
 *  This test suits aim to test the propagation of detection_hist interface
 **/
TEST_GROUP(f360_tracker_propagation_Detection_hist)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"

   /** \setup
   * Init the input test vector
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
*                                            Clustering                                                  *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check propagation of det_hist.det_data[79].f_dealiased to Clustering module.
 * \req
 * CPR-4527, CPR-4463, CPR-4528
 */
TEST(f360_tracker_propagation_Detection_hist, check_Propagation_of_det_hist_det_data_f_dealiased_jump_to_Cluster_Grouping)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value
     *- set cluster parameters
    **/
   /* the input default value from input vector is used */ 
   clusters[219].vcs_position.x = 138.0F;  
   
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Cluster_Grouping_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(clusters[219].f_dealiased== true);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change det_hist.det_data[79].f_dealiased to 0 that will affect  clusters[219].f_dealiased ,Note: det_hist.det_data[79].cluster_idx =219
    *- set clusters parameters.
    **/
   input_init();
   det_hist.det_data[79].f_dealiased = false;
   clusters[219].vcs_position.x = 138.0F;
   /** \action
    * Call modules functions up to the Clustering that will affect clusters[219].f_dealiased
    **/
   F360_Tracker_Modules_Call(Up_To_Cluster_Grouping_Module_call);

   /** \result
    *check that det_hist.clusters[219].f_dealiased is set to false.
    **/
   CHECK(clusters[219].f_dealiased== false);
}
/**
**********************************************************************************************************
*                                            Sensor PostProcessing                                       *
**********************************************************************************************************
**/
/** \purpose
 *  This test will check the  propagation of detection_hist.n_occupied to Sensor_Postprocessor.
 * \req
 * CPR-4527, CPR-4463
 */
TEST(f360_tracker_propagation_Detection_hist, check_Propagation_of_detection_hist_n_occupied_to_SensorPostprocessing)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   
   /** \action
    * Call modules functions up to the Sensor_Postprocessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   LONGS_EQUAL(det_hist.max_occupation,372);
   
   /** \step{2}
     *- Test input  propagation with a changed value
    **/   
    
   /** \precond
     *- change  det_hist.n_occupied and test its propagation
    **/
   input_init();
   det_hist.n_occupied = 330;       

   /** \action
    * Call modules functions up to the Sensor_Postprocessor  that will affect detection_hist.max_occupation
    **/
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);

   /** \result
     *check that detection_hist.max_occupation sensed the change and  propagated with the correct value =373
    **/
   LONGS_EQUAL(det_hist.max_occupation,376);
}
/** @}*/
