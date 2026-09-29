/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>

#include "Basic_Vectors.hpp"
#include <math.h>
#include "st_vector_2d_helper.hpp"
#include "ml_bool.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"

#define TOOLBOX_PRECISION    (1e-6)

/**
 * Test the function to multiply a 2d vector with a scalar:
 * Multiply a vector with 0.0 should result in origin vector
 * \sdd{WI-13917}
 */
TEST(StVector2dTest, WI_15310_Vector_2d_Alg_Multiply_Scalar__Multiply_X_Normal_With_Null_Should_Result_In_The_Origin)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_x_normal, 0.0f);

   /** \assert */
   EXPECT_VECTOR_2D_NEAR(test_result, Create_2d_Vector_Origin(), TOOLBOX_PRECISION);
}

/**
 * Test the function to multiply a 2d vector with a scalar:
 * Multiplication of x normal vector with -1.0 should result in a unit vector pointing towards the negative x axis.
 * \sdd{WI-13917}
 */
TEST(StVector2dTest, WI_15311_Vector_2d_Alg_Multiply_Scalar__Multiply_X_Normal_With_Minus_One_Should_Be_The_Same_as_Rotating_By_180_Deg)
{
   /** \arrange Create a normal vector in x direction */
   Vector_2d_T vec_x_normal = Create_2d_Vector_X_Normal();

   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Multiply_Scalar(&vec_x_normal, -1.0f);

   /** \assert Result matches unit vector in direction of negative x axis*/
   Vector_2d_T expected_result = Create_2d_Vector_Coordinates(-1.0f, 0.0f);

   EXPECT_VECTOR_2D_NEAR(test_result, expected_result, TOOLBOX_PRECISION);
}


/**
 * Test the function to add a 2d vector
 * Adding x normal to y normal should be first bisectrix.
 * \sdd{WI-13919}
 */
TEST(StVector2dTest, WI_15312_Vector_2d_Alg_Add__Add_X_Normal_To_Y_Normal_Should_Be_first_Bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Add(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_first_bisectrix, TOOLBOX_PRECISION);
}

/**
 * Test the function to add a 2d vector
 * The origin should be the neutral element
 * \sdd{WI-13919}
 */
TEST(StVector2dTest, WI_15313_Vector_2d_Alg_Add__The_Origin_Should_Be_The_Neutral_Element)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Add(&Basic_Vectors::vec_arbitrary1, &origin);

   /** \assert */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_arbitrary1, TOOLBOX_PRECISION);
}

/**
 * Test the function to add a 2d vector
 * Vector addition should be commutative.
 * \sdd{WI-13919}
 */
TEST(StVector2dTest, WI_15314_Vector_2d_Alg_Add__Vector_Addition_should_be_commutative)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Add(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \action call function under test with swapped arguments*/
   const Vector_2d_T test_result2 = Vector_2d_Alg_Add(&Basic_Vectors::vec_arbitrary2, &Basic_Vectors::vec_arbitrary1);

   /** \assert Expect the results to be identical */
   EXPECT_VECTOR_2D_NEAR(test_result, test_result2, TOOLBOX_PRECISION);
}



/**
 * Test the function to process middle values of a 2d vector element
 * Middle point of x and y normal should be one half of first bisectrix.
 * \sdd{WI-13923}
 */
TEST(StVector2dTest, WI_15315_Vector_2d_Alg_Middle__middle_point_Of_X_And_Y_Normal_should_be_one_half_of_first_bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Middle(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert Result matches first bisectrix*/
   EXPECT_VECTOR_2D_NEAR(test_result, Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_first_bisectrix, 0.5f), TOOLBOX_PRECISION);
}

/**
 * Test the function to process middle values of a 2d vector element
 * Middle point of first and third bisectrix should be the origin
 * \sdd{WI-13923}
 */
TEST(StVector2dTest, WI_15316_Vector_2d_Alg_Middle__middle_point_of_first_and_third_bisectrix_should_be_the_Origin)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Middle(&Basic_Vectors::vec_first_bisectrix, &Basic_Vectors::vec_third_bisectrix);

   /** \assert Result is origin*/
   EXPECT_VECTOR_2D_NEAR(test_result, Create_2d_Vector_Origin(), TOOLBOX_PRECISION);
}


/**
 * Test the function to process a normal of a 2d vector:
 * Norm of the origin should be zero
 * \sdd{WI-13937}
 */
TEST(StVector2dTest, WI_15317_Vector_2d_Alg_Abs__Norm_of_the_origin_should_be_zero)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Abs(&origin);

   /** \assert */
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
 * Test the function to process a normal of a 2d vector:
 * Norm of the x normal should be one
 * \sdd{WI-13937}
 */
TEST(StVector2dTest, WI_15318_Vector_2d_Alg_Abs__Norm_of_the_x_normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Abs(&Basic_Vectors::vec_x_normal);

   /** \assert Norm is one */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
 * Test the function to process a normal of a 2d vector:
 * Norm of the y normal should be one
 * \sdd{WI-13937}
 */
TEST(StVector2dTest, WI_15319_Vector_2d_Alg_Abs__Norm_of_the_y_normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Abs(&Basic_Vectors::vec_y_normal);

   /** \assert Norm is one */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
 * Test the function to process a normal of a 2d vector:
 * Norm of the 3d bisectrix normal should be square root of 2
 * \sdd{WI-13937}
 */
TEST(StVector2dTest, WI_15320_Vector_2d_Alg_Abs__Norm_of_the_3th_bisectrix_should_be_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Abs(&Basic_Vectors::vec_third_bisectrix);

   /** \assert Norm is square root of 2 */
   EXPECT_FLOAT_EQ(test_result, sqrt(2.0f));
}

/**
 * Test the function to process the scalar product of two 2d vectors:
 * Scalar product of any arbitrary vector with the origin should be zero
 * \sdd{WI-13924}
 */
TEST(StVector2dTest, WI_15321_Vector_2d_Alg_Scalar_Product__Scalar_product_of_any_arbitrary_vector_with_the_origin_should_be_zero)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_arbitrary1, &origin);

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
 * Test the function to process the scalar product of two 2d vectors:
 * Scalar product of any arbitrary vector with the origin should be zero
 * \sdd{WI-13924}
 */
TEST(StVector2dTest, WI_15322_Vector_2d_Alg_Scalar_Product__Scalar_product_of_any_arbitrary2_with_the_origin_should_be_zero)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_arbitrary2, &origin);

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
 * Test the function to process the scalar product of two 2d vectors:
 * Scalar product of x normal and y normal should be zero
 * \sdd{WI-13924}
 */
TEST(StVector2dTest, WI_15323_Vector_2d_Alg_Scalar_Product__Scalar_product_of_X_and_Y_Normal_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
 * Test the function to process the scalar product of two 2d vectors:
 * Scalar product of x normal and any arbitrary vector should be x component of arbitrary vector
 * \sdd{WI-13924}
 */
TEST(StVector2dTest, WI_15324_Vector_2d_Alg_Scalar_Product__Scalar_product_of_X_Normal_with_vec_arbitrary1_should_be_the_x_component)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_arbitrary1);

   /** \assert Scalar product is x component */
   EXPECT_FLOAT_EQ(test_result, Basic_Vectors::vec_arbitrary1.x);
}

/**
 * Test the function to process the scalar product of two 2d vectors:
 * Scalar product of x normal and any arbitrary vector should be x component of arbitrary vector
 * \sdd{WI-13924}
 */
TEST(StVector2dTest, WI_15325_Vector_2d_Alg_Scalar_Product__Scalar_product_of_X_Normal_with_vec_arbitrary1_should_be_the_y_component)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_y_normal, &Basic_Vectors::vec_arbitrary1);

   /** \assert Scalar product is x component */
   EXPECT_FLOAT_EQ(test_result, Basic_Vectors::vec_arbitrary1.y);
}

/**
 * Test the function to process the scalar product of two 2d vectors:
 * Scalar product should be commutative
 * \sdd{WI-13924}
 */
TEST(StVector2dTest, WI_15326_Vector_2d_Alg_Scalar_Product__Scalar_product_should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_arbitrary2, &Basic_Vectors::vec_arbitrary1);

   /** \action call function under test with swapped arguments */
   const float32_T test_result2 = Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \assert Results match*/
   EXPECT_FLOAT_EQ(test_result, test_result2);
}


/**
 * Test the function to process the distance between two 2d vectors:
 * Distance between the origin and any vector should be the norm
 * \sdd{WI-13921}
 */
TEST(StVector2dTest, WI_15327_Vector_2d_Alg_Distance__Distance_between_the_origin_and_any_vector_should_be_the_norm)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Distance(&origin, &Basic_Vectors::vec_arbitrary1);

   /** \assert Distance matches the norm*/
   EXPECT_FLOAT_EQ(test_result, Vector_2d_Alg_Abs(&Basic_Vectors::vec_arbitrary1));
}

/**
 * Test the function to process the distance between two 2d vectors:
 * Distance between the x and the y normal should be square root of 2
 * \sdd{WI-13921}
 */
TEST(StVector2dTest, WI_15328_Vector_2d_Alg_Distance__Distance_between_the_X_and_the_Y_normal_should_be_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Distance(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert Distance matches square root of 2 */
   EXPECT_FLOAT_EQ(test_result, sqrt(2.0f));
}

/**
 * Test the function to process the distance between two 2d vectors:
 * The distance computation should be commutative
 * \sdd{WI-13921}
 */
TEST(StVector2dTest, WI_15329_Vector_2d_Alg_Distance__The_distance_computation_should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Distance(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \action call function under test with swapped arguments */
   const float32_T test_result2 = Vector_2d_Alg_Distance(&Basic_Vectors::vec_arbitrary2, &Basic_Vectors::vec_arbitrary1);

   /** \assert Results match */
   EXPECT_FLOAT_EQ(test_result, test_result2);
}

/**
 * Test the function to rotate a 2d vector on 90 degree:
 * Rotate x normal should be y normal
 * \sdd{WI-13936}
 */
TEST(StVector2dTest, WI_15330_Vector_2d_Alg_Rotate_Half_Pi__Rotate_X_normal_should_be_Y_normal)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Rotate_Half_Pi(&Basic_Vectors::vec_x_normal);

   /** \assert Result matches y normal*/
   EXPECT_VECTOR_2D_EQ(test_result, Basic_Vectors::vec_neg_y_normal);
}

/**
 * Test the function to rotate a 2d vector on 90 degree:
 * Multiply a vector with its rotated counter part should be zero
 * \sdd{WI-13936}
 */
TEST(StVector2dTest, WI_15331_Vector_2d_Alg_Rotate_Half_Pi__Multiply_a_vector_with_its_rotated_counter_part_should_be_zero)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Rotate_Half_Pi(&Basic_Vectors::vec_x_normal);

   /** \assert Result of multiplication matches zero */
   EXPECT_FLOAT_EQ(Vector_2d_Alg_Scalar_Product(&Basic_Vectors::vec_x_normal, &test_result), 0.0f);
}


/**
 * Test the function to normalize a vector (scale a vector length to 1):
 * Norm of a normalized vector should be one
 * \sdd{WI-13914}
 */
TEST(StVector2dTest, WI_15332_Vector_2d_Alg_Normalize_Vector__Norm_of_a_normalized_vector_should_be_one)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Normalize_Vector(&Basic_Vectors::vec_arbitrary1);

   /** \assert Result equals 1*/
   EXPECT_FLOAT_EQ(Vector_2d_Alg_Abs(&test_result), 1.0f);
}


/**
 * Test the function to process the square magnitude of a 2d vector:
 * Norm squared of x normal should be one
 * \sdd{WI-13935}
 */
TEST(StVector2dTest, WI_15333_Vector_2d_Alg_Abs_Squared__Norm_squared_of_X_Normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Abs_Squared(&Basic_Vectors::vec_x_normal);

   /** \assert Result equals 1 */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
 * Test the function to process the square magnitude of a 2d vector:
 * Norm squared of a vector should be the norm squared
 * \sdd{WI-13935}
 */
TEST(StVector2dTest, WI_15334_Vector_2d_Alg_Abs_Squared__Norm_squared_of_a_vector_should_be_the_norm_squared)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Abs_Squared(&Basic_Vectors::vec_arbitrary1);

   /** \assert Expect result to match the squared norm */
   float32_T norm_squared = powf(Vector_2d_Alg_Abs(&Basic_Vectors::vec_arbitrary1), 2.0f);

   EXPECT_FLOAT_EQ(test_result, norm_squared);
}

/**
 * Test the function to compute the subtraction of two 2d vectors:
 * Diff by the origin should be the same
 * \sdd{WI-13915}
 */
TEST(StVector2dTest, WI_15335_Vector_2d_Alg_Diff__Diff_by_the_origin_should_be_the_same)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Diff(&Basic_Vectors::vec_arbitrary1, &origin);

   /** \assert Result matches parameter */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_arbitrary1, TOOLBOX_PRECISION);
}

/**
 * Test the function to compute the subtraction of two 2d vectors:
 * Diff y normal from x normal should be second bisectrix
 * \sdd{WI-13915}
 */
TEST(StVector2dTest, WI_15336_Vector_2d_Alg_Diff__Diff_Y_Normal_from_X_normal_should_be_second_bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Diff(&Basic_Vectors::vec_y_normal, &Basic_Vectors::vec_x_normal);

   /** \assert Result is second bisectrix */
   EXPECT_VECTOR_2D_NEAR(test_result, Basic_Vectors::vec_second_bisectrix, TOOLBOX_PRECISION);
}

/**
 * Test the function to compute the subtraction of two 2d vectors:
 * Diff should be commutative by a factor minus one
 * \sdd{WI-13915}
 */
TEST(StVector2dTest, WI_15337_Vector_2d_Alg_Diff__Diff_should_be_commutative_by_a_factor_minus_one)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Diff(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \action call function under test with swapped arguments */
   const Vector_2d_T test_result2 = Vector_2d_Alg_Diff(&Basic_Vectors::vec_arbitrary2, &Basic_Vectors::vec_arbitrary1);

   /** \assert result matches by a factor minus one */
   EXPECT_VECTOR_2D_NEAR(test_result, Vector_2d_Alg_Multiply_Scalar(&test_result2, -1.0f), TOOLBOX_PRECISION);
}


/**
 * Test the function to process the absolute value of each 2d vector element
 * Component norm of 3th bisectrix should be first bisectrix
 * \sdd{WI-13938}
 */
TEST(StVector2dTest, WI_15338_Vector_2d_Alg_Abs_Component_Wise__Component_norm_of_3th_bisectrix_should_be_first_bisectrix)
{
   /** \action call function under test */
   Vector_2d_T result = Vector_2d_Alg_Abs_Component_Wise(&Basic_Vectors::vec_third_bisectrix);

   /** \assert Result is first bisectrix */
   EXPECT_VECTOR_2D_NEAR(Basic_Vectors::vec_first_bisectrix, result, TOOLBOX_PRECISION);
}


/**
 * Test the function to compute the square root of each 2d vector elements
 * first bisectrix should not change
 * \sdd{WI-13929}
 */
TEST(StVector2dTest, WI_16192_Vector_2d_Alg_Sqrt_Component_Wise__first_bisectrix_should_not_change)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Vector_2d_Alg_Sqrt_Component_Wise(&Basic_Vectors::vec_first_bisectrix);

   /** \assert Result matches parameter */
   EXPECT_VECTOR_2D_NEAR(Basic_Vectors::vec_first_bisectrix, test_result, TOOLBOX_PRECISION);
}

/**
 * Test the function to compute the square root of each 2d vector elements
 * x and y component of 2 times first bisectrix should be sqrt2
 * \sdd{WI-13929}
 */
TEST(StVector2dTest, WI_16193_Vector_2d_Alg_Sqrt_Component_Wise__X_and_Y_component_of_2_times_first_bisectrix_should_be_sqrt2)
{
   /** \action call function under test */
   Vector_2d_T       scaled_vector = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_first_bisectrix, 2.0f);
   const Vector_2d_T test_result    = Vector_2d_Alg_Sqrt_Component_Wise(&scaled_vector);

   /** \assert x component matches square root of 2 */
   EXPECT_FLOAT_EQ(test_result.x, sqrt(2.0f));

   /** \assert y component matches square root of 2 */
   EXPECT_FLOAT_EQ(test_result.y, sqrt(2.0f));
}

/**
 * Test the function to compute the cosine of the angle between two 2d vectors
 * If a vector is too short VECTOR_2D_ALGEBRA_NOT_A_COSINE is returned, an assertion fires in debug.
 * \sdd{WI-13931}
 * \sdd{WI-13943}
 */
TEST(StVector2dTest, WI_15341_Vector_2d_Alg_Calculate_Cos_Between_Two_Vec__Vector_a_too_short)
{
   /** \arrange Create a short vector */
   Vector_2d_T vector_a = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_x_normal, THRESHOLD_IS_ZERO);
   float32_T   test_result;

   /** \action call function under test */
   EXPECT_DEBUG_DEATH(
      test_result = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&vector_a, &Basic_Vectors::vec_y_normal),
      "FALSE");
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
TEST(StVector2dTest, WI_15342_Vector_2d_Alg_Calculate_Cos_Between_Two_Vec__Vector_b_too_short)
{
   /** \arrange Create a short vector */
   Vector_2d_T vector_b = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_y_normal, THRESHOLD_IS_ZERO);
   float32_T   test_result;

   /** \action call function under test */
   EXPECT_DEBUG_DEATH(
      test_result = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&Basic_Vectors::vec_x_normal, &vector_b),
      "FALSE");
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
TEST(StVector2dTest, WI_15343_Vector_2d_Alg_Calculate_Cos_Between_Two_Vec__Cos_between_X_and_Y_normal_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert result is zero */
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
 * Test the function to compute the cosine of the angle between two 2d vectors
 * Cosine between x and negative x normal should be minus one
 * \sdd{WI-13931}
 * \sdd{WI-13943}
 */
TEST(StVector2dTest, WI_15344_Vector_2d_Alg_Calculate_Cos_Between_Two_Vec__Cos_between_X_and_neg_X_normal_should_be_minus_one)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_neg_x_normal);

   /** \assert Matches minus one */
   EXPECT_FLOAT_EQ(test_result, -1.0f);
}

/**
 * Test the function to compute the cosine of the angle between two 2d vectors
 * Cosine between x and first bisectrix normal should be one half af the square root of two
 * \sdd{WI-13931}
 * \sdd{WI-13943}
 */
TEST(StVector2dTest, WI_15345_Vector_2d_Alg_Calculate_Cos_Between_Two_Vec__Cos_between_X_and_first_bisectrix_should_be_one_half_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_first_bisectrix);

   /** \assert Result matches one half af the square root of two*/
   EXPECT_FLOAT_EQ(test_result, 0.5f * sqrtf(2));
}

/**
 * Test the function to compute the cosine of the angle between two 2d vectors
 * Should be commutative
 * \sdd{WI-13931}
 * \sdd{WI-13943}
 */
TEST(StVector2dTest, WI_15346_Vector_2d_Alg_Calculate_Cos_Between_Two_Vec__Should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \action call function under test with swapped parameters */
   const float32_T test_result2 = Vector_2d_Alg_Calculate_Cos_Between_Two_Vec(&Basic_Vectors::vec_arbitrary2, &Basic_Vectors::vec_arbitrary1);

   /** \assert Results match */
   EXPECT_FLOAT_EQ(test_result, test_result2);
}

/**
 * Test the function to limit the values of an 2d vector to a certain range
 * Limiting negative x normal to first quadrant should be the origin
 * \sdd{WI-13927}
 */
TEST(StVector2dTest, WI_15347_Vector_2d_Alg_Limit_Vector__Limit_neg_X_normal_to_first_quadrant_should_be_the_origin)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   Vector_2d_T result = Vector_2d_Alg_Limit_Vector(&origin, &Basic_Vectors::vec_first_bisectrix, &Basic_Vectors::vec_neg_x_normal);

   /** \assert Result matches origin*/
   EXPECT_VECTOR_2D_NEAR(result, origin, TOOLBOX_PRECISION);
}

/**
 * Test the function to limit the values of an 2d vector to a certain range
 * Limiting negative y normal to first quadrant should be the origin
 * \sdd{WI-13927}
 */
TEST(StVector2dTest, WI_15348_Vector_2d_Alg_Limit_Vector__Limit_neg_Y_normal_to_first_quadrant_should_be_the_origin)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   Vector_2d_T result = Vector_2d_Alg_Limit_Vector(&origin, &Basic_Vectors::vec_first_bisectrix, &Basic_Vectors::vec_neg_y_normal);

   /** \assert Result matches origin*/
   EXPECT_VECTOR_2D_NEAR(result, origin, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a 2d vector
 * \sdd{WI-13925}
 */
TEST(StVector2dTest, WI_15349_Create_2d_Vector_Coordinates_test)
{
   /** \arrange */
   float32_T x_component = 0.78f;
   float32_T y_component = 37.8f;

   /** \action call function under test */
   const Vector_2d_T test_result = Create_2d_Vector_Coordinates(x_component, y_component);

   /** \assert a vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, x_component);
   EXPECT_FLOAT_EQ(test_result.y, y_component);
}

/**
 * Test the function to create a zero 2d vector
 * \sdd{WI-13934}
 */
TEST(StVector2dTest, WI_15350_Create_2d_Vector_Origin_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Create_2d_Vector_Origin();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
}

/**
 * Test the function to create a normal 2d vector to y axis
 * \sdd{WI-13933}
 */
TEST(StVector2dTest, WI_15351_Create_2d_Vector_X_Normal_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Create_2d_Vector_X_Normal();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 1.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
}

/**
 * Test the function to create a normal 2d vector to x axis
 * \sdd{WI-13930}
 */
TEST(StVector2dTest, WI_15352_Create_2d_Vector_Y_Normal_test)
{
   /** \action call function under test */
   const Vector_2d_T test_result = Create_2d_Vector_Y_Normal();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 1.0f);
}

