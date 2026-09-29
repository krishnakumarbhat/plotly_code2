/** \file
*   This file aims to test the propagation of calibrations interface signals.
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

namespace f360_variant_A
{
   float get_wall_time()
   {
      return (float)0.0;
   }
}

using namespace f360_variant_A;

/** \defgroup  f360_tracker_propagation_calibrations
 *  @{
 */

/** \brief
 *  This test suit aims to test the propagation of calibrations interface signals
 **/
TEST_GROUP(f360_tracker_propagation_calibrations)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"

   const float tolerance = 0.00001F;
   
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
 *  This test will check the  propagation of  calibrations.max_age_of_older_dets, calibrations.max_age_of_older_no_amb_dets
 *  and calibrations.max_age_of_older_dets_lr  to PreProcessor.
 * \req
 * CPR-4519
 */
TEST(f360_tracker_propagation_calibrations, check_Propagation_of_calibrations_max_age_of_older_dets_to_PreProcessor)
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
   LONGS_EQUAL(clusters[11].old_det_idx[0], 11);
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/
    
   /** \precond
     *change  calibrations.max_age_of_older_dets, calibrations.max_age_of_older_no_amb_dets
     *and calibrations.max_age_of_older_dets_lr value and test their propagation
    **/
   input_init(); 
   calibrations.max_age_of_older_amb_dets = 0.001F;
   calibrations.max_age_of_older_no_amb_dets = 0.001F;
   calibrations.max_age_of_older_dets_lr = 0.001F;

   /** \action
    * Call modules functions up to the PreProcessor  that will affect clusters[0].old_det_idx[0].
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
     *check that clusters[11].old_det_idx[0] sensed the change and  propagated with the correct value= 0
    **/
   LONGS_EQUAL(clusters[11].old_det_idx[0], 0);
}

/**
**********************************************************************************************************
*                                            TimeUpdate                                                  *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check the  propagation of calibrations.k_max_conf_objtrk_coast_time to TimeUpdate.
 * \req
 * CPR-4519
 */
TEST(f360_tracker_propagation_calibrations, check_Propagation_of_calibrations_k_max_conf_objtrk_coast_time_to_TimeUpdate)
{

   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
     *- set object_track parameters
    **/
   /* the input default value from input vector is used */ 
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = 0.002F;
   
   /** \action
    * Call modules functions up to the TimeUpdate with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK(object_tracks[0].status == F360_OBJECT_STATUS_COASTED);
   
   /** \step{2}
     *- Test input  propagation with a changed value
    **/
    
   /** \precond
     *- change   calibrations.k_max_conf_objtrk_coast_time parameter value that will affect object_tracks[0].status
     *- set object_track parameters.
    **/
   input_init();
   calibrations.k_max_conf_objtrk_coast_time = 0.001F;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = 0.002F;

   /** \action
    * Call modules functions up to the TimeUpdate  that will affect object_tracks[0].status.
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
     *check that object_tracks[0].status sensed the change and  propagated with the correct value = F360_OBJECT_STATUS_INVALID
    **/
   CHECK(object_tracks[0].status == F360_OBJECT_STATUS_INVALID);   
}

/**
**********************************************************************************************************
*                                            PreAssociation                                              *
**********************************************************************************************************
**/
/** \purpose
 *  This test will check the propagation of calibrations.rp_max_object_lateral_distance to PreAssociation module
 * \req
 * CPR-4519
 */
TEST(f360_tracker_propagation_calibrations, check_Propagation_of_calibrations_rp_max_object_lateral_distance_to_PreAssociation)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
     *- set sensors,raw_detection_list,object_tracks parameters, Note: some parameters needed to be changed before module call to avoid overwriting     
    **/
   /* the input default value from input vector is used */ 
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 85.0F;
   calibrations.obj_aj_suspected_rr_handicap = -12.0F;
   raw_detection_list.detections[0].raw.range = 0.1F;
   
   /** \action
    * Call modules functions up to the PreAssociation with the original input unchanged value 
    **/
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_moving = true;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.1F });
   object_tracks[0].confidenceLevel = calibrations.rp_min_confidence_level + tolerance;
   raw_detection_list.detections[0].processed.vcs_az = 0.5F;
   F360_Tracker_Modules_Call(PreAssociation_Module_single_call);
   /** \result
     *- check that the output  has its original correct value 
    **/
   CHECK_TRUE(detections[0].f_object_based_angle_jump);
   
   /** \step{2}
     *- Test input propagation with a changed value
    **/
    
   /** \precond
    *- set calibrations.rp_max_object_lateral_distance= 0.01F
    *- set sensors,raw_detection_list,object_tracks parameters, Note:some parameters needed to be changed before module call to avoid overwriting
    **/
   input_init();
   calibrations.rp_max_object_lateral_distance = 0.01F;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 85.0F;
   calibrations.obj_aj_suspected_rr_handicap = -12.0F;
   raw_detection_list.detections[0].raw.range = 0.1F;

   /** \action
    *- Call module functions up to the PreAssociation with the changed input  value that will affect detections[0].f_object_based_angle_jump .
    **/
   object_tracks[0].vcs_position.y = 1.0F;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_moving = true;
   object_tracks[0].Set_Bbox_Orientation(Angle{ 0.1F });
   raw_detection_list.detections[0].processed.vcs_az = 0.5F;
   F360_Tracker_Modules_Call(PreAssociation_Module_single_call);
    
   /** \result
     *- check that detections[0].f_object_based_angle_jump, sensed the propagated input  change and has the correct value= false.
    **/
   CHECK_FALSE(detections[0].f_object_based_angle_jump);
}
/**
**********************************************************************************************************
*                                            Measurement_Update_Tracks                                   *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check the  propagation of calibrations.k_min_speed_for_updating_heading to MsmtUpdate.
 * \req
 * CPR-4519
 */
TEST(f360_tracker_propagation_calibrations, check_Propagation_of_calibrations_k_min_speed_for_updating_heading_to_measurement_update_tracks)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 

   /** \action
    * Call modules functions up to the MsmtUpdate with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_MsmtUpdate_Module_call);

   /** \result
     *- check that the output has its original correct value 
    **/
   //LONGS_EQUAL(object_tracks[0].visible_edges, F360_COMBINED_VISIBLE_EDGES_REAR_LEFT);

   /** \step{2}
     *- Test input  propagation with a changed value
    **/
    
   /** \precond
     *- change  calibrations.k_min_speed_for_updating_heading and test its propagation
    **/
   input_init();
   calibrations.k_min_speed_for_updating_heading = 10.0F; // TODO DFD-1606, modify test to expectations

   /** \action
    * Call modules functions up to the MsmtUpdate  that will affect object_tracks[0].visible_edges
    **/
   F360_Tracker_Modules_Call(Up_To_MsmtUpdate_Module_call);

   /** \result
     *check that object_tracks[0].visible_edges sensed the change and  propagated with the correct value= F360_COMBINED_VISIBLE_EDGES_LEFT
    **/
   //LONGS_EQUAL(object_tracks[0].visible_edges, F360_COMBINED_VISIBLE_EDGES_LEFT);
}

/**
**********************************************************************************************************
*                                            Initialize_Tracks                                           *
**********************************************************************************************************
**/


/** \purpose
 *  This test will check the propagation of calibrations.k_nonmoveable_target_diameter to Initialize_Tracks.
 * \req
 * CPR-4519
 **/
TEST(f360_tracker_propagation_calibrations, check_Propagation_of_calibrations_k_nonmoveable_target_diameter_Initialize_track)
{
   /** \step{1}
     *- Test input propagation without a change in the inputs
    **/
    
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 
   int obj_idx = tracker_info.inactive_obj_ids[0] - 1U;
   calibrations.k_nonmoveable_target_diameter = 1.0F;

   /** \action
    * Call modules functions up to the Initialize_Tracks with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_NewObjInit_Module_call);

   /** \result
     *- check that the output has its original correct value 
    **/
   CHECK(object_tracks[obj_idx].bbox.Get_Length() == 1.0F);

   /** \step{2}
     *- Test input propagation with a changed value
    **/   
    
   /** \precond
     *- Change input signal and test its propagation
    **/
   input_init();
   calibrations.k_nonmoveable_target_diameter = 1234.0F;
   obj_idx = tracker_info.inactive_obj_ids[0] - 1U;
    
   /** \action
    * Call modules functions up to the Initialize_Tracks that will affect object_tracks[0].f_vehicular_trk.
    **/
   F360_Tracker_Modules_Call(Up_To_NewObjInit_Module_call);

   /** \result
     *- check that object_tracks[obj_idx].Get_Length() sensed the change and propagated with the correct value=1234.
    **/
   CHECK(object_tracks[obj_idx].bbox.Get_Length() == 1234.0F);
}

/** @}*/
