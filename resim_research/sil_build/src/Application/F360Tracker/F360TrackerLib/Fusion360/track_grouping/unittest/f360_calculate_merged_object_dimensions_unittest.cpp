/** \file
 * This file contains unit tests for content of f360_calculate_merged_object_dimensions.cpp file
 */

#include "f360_calculate_merged_object_dimensions.h"
#include <CppUTest/TestHarness.h>
#include <iostream>

using namespace f360_variant_A;

/** \defgroup  f360_calculate_merged_object_dimensions
 *  @{
 */

 /** \brief
  *  Test group for testing Calculate_Merged_Object_Dimensions function.
  */
TEST_GROUP(f360_calculate_merged_object_dimensions)
{
   // Initialize common variables used within all tests in f360_calculate_merged_object_dimensions test group.
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T obj_to_keep = {};
   F360_Object_Track_T obj_to_kill = {};

   // Helper function for verification whether calculated dimensions are equal to expected parameters. 
   bool Are_Dimensions_Equal(const F360_Dimensions_T & expected_dimension, const F360_Dimensions_T & dimension)
   {
      float32_t TOLERANCE = 0.0001F;
      bool f_equal = (std::abs(expected_dimension.length - dimension.length) < TOLERANCE);
      f_equal &= (std::abs(expected_dimension.length - dimension.length) < TOLERANCE);
      f_equal &= (std::abs(expected_dimension.width - dimension.width) < TOLERANCE);
      f_equal &= (std::abs(expected_dimension.len1 - dimension.len1) < TOLERANCE);
      f_equal &= (std::abs(expected_dimension.len2 - dimension.len2) < TOLERANCE);
      f_equal &= (std::abs(expected_dimension.wid1 - dimension.wid1) < TOLERANCE);
      f_equal &= (std::abs(expected_dimension.wid2 - dimension.wid2) < TOLERANCE);
      return f_equal;
   }
};

/** \purpose
 * Test that Calculate_Merged_Object_Dimensions() return expected dimensions for two objects
 * with 0 rad orientation and placed in first quarter in VCS.
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_In_First_Quarter)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    */
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.time_since_initialization = 15.0F;

   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_kill.time_since_initialization = 15.0F;


   F360_Dimensions_T exp_dim;
   exp_dim.length = 5.0F;
   exp_dim.len1 = 2.0F;
   exp_dim.len2 = 3.0F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that Calculate_Merged_Object_Dimensions() return expected dimensions for two objects
 *  with pi/2 rad orientation and placed in second quarter in VCS.
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_In_Second_Quarter)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 15.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_kill.time_since_initialization = 15.0F;


   F360_Dimensions_T exp_dim;
   exp_dim.length = 6.5F;
   exp_dim.len1 = 4.0F;
   exp_dim.len2 = 2.5F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.5F;
   exp_dim.wid2 = 1.5F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 *  Test that Calculate_Merged_Object_Dimensions() returns expected dimensions for two objects
 *  with -pi rad orientation and placed in Third quarter in VCS.
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_In_Third_Quarter)
{
   /** \precond
    * Define two objects. Place object to kill below and on left side of object to keep.
    */
   obj_to_keep.vcs_position.x = -3.0F;
   obj_to_keep.vcs_position.y = -5.0F;
   obj_to_keep.bbox.Set_Length(3.0F);
   obj_to_keep.bbox.Set_Width(1.5F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ -F360_PI });
   obj_to_keep.time_since_initialization = 15.0F;

   obj_to_kill.vcs_position.x = -2.0F;
   obj_to_kill.vcs_position.y = -4.0F;
   obj_to_kill.bbox.Set_Length(3.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ -F360_PI });
   obj_to_kill.time_since_initialization = 15.0F;

   F360_Dimensions_T exp_dim;
   exp_dim.length = 4.0F;
   exp_dim.len1 = 2.5F;
   exp_dim.len2 = 1.5F;
   exp_dim.width = 2.75F;
   exp_dim.wid1 = 2.0F;
   exp_dim.wid2 = 0.75F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 *  Test that Calculate_Merged_Object_Dimensions() returns expected dimensions for two objects
 *  with pi/4 rad orientation and placed in fourth quarter in VCS.
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_In_Fourth_Quarter)
{
   /** \precond
    * Define two objects. Place object to kill above and on left side of object to keep.
    */
   obj_to_keep.vcs_position.x = -3.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(3.0F);
   obj_to_keep.bbox.Set_Width(1.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_keep.time_since_initialization = 11.0F;


   obj_to_kill.vcs_position.x = -2.0F;
   obj_to_kill.vcs_position.y = 5.0F;
   obj_to_kill.bbox.Set_Length(3.0F);
   obj_to_kill.bbox.Set_Width(1.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 15.0F;

   F360_Dimensions_T exp_dim;
   exp_dim.length = 3.7071F;
   exp_dim.len1 = 1.5;
   exp_dim.len2 = 2.2071F;
   exp_dim.width = 1.9571F;
   exp_dim.wid1 = 1.4571F;
   exp_dim.wid2 = 0.5F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that Calculate_Merged_Object_Dimensions() return expected dimensions for two objects
 *  with different rad orientation.
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Diff_In_Orientation)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    * Define different orientation for both objects.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 11.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 15.0F;

   F360_Dimensions_T exp_dim;
   exp_dim.length = 6.7981F;
   exp_dim.len1 = 4.2981F;
   exp_dim.len2 = 2.5F;
   exp_dim.width = 4.5962F;
   exp_dim.wid1 = 1.7981F;
   exp_dim.wid2 = 2.7981F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}


/** \purpose
 * Test that the time ratio is based on the yougest object, although the object to kill is older
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, check_time_ratio_when_obj_to_kill_is_older)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    * Define different orientation for both objects.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 11.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 15.0F;

   float expected_ratio = 0.733F;

   /** \action
    * call Calc_Time_Ratio().
    */
   const float time_ratio = Calc_Time_Ratio(obj_to_keep.time_since_initialization, obj_to_kill.time_since_initialization);

   /** \result
    * Check that the time ratio is obj_to_keep.time_since_initialization/obj_to_kill.time_since_initialization, which here is 11/15.
    */
   DOUBLES_EQUAL(expected_ratio, time_ratio, 0.001);
}

/** \purpose
 * Test that the time ratio is based on the yougest object, when the object to keep is older
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, check_time_ratio_when_obj_to_keep_is_older)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    * Define different orientation for both objects.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 15.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 11.0F;

   float expected_ratio = 0.733F;

   /** \action
    * call Calc_Time_Ratio().
    */
   const float time_ratio = Calc_Time_Ratio(obj_to_keep.time_since_initialization, obj_to_kill.time_since_initialization);

   /** \result
    * Check that the time ratio is obj_to_kill.time_since_initialization/obj_to_keep.time_since_initialization, which here is 11/15.
    */
   DOUBLES_EQUAL(expected_ratio, time_ratio, 0.001);
}


/** \purpose
 * Test that the weight for mature objects is returned as 0, no special handling
 * for dimensions based on age shall be considered
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, check_weights_when_both_objs_mature)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    * Define different orientation for both objects.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 50.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 50.0F;

   float expected_weight = 0.0F;

   /** \action
    * call Calc_Time_Ratio().
    */
   const float weight = Calc_Weight(obj_to_keep.time_since_initialization, obj_to_kill.time_since_initialization);

   /** \result
    * Check that the expected weight is 0
    */
   DOUBLES_EQUAL(expected_weight, weight, 0.001);
}

/** \purpose
 * Test that the weights when the object to kill is older, is the ratio of the younger_object_age/older_object_age
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, check_weights_when_obj_to_kill_is_older)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    * Define different orientation for both objects.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 5.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 9.0F;

   float expected_weight = 0.444F;

   /** \action
    * call Calc_Weight().
    */
   const float weight = Calc_Weight(obj_to_keep.time_since_initialization, obj_to_kill.time_since_initialization);

   /** \result
    * Check that the expected weight is (1-obj_to_keep.time_since_initialization/obj_to_kill.time_since_initialization).
    */
   DOUBLES_EQUAL(expected_weight, weight, 0.001);
}

/** \purpose
 * Test that the weights when the object to keep is older, is the ratio of the younger_object_age/older_object_age
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, check_weights_when_obj_to_keep_is_older)
{
   /** \precond
    * Define two objects. Place object to kill below and on right side of object to keep.
    * Define different orientation for both objects.
    */
   obj_to_keep.vcs_position.x = 3.0F;
   obj_to_keep.vcs_position.y = -4.0F;
   obj_to_keep.bbox.Set_Length(5.0F);
   obj_to_keep.bbox.Set_Width(3.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ F360_PI_2 });
   obj_to_keep.time_since_initialization = 9.0F;

   obj_to_kill.vcs_position.x = 2.5F;
   obj_to_kill.vcs_position.y = -6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.5F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
   obj_to_kill.time_since_initialization = 5.0F;

   float expected_weight = 0.444F;

   /** \action
    * call Calc_Weight().
    */
   const float weight = Calc_Weight(obj_to_keep.time_since_initialization, obj_to_kill.time_since_initialization);

   /** \result
    * Expect weight as (1-obj_to_kill.time_since_initialization/obj_to_keep.time_since_initialization)
    */
   DOUBLES_EQUAL(expected_weight, weight, 0.001);
}

/** \purpose
 * Test that confirms Calculate_Merged_Object_Dimensions return merged dimensions close to dimensions of kept object
 * when there is a significant object age difference.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Significant_Diff_In_Age)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set kept obj time_since_init to be significantly larger when compared to kill obj so the ratio is very small.
    */
   obj_to_keep.time_since_initialization = 100.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });

   obj_to_kill.time_since_initialization = 1.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });

   F360_Dimensions_T exp_dim;
   exp_dim.length = 4.01F;
   exp_dim.len1 = 2.0F;
   exp_dim.len2 = 2.01F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test Final_Merged_Obj_Dimensions() return expected dimensions of merged objects
 * based on the weight to trust the original dimension and the measured dimension.
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, check_final_merged_objects_dimensions)
{
   /** \precond
    * Define two objects, one is the meausered dimension, other is the original
    * and define weights to trust the original dimension more than the measured
    */
   
   const float32_t measured_len1 = 2.2981F;
   const float32_t measured_len2 = 2.5F;

   F360_Dimensions_T dimensions;
   dimensions.length = 6.7981F;
   dimensions.len1 = 4.2981F;
   dimensions.len2 = 2.5F;
   dimensions.width = 4.5962F;
   dimensions.wid1 = 1.7981F;
   dimensions.wid2 = 2.7981F;

   F360_Dimensions_T exp_dim;
   exp_dim.length = 6.3981F;
   exp_dim.len1 = 3.8981;
   exp_dim.len2 = 2.5F;
   exp_dim.width = 4.5962F;
   exp_dim.wid1 = 1.7981F;
   exp_dim.wid2 = 2.7981F;

   const float32_t alpha_trust_original_dimension = 0.8;
   const float32_t alpha_trust_measured_dimension = 0.2;

   /** \action
    * call Final_Merged_Obj_Dimensions().
    */
   const F360_Dimensions_T dim = Final_Merged_Obj_Dimensions(dimensions, alpha_trust_original_dimension, alpha_trust_measured_dimension, measured_len1, measured_len2);

   /** \result
    * Expect the dimensions should be close the the original dimension
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}


/** \purpose
 * Test that confirms Calculate_Merged_Object_Dimensions return merged to dimensions of kept object
 * when there is a significant object age difference and associated detection outside the boundary box behind
 * the objects.
 *
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Significant_Diff_In_Age_With_Det_Behind)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set kept obj time_since_init to be significantly larger when compared to kill obj so the ratio is very small.
    * Set an associated detection with pos.x behind the killed object.
    */
   det_props[0].vcs_position.x = 15.0F;
   det_props[0].vcs_position.y = 5.0F;


   obj_to_keep.time_since_initialization = 100.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });


   obj_to_kill.time_since_initialization = 1.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_kill.ndets = 1U;
   obj_to_kill.detids[0] = 1U;

   F360_Dimensions_T exp_dim;
   exp_dim.length = 6.98F;
   exp_dim.len1 = 2.0F;
   exp_dim.len2 = 4.98F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that confirms Calculate_Merged_Object_Dimensions return merged to dimensions of kept object
 * when there is a significant object age difference and associated detection outside the boundary box.
 *
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Significant_Diff_In_Age_With_Det_In_Front)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set kept obj time_since_init to be significantly larger when compared to kill obj so the ratio is very small.
    * Set an associated detection with pos.x behind the killed object.
    */
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 5.0F;

   obj_to_keep.time_since_initialization = 100.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 1U;

   obj_to_kill.time_since_initialization = 1.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });


   F360_Dimensions_T exp_dim;
   exp_dim.length = 11.93F;
   exp_dim.len1 = 9.92F;
   exp_dim.len2 = 2.01F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that confirms Calculate_Merged_Object_Dimensions return merged to dimensions of kept object
 * when there is a significant object age difference and associated detection inside the boundary box of kept object.
 *
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Significant_Diff_In_Age_With_Det_Inside_Bbox)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set kept obj time_since_init to be significantly larger when compared to kill obj so the ratio is very small.
    * Set an associated detection with pos.x inside the kept object bbox.
    */
   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = 5.0F;

   obj_to_keep.time_since_initialization = 100.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 1U;

   obj_to_kill.time_since_initialization = 1.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });


   F360_Dimensions_T exp_dim;
   exp_dim.length = 4.01F;
   exp_dim.len1 = 2.0F;
   exp_dim.len2 = 2.01F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that Calculate_Merged_Object_Dimensions return proper merged obj dimensions
 * when there is a medium object age difference.
 *
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Medium_Diff_In_Age)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set kept obj and killed obj time_since_init so the ratio is at 0.5
    */
   obj_to_keep.time_since_initialization = 10.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });

   obj_to_kill.time_since_initialization = 5.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });

   F360_Dimensions_T exp_dim;
   exp_dim.length = 4.5F;
   exp_dim.len1 = 2.0F;
   exp_dim.len2 = 2.5F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that Calculate_Merged_Object_Dimensions return proper merged obj dimensions
 * when there is a small object age difference.
 *
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Calculate_Merged_Object_Small_Diff_In_Age)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set kept obj and killed obj time_since_init so the ratio is at 1.0
    */
   obj_to_keep.time_since_initialization = 5.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });

   obj_to_kill.time_since_initialization = 5.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });

   F360_Dimensions_T exp_dim;
   exp_dim.length = 5.0F;
   exp_dim.len1 = 2.0F;
   exp_dim.len2 = 3.0F;
   exp_dim.width = 3.0F;
   exp_dim.wid1 = 1.0F;
   exp_dim.wid2 = 2.0F;

   /** \action
    * call Calculate_Merged_Object_Dimensions().
    */
   const F360_Dimensions_T dim = Calculate_Merged_Object_Dimensions(det_props, obj_to_keep, obj_to_kill);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_TRUE(Are_Dimensions_Equal(exp_dim, dim));
}

/** \purpose
 * Test that Det_Based_Merged_Obj_Length returns proper length estimation
 * when there is one detection associated to kept object.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Test_Det_Based_Merged_Obj_Len_obj_to_keep_det)
{
   /** \precond
    * Define two objects with exact same dimension. Place object to kill above and on right side of object to keep.
    * Set an associated detection with pos.x in front the kept object bbox.
    */
   det_props[0].vcs_position.x = 15.0F;
   det_props[0].vcs_position.y = 5.0F;

   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 1U;

   obj_to_kill.time_since_initialization = 1.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });

   float32_t measured_len1 = 0.0F;
   float32_t measured_len2 = 0.0F;
   const float32_t exp_dim_len1 = 2.0F;
   const float32_t exp_dim_len2 = 5.0F;
   
   /** \action
    * call Det_Based_Merged_Obj_Length().
    */
   Det_Based_Merged_Obj_Length(det_props, obj_to_keep, obj_to_kill, measured_len1, measured_len2);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_EQUAL(exp_dim_len1, measured_len1);
   CHECK_EQUAL(exp_dim_len2, measured_len2);
}

/** \purpose
 * Test that Det_Based_Merged_Obj_Length returns proper length estimation
 * when there is one detection associated to the killed object.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, Test_Det_Based_Merged_Obj_Length_obj_to_kill_det)
{
   /** \precond
    * Define objects dimension.
    * Set an associated detection with pos.x behind the second object bbox.
    */
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 5.0F;

   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 0U;
  

   obj_to_kill.time_since_initialization = 1.0F;
   obj_to_kill.vcs_position.x = 11.0F;
   obj_to_kill.vcs_position.y = 6.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 1U;

   float32_t measured_len1 = 0.0F;
   float32_t measured_len2 = 0.0F;
   const float32_t exp_len1 = 10.0F;
   const float32_t exp_len2 = 2.0F;

   /** \action
    * call Det_Based_Merged_Obj_Length().
    */
   Det_Based_Merged_Obj_Length(det_props, obj_to_keep, obj_to_kill, measured_len1, measured_len2);

   /** \result
    * Check that the dimension match expected data.
    */
   CHECK_EQUAL(exp_len1, measured_len1);
   CHECK_EQUAL(exp_len2, measured_len2);
   
}

/** \purpose
 * Test that Update_Min_Max_TCS_Pos returns properly updated minimum and maximum
 * position in tcs coordinate system when one detection is available.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, update_min_max_tcs_pos_single_det)
{
   /** \precond
    * Define objects dimension.
    * Set an associated detection with pos.x in front of object bbox.
    */
   uint32_t det_idx = 0U;
   det_props[det_idx].vcs_position.x = 15.0F;
   det_props[det_idx].vcs_position.y = 5.0F;

   obj_to_keep.time_since_initialization = 5.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 1U;

   float32_t det_x_tcs_max_pos = obj_to_keep.bbox.Get_Length() * 0.5F;
   float32_t det_x_tcs_min_pos = -obj_to_keep.bbox.Get_Length() * 0.5F;

   /** \action
    * call Update_Min_Max_TCS_Pos().
    */

   Update_Min_Max_TCS_Pos(det_props[det_idx], obj_to_keep, det_x_tcs_max_pos, det_x_tcs_min_pos);

   /** \result
    * Check that the det_x_tcs_max_pos and det_x_tcs_min_pos updated properly.
    */
   DOUBLES_EQUAL(5.0F, det_x_tcs_max_pos, F360_EPSILON);
   DOUBLES_EQUAL(-2.0F, det_x_tcs_min_pos, F360_EPSILON);
}

/** \purpose
 * Test that Update_Min_Max_TCS_Pos returns properly updated minimum and maximum
 * position in tcs coordinate system when two detections are available.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, update_min_max_tcs_pos_two_dets)
{
   /** \precond
    * Define object dimension.
    * Set two associated detections with pos.x in front and behind of object bbox.
    */
   det_props[0].vcs_position.x = 15.0F;
   det_props[0].vcs_position.y = 5.0F;
   det_props[1].vcs_position.x = 1.0F;
   det_props[1].vcs_position.y = 5.0F;

   obj_to_keep.time_since_initialization = 5.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 2U;
   obj_to_keep.detids[0] = 1U;
   obj_to_keep.detids[1] = 2U;

   float32_t det_x_tcs_max_pos = obj_to_keep.bbox.Get_Length() * 0.5F;
   float32_t det_x_tcs_min_pos = -obj_to_keep.bbox.Get_Length() * 0.5F;

   /** \action
    * call Update_Min_Max_TCS_Pos().
    */

   for (uint8_t i = 0U; i < obj_to_keep.ndets; i++)
   {
      const uint32_t det_idx = obj_to_keep.detids[i] - 1U;
      Update_Min_Max_TCS_Pos(det_props[det_idx], obj_to_keep, det_x_tcs_max_pos, det_x_tcs_min_pos);
   }

   /** \result
    * Check that the det_x_tcs_max_pos and det_x_tcs_min_pos updated properly.
    */
   DOUBLES_EQUAL(5.0F, det_x_tcs_max_pos, F360_EPSILON);
   DOUBLES_EQUAL(-9.0F, det_x_tcs_min_pos, F360_EPSILON);
}

/** \purpose
 * Test that Det_Based_Merged_Obj_Length returns properly updated length
 * when kept object is behind and detections are available.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, det_based_len_kept_obj_behind_two_dets)
{
   /** \precond
    * Define object dimension.
    * Define kept object position so its behind killed object
    * Set two associated detections with pos.x in front and behind of object bbox.
    */

   float32_t measured_len1 = 0.0F;
   float32_t measured_len2 = 0.0F;

   det_props[0].vcs_position.x = 20.0F;
   det_props[0].vcs_position.y = 5.0F;
   det_props[1].vcs_position.x = 1.0F;
   det_props[1].vcs_position.y = 5.0F;

   obj_to_keep.time_since_initialization = 25.0F;
   obj_to_keep.vcs_position.x = 10.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 2U;


   obj_to_kill.time_since_initialization = 5.0F;
   obj_to_kill.vcs_position.x = 15.0F;
   obj_to_kill.vcs_position.y = 5.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_kill.ndets = 1U;
   obj_to_kill.detids[0] = 1U;

   /** \action
    * call Det_Based_Merged_Obj_Length().
    */
   Det_Based_Merged_Obj_Length(det_props, obj_to_keep, obj_to_kill, measured_len1, measured_len2);

   /** \result
    *  Check that the measured_len1 and measured_len2 are updated properly.
    */
   DOUBLES_EQUAL(9.0F, measured_len1, F360_EPSILON);
   DOUBLES_EQUAL(10.0F, measured_len2, F360_EPSILON);
}

/** \purpose
 * Test that Det_Based_Merged_Obj_Length returns properly updated length
 * when kept object is infront and detections are available.
 *
 * \req  NA.
 */
TEST(f360_calculate_merged_object_dimensions, det_based_len_kept_obj_infront_two_dets)
{
   /** \precond
    * Define object dimension.
    * Define kept object position so its infront of killed object
    * Set two associated detections with pos.x in front and behind of objects bbox.
    */
   float32_t measured_len1 = 0.0F;
   float32_t measured_len2 = 0.0F;

   det_props[0].vcs_position.x = 20.0F;
   det_props[0].vcs_position.y = 5.0F;
   det_props[1].vcs_position.x = 1.0F;
   det_props[1].vcs_position.y = 5.0F;

   obj_to_keep.time_since_initialization = 25.0F;
   obj_to_keep.vcs_position.x = 15.0F;
   obj_to_keep.vcs_position.y = 5.0F;
   obj_to_keep.bbox.Set_Length(4.0F);
   obj_to_keep.bbox.Set_Width(2.0F);
   obj_to_keep.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_keep.ndets = 1U;
   obj_to_keep.detids[0] = 1U;

   obj_to_kill.time_since_initialization = 5.0F;
   obj_to_kill.vcs_position.x = 10.0F;
   obj_to_kill.vcs_position.y = 5.0F;
   obj_to_kill.bbox.Set_Length(4.0F);
   obj_to_kill.bbox.Set_Width(2.0F);
   obj_to_kill.Set_Bbox_Orientation(Angle{ 0.0F });
   obj_to_kill.ndets = 1U;
   obj_to_kill.detids[0] = 2U;

   /** \action
    * call Det_Based_Merged_Obj_Length().
    */
   Det_Based_Merged_Obj_Length(det_props, obj_to_keep, obj_to_kill, measured_len1, measured_len2);

   /** \result
    * Check that the measured_len1 and measured_len2 are updated properly.
    */
   DOUBLES_EQUAL(14.0F, measured_len1, F360_EPSILON);
   DOUBLES_EQUAL(2.0F, measured_len2, F360_EPSILON);
}
/** @}*/
