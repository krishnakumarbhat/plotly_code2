/** \file
*   This file aims to test the propagation of static_env_class interface signals.
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

/** \defgroup  f360_tracker_propagation_static_env_class
 *  @{
 */

 /** \brief
 *  This test suits aims to test the propagation of static_env_class interface signals
 **/
TEST_GROUP(f360_tracker_propagation_static_env_class)
{
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"
    
   /** \setup
   * Init input test vector
   **/

   int32_t test_obj_idx;

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
*                                            TrackDownselection                                          *
**********************************************************************************************************
**/

/** \purpose
 *  This test will check the  propagation of the static_env_class interface signals to TrackDownselection.
 * \req
 * CPR-4538, CPR-4476
 */
TEST(f360_tracker_propagation_static_env_class, check_Propagation_of_static_env_class_TrackDownselection)
{
   /** \step{1}
     *- Test input  propagation without a change in the inputs
    **/
   /** \precond
     *- set inputs to their original unchanged value 
    **/
   /* the input default value from input vector is used */ 

   /** \action
    * Call modules functions up to the Track Validity module with the original input unchanged value
    **/

   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);

   // Create object that will get downselected *only* because it is inside of the exclusion zone
   test_obj_idx = tracker_info.num_active_objs;
   tracker_info.num_active_objs++;
   tracker_info.active_obj_ids[test_obj_idx] = test_obj_idx + 1;

   object_tracks[test_obj_idx].id = test_obj_idx + 1;
   object_tracks[test_obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[test_obj_idx].f_valid_for_liberal_tracking = false;
   object_tracks[test_obj_idx].f_low_confidence_level = false;
   object_tracks[test_obj_idx].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[test_obj_idx].confidenceLevel = 1.1 * calibrations.k_track_downselect_confidence_thresh;
   object_tracks[test_obj_idx].conf_overall = CONF3_MED; // conf_overall < calib.k_low_conf_unreliability_min_overall_conf = true
   object_tracks[test_obj_idx].speed = 0.99 * host.speed;
   object_tracks[test_obj_idx].vcs_heading = Angle{ - 0.9F * calibrations.k_low_conf_unreliability_min_heading }; // abs(vcs_heading) > calib.k_low_conf_unreliability_min_heading = false
   object_tracks[test_obj_idx].vcs_velocity.longitudinal = object_tracks[test_obj_idx].speed;
   object_tracks[test_obj_idx].vcs_velocity.lateral = 0.0F;
   object_tracks[test_obj_idx].vcs_position.x = 0.0F;
   object_tracks[test_obj_idx].vcs_position.y = 0.99 * calibrations.k_conf_downselection_exclusion_box_lat;
   
   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);

   /** \result
     *- Verify that the created object is downselected
    **/
   
   CHECK_TRUE(object_tracks[test_obj_idx].reduced_id != 0);


   /** \step{2}
     *-Test input propagation with a changed value
    **/   
    
   /** \precond
     *- Initialize CWD and pre-load its circular buffer such that it will create a SEP between host and the created object
    **/
   input_init();

   F360_Concrete_Wall_Detector_T& cwd = const_cast<F360_Concrete_Wall_Detector_T&>( static_env_class.Get_CWD() );
   cwd.Set_CWD_Sensors_Count(2U);
   cwd.Set_CWD_Long_Zones_Count(2U);
   cwd.Set_f_initialized(true);

   Concrete_Wall_Side_T& cw_left = const_cast<Concrete_Wall_Side_T&>( cwd.Get_CWD_Left_Side() );
   cw_left.Set_Sensors_Count(0U);

   Concrete_Wall_Side_T& cw_right = const_cast<Concrete_Wall_Side_T&>( cwd.Get_CWD_Right_Side() );
   cw_right.Set_Sensors_Count(2U);
   cw_right.Set_Sensor_Idx_To_Array(0U, 0U);
   cw_right.Set_Sensor_Idx_To_Array(1U, 1U);

   for (uint32_t cwd_sensor_idx = 0; cwd_sensor_idx < 2U; cwd_sensor_idx++)
   {
      Concrete_Wall_Sensor_T& cwd_sensor = const_cast<Concrete_Wall_Sensor_T&>( cwd.Get_CWD_Sensor(cwd_sensor_idx) );

      const f360_variant_A::cmn::Circular_Buffer<f360_variant_A::Concrete_Wall_Sample_T, 5U>& samples_const = cwd_sensor.Get_CW_Measurements().Get_Samples();
            f360_variant_A::cmn::Circular_Buffer<f360_variant_A::Concrete_Wall_Sample_T, 5U>& samples = const_cast<f360_variant_A::cmn::Circular_Buffer<f360_variant_A::Concrete_Wall_Sample_T, 5U>&>(samples_const);

      Concrete_Wall_Sample_T sample;
      sample.lateral_position = 5.0F; // Between host and object
      sample.is_valid = true;

      for (uint32_t smpl_idx = 0; smpl_idx < cwd_sensor.Get_CW_Measurements().Get_Buffer_Capacity(); smpl_idx++)
      {
         samples.push(sample);
      }

   }

   const Static_Env_T::Static_Env_Polys_Array & const_static_env_polys = static_env_class.Get_Polynomials();
   Static_Env_T::Static_Env_Polys_Array & static_env_polys = const_cast<Static_Env_T::Static_Env_Polys_Array&>(const_static_env_polys);

   cwd.Run_Measurements(detections, raw_detection_list, sensor_props, calibrations, object_tracks, std::abs(host.speed), timing_info);
   cwd.Map_Data_To_Static_Env_Poly(static_env_polys, host.curvature_rear);

   /** \action
    * Call modules functions up to the TrackDownselection that will affect static_env_class interface signals
    **/
   F360_Tracker_Modules_Call(Up_To_TrackValidity_Module_call);

   // Create object that would get downselected *only* because it is inside of the exclusion zone
   test_obj_idx = tracker_info.num_active_objs;
   tracker_info.num_active_objs++;
   tracker_info.active_obj_ids[test_obj_idx] = test_obj_idx + 1;

   object_tracks[test_obj_idx].id = test_obj_idx + 1;
   object_tracks[test_obj_idx].movable_prob = 1.0F;
   object_tracks[test_obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[test_obj_idx].f_valid_for_liberal_tracking = false;
   object_tracks[test_obj_idx].f_low_confidence_level = false;
   object_tracks[test_obj_idx].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[test_obj_idx].confidenceLevel = 1.1 * calibrations.k_track_downselect_confidence_thresh;
   object_tracks[test_obj_idx].conf_overall = CONF3_MED; // < calib.k_low_conf_unreliability_min_overall_conf
   object_tracks[test_obj_idx].speed = 0.99 * host.speed;
   object_tracks[test_obj_idx].vcs_heading = Angle{- 0.9F * calibrations.k_low_conf_unreliability_min_heading}; // abs(vcs_heading) > calib.k_low_conf_unreliability_min_heading = false
   object_tracks[test_obj_idx].vcs_velocity.longitudinal = object_tracks[test_obj_idx].speed;
   object_tracks[test_obj_idx].vcs_velocity.lateral = 0.0F;
   object_tracks[test_obj_idx].vcs_position.x = 0.0F;

   (calibrations.k_conf_downselection_exclusion_box_lat < calibrations.k_track_downselect_max_vcs_y_range_to_preserve) ?
         object_tracks[test_obj_idx].vcs_position.y = 0.99 * calibrations.k_conf_downselection_exclusion_box_lat :
         object_tracks[test_obj_idx].vcs_position.y = 0.99 * calibrations.k_track_downselect_max_vcs_y_range_to_preserve;

   F360_Tracker_Modules_Call(TrackDownSelection_Module_single_call);


   /** \result
     * Verify that the test object did NOT get downselected as a result of the shrunk exclusion zone by the SEP created by CWD
     **/
   CHECK_TRUE(object_tracks[test_obj_idx].reduced_id == 0);

}

/** @}*/
