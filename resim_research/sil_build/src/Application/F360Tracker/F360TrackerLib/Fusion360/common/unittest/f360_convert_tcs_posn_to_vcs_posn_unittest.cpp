/** \file
   This file contains unit tests for f360_convert_tcs_posn_to_vcs_posn.cpp
*/

#include "f360_convert_tcs_posn_to_vcs_posn.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cfloat>

#include <string.h>
#include <math.h>
#include <cstdio>

using namespace f360_variant_A;

/** \defgroup  f360_convert_tcs_posn_to_vcs_posn
 *  @{
 */

/** \brief
 *  Testing of a function that converts a position from the Target Coordinate System (TCS)
 *  to the Vehicle Coordinate System (VCS).
 */

TEST_GROUP(f360_convert_tcs_posn_to_vcs_posn)
{
   /** \setup
   * Declare common variables used within all tests
   */
   const float TEST_PASS_TH = 1e-5F; // Threshold for test pass
   // Properties to be set before calling function
   float32_t tcs_x;
   float32_t tcs_y;
   float32_t target_vcs_pos_x;
   float32_t target_vcs_pos_y;
   // To store function output
   float32_t vcs_x;
   float32_t vcs_y;
   // Expected data
   float32_t expected_vcs_x;
   float32_t expected_vcs_y;

};

/**
*\purpose  Test that conversion function works as expected when target is aligned with VCS.
*\req    NA
*/
TEST(f360_convert_tcs_posn_to_vcs_posn, Convert_TCS_Posn_To_VCS_Posn_Target_Aligned_With_VCS)
{
   /** \precond
    * Set:
    *  target vcs_pointing = 0
    *  target vcs x position to -10.0F
    *  target vcs y positon to 2
    *  target tcs pos to (x,y) = (5, 2)
    *  expected data
    */
   Angle target_vcs_pointing{F360_DEG2RAD(0.0F)};
   target_vcs_pos_x = -10.0F;
   target_vcs_pos_y = 2.0F;

   tcs_x = 5.0F;
   tcs_y = 2.0F;

   expected_vcs_x = -5.0F;
   expected_vcs_y = 4.0F;
   /** \action
    * Call function
    */
   Convert_TCS_Posn_To_VCS_Posn(tcs_x, tcs_y, target_vcs_pos_x, target_vcs_pos_y, target_vcs_pointing, vcs_x, vcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_vcs_x, vcs_x, TEST_PASS_TH, "Unexpected x value after conversion from TCS to VCS.");
   DOUBLES_EQUAL_TEXT(expected_vcs_y, vcs_y, TEST_PASS_TH, "Unexpected y value after conversion from TCS to VCS.");
}

/**
*\purpose  Test that conversion function works as expected when target is pointing orthogonal to VCS (vcs_pointing = -90degrees).
*\req    NA
*/
TEST(f360_convert_tcs_posn_to_vcs_posn, Convert_TCS_Posn_To_VCS_Posn_Target_Minus_90_Deg_From_VCS)
{
   /** \precond
    * Set:
    *  target vcs_pointing = -90 degrees
    *  target vcs x position to 0.0F
    *  target vcs y positon to -10.0F
    *  target tcs pos to (x,y) = (5, 2)
    *  expected data
    */
   Angle target_vcs_pointing{F360_DEG2RAD(-90.0F)};
   target_vcs_pos_x = 0.0F;
   target_vcs_pos_y = -10.0F;

   tcs_x = 5.0F;
   tcs_y = 2.0F;

   expected_vcs_x = 2.0F;
   expected_vcs_y = -15.0F;
   /** \action
    * Call function
    */
   Convert_TCS_Posn_To_VCS_Posn(tcs_x, tcs_y, target_vcs_pos_x, target_vcs_pos_y, target_vcs_pointing, vcs_x, vcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_vcs_x, vcs_x, TEST_PASS_TH, "Unexpected x value after conversion from TCS to VCS.");
   DOUBLES_EQUAL_TEXT(expected_vcs_y, vcs_y, TEST_PASS_TH, "Unexpected y value after conversion from TCS to VCS.");
}

/**
*\purpose  Test that conversion function works as expected when target is rotated 180 degrees compared to VCS.
*\req    NA
*/
TEST(f360_convert_tcs_posn_to_vcs_posn, Convert_TCS_Posn_To_VCS_Posn_Target_180_Deg_From_VCS)
{
   /** \precond
    * Set:
    *  target vcs_pointing = 180 degrees
    *  target vcs x position to -6.0F
    *  target vcs y positon to 2.0F
    *  target tcs pos to (x,y) = (3, 1)
    *  expected data
    */
   Angle target_vcs_pointing{F360_DEG2RAD(180.0F)};
   target_vcs_pos_x = -6.0F;
   target_vcs_pos_y = 2.0F;

   tcs_x = 3.0F;
   tcs_y = 1.0F;

   expected_vcs_x = -9.0F;
   expected_vcs_y = 1.0F;
   /** \action
    * Call function
    */
   Convert_TCS_Posn_To_VCS_Posn(tcs_x, tcs_y, target_vcs_pos_x, target_vcs_pos_y, target_vcs_pointing, vcs_x, vcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_vcs_x, vcs_x, TEST_PASS_TH, "Unexpected x value after conversion from TCS to VCS.");
   DOUBLES_EQUAL_TEXT(expected_vcs_y, vcs_y, TEST_PASS_TH, "Unexpected y value after conversion from TCS to VCS.");
}

/**
*\purpose  Test that conversion function works as expected when target is rotated 120 degrees compared to VCS.
*\req    NA
*/
TEST(f360_convert_tcs_posn_to_vcs_posn, Convert_TCS_Posn_To_VCS_Posn_Target_120_Deg_From_VCS)
{
   /** \precond
    * Set:
    *  target vcs_pointing = 120 degrees
    *  target vcs x position to -10.0F
    *  target vcs y positon to 3.0F
    *  target tcs pos to (x,y) = (-1, -2)
    *  expected data
    */
   Angle target_vcs_pointing{F360_DEG2RAD(120.0F)};
   target_vcs_pos_x = -10.0F;
   target_vcs_pos_y = 3.0F;

   tcs_x = -1.0F;
   tcs_y = -2.0F;

   // Expected values computed externally in matlab
   expected_vcs_x = -7.767949192431123F;
   expected_vcs_y = 3.133974596215561F;

   /** \action
    * Call function
    */
   Convert_TCS_Posn_To_VCS_Posn(tcs_x, tcs_y, target_vcs_pos_x, target_vcs_pos_y, target_vcs_pointing, vcs_x, vcs_y);

   /** \result
    * Ensure function output is equal to the expected result.
    */
   DOUBLES_EQUAL_TEXT(expected_vcs_x, vcs_x, TEST_PASS_TH, "Unexpected x value after conversion from TCS to VCS.");
   DOUBLES_EQUAL_TEXT(expected_vcs_y, vcs_y, TEST_PASS_TH, "Unexpected y value after conversion from TCS to VCS.");
}
/** @}*/

