/** \file
 * This file contains unit tests for content of f360_time_update_tracks.cpp file
 */

#include "f360_time_update_tracks.h"
#include <CppUTest/TestHarness.h>
#include "f360_host_props_update.h"
#include "f360_sorted_tracks_mgmt.h"
#include "f360_set_variant.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_time_update_tracks
 *  @{
 */

/** \brief
 * Test group for Time_Update_Tracks. Tests in this test group verify that tracks are time updated correctly.
 */
TEST_GROUP(f360_time_update_tracks)
{
   // Declare common variables used within all tests in this test group.
   F360_Calibrations_T calibrations = {};
   float32_t elapsed_time_s = 1.0F;
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Host_Props_T host_props = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Globals_T globals;

   int32_t obj_idx = 10;
   int32_t obj_id = obj_idx + 1;
   float32_t test_pass_th = 1e-8F;
   /** \setup
    * Set up a default scenario where: 
    * - Host is stationary.
    * - There is one active CTCA object that is driving straight towards host with a speed of 2m/s.
    * - Set object to coasting and time since stage start to 0
    * - The active object has an existance probability of 90%.
    * - There is one valid sensor with 100m range.
    * 
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);

      host.vcs_speed = 0.0F;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = obj_id;
      tracker_info.p_lowest_priority_track = &object_tracks[obj_idx];
      tracker_info.p_highest_priority_track = &object_tracks[obj_idx];

      object_tracks[obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[obj_idx].speed = 2.0F;
      object_tracks[obj_idx].vcs_heading = Angle{ 0.0F };
      object_tracks[obj_idx].hdg_ptng_disagmt = 0.0F;
      object_tracks[obj_idx].curvature = 0.0F;
      object_tracks[obj_idx].vcs_position.x = -10.0F;
      object_tracks[obj_idx].vcs_position.y = 0.0F;
      object_tracks[obj_idx].id = obj_id;

      object_tracks[obj_idx].status = F360_OBJECT_STATUS_COASTED;
      object_tracks[obj_idx].time_since_stage_start = 0.0F;

      object_tracks[obj_idx].exist_prob = 0.9F;

      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].variable.is_valid = true;
      sensors[0].constant.range_limits[sensors[0].variable.look_id] = 100.0F;

      Sorted_Tracks_Insert(tracker_info, &object_tracks[obj_idx]);
   }
};

/** \purpose  
 * Test that position of CTCA object is correct after time updated.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_CTCA_Position)
{
   /** \precond
    * A CTCA object located at vcs position (-10, 0) has been set up in test group with a speed of 2m/s straight towards stationary host.
    * Elapsed time has been set to 1.0 in test group.
    * Set expected position to (-8, 0).
    */
   const float32_t expected_long_pos = -8.0F;
   const float32_t expected_lat_pos = 0.0F;
   Host_Props_Update(tracker_info.elapsed_time_s, host, host_props);
   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that object longitudinal and lateral position was updated correctly.
    */
   DOUBLES_EQUAL_TEXT(expected_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_th, "Object longitudinal position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_th, "Object lateral position was not updated correctly.");
}

/** \purpose
 * Test that position of CCA object is correct after time updated.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_CCA_Position)
{
   /** \precond
    * An object located at vcs position (-10, 0) has been set up in test group.
    * Elapsed time has been set to 1.0 in test group.
    * Set object filter type to CCA
    * Set object vcs velocity to (1, -1) and update object speed accordingly.
    * Set expected position to (-9, 1)
    */
   object_tracks[obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[obj_idx].vcs_velocity.longitudinal = 1.0F;
   object_tracks[obj_idx].vcs_velocity.lateral = -1.0F;
   object_tracks[obj_idx].speed = F360_Sqrtf(2.0F);
   const float32_t expected_long_pos = -9.0F;
   const float32_t expected_lat_pos = -1.0F;
   Host_Props_Update(tracker_info.elapsed_time_s, host, host_props);
   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that object longitudinal and lateral position was updated correctly.
    */
   DOUBLES_EQUAL_TEXT(expected_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_th, "Object longitudinal position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_th, "Object lateral position was not updated correctly.");
}

/** \purpose
 * Test that coasting objects are killed as expected.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_Kill_Coasted_Track)
{
   /** \precond
    * A coasting object has been set up in test group.
    * Number of active tracks have been set to 1 in test group.
    * Set time since stage start for the coasting track to 1.0
    */
   object_tracks[obj_idx].time_since_stage_start = 1.0F;

   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that the id for the killed object is part of inactive obj ids array, that the number of active objects is 0 and object status is set to invalid.
    */
   CHECK_TRUE_TEXT(obj_idx + 1 == tracker_info.inactive_obj_ids[NUMBER_OF_OBJECT_TRACKS - 1U], "The id for the killed object was not put into the inactive obj ids array as expected.");
   CHECK_TRUE_TEXT(0 == tracker_info.num_active_objs, "The number of active objects was not decremented as expected when a coasting object was killed.");
   CHECK_TRUE_TEXT(F360_OBJECT_STATUS_INVALID == object_tracks[obj_idx].status, "Object status was not set to invalid when it should have been.")
}

/** \purpose
 * Test that object list timestamp is updated as expected.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_Object_Timestamp)
{
   /** \precond
    * An object has been set up in test group.
    * Number of active tracks have been set to 1 in test group.
    * Set elapsed time to 4.0.
    * Set sensor is_valid to false.
    * Set tracker info object_list_timestamp to 2.
    * Set tracker info elapsed time to 4.0.
    * Set expected object list timestamp to tracker_info.object_list_timestamp + tracker_info.elapsed_time_s * 1.0e6F
    */
   sensors[0].variable.is_valid = false;
   tracker_info.object_list_timestamp = 2U;
   tracker_info.elapsed_time_s = 4.0F;
   const float32_t expected_object_list_timestamp = tracker_info.object_list_timestamp + tracker_info.elapsed_time_s * 1.0e6F;

   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that tracker info object_list_timestamp was updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_object_list_timestamp, tracker_info.object_list_timestamp, test_pass_th, "Object list timestamp was not updated correctly in tracker_info.")
}

/** \purpose
 * Test that host delta vcs position is calculated correctly.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_Host_Delta_Position)
{
   /** \precond
    * Elapsed time has been set to 1.0 in test group
    * Set host vcs speed to 5.0
    * Set expected host delta vcs position to (5, 0)
    */
   host.vcs_speed = 5.0F;
   tracker_info.elapsed_time_s = 1.0F;

   const float32_t expected_host_delta_vcs_long_pos = 5.0F;
   const float32_t expected_host_delta_vcs_lat_pos = 0.0F;

   Host_Props_Update(tracker_info.elapsed_time_s, host, host_props);
   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that host delta vcs position is updated correctly.
    */
   DOUBLES_EQUAL_TEXT(expected_host_delta_vcs_long_pos, host_props.delta_position_x, test_pass_th, "Host delta vcs longitudinal position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_host_delta_vcs_lat_pos, host_props.delta_position_y, test_pass_th, "Host delta vcs lateral position was not updated correctly.");
}

/** \purpose
 * Test that object vcs position is compensated for host movement.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_Compensate_For_Host_Movemement)
{
   /** \precond
    * An object located at vcs position (-10, 0) has been set up in test group.
    * Number of active tracks have been set to 1 in test group.
    * Elapsed time has been set to 1.0 in test group
    * Set host vcs speed to 5.0
    * Set object speed to 0.
    * Set object expected vcs position to (-15, 0)
    */
   host.vcs_speed = 5.0F;
   object_tracks[obj_idx].speed = 0.0F;
   tracker_info.elapsed_time_s = 1.0F;
   const float32_t expected_vcs_long_pos = -15.0F;
   const float32_t expected_vcs_lat_pos = 0.0F;
   Host_Props_Update(tracker_info.elapsed_time_s, host, host_props);
   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that object vcs position was compensated correctly for host movement.
    */
   DOUBLES_EQUAL_TEXT(expected_vcs_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_th, "Object longitudinal position was not compensated correctly for host movement.");
   DOUBLES_EQUAL_TEXT(expected_vcs_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_th, "Object lateral position was not compensated correctly for host movement.");
}

/** \purpose
 * Test that object existence probability is time updated correctly.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_Existence_Probability)
{
   /** \precond
    * An object with existence probability of 0.6 has been set up in test group.
    * Set object expected existence probabilty to 0.891
    */

   const float32_t expected_exist_prob = 0.891F;

   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that object existence probability was updated correctly.
    */
   DOUBLES_EQUAL_TEXT(expected_exist_prob, object_tracks[obj_idx].exist_prob, test_pass_th, "Object existence probability was not updated correctly.");
}

/** \purpose
 * Test that sorting of tracks according to their time updated priority is working as expected.
 * \req NA
 */
TEST(f360_time_update_tracks, Time_Update_Tracks_Update_Track_Priority)
{
   /** \precond
    * An object located at vcs position (-10, 0) has been set up in test group.
    * Create a second coasting CTCA object at vcs position (-5, 0) that drives straight towards host with a speed of 1m/s.
    * Set number of active objects to 2
    * Add object id for second object to active object ids 
    * Set first object to highest priority track 
    * Set second object to lowest priority track
    * Link the two objects together with lower/higher priority track pointer
    */
   const int32_t obj_idx_2 = 0;
   const int32_t obj_id_2 = obj_idx_2 + 1;

   object_tracks[obj_idx_2].id = obj_id_2;
   object_tracks[obj_idx_2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[obj_idx_2].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[obj_idx_2].vcs_position.x = -5.0F;
   object_tracks[obj_idx_2].vcs_position.y = 0.0F;
   object_tracks[obj_idx_2].speed = 1.0F;
   object_tracks[obj_idx_2].vcs_heading = Angle{ 0.0F };

   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[1] = obj_id_2;
   tracker_info.p_highest_priority_track = &object_tracks[obj_idx];
   tracker_info.p_lowest_priority_track = &object_tracks[obj_idx_2];
   object_tracks[obj_idx].p_lower_priority_track = &object_tracks[obj_idx_2];
   object_tracks[obj_idx_2].p_higher_priority_track = &object_tracks[obj_idx];

   Sorted_Tracks_Insert(tracker_info, &object_tracks[obj_idx_2]);
   Host_Props_Update(tracker_info.elapsed_time_s, host, host_props);
   /** \action
    * Call Time_Update_Tracks.
    */
   Time_Update_Tracks(calibrations, elapsed_time_s, host, sensors, host_props, globals, object_tracks, tracker_info, timing_info);

   /** \result
    * Verify that the two objects changed priority.
    */
   CHECK_TRUE_TEXT(tracker_info.p_highest_priority_track == &object_tracks[obj_idx_2], "The second object was not the highest priority track when it should have been.");
   CHECK_TRUE_TEXT(tracker_info.p_lowest_priority_track == &object_tracks[obj_idx], "The first object was not the lowest priority track when it should have been.");
   CHECK_TRUE_TEXT(object_tracks[obj_idx].p_higher_priority_track == &object_tracks[obj_idx_2], "The first object does not point to the second object as it should.");
   CHECK_TRUE_TEXT(object_tracks[obj_idx_2].p_lower_priority_track == &object_tracks[obj_idx], "The second object does not point to the first object as it should.");
}
/** @}*/
