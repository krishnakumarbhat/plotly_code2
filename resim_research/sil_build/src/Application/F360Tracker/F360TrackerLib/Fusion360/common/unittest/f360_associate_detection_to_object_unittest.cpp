/** \file
   File contains test cases for Update_Detection_Number_In_Object()

*/

#include "f360_associate_detection_to_object.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

/** \defgroup  f360_associate_detection_to_object
 *  @{
 */

using namespace f360_variant_A;

/** \defgroup  f360_associate_detection_to_object
 *  @{
 */

/** \brief
 *  Test group for f360_associate_detection_to_object 
 *  It checks, if num_types_of_dets and ndets were incremented propertly
 */

TEST_GROUP(f360_associate_detection_to_object)
{
   /** \setup
   * Nothing to setup in this test group
   */
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_track;
   F360_Detection_Props_T detection_props;
   uint32_t det_id;


   TEST_SETUP()
   {
      object_track = {};
      detection_props = {};
      det_id = 0U;
      Set_Tracker_Variant(tracker_info.variant);
   }

   /** \teardown
    * Nothing to teardown in this test group
    */
   TEST_TEARDOWN()
   {
   }

};


/**
*\purpose  This test checks whether number of detections is updated properly 
*\req      put in requirement tag if any otherwise set to NA
*/
TEST(f360_associate_detection_to_object, test_UpdateForMovingDetection)
{
   /** \step{step number e.g. 1}
    */

   /** \precond
    *   In this test case object will update it parameters by one moving detection
    */

   /** \action
    * Call Associate_Detection_To_Object().
    */

   /** \result
    * ndet and num_types_of_dets[0] were incremented in object_track properties.
    */

   object_track.id = 1;
   object_track.ndets = 5U;
   object_track.num_types_of_dets[0] = 1;
   object_track.num_types_of_dets[1] = 4;
 
   det_id = 4;
   detection_props.object_track_id = 10;
   detection_props.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   uint32_t expected_track_ndets = object_track.ndets + 1U;
   int32_t expected_track_num_types_0 = object_track.num_types_of_dets[0] + 1;
 
   Associate_Detection_To_Object(tracker_info, object_track, detection_props, det_id);

   CHECK_EQUAL_TEXT(expected_track_ndets, object_track.ndets, "Unexpected value of ndets in object track");
   CHECK_EQUAL_TEXT(expected_track_num_types_0, object_track.num_types_of_dets[0], "Unexpected value in type of dets in object track");
}

TEST(f360_associate_detection_to_object, test_UpdateForNonMovingDetection)
{
   /** \step{step number e.g. 2}
    */

   /** \precond
    *   In this test case object will update it parameters by one Ambiguous detection
    */

   /** \action
    * Call Associate_Detection_To_Object().
    */

   /** \result
    * ndet and num_types_of_dets[1] were incremented in object_track properties.
    */

   object_track.id = 1;
   object_track.ndets = 5U;
   object_track.num_types_of_dets[0] = 1;
   object_track.num_types_of_dets[1] = 4;

   det_id = 4;
   detection_props.object_track_id = 1;
   detection_props.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   uint32_t expected_track_ndets = object_track.ndets + 1U;
   int32_t expected_track_num_types_1 = object_track.num_types_of_dets[1] + 1;

   Associate_Detection_To_Object(tracker_info, object_track, detection_props, det_id);

   CHECK_EQUAL_TEXT(expected_track_ndets, object_track.ndets, "Unexpected value of ndets in object track");
   CHECK_EQUAL_TEXT(expected_track_num_types_1, object_track.num_types_of_dets[1], "Unexpected value in types of dets in object track");
}

TEST(f360_associate_detection_to_object, test_UpdateNoTPossibleDueDetsNumber)
{
   /** \step{step number e.g. 3}
    */

   /** \precond
    *   In this test case new detections association is not possible due to 
    *   number of detections already associated to object
    */

   /** \action
    * Call Associate_Detection_To_Object().
    */

   /** \result
    * function should return false.
    */
   object_track.id = 1;
   object_track.ndets = MAX_DETS_IN_OBJ_TRK;
   detection_props.object_track_id = 9;

   bool f_is_adding_new_detection_possible =  Associate_Detection_To_Object(tracker_info, object_track, detection_props, det_id);

   CHECK_FALSE_TEXT(f_is_adding_new_detection_possible, "Unexpected possibility of adding new data");
}

TEST(f360_associate_detection_to_object, test_UpdateNoTPossibleDueToInvalidObjectID)
{
   /** \step{step number e.g. 3}
    */

   /** \precond
    *   In this test case new detections association is not possible due to
    *   invalid object ID
    */

   /** \action
    * Call Associate_Detection_To_Object().
    */

   /** \result
    * function should return false.
    */
   object_track.id = 0;
   object_track.ndets = 8;

   bool f_is_adding_new_detection_possible = Associate_Detection_To_Object(tracker_info, object_track, detection_props, det_id);

   CHECK_FALSE_TEXT(f_is_adding_new_detection_possible, "Unexpected possibility of adding new data");
}

/** @}*/
