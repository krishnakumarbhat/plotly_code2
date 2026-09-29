/** \file
 * This file contains unit tests for content of f360_detection_association_evaluation.cpp file
 */

#include "f360_detection_association_evaluation.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_detection_association_evaluation
 *  @{
 */

/** \brief
 * This test group includes test of the function Detection_Association_Evaluation() defined in 
 * f360_detection_association_evaluation.cpp.
 */
TEST_GROUP(f360_detection_association_evaluation)
{	
   // Declare common variables used within all tests in this test group.
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Calibrations_T calibrations = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   /** \setup
   * Initialize calibrations.
   * Set up a scenario with:
   * - One active, moveable track
   * - Two valid detections in current scan
   * - 5 associated detections in precious scan (3 moving, 2 other)    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.active_obj_ids[0] = 1;
      tracker_info.num_active_objs = 1;

      raw_detection_list.number_of_valid_detections = 2U;

      object_tracks[0].ndets = 5;
      object_tracks[0].num_types_of_dets[0] = 3;
      object_tracks[0].num_types_of_dets[1] = 2;
      object_tracks[0].detids[0] = 1U;
      object_tracks[0].detids[1] = 2U;
      object_tracks[0].detids[2] = 3U;
      object_tracks[0].detids[3] = 4U;
      object_tracks[0].detids[4] = 5U;
      object_tracks[0].movable_prob = 1.0F;
   }
};

/** \purpose
* Test that Detection_Association_Evaluation() works as intended when there are no valid detections and no active objects.
* \req
* NA
*/
TEST(f360_detection_association_evaluation, DetectionAssociationEvaluation_NoObjNoDets)
{
   /** \precond
   * Set number of active objects to 0 and number of valid detections to 0.
   */
   tracker_info.num_active_objs = 0;
   raw_detection_list.number_of_valid_detections = 0U;
	
   /** \action
    * Call Detection_Association_Evaluation().
    */
   Detection_Association_Evaluation(host, sensors, tracker_info, raw_detection_list, calibrations, sep,detection_props, object_tracks);

   /** \result
   * Check that object properties ndets, num_types_of_dets and detids have not been modified.
   */
   CHECK_EQUAL_TEXT(5, object_tracks[0].ndets, "Object property ndets was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(3, object_tracks[0].num_types_of_dets[0], "Object property num_types_of_dets (moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(2, object_tracks[0].num_types_of_dets[1], "Object property num_types_of_dets (non-moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(1, object_tracks[0].detids[0], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(2, object_tracks[0].detids[1], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(3, object_tracks[0].detids[2], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(4, object_tracks[0].detids[3], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(5, object_tracks[0].detids[4], "Object property detids was modified in an unexpected way.")
}

/** \purpose
* Test that Detection_Association_Evaluation() works as intended when there are no valid detections but one active moveable object.
* \req
* NA
*/
TEST(f360_detection_association_evaluation, DetectionAssociationEvaluation_MoveableObj)
{
   /** \precond
   * Set number of valid detections to zero but let there be one active object.
   */
   raw_detection_list.number_of_valid_detections = 0U;

   /** \action
   * Call Detection_Association_Evaluation()
   */
   Detection_Association_Evaluation(host, sensors, tracker_info, raw_detection_list, calibrations, sep, detection_props, object_tracks);

   /** \result
   * Check that object properties ndets, num_types_of_dets and detids are reset to 0.
   */
   CHECK_EQUAL_TEXT(0, object_tracks[0].ndets, "Object property ndets was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].num_types_of_dets[0], "Object property num_types_of_dets (moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].num_types_of_dets[1], "Object property num_types_of_dets (non-moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[0], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[1], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[2], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[3], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[4], "Object property detids was modified in an unexpected way.")
}

/** \purpose
* Test that Detection_Association_Evaluation() works as intended when there are no valid detections but one active non-moveable object.
* \req
* NA
*/
TEST(f360_detection_association_evaluation, DetectionAssociationEvaluation_NonMoveableObj)
{
   /** \precond
   * Set number of valid detections to zero but let there be one active object. Set that object to non-moveable.
   */
   raw_detection_list.number_of_valid_detections = 0U;
   object_tracks[0].movable_prob = 0.0F;

   /** \action
   * Call Detection_Association_Evaluation()
   */
   Detection_Association_Evaluation(host, sensors, tracker_info, raw_detection_list, calibrations, sep, detection_props, object_tracks);

   /** \result
   * Check that object properties ndets, num_types_of_dets and detids are reset to 0.
   */
   CHECK_EQUAL_TEXT(0, object_tracks[0].ndets, "Object property ndets was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].num_types_of_dets[0], "Object property num_types_of_dets (moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].num_types_of_dets[1], "Object property num_types_of_dets (non-moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[0], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[1], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[2], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[3], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(0, object_tracks[0].detids[4], "Object property detids was modified in an unexpected way.")
}

/** \purpose
* Test that Detection_To_Track_Association() works as intended when we retrieve an obj idx which is less than 0.
* There are no valid detections in this test.
* \req
* NA
*/
TEST(f360_detection_association_evaluation, DetectionAssociationEvaluation_ObjIdxLessThanZero)
{
   /** \precond
   * Set number of valid detections to 0. Set first element in active_obj_ids to 0 to invoke an obj_idx smaller than 0.
   */
   raw_detection_list.number_of_valid_detections = 0U;
   tracker_info.active_obj_ids[0] = 0;

   /** \action
   * Call Detection_Association_Evaluation()
   */
   Detection_Association_Evaluation(host, sensors, tracker_info, raw_detection_list, calibrations, sep, detection_props, object_tracks);

   /** \result
   * Check that object properties ndets, num_types_of_dets and detids have not been modified.
   */
   CHECK_EQUAL_TEXT(5, object_tracks[0].ndets, "Object property ndets was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(3, object_tracks[0].num_types_of_dets[0], "Object property num_types_of_dets (moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(2, object_tracks[0].num_types_of_dets[1], "Object property num_types_of_dets (non-moving) was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(1, object_tracks[0].detids[0], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(2, object_tracks[0].detids[1], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(3, object_tracks[0].detids[2], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(4, object_tracks[0].detids[3], "Object property detids was modified in an unexpected way.")
   CHECK_EQUAL_TEXT(5, object_tracks[0].detids[4], "Object property detids was modified in an unexpected way.")
}
/** @}*/
