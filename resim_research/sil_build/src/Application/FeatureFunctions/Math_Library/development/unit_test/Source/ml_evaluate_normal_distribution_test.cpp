/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_evaluate_normal_distribution.h"

#include <array>

/**
 * Test with a value matching mean with high standard deviation
 * \sdd{WI-28395}
 */
TEST(ml_evaluate_normal_distribution, value_on_mean_100)
{
    /** \action call function under test with an arbitrary mean and a matching value.
     * Use a high standard deviation
     */
    auto ret = Evaluate_Normal_Distribution(
         0.0f,
         0.0f,
       100.0f
       );
    /** \assert expect result to be small */
    EXPECT_LE(ret, 0.004f);
}

/**
 * Test with a value matching mean with low standard deviation
 * \sdd{WI-28395}
 */
TEST(ml_evaluate_normal_distribution, value_on_mean_0_1)
{
    /** \action call function under test with an arbitrary mean and a matching value.
     * Use a low standard deviation
     */
    auto ret = Evaluate_Normal_Distribution(
       0.0f,
       0.0f,
       0.1f
       );
    /** \assert expect result to be big */
    EXPECT_GE(ret, 3.0f);
}

/**
 * Test with a value outside standard deviation
 * \sdd{WI-28395}
 */
TEST(ml_evaluate_normal_distribution, value_outside_sd)
{
    /** \action call function under test with an arbitrary mean.
     * Use a low standard deviation
     * Use a value outside the standard deviation around mean
     */
    auto ret = Evaluate_Normal_Distribution(
       200.0f,
         0.0f,
       100.0f
       );
       /** \assert expect result to be really small */
    EXPECT_LE(ret, 0.0006f);
}

/**
 * Test with a value matching negative mean with high standard deviation
 * \sdd{WI-28395}
 */
TEST(ml_evaluate_normal_distribution, negative_mean)
{
    /** \action call function under test with an arbitrary negative mean.
     * Use a low standard deviation
     * Set value to be equal to mean.
     */
    auto ret = Evaluate_Normal_Distribution(
       -100.0f,
       -100.0f,
        100.0f
       );
    /** \assert expect result to be small */
    EXPECT_LE(ret, 0.004f);
}

/**
 * Test with a value matching mean with too small standard deviation
 * \sdd{WI-28395}
 */
TEST(ml_evaluate_normal_distribution, too_small_standard_deviation)
{
    float ret = 1234.0f;
    /** \action call function under test with an arbitrary negative mean.
     * Use a standard deviation of zero
     * Set value to be equal to mean.
     */
    EXPECT_DEBUG_DEATH({
        ret = Evaluate_Normal_Distribution(
            -100.0f,
            -100.0f,
               0.0f
        );
       },
       /** \assert in debug expect an assertion to be thrown */
       "standard_deviation_local"
    );
#ifdef NDEBUG
/** \assert in Release expect the return value to match a call with standard deviation of 1.0f */
    auto ref = Evaluate_Normal_Distribution(
            -100.0f,
            -100.0f,
               1.0f
        );
    EXPECT_FLOAT_EQ(ret, ref);
#endif
}