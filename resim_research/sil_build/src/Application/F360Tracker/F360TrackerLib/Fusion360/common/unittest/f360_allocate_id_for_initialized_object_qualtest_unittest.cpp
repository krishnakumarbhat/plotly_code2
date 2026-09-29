/** \file
  File with set of qualification tests (which are also unit test) for Allocate_Id_For_Initialized_Object function.
*/

#include "f360_allocate_id_for_initialized_object.h"
#include "f360_set_variant.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_allocate_id_for_initialized_object
 *  @{
 */

/** \brief
 *  Add brief description of test group
 */

using namespace f360_variant_A;

TEST_GROUP(f360_allocate_id_for_initialized_object)
{
   /** \setup
   * Initialize structures which are inputs of allocate_id_for_initialized_object
   */
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];

   TEST_SETUP()
   {
      tracker_info = {};
      Set_Tracker_Variant(tracker_info.variant);
   }

   /** \teardown
    * Nothing to teardown in this test group
    */
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Test case where new initialized object is first ever active object, there is no other active tracks. 
*\req    CPR-3794
*/
TEST(f360_allocate_id_for_initialized_object, no_active_objects)
{
   /** \precond
    * Tracker info array inactive_obj_ids contains all IDs. 
    */
   for (uint32_t index = 0; index < NUMBER_OF_OBJECT_TRACKS; index++)
   {
      tracker_info.inactive_obj_ids[index] = index + 1;
   }
   /** \action
    * Call function Allocate_Id_For_Initialized_Object
    */
   int32_t new_id = Allocate_Id_For_Initialized_Object(tracker_info, object_tracks, det_props);

   /** \result
    * New ID should be equal to 1. Also check if now tracker_info.inactive_obj_ids first value is equal 2 (next free ID),
    and check if now tracker_info.active_obj_ids first value is equal 1. 
    */
   int32_t expected_id = 1;
   CHECK_EQUAL_TEXT(expected_id, new_id, "Expected ID is different than Allocate_Id_For_Initialized_Object function output");
   CHECK_EQUAL_TEXT(2, tracker_info.inactive_obj_ids[0], "Tracker info inactive object ids first value shold be 2, but is not");
   CHECK_EQUAL_TEXT(expected_id, tracker_info.active_obj_ids[0], "Tracker info active object IDs first value is different than expected ID");
}

/**
*\purpose  Test case where new initialized object is second ever active object, there is one other active tracks.
*\req    CPR-3794
*/
TEST(f360_allocate_id_for_initialized_object, single_active_object)
{
   /** \precond
   * Tracker info array inactive_obj_ids contains all IDs except 1. Set tracker_info.active_obj_ids first value to be 1 and num_active_objs to 1.
   */
   for (uint32_t index = 0; index < NUMBER_OF_OBJECT_TRACKS - 1; index++)
   {
      tracker_info.inactive_obj_ids[index] = index + 2;
   }
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.num_active_objs = 1;

   /** \action
   * Call function Allocate_Id_For_Initialized_Object
   */
   int32_t new_id = Allocate_Id_For_Initialized_Object(tracker_info, object_tracks, det_props);

   /** \result
   * New ID should be equal to 2. Also check if now tracker_info.inactive_obj_ids first value is equal 3 (next free ID),
   and check if now tracker_info.active_obj_ids second value is equal 2.
   */
   int32_t expected_id = 2;
   CHECK_EQUAL_TEXT(expected_id, new_id, "Expected ID is different than Allocate_Id_For_Initialized_Object function output");
   CHECK_EQUAL_TEXT(3, tracker_info.inactive_obj_ids[0], "Tracker info inactive object ids first value shold be 2, but is not");
   CHECK_EQUAL_TEXT(expected_id, tracker_info.active_obj_ids[1], "Tracker info active object IDs second value is different than expected ID");
}

/**
*\purpose  Test case where number of objects is staurated.
*\req    CPR-3794
*/
TEST(f360_allocate_id_for_initialized_object, objects_saturation)
{
   /** \precond
   * Tracker info array active_obj_ids is full. Set tracker_info.num_active_objs to maximum value. Initialize lowest priority track with ID 10
   and set tracker info pointer on this object.
   */
   for (uint32_t index = 0; index < NUMBER_OF_OBJECT_TRACKS; index++)
   {
      tracker_info.active_obj_ids[index] = index + 1;
   }
   tracker_info.num_active_objs = NUMBER_OF_OBJECT_TRACKS;
   F360_Object_Track_T lowest_prio_obj_track = {};
   lowest_prio_obj_track.id = 10;
   tracker_info.p_lowest_priority_track = &lowest_prio_obj_track;

   /** \action
   * Call function Allocate_Id_For_Initialized_Object
   */
   int32_t new_id = Allocate_Id_For_Initialized_Object(tracker_info, object_tracks, det_props);

   /** \result
   * New ID should be equal to 2. Also check if now tracker_info.inactive_obj_ids first value is equal 3 (next free ID),
   and check if now tracker_info.active_obj_ids second value is equal 2.
   */
   int32_t expected_id = 10;
   CHECK_EQUAL_TEXT(expected_id, new_id, "Expected ID is different than Allocate_Id_For_Initialized_Object function output");

   int32_t exp_inactive_obj_ids[NUMBER_OF_OBJECT_TRACKS] = {};
   MEMCMP_EQUAL(exp_inactive_obj_ids, tracker_info.inactive_obj_ids, NUMBER_OF_OBJECT_TRACKS); //If expected inactive_obj_ids does not contain any free indexes
}
/** @}*/
