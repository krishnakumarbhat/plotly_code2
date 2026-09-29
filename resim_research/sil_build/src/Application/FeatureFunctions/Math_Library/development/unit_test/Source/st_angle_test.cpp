/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_angle.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"



/**
* Class used to create a fixture
* For writing two or more tests that operate on similar data, it can use a test fixture
* It allows to reuse the same configuration of objects for several different tests
*/
class BasicMathTestFixture : public ::testing::Test
{
public:

   BasicMathTestFixture() = default;

protected:
   float32_T m_in_range_float { 0.69f};      /* in range */
   float32_T m_range_min_float { 0.36f};     /* range min border */
   float32_T m_range_max_float { 6.75f};     /* range max border */
   float32_T m_out_of_range_float1 { 7.06f}; /* outside range (min side) */
   float32_T m_out_of_range_float2 { 0.09f}; /* outside range (max side) */
   float32_T m_in_range_int { 4};            /* range min border */
   float32_T m_range_min_int { 3};           /* in range */
   float32_T m_range_max_int { 7};           /* range max border */
   float32_T m_out_of_range_int1 { 1};       /* outside range (min side) */
   float32_T m_out_of_range_int2 { 13};      /* outside range (max side) */
};

/**
* The unit tests for the function to create angles
* \sdd{WI-13809}
*/
TEST(BasicMathFactoryTest, WI_14849_Create_Angle__test)
{
   /** \arrange */
   const float val_1 = static_cast<float>(PI) / 3.f;

   /** \action call function under test */
   const Angle_T angle_1 = Create_Angle(val_1);

   /** \assert */
   EXPECT_NEAR(angle_1.sin, sqrtf(3.f) / 2.f, 0.001f);
   EXPECT_NEAR(angle_1.cos, 1.f / 2.f, 0.001f);
}

/**
 * \sdd{WI-13809}
 */
TEST(BasicMathFactoryTest, WI_14850_Create_Angle__test_angle_more_than_2_pi)
{
   /** \arrange */
   const float val_2 = (PI / 2.f) + 2.0f*PI;

   /** \action call function under test */
   const Angle_T angle_2 = Create_Angle(val_2);

   /** \assert */
   EXPECT_NEAR(angle_2.sin, 1.f, 0.001f);
   EXPECT_NEAR(angle_2.cos, -0.f, 0.001f);
}

/**
* \sdd{WI-13809}
*/
TEST(BasicMathFactoryTest, WI_14851_Create_Angle__test_angle_less_than_minus_2_pi)
{
   /** \arrange */
   const float val_2 = (PI / 2.f) - 4.0f*PI;

   /** \action call function under test */
   const Angle_T angle_2 = Create_Angle(val_2);

   /** \assert */
   EXPECT_NEAR(angle_2.sin, 1.f, 0.001f);
   EXPECT_NEAR(angle_2.cos, -0.f, 0.001f);
}


/**
* The unit tests for the function to normalize angles
* \sdd{WI-13810}
*/
TEST_F(BasicMathTestFixture, WI_14852_Normalize_Angle__normalize_higher_angle)
{
   /** \arrange */
   float32_T angle_to_be_normalized = 1.2f + 3.f * static_cast<float32_T>(PI);
   float32_T ref_angle = -5.433f;

   /** \action call function under test */
   float32_T normalized_angle = Normalize_Angle(angle_to_be_normalized, ref_angle);

   /** \assert */
   EXPECT_LE(ref_angle - (static_cast<float32_T>(PI)), normalized_angle);
   EXPECT_GE(ref_angle + (static_cast<float32_T>(PI)), normalized_angle);
}

/**
* The unit tests for the function to normalize angles. Call FUT with nan.
* \sdd{WI-13810}
*/
TEST(Normalize_Angle, WI_14853_Normalize_Angle__nan0)
{
   /** \arrange */
   float32_T normalized_angle;
   /** \action call function under test */
   EXPECT_DEBUG_DEATH(
      normalized_angle = Normalize_Angle(std::numeric_limits<float32_T>::quiet_NaN(), 0.123f),
      "theta_in");
#ifdef NDEBUG
   /** \assert */
   EXPECT_NE(normalized_angle, normalized_angle);
#endif
}

/**
* The unit tests for the function to normalize angles. Call FUT with nan.
* \sdd{WI-13810}
*/
TEST(Normalize_Angle, WI_14854_Normalize_Angle__nan1)
{
   /** \arrange */
   float32_T normalized_angle;
   /** \action call function under test */
   EXPECT_DEBUG_DEATH(
      normalized_angle = Normalize_Angle(0.123f, std::numeric_limits<float32_T>::quiet_NaN()),
      "theta_ref");
#ifdef NDEBUG
   /** \assert */
   EXPECT_NE(normalized_angle, normalized_angle);
#endif
}

/**
* The unit tests for the function to normalize angles
* \sdd{WI-13810}
*/
TEST_F(BasicMathTestFixture, WI_14855_Normalize_Angle__normalize_higher_angle_really_high_value)
{
   /** \arrange */
   float32_T angle_to_be_normalized = 1000.0f * (1.2f + 3.f * static_cast<float32_T>(PI));
   float32_T ref_angle = -5.433f;

   /** \action call function under test */
   float32_T normalized_angle = Normalize_Angle(angle_to_be_normalized, ref_angle);

   /** \assert */
   EXPECT_LE(ref_angle - (static_cast<float32_T>(PI)), normalized_angle);
   EXPECT_GE(ref_angle + (static_cast<float32_T>(PI)), normalized_angle);
}


/**
* To test normalizing angles a bunch of angles are created.
* - The resulting normalized angle needs to be in range of +-PI to the reference angle.
* - The resulting normalized angle needs to have the same sine and cosine as the original angle
*/
class NormalizeAngleFixture : public ::testing::TestWithParam<std::tuple<float, float, float, float>>
{};

INSTANTIATE_TEST_SUITE_P(
   values,
   NormalizeAngleFixture,
   ::testing::Combine
   (
      testing::Range(-PI, PI, 0.25f*PI), /* reference angle */
      testing::Range(-PI, PI, 0.25f*PI), /* angle to be normalized */
      testing::Values(-1000.0f, -10.0f, 0.0f, 10.0f, 1000.0f), /* Offset to add to angle to be normalized */
      testing::Values(-1000.0f, -10.0f, 0.0f, 10.0f, 1000.0f)  /* Offset to add to reference angle */
   )
);

/**
* testing the function Normalize_Angle
* \sdd{WI-13810}
*/
TEST_P(NormalizeAngleFixture, WI_14856_do_match_float_angle)
{
   /* Arrange prepare the values to be tested */
   float tolerance;
   float offset_for_reference_angle = std::get<3>(GetParam());
   float offset_for_angle_to_be_normalized = std::get<2>(GetParam());

   /* Apply the offset to both angles */
   float angle_to_be_normalized = std::get<0>(GetParam()) + offset_for_angle_to_be_normalized;
   float reference_angle = std::get<1>(GetParam()) + offset_for_reference_angle;

   /* Derive a tolerance from the angle offsets */
   float max_offset = Max(fabsf(offset_for_reference_angle), fabsf(offset_for_angle_to_be_normalized));
   max_offset = Max(1.0f, max_offset);
   tolerance = fabsf(max_offset * 2.5f * 1e-6f);
   /** Action */
   float32_T normalized_angle = Normalize_Angle(angle_to_be_normalized, reference_angle);

   /** Assert that sine and cosine did not change (too much) when compared to the original angle */
   EXPECT_NEAR(sinf(angle_to_be_normalized), sinf(normalized_angle), tolerance);
   EXPECT_NEAR(cosf(angle_to_be_normalized), cosf(normalized_angle), tolerance);

   /** Assert that the normalized angle is in the range of +-PI to the reference angle.
   * For higher differences between angle_to_be_normalized and reference_angle
   * the normalized_angle may be slightly outside the +-PI range */
   EXPECT_LE(reference_angle - PI - tolerance, normalized_angle);
   EXPECT_GE(reference_angle + PI + tolerance, normalized_angle);
}

/**
* testing the function Normalize_Angle_Struct
* \sdd{WI-13807}
*/
TEST_P(NormalizeAngleFixture, WI_14857_do_match_angle_struct)
{
   /* Arrange prepare the values to be tested */
   float tolerance;
   float offset_for_reference_angle = std::get<3>(GetParam());
   float offset_for_angle_to_be_normalized = std::get<2>(GetParam());

   /* Apply the offset to both angles */
   float angle_to_be_normalized = std::get<0>(GetParam()) + offset_for_angle_to_be_normalized;
   float reference_angle = std::get<1>(GetParam()) + offset_for_reference_angle;

   /* Derive a tolerance from the angle offsets */
   float max_offset = Max(fabsf(offset_for_reference_angle), fabsf(offset_for_angle_to_be_normalized));
   max_offset = Max(1.0f, max_offset);
   tolerance = fabsf(max_offset * 2.5f * 1e-6f);

   Angle_T angle;
   Angle_T angle_ref;

   angle_ref = Create_Angle(reference_angle);
   angle = Create_Angle(angle_to_be_normalized);
   /** \action call function under test */
   Normalize_Angle_Struct(&angle, &angle_ref);

   float normalized_angle = angle.angle;

   /** Assert that sine and cosine did not change (too much) when compared to the original angle */
   EXPECT_NEAR(sinf(angle_to_be_normalized), sinf(normalized_angle), tolerance);
   EXPECT_NEAR(cosf(angle_to_be_normalized), cosf(normalized_angle), tolerance);

   /** Assert that the normalized angle is in the range of +-PI to the reference angle.
   * For higher differences between angle_to_be_normalized and reference_angle
   * the normalized_angle may be slightly outside the +-PI range */
   EXPECT_LE(reference_angle - PI - tolerance, normalized_angle);
   EXPECT_GE(reference_angle + PI + tolerance, normalized_angle);
}


/**
* The unit tests for the function to compute the difference between two angles
* \sdd{WI-13813}
*/
TEST_F(BasicMathTestFixture, WI_14858_Angle_Diff__difference_0)
{
   /** \arrange */
   Angle_T angle_a;
   Angle_T angle_b;
   angle_a = Create_Angle(1.0f);
   angle_b = Create_Angle(1.0f);
   /** \action call function under test */
   Angle_T result = Angle_Diff(&angle_a, &angle_b);
   /** \assert */
   EXPECT_EQ(0.0f, result.angle);
}

/**
* \sdd{WI-13813}
*/
TEST_F(BasicMathTestFixture, WI_14859_Angle_Diff__difference_positive)
{
   /** \arrange */
   Angle_T angle_a;
   Angle_T angle_b;
   angle_a = Create_Angle(2.0f);
   angle_b = Create_Angle(1.0f);
   /** \action call function under test */
   Angle_T result = Angle_Diff(&angle_a, &angle_b);
   /** \assert */
   EXPECT_LT(result.angle - 1.0f, EPSILON);
}

/**
* \sdd{WI-13813}
*/
TEST_F(BasicMathTestFixture, WI_14860_Angle_Diff__difference_negative)
{
   /** \arrange */
   Angle_T angle_a;
   Angle_T angle_b;
   angle_a = Create_Angle(1.0f);
   angle_b = Create_Angle(2.0f);
   /** \action call function under test */
   Angle_T result = Angle_Diff(&angle_a, &angle_b);
   /** \assert */
   EXPECT_LT(result.angle + 1.0f, EPSILON);
}

class AngleMeanParamFixture :
   public BasicMathTestFixture,
   public::testing::WithParamInterface<std::tuple<float, float>>
{
public:

   void SetUp() override
   {
   }
protected:
};

INSTANTIATE_TEST_SUITE_P(Angle_Mean_Test,
   AngleMeanParamFixture,
   ::testing::Combine
   (
      ::testing::Range(-PI, PI, (45.0f / 180.0f)*PI),
      ::testing::Range(-PI, PI, (45.0f / 180.0f)*PI)
   )
);

/*
* \sdd{WI-13812}
*/
TEST_P(AngleMeanParamFixture, WI_14861_Angle_Mean__combine_different_angles)
{
   /** \arrange */
   Angle_T angle_a = Create_Angle(std::get<0>(GetParam()));
   Angle_T angle_b = Create_Angle(std::get<1>(GetParam()));

   /** \action call function under test */
   Angle_T result_angle = Angle_Mean(&angle_a, &angle_b);

   // Calculate Expected Result
   Vector_2d_T cartesian_angle_a = Create_2d_Vector_Coordinates(angle_a.cos, angle_a.sin);
   Vector_2d_T cartesian_angle_b = Create_2d_Vector_Coordinates(angle_b.cos, angle_b.sin);
   Vector_2d_T vec_mean = Vector_2d_Alg_Middle(&cartesian_angle_a, &cartesian_angle_b);
   Angle_T expected_angle = Create_Angle(Normalize_Angle(Fast_Atan2(vec_mean.y, vec_mean.x), 0.0f));

   /** \assert */
   Angle_T diff_angle = Angle_Diff(&angle_a, &angle_b);
   if (Abs(Abs(diff_angle.angle) - PI) < EPSILON)
   {
      /* If angles have a difference of 180 degrees, there are two solutions for mean vector possible.
      In both cases the difference between a single input angle and the mean angle must be
      90 degrees
      */
      Angle_T diff_angle_result_to_angle_a = Angle_Diff(&angle_a, &result_angle);
      Angle_T diff_angle_result_to_angle_b = Angle_Diff(&angle_b, &result_angle);
      EXPECT_NEAR(Abs(diff_angle_result_to_angle_a.angle), 0.5f*PI, (0.1f / 180.0f)*PI);
      EXPECT_NEAR(Abs(diff_angle_result_to_angle_b.angle), 0.5f*PI, (0.1f / 180.0f)*PI);
   }
   else
   {
      expected_angle.angle = Normalize_Angle(expected_angle.angle, result_angle.angle);
      EXPECT_NEAR(result_angle.angle, expected_angle.angle, (0.1f / 180.0f)*PI);
   }


}