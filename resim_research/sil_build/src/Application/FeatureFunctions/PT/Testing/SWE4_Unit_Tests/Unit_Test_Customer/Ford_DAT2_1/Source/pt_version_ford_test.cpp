/**
 * @file pt_version_ford_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Ford_DAT2_1 pt versions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-73368}
 */

#include "pt_version_ford_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "pa_reuse.h"
#include "pt_types.h"
#include "pt_version_ford.c"
}


/**
 * Check that pt output is reset to its default.
 * \uts{CSCSA-73369} \sdd{SF-7648} \testtype{positive}
 */
TEST_F(Pt_Version_Ford_Test, Pt_Get_Ford_Sw_Major_Version_test)
{
   /** \arrange setup variables for version */
   uint16_t result;

   /** \action Call function to test */
   result = Pt_Get_Ford_Sw_Major_Version();

   /** \assert Verify that output is reset accordingly. */
   EXPECT_EQ(result, 6u);
}

/**
 * Check that pt output is reset to its default.
 * \uts{CSCSA-73370} \sdd{SF-7647} \testtype{positive}
 */
TEST_F(Pt_Version_Ford_Test, Pt_Get_Ford_Sw_Minor_Version_test)
{
   /** \arrange setup variables for version */
   uint16_t result;

   /** \action Call function to test */
   result = Pt_Get_Ford_Sw_Minor_Version();

   /** \assert Verify that output is reset accordingly. */
   EXPECT_EQ(result, 12u);
}
