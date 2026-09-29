/**
 * @file ta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Rivian SRR6 TA pre run tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "ta_pre_run_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
#include "ta_pre_run.c"
}


/**
 * Checks, if TA pre run is intialized correctly for ECU mounting position.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Init_Input__all_parameters_set_correctly)
{
   /** \arrange */


   /** \action Call function Ta_Init_Input with parameter ta_input and central mounting position. */
   Ta_Init_Input(&ta_input, radar_position);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(ta_input.f_ta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_fta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.ta_warntrigger_hmi, TA_RIVIAN_SRR6_WARNTRIGGER_NORMAL);
}

#ifndef NDEBUG
/**
 * Checks, if Ta_Pre_Run throws exception, when input pointer is NULL.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__p_ta_instance_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Pre_Run throws exception, when p_ta_core_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Pre_Run(NULL, &ta_input, &fbk_output); }, ".*p_ta_instance.*");
}

/**
 * Checks, if Ta_Pre_Run throws exception, when input pointer is NULL.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__ta_input_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Pre_Run throws exception, when p_ta_input is NULL pointer. */
   EXPECT_DEATH({ Ta_Pre_Run(&ta_instance, NULL, &fbk_output); }, ".*p_ta_input.*");
}

/**
 * Checks, if Ta_Pre_Run throws exception, when input pointer is NULL.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__ta_cal_pointer_not_null_is_asserted)
{
   /** \arrange */
   /** \action */
   /** \assert Check if Ta_Pre_Run throws exception, when &ta_cal is NULL pointer. */
   EXPECT_DEATH({ Ta_Pre_Run(&ta_instance, &ta_input, NULL); }, ".*p_fbk_output.*");
}
#endif // !NDEBUG


/**
 * Checks, if TA pre run works as expected,  if to the input provided specific hmi mode
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__alert_ttp_threshold_alert_trigger_normal)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */
   ta_input.ta_warntrigger_hmi = TA_RIVIAN_SRR6_WARNTRIGGER_NORMAL;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if alert_ttp_threshold is set correctly. */
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, ta_cal.k_ta_alert_lvl_1_ttp_threshold[TA_RIVIAN_SRR6_WARNTRIGGER_NORMAL]);
}

/**
 * Checks, if TA pre run works as expected,  if to the input provided specific hmi mode.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__alert_ttp_threshold_alert_trigger_early)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */
   ta_input.ta_warntrigger_hmi = TA_RIVIAN_SRR6_WARNTRIGGER_EARLY;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if alert_ttp_threshold is set correctly. */
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, ta_cal.k_ta_alert_lvl_1_ttp_threshold[TA_RIVIAN_SRR6_WARNTRIGGER_EARLY]);
}

/**
 * Checks, if TA pre run works as expected, if to the input provided specific hmi mode.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__alert_ttp_threshold_alert_trigger_late)
{
   /** \arrange Set up TA input and calibration values with valid values and pointers. */
   ta_input.ta_warntrigger_hmi = TA_RIVIAN_SRR6_WARNTRIGGER_LATE;

   /** \action Call function Ta_Pre_Run with parameter ta_input. */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Check if alert_ttp_threshold is set correctly. */
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, ta_cal.k_ta_alert_lvl_1_ttp_threshold[TA_RIVIAN_SRR6_WARNTRIGGER_LATE]);
}
