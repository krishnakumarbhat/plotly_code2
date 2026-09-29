/** \file
 * This file contains unit tests for content of f360_point.cpp file
 */

#include "f360_point.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Point_Transform_To_New_CS
 *  @{
**/

/** \brief
 *  Tests for Point_Transform_To_New_CS
 **/
TEST_GROUP(Point_Transform_To_New_CS)
{
   /** \setup
    * Setting up float comparison thresholds for passing tests
    **/
   const float tolerance = 0.00001F;
};

/**
 *\purpose  Test Point_Transform_To_New_CS() calculation when new coordinate sytem is only translation
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, only_translated_new_coordinate_system)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ F360_DEG2RAD(0.0F) };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(5.0F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(4.0F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate sytem is translated and rotated (positive)
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, translated_and_rotated_coordinate_system__positive)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ 0.4F };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(6.162978F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(1.737152F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate sytem is only rotated
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, only_rotated_coordinate_system)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ };
   const Angle new_cs_rotation{ 0.4F };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(9.70499F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(2.411019F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate system is translated and rotated (negative)
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, translated_and_rotated_coordinate_system__negative)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ -0.4F };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(3.047631F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(5.631335F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when point is in new coordinate system origin
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, point_in_origin)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ 0.4F };

   Point point_new_cs(new_cs_origin);

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(0.0F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(0.0F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate system is same as original coordinate system
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, coordinate_system_is_same_as_original)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 0.0F, 0.0F };
   const Angle new_cs_rotation{ 0.0F };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(8.0F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(6.0F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate sytem is translated and rotated between 90 and 180 degree
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, translated_and_rotated_coordinate_system__between_90_and_180_deg)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ 0.7F };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(6.40108168F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(-0.16171968F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate sytem is translated and rotated more than 180 degree
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, translated_and_rotated_coordinate_system__more_than_180_deg)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ 1.6F };

   Point point_new_cs = { 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(3.852296F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(-5.114666F, point_new_cs.y, tolerance);
}

/**
 *\purpose  Test Transform_To_New_CS() calculation when new coordinate sytem is translated and rotated exactly -1.2 rad
 *\req    NA
 */
TEST(Point_Transform_To_New_CS, translated_and_rotated_coordinate_system__negative_1point2_rad)
{
   /** \precond
    * Setup point position
    * Set translation vector
    * Set rotation angle
    */
   const Point new_cs_origin{ 3.0F, 2.0F };
   const Angle new_cs_rotation{ -1.2F };

   Point point_new_cs{ 8.0F, 6.0F };

   /** \action
    * Call Transform_To_New_CS
    **/
   point_new_cs.Transform_To_Relative_Coordinate_System(new_cs_origin, new_cs_rotation);

   /** \result
    * Ensure that point is correctly transformed
    **/
   DOUBLES_EQUAL(-1.916367F, point_new_cs.x, tolerance);
   DOUBLES_EQUAL(6.1096264F, point_new_cs.y, tolerance);
}
/** @}*/

/** \brief
 *  Test group for equality operator between Point class objects
 **/
TEST_GROUP(Are_Points_Equal)
{
};

/**
 *\purpose  Check if equality operator returns correct result if points are equal
 *\req    NA
 */
TEST(Are_Points_Equal, pointA_is_equal_to_pointB)
{
   /** \precond
    * Setup points positions
    */
   const Point pointA = { 3.0F, 2.0F };
   const Point pointB(pointA);


   /** \action
    * Check if points are equal
    **/
   const bool are_equal = (pointA == pointB);

   /** \result
    * Verify if comparison result between points is as expected - points should be equal
    **/
   CHECK(are_equal)
}

/**
 *\purpose  Check if equality operator returns correct result if points are not equal
 *\req    NA
 */
TEST(Are_Points_Equal, pointA_is_not_equal_to_pointB_1)
{
   /** \precond
    * Setup points positions
    */
   const Point pointA = { 3.0F, 2.0F };
   const Point pointB = { 3.0F, -2.0F };


   /** \action
    * Check if points are equal
    **/
   const bool are_equal = (pointA == pointB);

   /** \result
    * Verify if comparison result is as expected - points should not be equal
    **/
   CHECK_FALSE(are_equal)
}

/**
 *\purpose  Check if equality operator returns correct result if points are not equal
 *\req    NA
 */
TEST(Are_Points_Equal, pointA_is_not_equal_to_pointB_2)
{
   /** \precond
    * Setup points positions
    */
   const Point pointA = { 3.0F, 2.001F };
   const Point pointB = { 3.0F, 2.0F };


   /** \action
    * Check if points are equal
    **/
   const bool are_equal = (pointA == pointB);

   /** \result
    * Verify if comparison result is as expected - points should not be equal
    **/
   CHECK_FALSE(are_equal)
}

/**
 *\purpose  Check if equality operator returns correct result if points are not equal
 *\req    NA
 */
TEST(Are_Points_Equal, pointA_is_not_equal_to_pointB_3)
{
   /** \precond
    * Setup points positions
    */
   const Point pointA = { 6.1F, 2.0F };
   const Point pointB = { 3.0F, 2.0F };


   /** \action
    * Check if points are equal
    **/
   const bool are_equal = (pointA == pointB);

   /** \result
    * Verify if comparison result is as expected - points should not be equal
    **/
   CHECK_FALSE(are_equal)
}

/**
 *\purpose  Check if equality operator returns correct result if points are not equal
 *\req    NA
 */
TEST(Are_Points_Equal, pointA_is_not_equal_to_pointB_4)
{
   /** \precond
    * Setup points positions
    */
   const Point pointA = { 3.0F, 2.0F };
   const Point pointB = { -2.11F, 2.0F };


   /** \action
    * Check if points are equal
    **/
   const bool are_equal = (pointA == pointB);

   /** \result
    * Verify if comparison result is as expected - points should not be equal
    **/
   CHECK_FALSE(are_equal)
}

/** @}*/


/** \brief
 *  Test group for testing slopes and distance between points
 **/
TEST_GROUP(SlopeAndDistanceTests)
{
    TEST_SETUP() {
        // Setup actions if necessary
    }

    void teardown() {
        // Teardown actions if necessary
    }
};

/** \purpose
 * Test the slope calculation when points have the same x-coordinate.
 * \req
 * NA.
 */
TEST(SlopeAndDistanceTests, PointsWithSameXCoordinate) {
    /** \precond
     * Create two points with the same x-coordinate.
     */
    Point point1(1.0F, 1.0F);
    Point point2(1.0F, 3.0F);

    /** \action
     * Call get_slope_between_points function and save result.
     */
    float32_t slope = Point::get_slope_between_points(point1, point2);

    /** \result
     * The slope should be infinite.
     */
    DOUBLES_EQUAL(INFTY, slope, 0.0001);
}

/** \purpose
 * Test the slope calculation when points have different x-coordinates.
 * \req
 * NA.
 */
TEST(SlopeAndDistanceTests, PointsWithDifferentXCoordinates) {
    /** \precond
     * Create two points with different x-coordinates.
     */
    Point point1(1.0F, 1.0F);
    Point point2(3.0F, 3.0F);

    /** \action
     * Call get_slope_between_points function and save result.
     */
    float32_t slope = Point::get_slope_between_points(point1, point2);

    /** \result
     * The slope should be calculated correctly.
     */
    float32_t expectedSlope = 1.0F;
    DOUBLES_EQUAL(expectedSlope, slope, 0.0001);
}

/** \purpose
 * Test the distance calculation between 2 points.
 * \req
 * NA.
 */
TEST(SlopeAndDistanceTests, DistanceTests) {
    /** \precond
     * Create two points placed diagonally.
     */
    Point point1(1.0F, 1.0F);
    Point point2(4.0F, 5.0F);

    /** \action
     * Call get_distance_between_points function and save result.
     */
    float distance = Point::get_distance_between_points(point1, point2);

    /** \result
     * The distance should be calculated correctly based on the diagonal separation.
     */
    float expectedDistance =5.0F;
    DOUBLES_EQUAL(expectedDistance, distance, 0.0001);
}

/** @}*/
