/** \file
 * This file contains unit tests for content of f360_angle.cpp file
 */

#include "f360_angle.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Angle_Constructors_Tests
 *  @{
 */

/** \brief
 * Group for testing Angle class constructors
 */
TEST_GROUP(Angle_Constructors_Tests)
{	
   const float32_t test_epsilon = 1e-6F;
};

/** \purpose  
 * Test behavior of default constructor.
 * \req
 * NA
 */
TEST(Angle_Constructors_Tests, Default_Constructor_Test)
{
   /** \precond
    * NA
    */
	
   /** \action
    * Create Angle instance using default constructor.
    */
   Angle ang;
   
   /** \result
    * Check if value, sine and cosine are set correctly.
    */
   const float32_t exp_ang_value = 0.0F;
   const float32_t exp_sin_value = 0.0F;
   const float32_t exp_cos_value = 1.0F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of single float constructor when only angle value is passed.
 * \req
 * NA
 */
TEST(Angle_Constructors_Tests, Single_Float_Constructor_Test)
{
   /** \precond
    * NA
    */

    /** \action
     * Create Angle instance using single float constructor.
     */
   const float32_t angle_value_rad = 0.1F;
   Angle ang(angle_value_rad);

   /** \result
    * Check if Angle::value is set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
}

/** \purpose
 * Test behavior of three floats constructor when angle value, sin and cos are passed.
 * \req
 * NA
 */
TEST(Angle_Constructors_Tests, Three_Floats_Constructor_Test)
{
   /** \precond
    * NA
    */

    /** \action
     * Create Angle instance using three floats constructor.
     */
   const float32_t angle_value_rad = F360_DEG2RAD(180.0F);
   const float32_t sin_value = 1.0F;
   const float32_t cos_value = 0.0F;
   Angle ang(angle_value_rad, sin_value, cos_value);

   /** \result
    * Check if Angle instance properties are set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;
   const float32_t exp_sin_value = sin_value;
   const float32_t exp_cos_value = cos_value;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}
/** @}*/


/** \defgroup  Angle_Setters_Getters_Tests
 *  @{
 */

 /** \brief
  * Group for testing Angle setters and getters: Angle::Value, Angle::Value, Angle::Value_Deg, Angle::Sin, Angle::Cos methods.
  */
TEST_GROUP(Angle_Setters_Getters_Tests)
{
   const float32_t test_epsilon = 1e-6F;
};

/** \purpose
 * Test behavior of Angle::Value and Angle::Value methods.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Set_Value_And_Value_Test)
{
   /** \precond
    * Create Angle instance with default constructor.
    */
   Angle ang;

    /** \action
     * Set angle value to 30 deg.
     */
   const float32_t angle_value_rad = F360_DEG2RAD(30.0F);
   ang.Value(angle_value_rad);

   /** \result
    * Check if Angle instance properties are set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Sin method.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Sin_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(30.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Get sine value
    */
   const float32_t sin_value = ang.Sin();

   /** \result
    * Check if sine value was evaluated correctly.
    */
   const float32_t exp_sin_value = 0.5F;

   DOUBLES_EQUAL(exp_sin_value, sin_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Sin method when given angle is negative.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Sin_Negative_Angle_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(-30.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Get sine value
    */
   const float32_t sin_value = ang.Sin();

   /** \result
    * Check if sine value was evaluated correctly.
    */
   const float32_t exp_sin_value = -0.5F;

   DOUBLES_EQUAL(exp_sin_value, sin_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Sin method when angle is over 360 deg.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Sin_Test_When_Angle_Is_Over_360)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(390.0F);  // 360 + 30
   Angle ang(angle_value_rad);

   /** \action
    * Get sine value
    */
   const float32_t sin_value = ang.Sin();

   /** \result
    * Check if sine value was evaluated correctly.
    */
   const float32_t exp_sin_value = 0.5F;

   DOUBLES_EQUAL(exp_sin_value, sin_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Sin after setting angle value other then initial one.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Sin_After_Setting_Different_Value_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(20.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Get sine value. After that set other angle value and evaluate sine again.
    */
   const float32_t init_sin_value = ang.Sin();
   ang.Value(F360_DEG2RAD(30.0F));
   const float32_t sin_value = ang.Sin();

   /** \result
    * Check if sine value was evaluated correctly.
    */
   const float32_t exp_init_sin_value = 0.3420201433F;
   const float32_t exp_sin_value = 0.5F;

   DOUBLES_EQUAL(exp_init_sin_value, init_sin_value, test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, sin_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Cos method.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Cos_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(60.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Get cosine value
    */
   const float32_t cos_value = ang.Cos();

   /** \result
    * Check if cosine value was correctly evaluated.
    */
   const float32_t exp_cos_value = 0.5F;

   DOUBLES_EQUAL(exp_cos_value, cos_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Cos method when given angle is negative.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Cos_Negative_Angle_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(-60.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Get cosine value
    */
   const float32_t cos_value = ang.Cos();

   /** \result
    * Check if cosine value was correctly evaluated.
    */
   const float32_t exp_cos_value = 0.5F;

   DOUBLES_EQUAL(exp_cos_value, cos_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Cos method when angle is over 360 deg.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Cos_Test_When_Angle_Is_Over_360)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(420.0F);  // 360 + 60
   Angle ang(angle_value_rad);

   /** \action
    * Get cosine value
    */
   const float32_t cos_value = ang.Cos();

   /** \result
    * Check if cosine value was correctly evaluated.
    */
   const float32_t exp_cos_value = 0.5F;

   DOUBLES_EQUAL(exp_cos_value, cos_value, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Value_Deg method.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Value_Deg_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_PI;
   Angle ang(angle_value_rad);

   /** \action
    * Get angle value in degrees.
    */
   const float32_t ang_value_dev = ang.Value_Deg();

   /** \result
    * Check if angle value in degree is correct.
    */
   const float32_t exp_ang_value = 180.0F;

   DOUBLES_EQUAL(exp_ang_value, ang_value_dev, test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Negate method when angle value is 0.0.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Negate_When_Value_Is_0_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = 0.0F;
   Angle ang(angle_value_rad);

   /** \action
    * Call Negate() method on created object.
    */
   ang.Negate();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = 0.0F;
   const float32_t exp_sin_value = 0.0F;
   const float32_t exp_cos_value = 1.0F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Negate method when angle value is positive.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Negate_When_Value_Is_Positive_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(30.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Negate() method on created object.
    */
   ang.Negate();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = -angle_value_rad;
   const float32_t exp_sin_value = -0.5F;
   const float32_t exp_cos_value = 0.866025F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Negate method when angle value is negative.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Negate_When_Value_Is_Negative_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(-30.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Negate() method on created object.
    */
   ang.Negate();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = -angle_value_rad;
   const float32_t exp_sin_value = 0.5F;
   const float32_t exp_cos_value = 0.866025F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Negate method when angle sine and cosine are already evaluated.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Negate_When_Sin_And_Cos_Are_Already_Evaluated)
{
   /** \precond
    * Create Angle instance with given angle value.
    * Call Sin() and Cos()
    */
   const float32_t angle_value_rad = F360_DEG2RAD(30.0F);
   Angle ang(angle_value_rad);
   ang.Sin();
   ang.Cos();

   /** \action
    * Call Negate() method on created object.
    */
   ang.Negate();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = -angle_value_rad;
   const float32_t exp_sin_value = -0.5F;
   const float32_t exp_cos_value = 0.866025F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Normalize method when angle value is 0.0.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Normalize_When_Value_Is_0_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(0.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Normalize() method on created object.
    */
   ang.Normalize();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;
   const float32_t exp_sin_value = 0.0F;
   const float32_t exp_cos_value = 1.0F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Normalize method when angle value is positive.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Normalize_When_Value_Is_Positive_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(30.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Normalize() method on created object.
    */
   ang.Normalize();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;
   const float32_t exp_sin_value = 0.5F;
   const float32_t exp_cos_value = 0.866025F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Normalize method when angle value is nagative.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Normalize_When_Value_Is_Negative_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(-30.0F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Normalize() method on created object.
    */
   ang.Normalize();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;
   const float32_t exp_sin_value = -0.5F;
   const float32_t exp_cos_value = 0.866025F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Normalize method when angle value is over 180 degree.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Normalize_When_Value_Is_Over_180_Deg_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(180.01F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Normalize() method on created object.
    */
   ang.Normalize();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = F360_DEG2RAD(-179.99F);
   const float32_t exp_sin_value = -1.745329e-4F;
   const float32_t exp_cos_value = -1.0F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Normalize method when angle value is below  -180 degree.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Normalize_When_Value_Is_Below_Minus_180_Deg_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(-180.01F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Normalize() method on created object.
    */
   ang.Normalize();

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = F360_DEG2RAD(179.99F);
   const float32_t exp_sin_value = 1.745329e-4F;
   const float32_t exp_cos_value = -1.0F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of Angle::Normalize method when angle value is over 360 deg and interval center is in 180 deg.
 * \req
 * NA
 */
TEST(Angle_Setters_Getters_Tests, Normalize_When_Value_Is_Over_180_And_Interval_Is_180_Test)
{
   /** \precond
    * Create Angle instance with given angle value.
    */
   const float32_t angle_value_rad = F360_DEG2RAD(180.1F);
   Angle ang(angle_value_rad);

   /** \action
    * Call Normalize() method on created object.
    */
   const float32_t interval_center = F360_DEG2RAD(180.0F);
   ang.Normalize(interval_center);

   /** \result
    * Check if angle attributes are set correctly.
    */
   const float32_t exp_ang_value = angle_value_rad;
   const float32_t exp_sin_value = -0.001745F;
   const float32_t exp_cos_value = -0.999998F;

   DOUBLES_EQUAL(exp_ang_value, ang.Value(), test_epsilon);
   DOUBLES_EQUAL(exp_sin_value, ang.Sin(), test_epsilon);
   DOUBLES_EQUAL(exp_cos_value, ang.Cos(), test_epsilon);
}

/** @}*/

/** \defgroup  Angle_Vs_Angle_Operators_Tests
 *  @{
 */

 /** \brief
  * Group for testing Angle operators.
  */
TEST_GROUP(Angle_Vs_Angle_Operators_Tests)
{
   const float32_t test_epsilon = 1.19e-7F;
};

/** \purpose
 * Test behavior of < operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Is_Lower_Operator_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F + test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_lower = ang_1 < ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower);
}

/** \purpose
 * Test behavior of > operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Is_Greater_Operator_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F - test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_greater = ang_1 > ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater);
}

/** \purpose
 * Test behavior of == operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Are_Equal_Operator_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool are_equal = ang_1 == ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(are_equal);
}

/** \purpose
 * Test behavior of != operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Are_Not_Equal_Operator_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F + test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool are_not_equal = ang_1 != ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(are_not_equal);
}

/** \purpose
 * Test behavior of <= operator when left value is lower then right.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Is_Lower_Or_Equal_Operator_When_Is_Lower_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F + test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_lower_or_equal = ang_1 <= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower_or_equal);
}

/** \purpose
 * Test behavior of <= operator when left value equal right.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Is_Lower_Or_Equal_Operator_When_Are_Equal_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool is_lower_or_equal = ang_1 <= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower_or_equal);
}

/** \purpose
 * Test behavior of >= operator when left value is greater.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Is_Greater_Or_Equal_Operator_When_Is_Greater_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F - test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_greater_or_equal = ang_1 >= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater_or_equal);
}

/** \purpose
 * Test behavior of >= operator when values are equal.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Is_Greater_Or_Equal_Operator_When_Are_Equal_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   Angle ang_1(0.1F);
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool is_greater_or_equal = ang_1 >= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater_or_equal);
}

/** \purpose
 * Test behavior of + operator when for two input angles trygonometric functions were not evaluated before.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Without_Sin_Cos_Evaluated_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Add two angles.
    */
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when for left value sin was evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Sin_Evaluated_For_First_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin for ang_1. Add two angles.
    */
   ang_1.Sin();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when for left value cos was evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Cos_Evaluated_For_First_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate cos for ang_1. Add two angles.
    */
   ang_1.Cos();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when for right value sin was evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Sin_Evaluated_For_Second_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin for ang_1. Add two angles.
    */
   ang_2.Sin();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when for right value cos was evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Cos_Evaluated_For_Second_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate cos for ang_1. Add two angles.
    */
   ang_2.Cos();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when only for left value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Sin_Cos_Evaluated_For_First_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Add two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when only for right value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Sin_Cos_Evaluated_For_Second_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Add two angles.
    */
   ang_2.Sin();
   ang_2.Cos();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of + operator when for both angles trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Operator_Sin_Cos_Evaluated_For_Both_Angles_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1 and ang_2. Add two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   ang_2.Sin();
   ang_2.Cos();
   Angle ang_sum = ang_1 + ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_sum.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_sum.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_sum.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of += operator when for two input angles trygonometric functions were not evaluated before.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Equal_Operator_Without_Sin_Cos_Evaluated_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Add two angles.
    */
   ang_1 += ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of += operator when only for left value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Equal_Operator_Sin_Cos_Evaluated_For_First_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Add two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   ang_1 += ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of += operator when only for right value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Equal_Operator_Sin_Cos_Evaluated_For_Second_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Add two angles.
    */
   ang_2.Sin();
   ang_2.Cos();
   ang_1 += ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of += operator when for both angles trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Plus_Equal_Operator_Sin_Cos_Evaluated_For_Both_Angles_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(10.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1 and ang_2. Add two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   ang_2.Sin();
   ang_2.Cos();
   ang_1 += ang_2;

   /** \result
    * Check if sum of angle has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of - operator when for two input angles trygonometric functions were not evaluated before.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Operator_Without_Sin_Cos_Evaluated_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Subtract two angles.
    */
   Angle ang_diff = ang_1 - ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_diff.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_diff.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_diff.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of - operator when only for left value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Operator_Sin_Cos_Evaluated_For_First_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Subtract two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   Angle ang_diff = ang_1 - ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_diff.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_diff.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_diff.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of - operator when only for right value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Operator_Sin_Cos_Evaluated_For_Second_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Subtract two angles.
    */
   ang_2.Sin();
   ang_2.Cos();
   Angle ang_diff = ang_1 - ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_diff.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_diff.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_diff.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of - operator when for both angles trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Operator_Sin_Cos_Evaluated_For_Both_Angles_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1 and ang_2. Subtract two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   ang_2.Sin();
   ang_2.Cos();
   Angle ang_diff = ang_1 - ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_diff.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_diff.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_diff.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of -= operator when for two input angles trygonometric functions were not evaluated before.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Equal_Operator_Without_Sin_Cos_Evaluated_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Subtract two angles.
    */
   ang_1 -= ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of -= operator when only for left value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Equal_Operator_Sin_Cos_Evaluated_For_First_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Subtract two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   ang_1 -= ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of -= operator when only for right value trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Equal_Operator_Sin_Cos_Evaluated_For_Second_Angle_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1. Subtract two angles.
    */
   ang_2.Sin();
   ang_2.Cos();
   ang_1 -= ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of -= operator when for both angles trygonometric functions were evaluated.
 * \req
 * NA
 */
TEST(Angle_Vs_Angle_Operators_Tests, Minus_Equal_Operator_Sin_Cos_Evaluated_For_Both_Angles_Test)
{
   /** \precond
    * Create two Angle instances with given angle value.
    */
   const float32_t ang_1_val = F360_DEG2RAD(50.0F);
   const float32_t ang_2_val = F360_DEG2RAD(20.0F);
   Angle ang_1(ang_1_val);
   Angle ang_2(ang_2_val);

   /** \action
    * Evaluate sin and cos for ang_1 and ang_2. Subtract two angles.
    */
   ang_1.Sin();
   ang_1.Cos();
   ang_2.Sin();
   ang_2.Cos();
   ang_1 -= ang_2;

   /** \result
    * Check if diff of angles has correct attributes.
    */
   const float32_t expected_angle_value = F360_DEG2RAD(30.0F);
   const float32_t expected_sin_value = 0.5F;
   const float32_t expected_cos_value = 0.86602540F;
   DOUBLES_EQUAL(expected_angle_value, ang_1.Value(), test_epsilon);
   DOUBLES_EQUAL(expected_sin_value, ang_1.Sin(), test_epsilon);
   DOUBLES_EQUAL(expected_cos_value, ang_1.Cos(), test_epsilon);
}
/** @}*/

/** \defgroup  Angle_Vs_Float_Operators_Tests
 *  @{
 */

 /** \brief
   * Group for testing Angle operators.
   */
TEST_GROUP(Angle_Vs_Float_Operators_Tests)
{
   const float32_t test_epsilon = 1.19e-7F;
};

/** \purpose
 * Test behavior of < operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Is_Lower_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F + test_epsilon;

   /** \action
    * Compare angles.
    */
   const bool is_lower = ang_1 < ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower);
}

/** \purpose
 * Test behavior of > operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Is_Greater_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F - test_epsilon;

   /** \action
    * Compare angles.
    */
   const bool is_greater = ang_1 > ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater);
}

/** \purpose
 * Test behavior of == operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Are_Equal_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F;

   /** \action
    * Compare angles.
    */
   const bool are_equal = ang_1 == ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(are_equal);
}

/** \purpose
 * Test behavior of != operator.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Are_Not_Equal_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F + test_epsilon;

   /** \action
    * Compare angles.
    */
   const bool are_not_equal = ang_1 != ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(are_not_equal);
}

/** \purpose
 * Test behavior of <= operator when left value is lower then right.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Is_Lower_Or_Equal_Operator_When_Is_Lower_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F + test_epsilon;

   /** \action
    * Compare angles.
    */
   const bool is_lower_or_equal = ang_1 <= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower_or_equal);
}

/** \purpose
 * Test behavior of <= operator when left value equal right.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Is_Lower_Or_Equal_Operator_When_Are_Equal_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F;

   /** \action
    * Compare angles.
    */
   const bool is_lower_or_equal = ang_1 <= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower_or_equal);
}

/** \purpose
 * Test behavior of >= operator when left value is greater.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Is_Greater_Or_Equal_Operator_When_Is_Greater_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F - test_epsilon;

   /** \action
    * Compare angles.
    */
   const bool is_greater_or_equal = ang_1 >= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater_or_equal);
}

/** \purpose
 * Test behavior of >= operator when values are equal.
 * \req
 * NA
 */
TEST(Angle_Vs_Float_Operators_Tests, Is_Greater_Or_Equal_Operator_When_Are_Equal_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   Angle ang_1(0.1F);
   const float32_t ang_2 = 0.1F;

   /** \action
    * Compare angles.
    */
   const bool is_greater_or_equal = ang_1 >= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater_or_equal);
}

/** \defgroup  Float_Vs_Angle_Operators_Tests
 *  @{
 */

 /** \brief
   * Group for testing Angle operators.
   */
TEST_GROUP(Float_Vs_Angle_Operators_Tests)
{
   const float32_t test_epsilon = 1.19e-7F;
};

/** \purpose
 * Test behavior of < operator.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Is_Lower_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F + test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_lower = ang_1 < ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower);
}

/** \purpose
 * Test behavior of > operator.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Is_Greater_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F - test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_greater = ang_1 > ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater);
}

/** \purpose
 * Test behavior of == operator.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Are_Equal_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool are_equal = ang_1 == ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(are_equal);
}

/** \purpose
 * Test behavior of != operator.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Are_Not_Equal_Operator_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F + test_epsilon;
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool are_not_equal = ang_1 != ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(are_not_equal);
}

/** \purpose
 * Test behavior of <= operator when left value is lower then right.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Is_Lower_Or_Equal_Operator_When_Is_Lower_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F + test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_lower_or_equal = ang_1 <= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower_or_equal);
}

/** \purpose
 * Test behavior of <= operator when left value equal right.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Is_Lower_Or_Equal_Operator_When_Are_Equal_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool is_lower_or_equal = ang_1 <= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_lower_or_equal);
}

/** \purpose
 * Test behavior of >= operator when left value is greater.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Is_Greater_Or_Equal_Operator_When_Is_Greater_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F - test_epsilon);

   /** \action
    * Compare angles.
    */
   const bool is_greater_or_equal = ang_1 >= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater_or_equal);
}

/** \purpose
 * Test behavior of >= operator when values are equal.
 * \req
 * NA
 */
TEST(Float_Vs_Angle_Operators_Tests, Is_Greater_Or_Equal_Operator_When_Are_Equal_Test)
{
   /** \precond
    * Create a float and an Angle instance with the same value.
    */
   const float32_t ang_1 = 0.1F;
   Angle ang_2(0.1F);

   /** \action
    * Compare angles.
    */
   const bool is_greater_or_equal = ang_1 >= ang_2;

   /** \result
    * Check if operator result is correct.
    */
   CHECK_TRUE(is_greater_or_equal);
}

/** @}*/


/** \defgroup  Angle_single_operators_Tests
 *  @{
 */

 /** \brief
   * Group for testing single Angle operators.
   */
TEST_GROUP(Angle_single_operators_Tests)
{
   const float32_t test_epsilon = 1.19e-7F;
};

/** \purpose
 * Test behavior of negation (operator-()) operator for positive angle.
 * \req
 * NA
 */
TEST(Angle_single_operators_Tests, negate_positive_angle)
{
   /** \precond
    * Create ANgle class instance with non-zero, positive value.
    */
   const Angle angle{ 0.1F };

   /** \action
    * Negate angle.
    */
   const Angle negated_angle = -angle;

   /** \result
    * Check if operator result is correct.
    */
   DOUBLES_EQUAL(-0.1F, negated_angle.Value(), test_epsilon);
   DOUBLES_EQUAL(-0.09983341664F, negated_angle.Sin(), test_epsilon);
   DOUBLES_EQUAL(0.99500416527F, negated_angle.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of negation (operator-()) operator for negative angle.
 * \req
 * NA
 */
TEST(Angle_single_operators_Tests, negate_negative_angle)
{
   /** \precond
    * Create ANgle class instance with non-zero, negative value.
    */
   const Angle angle{ -0.1F };

   /** \action
    * Negate angle.
    */
   const Angle negated_angle = -angle;

   /** \result
    * Check if operator result is correct.
    */
   DOUBLES_EQUAL(0.1F, negated_angle.Value(), test_epsilon);
   DOUBLES_EQUAL(0.09983341664F, negated_angle.Sin(), test_epsilon);
   DOUBLES_EQUAL(0.99500416527F, negated_angle.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of negation (operator-()) operator for zero angle.
 * \req
 * NA
 */
TEST(Angle_single_operators_Tests, negate_zero_angle)
{
   /** \precond
    * Create ANgle class instance with zero value.
    */
   const Angle angle{ 0.0F };

   /** \action
    * Negate angle.
    */
   const Angle negated_angle = -angle;

   /** \result
    * Check if operator result is correct.
    */
   DOUBLES_EQUAL(0.0F, negated_angle.Value(), test_epsilon);
   DOUBLES_EQUAL(0.0F, negated_angle.Sin(), test_epsilon);
   DOUBLES_EQUAL(1.0F, negated_angle.Cos(), test_epsilon);
}

/** \purpose
 * Test behavior of negation (operator-()) operator whether trygonometric functions are already evaluated.
 * \req
 * NA
 */
TEST(Angle_single_operators_Tests, negate_positive_angle__already_evaluated_tryg_functions)
{
   /** \precond
    * Create ANgle class instance with non-zero, positive value.
    * Call Sin() and Cos()
    */
   const Angle angle{ 0.1F };
   angle.Sin();
   angle.Cos();

   /** \action
    * Negate angle.
    */
   const Angle negated_angle = -angle;

   /** \result
    * Check if operator result is correct.
    */
   DOUBLES_EQUAL(-0.1F, negated_angle.Value(), test_epsilon);
   DOUBLES_EQUAL(-0.09983341664F, negated_angle.Sin(), test_epsilon);
   DOUBLES_EQUAL(0.99500416527F, negated_angle.Cos(), test_epsilon);
}
/** @}*/