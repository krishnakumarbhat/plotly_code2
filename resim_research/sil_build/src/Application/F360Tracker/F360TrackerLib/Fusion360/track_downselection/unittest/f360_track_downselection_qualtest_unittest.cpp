/** \file
   This file contains qualification tests for Track Downselection module.
*/

#include "f360_track_downselection.h"
#include "f360_track_downselection_internal_functions.h"
#include "f360_priority_update_tracks.h"
#include "f360_set_variant.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cstring>

using namespace f360_variant_A;

/** \defgroup Track_Downselection
*  @{
*/

/** \brief
   This test group is used for creatie data structure needed in further tests.
 */
TEST_GROUP(f360_track_downselection_qualtest)
{
   F360_Host_T host = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Purpose of this test is to verify whether tracker does downselect tracks basing on their calculated priority.
*\req    CPR-3749
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_does_reduce_tracks_basing_on_priority)
{
   /** \precond
   Set up more tracks than NUMBER_OF_REDUCED_OBJECT_TRACKS
   Assing them proper values so priority will be calculated for them
   Do not assign them reduced_id
   Increase lateral positon of selected tracks to decrease their priority
   **/
   // set initial parameters
   const int num_to_reduce = 5;
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = NUMBER_OF_REDUCED_OBJECT_TRACKS + num_to_reduce;
   tracker_info.reduced_num_active_objs = 0;
   int idx_to_reduce[num_to_reduce] = { 0, 5, 73, 102, 129 };

   // set parameters of all tracks to have same priority
   for (int i = 0; i < tracker_info.num_active_objs; i++)
   {
      object_tracks[i].vcs_position.x = 20.0F;
      object_tracks[i].vcs_position.y = 20.0F;
      object_tracks[i].movable_prob = 1.0F;
      object_tracks[i].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[i].conf_overall = CONF3_HIGH;
      object_tracks[i].reduced_id = 0;
      tracker_info.active_obj_ids[i] = i + 1;

   }

   // increase lateral position of selected tracks to decrease their priority
   for (int i = 0; i < num_to_reduce; i++)
   {
      int idx = idx_to_reduce[i];
      object_tracks[idx].vcs_position.x = 25.0F;

   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether specified tracks were not assigned reduced_id
   **/
   int num_properly_not_assigned = 0;
   for (int i = 0; i < num_to_reduce; i++)
   {
      if (object_tracks[idx_to_reduce[i]].reduced_id == 0)
      {
         num_properly_not_assigned++;
      }
   }

   CHECK_EQUAL(num_to_reduce, num_properly_not_assigned);
}

/**
*\purpose  Purpose of this test is to verify whether tracks updated are prioritized over new tracks
*\req    CPR-3750
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_updated_tracks_are_prioritized_over_new_tracks)
{
   /** \precond
    * Set up tracks to have same params and have different status
    * Tracks should not be qualified as near host
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make all tracks be qualified differently
   object_tracks[0].status = F360_OBJECT_STATUS_NEW;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].reduced_status = F360_OBJECT_STATUS_INVALID;

   // both tracks should be placed in same position
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 0.0F;

   object_tracks[1].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.y = 0.0F;

   // both tracks should have same confidence level, below threshold
   object_tracks[0].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;
   object_tracks[1].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether first track was not assigned reduced id
   * Verify whether second track was assigned reduced id
   **/
   CHECK_EQUAL(0, object_tracks[0].reduced_id);
   CHECK_TRUE(object_tracks[1].reduced_id > 0);
}

/**
*\purpose  Purpose of this test is to verify whether tracks which are already reduced will not have their reduced id overwritten
*\req    CPR-3750
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_tracks_with_reduced_status_set_as_invalid_are_not_downselected)
{
   /** \precond
    * Set up tracks to have same params and have different reduced status
    * Tracks should not be qualified as near host
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make all tracks be qualified differently
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].reduced_status = F360_OBJECT_STATUS_UPDATED;

   // both tracks should be placed in same position
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 0.0F;

   object_tracks[1].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.y = 0.0F;

   // both tracks should have same confidence level, below threshold
   object_tracks[0].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;
   object_tracks[1].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;

   // first track should have its reduced status assigned
   object_tracks[0].reduced_id = 1;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS - 1; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 2;
   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether first track was not assigned reduced id
   * Verify whether second track was assigned reduced id
   **/
   CHECK_EQUAL(1, object_tracks[0].reduced_id);
   CHECK_TRUE(object_tracks[1].reduced_id > 0);
}

/**
*\purpose  Purpose of this test is to verify whether tracks with higher confidence level are prioritized
*\req    CPR-3750
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_tracks_with_higher_confidence_level_are_prioritized)
{
   /** \precond
    * Set up tracks to have same params and different confidence level
    * Tracks should not be qualified as near host
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make all tracks be qualified differently
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].reduced_status = F360_OBJECT_STATUS_INVALID;

   // both tracks should be placed in same position
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 0.0F;

   object_tracks[1].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.y = 0.0F;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }
   object_tracks[0].confidenceLevel = calib.k_track_downselect_confidence_thresh - 0.1F;
   object_tracks[1].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether first track was not assigned reduced id
   * Verify whether second track was assigned reduced id
   **/
   CHECK_EQUAL(0, object_tracks[0].reduced_id);
   CHECK_TRUE(object_tracks[1].reduced_id > 0);
}

/**
*\purpose  Purpose of this test is to verify whether tracks closer to host are prioritized
*\req    CPR-3750
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_valid_trk_closer_to_host_is_prioritized)
{
   /** \precond
   Set up tracks that fill requirements for being qualified as near host
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 4;
   tracker_info.reduced_num_active_objs = 4;

   // set parameters to make all tracks be qualified differently
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[2].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[3].status = F360_OBJECT_STATUS_NEW_UPDATED;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;
   object_tracks[2].conf_overall = CONF3_HIGH;
   object_tracks[3].conf_overall = CONF3_HIGH;

   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[2].confidenceLevel = 1.0F;
   object_tracks[3].confidenceLevel = 1.0F;

   // far longitudianl, close lateral
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 0.0F;

   // close longitudinal, close lateral
   object_tracks[1].vcs_position.x = 2.0F;
   object_tracks[1].vcs_position.y = 0.0F;

   // close longitudinal, far lateral
   object_tracks[2].vcs_position.x = 2.0F;
   object_tracks[2].vcs_position.y = 15.0F;

   // far longitudinal, far lateral
   object_tracks[3].vcs_position.x = 20.0F;
   object_tracks[3].vcs_position.y = 15.0F;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;
   tracker_info.active_obj_ids[3] = 4;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether tracks were prioritized (lowwer reduced id -> higher priority)
   **/
   CHECK_TRUE(object_tracks[0].reduced_id > object_tracks[1].reduced_id);
   CHECK_TRUE(object_tracks[2].reduced_id > object_tracks[1].reduced_id);
   CHECK_TRUE(object_tracks[3].reduced_id > object_tracks[2].reduced_id);
}

/**
*\purpose  Purpose of this test is to verify whether tracks with lower time to collision are prioritized.
*          A closer object has lower TTC given the same host speed and thus should receive higher priority.
*\req    CPR-3750
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_tracks_with_lower_ttc_are_prioritized)
{
   /** \precond
   Set up two movable tracks at different longitudinal distances.
   The closer track (object 1) has lower TTC than the farther track (object 0).
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make tracks be qualified as movable but with different TTC
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;

   // object 0 is far from host -> higher TTC
   object_tracks[0].vcs_position.x = 100.0F;
   object_tracks[0].vcs_position.y = 0.0F;

   // object 1 is close to host -> lower TTC
   object_tracks[1].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.y = 0.0F;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   object_tracks[0].movable_prob = 1.0F;
   object_tracks[1].movable_prob = 1.0F;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;

   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[1].confidenceLevel = 1.0F;

   // host speed discriminates TTC: closer object has lower TTC
   host.vcs_speed = 20.0F;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }
   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether track with lower TTC (closer object) was prioritized (lower reduced_id)
   **/
   CHECK_TRUE(object_tracks[1].reduced_id < object_tracks[0].reduced_id);
}

/**
*\purpose  Purpose of this test is to verify that tracks are downselected correctly based on their overall confidence
*\req    CPR-3750
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_tracks_are_downselected_as_expected_based_on_overall_confidence)
{
   /** \precond
    * Set up tracks to have same params and have different overall confidence
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make all tracks be qualified differently
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].reduced_status = F360_OBJECT_STATUS_INVALID;

   // both tracks should be placed in same position
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 0.0F;

   object_tracks[1].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.y = 0.0F;

   // both tracks should have the same velocity towards host
   object_tracks[0].vcs_velocity.longitudinal = -5.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[1].vcs_velocity.longitudinal = -5.0F;
   object_tracks[1].vcs_velocity.lateral = 0.0F;

   // both tracks should have same confidence level, above threshold
   object_tracks[0].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;
   object_tracks[1].confidenceLevel = calib.k_track_downselect_confidence_thresh + 0.1F;

   // tracks should have different overall confidence
   object_tracks[0].conf_overall = CONF3_LOW;
   object_tracks[1].conf_overall = CONF3_HIGH;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   object_tracks[0].movable_prob = 1.0F;
   object_tracks[1].movable_prob = 1.0F;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether first track was not assigned reduced id
   * Verify whether second track was assigned reduced id
   **/
   CHECK_EQUAL(0, object_tracks[0].reduced_id);
   CHECK_TRUE(object_tracks[1].reduced_id > 0);
}

/**
*\purpose  Purpose of this test is to verify whether low priority objects have their reduced id taken
*\req    CPR-3751
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_resets_low_priority_target_reduced_id)
{
   /** \precond
   Set up tracks that fill requirements for having low priority
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 3;
   tracker_info.reduced_num_active_objs = 3;

   // set parameters to make all tracks have low priority but with different justification
   object_tracks[0].status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].reduced_status = F360_OBJECT_STATUS_INVALID;
   object_tracks[1].confidenceLevel = 0.0F;
   object_tracks[2].confidenceLevel = 0.0F;
   object_tracks[0].reduced_id = 1;
   object_tracks[1].reduced_id = 2;
   object_tracks[2].reduced_id = 3;
   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;
   object_tracks[2].conf_overall = CONF3_HIGH;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;
   tracker_info.reduced_active_obj_ids[0] = 1;
   tracker_info.reduced_active_obj_ids[1] = 2;
   tracker_info.reduced_active_obj_ids[2] = 3;
   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether their reduced id was reseted.
   **/
   CHECK_EQUAL(0, object_tracks[0].reduced_id);
   CHECK_EQUAL(0, object_tracks[1].reduced_id);
   CHECK_EQUAL(0, object_tracks[2].reduced_id);
}

/**
*\purpose  Purpose of this test is to verify whether tracks with high priority are assigned reduced ID. Low priority targets
*          should not be assigned reduced id.
*\req    CPR-3751
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_assigns_reduced_id_to_higher_priority_targets_and_resets_reduced_id_of_lower_priority_targets)
{
   /** \precond
   Create more tracks than NUMBER_OF_REDUCED_OBJECT_TRACKS.
   Assign reduced_id to tracks in range <1 - NUMBER_OF_REDUCED_TRACKS>.
   Set up parameters of selected tracks in range <1 - NUMBER_OF_REDUCED_TRACKS> so they will be treated as low priority.
   Set up parameters of all other tracks so they will be treated as high priority.
   **/

   // set initial parameters
   const int num_to_reduce = 5;
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = NUMBER_OF_REDUCED_OBJECT_TRACKS + num_to_reduce;
   tracker_info.reduced_num_active_objs = NUMBER_OF_REDUCED_OBJECT_TRACKS;

   int idx_to_reduce[num_to_reduce] = { 0, 5, 73, 84, 95 };
   //Switch preconditions for BMW case during OAL integration
   if (NUMBER_OF_REDUCED_OBJECT_TRACKS <= 50)
   {
      idx_to_reduce[2] = 23;
      idx_to_reduce[3] = 34;
      idx_to_reduce[4] = 49;
   }

   // set parameters of all tracks to have same priority
   for (int i = 0; i < tracker_info.num_active_objs; i++)
   {
      object_tracks[i].vcs_position.x = 20.0F;
      object_tracks[i].vcs_position.y = 20.0F;
      object_tracks[i].movable_prob = 1.0F;
      object_tracks[i].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[i].conf_overall = CONF3_HIGH;
      object_tracks[i].reduced_id = i + 1;
      tracker_info.active_obj_ids[i] = i + 1;
      object_tracks[i].confidenceLevel = 0.5F;

   }

   // set reduced_id of tracks with id higher than reduced_num_active_objs to 0
   for (int i = tracker_info.reduced_num_active_objs; i < tracker_info.num_active_objs; i++)
   {
      object_tracks[i].reduced_id = 0;
   }

   // increase longitudinal position of selected tracks to decrease their priority
   for (int i = 0; i < num_to_reduce; i++)
   {
      int idx = idx_to_reduce[i];
      object_tracks[idx].vcs_position.x = 25.0F;

   }

   // set reduced_active_obj_ids
   for (int i = 0; i < tracker_info.reduced_num_active_objs; i++)
   {
      tracker_info.reduced_active_obj_ids[i] = i + 1;
   }
   // set reduced_inactive_obj_ids
   for (unsigned int i = tracker_info.reduced_num_active_objs; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i - tracker_info.reduced_num_active_objs] = i + 1;
   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether reduced id was taken from tracks that were qualified as low priority.
   * Verify whether new tracks were assigned reduced_id.
   **/

   for (int i = 0; i < num_to_reduce; i++)
   {
      CHECK_TEXT(object_tracks[idx_to_reduce[i]].reduced_id == 0, "Object was not properly taken reduced_id");
   }
   for (int i = tracker_info.reduced_num_active_objs; i < tracker_info.num_active_objs; i++)
   {
      CHECK_TEXT(object_tracks[i].reduced_id > 0, "Object was not properly assigned reduced_id");
   }
}

/**
*\purpose  Purpose of this test is to verify whether tracks that changed their status
* from new to coasted have their confidence level decreased.
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_lowers_confidence_of_tracks_that_changed_their_status_from_new_to_coasted)
{
   /** \precond
   Set up tracks to be marked as changing status from updated to coasted.
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 3;
   tracker_info.reduced_num_active_objs = 3;

   // set parameters to make ghosts
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[2].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].f_ghost_NU_2_C = true;
   object_tracks[1].f_ghost_NU_2_C = true;
   object_tracks[2].f_ghost_NU_2_C = true;
   object_tracks[0].time_since_track_updated = 0.0F;
   object_tracks[1].time_since_track_updated = 1.0F;
   object_tracks[2].time_since_track_updated = 1.0F;

   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.x = 20.0F;
   object_tracks[2].vcs_position.x = 20.0F;

   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[1].confidenceLevel = 1.0F;
   object_tracks[2].confidenceLevel = 0.25F;
   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;
   object_tracks[2].conf_overall = CONF3_HIGH;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   calib.low_confidence_level_thresh = 0.3F;
   calib.k_hyst_time_for_coasted_objects = 0.5F;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether confidence evel of first track is higher than the second one
   * Confidence level of third track should not change
   **/
   CHECK_TRUE(object_tracks[0].confidenceLevel > object_tracks[1].confidenceLevel);
   DOUBLES_EQUAL(0.25F, object_tracks[2].confidenceLevel, F360_EPSILON);
}

/**
*\purpose  Purpose of this test is to verify whether tracks with high mirror probability have their confidence level and priority decreased
*/
TEST(f360_track_downselection_qualtest, f360_core_track_downselect__check_if_lowers_confidence_and_priority_of_mirror_detections)
{
   /** \precond
   Set up tracks that fill requirements for having low priority
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make tracks be qualified as mirror detections
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.x = 20.0F;

   object_tracks[0].mirror_prob = 1.0F;
   object_tracks[1].mirror_prob = 1.0F;

   // set parameters to make confidence level of first track high enuogh to be reduced
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[1].confidenceLevel = 0.2F;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   // set thresholds
   calib.k_mirror_prob_threshold = 0.5F;
   calib.low_confidence_level_thresh = 0.5F;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }
   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, static_env_polys, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether confidence level of first track was redueced
   * and confidence of second track was not changed.
   * Verify whether both tracks were not assigned reduced_id
   **/
   CHECK_TRUE(object_tracks[0].confidenceLevel < 1.0F);
   DOUBLES_EQUAL(0.2F, object_tracks[1].confidenceLevel, F360_EPSILON);
   CHECK_EQUAL(0, object_tracks[0].reduced_id);
   CHECK_EQUAL(0, object_tracks[1].reduced_id);
}

/** \defgroup Track_Downselection__Calculate_Track_Priority_Non_Moveable_Objects
*  @{
*/

/** \brief
   * This test group tests priority assignment strategy for non_moveable objects. It tests the following cases:
   * If host speed should impact importance (for downselection), such that the objects in the direction of host motion, should have higher importance.
   * For forward driving host, objects in front have higher importance. On the flip side, for reverse driving host, objects in the back of host have higher importance
   * The importance of the non_movable objects is decided based on their vcs longitudinal position and curvi lateral position (based on host trajectory)
   * Objects longitudinally further away from the host are given lower importance for downselection, as compared to longitudinally closer objects
   * Objects laterally further away from the host trajectory are given lower importance for downselection, as compared to laterally closer objects
 */
TEST_GROUP(f360_track_downselection_calc_track_priority_qualtest)
{
   F360_Host_T host = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   BoundingBox overall_confidence_exclusion_box = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   bool f_limited_FOV;

   TEST_SETUP()
   {
      // Initialize with default calibrations
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      // Set elpased time
      tracker_info.elapsed_time_s = 0.0001F;

      // Host Properties
      // In setup, host is assumed to be stationary with close to zero curvature
      host.vcs_sideslip = 0.0F;
      host.vcs_speed = 0.0F;
      host.curvature_rear = 0.0001F;
      host.dist_rear_axle_to_vcs_m = 3.0F;

      f_limited_FOV = false;

      // Number of active objects for the test
      tracker_info.num_active_objs = 7;

      // The following objects are set up with the following reasoning:
      // All objects are placed outside the region in which objects close to the host are given high importance by default.
      // All non-movable objects are set up so that they reach a condition where the priority is decided based on host speed and distance from the host's trajectory.
      // There are two objects in the front. Object-1 is longitudinally closer to the host than Object-2.
      // The distance between Object-1 and Object-2 should be considerable, but no exact value is required.
      // Object-3 and Object-4 are placed behind host and are longitudinally mirrored from Object-1 and Object-2, respectively, with respect to the host's center.
      // Object-5 and Object-6 are used for testing lateral prioritization; therefore, their vcs_x positions are the same.
      // Object-5 is meant to be laterally further away from the host's trajectory than Object-6.
      // Object-7 is the only movable object in this setup and is set far away from the host.
      // The purpose of Object-7 is to ensure that, regardless of conditions, the movable object is always of top importance compared to non-movable objects.

      for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
      {
         object_tracks[loop_idx].movable_prob = 0.0F;
         object_tracks[loop_idx].confidenceLevel = 0.9F;
         object_tracks[loop_idx].status = F360_OBJECT_STATUS_NEW_UPDATED;
         object_tracks[loop_idx].conf_overall = CONF3_HIGH;
         tracker_info.active_obj_ids[loop_idx] = loop_idx + 1;
      }
      // Object 1
      object_tracks[0].vcs_position = {20.0F, 0.0F};

      // Object 2
      object_tracks[1].vcs_position = {100.0F, 0.0F};

      // Object 3
      object_tracks[2].vcs_position = {-23.0F, 0.0F};

      // Object 4
      object_tracks[3].vcs_position = {-103.0F, 0.0F};

      // Object 5
      object_tracks[4].vcs_position = {35.0F, 8.0F};

      // Object 6
      object_tracks[5].vcs_position = {35.0F, -1.0F};

      // Pre-compute obj_trk.priority for each object (used by Calc_Track_Priority)
      Update_Track_Priority(host, object_tracks, tracker_info);
   }
};

/**
*\purpose  Purpose of this test is to verify that priority of objects is decided correctly, when host is stationary
*\req    CPR-3752
*/
TEST(f360_track_downselection_calc_track_priority_qualtest, Host_Is_Stationary)
{
   /** \precond
    * Same as the test setup i.e host speed and curvature is zero
   */
   float32_t priorities[7] = {};

   /** \action
      Call Calc_Track_Priority
   */
   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   /** \result
    * Higher priority value means more importance for downselection
   */
   // In front of host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities[0] > priorities[1]);

   // Behind the host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities[2] > priorities[3]);

   // Expect the priority value of laterally closer object to host trajectory to be higher than a further object
   CHECK_TRUE(priorities[5] > priorities[4]);

}

/**
*\purpose  Purpose of this test is to verify that priority of objects is decided correctly, when host is moving forward
*\req    CPR-3752
*/
TEST(f360_track_downselection_calc_track_priority_qualtest, Host_Is_Moving)
{
   /** \precond
    * Set host speed to be positive (5.0 m/s)
    * Host curvature is close to zero (0.0001)
   */
   host.vcs_speed = 5.0F;
   Update_Track_Priority(host, object_tracks, tracker_info);
   float32_t priorities[7] = {};

   /** \action
      Call Calc_Track_Priority
   */
   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   /** \result
    * Higher priority value means more importance for downselection
   */
   // In front of host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities[0] > priorities[1]);

   // Behind the host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities[2] > priorities[3]);

   // When host is moving forward, objects in front are closer to collision and have higher priority
   CHECK_TRUE(priorities[0] > priorities[2]);
   CHECK_TRUE(priorities[1] > priorities[3]);

   // Expect the priority value of laterally closer object to host trajectory to be higher than a further object
   CHECK_TRUE(priorities[5] > priorities[4]);
}

/**
*\purpose  Purpose of this test is to verify that priority of objects is decided correctly, when host is reversing
*\req    CPR-3752
*/
TEST(f360_track_downselection_calc_track_priority_qualtest, Host_Is_Reversing)
{
   /** \precond
    * Set host speed to be -3.0 m/s i.e the host is reversing 
    * Host curvature is close to zero (0.0001)
   */
   host.vcs_speed = -3.0F;
   Update_Track_Priority(host, object_tracks, tracker_info);
   float32_t priorities[7] = {};

   /** \action
      Call Calc_Track_Priority
   */
   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   /** \result
    * Higher priority value means more importance for downselection
   */
   // In front of host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities[0] > priorities[1]);

   // Behind the host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities[2] > priorities[3]);

   // When host is reversing, closer objects to host still have higher priority
   CHECK_TRUE(priorities[0] > priorities[2]);
   CHECK_TRUE(priorities[1] > priorities[3]);

   // Expect the priority value of laterally closer object to host trajectory to be higher than a further object
   CHECK_TRUE(priorities[5] > priorities[4]);
}

/**
*\purpose  Purpose of this test is to verify that priority of objects is decided correctly, when host is moving fast and forward
*          The test also checks the impact of host speed on object priority calculation
*\req    CPR-3752
*/
TEST(f360_track_downselection_calc_track_priority_qualtest, Impact_oF_Host_Speeds_On_Object_Priorities)
{
   /** \precond
    * Call the Calc_Track_Priority with slow host speed (5m/s)
    * After that, set the host speed to be high (30m/s)
    * The Host curvature is close to zero (0.0001) in both cases
    * The idea here is to test the impact of host speed on object priorities
   */
   host.vcs_speed = 5.0F;
   Update_Track_Priority(host, object_tracks, tracker_info);
   float32_t priorities_slow_host[7] = {};

   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities_slow_host[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   host.vcs_speed = 30.0F;
   Update_Track_Priority(host, object_tracks, tracker_info);
   float32_t priorities_fast_host[7] = {};

   /** \action
      Call Calc_Track_Priority
   */
   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities_fast_host[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   /** \result
    * Higher priority value means more importance for downselection
   */
   // In front of host
   // Expect the priority of far away front object to be higher when host is moving fast
   CHECK_TRUE(priorities_fast_host[1] > priorities_slow_host[1]);
}

/**
*\purpose  Purpose of this test is to verify that priority of objects is decided correctly, when the host has non close to zero curvature
*          The test also checks if the host tracjectory is taken into account, when deciding the priorities
*\req    CPR-3752
*/
TEST(f360_track_downselection_calc_track_priority_qualtest,Host_Curvature_Is_Not_Close_To_Zero)
{
   /** \precond
    * Call the Calc_Track_Priority with zero host curvature and slow host speed (5m/s)
    * After that, set the host curvature to be relatvely higher (0.005 m^-1)
    * The idea here is to test the impact of higher host curvature (0.005) on object priorities
   */
   host.vcs_speed = 5.0F;
   host.curvature_rear = 0.0F;
   tracker_info.elapsed_time_s = 0.001F;
   Update_Track_Priority(host, object_tracks, tracker_info);
   float32_t priorities_zero_curvature_host[7] = {};

   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities_zero_curvature_host[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   host.curvature_rear = 0.005F;
   Update_Track_Priority(host, object_tracks, tracker_info);
   float32_t priorities_non_zero_curvature_host[7] = {};

   /** \action
      Call Calc_Track_Priority
   */
   for (int32_t loop_idx = 0; loop_idx < tracker_info.num_active_objs; loop_idx++)
   {
      priorities_non_zero_curvature_host[loop_idx] = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[loop_idx]);
   }

   /** \result
    * Higher priority value means more importance for downselection
   */
   // Since the host trajectory is curved now, Object 5 is laterally closer to the host trajectory than before
   CHECK_TRUE(priorities_non_zero_curvature_host[4] > priorities_zero_curvature_host[4]);

   // On the other hand, Object 6 is now further from host trajectory, so priority should be lower
   CHECK_TRUE(priorities_non_zero_curvature_host[5] < priorities_zero_curvature_host[5]);

   // The remaining check the same properties as the slow moving test with zero curvature, in order to verify that they hold for non_zero curvatures
   // In front of host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities_non_zero_curvature_host[0] > priorities_non_zero_curvature_host[1]);

   // Behind the host, expect the priority of closer object to be higher than far away object
   CHECK_TRUE(priorities_non_zero_curvature_host[2] > priorities_non_zero_curvature_host[3]);

   // When host is moving forward, objects in front are closer to collision and have higher priority
   CHECK_TRUE(priorities_non_zero_curvature_host[0] > priorities_non_zero_curvature_host[2]);
   CHECK_TRUE(priorities_non_zero_curvature_host[1] > priorities_non_zero_curvature_host[3]);

   // Expect the priority value of laterally closer object to host trajectory to be higher than a further object
   CHECK_TRUE(priorities_non_zero_curvature_host[5] > priorities_non_zero_curvature_host[4]);
}
/** @}*/
