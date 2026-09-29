/** \file
   This file contains unit tests for content of f360_get_edge_tcs_start_and_end_point.cpp file
   The function under test contains a switch statement with a default branch that never can be reached.
   Thus it's impossible to get 100% coverage of this function.
*/

#include "f360_get_edge_tcs_start_and_end_point.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_get_edge_as_tcs_2d_points
 *  @{
 */

using namespace f360_variant_A;
/** \brief
 *  Standard basic test group containing common variables for all test cases.
 */

TEST_GROUP(f360_get_edge_tcs_start_and_end_point)
{
   /** \setup
   * Setup basic properties needed on object struct and declare needed variables.
   */
   F360_Object_Track_T obj;
   F360_Object_Sides_T edge;
   Point pt1_tcs;
   Point pt2_tcs;
   bool f_valid_edge;

   TEST_SETUP()
   {
      obj.bbox.Set_Length(1.0F);
      obj.bbox.Set_Width(1.0F);
      f_valid_edge = false;
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
*\purpose  Verify that correct points are returned for this given edge.
*\req    NA
*/
TEST(f360_get_edge_tcs_start_and_end_point, Test_Edge_Front)
{

   /** \precond
    * Set edge to Front
    */
   edge = F360_OBJECT_SIDES_FRONT;

   /** \action
    * Call function
    */
   f_valid_edge = Get_Edge_TCS_Start_And_End_Point(obj, edge, pt1_tcs, pt2_tcs);

   /** \result
    * Compare to expected data
    */
   CHECK_TRUE(f_valid_edge);
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Length(), pt1_tcs.x, F360_EPSILON, "Para coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Width(), pt1_tcs.y, F360_EPSILON, "Ortho coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Length(), pt2_tcs.x, F360_EPSILON, "Para coordinate of point 2 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Width(), pt2_tcs.y, F360_EPSILON, "Ortho coordinate of point 2 does not match expected data");
}

/**
*\purpose  Verify that correct points are returned for this given edge.
*\req    NA
*/
TEST(f360_get_edge_tcs_start_and_end_point, Test_Edge_Rear)
{

   /** \precond
   * Set edge to Rear
   */
   edge = F360_OBJECT_SIDES_REAR;

   /** \action
   * Call function
   */
   f_valid_edge = Get_Edge_TCS_Start_And_End_Point(obj, edge, pt1_tcs, pt2_tcs);

   /** \result
   * Compare to expected data
   */
   CHECK_TRUE(f_valid_edge);
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Length(), pt1_tcs.x, F360_EPSILON, "Para coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Width(), pt1_tcs.y, F360_EPSILON, "Ortho coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Length(), pt2_tcs.x, F360_EPSILON, "Para coordinate of point 2 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Width(), pt2_tcs.y, F360_EPSILON, "Ortho coordinate of point 2 does not match expected data");
}

/**
*\purpose  Verify that correct points are returned for this given edge.
*\req    NA
*/
TEST(f360_get_edge_tcs_start_and_end_point, Test_Edge_Left)
{

   /** \precond
   * Set edge to Left
   */
   edge = F360_OBJECT_SIDES_LEFT;

   /** \action
   * Call function
   */
   f_valid_edge = Get_Edge_TCS_Start_And_End_Point(obj, edge, pt1_tcs, pt2_tcs);

   /** \result
   * Compare to expected data
   */
   CHECK_TRUE(f_valid_edge);
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Length(), pt1_tcs.x, F360_EPSILON, "Para coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Width(), pt1_tcs.y, F360_EPSILON, "Ortho coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Length(), pt2_tcs.x, F360_EPSILON, "Para coordinate of point 2 does not match expected data");
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Width(), pt2_tcs.y, F360_EPSILON, "Ortho coordinate of point 2 does not match expected data");
}

/**
*\purpose  Verify that correct points are returned for this given edge.
*\req    NA
*/
TEST(f360_get_edge_tcs_start_and_end_point, Test_Edge_Right)
{

   /** \precond
   * Set edge to Right
   */
   edge = F360_OBJECT_SIDES_RIGHT;

   /** \action
   * Call function
   */
   f_valid_edge = Get_Edge_TCS_Start_And_End_Point(obj, edge, pt1_tcs, pt2_tcs);

   /** \result
   * Compare to expected data
   */
   CHECK_TRUE(f_valid_edge);
   DOUBLES_EQUAL_TEXT(-0.5F * obj.bbox.Get_Length(), pt1_tcs.x, F360_EPSILON, "Para coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Width(), pt1_tcs.y, F360_EPSILON, "Ortho coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F *obj.bbox.Get_Length(), pt2_tcs.x, F360_EPSILON, "Para coordinate of point 2 does not match expected data");
   DOUBLES_EQUAL_TEXT(0.5F * obj.bbox.Get_Width(), pt2_tcs.y, F360_EPSILON, "Ortho coordinate of point 2 does not match expected data");
}

/**
*\purpose  Verify that correct points are returned for this given edge.
*\req    NA
*/
TEST(f360_get_edge_tcs_start_and_end_point, Test_Edge_Invalid)
{

   /** \precond
   * Set edge to Invalid and init flag to true
   */
   edge = F360_OBJECT_SIDES_INVALID;
   f_valid_edge = true;

   /** \action
   * Call function
   */
   f_valid_edge = Get_Edge_TCS_Start_And_End_Point(obj, edge, pt1_tcs, pt2_tcs);

   /** \result
   * Compare to expected data
   */
   CHECK_FALSE(f_valid_edge);
   DOUBLES_EQUAL_TEXT(INFTY, pt1_tcs.x, F360_EPSILON, "Para coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(INFTY, pt1_tcs.y, F360_EPSILON, "Ortho coordinate of point 1 does not match expected data");
   DOUBLES_EQUAL_TEXT(INFTY, pt2_tcs.x, F360_EPSILON, "Para coordinate of point 2 does not match expected data");
   DOUBLES_EQUAL_TEXT(INFTY, pt2_tcs.y, F360_EPSILON, "Ortho coordinate of point 2 does not match expected data");
}

/** @}*/

