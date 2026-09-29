/**
 * @file fbk_macros_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK macros.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42235}
 */

#include "fbk_macros_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <math.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
}

/**
 * Tests the FBK_ABS_F macro for a negative value.
 * \uts{CSCSA-42406} \sdd{SF-4125} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_ABS_F_negative_value)
{
   /** \arrange Set up test value. */
   float32_T test_value = -1.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Abs_F(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, 1.0f);
}

/**
 * Tests the FBK_ABS_F macro for a positive value.
 * \uts{CSCSA-42407} \sdd{SF-4125} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_ABS_F_positive_value)
{
   /** \arrange Set up test value. */
   float32_T test_value = 1.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Abs_F(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, 1.0f);
}

/**
 * Tests the FBK_ABS macro for a negative integer value.
 * \uts{CSCSA-42408} \sdd{SF-4125} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_ABS_negative_int_value)
{
   /** \arrange Set up test value. */
   int8_t test_value = -1;

   /** \action Run test value on FBK macro. */
   int8_t result = Fbk_Abs_F(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, 1);
}

/**
 * Tests the FBK_ABS macro for a positive integer value.
 * \uts{CSCSA-42409} \sdd{SF-4125} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_ABS_positive_int_value)
{
   /** \arrange Set up test value. */
   int8_t test_value = 1;

   /** \action Run test value on FBK macro. */
   int8_t result = Fbk_Abs_F(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, 1);
}

/**
 * Tests the FBK_ISNAN macro for a NaN value.
 * \uts{CSCSA-42410} \sdd{SF-4126} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_ISNAN_is_nan)
{
   /** \arrange Set up test value. */
   float32_T test_value = NAN;

   /** \action Run test value on FBK macro. */
   boolean_T result = Fbk_Is_Nan(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_TRUE(result);
}

/**
 * Tests the FBK_ISNAN macro for a normal value.
 * \uts{CSCSA-42411} \sdd{SF-4126} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_ISNAN_is_not_nan)
{
   /** \arrange Set up test value. */
   float32_T test_value = 1.0f;

   /** \action Run test value on FBK macro. */
   boolean_T result = Fbk_Is_Nan(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FALSE(result);
}


/**
 * Tests the FBK_MIN macro.
 * \uts{CSCSA-42412} \sdd{SF-4123} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_MIN_returns_correct_value)
{
   /** \arrange Set up test value. */
   float32_T test_value_1 = 1.0f;
   float32_T test_value_2 = 2.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Min(test_value_1, test_value_2);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, test_value_1);
}


/**
 * Tests the FBK_MAX macro.
 * \uts{CSCSA-42413} \sdd{SF-4124} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_MAX_returns_correct_value)
{
   /** \arrange Set up test value. */
   float32_T test_value_1 = 1.0f;
   float32_T test_value_2 = 2.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Max(test_value_1, test_value_2);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, test_value_2);
}


/**
 * Tests the FBK_CLAMP macro.
 * \uts{CSCSA-42414} \sdd{SF-4117} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_CLAMP_returns_correct_value_upper_bound)
{
   /** \arrange Set up test value. */
   float32_T test_value = 2.5f;
   float32_T min_value  = 1.0f;
   float32_T max_value  = 2.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Clamp(test_value, min_value, max_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, max_value);
}

/**
 * Tests the FBK_CLAMP macro.
 * \uts{CSCSA-42415} \sdd{SF-4117} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_CLAMP_returns_correct_value_lower_bound)
{
   /** \arrange Set up test value. */
   float32_T test_value = 0.5f;
   float32_T min_value  = 1.0f;
   float32_T max_value  = 2.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Clamp(test_value, min_value, max_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, min_value);
}


/**
 * Tests the FBK_HALF macro.
 * \uts{CSCSA-42416} \sdd{SF-4119} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_HALF_returns_correct_value)
{
   /** \arrange Set up test value. */
   float32_T test_value = 1.0f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Half(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, test_value / 2.0f);
}


/**
 * Tests the FBK_SIGN macro for negative value.
 * \uts{CSCSA-42417} \sdd{SF-4156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_SIGN_detects_negative_value)
{
   /** \arrange Set up test value. */
   float32_T test_value = -1.0f;

   /** \action Run test value on FBK macro. */
   int8_t result = Fbk_Sign(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_EQ(result, -1);
}

/**
 * Tests the FBK_SIGN macro for zero value.
 * \uts{CSCSA-42418} \sdd{SF-4156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_SIGN_detects_zero_value)
{
   /** \arrange Set up test value. */
   float32_T test_value = 0.0f;

   /** \action Run test value on FBK macro. */
   int8_t result = Fbk_Sign(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_EQ(result, 0);
}

/**
 * Tests the FBK_SIGN macro for zero value.
 * \uts{CSCSA-42419} \sdd{SF-4156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_SIGN_detects_positive_value)
{
   /** \arrange Set up test value. */
   float32_T test_value = 1.0f;

   /** \action Run test value on FBK macro. */
   int8_t result = Fbk_Sign(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_EQ(result, 1);
}


/**
 * Tests the Fbk_Convert_Bool_To_Uint macro.
 * \uts{CSCSA-42420} \sdd{SF-4114} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__Fbk_Convert_Bool_To_Uint_converts_to_uint)
{
   /** \arrange Set up test value. */
   boolean_T test_value = FBK_TRUE;

   /** \action Run test value on FBK macro. */
   uint8_t result = Fbk_Convert_Bool_To_Uint(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_EQ(result, 1u);
}


/**
 * Tests the FBK_DEG2RAD macro.
 * \uts{CSCSA-42421} \sdd{SF-4114} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_DEG2RAD)
{
   /** \arrange Set up test value. */
   float32_T test_value = 90;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Deg_To_Rad(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, 1.570796f);
}


/**
 * Tests the FBK_RAD2DEG macro.
 * \uts{CSCSA-42422} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Macros_Test, Fbk_Macros__FBK_RAD2DEG)
{
   /** \arrange Set up test value. */
   float32_T test_value = 1.570796f;

   /** \action Run test value on FBK macro. */
   float32_T result = Fbk_Rad_To_Deg(test_value);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(result, 90);
}
