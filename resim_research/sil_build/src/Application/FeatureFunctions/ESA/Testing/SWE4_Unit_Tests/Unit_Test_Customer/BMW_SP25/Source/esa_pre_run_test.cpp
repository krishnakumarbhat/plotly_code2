/**
 * @file esa_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum teamesa
 * @brief Test implementation for ESA pre run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "esa_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_pre_run.c"
#include "esa_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/*
 * Tests pre run initialization. Expect that default values are returned correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Pre_Run_Init__initialize_pre_run_of_function)
{
   /** \arrange Set non default values for esa input. */
   esa_input.f_esa_enabled = FBK_TRUE;

   /** \action Call initialization routine for pre run. */
   Esa_Pre_Run_Init(&esa_instance);

   /** \assert Verify structures are filled with default values. */
   EXPECT_TRUE(esa_input.f_esa_enabled);
}


/*
 * Tests pre run. Expect that in this case all flags are enabled via calibration.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Pre_Run__initialize_flags_by_cals)
{
   /** \arrange Set inputs such that flags are enabled via cal. */
   p_esa_calibration->k_esa_f_enable_via_cal = FBK_TRUE;
   p_esa_calibration->k_esa_f_enable         = FBK_TRUE;
   p_esa_core_input->f_esa_enabled           = FBK_FALSE;

   /** \action Call esa pre run. */
   Esa_Pre_Run(&esa_instance, &esa_input, &fbk_output);

   /** \assert Verify structures are filled correctly. */
   EXPECT_TRUE(p_esa_core_input->f_esa_enabled);
}


/*
 * Tests input initialization. Expect that input is set to default values.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Init_Input__valid_input_pointer)
{
   /** \arrange Set inputs to non default values. */
   esa_input.f_esa_enabled = FBK_FALSE;

   /** \action Call esa input init. */
   Esa_Init_Input(&esa_input);

   /** \assert Verify structures are filled correctly. */
   EXPECT_TRUE(esa_input.f_esa_enabled);
}
