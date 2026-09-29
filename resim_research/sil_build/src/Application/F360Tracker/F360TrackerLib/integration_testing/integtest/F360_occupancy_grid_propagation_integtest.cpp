/** \file
*   This file aims to test the propagation of occupancy grid interface signals.
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
#include "f360_occlusion.h"
#include "f360_get_wall_time.h"
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


/** \defgroup  f360_tracker_propagation_occupancy_grid
 *  @{
 */

/** \brief
*  This test suit aims to test the propagation of occupancy grid interface signals
**/
TEST_GROUP(f360_tracker_propagation_occupancy_grid)
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
*                                            Track_Classification                              *
**********************************************************************************************************
**/                                           
/** \purpose  
 *  This test will check the  propagation of occupancy_grid.underdrivability.underdrivability_status[32][0] to Track_Classification.
 * \req
 * CPR-4429, CPR-4430, CPR-4431
 */
TEST(f360_tracker_propagation_occupancy_grid, check_Propagation_of_occupancy_grid_to_Track_Classification)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
      /* the input default value from input vector is used */ 

   /** \action
    * Call modules functions up to the Track_Classification with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- check that the output has its original correct value
    **/
   CHECK_EQUAL(occupancy_grid.underdrivability.underdrivability_classification[32][0].underdrivability_status, ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
   /** \precond
     *- change occupancy_grid.underdrivability.underdrivability_status[32][0] and test its propagation
     *- Set guardrail parameters.
    **/
    input_init();
    occupancy_grid.underdrivability.underdrivability_classification[32][0].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
 
   /** \action
    * Call modules functions up to the Track_Classification that will affect occupancy_grid.underdrivability.underdrivability_status
    **/
    F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *check that occupancy_grid.underdrivability.underdrivability_status[32][0] is propagated with the correct value
    **/
    CHECK_EQUAL(occupancy_grid.underdrivability.underdrivability_classification[32][0].underdrivability_status, ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER); 
}
/** @}*/
