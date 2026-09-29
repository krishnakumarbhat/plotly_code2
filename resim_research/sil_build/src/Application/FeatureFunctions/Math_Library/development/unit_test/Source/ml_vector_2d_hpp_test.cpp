/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>

#include "Basic_Vectors.hpp"
#include <math.h>
#include "st_vector_2d_helper.hpp"

#include "ml_vector_2d.hpp"
#include "ml_angle.hpp"
#include "ml_vector_2d.h"
#include "ml_math.h"

#define TOOLBOX_PRECISION    (1e-6)


/** Test that vector addition works */
TEST(StVector2dHppTest, operator_plus)
{
   /** \arrange create two vectors containing arbitrary values */
   Vector_2d_T a = { 0.0f, 0.0f };
   Vector_2d_T b = { 1.0f, 2.0f };

   /**\action perform addition */
   auto c = a + b;

   /** \assert result matches component wise addition */
   EXPECT_EQ(c.x, a.x + b.x);
   EXPECT_EQ(c.y, a.y + b.y);
}


/** Test that vector addition works within namespace 'st'*/
TEST(StVector2dHppTest, operator_plus_namespace_st)
{
   /** \arrange create two vectors within namespace st containing arbitrary values */
   ml::Vector_2d_T a = { 0.0f, 0.0f };
   ml::Vector_2d_T b = { 1.0f, 2.0f };

   /**\action perform addition */
   auto c = a + b;

   /** \assert result matches component wise addition */
   EXPECT_EQ(c.x, a.x + b.x);
   EXPECT_EQ(c.y, a.y + b.y);
}

/**
 * Subtract two vectors
 */
TEST(StVector2dHppTest, operator_minus)
{
   /** \arrange Create two arbitrary vectors */
   Vector_2d_T a = { 0.0f, 0.0f };
   Vector_2d_T b = { 1.0f, 2.0f };

   /** call function under test */
   Vector_2d_T c = a - b;

   /** \assert result matches component wise subtraction */
   EXPECT_EQ(c.x, a.x - b.x);
   EXPECT_EQ(c.y, a.y - b.y);
}

/**
 * Scale a vector
 */
TEST(StVector2dHppTest, operator_multiply_scalar)
{
   /** \arrange Create one arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   /** \arrange Pick an arbitrary float */
   float b = 2.0f;

   /** call function under test */
   Vector_2d_T c = a * b;

   /** \assert result matches component wise scaling */
   EXPECT_EQ(c.x, a.x * b);
   EXPECT_EQ(c.y, a.y * b);
}

/**
* Scale a vector
*/
TEST(StVector2dHppTest, operator_multiply_scalar_reverse)
{
   /** \arrange Create one arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   /** \arrange Pick an arbitrary float */
   float b = 2.0f;

   /** call function under test */
   Vector_2d_T c = b * a;

   /** \assert result matches component wise scaling */
   EXPECT_EQ(c.x, b * a.x);
   EXPECT_EQ(c.y, b * a.y);
}

/**
* Scale a vector while assigning
*/
TEST(StVector2dHppTest, operator_multiply_scalar_assign)
{
   /** \arrange Create one arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   Vector_2d_T res = a;

   /** \arrange Pick an arbitrary float */
   float b = 2.0f;

   /** call function under test */
   res *= b;

   /** \assert result matches component wise scaling */
   EXPECT_EQ(res.x, b * a.x);
   EXPECT_EQ(res.y, b * a.y);
}

/**
 * Create a vector from an angle
 */
TEST(StVector2dHppTest, create_from_angle)
{
   /** \arrange Create arbitrary angle */
   Angle_T angle = ml::angle::Angle(1.12f);

   /** \action call FUT */
   Vector_2d_T a = ml::vector2d::Vector(angle);

   /** \assert result matches unit vector in the direction of the angle */
   EXPECT_EQ(a.x, angle.cos);
   EXPECT_EQ(a.y, angle.sin);
}

/**
 * Scalar product of two vectors
 */
TEST(StVector2dHppTest, operator_scalar_product)
{
   /** \arrange pick two arbitrary vectors */
   Vector_2d_T a = { 1.0f, 2.0f };
   Vector_2d_T b = { 1.0f, 2.0f };

   /** call function under test */
   float c = b * a;

   /** \assert The result matches scalar product */
   EXPECT_EQ(c, 5.0f);
}

/**
 * Unary minus for a vector
 */
TEST(StVector2dHppTest, unary_minus)
{
   /** \arrange pick an arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   /** call function under test */
   Vector_2d_T b = -a;

   /** \assert result matches component wise scaling with -1 */
   EXPECT_EQ(b.x, -a.x);
   EXPECT_EQ(b.y, -a.y);
}

/**
* Unary minus for a vector
*/
TEST(StVector2dHppTest, unary_plus)
{
   /** \arrange pick an arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   /** call function under test */
   Vector_2d_T b = +a;

   /** \assert result matches original vector */
   EXPECT_VECTOR_2D_EQ(b, a);
}

/**
 * The at function returns the x component
 */
TEST(StVector2dHppTest, at_returns_x)
{
   /** \arrange pick an arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   /** \action at function returns the x component */
   EXPECT_EQ(a.x, at(a, 0));
}

/**
* The at function returns the y component
*/
TEST(StVector2dHppTest, at_returns_y)
{
   /** \arrange pick an arbitrary vector */
   Vector_2d_T a = { 1.0f, 2.0f };

   /** \action at function returns the y component */
   EXPECT_EQ(a.y, at(a, 1));
}

/**
* Test the function to multiply a 2d vector with a scalar:
* Multiply a vector with 0.0 should result in origin vector
* \sdd{WI-13917}
*/
TEST(StVector2dHppTest, Multiply_X_Normal_With_Null_Should_Result_In_The_Origin)
{
   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_x_normal, 0.0f);

   const Vector_2d_T test_result = Basic_Vectors::vec_x_normal * 0.0f;

   /** \assert */
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}

/**
* Test the function to multiply a 2d vector with a scalar:
* Multiply a vector with 0.0 should result in origin vector.
Testing the assigning variant.
* \sdd{WI-13917}
*/
TEST(StVector2dHppTest, Multiply_X_Normal_With_Null_Should_Result_In_The_Origin_assigning)
{
   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_x_normal, 0.0f);

   Vector_2d_T test_result = Basic_Vectors::vec_x_normal;
   test_result *= 0.0f;

   /** \assert */
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}


/**
* Test the function to multiply a 2d vector with a scalar:
* Multiplication of x normal vector with -1.0 should result in a unit vector pointing towards the negative x axis.
* \sdd{WI-13917}
*/
TEST(StVector2dHppTest, Multiply_X_Normal_With_Minus_One_Should_Be_The_Same_as_Rotating_By_180_Deg)
{
   /** \arrange Create a normal vector in x direction */
   Vector_2d_T vec_x_normal = Create_2d_Vector_X_Normal();

   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Multiply_Scalar(&vec_x_normal, -1.0f);

   /** \assert Result matches unit vector in direction of negative x axis*/
   Vector_2d_T test_result = vec_x_normal * -1.0f;

   EXPECT_VECTOR_2D_EQ(test_result, reference);
}


/**
* Test the function to add a 2d vector
* Adding x normal to y normal should be first bisectrix.
* \sdd{WI-13919}
*/
TEST(StVector2dHppTest, Add_X_Normal_To_Y_Normal_Should_Be_first_Bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Add(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   const Vector_2d_T test_result = Basic_Vectors::vec_x_normal + Basic_Vectors::vec_y_normal;

   /** \assert */
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}

/**
* Test the function to add a 2d vector
* Adding x normal to y normal should be first bisectrix.
* testing the assigning variant.
* \sdd{WI-13919}
*/
TEST(StVector2dHppTest, Add_X_Normal_To_Y_Normal_Should_Be_first_Bisectrix_assigning)
{
   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Add(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   Vector_2d_T test_result = Basic_Vectors::vec_x_normal;
   test_result  += Basic_Vectors::vec_y_normal;

   /** \assert */
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}

/**
* Test the function to add a 2d vector
* The origin should be the neutral element
* \sdd{WI-13919}
*/
TEST(StVector2dHppTest, Add__The_Origin_Should_Be_The_Neutral_Element)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Add(&Basic_Vectors::vec_arbitrary1, &origin);

   const Vector_2d_T test_result = Basic_Vectors::vec_arbitrary1 + origin;

   /** \assert */
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}

/**
* Test the function to add a 2d vector
* Vector addition should be commutative.
* \sdd{WI-13919}
*/
TEST(StVector2dHppTest, Vector_Addition_should_be_commutative)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Basic_Vectors::vec_arbitrary1 + Basic_Vectors::vec_arbitrary2;

   /** \action call function under test with swapped arguments*/
   const Vector_2d_T test_result2 = Basic_Vectors::vec_arbitrary2 + Basic_Vectors::vec_arbitrary1;

   /** \assert Expect the results to be identical */
   EXPECT_VECTOR_2D_NEAR(test_result, test_result2, TOOLBOX_PRECISION);
}



/**
* Test the function to process middle values of a 2d vector element
* Middle point of x and y normal should be one half of first bisectrix.
* \sdd{WI-13923}
*/
TEST(StVector2dHppTest, middle_point_Of_X_And_Y_Normal_should_be_one_half_of_first_bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Middle(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   const Vector_2d_T test_result = Middle(Basic_Vectors::vec_x_normal, Basic_Vectors::vec_y_normal);

   /** \assert Result matches first bisectrix*/
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}

/**
* Test the function to process middle values of a 2d vector element
* Middle point of first and third bisectrix should be the origin
* \sdd{WI-13923}
*/
TEST(StVector2dHppTest, middle_point_of_first_and_third_bisectrix_should_be_the_Origin)
{
   /** \action call function under test */
   const Vector_2d_T reference = Vector_2d_Alg_Middle(&Basic_Vectors::vec_first_bisectrix, &Basic_Vectors::vec_third_bisectrix);

   const Vector_2d_T test_result = Middle(Basic_Vectors::vec_first_bisectrix, Basic_Vectors::vec_third_bisectrix);

   /** \assert Result matches first bisectrix*/
   EXPECT_VECTOR_2D_EQ(test_result, reference);
}


/**
* Test the function to process a normal of a 2d vector:
* Norm of the origin should be zero
* \sdd{WI-13937}
*/
TEST(StVector2dHppTest, Norm_of_the_origin_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Norm(ml::vector2d::Vector());

   /** \assert */
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process a normal of a 2d vector:
* Norm of the x normal should be one
* \sdd{WI-13937}
*/
TEST(StVector2dHppTest, Norm_of_the_x_normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Norm(ml::vector2d::X());

   /** \assert Norm is one */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
* Test the function to process a normal of a 2d vector:
* Norm of the y normal should be one
* \sdd{WI-13937}
*/
TEST(StVector2dHppTest, Norm_of_the_y_normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Norm(Basic_Vectors::vec_y_normal);

   /** \assert Norm is one */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
* Test the function to process a normal of a 2d vector:
* Norm of the 3d bisectrix normal should be square root of 2
* \sdd{WI-13937}
*/
TEST(StVector2dHppTest, Norm_of_the_3th_bisectrix_should_be_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Norm(Basic_Vectors::vec_third_bisectrix);

   /** \assert Norm is square root of 2 */
   EXPECT_FLOAT_EQ(test_result, sqrt(2.0f));
}

/**
* Test the function to process the scalar product of two 2d vectors:
* Scalar product of any arbitrary vector with the origin should be zero
* \sdd{WI-13924}
*/
TEST(StVector2dHppTest, Scalar_product_of_any_arbitrary_vector_with_the_origin_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Basic_Vectors::vec_arbitrary1 * ml::vector2d::Vector();

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process the scalar product of two 2d vectors:
* Scalar product of any arbitrary vector with the origin should be zero
* \sdd{WI-13924}
*/
TEST(StVector2dHppTest, Scalar_product_of_any_arbitrary2_with_the_origin_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Basic_Vectors::vec_arbitrary2 * ml::vector2d::Vector();

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process the scalar product of two 2d vectors:
* Scalar product of x normal and y normal should be zero
* \sdd{WI-13924}
*/
TEST(StVector2dHppTest, Scalar_product_of_X_and_Y_Normal_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = ml::vector2d::X() * ml::vector2d::Y();

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process the scalar product of two 2d vectors:
* Scalar product of x normal and any arbitrary vector should be x component of arbitrary vector
* \sdd{WI-13924}
*/
TEST(StVector2dHppTest, Scalar_product_of_X_Normal_with_vec_arbitrary1_should_be_the_x_component)
{
   /** \action call function under test */
   const float32_T test_result = ml::vector2d::X() * Basic_Vectors::vec_arbitrary1;

   /** \assert Scalar product is x component */
   EXPECT_FLOAT_EQ(test_result, Basic_Vectors::vec_arbitrary1.x);
}

/**
* Test the function to process the scalar product of two 2d vectors:
* Scalar product of x normal and any arbitrary vector should be x component of arbitrary vector
* \sdd{WI-13924}
*/
TEST(StVector2dHppTest, Scalar_product_of_X_Normal_with_vec_arbitrary1_should_be_the_y_component)
{
   /** \action call function under test */
   const float32_T test_result = ml::vector2d::Y() * Basic_Vectors::vec_arbitrary1;

   /** \assert Scalar product is x component */
   EXPECT_FLOAT_EQ(test_result, Basic_Vectors::vec_arbitrary1.y);
}

/**
* Test the function to process the scalar product of two 2d vectors:
* Scalar product should be commutative
* \sdd{WI-13924}
*/
TEST(StVector2dHppTest, Scalar_product_should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = Basic_Vectors::vec_arbitrary2 * Basic_Vectors::vec_arbitrary1;

   /** \action call function under test with swapped arguments */
   const float32_T test_result2 = Basic_Vectors::vec_arbitrary1 * Basic_Vectors::vec_arbitrary2;

   /** \assert Results match*/
   EXPECT_FLOAT_EQ(test_result, test_result2);
}


/**
* Test the function to process the distance between two 2d vectors:
* Distance between the origin and any vector should be the norm
* \sdd{WI-13921}
*/
TEST(StVector2dHppTest, Distance_between_the_origin_and_any_vector_should_be_the_norm)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const float32_T test_result = Distance(origin, Basic_Vectors::vec_arbitrary1);

   /** \assert Distance matches the norm*/
   EXPECT_FLOAT_EQ(test_result, Norm(Basic_Vectors::vec_arbitrary1));
}

/**
* Test the function to process the distance between two 2d vectors:
* Distance between the x and the y normal should be square root of 2
* \sdd{WI-13921}
*/
TEST(StVector2dHppTest, Distance_between_the_X_and_the_Y_normal_should_be_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Distance(Basic_Vectors::vec_x_normal, Basic_Vectors::vec_y_normal);

   /** \assert Distance matches square root of 2 */
   EXPECT_FLOAT_EQ(test_result, sqrt(2.0f));
}

/**
* Test the function to process the distance between two 2d vectors:
* The distance computation should be commutative
* \sdd{WI-13921}
*/
TEST(StVector2dHppTest, The_distance_computation_should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = Distance(Basic_Vectors::vec_arbitrary1, Basic_Vectors::vec_arbitrary2);

   /** \action call function under test with swapped arguments */
   const float32_T test_result2 = Distance(Basic_Vectors::vec_arbitrary2, Basic_Vectors::vec_arbitrary1);

   /** \assert Results match */
   EXPECT_FLOAT_EQ(test_result, test_result2);
}

/**
* Test the function to rotate a 2d vector on 90 degree:
* Rotate x normal should be y normal
* \sdd{WI-13936}
*/
TEST(StVector2dHppTest, Perpendicular_Positive_of_X_normal_should_be_Y_normal)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Perpendicular_Positive(Basic_Vectors::vec_x_normal);

   /** \assert Result matches y normal*/
   EXPECT_VECTOR_2D_EQ(test_result, Basic_Vectors::vec_neg_y_normal);
}

/**
* Test the function to rotate a 2d vector on 90 degree:
* Rotate x normal should be y normal
* \sdd{WI-13936}
*/
TEST(StVector2dHppTest, Perpendicular_Negative_of_Y_normal_should_be_Y_normal)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Perpendicular_Negative(Basic_Vectors::vec_y_normal);

   /** \assert Result matches y normal*/
   EXPECT_VECTOR_2D_EQ(test_result, Basic_Vectors::vec_neg_x_normal);
}

/**
* Test the function to rotate a 2d vector on 90 degree:
* Multiply a vector with its rotated counter part should be zero
* \sdd{WI-13936}
*/
TEST(StVector2dHppTest, Multiply_a_vector_with_its_rotated_counter_part_should_be_zero)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Perpendicular_Positive(Basic_Vectors::vec_x_normal);

   /** \assert Result of multiplication matches zero */
   EXPECT_FLOAT_EQ(Basic_Vectors::vec_x_normal * test_result, 0.0f);
}


/**
* Test the function to normalize a vector (scale a vector length to 1):
* Norm of a normalized vector should be one
* \sdd{WI-13914}
*/
TEST(StVector2dHppTest, Norm_of_a_normalized_vector_should_be_one)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Normalize(Basic_Vectors::vec_arbitrary1);

   /** \assert Result equals 1*/
   EXPECT_FLOAT_EQ(Norm(test_result), 1.0f);
}


/**
* Test the function to process the square magnitude of a 2d vector:
* Norm squared of x normal should be one
* \sdd{WI-13935}
*/
TEST(StVector2dHppTest, Norm_squared_of_X_Normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Norm_Squared(Basic_Vectors::vec_x_normal);

   /** \assert Result equals 1 */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
* Test the function to process the square magnitude of a 2d vector:
* Norm squared of a vector should be the norm squared
* \sdd{WI-13935}
*/
TEST(StVector2dHppTest, Norm_squared_of_a_vector_should_be_the_norm_squared)
{
   /** \action call function under test */
   const float32_T test_result = Norm_Squared(Basic_Vectors::vec_arbitrary1);

   /** \assert Expect result to match the squared norm */
   float32_T norm_squared = powf(Norm(Basic_Vectors::vec_arbitrary1), 2.0f);

   EXPECT_FLOAT_EQ(test_result, norm_squared);
}

/**
* Test the function to compute the subtraction of two 2d vectors:
* Diff by the origin should be the same
* \sdd{WI-13915}
*/
TEST(StVector2dHppTest, Diff_by_the_origin_should_be_the_same)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Basic_Vectors::vec_arbitrary1 - ml::vector2d::Vector();

   /** \assert Result matches parameter */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_arbitrary1, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the subtraction of two 2d vectors:
* Diff y normal from x normal should be second bisectrix
* \sdd{WI-13915}
*/
TEST(StVector2dHppTest, Diff_Y_Normal_from_X_normal_should_be_second_bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T test_result = ml::vector2d::Y() - ml::vector2d::X();

   /** \assert Result is second bisectrix */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_second_bisectrix, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the subtraction of two 2d vectors:
* Diff y normal from x normal should be second bisectrix
* testing the assigning variant.
* \sdd{WI-13915}
*/
TEST(StVector2dHppTest, Diff_Y_Normal_from_X_normal_should_be_second_bisectrix_assigning)
{
   /** \action call function under test */
   Vector_2d_T test_result = ml::vector2d::Y();
   test_result  -= ml::vector2d::X();

   /** \assert Result is second bisectrix */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_second_bisectrix, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the subtraction of two 2d vectors:
* Diff should be commutative by a factor minus one
* \sdd{WI-13915}
*/
TEST(StVector2dHppTest, Diff_should_be_commutative_by_a_factor_minus_one)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Basic_Vectors::vec_arbitrary1 - Basic_Vectors::vec_arbitrary2;

   /** \action call function under test with swapped arguments */
   const Vector_2d_T test_result2 = Basic_Vectors::vec_arbitrary2 - Basic_Vectors::vec_arbitrary1;

   /** \assert result matches by a factor minus one */
   EXPECT_VECTOR_2D_NEAR(test_result, Vector_2d_Alg_Multiply_Scalar(&test_result2, -1.0f), TOOLBOX_PRECISION);
}


/**
* Test the function to process the absolute value of each 2d vector element
* Component norm of 3th bisectrix should be first bisectrix
* \sdd{WI-13938}
*/
TEST(StVector2dHppTest, Component_norm_of_3th_bisectrix_should_be_first_bisectrix)
{
   /** \action call function under test */
   Vector_2d_T result = Absolute(Basic_Vectors::vec_third_bisectrix);

   /** \assert Result is first bisectrix */
   EXPECT_VECTOR_2D_NEAR(Basic_Vectors::vec_first_bisectrix, result, TOOLBOX_PRECISION);
}


/**
* Test the function to compute the square root of each 2d vector elements
* first bisectrix should not change
* \sdd{WI-13929}
*/
TEST(StVector2dHppTest, Absolute_of_first_bisectrix_should_not_change)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Absolute(Basic_Vectors::vec_first_bisectrix);

   /** \assert Result matches parameter */
   EXPECT_VECTOR_2D_NEAR(Basic_Vectors::vec_first_bisectrix, test_result, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the cosine of the angle between two 2d vectors
* If a vector is too short VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned, an assertion fires in debug.
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
TEST(StVector2dHppTest, Cos__Vector_a_too_short)
{
   /** \arrange Create a short vector */
   Vector_2d_T vector_a = Basic_Vectors::vec_x_normal * THRESHOLD_IS_ZERO;
   float32_T   test_result;

   /** \action call function under test */
   EXPECT_DEBUG_DEATH(
      test_result = Cos(vector_a, Basic_Vectors::vec_y_normal),
      "false");
#ifdef NDEBUG
   /** \assert VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned */
   EXPECT_EQ(test_result, VECTOR_2D_ALGEBRA_NOT_A_COSINE);
#endif
}

/**
* Test the function to compute the cosine of the angle between two 2d vectors
* If a vector is too short VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned, an assertion fires in debug.
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
TEST(StVector2dHppTest, Cos_Between_Two_Vec__Vector_b_too_short)
{
   /** \arrange Create a short vector */
   Vector_2d_T vector_b = Basic_Vectors::vec_y_normal * THRESHOLD_IS_ZERO;
   float32_T   test_result;

   /** \action call function under test */
   EXPECT_DEBUG_DEATH(
      test_result = Cos(Basic_Vectors::vec_x_normal, vector_b),
      "false");
#ifdef NDEBUG
   /** \assert VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned */
   EXPECT_EQ(test_result, VECTOR_2D_ALGEBRA_NOT_A_COSINE);
#endif
}


/**
* Test the function to compute the cosine of the angle between two 2d vectors
* Cosine between x and y normal should be zero
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
TEST(StVector2dHppTest, Cos_between_X_and_Y_normal_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Cos(Basic_Vectors::vec_x_normal, Basic_Vectors::vec_y_normal);

   /** \assert result is zero */
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to compute the cosine of the angle between two 2d vectors
* Cosine between x and negative x normal should be minus one
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
TEST(StVector2dHppTest, Cos_Between_Two_Vec__Cos_between_X_and_neg_X_normal_should_be_minus_one)
{
   /** \action call function under test */
   const float32_T test_result = Cos(Basic_Vectors::vec_x_normal, Basic_Vectors::vec_neg_x_normal);

   /** \assert Matches minus one */
   EXPECT_FLOAT_EQ(test_result, -1.0f);
}

/**
* Test the function to compute the cosine of the angle between two 2d vectors
* Cosine between x and first bisectrix normal should be one half af the square root of two
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
TEST(StVector2dHppTest, Cos_between_X_and_first_bisectrix_should_be_one_half_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Cos(Basic_Vectors::vec_x_normal, Basic_Vectors::vec_first_bisectrix);

   /** \assert Result matches one half af the square root of two*/
   EXPECT_FLOAT_EQ(test_result, 0.5f * sqrtf(2));
}

/**
* Test the function to compute the cosine of the angle between two 2d vectors
* Should be commutative
* \sdd{WI-13931}
* \sdd{WI-13943}
*/
TEST(StVector2dHppTest, Cos_Between_Two_Vec__Should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = Cos(Basic_Vectors::vec_arbitrary1, Basic_Vectors::vec_arbitrary2);

   /** \action call function under test with swapped parameters */
   const float32_T test_result2 = Cos(Basic_Vectors::vec_arbitrary2, Basic_Vectors::vec_arbitrary1);

   /** \assert Results match */
   EXPECT_FLOAT_EQ(test_result, test_result2);
}

/**
* Test the function to limit the values of an 2d vector to a certain range
* Limiting negative x normal to first quadrant should be the origin
* \sdd{WI-13927}
*/
TEST(StVector2dHppTest, Clip_neg_X_normal_to_first_quadrant_should_be_the_origin)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   Vector_2d_T result = Clip(origin, Basic_Vectors::vec_first_bisectrix, Basic_Vectors::vec_neg_x_normal);

   /** \assert Result matches origin*/
   EXPECT_VECTOR_2D_NEAR(result, origin, TOOLBOX_PRECISION);
}

/**
* Test the function to limit the values of an 2d vector to a certain range
* Limiting negative y normal to first quadrant should be the origin
* \sdd{WI-13927}
*/
TEST(StVector2dHppTest, Clip_neg_Y_normal_to_first_quadrant_should_be_the_origin)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   Vector_2d_T result = Clip(origin, Basic_Vectors::vec_first_bisectrix, Basic_Vectors::vec_neg_y_normal);

   /** \assert Result matches origin*/
   EXPECT_VECTOR_2D_NEAR(result, origin, TOOLBOX_PRECISION);
}

/**
* Test the function to create a 2d vector
* \sdd{WI-13925}
*/
TEST(StVector2dHppTest, Create_2d_Vector_Coordinates_test)
{
   /** \arrange */
   float32_T x_component = 0.78f;
   float32_T y_component = 37.8f;

   /** \action call function under test */
   const Vector_2d_T test_result = ml::vector2d::Vector(x_component, y_component);

   /** \assert a vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, x_component);
   EXPECT_FLOAT_EQ(test_result.y, y_component);
}

/**
* Test the function to create a zero 2d vector
* \sdd{WI-13934}
*/
TEST(StVector2dHppTest, Create_2d_Vector_Origin_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = ml::vector2d::Vector();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
}

/**
* Test the function to create a normal 2d vector to y axis
* \sdd{WI-13933}
*/
TEST(StVector2dHppTest, Create_2d_Vector_X_Normal_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = ml::vector2d::X();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 1.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
}

/**
* Test the function to create a normal 2d vector to y axis
* \sdd{WI-13933}
*/
TEST(StVector2dHppTest, Create_2d_Vector_X_Normal_scaled_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = ml::vector2d::X(5.0f);

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 5.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
}

/**
* Test the function to create a normal 2d vector to x axis
* \sdd{WI-13930}
*/
TEST(StVector2dHppTest, Create_2d_Vector_Y_Normal_scaled_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = ml::vector2d::Y(5.0f);

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 5.0f);
}
