/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>
#include "ml_angle.h"
#include "ml_bool.h"
#include "ml_angle_range.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_angle_range_fuse_state_t.h"
#include "ml_angle_range_t.h"
#include "ml_angle_t.h"
#include "ml_overlapping_angle_range_t.h"
#include "ml_math_infinity_silent.h"


/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class AngleRangeTestFixture : public ::testing::Test
{
public:
   Angle_Range_T m_positive_angle_range = {
      /* .start= */ (-PI / 2),
      /* .end= */ (PI / 2)
      };

   float32_T m_angle_within_pos_range  = (0.f);;
   float32_T m_angle_outside_pos_range = (PI);;

   Angle_Range_T m_negative_angle_range = {
      /* .start= */ (PI / 2),
      /* .end= */ (-PI / 2)
   };

   float32_T m_angle_within_neg_range  = (PI);;
   float32_T m_angle_outside_neg_range = (0.f);;

   float32_T m_angle_non_norm_pos = (8 * PI);;
   float32_T m_angle_non_norm_neg = (-8 * PI);;

   Angle_Range_T m_angle_range_start_not_normalized_pos = {
      /* .start= */ (8 * PI),
      /* .end= */ (0)
   };
   Angle_Range_T m_angle_range_end_not_normalized_pos = {
      /* .start   = */ (0),
      /* .end     = */ (8 * PI)
   };
   Angle_Range_T m_angle_range_start_not_normalized_neg = {
      /* .start= */ (-8 * PI),
      /* .end= */ (0)
   };
   Angle_Range_T m_angle_range_end_not_normalized_neg = {
      /* .start   = */(0),
      /* .end     = */(-8 * PI)
   };
};

class AngleRangeTestFixtureParam : public ::testing::TestWithParam<int>
{
public:
   Angle_Range_T m_overlapping_ranges_variants_a;
   Angle_Range_T m_overlapping_ranges_variants_b;
   Overlapping_Angle_Range_T m_overlapping_ranges_variants_result;
   Angle_Range_T m_fused_range_result;
   Angle_Range_Fuse_State_T m_fused_range_state_result;
   bool m_f_fused_angle_start_matters = true;
   bool m_f_fused_angle_end_matters = true;
   bool m_f_does_overlap = true;
   AngleRangeTestFixtureParam():
      m_overlapping_ranges_variants_a{},
      m_overlapping_ranges_variants_b{},
      m_overlapping_ranges_variants_result{},
      m_fused_range_result{},
      m_fused_range_state_result{}
   {
      uint32_t param = GetParam();

      if (0 == param)
      {
         m_overlapping_ranges_variants_a.start = (0.f);
         m_overlapping_ranges_variants_a.end   = (PI / 2); /* |------|      */
         m_overlapping_ranges_variants_b.start = (PI / 3); /*      |------| */
         m_overlapping_ranges_variants_b.end   = (2 * PI / 3);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_a.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;
         m_fused_range_result.start                = m_overlapping_ranges_variants_a.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_b.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = FALSE;
         m_fused_range_state_result.f_end_from_b   = TRUE;
         m_fused_range_state_result.f_start_from_a = TRUE;
         m_fused_range_state_result.f_start_from_b = FALSE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = true;
      }

      if (1 == param)
      {
         m_overlapping_ranges_variants_a.start = (PI / 3);
         m_overlapping_ranges_variants_a.end   = (2 * PI / 3); /*      |------| */
         m_overlapping_ranges_variants_b.start = (0.f);        /* |------| */
         m_overlapping_ranges_variants_b.end   = (PI / 2);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_a.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_b.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;
         m_fused_range_result.start                = m_overlapping_ranges_variants_b.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_a.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = TRUE;
         m_fused_range_state_result.f_end_from_b   = FALSE;
         m_fused_range_state_result.f_start_from_a = FALSE;
         m_fused_range_state_result.f_start_from_b = TRUE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = true;
      }

      if (2 == param)
      {
         m_overlapping_ranges_variants_a.start = (PI / 3);
         m_overlapping_ranges_variants_a.end   = (-1 * PI / 3); /* ---|       |--- */
         m_overlapping_ranges_variants_b.start = (-2 * PI / 3); /*  |-----------| */
         m_overlapping_ranges_variants_b.end   = (2 * PI / 3);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_a.end;
         m_overlapping_ranges_variants_result.overlapping_ranges[1].start = m_overlapping_ranges_variants_a.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[1].end   = m_overlapping_ranges_variants_b.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 2;

         m_fused_range_result.start                = -PI;
         m_fused_range_result.end                  = PI;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = TRUE;
         m_fused_range_state_result.f_end_from_a   = FALSE;
         m_fused_range_state_result.f_end_from_b   = FALSE;
         m_fused_range_state_result.f_start_from_a = FALSE;
         m_fused_range_state_result.f_start_from_b = FALSE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = true;
      }

      if (3 == param)
      {
         m_overlapping_ranges_variants_a.start = (0.f);
         m_overlapping_ranges_variants_a.end   = (3 * PI / 4); /* |------| */
         m_overlapping_ranges_variants_b.start = (PI / 3);     /*   |--| */
         m_overlapping_ranges_variants_b.end   = (2 * PI / 3);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_b.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;

         m_fused_range_result.start                = m_overlapping_ranges_variants_a.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_a.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = TRUE;
         m_fused_range_state_result.f_end_from_b   = FALSE;
         m_fused_range_state_result.f_start_from_a = TRUE;
         m_fused_range_state_result.f_start_from_b = FALSE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = true;
      }

      if (4 == param)
      {
         m_overlapping_ranges_variants_a.start = (-PI);
         m_overlapping_ranges_variants_a.end   = (3 * PI / 4); /*|------| */
         m_overlapping_ranges_variants_b.start = (PI / 2);     /*     |--|*/
         m_overlapping_ranges_variants_b.end   = (PI);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_a.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;

         m_fused_range_result.start                = m_overlapping_ranges_variants_a.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_b.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = FALSE;
         m_fused_range_state_result.f_end_from_b   = TRUE;
         m_fused_range_state_result.f_start_from_a = TRUE;
         m_fused_range_state_result.f_start_from_b = FALSE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = true;
      }

      if (5 == param)
      {
         m_overlapping_ranges_variants_a.start = (0.f);
         m_overlapping_ranges_variants_a.end   = (3 * PI / 4);
         /*|------| */
         /*|------| */
         m_overlapping_ranges_variants_b = m_overlapping_ranges_variants_a;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_b.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;

         m_fused_range_result.start                = m_overlapping_ranges_variants_a.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_b.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = FALSE;
         m_fused_range_state_result.f_end_from_b   = TRUE;
         m_fused_range_state_result.f_start_from_a = FALSE;
         m_fused_range_state_result.f_start_from_b = TRUE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = false;
         m_f_fused_angle_end_matters   = false;
      }
      if (6 == param)
      {
         m_overlapping_ranges_variants_a.start = (-3.0f * PI / 4.0f);
         m_overlapping_ranges_variants_a.end   = (-PI / 4);
         /*|-|    */
         /*    |-| */
         m_overlapping_ranges_variants_b.start = (0);
         m_overlapping_ranges_variants_b.end   = (3.0f * PI / 4.0f);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = AS_TOOLBOX_INFINITY;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = AS_TOOLBOX_INFINITY;
         m_overlapping_ranges_variants_result.number_of_ranges            = 0;

         m_fused_range_result.start                = AS_TOOLBOX_INFINITY;
         m_fused_range_result.end                  = AS_TOOLBOX_INFINITY;
         m_fused_range_state_result.f_success      = FALSE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = FALSE;
         m_fused_range_state_result.f_end_from_b   = FALSE;
         m_fused_range_state_result.f_start_from_a = FALSE;
         m_fused_range_state_result.f_start_from_b = FALSE;
         m_f_does_overlap = false;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = true;
      }
      if (7 == param)
      {
         m_overlapping_ranges_variants_a.start = (0.f);
         m_overlapping_ranges_variants_a.end   = (3 * PI / 4); /* |------| */
         m_overlapping_ranges_variants_b.start = (PI / 3);     /*     |--| */
         m_overlapping_ranges_variants_b.end   = (3 * PI / 4);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_b.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;

         m_fused_range_result.start                = m_overlapping_ranges_variants_a.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_a.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = FALSE;
         m_fused_range_state_result.f_end_from_b   = TRUE;
         m_fused_range_state_result.f_start_from_a = TRUE;
         m_fused_range_state_result.f_start_from_b = FALSE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = true;
         m_f_fused_angle_end_matters   = false;
      }
      if (8 == param)
      {
         m_overlapping_ranges_variants_a.start = (0.f);
         m_overlapping_ranges_variants_a.end   = (3 * PI / 4); /* |------| */
         m_overlapping_ranges_variants_b.start = (0.0f);       /* |--| */
         m_overlapping_ranges_variants_b.end   = (PI / 3);
         m_overlapping_ranges_variants_result.overlapping_ranges[0].start = m_overlapping_ranges_variants_b.start;
         m_overlapping_ranges_variants_result.overlapping_ranges[0].end   = m_overlapping_ranges_variants_b.end;
         m_overlapping_ranges_variants_result.number_of_ranges            = 1;

         m_fused_range_result.start                = m_overlapping_ranges_variants_a.start;
         m_fused_range_result.end                  = m_overlapping_ranges_variants_a.end;
         m_fused_range_state_result.f_success      = TRUE;
         m_fused_range_state_result.f_two_pi       = FALSE;
         m_fused_range_state_result.f_end_from_a   = TRUE;
         m_fused_range_state_result.f_end_from_b   = FALSE;
         m_fused_range_state_result.f_start_from_a = FALSE;
         m_fused_range_state_result.f_start_from_b = TRUE;
         m_f_does_overlap = true;
         m_f_fused_angle_start_matters = false;
         m_f_fused_angle_end_matters   = true;
      }
   }
};

/**
 * \sdd{WI-13817}
 */
TEST_F(AngleRangeTestFixture, WI_16143_Is_Angle_Contained_In_Angle_Range__within_positive_range)
{
   boolean_T result = Is_Angle_Contained_In_Angle_Range(m_angle_within_pos_range, &m_positive_angle_range);

   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13817}
 */
TEST_F(AngleRangeTestFixture, WI_16144_Is_Angle_Contained_In_Angle_Range__outside_positive_range)
{
   boolean_T result = Is_Angle_Contained_In_Angle_Range(m_angle_outside_pos_range, &m_positive_angle_range);

   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13817}
 */
TEST_F(AngleRangeTestFixture, WI_16145_Is_Angle_Contained_In_Angle_Range__within_negative_range)
{
   boolean_T result = Is_Angle_Contained_In_Angle_Range(m_angle_within_neg_range, &m_negative_angle_range);

   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13817}
 */
TEST_F(AngleRangeTestFixture, WI_16146_Is_Angle_Contained_In_Angle_Range__outside_negative_range)
{
   boolean_T result = Is_Angle_Contained_In_Angle_Range(m_angle_outside_neg_range, &m_negative_angle_range);

   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13824}
 */
TEST_F(AngleRangeTestFixture, WI_16147_Create_Angle_Range__creates_angle_range)
{
   Angle_Range_T result_angle_range;
   Angle_T       angle_start = Create_Angle(m_positive_angle_range.start);
   Angle_T       angle_end   = Create_Angle(m_positive_angle_range.end);

   Create_Angle_Range(&result_angle_range, &angle_start, &angle_end);
   EXPECT_FLOAT_EQ(result_angle_range.start, m_positive_angle_range.start);
   EXPECT_FLOAT_EQ(result_angle_range.end, m_positive_angle_range.end);
}


/**
 * Creates an angle range from two given float values.
 * \sdd{WI-13823}
 */
TEST_F(AngleRangeTestFixture, WI_16148_Create_Angle_Range_From_Float__creates_angle_range)
{
   /** \arrange define new angle range to be filled */
   Angle_Range_T result_angle_range;

   /** \action call function under test with given float values */
   Create_Angle_Range_From_Float(&result_angle_range, m_positive_angle_range.start, m_positive_angle_range.end);

   /** \assert angle range start and end are filled with the specified float values */
   EXPECT_FLOAT_EQ(result_angle_range.start, m_positive_angle_range.start);
   EXPECT_FLOAT_EQ(result_angle_range.end, m_positive_angle_range.end);
}


/**
 * \sdd{WI-13824}
 */
TEST_F(AngleRangeTestFixture, WI_16149_Create_Angle_Range__creates_angle_range_from_non_normalized_positive)
{
   Angle_Range_T result_angle_range;
   Angle_Range_T result_angle_range_normalized;

   Angle_T angle_start = Create_Angle(m_angle_range_start_not_normalized_pos.start);
   Angle_T angle_end   = Create_Angle(m_angle_range_start_not_normalized_pos.end);

   Create_Angle_Range(&result_angle_range, &angle_start, &angle_end);

   float32_T angle_null = (0.f);
   result_angle_range_normalized       = m_angle_range_end_not_normalized_pos;
   result_angle_range_normalized.start = Normalize_Angle(result_angle_range_normalized.start, angle_null);
   result_angle_range_normalized.end   = Normalize_Angle(result_angle_range_normalized.end, angle_null);
   EXPECT_NEAR(result_angle_range.start, result_angle_range_normalized.start, 2.2e-6);
   EXPECT_NEAR(result_angle_range.end, result_angle_range_normalized.end, 2.2e-6);
}

/**
 * \sdd{WI-13824}
 */
TEST_F(AngleRangeTestFixture, WI_16150_Create_Angle_Range__creates_angle_range_from_non_normalized_negative)
{
   Angle_Range_T result_angle_range;
   Angle_Range_T result_angle_range_normalized;

   Angle_T angle_start = Create_Angle(m_angle_range_start_not_normalized_neg.start);
   Angle_T angle_end   = Create_Angle(m_angle_range_start_not_normalized_neg.end);

   Create_Angle_Range(&result_angle_range, &angle_start, &angle_end);

   float32_T angle_null = (0.f);
   result_angle_range_normalized       = m_angle_range_end_not_normalized_neg;
   result_angle_range_normalized.start = Normalize_Angle(result_angle_range_normalized.start, angle_null);
   result_angle_range_normalized.end   = Normalize_Angle(result_angle_range_normalized.end, angle_null);
   EXPECT_NEAR(result_angle_range.start, result_angle_range_normalized.start, 1e-6);
   EXPECT_NEAR(result_angle_range.end, result_angle_range_normalized.end, 1e-6);
}


/**
 * \sdd{WI-13827}
 */
TEST_F(AngleRangeTestFixture, WI_16151_Does_Angle_Range_Overlap_Angle_Range__non_overlapping)
{
   /** \arrange */
   Angle_Range_T angle_range_a;
   Angle_Range_T angle_range_b;

   angle_range_a.start = (-2 * PI / 3);
   angle_range_a.end   = (-1 * PI / 3);

   angle_range_b.start = (1 * PI / 3);
   angle_range_b.end   = (2 * PI / 3);
   /** \action call function under test */
   boolean_T result = Does_Angle_Range_Overlap_Angle_Range(&angle_range_a, &angle_range_b);
   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13827}
 */
TEST_F(AngleRangeTestFixture, WI_16152_Does_Angle_Range_Overlap_Angle_Range__overlapping)
{
   /** \arrange */
   Angle_Range_T angle_range_a;
   Angle_Range_T angle_range_b;

   angle_range_a.start = (-2 * PI / 3);
   angle_range_a.end   = (1 * PI / 3);

   angle_range_b.start = (2 * PI / 3);
   angle_range_b.end   = (-1 * PI / 3);
   /** \action call function under test */
   boolean_T result = Does_Angle_Range_Overlap_Angle_Range(&angle_range_a, &angle_range_b);
   /** \assert */
   EXPECT_TRUE(result);
}


/**
 * \sdd{WI-13827}
 */
TEST_F(AngleRangeTestFixture, WI_16153_Does_Angle_Range_Overlap_Angle_Range__fully_contained)
{
   /** \arrange */
   Angle_Range_T angle_range_a;
   Angle_Range_T angle_range_b;

   angle_range_a.start = (-2 * PI / 3);
   angle_range_a.end   = (2 * PI / 3);

   angle_range_b.start = (-1 * PI / 3);
   angle_range_b.end   = (1 * PI / 3);
   /** \action call function under test */
   boolean_T result = Does_Angle_Range_Overlap_Angle_Range(&angle_range_a, &angle_range_b);
   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13827}
 */
TEST_P(AngleRangeTestFixtureParam, WI_16154_Does_Angle_Range_Overlap_Angle_Range__overlapping_variants)
{
   /** \arrange */


   /** \action call function under test */
   boolean_T result = Does_Angle_Range_Overlap_Angle_Range(&m_overlapping_ranges_variants_a, &m_overlapping_ranges_variants_b);

   /** \assert */
   EXPECT_EQ(m_f_does_overlap, Is_True(result));
}

/**
 * \sdd{WI-13827}
 */
TEST_P(AngleRangeTestFixtureParam, WI_16155_Does_Angle_Range_Overlap_Angle_Range__overlapping_variants_inverse)
{
   /** \arrange */


   /** \action call function under test */
   boolean_T result = Does_Angle_Range_Overlap_Angle_Range(&m_overlapping_ranges_variants_b, &m_overlapping_ranges_variants_a);

   /** \assert */
   EXPECT_EQ(m_f_does_overlap, Is_True(result));
}


/**
 */
TEST_F(AngleRangeTestFixture, Initialize_Overlapping_Angle_Range__initializes)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;

   /** \action call function under test */
   Initialize_Overlapping_Angle_Range(&overlapping_range);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 0);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, AS_TOOLBOX_INFINITY);
   EXPECT_EQ(overlapping_range.overlapping_ranges[1].start, AS_TOOLBOX_INFINITY);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, AS_TOOLBOX_INFINITY);
   EXPECT_EQ(overlapping_range.overlapping_ranges[1].end, AS_TOOLBOX_INFINITY);
}

INSTANTIATE_TEST_SUITE_P(variants,
                         AngleRangeTestFixtureParam,
                         ::testing::Range(0, 9, 1));

/**
 * \sdd{WI-13830}
 */
TEST_P(AngleRangeTestFixtureParam, WI_16156_Get_Overlapping_Angle_Range__overlapping_variants)
{
   /** \arrange */

   Overlapping_Angle_Range_T overlapping_range;

   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &m_overlapping_ranges_variants_a, &m_overlapping_ranges_variants_b);
   /** \assert */
   EXPECT_EQ(m_overlapping_ranges_variants_result.number_of_ranges, overlapping_range.number_of_ranges);
   EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[0].start, overlapping_range.overlapping_ranges[0].start);
   EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[0].end, overlapping_range.overlapping_ranges[0].end);
   if (m_overlapping_ranges_variants_result.number_of_ranges == 2)
   {
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[1].start, overlapping_range.overlapping_ranges[1].start);
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[1].end, overlapping_range.overlapping_ranges[1].end);
   }
}

/**
 * \sdd{WI-13830}
 */
TEST_P(AngleRangeTestFixtureParam, WI_16157_Get_Overlapping_Angle_Range__overlapping_variants_inverse)
{
   /** \arrange */

   Overlapping_Angle_Range_T overlapping_range;

   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &m_overlapping_ranges_variants_b, &m_overlapping_ranges_variants_a);
   /** \assert */
   EXPECT_EQ(m_overlapping_ranges_variants_result.number_of_ranges, overlapping_range.number_of_ranges);
   if (m_overlapping_ranges_variants_result.number_of_ranges == 1)
   {
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[0].start, overlapping_range.overlapping_ranges[0].start);
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[0].end, overlapping_range.overlapping_ranges[0].end);
   }
   if (m_overlapping_ranges_variants_result.number_of_ranges == 2)
   {
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[0].start, overlapping_range.overlapping_ranges[1].start);
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[0].end, overlapping_range.overlapping_ranges[1].end);
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[1].start, overlapping_range.overlapping_ranges[0].start);
      EXPECT_EQ(m_overlapping_ranges_variants_result.overlapping_ranges[1].end, overlapping_range.overlapping_ranges[0].end);
   }
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16158_Get_Overlapping_Angle_Range__no_overlap)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (-1 * PI / 3);
   range_b.start = (1 * PI / 3);
   range_b.end   = (2 * PI / 3);
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_a, &range_b);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 0);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16159_Get_Overlapping_Angle_Range__full_overlap_two_resulting_ranges)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = (1 * PI / 3);
   range_b.end   = (-1 * PI / 3);
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_a, &range_b);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 2);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16160_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = (-1 * PI / 3);
   range_b.end   = (1 * PI / 3);
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_a, &range_b);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16161_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_identical_input)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_a, &range_a);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16162_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_size_0)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = (1 * PI / 3);
   range_b.end   = range_b.start;
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_b, &range_a);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, range_b.start);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, range_b.end);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16163_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_both_size_0)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = range_a.start;
   range_b.start = range_a.start;
   range_b.end   = range_b.start;
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_b, &range_a);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, range_b.start);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, range_b.end);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16164_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_start_matches)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = range_a.start;
   range_b.end   = (1 * PI / 3);
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_a, &range_b);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, range_b.start);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, range_b.end);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16165_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_start_matches_reverse)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = range_a.start;
   range_b.end   = (1 * PI / 3);
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_b, &range_a);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, range_b.start);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, range_b.end);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16166_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_end_matches)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = (-1 * PI / 3);
   range_b.end   = range_a.end;
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_a, &range_b);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, range_b.start);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, range_b.end);
}

/**
 * \sdd{WI-13830}
 */
TEST_F(AngleRangeTestFixture, WI_16167_Get_Overlapping_Angle_Range__full_overlap_one_resulting_range_end_matches_reverse)
{
   /** \arrange */
   Overlapping_Angle_Range_T overlapping_range;
   Angle_Range_T             range_a;
   Angle_Range_T             range_b;

   range_a.start = (-2 * PI / 3);
   range_a.end   = (2 * PI / 3);
   range_b.start = (-1 * PI / 3);
   range_b.end   = range_a.end;
   /** \action call function under test */
   Get_Overlapping_Angle_Range(&overlapping_range, &range_b, &range_a);
   /** \assert */
   EXPECT_EQ(overlapping_range.number_of_ranges, 1);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].start, range_b.start);
   EXPECT_EQ(overlapping_range.overlapping_ranges[0].end, range_b.end);
}



/**
 * \sdd{WI-13832}
 */
TEST_F(AngleRangeTestFixture, WI_16168_Get_Angle_Range_Width_Float__usual_range)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (-2 * PI / 3);
   range.end   = (-1 * PI / 3);
   /** \action call function under test */
   float32_T result = Get_Angle_Range_Width_Float(&range);
   /** \assert */
   EXPECT_EQ(result, PI / 3);
}

/**
 * \sdd{WI-13832}
 */
TEST_F(AngleRangeTestFixture, WI_16169_Get_Angle_Range_Width_Float__aliased_range)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   /** \action call function under test */
   float32_T result = Get_Angle_Range_Width_Float(&range);
   /** \assert */
   EXPECT_EQ(result, 2 * PI / 3);
}


/**
 * \sdd{WI-13831}
 */
TEST_F(AngleRangeTestFixture, WI_16170_Get_Angle_Range_Width_Angle__aliased_range)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   /** \action call function under test */
   Angle_T result = Get_Angle_Range_Width_Angle(&range);
   /** \assert */
   EXPECT_EQ(result.angle, 2 * PI / 3);
}


/**
 * \sdd{WI-13833}
 */
TEST_F(AngleRangeTestFixture, WI_16171_Swap_Angle_Range_Start_End__aliased_range)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   Angle_Range_T result = range;
   /** \action call function under test */
   Swap_Angle_Range_Start_End(&result);
   /** \assert */
   EXPECT_EQ(result.start, range.end);
   EXPECT_EQ(result.end, range.start);
}


/**
 * \sdd{WI-13820}
 */
TEST_F(AngleRangeTestFixture, WI_16172_Get_Angle_Range_End_Angle__returns_end)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   Angle_T result;
   /** \action call function under test */
   result = Get_Angle_Range_End_Angle(&range);
   /** \assert */
   EXPECT_EQ(result.angle, range.end);
}


/**
 * \sdd{WI-13829}
 */
TEST_F(AngleRangeTestFixture, WI_16173_Get_Angle_Range_Start_Angle__returns_start)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   Angle_T result;
   /** \action call function under test */
   result = Get_Angle_Range_Start_Angle(&range);
   /** \assert */
   EXPECT_EQ(result.angle, range.start);
}


/**
 * \sdd{WI-13816}
 */
TEST_F(AngleRangeTestFixture, WI_16174_Get_Angle_Range_End__returns_end)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   float32_T result;
   /** \action call function under test */
   result = Get_Angle_Range_End(&range);
   /** \assert */
   EXPECT_EQ(result, range.end);
}


/**
 * \sdd{WI-13822}
 */
TEST_F(AngleRangeTestFixture, WI_16175_Get_Angle_Range_Start__returns_start)
{
   /** \arrange */
   Angle_Range_T range;

   range.start = (2 * PI / 3);
   range.end   = (-2 * PI / 3);
   float32_T result;
   /** \action call function under test */
   result = Get_Angle_Range_Start(&range);
   /** \assert */
   EXPECT_EQ(result, range.start);
}

/**
 * The angle range start and end shall not be swapped when the reference is inside the angle range.
 * \sdd{WI-13821}
 */
TEST(Create_Angle_Range_Reference_Inside, WI_16176_reference_inside_no_swap_needed)
{
   Angle_Range_T angle_range;
   Angle_T       start;
   Angle_T       end;
   Angle_T       reference;

   /** \arrange set start, end and reference so that reference is inside the angle range */
   start     = Create_Angle(-PI / 2.0f);
   end       = Create_Angle(PI / 2.0f);
   reference = Create_Angle(0.0f);

   /** \action call function under test */
   Create_Angle_Range_Reference_Inside(
      &angle_range,
      &start,
      &end,
      &reference);

   /** \assert angle range start and end are not swapped */
   EXPECT_FLOAT_EQ(angle_range.start, start.angle);
   EXPECT_FLOAT_EQ(angle_range.end, end.angle);
}

/**
 * The angle range start and end shall be swapped when the reference is outside the angle range.
 * \sdd{WI-13821}
 */
TEST(Create_Angle_Range_Reference_Inside, WI_16177_reference_outside_swap_needed)
{
   Angle_Range_T angle_range;
   Angle_T       start;
   Angle_T       end;
   Angle_T       reference;

   /** \arrange set start, end and reference so that reference is outside the angle range */
   start     = Create_Angle(-PI / 2.0f);
   end       = Create_Angle(PI / 2.0f);
   reference = Create_Angle(3.0f * PI / 4.0f);

   /** \action call function under test */
   Create_Angle_Range_Reference_Inside(
      &angle_range,
      &start,
      &end,
      &reference);

   /** \assert angle range start and end are swapped */
   EXPECT_FLOAT_EQ(angle_range.start, end.angle);
   EXPECT_FLOAT_EQ(angle_range.end, start.angle);
}

/**
 * The angle range start and end shall not be swapped when the reference is outside the angle range.
 * \sdd{WI-13819}
 */
TEST(Create_Angle_Range_Reference_Outside, WI_16178_reference_outside_no_swap_needed)
{
   Angle_Range_T angle_range;
   Angle_T       start;
   Angle_T       end;
   Angle_T       reference;

   /** \arrange set start, end and reference so that reference is outside the angle range */
   start     = Create_Angle(-PI / 2.0f);
   end       = Create_Angle(PI / 2.0f);
   reference = Create_Angle(3.0f * PI / 4.0f);

   /** \action call function under test */
   Create_Angle_Range_Reference_Outside(
      &angle_range,
      &start,
      &end,
      &reference);

   /** \assert angle range start and end are not swapped */
   EXPECT_FLOAT_EQ(angle_range.start, start.angle);
   EXPECT_FLOAT_EQ(angle_range.end, end.angle);
}

/**
 * The angle range start and end shall be swapped when the reference is inside the angle range.
 * \sdd{WI-13819}
 */
TEST(Create_Angle_Range_Reference_Outside, WI_16179_reference_inside_swap_needed)
{
   Angle_Range_T angle_range;
   Angle_T       start;
   Angle_T       end;
   Angle_T       reference;

   /** \arrange set start, end and reference so that reference is inside the angle range */
   start     = Create_Angle(-PI / 2.0f);
   end       = Create_Angle(PI / 2.0f);
   reference = Create_Angle(0.0f);

   /** \action call function under test */
   Create_Angle_Range_Reference_Outside(
      &angle_range,
      &start,
      &end,
      &reference);

   /** \assert angle range start and end are swapped */
   EXPECT_FLOAT_EQ(angle_range.start, end.angle);
   EXPECT_FLOAT_EQ(angle_range.end, start.angle);
}

/**
 * \sdd{WI-13826}
 */
TEST(Angle_Range_Fuse, WI_16180_Angle_Range_Fuse__non_overlapping_ranges)
{
   Angle_Range_T angle_range;
   Angle_Range_T range_a;
   Angle_Range_T range_b;
   Angle_T       a_start;
   Angle_T       a_end;
   Angle_T       b_start;
   Angle_T       b_end;

   a_start = Create_Angle(-3.0f * PI / 4.0f);
   a_end   = Create_Angle(-PI / 4.0f);
   Create_Angle_Range(&range_a, &a_start, &a_end);

   b_start = Create_Angle(PI / 4.0f);
   b_end   = Create_Angle(3.0f * PI / 4.0f);
   Create_Angle_Range(&range_b, &b_start, &b_end);

   Angle_Range_Fuse_State_T ret = Angle_Range_Fuse(
      &angle_range,
      &range_a,
      &range_b
      );

   EXPECT_FALSE(ret.f_success);
   EXPECT_FALSE(ret.f_two_pi);
   EXPECT_FALSE(ret.f_end_from_a);
   EXPECT_FALSE(ret.f_end_from_b);
   EXPECT_FALSE(ret.f_start_from_a);
   EXPECT_FALSE(ret.f_start_from_b);

   Angle_Range_Fuse_State_T ret2 = Angle_Range_Fuse(
      &angle_range,
      &range_b,
      &range_a
      );

   EXPECT_FALSE(ret2.f_success);
   EXPECT_FALSE(ret2.f_two_pi);
   EXPECT_FALSE(ret2.f_end_from_a);
   EXPECT_FALSE(ret2.f_end_from_b);
   EXPECT_FALSE(ret2.f_start_from_a);
   EXPECT_FALSE(ret2.f_start_from_b);
}

/**
 * \sdd{WI-13826}
 */
TEST_P(AngleRangeTestFixtureParam, WI_16181_Angle_Range_Fuse__fuses)
{
   Angle_Range_T            angle_range;
   Angle_Range_Fuse_State_T ret = Angle_Range_Fuse(
      &angle_range,
      &m_overlapping_ranges_variants_a,
      &m_overlapping_ranges_variants_b
      );

   EXPECT_EQ(m_fused_range_state_result.f_success, ret.f_success);
   EXPECT_EQ(m_fused_range_state_result.f_two_pi, ret.f_two_pi);
   EXPECT_EQ(m_fused_range_state_result.f_end_from_a, ret.f_end_from_a);
   EXPECT_EQ(m_fused_range_state_result.f_end_from_b, ret.f_end_from_b);
   if (m_f_fused_angle_start_matters)
   {
      EXPECT_EQ(m_fused_range_state_result.f_start_from_a, ret.f_start_from_a);
   }
   if (m_f_fused_angle_end_matters)
   {
      EXPECT_EQ(m_fused_range_state_result.f_start_from_b, ret.f_start_from_b);
   }


   EXPECT_EQ(m_fused_range_result.start, angle_range.start);
   EXPECT_EQ(m_fused_range_result.end, angle_range.end);
}

/**
 * \sdd{WI-13826}
 */
TEST_P(AngleRangeTestFixtureParam, WI_16182_Angle_Range_Fuse__reversed_order)
{
   Angle_Range_T            angle_range;
   Angle_Range_Fuse_State_T ret = Angle_Range_Fuse(
      &angle_range,
      &m_overlapping_ranges_variants_b,
      &m_overlapping_ranges_variants_a
      );

   EXPECT_EQ(m_fused_range_state_result.f_success, ret.f_success);
   EXPECT_EQ(m_fused_range_state_result.f_two_pi, ret.f_two_pi);

   if (m_f_fused_angle_start_matters)
   {
      EXPECT_EQ(m_fused_range_state_result.f_start_from_b, ret.f_start_from_a);
      EXPECT_EQ(m_fused_range_state_result.f_start_from_a, ret.f_start_from_b);
   }
   if (m_f_fused_angle_end_matters)
   {
      EXPECT_EQ(m_fused_range_state_result.f_end_from_b, ret.f_end_from_a);
      EXPECT_EQ(m_fused_range_state_result.f_end_from_a, ret.f_end_from_b);
   }

   EXPECT_EQ(m_fused_range_result.start, angle_range.start);
   EXPECT_EQ(m_fused_range_result.end, angle_range.end);
}

/**
 * \sdd{WI-13826}
 */
TEST(Angle_Range_Fuse, WI_16183_Angle_Range_Fuse__result_2_pi)
{
   Angle_Range_T angle_range;
   Angle_Range_T range_a;
   Angle_Range_T range_b;
   Angle_T       a_start;
   Angle_T       a_end;
   Angle_T       b_start;
   Angle_T       b_end;

   a_start = Create_Angle(0.0f);
   a_end   = Create_Angle(PI);
   Create_Angle_Range(&range_a, &a_start, &a_end);

   b_start = Create_Angle(-PI);
   b_end   = Create_Angle(0.0f);
   Create_Angle_Range(&range_b, &b_start, &b_end);

   Angle_Range_Fuse_State_T ret = Angle_Range_Fuse(
      &angle_range,
      &range_a,
      &range_b
      );

   EXPECT_TRUE(ret.f_success);
   EXPECT_TRUE(ret.f_two_pi);
   EXPECT_FALSE(ret.f_end_from_a);
   EXPECT_FALSE(ret.f_end_from_b);
   EXPECT_FALSE(ret.f_start_from_a);
   EXPECT_FALSE(ret.f_start_from_b);
}

/**
 * \sdd{WI-13826}
 */
TEST(Angle_Range_Fuse, WI_16184_Angle_Range_Fuse__identical_ranges)
{
   Angle_Range_T angle_range;
   Angle_Range_T range_a;
   Angle_Range_T range_b;
   Angle_T       a_start;
   Angle_T       a_end;
   Angle_T       b_start;
   Angle_T       b_end;

   a_start = Create_Angle(0.0f);
   a_end   = Create_Angle(PI);
   Create_Angle_Range(&range_a, &a_start, &a_end);

   b_start = Create_Angle(0.0f);
   b_end   = Create_Angle(PI);
   Create_Angle_Range(&range_b, &b_start, &b_end);

   Angle_Range_Fuse_State_T ret = Angle_Range_Fuse(
      &angle_range,
      &range_a,
      &range_b
      );

   EXPECT_TRUE(ret.f_success);
   EXPECT_FALSE(ret.f_two_pi);
   EXPECT_TRUE(Is_True(ret.f_end_from_a) ^ Is_True(ret.f_end_from_b));
   EXPECT_TRUE(Is_True(ret.f_start_from_a) ^ Is_True(ret.f_start_from_b));
}

class GetAngleRangeCenter : public testing::TestWithParam<std::tuple<float, float> > {
protected:
   GetAngleRangeCenter():
      m_expected_center{std::get<0>(GetParam())},
      m_angle_range{
         /* .start = */ Normalize_Angle(m_expected_center - (0.5f * std::get<1>(GetParam())), 0.0f),
         /* .end   = */ Normalize_Angle(m_expected_center + (0.5f * std::get<1>(GetParam())), 0.0f)
      }
   {};

   float m_expected_center;
   Angle_Range_T m_angle_range;
};

INSTANTIATE_TEST_SUITE_P(
   combine_center_and_width,
   GetAngleRangeCenter,
   ::testing::Combine
   (
      testing::Range(-PI, PI, PI / 3.0f),       /* center */
      testing::Range(0.f, 1.9f * PI, PI / 3.0f) /* width  */
   )
   );

/**
 * Verify that the function returns the correct angle range center.
 * \sdd{WI-13818}
 */
TEST_P(GetAngleRangeCenter, WI_16185_Get_Angle_Range_Center__calculate_angle_range_center)
{
   /** \arrange set a combination of center angles and width during test instantiation */

   /** \action call function to calculate the angle range center */
   float ret = Get_Angle_Range_Center(&m_angle_range);

   /** \assert
    * the calculated angle range center shall match the expected center
    * (normalize angle if needed)
    */
   ret = Normalize_Angle(ret, m_expected_center);
   EXPECT_NEAR(m_expected_center, ret, 0.00001f);
}

