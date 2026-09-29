/** \file
   Give a detailed description of what this unit-test file contain.
*/

#include "f360_track_downselection.h"
#include "f360_object_track_equal_operator.h"
#include "f360_set_variant.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cstring>

using namespace f360_variant_A;

/** \brief
*  Test group of F360_Downselection module
**/
TEST_GROUP(f360_track_downselection)
{
   F360_Host_T host = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   Static_Env_Poly_T stat_env_poly[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Purpose of this test is to verify whether longer living tracks are prioritized
*\req    NA
*/
TEST(f360_track_downselection, f360_core_track_downselect__check_if_longer_living_tracks_are_prioritized)
{
   /** \precond
   Set up tracks that fill requirements: one has to be long-living one, the other - just initialized
   **/

   // set up initial parameters
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = 2;
   tracker_info.reduced_num_active_objs = 2;

   // set parameters to make one track long-living and the other - new one
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[1].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[1].vcs_position.x = 20.0F;

   object_tracks[0].time_since_initialization = 0.0F;
   object_tracks[1].time_since_initialization = 20.0F;

   object_tracks[0].filtered_dets = 1.25F;
   object_tracks[1].filtered_dets = 1.25F;

   // set specific parameters to be able to reach specific conditions checking track time of live
   object_tracks[0].total_reduced_dets = 1;
   object_tracks[1].total_reduced_dets = 1;

   object_tracks[0].behind_sep_id = 1U;
   object_tracks[1].behind_sep_id = 1U;

   object_tracks[0].movable_prob = 1.0F;
   object_tracks[1].movable_prob = 1.0F;

   object_tracks[0].confidenceLevel = 0.55F;
   object_tracks[1].confidenceLevel = 1.0F;

   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[1].conf_overall = CONF3_HIGH;

   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   calib.low_confidence_level_thresh = 0.5F;

   // set reduced_inactive_obj_ids (no reduced objects in current scan)
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }
   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, stat_env_poly, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether confidence level of first track was lowwered
   **/
   CHECK_TRUE(object_tracks[0].confidenceLevel < object_tracks[1].confidenceLevel);
}

/**
*\purpose  Purpose of this test is to verify whether error is detected
            when number of ids stored in tracker info does not match
            number_of_reduced_object_tracks
*\req    NA
*/
TEST(f360_track_downselection, f360_core_track_downselect__check_if_breakes_when_too_many_reduced_objects)
{
   /** \precond
   Create more tracks than NUMBER_OF_REDUCED_OBJECT_TRACKS
   Set up their params to generate one error while PoP_Reduced_Id
   **/

   // set up initial parameters
   const int num_to_reduce = 5;
   tracker_info.elapsed_time_s = 1.0F;
   tracker_info.num_active_objs = NUMBER_OF_REDUCED_OBJECT_TRACKS + num_to_reduce;
   tracker_info.reduced_num_active_objs = NUMBER_OF_REDUCED_OBJECT_TRACKS;
   const int idx_to_reduce[num_to_reduce] = { 0, 5, 73, 84, 95 };
   const int idx_to_reduce_priority = 128;

   // set parameters of all tracks to be the same
   for (int i = 0; i < tracker_info.num_active_objs; i++)
   {
      object_tracks[i].vcs_position.x = 20.0F;
      object_tracks[i].vcs_position.y = 20.0F;
      object_tracks[i].movable_prob = 1.0F;
      object_tracks[i].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[i].conf_overall = CONF3_HIGH;
      object_tracks[i].reduced_id = i + 1;
      tracker_info.active_obj_ids[i] = i + 1;
   }

   // set reduced id of over-limit tracks to 0
   for (int i = tracker_info.reduced_num_active_objs; i < tracker_info.num_active_objs; i++)
   {
      object_tracks[i].reduced_id = 0;
   }

   // increase lateral position of selected tracks to reduce their priority
   for (int i = 0; i < num_to_reduce; i++)
   {
      int idx = idx_to_reduce[i];
      object_tracks[idx].vcs_position.x = 25.0F;

   }

   // increase lateral position of one selected track to make its priority higher than ones to reduce but lowwer than all rest
   object_tracks[idx_to_reduce_priority].vcs_position.x = 22.0F;

   // write reduced_ids into structures informing whether they are used or not
   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      if (i < (uint32_t)tracker_info.reduced_num_active_objs)
      {
         tracker_info.reduced_active_obj_ids[i] = i + 1;
      }
      else
      {
         tracker_info.reduced_inactive_obj_ids[i - tracker_info.reduced_num_active_objs] = i + 1;
      }
   }

   // reset reduced_id of one active object to create error
   object_tracks[idx_to_reduce[0]].reduced_id = 0;

   /** \action
   * call Track_Downselection
   **/
   Track_Downselection(host, calib, stat_env_poly, sensors, object_tracks, tracker_info, timing_info);

   /** \result
   * Verify whether track with idx idx_to_reduce_priority does not have assigned reduced_id.
   * It will confirm that error was detected.
   **/

   CHECK_EQUAL(0, object_tracks[idx_to_reduce_priority].reduced_id);
}
