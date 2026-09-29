/** \file
*   This file aims to test the propagation of object_tracks interface signals.
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
#include "f360_sanity_check.h"
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

/** \defgroup  f360_tracker_propagation_object_track
 *  @{
 */

/** \brief
*  This test suit aims to test the propagation of object_track interface
**/
TEST_GROUP(f360_tracker_propagation_object_track)
{   
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"
    
   const float tolerance = 0.0000001F;   

   /** \setup
   * Nothing to setup in this test group
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
*                                            Time_Update                                                 *
**********************************************************************************************************
**/

                                               
/** \purpose  
 *  This test will check the  propagation of object_tracks[0].status to TimeUpdate module
 * \req
 * CPR-4536, CPR-4463 , CPR-4371 , CPR-4423 , CPR-4425
 */
TEST(f360_tracker_propagation_object_track, check_Propagation_object_tracks_to_time_update)
{
   /** \step{1}
    *- Test input  propagation without a change in the input
    **/
    
   /** \precond
     *- set input to their original unchanged value 
    **/
   /* input original default value is used */
    int obj_idx = tracker_info.active_obj_ids[0] - 1;
   /** \action
    * Call modules functions up to Time_Update with the original input unchanged value 
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);
   float default_object_tracks_priority = object_tracks[obj_idx].priority ;

   /** \result
     *- check that the output  has its original correct value 
    **/ 
   DOUBLES_EQUAL(object_tracks[obj_idx].priority,default_object_tracks_priority,1e-6F);    
   
   /** \step{2}
     *-Test input  propagation with a changed value
    **/ 

   /** \precond
    *- Change object_tracks[0].status and test its propagation 
    **/
   input_init();
   object_tracks[0].status = F360_OBJECT_STATUS_INVALID;

   /** \action
    * Call modules functions up to the TimeUpdate  that will affect object_tracks[obj_idx].priority.
    **/
   F360_Tracker_Modules_Call(Up_To_TimeUpdate_Module_call);

   /** \result
     *check that object_tracks[obj_idx].priority propagated with a correct value S    
    **/    
   CHECK(object_tracks[obj_idx].priority !=default_object_tracks_priority)
}

/**
**********************************************************************************************************
*                                           PreAssociation                             *
**********************************************************************************************************
**/
/** \purpose  
 *  This test will check the  propagation of object_track.vcs_heading to PreAssociation
 * \req
 * CPR-4536, CPR-4463 , CPR-4371 , CPR-4423 , CPR-4425
 */
TEST(f360_tracker_propagation_object_track, check_Propagation_object_tracks_to_PreAssociation)
{
   /** \step{1}
    **/ 
    
   /** \precond
     *- change detections[0].f_ok_to_use based on object_track[0].vcs_heading
     *- set sensors,calibrations and raw_detection_list parameters
    **/
   object_tracks[0].vcs_heading = Angle{ -0.17F }; //for next case
   object_tracks[0].reference_point = F360_REFERENCE_POINT_RIGHT;
   object_tracks[0].bbox.Set_Width(2.0F);
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 84;
   sensors[0].constant.mounting_position.vcs_position.lateral = 7;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   calibrations.obj_aj_suspected_rr_handicap = -8.5F;
   raw_detection_list.detections[0].raw.range = 5;
    
   /** \action
    * Call modules functions up to PreAssociation  that will affect detections[0].f_ok_to_use.
    **/
   F360_Tracker_Modules_Call(Up_To_PreAssociation_Module_call);

   /** \result
     *check that detections[0].f_ok_to_use, sensed the change and  propogated with the expected value= false       
    **/ 
   CHECK_FALSE(detections[0].f_ok_to_use); /*Pass here means test values have propagated successfully*/

   /** \step{2}
    *- Negation test
   **/ 
    
   /** \precond
     *- change detections[0].f_ok_to_use based on object_track[0].vcs_heading
     *- set sensors,calibrations and raw_detection_list parameters
    **/
   input_init();
   object_tracks[0].vcs_heading = Angle{ 0.5F };
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 84;
   sensors[0].constant.mounting_position.vcs_position.lateral = 7;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
   calibrations.obj_aj_suspected_rr_handicap = -8.5F;
   raw_detection_list.detections[0].raw.range = 5;
    
   /** \action
    * Call modules functions up to PreAssociation  that will affect detections[0].f_ok_to_use.
    **/
   F360_Tracker_Modules_Call(Up_To_PreAssociation_Module_call);

   /** \result
     *check that detections[0].f_ok_to_use, sensed the change and  propogated with the expected value= false       
    **/ 
   CHECK(detections[0].f_ok_to_use != false); /*Pass here means test values have propagated successfully*/

}

/**
**********************************************************************************************************
*                                           Association                             *
**********************************************************************************************************
**/
/** \purpose  
 *  This test will check the  propagation of object_track.movable_prob to Association
 * \req
 * CPR-4536, CPR-4463 , CPR-4371 , CPR-4423 , CPR-4425
 */
TEST(f360_tracker_propagation_object_track, check_Propagation_object_tracks_to_Association)
{
   /** \step{1}
   **/ 
   
   /** \precond
     *change object_tracks[0].lat_buffer_zone_max based on object_tracks[0].movable_prob
    **/
   object_tracks[0].movable_prob = 1.0F;
    
   /** \action
    * Call modules functions up to PreAssociation that will affect detections[0].f_ok_to_use.
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

/** \result
     *check that object_tracks[1].lat_buffer_zone_wid1, sensed the change and  propogated with the correct value  = 4.96981     
    **/ 
   DOUBLES_EQUAL(4.96981F, object_tracks[0].lat_buffer_zone_wid1, 1e-3F); /*Pass here means test values have propagated successfully*/
   
   /** \step{2}
    *- Negation test
   **/   
   
   /** \precond
     *-change object_tracks[0].lat_buffer_zone_wid1 based on object_track[0].movable_prob
    **/
   input_init(); 
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
    
   /** \action
    * Call modules functions up to PreAssociation  that will affect object_tracks[0].lat_buffer_zone_wid1.
    **/
   F360_Tracker_Modules_Call(Up_To_Association_Module_call);

    /** \result
     *check that object_tracks[1].lat_buffer_zone_wid1, sensed the change and  propogated with the correct value  = 2    
    **/ 
   DOUBLES_EQUAL(2.0F, object_tracks[0].lat_buffer_zone_wid1, 1e-3F); /*Pass here means test values have propagated successfully*/
}

/**
**********************************************************************************************************
*                                           Track Grouping                             *
**********************************************************************************************************
**/
/** \purpose  
 *  This test will check the  propagation of object_track.vcs_position,length,f_crossing, to Track Grouping       
 * \req
 * CPR-4536, CPR-4463 , CPR-4371 , CPR-4423 , CPR-4425
 */
TEST(f360_tracker_propagation_object_track, check_Propagation_object_tracks_to_TrackGrouping)
{
   /** \step{1}
    **/ 
   
   /** \precond
     Change object_tracks[2].status based on multiple object_track parameters
     Object id 2 & 3 are supposed to merge and since id 2 is older than id 3, id 2 is supposed to be killed
    **/
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -2.4F;
   object_tracks[1].time_since_cluster_created = 0.15F; // older than obj id 3
   object_tracks[1].Update_Bbox_Center();
   object_tracks[2].Update_Bbox_Center();
   object_tracks[1].f_moving = true;
   object_tracks[2].f_moving = true;
   
   /** \action
    * Call modules functions up to PreAssociation  that will affect object_tracks[2].status.
    **/
   F360_Tracker_Modules_Call(Up_To_TrackGrouping_Module_call);
  
    /** \result
     *check that object_tracks[2].status, sensed the change and  propogated with the correct value = F360_OBJECT_STATUS_INVALID
    **/ 
   CHECK(object_tracks[2].status == F360_OBJECT_STATUS_INVALID); /*Pass here means test values have propagated successfully*/   
   
   /** \step{2}
    *- Negation test
    **/ 
   
   /** \precond
     *change object_tracks[2].status based on multiple object_tracks parameters such that the merge does not occur
    **/
   input_init();
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -2.4F;
   object_tracks[1].time_since_cluster_created = 0.15F; // older than obj id 3
   object_tracks[1].Update_Bbox_Center();
   object_tracks[2].Update_Bbox_Center();
   object_tracks[1].f_moving = true;
   object_tracks[2].f_moving = false; // will prevent merge
    
   /** \action
    * Call modules functions up to PreAssociation that will affect object_tracks[2].status
    **/
   F360_Tracker_Modules_Call(Up_To_TrackGrouping_Module_call);
 
    /** \result
     *check that object_tracks[2].status, sensed the change and propogated with the correct value != F360_OBJECT_STATUS_INVALID
    **/
   CHECK(object_tracks[2].status != F360_OBJECT_STATUS_INVALID); /*Pass here means test values have propagated successfully*/   
}

/** @}*/
