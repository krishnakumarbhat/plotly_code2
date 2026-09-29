/** \file
 * This file contains unit tests for content of f360_points_with_extreme_azimuth.h file
 */

#include "f360_points_with_extreme_azimuth.h"
#include <CppUTest/TestHarness.h>

 // Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_points_with_extreme_azimuth
 *  @{
 */

 /** \brief
  * Test Determine_Points_With_Extreme_Azimuth() functionalty.
  */
TEST_GROUP(f360_points_with_extreme_azimuth)
{
   const Point view_point = { 10.0F, 4.0F };
   const float boresight = 2.6794F;
   const float tolerance = 0.00001F;
};

/** \purpose
 * Check of function returns correct output when 1 point is provided
 * \req
 * NA
 */
TEST(f360_points_with_extreme_azimuth, one_point)
{
   /** \precond
    * Setup one point
    */
   Point points[20];

   points[0] = { 8.0F, 6.0F };
   const unsigned num_points = 1U;


   /** \action
    * Call Determine_Points_With_Extreme_Azimuth()
    */
   Extreme_Azimuth_Points result = Determine_Points_With_Extreme_Azimuth(view_point, boresight, points, num_points);

   /** \result
    * Check results
    */
   DOUBLES_EQUAL(8.0F, result.min_az_point.x, tolerance);
   DOUBLES_EQUAL(6.0F, result.max_az_point.y, tolerance);
}

/** \purpose
 * Check of function returns correct output when 2 points are provided
 * \req
 * NA
 */
TEST(f360_points_with_extreme_azimuth, two_points)
{
   /** \precond
    * Setup two points
    */
   Point points[20];

   points[0] = { 8.0F, 6.0F };
   points[1] = { 6.0F, 4.0F };
   const unsigned num_points = 2U;

   /** \action
    * Call Determine_Points_With_Extreme_Azimuth()
    */
   Extreme_Azimuth_Points result = Determine_Points_With_Extreme_Azimuth(view_point, boresight, points, num_points);

   /** \result
    * Check results
    */
   DOUBLES_EQUAL(6.0F, result.min_az_point.x, tolerance);
   DOUBLES_EQUAL(4.0F, result.min_az_point.y, tolerance);
   DOUBLES_EQUAL(8.0F, result.max_az_point.x, tolerance);
   DOUBLES_EQUAL(6.0F, result.max_az_point.y, tolerance);
}

/** \purpose
 * Check of function returns correct output when 3 points are provided
 * \req
 * NA
 */
TEST(f360_points_with_extreme_azimuth, three_points)
{
   /** \precond
    * Setup three points
    */
   Point points[20];

   points[0] = { 8.0F, 6.0F };
   points[1] = { 6.0F, 4.0F };
   points[2] = { 14.0F, 4.0F };
   const unsigned num_points = 3U;

   /** \action
    * Call Determine_Points_With_Extreme_Azimuth()
    */
   Extreme_Azimuth_Points result = Determine_Points_With_Extreme_Azimuth(view_point, boresight, points, num_points);

   /** \result
    * Check results
    */
   DOUBLES_EQUAL(6.0F, result.min_az_point.x, tolerance);
   DOUBLES_EQUAL(4.0F, result.min_az_point.y, tolerance);
   DOUBLES_EQUAL(14.0F, result.max_az_point.x, tolerance);
   DOUBLES_EQUAL(4.0F, result.max_az_point.y, tolerance);
}

/** \purpose
 * Check of function returns correct output when 4 points are provided
 * \req
 * NA
 */
TEST(f360_points_with_extreme_azimuth, four_points)
{
   /** \precond
    * Setup four points
    */
   Point points[20];

   points[0] = { 8.0F, 6.0F };
   points[1] = { 6.0F, 4.0F };
   points[2] = { 14.0F, 4.0F };
   points[3] = { 14.0F, 0.0F };
   const unsigned num_points = 4U;

   /** \action
    * Call Determine_Points_With_Extreme_Azimuth()
    */
   Extreme_Azimuth_Points result = Determine_Points_With_Extreme_Azimuth(view_point, boresight, points, num_points);

   /** \result
    * Check results
    */
   DOUBLES_EQUAL(14.0F, result.min_az_point.x, tolerance);
   DOUBLES_EQUAL(0.0F, result.min_az_point.y, tolerance);
   DOUBLES_EQUAL(14.0F, result.max_az_point.x, tolerance);
   DOUBLES_EQUAL(4.0F, result.max_az_point.y, tolerance);
}

/** \purpose
 * Check of function returns correct output when 4 points are provided and one of the point lies on boresight
 * \req
 * NA
 */
TEST(f360_points_with_extreme_azimuth, four_points__one_of_is_on_boresight)
{
   /** \precond
    * Setup four points (one on boresight)
    */
   Point points[20];

   points[0] = { 8.0F, 6.0F };
   points[1] = { 6.0F, 6.0F };
   points[2] = { 14.0F, 4.0F };
   points[3] = { 14.0F, 0.0F };
   const unsigned num_points = 4U;

   /** \action
    * Call Determine_Points_With_Extreme_Azimuth()
    */
   Extreme_Azimuth_Points result = Determine_Points_With_Extreme_Azimuth(view_point, boresight, points, num_points);

   /** \result
    * Check results
    */
   DOUBLES_EQUAL(14.0F, result.min_az_point.x, tolerance);
   DOUBLES_EQUAL(0.0F, result.min_az_point.y, tolerance);
   DOUBLES_EQUAL(14.0F, result.max_az_point.x, tolerance);
   DOUBLES_EQUAL(4.0F, result.max_az_point.y, tolerance);
}
/** @}*/
