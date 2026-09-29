/** \file
*   This file aims to test the propagation of core_info interface signals.
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

/** \defgroup  f360_tracker_propagation_core_info
 *  @{
 */

 /** \brief
 *  This test suit aims to test the propagation of core_info interface signals
 **/
TEST_GROUP(f360_tracker_propagation_core_info)
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
*                                            PreProcessor                                                *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check propagation of core_info.elapsed_time_s  to  PreProcessor  module.
 * \req
 * CPR-4434, CPR-4438
 */
TEST(f360_tracker_propagation_core_info, check_Propagation_of_core_info_elapsed_time_s_to_preprocessor)
{
/** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   float32_t time_since_cluster_created_default = object_tracks[0].time_since_cluster_created;
   
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   DOUBLES_EQUAL(object_tracks[0].time_since_cluster_created, core_info.elapsed_time_s + time_since_cluster_created_default, 1e-6F);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
   /** \precond
     *change   core_info.elapsed_time_s  value which will result in a change to object_tracks[0].time_since_cluster_created
     **/
   input_init();
   core_info.elapsed_time_s = 1.0F;
   time_since_cluster_created_default = object_tracks[0].time_since_cluster_created;

   /** \action
    * Call modules functions up to the TimeUpdate  that will affect object_tracks[0].time_since_cluster_created.
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that  object_tracks[0].time_since_cluster_created , sensed the change and  propagated with the correct value= core_info.elapsed_time_s+time_since_cluster_created_default 
     **/
   DOUBLES_EQUAL(object_tracks[0].time_since_cluster_created, core_info.elapsed_time_s + time_since_cluster_created_default, 1e-6F);
}

/**
**********************************************************************************************************
*                                               TimeUpdate                                               *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check propagation of core_info.elapsed_time_s to TimeUpdate module.
 * \req
 * CPR-4434, CPR-4438
 */
TEST(f360_tracker_propagation_core_info, check_Propagation_of_core_info_elapsed_time_s_to_TimeUpdate)
{
/** \step{1}
     * Test input propagation without a change in the inputs
    **/
    
   /** \precond
     * set inputs to their original unchanged value
    **/
   /* the input default value from input vector is used */ 
   const float host_yaw_rate = host.yaw_rate_rad;
   
   /** \action
    * Call modules functions up to the TimeUpdate with the original input unchanged value
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
     * check that the output has its original correct value
    **/
   DOUBLES_EQUAL(core_info.elapsed_time_s * host_yaw_rate, host_props.delta_pointing, 1e-6F);
   
   /** \step{2}
     * Test input propagation with a changed value
    **/   
    
   /** \precond
     *change core_info.elapsed_time_s value which will result in a change to host_props.delta_pointing
     **/
   input_init();
   core_info.elapsed_time_s = 0.5F;
   /** \action
    * Call modules functions up to the TimeUpdate  that will affect host_props.delta_pointing.
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
     *check that host_props.delta_pointing sensed the change and propagated with the correct value = core_info.elapsed_time_s * host_yaw_rate
     **/
   DOUBLES_EQUAL(core_info.elapsed_time_s * host_yaw_rate, host_props.delta_pointing, 1e-6F);
}
/** @}*/
