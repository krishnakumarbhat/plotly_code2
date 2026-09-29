/**
 * @file esa_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for ESA Generic pre run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-136084}
 */

#include "esa_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_pre_run.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
}

/**
 * Verify that Generic Esa_Input_T structure is reset to default.
 * \uts{CSCSA-136085} \sdd{CSCSA-123064} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Init_Input__initialize_esa_input)
{
   /** \arrange set esa input to default */
   esa_input.f_esa_enabled = FBK_FALSE;

   /** \action execute pre run initialization */
   Esa_Init_Input(&esa_input);

   /** \assert expect esa input to be initialized correctly. */
   EXPECT_TRUE(esa_input.f_esa_enabled);
}

/**
 * Check that mapping of Esa_Core_Input_T and generic Esa_Input_T is done correctly.
 * \uts{CSCSA-136086} \sdd{CSCSA-66558} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Pre_Run__check_mapping_between_input_and_core_input)
{
   /** \arrange set esa generic input */
   esa_input.f_esa_enabled         = FBK_TRUE;
   p_esa_core_input->f_esa_enabled = FBK_FALSE;
   p_esa_core_input->p_pa_data     = nullptr;

   /** \action execute pre run */
   Esa_Pre_Run(&esa_instance, &esa_input, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(p_esa_core_input->f_esa_enabled);
   EXPECT_TRUE(p_esa_core_input->p_pa_data != nullptr);
}

#ifndef NDEBUG
/*
 * Tests pre run initialization. Expect that exception is thrown.
 * \uts{CSCSA-185758} \sdd{CSCSA-66557} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Pre_Run_Init__invalid_instance_pointer)
{
   /** \arrange */
   /** \action Call initialization routine for pre run. */
   /** \assert Verify that exception is thrown. */
   EXPECT_DEATH({ Esa_Pre_Run_Init(NULL); }, ".*p_esa_instance.*");
}
#endif //! NDEBUG

/*
 * Tests input init. Expect that in this case all input flags are enabled.
 * \uts{CSCSA-185759} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Init_Input__valid_input_pointer)
{
   /** \arrange Set inputs such that flags are disabled. */
   esa_input.f_esa_enabled = FBK_FALSE;

   /** \action Call esa input init. */
   Esa_Init_Input(&esa_input);

   /** \assert Verify all flags enabled. */
   EXPECT_TRUE(esa_input.f_esa_enabled);
}


/*
 * Tests pre run. Expect that in this case all flags are enabled via input.
 * \uts{CSCSA-185760} \sdd{CSCSA-66558} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Pre_Run_Test, Esa_Pre_Run__initialize_flags_Active)
{
   /** \arrange Set inputs such that flags are enabled via input. */
   esa_input.f_esa_enabled         = FBK_TRUE;
   p_esa_core_input->f_esa_enabled = FBK_FALSE;

   /** \action Call esa pre run. */
   Esa_Pre_Run(&esa_instance, &esa_input, &fbk_output);

   /** \assert Verify all flags are enabled. */
   EXPECT_TRUE(p_esa_core_input->f_esa_enabled);
}
