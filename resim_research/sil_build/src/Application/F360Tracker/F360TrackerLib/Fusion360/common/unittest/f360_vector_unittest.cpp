/** \file
 * This file contains unit tests for content of f360_vector__constructors.cpp file
 */

#include "f360_vector.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_vector__constructors
 *  @{
 */

/** \brief
 * Test class constructors
 */
TEST_GROUP(f360_vector__constructors)
{	
   const float tolerance = 0.00001;
};

/** \purpose  
 * Check default constructor
 * \req
 * NA.
 */
TEST(f360_vector__constructors, default_constructor)
{
   /** \precond
    * NA
    */ 
	
   /** \action
    * Create vector
    */
   const Vector_T vector = {};

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */	
   DOUBLES_EQUAL(0.0F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(0.0F, vector.Get_Y(), tolerance);
}

/** \purpose
 * Check points diff constructor (case 1)
 * \req
 * NA.
 */
TEST(f360_vector__constructors, points_diff_constructor__case_1)
{
   /** \precond
    * Create two points
    */
   const Point point_start = { 2.1F, 4.5F };
   const Point point_end = { 1.2F, 6.3F };

   /** \action
    * Create vector
    */
   const Vector_T vector = { point_start, point_end};

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-0.9F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(1.8F, vector.Get_Y(), tolerance);
}


/** \purpose
 * Check points diff constructor (case 2)
 * \req
 * NA.
 */
TEST(f360_vector__constructors, points_diff_constructor__case_2)
{
   /** \precond
    * Create two points
    */
   const Point point_start = { 2.1F, 4.5F };
   const Point point_end = { 1.2F, 6.3F };

   /** \action
    * Create vector
    */
   const Vector_T vector = { point_end, point_start };

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.9F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(-1.8F, vector.Get_Y(), tolerance);
}

/** \purpose
 * Check 2D points diff constructor (case 1)
 * \req
 * NA.
 */
TEST(f360_vector__constructors, 2D_points_diff_constructor__case_1)
{
   /** \precond
    * Create two points
    */
   const Point point_start = { 2.1F, 4.5F };
   const Point point_end = { 1.2F, 6.3F };

   /** \action
    * Create vector
    */
   const Vector_T vector = { point_start, point_end };

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-0.9F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(1.8F, vector.Get_Y(), tolerance);
}


/** \purpose
 * Check 2D points diff constructor (case 2)
 * \req
 * NA.
 */
TEST(f360_vector__constructors, 2D_points_diff_constructor__case_2)
{
   /** \precond
    * Create two points
    */
   const Point point_start = { 2.1F, 4.5F };
   const Point point_end = { 1.2F, 6.3F };

   /** \action
    * Create vector
    */
   const Vector_T vector = { point_end, point_start };

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.9F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(-1.8F, vector.Get_Y(), tolerance);
}

/** \purpose
 * Check velocity constructor
 * \req
 * NA.
 */
TEST(f360_vector__constructors, raw_data_constructor)
{
   /** \precond
    * N/A
    */

   /** \action
    * Create vector
    */
   const Vector_T vector{ 1.234F, 0.0F };

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(1.234F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(0.0F, vector.Get_Y(), tolerance);
}

/** \purpose
 * Check raw data constructor
 * \req
 * NA.
 */
TEST(f360_vector__constructors, raw_data_constructor__case_2)
{
   /** \precond
    * N/A
    */

    /** \action
     * Create vector
     */
   const Vector_T vector{ 0.0F, -1.234F };

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(-1.234F, vector.Get_Y(), tolerance);
}

/** \purpose
 * Check raw data constructor
 * \req
 * NA.
 */
TEST(f360_vector__constructors, velocity_constructor)
{
   /** \precond
    * Create velocity
    */
   const F360_VCS_Velocity_T velocity = { 2.1F, 4.5F };

   /** \action
    * Create vector
    */
   const Vector_T vector = { velocity };

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(2.1F, vector.Get_X(), tolerance);
   DOUBLES_EQUAL(4.5F, vector.Get_Y(), tolerance);
}
/** @}*/

/** \defgroup  f360_vector__magnitude
 *  @{
 */

 /** \brief
  * Test class Magnitude() method.
  */
TEST_GROUP(f360_vector__magnitude)
{
   const float tolerance = 0.00001;
};

/** \purpose
 * Check if Magnitude() returns 0 for zero vector
 * \req
 * NA.
 */
TEST(f360_vector__magnitude, zero_vector)
{
   /** \precond
    * Create zero vector
    */
   const Vector_T vector = {};

   /** \action
   * Call Magnitude()
   */
   const float32_t result = vector.Magnitude();

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if Magnitude() returns correct value for non-zero vector
 * \req
 * NA.
 */
TEST(f360_vector__magnitude, non_zero_vector)
{
   /** \precond
    * Create non-zero vector
    */
   const Vector_T vector = { Point{0.0F, 0.0F}, Point{2.1F, 4.5F} };

   /** \action
   * Call Magnitude()
   */
   const float32_t result = vector.Magnitude();

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(4.9658836F, result, tolerance);
}
/** @}*/

/** \defgroup  f360_vector__Cos_Angle
 *  @{
 */

 /** \brief
  * Test class Cos_Angle() method.
  */
TEST_GROUP(f360_vector__Cos_Angle)
{
   const float tolerance = 0.00001;
};

/** \purpose
 * Check if Cos_Angle() returns correct value when both vectors are zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, zero_vectors)
{
   /** \precond
    * Create two zero vectors
    */
   const Vector_T vector_1 = {};
   const Vector_T vector_2 = {};

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result = vector_1.Cos_Angle_Between(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE(std::isnan(result));
}

/** \purpose
 * Check if Cos_Angle() returns correct value when one of vector is zero vector (left case)
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, one_zero_and_one_nonzero_vectors__left_case)
{
   /** \precond
    * Create one zero vector and one non-zero
    */
   const Vector_T vector_1 = { Point{0.0F, 0.0F}, Point{2.1F, 4.5F} };
   const Vector_T vector_2 = {};

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result = vector_1.Cos_Angle_Between(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE(std::isnan(result));
}

/** \purpose
 * Check if Cos_Angle() returns correct value when one of vector is zero vector (right case)
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, one_zero_and_one_nonzero_vectors__right_case)
{
   /** \precond
    * Create one zero vector and one non-zero
    */
   const Vector_T vector_1 = {};
   const Vector_T vector_2 = { Point{0.0F, 0.0F}, Point{2.1F, 4.5F} };

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result = vector_1.Cos_Angle_Between(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE(std::isnan(result));
}

/** \purpose
 * Check if Cos_Angle() returns correct value for non-zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, nonzero_vectors)
{
   /** \precond
    * Create two non-zero vectors
    */
   const Vector_T vector_1 = { Point{4.4F, 1.5F}, Point{1.0F, 0.0F} };
   const Vector_T vector_2 = { Point{2.0F, 2.0F}, Point{2.1F, 4.5F} };

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result = vector_1.Cos_Angle_Between(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-0.439885F, result, tolerance);
}

/** \purpose
 * Check if Cos_Angle() returns correct value for non-zero vectors (both sides)
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, nonzero_vectors__reversion)
{
   /** \precond
    * Create two non-zero vectors
    */
   const Vector_T vector_1 = { Point{4.4F, 1.5F}, Point{1.0F, 0.0F} };
   const Vector_T vector_2 = { Point{2.0F, 2.0F}, Point{2.1F, 4.5F} };

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result_1 = vector_1.Cos_Angle_Between(vector_2);
   const float32_t result_2 = vector_2.Cos_Angle_Between(vector_1);
   const float32_t diff = result_1 - result_2;

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, diff, tolerance);
}

/** \purpose
 * Check if Cos_Angle() returns correct value for non-zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, nonzero_vectors__180_deg)
{
   /** \precond
    * Create two non-zero vectors
    */
   const Vector_T vector_1 = { Point{2.0F, 4.0F}, Point{1.0F, 4.0F} };
   const Vector_T vector_2 = { Point{1.0F, 3.0F}, Point{3.0F, 3.0F} };

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result = vector_1.Cos_Angle_Between(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-1.0F, result, tolerance);
}

/** \purpose
 * Check if Cos_Angle() returns correct value for non-zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__Cos_Angle, nonzero_vectors__0_deg)
{
   /** \precond
    * Create two non-zero vectors
    */
   const Vector_T vector_1 = { Point{2.0F, 4.0F}, Point{1.0F, 4.0F} };
   const Vector_T vector_2 = { Point{3.0F, 3.0F}, Point{1.0F, 3.0F} };

   /** \action
    * Call Cos_Angle()
    */
   const float32_t result = vector_1.Cos_Angle_Between(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(1.0F, result, tolerance);
}
/** @}*/


/** \defgroup  f360_vector__dot_operator
 *  @{
 */

 /** \brief
  * Test class operator*() method.
  */
TEST_GROUP(f360_vector__dot_operator)
{
   const float tolerance = 0.00001;
};

/** \purpose
 * Check if operator*() returns correct value when both vectors are zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__dot_operator, zero_vectors)
{
   /** \precond
    * Create two zero vectors
    */
   const Vector_T vector_1 = {};
   const Vector_T vector_2 = {};

    /** \action
     * Multiply vectors
     */
   const float32_t result = vector_1 * vector_2;

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if operator*() returns correct value when one of vector is zero vector (left case)
 * \req
 * NA.
 */
TEST(f360_vector__dot_operator, one_zero_and_one_nonzero_vectors__left_case)
{
   /** \precond
    * Create one zero vector and one non-zero
    */
   const Vector_T vector_1 = { Point{0.0F, 0.0F}, Point{2.1F, 4.5F} };
   const Vector_T vector_2 = {};

   /** \action
    * Multiply vectors
    */
   const float32_t result = vector_1 * vector_2;

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if operator*() returns correct value when one of vector is zero vector (right case)
 * \req
 * NA.
 */
TEST(f360_vector__dot_operator, one_zero_and_one_nonzero_vectors__right_case)
{
   /** \precond
    * Create one zero vector and one non-zero
    */
   const Vector_T vector_1 = {};
   const Vector_T vector_2 = { Point{0.0F, 0.0F}, Point{2.1F, 4.5F} };

   /** \action
    * Multiply vectors
    */
   const float32_t result = vector_1 * vector_2;

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if operator*() returns correct value for non-zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__dot_operator, nonzero_vectors)
{
   /** \precond
    * Create two non-zero vectors
    */
   const Vector_T vector_1 = { Point{4.4F, 1.5F}, Point{1.0F, 0.0F} };
   const Vector_T vector_2 = { Point{0.0F, 0.0F}, Point{2.1F, 4.5F} };

   /** \action
    * Multiply vectors
    */
   const float32_t result = vector_1 * vector_2;

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-13.89F, result, tolerance);
}

/** \purpose
 * Check if operator*() returns same values for case a*b = b*a
 * \req
 * NA.
 */
TEST(f360_vector__dot_operator, same_value_for_both_orders)
{
   /** \precond
    * Create two non-zero vectors
    */
   const Vector_T vector_1 = { Point{4.4F, 1.5F}, Point{1.0F, 20.0F} };
   const Vector_T vector_2 = { Point{-3.0F, 0.23333F}, Point{2.1F, 4.5F} };

   /** \action
    * Multiply vectors
    */
   const float32_t result_1 = vector_1 * vector_2;
   const float32_t result_2 = vector_2 * vector_1;
   const float32_t diff = result_1 - result_2;

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, diff, tolerance);
}
/** @}*/

/** \defgroup  f360_vector__Calc_Sign_Magnitude_Projected_On
 *  @{
 */

 /** \brief
  * Test class Calc_Sign_Magnitude_Projected_On() method.
  */
TEST_GROUP(f360_vector__Calc_Sign_Magnitude_Projected_On)
{
   const float tolerance = 0.00001;
};

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value when both vectors are zero vectors
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, zero_vectors)
{
   /** \precond
    * Create two zero vectors
    */
   const Vector_T vector_1 = {};
   const Vector_T vector_2 = {};

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for zero and nonzero vectors (case 1)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, zero_and_nonzero__case_1)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = {};
   const Vector_T vector_2 = { Point{4.0F, 10.0F} };

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for zero and nonzero vectors (case 2)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, zero_and_nonzero__case_2)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = {  Point{4.0F, 10.0F} };
   const Vector_T vector_2 = {};

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE(std::isnan(result));
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for nonzero vectors (positive magnitude)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, nonzero_vectors__positive_magnitude)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = { Point{4.0F, 8.0F} };
   const Vector_T vector_2 = { Point{12.0F, 10.0F} };

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(8.19436F, result, tolerance);
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for nonzero vectors (negative magnitude)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, nonzero_vectors__negative_magnitude)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = {  Point{-4.0F, -8.0F} };
   const Vector_T vector_2 = {  Point{12.0F, 10.0F} };

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-8.19436F, result, tolerance);
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for nonzero vectors (90 angle between)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, nonzero_vectors__90_angle)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = {  Point{-8.0F, 4.0F} };
   const Vector_T vector_2 = { Point{4.0F, 8.0F} };

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(0.0F, result, tolerance);
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for nonzero vectors (0 angle between)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, nonzero_vectors__zero_angle)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = { Point{4.0F, 8.0F} };
   const Vector_T vector_2 = { Point{4.0F, 8.0F} };

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(8.9442719F, result, tolerance);
   DOUBLES_EQUAL_TEXT(vector_1.Magnitude(), result, tolerance, "Projected range signed magnitude should be same as vector_1 magnitude in this test");
}

/** \purpose
 * Check if Calc_Sign_Magnitude_Projected_On() returns correct value for nonzero vectors (180 angle between)
 * \req
 * NA.
 */
TEST(f360_vector__Calc_Sign_Magnitude_Projected_On, nonzero_vectors__180_angle)
{
   /** \precond
    * Create zero and non-zero vector
    */
   const Vector_T vector_1 = { Point{-4.0F, -8.0F} };
   const Vector_T vector_2 = { Point{4.0F, 8.0F} };

   /** \action
    * Call Calc_Sign_Magnitude_Projected_On()
    */
   const float32_t result = vector_1.Calc_Signed_Magnitude_Projected_On(vector_2);

   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(-8.9442719F, result, tolerance);
   DOUBLES_EQUAL_TEXT(-vector_1.Magnitude(), result, tolerance, "Projected range signed magnitude should be same as negative vector_1 magnitude in this test");
}

/** @}*/
