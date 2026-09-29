/** \file
   File contains test cases for Move_Dets_From_Killed_To_Kept_Object() function.
*/

#include "f360_move_dets_from_killed_to_kept_object.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_move_dets_from_killed_to_kept_object
 *  @{
 */

 /** \brief
  *  Test group for testing Move_Dets_From_Killed_To_Kept_Object function.
  */
TEST_GROUP(f360_move_dets_from_killed_to_kept_object)
{
   // Initialize needed variables 
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Calibrations_T calib;
   F360_Object_Track_T object_track_to_kill = {};
   F360_Object_Track_T object_track_to_keep = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Initialize calibrations and objects' ids.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);

      object_track_to_kill.id = 1;
      object_track_to_keep.id = 2;
   }
};

/** \purpose Check whether all detections were properly moved form killed to kept object.
 *  \req     NA.
 */
TEST(f360_move_dets_from_killed_to_kept_object, Move_Dets_From_Killed_To_Kept_Object_All_Dets_Selected_By_Tkr)
{
   /** \precond
    * Define number of detections associated to each object. 
    * All detections associated to both objects have f_rr_inlier
    * flag set to true.
    */
   object_track_to_kill.ndets = 9;
   uint32_t exp_number_of_moved_dets = object_track_to_kill.ndets;
   for (uint32_t idx = 0U; idx < object_track_to_kill.ndets; idx++)
   {
      object_track_to_kill.detids[idx] = idx + 1;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
      detection_props[idx].f_rr_inlier = true;
   }

   object_track_to_keep.ndets = 10;
   uint32_t exp_number_of_kept_dets = object_track_to_keep.ndets;
   for (uint32_t idx = 0U; idx < object_track_to_keep.ndets; idx++)
   {
      object_track_to_keep.detids[idx] = idx + object_track_to_kill.ndets + 1;
      raw_detection_list.detections[idx + object_track_to_kill.ndets].raw.sensor_id = 1;
      detection_props[idx + object_track_to_kill.ndets].f_rr_inlier = true;
   }
   
   uint32_t exp_number_of_dets_in_kept_obj = object_track_to_keep.ndets + object_track_to_kill.ndets;
   
   /** \action
    * Call Move_Dets_From_Killed_To_Kept_Object().
    */
   Move_Dets_From_Killed_To_Kept_Object(host, tracker_info, sensors, raw_detection_list, calib, object_track_to_kill, object_track_to_keep, detection_props);

   /** \result
    *  Check if detections from killed object were move to kept object
    *  and number of detections in kept object had been increased by
    *  detections from killed object.
    */
   bool f_detection_moved = true;
   for (uint32_t idx = 0; idx < exp_number_of_moved_dets; idx++)
   {
      f_detection_moved &= (object_track_to_keep.detids[idx + exp_number_of_kept_dets] == idx + 1);
   }

   CHECK_TRUE(f_detection_moved);
   CHECK_EQUAL(exp_number_of_dets_in_kept_obj, object_track_to_keep.ndets);
}

/** \purpose Check whether all detections were properly moved form killed to kept object.
 *  \req     NA.
 */
TEST(f360_move_dets_from_killed_to_kept_object, Move_Dets_From_Killed_To_Kept_Object_All_Dets_From_Killed_Obj_Not_Selected_By_Tkr)
{
   /** \precond
    * Define number of detection associated to each object.
    * All detections associated to kept object have f_rr_inlier
    * flag set to true. Detections from killed object have f_rr_inlier
    * flag set to false.
    */
   object_track_to_kill.ndets = 9U;
   uint32_t exp_number_of_moved_dets = object_track_to_kill.ndets;
   for (uint32_t idx = 0U; idx < object_track_to_kill.ndets; idx++)
   {
      object_track_to_kill.detids[idx] = idx + 1;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
      detection_props[idx].f_rr_inlier = false;
   }

   object_track_to_keep.ndets = 10U;
   uint32_t exp_number_of_kept_dets = object_track_to_keep.ndets;
   for (uint32_t idx = 0U; idx < object_track_to_keep.ndets; idx++)
   {
      object_track_to_keep.detids[idx] = idx + object_track_to_kill.ndets + 1;
      raw_detection_list.detections[idx + object_track_to_kill.ndets].raw.sensor_id = 1;
      detection_props[idx + object_track_to_kill.ndets].f_rr_inlier = true;
   }

   uint32_t exp_number_of_dets_in_kept_obj = object_track_to_kill.ndets + object_track_to_keep.ndets;

   /** \action
    * Call Move_Dets_From_Killed_To_Kept_Object().
    */
   Move_Dets_From_Killed_To_Kept_Object(host, tracker_info, sensors, raw_detection_list, calib, object_track_to_kill, object_track_to_keep, detection_props);

   /** \result
    * Check if detections from killed object were move to kept object
    * and number of detections in kept object had been increased by 
    * detections from killed object.
    */
   bool f_detection_moved = true;
   for (uint32_t idx = 0; idx < exp_number_of_moved_dets; idx++)
   {
      f_detection_moved &= (object_track_to_keep.detids[idx + exp_number_of_kept_dets] == idx + 1);
   }

   CHECK_TRUE(f_detection_moved);
   CHECK_EQUAL(exp_number_of_dets_in_kept_obj, object_track_to_keep.ndets);
}

/** \purpose Check whether detections have not been moved form killed to kept object,
 *           when maximum number of detections is already associated to kept object.
 *  \req     NA.
 */
TEST(f360_move_dets_from_killed_to_kept_object, Move_Dets_From_Killed_To_Many_Selected_By_Trk_Dets_In_Killed_Obj)
{
   /** \precond
    * Define number of detection associated to each object.
    * All detections associated to both objects have f_rr_inlier
    * set to true.
    * Kept object has maximum number of associated detections.
    */
   object_track_to_kill.ndets = 9U;
   for (uint32_t idx = 0U; idx < object_track_to_kill.ndets; idx++)
   {
      object_track_to_kill.detids[idx] = idx + 1;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
      detection_props[idx].f_rr_inlier = true;
   }
   object_track_to_keep.ndets = MAX_DETS_IN_OBJ_TRK;
   for (uint32_t idx = 0U; idx < object_track_to_keep.ndets; idx++)
   {
      object_track_to_keep.detids[idx] = idx + object_track_to_kill.ndets + 1;
      raw_detection_list.detections[idx + object_track_to_kill.ndets].raw.sensor_id = 1;
      detection_props[idx + object_track_to_kill.ndets].f_rr_inlier = true;
   }

   /** \action
    * Call Move_Dets_From_Killed_To_Kept_Object().
    */
   Move_Dets_From_Killed_To_Kept_Object(host, tracker_info, sensors, raw_detection_list, calib, object_track_to_kill, object_track_to_keep, detection_props);

   /** \result
    * Check if detections association in kept object has not changed
    * and number of detections of kept object do not increase.
    */
   bool f_detection_kept = true;
   for (uint32_t idx = 0; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      f_detection_kept &= (object_track_to_keep.detids[idx] == idx + object_track_to_kill.ndets + 1);
   }

   CHECK_TRUE(f_detection_kept);
   CHECK_EQUAL(MAX_DETS_IN_OBJ_TRK, object_track_to_keep.ndets);
}

/** \purpose Check whether detections were properly moved form killed to kept object.
 *  \req     NA.
 */
TEST(f360_move_dets_from_killed_to_kept_object, Move_Dets_From_Killed_To_Many_Not_Selected_By_Trk_Dets_In_Killed_Obj)
{
   /** \precond
    * Define number of detection associated to each object.
    * All detections associated to kept object have f_rr_inlier set to true.
    * Killed object has maximum number of associated detections.
    */
   object_track_to_kill.ndets = MAX_DETS_IN_OBJ_TRK;
   for (uint32_t idx = 0U; idx < object_track_to_kill.ndets; idx++)
   {
      object_track_to_kill.detids[idx] = idx + 1;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
      detection_props[idx].f_rr_inlier = false;
   }
   
   object_track_to_keep.ndets = 9U;
   uint32_t initial_number_of_dets_in_kept_object = object_track_to_keep.ndets;
   for (uint32_t idx = 0U; idx < object_track_to_keep.ndets; idx++)
   {
      object_track_to_keep.detids[idx] = idx + object_track_to_kill.ndets + 1;
      raw_detection_list.detections[idx + object_track_to_kill.ndets].raw.sensor_id = 1;
      detection_props[idx + object_track_to_kill.ndets].f_rr_inlier = true;
   }

   /** \action
    * Call Move_Dets_From_Killed_To_Kept_Object().
    */
   Move_Dets_From_Killed_To_Kept_Object(host, tracker_info, sensors, raw_detection_list, calib, object_track_to_kill, object_track_to_keep, detection_props);

   /** \result
    * Check if kept object number is not greater than max number of 
    * detections and detections were move only till place in matrix ended.
    */
   bool f_detection_moved = true;
   uint32_t det_id = 1;
   for (uint32_t idx = initial_number_of_dets_in_kept_object; idx < MAX_DETS_IN_OBJ_TRK; idx++)
   {
      f_detection_moved &= (object_track_to_keep.detids[idx] == det_id++);
   }

   CHECK_TRUE(f_detection_moved);
   CHECK_EQUAL(MAX_DETS_IN_OBJ_TRK, object_track_to_keep.ndets);
}

/** \purpose Check whether detections were properly moved form killed to kept object.
 *  \req     NA.
 */
TEST(f360_move_dets_from_killed_to_kept_object, Move_Dets_From_Killed_To_Many_Some_Selected_By_Trk_Dets_In_Killed_Obj)
{
   /** \precond
    * Define number of detection associated to each object.
    * All detections associated to kept object have f_rr_inlier set to true.
    * Killed object has maximum number of associated detections.
    */
   object_track_to_kill.ndets = MAX_DETS_IN_OBJ_TRK;
   for (uint32_t idx = 0U; idx < object_track_to_kill.ndets; idx++)
   {
      object_track_to_kill.detids[idx] = idx + 1;
      detection_props[idx].f_rr_inlier = true;
      raw_detection_list.detections[idx].raw.sensor_id = 1;
   }
   detection_props[0].f_rr_inlier = false;
   detection_props[1].f_rr_inlier = false;
   detection_props[2].f_rr_inlier = false;

   object_track_to_keep.ndets = 2U;
   uint32_t initial_number_of_dets_in_kept_object = object_track_to_keep.ndets;
   for (uint32_t idx = 0U; idx < object_track_to_keep.ndets; idx++)
   {
      object_track_to_keep.detids[idx] = idx + object_track_to_kill.ndets + 1;
      detection_props[idx + object_track_to_kill.ndets].f_rr_inlier = true;
      raw_detection_list.detections[idx + object_track_to_kill.ndets].raw.sensor_id = 1;
   }


   /** \action
    * Call Move_Dets_From_Killed_To_Kept_Object().
    */
   Move_Dets_From_Killed_To_Kept_Object(host, tracker_info, sensors, raw_detection_list, calib, object_track_to_kill, object_track_to_keep, detection_props);

   /** \result
    * Check if kept object number is not greater than max number of
    * detections and detections were move only till place in matrix ended.
    */
   bool f_detection_moved = true;
   uint32_t det_id = 4;
   for (uint32_t idx = initial_number_of_dets_in_kept_object; idx < (MAX_DETS_IN_OBJ_TRK-1); idx++)
   {
      f_detection_moved &= (object_track_to_keep.detids[idx] == det_id++);
   }
   f_detection_moved &= (object_track_to_keep.detids[MAX_DETS_IN_OBJ_TRK - 1] == 1);

   CHECK_TRUE(f_detection_moved);
   CHECK_EQUAL(MAX_DETS_IN_OBJ_TRK, object_track_to_keep.ndets);
}
/** @}*/
