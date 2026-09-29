/*===========================================================================*\
* Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>

#include "ml_angle.hpp"
#include "ml_vector_2d.hpp"


/**
* The unit tests for the function to create angles
* \sdd{WI-13809}
*/
TEST(StAngleHppTest, Angle__test)
{
   /** \arrange */
   const float val_1 = static_cast<float>(PI) / 3.f;

   /** \action call function under test */
   const Angle_T angle_1 = ml::angle::Angle(val_1);

   /** \assert */
   EXPECT_NEAR(angle_1.sin, sqrtf(3.f) / 2.f, EPSILON);
   EXPECT_NEAR(angle_1.cos, 1.f / 2.f, EPSILON);
}

/**
 * \sdd{WI-13809}
 */
TEST(StAngleHppTest, test_angle_more_than_2_pi)
{
   /** \arrange */
   const float val_2 = (PI / 2.f) + 2.0f*PI;

   /** \action call function under test */
   const Angle_T angle_2 = ml::angle::Angle(val_2);

   /** \assert */
   EXPECT_NEAR(angle_2.sin, 1.f, EPSILON);
   EXPECT_NEAR(angle_2.cos, -0.f, EPSILON);
}

/**
* \sdd{WI-13809}
*/
TEST(StAngleHppTest, test_angle_less_than_minus_2_pi)
{
   /** \arrange */
   const float val_2 = (PI / 2.f) - 4.0f*PI;

   /** \action call function under test */
   const Angle_T angle_2 = ml::angle::Angle(val_2);

   /** \assert */
   EXPECT_NEAR(angle_2.sin, 1.f, EPSILON);
   EXPECT_NEAR(angle_2.cos, -0.f, EPSILON);
}


/**
* To test normalizing angles a bunch of angles are created.
* - The resulting normalized angle needs to be in range of +-PI to the reference angle.
* - The resulting normalized angle needs to have the same sine and cosine as the original angle
*/
class Normalize_AngleHppFixture : public ::testing::TestWithParam<std::tuple<float, float, float, float>>
{};

INSTANTIATE_TEST_SUITE_P(
   values,
   Normalize_AngleHppFixture,
   ::testing::Combine
   (
      testing::Range(-PI, PI, 0.25f*PI), /* reference angle */
      testing::Range(-PI, PI, 0.25f*PI), /* angle to be normalized */
      testing::Values(-1000.0f, -10.0f, 0.0f, 10.0f, 1000.0f), /* Offset to add to angle to be normalized */
      testing::Values(-1000.0f, -10.0f, 0.0f, 10.0f, 1000.0f)  /* Offset to add to reference angle */
   )
);

/**
 * test adding two angles, second angle in form of a float
 */
TEST(AngleHpp, addition_float)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action add the angle to itself */
   a = a + 1.0f;

   /** \assert all entries in Angle_T match to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
* test adding two angles, angle and float variant
*/
TEST(AngleHpp, addition_float_reverse)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action add the angle to itself in form of a float */
   float32_T t = 1.0f + a;

   /** \assert Result matches to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(t, result.angle);
}

/**
* test adding a float to an angle while assigning
*/
TEST(AngleHpp, addition_float_assigning)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action add the angle to itself in form of a float while assigning */
   a += 1.0f;

   /** \assert all entries in Angle_T match to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
* test adding an angle to a float while assigning
*/
TEST(AngleHpp, addition_float_assigning_reverse)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);
   Angle_T result = ml::angle::Angle(2.0f);

   /** \action add the angle to a float while assigning */
   float t = 1.0f;
   t += a;

   /** \assert resulting float matches expected angle */
   EXPECT_EQ(t, result.angle);
}

/**
* test adding two angles
*/
TEST(AngleHpp, addition_angle)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action add the angle to itself */
   a = a + a;

   /** \assert all entries in Angle_T match to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
* test adding two angles while assigning
*/
TEST(AngleHpp, addition_angle_assigning)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action add the angle to itself while assigning */
   a += a;

   /** \assert all entries in Angle_T match to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
 * Rotate an angle by a float
 */
TEST(AngleHpp, Rotate_float)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action call function under test */
   a = Rotate(a, 1.0f);

   /** \assert all entries in Angle_T match to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
* Rotate an angle by an angle
*/
TEST(AngleHpp, Rotate_angle)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action call function under test */
   a = Rotate(a, a);

   /** \assert all entries in Angle_T match to the sum angle */
   Angle_T result = ml::angle::Angle(2.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
 * Subtract an angle from an angle
 */
TEST(AngleHpp, subtraction)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action call function under test */
   a = a - 1.0f;

   /** \assert all entries in Angle_T match to the difference angle */
   Angle_T result = ml::angle::Angle(0.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
*/
TEST(AngleHpp, subtraction_reverse)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action call function under test */
   float r = 1.0f - a;

   EXPECT_EQ(r, 0.0f);
}

/**
* Subtract a float from an angle while assigning
*/
TEST(AngleHpp, subtraction_assigning)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   /** \action call function under test */
   a -= 1.0f;

   /** \assert all entries in Angle_T match to the difference angle */
   Angle_T result = ml::angle::Angle(0.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}

/**
* Subtract an angle from a float while assigning
*/
TEST(AngleHpp, subtraction_assigning_float)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);
   float t = 1.0f;

   /** \action call function under test */
   t -= a;

   /** \assert result matches to the difference angle */
   EXPECT_EQ(t, 0.0f);
}

/**
* Subtract an angle from aan angle while assigning
*/
TEST(AngleHpp, subtraction_assigning_angle)
{
   /** \arrange pick an arbitrary angle */
   Angle_T a = ml::angle::Angle(1.0f);

   Angle_T b = a;
   /** \action call function under test */
   a -= b;

   /** \assert all entries in Angle_T match to the difference angle */
   Angle_T result = ml::angle::Angle(0.0f);
   EXPECT_EQ(a.angle, result.angle);
   EXPECT_EQ(a.sin, result.sin);
   EXPECT_EQ(a.cos, result.cos);
}


/**
* testing the function Normalize_Angle_Struct
* \sdd{WI-13807}
*/
TEST_P(Normalize_AngleHppFixture, do_match_angle_struct)
{
   /** \arrange prepare the values to be tested */
   float tolerance;
   float offset_for_reference_angle = std::get<3>(GetParam());
   float offset_for_angle_to_be_normalized = std::get<2>(GetParam());

   /** \arrange Apply the offset to both angles */
   float angle_to_be_normalized = std::get<0>(GetParam()) + offset_for_angle_to_be_normalized;
   float reference_angle = std::get<1>(GetParam()) + offset_for_reference_angle;

   /** \arrange Derive a tolerance from the angle offsets */
   float max_offset = Max(fabsf(offset_for_reference_angle), fabsf(offset_for_angle_to_be_normalized));
   max_offset = Max(1.0f, max_offset);
   tolerance = fabsf(max_offset * 2.5f * 1e-6f);

   Angle_T angle;
   Angle_T angle_ref;

   angle_ref = ml::angle::Angle(reference_angle);
   angle = ml::angle::Angle(angle_to_be_normalized);
   /** \action call function under test */
   angle = Normalize(angle, angle_ref);

   float normalized_angle = angle.angle;

   /** \assert that sine and cosine did not change (too much) when compared to the original angle */
   EXPECT_NEAR(sinf(angle_to_be_normalized), sinf(normalized_angle), tolerance);
   EXPECT_NEAR(cosf(angle_to_be_normalized), cosf(normalized_angle), tolerance);

   /** \assert that the normalized angle is in the range of +-PI to the reference angle.
   * For higher differences between angle_to_be_normalized and reference_angle
   * the normalized_angle may be slightly outside the +-PI range */
   EXPECT_LE(reference_angle - PI - tolerance, normalized_angle);
   EXPECT_GE(reference_angle + PI + tolerance, normalized_angle);
}


/**
* The unit tests for the function to compute the difference between two angles
* \sdd{WI-13813}
*/
TEST(AngleHpp, Angle_Diff__difference_0)
{
   /** \arrange */
   Angle_T angle_a;
   Angle_T angle_b;
   angle_a = ml::angle::Angle(1.0f);
   angle_b = ml::angle::Angle(1.0f);
   /** \action call function under test */
   Angle_T result = angle_a - angle_b;
   /** \assert */
   EXPECT_EQ(0.0f, result.angle);
}

/**
* \sdd{WI-13813}
*/
TEST(AngleHpp, Angle_Diff__difference_positive)
{
   /** \arrange */
   Angle_T angle_a;
   Angle_T angle_b;
   angle_a = ml::angle::Angle(2.0f);
   angle_b = ml::angle::Angle(1.0f);
   /** \action call function under test */
   Angle_T result = angle_a - angle_b;
   /** \assert */
   EXPECT_LT(result.angle - 1.0f, EPSILON);
}

/**
* \sdd{WI-13813}
*/
TEST(AngleHpp, Angle_Diff__difference_negative)
{
   /** \arrange */
   Angle_T angle_a;
   Angle_T angle_b;
   angle_a = ml::angle::Angle(1.0f);
   angle_b = ml::angle::Angle(2.0f);
   /** \action call function under test */
   Angle_T result = angle_a - angle_b;
   /** \assert */
   EXPECT_LT(result.angle + 1.0f, EPSILON);
}

class AngleHpp_Mean_param_test :
   public::testing::TestWithParam<std::tuple<float, float>>
{};

INSTANTIATE_TEST_SUITE_P(Angle_Mean_Test,
   AngleHpp_Mean_param_test,
   ::testing::Combine
   (
      ::testing::Range(-PI, PI, (45.0f / 180.0f)*PI),
      ::testing::Range(-PI, PI, (45.0f / 180.0f)*PI)
   )
);

/*
* \sdd{WI-13812}
*/
TEST_P(AngleHpp_Mean_param_test, Angle_Mean__combine_different_angles)
{
   /** \arrange */
   Angle_T angle_a = ml::angle::Angle(std::get<0>(GetParam()));
   Angle_T angle_b = ml::angle::Angle(std::get<1>(GetParam()));

   /** \action call function under test */
   Angle_T result_angle = Middle(angle_a, angle_b);

   /** \arrange Calculate Expected Result */
   Vector_2d_T cartesian_angle_a = ml::vector2d::Vector(angle_a.cos, angle_a.sin);
   Vector_2d_T cartesian_angle_b = ml::vector2d::Vector(angle_b.cos, angle_b.sin);
   Vector_2d_T vec_mean = Middle(cartesian_angle_a, cartesian_angle_b);
   Angle_T expected_angle = ml::angle::Angle(Normalize_Angle(Fast_Atan2(vec_mean.y, vec_mean.x), 0.0f));

   /** \assert */
   Angle_T diff_angle = angle_a - angle_b;
   if (Abs(Abs(diff_angle.angle) - PI) < EPSILON)
   {
      /* If angles have a difference of 180 degrees, there are two solutions for mean vector possible.
      In both cases the difference between a single input angle and the mean angle must be
      90 degrees
      */
      Angle_T diff_angle_result_to_angle_a = angle_a -result_angle;
      Angle_T diff_angle_result_to_angle_b = angle_b - result_angle;
      EXPECT_NEAR(Abs(diff_angle_result_to_angle_a.angle), 0.5f*PI, (0.1f / 180.0f)*PI);
      EXPECT_NEAR(Abs(diff_angle_result_to_angle_b.angle), 0.5f*PI, (0.1f / 180.0f)*PI);
   }
   else
   {
      expected_angle.angle = Normalize_Angle(expected_angle.angle, result_angle.angle);
      EXPECT_NEAR(result_angle.angle, expected_angle.angle, (0.1f / 180.0f)*PI);
   }


}