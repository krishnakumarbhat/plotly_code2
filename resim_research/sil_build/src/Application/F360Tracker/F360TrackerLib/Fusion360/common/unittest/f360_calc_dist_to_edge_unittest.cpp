/** \file
   This file contains unit tests for content of f360_calc_dist_to_edge.cpp file
*/

#include "f360_calc_dist_to_edge.h"
#include "f360_constants.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_calc_dist_to_edge
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Testing of a function that calculates the distance from detections to an edge defined
*  by its two end points.
*/

TEST_GROUP(f360_calc_dist_to_edge)
{
   // Set up common variables to use within the test group.
   // 3 detections will be used in each test case.
   Point edge_pt_1;
   Point edge_pt_2;

   uint8_t nr_dets;
   Point det_pos[3];
   float32_t res[3];
   float32_t expected_res[3];

   /** \setup
   * Nothing to setup in this test group
   */
   TEST_SETUP()
   {
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
*\purpose  Verify that correct distance is returned for detections ON the edge.
*\req    NA
*/
TEST(f360_calc_dist_to_edge, Distance_To_Edge_Detections_On_Edge)
{
   /** \step{1}
    * Set values for edge end points and create 3 detections on the edge. 1 detection at each end point and 1 on the middle of the edge.
    */

   /** \precond
    * Set edge_pt_1 to (1, 1).
    * Set edge_pt_2 to (2, 1).
    * Set nr_dets to 3.
    * Create detections at (x, y) = :
    *   1. (1, 1)
    *   2. (1.5, 1)
    *   3. (2, 1)
    * Set expected results to 0.
    */
   edge_pt_1.x = 1.0F;
   edge_pt_1.y = 1.0F;
   edge_pt_2.x = 2.0F;
   edge_pt_2.y = 1.0F;

   nr_dets = 3U;

   // Detection 1
   det_pos[0].x = 1.0F;
   det_pos[0].y = 1.0F;

   // Detection 2
   det_pos[1].x = 1.5F;
   det_pos[1].y = 1.0F;

   // Detection 3
   det_pos[2].x = 2.0F;
   det_pos[2].y = 1.0F;

   expected_res[0] = 0.0F;
   expected_res[1] = 0.0F;
   expected_res[2] = 0.0F;

   /** \action
    * Call function
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      res[i] = Calculate_Distance_To_Edge(edge_pt_1, edge_pt_2, det_pos[i]);
   }

   /** \result
    * Expect the distance from each detection to the edge to be 0.
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_res[i], res[i], F360_EPSILON, "The returned distance to edge was not 0 for detections on the edge.")
   }
}

/**
*\purpose  Verify that correct distance is returned for detections that are not on the edge.
*\req    NA
*/
TEST(f360_calc_dist_to_edge, Distance_To_Edge_Detections_Not_On_Edge)
{
   /** \step{1}
    * Set values for edge end points.
    * Create 1 detection that falls within the line segment spanned by the edge (orthogonal distance is shortest distance).
    * Create 2 detection that falls outside the line segment spanned by the edge (distance to end point is shortest distance).
    */

   /** \precond
    * Set edge_pt_1 to (1, 1).
    * Set edge_pt_2 to (3, 3).
    * Set nr_dets to 3.
    * Create detections at (x, y) = :
    *   1. (2, 1)
    *   2. (0, -2)
    *   3. (4, 5)
    */
   edge_pt_1.x = 1.0F;
   edge_pt_1.y = 1.0F;
   edge_pt_2.x = 3.0F;
   edge_pt_2.y = 3.0F;

   nr_dets = 3U;

   // Detection 1
   det_pos[0].x = 2.0F;
   det_pos[0].y = 1.0F;

   // Detection 2
   det_pos[1].x = 0.0F;
   det_pos[1].y = -2.0F;

   // Detection 3
   det_pos[2].x = 4.0F;
   det_pos[2].y = 5.0F;

   // Expected results derived by hand
   expected_res[0] = 0.70710678F;  // 1.0f / sqrtf(2.0f);
   expected_res[1] = 3.16227766F;  // sqrtf(10.0f);
   expected_res[2] = 2.23606797F;  // sqrtf(5.0f);

   /** \action
    * Call function
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      res[i] = Calculate_Distance_To_Edge(edge_pt_1, edge_pt_2, det_pos[i]);
   }

   /** \result
    * Expect the distance from each detection to the edge to be the value in expected_res[i].
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_res[i], res[i], F360_EPSILON, "The returned distance to edge did not match the expected distance.")
   }
}
/**
*\purpose  Verify that correct distance is returned for detections that are on the right side of edge.
*\req    NA
*/
TEST(f360_calc_dist_to_edge, Distance_To_Edge_Detections_On_The_Right_Side_Of_Edge)
{
   /** \step{1}
    * Set values for edge end points.
    * Create 1 detection that falls within the line segment spanned and on the right side of the edge (orthogonal distance is shortest distance).
    * Create 2 detection that falls outside the line segment spanned on the right side of the edge (distance to end point is shortest distance).
    * Create 3 detection that falls outside the line segment spanned on the right side of the edge (distance to end point is shortest distance).
    */

    /** \precond
     * Set edge_pt_1 to (1, 1).
     * Set edge_pt_2 to (3, 1).
     * Set nr_dets to 3.
     * Create detections at (x, y) = :
     *   1. (1, 2)
     *   2. (2, 2)
     *   3. (2.5, 2)
     */
   edge_pt_1.x = 1.0F;
   edge_pt_1.y = 1.0F;
   edge_pt_2.x = 3.0F;
   edge_pt_2.y = 1.0F;

   nr_dets = 3U;

   // Detection 1
   det_pos[0].x = 1.0F;
   det_pos[0].y = 2.0F;

   // Detection 2
   det_pos[1].x = 2.0F;
   det_pos[1].y = 2.0F;

   // Detection 3
   det_pos[2].x = 2.5F;
   det_pos[2].y = 2.0F;

   // Expected results derived by hand
   expected_res[0] = 1.0F;  // sqrtf(1.0f);
   expected_res[1] = 1.0F;  // sqrtf(1.0f);
   expected_res[2] = 1.0F;  // sqrtf(1.0f);

   /** \action
    * Call function
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      res[i] = Calculate_Distance_To_Edge(edge_pt_1, edge_pt_2, det_pos[i]);
   }

   /** \result
    * Expect the distance from each detection to the edge to be the value in expected_res[i].
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_res[i], res[i], F360_EPSILON, "The returned distance to edge did not match the expected distance.")
   }
}

/**
*\purpose  Verify that correct distance is returned for detections that are on the left side of edge.
*\req    NA
*/
TEST(f360_calc_dist_to_edge, Distance_To_Edge_Detections_On_The_Left_Side_Of_Edge)
{
   /** \step{1}
    * Set values for edge end points.
    * Create 1 detection that falls within the line segment spanned and on the left side of the edge (orthogonal distance is shortest distance).
    * Create 2 detection that falls outside the line segment spanned on the left side of the edge (distance to end point is shortest distance).
    * Create 3 detection that falls outside the line segment spanned on the left side of the edge (distance to end point is shortest distance).
    */

    /** \precond
     * Set edge_pt_1 to (1, 1).
     * Set edge_pt_2 to (3, 1).
     * Set nr_dets to 3.
     * Create detections at (x, y) = :
     *   1. (1, -2)
     *   2. (2, -2)
     *   3. (2.5, -2)
     */
   edge_pt_1.x = 1.0F;
   edge_pt_1.y = 1.0F;
   edge_pt_2.x = 3.0F;
   edge_pt_2.y = 1.0F;

   nr_dets = 3U;

   // Detection 1
   det_pos[0].x = 1.0F;
   det_pos[0].y = -2.0F;

   // Detection 2
   det_pos[1].x = 2.0F;
   det_pos[1].y = -2.0F;

   // Detection 3
   det_pos[2].x = 4.0F;
   det_pos[2].y = -2.0F;

   // Expected results derived by hand
   expected_res[0] = 3.0F;  // sqrtf(9.0f);
   expected_res[1] = 3.0F;  // sqrtf(9.0f);
   expected_res[2] = 3.16227770F;  // sqrtf(10.0f);

   /** \action
    * Call function
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      res[i] = Calculate_Distance_To_Edge(edge_pt_1, edge_pt_2, det_pos[i]);
   }

   /** \result
    * Expect the distance from each detection to the edge to be the value in expected_res[i].
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_res[i], res[i], F360_EPSILON, "The returned distance to edge did not match the expected distance.")
   }
}
/**
*\purpose  Verify that correct distance is returned when edge points are close to each other.
*\req    NA
*/
TEST(f360_calc_dist_to_edge, Edge_Points_Are_Close_To_Each_Other)
{
   /** \step{1}
    * Set values for edge end points close to each other.
    * Create 1 detection that falls within the line segment spanned and on the left side of the edge (orthogonal distance is shortest distance).
    * Create 2 detection that falls outside the line segment spanned on the left side of the edge (distance to end point is shortest distance).
    * Create 3 detection that falls outside the line segment spanned on the left side of the edge (distance to end point is shortest distance).
    */

    /** \precond
     * Set edge_pt_1 to (1, 1).
     * Set edge_pt_2 to (1.00001, 1).
     * Set nr_dets to 3.
     * Create detections at (x, y) = :
     *   1. (1, -2)
     *   2. (2, -2)
     *   3. (2.5, -2)
     */
   edge_pt_1.x = 1.0F;
   edge_pt_1.y = 1.0F;
   edge_pt_2.x = 1.00001F;
   edge_pt_2.y = 1.0F;

   nr_dets = 3U;

   // Detection 1
   det_pos[0].x = 1.0F;
   det_pos[0].y = -2.0F;

   // Detection 2
   det_pos[1].x = 2.0F;
   det_pos[1].y = -2.0F;

   // Detection 3
   det_pos[2].x = 2.5F;
   det_pos[2].y = -2.0F;

   // Expected results derived by hand
   expected_res[0] = -1.0F;
   expected_res[1] = -1.0F;
   expected_res[2] = -1.0F;

   /** \action
    * Call function
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      res[i] = Calculate_Distance_To_Edge(edge_pt_1, edge_pt_2, det_pos[i]);
   }

   /** \result
    * Expect the distance from each detection to the edge to be the value in expected_res[i].
    */
   for (uint8_t i = 0; i < nr_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_res[i], res[i], F360_EPSILON, "The returned distance to edge did not match the expected distance.")
   }
}
/** @}*/

