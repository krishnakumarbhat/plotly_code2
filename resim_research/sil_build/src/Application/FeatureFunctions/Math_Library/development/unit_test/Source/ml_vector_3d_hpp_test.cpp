/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>

#include <math.h>
#include "st_vector_3d_helper.hpp"

#include "ml_vector_3d.hpp"
#include "ml_angle.hpp"
#include "ml_vector_3d.h"
#include "ml_math.h"

#define TOOLBOX_PRECISION    (1e-6)

struct Basic_3d_Vectors
{
	Vector_3d_T vec_arbitrary1 /**< This is an arbitrary vector*/;
	Vector_3d_T vec_arbitrary2 /**< This is an arbitrary vector*/;
	Vector_3d_T vec_x_normal; /**< x_normal */
	Vector_3d_T vec_y_normal; /**< y_normal */
   Vector_3d_T vec_z_normal; /**< y_normal */
	Vector_3d_T vec_neg_x_normal; /**< negative x_normal */
	Vector_3d_T vec_neg_y_normal; /**< negative y_normal */
   Vector_3d_T vec_neg_z_normal; /**< negative y_normal */
	Vector_3d_T vec_first_bisectrix; /**< vector bisecting the first 2d sector */
	Vector_3d_T vec_second_bisectrix; /**< vector bisecting the 2th 2d sector */
	Vector_3d_T vec_third_bisectrix; /**< vector bisecting the 3th 2d sector */
	Vector_3d_T vec_fourth_bisectrix; /**< vector bisecting the 4th 2d sector */
	Basic_3d_Vectors()
    {
      vec_x_normal = Create_3d_Vector_X_Normal();
      vec_y_normal = Create_3d_Vector_Y_Normal();
      vec_z_normal = Create_3d_Vector_Z_Normal();
      vec_neg_x_normal = Create_3d_Vector_Coordinates(-1., 0., 0.f);
      vec_neg_y_normal = Create_3d_Vector_Coordinates(0., -1., 0.f);
      vec_neg_z_normal = Create_3d_Vector_Coordinates(0., 0.f, -1.);
      vec_first_bisectrix = Create_3d_Vector_Coordinates(1., 1., 0.0f);
      vec_second_bisectrix = Create_3d_Vector_Coordinates(-1., 1., 0.0f);
      vec_third_bisectrix = Create_3d_Vector_Coordinates(-1., -1., 0.0f);
      vec_fourth_bisectrix = Create_3d_Vector_Coordinates(1., -1., 0.0f);
      vec_arbitrary1 = Create_3d_Vector_Coordinates(4275.82323234f, 4443.11f, 124.0f);
      vec_arbitrary2 = Create_3d_Vector_Coordinates(4886.43332f, -1244.334f, 9786.756f);
    }
};

static const Basic_3d_Vectors basic_3d_vectors;


/** Test that vector addition works */
TEST(MlVector3dHppTest, operator_plus)
{
   /** \arrange create two vectors containing arbitrary values */
   Vector_3d_T a = { 0.0f, 0.0f };
   Vector_3d_T b = { 1.0f, 2.0f };

   /**\action perform addition */
   auto c = a + b;

   /** \assert result matches component wise addition */
   EXPECT_EQ(c.x, a.x + b.x);
   EXPECT_EQ(c.y, a.y + b.y);
}


/** Test that vector addition works within namespace 'st'*/
TEST(MlVector3dHppTest, operator_plus_namespace_st)
{
   /** \arrange create two vectors within namespace st containing arbitrary values */
   ml::Vector_3d_T a = { 0.0f, 0.0f };
   ml::Vector_3d_T b = { 1.0f, 2.0f };

   /**\action perform addition */
   auto c = a + b;

   /** \assert result matches component wise addition */
   EXPECT_EQ(c.x, a.x + b.x);
   EXPECT_EQ(c.y, a.y + b.y);
}

/**
 * Subtract two vectors
 */
TEST(MlVector3dHppTest, operator_minus)
{
   /** \arrange Create two arbitrary vectors */
   Vector_3d_T a = { 0.0f, 0.0f };
   Vector_3d_T b = { 1.0f, 2.0f };

   /** call function under test */
   Vector_3d_T c = a - b;

   /** \assert result matches component wise subtraction */
   EXPECT_EQ(c.x, a.x - b.x);
   EXPECT_EQ(c.y, a.y - b.y);
}

/**
 * Scale a vector
 */
TEST(MlVector3dHppTest, operator_multiply_scalar)
{
   /** \arrange Create one arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f };

   /** \arrange Pick an arbitrary float */
   float b = 2.0f;

   /** call function under test */
   Vector_3d_T c = a * b;

   /** \assert result matches component wise scaling */
   EXPECT_EQ(c.x, a.x * b);
   EXPECT_EQ(c.y, a.y * b);
}

/**
* Scale a vector
*/
TEST(MlVector3dHppTest, operator_multiply_scalar_reverse)
{
   /** \arrange Create one arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f };

   /** \arrange Pick an arbitrary float */
   float b = 2.0f;

   /** call function under test */
   Vector_3d_T c = b * a;

   /** \assert result matches component wise scaling */
   EXPECT_EQ(c.x, b * a.x);
   EXPECT_EQ(c.y, b * a.y);
}

/**
* Scale a vector while assigning
*/
TEST(MlVector3dHppTest, operator_multiply_scalar_assign)
{
   /** \arrange Create one arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f };

   Vector_3d_T res = a;

   /** \arrange Pick an arbitrary float */
   float b = 2.0f;

   /** call function under test */
   res *= b;

   /** \assert result matches component wise scaling */
   EXPECT_EQ(res.x, b * a.x);
   EXPECT_EQ(res.y, b * a.y);
}

/**
 * Scalar product of two vectors
 */
TEST(MlVector3dHppTest, operator_scalar_product)
{
   /** \arrange pick two arbitrary vectors */
   Vector_3d_T a = { 1.0f, 2.0f };
   Vector_3d_T b = { 1.0f, 2.0f };

   /** call function under test */
   float c = b * a;

   /** \assert The result matches scalar product */
   EXPECT_EQ(c, 5.0f);
}

/**
 * Unary minus for a vector
 */
TEST(MlVector3dHppTest, unary_minus)
{
   /** \arrange pick an arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f };

   /** call function under test */
   Vector_3d_T b = -a;

   /** \assert result matches component wise scaling with -1 */
   EXPECT_EQ(b.x, -a.x);
   EXPECT_EQ(b.y, -a.y);
}

/**
* Unary minus for a vector
*/
TEST(MlVector3dHppTest, unary_plus)
{
   /** \arrange pick an arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f };

   /** call function under test */
   Vector_3d_T b = +a;

   /** \assert result matches original vector */
   EXPECT_VECTOR_3D_EQ(b, a);
}

/**
 * The at function returns the x component
 */
TEST(MlVector3dHppTest, at_returns_x)
{
   /** \arrange pick an arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f, 3.0f };

   /** \action at function returns the x component */
   EXPECT_EQ(a.x, at(a, 0));
}

/**
* The at function returns the y component
*/
TEST(MlVector3dHppTest, at_returns_y)
{
   /** \arrange pick an arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f, 3.0f };

   /** \action at function returns the y component */
   EXPECT_EQ(a.y, at(a, 1));
}

/**
* The at function returns the z component
*/
TEST(MlVector3dHppTest, at_returns_z)
{
   /** \arrange pick an arbitrary vector */
   Vector_3d_T a = { 1.0f, 2.0f, 3.0f };

   /** \action at function returns the y component */
   EXPECT_EQ(a.z, at(a, 2));
}

/**
* Test the function to multiply a 3d vector with a scalar:
* Multiply a vector with 0.0 should result in origin vector
* \sdd{WI-13917}
*/
TEST(MlVector3dHppTest, Multiply_X_Normal_With_Null_Should_Result_In_The_Origin)
{
   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Multiply_Scalar(&basic_3d_vectors.vec_x_normal, 0.0f);

   const Vector_3d_T test_result = basic_3d_vectors.vec_x_normal * 0.0f;

   /** \assert */
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}

/**
* Test the function to multiply a 3d vector with a scalar:
* Multiply a vector with 0.0 should result in origin vector.
Testing the assigning variant.
* \sdd{WI-13917}
*/
TEST(MlVector3dHppTest, Multiply_X_Normal_With_Null_Should_Result_In_The_Origin_assigning)
{
   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Multiply_Scalar(&basic_3d_vectors.vec_x_normal, 0.0f);

   Vector_3d_T test_result = basic_3d_vectors.vec_x_normal;
   test_result *= 0.0f;

   /** \assert */
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}


/**
* Test the function to multiply a 3d vector with a scalar:
* Multiplication of x normal vector with -1.0 should result in a unit vector pointing towards the negative x axis.
* \sdd{WI-13917}
*/
TEST(MlVector3dHppTest, Multiply_X_Normal_With_Minus_One_Should_Be_The_Same_as_Rotating_By_180_Deg)
{
   /** \arrange Create a normal vector in x direction */
   Vector_3d_T vec_x_normal = Create_3d_Vector_X_Normal();

   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Multiply_Scalar(&vec_x_normal, -1.0f);

   /** \assert Result matches unit vector in direction of negative x axis*/
   Vector_3d_T test_result = vec_x_normal * -1.0f;

   EXPECT_VECTOR_3D_EQ(test_result, reference);
}


/**
* Test the function to add a 3d vector
* Adding x normal to y normal should be first bisectrix.
* \sdd{WI-13919}
*/
TEST(MlVector3dHppTest, Add_X_Normal_To_Y_Normal_Should_Be_first_Bisectrix)
{
   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Add(&basic_3d_vectors.vec_x_normal, &basic_3d_vectors.vec_y_normal);

   const Vector_3d_T test_result = basic_3d_vectors.vec_x_normal + basic_3d_vectors.vec_y_normal;

   /** \assert */
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}

/**
* Test the function to add a 3d vector
* Adding x normal to y normal should be first bisectrix.
* testing the assigning variant.
* \sdd{WI-13919}
*/
TEST(MlVector3dHppTest, Add_X_Normal_To_Y_Normal_Should_Be_first_Bisectrix_assigning)
{
   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Add(&basic_3d_vectors.vec_x_normal, &basic_3d_vectors.vec_y_normal);

   Vector_3d_T test_result = basic_3d_vectors.vec_x_normal;
   test_result  += basic_3d_vectors.vec_y_normal;

   /** \assert */
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}

/**
* Test the function to add a 3d vector
* The origin should be the neutral element
* \sdd{WI-13919}
*/
TEST(MlVector3dHppTest, Add__The_Origin_Should_Be_The_Neutral_Element)
{
   /** \arrange create an origin  vector */
   Vector_3d_T origin = Create_3d_Vector_Origin();

   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Add(&basic_3d_vectors.vec_arbitrary1, &origin);

   const Vector_3d_T test_result = basic_3d_vectors.vec_arbitrary1 + origin;

   /** \assert */
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}

/**
* Test the function to add a 3d vector
* Vector addition should be commutative.
* \sdd{WI-13919}
*/
TEST(MlVector3dHppTest, Vector_Addition_should_be_commutative)
{
   /** \action call function under test */
   const Vector_3d_T test_result = basic_3d_vectors.vec_arbitrary1 + basic_3d_vectors.vec_arbitrary2;

   /** \action call function under test with swapped arguments*/
   const Vector_3d_T test_result2 = basic_3d_vectors.vec_arbitrary2 + basic_3d_vectors.vec_arbitrary1;

   /** \assert Expect the results to be identical */
   EXPECT_VECTOR_3D_NEAR(test_result, test_result2, TOOLBOX_PRECISION);
}



/**
* Test the function to process middle values of a 3d vector element
* Middle point of x and y normal should be one half of first bisectrix.
* \sdd{WI-13923}
*/
TEST(MlVector3dHppTest, middle_point_Of_X_And_Y_Normal_should_be_one_half_of_first_bisectrix)
{
   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Middle(&basic_3d_vectors.vec_x_normal, &basic_3d_vectors.vec_y_normal);

   const Vector_3d_T test_result = Middle(basic_3d_vectors.vec_x_normal, basic_3d_vectors.vec_y_normal);

   /** \assert Result matches first bisectrix*/
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}

/**
* Test the function to process middle values of a 3d vector element
* Middle point of first and third bisectrix should be the origin
* \sdd{WI-13923}
*/
TEST(MlVector3dHppTest, middle_point_of_first_and_third_bisectrix_should_be_the_Origin)
{
   /** \action call function under test */
   const Vector_3d_T reference = Vector_3d_Alg_Middle(&basic_3d_vectors.vec_first_bisectrix, &basic_3d_vectors.vec_third_bisectrix);

   const Vector_3d_T test_result = Middle(basic_3d_vectors.vec_first_bisectrix, basic_3d_vectors.vec_third_bisectrix);

   /** \assert Result matches first bisectrix*/
   EXPECT_VECTOR_3D_EQ(test_result, reference);
}


/**
* Test the function to process a normal of a 3d vector:
* Norm of the origin should be zero
* \sdd{WI-13937}
*/
TEST(MlVector3dHppTest, Norm_of_the_origin_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = Norm(ml::vector3d::Vector());

   /** \assert */
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process a normal of a 3d vector:
* Norm of the x normal should be one
* \sdd{WI-13937}
*/
TEST(MlVector3dHppTest, Norm_of_the_x_normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Norm(ml::vector3d::X());

   /** \assert Norm is one */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
* Test the function to process a normal of a 3d vector:
* Norm of the y normal should be one
* \sdd{WI-13937}
*/
TEST(MlVector3dHppTest, Norm_of_the_y_normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Norm(basic_3d_vectors.vec_y_normal);

   /** \assert Norm is one */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
* Test the function to process a normal of a 3d vector:
* Norm of the 3d bisectrix normal should be square root of 2
* \sdd{WI-13937}
*/
TEST(MlVector3dHppTest, Norm_of_the_3th_bisectrix_should_be_sqrt2)
{
   /** \action call function under test */
   const float32_T test_result = Norm(basic_3d_vectors.vec_third_bisectrix);

   /** \assert Norm is square root of 2 */
   EXPECT_FLOAT_EQ(test_result, sqrt(2.0f));
}

/**
* Test the function to process the scalar product of two 3d vectors:
* Scalar product of any arbitrary vector with the origin should be zero
* \sdd{WI-13924}
*/
TEST(MlVector3dHppTest, Scalar_product_of_any_arbitrary_vector_with_the_origin_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = basic_3d_vectors.vec_arbitrary1 * ml::vector3d::Vector();

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process the scalar product of two 3d vectors:
* Scalar product of any arbitrary vector with the origin should be zero
* \sdd{WI-13924}
*/
TEST(MlVector3dHppTest, Scalar_product_of_any_arbitrary2_with_the_origin_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = basic_3d_vectors.vec_arbitrary2 * ml::vector3d::Vector();

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process the scalar product of two 3d vectors:
* Scalar product of x normal and y normal should be zero
* \sdd{WI-13924}
*/
TEST(MlVector3dHppTest, Scalar_product_of_X_and_Y_Normal_should_be_zero)
{
   /** \action call function under test */
   const float32_T test_result = ml::vector3d::X() * ml::vector3d::Y();

   /** \assert Scalar product is close to zero*/
   EXPECT_FLOAT_EQ(test_result, 0.0f);
}

/**
* Test the function to process the scalar product of two 3d vectors:
* Scalar product of x normal and any arbitrary vector should be x component of arbitrary vector
* \sdd{WI-13924}
*/
TEST(MlVector3dHppTest, Scalar_product_of_X_Normal_with_vec_arbitrary1_should_be_the_x_component)
{
   /** \action call function under test */
   const float32_T test_result = ml::vector3d::X() * basic_3d_vectors.vec_arbitrary1;

   /** \assert Scalar product is x component */
   EXPECT_FLOAT_EQ(test_result, basic_3d_vectors.vec_arbitrary1.x);
}

/**
* Test the function to process the scalar product of two 3d vectors:
* Scalar product of x normal and any arbitrary vector should be x component of arbitrary vector
* \sdd{WI-13924}
*/
TEST(MlVector3dHppTest, Scalar_product_of_X_Normal_with_vec_arbitrary1_should_be_the_y_component)
{
   /** \action call function under test */
   const float32_T test_result = ml::vector3d::Y() * basic_3d_vectors.vec_arbitrary1;

   /** \assert Scalar product is x component */
   EXPECT_FLOAT_EQ(test_result, basic_3d_vectors.vec_arbitrary1.y);
}

/**
* Test the function to process the scalar product of two 3d vectors:
* Scalar product should be commutative
* \sdd{WI-13924}
*/
TEST(MlVector3dHppTest, Scalar_product_should_be_commutative)
{
   /** \action call function under test */
   const float32_T test_result = basic_3d_vectors.vec_arbitrary2 * basic_3d_vectors.vec_arbitrary1;

   /** \action call function under test with swapped arguments */
   const float32_T test_result2 = basic_3d_vectors.vec_arbitrary1 * basic_3d_vectors.vec_arbitrary2;

   /** \assert Results match*/
   EXPECT_FLOAT_EQ(test_result, test_result2);
}


/**
* Test the function to normalize a vector (scale a vector length to 1):
* Norm of a normalized vector should be one
* \sdd{WI-13914}
*/
TEST(MlVector3dHppTest, Norm_of_a_normalized_vector_should_be_one)
{
   /** \action call function under test */
   const Vector_3d_T test_result = Normalize(basic_3d_vectors.vec_arbitrary1);

   /** \assert Result equals 1*/
   EXPECT_FLOAT_EQ(Norm(test_result), 1.0f);
}


/**
* Test the function to process the square magnitude of a 3d vector:
* Norm squared of x normal should be one
* \sdd{WI-13935}
*/
TEST(MlVector3dHppTest, Norm_squared_of_X_Normal_should_be_one)
{
   /** \action call function under test */
   const float32_T test_result = Norm_Squared(basic_3d_vectors.vec_x_normal);

   /** \assert Result equals 1 */
   EXPECT_FLOAT_EQ(test_result, 1.0f);
}

/**
* Test the function to process the square magnitude of a 3d vector:
* Norm squared of a vector should be the norm squared
* \sdd{WI-13935}
*/
TEST(MlVector3dHppTest, Norm_squared_of_a_vector_should_be_the_norm_squared)
{
   /** \action call function under test */
   const float32_T test_result = Norm_Squared(basic_3d_vectors.vec_arbitrary1);

   /** \assert Expect result to match the squared norm */
   float32_T norm_squared = powf(Norm(basic_3d_vectors.vec_arbitrary1), 2.0f);

   EXPECT_FLOAT_EQ(test_result, norm_squared);
}

/**
* Test the function to compute the subtraction of two 3d vectors:
* Diff by the origin should be the same
* \sdd{WI-13915}
*/
TEST(MlVector3dHppTest, Diff_by_the_origin_should_be_the_same)
{
   /** \action call function under test */
   const Vector_3d_T test_result = basic_3d_vectors.vec_arbitrary1 - ml::vector3d::Vector();

   /** \assert Result matches parameter */
   EXPECT_VECTOR_3D_NEAR(test_result, basic_3d_vectors.vec_arbitrary1, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the subtraction of two 3d vectors:
* Diff y normal from x normal should be second bisectrix
* \sdd{WI-13915}
*/
TEST(MlVector3dHppTest, Diff_Y_Normal_from_X_normal_should_be_second_bisectrix)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Y() - ml::vector3d::X();

   /** \assert Result is second bisectrix */
   EXPECT_VECTOR_3D_NEAR(test_result, basic_3d_vectors.vec_second_bisectrix, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the subtraction of two 3d vectors:
* Diff y normal from x normal should be second bisectrix
* testing the assigning variant.
* \sdd{WI-13915}
*/
TEST(MlVector3dHppTest, Diff_Y_Normal_from_X_normal_should_be_second_bisectrix_assigning)
{
   /** \action call function under test */
   Vector_3d_T test_result = ml::vector3d::Y();
   test_result  -= ml::vector3d::X();

   /** \assert Result is second bisectrix */
   EXPECT_VECTOR_3D_NEAR(test_result, basic_3d_vectors.vec_second_bisectrix, TOOLBOX_PRECISION);
}

/**
* Test the function to compute the subtraction of two 3d vectors:
* Diff should be commutative by a factor minus one
* \sdd{WI-13915}
*/
TEST(MlVector3dHppTest, Diff_should_be_commutative_by_a_factor_minus_one)
{
   /** \action call function under test */
   const Vector_3d_T test_result = basic_3d_vectors.vec_arbitrary1 - basic_3d_vectors.vec_arbitrary2;

   /** \action call function under test with swapped arguments */
   const Vector_3d_T test_result2 = basic_3d_vectors.vec_arbitrary2 - basic_3d_vectors.vec_arbitrary1;

   /** \assert result matches by a factor minus one */
   EXPECT_VECTOR_3D_NEAR(test_result, Vector_3d_Alg_Multiply_Scalar(&test_result2, -1.0f), TOOLBOX_PRECISION);
}


/**
* Test the function to process the absolute value of each 3d vector element
* Component norm of 3th bisectrix should be first bisectrix
* \sdd{WI-13938}
*/
TEST(MlVector3dHppTest, Component_norm_of_3th_bisectrix_should_be_first_bisectrix)
{
   /** \action call function under test */
   Vector_3d_T result = Absolute(basic_3d_vectors.vec_third_bisectrix);

   /** \assert Result is first bisectrix */
   EXPECT_VECTOR_3D_NEAR(basic_3d_vectors.vec_first_bisectrix, result, TOOLBOX_PRECISION);
}


/**
* Test the function to compute the square root of each 3d vector elements
* first bisectrix should not change
* \sdd{WI-13929}
*/
TEST(MlVector3dHppTest, Absolute_of_first_bisectrix_should_not_change)
{
   /** \action call function under test */
   const Vector_3d_T test_result = Absolute(basic_3d_vectors.vec_first_bisectrix);

   /** \assert Result matches parameter */
   EXPECT_VECTOR_3D_NEAR(basic_3d_vectors.vec_first_bisectrix, test_result, TOOLBOX_PRECISION);
}


/**
* Test the function to create a 3d vector
* \sdd{WI-13925}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_Coordinates_test)
{
   /** \arrange */
   float32_T x_component = 0.78f;
   float32_T y_component = 37.8f;
   float32_T z_component = 567.8f;

   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Vector(x_component, y_component, z_component);

   /** \assert a vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, x_component);
   EXPECT_FLOAT_EQ(test_result.y, y_component);
   EXPECT_FLOAT_EQ(test_result.z, z_component);
}

/**
* Test the function to create a zero 3d vector
* \sdd{WI-13934}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_Origin_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Vector();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
   EXPECT_FLOAT_EQ(test_result.z, 0.0f);
}

/**
* Test the function to create a normal 3d vector to y axis
* \sdd{WI-13933}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_X_Normal_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::X();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 1.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
   EXPECT_FLOAT_EQ(test_result.z, 0.0f);
}

/**
* Test the function to create a normal 3d vector to y axis
* \sdd{WI-13933}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_X_Normal_scaled_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::X(5.0f);

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 5.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
   EXPECT_FLOAT_EQ(test_result.z, 0.0f);
}

/**
* Test the function to create a normal 3d vector to y axis
* \sdd{WI-13933}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_Y_Normal_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Y();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 1.0f);
   EXPECT_FLOAT_EQ(test_result.z, 0.0f);
}

/**
* Test the function to create a normal 3d vector to x axis
* \sdd{WI-13930}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_Y_Normal_scaled_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Y(5.0f);

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 5.0f);
   EXPECT_FLOAT_EQ(test_result.z, 0.0f);
}

/**
* Test the function to create a normal 3d vector to y axis
* \sdd{WI-13933}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_Z_Normal_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Z();

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
   EXPECT_FLOAT_EQ(test_result.z, 1.0f);
}

/**
* Test the function to create a normal 3d vector to x axis
* \sdd{WI-13930}
*/
TEST(MlVector3dHppTest, Create_3d_Vector_Z_Normal_scaled_test)
{
   /** \action call function under test */
   const Vector_3d_T test_result = ml::vector3d::Z(5.0f);

   /** \assert A vector is created*/
   EXPECT_FLOAT_EQ(test_result.x, 0.0f);
   EXPECT_FLOAT_EQ(test_result.y, 0.0f);
   EXPECT_FLOAT_EQ(test_result.z, 5.0f);
}
