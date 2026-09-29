/** \file
 * This file contains unit tests for content of f360_clear_det_assoc_obj_props.cpp file
 */

#include "f360_clear_det_assoc_obj_props.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_clear_det_assoc_obj_props
 *  @{
 */

/** \brief
 * This test group includes tests for the function Clear_Det_Assoc_Obj_Props(), i.e. checking
 * that object related detection properties are cleared for a specified object.
 */
TEST_GROUP(f360_clear_det_assoc_obj_props)
{
   F360_Object_Track_T obj = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * - An object is set up with two associated detections
    * - Four detections are set up, two that are associated to the object and two that are not.
    */
   TEST_SETUP()
   {
      obj.id = 5U;
      obj.ndets = 2;
      obj.detids[0U] = 1U;
      obj.detids[1U] = 3U;

      detection_props[0U].object_track_id = 5U;
      detection_props[0U].f_rr_inlier = true;
      detection_props[0U].range_rate_predicted = 10.0F;

      detection_props[1U].object_track_id = 1U;
      detection_props[1U].f_rr_inlier = true;
      detection_props[1U].range_rate_predicted = -10.0F;

      detection_props[2U].object_track_id = 5U;
      detection_props[2U].f_rr_inlier = true;
      detection_props[2U].range_rate_predicted = -10.0F;

      detection_props[3U].object_track_id = 1U;
      detection_props[3U].f_rr_inlier = true;
      detection_props[3U].range_rate_predicted = -15.0F;
   }
};

/** \purpose
 * The purpose is to tests that all detections associated to a specified object have their object related properties cleared.
 * \req NA
 */
TEST(f360_clear_det_assoc_obj_props, Clear_Det_Assoc_Obj_Props)
{
   /** \precond
    * A test case in set up with:
    * - One object
    * - Two detections associated to the object
    * - Two detections not associated to the object
    */

   /** \action
    * Call Clear_Det_Assoc_Obj_Props .
    */
   Clear_Det_Assoc_Obj_Props(obj, detection_props);
   /** \result
    * Check that the detections are modified correctly.
    */

   // Check that the two detections that are associated to the object have had their properties reset.
   CHECK_EQUAL_TEXT(0U, detection_props[0U].object_track_id, "Detection object_track_id should have been reset.");
   CHECK_FALSE_TEXT(detection_props[0U].f_rr_inlier, "f_rr_inlier should have been reset.")
   DOUBLES_EQUAL_TEXT(0.0F, detection_props[0U].range_rate_predicted, F360_EPSILON, "range_rate_predicted should have been reset.");

   CHECK_EQUAL_TEXT(0U, detection_props[2U].object_track_id, "Detection object_track_id should have been reset.");
   CHECK_FALSE_TEXT(detection_props[2U].f_rr_inlier, "f_rr_inlier should have been reset.")
   DOUBLES_EQUAL_TEXT(0.0F, detection_props[2U].range_rate_predicted, F360_EPSILON, "range_rate_predicted should have been reset.");

   // Check that the detections not associated to the object are unchanged.
   CHECK_EQUAL_TEXT(1U, detection_props[1U].object_track_id, "Detection object_track_id should not have been reset.");
   CHECK_TRUE_TEXT(detection_props[1U].f_rr_inlier, "f_rr_inlier should not have been reset.")
   DOUBLES_EQUAL_TEXT(-10.0F, detection_props[1U].range_rate_predicted, F360_EPSILON, "range_rate_predicted should not have been reset.");

   CHECK_EQUAL_TEXT(1U, detection_props[3U].object_track_id, "Detection object_track_id should not have been reset.");
   CHECK_TRUE_TEXT(detection_props[3U].f_rr_inlier, "f_rr_inlier should not have been reset.")
   DOUBLES_EQUAL_TEXT(-15.0F, detection_props[3U].range_rate_predicted, F360_EPSILON, "range_rate_predicted should not have been reset.");
}
/** @}*/
