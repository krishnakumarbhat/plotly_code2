/** \file
*   This file aims to test the propagation of detection proprieties interface signals
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


/** \defgroup  f360_tracker_propagation_Detection_props
 *  @{
 */

 /** \brief
 *  This test suit aims to test the propagation of detection props interface
 **/
TEST_GROUP(f360_tracker_propagation_Detection_props)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"

   const float tolerance = 0.0000001F;

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
   TEST_TEARDOWN()
   {
        
   }

};

/**
**********************************************************************************************************
*                                               PreProcessor                                             *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check propagation of detections[0].cos_vcs_az to PreProcessor module.
 * \req
 * CPR-4526 , CPR-4346
 */
TEST(f360_tracker_propagation_Detection_props, check_Propagation_of_detections_cos_vcs_az_to_PreProcessor)
{
   /** \step{1}
     *- Test input propagation without changing inputs 
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
   float det_props_vcs_position_long_default = detections[0].vcs_position.x;
   CHECK(detections[0].vcs_position.x == det_props_vcs_position_long_default);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
     
   /** \precond
     *- we cannot change the input directly as it will be overwritten in later modules so we change raw_detection_list.detections[0].azimuth that will lead to input change. 
     **/
   input_init();
   raw_detection_list.detections[0].raw.azimuth  =  3;
   /** \action
    * Call modules functions up to the PreProcessor  that will affect detections[0].vcs_position.x
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    *- check that detections[0].vcs_position.x, sensed the propagated detections[0].cos_vcs_az  change and has the correct value. 
    **/
   CHECK(detections[0].vcs_position.x != det_props_vcs_position_long_default);    
}
/**
**********************************************************************************************************
*                                            PreAssociation                                              *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check propagation of detections[0].f_object_based_angle_jump to PreAssociation module.
 * \req
 * CPR-4526 , CPR-4346
 */
TEST(f360_tracker_propagation_Detection_props, check_Propagation_of_detections_f_object_based_angle_jump_to_PreAssociation)
{
   /** \step{1}
     *- Test input propagation without changing inputs 
    **/

   /** \precond
     *-Note: detections structure here represent detection_props not raw_detection_list.detections
     *- Test input detections[0].f_object_based_angle_jump with original value =false
     **/
   /* the input default value from input vector is used */ 
   raw_detection_list.detections[0].raw.azimuth = 0.5;  /* will change detections[0].vcs_ac */
   calibrations.rp_max_object_lateral_distance = 25.0F;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 85.0F;
   calibrations.obj_aj_suspected_rr_handicap = -12.0F;
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   //F360_Tracker_Modules_Call(TimeUpdate_Module_single_call);
   object_tracks[0].vcs_position.y = 0.1F;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_moving = true;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
   F360_Tracker_Modules_Call(Up_To_PreAssociation_Module_call);
   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK_TRUE(detections[0].f_ok_to_use);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
     
   /** \precond
     *- we cannot change input  detections[0].f_object_based_angle_jump  directly so we do the following preconditions to set it to true and then it will affect the output 
     *- set raw_detection_list,sensors,calibrations and object_tracks parameters 
     **/
   input_init();
   raw_detection_list.detections[0].raw.range = 0.1F; /* will change input */
   raw_detection_list.detections[0].raw.azimuth = 0.5;  /* will change detections[0].vcs_ac */
   calibrations.rp_max_object_lateral_distance = 25.0F;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 85.0F;
   calibrations.obj_aj_suspected_rr_handicap = -12.0F;

   /** \action
    * Call modules functions up to the Pre_Association_Track_Management  that will affect detections[0].f_ok_to_use.
    **/
   //F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);
   object_tracks[0].vcs_position.y = 0.1F;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_moving = true;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
   object_tracks[0].confidenceLevel = calibrations.rp_min_confidence_level + tolerance;

   F360_Tracker_Modules_Call(PreAssociation_Module_single_call);

   /** \result
    *- check that detections[0].f_ok_to_use, sensed the propagated detections[0].f_object_based_angle_jump  change and has the correct value= false. 
    **/
   CHECK(detections[0].f_ok_to_use == false);
}
/**
**********************************************************************************************************
*                                            Sensor PostProcessing                                              *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check propagation of detections[0].vcs_position.x  to Sensor PostProcessing module.
 * \req
 * CPR-4526 , CPR-4346
 */
TEST(f360_tracker_propagation_Detection_props, check_Propagation_of_detections_vcs_position_longitudinal_to_PostProcessor)
{
   /** \step{1}
     *- Test input propagation without changing inputs 
    **/
   /** \precond
     *- set inputs to their original unchanged value 
     **/
   /* the input default value from input vector is used */ 

   /** \action
    * Call modules functions up to the PostProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);
  
   /** \result
     *- check that the output  has its original correct value 
    **/
   float det_hist_vcs_position_long_default = det_hist.det_data[158].vcs_position.x;
   CHECK(det_hist.det_data[158].vcs_position.x == det_hist_vcs_position_long_default);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
     
   /** \precond
     *- we cannot change the input directly as it will be overwritten in later modules so we change raw_detection_list.detections[0].range that will lead to input change. 
     **/
   input_init();
   raw_detection_list.detections[0].raw.range =  7.0F;
   /** \action
    * Call modules functions up to the PostProcessor  that will affect det_hist.det_data[158].vcs_position.x
    **/
   F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);

   /** \result
    *- check that detections[0].f_ok_to_use, sensed the propagated det_hist.det_data[158].vcs_position.x  change and has the correct value= false. 
    **/
   CHECK(det_hist.det_data[158].vcs_position.x != det_hist_vcs_position_long_default);    
}

/**
**********************************************************************************************************
*                                            Trailer_Detector_Core                                               *
**********************************************************************************************************
**/
/** \purpose
 *  This test will check propagation of detections[0].vcs_position.longitudinal to Trailer Detector module.
 * \req
 * CPR-4526 , CPR-4346
 */
TEST(f360_tracker_propagation_Detection_props, check_Propagation_of_detections_vcs_position_longitudinal_to_Trailer_Detector)
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
    *     The detection range is changed to 100m (NOTE: Since det_props is cleared in the beginning of the tracker then it is not really possible to set any det_props from outside. Instead det_props has to be changed by changing raw_dets.)
    **/
   trailer_detector_core.Initialize(); // Same as in nominal test

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F; // Same as in nominal test

   raw_detection_list.number_of_valid_detections = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.sensor_id = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.range_rate = 0.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.range = 100.0F; // Different from nominal test. NOTE: Since det_props is cleared in the beginning of the tracker then it is not really possible to set any det_props from outside. Instead det_props has to be changed by changing raw_dets.
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
