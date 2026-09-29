/** \file
 * This file contains unit tests for content of f360_kill_obj_trk.cpp file
 */

#include "f360_kill_obj_trk.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_kill_obj_trk_and_clear_assoc_det_obj_props
 *  @{
**/

/** \brief
*  This group includes testing of the functionality of Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props.
**/
TEST_GROUP(f360_kill_obj_trk_and_clear_assoc_det_obj_props)
{
   /** \setup
   * An object is set up with two associated detections
   **/
   int32_t obj_trk_id;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t det_id_1;
   uint32_t det_id_2;
   TEST_SETUP()
   {
      obj_trk_id = 4;
      object_tracks[obj_trk_id - 1].id = obj_trk_id;
      object_tracks[obj_trk_id - 1].ndets = 2;
      det_id_1 = 5U;
      det_id_2 = 10U;
      object_tracks[obj_trk_id - 1].detids[0U] = det_id_1;
      object_tracks[obj_trk_id - 1].detids[1U] = det_id_2;

      det_props[det_id_1 - 1].object_track_id = obj_trk_id;
      det_props[det_id_1 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_2 - 1].object_track_id = obj_trk_id;
      det_props[det_id_2 - 1].range_rate_predicted = 20.0F;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0U] = 4;
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  This test checks that detections associated to an object that is killed have some of their object
*\         related properties cleared.
*\req    NA
*/
TEST(f360_kill_obj_trk_and_clear_assoc_det_obj_props, Test_Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props)
{
   /** \precond
   An object with two associated detections has been set up
   **/

   /** \action
   * Call Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props().
   **/
   Kill_Obj_Track_And_Clear_Assoc_Det_Obj_Props(obj_trk_id, object_tracks, tracker_info, det_props);

   /** \result
   * Check that the object is killed and its associated detections have their object ID cleared.
   **/
   CHECK_EQUAL_TEXT(0, tracker_info.num_active_objs, "There should be 0 active objects")

   CHECK_EQUAL_TEXT(0, det_props[det_id_1 - 1].object_track_id, "Detections object_track_id should be 0.")
   CHECK_EQUAL_TEXT(0, det_props[det_id_2 - 1].object_track_id, "Detections object_track_id should be 0.")
}

/** @}*/
