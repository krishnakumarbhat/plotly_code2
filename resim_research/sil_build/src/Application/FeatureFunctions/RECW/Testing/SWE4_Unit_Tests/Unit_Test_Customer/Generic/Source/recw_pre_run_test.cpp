/**
 * @file recw_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic RECW pre run tests
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-185878}
 */

#include "recw_pre_run_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "recw_core_input_t.h"
#include "recw_input_t.h"
#include "recw_pre_run.c"
}


/**
 * Test the Recw pre run of Generic. Expect that function rnuns correctly.
 * \uts{CSCSA-185879} \sdd{SF-7971} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__works_correctly)
{
   /** \arrange Input data. */
   Recw_Init_Input(&recw_input);

   /** \action Call Generic pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correctly. */
   EXPECT_TRUE(recw_instance.core_input.f_enable_recw);
   EXPECT_TRUE(recw_instance.core_input.p_pa_data != nullptr);
}


/**
 * Test the Recw init input of Generic. Expect that function rnuns correctly.
 * \uts{CSCSA-185880} \sdd{CSCSA-196403} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Init_Input__works_correctly)
{
   /** \arrange Input data. */

   /** \action Call Generic pre run. */
   Recw_Init_Input(&recw_input);

   /** \assert Expect mapping to be done correct. */
   EXPECT_TRUE(recw_input.f_recw_enable);
}
