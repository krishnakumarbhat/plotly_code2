/** \file
 * This file contains unit tests for content of f360_bounding_box.cpp file
 */

#include "f360_bounding_box.h"
#include "f360_angle.h"
#include <CppUTest/TestHarness.h>
#include <array>
// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;


/** \defgroup  f360_bounding_box__Basic_constructors
 *  @{
 */

 /** \brief
  * Test group for testing BoundingBox constructors
  */
TEST_GROUP(f360_bounding_box__Basic_constructors)
{
   const float32_t test_epsilon = 1e-5F;
   const float32_t f360_sqrt2 = 1.4142135F; // square root of 2
};

/** \purpose
 * Verify param constructor
 * \req
 * NA.
 */
TEST(f360_bounding_box__Basic_constructors, param_constructor)
{
   /** \precond
    * N/A
    */

    /** \action
     * Setup bounding box via param constructor
     */
   const BoundingBox bbox{ Point{1.4F, -4.3F}, 12.3F, 3.21F, Angle{F360_DEG2RAD(90.0F)} };

   /** \result
    * Param bbox setup
    */
   DOUBLES_EQUAL(1.4F, bbox.Get_Center().x, test_epsilon);
   DOUBLES_EQUAL(-4.3F, bbox.Get_Center().y, test_epsilon);
   DOUBLES_EQUAL(12.3F, bbox.Get_Length(), test_epsilon);
   DOUBLES_EQUAL(3.21F, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(1.5707963F, bbox.Get_Orientation().Value(), test_epsilon);
   DOUBLES_EQUAL(1.0F, bbox.Get_Orientation().Sin(), test_epsilon);
   DOUBLES_EQUAL(0.0F, bbox.Get_Orientation().Cos(), test_epsilon);
}

/** \purpose
 * Verify non_orientation constructor
 * \req
 * NA.
 */
TEST(f360_bounding_box__Basic_constructors, non_orientation_constructor)
{
   /** \precond
    * N/A
    */

    /** \action
     * Setup bounding box via non_orientation constructor
     */
   const BoundingBox bbox{ Point{3.0F, 3.0F}, Point{5.4F, 6.2F} };

   /** \result
    * Non_orientation bbox setup
    */
   DOUBLES_EQUAL(4.2F, bbox.Get_Center().x, test_epsilon);
   DOUBLES_EQUAL(4.6F, bbox.Get_Center().y, test_epsilon);
   DOUBLES_EQUAL(2.4F, bbox.Get_Length(), test_epsilon);
   DOUBLES_EQUAL(3.2F, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(0.0F, bbox.Get_Orientation().Value(), test_epsilon);
   DOUBLES_EQUAL(0.0F, bbox.Get_Orientation().Sin(), test_epsilon);
   DOUBLES_EQUAL(1.0F, bbox.Get_Orientation().Cos(), test_epsilon);
}
/** @}*/


/** \defgroup  f360_bounding_box_Get_Closest_Distance_To_tests
 *  @{
 */

/** \brief
 * Test group for testing BoundingBox::Get_Closest_Distance_To method which returns closest distance (metal to metal) to other bounding box.
 */
TEST_GROUP(f360_bounding_box_Get_Closest_Distance_To_tests)
{
   const float32_t test_epsilon = 1e-5F;
   const float32_t f360_sqrt2 = 1.4142135F; // square root of 2
};

/** \purpose
 * Test case when both bounding boxes are not rotated, and first one is directly in front of second one.
 * \req
 * NA.
 */
TEST(f360_bounding_box_Get_Closest_Distance_To_tests, Get_Closest_Distance_To_First_Bbox_In_Front_Of_Second)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   Point center = Point(8.0F, 2.0F);
   float32_t length = 4.0F;
   float32_t width = 2.0F;
   Angle orientation{ 0.0F };
   const BoundingBox bbox_A = BoundingBox(center, length, width, orientation);

   center.x = 2.0F;
   const BoundingBox bbox_B = BoundingBox(center, length, width, orientation);
   /** \action
    * Call Get_Closest_Distance_To function and save results.
    */
   const float32_t expected = 2.0F;
   const float32_t dist = bbox_A.Closest_Distance_To(bbox_B);

   /** \result
    * Distance between bboxes should be equal to 2m.
    */
   DOUBLES_EQUAL(expected, dist, test_epsilon);
}

/** \purpose
 * Test case when both bounding boxes are not rotated, and first one is in front of second one and laterally shifted in way that left and right bounds of
   bboxes lays in same line.
 * \req
 * NA.
 */
TEST(f360_bounding_box_Get_Closest_Distance_To_tests, Get_Closest_Distance_To_First_Bbox_In_Front_Of_Second_Laterally_Shifted)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   Point center = Point(8.0F, 2.0F);
   float32_t length = 4.0F;
   float32_t width = 2.0F;
   Angle orientation{ 0.0F };
   const BoundingBox bbox_A = BoundingBox(center, length, width, orientation);

   center.x = 2.0F;
   center.y = 0.0F;
   const BoundingBox bbox_B = BoundingBox(center, length, width, orientation);
   /** \action
    * Call Get_Closest_Distance_To function and save results.
    */
   const float32_t expected = 2.0F;
   const float32_t dist = bbox_A.Closest_Distance_To(bbox_B);

   /** \result
    * Distance between bboxes should be equal to 2m.
    */
   DOUBLES_EQUAL(expected, dist, test_epsilon);
}

/** \purpose
 * Test case when both bounding boxes have same positions and dimensions.
 * \req
 * NA.
 */
TEST(f360_bounding_box_Get_Closest_Distance_To_tests, Get_Closest_Distance_To_Same_Bboxes)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   Point center = Point(8.0F, 2.0F);
   float32_t length = 4.0F;
   float32_t width = 2.0F;
   Angle orientation{ 0.0F };
   const BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   const BoundingBox bbox_B = BoundingBox(center, length, width, orientation);

   /** \action
    * Call Get_Closest_Distance_To function and save results.
    */
   const float32_t expected = 0.0F;
   const float32_t dist = bbox_A.Closest_Distance_To(bbox_B);

   /** \result
    * Distance between bboxes should be equal to 0m.
    */
   DOUBLES_EQUAL(expected, dist, test_epsilon);
}

/** \purpose
 * Test case when one bounding box is inside another.
 * \req
 * NA.
 */
TEST(f360_bounding_box_Get_Closest_Distance_To_tests, Get_Closest_Distance_To_One_Bbox_Inside_Another)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   Point center = Point(3.0F, 3.0F);
   float32_t length = 5.0F;
   float32_t width = 5.0F;
   Angle orientation{ 0.0F };
   const BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   length = 1.0F;
   width = 1.0F;
   orientation.Value(F360_DEG2RAD(45.0F));
   const BoundingBox bbox_B = BoundingBox(center, length, width, orientation);

   /** \action
    * Call Get_Closest_Distance_To function and save results.
    */
   const float32_t expected = 0.0F;
   const float32_t dist = bbox_A.Closest_Distance_To(bbox_B);

   /** \result
    * Distance between bboxes should be equal to 0m.
    */
   DOUBLES_EQUAL(expected, dist, test_epsilon);
}

/** \purpose
 * Test case when one of the corners of first bbox is inside second one.
 * \req
 * NA.
 */
TEST(f360_bounding_box_Get_Closest_Distance_To_tests, Get_Closest_Distance_To_Overlapping_Bboxes)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   Point center = Point(3.0F, 3.0F);
   float32_t length = 5.0F;
   float32_t width = 5.0F;
   Angle orientation{ 0.0F };
   const BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   center.x = 4.0F;
   center.y = 6.0F;
   length = 4.0F;
   width = 2.0F;
   const BoundingBox bbox_B = BoundingBox(center, length, width, orientation);

   /** \action
    * Call Get_Closest_Distance_To function and save results.
    */
   const float32_t expected = 0.0F;
   const float32_t dist = bbox_A.Closest_Distance_To(bbox_B);

   /** \result
    * Distance between bboxes should be equal to 0m.
    */
   DOUBLES_EQUAL(expected, dist, test_epsilon);
}

/** \purpose
 * Test case when when first bounding box is oriented parallel to VCS X axis and second one is moved to front right and tilted 45 deg.
 * \req
 * NA.
 */
TEST(f360_bounding_box_Get_Closest_Distance_To_tests, Get_Closest_Distance_To_Second_Bbox_Tilted_45_Deg)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   Point center = Point(0.0F, 0.0F);
   float32_t length = 2.0F;
   float32_t width = 2.0F;
   Angle orientation{ 0.0F };
   const BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   center.x = 4.0F;
   center.y = 4.0F;
   length = 5.0F;
   width = 2.0F;
   orientation.Value(F360_DEG2RAD(-45.0F));
   const BoundingBox bbox_B = BoundingBox(center, length, width, orientation);

   /** \action
    * Call Get_Closest_Distance_To function and save results.
    */
   const float32_t expected = 3.0F*f360_sqrt2 - (width*0.5F);
   const float32_t dist = bbox_A.Closest_Distance_To(bbox_B);

   /** \result
    * Distance between bboxes should be equal to 3*sqrt(2) - width/2 m.
    */
   DOUBLES_EQUAL(expected, dist, test_epsilon);
}
/** @}*/

/** \defgroup  f360_create_bbox_on_points
 *  @{
 */

/** \brief
 * Test group for testing BoundingBox constructor which let spread bbox over set of Points.
 */
TEST_GROUP(f360_create_bbox_on_points)
{
   const float32_t test_epsilon = 1e-5F;
   const float32_t f360_sqrt2 = 1.4142135F; // square root of 2
};

/** \purpose
 * Test case with two points which should be placed on left bottom and right top corners.
 * \req
 * NA.
 */
TEST(f360_create_bbox_on_points, create_bbox_on_two_points_zero_orientation)
{
   /** \precond
    * Set two points which should lays on left bottom and right top of bbox.
    */
   Point points[MAX_DETS_IN_OBJ_TRK] = { Point(-2.0, -2.0), Point(2.0, 2.0) };
   const Angle orientation{ 0.0F };
   /** \action
    * Create bbox on given points.
    */
   const float32_t expected_center_x = 0.0F;
   const float32_t expected_orientation = orientation.Value();
   const float32_t expected_len = 4.0F;
   const float32_t expected_width = 4.0F;

   const BoundingBox bbox = BoundingBox(points, 2, orientation);

   /** \result
    * Compare result bbox properties with expected values.
    */
   DOUBLES_EQUAL(expected_center_x, bbox.Get_Center().x, test_epsilon);
   DOUBLES_EQUAL(expected_center_x, bbox.Get_Center().y, test_epsilon);
   DOUBLES_EQUAL(expected_orientation, bbox.Get_Orientation().Value(), test_epsilon);
   DOUBLES_EQUAL(expected_len, bbox.Get_Length(), test_epsilon);
   DOUBLES_EQUAL(expected_width, bbox.Get_Width(), test_epsilon);
}

/** \purpose
 * Test case with 45 deg orented five points - center one and middle of bbox edges. Output center should be in CS origin.
 * \req
 * NA.
 */
TEST(f360_create_bbox_on_points, create_bbox_on_five_points_center_ones_and_center_of_edges)
{
   /** \precond
    * Set two points which should lays on left bottom and right top of bbox.
    */
   Point points[MAX_DETS_IN_OBJ_TRK] = {
      Point(-3.0, -3.0),
      Point(1.0, -1.0),
      Point(3.0, 3.0),
      Point(-1.0, 1.0),
      Point(0.0, 0.0),
   };

   const Angle orientation{ F360_DEG2RAD(45.0F) };
   /** \action
    * Create bbox on given points.
    */
   const BoundingBox bbox = BoundingBox(points, 5, orientation);

   /** \result
    * Compare result bbox properties with expected values.
    */
   const float32_t expected_center_x = 0.0F;
   const float32_t expected_center_y = 0.0F;
   const float32_t expected_orientation = orientation.Value();
   const float32_t expected_len = f360_sqrt2 * 3.0F * 2.0F;
   const float32_t expected_width = f360_sqrt2 * 2.0F;

   DOUBLES_EQUAL(expected_center_x, bbox.Get_Center().x, test_epsilon);
   DOUBLES_EQUAL(expected_center_y, bbox.Get_Center().y, test_epsilon);
   DOUBLES_EQUAL(expected_orientation, bbox.Get_Orientation().Value(), test_epsilon);
   DOUBLES_EQUAL(expected_len, bbox.Get_Length(), test_epsilon);
   DOUBLES_EQUAL(expected_width, bbox.Get_Width(), test_epsilon);
}

/** \purpose
 * Test case with 45 deg orented five points - center one and middle of bbox edges. Output center should be in point (1, 1).
 * \req
 * NA.
 */
TEST(f360_create_bbox_on_points, create_bbox_on_five_points_center_ones_and_center_of_edges_moved_1_1)
{
   /** \precond
    * Set two points which should lays on left bottom and right top of bbox.
    */
   Point points[MAX_DETS_IN_OBJ_TRK] = {
      Point(-2.0, -2.0),
      Point(2.0, 0.0),
      Point(4.0, 4.0),
      Point(0.0, 2.0),
      Point(1.0, 1.0),
   };

   const Angle orientation{ F360_DEG2RAD(45.0F) };
   /** \action
    * Create bbox on given points.
    */
   const BoundingBox bbox = BoundingBox(points, 5, orientation);

   /** \result
    * Compare result bbox properties with expected values.
    */
   const float32_t expected_center_x = 1.0F;
   const float32_t expected_center_y = 1.0F;
   const float32_t expected_orientation = orientation.Value();
   const float32_t expected_len = f360_sqrt2 * 3.0F * 2.0F;
   const float32_t expected_width = f360_sqrt2 * 2.0F;

   DOUBLES_EQUAL(expected_center_x, bbox.Get_Center().x, test_epsilon);
   DOUBLES_EQUAL(expected_center_y, bbox.Get_Center().y, test_epsilon);
   DOUBLES_EQUAL(expected_orientation, bbox.Get_Orientation().Value(), test_epsilon);
   DOUBLES_EQUAL(expected_len, bbox.Get_Length(), test_epsilon);
   DOUBLES_EQUAL(expected_width, bbox.Get_Width(), test_epsilon);
}
/** @}*/

/** \defgroup  f360_bounding_box_get_combined_distance_to
 *  @{
 */

/** \brief
 * Test group for testing BoundingBox::Get_Combined_Distance_To method.
 */
TEST_GROUP(f360_bounding_box_get_combined_distance_to)
{
   const float32_t test_epsilon = 1e-5F;
};

/** \purpose
 * Test case where both bboxes have pointing 0 deg and second one is in front right of first one.
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_combined_distance_to, two_axis_oriented_bboxes_long_and_lat_shifted)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(10.0F, 10.0F),  4.0F,    2.0F,  Angle{ 0.0F});

   /** \action
    * Calculate combined distance.
    */
   Distance_Between_Bboxes comb_dist = bbox_A.Combined_Distance_To(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_closest_dist = 7.2111F; // hypot(6,4)
   const float32_t exp_lateral_spread = 10.0F;

   DOUBLES_EQUAL(exp_closest_dist, comb_dist.closest_distance, test_epsilon);
   DOUBLES_EQUAL(exp_lateral_spread, comb_dist.lateral_spread, test_epsilon);
}

/** \purpose
 * Test case where bboxes partially overlap with each other.
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_combined_distance_to, bboxes_overlap)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(3.0F, 3.0F),    4.0F,    2.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   Distance_Between_Bboxes comb_dist = bbox_A.Combined_Distance_To(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_closest_dist = 0.0F;
   const float32_t exp_lateral_spread = 3.0F;

   DOUBLES_EQUAL(exp_closest_dist, comb_dist.closest_distance, test_epsilon);
   DOUBLES_EQUAL(exp_lateral_spread, comb_dist.lateral_spread, test_epsilon);
}

/** \purpose
 * Test case where bboxes are identical - one is on top of another (full overlapping).
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_combined_distance_to, two_identical_bboxes)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   Distance_Between_Bboxes comb_dist = bbox_A.Combined_Distance_To(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_closest_dist = 0.0F;
   const float32_t exp_lateral_spread = 2.0F;

   DOUBLES_EQUAL(exp_closest_dist, comb_dist.closest_distance, test_epsilon);
   DOUBLES_EQUAL(exp_lateral_spread, comb_dist.lateral_spread, test_epsilon);
}

/** \purpose
 * Test case where one bbox is inside another.
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_combined_distance_to, one_bbox_inside_another)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(2.0F, 2.0F),    5.0F,    3.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   Distance_Between_Bboxes comb_dist = bbox_A.Combined_Distance_To(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_closest_dist = 0.0F;
   const float32_t exp_lateral_spread = 2.5F;

   DOUBLES_EQUAL(exp_closest_dist, comb_dist.closest_distance, test_epsilon);
   DOUBLES_EQUAL(exp_lateral_spread, comb_dist.lateral_spread, test_epsilon);
}
/** @}*/

/** \defgroup  f360_bounding_box_get_lateral_spread_between
 *  @{
 */

/** \brief
 * Test group for testing BoundingBox::Get_Lateral_Spread_Between method.
 */
TEST_GROUP(f360_bounding_box_get_lateral_spread_between)
{
   const float32_t test_epsilon = 1e-5F;
};

/** \purpose
 * Test case where both bboxes have pointing 0 deg and second one is in front right of first one.
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_lateral_spread_between, two_axis_oriented_bboxes_long_and_lat_shifted)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(10.0F, 10.0F),   4.0F,    2.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   float32_t lateral_spread = bbox_A.Lateral_Spread_Between(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_lateral_spread = 10.0F;

   DOUBLES_EQUAL(exp_lateral_spread, lateral_spread, test_epsilon);
}

/** \purpose
 * Test case where bboxes partially overlap with each other.
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_lateral_spread_between, bboxes_overlap)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(3.0F, 3.0F),    4.0F,    2.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   float32_t lateral_spread = bbox_A.Lateral_Spread_Between(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_lateral_spread = 3.0F;

   DOUBLES_EQUAL(exp_lateral_spread, lateral_spread, test_epsilon);
}

/** \purpose
 * Test case where bboxes are identical - one is on top of another (full overlapping).
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_lateral_spread_between, two_identical_bboxes)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   float32_t lateral_spread = bbox_A.Lateral_Spread_Between(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_lateral_spread = 2.0F;

   DOUBLES_EQUAL(exp_lateral_spread, lateral_spread, test_epsilon);
}

/** \purpose
 * Test case where one bbox is inside another.
 * \req
 * NA.
 */
TEST(f360_bounding_box_get_lateral_spread_between, one_bbox_inside_another)
{
   /** \precond
   * Create two bounding boxes.
   */
   //                               center(x,    y)       len      wid      ori
   BoundingBox bbox_A = BoundingBox(Point(2.0F, 2.0F),    4.0F,    2.0F,  Angle{ 0.0F });
   BoundingBox bbox_B = BoundingBox(Point(2.0F, 2.0F),    5.0F,    3.0F,  Angle{ 0.0F });

   /** \action
    * Calculate combined distance.
    */
   float32_t lateral_spread = bbox_A.Lateral_Spread_Between(bbox_B);

   /** \result
    * Compare results with expected values.
    */
   const float32_t exp_lateral_spread = 2.5F;

   DOUBLES_EQUAL(exp_lateral_spread, lateral_spread, test_epsilon);
}
/** @}*/

/** \defgroup  f360_bounding_box__Contains
 *  @{
 */

 /** \brief
  * Test group for testing BoundingBox::Contains method.
  */
TEST_GROUP(f360_bounding_box__Contains)
{
   const float32_t test_epsilon = 1e-5F;

   const Point center = Point(8.0F, 2.0F);
   const float32_t length = 10.0F;
   const float32_t width = 2.0F;
   const Angle orientation{ F360_DEG2RAD(60.0F) };
   const BoundingBox bbox = BoundingBox(center, length, width, orientation);
};

/** \purpose
 * Check whether function return true when point is inside
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point_inside)
{
   /** \precond
    * Set point inside bbox.
    */
   const Point point(10.0F, 4.0F);

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox.Contains(point);

   /** \result
    * Point is inside
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether function return false when point is outside
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point_outside)
{
   /** \precond
    * Set point outside bbox.
    */
   const Point point(10.0F, 3.0F);

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox.Contains(point);

   /** \result
    * Point is outside
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check whether function return true when point is on bound
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point__is_on_bound)
{
   /** \precond
    * Set point on bbox bound.
    */
   const Point point(10.0F, 3.0F);
   const BoundingBox bbox_zero_orientation = BoundingBox(center, length, width, Angle{ 0.0F });

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox_zero_orientation.Contains(point);

   /** \result
    * Returns true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether function return true when point is near bound and inside
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point__is_near_bound_and_inside)
{
   /** \precond
    * Set point near bound and inside.
    */
   const Point point(10.0F, 3.0F - 0.0001F);
   const BoundingBox bbox_zero_orientation = BoundingBox(center, length, width, Angle{ 0.0F });

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox_zero_orientation.Contains(point);

   /** \result
    * Returns true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether function return false when point is near bound and outside
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point__is_near_bound_and_outside)
{
   /** \precond
    * Set point near bound and outside.
    */
   const Point point(10.0F, 3.0F + 0.0001F);
   const BoundingBox bbox_zero_orientation = BoundingBox(center, length, width, Angle{ 0.0F });

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox_zero_orientation.Contains(point);

   /** \result
    * Returns false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check whether function return true when point is on the edge (rear left corner)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point__is_on_edge_rl_corner)
{
   /** \precond
    * Set point on bbox edge.
    */
   const Point point(3.0F, 1.0F);
   const BoundingBox bbox_zero_orientation = BoundingBox(center, length, width, Angle{ 0.0F });

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox_zero_orientation.Contains(point);

   /** \result
    * Returns true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether function return true when point is on the edge (front right corner)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Contains, point__is_on_edge_fr_corner)
{
   /** \precond
    * Set point on bbox edge.
    */
   const Point point(13.0F, 3.0F);
   const BoundingBox bbox_zero_orientation = BoundingBox(center, length, width, Angle{ 0.0F });

   /** \action
    * Call Contains function and save results.
    */
   const bool result = bbox_zero_orientation.Contains(point);

   /** \result
    * Returns true
    */
   CHECK_TRUE(result);
}
/** @}*/


/** \defgroup  f360_bounding_box__Collides
 *  @{
 */

 /** \brief
  * Test group for testing BoundingBox::Collides method which checks if two bounding box collide each other.
  */
TEST_GROUP(f360_bounding_box__Collides)
{
};

/** \purpose
 * Check whether method returns false when two bounding boxes don't collide each other
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, no_collision)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 4.0F, Angle{ 1.2F });
   const BoundingBox bbox_B = BoundingBox(Point(2.0F, 0.0F), 4.0F, 2.0F, Angle{ -0.9F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check whether method returns false when two bounding boxes don't collide each other (reverted)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, no_collision__reverterd)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 4.0F, Angle{ 1.2F });
   const BoundingBox bbox_B = BoundingBox(Point(2.0F, 0.0F), 4.0F, 2.0F, Angle{ -0.9F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_B.Collides(bbox_A);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check whether method returns false when two bounding boxes don't collide each other.
 * In this case orientation should be zero for both (parralel)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, no_collision__zero_orientation)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 4.0F, Angle{ 0.0F });
   const BoundingBox bbox_B = BoundingBox(Point(2.0F, -2.0F), 4.0F, 2.0F, Angle{ 0.0F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check whether method returns true when two bounding boxes collide each other
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 4.0F, Angle{ 1.2F });
   const BoundingBox bbox_B = BoundingBox(Point(2.0F, 0.0F), 8.0F, 2.0F, Angle{ -0.9F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether method returns true when one bounding box is inside the other
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision__one_bbox_inside_other)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 4.0F, Angle{ 1.2F });
   const BoundingBox bbox_B = BoundingBox(Point(-2.0F, 2.0F), 4.0F, 2.0F, Angle{ 1.2F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether method returns true when one bounding box is inside the other (reverted)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision__one_bbox_inside_other__reverted)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 4.0F, Angle{ 1.2F });
   const BoundingBox bbox_B = BoundingBox(Point(-2.0F, 2.0F), 4.0F, 2.0F, Angle{ 1.2F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_B.Collides(bbox_A);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether method returns true when one bounding box side lies partially on the other
 * bounding box side
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision__bbox_side_lies_partially_on_other_bbox_side)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(10.0F, 0.0F), 4.0F, 4.0F, Angle{ 0.0F });
   const BoundingBox bbox_B = BoundingBox(Point(8.0F, 1.0F), 4.0F, 4.0F, Angle{ 0.0F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether method returns true when both bounding boxes have one same corner (sharing it)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision__bboxes_have_one_same_corner)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(10.0F, 0.0F), 4.0F, 4.0F, Angle{ 0.0F });
   const BoundingBox bbox_B = BoundingBox(Point(13.5F, 3.5F), 3.0F, 3.0F, Angle{ 0.0F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether method returns true when method is called with same object instance (collision with each other)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision__itself)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(8.0F, 2.0F), 4.0F, 2.0F, Angle{ 0.0F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_A);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check whether method returns true when two bounding box togehter look like star (centers are inside each other but corners not)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Collides, collision__star)
{
   /** \precond
    * Set two bounding boxes properties.
    */
   const BoundingBox bbox_A = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 6.0F, Angle{ 1.2F });
   const BoundingBox bbox_B = BoundingBox(Point(-2.0F, 2.0F), 6.0F, 6.0F, Angle{ -1.2F });

   /** \action
    * Call Collides().
    */
   const bool result = bbox_A.Collides(bbox_B);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}
/** @}*/


/** \defgroup  f360_bounding_box__Corners_Project_Shadow_On
 *  @{
 */

 /** \brief
  * Test group for testing BboxCorners Project_Shadow_On() method
  */
TEST_GROUP(f360_bounding_box__Corners_Project_Shadow_On)
{
   const float32_t test_epsilon = 1e-5F;
   const Point center{ 6.0F, 8.0F };
   const float32_t length{ 4.0F };
   const float32_t width{ 2.0F };
   const Vector_T axis{ 0.8944272F, 0.4472136F };
};

/** \purpose
 * Check if method returns correct interval (case 1)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Corners_Project_Shadow_On, case_1)
{
   /** \precond
    * Set bounding box and get its corners
    */
   const BoundingBox bbox{ center, length, width, Angle{-2.0F } };
   const BboxCorners corners = bbox.Get_Corners();

   /** \action
    * Call Project_Shadow_On().
    */
   const Interval<float32_t> interval = corners.Project_Shadow_On(axis);

   /** \result
    * Interval as expected
    */
   DOUBLES_EQUAL(6.759352F, interval.lower, test_epsilon);
   DOUBLES_EQUAL(11.1291922F, interval.upper, test_epsilon);
}

/** \purpose
 * Check if method returns correct interval (case 2)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Corners_Project_Shadow_On, case_2)
{
   /** \precond
    * Set bounding box and get its corners
    */
   const BoundingBox bbox{ center, length, width, Angle{-0.8F } };
   const BboxCorners corners = bbox.Get_Corners();

   /** \action
    * Call Project_Shadow_On().
    */
   const Interval<float32_t> interval = corners.Project_Shadow_On(axis);

   /** \result
    * Interval as expected
    */
   DOUBLES_EQUAL(7.386388F, interval.lower, test_epsilon);
   DOUBLES_EQUAL(10.5021555F, interval.upper, test_epsilon);
}

/** \purpose
 * Check if method returns correct interval (case 3)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Corners_Project_Shadow_On, case_3)
{
   /** \precond
    * Set bounding box and get its corners
    */
   const BoundingBox bbox{ center, length, width, Angle{0.7F} };
   const BboxCorners corners = bbox.Get_Corners();

   /** \action
    * Call Project_Shadow_On().
    */
   const Interval<float32_t> interval = corners.Project_Shadow_On(axis);

   /** \result
    * Interval as expected
    */
   DOUBLES_EQUAL(6.765717F, interval.lower, test_epsilon);
   DOUBLES_EQUAL(11.122827F, interval.upper, test_epsilon);
}


/** \purpose
 * Check if method returns correct interval (case 4)
 * \req
 * NA.
 */
TEST(f360_bounding_box__Corners_Project_Shadow_On, case_4)
{
   /** \precond
    * Set bounding box and get its corners
    */
   const BoundingBox bbox{ center, length, width, Angle{3.0F} };
   const BboxCorners corners = bbox.Get_Corners();

   /** \action
    * Call Project_Shadow_On().
    */
   const Interval<float32_t> interval = corners.Project_Shadow_On(axis);

   /** \result
    * Interval as expected
    */
   DOUBLES_EQUAL(6.7305814F, interval.lower, test_epsilon);
   DOUBLES_EQUAL(11.15796243F, interval.upper, test_epsilon);
}
/** @}*/

/** \defgroup  f360_bounding_box_Extension
 *  @{
 */

/** \brief
 * Test group for testing BoundingBox boundary extension.
 */
TEST_GROUP(f360_bounding_box_Extension)
{
   const float32_t test_epsilon = 1e-5F;

   const Point center{8.0F, 2.0F};
   const float32_t length = 10.0F;
   const float32_t width = 2.0F;
   const Angle orientation{ F360_DEG2RAD(60.0F) };
   BoundingBox bbox = BoundingBox(center, length, width, orientation);
};

/** \purpose
 * Check if bounding box corners are not changed for values of zero
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_0_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 0.0F;
   const float32_t right = 0.0F;
   const float32_t rear = 0.0F;
   const float32_t front = 0.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{6.36602540F, -2.83012701F};
   const Point exp_rear_right{4.63397459F, -1.83012701F};
   const Point exp_front_left{11.36602540F, 5.83012701F};
   const Point exp_front_right{9.63397459F, 6.83012701F};
   const float32_t exp_width = 2.0F;
   const float32_t exp_length = 10.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly extending only the left boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_left_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = 0.0F;
   const float32_t rear = 0.0F;
   const float32_t front = 0.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{8.09807621F, -3.83012701F};
   const Point exp_rear_right{4.63397459F, -1.83012701F};
   const Point exp_front_left{13.09807621F, 4.83012701F};
   const Point exp_front_right{9.63397459F, 6.83012701F};
   const float32_t exp_width = 4.0F;
   const float32_t exp_length = 10.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly extending only the right boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_right_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 0.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 0.0F;
   const float32_t front = 0.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{6.36602540F, -2.83012702F};
   const Point exp_rear_right{3.76794919F, -1.33012702F};
   const Point exp_front_left{11.36602540F, 5.83012702F};
   const Point exp_front_right{8.76794919F, 7.33012702F};
   const float32_t exp_width = 3.0F;
   const float32_t exp_length = 10.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly extending only the rear boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_rear_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 0.0F;
   const float32_t right = 0.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 0.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{5.86602540F, -3.69615242F};
   const Point exp_rear_right{4.13397460F, -2.69615242F};
   const Point exp_front_left{11.36602540F, 5.83012702F};
   const Point exp_front_right{9.63397460F, 6.83012702F};
   const float32_t exp_width = 2.0F;
   const float32_t exp_length = 11.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly extending only the front boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_front_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 0.0F;
   const float32_t right = 0.0F;
   const float32_t rear = 0.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{6.36602540F, -2.83012702F};
   const Point exp_rear_right{4.63397460F, -1.83012702F};
   const Point exp_front_left{12.86602540F, 8.42820323F};
   const Point exp_front_right{11.13397460F, 9.42820323F};
   const float32_t exp_width = 2.0F;
   const float32_t exp_length = 13.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly contracting the left boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_left_neg_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = -0.5F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{5.43301270F, -3.44615242F};
   const Point exp_rear_right{3.26794919F, -2.19615242F};
   const Point exp_front_left{12.43301270F, 8.67820323F};
   const Point exp_front_right{10.26794919F, 9.92820323F};
   const float32_t exp_width = 2.5F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly contracting the right boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_right_neg_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = -0.5F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{7.59807621F, -4.69615242F};
   const Point exp_rear_right{4.56698730F, -2.94615242F};
   const Point exp_front_left{14.59807621F, 7.42820323F};
   const Point exp_front_right{11.56698730F, 9.17820323F};
   const float32_t exp_width = 3.5F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly contracting the rear boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_rear_neg_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = -1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{8.59807621F, -2.96410162F};
   const Point exp_rear_right{4.26794919F, -0.46410162F};
   const Point exp_front_left{14.59807621F, 7.42820323F};
   const Point exp_front_right{10.26794919F, 9.92820323F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 12.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly contracting the front boundary
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_front_neg_change)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = -3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{7.59807621F, -4.69615242F};
   const Point exp_rear_right{3.26794919F, -2.19615242F};
   const Point exp_front_left{11.59807621F, 2.23205081F};
   const Point exp_front_right{7.26794919F, 4.73205081F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 8.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly for negative left absolutely greater than the original width
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_great_neg_left)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = -6.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{3.26794919F, -2.19615242F};
   const Point exp_rear_right{0.66987298F, -0.69615242F};
   const Point exp_front_left{10.26794919F, 9.92820323F};
   const Point exp_front_right{7.66987298F, 11.42820323F};
   const float32_t exp_width = 3.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly for negative right absolutely greater than the original width
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_great_neg_right)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = -6.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{9.33012702F, -5.69615242F};
   const Point exp_rear_right{7.59807621F, -4.69615242F};
   const Point exp_front_left{16.33012702F, 6.42820323F};
   const Point exp_front_right{14.59807621F, 7.42820323F};
   const float32_t exp_width = 2.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly for negative rear absolutely greater than the original length
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_great_neg_rear)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = -15.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{14.59807621F, 7.42820323F};
   const Point exp_rear_right{10.26794919F, 9.92820323F};
   const Point exp_front_left{15.59807621F, 9.16025404F};
   const Point exp_front_right{11.26794919F, 11.66025404F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 2.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly for negative front absolutely greater than the original length
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_great_neg_front)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = -13.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{6.59807621F, -6.42820323F };
   const Point exp_rear_right{2.26794919F, -3.92820323F};
   const Point exp_front_left{7.59807621F, -4.69615242F};
   const Point exp_front_right{3.26794919F, -2.19615242F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 2.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly with length and width extension for 0 deg orientation
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_0_deg)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const Angle orientation{ F360_DEG2RAD(0.0F) };
   BoundingBox bbox = BoundingBox(center, length, width, orientation);
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
       * Call Extend_Boundaries function.
       */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{2.0F, -1.0F};
   const Point exp_rear_right{2.0F, 4.0F};
   const Point exp_front_left{16.0F, -1.0F};
   const Point exp_front_right{16.0F, 4.0F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly with length and width extension for 60 deg orientation
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_60_deg)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{7.59807682F, -4.69615269F};
   const Point exp_rear_right{3.26794958F, -2.19615269F};
   const Point exp_front_left{14.5980759F, 7.42820358F};
   const Point exp_front_right{10.2679491F, 9.92820358F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly with length and width extension for 90 deg orientation
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_90_deg)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const Angle orientation{ F360_DEG2RAD(90.0F) };
   BoundingBox bbox = BoundingBox(center, length, width, orientation);
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{11.0F, -4.0F};
   const Point exp_rear_right{6.0F, -4.0F};
   const Point exp_front_left{11.0F, 10.0F};
   const Point exp_front_right{6.0F, 10.0F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly with length and width extension for 180 deg orientation
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_180_deg)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const Angle orientation{ F360_DEG2RAD(180.0F) };
   BoundingBox bbox = BoundingBox(center, length, width, orientation);
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{14.0F, 5.0F};
   const Point exp_rear_right{14.0F, 0.0F};
   const Point exp_front_left{0.0F, 5.0F};
   const Point exp_front_right{0.0F, 0.0F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}

/** \purpose
 * Check if bounding box corners are changed accordingly with length and width extension for 240 deg orientation
 * \req
 * NA.
 */
TEST(f360_bounding_box_Extension, Extend_Boundaries_check_corners_240_deg)
{
   /** \precond
    * set increment arguments for extending bounding box boundaries
    */
   const Angle orientation{ F360_DEG2RAD(240.0F) };
   BoundingBox bbox = BoundingBox(center, length, width, orientation);
   const float32_t left = 2.0F;
   const float32_t right = 1.0F;
   const float32_t rear = 1.0F;
   const float32_t front = 3.0F;

   /** \action
    * Call Extend_Boundaries function.
    */
   bbox.Extend_Boundaries(left, right, rear, front);

   /** \result
    * Compare resulting bbox corner coordinates with expected corner coordinates.
    */
   const BboxCorners corners = bbox.Get_Corners();
   const Point exp_rear_left{8.40192378F, 8.69615242F};
   const Point exp_rear_right{12.73205080F, 6.19615242F};
   const Point exp_front_left{1.40192378F, -3.42820323F};
   const Point exp_front_right{5.73205080F, -5.92820323F};
   const float32_t exp_width = 5.0F;
   const float32_t exp_length = 14.0F;

   DOUBLES_EQUAL(exp_rear_left.x, corners.Rear_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_left.y, corners.Rear_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.x, corners.Rear_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_rear_right.y, corners.Rear_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.x, corners.Front_Left().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_left.y, corners.Front_Left().y, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.x, corners.Front_Right().x, test_epsilon);
   DOUBLES_EQUAL(exp_front_right.y, corners.Front_Right().y, test_epsilon);
   DOUBLES_EQUAL(exp_width, bbox.Get_Width(), test_epsilon);
   DOUBLES_EQUAL(exp_length, bbox.Get_Length(), test_epsilon);
}
/** @}*/

/** \defgroup  f360_bounding_box__Overlap_Area__Overlaping_Cases
 *  @{
 */

 /** \brief
  * Test group for testing BoundingBox::Overlap_Area method in case when two bboxes are overlapping.
  */
TEST_GROUP(f360_bounding_box__Overlap_Area__Overlaping_Cases)
{
   const float32_t test_epsilon = 1e-5F;

   const Point center = Point(-5.0F, -5.0F);
   const float32_t length = 10.0F;
   const float32_t width = 2.0F;
   const Angle orientation{ 0.0F };
   BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   BoundingBox bbox_B = bbox_A;
};

/** \purpose
 * Check function behavior when bbox A is completly inside bbox B. Function called on bbox_A.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bbox_A_in_bbox_B_called_on_bbox_A)
{
   /** \precond
    * Set bbox B size to be a bigger than A.
    */
   bbox_B.Set_Length(bbox_A.Get_Length() + 2.00F);
   bbox_B.Set_Width(bbox_A.Get_Width() + 2.00F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when bbox A is completly inside bbox B. Function called on bbox_B.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bbox_A_in_bbox_B_called_on_bbox_B)
{
   /** \precond
    * Set bbox B size to be a bigger than A.
    */
   bbox_B.Set_Length(bbox_A.Get_Length() + 2.00F);
   bbox_B.Set_Width(bbox_A.Get_Width() + 2.00F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same - full overlap. Function called on bbox_A.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_called_on_bbox_A)
{
   /** \precond
    * Bboxes already the same.
    */

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same - full overlap, but one's orientation is set to 180 deg.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_first_one_rotated_180_deg)
{
   /** \precond
    * Rotate bbox B by 180 deg.
    */
   bbox_B.Set_Orientation(F360_DEG2RAD(180.F));

    /** \action
     * Call Overlap_Area function.
     */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same - full overlap. Function called on bbox_B.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_called_on_bbox_B)
{
   /** \precond
    * Bboxes already the same.
    */

    /** \action
     * Call Overlap_Area function.
     */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same ( and rotated ) - full overlap. Function called on bbox_A.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_and_rotated_called_on_bbox_A)
{
   /** \precond
    * Rotated both bboxes.
    */
   bbox_A.Rotate_About_Origin(Angle{ F360_DEG2RAD(30.0F) });
   bbox_B.Rotate_About_Origin(Angle{ F360_DEG2RAD(30.0F) });

    /** \action
     * Call Overlap_Area function.
     */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same ( and rotated ) - full overlap. Function called on bbox_B.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_and_rotated_called_on_bbox_B)
{
   /** \precond
    * Rotate both bboxes.
    */
   const Angle rot_angle { F360_DEG2RAD(30.0F) };
   bbox_A.Rotate_About_Origin(rot_angle);
   bbox_B.Rotate_About_Origin(rot_angle);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be same as bbox_A area.
    */

   const float32_t exp_area = bbox_A.Get_Area();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when orientation of one of bboxes is set to 90 deg. Case where none of corners from 1 bbox are inside second one.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, corners_of_one_bbox_are_outside_of_another_but_overlaps)
{
   /** \precond
    * Change orientation of bbox to 90 deg.
    */
   bbox_A.Set_Orientation(F360_DEG2RAD(90.0F));

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal bbox_width * bbox_width == 4
    */

   const float32_t exp_area = 4.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same, but one of it is slightly moved longitudinally.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_first_slightly_moved_longitudinally)
{
   /** \precond
    * Translate bbox_A by some small number.
    */
   const float dx = 0.001F;
   bbox_A.Translate(dx, 0.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be slightly smaller than bbox A/B area.
    */

   const float32_t exp_area = (bbox_A.Get_Length() - dx) * bbox_A.Get_Width();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when both bboxes are the same, but one of it is slightly moved laterally.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, same_bboxes_first_slightly_moved_laterally)
{
   /** \precond
    * Rotate both bboxes.
    */
   const float dy = 0.001F;
   bbox_A.Translate(0.0F, dy);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be slightly smaller than bbox A/B area.
    */

   const float32_t exp_area = (bbox_A.Get_Width() - dy) * bbox_A.Get_Length();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when corner of bbox A is placed in center of bbox B.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bbox_A_rear_left_corner_placed_in_bbox_B_center)
{
   /** \precond
    * Translate bbox B.
    */
   bbox_A.Translate(bbox_B.Get_Length() * 0.5F, bbox_B.Get_Width() * 0.5F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should equal to 0.25 of bboxes area.
    */

   const float32_t exp_area = bbox_A.Get_Area() * 0.25F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when corner of bbox A is placed in center of bbox B. Both bboxes are rotated.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bbox_A_rear_left_corner_placed_in_bbox_B_center_both_rotated)
{
   /** \precond
    * Translate bbox B then rotate both bboxes.
    */
   bbox_A.Translate(bbox_B.Get_Length() * 0.5F, bbox_B.Get_Width() * 0.5F);

   const Angle rot_angle{ F360_DEG2RAD(30.0F) };
   bbox_A.Rotate_About_Origin(rot_angle);
   bbox_B.Rotate_About_Origin(rot_angle);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should equal to 0.25 of bboxes area.
    */

   const float32_t exp_area = bbox_A.Get_Area() * 0.25F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when corner both bboxes are rotated and placed randomly. Expected area calculated using Geogebra tool.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bboxes_randomly_placed_and_rotated_case_1)
{
   /** \precond
    * Set bboxes properties.
    */
   bbox_A = BoundingBox{ Point{-1.7F, -0.38F}, 4.0F, 2.0F, Angle{-2.8F} };
   bbox_B = BoundingBox{ Point{-0.59F, 0.12F}, 4.0F, 2.0F, Angle{0.58F} };

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should equal 5.15001.
    */

   const float32_t exp_area = 5.15001F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when corner both bboxes are rotated and placed randomly. Expected area calculated using Geogebra tool.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bboxes_randomly_placed_and_rotated_case_2)
{
   /** \precond
    * Set bboxes properties.
    */
   bbox_A = BoundingBox{ Point{-1.4F, -1.1F}, 4.0F, 2.0F, Angle{-2.06F} };
   bbox_B = BoundingBox{ Point{-1.08F, -1.04F}, 4.0F, 2.0F, Angle{1.88F} };

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should equal 5.21501.
    */

   const float32_t exp_area = 5.21501F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when corner both bboxes are rotated and placed randomly. Expected area calculated using Geogebra tool.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Overlaping_Cases, bboxes_randomly_placed_and_rotated_case_3)
{
   /** \precond
    * Set bboxes properties.
    */
   bbox_A = BoundingBox{ Point{-0.28F, 0.53F}, 4.0F, 2.0F, Angle{-1.98F} };
   bbox_B = BoundingBox{ Point{0.34F, 1.62F}, 4.0F, 2.0F, Angle{1.42F} };

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should equal 4.77956.
    */

   const float32_t exp_area = 4.77956F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}
/** @}*/

 /** \brief
  * Test group for testing BoundingBox::Overlap_Area method in case when two bboxes are not overlapping.
  */
TEST_GROUP(f360_bounding_box__Overlap_Area__Not_Overlaping_Cases)
{
   const float32_t test_epsilon = 1e-5F;

   const Point center = Point(-5.0F, -5.0F);
   const float32_t length = 10.0F;
   const float32_t width = 2.0F;
   const Angle orientation{ 0.0F };
   BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   BoundingBox bbox_B = bbox_A;

   TEST_SETUP()
   {
      // Reset bboxes state
      bbox_A = BoundingBox(center, length, width, orientation);
      bbox_B = bbox_A;
   }
};

/** \purpose
 * Check function behavior when both bboxes are far away from each other.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Not_Overlaping_Cases, bboxes_far_away)
{
   /** \precond
    * Translate bbox B to be far away from bbox A.
    */
   bbox_B.Translate(100.0F, 100.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be 0.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when front edge of first bbox overlaps with rear edge of second bbox. Call OverlapArea on bboxB.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Not_Overlaping_Cases, bboxes_share_front_rear_edge_called_on_bbox_B)
{
   /** \precond
    * Translate one of bboxes.
    */
   bbox_B.Translate(bbox_A.Get_Length(), 0.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be 0.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when front edge of first bbox overlaps with rear edge of second bbox. Call OverlapArea on bboxA.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Not_Overlaping_Cases, bboxes_share_front_rear_edge_called_on_bbox_A)
{
   /** \precond
    * Translate one of bboxes.
    */
   bbox_B.Translate(bbox_A.Get_Length(), 0.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be 0.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when left edge of first bbox overlaps with right edge of second bbox. Call OverlapArea on bboxA.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Not_Overlaping_Cases, bboxes_share_left_right_edge_called_on_bbox_A)
{
   /** \precond
    * Translate one of bboxes.
    */
   bbox_B.Translate(0.0F, bbox_A.Get_Width());

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be 0.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when left edge of first bbox overlaps with right edge of second bbox. Call OverlapArea on bboxB.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Not_Overlaping_Cases, bboxes_share_left_right_edge_called_on_bbox_B)
{
   /** \precond
    * Translate one of bboxes.
    */
   bbox_B.Translate(0.0F, bbox_A.Get_Width());

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be 0.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}
/** @}*/

 /** \brief
  * Test group for testing BoundingBox::Overlap_Area method cornercases when points of first bbox lies on edges of second bbox.
  */
TEST_GROUP(f360_bounding_box__Overlap_Area__Corner_Cases)
{
   const float32_t test_epsilon = 1e-5F;

   const Point center = Point(-5.0F, -5.0F);
   const float32_t length = 10.0F;
   const float32_t width = 2.0F;
   const Angle orientation{ 0.0F };
   BoundingBox bbox_A = BoundingBox(center, length, width, orientation);
   BoundingBox bbox_B = bbox_A;

   TEST_SETUP()
   {
      // Reset bboxes state
      bbox_A = BoundingBox(center, length, width, orientation);
      bbox_B = bbox_A;
   }
};

/** \purpose
 * Check function behavior when front edge of first bbox is placed slightly inside second bbox. Overlapping area should be positive, but very small.
 * Call OverlapArea on bboxA.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barelly_overlaps_longitudinally_called_on_bbox_A)
{
   /** \precond
    * First Translate one of bboxes, to make bboxes touching one another. Next translate one of bbox by some small number to make them overlap.
    */
   float dx = 0.001F;
   bbox_B.Translate(bbox_A.Get_Length(), 0.0F);
   bbox_B.Translate(-dx, 0.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be equal to dx * width of bbox.
    */

   const float32_t exp_area = dx * bbox_B.Get_Width();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when front edge of first bbox is placed slightly inside second bbox. Overlapping area should be positive, but very small.
 * Call OverlapArea on bboxB.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barelly_overlaps_longitudinally_called_on_bbox_B)
{
   /** \precond
    * First Translate one of bboxes, to make bboxes touching one another. Next translate one of bbox by some small number to make them overlap.
    */
   float dx = 0.001F;
   bbox_B.Translate(bbox_A.Get_Length(), 0.0F);
   bbox_B.Translate(-dx, 0.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal to dx * width of bbox.
    */

   const float32_t exp_area = dx * bbox_B.Get_Width();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when front edge of first bbox is placed slightly inside second bbox. Overlapping area should be positive, but very small.
 * Both bboxes are rotated by 45 deg.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barelly_overlaps_longitudinally_rotated_45_deg)
{
   /** \precond
    * First Translate one of bboxes, to make bboxes touching one another. Next translate one of bbox by some small number to make them overlap. Finally rotate both bboxes.
    */
   float dx = 0.001F;
   Angle rot_angle = Angle(F360_DEG2RAD(45.0F));

   bbox_B.Translate(bbox_A.Get_Length(), 0.0F);
   bbox_B.Translate(-dx, 0.0F);

   bbox_A.Rotate_About_Origin(rot_angle);
   bbox_B.Rotate_About_Origin(rot_angle);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal to dx * width of bbox.
    */

   const float32_t exp_area = dx * bbox_B.Get_Width();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when front edge of first bbox is placed slightly inside second bbox. Overlapping area should be positive, but very small.
 * Both bboxes are rotated by -45 deg.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barelly_overlaps_longitudinally_rotated_neg_45_deg)
{
   /** \precond
    * First Translate one of bboxes, to make bboxes touching one another. Next translate one of bbox by some small number to make them overlap. Finally rotate both bboxes.
    */
   float dx = 0.001F;
   Angle rot_angle = Angle(F360_DEG2RAD(-45.0F));

   bbox_B.Translate(bbox_A.Get_Length(), 0.0F);
   bbox_B.Translate(-dx, 0.0F);

   bbox_A.Rotate_About_Origin(rot_angle);
   bbox_B.Rotate_About_Origin(rot_angle);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal to dx * width of bbox.
    */

   const float32_t exp_area = dx * bbox_B.Get_Width();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when left edge of first bbox is placed slightly inside second bbox. Overlapping area should be positive, but very small.
 * Call OverlapArea on bboxA.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barelly_overlaps_lateraly_called_on_bbox_A)
{
   /** \precond
    * First Translate one of bboxes, to make bboxes touching one another. Next translate one of bbox by some small number to make them overlap.
    */
   float dy = 0.001F;
   bbox_B.Translate(0.0F, bbox_B.Get_Width());
   bbox_B.Translate(0.0F, -dy);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_A.Overlap_Area(bbox_B);

   /** \result
    * Overlapping area should be equal to dx * length of bbox.
    */

   const float32_t exp_area = dy * bbox_B.Get_Length();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when left edge of first bbox is placed slightly inside second bbox. Overlapping area should be positive, but very small.
 * Call OverlapArea on bboxA.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barelly_overlaps_lateraly_called_on_bbox_B)
{
   /** \precond
    * First Translate one of bboxes, to make bboxes touching one another. Next translate one of bbox by some small number to make them overlap.
    */
   float dy = 0.001F;
   bbox_B.Translate(0.0F, bbox_B.Get_Width());
   bbox_B.Translate(0.0F, -dy);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal to dx * length of bbox.
    */

   const float32_t exp_area = dy * bbox_B.Get_Length();
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when first bbox covers half of second bounding box longitudinally.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_overlaps_in_half_longitudinally)
{
   /** \precond
    * Translate second bbox by half of it's length.
    */
   bbox_B.Translate(bbox_B.Get_Length() * 0.5F, 0.0F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal half area of bbox.
    */

   const float32_t exp_area = bbox_B.Get_Area() * 0.5F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when first bbox covers half of second bounding box laterally.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_overlaps_in_half_laterally)
{
   /** \precond
    * Translate second bbox by half of it's length.
    */
   bbox_B.Translate(0.0F, bbox_B.Get_Width() * 0.5F);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be equal half area of bbox.
    */

   const float32_t exp_area = bbox_B.Get_Area() * 0.5F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when bboxes are not overlapping, but shares one of the corners.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_share_one_corner)
{
   /** \precond
    * Translate second bbox by vector (length, width).
    */
   bbox_B.Translate(bbox_B.Get_Length(), bbox_B.Get_Width());

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be zero.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when bboxes are not overlapping, but shares one of the corners. Both bboxes are also rotated.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_rotated_share_one_corner)
{
   /** \precond
    * Translate second bbox by vector (length, width). Rotate both bboxes by some angle value.
    */
   bbox_B.Translate(bbox_B.Get_Length(), bbox_B.Get_Width());

   const Angle rot_angle{ F360_DEG2RAD(-30.0F) };
   bbox_A.Rotate_About_Origin(rot_angle);
   bbox_B.Rotate_About_Origin(rot_angle);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be zero.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when bboxes are barely overlapping, in region of one of the corners.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_barely_overlaps_near_corner)
{
   /** \precond
    * Translate second bbox by vector by little smaller vector than (length, width), to make them slightly overlap.
    */
   float dx = 0.001F, dy = 0.001F;
   bbox_B.Translate(bbox_B.Get_Length() - dx, bbox_B.Get_Width() - dy);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be very small.
    */

   const float32_t exp_area = dx * dy;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when bboxes are barely overlapping, in region of one of the corners. Both bboxes are rotated.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bboxes_rotated_barely_overlaps_near_corner)
{
   /** \precond
    * Translate second bbox by vector by little smaller vector than (length, width), to make them slightly overlap. Then rotate both by some angle value.
    */
   float dx = 0.001F, dy = 0.001F;
   bbox_B.Translate(bbox_B.Get_Length() - dx, bbox_B.Get_Width() - dy);

   const Angle rot_angle{ F360_DEG2RAD(10.F) };
   bbox_A.Rotate_About_Origin(rot_angle);
   bbox_B.Rotate_About_Origin(rot_angle);

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be very small.
    */

   const float32_t exp_area = dx * dy;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when one of corners of bbox A is located at one of bbox_B edges.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bbox_A_rear_left_corner_on_right_edge_of_bbox_B)
{
   /** \precond
    * First rotate bbox B around it's rear left corner. To achieve that place this corner at (0,0) and then rotate bbox. Then translate back to original position.
    * Next translate bbox B in way that it's rear left corner lay on right edge of bbox A.
    */
   const float dx = -bbox_B.Get_Center().x + bbox_B.Get_Length() * 0.5F;
   const float dy = -bbox_B.Get_Center().y + bbox_B.Get_Width() * 0.5F;
   const Angle rot_angle {F360_DEG2RAD(15.0F)};

   bbox_B.Translate(dx, dy);
   bbox_B.Rotate_About_Origin(rot_angle);
   bbox_B.Translate(-dx, -dy);

   bbox_B.Translate(bbox_A.Get_Length() * 0.5F, bbox_A.Get_Width());

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be zero.
    */

   const float32_t exp_area = 0.0F;
   DOUBLES_EQUAL(exp_area, area, test_epsilon);
}

/** \purpose
 * Check function behavior when one of corners of bbox B is located inside of bbox A and it's very close to it's right edge. Overlapping area should be positive, but very small.
 * \req
 * NA.
 */
TEST(f360_bounding_box__Overlap_Area__Corner_Cases, bbox_A_rear_left_corner_inside_bbox_B_very_close_to_right_edge)
{
   /** \precond
    * First rotate bbox B around it's rear left corner. To achieve that place this corner at (0,0) and then rotate bbox. Then translate back to original position.
    * Next translate bbox B in way that it's rear left corner lay inside bbox A, near it's right edge.
    */
   const float dx = -bbox_B.Get_Center().x + bbox_B.Get_Length() * 0.5F;
   const float dy = -bbox_B.Get_Center().y + bbox_B.Get_Width() * 0.5F;
   const Angle rot_angle{ F360_DEG2RAD(15.0F) };

   bbox_B.Translate(dx, dy);
   bbox_B.Rotate_About_Origin(rot_angle);
   bbox_B.Translate(-dx, -dy);

   bbox_B.Translate(bbox_A.Get_Length() * 0.5F, bbox_A.Get_Width() - 0.001F);  // Move laterally by slightly smaller value than bbox B width

   /** \action
    * Call Overlap_Area function.
    */
   const float32_t area = bbox_B.Overlap_Area(bbox_A);

   /** \result
    * Overlapping area should be positive, but very small. It's exact numeric value is not relevant.
    */
   CHECK_TRUE(area > 0.0F);
   CHECK_TRUE(area < 1e-5F);
}

/** @}*/

/** \defgroup  BoundingBoxSlopeTests
 *  @{
 */

 /** \brief
  * Test group for testing slopes of lines inside bounding box
  */
TEST_GROUP(BoundingBoxSlopeTests)
{
    BoundingBox* bbox;
    float32_t length;
    float32_t width;
    const float32_t test_epsilon = 1e-5F;

   TEST_SETUP() {
        Point center = Point(8.0F, 2.0F);
        length = 4.0F;
        width = 2.0F;
        Angle orientation{ 0.0F };
        bbox = new BoundingBox(center, length, width, orientation);
    }

    void teardown() {
        delete bbox;
    }
};

/** \purpose
 * Test the slope of the diagonal with a non-zero length.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, GetSlopeOfDiagonalWithNonZeroLength) {
    /** \precond
     * Bounding box created in setup with non-zero length.
     */

    /** \action
     * Call Get_Slope_Of_Diagonal function and save result.
     */
    float32_t expectedSlope = width / length;
    float32_t actualSlope = bbox->Get_Slope_Of_Diagonal();

    /** \result
     * The slope should match the expected value.
     */
    CHECK_EQUAL(expectedSlope, actualSlope);
}

/** \purpose
 * Test the slope of the diagonal when length is zero (should return INFINITY).
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, GetSlopeOfDiagonalWithZeroLength) {
    /** \precond
     * Set length of bounding box to zero.
     */
    bbox->Set_Length(0.0F);

    /** \action
     * Call Get_Slope_Of_Diagonal function and save result.
     */
    float32_t slope = bbox->Get_Slope_Of_Diagonal();

    /** \result
     * The slope should be positive infinity.
     */
    CHECK_TRUE(slope > 0);
    DOUBLES_EQUAL(INFTY, slope, test_epsilon);
}

/** \purpose
 * Test the slope calculation from the bounding box center to a point with non-zero delta_x.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, SlopeWithNonZeroDeltaX) {
    /** \precond
     * Set position to a point with non-zero delta_x and delta_y from the bounding box center.
     */
    Point position(10.0F, 4.0F);
    Point center = bbox->Get_Center();

    /** \action
     * Call Get_Slope_from_bbox_center_to_point function and save result.
     */
    float32_t expectedSlope = (position.y - center.y) / (position.x - center.x);
    float32_t actualSlope = bbox->Get_Slope_from_bbox_center_to_point(position);

    /** \result
     * The slope should match the expected value.
     */
    CHECK_EQUAL(expectedSlope, actualSlope);
}

/** \purpose
 * Test the slope calculation from the bounding box center to a point with zero delta_x (should return INFINITY).
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, SlopeWithZeroDeltaX) {
    /** \precond
     * Set position to a point with zero delta_x from the bounding box center.
     */
    Point center = bbox->Get_Center();
    Point position(center.x, 10.0F);

    /** \action
     * Call Get_Slope_from_bbox_center_to_point function and save result.
     */
    float32_t slope = bbox->Get_Slope_from_bbox_center_to_point(position);

    /** \result
     * The slope should be positive infinity.
     */
    CHECK_TRUE(slope > 0);
    DOUBLES_EQUAL(INFTY, slope, test_epsilon);
}

/** \purpose
 * Test the slope calculation from the bounding box center to itself.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, SlopeWithPointAtCenter) {
    /** \precond
     * Set position to the bounding box center.
     */
    Point position = bbox->Get_Center();

    /** \action
     * Call Get_Slope_from_bbox_center_to_point function and save result.
     */
    float32_t slope = bbox->Get_Slope_from_bbox_center_to_point(position);

    /** \result
     * check for expected behavior (INFINITY).
     */
    DOUBLES_EQUAL(INFTY, slope, test_epsilon);
}

/** \purpose
 * Test the closest corner calculation when the point is nearest to the front-left corner.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, ClosestCornerToFrontLeft) {
    /** \precond
     * Bounding box created in setup. Position the point closer to the front-left corner.
     */
    Point position(11.0F, 0.0F);

    /** \action
     * Call Get_Closest_Corner_To_Point function and save result.
     */
    Point expectedCorner = bbox->Get_Corners().Front_Left();
    Point closestCorner = bbox->Get_Closest_Corner_To_Point(position);

    /** \result
     * The closest corner should be the front-left corner.
     */
    DOUBLES_EQUAL(expectedCorner.x, closestCorner.x, test_epsilon);
    DOUBLES_EQUAL(expectedCorner.y, closestCorner.y, test_epsilon);
}


/** \purpose
 * Test the closest corner calculation when the point is nearest to the front-right corner.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, ClosestCornerToFrontRight) {
    /** \precond
     * Bounding box created in setup. Position the point closer to the front-right corner.
     */
    Point position(11.0F, 5.0F);

    /** \action
     * Call Get_Closest_Corner_To_Point function and save result.
     */
    Point expectedCorner = bbox->Get_Corners().Front_Right();
    Point closestCorner = bbox->Get_Closest_Corner_To_Point(position);

    /** \result
     * The closest corner should be the front-right corner.
     */
    DOUBLES_EQUAL(expectedCorner.x, closestCorner.x, test_epsilon);
    DOUBLES_EQUAL(expectedCorner.y, closestCorner.y, test_epsilon);
}

/** \purpose
 * Test the closest corner calculation when the point is nearest to the rear-left corner.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, ClosestCornerToRearLeft) {
    /** \precond
     * Bounding box created in setup. Position the point closer to the rear-left corner.
     */
    Point position(6.0F, 0.0F);

    /** \action
     * Call Get_Closest_Corner_To_Point function and save result.
     */
    Point expectedCorner = bbox->Get_Corners().Rear_Left();
    Point closestCorner = bbox->Get_Closest_Corner_To_Point(position);

    /** \result
     * The closest corner should be the rear-left corner.
     */
    DOUBLES_EQUAL(expectedCorner.x, closestCorner.x, test_epsilon);
    DOUBLES_EQUAL(expectedCorner.y, closestCorner.y, test_epsilon);
}

/** \purpose
 * Test the closest corner calculation when the point is nearest to the rear-right corner.
 * \req
 * NA.
 */
TEST(BoundingBoxSlopeTests, ClosestCornerToRearRight) {
    /** \precond
     * Bounding box created in setup. Position the point closer to the rear-right corner.
     */
    Point position(6.0F, 5.0F);

    /** \action
     * Call Get_Closest_Corner_To_Point function and save result.
     */
    Point expectedCorner = bbox->Get_Corners().Rear_Right();
    Point closestCorner = bbox->Get_Closest_Corner_To_Point(position);

    /** \result
     * The closest corner should be the rear-right corner.
     */
    DOUBLES_EQUAL(expectedCorner.x, closestCorner.x, test_epsilon);
    DOUBLES_EQUAL(expectedCorner.y, closestCorner.y, test_epsilon);
}

/** @}*/
/** \defgroup  BoundingBoxSlopeTests
 *  @{
 */

 /** \brief
  * Test group for testing flipping boundg box by the line
  */
TEST_GROUP(BoundingBoxFlipByLine)
{
    BoundingBox bbox;
    float32_t length;
    float32_t width;
    Line line;
    const float32_t test_epsilon = 1e-5F;
    
    // setup bounding box and line to reflect
   TEST_SETUP() {
        Point center = Point(0.0F, 0.0F);
        length = 4.0F;
        width = 2.0F;
        Angle orientation{ 0.0F };
        bbox = BoundingBox(center, length, width, orientation);
        line = Line{1.0F, 1.0F, -2.0F};
    }
};

/** \purpose
 * Test if bounding box is flipped correctly.
 * \req
 * NA.
 */
TEST(BoundingBoxFlipByLine, FlipByTheLine) {
    /** \precond
     * Bounding box and line created in group setup.
     */

    /** \action
     * Call Flip_By_The_Line function and save result.
     */
    bbox.Flip_By_The_Line(line);

    /** \result
     * The bounding box position and orientation is checked to be correclty flipped.
     */
    DOUBLES_EQUAL(2.0F, bbox.Get_Center().x, test_epsilon);
    DOUBLES_EQUAL(2.0F, bbox.Get_Center().y, test_epsilon);
    DOUBLES_EQUAL(bbox.Get_Orientation().Value(), -F360_PI_2, test_epsilon);
}

/** @}*/
