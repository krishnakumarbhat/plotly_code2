/**
 * @file ltb_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic LTB pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-185801}
 */

#include "ltb_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ltb_pre_run.c"
#include "pa_reuse.h"
}

/**
 * Check that mapping in ltb pre run is done correctly.
 * \uts{CSCSA-185802} \sdd{CSCSA-216621} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Pre_Run_Test, Ltb_Init_Input__runs_correctly)
{
   /** \arrange set ltb input to something */
   ltb_input.f_ltb_enable = FBK_FALSE;

   /** \action execute pre run */
   Ltb_Init_Input(&ltb_input);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(ltb_input.f_ltb_enable);
}

/**
 * Check that mapping in ltb pre run is done correctly.
 * \uts{CSCSA-185803} \sdd{CSCSA-54002} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Pre_Run_Test, Ltb_Pre_Run__runs_correctly)
{
   /** \arrange set ltb input to something */
   ltb_input.f_ltb_enable = FBK_TRUE;

   /** \action execute pre run */
   Ltb_Pre_Run(&ltb_instance, &ltb_input, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(p_ltb_core_input->f_ltb_enable);
}
