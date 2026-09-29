/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>

#include "st_vector_2d_helper.hpp"
#include "Basic_Vectors.hpp"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"


#define TOOLBOX_PRECISION    (1e-6)

struct Basic_Angles
{
   Basic_Angles()
   {
      angle_0   = Create_Exact_Angle(0.0f);
      angle_30  = Create_Exact_Angle(static_cast<float32_T>(PI) / 6.f);
      angle_45  = Create_Exact_Angle(static_cast<float32_T>(PI) / 4.f);
      angle_60  = Create_Exact_Angle(static_cast<float32_T>(PI) / 3.f);
      angle_90  = Create_Exact_Angle(static_cast<float32_T>(PI) / 2.f);
      angle_180 = Create_Exact_Angle(static_cast<float32_T>(PI));
      angle_360 = Create_Exact_Angle(static_cast<float32_T>(2.f * PI));
      angle_720 = Create_Exact_Angle(static_cast<float32_T>(4.f * PI));
   }

   /** Since the Vector_2d_Algebra shall only test vector algebra and shall not depend
    * on the variant of fast math chosen the predefined angles are calculated using
    * sinf() and cosf() always.
    * \return an Angle_T structure representing the given angle_rad */
   Angle_T Create_Exact_Angle(const float32_T angle_rad)
   {
      Angle_T angle_return;

      assert(angle_rad == angle_rad); /* assert no NaN */

      angle_return.angle = angle_rad;
      angle_return.sin   = sinf(angle_rad);
      angle_return.cos   = cosf(angle_rad);

      return angle_return;
   }

   Angle_T angle_0;   /**< angle 0 deg */
   Angle_T angle_30;  /**< angle 30 deg */
   Angle_T angle_45;  /**< angle 45 deg */
   Angle_T angle_60;  /**< angle 60 deg */
   Angle_T angle_90;  /**< angle 90 deg */
   Angle_T angle_180; /**< angle 180 deg */
   Angle_T angle_360; /**< angle 360 deg */
   Angle_T angle_720; /**< angle 720 deg */
};

class StVector2dAngleTestSuite : public ::testing::Test
{
protected:
   Basic_Angles basic_angles;
};

/**
 * Test the function to rotate a 2d vector
 * Rotating x normal by 90 deg should be y norm
 * \sdd{WI-13939}
 */
TEST_F(StVector2dAngleTestSuite, WI_15281_Vector_2d_Alg_Rotate__Rotating_X_normal_by_90_deg_should_be_Y_norm)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Rotate(&basic_angles.angle_90, &Basic_Vectors::vec_x_normal);

   /** \assert Result matches y norm */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_y_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to rotate a 2d vector
 * Rotating x normal by 360 deg should be x norm
 * \sdd{WI-13939}
 */
TEST_F(StVector2dAngleTestSuite, WI_15282_Vector_2d_Alg_Rotate__Rotating_X_normal_by_360_deg_should_be_X_norm)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Rotate(&basic_angles.angle_360, &Basic_Vectors::vec_x_normal);

   /** \assert Result matches x norm */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_x_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to rotate a 2d vector
 * Rotating a vector should not change the norm
 * \sdd{WI-13939}
 */
TEST_F(StVector2dAngleTestSuite, WI_15283_Vector_2d_Alg_Rotate__Rotating_a_vector_should_NOT_influence_the_norm)
{
   /** \action call function under test */
   Vector_2d_T rotated_vector = Vector_2d_Alg_Rotate(&basic_angles.angle_45, &Basic_Vectors::vec_third_bisectrix);

   /** \assert Norm did not change */
   EXPECT_FLOAT_EQ(Vector_2d_Alg_Abs(&rotated_vector), Vector_2d_Alg_Abs(&Basic_Vectors::vec_third_bisectrix));
}

/**
 * Test the function that creates an Angle_T from a vector
 * Testing x normal vector
 * \sdd{WI-13928}
 */
TEST_F(StVector2dAngleTestSuite, WI_15284_Vector_2d_Alg_Angle_From_Vector__x_normal)
{
   /** \action call function under test */
   const Angle_T testResult = Vector_2d_Alg_Angle_From_Vector(&Basic_Vectors::vec_x_normal);

   /** \assert Cos and sin within Angle_T match vector direction */
   EXPECT_NEAR(testResult.cos, 1.0f, TOOLBOX_PRECISION);
   EXPECT_NEAR(testResult.sin, 0.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function that creates an Angle_T from a vector
 * Testing x normal times 5
 * \sdd{WI-13928}
 */
TEST_F(StVector2dAngleTestSuite, WI_15285_Vector_2d_Alg_Angle_From_Vector__x_normal_times_5)
{
   Vector_2d_T x_five = Vector_2d_Alg_Multiply_Scalar(&Basic_Vectors::vec_x_normal, 5.0f);
   /** \action call function under test */
   const Angle_T testResult = Vector_2d_Alg_Angle_From_Vector(&x_five);

   /** \assert Cos and sin within Angle_T match vector direction */
   EXPECT_NEAR(testResult.cos, 1.0f, TOOLBOX_PRECISION);
   EXPECT_NEAR(testResult.sin, 0.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function that creates an Angle_T from a vector
 * Testing y normal
 * \sdd{WI-13928}
 */
TEST_F(StVector2dAngleTestSuite, WI_15286_Vector_2d_Alg_Angle_From_Vector__y_normal)
{
   /** \action call function under test */
   const Angle_T testResult = Vector_2d_Alg_Angle_From_Vector(&Basic_Vectors::vec_y_normal);

   /** \assert Cos and sin within Angle_T match vector direction */
   EXPECT_NEAR(testResult.cos, 0.0f, TOOLBOX_PRECISION);
   EXPECT_NEAR(testResult.sin, 1.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function that creates an Angle_T from a vector
 * Testing negative x normal
 * \sdd{WI-13928}
 */
TEST_F(StVector2dAngleTestSuite, WI_15287_Vector_2d_Alg_Angle_From_Vector__vec_neg_x_normal)
{
   /** \action call function under test */
   const Angle_T testResult = Vector_2d_Alg_Angle_From_Vector(&Basic_Vectors::vec_neg_x_normal);

   /** \assert Cos and sin within Angle_T match vector direction */
   EXPECT_NEAR(testResult.cos, -1.0f, TOOLBOX_PRECISION);
   EXPECT_NEAR(testResult.sin, 0.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function that creates an Angle_T from a vector
 * Testing negative y normal
 * \sdd{WI-13928}
 */
TEST_F(StVector2dAngleTestSuite, WI_15288_Vector_2d_Alg_Angle_From_Vector__vec_neg_y_normal)
{
   /** \action call function under test */
   const Angle_T testResult = Vector_2d_Alg_Angle_From_Vector(&Basic_Vectors::vec_neg_y_normal);

   /** \assert Cos and sin within Angle_T match vector direction */
   EXPECT_NEAR(testResult.cos, 0.0f, TOOLBOX_PRECISION);
   EXPECT_NEAR(testResult.sin, -1.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function to project a 2d vector on an angle
 * Projection on 90 deg should be zero
 * \sdd{WI-13922}
 */
TEST_F(StVector2dAngleTestSuite, WI_15289_Vector_2d_Alg_Project_On_Angle__Projection_on_90_deg_should_be_zero)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Project_On_Angle(&Basic_Vectors::vec_arbitrary1, &basic_angles.angle_90);

   /** \assert Result is zero*/
   EXPECT_NEAR(testResult, 0.0f, Vector_2d_Alg_Abs(&Basic_Vectors::vec_arbitrary1) * TOOLBOX_PRECISION);
}

/**
 * Test the function to project a 2d vector on an angle
 * Projection on 180 deg should be negative norm
 * \sdd{WI-13922}
 */
TEST_F(StVector2dAngleTestSuite, WI_15290_Vector_2d_Alg_Project_On_Angle__Projection_on_180_deg_should_be_minus_the_norm)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Project_On_Angle(&Basic_Vectors::vec_arbitrary1, &basic_angles.angle_180);

   /** \assert Result is negative norm*/
   EXPECT_FLOAT_EQ(testResult, Vector_2d_Alg_Abs(&Basic_Vectors::vec_arbitrary1) * -1.0f);
}

/**
 * Test the function to project a 2d vector on an angle
 * Projection on 360 deg should be norm
 * \sdd{WI-13922}
 */
TEST_F(StVector2dAngleTestSuite, WI_15291_Vector_2d_Alg_Project_On_Angle__Projection_on_360_deg_should_be_the_norm)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Project_On_Angle(&Basic_Vectors::vec_arbitrary1, &basic_angles.angle_360);

   /** \assert Result is norm*/
   EXPECT_FLOAT_EQ(testResult, Vector_2d_Alg_Abs(&Basic_Vectors::vec_arbitrary1));
}


/**
 * Test the function to project a 2d vector on a rotated axis
 * Projection on 90 deg should be zero
 * \sdd{WI-13916}
 */
TEST_F(StVector2dAngleTestSuite, WI_15292_Vector_2d_Alg_Project_On_Rotated_X_Axis__Projection_on_90_deg_should_be_zero)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Project_On_Rotated_X_Axis(&Basic_Vectors::vec_x_normal, &basic_angles.angle_90);

   /** \assert Result is zero */
   EXPECT_NEAR(testResult, 0.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function to project a 2d vector on a rotated axis
 * Projection on 180 deg should be negative norm
 * \sdd{WI-13916}
 */
TEST_F(StVector2dAngleTestSuite, WI_15293_Vector_2d_Alg_Project_On_Rotated_X_Axis__Projection_on_180_deg_should_be_minus_the_norm)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Project_On_Rotated_X_Axis(&Basic_Vectors::vec_x_normal, &basic_angles.angle_180);

   /** \assert Result is minus one */
   EXPECT_FLOAT_EQ(testResult, -1.0f);
}

/**
 * Test the function to project a 2d vector on a rotated axis
 * Projection on 360 deg should be norm
 * \sdd{WI-13916}
 */
TEST_F(StVector2dAngleTestSuite, WI_15294_Vector_2d_Alg_Project_On_Rotated_X_Axis__Projection_on_360_deg_should_be_the_norm)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Project_On_Rotated_X_Axis(&Basic_Vectors::vec_x_normal, &basic_angles.angle_360);

   /** \assert Result is norm */
   EXPECT_FLOAT_EQ(testResult, Vector_2d_Alg_Abs(&Basic_Vectors::vec_x_normal));
}

/**
 * Test the function to rotate a 2d vector in negative direction
 * Negative rotating x normal by 90 deg should be negative y norm
 * \sdd{WI-13940}
 */
TEST_F(StVector2dAngleTestSuite, WI_15295_Vector_2d_Alg_Rotate_Negative__Negative_rotating_X_normal_by_90_deg_should_be_negative_Y_norm)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Rotate_Negative(&basic_angles.angle_90, &Basic_Vectors::vec_x_normal);

   /** \assert Result is negative norm */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_neg_y_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to rotate a 2d vector in negative direction
 * Negative rotating x normal by 360 deg should be x norm
 * \sdd{WI-13940}
 */
TEST_F(StVector2dAngleTestSuite, WI_15296_Vector_2d_Alg_Rotate_Negative__Negative_rotating_X_normal_by_360_deg_should_be_X_norm)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Rotate_Negative(&basic_angles.angle_360, &Basic_Vectors::vec_x_normal);

   /** \assert Result is x  norm */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_x_normal, TOOLBOX_PRECISION);
}


/**
 * Test the function to compute scalar product between a 2d vector and an angle component
 * X normal projected on 360 deg should be one
 * \sdd{WI-13932}
 */
TEST_F(StVector2dAngleTestSuite, WI_16188_Vector_2d_Alg_Scalar_Product_With_Angle__X_normal_projected_on_360_deg_should_be_one)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Scalar_Product_With_Angle(&Basic_Vectors::vec_x_normal, &basic_angles.angle_360);

   /** \assert Result is one */
   EXPECT_FLOAT_EQ(testResult, 1.0f);
}

/**
 * Test the function to compute scalar product between a 2d vector and an angle component
 * X normal projected on 90 deg should be zero
 * \sdd{WI-13932}
 */
TEST_F(StVector2dAngleTestSuite, WI_16189_Vector_2d_Alg_Scalar_Product_With_Angle__X_normal_projected_on_90_deg_should_be_zero)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Scalar_Product_With_Angle(&Basic_Vectors::vec_x_normal, &basic_angles.angle_90);

   /** \assert Result is zero */
   EXPECT_NEAR(testResult, 0.0f, TOOLBOX_PRECISION);
}

/**
 * Test the function to compute scalar product between a 2d vector and an angle component
 * X normal projected on 180 deg should be minus one
 * \sdd{WI-13932}
 */
TEST_F(StVector2dAngleTestSuite, WI_16190_Vector_2d_Alg_Scalar_Product_With_Angle__X_normal_projected_on_180_deg_should_be_minus_one)
{
   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Scalar_Product_With_Angle(&Basic_Vectors::vec_x_normal, &basic_angles.angle_180);

   /** \assert Result is minus one */
   EXPECT_FLOAT_EQ(testResult, -1.0f);
}

/**
 * Test the function to compute scalar product between a 2d vector and an angle component
 * Origin projected on 90 deg should be zero
 * \sdd{WI-13932}
 */
TEST_F(StVector2dAngleTestSuite, WI_16191_Vector_2d_Alg_Scalar_Product_With_Angle__Origin_projected_on_90_deg_should_be_zero)
{
   /** \arrange create an origin  vector */
   Vector_2d_T origin = Create_2d_Vector_Origin();

   /** \action call function under test */
   const float32_T testResult = Vector_2d_Alg_Scalar_Product_With_Angle(&origin, &basic_angles.angle_90);

   /** \assert Result is zero */
   EXPECT_LE(testResult, TOOLBOX_PRECISION);
}


/**
 * Test the function to create a 2d vector unit vector with orientation to the given angle
 * Angle 90 should be y normal
 * \sdd{WI-13920}
 */
TEST_F(StVector2dAngleTestSuite, WI_15301_Vector_2d_Alg_Angle_To_Vector__Angle_90_should_be_Y_normal)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Vector(&basic_angles.angle_90);

   /** \assert Result is y normal */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_y_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a 2d vector unit vector with orientation to the given angle
 * Angle 180 should be X normal
 * \sdd{WI-13920}
 */
TEST_F(StVector2dAngleTestSuite, WI_15302_Vector_2d_Alg_Angle_To_Vector__Angle_180_should_be_neg_X_normal)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Vector(&basic_angles.angle_180);

   /** \assert Result is x normal */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_neg_x_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a 2d vector unit vector with orientation to the given angle
 * Angle 45 should be first bisectrix
 * \sdd{WI-13920}
 */
TEST_F(StVector2dAngleTestSuite, WI_15303_Vector_2d_Alg_Angle_To_Vector__Angle_45_should_be_normalized_first_bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Vector(&basic_angles.angle_45);

   /** \assert Result is first bisectrix */
   EXPECT_VECTOR_2D_NEAR(testResult, Vector_2d_Alg_Normalize_Vector(&Basic_Vectors::vec_first_bisectrix), TOOLBOX_PRECISION);
}

/**
 * Test the function to create a 2d vector unit vector with orientation to the given angle
 * Angle 0 should be angle 360
 * \sdd{WI-13920}
 */
TEST_F(StVector2dAngleTestSuite, WI_15304_Vector_2d_Alg_Angle_To_Vector__Angle_0_should_be_angle_360)
{
   /** \action call function under test with angle 0*/
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Vector(&basic_angles.angle_0);
   /** \action call function under test with angle 360 */
   const Vector_2d_T testResult2 = Vector_2d_Alg_Angle_To_Vector(&basic_angles.angle_360);

   /** \assert Results match */
   EXPECT_VECTOR_2D_NEAR(testResult, testResult2, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a unit 2d vector with 90 degree orientation
 * Angle 90 should be x normal
 * \sdd{WI-13918}
 */
TEST_F(StVector2dAngleTestSuite, WI_15305_Vector_2d_Alg_Angle_To_Perpendicular_Vector__Angle_90_should_be_neg_X_normal)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Perpendicular_Vector(&basic_angles.angle_90);

   /** \assert Results is x normal */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_neg_x_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a unit 2d vector with 90 degree orientation
 * Angle 180 should be y normal
 * \sdd{WI-13918}
 */
TEST_F(StVector2dAngleTestSuite, WI_15306_Vector_2d_Alg_Angle_To_Perpendicular_Vector__Angle_180_should_be_neg_Y_normal)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Perpendicular_Vector(&basic_angles.angle_180);

   /** \assert Results is y normal */
   EXPECT_VECTOR_2D_NEAR(testResult, Basic_Vectors::vec_neg_y_normal, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a unit 2d vector with 90 degree orientation
 * Angle 45 should be second bisectrix
 * \sdd{WI-13918}
 */
TEST_F(StVector2dAngleTestSuite, WI_15307_Vector_2d_Alg_Angle_To_Perpendicular_Vector__Angle_45_should_be_normalized_2th_bisectrix)
{
   /** \action call function under test */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Perpendicular_Vector(&basic_angles.angle_45);

   /** \assert Results is second bisectrix */
   EXPECT_VECTOR_2D_NEAR(testResult, Vector_2d_Alg_Normalize_Vector(&Basic_Vectors::vec_second_bisectrix), TOOLBOX_PRECISION);
}

/**
 * Test the function to create a unit 2d vector with 90 degree orientation
 * Angle 0 should be angle 360
 * \sdd{WI-13918}
 */
TEST_F(StVector2dAngleTestSuite, WI_15308_Vector_2d_Alg_Angle_To_Perpendicular_Vector__Angle_0_should_be_angle_360)
{
   /** \action call function under test with angle 0*/
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Perpendicular_Vector(&basic_angles.angle_0);

   /** \action call function under test with angle 360*/
   const Vector_2d_T testResult2 = Vector_2d_Alg_Angle_To_Perpendicular_Vector(&basic_angles.angle_360);

   /** \assert Results match*/
   EXPECT_VECTOR_2D_NEAR(testResult, testResult2, TOOLBOX_PRECISION);
}

/**
 * Test the function to create a unit 2d vector with 90 degree orientation
 * Should be perpendicular to result of Vector_2d_Alg_Angle_To_Vector
 * \sdd{WI-13918}
 */
TEST_F(StVector2dAngleTestSuite, WI_15309_Vector_2d_Alg_Angle_To_Perpendicular_Vector__Should_be_perpendicular_to_Vector_2d_Alg_Angle_To_Vector)
{
   /** \arrange Create a vector using Vector_2d_Alg_Angle_To_Vector */
   const Vector_2d_T testResult = Vector_2d_Alg_Angle_To_Vector(&basic_angles.angle_45);

   /** \action call function under test */
   const Vector_2d_T testResult2 = Vector_2d_Alg_Angle_To_Perpendicular_Vector(&basic_angles.angle_45);

   /** \assert Scalar product of the two results is zero indicating that the vectors are perpendicular */
   EXPECT_FLOAT_EQ(Vector_2d_Alg_Scalar_Product(&testResult, &testResult2), 0.0f);
}

