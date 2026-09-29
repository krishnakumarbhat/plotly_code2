/** \file
*   This file aims to test the propagation of tracker_info interface signals.
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

/** \defgroup  f360_tracker_propagation_tracker_info
 *  @{
 */

 /** \brief
 *  This test suit aims to test the propagation of tracker_info interface signals
 **/
TEST_GROUP(f360_tracker_propagation_tracker_info)
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
*                                            PreProcessor                                              *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check propagation of tracker_info.time_us  to  PreProcessor  module.
 * \req
 * CPR-4533, CPR-4442, CPR-4463 
 */
TEST(f360_tracker_propagation_tracker_info, check_Propagation_of_tracker_info_time_us_s_to_PreProcessor)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/

   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   float expected_time_sensor= 0.106808F;
   CHECK(sensor_props[0].time_since_measurement_s == expected_time_sensor);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   

   /** \precond
     *change   tracker_info.time_us  value which will result in a change to sensor_props[0].time_since_measurement_s
     **/
   input_init();
   tracker_info.time_us = (uint64_t)980400099;

   /** \action
    * Call modules functions up to the PreProcessor  that will affect sensor_props[0].time_since_measurement_s
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that sensor_props[0].time_since_measurement_s , sensed the change and  propagated with the correct value
     **/
   CHECK(sensor_props[0].time_since_measurement_s == expected_time_sensor);
}

/**
**********************************************************************************************************
*                                            TrackValidity                                               *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check propagation of tracker_info.elapsed_time_s  to  TrackValidity  module.
 * \req
 * CPR-4533, CPR-4442, CPR-4463 
 */
TEST(f360_tracker_propagation_tracker_info, check_Propagation_of_tracker_info_elapsed_time_s_to_TrackValidity)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/

   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   
   /** \action
    * Call modules functions up to the TrackValidity with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   F360_Tracker_Modules_Call(TrackValidity_Module_single_call);
   float confidenceLevel_default = object_tracks[0].confidenceLevel;

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(object_tracks[0].confidenceLevel == confidenceLevel_default);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   

   /** \precond
     *change   tracker_info.elapsed_time_s  value which will result in a change to object_tracks[0].confidenceLevel
     **/
   input_init();
   tracker_info.elapsed_time_s = 0.001;

   /** \action
    * Call modules functions up to the TrackValidity  that will affect object_tracks[0].confidenceLevel.
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_track_updated = calibrations.k_hyst_time_for_coasted_objects - 0.001F;
   object_tracks[0].num_rr_inlier_dets = 2;

   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   /** \result
     *check that  object_tracks[0].confidenceLevel , sensed the change and  propagated with the correct value
     **/
   CHECK(object_tracks[0].confidenceLevel != confidenceLevel_default);
}

/**
**********************************************************************************************************
*                                            TrackDownSelection                                          *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check propagation of trakcer_info.elapsed_time_s  to  TrackDownSelection  module.
 * \req
 * CPR-4533, CPR-4442, CPR-4463 
 */
TEST(f360_tracker_propagation_tracker_info, check_Propagation_of_tracker_info_elapsed_time_s_to_TrackDownSelection)
{
/** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   host.dist_rear_axle_to_vcs_m = 2.5F;

   /** \action
    * Call modules functions up to the TrackDownSelection with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   object_tracks[0].vcs_position.x = -5.0F;
   object_tracks[0].vcs_position.y = -2.0F;
   object_tracks[0].vcs_velocity.longitudinal = 30.0F;
   object_tracks[0].conf_overall  = (CONF3_T)1;
   object_tracks[0].confidenceLevel   = 1.0F;
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
     *- check that the output  has its original correct value 
    **/ 
   CHECK(object_tracks[0].reduced_id > 0);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- changing trakcer_info.elapsed_time_s and test its propagation
    **/
   input_init();
   tracker_info.elapsed_time_s = F360_EPSILON - 0.001F;
   host.dist_rear_axle_to_vcs_m = 2.5F;

   /** \action
    * Call module functions up to the TrackDownSelection with the changed tracker_info.elapsed_time_s value that will affect object_tracks[0].reduced_id
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   object_tracks[0].vcs_position.x = -5.0F;
   object_tracks[0].vcs_position.y = -2.0F;
   object_tracks[0].vcs_velocity.longitudinal = 30.0F;
   object_tracks[0].conf_overall  = (CONF3_T)0;
   object_tracks[0].confidenceLevel   = 1.0F;
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
    *check that object_tracks[0].reduced_id sensed the tracker_info.elapsed_time_s change and has the correct value
    **/
   CHECK(object_tracks[0].reduced_id == 0);     
}

/**
**********************************************************************************************************
*                                            SensorPostProcessing                                          *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check the  propagation of track_info.num_active_clusters to Sensor_Postprocessor.
 * \req
 * CPR-4533, CPR-4442, CPR-4463 
 */
TEST(f360_tracker_propagation_tracker_info, check_Propagation_of_track_info_num_active_clusters_to_SensorPostprocessing)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   det_hist.n_occupied = 326;       
   
   /** \action
    * Call modules functions up to the Sensor_Postprocessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(det_hist.max_occupation == 372);
   
   /** \step{2}
     *- Test input  propagation with a changed value
    **/   
    
   /** \precond
     *- change  det_hist.n_occupied and test its propagation
    **/
   input_init();
   tracker_info.num_active_clusters = 0;
   det_hist.n_occupied = 326;       

   /** \action
    * Call modules functions up to the Sensor_Postprocessor  that will affect detection_hist.max_occupation
    **/
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);

   /** \result
     *check that detection_hist.max_occupation sensed the change and  propagated with the correct value
    **/
   CHECK(det_hist.max_occupation != 369);
}
/** @}*/
