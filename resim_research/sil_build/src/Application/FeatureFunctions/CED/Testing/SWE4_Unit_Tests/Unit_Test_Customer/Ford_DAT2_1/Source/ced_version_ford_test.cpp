/**
 * @file ced_version_ford_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Ford_DAT2_1 CED versions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-73119}
 */

#include "ced_version_ford_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_types.h"
#include "ced_version_ford.c"
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
}


/**
 * Check is function returns correct Major Version.
 * \uts{CSCSA-73120} \sdd{SF-3507} \testtype{positive}
 */
TEST_F(Ced_Version_Ford_Test, Ced_Get_Ford_Sw_Major_Version_test)
{
   /** \arrange setup variables for version */
   uint16_t result;

   /** \action Call function to test */
   result = Ced_Get_Ford_Sw_Major_Version();

   /** \assert Verify that output is reset accordingly. */
   EXPECT_EQ(result, 10u);
}

/**
 * Check is function returns correct Minor Version
 * \uts{CSCSA-73121} \sdd{SF-3508} \testtype{positive}
 */
TEST_F(Ced_Version_Ford_Test, Ced_Get_Ford_Sw_Minor_Version_test)
{
   /** \arrange setup variables for version */
   uint16_t result;

   /** \action Call function to test */
   result = Ced_Get_Ford_Sw_Minor_Version();

   /** \assert Verify that output is reset accordingly. */
   EXPECT_EQ(result, 0u);
}
