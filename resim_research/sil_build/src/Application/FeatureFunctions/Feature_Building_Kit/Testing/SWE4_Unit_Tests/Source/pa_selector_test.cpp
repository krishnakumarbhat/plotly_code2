/**
 * @file pa_selector_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pa_selector functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42245}
 */

#include "pa_selector_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_selector.h"
}

#if !defined PA_Generic
/**
 * Check whether pointer are correctly returned for main function of selector.
 * \uts{CSCSA-42485} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pa_Selector_Test, Pa_Get_Perception_Data__returns_pointer)
{
   /** \arrange set pointers to NULL. */
   context.p_data = NULL;

   /** \action call perception data update routine. */
   Pa_Get_Perception_Data(&context);

   /** \assert check that pointers are initialized. */
   EXPECT_TRUE(context.p_data != NULL);
}
#endif
