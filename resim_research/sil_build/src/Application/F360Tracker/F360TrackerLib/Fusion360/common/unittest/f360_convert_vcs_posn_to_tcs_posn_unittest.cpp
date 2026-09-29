/** \file
   This file contains unit tests for f360_convert_vcs_posn_to_tcs_posn.cpp
*/

#include "f360_convert_vcs_posn_to_tcs_posn.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cfloat>

#include <string.h>
#include <math.h>
#include <cstdio>

using namespace f360_variant_A;

/** \defgroup  f360_convert_vcs_posn_to_tcs_posn
 *  @{
 */

/** \brief
 *  Testing of a function that converts a position from the Vehicle Coordinate System (VCS)
 *  to the Target Coordinate System (TCS).
 */

TEST_GROUP(f360_convert_vcs_posn_to_tcs_posn)
{
   /** \setup
   * Declare common variables used within all tests
   */
   const float TEST_PASS_TH = 1e-6F; // Threshold for test pass
   // Properties to be set before calling function
   float32_t vcs_x;
   float32_t vcs_y;
   float32_t target_vcs_position_x;
   float32_t target_vcs_position_y;
   // To store function output
   float32_t tcs_x;
   float32_t tcs_y;
   // Expected data
   float32_t expected_tcs_pos_x;
   float32_t expected_tcs_pos_y;

};

/**
*\purpose  Tests conversion of a position in VCS coordinates to TCS coordinates. In this test the target is pointing orthogonal to host (vcs_pointing = -90degrees).
*\req    NA
*/
TEST(f360_convert_vcs_posn_to_tcs_posn, Convert_VCS_Posn_To_TCS_Posn_Target_90_Deg_From_VCS)
{
   /** \precond
    * Target is to the left of host and oriented -90 degrees from host.
    */
   vcs_x = 1.0F;
   vcs_y = 1.0F;
   target_vcs_position_x = -0.5F;
   target_vcs_position_y = -1.5F;
   Angle vcs_pointing{F360_DEG2RAD(-90.0F)};

   // The expected position after conversion to TCS is [-2.5, 1.5].
   expected_tcs_pos_x = -2.5F;
   expected_tcs_pos_y = 1.5F;

   /** \action
    * Call function Convert_VCS_Posn_To_TCS_Posn.
    */
   Convert_VCS_Posn_To_TCS_Posn(vcs_x, vcs_y, target_vcs_position_x, target_vcs_position_y, vcs_pointing, tcs_x, tcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_x, tcs_x, TEST_PASS_TH, "Unexpected x value after conversion from VCS to TCS.");
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_y, tcs_y, TEST_PASS_TH, "Unexpected y value after conversion from VCS to TCS.");

}

/**
*\purpose  Tests conversion of a position in VCS coordinates to TCS coordinates. In this scenario the target has same position as host, but is
*          rotated by 45 degrees.
*\req    NA
*/
TEST(f360_convert_vcs_posn_to_tcs_posn, Convert_VCS_Posn_To_TCS_Posn_Target_Same_Pos_But_Rotated)
{
   /** \precond
    * Set up of initial conditions. Host and target have the same position and target is rotated 45 degrees.
    */
   vcs_x = 2.0F;
   vcs_y = 2.0F;
   target_vcs_position_x = 0.0F;
   target_vcs_position_y = 0.0F;
   Angle vcs_pointing{F360_DEG2RAD(45.0F)};

   // The expected position after conversion to TCS is [sqrtf(8), 0].
   expected_tcs_pos_x = sqrtf(8.0F);
   expected_tcs_pos_y = 0.0F;

   /** \action
    * Call function Convert_VCS_Posn_To_TCS_Posn.
    */
   Convert_VCS_Posn_To_TCS_Posn(vcs_x, vcs_y, target_vcs_position_x, target_vcs_position_y, vcs_pointing, tcs_x, tcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_x, tcs_x, TEST_PASS_TH, "Unexpected x value after conversion from VCS to TCS.");
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_y, tcs_y, TEST_PASS_TH, "Unexpected y value after conversion from VCS to TCS.");
}

/**
*\purpose  Tests conversion of a position in VCS coordinates to TCS coordinates. In this test the target has same orientation as host
*          but a different position.
*\req    NA
*/
TEST(f360_convert_vcs_posn_to_tcs_posn, Convert_VCS_Posn_To_TCS_Posn_Target_Same_Orientation)
{
   /** \precond
    * Set up of initial conditions. Host and target have the same orientation but different positions.
    */
   vcs_x = 0.0F;
   vcs_y = 2.0F;
   target_vcs_position_x = -3.0F;
   target_vcs_position_y = -1.0F;
   Angle vcs_pointing{F360_DEG2RAD(0.0F)};

   // The expected position after conversion to TCS is [3, 3].
   expected_tcs_pos_x = 3.0F;
   expected_tcs_pos_y = 3.0F;

   /** \action
    * Call function Convert_VCS_Posn_To_TCS_Posn.
    */
   Convert_VCS_Posn_To_TCS_Posn(vcs_x, vcs_y, target_vcs_position_x, target_vcs_position_y, vcs_pointing, tcs_x, tcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_x, tcs_x, TEST_PASS_TH, "Unexpected x value after conversion from VCS to TCS.");
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_y, tcs_y, TEST_PASS_TH, "Unexpected y value after conversion from VCS to TCS.");
}

/**
*\purpose  Tests conversion of a position in VCS coordinates to TCS coordinates. In this test the target and host have the same
*          position and orientation.
*\req    NA
*/
TEST(f360_convert_vcs_posn_to_tcs_posn, Convert_VCS_Posn_To_TCS_Posn_Target_Same_Pos_And_Orientation)
{
   /** \precond
    * Sets up initial conditions, host and target at same position and with the same orientation.
    */
   vcs_x = 1.0F;
   vcs_y = 1.0F;
   target_vcs_position_x = 0.0F;
   target_vcs_position_y = 0.0F;
   Angle vcs_pointing{F360_DEG2RAD(0.0F)};

   // Since the position and orientations of host and target are the same, expected position should be the same in
   // TCS and VCS, i.e. [1,1].
   expected_tcs_pos_x = 1.0F;
   expected_tcs_pos_y = 1.0F;

   /** \action
    * Call function Convert_VCS_Posn_To_TCS_Posn.
    */
   Convert_VCS_Posn_To_TCS_Posn(vcs_x, vcs_y, target_vcs_position_x, target_vcs_position_y, vcs_pointing, tcs_x, tcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_x, tcs_x, TEST_PASS_TH, "Unexpected x value after conversion from VCS to TCS.");
   DOUBLES_EQUAL_TEXT(expected_tcs_pos_y, tcs_y, TEST_PASS_TH, "Unexpected y value after conversion from VCS to TCS.");
}
/** @}*/

