/**
 * @file ta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic TA pre run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-125720}
 */

#include "ta_pre_run_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest_pred_impl.h>


extern "C"
{
#include "fbk_macros.h"
#include "pa_shared_types.h"
#include "ta_pre_run.c"
}

/**
 * Verify if copying variables from core output and tracker is valid.
 * \uts{CSCSA-125722} \sdd{SF-8550} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Pre_Run__map_from_core_and_tracker)
{
   /** \arrange Enable the Core TA Algorithm */
   ta_input.f_ta_enable       = FBK_FALSE;
   ta_input.f_fta_enable      = FBK_FALSE;
   ta_input.f_rta_enable      = FBK_FALSE;
   ta_core_input.f_ta_enable  = FBK_TRUE;
   ta_core_input.f_fta_enable = FBK_TRUE;
   ta_core_input.f_rta_enable = FBK_TRUE;

   /** \action Check inputs values */
   Ta_Pre_Run(&ta_instance, &ta_input, &fbk_output);

   /** \assert Verify output is set to default */
   EXPECT_FALSE(ta_core_input.f_ta_enable);
   EXPECT_FALSE(ta_core_input.f_fta_enable);
   EXPECT_FALSE(ta_core_input.f_rta_enable);
   EXPECT_FLOAT_EQ(ta_core_input.alert_ttp_threshold, p_cals->k_ta_alert_lvl_1_ttp_threshold[FBK_ZERO_UINT]);
}
/**
 * Checks, if address of static object is provided.
 * \uts{CSCSA-185996} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Init_Input__test_result_values)
{
   /** \arrange Declare pointer of type Ta_BMW_Boardnet_T. */
   radar_position = LEFT_CENTER;
   /** \action Get boardnet data pointer */
   Ta_Init_Input(&ta_input, radar_position);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(ta_input.f_ta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_fta_enable, FBK_TRUE);
   EXPECT_EQ(ta_input.f_rta_enable, FBK_TRUE);
}

/**
 * Checks, if address of static object is provided.
 * \uts{CSCSA-185997} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Pre_Run_Test, Ta_Init_Input__invalid_position)
{
   /** \arrange Declare pointer of type Ta_BMW_Boardnet_T. */
   radar_position = INVALID_POSITION;
   /** \action Get boardnet data pointer */
   Ta_Init_Input(&ta_input, radar_position);

   /** \assert Check if returned pointer is equal to address of static object. */
   EXPECT_EQ(ta_input.f_ta_enable, FBK_FALSE);
   EXPECT_EQ(ta_input.f_fta_enable, FBK_FALSE);
   EXPECT_EQ(ta_input.f_rta_enable, FBK_FALSE);
}
