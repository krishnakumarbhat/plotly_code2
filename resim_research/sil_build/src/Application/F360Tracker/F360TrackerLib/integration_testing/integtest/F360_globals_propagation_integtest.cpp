/** \file
*   This file aims to test the propagation of global interface signals.
*/
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cfloat>
#include <cmath>
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

/** \defgroup  f360_tracker_propagation_globals
 *  @{
 */

 /** \brief
 *  This test suit aims to test the propagation of globals interface signals
 **/
TEST_GROUP(f360_tracker_propagation_globals)
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
*                                            Initialize_Tracks                              *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check the  propagation of  globals.obj_vehicular_spd_thresh  to Initialize_Tracks.
 * \req
 * CPR-4517
 **/
TEST(f360_tracker_propagation_globals, check_Propagation_of_globals_obj_vehicular_spd_thresh_to_Initialize_track)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
     *- set calibrations parameter
    **/
   /* the input default value from input vector is used */ 
   calibrations.k_nonmoveable_target_diameter = 15.0F;     
   int obj_idx = tracker_info.inactive_obj_ids[0] - 1U;

   /** \action
    * Call modules functions up to the Initialize_Tracks with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_NewObjInit_Module_call);
   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK_TRUE(std::abs(object_tracks[obj_idx].bbox.Get_Length() - 15.0F) < F360_EPSILON);
   
   /** \step{2}
     *- Test input  propagation with a changed value
    **/   
    
   /** \precond
     *- Change input signal and test its propagation
     *- Set  calibrations parameters
    **/
   input_init();
   calibrations.k_nonmoveable_target_diameter = 15.0F;
   obj_idx = tracker_info.inactive_obj_ids[0] - 1U;
   
   /** \action
    * Call modules functions up to the Initialize_Tracks that will affect object_tracks[obj_idx].bbox.Get_Length().
    **/
   F360_Tracker_Modules_Call(Up_To_MsmtUpdate_Module_call);
   
   globals.obj_mov_stat_spd_thresh = -0.1F;  /*  object_tracks[obj_idx].speed = 0 so to have this negation test, the input is changed here because a value of zero is needed and the input is changed with predetermined value in PreProcessor module and 0 isn't among them */
   
   F360_Tracker_Modules_Call(NewObjInit_Module_single_call);
   /** \result
     *- check that oobject_tracks[obj_idx]..bbox.Get_Length() sensed the change and  propagated with the correct value != 15.
    **/
   CHECK_FALSE(std::abs(object_tracks[obj_idx].bbox.Get_Length() - 15.0F) < F360_EPSILON);
}


/**
**********************************************************************************************************
*                                            Track_Classification                              *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check the  propagation of  globals.obj_vehicular_spd_thresh  to Track_Classification.
 * \req
 * CPR-4517
 */
TEST(f360_tracker_propagation_globals, check_Propagation_of_globals_obj_vehicular_spd_thresh_thresh_to_Track_Classification)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
     *- set calibrations parameter
    **/
   /* the input default value from input vector is used */ 
   object_tracks[0].f_vehicular_trk = false;

   /** \action
    * Call modules functions up to the Track_Classification with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);


   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(object_tracks[0].f_vehicular_trk == true );    

   /** \step{2}
     *- Test input  propagation with a changed value
    **/   
   /** \precond
     *- change  globals.obj_vehicular_spd_thresh value that will affect object_tracks[0].f_vehicular_trk
     *- set object_tracks parameters
    **/
   input_init();
   globals.obj_vehicular_spd_thresh = 26.0F;
   object_tracks[0].f_vehicular_trk = false;
   /** \action
    * Call modules functions up to the Track_Classification  that will affect object_tracks[0].f_vehicular_trk.
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *check that object_tracks[0].f_vehicular_trk sensed the change and  propagated with the correct value= true
    **/
   CHECK(object_tracks[0].f_vehicular_trk == false );    
}

/** @}*/
