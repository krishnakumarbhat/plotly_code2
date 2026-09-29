/** \file
 * This file contains unit tests for content of Find_Intersections_in_TCS__functionality.cpp file
 */

#include "f360_intersections.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Find_Intersections_in_TCS__functionality
 *  @{
 */

/** \brief
 * Test group for Find_Intersections_in_TCS() function.
 */
TEST_GROUP(Find_Intersections_in_TCS__functionality)
{	
   F360_Object_Track_T object;
   const float tolerance = 0.00001F;
   
   /** \setup
    * Setup object size
    */
   TEST_SETUP()
   {
      object.bbox.Set_Length(6.0F); 
      object.bbox.Set_Width(3.0F); 

   }
};

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 0 intersections
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, no_intersection)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs ({ 1.0F, 3.0F }, 0.9F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */
   CHECK_EQUAL(0U, num_intersections);
}

/** \purpose  
 * Check if Find_Intersections_in_TCS() returns 1 correct intersection point (zero radius case)
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, one_intersection__zero_radius)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({ 1.0F, 1.5F }, 0.0F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};
	
   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */	
   CHECK_EQUAL(1U, num_intersections);
   DOUBLES_EQUAL(1.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(1.5F, intersections_tcs[0].y, tolerance);
}

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 1 correct intersection point
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, one_intersection)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({ 1.0F, 2.5F }, 1.0F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */
   CHECK_EQUAL(1U, num_intersections);
   DOUBLES_EQUAL(1.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(1.5F, intersections_tcs[0].y, tolerance);
}

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 2 correct intersection points (case 1)
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, two_intersections__case_1)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({0.0F, 3.5F }, 2.236068F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */
   CHECK_EQUAL(2U, num_intersections);
   DOUBLES_EQUAL(1.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(1.5F, intersections_tcs[0].y, tolerance);
   DOUBLES_EQUAL(-1.0F, intersections_tcs[1].x, tolerance);
   DOUBLES_EQUAL(1.5F, intersections_tcs[1].y, tolerance);
}

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 2 correct intersection points (case 2)
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, two_intersections__case_2)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({ 2.0F, -2.0F }, 2.7F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */
   CHECK_EQUAL(2U, num_intersections);
   DOUBLES_EQUAL(3.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(0.50799F, intersections_tcs[0].y, tolerance);
   DOUBLES_EQUAL(-0.65330F, intersections_tcs[1].x, tolerance);
   DOUBLES_EQUAL(-1.5F, intersections_tcs[1].y, tolerance);
}

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 3 correct intersection points
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, three_intersections)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({ 0.0F, 3.5F }, 5.0F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */
   CHECK_EQUAL(3U, num_intersections);
   DOUBLES_EQUAL(3.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(-0.5F, intersections_tcs[0].y, tolerance);
   DOUBLES_EQUAL(-3.0F, intersections_tcs[1].x, tolerance);
   DOUBLES_EQUAL(-0.5F, intersections_tcs[1].y, tolerance);
   DOUBLES_EQUAL(0.0, intersections_tcs[2].x, tolerance);
   DOUBLES_EQUAL(-1.5F, intersections_tcs[2].y, tolerance);
}

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 4 correct intersection points
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, four_intersections)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({ 0.0F, 3.5F }, 5.7F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections
    */
   CHECK_EQUAL(4U, num_intersections);
   DOUBLES_EQUAL(3.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(-1.346648F, intersections_tcs[0].y, tolerance);
   DOUBLES_EQUAL(-3.0F, intersections_tcs[1].x, tolerance);
   DOUBLES_EQUAL(-1.346648F, intersections_tcs[1].y, tolerance);
   DOUBLES_EQUAL(2.736786F, intersections_tcs[2].x, tolerance);
   DOUBLES_EQUAL(-1.5F, intersections_tcs[2].y, tolerance);
   DOUBLES_EQUAL(-2.736786F, intersections_tcs[3].x, tolerance);
   DOUBLES_EQUAL(-1.5F, intersections_tcs[3].y, tolerance);
}

/** \purpose
 * Check if Find_Intersections_in_TCS() returns 8 correct intersection points
 * \req
 * NA
 */
TEST(Find_Intersections_in_TCS__functionality, eight_intersections)
{
   /** \precond
    * Setup circle params
    */
   Circle circle_tcs({ 0.0F, 0.5F }, 3.1F);
   Point intersections_tcs[Max_Num_Intersections_Circle_Rectangle] = {};

   /** \action
    * Call Find_Intersections_in_TCS()
    */
   unsigned num_intersections = Find_Intersections_in_TCS(circle_tcs, object, intersections_tcs);

   /** \result
    * Check intersections  
    */
   CHECK_EQUAL(8U, num_intersections);
   DOUBLES_EQUAL(3.0F, intersections_tcs[0].x, tolerance);
   DOUBLES_EQUAL(1.281025F, intersections_tcs[0].y, tolerance);
   DOUBLES_EQUAL(3.0F, intersections_tcs[1].x, tolerance);
   DOUBLES_EQUAL(-0.281025F, intersections_tcs[1].y, tolerance);
   DOUBLES_EQUAL(-3.0F, intersections_tcs[2].x, tolerance);
   DOUBLES_EQUAL(1.281025F, intersections_tcs[2].y, tolerance);
   DOUBLES_EQUAL(-3.0F, intersections_tcs[3].x, tolerance);
   DOUBLES_EQUAL(-0.281025F, intersections_tcs[3].y, tolerance);
   DOUBLES_EQUAL(2.934280F, intersections_tcs[4].x, tolerance);
   DOUBLES_EQUAL(1.5F, intersections_tcs[4].y, tolerance);
   DOUBLES_EQUAL(-2.934280F, intersections_tcs[5].x, tolerance);
   DOUBLES_EQUAL(1.5F, intersections_tcs[5].y, tolerance);
   DOUBLES_EQUAL(2.368543F, intersections_tcs[6].x, tolerance);
   DOUBLES_EQUAL(-1.5F, intersections_tcs[6].y, tolerance);
   DOUBLES_EQUAL(-2.368543F, intersections_tcs[7].x, tolerance);
   DOUBLES_EQUAL(-1.5F, intersections_tcs[7].y, tolerance);
}
/** @}*/


/** \defgroup  Determine_Segments_Intersection_Limited__functionality
 *  @{
 */

 /** \brief
  * Test group for Determine_Segments_Intersection_Limited() function.
  */
TEST_GROUP(Determine_Segments_Intersection_Limited__functionality)
{
   const float tolerance = 0.00001F;
};

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments don't intersect each other (parallel case 1)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, no_intersection__parallel_segments__case_1)
{
   /** \precond
    * Setup two segments (via points) parallel to each other
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 1.0F };
   const float C[2] = { -7.0F, 3.0F };
   const float D[2] = { -5.0F, 3.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * No intersection found
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments don't intersect each other (parallel case 2)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, no_intersection__parallel_segments__case_2)
{
   /** \precond
    * Setup two segments (via points) parallel to each other
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -7.0F, 4.0F };
   const float C[2] = { -6.0F, 1.0F };
   const float D[2] = { -6.0F, 4.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * No intersection found
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments don't intersect each other (parallel case 3)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, no_intersection__parallel_segments__case_3)
{
   /** \precond
    * Setup two segments (via points) parallel to each other
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 4.0F };
   const float C[2] = { -6.0F, 1.0F };
   const float D[2] = { -4.0F, 4.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * No intersection found
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments don't intersect each other (too far away) - case 2
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, no_intersection__too_far_away__case_1)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 1.0F };
   const float C[2] = { -7.0F, 6.0F };
   const float D[2] = { -5.0F, 2.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * No intersection found
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments don't intersect each other (too far away) - case 2
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, no_intersection__too_far_away__case_2)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { 5.0F, 1.0F };
   const float B[2] = { -1.00001F, 4.0F };
   const float C[2] = { 1.0F, 3.0F };
   const float D[2] = { 1.0F, 2.99999F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments intersect each other (case 1)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, one_intersection_case_1)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 1.0F };
   const float C[2] = { -7.0F, 3.0F };
   const float D[2] = { -5.0F, -1.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * One intersection found (-6.0, 1.0)
    */
   CHECK_TRUE(f_intersect);
   DOUBLES_EQUAL(-6.0F, result_x, tolerance);
   DOUBLES_EQUAL(1.0F, result_y, tolerance);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments intersect each other (case 2)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, one_intersection_case_2)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, -1.0F };
   const float C[2] = { -7.0F, 3.0F };
   const float D[2] = { -6.0F, -2.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * One intersection found (-6.5, 0.5)
    */
   CHECK_TRUE(f_intersect);
   DOUBLES_EQUAL(-6.5F, result_x, tolerance);
   DOUBLES_EQUAL(0.5F, result_y, tolerance);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments intersect each other (case 3)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, one_intersection_case_3)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, -1.0F };
   const float C[2] = { -2.0F, 3.0F };
   const float D[2] = { -6.0F, -2.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * One intersection found (-5.(1), -0.(8))
    */
   CHECK_TRUE(f_intersect);
   DOUBLES_EQUAL(-5.111111F, result_x, tolerance);
   DOUBLES_EQUAL(-0.888889F, result_y, tolerance);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments overlap each other partially
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, partially_overlaping)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 1.0F };
   const float C[2] = { -6.0F, 1.0F };
   const float D[2] = { -4.0F, 1.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments fully overlap each other
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, fully_overlaping)
{
   /** \precond
    * Setup two segments (via points)
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 1.0F };
   const float C[2] = { -7.0F, 1.0F };
   const float D[2] = { -5.0F, 1.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when segments are connected at the ends
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, connected_segments)
{
   /** \precond
    * Setup two segments that are connected at the ends
    */
   const float A[2] = { -7.0F, 1.0F };
   const float B[2] = { -5.0F, 1.0F };
   const float C[2] = { -5.0F, 1.0F };
   const float D[2] = { -3.0F, 4.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when one of the segment is a point (seperated from second segment)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, one_segment_is_point__seperated)
{
   /** \precond
    * Setup two segments that are connected at the ends
    */
   const float A[2] = { 5.0F, 3.0F };
   const float B[2] = { 5.0F, 3.0F };
   const float C[2] = { 5.0F, 1.0F };
   const float D[2] = { 7.0F, 4.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when one of the sgement is a point (on second segment)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, one_segment_is_point__on_segment)
{
   /** \precond
    * Setup two segments that are connected at the ends
    */
   const float A[2] = { 5.0F, 3.0F };
   const float B[2] = { 5.0F, 3.0F };
   const float C[2] = { 5.0F, 1.0F };
   const float D[2] = { 5.0F, 5.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when both segments are points (seperated from second segment)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, both_segments_are_points__seperated)
{
   /** \precond
    * Setup two segments that are connected at the ends
    */
   const float A[2] = { 5.0F, 3.0F };
   const float B[2] = { 5.0F, 3.0F };
   const float C[2] = { 6.0F, 1.0F };
   const float D[2] = { 6.0F, 1.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when both segments are points (on second segment)
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, both_segments_are_points__on_segment)
{
   /** \precond
    * Setup two segments that are connected at the ends
    */
   const float A[2] = { 5.0F, 3.0F };
   const float B[2] = { 5.0F, 3.0F };
   const float C[2] = { 5.0F, 3.0F };
   const float D[2] = { 5.0F, 3.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}

/** \purpose
 * Check if Determine_Segments_Intersection_Limited() returns correct output when one segment touch another
 * \req
 * NA
 */
TEST(Determine_Segments_Intersection_Limited__functionality, one_segment_touch_another)
{
   /** \precond
    * Setup two segments that are connected at the ends
    */
   const float A[2] = { 2.0F, 3.0F };
   const float B[2] = { 5.0F, 3.0F };
   const float C[2] = { 3.0F, 3.0F };
   const float D[2] = { 3.0F, 1.0F };

   /** \action
    * Call Determine_Segments_Intersection_Limited()
    */
   float32_t result_x;
   float32_t result_y;
   const bool f_intersect = Determine_Segments_Intersection_Limited(A, B, C, D, result_x, result_y);

   /** \result
    * Returns false
    */
   CHECK_FALSE(f_intersect);
}
/** @}*/