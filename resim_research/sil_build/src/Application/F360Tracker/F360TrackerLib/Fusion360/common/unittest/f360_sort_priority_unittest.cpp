/** \file
 * This file contains unit tests for content of f360_sort_priority.cpp file
 */

#include "f360_sort_priority.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_sort_priority_Sort_Priority_With_New_Track
 *  @{
 */

/** \brief
 * Verify that the Sort_Priority_With_New_Track() function is behaving as expected.
 */
TEST_GROUP(f360_sort_priority_Sort_Priority_With_New_Track)
{
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];

   /** \setup
    * Set up three object tracks with different priority
    */
   TEST_SETUP()
   {
      // Clear all data in tracker info
      (void)memset(&tracker_info, 0, sizeof(tracker_info));
   }
};

/** \purpose
 * Verify that function is behaving as expected with zero active object tracks.
 * \req
 * NA
 */
TEST(f360_sort_priority_Sort_Priority_With_New_Track, No_existing_track)
{
   /** \precond
    * Object ID has been allocated, but linked priority list not updated
    */
   tracker_info.p_highest_priority_track = NULL;
   tracker_info.p_lowest_priority_track = NULL;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   object_tracks[0].id = 1;
   object_tracks[0].priority = 0.5F;

   /** \action
    * Call Sort_Priority_With_New_Track()
    */
   Sort_Priority_With_New_Track(tracker_info, &object_tracks[0]);

   /** \result
    * Verify that both highest and lowest priority pointers are referencing the active track.
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == &object_tracks[0]);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == &object_tracks[0]);
}

/** \purpose
 * Verify that function is behaving as expected with one active object tracks.
 * \req
 * NA
 */
TEST(f360_sort_priority_Sort_Priority_With_New_Track, One_existing_track)
{
   /** \precond
    * Object ID has been allocated, but linked priority list not updated
    */
   object_tracks[0].id = 1;
   object_tracks[0].priority = 0.5F;
   object_tracks[0].p_higher_priority_track = NULL;
   object_tracks[0].p_lower_priority_track = NULL;
   object_tracks[1].id = 2;
   object_tracks[1].priority = 0.6F;
   tracker_info.p_highest_priority_track = &object_tracks[0];
   tracker_info.p_lowest_priority_track = &object_tracks[0];
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   /** \action
    * Call Sort_Priority_With_New_Track()
    */
   Sort_Priority_With_New_Track(tracker_info, &object_tracks[1]);

   /** \result
    * Verify that both highest and lowest priority pointers are referencing the expected tracks.
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == &object_tracks[1]);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == &object_tracks[0]);
}

/** \purpose
 * Verify that function is behaving as expected with two active object tracks.
 * \req
 * NA
 */
TEST(f360_sort_priority_Sort_Priority_With_New_Track, Two_existing_track)
{
   /** \precond
    * Object ID has been allocated, but linked priority list not updated
    */
   object_tracks[0].id = 1;
   object_tracks[0].priority = 0.5F;
   object_tracks[0].p_higher_priority_track = &object_tracks[1];
   object_tracks[0].p_lower_priority_track = NULL;
   object_tracks[1].id = 2;
   object_tracks[1].priority = 0.7F;
   object_tracks[1].p_higher_priority_track = NULL;
   object_tracks[1].p_lower_priority_track = &object_tracks[0];
   object_tracks[2].id = 3;
   object_tracks[2].priority = 0.6F;
   tracker_info.p_highest_priority_track = &object_tracks[1];
   tracker_info.p_lowest_priority_track = &object_tracks[0];
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   /** \action
    * Call Sort_Priority_With_New_Track()
    */
   Sort_Priority_With_New_Track(tracker_info, &object_tracks[2]);

   /** \result
    * Verify that both highest and lowest priority pointers are referencing the expected tracks.
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == &object_tracks[1]);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == &object_tracks[0]);
}

/** \defgroup  f360_sort_priority_Quick_sort_track_priority
 *  @{
 */

/** \brief
 * Verify that the Quick_Sort_Track_Priority() function is behaving as expected.
 */
TEST_GROUP(f360_sort_priority_Quick_sort_track_priority)
{
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];

   /** \setup
    * Set up three object tracks with different priority
    */
   TEST_SETUP()
   {
      // Clear all data in tracker info
      (void)memset(&tracker_info, 0, sizeof(tracker_info));

      object_tracks[0].id = 1;
      object_tracks[1].id = 2;
      object_tracks[2].id = 3;

      object_tracks[0].priority = 1.0F;
      object_tracks[1].priority = 0.5F;
      object_tracks[2].priority = 0.1F;
   }
};

/** \purpose
 * Verify that function is behaving as expected with zero active object tracks.
 * \req
 * NA
 */
TEST(f360_sort_priority_Quick_sort_track_priority, Zero_objects_input)
{
   /** \precond
    * Use default test group data
    */

   /** \action
    * Call Quick_Sort_Track_Priority()
    */
   Quick_Sort_Track_Priority(tracker_info, object_tracks);

   /** \result
    * Verify that both highest and lowest priority pointers are NULL
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == NULL);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == NULL);
}

/** \purpose
 * Verify that function is behaving as expected for a single object track.
 * \req
 * NA
 */
TEST(f360_sort_priority_Quick_sort_track_priority, One_object_input)
{
   /** \precond
    * Set the tracker to process one object.
    */
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.p_highest_priority_track = &object_tracks[0];
   tracker_info.p_lowest_priority_track = &object_tracks[0];

   /** \action
    * Call Quick_Sort_Track_Priority()
    */
   Quick_Sort_Track_Priority(tracker_info, object_tracks);

   /** \result
    * Verify that the object has both highest and lowest priority
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == &object_tracks[0]);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == &object_tracks[0]);
}

/** \purpose
 * Verify that function is behaving as expected for two object tracks.
 * \req
 * NA
 */
TEST(f360_sort_priority_Quick_sort_track_priority, Two_objects_input)
{
   /** \precond
    * Set the tracker to process two objects.
    */
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.p_highest_priority_track = &object_tracks[0];
   tracker_info.p_lowest_priority_track = &object_tracks[1];

   object_tracks[0].p_higher_priority_track = NULL;
   object_tracks[0].p_lower_priority_track = &object_tracks[1];
   object_tracks[1].p_higher_priority_track = &object_tracks[0];
   object_tracks[1].p_lower_priority_track = NULL;

   /** \action
    * Call Quick_Sort_Track_Priority()
    */
   Quick_Sort_Track_Priority(tracker_info, object_tracks);

   /** \result
    * Verify that the pointers to highest and lowest priority tracks are as expected.
    * Verify that the linked relative priority list pointers for objects are as expected.
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == &object_tracks[0]);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == &object_tracks[1]);

   CHECK_TRUE(object_tracks[0].p_higher_priority_track == NULL);
   CHECK_TRUE(object_tracks[0].p_lower_priority_track == &object_tracks[1]);

   CHECK_TRUE(object_tracks[1].p_higher_priority_track == &object_tracks[0]);
   CHECK_TRUE(object_tracks[1].p_lower_priority_track == NULL);
}

/** \purpose
 * Verify that function is behaving as expected for two object tracks.
 * \req
 * NA
 */
TEST(f360_sort_priority_Quick_sort_track_priority, Three_objects_input)
{
   /** \precond
    * Set the tracker to process one object.
    */
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;
   tracker_info.p_highest_priority_track = &object_tracks[0];
   tracker_info.p_lowest_priority_track = &object_tracks[2];

   object_tracks[0].p_higher_priority_track = NULL;
   object_tracks[0].p_lower_priority_track = &object_tracks[1];
   object_tracks[1].p_higher_priority_track = &object_tracks[0];
   object_tracks[1].p_lower_priority_track = &object_tracks[2];
   object_tracks[2].p_higher_priority_track = &object_tracks[1];
   object_tracks[2].p_lower_priority_track = NULL;

   /** \action
    * Call Quick_Sort_Track_Priority()
    */
   Quick_Sort_Track_Priority(tracker_info, object_tracks);

   /** \result
    * Verify that the pointers to highest and lowest priority tracks are as expected.
    * Verify that the linked relative priority list pointers for objects are as expected.
    */
   CHECK_TRUE(tracker_info.p_highest_priority_track == &object_tracks[0]);
   CHECK_TRUE(tracker_info.p_lowest_priority_track == &object_tracks[2]);

   CHECK_TRUE(object_tracks[0].p_higher_priority_track == NULL);
   CHECK_TRUE(object_tracks[0].p_lower_priority_track == &object_tracks[1]);

   CHECK_TRUE(object_tracks[1].p_higher_priority_track == &object_tracks[0]);
   CHECK_TRUE(object_tracks[1].p_lower_priority_track == &object_tracks[2]);

   CHECK_TRUE(object_tracks[2].p_higher_priority_track == &object_tracks[1]);
   CHECK_TRUE(object_tracks[2].p_lower_priority_track == NULL);
}
/** @}*/
