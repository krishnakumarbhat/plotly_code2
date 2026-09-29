/** \file
 * This file contains unit tests for content of f360_merge_bbox_overlap_test__Different_Overlaps.cpp file
 */

#include "f360_merge_bbox_overlap_test.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_merge_bbox_overlap_test__Different_Overlaps
 *  @{
 */

/** \brief
 * Test group defined for testing Merge_Bbox_Overlap_Test function that checks that the function handles
 * different types of overlap as expected.
 */
TEST_GROUP(f360_merge_bbox_overlap_test__Different_Overlaps)
{
   // Declare common variables used within all tests in f360_merge_bbox_overlap_test__Different_Overlaps test group.
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   TEST_SETUP()
   {

      first_object.vcs_position.x = 0.0F;
      first_object.vcs_position.y = 0.0F;
      first_object.Set_Bbox_Orientation(Angle{ 0.0F });

      second_object.vcs_position.x = 0.0F;
      second_object.vcs_position.y = 0.0F;
      second_object.Set_Bbox_Orientation(Angle{ 0.0F });
   }
};

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test pass when second object bbox is inside first object bbox.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_Second_Obj_Bbox_In_First_Obj_Bbox)
{
   /** \precond
   *  Put second object bbox inside first one.
   */
   first_object.Update_Bbox_Size(5.0F, 3.0F);
   second_object.Update_Bbox_Size(2.0F, 1.0F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}

/** \purpose
 * Checks whether Merge_Bbox_Overlap_Test pass when bbox corners aren't overlaps, but first object centroid is inside second object bbox.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_First_Obj_Centroid_In_Second_Obj_Bbox)
{
   /** \precond
    * Put first object centroid inside second object bbox.
    */
   first_object.Update_Bbox_Size(2.0F, 1.0F);
   second_object.Update_Bbox_Size(10.0F, 0.1F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}

/** \purpose
 * Checks whether Merge_Bbox_Overlap_Test pass when bbox corners aren't overlaps, but second object centroid is inside first object bbox.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_Second_Obj_Centroid_In_First_Obj_Bbox)
{
   /** \precond
    * Put second object centroid inside first object bbox.
    */
   first_object.vcs_position.x = 1.0F;
   first_object.vcs_position.y = 0.5F;

   first_object.Update_Bbox_Size(2.0F, 1.0F);
   second_object.Update_Bbox_Size(10.0F, 0.1F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test pass when first object front right corner is inside second object bbox.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_First_Obj_FR_Corner_In_Second_Bbox)
{
   /** \precond
   *  Put first object front right corner inside second object bbox.
   */
   first_object.Update_Bbox_Size(5.0F, 3.0F);

   second_object.vcs_position.x = 1.0F;
   second_object.vcs_position.y = 1.0F;

   second_object.Update_Bbox_Size(2.0F, 1.0F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test pass when first object rear left corner is inside second object bbox.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_First_Obj_RL_Corner_In_Second_Bbox)
{
   /** \precond
   *  Put first object rear left corner inside second object bbox.
   */
   first_object.Update_Bbox_Size(5.0F, 3.0F);

   second_object.vcs_position.x = -1.0F;
   second_object.vcs_position.y = -1.0F;

   second_object.Update_Bbox_Size(4.0F, 1.0F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test pass when first object rear right corner is inside second object bbox.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_First_Obj_RR_Corner_In_Second_Bbox)
{
   /** \precond
   *  Put first object rear right corner inside second object bbox.
   */
   first_object.Update_Bbox_Size(5.0F, 3.0F);

   second_object.vcs_position.x = -1.0F;
   second_object.vcs_position.y = 1.0F;

   second_object.Update_Bbox_Size(4.0F, 1.0F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test pass when first object front left corner is inside second object bbox.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__Different_Overlaps, Merge_Bbox_Overlap_Test_First_Obj_FL_Corner_In_Second_Bbox)
{
   /** \precond
   *  Put first object front left corner inside second object bbox.
   */
   first_object.Update_Bbox_Size(5.0F, 3.0F);

   second_object.vcs_position.x = 1.0F;
   second_object.vcs_position.y = -1.0F;
   
   second_object.Update_Bbox_Size(4.0F, 1.0F);

   /** \action
    * call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check if function returns true.
    */
   CHECK_TRUE(f_test_pass);
}
/** @}*/

/** \defgroup  f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency
 *  @{
 */

/** \brief
 * Test group defined for testing Merge_Bbox_Overlap_Test function that check that the bbox extension
 * is computed as expected with respect to its object length dependeny. 
 */
TEST_GROUP(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency)
{
   
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   TEST_SETUP()
   {
      // Create two small objects (total length below 18m).
      // Set objects to have same slow speed (below 30kph) and same heading.
      // Place objects such that they are further apart than 2 * 0.0417*18m (expected mimimum possible bbox extension).
      // Each test in the test group will modify the object lengths and the object positions to verify that they are flagged as overlapping/not overlapping as expected for some different length combinations

      first_object.reference_point = F360_REFERENCE_POINT_FRONT;
      first_object.vcs_position.x = 0.0F;
      first_object.vcs_position.y = 0.0F;
      first_object.bbox.Set_Orientation(Angle{ 0.0F });
      first_object.bbox.Set_Length(8.0F);
      first_object.bbox.Set_Width(2.0F);
      first_object.Update_Bbox_Center();
      first_object.speed = 8.0F;
      first_object.vcs_heading.Value(0.0F);

      const float32_t distance_between_objs = 2.0F * 0.0417F * 18.0F + 0.1F;

      second_object.reference_point = F360_REFERENCE_POINT_REAR;
      second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
      second_object.vcs_position.y = first_object.vcs_position.y;
      second_object.bbox.Set_Orientation(Angle{ 0.0F });
      second_object.bbox.Set_Length(8.0F);
      second_object.bbox.Set_Width(2.0F);
      second_object.Update_Bbox_Center();
      second_object.speed = 8.0F;
      second_object.vcs_heading.Value(0.0F);
   }
};

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and total object total length is small such that is should be saturated at 18m
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturated_At_Min_18m_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Two small objects (8m respectively such that total length is below 18m).
   *     - Objects have same slow speed (8m/s) and same heading (0deg).
   *     - Objects are positioned such that they are further apart than 2 * 0.0417*18m (expected mimimum possible bbox extension).
   *  Changes from TEST_SETUP:
   *     - None
   */

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed closer
 *  than the expected bbox extension and total object total length is small such that is should be saturated at 18m
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturated_At_Min_18m_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Two small objects (total length below 18m).
   *     - Objects have same slow speed (8m/s) and same heading (0deg).
   *  Changes from TEST_SETUP:
   *     - Objects are placed such that they are closer together than 2 * 0.0417*18m (expected mimimum possible bbox extension).
   */
   const float32_t distance_between_objs = 2.0F * 0.0417F * 18.0F - 0.1F;
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and total object total length is mid long (in between 18 and 30m)
 *  such that is should not be saturated
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Mid_Length_Not_Saturated_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP is being used:
   *     - Two object with same slow speed (8m/s) and same heading (0deg).
   *   Changes from TEST_SETUP:
   *     - Set object sizes to 14m and 10m respectively (total length is 24m which is in between 18m and 30m)
   *     - Position objects such that they are further apart than 2 * 0.0417*total_object_length (expected bbox extension).
   */
   first_object.bbox.Set_Length(14.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Length(10.0F);

   const float32_t distance_between_objs = 2.0F * 0.0417F * (first_object.bbox.Get_Length() + second_object.bbox.Get_Length()) + 0.1F;

   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed closer together
 *  than the expected bbox extension and total object total length is mid long (in between 18 and 30m)
 *  such that is should not be saturated.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Mid_Length_Not_Saturated_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP is being used:
   *     - Two object with same slow speed (8m/s) and same heading (0deg).
   *   Changes from TEST_SETUP:
   *     - Set object sizes to 14m and 10m respectively (total length is 24m which is in between 18m and 30m)
   *     - Position objects such that they are closer to each other than 2 * 0.0417*total_object_length (expected bbox extension).
   */
   first_object.bbox.Set_Length(14.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Length(10.0F);

   const float32_t distance_between_objs = 2.0F * 0.0417F * (first_object.bbox.Get_Length() + second_object.bbox.Get_Length()) - 0.1F;

   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and total object total length is long (exceeding 30m)
 *  such that is should be saturated at 30m
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturated_At_Max_30m_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP is being used:
   *     - Two object with same slow speed (8m/s) and same heading (0deg).
   *   Changes from TEST_SETUP:
   *     - Set object sizes to 16m and 15m respectively (total length is 31m which is larger than 30m)
   *     - Position objects such that they are further apart than 2 * 0.0417*30m (expected bbox extension).
   */
   first_object.bbox.Set_Length(16.0F);
   first_object.Update_Bbox_Center();

   const float32_t distance_between_objs = 2.0F * 0.0417F * 30.0F + 0.1F;

   second_object.bbox.Set_Length(15.0F);
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and total object total length is long (exceeding 30m)
 *  such that is should be saturated at 30m
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturated_At_Max_30m_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP is being used:
   *     - Two object with same slow speed (8m/s) and same heading (0deg).
   *   Changes from TEST_SETUP:
   *     - Set object sizes to 16m and 15m respectively (total length is 31m which is larger than 30m)
   *     - Position objects such that they are further apart than 2 * 0.0417*30m (expected bbox extension).
   */
   first_object.bbox.Set_Length(16.0F);
   first_object.Update_Bbox_Center();

   const float32_t distance_between_objs = 2.0F * 0.0417F * 30.0F - 0.1F;

   second_object.bbox.Set_Length(15.0F);
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}
/** @}*/

/** \defgroup  f360_merge_bbox_overlap_test__BBox_Extension_Object_Speed_Dependency
 *  @{
 */

/** \brief
 * Test group defined for testing Merge_Bbox_Overlap_Test function that check that the bbox extension
 * is computed as expected with respect to its object speed dependeny. 
 */
TEST_GROUP(f360_merge_bbox_overlap_test__BBox_Extension_Object_Speed_Dependency)
{
   
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   TEST_SETUP()
   {
      // Create two small objects (total length below 18m).
      // Set objects to have same slow speed (below 30kph) and same heading.
      // Place objects such that they are further apart than 2 * 0.0417*18m (expected mimimum possible bbox extension).
      // Each test in the test group will modify the object speeds and the object positions to verify that they are flagged as overlapping/not overlapping as expected for some different speed combinations.
      first_object.reference_point = F360_REFERENCE_POINT_FRONT;
      first_object.vcs_position.x = 0.0F;
      first_object.vcs_position.y = 0.0F;
      first_object.bbox.Set_Orientation(Angle{ 0.0F });
      first_object.bbox.Set_Length(8.0F);
      first_object.bbox.Set_Width(2.0F);
      first_object.Update_Bbox_Center();
      first_object.speed = 8.0F;
      first_object.vcs_heading.Value(0.0F);

      const float32_t distance_between_objs = 2.0F * 0.0417F * 18.0F + 0.1F;

      second_object.reference_point = F360_REFERENCE_POINT_REAR;
      second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
      second_object.vcs_position.y = first_object.vcs_position.y;
      second_object.bbox.Set_Orientation(Angle{ 0.0F });
      second_object.bbox.Set_Length(8.0F);
      second_object.bbox.Set_Width(2.0F);
      second_object.Update_Bbox_Center();
      second_object.speed = 8.0F;
      second_object.vcs_heading.Value(0.0F);
   }
};

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and average object speed is small such that is should be saturated at 30kph
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Speed_Dependency, Saturated_At_Min_30kph_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Two objects with small speed (8m/s such that average speed is below 30kph = 8.33m/s).
   *     - Objects have short lengths (8m such that the totallength is below 18m) and same heading (0deg).
   *     - Objects are positioned such that they are further apart than 2 * 0.0417*18m (expected mimimum possible bbox extension).
   *  Changes from TEST_SETUP:
   *     - None
   */

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed closer
 *  than the expected bbox extension and average object speed is small such that is should be saturated at 30kph
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturated_At_Min_30kph_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Two objects with small speed (8m/s such that average speed is below 30kph = 8.33m/s).
   *     - Objects have short lengths (8m such that the totallength is below 18m) and same heading (0deg).
   *  Changes from TEST_SETUP:
   *     - Position objects such that they are closer together than 2 * 0.0417*18m (expected mimimum possible bbox extension).
   */
   const float32_t distance_between_objs = 2.0F * 0.0417F * 18.0F - 0.1F;

   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further apart
 *  than the expected bbox extension and average object speed is medium (in between 30kph and 90kph) such that is
 * should not be saturated.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Mid_Speed_Not_Saturated_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have short lengths (8m such that the totallength is below 18m) and same heading (0deg).
   *  Changes from TEST_SETUP:
   *     - Set object speeds to 65kph and 55kph respectively (average speed is 60kph which is in beween 30kph and 90kph)
   *     - Position objects such that they are further apart than 2 * 2/24*18m (expected bbox extension).
   */
   first_object.speed = 65.0F/3.6F;

   const float32_t distance_between_objs = 2.0F * 2.0F/24.0F * 18.0F + 0.1F;
   
   second_object.speed = 55.0F/3.6F;
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed closer together
 *  than the expected bbox extension and average object speed is medium (in between 30kph and 90kph) such that is
 * should not be saturated.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Mid_Speed_Not_Saturated_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have short lengths (8m such that the totallength is below 18m) and same heading (0deg).
   *  Changes from TEST_SETUP:
   *     - Set object speeds to 65kph and 55kph respectively (average speed is 60kph which is in beween 30kph and 90kph)
   *     - Position objects such that they are closer together than 2 * 2/24*18m (expected bbox extension).
   */
   first_object.speed = 65.0F/3.6F;

   const float32_t distance_between_objs = 2.0F * 2.0F/24.0F * 18.0F - 0.1F;
   
   second_object.speed = 55.0F/3.6F;
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further apart
 *  than the expected bbox extension and average object speed is large (above 90kph) such that is
 * should be saturated.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturate_At_Max_90kph_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have short lengths (8m such that the totallength is below 18m) and same heading (0deg).
   *  Changes from TEST_SETUP:
   *     - Set object speeds to 95kph (average speed is above 90kph)
   *     - Position objects such that they are further apart than 2*0.125*18m (expected bbox extension).
   */
   first_object.speed = 95.0F/3.6F;

   const float32_t distance_between_objs = 2.0F * 0.125F * 18.0F + 0.1F;
   
   second_object.speed = 95.0F/3.6F;
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed further apart
 *  than the expected bbox extension and average object speed is large (above 90kph) such that is
 * should be saturated.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Length_Dependency, Saturate_At_Max_90kph_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have short lengths (8m such that the totallength is below 18m) and same heading (0deg).
   *  Changes from TEST_SETUP:
   *     - Set object speeds to 95kph (average speed is above 90kph)
   *     - Position objects such that they are closer together than 2*0.125*18m (expected bbox extension).
   */
   first_object.speed = 95.0F/3.6F;

   const float32_t distance_between_objs = 2.0F * 0.125F * 18.0F - 0.1F;
   
   second_object.speed = 95.0F/3.6F;
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}
/** @}*/

/** \defgroup  f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency
 *  @{
 */

/** \brief
 * Test group defined for testing Merge_Bbox_Overlap_Test function that check that the bbox extension
 * is computed as expected with respect to its object heading difference dependeny. 
 */
TEST_GROUP(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency)
{
   
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   TEST_SETUP()
   {
      // Create two small objects (total length below 18m).
      // Set objects to have same large speed (above 90kph such that bbox extension is maximized) and same heading.
      // Place objects such that they are further apart than 2 * 0.125*18m (expected bbox extension).
      // Each test in the test group will modify the object headings and the object positions to verify that they are flagged as overlapping/not overlapping as expected for some different heading combinations.
      first_object.reference_point = F360_REFERENCE_POINT_FRONT;
      first_object.vcs_position.x = 0.0F;
      first_object.vcs_position.y = 0.0F;
      first_object.bbox.Set_Orientation(Angle{ 0.0F });
      first_object.bbox.Set_Length(8.0F);
      first_object.bbox.Set_Width(2.0F);
      first_object.Update_Bbox_Center();
      first_object.speed = 100.0F / 3.6F;
      first_object.vcs_heading.Value(0.0F);

      const float32_t distance_between_objs = 2.0F * 0.125F * 18.0F + 0.1F;

      second_object.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
      second_object.vcs_position.y = first_object.vcs_position.y;
      second_object.bbox.Set_Orientation(Angle{ 0.0F });
      second_object.bbox.Set_Length(8.0F);
      second_object.bbox.Set_Width(2.0F);
      second_object.Update_Bbox_Center();
      second_object.speed = 100.0F / 3.6F;
      second_object.vcs_heading.Value(0.0F);
   }
};

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and object heading difference is small such that maximum possible
 *  extension factor should be allowed.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency, Hdg_Diff_Small_Max_Factor_Allowed_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Two objects with same heading.
   *     - Objects have large speed (27m/s such that average speed is above 90kph = 25m/s).
   *     - Objects have short lengths (8m such that the total length is below 18m).
   *     - Objects are positioned such that they are further apart than 2 * 0.125*18m (expected bbox extension).
   *  Changes from TEST_SETUP:
   *     - None
   */

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed closer
 *  together than the expected bbox extension and object heading difference is small such that maximum possible
 *  extension factor should be allowed.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency, Hdg_Diff_Small_Max_Factor_Allowed_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Two objects with same heading.
   *     - Objects have large speed (27m/s such that average speed is above 90kph = 25m/s).
   *     - Objects have short lengths (8m such that the total length is below 18m).
   *  Changes from TEST_SETUP:
   *     - Position objects such that they are closer together than 2 * 0.125*18m (expected bbox extension).
   */
   const float32_t distance_between_objs = 2.0F * 0.125F * 18.0F - 0.1F;
   
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and object heading difference is medium (between 3 and 10 degrees) such that maximum possible
 *  extension factor should be somewhat decreased.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency, Hdg_Diff_Mid_Factor_Reduced_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have large speed (27m/s such that average speed is above 90kph = 25m/s).
   *     - Objects have short lengths (8m such that the total length is below 18m).
   *  Changes from TEST_SETUP:
   *     - Change object headings such that they differ 6.5degrees (6.5degrees is in between 3degrees and 10 degrees).
   *     - Position objects such that they are further apart than 2 * 2/24*18m (expected bbox extension).
   */
   first_object.vcs_heading.Value(F360_DEG2RAD(6.5F));

   const float32_t distance_between_objs = 2.0F * 2.0F/24.0F * 18.0F + 0.1F;
   
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as overlapping when they are placed closer together
 *  than the expected bbox extension and object heading difference is medium (between 3 and 10 degrees) such that maximum possible
 *  extension factor should be somewhat decreased.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency, Hdg_Diff_Mid_Factor_Reduced_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have large speed (27m/s such that average speed is above 90kph = 25m/s).
   *     - Objects have short lengths (8m such that the total length is below 18m).
   *  Changes from TEST_SETUP:
   *     - Change object headings such that they differ 6.5degrees (6.5degrees is in between 3degrees and 10 degrees).
   *     - Position objects such that they are closer together than 2 * 2/24*18m (expected bbox extension).
   */
   first_object.vcs_heading.Value(F360_DEG2RAD(6.5F));

   const float32_t distance_between_objs = 2.0F * 2.0F/24.0F * 18.0F - 0.1F;
   
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed further
 *  apart than the expected bbox extension and object heading difference is large (above 10 degrees) such that minimum
 *  extension factor should always be used.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency, Hdg_Diff_Large_Min_Factor_Used_Not_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have large speed (27m/s such that average speed is above 90kph = 25m/s).
   *     - Objects have short lengths (8m such that the total length is below 18m).
   *  Changes from TEST_SETUP:
   *     - Change object headings such that they differ 11degrees (above 10 degrees).
   *     - Position objects such that they are further apart than 2 * 0.0417*18m (expected bbox extension).
   */
   first_object.vcs_heading.Value(F360_DEG2RAD(11.0F));

   const float32_t distance_between_objs = 2.0F * 0.0417F * 18.0F + 0.1F;
   
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns false.
    */
   CHECK_FALSE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two non-overlapping objects as overlapping");
}

/** \purpose
 *  Checks whether Merge_Bbox_Overlap_Test flags the objects as not overlapping when they are placed closer together
 *  than the expected bbox extension and object heading difference is large (above 10 degrees) such that minimum
 *  extension factor should always be used.
 * \req  NA.
 */
TEST(f360_merge_bbox_overlap_test__BBox_Extension_Object_Heading_Dependency, Hdg_Diff_Large_Min_Factor_Used_Overlapping)
{
   /** \precond
   *  Set up from TEST_SETUP that is being used:
   *     - Objects have large speed (27m/s such that average speed is above 90kph = 25m/s).
   *     - Objects have short lengths (8m such that the total length is below 18m).
   *  Changes from TEST_SETUP:
   *     - Change object headings such that they differ 11degrees (above 10 degrees).
   *     - Position objects such that they are closer together than 2 * 0.0417*18m (expected bbox extension).
   */
   first_object.vcs_heading.Value(F360_DEG2RAD(11.0F));

   const float32_t distance_between_objs = 2.0F * 0.0417F * 18.0F - 0.1F;
   
   second_object.vcs_position.x = first_object.vcs_position.x + distance_between_objs;
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_test_pass = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Check that function returns true.
    */
   CHECK_TRUE_TEXT(f_test_pass, "Function Merge_Bbox_Overlap_Test() is wrongly marking two overlapping objects as not overlapping");
}

/** \brief
 * Test group for side-by-side vs longitudinal classification at slow speed.
 * Focus: verify k_slow_speed_mult_factor = 0.06 (longitudinal) vs 0.0417 (side-by-side).
 */
TEST_GROUP(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal)
{
   // No shared fixture required; 
   // Each test case sets minimal state.
};

/** \purpose
 *  Verify that at slow speed and longitudinal layout (not side-by-side),
 *  the larger low-speed extension factor (0.06) causes overlap at a borderline distance.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal, Not_SideBySide_Slow_Speed_Overlap_With_0p06)
{
   /** \precond
    *  Two objects: length = 8m, width = 2m, heading = 0 rad, speed = 3.0 m/s.
    *  Place along +x so abs(lateral) <= abs(longitudinal) (not side-by-side).
    *  Use center x-gap d = 9.7 m.
    *  Thresholds with total length = 16m:
    *    - with 0.0417: 8 + 2*(0.0417*16) = 9.3344
    *    - with 0.06:   8 + 2*(0.06*16)   = 9.92
    *  9.7 is between them. For not side-by-side slow, factor is 0.06 -> expect overlap.
    */
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   first_object.bbox.Set_Orientation(Angle{ 0.0F });
   first_object.bbox.Set_Length(8.0F);
   first_object.bbox.Set_Width(2.0F);
   first_object.vcs_position = { 0.0F, 0.0F };
   first_object.speed = 3.0F;
   first_object.vcs_heading.Value(0.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Orientation(Angle{ 0.0F });
   second_object.bbox.Set_Length(8.0F);
   second_object.bbox.Set_Width(2.0F);
   second_object.speed = 3.0F;
   second_object.vcs_heading.Value(0.0F);
   second_object.vcs_position = { 9.7F, 0.0F }; // not side-by-side
   second_object.Update_Bbox_Center();

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Expect true: with k_slow_speed_mult_factor = 0.06, extended boxes overlap.
    */
   CHECK_TRUE(f_overlap);
}

/** \purpose
 *  Verify that at the same slow speed but side-by-side layout,
 *  the low-speed extension factor reduces to 0.0417 and overlap no longer occurs
 *  for the same longitudinal gap.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal, SideBySide_Slow_Speed_No_Overlap_With_0p0417)
{
   /** \precond
    *  Same lengths and speeds as the previous test; same longitudinal gap x = 9.7 m.
    *  Force side-by-side by making |y| > |x|. Keep lateral overlap feasible (no width extension),
    *  by using larger widths. Choose x = 9.7 m, y = 9.8 m, widths = 10.0 m.
    *  Side-by-side at slow speed forces factor to 0.0417 -> threshold along x is 9.3344,
    *  so x = 9.7 does not overlap along x anymore.
    */
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   first_object.bbox.Set_Orientation(Angle{ 0.0F });
   first_object.bbox.Set_Length(8.0F);
   first_object.bbox.Set_Width(10.0F);
   first_object.vcs_position = { 0.0F, 0.0F };
   first_object.speed = 3.0F;
   first_object.vcs_heading.Value(0.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Orientation(Angle{ 0.0F });
   second_object.bbox.Set_Length(8.0F);
   second_object.bbox.Set_Width(10.0F);
   second_object.speed = 3.0F;
   second_object.vcs_heading.Value(0.0F);
   second_object.vcs_position = { 9.7F, 9.8F }; // side-by-side: |y| > |x|
   second_object.Update_Bbox_Center();

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Expect false: factor drops to 0.0417 in side-by-side slow case, so no overlap along x.
    *  Lateral overlap is not decisive here because there is no width extension.
    */
   CHECK_FALSE(f_overlap);
}

TEST(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal, Not_SideBySide_Below5mps_Overlap_With_0p06)
{
   /** \purpose
    * Verify not side-by-side at slow speed (< 5.0 m/s) uses factor 0.06 and causes overlap at a borderline x-gap.
    * \req NA.
    */

   /** \precond
    * Two objects: length = 8m, width = 2m, heading = 0 rad, speeds = 4.9 m/s (avg < 5.0).
    * Place along +x so abs(lateral) <= abs(longitudinal) (not side-by-side).
    * Use center x-gap d = 10.0 m.
    * With high-speed length saturation (min=18), thresholds:
    *   factor 0.06   -> x threshold = 8 + 2*(0.06*18)   = 10.16  -> overlap expected (10.0 <= 10.16).
    *   factor 0.0417 -> 9.5012 (for reference in next test).
    */
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   first_object.bbox.Set_Orientation(Angle{ 0.0F });
   first_object.bbox.Set_Length(8.0F);
   first_object.bbox.Set_Width(2.0F);
   first_object.vcs_position = { 0.0F, 0.0F };
   first_object.speed = 4.9F;              // avg < 5.0 m/s -> keep factor 0.06 when not side-by-side
   first_object.vcs_heading.Value(0.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Orientation(Angle{ 0.0F });
   second_object.bbox.Set_Length(8.0F);
   second_object.bbox.Set_Width(2.0F);
   second_object.speed = 4.9F;
   second_object.vcs_heading.Value(0.0F);
   second_object.vcs_position = { 10.0F, 0.0F };  // not side-by-side (lateral = 0)
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Expect true: factor 0.06 at avg speed < 5.0 m/s yields overlap for x-gap 10.0 m.
    */
   CHECK_TRUE(f_overlap);
}

TEST(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal, Not_SideBySide_Above5mps_NoOverlap_With_0p0417)
{
   /** \purpose
    * Verify not side-by-side at higher speed (> 5.0 m/s) reduces factor to 0.0417 and prevents overlap at the same x-gap.
    * \req NA.
    */

   /** \precond
    * Same geometry and headings as previous test.
    * Increase speeds to 5.1 m/s (avg > 5.0) and keep the same x-gap d = 10.0 m.
    * With high-speed length saturation (min=18), factor 0.0417 -> x threshold = 9.5012.
    * Since 10.0 > 9.5012, no overlap is expected.
    */
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   first_object.bbox.Set_Orientation(Angle{ 0.0F });
   first_object.bbox.Set_Length(8.0F);
   first_object.bbox.Set_Width(2.0F);
   first_object.vcs_position = { 0.0F, 0.0F };
   first_object.speed = 5.1F;              // avg > 5.0 m/s -> factor forced to 0.0417 even when not side-by-side
   first_object.vcs_heading.Value(0.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Orientation(Angle{ 0.0F });
   second_object.bbox.Set_Length(8.0F);
   second_object.bbox.Set_Width(2.0F);
   second_object.speed = 5.1F;
   second_object.vcs_heading.Value(0.0F);
   second_object.vcs_position = { 10.0F, 0.0F };  // same gap as previous test
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Expect false: factor 0.0417 at avg speed > 5.0 m/s makes the threshold smaller than 10.0 m.
    */
   CHECK_FALSE(f_overlap);
}

TEST(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal, Not_SideBySide_Below4mps_NoOverlap_With_0p06)
{
   /** \purpose
    * Verify that just below 4.0 m/s, saturated total length stays 16.0 and the x-gap 10.0 m does not overlap.
    * \req NA.
    */

   /** \precond
    * Objects: length = 8m, width = 2m, heading = 0 rad, speeds = 3.99 m/s (avg < 4.0).
    * Not side-by-side: place along +x (y = 0).
    * Thresholds (factor 0.06 since avg < 5 and not side-by-side):
    *   pre-4.0 m/s: saturated_total_length = 16.0  -> x threshold = 8 + 2*(0.06*16)  = 9.92
    * Use x-gap d = 10.0 m, which is > 9.92, so no overlap.
    */
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   first_object.bbox.Set_Orientation(Angle{ 0.0F });
   first_object.bbox.Set_Length(8.0F);
   first_object.bbox.Set_Width(2.0F);
   first_object.vcs_position = { 0.0F, 0.0F };
   first_object.speed = 3.99F;
   first_object.vcs_heading.Value(0.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Orientation(Angle{ 0.0F });
   second_object.bbox.Set_Length(8.0F);
   second_object.bbox.Set_Width(2.0F);
   second_object.speed = 3.99F;
   second_object.vcs_heading.Value(0.0F);
   second_object.vcs_position = { 10.0F, 0.0F }; // not side-by-side
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Expect false (10.0 m > 9.92 m).
    */
   CHECK_FALSE(f_overlap);
}
TEST(f360_merge_bbox_overlap_test__SideBySideVsLongitudinal, Not_SideBySide_AtOrAbove4mps_Overlap_With_0p06)
{
   /** \purpose
    * Verify that at or above 4.0 m/s, saturated total length jumps to 18.0 and the x-gap 10.0 m overlaps.
    * \req NA.
    */

   /** \precond
    * Same geometry as the previous test; set speeds = 4.01 m/s (avg > 4.0, < 5.0).
    * Not side-by-side: place along +x (y = 0).
    * Thresholds (factor 0.06 since avg < 5 and not side-by-side):
    *   post-4.0 m/s: saturated_total_length = 18.0 -> x threshold = 8 + 2*(0.06*18) = 10.16
    * Use same x-gap d = 10.0 m, which is <= 10.16, so overlap.
    */
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   first_object.bbox.Set_Orientation(Angle{ 0.0F });
   first_object.bbox.Set_Length(8.0F);
   first_object.bbox.Set_Width(2.0F);
   first_object.vcs_position = { 0.0F, 0.0F };
   first_object.speed = 4.01F;
   first_object.vcs_heading.Value(0.0F);
   first_object.Update_Bbox_Center();

   second_object.bbox.Set_Orientation(Angle{ 0.0F });
   second_object.bbox.Set_Length(8.0F);
   second_object.bbox.Set_Width(2.0F);
   second_object.speed = 4.01F;
   second_object.vcs_heading.Value(0.0F);
   second_object.vcs_position = { 10.0F, 0.0F }; // not side-by-side
   second_object.Update_Bbox_Center();

   /** \action
    * Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    * Expect true (10.0 m <= 10.16 m).
    */
   CHECK_TRUE(f_overlap);
}
/** @}*/

/** \defgroup  f360_merge_bbox_overlap_test__Rotated_Bboxes
 *  @{
 */

/** \brief
 * Test group for Merge_Bbox_Overlap_Test with rotated bounding boxes.
 */
TEST_GROUP(f360_merge_bbox_overlap_test__Rotated_Bboxes)
{
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};

   TEST_SETUP()
   {
      /* Set up two objects with non-zero bounding box rotation that are expected to overlap when the
      average speed is around 4.2 m/s, i.e. when the higher base extension factor is applied.
      */
      // Object 1
      first_object.bbox.Set_Center(9.5F, -9.8F);
      first_object.bbox.Set_Orientation(Angle{1.7F});
      first_object.bbox.Set_Length(3.0F);
      first_object.bbox.Set_Width(1.2F);
      first_object.speed = 4.0F;
      first_object.vcs_heading.Value(1.7F);

      // Object 2
      second_object.bbox.Set_Center(10.5F, -14.0F);
      second_object.bbox.Set_Orientation(Angle{1.82F});
      second_object.bbox.Set_Length(1.6F);
      second_object.bbox.Set_Width(1.1F);
      second_object.speed = 4.4F;
      second_object.vcs_heading.Value(1.82F);
   }
};

/** \purpose
 *  Verifies Merge_Bbox_Overlap_Test returns true for rotated bounding boxes with specified configuration.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Rotated_Bboxes, Rotated_Bboxes_Overlap_Returns_True)
{
   /** \precond
    *  Use setup: rotated bounding boxes, mean speed 4.2 m/s.
    */

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Should return true.
    */
   CHECK_TRUE(f_overlap);
}

/** \purpose
 *  Verifies Merge_Bbox_Overlap_Test returns true when obj1 is flipped by ~180 degrees (orientation -82.5 deg) 
 *  such that the objects have roughly opposite orientations.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Rotated_Bboxes, Rotated_Bboxes_Overlap_Flipped_Obj1_Returns_True)
{
   /** \precond
    *  Use setup: obj1 orientation flipped by ~180 deg, mean speed 4.2 m/s.
    */
   first_object.bbox.Set_Orientation(Angle{-1.43989663F}); // -82.5 deg in radians
   first_object.vcs_heading.Value(-1.43989663F);

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Should return true.
    */
   CHECK_TRUE(f_overlap);
}

/** \purpose
 *  Verifies Merge_Bbox_Overlap_Test returns false for the same geometry when mean speed > 5 m/s, i.e.
 *  when the base extension factor is reduced to 0.0417 and the boxes are no longer expected to overlap.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Rotated_Bboxes, Rotated_Bboxes_Overlap_MeanSpeedAbove5_Returns_False)
{
   /** \precond
    *  Use setup: mean speed > 5 m/s, geometry as in first test.
    */
   first_object.speed = 6.0F;
   second_object.speed = 6.2F;

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Should return false.
    */
   CHECK_FALSE(f_overlap);
}

/** \purpose
 *  Verifies Merge_Bbox_Overlap_Test returns true when both boxes have zero size but are close enough for
 *  their extended lines (extended boxes become lines since there's no width extension) to overlap.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Rotated_Bboxes, Rotated_Bboxes_ZeroSize_Overlap_Returns_True)
{
   /** \precond
    *  Set both boxes to zero size and move them close enough for extension lines to overlap.
    */
   first_object.bbox.Set_Center(10.3F, -13.0F);
   first_object.bbox.Set_Length(0.0F);
   first_object.bbox.Set_Width(0.0F);

   second_object.bbox.Set_Length(0.0F);
   second_object.bbox.Set_Width(0.0F);

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Should return true.
    */
   CHECK_TRUE(f_overlap);
}

/** \purpose
 *  Verifies Merge_Bbox_Overlap_Test returns true when both object speeds are negative (average speed uses absolute values).
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Rotated_Bboxes, Rotated_Bboxes_NegativeSpeeds_StillOverlaps)
{
   /** \precond
    *  Use default geometry, but set both speeds negative.
    */
   first_object.speed = -4.0F;
   second_object.speed = -4.4F;

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Should return true (average speed is taken as absolute).
    */
   CHECK_TRUE(f_overlap);
}

/** \purpose
 *  Verifies Merge_Bbox_Overlap_Test returns true for large object boxes.
 * \req NA.
 */
TEST(f360_merge_bbox_overlap_test__Rotated_Bboxes, Rotated_Bboxes_LargeBoxes_Overlap_Returns_True)
{
   /** \precond
    *  Set both boxes to large sizes and positions far apart, but still close enough to overlap.
    */
   first_object.bbox.Set_Center(8.0F, 11.8F);
   first_object.bbox.Set_Length(20.0F);
   first_object.bbox.Set_Width(5.0F);

   second_object.bbox.Set_Length(25.0F);
   second_object.bbox.Set_Width(3.0F);

   /** \action
    *  Call Merge_Bbox_Overlap_Test().
    */
   const bool f_overlap = Merge_Bbox_Overlap_Test(first_object, second_object);

   /** \result
    *  Should return true.
    */
   CHECK_TRUE(f_overlap);
}


/** @}*/
