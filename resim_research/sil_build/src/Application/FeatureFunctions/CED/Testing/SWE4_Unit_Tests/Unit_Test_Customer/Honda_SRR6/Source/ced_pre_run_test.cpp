/**
 * @file ced_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Honda_SRR6 CED post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41687}
 */

#include "ced_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_pre_run.c"
#include "ced_types.h"
#include "fbk_macros.h"
}

/**
 * Check that ced input is correctly initialized.
 * \uts{} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run_Init__initialize_ced_input)
{
   /** \arrange set ced input to default */
   ced_input.f_ced_enable                   = FBK_FALSE;
   ced_input.f_ced_front_mode               = FBK_TRUE;
   ced_input.f_ced_rear_mode                = FBK_FALSE;
   ced_input.threshold_timer_for_ced_enable = 2u;

   /** \action execute pre run initialization */
   Ced_Init_Input(&ced_input);

   /** \assert expect ced input to be initialized correctly. */
   EXPECT_TRUE(ced_input.f_ced_enable);
   EXPECT_TRUE(ced_input.f_ced_rear_mode);
   EXPECT_FALSE(ced_input.f_ced_front_mode);
   EXPECT_EQ(ced_input.threshold_timer_for_ced_enable, 4u);
}
