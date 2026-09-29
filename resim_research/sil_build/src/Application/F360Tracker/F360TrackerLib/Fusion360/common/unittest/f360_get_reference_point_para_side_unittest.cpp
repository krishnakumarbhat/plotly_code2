/** \file
   This file contains unit tests for content of f360_get_visible_edge_para_dir.cpp file
*/

#include "f360_get_reference_point_para_side.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_get_visible_edge_para_dir
 *  @{
 */

using namespace f360_variant_A;
/** \brief
 *  Testing function that determines the visible edge in para direction
 */

TEST_GROUP(f360_get_visible_edge_para_dir)
{
   /** \setup
   * No setup required
   **/
   TEST_SETUP()
   {
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
*\purpose  Verify that correct para edge is returned when no edges are visible.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, No_Side_Visible)
{
   /** \precond
    * Set object reference point to F360_REFERENCE_POINT_CENTER
    **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_CENTER;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);


   /** \result
   * Expect no valid para edge to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_INVALID, visible_para_edge);
}

/**
*\purpose  Verify that correct para edge is returned when host is located straight to the left of the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Left_Side_Visible)
{
   /** \precond
    * Set object reference point to F360_REFERENCE_POINT_LEFT
    **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);


   /** \result
   * Expect no valid para edge to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_INVALID, visible_para_edge);
}

/**
*\purpose  Verify that correct para edge is returned when host is located straight to the right of the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Right_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_RIGHT
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_RIGHT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect no valid para edge to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_INVALID, visible_para_edge);

}

/**
*\purpose  Verify that correct para edge is returned when host is in front and to the left of the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Front_Left_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_FRONT_LEFT
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect front side to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_FRONT, visible_para_edge);
}

/**
*\purpose  Verify that correct para edge is returned when host is in front and to the right of the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Front_Right_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_FRONT_RIGHT
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect front side to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_FRONT, visible_para_edge);
}

/**
*\purpose  Verify that correct para edge is returned when host is straight in front of object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Front_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_FRONT
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect front side to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_FRONT, visible_para_edge);
}

/**
*\purpose  Verify that correct para edge is returned when host is straight behind the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Rear_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_REAR
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect rear side to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_REAR, visible_para_edge);
}

/**
*\purpose  Verify that correct para edge is returned when host is behind and to the left of the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Rear_Left_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_REAR_LEFT
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect rear side to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_REAR, visible_para_edge);
}


/**
*\purpose  Verify that correct para edge is returned when host is behind and to the right of the object.
*\req    NA
*/
TEST(f360_get_visible_edge_para_dir, Rear_Right_Side_Visible)
{
   /** \precond
   * Set object reference point to F360_REFERENCE_POINT_REAR_RIGHT
   **/
   const F360_Reference_Point_T obj_ref_pnt = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Call function
   **/
   const F360_Object_Sides_T visible_para_edge = Get_Reference_Point_Para_Side(obj_ref_pnt);

   /** \result
   * Expect rear side to be visible and valid
   **/
   CHECK_EQUAL(F360_OBJECT_SIDES_REAR, visible_para_edge);
}
/** @}*/

