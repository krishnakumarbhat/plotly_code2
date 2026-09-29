/** \file
*   This file aims to test the propagation of some important interfaces(host,sensors,sensors) signals.
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

/** \defgroup  f360_tracker_propagation_important_signals
 *  @{
 */

/** \brief
 *  This test suit aims to test the propagation of some important host,sensors,sensors interfaces signals
 **/
TEST_GROUP(f360_tracker_propagation_important_signals)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"
    
   const float tolerance = 0.0000001F;

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
*                                              F360_Host_T                                               *
**********************************************************************************************************
**/

//                                                #host.speed
/** \purpose
 *  This test will check the propagation of  host.speed  to PreProcessor module
 * \req
 * CPR-4266, CPR-4268 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_speed_to_PreProcessor)
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
   DOUBLES_EQUAL(host_props.std_speed_scm, 0.109195F, 1e-6F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
     *-change host.speed  and test its  propagation
    **/
   input_init();
   host.speed = 25.0F;

   /** \action
    * -Call PreProcessor module functions with the changed host.speed value that will affect host_props.std_speed_scm
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    * -check that host_props.std_speed_scm  sensed the propagated host.speed change and has the correct value = =0.07984156F
    **/
   DOUBLES_EQUAL(host_props.std_speed_scm, 0.07984156F, 1e-6F);    
}

/** \purpose
 *  This test will check the propagation of  host.speed  to Downselection module
 * \req
 * CPR-4266, CPR-4268 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_speed_to_Downselection)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    /** \precond
      *- set inputs to their original unchanged value
     **/
     /* the input default value from input vector is used */

   /** \action
    * Call modules up to TrackDownSelection with the original input unchanged value
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].vcs_position.x = 15.0F;
   object_tracks[0].vcs_velocity.longitudinal = 1.0F;
   object_tracks[0].vcs_heading = Angle{-1.57F};
   object_tracks[0].conf_overall = CONF3_MED;
   object_tracks[0].f_low_confidence_level = false;
   object_tracks[0].confidenceLevel = 1.0F;
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
     *- check that object_tracks[0].reduced_id has its original correct value
    **/
   CHECK_TRUE(0 == object_tracks[0].reduced_id);

   /** \step{2}
     *-Test input  propagation with a changed value
    **/

    /** \precond
      *-change host.speed  and test its  propagation
     **/
   input_init();
   host.vcs_speed = 0.0F;

   /** \action
    * -Call modules up to TrackDownSelection with changed host.speed value that will affect object_track[0].reduced_id
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].vcs_position.x = 15.0F;
   object_tracks[0].vcs_velocity.longitudinal = 1.0F;
   object_tracks[0].vcs_heading = Angle{-1.57F};
   object_tracks[0].conf_overall = CONF3_MED;
   object_tracks[0].confidenceLevel = 1.0F;
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
    * -check that object_tracks[0].reduced_id sensed the propagated host.speed change and has the correct value
    **/
   CHECK_FALSE(0 == object_tracks[0].reduced_id);
}

/** \purpose
 *  This test will check the propagation of host.speed  to TrailerDetector module
 * \req
 * CPR-4266, CPR-4268 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_speed_to_Trailer_Detector)
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
    *     Host speed is 0
    **/
   trailer_detector_core.Initialize(); // Same as in nominal test

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F; // Same as in nominal test

   raw_detection_list.number_of_valid_detections = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.sensor_id = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.range_rate = 0.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.range = 2.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.azimuth = 0.0F; // Same as in nominal test
   detections[0].f_double_bounce = false; // Same as in nominal test
   detections[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; // Same as in nominal test
   detections[0].f_water_spray = false; // Same as in nominal test

   host.speed = 0.0F; // Different from in nominal test
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
   CHECK_EQUAL_TEXT(TRAILER_PRESENCE_STATE_UNKNOWN, non_nominal_output.trailer_presence, "Non-nominal test failed");  
}


//                                                #host.yaw_rate_rad

/** \purpose
 *  This test will check the  propagation of  host.yaw_rate_rad  to PreProcessor module
 * \req
 * CPR-4266  , CPR-4271 ,CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_yaw_rate_rad_to_PreProcessor)
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
   DOUBLES_EQUAL(host_props.vel_cov_scm[0][0], 0.01192362F ,1e-7F);
   DOUBLES_EQUAL(host_props.vel_cov_scm[0][1], 1.4464e-05F, 1e-7F);
   DOUBLES_EQUAL(host_props.vel_cov_scm[1][0], 1.4464e-05F, 1e-7F);
   DOUBLES_EQUAL(host_props.vel_cov_scm[1][1], 0.00138268F, 1e-7F);   
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *-changing host.yaw_rate_rad will result in changing the overall value of host_props.vel_cov_scm
    **/
   input_init();
   host.yaw_rate_rad = -0.01;

   /** \action
    * -Call PreProcessor modules functions with   the changed host.yaw_rate_rad value that will affect host_props.vel_cov_scm
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    *check that host_props.vel_cov_scm sensed the propagated host.yaw_rate_rad change and has the correct value.
    *host_props.vel_cov_scm[0][0]=0.011923598126F
    *host_props.vel_cov_scm[0][1]=0.000026452606F
    *host_props.vel_cov_scm[1][0]=0.000026452606F
    *host_props.vel_cov_scm[1][1]=0.003475171747F
    **/
   DOUBLES_EQUAL(host_props.vel_cov_scm[0][0], 0.011923598126F, 1e-7F);
   DOUBLES_EQUAL(host_props.vel_cov_scm[0][1], 0.000026452606F, 1e-7F);
   DOUBLES_EQUAL(host_props.vel_cov_scm[1][0], 0.000026452606F, 1e-7F);
   DOUBLES_EQUAL(host_props.vel_cov_scm[1][1], 0.003475171747F, 1e-7F);    
}

/** \purpose
 *  This test will check the  propagation of  host.yaw_rate_rad  to TrackClassification module
 * \req
 * CPR-4266  , CPR-4271 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_yaw_rate_rad_to_TrackClassification)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/    
   /* the input default value from input vector is used */ 
   calibrations.k_object_motion_min_consec_moving_cnt_th = 2 ;
   object_tracks[0].f_moving= false ;
   
   /** \action
    * Call modules functions up to the TrackClassification with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_PostUpdateTrack_Module_call);
   object_tracks[0].movable_prob = 0.0F ;
   F360_Tracker_Modules_Call(TrackClassification_Module_single_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(object_tracks[0].f_moving == true );
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *-changing host.yaw_rate_rad will result in changing the overall value of object_tracks[0].f_moving
    **/
   input_init();
   host.yaw_rate_rad = -0.1;
   calibrations.k_object_motion_min_consec_moving_cnt_th = 2 ;
   object_tracks[0].f_moving= false ;

   /** \action
    * -Call TrackClassification modules functions with   the changed host.yaw_rate_rad value that will affect object_tracks[0].f_moving
    **/
   F360_Tracker_Modules_Call(Up_To_PostUpdateTrack_Module_call);
   object_tracks[0].movable_prob = 0.0F ;
   F360_Tracker_Modules_Call(TrackClassification_Module_single_call);
   
   /** \result
    *check thatobject_tracks[0].f_moving sensed the propagated host.yaw_rate_rad change and has the correct value.
    **/
   CHECK(object_tracks[0].f_moving == false  );
 
}

/** \purpose
 *  This test will check the  propagation of  host.yaw_rate_rad  to Trailer_Detector_Core module
 * \req
 * CPR-4266  , CPR-4271 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_yaw_rate_rad_to_Trailer_Detector)
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
    * Extract the output trailer angle from the trailer detector class (so that we can later compare and make sure the non-nominal test generates a different value)
    **/
   for (uint32_t i = 0U; i < 199U; i++)
   {
      F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);
   }
   F360_Tracker_Modules_Call(Up_To_Trailer_Detector_Core_Module_call);
   const Trailer_Detector_Flt_Fus_Output nominal_output = trailer_detector_core.Get_Trailer_Detector_Output();
   const float32_t nominal_trailer_angle = nominal_output.trailer_angle; 
   
   /** \step{2}
    * Test where host yaw rate is changed from its nominal value
    **/

   /** \precond
    * - Re-initialize the TrackerCoreDetector class
    * - Setup default data to use in the test. Use the same as for the nominal case except for:
    *     Host yaw rate is large
    **/
   trailer_detector_core.Initialize(); // Same as in nominal test

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F; // Same as in nominal test

   raw_detection_list.number_of_valid_detections = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.sensor_id = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.range_rate = 0.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.range = 2.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.azimuth = 0.0F; // Same as in nominal test
   detections[0].f_double_bounce = false; // Same as in nominal test
   detections[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; // Same as in nominal test
   detections[0].f_water_spray = false; // Same as in nominal test

   host.speed = 20.0F; // Same as in nominal test
   host.yaw_rate_rad = 0.2F; // Different from in nominal test
   
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
     * Check that the non-nominal trailer angle and the nominal trailer angle differs
    **/
   CHECK_TRUE_TEXT(std::abs(nominal_trailer_angle - non_nominal_output.trailer_angle) > 1e-4F, "Non-nominal test failed");  
}

/** \purpose
 *  check propagation of  host.dist_rear_axle_to_vcs_m  to TrackDownSelection module
 * \req
 * CPR-4266  , CPR-4274, CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_dist_rear_axle_to_vcs_m_to_TrackDownSelection)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set input to their original unchanged value 
     *- set object_tracks and calibrations parameters
    **/
   /* the input default value from input vector is used */ 
   calibrations.k_conf_downselection_exclusion_box_lat = 1.0F;
   /** \action
    * Call modules functions up to the TrackDownSelection with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   object_tracks[0].vcs_position.x = -5.0F;
   object_tracks[0].vcs_position.y = -2.0F;
   object_tracks[0].vcs_velocity.longitudinal = 30.0F;
   object_tracks[0].conf_overall  = (CONF3_T)0;
   object_tracks[0].confidenceLevel   = 1.0F;
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(object_tracks[0].reduced_id == 0);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- changing host.dist_rear_axle_to_vcs_m and test its propagation
    *- set object_tracks and calibrations parameters
    **/
   input_init();
   host.dist_rear_axle_to_vcs_m = 2.5F;
   calibrations.k_conf_downselection_exclusion_box_lat =1.0F;

   /** \action
    * Call module functions up to the TrackDownSelection with the changed host.dist_rear_axle_to_vcs_m value that will affect object_tracks[0].reduced_id
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);
   object_tracks[0].vcs_position.x = -5.0F;
   object_tracks[0].vcs_position.y = -2.0F;
   object_tracks[0].vcs_velocity.longitudinal = 30.0F;
   object_tracks[0].conf_overall  = (CONF3_T)1;
   object_tracks[0].confidenceLevel   = 1.0F;
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
    *check that object_tracks[0].reduced_id sensed the propagated host.dist_rear_axle_to_vcs_m change and has the correct value
    *object_tracks[0].reduced_id > 0
    **/
   CHECK(object_tracks[0].reduced_id > 0);
}

/** \purpose
 *  This test will check the propagation of host.dist_rear_axle_to_vcs_m to TrailerDetector module
 * \req
 * CPR-4266, CPR-4268 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_host_dist_rear_axle_to_vcs_m_to_Trailer_Detector)
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
    * Extract the outputted estimated trailer angle from the trailer detector class so that we can later compare that the non-nominal test generates a different angle output value
    **/
   for (uint32_t i = 0U; i < 199U; i++)
   {
         F360_Tracker_Modules_Call(Up_To_PostProcessor_Module_call);
   }
   F360_Tracker_Modules_Call(Up_To_Trailer_Detector_Core_Module_call);
   const Trailer_Detector_Flt_Fus_Output nominal_output = trailer_detector_core.Get_Trailer_Detector_Output();
   const float32_t nominal_trailer_angle = nominal_output.trailer_angle;
   
   /** \step{2}
    * Test where host distance to rear axle is changed from its nominal value
    **/

   /** \precond
    * - Re-initialize the TrackerCoreDetector class
    * - Setup default data to use in the test. Use the same as for the nominal case except for:
    *     Host distance to rear axle is 1
    **/
   trailer_detector_core.Initialize(); // Same as in nominal test

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F; // Same as in nominal test

   raw_detection_list.number_of_valid_detections = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.sensor_id = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.range_rate = 0.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.range = 2.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.azimuth = 0.0F; // Same as in nominal test
   detections[0].f_double_bounce = false; // Same as in nominal test
   detections[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; // Same as in nominal test
   detections[0].f_water_spray = false; // Same as in nominal test

   host.speed = 2.0F; // Same as in nominal test
   host.yaw_rate_rad = 0.0F; // Same as in nominal test
   host.dist_rear_axle_to_vcs_m = 1.0F; // Different from in nominal test
   
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
     * Check that the non-nominal trailer angle and the nominal trailer angle differs
    **/
   CHECK_TRUE_TEXT(std::abs(nominal_trailer_angle - non_nominal_output.trailer_presence) > 1e-3F, "Non-nominal test failed");  
}


/**
**********************************************************************************************************
*                                              F360_Radar_Sensor_T                                       *
**********************************************************************************************************
*/

//                                                #sensors.variable.timestamp_us
/** \purpose
 *  this test will check  the propagation of  sensors.variable.timestamp_us  to PreProcessor module
 * \req
 * CPR-4308 , CPR-4442  , CPR-4309
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_timestamp_us_to_PreProcessor)
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
   DOUBLES_EQUAL(sensor_props[0].time_since_measurement_s,0.106808F, 1e-6F);       
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- change timestamp_us value and check the output for a correctly propagated
    **/
   input_init();
   sensors[0].variable.timestamp_us = 980300000;

   /** \action
    * Call module functions up to the PreProcessor with the changed sensors[0].variable.timestamp_us value that will affect sensor_props[0].time_since_measurement_s
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    *check that sensor_props[0].time_since_measurement_s sensed the propagated sensors[0].variable.timestamp_us  change and has the correct value.
    *sensor_props[0].time_since_measurement_s=0.1040000021F
    **/
   DOUBLES_EQUAL(sensor_props[0].time_since_measurement_s, 0.1040000021F, 1e-6F);    
}

/** \purpose
 * This test will check the propagation of  sensors.timestamp_us  to TimeUpdate module
 * \req
 * CPR-4308 , CPR-4442  , CPR-4309
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_timestamp_us_to_TimeUpdate)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   
   /** \action
    * Call modules functions up to the TimeUpdate with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   LONGS_EQUAL(tracker_info.object_list_timestamp,980297192);       
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
     *- according to equation sensor_props[0].time_since_measurement_s = static_cast<float32_t>(tracker_info->time_us - sensors[0].variable.timestamp_us) * 1e-6F;
    so we change timestamp_us value and check the output for a correctly propagated
    **/
   input_init();
   sensors[0].variable.timestamp_us = 980304000;
   
   /** \action
    * Call modules functions up to the TimeUpdate with the changed input value 
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
    *check that sensor_props[0].time_since_measurement_s sensed the propagated sensors[0].variable.timestamp_us  change and has the correct value.
    **/
   LONGS_EQUAL(tracker_info.object_list_timestamp,980304000);       
}

//                                                #sensors.number_of_valid_detections

/** \purpose
 *  This test will check  the propagation of  sensors.number_of_valid_detections  to PreProcessor module
 * \req
 * CPR-4308 , CPR-4442  , CPR-4310
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_number_of_valid_detections_to_PreProcessor)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */
   raw_detection_list.detections[70].raw.sensor_id  = 2;
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *- check that the output  has its original correct value
    **/
   LONGS_EQUAL(sensor_props[0].first_detection_list_idx,  0);
   LONGS_EQUAL(sensor_props[1].first_detection_list_idx, -1);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
     *- Change input value and test its propagation
     *- set raw_detection_list parameters
    **/
   input_init(); 
   sensors[0].variable.number_of_valid_detections = 70;
   raw_detection_list.detections[70].raw.sensor_id  = 2;

   /** \action
    * Call module functions up to the PreProcessor with the changed sensors[0].variable.number_of_valid_detections value that will affect sensor_props[1].first_detection_list_idx
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    *check that sensor_props[1].first_detection_list_idx sensed the propagated sensors[0].variable.number_of_valid_detections  change and has the correct value.
    **/
   LONGS_EQUAL(sensor_props[0].first_detection_list_idx, 0);
   LONGS_EQUAL(sensor_props[1].first_detection_list_idx, 70);
}

//                                             #sensors.look_index ->> not used inside the tracker modules ... CPR-4311

//                                                #sensors.vcs_velocity.longitudinal 

/** \purpose
 *  This test will check  the propagation of  sensors.vcs_velocity.longitudinal  to PreProcessor module
 * \req
 * CPR-4308 , CPR-4442, CPR-4319
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_vcs_velocity_longitudinal_to_PreProcessor)
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
   DOUBLES_EQUAL(detections[0].range_rate_compensated, 30.58644F, 1e-5F);
      
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    * change sensors[0].variable.vcs_velocity.longitudinal value to test its propagation
    **/
   input_init();
   sensors[0].variable.vcs_velocity.longitudinal = 50.0F;

   /** \action
    * Call module functions up to the PreProcessor with the changed sensors[0].variable.vcs_velocity.longitudinal value that will affect detections[0].vcs_cross_covariances_scm
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *- check that detections[0].vcs_cross_covariances_scm, sensed the propagated sensors[0].variable.vcs_velocity.longitudinal  change and has the correct values.
    **/
   DOUBLES_EQUAL(detections[0].range_rate_compensated, 44.23093F, 1e-5F);
}

/** \purpose
 *  This test will check the  propagation of  sensors.vcs_velocity.lateral  to PreProcessor module
 * \req
 * CPR-4308 , CPR-4442, CPR-4319
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_vcs_velocity_lateral_to_PreProcessor)
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
   DOUBLES_EQUAL(detections[0].range_rate_compensated, 30.58644F, 1e-5F);
     
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- change sensors[0].variable.vcs_velocity.lateral to test its propagation
    **/
   input_init(); 
   sensors[0].variable.vcs_velocity.lateral = 1.0F;

   /** \action
    * Call module functions up to the PreProcessor with the changed sensors[0].variable.vcs_velocity.lateral value that will affect detections[0].vcs_cross_covariances_scm
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].vcs_cross_covariances_scm, sensed the propagated sensors[0].variable.vcs_velocity.lateral  change and has the correct values.
    **/
   DOUBLES_EQUAL(detections[0].range_rate_compensated, 30.10039F, 1e-5F);

}
/** \purpose
 *  This test will check the  propagation of  sensors.vcs_velocity.lateral  to Association module
 * \req
 * CPR-4308 , CPR-4442, CPR-4319
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_vcs_velocity_lateral_to_Association)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   
   /** \action
    * Call modules functions up to the Association with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   DOUBLES_EQUAL(detections[44].range_rate_predicted, -12.806F, 1e-3F);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- change sensors[0].variable.vcs_velocity.lateral to test its propagation
    **/
   input_init(); 
   sensors[0].variable.vcs_velocity.lateral = 10.0F;

   /** \action
    * Call module functions up to the Association with the changed sensors[0].variable.vcs_velocity.lateral value that will affect detections[44].range_rate_predicted
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

   /** \result
     *check that detections[44].range_rate_predicted, sensed the propagated sensors[0].variable.vcs_velocity.lateral  change and has the correct values.
    **/
   DOUBLES_EQUAL(detections[44].range_rate_predicted, -13.282F, 1e-3F);
}

/** \purpose
 *  This test will check the  propagation of  sensors.vcs_velocity.longitudinal  to Association module
 * \req
 * CPR-4308 , CPR-4442, CPR-4319
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_vcs_velocity_longitudinal_to_Association)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   
   /** \action
    * Call modules functions up to the Association with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   DOUBLES_EQUAL(detections[44].range_rate_predicted, -12.806F, 1e-3F);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- change sensors[0].variable.vcs_velocity.longitudinal to test its propagation
    **/
   input_init(); 
   sensors[0].variable.vcs_velocity.longitudinal = 33.0F;

   /** \action
    * Call module functions up to the Association with the changed sensors[0].variable.vcs_velocity.longitudinal value that will affect detections[44].range_rate_predicted
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

   /** \result
     *check that detections[44].range_rate_predicted, sensed the propagated sensors[0].variable.vcs_velocity.longitudinal  change and has the correct values.
    **/
   DOUBLES_EQUAL(detections[44].range_rate_predicted, -11.489F, 1e-3F);
}
//                                              #sensors.variable.look_id
/** \purpose
 *  This test will check the  propagation of  sensors.variable.look_id  to PreProcessor module
 * \req
 * CPR-4308 , CPR-4442, CPR-4312
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensor_look_id_to_PreProcessor)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   sensors[0].constant.range_limits[2] = 1.0F;

   
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.7F, 1e-3F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- set sensors[0].range_limits[look_id]
    *- set sensors[0].variable.look_id to test its propagation.
    **/
   input_init();
   sensors[0].variable.look_id = F360_DET_LOOK_ID_2;
   sensors[0].constant.range_limits[2] = 1.0F;

   /** \action
    * Call module functions up to the PreProcessor with the changed change sensors[0].variable.look_id  value that will affect detections[0].probability_of_detection
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].probability_of_detection, sensed the propagated sensors[0].variable.look_id  change and has the correct value= 0.1
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.1F, 1e-3F);
}

/** \purpose
 *  This test will check the  propagation of  sensors[0].variable.look_id  to Association module
 * \req
 * CPR-4308 , CPR-4442, CPR-4312
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensor_look_id_to_Association)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   sensors[0].constant.min_aliaised_range_rate[3] -= sensors[0].constant.v_wrapping[3];
   raw_detection_list.detections[44].raw.range_rate -=  sensors[0].constant.v_wrapping[3];
   /** \action
    * Call modules functions up to the Association with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(detections[44].f_dealiased == true  );    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   
    
   /** \precond
    *- set sensors[0].variable.look_id to test its propagation.
    **/
   input_init();
   sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
   sensors[0].constant.min_aliaised_range_rate[3] -= sensors[0].constant.v_wrapping[3];
   raw_detection_list.detections[44].raw.range_rate -=  sensors[0].constant.v_wrapping[3];
  
  /** \action
    * Call module functions up to the Association with the changed change sensors[0].variable.look_id  value that will affect detections[44].f_dealiased
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

   /** \result
     *check that detections[44].f_dealiased, sensed the propagated sensors[0].variable.look_id  change and has the correct value= false 
    **/
   CHECK(detections[44].f_dealiased == false );    
}
/**
**********************************************************************************************************
*                                              F360_Radar_Sensor_T                                 *
**********************************************************************************************************
*/

//                                                #sensors.constant.sensor_type
/** \purpose
 *  This test will check the  propagation of  sensors.constant.sensor_type  to PreProcessor module
 * \req
 * CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_sensor_type_to_PreProcessor)
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
   CHECK(detections[0].f_dealiased == false);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   

    /** \precond
    *- change sensors[0].constant.sensor_type to test its propagation
    **/
   input_init();
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_ESR_RADAR;

   /** \action
    * Call module functions up to the PreProcessor with the changed change sensors[0].constant.sensor_type  value that will affect detections[0].f_dealiased
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].f_dealiased, sensed the propagated sensors[0].constant.sensor_type change and has the correct value= true.
    **/
   CHECK(detections[0].f_dealiased == true ); 
}
/** \purpose
 *  This test will check the  propagation of  sensors[0].constant.sensor_type  to TrackClassification module
 * \req
 * CPR-4304 
 */
IGNORE_TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_sensor_type_to_TrackClassification)
{
   // This test has been intentionally left to fail,  since it has not been decided how to deal with this propogation test
   // Will be dealt in DFD-1929
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 

   /** \action
    * Call modules functions up to the TrackClassification with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);

   /** \result
     *- The output has been intentionally set to fail, since it has not been decided how to deal with this propogation test 
    **/
   CHECK(false); 
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   

    /** \precond
    *- change sensors[0].constant.sensor_type to test its propagation
    **/
   input_init();
   int id=0;
   while (id< 8)
   {
    sensors[id].constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
    id++;
   }

   /** \action
    * Call module functions up to the TrackClassification with the changed change sensors[id].sensor_type value that will affect underdrivability.zones[6].state_height_can_pass[0]
    **/

   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);
   
   /** \result
     *The output has been intentionally set to fail, since it has not been decided how to deal with this propogation test
    **/
   CHECK(false); 
}
//                                                #sensors.polarity
/** \purpose
 *  This test will check the  propagation of  sensors.polarity  to PreProcessor module
 * \req
 * CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_polarity_to_PreProcessor)
{
   /** \step{1}
     *- Test input  propagation without a change in the input
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
    sensors[0].constant.fov_max_az_rad[3]= 0.3F;
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.7F, 1e-3F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/   

    /** \precond
    *- change sensors[0].constant.polarity to test its propagation
    **/
   input_init();
   sensors[0].constant.polarity = -1;
   sensors[0].constant.fov_max_az_rad[3]= 0.3F;

   /** \action
    * Call module functions up to the PreProcessor with the changed  sensors[0].constant.polarity  value that will affect detections[0].probability_of_detection
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].probability_of_detection, sensed the propagated sensors[0].constant.polarity  change and has the correct value= 0.1.
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.1F, 1e-3F); 
}

//                                                  #sensors.constant.fov_min_az_rad  
/** \purpose
 *  This test will check the  propagation of  sensors.constant.fov_min_az_rad  to PreProcessor module
 * \req
 * CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_fov_min_az_rad_to_PreProcessor)
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
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.7F, 1e-3F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change sensors[0].constant.polarity to test its propagation
    **/
   input_init();
   sensors[0].constant.fov_min_az_rad[sensors[0].variable.look_id] = -0.3F;

   /** \action
    * Call module functions up to the PreProcessor with the changed  sensors[0].constant.fov_max_az_rad  value that will affect detections[0].probability_of_detection
    by making the det_azimuth lower than sensors[sensor_idx].fov_min_az_rad[look_id] then the init probability_of_detection will take effect.
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].probability_of_detection, sensed the propagated sensors[0].constant.fov_min_az_rad  change and has the correct value= 0.1.
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.1F, 1e-3F); 
}

//                                                  #sensors.constant.fov_max_az_rad  

/** \purpose
 *  This test will check the  propagation of  sensors.constant.fov_min_az_rad  to PreProcessor module
 * \req
 * CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_fov_max_az_rad_to_PreProcessor)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   sensors[0].constant.polarity = -1;
      
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);
   
   /** \result
     *- check that the output  has its original correct value 
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.7F, 1e-3F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change sensors[0].constant.polarity to test its propagation
    **/
   sensors[0].constant.fov_max_az_rad[sensors[0].variable.look_id] = 0.3F;
   sensors[0].constant.polarity = -1;

   /** \action
    * Call module functions up to the PreProcessor with the changed  sensors[0].constant.fov_min_az_rad  value that will affect detections[0].probability_of_detection
    by making the det_azimuth higher than sensors[sensor_idx].fov_max_az_rad[look_id] then the init probability_of_detection will take effect.
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].probability_of_detection, sensed the propagated sensors[0].constant.fov_max_az_rad  change and has the correct value= 0.1.
    **/
   DOUBLES_EQUAL(detections[0].probability_of_detection, 0.1F, 1e-3F); 
}

//                                                  #sensors.v_wrapping

/** \purpose
 *  This test will check the  propagation of  sensors.v_wrapping  to PreProcessor module
 * \req
 * CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_rng_rate_interval_widths_to_PreProcessor)
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
   DOUBLES_EQUAL(tracker_info.unique_rdot_interval_widths[0], 35.5742188F, 1e-6F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change sensors[0].constant.v_wrapping to test its propagation
    **/
   input_init(); 
   sensors[0].constant.v_wrapping[0] = 15.0F;

   /** \action
    * Call module functions up to the PreProcessor with the changed  sensors[0].constant.v_wrapping  value that will affect tracker_info.unique_rdot_interval_widths[0]
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that tracker_info.unique_rdot_interval_widths[0], sensed the propagated sensors[0].constant.fov_max_az_rad  change and has the correct value= 15.0
    **/
   DOUBLES_EQUAL(tracker_info.unique_rdot_interval_widths[0], 15.0F, 1e-6F);  
}

//                                               #sensors[0].constant.mounting_position.longitudinal, sensors[0].constant.mounting_position.vcs_position.lateral

/** \purpose
 *  This test will check the  propagation of sensors[0].constant.mounting_position.lateral  and  sensors[0].constant.mounting_position.vcs_position.longitudinal to PreProcessor.
 * \req
 *  CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_mounting_position_to_PreProcessor)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   raw_detection_list.detections[0].raw.azimuth = 3.0F;
   raw_detection_list.detections[0].raw.sensor_id = 2;
      
   /** \action
    * Call modules functions up to the PreProcessor with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);
   
   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(detections[0].f_FOV_edge == false);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change  sensors[0].constant.mounting_position.vcs_position.longitudinal and sensors[0].constant.mounting_position.vcs_position.longitudinal to test its propagation
    *- set raw_detection_list parameters
    **/
   input_init();
   raw_detection_list.detections[0].raw.azimuth = 3.0F;
   raw_detection_list.detections[0].raw.sensor_id = 2;
   sensors[0].constant.mounting_position.vcs_position.lateral = -10;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -18.0F;

   /** \action
    * Call modules functions up to the PreProcessor  that will affect detections[0].f_FOV_edge.
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that detections[0].f_FOV_edge sensed the change and  propagated with the correct value= true
    **/
   CHECK(detections[0].f_FOV_edge == true);    
}

/** \purpose
 *  This test will check the propagation of sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle to PostUpdateTrack.
 * \req
 *  CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_position_to_PostUpdateTrack)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value
     *- store the initial length of the object 
    **/
   /* the input default value from input vector is used */
   float32_t initial_length = object_tracks[0].bbox.Get_Length();
   
   /** \action
    * Call modules functions up to the PostUpdateTrack with the original input unchanged value
    * Calculate the difference in length before and after function call 
    **/
   F360_Tracker_Modules_Call(Up_To_PostUpdateTrack_Module_call);
   
   float32_t length_diff = std::abs(initial_length - object_tracks[0].bbox.Get_Length());

   /** \result
     *- check that the output length is changed from initial length 
    **/
   CHECK(length_diff > 0.0F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change  sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle to test its propagation
    *- store the initial length of the object 
    **/
   input_init();
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -0.83795F;
   initial_length = object_tracks[0].bbox.Get_Length();

   /** \action
    * Call modules functions up to the PostUpdateTrack  that will affect object_tracks[0].linear_moving
    * Calculate the difference in length before and after function call 
    **/
   F360_Tracker_Modules_Call(Up_To_PostUpdateTrack_Module_call);
   length_diff = std::abs(initial_length - object_tracks[0].bbox.Get_Length());

   /** \result
     *check that the object length sensed the change and  propagated with an unchanged length
    **/
   CHECK(length_diff == 0.0F);  
}

/** \purpose
 *  This test will check the  propagation of sensors[0].constant.mounting_position.vcs_position.height to TrackClassification.
 * \req
 *  CPR-4304 
 */
IGNORE_TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_position_to_TrackClassification)
{
   // This test has been intentionally left to fail,  since it has not been decided how to deal with this propogation test
   // Will be dealt in DFD-1929
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
     *- set sensors.constant.sensor_type parameter
    **/
   /* the input default value from input vector is used */ 
    int id=0;
   while (id< 8)
   {
    sensors[id].constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
    id++;
   }
   /** \action
    * Call modules functions up to the TrackClassification with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);
   /** \result
     *- The output has been intentionally set to fail, since it has not been decided how to deal with this propogation test
    **/
    CHECK(false);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
    *- change  sensors[0].constant.mounting_position.vcs_position.longitudinal and sensors[0].constant.mounting_position.vcs_position.longitudinal to test its propagation
     *- set sensors.constant.sensor_type parameter
    **/
   input_init();
   id=0;
   while (id< 8)
   {
    sensors[id].constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
    id++;
   }
   sensors[0].constant.mounting_position.vcs_position.height = 1.0F;

   /** \action
    * Call modules functions up to the PostUpdateTrack  that will affect detections[0].f_FOV_edge.
    **/
   F360_Tracker_Modules_Call(Up_To_TrackClassification_Module_call);
   
   /** \result
     *The output has been intentionally set to fail, since it has not been decided how to deal with this propogation test
    **/
   CHECK(false);  
}
/** \purpose
 *  This test will check the propagation of sensors[0].constant.mounting_location to PreAssociation module
 * \req
 * CPR-4304 
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_mounting_location_to_PreAssociation)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD; // Will disqualify sensor for angle jump detector
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 85.0F;
   calibrations.rp_max_object_lateral_distance = 25.0F;
   calibrations.obj_aj_suspected_rr_handicap = -12.0F;
   
   /** \action
    * Call modules functions up to the PreAssociation with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_moving = true;
   raw_detection_list.detections[0].raw.range = 0.1F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.1F });
   object_tracks[0].confidenceLevel = calibrations.rp_min_confidence_level + tolerance;
   raw_detection_list.detections[0].processed.vcs_az = 0.5F;
    
   F360_Tracker_Modules_Call(PreAssociation_Module_single_call);   
   /** \result
     *- check that the output has its original correct value 
    **/
   CHECK_FALSE(detections[0].f_object_based_angle_jump);   
   
   /** \step{2}
     *-Test input propagation with a changed value
    **/
    
   /** \precond
    *- sensors[0].constant.mounting_location ,mounting_position to test their propagation
    *- set calibrations,raw_detection_list and object_tracks parameters,Note: some parameters needed to be changed before module call to avoid overwriting
    **/
   input_init();
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;  // Will qualify sensor for angle jump detector
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 85.0F;
   calibrations.rp_max_object_lateral_distance = 25.0F;
   calibrations.obj_aj_suspected_rr_handicap = -12.0F;

   /** \action
    *- Call module functions up to the PreAssociation with the changed sensors[0].constant.mounting_location  value that will affect detections[0].f_object_based_angle_jump.
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_moving = true;
   raw_detection_list.detections[0].raw.range = 0.1F;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.1F });
   object_tracks[0].confidenceLevel = calibrations.rp_min_confidence_level + tolerance;
   raw_detection_list.detections[0].processed.vcs_az = 0.5F;
    
   F360_Tracker_Modules_Call(PreAssociation_Module_single_call);
    
   /** \result
     *check that detections[0].f_object_based_angle_jump, sensed the propagated sensors[0].constant.mounting_location change and has the correct value = true.
    **/
   CHECK_TRUE(detections[0].f_object_based_angle_jump);
}

/** \purpose
 *  This test will check the  propagation of  sensors.mounting_location  to Trailer_Detector_Core module
 * \req
 * CPR-4266  , CPR-4271 , CPR-4281
 */
TEST(f360_tracker_propagation_important_signals, check_Propagation_of_sensors_mounting_location_to_Trailer_Detector)
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
    * Test where sensor mounting location is changed from its nominal value
    **/

   /** \precond
    * - Re-initialize the TrackerCoreDetector class
    * - Setup default data to use in the test. Use the same as for the nominal case except for:
    *     Change sensor mounting location FROM REAR to FORWARD
    **/
   trailer_detector_core.Initialize(); // Same as in nominal test

   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD; // Different from in nominal test
   sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -F360_PI; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F; // Same as in nominal test
   sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F; // Same as in nominal test

   raw_detection_list.number_of_valid_detections = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.sensor_id = 1U; // Same as in nominal test
   raw_detection_list.detections[0].raw.range_rate = 0.0F; // Same as in nominal test
   raw_detection_list.detections[0].raw.range = 2.0F; // Same as in nominal test
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
     * Check that trailer detector doesn't detect the precense of a trailer
    **/
   CHECK_EQUAL_TEXT(TRAILER_PRESENCE_STATE_NOT_DETECTED, non_nominal_output.trailer_presence, "Non-nominal test failed");  
}
/** @}*/
