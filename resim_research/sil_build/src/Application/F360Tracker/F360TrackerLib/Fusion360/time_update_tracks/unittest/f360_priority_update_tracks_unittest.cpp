/** \file
 * This file contains unit tests for content of f360_priority_update_tracks.cpp file
 */

#include "f360_priority_update_tracks.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_priority_update_tracks
 *  @{
 */

/** \brief
 * Group of tests covers Update_Track_Priority function responsible to set the object priority
 */
TEST_GROUP(f360_priority_update_tracks)
{
   /** \setup
   * Setting up two objects, host props and tracker info
   */
   F360_Tracker_Info_T tracker_info;
   F360_Host_T host_props = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};

   TEST_SETUP()
   {
      // Assign tracker info variables used in the test
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.p_highest_priority_track = &object_tracks[0];

      // Reset object states and assign object variables used in the test
      object_tracks[0].movable_prob = 1.0F;
      object_tracks[0].confidenceLevel = 1.0F;
      object_tracks[0].id = 1;
      object_tracks[0].vcs_position.x = 10.0F;
      object_tracks[0].vcs_position.y = 2.0F;
      object_tracks[0].p_lower_priority_track = &object_tracks[1];
      object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;

      object_tracks[1].movable_prob = 0.0F;
      object_tracks[1].confidenceLevel = 0.1F;
      object_tracks[1].id = 2;
      object_tracks[1].vcs_position.x = 20.0F;
      object_tracks[1].vcs_position.y = -2.0F;
      object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
   }
};

/** \purpose  
 * Test that Update_Track_Priority function only sets object priority to objects that do not have status F360_OBJECT_STATUS_INVALID
 * \req
 * NA.
 */
TEST(f360_priority_update_tracks, Update_Track_Priority_test_priority_two_objs)
{
   /** \precond
    * object status set to F360_OBJECT_STATUS_INVALID for obj2
    */
   object_tracks[1].status = F360_OBJECT_STATUS_INVALID;

   /** \action
    * Call Update_Track_Priority()
    */
   Update_Track_Priority(host_props, object_tracks, tracker_info);

   /** \result
    * Check that priority for obj1 is updated and not updated for obj2
    */
   float32_t prio_obj1 = object_tracks[0].priority;
   float32_t prio_obj2 = object_tracks[1].priority;
   CHECK_TRUE_TEXT(prio_obj1 > 0.0F, "Track priority is not updated.")
   DOUBLES_EQUAL_TEXT(prio_obj2, 0.0F, F360_EPSILON, "Track priority is incorrectly updated.")
}

/** \purpose
 * Test that sorting of tracks is working as expected.
 * \req NA
 */
TEST(f360_priority_update_tracks, Update_Track_Priority_quick_sort_track_priority)
{
   /** \precond
    * Set first object to lowest priority track
    * Set second object to highest priority track
    * Link the two objects together with lower/higher priority track pointer
    */
   tracker_info.p_lowest_priority_track = &object_tracks[0];
   tracker_info.p_highest_priority_track = &object_tracks[1];

   object_tracks[1].p_lower_priority_track = &object_tracks[0];
   object_tracks[0].p_higher_priority_track = &object_tracks[1];
   object_tracks[0].p_lower_priority_track = NULL;

   /** \action
    * Call Update_Track_Priority.
    */
   Update_Track_Priority(host_props, object_tracks, tracker_info);

   /** \result
    * Verify that the two objects changed priority.
    */
   POINTERS_EQUAL_TEXT(&object_tracks[1], object_tracks[0].p_lower_priority_track, "The first object does not point to the second object as it should.");
   POINTERS_EQUAL_TEXT(&object_tracks[0], object_tracks[1].p_higher_priority_track, "The second object does not point to the first object as it should.");
   POINTERS_EQUAL_TEXT(&object_tracks[1], tracker_info.p_lowest_priority_track, "The second object was not the lowest priority track when it should have been.");
   POINTERS_EQUAL_TEXT(&object_tracks[0], tracker_info.p_highest_priority_track, "The first object was not the highest priority track when it should have been.");
}

/** \purpose
 * Test that Quick_Sort_Track_Priority function is not called with only one active object.
 * \req
 * NA
 */
TEST(f360_priority_update_tracks, Update_Track_Priority_quick_sort_track_priority_not_called)
{
   /** \precond
    * Set tracker_info num_active_objects to 1
    * Set track_info p_highest_priority_track and p_lowest_priority_track to object with index 0
    */
   tracker_info.num_active_objs = 1;
   tracker_info.p_highest_priority_track = &object_tracks[0];
   tracker_info.p_lowest_priority_track = &object_tracks[0];

   /** \action
    * Call Update_Track_Priority()
    */
   Update_Track_Priority(host_props, object_tracks, tracker_info);

   /** \result
    * Check that tracker info pointers has not change
    */
   POINTERS_EQUAL_TEXT(&object_tracks[0], tracker_info.p_highest_priority_track, "Pointer should be pointing to object with index 0.")
   POINTERS_EQUAL_TEXT(&object_tracks[0], tracker_info.p_lowest_priority_track, "Pointer should be pointing to object with index 0.")
}

/** @}*/
