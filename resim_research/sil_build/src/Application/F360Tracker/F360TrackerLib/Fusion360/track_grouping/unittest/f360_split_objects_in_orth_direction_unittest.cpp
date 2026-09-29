/** \file
 * This file contains unit tests for content of f360_split_objects_in_orth_direction.cpp file
 */

#include "f360_split_objects_in_orth_direction.h"
#include <CppUTest/TestHarness.h>
#include "f360_set_variant.h"

#include "f360_static_env_helpers.h"
#include "f360_clear_detections_props.h"
#include "f360_clear_object_track.h"
#include "f360_allocate_id_for_initialized_object.h"

using namespace f360_variant_A;

/** \defgroup  f360_split_objects_in_orth_direction_supporter_functions
 *  @{
 */

 /** \brief
  * Test group for functions related to orthogonal split logic.
  * The test group resets commonly used variables for multiple tests.
  */
TEST_GROUP(f360_split_objects_in_orth_direction_supporter_functions)
{
   // Initialize common variables used within all tests in this test group.
   F360_Host_T host = {};
   const float32_t test_pass_thres = 0.00001F;
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Clear_Detections_Props(det_p);
      host.dist_rear_axle_to_vcs_m = 3.0F;

      globals.f_single_front_center_radar_only = false;

      for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
      }

   }
};

/** \purpose
 * Verify that function Is_Object_Valid_For_Split() returns expected boolean flag given some objects.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Is_Object_Valid_For_Split_Verify_Computation_Correctness_Medium_Wide_Gap)
{
   /** \precond
    * Setup objects such that
    * object_expected_to_split should fit criteria for split and have filter type CTCA
    * object_expected_to_split_2 should fit criteria for split and have filter type CCA
    * object_expected_to_split_3 should fit criteria for split with medium orth_gap
    * object_expected_to_not_split_1 should fit all criteria for split except for its filter type
    * object_expected_to_not_split_2 should fit all criteria for split except for its speed
    * object_expected_to_not_split_3 should fit all criteria for split except for its filtered gap
    * object_expected_to_not_split_4 should fit all criteria for split except for range_rate_diff for medium gap
    * object_expected_to_not_split_5 should not fit initial criteria for aplit
    */
   F360_Object_Track_T& object_expected_to_split = object_tracks[0];
   object_expected_to_split.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_split.speed = 1.1 * calibs.k_orth_split_min_speed;
   object_expected_to_split.orth_gap_filtered = 1.1 * calibs.k_orth_split_min_orth_gap_for_split_high;

   F360_Object_Track_T& object_expected_to_split_2 = object_tracks[1];
   object_expected_to_split_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_expected_to_split_2.speed = 1.1 * calibs.k_orth_split_min_speed;
   object_expected_to_split_2.orth_gap_filtered = 1.1 * calibs.k_orth_split_min_orth_gap_for_split_high;

   F360_Object_Track_T& object_expected_to_not_split_1 = object_tracks[2];
   object_expected_to_not_split_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_expected_to_not_split_1.speed = 1.1 * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_1.orth_gap_filtered = 1.1 * calibs.k_orth_split_min_orth_gap_for_split_high;

   F360_Object_Track_T& object_expected_to_not_split_2 = object_tracks[3];
   object_expected_to_not_split_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_2.speed = 0.9 * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_2.orth_gap_filtered = 1.1 * calibs.k_orth_split_min_orth_gap_for_split_high;

   F360_Object_Track_T& object_expected_to_split_3 = object_tracks[4];
   object_expected_to_split_3.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_split_3.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_split_3.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_medium;
   object_expected_to_split_3.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_medium_orth_gap;

   F360_Object_Track_T& object_expected_to_not_split_3 = object_tracks[5];
   object_expected_to_not_split_3.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_3.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_3.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_medium;
   object_expected_to_not_split_3.orth_range_rate_diff_filtered = 0.9F * calibs.k_orth_split_min_rr_diff_for_medium_orth_gap;
   object_expected_to_not_split_3.length_processed = 1.1F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_4 = object_tracks[6];
   object_expected_to_not_split_4.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_4.speed = 1.1 * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_4.orth_gap_filtered = 0.9 * calibs.k_orth_split_min_orth_gap_for_split_low;

   F360_Object_Track_T& object_expected_to_not_split_5 = object_tracks[2];
   object_expected_to_not_split_5.trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;
   object_expected_to_not_split_5.speed = 0.9 * calibs.k_orth_split_min_speed;

   /** \action
    * Call Is_Object_Valid_For_Split().
    */
   bool f_need_split_1 = Is_Object_Valid_For_Split(calibs, object_expected_to_split);
   bool f_need_split_2 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_1);
   bool f_need_split_3 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_2);
   bool f_need_split_4 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_3);
   bool f_need_split_5 = Is_Object_Valid_For_Split(calibs, object_expected_to_split_2);
   bool f_need_split_6 = Is_Object_Valid_For_Split(calibs, object_expected_to_split_3);
   bool f_need_split_7 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_4);
   bool f_need_split_8 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_5);

   /** \result
    * Verify that the function correctly flags for objects that should be split.
    */
   CHECK_TRUE(f_need_split_1);
   CHECK_FALSE(f_need_split_2);
   CHECK_FALSE(f_need_split_3);
   CHECK_FALSE(f_need_split_4);
   CHECK_TRUE(f_need_split_5);
   CHECK_TRUE(f_need_split_6);
   CHECK_FALSE(f_need_split_7);
   CHECK_FALSE(f_need_split_8);

}

/** \purpose
 * Verify that function Is_Object_Valid_For_Split() returns expected boolean flag given short object.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Is_Object_Valid_For_Split_Verify_Computation_Correctness_Short_Object)
{
   /** \precond
    * Setup objects such that
    * object_expected_to_split_1 should have sufficient gap, range-rate and length
    * object_expected_to_not_split_1 should have sufficient gap and range-rate but not length
    * object_expected_to_not_split_2 should have sufficient gap and length but not range-rate
    * object_expected_to_not_split_3 should have sufficient gap but not range-rate and not length
    * object_expected_to_not_split_4 should have sufficient range-rate and length but not gap
    * object_expected_to_not_split_5 should have sufficient range-rate but not length and not gap
    * object_expected_to_not_split_6 should have sufficient length but not range-rate and not gap
    */
   F360_Object_Track_T& object_expected_to_split_1 = object_tracks[0];
   object_expected_to_split_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_split_1.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_split_1.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_split_1.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_split_1.length_processed = 1.1F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_1 = object_tracks[1];
   object_expected_to_split_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_split_1.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_split_1.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_split_1.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_split_1.length_processed = 0.9F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_2 = object_tracks[1];
   object_expected_to_not_split_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_2.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_2.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_not_split_2.orth_range_rate_diff_filtered = 0.9F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_not_split_2.length_processed = 1.1F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_3 = object_tracks[1];
   object_expected_to_not_split_3.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_3.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_3.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_not_split_3.orth_range_rate_diff_filtered = 0.9F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_not_split_3.length_processed = 0.9F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_4 = object_tracks[1];
   object_expected_to_not_split_4.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_4.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_4.orth_gap_filtered = 0.9F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_not_split_4.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_not_split_4.length_processed = 1.1F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_5 = object_tracks[1];
   object_expected_to_not_split_5.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_5.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_5.orth_gap_filtered = 0.9F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_not_split_5.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_not_split_5.length_processed = 0.9F * calibs.k_orth_split_max_length_short_object;

   F360_Object_Track_T& object_expected_to_not_split_6 = object_tracks[1];
   object_expected_to_not_split_6.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_6.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_6.orth_gap_filtered = 0.9F * calibs.k_orth_split_min_orth_gap_for_split_short_objects;
   object_expected_to_not_split_6.orth_range_rate_diff_filtered = 0.9F * calibs.k_orth_split_min_rr_diff_for_short_object_orth_gap;
   object_expected_to_not_split_6.length_processed = 1.1F * calibs.k_orth_split_max_length_short_object;

   /** \action
    * Call Is_Object_Valid_For_Split().
    */
   bool f_need_split_1 = Is_Object_Valid_For_Split(calibs, object_expected_to_split_1);
   bool f_need_split_2 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_1);
   bool f_need_split_3 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_2);
   bool f_need_split_4 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_3);
   bool f_need_split_5 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_4);
   bool f_need_split_6 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_5);
   bool f_need_split_7 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_6);

   /** \result
    * Verify that the function correctly flags for objects that should be split.
    */
   CHECK_TRUE(f_need_split_1);
   CHECK_FALSE(f_need_split_2);
   CHECK_FALSE(f_need_split_3);
   CHECK_FALSE(f_need_split_4);
   CHECK_FALSE(f_need_split_5);
   CHECK_FALSE(f_need_split_6);
   CHECK_FALSE(f_need_split_7);
}

/** \purpose
 * Verify that function Is_Object_Valid_For_Split() returns expected boolean flag given some objects with low orth_gap.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Is_Object_Valid_For_Split_Verify_Computation_Correctness_Low_Gap)
{

   for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
      }

   /** \precond
    * Setup objects such that
    * object_expected_to_split_1 should fit criteria for split given low orth_gap
    * object_expected_to_not_split_1 should fit all criteria for split given low orth_gap except its distance is too low
    * object_expected_to_not_split_2 should fit all criteria for split given low orth_gap except its distance is too high
    * object_expected_to_not_split_3 should fit all criteria for split low orth_gap except its length is too high
    * object_expected_to_not_split_4 should fit criteria for split given low orth_gap except its length is too high and distance is too low
    * object_expected_to_not_split_5 should fit criteria for split given low orth_gap except its length is too high and distance is too high
    * object_expected_to_not_split_6 should not fit high rr_diff criteria for low orth_gap

    */


   F360_Object_Track_T& object_expected_to_split_1 = object_tracks[0];
   object_expected_to_split_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_split_1.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_split_1.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_split_1.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;
   object_expected_to_split_1.vcs_position.x = 1.5 * F360_Sqrtf(calibs.k_orth_split_min_distance_sq_for_low_orth_gap);
   object_expected_to_split_1.bbox.Set_Length(0.9 * calibs.k_orth_split_max_length_for_low_orth_gap);

   F360_Object_Track_T& object_expected_to_not_split_1 = object_tracks[1];
   object_expected_to_not_split_1.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_1.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_1.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_not_split_1.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;
   object_expected_to_not_split_1.vcs_position.x = 0.9 * F360_Sqrtf(calibs.k_orth_split_min_distance_sq_for_low_orth_gap);
   object_expected_to_not_split_1.bbox.Set_Length(0.9 * calibs.k_orth_split_max_length_for_low_orth_gap);

   F360_Object_Track_T& object_expected_to_not_split_2 = object_tracks[2];
   object_expected_to_not_split_2.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_2.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_2.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_not_split_2.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;
   object_expected_to_not_split_2.vcs_position.x = 1.1 * F360_Sqrtf(calibs.k_orth_split_max_distance_sq_for_low_orth_gap);
   object_expected_to_not_split_2.bbox.Set_Length(0.9 * calibs.k_orth_split_max_length_for_low_orth_gap);

   F360_Object_Track_T& object_expected_to_not_split_3 = object_tracks[3];
   object_expected_to_not_split_3.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_3.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_3.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_not_split_3.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;
   object_expected_to_not_split_3.vcs_position.x = 1.1 * F360_Sqrtf(calibs.k_orth_split_min_distance_sq_for_low_orth_gap);
   object_expected_to_not_split_3.bbox.Set_Length(1.1 * calibs.k_orth_split_max_length_for_low_orth_gap);

   F360_Object_Track_T& object_expected_to_not_split_4 = object_tracks[4];
   object_expected_to_not_split_4.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_4.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_4.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_not_split_4.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;
   object_expected_to_not_split_4.vcs_position.x = 0.9 * F360_Sqrtf(calibs.k_orth_split_min_distance_sq_for_low_orth_gap);
   object_expected_to_not_split_4.bbox.Set_Length(1.1 * calibs.k_orth_split_max_length_for_low_orth_gap);

   F360_Object_Track_T& object_expected_to_not_split_5 = object_tracks[5];
   object_expected_to_not_split_5.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_5.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_5.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_not_split_5.orth_range_rate_diff_filtered = 1.1F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;
   object_expected_to_not_split_5.vcs_position.x = 1.1 * F360_Sqrtf(calibs.k_orth_split_max_distance_sq_for_low_orth_gap);
   object_expected_to_not_split_5.bbox.Set_Length(1.1 * calibs.k_orth_split_max_length_for_low_orth_gap);

   F360_Object_Track_T& object_expected_to_not_split_6 = object_tracks[6];
   object_expected_to_not_split_6.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_not_split_6.speed = 1.1F * calibs.k_orth_split_min_speed;
   object_expected_to_not_split_6.orth_gap_filtered = 1.1F * calibs.k_orth_split_min_orth_gap_for_split_low;
   object_expected_to_not_split_6.orth_range_rate_diff_filtered = 0.9F * calibs.k_orth_split_min_rr_diff_for_low_orth_gap;

   /** \action
    * Call Is_Object_Valid_For_Split().
    */
   bool f_need_split_1 = Is_Object_Valid_For_Split(calibs, object_expected_to_split_1);
   bool f_need_split_2 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_1);
   bool f_need_split_3 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_2);
   bool f_need_split_4 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_3);
   bool f_need_split_5 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_4);
   bool f_need_split_6 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_5);
   bool f_need_split_7 = Is_Object_Valid_For_Split(calibs, object_expected_to_not_split_6);

   /** \result
    * Verify that the function correctly flags for objects that should be split.
    */
   CHECK_TRUE(f_need_split_1);
   CHECK_FALSE(f_need_split_2);
   CHECK_FALSE(f_need_split_3);
   CHECK_FALSE(f_need_split_4);
   CHECK_FALSE(f_need_split_5);
   CHECK_FALSE(f_need_split_6);
   CHECK_FALSE(f_need_split_7);
}

/** \purpose
 * Verify that function Is_Object_Valid_For_Split() returns true for RR_ERROR_BASED split
 * when object satisfies longitudinal constraint |X| < 60 m and has sufficient mean position spread.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Is_Object_Valid_For_Split_RR_Error_Based_Split_Valid_Case)
{
   for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
   {
      Clear_Object_Track(object_tracks[idx]);
      object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[idx].vcs_heading = Angle{ 0.0F };
   }

   // Setup object that should trigger RR_ERROR_BASED split:
   // - CTCA filter type and sufficient speed
   // - Longitudinal position satisfies |X| < 60 m
   // - Mean position spread above saturated width threshold
   // - Orth gap below high threshold (to avoid other split conditions)
   F360_Object_Track_T& object_expected_to_split = object_tracks[0];
   object_expected_to_split.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_expected_to_split.speed = 1.1F * calibs.k_orth_split_min_speed;

   // Longitudinal position with |X| < 60 m
   object_expected_to_split.vcs_position.x = 40.0F; // |40| < 60
   object_expected_to_split.vcs_position.y = 2.5F;

   // Set bbox width and RR error bin positions to create sufficient spread
   object_expected_to_split.bbox.Set_Width(2.0F); // saturated_width_threshold = max(1.0, 2.0) = 2.0
   object_expected_to_split.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = -2.0F;
   object_expected_to_split.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 1.5F;
   // tcs_y_pos_mean_diff = abs(-2.0 - 1.5) = 3.5 > 2.0 (saturated_width_threshold)

   // Set orth gap below high threshold to avoid other split conditions
   object_expected_to_split.orth_gap_filtered = 0.5F * calibs.k_orth_split_min_orth_gap_for_split_high;
   object_expected_to_split.orth_range_rate_diff_filtered = 0.5F * calibs.k_orth_split_min_rr_diff_for_medium_orth_gap;

   /** \action
    * Call Is_Object_Valid_For_Split().
    */
   bool f_need_split = Is_Object_Valid_For_Split(calibs, object_expected_to_split);

   /** \result
    * Verify that split is triggered and split_type is set to RR_ERROR_BASED.
    */
   CHECK_TRUE(f_need_split);
   CHECK_EQUAL(RR_ERROR_BASED, object_expected_to_split.split_type);
}

/** \purpose
 * Verify that function Is_Object_Valid_For_Split() returns false for RR_ERROR_BASED split
 * when mean position spread is below saturated width threshold (even though |X| < 60 m condition is met).
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Is_Object_Valid_For_Split_RR_Error_Based_Insufficient_Spread)
{
   for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
   {
      Clear_Object_Track(object_tracks[idx]);
      object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[idx].vcs_heading = Angle{ 0.0F };
   }

   /** \precond
    * Setup object satisfying |X| < 60 m but with insufficient mean position spread
    */
   F360_Object_Track_T& object_insufficient_spread = object_tracks[0];
   object_insufficient_spread.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_insufficient_spread.speed = 1.1F * calibs.k_orth_split_min_speed;

   // Longitudinal position within range (|X| < 60 m)
   object_insufficient_spread.vcs_position.x = 30.0F; // |30| < 60
   object_insufficient_spread.vcs_position.y = -3.0F;

   // Set insufficient spread: bbox width = 3.0, spread = 2.5 < 3.0
   object_insufficient_spread.bbox.Set_Width(3.0F); // saturated_width_threshold = max(1.0, 3.0) = 3.0
   object_insufficient_spread.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = -1.0F;
   object_insufficient_spread.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 1.5F;
   // tcs_y_pos_mean_diff = abs(-1.0 - 1.5) = 2.5 < 3.0 (saturated_width_threshold)

   // Set orth gap below high threshold
   object_insufficient_spread.orth_gap_filtered = 0.5F * calibs.k_orth_split_min_orth_gap_for_split_high;
   object_insufficient_spread.orth_range_rate_diff_filtered = 0.5F * calibs.k_orth_split_min_rr_diff_for_medium_orth_gap;

   /** \action
    * Call Is_Object_Valid_For_Split().
    */
   bool f_need_split = Is_Object_Valid_For_Split(calibs, object_insufficient_spread);

   /** \result
    * Verify that split is not triggered when spread is insufficient.
    */
   CHECK_FALSE(f_need_split);
}


/** \purpose
 * Verify that function Find_Objects_To_Split() correctly identifies objects that should be flagged for split.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Find_Objects_To_Split_Verify_Expected_Objects_To_Split_Are_Found)
{
   /** \precond
    * Setup 2 objects, 1 that should trigger a split and 1 that should not
    */
   tracker_info.num_active_objs = 2;
   uint32_t obj_idx_to_split = 2;
   uint32_t obj_idx_to_not_split = 5;

   uint32_t expected_obj_idx_flagged_for_split[NUMBER_OF_OBJECT_TRACKS] = {};
   expected_obj_idx_flagged_for_split[0] = obj_idx_to_split;
   uint32_t expected_nr_objects_to_split = 1;

   tracker_info.active_obj_ids[0] = static_cast<int32_t>(obj_idx_to_split) + 1;
   tracker_info.active_obj_ids[1] = static_cast<int32_t>(obj_idx_to_not_split) + 1;

   object_tracks[obj_idx_to_split].id = tracker_info.active_obj_ids[0];
   object_tracks[obj_idx_to_split].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[obj_idx_to_split].speed = 1.1 * calibs.k_orth_split_min_speed;
   object_tracks[obj_idx_to_split].orth_gap_filtered = 1.1 * calibs.k_orth_split_min_orth_gap_for_split_high;

   object_tracks[obj_idx_to_not_split].id = tracker_info.active_obj_ids[1];
   object_tracks[obj_idx_to_not_split].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[obj_idx_to_not_split].speed = 0.9 * calibs.k_orth_split_min_speed;
   object_tracks[obj_idx_to_not_split].orth_gap_filtered = 0.9 * calibs.k_orth_split_min_orth_gap_for_split_low;

   // Garbage data, should be overwritten and not have any impact on the function
   uint32_t nr_objects_to_split = 5U;
   uint32_t obj_idx_flagged_for_split[NUMBER_OF_OBJECT_TRACKS] = {};
   obj_idx_flagged_for_split[0] = 1000U;

   /** \action
    * Call Find_Objects_To_Split().
    */
   Find_Objects_To_Split(calibs, object_tracks, tracker_info, nr_objects_to_split, obj_idx_flagged_for_split);

   /** \result
    * Verify that the function correctly identifies the object that should be flagged for split.
    */
   CHECK_EQUAL(nr_objects_to_split, expected_nr_objects_to_split);

   for (uint32_t i = 0; i < (expected_nr_objects_to_split + 1U); i++) {
      CHECK_EQUAL(obj_idx_flagged_for_split[i], expected_obj_idx_flagged_for_split[i]);
   }
}

/** \purpose
 * Verify that function Fill_New_Object_Properties() copies object_to_split onto new_obj and correctly fills some specific fields..
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Fill_New_Object_Properties_Verify_New_Object_Is_Created_As_Expected)
{
   /** \precond
    * Properties specified in test group
    */
   F360_Object_Track_T& object_to_split = object_tracks[0];
   F360_Object_Track_T& new_obj = object_tracks[1];

   const int32_t new_obj_id = 100;
   const uint32_t new_unique_id = 1;

   // Part of data that should be copied onto new_obj
   object_to_split.raw_confidence_level = 0.99;

   // Garbage data, should be overwritten by function
   new_obj.id = 11;
   new_obj.reduced_id = 11;
   new_obj.lsc_next_in_cluster = &object_to_split; // Todo: Verify that this is a ok dummypointer
   new_obj.lsc_prev_in_cluster = &object_to_split;
   new_obj.p_higher_priority_track = &object_to_split;
   new_obj.p_lower_priority_track = &object_to_split;

   /** \action
    * Call Fill_New_Object_Properties().
    */
   Fill_New_Object_Properties(object_to_split, new_obj_id, new_unique_id, new_obj);

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */
   CHECK_EQUAL(new_obj.id, new_obj_id);
   CHECK_EQUAL(new_obj.unique_id, new_unique_id);
   CHECK_EQUAL(new_obj.reduced_id, F360_INVALID_REDUCED_ID);
   POINTERS_EQUAL(NULL, new_obj.lsc_next_in_cluster);
   POINTERS_EQUAL(NULL, new_obj.lsc_prev_in_cluster);
   POINTERS_EQUAL(NULL, new_obj.p_higher_priority_track);
   POINTERS_EQUAL(NULL, new_obj.p_lower_priority_track);

   DOUBLES_EQUAL(object_to_split.raw_confidence_level, new_obj.raw_confidence_level, test_pass_thres);
}

/** \purpose
 * Verify that function Find_Re_Associated_Detections_Ids() finds and correctly partitions detections to
 * org/new obj based on the detections position in relation to the position of object_to_split.
 * Note that this test is very similar to Re_Associate_Detections().
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Find_Re_Associated_Detections_Ids__Verify_That_Dets_Have_Been_Identified_Correctly)
{
   /** \precond
    * Setup an object and associated detections that are centered around the object.
    */
   F360_Object_Track_T& object_to_split = object_tracks[0];
   object_to_split.ndets = 4;
   object_to_split.detids[0] = 11U;
   object_to_split.detids[1] = 12U;
   object_to_split.detids[2] = 13U;
   object_to_split.detids[3] = 14U;
   object_to_split.vcs_position.x = -50.0F;
   object_to_split.vcs_position.y = 2.0F;

   // Set the ref point as CENTER and thus, vcs position of object == center position of bbox
   object_to_split.reference_point = F360_REFERENCE_POINT_CENTER;
   object_to_split.bbox.Set_Center(object_to_split.vcs_position);

   // Set detections around object_to_split
   det_p[object_to_split.detids[0] - 1].vcs_position.x = object_to_split.vcs_position.x + 1.0F; // top left
   det_p[object_to_split.detids[0] - 1].vcs_position.y = object_to_split.vcs_position.y - 1.0F;
   det_p[object_to_split.detids[1] - 1].vcs_position.x = object_to_split.vcs_position.x - 1.0F; // bottom left
   det_p[object_to_split.detids[1] - 1].vcs_position.y = object_to_split.vcs_position.y - 1.0F;
   det_p[object_to_split.detids[2] - 1].vcs_position.x = object_to_split.vcs_position.x + 1.0F; // top right
   det_p[object_to_split.detids[2] - 1].vcs_position.y = object_to_split.vcs_position.y + 1.0F;
   det_p[object_to_split.detids[3] - 1].vcs_position.x = object_to_split.vcs_position.x - 1.0F; // bottom right
   det_p[object_to_split.detids[3] - 1].vcs_position.y = object_to_split.vcs_position.y + 1.0F;

   // Garbage data, this should be reset inside function
   uint32_t nr_dets_org_obj = 111U;
   uint32_t org_obj_det_ids[MAX_DETS_IN_OBJ_TRK] = {};
   org_obj_det_ids[0] = 111U;
   uint32_t nr_dets_new_obj = 123U;
   uint32_t new_obj_det_ids[MAX_DETS_IN_OBJ_TRK] = {};
   new_obj_det_ids[0] = 123U;

   /** \action
    * Call Find_Re_Associated_Detections_Ids().
    */
   Find_Re_Associated_Detections_Ids(object_to_split, det_p, nr_dets_org_obj, org_obj_det_ids, nr_dets_new_obj, new_obj_det_ids);

   /** \result
    * Verify that detections placed to the right in orth direction gets assigned to the new object
    * and detections placed to the left in orth direction gets assigned to the original object.
    */
   CHECK_EQUAL(nr_dets_org_obj, 2U);
   CHECK_EQUAL(nr_dets_new_obj, 2U);

   CHECK_EQUAL(org_obj_det_ids[0], 11U);
   CHECK_EQUAL(org_obj_det_ids[1], 12U);
   CHECK_EQUAL(org_obj_det_ids[2], 0U);

   CHECK_EQUAL(new_obj_det_ids[0], 13U);
   CHECK_EQUAL(new_obj_det_ids[1], 14U);
   CHECK_EQUAL(new_obj_det_ids[2], 0U);
}

/** \purpose
 * Verify that function Re_Associate_Detections_Single_Object() correctly re-associates detections
 * to an object given a list of det_ids to re-associate to object.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Re_Associate_Detections_Single_Object_Check_If_Detections_Are_Re_Associated_Correctly)
{
   /** \precond
    * Setup 1 detection that should be downselected and classified as 'moving'.
    * Setup 1 detection that should be classified as 'other'.
    * Fill object with some garbage data that should be overwritten/reset by function.
    */

   F360_Object_Track_T& object = object_tracks[0];

   int32_t obj_id = 123;
   uint32_t det_id_to_downselect = 11U;
   uint32_t det_id_stationary = 21U;

   // Should trigger downselect
   det_p[det_id_to_downselect - 1].range_rate_predicted = 10.0F;
   det_p[det_id_to_downselect - 1].range_rate_dealiased = 10.0F;
   det_p[det_id_to_downselect - 1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   det_p[det_id_stationary - 1].f_close_target = true; // Should disqualify det for downselect
   det_p[det_id_stationary - 1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object.speed = 10.0F;
   object.curvature = 0.0F;
   object.id = obj_id;

   uint32_t nr_dets = 2;
   uint32_t obj_det_ids[MAX_DETS_IN_OBJ_TRK] = {};
   obj_det_ids[0] = det_id_to_downselect;
   obj_det_ids[1] = det_id_stationary;

   // Garbage data
   object.ndets = 3;
   object.num_types_of_dets[0] = 3;
   object.num_types_of_dets[1] = 3;
   object.detids[5] = 100;

   /** \action
    * Call Re_Associate_Detections_Single_Object().
    */
   Re_Associate_Detections_Single_Object(host, calibs, nr_dets, obj_det_ids, det_p, object);

   /** \result
    * Verify that the detections were associated to the object correctly.
    */
   CHECK_EQUAL(object.ndets, 2);
   CHECK_EQUAL(object.num_types_of_dets[0], 1);
   CHECK_EQUAL(object.num_types_of_dets[1], 1);
   CHECK_EQUAL(object.num_rr_inlier_dets, 1);

   CHECK_EQUAL(object.detids[0], obj_det_ids[0]);
   CHECK_EQUAL(object.detids[1], obj_det_ids[1]);
   CHECK_EQUAL(object.detids[5], object_tracks[1].detids[5]); // initial value

   CHECK_EQUAL(det_p[det_id_to_downselect - 1].object_track_id, obj_id);
   CHECK_EQUAL(det_p[det_id_stationary - 1].object_track_id, obj_id);
}

/** \purpose
 * Verify that function Re_Associate_Detections() finds and correctly partitions detections to
 * org/new obj based on the detections position in relation to the position of object_to_split.
 * Note that this test is very similar to the test for Find_Re_Associated_Detections_Ids()
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Re_Associate_Detections__Verify_That_Dets_Are_Re_Associated_Correctly_For_Both_Objects)
{
   /** \precond
    * Setup an object and associated detections that are centered around the object.
    */

   F360_Object_Track_T& object_to_split = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   object_to_split.ndets = 4;
   object_to_split.detids[0] = 11U;
   object_to_split.detids[1] = 12U;
   object_to_split.detids[2] = 13U;
   object_to_split.detids[3] = 14U;
   object_to_split.vcs_position.x = -50.0F;
   object_to_split.vcs_position.y = 2.0F;

   // Set the ref point as CENTER and thus, vcs position of object == center position of bbox
   object_to_split.reference_point = F360_REFERENCE_POINT_CENTER;
   object_to_split.bbox.Set_Center(object_to_split.vcs_position);

   // Set detections around object_to_split
   det_p[object_to_split.detids[0] - 1].vcs_position.x = object_to_split.vcs_position.x + 1.0F; // top left
   det_p[object_to_split.detids[0] - 1].vcs_position.y = object_to_split.vcs_position.y - 1.0F;
   det_p[object_to_split.detids[1] - 1].vcs_position.x = object_to_split.vcs_position.x - 1.0F; // bottom left
   det_p[object_to_split.detids[1] - 1].vcs_position.y = object_to_split.vcs_position.y - 1.0F;
   det_p[object_to_split.detids[2] - 1].vcs_position.x = object_to_split.vcs_position.x + 1.0F; // top right
   det_p[object_to_split.detids[2] - 1].vcs_position.y = object_to_split.vcs_position.y + 1.0F;
   det_p[object_to_split.detids[3] - 1].vcs_position.x = object_to_split.vcs_position.x - 1.0F; // bottom right
   det_p[object_to_split.detids[3] - 1].vcs_position.y = object_to_split.vcs_position.y + 1.0F;

   /** \action
    * Call Re_Associate_Detections().
    */
   Re_Associate_Detections(host, calibs, det_p, object_to_split, new_object);

   /** \result
    * Verify that the detections were reassigned correctly.
    */
   CHECK_EQUAL(object_to_split.ndets, 2U);
   CHECK_EQUAL(new_object.ndets, 2U);

   CHECK_EQUAL(object_to_split.detids[0], 11U);
   CHECK_EQUAL(object_to_split.detids[1], 12U);
   CHECK_EQUAL(object_to_split.detids[2], 0U);

   CHECK_EQUAL(new_object.detids[0], 13U);
   CHECK_EQUAL(new_object.detids[1], 14U);
   CHECK_EQUAL(new_object.detids[2], 0U);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly computes and populates fields for, amongst others,
 * new position and new width for objects after split.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split__Check_Properties_After_Split)
{
   /** \precond
    * Setup two objects with data.
    * Compute the expected position and width of object after function call.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   org_object.vcs_position.x = -68.0F;
   org_object.vcs_position.y = 0.0F;
   org_object.predicted_vcs_position.x = -68.0F;
   org_object.predicted_vcs_position.y = 0.0F;

   // Set the ref point as CENTER and thus, vcs center position of bbox == vcs position of object
   org_object.reference_point = F360_REFERENCE_POINT_CENTER;
   org_object.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;
   org_object.bbox.Set_Center(org_object.vcs_position);

   org_object.orth_delta_filtered = 3.0F;
   org_object.orth_gap_filtered = 6.0F;
   org_object.orth_range_rate_diff_filtered = 0.8F;

   //org_object.ref_point_lat_offset_ratio = 0.5;

   // Set the expected reference point
   F360_Reference_Point_T expected_ref_point = F360_REFERENCE_POINT_LEFT;
   F360_Reference_Point_T expected_min_proj_ref_point = F360_REFERENCE_POINT_LEFT;

   float32_t expected_vcs_pos_longitudinal = -67.1F;
   float32_t expected_vcs_pos_lateral_org_obj = -1.5F;
   float32_t expected_vcs_pos_lateral_new_obj = 1.5F;

   float32_t expected_width = 1.8F;

   org_object.bbox.Set_Length(6.0F);
   new_object.bbox.Set_Length(6.0F);
   org_object.bbox.Set_Width(2.0F);

   org_object.hdg_ptng_disagmt = 0.05F;

   org_object.filtered_pos_diff_heading = F360_PI_2;

   // Set initial RCS values to verify inheritance and updates
   org_object.maximum_rcs = -15.0F;
   org_object.average_rcs = -20.0F;
   org_object.time_since_obj_considered_veh_for_class_freeze = 5.0F;

   new_object = org_object;
   new_object.bbox.Set_Width(5.0F);

   //Set the object's f_movable flag to true, so that the derive_ref_point algorithm returns a new ref point
   org_object.movable_prob = 1.0F;
   new_object.movable_prob = 1.0F;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify that data contained in the objects are populated as expected.
    */
   CHECK_EQUAL_TEXT(expected_ref_point, org_object.reference_point, "Ref point of original obj is not as expected");
   CHECK_EQUAL_TEXT(expected_min_proj_ref_point, org_object.min_projection_reference_point , "min projection ref point of original obj is not as expected");
   CHECK_EQUAL_TEXT(expected_ref_point, new_object.reference_point, "Ref point of new obj is not as expected");
   CHECK_EQUAL_TEXT(expected_min_proj_ref_point, new_object.min_projection_reference_point , "min projection ref point of new obj is not as expected");

   DOUBLES_EQUAL(expected_vcs_pos_lateral_org_obj, org_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_lateral_new_obj, new_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_longitudinal, org_object.vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_longitudinal, new_object.vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_lateral_org_obj, org_object.predicted_vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_lateral_new_obj, new_object.predicted_vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_longitudinal, org_object.predicted_vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(expected_vcs_pos_longitudinal, new_object.predicted_vcs_position.x, test_pass_thres);

   DOUBLES_EQUAL(expected_width, org_object.bbox.Get_Width(), test_pass_thres);
   DOUBLES_EQUAL(expected_width, new_object.bbox.Get_Width(), test_pass_thres);

   // Verify RCS properties after split
   DOUBLES_EQUAL(org_object.maximum_rcs, new_object.maximum_rcs, test_pass_thres);
   DOUBLES_EQUAL(org_object.average_rcs, new_object.average_rcs, test_pass_thres);
   DOUBLES_EQUAL(-1.0F, new_object.time_since_obj_considered_veh_for_class_freeze, test_pass_thres);

   DOUBLES_EQUAL(0.0F, org_object.orth_delta_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, new_object.orth_delta_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, org_object.orth_gap_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, new_object.orth_gap_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, org_object.orth_range_rate_diff_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, new_object.orth_range_rate_diff_filtered, test_pass_thres);

   DOUBLES_EQUAL(0.00F, org_object.hdg_ptng_disagmt, test_pass_thres);
   DOUBLES_EQUAL(0.00F, new_object.hdg_ptng_disagmt, test_pass_thres);

   DOUBLES_EQUAL(INFTY, org_object.filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(INFTY, new_object.filtered_pos_diff_heading, test_pass_thres);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly computes and populates fields for width when it needs saturation.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_Saturate_Width)
{
   /** \precond
    * Setup two objects with data.
    * Compute the expected width of object after function call.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Set the ref point as CENTER and thus, vcs center position of bbox == vcs position of object
   org_object.reference_point = F360_REFERENCE_POINT_CENTER;
   org_object.bbox.Set_Center(org_object.vcs_position);

   // With orth_delta_filtered set to 5, raw width is 3m and it should be saturated.
   org_object.orth_delta_filtered = 5.0F;

   // Set the expected width
   float32_t expected_width = 1.8F;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify that data contained in the objects are populated as expected.
    */
   DOUBLES_EQUAL(expected_width, org_object.bbox.Get_Width(), test_pass_thres);
   DOUBLES_EQUAL(expected_width, new_object.bbox.Get_Width(), test_pass_thres);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly computes and populates fields for width when it doesn't need saturation.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_Dont_Saturate_Width)
{
   /** \precond
    * Setup two objects with data.
    * Compute the expected width of object after function call.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Set the ref point as CENTER and thus, vcs center position of bbox == vcs position of object
   org_object.reference_point = F360_REFERENCE_POINT_CENTER;
   org_object.bbox.Set_Center(org_object.vcs_position);

   // With orth_delta_filtered set to 2, raw width is 1.2m and it should not be saturated.
   org_object.orth_delta_filtered = 2.0F;

   // Set the expected width
   float32_t expected_width = 1.2F;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify that data contained in the objects are populated as expected.
    */
   DOUBLES_EQUAL(expected_width, org_object.bbox.Get_Width(), test_pass_thres);
   DOUBLES_EQUAL(expected_width, new_object.bbox.Get_Width(), test_pass_thres);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly computes position shifts
 * when split_type is RR_ERROR_BASED and higher RR error bin is further from bbox center.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_RR_Error_Based_Higher_Bin_Further)
{
   /** \precond
    * Setup two objects with RR_ERROR_BASED split type.
    * Set filtered mean TCS Y positions such that higher RR error bin is further from bbox center.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Set initial position and bounding box
   org_object.vcs_position.x = 50.0F;
   org_object.vcs_position.y = 0.0F;
   org_object.bbox.Set_Center(org_object.vcs_position);
   org_object.bbox.Set_Orientation(Angle{ 0.0F });

   // Set split type to RR_ERROR_BASED
   org_object.split_type = RR_ERROR_BASED;

   // Set filtered mean TCS Y positions - higher bin further from center
   org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 1.0F;   // Distance from center: 1.0
   org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -2.5F; // Distance from center: 2.5 (further)

   // Copy org_object to new_object
   new_object = org_object;

   // Expected position shifts based on the logic:
   // org_object gets shifted to the closer bin (lower RR error bin): 1.0F
   // new_object gets shifted to the further bin (higher RR error bin): -2.5F
   float32_t expected_org_obj_pos_shift = 1.0F;
   float32_t expected_new_obj_pos_shift = -2.5F;

   // Capture base Y before adaptation
   const float32_t base_y_before = org_object.vcs_position.y;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify that position shifts are computed correctly based on the logic.
    */
   const float32_t expected_org_obj_vcs_y = base_y_before + expected_org_obj_pos_shift;
   const float32_t expected_new_obj_vcs_y = base_y_before + expected_new_obj_pos_shift;

   DOUBLES_EQUAL(expected_org_obj_vcs_y, org_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_new_obj_vcs_y, new_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(std::abs(expected_org_obj_pos_shift - expected_new_obj_pos_shift), std::abs(org_object.vcs_position.y - new_object.vcs_position.y), test_pass_thres);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly computes position shifts
 * when split_type is RR_ERROR_BASED and higher RR error bin is closer to bbox center.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_RR_Error_Based_Closer_Bin_Shifts)
{
   /** \precond
    * Setup two objects with RR_ERROR_BASED split type.
    * Set filtered mean TCS Y positions such that higher RR error bin is closer to bbox center.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Set initial position and bounding box
   org_object.vcs_position.x = 50.0F;
   org_object.vcs_position.y = -6.0F;
   org_object.bbox.Set_Center(org_object.vcs_position);
   org_object.bbox.Set_Orientation(Angle{ 0.0F });

   // Set split type to RR_ERROR_BASED
   org_object.split_type = RR_ERROR_BASED;

   // Set filtered mean TCS Y positions - higher bin closer to center
   org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = -2.0F; // Distance from center: 2.0
   org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = 0.5F;  // Distance from center: 0.5 (closer)

   // Copy org_object to new_object
   new_object = org_object;

   // Expected position shifts based on the logic:
   // org_object gets shifted to the closer bin (higher RR error bin): 0.5F
   // new_object gets shifted to the further bin (lower RR error bin): -2.0F
   float32_t expected_org_obj_pos_shift = 0.5F;
   float32_t expected_new_obj_pos_shift = -2.0F;

   // Capture base Y before adaptation
   const float32_t base_y_before = org_object.vcs_position.y;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify that position shifts are computed correctly based on RR error bins.
    */
   const float32_t expected_org_obj_vcs_y = base_y_before + expected_org_obj_pos_shift;  // -5.5F
   const float32_t expected_new_obj_vcs_y = base_y_before + expected_new_obj_pos_shift;  // -8.0F

   DOUBLES_EQUAL(expected_org_obj_vcs_y, org_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_new_obj_vcs_y, new_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(std::abs(expected_org_obj_pos_shift - expected_new_obj_pos_shift), std::abs(org_object.vcs_position.y - new_object.vcs_position.y), test_pass_thres);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly handles edge case
 * when both RR error bins have equal distance from bbox center in RR_ERROR_BASED split.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_RR_Error_Based_Equal_Distance)
{
   /** \precond
    * Setup two objects with RR_ERROR_BASED split type.
    * Set filtered mean TCS Y positions such that both bins are equidistant from bbox center.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Set initial position and bounding box
   org_object.vcs_position.x = 40.0F;
   org_object.vcs_position.y = 7.0F;
   org_object.bbox.Set_Center(org_object.vcs_position);
   org_object.bbox.Set_Orientation(Angle{ 0.0F });

   // Set split type to RR_ERROR_BASED
   org_object.split_type = RR_ERROR_BASED;

   // Set filtered mean TCS Y positions - equal distance from center
   org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 2.0F;   // Distance from center: 2.0
   org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -2.0F; // Distance from center: 2.0 (equal)

   // Copy org_object to new_object
   new_object = org_object;

   // Expected position shifts when distances are equal:
   // The condition f_higher_bin_mean_further_to_bbox_center_y will be false (since 2.0 > 2.0 is false)
   // org_object gets shifted to higher RR error bin: -2.0F
   // new_object gets shifted to lower RR error bin: 2.0F
   float32_t expected_org_obj_pos_shift = -2.0F;
   float32_t expected_new_obj_pos_shift = 2.0F;

   // Capture base Y prior to adaptation
   const float32_t base_y_before = org_object.vcs_position.y;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify that position shifts are computed correctly when bins are equidistant.
    */
   const float32_t expected_org_obj_vcs_y = base_y_before + expected_org_obj_pos_shift;  // 5.0F
   const float32_t expected_new_obj_vcs_y = base_y_before + expected_new_obj_pos_shift;  // 9.0F

   DOUBLES_EQUAL(expected_org_obj_vcs_y, org_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_new_obj_vcs_y, new_object.vcs_position.y, test_pass_thres);
   // Distance should equal difference in shifts
   DOUBLES_EQUAL(std::abs(expected_org_obj_pos_shift - expected_new_obj_pos_shift), std::abs(org_object.vcs_position.y - new_object.vcs_position.y), test_pass_thres);
}


/** @}*/


/**\defgroup  f360_split_single_object_in_ortho_direction
* @{
*/

/** \brief
 * Test group for tests related to function Split_Single_Object_In_Ortho_Direction()
 */
TEST_GROUP(f360_split_single_object_in_ortho_direction)
{
   // Initialize common variables used within all tests in this test group.
   F360_Host_T host = {};
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   float32_t test_pass_thres = 0.0001F;

   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Clear_Detections_Props(det_p);
      Set_Tracker_Variant(tracker_info.variant);
      host.dist_rear_axle_to_vcs_m = 3.0F;

      globals.f_single_front_center_radar_only = false;

      // Create an arbitrary object that shall be splitted
      objects[0].id = 1;
      objects[0].unique_id = 1U;
      objects[0].vcs_position.x = 10.0F;
      objects[0].vcs_position.y = 10.0F;
      objects[0].speed = 10.0F;
      objects[0].Set_Bbox_Orientation(Angle{ 0.0F });
      objects[0].curvature = 1.0F;
      objects[0].vcs_heading = Angle{ 0.0F };
      objects[0].priority = 0.5F;
      objects[0].p_higher_priority_track = NULL;
      objects[0].p_lower_priority_track = NULL;
      objects[0].orth_delta_filtered = 3.0F;
      objects[0].filtered_pos_diff_heading = F360_PI_2;
      objects[0].ndets = 2;
      objects[0].detids[0] = 1;
      objects[0].detids[1] = 2;
      objects[0].movable_prob = 1.0F;
      objects[0].Update_Bbox_Size(0.0F, 2.0F);

      // Fill detections
      det_p[0].vcs_position.x = objects[0].vcs_position.x;
      det_p[0].vcs_position.y = objects[0].vcs_position.y - 1.0F;
      det_p[0].object_track_id = 1;
      det_p[1].vcs_position.x = objects[0].vcs_position.x;
      det_p[1].vcs_position.y = objects[0].vcs_position.y + 1.0F;
      det_p[1].object_track_id = 1;

      // Fill tracker info
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = objects[0].id;
      tracker_info.inactive_obj_ids[0] = 2;
      tracker_info.num_unique_objs = 1U;
      tracker_info.vcslong_sorted_start = &objects[0];
      tracker_info.vcslong_sorted_next_track[0] = NULL;
      tracker_info.vcslong_sorted_prev_track[0] = NULL;
      tracker_info.p_highest_priority_track = &objects[0];
      tracker_info.p_lowest_priority_track = &objects[0];
   }
};

/** \purpose
* Verify that function that splits a single object works as intended.
* \req
* NA.
*/
TEST(f360_split_single_object_in_ortho_direction, Split_Single_Object_In_Ortho_Direction__Test_Object_Split)
{
   /** \precond
    * In test group:
    * Object have been set up with valid infomation for a split
    * Object is added to tracker info
    * Two detections are associated to the object
    * Object have one historic detection associated
    */
   F360_Object_Track_T& object_to_split = objects[0];

   /** \action
    * Call Split_Single_Object_In_Ortho_Direction().
    */
   Split_Single_Object_In_Ortho_Direction(
      host,
      calibs,
      sensors,
      globals,
      object_to_split,
      objects,
      tracker_info,
      det_p);

   /** \result
    * Verify that new uniqe ids have been assigned correctly to both objects
    * Verify that some arbitrary signal (speed) have been inherited from the splitted object
    * Verify that heading/pointing is updated for both objects
    * Verify curvature have been reset for both objects
    * Verify that detections have been associated correctly
    * Verify that split signals have been reset correctly
    * Verify that new object have been inserted in the vcs-longitudinal sorted list
    * Verify that new object have been inserted in the priority sorted list
    * Verify that objects have shifted in position as expected
    */
   CHECK_EQUAL(1U, objects[0].unique_id);
   CHECK_EQUAL(2U, objects[1].unique_id);

   DOUBLES_EQUAL(objects[0].speed, objects[1].speed, test_pass_thres);

   DOUBLES_EQUAL(F360_PI_2, objects[0].bbox.Get_Orientation().Value(), test_pass_thres);
   DOUBLES_EQUAL(F360_PI_2, objects[0].vcs_heading.Value(), test_pass_thres);
   DOUBLES_EQUAL(F360_PI_2, objects[1].bbox.Get_Orientation().Value(), test_pass_thres);
   DOUBLES_EQUAL(F360_PI_2, objects[1].vcs_heading.Value(), test_pass_thres);

   DOUBLES_EQUAL(0.0F, objects[0].curvature, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[1].curvature, test_pass_thres);

   CHECK_EQUAL(1, objects[0].ndets);
   CHECK_EQUAL(1, objects[0].detids[0]);
   CHECK_EQUAL(objects[0].id, det_p[0].object_track_id);
   CHECK_EQUAL(1, objects[1].ndets);
   CHECK_EQUAL(2, objects[1].detids[0]);
   CHECK_EQUAL(objects[1].id, det_p[1].object_track_id);

   DOUBLES_EQUAL(0.0F, objects[0].orth_delta_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[0].orth_gap_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[0].orth_range_rate_diff_filtered, test_pass_thres);
   DOUBLES_EQUAL(INFTY, objects[0].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[1].orth_delta_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[1].orth_gap_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[1].orth_range_rate_diff_filtered, test_pass_thres);
   DOUBLES_EQUAL(INFTY, objects[1].filtered_pos_diff_heading, test_pass_thres);

   POINTERS_EQUAL(&objects[1], tracker_info.vcslong_sorted_next_track[0]);
   POINTERS_EQUAL(&objects[0], tracker_info.vcslong_sorted_prev_track[1]);

   POINTERS_EQUAL(&objects[1], objects[0].p_lower_priority_track);
   POINTERS_EQUAL(NULL, objects[0].p_higher_priority_track);
   POINTERS_EQUAL(NULL, objects[1].p_lower_priority_track);
   POINTERS_EQUAL(&objects[0], objects[1].p_higher_priority_track);

   DOUBLES_EQUAL(10.0F, objects[0].vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(8.5F, objects[0].vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(10.0F, objects[1].vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(11.5F, objects[1].vcs_position.y, test_pass_thres);
}

/** @}*/

TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_RR_Error_Based_Mean_Centered_Shifts)
{
   // Precond: Create org/new objects with RR_ERROR_BASED split where updated_width >= 1.1 * |lower - higher| so else-branch triggers.
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Reset any previous state that earlier tests might have left (only the fields we rely on)
   org_object.vcs_position.x = 30.0F;
   org_object.vcs_position.y = 3.5F;
   org_object.bbox.Set_Center(org_object.vcs_position);
   org_object.bbox.Set_Orientation(Angle{0.0F});
   org_object.split_type = RR_ERROR_BASED;

   // Choose filtered mean bin positions with small spread so that diff is SMALL and updated_width (saturated) becomes >= 1.1*diff.
   // diff = |0.5F - (-0.5F)| = 1.0F
   org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.5F;
   org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -0.5F;

   // Make orth_delta_filtered large so updated_width saturates at threshold and exceeds 1.1 * diff
   // We rely on calibrations: updated_width = min(0.5 * orth_delta_filtered * gain, threshold)
   // Set orth_delta large enough to saturate updated_width == k_orth_split_width_threshold (assumed >= 3.0F here)
   org_object.orth_delta_filtered = 100.0F; // large to ensure saturation

   // Copy to new object
   new_object = org_object;

   // Capture original base Y before adaptation
   const float32_t base_y_before = org_object.vcs_position.y;

   // Action
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   // Result: else branch => shifts are mean +/- 0.7 * updated_width
   const float32_t lower_bin = 0.5F;
   const float32_t higher_bin = -0.5F;
   const float32_t mean_bin_pos = (lower_bin + higher_bin) * 0.5F; // 0.0F
   const float32_t updated_width = new_object.bbox.Get_Width(); // after adaptation
   const float32_t expected_org_shift = mean_bin_pos + 0.7F * updated_width; // positive shift
   const float32_t expected_new_shift = mean_bin_pos - 0.7F * updated_width; // negative shift

   const float32_t expected_org_y = base_y_before + expected_org_shift;
   const float32_t expected_new_y = base_y_before + expected_new_shift;

   DOUBLES_EQUAL(expected_org_y, org_object.vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(expected_new_y, new_object.vcs_position.y, test_pass_thres);

   // Sanity: ensure else path used (distance should be 1.4 * updated_width)
   DOUBLES_EQUAL(1.4F * updated_width, std::abs(org_object.vcs_position.y - new_object.vcs_position.y), test_pass_thres);

   // Additional sanity: updated_width must be >= 1.1 * diff for this branch
   const float32_t diff = std::abs(lower_bin - higher_bin); // 1.0F
   CHECK_TRUE(updated_width >= 1.1F * diff);
}

/** \purpose
 * Verify that function Adapt_Objects_Properties_After_Orth_Split() correctly handles
 * RCS property inheritance and reset behavior when splitting objects.
 * \req
 * NA.
 */
TEST(f360_split_objects_in_orth_direction_supporter_functions, Adapt_Objects_Properties_After_Orth_Split_RCS_Inheritance_And_Reset)
{
   /** \precond
    * Setup two objects with RR_ERROR_BASED split type and specific RCS values.
    */
   F360_Object_Track_T& org_object = object_tracks[0];
   F360_Object_Track_T& new_object = object_tracks[1];

   // Set initial position and bounding box - ensure |x| < 60 for validity
   org_object.vcs_position.x = 45.0F;
   org_object.vcs_position.y = 2.0F;
   org_object.bbox.Set_Center(org_object.vcs_position);
   org_object.bbox.Set_Orientation(Angle{ 0.0F });

   // Set split type to RR_ERROR_BASED
   org_object.split_type = RR_ERROR_BASED;

   // Set filtered mean TCS Y positions for valid split
   org_object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 1.0F;
   org_object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -1.0F;

   // Set specific RCS values to test inheritance
   org_object.maximum_rcs = -12.5F;
   org_object.average_rcs = -18.3F;
   org_object.time_since_obj_considered_veh_for_class_freeze = 5.7F;

   // Copy org_object to new_object
   new_object = org_object;

   /** \action
    * Call Adapt_Objects_Properties_After_Orth_Split().
    */
   Adapt_Objects_Properties_After_Orth_Split(host, calibs, sensors, globals, org_object, new_object);

   /** \result
    * Verify RCS properties are correctly updated and time_since_obj_considered_veh_for_class_freeze is reset for new object.
    */
   // Both objects should have the same RCS values after the RCS update functions are called
   // The actual values depend on the detection content and the RCS update algorithms
   DOUBLES_EQUAL(org_object.maximum_rcs, new_object.maximum_rcs, test_pass_thres);
   DOUBLES_EQUAL(org_object.average_rcs, new_object.average_rcs, test_pass_thres);

   // Original object should keep its time_since_obj_considered_veh_for_class_freeze
   DOUBLES_EQUAL(5.7F, org_object.time_since_obj_considered_veh_for_class_freeze, test_pass_thres);

   // New object should have time_since_obj_considered_veh_for_class_freeze reset to -1.0F
   DOUBLES_EQUAL(-1.0F, new_object.time_since_obj_considered_veh_for_class_freeze, test_pass_thres);
}

/**\defgroup  f360_split_objects_in_orth_direction
* @{
*/

/** \brief
 * Test group for tests related to function Split_Objects_In_Orth_Direction()
 */
TEST_GROUP(f360_split_objects_in_orth_direction)
{
   // Initialize common variables used within all tests in this test group.
   F360_Host_T host = {};
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS];
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   float32_t test_pass_thres = 0.0001F;

   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Clear_Detections_Props(det_p);
      Set_Tracker_Variant(tracker_info.variant);

      globals.f_single_front_center_radar_only = false;

      // Set distance to rear axle for host
      host.dist_rear_axle_to_vcs_m = 3.0F;

      for (uint32_t idx = 0U; idx < F360_NUM_OF_STATIC_ENV_POLYS; idx++)
      {
         Reset_Single_Static_Env_Poly(static_env_polys[idx]);
      }

      // Create an arbitrary object that shall be splitted
      objects[0].id = 1;
      objects[0].unique_id = 10U;
      objects[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      objects[0].vcs_position.x = 10.0F;
      objects[0].vcs_position.y = 10.0F;
      objects[0].speed = calibs.k_orth_split_min_speed + 1.0F;
      objects[0].Set_Bbox_Orientation(Angle{ 0.0F });
      objects[0].curvature = 1.0F;
      objects[0].vcs_heading = Angle{ 0.0F };
      objects[0].priority = 0.5F;
      objects[0].p_higher_priority_track = NULL;
      objects[0].p_lower_priority_track = &objects[1];
      objects[0].orth_delta_filtered = 5.0F;
      objects[0].orth_gap_filtered = calibs.k_orth_split_min_orth_gap_for_split_high + 1.0F;
      objects[0].filtered_pos_diff_heading = F360_PI_2;
      objects[0].ndets = 2;
      objects[0].detids[0] = 1;
      objects[0].detids[1] = 2;
      objects[0].movable_prob = 1.0F;
      objects[0].Update_Bbox_Size(0.0F, 2.0F);
      objects[0].time_since_split = -1.0F;

      // Create another object that shall not be split.
      // Set time since split above threshold for resetting
      objects[1].id = 2;
      objects[1].unique_id = 11U;
      objects[1].vcs_position.x = -10.0F;
      objects[1].vcs_position.y = -10.0F;
      objects[1].priority = 0.4F;
      objects[1].p_higher_priority_track = &objects[0];
      objects[1].p_lower_priority_track = NULL;
      objects[1].orth_gap_filtered = 0.0F;
      objects[1].time_since_split = 5.01F;

      // Fill detections to object with id 1
      det_p[0].vcs_position.x = objects[0].vcs_position.x;
      det_p[0].vcs_position.y = objects[0].vcs_position.y - 1.0F;
      det_p[0].object_track_id = 1;
      det_p[1].vcs_position.x = objects[0].vcs_position.x;
      det_p[1].vcs_position.y = objects[0].vcs_position.y + 1.0F;
      det_p[1].object_track_id = 1;

      // Fill tracker info
      tracker_info.num_active_objs = 2;
      tracker_info.num_unique_objs = 11U;
      tracker_info.active_obj_ids[0] = objects[0].id;
      tracker_info.active_obj_ids[1] = objects[1].id;
      tracker_info.inactive_obj_ids[0] = 3;
      tracker_info.vcslong_sorted_start = &objects[1];
      tracker_info.vcslong_sorted_next_track[0] = &objects[0];
      tracker_info.vcslong_sorted_prev_track[0] = NULL;
      tracker_info.vcslong_sorted_next_track[1] = NULL;
      tracker_info.vcslong_sorted_prev_track[1] = &objects[1];
      tracker_info.p_highest_priority_track = &objects[0];
      tracker_info.p_lowest_priority_track = &objects[1];

      // Create a valid SEP
      static_env_polys[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      static_env_polys[0].poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;
      static_env_polys[0].age = 0;
      static_env_polys[0].lower_limit = -100.0F;
      static_env_polys[0].upper_limit = 100.0F;
      static_env_polys[0].p2 = 0.0F;
      static_env_polys[0].p1 = 0.0F;
      static_env_polys[0].p0 = objects[0].vcs_position.y;
   }
};

/** \purpose
* Verify that function that splits a single object works as intended.
* \req
* NA.
*/
TEST(f360_split_objects_in_orth_direction, Split_Single_Object_In_Ortho_Direction__Test_Object_Split_With_Active_SEP)
{
   /** \precond
    * Same as test setup
    */

   /** \action
    * Call Split_Objects_In_Orth_Direction().
    */
   Split_Objects_In_Orth_Direction(
      host,
      calibs,
      static_env_polys,
      sensors,
      globals,
      objects,
      tracker_info,
      det_p);

   /** \result
    * Verify that new uniqe ids have been assigned correctly
    * Verify that some arbitrary signal (speed) have been inherited from the splitted object
    * Verify that heading/pointing is updated for both objects
    * Verify curvature have been reset for both objects
    * Verify that detections have been associated correctly
    * Verify that split signals have been reset correctly
    * Verify that new object have been inserted in the vcs-longitudinal sorted list
    * Verify that new object have been inserted in the priority sorted list
    * Verify that objects have shifted in position as expected
    * Verify that on/behind SEP id have been updated
    * Verify that the objects that have been split have their time_since_split timers started (set >= 0)
    * Verify that the object that doesn't split and has a time_since_split above threshold has its timer reset (set to -1)
    */
   CHECK_EQUAL(10U, objects[0].unique_id);
   CHECK_EQUAL(12U, objects[2].unique_id);

   DOUBLES_EQUAL(objects[0].speed, objects[2].speed, test_pass_thres);

   DOUBLES_EQUAL(F360_PI_2, objects[0].bbox.Get_Orientation().Value(), test_pass_thres);
   DOUBLES_EQUAL(F360_PI_2, objects[0].vcs_heading.Value(), test_pass_thres);
   DOUBLES_EQUAL(F360_PI_2, objects[2].bbox.Get_Orientation().Value(), test_pass_thres);
   DOUBLES_EQUAL(F360_PI_2, objects[2].vcs_heading.Value(), test_pass_thres);

   DOUBLES_EQUAL(0.0F, objects[0].curvature, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[2].curvature, test_pass_thres);

   CHECK_EQUAL(1, objects[0].ndets);
   CHECK_EQUAL(1, objects[0].detids[0]);
   CHECK_EQUAL(objects[0].id, det_p[0].object_track_id);
   CHECK_EQUAL(1, objects[2].ndets);
   CHECK_EQUAL(2, objects[2].detids[0]);
   CHECK_EQUAL(objects[2].id, det_p[1].object_track_id);

   DOUBLES_EQUAL(0.0F, objects[0].orth_delta_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[0].orth_gap_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[0].orth_range_rate_diff_filtered, test_pass_thres);
   DOUBLES_EQUAL(INFTY, objects[0].filtered_pos_diff_heading, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[2].orth_delta_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[2].orth_gap_filtered, test_pass_thres);
   DOUBLES_EQUAL(0.0F, objects[2].orth_range_rate_diff_filtered, test_pass_thres);
   DOUBLES_EQUAL(INFTY, objects[2].filtered_pos_diff_heading, test_pass_thres);

   POINTERS_EQUAL(&objects[0], tracker_info.vcslong_sorted_next_track[0]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_prev_track[0]);
   POINTERS_EQUAL(&objects[2], tracker_info.vcslong_sorted_next_track[1]);
   POINTERS_EQUAL(&objects[1], tracker_info.vcslong_sorted_prev_track[1]);
   POINTERS_EQUAL(NULL, tracker_info.vcslong_sorted_next_track[2]);
   POINTERS_EQUAL(&objects[1], tracker_info.vcslong_sorted_prev_track[2]);

   POINTERS_EQUAL(&objects[2], objects[0].p_lower_priority_track);
   POINTERS_EQUAL(NULL, objects[0].p_higher_priority_track);
   POINTERS_EQUAL(NULL, objects[1].p_lower_priority_track);
   POINTERS_EQUAL(&objects[2], objects[1].p_higher_priority_track);
   POINTERS_EQUAL(&objects[0], objects[2].p_higher_priority_track);
   POINTERS_EQUAL(&objects[1], objects[2].p_lower_priority_track);

   DOUBLES_EQUAL(10.0F, objects[0].vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(7.5F, objects[0].vcs_position.y, test_pass_thres);
   DOUBLES_EQUAL(10.0F, objects[2].vcs_position.x, test_pass_thres);
   DOUBLES_EQUAL(12.5F, objects[2].vcs_position.y, test_pass_thres);

   CHECK_EQUAL(objects[0].behind_sep_id, F360_INVALID_UNSIGNED_ID);
   CHECK_EQUAL(objects[0].on_sep_id, F360_INVALID_UNSIGNED_ID);
   CHECK_EQUAL(objects[2].behind_sep_id, 1);
   CHECK_EQUAL(objects[2].on_sep_id, F360_INVALID_UNSIGNED_ID);

   CHECK_TRUE(objects[0].time_since_split >= 0.0F);
   CHECK_TRUE(objects[2].time_since_split >= 0.0F);
   CHECK_TRUE(objects[1].time_since_split < 0.0F);

}

/** @}*/
