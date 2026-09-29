/** \file
 * This file contains unit tests for content of f360_kill_coasted_tracks.cpp file
 */

#include "f360_kill_coasted_tracks.h"
#include "f360_radar_sensor_props.h"
#include "f360_internal_preprocessing.h"
#include <CppUTest/TestHarness.h>
#include <iostream>

using namespace f360_variant_A;

/** \defgroup  f360_kill_coasted_tracks
 *  @{
 */

/** \brief
 *
 * The group populates variables used for testing. Sensor mounting location is set as center forward. Azimuth angle, latitude and longitude
 * are set
 */
TEST_GROUP(f360_kill_coasted_tracks)
{
   F360_Object_Track_T object_track;
   F360_Globals_T globals;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibrations;
   const float32_t azim_th = F360_PI / 4;

   /** \setup
    * Setting up a single front sensor in the first cell of the array containing all sensors.
    * Setting the global variable that would indicate that there is only one sensor present and it is the FLR sensor.
    * Setting the object to fulfill all conditions to be marked for removal.
    */
   TEST_SETUP()
   {
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;

      for (int i = 0; i < 4; i++)
      {
         sensors[0].constant.fov_min_az_rad[i] = -azim_th;
         sensors[0].constant.fov_max_az_rad[i] = azim_th;
      }

      const float min_fov_az_angle_lr = std::min(sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_0], sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_1]);
      const float min_fov_az_interior_angle_lr = min_fov_az_angle_lr;
      const float max_fov_az_angle_lr = std::max(sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_0], sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_1]);
      const float max_fov_az_interior_angle_lr = max_fov_az_angle_lr;
      const float min_fov_az_angle_mr = std::min(sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_2], sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_3]);
      const float min_fov_az_interior_angle_mr = min_fov_az_angle_mr;
      const float max_fov_az_angle_mr = std::max(sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_2], sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_3]);
      const float max_fov_az_interior_angle_mr = max_fov_az_angle_mr;

      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;

      const float min_fov_vcs_az_angle_lr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
      const float max_fov_vcs_az_angle_lr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
      const float min_fov_vcs_az_angle_mr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
      const float max_fov_vcs_az_angle_mr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;

      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_0] = -F360_Sinf(min_fov_vcs_az_angle_lr);
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_1] = F360_Cosf(min_fov_vcs_az_angle_lr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_0] = F360_Sinf(max_fov_vcs_az_angle_lr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_1] = -F360_Cosf(max_fov_vcs_az_angle_lr);
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_2] = -F360_Sinf(min_fov_vcs_az_angle_mr);
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_3] = F360_Cosf(min_fov_vcs_az_angle_mr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_2] = F360_Sinf(max_fov_vcs_az_angle_mr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_3] = -F360_Cosf(max_fov_vcs_az_angle_mr);

      globals.f_single_front_center_radar_only = true;

      object_track.status = F360_OBJECT_STATUS_COASTED;
      object_track.movable_prob = 0.0F;
      object_track.time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.005F;
      object_track.vcs_position = Point(0.0F, 10.0F);

   }
};

/** \purpose
 * The test verifies that movable objects are not considered for removal outside FOV.
 * \req
 * NA
 */
TEST(f360_kill_coasted_tracks, Is_Non_Movable_Obj_Outside_Front_Only_FOV_moving)
{
   /** \precond
    * Declare the test object as movable.
    * Set the result variable to the answer opposite to the correct one.
    */
   object_track.movable_prob = 1.0F;
   bool result = true;

   /** \action
    * Call the tested function Is_Non_Movable_Obj_Outside_Front_Only_FOV.
    */
   result = Is_Non_Movable_Obj_Outside_Front_Only_FOV(object_track, globals, sensors);

   /** \result
    * The tested function is expected to return "false" since the tested object is movable.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * This test verifies that the tested function returns false since the object is inside FOV so
 * should not be evaluated.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Is_Non_Movable_Obj_Outside_Front_Only_FOV_inside_fov)
{
   /** \precond
    * Set the tested object's position so that the object is inside FOV
    * Set the result variable to the answer opposite to the correct one.
    */
   object_track.vcs_position = Point(10.0F, 0.0F);
   bool result = true;

   /** \action
    * Call the tested function Is_Non_Movable_Obj_Outside_Front_Only_FOV.
    */
   result = Is_Non_Movable_Obj_Outside_Front_Only_FOV(object_track, globals, sensors);

   /** \result
    * The tested function is expected to return "false" since the tested object is inside FOV.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * This test verifies that the tested function returns true since the object is outside FOV so
 * should be evaluated.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Is_Non_Movable_Obj_Outside_Front_Only_FOV_outside_fov)
{
   /** \precond
    * Set the result variable to the answer opposite to the correct one.
    */
   bool result = false;

   /** \action
    * Call the tested function Is_Non_Movable_Obj_Outside_Front_Only_FOV.
    */
   result = Is_Non_Movable_Obj_Outside_Front_Only_FOV(object_track, globals, sensors);

   /** \result
    * The tested function is expected to return "true" since the tested object fulfills all conditions to be dropped.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * This test verifies that the tested function returns false since there is no front radar.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Is_Non_Movable_Obj_Outside_Front_Only_FOV_not_front_sensor)
{
   /** \precond
    * Set the only sensor's position as an incorrect one. 
    * Set the result variable to the answer opposite to the correct one.
    */
   bool result = true;
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;
   /** \action
    * Call the tested function Is_Non_Movable_Obj_Outside_Front_Only_FOV.
    */
   result = Is_Non_Movable_Obj_Outside_Front_Only_FOV(object_track, globals, sensors);

   /** \result
    * The tested function is expected to return "false" since the tested object has not yet been outside FOV long enough.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * This test verifies that the tested function returns false since the global variable is false.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Is_Non_Movable_Obj_Outside_Front_Only_FOV_false_global_front_only)
{
   /** \precond
    * Set the global variable indicating that there is only one single front center radar and false. 
    * Set the result variable to the answer opposite to the correct one.
    */
   globals.f_single_front_center_radar_only = false;
   bool result = true;

   /** \action
    * Call the tested function Is_Non_Movable_Obj_Outside_Front_Only_FOV.
    */
   result = Is_Non_Movable_Obj_Outside_Front_Only_FOV(object_track, globals, sensors);

   /** \result
    * The tested function is expected to return "false" since the this is not front radar only set up.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * This test verified that the non movable object that has been outside FLR FOV not long enough 
 * is not designated for removal.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Kill_Coasted_Track_Non_Movable_Coasting_Obj_Outside_Front_Only_FOV_Insufficient_Time)
{
   /** \precond
    * Set the time since the state change to a value that is less than the one that
    * would indicate that the object has not yet been outside FOV long enough.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   // Initialize tracker info variant
   tracker_info.variant.num_tracks = NUMBER_OF_OBJECT_TRACKS;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_outside_fov - 0.01F;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -10.0F; // Outside of FOV

   /** \action
    * Call the tested function Kill_Coasted_Tracks.
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * The tested function should not kill the object because it has not yet been outside FOV long enough, num_active_objs should remain 1.
    */
   CHECK_EQUAL(1, tracker_info.num_active_objs);
}

/** \purpose
 * This test verified that the non movable object that has been outside FLR FOV long enough 
 * is designated for removal.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Kill_Coasted_Track_Non_Movable_Coasting_Obj_Outside_Front_Only_FOV_Sufficient_Time)
{
   /** \precond
    * Set the time since the state change to a value that is greater than the one that
    * would indicate that the object has not yet been outside FOV long enough.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   // Initialize tracker info variant
   tracker_info.variant.num_tracks = NUMBER_OF_OBJECT_TRACKS;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.01F;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].id = 1;
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = -10.0F; // Outside of FOV

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = nullptr;
   tracker_info.vcslong_sorted_prev_track[0] = nullptr;

   /** \action
    * Call the tested function Kill_Coasted_Tracks.
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * The tested function should kill the object because it has been outside FOV long enough, num_active_objs should be 0.
    */
   CHECK_EQUAL(0, tracker_info.num_active_objs);
}

/** \purpose  
 * This test verifies that the tested function does not kill object when status is not "coasted".
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Kill_Coasted_Tracks_Not_Coasted)
{
   /** \precond
    * Set object status to something other than F360_OBJECT_STATUS_COASTED.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   // Initialize tracker info variant
   tracker_info.variant.num_tracks = NUMBER_OF_OBJECT_TRACKS;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   
   // Set up object with non-coasted status
   object_tracks[0].status = F360_OBJECT_STATUS_NEW;
   object_tracks[0].time_since_stage_start = 1.0F;

   /** \action
    * Call the tested function Kill_Coasted_Tracks.
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * The tested function is expected to not kill the tested object since it is not coasting.
    */
   CHECK_EQUAL(1, tracker_info.num_active_objs);
}

/** \purpose  
 * This test verifies that objects with f_suspectable_for_det_drop flag set are allowed
 * to coast longer (0.65s) compared to normal coasting time before being killed.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Kill_Coasted_Tracks_Detection_Drop_Longer_Coast_Time)
{
   /** \precond
    * Set up object tracks array, tracker info, and timing info.
    * Create two coasted objects:
    * - Object 0: f_suspectable_for_det_drop = true, coasting time = 0.6s (should NOT be killed)
    * - Object 1: f_suspectable_for_det_drop = false, coasting time = 0.6s (should be killed if > normal threshold)
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   // Initialize tracker info variant
   tracker_info.variant.num_tracks = NUMBER_OF_OBJECT_TRACKS;
   
   // Assume normal coasting time is 0.5s (typical value)
   calibrations.k_max_conf_objtrk_coast_time = 0.5F;
   calibrations.k_max_coast_time_mirror = 0.3F;
   calibrations.k_mirror_prob_threshold = 0.8F;
   
   // Set up object 0: suspectable for detection drop
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].id = 1;
   object_tracks[0].f_suspectable_for_det_drop = true;
   object_tracks[0].time_since_stage_start = 0.6F; // Between normal (0.5s) and det_drop (0.65s)
   object_tracks[0].mirror_prob = 0.0F;
   object_tracks[0].movable_prob = 1.0F; // movable to avoid FOV check
   
   // Set up object 1: normal coasted object
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].id = 2;
   object_tracks[1].f_suspectable_for_det_drop = false;
   object_tracks[1].time_since_stage_start = 0.6F; // Above normal threshold (0.5s)
   object_tracks[1].mirror_prob = 0.0F;
   object_tracks[1].movable_prob = 1.0F; // movable to avoid FOV check
   
   // Set up tracker info with 2 active objects
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1; // Object index 0
   tracker_info.active_obj_ids[1] = 2; // Object index 1

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = &object_tracks[1];
   tracker_info.vcslong_sorted_prev_track[0] = nullptr;
   tracker_info.vcslong_sorted_next_track[1] = nullptr;
   tracker_info.vcslong_sorted_prev_track[1] = &object_tracks[0];
   
   // Set global to avoid FOV check interfering
   globals.f_single_front_center_radar_only = false;
   
   /** \action
    * Call Kill_Coasted_Tracks function.
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);
   
   /** \result
    * Object 0 (detection drop suspectable) should still be active (NOT killed) because 0.6s < 0.65s
    * Object 1 (normal) should be killed because 0.6s > 0.5s
    * Verify by checking:
    * - num_active_objs should be 1 (only object 0 remains active)
    * - active_obj_ids[0] should still be 1 (object 0)
    * - object 0 status should still be COASTED
    */
   CHECK_EQUAL(1, tracker_info.num_active_objs);
   CHECK_EQUAL(1, tracker_info.active_obj_ids[0]);
   CHECK_EQUAL(F360_OBJECT_STATUS_COASTED, object_tracks[0].status);
}

/** \purpose  
 * This test verifies that objects with f_suspectable_for_det_drop flag set will eventually
 * be killed when coasting time exceeds 0.65s.
 * \req
 * NA.
 */
TEST(f360_kill_coasted_tracks, Kill_Coasted_Tracks_Detection_Drop_Eventually_Killed)
{
   /** \precond
    * Set up object with f_suspectable_for_det_drop = true and coasting time > 0.65s.
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   // Initialize tracker info variant
   tracker_info.variant.num_tracks = NUMBER_OF_OBJECT_TRACKS;
   
   calibrations.k_max_conf_objtrk_coast_time = 0.5F;
   calibrations.k_max_coast_time_mirror = 0.3F;
   calibrations.k_mirror_prob_threshold = 0.8F;
   
   // Set up object with detection drop flag but coasting too long
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].id = 1;
   object_tracks[0].f_suspectable_for_det_drop = true;
   object_tracks[0].time_since_stage_start = 0.7F; // Above det_drop threshold (0.65s)
   object_tracks[0].mirror_prob = 0.0F;
   object_tracks[0].movable_prob = 1.0F;
   
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;

   tracker_info.vcslong_sorted_start = &object_tracks[0];
   tracker_info.vcslong_sorted_next_track[0] = nullptr;
   tracker_info.vcslong_sorted_prev_track[0] = nullptr;
   
   globals.f_single_front_center_radar_only = false;
   
   /** \action
    * Call Kill_Coasted_Tracks function.
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);
   
   /** \result
    * Object should be killed because 0.7s > 0.65s (detection drop threshold)
    * Verify by checking:
    * - num_active_objs should be 0 (all objects killed)
    */
   CHECK_EQUAL(0, tracker_info.num_active_objs);
}

/** @}*/
