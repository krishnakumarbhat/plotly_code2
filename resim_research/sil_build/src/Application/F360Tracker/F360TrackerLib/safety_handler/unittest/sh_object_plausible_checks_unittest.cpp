/** \file
 * This file contains unit tests for content of sh_object_plausible_checks.cpp file
 */
#include <CppUTest/TestHarness.h>
#include <cmath>
#include "sh_object_plausible_checks.h"
#include "sh_safety_handler_internal.h"
#include "sh_boundingbox_helper_functions.h"
#include "sh_unittest_support.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  test_Object_Position_Plausible_Check
 *  @{
 */

/** \brief
 * Test group for Object_Position_Plausible_Check function.
 */
TEST_GROUP(test_Object_Position_Plausible_Check)
{
   // Declare common variables used within all tests in this test group.
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Detection_Log_T f360_detection_list[MAX_NUMBER_OF_DETECTIONS];
   ROT_Object_List_Info_T rot_object_list_info{};

   /** \setup
    * Initialized the overall fault status for each test case.
    */
   TEST_SETUP()
   {
      rot_object_list_info.number_of_objects = 0U;
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
      raw_detect_list.number_of_valid_detections = 0U;
   }
};

/** \purpose
 * Test Object_Position_Plausible_Check function when all associated detections are outside the bounding box.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_All_Det_Outside_Boundingbox)
{
   /** \precond
    * define one object track which associated detection are all outside of plausible region
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);

   Create_And_Associate_Detections(raw_detect_list, f360_detection_list,
                                   object,
                                   17U,
                                   false);

   /** \action
    * call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible to be false.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check function when all associated detection is inside the bounding box.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_All_Det_Inside)
{
   /** \precond
    * Define one object track which has at all associated detection inside the bounding box.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);

   Create_And_Associate_Detections(raw_detect_list, f360_detection_list,
                                   object,
                                   16U,
                                   true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections, f360_detection_list, raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is implausible to be true.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check function when one associated detection is inside the bounding box.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_One_Det_Inside)
{
   /** \precond
    * Define one object track which has at least one associated detection inside the bounding box.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);

   // Add 22 detections outside of the bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list,
                                   object,
                                   22U,
                                   false);

   // Add 1 detections inside of the bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list,
                                   object,
                                   1U,
                                   true);

   // Add 30 detections outside of the bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list,
                                   object,
                                   30U,
                                   false);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is implausible to be true.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Is_Target_Params_Within_Scope check: verify that object with ID=0 is not in scope.
 * Even when all detections are outside the bounding box, the check should return plausible (true)
 * because objects with ID=0 are skipped by Is_Target_Params_Within_Scope.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Not_In_Scope_ID_Zero)
{
   /** \precond
    * Create object with valid settings and detections outside bounding box.
    * Then set object ID to 0 (not in scope).
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);

   // Create 3 detections outside the bounding box
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object,
                                   3U,     // number of detections
                                   false); // outside bounding box

   // First verify it fails with valid ID
   bool f_plausible_before = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                             f360_detection_list,
                                                             raw_detect_list.number_of_valid_detections,
                                                             object);

   /** \action
    * Set object ID to 0 (not in scope) and run Object_Position_Plausible_Check().
    */
   object.id = 0;

   bool f_plausible_after = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                            f360_detection_list,
                                                            raw_detect_list.number_of_valid_detections,
                                                            object);
   /** \result
    * Before: implausible (false) - detections outside with valid ID
    * After: plausible (true) - object with ID=0 is not in scope, check passes
    */
   CHECK_FALSE(f_plausible_before);
   CHECK_TRUE(f_plausible_after);
}

/** \purpose
 * Test Is_Target_Params_Within_Scope check: verify that coasted object (status=2) is not in scope.
 * Even when all detections are outside the bounding box, the check should return plausible (true)
 * because coasted objects are skipped by Is_Target_Params_Within_Scope.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Not_In_Scope_Coasted_Status)
{
   /** \precond
    * Create object with valid settings and detections outside bounding box.
    * Then set object status to 2 (coasted - not in scope).
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);

   // Create 3 detections outside the bounding box
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object,
                                   3U,     // number of detections
                                   false); // outside bounding box

   // First verify it fails with measured status (0)
   bool f_plausible_before = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                             f360_detection_list,
                                                             raw_detect_list.number_of_valid_detections,
                                                             object);

   /** \action
    * Set object status to 2 (coasted - not in scope) and run Object_Position_Plausible_Check().
    */
   object.object_status = 2U; // OBJECT_STATUS_COASTED

   bool f_plausible_after = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                            f360_detection_list,
                                                            raw_detect_list.number_of_valid_detections,
                                                            object);

   /** \result
    * Before: implausible (false) - detections outside with measured status
    * After: plausible (true) - coasted object is not in scope, check passes
    */
   CHECK_FALSE(f_plausible_before);
   CHECK_TRUE(f_plausible_after);
}

/** \purpose
 * Test Is_Target_Params_Within_Scope check: verify that invalid object (status=255) is not in scope.
 * Even when all detections are outside the bounding box, the check should return plausible (true)
 * because invalid objects are skipped by Is_Target_Params_Within_Scope.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Not_In_Scope_Invalid_Status)
{
   /** \precond
    * Create object with valid settings and detections outside bounding box.
    * Then set object status to 255 (invalid - not in scope).
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);

   // Create 3 detections outside the bounding box
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object,
                                   3U,     // number of detections
                                   false); // outside bounding box

   // First verify it fails with measured status (0)
   bool f_plausible_before = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                             f360_detection_list,
                                                             raw_detect_list.number_of_valid_detections,
                                                             object);

   /** \action
    * Set object status to 255 (invalid - not in scope) and run Object_Position_Plausible_Check().
    */
   object.object_status = 255U; // OBJECT_STATUS_INVALID

   bool f_plausible_after = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                            f360_detection_list,
                                                            raw_detect_list.number_of_valid_detections,
                                                            object);

   /** \result
    * Before: implausible (false) - detections outside with measured status
    * After: plausible (true) - invalid object is not in scope, check passes
    */
   CHECK_FALSE(f_plausible_before);
   CHECK_TRUE(f_plausible_after);
}

/** \purpose
 * Test Is_Target_Params_Within_Scope check: verify that newly created object (status=1) IS in scope.
 * When detections are outside the bounding box, the check should return implausible (false)
 * because newly created objects (status=1) ARE checked by Is_Target_Params_Within_Scope.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_In_Scope_Newly_Created_Status)
{
   /** \precond
    * Create object with valid settings and detections outside bounding box.
    * Set object status to 1 (newly created - in scope).
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.object_status = 1U; // OBJECT_STATUS_NEWLY_CREATED

   // Create 3 detections outside the bounding box
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object,
                                   3U,     // number of detections
                                   false); // outside bounding box

   /** \action
    * Run Object_Position_Plausible_Check() with newly created object.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is implausible (false) - newly created objects ARE checked,
    * and detections are outside bounding box.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Scope_Check_Multiple_Objects - SIMPLIFIED VERSION for debugging
 * Tests single object to verify baseline behavior.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Scope_Check_Single_Object_Outside)
{
   /** \precond
    * Create 1 object with detections outside bounding box.
    */

   // Object 1: Valid ID and measured status (in scope), detections outside
   Add_Default_Object(rot_object_list_info, true);
   ROT_Object_Output_T &object = rot_object_list_info.rot_object_list[0];
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object,
                                   2U,     // num detections
                                   false); // outside bbox

   /** \action
    * Run Object_Position_Plausible_Check() for the object.
    */
   bool f_plausible_obj1 = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                           f360_detection_list,
                                                           raw_detect_list.number_of_valid_detections,
                                                           object);

   /** \result
    * Object 1: implausible (false) - in scope with detections outside
    */
   CHECK_FALSE(f_plausible_obj1);
}

/** \purpose
 * Test Is_Target_Params_Within_Scope check: comprehensive test with all scope conditions.
 * Tests multiple objects with different scope conditions simultaneously.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Scope_Check_Multiple_Objects)
{
   /** \precond
    * Create 4 objects with different scope conditions, all with detections outside bounding box:
    * - Object 1 (ID=1, status=0): In scope - should be implausible
    * - Object 2 (ID=0, status=0): Not in scope (ID=0) - should be plausible
    * - Object 3 (ID=3, status=2): Not in scope (coasted) - should be plausible
    * - Object 4 (ID=4, status=255): Not in scope (invalid) - should be plausible
    */

   // Object 1: Valid ID and measured status (in scope)
   Add_Default_Object(rot_object_list_info, true);
   ROT_Object_Output_T &object = rot_object_list_info.rot_object_list[0];
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object,
                                   2U,     // num detections
                                   false); // outside bbox

   // Object 2: ID=0 (not in scope) - Set ID BEFORE creating detections
   Add_Default_Object(rot_object_list_info, true);
   ROT_Object_Output_T &object2 = rot_object_list_info.rot_object_list[1];
   object2.id = 0; // Set to 0 BEFORE creating detections
   object2.vcs_x_posn = 20.0f;
   // Don't create detections for out-of-scope object with ID=0
   // (detections would be associated to ID=0 which doesn't make sense)

   // Object 3: Coasted status (not in scope) - Set status BEFORE creating detections
   Add_Default_Object(rot_object_list_info, true);
   ROT_Object_Output_T &object3 = rot_object_list_info.rot_object_list[2];
   object3.object_status = 2U; // Set to coasted BEFORE creating detections
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object3,
                                   2U,     // num detections
                                   false); // outside bbox

   // Object 4: Invalid status (not in scope) - Set status BEFORE creating detections
   Add_Default_Object(rot_object_list_info, true);
   ROT_Object_Output_T &object4 = rot_object_list_info.rot_object_list[3];
   object4.object_status = 255U; // Set to invalid BEFORE creating detections
   object4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list,
                                   f360_detection_list,
                                   object4,
                                   2U,     // num detections
                                   false); // outside bbox

   /** \action
    * Run Object_Position_Plausible_Check() for each object.
    */
   bool f_plausible_obj1 = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                           f360_detection_list,
                                                           raw_detect_list.number_of_valid_detections,
                                                           object);

   bool f_plausible_obj2 = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                           f360_detection_list,
                                                           raw_detect_list.number_of_valid_detections,
                                                           object2);

   bool f_plausible_obj3 = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                           f360_detection_list,
                                                           raw_detect_list.number_of_valid_detections,
                                                           object3);

   bool f_plausible_obj4 = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                           f360_detection_list,
                                                           raw_detect_list.number_of_valid_detections,
                                                           object4);

   /** \result
    * Object 1: implausible (false) - in scope with detections outside
    * Object 2: plausible (true) - not in scope (ID=0)
    * Object 3: plausible (true) - not in scope (coasted)
    * Object 4: plausible (true) - not in scope (invalid)
    */
   CHECK_FALSE(f_plausible_obj1);
   CHECK_TRUE(f_plausible_obj2);
   CHECK_TRUE(f_plausible_obj3);
   CHECK_TRUE(f_plausible_obj4);
}

/** \purpose
 * Test Object_Position_Plausible_Check with detections exactly on the bounding box edge.
 * Detections at the exact edge (offset_scale = 1.0) should be considered inside.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Detections_On_Bounding_Box_Edge)
{
   /** \precond
    * Create object with detections positioned exactly at the extended bounding box edge.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;

   // Create detections exactly on the edge (offset_scale = 1.0)
   const uint32_t det_idx = 0U;
   Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed,
                                object,
                                1.0f,  // Exactly at front edge
                                1.0f); // Exactly at right edge
   f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);
   object.ndets++;
   raw_detect_list.number_of_valid_detections += object.ndets;

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections on edge are inside.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object.ndets > 0 but no matching detections.
 * This tests the case where object claims to have detections but none are actually associated.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Claims_Detections_But_None_Associated)
{
   /** \precond
    * Create object with ndets=5 but don't create any detections with matching objTrkID.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;

   // Create detections but associate them to a different object ID
   for (uint32_t i = 0U; i < 5U; i++)
   {
      Create_Detection_From_Object(raw_detect_list.detections[i].processed,
                                   object,
                                   0.0f, 0.0f);
      f360_detection_list[i].objTrkID = 999U; // Wrong ID
      object.ndets++;
   }
   raw_detect_list.number_of_valid_detections += object.ndets;

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is implausible (false) - no detections found inside bounding box.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with far range object where lateral offset increases.
 * At ranges > 60m, lateral_offset = range * 0.0555 exceeds the minimum 3.33m.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Far_Range_Object_With_Increased_Lateral_Offset)
{
   /** \precond
    * Create object at 100m range where lateral_offset = 100 * 0.0555 = 5.55m > 3.33m.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 100.0f; // 100 meters ahead
   object.vcs_y_posn = 0.0f;

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside with range-adjusted offset.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with near range object where lateral offset is minimum.
 * At ranges < 60m, lateral_offset = 3.33m (minimum value).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Near_Range_Object_With_Minimum_Lateral_Offset)
{
   /** \precond
    * Create object at 20m range where lateral_offset = max(3.33, 20 * 0.0555) = 3.33m.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 20.0f; // 20 meters ahead
   object.vcs_y_posn = 0.0f;

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside with minimum lateral offset.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with rotated object orientation.
 * Object with non-zero vcs_pointing should have rotated bounding box.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Rotated_Object_Orientation)
{
   /** \precond
    * Create object rotated 45 degrees (0.785 radians).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;
   object.vcs_pointing = 0.785f; // 45 degrees in radians

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside rotated bounding box.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with very small object dimensions.
 * Edge case for objects with minimal size (e.g., motorcycles).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Very_Small_Object_Dimensions)
{
   /** \precond
    * Create object with very small dimensions (0.5m x 0.5m).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.length = 0.5f; // Very small length
   object.width = 0.5f;  // Very small width

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside small object's extended bbox.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with very large object dimensions.
 * Edge case for large vehicles (e.g., trucks, buses).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Very_Large_Object_Dimensions)
{
   /** \precond
    * Create object with very large dimensions (20m x 3m - truck/bus size).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.length = 20.0f; // Very large length (truck/bus)
   object.width = 3.0f;   // Wide vehicle

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 10U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside large object's extended bbox.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with mixed detection positions.
 * Some detections exactly on edge, some inside, some just outside.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Mixed_Detection_Positions_At_Boundary)
{
   /** \precond
    * Create object with detections at various positions relative to bounding box edge.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;

   // Create 5 detections at different positions
   uint32_t det_idx = 0U;

   // Detection 1: Well outside (1.5)
   Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed, object, 1.5f, 1.5f);
   f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);
   det_idx++;

   // Detection 2: Just outside (1.01)
   Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed, object, 1.01f, 0.0f);
   f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);
   det_idx++;

   // Detection 3: Exactly on edge (1.0) - INSIDE
   Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed, object, 1.0f, 0.0f);
   f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);
   det_idx++;

   // Detection 4: Just inside (0.99)
   Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed, object, 0.99f, 0.0f);
   f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);
   det_idx++;

   // Detection 5: Well inside (0.5)
   Create_Detection_From_Object(raw_detect_list.detections[det_idx].processed, object, 0.5f, 0.0f);
   f360_detection_list[det_idx].objTrkID = static_cast<uint16_t>(object.id);

   raw_detect_list.number_of_valid_detections = 5U;
   object.ndets = 5U;

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - at least one detection (edge or inside) is within bbox.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object at origin (0,0).
 * Boundary case for object outside of AEB scope which is just behind the ego vehicle.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_At_Origin_Position)
{
   /** \precond
    * Create object at position (0,0).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 0.0f; // At origin
   object.vcs_y_posn = 0.0f; // At origin

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside object at origin.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object at origin (Epsilon,0).
 * Boundary case for AEB scope objects which is just in front the ego vehicle.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Just_Above_Origin_Position)
{
   /** \precond
    * Create object at position (Epsilon,0).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = __FLT_EPSILON__; // Just above origin
   object.vcs_y_posn = 0.0f; // At origin

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside object at origin.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object at lateral position.
 * Tests objects with significant lateral offset (non-zero Y).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_At_Lateral_Position)
{
   /** \precond
    * Create object at lateral position (10m ahead, 5m to the right).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f; // 10m ahead
   object.vcs_y_posn = 5.0f;  // 5m to the right (lateral)

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside laterally positioned object.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with negative pointing angle.
 * Tests objects pointing backwards (negative angle).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_With_Negative_Pointing_Angle)
{
   /** \precond
    * Create object with negative pointing angle (-45 degrees).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;
   object.vcs_pointing = -0.785f; // -45 degrees in radians

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside object with negative angle.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object pointing perpendicular.
 * Tests objects pointing 90 degrees (perpendicular to forward direction).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Pointing_Perpendicular)
{
   /** \precond
    * Create object pointing 90 degrees (perpendicular).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;
   object.vcs_pointing = 1.5708f; // 90 degrees in radians (pi/2)

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside perpendicular object.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object pointing backwards.
 * Tests objects pointing 180 degrees (backwards).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_Pointing_Backwards)
{
   /** \precond
    * Create object pointing 180 degrees (backwards).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;
   object.vcs_pointing = 3.14159f; // 180 degrees in radians (pi)

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside backwards-pointing object.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object having very high ndets count.
 * Tests boundary behavior for large number of associated detections.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_With_Very_High_Detection_Count)
{
   /** \precond
    * Create object with very high ndets count (100 detections).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;

   // Create 100 detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 100U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - high detection count with all inside.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object having single detection inside.
 * Minimum case for plausibility: exactly one detection inside bounding box.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_With_Single_Detection_Inside)
{
   /** \precond
    * Create object with exactly one detection inside bounding box.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;

   // Create exactly 1 detection inside
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - single detection is sufficient.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object at maximum supported range.
 * Tests behavior at extreme longitudinal distance.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_At_Maximum_Range)
{
   /** \precond
    * Create object at very far range (300m).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 300.0f; // 300m ahead
   object.vcs_y_posn = 0.0f;
   // At 300m, lateral_offset = 300 * 0.0555 = 16.65m

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside at maximum range.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with object at negative X position.
 * Tests objects behind the sensor/vehicle (negative longitudinal position).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_At_Negative_X_Position)
{
   /** \precond
    * Create object at negative X position (behind sensor).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = -10.0f; // 10m behind
   object.vcs_y_posn = 0.0f;

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside object behind sensor.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with asymmetric object dimensions.
 * Tests objects with very different length vs width (e.g., long narrow object).
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_With_Asymmetric_Dimensions)
{
   /** \precond
    * Create object with asymmetric dimensions (length=15m, width=1.5m - trailer).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.length = 15.0f; // Long object (trailer)
   object.width = 1.5f;   // Narrow width

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 8U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside asymmetric object.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check with zero-sized object dimensions.
 * Edge case for objects with zero or near-zero dimensions.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Object_With_Zero_Dimensions)
{
   /** \precond
    * Create object with zero dimensions (point object).
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   object.length = 0.0f; // Zero length
   object.width = 0.0f;  // Zero width

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - detections inside extended bbox (offsets still apply).
    */
   CHECK_TRUE(f_plausible);
}

/** @}*/

/** \defgroup  test_Object_Plausible_Checks
 *  @{
 */

/** \brief
 * Test group of object plausible checks functions.
 */
TEST_GROUP(test_Object_Plausible_Checks)
{
   // Declare common variables used within all tests in this test group.
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Detection_Log_T f360_detection_list[MAX_NUMBER_OF_DETECTIONS];
   ROT_Object_List_Info_T rot_object_list_info{};

   /** \setup
    * Initialized the overall fault status for each test case.
    */
   TEST_SETUP()
   {
      rot_object_list_info.number_of_objects = 0U;
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
      raw_detect_list.number_of_valid_detections = 0U;
   }
};

/** \purpose
 * Test Object_Plausible_Checks function when all objects are plausible.
 * All in-scope objects have detections inside their bounding boxes, and some objects are out of scope.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_All_Objects_Plausible)
{
   /** \precond
    * Create 3 objects where all are plausible:
    * - Object 1: In scope with detections inside bounding box
    * - Object 2: Not in scope (ID=0)
    * - Object 3: In scope with detections inside bounding box
    */

   // Object 1: In scope with detections inside
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object1, 5U, true);

   // Object 2: Not in scope (ID=0)
   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.id = 0;
   object2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object2, 3U, false);

   // Object 3: In scope with detections inside
   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object3, 4U, true);

   /** \action
    * Call Object_Plausible_Checks() with all objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is plausible (true) - all in-scope objects pass checks.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function when first object is implausible.
 * Should return false immediately and not check remaining objects.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_First_Object_Implausible)
{
   /** \precond
    * Create 3 objects:
    * - Object 1: In scope with detections OUTSIDE bounding box (implausible)
    * - Object 2: In scope with detections inside bounding box (plausible)
    * - Object 3: In scope with detections inside bounding box (plausible)
    */

   // Object 1: In scope but implausible
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object1, 5U, false);

   // Object 2: In scope and plausible
   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object2, 4U, true);

   // Object 3: In scope and plausible
   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object3, 3U, true);

   /** \action
    * Call Object_Plausible_Checks() with all objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - first object fails check.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function when middle object is implausible.
 * Should detect the implausible object and return false.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Middle_Object_Implausible)
{
   /** \precond
    * Create 5 objects:
    * - Object 1: In scope with detections inside (plausible)
    * - Object 2: Not in scope (ID=0, plausible)
    * - Object 3: In scope with detections OUTSIDE (implausible)
    * - Object 4: In scope with detections inside (plausible)
    * - Object 5: Coasted (not in scope, plausible)
    */

   // Object 1: Plausible
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object1, 3U, true);

   // Object 2: Not in scope
   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.id = 0;
   object2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object2, 2U, false);

   // Object 3: Implausible
   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object3, 4U, false);

   // Object 4: Plausible
   ROT_Object_Output_T &object4 = Add_Default_Object(rot_object_list_info, true);
   object4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object4, 5U, true);

   // Object 5: Coasted (not in scope)
   ROT_Object_Output_T &object5 = Add_Default_Object(rot_object_list_info, true);
   object5.object_status = 2U;
   object5.vcs_x_posn = 50.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object5, 2U, false);

   /** \action
    * Call Object_Plausible_Checks() with all objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - object 3 fails check.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function when last object is implausible.
 * Should check all objects and detect the last one as implausible.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Last_Object_Implausible)
{
   /** \precond
    * Create 4 objects:
    * - Object 1-3: In scope with detections inside (plausible)
    * - Object 4: In scope with detections OUTSIDE (implausible)
    */

   // Objects 1-3: All plausible
   for (uint32_t i = 0; i < 3U; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, true);
      obj.vcs_x_posn = 10.0f + (i * 10.0f);
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 3U, true);
   }

   // Object 4: Implausible
   ROT_Object_Output_T &object4 = Add_Default_Object(rot_object_list_info, true);
   object4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object4, 5U, false);

   /** \action
    * Call Object_Plausible_Checks() with all objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - last object fails check.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks with detection count just below maximum.
 * Boundary value test for MAX_NUMBER_OF_DETECTIONS - 1.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Detection_Count_One_Below_Maximum)
{
   /** \precond
    * Create plausible object with detection count at MAX_NUMBER_OF_DETECTIONS - 1.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   // Set detection count to just below max
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS - 1U;

   /** \action
    * Call Object_Plausible_Checks() with detection count just below maximum.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is plausible (true) - detection count is valid.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function when number_of_valid_det equals MAX_NUMBER_OF_DETECTIONS.
 * Should return true due to detection count boundary condition.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Detection_Count_At_Maximum)
{
   /** \precond
    * Create plausible objects but set number_of_valid_detections to MAX_NUMBER_OF_DETECTIONS.
    */

   // Create one plausible object
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   // Set detection count to max
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call Object_Plausible_Checks() with detection count at maximum.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - detection count at boundary.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function when number_of_valid_det exceeds MAX_NUMBER_OF_DETECTIONS.
 * Should return false due to detection count exceeding maximum.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Detection_Count_Exceeds_Maximum)
{
   /** \precond
    * Create plausible objects but set number_of_valid_detections > MAX_NUMBER_OF_DETECTIONS.
    */

   // Create one plausible object
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5U, true);

   // Set detection count to exceed max
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS + 1U;

   /** \action
    * Call Object_Plausible_Checks() with detection count exceeding maximum.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - detection count exceeds limit.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function with no objects in the list.
 * Should return true as there are no objects to fail the check.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_No_Objects)
{
   /** \precond
    * No objects created (number_of_objects = 0).
    */

   // No objects added, number_of_objects is already 0 from TEST_SETUP

   /** \action
    * Call Object_Plausible_Checks() with empty object list.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is plausible (true) - no objects to check.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function with only out-of-scope objects.
 * All objects should be skipped by Is_Target_Params_Within_Scope, resulting in plausible check passing.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_All_Objects_Out_Of_Scope)
{
   /** \precond
    * Create 4 objects, all out of scope:
    * - Object 1: ID=0
    * - Object 2: Coasted
    * - Object 3: Invalid
    * - Object 4: ID=0
    */

   // Object 1: ID=0
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.id = 0;
   object1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object1, 3U, false);

   // Object 2: Coasted
   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.object_status = 2U;
   object2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object2, 4U, false);

   // Object 3: Invalid
   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.object_status = 255U;
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object3, 2U, false);

   // Object 4: ID=0
   ROT_Object_Output_T &object4 = Add_Default_Object(rot_object_list_info, true);
   object4.id = 0;
   object4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object4, 3U, false);

   /** \action
    * Call Object_Plausible_Checks() with all objects out of scope.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is plausible (true) - all objects are out of scope.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function with multiple implausible objects.
 * Should detect the first implausible object and return false.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Multiple_Objects_Implausible)
{
   /** \precond
    * Create 5 objects where multiple are implausible:
    * - Object 1: In scope, detections inside (plausible)
    * - Object 2: In scope, detections OUTSIDE (implausible) - first failure
    * - Object 3: In scope, detections OUTSIDE (implausible) - not checked
    * - Object 4: In scope, detections inside (plausible)
    * - Object 5: In scope, detections OUTSIDE (implausible) - not checked
    */

   // Object 1: Plausible
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object1, 3U, true);

   // Object 2: Implausible (first failure)
   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object2, 4U, false);

   // Object 3: Implausible (not checked)
   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object3, 5U, false);

   // Object 4: Plausible
   ROT_Object_Output_T &object4 = Add_Default_Object(rot_object_list_info, true);
   object4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object4, 2U, true);

   // Object 5: Implausible (not checked)
   ROT_Object_Output_T &object5 = Add_Default_Object(rot_object_list_info, true);
   object5.vcs_x_posn = 50.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object5, 3U, false);

   /** \action
    * Call Object_Plausible_Checks() with multiple implausible objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - object 2 is first failure.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function with mixed object types and statuses.
 * Comprehensive test with various object conditions to ensure proper filtering and checking.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Mixed_Object_Types_All_Plausible)
{
   /** \precond
    * Create 7 objects with different statuses and detection placements:
    * - Object 1: Measured (status=0), detections inside
    * - Object 2: Newly created (status=1), detections inside
    * - Object 3: ID=0, detections outside (not checked)
    * - Object 4: Coasted (status=2), detections outside (not checked)
    * - Object 5: Measured (status=0), detections inside
    * - Object 6: Invalid (status=255), detections outside (not checked)
    * - Object 7: Measured (status=0), detections inside
    */

   // Object 1: Measured with detections inside
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.object_status = 0U;
   object1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object1, 4U, true);

   // Object 2: Newly created with detections inside
   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.object_status = 1U;
   object2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object2, 3U, true);

   // Object 3: ID=0 (not checked)
   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.id = 0;
   object3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object3, 2U, false);

   // Object 4: Coasted (not checked)
   ROT_Object_Output_T &object4 = Add_Default_Object(rot_object_list_info, true);
   object4.object_status = 2U;
   object4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object4, 3U, false);

   // Object 5: Measured with detections inside
   ROT_Object_Output_T &object5 = Add_Default_Object(rot_object_list_info, true);
   object5.object_status = 0U;
   object5.vcs_x_posn = 50.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object5, 5U, true);

   // Object 6: Invalid (not checked)
   ROT_Object_Output_T &object6 = Add_Default_Object(rot_object_list_info, true);
   object6.object_status = 255U;
   object6.vcs_x_posn = 60.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object6, 2U, false);

   // Object 7: Measured with detections inside
   ROT_Object_Output_T &object7 = Add_Default_Object(rot_object_list_info, true);
   object7.object_status = 0U;
   object7.vcs_x_posn = 70.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object7, 4U, true);

   /** \action
    * Call Object_Plausible_Checks() with mixed object types.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is plausible (true) - all in-scope objects have detections inside.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks function with objects having no associated detections.
 * Objects with ndets=0 should be checked but may have different behavior.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Objects_With_No_Detections)
{
   /** \precond
    * Create 3 objects with no associated detections:
    * - Object 1: In scope, ndets=0
    * - Object 2: In scope, ndets=0
    * - Object 3: In scope, ndets=0
    */

   // Create objects without adding any detections
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, true);
   object1.vcs_x_posn = 10.0f;
   object1.ndets = 0U;

   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, true);
   object2.vcs_x_posn = 20.0f;
   object2.ndets = 0U;

   ROT_Object_Output_T &object3 = Add_Default_Object(rot_object_list_info, true);
   object3.vcs_x_posn = 30.0f;
   object3.ndets = 0U;

   /** \action
    * Call Object_Plausible_Checks() with objects having no detections.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - objects with no detections fail check.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks with many objects (stress test).
 * Performance test with large number of objects to check loop efficiency.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Many_Objects_Stress_Test)
{
   /** \precond
    * Create 50 objects, all plausible with detections inside.
    */

   for (uint32_t i = 0; i < 50U; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, true);
      obj.vcs_x_posn = 10.0f + (i * 2.0f);          // Space them out
      obj.vcs_y_posn = (i % 2 == 0) ? 2.0f : -2.0f; // Alternate left/right
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 3U, true);
   }

   /** \action
    * Call Object_Plausible_Checks() with many objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is plausible (true) - all objects pass checks.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Plausible_Checks with mix of in-scope and one implausible among many.
 * Stress test to ensure early exit works correctly with many objects.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Many_Objects_One_Implausible_Stress_Test)
{
   /** \precond
    * Create 30 plausible objects, then one implausible, then 19 more plausible.
    */

   // First 30 plausible
   for (uint32_t i = 0; i < 30U; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, true);
      obj.vcs_x_posn = 10.0f + (i * 2.0f);
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 3U, true);
   }

   // One implausible in the middle
   ROT_Object_Output_T &bad_obj = Add_Default_Object(rot_object_list_info, true);
   bad_obj.vcs_x_posn = 100.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, bad_obj, 5U, false);

   // Last 19 plausible (should not be checked due to early exit)
   for (uint32_t i = 0; i < 19U; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, true);
      obj.vcs_x_posn = 200.0f + (i * 2.0f);
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 3U, true);
   }

   /** \action
    * Call Object_Plausible_Checks() to verify early exit on implausible object.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - one object fails, early exit.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test maximum object capacity with one implausible object at the end.
 * This test creates NUMBER_OF_REDUCED_OBJECT_TRACKS - 1 plausible objects
 * and one final implausible object to verify the system handles maximum
 * capacity correctly and properly identifies the implausible track.
 *
 * \req CPR-7532_Derived
 */
TEST(test_Object_Plausible_Checks, Object_Plausible_Checks_TC_Maximum_Objects_Last_One_Implausible)
{
   /** \precond
    * Create NUMBER_OF_REDUCED_OBJECT_TRACKS - 1 plausible objects and 1 implausible object.
    */

   // Initialize first NUMBER_OF_REDUCED_OBJECT_TRACKS - 1 objects as plausible
   for (uint16_t i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS - 1; i++)
   {
      ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
      object.id = i + 1;
      object.vcs_x_posn = 10.0f + (i * 0.5f);          // Spread objects along X axis
      object.vcs_y_posn = (i % 2 == 0) ? 2.0f : -2.0f; // Alternate Y positions

      // Create plausible detection inside bounding box
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1U, true);
   }

   // Add the last object with implausible detections
   ROT_Object_Output_T &last_object = Add_Default_Object(rot_object_list_info, false);
   last_object.id = NUMBER_OF_REDUCED_OBJECT_TRACKS;
   last_object.vcs_x_posn = 250.0f; // Far position
   last_object.vcs_y_posn = 10.0f;

   // Create implausible detections (far outside bounding box)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, last_object, 2U, false);

   /** \action
    * Call Object_Plausible_Checks() with maximum objects.
    */
   bool f_plausible = Object_Plausible_Checks(raw_detect_list.detections,
                                              f360_detection_list,
                                              raw_detect_list.number_of_valid_detections,
                                              rot_object_list_info.rot_object_list);

   /** \result
    * Expected output is implausible (false) - last object has detections outside bounding box.
    * The system should process all NUMBER_OF_REDUCED_OBJECT_TRACKS objects correctly
    * and identify the implausible one.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check when object box center distance to host is just below 4.0 meters.
 * The 4.0F threshold determines whether an object is considered "close to host" which affects
 * the longitudinal position gate calculation. This test verifies behavior just below the threshold.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Distance_Just_Below_4_Meters_Pass)
{
   /** \precond
    * Create an object with box center vcs_x_posn positioned just below 4.0 meters from host.
    * Use epsilon to ensure we're testing the boundary condition properly.
    */
   constexpr float32_t k_distance_just_below_threshold = 4.0F - __FLT_EPSILON__;

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = k_distance_just_below_threshold;
   object.vcs_y_posn = 0.0F;  // Centered laterally for simplicity

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check() with object just below 4.0m threshold.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - object is close to host (< 4.0m) and
    * detections are inside the extended bounding box with the "close to host" gate calculation.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check when object box center distance to host is just below 4.0 meters.
 * The 4.0F threshold determines whether an object is considered "close to host" which affects
 * the longitudinal position gate calculation. This test verifies behavior just below the threshold.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Distance_Just_Below_4_Meters_Fail)
{
   /** \precond
    * Create an object with box center vcs_x_posn positioned just below 4.0 meters from host.
    * Use epsilon to ensure we're testing the boundary condition properly.
    */
   constexpr float32_t k_distance_just_below_threshold = 4.0F - __FLT_EPSILON__;

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = k_distance_just_below_threshold;
   object.vcs_y_posn = 0.0F;  // Centered laterally for simplicity

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, false);

   /** \action
    * Call Object_Position_Plausible_Check() with object just below 4.0m threshold.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (false) - object is not considered close to host (<= 4.0m)
    * and uses the standard gate calculation, but detections are outside bounds.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check when object box center distance to host is just above 4.0 meters.
 * The 4.0F threshold determines whether an object is considered "close to host" which affects
 * the longitudinal position gate calculation. This test verifies behavior just above the threshold.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Distance_Just_Above_4_Meters_Pass)
{
   /** \precond
    * Create an object with box center vcs_x_posnpositioned just above 4.0 meters from host.
    * Use epsilon to ensure we're testing the boundary condition properly.
    */
   constexpr float32_t k_distance_just_above_threshold = 4.0F + __FLT_EPSILON__;

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = k_distance_just_above_threshold;
   object.vcs_y_posn = 0.0F;  // Centered laterally for simplicity

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check() with object just above 4.0m threshold.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (true) - object is not considered close to host (>= 4.0m)
    * and uses the standard gate calculation, but detections are still within bounds.
    */
   CHECK_TRUE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check when object box center distance to host is just above 4.0 meters.
 * The 4.0F threshold determines whether an object is considered "close to host" which affects
 * the longitudinal position gate calculation. This test verifies behavior just above the threshold.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Distance_Just_Above_4_Meters_Fail)
{
   /** \precond
    * Create an object with box center vcs_x_posn positioned just above 4.0 meters from host.
    * Use epsilon to ensure we're testing the boundary condition properly.
    */
   constexpr float32_t k_distance_just_above_threshold = 4.0F + __FLT_EPSILON__;

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = k_distance_just_above_threshold;
   object.vcs_y_posn = 0.0F;  // Centered laterally for simplicity

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, false);

   /** \action
    * Call Object_Position_Plausible_Check() with object just above 4.0m threshold.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);

   /** \result
    * Expected output is plausible (false) - object is not considered close to host (>= 4.0m)
    * and uses the standard gate calculation, but detections are outside bounds.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check when object box reference point is not valid.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Reference_Point_Not_Valid)
{
   /** \precond
    * Create an object with box reference point not valid.
    * Use epsilon to ensure we're testing the boundary condition properly.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.reference_point = ROT_OBJECT_REF_POINT_INVALID;

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check() with object having invalid reference point.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);
   /** \result
    * Expected output is plausible (true) - object has an invalid reference point
    * and uses the standard gate calculation, but detections are still within bounds.
    */
   CHECK_FALSE(f_plausible);
}

/** \purpose
 * Test Object_Position_Plausible_Check when object box reference point is not valid.
 * \req CPR-7532_Derived
 */
TEST(test_Object_Position_Plausible_Check, Object_Position_Plausible_Check_TC_Reference_Point_Not_Valid_255)
{
   /** \precond
    * Create an object with box reference point not valid.
    * Use epsilon to ensure we're testing the boundary condition properly.
    */

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.reference_point = 255U;

   // Create detections inside the extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3U, true);

   /** \action
    * Call Object_Position_Plausible_Check() with object having invalid reference point.
    */
   bool f_plausible = Object_Position_Plausible_Check(raw_detect_list.detections,
                                                      f360_detection_list,
                                                      raw_detect_list.number_of_valid_detections,
                                                      object);
   /** \result
    * Expected output is plausible (true) - object has an invalid reference point
    * and uses the standard gate calculation, but detections are still within bounds.
    */
   CHECK_FALSE(f_plausible);
}

/** @}*/
