/** \file
 * This file contains unit tests for content of f360_reflector_object.cpp file
 */

#include "f360_reflector_object.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;


/** \defgroup  Is_On_Radius__Basics
 *  @{
 */

 /** \brief
  * Check functionality of Is_On_Radius() method - basics
  */
TEST_GROUP(Is_On_Radius__Basics)
{
   F360_Object_Track_T object = {};

   /** \setup
    * Set object's position
    */
   TEST_SETUP()
   {   

      object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });

      Point center = { 7.0F, 9.5F };
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(4.0F);
      object.bbox.Set_Width(3.0F);
      object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object.vcs_position = object.bbox.Get_Corners().Rear_Left();

   }
};

/** \purpose
 * Check if method returns false when reflector is far enough
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_too_far_away)
{
   /** \precond
    * Setup view point and searching range to not intersect with reflector (it is far away)
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 0.8F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** \purpose
 *  Check if method returns true when reflector has only one intersection point with searching range circle
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_tangential_to_view_point_circle)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector only in one point
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 1.0F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 1)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_has_two_intersections_with_searching_range__case_1)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 1.2F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 2)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_has_two_intersections_with_searching_range__case_2)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 1.7F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 3)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_has_two_intersections_with_searching_range__case_3)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 2.5F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has three intersection points with searching range circle
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_has_three_intersections_with_searching_range)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in three points
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 5.0F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 4)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_has_two_intersections_with_searching_range__case_4)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 5.1F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 5)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_has_two_intersections_with_searching_range__case_5)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 5.3F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 *  Check if method returns false when reflector is too close (whole inside circle)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics, reflector_is_too_close)
{
    /** \precond 
    * Setup view point and searching range in the way that whole reflector is inside searhing circle
    */
   const Point view_point = { 10.0F, 9.0F };
   const float searching_range = 5.5F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** @}*/
/** \defgroup  Is_On_Radius__Basics_Rotated_Obj
 *  @{
 */

 /** \brief
  * Check functionality of Is_On_Radius() method - basics, object is rotated
  */
TEST_GROUP(Is_On_Radius__Basics_Rotated_Obj)
{
   F360_Object_Track_T object = {};

   /** \setup
    * Set object's position
    */
   TEST_SETUP()
   {

      object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(59.0F) });
      Point center = { 7.0F, 9.5F };
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(4.0F);
      object.bbox.Set_Width(3.0F);
      object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object.vcs_position = object.bbox.Get_Corners().Rear_Left();
      
   }
};

/** \purpose
 * Check if method returns false when reflector is far enough
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics_Rotated_Obj, reflector_too_far_away)
{
   /** \precond
    * Setup view point and searching range to not intersect with reflector (it is far away)
    */
   const Point view_point = { 12.0F, 9.0F };
   const float searching_range = 1.7F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 1) 
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics_Rotated_Obj, reflector_has_two_intersections_with_searching_range__case_1)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 12.0F, 9.0F };
   const float searching_range = 3.7F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 2) 
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics_Rotated_Obj, reflector_has_two_intersections_with_searching_range__case_2)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 12.0F, 9.0F };
   const float searching_range = 4.8F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 3) 
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics_Rotated_Obj, reflector_has_two_intersections_with_searching_range__case_3)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 12.0F, 9.0F };
   const float searching_range = 5.1F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns true when reflector has two intersection points with searching range circle (case 4) 
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics_Rotated_Obj, reflector_has_two_intersections_with_searching_range__case_4)
{
   /** \precond
    * Setup view point and searching range to intersect with reflector in two points
    */
   const Point view_point = { 12.0F, 9.0F };
   const float searching_range = 6.4F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if method returns false when reflector is too close (whole inside circle)
 * \req
 * NA
 */
TEST(Is_On_Radius__Basics_Rotated_Obj, reflector_is_too_close)
{
   /** \precond
    * Setup view point and searching range in the way that whole reflector is inside searhing circle
    */
   const Point view_point = { 12.0F, 9.0F };
   const float searching_range = 7.6F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const bool result = reflector.Is_On_Radius(view_point, searching_range);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}
/** @}*/


/** \defgroup  Is_On_Radius__Basics_Rotated_Obj
 *  @{
 */

 /** \brief
  * Group for testing Compute_Range_Rate_Interval() functionality
  */
TEST_GROUP(Compute_Range_Rate_Interval__Basics)
{
   F360_Object_Track_T object = {};
   const float tolerance = 0.01F;

   /** \setup
    * Set object's position
    */
   TEST_SETUP()
   {

      object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
      Point center = { 16.5F, 15.5F };
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(13.0F);
      object.bbox.Set_Width(3.0F);
      object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object.vcs_position = object.bbox.Get_Corners().Rear_Left();
      object.vcs_velocity = { 3.0F, 8.0F };
   }
};

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns (INFT, INFTY) when there is no any intersection point (reflector is outside circle)
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, no_intersections__reflector_is_out)
{
   /** \precond
    * Setup object and view point in the way that there is no intersection point (reflector is outside circle)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 2.8F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (INFTY, INFTY)
    */
   DOUBLES_EQUAL(INFTY, result.rr_min, tolerance);
   DOUBLES_EQUAL(INFTY, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns (INFT, INFTY) when there is no any intersection point (reflector is inside circle)
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, no_intersections__reflector_is_in)
{
   /** \precond
    * Setup object and view point in the way that there is no intersection point (reflector is inside circle)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 11.6F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (INFTY, INFTY)
    */
   DOUBLES_EQUAL(INFTY, result.rr_min, tolerance);
   DOUBLES_EQUAL(INFTY, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there is only intersection point
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, one_visible_intersections)
{
   /** \precond
    * Setup object and view point in the way that there is only one intersection point
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 3.0F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (8.0F, 8.0F)
    */
   DOUBLES_EQUAL(8.0F, result.rr_min, tolerance);
   DOUBLES_EQUAL(8.0F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there is only one intersection point 
 * (zero radius case)
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, one_visible_intersections__zero_radius)
{
   /** \precond
    * Setup object and view point in the way that there is only one intersection point
    */
   const Point view_point = { 23.0F, 15.0F };
   const float searching_range = 0.0F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (INFTY, INFTY)
    */
   DOUBLES_EQUAL(INFTY, result.rr_min, tolerance);
   DOUBLES_EQUAL(INFTY, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are two visible intersection points
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, two_visible_intersections)
{
   /** \precond
    * Setup object and view point in the way that there are two visible intersection points
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 3.5F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (5.311907F, 8.402379F)
    */
   DOUBLES_EQUAL(5.311907F, result.rr_min, tolerance);
   DOUBLES_EQUAL(8.402379F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are two visible intersection points (one is corner)
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, one_visible_and_one_corner_intersections)
{
   /** \precond
    * Setup object and view point in the way that there are two visible intersection points (one is corner)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 5.0F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (2.4F, 7.2F)
    */
   DOUBLES_EQUAL(2.4F, result.rr_min, tolerance);
   DOUBLES_EQUAL(7.2F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are two intersection points (one is visible and one not) - case 1
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, visible_and_occluded_intersections_case_1)
{
   /** \precond
    * Setup object and view point in the way that there are tw ointersection points (one is visible and one not)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 5.3F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (2.05535F, 7.512516F)
    */
   DOUBLES_EQUAL(2.05535F, result.rr_min, tolerance);
   DOUBLES_EQUAL(7.512516F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are two intersection points (one is visible and one not) - case 2
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, visible_and_occluded_intersections_case_2)
{
   /** \precond
    * Setup object and view point in the way that there are tw ointersection points (one is visible and one not)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 8.5F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (0.0166F, 4.5219F)
    */
   DOUBLES_EQUAL(0.0166F, result.rr_min, tolerance);
   DOUBLES_EQUAL(3.5219F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are three intersection points (one visibe and two occluded)
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, visible_and_two_occluded_intersections)
{
   /** \precond
    * Setup object and view point in the way that there are three intersection points (one visibe and two occluded)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 6.0F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (1.40203F, 7.9650F)
    */
   DOUBLES_EQUAL(1.40203F, result.rr_min, tolerance);
   DOUBLES_EQUAL(7.9650F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are four intersection points (two visibe and two occluded)
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, two_visible_and_two_occluded_intersections)
{
   /** \precond
    * Setup object and view point in the way that there are four intersection points (two visibe and two occluded)
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 6.5F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (1.031F, 8.1519F)
    */
   DOUBLES_EQUAL(1.031F, result.rr_min, tolerance);
   DOUBLES_EQUAL(8.1519F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when there are two occluded intersection points
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, two_occluded_intersections)
{
   /** \precond
    * Setup object and view point in the way that there are two occluded intersection points
    */
   const Point view_point = { 19.0F, 11.0F };
   const float searching_range = 9.9F;
   Object_Reflector reflector = { object };

   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (0.6045F, 2.46225F)
    */
   DOUBLES_EQUAL(0.6045F, result.rr_min, tolerance);
   DOUBLES_EQUAL(2.46225F, result.rr_max, tolerance);
}

/** \purpose
 * Check if Compute_Range_Rate_Interval() returns correct values when object is rotated and there are two intersection points.
 * \req
 * NA
 */
TEST(Compute_Range_Rate_Interval__Basics, rotated_object)
{
   /** \precond
    * Rotate object
    * Setup object and view point in the way that there are two intersection points
    */
   object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(68.75F) });
   Point center = { 16.5F, 15.5F };
   object.bbox.Set_Center(center);
   object.bbox.Set_Length(13.0F);
   object.bbox.Set_Width(3.0F);
   const Point view_point = { 24.0F, 10.0F };
   const float searching_range = 8.0F;
   object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   object.vcs_position = object.bbox.Get_Corners().Rear_Left();
   Object_Reflector reflector = { object };


   /** \action
    * Call Is_On_Radius()
    */
   const Range_Rate_Interval_T result = reflector.Compute_Range_Rate_Interval(view_point, searching_range);

   /** \result
    * Return (-2.92459F, 3.11680F)
    */
   DOUBLES_EQUAL(-2.92459F, result.rr_min, tolerance);
   DOUBLES_EQUAL(3.11680F, result.rr_max, tolerance);
}

/** @}*/


/** \defgroup  Find_Intersection__Basics
 *  @{
 */

 /** \brief
  * Check Find_Intersection() functionality - basics
  */
TEST_GROUP(Find_Intersection__Basics)
{
   F360_Object_Track_T object = {};
   const float tolerance = 0.001F;

   /** \setup
    * Set object's position
    */
   TEST_SETUP()
   {
      object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(68.75F) });
      Point center = { 7.5F, 13.5F };
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(5.0F);
      object.bbox.Set_Width(3.0F);
      object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object.vcs_position = object.bbox.Get_Corners().Rear_Left();
   }
};

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (sides case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__sides)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (sides)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 18.0F, 10.0F };
   const Point segment_point_B = { 2.0F, 18.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(10.76462F, result.second.x, tolerance);
   DOUBLES_EQUAL(13.61769F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (sides case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__sides_with_extension_and_reduced_basic_size)
{
   /** \precond
    * Setup reflector with reduced size and extended corners
    * Setup two points of segment next to an object (sides)
    */
   const float size_extension = 0.6F;
   float32_t length = object.bbox.Get_Length() - 2.0F*size_extension;
   float32_t width = object.bbox.Get_Width() - 2.0F*size_extension;

   object.Update_Bbox_Size(length, width);

   Point center = {8.0F, 14.0F};
   object.bbox.Set_Center(center);
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 18.0F, 10.0F };
   const Point segment_point_B = { 2.0F, 18.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(9.942496F, result.second.x, tolerance);
   DOUBLES_EQUAL(14.02875F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (sides case) and reverted segment points.
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__sides__reverted_points)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (sides) but reverted A with B
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 18.0F, 10.0F };
   const Point segment_point_B = { 2.0F, 18.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_B, segment_point_A);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(5.374887F, result.second.x, tolerance);
   DOUBLES_EQUAL(16.31256F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (front/back case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__front_back)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (front and back)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 10.0F, 18.0F };
   const Point segment_point_B = { 4.0F, 8.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(9.947734F, result.second.x, tolerance);
   DOUBLES_EQUAL(17.91289F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (front right case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__front_right)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (front right)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 10.0F, 18.0F };
   const Point segment_point_B = { 9.0F, 12.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(9.983184F, result.second.x, tolerance);
   DOUBLES_EQUAL(17.8991F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (front left case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__front_left)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (front left)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 10.0F, 18.0F };
   const Point segment_point_B = { 6.0F, 14.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(9.922646F, result.second.x, tolerance);
   DOUBLES_EQUAL(17.92265F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (rear left case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__rear_left)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (rear left)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 6.0F, 14.0F };
   const Point segment_point_B = { 9.0F, 8.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(8.763761F, result.second.x, tolerance);
   DOUBLES_EQUAL(8.472479F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns correct intersection point (rear right case).
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, two_intersections__rear_right)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (rear right)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 12.0F, 12.0F };
   const Point segment_point_B = { 6.0F, 10.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return correct intersection point
    */
   CHECK_TRUE(result.first);
   DOUBLES_EQUAL(9.857862F, result.second.x, tolerance);
   DOUBLES_EQUAL(11.28595F, result.second.y, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no itersection.
 * \req
 * NA
 */
TEST(Find_Intersection__Basics, no_intersections)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object (but not intersect it)
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 6.0F, 20.0F };
   const Point segment_point_B = { 18.0F, 15.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first); 
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}
/** @}*/


/** \defgroup  Intersection__Corner_Cases
 *  @{
 */

 /** \brief
  * Check Find_Intersection() method functionality - corner cases
  */
TEST_GROUP(Intersection__Corner_Cases)
{
   F360_Object_Track_T object = {};
   const float tolerance = 0.0001F;

   /** \setup
    * Set object's position
    */
   TEST_SETUP()
   {

      object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
      Point center = { 7.5F, 13.5F };
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(5.0F);
      object.bbox.Set_Width(3.0F);
      object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object.vcs_position = object.bbox.Get_Corners().Rear_Left();

   }
};

/** \purpose
 * Check if Find_Intersection() returns no intersection when segment fully overlap object's right side.
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, full_overlapping__right_side)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 3.0F, 15.0F };
   const Point segment_point_B = { 12.0F, 15.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when segment fully overlap object's left side.
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, full_overlapping__left_side)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 3.0F, 12.0F };
   const Point segment_point_B = { 12.0F, 12.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when segment fully overlap object's front
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, full_overlapping__front)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 10.0F, 11.0F };
   const Point segment_point_B = { 10.0F, 16.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when segment fully overlap object's back
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, full_overlapping__back)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 5.0F, 11.0F };
   const Point segment_point_B = { 5.0F, 16.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when segment is on one of object's border
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, segment_on_border)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment on object's left border
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 6.0F, 15.0F };
   const Point segment_point_B = { 8.0F, 15.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when segment is inside object
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, segment_inside_object)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment inside object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 6.0F, 13.0F };
   const Point segment_point_B = { 7.0F, 14.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when only intersect with object's front right corner
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, no_intersection__front_right)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 11.0F, 14.0F };
   const Point segment_point_B = { 9.0F, 16.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when only intersect with object's front left corner
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, no_intersection__front_left)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 11.0F, 13.0F };
   const Point segment_point_B = { 9.0F, 11.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when only intersect with object's rear left corner
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, no_intersection__rear_left)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 4.0F, 13.0F };
   const Point segment_point_B = { 6.0F, 11.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}

/** \purpose
 * Check if Find_Intersection() returns no intersection when only intersect with object's rear right corner
 * \req
 * NA
 */
TEST(Intersection__Corner_Cases, no_intersection__rear_right)
{
   /** \precond
    * Setup reflector
    * Setup two points of segment next to an object
    */
   Object_Reflector reflector = { object };
   const Point segment_point_A = { 4.0F, 14.0F };
   const Point segment_point_B = { 6.0F, 16.0F };

   /** \action
    * Call Find_Intersection()
    */
   std::pair<bool, Point> result = reflector.Find_Intersection(segment_point_A, segment_point_B);

   /** \result
    * Return no intersection
    */
   CHECK_FALSE(result.first);
   DOUBLES_EQUAL(0.0F, result.second.y, tolerance);
   DOUBLES_EQUAL(0.0F, result.second.x, tolerance);
}
/** @}*/


/** \defgroup  Is_Inside__functionality
 *  @{
 */

 /** \brief
  * Check Is_Inside() method functionality
  */
TEST_GROUP(Is_Inside__functionality)
{
   F360_Object_Track_T object = {};
   const float tolerance = 0.0001F;

   /** \setup
    * Set object's position
    */
   TEST_SETUP()
   {

      object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
      Point center = { 5.5F, 12.5F };
      object.bbox.Set_Center(center);
      object.bbox.Set_Length(5.0F);
      object.bbox.Set_Width(3.0F);
      object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      object.vcs_position = object.bbox.Get_Corners().Rear_Left();
   }
};

/** \purpose
 * Check if Is_Inside() returns false when point is outside
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_out)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 4.0F, 14.0F + tolerance };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is inside
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_in)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 4.0F, 14.0F - tolerance };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on boundary (case 1)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_boundary__case_1)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 4.0F, 11.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on boundary (case 2)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_boundary__case_2)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 3.0F, 12.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on boundary (case 3)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_boundary__case_3)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 4.0F, 14.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on boundary (case 4)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_boundary__case_4)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 8.0F, 12.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on the corner (case 1)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_the_corner__case_1)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 8.0F, 14.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on the corner (case 2)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_the_corner__case_2)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 8.0F, 11.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on the corner (case 3)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_the_corner__case_3)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 3.0F, 11.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Check if Is_Inside() returns true when point is on the corner (case 4)
 * \req
 * NA
 */
TEST(Is_Inside__functionality, point_is_on_the_corner__case_4)
{
   /** \precond
    * Setup reflector
    * Setup point that will be verified
    */
   Object_Reflector reflector = { object };
   const Point point = { 3.0F, 14.0F };

   /** \action
    * Call Is_Inside()
    */
   const bool result = reflector.Is_Inside(point);

   /** \result
    * Return true
    */
   CHECK_TRUE(result);
}
/** @}*/