/** \file
   This file contains qualification tests for f360_split_objects_in_orth_direction.cpp
*/

#include "f360_split_objects_in_orth_direction.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

#include "f360_static_env_helpers.h"
#include "f360_clear_object_track.h"
#include "f360_set_variant.h"

/** \defgroup  f360_split_objects_in_orth_direction_qualtest
 *  @{
 */
using namespace f360_variant_A;
 /** \brief
 *  This purpose of this test group is for testing the functionality
 *  of the main function in f360_split_objects_in_orth_direction.cpp file.
 **/
TEST_GROUP(f360_split_objects_in_orth_direction_qualtest)
{

   //Initialize common variables used within all tests in this test group.
   F360_Host_T host = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calib = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   /** \setup
    * Initialize common variable used in tests.
    * Set tracker info to have 1 active object
    * Initialize an object that should trigger a split.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);
      host.dist_rear_axle_to_vcs_m = 3.0F;

      globals.f_single_front_center_radar_only = false;
      
      for (uint32_t idx = 0U; idx < F360_NUM_OF_STATIC_ENV_POLYS; idx++)
      {
         Reset_Single_Static_Env_Poly(static_env_polys[idx]);
      }

      for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
      }

      object_tracks[20].id = 21;
      object_tracks[20].speed = calib.k_orth_split_min_speed + 1.0F;
      object_tracks[20].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[20].orth_gap_filtered = calib.k_orth_split_min_orth_gap_for_split_high + 1.0F;
      object_tracks[20].orth_delta_filtered = object_tracks[20].orth_gap_filtered + 1.0F;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 21;
      tracker_info.inactive_obj_ids[0] = 41;
      tracker_info.p_highest_priority_track = NULL;

      tracker_info.vcslong_sorted_start = &(object_tracks[20]);
      tracker_info.vcslong_sorted_prev_track[20] = NULL;
      tracker_info.vcslong_sorted_next_track[20] = NULL;

   }
};

/** \purpose
 *  Test checks that the function split an object in its orthogonal direction
 *  when the object have aggregated a large gap between its associated detections
 *  in the objects orthogonal direction.
 * \req CPR-3782
 */
TEST(f360_split_objects_in_orth_direction_qualtest, Verify_Split_Logic)
{
   /** \precond
    */

   /** \action
    *  call main function in f360_split_objects_in_orth_direction.cpp; Split_Objects_In_Orth_Direction().
    */
   Split_Objects_In_Orth_Direction(
         host,
         calib,
         static_env_polys,
         sensors,
         globals,
         object_tracks,
         tracker_info,
         detection_props);

   /** \result
    *  Verify that split happened by checking that the number of
    *  active objects have increased by 1
    */
   CHECK_EQUAL(2, tracker_info.num_active_objs);
}

/** @}*/

