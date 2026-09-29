/** \file
*   This file aims to test the propagation of sensor_props interface signals.
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

/** \defgroup  f360_tracker_propagation_sensor_props
 *  @{
 */

/** \brief
*  This test suit aims to test the propagation of sensor_props interface signals
**/
TEST_GROUP(f360_tracker_propagation_sensor_props)
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
*                                            PreProcessor                              *
**********************************************************************************************************
**/
                                             
/** \purpose  
 *  This test will check the  propagation of sensor_props[0].time_since_measurement_s to PreProcessor.
 * \req
 * CPR-4523
 */
TEST(f360_tracker_propagation_sensor_props, check_Propagation_of_sensor_props_PreProcessor)
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
   DOUBLES_EQUAL(raw_detection_list.detections[0].processed.time_since_measurement,0.106808,1e-6F);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- by changing tracker_info.time_us that will result in a change to sensor_props[0].time_since_measurement_s
    *- change sensor_props[0].time_since_measurement_s signals and test its propagation.
    **/
   input_init();
   core_info.time_us = (uint64_t)980497192;
 
   /** \action
    * Call modules functions up to the PreProcessor  that will affect detections[0].time_since_measurement.
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].time_since_measurement sensed the change and  propagated with the correct value= 0.2        
    **/
   DOUBLES_EQUAL(raw_detection_list.detections[0].processed.time_since_measurement,0.2F,1e-6F);
}

/**
**********************************************************************************************************
*                                            Trailer_Detector_Core                                               *
**********************************************************************************************************
**/
/** \purpose  
 *  This test will check the propagation of sensors[0].constant.mounting_location to Trailer Detector module.
 * \req
 * CPR-4523
 */
TEST(f360_tracker_propagation_sensor_props, check_Propagation_of_sensors_mounting_location_TrailerDetector)
{
   /** \step{1}
    * Nominal test
    **/

   /** \precond
    * - Re-initialize the TrackerCoreDetector class
    * - Setup default data to use in the test for the nominal case:
	  *     One detection that is
	  *       - not flagged by any waterspray, double bounce, wheel spin flags etc
		*       - from the a rear sensor
		*       - 0 range rate
		*       - position inside of the box behind host that is defined by: xpos in [-5.7, -9.7] and ypos in [-1.75, 1.75]
    *     Host vehicle with
	  *       - Large speed
	  *       - 0 yaw rate
	  *       - distance to rear axle from default input value
    **/
   trailer_detector_core.Initialize();

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F;
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;

   raw_detection_list.number_of_valid_detections = 1U;
   raw_detection_list.detections[0].raw.sensor_id = 1U;
   raw_detection_list.detections[0].raw.range_rate = 0.0F;
   raw_detection_list.detections[0].raw.range = 2.0F; // Note: Detection vcs position is given by sensor position + detection range + detection azimuth
   raw_detection_list.detections[0].raw.azimuth = 0.0F; // Note: Detection vcs position is given by sensor position + detection range + detection azimuth
   detections[0].f_double_bounce = false;
   detections[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   detections[0].f_water_spray = false;

   host.speed = 20.0F;
   host.yaw_rate_rad = 0.0F;

   
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 200 times (200 times is needed because if called few 
    * times than this the output is always 0 no matter what the internal state of the TrackerDetectorCore class is).
    * Extract the output from the trailer detector class.
    **/
   for (uint32_t i = 0U; i < 199U; i++)
   {
         F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);
   }
   F360_Tracker_Modules_Call(Up_To_Trailer_Detector_Core_Module_call);
   const Trailer_Detector_Flt_Fus_Output nominal_output = trailer_detector_core.Get_Trailer_Detector_Output();


   /** \result
     * Check that trailer detector detects the presence of a trailer
    **/
   CHECK_EQUAL_TEXT(TRAILER_PRESENCE_STATE_DETECTED, nominal_output.trailer_presence, "Nominal test failed");    
   
   /** \step{2}
    * Test where host speed is changed from its nominal value
    **/

   /** \precond
    * - Re-initialize the TrackerCoreDetector class
    * - Setup default data to use in the test. Use the same as for the nominal case except for:
    *     The sensor mounting location is changed from REAR to FORWARD
    **/
   trailer_detector_core.Initialize(); // Same as in nominal test

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;  // Different from nominal test.
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F; // Same as in nominal test

   raw_detection_list.number_of_valid_detections = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.sensor_id = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.range_rate = 0.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.range = 2.0F;  // Same as in nominal test
   raw_detection_list.detections[0].raw.azimuth = 0.0F; // Same as in nominal test
   detections[0].f_double_bounce = false; // Same as in nominal test
   detections[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; // Same as in nominal test
   detections[0].f_water_spray = false; // Same as in nominal test

   host.speed = 20.0F; // Same as in nominal test
   host.yaw_rate_rad = 0.0F; // Same as in nominal test
   
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 200 times (200 times is needed because if called few 
    * times than this the output is always 0 no matter what the internal state of the TrackerDetectorCore class is).
    * Extract the output from the trailer detector class.
    **/
   for (uint32_t i = 0U; i < 199U; i++)
   {
         F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);
   }
   F360_Tracker_Modules_Call(Up_To_Trailer_Detector_Core_Module_call);
   const Trailer_Detector_Flt_Fus_Output non_nominal_output = trailer_detector_core.Get_Trailer_Detector_Output();


   /** \result
     * Check that trailer detector does not detect the precense of a trailer
    **/
   CHECK_EQUAL_TEXT(TRAILER_PRESENCE_STATE_NOT_DETECTED, non_nominal_output.trailer_presence, "Non-nominal test failed");  
}
/** @}*/
