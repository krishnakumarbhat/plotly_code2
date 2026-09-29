/** \file
   This file contains qualification tests (which are also unit tests) for f360_track_grouping.cpp
*/

#include "f360_track_grouping.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cfloat>

#include "f360_static_env_helpers.h"
#include "f360_calibrations.h"
#include "f360_set_variant.h"

/** \defgroup  f360_track_grouping
 *  @{
 */
using namespace f360_variant_A;
 /** \brief
 *  This purpose of this test group is for testing the functionality
 *  of the Track_Grouping() function.
 **/
TEST_GROUP(f360_track_grouping)
{
   /** \setup
    * Initialization of  used variables.
    **/
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T trkrInfo = {};
   F360_Calibrations_T calib;
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Globals_T globals = {};

   TEST_SETUP()
   {
      // Setup tracker calibrations
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(trkrInfo.variant);

      globals.f_single_front_center_radar_only = false;

      // Initialize static environment polynomials
      for (uint32_t idx = 0U; idx < F360_NUM_OF_STATIC_ENV_POLYS; idx++)
      {
         Reset_Single_Static_Env_Poly(static_env_polys[idx]);
      }

      // Initialize base object properties for merging
      trkrInfo.active_obj_ids[0] = 1;
      trkrInfo.active_obj_ids[1] = 2;
      object_tracks[0].id = 1;
      trkrInfo.active_obj_ids[0] = 1;
      trkrInfo.active_obj_ids[1] = 2;
      object_tracks[0].movable_prob = 1.0F;
      object_tracks[0].f_moving = true;
      object_tracks[0].pseudo_vcs_position = { 5.0F, 10.0F };
      object_tracks[0].vcs_velocity = { 20.0F, 1.0F };
      object_tracks[0].Update_Bbox_Size(3.0F, 2.5F);
      object_tracks[0].time_since_initialization = 15.0F;
      object_tracks[0].vcs_heading = Angle{ 0.0F };
      object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[0].speed = 10.0F;
      object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[0].hdg_ptng_disagmt = 0.0F;
      object_tracks[0].ndets = 1U;
      object_tracks[0].detids[0] = 1U;

      object_tracks[1] = object_tracks[0];
      object_tracks[1].id = 2;
      object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[1].detids[0] = 2U;

      detection_props[object_tracks[0].detids[0]-1U].vcs_position.x = object_tracks[0].vcs_position.x;
      detection_props[object_tracks[0].detids[0]-1U].vcs_position.y = object_tracks[0].vcs_position.y;
      raw_detection_list.detections[object_tracks[0].detids[0]-1U].raw.sensor_id = 1U;

      detection_props[object_tracks[1].detids[0]-1U].vcs_position.x = object_tracks[0].vcs_position.x;
      detection_props[object_tracks[1].detids[0]-1U].vcs_position.y = object_tracks[0].vcs_position.y;
      raw_detection_list.detections[object_tracks[1].detids[0]-1U].raw.sensor_id = 1U;
   }
};

/**
*\purpose  The purpose of this test is to check,
 *         if two objects with same parameters of position,
 *         velocity and heading have been merged.
 *\req     CPR-3783
 */
TEST(f360_track_grouping, Test_Similar_Objects_Were_Merged)
{
   /** \precond
    *  Define order of sorted tracks and number of active objects.
    **/
   trkrInfo.num_active_objs = 2;
   trkrInfo.vcslong_sorted_start = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];
   trkrInfo.vcslong_sorted_prev_track[0] = NULL;
   trkrInfo.vcslong_sorted_next_track[1] = NULL;
   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  One of defined object should be invalidated
    **/
   bool f_is_one_object_invalid = (F360_OBJECT_STATUS_UPDATED == object_tracks[0].status) && (F360_OBJECT_STATUS_INVALID == object_tracks[1].status);
   f_is_one_object_invalid |= (F360_OBJECT_STATUS_INVALID == object_tracks[0].status) && (F360_OBJECT_STATUS_UPDATED == object_tracks[1].status);

   CHECK_TRUE(f_is_one_object_invalid);
}


/**
*\purpose  The purpose of this test is to check,
 *         if size of merged object doesn't exceed maximum parameters.
 *\req     CPR-3784
 */
TEST(f360_track_grouping, Test_Merged_Obj_Max_Size_Is_Not_excced)
{
   /** \precond
    *  Define order of sorted tracks.
    *  Define objects width which is greater that maximum width value.
    **/
   trkrInfo.num_active_objs = 2;

   trkrInfo.vcslong_sorted_start = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];
   trkrInfo.vcslong_sorted_prev_track[0] = NULL;
   trkrInfo.vcslong_sorted_next_track[1] = NULL;
   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Merged object width should be less or equal than maximum object width
    **/
   bool f_object_has_proper_size = (calib.k_movable_max_target_width >= object_tracks[0].bbox.Get_Width());
   CHECK_TRUE(f_object_has_proper_size);
}

/**
*\purpose  The purpose of this test is to check,
 *         if all detections previously associated to original object
 *         are still associated to newly merged one.
 *\req     CPR-3785
 */
TEST(f360_track_grouping, Test_Remain_Dets_Assoc)
{
   /** \precond
    *  Define order of sorted tracks.
    *  Define detections associated to object.
    **/
   trkrInfo.num_active_objs = 2;

   trkrInfo.vcslong_sorted_start = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];
   trkrInfo.vcslong_sorted_prev_track[0] = NULL;
   trkrInfo.vcslong_sorted_next_track[1] = NULL;
   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];

   uint32_t expected_num_of_dets = MAX_DETS_IN_OBJ_TRK;
   for (uint32_t idx = 0U; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      detection_props[idx].object_track_id = object_tracks[1].id;
      object_tracks[1].detids[idx] = idx + 1U;
      detection_props[idx].vcs_position.x = object_tracks[1].vcs_position.x;
      detection_props[idx].vcs_position.y = object_tracks[1].vcs_position.y;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
   }
   object_tracks[1].ndets = MAX_DETS_IN_OBJ_TRK;

   detection_props[MAX_DETS_IN_OBJ_TRK].object_track_id = object_tracks[0].id;
   detection_props[MAX_DETS_IN_OBJ_TRK].vcs_position.x = object_tracks[0].vcs_position.x;
   detection_props[MAX_DETS_IN_OBJ_TRK].vcs_position.y = object_tracks[0].vcs_position.y;
   object_tracks[0].detids[0] = MAX_DETS_IN_OBJ_TRK + 1U;
   object_tracks[0].ndets = 1U;

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Number of detections in merged object should be equal to expected value.
    *  Killed object should not contain any associated detections.
    **/
   CHECK_EQUAL(expected_num_of_dets, object_tracks[1].ndets);
   CHECK_EQUAL(0U, object_tracks[0].ndets);
}

/**
*\purpose  The purpose of this test is to check,
 *         if detections previously associated to killed object
 *         are associated properly to newly merged one.
 *\req     CPR-3786
 */
TEST(f360_track_grouping, Test_Move_All_Dets_Assoc)
{
   /** \precond
    *  Define order of sorted tracks.
    *  Define detections associated to killed Object.
    **/
   trkrInfo.num_active_objs = 2;

   trkrInfo.vcslong_sorted_start = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];
   trkrInfo.vcslong_sorted_prev_track[0] = NULL;
   trkrInfo.vcslong_sorted_next_track[1] = NULL;
   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];

   uint32_t expected_num_of_dets = MAX_DETS_IN_OBJ_TRK;
   for (uint32_t idx = 0U; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      detection_props[idx].object_track_id = object_tracks[0].id;
      object_tracks[0].detids[idx] = idx + 1U;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
   }
   object_tracks[0].ndets = MAX_DETS_IN_OBJ_TRK;

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Number of detections in merged object should be equal to expected value.
    *  Killed object should not contain any associated detections.
    **/
   CHECK_EQUAL(expected_num_of_dets, object_tracks[1].ndets);
   CHECK_EQUAL(0U, object_tracks[0].ndets);
}

/**
*\purpose  The purpose of this test is to check,
 *         if detections previously associated to killed object
 *         are associated properly to newly merged one,
 *         when sum of detections from both objects is greater
 *         than max number of detections associated to object.
 *\req     CPR-3786
 */
TEST(f360_track_grouping, Test_Move_Dets_Assoc)
{
   /** \precond
    *  Define order of sorted tracks.
    *  Define detections association to both objects.
    **/
   trkrInfo.num_active_objs = 2;

   trkrInfo.vcslong_sorted_start = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];
   trkrInfo.vcslong_sorted_prev_track[0] = NULL;
   trkrInfo.vcslong_sorted_next_track[1] = NULL;
   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];

   uint32_t number_of_dets_in_kept_obj = MAX_DETS_IN_OBJ_TRK - 10U;
   for (uint32_t idx = 0U; idx < number_of_dets_in_kept_obj; idx++)
   {
      detection_props[idx].object_track_id = object_tracks[1].id;
      object_tracks[1].detids[idx] = idx + 1U;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
   }
   object_tracks[1].ndets = number_of_dets_in_kept_obj;

   for (uint32_t idx = 0U; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      uint32_t det_idx = idx + MAX_DETS_IN_OBJ_TRK;
      detection_props[det_idx].object_track_id = object_tracks[0].id;
      object_tracks[0].detids[idx] = det_idx + 1U;
      raw_detection_list.detections[det_idx].raw.sensor_id = 1;
      (object_tracks[0].ndets)++;
   }
   object_tracks[0].ndets = number_of_dets_in_kept_obj;

   uint32_t expected_num_of_dets = MAX_DETS_IN_OBJ_TRK;

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Number of detections in merged object should be equal to expected value.
    *  Killed object should not contain any associated detections.
    **/
   CHECK_EQUAL(expected_num_of_dets, object_tracks[1].ndets);
   CHECK_EQUAL(0U, object_tracks[0].ndets);
}

/**
*\purpose  The purpose of this test is to check,
 *         if newly merged object has id of kept object.
 *\req     CPR-3787
 */
TEST(f360_track_grouping, Check_Consinsency_In_Obj_Id)
{
   /** \precond
    *  Define order of sorted tracks.
    **/

   trkrInfo.num_active_objs = 2;

   trkrInfo.vcslong_sorted_start = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];
   trkrInfo.vcslong_sorted_prev_track[0] = NULL;
   trkrInfo.vcslong_sorted_next_track[1] = NULL;
   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];

   int32_t exp_obj_id = 1;
   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Merged object id should be equal to expected vaule.
    **/
   CHECK_EQUAL(exp_obj_id, object_tracks[0].id);
}

/**
*\purpose  The purpose of this test is to check,
 *         if sort list of sorted object by vcs long
 *         position was updated properly.
 *\req     CPR-3789
 */
TEST(f360_track_grouping, Check_Updates_In_Long_Pos_Sort)
{
   /** \precond
    *  Fill relevant properties of object with same values,
    *  Define different IDs for this object.
    *  Define order of sorted tracks.
    **/

   trkrInfo.num_active_objs = 5;

   trkrInfo.active_obj_ids[2] = 3;
   trkrInfo.active_obj_ids[3] = 4;
   trkrInfo.active_obj_ids[4] = 5;

   object_tracks[2].id = 3;
   object_tracks[2].pseudo_vcs_position = { 1.0F, 11.0F };
   object_tracks[3].id = 4;
   object_tracks[3].pseudo_vcs_position = { 22.0F, 11.0F };
   object_tracks[4].id = 5;
   object_tracks[4].pseudo_vcs_position = { 2.0F, 11.0F };

   // Order of object idx by vcs long  2,4,3,0,1
   trkrInfo.vcslong_sorted_start = &object_tracks[2];
   trkrInfo.vcslong_sorted_prev_track[0] = &object_tracks[3];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];

   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[1] = NULL;

   trkrInfo.vcslong_sorted_prev_track[2] = NULL;
   trkrInfo.vcslong_sorted_next_track[2] = &object_tracks[4];

   trkrInfo.vcslong_sorted_prev_track[3] = &object_tracks[4];
   trkrInfo.vcslong_sorted_next_track[3] = &object_tracks[0];

   trkrInfo.vcslong_sorted_prev_track[4] = &object_tracks[1];
   trkrInfo.vcslong_sorted_next_track[4] = &object_tracks[3];

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Check if vcslong_sorted list changed as expected.
    *  Order of object idx by vcs long should have been changed 
    *  from 2,4,3,0,1 to 2,4,3,0
    **/
   POINTERS_EQUAL(NULL, trkrInfo.vcslong_sorted_next_track[0]);
}

/**
*\purpose  The purpose of this test is to check,
 *         if object priority list was updated properly.
 *\req     CPR-3788
 */
TEST(f360_track_grouping, Check_Updates_In_Priority_Sort)
{
   /** \precond
    *  Fill relevant properties of object with same values,
    *  Define different IDs for this object.
    *  Define order of sorted tracks.
    **/

   trkrInfo.num_active_objs = 5;

   trkrInfo.active_obj_ids[2] = 3;
   trkrInfo.active_obj_ids[3] = 4;
   trkrInfo.active_obj_ids[4] = 5;

   trkrInfo.vcslong_sorted_start = &object_tracks[2];
   trkrInfo.vcslong_sorted_prev_track[0] = &object_tracks[3];
   trkrInfo.vcslong_sorted_next_track[0] = &object_tracks[1];

   trkrInfo.vcslong_sorted_prev_track[1] = &object_tracks[0];
   trkrInfo.vcslong_sorted_next_track[1] = NULL;

   trkrInfo.vcslong_sorted_prev_track[2] = NULL;
   trkrInfo.vcslong_sorted_next_track[2] = &object_tracks[4];

   trkrInfo.vcslong_sorted_prev_track[3] = &object_tracks[4];
   trkrInfo.vcslong_sorted_next_track[3] = &object_tracks[0];

   trkrInfo.vcslong_sorted_prev_track[4] = &object_tracks[1];
   trkrInfo.vcslong_sorted_next_track[4] = &object_tracks[3];

   // Order of object idx by priority 4,0,1,2,3
   object_tracks[0].p_lower_priority_track = &object_tracks[1];
   object_tracks[0].p_higher_priority_track = &object_tracks[4];

   object_tracks[1].p_lower_priority_track = &object_tracks[2];
   object_tracks[1].p_higher_priority_track = &object_tracks[0];

   object_tracks[2].id = 3;
   object_tracks[2].pseudo_vcs_position = { -1.0F, 11.0F };
   object_tracks[2].p_lower_priority_track = &object_tracks[3];
   object_tracks[2].p_higher_priority_track = &object_tracks[1];
   object_tracks[3].id = 4;
   object_tracks[3].pseudo_vcs_position = { 22.0F, 11.0F };
   object_tracks[3].p_lower_priority_track = NULL;
   object_tracks[3].p_higher_priority_track = &object_tracks[2];
   object_tracks[4].id = 5;
   object_tracks[4].pseudo_vcs_position = { 4.0F, 11.0F };
   object_tracks[4].p_lower_priority_track = &object_tracks[0];
   object_tracks[4].p_higher_priority_track = NULL;

   trkrInfo.p_highest_priority_track = &object_tracks[4];
   trkrInfo.p_lowest_priority_track = &object_tracks[3];

   /** \action
    * Call the function Track_Grouping()
    **/
   Track_Grouping(calib, static_env_polys, host, sensors, raw_detection_list, globals, object_tracks, detection_props, trkrInfo, timing_info);

   /** \result
    *  Check if track priority list changed as expected.
    *  Order of object idx by priority should be 4,1,2,3
    **/
   CHECK_EQUAL(object_tracks[2].id, object_tracks[1].p_lower_priority_track->id);
   CHECK_EQUAL(object_tracks[1].id, object_tracks[2].p_higher_priority_track->id);
}
/** @}*/
