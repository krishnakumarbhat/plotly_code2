/**
 * @file fbk_functions_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42227}
 */

#include "fbk_functions_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_functions.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
}

/**
 * Tests the Fbk_Swap_Float function.
 * \uts{CSCSA-42366} \sdd{SF-4186} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Functions_Test, Fbk_Swap_Float__swaps_values_correctly)
{
   /** \arrange Set up test value. */
   float32_T test_value_1 = 1.0f;
   float32_T test_value_2 = 2.0f;

   /** \action Run test value on Fbk_Swap_Float function. */
   Fbk_Swap_Float(&test_value_1, &test_value_2);

   /** \assert Verify that the result is correct. */
   EXPECT_FLOAT_EQ(test_value_1, 2.0f);
   EXPECT_FLOAT_EQ(test_value_2, 1.0f);
}

/**
 * Tests the Fbk_Swap_Uint8 function.
 * \uts{CSCSA-42367} \sdd{SF-4185} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Functions_Test, Fbk_Swap_Uint8__swaps_values_correctly)
{
   /** \arrange Set up test value. */
   uint8_t test_value_1 = 1u;
   uint8_t test_value_2 = 2u;

   /** \action Run test value on Fbk_Swap_Uint8 function. */
   Fbk_Swap_Uint8(&test_value_1, &test_value_2);

   /** \assert Verify that the result is correct. */
   EXPECT_EQ(test_value_1, 2u);
   EXPECT_EQ(test_value_2, 1u);
}

/**
 * Tests if float is in given range, when extended range is disabled (no hysteresis). The value to check shall be in the normal
 * range. \uts{CSCSA-116573} \sdd{CSCSA-116572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Functions_Test, Fbk_Is_Float_In_Given_Range__check_value_inside_normal_range)
{
   /** \arrange Setup thresholds and their hystereses. */
   float32_T value                     = 5.0f;
   float32_T lower_threshold           = 4.0f;
   float32_T upper_threshold           = 6.0f;
   boolean_T f_check_extended_range    = FBK_FALSE;
   float32_T lower_threshold_extension = 1.5f;
   float32_T upper_threshold_extension = -1.5f;

   /** \action Check if value is within normal thresholds */
   boolean_T result = Fbk_Is_Float_In_Given_Range(value, lower_threshold, upper_threshold, f_check_extended_range,
                                                  lower_threshold_extension, upper_threshold_extension);

   /** \assert Verify that the result is true. */
   EXPECT_TRUE(result);
}

/**
 * Tests if float is in given range, when extended range is disabled (no hysteresis). The value to check shall be out of normal
 * range. \uts{CSCSA-116574} \sdd{CSCSA-116572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Functions_Test, Fbk_Is_Float_In_Given_Range__check_value_outside_normal_range)
{
   /** \arrange Setup thresholds and their hystereses. */
   float32_T value                     = 6.5f;
   float32_T lower_threshold           = 4.0f;
   float32_T upper_threshold           = 6.0f;
   boolean_T f_check_extended_range    = FBK_FALSE;
   float32_T lower_threshold_extension = -1.0f;
   float32_T upper_threshold_extension = 1.0f;

   /** \action Check if value is within normal thresholds */
   boolean_T result = Fbk_Is_Float_In_Given_Range(value, lower_threshold, upper_threshold, f_check_extended_range,
                                                  lower_threshold_extension, upper_threshold_extension);

   /** \assert Verify that checked value is outside thresholds. */
   EXPECT_FALSE(result);
}

/**
 * Tests if float is in given range, when extended range is enabled (with hysteresis). The value to check shall be in the extended
 * range. \uts{CSCSA-116575} \sdd{CSCSA-116572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Functions_Test, Fbk_Is_Float_In_Given_Range__check_value_inside_extended_range)
{
   /** \arrange Setup thresholds and their hystereses. */
   float32_T value                     = 6.5f;
   float32_T lower_threshold           = 4.0f;
   float32_T upper_threshold           = 6.0f;
   boolean_T f_check_extended_range    = FBK_TRUE;
   float32_T lower_threshold_extension = -1.0f;
   float32_T upper_threshold_extension = 1.0f;

   /** \action Check if value is within extended thresholds */
   boolean_T result = Fbk_Is_Float_In_Given_Range(value, lower_threshold, upper_threshold, f_check_extended_range,
                                                  lower_threshold_extension, upper_threshold_extension);

   /** \assert Verify that the result is true. */
   EXPECT_TRUE(result);
}

/**
 * Tests if float is in given range, when extended range is enabled (with hysteresis). The value to check shall be out of extended
 * range. \uts{CSCSA-116576} \sdd{CSCSA-116572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Functions_Test, Fbk_Is_Float_In_Given_Range__check_value_outside_extended_range)
{
   /** \arrange Setup thresholds and their hystereses. */
   float32_T value                     = 7.5f;
   float32_T lower_threshold           = 4.0f;
   float32_T upper_threshold           = 6.0f;
   boolean_T f_check_extended_range    = FBK_TRUE;
   float32_T lower_threshold_extension = -1.0f;
   float32_T upper_threshold_extension = 1.0f;

   /** \action Check if value is outside extended thresholds */
   boolean_T result = Fbk_Is_Float_In_Given_Range(value, lower_threshold, upper_threshold, f_check_extended_range,
                                                  lower_threshold_extension, upper_threshold_extension);

   /** \assert Verify that checked value is outside thresholds. */
   EXPECT_FALSE(result);
}
