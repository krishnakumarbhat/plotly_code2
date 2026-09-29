/** \file
 * This file contains unit tests for content of f360_get_track_bbox_in_vcs.cpp file
 */

#include "f360_get_track_bbox_in_vcs.h"
#include <CppUTest/TestHarness.h>
#include "f360_math.h"

//#include "headerfile_needed.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_get_tcs_box_corners_in_vcs
 *  @{
 */

/** \brief
 * Testing of a function that finds coordinates of a TCS box's corners in VCS.
 * In this test group a scenario is set up to test that function returns correct values
 * for objects when pointing angle coincides with host pointing.
 */
TEST_GROUP(f360_get_tcs_box_corners_in_vcs)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj = {};
   float32_t box_tcs[2][2] = {};
   BboxCorners box_vcs = {};

   float32_t TEST_PASS_TH = 1e-6F;

   Point expected_front_left_corner_vcs;
   Point expected_front_right_corner_vcs;
   Point expected_rear_left_corner_vcs;
   Point expected_rear_right_corner_vcs;
   /** \setup
    * Set up a default object with zero pointing angle in VCS
    */
   TEST_SETUP()
   {
      // Place an object in front of host with same pointing as host
      obj.vcs_position.x = 4.0F;
      obj.vcs_position.y = 6.0F;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });

      // Define a box in TCS
      box_tcs[0][0] = 2.0F;
      box_tcs[0][1] = 6.0F;
      box_tcs[1][0] = -3.0F;
      box_tcs[1][1] = -1.0F;

      // Expected data
      expected_front_left_corner_vcs = { 10.0F, 3.0F };
      expected_front_right_corner_vcs = { 10.0F, 5.0F };
      expected_rear_left_corner_vcs = { 6.0F, 3.0F };
      expected_rear_right_corner_vcs = { 6.0F, 5.0F };
   }
};

/** \purpose Test that Get_TCS_Box_Corners_In_VCS correctly returns the VCS corners of
*            a box defined in TCS with object having pointing aligned with host.
* \req NA
*/
TEST(f360_get_tcs_box_corners_in_vcs, Get_TCS_Box_Corners_In_VCS_Zero_Pointing)
{
   /** \precond
   * An object and a box defined in its TCS have been set up.
   */

   /** \action
   * Call Get_TCS_Box_Corners_In_VCS()
   */
   BboxCorners vcs_corners = Get_TCS_Box_Corners_In_VCS(obj, box_tcs);

   /** \result
   * Check that position of VCS box's corners are correct
   */
   DOUBLES_EQUAL_TEXT(expected_front_left_corner_vcs.x, vcs_corners.Front_Left().x,
      TEST_PASS_TH, "Longitudinal position of object's front left corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_front_left_corner_vcs.y, vcs_corners.Front_Left().y,
      TEST_PASS_TH, "Lateral position of object's rear front corner was incorrect.");

   DOUBLES_EQUAL_TEXT(expected_front_right_corner_vcs.x, vcs_corners.Front_Right().x,
      TEST_PASS_TH, "Longitudinal position of object's front right corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_front_left_corner_vcs.y, vcs_corners.Front_Left().y,
      TEST_PASS_TH, "Lateral position of object's front left right was incorrect.");

   DOUBLES_EQUAL_TEXT(expected_rear_left_corner_vcs.x, vcs_corners.Rear_Left().x,
      TEST_PASS_TH, "Longitudinal position of object's rear left corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_rear_left_corner_vcs.y, vcs_corners.Rear_Left().y,
      TEST_PASS_TH, "Lateral position of object's rear left corner was incorrect.");

   DOUBLES_EQUAL_TEXT(expected_rear_right_corner_vcs.x, vcs_corners.Rear_Right().x,
      TEST_PASS_TH, "Longitudinal position of object's rear right corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_rear_right_corner_vcs.y, vcs_corners.Rear_Right().y,
      TEST_PASS_TH, "Lateral position of object's rear right corner was incorrect.");
}

/** \purpose Test that Get_TCS_Box_Corners_In_VCS correctly returns the VCS corners of
*            a box defined in TCS with object having 90 degrees vcs pointing.
* \req NA
*/
TEST(f360_get_tcs_box_corners_in_vcs, Get_TCS_Box_Corners_In_VCS_90_Deg_Pointing)
{
   /** \precond
   * An object and a box defined in its TCS have been set up.
   * Set object poiting to pi/2.
   * Set box min/max values such that it's identical to in previous test, taking new object poiting into consideration.
   */
   obj.Set_Bbox_Orientation(Angle{ F360_PI_2 });

   box_tcs[0][0] = -3.0F;
   box_tcs[0][1] = -1.0F;
   box_tcs[1][0] = -6.0F;
   box_tcs[1][1] = -2.0F;

   /** \action
   * Call Get_TCS_Box_Corners_In_VCS()
   */
   BboxCorners vcs_corners = Get_TCS_Box_Corners_In_VCS(obj, box_tcs);

   /** \result
   * Check that position of VCS box's corners are correct. Note that since the orientation of the object is changed
   * while the box's isn't, the mapping of the corners is changed.
   */
   DOUBLES_EQUAL_TEXT(expected_front_right_corner_vcs.x, vcs_corners.Front_Left().x,
      TEST_PASS_TH, "Longitudinal position of object's front left corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_front_right_corner_vcs.y, vcs_corners.Front_Left().y,
      TEST_PASS_TH, "Lateral position of object's front left corner was incorrect.");

   DOUBLES_EQUAL_TEXT(expected_rear_right_corner_vcs.x, vcs_corners.Front_Right().x,
      TEST_PASS_TH, "Longitudinal position of object's front right corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_rear_right_corner_vcs.y, vcs_corners.Front_Right().y,
      TEST_PASS_TH, "Lateral position of object's front right corner was incorrect.");

   DOUBLES_EQUAL_TEXT(expected_front_left_corner_vcs.x, vcs_corners.Rear_Left().x,
      TEST_PASS_TH, "Longitudinal position of object's rear left corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_front_left_corner_vcs.y, vcs_corners.Rear_Left().y,
      TEST_PASS_TH, "Lateral position of object's rear left corner was incorrect.");

   DOUBLES_EQUAL_TEXT(expected_rear_left_corner_vcs.x, vcs_corners.Rear_Right().x,
      TEST_PASS_TH, "Longitudinal position of object's rear right corner was incorrect.");
   DOUBLES_EQUAL_TEXT(expected_rear_left_corner_vcs.y, vcs_corners.Rear_Right().y,
      TEST_PASS_TH, "Lateral position of object's rear right corner was incorrect.");

}

/** \defgroup  f360_get_min_and_max_of_vcs_box
 *  @{
 */

/** \brief
 * Testing of function that finds min and max longitudinal and lateral values of a box.
 */
TEST_GROUP(f360_get_min_and_max_of_vcs_box)
{
   // Declare common variables used within all tests in this test group.
   BboxCorners vcs_corners = {};
   float32_t min_max_vals[2][2] = {};
   float32_t exp_min_max_vals[2][2] = {};

   float32_t TEST_PASS_TH = 1e-6F;

   /** \setup
    * Set up a default box and expected return values.
    */
   TEST_SETUP()
   {
      vcs_corners.Front_Left() = {5, 3};
      vcs_corners.Front_Right() = {4, 4};
      vcs_corners.Rear_Left() = {3, 1};
      vcs_corners.Rear_Right() = {1, 1};

      exp_min_max_vals[0][0] = 1.0F;
      exp_min_max_vals[0][1] = 5.0F;
      exp_min_max_vals[1][0] = 1.0F;
      exp_min_max_vals[1][1] = 4.0F;
   }
};

/** \purpose Test that Get_Min_And_Max_Of_VCS_Box correctly returns the min and max
 *           longitudinal and lateral values of a given box.
* \req NA
*/
TEST(f360_get_min_and_max_of_vcs_box, Get_Min_And_Max_Of_VCS_Box)
{
   /** \action
   * Call Get_Min_And_Max_Of_VCS_Box()
   */
   Get_Min_And_Max_Of_VCS_Box(vcs_corners, min_max_vals);

   /** \result
   * Check that min and max values are correct.
   */
   DOUBLES_EQUAL_TEXT(exp_min_max_vals[0][0], min_max_vals[0][0],
         TEST_PASS_TH, "Incorrect longitudinal min value.");
   DOUBLES_EQUAL_TEXT(exp_min_max_vals[0][1], min_max_vals[0][1],
         TEST_PASS_TH, "Incorrect longitudinal max value.");
   DOUBLES_EQUAL_TEXT(exp_min_max_vals[1][0], min_max_vals[1][0],
         TEST_PASS_TH, "Incorrect lateral min value.");
   DOUBLES_EQUAL_TEXT(exp_min_max_vals[1][1], min_max_vals[1][1],
         TEST_PASS_TH, "Incorrect lateral max value.");

}
/** @}*/
